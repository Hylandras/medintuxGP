/********************************* CApp.cpp ***************************************
 *                                                                                *
 *   #include CApp.h"                                                             *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat à l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Fédération, 75752 PARIS cedex 15.   *
 *                            FRANCE                                              *
 *   Centre National de la Recherche Scientifique                                 *
 *   - CNRS,                                                                      *
 *                            3 rue Michel-Ange, 75794 Paris cedex 16.            *
 *                            FRANCE                                              *
 *   Institut National de Recherche en Informatique et en Automatique             *
 *   - INRIA,                                                                     *
 *                            Domaine de Voluceau, Rocquencourt, BP 105, 78153    *
 *                            FRANCE                                              *
 *                                                                                *
 *         This program is distributed in the hope that it will be useful,        *
 *         but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                   *
 *         CeCILL  License (GPL compatible) for more details.                     *
 *                                                                                *
 **********************************************************************************/
//
//    mysqldump --quote-name --compatible=mysql323 CCAMTest | gzip > /home/ro/CCAMTest.gz
//    Les spectatrices ne veulent pas qu'on les ennuie dans la file.
#include <qdir.h>
#include <qfileinfo.h>
#include <qmessagebox.h>
#include <qtextstream.h>
#include <qsqlquery.h>
#include <qsqlcursor.h>
#include <qcursor.h>
#include "CApp.h"


CApp* G_pCApp = 0;  // contiendra l'instance globale de l'application
//--------------------------------------------- CApp -------------------------------------------------------------------
CApp::CApp(int & argc, char ** argv)
    : QApplication (  argc, argv ), CGetDatasempAppCtx(  argc, argv ), CSetupBase(), m_MySQL_DataBase(0)
{G_pCApp          = this;
}

//--------------------------------------------- CApp -------------------------------------------------------------------
CApp::~CApp()
{G_pCApp          = 0;
    if (m_MySQL_DataBase)
    {if (m_MySQL_DataBase->isOpen()) m_MySQL_DataBase->close();
        QSqlDatabase::removeDatabase (m_MySQL_DataBase);
    }
}
//--------------------------------------------- GotoDebug -------------------------------------------------------------------
int CApp::GotoDebug()
{return 2;
}

#define MAX_READ 0XFFFFFFF
//----------------------------------------------------- read_line    -------------------------------------------
QString  CApp::read_line( QFile *file, char *buffer, int &len_read, Q_ULONG maxlen)
{       len_read         = (int)file->readLine ( buffer, maxlen );
        buffer[len_read] = 0;    //  on assure le zero de fin
        int isUtf8       = CGestIni::IsUtf8  ( buffer, len_read );
        if (isUtf8)
           {return  QString::fromUtf8 ( buffer ) ;
           }
        else
           {return  QString (buffer);
           } 
}
//-----------------------------------------------------  ParseSQL_Dump -------------------------------------------
void  CApp::ParseSQL_Dump(QSqlDatabase *dataBase, const QString &fname, QProgressBar *pQProgressBar /* = 0 */, CW_EditText *logWidget /* = 0 */)
{//....................... ouvrir le fichier en mode QTextStream ...........................
    if ( !QFile::exists( fname ) )           return;
    QFile file( fname );
    if ( !file.open( IO_ReadOnly|IO_Raw  ) ) return;
    Q_ULONG maxlen        = 3512000;
    char    *buffer       = new char[maxlen+50];    // +5 pour permettre analyse utf8 qui explore trois apres
    QDateTime dtDeb       = QDateTime::currentDateTime();
    QString requete       = "";
    QString line          = "";
    QString table         = "";
    int nbErr             = 0;
    long len_line         = 0;
    int  len_read         = 0;
    if (buffer==0) {      Datasemp_OutMessage(logWidget, QObject::tr("CApp::ParseSQL_Dump() Réservation du buffer de lecture <font color=#01eaff><b>%1</b></font> octets impossible.")
                                                        .arg(QString::number(maxlen)));
                          return;
                   }
   //.......... on déclare une variable pour lire le fichier ligne à ligne ..................
    QSqlQuery query(QString::null, dataBase);
    while (!file.atEnd())
    {
        //m_position += (int)file.readLine(line,MAX_READ); // line = line.stripWhiteSpace();
        line           = read_line( &file, buffer, len_read, maxlen);
        m_position    += len_read;
        len_line       = line.length();
        if (pQProgressBar)  {pQProgressBar->setProgress(m_position/1000); qApp->processEvents();qApp->processEvents();} 
            //.................creation de la table .....................
        if ( len_line>=12 && line[0]=='C' && line[1]=='R' && line.left(12)=="CREATE TABLE")
        {
            int pos = line.find("(",12);
            if ( pos != -1 )
               {table = line.mid(12, pos-12);
                query.exec( QString("DROP TABLE IF EXISTS ")+table+";");
                OutSQL_error(query, QObject::tr("ERREUR DROP TABLE IF EXISTS : ParseSQL_Dump()"), table );
                Datasemp_OutMessage(logWidget, QObject::tr("Création de la table <font color=#01eaff><b>") + table + QObject::tr("</b></font> en cours....")) ;
               }
            requete = line;
            while (!file.atEnd())
            {   //m_position    += (int)file.readLine(line,MAX_READ);
                line           = read_line( &file, buffer, len_read, maxlen);
                m_position    += len_read;
                if (pQProgressBar)  {pQProgressBar->setProgress(m_position/1000); qApp->processEvents();qApp->processEvents();} 
                if (line[0]==')' && (line[2]=='T' || line[2]=='E' || line[1]==';'))
                   {   if (line.left(7) == ") TYPE=" || line.left(9) == ") ENGINE=" )  // ne surtout pas inclure ';' dans la comparaison
                          {requete += ");"; // TYPE=MyISAM;  passe pas sous mac;
                           requete.remove ("collate utf8_unicode_ci");
                           requete.remove ("collate utf8_bin");
                           requete.remove ("character set utf8");
                           query.exec(requete);
                           if (OutSQL_error(query, QObject::tr("ERREUR CREATE TABLE : ParseSQL_Dump()"), requete ))
                              { Datasemp_OutMessage(logWidget, QString( "<font color=#ff0000><b>ERREUR SQL CREATE TABLE: %1 \n</b></font>").arg(table) + m_LastError) ;
                              }
                           requete = "";
                           break;
                          }
                   }
               requete += line;
            }
        }
        else if ( len_line>=11 &&  line[0]=='I' && line[1]=='N' && line.left(11)=="INSERT INTO")
        {   // PB de ce */$x?! de datasemp; certaines données ont des retours chariots
            //............ isoler le verbe .................................................................
            requete            = "";
            int    pos         = 0;
            int isMultiLines   = 0;
            //..................... isoler les données à inserer de toutes les donnees INSERT............................................
            //                      en effet un INSERT INTO peut etre sur plusieurs lignes
            //                      (aller jusqu'au ');' de fin
            do      {
                       len_line = line.length();
                       pos      = len_line;
                       if (pos>2)
                          { while (line[pos-1]=='\r' || line[pos-1]=='\n')
                                  {--pos;
                                  }
                            if (line[pos-2]==')' && line[pos-1]==';')
                               { if (isMultiLines)
                                    { requete += line;
                                      query.exec(requete);
                                      if (  OutSQL_error(query, QObject::tr("\nERREUR INSERT INTO : ParseSQL_Dump()"), requete + "\n"))
                                         {  //CGestIni::Param_UpdateToDisk( QString("/home/ro/lastRequete-%1.sql").arg(QString::number(nbErr++)), requete);
                                            Datasemp_OutMessage(logWidget, QString( "<font color=#ff0000><b>ERREUR INSERT INTO : \n</b></font>") + m_LastError) ; // \n pour mettre m_LastError � la ligne
                                         }
                                      isMultiLines  = 0;
                                    }
                                 else  //.... on se sert de multi ligne pour eviter recopie dans requete (plus rapide) ........
                                    { query.exec(line);
                                      if (  OutSQL_error(query, QObject::tr("\nERREUR INSERT INTO : ParseSQL_Dump()"), line + "\n"))
                                         {  //CGestIni::Param_UpdateToDisk( QString("/home/ro/lastRequete-%1.sql").arg(QString::number(nbErr++)), line);
                                            Datasemp_OutMessage(logWidget, QString( "<font color=#ff0000><b>ERREUR INSERT INTO : \n</b></font>") + m_LastError) ; // \n pour mettre m_LastError � la ligne
                                         }
                                    }
                                 break;
                               }
                            else  //.. on a rencontre une fin de ligne qui n'est pas une fin INSERT TO ...........
                                  //   faisons l'hypothese que c'est une donnee textuelle sur plusieurs lignes
                                  //   et remplacons cette donnee par un espace
                               {//while (pos<len_line)
                                //      {line[pos] = '$';
                                //       ++pos;
                                //      }
                                ++ isMultiLines;
                                requete += line;
                               } 
                          }
                       else //..... on ne rajoute que si besoin .............
                          { requete      += line;
                            ++ isMultiLines;
                          }
                       //m_position    += (int)file.readLine(line,MAX_READ); //line=line.stripWhiteSpace();
                       line           = read_line( &file, buffer, len_read, maxlen);
                       m_position    += len_read;
                       if (pQProgressBar) {pQProgressBar->setProgress(m_position/1000); qApp->processEvents();qApp->processEvents();} 
                    } while (!file.atEnd());
        }
    }
    delete buffer;
    long    s = dtDeb.secsTo (QDateTime::currentDateTime());
    long    h = s/3600; s -=  h*3600;
    long    m = s/60;   s -=  m*60;
    Datasemp_OutMessage(logWidget, QObject::tr( "=== Intégration SQL terminée en : <font color=#ff0000><b>%1</b></font>==")
                                            .arg(QString::number(h) + "h" + QString::number(m) + "mn" + QString::number(s) + "s"));
}

//-----------------------------------------------------  displayProgression -------------------------------------------
void  CApp::displayProgression(QProgressBar *pQProgressBar)
{if (pQProgressBar) pQProgressBar->setProgress(m_position);
 qApp->processEvents();qApp->processEvents();//qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();
 qApp->processEvents();qApp->processEvents();//qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();
}

//----------------------------------------------- Datasemp_OutMessage ---------------------------------------------------------------------
void  CApp::Datasemp_OutMessage( CW_EditText *logWidget, const QString &mess)
{if (logWidget==0)  return;
    logWidget->append( mess );
    logWidget->scrollToBottom ();
    qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();
    qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();
}

//-------------------------------------------------- Decompresse -----------------------------------------------
int CApp::Decompresse( QString filename)
{   C_Zlib czlib;
    return czlib.Decompresse(filename, filename + ".sql", ".zql");
}

//-------------------------------------------------- OutSQL_error -----------------------------------------------
int CApp::OutSQL_error( const QSqlQuery &cur, const char *messFunc /*=0*/, const char *requete /*=0*/)
{   QSqlError error = cur.lastError();
    m_LastError     ="";

    if (error.type() != QSqlError::None)
    {switch (error.type())
     {       case    QSqlError::None:           m_LastError =" - no error occurred: "          ; break;
             case    QSqlError::Connection:     m_LastError =" - connection error: "           ; break;
             case    QSqlError::Statement:      m_LastError =" - SQL statement syntax error: " ; break;
             case    QSqlError::Transaction:    m_LastError =" - transaction failed error: "   ; break;
             case    QSqlError::Unknown:        m_LastError =" - unknown error: "              ; break;
             default:                           m_LastError =" - unknown error: "              ; break;
             }
         
        if (messFunc) m_LastError  = m_LastError.prepend(QString(messFunc) + "\r");
                      m_LastError += error.databaseText();
        if (requete)  m_LastError += QString("\r") + requete;
        // qDebug(m_LastError);
        return 1;
    }
    //     qDebug(qserr + error.databaseText ());
    return 0;
}

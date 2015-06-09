/********************************* CUpdateThread.cpp ******************************
 *                                                                                *
 *   #include "CUpdateThread.cpp"                                                 *
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

#include "CUpdateThread.h"
#include "CApp.h"

#include <qdir.h>
#include <qfileinfo.h>
#include <qtextstream.h>
#include <qsqlquery.h>

//------------------------------------------------ Datasemp_Init --------------------------------------------
long   CUpdateThread::Init(QSqlDatabase *dataBase, const QString &srcPath, const QString &mainDir, const QString &dataDir, const QString &docsDir, const QString &structDir, QTextEdit *logWidget)
{m_dataBase       =  dataBase;
 m_srcPath        =  srcPath + QDir::separator();
 m_logWidget      =  logWidget;
 m_Satus          = "";
 m_NbPosition     = 0;
 m_position       = 1;
 m_Continue       = 1;
 m_list_create.clear();
 m_list_update.clear();
 m_list_docs.clear();
 m_mainDir        = mainDir;
 m_dataDir        = dataDir;
 m_docsDir        = docsDir;
 m_structDir      = structDir;

 QString          outMess ;
 long       nb(0);
 m_OutLog  = logWidget->text();

    //............ relever tous les fichiers du folder create......................
  { nb = 0;
    QDir dir (srcPath + QDir::separator() + m_mainDir + QDir::separator() + m_structDir);
    dir.convertToAbs();
    dir.setFilter( QDir::All | QDir::NoSymLinks );
    const QFileInfoList * pQFileInfoList = dir.entryInfoList();
    if (pQFileInfoList==0)
       {m_OutLog += QObject::tr("ERREUR : Ce repertoire ne semble pas correspondre à celui de Datasemp");
        m_Fini    = 1;
        return 0;
       }
    m_Satus = QObject::tr(" Analyse des tables à remplir");
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo     *fi;
    while ( (fi = it.current()) != 0 )
          {QString fname = fi->fileName().latin1();
           if (fi->isFile())
                   {m_list_create.append(fname);
                   }
           ++it;
           ++nb;
          }

    m_OutLog += QObject::tr("\r\nDénombrage des tables à créer terminé : ") +QString::number(nb) + QObject::tr(" tables à créer") ;
   }

    //............ relever tous les fichiers du folder data......................
  { nb = 0;
    m_Satus = QObject::tr(" Analyse des tables à remplir");
    QDir dir (srcPath + QDir::separator() + m_mainDir + QDir::separator() + m_dataDir);
    dir.convertToAbs();
    dir.setFilter( QDir::All | QDir::NoSymLinks );
    const QFileInfoList * pQFileInfoList = dir.entryInfoList();
    if (pQFileInfoList==0)
       {m_OutLog += QObject::tr("ERREUR : Ce repertoire ne semble pas correspondre à celui de Datasemp");
        m_Fini    = 1;
        return 0;
       }
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo     *fi;
    while ( (fi = it.current()) != 0 )
          {QString fname = fi->fileName().latin1();
           if (fi->isFile())
                   {m_list_update.append(fname);
                   }
           ++it;
           ++nb;
          }

    m_OutLog += QObject::tr("\r\nDénombrage des tables à remplir terminé : ") +QString::number(nb) + QObject::tr(" tables à remplir") ;
   }

     //............ relever tous les fichiers du folder docs......................
  { nb = 0;
    m_Satus = QObject::tr(" Analyse des tables à remplir");
    QDir dir (srcPath + QDir::separator() + m_mainDir + QDir::separator() + m_docsDir);
    dir.convertToAbs();
    dir.setFilter( QDir::All | QDir::NoSymLinks );
    const QFileInfoList * pQFileInfoList = dir.entryInfoList();
    if (pQFileInfoList==0)
       {m_OutLog += QObject::tr("ERREUR : Ce repertoire ne semble pas correspondre à celui de Datasemp");
        m_Fini    = 1;
        return 0;
       }
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo     *fi;
    while ( (fi = it.current()) != 0 )
          {QString fname = fi->fileName().latin1();
           if (fi->isFile())
                   {m_list_docs.append(fname);
                   }
           ++it;
           ++nb;
          }

    m_OutLog += QObject::tr("\r\nDénombrage des tables de documents à remplir terminé : ") +QString::number(nb) + QObject::tr(" tables à remplir") ;
   }
    //............ les executer ......................
    m_NbPosition = (m_list_create.count() + m_list_update.count() + m_list_docs.count());
    return m_NbPosition;
}

//------------------------------------------------ Datasemp_SetBases --------------------------------------------
void  CUpdateThread::Datasemp_SetBases(QSqlDatabase *dataBase, const QString &srcPath)
{  m_Fini = 0;

   {
    for ( QStringList::Iterator it = m_list_create.begin(); it != m_list_create.end(); ++it )
        {QString name = srcPath + m_mainDir + QDir::separator() + m_structDir + QDir::separator() + (*it);
         m_Satus = QObject::tr(" Création de la table : ") + name + " en cours: ";
         if (Datasemp_CreationTables(dataBase, name)!=1)                 goto End_Datasemp_SetBases;
         m_OutLog += QObject::tr("\r\nTable : ") + name + QObject::tr(" Créée avec succès") ;
        }
   }
   {
    for ( QStringList::Iterator it = m_list_update.begin(); it != m_list_update.end(); ++it )
        {QString name = srcPath + m_mainDir + QDir::separator() + m_dataDir + QDir::separator() + (*it);
         m_Satus = QObject::tr(" Insertion de données dans la table : ") + name + " en cours : ";
         if (Datasemp_UpdateTables(dataBase, name ) <= -1)                goto End_Datasemp_SetBases;
         m_OutLog += QObject::tr("\r\n la table : ") + name + QObject::tr(" a été initialisée avec : ") + QString::number(m_CurrentLine)+ QObject::tr(" données ") ;
        }
   }
   {
    for ( QStringList::Iterator it = m_list_docs.begin(); it != m_list_docs.end(); ++it )
        {QString name = srcPath + m_mainDir + QDir::separator() + m_docsDir + QDir::separator() + (*it);
         m_Satus = QObject::tr(" Mise à jour des données dans la table : ") + name + " en cours : ";
         if (Datasemp_UpdateDocu(dataBase, name ) <= -1)                goto End_Datasemp_SetBases;
         m_OutLog += QObject::tr("\r\n la table : ") + name + QObject::tr(" a été mise à jour avec : ") + QString::number(m_CurrentLine)+ QObject::tr(" données ") ;
        }
   }
End_Datasemp_SetBases:
  m_Fini = 1;
}

//-----------------------------------------------------  StopSetBases -------------------------------------------
void  CUpdateThread::StopSetBases()
{m_Continue = 0;
}

//-----------------------------------------------------  GetCurrentLine -------------------------------------------
long  CUpdateThread::GetCurrentLine()
{return m_CurrentLine;
}

//-----------------------------------------------------  GetCurrentStatus -------------------------------------------
QString  CUpdateThread::GetCurrentStatus()
{return m_Satus;
}

//-----------------------------------------------------  GetCurrentProgress -------------------------------------------
long  CUpdateThread::GetCurrentProgress(long *nbTotal)
{if (nbTotal) *nbTotal = m_NbPosition;
 return m_position;
}

//-----------------------------------------------------  InsertData -------------------------------------------
long  CUpdateThread::Datasemp_UpdateTables(QSqlDatabase *dataBase, const QString &fname)
{
 if ( !QFile::exists( fname ) )   return -1;
 QFile file( fname );
 if ( !file.open( IO_ReadOnly ) ) return -2;
 QTextStream stream ( &file );
 stream.setEncoding ( QTextStream::Latin1 );
 //.......... on déclare une variable pour le lire ligne à ligne ..................
 QString buffer("");

 QSqlQuery query(QString::null, dataBase);
 query.exec("ALTER TABLE `DatasempTest`.`t00` ADD INDEX `a2prim` (`f2`)" );    // index ultra-critique sur le cip
 m_CurrentLine =  0;
 m_position++;
 while (!stream.atEnd())
    {buffer    += stream.readLine();
     if (buffer.left(2)=="--" || buffer.left(2)=="/*")
        {buffer = "";
        }
     else
        {uint len   = buffer.length();
         if ( (buffer.at(len-2) == ')')  &&  (buffer.at(len-1) == ';'))
            {query.exec(buffer);
             buffer = "";
             ++m_CurrentLine;
            }
         }
     if (m_Continue==0) return -3;
    }
 return m_CurrentLine;
}

//-----------------------------------------------------  Datasemp_UpdateDocu -------------------------------------------
long  CUpdateThread::Datasemp_UpdateDocu(QSqlDatabase *dataBase, const QString &fname)
{
 if ( !QFile::exists( fname ) )   return -1;
 QFile file( fname );
 if ( !file.open( IO_ReadOnly ) ) return -2;
 QTextStream stream ( &file );
 stream.setEncoding ( QTextStream::Latin1 );

 QSqlQuery query(QString::null, dataBase);
 QString requete   ="";
 m_CurrentLine     =  0;
 m_position++;

 QString buffer("");
 //..................... isoler les données à inserer ............................................
 buffer    = stream.readLine().stripWhiteSpace();
 ++m_CurrentLine;
 do
   {
    if (buffer.left(2)=="--" || buffer.left(2)=="/*")
       {buffer = "";
       }
    else
        {uint len   = buffer.length();
         if ( (buffer.at(len-2) == ')')  &&  (buffer.at(len-1) == ';'))
           {requete += buffer;
            query.exec(requete);
            requete = "";
           }
        else
           { requete += buffer+" ";
           }
       }
    if (m_Continue==0) return -3;
    buffer     = stream.readLine().stripWhiteSpace();
    ++m_CurrentLine;
   } while (!stream.atEnd());
 return m_CurrentLine;
}

//-----------------------------------------------------  CreationTables -------------------------------------------
int  CUpdateThread::Datasemp_CreationTables(QSqlDatabase *dataBase, const QString &f_name)
{
 if ( !QFile::exists( f_name ) )  return -1;
 QFile file( f_name );
 if ( !file.open( IO_ReadOnly ) ) return -2;

 QTextStream stream( &file );
 stream.setEncoding ( QTextStream::Latin1 );

 QString requete(stream.read());
 QStringList list = QStringList::split(';',requete);
 QSqlQuery query(QString::null, dataBase);
 m_position++;
 for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
     {requete = (*it).stripWhiteSpace();
      if (requete.length())query.exec(requete);
      if (m_Continue==0) return 0;
     }
 return 1;
}


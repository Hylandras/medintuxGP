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
 *   Commissariat à l"Energie Atomique                                            *
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
#include <qtimer.h>
// #include <qdebug.h>
#include "CApp.h"
#include "../../MedinTuxTools/CGestIni.h"

#define MAX_READ 0XFFFFFFF

CApp* G_pCApp = 0;  // contiendra l'instance globale de l'application
static char NUM_VERSION[]     = "==##@@==2.16.002==@@##==";
 
//--------------------------------------------- CApp -------------------------------------------------------------------
CApp::CApp(int & argc, char ** argv)
    : QApplication (  argc, argv ) , CSetupBase(), m_MySQL_DataBase(0)
{  QFileInfo qfi(argv[0]);
   m_NUM_VERSION           = NUM_VERSION;
   m_NameAppli             = "set_bases";
   m_PathAppli             = CGestIni::Construct_PathBin_Module(m_NameAppli,  qfi.dirPath (true));
   m_ParamPath             = m_PathAppli + "set_bases.ini";  // utiliser le .ini local
   m_LastError             = "";
   m_DisplayFilePosLabel   = 0;
   m_QProgressBar          = 0;
   m_bufferSize            = 15000000;
   m_rejectedStr           = "";
   if (argc>1 && QString(argv[1]) == "BASE_INSTALL")            m_justInstallBase = 1;
   else                                                         m_justInstallBase = 0;
   if (argc>1 && QString(argv[1]).startsWith("COMPILATION"))  { m_justCompilation = 1; 
                                                                m_rejectedStr     = argv[1]; 
                                                                m_rejectedStr     = m_rejectedStr.remove("COMPILATION(").remove(")");
                                                              } 
   else                                                       { m_justCompilation = 0;
                                                              }

   // qDebug(QString("argc : %1  'arv[0]' : %2 m_justInstallBase : %3").arg(QString::number(argc),argv[0],QString::number(m_justInstallBase)));
   // qDebug(QString("argc : %1  'arv[1]' : %2 m_justInstallBase : %3").arg(QString::number(argc),argv[1],QString::number(m_justInstallBase)));
   //.................. recuperer parametres sesam-vitale ...............................................
   CGestIni::Param_UpdateFromDisk(m_ParamPath, m_ParamData);
   m_bufferSize            = CGestIni::Param_ReadUniqueParam(m_ParamData, "Parametres", "sql_buffer").toLong();
   if (m_bufferSize==0)    m_bufferSize = 15000000;   // on est plus sur Ataris ST 1040 hein !!!
   //....................... systheme du theme ..............................................
   if (CGestIni::Param_ReadParam( m_ParamData, "Theme", "Path", &m_PathTheme) !=0 )  // zero = pas d'erreur
       { m_PathTheme       =  "../../Themes/Default/"; 	// valeur par défaut
       }
   if (!QDir(m_PathTheme).exists()) m_PathTheme     =  "../../Themes/Default/"; 	// valeur par défaut
   if (QDir(m_PathTheme).isRelative()) m_PathTheme.prepend(m_PathAppli);
   m_PathTheme = QDir::cleanDirPath(m_PathTheme) + "/";
   Theme::setPath(m_PathTheme);
   G_pCApp                 = this;
}

//--------------------------------------------- quit -------------------------------------------------------------------
/*! \brief surcharge du Slot quit afin d'envoyer le message Sign_QuitterRequired permettant a ceux qui s'y connectent de sauver les meubles \
*/
void CApp::quit()
{emit Sign_QuitterRequired();    // se connecter a tous ceux connectes a ce signal pour qu'il puissent Sauver le meubles.
 QApplication::quit();
}

//--------------------------------------------- CApp -------------------------------------------------------------------
CApp::~CApp()
{G_pCApp          = 0;
    if (m_MySQL_DataBase)
    {if (m_MySQL_DataBase->isOpen()) m_MySQL_DataBase->close();
        QSqlDatabase::removeDatabase (m_MySQL_DataBase);
    }
}

//--------------------------------------------------- GotoDebug ------------------------------------------------
int CApp::GotoDebug()
{
  return 1;
}

//--------------------------------------------------- GetMySQL_DataBase ------------------------------------------------
QSqlDatabase*  CApp::GetMySQL_DataBase(QString user, QString password, QString hostName, QString port, CW_EditText  *logWidget)
{
//      int ret_create=FALSE;
//      ret_create = QMessageBox::information( 0, "TEST","ICi");
if (m_MySQL_DataBase) return m_MySQL_DataBase;

    QString errMess;
    QString outMess;
    int     errNum ;
    //..................... ouvrir la base test ...............................................................
    m_MySQL_DataBase = BaseConnect("QMYSQL3", "test", user, password, hostName, port, "TestBase", &errMess, &errNum);
    if (m_MySQL_DataBase==0)
    {   QString outMess  =     QObject::tr("============== GetMySQL_DataBase() Échec de la connexion à la base test  =============================================\n") ;
        outMess         +=     errMess + " " + QString::number(errNum) ;
        if (logWidget) Datasemp_OutMessage(logWidget, outMess) ;
        //..................... si echec ouvrir la base mysql ...............................................................
        m_MySQL_DataBase = BaseConnect("QMYSQL3", "mysql", user, password, hostName, port, "TestBase", &errMess, &errNum);
        if (m_MySQL_DataBase==0)
           {   QString outMess  =     QObject::tr("============== GetMySQL_DataBase() Échec de la connexion à la base mysql =============================================\n") ;
               outMess         +=     errMess + " " + QString::number(errNum) ;
               if (logWidget)  Datasemp_OutMessage(logWidget, outMess) ;
               //..................... si echec essayer d'ouvrir une des bases de DrTux ...............................................................
               //                      mauvaise idee car elle peut etre detruite au cours des operations de MAJ
               return 0;
           }
    }
    return m_MySQL_DataBase;
}

//-----------------------------------------------------  GetBasesListToSet -------------------------------------------
QStringList  CApp::GetBasesListToSet()
{   QStringList list;
    QString     dataBaseName("");
    char                 *pt  = (char*)(const char*) m_ParamData;
    while((pt = CGestIni::Param_GotoNextSection(pt, 0, &dataBaseName)) && *pt)     // recupere le nom de la section dans  dataBaseName
    {if (dataBaseName.left(3)=="DB_")
        {list.append(dataBaseName.mid(3));
        } // end if (dataBaseName.left(3)=="DB_")
        dataBaseName = "";
    } //end while((pt = CGestIni::Param_GotoNextSection(pt, 0, &dataBaseName)) && *pt)

    return list;
}

//-----------------------------------------------------  reataureBase -------------------------------------------
QSqlDatabase*  CApp::restaureBase(    QString fileName,
                                      QString driver,              // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                      QString user,                // = "root"
                                      QString passWord,            // = ""
                                      QString hostName,            // = "localhost"
                                      QString port,
                                      CW_EditText  *logWidget,     // QEdit pour afficher les logs
                                      QProgressBar *pQProgressBar,  // progress_barr
                                      QLabel *pQlabel
                                     )
{   QString dataBaseName      = getBaseNameToSetFromSQLFile(fileName);  if (dataBaseName.length()==0)  return 0;
    m_DisplayFilePosLabel     = pQlabel;
    m_QProgressBar            = pQProgressBar;
    m_fileToRestaure          = fileName;
    m_positionEnd             = 0;
    m_divSteep                = 10000;
    m_position                = 0;
    QString        baseToSet  = dataBaseName;
    QString          outMess  = "";
    QString          errMess  = "";
    int               errNum  = 0;
    char                 *pt  = (char*)(const char*) m_ParamData;
    int            ret_create = FALSE;
    QString            nb_pas = "";
    QString   dataBaseConName = "SetupTest";
    QSqlDatabase* dataBase    = 0;
    QSqlDatabase* mysql_base  = GetMySQL_DataBase(user, passWord, hostName, port, logWidget);       // base MySQL pour creer les bases
    if (mysql_base ==0) return 0;

    if (pQlabel) pQlabel->setText( tr("Restauration de la base de données : ") + m_fileToRestaure + tr(" en cours ... lus : %1 octets sur : %2 " ).arg(QString::number(m_position) , QString::number(m_positionEnd) ) );
    //........................ lister les bases deja presentes ...............................
    if (!mysql_base->isOpen())
       { if (mysql_base->open()==FALSE) {Datasemp_OutMessage(logWidget, QObject::tr("Echec  SetOneBase(), La base  '%1' n'a pu être ouverte \n").arg(mysql_base->databaseName ())) ;return 0;}
       }
    QStringList basesAlreadyPresents;
    QSqlQuery query(QString::null, mysql_base);
    query.exec(QString("SHOW DATABASES"));
    if (query.isActive())
       {while (query.next())  basesAlreadyPresents.append (query.value(0).toString());
       }
    mysql_base->close();

    while((pt = CGestIni::Param_GotoNextSection(pt, 0, &dataBaseName)) && *pt)     // recupere le nom de la section dans  dataBaseName
    {QString toCompare = dataBaseName.mid(3);
     if (toCompare.lower() == baseToSet.lower())           // PB   baseToSet avec majuscules telles que le nom devrai etre
        {   dataBaseName = toCompare;              // PB   dataBaseName sans majuscules car Windows connait pas
            //................ existe deja --> poser la question de l'effacement .....................................
#ifdef Q_WS_WIN
            if (basesAlreadyPresents.findIndex(dataBaseName.lower()) != -1
#else
            if (basesAlreadyPresents.findIndex(dataBaseName)         != -1
#endif
               )
               {outMess   = QObject::tr("============== La base  ")+ baseToSet +QObject::tr(" existe déjà ======================================\n");
                Datasemp_OutMessage(logWidget, outMess) ;
                QString backName = baseToSet + "Back"+ QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
                ret_create =  QMessageBox::information( 0,    QObject::tr(  " MedinTux Setup "),
                                                              QObject::tr ( "La base : <b>") + dataBaseName +
                                                              QObject::tr ( "</b><br> semble déjà exister <br>"
                                                                            "faut-il l'effacer et la recréer <br>"
                                                                            "<b>au risque de d'écraser des données précieuses</b> <br>"
                                                                            "ou annuler l'opération en cours ?<br>"
                                                                            "l'ancienne base sera sauvegardée en : <b><font color=#002aff>'%1'</font></b><br>").arg(backName) ,
                                                              QObject::tr("Annu&ler"), QObject::tr("&Recréer"), 0,
                                                        1, 1 ) ;
                if (ret_create==1)
                   {outMess = QObject::tr("<font color=#ff6f01>============== Backup de la base : '%1' en '%2' en cours .... ======================================</font>\n").arg(baseToSet, backName);
                    Datasemp_OutMessage(logWidget, outMess);
                    if (! BackUpBase(baseToSet, backName,  driver, user,  passWord,  hostName,  port, &outMess,  &errNum ))
                       {   outMess.prepend(QObject::tr("<font color=#ff0000>============== Échec du backup de la base : '%1' en '%2'  ======================================</font>\n").arg(baseToSet, backName));
                           Datasemp_OutMessage(logWidget, outMess);
                           return 0;
                       }
                    else
                       {   outMess = QObject::tr("============== Succès du backup de la base : '%1' en '%2'  ======================================\n").arg(baseToSet, backName);
                           Datasemp_OutMessage(logWidget, outMess);
                       }
                   }
                else
                   {outMess = QObject::tr("============== Annulation de l'installation de la base ")+ baseToSet +QObject::tr(" ======================================\n");
                    Datasemp_OutMessage(logWidget, outMess);
                    return 0;
                   }
               }
            //.................... le champ devrait être libre pour creation ....................................
            ret_create = CreationBase(mysql_base, baseToSet, &errMess, 0, driver, user, passWord, hostName, port);
            if ( ret_create == 1 )   // si la creation de la base s'est passée normalement
               {        //............... Ouverture de la base et remplissage....................................
                outMess  =      QObject::tr("<font color=#ff6f01>============== Création de la base ")+ baseToSet +QObject::tr(" ============================================</font>\n") ;
                Datasemp_OutMessage(logWidget, outMess) ;
                // on se connecte à la nouvelle base
                dataBase =  BaseConnect( driver  ,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                         baseToSet,       // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                         user,            // = "root"
                                         passWord,        // = ""
                                         hostName,        // = "localhost"
                                         port,
                                         dataBaseConName+baseToSet,
                                         &errMess,
                                         &errNum );
                if (dataBase==0)
                   {   outMess  = QObject::tr("<font color=#ff0000>============== Échec de la création de la base ")+ baseToSet +QObject::tr(" ======================================</font>\n") ;
                       outMess += errMess + " " + QString::number(errNum) ;
                       Datasemp_OutMessage(logWidget, outMess) ;
                   }
                else
                   {   QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
                       //............... Ok la base a bien été créée ....................................
                       outMess  = QObject::tr("La base de données ")+ baseToSet +QObject::tr(" a été créée avec succès \n");
                       Datasemp_OutMessage(logWidget, outMess) ;
                       //............... Création des tables ............................................
                       Datasemp_OutMessage(logWidget,QObject::tr("<font color=#ffd201>Exécution du fichier de requêtes SQL   ")+ fileName + QObject::tr(" en cours....</font>"));
                       Datasemp_OutMessage(logWidget,QObject::tr("<font color=#ffd201>La taille du buffer SQL est de <b>%1</b> octets ").arg(QString::number(m_bufferSize)));
                       ParseSQL_Dump(dataBase,  fileName, pQProgressBar, logWidget, pQlabel);
                       QApplication::restoreOverrideCursor();
                   } // end else if if (dataBase==0)
               }  //end if (ret_create == 1 )
            else
               {outMess = QObject::tr("Opération annulée : la base ")+ baseToSet +QObject::tr(" n'a pu être créée (vérifiez vos droits MySQL) \n");
                Datasemp_OutMessage(logWidget, outMess) ;
               } //end if else(dataBase ==0 )
        } // end if (toCompare == baseToSet)
        dataBaseName = "";
    } //end while((pt = CGestIni::Param_GotoNextSection(pt, 0, &dataBaseName)) && *pt)

    QString   qsNbPas = QString::number(m_position,'f',0);
    outMess           = QObject::tr("  Analyse de : <font color=#ffd201><b>") + qsNbPas + QObject::tr("</b></font> lignes effectuée \n");
    Datasemp_OutMessage(logWidget, outMess) ;
    CGestIni::Param_WriteParam(  &m_ParamData,  baseToSet, "nb_progress", qsNbPas);
    CGestIni::Param_UpdateToDisk( m_ParamPath,  m_ParamData);
    return dataBase;
}

//-----------------------------------------------------  getBaseNameToSetFromSQLFile -------------------------------------------
QString CApp::getBaseNameToSetFromSQLFile(const QString &fname)
{   QString baseName   = "";
    QString line       = "";
    QStringList lstName;
     if ( !QFile::exists( fname ) )           return baseName;
     QFile file( fname );
     if ( !file.open( IO_ReadOnly|IO_Raw  ) ) return baseName;

    //.......... lit le fichier ligne à ligne ..................
    while (!file.atEnd())
    {   file.readLine(line, MAX_READ); // line = line.stripWhiteSpace();
        //.................creation de la table .....................
        int pos = -1;
        if (line.left(8)=="-- Host:" && (pos=line.find("Database:",8))!= -1)
           {pos += 9;
            baseName =  line.mid(pos).remove(";").remove("\n").remove("\r").stripWhiteSpace();
            lstName  = GetBasesListToSet();
            for (int i = 0; i < (int)lstName.count(); ++i)
                {if (lstName[i].lower() == baseName.lower()) baseName = lstName[i];
                }
            break;
           }
        else if (line.left(29)=="CREATE DATABASE IF NOT EXISTS")
           {baseName =  line.mid(29).remove(";").remove("\n").remove("\r").stripWhiteSpace();
            lstName  = GetBasesListToSet();
            for (int i = 0; i<(int)lstName.count();++i)
                {if (lstName[i].lower() == baseName.lower()) baseName = lstName[i];
                }
            break;
           }
        else if (line.left(16)=="-- Create schema")              //-- Create schema CIM10Test
           {baseName =  line.mid(16).remove(";").remove("\n").remove("\r").stripWhiteSpace();
            lstName  = GetBasesListToSet();
            for (int i = 0; i<(int)lstName.count();++i)
                {if (lstName[i].lower() == baseName.lower()) baseName = lstName[i];
                }
            break;
           }

    }
   file.close();
   return baseName;
}

//-----------------------------------------------------  RemoveDesignerVersion -------------------------------------------
void  CApp::RemoveDesignerVersion(QString &txt)
{   int deb = txt.find("version=\"");
    if (deb == -1) return;
    int end =  txt.find("\"", deb+9);
    if (end == -1) return;
    txt.replace(deb, end+1-deb, "version=\"0.0\"");
}

/*
<includehints>
    <includehint>cmaskedlineedit.h</includehint>
    <includehint>cmaskedlineedit.h</includehint>
    <includehint>cmaskedlineedit.h</includehint>
    <includehint>cmaskedlineedit.h</includehint>
    <includehint>cmaskedlineedit.h</includehint>
    <includehint>cmaskedlineedit.h</includehint>
    <includehint>cmaskedlineedit.h</includehint>
    <includehint>cmaskedlineedit.h</includehint>
    <includehint>cmaskedlineedit.h</includehint>
    <includehint>cmaskedlineedit.h</includehint>
    <includehint>cmaskedlineedit.h</includehint>
    <includehint>cmaskedlineedit.h</includehint>
    <includehint>cmaskedlineedit.h</includehint>
</includehints>
*/

//-----------------------------------------------------  Remove_TAG_includehints -------------------------------------------
void  CApp::Remove_TAG_includehints(QString &txt)
{   int deb = txt.find("<includehints>");
    if (deb == -1) return;
    int end =  txt.find("</includehints>", deb+15);
    if (end == -1) return;
    txt.replace(deb, end+1-deb, " ");
}

//-----------------------------------------------------  RemoveAllDesignerVersion -------------------------------------------
void  CApp::RemoveAllDesignerVersion(const QString &path_in)
{   QString    path(path_in);
    QDir       qDir;
    qDir.setPath ( path );
    qDir.convertToAbs();
    path = qDir.path();
    qDir.setFilter( QDir::All | QDir::NoSymLinks );

    //............ exploration de la liste ......................
    const QFileInfoList * pQFileInfoList = qDir.entryInfoList();
    if (pQFileInfoList==0) return ;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo     *fi;
    while ( (fi = it.current()) != 0 )
    {QString fname = fi->fileName().latin1();
        if      (fname == "..")
        {
        }
        else if (fi->isDir() && fname != "." )
        {
        }
        else if (fi->isFile())
        {   QString file_buffer;
            if (fname.right(3).lower()==".ui")
            {   fname.prepend(path_in+QDir::separator());
                CGestIni::Param_UpdateFromDisk(fname, file_buffer);
                RemoveDesignerVersion(file_buffer);
                Remove_TAG_includehints(file_buffer);
                QFile file( fname);
                if ( !file.open( IO_WriteOnly ) )    return;
                QTextStream stream( &file );
                QCString buf;
                if (CGestIni::IsUtf8( file_buffer ))
                   {//stream.setEncoding (QTextStream::UnicodeUTF8 );
                    buf = file_buffer;
                   }
                else
                   {//s tream.setEncoding (QTextStream::Latin1 );
                    // file_buffer    =  QString::fromUtf8 ( file_buffer ) ;
                    buf = file_buffer.utf8 ();
                   }
                stream.setEncoding (QTextStream::UnicodeUTF8 );
                stream << file_buffer;
                //Param_UpdateToDisk(fname, file_buffer);
            }
        }
        ++it;
    }
}

#define MAX_READ 0XFFFFFFF


//-----------------------------------------------------  readLine -------------------------------------------
unsigned long  CApp::readLine(QFile *pQFile, char *buffer, QString &outParam, unsigned long nbMax)
{
         unsigned long nb                              =  pQFile->readLine(buffer, nbMax);
         buffer[nb]                                    =  0;
         if (CGestIni::IsUtf8(buffer, nb)) outParam    =  QString::fromUtf8 ( buffer ) ;
         else                              outParam    =  buffer;

         return nb;
}
/*
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
void  CApp::ParseSQL_Dump(QSqlDatabase *dataBase, const QString &fname, QProgressBar *pQProgressBar / * = 0 * /, CW_EditText *logWidget / * = 0 * /)
{//....................... ouvrir le fichier en mode QTextStream ...........................
    if ( !QFile::exists( fname ) )           return;
    QFile file( fname );
    if ( !file.open( IO_ReadOnly|IO_Raw  ) ) return;
    Q_ULONG maxlen        = 16004000;
    char    *buffer       = new char[maxlen+5];    // +5 pour permettre analyse utf8 qui explore trois apres
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
                           // requete.remove ("collate utf8_unicode_ci");
                           // requete.remove ("collate utf8_bin");
                           // requete.remove ("character set utf8");
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
        {   // PB de ce * /$x?! de datasemp; certaines données ont des retours chariots
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
                                         {  CGestIni::Param_UpdateToDisk( QString("/home/ro/lastRequete-%1.sql").arg(QString::number(nbErr++)), requete);
                                            Datasemp_OutMessage(logWidget, QString( "<font color=#ff0000><b>ERREUR INSERT INTO : \n</b></font>") + m_LastError) ; // \n pour mettre m_LastError � la ligne
                                         }
                                      isMultiLines  = 0;
                                    }
                                 else  //.... on se sert de multi ligne pour eviter recopie dans requete (plus rapide) ........
                                    { query.exec(line);
                                      if (  OutSQL_error(query, QObject::tr("\nERREUR INSERT INTO : ParseSQL_Dump()"), line + "\n"))
                                         {  CGestIni::Param_UpdateToDisk( QString("/home/ro/lastRequete-%1.sql").arg(QString::number(nbErr++)), line);
                                            Datasemp_OutMessage(logWidget, QString( "<font color=#ff0000><b>ERREUR INSERT INTO : \n</b></font>") + m_LastError) ; // \n pour mettre m_LastError � la ligne
                                         }
                                    }
                                 break;
                               }
                            else  //.. on a rencontre une fin de ligne qui n'est pas une fin INSERT TO ...........
                                  //   faisons l'hypothese que c'est une donnee textuelle sur plusieurs lignes
                                  //   et remplacons cette donnee par un espace
                               {while (pos<len_line)
                                      {line[pos] = '$';
                                       ++pos;
                                      }
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
*/
//-----------------------------------------------------  get_file_size -------------------------------------------
double CApp::get_file_size(const char *file_name , QLabel *pQlabel /* = 0 */)
{  /*
   FILE *fp;
   unsigned long file_size = 0;
   if ((fp = fopen(file_name, "rb" )) == NULL) {
      fprintf(stderr, "Cannot open %s.\n", file_name);
      return(file_size);
   }
   if (fseek(fp, (long)(0), SEEK_END) != 0) {
      fclose(fp);
      return(file_size);
   }
   file_size = (unsigned long) (ftell(fp));
   fclose(fp);
   return(file_size);
*/
/*
QFile file( file_name );
if ( !file.open( IO_ReadOnly|IO_Raw  ) ) return 0;
double size = file.size();
file.close();
return size;
*/

QFile file( file_name );
if ( !file.open( IO_ReadOnly|IO_Raw  ) ) return 0;
char          *buffer = new char[m_bufferSize+5];     // +5 pour permettre analyse utf8 qui explore trois apres
if (buffer==0)                           return 0;
QString line          = "";
double size = 0;
while (!file.atEnd())
    {   size += file.readBlock(buffer, m_bufferSize);
        if (pQlabel) { pQlabel->setText( tr("Mesure de la taille du fichier %1  en cours ...").arg(QString::number(size,'f',2))); 
                       qApp->processEvents();qApp->processEvents();
                     }
    }
 delete buffer;
 file.close();
 return size;
}

//-----------------------------------------------------  Slot_displayAvancement -------------------------------------------
// m_startTime                   dtCurr                       dtEnd
//    ^                            ^                             ^
//    |<--------- s_past --------->|                             |
//    |<++++++ m_position ++++++++>|                             |
//    |                            |<---------- s_rest --------->|
//    |<------------------------------ s_end ------------------->|
//    |<++++++++++++++++++++++++++++++ m_positionEnd +++++++++++>|
//
void  CApp::Slot_displayAvancement()
{if (m_DisplayFilePosLabel)  
    { //........... on collate les infos ............................
      QDateTime dtCurr     = QDateTime::currentDateTime();
      long    s_past       = m_startTime.secsTo (dtCurr);
      long    s_end        = (long)((m_positionEnd/m_position)*s_past);
      long    s_rest       = s_end - s_past;
      QDateTime dtEnd      = m_startTime.addSecs ( s_end );
      //........... on prepare les infos ............................
      long    h_past       = s_past/3600; s_past -=  h_past*3600;
      long    m_past       = s_past/60;   s_past -=  m_past*60;
      long    h_rest       = s_rest/3600; s_rest -=  h_rest*3600;
      long    m_rest       = s_rest/60;   s_rest -=  m_rest*60;
      QString s_timePast   = QString::number(h_past)  + "h" + QString::number(m_past) + "mn" + QString::number(s_past) + "s";
      QString s_timeRest   = QString::number(h_rest)  + "h" + QString::number(m_rest) + "mn" + QString::number(s_rest) + "s";

      //........... on affiche les infos ............................
      m_DisplayFilePosLabel->setText( tr("Restauration de %1 octets sur %2 temps écoulé %3 restant %4 fin prévue " )
                                        .arg( QString::number(m_position,    'f', 0),
                                              QString::number(m_positionEnd, 'f', 0),
                                              s_timePast,
                                              s_timeRest
                                            ) + dtEnd.toString ( tr("le dd à hh:mm:ss"))
                                    );
    }
 if (m_QProgressBar)  {m_QProgressBar->setProgress(m_position/m_divSteep); }
 qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();
}
//-----------------------------------------------------  normaliseMajMinTableName -------------------------------------------
QString  CApp::normaliseMajMinTableName(const QString &table)
{ QString         data = CGestIni::Param_UpdateFromDisk( m_PathAppli+"SqlCreateTable/DrTuxTestTablesNames.txt" ).stripWhiteSpace();
                  data = data.remove("Création de la table `DrTuxTest`.`");
                  data = data.remove('\r');
                  data = data.remove('\n');
  QStringList     list = QStringList::split("` en cours....",data);
  QString    nameLower = table.lower().stripWhiteSpace(); nameLower = nameLower.remove('`');
  QString        name  = "";
  for ( int i = 0; i<(int)list.count(); ++i )
      { name    = list[i].lower();
        int pos = name.find("`drtuxtest`.`");
        if ( pos != -1 ) name = name.mid(pos+13);
        if (name==nameLower)
           { name = QString(" `")+ list[i].mid(pos+13) + "` ";
             return name;
           }
      }
  return table;
}
//-----------------------------------------------------  ParseSQL_Dump -------------------------------------------
void  CApp::ParseSQL_Dump(QSqlDatabase *dataBase, const QString &fname, QProgressBar *pQProgressBar /* = 0 */, CW_EditText *logWidget /* = 0 */, QLabel *pQlabel /* = 0 */)
{//....................... ouvrir le fichier en mode QTextStream ...........................
    m_positionEnd                  = get_file_size( fname, pQlabel ); //file.size();
    unsigned long  max_length_find = 0;
    if ( !QFile::exists( fname ) )           return;
    QFile file( fname );
    if ( !file.open( IO_ReadOnly|IO_Raw  ) ) return;
    char          *buffer = new char[m_bufferSize+5];     // +5 pour permettre analyse utf8 qui explore trois apres
    if (buffer==0)  
       { Datasemp_OutMessage(logWidget, QObject::tr("Allocation du buffer de lecture de taille : <font color=#01eaff><b>%1</b></font> non possible.").arg(QString::number(m_bufferSize+5))) ;
         return;
       }
    //............. creer et initialiser le systeme d'affichage de l'avancement ......................
    m_DisplayFilePosLabel     = pQlabel;
    m_QProgressBar            = pQProgressBar;
    QTimer *timer             = new QTimer( this );
    m_startTime               = QDateTime::currentDateTime();
    connect( timer, SIGNAL(timeout()), this, SLOT(Slot_displayAvancement()) );
    timer->start( 1000, FALSE );
    //.......... on déclare une variable pour lire le fichier ligne à ligne ..........................
    QString requete       = "";
    QString line          = "";
    QString table         = "";
    QSqlQuery query(QString::null, dataBase);
    pQProgressBar->setTotalSteps(m_positionEnd/m_divSteep);
    while (!file.atEnd())

    {
        m_position += readLine(&file, buffer, line, m_bufferSize);
        //.................creation de la table .....................
        if (line.left(12)=="CREATE TABLE")
        {
            int pos = line.find("(",12);
            if ( pos != -1 )
               {table = line.mid(12, pos-12);
                table = normaliseMajMinTableName(table);
                query.exec( QString("DROP TABLE IF EXISTS ")+table.lower()+";");
                query.exec( QString("DROP TABLE IF EXISTS ")+table+";");
                OutSQL_error(query, QObject::tr("ERREUR DROP TABLE IF EXISTS : ParseSQL_Dump()"), table );
                Datasemp_OutMessage(logWidget, QObject::tr("Création de la table <font color=#01eaff><b>") + table + QObject::tr("</b></font> en cours....")) ;
               }
            requete = line;
            while (!file.atEnd())
            {   m_position += readLine(&file, buffer, line, m_bufferSize);
                if (line[0]==')' && (line[2]=='T' || line[2]=='E' || line[1]==';'))
                   {   if (line.left(7) == ") TYPE=" || line.left(9) == ") ENGINE=" )  // ne surtout pas inclure ';' dans la comparaison
                          {requete += ") ENGINE=MyISAM;";    // TYPE=MyISAM;";  pour passer sur mac
                           line.remove ("collate utf8_unicode_ci");
                           line.remove ("collate utf8_bin");
                           line.remove ("character set utf8");
                           requete = requete.replace(QString("CREATE TABLE")+table.lower(), QString("CREATE TABLE")+table);
                           query.exec(requete);
                           if (OutSQL_error(query, QObject::tr("ERREUR CREATE TABLE : ParseSQL_Dump()"), requete ))
                              {Datasemp_OutMessage(logWidget, QString( "<font color=#ff0000><b>ERREUR SQL CREATE TABLE: %1 \n</b></font>").arg(table) + m_LastError) ;
                              }
                           break;
                          }
                   }
               requete += line;
            }
        }
// INSERT INTO `RubriquesHead` VALUES (1,'30DC82DD-7632-2C46-9DC3-BC1AC0ECD20A',20030000,'Asthme sai','2004-12-08 19:23:41','admin',0,0);
// INSERT INTO `RubriquesHead` VALUES (2,'30DC82DD-7632-2C46-9DC3-BC1AC0ECD20A',20020200,'Bilan labo complet','2004-12-08 19:23:41','admin',0,0);
// INSERT INTO `ACCES2` VALUES ('A','à foyer ouvert'),('A','par ... abord '),('A','par dissection ...');
// INSERT INTO `codes_postaux` (`id`,`code_postal`,`ville`) VALUES
//  (26662,62153,'ABLAIN ST NAZAIRE'),
//  (35076,80320,'ABLAINCOURT PRESSOIR');

        else if (line.left(11)=="INSERT INTO")
        {   // "INSERT INTO `agenda` ("
            int          pos   = 0;
            int          pos1  = line.find("VALUES",11);
            int          pos2  = line.find('(',11);
            if (pos1<pos2) pos = pos1;
            else           pos = pos2;
            if (pos !=-1)
               { table = line.mid(11, pos-11).stripWhiteSpace();
                 table = normaliseMajMinTableName(table);
                 line  = QString ("INSERT INTO ")+ table + line.mid(pos);
               }
            //............ isoler le verbe .................................................................
            long len_line;
            requete   ="";
            //..................... isoler les données à inserer de toutes les donnees INSERT............................................
            //                      (aller jusqu'au ');' de fin
            do      {
                       len_line = line.length();
                       if (len_line>2)
                          { while (line[len_line-1]=='\r' || line[len_line-1]=='\n')
                                  {--len_line;
                                  }
                            if (line[len_line-2]==')' && line[len_line-1]==';')
                               { requete += line;
                                 if (requete.length()>max_length_find)
                                    { max_length_find = requete.length();
                                      CGestIni::Param_UpdateToDisk(m_PathAppli+"MaxLastInsert.sql", requete);
                                    }
                                 CGestIni::Param_UpdateToDisk(m_PathAppli+"LastInsert.sql", requete);
                                 ParseSQL_InsertInto(requete, dataBase, pQProgressBar, logWidget );
                                 qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();
                                 break;
                               }
                          }
                       requete    += line;
                       m_position += readLine(&file, buffer, line, m_bufferSize);
                    } while (!file.atEnd());
        }
    }
 delete buffer;
 delete timer;
}

//-----------------------------------------------------  ParseSQL_InsertInto -------------------------------------------
// INSERT INTO `RubriquesHead` VALUES (1,'30DC82DD-7632-2C46-9DC3-BC1AC0ECD20A',20030000,'Asthme sai','2004-12-08 19:23:41','admin',0,0);
// INSERT INTO `RubriquesHead` VALUES (2,'30DC82DD-7632-2C46-9DC3-BC1AC0ECD20A',20020200,'Bilan labo complet','2004-12-08 19:23:41','admin',0,0);
// INSERT INTO `ACCES2` VALUES ('A','� foyer ouvert'),('A','par ... abord'),('A','par dissection ...');
// INSERT INTO `codes_postaux` (`id`,`code_postal`,`ville`) VALUES
//  (26662,62153,'ABLAIN ST NAZAIRE'),
//  (35076,80320,'ABLAINCOURT PRESSOIR');
void  CApp::ParseSQL_InsertInto(QString &text, QSqlDatabase *dataBase, QProgressBar * /* = 0 */, CW_EditText *logWidget /* = 0 */)
{   int pos,deb, len_sep, i, textLen;
    QSqlQuery query(QString::null, dataBase);
    deb = text.find("INSERT INTO");
    pos = text.find("VALUES");
    if (pos ==-1 || deb==-1)
       {Datasemp_OutMessage(logWidget,QObject::tr("    <font color=#ff0000><b>Erreur de syntaxe  INSERT INTO VALUES : ParseSQL_InsertInto() INSERT INTO or VALUES not found </b></font>"));
        return;
       }
    //............ isoler le verbe .................................................................
    pos         += 6;
    QString verb = text.mid(deb, pos-deb)+" ";  // "INSERT INTO `ACCES2` VALUES "
    //............ parser le reste pour les valeurs ................................................
    long end = -1;
    bool run = TRUE;
    //...................... se mettre sur premiere parenthese de donnees ........................................
    deb      = text.find("(", pos);
    if ( deb==-1)
       {Datasemp_OutMessage(logWidget,QObject::tr("    <font color=#ff0000><b>Erreur de syntaxe  INSERT INTO VALUES  ')' de début non trouvée  </b></font>"));
        return;
       }
    //....................... parser ................................................
    while (run)
    {
        end = findNextSep(text, deb, "),");               //==> chercher fin d'un bloc de donnees par le separateur '),'
        if (end != -1)                                    // si trouve '),' chercher si "(" suit immediatement (virer les \r \n \t ' ') jusqu'a le rencontrer
           {len_sep = 0;
            i       = end + 2;
            textLen = text.length();
            while (i<textLen && ( text[i]==' '||text[i]=='\r'||text[i]=='\n'||text[i]=='\t') )++i;
            if (i>=textLen || text[i]!='(')              //  '),' trouve sans rencontrer le "("
               { Datasemp_OutMessage(logWidget,QObject::tr("    <font color=#ff0000><b>Erreur de syntaxe  INSERT INTO VALUES : '),' trouvé mais '(' non trouvé </b></font>"));
                 return;
               }
            else
               { len_sep = (i-end);
               }
           }
        if (end==-1)                                      //==> si pas trouve chercher derniere insertion ");"
           {   end = findNextSep(text, deb, ");");
               if (end==-1)
                  { Datasemp_OutMessage(logWidget,QObject::tr("    <font color=#ff0000><b>Erreur de syntaxe  INSERT INTO VALUES :  '),' non trouvé mais ');' non plus</b></font>"));
                    return;
                  }
               else
                  { run = FALSE;
                  }
           }
        //........................ si donnees valide les inserer ...................................................
        if (end!=-1)
           {   QString requete = verb + text.mid(deb, end-deb+1);
               query.exec(requete);
               if (OutSQL_error(query, QObject::tr("\nERREUR INSERT INTO : ParseSQL_Dump()"), requete + "\n"))
                  {Datasemp_OutMessage(logWidget, QString( "<font color=#ff0000><b>ERREUR INSERT INTO : \n</b></font>") + m_LastError) ; // \n pour mettre m_LastError � la ligne
                  }
               if (run) deb = end + len_sep;
           }
    } //end while (run)
}

//-----------------------------------------------------  findNext -------------------------------------------
//
int  CApp::findNextSep(const QString & text, int pos, const QString &sep)
{int textLen = text.length();
 int  inside = FALSE;
 int  lenSep = sep.length();
 while (pos<textLen)
    {if (text[pos]=='\\')
        {++pos;
        }
     else if (text[pos]=='\'')
        {inside ^= 1;
        }
     else if (!inside                         &&   // ne pas etre inside chaine
               text[pos]            == sep[0] &&   // en testant prem car on gagne un peu de temps, sur un text.mid(pos,lenSep) direct
               text.mid(pos,lenSep) == sep
             ) return pos;
     ++pos;
    }
 return -1;
}


//-----------------------------------------------------  ExecSQL_File -------------------------------------------
int  CApp::ExecSQL_File(QSqlDatabase *dataBase, const QString &fname, QProgressBar *pQProgressBar /* = 0 */)
{//....................... ouvrir le fichier en mode QTextStream ...........................
    if ( !QFile::exists( fname ) )   return -1;
    QFile file( fname );
    if ( !file.open( IO_ReadOnly ) ) return -2;
    QTextStream stream ( &file );
    qDebug(QObject::tr("*************************** ") + fname + "*************************** ");
    //.......... on déclare une variable pour lire le fichier ligne à ligne ..................
    QString   buffer("");
    QSqlQuery query(QString::null, dataBase);
    while (!stream.atEnd())
    {   buffer    += stream.readLine().stripWhiteSpace();
        long len   = buffer.length();
        if ( buffer.at(len-1L)==';' &&
            (buffer.at(len-2L)==')' ||
            (len>7 &&  buffer.mid(len-7) == "MyISAM;" ) )
           )
           {query.exec(buffer);
            OutSQL_error(query, QObject::tr("ERREUR : ExecSQL_File()"), buffer );
            buffer = "";
            ++m_position;
            if (pQProgressBar) pQProgressBar->setProgress(m_position);
        }
    }
    return m_position;
}

//-----------------------------------------------------  CreationTables -------------------------------------------
int  CApp::CreationTables(QSqlDatabase *dataBase, const QString &f_name, QProgressBar *pQProgressBar /* = 0 */)
{if ( !QFile::exists( f_name ) )  return 0;
    QFile file( f_name );
    if ( !file.open( IO_ReadOnly ) ) return 0;

    QString requete( QTextStream ( &file ).read());
    QStringList list = QStringList::split(';',requete);

    QSqlQuery query(QString::null, dataBase);
    for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
    {requete = (*it).stripWhiteSpace();
        if (requete.length())
        {query.exec(requete);
            if (OutSQL_error(query, f_name, requete ) != QSqlError::None) return 0;
            ++m_position;
        }
    }
    if (pQProgressBar) pQProgressBar->setProgress(m_position);
    return m_position;
}

//-----------------------------------------------------  tryToConnectToDB -------------------------------------------
bool CApp::tryToConnectToDB(const QString &dataBaseName, QSqlDatabase *alreadyActiveBase )
{  QSqlDatabase *defaultDB = QSqlDatabase::addDatabase( alreadyActiveBase->driverName ()  , QString("DBASE_TEST_") + dataBaseName);
   if (defaultDB<=0) return false;     
   defaultDB->setDatabaseName( dataBaseName );                     // le shema
   defaultDB->setUserName( alreadyActiveBase->userName() );
   defaultDB->setPassword( alreadyActiveBase->password() );
   defaultDB->setHostName( alreadyActiveBase->hostName());
   defaultDB->setPort    ( alreadyActiveBase->port());
   bool ret  = defaultDB->open();
   QSqlDatabase::removeDatabase ( QString("DBASE_TEST_") + dataBaseName );
   return ret;
}



//------------------------------------------------ Datasemp_CreateBases --------------------------------------------
QSqlDatabase*  CApp::Datasemp_CreateBases( QString driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                           QString dataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                           QString user,          // = "root"
                                           QString passWord,      // = ""
                                           QString hostName,      // = "localhost"
                                           QString port,
                                           CW_EditText *logWidget,
                                           int         *isThisBaseExist
                                           )
{*isThisBaseExist = 0;
    QString          outMess  = "";
    QString          errMess  = "";
    int               errNum  = 0;
    QSqlDatabase* mysql_base  = 0;
    QSqlDatabase* dataBase    = 0;
    dataBase   = BaseConnect(   driver  ,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                dataBaseName,    // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                user,            // = "root"
                                passWord,        // = ""
                                hostName,        // = "localhost"
                                port,
                                "Datasemp",
                                &errMess,
                                &errNum);



    if ( dataBase == 0 )
    {
        if (errNum != 1049 && errNum != -1 && errNum != 0)
        { // La base existe mais problème de connection
            outMess  =          QObject::tr("============== La base ") + dataBaseName + QObject::tr(" existe mais connexion impossible ==========================\n") ;
            outMess += errMess + " " + QString::number(errNum) ;
            Datasemp_OutMessage(logWidget, outMess);
        }
        else
        {   outMess    =        QObject::tr("============== La base : ")+ dataBaseName +QObject::tr(" n'existe pas ================================================\n") ;
            outMess   += errMess + " " + QString::number(errNum) ;
            Datasemp_OutMessage(logWidget, outMess);
            mysql_base = GetMySQL_DataBase(user, passWord, hostName, port, logWidget);
            if (mysql_base)
            {//............... Création de la base ....................................
                outMess  =      QObject::tr("============== Création de la base ")+ dataBaseName +QObject::tr(" =================================================\n") ;
                Datasemp_OutMessage(logWidget, outMess);
                CreationBase(mysql_base, dataBaseName);
                // on se connecte à la nouvelle base
                dataBase =  BaseConnect( driver  ,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                         dataBaseName,    // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                         user,            // = "root"
                                         passWord,        // = ""
                                         hostName,        // = "localhost"
                                         port,
                                         "DatasempTest",
                                         &errMess,
                                         &errNum);
                if (dataBase==0)
                {   outMess  = QObject::tr("============== Échec de la création de la base : ")+ dataBaseName +QObject::tr(" ===========================================\n") ;
                    outMess += errMess + " " + QString::number(errNum) ;
                    Datasemp_OutMessage(logWidget, outMess);
                }
                else
                {//............... Ok la base a bien été créée ....................................
                    outMess  = QObject::tr("  La base de données ")+ dataBaseName +QObject::tr(" a été créée avec succès \n");
                    Datasemp_OutMessage(logWidget, outMess);
                    return dataBase;
                }
            }
        }
    }
    else
    {   outMess +=             QObject::tr("============== la base : ") + dataBaseName + QObject::tr(" existe déjà ===============================================\n") ;
        //outMess += errMess + " " + QString::number(errNum) ;
        Datasemp_OutMessage(logWidget, outMess);
        *isThisBaseExist = 1;
        return dataBase;
    }
    return 0;
}

//----------------------------------------------- Datasemp_GetBaseVersion ---------------------------------------------------------------------
QString CApp::Datasemp_GetBaseVersion(QSqlDatabase* dataBase, QString *numEdition /* = 0 */, QString *packBCArrete /* = 0 */, QString *packIRArrete /* = 0 */)
{   QString ods("");
    QString value("");
    if      (packBCArrete ) *packBCArrete = "";
    if      (packIRArrete)  *packIRArrete = "";
    if      (numEdition)    *numEdition = "";
    //................. Preparer la requete .....................................
    if (dataBase->isOpen()==FALSE && dataBase->open()==FALSE)              return ods;
    QSqlQuery query(QString::null, dataBase );
    query.exec("SELECT f0,f1 FROM tFB ");
    //.................. si la requète a un resultat ..............................................
    if (query.isActive())
    {while (query.next())
        { value = query.value(0).toString().upper();
            if      (packBCArrete && value=="ARRETE PACK BC") *packBCArrete = query.value(1).toString();
            else if (packIRArrete && value=="ARRETE PACK IR") *packIRArrete = query.value(1).toString();
            else if (numEdition   && value=="EDITION NUMERO") *numEdition   = query.value(1).toString();
            else if (value=="ODS")                             ods          = query.value(1).toString();
        }
    }
    else
    {OutSQL_error( query, QObject::tr("Erreur : Datasemp_GetBaseVersion()"));
    }
    return ods.remove("#00");
}

//----------------------------------------------- Datasemp_Maj_GetDiskVersion ---------------------------------------------------------------------
QString CApp::Datasemp_Maj_GetDiskVersion(const QString &fname)
{   QString ods("");
    //............ ouverture fichier ..................................
    if ( !QFile::exists( fname ) )                     return ods;
    QFile file( fname );
    if ( !file.open( IO_ReadOnly ) )                   return ods;

    //............ creation buffer de lecture ..........................
    long len_buffer_c = 36;
    char *buffer_c    = new char[len_buffer_c];
    if (buffer_c==0)                                   return ods;
    long nb_read = file.readBlock ( buffer_c, len_buffer_c);
    file.close();
    if (nb_read!=len_buffer_c)       {delete buffer_c; return ods;}
    ods.setLatin1(buffer_c+20,16);
    delete buffer_c;
    return ods;
}

//----------------------------------------------- Datasemp_GetDiskVersion ---------------------------------------------------------------------
//   on lit directement le fichier SQL TFB_D.SQL dans le repertoire DATA
//   INSERT INTO tFB (f0, f1) VALUES ('ARRETE PACK BC', '26/04/2011');
//   INSERT INTO tFB (f0, f1) VALUES ('ARRETE PACK IR', '22/03/2011');
//   INSERT INTO tFB (f0, f1) VALUES ('EDITION NUMERO', '173A');
//   -- l'enreg. NEWS n'est plus livre ;
//   INSERT INTO tFB (f0, f1) VALUES ('ODS', 'EBREPD0292007354');
QString CApp::Datasemp_GetDiskVersion(const QString &fname, QString *numEdition /* = 0 */, QString *packBCArrete /* = 0 */, QString *packIRArrete /* = 0 */)
{   if (numEdition)   *numEdition   ="";
    if (packBCArrete) *packBCArrete ="";
    if (packIRArrete) *packIRArrete ="";
    QString ods("");
    QString data = CGestIni::Param_UpdateFromDisk(fname);
    if (data.length()==0) return ods;

    int deb       = -1;
    int end       = -1;

    deb           = data.find("ODS'");             if (deb==-1) return ods;
    deb           = data.find("'", deb+4);         if (deb==-1) return ods;  ++deb;
    end           = data.find("'", deb);           if (end==-1) return ods;
    ods           = data.mid(deb,end-deb);

    deb           = data.find("ARRETE PACK BC'");  if (deb==-1) return ods;
    deb           = data.find("'", deb+15);        if (deb==-1) return ods;  ++deb;
    end           = data.find("'", deb);           if (end==-1) return ods;
    if (packBCArrete)  *packBCArrete  = data.mid(deb,end-deb);

    deb           = data.find("ARRETE PACK IR'");  if (deb==-1) return ods;
    deb           = data.find("'", deb+15);        if (deb==-1) return ods;  ++deb;
    end           = data.find("'", deb);           if (end==-1) return ods;
    if (packIRArrete)  *packIRArrete  = data.mid(deb,end-deb);

    deb           = data.find("EDITION NUMERO'");  if (deb==-1) return ods;
    deb           = data.find("'", deb+15);        if (deb==-1) return ods;  ++deb;
    end           = data.find("'", deb);           if (end==-1) return ods;
    if (numEdition)  *numEdition  = data.mid(deb,end-deb);

    return ods;
}
//----------------------------------------------- Datasemp_Maj_GetMajList ---------------------------------------------------------------------
long CApp::Datasemp_Maj_GetMajList(const QString &srcPath, const QString &baseVersion, QStringList &list)
{ long nb(0);
    list.clear();
    QDir dir (srcPath+"maj");
    dir.convertToAbs();
    dir.setFilter( QDir::All | QDir::NoSymLinks );
    const QFileInfoList * pQFileInfoList = dir.entryInfoList();
    if (pQFileInfoList==0)  return 0;

    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo     *fi;
    QString        diskVersion;
    while ( (fi = it.current()) != 0 )
    {QString fname = fi->fileName().latin1();
        if (fi->isFile())
        {diskVersion = Datasemp_Maj_GetDiskVersion(srcPath+"maj" + QDir::separator() + fname);
            //..................... verifier si mise à jour concerne notre base .................
            if (fname.mid(fname.findRev('.')+1)=="maj")
            {if (diskVersion==baseVersion)
                {list.append(QString("+ ") + fname);
                    ++nb;
                }
                else
                {list.append(QString("- ") + fname);
                }
            }
        }
        ++it;
    }
    return nb;
}

//----------------------------------------------- Datasemp_Maj_Update ---------------------------------------------------------------------
long CApp::Datasemp_Maj_Update(QSqlDatabase* pDataBase, const QString &srcPath, const QString &baseODS, UPDATE_MODE mode, CW_EditText *logWidget)
{   QString outMess = logWidget->text();
    if ( (int)(mode&CApp::MajFiles)==0 && (int)(mode&CApp::SqlFiles)==0 )
    {Datasemp_OutMessage(logWidget,  QObject::tr(" Mise à jour non effectuée car aucune option de mise à jour positionnée  " ));
        return 0;
    }
    long nb(0);
    long nb_sqlLine(0);
    QDir dir (srcPath+"maj");
    dir.convertToAbs();
    dir.setFilter( QDir::All | QDir::NoSymLinks );
    const QFileInfoList * pQFileInfoList = dir.entryInfoList();
    if (pQFileInfoList==0)
    {Datasemp_OutMessage(logWidget,  QObject::tr("  Aucun fichier de mise à jour trouvé dans ce repertoire !!!! " ));
        return 0;
    }
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo     *fi;
    QString        diskVersion;
    //................. fichier de sortie .........................................
    // FilesOut           = Out_Remove.txt , Out_Insert.txt  ,  Out_Update.txt
    QString out_Remove(""), out_Insert(""), out_Update("");
    CGestIni::Param_ReadParam(G_pCApp->m_ParamData,  "Datasemp", "FilesOut", &out_Remove, &out_Insert, &out_Update);
    if (out_Remove.length()) out_Remove.prepend(m_PathAppli);
    if (out_Insert.length()) out_Insert.prepend(m_PathAppli);
    if (out_Update.length()) out_Update.prepend(m_PathAppli);
    while ( (fi = it.current()) != 0 )
    {   QString fname = fi->fileName().latin1();
        if (fi->isFile())
        {   diskVersion = Datasemp_Maj_GetDiskVersion(srcPath+"maj" + QDir::separator() + fname);
            //..................... verifier si mis à jour concerne notre base ............
            if (mode&CApp::MajFiles && fname.mid(fname.findRev('.')+1)=="maj" && (diskVersion==baseODS||mode&CApp::Force))
            {//............... Remove en cours ....................................
                Datasemp_OutMessage(logWidget,  QObject::tr("  Mise à jour Datasemp livraison : ") + diskVersion + QObject::tr("    effacement des données en cours ....")) ;
                Datasemp_Maj_Update(srcPath+"maj"+QDir::separator()+fname, nb_sqlLine, CApp::Remove, pDataBase, out_Remove);
                //............... Insert en cours ....................................
                Datasemp_OutMessage(logWidget,  QObject::tr("  Mise à jour Datasemp livraison : ") + diskVersion + QObject::tr("    insertion des données en cours .... " ));
                Datasemp_Maj_Update(srcPath+"maj"+QDir::separator()+fname, nb_sqlLine, CApp::Insert, pDataBase, out_Insert);
                //............... Update en cours ....................................
                Datasemp_OutMessage(logWidget,  QObject::tr("  Mise à jour Datasemp livraison : ") + diskVersion + QObject::tr("    mise à jour des données en cours ....")) ;
                Datasemp_Maj_Update(srcPath+"maj"+QDir::separator()+fname, nb_sqlLine, CApp::Update, pDataBase, out_Update);
                ++nb;
            }// end if (fname.mid(fname.findRev('.')+1)=="maj" && (diskVersion==baseODS||mode==CApp::Force))
            else if (mode&CApp::SqlFiles && fname.mid(fname.findRev('.')+1)=="sql")
            {//............... Execution SQL en cours ....................................
                Datasemp_OutMessage(logWidget,  QObject::tr("  Mise à jour Datasemp : fichier SQL :  ") + QObject::tr("    en cours d'exécution....")) ;
                ExecSQL_File(pDataBase, srcPath+"maj"+QDir::separator()+fname);
                ++nb;
            }
        } // end if (fi->isFile())
        ++it;
    }
    if (nb) Datasemp_OutMessage( logWidget,  QObject::tr("  Mise à jour Datasemp terminée de : " ) + QString::number(nb_sqlLine) + QObject::tr(" enregistrements"));
    else    Datasemp_OutMessage( logWidget,  QObject::tr("  Pas de fichier de mise à jour trouvés : " ));
    return nb_sqlLine;
}

//----------------------------------------------- Datasemp_OutMessage ---------------------------------------------------------------------
void  CApp::Datasemp_OutMessage( CW_EditText *logWidget, const QString &mess)
{if (logWidget==0)  return;
    logWidget->append( mess );
    logWidget->scrollToBottom ();
    //logWidget->moveCursor(QTextEdit::MoveEnd);
    //logWidget->ensureCursorVisible ();
    //hasPendingEvents ();
    //qApp->flush();
    //qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();
    qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();
}

//----------------------------------------------- Datasemp_Maj_Update ---------------------------------------------------------------------
QString CApp::Datasemp_Maj_Update(const QString &fname,
                                  long &nb_sqlLine,
                                  UPDATE_MODE mode,          // = CMedicaBase::AllUpdate
                                  QSqlDatabase* pDataBase,   // = 0  si pas egal zero alors les requetes SQL de mise à jour seront executées
                                  const char* fileOut        // = 0  si pas egal zero alors le fichier SQL de mise à jour sera ecrit
                                  )
{long nb_read = 0;
    QString table;
    QString sql("");
    char *end;
    char *pt;

    //............ ouverture fichier ..................................
    if ( !QFile::exists( fname ) )                     return sql;
    QFile file( fname );
    if ( !file.open( IO_ReadOnly ) )                   return sql;

    //............ creation buffer de lecture ..........................
    long len_buffer_c = file.size();
    char *buffer_c = new char[len_buffer_c];
    if (buffer_c==0)                                   return sql;
    nb_read = file.readBlock ( buffer_c, len_buffer_c);
    file.close();
    if (nb_read==0)                  {delete buffer_c; return sql;}

    //................. lire l'entete ..................................
    QString head("");
    head.setLatin1(buffer_c, 68);
    //................. parser le fichier ..............................

    end = buffer_c + len_buffer_c - 93;      // - 93 bloc de fin
    pt  = buffer_c + 68;
    while (pt<end)
    {switch(pt[0])
     {case'[':
      table.setLatin1(pt + 1, 2);  table.prepend("t");
            pt += 15;
            break;
        case '~':
            pt += 2;
            if      (pt[-1]=='M') sql += Datasemp_Maj_UpdateInTable(pt, end, table, mode & CApp::Update, nb_sqlLine, pDataBase);
            else if (pt[-1]=='C') sql += Datasemp_Maj_InsertInTable(pt, end, table, mode & CApp::Insert, nb_sqlLine, pDataBase);
            else if (pt[-1]=='S') sql += Datasemp_Maj_DeleteInTable(pt, end, table, mode & CApp::Remove, nb_sqlLine, pDataBase);
            break;
        default: ++pt;
        }
    }
    if (fileOut)
    {QFile fileW(fileOut);
        if ( !fileW.open( IO_WriteOnly ) )    {delete buffer_c; return sql;}
        fileW.writeBlock (sql , sql.length());
        fileW.close();
    }
    delete buffer_c;
    //.......... on déclare une variable pour le lire ligne à ligne ..................
    return sql;
}

//----------------------------------------------- Datasemp_Maj_UpdateInTable ---------------------------------------------------------------------
// ENTREE: ptIn  pointe sur le signe $  ( celui de  ~M$ )
QString CApp::Datasemp_Maj_UpdateInTable(char* &pt, char* end, const QString &table, int doIt, long &nb_sqlLine, QSqlDatabase* pDataBase)
{if (doIt==0)
    {while (pt < end && *pt != '~' && *pt != '[') ++pt;
        return QString("");
    }
    //................... extraire valeur de la clef ......................................
    QString key        = QString("f") + pt[1];    pt+=2;
    QString key_value  = Datasemp_Maj_ExtractValue(pt, end);
    QString data_sql   = QString("UPDATE ") + table + " SET  ";

    //................... extraire autres valeurs ..........................................
    int     n   = 0;
    while (pt < end && *pt != '~' && *pt != '[')
    {//                          @1345   Premiére valeur apres le @ est le N°champ,
        //                                  la deuxième jusqu'à fin (@ autre champ ou ~ fin des champs) est la valeur du champ
        if (*pt == '@')
        {if (n++) data_sql += " , ";     // rajouter virgule de separation si besoin
            data_sql    += QString("f") + pt[1];  pt+=2;
            data_sql    += "='" + Datasemp_Maj_ExtractValue(pt, end).replace('\'','`') + "'";
        }
    }
    data_sql += "  WHERE ( " + key + "='" + key_value + "' );\r\n";
    if (pDataBase)
    { QSqlQuery query(QString::null, pDataBase);
        query.exec(data_sql);
        OutSQL_error( query, QObject::tr("Erreur : Datasemp_Maj_UpdateInTable()"), data_sql);         //8EBREPD0212005271
    }
    ++nb_sqlLine;
    return data_sql;
}

//----------------------------------------------- Datasemp_Maj_DeleteInTable ---------------------------------------------------------------------
// ENTREE: ptIn  pointe sur le signe $  ( celui de  ~S$ )
QString CApp::Datasemp_Maj_DeleteInTable(char* &pt, char* end, const QString &table, int doIt, long &nb_sqlLine, QSqlDatabase* pDataBase)
{if (doIt==0)
    {while (pt < end && *pt != '~' && *pt != '[') ++pt;
        return QString("");
    }
    //............................... la commence le truc ......................................
    QString data_sql = QString("DELETE FROM ") + table + " WHERE ( ";
    int n            = 0;
    while (pt < end && *pt != '~' && *pt != '[')
    {//                          @1345   Premiére valeur apres le @ est le N°champ,
        //                                  la deuxième jusqu'à fin (@ autre champ ou ~ fin des champs) est la valeur du champ
        if (n++) data_sql += " AND ";    // rajouter virgule de separation si besoin
        data_sql          += QString("f") + pt [1] + "='"; pt+=2;
        while (pt < end && *pt != '~' && *pt != '[' && *pt != '$' && *pt==' ') ++pt;
        data_sql += Datasemp_Maj_ExtractValue(pt, end, "$[~\0x00\0x00")+ "'";
    }
    data_sql +=  " );\r\n";
    if (pDataBase)
    {QSqlQuery query(QString::null, pDataBase);
        query.exec(data_sql);
        OutSQL_error( query, QObject::tr("Erreur : Datasemp_Maj_DeleteInTable()"), data_sql);
    }
    ++nb_sqlLine;
    return data_sql;
}

//----------------------------------------------- Datasemp_Maj_InsertInTable ---------------------------------------------------------------------
// ENTREE: ptIn  pointe sur le signe $  ( celui de  ~C$ )
//         [4G0200000000053~C$0C00510$1C03234~C$    peut aussi exister si f0 et f1 font partie de la clef     !!!!
//         [4G0200000000053~C$0C00510@1C03234~C$    peut aussi exister si f0 fait partie de la clef et pas f1 !!!!
QString CApp::Datasemp_Maj_InsertInTable(char* &pt, char* end, const QString &table, int doIt, long &nb_sqlLine, QSqlDatabase* pDataBase)
{if (doIt==0)
    {while (pt < end && *pt != '~' && *pt != '[') ++pt;
        return QString("");
    }
    //............................... la commence le truc ......................................
    QString data_sql   = QString("INSERT INTO ") + table + " ( ";
    int  n             = 0;
    QString field_list = "";
    QString value_list = "'";
    while (pt < end && *pt != '~' && *pt != '[')
    {//                          @1345   Premiére valeur apres le @ est le N°champ,
        //                                  la deuxième jusqu'à fin (@ autre champ ou ~ fin des champs) est la valeur du champ
        if (n++) {field_list += " , ";  value_list += " , '";}   // rajouter virgule de separation si besoin

        field_list      += QString("f") + pt[1];  pt+=2;
        while (pt < end && *pt != '~' && *pt != '[' && *pt != '$' && *pt==' ') ++pt;       // passer espaces inutiles
        value_list  += Datasemp_Maj_ExtractValue(pt, end, "$@[~\0x00").replace('\'','`') + "'";
    }
    data_sql += field_list + " ) VALUES ( " + value_list + " );\r\n";
    if (pDataBase)
    {  QSqlQuery query(QString::null, pDataBase);
        query.exec(data_sql);
        OutSQL_error( query, QObject::tr("Erreur : Datasemp_Maj_InsertInTable()"), data_sql);
    }
    ++nb_sqlLine;
    return data_sql;
}

//----------------------------------------------- Datasemp_Maj_ExtractValue ---------------------------------------------------------------------
QString CApp::Datasemp_Maj_ExtractValue(char* &pt, const char* end, const char* sep_str)
{char   *deb = pt;
    QString key_value("");
    while (pt < end &&  *pt != sep_str[0] && *pt != sep_str[1] && *pt != sep_str[2] && *pt != sep_str[3])
    {if (*pt=='#')
        {key_value += QString::fromLatin1(deb, pt-deb);
            ++pt;
            key_value += Datasemp_Maj_DecodeSpecialCar(QString::fromLatin1(pt, 2));
            pt += 2;
            deb = pt;
        }
        else
        {++pt;
        }
    }
    key_value += QString::fromLatin1(deb, pt-deb);
    return key_value;
}

//----------------------------------------------- Datasemp_Maj_DecodeSpecialCar ---------------------------------------------------------------------
QString CApp::Datasemp_Maj_DecodeSpecialCar(const QString &val)
{QString ret;
                              //  0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F      00
    QString DECODE_TBL[256] = {  "", "?", "?", "?", "?", "?", "?", "?", "?",  "\t","\n", "?", "?","\r", "?", "?",\
                              // 10   11   12   13   14   15   16   17   18   19   1A   1B   1C   1D   1E   1F      01
                              "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",\
                              // 20   21   22   23   24   25   26   27   28   29   2A   2B   1C   2D   2E   2F      02
                              "?", "?","\"", "#", "$", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",\
                              // 30   31   32   33   34   35   36   37   38   39   3A   3B   3C   3D   3E   3F      03
                              "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",\
                              // 40   41   42   43   44   45   46   47   48   49   4A   4B   4C   4D   4E   4F      04
                              "@", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",\
                              // 50   51   52   53   54   55   56   57   58   59   5A   5B   5C   5D   5E   5F      05
                              "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "[","\\", "]", "^", "_",\
                              // 60   61   62   63   64   65   66   67   68   69   6A   6B   6C   6D   6E   6F      06
                              "\"", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",\
                              // 70   71   72   73   74   75   76   77   78   79   7A   7B   7C   7D   7E   7F      07
                              "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "{", "|", "}", "~", "?",\
                              // 80   81   82   83   84   85   86   87   88   89   8A   8B   8C   8D   8E   8F      08
                              "?", "?", ",", "§","\"", "_", "¡", "?", "^", "?", "?", "«", "?", "?", "?", "?",\
                              // 90   91   92   93   94   95   96   97   98   99   9A   9B   9C   9D   9E   9F      09
                              "?", "?","\"","\"","\"", "·", "-", "-", "~", "?", "s", "»", "æ", "?", "?", "ÿ",\
                              // A0   A1   A2   A3   A4   A5   A6   A7   A8   A9   AA   AB   AC   AD   AE   AF      0A
                              "?", "¡", "?", "£", "?", "¥", "?", "§", "?", "©", "ª", "«", "¬", "-", "®", "-",\
                              // B0   B1   B2   B3   B4   B5   B6   B7   B8   B9   BA   BB   BC   BD   BE   BF      0B
                              "°", "±", "²", "³", "?", "µ", "¶", "·", "?", "¹", "º", "»", "?", "?", "?", "¿",\
                              // C0   C1   C2   C3   C4   C5   C6   C7   C8   C9   CA   CB   CC   CD   CE   CF      0C
                              "À", "Á", "Â", "Ã", "Ä", "Å", "Æ", "Ç", "È", "É", "Ê", "Ë", "Ì", "Í", "Î", "Ï",\
                              // D0   D1   D2   D3   D4   D5   D6   D7   D8   D9   DA   DB   DC   DD   DE   DF      0D
                              "Ð", "Ñ", "Ò", "Ó", "Ô", "Õ", "Ö", "×", "Ø", "Ù", "Ú", "Û", "Ü", "Ý", "Þ", "ß",\
                              // E0   E1   E2   E3   E4   E5   E6   E7   E8   E9   EA   EB   EC   ED   EE   EF      OE
                              "à", "à", "â", "ã", "ä", "å", "æ", "ç", "è", "é", "ê", "ë", "ì", "í", "î", "ï",\
                              // F0   F1   F2   F3   F4   F5   F6   F7   F8   F9   FA   FB   FC   FD   FE   FF      0F
                              "ð", "ñ", "ò", "ó", "ô", "õ", "ö", "÷", "ø", "ù", "ú", "û", "ü", "ý", "þ", "ÿ"
                          };
    if       (  val ==  "EU") ret = "euro";
    else if  (  val ==  "9C") ret = "oe";
    else if  (  val ==  "8C") ret = "OE";
    else if  (  val ==  "85") ret = "...";
    else if  (  val ==  "89") ret = "°/oo";
    else if  (  val ==  "99") ret = "TM";
    else if  (  val ==  "00") ret = "";
    else                      ret = DECODE_TBL[val.toInt(0,16)];
    return ret;
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
        qDebug(m_LastError);
        return 1;
    }
    //     qDebug(qserr + error.databaseText ());
    return 0;
}

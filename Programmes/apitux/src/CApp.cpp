/********************************* CApp.cpp ***************************************
 *                                                                                *
 *   #include CApp.h"                                                             *
 *   Project: MedinTux                                                            *
 *   Copyright (C) for the eternity by Sevin Roland  and Data Medical Design      *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat �   l'Energie Atomique                                           *
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

#include "CApp.h"

#include <QFileInfo>
#include <QFile>

#include <QStringList>
#include <QTextStream>
#include <QIODevice>
#include <QProcess>
#include <QSqlError>
#include <QTextCodec>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDebug>
#include "../../MedinTuxTools-QT4/Theme/Theme.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"

CApp* G_pCApp = 0;  // contiendra l'instance globale de l'application


//--------------------------------------------- C_App -------------------------------------------------------------------
CApp::~CApp()
{
}

//--------------------------------------------- CApp -------------------------------------------------------------------
CApp::CApp(QString mui_name, int & argc, char ** argv)
:C_AppCore(mui_name, argc,argv)
{   QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
    m_BaseLabel         = "DB_FOR_APITUX";
    m_result_point      = "";
    m_result_family     = "";
    m_hideInfoWidget    = true;
    QString value;
    //.............. recuperer le .ini .....................................................................
    QString file = pathAppli()+"Ressources/apitux.ini";
    QFile fichier(file);
    if(!fichier.open(QIODevice::ReadOnly | QIODevice::Text))
      { QMessageBox::warning(0,"erreur", QObject::trUtf8("le fichier %1 n'est pas accessible").arg(file),QMessageBox::Ok);
      }
    else
      {
        QSettings set(file,QSettings::IniFormat);
        //.............. police de caractere pour l'affichage html...............................................
        m_result_point      = set.value("Font/PointSize").toString().trimmed();
        m_result_family     = set.value("Font/Family").toString().trimmed();
        //.............. numero de version pour la conversion html...............................................
        m_convertVersion = set.value("version/major").toString().trimmed().toInt();
        //.............. la fenetre d'info (inutile avec l'aide en ligne) .......................................
        value = set.value("interface/show_info_widget").toString().toLower().trimmed();
        // qWarning() << __FILE__ << QString::number(__LINE__)<<Q_FUNC_INFO<< "show info widget"<<value ;
        m_hideInfoWidget =  !(value=="true");
      }
    if (m_result_point.isEmpty())   m_result_point  = "12";
    if (m_result_family.isEmpty())  m_result_family = "Courier";
    if (m_convertVersion==0)        m_convertVersion = 14;


    //setWindowIcon (QPixmap(m_PathIcone));
    G_pCApp    =    this;
}
//--------------------------------------------- pCApp -------------------------------------------------------------------
/*! \brief retourne l'instance de l'application. est a zero tant que l'initialisation n'est pas finie.
 * l'acces ulterieur aux fonctions de la classe CApp se fait par CApp::pCApp()
 */
CApp* CApp::pCApp()
{return G_pCApp;
}
//---------------------------------------- database -------------------------------------------------------
QSqlDatabase CApp::database()
{ return QSqlDatabase::database(m_BaseLabel);
}

//---------------------------------------- CreateConnection -------------------------------------------------------
bool CApp::createConnection()
{   QString baseLabel = m_BaseLabel;
    QString hostname    = "";
    QString driver      = "";
    QString user        = "";
    QString password    = "";
    QString port        = "";
    //.............. recuperer le .ini .....................................................................
    QString file = CApp::pCApp()->pathAppli()+"Ressources/apitux.ini";
    QFile fichier(file);
    if (!fichier.open(QIODevice::ReadOnly | QIODevice::Text))
       {
        QMessageBox::warning(0,"erreur", QObject::trUtf8("le fichier %1 n'est pas accessible").arg(file),QMessageBox::Ok);
        return false;
       }
    QSettings set(file,QSettings::IniFormat);
    //.............. recuperer les parametres de connexion a la base .......................................
    driver      = set.value("connexion/driver").toString().trimmed();
    hostname    = set.value("connexion/host").toString().trimmed();
    user        = set.value("connexion/utilisateur").toString().trimmed();
    password    = set.value("connexion/motdepassemysql").toString().trimmed();
    port        = set.value("connexion/port").toString().trimmed();
    if(port.isEmpty()) port = "3306";

    if (password.startsWith('#'))   password = CGestIni::PassWordDecode(password.mid(1));
    //.............. se connecter a la base .......................................
    QSqlDatabase::removeDatabase (baseLabel);
    QSqlDatabase defaultDB  = QSqlDatabase::addDatabase(driver, baseLabel);
    defaultDB.setDatabaseName( "DrTuxTest" );
    defaultDB.setUserName    ( user     );
    defaultDB.setPassword    ( password );
    defaultDB.setHostName    ( hostname );
    defaultDB.setPort        ( port.toInt() );

    if ( ! defaultDB.open() )
       {
            QMessageBox::critical(0,QString("Erreur ouverture de base : %1<br/>"
                                            "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>host</b>   : %2 <br/>"
                                            "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>driver</b> : %3 <br/>"
                                            "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>user</b>   : %4 <br/>"
                                            "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>port</b>   : %5<br/> "
                                            "----------------").arg("DrTuxTest",hostname,driver,user,port), defaultDB.lastError().text());
            return false;
        }
  return true;
}

//---------------------------------------- convertToHtml -------------------------------------------------------
QString CApp::convertToHtml(const QString &_text)
{QString text = _text;
 switch(m_convertVersion){
    case 12 :
         qWarning() << __FILE__ << QString::number(__LINE__)<<Q_FUNC_INFO<< "version 12" ;
         text = QString("<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\" ?><MedinTux_Multimedia_data>"
                        "<HTML_Data><html><head><meta name=\"qrichtext\" content=\"1\" /></head>"
                        "<body style=\"font-size:%1pt;font-family:%2\"><p>%3</p></body></html></HTML_Data><ImageListe></ImageListe></MedinTux_Multimedia_data>")
                        .arg( CApp::pCApp()->result_point(),
                              CApp::pCApp()->result_family(),
                              text);
         break;
    case 14 :
         qWarning() << __FILE__ << QString::number(__LINE__) <<Q_FUNC_INFO<< "version 14" ;
         text = QString("<html><head><meta name=\"qrichtext\" content=\"1\" />""</head>"
                         "<body style=\"font-size:%1pt;font-family:%2\"><p>%3</p></body></html>")
                         .arg( CApp::pCApp()->result_point(),
                               CApp::pCApp()->result_family(),
                               text);
         break;
    default :
         break;
    }
return text;
}

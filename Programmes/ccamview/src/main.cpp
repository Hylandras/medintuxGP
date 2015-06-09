
/***************************************************************************
 *   Copyright (C) 2003 by Sevin Roland                                    *
 *   upsis@club-internet.fr                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <qapplication.h>
#include <qmessagebox.h>
#include <qdir.h>
#include <qstring.h>
#include <qlineedit.h>
#include <qlineedit.h>
#include <stdlib.h>
#include <qtextcodec.h>
#include <qobject.h>
#ifdef Q_WS_X11
  #include <QX11Info>
#endif
#include "Global.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "C_Dlg_FeuillePrincipale.h"
#include "C_Dlg_login.h"
#ifdef Q_WS_X11
    #define NAME_APPLI        "ccamview"
    #define NAME_APPLI_EXT    ""
#else
    #define NAME_APPLI        "ccamview"
    #define NAME_APPLI_EXT    ".exe"
#endif

#define DRIVER            "QMYSQL3"
#define DATABASE          "CCAM"
#define USER              ""
#define PASSWORD          ""
#define HOST              ""

//=============================================== VARIABLES GLOBALES FONDAMENTALES =====================================================
//                            �  instanciation UNIQUE,  Pour les rendre accessibles de partout
//........................... Chemins Frequements utilisés ............................................................................
 QString       GlobalPathAppli              = "";                // chemin de l'Applicatif
 QString       Global_PathIni               = "";
 QString       Global_ParamData             = "";
 //int           Global_DebugFlag             = 1;                 // zero si pas de mode debug
 //QString       Global_DebugStr              = "";                 // zero si pas de mode debug
//........................... Variables Globales fondamentales ........................................................................
//                            Pour les rendre accessibles de partout


//=============================================== MAIN IMPLEMENTATION ==================================================================
//-------------------------------------------------------------- main ------------------------------------------------------------------
int main( int argc, char ** argv )
{   QString    errMess      = "";
    int        errNum       = 0;
    QString    drv_type     = "";
    QString    base_name    = "";
    QString    user         = "";
    QString    password     = "";
    QString    hostname     = "";
    QString    qstr         = "";
    QString    base_secours = "mysql";

    //.............................. instancier notre application ..................................................
    QApplication a( argc, argv );
    QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );

    //............................... determiner le chemin de l'application ........................................
    GlobalPathAppli            = CGestIni::Construct_PathBin_Module( "ccamview", QFileInfo (qApp->argv()[0]).dirPath (true));

    //.............. lire dans le .ini les paramètres de connexion
    if (argc > 1 && argv[1][0])         Global_PathIni = QString(argv[1]);
    if (Global_PathIni.length()==0)     Global_PathIni = GlobalPathAppli + NAME_APPLI + ".ini";
    //............... charger les parametres ...............................................................
    CGestIni::Param_UpdateFromDisk(Global_PathIni, Global_ParamData);

    //if (CGestIni::Param_ReadParam(Global_ParamData, "Session", "Debug mode", &debg)==0)  // zero = pas d'erreur
    //   {if (debg.lower()=="oui") Global_DebugFlag = 1;
    //   }
    //for (int i=0; i<argc; ++i) Global_DebugStr.append(QString::number(i)+"   "+argv[i]+"\n");

    //.......................Charger les parametres de connexion ....................................................
    if (CGestIni::Param_ReadParam(Global_ParamData, "Connexion", "Parametres", &drv_type, &base_name, &user, &password, &hostname) != QString::null )  // zero = pas d'erreur
       { drv_type     = DRIVER;
         base_name    = DATABASE;
         user         = USER;
         password     = PASSWORD;
         hostname     = HOST;
       }
    // On crée le feuille ici pour utiliser ses fonctions .....................................
    //........................ creation du moteur de base de données ......................................
    //                         gere interractions entre la base et l'application
    CMoteurCCAM_Base  *pCMoteurCCAM_Base = new CMoteurCCAM_Base(drv_type,         // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                                                base_name,        // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                                                user,             // = "root"
                                                                password,         // = ""
                                                                hostname,
                                                                &errMess,
                                                                &errNum);
    //.......................... si erreur afficher mesage erreur et  cassos ................................
   // QStringList tableList = pCMoteurCCAM_Base->m_pCCAM_Base->tables ();
    QMessageBox *mb;
    if (pCMoteurCCAM_Base->m_pCCAM_Base==0)
       {
        if (errNum != 1049 && errNum != -1)
           { // La base existe mais problème de connection
            QMessageBox::critical (0, NAME_APPLI ,
                                   errMess + " " + QString::number(errNum),
                                   QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
            delete pCMoteurCCAM_Base;
            return -1;
           }
        else
           { // la base n'existe pas et on peut la créer
            // message d'alerte
            /*QMessageBox*/ mb = new QMessageBox( QObject::tr("ATTENTION !!!"),
                             QObject::tr("La base de données ") + base_name + QObject::tr(" n'existe pas\n"
                             "\n"
                             "Pour vérifier vos paramètres de connection :") +
                             QObject::tr("\n    Driver       : ")+ drv_type +
                             QObject::tr("\n    Utilisateur  : ")+ user     +
                             QObject::tr("\n    Mot de passe : ")+ password +
                             QObject::tr("\n    Serveur      : ")+ hostname +
                             QObject::tr("\n    Driver       : ")+ drv_type +
                             QObject::tr("\n cliquez sur [Annuler]"
                                "\n Pour Créer la base de données nommée ") + base_name + QObject::tr(" cliquez sur [Créer]"),
                            QMessageBox::Information,
                            QMessageBox::Yes,
                            QMessageBox::Cancel  | QMessageBox::Default,0);
            mb->setButtonText( QMessageBox::Yes, QObject::tr("Créer" ));
            mb->setButtonText( QMessageBox::Cancel, QObject::tr("Annuler"));
            switch( mb->exec() )
            {
             case QMessageBox::Yes:
                // on se connecte �  la base mysql obligatoirement créée �  l'installation de mysql server
                delete mb;
                pCMoteurCCAM_Base = new CMoteurCCAM_Base(drv_type,
                                                         base_secours,
                                                         user,
                                                         password,
                                                         hostname,
                                                         &errMess,
                                                         &errNum);
                if (pCMoteurCCAM_Base->m_pCCAM_Base==0)
                   { // si problème cassos
                    QMessageBox::critical (0, NAME_APPLI ,
                                           errMess + " " + QString::number(errNum),
                                           QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
                    delete pCMoteurCCAM_Base;
                    return -1;
                   }
                // Création de la base
                pCMoteurCCAM_Base->CreationBase(base_name);
                // on se connecte �  la nouvelle base
                pCMoteurCCAM_Base = new CMoteurCCAM_Base(drv_type,
                                                         base_name,
                                                         user,
                                                         password,
                                                         hostname,
                                                         &errMess,
                                                         &errNum);
                if (pCMoteurCCAM_Base->m_pCCAM_Base==0)
                   { // si problème cassos
                    QMessageBox::critical (0, NAME_APPLI ,
                                           errMess + " " + QString::number(errNum),
                                           QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
                    delete pCMoteurCCAM_Base;
                    return -1;
                   }
                pCMoteurCCAM_Base->CreationTables();
                /*QMessageBox*/ mb = new QMessageBox( "CCAMVIEW",
                                QObject::tr("La base de données ") + base_name + QObject::tr(" a été créée avec succés\n"
                                "\n"
                                "Voulez-vous exécuter CCAMVIEW\n"),
                                QMessageBox::Information,
                                QMessageBox::Yes,
                                QMessageBox::Cancel  | QMessageBox::Default,0);
                mb->setButtonText( QMessageBox::Yes, QObject::tr("Oui") );
                mb->setButtonText( QMessageBox::Cancel, QObject::tr("Non"));
                switch( mb->exec() )
                  { case QMessageBox::Yes:
                         break;
                    case QMessageBox::Cancel:
                         delete pCMoteurCCAM_Base;
                         return -1;
                    break;
                  }
                break;
            case QMessageBox::Cancel:
                delete pCMoteurCCAM_Base;
                return -1;
            }
        }
    }
    //.......................Charger les parametres de connexion PRATICIENS....................................................
    if (CGestIni::Param_ReadParam(Global_ParamData,  "Praticiens", "Connexion", &drv_type, &base_name, &user, &password, &hostname) == QString::null )  // zero = pas d'erreur
       {pCMoteurCCAM_Base->ConnectUserBase(drv_type,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                           base_name,       // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                           user,            // = "root"
                                           password,        // = ""
                                           hostname
                                          );
       }
    //.............................. initialisation de la base de données  PRATICIENS ...........................................
    //                               le fichier de configuration "DataBase.cfg" doit
    //                               etre au même niveau que le fichier .ini
    if (argc > 1 && argv[1][0])
       {QString path = QString(argv[1]); int pos = path.findRev('/'); if (pos != -1) path = path.left(pos);
        if (pCMoteurCCAM_Base->SetConfBase( path + QDir::separator() + "DataBase.cfg", &errMess)==0)
           {QMessageBox::critical (0, NAME_APPLI ,
                                   errMess ,
                                   QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
            delete pCMoteurCCAM_Base;
            return 0;
           }
       }
    else
       {pCMoteurCCAM_Base->SetConfBase( GlobalPathAppli +"DataBase.cfg", &errMess);
       }


   // ................. GESTION DU LOGIN ..........................................................................................
   QString code_medecin = "";
   QString last_user    = "";
   QString passw        = "";
   int     do_log       = TRUE;
   CGestIni::Param_ReadParam(Global_ParamData,  "Session", "Dernier utilisateur", &last_user);

   //............................... utilisateur fourni dans le paramètres d'appel ................................................
   if (qApp->argc()>=23)
      {code_medecin = qApp->argv()[ 22 ];
       int pos = code_medecin.find(':');
       if ( pos != -1) code_medecin = code_medecin.left(pos);
       do_log = FALSE;   // pas de log si paramètres de connexion en arguments d'appel
      }
   //............................... chercher si le dernier utilisateur n'est pas en connexion auto ...............................
   //                                CAD  mot de passe = NOPASSW
   if (code_medecin == "" && last_user.length() )  // zero = pas d'erreur
      {bool b = pCMoteurCCAM_Base->VerificationLogin(last_user,  "NOPASSW",  &passw);
       if  (b==FALSE)
               {b = pCMoteurCCAM_Base->VerificationLogin(last_user, "DEFAULT",  &passw);
               }
       if  (b==TRUE)  do_log = TRUE;
       else           do_log = FALSE;
       code_medecin = last_user;
      }
   // ................. affiche le dialogue LOGIN .................................................................................
   if (do_log)
      {C_Dlg_login* log   = new C_Dlg_login(pCMoteurCCAM_Base, last_user, passw);
       if (log->exec()== QDialog::Rejected) {delete log; delete pCMoteurCCAM_Base;  return -1;}
       code_medecin = log->lineEdit_Utilisateur->text();
       last_user    = code_medecin;
       delete log;
      }
   CGestIni::Param_WriteParam(&Global_ParamData,  "Session", "Dernier utilisateur", last_user );

   //.................... ajuster resolution d'impression (sous X seulement) .....................................................
   #ifdef Q_WS_X11
    //QX11Info screenInfo;
    int screen = QX11Info().screen();
    QString reso_x(""), reso_y("");
    if (CGestIni::Param_ReadParam(Global_ParamData, "Impression", "Resolution", &reso_x, &reso_y)==QString::null)  // zero = pas d'erreur
       {QPaintDevice::x11SetAppDpiX(atoi(reso_x),screen);
        QPaintDevice::x11SetAppDpiY(atoi(reso_y),screen);
       }
   #endif
    //........................ si Ok alors on creer le dialogue et on l'affiche ..................................................
    C_Dlg_FeuillePrincipale* w   = new C_Dlg_FeuillePrincipale();
    w->initDialog(pCMoteurCCAM_Base, code_medecin);
    w->setCaption( QString("CCAM View V") + pCMoteurCCAM_Base->GetCCAM_Version() + QObject::tr(" Copyright (C) 2004 to 2100 by Sébastien SIVERA & Roland SEVIN  user : ") + code_medecin);
    //#ifdef Q_WS_X11
    QString x,y,width,height;
    if (CGestIni::Param_ReadParam(Global_ParamData,  "Session", "Full Screen", &x) ==QString::null )
       { if (x.left(1).lower() == "o")
            { w->showMaximized();
            }
         else if (CGestIni::Param_ReadParam(Global_ParamData,  "Session", "Position", &x, &y, &width, &height) ==QString::null )             // zero = pas d'erreur
            {
             //w->setGeometry (atoi(x),atoi(y),atoi(width), atoi(height));
             w->move(x.toInt(), y.toInt());
             w->resize(width.toInt(), height.toInt());
            }
       }
    else if (CGestIni::Param_ReadParam(Global_ParamData,  "Session", "Position", &x, &y, &width, &height) ==QString::null )             // zero = pas d'erreur
       { //w->setGeometry (atoi(x),atoi(y),atoi(width), atoi(height));
         w->move(x.toInt(), y.toInt());
         w->resize(width.toInt(), height.toInt());
       }

    //#endif

    w->show();
    a.setMainWidget(w);
    w->show();
    w->setWindowState(w->windowState() & ~Qt::WindowMinimized);
    w->activateWindow();
    w->raise();
    
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );

    int ret = a.exec();
    delete pCMoteurCCAM_Base;
    delete w;
    return ret;
}



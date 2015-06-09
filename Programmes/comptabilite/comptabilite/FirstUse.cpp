 /********************************* FirstUse.cpp **********************************
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 2                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat a l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Federation, 75752 PARIS cedex 15.   *
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
/*! \file FirstUse.cpp
    \brief (put some short descripion here).

     #include "FirstUse.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#include "FirstUse.h"
#include <qobject.h>
#include <qmessagebox.h>
#include <qtextstream.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qdir.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qwidgetstack.h>
#include <qlistbox.h>
#include <qimage.h>


#include <qbuffer.h>

#include <qsqlcursor.h>
#include <qsqlrecord.h>
#include <qsqldatabase.h>

//--------------------------------------------------- FirstUse --------------------------------------------------------------------------
FirstUse::FirstUse(QString params, QString pathIni, bool verboseMode, QObject *parent, const char *name)
 : FormFirstUse(), m_verbose(verboseMode), m_Params(params), m_PathIni(pathIni)
{ if (m_verbose) qWarning("Création de FirstUse");
  m_drtuxLinkedId     = 0;
  m_connectionBase    = FALSE;	// Aucune base de comptabilité configurée
  m_paramUtilisateur  = FALSE;	// Aucun utilisateur configuré
  m_connectionDrTux   = FALSE;	// Impossible de se connecter à la base de DrTux
  m_Utilisateurs_Liste_Tux.clear();
  m_pComptaMoteurBase = 0;
  m_managerIniPath    = "";

  if (m_verbose)  qWarning("FirstUse :: Première utilisation : Saisie des paramètres.");

  // Cherche le fichier ini de Manager
  QString tmp = CGestIni::Construct_Name_File_Ini("Manager", G_pCApp->m_PathAppli);
  if ( QFile::exists( tmp ) )
     { QDir   dtmp = QDir( tmp );
       tmp = dtmp.absPath();
       FormFirstUse::pathToManager->setText(tmp);
       m_managerIniPath = tmp;
       FormFirstUse::dbUsr->setText( G_pCApp->m_tuxdb_User);
       FormFirstUse::dbHost->setText( G_pCApp->m_tuxdb_Host);
       FormFirstUse::dbPort->setText( G_pCApp->m_tuxdb_Port);
     }

  // Connecte les bouttons
  connect(test_DDB,         SIGNAL(clicked()), this, SLOT ( on_test_DDB_clicked()            ) );
  connect(chooseManagerDir, SIGNAL(clicked()), this, SLOT ( on_chooseManagerDir_clicked()    ) );
  connect(testIniTux,       SIGNAL(clicked()), this, SLOT ( on_testIniTux_clicked()          ) );
  connect(but_Valider,      SIGNAL(clicked()), this, SLOT ( on_but_Valider_clicked()         ) );
  connect(but_Quitter,      SIGNAL(clicked()), this, SLOT ( on_but_Quitter_clicked()         ) );
  // Connecte la liste box des utilisateurs MedinTux
  connect(usersList,  SIGNAL( doubleClicked(QListBoxItem*) ),  this,       SLOT  ( on_usersList_doubleClicked(QListBoxItem*) ) );

  // Esthétique de la fenêtre
  FormFirstUse::but_Valider->hide();
  widgetStack1->raiseWidget( 0 );
  QImage img;
  img.loadFromData( icone, sizeof( icone ), "PNG" );
  m_etape1 = TR("Configuration de la base de données de ComptaTux");
  m_etape2 = TR("Liaison avec MedinTux");
  m_etape3 = TR("Configuration de l'utilisateur");

  lbl1->setText("<b>" + m_etape1 + "</b>");
  FormFirstUse::setIcon( img );
  FormFirstUse::exec();
}


//--------------------------------------------------- FirstUse --------------------------------------------------------------------------
FirstUse::~FirstUse()
{ if (m_verbose) qWarning("FirstUse::Destruct");
//  if (m_pFormFirstUse) delete m_pFormFirstUse;
  if (m_pComptaMoteurBase) delete m_pComptaMoteurBase; m_pComptaMoteurBase=0;
  if (m_verbose) qWarning("FirstUse::Destruct Fin");
}


//--------------------------------------------------- on_but_Quitter_clicked --------------------------------------------------------------------------
void FirstUse::on_but_Quitter_clicked()
{ reject();
}

//--------------------------------------------------- on_chooseManagerDir_clicked --------------------------------------------------------------------------
/*! \brief Slot appelé lorsque l'utilisateur clique sur le bouton pour rechercher le chemin vers Manager.
*/
void FirstUse::on_chooseManagerDir_clicked()
{ QString s = QFileDialog::getExistingDirectory(
                    QDir::home().path(),
                    this,
                    TR("Indiquer le chemin vers le fichier ini de Manager"),
                    TR("Indiquer le chemin vers le fichier ini de Manager"),
                    TRUE );
  // Vérifier qu'existe le fichier ini sinon indiquer un message d'erreur
  pathToManager->setText(s);
}

//--------------------------------------------------- on_test_DDB_clicked --------------------------------------------------------------------------
/*! \brief slot appelé lorsque l'utilisateur clique sur "Tester et connecter" après avoir renseigner les champs de connection à la base de données. Tente de se connecter à la base "mysql" du serveur.
 *  Si OK -> getConnection()
*/
void FirstUse::on_test_DDB_clicked()
{ QString erreur = "";
  QString db_Driver = "QMYSQL3";
  QString db_Name = "mysql";   // Juste pour tester les paramètres de connection.
  QString db_User = dbUsr->text();
  QString db_Pass = dbPass->text();
  QString db_Host = dbHost->text();
  QString db_Port = dbPort->text();
  if (db_Host == "") {  db_Host = "localhost"; dbHost->setText(db_Host); }
  if (db_Port == "") {  db_Port = "3306";      dbHost->setText(db_Port); }
  QString db_Label = "comptabilite";
  QString f = "";
  // Teste la connection
  m_pComptaMoteurBase = new ComptaMoteurBase( db_Driver, db_Name, db_User, db_Pass, db_Host, db_Port,
                                              f, db_Label, &erreur);
  if (m_pComptaMoteurBase) { delete m_pComptaMoteurBase; m_pComptaMoteurBase=0; }
  // Récupère le message d'erreur
  if (erreur == "")
   {  test_DDB->setText(TR("Connexion paramétrée"));
      getConnection();
   }
  else
   { QMessageBox::warning(this, TR("Erreur de connexion"), erreur,"OK","","",0,1);
   }
}

//--------------------------------------------------- getConnection --------------------------------------------------------------------------
/*! \brief Les paramètres de connection étant exacts et testés, teste l'existence de la base de données de comptabilité. Si elle n'existe pas alors création. Sinon à faire...
*/
void FirstUse::getConnection()
{ m_db_Usr           = FormFirstUse::dbUsr->text();
  m_db_Pass          = FormFirstUse::dbPass->text();
  m_db_Host          = FormFirstUse::dbHost->text();
  m_db_Port          = FormFirstUse::dbPort->text();
  m_db_Driver        = BASE_COMPTA_DRIVER;
  m_db_Label         = BASE_COMPTA_NOM;
  QString db_Name    = BASE_COMPTA_NOM;
  QString erreur     = "";
  QString          f = "";

  // Test l'ouverture de la base comptabilité
  m_pComptaMoteurBase = new ComptaMoteurBase( m_db_Driver, db_Name, m_db_Usr, m_db_Pass, m_db_Host, m_db_Port,
                                              f, m_db_Label, &erreur);
  if (erreur.length())
  { if (m_verbose)
    { qWarning( TR("FirstUse::getConnection : La base comptabilité n'existe pas.  En cours de création" ));}
      if (m_pComptaMoteurBase) { delete m_pComptaMoteurBase; m_pComptaMoteurBase=0; }

    m_pComptaMoteurBase = new ComptaMoteurBase( m_db_Driver, "mysql", m_db_Usr, m_db_Pass, m_db_Host, m_db_Port,
                                                f, m_db_Label, &erreur);
    QString requete     = QString ("CREATE DATABASE `") + db_Name + "` ;";
    QSqlQuery query(requete, m_pComptaMoteurBase->m_DataBase);
    qDebug(requete);
    // Créer la base de données comptabilité dans laquelle on va insérer les tables
    if (query.exec())
       { if (m_verbose)
         qWarning("FirstUse :: La base de données a été correctement créée.");
	   }
    else
       { if (m_verbose)
         qWarning("FirstUse :: ERREUR : La base de données n'a pas pu être créée. "\
               "Vérifiez vos paramètres MySQL.");
       }
    if (m_pComptaMoteurBase) { delete m_pComptaMoteurBase; m_pComptaMoteurBase=0; }

    // on se reconnecte à la base de données de ComptaTux et on crée la base
    m_pComptaMoteurBase = new ComptaMoteurBase( m_db_Driver, db_Name, m_db_Usr, m_db_Pass, m_db_Host, m_db_Port,
                                                f, m_db_Label, &erreur);
    createBase(m_pComptaMoteurBase->m_DataBase);
    if (m_pComptaMoteurBase) { delete m_pComptaMoteurBase; m_pComptaMoteurBase=0; }
  }
  else
  { // La base existe déjà on ne fait rien.
    // Faudra gérer les mise à jour de la base !!!!
    if (m_verbose)
    { qWarning( TR("La base comptabilité existe déjà." ) ); }
    m_connectionBase = TRUE;
    if (m_pComptaMoteurBase) { delete m_pComptaMoteurBase; m_pComptaMoteurBase=0; }
  }
  lbl1->setText(m_etape1 + " ->  OK");
  lbl2->setText("<b>"+m_etape2+"</b>");
  FormFirstUse::widgetStack1->raiseWidget( 1 );
}

//--------------------------------------------------- createBase --------------------------------------------------------------------------
/*! \brief Crée la base de données de ComptaTux.
*/
void FirstUse::createBase(QSqlDatabase* dBase)
{ QString sqlFile = G_pCApp->m_PathAppli  + "/comptabilite.sql";
  int pos;
  //....................... ouvrir le fichier en mode QTextStream ...........................
  if ( !QFile::exists( sqlFile ) )
   { if (m_verbose)  qWarning("FisrtUse :: Le fichier SQL pour la base de données compatibilité n'existe pas");
     QMessageBox::warning(this, TR("Erreur"),
                          TR("Le fichier de structure de la base de données (%1) n'existe pas.").arg(sqlFile),"Ok","","",0,1);
  }
  else
  { if (m_verbose)  qWarning("FisrtUse :: Le fichier SQL est trouvé.");
    QFile file( sqlFile );
    if ( !file.open( IO_ReadOnly ) )
       { if (m_verbose) qWarning("FirstUse :: Le fichier SQL pour la base de données compatibilité ne peut pas être ouvert.");
         QMessageBox::warning(this, TR("Erreur"),
                              TR("Le fichier de structure de la base de données (%1) n'existe pas.").arg(sqlFile),"Ok","","",0,1);
       }
    else
    { if (m_verbose) qWarning("FisrtUse :: Le fichier SQL est en cours de lecture.");
      QTextStream stream ( &file );
      //.......... on déclare une variable pour lire le fichier ligne à ligne
      QString   buffer("");
      QSqlQuery query(QString::null, dBase);
	  /*
      while (!stream.atEnd())
      {  buffer    += stream.readLine().stripWhiteSpace();
         long len   = buffer.length();
         if ( buffer.at(len-1L)==';' ) // && (buffer.at(len-2L)==')' || (len>7 &&  buffer.mid(len-7) == "MyISAM;" ) ) )
         { if (query.exec(buffer))
		{ if (m_verbose) qWarning("FirstUse :: Instruction SQL lue et exécutée."); }
	   else
		{ if (m_verbose) qWarning("FirstUse :: L'instruction SQL suivante n'a pas pu être exécutée : "+buffer); }
           buffer = "";
         }
	  */
	  while (!stream.atEnd())
      {  buffer    += stream.readLine().stripWhiteSpace();
         long len   = buffer.length();
         if (  (pos = buffer.find("MyISAM")) != -1 )
            {buffer.truncate(pos+6);  buffer += ';';
             buffer.remove ("collate utf8_unicode_ci");
             buffer.remove ("collate utf8_bin");
             buffer.remove ("character set utf8");
             if (query.exec(buffer))
		        { if (m_verbose) qWarning("FirstUse :: Instruction SQL lue et exécutée.");
                }
	         else
		        { QSqlError qerr = query.lastError ();
                  if (m_verbose) qWarning("FirstUse :: L'instruction SQL suivante n'a pas pu être exécutée : "+ buffer + "\r\n " + qerr.text());
                }
             buffer = "";
            }
         else if (buffer.at(len-1L)==';')
            {buffer.remove ("collate utf8_unicode_ci");
             buffer.remove ("collate utf8_bin");
             buffer.remove ("character set utf8");

             if (query.exec(buffer))
		        { if (m_verbose) qWarning("FirstUse :: Instruction SQL lue et exécutée.");
                }
	         else
		        { QSqlError qerr = query.lastError ();
                  if (m_verbose) qWarning("FirstUse :: L'instruction SQL suivante n'a pas pu être exécutée : "+buffer + "\r\n " + qerr.text());
                }
             buffer = "";
           }
    } // Fin while
      m_connectionBase = TRUE;
    } // Fin if file open
   } // Fin if file exists
}


//--------------------------------------------------- on_testIniTux_clicked --------------------------------------------------------------------------
/*!\brief SLOT appelé lorsque l'utilisateur clique sur le boutton "Teste et Valide" du groupe MANAGER_INI de FormFirstUse.
 * Teste l'existence du fichier ini de manager. Se connecte à la base de données de DrTux. Si tout va bien la suite.
*/
void FirstUse::on_testIniTux_clicked()
{ // Récupère le répertoire de Manager signifié par l'utilisateur
  QString pathManager = FormFirstUse::pathToManager->text();
  m_managerIniPath = CGestIni::Construct_Name_File_Ini("Manager", pathManager);
  if ( ! QFile::exists( m_managerIniPath ) )
  {  m_managerIniPath = "../../Manager/bin/Manager.ini";
  }
  if ( QDir(m_managerIniPath).isRelative() )
  {  m_managerIniPath.prepend(G_pCApp->m_PathAppli + "/");
     m_managerIniPath = QDir::cleanDirPath(m_managerIniPath);
     if (m_verbose) qWarning("FirstUse :: m_managerIniPath = " + m_managerIniPath);
  }

  // Récupère les paramètres de connection à la base de données DrTux
  if (QFile::exists( m_managerIniPath ) )
  {  if (m_verbose) { qWarning("FirstUse :: Fichier Ini de Manager trouvé."); }
     QString paramDrTux,f, erreur;
     CGestIni::Param_UpdateFromDisk( m_managerIniPath, paramDrTux);
     CGestIni::Param_ReadParam( paramDrTux, "Connexion", "Parametres",
                                &m_ddbTux_Driver, &m_ddbTux_Base, &m_ddbTux_User,
                                &m_ddbTux_Pass,   &m_ddbTux_Host, &m_ddbTux_Port);

     // Teste la connection à la base MedinTux
     m_pComptaMoteurBase = new ComptaMoteurBase( m_ddbTux_Driver, m_ddbTux_Base , m_ddbTux_User, m_ddbTux_Pass, m_ddbTux_Host,m_ddbTux_Port,
                                                 f, m_ddbTux_Base, &erreur);
     if (erreur == "")
     {   if (m_pComptaMoteurBase != 0)  // C'est bon
          { if (m_verbose)
            { qWarning("FirstUse :: Configuration d'accès à la base de DrTux récupérée et fonctionnelle."); }
            getMedinTux(m_pComptaMoteurBase);
          }
         else
          { if (m_verbose)
            {  qWarning("FirstUse :: Erreur : Configuration d'accès à la base de DrTux non-fonctionnelle."); }
          }
     }
     else
        { if (m_verbose)
          {  qWarning("FirstUse :: Erreur : Configuration d'accès à la base "\
                      "de DrTux non-fonctionnelle. " + erreur);
          }
        }
   } // Fin File ini existe
   else
   { if (m_verbose)
     { qWarning("FirstUse :: Fichier Ini de Manager introuvable. : "+m_managerIniPath); }
   }
   if (m_pComptaMoteurBase) {delete m_pComptaMoteurBase; m_pComptaMoteurBase=0;}
}



//--------------------------------------------------- getMedinTux --------------------------------------------------------------------------
/*! \brief Récupère l'utilisateur lié DrTux.
*/
void FirstUse::getMedinTux(ComptaMoteurBase* pMedinTuxBase)
{ // Sommes connectés à la base de DrTux via pMedinTuxBase
  // Ne pas delete ce pointeur sera delete dans la procédure appelante

  QString f      = "";
  QString erreur = "";
  if (m_verbose)
  {  qWarning("FirstUse :: getMedinTux : Récupération des utilisateurs de MedinTux"); }

   // Récupérer les utilisateurs drtux
   if (!pMedinTuxBase->getDrTuxUsers( &m_Utilisateurs_Liste_Tux )  )
    { if (m_verbose)
      {  qWarning("FirstUse :: getMedinTux : Impossible de récupérer "\
                  "les utilisateurs  enregistrés de MedinTux" + pMedinTuxBase->m_messageErreur);
      }
    }
   else
    { if (m_verbose) qWarning("FirstUse :: Récupération des utilisateurs de DrTux effectuée.");

      // Afficher la liste dans la box prévue à cet effet
      UTILISATEURS_LISTE::iterator it;
      FormFirstUse::usersList->clear();
      for (it = m_Utilisateurs_Liste_Tux.begin() ; it != m_Utilisateurs_Liste_Tux.end(); ++it)
          { FormFirstUse::usersList->insertItem( (*it).getNom() ); }
            // Afficher le Widget Adéquat dans le stack
            lbl2->setText(m_etape2 + " ->  OK");
            lbl3->setText("<b>" + m_etape3+"</b>");
            widgetStack1->raiseWidget( 2 );
          }
   if (m_verbose)  qWarning("FirstUse :: getMedinTux : FormTux créée.");
}


//--------------------------------------------------- on_usersList_doubleClicked --------------------------------------------------------------------------
/*! \brief Slot appelé lorsque l'utilisateur double clique dans la liste des utilisateurs de MedinTux. Appele un widget de demande de login et mot de passe.
 * \todo Améliorable.
*/
void FirstUse::on_usersList_doubleClicked(QListBoxItem* item)
{ if (!item)   return;
  if (item==0) return;
  bool passOK     = FALSE;
  bool nbEssais   = 0;
  bool run        = TRUE;
  QString tmp     = "";
  QString tmpLog  = "";
  QString tmpPass = "";
  Dlg_LoginPass_Medintux*   pPass = 0;
  m_pComptaMoteurBase             = new ComptaMoteurBase();

  UTILISATEURS_LISTE::iterator it = m_Utilisateurs_Liste_Tux.at ( usersList->currentItem() );
  tmpLog                          = (*it).getLogin();
  tmpPass                         = (*it).getPassword();
  // Affiche widget de saisie des log et pass
  do
  { pPass = new Dlg_LoginPass_Medintux(this);
    pPass->login->setText(tmpLog);
    pPass->textLabel1->setText(TR("Saisissez l'identifiant et mot de passe de l'utilisateur MedinTux essai N° : %1").arg(nbEssais));
    pPass->exec();
    if (pPass->result() == QDialog::Accepted)
       { // Teste l'exactitude du log/pass
         tmp = pPass->password->text();
         m_pComptaMoteurBase->PassWordEncode(tmp);
         if ( (pPass->login->text() == tmpLog ) &&
              ( tmp                 == tmpPass ) )
              { passOK = TRUE;
			    run    = FALSE;
			  }
         else
             {
               QMessageBox::warning(this, TR("Erreur"),
                          TR("Les identifiants et mot de passe ne correspondent pas.\n"\
                             "Il vous reste %1 essai(s).").arg(3-nbEssais),"OK","","",0,1);
             }
	   }
	nbEssais++;
	delete pPass;
    if (passOK==FALSE && nbEssais == 1)   run    = FALSE;
  } while (run);
  if (m_pComptaMoteurBase) { delete m_pComptaMoteurBase; m_pComptaMoteurBase=0; }
  if (!passOK) return;
  m_paramUtilisateur = TRUE;
  FormFirstUse::but_Valider->show();
  lbl3->setText(m_etape3 + " ->  OK");
  widgetStack1->raiseWidget( 3 );
}



//--------------------------------------------------- on_but_Valider_clicked --------------------------------------------------------------------------
/*! \brief Valide les informations de l'utilisateur et sauvegarde dans ini et base de données. */
void FirstUse::on_but_Valider_clicked()
{ // Vérifier la complétion des items des formulaires
  if (!m_connectionBase)
   QMessageBox::information(this, TR("Attention"),
                            TR("Le logiciel ne peut actuellement pas se connecter "\
                               "à la base de données.\n\nVous ne pouvez donc pas l'utiliser "\
                               "si vous ne renseignez pas correctement les paramètres de ce "\
                               "formulaire."),"OK","","",0,1);
  else if (!m_paramUtilisateur)
   QMessageBox::information(this, TR("Attention"),
                            TR("Vous n'avez pas configuré d'utilisateur principal.\n\n"\
                               "Dans ces conditions, l'application de comptabilité ne peut pas "\
                               "être utilisée."),"OK","","",0,1);
  else
  // Sauvegarder dans le fichier ini et la base de données de comptabilité
  { // Fichier Ini
    QString erreur, f, tmp = "";
    if (Param_WriteParam(&m_Params,"Configuration","FirstTime","non") == 0)
    { if (m_verbose)
      {    qWarning("Impossible d'écrire dans le fichier ini le paramètre FirstTime.");  }
    }

    if (Param_WriteParam(&m_Params,"Connexion","Parametres", m_db_Driver, BASE_COMPTA_NOM, m_db_Usr, m_db_Pass, m_db_Host, m_db_Port) == 0)
    {  if (m_verbose)
       {    qWarning("Impossible d'écrire dans le fichier ini le paramètre User."); }
    }
    if (Param_WriteParam(&m_Params,"MedinTux","Path",m_managerIniPath) == 0)
    { if (m_verbose)
       {   qWarning("Impossible d'écrire dans le fichier ini le paramètre Path de MedinTux."); }
    }

    Param_UpdateToDisk( m_PathIni , m_Params);

    // Sauvegarde de l'utilisateur dans la base de données
    m_pComptaMoteurBase = new ComptaMoteurBase( m_db_Driver, BASE_COMPTA_NOM, m_db_Usr, m_db_Pass, m_db_Host, m_db_Port,
                                                f, m_db_Label, &erreur);
    UTILISATEURS_LISTE::iterator it = m_Utilisateurs_Liste_Tux.at( m_drtuxLinkedId );
    if (m_pComptaMoteurBase->saveUserToBase( &(*it) ))
      { if (m_verbose)
        {    qWarning("FirstUse :: L'utilisateur est sauvegardé dans la base de données"); }
      }
    else
      { if (m_verbose)
        {    qWarning("FirstUse :: L'utilisateur n'a pas pu être sauvegardé dans la base de données"); }
      }
    accept();
    if (m_verbose)
    {    qWarning("FirstUse::Fin de la procédure.");  }
  }

}


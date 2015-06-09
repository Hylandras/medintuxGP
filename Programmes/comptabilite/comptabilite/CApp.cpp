/************************************** CApp.cpp **********************************
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 2                                   *
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

#include "CApp.h"

CApp* G_pCApp = 0;  /*!< string which contains pointer on global instance of application */

//-------------------------------------------- GotoDebug --------------------------------------------------
int CApp::GotoDebug()
{return 0;
}



//-------------------------------------------- ~CApp -------------------------------------------------------
/**
 * \brief Destructeur de la classe.
 * Efface les pointeurs (en particulier m_pComptaMoteurBase qui se balade dans toutes les autres classes).
 */
CApp::~CApp()
{ if (m_Verbose) qWarning("CApp::Destruct");
  if (m_pComptaMoteurBase)  delete m_pComptaMoteurBase;
  if (m_pActesListe)        delete m_pActesListe;
  if (m_UsersListe)         delete m_UsersListe;
  if (m_Verbose) qWarning("CApp::Destruct Fin");
}


//-------------------------------------------- CApp -------------------------------------------------------
/**
 * \brief Constructeur de la classe.
 * Récupère les paramètres de l'application
 * @param argc
 * @param argv
 */
CApp::CApp(int & argc, char ** argv)
        : QApplication (  argc, argv )
{ // .............. Initialise le pointeur générale de l'application sur cet objet ..............
  G_pCApp             = 0;

  QTextCodec::setCodecForTr( QTextCodec::codecForName("Utf8") );
  QString tmp         ="";
  m_pComptaMoteurBase = 0;
  m_pActesListe       = 0;
  m_UsersListe        = 0;
  m_currentUser       = 0;
  m_Verbose           = FALSE;

  //..............  Récupère les paramètres indispensables pour le plugins (nomexe, ini...) ..............
  m_AppliName   = NOM_APPLICATION;
  m_PathAppli   = CGestIni::Construct_PathBin_Module("comptabilite", QFileInfo(argv[0]).dirPath( TRUE ));
  m_PathIni     = m_PathAppli + "/comptabilite.ini";  // CGestIni::Construct_Name_File_Ini("comptabilite", m_PathAppli);
  m_NbParam     = argc;
 if (!QFile::exists (m_PathIni))
    {   QMessageBox::information(0,TR("Attention"), TR("Attention le fichier <b>")+m_PathIni+TR("</b> n'existe pas"),"Ok","","",1,0);
	    G_pCApp = 0;
	    return;
    }
  //..............  Récupère les paramètres ligne de commande ..............
  for (int i = 0; i < argc; ++i)
      { tmp = argv[i];
        if (tmp == "-v")
           { qWarning("Mode parlant.");  m_Verbose = TRUE;
           }
      }

  //.............. Récupère le fichier paramètre de l'application .................................
  CGestIni::Param_UpdateFromDisk(m_PathIni, m_Param);
  G_pCApp = this;
  getIni();

  G_pCApp = this;
  if (m_FirstTime.lower() == "oui")
     { FirstUse* FU = new FirstUse(m_Param, m_PathIni, m_Verbose);
       if (FU->result() == QDialog::Rejected)
          { QMessageBox::warning(0, TR("Erreur"),
                           TR("Attention, les paramètres de connexion, la liaison à medintux "\
                              "ne sont pas correctement configurés.<br /><br />ComptaTux va fermer."\
                              "<br /><br /><b>Vous devez aller jusqu'au bout de la procédure "\
                              "d'installation avant d'utiliser ComptaTux.</b><br />"),"Ok","","",1,0);
            if (FU) delete FU;
            G_pCApp = 0;
            return;
          }
       if (FU) delete FU;
       // Attention il faut relire les paramètres ini...
       // Récupère le fichier paramètre de l'application
       if (m_Verbose)  qWarning("CApp :: Post-FirstUse :: Récupération des nouveaux paramètres saisis.");
       CGestIni::Param_UpdateFromDisk(m_PathIni, m_Param);
       getIni();
     }

  //..............  Prépare la base de données ..............
  QString confBase, baseLabel = "";

  if (m_Verbose) qWarning("CApp :: Connexion à la base de données.");
  m_pComptaMoteurBase = new ComptaMoteurBase(     m_db_Driver,     // nom du driver
                                                  m_db_Base,    // nom de la base
                                                  m_db_User,    // = "root"       user
                                                  m_db_Pass,    // = ""           password
                                                  m_db_Host,    // = "localhost"  hostname
                                                  m_db_Port,
                                                  confBase,
                                                  baseLabel,
                                                  &m_LastError);        // Msg erreur
   if (m_LastError)
      {m_LastError += TR("\r\nLes données de connexion du fichier de configuration sont invalides : Driver, BaseName,User, Password, Hostname. ");
      }
   if (m_pComptaMoteurBase==0)
      {    m_LastError.prepend(TR("CApp :: Échec de la connexion à la base de données : Impossible de créer la base."));
            if (m_Verbose) qWarning(m_LastError);
            return;
      }
   if (m_pComptaMoteurBase->m_DataBase ==0)
      { //delete m_pComptaMoteurBase;
        //m_pComptaMoteurBase=0;
        m_LastError.prepend(TR("CApp :: Échec de la connexion à la base de données : Impossible de créer la base de données."));
        if (m_Verbose) qWarning(m_LastError);
        if (QMessageBox::information( 0,     TR("Accés aux bases impossible"),
                                             TR ( " <b><u>ATTENTION bien lire le texte ci-dessous</b></u> <br> La base désignée par : <br>"
                                                  "&nbsp;&nbsp;&nbsp;&nbsp;Nom Base <b>")               + BASE_COMPTA_NOM  +
                                             TR(  "<br></b>&nbsp;&nbsp;&nbsp;&nbsp;Driver   \t<b>" )    + BASE_COMPTA_DRIVER  +
                                             TR(  "<br></b>&nbsp;&nbsp;&nbsp;&nbsp;User     \t<b>" )    + m_db_User  +
                                             TR(  "<br></b>&nbsp;&nbsp;&nbsp;&nbsp;Serveur  \t<b>" )    + m_db_Host  +
                                             TR(  "</b><br> n'est pas accessible. <br>"
                                                  "Cela est possible lors de la premiere utilisation de ce programme<br>"
                                                  "&nbsp;&nbsp; <b>uniquement dans ce cas</b> il est nécessaire d'installer les bases.<br>"
                                                  "&nbsp;&nbsp; dans les autres cas installer les bases écrasera toutes les données déjà existantes.") ,
                                                  //"&nbsp;&nbsp;&nbsp;&nbsp;  <b>Quitter pour <u>Annuler</u></b> l'op�ation en cours" ) ,
                                             TR("&Quitter"), TR("Installer les bases"), 0 /*tr("Annuler")*/,
                                             0, 1 ) ==1
            )
            {FirstUse* FU = new FirstUse(m_Param, m_PathIni, m_Verbose);
             if (FU->result() == QDialog::Rejected)
                { QMessageBox::warning(0, TR("Erreur"),
                           TR("Attention, les paramètres de connexion, la liaison à medintux "\
                              "ne sont pas correctement configurés.<br /><br />ComptaTux va fermer."\
                              "<br /><br /><b>Vous devez aller jusqu'au bout de la procédure "\
                              "d'installation avant d'utiliser ComptaTux.</b><br />"),"Ok","","",1,0);
                  if (FU) delete FU;
                  G_pCApp = 0;
                  return;
                }
             if (FU) delete FU;
             // Attention il faut relire les paramètres ini...
             // Récupère le fichier paramètre de l'application
             if (m_Verbose)  qWarning("CApp :: Post-FirstUse :: Récupération des nouveaux paramètres saisis.");
             CGestIni::Param_UpdateFromDisk(m_PathIni, m_Param);
             getIni();
             m_pComptaMoteurBase = new ComptaMoteurBase(  m_db_Driver,      // nom du driver
                                                          m_db_Base,        // nom de la base
                                                          m_db_User,        // = "root"       user
                                                          m_db_Pass,        // = ""           password
                                                          m_db_Host,        // = "localhost"  hostname
                                                          m_db_Port,        // = "localhost"  hostname
                                                          confBase,
                                                          baseLabel,
                                                          &m_LastError);          // Msg erreur
             /*
             m_pComptaMoteurBase = new ComptaMoteurBase(     BASE_COMPTA_DRIVER,     // nom du driver
                                                             BASE_COMPTA_NOM,    // nom de la base
                                                             m_db_User,    // = "root"       user
                                                             m_db_Pass,    // = ""           password
                                                             m_db_Serv,    // = "localhost"  hostname
                                                             confBase,
                                                             baseLabel,
                                                             &m_LastError);        // Msg erreur
              */
             if (m_LastError)
                {m_LastError += TR("\r\nLes données de connexion du fichier de configuration sont invalides : Driver, BaseName,User, Password, Hostname. ");
                }
             if (m_pComptaMoteurBase==0)
                {    m_LastError.prepend(TR("CApp :: Échec de la connexion à la base de données : Impossible de créer la base."));
                    if (m_Verbose) qWarning(m_LastError);
                    G_pCApp = 0;
                    return;
                 }
             if (m_pComptaMoteurBase->m_DataBase ==0)
                {    delete m_pComptaMoteurBase;
                     m_pComptaMoteurBase=0;
                }
            }
        else
            {G_pCApp = 0;
             return;
            }
      }


   //.............. Récupère la liste des utilisateurs configurés pour l'application de comptabilité ..............
   //               ATTENTION Gérer : pas d'utilisateur compta, pas d'utilisateurs DrTux.
   m_UsersListe = new UTILISATEURS_LISTE;
   if (!m_UsersListe) return ;

   if (  !m_pComptaMoteurBase->getUsersForList( m_UsersListe )  )
      {    QMessageBox::information(0, "", m_pComptaMoteurBase->m_messageErreur, "Ok","","",0,1);
        if (m_Verbose) qWarning("CApp :: ERREUR Impossible de créer la liste des utilisateurs");
      }
   else
      { if (m_Verbose) qWarning("CApp :: Liste des utilisateurs de l'application de comptabilité récupérée.");
      }


   //.............. Vérifie que la base de données est à jour ..............
   //                Récupère les données de la base concernant la version actuelle
   QString params = "";
   if ( !m_pComptaMoteurBase->getVersion(params) )
      { qWarning( m_pComptaMoteurBase->m_messageErreur);
      }
   tmp = "";
   QString actualVersion = VERSION_CODE ;
   if ((Param_ReadParam(params, "Version", "num", &tmp)!=0) || ( actualVersion.contains(tmp)==0 ))
      { QMessageBox::information(0, TR("Attention ! Mise à jour requise."),
        TR("Attention, vous ne disposez pas de la bonne version de base de données.<br /><br />"\
           "L'application va tenter de se mettre à jour. Vos données seront préservées.<br /><br />"\
           "En cas d'erreur, suivez les instructions diffusées sur le site internet suivant : "\
           "<br />http://ericmaeker.fr/spip.php?article51"),
        TR("L'application va mettre à jour la base de données."),"","",0,1);
     //int pos;
     //....................... ouvrir le fichier de mise à jour
     if (QFile::exists( m_PathAppli + "/Update.sql" ))
        { QFile file( m_PathAppli + "/Update.sql" );
          if ( !file.open( IO_ReadOnly ) )
             { return ;
             }
          else
             { QTextStream stream ( &file );
               QString   buffer = "";
               m_pComptaMoteurBase->connexion();
               QSqlQuery query(QString::null, m_pComptaMoteurBase->m_DataBase );
               while (!stream.atEnd())
                    {  buffer    += stream.readLine().stripWhiteSpace();
                       long len   = buffer.length();
                       if (buffer.at(len-1L)==';')
                          { if (query.exec(buffer))
                               {  if (m_Verbose) qWarning("CApp :: MAJ Base : Instruction SQL lue et exécutée.");
                               }
                            else
                               {  QSqlError qerr = query.lastError ();
                                  if (m_Verbose) qWarning("FirstUse :: L'instruction SQL suivante n'a pas pu être exécutée : "
                                                           + buffer + "\r\n " + qerr.text());
                                   return;
                               }
                             buffer = "";
                          }
                     } // Fin while
             } // Fin if file open
         } // Fin if file exists
       else
         { return;
         }
     } // fin if wrong database version


   // Ouvre le tout premier dialogue : sélection de l'utilisateur de l'application
   // Ouvrir la fenêtre de sélection de l'utilisateur
   m_currentUser = new Utilisateurs();
   if (!m_currentUser) return ;

   FormSelectionUser *pFUser = new FormSelectionUser();
   if (!pFUser) return ;
   pFUser->setUsersList ( m_UsersListe );
   pFUser->setComptaMoteurBase( m_pComptaMoteurBase );
   if ( pFUser->exec() != QDialog::Accepted )
      {  m_currentUser = 0;
         m_UsrPk = -1;
        if (m_Verbose) qWarning ("CApp:: Aucun utilisateur authentifié. Fin de l'application.");
        return;
      }
   else
      { m_currentUser = pFUser->getUser();
        m_UsrPk       = m_currentUser->getId();
        if (m_Verbose) qWarning( "CApp :: Utilisateur authentifié : " + m_currentUser->getNom() );
      }
   delete pFUser;

   m_pComptaMoteurBase->setParams( m_currentUser );
   m_pComptaMoteurBase->setVerboseMode( m_Verbose );

   // Récupère la liste des actes disponibles
   m_pActesListe = new ACTES_LISTE;
   if (!m_pActesListe) return;

   if (!m_pComptaMoteurBase->getActesForCombobox( *m_pActesListe ) )
      { if (m_Verbose) qWarning("CApp :: Impossible de récupérer la liste des actes disponibles.");
        // Ferme l'appli sinon seg fault à cause de la fonction sythèse de CMDI Honoraires
        return ;
      }
   else
      { if (m_Verbose) qWarning("CApp :: Liste des actes disponibles récupérée.");
      }

   // Initialise le pointeur générale de l'application sur cet objet
   G_pCApp = this;
}


//-------------------------------------------- getIni -------------------------------------------------------
/*! \brief R�up�e les informations du fichier ini.
 * \return FALSE si une erreur s'est produite. Le message d'erreur est g��par cette fonction.
*/
bool CApp::getIni()
{  m_Param    =  "";

  // R�up�e le fichier param�re de l'application de comptabilit�
  CGestIni::Param_UpdateFromDisk(m_PathIni, m_Param);
  // Test premier d�arrage
  QString tmp = "";
  if (CGestIni::Param_ReadParam(m_Param, "Configuration", "FirstTime", &m_FirstTime) != 0)
  {  QMessageBox::information(0,TR("Attention"), TR("Attention : (Configuration - FirstTime) le fichier <b>")+m_PathIni+TR("</b>est erroné"),"Ok","","",1,0);
     return FALSE;
  }

  if (m_FirstTime.lower() != "oui")
  {
  if (CGestIni::Param_ReadParam(  m_Param, "Connexion", "Parametres", &m_db_Driver, &m_db_Base, &m_db_User, &m_db_Pass, &m_db_Host, &m_db_Port) != 0)
  {  QWidget *l = new QWidget();
     QMessageBox::information(l,TR("Attention"), TR("Attention, le fichier <b>")+m_PathIni+TR("</b> est erroné."),"OK","","",1,0);
     delete l;
     return FALSE;
  }

  if (CGestIni::Param_ReadParam(m_Param, "Sauvegarde", "Path", &m_PathSauvegarde) != 0)
  {  QMessageBox::information(0,TR("Attention"), TR("Attention : (Sauvegarde - Path) le fichier <b>")+m_PathIni+TR("</b> est erroné"),"Ok","","",1,0);
     return FALSE;
  }
  else if (m_PathSauvegarde == "") m_PathSauvegarde = QDir::home().path();
  if ( QDir::isRelativePath ( m_PathSauvegarde ) )
       m_PathSauvegarde = QDir::cleanDirPath (m_PathSauvegarde.prepend(G_pCApp->m_PathAppli) );

  //..................... Lire les informations de connexion à la base DrTux ......................................
  QString iniTux = "../../Manager/bin/Manager.ini";
  QString paramTux;
  iniTux  = QDir::cleanDirPath (iniTux.prepend(G_pCApp->m_PathAppli) );
  if (!QFile::exists (iniTux))
     {  QMessageBox::information(0,TR("Attention"), TR("Attention le fichier <b>") + iniTux + TR("</b> n'a pas été trouvé"),"Ok","","",1,0);
	    return FALSE;
     }
  CGestIni::Param_UpdateFromDisk(iniTux, paramTux);
  //    Parametres = QMYSQL3 , DrTuxTest , root ,  , localhost
  if (CGestIni::Param_ReadParam(paramTux, "Connexion", "Parametres", &m_tuxdb_Driver, &m_tuxdb_Name, &m_tuxdb_User, &m_tuxdb_Pass, &m_tuxdb_Host, &m_tuxdb_Port) != 0)
     {  //....................... si echec du truc ecrit en dur dans le compta.ini ........................
        //                        on essaie en supposant que Manager est a sa place
        iniTux = CGestIni::Construct_Name_File_Ini("Manager", m_PathAppli);
        CGestIni::Param_UpdateFromDisk(iniTux, paramTux);

        if (CGestIni::Param_ReadParam(paramTux, "Connexion", "Parametres", &m_tuxdb_Driver, &m_tuxdb_Name, &m_tuxdb_User, &m_tuxdb_Pass, &m_tuxdb_Host, &m_tuxdb_Port) != 0)
           { QMessageBox::information(0,TR("Attention"), TR("Attention, le fichier <b>")+iniTux+TR("</b> est erroné<br />Impossible de trouver les paramètres de connexion à la base de données de DrTux."),"Ok","","",1,0);
             return FALSE;
           }
        else
           {CGestIni::Param_WriteParam(&m_Param, "MedinTux", "Path",iniTux);
            CGestIni::Param_UpdateToDisk(m_PathIni, m_Param);
           }
     }
  } // Fin if firsttime

  return TRUE;
}

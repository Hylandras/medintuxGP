/************************************** CApp_Dus.cpp *****************************
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 2                     *
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

#include "CApp_Dus.h"


CApp_Dus* G_pCApp_Dus = 0;  /*!< string which contains pointer on global instance of application */
//---------------- GotoDebug ---------------------
int CApp_Dus::GotoDebug()
{return 0;
}

//---------------------- CApp_Dus -------------------------------
CApp_Dus::CApp_Dus(int & argc, char ** argv)
        : QApplication (  argc, argv )
{ QTextCodec::setCodecForTr( QTextCodec::codecForName("Utf8") );
  QString tmp ="";
  m_PathExchg	=  "";

  // Récupère les paramètres indispensables pour le plugins (nomexe, ini...)
  m_AppliName	= NOM_APPLICATION;
  m_Param_GUID	= argv[4];
//   m_PathAppli	= QFileInfo(argv[0]).dirPath( TRUE );
//   m_PathIni     = m_PathAppli + QDir::separator() + "comptabilite.ini";
  m_PathAppli   = CGestIni::Construct_PathBin_Module("comptabilite", QFileInfo(argv[0]).dirPath( TRUE ));
  m_PathIni     = CGestIni::Construct_Name_File_Ini("comptabilite", m_PathAppli);
  m_NbParam	= argc;

  // Récupère les infos sur l'appli appelante
  m_AppelName	 = argv[2];
  m_PathAppel	 = QFileInfo (qApp->argv()[2]).dirPath (true);
  m_PathAppelIni = m_PathAppel  + QFileInfo(m_AppelName).baseName() + ".ini";
  if (argc>=3)     m_PathExchg    = argv[3];

  // Récupère les informations du fichier INI de comptabilité
  if (!getIniComptabilite()) return;

  // Récupère les paramètres 13 à la fin
  m_Param_Patient	= argv[13];
  if (CGestIni::IsUtf8(m_Param_Patient))  m_Param_Patient = QString::fromUtf8(m_Param_Patient);

  // Prépare la base de données
  QString confBase, baseLabel = "";

  m_ComptaBase = new ComptaMoteurBase( 	m_db_Driver, 	  // nom du driver
					m_db_Base,	  // nom de la base
					m_db_User,	  // = "root"       user
					m_db_Pass,	  // = ""           password
					m_db_Host,	  // = "localhost"  hostname
					m_db_Port,        // = "localhost"  hostname
                                        confBase,
					baseLabel,
					&m_LastError);		// Msg erreur
   if ((m_LastError) || (m_ComptaBase==0) || (m_ComptaBase->m_DataBase ==0) )
   { QWidget *l= new QWidget();
     QMessageBox::information(l,TR("Attention"), TR("ERREUR : impossible de se connecter à la base de données. Vérifiez les paramètres."),"Ok","","",1,0);
     delete l;
     if (m_ComptaBase->m_DataBase == 0) delete m_ComptaBase;
     return;
   }

  // Récupère la clé primaire de l'utilisateur qui envoie l'honoraire au plug-ins
  tmp = argv[8];
  m_Param_PrimKeyUser = tmp.toInt();
  m_Param_DrTuxUsrPk = m_Param_PrimKeyUser;
  if (!m_ComptaBase->getIdUsrFromDrTuxPk( m_Param_PrimKeyUser ))
   { QWidget *l= new QWidget();
     QMessageBox::information(l,TR("Attention"), TR("Attention, il n'existe aucun utilisateur dans la base de données comptabilité \nlié à l'utilisateur de MedinTux qui vient d'appeler le plugin.\n\nPour pouvoir bénéficier des fonctionnalités de ce plugin,\nvous devez paramétrer un utilisateur.\n\nPour ce faire, démarrez le logiciel de comptabilité fourni avec ce plugin."),"OK","","",1,0);
     delete l;
     return;
   }
  if (m_Param_PrimKeyUser == -1)
   { QWidget *l= new QWidget();
     QMessageBox::information(l,TR("Attention"), TR("Attention, il n'existe aucun utilisateur dans la base de données comptabilité lié à l'utilisateur MedinTux qui vient d'appeler le plugin.\n\nL'opération se poursuit tout de même, mais n'oubliez pas qu'il vous faudra créer un utilisateur dans le logiciel de comptabilité et le lier à l'utilisateur de MedinTux pour pouvoir travailler avec ses honoraires."),"OK","","",1,0);
     delete l;
     return;
   }

  currentUser = new Utilisateurs();
  if (!m_ComptaBase->getUsers( m_Param_PrimKeyUser, *currentUser ))
   { QWidget *l= new QWidget();
     QMessageBox::information(l,TR("Attention"), TR("Attention, il n'existe aucun utilisateur dans la base de données comptabilité lié à l'utilisateur de MedinTux qui vient d'appeler le plugin.\n\nL'opération se poursuit tout de même, mais n'oubliez pas qu'il vous faudra créer un utilisateur dans le logiciel de comptabilité et le lier à l'utilisateur de MedinTux pour pouvoir travailler avec ses honoraires."),"OK","","",1,0);
     delete l;
   }

  // Ici nous avons l'utilisateur et le GUID patient
  // Vérifie qu'il n'existe pas d'honoraire dû pour ce patient à partir de son GUID
  du = 0;
  if (m_ComptaBase->checkDus(currentUser, m_Param_GUID, du) )
  { m_msgToDrTux = "Montant dû = " + QString::number(du);
  }
  else
  { m_msgToDrTux = "";
    QWidget* l = new QWidget();
    QMessageBox::information(l,"Erreur d'accès à la base de données.",m_ComptaBase->m_messageErreur,TR("OK"),"","",1,0);
    delete l;
  }

  // Un peu de cosmétique
  m_Param_GUID		= m_Param_GUID.stripWhiteSpace();
  m_AppelName		= m_AppelName.stripWhiteSpace();
  m_PathAppel		= m_PathAppel.stripWhiteSpace();
  m_PathAppelIni	= m_PathAppelIni.stripWhiteSpace();

  // Tout est OK ==> Initialise le pointeur générale de l'application sur cet objet
  G_pCApp_Dus = this;
}


CApp_Dus::~CApp_Dus()
{ if (m_ComptaBase)   delete m_ComptaBase;
}


/*! \brief Récupère les informations du fichier ini.
 * \return FALSE si une erreur s'est produite. Le message d'erreur est géré par cette fonction.
*/
bool CApp_Dus::getIniComptabilite()
{  m_Param	=  "";

  // Récupère le fichier paramètre de l'application de comptabilité
  CGestIni::Param_UpdateFromDisk(m_PathIni, m_Param);
  // Test premier démarrage
  QString tmp = "";
  if (CGestIni::Param_ReadParam(m_Param, "Configuration", "FirstTime", &tmp) != 0)
  {  QWidget *l = new QWidget();
     QMessageBox::information(l,TR("Attention"), TR("Attention, le fichier <b>")+m_PathIni+TR("</b>est erroné."),"OK","","",1,0);
     delete l;
     return FALSE;
  }

  if (tmp.lower() == "oui" || tmp.lower() == "o")
  {  QWidget *l = new QWidget();
     QMessageBox::information(l,TR("Attention"), TR("Avant d'utiliser le plugin de comptabilité,\nvous devez configurer l'application de comptabilité."),"OK","","",1,0);
     delete l;
     return FALSE;
  }

  if (CGestIni::Param_ReadParam(  m_Param, "Connexion", "Parametres", &m_db_Driver, &m_db_Base, &m_db_User, &m_db_Pass, &m_db_Host, &m_db_Port) != 0)
  {  QWidget *l = new QWidget();
     QMessageBox::information(l,TR("Attention"), TR("Attention, le fichier <b>")+m_PathIni+TR("</b>est erroné."),"OK","","",1,0);
     delete l;
     return FALSE;
  }

  if (CGestIni::Param_ReadParam(m_Param, "MedinTux", "Path", &m_PathManagerIni) != 0)
  {  QWidget *l = new QWidget();
     QMessageBox::information(l,TR("Attention"), TR("Attention, le fichier <b>")+m_PathIni+TR("</b>est erroné."),"OK","","",1,0);
     delete l;
     return FALSE;
  }

  // Les informations de l'ini étant codées il faut les décoder
  return TRUE;
}


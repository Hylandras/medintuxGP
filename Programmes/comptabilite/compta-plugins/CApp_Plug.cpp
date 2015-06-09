/************************************** CApp_Plug.cpp *****************************
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

#include "CApp_Plug.h"
#include <qmessagebox.h>
#include <qobject.h>
#define TR  QObject::tr
CApp_Plug* G_pCApp_Plug = 0;  /*!< string which contains pointer on global instance of application */


//---------------------- CApp_Plug -------------------------------
CApp_Plug::CApp_Plug(int & argc, char ** argv)
        : QApplication (  argc, argv )
{ QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
  m_pUtilisateurs = 0;
  m_ComptaBase    = 0;

}

//---------------------- ~CApp_Plug -------------------------------
CApp_Plug::~CApp_Plug()
{ if (m_ComptaBase)	        delete m_ComptaBase;
  if (m_pUtilisateurs)	    delete m_pUtilisateurs;
}


//---------------- GotoDebug ---------------------
int CApp_Plug::GotoDebug()
{return 0;
}

//---------------- setParam ---------------------
void CApp_Plug::setParam(QStringList &list)
{ QString tmp     = "";
  m_PathExchg	  = "";;
  m_pUtilisateurs = 0;
  // Récupère les paramètres indispensables pour le plugins (nomexe, ini...)
  m_AppliName	= NOM_APPLICATION;
  m_Param_GUID	= list[4];
  m_PathAppli	= CGestIni::Construct_PathBin_Module("comptabilite",  QFileInfo(argv()[0]).dirPath (true));
  if (m_PathAppli.endsWith("/") ||  m_PathAppli.endsWith("\\")) m_PathAppli = m_PathAppli.left(m_PathAppli.length()-1);
  m_PathIni     = m_PathAppli +  "/comptabilite.ini";
  m_NbParam	    = list.count();;

  // Récupère les infos sur l'appli appelante
  m_AppelName	= list[2];
  m_PathAppel	= QFileInfo (list[2]).dirPath (true);
  m_PathAppelIni=    m_PathAppel    + QFileInfo(m_AppelName).baseName() + ".ini";
  if (m_NbParam>=3)  m_PathExchg    = list[3];

  // Récupère les informations du fichier INI de comptabilité
  if (!getIniComptabilite()) return;

  // Récupère les paramètres 13 à la fin
  m_Param_Patient	= list[13];
  if (CGestIni::IsUtf8(m_Param_Patient))   m_Param_Patient = QString::fromUtf8(m_Param_Patient);

  m_Param_Date		= list[14];
  if (CGestIni::IsUtf8(m_Param_Date))      m_Param_Date    = QString::fromUtf8(m_Param_Date);
  m_Param_QDate = setDateTimeNatural_jjMMyyyy_Order(m_Param_Date);
  m_Param_Date  = m_Param_QDate.toString("dd-MM-yyyy");


  m_Param_Acte  = list[15];
  if (CGestIni::IsUtf8(m_Param_Acte))      m_Param_Acte    = QString::fromUtf8(m_Param_Acte);

  QString t = list[16];  // Tiers = oui ou non
  if (t.lower() == "oui" ) 	m_Param_Tiers = TRUE;
  else				        m_Param_Tiers = FALSE;

  m_Param_ModePaiement	= list[17];
  if (CGestIni::IsUtf8(m_Param_ModePaiement))
	m_Param_ModePaiement = QString::fromUtf8(m_Param_ModePaiement);

  // Prépare la base de données
  QString confBase, baseLabel = "";
  m_ComptaBase = new ComptaMoteurBase(  m_db_Driver,      // nom du driver
                                        m_db_Base,        // nom de la base
                                        m_db_User,        // = "root"       user
                                        m_db_Pass,        // = ""           password
                                        m_db_Host,        // = "localhost"  hostname
                                        m_db_Port,        // = "localhost"  hostname
                                        confBase,
                                        baseLabel,
                                        &m_LastError);          // Msg erreur

   if ((m_LastError) || (m_ComptaBase==0) || (m_ComptaBase->m_DataBase ==0) )
   { QMessageBox::information(0,TR("Attention"), TR("ERREUR : impossible de se connecter à la base de données. Vérifiez les paramètres."),"OK","","",1,0);
     if (m_ComptaBase->m_DataBase == 0) delete m_ComptaBase;
     return;
   }

  // Récupère la clé primaire de l'utilisateur qui envoie l'honoraire au plug-ins
  tmp = list[8];
  m_Param_PrimKeyUser = tmp.toInt();
  m_Param_DrTuxUsrPk = m_Param_PrimKeyUser;
  if (!m_ComptaBase->getIdUsrFromDrTuxPk( m_Param_PrimKeyUser ))
   { QMessageBox::information (0,TR("Attention"), TR("Attention, il n'existe aucun utilisateur dans la base de données comptabilité \nlié à l'utilisateur de MedinTux qui vient d'appeler le plug-ins.\n\nPour pouvoir bénéficier des fonctionnalités de ce plugin,\nvous devez paramétrer un utilisateur.\n\nPour ce faire, démarrez le logiciel de comptabilité fourni avec ce plugin."),"OK","","",1,0);
     return;
   }
  if (m_Param_PrimKeyUser == -1)
   { QMessageBox::information(0,TR("Attention"), TR("Attention, il n'existe aucun utilisateur dans la base de données comptabilité lié à l'utilisateur de MedinTux qui vient d'appeler le plugin.\n\nL'opération se poursuit tout de même, mais n'oubliez pas qu'il vous faudra créer un utilisateur dans le logiciel de comptabilité et le lier à l'utilisateur de MedinTux pour pouvoir travailler avec ses honoraires."),"OK","","",1,0);
     return;
   }

  m_pUtilisateurs = new Utilisateurs();
  if (!m_ComptaBase->getUsers( m_Param_PrimKeyUser, *m_pUtilisateurs ))
   { QMessageBox::information(0,TR("Attention"), TR("Attention, il n'existe aucun utilisateur dans la base de données comptabilité lié à l'utilisateur de MedinTux qui vient d'appeler le plugin.\n\nL'opération se poursuit tout de même, mais n'oubliez pas qu'il vous faudra créer un utilisateur dans le logiciel de comptabilité et le lier à l'utilisateur de MedinTux pour pouvoir travailler avec ses honoraires."),"OK","","",1,0);
   }

  // Un peu de cosmétique
  m_Param_Patient	    = m_Param_Patient.stripWhiteSpace();
  m_Param_Praticien	    = m_pUtilisateurs->getNom();
  m_Param_Date		    = m_Param_Date.stripWhiteSpace();
  m_Param_GUID		    = m_Param_GUID.stripWhiteSpace();
  m_Param_Acte		    = m_Param_Acte.stripWhiteSpace();
  m_Param_ModePaiement	= m_Param_ModePaiement.stripWhiteSpace();
  m_Param_ModePaiement	= m_Param_ModePaiement.lower();

  m_AppelName		    = m_AppelName.stripWhiteSpace();
  m_PathAppel		    = m_PathAppel.stripWhiteSpace();
  m_PathAppelIni	    = m_PathAppelIni.stripWhiteSpace();
  // Tout est OK ==> Initialise le pointeur générale de l'application sur cet objet
  G_pCApp_Plug          = this;
}
//---------------------------------------- setDateTimeNatural_jjMMyyyy_Order ---------------------------------------
QDate CApp_Plug::setDateTimeNatural_jjMMyyyy_Order(const QString & dateIn)
{ int i= 0, len = dateIn.length();
  QString str = "";
  for (i= 0; i<len; ++i)
      {if (dateIn[i]>='0' && dateIn[i]<='9') str += dateIn[i];
      }
  QString date  = str.left(8);
  int jour  = date.mid(0,2).toInt();
  int mois  = date.mid(2,2).toInt();
  int annee = date.mid(4,4).toInt();

  QDate dt = QDate ( annee, mois, jour );
  if (dt.isValid () ) return dt;
  else
        {QMessageBox::information (0, TR("date invalide en entrée"), TR("La date spécifiée en entrée est invalide\r\naussi elle sera ajustée à la date courante : %1").arg(m_Param_Date));
         return QDate::currentDate();
        }

}

//---------------------- getIniComptabilite -------------------------------
/*! \brief Récupère les informations du fichier ini.
 * \return FALSE si une erreur s'est produite. Le message d'erreur est géré par cette fonction.
*/
bool CApp_Plug::getIniComptabilite()
{  m_Param	=  "";

  // Récupère le fichier paramètre de l'application de comptabilité
  CGestIni::Param_UpdateFromDisk(m_PathIni, m_Param);
  // Test premier démarrage
  QString tmp = "";
  if (CGestIni::Param_ReadParam(m_Param, "Configuration", "FirstTime", &tmp) != 0)
  {  QMessageBox::information(0,TR("Attention"), TR("Attention, le fichier <b>")+m_PathIni+TR("</b> est erroné."),"Ok","","",1,0);
     return FALSE;
  }

  if (tmp.lower() == "oui" || tmp.lower() == "o")
  {  QMessageBox::information(0,TR("Attention"), TR("Avant d'utiliser le plugin de comptabilité, \nvous devez configurer l'application de comptabilité."),"OK","","",1,0);
     return FALSE;
  }

  if (CGestIni::Param_ReadParam(  m_Param, "Connexion", "Parametres", &m_db_Driver, &m_db_Base, &m_db_User, &m_db_Pass, &m_db_Host, &m_db_Port) != 0)
  {  QWidget *l = new QWidget();
     QMessageBox::information(l,TR("Attention"), TR("Attention, le fichier <b>")+m_PathIni+TR("</b>est erroné."),"OK","","",1,0);
     delete l;
     return FALSE;
  }

  if (CGestIni::Param_ReadParam(m_Param, "MedinTux", "Path", &m_PathManagerIni) != 0)
  {  QMessageBox::information(0,TR("Attention"), TR("Attention, le fichier <b>")+m_PathIni+TR("</b>est erroné."),"OK","","",1,0);
     return FALSE;
  }

  // Les informations de l'ini étant codées il faut les décoder
  return TRUE;
}


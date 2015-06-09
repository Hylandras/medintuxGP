 /********************************* ComptaMoteurBase.cpp **********************************
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
/*! \file ComptaMoteurBase.cpp
    \brief Fonctions de gestion de la base de données.

     #include "ComptaMoteurBase.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#include "ComptaMoteurBase.h"


ComptaMoteurBase::ComptaMoteurBase()
{ m_DriverName  = "";
  m_BaseName    = "";
  m_UserName    = "";
  m_PassWord    = "";
  m_HostName    = "";
  m_DataBase    = 0;
  QSqlDatabase::removeDatabase (BASE_COMPTA_NOM);
}


/** \brief Contructeur de la classe et de la base de comptabilité.
 *
 * @param driver nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
 * @param DataBaseName
 * @param user
 * @param password
 * @param hostname
 * @param confFile
 * @param baseLabel
 * @param errMess
 * Initialise la base de données selon les paramètres fournis. La base est accessible par le biais des variables globales de la classe.
 * \sa m_DataBase, initBase
 */
ComptaMoteurBase::ComptaMoteurBase(
                         const QString& driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                         const QString& DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                         const QString& user,          // = "root"
                         const QString& password,      // = ""
                         const QString& hostname,
                         const QString& port,
                         const char* confFile,
                         const char* baseLabel,
                         QString*    errMess)
{ initBase    (driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
               DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
               user,          // = "root"
               password,      // = ""
               hostname,
               port,
               confFile,
               baseLabel,
               errMess);
 m_pUtilisateurActuel   = 0;
 m_Filtre_Change        = TRUE;
 m_Filtre_Autre         = "";
 m_Filtre_Type_Paiement = "";
 m_Filtre_Date_Debut    = QDate::currentDate();
 m_Filtre_Date_Fin      = QDate::currentDate();

}

/** \brief Destructeur de la classe et de la base de comptabilité
 *
 * Ferme la base de donnée de comptabilité.
 * \sa m_DataBase, initBase
 */
ComptaMoteurBase::~ComptaMoteurBase()
{//if (G_pCApp->m_Verbose) qWarning("ComptaMoteurBase::Destruct");
   if (m_DataBase != 0)
    {if (m_DataBase->isOpen()) m_DataBase->close();
     QSqlDatabase::removeDatabase (m_BaseLabel);
     //delete m_DataBase;
     //m_DataBase = 0;
    }
// if (G_pCApp->m_Verbose) qWarning("ComptaMoteurBase::Destruct Fin");
}

void ComptaMoteurBase::setVerboseMode(bool to)
{ m_Verbose = to;
}

//-----------------------------------------------------  initBase -------------------------------------------
/** \brief initialise la base de données de la comptabilité
 *
 * @param driver
 * @param DataBaseName
 * @param user
 * @param password
 * @param hostname
 * @param confFile
 * @param baseLabel
 * @param errMess
 */
void ComptaMoteurBase::initBase   (const QString& driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                   const QString& DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                   const QString& user,          // = "root"
                                   const QString& password,      // = ""
                                   const QString& hostname,
                                   const QString& port,
                                   const char* /*confFile*/,
                                   const char* baseLabel,
                                   QString*    errMess)
{ //int ret;
  m_DriverName  = driver;
  m_BaseName    = DataBaseName;
  m_UserName    = user;
  m_PassWord    = password;
  m_HostName    = hostname;
  m_Port        = port;
  m_DataBase    = 0;
  m_ConvertRun  = FALSE;

  m_DataBase    = BaseConnect(driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                              DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                              user,          // = "root"
                              password,      // = ""
                              hostname,      // = "localhost"
                              port,
                              errMess,
                              baseLabel);
}


/** \brief Se connecte à la base de données de la comptabilité
 *
 * @param driver
 * @param DataBaseName
 * @param user
 * @param password
 * @param hostname
 * @param confFile
 * @param baseLabel
 * @param errMess
 */
//-----------------------------------------------------  BaseConnect -------------------------------------------
QSqlDatabase*
ComptaMoteurBase::BaseConnect(              const QString& driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                            const QString& DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                            const QString& user,          // = "root"
                                            const QString& pasword,       // = ""
                                            const QString& hostname,      // = "localhost"
                                            const QString& port,          // = "3306"
                                            QString*    errMess,
                                            QString     baseLabel)
    {
        m_BaseLabel             = baseLabel;
        QSqlDatabase *defaultDB = QSqlDatabase::addDatabase( driver , baseLabel);
        if (defaultDB<=0)
	       { QMessageBox *b = new QMessageBox();
	         b->setCaption(TR("ERREUR DU PLUGIN"));
	         b->setText(TR("Impossible de se connecter à la base de données.<br>Vérifiez que vous avez installé le gestionnaire de base de données MySQL pour Qt."));
	         b->setIcon(QMessageBox::Critical);
	         b->exec();
	         return 0;
	       }
        defaultDB->setDatabaseName( DataBaseName );
        defaultDB->setUserName( user );
        defaultDB->setPassword( pasword );
        defaultDB->setHostName( hostname);
        defaultDB->setPort( port.toInt());
        if ( ! defaultDB->open() )
           {QString qstr = "";
            qstr += TR("Impossible d'ouvrir la base de données : ") + QString(driver) + "  " + QString(DataBaseName) + "<br>" + defaultDB->lastError().driverText() + "<br>" + defaultDB->lastError().databaseText() + "<br><br>" + TR("Référez-vous à la documentation du plugin");
            if (errMess) *errMess = qstr;
            return 0;
           }
        return defaultDB;
    }



/*! \brief Teste la connection à la base de données. Renvoie FALSE si erreur. Le message d'erreur est stocké dans m-messageErreur.
*/
bool ComptaMoteurBase::connexion()
{  if (m_DataBase==0)
     {	m_messageErreur = TR("Impossible de se connecter à la base de données. Soit celle-ci n'existe pas, soit les paramètres de connexion sont erronés.");
	return FALSE;
     }
  if (m_DataBase->isOpen()==FALSE && m_DataBase->open()==FALSE)
     {	m_messageErreur = TR("Impossible d'ouvrir la base de données.");
	return FALSE;
     }
  return TRUE;
}


void ComptaMoteurBase::setParams( Utilisateurs* pUtilisateur)
{ m_pUtilisateurActuel = pUtilisateur;
  m_UsrPk = m_pUtilisateurActuel->getId();
}

/*! \brief retourne les valeurs par défaut des actes disponibles pour la comptabilité (gestion des erreurs de base
  * Chaque élément de la liste contient les variables de l'acte séparés par \em /;/ .La Liste est construite de la sorte : Nom Acte /;/ Descriptif de l'acte /;/ Montant total de l'acte /;/ Montant du tiers payant de l'acte
*/
QStringList getActesDisponiblesDefauts()
{ QStringList ret;
  ret.clear();
  ret.append("C /;/ Consultation omnipraticien /;/ 22 /;/ 6.6");
  ret.append("C+MNO /;/ Consultation du nourrisson /;/ 27 /;/ 11");
  return ret;
}


/*! \brief Récupère la liste des actes disponibles dans la base pour construire le contenu du combobox de la fenêtre de plugins (par exemple).
 * \param al : Liste des actes disponibles dans la base de données
 * \return TRUE : pas d'erreur, FALSE = erreur. Le message d'erreur est stocké dans m_messageErreur.
*/
bool ComptaMoteurBase::getActesForCombobox(ACTES_LISTE& al)
{ al.clear();
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  //............. by Roland pour UTF8 .......................
  QString requete("SELECT ");
  requete       += "nom_acte, desc_acte, type, montant_total, montant_tiers, id_acte_dispo FROM actes_disponibles ORDER BY id_acte_dispo";
  QSqlQuery query(requete , m_DataBase );
  if (query.isActive())
     {while (query.next())
	      {Actes *acte = new Actes();
	       if (acte)
	          { acte->setNom	 	(Utf8_Query(query, 0 ) ) ;
                acte->setDesc 	    (Utf8_Query(query, 1 ) ) ;
	            acte->setType 	    (Utf8_Query(query, 2 ) ) ;
	            acte->setTotal 	    (query.value( 3 ).toDouble() ) ;
	            acte->setTiers	    (query.value( 4 ).toDouble() ) ;
	            acte->setId 		(query.value( 5 ).toInt() ) ;
	            al.append( *acte );
	            delete acte;
	          }
		  }
     } //endif (pSqlQuery && pSqlQuery->isActive())
 else
     {m_DataBase->close();
      m_messageErreur = TR("ComptaMoteurBase::getActesForCombobox() : Impossible de récupérer les actes :");
	  m_messageErreur += "\n\n" + query.lastError().driverText() + "\n\n" + query.lastError().databaseText() + ".";
	  return FALSE;
	 }
/*
  //............. by Eric no UTF8 .......................
    // Prépare le Curseur
  QSqlCursor cur("actes_disponibles", TRUE, m_DataBase);
  // Trie et Filtre
  cur.setSort( cur.index( "id_acte_dispo" ) );

  if ( !cur.select() )
   {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::getActesForCombobox() : Impossible de récupérer les actes :");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }
   else
   { while ( cur.next() )
      {	// Construit l'honoraire pour chaque résultat de la requête
	Actes *acte = new Actes();
	if (acte)
	{ acte->setNom	 	(cur.value( "nom_acte" ).toString() ) ;
	  acte->setDesc 	(cur.value( "desc_acte" ).toString() ) ;
	  acte->setType 	(cur.value( "type" ).toString() ) ;
	  acte->setTotal 	(cur.value( "montant_total" ).toDouble() ) ;
	  acte->setTiers	(cur.value( "montant_tiers" ).toDouble() ) ;
	  acte->setId 		(cur.value( "id_acte_dispo" ).toInt() ) ;
	  al.append( *acte );
	  delete acte;
	}
      } // Fin while (requête finie)
    }
   */
  // Fermer la base
  m_DataBase->close();
  return TRUE;
}


/*! \brief Fonction rudimentaire, appelée après ouverture de la base de données, création d'un curseur sur la table honoraire pour récupérer l'honoraire sur lequel pointe le curseur courant.
 * \return La fonction renvoie un booléen : TRUE si aucun problème à l'exécution de la fonction, FALSE cas contraire.
 * \param SQL_Curseur : Curseur SQL de l'honoraire à récupérer.
 * \param hono : honoraire tel que récupéré.
 * Fonction crée pour améliorer la maintenance du logiciel et de la base. Toutes les fonctions de lecture des honoraires font appel à cette fonction.
*/
void ComptaMoteurBase::readHonoraire(QSqlCursor & SQL_Curseur, Honoraires* hono)
{ QDate dt;
  hono->setId	 	(SQL_Curseur.value( "id_hono" ).toInt() ) ;
  hono->setIdUsr	(SQL_Curseur.value( "id_usr" ).toInt() ) ;
  hono->setDrTuxUsr	(SQL_Curseur.value( "id_drtux_usr" ).toInt() ) ;
  hono->setPatient 	(SQL_Curseur.value( "patient" ).toString() ) ;
  hono->setPraticien 	(SQL_Curseur.value( "praticien" ).toString() ) ;
  dt = QDate::fromString(SQL_Curseur.value( "date" ).toString(), Qt::ISODate);
  hono->setDate 	( dt ) ;
  hono->setActes 	(SQL_Curseur.value( "acte" ).toString() ) ;
  hono->setGUID 	(SQL_Curseur.value( "GUID" ).toString() ) ;
  hono->setRemarque 	(SQL_Curseur.value( "remarque" ).toString() ) ;
  hono->setEspeces 	(SQL_Curseur.value( "esp" ).toDouble() ) ;
  hono->setCheque 	(SQL_Curseur.value( "chq" ).toDouble() ) ;
  hono->setCB 		(SQL_Curseur.value( "cb" ).toDouble() ) ;
  hono->setDAF 		(SQL_Curseur.value( "daf" ).toDouble() ) ;
  hono->setAutre 	(SQL_Curseur.value( "autre" ).toDouble() ) ;
  hono->setCMU 		(SQL_Curseur.value( "CMU" ).toDouble() ) ;
  hono->setVirement	(SQL_Curseur.value( "vir" ).toDouble() ) ;
  hono->setValidite 	(SQL_Curseur.value( "valide" ).toInt() ) ;
  hono->setTracabilite 	(SQL_Curseur.value( "tracabilite" ).toString() ) ;
}


/*! \brief Fonction rudimentaire, appelée après ouverture de la base de données, création d'un curseur sur la table mouvements pour récupérer le mouvement sur lequel pointe le curseur courant.
 * \return La fonction renvoie un booléen : TRUE si aucun problème à l'exécution de la fonction, FALSE cas contraire.
 * \param SQL_Curseur : Curseur SQL du mouvement à récupérer.
 * \param hono : mouvement tel que récupéré.
 * Fonction crée pour améliorer la maintenance du logiciel et de la base. Toutes les fonctions de lecture des mouvements font appel à cette fonction.
*/
void ComptaMoteurBase::readMouvement(QSqlCursor & SQL_Curseur, Mouvements* mvt)
{ mvt->setId 		(SQL_Curseur.value( "id_mouvement" ).toInt() ) ;
  mvt->setIdParent 	(SQL_Curseur.value( "id_mvt_dispo" ).toInt() ) ;
  mvt->setIdUsr		(SQL_Curseur.value( "id_usr" ).toInt() );
  mvt->setIdCompte	(SQL_Curseur.value( "id_compte" ).toInt() );
  mvt->setType 		(SQL_Curseur.value( "type" ).toInt() ) ;
  mvt->setNom	 	(SQL_Curseur.value( "libelle" ).toString() ) ;
  mvt->setDate		(SQL_Curseur.value( "date").toString() );
  mvt->setDateValeur	(SQL_Curseur.value( "date_valeur").toString() );
  mvt->setTotal		(SQL_Curseur.value( "montant").toDouble() );
  mvt->setRemarque	(SQL_Curseur.value( "remarque" ).toString() ) ;
  mvt->setValidite	(SQL_Curseur.value( "valide" ).toInt() );
  mvt->setTracabilite	(SQL_Curseur.value( "tracabilite" ).toString() ) ;
}


/*! \brief Fonction rudimentaire, appelée après ouverture de la base de données, création d'un curseur sur la table immobilisations pour récupérer l'immobilisation sur lequel pointe le curseur courant.
 * \return La fonction renvoie un booléen : TRUE si aucun problème à l'exécution de la fonction, FALSE cas contraire.
 * \param SQL_Curseur : Curseur SQL du mouvement à récupérer.
 * \param immo : mouvement tel que récupéré.
 * Fonction crée pour améliorer la maintenance du logiciel et de la base. Toutes les fonctions de lecture des immobilisations font appel à cette fonction.
*/
void ComptaMoteurBase::readImmobilisation(QSqlCursor & SQL_Curseur, Immobilisations* immo)
{ immo->setId	 	(SQL_Curseur.value( "id_immob" ).toInt() ) ;
  immo->setIdUsr 	(SQL_Curseur.value( "id_usr" ).toInt() ) ;
  immo->setNom		(SQL_Curseur.value( "libelle" ).toString() ) ;
  immo->setDateService 	(SQL_Curseur.value( "date_service" ).toString() ) ;
  immo->setDuree 	(SQL_Curseur.value( "duree" ).toInt() ) ;
  immo->setMode 	(SQL_Curseur.value( "mode" ).toInt() ) ;
  immo->setValeur 	(SQL_Curseur.value( "valeur" ).toInt() ) ;
  immo->setResiduel 	(SQL_Curseur.value( "valeur_residuelle" ).toInt() ) ;
  immo->setResultat 	(SQL_Curseur.value( "resultat" ).toString() ) ;
  immo->setRemarque 	(SQL_Curseur.value( "remarque" ).toString() ) ;
  // Récupère les mouvements associés
  getMvts( immo->m_Mvts_Liste , immo->getId() );
  immo->setValidite	(SQL_Curseur.value( "valide" ).toInt() );
  immo->setTracabilite	(SQL_Curseur.value( "tracabilite" ).toString() ) ;
}

/*! \brief Récupère en un passage les honoraires pour la période définie par les paramètres en vue de la création d'une remise de chèques.
 * \return La fonction renvoie un booléen : TRUE si aucun problème à l'exécution de la fonction, FALSE cas contraire.
 * \param *hl : Pointeur vers la liste d'honoraires demandée ( HONO_LISTE ).
*/
bool ComptaMoteurBase::getHonorairesForDepot(HONO_LISTE* hl)
{ hl->clear();

  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Prépare le Curseur
  QSqlCursor cur("honoraires", TRUE, m_DataBase);
  // Trie et Filtre
  cur.setSort( cur.index( "date" ) );
  cur.setFilter( getFiltre("", DATES_ONLY) + " AND chq>0 AND remarque LIKE \'%"+CHEQUE_PAS_DEPOSE+"%\'" );

  if ( !cur.select() )
   {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::getHonorairesForDepot() : Impossible de récupérer les honoraires :");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }
   else
   { while ( cur.next() )
      {// Construit l'honoraire pour chaque résultat de la requête
	Honoraires *hono = new Honoraires();
	if (hono)
	{ readHonoraire(cur, hono);
	  hl->append( *hono );
	  delete hono;
	}
      } // Fin while (requête finie)
    }

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}


/*! \brief Valide la liste des honoraires passés en argument à la fonction.
  * \param pHono_Liste = Liste des honoraires à valider dans la base de données
  * \param pUtilisateurActuel = Utilisateur en cours
  * La fonction vérifie d'abord que les honoraires ne sont pas déjà validés. Ensuite valide les honoraires en mettant la valeur 1 dans le champs 'valide' des honoraires et HONO_VALIDE suivi de la date de validation au format yyyyMMdd suivi du nom de l'utilisateur précédé d'une ';'. Ce qui donne par exemple :
  * \code |OK=20080131;Dr MAEKER Eric
 * \return La fonction renvoie un booléen : TRUE si aucun problème à l'exécution de la fonction, FALSE cas contraire.
*/
bool ComptaMoteurBase::valideHonoraires(HONO_LISTE* pHono_Liste, Utilisateurs* pUtilisateurActuel, int & nbUpdated)
{ QString nom = pUtilisateurActuel->getNom();
  QString remarque = "";
  nbUpdated = 0;
  HONO_LISTE::iterator it;
  for (it = pHono_Liste->begin(); it != pHono_Liste->end(); ++it)
  { if ((*it).getValidite() == 0) // honoraire non validé
   { (*it).setValidite( 1 );
     remarque = (*it).getRemarque() + HONO_VALIDE + QDate::currentDate().toString("yyyyMMdd") + nom;
     (*it).setRemarque( remarque );
     if (updateHonoraires( &(*it) )) nbUpdated++;
   }
  }
  return TRUE;
}


/*! \brief Valide la liste des mouvements passés en argument à la fonction.
  * \param pMvts_Liste = Liste des mouvements à valider dans la base de données
  * \param pUtilisateurActuel = Utilisateur en cours
  * \param idCpt = Compte bancaire à valider
  * La fonction vérifie d'abord que les mouvements ne sont pas déjà validés. Ensuite les valide en mettant la valeur 1 dans le champs 'valide'  et HONO_VALIDE suivi de la date de validation au format yyyyMMdd suivi du nom de l'utilisateur précédé d'une ';'. Ce qui donne par exemple :
  * \code |OK=20080131;Dr MAEKER Eric
 * \return La fonction renvoie un booléen : TRUE si aucun problème à l'exécution de la fonction, FALSE cas contraire.
*/
bool ComptaMoteurBase::valideMouvements(MVTS_LISTE* pMvts_Liste, Utilisateurs* pUtilisateurActuel, int idCpt, int & nbUpdated)
{ QString nom = pUtilisateurActuel->getNom();
  QString remarque = "";
  nbUpdated = 0;
  MVTS_LISTE::iterator it;
  for (it = pMvts_Liste->begin(); it != pMvts_Liste->end(); ++it)
  { if (((*it).getValidite() == 0) && ((*it).getIdImmob() == 0)) // honoraire non validé
      if ((*it).getIdCompte() == idCpt)
      { (*it).setValidite( 1 );
       remarque = (*it).getRemarque() + HONO_VALIDE + QDate::currentDate().toString("yyyyMMdd") + nom;
       (*it).setRemarque( remarque );
       if (updateMvt( &(*it) )) nbUpdated++;
      }
// qWarning("valide = "+QString::number((*it).getValidite()));
// qWarning("IdImmob = "+QString::number((*it).getIdImmob()));
// qWarning("Id Cpt = "+QString::number((*it).getIdCompte()));
  }
  return TRUE;
}


/*! \brief Encaisse la liste des honoraires en DAF passés en argument à la fonction.
  * \param pHono_Liste = Liste des honoraires à valider dans la base de données
  * \param pUtilisateurActuel = Utilisateur en cours
  * La fonction vérifie d'abord que les honoraires ne sont pas déjà validés. Ensuite valide les honoraires en mettant la valeur 1 dans le champs 'valide' des honoraires et HONO_VALIDE suivi de la date de validation au format yyyyMMdd suivi du nom de l'utilisateur précédé d'une ';'. Ce qui donne par exemple :
  * \code |OK=20080131;Dr MAEKER Eric
 * \return La fonction renvoie un booléen : TRUE si aucun problème à l'exécution de la fonction, FALSE cas contraire.
*/
bool ComptaMoteurBase::encaisseDAF(HONO_LISTE* pHono_Liste, Utilisateurs* pUtilisateurActuel, int & nbUpdated)
{ QString nom = pUtilisateurActuel->getNom();
  QString remarque = "";
  nbUpdated = 0;
  HONO_LISTE::iterator it;
  for (it = pHono_Liste->begin(); it != pHono_Liste->end(); ++it)
  { remarque = (*it).getRemarque();
    if (remarque.find(DAF_PAS_OK) != -1)		// Vérifie que l'honoraire et un DAF non encaissé
    { remarque.replace(DAF_PAS_OK, DAF_OK);		// Encaisse le DAF
      (*it).setRemarque( remarque );
      if (updateHonoraires( &(*it) )) nbUpdated++;	// Mise à jour de la base de données
    } // Fin if contient DAF_PAS_OK
   } // Fin For
 return TRUE;
}

bool ComptaMoteurBase::encaisseCMU(HONO_LISTE* pHono_Liste, Utilisateurs* pUtilisateurActuel, int & nbUpdated)
{ QString nom = pUtilisateurActuel->getNom();
  QString remarque = "";
  nbUpdated = 0;
  HONO_LISTE::iterator it;
  for (it = pHono_Liste->begin(); it != pHono_Liste->end(); ++it)
  { remarque = (*it).getRemarque();
    if (remarque.find(CMU_PAS_OK) != -1)		// Vérifie que l'honoraire et un DAF non encaissé
    { remarque.replace(CMU_PAS_OK, CMU_OK);		// Encaisse le cmu
      (*it).setRemarque( remarque );
      if (updateHonoraires( &(*it) )) nbUpdated++;	// Mise à jour de la base de données
    } // Fin if contient CMU_PAS_OK
   } // Fin For
 return TRUE;
}
/*! \brief Récupère en un passage les honoraires pour la période définie par le filtre.
  * \param table = table de la base à filtrer (honoraires, depots...)
  * ATTENTION, les dates du filtre doivent être \em préalablement remplies.
 * \return La fonction renvoie un booléen : TRUE si aucun problème à l'exécution de la fonction, FALSE cas contraire.
 * \param *hl : Pointeur vers la liste d'honoraires demandée ( HONO_LISTE ).
*/
bool ComptaMoteurBase::getHonoraires_DAF_ForGestion(HONO_LISTE* hl)
{ // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;
  hl->clear();

  // Prépare le Curseur
  QSqlCursor cur("honoraires", TRUE, m_DataBase);
  // Trie et Filtre
  cur.setSort( cur.index( "date" ) );
  QString filtre = getFiltre("",DATES_ONLY) + " AND daf>0 AND remarque LIKE '%";
  filtre += DAF_PAS_OK;
  filtre += "%'";
  cur.setFilter( filtre );

  if ( !cur.select() )
   {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::getHonoraires_DAF_ForGestion() : Impossible de récupérer les honoraires : select \n");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }
   else
   { QDate dt;
     while ( cur.next() )
      {	dt = QDate::fromString(cur.value( "date" ).toString(), Qt::ISODate);
	// Construit l'honoraire pour chaque résultat de la requête
	Honoraires *hono = new Honoraires();
	if (hono)
	{ readHonoraire(cur, hono);
	  hl->append( *hono );
	  delete hono;
	}
      } // Fin while (requête finie)
    }

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}

bool ComptaMoteurBase::getHonoraires_CMU_ForGestion(HONO_LISTE* hl)
{ // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;
  hl->clear();

  // Prépare le Curseur
  QSqlCursor cur("honoraires", TRUE, m_DataBase);
  // Trie et Filtre
  cur.setSort( cur.index( "date" ) );
  QString filtre = getFiltre("",DATES_ONLY) + " AND cmu>0 AND remarque LIKE '%";
  filtre += CMU_PAS_OK;
  filtre += "%'";
  cur.setFilter( filtre );

  if ( !cur.select() )
   {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::getHonoraires_CMU_ForGestion() : Impossible de récupérer les honoraires : select \n");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }
   else
   { QDate dt;
     while ( cur.next() )
      {	dt = QDate::fromString(cur.value( "date" ).toString(), Qt::ISODate);
	// Construit l'honoraire pour chaque résultat de la requête
	Honoraires *hono = new Honoraires();
	if (hono)
	{ readHonoraire(cur, hono);
	  hl->append( *hono );
	  delete hono;
	}
      } // Fin while (requête finie)
    }

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}

/*! \brief Récupère en un passage les honoraires pour la période définie par le filtre.
  * \param table = table de la base à filtrer (honoraires, depots...)
  * ATTENTION, les dates du filtre doivent être \em préalablement remplies.
 * \return La fonction renvoie un booléen : TRUE si aucun problème à l'exécution de la fonction, FALSE cas contraire.
 * \param *hl : Pointeur vers la liste d'honoraires demandée ( HONO_LISTE ).
*/
bool ComptaMoteurBase::getHonorairesForTable(HONO_LISTE* hl)
{ // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Prépare le Curseur
  QSqlCursor cur("honoraires", TRUE, m_DataBase);
  // Trie et Filtre
  cur.setSort( cur.index( "date" ) );
  cur.setFilter( getFiltre("") );

  if ( !cur.select() )
   {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::getHonorairesForTable() : Impossible de récupérer les honoraires :");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }
   else
   { QDate dt;
     while ( cur.next() )
      {	dt = QDate::fromString(cur.value( "date" ).toString(), Qt::ISODate);
	// Construit l'honoraire pour chaque résultat de la requête
	Honoraires *hono = new Honoraires();
	if (hono)
	{ readHonoraire(cur, hono);
	  hl->append( *hono );
	  delete hono;
	}
      } // Fin while (requête finie)
    }

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}


/*! \brief Récupère en un le total des honoraires de la période filtrée.
  * \param total = montant total de la période. Ne prend pas en compte les montants dus.
  * \param tousHonoValide = TRUE si tous les honoraires sont validés, FALSE cas contraire.
 * \return La fonction renvoie un booléen : TRUE si aucun problème à l'exécution de la fonction, FALSE cas contraire.
 *  \todo Faut-il prendre en compte les DAF selon validation, chèques selon dépôts ?
*/
bool ComptaMoteurBase::getHonoraires_MontantTotal(double& total, double& esp,
						  bool& tousHonoValide, bool pourBilanAnnuel, int annee)
{ total = 0;
  esp = 0;
  tousHonoValide = TRUE;
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Prépare le Curseur
  QSqlCursor cur("honoraires", TRUE, m_DataBase);
  // Trie et Filtre
  cur.setSort( cur.index( "date" ) );
  QString filtre = "";
  if (pourBilanAnnuel)
    filtre = "date LIKE \""+QString::number(annee)+"%\" AND id_usr="+QString::number(m_UsrPk);
  else  filtre = getFiltre("");
  cur.setFilter( filtre );

  if ( !cur.select() )
   {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::getHonoraires_MontantTotal() : Impossible de récupérer les honoraires :");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }
   else
   { QDate dt;
     while ( cur.next() )
      {	  total += (cur.value( "esp" ).toDouble() ) ;
	  esp   += (cur.value( "esp" ).toDouble() ) ;
	  total += (cur.value( "chq" ).toDouble() ) ;
	  total += (cur.value( "cb" ).toDouble() ) ;
	  total += (cur.value( "daf" ).toDouble() ) ;
	  total += (cur.value( "autre" ).toDouble() ) ;
	  total += (cur.value( "cmu" ).toDouble() ) ;
	  total += (cur.value( "vir" ).toDouble() ) ;
	  if (cur.value("valide").toInt() == 0) tousHonoValide = FALSE;
      } // Fin while (requête finie)
    }

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}


/*! \brief Récupère en une passe le total des dépôts de la période filtrée.
  * \param total = montant total de la période. Ne prend pas en compte les montants dus.
 * \return La fonction renvoie un booléen : TRUE si aucun problème à l'exécution de la fonction, FALSE cas contraire.
 *  \todo Faut-il prendre en compte les DAF selon validation, chèques selon dépôts ?
*/
bool ComptaMoteurBase::getDepots_MontantTotal(double& total, double& esp, bool pourBilanAnnuel, int annee)
{ total = 0;
  esp = 0;
  QString tmp, val, filtre;
  tmp = "";
  val = "";
  filtre = "";

  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Prépare le Curseur et le Filtre
  QSqlCursor cur("depots", TRUE, m_DataBase);
  if (pourBilanAnnuel)
    filtre = "date LIKE \""+QString::number(annee)+"%\" AND id_usr="+QString::number(m_UsrPk);
  else  filtre = getFiltre("");

  if ( !cur.select( filtre ) )
   { m_DataBase->close();
     m_messageErreur = TR("ComptaMoteurBase::getDepots_MontantTotal : Impossible de récupérer le dépôt depuis la base de données : select \n\n");
     m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
     return FALSE;
   }
  else
   while ( cur.next() )
    {	tmp = cur.value( "remarque").toString();
	getIntoRemarque(tmp, TOTAL_DEPOT, val);
	total += val.toDouble();
	if (tmp.find(DEPOT_ESPECES) != -1 )
	   esp += val.toDouble();
    } // Fin while

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}


//.------------------------------------ getMvts_MontantTotal ------------------------------------------------------
/*! \brief Récupère en un le total des honoraires de la période filtrée.
  * \param recettes
  * \param depenses
  * \param esp : retourne le montant total des espèces au sein des mouvements filtrés
  * \param pourBilanAnnuel : TRUE fait le bilan de l'année en cours sur le filtre, FALSE utilise le filtre
 * \return La fonction renvoie un booléen : TRUE si aucun problème à l'exécution de la fonction, FALSE cas contraire.
 *  \todo Faut-il prendre en compte les DAF selon validation, chèques selon dépôts ?
*/
bool ComptaMoteurBase::getMvts_MontantTotal(double& recettes, double& depenses, double& esp, bool pourBilanAnnuel, int annee)
{ recettes = 0;
  depenses = 0;
  esp = 0;
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Prépare le Curseur
  QSqlCursor cur("mouvements", TRUE, m_DataBase);

  // Trie et Filtre
  cur.setSort( cur.index( "date" ) );
  QString filtre = "";
  if (pourBilanAnnuel)
    filtre = "date_valeur LIKE \""+QString::number(annee)+"%\" AND id_usr="+QString::number(m_UsrPk);
  else  filtre = getFiltre("");
  cur.setFilter( filtre );

  if ( !cur.select() )
   {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::getMvts_MontantTotal() : Impossible de récupérer les honoraires :");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }
   else
   { QDate dt;
     while ( cur.next() )
      { if (cur.value( "type" ).toInt() == 0) 	 // Recettes
	 { recettes += cur.value( "montant" ).toDouble();
	   if (cur.value ("remarque").toString().find(PAIEMENT_ESPECES) != -1)
		{ esp += cur.value( "montant" ).toDouble(); }
	 }
	else  // Dépenses
	 { depenses += cur.value( "montant" ).toDouble();
	   if (cur.value ("remarque").toString().find(PAIEMENT_ESPECES) != -1)
		{ esp -= cur.value( "montant" ).toDouble(); }
	 }
      } // Fin while (requête finie)
    }

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}


//.------------------------------------ getHonoraire ------------------------------------------------------
/*! \brief Récupère en un passage les honoraires pour la période définie par le filtre.
  * \return La fonction renvoie un booléen : TRUE si aucun problème à l'exécution de la fonction, FALSE cas contraire.
  * \param id  : Identifiant de l'honoraire dans la base.
  * \param *hono : en retour l'honoraire est stocké dans ce pointeur.
*/
bool ComptaMoteurBase::getHonoraire( int id , Honoraires* hono )
{ // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Prépare le Curseur
  QSqlCursor cur("honoraires", TRUE, m_DataBase);
  // Trie et Filtre
  cur.setFilter( "id_hono="+QString::number(id) );
  if ( !cur.select() )
   {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::getHonoraires(id) : Impossible de récupérer l'honoraire : select\n\n");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }
   else
   { QDate dt;
     if ( cur.next() )
      {	dt = QDate::fromString(cur.value( "date" ).toString(), Qt::ISODate);
	// Construit l'honoraire pour chaque résultat de la requête
	//hono = new Honoraires();
	if (hono)
	{ readHonoraire(cur, hono); }
      } // Fin while (requête finie)
    }

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}


//.------------------------------------ getHonorairesFromBlobDepot ------------------------------------------------------
/*! \brief Récupère les honoraires concernés par un dépôt et les renvoie dans une HONO_LISTE.
  * \return La fonction renvoie un booléen : TRUE si aucun problème à l'exécution de la fonction, FALSE cas contraire.
  * \param listeHono  : La liste des honoraires telle que codée dans le blob du dépôt.
  * \param &lHono : en retour la liste des honoraires est stocké dans cette liste.
  * Si dépot d'espèce renvoie simplement un seul honoraire et le total du dépôt
*/
bool ComptaMoteurBase::getHonorairesFromBlobDepot( const char* listeHono , HONO_LISTE& lHono )
{ // Pour chaque id -> le récupérer
  QString tmp = listeHono;
  if (tmp.length() < 2) return FALSE;
  if (tmp.find(";") == -1) return FALSE;
  lHono.clear();

  QStringList ids = QStringList::split(";", listeHono);
  if (tmp.find(DEPOT_ESPECES) != -1)
  { Honoraires* h = new Honoraires();
    if (h)
    {	double total = 0.0;
	for ( QStringList::Iterator it = ids.begin(); it != ids.end(); ++it )
	{ QStringList montant = QStringList::split( "=", (*it));
	total += montant[0].toDouble() * montant[1].toDouble();
	}
	h->setEspeces( total );
	h->setCheque( total );
	h->setPatient( TR("Dépôt d'espèces"));
	lHono.append ( *h );
	delete h;
    }
    return TRUE;
  }

  for ( QStringList::Iterator it = ids.begin(); it != ids.end(); ++it )
   { Honoraires* h = new Honoraires();
     if (h)
	{ getHonoraire( (*it).toInt() , h   );
	  lHono.append ( *h );
	  delete h;
	}
   } // fin for
  return TRUE;
}



//.------------------------------------ getImmobilisations ------------------------------------------------------
/*! \brief Récupère en un passage les immobilisations enregistrées pour la période définie par le filtre.
  * ATTENTION, les dates du filtre doivent être \em préalablement remplies.
 * \return La fonction renvoie un booléen : TRUE si aucun problème à l'exécution de la fonction, FALSE cas contraire.
 * \param il : Pointeur vers la liste des immobilisations demandées ( IMMO_LISTE ).
  * \todo TRAVAILLER LE FILTRE +++++
*/
bool ComptaMoteurBase::getImmobilisations(IMMO_LISTE* il)
{ if (!il) return FALSE;
  il->clear();
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Prépare le Curseur
  QSqlCursor cur("immobilisations", TRUE, m_DataBase);
  // Trie et Filtre
  cur.setSort( cur.index( "date_service" ) );
  cur.setFilter( "id_usr="+QString::number(m_UsrPk) );

  if ( !cur.select() )
   {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::getImmobilisations() : Impossible de récupérer les immobilisations : select \n\n");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }
   else
   { while ( cur.next() )
      {	// Construit l'immobilisation pour chaque résultat de la requête
	Immobilisations *immo = new Immobilisations();
	if (immo)
	{ readImmobilisation(cur, immo);
	  il->append( *immo );
	  delete immo;
	}
      } // Fin while (requête finie)
    }

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}



//.------------------------------------ getFiltre ------------------------------------------------------
/*! \brief Construit le filtre des requêtes en fonction des données de CApp : m_Filtre_Date_Debut, m_Filtre_Date_Fin, m_Filtre_Type_Paiement, m_Filtre_Autre, m_Filtre_Change .
  * \param table = table de la base à filtrer (honoraires, depots...)
  * \param moreParams = 0 pour l'intégralité du filtre (dates, type des paiements...), DATES_ONLY pour que la fonction ne renvoie que les dates.

  * Ces variables sont initialisées par la classe Filtre_Hono et l'UI filtre_hono.ui / filtre_hono.ui.h.
 * Les dates correspondent aux périodes, Type_Paiement = chèques, espèces..., Autre = ce qu'il convient d'ajouter en plus.

 * \return La fonction renvoie le filtre calculé à insérer après la commande WHERE sans la commande WHERE. Renvoie QString::null si l'utilisateur n'a pas sélectionné de filtre, ou si une erreur s'est produite.
*/
QString ComptaMoteurBase::getFiltre(const char* table, int moreParams /*= 0*/)
{ QString filtre = QString::null;
  QString tb = table;

  // Définir l'utilisateur dont l'Id est dans CApp
  if (m_UsrPk != -1)
      { filtre += " `id_usr` = \'" + QString::number(m_UsrPk) + "\'"; }
  else return filtre;

  if (m_Filtre_Change)
   { // Définir les dates du filtre
     if ( m_Filtre_Date_Debut.isValid() && m_Filtre_Date_Fin.isValid() )
      {	//filtre =  "`"+tb+"`"+".";
        filtre += "\n AND `date` >= \'"+m_Filtre_Date_Debut.toString("yyyy-MM-dd") + "\'";
	filtre += "\n AND ";
	//filtre += "`"+tb+"`"+".";
	filtre += "`date` <= \'"+m_Filtre_Date_Fin.toString("yyyy-MM-dd") + "\'";
      }

     if (moreParams != DATES_ONLY)
     {	// Définir les type d'honoraires
	filtre += m_Filtre_Autre;
	// Définir les moyens de paiements du filtre
	if ( m_Filtre_Type_Paiement != "" && m_Filtre_Type_Paiement != TR("Tous") )
        { filtre += "\nAND ";
	  if (m_Filtre_Type_Paiement == TR("Espèces") ) filtre += "esp>0 ";
	  if (m_Filtre_Type_Paiement == TR("Chèques") ) filtre += "chq>0 ";
	  if (m_Filtre_Type_Paiement == TR("DAF") ) filtre += "daf>0 ";
	  if (m_Filtre_Type_Paiement == TR("CMU") ) filtre += "cmu>0 ";
	  if (m_Filtre_Type_Paiement == TR("Autres") ) filtre += "autre>0 ";
	  if (m_Filtre_Type_Paiement == TR("CB") ) filtre += "cb>0 ";
	  if (m_Filtre_Type_Paiement == TR("Virements") ) filtre += "vir>0 ";
        } // Fin if type paiement
        else return filtre; // erreur

     } // if NOT DATES_ONLY
   }
//qWarning(filtre);
  return filtre;
}


//.------------------------------------ getFiltrePeriode ------------------------------------------------------
/*! \brief Renvoie la période filtrée sous la forme QString prête à être affichée dans les rapports et synthèses.
*/
QString ComptaMoteurBase::getFiltrePeriode()
{ QString filtre = QString::null;
  if (m_Filtre_Change)
   { if ( m_Filtre_Date_Debut.isValid() && m_Filtre_Date_Fin.isValid() )
      {	filtre  = TR("dû ") + m_Filtre_Date_Debut.toString("dd MMMM yyyy") + " au ";
	filtre += m_Filtre_Date_Fin.toString("dd MMMM yyyy");
      }
     else return filtre; // erreur
   }
  return filtre;
}


//.------------------------------------ integrationEspecesRestantes ------------------------------------------------------
/*! \brief Réintégration automatique des espèces restantes d'une année définie dans la compta.
 * \return TRUE si tout OK, FALSE cas contraire. Message d'erreur stocké dans m_messageErreur de cette classe.
*/
bool ComptaMoteurBase::integrationEspecesRestantes(const char* annee)
{ QString an = annee;
  QString msg = "";
  bool tousHonosValide = TRUE;
 // Réintégration des espèces dans le bilan comptable
 double especes, hono_esp, mvts_esp, depots_esp, tmp1, tmp2;
 getHonoraires_MontantTotal(tmp1, hono_esp, tousHonosValide, TRUE, an.toInt() );
 especes = hono_esp;
 getMvts_MontantTotal(tmp1, tmp2, mvts_esp, TRUE, an.toInt() );
 especes += mvts_esp;
 getDepots_MontantTotal(tmp1, depots_esp, TRUE, an.toInt());
 especes -= depots_esp;

 if (especes > 0)
 { msg =  "<p align=center><b>"+TR("Réintégration des espèces")+"</b></p>";
   msg += "<p>"+TR("Vous avez déclaré :")+" <br />";
   msg += "  -  "+QString::number(hono_esp, 'f', 2)+" "+TR("euros d'espèces en honoraires,") +"<br />";
   msg += "  -  "+QString::number(mvts_esp, 'f', 2)+" "+TR("euros d'espèces en mouvements (recettes et dépenses),")+"<br />";
   msg += "  -  "+QString::number(depots_esp, 'f', 2)+" "+TR("euros d'espèces déposées en banque,")+"<br />";
   msg += "</p><p><b>"+TR("Le montant total des espèces à réintégrer dans l'exercice annuel est de :")+" ";
   msg += QString::number(especes)+" "+TR("euros.")+"</b></p>";
   msg += "<p>"+TR("Souhaitez-vous créer un mouvement nommé 'Prélèvement Praticien Espèces' pour réintégrer cette somme dans votre comptabilité ?")+"<br /><br />";
   msg += "<b>"+TR("Attention : ce mouvement calculé sera alors sauvegardé dans la base de données à la date du jour.")+"</b></p>";

   QWidget* l = new QWidget();
   if (QMessageBox::information(l, TR("Réintégration d'espèces dans l'exercice comptable"), msg, TR("Oui"), TR("Non"), "", 1, 0) == 0)
   { Mouvements* mvt_Esp = new Mouvements();
     mvt_Esp->setNom( TR("Prélèvements praticien en espèces") );
     mvt_Esp->setTotal(especes);
     mvt_Esp->setType( 1 );
     mvt_Esp->setDate( QDate::currentDate().toString("yyyy-MM-dd") );
     mvt_Esp->setDateValeur( QDate::currentDate().toString("yyyy-MM-dd") );
     mvt_Esp->setIdParent( 5 );
     mvt_Esp->setIdUsr( m_UsrPk  );
     mvt_Esp->setRemarque( PAIEMENT_ESPECES );
     // Sauvegarde ce mouvement dans la base de données
     saveMvtToBase( mvt_Esp );
     delete mvt_Esp;
   }
 } // Fin if especes > 0
 else
 { msg += "<p>"+TR("Aucune espèce à intégrer.")+"</p>";
 }
 return TRUE;
}

//.------------------------------------ getBilanAnnuel ------------------------------------------------------
/*! \brief Calcul le rapport annuel des recettes et des dépenses et le retourne sous la forme d'un tableau HTML (pour limiter l'utilisation de la mémoire).
 *  \param retour : tableau HTML de retour.
 *  \param annee : année pour le rapport.
 *  \param pourGrandLivre : FALSE par défaut. Si TRUE renvoie dans retour le document pour le grand livre, sinon renvoie le bilan comptable annuel.
 * \return TRUE si tout OK, FALSE cas contraire. Message d'erreur stocké dans m_messageErreur de cette classe.
*/
bool ComptaMoteurBase::getBilanAnnuel(const char* annee, QString& retour, bool pourGrandLivre)
{ if (m_Verbose) qWarning("ComptaMoteurBase :: getBilanAnnuel");
  double honoraires, recettes, depenses;
  bool   tousMvtsValide = TRUE;
  bool   tousHonosValide = TRUE;
  honoraires = 0;
  //dus = 0;
  recettes = 0;
  depenses = 0;
  QString an = annee;
  QStringList hd;
  QStringList lines;

  //.......... Prépare le tableau synthétique des mouvements
  MVTS_LISTE*	mvtsDispo = new MVTS_LISTE;
  if (!mvtsDispo) return FALSE;
  MVTS_LISTE*	mvtsListe = new MVTS_LISTE;
  if (!mvtsListe) return FALSE;
  MVTS_LISTE*	immoListe = new MVTS_LISTE;
  if (!immoListe) return FALSE;
  immoListe->clear();

  // récupère les mvtsdispo
  if (!getMvtsDispo( *mvtsDispo ))
   { delete mvtsDispo; delete mvtsListe; return FALSE; }
  // récupère le filtre actuel et prépare le filtre pour l'année
  QDate filtre_deb = m_Filtre_Date_Debut;
  QDate filtre_fin = m_Filtre_Date_Fin;
  m_Filtre_Date_Debut = QDate ( an.toInt() , 1, 1);
  m_Filtre_Date_Fin   = QDate ( an.toInt() , 12, 31);
  // récupère les mvts de l'année
  if (!getMvts( *mvtsListe ))
   { delete mvtsDispo; delete mvtsListe; return FALSE; }
  // remet le filtre à sa valeur initiale
  m_Filtre_Date_Debut = filtre_deb;
  m_Filtre_Date_Fin   = filtre_fin;

  // Réintégration des espèces dans le bilan comptable
  double especes, hono_esp, mvts_esp, depots_esp, double_tmp;
  getHonoraires_MontantTotal(honoraires, hono_esp, tousHonosValide, TRUE, an.toInt() );
  especes = hono_esp;
  getMvts_MontantTotal(recettes, depenses, mvts_esp, TRUE, an.toInt() );
  especes += mvts_esp;
  getDepots_MontantTotal(double_tmp, depots_esp, TRUE, an.toInt());
  especes -= depots_esp;

  // calcul les totaux de chaque rubriques
  MVTS_LISTE::iterator it;
  for (it = mvtsDispo->begin(); it !=  mvtsDispo->end(); ++it )
   { (*it).setTotal(0); }
  for (it = mvtsListe->begin(); it !=  mvtsListe->end(); ++it )
  { (*mvtsDispo->at( (*it).getIdParent() - 1)).addToTotal( (*it).getTotal() );
    if ((*it).getIdParent() == ID_IMMOBILISATIONS)
	immoListe->append( (*it) );
    else  if ((*it).getValidite() == 0)  { tousMvtsValide = FALSE; }

  }

    retour = "<html><head><meta name=\"qrichtext\" content=\"text/html; charset=utf-8\" />\n\n";
//    retour += "<style type=text/css>\nTABLE { border: 0.5px solid #000; width: 100%; border-spacing:0px; border-collapse:collapse; }\nTABLE TD { border: 0.5px solid #000; font-size: 7pt; border-collapse:collapse; }\n</STYLE>\n\n";
   retour += "</head>\n<body style=\"font-size:10pt;font-family:Sans Serif\">\n\n";

  if (!pourGrandLivre)
  {retour += "<p align=\"center\"><b>"+ TR("Bilan annuel pour l'année : ") + an + "</b><br />";
   retour += m_pUtilisateurActuel->getNom() + "<br />";
  }
  else
  {retour += "<p align=\"center\"><b>"+ TR("Grand livre comptable pour l'année : ") + an + "</b><br />";
   retour += m_pUtilisateurActuel->getNom() + "<br />";
  }
  retour += QString(TR("Édité le %1")).arg(QDateTime::currentDateTime().toString("dddd dd MMMM yyyy hh:mm:s")) +"</p>\n\n";

  retour += "<p align=center>";
  hd << TR("Analyse de la comptabilité.");
   if (especes > 0)
   {lines <<  "<b>"+TR("Il reste des espèces à intégrer dans la comptabilité")+"</b>";
   }
   else
   {lines << "<b>"+ TR("Toutes les espèces de l'année sont intégrées.") + "</b>";
   }

   if (!tousMvtsValide)
   { lines << "<b>"+TR("Tous les mouvements ne sont pas validés.")+"</b>";
   }
   else
   { lines << "<b>"+TR("Tous les mouvements de l'année sont validés.")+"</b>";
   }

   if (!tousHonosValide)
   { lines << "<b>"+TR("Tous les honoraires ne sont pas validés.")+"</b>";
   }
   else
   { lines << "<b>"+TR("Tous les honoraires de l'année sont validés.")+"</b>";
   }

   createHTMLTable(hd, lines, retour, 4,0);
   hd.clear();
   lines.clear();

   retour += "</p>";

  /////// CALCUL LE BILAN ANNUEL
  if (!pourGrandLivre)
  {if (m_Verbose) qWarning("ComptaMoteurBase :: calcule le Bilan Annuel");
   // Recettes
   hd.clear();
   lines.clear();

   //.......... Prépare le premier tableau récapitulatif des Recettes / Dépenses
    hd << TR("Honoraires") << TR("Recettes supplémentaires") << TR("Dépenses engagées") << TR("Total");
    lines << QString::number(honoraires, 'f', 2)+" = " +  QString::number(honoraires, 'f', 2) << QString::number(recettes, 'f', 2) << QString::number(depenses, 'f', 2) << QString::number( recettes+honoraires-depenses , 'f', 2);
    createHTMLTable(hd, lines, retour, 4,0);
    retour += "<br /><br />";
   hd.clear();
   lines.clear();


   hd << TR("Catégorie") << TR("Libellé") << TR("Débit") << TR("Crédit");
   lines << "<b>"+TR("Honoraires")+"</b>" << "&nbsp;" << "&nbsp;" << "&nbsp;" ;
   lines << "&nbsp;" << TR("Perçus") << "&nbsp;" << QString::number(honoraires, 'f', 2);
   //lines << "&nbsp;" << TR("Restant dus (non comptabilisés)");
   recettes = honoraires;

   for (it = mvtsDispo->begin(); it !=  mvtsDispo->end(); ++it )
    { if ((*it).getType() == 0)    // Recettes
      { if ((*it).getTotal() != 0)
        { recettes += (*it).getTotal();
 	 if ((*it).isParent() ) lines << "<b>"+(*it).getNom()+"</b>" << "&nbsp;" << "&nbsp;" <<  QString::number((*it).getTotal() , 'f', 2);
	  else lines  << "&nbsp;" << (*it).getNom()  << "&nbsp;" << QString::number((*it).getTotal(), 'f', 2);
       }
        else
 	if ((*it).isParent()) lines << "<b>"+(*it).getNom()+"</b>" << "&nbsp;" << "&nbsp;" << "&nbsp;";
      }
    }
   lines << "<b>"+TR("Total")+"</b>" << "&nbsp;" << "&nbsp;" << "<b>"+QString::number((recettes), 'f', 2)+"</b>";
   retour += "<br /><p align=\"center\"><b>"+TR("RECETTES")+"</b></p>";
   createHTMLTable(hd, lines, retour, 4,0);
   lines.clear();

   depenses = 0;
   for (it = mvtsDispo->begin(); it !=  mvtsDispo->end(); ++it )
   { if ((*it).getType() == 1)
     { if ((*it).getTotal() != 0)
       { depenses += (*it).getTotal();
	 if ((*it).isParent())  lines << "<b>"+(*it).getNom()+"</b>" << "&nbsp;" << QString::number((*it).getTotal(), 'f', 2) << "&nbsp;";
	 else lines  << "&nbsp;" << (*it).getNom() << QString::number((*it).getTotal(), 'f', 2) << "&nbsp;";
       }
       else
	if ((*it).isParent()) lines << "<b>"+(*it).getNom()+"</b>" << "&nbsp;" << "&nbsp;" << "&nbsp;";
     }
   }
   lines << "<b>"+TR("Total")+"</b>" << "&nbsp;" << "<b>"+QString::number(depenses, 'f', 2)+"</b>" << "&nbsp;";
   retour += "<br /><p align=\"center\"><b>"+TR("DÉPENSES")+"</b></p>";
   createHTMLTable(hd, lines, retour, 4,0);
   lines.clear();
   hd.clear();

  // Insère les tableaux des immobilisations
   if (immoListe->count() > 0)
   { int immob = 0;
     int totalVnc = 0;
     hd << TR("Libellé") << TR("Taux") << TR("Amortissement") << TR("VNC");
     for (it = immoListe->begin(); it !=  immoListe->end(); ++it )
     {	QString tx, am, vnc;
	tx="";   am="";   vnc="";
	getIntoRemarque( (*it).getRemarque(), TAUX_IMMOBILISATION, tx);
	getIntoRemarque( (*it).getRemarque(), VNC_IMMOBILISATION , vnc);
	lines  << (*it).getNom() << tx <<  QString::number((*it).getTotal(), 'f', 2) << vnc;
	immob += (int)(*it).getTotal();
	totalVnc += vnc.toInt();
     }
     lines << "<b>"+TR("Total")+"</b>" << "&nbsp;" << "<b>"+QString::number(immob, 'f', 2)+"</b>" << "<b>"+QString::number(totalVnc, 'f', 2)+"</b>";
     retour += "<br /><p align=\"center\"><b>"+TR("TABLEAU DES IMMOBILISATIONS")+"</b></p>";
     createHTMLTable(hd, lines, retour, 4,0);
     lines.clear();
     hd.clear();
   } // Fin if immoListe > 0


   // Récapitulatif
   hd << TR("Totaux") << TR("Débit") << TR("Crédit") << TR("Total");
   lines << TR("Pour l'année ")+an << QString::number(depenses, 'f', 2) <<  QString::number((recettes), 'f', 2) <<  QString::number((recettes-depenses), 'f', 2);
   //if (dus > 0)   lines << TR("Dont restant dus") << "&nbsp;" <<  QString::number(dus, 'f', 2) << "&nbsp;";
   ///retour += "<br />";
   //retour += "<br /><p align=\"center\"><b>"+TR("RÉCAPITULATIF")+"</b></p>";
   //createHTMLTable(hd, lines, retour, 4,0);
   //retour += "<br /><br />";

  } // Fin if (!grandLivre)
  else
  { //////////////// PREPARE LE GRAND LIVRE DES COMPTES ANNUEL
    if (m_Verbose) qWarning("ComptaMoteurBase :: calcule le Grand Livre des Comptes");
    QString line1, line2, lineTotal;
    line1 = "";
    line2 = "";
    lineTotal = "";
    QString tmp = "";
    hd.clear();
    lines.clear();
    double recettes, depenses;
    recettes = 0;
    depenses = 0;
    int col = 0;
    int id = 0;
    int nbSR = 0;
    QMemArray<int> cols (mvtsDispo->count());
    for (int i=0; i < (int)mvtsDispo->count(); ++i)
    { cols[i] = -1; }

   // Calcul les totaux des mvtsDispo
   // Initialise les totaux avant calculs
   // Prépare la QStringList pour trier les mouvements par idParents
   // L'idée est de stocker dans une string l'id du mouvement parent ';' l'id du mvt actuel
   // pour construire le tableau ensuite rapidement. Pour cela -> utilisation sort de QStringList...
   // Peut mieux faire !!!
   // Calcul le nb de zéro à mettre en début de string pour que le trie soit efficace
   int nbZeros = (mvtsDispo->count() / 10 ) - 1;

   for (it = mvtsDispo->begin(); it !=  mvtsDispo->end(); ++it )
   { (*it).setTotal(0); }

   // Calcul les totaux et
   QStringList lst;
   lst.clear();
   tmp = "";
   int indexList = 0;
   for (it = mvtsListe->begin(); it !=  mvtsListe->end(); ++it )
    { (*mvtsDispo->at( (*it).getIdParent() - 1 )).addToTotal( (*it).getTotal() );
      tmp = QString::number( (*it).getIdParent() );
      // Ajoute les zéros devant l'id si nécessaire
      int nbZerosToAdd = nbZeros - tmp.length();
      for (int i = 0; i < nbZerosToAdd;++i)
       { tmp.prepend("0"); }
      tmp += ";" + QString::number( indexList );
      indexList++;
      lst << tmp;
    }
   // Trie la liste par IdParent
   lst.sort();
   qWarning("ici");

   // Recettes
   // insère nom et date
   retour += "<p align=\"center\"><b>"+ TR("Recettes") + "</b></p>\n\n";
   line1 += "  <td colspan=1 rowspan=2 align=\"center\">" + TR("Libellé") + "</td>\n";
   line1 += "  <td colspan=1 rowspan=2 align=\"center\">" + TR("Date") + "</td>\n";
   lineTotal += "  <td colspan=1 rowspan=2 align=\"center\"><b>" + TR("Totaux") + "</b></td>\n";
   lineTotal += "  <td>&nbsp;</td>\n";

   for (it = mvtsDispo->begin(); it !=  mvtsDispo->end(); ++it )
    { if ((*it).getType() == 0)
      { if ((*it).isParent())					// Parent calculer le nombre de sous-rubriques
        { id = (*it).getId();
	  MVTS_LISTE sousRub;
	  nbSR = nbSubRubriques( *mvtsDispo, (*it).getId(), sousRub);
          if (nbSR > 0)
	  { tmp = "  <td colspan="+QString::number(nbSR)+" rowspan=1 align=\"center\">";
	    line1 += tmp + (*it).getNom() + "</td>\n";
	    // Boucle pour récupérer les sous-rubriques
	    MVTS_LISTE::iterator it2;
	    for (it2 = sousRub.begin(); it2 != sousRub.end(); it2++)
	    { col++;
	      cols[ (*it2).getId() - 1 ] = col;
	      line2 += "  <td>"+(*it2).getNom()+"</td>\n";
	      lineTotal += "  <td align=\"center\"><b>"+QString::number((*it2).getTotal(), 'f', 2)+"</b></td>\n";
	    }
	  }
	  else
	  {  col++;
	     cols[ id - 1 ] = col;
	     line1 += "  <td colspan=1 rowspan=2 align=\"center\">"+(*it).getNom() + "</td>\n";
	     lineTotal += "  <td align=\"center\"><b>"+QString::number((*it).getTotal(), 'f', 2)+"</b></td>\n";
	  }
        } // Fin isParent
      }  // Fin getType
    } // fin for

   retour += "<table cellpadding=1 cellspacing=2>";
   retour += " <tr bgcolor=\"#f0f0f0\">\n" + line1 + "\n </tr>\n <tr bgcolor=\"#f0f0f0\">\n"+ line2+"\n </tr>\n";
   QStringList bgcolor;
   bgcolor << "#f6f6f6" << "#f0f0f0";

   qWarning("2");



   int colorToUse = 0;

   for (it = mvtsListe->begin(); it !=  mvtsListe->end(); ++it )
     {if((*it).getType() == 0)
      {	retour += " <tr bgcolor=\""+bgcolor[colorToUse%2]+"\">\n<td align=\"center\">" + (*it).getNom() + "</td>\n";
	retour += "  <td align=\"center\">" + (*it).getDateValeur().toString("dd-MM-yyyy") + "</td>\n";
	int nbAvant = cols[ (*it).getIdParent() - 1] - 1;
	int i;
	for ( i = 0; i < (nbAvant);++i)
	 { retour += "  <td>&nbsp;</td>\n"; }
	retour += "  <td align=\"center\">" + QString::number((*it).getTotal(), 'f', 2) + "</td>\n";
	int nbApres = col - nbAvant - 1;
    for ( i = 0; i < nbApres; ++i)
	  { retour += "  <td>&nbsp;</td>\n"; }
	colorToUse++;
      }
     }


qWarning("3");


  retour += " </tr>\n <tr bgcolor=\""+bgcolor[colorToUse%2]+"\">"+lineTotal+"\n </tr>\n";
  retour += "</table>\n";

   // Depenses
   line1 = "";
   line2 = "";
   lineTotal = "";
   colorToUse = 0;
   col = 0;

   // insère nom et date
   retour += "<p align=\"center\"><b>"+ TR("Dépenses") + "</b></p>\n\n";
   line1 += "  <td colspan=1 rowspan=2 align=\"center\">" + TR("Libellé") + "</td>\n";
   line1 += "  <td colspan=1 rowspan=2 align=\"center\">" + TR("Date") + "</td>\n";
   lineTotal += "  <td colspan=1 rowspan=2 align=\"center\"><b>" + TR("Totaux") + "</b></td>\n";
   lineTotal += "  <td>&nbsp;</td>\n";

   for (it = mvtsDispo->begin(); it !=  mvtsDispo->end(); ++it )
    { if ((*it).getType() == 1)
      { if ((*it).isParent())					// Parent calculer le nombre de sous-rubriques
        { id = (*it).getId();
	  MVTS_LISTE sousRub;
	  nbSR = nbSubRubriques( *mvtsDispo, (*it).getId(), sousRub);
          if (nbSR > 0)
	  { tmp = "  <td colspan="+QString::number(nbSR)+" rowspan=1 align=\"center\">";
	    line1 += tmp + (*it).getNom() + "</td>\n";
	    // Boucle pour récupérer les sous-rubriques
	    MVTS_LISTE::iterator it2;
	    for (it2 = sousRub.begin(); it2 != sousRub.end(); it2++)
	    { col++;
	      cols[ (*it2).getId() - 1 ] = col;
	      line2 += "  <td>"+(*it2).getNom()+"</td>\n";
	      lineTotal += "  <td align=\"center\"><b>"+QString::number((*it2).getTotal(), 'f', 2)+"</b></td>\n";
	    }
	  }
	  else
	  {  col++;
	     cols[ id - 1 ] = col;
	     line1 += "  <td colspan=1 rowspan=2 align=\"center\">"+(*it).getNom() + "</td>\n";
	     lineTotal += "  <td align=\"center\"><b>"+QString::number((*it).getTotal(), 'f', 2)+"</b></td>\n";
	  }
        } // Fin isParent
      }  // Fin getType
    } // fin for



qWarning("4");

   retour += "<table cellpadding=1 cellspacing=2>";
   retour += " <tr bgcolor=\"#f0f0f0\">\n" + line1 + "\n </tr>\n <tr bgcolor=\"#f0f0f0\">\n"+ line2+"\n </tr>\n";

   for (it = mvtsListe->begin(); it !=  mvtsListe->end(); ++it )
     {if((*it).getType() == 1)
      {	retour += " <tr bgcolor=\""+bgcolor[colorToUse%2]+"\">\n<td align=\"center\">" + (*it).getNom() + "</td>\n";
	retour += "  <td align=\"center\">" + (*it).getDateValeur().toString("dd-MM-yyyy") + "</td>\n";
	int nbAvant = cols[ (*it).getIdParent() - 1] - 1;
	int l;
    for ( l = 0; l < (nbAvant);++l)
	 { retour += "  <td>&nbsp;</td>\n"; }
	    retour += "  <td align=\"center\">" + QString::number((*it).getTotal(), 'f', 2) + "</td>\n";
	int nbApres = col - nbAvant - 1;
	for ( l = 0; l < nbApres; ++l)
	  { retour += "  <td>&nbsp;</td>\n"; }
	    colorToUse++;
      }
     }

  qWarning("5");


  retour += " </tr>\n <tr bgcolor=\""+bgcolor[colorToUse%2]+"\">"+lineTotal+"\n </tr>\n";
  retour += "</table>\n";


  // TABLEAU GRAND LIVRE LINEAIRE

  retour += "<p align=\"center\"><b>"+TR("Tableau linéaire")+"</b></p>";

  hd << TR("Catégorie") << TR("Libellé") << TR("Date") << TR("Débit") << TR("Crédit");

  uint rubrique = 0; // sauvegarde la rubrique actuelle à laquelle on ajoute chaque lignes
  uint nouvelle  = 0; // pour tester si doit changer de catégorie
  double totalRubrique = 0.0;
  id = 0;
  QString str_Recettes, str_Depenses;

  // Insère le montant des honoraires dans les recettes
  lines << "<b>"+TR("Honoraires")+"</b>"<< TR("Pour l'année") << an << "&nbsp;";
  lines << QString::number(honoraires);
  uint n;
  for ( n=0; n<lst.count();++n)
  { id = lst[n].mid ( lst[n].find(";")+1, lst[n].length() ).toInt();
    it = mvtsListe->at( id );
    nouvelle = lst[n].mid ( 0 , lst[n].find(";") ).toInt();

    if (nouvelle > rubrique)	// Alors changement de catégorie
    { 	// Ajoute la somme de la rubrique précédente
	if (totalRubrique != 0)
        { if (totalRubrique > 0)
	  { str_Recettes = QString::number( totalRubrique ) ;
	    str_Depenses = "&nbsp;";
	    recettes += totalRubrique;
	  }
	  else
	  { str_Depenses = QString::number( totalRubrique*(-1) ) ;
	    str_Recettes = "&nbsp;";
	    depenses += totalRubrique;
	  }
	  lines << "&nbsp;" << TR("Total") << "&nbsp;" << str_Depenses << str_Recettes;
	  totalRubrique = 0;
	} // Fin if totalRubrique != 0
	// Ajoute les catégories jusqu'à ce que l'on soit dans la bonne
	while ((nouvelle > rubrique) && (rubrique < (mvtsDispo->count() - 1)))
	{ rubrique++;

	  if ((*mvtsDispo->at( rubrique - 1 )).isParent()) tmp = "<p align=left><b>--</b></p>";
	  else tmp = "<p align=right>--</p>";
	  tmp.replace ("--", (*mvtsDispo->at( rubrique - 1 )).getNom() );
	  lines << tmp << "&nbsp;" << "&nbsp;" << "&nbsp;" << "&nbsp;";
	} // fin While
	// Ajoute la ligne
	if ( (*it).getType() == 0)
	{ str_Recettes = QString::number( (*it).getTotal() ) ;
	  str_Depenses = "&nbsp;";
	  totalRubrique += (*it).getTotal();
	}
	else
	{ str_Depenses = QString::number( (*it).getTotal() ) ;
	  str_Recettes = "&nbsp;";
	  totalRubrique -= (*it).getTotal();
	}
	lines << "&nbsp;" << (*it).getNom() << (*it).getDateValeur().toString("dd-MM-yyyy") << str_Depenses << str_Recettes;
    }
    else			// reste dans la même catégorie
    { // Ajoute la ligne
      if ( (*it).getType() == 0)
	{ str_Recettes = QString::number( (*it).getTotal() ) ;
	  str_Depenses = "&nbsp;";
	  totalRubrique += (*it).getTotal();
	}
      else
	{ str_Depenses = QString::number( (*it).getTotal() ) ;
	  str_Recettes = "&nbsp;";
	  totalRubrique -= (*it).getTotal();
	}
      lines << "&nbsp;" << (*it).getNom() << (*it).getDateValeur().toString("dd-MM-yyyy") << str_Depenses << str_Recettes;
    }

  } // Fin for
  // Ajoute la dernière ligne (total de la dernière rubrique
  if (totalRubrique > 0)
  { str_Recettes = "<b>"+QString::number( totalRubrique )+"</b>" ;
    str_Depenses = "&nbsp;";
    recettes += totalRubrique;
  }
  else
  { str_Depenses = "<b>"+QString::number( totalRubrique*(-1) )+"</b>" ;
    str_Recettes = "&nbsp;";
    depenses += totalRubrique;
  }
  lines << "&nbsp;" << TR("Total") << "&nbsp;" << str_Depenses << str_Recettes;


  // Si reste des rubriques non utilisées les ajouter
  if (mvtsDispo->count() > 0)
  { while (rubrique < (mvtsDispo->count() - 1))
   { 	rubrique++;
	if ((*mvtsDispo->at( rubrique - 1 )).isParent()) tmp = "<p align=left><b>--</b></p>";
	else tmp = "<p align=right>--</p>";
	tmp.replace ("--", (*mvtsDispo->at( rubrique - 1 )).getNom() );
	lines << tmp << "&nbsp;" << "&nbsp;" << "&nbsp;" << "&nbsp;";
   } // fin While
  }

/*    for (it = mvtsListe->begin(); it !=  mvtsListe->end(); ++it )
     {	tmp = (*mvtsDispo->at( (*it).getIdParent() - 1)).getNom();
	if ((*it).getType() == 0) // Recettes
	 { lines << tmp << (*it).getNom() << (*it).getDateValeur().toString("dd-MM-yyyy") << "&nbsp;" << QString::number( (*it).getTotal() , 'f', 2);
	   recettes += (*it).getTotal();
	 }
	else
	 { lines << tmp << (*it).getNom() << (*it).getDateValeur().toString("dd-MM-yyyy") << QString::number( (*it).getTotal() , 'f', 2) << "&nbsp;";
	   depenses += (*it).getTotal();
	 }
	tmp = "";
     }
*/
   lines << "<b>"+TR("TOTAL")+"</b>" << TR("Année :") << an << QString::number( (depenses*(-1)) , 'f', 2) << QString::number( recettes+honoraires , 'f', 2);
   double reste = recettes+honoraires-depenses;
   if (reste > 0)
    lines << "&nbsp;"<< "&nbsp;"<< "&nbsp;"<< "&nbsp;"<< QString::number(reste);
   else
    lines << "&nbsp;"<< "&nbsp;"<< "&nbsp;"<< QString::number(reste)<< "&nbsp;";

   createHTMLTable(hd, lines, retour, 4,0);
   retour += "<br /><br />";

qWarning("6");

  } // Fin else if (!grandLivre)

  // gère mémoire
  delete mvtsDispo;
  delete mvtsListe;
  delete immoListe;

  // Fermer la base
  m_DataBase->close();
  return TRUE;

}


//.------------------------------------ nbSubRubriques ------------------------------------------------------
/*! \brief Retourne le nombre de sous-rubrique d'un mouvement parent.
*/
int ComptaMoteurBase::nbSubRubriques(MVTS_LISTE& mvts, int id, MVTS_LISTE& sRub)
{ MVTS_LISTE::iterator it;
  sRub.clear();
  int nb = 0;
  for (it = mvts.begin(); it != mvts.end(); it++)
  { if ((*it).getIdParent() == id)
    { nb++;
      sRub.append( (*it) );
    }
  }
  return nb;
}

//.------------------------------------ getMvtsDispo ------------------------------------------------------
/*! \brief Récupère la liste des mouvements disponibles dans la base pour construire le contenu du treeview.
 * \param ml : Liste des mouvements disponibles dans la base de données
 * \return TRUE : pas d'erreur, FALSE = erreur. Le message d'erreur est stocké dans m_messageErreur.
*/
bool ComptaMoteurBase::getMvtsDispo(MVTS_LISTE& ml)
{ ml.clear();

  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Prépare le Curseur
  QSqlCursor cur("mouvements_disponibles", TRUE, m_DataBase);
  // Trie et Filtre
  cur.setSort( cur.index( "id_mvt_dispo" ) );

  if ( !cur.select() )
   {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::getMvtsDispo() : Impossible de récupérer les mouvements disponibles :");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }
   else
   { while ( cur.next() )
      {	// Construit l'honoraire pour chaque résultat de la requête
	Mouvements *mvt = new Mouvements();
	if (mvt)
	{ mvt->setNom	 	(cur.value( "libelle" ).toString() ) ;
	  mvt->setRemarque	(cur.value( "remarque" ).toString() ) ;
	  mvt->setType 		(cur.value( "type" ).toInt() ) ;
	  mvt->setId 		(cur.value( "id_mvt_dispo" ).toInt() ) ;
	  mvt->setIdParent 	(cur.value( "id_mvt_parent" ).toInt() ) ;
	  ml.append( *mvt );
	  delete mvt;
	}
      } // Fin while (requête finie)
    }

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}



//.------------------------------------ getMvtsForReleve ------------------------------------------------------
/*! \brief Récupère la liste des mouvements et des dépôts dans la base pour construire les relevés de compte.
 * \param ml : Liste des mouvements disponibles dans la base de données
 * \return TRUE : pas d'erreur, FALSE = erreur. Le message d'erreur est stocké dans m_messageErreur.
*/
bool ComptaMoteurBase::getMvtsForReleve(MVTS_LISTE& ml)
{ ml.clear();

  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Récupère les mouvements
  QSqlCursor cur("mouvements", TRUE, m_DataBase);
  // Trie et Filtre
  cur.setSort( cur.index( "id_mouvement" ) );
  if ( !cur.select( "id_usr="+QString::number(m_UsrPk) ) )
   {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::getMvtsForReleve() : Impossible de récupérer les mouvements disponibles :");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }
   else
   { while ( cur.next() )
      {	// Construit l'honoraire pour chaque résultat de la requête
	Mouvements *mvt = new Mouvements();
	if (mvt)
	{ readMouvement(cur, mvt );
	  ml.append( *mvt );
	  delete mvt;
	}
      } // Fin while (requête finie)
    }


  cur = QSqlCursor("depots", TRUE, m_DataBase);
  if ( !cur.select( "id_usr="+QString::number(m_UsrPk) ) )
   { m_DataBase->close();
     m_messageErreur = TR("Impossible de récupérer le dépôt depuis la base de données :");
     m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
     return FALSE;
   }
  else
   while ( cur.next() )
    {	Mouvements* ret = new Mouvements();
	if (ret)
	{ ret->setIdUsr( cur.value( "id_depot" ).toInt() );
	  ret->setIdCompte( cur.value( "id_compte" ).toInt() );
	  ret->setDateValeur( cur.value("date").toString() );
	  // Récupère le montant total du dépôt
	  QString total = cur.value( "remarque").toString() ;
	  ret->setTotal( ret->getTotalDepot( total ).toDouble() );
	  ret->setType(0);
	  if (total.find(DEPOT_ESPECES) != -1) ret->setNom(TR("Dépôt d'espèces"));
	  else ret->setNom(TR("Remise de chèques"));
	  ml.append ( *ret );
	  delete ret;
	}
    }

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}



//.------------------------------------ getMvts ------------------------------------------------------
/*! \brief Récupère la liste des mouvements dans la base.
 * \param ml : Liste des mouvements disponibles dans la base de données
 * \return TRUE : pas d'erreur, FALSE = erreur. Le message d'erreur est stocké dans m_messageErreur.
*/
bool ComptaMoteurBase::getMvts(MVTS_LISTE& ml, int id_immobilisation)
{ ml.clear();
  QString filtre = getFiltre("", DATES_ONLY);
  filtre.replace("date","date_valeur");
  if (id_immobilisation != -1)
  { filtre = " remarque LIKE '%";
    filtre.append( IS_IMMOBILISATION );
    filtre.append( QString::number(id_immobilisation) + "_%'");
  }

  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Prépare le Curseur
  QSqlCursor cur("mouvements", TRUE, m_DataBase);
  // Trie et Filtre
  if ( !cur.select( filtre, cur.index( "date_valeur" ) ) )
   {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::getMvts() : Impossible de récupérer les mouvements disponibles :");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }
   else
   { while ( cur.next() )
      {	// Construit l'honoraire pour chaque résultat de la requête
	Mouvements *mvt = new Mouvements();
	if (mvt)
	{ readMouvement(cur, mvt);
	  ml.append( *mvt );
	  delete mvt;
	}
      } // Fin while (requête finie)
    }

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}


//.------------------------------------ getUsers ------------------------------------------------------
/*! \brief Récupère les infos de l'utilisateurs dont l'id est id_baseCompta.
 * \param resultat : Classe Utilisateurs qui contient les infos concernant l'utilisateur
 * \return TRUE si tout s'est déroulé correctement sinon FALSE. Le message d'erreur est stocké dans m_messageErreur de cette classe.
 * \sa CApp , usersListe .
*/
bool ComptaMoteurBase::getUsers(int id_baseCompta, Utilisateurs& resultat)
{ // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Prépare le Curseur
  QSqlCursor cur("utilisateurs", TRUE, m_DataBase);
  if ( !cur.select ("id_usr= \'" + QString::number(id_baseCompta) + "\'"))
   {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::getUsers : Impossible de récupérer l'identifiant utisateur : select \n\n");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }
  else if (cur.next() )
      {	resultat = Utilisateurs();
	resultat.setId( cur.value( "id_usr" ).toInt() );
      	resultat.setNom( cur.value( "nom_usr" ).toString() );
	resultat.setDrTuxUsrPk(  cur.value( "id_drtux_usr" ).toInt() );
	resultat.setLogin(  cur.value( "login" ).toString() );
	resultat.setPassword( cur.value( "password" ).toString() );
      }
  else
   {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::getUsers : Impossible de récupérer l'identifiant utisateur : next \n\n");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}


//.------------------------------------ getVersion ------------------------------------------------------
/*! \brief Récupère les infos de la version actuelle telles que sauvegardées dans la base.
 * \param resultat : contient le texte de la version
 * \return TRUE si tout s'est déroulé correctement sinon FALSE. Le message d'erreur est stocké dans m_messageErreur de cette classe.
 * \sa CApp , usersListe .
*/
bool ComptaMoteurBase::getVersion(QString& resultat)
{ resultat = "";
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Prépare le Curseur
  QSqlCursor cur("z_version", TRUE, m_DataBase);
  if ( !cur.select() )
   {	m_DataBase->close();
	m_messageErreur = TR("ERREUR : ComptaMoteurBase::getVersion : Impossible de récupérer le champs de version : select \n\n");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }
  else if (cur.next() )
      {	resultat = cur.value( "infos_version" ).toString(); }
  else
   {	m_DataBase->close();
	m_messageErreur = TR("ERREUR : ComptaMoteurBase::getVersion : Impossible de récupérer le Champs de Version : next \n\n");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}


//.------------------------------------ getUsersForList ------------------------------------------------------
/*! \brief Récupère la liste de toutes les utilisateurs de l'application de comptabilité et la sauvegarde dans ul.
 * \param dt_depot : date du dépôt (date de validation par exemple)
 * \return TRUE si tout s'est déroulé correctement sinon FALSE. Le message d'erreur est stocké dans m_messageErreur de cette classe.
 * \sa CApp , usersListe .
*/
bool ComptaMoteurBase::getUsersForList(UTILISATEURS_LISTE * ul)
{ ul->clear();
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Prépare le Curseur
  QSqlCursor cur("utilisateurs", TRUE, m_DataBase);
  if ( !cur.select () )
   {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::getUsersForList : Impossible de récupérer les utisateurs du logiciel de comptabilité : select\n");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }
  else while (cur.next() )
      {	Utilisateurs* resultat = new Utilisateurs();
	if (resultat)
	{ resultat->setId( cur.value( "id_usr" ).toInt() );
	  resultat->setNom( cur.value( "nom_usr" ).toString() );
	  resultat->setDrTuxUsrPk(  cur.value( "id_drtux_usr" ).toInt() );
	  resultat->setLogin(  cur.value( "login" ).toString() );
	  resultat->setPassword( cur.value( "password" ).toString() );
	  ul->append( *resultat );
	  delete resultat;
	}
      }

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}


//.------------------------------------ getIdUsrFromDrTuxPk ------------------------------------------------------
/*! \brief Trouve la clé primaire de l'utilisateur de la base comptabilité à partir de l'identifiant (primKey) utilisateur de drtux.
 * \return TRUE si tout s'est déroulé correctement sinon FALSE. Le message d'erreur est stocké dans m_messageErreur de cette classe.
 * \sa FirstUse, FormFirstUse, FormPlugins, CApp
*/
bool ComptaMoteurBase::getIdUsrFromDrTuxPk(int &idDrTuxToModify)
{ // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Prépare le Curseur
  QSqlCursor cur("utilisateurs", TRUE, m_DataBase);
  if ( !cur.select ("id_drtux_usr= \'" + QString::number(idDrTuxToModify) + "\'"))
   {	m_DataBase->close();
	m_messageErreur = TR("Impossible de récupérer l'identifiant utisateur :");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }
  else if (cur.next() )
    {	idDrTuxToModify = cur.value( "id_usr" ).toInt();    }
  else
   {	m_DataBase->close();
	m_messageErreur = TR("Impossible de récupérer l'identifiant utisateur :");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}


//.------------------------------------ getIdListeFromIdCpt ------------------------------------------------------
/*! \brief Retourne l'id dans une liste de compte du compte à l'Id IdCpt.
 * \return -1 si rien n'est trouvé, sinon l'id dans la liste.
*/
int ComptaMoteurBase::getIdListeFromIdCpt(COMPTES_LISTE& cl, int idCpt)
{ COMPTES_LISTE::iterator it;
  int id = 0;
  int findId = -1;
  for (it = cl.begin(); it != cl.end(); ++it)
  { if ((*it).getId() == idCpt) { findId = id; break; }
    id++;
  }
  return findId;
}


//.------------------------------------ checkDoubleUser ------------------------------------------------------
/*! \brief Recherche dans la base comptabilité l'existence d'un utilisateur avec un identifiant DrTux = idDrTux.
 * \param idDrTux : Identifiant à rechercher
 * \param retour : -1 si aucun utilisateur trouvé, sinon = Identifiant de l'utilisateur comptabilité (id_usr).
 * Cette fonction est utilisée pour éviter les doublons d'utilisateurs liés au même utilisateur DrTux.
 * \return TRUE si tout s'est déroulé correctement sinon FALSE. Le message d'erreur est stocké dans m_messageErreur de cette classe.
 * \sa FirstUse, FormFirstUse, saveUserToBase
*/
bool ComptaMoteurBase::checkDoubleUser(int idDrTux, int &retour)
{ retour = -1;
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Prépare le Curseur
  QSqlCursor cur("utilisateurs", TRUE, m_DataBase);
  if ( !cur.select ("id_drtux_usr= \'" + QString::number(idDrTux) + "\'"))
   {	m_DataBase->close();
	m_messageErreur = TR("Impossible de récupérer l'identifiant utisateur :");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }
  else if (cur.next() )
    {	retour = cur.value( "id_usr" ).toInt();    }
  else
   {	m_DataBase->close();
	m_messageErreur = TR("Impossible de récupérer l'identifiant utisateur :");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}


//.------------------------------------ getDrTuxUsers ------------------------------------------------------
/*! \brief Récupère la liste de toutes les utilisateurs de MedinTux et la sauvegarde dans ul.
 * \return TRUE si tout s'est déroulé correctement sinon FALSE. Le message d'erreur est stocké dans m_messageErreur de cette classe.
 * \sa FirstUse, FormFirstUse
 * ATTENTION la base doit être celle de MEDINTUX et pas celle de comptabilité...
*/
bool ComptaMoteurBase::getDrTuxUsers(UTILISATEURS_LISTE* ul)
{ // Prépare la requête SQL et prépare la liste
  QString req = "SELECT  `Login` , `PassWord` , `Nom` ,  `Prenom` , \n `ID_PrimKey` , `Titre`  \nFROM `";
  req += BASE_TUX_TABLE;
  req += "` WHERE Droits Like \"%-med%\" ORDER BY Nom ASC ";
  ul->clear();

  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Execute la requete
  QSqlQuery query(req , m_DataBase );
  if (!query.isActive())
     {	m_DataBase->close();
 	m_messageErreur = TR("ComptaMoteurBase::getDrTuxUsers :: Impossible de récupérer la liste des utilisateurs de MedinTux : isActive ");
	m_messageErreur += "\n\n" + query.lastError().driverText() + "\n\n" + query.lastError().databaseText() + ".";
	return FALSE;
     }
  else
   { while ( query.next() )
      {	Utilisateurs *u = new Utilisateurs();
	if (u)
	{ u->setId( query.value(4).toInt()  );
	  u->setDrTuxUsrPk ( query.value(4).toInt()  );
	  QString nom_complet =  query.value(2).toString() +" "+ query.value(3).toString() ;
	  QString titre = query.value(5).toString();
	  if (titre != "") nom_complet.prepend(titre + " ");
	  u->setNom( nom_complet );
	  u->setPassword(  query.value(1).toString()  );
	  u->setLogin( query.value(0).toString()  );
	  ul->append( *u );
	  delete u;
	}
      }
    }
  // Fermer la base
  m_DataBase->close();
  return TRUE;
}


//.------------------------------------ getComptes ------------------------------------------------------
/*! \brief Récupère les comptes bancaires de l'utilisateur courant.
 * \param depot_id : Identifiant du dépôt.
 * \param retour : le dépôt récupéré encapsulé dans sa classe
 * \sa saveDepotToBase .
*/
bool ComptaMoteurBase::getComptes(COMPTES_LISTE& cl)
{ m_messageErreur = "";
  cl.clear();
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  QSqlCursor cur("comptes_bancaires", TRUE, m_DataBase);
  cur.setSort( cur.index( "id_compte"));
  if ( !cur.select( "id_usr="+QString::number(m_UsrPk)) )
   { m_DataBase->close();
     m_messageErreur = TR("ComptaMoteurBase::getComptes : Impossible de récupérer les comptes bancaires depuis la base de données : select \n\n");
     m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
     return FALSE;
   }
  else
   while ( cur.next() )
    {	Comptes* c= new Comptes();
	if (c)
	{ QString bq = cur.value( "rib_code_banque" ).toString();
	  QString gu = cur.value( "rib_code_guichet" ).toString();
	  QString cpt = cur.value( "rib_numcompte" ).toString();
	  QString cle = cur.value( "rib_cle" ).toString();
	  c->setRib(  bq, gu , cpt , cle);
	  c->setNomBanque( cur.value( "nom_banque" ).toString() );
	  c->setLibelle ( cur.value( "libelle" ).toString() );
	  c->setTitulaire(cur.value( "titulaire" ).toString() );
	  c->setRemarque( cur.value( "remarque" ).toString() );
	  c->setSolde( cur.value( "solde_initial" ).toDouble() );
	  c->setId ( cur.value ("id_compte").toInt() );
	  c->setIdUsr ( cur.value ("id_usr").toInt());
	  cl.append( *c );
	  delete c;
	}
    } // Fin While

  m_DataBase->close();
  return TRUE;
}


//.------------------------------------ getDepot ------------------------------------------------------
/*! \brief Récupère le dépôt dont l'identifiant est le 'depot_id'.
 * \param depot_id : Identifiant du dépôt.
 * \param retour : le dépôt récupéré encapsulé dans sa classe
 * \sa saveDepotToBase .
*/
bool ComptaMoteurBase::getDepot(int depot_id, Depots &retour)
{ m_messageErreur = "";
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  QSqlCursor cur("depots", TRUE, m_DataBase);
  if ( !cur.select( "id_usr="+QString::number(m_UsrPk)+" AND id_depot="+ QString::number(depot_id) ) )
   { m_DataBase->close();
     m_messageErreur = TR("Impossible de récupérer le dépôt depuis la base de données :");
     m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
     return FALSE;
   }
  else
   if ( cur.next() )
    {	retour.setListeIdHono( cur.value( "blob_depot" ).toString() );
	retour.setIdUser( cur.value( "id_depot" ).toInt() );
	retour.setIdCompte( cur.value( "id_compte" ).toInt() );
	retour.setDateValidation( cur.value("date").toString() );
	retour.setDateDebut( cur.value( "periode_deb" ).toString() );
	retour.setDateFin( cur.value( "periode_fin" ).toString() );;
	retour.setIdDepot( cur.value( "id_depot" ).toInt() );
	retour.setRemarque ( cur.value( "remarque").toString() );
    }
   else { m_DataBase->close(); return FALSE;}
  m_DataBase->close();
  return TRUE;
}



//.------------------------------------ getDepotsForList ------------------------------------------------------
/*! \brief Récupère tous les dépôts de l'utilisateur.
 * \param depot_id : Identifiant du dépôt.
 * \param retour : le dépôt récupéré encapsulé dans sa classe
 * \todo Ajouter le filtre ( à l'année ?? )
 * \sa saveDepotToBase .
*/
bool ComptaMoteurBase::getDepotsForList(DEPOTS_LISTE& retour)
{ m_messageErreur = "";
  retour.clear();
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  QSqlCursor cur("depots", TRUE, m_DataBase);
  cur.setSort( cur.index( "date"));
  if ( !cur.select( "id_usr="+QString::number(m_UsrPk) ) )
   { m_DataBase->close();
     m_messageErreur = TR("ComptaMoteurBase::getDepotsForList : Impossible de récupérer le dépôt depuis la base de données : select \n\n");
     m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
     return FALSE;
   }
  else
   while ( cur.next() )
    {	Depots* ret = new Depots();
	if (ret)
	{ ret->setListeIdHono( cur.value( "blob_depot" ).toString() );
	  ret->setIdUser( cur.value( "id_depot" ).toInt() );
	  ret->setIdCompte( cur.value( "id_compte" ).toInt() );
	  ret->setDateValidation( cur.value("date").toString() );
	  ret->setDateDebut( cur.value( "periode_deb" ).toString() );
	  ret->setDateFin( cur.value( "periode_fin" ).toString() );;
	  ret->setIdDepot( cur.value( "id_depot" ).toInt() );
	  ret->setRemarque ( cur.value( "remarque").toString() );
	  retour.append ( *ret );
	  delete ret;
	}
    }
  m_DataBase->close();
  return TRUE;
}


/*! \brief Update l'acte passé en paramètre.
 * \return TRUE si tout s'est déroulé correctement sinon FALSE. Le message d'erreur est stocké dans m_messageErreur de cette classe.
*/
bool ComptaMoteurBase::updateActe(Actes* acte)
{ m_messageErreur = "";
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  QSqlCursor cur("actes_disponibles", TRUE, m_DataBase);

  if ( !cur.select( "id_acte_dispo="+ QString::number( acte->getId() ) ) )
   { m_DataBase->close();
     m_messageErreur = TR("ComptaMoteurBase::updateActe : Impossible de mettre à jour l'acte dans la base de données : select ");
     m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
     return FALSE;
   }
  else // Select OK
   if ( !cur.canUpdate() )
    {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::updateActe : Impossible de mettre à jour l'acte dans la base de données : canUpdate ");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
    }
   else // peut updater
    if ( cur.next() )
      {	QSqlRecord *buffer = cur.primeUpdate();
	if (buffer)
	{ buffer->setValue( "nom_acte", acte->getNom() );
	  buffer->setValue( "desc_acte", acte->getDesc() );
	  buffer->setValue( "type", acte->getType() );
	  buffer->setValue( "montant_total", QString::number(acte->getTotal()) );
	  buffer->setValue( "montant_tiers", QString::number(acte->getTiers()) );
	  if (!cur.update())
	   {m_DataBase->close();
	    m_messageErreur = TR("ComptaMoteurBase::updateActe : Impossible de mettre à jour l'acte dans la base de données : update");
	    m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	    return FALSE;
	   }

	} // Fin if buffer
       } // Fin cur.next()
  m_DataBase->close();
  return TRUE;
}



/*! \brief Update le champs Remarque l'honoraire passé en paramètre.
 * \return TRUE si tout s'est déroulé correctement sinon FALSE. Le message d'erreur est stocké dans m_messageErreur de cette classe.
*/
bool ComptaMoteurBase::updateRemarqueHonoraires(Honoraires *hono)
{ m_messageErreur = "";
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  QSqlCursor cur("honoraires", TRUE, m_DataBase);
  if ( !cur.select( "id_hono="+ QString::number( hono->getId() ) ) )
   { m_DataBase->close();
     m_messageErreur = TR("ComptaMoteurBase::updateRemarqueHonoraires : Impossible de mettre à jour les honoraires dans la base de données : select\n");
     m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
     return FALSE;
   }
  else // Select OK
   if ( !cur.canUpdate() )
    {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::updateRemarqueHonoraires : Impossible de mettre à jour les honoraires dans la base de données : canUpdate\n");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
    }
   else // peut updater
    if ( cur.next() )
      {	QSqlRecord *buffer = cur.primeUpdate();
	if (buffer)
	{ buffer->setValue( "remarque", hono->getRemarque() );
	  if (!cur.update())
	  {m_DataBase->close();
	   m_messageErreur = TR("ComptaMoteurBase::updateRemarqueHonoraires : Impossible de mettre à jour les honoraires dans la base de données : update\n");
	   m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	   return FALSE;
	  }

	}
       }
  m_DataBase->close();
  return TRUE;
}


/*! \brief Update le champs Remarque l'honoraire passé en paramètre.
 * \return TRUE si tout s'est déroulé correctement sinon FALSE. Le message d'erreur est stocké dans m_messageErreur de cette classe.
*/
bool ComptaMoteurBase::updateHonoraires(Honoraires *pHono)
{ m_messageErreur = "";
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  QSqlCursor cur("honoraires", TRUE, m_DataBase);
  if ( !cur.select( "id_hono="+ QString::number( pHono->getId() ) ) )
   { m_DataBase->close();
     m_messageErreur = TR("ComptaMoteurBase::updateRemarqueHonoraires : Impossible de mettre à jour les honoraires dans la base de données : select\n");
     m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
     return FALSE;
   }
  else // Select OK
   if ( !cur.canUpdate() )
    {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::updateRemarqueHonoraires : Impossible de mettre à jour les honoraires dans la base de données : canUpdate\n");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
    }
   else // peut updater
    if ( cur.next() )
      {	QSqlRecord *buffer = cur.primeUpdate();
	if (buffer)
	{ // Reformater la date ++++
	  QDate dt;
	  dt = dt.fromString( dateForSQL(pHono->m_Date) , Qt::ISODate);
	  buffer->setValue( "patient", pHono->getPatient() );
	  buffer->setValue( "praticien", pHono->getPraticien() );
	  buffer->setValue( "date", dt.toString( "yyyy-MM-dd" ) );
	  buffer->setValue( "acte", pHono->getActesForBase() );
	  buffer->setValue( "remarque", pHono->getRemarque() );
	  buffer->setValue( "esp", pHono->getEsp() );
	  buffer->setValue( "chq", pHono->getChq() );
	  buffer->setValue( "cb", pHono->getCB() );
	  buffer->setValue( "daf", pHono->getDAF() );
	  buffer->setValue( "autre", pHono->getAutre() );
	  buffer->setValue( "cmu", pHono->getCMU() );
      buffer->setValue( "vir", pHono->getVirement() );
	  buffer->setValue( "id_usr", pHono->getIdUsr() );
	  buffer->setValue( "GUID", pHono->getGUID() );
	  buffer->setValue( "id_drtux_usr", pHono->getDrTuxUsr() );
	  buffer->setValue( "valide", pHono->getValidite() );
	  buffer->setValue( "tracabilite", pHono->getTracabilite() );

	cur.update();

	} // Fin  buffer
       } // Fin next
  m_DataBase->close();
  return TRUE;
}



/*! \brief Update le champs Remarque l'honoraire passé en paramètre.
 * \return TRUE si tout s'est déroulé correctement sinon FALSE. Le message d'erreur est stocké dans m_messageErreur de cette classe.
*/
bool ComptaMoteurBase::updateCpt(Comptes* pCpt)
{ m_messageErreur = "";
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  QSqlCursor cur("comptes_bancaires", TRUE, m_DataBase);
  if ( !cur.select( "id_compte="+ QString::number( pCpt->getId() ) ) )
   { m_DataBase->close();
     m_messageErreur = TR("ComptaMoteurBase::updateCpt : Impossible de mettre à jour les honoraires dans la base de données : select\n");
     m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
     return FALSE;
   }
  else // Select OK
   if ( !cur.canUpdate() )
    {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::updateCpt : Impossible de mettre à jour les honoraires dans la base de données : canUpdate\n");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
    }
   else // peut updater
    if ( cur.next() )
      {	QSqlRecord *buffer = cur.primeUpdate();
	if (buffer)
	{ buffer->setValue( "id_usr", pCpt->getIdUsr() );
	  buffer->setValue( "nom_banque", pCpt->getNomBanque() );
	  buffer->setValue( "titulaire", pCpt->getTitulaire() );
	  buffer->setValue( "libelle", pCpt->getLibelle() );
	  buffer->setValue( "rib_code_banque", pCpt->getRibBanque() );
	  buffer->setValue( "rib_code_guichet", pCpt->getRibGuichet() );
	  buffer->setValue( "rib_numcompte", pCpt->getRibCompte() );
	  buffer->setValue( "rib_cle", pCpt->getRibCle() );
	  buffer->setValue( "solde_initial", pCpt->getSolde() );
	  buffer->setValue( "remarque", pCpt->getRemarque() );

	  cur.update();

	} // Fin  buffer
       } // Fin next
  m_DataBase->close();
  return TRUE;
}


/*! \brief Update l'immobilisation passé en paramètre.
 * \return TRUE si tout s'est déroulé correctement sinon FALSE. Le message d'erreur est stocké dans m_messageErreur de cette classe.
*/
bool ComptaMoteurBase::updateImmob(Immobilisations* pImm)
{ m_messageErreur = "";
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  QSqlCursor cur("immobilisations", TRUE, m_DataBase);
  if ( !cur.select( "id_immob="+ QString::number( pImm->getId() ) ) )
   { m_DataBase->close();
     m_messageErreur = TR("ComptaMoteurBase::updateImmob : Impossible de mettre à jour les immobilisations dans la base de données 'immobilisations' : select\n");
     m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
     return FALSE;
   }
  else // Select OK
   if ( !cur.canUpdate() )
    {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::updateImmob : Impossible de mettre à jour les immobilisations dans la base de données 'immobilisations' : canUpdate\n");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
    }
   else // peut updater
    if ( cur.next() )
      {	QSqlRecord *buffer = cur.primeUpdate();
	if (buffer)
	  { buffer->setValue( "id_immob", pImm->getId() );
	    buffer->setValue( "id_compte", pImm->getIdCompte() );
	    buffer->setValue( "id_usr", pImm->getIdUsr() );
	    buffer->setValue( "libelle", pImm->getNom() );
	    buffer->setValue( "date_service", pImm->getDateService().toString("yyyy-MM-dd") );
	    buffer->setValue( "duree", pImm->getDuree() );
	    buffer->setValue( "mode", pImm->getMode() );
	    buffer->setValue( "valeur", pImm->getValeur() );
	    buffer->setValue( "valeur_residuelle", pImm->getResiduel() );
	    buffer->setValue( "resultat", pImm->getResultat() );
	    buffer->setValue( "remarque", pImm->getRemarque() );
	    cur.update();
	} // Fin  buffer
       } // Fin next

  // Supprimer les anciens mvts liés à cette immobilisation
  QString q = "DELETE FROM `mouvements` WHERE `remarque` LIKE '%";
  q += IS_IMMOBILISATION;
  q += QString::number( pImm->getId() ) +"_%'";
  bool ok = TRUE;

  QSqlQuery query(q , m_DataBase );
  if (query.isValid())
   if (!query.exec())
     {	m_DataBase->close();
	m_messageErreur = TR("Impossible de supprimer les mouvements antérieurs liés à l'immobilisation lors de sa mise à jour : ");
	m_messageErreur += "\n\n" + query.lastError().driverText() + "\n\n" + query.lastError().databaseText() + ".";
	return FALSE;
      }

  // Sauvegarder les mouvements un à un
   MVTS_LISTE mvts;
   pImm->getMouvements( mvts );
   MVTS_LISTE::iterator it;
   ok = TRUE;
   for (it = mvts.begin(); it!=mvts.end(); ++it)
   { (*it).setIdUsr( m_UsrPk );
     (*it).setIdImmob( pImm->getId() );
     if (!saveMvtToBase( &(*it) )) { ok=FALSE; break;}
   }

  m_DataBase->close();
  return TRUE;
}



/*! \brief Update le champs Remarque l'honoraire passé en paramètre.
 * \return TRUE si tout s'est déroulé correctement sinon FALSE. Le message d'erreur est stocké dans m_messageErreur de cette classe.
*/
bool ComptaMoteurBase::updateMvt(Mouvements* pMvt)
{ m_messageErreur = "";
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  QSqlCursor cur("mouvements", TRUE, m_DataBase);
  if ( !cur.select( "id_mouvement="+ QString::number( pMvt->getId() ) ) )
   { m_DataBase->close();
     m_messageErreur = TR("ComptaMoteurBase::updateMvt : Impossible de mettre à jour les honoraires dans la base de données : select\n");
     m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
     return FALSE;
   }
  else // Select OK
   if ( !cur.canUpdate() )
    {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::updateMvt : Impossible de mettre à jour les honoraires dans la base de données : canUpdate\n");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
    }
   else // peut updater
    if ( cur.next() )
      {	QSqlRecord *buffer = cur.primeUpdate();
	if (buffer)
	{ buffer->setValue( "id_mvt_dispo", pMvt->getIdParent() );
	  buffer->setValue( "id_usr", pMvt->getIdUsr() );
	  buffer->setValue( "id_compte", pMvt->getIdCompte() );
	  buffer->setValue( "type", pMvt->getType() );
	  buffer->setValue( "libelle", pMvt->getNom() );
	  buffer->setValue( "date", pMvt->getDate().toString("yyyy-MM-dd") );
	  buffer->setValue( "date_valeur", pMvt->getDateValeur().toString("yyyy-MM-dd") );
	  buffer->setValue( "remarque", pMvt->getRemarque() );
	  buffer->setValue( "montant", pMvt->getTotal() );
	  buffer->setValue( "valide", pMvt->getValidite() );
	  buffer->setValue( "tracabilite", pMvt->getTracabilite() );


	  cur.update();

	} // Fin  buffer
       } // Fin next
  m_DataBase->close();
  return TRUE;
}


/*! \brief Sauvegarde une remise de chèque à la date indiquée pour une période donnée avec une liste d'honoraires.
 * \param dt_depot : date du dépôt (date de validation par exemple)
 * \param hl : liste des honoraires concernés par le dépôt
 * \param rib : rib du compte ou se fait le dépôt
 * \sa getDepot , updateRemarqueHonoraires , getRibForBase .
*/
bool ComptaMoteurBase::saveDepotToBase(	QDate* dt_depot, HONO_LISTE* hl, const char* rib, int idCpt)
{ QString tmp = rib;
  tmp += TOTAL_DEPOT;
  m_messageErreur = "";
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Prépare le BLOB du dépôt : il contient tous les id_hono séparés par un point virgule.
  QString blobDepot = ";"; // Débute par un ; pour les recherche (tous les id débutent par ; et finissent par ;)
  double total = 0;
  HONO_LISTE::iterator it;
  for (it = hl->begin(); it != hl->end(); ++it)
   { blobDepot += QString::number((*it).getId()) + ";" ; total += (*it).total(); }
  tmp += QString::number(total);

  QSqlCursor cur("depots", TRUE, m_DataBase);
  if ( !cur.canInsert ())
   { m_DataBase->close();
     m_messageErreur = TR("Impossible d'ajouter le dépôt dans la base de données :");
     m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
     return FALSE;
   }
  //............... si OK on recupere le buffer et on le charge avec les données .........
  QSqlRecord *buffer = cur.primeInsert();
  if (buffer)
  { buffer->setValue( "date",        dt_depot->toString("yyyy-MM-dd") );
    buffer->setValue( "periode_deb", m_Filtre_Date_Debut.toString("yyyy-MM-dd") );
    buffer->setValue( "periode_fin", m_Filtre_Date_Fin.toString("yyyy-MM-dd") );
    buffer->setValue( "blob_depot", blobDepot);
    buffer->setValue( "remarque", tmp);
    buffer->setValue( "id_usr", QString::number(m_UsrPk));
    buffer->setValue( "id_compte", idCpt );
    if (!cur.insert())
     { m_DataBase->close();
       m_messageErreur = TR("Impossible d'ajouter le dépôt dans la base de données :");
       m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
       return FALSE;
     }
    else // Mettre à jour les champs remarque des honoraires (les chèques sont maintenant déposés...)
     {  for (it = hl->begin(); it != hl->end(); ++it)
 	{ tmp = (*it).getRemarque();
	  tmp.replace( CHEQUE_PAS_DEPOSE , CHEQUE_DEPOSE );
	  (*it).setRemarque(tmp);
	  updateRemarqueHonoraires( &(*it) );
	}
     } // Fin insert

  } // Fin if buffer
  // Fermer la base
  m_DataBase->close();
  return TRUE;
}



/*! \brief Sauvegarde un dépot d'espèces.
 * \param depot : pointeur vers le dépôt à sauvegarder
 * \sa getDepot , updateRemarqueHonoraires .
*/
bool ComptaMoteurBase::saveDepotEspecesToBase(Depots* depot)
{ m_messageErreur = "";
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  QSqlCursor cur("depots", TRUE, m_DataBase);
  if ( !cur.canInsert ())
   { m_DataBase->close();
     m_messageErreur = TR("ComptaMoteurBase::saveDepotEspecesToBase : Impossible d'ajouter le dépôt dans la base de données : canInsert()\n\n");
     m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
     return FALSE;
   }
  //............... si OK on recupere le buffer et on le charge avec les données .........
  QSqlRecord *buffer = cur.primeInsert();
  if (buffer)
  { buffer->setValue( "date",        depot->getDateValid().toString("yyyy-MM-dd") );
    buffer->setValue( "periode_deb", m_Filtre_Date_Debut.toString("yyyy-MM-dd") );
    buffer->setValue( "periode_fin", m_Filtre_Date_Fin.toString("yyyy-MM-dd") );
    buffer->setValue( "blob_depot", depot->getListeIdHono() );
    buffer->setValue( "remarque", depot->getRemarque() );
//    buffer->setValue( "type_depot", titre);
    buffer->setValue( "id_usr", QString::number(m_UsrPk));
    buffer->setValue( "id_compte", QString::number( depot->getIdCompte() ));
    if (!cur.insert())
     { m_DataBase->close();
       m_messageErreur = TR("ComptaMoteurBase::saveDepotEspecesToBase : Impossible d'ajouter le dépôt dans la base de données : insert\n\n");
       m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
       return FALSE;
     }

  } // Fin if buffer
  // Fermer la base
  m_DataBase->close();
  return TRUE;
}



/*! \brief Sauvegarde l'honoraire dans la base de données.
 * \return Retourne un booléen qui reflète une éventuelle erreur qui serait survenue surant l'opération. \em TRUE si aucune erreur n'est survenue, sinon \em FALSE.
*/
bool ComptaMoteurBase::saveHonorairesToBase(Honoraires* pHono)
{ QString table = "honoraires";
  m_messageErreur = "";
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Créé un curseur pour insérer l'honoraire dans la base
  QSqlCursor cur(table, TRUE, m_DataBase);
  if ( !cur.canInsert ())
    {	m_DataBase->close();
	m_messageErreur = TR("Impossible d'insérer de nouveaux honoraires dans la base de données.");
	return FALSE;
    }

  // si OK on recupere le buffer et on le charge avec les données
  QSqlRecord *buffer = cur.primeInsert();
  if (buffer)
  { // Reformater la date ++++
    QDate dt;
    dt = dt.fromString( dateForSQL(pHono->m_Date) , Qt::ISODate);
    buffer->setValue( "patient", pHono->getPatient() );
    buffer->setValue( "praticien", pHono->getPraticien() );
    buffer->setValue( "date", dt.toString( "yyyy-MM-dd" ) );
    buffer->setValue( "acte", pHono->getActesForBase() );
    buffer->setValue( "remarque", pHono->getRemarque() );
    buffer->setValue( "esp", pHono->getEsp() );
    buffer->setValue( "chq", pHono->getChq() );
    buffer->setValue( "cb", pHono->getCB() );
    buffer->setValue( "daf", pHono->getDAF() );
    buffer->setValue( "autre", pHono->getAutre() );
    buffer->setValue( "cmu", pHono->getCMU() );
	buffer->setValue( "vir", pHono->getVirement() );
    buffer->setValue( "id_usr", pHono->getIdUsr() );
    buffer->setValue( "GUID", pHono->getGUID() );
    buffer->setValue( "id_drtux_usr", pHono->getDrTuxUsr() );
    buffer->setValue( "valide", pHono->getValidite() );
    buffer->setValue( "tracabilite", pHono->getTracabilite() );

    if (!cur.insert())
     {	m_DataBase->close();
 	m_messageErreur = TR("Impossible d'insérer l'honoraire dans la base de données : \n\n") + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
     }
  } // Fin if buffer

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}


/*! \brief Sauvegarde le compte bancaire dans la base.
 * \return Retourne un booléen qui reflète une éventuelle erreur qui serait survenue surant l'opération. \em TRUE si aucune erreur n'est survenue, sinon \em FALSE.
*/
bool ComptaMoteurBase::saveCptToBase(Comptes* pCpt)
{ QString table = "comptes_bancaires";
  m_messageErreur = "";
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Créé un curseur pour insérer l'honoraire dans la base
  QSqlCursor cur(table, TRUE, m_DataBase);
  if ( !cur.canInsert ())
    {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::saveCptToBase : Impossible d'insérer le nouveau compte bancaire dans la base de données, canInsert().");
	return FALSE;
    }

  // si OK on recupere le buffer et on le charge avec les données
  QSqlRecord *buffer = cur.primeInsert();
  if (buffer)
  { buffer->setValue( "id_usr", pCpt->getIdUsr() );
    buffer->setValue( "nom_banque", pCpt->getNomBanque() );
    buffer->setValue( "libelle", pCpt->getLibelle() );
    buffer->setValue( "titulaire", pCpt->getTitulaire() );
    buffer->setValue( "rib_code_banque", pCpt->getRibBanque() );
    buffer->setValue( "rib_code_guichet", pCpt->getRibGuichet() );
    buffer->setValue( "rib_numcompte", pCpt->getRibCompte() );
    buffer->setValue( "rib_cle", pCpt->getRibCle() );
    buffer->setValue( "solde_initial", pCpt->getSolde() );
    buffer->setValue( "remarque", pCpt->getRemarque() );

    if (!cur.insert())
     {	m_DataBase->close();
 	m_messageErreur = TR("ComptaMoteurBase::saveCptToBase : Impossible d'insérer le nouveau compte bancaire dans la base de données : insert()\n\n") + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
     }
  } // Fin if buffer

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}



/*! \brief Sauvegarde le mouvement dans la base.
 * \return Retourne un booléen qui reflète une éventuelle erreur qui serait survenue surant l'opération. \em TRUE si aucune erreur n'est survenue, sinon \em FALSE.
*/
bool ComptaMoteurBase::saveMvtToBase(Mouvements* pMvt)
{ QString table = "mouvements";
  m_messageErreur = "";
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Créé un curseur pour insérer l'honoraire dans la base
  QSqlCursor cur(table, TRUE, m_DataBase);
  if ( !cur.canInsert ())
    {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::saveMvtToBase : Impossible d'insérer de nouveaux mouvements dans la base de données, canInsert().");
	return FALSE;
    }

  // si OK on recupere le buffer et on le charge avec les données
  QSqlRecord *buffer = cur.primeInsert();
  if (buffer)
  { buffer->setValue( "id_mvt_dispo", pMvt->getIdParent() );
    buffer->setValue( "id_compte", pMvt->getIdCompte() );
    buffer->setValue( "id_usr", pMvt->getIdUsr() );
    buffer->setValue( "type", pMvt->getType() );
    buffer->setValue( "libelle", pMvt->getNom() );
    buffer->setValue( "date", pMvt->getDate().toString("yyyy-MM-dd") );
    buffer->setValue( "date_valeur", pMvt->getDateValeur().toString("yyyy-MM-dd") );
    buffer->setValue( "remarque", pMvt->getRemarque() );
    buffer->setValue( "montant", pMvt->getTotal() );

    if (!cur.insert())
     {	m_DataBase->close();
 	m_messageErreur = TR("ComptaMoteurBase::saveMvtToBase : Impossible d'insérer le mouvement dans la base de données : insert()\n\n") + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
     }
  } // Fin if buffer

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}


/*! \brief Sauvegarde l'immobilisation dans la base.
 * \return Retourne un booléen qui reflète une éventuelle erreur qui serait survenue surant l'opération. \em TRUE si aucune erreur n'est survenue, sinon \em FALSE.
*/
bool ComptaMoteurBase::saveImmobToBase(Immobilisations* pImm)
{ QString table = "immobilisations";
  m_messageErreur = "";
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Créé un curseur pour insérer l'honoraire dans la base
  QSqlCursor cur(table, TRUE, m_DataBase);
  if ( !cur.canInsert ())
    {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::saveImmobToBase : Impossible d'insérer de nouveaux mouvements dans la base de données, canInsert().");
	return FALSE;
    }

  // si OK on recupere le buffer et on le charge avec les données
  QSqlRecord *buffer = cur.primeInsert();
  if (buffer)
  { buffer->setValue( "id_immob", pImm->getId() );
    buffer->setValue( "id_compte", pImm->getIdCompte() );
    buffer->setValue( "id_usr", pImm->getIdUsr() );
    buffer->setValue( "libelle", pImm->getNom() );
    buffer->setValue( "date_service", pImm->getDateService().toString("yyyy-MM-dd") );
    buffer->setValue( "duree", pImm->getDuree() );
    buffer->setValue( "mode", pImm->getMode() );
    buffer->setValue( "valeur", pImm->getValeur() );
    buffer->setValue( "valeur_residuelle", pImm->getResiduel() );
    buffer->setValue( "resultat", pImm->getResultat() );
    buffer->setValue( "remarque", pImm->getRemarque() );
    buffer->setValue( "mouvements", pImm->getMouvements() );

    if (!cur.insert())
     {	m_DataBase->close();
 	m_messageErreur = TR("ComptaMoteurBase::saveImmobToBase : Impossible d'insérer le mouvement dans la base de données : insert()\n\n") + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
     }
  } // Fin if buffer

  // Récupérer l'id de l'immob que l'on vient de sauvegarder
  QString f = "id_usr="+QString::number(m_UsrPk);
  f += " AND date_service='"+pImm->getDateService().toString("yyyy-MM-dd")+"'";
  f += " AND libelle='"+pImm->getNom()+"'";
  f += " AND valeur='"+QString::number(pImm->getValeur())+"'";
  f += " AND valeur_residuelle='"+QString::number(pImm->getResiduel())+"'";
  f += " AND duree='"+ QString::number(pImm->getDuree()) + "'";
  f += " AND id_compte='"+QString::number(pImm->getIdCompte()) + "'";
  int id = -1;
  cur = QSqlCursor(table, TRUE, m_DataBase);
  if ( !cur.select(f))
    {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::saveImmobToBase : Impossible de récupérer l'id, Select().");
	return FALSE;
    }
  else if (cur.next()) id = cur.value("id_immob").toInt();

  // Sauvegarder les mouvements un à un
  MVTS_LISTE mvts;
  pImm->getMouvements( mvts );
  MVTS_LISTE::iterator it;
  bool ok = TRUE;
  for (it = mvts.begin(); it!=mvts.end(); ++it)
  { (*it).setIdUsr( m_UsrPk );
    (*it).setIdImmob( id );
    if (!saveMvtToBase( &(*it) )) { ok=FALSE; break;}
  }

  // Fermer la base
  m_DataBase->close();
  if (!ok) return FALSE;
  return TRUE;
}


/*! \brief Sauvegarde dans la base de données comptabilité les configurations utilisateurs (après vérification des doublons).
 * \param pUser : Paramètres utilisateurs à sauvegarder.
 * \return Retourne un booléen qui reflète une éventuelle erreur qui serait survenue surant l'opération. \em TRUE si aucune erreur n'est survenue, sinon \em FALSE.
 * \sa Utilisateurs.
*/
bool ComptaMoteurBase::saveUserToBase(Utilisateurs* pUser)
{ int dble = -1;
  if (checkDoubleUser(pUser->getDrTuxUsrPk(), dble))
  if (dble != -1)
   {	m_messageErreur = TR("Un utilisateur est déjà lié à cet utilisateur de MedinTux.");
	qWarning("Attention utilisateur en double dans la base");
	return FALSE;
   }

  QString table = "utilisateurs";
  m_messageErreur = "";
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Créé un curseur pour insérer l'honoraire dans la base
  QSqlCursor cur(table, TRUE, m_DataBase);
  if ( !cur.canInsert ())
    {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::saveUserToBase : Impossible d'insérer un nouvel utilisateur dans la base de données.");
	return FALSE;
    }

  // si OK on recupere le buffer et on le charge avec les données
  QSqlRecord *buffer = cur.primeInsert();
if (buffer)
 {
  buffer->setValue( "nom_usr", pUser->getNom() );
  buffer->setValue( "login", pUser->getLogin() );
  buffer->setValue( "password", pUser->getPassword() );
  buffer->setValue( "id_drtux_usr", pUser->getDrTuxUsrPk() );

  if (!cur.insert())
   {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::saveUserToBase : Impossible d'insérer le nouvel utilisateur dans la base de données : insert\n\n") + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }

 }
  // Fermer la base
  m_DataBase->close();
  return TRUE;
}

/*! \brief Sauvegarde dans la base de données comptabilité l'acte passé en paramètre (attention les doublons ne sont pas gérés).
 * \param pActe : Acte à sauvegarder.
 * \return Retourne un booléen qui reflète une éventuelle erreur qui serait survenue surant l'opération. \em TRUE si aucune erreur n'est survenue, sinon \em FALSE.
 * \todo gérer les doublons
*/
bool ComptaMoteurBase::saveActe(Actes* pActe)
{ QString table = "actes_disponibles";
  m_messageErreur = "";
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Créé un curseur pour insérer l'honoraire dans la base
  QSqlCursor cur(table, TRUE, m_DataBase);
  if ( !cur.canInsert ())
    {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::saveActe : Impossible d'insérer un nouvel utilisateur dans la base de données : canInsert.");
	return FALSE;
    }

  // si OK on recupere le buffer et on le charge avec les données
  QSqlRecord *buffer = cur.primeInsert();
if (buffer)
 {
  buffer->setValue( "nom_acte", pActe->getNom() );
  buffer->setValue( "desc_acte", pActe->getDesc() );
  buffer->setValue( "type", pActe->getType() );
  buffer->setValue( "montant_total", QString::number(pActe->getTotal()) );
  buffer->setValue( "montant_tiers", QString::number(pActe->getTiers()) );

  if (!cur.insert())
   {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::saveActe : Impossible d'insérer l'honoraire dans la base de données : Insert : \n\n") + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }

 }
  // Fermer la base
  m_DataBase->close();
  return TRUE;
}


/*! \brief Efface un enregistrement "honoraire" pointé par le QSqlRecord de la base de données.
 * \param h : QSqlRecord de l'enregistrement à effacer.
 * \return booléen qui reflète une éventuelle erreur qui serait survenue surant l'opération. TRUE = pas d'erreur.
*  \todo Mettre l'honoraire à effacer en mémoire dans une table poubelle et créer les fonctions de récupération.
*/
bool ComptaMoteurBase::deleteHonoraire(QSqlRecord* h)
{ int id;
  id = h->value("id_hono").toInt();
  return deleteHonoraire(id);
}

/*! \brief Efface un enregistrement "honoraire" pointé par le QSqlRecord de la base de données.
 * \param id : id de l'enregistrement à effacer.
 * \return booléen qui reflète une éventuelle erreur qui serait survenue surant l'opération. TRUE = pas d'erreur.
*  \todo Mettre l'honoraire à effacer en mémoire dans une table poubelle et créer les fonctions de récupération.
*/
bool ComptaMoteurBase::deleteHonoraire(int id)
{ QString requete ("DELETE FROM `honoraires` WHERE `honoraires`.`id_hono` = \'");
  requete += QString::number(id) +"\'";
  requete += "LIMIT 1 ; ";

  m_messageErreur = "";
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Execute la requete
  QSqlQuery query(requete , m_DataBase );
  if (query.isValid())
   if (!query.exec())
     {	m_DataBase->close();
	m_messageErreur = TR("Impossible de supprimer l'honoraire de la base de données :");
	m_messageErreur += "\n\n" + query.lastError().driverText() + "\n\n" + query.lastError().databaseText() + ".";
	return FALSE;
      }
  return TRUE;
}



/*! \brief Efface un enregistrement "mouvement" pointé par son Id de la base de données.
 * \param id : id de l'enregistrement à effacer.
 * \return booléen qui reflète une éventuelle erreur qui serait survenue surant l'opération. TRUE = pas d'erreur.
*/
bool ComptaMoteurBase::deleteMvt(int id)
{ QString requete ("DELETE FROM `mouvements` WHERE `mouvements`.`id_mouvement` = \'");
  requete += QString::number(id) +"\'";
  requete += "LIMIT 1 ; ";

  m_messageErreur = "";
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Execute la requete
  QSqlQuery query(requete , m_DataBase );
  if (query.isValid())
   if (!query.exec())
     {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::deleteMvt : Impossible de supprimer le mouvement de la base de données :");
	m_messageErreur += "\n\n" + query.lastError().driverText() + "\n\n" + query.lastError().databaseText() + ".";
	return FALSE;
      }
  return TRUE;
}


/*! \brief Efface un enregistrement "actes_disponibles" pointé par son Id de la base de données.
 * \param id : id de l'enregistrement à effacer.
 * \return booléen qui reflète une éventuelle erreur qui serait survenue surant l'opération. TRUE = pas d'erreur.
*/
bool ComptaMoteurBase::deleteActe(int id)
{ QString requete ("DELETE FROM `actes_disponibles` WHERE `actes_disponibles`.`id_acte_dispo` = \'");
  requete += QString::number(id) +"\'";
  requete += "LIMIT 1 ; ";

  m_messageErreur = "";
  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Execute la requete
  QSqlQuery query(requete , m_DataBase );
  if (query.isValid())
   if (!query.exec())
     {	m_DataBase->close();
	m_messageErreur = TR("ComptaMoteurBase::deleteActe : Impossible de supprimer l'acte de la base de données : exec");
	m_messageErreur += "\n\n" + query.lastError().driverText() + "\n\n" + query.lastError().databaseText() + ".";
	return FALSE;
      }
  return TRUE;
}


/*! \brief Vérifie si existe un du pour un patient lié à un utilisateur de la comptabilité.
  * \return La fonction renvoie un booléen : TRUE si aucun problème à l'exécution de la fonction, FALSE cas contraire.
*/
bool ComptaMoteurBase::checkDus( Utilisateurs* user , const char* GUID, int& total_Du )
{ // Prépare la requête SQL et prépare la liste
  QString req = "`id_usr`="+QString::number(user->getId()) + " AND `du`>0 ";
  req += "AND `GUID`=\"";
  req += GUID;
  req += "\"";
  total_Du = 0;

 // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  QSqlCursor cur("honoraires", TRUE, m_DataBase);
  if ( !cur.select(req) )
   { m_DataBase->close();
     m_messageErreur = TR("Impossible de vérifier l'existence d'un dû depuis la base de données :");
     m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
     return FALSE;
   }
  else
   while ( cur.next() )
    {	total_Du += cur.value( "du" ).toInt();  }

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}


/*! \brief Vérifier qu'un honoraire n'est pas déjà enregistré le même jour au même noms.
 * \return -1 si l'honoraire n'est pas trouvé, sinon retourne son id_hono
 * \todo    A RECODER --- FONCTIONNE PAS
*/
int ComptaMoteurBase::checkDoubleHono(Honoraires *pHono)
{ QString requete ("SELECT `patient` , `date`, `id_hono` FROM `honoraires` WHERE `patient` = \'");
  requete += pHono->m_Patient+"\' AND `date` = \'";
  requete += pHono->m_Date + "\' ; ";
  int tmp = 0;

  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

  // Execute la requete
  QSqlQuery query(requete , m_DataBase );
  //Récupère les données de la requête (la liste des actes pour le combobox)
  if (query.isActive())
      { tmp = query.value(2).toInt();  }
  else { tmp = -1; }

  m_DataBase->close();
  return tmp;
}


//--------------------------- Utf8_Query ---------------------------
QString ComptaMoteurBase::Utf8_Query(QSqlQuery &cur, int field)
{QCString     cString  = cur.value(field).toCString ();
 char *ptr             =  cString.data();
 if (CGestIni::IsUtf8( ptr ))
    {return QString::fromUtf8 ( ptr );
    }
 else
    {return QString(ptr) ;
    }
}



//--------------------------- dateForSQL ---------------------------
/*! \brief Transforme une date à la française dd-MM-yyyy en date pour SQL yyyy-MM-dd.
 * \return Retourne la date passée en entrée sous le format dd-MM-yyyy sous forme yyyy-MM-dd
*/
QString ComptaMoteurBase::dateForSQL(const QString &date)
{int end = date.length();
 int   i = -1;
 QString str;
 while (++i < end) { if (  (date[i]>='0' && date[i]<='9') ) str += date[i]; }
 if (str.length() ==6 )  str.insert ( 4, "20" ); // date forme 011207 --> 01122007
 if (str.length() !=8 )  return TR("format invalide");

 return str.mid(4,4)+"-"+str.mid(2,2)+"-"+str.mid(0,2);
}



//--------------------------------------  PassWordEncode ---------------------------------
/*! \brief Copié de DrTux. Encode un mot de passe :
 * Algo minimal pour l'instant
 * chaque octet tourne de sa position dans la chaine + 1, et Xor final avec le mot de passe en clair puis converti en hexa
 * resultat final en hexa;
*/
int ComptaMoteurBase::PassWordEncode(QString &pass)
{char str[34];
 INT16    val;
 int len         = pass.length();

 if (len>32)       return ERR_PASS_MAX;
 if (len==0)       return ERR_PASS_ZERO;
 strcpy(str , pass);
 pass = "";
 for (int i=0; i<len; ++i)
     { val   =  str[i];
       val   =  (val << ((i+1)&0x000F) )^val;
      pass  += UINTtoHex(val);
     }
 return ERR_PASS_OK;
}

//-----------------------------------  PassWordDecode ----------------------------------
/*! \brief Copié de DrTux. Décode un mot de passe :
 * Algo minimal pour l'instant
 * chaque octet tourne de sa position dans la chaine + 1, et Xor final avec le mot de passe en clair puis converti en hexa résultat final en hexa;
*/
int ComptaMoteurBase::PassWordDecode(const char* str_to_decode, const char* str_pass)
{char     buf[16];
 UINT16   val;
 if (str_to_decode == 0 || str_pass == 0) return ERR_PASS_MAX;
 int len_to_decode      = strlen(str_to_decode);
 if (len_to_decode==0)             return ERR_PASS_ZERO;
 int len_pass           = strlen(str_pass);
 if (len_pass != len_to_decode/4)  return ERR_PASS_MAX;
 QString result;
 int end = len_to_decode/4;
 buf[4 ] = 0;
 for (int i=0; i<end; ++i)
     {memcpy(buf, str_to_decode + i*4, 4);  // recuperer les 4 caractere hexa
      val = HexToUINT(buf);                 // les convertir en UINT16      val = val ^ str_pass[i];              // xor avec le mot de passe
      val = val >> ((i+1)&0x000F);          // rotation inverse maxi de 8 et on devrait avoir le code ASCII
      if (val != str_pass[i])      return ERR_PASS_MAX;    // erreur si pas egal
     }
 return ERR_PASS_OK;
}


//-----------------------------------------------------  UINTtoHex --------------------------------------------------------
QString ComptaMoteurBase::UINTtoHex( UINT16 val)
{char hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};   // là on peut mettre une table Bidon
 QString res="";
 res += hex[(val>>12)& 0x000F];
 res += hex[(val>>8) & 0x000F];
 res += hex[(val>>4) & 0x000F];
 res += hex[val&0x000F];
 return res;
}

//-----------------------------------------------------  HexToUINT --------------------------------------------------------
UINT16 ComptaMoteurBase::HexToUINT( const char *str)
{int i   = strlen(str);
 if (i==0) return 0;
 UINT16 val = 0;
 int    dec = 0;
 while (i>0)
   {--i;
    unsigned char car = str[i] | 32;     // mettre en minuscules
    if      (car>='a'&& car<= 'f')  val  += (car-'W')<<dec;  //a (97) - W (87) --> 10
    else                            val  += (car-'0')<<dec;  //0 (48) - 0 (48) --> 0
    dec += 4;
   }
 return val;
}



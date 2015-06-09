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
  m_Port        = "";
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
 * @param port
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
{ initBase   ( driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
               DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
               user,          // = "root"
               password,      // = ""
               hostname,
               port,
               confFile,
               baseLabel,
               errMess);
}

/** \brief Destructeur de la classe et de la base de comptabilité
 *
 * Ferme la base de donnée de comptabilité.
 * \sa m_DataBase, initBase
 */
ComptaMoteurBase::~ComptaMoteurBase()
{if (m_DataBase)
    {if (m_DataBase->isOpen()) m_DataBase->close();
     QSqlDatabase::removeDatabase (m_BaseLabel);
     //delete m_DataBase;
     //m_DataBase = 0;
    }
}


//-----------------------------------------------------  initBase -------------------------------------------
/** \brief initialise la base de données de la comptabilité
 *
 * @param driver
 * @param DataBaseName
 * @param user
 * @param password
 * @param hostname
 * @param port
 * @param confFile
 * @param baseLabel
 * @param errMess
 */
void ComptaMoteurBase::initBase   (
                                   const QString& driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
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
 * @param port
 * @param confFile
 * @param baseLabel
 * @param errMess
 */
//-----------------------------------------------------  BaseConnect -------------------------------------------
QSqlDatabase*
ComptaMoteurBase::BaseConnect(
                              const QString& driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                              const QString& DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                              const QString& user,          // = "root"
                              const QString& password,      // = ""
                              const QString& hostname,
                              const QString& port,
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
        defaultDB->setPassword( password );
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



/*! \brief Récupère la liste des actes disponibles dans la base pour construire le contenu du combobox de la fenêtre de plugins (par exemple).
 * \param al : Liste des actes disponibles dans la base de données
 * \return TRUE : pas d'erreur, FALSE = erreur. Le message d'erreur est stocké dans m_messageErreur.
*/
bool ComptaMoteurBase::getActesForCombobox(ACTES_LISTE& al)
{ al.clear();

  // Teste et Ouvre la base de données
  if (!connexion()) return FALSE;

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
	  acte->setDesc 		(cur.value( "desc_acte" ).toString() ) ;
	  acte->setType 		(cur.value( "type" ).toString() ) ;
	  acte->setTotal 		(cur.value( "montant_total" ).toDouble() ) ;
	  acte->setTiers	 	(cur.value( "montant_tiers" ).toDouble() ) ;
	  acte->setId 		(cur.value( "id_acte_dispo" ).toInt() ) ;
	  al.append( *acte );
	  delete acte;
	}
      } // Fin while (requête finie)
    }

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}


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
	m_messageErreur = TR("Impossible de récupérer l'identifiant utisateur :");
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
	m_messageErreur = TR("Impossible de récupérer l'identifiant utisateur :");
	m_messageErreur += "\n\n" + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}


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




/*! \brief Récupère la liste des actes disponibles dans la base pour construire le contenu du combobox de la fenêtre de plugins (par exemple).
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
  // Reformater la date ++++
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
  buffer->setValue( "id_usr", pHono->getIdUsr() );
  buffer->setValue( "GUID", pHono->getGUID() );
  buffer->setValue( "id_drtux_usr", pHono->getDrTuxUsr() );

  if (!cur.insert())
   {	m_DataBase->close();
	m_messageErreur = TR("Impossible d'insérer l'honoraire dans la base de données : \n\n") + cur.lastError().driverText() + "\n\n" + cur.lastError().databaseText() + ".";
	return FALSE;
   }

  // Fermer la base
  m_DataBase->close();
  return TRUE;
}

bool ComptaMoteurBase::updateHonoraires(Honoraires * /*hono*/)
 {return FALSE;}


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
 * \todo  NE FONCTIONNE PAS CORRECTEMENT////
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



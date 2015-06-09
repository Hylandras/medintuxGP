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
  m_Port        = "3306";
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
 * @param port
 * @param confFile
 * @param baseLabel
 * @param errMess
 */
//-----------------------------------------------------  BaseConnect -------------------------------------------
QSqlDatabase* ComptaMoteurBase::BaseConnect(const QString& driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
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
	  b->setCaption(TR("ERREUR DU PLUG-INS"));
	  b->setText(TR("Impossible de se connecter à la base de données.<br>Vérifiez que vous avez installer le gestionnaire de base de données MySQL pour Qt."));
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
            qstr += TR("Impossible d'ouvrir la base de données : ") + QString(driver) + "  " + QString(DataBaseName) + "<br>" + defaultDB->lastError().driverText() + "<br>" + defaultDB->lastError().databaseText() + "<br><br>" + TR("Référez-vous à la documentation du plugins");
            if (errMess) *errMess = qstr;
            return 0;
           }
        return defaultDB;
    }

bool ComptaMoteurBase::connexion()
{  if (m_DataBase==0)
     {	m_messageErreur = TR("Impossible de se connecter à la base de données. Soit celle-ci n'existe pas, soit les paramètres de connection sont erronés.");
	return FALSE;
     }
  if (m_DataBase->isOpen()==FALSE && m_DataBase->open()==FALSE)
     {	m_messageErreur = TR("Impossible d'ouvrir la base de données.");
	return FALSE;
     }
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

/*! \brief Vérifie si il existe un du pour un patient lié à un utilisateur de drtux.
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
     m_messageErreur = TR("Impossible de récupérer le dépôt depuis la base de données :");
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



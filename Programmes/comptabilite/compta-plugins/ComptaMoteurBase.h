 /********************************* ComptaMoteurBase.h ************************************
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
/*! \file ComptaMoteurBase.h
    \brief Fonctions de gestion de la base de données.

     #include "ComptaMoteurBase.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/
/*! \class ComptaMoteurBase
    \brief Fonctions de gestion de la base de données.

     #include "ComptaMoteurBase.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#ifndef COMPTAMOTEURBASE_H
#define COMPTAMOTEURBASE_H

#include <qstringlist.h>
#include <qmessagebox.h>
#include <qsqldatabase.h>
#include <qstring.h>
#include <qsqlcursor.h>
#include <qsqlrecord.h>
#include <qdatetime.h>

#include "../comptabilite/Listes.h"
#include "../comptabilite/Defines.h"
#include "../../../MedinTuxTools/CGestIni.h"



#define  ERR_PASS_MAX       1
#define  ERR_PASS_ZERO      2
#define  ERR_PASS_OK        3

class Actes;
class Honoraires;
class Paiements;

/**
	@author Eric Maeker <eric.maeker@free.fr>
*/


class ComptaMoteurBase
{
public:
    ComptaMoteurBase();

   ComptaMoteurBase(       const QString& driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                           const QString& DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                           const QString& user,          // = "root"
                           const QString& password,      // = ""
                           const QString& hostname,
                           const QString& port,
                           const char* confFile,
                           const char* baseLabel,
                           QString*    errMess);

    ~ComptaMoteurBase();

                //////////////////////////////////////////////////
                //              INITIALISATIOIN                 //
                //////////////////////////////////////////////////


  void initBase   (const QString& driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                   const QString& DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                   const QString& user,          // = "root"
                   const QString& password,      // = ""
                   const QString& hostname,
                   const QString& port,
                   const char* confFile,
                   const char* baseLabel,
                   QString*    errMess);

  QSqlDatabase*  BaseConnect(const QString& driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                             const QString& DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                             const QString& user,          // = "root"
                             const QString& pasword,       // = ""
                             const QString& hostname,      // = "localhost"
                             const QString& port,      // = "localhost"
                             QString*    errMess ,
                             QString     baseLabel);

   bool		 connexion();

		//////////////////////////////////////////////////
		//		RECUPERATION			//
		//////////////////////////////////////////////////
	bool	 	getActesForCombobox		(ACTES_LISTE& al);

	bool		checkDus			( Utilisateurs* user , const char* GUID, int& total_Du );
	int		checkDoubleHono			(Honoraires *pHono);

	bool		getIdUsrFromDrTuxPk		(int &idDrTuxToModify);
	bool		getUsers			(int id_baseCompta, Utilisateurs& resultat);


		//////////////////////////////////////////////////
		//		SAUVEGARDE			//
		//////////////////////////////////////////////////
	bool		saveHonorairesToBase	(Honoraires* pHono);

	QString 	Utf8_Query		(QSqlQuery &cur, int field);
	QString		dateForSQL		(const QString &date);

	bool		updateHonoraires	(Honoraires *hono);


		//////////////////////////////////////////
		//		DONNEES			//
		//////////////////////////////////////////
public:
	QSqlDatabase*	m_DataBase;	/*!< pointeur vers la base de donnée de comptabilité */
	QString		m_DriverName;	/*!< paramètres pour la base de donnée de comptabilité */
	QString		m_BaseName;	/*!< paramètres pour la base de donnée de comptabilité */
	QString		m_UserName;	/*!< paramètres pour la base de donnée de comptabilité */
	QString		m_PassWord;	/*!< paramètres pour la base de donnée de comptabilité */
	QString		m_HostName;	/*!< paramètres pour la base de donnée de comptabilité */
        QString         m_Port;         /*!< paramètres pour la base de donnée de comptabilité */



	QStringList	m_DefautActes;	/*!< paramètres par défaut des actes disponibles pour les combobox */

	QString		m_messageErreur;	/*!< Message d'erreur retouné par les fonctions de la base de données. */

private:
   QString      m_BaseLabel;
   int          m_XmlDepth;
   int          m_ConvertRun;

		//////////////////////////////////////////
		//		DONNEES PRIVÉES		//
		//////////////////////////////////////////
   int		m_NbPaiement;    /*!< Obsolete */
   double	m_MontantPaiement; /*!< Obsolete */

};

#endif

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

#include <qfile.h>
#include <qmessagebox.h>

#include "Listes.h"
#include "Tools.h"
#include "Defines.h"
#include "../../MedinTuxTools/CGestIni.h"


#define  ERR_PASS_MAX       1
#define  ERR_PASS_ZERO      2
#define  ERR_PASS_OK        3

class Actes;
class Honoraires;
class Paiements;

/**
	@author Eric Maeker <eric.maeker@free.fr>
*/


class ComptaMoteurBase : public Tools
{
public:

   enum ComptaBaseBooleens { ForBilanAnnuel = false,
                             ForGrandLivre = true
                           };

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
		//		INITIALISATIOIN			//
		//////////////////////////////////////////////////


  void setParams  ( Utilisateurs* pUtilisateur);
  void setVerboseMode(bool to);

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

	void		readHonoraire			(QSqlCursor & SQL_Curseur, Honoraires* hono);
	void 		readMouvement			(QSqlCursor & SQL_Curseur, Mouvements* mvt);
	void 		readImmobilisation		(QSqlCursor & SQL_Curseur, Immobilisations* immo);


	bool	 	getActesForCombobox		(ACTES_LISTE& al);
	QStringList 	getActesDisponiblesDefauts	();

	bool		getHonoraire			( int id , Honoraires* hono );
	bool	 	getHonorairesForTable		(HONO_LISTE* hl);
	bool		getHonorairesForDepot		(HONO_LISTE* hl);
	bool		getHonorairesFromBlobDepot	( const char* listeHono , HONO_LISTE& lHono );
	bool		getHonoraires_DAF_ForGestion	(HONO_LISTE* hl);
	bool		getHonoraires_CMU_ForGestion	(HONO_LISTE* hl);
	bool		getHonoraires_MontantTotal	(double& total, double& esp,
							 bool& tousHonoValide, bool pourBilanAnnuel=FALSE,
							 int annee=0);

	bool		checkDus			( Utilisateurs* user , const char* GUID, int& total_Du );
	int		checkDoubleHono			(Honoraires *pHono);
	bool		checkDoubleUser			(int idDrTux, int &retour);

	bool		getMvts				(MVTS_LISTE& ml, int id_immobilisation = -1);
	bool		getMvtsDispo			(MVTS_LISTE& ml);
	bool		getMvtsForReleve		(MVTS_LISTE& ml);
	bool		getMvts_MontantTotal		(double& recettes, double& depenses, double& esp,
							 bool pourBilanAnnuel=FALSE, int annee=0);

	bool		getImmobilisations		(IMMO_LISTE* il);

	bool		getUsersForList			(UTILISATEURS_LISTE * ul);
	bool		getDrTuxUsers			(UTILISATEURS_LISTE * ul);
	bool		getIdUsrFromDrTuxPk		(int &idDrTuxToModify);
	bool		getUsers			(int id_baseCompta, Utilisateurs& resultat);

	bool		getComptes			(COMPTES_LISTE& cl);

	bool		getDepotsForList		(DEPOTS_LISTE& retour);
	bool		getDepot			(int depot_id, Depots &retour);
	bool		getDepots_MontantTotal		(double& total, double& esp,
							 bool pourBilanAnnuel, int annee);


	bool		getVersion			(QString& resultat);



		//////////////////////////////////////////////////
		//		VALIDATION			//
		//////////////////////////////////////////////////

	bool	valideHonoraires			(HONO_LISTE* pHono_Liste,
							 Utilisateurs* pUtilisateurActuel,
							 int & nbUpdated);
	bool	valideMouvements			(MVTS_LISTE* pMvts_Liste,
							 Utilisateurs* pUtilisateurActuel,
							 int idCpt, int & nbUpdated);

	bool	encaisseDAF				(HONO_LISTE* pHono_Liste,
							 Utilisateurs* pUtilisateurActuel,
							 int & nbUpdated);
	
	bool	encaisseCMU				(HONO_LISTE* pHono_Liste,
							 Utilisateurs* pUtilisateurActuel,
							 int & nbUpdated);



		//////////////////////////////////////////////////
		//		SAUVEGARDE			//
		//////////////////////////////////////////////////
	bool		saveUserToBase		(Utilisateurs* pUser);
	bool		saveHonorairesToBase	(Honoraires* pHono);
	bool		saveActe		(Actes* pActe);
	bool		saveCptToBase		(Comptes* pCpt);
	bool		saveMvtToBase		(Mouvements* pMvt);
	bool		saveDepotToBase		(QDate* dt_depot, HONO_LISTE* hl, const char* rib, int idCpt);
	bool		saveDepotEspecesToBase	(Depots* depot);
	bool		saveImmobToBase		(Immobilisations* pImm);


	bool		deleteHonoraire		(QSqlRecord* h);
	bool		deleteHonoraire		(int id);
	bool		deleteMvt		(int id);
	bool		deleteActe		(int id);

	bool		updateRemarqueHonoraires(Honoraires *hono);
	bool		updateHonoraires	(Honoraires *hono);
	bool		updateMvt		(Mouvements* pMvt);
	bool		updateCpt		(Comptes* pCpt);
	bool		updateActe		(Actes* acte);
	bool		updateImmob		(Immobilisations* pImm);



		//////////////////////////////////////////////////
		//		DIVERS				//
		//////////////////////////////////////////////////
	QString		getFiltre		(const char* table, int moreParams=0);
	QString		getFiltrePeriode	();
	int		getIdListeFromIdCpt	(COMPTES_LISTE& cl, int idCpt);

	bool		getBilanAnnuel		(const char* annee, QString& retour, bool pourGrandLivre=FALSE);
	int		nbSubRubriques		(MVTS_LISTE& mvts, int id, MVTS_LISTE& sRub);

	bool		integrationEspecesRestantes(const char* annee);

	QString 	Utf8_Query		(QSqlQuery &cur, int field);
	QString		dateForSQL		(const QString &date);
	int		PassWordEncode		(QString &pass);
	int		PassWordDecode		(const char* str_to_decode, const char* str_pass);
	static  UINT16  HexToUINT( const char *str);
	static  QString UINTtoHex( UINT16 val);


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
   QString              m_Port;         /*!< paramètres pour la base de donnée de comptabilité */
   QStringList	        m_DefautActes;  /*!< paramètres par défaut des actes disponibles pour les combobox */
   QString		m_messageErreur;/*!< Message d'erreur retouné par les fonctions de la base de données. */

  Utilisateurs* 	m_pUtilisateurActuel;	/*!< Enregistrement complet de l'utilisateur identifié */
  int			m_UsrPk;     		/*!< contient l'id de l'utilisateur courant */
  QDate   		m_Filtre_Date_Debut;	/*!< Variables pour le filtre des requêtes */
  QDate   		m_Filtre_Date_Fin;	/*!< Variables pour le filtre des requêtes */
  QString 		m_Filtre_Type_Paiement;	/*!< Variables pour le filtre des requêtes */
  QString 		m_Filtre_Autre;		/*!< Variables pour le filtre des requêtes */
  bool    		m_Filtre_Change;	/*!< Variables pour le filtre des requêtes */
  bool                  m_Verbose;

private:
   QString      m_BaseLabel;
   int          m_XmlDepth;
   int          m_ConvertRun;

		//////////////////////////////////////////
		//		DONNEES PRIVÉES		//
		//////////////////////////////////////////


};

#endif

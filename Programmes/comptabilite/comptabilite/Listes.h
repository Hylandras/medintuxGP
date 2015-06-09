/********************************* Listes.h ************************************
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
/*! \file Listes.h
    \brief Définition des différentes classes (Actes, Honoraires...) pour la création des listes.

     #include "Listes.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#ifndef LISTES_H
#define LISTES_H
#include <qstring.h>
#include <qdatetime.h>
#include <qvaluelist.h>
#include <qstringlist.h>
#include "Defines.h"
#include "Tools.h"





class Utilisateurs
{
public:
	Utilisateurs() { m_Nom=""; m_Login=""; m_Password=""; m_Id=-1; m_DrTux_UsrPk=-1; }

	~Utilisateurs()  {}


		//////////////////////////////////////////
		//		METHODES		//
		//////////////////////////////////////////

	void		setNom		(const char* n)  { m_Nom        = n; }
	void		setLogin	(const char* n)  { m_Login      = n; }
	void		setPassword	(const char* n)  { m_Password   = n; }
	void		setId		(int n)  	 { m_Id         = n; }
	void		setDrTuxUsrPk	(int n)		 { m_DrTux_UsrPk = n; }

	QString		getNom		()  { return m_Nom; }
	QString		getLogin	()  { return m_Login; }
	QString		getPassword	()  { return m_Password; }
	int		getDrTuxUsrPk	()  { return m_DrTux_UsrPk; }
	int		getId		()  { return m_Id; }
	Utilisateurs*	getUser		()  { return this; }


		//////////////////////////////////////////
		//		DONNEES			//
		//////////////////////////////////////////

public:
	QString	m_Nom;		/*!< Nom de l'utilisateur */
	QString	m_Login;	/*!< Login de l'utilisateur */
	QString	m_Password;	/*!< Mot de passe de l'utilisateur */
	int     m_DrTux_UsrPk;	/*!< Identifiant de l'utilisateur pour la base DrTux */
	int     m_Id;		/*!< Identifiant de l'utilisateur pour la base comptabilité */
};

// Définition d'un alias vers une liste de paiements
typedef QValueList<Utilisateurs> UTILISATEURS_LISTE;



/** \class Actes
 * \brief Classe pour la création de la liste des actes disponibles dans le logiciel.
 * Cette classe ne sert que pour la définition des caractéristiques des actes :
- nom,
- description (courte description de l'acte),
- type (texte définissant le type de l'acte : Consultation, Visite... utilisé pour les recherches),
- montant total,
- montant du tiers (à préciser),
- id (identifiant dans la base, non obligatoire sera calculé par la base de données).

 * Un alias vers une QValueList d'Actes est créé : ACTES_LISTE.

	@author Eric Maeker <eric.maeker@free.fr>
*/
class Actes
{
public:
	Actes() {}

	Actes (	const char*	nom,
	        const char*	desc,
	        const char*	type,
	        double		total,
	        double		tiers,
	        int 		id = 0
	      )

	{
		m_Nom        = nom;
		m_Desc       = desc;
		m_Type       = type;
		m_Total      = total;
		m_Tiers      = tiers;
		m_Id         = id;
	}

	~Actes()  {}


		//////////////////////////////////////////
		//		METHODES		//
		//////////////////////////////////////////

	void		setNom		(const char* n )  { m_Nom = n; }
	void		setDesc		(const char* n )  { m_Desc = n; }
	void		setId		(int n) 	  { m_Id = n; }
	void		setType		(const char* n )  { m_Type = n; }
	void		setTotal	(double n )	  { m_Total = n; }
	void		setTiers	(double n)	  { m_Tiers = n; }


	QString		getNom		()  { return m_Nom; }
	int		getId		()  { return m_Id; }
	QString		getType		()  { return m_Type; }
	QString		getDesc		()  { return m_Desc; }
	double		getTotal	()  { return m_Total; }
	double		getTiers	()  { return m_Tiers; }


		//////////////////////////////////////////
		//		DONNEES			//
		//////////////////////////////////////////

public:
	QString	m_Nom;		/*!< Nom de l'acte pourle combobox. */
	QString	m_Desc;		/*!< Court descriptif pour se repérer. */
	QString m_Type;		/*!< Type d'acte : Consultation, Visite, Bloc... */
	double	m_Total;	/*!< Montant total de l'acte. */
	double	m_Tiers;	/*!< Montant du tiers payant. */
	int	m_Id;		/*!< Identificateur de l'acte dans la base. */
};

// Définition d'un alias vers une liste d'actes
typedef QValueList<Actes> ACTES_LISTE;

/*! \class Paiements
  * \brief Classe définissant un paiement.
  * Un paiement est défini par :
  * - acte et type d'acte
  * - montant des espèces, chèques, CB, DAF, du, autre
  * - emetteur du chèque si nécessaire

  * \sa Acte_Selection , Honoraires , Actes
 */
class Paiements
{
public:
	Paiements() { 	m_Nom=""; m_Type=""; m_Tiers=FALSE;
			m_Esp=0; m_Chq=0; m_CB=0; m_DAF=0; m_Autre=0; m_CMU=0; m_Virement=0;}

	Paiements (const char*	acte,
		  const char*	type,
		  bool		tiers,
		  double	esp,
		  double	chq,
		  double	cb,
		  double	daf,
		  double	autre,
		  double	cmu,
          double	vir
	      )

	{
		m_Nom        = acte;
		m_Type	     = type;
		m_Tiers	     = tiers;
		m_Esp        = esp;
		m_Chq        = chq;
		m_CB	     = cb;
		m_DAF        = daf;
		m_Autre      = autre;
		m_CMU	     = cmu;
		m_Virement   = vir;
	}

	~Paiements()  {}


		//////////////////////////////////////////
		//		METHODES		//
		//////////////////////////////////////////

	void		setNom		(const char* n)  { m_Nom      = n; }
	void		setType		(const char* n)  { m_Type     = n; }
	void		setTiers	(bool n)  	 { m_Tiers    = n; }
	void		setEspeces	(double n)  	 { m_Esp      = n; }
	void		setCheque	(double n)  	 { m_Chq      = n; }
	void		setCB		(double n)  	 { m_CB       = n; }
	void		setDAF		(double n)  	 { m_DAF      = n; }
	void		setAutre	(double n)  	 { m_Autre    = n; }
	void		setCMU		(double n)  	 { m_CMU       = n; }
    void		setVirement	(double n)  	 { m_Virement  = n; }
	void		setEmetteur	(const char* n)	 { m_Emetteur = n; }

	QString		getNom		()  { return m_Nom; }
	QString		getType		()  { return m_Type; }
	bool		getTiers	()  { return m_Tiers; }
	double		getEsp		()  { return m_Esp; }
	double		getChq		()  { return m_Chq; }
	double		getCB		()  { return m_CB; }
	double		getDAF		()  { return m_DAF; }
	double		getAutre	()  { return m_Autre; }
	double		getCMU		()  { return m_CMU; }
    double		getVirement	()  { return m_Virement; }
	QString		getEmetteur	()  { return m_Emetteur; }

	void		remiseAZero	()    { m_Nom=""; m_Type=""; m_Tiers=FALSE;
						m_Esp=0; m_Chq=0; m_CB=0; m_DAF=0;
						m_Autre=0; m_CMU=0; m_Virement=0;
					      }
	double		total		()    { return m_Esp+m_Chq+m_CB+m_DAF+m_Autre+m_CMU+m_Virement; }
	QString		toString	();

		//////////////////////////////////////////
		//		DONNEES			//
		//////////////////////////////////////////

public:
	QString	m_Nom;		/*!< Nom de l'acte payé */
	QString	m_Type;		/*!< Type de l'acte payé */
	QString m_Emetteur;	/*!< Emetteur du chèque (si paiement par chèque) */

	bool	m_Tiers;	/*!< TRUE = par tiers payant, FALSE = pas de tiers payant */

	double  m_Esp;		/*!< Montant des espèces */
	double  m_Chq;		/*!< Montant du chèque */
	double	m_CB;		/*!< Montant de la CB */
	double	m_DAF;		/*!< Montant du DAF */
	double	m_Autre;	/*!< Montant des autres types de paiements */
	double	m_CMU;		/*!< Montant restant du */
    double	m_Virement;
};

// Définition d'un alias vers une liste de paiements
typedef QValueList<Paiements> PAIEMENTS_LISTE;


/** \class Honoraires
 * \brief Classe pour la création de la liste des honoraires.
 * Cette classe ne sert que pour la définition des caractéristiques des honoraires (nom patient et praticien, date, acte effectué, especes, chèques, cb, daf, autre....). Un alias vers une QValueList d'honoraires est créé : HONO_LISTE. \n \n
 ATTENTION les dates doivent passées sous le format : "dd-MM-yyyy". Pour une meilleure compatibilité, la fonction setDate(QDate) gère les dates des honoraires.

* Pour le codage du champs m_Remarque cf le Macros de Defines.h .

	@author Eric Maeker <eric.maeker@free.fr>
*/
class Honoraires
{
public:
	/*! \brief Crée et initialise une classe Honoraires */
	Honoraires() {	m_Patient=""; m_GUID = ""; m_Praticien=""; m_Date=""; m_Actes=""; m_Remarque="";
			m_IdUsr=0; m_Cheque=0; m_CB=0; m_Especes=0; m_DAF=0; m_Autre=0; m_CMU=0; m_Virement=0; m_Id=0;
			m_Validite=0; m_Tracabilite=""; }

	~Honoraires() {}

	//////////////////////////////////////////
	//		METHODES		//
	//////////////////////////////////////////

	void		setPatient	(const char* n)
					{ m_Patient   = n;
					  m_Patient = m_Patient.stripWhiteSpace(); }

	void		setPraticien	(const char* n)
					 { m_Praticien = n;
					   m_Praticien = m_Praticien.stripWhiteSpace(); }

	void		setActes	(PAIEMENTS_LISTE &pay);
	void		setActes	(const char* recup_depuis_la_base);

	void		setDate		(const char* n)
					{ m_Date = n;
					  m_Date = m_Date.stripWhiteSpace(); }

	void		setDate		(QDate d)	 { m_Date = d.toString("dd-MM-yyyy"); }
	void		setEspeces	(double n)  	 { m_Especes  = n; }
	void		setCheque	(double n);
	void		setCB		(double n)  	 { m_CB    = n; }
	void		setDAF		(double n)  	 { m_DAF   = n; }
	void		setAutre	(double n)  	 { m_Autre = n; }
	void		setCMU		(double n)  	 { m_CMU   = n; }
    void		setVirement	(double n)  	 { m_Virement    = n; }
	void		setRemarque	(const char* n)	 { m_Remarque = n; }
	void		setGUID		(const char* n)
					{ m_GUID = n;   m_GUID = m_GUID.stripWhiteSpace();}
	void		setEmetteur	(const char* em);
	void		setValidite	(int n)  	 { m_Validite  = n; }
	void		setId		(int n)  	 { m_Id = n; }
	void		setIdUsr	(int n)  	 { m_IdUsr = n; }
	void		setDrTuxUsr	(int n)		 { m_DrTuxUsr = n; }
	void		setTracabilite	(const char* n)	 { m_Tracabilite = n; }


	QString		getPatient	()  { return m_Patient; }
	QString		getPraticien	()  { return m_Praticien; }
	void		getActes	(PAIEMENTS_LISTE &pay);
	QString		getActesForBase	()  { return m_Actes; }
	QString		getActesToString();
	QString		getTypesActes	();
	double		getEsp		()  { return m_Especes; }
	double		getChq		()  { return m_Cheque; }
	double		getCB		()  { return m_CB; }
	double		getDAF		()  { return m_DAF; }
	double		getAutre	()  { return m_Autre; }
	double		getCMU		()  { return m_CMU; }
    double		getVirement		()  { return m_Virement; }
	int		getId		()  { return m_Id; }
	int		getIdUsr	()  { return m_IdUsr; }
	QDate		getDate		();
	QString		getGUID		() { return m_GUID; }
	QString		getRemarque	() { return m_Remarque; }
	QString		getRemarqueLisible();
	QString		getEmetteur 	();
	int		getValidite	() { return m_Validite; }
	int		getDrTuxUsr	() { return m_DrTuxUsr; }
	QString		getTracabilite	() { return m_Tracabilite; }

	void		traceModifications	(Honoraires* anterieur, Utilisateurs* user);

	QString		dateForSQL	(const QString &date);
	double		total		() { return m_Especes+m_Cheque+m_CB+m_DAF+m_Autre+m_CMU+m_Virement; }
	QString		toString	();

	//////////////////////////////////////////
	//		DONNEES			//
	//////////////////////////////////////////

public:
	QString	m_Patient;	/*!< Nom du patient concerné ( + GUID drtux ). */
	QString	m_Praticien;	/*!< Nom du praticien effectuant l'acte. */
	QString	m_Date;		/*!< Date de l'honoraire. */
	QString	m_Actes;	/*!< Liste des actes. */
	QString	m_Remarque;	/*!< Remarque. */
	QString	m_GUID;		/*!< La GUID du patient pour faire le lien avec drtux */
	int     m_IdUsr;	/*!< La clé primaire de l'utilisateur du logiciel de compta */
	double	m_Especes;	/*!< Montant des Espèces. */
	double	m_Cheque;	/*!< Montant des chèques. */
	double	m_DAF;		/*!< Montant du DAF (paiement différé sécu, mutuelle...). */
	double	m_CB;		/*!< Montant des CB. */
	int	m_Id;		/*!< Identifiant de l'honoraire dans le base. */
	double	m_Autre;	/*!< Montant de Autre (tribunal, salaire...). */
	double	m_CMU;		/*!< Montant restant dû. */
	double  m_Virement;
	int	m_Validite;	/*!< Validité de l'honoraire (en fait devrait être un bool) */
	int	m_DrTuxUsr;	/*!< identifiant de l'utilisateur dans la base de DrTux */
	QString	m_Tracabilite;	/*!< Tracabilité des modifications, sous forme tableau HTML. */
	int	m_State;
};

// Définition d'un alias vers une liste d'actes
typedef QValueList<Honoraires> HONO_LISTE;





/** \class Depots
 * \brief Classe pour la création de la liste des Depots.
 * Cette classe ne sert que pour la définition des caractéristiques des Depots . Un alias vers une QValueList de Depots est créé : DEPOTS_LISTE. \n \n
	@author Eric Maeker <eric.maeker@free.fr>
*/
class Depots
{
public:
	Depots() { m_Nom=""; m_IdCompte=-1; m_Remarque=""; m_Total=0; m_NbCheques=0; m_ListeIdHono="";
			m_Id=-1; m_IdUsr=-1; }

	~Depots() {}

	//////////////////////////////////////////
	//		METHODES		//
	//////////////////////////////////////////

	void		setNom		(const char* n)
					{ m_Nom = n;
					  m_Nom = m_Nom.stripWhiteSpace(); }
	void		setDateValidation	(const char* n)
						{ m_DateValidation = QDate::fromString(n,Qt::ISODate); }
	void		setDateDebut		(const char* n)
						{ m_DateDebut = QDate::fromString(n, Qt::ISODate); }
	void		setDateFin		(const char* n)
						{ m_DateFin = QDate::fromString(n, Qt::ISODate); }
	void		setRemarque		(const char* n) { m_Remarque    = n; }
	void		setTotal		(double n)	{ m_Total       = n; }
	void		setNbCheques		(int n)		{ m_NbCheques   = n; }
	void		setListeIdHono		(const char* n) { m_ListeIdHono = n; }
	void		setIdDepot		(int n) 	{ m_Id          = n; }
	void		setIdUser		(int n) 	{ m_IdUsr       = n; }
	void		setIdCompte		(int n) 	{ m_IdCompte    = n; }

	QString		getNom		()  { return m_Nom; }
	QString		getDate		()  { return m_DateValidation.toString("dd-MM-yyyy");}
	QDate		getDateValid	()  { return m_DateValidation; }
	QDate		getDateDebut	()  { return m_DateDebut;}
	QDate		getDateFin	()  { return m_DateFin;}
	QString		getListeIdHono	()  { return m_ListeIdHono; }
	QString		getRemarque	()  { return m_Remarque; }
	int		getIdCompte	()  { return m_IdCompte; }

	HONO_LISTE*	getDepotHonoListe	();

	//////////////////////////////////////////
	//		DONNEES			//
	//////////////////////////////////////////

public:
	QString	m_Nom;			/*!< Nom du dépôt. */
	QDate	m_DateValidation;	/*!< Date de validation du dépôt. */
	QDate	m_DateDebut;		/*!< Date de début de période concernant le dépôt. */
	QDate	m_DateFin;		/*!< Date de fin de période concernant le dépôt. */
	QString	m_Remarque;		/*!< Remarque des dépôts (contient le RIB du compte où le dépôt est effectué. */
	double	m_Total;		/*!< Montant des Espèces. */
	int	m_NbCheques;		/*!< Montant des chèques. */
	QString m_ListeIdHono;		/*!< Liste des honoraires concernés par le dépot (suite d'Id_Hono séparés par ';')*/
	int m_Id;			/*!< Identifiant du dépot */
	int m_IdUsr;			/*!< Identifiant de l'utilisateur lié à ce dépot */
	int m_IdCompte;			/*!< Identifiant du compte auquel est lié ce dépôt */

};

// Définition d'un alias vers une liste d'actes
typedef QValueList<Depots> DEPOTS_LISTE;



class Comptes
{
public:
	Comptes() { m_NomBanque=""; m_Rib_Banque=""; m_Rib_Guichet=""; m_Rib_Compte=""; m_Titulaire="";
		    m_Rib_Cle="";  m_Id=-1; m_IdUsr=-1;  m_Remarque=""; m_Solde=0;}

	~Comptes()  {}


		//////////////////////////////////////////
		//		METHODES		//
		//////////////////////////////////////////

	void		setLibelle	(const char* n)  { m_Libelle    = n; }
	void		setNomBanque	(const char* n)  { m_NomBanque  = n; }
	void		setBanque	(const char* n)  { m_Rib_Banque = n; }
	void		setRib		(const char* banq, const char* guich, const char* compte, const char* cle);
	void		setId		(int n)  	 { m_Id         = n; }
	void		setIdUsr	(int n)		 { m_IdUsr      = n; }
	void		setRemarque	(const char* n)  { m_Remarque   = n; }
	void		setTitulaire	(const char* n)  { m_Titulaire  = n; }
	void		setSolde	(double n)	 { m_Solde      = n; }

	QString		getLibelle	()  { return m_Libelle; }
	QString		getRemarque	()  { return m_Remarque; }
	QString		getNomBanque	()  { return m_NomBanque; }
	QString		getRibBanque	()  { return m_Rib_Banque; }
	QString		getRibGuichet	()  { return m_Rib_Guichet; }
	QString		getRibCompte	()  { return m_Rib_Compte; }
	QString		getRibCle	()  { return m_Rib_Cle; }
	QString		getRib		(const char* type);
	QString		getRibForBase	();
	int		getId		()  { return m_Id; }
	bool		isDefaut	()  { if (m_Remarque.find(CPT_PAR_DEFAUT) == -1)
						return FALSE; else return TRUE; }
	int		getIdUsr	()  { return m_IdUsr; }
	QString		getTitulaire	()  { return m_Titulaire; }
	double		getSolde	()  { return m_Solde; }


		//////////////////////////////////////////
		//		DONNEES			//
		//////////////////////////////////////////
public:
	QString m_Libelle;	/*!< Libellé du compte */
	QString m_Titulaire;	/*!< Nom du titulaire du compte */
	QString m_NomBanque;	/*!< Nom de la banque */
	QString m_Rib_Banque;	/*!< String pour la construction du RIB : Code Banque */
	QString m_Rib_Guichet;	/*!< String pour la construction du RIB : Code Guichet */
	QString m_Rib_Compte;	/*!< String pour la construction du RIB : Compte */
	QString m_Rib_Cle;	/*!< String pour la construction du RIB : Clé RIB */
	int     m_IdUsr;	/*!< Identifiant de l'utilisateur */
	int     m_Id;		/*!< Identifiant du compte */
	QString m_Remarque;	/*!< Champs remarque du compte */
	double	m_Solde;	/*!< Solde à la création du compte dans le logiciel de compta */

};

// Définition d'un alias vers une liste de paiements
typedef QValueList<Comptes> COMPTES_LISTE;


class Mouvements
{
public:
	Mouvements() { m_Nom=""; m_Remarque=""; m_Total=0; m_Id=-1; m_IdUsr=-1; m_IdParent=-1; m_Type=-1;
			m_Valide=0; m_Tracabilite=""; m_IdCompte=-1; m_IdImmob=0;}

	~Mouvements()  {}


		//////////////////////////////////////////
		//		METHODES		//
		//////////////////////////////////////////

	void		setNom		(const char* n)  { m_Nom        = n; }
	void		setRemarque	(const char* n)	 { m_Remarque   = n; }
	void		setId		(int n)  	 { m_Id         = n; }
	void		setIdCompte	(int n)  	 { m_IdCompte   = n; }
	void		setIdUsr	(int n)  	 { m_IdUsr      = n; }
	void		setIdParent	(int n)		 { m_IdParent   = n; }
	void		setType		(int n)		 { m_Type   = n; }
	void		setTotal	(double n)	 { m_Total  = n; }
	void		addToTotal	(double n)	 { m_Total += n; }
	void		setDate		(const char* n)	 { m_Date = QDate::fromString(n, Qt::ISODate); }
	void		setDateValeur	(const char* n)	 { m_DateValeur = QDate::fromString(n, Qt::ISODate); }
	void		setDateValeur	(QDate n)	 { m_DateValeur = n; }
	void		setDate		(QDate n)	 { m_Date = n; }
	void		setIdImmob	(int n);
	void		setValidite	(int n)		 { m_Valide = n; }
	void		setTracabilite	(const char* n)	 { m_Tracabilite = n; }


	QString		getTracabilite	()  { return m_Tracabilite; }
	QString		getNom		()  { return m_Nom; }
	QString		getRemarque	()  { return m_Remarque; }
	int		getIdUsr	()  { return m_IdUsr; }
	int		getId		()  { return m_Id; }
	int		getType		()  { return m_Type; }
	double		getTotal	()  { return m_Total; }
	int		getIdParent	()  { return m_IdParent; }
	QDate		getDate		()  { return m_Date; }
	QDate		getDateValeur	()  { return m_DateValeur; }
	int		getIdCompte	()  { return m_IdCompte; }
	int		getIdImmob	()  { return m_IdImmob; }
	int		getValidite	()  { return m_Valide; }

	bool		isParent	()  { if (m_IdParent > 0) return FALSE; else return TRUE; }
	QString		getTotalDepot	(QString& rmq);

	void 		traceModifications(Mouvements* anterieur, Utilisateurs* user,
					   Mouvements* pMvts_Dispo_Ant, Mouvements* pMvts_Dispo_Actuel,
					   COMPTES_LISTE& pComptes_Liste );


		//////////////////////////////////////////
		//		DONNEES			//
		//////////////////////////////////////////

public:
	QString	m_Nom;		/*!< Nom de l'utilisateur */
	QString	m_Remarque;
	QDate	m_Date;
	QDate	m_DateValeur;
	QString	m_Tracabilite;	/*!< Tracabilité des modifications, sous forme tableau HTML. */

	int     m_Type;		/*!< Type de mouvement 0 = recettes; 1= depenses */
	int     m_IdParent;	/*!< Identifiant de l'utilisateur pour la base DrTux */
	int     m_Id;		/*!< Identifiant de l'utilisateur pour la base comptabilité */
	int     m_IdUsr;	/*!< Identifiant de l'utilisateur pour la base comptabilité */
	int	m_IdCompte;	/*!< Identifiant du compte auquel est lié ce mouvement */
	int	m_Valide;	/*!< Validité du mouvement (en fait devrait être un bool) */
	int	m_IdImmob;	/*!< Identifiant de l'immobilisation à la source de ce mouvement */
	double	m_Total;

};

// Définition d'un alias vers une liste de paiements
typedef QValueList<Mouvements> MVTS_LISTE;


class Immobilisations
{
public:
	Immobilisations() { m_Nom=""; m_Remarque=""; m_Id=-1; m_IdUsr=-1; m_Duree=0; m_Mode=-1;
				m_IdCompte=-1;  m_Valeur=0; m_Residuel=0; m_Resultat=""; m_Mvts_Liste.clear();
				m_Valide=0; m_Tracabilite="";}

	~Immobilisations()  {}


		//////////////////////////////////////////
		//		METHODES		//
		//////////////////////////////////////////

	void		setNom		(const char* n)  { m_Nom        = n; }
	void		setRemarque	(const char* n)	 { m_Remarque   = n; }
	void		setMouvements	(const char* n)  { m_Mouvements = n; }
	void		setMouvements	(MVTS_LISTE& il);
	void		setId		(int n)  	 { m_Id         = n; }
	void		setIdCompte	(int n)  	 { m_IdCompte   = n; }
	void		setIdUsr	(int n)  	 { m_IdUsr      = n; }
	void		setValeur	(int n)	 { m_Valeur     = n; }
	void		setResiduel	(int n)	 { m_Residuel   = n; }
	void		setDateService	(const char* n)	 { m_DateService = QDate::fromString(n, Qt::ISODate); }
	void		setDuree	(int n)  	 { m_Duree      = n; }
	void		setMode		(int n)  	 { m_Mode         = n; }
	void		setResultat	(const char* n)  { m_Resultat   = n; }
	void		setValidite	(int n)  	 { m_Valide   = n; }
	void		setTracabilite	(const char* n)	 { m_Tracabilite = n; }


	QString		getTracabilite	()  { return m_Tracabilite; }
	QString		getNom		()  { return m_Nom; }
	QString		getRemarque	()  { return m_Remarque; }
	QString		getResultat	()  { return m_Resultat; }
	QString		getMouvements	()  { return m_Mouvements; }
	bool		getMouvements	(MVTS_LISTE& il);
	int		getIdUsr	()  { return m_IdUsr; }
	int		getId		()  { return m_Id; }
	int		getIdCompte	()  { return m_IdCompte; }
	int		getMode		()  { return m_Mode; }
	int		getValeur	()  { return m_Valeur; }
	int		getResiduel	()  { return m_Residuel; }
	QDate		getDateService	()  { return m_DateService; }
	int		getDuree	()  { return m_Duree; }
	int		getValidite	()  { return m_Valide; }


	void		prepareResultat	();

		//////////////////////////////////////////
		//		DONNEES			//
		//////////////////////////////////////////

public:
	QString	m_Nom;		/*!< Nom de l'utilisateur */
	QString	m_Mouvements;	/*!< Champs mouvements de la table : contient les id des mouvements liés à cette immobilisation */
	QString	m_Remarque;	/*!< Champs remarque de la table */
	QString	m_Resultat;	/*!< Champs résultat de la table : contient le tableau HTML de l'immobilisation */
	QDate	m_DateService;	/*!< Date de mise en service du bien à amortir */
	QString	m_Tracabilite;	/*!< Tracabilité des modifications, sous forme tableau HTML. */

	int     m_Mode;		/*!< Type d'amortissement 0=linéaire; 1=dégressif */
	int     m_Id;		/*!< Identifiant de l'utilisateur pour la base comptabilité */
	int     m_IdUsr;	/*!< Identifiant de l'utilisateur pour la base comptabilité */
	int	m_IdCompte;	/*!< Identifiant du compte auquel est lié cette immobilisation */
	int	m_Duree;	/*!< Durée de l'immobilisation */
	int	m_Valeur;	/*!< Valeur du bien */
	int	m_Residuel;	/*!< Valeur résiduelle */
	int	m_Valide;	/*!< Validation du mouvement */

	MVTS_LISTE	m_Mvts_Liste;	/*!< Liste des mouvements de l'immobilisation qui seront sauvegardés séparément dans la base de données */

};

// Définition d'un alias vers une liste de paiements
typedef QValueList<Immobilisations> IMMO_LISTE;


#endif



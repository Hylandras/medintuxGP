//
// C++ Interface: C_Var
//
// Description: 
//
//
// Author: Eric MAEKER eric.maeker@free.fr, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef VAR_CODE_H
#define VAR_CODE_H

#include <qstring.h>
#include <qstringlist.h>
#include <qdatetime.h>
#include <qvaluelist.h>
#include <qobject.h>
#include <qlistview.h>
#include "CMoteur_Base.h"
#include "../../MedinTuxTools/CPrtQListViewItem.h"

// Il faut récupérer les choix de méthode de tri des variables G_pCApp->m_Var_SortBy
#include "CApp.h"

#define  TR	QObject::tr

/**
	@author Eric MAEKER <eric.maeker@free.fr>
*/

/*! \class Var_Element
 *  \brief Classe Var_Element et Var_Code : Deux classes qui gèrent les Variables pour toute l'application. Il suffit d'injecter le fichier de Terrain et le reste suit.
 *  Cette classe contient les items de chaque Variables :
 *  - libellé
 *  - paramètres de définition : valeur maximum et minimum avant alerte, section, durée de validité, message d'alertes.
 */

class Var_Value
{
public :
	Var_Value()
		{ m_Value=0; }

	Var_Value(	QDateTime 	dt, 
			double		value)
		{ m_Value    = value;
		  m_DateTime = dt;
		}

	~Var_Value() {}

	// Implémentation de l'opérateur inférieur pour accéder aux fonctions qHeapSort
	// Trie selon le nom de le choix sauvegardé dans la variable G_pCApp->m_Var_SortBy
	bool operator < (const Var_Value& e) const
		{ if      (G_pCApp->m_VarValue_SortBy == CApp::SortByDate)        return e.m_DateTime < m_DateTime;
		  else if (G_pCApp->m_VarValue_SortBy == CApp::SortByValue)       return e.m_Value    < m_Value;
                  else   return 0;
		}

public:
	QDateTime	m_DateTime;	/*!< Date de la variable */
	double		m_Value;	/*!< Valeur de la variable pour la date de la classe. */
};
// Définition d'un alias vers une liste d'antécédents
typedef QValueList<Var_Value> VAR_VALUE_LISTE;




/*! \class Var_Element
 *  \brief Classe Var_Element et Var_Code : Deux classes qui gèrent les Variables pour toute l'application. Il suffit d'injecter le fichier de Terrain et le reste suit.
 *  Cette classe contient les items de chaque Variables :
 *  - libellé
 *  - paramètres de définition : valeur maximum et minimum avant alerte, section, durée de validité, message d'alertes.
 */

class Var_Element
{
public :
	Var_Element()
		{ m_Libelle=""; m_Section=""; m_Maximum=0; m_Minimum=0; m_Unite="";
		  m_DureeValid=0; m_AlerteMax=""; m_AlerteMin=""; m_pValue_Liste.clear(); }

	Var_Element(	QString&	section,
			QString&	libelle,
			QString&	unite,
			double		max,
			double		min,
			long int	dureeMax,
			QString&	alerteMax,
			QString&	alerteMin
		   )
		{ m_Section	= section; 
		  m_Libelle	= libelle;
		  m_Unite	= unite;
		  m_Maximum	= max;
		  m_Minimum	= min;
		  m_DureeValid	= dureeMax;
		  m_AlerteMax	= alerteMax;
		  m_AlerteMin	= alerteMin;
		  m_pValue_Liste.clear();
		}

	~Var_Element() {}

	void	addValue(QString& dt_str, QString& value);

	void	setValueSort(int s)	{ G_pCApp->m_VarValue_SortBy = s; }
	void	sortValue()		{ qHeapSort(m_pValue_Liste); }

	// Implémentation de l'opérateur inférieur pour accéder aux fonction qHeapSort
	// Trie selon le nom de la rubrique
	bool operator< (const Var_Element& e) const;

	bool 		isOverMaximum();
	bool 		isUnderMinimum();

	QString 	toOldIniParser();
	QString		toXMLParser();

public:
	QString		m_Libelle;	/*!< libellé de l'ATCD */
	QString		m_Section;	/*!< Section à laquelle appartient la variable */
	QString		m_Unite;	/*!< unité de mesure de la variable */
	double		m_Maximum;	/*!< Valeur maximale que peut prendre la variable */
	double		m_Minimum;	/*!< Valeur minimale que peut prendre la variable */
	long int	m_DureeValid;	/*!< Durée de validité de la variable en secondes */

	QString		m_AlerteMax;	/*!< Message d'alerte en cas de dépassement de la valeur supérieure */
	QString		m_AlerteMin;	/*!< Message d'alerte en cas de valeur inférieure à la valeur minimale */
	VAR_VALUE_LISTE	m_pValue_Liste; /*!< Toutes les valeurs de la variable */

};
// Définition d'un alias vers une liste d'antécédents
typedef QValueList<Var_Element> VAR_LISTE;




class Var_Code
{
public:
	Var_Code();

	~Var_Code();

	////////////////////////////////////////////
	///////////////  METHODES  /////////////////
	////////////////////////////////////////////

	// GESTION DE LA LECTURE
	void		setTerrain		(const char* strTerrain, CMoteurBase* pCMoteurBase);
	int		setTerrain		(RUBREC_LIST::iterator it, CMoteurBase* pCMoteurBase);

	// FONCTIONS DE TRIE
	void		setSort			(int sort);
	void		sort			();

	// FONCTIONS DE PRESENTATION
	void		getVarToListViewItem	(CPrtQListViewItem& retour);

	// FONCTIONS DE RECUPERATION ET D'AJOUT
	VAR_LISTE::iterator	getVarIteratorUsingVarName (const char* section_var, const char* varName);
	QString		getVarNearer		( const char* varName, QDateTime dt );
	QStringList	getVar			(const char* section_var, const char* name_var);
	bool		getVarValueList		(const char* section_var, 
						 const char* name_var,
						 VAR_VALUE_LISTE& liste );
	

	// GESTION DE LA SAUVEGARDE
	QString 	toOldIni();
	QString 	toOldIni_AddValues_ToSection(VAR_LISTE& pVar_Liste);

	QString*	toXMLParser();

	////////////////////////////////////////////
	///////////////   DONNES   /////////////////
	////////////////////////////////////////////

public :

	QString		m_Terrain;		/*!< Le fichier terrain à analyser */
	QStringList	m_Sections;		/*!< liste des sections disponibles */
	VAR_LISTE	m_pVar_Liste;		/*!< Liste des Variables */
	CMoteurBase*	m_pCMoteurBase;		/*!< Pointeur vers les fonctions de la base de données */

};

#endif

//
// C++ Implementation: C_Var
//
// Description:
//
//
// Author: Eric MAEKER <eric.maeker@free.fr> (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "C_Var.h"

//============================= Var_Element =========================================================================================

//------------------------------ isOverMaximum ------------------------------------------------------------------------------------------
/*! \brief TRUE si la valeur de la variable est supérieur à la valeur maximale. */
bool Var_Element::isOverMaximum()
{ return TRUE;
}
//------------------------------ isUnderMinimum ------------------------------------------------------------------------------------------
/*! \brief TRUE si la valeur de la variable est supérieur à la valeur maximale. */
bool Var_Element::isUnderMinimum()
{ return TRUE;
}

//------------------------------ addValue ------------------------------------------------------------------------------------------
/*! \brief Ajoute une valeur dans la liste lièe à une variable. */
void Var_Element::addValue( QString& dt, QString& value )
{ m_pValue_Liste.append( Var_Value( QDateTime::fromString(dt,Qt::ISODate), value.toDouble() ) );
}

//------------------------------ operator < ------------------------------------------------------------------------------------------
/*! \brief défini la comparaison de deux éléments pour permettre le trie des variables.
 *  Les variables sont triées soit par ordre de section, soit par ordre de libellé. Pour trier les valeurs, utiliser Var_Value::setsort   et  Var_Value::sort() sur la variable m_pValue_Liste de cette classe.
 *  \sa enum CApp:SortBy
*/
bool Var_Element::operator< (const Var_Element& e) const
{ bool resultat;
  switch (G_pCApp->m_Var_SortBy)
  { case CApp::SortByLibelle :
	{ resultat = m_Libelle < e.m_Libelle;
	  break;
	}
    default : // SortBySection
	{ resultat = m_Section < e.m_Section;
	  break;
	}
  } // end switch
  return resultat;
}



//============================= Var_Element =========================================================================================
//------------------------------ Var_Code ------------------------------------------------------------------------------------------
Var_Code::Var_Code()
{ m_Terrain = "";
  m_Sections.clear();
  m_pVar_Liste.clear();
  m_pCMoteurBase = 0;
}


//------------------------------ ~Var_Code ------------------------------------------------------------------------------------------
Var_Code::~Var_Code()
{
}


//------------------------------ setSort ------------------------------------------------------------------------------------------
/*! \brief définit l'item principal pour le tri.
 *  \param sort : doit être un membre de l'énumération SortBy de CApp.
 *  \sa CApp, m_Var_SortBy, enum CApp::SortBy.
*/
void Var_Code::setSort(int sort)
{ G_pCApp->m_Var_SortBy = sort; }

//------------------------------ sort ------------------------------------------------------------------------------------------
/*! \brief Lance le tri des variables selon l'item demandé.
 *  \sa Var_Code::setSort().
*/
void Var_Code::sort()
{ qHeapSort( m_pVar_Liste ) ;
}


//------------------------------ getVarNearer ------------------------------------------------------------------------------------------
/*! \brief Retourne la valeur de la variable nommée \em varName la plus proche de la date \em dt .
 * Fonction testée OK
*/
QString Var_Code::getVarNearer( const char* varName, QDateTime dt )
{if (dt.isNull())                  return "";
 if (varName==0 || varName[0]==0)  return "";

 // Récupère l'iterator sur la variable recherchée
 VAR_LISTE::iterator it = getVarIteratorUsingVarName("", varName);
 if (it==0) return "";

 long          secondsToVar;		// Secondes entre la date passée en paramètre et la variable en teste
 long          secondsToRecent;		// Temps le plus court trouvé
 double        mostRecentVal = 0;	// Valeur à renvoyer
 QDateTime     varQDate;
 if ((*it).m_pValue_Liste.count() == 1)
  { return QString::number( (*(*it).m_pValue_Liste.at(0)).m_Value ) ;
  }
 // trie de façon ascendante par la date (valeurVar[0] = la plus ancienne)
 (*it).setValueSort( CApp::SortByDate );
 (*it).sortValue();
 VAR_VALUE_LISTE::iterator it_value;

 // récupère dans varDate la première date comme référence la plus ancienne
 QDateTime   varQDateRecent    = (*(*it).m_pValue_Liste.at(0)).m_DateTime;
 mostRecentVal       = (*(*it).m_pValue_Liste.at(0)).m_Value; // au cas où il n'y aurait qu'une seule valeur...

 for (it_value=(*it).m_pValue_Liste.begin() ; it_value!=(*it).m_pValue_Liste.end(); ++it_value)
 { varQDate       = (*it_value).m_DateTime;
   // Compare le nombre de secondes entre DATE VISITE et les deux dates
   secondsToVar    = varQDate.secsTo(dt);
   secondsToRecent = varQDateRecent.secsTo(dt);
   if (secondsToVar < 0)         secondsToVar          = secondsToVar * (-1);
   if (secondsToRecent < 0)      secondsToRecent       = secondsToRecent * (-1);
   if (secondsToVar < secondsToRecent)
   { mostRecentVal  = (*it_value).m_Value;
     varQDateRecent = varQDate;
   }
  } // Fin  for
// qWarning(QString::number( mostRecentVal ));
  return QString::number( mostRecentVal );
 return "";
}

//------------------------------ getVarIteratorUsingVarName ------------------------------------------------------------------------------------------
/*! \brief Retourne l'itérator de VAR_LISTE pointant sur la variable demandée.
 *  Fonction Testée OK.
 *  \todo améliorable.
*/
VAR_LISTE::iterator Var_Code::getVarIteratorUsingVarName (const char* section_var, const char* varName)
{ VAR_LISTE::iterator it;
  for ( it= m_pVar_Liste.begin(); it != m_pVar_Liste.end() ; ++it)
  { if  ((*it).m_Libelle == varName )
    { if (section_var != 0 && section_var[0] != 0) // section spécifiée alors vérifier, sinon arrête et récupère l'iterator
      { if ((*it).m_Section == section_var) break; }
      else break;
    }
  }
  // 1bis. trouvé ?
  if ((*it).m_Libelle != varName) return 0;
  else return it;
}

//------------------------------ getVar ------------------------------------------------------------------------------------------
/*! \brief Récupère toutes les valeurs de la variable incluse dans section section_var de la rubrique Terrain
 * \param section_var     : section
 * \param  nalme_var       :le nom de la variable
 * \return QStringList des valeurs triés en ascendant des valeurs au format DATE/;/VALEUR
 * \todo Utiliser la classe Var_Element pour le retour des données ? Nécessite réécriture de CDevilCrucible
*/
QStringList Var_Code::getVar (const char* section_var, const char* name_var)
{ if (name_var == 0    || name_var[0] == 0)    return 0;
  if (section_var == 0 || section_var[0] == 0) return 0;
  // 1. Trouver la variable nommée name_var
  VAR_LISTE::iterator it = getVarIteratorUsingVarName(section_var, name_var);
  if (it==0) return 0;

  // 2. Construire la QStringList demandée
  QStringList retour;
  retour.clear();
  VAR_VALUE_LISTE::iterator it_value;
  for (it_value=(*it).m_pValue_Liste.begin() ; it_value!=(*it).m_pValue_Liste.end(); ++it_value)
  { retour.append( (*it_value).m_DateTime.toString(Qt::ISODate) +"/;/" +  QString::number((*it_value).m_Value) );
  }
  return retour;
}

//------------------------------ getVarValueList ------------------------------------------------------------------------------------------
/*! \brief Récupère toutes les valeurs de la variable incluse dans section section_var de la rubrique Terrain
 * \param section_var     : section
 * \param  nalme_var       :le nom de la variable
 * \param liste		: VAR_VALUE_LISTE de retour
 * \return bool qui indique si tout OK ou pas
 * \todo à tester
*/
bool Var_Code::getVarValueList(const char* section_var, const char* name_var,VAR_VALUE_LISTE& liste )
{ if (name_var == 0 || name_var[0] == 0) return FALSE;
  // 1. Trouver la variable nommée name_var
  VAR_LISTE::iterator it = getVarIteratorUsingVarName(section_var, name_var);
  if (it==0) return FALSE;
  liste = (*it).m_pValue_Liste;
  return TRUE;
}


//------------------------------ setTerrain ------------------------------------------------------------------------------------------
/*! \brief Cette fonction définie pour la classe le fichier de terrain à analyser et construit la liste des Variables.
 *  \param CRubRecord : la rubrique contenant le fichier terrain à analyser
 *  \param it : pointeur vers les fonctions et données de la base de données
*/
int Var_Code::setTerrain(RUBREC_LIST::iterator it, CMoteurBase* pCMoteurBase)
{
 //........................... si rubrique qui etait avant celle a afficher, a ete modifiée ................................................
 //                            replacer son contenu dans la liste des rubriques avant d'afficher la nouvelle
 // if (m_IsModifiable && m_IsModified && m_LastRub != -1)
 //   {FormToData(stringDST);
 //    m_pCMoteurBase->ReplaceDataInRubList(stringDST, m_pRubList, m_LastRub );
 //   }
 //........................... retirer les donnees de la base SQL ..............................
 //
 QByteArray  data;
 int           ret = pCMoteurBase->GetDataFromRubList(data, it);
 QString stringDST = "";
 char         *ptr = data.data();
 long          len = data.size();
 if (ptr && len>0)
    {if (CGestIni::IsUtf8( ptr , len))
        {stringDST = QString::fromUtf8 ( ptr );
         ptr = (char*)(const char*) stringDST;
        }
     setTerrain(ptr, pCMoteurBase);
    }
 else
    {setTerrain("", pCMoteurBase);
    }
  return ret;
}

//------------------------------ setTerrain ------------------------------------------------------------------------------------------
/*! \brief Cette fonction défini pour la classe le fichier de terrain à analyser et construit la liste des Variables.
 *  \param strTerrain : le fichier terrain à analyser en mode texte
 *  \param CMoteurBase : pointeur vers les fonctions et données de la base de données
*/
void Var_Code::setTerrain(const char* strTerrain, CMoteurBase* /*pCMoteurBase*/)
{char *pt = (char*) (const char*) strTerrain;

 m_pVar_Liste.clear();
 m_Sections.clear();

 QString	strTab[10];
 QString	section		= "";
 QString	var_name	= "";
 //int		etat_int	= 0;
 QDate		dt;
 VAR_LISTE*	pVar_Liste = new VAR_LISTE;

 while((pt = CGestIni::Param_GotoNextSection(pt, 0, &section)) && *pt)
 { // [VAR Poids] 		puis les modèles puis les valeurs.
   if (section.left(3)=="VAR" )                  // à ce stade pt pointe au debut de la nouvelle ligne
   { QString name_section = section.mid(4);
     int        n_var      = 0;
     QString    title      = "";
     m_Sections.append( name_section ) ;
     pVar_Liste->clear();
     while (*pt && *pt != '[')
     {	var_name = "";
	pt   = CGestIni::Param_ExtraireNextValeurs(pt, var_name, &strTab[0], &strTab[1], &strTab[2], &strTab[3], &strTab[4], &strTab[5], &strTab[6], &strTab[7], &strTab[8]);
	//................ MODELES DES VARIABLES DE LA SECTION ...............................
	if (var_name==TR("Modèle"))
	{ // Modèle = Date, Poids, 31536000, 100, 50, Kg(s), Obésité, Amaigrissement
	  // Crée une nouvelle liste de variable_element et injecte les modèles digérés de la section
	  pVar_Liste->append( Var_Element(name_section,		// section,
					strTab[1],		// libelle,
					strTab[5],		// Unité
					strTab[3].toDouble(),	// max,
					strTab[4].toDouble(),	// min,
					strTab[5].toLong(),	// dureeMax,
					strTab[6],		// alerteMax,
					strTab[7]		// alerteMin
			   ) );
	  n_var++;
	} // endif (var_name=="Modèle")
	//................ VALEURS DATEES DE VARIABLE ........................................
	else
	if (var_name!="")
	{ // 2007-02-14T22:39:02 = 21, 117
	  // 2006-09-14T10:56:59 = 20, 114
	  // 2007-09-14T10:57:04 = 25, 120  Puis nouvelle section []
	    // Récupère la première ligne puis les autres.
	  for (int i=0; i<=n_var;i++)
	  { if (strTab[i] != "")
		(*pVar_Liste->at( i ) ).addValue( var_name, strTab[i] );
	  } // Fin for première ligne
	  while (*pt && *pt != '[')
	  { var_name = "";
	    pt   = CGestIni::Param_ExtraireNextValeurs(pt, var_name, &strTab[0], &strTab[1], &strTab[2], &strTab[3], &strTab[4], &strTab[5], &strTab[6], &strTab[7], &strTab[8]);
	    for (int i=0; i<=n_var;i++)
	    { if (strTab[i] != "")
		(*pVar_Liste->at( i ) ).addValue( var_name, strTab[i] );
	    } // Fin for autres lignes

	  }
	}
     } // end while (*pt && *pt != '[')

     // on a fait le tour de la section, sauvegarder la liste temporaire dans la liste Var de la classe
     VAR_LISTE::iterator it;
     for (it= pVar_Liste->begin(); it != pVar_Liste->end(); ++it)
     { m_pVar_Liste.append( (*it) ); }

     section="";
  } // endif (section.left(3)=="VAR" )
 } // Fin while Param_GotoNextSection
 if (pVar_Liste) delete pVar_Liste; pVar_Liste = 0;

}

//------------------------------ getVarToListViewItem ------------------------------------------------------------------------------------------
/*! \brief Construit un ListViewItem avec les variables prêt à être affiché.
 *  \todo limiter le tri par le biais d'un bool isModified ?
*/
void Var_Code::getVarToListViewItem(CPrtQListViewItem& retour)
{ if (m_pVar_Liste.count() == 0) return;

  VAR_LISTE::iterator it;
  VAR_VALUE_LISTE::iterator it_value;
  int i = 0;

  QString section_precedente = "";
  QString tmp = "";
  CPrtQListViewItem* rub_Item = 0;
  CPrtQListViewItem* var_Item = 0;
  //CPrtQListViewItem* val_Item = 0;


  // Trier les variables selon la section, et les valeurs selon la date
  setSort(CApp::SortByDate);
  sort();
  for (it=m_pVar_Liste.begin(); it != m_pVar_Liste.end(); ++it)
  { (*it).setValueSort(CApp::SortByDate);
    (*it).sortValue();
  }

  // Section par section, ajoute les 5 variables les plus récentes
  section_precedente = "";
  i = 0;
  for (it = m_pVar_Liste.begin(); it != m_pVar_Liste.end(); ++it)
  { if ( (*it).m_Section != section_precedente)
    { // Ajoute une branche section
      rub_Item = new CPrtQListViewItem( &retour, 0, (*it).m_Section);
      rub_Item->setOpen( TRUE );
      section_precedente = (*it).m_Section;
    } // Fin if changement de section

    // Ajoute la variables au rub_Item
    it_value = (*it).m_pValue_Liste.begin();
    tmp  = (*it).m_Libelle + " : ";
    tmp += QString::number((*it_value).m_Value) + " ";
    tmp += (*it).m_Unite + " (" ;
    tmp += (*it_value).m_DateTime.toString("dd-MM-yyyy") + ")";
    var_Item = new CPrtQListViewItem( rub_Item, 0, tmp );
    var_Item->setOpen( TRUE );


//     for (it_value = (*it).m_pValue_Liste.begin();
// 	it_value != (*it).m_pValue_Liste.end();
// 	++it_value)
//     { // Ajoute les valeurs
//       tmp =  (*it_value).m_DateTime.toString("dd MMM yyyy") + " - " + QString::number( (*it_value).m_Value) ;
//       tmp += " " + (*it).m_Unite;
//       val_Item = new CPrtQListViewItem( var_Item, val_Item, tmp);
//       ++i;
//       if (i == 6) break;
//     } // Fin for values
  }
}


//------------------------------ toOldIni_AddValues_ToSection ------------------------------------------------------------------------------------------
/*! \brief Fonction qui prépare une section pour la sauvegarde au format "INI".
 *  \param pVar_Liste : liste des variables de la section uniquement.
 * Fonctionnement vérifié.
*/
QString Var_Code::toOldIni_AddValues_ToSection(VAR_LISTE& pVar_Liste)
{QString resultat = "";
 QString tmp_pattern = "";			// Pattern temporaire général de la section
 QString dt_tmp = "";
 int i =0;

 QStringList pattern; 	pattern.clear();	// Patterns liés aux dates où sont insérés les valeurs
 QStringList lines;	lines.clear();		// Date des valeurs

 VAR_LISTE::iterator it_section;
 VAR_VALUE_LISTE::iterator it_value;

 for (it_section=pVar_Liste.begin(); it_section != pVar_Liste.end(); ++it_section)
 { (*it_section).setValueSort(CApp::SortByDate);
   (*it_section).sortValue();
   tmp_pattern += (*it_section).m_Libelle+", ";
 }

 // Prépare les lignes à insérer (ne met que les dates et le pattern)
 for (it_section=pVar_Liste.begin(); it_section != pVar_Liste.end(); ++it_section)
 { for (it_value = (*it_section).m_pValue_Liste.begin();
	it_value != (*it_section).m_pValue_Liste.end();
	++it_value)
    { if (lines.contains((*it_value).m_DateTime.toString(Qt::ISODate)) == 0)
	 { lines.append((*it_value).m_DateTime.toString(Qt::ISODate));
	   pattern.append( tmp_pattern);
	 }
     } // Fin for
 }

 // Remplace le pattern par les valeurs
 for (it_section=pVar_Liste.begin(); it_section != pVar_Liste.end(); it_section++)
 { for (it_value = (*it_section).m_pValue_Liste.begin();
	it_value != (*it_section).m_pValue_Liste.end();
	++it_value)
  { dt_tmp = (*it_value).m_DateTime.toString(Qt::ISODate);
    i = lines.findIndex( dt_tmp );
    pattern[i].replace( (*it_section).m_Libelle, QString::number((*it_value).m_Value) );
  } // Fin for values
  // Enlève les réferences dans le pattern pour lesquelles aucune valeur n'est insérée
  for (i=0; i<(int)pattern.count();++i)
  { pattern[i].replace( (*it_section).m_Libelle, "" ); }
 } // Fin for variables

 // Insère le QStringList dans la section
 for (i=0; i<(int)lines.count();++i)
 { resultat += "    " + lines[i] + " = "+pattern[i]+"\n"; }

 resultat += "\n";
 // Prépare le prochain tour
 pVar_Liste.clear();
 return resultat;
}

//------------------------------ toOldIni ------------------------------------------------------------------------------------------
/*! \brief Transforme les données de la classe pour qu'elles puissent intégrer une rubrique terrain parsée à l'ancienne mode
 * [VAR Poids]  <-- Section
 * Modèle = Date, Poids, 31536000, 100, 50, Kg(s), Obésité, Amaigrissement	<-- Définition des modèles
 * Modèle = Date, Taille, 31536000, 0, 0, cm(s), ,
 * 2008-09-26T14:18:50 = 27, 126						<-- les valeurs de la section
*/
QString Var_Code::toOldIni()
{ if ((m_pVar_Liste.count() == 0)) return QString::null;
  QString section_precedente = "";
  QString resultat = "";
  QString tmp = "";

  QStringList pattern;  pattern.clear();	// Contiendra les patterns pour les dates
  QStringList lines;  lines.clear();		// Contiendra les dates à lier aux patterns

  // Trie la liste des Variables selon le nom de la section
  setSort( CApp::SortBySection);
  sort();

  VAR_LISTE::iterator it = m_pVar_Liste.begin();

  VAR_LISTE varsOfSection;
  varsOfSection.clear();

  // Faire en boucle les étapes 1 à 4 jusqu'à ce que toutes variables soient enregistrées
  while ( it != m_pVar_Liste.end() )
  { // Créer la section
    if (section_precedente == "")
    { section_precedente = (*it).m_Section;
      resultat += "[VAR " + (*it).m_Section + "]\n";
    }
    else     // Teste si l'on se trouve toujours dans la même section, sinon en crée une nouvelle
    { if (section_precedente != (*it).m_Section) // Il faut créer une nouvelle section
      { resultat += "\n";
	resultat += toOldIni_AddValues_ToSection(varsOfSection);
	// Créer une nouvelle section pour y insérer les modèles
	resultat += "[VAR " + (*it).m_Section + "]\n";
	section_precedente = (*it).m_Section;
      }
    }

    // Insérer les modèles des variables de la section (max 10) et garder de côté les variables de la section
    resultat += "  Modèle = Date, " + (*it).m_Libelle + ", ";	// Libelle
    resultat += QString::number( (*it).m_DureeValid ) + ", ";	// Durée Validité
    resultat += QString::number( (*it).m_Maximum ) + ", ";	// Max
    resultat += QString::number( (*it).m_Minimum ) + ", ";	// Min
    resultat += (*it).m_Unite + ", ";				// Unités
    resultat += (*it).m_AlerteMax + ", ";			// Alerte Max
    resultat += (*it).m_AlerteMin + ", ";			// Alerte Min
    resultat += "\n";						// FIN

    // Ajoute la variable aux variables de la section
    varsOfSection.append( (*it) );
    // Passe à l'enregistrement suivant
    ++it;
  }
  // Insère les valeurs de la dernière section
  resultat += toOldIni_AddValues_ToSection(varsOfSection);

  // 4. Retourner le résultat
  return resultat;
}

//------------------------------ toXMLParser ------------------------------------------------------------------------------------------
/*! \brief Transforme les données de la classe pour qu'elles puissent intégrer une rubrique terrain parsée XML.
*/
QString* Var_Code::toXMLParser()
{ return 0;
}



//
// C++ Implementation: C_Atcd
//
// Description:
//
//
// Author: Eric MAEKER  <eric.maeker@free.fr> (C) 2008
// Author: Roland SEVIN <roland-sevin@medintux.org> (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "C_Atcd.h"
#include "drtux.h"
#include "../../MedinTuxTools/CGenTools.h"
#include "ui/Dlg_Calendar.h"
#include "C_TokenInterpret.h"
#include <qnamespace.h>
#include <qmap.h>
#include <qlineedit.h>
//============================= Atcd_Element =========================================================================================
//------------------------------ isAllergie ------------------------------------------------------------------------------------------
/*! \brief Teste l'ATCD et retourne TRUE si il s'agit d'une allergie. */
bool Atcd_Element::isAllergie() { return isAllergie(m_Code);}
//------------------------------ is_ATC ------------------------------------------------------------------------------------------
/*! \brief Teste l'ATCD et retourne TRUE si il s'agit d'un code atc. */
bool Atcd_Element::is_ATC()     { return is_ATC(m_Code);}
//------------------------------ isCisp ------------------------------------------------------------------------------------------
/*! \brief Teste l'ATCD et retourne TRUE si il s'agit d'un codage CISP. */
bool Atcd_Element::isCisp()     { return isCisp(m_Code);}
//------------------------------ isCIM10 --------------------------------------------------
/*! \brief Teste l'ATCD et retourne TRUE si il s'agit d'un codage CIM10. */
bool Atcd_Element::isCIM10()    { return isCIM10(m_Code);}

//------------------------------ isAllergie ------------------------------------------------------------------------------------------
/*! \brief fonction statique qui teste l'ATCD et retourne TRUE si il s'agit d'une allergie. */
bool Atcd_Element::isAllergie(const QString &code)
{ return (code.left(1) == "(");}
//------------------------------ is_ATC ------------------------------------------------------------------------------------------
/*! \brief fonction statique qui teste l'ATCD et retourne TRUE si il s'agit d'un code atc. */
bool Atcd_Element::is_ATC(const QString &code)
{ return (code.left(2) == "(.");}
//------------------------------ isCisp ------------------------------------------------------------------------------------------
/*! \brief fonction statique qui teste l'ATCD et retourne TRUE si il s'agit d'un codage CISP. */
bool Atcd_Element::isCisp(const QString &code)
{ return (code.left(1) == "-");}
//------------------------------ isCIM10 --------------------------------------------------
/*! \brief fonction statique qui teste l'ATCD et retourne TRUE si il s'agit d'un codage CIM10. */
bool Atcd_Element::isCIM10(const QString &code)
{ return (code.left(1) == "~" );}

//------------------------------ getPixmap --------------------------------------------------
/*! \brief Récupère le Pixmap de l'ATCD selon son code dans le Thème défini dans CApp.
*/
QPixmap Atcd_Element::getPixmap()
{ return getPixmap(m_Code);
}

//------------------------------ getPixmap --------------------------------------------------
/*! \brief fonction statique qui récupère le Pixmap de l'ATCD selon son code dans le Thème défini dans CApp.
*/
QPixmap Atcd_Element::getPixmap(const QString &code)
{ if ( isCIM10(code)  )
     {return Theme::getIcon( "16x16/Cim10ItemTab.png");
     }
  else if (isCisp(code))
     {return Theme::getIcon( "16x16/CispItemTab.png");
     }
  else if (is_ATC(code))
     {return Theme::getIcon( "16x16/atc_interdit.png");
     }
  else if (isAllergie(code))
     {return Theme::getIcon( "16x16/AllergieTab.png");
     }
  else
     {return Theme::getIcon( "16x16/item.png");
     }
}
//------------------------------ getLightCIM10 --------------------------------------------------
/*! \brief Les intitulés de la CIM10 sont parfois peu comptabiles avec la lecture. Cette fonction allège les intutilés pour les rendre plus compréhensibles.
*/
QString Atcd_Element::getLightCIM10()
{ QString retour = "";
  if ( isCIM10() )
  { // Enlève SAI, le texte après la virgule, le texte entre parenthèse
    retour = m_Libelle;
    retour.replace("SAI","");
    int deb = retour.find(",");
    int fin = -1;
    if (deb != -1) retour = retour.left(deb);
    int i = 0;
    while ((retour.find("(") != -1) || (i != 10))
    { ++i;
      deb = retour.find("(");
      fin = retour.find(")", deb);
      if (fin != -1)
      { //si le caractère avant la parenthèse est un espace -> l'enlever, sinon on le garde
        int i = 0;
        if (retour[fin-1] == ' ') i++;
        retour = retour.left(deb-i) + retour.right(retour.length() - fin - 1);
      }
      else break;
    }
   retour = retour.stripWhiteSpace();
 } // Fin ATCD CIM 10
 return retour;
}

//------------------------------ operator< --------------------------------------------------
/*! \brief défini la comparaison de deux éléments pour permettre le trie des antécédents.
 *  Les variables sont triées soit par ordre de rubrique, soit par ordre de libellé, soit par date. Pour trier les valeurs, utiliser Atcd_Code::setsort   et  Atcd_Code::sort() sur la variable m_Atcd_Liste de cette classe.
 *  \sa enum CApp:SortBy, CApp::m_Atcd_SortBy
*/
bool Atcd_Element::operator< (const Atcd_Element& e) const
{ bool resultat;
  switch (G_pCApp->m_Atcd_SortBy)
  { case CApp::SortByLibelle :
        { resultat = m_Libelle < e.m_Libelle;
          break;
        }
   case CApp::SortByDate :
        { resultat = m_DateDeb < e.m_DateDeb;
          break;
        }
   default : // SortByRubrique
        { if (m_Rubrique == e.m_Rubrique) resultat = m_Libelle < e.m_Libelle;
          else resultat = m_Rubrique < e.m_Rubrique;
          break;
        }
  } // end switch
  return resultat;
}

//------------------------------ operator== --------------------------------------------------
/*! \brief défini l'égalité entre deux éléments pour permettre la suppression des antécédents par remove().
 *  Teste d'abord l'égalité des m_Id_ATCD pour aller vite. S'il n'ont pas été définit : teste tous les items un �  un.
*/
bool Atcd_Element::operator== (const Atcd_Element& e) const
{ if ((m_Id_ATCD != -1) && (e.m_Id_ATCD != -1))
     { if (m_Id_ATCD  != e.m_Id_ATCD)             return FALSE; else return TRUE;
     }
  // Si les id n'étaient pas définis lors teste tous les items
  if (m_Rubrique      != e.m_Rubrique)            return FALSE;
  if (m_Libelle       != e.m_Libelle)             return FALSE;
  if (m_DateDeb       != e.m_DateDeb)             return FALSE;
  if (m_DateFin       != e.m_DateFin)             return FALSE;
  if (m_Commentaire   != e.m_Commentaire)         return FALSE;
  if (m_Code          != e.m_Code)                return FALSE;
  if (m_ald           != e.m_ald)                 return FALSE;
 return TRUE;
}

//================================ Atcd_Code ======================================================================

/*! \class Atcd_Code
 *  \brief Gestion des antécédents indépendemment de la vue actuelle.
 * <b>Cette classe offre tous les fonctions nécessaire �  la gestion des antécédents médicaux/chirugicaux/obstétricaux et autres. Pour profiter de ses fonctionnalités, vous devez d'abord l'instancier.</b>

 * <b>Initialisation :</b> avant de travailler sur le terrain, vous devez signifier �  la classe le CRubRecord �  utiliser. La fonction setTerrain(CRubRecord* pCRubRecord, CMoteurBase* pCMoteurBase) est prévue pour cela. Elle analysera le fichier de terrain, extrairera les antécédents de sorte qu'ils soient exploitables par l'application et sauvegardera la clé primaire du document terrain (pour éviter de le relire si aucune modification n'a été effectuée). La fonction getPkDocTerrain() retourne le PrimKey du document terrain en cours d'utilisation. Vous devez aussi définir les liste de rubbriques pour les antécédents médicaux et chirurgicaux. Les fonctions init_Get_RubChir(QStringList *pListItem  = 0 )  en sont chargées. Si aucune QStringList n'est passée en paramètre, les rubriques par défaut sont utilisées. Sinon ce sont les listes passées en paramètres. Voir aussi CApp::CApp au moment de l'instanciation initiale de la classe qui récupère les listes dans le fichier de paramètres de DrTux.

 * <b>Ajouts d'antécédents :</b> les fonctions addATCD_CIM10(), addATCD_Textuel(), addATCD_Allergie(), setGrossesse(bool etat), setAllaitement(bool etat) vous permettent de d'ajouter de nouveaux antécédents sans vous soucier des widgets d'affichage. Lorsque vous appelez addATCD_CIM10() la fenêtre d'ajout d'antécédents encodés en CIM10 apparaît automatiquement. Ceci simplifiera grandement votre code et sa lecture. La classe émet, si nécessaire, le signal ATCD_Changed() �  chaque fois qu'un antécédent est modifié ou ajouté. Les classes appelantes pourront en tirer partie pour leur synchronisation.

 * <b>Les fonctions de modification :</b> changeDDR(), (Atcd_Element* pAtcd), changeEtat(Atcd_Element* pAtcd), changeCommentaire(Atcd_Element* pAtcd), eraseAtcd(Atcd_Element* pAtcd), modifyAtcd(Atcd_Element* pAtcd), setRubrique(Atcd_Element* pAtcd, QString& rubrique), setGrossesse(bool etat),setAllaitement(bool etat), setDDRTo(QDate dt) permettent la modifications des éléments de l'antécédent (Atcd_Element) passé en paramètre. La classe émet, si nécessaire, le signal ATCD_Changed() �  chaque fois qu'un antécédent est modifié ou ajouté. Les classes appelantes pourront en tirer partie pour leur synchronisation.

 * <b>Connaissance des antécédents :</b> get_ATCD_Rubname(Atcd_Element& pAtcd) retourne la rubrique de l'antécédent. getDDR() retourne la date des dernières règles enregistrée. getTermeObstetrical(QDate dt = QDate()) calcule automatiquement le terme de la grossesse �  partir de la date passée en paramètre. getAtcd_byID(int idAtcd) retourne l'Atcd_Element dont l'id est celle passée en paramètre.

 * clear() efface toutes les données de la classe et remet �  zéro les différents indicateurs d'état.

 * <b>Connaître l'état de modification des antécédents :</b> géré de façon automatique cet état peut être définit manuellement grâce �  la fonction setModifiedState( bool state ). L'utilisation de cette fonction n'est pas souhaitable. Vous pouvez connaître l'état de modification grâce �  la fonction isModified() qui retournera un booléen (TRUE = modifié, FALSE = non modifié). Ces fonctions serviront �  la gestion des sauvegardes.

 * <b>La sauvegarde :</b> toOldIni() renvoie les antécédents encodés �  l'ancienne mode prêts pour la sauvegarde. Il ne restera plus qu'aux classes appelantes de gérer la suite du fichier terrain (qui contient en plus les variables, le traitement en cours). Une fonction toXML() est en cours d'écriture.

 * <b>Les fonctions de tri :</b> setSort(int mode) et sort(). Le mode de tri est définissable via l'énumération CApp::SortBy. Il est possible de trier par date, rubrique, libellé.

 * <b>Les fonctions pour l'affichage :</b> getAtcdToListViewItem(CPrtQListViewItem& retour) (utilisée par Dock_Menu, le menu d'accès rapide de DrTux), atcd_To_ListView(QListView& retour) (utilisée par FormRubTerrain)

 * <b>Pour des exemples de codage �  l'aide de cette classe : </b> voir Dock_Menu et FormRubTerrain.

 * \sa Atcd_Element

*/

//------------------------------ Atcd_Code ------------------------------------------------------------------------
/*! \brief Constructeur obligatoire. */
Atcd_Code::Atcd_Code(CMoteurBase  *pCMoteurBase, QWidget *parent, const char *name)
 : QObject(parent, name)
{ m_PkDocTerrain        = "";
  m_TerrainIsModified   = FALSE;
  m_pAtcdFatherListView = 0;
  m_pAtcdListViewItem   = 0;
  m_pLastElementAdded   = 0;
  m_pCMoteurBase        = pCMoteurBase;
  m_sexe                = GetSexe_FMI_FromBD();
}

//------------------------------ Atcd_Code ------------------------------------------------------------------------
/*! \brief Destructeur. */
Atcd_Code::~Atcd_Code()
{
}

//------------------------------ clear --------------------------------------------------
/*! \brief Efface les données de la classe et envoie un signal de changement des ATCD pour que les view se mettent �  jour.
*/
void Atcd_Code::clear()
{ m_Atcd_Liste.clear();
  m_Terrain           = "";
  m_Grossesse         = FALSE;
  m_Allaitement       = FALSE;
  m_DDR               = QDate();
  m_PkDocTerrain      = "";
  m_TerrainIsModified = FALSE;
  m_pLastElementAdded = 0;
  //emit (ATCD_Changed() );   // emit (ATCD_Changed() �  faire en fin de fonction utilisant clear() mais pas l�
}

//------------------------------ isModified --------------------------------------------------
/*! \brief retourne l'état de modification de la classe antécédent. TRUE si les antécédents ont été modifiés, FALSE cas contraire.
 *  \sa m_TerrainIsModified, emitATCD_Changed.
*/
bool Atcd_Code::isModified()
{ return m_TerrainIsModified; }

//------------------------------ isModified --------------------------------------------------
/*! \brief définit l'état de modification de la classe antécédent. TRUE si les antécédents ont été modifiés, FALSE cas contraire.
 *  \sa m_TerrainIsModified, emitATCD_Changed.
*/
void Atcd_Code::setModifiedState( bool state )
{ m_TerrainIsModified = state;
}

//------------------------------ emitATCD_Changed --------------------------------------------------
/*! \brief Avant d'émettre le signal ATCD_Changed , procède �  quelques vérifications de base, en particuliers définit la prorité m_TerrainIsModified �  TRUE.
*/
void Atcd_Code::emitATCD_Changed()
{ m_TerrainIsModified = TRUE;
  emit (ATCD_Changed() );
 }

//------------------------------ getPkDocTerrain --------------------------------------------------
/*! \brief Retourne la clé primaire du document actuellement utilisé pour le terrain.
 *  \sa CRubRecord::m_PrimKey
*/
QString Atcd_Code::getPkDocTerrain()
{ return m_PkDocTerrain; }

//------------------------------ setSort --------------------------------------------------
/*! \brief Définition de la méthode de tri : par Rubrique, par libelle, par Date.
 *  \sa CApp::SortBy, CApp::m_Atcd_SortBy
*/
void Atcd_Code::setSort(int mode)
{ G_pCApp->m_Atcd_SortBy = mode;
}

//------------------------------ sort --------------------------------------------------
/*! \brief Lance le tri selon la méthode sauvegardée dans CApp.
*/
void Atcd_Code::sort()
{ qHeapSort(m_Atcd_Liste);
}

//------------------------------ ATCD_To_Text --------------------------------------------------
/*! \brief Cette fonction formate les ATCD selon une chaine de parametrage
 * Plusieurs utilisations possibles de cette fonction : l'appel par "pattern" qui peut contenir
 * - ::LIBELLE::        Pour obtenir le libellé de l'antécédent
 * - ::MEDCHIR::        Pour le type d'antécédent (Médical, chirurgical, Facteur de risque, cardio-vasculaire...)
 * - ::ETAT::                Pour obtenir son état (guéri ou actif)
 * - ::CODE::                Pour obtenir le code CIM10 de l'antécédent
 * - ::NOTE::                Pour obtenir le commentaire de l'antécédent
 * - ::DATE::                Pour obtenir la date de l'antécédent.
*  - ::ALD::                 Pour obtenir la propriete ALD de l'antécédent.

 * L'appel du mode "linéaire" ou lisible permet d'obtenir une phrase digérée des antécédents. Pratique pour les courriers de correspondance.

 * Exemples d'utilisation dans les masques :
 *  \code   {{:: Extract_ATCD (lineaire) }}  \endcode
 *  \code   {{::#Extract_ATCD (° <b><u>::TYPE:: :</u></b>, ::LIBELLE:: (::NOTE::) en ::DATE::), en, en rapport avec une pathologie ALD}}  \endcode
 *  \todo A CONNECTER AVEC G_pCApp->m_pAtcd_Code

*/
QString Atcd_Code::ATCD_To_Text(            const QString &typePrefix,
                                            const QString &linePatern,
                                            const QString &datePrefix,
                                            const QString &aldPrefix)
{return C_TokenInterpret::Func_Extract_ATCD_D(toOldIni(),
                                            typePrefix,
                                            linePatern,
                                            datePrefix,
                                            aldPrefix
                                          );
}
//------------------------------ setTerrain --------------------------------------------------
/*! \brief Cette fonction défini pour la classe le fichier de terrain �  analyser et construit la liste des ATCD.
 * Attention, si des données sont déj�  présentes dans la classe elles seront effacées. Veillez donc �  gérer la sauvegarde si nécessaire avant l'appel de cette fonction.
 *  \param CRubRecord = pointeur vers l'enregistrement de terrain �  utiliser
 *  \param CMoteurBase : pointeur vers le moteur de la base de données
*/
void Atcd_Code::setTerrain(CRubRecord* pCRubRecord, CMoteurBase* pCMoteurBase)
{ if (!pCRubRecord)                             return;
  if (!pCMoteurBase)                            return;
  QString data = "";
  if (pCRubRecord->m_PrimKey == m_PkDocTerrain) return;
  pCMoteurBase->GetDataFromRubList(data, pCRubRecord);
  m_PkDocTerrain = pCRubRecord->m_PrimKey;
  setTerrain(data, pCMoteurBase);
}

//------------------------------ setTerrain --------------------------------------------------
/*! \brief Cette fonction renseigne et initialise la classe avec les donnees SQL de terrain �  analyser et construit la liste des ATCD.
 *  \param CRubRecord = pointeur vers l'enregistrement de terrain �  utiliser
 *  \param it : pointeur vers le moteur de la base de données
*/
int Atcd_Code::setTerrain(RUBREC_LIST::iterator it, CMoteurBase* pCMoteurBase)
{
 //........................... si rubrique qui etait avant celle a afficher, a ete modifiée ................................................
 //                            replacer son contenu dans la liste des rubriques avant d'afficher la nouvelle
 // if (m_IsModifiable && m_IsModified && m_LastRub != -1)
 //   {FormToData(stringDST);
 //    m_pCMoteurBase->ReplaceDataInRubList(stringDST, m_pRubList, m_LastRub );
 //   }
  if ((*it).m_PrimKey == m_PkDocTerrain) return 0;
  clear();
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
  emit ATCD_Changed();
  return ret;
}
//------------------------------ setDatesToNewVersion --------------------------------------------------
/*! \brief converti l'ancien format (date debut, etat) d'un atcd en s'assurant que les dates de fin et debut soient correctes.
 *  \param dateDeb = texte de la date de debut au format dd-MM-yyyy
 *  \param dateFin = texte de la date de fin   au format dd-MM-yyyy
 *  \param dt_deb  = date de debut calculee en fonction de elements fournis par dateDeb et dateFin
 *  \param dt_fin  = date de fin   calculee en fonction de elements fournis par dateDeb et dateFin
*/
void Atcd_Code::setDatesToNewVersion(QString &dateDeb, QString &dateFin, QDate &dt_deb, QDate &dt_fin)
{       dt_deb = QDate::fromString(CGenTools::dd_MM_yyyy_ToIso(dateDeb), Qt::ISODate);
        dt_fin = QDate::fromString(CGenTools::dd_MM_yyyy_ToIso(dateFin), Qt::ISODate);
        if (dateFin[0]=='P')
           {if (dateDeb.length())
               {dt_deb = QDate::fromString(CGenTools::dd_MM_yyyy_ToIso(dateDeb), Qt::ISODate);
                dt_fin = dt_deb.addDays ( 1 );     // en mettant +1 jour pour la date de fin on est quasi sur que l'antecedant est du passe
               }
           }
        else if (dateFin[0]=='A')
           {if (dateDeb.length())
               {dt_deb = QDate::fromString(CGenTools::dd_MM_yyyy_ToIso(dateDeb), Qt::ISODate);
                dt_fin = QDate();     // en mettant une date invalide on met l'antecedent actif
               }
           }
}

//------------------------------ setTerrain --------------------------------------------------
/*! \brief Préférez utiliser la fonction setTerrain(CRubRecord* pCRubRecord, CMoteurBase* pCMoteurBase) plutôt que cette fonction, car elle gère pas la synchronisation. Cette fonction défini pour la classe le fichier de terrain �  analyser et construit la liste des ATCD.
 * Attention, si des données sont déj�  présentes dans la classe elles seront effacées. Veillez donc �  gérer la sauvegarde si nécessaire avant l'appel de cette fonction.
 *  \param strTerrain = texte de l'enregistrement de terrain �  utiliser
 *  \param CMoteurBase : pointeur vers le moteur de la base de données
*/
void Atcd_Code::setTerrain(const char* strTerrain, CMoteurBase* /*pCMoteurBase*/)
{// Récupère la liste des ATCD
 //clear();
 int id        = 0;
 m_Grossesse   = FALSE;
 m_Allaitement = FALSE;
 m_Terrain     = strTerrain;
 m_Atcd_Liste.clear();

 char *pt = (char*) (const char*) strTerrain;

 //........... effacer les anciennes tables de variables ................
 //
 QString libelle, dt_str, rubrique, comm, etat, code, ald;
 QString      section          = "";
 QString     var_name          = "";
 Atcd_Element*            Atcd = 0;
 QString               dateDeb = "";
 QString               dateFin = "";
 QDate        dt_deb;
 QDate        dt_fin;
 while((pt = CGestIni::Param_GotoNextSection(pt, 0, &section)) && *pt)
 { if (section == TR("Antecedents") || section==TR("Antécédents")||section == TR("Ant�c�dents"))
     { while (*pt && *pt != '[')
       {var_name = "";
        libelle  = ""; rubrique=""; dateFin=""; code=""; comm=""; dateDeb=""; ald="";
        pt = CGestIni::Param_ExtraireNextValeurs(pt, libelle, &rubrique, &dateFin, &code, &comm, &dateDeb, &ald);
        // Nettoie les items de l'antécédent
        libelle.replace("\\,",",");        libelle.replace("\\;",";");        libelle.replace("\\[","[");
        libelle.replace("\\]","]");        comm.replace("\\,",",");           comm.replace("\\;",";");
        comm.replace("\\[","[");           comm.replace("\\]","]");

        setDatesToNewVersion(dateDeb, dateFin, dt_deb, dt_fin);

        Atcd = new Atcd_Element(rubrique, libelle, dt_deb , code, dt_fin, comm, ald) ;
        Atcd->setId(id);
        m_Atcd_Liste.append ( *Atcd );
        ++id;
       }  //  Fin while
       section="";
     }  // Fin if section == "Antécédents"

   // OBSTETRIQUE
   if (section == TR("Obstetrique") || section == TR("Obstétrique")||section == TR("Obst�trique"))
     { while (*pt && *pt != '[')
       { pt = CGestIni::Param_ExtraireNextValeurs(pt, libelle, &dt_str); // 18-03-2004
         if (libelle == TR("Grossesse"))
         { if (dt_str.length() && dt_str != TR("non"))
				{ m_Grossesse = TRUE;
				  m_DDR = QDate( atoi(dt_str.right(4)), atoi(dt_str.mid(3,2)), atoi(dt_str.left(2)) );
				}
           else m_Grossesse = FALSE;
         }
         if (dt_str.length() && libelle == TR("Allaitement") && dt_str==TR("oui"))  m_Allaitement = TRUE;
         else                                                                       m_Allaitement = FALSE;
         section="";
       } // Fin while
      } // Fin if section
 } // Fin while GotoNextSection

//  ATCD_LISTE::iterator it;
//  for (it=m_Atcd_Liste.begin(); it != m_Atcd_Liste.end();++it)
//  { qWarning( QString::number( (*it).m_Etat ) + "    " + (*it).m_Libelle ); }

 // Trie la liste des ATCD selon le nom de la rubrique
 qHeapSort( m_Atcd_Liste );

}
//--------------------------------- valid_DateStringOrEmptyString -------------------------------------------------------------
/*! \brief retourne une chaine vide si date invalide sinon une de la forme "dd-MM-yyyy"
 *  \param date_in = date dont il faut tester la validite
 *  \param format  = format de retour
 *  \return QString qui est vide si date invalide sinon la date avec le format d'entree
*/
QString Atcd_Code::valid_DateStringOrEmptyString(QString date_in, const QString &format  /* = "dd-MM-yyyy" */ )
{QString date    = CGenTools::NormaliseDateToIso(date_in) ;
 QDate   dateEnd = QDate::fromString(date,Qt::ISODate);
 if (dateEnd.isValid()) return dateEnd.toString(format);
 else                   return QString("");
}
//------------------------------ setNewState_FromOldState --------------------------------------------------
/*! \brief l'etat ancien stocké est 'Passé' 'Actif' le nouveau devant etre une date qui si invalide l'atcd est actif si valide il est termine a la date présente
 *  \param old_state = ancien etat a interpreter (il tient compte d'un eventuel etat date deja positionne)
 *  \return QString qui est le nouvel etat sous forme nouvelle (date valide ou invalide) selon actif ou inactif
*/
QString Atcd_Code::setNewState_FromOldState(const QString &old_state, QDate *date_to_set /* =0 */ )
{       QDate   dateEnd;
        if (old_state.upper()[0] == 'P')
           {dateEnd = QDate::currentDate().addDays (-1);   // on met une date de fin valide anterieure a ce jour ==> atcd inactif;
           }
        if (old_state.upper()[0] == 'A')
           {dateEnd = QDate();                             // on met une date de fin invalide donc non determinee  ==> atcd encore actif
           }
        else
           {QString date    = CGenTools::NormaliseDateToIso(old_state) ;
            dateEnd         = QDate::fromString(date,Qt::ISODate);
           }
       if (date_to_set) *date_to_set = dateEnd;
       if (dateEnd.isValid() ) return QString("");
       else                    return dateEnd.toString("dd-MM-yyyy");
}
//------------------------------ toOldIni --------------------------------------------------
/*! \brief Prépare le fichier de sauvegarde des ATCD dans l'ancien format dit "format INI".
 *
 * A savoir le terrain dans les versions 1 �  2.10 de MedinTux est sauvegardé en version INI. C'est grâce �  CGestIni qu'il est parsé. Le fichier de terrain comprend plusieurs paramètres sauvegardés les uns derrière les autres :
 * - les antécédents
 * - les variables
 * - le traitement en cours.
 *
 * Voici comment sont sauvegardés les antécédents :
 * [Antécédents]\n
 *     Libelle = rubrique, etat, code, commentaire, date(dd-MM-yyyy)\n
 *  ATTENTION : les libellés CIM10 peuvent contenir des crochets "[]" qui perturbent la sauvegarde et la lecture des ATCD. Ils seront remplacés par des accolades "{}".
 *
*/
QString Atcd_Code::toOldIni()
{ ATCD_LISTE::iterator it;
  QString retour = "";
  for (it=m_Atcd_Liste.begin(); it!=m_Atcd_Liste.end();++it)
  { if ((*it).m_Libelle.find("[") )
       { (*it).m_Libelle.replace("[", "{");
         (*it).m_Libelle.replace("]", "}");
       }

    retour +=  "   ";
    retour +=  (*it).m_Libelle                                + " = ";    // 0
    retour +=  (*it).m_Rubrique                               + " , ";    // 1
    retour +=  (*it).m_DateFin.toString("dd-MM-yyyy")         + " , ";    // 2
    retour +=  (*it).m_Code                                   + " , ";    // 3
    retour +=  (*it).m_Commentaire                            + " , ";    // 4
    retour +=  (*it).m_DateDeb.toString("dd-MM-yyyy")         + " , ";    // 5
    retour +=  (*it).m_ald                                    + "\r\n";   // 6
  } // fin for
  retour.prepend("[Antecedents]\r\n");
  retour += "[Obstetrique]\r\n";
  if (!m_Grossesse)  retour += "   " + TR("Grossesse")   + "  = " + TR("non") + "\r\n";
  else               retour += "   " + TR("Grossesse")   + "  = " + m_DDR.toString("dd-MM-yyyy") + "\r\n";
  if (m_Allaitement) retour += "   " + TR("Allaitement") + "  = " + TR("oui") + "\r\n";
  else               retour += "   " + TR("Allaitement") + "  = " + TR("non") + "\r\n";

  return retour;
}

//------------------------------ getDDR --------------------------------------------------
/*! \brief retourne la date des dernières règles si elle est renseignée.
*/
QDate Atcd_Code::getDDR()
{ return m_DDR; }

/*! \brief Calcul le terme de la grossesse �  la date souhaitée.
 *  \param QDate dt        Date pour le calcul (si =0 calcule �  partir de la date sytème).
 *  \return Le terme au format string du style : " XX SA XX Jours "
 *  Testée fonctionnelle
*/

//------------------------------ getTermeObstetrical --------------------------------------------------
/*! \brief Retourne une QString contenant le terme obstétrical en SA et jours, ou le message d'erreur "Non calculable".
*/
QString Atcd_Code::getTermeObstetrical(QDate dt /*=QDate()*/)
{ // Quelques contrôles
  if (!m_Grossesse) return QString::null;
  if (!m_DDR.isValid() ) return QString::null;
  if (dt.isNull()) {dt = QDate::currentDate(); }
  else { if (!dt.isValid() ) return QString::null; }

  // Calcul du terme
  int nbDays = m_DDR.daysTo(dt);
  int nbDaysLeft = nbDays % 7;
  int nbWeeks = (nbDays - nbDaysLeft ) / 7;
  QString ret = "";

  if (dt < m_DDR)
  { ret = TR("Non calculable"); }
  else if (nbWeeks > 45)
  { ret = TR("Non calculable"); }
  else
  { ret = QString(TR("%1 SA %2 Jours")).arg(nbWeeks).arg(nbDaysLeft); }
  return ret;
}
//------------------------------ createAtcdListView --------------------------------------------------
/*! \brief Construit un ListViewItem hiérarchique �  une colonne avec les ATCD prêt �  être affiché.
 *  \param CPrtQListViewItem : L'item qui recevra l'arborescence des antécédents.
 *  \sa CPrtQListViewItem, Dock_Menu
*/
void Atcd_Code::createAtcdListView()
{if (m_pAtcdListViewItem==0) return;
 //....... relever les items fils de cet item ......................
 QList <CPrtQListViewItem> list;
 CPrtQListViewItem * myChild = (CPrtQListViewItem*) m_pAtcdListViewItem->firstChild();
 while( myChild )
      {list.append(myChild);
       myChild = (CPrtQListViewItem*) myChild->nextSibling();
      }
 //....... effacer les items fils de cet item ......................
 for ( myChild = list.first(); myChild; myChild = list.next() ) delete myChild;
 createAtcdListView(*m_pAtcdListViewItem);
}

//------------------------------ createAtcdListView --------------------------------------------------
/*! \brief Construit un ListViewItem hiérarchique �  une colonne avec les ATCD prêt �  être affiché.
 *  \param CPrtQListViewItem : L'item qui recevra l'arborescence des antécédents.
 *  \sa CPrtQListViewItem, Dock_Menu
*/
void Atcd_Code::createAtcdListView(CPrtQListViewItem& retour)
{ m_pAtcdListViewItem       = &retour;
  m_pAtcdFatherListView     = retour.listView ();
  if (m_Atcd_Liste.count() == 0) return;

  setSort(CApp::SortByRubrique);
  sort();

  QMap<QString, CPrtQListViewItem* > MAP_pLVITEM;                 // on se cree une map pour matcher non de la rubrique et CPrtQListViewItem allant avec
//  bool isMed_Act, isChir_Act, isAllergie_Act;
  QString tmp      = "";
//  int id_Act       = -1;

  CPrtQListViewItem*  last_rub_Item = 0;                          // pointeur sur la rubrique pere precedente
  CPrtQListViewItem*   cur_rub_Item = 0;                          // pointeur sur la rubrique pere courante
  CPrtQListViewItem*      atcd_Item = 0;
  QString                   rubname = "";
  ATCD_LISTE::iterator           it = m_Atcd_Liste.at(0);
  for (it = m_Atcd_Liste.at(0); it != m_Atcd_Liste.end(); ++it)
      { //.................... nom de la rubrique (pere) de cet antecedent ....................................
        rubname = get_ATCD_Rubname((*it));                                                         // generer le nom de la rubrique de cet antecedent
        QMap<QString, CPrtQListViewItem*>::Iterator mit = MAP_pLVITEM.find ( rubname );            // rechercher dans la map si il n'y a pas deja un item pere a cet antecedent
        if ( mit == MAP_pLVITEM.end() )                                                            // si pas trouve
           { last_rub_Item = new CPrtQListViewItem( &retour, last_rub_Item, rubname);              // on cree la rubrique pere
             last_rub_Item->setAtcd(0);
             if (last_rub_Item)
                {last_rub_Item->setOpen( TRUE );                                                   // on l'ouvre
                 MAP_pLVITEM.insert ( rubname, last_rub_Item, FALSE );                             // on l'insere dans la map
                }
             cur_rub_Item = last_rub_Item;                                                         // c'est la nouvelle   qui sera le pere de l'antecedent
           }
        else cur_rub_Item = *mit;                                                                  // c'est celle trouvee dans la map qui sera le pere de l'antecedent
        //..................... inserer l'antecedent ...........................................................
                                            tmp  = (*it).m_Libelle;
        if ( (*it).isCIM10() )              tmp  = (*it).getLightCIM10();
        if ( (*it).m_DateDeb.isValid() )    tmp += " ("+(*it).m_DateDeb.toString("yyyy")+")";
        if ( (*it).m_DateFin.isValid() )    tmp += " ("+(*it).m_DateFin.toString("yyyy")+")";
        atcd_Item = new CPrtQListViewItem ( cur_rub_Item, atcd_Item, tmp);
        if (atcd_Item)
           {atcd_Item->set_ID_Rublist( (*it).getId() );
            atcd_Item->setPixmap(LV_TYPE, ald_sport_codeToPixmap((*it).m_ald));
            atcd_Item->setAtcd(&(*it));    // noter les donnees responsales de l'affichage
           }
  } // Fin FOR
  // Grossesse et Allaitement
  if (m_Grossesse)   { new CPrtQListViewItem ( &retour,0, TR("Grossesse en cours"));   }
  if (m_Allaitement) { new CPrtQListViewItem ( &retour,0, TR("Allaitement en cours")); }
}

//------------------------------ get_ATCD_Rubname --------------------------------------------------
/*! \brief Teste l'ATCD et renvoie le nom de la classe de familleGenre auquel il appartient.
 *  \param  Atcd_Element& pAtcd : reference sur l'antecedent
*/
QString Atcd_Code::get_ATCD_Rubname( Atcd_Element& pAtcd )
{return CGenTools::get_FamilleGenreClasse(pAtcd.m_Rubrique, pAtcd.m_Code );
}

//------------------------------ atcd_To_ListView --------------------------------------------------
/*! \brief Construit un ListViewItem hiérarchique �  une colonne avec les ATCD prêt �  être affiché.
 *  \param QListView : ListView qui doit etre initialisee.
 *  \sa atcd_To_ListView, atcd_Element_To_ListViewItem, updateFromListViewTerrain
*/
void Atcd_Code::atcd_To_ListView(QListView& retour)
{ if (m_Atcd_Liste.count() == 0){retour.clear(); return;}
  //CPrtQListViewItem* atcd_Item = 0;
  setSort(CApp::SortByRubrique);
  sort();
  retour.clear();

  // Récupère l'id_rubrique du premier ATCD
  ATCD_LISTE::iterator it;
  for (it = m_Atcd_Liste.begin(); it != m_Atcd_Liste.end(); ++it)
      {atcd_Element_To_ListViewItem(*it, &retour);
      } // Fin FOR
}
//------------------------------ ald_sport_codeToPixmap --------------------------------------------------
/*! \brief retourne le type d'icone correspondant au code ald_sport
 *  \param ald_sport_code : code ald_sport 'ALD' 'Sport' ''
 *  \return QPixmap correspondant (icone ALD ou sport)
*/
QPixmap Atcd_Code::ald_sport_codeToPixmap(const QString & ald_sport_code)
{      if (ald_sport_code.length())
          { if      (ald_sport_code[0]=='A')           return Theme::getIcon("16x16/ald_on.png");
            else if (ald_sport_code[0]=='S')           return Theme::getIcon("16x16/sport_on.png");
            else /*if (ald_sport_code[0]=='V')*/       return Theme::getIcon("16x16/vigilance_on.png");
          }
       else                                            return Theme::getIcon("16x16/ald_off.png") ;
}
//------------------------------ datesDebFinToPixmap --------------------------------------------------
/*! \brief retourne le type d'icone correspondant aux dates de debut et fin
 *  \param dateDeb : date de debut
 *  \param dateDeb : date de fin
 *  \return QPixmap correspondant (actif inactif)
*/
QPixmap Atcd_Code::datesDebFinToPixmap(const QDate &dateDeb, const QDate &dateFin)
{      if (dateDeb.isValid())
          {if (dateFin.isValid())
              {if (dateFin<QDate::currentDate()) return Theme::getIcon("16x16/listok.png");
               else                              return Theme::getIcon("16x16/warning.png");
              }
           else                                  return Theme::getIcon("16x16/warning.png");
          }
       return Theme::getIcon("16x16/listok.png");
}
//------------------------------ atcd_Element_To_ListViewItem --------------------------------------------------
/*! \brief Ajoute a une listView un item affichant un ATCD.
 *
 *  \sa DrTux::atcd_To_ListView()
*/
void Atcd_Code::atcd_Element_To_ListViewItem(Atcd_Element& pAtcd_Element, QListView* pQListView)
{      CPrtQListViewItem* pCPrtQListViewItem  = new CPrtQListViewItem ( pQListView, 0,  pAtcd_Element.m_Libelle);       // 0 Libelle
       pCPrtQListViewItem->set_ID_Rublist( pAtcd_Element.getId() );
       pCPrtQListViewItem->setText( LV_TYPE,  pAtcd_Element.m_Rubrique);                                       // 1 famille Genre
       pCPrtQListViewItem->setText( LV_DFIN,  pAtcd_Element.m_DateFin.toString("dd-MM-yyyy"));                 // 2 etat Actif/gueri
       pCPrtQListViewItem->setText( LV_COMM,  pAtcd_Element.m_Commentaire);                                    // 3 commentaire
       pCPrtQListViewItem->setText( LV_DDEB,  pAtcd_Element.m_DateDeb.toString("dd-MM-yyyy"));                 // 4 date
       pCPrtQListViewItem->setText( LV_ALSP,  pAtcd_Element.m_ald);                                            // 5 ald
       pCPrtQListViewItem->setText( LV_CODE,  pAtcd_Element.m_Code);                                           // 6 code
       //............. les icones .........................
       pCPrtQListViewItem->setPixmap(LV_TYPE, ald_sport_codeToPixmap(pAtcd_Element.m_ald) );
       pCPrtQListViewItem->setPixmap(LV_DDEB, datesDebFinToPixmap(pAtcd_Element.m_DateDeb, pAtcd_Element.m_DateFin) );
       pCPrtQListViewItem->setPixmap(LV_NAME, Atcd_Element::getPixmap(pAtcd_Element.m_Code) );
}

//------------------------------ addATCD_Textuel --------------------------------------------------
/*! \brief Ajoute �  la liste actuelle des ATCD un ATCD en mode textuel.
 *  Fait apparaître le Widget d'ajout DlgAtcd_txt.
*/
void Atcd_Code::addATCD_Textuel(QWidget *parent , int sendModifMessage /*=Atcd_Code::sendModifMessage*/)
{// Appel le Widget de saisie ATCD en texte libre
 DlgAtcd_txt* pDlgAtcd_txt = new DlgAtcd_txt(parent);
 if (!pDlgAtcd_txt) return;
 pDlgAtcd_txt->setFamilyGenre("");
 pDlgAtcd_txt->exec();
 if (pDlgAtcd_txt->result() == QDialog::Accepted)
    { // Récupère les données du widget set_Atcd_Element(rubrique, libelle,  dt,  code, etat,  commentaire, ald,  -1);
      QString libelle, commentaire, dateDebIso,dateFinIso, rubrique;
      QDate dt_deb, dt_fin ;
      pDlgAtcd_txt->getInfos(libelle, rubrique, dateDebIso, dateFinIso, commentaire);
      if (dateDebIso != "") dt_deb = QDate::fromString(dateDebIso, Qt::ISODate);
      else                  dt_deb = QDate();
      if (dateFinIso != "") dt_fin = QDate::fromString(dateFinIso, Qt::ISODate);
      else                  dt_fin = QDate();
      int id_ATCD                  = m_Atcd_Liste.count()+1;
      Atcd_Element elementAdded    = Atcd_Element(rubrique, libelle, dt_deb, "", dt_fin, commentaire, pDlgAtcd_txt->getSate_Ald_Sport());
      elementAdded.setId( id_ATCD );
      ATCD_LISTE::iterator it = m_Atcd_Liste.append (elementAdded);
      m_pLastElementAdded     = &(*it);          // noter le dernier element ajoute
      if (sendModifMessage) emitATCD_Changed();
    }
 else
    {m_pLastElementAdded     = 0;
    }
 if (pDlgAtcd_txt) delete pDlgAtcd_txt;
}

//------------------------------ get_lastElementAdded --------------------------------------------------
/*! \brief retourne une copie du dernier element ajoute
 *
*/
Atcd_Element* Atcd_Code::get_lastElementAdded()
{return m_pLastElementAdded;
}

//------------------------------ getAtcd_byID --------------------------------------------------
/*! \brief Renvoie l'antécédent dont l'id est celle passée en paramètre.
*/
Atcd_Element* Atcd_Code::getAtcd_byID(int idAtcd)
{if (idAtcd == -1) return 0;
 ATCD_LISTE::iterator it;
 for (it=m_Atcd_Liste.begin(); it != m_Atcd_Liste.end();++it)
     { if ((*it).getId() == idAtcd) break;
     }
 return &(*it);
}

//------------------------------ viewATCD --------------------------------------------------
/*! \brief Affiche les renseignements d'un ATCD sans possiblité de modification.
 *  Fait apparaître le Widget de visualisation des ATCD.
*/
void Atcd_Code::viewATCD(QWidget *parent ,int id_ATCD)
{// Trouve l'ATCD selon l'id passée en paramètre
 ATCD_LISTE::iterator it;
 for (it=m_Atcd_Liste.begin(); it != m_Atcd_Liste.end();++it)
 { if ((*it).getId() == id_ATCD) break; }

 // Appel le Widget de saisie ATCD en texte libre
 DlgAtcd_txt* pDlgAtcd_txt = new DlgAtcd_txt(parent);
 if (!pDlgAtcd_txt) return;
 pDlgAtcd_txt->setATCD_toView( this, &(*it) );
 pDlgAtcd_txt->exec();
 if (pDlgAtcd_txt) delete pDlgAtcd_txt;
}

//------------------------------ setRubrique --------------------------------------------------
/*! \brief Modifie la famille-genre de l'antécédent passé en paramètre par la rubrique passée en paramètre.
*/
void Atcd_Code::setRubrique(Atcd_Element* pAtcd, QString& rubrique,  int sendModifMessage /*= Atcd_Code::sendModifMessage*/)
{ if (!pAtcd) return;
 pAtcd->m_Rubrique = rubrique;
 if (sendModifMessage) emitATCD_Changed();
}

//------------------------------ setRubrique --------------------------------------------------
/*! \brief Modifie l'ALD de l'antécédent passé en paramètre par l'ald passée en paramètre.
*/
void Atcd_Code::setALD(Atcd_Element* pAtcd, const QString& ald, int sendModifMessage /*=Atcd_Code::sendModifMessage*/)
{ if (!pAtcd) return;
 pAtcd->m_ald = ald;
 //atcd_Item = new CPrtQListViewItem ( &retour, atcd_Item,  (*it).m_Libelle);        // Libelle
 // recuperer les donnees responsales de l'affichage
 CPrtQListViewItem* atcd_Item = getListViewItemFromAtcd(pAtcd);
 if (atcd_Item)
    {if (pAtcd->m_ald.length()) atcd_Item->setPixmap(0, Theme::getIcon("16x16/ald_on.png") );
     else                       atcd_Item->setPixmap(0, Theme::getIcon("16x16/ald_off.png") );
    }
 if (sendModifMessage) emitATCD_Changed();
}

//------------------------------ getListViewItemFromAtcd --------------------------------------------------
/*! \brief recuperer le LisViewItem responsable de l'affichage de l'ATCD donne en entree
*/
CPrtQListViewItem* Atcd_Code::getListViewItemFromAtcd(Atcd_Element* pAtcd)
{
  QListViewItemIterator it( m_pAtcdFatherListView );
  while ( it.current() )
        {CPrtQListViewItem *item = (CPrtQListViewItem*) it.current();
         if (item->getAtcd()==pAtcd) return item;
         ++it;
        }
  return 0;
}

//------------------------------ setGrossesse --------------------------------------------------
/*! \brief Modifie l'état de la grossesse.
*/
void Atcd_Code::setGrossesse(bool etat)
{m_Grossesse = etat;
 emitATCD_Changed();
}

//------------------------------ setAllaitement --------------------------------------------------
/*! \brief Modifie l'état de l'allaitement.
*/
void Atcd_Code::setAllaitement(bool etat)
{m_Allaitement = etat;
 emitATCD_Changed();
}

//------------------------------ setDDRTo --------------------------------------------------
/*! \brief Modifie l'état de la grossesse.
*/
void Atcd_Code::setDDRTo(QDate dt)
{m_DDR = dt;
 emitATCD_Changed();
}

//------------------------------ modifyDateDeb --------------------------------------------------
/*! \brief Affiche le widget calendar pour modifier la date debut de l'antécédent passé en paramètre.
*/
void Atcd_Code::modifyDateDeb(QWidget *parent, Atcd_Element* pAtcd,  int sendModifMessage /*= Atcd_Code::sendModifMessage*/)
{ if (!pAtcd) return;
  Dlg_Calendar* dlg  = new Dlg_Calendar(parent, "Calendar_Dial", FALSE);
  if (dlg==0) return;
  dlg->setCaption(TR("Date de debut de l'antécédent"));
  dlg->move ( QCursor::pos().x(), QCursor::pos().y() );
  QDate dt_deb = pAtcd->m_DateDeb;
  if ( ! dt_deb.isValid() ) dt_deb = QDate::currentDate();

  dlg->setDate(dt_deb);
  dlg->setComboAnOnDate_Offset(dt_deb, 25,0);
  dlg->SelectDateAll();

  if (dlg->exec()== QDialog::Accepted )
     { setDateDeb(pAtcd, dlg->getDate(), sendModifMessage);
     }
 if (dlg) delete dlg;
}
//------------------------------ modifyDateFin --------------------------------------------------
/*! \brief Affiche le widget calendar pour modifier la date de fin de l'antécédent passé en paramètre.
*/
void Atcd_Code::modifyDateFin(QWidget *parent, Atcd_Element* pAtcd,  int sendModifMessage /*= Atcd_Code::sendModifMessage*/)
{ if (!pAtcd) return;
  Dlg_Calendar* dlg  = new Dlg_Calendar(parent, "Calendar_Dial", FALSE);
  if (dlg==0) return;
  dlg->setCaption(TR("Date de fin de l'antécédent"));
  dlg->move ( QCursor::pos().x(), QCursor::pos().y() );
  QDate dt_fin = pAtcd->m_DateFin;
  if ( ! dt_fin.isValid() ) dt_fin = QDate::currentDate();

  dlg->setDate(dt_fin);
  dlg->setComboAnOnDate_Offset(dt_fin, 25,0);
  dlg->SelectDateAll();

  if (dlg->exec()== QDialog::Accepted )
     { setDateFin(pAtcd, dlg->getDate(), sendModifMessage);
     }
 if (dlg) delete dlg;
}
//------------------------------ setDateDeb --------------------------------------------------
/*! \brief modifie la date de debut et emet le signal de changement.
*/
void Atcd_Code::setDateDeb(Atcd_Element* pAtcd, const QDate &date,  int sendModifMessage /*= Atcd_Code::sendModifMessage*/)
{if (!pAtcd) return;
 pAtcd->m_DateDeb = date;
 if (sendModifMessage==Atcd_Code::sendModifMessage) emitATCD_Changed();
}
//------------------------------ setDateFin --------------------------------------------------
/*! \brief  modifie la date de fin et emet le signal de changement.
*/
void Atcd_Code::setDateFin(Atcd_Element* pAtcd, const QDate &date,  int sendModifMessage /*= Atcd_Code::sendModifMessage*/)
{if (!pAtcd) return;
 pAtcd->m_DateFin = date;
 if (sendModifMessage==Atcd_Code::sendModifMessage) emitATCD_Changed();
}
//------------------------------ deleteDateDeb --------------------------------------------------
/*! \brief efface la date de debut de l'atcd passe en parametre.
*/
void Atcd_Code::deleteDateDeb(Atcd_Element* pAtcd,  int sendModifMessage /*= Atcd_Code::sendModifMessage*/)
{ if (!pAtcd) return;
  if (pAtcd->m_DateDeb.isValid())
     {pAtcd->m_DateDeb = QDate();
      if (sendModifMessage==Atcd_Code::sendModifMessage) emitATCD_Changed();
     }
}
//------------------------------ deleteDateFin --------------------------------------------------
/*! \brief efface la date de debut de l'atcd passe en parametre.
*/
void Atcd_Code::deleteDateFin(Atcd_Element* pAtcd,  int sendModifMessage /*= Atcd_Code::sendModifMessage*/)
{ if (!pAtcd) return;
  if (pAtcd->m_DateFin.isValid())
     {pAtcd->m_DateFin = QDate();
      if (sendModifMessage==Atcd_Code::sendModifMessage) emitATCD_Changed();
     }
}
//------------------------------ addATCD --------------------------------------------------
/*! \brief Ajoute �  la liste actuelle des ATCD un ATCD en mode textuel.
 *  Fait apparaître le Widget d'ajout DlgAtcd_txt.
*/
void Atcd_Code::addATCD(QWidget *parent, const QString &libelle, const QString &code, const QString &familyGenre, const QString &comment, int sendModifMessage /*=Atcd_Code::sendModifMessage*/)
{// Appel le Widget de saisie ATCD en texte libre
 DlgAtcd_txt* pDlgAtcd_txt = new DlgAtcd_txt(parent);  if (!pDlgAtcd_txt) return;
 pDlgAtcd_txt->setToUpdate(TRUE);
 pDlgAtcd_txt->m_LineEdit_Libelle->setText(libelle);
 pDlgAtcd_txt->setFamilyGenre(familyGenre);
 pDlgAtcd_txt->m_LineEdit_Commentaire->setText(comment);
 pDlgAtcd_txt->m_LineEdit_Commentaire->setFocus();
 pDlgAtcd_txt->exec();
 if (pDlgAtcd_txt->result() == QDialog::Accepted)
    { // Récupère les données du widget set_Atcd_Element(rubrique, libelle,  dt,  code, etat,  commentaire, ald,  -1);
      QString libelle, commentaire, dateDebIso, dateFinIso, rubrique;
      QDate dt_deb ;
      QDate dt_fin ;
      pDlgAtcd_txt->getInfos(libelle, rubrique, dateDebIso, dateFinIso, commentaire);
      if (dateDebIso.length()) dt_deb = QDate::fromString(dateDebIso, Qt::ISODate);
      else                     dt_deb = QDate();
      if (dateFinIso.length()) dt_fin = QDate::fromString(dateFinIso, Qt::ISODate);
      else                     dt_fin = QDate();
      int id_ATCD                  = m_Atcd_Liste.count()+1;
      Atcd_Element elementAdded    = Atcd_Element(rubrique, libelle, dt_deb, code, dt_fin, commentaire, pDlgAtcd_txt->getSate_Ald_Sport());
      elementAdded.setId( id_ATCD );
      ATCD_LISTE::iterator it = m_Atcd_Liste.append (elementAdded);
      m_pLastElementAdded     = &(*it);          // noter le dernier element ajoute
      if (sendModifMessage) emitATCD_Changed();
    }
 else
    {m_pLastElementAdded     = 0;
    }
 if (pDlgAtcd_txt) delete pDlgAtcd_txt;
}

//------------------------------ addATCD_CIM10 --------------------------------------------------
/*! \brief Ajoute �  la liste actuelle des ATCD codés en CIM10.
 *  Fait apparaître le Widget d'ajout qui se trouve dans DrTux.
 *  \sa DrTux::CodageCim10All()
*/
void Atcd_Code::addATCD_CIM10(int tabToSet /*=-1*/)
{QString       strlistCode = "";
 QString result = G_pCApp->m_pDrTux->CodageCim10(DrTux::GestionATCD, strlistCode, tabToSet);
}
//------------------------------ addATCD_Allergie --------------------------------------------------
/*! \brief Ajoute �  la liste actuelle des ATCD un ATCD allergique.
 *  Fait apparaître le Widget d'ajout DlgAllergie.
*/
void Atcd_Code::addATCD_Allergie()
{QString       strlistCode = "";
 QString result = G_pCApp->m_pDrTux->CodageCim10(DrTux::GestionATCD, strlistCode, 2);
}

//------------------------------ modifyAtcd --------------------------------------------------
/*! \brief Affiche le widget de modification de l'antécédent selon son type et gère sa modification.
 *  \todo gestion des Atcd Texte Libre
*/
void Atcd_Code::modifyAtcd(QWidget *parent, Atcd_Element* pAtcd,  int sendModifMessage /*= Atcd_Code::sendModifMessage*/ )
{ if (!pAtcd) return;
  // ATCD CIM10
  if (pAtcd->isCIM10())
     {addATCD_CIM10(0);
      if (sendModifMessage==Atcd_Code::sendModifMessage) emitATCD_Changed();
     } // Fin modifyCIM10
  // ATCD ALLERGIQUE
  else if (pAtcd->isCisp() )
     {addATCD_CIM10(4);
      if (sendModifMessage==Atcd_Code::sendModifMessage) emitATCD_Changed();
     }
  else if (pAtcd->is_ATC() )
     {addATCD_CIM10(2);
      if (sendModifMessage==Atcd_Code::sendModifMessage) emitATCD_Changed();
     }
  else if (pAtcd->isAllergie() )
     {addATCD_CIM10(2);
      if (sendModifMessage==Atcd_Code::sendModifMessage) emitATCD_Changed();
     }
  // ATCD TEXTE LIBRE
  else
  { // Appel le Widget de saisie ATCD en texte libre
    DlgAtcd_txt* pDlgAtcd_txt = new DlgAtcd_txt(parent);  if (!pDlgAtcd_txt) return;
    pDlgAtcd_txt->setATCD_toView( this, pAtcd);
    pDlgAtcd_txt->setToUpdate(TRUE);
    pDlgAtcd_txt->exec();
    if (pDlgAtcd_txt->result() == QDialog::Accepted)
       { // Récupère les données du widget
         QString libelle, commentaire, dateDebIso, dateFinIso, rubrique;
         QDate dt_deb, dt_fin ;
         pDlgAtcd_txt->getInfos(libelle, rubrique, dateDebIso, dateFinIso, commentaire);
         if (dateDebIso.length()) dt_deb = QDate::fromString(dateDebIso, Qt::ISODate);
         else                     dt_deb = QDate();
         if (dateFinIso.length()) dt_fin = QDate::fromString(dateFinIso, Qt::ISODate);
         else                     dt_fin = QDate();
         pAtcd->set_Atcd_Element(rubrique, libelle, dt_deb, "", dt_fin, commentaire, pDlgAtcd_txt->getSate_Ald_Sport(), pAtcd->getId());
       }
    if (pDlgAtcd_txt) delete pDlgAtcd_txt;
    if (sendModifMessage==Atcd_Code::sendModifMessage) emitATCD_Changed();
  }
}

//------------------------------ changeDDR --------------------------------------------------
/*! \brief Affiche le widget calendar pour modifier la date des dernières règles.
*/
void Atcd_Code::changeDDR(QWidget *parent)
{ Dlg_Calendar* dlg  = new Dlg_Calendar(parent, "Calendar_Dial", FALSE);
 if (dlg==0) return;
 dlg->setCaption(TR("Date des dernières règles"));
 dlg->move ( QCursor::pos().x(), QCursor::pos().y() );
 if (m_DDR.isValid() )
    { dlg->setDate(m_DDR);
      dlg->SelectDateAll();
    }
 if (dlg->exec()== QDialog::Accepted )
    { m_DDR       = dlg->getDate();
      m_Grossesse = TRUE;
      emit ATCD_Changed();
    }
 if (dlg) delete dlg;
}

//------------------------------ changeDateFin --------------------------------------------------
/*! \brief Change la date de fin de l'antécédent, passé en paramètre.
*/
void Atcd_Code::changeDateFin(QWidget *parent, Atcd_Element* pAtcd)
{  if (!pAtcd) return;
   QDate dt;
   if (pAtcd->m_DateFin.isValid())  dt = pAtcd->m_DateFin;
   else                             dt = QDate::currentDate();
   Dlg_Calendar          *dlg  = new Dlg_Calendar(parent, "Calendar_Dial", FALSE);
   if (dlg ==0)                                                   return;
   dlg->setCaption(tr("Choix dans la date de fin "));
   dlg->setDate  (dt);

   dlg->hideHeure();
   if (dlg->exec()== QDialog::Accepted )
      {pAtcd->m_DateFin = dlg->getDate();
       delete dlg;
       emitATCD_Changed();
       return;
      }
   delete dlg;
}

//------------------------------ setEtat --------------------------------------------------
/*! \brief Change l'état de l'antécédent, passé en paramètre,
*/
/*
void Atcd_Code::setEtat(Atcd_Element* pAtcd, QString etat,  int sendModifMessage / *= Atcd_Code::sendModifMessage* /)
{ if (!pAtcd)     return;
  pAtcd->m_Etat = etat;
  if (sendModifMessage==Atcd_Code::sendModifMessage) emitATCD_Changed();
}
*/
//------------------------------ changeCommentaire --------------------------------------------------
/*! \brief Appel un widget pour changer le commentaire de l'antécédent.
*/
void Atcd_Code::changeCommentaire(Atcd_Element* pAtcd,  int sendModifMessage /*= Atcd_Code::sendModifMessage*/)
{ if (CGenTools::DialogGetString(0, TR("Saisir un commentaire (mode texte libre)") , pAtcd->m_Commentaire) == QDialog::Accepted)
     {  if (sendModifMessage==Atcd_Code::sendModifMessage) emitATCD_Changed();
     }
}

//------------------------------ eraseAtcd --------------------------------------------------
/*! \brief Change l'état de l'antécédent, passé en paramètre, de Actif<->Guéri.
*/
void Atcd_Code::eraseAtcd(Atcd_Element* pAtcd,  int sendModifMessage /* = Atcd_Code::sendModifMessage*/)
{ if (!pAtcd) return;
  // Trouve l'Atcd dans la liste
  m_Atcd_Liste.remove( *pAtcd );
 if (sendModifMessage==Atcd_Code::sendModifMessage) emitATCD_Changed();
}

//--------------------------------- GetSexe_FMI--------------------------------------------------
/*! \brief recherche la lettre du sexe 'F' 'M' 'I' a partir de la base de donnees
*/
QChar Atcd_Code::GetSexe_FMI_FromBD()
{
 QString  s = GetSexe_FromBD();
 return   s[0].upper();
}

//--------------------------------- GetSexe--------------------------------------------------
/*! \brief retourne la lettre du sexe 'F' 'M' 'I'
*/
QChar Atcd_Code::GetSexe()
{return m_sexe;
}

//--------------------------------- GetSexe_FromBD--------------------------------------------------
/*! \brief recherche le sexe a partir de la base de donnees
*/
QString Atcd_Code::GetSexe_FromBD()
{QVariant qvar;
 m_pCMoteurBase->GetFieldValue(    m_pCMoteurBase->m_DOSS_IDENT_TBL_NAME,
                                   m_pCMoteurBase->m_DOSS_IDENT_SEX,
                                   m_pCMoteurBase->m_DOSS_IDENT_REF_PK ,
                                   G_pCApp->m_ID_Doss,
                                   &qvar);
 return qvar.toString();
}

//------------------------------ updateFromListViewTerrain --------------------------------------------------
/*! \brief met a jour a partir de la listview du terrain
*/
void Atcd_Code::updateFromListViewTerrain(QListView* pQListView, int sendModifMessage /* = Atcd_Code::sendModifMessage*/)
{       m_Atcd_Liste.clear();
        QListViewItemIterator it( pQListView );
        while ( it.current() )
              {
               QListViewItem *pQListViewItem = it.current();
               QString dateDeb  = pQListViewItem->text(LV_DDEB);
               QString dateFin  = pQListViewItem->text(LV_DFIN);
               QDate     dt_deb = QDate();
               QDate     dt_fin = QDate();

               setDatesToNewVersion(dateDeb, dateFin, dt_deb, dt_fin);

               Atcd_Element         elementAdded(pQListViewItem->text(LV_TYPE),
                                                 pQListViewItem->text(LV_NAME),
                                                 dt_deb,
                                                 pQListViewItem->text(LV_CODE),
                                                 dt_fin,
                                                 pQListViewItem->text(LV_COMM),
                                                 pQListViewItem->text(LV_ALSP),
                                                 m_Atcd_Liste.count()+1
                                                );
               m_Atcd_Liste.append (elementAdded);
               ++it;
              }
        if (sendModifMessage==Atcd_Code::sendModifMessage) emitATCD_Changed();
}

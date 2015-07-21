/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/


#define TR                        QObject::tr
#define DOCK_MENU_SECTION_INI     "Dock_Rapid_Access"
#define NBMAXSOUSMENU             6

#include "../../../MedinTuxTools/CGestIni.h"
#include "../../../MedinTuxTools/CPrtQListViewItem.h"
#include "../../MedinTuxTools/Theme.h"  // Gestion du thème de l'appli
#include "CMDI_ChoixPatient.h"

#include "C_Atcd.h"
#include "C_Var.h"
#include <qimage.h>
#include <qprocess.h>
#include <qtimer.h>
#include <qeventloop.h>

#include "CApp.h"
#include "drtux.h"
#include "CMDI_Observation.h"
#include "CMDI_Prescription.h"
//#include "CMDI_Documents.h"
#include "CMDI_Ident.h"



class CGenTools;


/*! \class Dock_Menu
 *  \brief Classe construisant le menu d'accès rapide.
 * Le menu d'accès rapide permet de gérer tous les aspects des antécédents. Il présente selon le paramétrage utilisateur (stocké dans la base de données) les observations, les prescriptions, les documents multimédias et les différents terrains enregistrés. L'affichage est limité �  5 items par branche.
 * Plusieurs classes sont nécessaires �  Dock_Menu :
 * - Atcd_Code : qui gère les antécédents et doit toujours resté synchro avec FormRubTerrain.
 * - Var_Code : classe en cours de réécriture au moment de la V2.10. Gère les variables du terrain. N'est pas encore synchronisée avec FormRubTerrain.
 * - CPrtQListViewItem : pour la mise en forme de la zone de liste
 * - Theme : pour la thémétisation de l'affichage des icones
 * - ThemePopup : pour la thématisation de l'affichage des PopupMenus
 * - CApp : pour les droits, les paramètres, le glossaire et autres.
 *
 * Il convient initialement d'instancier la classe puis de lui définir ses paramètres grâce �  la fonction setParams.
 *
*/

/*! \var int Dock_Menu::m_ClickBehavior
 * Selon les paramètres utilisateur, lorsque l'on clique sur un nom de rubrique (Antécédents, Observations, Prescriptions...) se comporte de la sorte :
 * - ouvre la fenêtre de la rubrique dans l'espace de travail si == 0
 * - Déploie ou rétracte la branche cliquée si == 1
*/

/*! \var bool Dock_Menu::m_ParentInBold
 * Selon les paramètres utilisateur, met ou non en gras les noms de rubriques (Antécédents, Observations...)
*/

/*! \var bool Dock_Menu::m_OpenATCD
 * Selon les paramètres utilisateur, déploie ou rétracte systématiquement après sauvegarde ou �  l'ouverture la branche antécédents.
*/
/*! \var bool Dock_Menu::m_OpenTerrain
 * Selon les paramètres utilisateur, déploie ou rétracte systématiquement après sauvegarde ou �  l'ouverture la branche terrains.
*/

/*! \var bool Dock_Menu::m_OpenObs
 * Selon les paramètres utilisateur, déploie ou rétracte systématiquement après sauvegarde ou �  l'ouverture la branche observations.
*/

/*! \var bool Dock_Menu::m_OpenPrescr
 * Selon les paramètres utilisateur, déploie ou rétracte systématiquement après sauvegarde ou �  l'ouverture la branche prescriptions.
*/

/*! \var bool Dock_Menu::m_OpenDoc
 * Selon les paramètres utilisateur, déploie ou rétracte systématiquement après sauvegarde ou �  l'ouverture la branche docuements.
*/

/*! \var bool Dock_Menu::m_OpenIdentity
 * Selon les paramètres utilisateur, déploie ou rétracte systématiquement après sauvegarde ou �  l'ouverture la branche coordonnées.
*/

/*! \var bool Dock_Menu::m_OpenVars
 * Selon les paramètres utilisateur, déploie ou rétracte systématiquement après sauvegarde ou �  l'ouverture la branche variables.
*/
/*! \var bool Dock_Menu::m_ShowDoc
 * Selon les paramètres utilisateur, affiche ou non la branche documents.
*/

/*! \var bool Dock_Menu::m_ShowPrescr
 * Selon les paramètres utilisateur, affiche ou non la branche prescriptions.
*/

/*! \var bool Dock_Menu::m_ShowObs
 * Selon les paramètres utilisateur, affiche ou non la branche observations.
*/

/*! \var bool Dock_Menu::m_ShowIdentity
 * Selon les paramètres utilisateur, affiche ou non la branche coordonnées.
*/

/*! \var bool Dock_Menu::m_ShowTerrain
 * Selon les paramètres utilisateur, affiche ou non la branche terrains.
*/

/*! \var bool Dock_Menu::m_ShowGlossaire
 * Selon les paramètres utilisateur, permet ou non l'utilisation du glossaire.
*/

/*! \var int Dock_Menu::m_DefaultFontSize
 * Taille de la police par défaut de la zone de liste.
*/

/*! \var bool Dock_Menu::m_Droits_Modif_ATCD
 * Droits utilisateur.
*/

/*! \var bool Dock_Menu::m_Droits_Modif_TTTFond
 * Droits utilisateur.
*/

/*! \var bool Dock_Menu::m_Droits_Modif_Variables
 * Droits utilisateur.
*/

/*! \var bool Dock_Menu::m_Droits_View_TTTFond
 * Droits utilisateur.
*/

/*! \var bool Dock_Menu::m_Droits_View_Variables
 * Droits utilisateur.
*/

/*! \var bool Dock_Menu::m_Droits_View_Prescriptions
 * Droits utilisateur.
*/

/*! \var bool Dock_Menu::m_Droits_View_Observations
 * Droits utilisateur.
*/

/*! \var bool Dock_Menu::m_Droits_View_Documents
 * Droits utilisateur.
*/

/*! \var bool Dock_Menu::m_Droits_View_Identite
 * Droits utilisateur.
*/

/*! \var bool Dock_Menu::m_Droits_View_Courriers
 * Droits utilisateur.
*/

/*! \var bool Dock_Menu::m_Droits_View_Certificats
 * Droits utilisateur.
*/

/*! \var bool Dock_Menu::m_Droits_View_ATCD
 * Droits utilisateur.
*/

/*! \var bool Dock_Menu::m_Droits_Create_Prescriptions
 * Droits utilisateur.
*/

/*! \var bool Dock_Menu::m_Droits_Create_Observations
 * Droits utilisateur.
*/

/*! \var bool Dock_Menu::m_Droits_Create_Documents
 * Droits utilisateur.
*/

/*! \var bool Dock_Menu::m_Droits_Create_Certificats
 * Droits utilisateur.
*/

/*! \var bool Dock_Menu::m_Droits_Create_Courriers
 * Droits utilisateur.
*/

/*! \var bool Dock_Menu::m_Droits_Create_ATCD
 * Droits utilisateur.
*/

/*! \var QString Dock_Menu::m_PkDocTerrain
 * Clé primaire (m_PrimKey) du document de terrain en cours d'affichage. Utilisé pour la synchronisation des terrains, la lecture (éviter de RElire) et la sauvegarde.
*/

/*! \var QString Dock_Menu::m_SelectedFile
 * Utilisée pour récupérer le nom du fichier sélectionné dans le popup de glossaire de cette classe.
 * \sa Slot_FileSelected , OnMenuActivated.
*/

/*! \var bool Dock_Menu::m_IsFeminin
 * Sexe féminin ou non ? Est défini �  l'initialisation et aucours des changements d'identités.
*/

/*! \var QString Dock_Menu::m_Identity
 * Sauvegarde le nom, prénom et âge dans un string affiché dans le label d'identité. Utilisé pour ajouter l'état de grossesse (ajoute le terme) ou non.
*/

/*! \var int Dock_Menu::m_Id_RubList_ToUseForThisTerrain
 * Sauvegarde de l'id dans la pRubList du document de terrain.
*/

/*! \var CPrtQListViewItem* Dock_Menu::m_Item_Coordonnees
 * Branche principale pour les coordonnées au sein de la zone de liste.
 * \sa CPrtQListViewItem
*/

/*! \var CPrtQListViewItem* Dock_Menu::m_Item_ATCD
 * Branche principale pour les antécédents au sein de la zone de liste.
 * \sa CPrtQListViewItem
*/

/*! \var CPrtQListViewItem* Dock_Menu::m_Item_Vars
 * Branche principale pour les variables au sein de la zone de liste.
 * \sa CPrtQListViewItem
*/

/*! \var CPrtQListViewItem* Dock_Menu::m_Item_Observations
 * Branche principale pour les observations au sein de la zone de liste.
 * \sa CPrtQListViewItem
*/

/*! \var CPrtQListViewItem* Dock_Menu::m_Item_Prescriptions
 * Branche principale pour les prescriptions au sein de la zone de liste.
 * \sa CPrtQListViewItem
*/

/*! \var CPrtQListViewItem* Dock_Menu::m_Item_Documents
 * Branche principale pour les docuements au sein de la zone de liste.
 * \sa CPrtQListViewItem
*/

/*! \var CPrtQListViewItem* Dock_Menu::m_Item_Terrains
 * Branche principale pour les terrains au sein de la zone de liste.
 * \sa CPrtQListViewItem
*/

/*! \var QString Dock_Menu::m_User
 * Contient le nom de l'utilisateur courant.
*/

/*! \var QString Dock_Menu::m_DossPrenom
 * Contient le prénom associé au dossier.
*/

/*! \var QString Dock_Menu::m_DossNom
 * Contient le nom associé au dossier.
*/

/*! \var QString Dock_Menu::m_Id_Doss
 * Contient la clé primaire du dossier.
*/

/*! \var QString Dock_Menu::m_GUID
 * Contient le GUID du dossier.
*/

/*! \var RUBREC_LIST* Dock_Menu::m_pRubList
 * Contient la liste des documents du dossier.
 * \sa CRubRecord
*/

/*! \var CMoteurBase* Dock_Menu::m_pCMoteurBase
 * Pointeur vers la base de données patient et ses fonctions.
*/

/*! \var QPixmap Dock_Menu::m_PixMap_Femme
 * Pixmap pour les femmes. Défini dans init().
*/

/*! \var QPixmap Dock_Menu::m_PixMap_Homme
 * Pixmap pour les hommes. Défini dans init().
*/

/*! \var QPixmap Dock_Menu::m_PixMap_Parturiente
 * Pixmap pour les femmes enceintes. Défini dans init().
*/

/*! \var QStringList Dock_Menu::m_QStringList_ATCD
 * non utilisé.
*/

/*! \var Atcd_Code* Dock_Menu::m_pAtcd_Code
 * Pointeur vers la classe gestionnaire des antécédents.
 * \sa CApp::m_pAtcd_Code
*/

/*! \var bool Dock_Menu::m_Notes_Changed
 * Renseigne sur l'état de modification de la note.
*/

/*! \var Var_Code* Dock_Menu::m_pVar_Code
 * Non documenté en V2.10
*/

/*! \var ThemePopup* Dock_Menu::m_Popup
 * Popup général.
*/

/*! \var Atcd_Element* Dock_Menu::m_pAtcd_Element_Selected
 * Antécédent sélectionné au moment de l'appel d'un PopupMenu
*/

/*! \var int Dock_Menu::m_IsModifiable
 * Renseigne sur les possibilité de modification du dossier en cours (est-il verrouillé ou non).
*/

/*! \var bool Dock_Menu::m_Droits_All
 * TRUE si l'utilisateur a tous les droits.
*/

/*! \var QString Dock_Menu::m_TypeATCD_Selectionne
 * Contient le nom de l'item du PopupMenu des rubriques d'antécédent au moment de la sélection.
*/

/*! \var bool Dock_Menu::m_TerrainIsModified
 * Variable non utilisée car maintenant Atcd_Code gère lui-même l'état de modification des antécédents.
*/


//---------------------------------- init --------------------------------------------------------------
/*! \brief constructeur de la classe. Met tout �  zéro.
 *  Pour travailler avec cette classe, il faut lui définir les paramètres indispensables grâce �  setParams()
*/
void Dock_Menu::init()
{ m_pVar_Code = 0;
  m_pVar_Code = new Var_Code();
  if (!m_pVar_Code) return;
  // Enl�ve l'ent�te du ListView
  m_ListView_AccesRapide->header()->hide();
  m_ListView_AccesRapide->addColumn("_");
//   m_ListView_AccesRapide->addColumn("_");
  // Ne pas trier par d�faut le listView
  m_ListView_AccesRapide->setSorting(-1);
  m_pRubList           = 0;
  m_pAtcd_Code         = 0;
  m_Item_Coordonnees   = 0;
  m_Item_ATCD          = 0;
  m_Item_Observations  = 0;
  m_Item_Prescriptions = 0;
  m_Item_Documents     = 0;
  m_Item_Terrains      = 0;
  m_Item_Intervenants  = 0;
  m_Item_Vars          = 0;
  m_Notes_Changed      = FALSE;
  m_IsModifiable       = 0;
  m_DefaultFontSize    = 9;
  m_PkDocTerrain       = "";
  m_ClickBehavior      = 0;

  // m_OpenVisuNote
  //bool m_OpenInterv;
  // initialise les bool�ens des droits utilisateurs
  m_Droits_All =                FALSE;
  m_Droits_Create_ATCD =        FALSE;
  m_Droits_Create_Courriers =   FALSE;
  m_Droits_Create_Certificats = FALSE;
  m_Droits_Create_Documents =   FALSE;
  m_Droits_Create_Observations =FALSE;
  m_Droits_Create_Prescriptions=FALSE;

  m_Droits_View_ATCD =          FALSE;
  m_Droits_View_Certificats =   FALSE;
  m_Droits_View_Courriers =     FALSE;
  m_Droits_View_Identite =      FALSE;
  m_Droits_View_Documents =     FALSE;
  m_Droits_View_Observations =  FALSE;
  m_Droits_View_Prescriptions = FALSE;
  m_Droits_View_Variables =     FALSE;
  m_Droits_View_TTTFond =       FALSE;

  m_Droits_Modif_Variables =    FALSE;
  m_Droits_Modif_TTTFond =      FALSE;
  m_Droits_Modif_ATCD =         FALSE;
  m_OpenVisuNote      =         TRUE;
  m_OpenInterv        =         TRUE;
  //m_TerrainIsModified =         FALSE;

  m_PixMap_Homme              =  QPixmap(Theme::getIcon("Homme.png"));
  m_PixMap_Femme              =  QPixmap(Theme::getIcon("Femme.png"));
  m_PixMap_Parturiente        =  QPixmap(Theme::getIcon("Parturiente.png"));
  but_ZoomPlus->setPixmap               (Theme::getIcon("16x16/viewmag+.png"));
  but_ZoomMinus->setPixmap              (Theme::getIcon("16x16/viewmag-.png"));
  but_ZoomDefault->setPixmap            (Theme::getIcon("16x16/viewmag1.png"));
  but_NewIdent->setPixmap               (Theme::getIcon("16x16/patients.png") );
  if ( ! (QFile::exists (  "/home/ro/MedinTuxRo.txt" ) &&   G_pCApp->IsThisDroitExist( G_pCApp->m_Droits, G_pCApp->m_mapNameRubInfos[CMDI_ChoixPatient::S_GetRubName()]+"v")) )
     {  textLabel1->hide();
        m_LineEdit_ToSearch->hide();
        but_NewIdent->hide();
     }
  connect( m_ListView_AccesRapide, SIGNAL( doubleClicked(QListViewItem*,const QPoint&,int) ), this, SLOT( listView_AccesRapide_doubleClicked(QListViewItem*,const QPoint&,int) ) );

}

//---------------------------------- destroy --------------------------------------------------------------------
/*! \brief destructeur de la classe. Nettoye les pointeurs.
*/
void Dock_Menu::destroy()
{ if (m_Item_Coordonnees)         delete m_Item_Coordonnees;
  if (m_Item_ATCD)                delete m_Item_ATCD;
  if (m_Item_Vars)                delete m_Item_Vars;
  if (m_Item_Observations)        delete m_Item_Observations;
  if (m_Item_Prescriptions)       delete m_Item_Prescriptions;
  if (m_Item_Documents)           delete m_Item_Documents;
  if (m_Item_Terrains)            delete m_Item_Terrains;
  if (m_pVar_Code)                delete m_pVar_Code;
}

//---------------------------------- listView_AccesRapide_doubleClicked --------------------------------------------------------------------
/*! \brief reponse au double click sur la listview du menu
*/
void Dock_Menu::listView_AccesRapide_doubleClicked(QListViewItem* pQListViewItem, const QPoint&,int)
{ if (pQListViewItem==0) return;
  CPrtQListViewItem* pCPrtQListViewItem   = (CPrtQListViewItem*) pQListViewItem;
  CPrtQListViewItem* pQListViewItemParent = (CPrtQListViewItem*) pQListViewItem->parent();
  //.............. si racine cassos ........................
  if (pQListViewItemParent==0)
      {//pQListViewItemParent = pCPrtQListViewItem;
       //exeAnnuaire( "" );
       return;
      }
  //........... si parent est Correspondants .......................
  //            alors item est celui d'un correspondant
  if (pQListViewItemParent->text(0).left(14)==tr("Correspondants"))
     {QString pk = pCPrtQListViewItem->getSubType();
      if (pk.length()) exeAnnuaire( pk );
      else             exeAnnuaire( "" );
     }
}
//--------------------------------- exeAnnuaire -------------------------------------------------------------
QStringList Dock_Menu::exeAnnuaire(QString idInterv )
{/*
 QString pathExe, pathExch;
 if (CGestIni::Param_ReadParam(G_pCApp->m_DrTuxParam, "Gestion des contacts", "PathExe",     &pathExe) !=0)             return;         // path editeur de texte non défini
 if (CGestIni::Param_ReadParam(G_pCApp->m_DrTuxParam, "Repertoire Temporaire", "Repertoire", &pathExch)!=0)             return;         // path editeur de texte non défini
 if ( QDir::isRelativePath ( pathExch ) )   pathExch = QDir::cleanDirPath (pathExch.prepend(G_pCApp->m_PathAppli) );
 //if ( QDir::isRelativePath (pathExe ) )   pathExe  = QDir::cleanDirPath (pathExe.prepend(G_pCApp->m_PathAppli) );
 */
 QStringList lst;
 if (! G_pCApp->IsThisDroitExist(G_pCApp->m_Droits,"idc") )                                                           return lst;
 QString pathExe  = CGestIni::Construct_Name_Exe("personnes",QFileInfo (qApp->argv()[0]).dirPath (true));
 QString pathExch;


 // path editeur de texte non défini
 if (CGestIni::Param_ReadParam(G_pCApp->m_DrTuxParam, "Repertoire Temporaire", "Repertoire", &pathExch)!=0)           return lst;         // path editeur de texte non défini
 // Adaptation �  la plateforme
 pathExch = CGestIni::Construct_PathBin_Module("Manager",QFileInfo (qApp->argv()[0]).dirPath (true)) + pathExch;

 QProcess*  proc = new QProcess( this );
 if (proc==0)                                                                                                         return lst;
 QString exchFile = QDir::cleanDirPath (pathExch + QDir::separator() +  "GetPersonnne" + G_pCApp->m_NumGUID + ".ech");


 QFile file( exchFile );
 if (file.exists( exchFile )) file.remove();

 proc->addArgument( pathExe);
 proc->addArgument( exchFile );
 proc->addArgument( "Record");
 proc->addArgument( idInterv);
 proc->addArgument( G_pCApp->m_Droits);
 // long tempo = 0;
 if ( /*m_Contacts_Run==FALSE && */proc->start() )
    {//m_Contacts_Run = TRUE;
     QTimer timer;               // pour etre sur qu'un evenement se produise
     timer.start(100, FALSE);    // sinon boucle sans fin dans    processEvents ( QEventLoop::WaitForMore )
     qApp->processEvents ();
     while(proc->isRunning () )
           {QApplication::eventLoop()->processEvents ( QEventLoop::WaitForMore );
           }
     //m_Contacts_Run = FALSE;
     //......... recuperer fichier d'echange .............
     if ( !file.open( IO_ReadOnly ) )                                                                                   return lst;
     file.close();
     //QTextStream ts( &file );
     //QString ret = ts.read();    // 281|ANTOINE|Christine|Viscéral
     QString ret="";
     CGestIni::Param_UpdateFromDisk(exchFile, ret);
     lst = QStringList::split ('|', ret,TRUE );
     /*
     QString id, spec,nom, prenom;
     int deb =  0;
     int end = ret.find('|');
     id      = ret.mid(deb, end-deb);     // 0 - ID
     deb     = end + 1;
     end     = ret.find('|', deb);
     nom     = ret.mid(deb, end-deb);     // 1 - Nom
     deb     = end + 1;
     end     = ret.find('|', deb);
     prenom  = ret.mid(deb, end-deb);     // 2 - Prenom
     deb     = end + 1;
     spec    = ret.mid(deb);              // 3 - spec

     QString numGUID   = textLabel_NumDoss->text();

     if (pushButtonCreate->isShown()) new QListViewItem( listView_Praticiens, spec, nom, prenom, "", id );
     else if (m_pCMoteurBase->PatientIntervenantsWrite( m_ID_Doss , numGUID,  id, "") != 0)
        {new QListViewItem( listView_Praticiens, spec, nom, prenom, "", id );
        }
     */
     //QString numGUID   = textLabel_NumDoss->text();

     //if (pushButtonCreate->isShown()) new QListViewItem( listView_Praticiens, lst[3], lst[1], lst[2], "", lst[0] );
     //else if (m_pCMoteurBase->PatientIntervenantsWrite( m_ID_Doss , numGUID,  lst[0], "") != 0)
     //   {new QListViewItem( listView_Praticiens, lst[3], lst[1], lst[2], "", lst[0] );
     //   }

    }
 if (file.exists( exchFile )) file.remove();
 delete proc;
 return lst;
}
//---------------------------------- createIniParams -----------------------------------------------------------
/*! \brief Ajoute dans la paramString la section Dock_Menu avec ses variables par défaut.
 * Ajoute dans le fichier ini passé en paramètre les paramètres du dock_menu �  savoir :
 * ShowXXXX true/false  pour montrer l'item XXXX par défaut
 * OpenXXXX true/false  pour ouvrir la branche XXXX par défaut
 * Glossaire true/false pour savoir si Dock_Menu peut utiliser le Glossaire par le biais d'un popupmenu
 * DefaultFontSize      taille de la polica de caractère par défaut pour la listview
 * ClickOnParent        "openCMDI" ou "openCloseList" lorsque l'on clique sur l'item parent de la listView : ouvre la CMDI (openCMDI) ou ouvre la branche cliquée (openCloseList).
 */
void Dock_Menu::createIniParams(QString& paramString)
{ CGestIni::Param_WriteParam(&paramString, DOCK_MENU_SECTION_INI, "ShowTerrain", "true");
  CGestIni::Param_WriteParam(&paramString, DOCK_MENU_SECTION_INI, "ShowObs", "true");
  CGestIni::Param_WriteParam(&paramString, DOCK_MENU_SECTION_INI, "ShowPrescr", "true");
  CGestIni::Param_WriteParam(&paramString, DOCK_MENU_SECTION_INI, "ShowDoc", "true");
  CGestIni::Param_WriteParam(&paramString, DOCK_MENU_SECTION_INI, "ShowIdent", "true");
  CGestIni::Param_WriteParam(&paramString, DOCK_MENU_SECTION_INI, "ShowInterv", "true");
  CGestIni::Param_WriteParam(&paramString, DOCK_MENU_SECTION_INI, "Glossaire", "on");
  CGestIni::Param_WriteParam(&paramString, DOCK_MENU_SECTION_INI, "DefaultFontSize", "9");
  CGestIni::Param_WriteParam(&paramString, DOCK_MENU_SECTION_INI, "OpenIdent", "true");
  CGestIni::Param_WriteParam(&paramString, DOCK_MENU_SECTION_INI, "OpenATCD", "true");
  CGestIni::Param_WriteParam(&paramString, DOCK_MENU_SECTION_INI, "OpenVars", "true");
  CGestIni::Param_WriteParam(&paramString, DOCK_MENU_SECTION_INI, "OpenTerrain", "true");
  CGestIni::Param_WriteParam(&paramString, DOCK_MENU_SECTION_INI, "OpenVisuNote", "true");
  CGestIni::Param_WriteParam(&paramString, DOCK_MENU_SECTION_INI, "OpenObs", "true");
  CGestIni::Param_WriteParam(&paramString, DOCK_MENU_SECTION_INI, "OpenPrescr", "true");
  CGestIni::Param_WriteParam(&paramString, DOCK_MENU_SECTION_INI, "OpenInterv", "true");
  CGestIni::Param_WriteParam(&paramString, DOCK_MENU_SECTION_INI, "OpenDoc", "true");
  CGestIni::Param_WriteParam(&paramString, DOCK_MENU_SECTION_INI, "ParentItemInBold", "true");
  CGestIni::Param_WriteParam(&paramString, DOCK_MENU_SECTION_INI, "ClickOnParent", "openCMDI");
}

//---------------------------------- getIniParams -----------------------------------------------------------------
/*! \brief Récupère les paramètres dans la paramString.
 *  \sa Pour recréer les paramètres par défaut -> createIniParams.
*/
void Dock_Menu::getIniParams(QString& paramString)
{ QString tmp = "";
  CGestIni::Param_ReadParam(paramString, DOCK_MENU_SECTION_INI, "ShowTerrain", &tmp);
  if (tmp != "true") m_ShowTerrain = FALSE; else m_ShowTerrain = TRUE;
  tmp = "";
  CGestIni::Param_ReadParam(paramString, DOCK_MENU_SECTION_INI, "ShowObs", &tmp);
  if (tmp != "true") m_ShowObs = FALSE; else m_ShowObs = TRUE;
  tmp = "";
  CGestIni::Param_ReadParam(paramString, DOCK_MENU_SECTION_INI, "ShowPrescr", &tmp);
  if (tmp != "true") m_ShowPrescr = FALSE; else m_ShowPrescr = TRUE;
  tmp = "";
  CGestIni::Param_ReadParam(paramString, DOCK_MENU_SECTION_INI, "ShowDoc", &tmp);
  if (tmp != "true") m_ShowDoc = FALSE; else m_ShowDoc = TRUE;
  tmp = "";
  CGestIni::Param_ReadParam(paramString, DOCK_MENU_SECTION_INI, "ShowInterv", &tmp);
  if (tmp != "true") m_ShowInterv = FALSE; else m_ShowInterv = TRUE;
  tmp = "";
  CGestIni::Param_ReadParam(paramString, DOCK_MENU_SECTION_INI, "ShowIdent", &tmp);
  if (tmp != "true") m_ShowIdentity = FALSE; else m_ShowIdentity = TRUE;
  tmp = "";
  if (CGestIni::Param_ReadParam(paramString, DOCK_MENU_SECTION_INI, "Glossaire", &tmp) != 0)
      tmp  = "on"; // Valeur par d�faut
  if (tmp != "on") m_ShowGlossaire = FALSE; else m_ShowGlossaire = TRUE;
  tmp = "";
  if (CGestIni::Param_ReadParam(paramString, DOCK_MENU_SECTION_INI, "DefaultFontSize", &tmp) != 0)
      tmp = "9"; // Valeur par d�faut
  m_DefaultFontSize = tmp.toInt();
  tmp = "";

  CGestIni::Param_ReadParam(paramString, DOCK_MENU_SECTION_INI, "OpenIdent", &tmp);
  if (tmp != "true") m_OpenIdentity = FALSE; else m_OpenIdentity = TRUE;
  tmp = "";
  CGestIni::Param_ReadParam(paramString, DOCK_MENU_SECTION_INI, "OpenATCD", &tmp);
  if (tmp != "true") m_OpenATCD = FALSE; else m_OpenATCD = TRUE;
  tmp = "";
  CGestIni::Param_ReadParam(paramString, DOCK_MENU_SECTION_INI, "OpenVars", &tmp);
  if (tmp != "true") m_OpenVars = FALSE; else m_OpenVars = TRUE;
  tmp = "";
  CGestIni::Param_ReadParam(paramString, DOCK_MENU_SECTION_INI, "OpenTerrain", &tmp);
  if (tmp != "true") m_OpenTerrain = FALSE; else m_OpenTerrain = TRUE;
  tmp = "";
  CGestIni::Param_ReadParam(paramString, DOCK_MENU_SECTION_INI, "OpenVisuNote", &tmp);
  if (tmp != "true") m_OpenVisuNote = FALSE; else m_OpenVisuNote = TRUE;
  tmp = "";
  CGestIni::Param_ReadParam(paramString, DOCK_MENU_SECTION_INI, "OpenInterv", &tmp);
  if (tmp != "true") m_OpenInterv = FALSE; else m_OpenInterv = TRUE;
  tmp = "";
  CGestIni::Param_ReadParam(paramString, DOCK_MENU_SECTION_INI, "OpenObs", &tmp);
  if (tmp != "true") m_OpenObs = FALSE; else m_OpenObs = TRUE;
  tmp = "";
  CGestIni::Param_ReadParam(paramString, DOCK_MENU_SECTION_INI, "OpenPrescr", &tmp);
  if (tmp != "true") m_OpenPrescr = FALSE; else m_OpenPrescr = TRUE;
  tmp = "";
  CGestIni::Param_ReadParam(paramString, DOCK_MENU_SECTION_INI, "OpenDoc", &tmp);
  if (tmp != "true") m_OpenDoc = FALSE; else m_OpenDoc = TRUE;
  tmp = "";
  if (CGestIni::Param_ReadParam(paramString, DOCK_MENU_SECTION_INI, "ParentItemInBold", &tmp) != 0)
        tmp = "true"; // Valeur par d�faut
  if (tmp != "true") m_ParentInBold = FALSE; else m_ParentInBold = TRUE;
  tmp = "";
  if (m_DefaultFontSize < 4) m_DefaultFontSize = 9;
  QFont f = m_ListView_AccesRapide->font();
  f.setPointSize( m_DefaultFontSize ) ;
  m_ListView_AccesRapide->setFont(f);
  m_ListView_AccesRapide->triggerUpdate();
  if (CGestIni::Param_ReadParam(paramString, DOCK_MENU_SECTION_INI, "ClickOnParent", &tmp) != 0)
        tmp = "openCMDI"; // Valeur par d�faut
  if (tmp == "openCMDI") m_ClickBehavior = 0;
  else if (tmp == "openCloseList") m_ClickBehavior = 1;
  else m_ClickBehavior = 1;
  tmp = "";

}


//---------------------------------- setParams -----------------------------------------------------------------
/*! \brief Avant d'utiliser cette classe il faut renseigner les variables nécessaires.
 *  \param CMoteurBase : pointeur vers la classe de gestion de la base de données
 *  \param RUBREC_LIST : liste des documents du dossier patient
 *  \param Atcd_Code : Pointeur, même null, vers la classe Atcd_Code qui gère les ATCD du terrain.
*/
void Dock_Menu::setParams(CMoteurBase *pCMoteurBase, RUBREC_LIST  *pRubList,
                          const char* num_GUID, const char* id_doss,
                          const char* dossNom, const char* dossPrenom,
                          const char* user, const char* droits, Atcd_Code* pAtcd_Code)
{ m_pCMoteurBase = pCMoteurBase;
  m_pRubList = pRubList;
  // Attention gérer le cas où pAtcd_Code=0
  if (pAtcd_Code==0)            return;
  m_pAtcd_Code = pAtcd_Code;
  connect (m_pAtcd_Code, SIGNAL( ATCD_Changed()), this, SLOT(ATCD_HaveChanged()) );

  m_GUID       = num_GUID;
  m_Id_Doss    = id_doss;
  m_DossNom    = dossNom;
  m_DossPrenom = dossPrenom;
  m_User = user;
  if (m_pRubList->count() == 0) return;

  // Récupère les droits puis le terrain et recrée le menu
  SetInterfaceOnDroits(droits);
  getTerrain();
  //createMenu();
}


//---------------------------------- createMenu -----------------------------------------------------------------
/*! \brief �  vocation privée : met �  jour le menu avec le terrain chargé.
 *  Efface les items du listview en gardant l'item des terrains qui est calculé dans getTerrains.
*/
void Dock_Menu::createMenu()
{ // efface le contenu du listview actuel
  if (m_Item_Coordonnees)   { delete m_Item_Coordonnees;   m_Item_Coordonnees = 0;  }
  if (m_Item_ATCD)          { delete m_Item_ATCD;          m_Item_ATCD = 0; }
  if (m_Item_Observations)  { delete m_Item_Observations;  m_Item_Observations = 0; }
  if (m_Item_Prescriptions) { delete m_Item_Prescriptions; m_Item_Prescriptions = 0; }
  if (m_Item_Documents)     { delete m_Item_Documents;     m_Item_Documents = 0;}
  //if (m_Item_Terrain)       { delete m_Item_Terrain;       m_Item_Terrain = 0;}
  if (m_Item_Vars)          { delete m_Item_Vars;          m_Item_Vars = 0; }
  if (m_Item_Intervenants)  { delete m_Item_Intervenants;  m_Item_Intervenants = 0; }
  //  On garde m_Item_Terrains qui est calculé dans getTerrain()
  //  On efface les labels et TextEdits
  m_Lbl_Sexe->clear();
  m_Lbl_Nom->clear();
  m_Text_Note->clear();

  // Prépare le Menu
  if ((m_Droits_All) || (m_Droits_View_Identite))
  { createIdentAndNote();
    if (m_pAtcd_Code->m_Grossesse)
    { m_Lbl_Sexe->setPixmap( m_PixMap_Parturiente );
      m_Lbl_Sexe->setFixedWidth( 50 );
      m_Lbl_Nom->setText( m_Lbl_Nom->text()+"<br />"+ m_pAtcd_Code->getTermeObstetrical() );
    }
  }
  if (m_ShowInterv   &&    ((m_Droits_All) || (m_Droits_View_Identite)))             getIntervenants();
  if (m_ShowTerrain  &&    ((m_Droits_All) || (m_Droits_View_ATCD)))                 getATCDFromRubList();
  if (m_ShowTerrain  &&    ((m_Droits_All) || (m_Droits_View_Variables)))            getVarFromRubList();

  if (m_ShowObs      &&    ((m_Droits_All) || (m_Droits_View_Observations)))         getObservationsFromRubList();
  if (m_ShowPrescr   &&    ((m_Droits_All) || (m_Droits_View_Prescriptions)))        getPrescriptionsFromRubList();
  if (m_ShowDoc      &&    ((m_Droits_All) || (m_Droits_View_Documents)))            getDocumentsFromRubList();
  if (m_OpenVisuNote)        m_Text_Note->show();
  else                       m_Text_Note->hide();
}


//---------------------------------- checkItemToSave -------------------------------------------------------------
/*! \brief Fonction qui vérifie si des items doivent être sauvegardés avant de tout effacer (destruction, changement de patient ou d'utilisateur).
*/
void Dock_Menu::checkItemToSave()
{ if (m_IsModifiable==0) return;

  // Sauvegarde la note si droits OK
  if (((m_Droits_All) || (m_Droits_View_Identite)) && (m_Notes_Changed))
  { QString note_id = "";
    note_id = m_pCMoteurBase->PatientNoteIsThisExist(m_Id_Doss);
    if ( note_id == "")
    { // Créer la note
      m_pCMoteurBase->PatientNoteCreate(m_Id_Doss, m_GUID, m_Text_Note->text() );
    }
    else
    { // Updater la note
      m_pCMoteurBase->PatientNoteUpdate(note_id, m_Id_Doss, m_GUID, m_Text_Note->text() );
    }
    m_Notes_Changed = FALSE;
  } // if m_Notes_Changed

  // Si droits pour sauvegarde ATCD / Var ...
  if ((m_Droits_All) || (m_Droits_Modif_ATCD))
  {// modifications �  sauvegarder
   if ( m_pAtcd_Code->isModified() )
   { if (G_pCApp->m_pDrTux->m_pCMDI_Terrain == 0)
     { // Il faut faire soit même la sauvegarde du terrain
       int pos_deb   = m_pAtcd_Code->m_Terrain.find("[VAR");
       int idRubList = 0;
       RUBREC_LIST::iterator it;
       QString terrain = m_pAtcd_Code->toOldIni();
       terrain += m_pAtcd_Code->m_Terrain.mid(pos_deb);
       // Retrouve le terrain selon le pkdoc passé en paramètre
       for ( it = m_pRubList->begin(); it != m_pRubList->end(); ++it )
       { if ((*it).m_PrimKey == m_PkDocTerrain) break;
         idRubList++;
       }
       m_pCMoteurBase->ReplaceDataInRubList(terrain, m_pRubList, idRubList );
       m_pAtcd_Code->setModifiedState(FALSE);
     } // Fin if CMDP_Terrain==0
   } // Fin if m_TerrainIsModified
 } // Fin if droits
}

//------------------------------ getTerrain --------------------------------------------------
/*! \brief Prépare les variables de terrain (Atcd_Code et Var_Code) �  partir du Record n° id_RubList de la liste.
 *  Fonction �  appeler lorsque l'utilisateur change de terrain.
*/
void Dock_Menu::getTerrain(QString pkDoc)
{ //m_TerrainIsModified = FALSE;
  RUBREC_LIST::Iterator it ;
  bool trouve = FALSE;
  if (pkDoc == "") return;
  // Teste les droits de l'utilisateur
  if (!m_Droits_All)
   { if ((!m_Droits_View_Variables) || (!m_Droits_View_ATCD)) return; }

  // Vérifie que le terrain demandé n'est pas déj�  dans Atcd_Code
  if (pkDoc != m_pAtcd_Code->getPkDocTerrain() )
  { // Retrouve le terrain selon le pkdoc passé en paramètre
    for ( it = m_pRubList->begin(); it != m_pRubList->end(); ++it )
    { if ((*it).m_PrimKey == pkDoc) { trouve = TRUE;   break; }
    }
    if (!trouve) return;

    // Construit les ATCD �  l'aide de la classe Atcd_Code
    // m_pAtcd_Code->setTerrain( &(*it) , m_pCMoteurBase);
    // Construit les Variables �  l'aide de la classe Var_Code
    // m_pVar_Code->setTerrain( &(*it) , m_pCMoteurBase);
    m_pAtcd_Code->setTerrain(it , m_pCMoteurBase);
    // Construit les Variables �  l'aide de la classe Var_Code
    m_pVar_Code->setTerrain( it , m_pCMoteurBase);
  }
  createMenu();

  QString tmp = "";
  QDate dt;
  CPrtQListViewItem* precedent = 0;
  int i=0;
  int nbTerrains = 0;
  if (m_Item_Terrains) delete m_Item_Terrains;
  m_Item_Terrains = 0;

  for ( it = m_pRubList->begin(); it != m_pRubList->end(); ++it )
   { tmp = (*it).m_Type;
     if ( tmp == "20060000" )
     { // Si l'item terrains n'existe pas le créer
       if (m_Item_Terrains == 0)
       { m_Item_Terrains = new CPrtQListViewItem( m_ListView_AccesRapide, m_Item_Documents, "_"); }

         // Ajouter l'item de terrain
         dt = QDate::fromString( (*it).m_Date, Qt::ISODate );
         precedent = new CPrtQListViewItem( m_Item_Terrains ,        // QListView dans laquelle on insère un item
                                           precedent,
                                           dt.toString("dd MMM yy") + " - " + (*it).m_Libelle);                // libelle
         precedent->setValue( QString::number(i) ,        //  position du document dans la liste RUBREC_LIST
                                   (*it).m_Type,         //  type du document
                                   (*it).m_User,         //  Utilisateur courant de l'appli
                                   (*it).m_SubType);     //  sous type du document );
         precedent->setDate((*it).m_Date);                //  date document
         precedent->setPkDoc( (*it).m_PrimKey);           //  pk du document
         precedent->setSignUser( (*it).m_SignUser);       //  Signataire
         nbTerrains++;
     } // If == terrain
    if (nbTerrains == 6) break;
    i++;
   } // fin for
   tmp = QString(TR("Terrains (%1)")).arg(nbTerrains);
   if (m_ParentInBold) tmp.prepend("<b>");
   m_Item_Terrains->setPixmap(0, Theme::getIconListTypeTerrain().pixmap(QIconSet::Automatic,TRUE,QIconSet::On) );
   m_Item_Terrains->setText(0, tmp );
   m_Item_Terrains->setOpen(m_OpenTerrain);
   m_PkDocTerrain = pkDoc;
}

//------------------------------ getTerrain --------------------------------------------------
/*! \brief Recherche dans la RubList tous les terrains, affiche-les dans le menu, et prend le dernier enregistré pour prépare les variables de terrain (Atcd_Code et Var_Code).
*/
void Dock_Menu::getTerrain()
{ // Teste les droits de l'utilisateur
  if (!m_Droits_All)
   { if ((!m_Droits_View_Variables) || (!m_Droits_View_ATCD)) return; }
  //m_TerrainIsModified = FALSE;
  // Si aucun terrain affiche tout de même l'item pour pouvoir accéder au Glossaire
  if ((!m_pRubList) || (m_pRubList->count() == 0))
  { m_Item_Terrains = new CPrtQListViewItem( m_ListView_AccesRapide, 0, "<b>"+QString(TR("Terrains (%1)")).arg(0));
    m_pAtcd_Code->clear();
    return;
  }
  // Récupère le dernier Terrain enregistré (le plus récent donc)
  RUBREC_LIST::Iterator it ;
  QString tmp                      = "";

  m_Id_RubList_ToUseForThisTerrain = -1;
  int nbTerrains                   =  0;
  int i                            =  0;
  CPrtQListViewItem* precedent     =  0;
  m_Item_Terrains                  =  0;
  QDate dt;

  m_Item_Terrains = new CPrtQListViewItem( m_ListView_AccesRapide, 0, "");

  for ( it = m_pRubList->begin(); it != m_pRubList->end(); ++it )
   { tmp = (*it).m_Type;
     if ( tmp == "20060000" )
     { m_Id_RubList_ToUseForThisTerrain = i ;
       dt = QDate::fromString( (*it).m_Date, Qt::ISODate );
       precedent = new CPrtQListViewItem( m_Item_Terrains ,        // QListView dans laquelle on insère un item
                                             precedent,
                                             dt.toString("dd MMM yy") + " - " + (*it).m_Libelle);                // libelle
       precedent->setValue(      QString::number(i) ,          //  position du document dans la liste RUBREC_LIST
                                    (*it).m_Type,                 //  type du document
                                    (*it).m_User,                 //  Utilisateur courant de l'appli
                                    (*it).m_SubType);             //  sous type du document );
       precedent->setDate( (*it).m_Date);                      //  date document
       precedent->setPkDoc( (*it).m_PrimKey);                  //  pk du document
       precedent->setSignUser( (*it).m_SignUser);              //  Signataire


       nbTerrains++;
     } // If == terrain
     i++;
   } // Fin for

 if (m_Id_RubList_ToUseForThisTerrain == -1) // Aucun terrain retrouvé
 { m_Item_Terrains->setText(0, "<b>"+QString(TR("Terrains (%1)")).arg(nbTerrains));
   m_Item_Terrains->setPixmap(0, Theme::getIconListTypeTerrain().pixmap(QIconSet::Automatic,TRUE,QIconSet::On) );
   m_PkDocTerrain = "";
   return;
 }

 tmp = QString(TR("Terrains (%1)")).arg(nbTerrains);
 if (m_ParentInBold) tmp.prepend("<b>");
 m_Item_Terrains->setPixmap(0, Theme::getIconListTypeTerrain().pixmap(QIconSet::Automatic,TRUE,QIconSet::On) );
 m_Item_Terrains->setText(0, tmp);
 m_Item_Terrains->setOpen(m_OpenTerrain);
 it = m_pRubList->at ( m_Id_RubList_ToUseForThisTerrain );
 m_PkDocTerrain = (*it).m_PrimKey;
 // Construit les ATCD �  l'aide de la classe Atcd_Code
 // m_pAtcd_Code->setTerrain( &(*it) , m_pCMoteurBase);
 // Construit les Variables �  l'aide de la classe Var_Code
 // m_pVar_Code->setTerrain( &(*it) , m_pCMoteurBase);

 m_pAtcd_Code->setTerrain(it , m_pCMoteurBase);
 // Construit les Variables �  l'aide de la classe Var_Code
 m_pVar_Code->setTerrain( it , m_pCMoteurBase);

}


//---------------------------------- Slot_ChangeIdentity ----------------------------------------------------------
/*! \brief Slot appelé lors d'un changement d'identité (chgt de patient, ou modif de l'identité).
*/
void Dock_Menu::Slot_ChangeIdentity(const char* num_GUID, const char* id_doss,
                                    const char* dossNom, const char* dossPrenom, RUBREC_LIST* pRubList)
{ // Des éléments �  sauvegarder avant ?
  checkItemToSave();

  // Récupère les données
  m_GUID       = num_GUID;
  m_Id_Doss    = id_doss;
  m_DossNom    = dossNom;
  m_DossPrenom = dossPrenom;
  m_pRubList   = pRubList;
  m_PkDocTerrain = "";
  //if (m_pRubList->count() == 0) return;
  getRubListAndUpdateView( 0 );
}



//---------------------------------- getRubListAndUpdateView ------------------------------------------------------
/*! \brief Met �  jour la list view selon la rublist passée en paramètre.
 *  \sa DrTux::OnDrTuxSaveRubList(), Dock_Menu::Slot_ChangeIdentity
*/
void Dock_Menu::getRubListAndUpdateView(RUBREC_LIST* pRubList)
{ //m_TerrainIsModified = FALSE;
  // Récupère la RubList si une est passée en paramètre (dans DrTux::OnDrTuxSaveRubList() )
  if (pRubList!=0) m_pRubList = pRubList;

  bool coordOpen    = FALSE;
  bool ATCDOpen     = FALSE;
  bool VarsOpen     = FALSE;
  bool ObsOpen      = FALSE;
  bool PrescrOpen   = FALSE;
  bool DocOpen      = FALSE;
  bool terrainsOpen = FALSE;


  // Récupère l'état d'ouverture des branches principales
  if (m_Item_Coordonnees)   coordOpen    = m_Item_Coordonnees->isOpen();
  if (m_Item_ATCD)          ATCDOpen     = m_Item_ATCD->isOpen();
  if (m_Item_Vars)          VarsOpen     = m_Item_Vars->isOpen();
  if (m_Item_Observations)  ObsOpen      = m_Item_Observations->isOpen();
  if (m_Item_Prescriptions) PrescrOpen   = m_Item_Prescriptions->isOpen();
  if (m_Item_Documents)     DocOpen      = m_Item_Documents->isOpen();

  if (m_Item_Terrains) { terrainsOpen = m_Item_Terrains->isOpen();
                         delete m_Item_Terrains;
                         m_Item_Terrains = 0;
                       }

  if (m_PkDocTerrain != "") getTerrain(m_PkDocTerrain);
  else                      getTerrain();

  createMenu();

  // Remet l'affichage comme il était
  if (m_Item_Coordonnees)   m_Item_Coordonnees->setOpen(coordOpen);
  if (m_Item_ATCD)          m_Item_ATCD->setOpen(ATCDOpen);
  if (m_Item_Vars)          m_Item_Vars->setOpen(VarsOpen);
  if (m_Item_Observations)  m_Item_Observations->setOpen(ObsOpen);
  if (m_Item_Prescriptions) m_Item_Prescriptions->setOpen(PrescrOpen);
  if (m_Item_Documents)     m_Item_Documents->setOpen(DocOpen);
  if ((m_OpenVisuNote))     m_Text_Note->show();
  else                      m_Text_Note->hide();

//  m_Item_Terrains->setOpen(terrainsOpen );
}

//---------------------------------- createIdentAndNote -----------------------------------------------------------
/*! \brief Met �  jour le ListView avec l'identité sélectionnée.
  * \param Les paramètres sont récupérés dans les variables de la classe.
*/
void Dock_Menu::createIdentAndNote()
{// Gestion des droits
  if ((m_OpenVisuNote))     m_Text_Note->show();
  else                      m_Text_Note->hide();

 if (!m_Droits_All)
 { if (!m_Droits_View_Identite) return; }
 QString tmp = "";

 //............................... recuperer les notes ............................................
 QString note;
 m_pCMoteurBase->PatientNoteGetData(m_Id_Doss, note);
 m_Text_Note->setText(note);

 // Prépare l'item coordonnées
 tmp = TR("Identité");
 if (m_ParentInBold) tmp.prepend("<b>");
 m_Item_Coordonnees = new CPrtQListViewItem( m_ListView_AccesRapide,0, tmp);
 m_Item_Coordonnees->setPixmap(0, Theme::getIcon("16x16/identity.png"));
 m_Item_Coordonnees->setOpen(m_OpenIdentity);

 //............... chaines d'erreurs  ..................................................
 if  (m_pCMoteurBase->OpenBase()==0) return;

  //............... crer un curseur SQL ..................................................
    QString requete  = QString( "SELECT ");
            requete += m_pCMoteurBase->m_DOSS_IDENT_NOM_ASS          + ", " +       // 0
                       m_pCMoteurBase->m_DOSS_IDENT_PRENOM_ASS       + ", " +       // 1
                       m_pCMoteurBase->m_DOSS_IDENT_RUE              + ", " +       // 2
                       m_pCMoteurBase->m_DOSS_IDENT_TEL1             + ", " +       // 3
                       m_pCMoteurBase->m_DOSS_IDENT_TEL2             + ", " +       // 4
                       m_pCMoteurBase->m_DOSS_IDENT_TEL3             + ", " +       // 5
                       m_pCMoteurBase->m_DOSS_IDENT_EMAIL            + ", " +       // 6
                       m_pCMoteurBase->m_DOSS_IDENT_CODE_POST        + ", " +       // 7
                       m_pCMoteurBase->m_DOSS_IDENT_VILLE            + ", " +       // 8
                       m_pCMoteurBase->m_DOSS_IDENT_SEX              + ", " +       // 9
                       m_pCMoteurBase->m_DOSS_IDENT_GUID             + ", " +       // 10
                       m_pCMoteurBase->m_DOSS_IDENT_JFNOM            + ", " +       // 11
                       m_pCMoteurBase->m_DOSS_IDENT_TITRE            + ", " +       // 12
                       m_pCMoteurBase->m_DOSS_IDENT_PROF             + ", " +       // 13
                       m_pCMoteurBase->m_DOSS_IDENT_NSS              + ", " +       // 14
                       m_pCMoteurBase->m_DOSS_IDENT_NUMSS            + ", " +       // 15
                       m_pCMoteurBase->m_DOSS_IDENT_IS_ASS           + " FROM "  +  // 16
                       m_pCMoteurBase->m_DOSS_IDENT_TBL_NAME         + " WHERE " +
                       m_pCMoteurBase->m_DOSS_IDENT_REF_PK           + " = '"    + m_Id_Doss + "';";

    QSqlQuery query( requete  , m_pCMoteurBase->m_DataBase );
    if ( query.isActive() && query.next())
       {// Nom prénom âge + icone selon sexe
        QDate qDteNss = query.value( 14).toDate();
        tmp = "<b>"+ m_DossNom;
        if (CGestIni::Utf8_Query(query , 11) != "" ) tmp += TR(" Née ") + CGestIni::Utf8_Query(query , 11);
        tmp += " " + m_DossPrenom +"</b><br />"+m_pCMoteurBase->CalculeDiffAge(&qDteNss);
        m_Identity = tmp;
        m_Lbl_Nom->setText( tmp );
        tmp = "";

        // Sexe
        if (CGestIni::Utf8_Query(query , 9) == "M")
        { m_Lbl_Sexe->setPixmap( m_PixMap_Homme );
          m_IsFeminin = FALSE;
        }
        else if (CGestIni::Utf8_Query(query , 9) == "F")
        { m_Lbl_Sexe->setPixmap( m_PixMap_Femme );
          m_IsFeminin = TRUE;
        }
        else { m_Lbl_Sexe->clear(); m_Lbl_Sexe->setEnabled(FALSE);}
        m_Lbl_Sexe->setFixedWidth( 30 );
        tmp = "";

        if (query.value(15).toString() != "")
        { tmp  = query.value(15).toString();
          tmp  = tmp.left(1) + " " + tmp.mid(1,2) + " " + tmp.mid(3,2) + " " + tmp.mid(5,2) + " " + tmp.mid(7,3) + " " + tmp.mid(10,3) + " " + tmp.mid(13,2);
          tmp.prepend( TR("Num SS : ") );
          new CPrtQListViewItem( m_Item_Coordonnees,0, tmp);
        }

        if (query.value(13).toString() != "")
        { tmp  = query.value(13).toString();
          tmp.prepend( TR("Profession : ") );
          new CPrtQListViewItem( m_Item_Coordonnees,0, tmp);
        }

        if (query.value( 5).toString() != "")
        { tmp = query.value(5).toString(); tmp =tmp.remove(' ');
          tmp = tmp.left(2)+" "+tmp.mid(2,2)+" "+tmp.mid(4,2)+" "+tmp.mid(6,2)+" "+tmp.mid(8,2);
          tmp.prepend( TR("Tél3 : ") );
          new CPrtQListViewItem( m_Item_Coordonnees,0, tmp);
        }
        if (query.value( 4).toString() != "")
        { tmp = query.value(4).toString(); tmp =tmp.remove(' ');
          tmp = tmp.left(2)+" "+tmp.mid(2,2)+" "+tmp.mid(4,2)+" "+tmp.mid(6,2)+" "+tmp.mid(8,2);
          tmp.prepend( TR("Tél2 : ") );
          new CPrtQListViewItem( m_Item_Coordonnees, 0, tmp);
        }
        if (query.value( 3).toString() != "")
        { tmp = query.value(3).toString(); tmp =tmp.remove(' ');
          tmp = tmp.left(2)+" "+tmp.mid(2,2)+" "+tmp.mid(4,2)+" "+tmp.mid(6,2)+" "+tmp.mid(8,2);
          tmp.prepend( TR("Tél1 : ") );
          new CPrtQListViewItem( m_Item_Coordonnees,0, tmp);
        }
        if (query.value( 7).toString() != "")
        { tmp = TR("Ville : ") + CGestIni::Utf8_Query(query , 7) + " " + CGestIni::Utf8_Query(query , 8);
          new CPrtQListViewItem( m_Item_Coordonnees,0, tmp);
        }
        if (CGestIni::Utf8_Query(query , 2) != "")
        { tmp = TR("Adresse : ") + CGestIni::Utf8_Query(query , 2);
          new CPrtQListViewItem( m_Item_Coordonnees,0, tmp);
        }

       m_pCMoteurBase->CloseBase();
      return;
    }
    else
    {m_pCMoteurBase->OutSQL_error(query);
     m_pCMoteurBase->CloseBase();
    }
}


//---------------------------------- getVarFromRubList -------------------------------------------------------------
/*! \brief Fonction qui récupère la liste des Variables en se basant sur l'analyse du terrain le plus récent.
*/
void Dock_Menu::getVarFromRubList()
{ if (!m_pVar_Code) return;
  if (m_Item_Vars) { delete m_Item_Vars; m_Item_Vars=0; }
  QString tmp = TR("Variables");
  if (m_ParentInBold) tmp.prepend("<b>");
  m_Item_Vars = new CPrtQListViewItem(m_ListView_AccesRapide, m_Item_ATCD, tmp );
  m_pVar_Code->getVarToListViewItem( *m_Item_Vars );
  m_Item_Vars->setPixmap(0, Theme::getIconListTypeTerrain().pixmap(QIconSet::Automatic,TRUE,QIconSet::On) );
  m_Item_Vars->setOpen(m_OpenVars);
}

//---------------------------------- setSexeAndParturienteDisplay ------------------------------------------------------------
/*! \brief Fonction qui affiche tout ce qui correspond au sexe et etat grosesse.
*/
void Dock_Menu::setSexeAndParturienteDisplay()
{ if (m_pAtcd_Code->m_Grossesse)
  { m_Lbl_Sexe->setPixmap( m_PixMap_Parturiente );
    m_Lbl_Sexe->setFixedWidth( 50 );
    m_Lbl_Nom->setText( m_Identity+"<br />"+ m_pAtcd_Code->getTermeObstetrical() );
  }
 else
 { if (!m_IsFeminin)
    { m_Lbl_Sexe->setPixmap( m_PixMap_Homme );
      m_Lbl_Sexe->setFixedWidth( 30 );
      m_Lbl_Nom->setText( m_Identity);
      m_IsFeminin = FALSE;
    }
    else
    { m_Lbl_Sexe->setPixmap( m_PixMap_Femme );
      m_Lbl_Sexe->setFixedWidth( 30 );
      m_Lbl_Nom->setText( m_Identity );
      m_IsFeminin = TRUE;
    }
 }
}

//---------------------------------- ATCD_HaveChanged -------------------------------------------------------------
/*! \brief Slot connecté au signal Atcd_Code::ATCD_Changed(). Met �  jour : les ATCD et ouvre la branche ; le pixmap d'identité et le label de l'identité si enceinte ou pas.
*/
void Dock_Menu::ATCD_HaveChanged()
{// Gestion des droits
 if (!m_ShowTerrain)           return;
 if (!m_Droits_All)
    { if (!m_Droits_View_ATCD) return;
    }
 //m_TerrainIsModified = TRUE;

 getATCDFromRubList();
 setSexeAndParturienteDisplay();
 m_Item_ATCD->setOpen(TRUE);
}

//---------------------------------- getATCDFromRubList ------------------------------------------------------------
/*! \brief Fonction qui récupère les ATCD stockés dans Atcd_Code de CApp.
 * Les droits utilisateurs doivent être gérés avant l'appel de cette fonction
*/
void Dock_Menu::getATCDFromRubList()
{if (!m_pAtcd_Code)      return;
 if (m_Item_ATCD != 0)
    { delete m_Item_ATCD;
      m_Item_ATCD = 0;
    }
 QString tmp = TR("Antécédents");
 if (m_ParentInBold) tmp.prepend("<b>");
 m_Item_ATCD = new CPrtQListViewItem( m_ListView_AccesRapide, m_Item_Intervenants, tmp);
 m_pAtcd_Code->createAtcdListView( *m_Item_ATCD );
 m_Item_ATCD->setPixmap(0, Theme::getIconListTypeTerrain().pixmap(QIconSet::Automatic,TRUE,QIconSet::On) );
 m_Item_ATCD->setOpen(m_OpenATCD);
}


//---------------------------------- getObservationsFromRubList ----------------------------------------------------
/*! \brief Fonction qui présente le nom des dernières observations enregistrées dans le QListView.
 * Les droits utilisateurs doivent être gérés avant l'appel de cette fonction
*/
void Dock_Menu::getObservationsFromRubList()
{ int nbObs = 0;                                  // Compte le nb d'observations
  int pos = 0;                                // Position de l'item dans la RubList
  QString tmp = "";
  QDate dt;
  CPrtQListViewItem* precedent = 0;

  // Récupère le dernier terrain
  RUBREC_LIST::Iterator it;
  pos = m_pRubList->count();
  m_Item_Observations = new CPrtQListViewItem( m_ListView_AccesRapide, m_Item_Vars);
  // Récupère le dernier Terrain enregistré (le plus récent donc)
  for ( int i = pos; i != -1; --i )
  { it = m_pRubList->at(i);
    tmp = (*it).m_Type;
    if ( tmp.left(4) == "2003" )
    { if (nbObs < 5)
      { dt = QDate::fromString( (*it).m_Date, Qt::ISODate );
        precedent = new CPrtQListViewItem   (   m_Item_Observations ,        // QListView dans laquelle on insère un item
                                   precedent,
                                   dt.toString("dd MMM yy") + " - " + (*it).m_Libelle);                // libelle
        //setValue(const QString &pk, const QString  &type, const QString &user, const QString &subType) ;
        precedent->setValue( QString::number(pos) ,        //  position du document dans la liste RUBREC_LIST
                                   (*it).m_Type,        //  type du document
                                   (*it).m_User,        //  Utilisateur courant de l'appli
                                   (*it).m_SubType);        //  sous type du document
        precedent->setDate((*it).m_Date);                //  date document
        precedent->setPkDoc( (*it).m_PrimKey);                //  pk du document
        precedent->setSignUser( (*it).m_SignUser);                //  Signataire
      }
    nbObs++;
   }
   tmp = "";
   pos--;
  }
  tmp = QString(TR("Observations (%1)")).arg(nbObs);
  if (m_ParentInBold) tmp.prepend("<b>");
  m_Item_Observations->setPixmap(0, Theme::getIcon( "16x16/type_Observation.png" ));
  m_Item_Observations->setText(0, tmp );
  m_Item_Observations->setOpen(m_OpenObs);
}


//---------------------------------- getPrescriptionsFromRubList -------------------------------------------------------
/*! \brief Fonction qui présente le nom des dernières prescriptions enregistrées dans le QListView.
 * Les droits utilisateurs doivent être gérés avant l'appel de cette fonction
*/
void Dock_Menu::getPrescriptionsFromRubList()
{ int nbPresc = 0;                                 // Compte le nb d'observation insérées.
  int pos = 0;                                // Position de l'item dans la RubList
  QString tmp = "";
  QDate dt;
  CPrtQListViewItem* precedent = 0;

  // Récupère le dernier terrain
  pos = m_pRubList->count();
  RUBREC_LIST::Iterator it;
  m_Item_Prescriptions = new CPrtQListViewItem( m_ListView_AccesRapide, m_Item_Observations );
  // Récupère le dernier Terrain enregistré (le plus récent donc)
  for ( int i = pos; i != -1; --i )
  { it = m_pRubList->at(i);
    tmp = (*it).m_Type;
    if ( tmp.left(4) ==  "2002" )
    { if ( nbPresc < 5 )
      { dt = QDate::fromString( (*it).m_Date, Qt::ISODate );
        precedent = new CPrtQListViewItem   (   m_Item_Prescriptions ,        // QListView dans laquelle on insère un item
                                   precedent,
                                   dt.toString("dd MMM yy") + " - " + (*it).m_Libelle);                // libelle
        //setValue(const QString &pk, const QString  &type, const QString &user, const QString &subType) ;
        precedent->setValue( QString::number(pos) ,        //  position du document dans la liste RUBREC_LIST
                                   (*it).m_Type,        //  type du document
                                   (*it).m_User,        //  Utilisateur courant de l'appli
                                   (*it).m_SubType);        //  sous type du document
        precedent->setDate((*it).m_Date);                //  date document
        precedent->setPkDoc( (*it).m_PrimKey);                //  pk du document
        precedent->setSignUser( (*it).m_SignUser);                //  Signataire
      }
      nbPresc++;
    }
    tmp = "";
   pos--;
  }
  tmp = QString(TR("Prescriptions (%1)")).arg(nbPresc);
  if (m_ParentInBold) tmp.prepend("<b>");
  m_Item_Prescriptions->setPixmap(0, Theme::getIcon( "16x16/type_Prescription.png" ) );
  m_Item_Prescriptions->setText(0, tmp );
  m_Item_Prescriptions->setOpen(m_OpenPrescr);
}
//---------------------------------- getIntervenants ------------------------------------------------------
/*! \brief Fonction qui présente le nom des dernières observations enregistrées dans le QListView.
 * Les droits utilisateurs doivent être gérés avant l'appel de cette fonction
*/
void Dock_Menu::getIntervenants()
{ CPrtQListViewItem* precedent = 0;
  if (m_Item_Intervenants) delete m_Item_Intervenants;
  m_Item_Intervenants = new CPrtQListViewItem ( m_ListView_AccesRapide, m_Item_Coordonnees);

  QStringList list;
  m_pCMoteurBase->PatientIntervenantsGetListData(m_Id_Doss ,list);
  for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
      { QString data = *it;
        QStringList fieldData = QStringList::split ('|', data, TRUE );
        precedent = new CPrtQListViewItem   (   m_Item_Intervenants ,   precedent, fieldData[1]+" "+fieldData[2]+" "+fieldData[0]);
        if (precedent) precedent->setSubType(fieldData[4]);      // noter le pk
      }

  QString tmp = QString(TR("Correspondants (%1)")).arg(list.count());
  if (m_ParentInBold) tmp.prepend("<b>");
  m_Item_Intervenants ->setPixmap(0, Theme::getIcon( "16x16/type_intervenants.png" ) );
  m_Item_Intervenants ->setText(0, tmp );
  m_Item_Intervenants ->setOpen(m_OpenInterv);
}
//---------------------------------- getDocumentsFromRubList ------------------------------------------------------
/*! \brief Fonction qui présente le nom des dernières observations enregistrées dans le QListView.
 * Les droits utilisateurs doivent être gérés avant l'appel de cette fonction
*/
void Dock_Menu::getDocumentsFromRubList()
{ int nbDoc = 0;                                  // Compte le nb d'observations
  int pos = 0;                                // Position de l'item dans la RubList
  QString tmp = "";
  QDate dt;
  CPrtQListViewItem* precedent = 0;

  // Récupère le dernier terrain
  RUBREC_LIST::Iterator it;
  pos = m_pRubList->count();
  m_Item_Documents = new CPrtQListViewItem ( m_ListView_AccesRapide, m_Item_Prescriptions);
  // Récupère le dernier Terrain enregistré (le plus récent donc)
  for ( int i = pos; i != -1; --i )
  { it = m_pRubList->at(i);
    tmp = (*it).m_Type;
    if ( tmp.left(4) == "2008" )
    { if (nbDoc < 5)
      { dt = QDate::fromString( (*it).m_Date, Qt::ISODate );
        precedent = new CPrtQListViewItem   (   m_Item_Documents ,        // QListView dans laquelle on insère un item
                                   precedent,
                                   dt.toString("dd MMM yy") + " - " + (*it).m_Libelle);                // libelle
        precedent->setValue( QString::number(pos) ,        //  position du document dans la liste RUBREC_LIST
                                   (*it).m_Type,        //  type du document
                                   (*it).m_User,        //  Utilisateur courant de l'appli
                                   (*it).m_SubType);        //  sous type du document
        precedent->setDate((*it).m_Date);                //  date document
        precedent->setPkDoc( (*it).m_PrimKey);                //  pk du document
        precedent->setSignUser( (*it).m_SignUser);                //  Signataire
      }
    nbDoc++;
   }
   tmp = "";
   pos--;
  }
  tmp = QString(TR("Documents (%1)")).arg(nbDoc);
  if (m_ParentInBold) tmp.prepend("<b>");
  m_Item_Documents->setPixmap(0, Theme::getIcon( "16x16/type_Documents.png" ) );
  m_Item_Documents->setText(0, tmp );
  m_Item_Documents->setOpen(m_OpenDoc);
}

//---------------------------------- m_ListView_AccesRapide_contextMenuRequested --------------------------------
/*! \brief ContextMenu de la ListView.
  * \todo Définir les touches de raccourci
*/
void Dock_Menu::m_ListView_AccesRapide_contextMenuRequested( QListViewItem * item, const QPoint & p, int )
{ if (!item) return;
  CPrtQListViewItem* pCPrt = (CPrtQListViewItem*) item;
  QString libItem = "";
  QString libParent = "";
  QListViewItem * parentItem = item;
  m_Popup = 0;

  // Récupère le nom du premier parent de l'item cliqué ou le nom de l'item
  if (item->parent() != 0)
  { while( parentItem->depth() != 0 )
    { parentItem = parentItem->parent();
    }
    libParent = parentItem->text(0);
    libItem = item->text(0);
  }
  else
  { libItem = item->text(0);
    libParent = "";
  }

  if ((libParent == TR("Antécédents") ) || libItem == TR("Antécédents"))
  { m_Popup = new ThemePopup(this);
    ATCD_createPopup(m_Popup, pCPrt);
  }  // Fin TR("Antecedents")

  if (m_ShowGlossaire)
  {  if ((libParent.find( TR("Observation") ) != -1 ) || (libItem.find( TR("Observation")) != -1))
     { m_Popup = new ThemePopup(this);
       Observations_createPopup(m_Popup, pCPrt);
     }

     if ((libParent.find( TR("Prescription") ) != -1 ) || (libItem.find( TR("Prescription")) != -1))
     { m_Popup = new ThemePopup(this);
       Prescriptions_createPopup(m_Popup, pCPrt);
     }

     if ((libParent.find( TR("Document") ) != -1 ) || (libItem.find( TR("Document")) != -1))
     { m_Popup = new ThemePopup(this);
       Documents_createPopup(m_Popup, pCPrt);
     }

     if ((libParent.find( TR("Terrains") ) != -1) || libItem.find( TR("Terrains")) != -1)
     { m_Popup = new ThemePopup(this);
       Terrain_createPopup(m_Popup, pCPrt);
     }  // Fin TR("Terrains")
  } // Fin m_ShowGlossaire

  if (!m_Popup)  { m_Popup = new ThemePopup(this); }
  if (m_Popup)
  { ThemePopup* popupDroits = new ThemePopup(m_Popup);
    Droits_createPopup(popupDroits);
    m_Popup->insertSeparator();
    m_Popup->insertItem( Theme::getIconListLocked(), TR("Voir vos droits") , popupDroits);
    m_Popup->popup(p);
  }
}

//------------------------------------------- Terrain_createPopup ----------------------------------------
/*! \brief Créer un popup en relation avec l'item terrain sélectionné.
*/
void Dock_Menu::Terrain_createPopup(ThemePopup* pPopup, CPrtQListViewItem* /*pCPrt*/)
{ if ((m_Droits_All) || (m_Droits_Create_ATCD))
  { pPopup->insertItem( Theme::getIconListAdd(), TR("Ajouter un terrain"));
   pPopup->insertSeparator();
   directoryToPopup(pPopup,G_pCApp->m_PathGlossaire + CMDI_Terrain::S_GetRubName(),0);
   connect( pPopup, SIGNAL( activated(int)), this, SLOT(OnMenuActivated(int)));
  }
  else
  { pPopup->insertItem( Theme::getIconListWarning(), TR("Vous ne possédez pas les droits suffisants pour accéder au glossaire")); }
}

//------------------------------------------- Observations_createPopup ----------------------------------------
/*! \brief Créer un popup en relation avec l'item observations sélectionné.
*/
void Dock_Menu::Observations_createPopup(ThemePopup* pPopup, CPrtQListViewItem* /*pCPrt*/)
{ if ((m_Droits_All) || (m_Droits_Create_Observations))
  { pPopup->insertItem( Theme::getIconListAdd(), TR("Ajouter une observation"));
   pPopup->insertSeparator();
   directoryToPopup(pPopup,G_pCApp->m_PathGlossaire + RUBNAME_OBSERVATION,0);
   connect( pPopup, SIGNAL( activated(int)), this, SLOT(OnMenuActivated(int)));
  }
  else
  { pPopup->insertItem( Theme::getIconListWarning(), TR("Vous ne possédez pas les droits suffisants pour accéder au glossaire")); }
}

//------------------------------------------- Prescriptions_createPopup ----------------------------------------
/*! \brief Créer un popup en relation avec l'item prescriptions sélectionné.
*/
void Dock_Menu::Prescriptions_createPopup(ThemePopup* pPopup, CPrtQListViewItem* /*pCPrt*/)
{ if ((m_Droits_All) || (m_Droits_Create_Prescriptions))
  { pPopup->insertItem( Theme::getIconListAdd(), TR("Ajouter une observation"));
    pPopup->insertSeparator();
    directoryToPopup(pPopup,G_pCApp->m_PathGlossaire + CMDI_Prescription::S_GetRubName(),0);
    connect( pPopup, SIGNAL( activated(int)), this, SLOT(OnMenuActivated(int)));
  }
  else
  {pPopup->insertItem( Theme::getIconListWarning(), TR("Vous ne possédez pas les droits suffisants pour accéder au glossaire")); }
}

//------------------------------------------- Documents_createPopup ----------------------------------------
/*! \brief Créer un popup en relation avec l'item documents sélectionné.
*/
void Dock_Menu::Documents_createPopup(ThemePopup* pPopup, CPrtQListViewItem* /*pCPrt*/)
{ if ((m_Droits_All) || (m_Droits_Create_Documents))
  { pPopup->insertItem( Theme::getIconListAdd(), TR("Ajouter une observation"));
    pPopup->insertSeparator();
    directoryToPopup(pPopup,G_pCApp->m_PathGlossaire + RUBNAME_DOCUMENTS,0);
    connect( pPopup, SIGNAL( activated(int)), this, SLOT(OnMenuActivated(int)));
  }
  else
  {pPopup->insertItem( Theme::getIconListWarning(), TR("Vous ne possédez pas les droits suffisants pour accéder au glossaire")); }
}

//------------------------------------------- Droits_createPopup ----------------------------------------
/*! \brief Créer un popup pour visualiser tous les droits. Fonction utilisée principalement pour débuggage.
*/
void Dock_Menu::Droits_createPopup(ThemePopup* pPopup)
{ if (!pPopup) return;
  if (m_Droits_All)
  { pPopup->insertItem(Theme::getIconListOK(), TR("Tous les droits sont accordés (super-administrateur)"));
    return ;
  }
  ThemePopup *createPop = new ThemePopup ( pPopup);
  if (m_Droits_Create_ATCD) createPop->insertItem(Theme::getIconListOK(), TR("Antécédents"));
  else createPop->insertItem(Theme::getIconListLocked(), TR("Antécédents"));
  if (m_Droits_Create_Courriers) createPop->insertItem(Theme::getIconListOK(), TR("Courriers"));
  else createPop->insertItem(Theme::getIconListLocked(), TR("Courriers"));
  if (m_Droits_Create_Certificats) createPop->insertItem(Theme::getIconListOK(), TR("Certificats"));
  else createPop->insertItem(Theme::getIconListLocked(), TR("Certificats"));
  if (m_Droits_Create_Documents) createPop->insertItem(Theme::getIconListOK(), TR("Documents"));
  else createPop->insertItem(Theme::getIconListLocked(), TR("Ne peut pas créer de documents"));
  if (m_Droits_Create_Observations) createPop->insertItem(Theme::getIconListOK(), TR("Observations"));
  else createPop->insertItem(Theme::getIconListLocked(), TR("Ne peut pas créer d'observations"));
  if (m_Droits_Create_Prescriptions) createPop->insertItem(TR("Prescriptions"));
  else createPop->insertItem(Theme::getIconListLocked(), TR("Prescriptions"));
  pPopup->insertItem(TR("Création"), createPop);

  ThemePopup *viewPop = new ThemePopup (pPopup);
  if (m_Droits_View_ATCD) viewPop->insertItem(Theme::getIconListOK(), TR("Antécédents"));
  else viewPop->insertItem(Theme::getIconListLocked(), TR("Antécédents"));
  if (m_Droits_View_Certificats) viewPop->insertItem(Theme::getIconListOK(), TR("Certificats"));
  else viewPop->insertItem(Theme::getIconListLocked(), TR("Certificats"));
  if (m_Droits_View_Courriers) viewPop->insertItem(Theme::getIconListOK(), TR("Courriers"));
  else viewPop->insertItem(Theme::getIconListLocked(), TR("Courriers"));
  if (m_Droits_View_Identite) viewPop->insertItem(Theme::getIconListOK(), TR("Identité"));
  else viewPop->insertItem(Theme::getIconListLocked(), TR("Identité"));
  if (m_Droits_View_Documents) viewPop->insertItem(Theme::getIconListOK(), TR("Documents"));
  else viewPop->insertItem(Theme::getIconListLocked(), TR("Documents"));
  if (m_Droits_View_Observations) viewPop->insertItem(Theme::getIconListOK(), TR("Observations"));
  else viewPop->insertItem(Theme::getIconListLocked(), TR("Observations"));
  if (m_Droits_View_Prescriptions) viewPop->insertItem(Theme::getIconListOK(), TR("Prescriptions"));
  else viewPop->insertItem(Theme::getIconListLocked(), TR("Prescriptions"));
  if (m_Droits_View_Variables) viewPop->insertItem(Theme::getIconListOK(), TR("Variables du terrain"));
  else viewPop->insertItem(Theme::getIconListLocked(), TR("Variables du terrain"));
  if (m_Droits_View_TTTFond) viewPop->insertItem(Theme::getIconListOK(), TR("Traitement de fond"));
  else viewPop->insertItem(Theme::getIconListLocked(), TR("Traitement de fond"));
  pPopup->insertItem(TR("Visualisation"), viewPop);

  ThemePopup *modPop = new ThemePopup (pPopup);
  if (m_Droits_Modif_Variables) modPop->insertItem(Theme::getIconListOK(), TR("Variables du terrain"));
  else modPop->insertItem(Theme::getIconListLocked(), TR("Variables du terrain"));
  if (m_Droits_Modif_TTTFond) modPop->insertItem(Theme::getIconListOK(), TR("Traitement de fond"));
  else modPop->insertItem(Theme::getIconListLocked(), TR("Traitement de fond"));
  if (m_Droits_Modif_ATCD) modPop->insertItem(Theme::getIconListOK(), TR("Antécédents"));
  else modPop->insertItem(Theme::getIconListLocked(), TR("Antécédents"));
  pPopup->insertItem(TR("Modification"), modPop);

}

//------------------------------------------- directoryToPopup ----------------------------------------
/*! \brief Créer l'arborescence de la liste et l'introduit dans le fichier cache.
 *  \param ThemePopup : menu qui reçoit l'arborescence
 *  \param path : Chemin �  explorer
 *  \param numSousMenus : nb de sous-menus avant cet item. Utilisé pour limiter le nombre de sous-menus.
 * \return int :
*/
int Dock_Menu::directoryToPopup(ThemePopup* pPopup, QString path, int numSousMenus)
{ if (!pPopup) return 0;
  QDir dir (path);
  if (!dir.exists()) return 0;
//  dir.convertToAbs();
  dir.setFilter( QDir::All | QDir::NoSymLinks );
  dir.setSorting(QDir::DirsFirst | QDir::Name);

  const QFileInfoList * pQFileInfoList = dir.entryInfoList();
  if (pQFileInfoList==0) return 0;
  QFileInfoListIterator it( *pQFileInfoList );
  QFileInfo     *fi;
  QString fname="";
  // Parcours le répertoire
  while ( (fi = it.current()) != 0 )
  {  fname = fi->fileName();
     if (fname == ".." || fname == ".")
     {}
     else if (fi->isDir() && fname != "." && fname.lower() != "cvs")            // c'est un autre répertoire
     {        // Prépare le WhatsThis qui contient le path complet du fichier
        QString wT = path +QDir::separator()+ fname;
        // Vérifie qu'on peut créer une nouvelle hiérarchie de menu
        int limite = numSousMenus + 1;
        if (limite < NBMAXSOUSMENU)
        { // OK On peut créer une nouvelle branche
          ThemePopup* sousMenuPop = new ThemePopup(pPopup);
          directoryToPopup(sousMenuPop, wT, limite);
          connect( sousMenuPop, SIGNAL( activated(int)), this, SLOT(OnMenuActivated(int)));
          // Insérer le popup dans popup de la fonction
          pPopup->insertItem( Theme::getIconListFolder(), fname, sousMenuPop);
        }
     }
     else  // c'est un fichier
     {        QString ext = fi->extension();
        if (ext.lower() == "txt" || ext.lower() == "html" || ext.lower() == "htm" || ext.lower() == "rtf" || ext == "")
        { int id = pPopup->insertItem( Theme::getIconListHTML(), fi->baseName(),  this, SLOT( Slot_FileSelected() ) );
          pPopup->setWhatsThis(id , path+QDir::separator()+fi->fileName());
        }
     }  // end else if (fi->isFile())
    ++it;
    } // fin while
  return 0;
}
//------------------------------------------- Slot_FileSelected ----------------------------------------
/*! \brief Slot appelé lors de la sélection d'un fichier dans le menu "Glossaire".
 * Emet  le signal Sign_ActiverRubrique() poour mettre au premier plan la CMDI concernée par ce document.
 * Puis émet le signal Sign_GlossaireFileSelected qui sera connecté dans drtux �  OnGlossaireFileClicked pour signifier �  DrTux qu'un nouveau document doit être créé.
*/
void Dock_Menu::Slot_FileSelected()
{ if (m_SelectedFile != "")
  { int typ = G_pCApp->PathDocumentToType( m_SelectedFile );
    QString rubName = G_pCApp->TypeToRubName(typ );
    emit Sign_ActiverRubrique(rubName,0);
    emit Sign_GlossaireFileSelected( m_SelectedFile, typ);
  }
}

//------------------------------------------- OnMenuActivated ----------------------------------------
/*! \brief lorsque le popup est activé récupère le fichier sélectionné (le chemin est dans le whatsThis de l'item du popup.
*/
void Dock_Menu::OnMenuActivated(int id)
{ if ((id != -1) && (m_Popup))
    { m_SelectedFile   = m_Popup->whatsThis(id);
    }
  else m_SelectedFile = "";
}


//------------------------------------ ATCDRubriques_createPopup --------------------------------------------------
/*! \brief Ajoute le menu de selection du type d'antécedent �  un menu quelconque
*/
void Dock_Menu::ATCDRubriques_createPopup(ThemePopup* pQPopupMenu)
{pQPopupMenu->insertSeparator ();
 connect ( G_pCApp, SIGNAL(Sign_popup_HierarchOptionSelected()) , this, SLOT(ATCD_MenuActionSetRubrique()));
 G_pCApp->addPopupHierarchique(G_pCApp->m_PathAppli + "Ressources/MenuATCD.txt", pQPopupMenu, &m_TypeATCD_Selectionne);
}

//------------------------------------ ATCD_MenuActionSetRubrique --------------------------------------------------
/*! \brief Modifie la rubrique d'un ATCD par le biais de la classe Atcd_Code.
*/void Dock_Menu::ATCD_MenuActionSetRubrique()
{disconnect ( G_pCApp, SIGNAL(Sign_popup_HierarchOptionSelected()) , this, SLOT(ATCD_MenuActionSetRubrique()));
 if (m_TypeATCD_Selectionne == "") return;
 if (m_IsModifiable==0)            return;
 if (!m_pAtcd_Code)                return;
 m_pAtcd_Code->setRubrique(m_pAtcd_Element_Selected, m_TypeATCD_Selectionne);
 // L'affichage se remet �  jour grâce au signal de Atcd_Code.
}


//------------------------------------------- ATCD_createPopup ----------------------------------------
/*! \brief Créer un popup en relation avec l'item ATCD sélectionné.
 *  \todo gestion des droits
*/
void Dock_Menu::ATCD_createPopup(ThemePopup* pPopup, CPrtQListViewItem* pCPrt)
{ ThemePopup* m_Popup_ATCD = new ThemePopup(pPopup);
  if (! ((m_Droits_All) || (m_Droits_Modif_ATCD) ) )
        pPopup->insertItem( Theme::getIconListLocked(),
                            TR("Vous n'avez pas le droit de modifier les antécédents"));
  if (!m_pAtcd_Code) return;
  if ((m_IsModifiable!=0) && ((m_Droits_All) || (m_Droits_Create_ATCD)) )
  {  // Ajouter de nouveaux ATCD
      m_Popup_ATCD->insertItem(        Theme::getIcon( "Cim10All_Icon.png"),
                                TR("Ajouter un antécédent CIM 10"),
                                this, SLOT(ATCD_addCIM10() ) );
      m_Popup_ATCD->insertItem(        G_pCApp->m_IconTherapeutique,
                                 TR("Ajouter une allergie"),
                                this, SLOT( ATCD_addAllergie() ) );
      m_Popup_ATCD->insertItem(        Theme::getIconListItem(),
                                TR("Ajouter un antécédent texte libre"),
                                this, SLOT( ATCD_addTextuel() ) );
      if ((m_IsFeminin) && (!m_pAtcd_Code->m_Grossesse))
      { m_Popup_ATCD->insertItem(        Theme::getIcon("16x16/enceinte.png"),
                                TR("Définir comme enceinte"),
                                this, SLOT( ATCD_modifyDateGrossesse() ) );
      }
      pPopup->insertItem( Theme::getIconListAdd(), TR("Ajouter"), m_Popup_ATCD);

  } // fin if isModifiable
  else
  {  pPopup->insertItem(Theme::getIconListWarning(), TR("Le dossier est verrouillé") );
  }

  // Si l'on clique sur le dernier item d'une hiérarchie
  if (pCPrt->childCount() == 0)
  {  // D'abord éliminer la grossesse et l'allaitement
     pPopup->insertSeparator();
     if (pCPrt->getLibelle() == (TR("Grossesse en cours")))
     { if ((m_IsModifiable!=0) && ((m_Droits_All) || (m_Droits_Modif_ATCD)) )
        { pPopup->insertItem(        Theme::getIconListDelete(),
                                TR("La grossesse n'est plus en cours"),
                                this, SLOT(ATCD_endGrossesse() ) );
          pPopup->insertSeparator();
        }

        // Remettre le terme pour que d'un clique on puisse modifier la DDR
        pPopup->insertItem( Theme::getIcon("16x16/calendrier.png"),
                            QString( TR("DDR le %1, Terme de %2") )
                            .arg(        m_pAtcd_Code->m_DDR.toString("dd MMM yyyy") ,
                            m_pAtcd_Code->getTermeObstetrical()),
                            this, SLOT(ATCD_modifyDateGrossesse() ) );
     }
     else if (pCPrt->getLibelle() == (TR("Allaitement en cours")) )
     { if ((m_IsModifiable!=0) && ((m_Droits_All) || (m_Droits_Modif_ATCD)) )
        { pPopup->insertItem(         Theme::getIconListDelete(),
                                TR("L'allaitement n'est plus en cours"),
                                this, SLOT(ATCD_endAllaitement() ) );
        }
     }
     else
     {if (m_pAtcd_Element_Selected) m_pAtcd_Element_Selected = 0;
      pPopup->insertItem( Theme::getIconListView(), TR("Visualiser"),this, SLOT( ATCD_view() ) );
      // Récupère l'Atcd selon son ID_ATCD
      if (m_pAtcd_Code)
      { m_pAtcd_Element_Selected = m_pAtcd_Code->getAtcd_byID( pCPrt->get_ID_Rublist().toInt() );
        if (!m_pAtcd_Element_Selected) return;
        // Propose modification
        if ((m_IsModifiable!=0) && ((m_Droits_All) || (m_Droits_Modif_ATCD)) )
        {  pPopup->insertItem( Theme::getIconListEdit() ,
                                TR("Modifier cet antécédent"),
                                this, SLOT( ATCD_modifyIt() )
                           );
        }
        // Gestion de la date de l'ATCD
        if (m_pAtcd_Element_Selected->m_DateDeb.isValid() )
        { pPopup->insertItem( Theme::getIconListDateTime(),
                             QString(TR("En date du %1 (Modifier)"))
                                .arg(m_pAtcd_Element_Selected->m_DateDeb.toString("dd MMM yyyy")),
                             this, SLOT ( ATCD_modifyDate() )
                           );
        }
        else
        { if ((m_IsModifiable!=0) && ((m_Droits_All) || (m_Droits_Modif_ATCD)) )
             pPopup->insertItem( Theme::getIconListDateTime(),
                             TR("Définir une date de début"),
                             this, SLOT ( ATCD_modifyDateDeb() )
                           );
        }
        if (m_pAtcd_Element_Selected->m_DateFin.isValid() )
        { pPopup->insertItem( Theme::getIconListDateTime(),
                             QString(TR("Terminé le %1 (Modifier)"))
                                .arg(m_pAtcd_Element_Selected->m_DateFin.toString("dd MMM yyyy")),
                             this, SLOT ( ATCD_modifyDateFin() )
                           );
        }
        else
        { if ((m_IsModifiable!=0) && ((m_Droits_All) || (m_Droits_Modif_ATCD)) )
             pPopup->insertItem( Theme::getIconListDateTime(),
                             TR("Définir une date de fin"),
                             this, SLOT ( ATCD_modifyDateFin() )
                           );
        }

        // Gestion du commentaire
        if (m_pAtcd_Element_Selected->m_Commentaire != "")
        { pPopup->insertItem( Theme::getIcon(("16x16/commentaire.png")),
                                m_pAtcd_Element_Selected->m_Commentaire,
                                this, SLOT(ATCD_modifyCommentaire() )
                           );
        }
        else
        { if ((m_IsModifiable!=0) && ((m_Droits_All) || (m_Droits_Modif_ATCD)) )
             pPopup->insertItem( Theme::getIcon("16x16/commentaire.png"),
                                 TR("Définir un commentaire"),
                                 this, SLOT(ATCD_modifyCommentaire() )
                               );
        }
        // Gestion des rubriques
        if ((m_IsModifiable!=0) && ((m_Droits_All) || (m_Droits_Modif_ATCD)) )
        { ATCDRubriques_createPopup( pPopup );
        }
        // Gestion de l'ALD
        if ((m_IsModifiable!=0) && ((m_Droits_All) || (m_Droits_Modif_ATCD)) )
        { m_Popup->insertSeparator ();
          m_Popup->insertItem( Theme::getIcon("16x16/ald_on.png"),  tr("En rapport avec une pathologie ALD"),      this, SLOT( ATCD_ald_on()),         CTRL+Key_L );
          m_Popup->insertItem( Theme::getIcon("16x16/ald_off.png"), tr("Sans rapport avec une pathologie ALD"),    this, SLOT( ATCD_ald_off()),        CTRL+Key_O );
        }
        // Effacer un ATCD
        if ((m_IsModifiable!=0) && ((m_Droits_All) || (m_Droits_Modif_ATCD)) )
        { m_Popup->insertSeparator();
          // Effacer l'ATCD
          pPopup->insertItem(   Theme::getIconListDelete() ,
                                TR("Effacer un antécédent"),
                                this, SLOT( ATCD_eraseIt() )
                           );
        } // fin if isModifiable
       } // fin if m_pAtcd_Code
      } // Fin n'est pas grossesse, n'est pas allaitement
  } // fin childcount == 0
}

//------------------------------------------- ATCD_modifyDateGrossesse ----------------------------------------
/*! \brief Modification de la date des dernières règles.
*/
void Dock_Menu::ATCD_modifyDateGrossesse()
{ if (m_IsModifiable==0)                           return;
  if (!((m_Droits_All) || (m_Droits_Modif_ATCD)) ) return;
  if (!m_pAtcd_Code)                               return;
  if (!m_pAtcd_Element_Selected)                   return;
  if (!m_pAtcd_Code->m_Grossesse) m_pAtcd_Code->setGrossesse(TRUE);
  m_pAtcd_Code->changeDDR((QWidget*)this);
  // L'affichage se remet �  jour grâce au signal de Atcd_Code.
}

//------------------------------------------- ATCD_modifyCommentaire ----------------------------------------
/*! \brief Connecte avec Atcd_Code la modification du commentaire de l'antécédent sélectionné.
*/
void Dock_Menu::ATCD_modifyCommentaire()
{ if (m_IsModifiable==0)                           return;
  if (!((m_Droits_All) || (m_Droits_Modif_ATCD)) ) return;
  if (!m_pAtcd_Code)                               return;
  if (!m_pAtcd_Element_Selected)                   return;
  m_pAtcd_Code->changeCommentaire(m_pAtcd_Element_Selected);
  // L'affichage se remet �  jour grâce au signal de Atcd_Code.
}

//------------------------------------------- ATCD_eraseIt ----------------------------------------
/*! \brief Connecte avec Atcd_Code la suppression de l'antécédent sélectionné.
*/
void Dock_Menu::ATCD_eraseIt()
{ if (m_IsModifiable==0)                           return;
  if (!((m_Droits_All) || (m_Droits_Modif_ATCD)) ) return;
  if (!m_pAtcd_Code)                               return;
  if (!m_pAtcd_Element_Selected)                   return;
  m_pAtcd_Code->eraseAtcd(m_pAtcd_Element_Selected);
  // L'affichage se remet �  jour grâce au signal de Atcd_Code.
}


//------------------------------------------- ATCD_modifyDate ----------------------------------------
/*! \brief Connecte avec Atcd_Code laq modification de la date de l'antécédent sélectionné.
*/
void Dock_Menu::ATCD_modifyDateDeb()
{ if (m_IsModifiable==0)                           return;
  if (!((m_Droits_All) || (m_Droits_Modif_ATCD)) ) return;
  if (!m_pAtcd_Code)                               return;
  if (!m_pAtcd_Element_Selected)                   return;
  m_pAtcd_Code->modifyDateDeb((QWidget*)this,m_pAtcd_Element_Selected);
  // L'affichage se remet �  jour grâce au signal de Atcd_Code.
}
//------------------------------------------- ATCD_modifyDate ----------------------------------------
/*! \brief Connecte avec Atcd_Code laq modification de la date de l'antécédent sélectionné.
*/
void Dock_Menu::ATCD_modifyDateFin()
{ if (m_IsModifiable==0)                           return;
  if (!((m_Droits_All) || (m_Droits_Modif_ATCD)) ) return;
  if (!m_pAtcd_Code)                               return;
  if (!m_pAtcd_Element_Selected)                   return;
  m_pAtcd_Code->modifyDateFin((QWidget*)this,m_pAtcd_Element_Selected);
  // L'affichage se remet �  jour grâce au signal de Atcd_Code.
}
//------------------------------------------- ATCD_ald_on() ----------------------------------------
/*! \brief Connecte avec Atcd_Code laq modification de la date de l'antécédent sélectionné.
*/
void Dock_Menu::ATCD_ald_on()
{ if (m_IsModifiable==0)                           return;
  if (!((m_Droits_All) || (m_Droits_Modif_ATCD)) ) return;
  if (!m_pAtcd_Code)                               return;
  if (!m_pAtcd_Element_Selected)                   return;
  m_pAtcd_Code->setALD(m_pAtcd_Element_Selected, tr("ALD"));
  // L'affichage se remet �  jour grâce au signal de Atcd_Code.
}
//------------------------------------------- ATCD_ald_off() ----------------------------------------
/*! \brief Connecte avec Atcd_Code laq modification de la date de l'antécédent sélectionné.
*/
void Dock_Menu::ATCD_ald_off()
{ if (m_IsModifiable==0)                           return;
  if (!((m_Droits_All) || (m_Droits_Modif_ATCD)) ) return;
  if (!m_pAtcd_Code)                               return;
  if (!m_pAtcd_Element_Selected)                   return;
  m_pAtcd_Code->setALD(m_pAtcd_Element_Selected, "");
  // L'affichage se remet �  jour grâce au signal de Atcd_Code.
}
//------------------------------------------- ATCD_view ----------------------------------------
/*! \brief Connecte avec Atcd_Code l'affichage des informations de l'antécédent sélectionné.
*/
void Dock_Menu::ATCD_view()
{ if (!m_pAtcd_Code)                              return;
  if (!((m_Droits_All) || (m_Droits_View_ATCD)) ) return;
  CPrtQListViewItem* CPrtItem = (CPrtQListViewItem*) m_ListView_AccesRapide->selectedItem();
  if (CPrtItem==0)                                return;
  m_pAtcd_Code->viewATCD( (QWidget*)this, CPrtItem->get_ID_Rublist().toInt() );
}

//------------------------------------------- ATCD_endGrossesse ----------------------------------------
/*! \brief Connecte avec Atcd_Code l'arrêt de la grossesse.
*/
void Dock_Menu::ATCD_endGrossesse()
{ if (m_IsModifiable==0)                           return;
  if (!((m_Droits_All) || (m_Droits_Modif_ATCD)) ) return;
  if (!m_pAtcd_Code)                               return;
  m_pAtcd_Code->setGrossesse(FALSE);
  // L'affichage se remet �  jour grâce au signal de Atcd_Code.
}

//------------------------------------------- ATCD_endAllaitement ----------------------------------------
/*! \brief Connecte avec Atcd_Code l'arrêt de l'allaitement.
*/
void Dock_Menu::ATCD_endAllaitement()
{ if (m_IsModifiable==0) return;
  if (!((m_Droits_All) || (m_Droits_Modif_ATCD)) ) return;
  if (!m_pAtcd_Code) return;
  m_pAtcd_Code->setAllaitement(FALSE);
  // L'affichage se remet �  jour grâce au signal de Atcd_Code.
}

//------------------------------------------- ATCD_modifyIt ----------------------------------------
/*! \brief Connecte avec Atcd_Code la modification d'un ATCD qqsoit son type (CIM10, allergie, texte).
*/
void Dock_Menu::ATCD_modifyIt()
{ if (m_IsModifiable==0) return;
  if (!((m_Droits_All) || (m_Droits_Modif_ATCD)) ) return;
  if (!m_pAtcd_Code) return;
  // Appel le Widget d'ajout de l'ATCD
  if (!m_pAtcd_Element_Selected) return;
  m_pAtcd_Code->modifyAtcd((QWidget*)this, m_pAtcd_Element_Selected);
  // L'affichage se remet �  jour grâce au signal de Atcd_Code.
}

//------------------------------------------- ATCD_addCIM10 ----------------------------------------
/*! \brief Connecte avec Atcd_Code l'ajout d'un ATCD en mode CIM 10.
*/
void Dock_Menu::ATCD_addCIM10()
{ if (m_IsModifiable==0) return;
  if (!((m_Droits_All) || (m_Droits_Create_ATCD)) ) return;
  if (!m_pAtcd_Code) return;
  // Appel le Widget d'ajout de l'ATCD
  m_pAtcd_Code->addATCD_CIM10();
  // L'affichage se remet �  jour grâce au signal de Atcd_Code.
}

//------------------------------------------- ATCD_addTextuel ----------------------------------------
/*! \brief Connecte avec Atcd_Code l'ajout d'un ATCD en mode textuel.
*/
void Dock_Menu::ATCD_addTextuel()
{ if (m_IsModifiable==0) return;
  if (!((m_Droits_All) || (m_Droits_Create_ATCD)) ) return;
  if (!m_pAtcd_Code) return;
  // Appel le Widget d'ajout de l'ATCD
  m_pAtcd_Code->addATCD_Textuel((QWidget*)this);
  // L'affichage se remet �  jour grâce au signal de Atcd_Code.
}

//------------------------------------------- ATCD_addAllergie ----------------------------------------
/*! \brief Connecte avec Atcd_Code l'ajout d'un ATCD allergique.
*/
void Dock_Menu::ATCD_addAllergie()
{ if (m_IsModifiable==0) return;
  if (!((m_Droits_All) || (m_Droits_Create_ATCD)) ) return;
  if (!m_pAtcd_Code) return;
  // Appel le Widget d'ajout de l'ATCD
  m_pAtcd_Code->addATCD_Allergie();
  // L'affichage se remet �  jour grâce au signal de Atcd_Code.
}

//---------------------------------- m_ListView_AccesRapide_clicked ---------------------------------------------------
/*! \brief Envoie un signal pour commander l'ouverture, dans le GroupView ou la CMDI concernée, de l'item sélectionné.
 * \sa CMDI_GroupedView::Slot_ChangeViewToRub,  DrTux::Slot_ActiverDocument,  CMDI_Generic::SetCurentDocByPrimkey()
*/
void Dock_Menu::m_ListView_AccesRapide_clicked( QListViewItem * item , const QPoint & , int  )
{ // Si QListViewItem est un root on arrête
  if (!item) return;
  QListViewItem* parent    = item->parent();
  CPrtQListViewItem* pCPrt = (CPrtQListViewItem*)item;

  // Si l'utilisateur clique sur une entête
  if (parent == 0 )
  { // Si aucun Child fait surgir la CMDI concernée
    if ((pCPrt->childCount() == 0) || (m_ClickBehavior==0))
    { // ouvre la CMDI
      QString tmp = pCPrt->text(0);
      QString rubName = "";
      if ((tmp == TR("Antécédents")) || (tmp == TR("Variables")) || (tmp.find(TR("Terrains"))!=-1) )
      { rubName = TR("Terrain"); }
      else { G_pCApp->ApproximativeRubNameToType(tmp , &rubName); }
      emit Sign_ActiverRubrique( rubName, 0 );
      return;
    }
    else // if (m_ClickBehavior==1)
    { // ouvre complètement la branche
      pCPrt->setOpen( !item->isOpen() );
    }
  }
  else
  { QListViewItem * parentItem = item;
    while( parentItem->depth() != 0 )
         {parentItem = parentItem->parent();
         }
    CPrtQListViewItem* pCPrtItem = (CPrtQListViewItem*) item;
    if (!pCPrtItem) return;
    QString pkDoc = pCPrtItem->getPkDoc();
    QString tmp   = parentItem->text(0);
    // Avant de dire aux CMDI de se mettre �  jour
    // Si nécessaire recalcul le terrain après s'être assurer de ne rien avoir �  sauvegarder
    if (tmp.contains("Terrains") != 0)
    { if ( m_pAtcd_Code->isModified() )
         { checkItemToSave();
         }
      getTerrain( pCPrtItem->getPkDoc() );
      createMenu();
    }
    QString rubName = "";

    if ((tmp == TR("Antécédents")) || (tmp == TR("Variables")) || (tmp.find(TR("Terrains"))!=-1) )
    { rubName = TR("Terrain"); }
    else { G_pCApp->ApproximativeRubNameToType(tmp , &rubName); }

    if (rubName.length() )
        emit Sign_ActiverDocument( pkDoc ,  rubName , 0);
  }
}

//---------------------------------- m_Text_Note_textChanged ------------------------------------------
/*! \brief Capture l'état de modification de la note et émet le signal de Synchronisation de la note. */
void Dock_Menu::m_Text_Note_textChanged()
{ m_Notes_Changed = TRUE;
  emit Sign_NoteToBeSynchronized(0, m_Text_Note->text() );
}

//--------------------------------- setModifiedState --------------------------------------------------
void  Dock_Menu::setModifiableState(int state)
{ m_IsModifiable = state;}

/*! \brief Fonction d'analyse et de gestion des droits utilisateurs.
 *  Cette fonction une tonne de booléens représentant les droits utilisateur.
*/
void  Dock_Menu::SetInterfaceOnDroits(const QString &droits)
{ m_Droits_All =                       G_pCApp->IsThisDroitExist( droits, "adm");
  if (!m_Droits_All)
  {//qWarning("Pas tous les droits");
    m_Droits_Create_ATCD =             G_pCApp->IsThisDroitExist( droits, "etc");
    m_Droits_Create_Courriers =        G_pCApp->IsThisDroitExist( droits, "cec");
    m_Droits_Create_Certificats =      G_pCApp->IsThisDroitExist( droits, "coc");
    m_Droits_Create_Documents =        G_pCApp->IsThisDroitExist( droits, "doc");
    m_Droits_Create_Observations =     G_pCApp->IsThisDroitExist( droits, "obc");
    m_Droits_Create_Prescriptions =    G_pCApp->IsThisDroitExist( droits, "orc");

    m_Droits_View_ATCD =               G_pCApp->IsThisDroitExist( droits, "atv");
    m_Droits_View_Certificats =        G_pCApp->IsThisDroitExist( droits, "cev");
    m_Droits_View_Courriers =          G_pCApp->IsThisDroitExist( droits, "cov");
    m_Droits_View_Identite =           G_pCApp->IsThisDroitExist( droits, "idv");
    m_Droits_View_Documents =          G_pCApp->IsThisDroitExist( droits, "dov");
    m_Droits_View_Observations =       G_pCApp->IsThisDroitExist( droits, "obv");
    m_Droits_View_Prescriptions =      G_pCApp->IsThisDroitExist( droits, "orv");
    m_Droits_View_Variables =          G_pCApp->IsThisDroitExist( droits, "tvv");
    m_Droits_View_TTTFond =            G_pCApp->IsThisDroitExist( droits, "ttv");

    m_Droits_Modif_Variables =         G_pCApp->IsThisDroitExist( droits, "tvc");
    m_Droits_Modif_TTTFond =           G_pCApp->IsThisDroitExist( droits, "ttc");
    m_Droits_Modif_ATCD =              G_pCApp->IsThisDroitExist( droits, "atc");
  }

  // Rafraîchit le menu
  createMenu();
}


//------------------------------------------- but_ZoomPlus_clicked --------------------------------------
/*! \brief Augmente la police de caractère du menu d'accès rapide
*/
void Dock_Menu::but_ZoomPlus_clicked()
{ QFont f = m_ListView_AccesRapide->font();
  int s = f.pointSize() + 1;
  f.setPointSize( s ) ;
  m_ListView_AccesRapide->setFont(f);
  m_ListView_AccesRapide->triggerUpdate();
  WRITE_USER_PARAM  (&USER_PARAM, DOCK_MENU_SECTION_INI, "DefaultFontSize", QString::number(s));
  UPDATE_USER_PARAM (&USER_PARAM, G_pCApp->m_User );
}

//------------------------------------------- but_ZoomMinus_clicked --------------------------------------
/*! \brief Augmente la police de caractère du menu d'accès rapide
*/
void Dock_Menu::but_ZoomMinus_clicked()
{ QFont f = m_ListView_AccesRapide->font();
  int s = f.pointSize() - 1;
  f.setPointSize( s ) ;
  m_ListView_AccesRapide->setFont(f);
  m_ListView_AccesRapide->triggerUpdate();
  WRITE_USER_PARAM  (&USER_PARAM, DOCK_MENU_SECTION_INI, "DefaultFontSize", QString::number(s));
  UPDATE_USER_PARAM (&USER_PARAM, G_pCApp->m_User );
}

//------------------------------------------- but_ZoomDefault_clicked --------------------------------------
/*! \brief Augmente la police de caractère du menu d'accès rapide
*/
void Dock_Menu::but_ZoomDefault_clicked()
{ QFont f = m_ListView_AccesRapide->font();
  f.setPointSize( m_DefaultFontSize );
  m_ListView_AccesRapide->setFont(f);
  m_ListView_AccesRapide->triggerUpdate();
}


//------------------------------------------- m_LineEdit_ToSearch_returnPressed ----------------------------
/*! \brief Permet de rechercher un patient dans la liste des patients connus. Si la fenêtre Choix Patients n'est pas ouverte -> l'ouvre. Demande la recherche saisie.
 *  \sa DrTux::OnActiverOrCreateRubrique, FormRubChoixPatient::Slot_SearchFor
*/
void Dock_Menu::m_LineEdit_ToSearch_returnPressed()
{ emit Sign_ActiverRubrique(TR("Choix Patients"), 0);
  emit Sign_SearchPatient( m_LineEdit_ToSearch->text() );
}


//------------------------------------------- but_NewIdent_clicked ----------------------------
/*! \brief Permet de créer un nouveau dossier
*/
void Dock_Menu::but_NewIdent_clicked()
{ emit Sign_CreateNewIdent();
}

//------------------------------------------- but_NewIdent_clicked ----------------------------
/*! \brief Permet de créer un nouveau dossier
*/
void Dock_Menu::synchronizeNoteWith(const char* txt)
{ m_Text_Note->setText( txt );
}


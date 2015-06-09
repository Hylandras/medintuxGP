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

/*! \file
 * Contient les fonctions de la classe FormConfig  (FormConfig.ui et FormConfig.ui.h)
*/

/*! \class FormConfig
 *  \brief Donne acces a la classe FormConfig qui est un moteur de verification des fichiers de parametres (.ini).
 *  \author Eric MAEKER (stop in 31-08-2008) and Roland Sevin for OS10 and Windows adaptation and futures .
 *   La classe fourni les fonctions de verification des donnees des fichiers *.ini (leur presence et leur validite) pour :
 *	- l'ensemble des parametres,
 *	- quelques classes externes. Exemple : une fonction de verification est disponible pour la classe CDevilFormPDF.
 *   En cas d'erreur, vous pouvez choisir d'afficher la fenêtre ou d'ignorer l'erreur (la fenêtre ne s'affiche pas automatiquement en cas d'erreur).
 *  Les modifications faites peuvent être validees. Elles sont alors repercutees dans les variables parametres (m_IniParam et m_IniUser) qui peuvent être recuperes pour sauvegarde a l'exterieur de cette classe.
 *  Utilisation de la classe :
 *  - Definir la classe
 *  - Definir les structures de parametres de l'appli et de l'utilisateur ( setParams() )
 *  - Si la classe est validee (result == QDialog::Accepted) --> recupere les structures parametres ( getParams() ) sauvegarde sinon rien.
*/

/*! \var FormConfig::m_IniParam
 *  \brief Contient le fichier ini de l'application sur lequel travailler.
 *  Lors de l'initilisation de la classe, les parametres sont recuperes depuis G_pCApp->m_DrTuxIni. Durant son execution cette classe travaille uniquement sur cette variable. Lors de la demande de sauvegarde, le fichier ini est mis a jour avec cette variable et G_pCApp->m_DrTuxIni est initialise avec cette variable (pour eviter une relecture du fichier ini).
 *  \todo Emettre un signal pour que tout le monde relise ses parametres et s'ajuste en fonction...
*/

/*! \var FormConfig::m_IniUser
 *  \brief Contient le fichier ini de l'utilisateur sur lequel travailler.
 *  \todo Emettre un signal pour que tout le monde relise ses parametres et s'ajuste en fonction...
*/

/*! \var FormConfig::m_CheckErreur
 *  \brief booleen qui reÃ§oit TRUE si une erreur est survenue lors de la verification des parametres
 *  \sa checkParam
*/

/*! \var FormConfig::m_erreurDrTuxIni
 *  \brief booleen qui indique si existe une erreur dans le fichier ini
*/

/*! \var FormConfig::m_varErreurDrTuxIni
 *  \brief Message d'erreur affichable dans la page d'analyse
*/

/*! \var FormConfig::m_SplashScreen
 *  \brief Affiche ou non le splashScreen de DrTux a chaque demarrage. Valeurs possibles :
 *  \sa m_IniParam
*/

/*! \var FormConfig::m_RepGlossaire
 *  \brief
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_RepTmpGene
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_CheminAide
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_MenuObs
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_MenuPrescr
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_MenuDoc
 *  \brief Liste par defaut pour les documents
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_MenuImage
 *  \brief Liste par defaut pour les images
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_DefautObs
 *  \brief Masque par defaut pour les observations
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_DefautTerrain
 *  \brief Masque par defaut pour les prescriptions
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_CCAMLibPref
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_CCAMLibPostf
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_CCAMCodePref
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_CCAMCodePostf
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_CCAMPanneau
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_CCAMIni
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_CCAMExe
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_PDFPath
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_PDFVisu
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_PDFtk
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_PDFTmp
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_Contact
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_GestUser
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_NouveauDossier
 *  \brief "oui" / "non" -> DrTux peut ou ne peut pas creer et detruire les dossiers patients
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_DossierConcurrent
 *  \brief Utilise par Manager avant l'ouverture d'un dossier (ouverture conflictuelle)
 *  \sa m_IniParam
*/
/*! \var FormConfig::m_VerifDoss
 *  \brief NON UTILISE
 *  \sa m_IniParam
*/


//////// VARIABLES DE L'UTILISATEUR ///////////
/*! \var FormConfig::m_Dock_TabRubrique
 *  \sa m_IniUser
*/
/*! \var FormConfig::m_DockMenu_OnScreen
 *  \sa m_IniUser
*/
/*! \var FormConfig::m_DockMenu_ShowTerrain
 *  \sa m_IniUser
*/
/*! \var FormConfig::m_DockMenu_ShowObs
 *  \sa m_IniUser
*/
/*! \var FormConfig::m_DockMenu_ShowPrescr
 *  \sa m_IniUser
*/
/*! \var FormConfig::m_DockMenu_ShowDoc
 *  \sa m_IniUser
*/
/*! \var FormConfig::m_DockMenu_ShowIdent
 *  \sa m_IniUser
*/
/*! \var FormConfig::m_DockMenu_Glossaire
 *  \sa m_IniUser
*/
/*! \var FormConfig::m_DockMenu_DefaultFontSize
 *  \sa m_IniUser
*/
/*! \var FormConfig::m_DockMenu_ClickBehavior
 *  \brief Fonctionnalite du menu d'acces rapide : lorsque l'utilisateur clique sur un item "root" -> ouvre soit la CMDI, soit la branche cliquee.
 *  Valeurs possibles : "openCMDI" ou "openCloseList"
 *  \sa m_IniUser
*/
#include <qdir.h>
#include <qfileinfo.h>
#include <qfiledialog.h>
#include <qlistview.h>

#include "../../MedinTuxTools/CGestIni.h"
#include "../../MedinTuxTools/CHtmlTools.h"
#include "ui/DlgListOff_Listes.h"
#include "../../drtux/src/CMoteur_Base.h"       // inclu aussi      #include "CRubList.h"
#include "CApp.h"

//#include "FormDlgListOff_Listes.h"

//! Valeur par defaut de SplasScreen, rubrique Connexion.  ..  drtux.ini
#define DEFAUT_SPLASH		"oui"
//! Valeur par defaut de Nouveau Dossier, rubrique Observation.  ..  drtux.ini
#define DEFAUT_NOUVEAUDOSSIER	"oui"
//! Valeur par defaut de Acces concurrent, rubrique Gestion des dossiers.  ..  drtux.ini
#define DEFAUT_CONCURRENT	"possible"
//! Valeur par defaut de Verifier dossier, rubrique Observation.  ..  drtux.ini
#define DEFAUT_VERIFDOSS	"non"

//! Valeur par defaut de repertoire temporaire general.  ..  drtux.ini
#define DEFAUT_REPTMPGENE	"../../tmp"
//! Valeur par defaut de repertoire pour le glossaire.  ..  drtux.ini
#define DEFAUT_REPGLOSSAIRE	"../../Glossaire"
//! Valeur par defaut de chemin vers l'aide.  ..  drtux.ini
#define DEFAUT_CHEMINAIDE	"../../Doc/index.html"

//! Valeur par defaut de ListObs, rubrique MenuContextuel.  ..  drtux.ini
#define DEFAUT_MENUOBS		""
//! Valeur par defaut de ListVigie, rubrique MenuContextuel.  ..  drtux.ini
#define DEFAUT_MENUVIGIE        ""
//! Valeur par defaut de ListIdentification, rubrique MenuContextuel.  ..  drtux.ini
#define DEFAUT_MENUIDENT        ""
//! Valeur par defaut de ListPrescr, rubrique MenuContextuel.  ..  drtux.ini
#define DEFAUT_MENUPRESCR	""
//! Valeur par defaut de ListDocuments, rubrique MenuContextuel.  ..  drtux.ini
#define DEFAUT_MENUDOC		"Gestion des documents"
//! Valeur par defaut de ListImages, rubrique MenuContextuel.  ..  drtux.ini
#define DEFAUT_MENUIMAGE	"Gestion des images"


//! Valeur par defaut de repertoire pour les PDF, rubrique Formulaire PDF.  ..  drtux.ini
#define DEFAUT_REPPDF		"../../pdf"
//! Valeur par defaut de repertoire temporaire pour les PDF, rubrique Formulaire PDF.  ..  drtux.ini
#define DEFAUT_REPPDFTMP	"../../pdf/tmp"
//.................. chemin par defaut OS dependant ...........................
#ifdef Q_WS_MAC
    //! Valeur par defaut de chemin vers pdf toolkit, rubrique Formulaire PDF.  ..  drtux.ini
    #define FORM_PDF            "Formulaires PDF Mac"
    #define DEFAUT_PDFTK		"/usr/local/bin/pdftk"
    //! Valeur par defaut de chemin vers appli visualisatrice de PDF, rubrique Formulaire PDF.  ..  drtux.ini
    #define DEFAUT_PDFVISU		"/Applications/Preview.app"
    #define EXTENSION_EXE       ".app"
#endif
#ifdef Q_WS_WIN
    //! Valeur par defaut de chemin vers pdf toolkit, rubrique Formulaire PDF.  ..  drtux.ini
    #define FORM_PDF            "Formulaires PDF Win"
    #define DEFAUT_PDFTK		"../../pdf/pdftk.exe"
    //! Valeur par defaut de chemin vers appli visualisatrice de PDF, rubrique Formulaire PDF.  ..  drtux.ini
    #define DEFAUT_PDFVISU		"C:/Program Files/Adobe/Acrobat 5.0/Reader/AcroRd32.exe"
    #define EXTENSION_EXE       ".exe"
#endif
#ifdef Q_WS_X11
    //! Valeur par defaut de chemin vers pdf toolkit, rubrique Formulaire PDF.  ..  drtux.ini
    #define FORM_PDF            "Formulaires PDF Lin"
    #define DEFAUT_PDFTK		"/usr/bin/pdftk"
    //! Valeur par defaut de chemin vers appli visualisatrice de PDF, rubrique Formulaire PDF.  ..  drtux.ini
    #define DEFAUT_PDFVISU		"/usr/bin/kpdf"
    #define EXTENSION_EXE      ""
#endif

//! Valeur par defaut de libelle prefixe, rubrique CCAM.  ..  drtux.ini
#define DEFAUT_CCAMLBLPRE	QString::fromUtf8("oÂ° <i><u>Libelle CCAM</u> :")
//! Valeur par defaut de libelle postfixe, rubrique CCAM.  ..  drtux.ini
#define DEFAUT_CCAMLBLPOST	" </i>^?"
//! Valeur par defaut de code prefixe, rubrique CCAM.  ..  drtux.ini
#define DEFAUT_CCAMCODEPRE	"<br />        <b>::["
//! Valeur par defaut de code postfixe, rubrique CCAM.  ..  drtux.ini
#define DEFAUT_CCAMCODEPOST	"]::</b>"
//! Valeur par defaut de panneau a activer, rubrique CCAM.  ..  drtux.ini
#define DEFAUT_CCAMPANNEAU	"&Thesaurus"
//! Valeur par defaut de chemin vers exe, rubrique CCAM.  ..  drtux.ini
#define DEFAUT_CCAMEXE		"../../ccamview/bin/ccamview"
//! Valeur par defaut de chemin vers ini, rubrique CCAM.  ..  drtux.ini
#define DEFAUT_CCAMINI		"../../drtux/bin/Ressources/ccam/ccamview.ini"

//! Valeur par defaut de pathexe, rubrique Gestionnaire contacts.  ..  drtux.ini
#define DEFAUT_CONTACT		"../../personnes/bin/personnes"
//! Valeur par defaut de pathexe, rubrique Gestionnaire user.  ..  drtux.ini
#define DEFAUT_GESTUSER		"../../gest_user/bin/gest_user"

//! Valeur par defaut de Defaut, rubrique Observation.  ..  drtux.ini
#define DEFAUT_DEFAUTOBS	"$Glossaire/Observation/OBSERV_STANDARD.html"
//! N'existe pas encore dans drtux.ini
#define DEFAUT_DEFAUTPRESCR	""
//! Valeur par defaut de Defaut, rubrique Terrain.  ..  drtux.ini
#define DEFAUT_DEFAUTTERRAIN	"$Glossaire/Terrain/Pouls-TA-SAO2-Temp-Hgt-Poids.txt"
//! Valeur par defaut de Defaut, rubrique Terrain.  ..  drtux.ini
#define DEFAUT_DEFAUTIDENT	"$Glossaire/Identification/Defaut Ident.html "

//! Nom de la section pour le theme dans les parametres utilisateur
#define THEME_CFG		"Theme config"
//! Valeur par defaut de Theme.  ..  drtux.ini
#define DEFAUT_THEME		"../../Themes/Default"
//! taille de la police par defaut pour les menus
#define DEFAUT_MENU_FNTSIZE	"9"

//! Nom de la section pour Rapid_Menu
#define DOCK_RAPID_MENU		"Dock_Rapid_Access"
//! Nom de la section pour Dock_Glossaire
#define DOCK_GLOSSAIRE		"Dock_Glossaire"

//! Nom de la section Tab_Rubriques
#define  TAB_RUBRIQUES           "Tab Rubriques"

//! Nom de la section pour Dock_Rubriques
#define DOCK_RUBRIQUES		"Dock_Rubriques"

//! Nom de la section pour les menus constextuels
#define CONTEXT_MENU		"MenuContextuel"

//! Nom de la section pour Dock_Glossaire
#define DEFAUT_DOCK_GLOSSAIRE	"Normal"

//! valeur par defaut de la section Tab_Rubriques
#define DEFAUT_TAB_RUBRIQUES     "true"

//! Nom de la section pour Dock_Access Rapid
#define DEFAUT_DOCK_RAPIDE	"Normal"
//! Nom de la section pour Dock_Access Rapid
#define DEFAUT_DOCK_RAPIDE_TRUE	"true"
//! taille de la police par defaut pour l'acces rapide
#define DEFAUT_DOCK_FNTSIZE	"9"


#define TR			QObject::tr
#define RELATIVE_PATH		TRUE
#define ABSOLUTE_PATH		FALSE


//------------------------------------ Init ---------------------------------------
/*! \brief Recupere les parametres dans le drtux.ini, puis lance le checkIni() et checkParam(). Les champs des formulaires de la fenêtre sont renseignes et les erreurs potentielles sont notifiees.
 *  \return rien.
 *   La fenêtre ne s'affiche pas apres la contruction de cette classe. Il faut appeler la fonction exec(). Cela permet d'effectuer les verifications sans alerter inutilement l'utilisateur.
 */
void FormConfig::init()
{ QString param = "";
  QString msg , section, variable, defaut = "";
  m_IniParam = "";
  m_IniUser  = "";

  // initialise toutes les variables;
  m_Dock_Glossaire    = "";
  m_MaxIamgeWidth     = "";
  m_RepDocuments      = "";
  m_Theme  = "";
  m_erreur = " *";               // schema qui sera ajoute aux labels / variables erronees
  m_erreurDrTuxIni = FALSE;
  m_varErreurDrTuxIni = "";
  m_CheckErreur = FALSE;
  m_CheckMsgErreur = "";
  m_SplashScreen = "";
  m_ZoomObs = "";
  m_ZoomPrescr = "";
  m_NouveauDossier = "";
  m_DossierConcurrent = "";
  m_CCAMLibPref = "";
  m_CCAMLibPostf = "";
  m_CCAMCodePref = "";
  m_CCAMCodePostf = "";
  m_CCAMPanneau = "";
  m_PopMenu_DefaultFontSize = "";
  /* ??..?? debut  modif Roland Variables inutiles car à recuperer directment dans les controles
  //         cela evite les erreurs de mise  jour des variables intermediaires
  m_PDFPath = "";
  m_PDFTmp = "";
  m_PDFtk = "";
  m_PDFVisu = "";
  m_GestUser = "";
  m_CheminAide = "";
  m_MenuDoc = "";
  m_MenuImage = "";
  m_MenuObs = "";
  m_MenuPrescr = "";
  m_DefautObs = "";
  m_DefautTerrain = "";
  m_RepTmpGene = "";
  m_RepGlossaire = "";
  m_Contact = "";
  m_CCAMExe = "";
  m_CCAMIni = "";

  <variables>
    <variable access="public">QString m_GestUser;</variable>
    <variable access="public">QString m_CheminAide;</variable>
    <variable access="public">QString m_Contact;</variable>
    <variable access="public">QString m_RepTmpGene;</variable>
    <variable access="public">QString m_RepGlossaire;</variable>
    <variable access="public">QString m_MenuObs;</variable>
    <variable access="public">QString m_MenuPrescr;</variable>
    <variable access="public">QString m_MenuDoc;</variable>
    <variable access="public">QString m_MenuImage;</variable>
    <variable access="public">QString m_DefautObs;</variable>
    <variable access="public">QString m_DefautTerrain;</variable>

    <variable access="public">QString m_CCAMExe;</variable>
    <variable access="public">QString m_CCAMIni;</variable>
    <variable access="public">QString m_PDFPath;</variable>
    <variable access="public">QString m_PDFTmp;</variable>
    <variable access="public">QString m_PDFtk;</variable>
    <variable access="public">QString m_PDFVisu;</variable>
 </variables>
  */

  repDefautPDF->setText("");
  repPDFTmp->setText("");
  repPdftk->setText("");
  appliPDF->setText("");
  GestUser->setText("");
  Aide->setText("");
  menuImage->setText("");
  defautVigie->setText("");
  repTmpGeneral->setText("");
  repGlossaire->setText("");
  GestContact->setText("");
  CCAMExe->setText("");
  CCAMIni->setText("");
  // ??..?? fin


  m_VerifDoss = "";
  m_Dock_TabRubrique = "";
  m_DockMenu_Glossaire = "";
  m_DockMenu_OnScreen = "";
  m_DockMenu_ShowTerrain = "";
  m_DockMenu_ShowObs = "";
  m_DockMenu_ShowPrescr = "";
  m_DockMenu_ShowDoc = "";
  m_DockMenu_ShowIdent = "";
  m_DockMenu_DefaultFontSize = "";
  m_DockMenu_ClickBehavior = "";
  spinBox_menus_FontSize->setValue(9);
   //................................ corriger erreur de Designer......................................
   line_Appli->setFrameStyle ( QFrame::HLine );            line_Appli->setFrameShadow (QFrame::Sunken );
   line_Info->setFrameStyle ( QFrame::HLine );             line_Info->setFrameShadow (QFrame::Sunken );
   line_repAppli->setFrameStyle ( QFrame::HLine );         line_repAppli->setFrameShadow (QFrame::Sunken );
   line_RepMask->setFrameStyle ( QFrame::HLine );          line_RepMask->setFrameShadow (QFrame::Sunken );
   line_Contact->setFrameStyle ( QFrame::HLine );          line_Contact->setFrameShadow (QFrame::Sunken );
   line_Demarrage->setFrameStyle ( QFrame::HLine );        line_Demarrage->setFrameShadow (QFrame::Sunken );
   line_GestDoss->setFrameStyle ( QFrame::HLine );         line_GestDoss->setFrameShadow (QFrame::Sunken );
   line_Themes->setFrameStyle ( QFrame::HLine );           line_Themes->setFrameShadow (QFrame::Sunken );
   line_MenAccRap->setFrameStyle ( QFrame::HLine );        line_MenAccRap->setFrameShadow (QFrame::Sunken );
   line_RubAct->setFrameStyle ( QFrame::HLine );           line_RubAct->setFrameShadow (QFrame::Sunken );
   line_FoncGlo->setFrameStyle ( QFrame::HLine );          line_FoncGlo->setFrameShadow (QFrame::Sunken );
   line_DocList->setFrameStyle ( QFrame::HLine );          line_DocList->setFrameShadow (QFrame::Sunken );
   line_MenuAccesRapide->setFrameStyle ( QFrame::HLine );  line_MenuAccesRapide->setFrameShadow (QFrame::Sunken );
   line_MenuGlo->setFrameStyle ( QFrame::HLine );          line_MenuGlo->setFrameShadow (QFrame::Sunken );
   line_MenuRub->setFrameStyle ( QFrame::HLine );          line_MenuRub->setFrameShadow (QFrame::Sunken );
   line_PresCCAM->setFrameStyle ( QFrame::HLine );         line_PresCCAM->setFrameShadow (QFrame::Sunken );
   line_Exemple->setFrameStyle ( QFrame::HLine );          line_Exemple->setFrameShadow (QFrame::Sunken );
   line_MenuRub_Onglet->setFrameStyle ( QFrame::HLine );   line_MenuRub_Onglet->setFrameShadow (QFrame::Sunken );
}

//----------------------------------- init_listViewRubParameters ----------------------------------------
/*! \brief Recupere les structures de parametres pour l'application et l'utilisateur. Puis prepare leur integration a l'interface et leur verification.
 */
void FormConfig::init_listViewRubParameters()
{QListViewItem *pQListViewItemRub;
 QListViewItem *pQListViewItemValue;
 QString        rubName;

 for (int i=0; i < (int)G_pCApp->m_ListRubName.size(); ++i)
     {rubName = G_pCApp->m_ListRubName[i];
      pQListViewItemRub = new QListViewItem( listViewRubParameters, rubName ,"________________________________________________________________________________________________" );
      if (pQListViewItemRub)
         { pQListViewItemRub->setPixmap (0, Theme::getIcon(QString("16x16/type_")+rubName+".png") );
           pQListViewItemRub->setOpen ( TRUE );
           //  [Observation]
           //     Path documents = /home/ro/MedinTux-V2.13-Mac-Intel-105-src/Icones
           //     Defaut = $Glossaire/Identification/A G G I R.html
           pQListViewItemValue = new QListViewItem( pQListViewItemRub, tr("1- Chemin par défaut où chercher des images "),   CGestIni::Param_ReadUniqueParam(m_IniParam, rubName , "Path documents").remove("*") );
           pQListViewItemValue = new QListViewItem( pQListViewItemRub, tr("2- Document par défaut lors première création "), CGestIni::Param_ReadUniqueParam(m_IniParam, rubName , "Defaut").remove("*") );
           //  [MenuContextuel]
           //     Observation =
           //     Prescription =
           //     Documents = Gestion des documents
           //     Identification = ° HOSPITALISATION
           pQListViewItemValue = new QListViewItem( pQListViewItemRub, tr("3- Menu contextuel fixe "),               CGestIni::Param_ReadUniqueParam(m_IniParam, "MenuContextuel", rubName).remove("*") );
           pQListViewItemValue = new QListViewItem( pQListViewItemRub, tr("4- Identificateur de type de rubrique "), QString::number(G_pCApp->m_mapNameRubType[rubName]));
           pQListViewItemValue = new QListViewItem( pQListViewItemRub, tr("5- Préfixe du droit "),                   G_pCApp->m_mapNameRubInfos[rubName]);
           pQListViewItemValue = new QListViewItem( pQListViewItemRub, tr("6- Icône de la rubrique (thème) "),       Theme::getPath()+ "16x16/type_"+rubName+".png");
           pQListViewItemValue = new QListViewItem( pQListViewItemRub, tr("7- Nom de la rubrique "),                 rubName);
          }

     }
 //listViewRubParameters
}


//----------------------------------- listViewRubParameters_doubleClicked ----------------------------------------
void FormConfig::listViewRubParameters_doubleClicked( QListViewItem *pQListViewItem, const QPoint &, int )
{if (pQListViewItem==0)                           return;
 QListViewItem *pQListViewItemParent = pQListViewItem->parent();
 if (pQListViewItemParent ==0)                    return;
 QString text    = pQListViewItem->text(0);
 int pos         = text.find("-"); if (pos==-1)   return;
 int choix       = text.left(pos).toInt();
 QString val     = pQListViewItem->text(1);
 QString type    = "";
 QString rubName = pQListViewItemParent->text(0);
 //................. recuperer le type actuel de la rubrique .....................
 QListViewItem * myChild = pQListViewItemParent->firstChild();
 while( myChild )
      {  if (myChild->text(0)[0]=='4' ) type = myChild->text(1);
         myChild = myChild->nextSibling();
      }

 switch(choix)
  {case 1:       //"1- Chemin par défaut où chercher des images"
        // QString *last_path  = 0 , QString stringPreselect ="", int prewiew =1, const QWidget *pQWidget =0
        val = G_pCApp->GetImageFileName(&val,"",1,this);
        if (val.length()) pQListViewItem->setText(1,QFileInfo(val).dirPath());
        break;
   case 2:      // "2- Document par défaut lors première création "
        val = val.replace("$Glossaire", G_pCApp->m_PathGlossaire);
        if ( val.length()==0 || !QFile::exists(val)) val = G_pCApp->m_PathGlossaire;
        val = QFileDialog::getOpenFileName( val,  "Fichiers de démarrage (*.html *.txt)",  this, tr("open file dialog"),  tr("Choose a file") );
        if ( val.length() && QFile::exists(val)) pQListViewItem->setText(1,val.replace(G_pCApp->m_PathGlossaire,"$Glossaire/"));
        break;
  case 3:      // "3- Menu contextuel fixe "
       {
        QString path = "";
        if (QDir(repGlossaire->text()).isRelative() ) path = m_PathAppli + repGlossaire->text();
        else path = repGlossaire->text();
        path = QDir::cleanDirPath(path) + INSERTION_ROOT;

        FormDlgListOff_Listes* pDlg = new FormDlgListOff_Listes(this);
        pDlg->setPath( path );

        pDlg->exec();
        if (pDlg->result() == QDialog::Accepted)
           { pQListViewItem->setText(1, pDlg->getSelectedItem() );
           }
        delete pDlg;
       }
        break;
  case 4:    // "4- Identificateur de type de rubrique "   2= Imagerie|im|90040000  PrescriPlus.png  Info_IdentificateurRubrique.html
        if (val[0]=='*' || val.startsWith("9"))
           {QString          prefixNew = "";
            if (val[0]=='*') prefixNew = "*";
            val = val.remove("*");           // on enleve l'indicateur de nouvelle rubrique
            if (CGenTools::DialogGetString(QString("$File:")+G_pCApp->m_PathAppli + "Ressources/icones/Info_IdentificateurRubrique.html",
                                            this,
                                            tr("Modifier l'identificateur de rubrique"),
                                            val)==QDialog::Rejected) return;
             if (val.length() != 8)  {G_pCApp->CouCou( tr("L'identificateur doit être long de 8 chiffres"), "$Theme/MessagePopError-01.png", 5000); return;}
             if (val[0] != '9')      {val = val.mid(1).prepend("9");}
             // ---------------- verifier si pas déjà utilisé --------------------------------
             QListViewItemIterator it( listViewRubParameters );
             QListViewItem *item_Find = 0;
             while ( it.current() )
                   { QListViewItem *item = it.current();
                     if (item->text(0)[0]=='4' && item->text(1).remove("*")==val) {item_Find = item; break;}
                     ++it;
                   }
             if (item_Find)          {G_pCApp->CouCou( tr("Cet Identificateur '%1' est déjà utilisé par '%2'").arg(val,item_Find->parent()->text(0)), "$Theme/MessagePopError-01.png", 5000);}
             pQListViewItem->setText(1,val.prepend(prefixNew));   // on remet l'indicateur de nouvelle rubrique
           }
        else
           {G_pCApp->CouCou( tr("L'identificateur de cette rubrique est non modifiable"), "$Theme/MessagePopError-01.png", 5000);
           }
        break;
  case 5:   // "5- Préfixe du droit "  Info_PrefixeRubrique.html
        if (type[0]=='*' || type[0]=='9')
           {QString          prefixNew = "";
            if (val[0]=='*') prefixNew = "*";
            val = val.remove("*");           // on enleve l'indicateur de nouvelle rubrique
            if (CGenTools::DialogGetString(QString("$File:")+G_pCApp->m_PathAppli + "Ressources/icones/Info_PrefixeRubrique.html",
                                            this,
                                            tr("Modifier le préfixe de droits d'une rubrique"),
                                            val)==QDialog::Rejected) return;
             if (val.length() != 2)  {G_pCApp->CouCou( tr("Le préfixe du droit doit être long de 2 caractères"), "$Theme/MessagePopError-01.png", 5000); return;}
             // ---------------- verifier si pas déjà utilisé --------------------------------
             QListViewItemIterator it( listViewRubParameters );
             QListViewItem *item_Find = 0;
             while ( it.current() )
                   { QListViewItem *item = it.current();
                     if (item->text(0)[0]=='5' && item->text(1).remove("*")==val) {item_Find = item; break;}
                     ++it;
                   }
             if (item_Find)          {G_pCApp->CouCou( tr("Ce préfixe de droits '%1' est déjà utilisé par '%2'").arg(val,item_Find->parent()->text(0)), "$Theme/MessagePopError-01.png", 5000);}
             pQListViewItem->setText(1,val.prepend(prefixNew));   // on remet l'indicateur de nouvelle rubrique
           }
        else
           {G_pCApp->CouCou( tr("Le préfixe de droits de cette rubrique est non modifiable"), "$Theme/MessagePopError-01.png", 5000);
           }
        break;
  case 6:   // "6- Icône (du thème) "
       {
        QString path = QFileInfo(val).dirPath();
        path         = G_pCApp->GetImageFileName(&path,"",1,this);
		
        if (path.length())   // chemin standard : Theme::getPath()+ "16x16/type_"+rubName+".png"
           { //CHtmlTools::Copy_File(path, Theme::getPath()+ "16x16", rubName+".png");
             pQListViewItem->setText(1, path);    // pour la detection du changement on se servira du fait que ce chemin et different de celui standard
           }
       }
        break;
  case 7:   //  "7- Nom de la rubrique "
       if (type[0]=='*' || type[0]=='9')
          {QString          prefixNew = "";
           if (val[0]=='*') prefixNew = "*";
           val = val.remove("*");           // on enleve l'indicateur de nouvelle rubrique
           if (CGenTools::DialogGetString(QString("$File:")+G_pCApp->m_PathAppli + "Ressources/icones/Info_NomRubrique.html",
                                           this,
                                           tr("Modifier le Nom d'une rubrique"),
                                           val)==QDialog::Rejected) return;
            if (val[0] == '*')     {G_pCApp->CouCou( tr("Le Nom d'une rubrique ne doit pas comporter de caractère étoile"), "$Theme/MessagePopError-01.png", 5000); return;}
            if (val.length() < 5)  {G_pCApp->CouCou( tr("Le Nom d'une rubrique doit comporter au moins cinq caractères"), "$Theme/MessagePopError-01.png", 5000);       return;}
            // ---------------- verifier si pas déjà utilisé --------------------------------
            QListViewItemIterator it( listViewRubParameters );
            QListViewItem *item_Find = 0;
            while ( it.current() )
                  { QListViewItem *item = it.current();
                    if (item->text(0)[0]=='7' && item->text(1).remove("*")==val) {item_Find = item; break;}
                    ++it;
                  }
            if (item_Find)          {G_pCApp->CouCou( tr("Ce nom de rubrique '%1' est déjà utilisé par '%2'").arg(val,item_Find->parent()->text(0)), "$Theme/MessagePopError-01.png", 5000); return;}
            pQListViewItem->setText(1,val.prepend(prefixNew));   // on remet l'indicateur de nouvelle rubrique
           }
        else
           {G_pCApp->CouCou( tr("Le nom de cette rubrique est non modifiable"), "$Theme/MessagePopError-01.png", 5000);
           }
        break;
  case 8:
        break;
  case 9:
        break;
  }
}

//----------------------------------- AddNewRub ----------------------------------------
/*! \brief Ajoute un nouveau type de rubrique vierge.
 */
void FormConfig::AddNewRub()
{ QListViewItem *pQListViewItemRub;
  QListViewItem *pQListViewItemValue;

  QString        rubName = "";
  if (CGenTools::DialogGetString(QString("$File:")+G_pCApp->m_PathAppli + "Ressources/icones/Info_NomRubrique.html",
                                           this,
                                           tr("Definir le Nom d'une rubrique"),
                                           rubName)==QDialog::Rejected) return;
   // ---------------- verifier si pas déjà utilisé --------------------------------
   QListViewItemIterator it( listViewRubParameters );
   QListViewItem *item_Find = 0;
   while ( it.current() )
         { QListViewItem *item = it.current();
           if (item->text(0)[0]=='7' && item->text(1).remove("*")==rubName) {item_Find = item; break;}
           ++it;
         }
  if (item_Find)             {G_pCApp->CouCou( tr("Ce nom de rubrique '%1' est déjà utilisé par '%2'").arg(rubName,item_Find->parent()->text(0)), "$Theme/MessagePopError-01.png", 5000); return;}
  if (rubName[0] == '*')     {G_pCApp->CouCou( tr("Le Nom d'une rubrique ne doit pas comporter de caractère étoile"), "$Theme/MessagePopError-01.png", 5000); return;}
  if (rubName.length() < 5)  {G_pCApp->CouCou( tr("Le Nom d'une rubrique doit comporter au moins cinq caractères"), "$Theme/MessagePopError-01.png", 5000);       return;}

 pQListViewItemRub = new QListViewItem( listViewRubParameters, rubName ,"________________________________________________________________________________________________" );
 if (pQListViewItemRub)
    { pQListViewItemRub->setPixmap (0, Theme::getIcon(QString("16x16/type_")+rubName+".png") );
      pQListViewItemRub->setOpen ( TRUE );
      //  [Observation]
      //     Path documents = /home/ro/MedinTux-V2.13-Mac-Intel-105-src/Iconestr("Nouvelle Rubrique")
      //     Defaut = $Glossaire/Identification/A G G I R.html
      pQListViewItemValue = new QListViewItem( pQListViewItemRub, tr("1- Chemin par défaut où chercher des images "),   G_pCApp->m_PathGlossaire+"ImagesFolder" );
      pQListViewItemValue = new QListViewItem( pQListViewItemRub, tr("2- Document par défaut lors première création "), G_pCApp->m_PathGlossaire+rubName+"/Defaut.html");
      //  [MenuContextuel]
      //     Observation =
      //     Prescription =
      //     Documents = Gestion des documents
      //     Identification = ° HOSPITALISATION
      pQListViewItemValue = new QListViewItem( pQListViewItemRub, tr("3- Menu contextuel fixe "),               "" );
      pQListViewItemValue = new QListViewItem( pQListViewItemRub, tr("4- Identificateur de type de rubrique "), "*90010000" );
      pQListViewItemValue = new QListViewItem( pQListViewItemRub, tr("5- Préfixe du droit "),                   "*xx");
      pQListViewItemValue = new QListViewItem( pQListViewItemRub, tr("6- Icône de la rubrique (thème) "),       Theme::getPath()+ "16x16/type_"+rubName+".png");
      pQListViewItemValue = new QListViewItem( pQListViewItemRub, tr("7- Nom de la rubrique "),                 rubName.prepend("*"));
      listViewRubParameters-> ensureItemVisible ( pQListViewItemValue );
    }
}

//----------- save_listViewRubParameters ----------------------------------------
/*! \brief Enregistre les modifications des parametres de rubrique.
 */
void FormConfig::save_listViewRubParameters()
{QListViewItem *pQListViewItemParent = listViewRubParameters->firstChild();
 QString type              = "";
 QString rubName           = "";
 int     n_rubAuto         = 1;
 QString         prefix    = "";
 QString            val    = "";
 QString            key    = "";
 QString     allDroitsStr  = "";
 QStringList listAllDroits = "";
 //................ charger la liste des droits ......................................................
 QString   pathFileDroit = CGestIni::Construct_PathBin_Module("gest_user", QFileInfo(qApp->argv()[0]).dirPath (true))+"Images/ListeDroits.txt";
 //CMoteurBase::GotoDebug();
 CGestIni::Param_UpdateFromDisk(pathFileDroit , allDroitsStr);
 //........................ virer la section de parametrage des rubriques automatiques ..............
 CGestIni::Param_RemoveSection(m_IniParam, "Rubriques Automatiques");
 while ( pQListViewItemParent )
 {
  //................. premier tour pour recuperer le type actuel de la rubrique .....................................
  QListViewItem * myChild = pQListViewItemParent->firstChild();
  while( myChild )
      {  val  = myChild->text(1);
         key  = myChild->text(0);
         if (key[0]=='4' ) type    = val;
         if (key[0]=='5' ) prefix  = val;
         if (key[0]=='7' ) rubName = val.remove('*');
         myChild = myChild->nextSibling();
      }
  //........................ creer si n'exite pas le droit ...................................
  if (allDroitsStr.find(prefix+"v")==-1)
     {allDroitsStr += tr("-Peut créer ou modifier la rubrique ") + rubName + "("+ prefix.remove('*')+"c)";
      allDroitsStr += tr("-Peut voir et afficher la rubrique ")  + rubName + "("+ prefix.remove('*')+"v)";
      allDroitsStr += tr("-Peut imprimer la rubrique ")          + rubName + "("+ prefix.remove('*')+"p)";
      allDroitsStr  = allDroitsStr.stripWhiteSpace();
      allDroitsStr  = allDroitsStr.remove("\n");
      allDroitsStr  = allDroitsStr.remove("\r");
      listAllDroits = QStringList::split("-",allDroitsStr );
      allDroitsStr  = "";
      for (int i=0; i<(int)listAllDroits.count(); ++i)
          {allDroitsStr  += QString("-") + listAllDroits[i]+"\r\n";
          }
    }
  //.............. enregistrer les modifs accumulées ....................
  //     [Rubriques Automatiques]
  //        1= Biologie|bi|90070000
  //        2= Imagerie|im|90040000

  if (type[0]=='9' || type[0]=='*')
     {val = rubName+"|"+prefix+"|"+type;
      CGestIni::Param_WriteParam(&m_IniParam, "Rubriques Automatiques", QString::number(n_rubAuto++) , val.remove('*'));
      if (type[0]=='*')
         {if (G_pCApp->m_ListRubName.findIndex (rubName)==-1) G_pCApp->m_ListRubName.append(rubName);
          G_pCApp->m_mapNameRubType[rubName]  =  type.remove('*').toInt();
          G_pCApp->m_mapNameRubInfos[rubName] =  prefix.remove('*');
         }
      val = G_pCApp->m_PathGlossaire+"/"+rubName;
      QDir dir(val);
      if (! dir.exists (val)) dir.mkdir(val);
      //................ creer le droit ......................................

     }
  //................. deuxième tour pour enregistrer si le type actuel de la rubrique le permet .....................
  myChild = pQListViewItemParent->firstChild();
  while( myChild )
      {
              val      = myChild->text(1);
              int cas  = myChild->text(0).left(1).toInt();
              switch (cas)
                 {
                  //  [Observation]
                  //     Path documents = /home/ro/MedinTux-V2.13-Mac-Intel-105-src/Icones
                  //     Defaut = $Glossaire/Identification/A G G I R.html
                   case 1 :  if (type != "2" && type != "20060000")
                                 CGestIni::Param_WriteParam(&m_IniParam, rubName , "Path documents", val.remove("*")) ;
                             break;
                   case 2 :  if (type != "2")
                                 CGestIni::Param_WriteParam(&m_IniParam, rubName , "Defaut",         val.remove("*")) ;
                             break;
                  //     [MenuContextuel]
                  //       Observation =
                  //       Prescription =
                  //       Documents = Gestion des documents
                  //       Identification = ° HOSPITALISATION
                   case 3 :  if (type != "2" && type != "20060000")
                                 CGestIni::Param_WriteParam(&m_IniParam, "MenuContextuel", rubName , val) ;
                             break;
                   case 6 :  if (QFile::exists(val))
                                {QString tmp = rubName+".png"; tmp = tmp.prepend("type_");
                                 CHtmlTools::Copy_File(val, Theme::getPath() + "16x16", &tmp);
                                }
                             break;
                 } // end switch (cas)
         myChild = myChild->nextSibling();
      } // end while( myChild )
  pQListViewItemParent = pQListViewItemParent->nextSibling();
 } //end  while ( pQListViewItemParent )
 CGestIni::Param_UpdateToDisk(pathFileDroit , allDroitsStr);
}

//----------------------------------- setParams ----------------------------------------
/*! \brief Recupere les structures de parametres pour l'application et l'utilisateur. Puis prepare leur integration a l'interface et leur verification.
 */
void FormConfig::setParams(const char* ParamsOfAppli, const char* ParamsOfUser, const char* PathOfAppli)
{ m_IniParam  = ParamsOfAppli;
  m_IniUser   = ParamsOfUser;
  m_PathAppli = PathOfAppli;


    //.............. recuperer le theme choisi par l'utilisateur ...........................................
  CGestIni::Param_ReadParam(m_IniUser, THEME_CFG,       "Current Theme",   &m_Theme );  // récuperer le theme courant dans les parametres
  CGestIni::Param_ReadParam(m_IniUser, THEME_CFG,       "PopupFntSize",    &m_PopMenu_DefaultFontSize );

  //.............. Cree le ComboBox pour les themes .....................................................
  QDir dirTheme = QDir( m_PathAppli + "../../Themes");
  if ( dirTheme.exists() )
     { dirTheme.setFilter( QDir::Dirs | QDir::NoSymLinks );
       dirTheme.setSorting( QDir::Name );
       const QFileInfoList* pQFileInfoList = dirTheme.entryInfoList();
       if (pQFileInfoList==0) return ;
       QFileInfoListIterator it( *pQFileInfoList );
       QFileInfo*     fi;
	   // Parcours le repertoire
       while ( (fi = it.current()) != 0 )
             {  QString fname = fi->fileName().latin1();
                if (fname != ".." && fname != ".")
                   { m_Combo_Themes->insertItem( fname );
                   }
              ++it;
             } // fin while
       //.............. positionner le combo sur le theme utilisateur ..............
       if (m_Combo_Themes->count())
          {if (m_Theme.length()==0) m_Theme = m_Combo_Themes->currentText();
           QListBoxItem     *pQListBoxItem  = m_Combo_Themes->listBox()->findItem (m_Theme, Qt::ExactMatch );
           if (pQListBoxItem)
              {int id  =      m_Combo_Themes->listBox()->index ( pQListBoxItem );
               if (id != -1)  m_Combo_Themes->setCurrentItem ( id );
              }
          }
       }
  //................... Complete la liste de choix avec le texte et les icones.................
  listBox->clear();
  listBox->insertItem( Theme::getIcon("32x32/help.png") ,               TR("Analyse") );
  listBox->insertItem( Theme::getIcon("32x32/folder.png") ,             TR("Répertoires") );
  listBox->insertItem( Theme::getIcon("32x32/package_utilities.png") ,  TR("Général") );
  listBox->insertItem( Theme::getIcon("32x32/rapid_access.png") ,       TR("Accès Rapide") );
  listBox->insertItem( Theme::getIcon("32x32/rubriques.png") ,          TR("Rubriques") );
  listBox->insertItem( Theme::getIcon("32x32/displayconfig.png") ,      TR("Docks") );
  listBox->insertItem( Theme::getIcon("32x32/CodageCCAM.png") ,         TR("CCAM") );
  listBox->insertItem( Theme::getIcon("32x32/acroread.png") ,           TR("PDF") );

  // Initialise les valeurs avec le drtux.ini
  QString val;
  CGestIni::Param_ReadParam(m_IniParam, "Connexion", "SplashScreen", &m_SplashScreen );

  // Initialise la partie Repertoires , menus et defauts
  val=""; CGestIni::Param_ReadParam(m_IniParam, "Glossaire", "Path", &val );                            repGlossaire->setText(val);
  val=""; CGestIni::Param_ReadParam(m_IniParam, "Repertoire Temporaire", "Repertoire", &val );          repTmpGeneral->setText(val);
  val=""; CGestIni::Param_ReadParam(m_IniParam, "Aide", "Path", &val );                                 Aide->setText(val);

  // ??..?? Initialise les dialogues pour le Form_PDF de CDevilFormPDF
  val=""; CGestIni::Param_ReadParam(m_IniParam, FORM_PDF, "Chemin vers PDF", &val );                    repDefautPDF->setText(val);
  val=""; CGestIni::Param_ReadParam(m_IniParam, FORM_PDF, "Application visualisateur de PDF", &val );   appliPDF->setText(val);
  val=""; CGestIni::Param_ReadParam(m_IniParam, FORM_PDF, "Chemin vers pdftk", &val );                  repPdftk->setText(val);
  val=""; CGestIni::Param_ReadParam(m_IniParam, FORM_PDF, "Chemin vers tmp", &val );                    repPDFTmp->setText(val);

  // Listes d'observation et de prescription
  val=""; CGestIni::Param_ReadParam(m_IniParam, CONTEXT_MENU, "ListImages", &val );                     menuImage->setText(val);

  // Documents par defaut
  val=""; CGestIni::Param_ReadParam(m_IniParam, "Vigie",          "Defaut", &val );                        defautVigie->setText(val);

  // Gestion des contacts et utilisateurs
  val=""; CGestIni::Param_ReadParam(m_IniParam, "Gestion des contacts", "PathExe", &val );              GestContact->setText(val);
  val=""; CGestIni::Param_ReadParam(m_IniParam, "Gestion des utilisateurs", "PathExe", &val );          GestUser->setText(val);


  // Parametres pour la CCAM
  val=""; CGestIni::Param_ReadParam(m_IniParam, "Codage CCAM", "PathCCAM_Ini", &val );                  CCAMIni->setText(val);
  val=""; CGestIni::Param_ReadParam(m_IniParam, "Codage CCAM", "PathExe", &val );                       CCAMExe->setText(val);

  CGestIni::Param_ReadParam(m_IniParam, "Codage CCAM", "LibellePrefix",     &m_CCAMLibPref );
  CGestIni::Param_ReadParam(m_IniParam, "Codage CCAM", "LibellePostfix",    &m_CCAMLibPostf );
  CGestIni::Param_ReadParam(m_IniParam, "Codage CCAM", "CodePrefix",        &m_CCAMCodePref );
  CGestIni::Param_ReadParam(m_IniParam, "Codage CCAM", "CodePostfix",       &m_CCAMCodePostf );
  CGestIni::Param_ReadParam(m_IniParam, "Codage CCAM", "Panneau a activer", &m_CCAMPanneau );
  // met a jour l'exemple pour le visuel
  CCAMViewUpdate();


  // Gestion des dossiers
  CGestIni::Param_ReadParam(m_IniParam, "Gestion des dossiers", "Nouveau dossier", &m_NouveauDossier );
  CGestIni::Param_ReadParam(m_IniParam, "Gestion des dossiers", "Acces Concurrent", &m_DossierConcurrent );
  CGestIni::Param_ReadParam(m_IniParam, "Observation", "Verifier dossier", &m_VerifDoss );

  // Gestion des documents
  CGestIni::Param_ReadParam(m_IniParam, "Documents", "Path documents", &m_RepDocuments );
  CGestIni::Param_ReadParam(m_IniParam, "Documents", "MaxImageWidth",  &m_MaxIamgeWidth );

  // Gestion de la barre d'onglets
  CGestIni::Param_ReadParam(m_IniUser, TAB_RUBRIQUES, "Display", &m_TabRubriques );

  // Gestion des docks
  CGestIni::Param_ReadParam(m_IniUser, DOCK_RUBRIQUES, "Montrer", &m_Dock_TabRubrique );
  CGestIni::Param_ReadParam(m_IniUser, DOCK_GLOSSAIRE, "Montrer", &m_Dock_Glossaire );

  CGestIni::Param_ReadParam(m_IniUser, DOCK_RAPID_MENU, "Montrer",     &m_DockMenu_OnScreen );
  CGestIni::Param_ReadParam(m_IniUser, DOCK_RAPID_MENU, "ShowTerrain", &m_DockMenu_ShowTerrain );
  CGestIni::Param_ReadParam(m_IniUser, DOCK_RAPID_MENU, "ShowObs",     &m_DockMenu_ShowObs );
  CGestIni::Param_ReadParam(m_IniUser, DOCK_RAPID_MENU, "ShowPrescr",  &m_DockMenu_ShowPrescr );
  CGestIni::Param_ReadParam(m_IniUser, DOCK_RAPID_MENU, "ShowDoc",     &m_DockMenu_ShowDoc );
  CGestIni::Param_ReadParam(m_IniUser, DOCK_RAPID_MENU, "ShowInterv",  &m_DockMenu_ShowInterv );
  CGestIni::Param_ReadParam(m_IniUser, DOCK_RAPID_MENU, "ShowIdent",   &m_DockMenu_ShowIdent );

  CGestIni::Param_ReadParam(m_IniUser, DOCK_RAPID_MENU, "OpenIdent",    &m_DockMenu_OpenIdent );
  CGestIni::Param_ReadParam(m_IniUser, DOCK_RAPID_MENU, "OpenATCD",     &m_DockMenu_OpenATCD );
  CGestIni::Param_ReadParam(m_IniUser, DOCK_RAPID_MENU, "OpenVars",     &m_DockMenu_OpenVars );
  CGestIni::Param_ReadParam(m_IniUser, DOCK_RAPID_MENU, "OpenObs",      &m_DockMenu_OpenObs );
  CGestIni::Param_ReadParam(m_IniUser, DOCK_RAPID_MENU, "OpenPrescr",   &m_DockMenu_OpenPrescr );
  CGestIni::Param_ReadParam(m_IniUser, DOCK_RAPID_MENU, "OpenDoc",      &m_DockMenu_OpenDoc );
  CGestIni::Param_ReadParam(m_IniUser, DOCK_RAPID_MENU, "OpenInterv",   &m_DockMenu_OpenInterv );
  CGestIni::Param_ReadParam(m_IniUser, DOCK_RAPID_MENU, "OpenTerrain",  &m_DockMenu_OpenTerrain );
  CGestIni::Param_ReadParam(m_IniUser, DOCK_RAPID_MENU, "OpenVisuNote", &m_DockMenu_OpenVisuNote );

  CGestIni::Param_ReadParam(m_IniUser, DOCK_RAPID_MENU, "Glossaire",       &m_DockMenu_Glossaire );
  CGestIni::Param_ReadParam(m_IniUser, DOCK_RAPID_MENU, "ClickOnParent",   &m_DockMenu_ClickBehavior );
  CGestIni::Param_ReadParam(m_IniUser, DOCK_RAPID_MENU, "DefaultFontSize", &m_DockMenu_DefaultFontSize );

  init_listViewRubParameters();

  checkIni();
  checkParam();

  // Si des valeurs ont ete mise a leur defaut et qu'elles ne sont pas source d'erreur lors de la validation
  // alors on considere que nous n'avons trouve aucune erreur. Cela evite a l'utilisateur de chercher dans les
  // documentations, de nous harponner....
  if (!m_CheckErreur) m_erreurDrTuxIni = FALSE;
}

//----------------------------------- setParams ----------------------------------------
/*! \brief Retour dans les variables passees en parametre les structures de parametres de l'application et de l'utilisateur.
 */
void FormConfig::getParams(QString& ParamsOfAppli, QString& ParamsOfUser)
{ ParamsOfAppli = m_IniParam;
  ParamsOfUser  = m_IniUser;
}



//----------------------------------- checkError ----------------------------------------
/*! \fn bool FormConfig::checkError()
 *  \brief Renvoie le statut des variables d'erreur. Si TRUE alors une erreur est intervenue dans la recuperation des donnees dans le fichier ini et/ou dans la verification des donnees.
 *  \return TRUE en cas d'erreur, FALSE si aucune erreur.
 */
bool FormConfig::checkError()
{ return (m_CheckErreur || m_erreurDrTuxIni);  }


//----------------------------------- checkIni ----------------------------------------
/*! \brief Recupère les parametres du fichier ini. Notifie les erreurs eventuellement rencontrees (champs absents ou vides) dans la zone de texte de la premiere page des parametres.
 *  \return rien.
 *  \sa msgErreurIni() , erreurIni ()
*/
bool FormConfig::checkIni()
{ QString msg;

  //........ Initialise les variables privates avant de lancer les tests
  m_erreurDrTuxIni = FALSE;	// de base aucune erreur
  m_varErreurDrTuxIni = "";	// et donc pas de message !!!

  //........ Initialise le textEdit de la page de garde qui donnera les infos de debuggage
  msgConfig->clear();
  msgConfig->setText( tr("<p align=\"center\"><span style=\"font-size:10pt;font-weight:600\">Outil de configuration de Dr Tux</span></p>") );
  msgConfig->append("<p>&nbsp;</p>");
  msgConfig->append("<p>&nbsp;</p>");
  msgConfig->append("<p><span style=\"font-size:10pt\">"+TR("Dr Tux a été exécuté depuis")+"</span></p>");
  msgConfig->append("<p align=\"right\"><span style=\"font-size:10pt\">"+m_PathAppli+"</span></p>");
  msgConfig->append(tr("<p align=\"left\"><span style=\"font-size:10pt\">Vérification du fichier de sauvegarde des paramètres</span></p>") );
  msgConfig->append(tr("<p align=\"left\"><span style=\"font-size:10pt\">Récupération des données</span></p>") );

  // Gestion des checkBoxes
  readVarString( m_SplashScreen,         DEFAUT_SPLASH,              *boxSplashScreen,                  tr("oui"),      tr("non"));
  readVarString( m_NouveauDossier,       DEFAUT_NOUVEAUDOSSIER,      *boxNouveauDossier,                tr("oui"),      tr("non"));
  readVarString( m_DossierConcurrent,    DEFAUT_CONCURRENT,          *boxConcurrent,                    tr("possible"), tr("non"));
  readVarString( m_VerifDoss,            DEFAUT_VERIFDOSS,           *boxVerificationDossier,           tr("oui"),      tr("non"));

  readVarString( m_TabRubriques,         DEFAUT_TAB_RUBRIQUES,       *boxDockBarreOnglets,              tr("true"),     tr("false"));
  readVarString( m_Dock_Glossaire,       DEFAUT_DOCK_GLOSSAIRE,      *boxDockGlossaire,                 tr("Normal"),   tr("Cacher"));
  readVarString( m_Dock_TabRubrique,     DEFAUT_DOCK_GLOSSAIRE ,     *boxDockRubriques,                 tr("Normal"),   tr("Cacher"));
  readVarString( m_DockMenu_OnScreen,    DEFAUT_DOCK_RAPIDE,         *boxDockRapidAccess,               tr("Normal"),   tr("Cacher"));
  readVarString( m_DockMenu_ShowIdent,   DEFAUT_DOCK_RAPIDE_TRUE,    *boxDockRapidAccess_Ident,         tr("true"),     tr("false"));
  readVarString( m_DockMenu_ShowObs,     DEFAUT_DOCK_RAPIDE_TRUE ,   *boxDockRapidAccess_Obs,           tr("true"),     tr("false"));
  readVarString( m_DockMenu_ShowPrescr,  DEFAUT_DOCK_RAPIDE_TRUE,    *boxDockRapidAccess_Prescr,        tr("true"),     tr("false"));
  readVarString( m_DockMenu_ShowDoc,     DEFAUT_DOCK_RAPIDE_TRUE,    *boxDockRapidAccess_Doc,           tr("true"),     tr("false"));
  readVarString( m_DockMenu_ShowInterv,  DEFAUT_DOCK_RAPIDE_TRUE,    *boxDockRapidAccessIntervenants,   tr("true"),     tr("false"));
  readVarString( m_DockMenu_ShowTerrain, DEFAUT_DOCK_RAPIDE_TRUE ,   *boxDockRapidAccess_Terrain,       tr("true"),     tr("false"));
  readVarString( m_DockMenu_Glossaire,   "on" ,                      *boxDockRapidAccess_Glossaire,     tr("on"),       tr("off"));

  readVarString( m_DockMenu_OpenIdent,   DEFAUT_DOCK_RAPIDE_TRUE,    *boxDockRapidAccess_OpenIdent,        tr("true"), tr("false"));
  readVarString( m_DockMenu_OpenATCD,    DEFAUT_DOCK_RAPIDE_TRUE,    *boxDockRapidAccess_OpenATCD,         tr("true"), tr("false"));
  readVarString( m_DockMenu_OpenVars,    DEFAUT_DOCK_RAPIDE_TRUE,    *boxDockRapidAccess_OpenVars,         tr("true"), tr("false"));
  readVarString( m_DockMenu_OpenObs,     DEFAUT_DOCK_RAPIDE_TRUE ,   *boxDockRapidAccess_OpenObs,          tr("true"), tr("false"));
  readVarString( m_DockMenu_OpenPrescr,  DEFAUT_DOCK_RAPIDE_TRUE,    *boxDockRapidAccess_OpenPrescr,       tr("true"), tr("false"));
  readVarString( m_DockMenu_OpenDoc,     DEFAUT_DOCK_RAPIDE_TRUE,    *boxDockRapidAccess_OpenDoc,          tr("true"), tr("false"));
  readVarString( m_DockMenu_OpenInterv,  DEFAUT_DOCK_RAPIDE_TRUE,    *boxDockRapidAccess_OpenIntervenants, tr("true"), tr("false"));
  readVarString( m_DockMenu_OpenTerrain, DEFAUT_DOCK_RAPIDE_TRUE ,   *boxDockRapidAccess_OpenTerrain,      tr("true"), tr("false"));
  readVarString( m_DockMenu_OpenVisuNote,DEFAUT_DOCK_RAPIDE_TRUE ,   *boxDockRapidAccess_OpenVisuNote,     tr("true"), tr("false"));

  readVarString( m_DockMenu_DefaultFontSize, DEFAUT_DOCK_FNTSIZE,        *textLabel_DockFntSize,            spinBoxDockRapidAccess_FontSize);
  readVarString( m_PopMenu_DefaultFontSize,  DEFAUT_MENU_FNTSIZE,        *textLabel_DefMenuFontSize,        spinBox_menus_FontSize);

  if (m_DockMenu_ClickBehavior == "openCMDI")
  { radioOpenCMDI->setChecked(TRUE);
  }
  else if (m_DockMenu_ClickBehavior == "openCloseList")
  { radioOpenParent->setChecked(TRUE);
  }
  else // erreur
  { m_erreurDrTuxIni = TRUE;
    m_DockMenu_ClickBehavior = "openCMDI";
    m_varErreurDrTuxIni += QString(TR("Paramètre [%1] initialisé à sa valeur par défaut.")).arg("ClickOnParent du menu d'accès rapide")+"<br />";
    radioOpenCMDI->setChecked(TRUE);
  }

  readVarString( DEFAUT_REPGLOSSAIRE,   *lblRepGlossaire,   *repGlossaire);
  readVarString( DEFAUT_REPTMPGENE,     *lblRepTmpGeneral,  *repTmpGeneral);
  readVarString( DEFAUT_CHEMINAIDE,     *lblAide,           *Aide);

  readVarString( DEFAUT_REPPDF,         *lblRepPDF,         *repDefautPDF);
  readVarString( DEFAUT_REPPDFTMP,      *lblAppliPDF,       *appliPDF);
  readVarString( DEFAUT_PDFTK,          *lblRepPdftk,       *repPdftk);
  readVarString( DEFAUT_PDFVISU,        *lblRepPDFTmp,      *repPDFTmp);
  //.................. listes de menus permanentes ........................
  readVarString( DEFAUT_MENUPRESCR,     *lblMenuImages,     *menuImage);
  //.................. documents par defaut à installer lorsque creation patient ou vides ................
  readVarString( DEFAUT_MENUVIGIE,      *lblDefautVigie,             *defautVigie);

  readVarString( DEFAUT_CCAMINI,        *lblCCAMIni,        *CCAMIni);
  readVarString( DEFAUT_CCAMEXE,        *lblCCAMExe,        *CCAMExe);

  readVarString( DEFAUT_CONTACT,        *lblGestContact,    *GestContact);
  readVarString( DEFAUT_GESTUSER,       *lblGestUser,       *GestUser);

  readVarString(m_CCAMLibPref,        DEFAUT_CCAMLBLPRE,     *lblCCAMPrefixe, *CCAMPrefixe);
  readVarString(m_CCAMLibPostf,       DEFAUT_CCAMLBLPOST,    *lblCCAMPostfixe, *CCAMPostfixe);
  readVarString(m_CCAMCodePref,       DEFAUT_CCAMCODEPRE,    *lblCCAMPrefixe_2, *CCAMCodePrefixe);
  readVarString(m_CCAMCodePostf,      DEFAUT_CCAMCODEPOST,   *lblCCAMPostfixe_2, *CCAMCodePostfixe);
  readVarString(m_CCAMPanneau,        DEFAUT_CCAMPANNEAU,    *lblCCAMPanneau, *CCAMPanneau);


  // Affiche le message d'erreur potentiel dans la page de garde de la fenêtre
  if (m_erreurDrTuxIni)
   {  msgConfig->append(tr("<p style=\"margin-left:30px;\"><span style=\"font-size:10pt;font-weight:600;color:#ff0000\">Erreur(s) d&eacute;tect&eacute;e(s) : <br /></span></p>"));
      msgConfig->append("<p style=\"margin-left:70px;\"><span style=\"font-size:10pt;color:#ff0000\">"+m_varErreurDrTuxIni+ "</span></p>");
   }
  else
   {  msgConfig->append(tr("<p align=\"right\"><span style=\"font-size:10pt;color:#5160bd\">Récupération des données terminée sans erreur</span></p>") );
   }
  msgConfig->append("<p style=\"margin-left:0px;\">&nbsp;</p>");  // indispensable pour ne pas indenter tout le texte qui suivra...
  return TRUE;
}


//----------------------------- readVarString --------------------------------
/*! \brief Verifie si param est defini sinon le label de la variable est mis en rouge avec le message d'erreur en fin de string,et les variables : m_erreurDrTuxIni recoit TRUE (erreur !) m_varErreurDrTuxIni recoit le message d'erreur.
 *  \param def      : la valeur par defaut de la variable qui sera introduite dans la fenetre si necessaire
 *  \param label    : le label de la variable dans la fenetre
 *  \param lineedit : le line edit (ou le combobox) de la variable dans la fenetre
 *  \return param, label et lineedit
 */
void FormConfig::readVarString( QString def, QLabel &label, QLineEdit &lineedit)
{  QString msg;
   QColor red (255, 0, 0);
   QColor black (0, 0, 0);

   if (lineedit.text() == "")
      {    lineedit.setText(def);
	   m_erreurDrTuxIni     = TRUE;
	   m_varErreurDrTuxIni += TR("-a- Parametre [%1] initialisé à sa valeur par défaut : ").arg(label.text()) + "<br />";
	   msg                  = label.text();
	   if (msg.at(msg.length()-1) != m_erreur.at(m_erreur.length()-1) )  // le lbl est a modifier
		  { msg += m_erreur;
		  }
	   label.setText( msg );
	   label.setPaletteForegroundColor(red);
      }
  else
      {label.setPaletteForegroundColor(black);
	  }
}
//----------------------------- readVarString --------------------------------
/*! \brief Verifie si param est défini sinon le label de la variable est mis en rouge avec le message d'erreur en fin de string,et les variables : m_erreurDrTuxIni reoit TRUE (erreur !) m_varErreurDrTuxIni reoit le message d'erreur.
 *  \param param : nom de la variable à tester
 *  \param def : la valeur par défaut de la variable qui sera introduite dans la fentre si nécessaire
 *  \param pW1 : le label de la variable dans la fentre
 *  \param pW2 : le line edit (ou le combobox) de la variable dans la fentre
 *  \return param, pW1 et pW2
 */
void FormConfig::readVarString(QString& param, QString def, QLabel &label, QSpinBox *spinbox)
{  QString msg;
   QColor red (255, 0, 0);
   QColor black (0, 0, 0);
   if (param.length()) spinbox->setValue(param.toInt());
   if (spinbox->value() <= 0)
      {    spinbox->setValue(def.toInt());
           param                = def;
	   m_erreurDrTuxIni     = TRUE;
	   m_varErreurDrTuxIni += TR("-e- Parametre [%1] initialisé à sa valeur par défaut : %2").arg(label.text(),def) + "<br />";
	   msg                  = label.text();
	   if (msg.at(msg.length()-1) != m_erreur.at(m_erreur.length()-1) )  // le lbl est a modifier
		  { msg += m_erreur;
		  }
	   label.setText( msg );
	   label.setPaletteForegroundColor(red);
      }
  else
      {label.setPaletteForegroundColor(black);
      }
}

//----------------------------- readVarString --------------------------------
/*! \brief Verifie si param est défini sinon le label de la variable est mis en rouge avec le message d'erreur en fin de string,et les variables : m_erreurDrTuxIni reoit TRUE (erreur !) m_varErreurDrTuxIni reoit le message d'erreur.
 *  \param param : nom de la variable à tester
 *  \param def : la valeur par défaut de la variable qui sera introduite dans la fentre si nécessaire
 *  \param pW1 : le label de la variable dans la fentre
 *  \param pW2 : le line edit (ou le combobox) de la variable dans la fentre
 *  \return param, pW1 et pW2
 */
void FormConfig::readVarString(QString& param, QString def, QLabel &pW1, QLineEdit &pW2)
{ QString msg;
  QColor red (255, 0, 0);
  QColor black (0, 0, 0);
  if (param == "")
   { 	param = def;
	m_erreurDrTuxIni = TRUE;
	m_varErreurDrTuxIni += TR("-b- Paramètre [%1] initialisé à sa valeur par défaut : ").arg(pW1.text()) + param + "<br />";
	msg = pW1.text();
	if (msg.at(msg.length()-1) != m_erreur.at(m_erreur.length()-1) )  // le lbl est à modifier
		{ msg += m_erreur; }
	pW1.setText( msg );
	pW1.setPaletteForegroundColor(red);
   }
  else {  pW1.setPaletteForegroundColor(black);  }
  pW2.setText(param); param = "";
}

//----------------------------- readVarString --------------------------------
void FormConfig::readVarString(QString& param, QString def, QLabel &pW1, QComboBox &pW2)
{ QString msg;
  QColor red (255, 0, 0);
  QColor black (0, 0, 0);
  if (param == "")
   { 	param = def;
	m_erreurDrTuxIni = TRUE;
	m_varErreurDrTuxIni += TR("-c- Paramètre [%1] initialisé à sa valeur par défaut : ").arg(pW1.text()) + param + "<br />";
	msg = pW1.text();
	if (msg.at(msg.length()-1) != m_erreur.at(m_erreur.length()-1) )  // le lbl est a modifier
		{ msg += m_erreur; }
	pW1.setText( msg );
	pW1.setPaletteForegroundColor(red);
   }
  else {  pW1.setPaletteForegroundColor(black);  }
  pW2.setCurrentText(param); param = "";
}

//----------------------------- readVarString --------------------------------
void FormConfig::readVarString(QString& param, QString defaut, QCheckBox &pW1,
                               const char* ValPourOui, const char* ValPourNon)
{ //.......... Verifie les donnees des formulaires une a une
  if (param == "")
   { 	m_SplashScreen = defaut;
	pW1.setPaletteForegroundColor(red);
	m_erreurDrTuxIni = TRUE;
	m_varErreurDrTuxIni += TR("-d- Paramètre [%1] initialisé à sa valeur par défaut : ").arg(pW1.text()) + param + "<br />";
   }
  if (param == ValPourNon) 	pW1.setChecked (FALSE);
  else if (param == ValPourOui)	pW1.setChecked (TRUE);

}
// ------------------------- specialBoxes ----------------------------
/*! \brief Recupere dans les variables privates les valeur indiquees dans le formulaire.
 *  \return Tout est stocke dans les variables privates de la classe
 * Il faut appeler cette fonction avant de sauvegarder dans le fichier ini pour recuperer toutes les variables.
 * Ne devrait pas être appelee par d'autres classes...
*/
void FormConfig::specialBoxes()
{ // SplashScreen
   if (boxSplashScreen->isChecked()) m_SplashScreen = "oui";
   else                              m_SplashScreen = "non";

   if (boxNouveauDossier->isChecked())        m_NouveauDossier = "oui";         else  m_NouveauDossier ="non";
   if (boxConcurrent->isChecked())            m_DossierConcurrent = "possible"; else  m_DossierConcurrent ="non";
   if (boxVerificationDossier->isChecked())   m_VerifDoss = "oui";              else  m_VerifDoss ="non";

   if (boxDockGlossaire->isChecked())    m_Dock_Glossaire    = "Normal"; else  m_Dock_Glossaire    ="Cacher";
   if (boxDockRubriques->isChecked())    m_Dock_TabRubrique  = "Normal"; else  m_Dock_TabRubrique  ="Cacher";
   if (boxDockRapidAccess->isChecked())  m_DockMenu_OnScreen = "Normal"; else  m_DockMenu_OnScreen ="Cacher";
   if (boxDockBarreOnglets->isChecked()) m_TabRubriques      = "true";   else  m_TabRubriques      ="false";

   if (boxDockRapidAccess_Ident->isChecked())         m_DockMenu_ShowIdent = "true";
                                            else      m_DockMenu_ShowIdent ="false";
   if (boxDockRapidAccess_Terrain->isChecked())       m_DockMenu_ShowTerrain = "true";
                                            else      m_DockMenu_ShowTerrain ="false";
   if (boxDockRapidAccess_Obs->isChecked())           m_DockMenu_ShowObs = "true";
                                            else      m_DockMenu_ShowObs ="false";
   if (boxDockRapidAccess_Prescr->isChecked())        m_DockMenu_ShowPrescr = "true";
                                            else      m_DockMenu_ShowPrescr ="false";
   if (boxDockRapidAccess_Doc->isChecked())           m_DockMenu_ShowDoc = "true";
                                            else      m_DockMenu_ShowDoc ="false";
   if (boxDockRapidAccessIntervenants->isChecked())   m_DockMenu_ShowInterv = "true";
                                            else      m_DockMenu_ShowInterv ="false";
   if (boxDockRapidAccess_Glossaire->isChecked())     m_DockMenu_Glossaire = "on";
                                            else      m_DockMenu_Glossaire ="off";

   if (boxDockRapidAccess_OpenIdent->isChecked())     m_DockMenu_OpenIdent = "true";
                                                else  m_DockMenu_OpenIdent ="false";
   if (boxDockRapidAccess_OpenATCD->isChecked())      m_DockMenu_OpenATCD = "true";
                                                else  m_DockMenu_OpenATCD ="false";
   if (boxDockRapidAccess_OpenVars->isChecked())      m_DockMenu_OpenVars = "true";
                                                else  m_DockMenu_OpenVars ="false";
   if (boxDockRapidAccess_OpenTerrain->isChecked())   m_DockMenu_OpenTerrain = "true";
                                                else  m_DockMenu_OpenTerrain = "false";
   if (boxDockRapidAccess_OpenVisuNote->isChecked())  m_DockMenu_OpenVisuNote = "true";
                                                else  m_DockMenu_OpenVisuNote ="false";
   if (boxDockRapidAccess_OpenObs->isChecked())       m_DockMenu_OpenObs = "true";
                                                else  m_DockMenu_OpenObs ="false";
   if (boxDockRapidAccess_OpenPrescr->isChecked())    m_DockMenu_OpenPrescr = "true";
                                                else  m_DockMenu_OpenPrescr ="false";
   if (boxDockRapidAccess_OpenDoc->isChecked())       m_DockMenu_OpenDoc = "true";
                                                else  m_DockMenu_OpenDoc ="false";
   if (boxDockRapidAccess_OpenIntervenants->isChecked())   m_DockMenu_OpenInterv = "true";
                                                else       m_DockMenu_OpenInterv ="false";

   m_DockMenu_DefaultFontSize  =   spinBoxDockRapidAccess_FontSize->text();
   m_PopMenu_DefaultFontSize   =   spinBox_menus_FontSize->text();


  // ComboBoxes
  m_CCAMPanneau = CCAMPanneau->currentText();
  m_Theme       = m_Combo_Themes->currentText();
}


// ------------------------- buttonHelp_clicked ----------------------------
/*! \brief Verifie que les parametres inseres dans le formulaire sont valides.
*/
void FormConfig::buttonHelp_clicked()
{ m_CheckErreur = FALSE;	// TRUE si une erreur est a signaler lors de la verification des parametres
  m_CheckMsgErreur = "";	// Message qui suit la variable precedente...
  checkParam();
}


//-------------------------  CCAMViewUpdate  -------------------------------
/*! \brief Si modifications dans CCAM, mettre a jour l'exemple
*/
void FormConfig::CCAMViewUpdate()
{ textCCAMView->clear();
  QString str = "";
  str  = CCAMPrefixe->text() + tr("Label de l'acte CCAM") + CCAMPostfixe->text();
  str += CCAMCodePrefixe->text() + tr("Code de l'acte CCAM") + CCAMCodePostfixe->text();
  textCCAMView->setText(str);
}
void FormConfig::CCAMPrefixe_textChanged( const QString & )             {  CCAMViewUpdate();  }
void FormConfig::CCAMPostfixe_textChanged( const QString & )		{  CCAMViewUpdate();  }
void FormConfig::CCAMCodePrefixe_textChanged( const QString & )		{  CCAMViewUpdate();  }
void FormConfig::CCAMCodePostfixe_textChanged( const QString & )	{  CCAMViewUpdate();  }

//----------------------------------- checkFile   ----------------------------------------
/*! \fn QString FormConfig::checkFile(QString path, QLabel &lbl, bool exe, bool w)
 *  \brief Verifie la validite d'un chemin vers un fichier.
 *  \return Cette fonction verifie que le fichier : existe, qu'il est accessible Ã  la lecture, Ã  l'ecriture (selon les parametres) et est executable (selon les parametres). Cette fonction fait parti des fonctions de verification (checkParam()). \n
 *  Le resultat de ce test est bascule dans les variables privates que l'on peut recuperer par erreurCheck() et msgErreurCheck().\n
 *  La fenêtre ne s'affiche pas.
 * \param &path	:	path Ã  verifier (relatif ou absolu). Si ce chemin est relatif c'est le chemin vers DRTUX qui est ajoute !
 * \param lbl	:	le label correspondant dans la fenêtre de configuration Ã  modifier en cas d'erreur
 * \param exe	:	le fichier Ã  tester est un executable si TRUE
 * \param w	:	le fichier Ã  tester doit être accessible en ecriture si TRUE
 */
int FormConfig::checkFile(QString path, QLabel &lbl, int exe, bool w)
{QString tmp                = path;
  QString tmpMessAcceFichier = TR("Vérifiez vos droits d'accès à ce fichier.");

  // Si debute par "$Glossaire" remplacer par chemin vers glossaire
  tmp.replace("$Glossaire",repGlossaire->text());

  // Si chemin relatif --> calculer le chemin absolu
  if (QDir(tmp).isRelative()) tmp.prepend(m_PathAppli);
  tmp = QDir::cleanDirPath(tmp);
  if (exe==2) tmp += EXTENSION_EXE;
  //QString fileName = QFile::decodeName ((QCString)tmp);
 //  if (CGestIni::IsUtf8( tmp ) )  tmp = QString::fromUtf8(tmp);
  int ret = 1;
  QFileInfo f (tmp);
  // Test l'existence du fichier
  if (!f.exists())
   {	m_CheckErreur = TRUE;
	QString m = QString(TR(" <b>Paramètre : %1 :</b> <u> %2 </u> n'existe pas.")).arg(lbl.text(), tmp) + "<br />";
	m_CheckMsgErreur.append(m);
	m = lbl.text();
	if (m.at(m.length()-1) != m_erreur.at(m_erreur.length()-1))  // le lbl est Ã  modifier
		{ m += m_erreur;    lbl.setText( m ); }
	lbl.setPaletteForegroundColor(red);
	return 0;
   }
  // Test le fichier en lecture
  if (!f.isReadable())
   {	m_CheckErreur = TRUE;
	QString m = QString(TR(" <b>Paramètre :</b> %1 : <u> %2 </u> n'est pas accessible en lecture.")).arg(lbl.text(), path) + "<br />" + tmpMessAcceFichier + "<br />";
	m_CheckMsgErreur.append(m);
	m = lbl.text();
	if (m.at(m.length()-1) != m_erreur.at(m_erreur.length()-1))  // le lbl est Ã  modifier
		{ m += m_erreur;    lbl.setText( m ); }
	ret = 0;
   }
   if (exe)
   if (!f.isExecutable())
   {	m_CheckErreur = TRUE;
	QString m = QString(TR(" <b>Paramètre :</b> %1 : <u> %2 </u> n'est pas exécutable.")).arg(lbl.text(), path) + "<br />" + tmpMessAcceFichier + "<br />";
	m_CheckMsgErreur.append(m);
	m = lbl.text();
	if (m.at(m.length()-1) != m_erreur.at(m_erreur.length()-1))  // le lbl est Ã  modifier
		{ m += m_erreur;    lbl.setText( m ); }
	ret = 0;
   }
   if (w)
   if (!f.isWritable())
   {	m_CheckErreur = TRUE;
	QString m = QString(TR(" <b>Paramètre :</b> %1 : <u> %2 </u> n'est pas accessible en écriture.")).arg(lbl.text(), path) + "<br />" + tmpMessAcceFichier + "<br />";
	m_CheckMsgErreur.append(m);
	m = lbl.text();
	if (m.at(m.length()-1) != m_erreur.at(m_erreur.length()-1))  // le lbl est Ã  modifier
		{ m += m_erreur;    lbl.setText( m ); }
	ret = 0;
   }
  if (ret==0) lbl.setPaletteForegroundColor(red);
  else            lbl.setPaletteForegroundColor(black);
  return ret;
}


//----------------------------------- checkDir   ----------------------------------------
/*! \fn void FormConfig::checkDir(QString path, QLabel &lbl)
 *  \brief Verifie la validite d'un repertoire.
 *  \return int Cette fonction verifie que le repertoire : existe et qu'il est accessible Ã  la lecture. Cette fonction fait parti des fonctions de verification (checkParam()).\n
 *  Le resultat de ce test est bascule dans les variables privates que l'on peut recuperer par erreurCheck() et msgErreurCheck().\n
 *  La fenêtre ne s'affiche pas.
 * \param &path	:	path a  verifier (relatif ou absolu). Si ce chemin est relatif c'est le chemin vers DRTUX qui est ajoute !
 * \param lbl	:	le label correspondant dans la fenêtre de configuration Ã  modifier en cas d'erreur
 */
int FormConfig::checkDir(QString path, QLabel &lbl)
{ QString  tmp = path;
  int      ret = 3;
  bool   isErr = FALSE;
  // Si repertoire relatif --> calculer le repertoire absolu
  if (QDir(tmp).isRelative()) tmp.prepend(m_PathAppli);
  tmp = QDir::cleanDirPath(tmp);
  QDir dirTmp(tmp);
  // Verifie que le repertoire absolu existe
  if (!dirTmp.exists())
   {	m_CheckErreur = TRUE;
	QString m = QString(TR(" <b>Le paramètre : %1 :</b> <u> %2 </u> n'existe pas.")).arg(lbl.text() , path) + "<br />";
	m_CheckMsgErreur.append(m);
	m = lbl.text();
	if (m.at(m.length()-1) != m_erreur.at(m_erreur.length()-1))  // le lbl est Ã  modifier
		{ m += m_erreur;    lbl.setText( m ); }
	isErr = TRUE;
	ret -= 1;
   }
  else lbl.setPaletteForegroundColor(black);
  // Verifie les droits d'acces au repertoire
  if (!dirTmp.isReadable())
   {	m_CheckErreur = TRUE;
	QString m = QString(TR(" <b>Le paramètre : %1 :</b> <u> %2 </u> n'est pas accessible en lecture.")).arg(lbl.text() , path) + "<br />" + TR("Vérifiez les droits d'accès au répertoire.") + "<br />";
	m_CheckMsgErreur.append(m);
	m = lbl.text();
	if (m.at(m.length()-1) != m_erreur.at(m_erreur.length()-1))  // le lbl est Ã  modifier
		{ m += m_erreur;    lbl.setText( m ); }
	isErr = TRUE;
	ret -= 2;
   }
  if (isErr) lbl.setPaletteForegroundColor(red);
  else       lbl.setPaletteForegroundColor(black);
  return ret;
}



//----------------------------------- checkParam ----------------------------------------
/*! \brief Verifie l'exactitude des parametres indiques dans la fenêtre sans verifier le fichier *.ini.
 * Verifie les repertoires (existent ? accessiblent ? droits ?...) et les fichiers (idem, executable ?). En cas d'erreur, les variables m_CheckErreur et m_CheckMsgErreur sont renseignees.
 *  \sa    erreurCheck () , msgErreurCheck () , checkInit ()
*/
void FormConfig::checkParam()
{ //.......... Quelques variables tampon
  QString 	tmp;

  msgConfig->append(tr("<p><span style=\"font-size:10pt\">Vérification des données du formulaire</span></p>") );

  //.......... Verifie les repertoires
  checkDir( repGlossaire->text(),  *lblRepGlossaire);
  checkDir( repTmpGeneral->text(), *lblRepTmpGeneral);
  checkDir( repDefautPDF->text(),  *lblRepPDF);
  checkDir( repPDFTmp->text(),     *lblRepPDFTmp);

  //.......... Verifie l'existence des applis et fichiers ini
  // checkFile(QString path, QLabel &lbl,              bool executable, bool writable)
  checkFile( defautVigie->text(),             *lblDefautVigie,            0,FALSE);
  checkFile( GestContact->text(),             *lblGestContact,            2,FALSE);
  checkFile( CCAMExe->text(),                 *lblCCAMExe,                2,FALSE);
  checkFile( CCAMIni->text(),                 *lblCCAMIni,                0,FALSE);

  //bool last_err = m_CheckErreur;
  if (checkFile( repPdftk->text(),      *lblRepPdftk,       1,FALSE) == 0)
     { if (checkFile( DEFAUT_PDFTK,     *lblRepPdftk,       1,FALSE))
          {repPdftk->setText(DEFAUT_PDFTK);
           m_CheckMsgErreur.append(tr("<p><span style=\"font-size:10pt\">Correction du chemin d'accès au programme <b>pdftk</b> par celui qui semble correct.</span></p>") );
           m_CheckMsgErreur.append(DEFAUT_PDFTK);
           m_CheckMsgErreur.append("<br>");
           //m_CheckErreur = last_err;
          }
     }
  //last_err = m_CheckErreur;
  if (checkFile( appliPDF->text(),      *lblAppliPDF,      1,FALSE) == 0)
     { if (checkFile( DEFAUT_PDFVISU,   *lblAppliPDF,      1,FALSE))
          {appliPDF->setText(DEFAUT_PDFVISU);
           m_CheckMsgErreur.append(tr("<p><span style=\"font-size:10pt\">Correction du chemin d'accès au <b> visualisateur de PDF</b> par celui qui semble correct.</span></p>") );
           m_CheckMsgErreur.append(DEFAUT_PDFVISU);
           m_CheckMsgErreur.append("<br>");
           //m_CheckErreur = last_err;
          }
     }
  //.......... Verifie les box de la fenêtre (SplashScreen...)
  specialBoxes();

  if (m_CheckErreur) 		// si drtux a rencontre une erreur affiche le msg d'erreur et nettoie le
   {  QString m = "";
      m = "<p style=\"margin-left:30px;\"><span style=\"font-size:10pt;color:#ff0000\">";
      m += tr("Erreur(s) détectée(s) lors de la vérification&nbsp;:");
      m += "</span></p>";
      msgConfig->append(m);
      m  = "<p style=\"margin-left:70px;\"><span style=\"font-size:10pt;color:#ff0000\">";
      m += m_CheckMsgErreur + "</span></p>";
      msgConfig->append(m);
      m_CheckMsgErreur = "";
  }
  else msgConfig->append(tr("<p align=\"right\"><span style=\"font-size:10pt;color:#5160bd\">Tous les paramètres sont valides.</span></p>") );
  msgConfig->append("<p style=\"margin-left:0px;\">&nbsp;</p>");  // indispensable pour ne pas indenter tout le texte qui suivra...
}


// --------------------------- Qq SLOTS pour recuperer les donnees privates -----------------
/*!\brief Une erreur a-t-elle ete detectee dans le fichier ini ? TRUE = oui
 *  \sa    checkInit()
*/
bool FormConfig::getErreurIni()
{ return m_erreurDrTuxIni; }

/*! \brief Renvoie le message d'erreur concernant le fichier ini.
 *  \sa   erreurInit(), checkInit()
*/
QString FormConfig::getMsgErreurIni()
{ return m_varErreurDrTuxIni; }

/*! \brief Une erreur a-t-elle ete detectee lors de la verification des elements des formulaires ? TRUE = oui
 *  \sa    checkParam()
*/
bool FormConfig::getErreurCheck()
{ return m_CheckErreur; }

/*!\brief Renvoie le message d'erreur concernant les elements du formulaire.
 *  \sa   erreurInit(), checkInit()
*/
QString FormConfig::getMsgErreurCheck()
{ return m_CheckMsgErreur; }

/*! \fn void FormConfig::grabPDFIni(QString &pdfPath, QString &pdftkPath, QString &visualPDF, QString &tmpPath)
 * \brief Recupere les parametres pour la classe CDevolFormPDF.
 * Il convient de verifier l'etat des variables d'erreur apres l'appel a cette fonction. La fenêtre ne s'affiche pas même si une erreur est detectee. Pour l'afficher appeler la fonction exec().
*/
void FormConfig::grabPDFIni(QString &pdfPath, QString &pdftkPath, QString &visualPDF, QString &tmpPath)
{ // Si erreur d'emblee alors affiche la fenêtre de config
  if (m_erreurDrTuxIni || m_CheckErreur)
   {	// Affiche la fenêtre
	QString str = tr("<p align=\"center\"><span style=\"font-size:12pt;font-weight:600;color:#ff0000\">Une erreur dans votre configuration a été détectée<br />lors de l'utilisation de la fonction {{::FORM_PDF () }}.<br /><br />Merci de la corriger en vous aidant des messages ci-dessous.<br /><br /></span></p>");
	str += msgConfig->text();
	msgConfig->clear();
	msgConfig->setText(str);
	exec();
   }

  // Si l'erreur persiste alors renvoie des qstrings nulles
  if (m_erreurDrTuxIni || m_CheckErreur)
   {pdfPath = QString::null;
    pdftkPath = QString::null;
	visualPDF = QString::null;
	tmpPath = QString::null;
   }
  else
   {pdfPath   = repDefautPDF->text();
	pdftkPath = repPdftk->text();
	visualPDF = appliPDF->text();
	tmpPath   = repPDFTmp->text();
   }
}

//----------------------------- getPrivateVariable -----------------------------
/*! \brief Recupere les valeurs du formulaire dans les variables privates de la classe pour permettre de les sauvegarder.
 * Ne devrait pas être appelee par d'autres classes...
*/
void FormConfig::getPrivateVariable()
{
  /*
  m_RepGlossaire = repGlossaire->text();
  m_RepTmpGene = repTmpGeneral->text();
  m_CheminAide = Aide->text();

  m_MenuObs = menuObs->text();
  m_MenuPrescr = menuPrescr->text();
  m_MenuDoc = menuDoc->text();
  m_MenuImage = menuImage->text();

  m_DefautObs = defautObserv->text();
  m_DefautTerrain = defautTerrain->text();

  m_CCAMIni = CCAMIni->text();
  m_CCAMExe = CCAMExe->text();

  m_PDFPath = repDefautPDF->text();
  m_PDFVisu = appliPDF->text();
  m_PDFtk = repPdftk->text();
  m_PDFTmp = repPDFTmp->text();

  m_Contact = GestContact->text();
  m_GestUser = GestUser->text();

  */

  m_CCAMLibPref = CCAMPrefixe->text();
  m_CCAMLibPostf = CCAMPostfixe->text();
  m_CCAMCodePref = CCAMCodePrefixe->text();
  m_CCAMCodePostf = CCAMCodePostfixe->text();

  specialBoxes();
  if (radioOpenCMDI->isChecked())
  { m_DockMenu_ClickBehavior = "openCMDI";
  }
  else
  { m_DockMenu_ClickBehavior = "openCloseList";
  }
}

//----------------------------- writeAllParams ---------------------------------------------------------------
/*!\brief Sauvegarde les parametres du formulaire dans les QString des ini passes en parametres dans setParams().
*/
void FormConfig::writeAllParams()
{ // Recupere les valeurs de la fenêtre vers les variables private
  getPrivateVariable();

  // Initialise les valeurs avec le drtux.ini
  CGestIni::Param_WriteParam(&m_IniParam, "Connexion" , "SplashScreen", m_SplashScreen );

  // Initialise la partie Repertoires , menus et defauts
  CGestIni::Param_WriteParam(&m_IniParam, "Glossaire", "Path",                          repGlossaire->text() );
  CGestIni::Param_WriteParam(&m_IniParam, "Repertoire Temporaire", "Repertoire",        repTmpGeneral->text() );
  CGestIni::Param_WriteParam(&m_IniParam, "Aide", "Path",                               Aide->text() );

  // Initialise les dialogues pour le Form_PDF de CDevilFormPDF
  CGestIni::Param_WriteParam(&m_IniParam, FORM_PDF, "Chemin vers PDF",                  repDefautPDF->text() );
  CGestIni::Param_WriteParam(&m_IniParam, FORM_PDF, "Application visualisateur de PDF", appliPDF->text() );
  CGestIni::Param_WriteParam(&m_IniParam, FORM_PDF, "Chemin vers pdftk",                repPdftk->text() );
  CGestIni::Param_WriteParam(&m_IniParam, FORM_PDF, "Chemin vers tmp",                  repPDFTmp->text() );

  // Listes d'observation et de prescription
  save_listViewRubParameters();
  CGestIni::Param_WriteParam(&m_IniParam, CONTEXT_MENU, "ListImages",                   menuImage->text() );

  // Documents par defaut
  CGestIni::Param_WriteParam(&m_IniParam, "Vigie",                tr("Defaut"),                  defautVigie->text() );

  // Gestion des contacts et utilisateurs
  CGestIni::Param_WriteParam(&m_IniParam, "Gestion des contacts", "PathExe",            GestContact->text() );
  CGestIni::Param_WriteParam(&m_IniParam, "Gestion des utilisateurs", "PathExe",        GestUser->text() );

  // Parametres pour la CCAM
  CGestIni::Param_WriteParam(&m_IniParam, "Codage CCAM", "PathCCAM_Ini",                CCAMIni->text() );
  CGestIni::Param_WriteParam(&m_IniParam, "Codage CCAM", "PathExe",                     CCAMExe->text() );

  CGestIni::Param_WriteParam(&m_IniParam, "Codage CCAM", "LibellePrefix", m_CCAMLibPref );
  CGestIni::Param_WriteParam(&m_IniParam, "Codage CCAM", "LibellePostfix", m_CCAMLibPostf );
  CGestIni::Param_WriteParam(&m_IniParam, "Codage CCAM", "CodePrefix", m_CCAMCodePref );
  CGestIni::Param_WriteParam(&m_IniParam, "Codage CCAM", "CodePostfix", m_CCAMCodePostf );
  CGestIni::Param_WriteParam(&m_IniParam, "Codage CCAM", "Panneau a activer", m_CCAMPanneau );

  // Gestion des dossiers
  CGestIni::Param_WriteParam(&m_IniParam, "Gestion des dossiers", "Nouveau dossier", m_NouveauDossier );
  CGestIni::Param_WriteParam(&m_IniParam, "Gestion des dossiers", "Acces Concurrent", m_DossierConcurrent );
  CGestIni::Param_WriteParam(&m_IniParam, "Observation", tr("Verifier dossier"), m_VerifDoss );

  // Gestion des documents
  //CGestIni::Param_WriteParam(&m_IniParam, "Documents", "Path documents", m_RepDocuments );
  CGestIni::Param_WriteParam(&m_IniParam, "Documents", "MaxImageWidth", m_MaxIamgeWidth );

  // Gestion des docks
  CGestIni::Param_WriteParam(&m_IniUser, DOCK_RUBRIQUES, "Montrer", m_Dock_TabRubrique );
  CGestIni::Param_WriteParam(&m_IniUser, DOCK_GLOSSAIRE, "Montrer", m_Dock_Glossaire );

  CGestIni::Param_WriteParam(&m_IniUser, DOCK_RAPID_MENU, "Montrer",         m_DockMenu_OnScreen );
  CGestIni::Param_WriteParam(&m_IniUser, DOCK_RAPID_MENU, "ShowTerrain",     m_DockMenu_ShowTerrain );
  CGestIni::Param_WriteParam(&m_IniUser, DOCK_RAPID_MENU, "ShowObs",         m_DockMenu_ShowObs );
  CGestIni::Param_WriteParam(&m_IniUser, DOCK_RAPID_MENU, "ShowPrescr",      m_DockMenu_ShowPrescr );
  CGestIni::Param_WriteParam(&m_IniUser, DOCK_RAPID_MENU, "ShowDoc",         m_DockMenu_ShowDoc );
  CGestIni::Param_WriteParam(&m_IniUser, DOCK_RAPID_MENU, "ShowDoc",         m_DockMenu_ShowDoc );
  CGestIni::Param_WriteParam(&m_IniUser, DOCK_RAPID_MENU, "ShowInterv",      m_DockMenu_ShowInterv );
  CGestIni::Param_WriteParam(&m_IniUser, DOCK_RAPID_MENU, "OpenIdent",       m_DockMenu_OpenIdent );
  CGestIni::Param_WriteParam(&m_IniUser, DOCK_RAPID_MENU, "OpenATCD",        m_DockMenu_OpenATCD );
  CGestIni::Param_WriteParam(&m_IniUser, DOCK_RAPID_MENU, "OpenVars",        m_DockMenu_OpenVars );
  CGestIni::Param_WriteParam(&m_IniUser, DOCK_RAPID_MENU, "OpenObs",         m_DockMenu_OpenObs );
  CGestIni::Param_WriteParam(&m_IniUser, DOCK_RAPID_MENU, "OpenPrescr",      m_DockMenu_OpenPrescr );
  CGestIni::Param_WriteParam(&m_IniUser, DOCK_RAPID_MENU, "OpenDoc",         m_DockMenu_OpenDoc );
  CGestIni::Param_WriteParam(&m_IniUser, DOCK_RAPID_MENU, "OpenInterv",      m_DockMenu_OpenInterv );
  CGestIni::Param_WriteParam(&m_IniUser, DOCK_RAPID_MENU, "OpenTerrain",     m_DockMenu_OpenTerrain );
  CGestIni::Param_WriteParam(&m_IniUser, DOCK_RAPID_MENU, "OpenVisuNote",    m_DockMenu_OpenVisuNote);
  CGestIni::Param_WriteParam(&m_IniUser, DOCK_RAPID_MENU, "Glossaire",       m_DockMenu_Glossaire );
  CGestIni::Param_WriteParam(&m_IniUser, DOCK_RAPID_MENU, "ClickOnParent",   m_DockMenu_ClickBehavior );
  CGestIni::Param_WriteParam(&m_IniUser, TAB_RUBRIQUES,   "Display",         m_TabRubriques );
  CGestIni::Param_WriteParam(&m_IniUser, DOCK_RAPID_MENU, "DefaultFontSize", m_DockMenu_DefaultFontSize );
  CGestIni::Param_WriteParam(&m_IniUser, THEME_CFG,       "PopupFntSize",    m_PopMenu_DefaultFontSize );
  CGestIni::Param_WriteParam(&m_IniUser, THEME_CFG,       "Current Theme",   m_Theme );


}


//----------------------------------- buttonOk_clicked() ----------------------------------------
/*! \brief Apres avoir valider les donnees, sauvegarde dans les QString des structures ini passees en parametre dans setParams(). Les donnees peuvent être recuperees par getParams()
 *  Ne devrait pas être appelee par d'autres classes...
*/
void FormConfig::buttonOk_clicked()
{ checkParam();
  //.......... Verifie l'exactitude des donnees
  //if (!m_erreurDrTuxIni && !m_CheckErreur)
    {	//..... Prepare les parametres pour la sauvegarde
	writeAllParams();
    }
  accept();
}

//----------------------------- listBox_highlighted ---------------------------------------------------------------
/*! \brief Slot appele lorsque l'utilisateur selectionne une partie des parametres depuis la liste. Ouvre et affiche le widget correspondant.
*/
void FormConfig::listBox_highlighted( int id )
{ m_WidgetStack->raiseWidget( id );
}


void FormConfig::but_MenuImages_clicked()
{ QString path = "";
  if (QDir(repGlossaire->text()).isRelative() ) path = m_PathAppli + repGlossaire->text();
  else path = repGlossaire->text();
  path = QDir::cleanDirPath(path) + INSERTION_ROOT;

  FormDlgListOff_Listes* pDlg = new FormDlgListOff_Listes(this);
  pDlg->setPath( path );

  pDlg->exec();
  if (pDlg->result() == QDialog::Accepted)
  { menuImage->setText( pDlg->getSelectedItem() );
  }
  delete pDlg;
}


//----------------------------- getDirectory ---------------------------------------------------------------
/*! \brief Demande a l'utilisateur de selectionner un repertoire en debutant dans le repertoire initialPath avec le message de dialogue message.
 *  \return si possible le chemin relatif par rapport a l'application en cours.
*/
QString FormConfig::getDirectory(QString initialPath, const char* message)
{QString path = "";
 if (!QDir(initialPath).exists()) path = m_PathAppli;
 else path = initialPath;

 QString s = QFileDialog::getExistingDirectory(
                    path,
                    this,
                    message,
                    message,
                    TRUE );
 if (s=="") return initialPath;
 s = CGestIni::AbsoluteToRelativePath(m_PathAppli,s);
 return s;
}


//----------------------------- getFile ---------------------------------------------------------------
/*! \brief Demande a l'utilisateur de selectionner un repertoire en debutant dans le repertoire initialPath avec le message de dialogue message.
 *  \param  typePath : indiquez si recuperation du fichier en RELATIVE_PATH ou en ABSOLUTE_PATH
 *  \return si possible le chemin relatif par rapport a l'application en cours.
*/
QString FormConfig::getFile(QString initialPath, const char* message, bool typePath)
{QString path = "";
 path = initialPath;
 if (path.find("$Glossaire") != -1 )
    { path.replace ("$Glossaire", repGlossaire->text() );
      if (QDir(path).isRelative()) path.prepend(m_PathAppli);
      path = QDir::cleanDirPath(path);
    }
 if (!QFile(path).exists()) path = m_PathAppli;

 QString s = QFileDialog::getOpenFileName(
                    path,
                    "Tous les fichiers (*)",
                    this,
                    message,
                    message );
 if (s=="") return initialPath;
 if (typePath == RELATIVE_PATH)
    { path = CGestIni::AbsoluteToRelativePath(m_PathAppli,s);
     if (path.startsWith(repGlossaire->text())) path.replace(repGlossaire->text(), "$Glossaire");
    }
 else {path=s;}
 return path;
}

void FormConfig::but_tmpRepTmp_clicked()
{ repTmpGeneral->setText(getDirectory(repTmpGeneral->text(), TR("Répertoire temporaire général")));
}

void FormConfig::but_RepGlossaire_clicked()
{ repGlossaire->setText(getDirectory(repGlossaire->text(), TR("Répertoire du glossaire")));
}

void FormConfig::but_RepAide_clicked()
{ Aide->setText(getDirectory(Aide->text(), TR("Répertoire de l'aide")));
}

void FormConfig::but_MasqueVigie_clicked()
{ defautVigie->setText(getFile(defautVigie->text(), TR("Masque par défaut des observations"), RELATIVE_PATH));
}

void FormConfig::but_AppliContact_clicked()
{ GestContact->setText(getFile(GestContact->text(), TR("Application gestion des contacts"), RELATIVE_PATH));
}

void FormConfig::but_AppliUtilisateur_clicked()
{ GestUser->setText(getFile(GestUser->text(), TR("Application pour la gestion des utilisateurs"), RELATIVE_PATH));
}

void FormConfig::but_CCAMExe_clicked()
{ CCAMExe->setText(getFile(CCAMExe->text(), TR("Fichier exécutable de la CCAM"), RELATIVE_PATH));
}

void FormConfig::but_CCAMConfig_clicked()
{ CCAMIni->setText(getFile(CCAMIni->text(), TR("Fichier ini de CCAM"), RELATIVE_PATH));
}

void FormConfig::but_RepDesPDF_clicked()
{ repDefautPDF->setText(getDirectory(Aide->text(), TR("Répertoire o se trouvent les PDF vierges")));
  if ( checkDir(repDefautPDF->text(), *lblRepPDF)==3)
    { CGestIni::Param_WriteParam(&m_IniParam, FORM_PDF, "Chemin vers PDF", repDefautPDF->text() );
    }
}

void FormConfig::but_RepTmpPDF_clicked()
{ repPDFTmp->setText(getDirectory(repPDFTmp->text(), TR("Répertoire temporaire pour la gestion des PDF")));
 if ( checkDir(repPDFTmp->text(), *lblRepPDFTmp)==3)
    { CGestIni::Param_WriteParam(&m_IniParam, FORM_PDF, "Chemin vers tmp",  repPDFTmp->text() );
    }
}

void FormConfig::but_AppliPdftk_clicked()
{ repPdftk->setText(getFile( repPdftk->text(), TR("Application : PDFTK"), ABSOLUTE_PATH ) );
  if ( checkFile(repPdftk->text(), *lblRepPdftk, 1, FALSE))
    {CGestIni::Param_WriteParam(&m_IniParam, FORM_PDF, "Chemin vers pdftk", repPdftk->text() );
    }
}

void FormConfig::but_VisuPDF_clicked()
{ appliPDF->setText(getFile(appliPDF->text(), TR("Application pour visualiser les PDF"), ABSOLUTE_PATH));
  if ( checkFile(appliPDF->text(), *lblAppliPDF, 1, FALSE))
    {CGestIni::Param_WriteParam(&m_IniParam, FORM_PDF, "Application visualisateur de PDF", appliPDF->text() );
    }
}

void FormConfig::pushButtonAddRubrique_clicked()
{AddNewRub();
}

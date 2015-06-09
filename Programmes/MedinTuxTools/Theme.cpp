
// C++ Implementation: Themeable.cpp
//
// Description:
//
//
// Author: Eric MAEKER <eric.maeker.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "Theme.h"
#include <qdir.h>

/*! \class Theme
 *  Cette classe est entièrement statique. Avant d'accéder aux fonction de récupération des icones, il faut définir le chemin vers le répertoire de thème par la fonction setPath(). Celui-ci est structuré de la sorte :
 * racine : les icones de taille mal définie
 * 16x16 : répertoire pour les icones visualisables dans les Popup et les ListView
 * 22x22 : pour les icones des bouttons
 * 32x32 : pour les listBox des fenêtres de paramètre.
 * dans le répertoire racine existe un fichier Theme.ini qui contient les informations sur les couleurs, polices...

 *  Cette classe peut être utilisée par toutes les applications de MedinTux.

 *  Pour limiter l'utilisation massive du CPU au démarrage de l'application les icones sont chargés au moment de leur demande par les fonctions getIcon....().
*/

// Faut tou initialiser avant d'utiliser la classe.
QString   Theme::m_Path = "";
QString   Theme::m_ThemeIni = "";

QIconSet Theme::m_IconIsDeleted = QIconSet();
QIconSet Theme::m_IconDelete = QIconSet();
QIconSet Theme::m_IconChangeDate = QIconSet();
QIconSet Theme::m_IconCadenas = QIconSet();
QIconSet Theme::m_IconSave = QIconSet();
QIconSet Theme::m_IconPrint = QIconSet();
QIconSet Theme::m_IconTherapeutique = QIconSet();
QIconSet Theme::m_IconNewDoc = QIconSet();
QIconSet Theme::m_IconNewFolder = QIconSet();
QIconSet Theme::m_IconTextLeft = QIconSet();
QIconSet Theme::m_IconTextRight = QIconSet();
QIconSet Theme::m_IconTextCenter = QIconSet();
QIconSet Theme::m_IconTextJustify = QIconSet();
QIconSet Theme::m_IconTextBold = QIconSet();
QIconSet Theme::m_IconTextItalic = QIconSet();
QIconSet Theme::m_IconTextUnderline = QIconSet();
QIconSet Theme::m_IconTextUndo = QIconSet();
QIconSet Theme::m_IconTextRedo = QIconSet();
QIconSet Theme::m_IconTextCut = QIconSet();
QIconSet Theme::m_IconTextCopy = QIconSet();
QIconSet Theme::m_IconTextPaste = QIconSet();
QIconSet Theme::m_IconTextSelectAll = QIconSet();
QIconSet Theme::m_IconNext = QIconSet();
QIconSet Theme::m_IconPrevious = QIconSet();
QIconSet Theme::m_IconMedicaments = QIconSet();
QIconSet Theme::m_IconVidal = QIconSet();

QIconSet Theme::m_IconListLocked = QIconSet();
QIconSet Theme::m_IconListEdit = QIconSet();
QIconSet Theme::m_IconListView = QIconSet();
QIconSet Theme::m_IconForbidden = QIconSet();
QIconSet Theme::m_IconDownArrow = QIconSet();
QIconSet Theme::m_IconRightArrow = QIconSet();
QIconSet Theme::m_IconListAdd = QIconSet();
QIconSet Theme::m_IconListSave = QIconSet();
QIconSet Theme::m_IconListDelete = QIconSet();
QIconSet Theme::m_IconListDateTime = QIconSet();
QIconSet Theme::m_IconListFolder = QIconSet();
QIconSet Theme::m_IconListHTML = QIconSet();
QIconSet Theme::m_IconListItem = QIconSet();
QIconSet Theme::m_IconListWarning = QIconSet();
QIconSet Theme::m_IconListPriority = QIconSet();
QIconSet Theme::m_IconListOK = QIconSet();
QIconSet Theme::m_IconListTypeDocument = QIconSet();
QIconSet Theme::m_IconListTypeObservation = QIconSet();
QIconSet Theme::m_IconListTypePrescription = QIconSet();
QIconSet Theme::m_IconListTypeCertificat = QIconSet();
QIconSet Theme::m_IconListTypeTerrain = QIconSet();
QIconSet Theme::m_PixmapIdentite = QIconSet();

QColor Theme::m_RapidAccess_LighterBackground = QColor(255,255,255);
QColor Theme::m_RapidAccess_DarkerBackground  = QColor(245,245,255);
QColor Theme::m_RapidAccess_HighLightColor    = QColor(210,210,255);
QColor Theme::m_RapidAccess_FontColor_Ident   = QColor(0,0,0);
QColor Theme::m_RapidAccess_FontColor_Terrain = QColor(165,165,165);
QColor Theme::m_RapidAccess_FontColor_Obs     = QColor(0 , 0 , 50);
QColor Theme::m_RapidAccess_FontColor_Prescr  = QColor(50 , 0 , 0);
QColor Theme::m_RapidAccess_FontColor_Doc     = QColor(0 , 50 , 0);

bool   Theme::m_RapidAccess_LighterBackground_Defined = FALSE;
bool   Theme::m_RapidAccess_DarkerBackground_Defined  = FALSE;
bool   Theme::m_RapidAccess_HighLightColor_Defined    = FALSE;
bool   Theme::m_RapidAccess_FontColor_Ident_Defined   = FALSE;
bool   Theme::m_RapidAccess_FontColor_Terrain_Defined = FALSE;
bool   Theme::m_RapidAccess_FontColor_Obsr_Defined    = FALSE;
bool   Theme::m_RapidAccess_FontColor_Prescr_Defined  = FALSE;
bool   Theme::m_RapidAccess_FontColor_Doc_Defined     = FALSE;

int     Theme::m_FontSize_Popups = -1;
int     Theme::m_FontSize_Menus  = -1;
bool    Theme::m_FontSize_Popups_Defined  = FALSE;
bool    Theme::m_FontSize_Menus_Defined   = FALSE;


// Viennent ensuite les méthodes statiques
// qui donnent accès aux données n'importe où dans le programme
// sans se souciès de quoique ce soit, et surtout sans passer
// par le pointeur global de CApp.

QIconSet Theme::getIconIsDeleted		()
{ if (m_IconIsDeleted.isNull() )
   m_IconIsDeleted.setPixmap     (QPixmap(m_Path +"22x22/trashcan_full.png"),	QIconSet::Automatic);
  return m_IconIsDeleted;
}

QIconSet Theme::getIconDelete		()
{ if (m_IconDelete.isNull() )
  m_IconDelete.setPixmap        (QPixmap(m_Path +"22x22/trashcan_empty.png"),QIconSet::Automatic);
  return m_IconDelete;
}

QIconSet Theme::getIconChangeDate	()
{ if (m_IconChangeDate.isNull() )
  m_IconChangeDate.setPixmap    (QPixmap(m_Path + "changerdate.png"),	QIconSet::Automatic);
  return m_IconChangeDate;
}

QIconSet Theme::getIconCadenas		()
{ if (m_IconCadenas.isNull() )
  m_IconCadenas.setPixmap       (QPixmap(m_Path + "22x22/locked.png"),	QIconSet::Automatic);
  return m_IconCadenas;
}

QIconSet Theme::getIconSave		()
{ if (m_IconSave.isNull() )
  m_IconSave.setPixmap          (QPixmap(m_Path +"22x22/filesave.png") ,	QIconSet::Automatic);
  return m_IconSave;
}

QIconSet Theme::getIconPrint		()
{ if (m_IconPrint.isNull() )
  m_IconPrint.setPixmap         (QPixmap(m_Path +"22x22/print.png")  ,	QIconSet::Automatic);
  return m_IconPrint;
}

QIconSet Theme::getIconVidal	()
{ if (m_IconVidal.isNull() )
  m_IconVidal.setPixmap    (QPixmap(m_Path +"22x22/vidal.png"),    QIconSet::Automatic);
  return m_IconVidal;
}

QIconSet Theme::getIconMedicaments	()
{ if (m_IconMedicaments.isNull() )
  m_IconMedicaments.setPixmap    (QPixmap(m_Path +"22x22/pills.png"),    QIconSet::Automatic);
  return m_IconMedicaments;
}

QIconSet Theme::getIconNewDoc		()
{ if (m_IconNewDoc.isNull() )
   m_IconNewDoc.setPixmap        (QPixmap(m_Path +"22x22/filenew.png")  ,	QIconSet::Automatic);
  return m_IconNewDoc;
}

QIconSet Theme::getIconNewFolder	()
{ if (m_IconNewFolder.isNull() )
   m_IconNewFolder.setPixmap        (QPixmap(m_Path +"22x22/folder_new.png")  ,	QIconSet::Automatic);
  return m_IconNewFolder;
}
QIconSet Theme::getIconTextLeft		()
{ if (m_IconTextLeft.isNull() )
  m_IconTextLeft.setPixmap      (QPixmap(m_Path +"22x22/textleft.png") , 	QIconSet::Automatic);
  return m_IconTextLeft;
}

QIconSet Theme::getIconTextRight	()
{ if (m_IconTextRight.isNull() )
  m_IconTextRight.setPixmap     (QPixmap(m_Path +"22x22/textright.png") , 	QIconSet::Automatic);
  return m_IconTextRight;
}

QIconSet Theme::getIconTextCenter	()
{ if (m_IconTextCenter.isNull() )
  m_IconTextCenter.setPixmap    (QPixmap(m_Path +"22x22/textcenter.png") ,	QIconSet::Automatic);
  return m_IconTextCenter;
}

QIconSet Theme::getIconTextJustify	()
{ if (m_IconTextJustify.isNull() )
  m_IconTextJustify.setPixmap   (QPixmap(m_Path +"22x22/textjustify.png"),	QIconSet::Automatic);
  return m_IconTextJustify;
}

QIconSet Theme::getIconTextBold		()
{ if (m_IconTextBold.isNull() )
  m_IconTextBold.setPixmap      (QPixmap(m_Path +"22x22/textbold.png")  ,	QIconSet::Automatic);
  return m_IconTextBold;
}

QIconSet Theme::getIconTextItalic	()
{ if (m_IconTextItalic.isNull() )
  m_IconTextItalic.setPixmap    (QPixmap(m_Path +"22x22/textitalic.png")  ,	QIconSet::Automatic);
  return m_IconTextItalic;
}

QIconSet Theme::getIconTextUnderline	()
{ if (m_IconTextUnderline.isNull() )
  m_IconTextUnderline.setPixmap (QPixmap(m_Path +"22x22/textunderline.png"),	QIconSet::Automatic);
  return m_IconTextUnderline;
}

QIconSet Theme::getIconUndo		()
{ if (m_IconTextUndo.isNull() )
  m_IconTextUndo.setPixmap      (QPixmap(m_Path +"22x22/redo.png")  ,	QIconSet::Automatic);
  return m_IconTextUndo;
}


QIconSet Theme::getIconRedo		()
{ if (m_IconTextRedo.isNull() )
  m_IconTextRedo.setPixmap      (QPixmap(m_Path +"22x22/undo.png")  ,	QIconSet::Automatic);
  return m_IconTextRedo;
}

QIconSet Theme::getIconCut		()
{ if (m_IconTextCut.isNull() )
  m_IconTextCut.setPixmap       (QPixmap(m_Path +"22x22/editcut.png"),	QIconSet::Automatic);
  return m_IconTextCut;
}

QIconSet Theme::getIconCopy		()
{ if (m_IconTextCopy.isNull() )
  m_IconTextCopy.setPixmap      (QPixmap(m_Path +"22x22/editcopy.png")  ,	QIconSet::Automatic);
  return m_IconTextCopy;
}

QIconSet Theme::getIconPaste		()
{ if (m_IconTextPaste.isNull() )
  m_IconTextPaste.setPixmap     (QPixmap(m_Path +"22x22/editpaste.png")  ,	QIconSet::Automatic);
  return m_IconTextPaste;
}

QIconSet Theme::getIconSelectAll	()
{ if (m_IconTextSelectAll.isNull() )
  m_IconTextSelectAll.setPixmap (QPixmap(m_Path +"22x22/edit-select-all.png"),QIconSet::Automatic);
  return m_IconTextSelectAll;
}


QIconSet Theme::getIconListLocked		()
{ if (m_IconListLocked.isNull() )
   m_IconListLocked.setPixmap        (QPixmap(m_Path + "16x16/locked.png")  ,	QIconSet::Automatic);
  return m_IconListLocked;
}

QIconSet Theme::getIconListOK		()
{ if (m_IconListOK.isNull() )
   m_IconListOK.setPixmap        (QPixmap(m_Path + "16x16/listok.png")  ,	QIconSet::Automatic);
  return m_IconListOK;
}

QIconSet Theme::getIconForbidden	()
{ if (m_IconNewDoc.isNull() )
   m_IconNewDoc.setPixmap        (QPixmap(m_Path + "22x22/locked.png")  ,	QIconSet::Automatic);
  return m_IconNewDoc;
}

QIconSet Theme::getIconDownArrow	()
{ if (m_IconDownArrow.isNull() )
  m_IconDownArrow.setPixmap (QPixmap(m_Path + "22x22/1downarrow.png"),QIconSet::Automatic);
  return m_IconDownArrow;
}

QIconSet Theme::getIconRightArrow	()
{if (m_IconRightArrow.isNull() )
  m_IconRightArrow.setPixmap (QPixmap(m_Path + "22x22/1rightarrow.png"),QIconSet::Automatic);
  return m_IconRightArrow;
}


QIconSet Theme::getIconNext	()
{if (m_IconNext.isNull() )
  m_IconNext.setPixmap (QPixmap(m_Path + "22x22/next.png"),QIconSet::Automatic);
  return m_IconNext;
}
QIconSet Theme::getIconPrevious	()
{if (m_IconPrevious.isNull() )
  m_IconPrevious.setPixmap (QPixmap(m_Path + "22x22/previous.png"),QIconSet::Automatic);
  return m_IconPrevious;
}


QIconSet Theme::getIconListAdd	()
{if (m_IconListAdd.isNull() )
  m_IconListAdd.setPixmap (QPixmap(m_Path + "16x16/add.png"),QIconSet::Automatic);
  return m_IconListAdd;
}

QIconSet Theme::getIconListView		()
{if (m_IconListView.isNull() )
  m_IconListView.setPixmap (QPixmap(m_Path + "16x16/info.png"),QIconSet::Automatic);
  return m_IconListView;
}
QIconSet  Theme::getIconListTypeIdentite ()
{if (m_PixmapIdentite.isNull() )
     m_PixmapIdentite.setPixmap(QPixmap(m_Path + "16x16/identity.png"), QIconSet::Automatic);
 return m_PixmapIdentite;
}

QIconSet Theme::getIconListSave	()
{if (m_IconListSave.isNull() )
  m_IconListSave.setPixmap (QPixmap(m_Path + "16x16/save.png"),QIconSet::Automatic);
  return m_IconListSave;
}

QIconSet Theme::getIconListDelete	()
{if (m_IconListDelete.isNull() )
  m_IconListDelete.setPixmap (QPixmap(m_Path + "16x16/remove.png"),QIconSet::Automatic);
  return m_IconListDelete;
}

QIconSet Theme::getIconListDateTime()
{if (m_IconListDateTime.isNull() )
  m_IconListDateTime.setPixmap (QPixmap(m_Path + "16x16/datetime.png"),QIconSet::Automatic);
  return m_IconListDateTime;
}

QIconSet Theme::getIconListFolder	()
{if (m_IconListFolder.isNull() )
  m_IconListFolder.setPixmap (QPixmap(m_Path + "16x16/folder.png"),QIconSet::Automatic);
  return m_IconListFolder;
}

QIconSet Theme::getIconListHTML	()
{if (m_IconListHTML.isNull() )
  m_IconListHTML.setPixmap (QPixmap(m_Path + "16x16/html.png"),QIconSet::Automatic);
  return m_IconListHTML;
}

QIconSet Theme::getIconListItem	()
{if (m_IconListItem.isNull() )
  m_IconListItem.setPixmap (QPixmap(m_Path + "16x16/item.png"),QIconSet::Automatic);
  return m_IconListItem;
}

QIconSet Theme::getIconListEdit	()
{if (m_IconListEdit.isNull() )
  m_IconListEdit.setPixmap (QPixmap(m_Path + "16x16/edit.png"),QIconSet::Automatic);
  return m_IconListEdit;
}

QIconSet Theme::getIconListWarning	()
{if (m_IconListWarning.isNull() )
  m_IconListWarning.setPixmap (QPixmap(m_Path + "16x16/warning.png"),QIconSet::Automatic);
  return m_IconListWarning;
}

QIconSet Theme::getIconListPriority	()
{if (m_IconListPriority.isNull() )
  m_IconListPriority.setPixmap (QPixmap(m_Path + "16x16/priority.png"),QIconSet::Automatic);
  return m_IconListPriority;
}

QIconSet Theme::getIconListTypeDocument	()
{if (m_IconListTypeDocument.isNull() )
  m_IconListTypeDocument.setPixmap (QPixmap(m_Path + "16x16/type_Documents.png"),QIconSet::Automatic);
  return m_IconListTypeDocument;
}
QIconSet Theme::getIconListTypeObservation	()
{if (m_IconListTypeObservation.isNull() )
  m_IconListTypeObservation.setPixmap (QPixmap(m_Path+"16x16/type_Observation.png"),QIconSet::Automatic);
  return m_IconListTypeObservation;
}
QIconSet Theme::getIconListTypePrescription	()
{if (m_IconListTypePrescription.isNull() )
  m_IconListTypePrescription.setPixmap (QPixmap(m_Path + "16x16/type_Prescription.png"),QIconSet::Automatic);
  return m_IconListTypePrescription;
}
QIconSet Theme::getIconListTypeCertificat	()
{if (m_IconListTypeCertificat.isNull() )
  m_IconListTypeCertificat.setPixmap (QPixmap(m_Path + "16x16/type_certificat.png"),QIconSet::Automatic);
  return m_IconListTypeCertificat;
}
QIconSet Theme::getIconListTypeTerrain	()
{if (m_IconListTypeTerrain.isNull() )
  m_IconListTypeTerrain.setPixmap (QPixmap(m_Path + "16x16/type_Terrain.png"),QIconSet::Automatic);
  return m_IconListTypeTerrain;
}



QIconSet Theme::getIconForMyEditText(const char* icone)
{ switch (*icone)
  { case 'F' : // Folder
	return getIconListFolder();
	break;
    case 'T' : // Textes composés TXT, RTF, HTML
	return getIconListHTML();
	break;
    default  : // Items
	return getIconListItem();
	break;
  }
}

QIconSet Theme::getIconSet(const QString iconeFile)
{return QIconSet(getIcon(iconeFile));
}

QPixmap  Theme::getIcon(const QString iconeFile)
{return QPixmap(m_Path + iconeFile);
}

QColor Theme::getFromIniFile(const char* section, const char* var, bool& finded, QColor& color)
{ // gagner du temps process
  if (finded) return color;
  if (m_ThemeIni=="") return color;
  // Récupère la couleur dans le Ini
  QString r,g,b;
  r=""; g=""; b="";
  if (CGestIni::Param_ReadParam(m_ThemeIni, section, var, &r, &g, &b) != 0)
  { return color; }

  if ((r!="") && (g!="") && (b!=""))
  { color = QColor(r.toInt(), g.toInt(), b.toInt() );
    finded = TRUE;
    return color; // valeur du ini
  }
  else
  { finded = TRUE;
    return color; // valeur par défaut
  }
}

int Theme::getFromIniFile(const char* section, const char* var, bool& finded, int& value)
{ // gagner du temps process
  if (finded) return value;
  if (m_ThemeIni=="") return value;
  // Récupère la couleur dans le Ini
  QString val = "";
  if (CGestIni::Param_ReadParam(m_ThemeIni, section, var, &val) != 0)
  { return value; }

  if (val!="")
  { value = val.toInt();
    finded = TRUE;
    return value; // valeur du ini
  }
  else
  { finded = TRUE;
    return value; // valeur par défaut
  }
}

/*!\brief Récupère la couleur du background illuminé pour la ListView */
QColor Theme::getRapidAccess_LighterBackground()
{ return getFromIniFile("RapidAccess", "LighterBackground",
                 m_RapidAccess_LighterBackground_Defined, m_RapidAccess_LighterBackground);
}
/*!\brief Récupère la couleur du background sombre pour la ListView */
QColor Theme::getRapidAccess_DarkerBackground()
{ return getFromIniFile("RapidAccess", "DarkerBackground",
                 m_RapidAccess_DarkerBackground_Defined, m_RapidAccess_DarkerBackground);
}
/*!\brief Récupère la couleur du mise en lumière de l'item pour la ListView */
QColor Theme::getRapidAccess_HighLightColor()
{ return getFromIniFile("RapidAccess", "HighLightColor",
                 m_RapidAccess_HighLightColor_Defined, m_RapidAccess_HighLightColor);
}


/*!\brief Récupère la couleur de la police de caractère pour l'identité */
QColor Theme::getRapidAccess_FontColor_Ident()
{ return getFromIniFile("RapidAccess", "FontColor_Ident",
                 m_RapidAccess_FontColor_Ident_Defined, m_RapidAccess_FontColor_Ident);
}

/*!\brief Récupère la couleur de la police de caractère pour les items de terrains*/
QColor Theme::getRapidAccess_FontColor_Terrain()
{ return getFromIniFile("RapidAccess", "FontColor_Terrain",
                 m_RapidAccess_FontColor_Terrain_Defined, m_RapidAccess_FontColor_Terrain);
}

/*!\brief Récupère la couleur de la police de caractère pour les observations */
QColor Theme::getRapidAccess_FontColor_Obs()
{ return getFromIniFile("RapidAccess", "FontColor_Obs",
                 m_RapidAccess_FontColor_Obsr_Defined, m_RapidAccess_FontColor_Obs);
}

/*!\brief Récupère la couleur de la police de caractère pour les prescriptions */
QColor Theme::getRapidAccess_FontColor_Prescr()
{ return getFromIniFile("RapidAccess", "FontColor_Prescr",
                 m_RapidAccess_FontColor_Prescr_Defined, m_RapidAccess_FontColor_Prescr);
}

/*!\brief Récupère la couleur de la police de caractère pour les documents */
QColor Theme::getRapidAccess_FontColor_Doc()
{ return getFromIniFile("RapidAccess", "FontColor_Doc",
                 m_RapidAccess_FontColor_Doc_Defined, m_RapidAccess_FontColor_Doc);
}

int Theme::getFontSize_Popups()
{  return getFromIniFile("FontSize", "PopupMenus",
                 m_FontSize_Popups_Defined, m_FontSize_Popups);
}

void Theme::setFontSize_Menu(int fntSize)
{ m_FontSize_Popups_Defined = m_FontSize_Menus_Defined = TRUE;
  m_FontSize_Popups         = m_FontSize_Menus         = fntSize;
}

int Theme::getFontSize_Menus()
{  return getFromIniFile("FontSize", "Menus",
                 m_FontSize_Menus_Defined, m_FontSize_Menus);
}

/*! \brief Renvoie le répertoire de thème en cours. */
QString  Theme::getPath(bool withSeparator)
{if (withSeparator==FALSE)
    {if (m_Path.endsWith("/")) return m_Path.left(m_Path.length()-1);
    }
 return m_Path;
}

/*! \brief Définit le répertoire de thème à utiliser. Fonction OBLIGATOIRE avant toute autre pour définir le répertoire sinon risque de plantage. */
void Theme::setPath(const QString p)
{ m_Path = p;
  QString pathIni = p + "/Theme.ini";
  pathIni = QDir::cleanDirPath(pathIni);
  // Récupère au passage le fichier ini du theme qui contient les codes couleur
  if (!QFile(pathIni).exists())
  { createDefaultIniFile();
  }
  CGestIni::Param_UpdateFromDisk(pathIni, m_ThemeIni);
}

/*! \brief Crée le fichier de toute pièce pour le thème par défaut. */
void Theme::createDefaultIniFile()
{ QString pathIni = m_Path + "/Theme.ini";
  pathIni = QDir::cleanDirPath(pathIni);
  m_ThemeIni = "[RapidAccess]\r\n";

  CGestIni::Param_WriteParam(&m_ThemeIni, "RapidAccess", "LighterBackground", "255","255","255");
  CGestIni::Param_WriteParam(&m_ThemeIni, "RapidAccess", "DarkerBackground", "245","245","255");
  CGestIni::Param_WriteParam(&m_ThemeIni, "RapidAccess", "HighLightColor", "210","210","255");
  CGestIni::Param_WriteParam(&m_ThemeIni, "RapidAccess", "FontColor_Ident", "0","0","0");
  CGestIni::Param_WriteParam(&m_ThemeIni, "RapidAccess", "FontColor_Terrain", "160","160","160");
  CGestIni::Param_WriteParam(&m_ThemeIni, "RapidAccess", "FontColor_Obs", "0","0","50");
  CGestIni::Param_WriteParam(&m_ThemeIni, "RapidAccess", "FontColor_Prescr", "50","0","0");
  CGestIni::Param_WriteParam(&m_ThemeIni, "RapidAccess", "FontColor_Doc", "0","50","0");

  CGestIni::Param_WriteParam(&m_ThemeIni, "FontSize", "PopupMenus", "9");
  CGestIni::Param_WriteParam(&m_ThemeIni, "FontSize", "Menus", "9");

  CGestIni::Param_UpdateToDisk(pathIni, m_ThemeIni);
}

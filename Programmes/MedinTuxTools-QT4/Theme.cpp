
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
//Added by qt3to4:
#include <QPixmap>

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

QIcon Theme::m_IconIsDeleted = QIcon();
QIcon Theme::m_IconDelete = QIcon();
QIcon Theme::m_IconChangeDate = QIcon();
QIcon Theme::m_IconCadenas = QIcon();
QIcon Theme::m_IconSave = QIcon();
QIcon Theme::m_IconPrint = QIcon();
QIcon Theme::m_IconTherapeutique = QIcon();
QIcon Theme::m_IconNewDoc = QIcon();
QIcon Theme::m_IconNewFolder = QIcon();
QIcon Theme::m_IconTextLeft = QIcon();
QIcon Theme::m_IconTextRight = QIcon();
QIcon Theme::m_IconTextCenter = QIcon();
QIcon Theme::m_IconTextJustify = QIcon();
QIcon Theme::m_IconTextBold = QIcon();
QIcon Theme::m_IconTextItalic = QIcon();
QIcon Theme::m_IconTextUnderline = QIcon();
QIcon Theme::m_IconTextUndo = QIcon();
QIcon Theme::m_IconTextRedo = QIcon();
QIcon Theme::m_IconTextCut = QIcon();
QIcon Theme::m_IconTextCopy = QIcon();
QIcon Theme::m_IconTextPaste = QIcon();
QIcon Theme::m_IconTextSelectAll = QIcon();
QIcon Theme::m_IconNext = QIcon();
QIcon Theme::m_IconPrevious = QIcon();
QIcon Theme::m_IconMedicaments = QIcon();
QIcon Theme::m_IconVidal = QIcon();

QIcon Theme::m_IconListLocked = QIcon();
QIcon Theme::m_IconListEdit = QIcon();
QIcon Theme::m_IconListView = QIcon();
QIcon Theme::m_IconForbidden = QIcon();
QIcon Theme::m_IconDownArrow = QIcon();
QIcon Theme::m_IconRightArrow = QIcon();
QIcon Theme::m_IconListAdd = QIcon();
QIcon Theme::m_IconListSave = QIcon();
QIcon Theme::m_IconListDelete = QIcon();
QIcon Theme::m_IconListDateTime = QIcon();
QIcon Theme::m_IconListFolder = QIcon();
QIcon Theme::m_IconListHTML = QIcon();
QIcon Theme::m_IconListItem = QIcon();
QIcon Theme::m_IconListWarning = QIcon();
QIcon Theme::m_IconListPriority = QIcon();
QIcon Theme::m_IconListOK = QIcon();
QIcon Theme::m_IconListTypeDocument = QIcon();
QIcon Theme::m_IconListTypeObservation = QIcon();
QIcon Theme::m_IconListTypePrescription = QIcon();
QIcon Theme::m_IconListTypeCertificat = QIcon();
QIcon Theme::m_IconListTypeTerrain = QIcon();
QIcon Theme::m_PixmapIdentite = QIcon();

QColor Theme::m_RapidAccess_LighterBackground = QColor(255,255,255);
QColor Theme::m_RapidAccess_DarkerBackground  = QColor(245,245,255);
QColor Theme::m_RapidAccess_HighLightColor    = QColor(210,210,255);
QColor Theme::m_RapidAccess_FontColor_Ident   = QColor(0,0,0);
QColor Theme::m_RapidAccess_FontColor_Terrain = QColor(165,165,165);
QColor Theme::m_RapidAccess_FontColor_Obs     = QColor(0 , 0 , 50);
QColor Theme::m_RapidAccess_FontColor_Prescr  = QColor(50 , 0 , 0);
QColor Theme::m_RapidAccess_FontColor_Doc     = QColor(0 , 50 , 0);

bool   Theme::m_RapidAccess_LighterBackground_Defined = false;
bool   Theme::m_RapidAccess_DarkerBackground_Defined  = false;
bool   Theme::m_RapidAccess_HighLightColor_Defined    = false;
bool   Theme::m_RapidAccess_FontColor_Ident_Defined   = false;
bool   Theme::m_RapidAccess_FontColor_Terrain_Defined = false;
bool   Theme::m_RapidAccess_FontColor_Obsr_Defined    = false;
bool   Theme::m_RapidAccess_FontColor_Prescr_Defined  = false;
bool   Theme::m_RapidAccess_FontColor_Doc_Defined     = false;

int     Theme::m_FontSize_Popups = -1;
int     Theme::m_FontSize_Menus  = -1;
bool    Theme::m_FontSize_Popups_Defined  = false;
bool    Theme::m_FontSize_Menus_Defined   = false;


// Viennent ensuite les méthodes statiques
// qui donnent accès aux données n'importe où dans le programme
// sans se souciès de quoique ce soit, et surtout sans passer
// par le pointeur global de CApp.

QIcon Theme::getIconIsDeleted		()
{ if (m_IconIsDeleted.isNull() )
   m_IconIsDeleted.addPixmap     (QPixmap(m_Path +"22x22/trashcan_full.png"));
  return m_IconIsDeleted;
}

QIcon Theme::getIconDelete		()
{ if (m_IconDelete.isNull() )
  m_IconDelete.addPixmap        (QPixmap(m_Path +"22x22/trashcan_empty.png"));
  return m_IconDelete;
}

QIcon Theme::getIconChangeDate	()
{ if (m_IconChangeDate.isNull() )
  m_IconChangeDate.addPixmap    (QPixmap(m_Path + "changerdate.png"));
  return m_IconChangeDate;
}

QIcon Theme::getIconCadenas		()
{ if (m_IconCadenas.isNull() )
  m_IconCadenas.addPixmap       (QPixmap(m_Path + "22x22/locked.png"));
  return m_IconCadenas;
}

QIcon Theme::getIconSave		()
{ if (m_IconSave.isNull() )
  m_IconSave.addPixmap          (QPixmap(m_Path +"22x22/filesave.png") );
  return m_IconSave;
}

QIcon Theme::getIconPrint		()
{ if (m_IconPrint.isNull() )
  m_IconPrint.addPixmap         (QPixmap(m_Path +"22x22/print.png") );
  return m_IconPrint;
}

QIcon Theme::getIconVidal	()
{ if (m_IconVidal.isNull() )
  m_IconVidal.addPixmap    (QPixmap(m_Path +"22x22/vidal.png"));
  return m_IconVidal;
}

QIcon Theme::getIconMedicaments	()
{ if (m_IconMedicaments.isNull() )
  m_IconMedicaments.addPixmap    (QPixmap(m_Path +"22x22/pills.png"));
  return m_IconMedicaments;
}

QIcon Theme::getIconNewDoc		()
{ if (m_IconNewDoc.isNull() )
   m_IconNewDoc.addPixmap        (QPixmap(m_Path +"22x22/filenew.png") );
  return m_IconNewDoc;
}

QIcon Theme::getIconNewFolder	()
{ if (m_IconNewFolder.isNull() )
   m_IconNewFolder.addPixmap        (QPixmap(m_Path +"22x22/folder_new.png"));
  return m_IconNewFolder;
}
QIcon Theme::getIconTextLeft		()
{ if (m_IconTextLeft.isNull() )
  m_IconTextLeft.addPixmap      (QPixmap(m_Path +"22x22/textleft.png"));
  return m_IconTextLeft;
}

QIcon Theme::getIconTextRight	()
{ if (m_IconTextRight.isNull() )
  m_IconTextRight.addPixmap     (QPixmap(m_Path +"22x22/textright.png") );
  return m_IconTextRight;
}

QIcon Theme::getIconTextCenter	()
{ if (m_IconTextCenter.isNull() )
  m_IconTextCenter.addPixmap    (QPixmap(m_Path +"22x22/textcenter.png") );
  return m_IconTextCenter;
}

QIcon Theme::getIconTextJustify	()
{ if (m_IconTextJustify.isNull() )
  m_IconTextJustify.addPixmap   (QPixmap(m_Path +"22x22/textjustify.png"));
  return m_IconTextJustify;
}

QIcon Theme::getIconTextBold		()
{ if (m_IconTextBold.isNull() )
  m_IconTextBold.addPixmap      (QPixmap(m_Path +"22x22/textbold.png") );
  return m_IconTextBold;
}

QIcon Theme::getIconTextItalic	()
{ if (m_IconTextItalic.isNull() )
  m_IconTextItalic.addPixmap    (QPixmap(m_Path +"22x22/textitalic.png"));
  return m_IconTextItalic;
}

QIcon Theme::getIconTextUnderline	()
{ if (m_IconTextUnderline.isNull() )
  m_IconTextUnderline.addPixmap (QPixmap(m_Path +"22x22/textunderline.png"));
  return m_IconTextUnderline;
}

QIcon Theme::getIconUndo		()
{ if (m_IconTextUndo.isNull() )
  m_IconTextUndo.addPixmap      (QPixmap(m_Path +"22x22/redo.png") );
  return m_IconTextUndo;
}


QIcon Theme::getIconRedo		()
{ if (m_IconTextRedo.isNull() )
  m_IconTextRedo.addPixmap      (QPixmap(m_Path +"22x22/undo.png") );
  return m_IconTextRedo;
}

QIcon Theme::getIconCut		()
{ if (m_IconTextCut.isNull() )
  m_IconTextCut.addPixmap       (QPixmap(m_Path +"22x22/editcut.png"));
  return m_IconTextCut;
}

QIcon Theme::getIconCopy		()
{ if (m_IconTextCopy.isNull() )
  m_IconTextCopy.addPixmap      (QPixmap(m_Path +"22x22/editcopy.png") );
  return m_IconTextCopy;
}

QIcon Theme::getIconPaste		()
{ if (m_IconTextPaste.isNull() )
  m_IconTextPaste.addPixmap     (QPixmap(m_Path +"22x22/editpaste.png") );
  return m_IconTextPaste;
}

QIcon Theme::getIconSelectAll	()
{ if (m_IconTextSelectAll.isNull() )
  m_IconTextSelectAll.addPixmap (QPixmap(m_Path +"22x22/edit-select-all.png"));
  return m_IconTextSelectAll;
}


QIcon Theme::getIconListLocked		()
{ if (m_IconListLocked.isNull() )
   m_IconListLocked.addPixmap        (QPixmap(m_Path + "16x16/locked.png") );
  return m_IconListLocked;
}

QIcon Theme::getIconListOK		()
{ if (m_IconListOK.isNull() )
   m_IconListOK.addPixmap        (QPixmap(m_Path + "16x16/listok.png"));
  return m_IconListOK;
}

QIcon Theme::getIconForbidden	()
{ if (m_IconNewDoc.isNull() )
   m_IconNewDoc.addPixmap        (QPixmap(m_Path + "22x22/locked.png") );
  return m_IconNewDoc;
}

QIcon Theme::getIconDownArrow	()
{ if (m_IconDownArrow.isNull() )
  m_IconDownArrow.addPixmap (QPixmap(m_Path + "22x22/1downarrow.png"));
  return m_IconDownArrow;
}

QIcon Theme::getIconRightArrow	()
{if (m_IconRightArrow.isNull() )
  m_IconRightArrow.addPixmap (QPixmap(m_Path + "22x22/1rightarrow.png"));
  return m_IconRightArrow;
}


QIcon Theme::getIconNext	()
{if (m_IconNext.isNull() )
  m_IconNext.addPixmap (QPixmap(m_Path + "22x22/next.png"));
  return m_IconNext;
}
QIcon Theme::getIconPrevious	()
{if (m_IconPrevious.isNull() )
  m_IconPrevious.addPixmap (QPixmap(m_Path + "22x22/previous.png"));
  return m_IconPrevious;
}


QIcon Theme::getIconListAdd	()
{if (m_IconListAdd.isNull() )
  m_IconListAdd.addPixmap (QPixmap(m_Path + "16x16/add.png"));
  return m_IconListAdd;
}

QIcon Theme::getIconListView		()
{if (m_IconListView.isNull() )
  m_IconListView.addPixmap (QPixmap(m_Path + "16x16/info.png"));
  return m_IconListView;
}
QIcon  Theme::getIconListTypeIdentite ()
{if (m_PixmapIdentite.isNull() )
     m_PixmapIdentite.addPixmap(QPixmap(m_Path + "16x16/identity.png"));
 return m_PixmapIdentite;
}

QIcon Theme::getIconListSave	()
{if (m_IconListSave.isNull() )
  m_IconListSave.addPixmap (QPixmap(m_Path + "16x16/save.png"));
  return m_IconListSave;
}

QIcon Theme::getIconListDelete	()
{if (m_IconListDelete.isNull() )
  m_IconListDelete.addPixmap (QPixmap(m_Path + "16x16/remove.png"));
  return m_IconListDelete;
}

QIcon Theme::getIconListDateTime()
{if (m_IconListDateTime.isNull() )
  m_IconListDateTime.addPixmap (QPixmap(m_Path + "16x16/datetime.png"));
  return m_IconListDateTime;
}

QIcon Theme::getIconListFolder	()
{if (m_IconListFolder.isNull() )
  m_IconListFolder.addPixmap (QPixmap(m_Path + "16x16/folder.png"));
  return m_IconListFolder;
}

QIcon Theme::getIconListHTML	()
{if (m_IconListHTML.isNull() )
  m_IconListHTML.addPixmap (QPixmap(m_Path + "16x16/html.png"));
  return m_IconListHTML;
}

QIcon Theme::getIconListItem	()
{if (m_IconListItem.isNull() )
  m_IconListItem.addPixmap (QPixmap(m_Path + "16x16/item.png"));
  return m_IconListItem;
}

QIcon Theme::getIconListEdit	()
{if (m_IconListEdit.isNull() )
  m_IconListEdit.addPixmap (QPixmap(m_Path + "16x16/edit.png") );
  return m_IconListEdit;
}

QIcon Theme::getIconListWarning	()
{if (m_IconListWarning.isNull() )
  m_IconListWarning.addPixmap (QPixmap(m_Path + "16x16/warning.png") );
  return m_IconListWarning;
}

QIcon Theme::getIconListPriority	()
{if (m_IconListPriority.isNull() )
  m_IconListPriority.addPixmap (QPixmap(m_Path + "16x16/priority.png") );
  return m_IconListPriority;
}

QIcon Theme::getIconListTypeDocument	()
{if (m_IconListTypeDocument.isNull() )
  m_IconListTypeDocument.addPixmap (QPixmap(m_Path + "16x16/type_document.png") );
  return m_IconListTypeDocument;
}
QIcon Theme::getIconListTypeObservation	()
{if (m_IconListTypeObservation.isNull() )
  m_IconListTypeObservation.addPixmap (QPixmap(m_Path+"16x16/type_observation.png") );
  return m_IconListTypeObservation;
}
QIcon Theme::getIconListTypePrescription	()
{if (m_IconListTypePrescription.isNull() )
  m_IconListTypePrescription.addPixmap (QPixmap(m_Path + "16x16/type_prescription.png") );
  return m_IconListTypePrescription;
}
QIcon Theme::getIconListTypeCertificat	()
{if (m_IconListTypeCertificat.isNull() )
  m_IconListTypeCertificat.addPixmap (QPixmap(m_Path + "16x16/type_certificat.png") );
  return m_IconListTypeCertificat;
}
QIcon Theme::getIconListTypeTerrain	()
{if (m_IconListTypeTerrain.isNull() )
  m_IconListTypeTerrain.addPixmap (QPixmap(m_Path + "16x16/type_terrain.png") );
  return m_IconListTypeTerrain;
}



QIcon Theme::getIconForMyEditText(const char* icone)
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

QIcon Theme::getIconSet(const QString iconeFile)
{return QIcon(getIcon(iconeFile));
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
  if (CGestIni::Param_ReadParam(m_ThemeIni.toLatin1(), section, var, &r, &g, &b) != QString::null)
  { return color; }

  if ((r!="") && (g!="") && (b!=""))
  { color = QColor(r.toInt(), g.toInt(), b.toInt() );
    finded = true;
    return color; // valeur du ini
  }
  else
  { finded = true;
    return color; // valeur par défaut
  }
}

int Theme::getFromIniFile(const char* section, const char* var, bool& finded, int& value)
{ // gagner du temps process
  if (finded) return value;
  if (m_ThemeIni=="") return value;
  // Récupère la couleur dans le Ini
  QString val = "";
  if (CGestIni::Param_ReadParam(m_ThemeIni.toLatin1(), section, var, &val) != QString::null)
  { return value; }

  if (val!="")
  { value = val.toInt();
    finded = true;
    return value; // valeur du ini
  }
  else
  { finded = true;
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
{ m_FontSize_Popups_Defined = m_FontSize_Menus_Defined = true;
  m_FontSize_Popups         = m_FontSize_Menus         = fntSize;
}

int Theme::getFontSize_Menus()
{  return getFromIniFile("FontSize", "Menus",
                 m_FontSize_Menus_Defined, m_FontSize_Menus);
}

/*! \brief Renvoie le répertoire de thème en cours. */
QString  Theme::getPath(bool withSeparator)
{if (withSeparator==false)
    {if (m_Path.endsWith("/")) return m_Path.left(m_Path.length()-1);
    }
 return m_Path;
}

/*! \brief Définit le répertoire de thème à utiliser. Fonction OBLIGATOIRE avant toute autre pour définir le répertoire sinon risque de plantage. */
void Theme::setPath(const QString p)
{ m_Path = p;
  QString pathIni = p + "/Theme.ini";
  pathIni = QDir:: cleanPath(pathIni);
  // Récupère au passage le fichier ini du theme qui contient les codes couleur
  if (!QFile(pathIni).exists())
  { createDefaultIniFile();
  }
  CGestIni::Param_UpdateFromDisk(pathIni, m_ThemeIni);
}

/*! \brief Crée le fichier de toute pièce pour le thème par défaut. */
void Theme::createDefaultIniFile()
{ QString pathIni = m_Path + "/Theme.ini";
  pathIni = QDir:: cleanPath(pathIni);
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

/********************************* Themeable.h ****************************
 *                                                                                *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat à l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Fédération, 75752 PARIS cedex 15.   *
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

// C++ Interface: Theme.h
//
// Description:
//
//
// Author: Eric MAEKER <eric.maeker@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef Theme_H
#define Theme_H

#include <qstring.h>
#include <qpainter.h>
#include <qcolor.h>
#include <qpixmap.h>
#include <qiconset.h>
#include <qdir.h>

#include "CGestIni.h"

class Theme
{

public:
//     Theme ();
//    ~Theme() { }
        enum m_Mode {WithNoSeparator = 0,
                     WithSeparator   = 1
                    };
        static QString         getPath(bool withSeparator = Theme::WithSeparator);
        static void            setPath(const QString p);

	////////////////////////////////////////////
	///////////////  METHODES  /////////////////
	////////////////////////////////////////////
        static QIconSet	getIconIsDeleted	();
	static QIconSet	getIconDelete		();
	static QIconSet	getIconChangeDate	();
	static QIconSet	getIconCadenas		();
	static QIconSet	getIconSave		();
	static QIconSet	getIconPrint		();
	static QIconSet	getIconTherapeutique	();
	static QIconSet	getIconNewDoc		();
	static QIconSet	getIconNewFolder	();

	// ICONES POUR L'EDITION
	static QIconSet	getIconTextLeft		();
	static QIconSet	getIconTextRight	();
	static QIconSet	getIconTextCenter	();
	static QIconSet	getIconTextJustify	();
	static QIconSet	getIconTextBold		();
	static QIconSet	getIconTextItalic	();
	static QIconSet	getIconTextUnderline	();
	static QIconSet	getIconUndo		();
	static QIconSet	getIconRedo		();
	static QIconSet	getIconCut		();
	static QIconSet	getIconCopy		();
	static QIconSet	getIconPaste		();
	static QIconSet	getIconSelectAll	();

	// ICONES DE NAVIGATION
	static QIconSet	getIconNext		();
	static QIconSet	getIconPrevious		();


	// ICONES D'ALERTE
	static QIconSet	getIconForbidden	();

	// ICONES THERAPEUTIQUES
	static QIconSet	getIconVidal		();
	static QIconSet	getIconMedicaments	();

	static QIconSet	getIconDownArrow	();
	static QIconSet	getIconRightArrow	();

	// ICONES MINIATURES POUR LES LISTES ET LES POPUPS
	static QIconSet	getIconListAdd		();
	static QIconSet	getIconListView		();
	static QIconSet	getIconListSave		();
	static QIconSet	getIconListDelete	();
	static QIconSet	getIconListDateTime	();
	static QIconSet	getIconListFolder	();
	static QIconSet	getIconListHTML		();
	static QIconSet	getIconListItem		();
	static QIconSet	getIconListLocked	();
	static QIconSet	getIconListEdit		();
	static QIconSet	getIconListWarning	();
	static QIconSet	getIconListPriority	();
	static QIconSet	getIconListOK		();

	static QIconSet	getIconListTypeObservation	();
	static QIconSet	getIconListTypePrescription	();
	static QIconSet	getIconListTypeCertificat	();
	static QIconSet	getIconListTypeDocument		();
        static QIconSet getIconListTypeIdentite         ();
	static QIconSet getIconListTypeTerrain		();

	static QIconSet	getIconForMyEditText	(const char* item);

        static QIconSet        getIconSet(const QString iconeFile);
        static QPixmap         getIcon(const QString iconeFile);

        // Couleurs du fichier Ini
        static void   createDefaultIniFile();
        static QColor getFromIniFile(const char* section, const char* var, bool& finded, QColor& color);
        static int    getFromIniFile(const char* section, const char* var, bool& finded, int& value);

        static QColor getRapidAccess_LighterBackground();
        static QColor getRapidAccess_DarkerBackground();
        static QColor getRapidAccess_HighLightColor();
        static QColor getRapidAccess_FontColor_Ident();
        static QColor getRapidAccess_FontColor_Terrain();
        static QColor getRapidAccess_FontColor_Obs();
        static QColor getRapidAccess_FontColor_Prescr();
        static QColor getRapidAccess_FontColor_Doc();

        static void   setFontSize_Menu(int fntSize);
        static int    getFontSize_Popups();
        static int    getFontSize_Menus();


	////////////////////////////////////////////
	///////////////   DONNES   /////////////////
	////////////////////////////////////////////
public:
    static QString        m_Path;	/*!< Contient le Répertoire dans lequel se trouve le Theme */
    static QString        m_ThemeIni;	/*!< Contient le fichier Theme.ini qui se trouve dans ce même répertoire */
    static int            m_FontSize_Popups;
    static int            m_FontSize_Menus;

    static QIconSet m_PixmapIdentite;
    static QIconSet
		m_IconIsDeleted,
		m_IconDelete,
		m_IconChangeDate,
		m_IconCadenas,
		m_IconSave,
		m_IconPrint,
		m_IconTherapeutique,
		m_IconNewDoc,
		m_IconNewFolder,
		m_IconTextLeft,
		m_IconTextRight,
		m_IconTextCenter,
		m_IconTextJustify,
		m_IconTextBold,
		m_IconTextItalic,
		m_IconTextUnderline,
		m_IconTextUndo,
		m_IconTextRedo,
		m_IconTextCut,
		m_IconTextCopy,
		m_IconTextPaste,
		m_IconTextSelectAll,
		m_IconNext,
		m_IconPrevious;

    // ICONES POUR LES POPUPS ET LISTVIEWS
    static QIconSet	m_IconListLocked,
		m_IconListEdit,
		m_IconListView,
		m_IconForbidden,
		m_IconDownArrow,
		m_IconRightArrow,
		m_IconListAdd,
		m_IconListSave,
		m_IconListDelete,
		m_IconListDateTime,
		m_IconListFolder,
		m_IconListHTML,
		m_IconListItem,
		m_IconListWarning,
		m_IconListPriority,
		m_IconListOK,
		m_IconListTypeDocument,
		m_IconListTypeObservation,
		m_IconListTypePrescription,
		m_IconListTypeCertificat,
		m_IconListTypeTerrain;
//     QIconSet       m_IconListSave;
//     QIconSet       m_IconList;

//     QIconSet       m_IconHomme;
//     QIconSet       m_IconFemme;
//     QIconSet       m_IconParturiente;
//     QIconSet       m_IconUser;

//      QIconSet       m_IconList;


    static QIconSet
	m_IconVidal,
	m_IconMedicaments;

   static QColor  m_RapidAccess_LighterBackground,
                  m_RapidAccess_DarkerBackground,
                  m_RapidAccess_HighLightColor,
                  m_RapidAccess_FontColor_Ident,
                  m_RapidAccess_FontColor_Terrain,
                  m_RapidAccess_FontColor_Obs,
                  m_RapidAccess_FontColor_Prescr,
                  m_RapidAccess_FontColor_Doc;

   static bool    m_RapidAccess_LighterBackground_Defined,
                  m_RapidAccess_DarkerBackground_Defined,
                  m_RapidAccess_HighLightColor_Defined,
                  m_RapidAccess_FontColor_Ident_Defined,
                  m_RapidAccess_FontColor_Terrain_Defined,
                  m_RapidAccess_FontColor_Obsr_Defined,
                  m_RapidAccess_FontColor_Prescr_Defined,
                  m_RapidAccess_FontColor_Doc_Defined,
                  m_FontSize_Popups_Defined,
                  m_FontSize_Menus_Defined;
};

#endif

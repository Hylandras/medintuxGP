 /********************************* ThemePopup.h **********************************
 *  #include "ThemePopup.h"                                                       *
 * ...............................................................................*
 *   Project   :  MedinTux  (typographie made in france)                          *
 *   Copyright : (C) 2004-2005-2006-2007-2008-2009-2010 and for the eternity      *
 *   by        :  Sevin Roland     from   MedinTux project                        *
 *   E-Mail    : roland-sevin@medintux.org                                        *
 *   Web site  : www.medintux.org                                                 *
 * ...............................................................................*
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
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
#ifndef THEMEPOPUP_H
#define THEMEPOPUP_H

#include <QMenu>
#include <qobject.h>
#include <QStringList>
#include <qfont.h>
#include "Theme.h"

/** \class ThemePopup
 *  \brief Reimplementation du constructeur de QPopupMenu pour permettre la mise en place du theme de popup.
 *  Cette sera utile lors du passe e Qt4 car si toutes les applis utilisent ThemePopup a la place de QPopupMenu , une seule classe sera a reecrire.

        @author Eric MAEKER <eric.maeker@free.fr>
        @author Roland Sevin <roland-sevin@medintux.org>
*/
class ThemePopup : public QMenu
{
    Q_OBJECT

public:
    ThemePopup(QWidget * parent = 0, const QString & style="");
    ThemePopup(QStringList &list, QWidget * parent = 0, const QString & style="", const QString& sep = "|", int toMask = 0);
    static  void addOptionMenu(QMenu* pQMenu, const QString &optionsTitle);
    QString DoPopupList(const QString & style="");
    static  QIcon    getIconFromOption(QString & optionText);
    static  QString  getIconIdentifiantOption(QString & optionText);
    //------------------------------------------- directoryToPopup ----------------------------------------
    /*! \brief Creer l'arborescence d'un repertoire et l'ajoute a un menu.
     *  Chaque option de fichier a pour nom d'objet : "$file_" + Option_prefix + "=" + chemin avec le nom du fichier \
     *  ce qui permet en interrogeant QAction->objectName() de retrouver :
     *  le prefixage entre : '$file_' et '='  \
     *  le nom du fichier de cette option apres le signe '='
     *  le prefixage permet par exemple lors de plusieurs sous menus avec meme arborescence de distinguer lequel a ete appele
     *
     *  \param pPopup :       menu qui recoit l'arborescence
     *  \param start_dir :    chemin a explorer
     *  \param black_list :   liste des motifs de fichiers a ne pas integrer
     *  \param Option_prefix : prefixage du nom de l'option du menu. Chaque option de fichier a pour nom d'objet : "$file_" + Option_prefix + "=" + chemin avec le nom du fichier
     *  \param extensionFilter : chaine de texte precisant les extensions souhaitees comme nom de fichier a metre dans l'arborescence.
     */
    static void directoryToPopup(QMenu* pPopup, QString start_dir, const QString &black_list  = "", const QString &Option_prefix="", const QString &extensionFilter ="txt;htm*;ord;rtf;pdf");
};

#endif

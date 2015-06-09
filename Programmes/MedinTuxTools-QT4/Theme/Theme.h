/**********************************************************************************
 *                                                                                *
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
// bad code (all code) from Initial Author: Eric MAEKER <eric.maeker@free.fr>  is removed

#ifndef Theme_H
#define Theme_H

#include <QString>
#include <QPixmap>
#include <QIcon>
#include <qdir.h>
#include <QMap>
#include <QPixmap>
#include <QPixmapCache>

//================================ Theme ===============================================
/*! \class Theme
 * Cette classe est entierement statique.
 * TRES IMPORTANT : Avant d'acceder aux fonction de recuperation des icones, il faut definir le chemin
 * vers le repertoire de theme par la fonction setPath(). Celui-ci est structure de la sorte :
 * racine : les icones de taille mal definie
 * 16x16 : repertoire pour les icones visualisables dans les Popup et les ListView
 * 22x22 : pour les icones des bouttons
 * 32x32 : pour les listBox des fenetres de parametre.

 *  Cette classe peut etre utilisee par toutes les applications de MedinTux.

 *  Pour accelerer l'utilisation des icones elles sont chargees au moment de
 *  leur premiere demande et mises en cache
*/


class Theme
{

public:
//     Theme ();
//    ~Theme() { }
        enum m_Mode { WithNoSeparator = 0,
                      WithSeparator   = 1
                    };

 ////////////////////////////////////////////
 ///////////////  METHODES  /////////////////
 ////////////////////////////////////////////
//------------------------ getPath ----------------------------------------------
/*! \brief Renvoie le repertoire de theme en cours.
*  \param  bool withSeparator if = Theme::WithSeparator then path ended whith file separator
*  \return QString theme path
*/
 static QString         getPath    ( bool withSeparator = Theme::WithSeparator  );

 //------------------------ setPath ----------------------------------------------
 /*! \brief Initialise le repertoire de theme a utiliser. Fonction OBLIGATOIRE avant toute autre pour definir le repertoire sinon risque de plantage.
 *  \param  const QString path  theme path wich with initialise theme.
 */
 static void            setPath    ( const QString &path                        );

 //------------------------ getIconSet ----------------------------------------------
 /*! \brief Renvoie l'icone specifiee dans le chemin en entree, sous forme de QIcone.
  *  \param  const QString iconeFile : relative icone file to theme path exp : "/16x16/openfile.png"
  *  \return QIcon of icone
  */
 static QIcon           getIconSet ( const QString &iconeFile                   );

 //------------------------ getIcon ----------------------------------------------
 /*! \brief  Renvoie l'icone specifiee dans le chemin en entree sous forme de QPixmap.
  *  \param  const QString iconeFile : relative icone file to theme path exp : "/16x16/openfile.png"
  *  \return QPixmap of icone
  */
 static QPixmap         getIcon    (const QString &iconeFile                   );

 ////////////////////////////////////////////
 ///////////////   DONNES   /////////////////
 ////////////////////////////////////////////
private :
    static QString                 m_Path;       /*!< Contient le repertoire dans lequel se trouve le Theme */
#ifdef USE_MY_PIXMAP_CACHE
    static QMap <QString, QPixmap> m_IconCache;  /*!< cache d'icones sous forme : key = Chemin raccourci relatif du chemin du theme value = pixmap de l'icone */
#endif
};

#endif

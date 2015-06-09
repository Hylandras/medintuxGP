/*********************************************************************************
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

#include "Theme.h"
#include <QFile>
#include <QPixmap>
#include <QDebug>
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

QString                 Theme::m_Path      = "";

#ifdef USE_MY_PIXMAP_CACHE
   QMap <QString, QPixmap> Theme::m_IconCache = QMap<QString, QPixmap>();
#endif


//------------------------ getIconSet ----------------------------------------------
/*! \brief Renvoie l'icone specifiee dans le chemin en entree, sous forme de QIcone.
 *  \param  const QString iconeFile : relative icone file to theme path exp : "/16x16/openfile.png"
 *  \return QIcon of icone
 */
QIcon Theme::getIconSet(const QString &iconeFile)
{ return QIcon( getIcon( iconeFile ) );
}

//------------------------ getIcon ----------------------------------------------
/*! \brief  Renvoie l'icone specifiee dans le chemin en entree sous forme de QPixmap.
 *  \param  const QString iconeFile : relative icone file to theme path exp : "/16x16/openfile.png"
 *  \return QPixmap of icone
 */
QPixmap  Theme::getIcon(const QString &iconeFile)
{
#ifdef USE_MY_PIXMAP_CACHE
  if ( ! m_IconCache.contains(iconeFile) )
     { QString path = m_Path + iconeFile;
       if ( ! QFile::exists(path) ) { qDebug()<<QObject::tr("Icon not found : %1").arg(path); return QPixmap(); }
       QPixmap pm(path);
       m_IconCache[iconeFile] = pm;
       return pm;
     }
  return m_IconCache[iconeFile];
  #else
    QPixmap pixmap;
    if ( ! QPixmapCache::find( iconeFile , &pixmap ) )
       { QString path = m_Path + iconeFile;
         if ( ! QFile::exists(path) ) { qDebug()<<QObject::tr("Icon not found : %1").arg(path); return QPixmap(); }
         pixmap.load(path);
         QPixmapCache::insert(iconeFile, pixmap);
         return pixmap;
       }
    else
       {
        return pixmap;
       }
  #endif
}

//------------------------ getPath ----------------------------------------------
/*! \brief Renvoie le repertoire de theme en cours.
 *  \param  bool withSeparator if = Theme::WithSeparator then path ended whith file separator
 *  \return QString theme path
 */
QString  Theme::getPath(bool withSeparator /*  = Theme::WithSeparator */)
{ if (withSeparator==false)
     {if (m_Path.endsWith("/")) return m_Path.left(m_Path.length()-1);
     }
  return m_Path;
}

//------------------------ setPath ----------------------------------------------
/*! \brief Initialise le repertoire de theme a utiliser. Fonction OBLIGATOIRE avant toute autre pour definir le repertoire sinon risque de plantage.
*  \param  const QString path  theme path wich with initialise theme.
*/
void Theme::setPath(const QString &path)
{ m_Path = path;
}


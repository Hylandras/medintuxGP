/********************************* CApp.h **********************************
 *  #include "CApp.h"                                                      *
 * ........................................................................*
 *   Project   :  MedinTux  (typographie made in france)                   *
 *   Copyright : (C) 2005-2006-2007-2008-2009-2010 and for the eternity    *
 *   by        :  Sevin Roland     from   MedinTux project                 *
 *   E-Mail    : roland-sevin@medintux.org                                 *
 *   Web site  : www.medintux.org                                          *
 * ........................................................................*
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef C_APP_H
#define C_APP_H

 #include <QObject>
 #include <QProcess>

#if QT_VERSION >= 0x040890    // Qt 4.1.2, the QT_VERSION macro will expand to 0x040102.
    #include <QtWidgets/QApplication>
#else
    #include <QApplication>
#endif

#ifdef Q_WS_WIN
   #define F_EXE ".exe"
#else
   #define F_EXE ""
#endif

#define TR QObject::tr

//================================================= CApp ==============================================================
/*! \class CApp
 *  \brief Main class derived from QApplication.
 *  - this Class must be instancied in main.cpp in first position
 *    when this class is instancied a global pointer  CApp* G_pCApp will be created.
 *    so all publics functions will be accessibles by this pointer.
 */

class CApp : public QApplication
{
   Q_OBJECT
 public:

     CApp(QString mui_name, int & argc, char ** argv);
    ~CApp();
    QString getPathAppli(){return m_PathAppli;}
public:
//................... les objets suivants sont relatifs au contexte de l'application..............................................
QString              m_NameAppli;           /*!< Nom de l'applicatif */
QString              m_PathAppli;           /*!< chemin du repertoire de l'applicatif (termine par /) */
QString              m_PathChangements;     /*!< chemin du fichier changements */
QString              m_ModuleName;          /*!< nom du module pour lequel afficher l'info */
QString              m_Description;         /*!< description courte du module pour lequel afficher l'info */
QString              m_Version;             /*!< numero de version du module pour lequel afficher l'info */
QString              m_PathApropos;         /*!< chemin de la page html de l'onglet A Propos */
QString              m_PathIcone;           /*!< chemin de l'icone du module a afficher */
QString              m_PathHelp;            /*!< chemin de la page html de l'onglet d'aide */
QString              m_PathTheme;           /*!< chemin du repertoire de themes */
QString              m_BaseVersion;         /*!< numero de version de la base de donnees a afficher */
};

extern CApp* G_pCApp;                                 /*!< pointeur global sur l'application */

#endif


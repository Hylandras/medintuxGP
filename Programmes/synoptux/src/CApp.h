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
#include <QtGui/QApplication>
#include "../../MedinTuxTools-QT4/C_AppCore.h"

#define C_APP   CApp::pCApp()

class C_BaseSynoptux;

//================================================= CApp ==============================================================
/*! \class CApp
 *  \brief Main class derived from QApplication.
 *  - this Class must be instancied in main.cpp in first position
 *    when this class is instancied a global pointer  CApp* G_pCApp will be created.
 *    if fatal error in class initialisation occured  CApp::pCApp()==0
 *    so all publics functions will be accessibles by CApp::pCApp()->publicFonctions().
 */


class CApp : public C_AppCore
{
   Q_OBJECT
public:
     CApp(QString mui_name, int & argc, char ** argv);
    ~CApp();

    static CApp         *pCApp();                                /*!< retourne l'instance de l'application          */
    C_BaseSynoptux      *getDB(){return m_pC_BaseSynoptux;}      /*!< retourne l'instance de la base                */
    QString              loadContentsIfBeginByTokenFile(QString &arg);
    QString              resolvePath(QString pathFile, const QString &pathGlossaire  = "");
    QString              resolvePathKeyWord(QString pathFile, const QString &pathGlossaire  = "" );
    bool                 isEndPageDisplay(){return m_EndPageDisplay;}
    //long                 waitForEndPageDisplay(long timeOut, QWebView *pQWebView=0);
    void                 setEndPageDisplay(bool state){m_EndPageDisplay = state;}

    //------------------------------------ outMessage -------------------------------
    /*! \brief output a message on std error out put and logwidget if defined
     *  \param mess  const QString  message to display
     *  \param file  const QString source file from where the message is display
     *  \param line  const QString source line in file from where the message is display
     *  \return message diplayed
     */
    void                 outMessage( const QString &mess, const QString &file   = ""  , const int line   = -1 );

private:
    //................... les objets suivants sont relatifs au contexte de l'application..............................................
    C_BaseSynoptux      *m_pC_BaseSynoptux;       /*!< pointeur sur le moteur de base de donnees */
    bool                 m_EndPageDisplay;

};
#endif

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
 #include <QSqlDatabase>
 #include <QSettings>
 #include <QMessageBox>

 #include "../../MedinTuxTools-QT4/C_AppCore.h"

//================================================= CApp ==============================================================
/*! \class CApp
 *  \brief Main class derived from QApplication.
 *  - this Class must be instancied in main.cpp in first position
 *    when this class is instancied a global pointer  CApp* G_pCApp will be created.
 *    so all publics functions will be accessibles by this pointer.
 */

class CApp : public C_AppCore
{
   Q_OBJECT
 public:

     CApp(QString mui_name, int & argc, char ** argv);
    ~CApp();
    //--------------------------------------------- pCApp -------------------------------------------------------------------
    /*! \brief retourne l'instance de l'application. est a zero tant que l'initialisation n'est pas finie.
     * l'acces ultérieur aux fonctions de la classe CApp se fait par CApp::pCApp()
     */

    QSqlDatabase   database();
    bool           createConnection();
    bool           hideInfoWidget() {return m_hideInfoWidget;}
    QString        result_point()   {return m_result_point;}
    QString        result_family()  {return m_result_family;}
    int            convertVersion() {return m_convertVersion;}
    QString        convertToHtml(const QString &_text);
    static CApp* pCApp();

private:
    QString m_BaseLabel;
    QString m_result_point;
    QString m_result_family;
    int     m_convertVersion;
    bool    m_hideInfoWidget;
};

extern CApp* G_pCApp;                                 /*!< pointeur global sur l'application */

#endif



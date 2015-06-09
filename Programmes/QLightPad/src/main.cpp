/************************** main.cpp ***************************************
 *   Copyright (C) 2004 by Sevin Roland                                    *
 *   upsis@club-internet.fr                                                *
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
#include <qapplication.h>
#include <qstring.h>
#include <qdir.h>
#include <qpixmap.h>
#include <qtextcodec.h> 
#include <qobject.h> 
#include <qmessagebox.h>
#include "QLightPad.h"
#include "../../MedinTuxTools/CGestIni.h"
//=============================================== VARIABLES GLOBALES FONDAMENTALES =====================================================
//                            à instanciation UNIQUE,  Pour les rendre accessibles de partout
//........................... Chemins Frequements utilisés ............................................................................
 QString       GlobalPathAppli              = "";                // chemin de l'Applicatif


int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
    //............................... determiner le chemin de l'application ........................................
    GlobalPathAppli    =  CGestIni::Construct_PathBin_Module("QLightPad",  QFileInfo(argv[0]).dirPath (true));  
    QLightPad     * mw = new QLightPad(0,"QLightPad_FRM" , argc, argv);
    mw->setCaption( "QLightPad by and for Med'in Tux" );
    mw->resize( 640, 700 );
    mw->show();
    mw->setModifiedAll(FALSE);
    QString fname = GlobalPathAppli + "QLightPadSys/QLightPadIcn.png";
    mw->setIcon( QPixmap (fname , QPixmap::imageFormat (fname ), Qt::AutoColor ) );
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}


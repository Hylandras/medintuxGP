/***************************************************************************
 *   Copyright (C) 2003 by Sevin Roland                                    *
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
#include <qobject.h>
#include <qapplication.h>
#include <qdir.h>
#include <qmessagebox.h>
#include <qfileinfo.h>
#include <qtextcodec.h>

#include "CApp.h"
#include "med_stat.h"


//-------------------------------------------------------------------------- main -------------------------------------------------------------------------------------
int main( int argc, char ** argv )
{
    //......................... initialisation classique Application......................................
    CApp a("MedinTux med_stat", argc, argv);
    if (G_pCApp==0) return 0;


    //......................... initialisation classique widgets......................................
    med_stat * mw = new med_stat();
    if (mw==0)
       {QMessageBox::critical (0, "MedinTux med_stat" ,
                                  QObject::tr("MedinTux med_stat ne peut s'instancier \r\n probablement mémoire insuffisante"),
                                  QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
        return 0;
       }

    //....................... on continue avec mot de passe ...................................
    mw->show();
    mw->m_CMDI_Identification->showMaximized();
    mw->DemarrerAccessoires();
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    a.connect( &a, SIGNAL(messageReceived(const QString&)), mw, SLOT(SlotMakeVisible(const QString&)));
    a.exec();

    return 0;
}


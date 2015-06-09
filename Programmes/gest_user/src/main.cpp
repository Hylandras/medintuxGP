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
#include <qapplication.h>
#include <qtextcodec.h>
#include <qobject.h>
#include <qmessagebox.h>

#include "CApp.h"
#include "gest_user.h"
#include "../../MedinTuxTools/CGestIni.h"


// 'se' '#009501F1'
//......................................... y a rien la ??? ............................
int main( int argc, char ** argv )
{   QString erreur;
    CApp a( argc, argv, erreur );
    if (G_pCApp<=0)
       {if (erreur != QObject::tr("Annuler saisie mot de passe"))
           {QMessageBox::warning(0, QObject::tr("Initialisation de Gest User: "),
                                 QObject::tr("ATTENTION : echec de l'initialisation de CApp \n"
                                   ) + erreur,
                                 QObject::tr("Désolé"),0
                            );
           }
        return -1;
       }

    Gest_User * mw = new Gest_User();
    mw->show();
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    return a.exec();
}


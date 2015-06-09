/***************************** CBigShrinker.h *********************************
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

#ifndef C_BIGSHRINKER_H
#define C_BIGSHRINKER_H

#include <qwidget.h>
#include <qobject.h>
#include <qevent.h> 
#include <qframe.h> 

//====================================== BigShrinker ================================================================
class CBigShrinker : public QFrame 
{
    Q_OBJECT
public:
    //................ BigShrinker ...................................
    CBigShrinker(QWidget* parent, int w, int h) :
	QFrame(parent)
       {setFrameStyle(QFrame::Box|QFrame::Sunken);
	resize(w, h);
       } 
    
    //................. mouseReleaseEvent .............................
    void mouseReleaseEvent(QMouseEvent* e)
    {
	emit clicked(e->x(), e->y());
    }

signals:
    void clicked(int,int);
};

#endif


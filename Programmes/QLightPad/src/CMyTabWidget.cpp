/***************************************************************************
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

#include "CMyTabWidget.h"
#include <qtabbar.h>
#include <qpoint.h>
#include <qrect.h>
#include <qcursor.h>

CMyTab::CMyTab (  )
          : QTab()
          {
          };
CMyTab::CMyTab(const QString & text)
         :QTab( text )
         {

         };
CMyTab::CMyTab(  const QIconSet & icon, const QString & text  )
         :QTab(  icon,  text )
         {
         };

void CMyTab::contextMenuEvent ( QContextMenuEvent * /*p_cmevtTriggered*/ )
{
}

//------------------------------------------------ CMyTabWidget -----------------------------------------------------
CMyTabWidget::CMyTabWidget( QWidget       * parent ,
                           const char     * name ,
                           WFlags           f   )
    :QTabWidget(parent,name,f)
{
 //setCornerWidget ( new QButton ( parent, "TabbCornerButton" ) );

}
//------------------------------------------------ addTab -----------------------------------------------------
void CMyTabWidget::addTab ( QWidget * child, const QString & label )
{CMyTab *wt = new CMyTab(label);
 QTabWidget::addTab (child, wt ); //CMyTab

}

//------------------------------------------------ addTab -----------------------------------------------------
 void CMyTabWidget::addTab ( QWidget * child, const QIconSet & iconset, const QString & label )
{CMyTab *wt = new CMyTab(iconset, label);
 QTabWidget::addTab (child, wt );
}

//------------------------------------------------ addTab -----------------------------------------------------
void CMyTabWidget::addTab ( QWidget * child, QTab * tab )
{
 QTabWidget::addTab ( child,  tab );
}

//------------------------------------------------ contextMenuEvent -----------------------------------------------------
void CMyTabWidget::contextMenuEvent ( QContextMenuEvent * p_cmevtTriggered )
{
 QTabWidget::contextMenuEvent ( p_cmevtTriggered );
}

//------------------------------------------------ event -----------------------------------------------------
bool CMyTabWidget::event ( QEvent * ev )
{//qDebug(" clic ");
 if (ev->type() == QEvent::MouseButtonPress)
    {qDebug(" clic on ");
    }
 if (ev->type() == QEvent::MouseButtonRelease)
    {bool ret = QTabWidget::event(ev);
     QPoint pos = mapFromGlobal ( QCursor::pos() );
     qDebug(" clic off ");
     //QWidget *w = childAt ( pos );
     //if (w )
        {//QWidget *truc = this; //QTabBar
         //qDebug(w->name());
          QTabBar* pQTabBar = tabBar();
          QTab     *nextTab = pQTabBar->selectTab ( pos );
          if (nextTab)
             {//qDebug(nextTab->text());
              //createPopup();
              emit Sig_RigthButton(pQTabBar->indexOf(nextTab->identifier()));
             }
          /*
          for ( int i = 0; i < pQTabBar->count(); ++i )
              {
                 QTab *nextTab = pQTabBar->tabAt( i );
                 QRect rect = nextTab->rect();
                 if (rect.contains ( pos))
                    {qDebug(nextTab->text());
                    }
              }
          */
        }

     return ret;
    }
 return QTabWidget::event(ev);
}




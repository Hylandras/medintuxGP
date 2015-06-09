/************************** CMyTabWidget.h *********************************
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

#ifndef CMyTabWidget_H
#define CMyTabWidget_H
#include <qtabbar.h> 
#include <qtabwidget.h>
#include <qiconset.h> 

//====================================== CMyTab =============================================================
class CMyTab : public QTab
{
 

 public:
   //................... constructeurs et destructeurs....................
   CMyTab (  );
   CMyTab(const QString & text);
   CMyTab(  const QIconSet & icon, const QString & text = QString::null );

   void contextMenuEvent ( QContextMenuEvent * p_cmevtTriggered );
  //...................  DATAS ........................................

};

//====================================== CMyTabWidget =============================================================
class CMyTabWidget : public QTabWidget
{ 
    
 Q_OBJECT

 public:       
   //................... constructeurs et destructeurs....................
		   
   CMyTabWidget( QWidget       * parent    = 0,
                 const char    * name      = 0,
		 WFlags          f         = 0);
   void contextMenuEvent ( QContextMenuEvent * p_cmevtTriggered );
   void addTab ( QWidget * child, const QString & label );
   void addTab ( QWidget * child, const QIconSet & iconset, const QString & label ); 
  void  addTab ( QWidget * child, QTab * tab ) ;
private slots:   
  //...................  methodes ........................................
    virtual bool         event ( QEvent * e );
  //...................  DATAS ........................................
signals:
    void Sig_RigthButton(int);
};



#endif


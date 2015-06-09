/* Copyright (C) 2003 MySQL AB & Jorge del Conde

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.
    
  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free
  Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
  MA 02111-1307, USA 
*/
//=============================================== INCLUDES ==================================================================

#include "CDockWindow.h"
//#include "config.h"
//#include "globals.h"

#ifdef DEBUG_LEVEL
#if DEBUG_LEVEL < 2
#undef DEBUG
#endif
#else
#ifdef DEBUG
#undef DEBUG
#endif
#endif
//=============================================== IMPLEMENTATION =====================================================



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//====================================== CDockWindow =======================================================================

//-----------------------------------------------------  CDockWindow -------------------------------------------------------
CDockWindow::CDockWindow(Place p, QWidget * parent, const char * name, WFlags f)
: QDockWindow(p, parent, name, f)
{
#ifdef DEBUG
  qDebug("CDockWindow::CDockWindow()");
#endif
  
  if (!name)    setName("CDockWindow");
  setMovingEnabled (true);
  setResizeEnabled (true);
  setHorizontallyStretchable (true);
  setVerticallyStretchable (true);
  setOpaqueMoving (false);
  setCloseMode(Always);    
  setNewLine(true);
}  

//-----------------------------------------------------  closeEvent -------------------------------------------------------
void CDockWindow::closeEvent(QCloseEvent *e)
{
#ifdef DEBUG
  qDebug("CDockWindow::closeEvent()");
#endif

  e->ignore();
  hide();
}

//-----------------------------------------------------  setCaption -------------------------------------------------------
void CDockWindow::setCaption(const QString &s)
{
  if (QDockWindow::caption() == s)
    return;

  QDockWindow::setCaption(s);
  
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//====================================== CMessageWindow =======================================================================
CMessageWindow::CMessageWindow(Place p, QWidget * parent, const char * name, WFlags f)
:CDockWindow (p, parent, name, f)
{
#ifdef DEBUG
  qDebug("CMessageWindow::CMessageWindow()");
#endif

  if ( !name )  setName("CMessageWindow");
  setCaption(tr("Message Panel"));
  Tabs = new QTabWidget(this, "Tabs" );
  Tabs->setTabPosition( QTabWidget::Top );
  Tabs->setTabShape( QTabWidget::Triangular );
  connect(Tabs, SIGNAL(currentChanged(QWidget *)), this, SLOT(tab_changed(QWidget *)));
  setWidget(Tabs);
}

//-----------------------------------------------------  setCarriageReturn -------------------------------------------------------
void CMessageWindow::setCarriageReturn(const QString &s)
{
#ifdef DEBUG
  qDebug("CMessageWindow::setCarriageReturn()");
#endif

  
  //for (int i = 0; i < Tabs->count(); i++)
  //  ((CPanel *)Tabs->page(i))->setCarriageReturn(s);
}

//-----------------------------------------------------  addPanel -------------------------------------------------------
void CMessageWindow::addPanel(QWidget *panel)
{
#ifdef DEBUG
  qDebug("CMessageWindow::addPanel()");
#endif

  if (panel != 0)
  {
    panel->reparent(Tabs, 0, QPoint(0,0));
    //if (!cr.isEmpty() && panel->inherits("CPanel"))
    //  ((CPanel *) panel)->setCarriageReturn(cr);
    Tabs->insertTab(panel, panel->caption());
  }
}

//-----------------------------------------------------  tab_changed -------------------------------------------------------
void CMessageWindow::tab_changed(QWidget *w)
{
#ifdef DEBUG
  qDebug("CMessageWindow::tab_changed()");
#endif

 // if (w->inherits("CPanel"))
 //   ((CPanel *) w)->setBottomItem();
}

//-----------------------------------------------------  setTabEnabled -------------------------------------------------------
void CMessageWindow::setTabEnabled(QWidget * w, bool enable)
{
#ifdef DEBUG
  qDebug("CMessageWindow::enablePanel(QWidget, bool)");
#endif

  if (w != 0)    Tabs->setTabEnabled(w, enable);
}

//-----------------------------------------------------  setActivePanel -------------------------------------------------------
void CMessageWindow::setActivePanel(int t)
{
#ifdef DEBUG
  qDebug("CMessageWindow::setActivePanel(int)");
#endif

  if (Tabs->page(t) != 0 && Tabs->currentPageIndex() != t)
    Tabs->setCurrentPage(t);
}

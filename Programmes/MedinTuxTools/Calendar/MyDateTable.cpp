/*  -*- C++ -*-
    This file is part of the KDE libraries
    Copyright (C) 1997 Tim D. Gilman (tdgilman@best.org)
              (C) 1998-2001 Mirko Boehm (mirko@kde.org)
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

//
// Written using Qt (http://www.troll.no) for the
// FROM KDE project (http://www.kde.org)
//
// This is a support class for the KDatePicker class.  It just
// draws the calender table without titles, but could theoretically
// be used as a standalone.
//
// When a date is selected by the user, it emits a signal:
//      dateSelected(QDate)
//
/////////////////// MyDateTable widget class /////////////////////////////////////
// MODIFIED BY SEVIN ROLAND FOR USE WITHOUT KDE ( Sorry for this simplified code )
// Original wonderfull code writed by:
// Copyright (C) 1997 Tim D. Gilman
//           (C) 1998-2001 Mirko Boehm
//


#include "MyDateTable.h"

#include <qstring.h>
#include <qpen.h>
#include <qpainter.h>
#include <qdialog.h>
#include <qdict.h>
#include <assert.h>
#include <qapplication.h>
#include <qdrawutil.h>

//===========================================================================  MyDateTable ==========================================================================


//--------------------------------------------------------- MyDateTable --------------------------------------------------------------------
MyDateTable::MyDateTable(QWidget *parent, QDate date_, const char* name, WFlags f)
  : QGridView(parent, name, f), m_pDateDeb(0)
{ setFontSize(10);
  if (!date_.isValid())
     {qDebug("MyDateTable ctor: WARNING: Given date is invalid, using current date.");
      date_= QDate::currentDate();
     }
  setFocusPolicy( QWidget::StrongFocus );
  setNumRows(7); // 6 weeks max + headline
  setNumCols(7); // 7 days a week
  setHScrollBarMode(AlwaysOff);
  setVScrollBarMode(AlwaysOff);
  viewport()->setEraseColor(QColor ( "white") );
  setDate(date_);                          // this initializes m_firstday, m_numdays, m_numDaysPrevMonth
}

//--------------------------------------------------------- setDateDeb --------------------------------------------------------------------
void MyDateTable::setDateDeb(QDate* pdate)
{m_pDateDeb = pdate;
 repaintContents(false);
}

//--------------------------------------------------------- getDate --------------------------------------------------------------------
QDate MyDateTable::getDate()
{return m_Date;
}

//---------------------------------------------------- ~MyDateTable --------------------------------------------------------------------
MyDateTable::~MyDateTable()
{
}

//--------------------------------------------------------- posFromDate --------------------------------------------------------------------
int MyDateTable::posFromDate( const QDate &dt )
{ const int firstWeekDay            = WEEK_START_DAY;
  int pos                           = dt.day();
  int offset                        = (m_firstday - firstWeekDay + 7) % 7;
  // make sure at least one day of the previous month is visible.
  // adjust this <1 if more days should be forced visible:
  if ( offset < 1 ) offset += 7;
  return pos + offset;
}

//--------------------------------------------------------- dateFromPos --------------------------------------------------------------------
QDate MyDateTable::dateFromPos( int pos )
{ QDate pCellDate;
  setYMD(pCellDate, m_Date.year(), m_Date.month(), 1);
  int firstWeekDay = WEEK_START_DAY;
  int offset       = (m_firstday - firstWeekDay + 7) % 7;
  // make sure at least one day of the previous month is visible.
  // adjust this <1 if more days should be forced visible:
  if ( offset < 1 ) offset += 7;
  return pCellDate.addDays( pos - offset );
}

//--------------------------------------------------------- setYMD --------------------------------------------------------------------------
bool MyDateTable::setYMD(QDate & date, int y, int m, int d) const
{ // We don't want Qt to add 1900 to them
  if ( y >= 0 && y <= 99 )
    return false;
  // QDate supports gregorian internally
  return date.setYMD(y, m, d);
}

//--------------------------------------------------------- setYMD --------------------------------------------------------------------------
bool MyDateTable::setYMD( int y, int m, int d)
{return setYMD(m_Date,  y,  m,  d);
}

//--------------------------------------------------------- weekDayName --------------------------------------------------------------------
QString MyDateTable::weekDayName (int weekDay, bool shortName /*=TRUE */) const
 { if ( shortName )
     switch ( weekDay )
       {
       case 1:  return QObject::tr("Lun");
       case 2:  return QObject::tr("Mar");
       case 3:  return QObject::tr("Mer");
       case 4:  return QObject::tr("Jeu");
       case 5:  return QObject::tr("Ven");
       case 6:  return QObject::tr("Sam");
       case 7:  return QObject::tr("Dim");
       }
   else
     switch ( weekDay )
       {
       case 1:  return QObject::tr("Lundi");
       case 2:  return QObject::tr("Mardi");
       case 3:  return QObject::tr("Mercredi");
       case 4:  return QObject::tr("Jeudi");
       case 5:  return QObject::tr("Vendredi");
       case 6:  return QObject::tr("Samedi");
       case 7:  return QObject::tr("Dimanche");
       }

   return QString::null;
 }

//--------------------------------------------------------- dayString --------------------------------------------------------------------
QString MyDateTable::dayString(const QDate & pDate, bool bShort ) const
 {
   QString sResult;
   sResult.setNum(pDate.day());
   if (!bShort && sResult.length() == 1 )
     sResult.prepend('0');
   return sResult;
 }

//--------------------------------------------------------- paintCell --------------------------------------------------------------------
void MyDateTable::paintCell(QPainter *painter, int row, int col)
{ QRect rect;
  QString text;
  QPen pen;
  int w=cellWidth();
  int h=cellHeight();
  QFont        font = this->font();
  Qt::BrushStyle br = Qt::NoBrush; // Qt::NoBrush   Qt::Dense7Pattern
  // -----

  if(row==0)
    { // we are drawing the headline
      font.setBold(true);
      painter->setFont(font);
      bool normalday   = true;
      int firstWeekDay = WEEK_START_DAY;
      int daynum = ( col+firstWeekDay < 8 ) ? col+firstWeekDay :
                                              col+firstWeekDay-7;
      if ( daynum == 7 ||           // sunday
         ( daynum == 6 ) )
           normalday=false;
          // #define ACTIVE_TITLE_COL     "white"
          // #define INACTIVE_TITLE_COL   "gray"
          // #define ACTIVE_TEXT_COL      "black"
          // #define INACTIVE_TEXT_COL    "darkGray"
                        QBrush brushTitle();
                        QBrush brushInvertTitle(colorGroup().base());
                        QColor titleColor(isEnabled()?(ACTIVE_TITLE_COL):( INACTIVE_TITLE_COL) );
                        QColor textColor(isEnabled()?( ACTIVE_TEXT_COL ):( INACTIVE_TEXT_COL ) );
      if (!normalday)
        {
          //painter->setPen(textColor);
          //painter->setBrush(textColor);
          painter->setPen(QColor("darkBlue"));
          painter->setBrush(QColor("darkBlue"));
          painter->drawRect(0, 0, w, h);
          // painter->setPen(titleColor);
          painter->setPen(QColor("white"));
        } else {
          //painter->setPen(titleColor);
          //painter->setBrush(titleColor);
          painter->setPen(QColor("lightGray"));
          painter->setBrush(QColor("lightGray"));
          painter->drawRect(0, 0, w, h);
          // painter->setPen(textColor);
          painter->setPen(QColor("darkBlue"));
        }
      painter->drawText(0, 0, w, h-1,  AlignCenter,  weekDayName(daynum), -1, &rect);
      //painter->setPen(colorGroup().text());
      painter->setPen(QColor("blue"));
      painter->moveTo(0, h-1);
      painter->lineTo(w-1, h-1);
      painter->setPen(QColor("darkCyan"));
      painter->drawLine (0, h-2, w, h-2 ) ;
      // ----- draw the weekday:
    } else {
      bool paintRect=true;
      int pos=7*(row-1)+col;

      QDate pCellDate = dateFromPos( pos );
      // First day of month
      text = dayString(pCellDate, true);
      if( pCellDate.month() != m_Date.month() )
        { // we are either
          // ° painting a day of the previous month or
          // ° painting a day of the following month
          // TODO: don't hardcode gray here! Use a color with less contrast to the background than normal text.
          painter->setPen( colorGroup().mid() );
          // painter->setPen(gray);
        }
      else
        { // paint a day of the current month
          painter->setPen(colorGroup().text());
        }
      pen=painter->pen();
      int firstWeekDay = WEEK_START_DAY; //KGlobal::locale()->weekStartDay();
      int offset       = m_firstday-firstWeekDay;
      if(offset<1)    offset+=7;
      int d = m_Date.day();
      if( ((offset+d) == (pos+1)) && hasFocus())
        {
           // draw the currently selected date
           painter->setPen(colorGroup().highlight());
           painter->setBrush(colorGroup().highlight());
           pen=colorGroup().highlightedText();
        } else {
          painter->setBrush(paletteBackgroundColor());
          painter->setPen(paletteBackgroundColor());
          //          painter->setBrush(colorGroup().base());
          //          painter->setPen(colorGroup().base());
        }

      if ( pCellDate == QDate::currentDate() )
         {// painter->setPen(colorGroup().text());
          font.setBold(true);
          painter->setPen(QColor("red"));
         }
      if ( paintRect )
         {painter->drawRect(0, 0, w, h);
          //painter->drawRect(2, 2, w-4, h-4);
          //painter->setBackgroundMode ( Qt::OpaqueMode );
          //painter->setBackgroundColor ( QColor("white") );
          painter->setPen(QColor("lightGray"));
          painter->drawRect(1, 1, w-2, h-2);
          if (m_pDateDeb )
             {
              QDate infD = *m_pDateDeb;
              QDate supD =  m_Date;
              if (infD>supD)
                 {QDate swpD = infD;
                  infD       = supD;
                  supD       = swpD;
                 }
              if (pCellDate == *m_pDateDeb)
                 {font.setBold(true);
                  //painter->setPen(QColor("green"));
                  //painter->drawRect(3, 3, w-6, h-6);
                  //painter->setPen(QColor("black"));
                  //painter->drawRect(2, 2, w-4, h-4);
                  QBrush brush(br);
                  //QColor color("green")
                  qDrawPlainRect ( painter, 2, 2, w-4, h-4, QColor("green"),1, &brush);
                 }
              else if (pCellDate>=infD && pCellDate<=supD)
                 {font.setBold(true);
                  QBrush brush(br);
                  qDrawPlainRect ( painter, 2, 2, w-4, h-4, QColor("green"),1, &brush);
                  //painter->drawRect(3, 3, w-6, h-6);
                  //painter->setPen(QColor("black"));
                  //painter->drawRect(2, 2, w-4, h-4);
                 }
             }
          if ( pCellDate == m_Date )
             {// painter->setPen(colorGroup().text());
              font.setBold(true);
              painter->setPen(QColor("blue"));
              painter->drawRect(2, 2, w-4, h-4);
              QBrush brush(br);
              qDrawPlainRect ( painter, 3, 3, w-6, h-6, QColor("green"),1, &brush);
             }
         }
      painter->setPen(pen);
      painter->setFont(font);
      painter->drawText(0, 0, w, h, AlignCenter, text, -1, &rect);
    }
  if(rect.width()  > m_maxCell.width())   m_maxCell.setWidth(rect.width());
  if(rect.height() > m_maxCell.height())  m_maxCell.setHeight(rect.height());
}

//--------------------------------------------------------- keyPressEvent --------------------------------------------------------------------
void MyDateTable::keyPressEvent( QKeyEvent *e )
{

    QDate temp = m_Date;

    switch( e->key() ) {
    case Key_Prior:
        temp = m_Date.addMonths( -1 );
        setDate(temp);
        return;
    case Key_Next:
        temp = m_Date.addMonths(  1 );
        setDate(temp);
        return;
    case Key_Up:
        if ( m_Date.day() > 7 ) {
            setDate(m_Date.addDays(-7));
            return;
        }
        break;
    case Key_Down:
        if ( m_Date.day() <= m_Date.daysInMonth()-7 ) {
            setDate(m_Date.addDays(7));
            return;
        }
        break;
    case Key_Left:
        if (m_Date.day() > 1 ) {
            setDate(m_Date.addDays(-1));
            return;
        }
        break;
    case Key_Right:
        if (m_Date.day() < m_Date.daysInMonth() ) {
            setDate(m_Date.addDays(1));
            return;
        }
        break;
    case Key_Minus:
        setDate(m_Date.addDays(-1));
        return;
    case Key_Plus:
        setDate(m_Date.addDays(1));
        return;
    case Key_N:
        setDate(QDate::currentDate());
        return;
    case Key_Return:
    case Key_Enter:
        emit tableClicked();
        return;
    default:
        break;
    }
}

//--------------------------------------------------------- viewportResizeEvent --------------------------------------------------------------------
void MyDateTable::viewportResizeEvent(QResizeEvent * e)
{ QGridView::viewportResizeEvent(e);
  setCellWidth(viewport()->width()/7);
  setCellHeight(viewport()->height()/7);
}

//--------------------------------------------------------- setFontSize --------------------------------------------------------------------
void MyDateTable::setFontSize(int size)
{
  int count;
  QFontMetrics metrics(fontMetrics());
  QRect rect;
  // ----- store rectangles:
  m_fontsize=size;
  // ----- find largest day name:
  m_maxCell.setWidth(0);
  m_maxCell.setHeight(0);
  for(count=0; count<7; ++count)
    { rect=metrics.boundingRect(weekDayName(count+1));
      m_maxCell.setWidth(QMAX(m_maxCell.width(), rect.width()));
      m_maxCell.setHeight(QMAX(m_maxCell.height(), rect.height()));
    }
  // ----- compare with a real wide number and add some space:
  rect=metrics.boundingRect(QString::fromLatin1("88"));
  m_maxCell.setWidth(QMAX(m_maxCell.width()+2, rect.width()));
  m_maxCell.setHeight(QMAX(m_maxCell.height()+4, rect.height()));
}

//--------------------------------------------------------- wheelEvent --------------------------------------------------------------------
void MyDateTable::wheelEvent ( QWheelEvent * e )
{
    setDate(m_Date.addMonths( -(int)(e->delta()/120)) );
    e->accept();
}

//--------------------------------------------------------- contentsMousePressEvent --------------------------------------------------------------------
void MyDateTable::contentsMousePressEvent(QMouseEvent *e)
{

  if( !(e->type()==QEvent::MouseButtonPress || e->type()==QEvent::MouseButtonDblClick))
    { // the KDatePicker only reacts on mouse press events:
      return;
    }
  if(!isEnabled())
    { return;
    }

  // -----
  int row, col, pos, temp;
  QPoint mouseCoord;
  // -----
  mouseCoord = e->pos();
  row=rowAt(mouseCoord.y());
  col=columnAt(mouseCoord.x());
  if(row<1 || col<0)
    { // the user clicked on the frame of the table
      return;
    }

  // Rows and columns are zero indexed.  The (row - 1) below is to avoid counting
  // the row with the days of the week in the calculation.

  // old selected date:
  temp = posFromDate( m_Date );
  // new position and date
  pos = (7 * (row - 1)) + col;
  QDate clickedDate = dateFromPos( pos );

  // set the new date. If it is in the previous or next month, the month will
  // automatically be changed, no need to do that manually...
  setDate( clickedDate );

  // call updateCell on the old and new selection. If setDate switched to a different
  // month, these cells will be painted twice, but that's no problem.
  updateCell( temp/7+1, temp%7 );
  updateCell( row, col );
  if (e->type()==QEvent::MouseButtonDblClick)  emit tableDoubleClicked();
  else                                         emit tableClicked();

  if (  e->button() == Qt::RightButton)
  {
        //KPopupMenu *menu = new KPopupMenu();
        //menu->insertTitle( KGlobal::locale()->formatDate(clickedDate) );
        //emit aboutToShowContextMenu( menu, clickedDate );
        //menu->popup(e->globalPos());
  }
}

//--------------------------------------------------------- setDate --------------------------------------------------------------------
bool MyDateTable::setDate(const QDate& date_)
{
  bool changed=false;
  QDate temp;
  // -----
  if(!date_.isValid())
    { qDebug( "MyDateTable::setDate: refusing to set invalid date." );
      return false;
    }
  if(m_Date!=date_)
    {
      emit(dateChanged(m_Date, date_));
      m_Date=date_;
      emit(dateChanged(m_Date));
      changed=true;
    }
  setYMD(temp, m_Date.year(), m_Date.month(), 1);
  m_firstday         = temp.dayOfWeek();
  m_numdays          = m_Date.daysInMonth();
  temp               = temp.addMonths(-1);
  m_numDaysPrevMonth = temp.daysInMonth();
  if(changed)
    { repaintContents(false);
    }
  return true;
}

//--------------------------------------------------------- focusInEvent --------------------------------------------------------------------
// what are those repaintContents() good for? (pfeiffer)
void MyDateTable::focusInEvent( QFocusEvent *e )
{
//    repaintContents(false);
    QGridView::focusInEvent( e );
}

//--------------------------------------------------------- focusOutEvent --------------------------------------------------------------------
void MyDateTable::focusOutEvent( QFocusEvent *e )
{
//    repaintContents(false);
    QGridView::focusOutEvent( e );
}

//--------------------------------------------------------- sizeHint --------------------------------------------------------------------
QSize MyDateTable::sizeHint() const
{
  if(m_maxCell.height()>0 && m_maxCell.width()>0)
    {
      return QSize(m_maxCell.width()*numCols()+2*frameWidth(),
                   (m_maxCell.height()+2)*numRows()+2*frameWidth());
    } else {
      qDebug( "MyDateTable::sizeHint: obscure failure - ");
      return QSize(-1, -1);
    }
}


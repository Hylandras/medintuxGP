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

 // // MODIFIED BY SEVIN ROLAND FOR USE WITHOUT KDE ( Sorry for this simplified code )

 #ifndef K_SIMPLE_DATETBL_H
 #define K_SIMPLE_DATETBL_H


 #include <qvalidator.h>
 #include <q3gridview.h>
 #include <qlineedit.h>
 #include <qdatetime.h>
 #include <qcolor.h>
//Added by qt3to4:
#include <QResizeEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFocusEvent>

 #define WEEK_START_DAY       1
 #define ACTIVE_TITLE_COL     "white"
 #define INACTIVE_TITLE_COL   "gray"
 #define ACTIVE_TEXT_COL      "black"
 #define INACTIVE_TEXT_COL    "darkGray"

//=========================================== MyDateTable ================================================
 class  MyDateTable : public Q3GridView
 {
     Q_OBJECT
     //Q_PROPERTY( QDate date READ getDate WRITE setDate )
     //Q_PROPERTY( bool popupMenu READ popupMenuEnabled WRITE setPopupMenuEnabled )

 public:
     MyDateTable(QWidget *parent=0, QDate date=QDate::currentDate(), const char* name=0, Qt::WFlags f=0);
     MyDateTable(QWidget *parent, const char* name, Qt::WFlags f=0);
     ~MyDateTable();

     virtual QSize sizeHint() const;
     void          setFontSize(int size);
     bool          setDate(const QDate&);
     bool          setYMD( int y, int m, int d);
     QDate         getDate();
     void          setDateDeb(QDate* pdate);
     enum BackgroundMode { NoBgMode=0, RectangleMode, CircleMode };

 protected:
     int posFromDate( const QDate &date ); // KDE4: make this virtual, so subclasses can reimplement this and use a different default for the start of the matrix
     QDate dateFromPos( int pos );         // KDE4: make this virtual

     virtual void paintCell(QPainter*, int, int);
     virtual void viewportResizeEvent(QResizeEvent *);
     virtual void contentsMousePressEvent(QMouseEvent *);
     virtual void wheelEvent( QWheelEvent * e );
     virtual void keyPressEvent( QKeyEvent *e );
     virtual void focusInEvent( QFocusEvent *e );
     virtual void focusOutEvent( QFocusEvent *e );
     bool         setYMD(QDate & date, int y, int m, int d)        const;    // from calendar KDE
     QString      weekDayName (int weekDay, bool shortName=TRUE)   const;    // from calendar KDE
     QString      dayString(const QDate & pDate, bool bShort)      const;    // from calendar KDE
     int   m_fontsize;
     int   m_firstday;
     int   m_numdays;
     int   m_numDaysPrevMonth;
     QRect m_maxCell;
     QDate *m_pDateDeb;
     QDate m_Date;
 signals:
     void dateChanged(QDate);
     void dateChanged(const QDate& cur, const QDate& old);
     void tableClicked();
     void tableDoubleClicked();
 };
 #endif // K_SIMPLE_DATETBL_H



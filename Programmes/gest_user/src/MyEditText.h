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
#ifndef MYEDITTEXT_H
#define MYEDITTEXT_H

#include <qobject.h>
#include <qwidget.h>
#include <qtextedit.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qpopupmenu.h>
#include <qsqldatabase.h>
#include <qevent.h>
#include <qtextstream.h>

#include "ui/DLG_InsertList.h"

//====================================== MyEditText ===================================================
class MyEditText : public QTextEdit
{

 Q_OBJECT

 public:
    //................... constructeurs et destructeurs....................
    MyEditText(const QString & text,
	       const QString & context   = QString::null,
               QWidget       * parent    = 0,
               const char    * name      = 0)
              :QTextEdit(text, context, parent, name)
            {m_pQPopupMenu     = 0;
	     setTextFormat(Qt::RichText);
	     setWordWrap(QTextEdit::FixedPixelWidth);
	     setWrapColumnOrWidth (580 );   //( corresponds A4 width )
             //......................... interligne des paragraphes le plus petit possible ..............................
             //                          comme celui des retours chariot
             QStyleSheet      *sheet  = styleSheet() ;
             QStyleSheetItem  *stylep = sheet->item("p");
             stylep->setDisplayMode(QStyleSheetItem::DisplayBlock);
             stylep->setMargin(QStyleSheetItem::MarginVertical, 0);
            }
    MyEditText(QWidget       * parent    = 0,
	       const char    * name      = 0)
              :QTextEdit(parent, name)
            {m_pQPopupMenu     = 0;
	     setTextFormat(Qt::RichText);
	     setWordWrap(QTextEdit::FixedPixelWidth);
	     setWrapColumnOrWidth (580 );   //( corresponds A4 width )
             //......................... interligne des paragraphes le plus petit possible ..............................
             //                          comme celui des retours chariot
             QStyleSheet      *sheet  = styleSheet() ;
             QStyleSheetItem  *stylep = sheet->item("p");
             stylep->setDisplayMode(QStyleSheetItem::DisplayBlock);
             stylep->setMargin(QStyleSheetItem::MarginVertical, 0);

            }

  //...................  methodes ........................................
  bool         event ( QEvent * e );
  QPopupMenu * createPopupMenu ( const QPoint & pos );
  void         moveCursor ( int action, bool select =0);
  void         focusInEvent ( QFocusEvent *pQfocus );
  void         EmuleReturnKeyPress();

public slots:
    void    OnMenuAboutToHide ();
    void    OnMenuActivated (int id);
    void    OnMenuHighlighted (int id);
    int     DoContextListPopup();
    void    ListField();
protected:
  QString     m_PopupItem;
  QStringList m_InsertList;
  QPopupMenu *m_pQPopupMenu;

signals:
  void message(const QString&, int);
  void Sign_EditGotFocus( MyEditText *);
  void Sign_EditFusionFieldRequest();
};

#endif

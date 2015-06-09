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
#include "MyEditText.h"
#include <qapplication.h>

#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qpixmap.h>
#include <qclipboard.h>
#include <qlistview.h>
#include <qcursor.h>
#include <qtimer.h>
#include <qtooltip.h>
#include <qtabwidget.h>
#include <qlistbox.h>

#include "../../MedinTuxTools/ThemePopup.h"

//====================================== MyEditText ==========================================================================
// QKeyEvent( Type type, int key, int ascii, int state,
//            const QString& text=QString::null, bool autorep=FALSE, ushort count=1 )

 //---------------------------------- event -------------------------------------------------------------------
// ACTION: surcharge de la fonction clavier de QTextEdit, afin de corriger l'anomalie de fonctionnement inversé
//        de la touche enter et ctrl+enter.

bool MyEditText::event ( QEvent * ev )
{//if ( ev->type() == QEvent::MouseButtonRelease)
 //   {QMouseEvent *m = (QMouseEvent*)ev;
 //    int buton = m->button ();
 //   }

 if ( ev->type() == QEvent::KeyPress )
    {
     QKeyEvent   *k = (QKeyEvent *)ev;
     int        car = k->ascii();

     //............. inverser le fonctionnement de la TOUCHE ENTER................................
     //              car d'origine: ENTER        --> fin de paragraphe avec double interligne
     //                             ENTER + CTRL --> fin de ligne avec interligne normal
     //              et cela ne me va pas
     if (car==13 || car==10)
        {int      state = k->state();
	 /*...................... exploration des touches ........................................
	 QString   qstr = "";
	 QString    str = "";
	 if (k->text().length()>0)
	    {char buft[32];
	     memcpy(buft, k->text(), k->text().length());
	     str += " \r\ntext: " + qstr.setNum( buft[0] );
	    }
	 else                      str += "\r\ntext: empty";
	 qstr.sprintf("KeyPressed: %d \r\nAscii: %d \r\nState:",k->key(), k->ascii() );

	 switch (state&Qt::KeyButtonMask)
	        {case Qt::NoButton:        qstr+=" - no button  pressed.";                          break;
              	 case Qt::LeftButton:      qstr+=" - set if the left button is pressed.";           break;
	         case Qt::RightButton:     qstr+=" - the right button.";                            break;
                 case Qt::MidButton:       qstr+=" - the middle button.";                           break;
                 case Qt::ShiftButton:     qstr+=" - a Shift key on the keyboard is also pressed."; break;
                 case Qt::ControlButton:   qstr+=" - a Ctrl key on the keyboard is also pressed.";  break;
                 case Qt::AltButton:       qstr+=" - an Alt key on the keyboard is also pressed.";  break;
                 case Qt::MetaButton:      qstr+=" - a Meta key on the keyboard is also pressed.";  break;
                 case Qt::Keypad:          qstr+=" - a keypad button is pressed.";                  break;
                 case Qt::KeyButtonMask:   qstr+=" - a mask for ShiftButton, ControlButton, AltButton and MetaButton.";break;
                 case Qt::MouseButtonMask: qstr+=" - a mask for LeftButton, RightButton and MidButton.";break;
                }
	 qstr += str;
	 qstr += "\r\nNum K State: " + str.setNum(state,16);
	 setText(qstr);
	 */
	 if (state!=Qt::ControlButton) state = Qt::ControlButton;
	 else                          state = Qt::NoButton;
	 QKeyEvent keyEvent (ev->type(), k->key(), car, state,
			     k->text(),  k->isAutoRepeat(), k->count() );
	 if ( k->isAccepted ()) keyEvent.accept();
	 else                   keyEvent.ignore();
	 return QTextEdit::event (&keyEvent);
	}   //endif (car==13)
    }  //endif ( ev->type() == QEvent::KeyPress )
 return QTextEdit::event (ev);
}

//----------------------------------------- focusInEvent -----------------------------------------------------------------
void MyEditText::focusInEvent ( QFocusEvent *pQfocus )
{if (pQfocus->gotFocus()== TRUE) emit Sign_EditGotFocus(this);
}

//----------------------------------------- moveCursor -----------------------------------------------------------------
// ACTION: permet à notre classe d'acceder à la methode protégée: QTextEdit::moveCursor( (CursorAction)action,  select);

void MyEditText::moveCursor (int action, bool select )
{QTextEdit::moveCursor( (CursorAction)action,  select);
}

//----------------------------------------- createPopupMenu -----------------------------------------------------------------
// ACTION: recherche lors de l'appuis du bouton droit si il y a une zone de liste (bleue #0000ff)
//         et auquel cas rajoute les items de cette liste au popup menu classique.

QPopupMenu * MyEditText::createPopupMenu ( const QPoint &  )
{m_PopupItem="";
 QPopupMenu *pQPopupMenu = new QPopupMenu(0, "MyPopupMenu" );
 if (pQPopupMenu==0)    return 0;

 //............................... créer le Popup classique .........................................
 pQPopupMenu->insertItem( Theme::getIconUndo(),      ("&Undo"),       this, SLOT( undo()),   CTRL+Key_Z );
 pQPopupMenu->insertItem( Theme::getIconRedo(),      ("&Redo"),       this, SLOT( redo()),   CTRL+Key_Y );
 pQPopupMenu->insertItem( Theme::getIconCut(),       ("&Cut"),        this, SLOT( cut()),    CTRL+Key_X );
 pQPopupMenu->insertItem( Theme::getIconCopy(),      ("C&opy"),       this, SLOT( copy()),   CTRL+Key_C );
 pQPopupMenu->insertItem( Theme::getIconPaste(),     ("&Paste"),      this, SLOT( paste()),  CTRL+Key_C );
 pQPopupMenu->insertItem( Theme::getIconSelectAll(), ("&Select all"), this, SLOT( selectAll()),  CTRL+Key_A );
 pQPopupMenu->insertSeparator ();
 pQPopupMenu->insertItem( Theme::getIcon("InsererChampInsertion.png") ,     tr("Insérer un champ de fusion ..."),      this, SLOT( ListField()),  CTRL+Key_L );
 //............... rajouter (si il y en a) les items de la liste .......................................
 //GetListItem (pQPopupMenu, g_text );

 //............... si zone de liste proposer modification ..............................................
 //if (g_text != "")
 //   {pQPopupMenu->insertSeparator ();
 //    pQPopupMenu->insertItem( ("Modifier cette liste"),    this, SLOT( ListManager()),  CTRL+Key_L );
 //   }
 //................. connecter le menu .................................................................
 connect( pQPopupMenu, SIGNAL( activated( int)),
          this,        SLOT(OnMenuActivated( int))
        );
 connect( pQPopupMenu, SIGNAL( highlighted ( int)),
          this,        SLOT(OnMenuHighlighted( int))
        );
 connect( pQPopupMenu, SIGNAL( aboutToHide ()),
          this,        SLOT(OnMenuAboutToHide ())
        );

 m_pQPopupMenu = pQPopupMenu;
 return  pQPopupMenu;
}

//------------------------------------- DoContextListPopup ----------------------------------------------------------------
int MyEditText::DoContextListPopup()
{m_PopupItem="";
 QPopupMenu *pQPopupMenu = new QPopupMenu(0, "MyPopupMenu" );
 if (pQPopupMenu==0)    return 0;
 //............................... chercher une zone de liste deroulante (bleue) .....................
 //QString          g_text = GetLastList();     //chercher si une liste existe cinq lignes avant le curseur texte

 //............................... créer le Popup classique .........................................

 //pQPopupMenu->insertItem( QPixmap( undo ),          ("&Undo"),     this, SLOT(undo()),    CTRL+Key_Z );
 //pQPopupMenu->insertItem( QPixmap( redo ),          ("&Redo"),     this, SLOT( redo()),   CTRL+Key_Y );
 //pQPopupMenu->insertItem( QPixmap( editcut ),       ("&Cut"),      this, SLOT( cut()),    CTRL+Key_X );
 //pQPopupMenu->insertItem( QPixmap( editcopy ),      ("C&opy"),     this, SLOT( copy()),   CTRL+Key_C );
 //pQPopupMenu->insertItem( QPixmap( editpaste ),     ("&Paste"),    this, SLOT( paste()),  CTRL+Key_C );
 //pQPopupMenu->insertItem( ("&Select all"),    this, SLOT( selectAll()),  CTRL+Key_A );

 //............... rajouter (si il y en a) les items de la liste .......................................
 //GetListItem (pQPopupMenu, g_text );

 //............... si zone de liste proposer modification ..............................................
 //if (g_text != "")
 //   {pQPopupMenu->insertSeparator ();
 //   }
 //pQPopupMenu->insertItem( ("Gestionnaire de listes ..."),    this, SLOT( ListManager()),  CTRL+Key_L );
 //................. connecter le menu .................................................................
 connect( pQPopupMenu, SIGNAL( activated( int)),
          this,        SLOT(OnMenuActivated( int))
        );
 connect( pQPopupMenu, SIGNAL( highlighted ( int)),
          this,        SLOT(OnMenuHighlighted( int))
        );
 connect( pQPopupMenu, SIGNAL( aboutToHide ()),
          this,        SLOT(OnMenuAboutToHide ())
        );
 m_pQPopupMenu = pQPopupMenu;
 pQPopupMenu->exec(QCursor::pos());
 delete  pQPopupMenu;
 return 1;
}


//----------------------------------------- OnMenuAboutToHide -----------------------------------------------------------------
void MyEditText::OnMenuAboutToHide ()
{
}

//----------------------------------------- OnMenuHighlighted -----------------------------------------------------------------
// ACTION: lors du passage de la souris au dessus d'une option de menu, verifie si c'est elle correspond à un
//         fichier texte et auquel cas l'affiche en bulle d'aide à coté du menu

void MyEditText::OnMenuHighlighted ( int id )
{if (id != -1 && m_pQPopupMenu)
    {m_PopupItem    = m_pQPopupMenu->text(id);
    }
}

//----------------------------------------- OnMenuActivated -----------------------------------------------------------------
// ACTION: Slot activé lorsque une option du popup est survolée, il est noté alors le texte de l'option popup dans la
//         variable de la classe: m_ListItem.  cela permet de noter en fait le nom de la liste deroulante, et de le connaitre
//         lors de la selection.

void MyEditText::OnMenuActivated (int id)
{if (id != -1 && m_pQPopupMenu)
    {m_PopupItem   = m_pQPopupMenu->text(id);
    }
}



//------------------------------------- EmuleReturnKeyPress ----------------------------------------------------------------
// ACTION: simule l'action de l'appui de la touche return cela permet de corriger le fameux bug
//         du double interligne

void MyEditText::EmuleReturnKeyPress()
{//............. on simule un appui sur return (avec bug CRLF corrigé) ............
  QKeyEvent keyEvent  ( QEvent::KeyPress,   Qt::Key_Enter, '\n', Qt::ControlButton, "\n");
  QTextEdit::event (&keyEvent);
  QKeyEvent keyEvent2 ( QEvent::KeyRelease, Qt::Key_Enter, '\n', Qt::ControlButton, "\n");
  QTextEdit::event (&keyEvent2);
}

//------------------------------------- ListField -----------------------------------------------------------------------------
void MyEditText::ListField()
{   emit Sign_EditFusionFieldRequest();
}

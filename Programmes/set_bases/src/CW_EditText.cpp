/********************************* CW_EditText.cpp ********************************
 *                                                                                *
 *   #include "CW_EditText.h"                                                     *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat à l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Fédération, 75752 PARIS cedex 15.   *
 *                            FRANCE                                              *
 *   Centre National de la Recherche Scientifique                                 *
 *   - CNRS,                                                                      *
 *                            3 rue Michel-Ange, 75794 Paris cedex 16.            *
 *                            FRANCE                                              *
 *   Institut National de Recherche en Informatique et en Automatique             *
 *   - INRIA,                                                                     *
 *                            Domaine de Voluceau, Rocquencourt, BP 105, 78153    *
 *                            FRANCE                                              *
 *                                                                                *
 *         This program is distributed in the hope that it will be useful,        *
 *         but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                   *
 *         CeCILL  License (GPL compatible) for more details.                     *
 *                                                                                *
 **********************************************************************************/

#include "CW_EditText.h"

#include <qdir.h>
#include "../../MedinTuxTools/CHtmlTools.h"
//====================================== MyEditText ==========================================================================
// QKeyEvent( Type type, int key, int ascii, int state,
//            const QString& text=QString::null, bool autorep=FALSE, ushort count=1 )


//---------------------------------- event -------------------------------------------------------------------
// ACTION: surcharge de la fonction clavier de QTextEdit, afin de corriger l'anomalie de fonctionnement inversé
//        de la touche enter et ctrl+enter.

bool CW_EditText::event ( QEvent * ev )
{//if ( ev->type() == QEvent::MouseButtonRelease)
 //   {QMouseEvent *m = (QMouseEvent*)ev;
 //    int buton = m->button ();
 //   }
 if ( ev->type() == QEvent::KeyPress )
    {QKeyEvent   *k = (QKeyEvent *)ev;
     int      state = k->state();
     int        car = k->ascii();
    //.............. Control A .....................
     if (state==Qt::ControlButton && car == 1)
        {selectAll();
         QKeyEvent keyEvent (QKeyEvent::None, 0, 0, 0,  k->text(),  0, 0 );
         keyEvent.accept();
         return QTextEdit::event (&keyEvent);
        }
     //............. inverser le fonctionnement de la TOUCHE ENTER................................
     //              car d'origine: ENTER        --> fin de paragraphe avec double interligne
     //                             ENTER + CTRL --> fin de ligne avec interligne normal
     //              et cela ne me va pas
     if (car==13)
        {//int      state = k->state();
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

//------------------------------------------------- setText ----------------------------------------------------------------
void CW_EditText::setText ( const QString & txt )
{//mimeSourceFactory()->addFilePath(GlobalPathAppli + "IMAGES" + QDir::separator() + "RubriquesTextes");

 QString style     = CHtmlTools::getTextDefaultAttribut(txt, "style");
 //QString backColor = CHtmlTools::getTextDefaultAttribut(txt, "bgcolor");
 //if (backColor.length()>0) setPaper(QBrush( QColor(backColor)));
 //else                      setPaper(QBrush( QColor("#FFF")));
 if (style.length())
   {QFont font;
     CHtmlTools::QT_style_To_QFont(style , font);
     setFont(font);
    }
 QTextEdit::setText(txt);
}
//----------------------------------------- fillWith -----------------------------------------------------------------
// ACTION: rempli une chaine avec nb fois in
QString CW_EditText::fillWith (QString in, QString toAdd, int nb )
{for (int i=0; i<nb; ++i) in += toAdd;
 return in;
}

//----------------------------------------- moveCursor -----------------------------------------------------------------
// ACTION: permet à notre classe d'acceder à la methode protégée: QTextEdit::moveCursor( (CursorAction)action,  select);

void CW_EditText::moveCursor (int action, bool select )
{QTextEdit::moveCursor( (CursorAction)action,  select);
}




 /********************************* C_QLineEdit.cpp *******************************
 *  #include "C_QLineEdit.h"                                                      *
 * ...............................................................................*
 *   Project   :  MedinTux  (typographie made in france)                          *
 *   Copyright : (C) 2004-2005-2006-2007-2008-2009-2010 and for the eternity      *
 *   by        :  Sevin Roland     from   MedinTux project                        *
 *   E-Mail    : roland-sevin@medintux.org                                        *
 *   Web site  : www.medintux.org                                                 *
 * ...............................................................................*
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat �  l'Energie Atomique                                            *
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
 #include "C_QLineEdit.h"

 #include <QtGui>
 #include <QKeyEvent>
 #include <QFocusEvent>
 #include <QTimer>
 #include <QApplication>

#define NB_PROCESS_LOOP 10

 C_QLineEdit::C_QLineEdit(QWidget *parent)
     : QLineEdit(parent)
 {
 }

 void C_QLineEdit::keyPressEvent ( QKeyEvent * event )
 {    int call_StandardEvent_1 = C_QLineEdit::AFTER_CALL_STD_EVENT;         //
      int call_StandardEvent_2 = C_QLineEdit::AFTER_CALL_STD_EVENT;
      emit Sign_keyPressEvent(event, call_StandardEvent_1);
      emit Sign_keyPressEvent(event, call_StandardEvent_2, this);
      if ( call_StandardEvent_1!=C_QLineEdit::NOT_CALL_STD_EVENT &&
           call_StandardEvent_2!=C_QLineEdit::NOT_CALL_STD_EVENT
         ) QLineEdit::keyPressEvent(event);
 }

 void C_QLineEdit::focusInEvent ( QFocusEvent * event )
 {    int call_StandardEvent = 1;
      emit Sign_focusInEvent(event, call_StandardEvent);
      if (call_StandardEvent) QLineEdit::focusInEvent(event);
      if (event->reason ()==Qt::TabFocusReason ||event->reason ()==Qt::MouseFocusReason)
         { //emuleKeyPress(Qt::Key_End);
           //emuleKeyPress(Qt::Key_Home, Qt::ShiftModifier);
           QTimer::singleShot ( 1, this, SLOT(Slot_emuleSelectAll()));
         }
 }
/*
void C_QLineEdit::forceCursorPosition ( int pos )
 {  m_pos = pos;
   QTimer::singleShot ( 1, this, SLOT(Slot_forceCursorPosition()));
 }
void C_QLineEdit::Slot_forceCursorPosition ( )
 {  QLineEdit::setCursorPosition(m_pos);
    int i = NB_PROCESS_LOOP;
    while (--i) QApplication::processEvents ();
 }
*/
void C_QLineEdit::Slot_emuleSelectAll()
 {selectAll();
 }

void C_QLineEdit::focusOutEvent ( QFocusEvent * event )
{     int call_StandardEvent = 1;
      emit Sign_focusOutEvent(event, call_StandardEvent);
      if (call_StandardEvent) QLineEdit::focusOutEvent(event);
}

 void C_QLineEdit::Slot_emuleKeyPress()
 {
      int i;
      QKeyEvent myEv_P( QEvent::KeyPress,   m_key, m_mofifier, "");  //Qt::Key_End  Qt::ShiftModifier
      QKeyEvent myEv_R( QEvent::KeyRelease, m_key, m_mofifier, "");

      keyPressEvent   ( &myEv_P );
      i = NB_PROCESS_LOOP;
      while (--i) QApplication::processEvents ();
      keyReleaseEvent ( &myEv_R );
      i = NB_PROCESS_LOOP;
      while (--i) QApplication::processEvents ();
 }

 void C_QLineEdit::emuleKeyPress(int key, Qt::KeyboardModifiers modifier)
 {m_key      = key;
  m_mofifier = modifier;
  QTimer::singleShot ( 1, this, SLOT(Slot_emuleKeyPress() ));
 }

 void C_QLineEdit::paintEvent(QPaintEvent *pe)
 {   QLineEdit::paintEvent(pe);
 }

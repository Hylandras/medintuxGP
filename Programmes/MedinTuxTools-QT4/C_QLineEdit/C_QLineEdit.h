 /********************************* C_QLineEdit.h *********************************
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
 *   Commissariat a  l'Energie Atomique                                           *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Federation, 75752 PARIS cedex 15.   *
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


 #ifndef C_QLineEdit_H
 #define C_QLineEdit_H

 #include <QWidget>
 #include <QLineEdit>
 #include <QtDesigner/QDesignerExportWidget>

class QKeyEvent;
/*! \class C_QLineEdit
 *  \brief line edit  class derived from C_QLineEdit.
 *  - this Class is defined just to use Sign_keyPressEvent() and focusInEvent ()
 *    not disponible in QLineEdit
 *    so all publics functions will be accessibles by this pointer.
 */

 class /*QDESIGNER_WIDGET_EXPORT */C_QLineEdit : public QLineEdit
 {
     Q_OBJECT

 public:
     enum flags  { NOT_CALL_STD_EVENT     = 0,
                   AFTER_CALL_STD_EVENT   = 1
                 };
     C_QLineEdit(QWidget *parent = 0);
     void doParentPressEvent ( QKeyEvent * event ){QLineEdit::keyPressEvent(event);}
     void emuleKeyPress(int key, Qt::KeyboardModifiers modifier = Qt::NoModifier);
     //void forceCursorPosition ( int pos );
 signals:
     void Sign_keyPressEvent(QKeyEvent *, int &);
     void Sign_keyPressEvent(QKeyEvent *, int &, C_QLineEdit*);
     void Sign_focusInEvent(QFocusEvent*, int &);
     void Sign_focusOutEvent(QFocusEvent*, int &);
 protected:
     void paintEvent(QPaintEvent *event);
     void keyPressEvent ( QKeyEvent * event );
     void focusInEvent ( QFocusEvent * event );
     void focusOutEvent ( QFocusEvent * event );
 protected slots:
     void Slot_emuleKeyPress();
     void Slot_emuleSelectAll();
     //void Slot_forceCursorPosition();

 private:
     int m_key;
     //int m_pos;
     Qt::KeyboardModifiers m_mofifier;
 };

 #endif

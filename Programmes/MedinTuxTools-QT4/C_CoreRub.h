/*********************************************************************************
 *                                                                                *
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
 *   Commissariat a l'Energie Atomique                                            *
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

#ifndef C_CORERUB_H
#define C_CORERUB_H

#include <QWidget>
#include <QTextEdit>
class QTextEdit;


//========================= C_CoreRub ====================================
class C_CoreRub : public QWidget
{
    Q_OBJECT
    Q_PROPERTY( QString type_c_mdi READ type_c_mdi )
public:
    explicit C_CoreRub(QWidget *parent)
        : QWidget(parent)
    {

    }

    virtual QWidget getCentralWidget();
signals:

public slots:
private :
    QString   type_c_mdi() {return "C_CoreRub";}

};


//========================= C_CoreRub ====================================
class C_RubTextEdit : public C_CoreRub
{
    Q_OBJECT

public:
    explicit C_RubTextEdit(QWidget *parent)
        : C_CoreRub( parent )
    {
      m_pQTextEdit = new QTextEdit( this );
      m_pQTextEdit->setObjectName("mdi_CoreRubTextEditor,");
    }

    QWidget getCentralWidget() {return m_pQTextEdit;}
signals:

public slots:
private :
    QTextEdit *m_pQTextEdit;
};
#endif // C_CORERUB_H

 /********************************* C_VitaleListBoxItem.h *************************
 \file C_VitaleListBoxItem.h
 \brief (put some short descripion here).

     #include "C_VitaleListBoxItem.h"
     Project: MedinTux
     Copyright (C) 2003 2004 2005 2006 2007
     by Sevin Roland  and Data Medical Design
     E-Mail: medintux@medintux.org

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


#ifndef QFSE_C_VitaleListBoxItem
#define QFSE_C_VitaleListBoxItem

#include <qlistbox.h>
#include <qpainter.h>
#define SPACE_TXT 9
//================================================= C_VitaleListBoxItem ==============================================================
class C_VitaleListBoxItem : public QListBoxItem
{

   //Q_OBJECT
public:
    C_VitaleListBoxItem( QListBox * listbox,
                                          QString   nom,
                                          QString   prenom,
                                          QString   dateNss,
                                          QString   rangGem,
                                          QString   occurence) ;
    C_VitaleListBoxItem( QListBox * listbox, QListBoxItem * after,
                                          QString   nom,
                                          QString   prenom,
                                          QString   dateNss,
                                          QString   rangGem,
                                          QString   occurence );
    ~C_VitaleListBoxItem();
    virtual void paint( QPainter * p);
    virtual void setText( const QString &text ) {QListBoxItem::setText( text ); }
    virtual int width( const QListBox* ) const { QFontMetrics fm=(listBox()->font ());
                                                return fm.width ( text() ) + SPACE_TXT;
                                              }
    virtual int height( const QListBox* ) const {QFontMetrics fm=(listBox()->font ());
                                                return fm.height (); /* return 16; */}
    QString m_Nom;
    QString m_Prenom;
    QString m_DateNss;
    QString m_RangGem;
    QString m_Occurence;


};



#endif


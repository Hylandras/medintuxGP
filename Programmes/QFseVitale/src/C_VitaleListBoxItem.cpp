 /********************************* C_VitaleListBoxItem.cpp **********************************
 \file C_VitaleListBoxItem.cpp
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


#include "C_VitaleListBoxItem.h"
#include <qstyle.h>
//------------------------------------- C_VitaleListBoxItem --------------------------------------------------------
C_VitaleListBoxItem::C_VitaleListBoxItem( QListBox *listbox,
                                          QString   nom,
                                          QString   prenom,
                                          QString   dateNss,
                                          QString   rangGem,
                                          QString   occurence)
:QListBoxItem ( listbox ), m_Nom(nom), m_Prenom(prenom), m_DateNss(dateNss) , m_RangGem(rangGem), m_Occurence(occurence)
{
}

//------------------------------------- C_VitaleListBoxItem --------------------------------------------------------
C_VitaleListBoxItem::C_VitaleListBoxItem( QListBox     *listbox,
                                          QListBoxItem *after,
                                          QString       nom,
                                          QString       prenom,
                                          QString       dateNss,
                                          QString       rangGem,
                                          QString       occurence )
:QListBoxItem ( listbox , after), m_Nom(nom), m_Prenom(prenom), m_DateNss(dateNss) , m_RangGem(rangGem), m_Occurence(occurence)
{
}

//------------------------------------- ~C_VitaleListBoxItem --------------------------------------------------------
C_VitaleListBoxItem::~C_VitaleListBoxItem()
{
}

//---------------------------------------------------- paint --------------------------------------------------------
void C_VitaleListBoxItem::paint( QPainter *painter )
{
    // evil trick: find out whether we are painted onto our listbox
    bool in_list_box = listBox() && listBox()->viewport() == painter->device();
    int           y = ((height(listBox()) )/2)+4;
    QRect r ( 0, 0, width( listBox() ), height( listBox() ) );
    if ( in_list_box && isSelected() )   painter->eraseRect( r );
    painter->drawText (SPACE_TXT,y, m_Nom + " " + m_Prenom );

}


/********************************* CPrtQListBoxItem.cpp **************************
 *                                                                                *
 *   #include "CPrtQListBoxItem.h"                                                *
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
// C++ Implementation: CPrtQListBoxItem.cpp
//
// Description:
//
//
// Author: Sevin Roland <upsis@club-internet.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "CPrtQListBoxItem.h"
#include <qfontmetrics.h>
//-------------------------------- CPrtQListBoxItem ----------------------------------------------------
CPrtQListBoxItem::CPrtQListBoxItem(QListBox * listbox, const QString &text , const QString &pk, const QString &type, const QString &user, const QString &subType)
    : QListBoxItem(listbox), m_Type(type), m_Pk(pk), m_User(user), m_SubType(subType)
{ setText(text);
}

//-------------------------------- rtti ----------------------------------------------------
int CPrtQListBoxItem::rtti ()
{return 1001;
}

//-------------------------------- paint ----------------------------------------------------
void CPrtQListBoxItem::paint( QPainter *painter )
{
    // evil trick: find out whether we are painted onto our listbox
    bool in_list_box = listBox() && listBox()->viewport() == painter->device();
    int           y = ((height(listBox()) )/2)+5;
    QRect r ( 0, 0, width( listBox() ), height( listBox() ) );
    if ( in_list_box && isSelected() )   painter->eraseRect( r );
    if (text().at(0)=='*')
       {painter->fillRect( 5, 5, 5, height( listBox() ) - 10, Qt::red );
        painter->drawText (SPACE_TXT,y, text().mid(1));
       }
    else if (text().at(0)=='#')
       {painter->fillRect( 5, 5, 5, height( listBox() ) - 10, Qt::blue );
        painter->drawText (SPACE_TXT,y, text().mid(1));
       }
    else if (text().at(0)=='$')
       {painter->fillRect( 5, 5, 5, height( listBox() ) - 10, Qt::green );
        painter->drawText (SPACE_TXT,y, text().mid(1));
       }
    else
       {painter->drawText (SPACE_TXT,y, text());
       }
    //if ( in_list_box && isCurrent() )
       // listBox()->style().drawPrimitive( QStyle::PE_FocusRect, painter, r, listBox()->colorGroup() );
}

//-------------------------------- SetValue ----------------------------------------------------
void CPrtQListBoxItem::SetValue(const QString &pk, const QString &type, const QString &user, const QString &subType)
{m_Type      = type;
 m_Pk        = pk;
 m_User      = user;
 m_SubType   = subType;
}

//-------------------------------- GetValue ----------------------------------------------------
void CPrtQListBoxItem::GetValue(QString &pk, QString &type, QString &user, QString &subType)
{type    = m_Type;
 pk      = m_Pk;
 user    = m_User;
 subType = m_SubType;
}

//-------------------------------- GetPk ----------------------------------------------------
QString CPrtQListBoxItem::GetPk()
{ return m_Pk; }
//-------------------------------- GetType ----------------------------------------------------
QString CPrtQListBoxItem::GetType()
{ return m_Type; }
//-------------------------------- GetUser ----------------------------------------------------
QString CPrtQListBoxItem::GetUser()
{ return m_User; }
//-------------------------------- GetSubType ----------------------------------------------------
QString CPrtQListBoxItem::GetSubType()
{ return m_SubType; }

//-------------------------------- SetPk ----------------------------------------------------
void CPrtQListBoxItem::SetPk(QString& n)
{ m_Pk = n; }
//-------------------------------- SetType ----------------------------------------------------
void CPrtQListBoxItem::SetType(QString& n)
{ m_Type = n; }
//-------------------------------- SetUser ----------------------------------------------------
void CPrtQListBoxItem::SetUser(QString& n)
{ m_User = n; }
//-------------------------------- SetSubType ----------------------------------------------------
void CPrtQListBoxItem::SetSubType(QString& n)
{ m_SubType = n; }

void CPrtQListBoxItem::SetDate(QString& n)
{ m_Date = QDateTime::fromString(n, Qt::ISODate); }

void CPrtQListBoxItem::SetDate(QDateTime& dt)
{ m_Date = dt; }

QString CPrtQListBoxItem::GetDate(QString& format)
{ return m_Date.toString( format ); }

QDateTime CPrtQListBoxItem::GetDate()
{ return m_Date; }


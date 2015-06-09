 /********************************* CPrtQListBoxItem.h ****************************
 *  #include "CPrtQListBoxItem.h"                                                 *
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

// C++ Interface: CPrtQListBoxItem.h
//
// Description:
//
//
// Author: Sevin Roland <upsis@club-internet.fr>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CPRTQLISTBOXITEM_H
#define CPRTQLISTBOXITEM_H

#include <q3listbox.h>
#include <qstring.h>
#include <qpainter.h>
#include <qstyle.h>
#include <qdatetime.h>

#define SPACE_TXT 9
class CPrtQListBoxItem : public Q3ListBoxItem
{

public:
   CPrtQListBoxItem(Q3ListBox * listbox , const QString &text, const QString &pk, const QString &type, const QString &user, const QString &subType );
   int     rtti () ;
   void    SetValue(const QString &pk, const QString  &type, const QString &user, const QString &subType) ;
   void    GetValue(QString &pk, QString  &type, QString &user, QString &subType) ;

   QString   GetPk() ;
   QString   GetType() ;
   QString   GetUser() ;
   QString   GetSubType();
   QString   GetDate(QString& format);
   QDateTime GetDate();

   void      SetPk(QString& n);
   void      SetType(QString& n);
   void      SetUser(QString& n);
   void      SetSubType(QString& n);
   void      SetDate(QString& n);
   void      SetDate(QDateTime& dt);


   virtual void paint( QPainter * p);
   virtual void setText( const QString &text ) {Q3ListBoxItem::setText( text ); }
   virtual int width( const Q3ListBox* ) const { QFontMetrics fm=(listBox()->font ());
                                                return fm.width ( text() ) + SPACE_TXT;
                                              }
   virtual int height( const Q3ListBox* ) const {QFontMetrics fm=(listBox()->font ());
                                                return fm.height (); /* return 16; */}
 //................. DATA .....................................
protected:
   QString     m_Type;
   QString     m_Pk;
   QString     m_User;
   QString     m_SubType;
   QDateTime   m_Date;
};

#endif

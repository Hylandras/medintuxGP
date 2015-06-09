/********************************* CW_EditText.h **********************************
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

#ifndef CW_EDITTEXT_H
#define CW_EDITTEXT_H

#include <qobject.h>
#include <qwidget.h>
#include <qtextedit.h>
//#include <qwidgetplugin.h>

//  QT_WIDGET_PLUGIN_EXPORT //avant CW_EditText
//====================================== CW_EditText =============================================================
class  CW_EditText : public QTextEdit    //  QT_WIDGET_PLUGIN_EXPORT avant CW_EditText
{

 Q_OBJECT

 public:
   //................... constructeurs et destructeurs....................
   CW_EditText( const QString & text,
                const QString & context   = QString::null,
                QWidget       * parent    = 0,
                const char    * name      = 0)
                :QTextEdit(text, context, parent, name)
            {setTextFormat(Qt::RichText);

            }
   CW_EditText( QWidget       * parent    = 0,
	        const char    * name      = 0)
                :QTextEdit(parent, name)
            {
            }
    virtual ~CW_EditText(){//if (pCMimeSourceFactoryDoc) delete pCMimeSourceFactoryDoc;
                         }
  //...................  methodes ........................................
    virtual bool         event ( QEvent * e );
    virtual void         moveCursor ( int action, bool select =0);
    static               QString fillWith (QString in, QString toAdd, int nb );
public slots:
    void    setText ( const QString & txt );


  //...................  DATAS ........................................


};

#endif

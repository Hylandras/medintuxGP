/********************************* CBigShrinker.h ********************************
 * test                                                                           *
 *   #include "CBigShrinker.h"                                                    *
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
 *   Commissariat �l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la F��ation, 75752 PARIS cedex 15.   *
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


#ifndef C_BIGSHRINKER_H
#define C_BIGSHRINKER_H



#include <qwidget.h>
#include <qobject.h>
#include <qevent.h> 
#include <qframe.h> 


//====================================== BigShrinker ================================================================
class CBigShrinker : public QFrame 
{
    Q_OBJECT
public:
    //................ BigShrinker ...................................
    CBigShrinker(QWidget* parent, int w, int h) :
	QFrame(parent)
       {setFrameStyle(QFrame::Box|QFrame::Sunken);
	resize(w, h);
       }    

    //................. mouseReleaseEvent .............................
    void mouseReleaseEvent(QMouseEvent* e)
    {	emit clicked(e->x(), e->y());
    }

signals:
    void clicked(int,int);
};

#endif



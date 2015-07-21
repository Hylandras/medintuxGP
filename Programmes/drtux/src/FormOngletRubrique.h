/************************* FormOngletRubrique.h ***********************************
 *                                                                                *
 *   #include "FormOngletRubrique.h"                                              *
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


#ifndef FORMONGLETRUBRIQUE_H

#define FORMONGLETRUBRIQUE_H



#include <qvariant.h>

#include <qwidget.h>


class QTabWidget;



class FormOngletRubrique : public QWidget
{

    Q_OBJECT

public:

     FormOngletRubrique( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

    ~FormOngletRubrique();



    QTabWidget* tabWidget;



    virtual void addPanel( const char * name  , const QPixmap &pixmap);
    virtual void removePage( int i );

public slots:

    virtual void languageChange();
    void TimedSetOnglet();
    void TimedRemoveOnglet();
    virtual void OnRubriqueActived( QWidget * pWdgRub );
    virtual void removePage( const char* name );
    virtual void removeAllPage();

signals:

    void message(const QString&, int );

protected:
    int  m_OngletToSet;
    int  m_OngletToRemove;

protected slots:


private:

    void init();



};



#endif // FORMONGLETRUBRIQUE_H


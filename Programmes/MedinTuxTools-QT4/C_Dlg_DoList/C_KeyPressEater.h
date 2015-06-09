/*********************************** C_KeyPressEater.h ****************************
 *                                                                                *
 *   #include "C_KeyPressEater.h"             QT4-ready                           *
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
#ifndef C_KEYPRESSEATER_H
#define C_KEYPRESSEATER_H

#include <QObject>
#include <QKeyEvent>
#include <QTreeWidget>
#include <QTreeWidgetItem>

//=============================== KeyPressEater ===============================
class C_KeyPressEater : public QObject
    {
    Q_OBJECT
    public:
        C_KeyPressEater(QTreeWidget *pQTreeWidget , QObject * parent = 0, const QString & name = "C_KeyPressEater" );
    signals:
        void Sign_F1_Pressed();
        void Sign_F2_Pressed();
        void Sign_F6_Pressed();
        void Sign_Down_Pressed(QTreeWidgetItem *pQListViewItem);
        void Sign_Up_Pressed(QTreeWidgetItem *pQListViewItem);

    protected:
        QTreeWidgetItem * getSelectedListViewItem(QTreeWidget *pQTreeWidget);
        bool eventFilter( QObject *o, QEvent *e );
	QTreeWidget *m_pQTreeWidget;
    };

#endif


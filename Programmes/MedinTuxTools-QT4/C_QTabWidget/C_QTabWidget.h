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

#ifndef C_QTABWIDGET_H
#define C_QTABWIDGET_H

#include <QTabWidget>
#include <QMap>

class C_QTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit    C_QTabWidget(QWidget *parent = 0);
    QWidget   * activeTabWidgetByObjectName( const QString & widget_page_object_name);
    QWidget   * tabWidgetByTabObjectName( const QString &name, int *index = 0);
    int         tabWidgetIndexByTabObjectName( const QString &name);
    QWidget   * tabWidgetByTabText(const QString &text, int *index = 0);
    void        doPageChoiceMenu( QWidget * parent );
    QWidget   * setCurrentItem (  const QString &pageName );
    QStringList removedList();
signals:

public slots:
    void Slot_CloseRequested( int index);
private:
    QMap <QString, QWidget *>  m_removedObject;

};

#endif // C_QTABWIDGET_H

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

#ifndef C_RUBIQUESMANAGER_H
#define C_RUBIQUESMANAGER_H

#include <QWidget>
#include <QGridLayout>

#include "C_QMdiArea.h"
#include "C_RubriquesTabBar.h"
#include "C_QMdiSubWindow.h"
#include "C_Mdi.h"

//======================== C_RubriquesManager ====================================
class C_RubriquesManager : public QWidget
{
    Q_OBJECT
public:
    explicit C_RubriquesManager(QWidget *parent = 0);
    C_QMdiSubWindow *addRubrique(C_Mdi *pRubrique);

signals:

public slots:
    void Slot_showTabBar(bool state);
    void Slot_closeRubrique(const QString &tabText);
    void Slot_clear();
private slots:
    void Slot_On_Cmdi_SubWindowActivated ( QMdiSubWindow * window );
    void Slot_On_Tab_CurrentChanged ( int index );
    void Slot_On_Tab_CloseRequested( int index);
    void Slot_On_CmdiSubWindowAboutToClose(C_QMdiSubWindow *pC_QMdiSubWindow);
private :
    QGridLayout       *m_MainQGridLayout;
    C_RubriquesTabBar *m_pC_RubriquesTabBar;
    C_QMdiArea        *m_pC_QMdiArea;
};

#endif // C_RUBIQUESMANAGER_H

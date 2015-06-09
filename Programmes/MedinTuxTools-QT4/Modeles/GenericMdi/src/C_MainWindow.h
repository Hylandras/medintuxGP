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

#ifndef C_MAINWINDOW_H
#define C_MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>

#include "C_QMdiArea.h"
#include "C_RubriquesManager.h"

//========================= C_MainWindow ========================
class C_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit C_MainWindow(QWidget *parent = 0);
    ~C_MainWindow();

private:
    void        retranslateUi(QMainWindow *pC_MainWindow);
    void        setupUi(QMainWindow *pC_MainWindow);
    void        setWindowTitle();
    QMenuBar             *m_menuBar;
    QToolBar             *m_mainToolBar;
    C_RubriquesManager   *m_pC_RubriquesManager;
    QStatusBar           *m_statusBar;

private slots:
    void Slot_On_DB_ChangeConnexions( const QString &_driver , const QString &_schema, const QString &_user, const QString &_password ,const QString &_hostname ,const QString &_port );

};

#endif // C_MAINWINDOW_H

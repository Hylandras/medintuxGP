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

#include "C_MainWindow.h"
#include "ui_C_MW_MainWindow.h"
#include "CApp.h"
#include "C_BDM_Manager.h"
//--------------------------------------------- C_MainWindow -------------------------------------------------------------------
C_MainWindow::C_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_pGUI(new Ui::C_MW_MainWindow)
{
    m_pGUI->setupUi(this);
    m_pGUI->progressBar_Integration->setValue(0);
    m_pC_BDM_Manager = new C_BDM_Manager(CApp::pCApp()->iniParam(),this,m_pGUI->textEdit_Monitor, m_pGUI->progressBar_Integration);
    connect( m_pGUI->pushButton_create,     SIGNAL(pressed()),   this,   SLOT(Slot_makeTables()) );
    QMainWindow::setWindowTitle(tr("Government public drugs database initialisation"));
}

//--------------------------------------------- ~C_MainWindow -------------------------------------------------------------------
C_MainWindow::~C_MainWindow()
{
    delete m_pGUI;
}
//--------------------------------------------- Slot_makeTables -------------------------------------------------------------------
void C_MainWindow::Slot_makeTables()
{
  if (m_pGUI->tabWidget_main->currentIndex()==0)
     { m_pC_BDM_Manager->Slot_makeTables();
     }
  m_pC_BDM_Manager->it_initInteractionDataBase( m_pGUI->treeWidget_Compo,
                                                    m_pGUI->label_synthese,
                                                    "GOUV"
                                              );
}

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
#include "CApp.h"
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QStatusBar>
#include <QDebug>
#include "C_QMdiSubWindow.h"
#include "C_Mdi.h"

//------------------------ C_MainWindow -----------------------------------------
C_MainWindow::C_MainWindow(QWidget *parent) :
    QMainWindow(parent)
{   setupUi(this);

    if ( CApp::pCDb() && CApp::pCDb()->database().isValid() )
       { Slot_On_DB_ChangeConnexions( CApp::pCDb()->database().driverName() ,
                                      CApp::pCDb()->database().databaseName(),
                                      CApp::pCDb()->database().userName(),
                                      CApp::pCDb()->database().password() ,
                                      CApp::pCDb()->database().hostName(),
                                      QString::number( CApp::pCDb()->database().port()) );
       }
}

//------------------------ ~C_MainWindow -----------------------------------------
C_MainWindow::~C_MainWindow()
{

}
//------------------------ setWindowTitle -----------------------------------------
void C_MainWindow::setWindowTitle()
{    QString title = tr("%1 is not connected").arg( CApp::applicationName() );
     if ( CApp::pCDb() )
        { title = tr ("%1 connected to %2@%3:%4").arg(CApp::applicationName(), CApp::pCDb()->database().userName(), CApp::pCDb()->database().hostName(), QString::number( CApp::pCDb()->database().port()) );
        }
     QMainWindow::setWindowTitle(title);
}

//------------------------ Slot_On_DB_ChangeConnexions -----------------------------------------
void C_MainWindow::Slot_On_DB_ChangeConnexions( const QString &_driver , const QString &_schema, const QString &_user, const QString &_password ,const QString &_hostname ,const QString &_port )
{   m_pC_RubriquesManager->Slot_clear();
    C_QMdiSubWindow *pC_QMdiSubWindow = 0;
    QMapIterator<QString, QString> it( CApp::pCApp()->getRubriquesMap() );
    int ofset = 0;
    while (it.hasNext())
       { it.next();
         pC_QMdiSubWindow = m_pC_RubriquesManager->addRubrique(new C_Mdi(this,it.key() ));
         pC_QMdiSubWindow->setGeometry(20 + ofset ,20 + ofset,300,200);
         ofset += 20;
       }
    setWindowTitle();
}

//------------------------ setupUi -----------------------------------------
void C_MainWindow::setupUi(QMainWindow *pC_MainWindow)
{
    pC_MainWindow->setObjectName(QString("C_MainWindow_"NAME_APPLI));
    pC_MainWindow->resize(800, 600);

    m_menuBar = new QMenuBar(pC_MainWindow);
    m_menuBar->setObjectName(QString("m_menuBar_"NAME_APPLI));
    pC_MainWindow->setMenuBar(m_menuBar);

    m_mainToolBar = new QToolBar(pC_MainWindow);
    m_mainToolBar->setObjectName(QString("m_mainToolBar_"NAME_APPLI));
    pC_MainWindow->addToolBar(m_mainToolBar);

    m_pC_RubriquesManager   = new C_RubriquesManager(pC_MainWindow);
    m_pC_RubriquesManager->setObjectName(QString("m_pC_RubriquesManager_"NAME_APPLI));
    pC_MainWindow->setCentralWidget(m_pC_RubriquesManager);

    m_statusBar = new QStatusBar(pC_MainWindow);
    m_statusBar->setObjectName(QString("m_statusBar"NAME_APPLI));
    pC_MainWindow->setStatusBar(m_statusBar);

    retranslateUi(pC_MainWindow);

} // setupUi

//------------------------ retranslateUi -----------------------------------------
void C_MainWindow::retranslateUi(QMainWindow *pC_MainWindow)
{
 Q_UNUSED(pC_MainWindow)
    //C_MainWindow->setWindowTitle(QApplication::translate("C_MainWindow", "C_MainWindow", 0));
} // retranslateUi

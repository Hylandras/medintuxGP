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
#include <QDebug>
#include "C_RubriquesManager.h"
#include "C_QMdiSubWindow.h"
#include "../../MedinTuxTools-QT4/Theme.h"

//----------------------------- C_RubriquesManager -------------------------------------------
C_RubriquesManager::C_RubriquesManager(QWidget *parent) :
    QWidget(parent)
{
     //................ creer la barre de tabulations ......................................
     m_pC_RubriquesTabBar = new C_RubriquesTabBar(this);
     m_pC_RubriquesTabBar->setElideMode(Qt::ElideRight);
     m_pC_RubriquesTabBar->setUsesScrollButtons(true);
     m_pC_RubriquesTabBar->setDocumentMode(true);
     m_pC_RubriquesTabBar->setTabsClosable(true);
     m_pC_RubriquesTabBar->setMovable(true);
     m_pC_RubriquesTabBar->setObjectName(QString("m_pC_RubriquesTabBar_"NAME_APPLI));

     //................ creer le conteneur de fenetre cmdi ......................................
     m_pC_QMdiArea = new C_QMdiArea(this);
     m_pC_QMdiArea->setObjectName(QString("m_pC_QMdiArea_"NAME_APPLI));
     QPalette back;
     back.setBrush ( QPalette::Window, QBrush(Theme::getIcon( "medintux_logo.png" )) );
     m_pC_QMdiArea->setPalette(back);                                               // fond par defaut des fenetres mdi
     m_pC_QMdiArea->setBackground(QBrush(Theme::getIcon( "ImageDeFond.png" )));     // fond du conteneur

     //................ creer le layout principal ......................................
     //                 qui contiendra la barre de tabulations  et le conteneur des fenetres CMDI
     m_MainQGridLayout = new QGridLayout(this);
     m_MainQGridLayout->setObjectName(QString("m_MainQGridLayout_"NAME_APPLI));
     m_MainQGridLayout->setMargin(0);
     m_MainQGridLayout->setSpacing(0);
     setLayout(m_MainQGridLayout);
     m_MainQGridLayout->addWidget(m_pC_RubriquesTabBar,0,0);  // ajouter la barre de tabulations au layout principal
     m_MainQGridLayout->addWidget(m_pC_QMdiArea,1,0);         // ajouter conteneur des fenetres CMDI au layout principal

     connect(m_pC_RubriquesTabBar, SIGNAL(currentChanged ( int )),                this, SLOT(   Slot_On_Tab_CurrentChanged ( int  )));
     connect(m_pC_RubriquesTabBar, SIGNAL(tabCloseRequested (int )) ,             this, SLOT (  Slot_On_Tab_CloseRequested( int )) );
     connect(m_pC_QMdiArea,        SIGNAL(subWindowActivated ( QMdiSubWindow *)), this, SLOT(   Slot_On_Cmdi_SubWindowActivated ( QMdiSubWindow *  ) ));
}

//----------------------------- Slot_clear -------------------------------------------
void C_RubriquesManager::Slot_clear()
{
    m_pC_QMdiArea->closeAllSubWindows();

}

//----------------------------- Slot_showTabBar -------------------------------------------
void C_RubriquesManager::Slot_showTabBar(bool state)
{ if (state) m_pC_RubriquesTabBar->show();
  else       m_pC_RubriquesTabBar->hide();
}
//----------------------------- Slot_closeRubrique -------------------------------------------
void C_RubriquesManager::Slot_closeRubrique(const QString &tabText)
{   if (tabText==0) return;
    int index = m_pC_RubriquesTabBar->tabIndexByTabText(tabText);
    if (index>=0) Slot_On_Tab_CloseRequested( index );
}
//----------------------------- addRubrique -------------------------------------------
C_QMdiSubWindow *C_RubriquesManager::addRubrique(C_Mdi *pRubrique)
{    C_QMdiSubWindow *pC_QMdiSubWindow = new C_QMdiSubWindow(m_pC_QMdiArea);
     pC_QMdiSubWindow->setAttribute(Qt::WA_DeleteOnClose);
     pC_QMdiSubWindow->setWidget ( pRubrique );
     pC_QMdiSubWindow->setWindowTitle ( pRubrique->rubName() );
     m_pC_RubriquesTabBar->addTab( pRubrique->rubName() );
     m_pC_QMdiArea->addSubWindow (pC_QMdiSubWindow);
     connect(pC_QMdiSubWindow,        SIGNAL(Sign_SubWindowAboutToClose(C_QMdiSubWindow *)), this, SLOT(Slot_On_CmdiSubWindowAboutToClose(C_QMdiSubWindow *)));
     return pC_QMdiSubWindow;
}
//----------------------------- Slot_On_CmdiSubWindowAboutToClose -------------------------------------------
void C_RubriquesManager::Slot_On_CmdiSubWindowAboutToClose(C_QMdiSubWindow *window)
{ if (window==0)   return;
  C_Mdi *pRubrique = (C_Mdi*)window->widget();
  if (pRubrique==0) return;
  disconnect(m_pC_RubriquesTabBar, SIGNAL(currentChanged ( int )),             this, SLOT(Slot_On_Tab_CurrentChanged ( int  )));
  int index = m_pC_RubriquesTabBar->tabIndexByTabText(pRubrique->rubName());
  if (index>=0) m_pC_RubriquesTabBar->removeTab(index);
  connect(m_pC_RubriquesTabBar,    SIGNAL(currentChanged ( int )),             this, SLOT(Slot_On_Tab_CurrentChanged ( int  )));
}

//----------------------------- Slot_On_Cmdi_SubWindowActivated -------------------------------------------
void C_RubriquesManager::Slot_On_Cmdi_SubWindowActivated ( QMdiSubWindow * window )
{ if (window==0)   return;
  C_Mdi *pRubrique = (C_Mdi*)window->widget();
  if (pRubrique==0) return;
  disconnect(m_pC_RubriquesTabBar, SIGNAL(currentChanged ( int )),             this, SLOT(Slot_On_Tab_CurrentChanged ( int  )));
  int index = m_pC_RubriquesTabBar->tabIndexByTabText(pRubrique->rubName());
  if (index>=0) m_pC_RubriquesTabBar->setCurrentIndex(index);
  connect(m_pC_RubriquesTabBar,    SIGNAL(currentChanged ( int )),             this, SLOT(Slot_On_Tab_CurrentChanged ( int  )));
}

//----------------------------- Slot_On_Tab_CloseRequested -------------------------------------------
void C_RubriquesManager::Slot_On_Tab_CloseRequested( int index)
{ // disconnect(m_pC_QMdiArea,        SIGNAL(subWindowActivated ( QMdiSubWindow *)), this, SLOT(Slot_On_Cmdi_SubWindowActivated ( QMdiSubWindow *  ) ));
  if (index <= -1) return;
  QString tabText = m_pC_RubriquesTabBar->tabText ( index );
  //
  QMdiSubWindow *rubriqueToDelete     = 0;
  QList<QMdiSubWindow *> mdiSubWindowList =  m_pC_QMdiArea->subWindowList ();
  for (int i=0; i<mdiSubWindowList.size();++i)
      { QMdiSubWindow *window = mdiSubWindowList[i];
        C_Mdi *pRubrique      = (C_Mdi*)window->widget();
        if (pRubrique->rubName()==tabText) rubriqueToDelete = window;
      }

  if ( rubriqueToDelete )
     {  C_Mdi *pRubrique      = (C_Mdi*)rubriqueToDelete->widget();
        m_pC_QMdiArea->removeSubWindow ( rubriqueToDelete );
        if (pRubrique) delete pRubrique;
     }
  m_pC_RubriquesTabBar->removeTab(index);
  // connect(m_pC_QMdiArea,           SIGNAL(subWindowActivated ( QMdiSubWindow *)), this, SLOT(Slot_On_Cmdi_SubWindowActivated ( QMdiSubWindow *  ) ));

}

//----------------------------- Slot_On_Tab_CurrentChanged -------------------------------------------
void C_RubriquesManager::Slot_On_Tab_CurrentChanged ( int index )
{   disconnect(m_pC_QMdiArea,        SIGNAL(subWindowActivated ( QMdiSubWindow *)), this, SLOT(Slot_On_Cmdi_SubWindowActivated ( QMdiSubWindow *  ) ));
    if (index <= -1) return;
    QString tabText = m_pC_RubriquesTabBar->tabText ( index );
    //
    QMdiSubWindow *rubriqueToActivate     = 0;
    QList<QMdiSubWindow *> mdiSubWindowList =  m_pC_QMdiArea->subWindowList ();
    for (int i=0; i<mdiSubWindowList.size();++i)
        { QMdiSubWindow *window = mdiSubWindowList[i];
          C_Mdi *pRubrique      = (C_Mdi*)window->widget();
          if (pRubrique->rubName()==tabText) rubriqueToActivate = window;
        }

    if ( rubriqueToActivate )
       {  m_pC_QMdiArea->setActiveSubWindow ( rubriqueToActivate );
       }
  connect(m_pC_QMdiArea,           SIGNAL(subWindowActivated ( QMdiSubWindow *)), this, SLOT(Slot_On_Cmdi_SubWindowActivated ( QMdiSubWindow *  ) ));
}

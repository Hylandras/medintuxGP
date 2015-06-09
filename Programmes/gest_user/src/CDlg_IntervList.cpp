
/******************************* CDlg_IntervList.cpp ******************************
 *                                                                                *
 *   #include "CDlg_IntervList.h"                                                 *
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
 *   Commissariat Ã  l'Energie Atomique                                           *
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

#include "CDlg_IntervList.h"
#include "CApp.h"
#include "../../drtux/src/CMoteur_Base.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qsplitter.h>
#include <qlineedit.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qheader.h>
#include <qlistview.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

//---------------------------------------------- ~Dlg_IntervList ----------------------------------------------------------------------
CDlg_IntervList::CDlg_IntervList(const QStringList &listGUIDAlreadyPresent, QWidget* parent, const char* name, bool modal, WFlags fl)
    : Dlg_IntervList(parent,name, modal,fl)
{connect( pushButtonOk, SIGNAL( clicked() ), this, SLOT( pushButtonOk_clicked() ) );
 connect( pushButtonAnnuler, SIGNAL( clicked() ), this, SLOT( pushButtonAnnuler_clicked() ) );
 connect( ListView_UserList, SIGNAL( doubleClicked(QListViewItem*) ), this, SLOT( ListView_UserList_doubleClicked(QListViewItem*) ) );
 connect( lineEditNom, SIGNAL( textChanged(const QString&) ), this, SLOT( lineEditNom_textChanged(const QString&) ) );
 radioButtonCommence->setChecked (TRUE);
 radioButtonContient->setChecked (FALSE);
 lineEditNom->setFocus();
 m_pCMoteurBase  = G_pCApp->m_pCMoteurBase;
 m_Pk_Returned   = "";
 if (m_pCMoteurBase) InitListView(listGUIDAlreadyPresent);
}

//---------------------------------------------- ~CDlg_IntervList ----------------------------------------------------------------------
CDlg_IntervList::~CDlg_IntervList()
{
}

/*$SPECIALIZATION$*/
//---------------------------------------------- InitListView ----------------------------------------------------------------------
int CDlg_IntervList::InitListView(const QStringList &listGUIDAlreadyPresent)
{//................. Preparer la requete .....................................
  if (m_pCMoteurBase->OpenBase()==0)  { return 0; }
  //................. SI un nom ou prenom est donné en entrée alors filtrer selon ...................
  QString requete("SELECT ");
  requete       += m_pCMoteurBase->m_USER_IDENT_NOM      + "," +   // 0
                   m_pCMoteurBase->m_USER_IDENT_PRENOM   + "," +   // 1
                   m_pCMoteurBase->m_USER_IDENT_PRIMKEY  + "," +   // 2
                   m_pCMoteurBase->m_USER_IDENT_GUID     + " " +   // 3
                   " FROM "     +  m_pCMoteurBase->m_USER_IDENT_TBL_NAME +
                   " ORDER BY " +  m_pCMoteurBase->m_USER_IDENT_NOM;
  QSqlQuery query(requete , m_pCMoteurBase->m_DataBase );
  if (query.isActive())
     {while (query.next())
            {QString str = m_pCMoteurBase->Utf8_Query(query, 2 );
             if (listGUIDAlreadyPresent.findIndex(str)==-1) new QListViewItem(     ListView_UserList,
                                                                                   m_pCMoteurBase->Utf8_Query(query, 0 ),
                                                                                   m_pCMoteurBase->Utf8_Query(query, 1 ),
                                                                                   str,
                                                                                   m_pCMoteurBase->Utf8_Query(query, 3 )
                                                                              );
            }
     } //endif (pSqlQuery && pSqlQuery->isActive())
  m_pCMoteurBase->CloseBase();
  return 1;
}

//---------------------------------------------- pushButtonOk_clicked ----------------------------------------------------------------------
void CDlg_IntervList::pushButtonOk_clicked()
{accept();
}

//---------------------------------------------- pushButtonAnnuler_clicked ----------------------------------------------------------------------
void CDlg_IntervList::pushButtonAnnuler_clicked()
{reject();
}

//---------------------------------------------- ListView_UserList_doubleClicked ----------------------------------------------------------------------
void CDlg_IntervList::ListView_UserList_doubleClicked(QListViewItem *pQListViewItem)
{accept();
}

//---------------------------------------------- lineEditNom_textChanged ----------------------------------------------------------------------
void CDlg_IntervList::lineEditNom_textChanged(const QString &text)
{
 QListViewItem *pQListViewItem = 0;
 if (radioButtonCommence->isChecked ())
     pQListViewItem = ListView_UserList->findItem ( text, 0, Qt::BeginsWith);
 else
     pQListViewItem = ListView_UserList->findItem ( text, 0, Qt::Contains);
 if (pQListViewItem == 0) return;
 ListView_UserList->setSelected ( pQListViewItem, TRUE );
 ListView_UserList->ensureItemVisible ( pQListViewItem );
}

//---------------------------------------------- ~reject ----------------------------------------------------------------------
void CDlg_IntervList::reject()
{ m_Pk_Returned = "";
  QDialog::reject();
}

//---------------------------------------------- ~accept ----------------------------------------------------------------------
void CDlg_IntervList::accept()
{ QListViewItem *pQListViewItem       =  ListView_UserList->selectedItem();
  if (pQListViewItem) m_Pk_Returned = pQListViewItem->text(2);
  QDialog::accept();
}




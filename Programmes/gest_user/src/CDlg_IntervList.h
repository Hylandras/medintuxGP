/******************************* CDlg_IntervList.h ********************************
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
#ifndef CDLG_INTERVLIST_H
#define CDLG_INTERVLIST_H

#include "ui/Dlg_IntervList.h"
#include "../../drtux/src/CMoteur_Base.h"

//====================================== CDlg_IntervList =======================================================
class CDlg_IntervList : public Dlg_IntervList
{
  Q_OBJECT

public:
  CDlg_IntervList(const QStringList &listGUIDAlreadyPresent, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  ~CDlg_IntervList();
  /*$PUBLIC_FUNCTIONS$*/
  QString       m_Pk_Returned;
public slots:
  /*$PUBLIC_SLOTS$*/

protected:
  /*$PROTECTED_FUNCTIONS$*/
int InitListView(const QStringList &listGUIDAlreadyPresent);
CMoteurBase  *m_pCMoteurBase;

protected slots:
  /*$PROTECTED_SLOTS$*/
  virtual void          reject();
  virtual void          accept();
  virtual void          pushButtonOk_clicked();
  virtual void          pushButtonAnnuler_clicked();
  virtual void          ListView_UserList_doubleClicked(QListViewItem *pQListViewItem);
  virtual void          lineEditNom_textChanged(const QString &text);
};

#endif


 /********************************* CDlg_MetaFieldDefine.h ************************************
 \file CDlg_MetaFieldDefine.h
 \brief (put some short descripion here).

     #include "CDlg_MetaFieldDefine.h"
     Project: MedinTux
     Copyright (C) 2003 2004 2005 2006 2007
     by Sevin Roland  and Data Medical Design
     E-Mail: medintux@medintux.org

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

#ifndef CDLG_METAFIELDDEFINE_H
#define CDLG_METAFIELDDEFINE_H

#include <qlistview.h>
#include <qcombobox.h>
#include <qlistbox.h>
#include <qlineedit.h>
#include <qtable.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <qmessagebox.h>

#include "Dlg_MetaFieldDefine.h"

class CDlg_MetaFieldDefine : public Dlg_MetaFieldDefine
{
  Q_OBJECT

public:
  enum exeMode {
        create,
        modify
    };
  CDlg_MetaFieldDefine(QListView * pQListView,  QListViewItem* pQListViewItem, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  ~CDlg_MetaFieldDefine();
  /*$PUBLIC_FUNCTIONS$*/
  void CDlg_MetaFieldDefineInitDialog(QListView * pQListView, QListViewItem* pQListViewItem);
  void table_ParamClear();
  QListViewItem *IfThisNameExist( QListView * pQListView, const QString & name );
public slots:
  /*$PUBLIC_SLOTS$*/
  virtual void          comboBox_SyntaxModels_activated( const QString & );
  virtual void          comboBox_TypeRubrique_activated( const QString & );

protected:
  /*$PROTECTED_FUNCTIONS$*/
QListView*     m_pQListView;
QListViewItem* m_pQListViewItem;

protected slots:
  /*$PROTECTED_SLOTS$*/
  virtual void          reject();
  virtual void          accept();

};

#endif



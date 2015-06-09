 /********************************* .h ************************************
 \file .h
 \brief (put some short descripion here).

     #include ".h"
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



#ifndef C_DLG_MAINDIALOG_H
#define C_DLG_MAINDIALOG_H

#include "ui/Dlg_MainDialog.h"

class C_Dlg_MainDialog : public Dlg_MainDialog
{
  Q_OBJECT

public:
  C_Dlg_MainDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  ~C_Dlg_MainDialog();
  /*$PUBLIC_FUNCTIONS$*/
   void Test_And_RemoveDir(QString src_Dir,  QString dirList_To_remove,  QString motifList_To_Erase);
   void EraseDirectory( QString dir_to_erase);
   bool isThisFileMustBeErase( const QString &fname, const QStringList &fileToRmList);
   bool isThisDirMustBeErase(  const QString &dname, const QStringList &dirToRmList);
   void setInterfaceOnProfil(QString profil);
   void setComboOnProfil(const QString & );
   void saveProfil(const QString & profil);
public slots:
  /*$PUBLIC_SLOTS$*/

protected:
  /*$PROTECTED_FUNCTIONS$*/
  QString m_PathAppli;
  QString m_Param;
  QString m_PathProfils;

protected slots:
  /*$PROTECTED_SLOTS$*/
  virtual void          Slot_On_pushButtonAddProfil();
  virtual void          Slot_On_pushButtonDelProfil();
  virtual void          Slot_On_pushButtonSaveProfil();
  virtual void          Slot_On_comboBoxProfils_activated(const QString &);
  virtual void          Slot_On_comboBoxProfils_highlighted(const QString &profil);
  virtual void          Slot_On_pushButton_find();
  virtual void          Slot_On_pushButton_Effacer();
  virtual void          Slot_On_pushButton_Quitter();

};

#endif


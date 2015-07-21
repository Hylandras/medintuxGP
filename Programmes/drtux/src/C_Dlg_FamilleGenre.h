/******************** C_Dlg_FamilleGenre.cpp *************************************
 *                                                                                *
 *   #include "C_Dlg_FamilleGenre.h"                                              *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 ->2009 by Sevin Roland  and Data Medical Design *
 *   E-Mail: roland-sevin@medintux.org                                            *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat à l'Energie Atomique                                            *
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

#ifndef C_DLG_FAMILLEGENRE_H
#define C_DLG_FAMILLEGENRE_H

#include "ui/Dlg_FamilleGenre.h"

class C_Dlg_FamilleGenre : public Dlg_FamilleGenre
  {
      Q_OBJECT

    public:
      C_Dlg_FamilleGenre(QString* pStructStr, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0);
      ~C_Dlg_FamilleGenre();
      /*$PUBLIC_FUNCTIONS$*/
      bool isModified() {return m_isModified;}
    public slots:
      /*$PUBLIC_SLOTS$*/

    protected:
      /*$PROTECTED_FUNCTIONS$*/
    void      unserialize(const QString &data);
    QString   serialize();

    QString *m_pStructStr;
    bool     m_isModified;
    protected slots:
      /*$PROTECTED_SLOTS$*/
       void Slot_pushButton_save();
       void Slot_pushButton_del();
       void Slot_pushButton_add_Famille();
       void Slot_pushButton_add_Genre();

      virtual void          Slot_reject();
      virtual void          Slot_accept();

  };

#endif


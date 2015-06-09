/********************************* C_Dlg_VigieFiltreConf.cpp *************************

    Project: MedinTux
    Copyright (C) 2005 by Roland Sevin  and Data Medical Design
    E-Mail: roland-sevin@medintux.org

    This program is free software; you can redistribute it and/or modify
    it under the terms of the CeCILL Version 1 du 21/06/2004
    License (GPL compatible)
                               http://www.cecill.info/
    as published by :

    Commissariat � l'Energie Atomique
    - CEA,
                             31-33 rue de la F�d�ration, 75752 PARIS cedex 15.
                             FRANCE
    Centre National de la Recherche Scientifique
    - CNRS,
                             3 rue Michel-Ange, 75794 Paris cedex 16.
                             FRANCE
    Institut National de Recherche en Informatique et en Automatique
    - INRIA,
                             Domaine de Voluceau, Rocquencourt, BP 105, 78153
                             FRANCE

          This program is distributed in the hope that it will be useful,
          but WITHOUT ANY WARRANTY; without even the implied warranty of
          MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
          CeCILL  License (GPL compatible) for more details.
*/
#include <qlineedit.h>
#include <qtextedit.h>
#include <qpushbutton.h>

#include "C_Dlg_VigieFiltreConf.h"

//----------------------------------------------- C_Dlg_VigieFiltreConf ------------------------------------------------------------------------
C_Dlg_VigieFiltreConf::C_Dlg_VigieFiltreConf(QString *pfilterName, QString *pfilter, QWidget* parent, const char* name, bool modal, WFlags fl)
: Dlg_VigieFiltreConf(parent,name, modal,fl)
{
 m_pFilterName = pfilterName;
 m_pFilter     = pfilter;
 *m_pFilter    = m_pFilter->replace("\r\n","\n");
 lineEdit_FiltreName->setText(*m_pFilterName);
 textEdit_Authorized->setText(*m_pFilter);
 connect( pushButton_Ok ,        SIGNAL( clicked() ),  this, SLOT( accept() ) );
 connect( pushButton_Annuler ,   SIGNAL( clicked() ),  this, SLOT( reject() ) );
}

//----------------------------------------------- ~C_Dlg_VigieFiltreConf ------------------------------------------------------------------------
C_Dlg_VigieFiltreConf::~C_Dlg_VigieFiltreConf()
{
}

/*$SPECIALIZATION$*/
//----------------------------------------------- reject ------------------------------------------------------------------------
void C_Dlg_VigieFiltreConf::reject()
{
  QDialog::reject();
}

//----------------------------------------------- accept ------------------------------------------------------------------------
void C_Dlg_VigieFiltreConf::accept()
{ *m_pFilter       = textEdit_Authorized->text().replace("\r\n","\n");
  *m_pFilter       = m_pFilter->stripWhiteSpace();
  *m_pFilterName   = lineEdit_FiltreName->text();
  *m_pFilterName   = m_pFilterName->stripWhiteSpace();
  QDialog::accept();
}




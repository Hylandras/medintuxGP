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

#include "C_Dlg_GetSecrets.h"
#include "ui_C_Dlg_GetSecrets.h"

C_Dlg_GetSecrets::C_Dlg_GetSecrets(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::C_Dlg_GetSecrets)
{
    ui->setupUi(this);
    connect(ui->pushButton_ok ,      SIGNAL(clicked()),      this, SLOT(accept()));
    connect(ui->pushButton_cancel ,  SIGNAL(clicked()),      this, SLOT(reject()));
}

C_Dlg_GetSecrets::~C_Dlg_GetSecrets()
{ delete ui;
}
QString C_Dlg_GetSecrets::client_ID()
{ return ui->lineEdit_Client_ID->text().trimmed();
}

QString C_Dlg_GetSecrets::client_Secret()
{ return ui->lineEdit_Client_Secret->text().trimmed();
}
void C_Dlg_GetSecrets::setClient_ID(const QString &value)
{ ui->lineEdit_Client_ID->setText(value.trimmed());
}

void C_Dlg_GetSecrets::setClient_Secret(const QString &value)
{ ui->lineEdit_Client_Secret->setText(value.trimmed());
}

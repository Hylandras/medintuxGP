/********************************* C_Dlg_Assure.cpp ******************************
*  #include "C_Dlg_Assure.h"                                                     *
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
#include "C_Dlg_Assure.h"
#include "ui_C_Dlg_Assure.h"

//---------------------------- C_Dlg_Assure ----------------------------
C_Dlg_Assure::C_Dlg_Assure(QWidget *parent,const QString &nom,const QString &prenom,const QString &dateNss) :
    QDialog(parent),
    m_ui(new Ui::C_Dlg_Assure)
{
    m_ui->setupUi(this);
    m_ui->lineEdit_dateNss->setInputMask ("99-99-9999");

    m_ui->lineEdit_nom->setText(nom);
    m_ui->lineEdit_nom->setFocus();
    m_ui->lineEdit_nom->selectAll();
    m_ui->lineEdit_prenom->setText(prenom);
    m_ui->lineEdit_dateNss->setText(QString(dateNss).remove('-').remove('/').trimmed());
    connect( m_ui->pushButton_Ok,        SIGNAL(clicked()),     this, SLOT(Slot_pushButton_Ok_clicked()) );
    connect( m_ui->pushButton_Annuler,   SIGNAL(clicked()),     this, SLOT(Slot_pushButton_pushButton_Annuler_clicked()) );

}
//---------------------------- ~C_Dlg_Assure ----------------------------
C_Dlg_Assure::~C_Dlg_Assure()
{
    delete m_ui;
}
//---------------------------- changeEvent ----------------------------
void C_Dlg_Assure::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
//---------------------------- getNom ----------------------------
QString C_Dlg_Assure::getNom()
{return m_ui->lineEdit_nom->text();
}
//---------------------------- getPrenom ----------------------------
QString C_Dlg_Assure::getPrenom()
{return m_ui->lineEdit_prenom->text();
}
//---------------------------- getDateNss ----------------------------
QString C_Dlg_Assure::getDateNss()
{return m_ui->lineEdit_dateNss->text();
}
//---------------------------- Slot_pushButton_Ok_clicked ----------------------------
void C_Dlg_Assure::Slot_pushButton_Ok_clicked()
{accept();
}
//---------------------------- Slot_pushButton_pushButton_Annuler_clicked ----------------------------
void C_Dlg_Assure::Slot_pushButton_pushButton_Annuler_clicked()
{reject();
}

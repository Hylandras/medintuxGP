/*************************** C_Dlg_ImprimerRDV.cpp *******************************
*  #include "C_Dlg_ImprimerRDV.h"                                                *
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
*   Commissariat \303\240 l'Energie Atomique                                     *
*   - CEA,                                                                       *
*              31-33 rue de la F\303\251d\303\251ration, 75752 PARIS cedex 15.   *
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
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPlainTextEdit>
#include <QPrinter>
#include <QApplication>

#include "C_Dlg_ImprimerRDV.h"
#include "ui_C_Dlg_ImprimerRDV.h"
#include "C_Dlg_Agenda.h"
#include "../../Manager/src/CApp.h"

//------------------------------------------------- C_Dlg_ImprimerRDV --------------------------
C_Dlg_ImprimerRDV::C_Dlg_ImprimerRDV(CMoteurAgenda* pCMoteurAgenda, QDateTime date_rdv, QString code_user, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::C_Dlg_ImprimerRDV)
{   m_pCMoteurAgenda = pCMoteurAgenda;
    QPixmap pm_deb = Theme::getIcon("Agenda/agendaDebut.png");
    QPixmap pm_end = Theme::getIcon("Agenda/agendaFin.png");
    ui->setupUi(this);
    ui->label_Icone->setPixmap(Theme::getIcon("Agenda/PrintListRdv.png"));
    ui->pushButton_DateDeb->setPixmap(pm_deb);
    ui->pushButton_DateEnd->setPixmap(pm_end);
    ui->pushButton_DateDeb->setMaximumWidth(pm_deb.width()+4);
    ui->pushButton_DateEnd->setMaximumWidth(pm_end.width()+4);
    ui->pushButton_DateDeb->setMaximumHeight(pm_deb.height()+4);
    ui->pushButton_DateEnd->setMaximumHeight(pm_end.height()+4);
    ui->dateEdit_dateDeb->setDate(date_rdv.date());
    ui->dateEdit_dateFin->setDate(date_rdv.date());
    m_pCMoteurAgenda->initComboMedecins(ui->comboBox_Users, code_user);
    m_pQPlainTextEdit = new QPlainTextEdit(this); //ui->plainTextEdit_view;
    m_pQPlainTextEdit->hide();
    ui->pushButton_Imprimer->hide();
    connect (ui->pushButton_preview,        SIGNAL(clicked()),                       this, SLOT(Slot_preview()));
    connect (ui->pushButton_DateDeb,        SIGNAL(clicked()),                       this, SLOT(Slot_pushButton_DateDeb_clicked()));
    connect (ui->pushButton_DateEnd,        SIGNAL(clicked()),                       this, SLOT(Slot_pushButton_DateEnd_clicked()));
}

//------------------------------------------------- ~C_Dlg_ImprimerRDV --------------------------
C_Dlg_ImprimerRDV::~C_Dlg_ImprimerRDV()
{
    delete ui;
}

//------------------------------------------------- changeEvent --------------------------
void C_Dlg_ImprimerRDV::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

//------------------------------------------------- Slot_pushButton_DateDeb_clicked --------------------------
void C_Dlg_ImprimerRDV::Slot_pushButton_DateDeb_clicked()
{execCalendrier(ui->dateEdit_dateDeb);
}
//------------------------------------------------- Slot_pushButton_DateEnd_clicked --------------------------
void C_Dlg_ImprimerRDV::Slot_pushButton_DateEnd_clicked()
{execCalendrier(ui->dateEdit_dateFin);
}

//------------------------------------------------- execCalendrier --------------------------
void C_Dlg_ImprimerRDV::execCalendrier(QDateEdit *pQDateEdit)
{//............... lancer le calendrier .................................................
    QDate dateIn  = pQDateEdit->date();
    QString date  = G_pCApp->execCalendrier(dateIn); date = date.remove('-');
    QDate dateNew = QDate::fromString(date, "ddMMyyyy");
    //....................... analyse du retour ..........................................................................
    if (dateNew.isValid () && dateNew != dateIn )
       {pQDateEdit->setDate(dateNew);
       }
}

//------------------------------------------------- Slot_preview --------------------------
void C_Dlg_ImprimerRDV::Slot_preview()
{   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString loginMed = ui->comboBox_Users->itemData(ui->comboBox_Users->currentIndex()).toString();
    QDate   dateDEB  = ui->dateEdit_dateDeb->date();
    QDate   dateFIN  = ui->dateEdit_dateFin->date();
    m_pCMoteurAgenda->paginer_les_RDV(loginMed, dateDEB, dateFIN, m_pQPlainTextEdit);
    QApplication::restoreOverrideCursor ();
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    preview.setWindowTitle(tr("Appointments printing for: %1").arg(ui->comboBox_Users->text(ui->comboBox_Users->currentIndex())));
    connect(&preview, SIGNAL(paintRequested(QPrinter *)), SLOT(Slot_printPreview(QPrinter *)));
    preview.setMinimumSize(800,600);
    preview.setBaseSize(800,600);
    preview.exec();

#endif

}

//----------------------------------------Slot_printPreview-------------------------------------------------
void C_Dlg_ImprimerRDV::Slot_printPreview(QPrinter *printer)
{
#ifdef QT_NO_PRINTER
    Q_UNUSED(printer);
#else
    m_pQPlainTextEdit->print(printer);
#endif
}

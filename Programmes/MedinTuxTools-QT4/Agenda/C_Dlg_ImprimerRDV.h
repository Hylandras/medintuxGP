/*************************** C_Dlg_ImprimerRDV.h *********************************
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

#ifndef C_DLG_IMPRIMERRDV_H
#define C_DLG_IMPRIMERRDV_H

#include <QDialog>
#include "CMoteurAgenda.h"
#include "ui_C_Dlg_ImprimerRDV.h"

namespace Ui {
    class C_Dlg_ImprimerRDV;
}
class C_Dlg_ImprimerRDV : public QDialog {
    Q_OBJECT
public:
    C_Dlg_ImprimerRDV(CMoteurAgenda* pCMoteurAgenda, QDateTime date_rdv, QString code_user, QWidget *parent = 0);
    ~C_Dlg_ImprimerRDV();
     Ui::C_Dlg_ImprimerRDV *ui;
     QPlainTextEdit *m_pQPlainTextEdit;
protected:
    void changeEvent(QEvent *e);
    void execCalendrier(QDateEdit *pQDateEdit);
     CMoteurAgenda *m_pCMoteurAgenda;

private slots:
    void Slot_pushButton_DateDeb_clicked();
    void Slot_pushButton_DateEnd_clicked();
    void Slot_preview();
    void Slot_printPreview(QPrinter *printer);
};

#endif // C_DLG_IMPRIMERRDV_H

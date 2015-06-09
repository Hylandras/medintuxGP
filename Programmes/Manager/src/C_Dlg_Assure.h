
/********************************* C_Dlg_Assure.h ********************************
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
#ifndef C_DLG_ASSURE_H
#define C_DLG_ASSURE_H

#include <QDialog>

namespace Ui {
    class C_Dlg_Assure;
}
//=========================================== C_Dlg_Assure =============================
class C_Dlg_Assure : public QDialog {
    Q_OBJECT
public:
    C_Dlg_Assure(QWidget *parent, const QString &nom, const QString &prenom, const QString &dateNss);
    ~C_Dlg_Assure();
    QString getNom();
    QString getPrenom();
    QString getDateNss();
protected:
    void changeEvent(QEvent *e);

private:
    Ui::C_Dlg_Assure *m_ui;

private slots:
    void Slot_pushButton_Ok_clicked();
    void Slot_pushButton_pushButton_Annuler_clicked();
};

#endif // C_DLG_ASSURE_H

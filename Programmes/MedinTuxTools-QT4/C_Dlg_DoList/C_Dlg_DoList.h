 /********************************* C_Dlg_DoList.h ********************************
 *  #include "C_Dlg_DoList.h"                                                     *
 * ...............................................................................*
 *   Project   :  MedinTux  (typographie made in france)                          *
 *   Copyright : (C) 2004-2005-2006-2007-2008-2009-2010 and for the eternity      *
 *   by        :  Sevin Roland     from   MedinTux project                        *
 *   E-Mail    : roland-sevin@medintux.org                                        *
 *   Web site  : www.medintux.org                                                 *
 * ...............................................................................*
 *                                                                                *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat ?  l'Energie Atomique                                           *
 *   - CEA,                                                                       *
 *                            31-33 rue de la F�d�ration, 75752 PARIS cedex 15.   *
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

#ifndef C_DLG_DOLIST_H
#define C_DLG_DOLIST_H

#include "ui_C_Dlg_DoList.h"
#include <QStringList>
 #include <QKeyEvent>
//==================================== C_Dlg_DoList ===============================================
class C_Dlg_DoList : public QDialog
{
    Q_OBJECT


public:
   enum mode {  SingleSelection = 0,
                MultiSelection  = 1
             };

    C_Dlg_DoList(const QStringList &list,  const QString& sep = "|", int posToHide  = 0, int selectionMode=C_Dlg_DoList::SingleSelection, QWidget *parent = 0);
    QStringList  getListMultiSelectedResult();
    QString      getStringMultiSelectedResult(QString sep = "");
    QString      getStringSingleSelectedResult();
protected:
    void         changeEvent(QEvent *e);
protected slots:
    void Slot_FiltrerListe( const QString &textFilter);
    void Slot_treeWidget_Choix_DoubleClicked( QTreeWidgetItem * , int);
    void Slot_pushButton_Ok_clicked();
    void Slot_pushButton_Cancel_clicked();
private:
    Ui::C_Dlg_DoList  m_ui;
    QString           m_resultString;
    QStringList       m_resultList;
    QStringList       m_inputList;
    int               m_posToHide;
    QString           m_sep;
    int               m_selectionMode;
};

#endif // C_DLG_DOLIST_H

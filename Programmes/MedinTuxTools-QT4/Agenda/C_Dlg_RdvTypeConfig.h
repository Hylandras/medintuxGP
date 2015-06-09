 /********************************* C_Dlg_RdvTypeConfig.h *************************
 *  #include "C_Dlg_RdvTypeConfig.h"                                              *
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
 *   Commissariat �  l'Energie Atomique                                            *
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

#ifndef C_DLG_RDVTYPECONFIG_H
#define C_DLG_RDVTYPECONFIG_H

#include "ui_C_Dlg_RdvTypeConfig.h"
#include "CMoteurAgenda.h"

#define TYP_ICON 0
#define TYP_NAME 1
#define TYP_COUL 2
#define TYP_TIME 3

namespace Ui {
    class C_Dlg_RdvTypeConfig;
}
class C_RendezVous;
class C_RendezVousList;
//====================================== C_Dlg_RdvTypeConfig ==========================================================
//-------------------------------------- C_Dlg_RdvTypeConfig ----------------------------------------------------------
class C_Dlg_RdvTypeConfig : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(C_Dlg_RdvTypeConfig)
public:
    enum menuMode {
        Long,
        Short
    };
    explicit C_Dlg_RdvTypeConfig(MAP_COLOR* pColorProfils, CMoteurAgenda* pCMoteurAgenda, QWidget *parent = 0, C_RendezVous *pC_RendezVous=0, int isNew=0);
    int  m_IsNew;
    Ui::C_Dlg_RdvTypeConfig *m_ui;
protected:
    virtual void     changeEvent(QEvent *e);
    int              initRdvTypeList();
    void             initListePatient( const QString & qstr_nom, const QString & qstr_prenom); // CZA
    QTreeWidgetItem *getSelectedListViewItem(QTreeWidget *pQTreeWidget);
    QMenu           *makeMenuDuree(menuMode mode = C_Dlg_RdvTypeConfig::Long);
private slots:
    void Slot_treeWidget_RdvTypeList_Clicked( QTreeWidgetItem *pQTreeWidgetItem , int c);
    void Slot_treeWidget_RdvTypeList_DoubleClicked( QTreeWidgetItem *pQTreeWidgetItem , int c);
    void Slot_pushButton_Quitter_clicked();
    void Slot_pushButton_Ok_clicked();
    void Slot_pushButton_Moins_clicked();
    void Slot_pushButton_Plus_clicked();
    void Slot_pushButton_Anonymize_clicked();
    void Slot_pushButton_Heure_clicked();
    void Slot_pushButton_Duree_clicked();
    void Slot_menuHeure_hovered (QAction *pQAction);
    void Slot_createTreeViewPatients ();                                    // CZA
    void Slot_lineEdit_Nom_textChanged(const QString &);                    // CZA
    void Slot_lineEdit_Prenom_textChanged(const QString &);
    void getListePatient();
    //void Slot_lineEdit_Nom_keyPressEvent(QKeyEvent *, int &);
    void Slot_TreeWidgetPatients_DoubleClicked( QTreeWidgetItem * , int);   // CZA
    void Valid_Patient_Selected(QTreeWidgetItem *pQTreeWidgetPatientItem);  // CZA
    void keyPressEvent ( QKeyEvent * event );                               // CZA

private:
    CMoteurAgenda           *m_pCMoteurAgenda;
    MAP_COLOR               *m_pColorProfils;
    C_RendezVous            *m_pC_RendezVous;
    QAction                 *m_pLastQActionHovered;

    QFrame                  *m_pQFrameListPatients;              //CZA
    QTreeWidget             *m_pTreeWidgetPatients;              //CZA
    QLabel                  *m_pQLabelStatus;

};

#endif // C_DLG_RDVTYPECONFIG_H

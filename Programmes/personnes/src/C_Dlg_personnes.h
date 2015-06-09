/********************************* C_Dlg_personnes.h ******************************
 *  #include "C_Dlg_personnes.h"         QT4-ready                                *
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

#ifndef C_DLG_PERSONNES_H
#define C_DLG_PERSONNES_H

#include <QtGui/QDialog>
#include <QStringList>
#include <QComboBox>
#include <QProcess>
#include <QTreeWidgetItem>
namespace Ui
{
    class Dlg_personnes;
}
//============================= C_Dlg_personnes =================================
class C_Dlg_personnes : public QDialog
{
    Q_OBJECT

public:
    C_Dlg_personnes(QWidget *parent = 0);
    ~C_Dlg_personnes();
private:
    QStringList       initListesTypesTel();
    void              InitAllComboTelType();
    void              InitComboSpec();
    int               selectMedecinById(const QString &start_ID);
    int               IsThisValueInComboBox(QComboBox *pQComboBox, const QString& value);
    int               setComboBoxOnValue(QComboBox *pQComboBox, const QString& value);
    QTreeWidgetItem*  initListesMedecin(int index, QString q_nom);
    QTreeWidgetItem*  initListesMedecin( QString spec, QString nom);
    QString           GetNbRecord(const QString &table);
    void              ItemToForm( const QString& pk );
    QString           FormToDisk(QString pk );
    QTreeWidgetItem*  getFirstSelectedItem(QTreeWidget *pQTreeWidget);
    QTreeWidgetItem*  getSelectedMedecinById(const QString &start_ID);
    void              setExchangeFile();
    void              EraseRecord( const QString &pk);
    void              ClearSaisie();
    QString           DoPopupList(QLineEdit *pQLineEdit, QStringList &list);
    void              tryToSetCodePostalFromVille();
    void              tryToSetVilleFromCodePostal();
    void              tryToStopAPropos();
    void              recordPositions();
    void              getAndSetPositions();
private:
    Ui::Dlg_personnes *m_pUI;
    QStringList        m_ListeMedecins;
    QString            m_LastPk;
    QProcess          *m_Apropos_Proc;
private slots:
    virtual void accept();
    virtual void reject();
    void         Slot_ActivateMainWindow();
    void         Slot_pushButton_Apropos_clicked();
    void         Slot_actionAproposDisplay();
    void         Slot_Apropos_Proc_finished (int,  QProcess::ExitStatus);
    void         Slot_FiltrerListe(const QString &text);
    void         Slot_comboBox_FiltreListe_currentIndexChanged(int index);
    void         Slot_treeWidget_ListeMedecins_itemClicked( QTreeWidgetItem *pQTreeWidgetItem , int);
    void         Slot_treeWidget_ListeMedecins_itemSelectionChanged();
    void         Slot_pushButtonOK_clicked();
    void         Slot_treeWidget_ListeMedecins_itemDoubleClicked( QTreeWidgetItem *pQTreeWidgetItem, int c );
    void         Slot_quitPushButton_clicked();
    void         Slot_pushButton_Effacer_clicked();
    void         Slot_pushButton_NouvelleFiche_clicked();
    void         Slot_pushButton_Enregistrer_clicked();
    void         Slot_OutFocusVille(QFocusEvent*, int &);
    void         Slot_OutFocusCodePostal(QFocusEvent*, int &);
    void         Slot_CharEventCodePostal(QKeyEvent *, int &);

};

#endif // C_DLG_PERSONNES_H

/********************************* C_Dlg_FseDefine.h *****************************
\C_Dlg_FseDefine.h
\brief (put some short descripion here).

    #include "C_Dlg_FseDefine.h"
    Project: MedinTux
    Copyright (C) 2003 2004 2005 2006 2007
    by Sevin Roland  and Data Medical Design
    E-Mail: medintux@medintux.org

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



#ifndef C_DLG_FSEDEFINE_H
#define C_DLG_FSEDEFINE_H

#include <qlistbox.h>
#include <qlistview.h>
#include <qdatetime.h>

#include "ui/Dlg_FseDefine.h"
#include "CFseVitaleApp.h"
#include "../../SesamVitale-QT3/C_Cps.h"
#include "../../SesamVitale-QT3/C_Vitale.h"

class C_Dlg_FseDefine : public Dlg_FseDefine
{
  Q_OBJECT

public:
  C_Dlg_FseDefine(CFseVitaleApp* pCFseVitaleApp, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  ~C_Dlg_FseDefine();

  CFseVitaleApp* m_pCFseVitaleApp;
  C_Cps*         m_pC_Cps;
  C_Cps*         m_pC_CpsRempla;
  C_Vitale*      m_pC_Vitale;
  QString        m_CaisseFileData;
  QString        m_CorrespTableData;
  /*$PUBLIC_FUNCTIONS$*/
  void    setEnabledReadState(bool state);
  void    setSecurisationState(int state);
  QDate   DateTimeStr_jjMMyyyy_toDate(const QString & dateTime );
  QString getDateFSE();
  QString getDateAT(QDate *pQDate  = 0 );
  QString DateATControl();
  void    GroupToListView(QListView *pQListView, const QString &serializeData);
  QString CPS_To_Html_Display(C_Cps *pC_Cps );
  void    setComboOnValue(QComboBox* pQComboBox, const QString &value);
  void    clear_AMC_View();
  void    clear_MUT_View();
  QString MakeSesamAMC_String();
  QString GetIndicTraitement();
  QString DoCalendrier(const QString titre, QString date);
  void    C_CPS_ToInterface(const QString &data_CPS );
  void    C_Vitale_ToInterface(const QString data_Vitale);
  void    init_listBox_ActesDispos();
public slots:
  /*$PUBLIC_SLOTS$*/

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/

  virtual void          reject();
  virtual void          accept();
  void pushButtonSaveCPS_Titulaire_clicked();
  void pushButtonLoadCPS_Titulaire_clicked();
  void pushButton_Annuler_clicked();
  void pushButton_OK_clicked();
  void pushButton_Vitale_clicked();
  void pushButton_CPS_clicked();
  void pushButtonFormater_clicked();
  void pushButton_FSE_Calendrier_clicked();
  void pushButton_AT_DateDebut_clicked();
  void pushButton_AT_DateFin_clicked();
  void listBox_Vitale_clicked(QListBoxItem *pQListBoxItem);
  void listView_FseGroup_doubleClicked(QListViewItem *pQListViewItem, const QPoint &, int);
  void lineEdit_Nbr_ArretTravail_textChanged ( const QString &text );
  void lineEdit_Nbr_ArretTravail_lostFocus();
  void comboBox_AMC_IndicTTT_activated(int);
  void lineEdit_AMC_CodeRoutage_textChanged(const QString& text);
  void lineEdit_AMC_IdHote_textChanged(const QString& text);
  void lineEdit_AMC_NomDomaine_textChanged(const QString& text);
  void lineEdit_AMC_AiguillageSTS_textChanged(const QString& text);
  void lineEdit_MUT_Numero_lostFocus();
  void toolBox_PEC_currentChanged(int);
  void checkBox_ExoTicketMod_clicked_Non ();
  void checkBox_AccParTiers_clicked_Non ();
  void checkBox_ExoTicketMod_clicked_Oui ();
  void checkBox_AccParTiers_clicked_Oui ();
  void pushButton_ReadFileVitale_clicked();
  void ATMP_SetDate();
  void AccParTiers_SetDate();
  void Maternite_SetDate();
  void pushButton_WhatIsIt_clicked ();
  void listBox_ActesDispos_onItem ( QListBoxItem *pQListBoxItem);
  void radioButton_GestionUnique_StateChanged (int state );
  void radioButton_GestionSeparee_StateChanged (int state );
  void pushButton_DoVirtual_clicked();
  void pushButton_SaveVirtual_clicked();
};

#endif


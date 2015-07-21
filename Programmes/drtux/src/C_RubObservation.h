/*********************************** C_RubObservation.h ***************************
 *                                                                                *
 *   #include "C_RubObservation.h"                                                *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat à l'Energie Atomique                                            *
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

#ifndef C_RUBOBSERVATION_H
#define C_RUBOBSERVATION_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>
#include <qsqldatabase.h>
#include <qimage.h>

#include <qcolor.h>
#include <qbrush.h>
#include <stdlib.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

#include <qiconset.h>
#include <qsplitter.h>
#include <qiconview.h>
#include <qdict.h>


#include "CMoteur_Base.h"
#include "MyEditText.h"
#include "ui/DlgDateNom.h"
#include "../../MedinTuxTools/CHtmlTools.h"
#include "CApp.h"
#include "../../MedinTuxTools/CPrtQListBoxItem.h"
#define Min(a, b)             ((a) < (b) ? (a) : (b))
#define Max(a, b)             ((a) > (b) ? (a) : (b))

class C_RubObservation :public QWidget
{
  Q_OBJECT

public:
  C_RubObservation(CMoteurBase* pCMoteurBase, RUBREC_LIST  *pRubList,
                   const char* num_GUID, const char* id_doss, const char* dossNom, const char* dossPrenom, const char* user,
                   QWidget* parent , const char* name , QString prDroits, WFlags fl = 0 );
  ~C_RubObservation();
      /*$PUBLIC_FUNCTIONS$*/
      virtual void          GetRubDateName(QString & name, QString & date);
      virtual void          SetRubDateName(const QString & name, const QString & date);
      virtual int           SetCurentDocByPrimkey(const QString & primKey);
      virtual RUBREC_LIST::iterator Current_RubList_Iterator();
      virtual int           Current_RubList_ID();
      virtual int           IfModified_SaveInRubList();
      virtual long          initComboListHistoric(void);
      virtual void          clearDocument();
      virtual int           initData();

        int          m_IsModifiable;
        QString      m_User;
        QString      m_DossNom;
        CMoteurBase* m_pCMoteurBase;
        QString      m_NumGUID;
        QString      m_ID_Doss;
        QString      m_DossPrenom;
        RUBREC_LIST *m_pRubList;
        int          m_LastRub;
        int          m_RubType;
        QString      m_PrefixDroit;

    //............................... traitement des images ............................................

    QString m_InsertMask;

    //............................. ajout à la forme ..................................................
    QSplitter          *m_splitterVertical;
    QIconView          *m_iconViewDocuments;
    MyEditText         *m_pMyEditText;
    QToolBar           *m_pQToolBar_Combo;
    QToolBar           *m_pQToolBar_Button;
    int                 m_MaxImageWidth;
    QPushButton*        textLabel_NbRub;
    QComboBox*          comboBox_RubName;
    QPushButton*        pushButtonRubDateChange;
    QLabel*             textLabel1;
    QLabel*             textLabel_UserName;
    QPushButton*        pushButtonSave;
    QPushButton*        pushButtonPrint;
    QPushButton*        pushButtonDelete;
    QPushButton*        pushButtonAdd;
    QPushButton*        pushButtonExporter;
    QGridLayout*        FormRubDocumentsLayout;

    protected slots:
      virtual void languageChange();

    public slots:
      /*$PUBLIC_SLOTS$*/
      virtual void          SaveTextInRubList();
      virtual void          pushButtonRubDateChange_clicked();
      virtual void          pushButtonSave_clicked();
      virtual void          pushButtonDelete_clicked();
      virtual void          comboBox_RubName_activated(int item);
      virtual void          comboBox_RubName_highlighted(int item);
      virtual void          FormPatientList_destroyed(QObject *);
      virtual void          SetModifiableState(int state);
      virtual void          On_buttonDragClicked();

    protected:
      /*$PROTECTED_FUNCTIONS$*/

    protected slots:
      /*$PROTECTED_SLOTS$*/

    signals:
        void Sign_SaveButtonClicked();
  };

#endif


//
// C++ Interface:
//
/********************************* C_RubIdentite.h *****************************************

    Project: MedinTux
    Copyright (C) 2005 by Roland Sevin  and Data Medical Design
    E-Mail: medintux@medintux.org

    This program is free software; you can redistribute it and/or modify
    it under the terms of the CeCILL Version 1 du 21/06/2004
    License (GPL compatible)
                               http://www.cecill.info/
    as published by :

    Commissariat � l'Energie Atomique
    - CEA,
                             31-33 rue de la F�d�ration, 75752 PARIS cedex 15.
                             FRANCE
    Centre National de la Recherche Scientifique
    - CNRS,
                             3 rue Michel-Ange, 75794 Paris cedex 16.
                             FRANCE
    Institut National de Recherche en Informatique et en Automatique
    - INRIA,
                             Domaine de Voluceau, Rocquencourt, BP 105, 78153
                             FRANCE

          This program is distributed in the hope that it will be useful,
          but WITHOUT ANY WARRANTY; without even the implied warranty of
          MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
          CeCILL  License (GPL compatible) for more details.

*/
#ifndef C_RUBIDENTITE_H
#define C_RUBIDENTITE_H

#include <qiconset.h>
#include <qsplitter.h>
#include <qiconview.h>
#include <qlayout.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qdict.h>

#include "ui/RubIdentite.h"
#include "CMoteur_Base.h"
#include "MyEditText.h"
#include "../../MedinTuxTools/CPrtQListBoxItem.h"
#include "../../MedinTuxTools/C_ImageList.h"
#include "CMoteur_Base.h"
#include "CApp.h"
#include "../../MedinTuxTools/ThemePopup.h"
#include "../../MedinTuxTools/Theme.h"

//=============================================== DEFINE =========================================================


#define Min(a, b)             ((a) < (b) ? (a) : (b))
#define Max(a, b)             ((a) > (b) ? (a) : (b))


//====================================== C_RubDocuments =======================================================

class C_RubIdentite : public FormRubIdent
{
  Q_OBJECT

public:
  C_RubIdentite(CMoteurBase* pCMoteurBase, RUBREC_LIST  *pRubList,
                 const char* num_GUID, const char* id_doss, const char* dossNom, const char* dossPrenom, const char* user,
                 QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
  ~C_RubIdentite();
  /*$PUBLIC_FUNCTIONS$*/

  RUBREC_LIST::iterator  Current_RubList_Iterator();
  int                    Current_RubList_ID();
  int                    IfModified_SaveInRubList();
  long                   initComboListHistoric( void );
  int                    SetCurentDocByPrimkey(const QString &pk);
  void                   initData();
  MyEditText*            GetCurrentEditor();
  void                   setDefaultWordWrapFromIniFile();
  QString                DoPopupList( QStringList & list ,  int x ,int y);
  QPixmap*               decodeStringTo_pQPixmap( const QString& in );
  QPixmap                decodeStringTo_QPixmap( const QString& in );
  QString                encodePixmapToBase64( const QPixmap& pixIn);
  QString                encodePixmapToBase64( QPixmap* pQPixmap);
  void                   ImageListClear();
  void                   GetImageListToDelete(QStringList &imageNameList, int key);
  int                    refImageCount(const QString &text, const QString &image);
  void                   refImageReinit(const QString &qs_text);
  void                   SetDefaultMaskFile( QString path );
  void                   clearDocument();
  void                   SaveTextInRubList();
  void                   GetRubDateName( QString &name, QString &date );
  QString                ComputeClef_Secu(const char *txt);
  int                    DataToFormulaire( QString &refToPrimkeyDoss );
  QString                ToNum(QString in);

    int            m_IsModifiable;
    QString        m_User;
    QString        m_DossNom;
    CMoteurBase*   m_pCMoteurBase;
    QString        m_NumGUID;
    QString        m_ID_Doss;
    QString        m_DossPrenom;
    RUBREC_LIST   *m_pRubList;
    int            m_LastRub;
    int            m_MaxImageWidth;
    MyEditText    *m_pMyEditText;
    QString        m_InsertMask;

signals:
    void Sign_IdentChange(const char*, const char*, const char*, const char* );
    void Sign_IdentModified(const char*, const char*, const char*, const char* );
    void Sign_PatientListMustBeUpdated(const char*, const char*, const char*, const char* );
    void Sign_Ident_KillMe();
    void Sign_NoteToBeSynchronized(int, const char* );

    void Sign_SaveButtonClicked();
    void Sign_RubChanged(RUBREC_LIST::iterator);


public slots:
  /*$PUBLIC_SLOTS$*/
    virtual void          CheckIfImageIsToRemoveFromIconView(int key);
    virtual void          On_Signal_Del_Before();
    virtual void          On_Signal_Del_After();
    virtual void          On_Signal_Paste_After();
    virtual void          SetRubDateName( const QString & name, const QString & date );
    virtual void          comboBox_RubName_activated( int item );
    virtual void          comboBox_RubName_highlighted( int item );
    virtual void          OnMenuActivated( int id );
    virtual void          On_pushButtonSave();
    virtual void          pushButtonRubDateChange_clicked();
    virtual void          pushButtonDelete_clicked();
    virtual void          SetModifiableState( int state );
    virtual void          listView_Praticiens_contextMenuRequested( QListViewItem *pQListViewItem, const QPoint &, int );
    virtual void          OnActivated_comboBoxQualiteAyantDroit( int qualiteAssure);
    virtual void SetInterfaceOnDroits( const QString & droits );
    virtual void OnOutFocuslineEdit_DtNss();
    virtual void OnInFocusNumSecu();
    virtual void OnOutFocusNumSecu();
    virtual void OnOutFocusVille();
    virtual void OnOutFocusCodePostal();
    virtual void GetPopupItem();
    virtual void OnCharEventCodePostal( QEvent * );
    virtual void OnCharEventNumSecu( QEvent * );
    virtual void SetData();
    virtual void SetData( int flag );
    virtual void ClearDialog();
    virtual void pushButtonCreate_clicked();
    virtual void pushButtonSavModif_clicked();
    virtual void SetFieldListData( PtrListIdentFieldData & list_field );
    virtual void pushButtonCancel_clicked();
    virtual void listView_Praticiens_doubleClicked( QListViewItem * pQListViewItem );
    virtual void pushButton_AddMedTTT_clicked();
    virtual void exeAnnuaire( QString idInterv );
    virtual void pushButton_DelMedTTT_clicked();
    virtual void pushButton_ViewNotes_clicked();
    virtual void synchronizeNoteWith( const char * txt );
    virtual void textEdit_Note_textChanged();
    virtual void On_buttonDragClicked();
protected:
    int         m_Contacts_Run;
    int         m_NoteViewState;
    QString     m_CurrentPopupItem;
    QString     m_RetPopupItem;
    ThemePopup *m_pThemePopup;

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/

};

#endif


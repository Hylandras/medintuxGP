
// C++ Interface:
//
/********************************* C_FormGlossaire.h *****************************************

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
#ifndef C_FormGlossaire_H
#define C_FormGlossaire_H

#include <qvariant.h>
#include <qiconset.h>
#include <qsplitter.h>
#include <qiconview.h>
#include <qlayout.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qdict.h>
#include <qwidget.h>
#include <qlistview.h>

#include "ui/FormGlossaire.h"
#include "../../MedinTuxTools/CPrtQListBoxItem.h"
#include "../../../MedinTuxTools/CPrtQListViewItem.h"
#include "../../MedinTuxTools/ThemePopup.h"
#include "../../MedinTuxTools/Theme.h"
#include "CMoteurGlo.h"
#include "Rubriques.h"


//=============================================== DEFINE =========================================================


#define Min(a, b)             ((a) < (b) ? (a) : (b))
#define Max(a, b)             ((a) > (b) ? (a) : (b))


//====================================== C_FormGlossaire =======================================================
/*! \class FormGlossaire
  * Gestion du glossaire dans le dock.
  */

class C_FormGlossaire : public FormGlossaire
{
 Q_OBJECT
 
public:
  C_FormGlossaire(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
  ~C_FormGlossaire();

    QSplitter*         m_splitter;
    C_DragQListView*   m_ListView_Glossaire;
    CW_EditText*       m_textEditView;

    virtual QString GetCurrentFileName();
    virtual ThemePopup * DoGlossairePopupMenu( QString filepath );

   long GetMasquesList(  QListView *pQlistView , const QString &folder_name);
   void SetTextEditWithData(   QString file ,  CW_EditText *pQTextEdit);
   int  SetListTyp( int typ , const QString &path, QListView *pQlistView =0  );
   int  PathGlossaireToTyp();
   void dragClose();
   QString  GetCurrentRubName();
   void GetCurrentPath(QString &path, QString &defaultMaskDirectory);
  enum ItemType { File = 0, Dir, Link};
public slots:
    virtual void setInterfaceOnPermisions( const QString & droits );
    virtual void OnGlossaireRubriqueActived( QWidget * pQwdgRub );
    virtual void Slot_RestaureConnection();
    virtual void Slot_GlossaireRefresh();
    virtual void SetCurrentPath( const QString &path , const QString &defaultMaskDirectory = "");
    virtual void pushButtonBack_clicked();
    virtual void pushButtonHome_clicked();
    virtual void pushButtonMenu_clicked();
    virtual void pushButtonNewDir_clicked();
    virtual void pushButtonDelDir_clicked();
    virtual void On_mouseButtonClicked (  int button, QListViewItem * item, const QPoint & pos, int c );
    virtual void ListView_Glossaire_clicked();
    virtual void ListView_Glossaire_clicked( QListViewItem * qlistViewItem );
    virtual void ListView_Glossaire_doubleClicked( QListViewItem * qlistViewItem );
    virtual void ListView_Glossaire_returnPressed( QListViewItem * qlistViewItem );
    virtual void ListView_Glossaire_selectionChanged( QListViewItem * qlistViewItem );
    virtual void pushButtonEdit_clicked();
    virtual void ListView_Glossaire_contextMenuRequested( QListViewItem * pQListViewItem, const QPoint &, int );
    virtual void but_ConfFiltre_HideShow_clicked();
    virtual void Slot_PopupActionDumy();
    virtual void Slot_PopupActionNewModele();
    virtual void Slot_PopupActionNewDoc();
    virtual void Slot_PopupActionAddDoc();
    virtual void Slot_PopupActionReplaceDoc();
    virtual void Slot_PopupActionEditDoc();
    virtual void Slot_PopupActionRenameDoc();
    virtual void Slot_PopupActionDeleteDoc();
    virtual void Slot_PopupActionRenameDir();
    virtual void Slot_pushDefault_clicked();
    virtual void OnMenuActivated( int id );
    virtual void OnMenuHighlighted( int id );
    virtual void OnMenuAboutToHide();
    virtual void On_Sign_MouseEnter(QString&, QString&);
    virtual void On_Sign_MouseLeave(QString&, QString&);
signals:
    void Sign_FileSelected(const char*, int);
    void Sign_FileToEdit(QString, QString, int);
    void Sign_FileChanged(const char*, int);
    void Sign_DirectoryChanged(const QString&, const QString&);
protected:

    ThemePopup* m_pThemePopup;
    QString     m_PopupItem;
    QString     m_PopupFilePath;
    QDir        m_QDir;
    QString     m_DefaultMaskDirectory;
    QString     m_curPath;
    int         m_RubType;
    QListViewItem * m_pLastQListViewItem;
};

#endif // FORMGLOSSAIRE_H


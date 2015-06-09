/********************************* CMDI_Generic.h *********************************
 *                                                                                *
 *   #include "CMDI_Generic.h"                                                    *
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
 #ifndef CMDI_GENERIC_H
 #define CMDI_GENERIC_H

 #include "CMoteur_Base.h"
 #include <qmainwindow.h>
 #include <qobject.h>
 #include <stdlib.h>

 #include "CApp.h"
 #include "CRubList.h"
 #include "MyEditText.h"

 class CPrtQListBoxItem;
 class C_FormGlossaire;
 //====================================== CMDI_Generic =======================================================
 class CMDI_Generic : public QMainWindow
 {
     Q_OBJECT

 public:

    enum SetPos { CFG_PARAM=1, RECT_PARAM=2 };

    //static  QString      S_GetRubName(){return QString("Generic");};
    //static  int          S_GetType() {return 0;};

    CMDI_Generic(QWidget* parent, const char* name, int wflags, CMoteurBase *pCMoteurBase, RUBREC_LIST  *pRubList, int type);
    virtual int          GetType();
    virtual void         SetType(int type);
    virtual QString      GetRubName(){return m_RubName;}
    virtual void         SetRubName(const QString &rubName){m_RubName=rubName;}
    virtual void         SetPrintableState(bool state){m_IsPrintable = state;};
    virtual bool         IsPrintable(){return m_IsPrintable;};
    virtual bool         IsModifiable(){return FALSE;}
    //void                 setGeometry(QRect rect, int flag=1);
    virtual void         setGeometry(QRect rect, int flag=CMDI_Generic::CFG_PARAM|CMDI_Generic::RECT_PARAM);
    virtual void         saveGeometry();
    virtual QString      GetDefaultMaskDirectory();
    virtual QString      GetCurrentMaskDirectory();
    virtual QString      GetDefaultMaskFile();
    virtual int          GetCurrent_RubList_ID(){return -1;};
    virtual RUBREC_LIST::iterator Current_RubList_Iterator(){return 0;};
    virtual void         IfModified_SaveInRubList(){return ;};
    virtual int          initData(){return 0;};
    virtual int          SetCurentDocByPrimkey(const QString &/*pk*/){return -1;};
    virtual MyEditText  *GetCurrentEditor(){return 0;};
    virtual void         setDataToModifiedState(){return;};
    virtual void         SetInterfaceOnDroits(const QString &/*droits*/){return ;};
    virtual void         DoActionWithThisDocument(const QString &action, int typ, const char *path =0, const QDateTime *pQDateTime=0, const QString *libelle=0, const QString *subType=0);
    virtual CRubRecord*  AddNewDocument(const QString &strData, int typ, const char *path =0, const QDateTime *pQDateTime=0, const QString *libelle=0,  const QString *subType=0, int noInquire =0);
    virtual void         ChangeCurrentComboBoxItemName(const QString &name);
    virtual int          reinitComboBoxWithRubList(RUBREC_LIST  *, const QString & =""){return -1;};
    virtual CPrtQListBoxItem    *GetCurrentDocItem();
    virtual QComboBox    *GetDocListComboBox(){return 0;}
    virtual void          setSaveButtonVisible(int /*state*/ ){};
 public slots:
    void                 ActiverRubrique(const char *rubName,   int mode);
 public:
    CMoteurBase     *m_pCMoteurBase;
    QString          m_RubName;
    RUBREC_LIST     *m_pRubList;
    QString          m_PrefixDroit;
    bool             m_IsModifiable;
 signals:
    void Sign_Print();
//........................ kit de connexion au glossaire ............................................
public:
    virtual void DoConnexionOnGossaire(C_FormGlossaire *pC_FormGlossaire);
protected:
    virtual void    mouseEnter ();
    virtual void    mouseLeave ();
    bool    eventFilter ( QObject * o, QEvent * e );
    QString m_PathDocuments;
    QString m_DefaultMaskDirectory;
signals:
    void Sign_MouseEnter(QString & ,  QString &);
    void Sign_MouseLeave(QString & ,  QString & );
public slots:
    virtual void On_DirectoryGlossaireChanged(const QString &pathDocuments, const QString &defaultMaskDirectory);
//................ autres .....................................................................
 private:
    int              m_Type;
    bool             m_IsPrintable;
 };

 #endif


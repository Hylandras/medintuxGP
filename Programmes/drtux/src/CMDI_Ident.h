/********************************* CMDI_Ident.h ***********************************
 *                                                                                *
 *   #include "CMDI_Ident.h"                                                      *
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


#ifndef CMDI_IDENT_H
#define CMDI_IDENT_H

#include "CMDI_Generic.h"
#include "C_RubIdentite.h"

//====================================== CMDI_Ident =======================================================
class CMDI_Ident : public CMDI_Generic
{

    Q_OBJECT
public:
    static  QString      S_GetRubName(){return tr("Identification");};
    static  int          S_GetType() {return 20090000;;};      //#define TYP_IDENTITE       1

    CMDI_Ident( QWidget* parent, const char* name, int wflags , CMoteurBase *pCMoteurBase, RUBREC_LIST  *pRubList,
                const char* num_GUID, const char* id_doss, const char* dossNom, const char* dossPrenom, const char* user, int type);
   ~CMDI_Ident();
    int          GetCurrent_RubList_ID();
    RUBREC_LIST::iterator Current_RubList_Iterator();
    int          initData();
    void         IfModified_SaveInRubList();
    bool         IsModifiable(){return m_pFormRubIdent->m_IsModifiable;}
    int          SetCurentDocByPrimkey(const QString &pk);
    MyEditText  *GetCurrentEditor();
    void         SetInterfaceOnDroits(const QString &droits);
    void         DoActionWithThisDocument(const QString &verbe, int typ, const char *path =0, const QDateTime *pQDateTime=0, const QString *libelle=0, const QString *subType=0);
    CRubRecord*  AddNewDocument(const QString &strData, int typ, const char *path =0, const QDateTime *pQDateTime=0, const QString *libelle=0, const QString *subType=0, int noInquire =0);
    void         ChangeCurrentComboBoxItemName(const QString &name);
    //----------------------------- GetDocListComboBox -----------------------------------
    /*! \brief retourne la combobox affectee a al liste des documents */
    QComboBox    *GetDocListComboBox();
    QString              GetDefaultMaskFile();
    void                 setSaveButtonVisible(int state );
    int     reinitComboBoxWithRubList(RUBREC_LIST  *pRubList, const QString &pkToDisplay ="");

public slots:
    void RubOnUserChange(const char*user, const char*sign_user);
    //............. appelé par DrTux pour mettre à jour les ..................
    //              données en fonction de l'identité du patient
    void OnIdentChange(const char* num_GUID,          const char* id_doss,
                       const char* dossNom ,          const char* dossPrenom );
    void SetModifiableState(int state);
    void OnPrint();
    void On_pushButtonAppend();
    void Slot_OnGlossaireFileClicked(const char *file , int typ_rub);
    void ActiverRubrique(const char *rubName, int mode);

public:
    C_RubIdentite       *m_pFormRubIdent;

signals:
    void message(const QString&, int );
    void Sign_RubIsDeleted(const char* name);

private:

};
#endif   // CMDI_IDENT_H

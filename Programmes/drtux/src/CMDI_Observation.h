/********************************* CMDI_Observation.h ****************************
 *                                                                                *
 *   #include "CMDI_Observation.h"                                               *
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


#ifndef CMDI_OBSERVATION_H
#define CMDI_OBSERVATION_H

#include "CMDI_Generic.h"
#include "C_RubObservation.h"

class QComboBox;
class ThemePopup;

//====================================== CMDI_Observation =======================================================
class CMDI_Observation: public CMDI_Generic
{
    Q_OBJECT
public:
    //static  QString      S_GetRubName(){return QString("Observation");};
    //static  int          S_GetType() {return 20030000;};      //TYP_OBSERVATION    20030000

    CMDI_Observation( QWidget* parent, const char* name, int wflags , CMoteurBase *pCMoteurBase, RUBREC_LIST *pRubList,
                      const char* num_GUID, const char* id_doss,  const char* dossNom, const char* dossPrenom, const char* user, int type, QString prDroits);
   ~CMDI_Observation();
    int          GetCurrent_RubList_ID();
    RUBREC_LIST::iterator Current_RubList_Iterator();
    int          initData();
    void         IfModified_SaveInRubList(void);
    bool         IsModifiable(){return m_pC_RubObservation->m_IsModifiable;}
    int          SetCurentDocByPrimkey(const QString &pk);
    MyEditText  *GetCurrentEditor();
    void         SetInterfaceOnDroits(const QString &droits);
    void         DoActionWithThisDocument(const QString &verbe, int typ, const char *path =0, const QDateTime *pQDateTime=0, const QString *libelle=0, const QString *subType=0);
    CRubRecord*  AddNewDocument(const QString &strData, int typ, const char *path =0, const QDateTime *pQDateTime=0, const QString *libelle=0, const QString *subType=0, int noInquire =0);
    void         setDataToModifiedState();
    void         SetZoom(int zoom);
    void         setWordWrap(int width);
    void         ChangeCurrentComboBoxItemName(const QString &name);
    QString      GetDefaultMaskFile();
    void         setSaveButtonVisible(int state );
    int          reinitComboBoxWithRubList(RUBREC_LIST  *pRubList, const QString &pkToDisplay ="");
    //----------------------------- GetDocListComboBox -----------------------------------
    /*! \brief retourne la combobox affectee a al liste des documents */
    QComboBox    *GetDocListComboBox();

public:
    C_RubObservation    *m_pC_RubObservation;
    int                  m_CustomMenuRequestState;

public slots:
    void RubOnUserChange(const char*user, const char*sign_user);
    //............. appelé par DrTux pour mettre à jour les ..................
    //              données en fonction de l'identité du patient
    void OnIdentChange(const char* num_GUID,          const char* id_doss,
                       const char* dossNom,           const char* dossPrenom );
    void OnPrint(void);
    void SetModifiableState(int state);
    void Slot_ExeCommand(QString &command);
    void Slot_Get_Custom_Menu(const QString &, QString&, QString &, QKeySequence &);
    void On_pushButtonAppend();

signals:
    void message(const QString&, int );
    void Sign_RubIsDeleted(const char* name);

private:

};
#endif

/********************************* CMDI_ChoixPatient.h ****************************
 *                                                                                *
 *   #include "CMDI_ChoixPatient.h"                                               *
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


#ifndef CMDI_CHOIXPATIENT_H
#define CMDI_CHOIXPATIENT_H

#include "CMDI_Generic.h"
#include "ui/RubChoixPatient.h"

//====================================== CMDI_ChoixPatient =======================================================
class CMDI_ChoixPatient : public CMDI_Generic
{
    Q_OBJECT
public:
     static  QString      S_GetRubName(){return tr("Choix Patients");};
     static  int          S_GetType() {return 2;};      //#define TYP_CHOIX_PATIENT  2

     CMDI_ChoixPatient( QWidget* parent, const char* name, int wflags , CMoteurBase *pCMoteurBase, RUBREC_LIST  *pRubList,
	  	        const char* num_GUID, const char* id_doss, const char* dossNom, const char* dossPrenom, const char* user, int type);
    ~CMDI_ChoixPatient();
     int          GetCurrent_RubList_ID(){return -1;};
     void         IfModified_SaveInRubList(){};
     int          initData(){return 1;};
     int          SetCurentDocByPrimkey(const QString &/*pk*/){return -1;};
     MyEditText  *GetCurrentEditor(){return 0;};
     void         SetInterfaceOnDroits(const QString &droits);
     CRubRecord*  AddNewDocument(const QString &/*strData*/, int /*typ*/, const char * /*path =0*/, const QDateTime * /*pQDateTime=0*/, const QString * /*libelle=0*/, int ){return 0;};
     void         DoConnexionOnGossaire(C_FormGlossaire *);
     int          reinitComboBoxWithRubList(RUBREC_LIST  *, const QString & =""){return -1;}
     QComboBox    *GetDocListComboBox(){return 0;}
     virtual void    mouseEnter ();
     virtual void    mouseLeave ();

public slots:
    void RubOnUserChange(const char*user, const char*sign_user);
    //............. appelé par DrTux pour mettre à jour les ..................
    //              données en fonction de l'identité du patient
    void OnIdentChange(const char* num_GUID,  const char* id_doss,
                       const char* dossNom ,  const char* dossPrenom );

public:
    FormRubChoixPatient  *m_pFormRubChoixPatient;

signals:
    void message(const QString&, int );
    void Sign_RubIsDeleted(const char* name);

private:

};


#endif

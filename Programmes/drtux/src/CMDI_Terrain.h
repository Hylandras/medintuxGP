/********************************* CMDI_Terrain.h *********************************
 *                                                                                *
 *   #include "CMDI_Terrain.h"                                                    *
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


#ifndef CMDI_TERRAIN_H
#define CMDI_TERRAIN_H

#include "CMDI_Generic.h"
#include "ui/RubTerrain.h"

#define TERRAIN_DEF_VAR     "\n[VAR Constantes de Base]\n"\
                            "          Modèle = Date, Pouls, 31536000, 130, 40, pps, Tachycardie, Bradycardie\n"\
                            "          Modèle = Date, SAO², 31536000, 0, 92, %, , Hypoxie\n"\
                            "          Modèle = Date, T°, 31536000, 40.5, 35, °, Hyporthermie, Hyperthermie\n"\
                            "[VAR Poids]\n"\
                            "          Modèle = Date, Poids, 31536000, 100, 50, Kg(s), Obésité, Amaigrissement\n"



//====================================== CMDI_Terrain =======================================================
class CMDI_Terrain: public CMDI_Generic
{
   Q_OBJECT

public:
    static  QString      S_GetRubName(){return tr("Terrain");};
    static  int          S_GetType() {return 20060000;};      //#define TYP_TERRAIN               20060000

    CMDI_Terrain( QWidget* parent, const char* name, int wflags , CMoteurBase *pCMoteurBase, RUBREC_LIST *pRubList,
                  const char* num_GUID, const char* id_doss,  const char* dossNom, const char* dossPrenom, const char* user, int type);

   ~CMDI_Terrain();
    int          GetCurrent_RubList_ID();
    RUBREC_LIST::iterator Current_RubList_Iterator();
    int          initData();
    void         IfModified_SaveInRubList(void);
    int          SetCurentDocByPrimkey(const QString &pk);
    MyEditText  *GetCurrentEditor();
    void         SetInterfaceOnDroits(const QString &droits);
    RUBREC_LIST::iterator GetCurrent_RubList_IT();
    CRubRecord*  AddNewDocument(const QString &strData, int typ, const char *path =0, const QDateTime *pQDateTime=0, const QString *libelle=0, const QString *subType=0, int noInquire =0);
    void         setDataToModifiedState(){return;};
    QString GetPoids();
    QString GetTaille();
    QString GetSurface();
    void    SetPoids(QString poids);
    void    SetTaille(QString taille);
    void    SetPoidsTaille(QString poids, QString taille);
    void    ChangeCurrentComboBoxItemName(const QString &name);
    int     reinitComboBoxWithRubList(RUBREC_LIST  *pRubList, const QString &pkToDisplay ="");
    //----------------------------- GetDocListComboBox -----------------------------------
    /*! \brief retourne la combobox affectee a al liste des documents */
    QComboBox    *GetDocListComboBox();

public:
    FormRubTerrain*  m_pFormRubTerrain;
    void SetModifiableState(int state);

public slots:
    void RubOnUserChange(const char*user, const char*sign_user);
    //............. appelé par DrTux pour mettre à jour les ..................
    //              données en fonction de l'identité du patient
    void OnIdentChange(const char* num_GUID,          const char* id_doss,
                       const char* dossNom,           const char* dossPrenom );

signals:
    void message(const QString&, int );
    void Sign_RubIsDeleted(const char* name);
    //void Sign_Renouveler(QString &strData, int typ, const char *path );
    //void Sign_Prescrire(QString &strData, int typ,  const char *path );
private:


};



#endif




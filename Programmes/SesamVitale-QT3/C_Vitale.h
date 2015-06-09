/************************************** C_Cps.h ***********************************
 *                                                                                *
 *   #include"C_Vitale.h"                                                            *
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

#ifndef C_VITALE_H
#define C_VITALE_H

#include <qstring.h>
#include <qstringlist.h>
#include <qobject.h>
#include <qdatetime.h>
#define TR QObject::tr

//..................................... specifites selon plateforme et OS ......................................................
#if   defined(Q_WS_WIN)

#elif defined(Q_WS_X11)
   #define __declspec(dllexport)
#elif defined(Q_WS_MAC)

#endif

//............................................. quelques définitions ..........................................................
class QString;
//================================================= C_Organisme ==============================================================
class C_Organisme
{//"code régime";"caisse gestionnaire";"centre gestionnaire";"libellé;"régime";"organisme destinataire";"code centre informatique";"adresse 1";"adresse 2";"commune";"code postal";"cedex";"téléphone";"fax"
 //  90;100;0000;Caisse d assurance Vieillesse Invalidite et Maladie des cultes;Caisse d'Assurance Vieillesse Invalidité et Maladie des Cultes - CAVIMAC;100;004;119, rue du Président Wilson;;LEVALLOIS-PERRET CEDEX ;92309;;01 49 68 57 00;
 //  01;011;0000;BOURG EN BRESSE;Régime Général;011;016;Place de la Grenouillère;;BOURG en BRESSE;01015;CEDEX;08 20 90 41 22;04 74 45 84 90
 //  ^   ^   ^         ^              ^           ^   ^             ^            ^        ^         ^      ^           ^            ^
 //  |   |   |         |              |           |   |             |            |        |         |      |           |            |
 //  |   |   |         |              |           |   |             |            |        |         |      |           |           fax    (11)
 //  |   |   |         |              |           |   |             |            |        |         |      |        tél 1   (10)
 //  |   |   |         |              |           |   |             |            |        |         |   Cedex  (9)
 //  |   |   |         |              |           |   |             |            |        |    Code Postal (8)
 //  |   |   |         |              |           |   |             |            |      ville (7)
 //  |   |   |         |              |           |   |             |         Adresse 2 (6)
 //  |   |   |         |              |           |   |            Adresse 1  (5)
 //  |   |   |         |              |           |  code centre informatique (4)
 //  |   |   |         |              |   Organisme destinataire (3)
 //  |   |   |         |          Régime (2)
 //  |   |   |     libellé (1)
 //  |   | centre gestionnaire (0)
 //  | caisse gestionnaire    (Match 2)
 // code régime  (Match 1)
 public:
     C_Organisme(const QString &dataCaisse, QString codeRegime, QString caisseGest)
     :m_CodeRegime(codeRegime), m_CaisseGestionnaire(caisseGest), m_CentreGestionnaire(""),m_Name("")
     {      m_Error              = TR("Pas d'entrée trouvée pour ce code régime : ") + codeRegime + TR(" et caisse gestionnaire : ") + caisseGest;
            QString toFind       = QString("\"") + codeRegime + "\";\"" + caisseGest + "\";\"";
            int     deb          = -1;
            int     end          = -1;

            if ( (deb = dataCaisse.find(toFind)) != -1)
               {if ( (end = dataCaisse.find ("\r",deb)) == -1) end = dataCaisse.find ("\n",deb);
                if (end == -1) end = dataCaisse.length();
                deb                              += toFind.length();                // se placer sur donnée non connues
                toFind                            = dataCaisse.mid(deb, end-deb);
                QStringList lst                   = QStringList::split("\";\"",toFind, TRUE);
                m_CentreGestionnaire     = lst[0];
                m_Name                   = lst[1];
                m_Regime                 = lst[2];
                m_OrganismeDestinataire  = lst[3];
                m_CodeCentreInformatique = lst[4];
                m_Adresse_1              = lst[5];
                m_Adresse_2              = lst[6];
                m_Ville                  = lst[7];
                m_CodePostal             = lst[8];
                m_Cedex                  = lst[9];
                m_Tel_1                  = lst[10];
                m_Fax                    = lst[11].remove('\"');
                m_Error                  = "";
               }
     };
     //----------------------------------- CodeRegimeToOrganismeDestinataire ------------------------------------------
     //  m_Num-10 = 90 ,  10 , Code régime
     //  m_Num-11 = 100 , 11 , Caisse gestionnaire
     //  m_Num-12 = 0000, 12 , Centre gestionnaire
     //  m_Num-13 = 70,   13 , Code gestion

     //  01;999;0000;Caisse de TEST;999;999
     //  90;100;0000;Caisse d'assurance Vieillesse Invalidite et Maladie des cultes;100;004


     //  m_Num-10 = 01 ,   10 , Code régime
     //  m_Num-11 = 349 ,  11 , Caisse gestionnaire
     //  m_Num-12 = 9881 , 12 , Centre gestionnaire
     //  m_Num-13 = 10 ,   13 , Code gestion

 public:
    QString m_CodeRegime;
    QString m_CaisseGestionnaire;
    QString m_CentreGestionnaire;
    QString m_Name;
    QString m_Regime;
    QString m_OrganismeDestinataire;
    QString m_CodeCentreInformatique;
    QString m_Adresse_1;
    QString m_Adresse_2;
    QString m_Ville;
    QString m_CodePostal;
    QString m_Cedex;
    QString m_Tel_1;
    QString m_Fax;
    QString m_Error;
};

//================================================= C_Vitale ==============================================================
class  C_Vitale
{

 public:
     C_Vitale(const QString &data="");
     static  QStringList     CodeService_AMO_ToList();
     static  QStringList     CodeIndicTT_AMC_ToList();
     static  QStringList     CodeIndicTT_MUT_ToList();
     static  QString CodeService_AMO_ToName( const QString &codeService );
     static  QString CodeIndicTT_AMC_ToName( const QString &codeIndic );
     static  QString CodeIndicTT_MUT_ToName( const QString &codeIndic );
     static  QString Code_AiguillageSTS_ToName( const QString code );
     static  QString CodeQualiteToName( const QString &codeQualite );
     static  QStringList Code_AiguillageSTS_ToList();
     QString CodeRegimeToName(const QString &datacaisse, int avecNumero =1 );
     static  QString Mutnum_To_Complementaire(const QString &dataCorrespondance,
                                              QString  mutnum,
                                              QString  codeRegime,
                                              QString  caisseGest,
                                              QString  centreGest,
                                              QString *pOrgaComplName=0);

     QString CodeRegimeToName(int avecNumero = 1);
     QString CodeRegimeToName(const QString &codeRegime  ,  const QString &caisseGest  ,int avecNumero = 1);
     QString CodeRegimeToOrganismeDestinataire(QString &codeCentreInformatique, const QString &dataCaisses  = "" );
     int     SetMember(int memberIndex, unsigned short  iDGroupeCourant, const char* memberData, size_t len_data, int occur);
     QString GetMember(int memberIndex, unsigned short  iDGroupeCourant, int occur);
     QString GetDateNaissanceAssure( QDate* pQDate=0, const QString& sep="-");
     QString GetDateNaissanceAyantDroit(int occurence, QDate* pQDate=0, const QString& sep="-");
     QString GetRangGemellaire(int occurence);
     QString GetNomPrenomAssure(const QString& sep=" ", QString* p_nom=0, QString* p_prenom=0, QString* p_jfille=0);
     QString GetNomPrenomAyanDroit(int occurence, const QString& sep=" ", QString* p_nom=0, QString* p_prenom=0, QString* p_jfille=0);
     QString GetAdresseAssure(const QString& sep=" ");
     QString GetAdresseAyanDroit(int occurence, const QString& sep=" ");
     void    GetCodePostalVilleAssure(QString* codePostal, QString* ville);
     void    GetCodePostalVilleAyanDroit(int occurence, QString* codePostal, QString* ville);
     int     Compare(int occurence, const QString &numSecu, const QString &dtNss, const QString &rangGem, const QString &nom="",const QString &prenom="");
     int         GetNbOccurences(unsigned short  iDGroupeCourant);
     static  int GetNbOccurences(const QString &data, unsigned short  iDGroupeCourant);
     QString Serialize();
     void    UnSerialize(const QString &data);
     static  QString ToNum(QString in);

 public:
    QString m_VitaleData;
    QString m_Erreur;
};


#endif


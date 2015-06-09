/********************************* C_App.h ****************************************
 *  #include "C_App.h"          QT4-ready                                         *
 * ...............................................................................*
 *   Project   :  MedinTux  (typographie made in france)                          *
 *   Copyright : (C) 2004-2005-2006-2007-2008-2009-2010 and for the eternity      *
 *   by        :  Sevin Roland     from   MedinTux project                        *
 *   E-Mail    : roland-sevin@medintux.org                                        *
 *   Web site  : www.medintux.org                                                 *
 * ...............................................................................*
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat �  l'Energie Atomique                                           *
 *   - CEA,                                                                       *
 *                            31-33 rue de la F�d�ration, 75752 PARIS cedex 15.   *
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
#ifndef C_APP_H
#define C_APP_H

 #include <QObject>
 #include <QtGui/QApplication>
 #include <QTimer>

class QSqlQuery;
class QSqlError;
class QSqlDatabase;
//================================================= C_App ==============================================================
class C_App : public QApplication
{
    Q_OBJECT

 public:
     C_App(int & argc, char ** argv);
    ~C_App();
  int     BaseConnect(const QString & driver,              // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                      const QString & dataBaseToConnect,   // nom de la base: si QODBC3 -> nom de la source de donnees (userDSN)
                      const QString & user,                // = "root"
                      const QString & password,            // = ""
                      const QString & hostname,            // = "localhost"
                      const QString & port,
                      QString       * errMess,
                      QString         baseLabel);
    void            killBase();
    QString         GUID_Create();
    //..................................... Codes Postaux ..............................................................
    QString         GetVilleFromCodePostal(const QString &codePostal);
    QString         GetCodePostalFromVille(const QString &Ville);
    void            GetVilleFromCodePostal(const QString &codePostal, QStringList &villeList);
    void            GetCodePostalFromVille(const QString &ville     , QStringList &codePostalList);
    bool            IsThisDroitExist(const char *droitToFind);
    bool            IsThisDroitExist(const char *listDroits, const char *droitToFind);
    int             isValid(){return m_IsValid;}
    QString         getStartID(){return m_Start_ID;}
    QSqlDatabase    getDatabase();
    int             OpenBase();
    void            CloseBase();
    QString         getBDVersionNumber();
    QString         OutSQL_error(const QSqlQuery &cur,   const char *messFunc =0, const char *requete =0);
    QString         OutSQL_error(const QSqlError &error, const char *messFunc =0, const char *requete =0);
    QString         Utf8_Query(QSqlQuery &cur, int field);
    void            setDroits(const QString &droits){m_Droits = droits;}
    QString         getDroits(){return m_Droits;}
    QString*        pIniParam(){return &m_LocalParam;}
    QString         iniParam(){return   m_LocalParam;}
    QString         getPathAppli(){return m_PathAppli;}
private:
    int             initValues( QString &data, QString *errMess = 0);
    int             initValues(const char *pt_in, QString *errMess = 0);
    char           *SetConfBase_SetProperties(char *pt, QString &propertie, const char *token, int *line , QString &err);
public:
    QString       m_NUM_VERSION;
    QString       m_PathTheme;
    //............................ DATA .........................................................
    QString       m_VERSION_NUMBER;                   // numero de version de la base de donnees
    QString       m_INTERVENANTS_TBL_NAME;            // nom de la table des intervenants
    QString       m_INTERVENANTS_GUID;                // Identifiant unique de cet intervenant
    QString       m_INTERVENANTS_LOGIN_SIGEMS;        // Identifiant Sigems d'intervenant
    QString       m_INTERVENANTS_LOGIN;               // Identifiant d'intervenant
    QString       m_INTERVENANTS_PASSWORD_SIGEMS;     // mot de passe Sigems
    QString       m_INTERVENANTS_PASSWORD;            // mot de passe
    QString       m_INTERVENANTS_DROITS;              // Droits
    QString       m_INTERVENANTS_CODE_SPEC;           // spécialité de l'intervenant
    QString       m_INTERVENANTS_NUM_ORDRE;           // Numéro d'ordre (si il y en a un)
    QString       m_INTERVENANTS_NOM;                 // nom de l'intervenant
    QString       m_INTERVENANTS_PRENOM;              // prenom de l'intervenant
    QString       m_INTERVENANTS_ADRESSE;             // spécialité de l'intervenant
    QString       m_INTERVENANTS_CONVENTION;          // convention exple "01 MEDECINE GENERALE"
    QString       m_INTERVENANTS_CODE_POST;           // Code postal de l'intervenant
    QString       m_INTERVENANTS_VILLE;               // Ville de l'intervenant
    QString       m_INTERVENANTS_TEL_1;               // Numero de telephone
    QString       m_INTERVENANTS_TEL_2;               // Numero de telephone
    QString       m_INTERVENANTS_TEL_3;               // Numero de telephone
    QString       m_INTERVENANTS_EMAIL;               // Email
    QString       m_INTERVENANTS_PK;                  // primary key
    QString       m_INTERVENANTS_NOTE;                // Note de l'intervenant (Dr Pr Mr Mm etc...)
    QString       m_INTERVENANTS_QUALITE;             // Type d'intervenant (exple: Medecin Urgentiste)
    QString       m_INTERVENANTS_CODE_REGIME;         // Code Regime
    QString       m_INTERVENANTS_SEXE;                // sexe
    QString       m_INTERVENANTS_TITRE;               // Titre de l'intervenant (Mr Dr Pr Melle)
    QString       m_INTERVENANTS_CHER;                // Lien de politesse (cher ami etc ...)
    QString       m_INTERVENANTS_TEL_TYPE_1;          // type de Numero de telephone
    QString       m_INTERVENANTS_TEL_TYPE_2;          // type de Numero de telephone
    QString       m_INTERVENANTS_TEL_TYPE_3;          // type de Numero de telephone
    QString       m_INTERVENANTS_TEL_ABR_1;           // Numero de telephone abrégé
    QString       m_INTERVENANTS_TEL_ABR_2;           // Numero de telephone abrégé
    QString       m_INTERVENANTS_TEL_ABR_3;           // Numero de telephone abrégé
    QString       m_INTERVENANTS_COULEUR;             // couleur associee a cet intervenant

    QString       m_CODEPOST_TBL_NAME;                // table des codes postaux
    QString       m_CODEPOST_PK;                      // clef primaire
    QString       m_CODEPOST_CODE_POST;               // code postal
    QString       m_CODEPOST_VILLE;                   // ville

private:

    QString       m_DriverName;
    QString       m_BaseName;
    QString       m_UserName;
    QString       m_PassWord;
    QString       m_HostName;
    QString       m_Port;
    QString       m_BaseLabel;
    int           m_IsValid;
    QString       m_Start_ID;
    QString       m_Droits;              /*!< Droits de l'utilisateur */
    QTimer       *m_pReconnectTimer;
    QString       m_PathAppli;
    QString       m_LocalParam;
 protected slots:
    void Slot_ReconnectTimerDone();

};

extern C_App* G_pCApp;


#endif // C_APP_H

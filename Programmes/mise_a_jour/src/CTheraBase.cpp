/********************************* $FILE$ *****************************************

    Project: MedinTux
    Copyright (C) 2005 by Roland Sevin  and Data Medical Design
    E-Mail: data.medical.design@club-internet.fr

    This program is free software; you can redistribute it and/or modify
    it under the terms of the CeCILL Version 1 du 21/06/2004
    License (GPL compatible)
                               http://www.cecill.info/
    as published by :

    Commissariat �l'Energie Atomique
    - CEA,
                             31-33 rue de la F��ation, 75752 PARIS cedex 15.
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

 **********************************************************************************/
 #include <stdlib.h>


#include <qapplication.h>
#include <qfileinfo.h>
#include <qfile.h>
#include <qdir.h>
#include <qpixmap.h>
#include <qtextstream.h>
#include <qsqlcursor.h>
#include <qlineedit.h>
#include "CTheraBase.h"
#include "../../MedinTuxTools/CGestIni.h"

/*....................................... datasemp ....................................................
[Connexion]
    Parametres MedicaBase = QMYSQL3 , MedicaTuxTest , root ,  , localhost
    Parametres SrceBase = QMYSQL3 , DatasempTest , root ,  , localhost
    Parametres Get_Base = QMYSQL3 , BaseGetTest , root ,  , localhost
    InUseBase = Parametres MedicaBase , noMedicaBase
    SplashScreen = non
/ *....................................... datasemp ....................................................
[Connexion]
    Parametres MedicaBase = QMYSQL3 , MedicaTuxTest , root ,  , localhost
    Parametres SrceBase = QMYSQL3 , DatasempTest , root ,  , localhost
    Parametres Get_Base = QMYSQL3 , BaseGetTest , root ,  , localhost
    InUseBase = Parametres Get_Base , MedicaBase
    SplashScreen = non
*/

//-----------------------------------------------------  CTheraBase ------------------------------------------------
CTheraBase::CTheraBase()
{
  QString path_MedicaBase_ini   =   CGestIni::Construct_Name_File_Ini( "mise_a_jour", QFileInfo (qApp->argv()[0]).dirPath (true),"Ressources/MedicaBase.ini");
  QString path_MedicaBase_cfg   =   CGestIni::Construct_Name_File_Ini( "drtux",       QFileInfo (qApp->argv()[0]).dirPath (true),"MedicaBase.cfg");
  Init_Base(path_MedicaBase_ini,  path_MedicaBase_cfg);
}

//-----------------------------------------------------  Init_Base -------------------------------------------------
CTheraBase::CTheraBase( QString path_MedicaBase_ini,  QString path_MedicaBase_cfg)
{ Init_Base( path_MedicaBase_ini,  path_MedicaBase_cfg);
}

//-----------------------------------------------------  Init_Base -------------------------------------------------
void CTheraBase::Init_Base( QString &path_MedicaBase_ini,  QString &path_MedicaBase_cfg)
{  m_DatasempBase      = 0;
   m_DataGet_Base      = 0;
   m_DatasempIndexBase = 0;
   m_DataIndexGet_Base = 0;
   m_AllIsOk           = FALSE;
  //.................. lire le fichier ini pour en retirer les paramêtres de connexion ...............................
  CGestIni::Param_UpdateFromDisk(path_MedicaBase_ini, m_IniParam);

  //................................... DATASEMP  ...............................................................
  if (CGestIni::Param_ReadParam(  m_IniParam, "Connexion",  "Parametres SrceBase",
                                  &m_DriverName, &m_BaseName, &m_UserName, &m_PassWord, &m_HostName) != 0
     )  // zero = pas d'erreur
     {   m_DriverName  = "QMYSQL3";
         m_BaseName    = "DrTux";
         m_UserName    = "root";
         m_PassWord    = "";
         m_HostName    = "";
     }

   m_DatasempBase =       BaseConnect( m_DriverName,    // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                       "DatasempTest",  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                       m_UserName,      // = "root"
                                       m_PassWord,      // = ""
                                       m_HostName,      // = "localhost"
                                       "DatasempTest",  &m_ErrMess);
   if (m_DatasempBase==0)      return;

   m_DataGet_Base =       BaseConnect( m_DriverName,    // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                       "BaseGetTest",   // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                       m_UserName,      // = "root"
                                       m_PassWord,      // = ""
                                       m_HostName,      // = "localhost"
                                       "BaseGetTest",   &m_ErrMess);
   if (m_DataGet_Base==0)      return;

   m_DatasempIndexBase =  BaseConnect( m_DriverName,    // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                       "MedicaTuxTest", // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                       m_UserName,      // = "root"
                                       m_PassWord,      // = ""
                                       m_HostName,      // = "localhost"
                                       "MedicaTuxTest", &m_ErrMess);
   if (m_DatasempIndexBase==0) return;

   m_DataIndexGet_Base =  BaseConnect( m_DriverName,    // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                       "BaseGetTest",   // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                       m_UserName,      // = "root"
                                       m_PassWord,      // = ""
                                       m_HostName,      // = "localhost"
                                       "BaseGetTest",   &m_ErrMess);
   if (m_DataIndexGet_Base==0) return;

   //................. fichier de config des bases.......................................
   QString str = "";;
   if (CGestIni::Param_UpdateFromDisk(path_MedicaBase_cfg, str )<=0)
      {m_ErrMess = TR("Impossible de lire le fichier de configuration : ") + path_MedicaBase_cfg;
       qDebug ( m_ErrMess );   return;
      }

   SetConfBase(str,  m_ErrMess);
   if (m_ErrMess.length()) {qDebug(m_ErrMess);  return;
                           }
   m_AllIsOk = TRUE;
}

//-----------------------------------------------------  ~CTheraBase -------------------------------------------
CTheraBase::~CTheraBase()
{if (m_DatasempBase==0)      QSqlDatabase::removeDatabase ("DatasempTest");
 if (m_DataGet_Base==0 )     QSqlDatabase::removeDatabase ("BaseGetTest");
 if (m_DatasempIndexBase==0) QSqlDatabase::removeDatabase ("MedicaTuxTest");
 if (m_DataIndexGet_Base==0) QSqlDatabase::removeDatabase ("BaseGetTest");

 m_DatasempBase      = 0;
 m_DataGet_Base      = 0;
 m_DatasempIndexBase = 0;
 m_DataIndexGet_Base = 0;

}

//-----------------------------------------------------  BaseConnect -------------------------------------------
QSqlDatabase*  CTheraBase::BaseConnect(const char* driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                        const char* DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                        const char* user,          // = "root"
                                        const char* pasword,       // = ""
                                        const char* hostname,      // = "localhost"
                                        const char* nameBase,
                                        QString*    errMess)
    {
        QSqlDatabase::removeDatabase (nameBase);
        QSqlDatabase *defaultDB = QSqlDatabase::addDatabase( driver , nameBase);
        if (defaultDB<=0) return 0;
        defaultDB->setDatabaseName( DataBaseName );
        defaultDB->setUserName( user );
        defaultDB->setPassword( pasword );
        defaultDB->setHostName( hostname);
        if ( ! defaultDB->open() )
           {QString qstr = "";
            qstr += QString("Failed to open database: ") + QString(driver) + "  " + QString(DataBaseName) + "\r\n" +
                     defaultDB->lastError().driverText() + "\r\n" + defaultDB->lastError().databaseText();
            qWarning(qstr);
            if (errMess) *errMess = qstr;
            return 0;
           }
        return defaultDB;
    }

//-----------------------------------------------------  SetConfBase -------------------------------------------
int CTheraBase::SetConfBase(const char* confFile, QString err)
{ char     *pt = (char*) confFile;
  int     line = 0;

  pt = SetConfBase_SetProperties(pt,  m_MEDICA_INDEX_MED_TBL_NAME,     "m_MEDICA_INDEX_MED_TBL_NAME",    &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_MEDICA_INDEX_DIET_TBL_NAME,    "m_MEDICA_INDEX_DIET_TBL_NAME",   &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_MEDICA_INDEX_VETO_TBL_NAME,    "m_MEDICA_INDEX_VETO_TBL_NAME",   &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_MEDICA_INDEX_PARA_TBL_NAME,    "m_MEDICA_INDEX_PARA_TBL_NAME",   &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_MEDICA_INDEX_ACCS_TBL_NAME,    "m_MEDICA_INDEX_ACCS_TBL_NAME",   &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_MEDICA_INDEX_HERBO_TBL_NAME,   "m_MEDICA_INDEX_HERBO_TBL_NAME",  &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,  m_MEDICA_INDEX_HOMEO_TBL_NAME,   "m_MEDICA_INDEX_HOMEO_TBL_NAME",  &line , err); if (err.length())     goto SetConfBase_Error;


  pt = SetConfBase_SetProperties(pt , m_MEDICA_INDEX_PK,            "m_MEDICA_INDEX_PK",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_MEDICA_INDEX_LIBELLE,       "m_MEDICA_INDEX_LIBELLE",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_MEDICA_INDEX_CIP,           "m_MEDICA_INDEX_CIP",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_MEDICA_INDEX_PK_T00,        "m_MEDICA_INDEX_PK_T00",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_MEDICA_INDEX_PK_T4B,        "m_MEDICA_INDEX_PK_T4B",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_MEDICA_INDEX_CD_PROD,       "m_MEDICA_INDEX_CD_PROD",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_MEDICA_INDEX_CD_UV,         "m_MEDICA_INDEX_CD_UV",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_MEDICA_INDEX_IS_GENE,       "m_MEDICA_INDEX_IS_GENE",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_MEDICA_INDEX_IS_DISPO,      "m_MEDICA_INDEX_IS_DISPO",       &line , err); if (err.length())     goto SetConfBase_Error;

  pt = SetConfBase_SetProperties(pt , m_MEDICA_SPEC_TBL_NAME         , "m_MEDICA_SPEC_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_MEDICA_SPEC_CIP              , "m_MEDICA_SPEC_CIP",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_MEDICA_SPEC_UP_UNIT          , "m_MEDICA_SPEC_UP_UNIT",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_MEDICA_SPEC_PA_UNIT          , "m_MEDICA_SPEC_PA_UNIT",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_MEDICA_SPEC_PA_QU            , "m_MEDICA_SPEC_PA_QU",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_MEDICA_SPEC_SECABILITE       , "m_MEDICA_SPEC_SECABILITE",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_MEDICA_SPEC_IS_GENERIC       , "m_MEDICA_SPEC_IS_GENERIC",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_MEDICA_SPEC_FORM_STRUC       , "m_MEDICA_SPEC_FORM_STRUC",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_MEDICA_SPEC_NB_FORME         , "m_MEDICA_SPEC_NB_FORME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_MEDICA_SPEC_PRIX_BOITE       , "m_MEDICA_SPEC_PRIX_BOITE",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_MEDICA_SPEC_REMBOURSEMENT    , "m_MEDICA_SPEC_REMBOURSEMENT",    &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_MEDICA_SPEC_TABLEAU          , "m_MEDICA_SPEC_TABLEAU",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_MEDICA_SPEC_LABO             , "m_MEDICA_SPEC_LABO",             &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt , m_MEDICA_SPEC_ATC              , "m_MEDICA_SPEC_ATC",              &line , err); if (err.length())     goto SetConfBase_Error;

  pt =  SetConfBase_SetProperties(pt , m_MEDICA_FORME_UP_PERSO_TBL_NAME, "m_MEDICA_FORME_UP_PERSO_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_FORME_AD_PERSO_TBL_NAME, "m_MEDICA_FORME_AD_PERSO_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_FORME_UP_TBL_NAME,  "m_MEDICA_FORME_UP_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_FORME_PA_TBL_NAME,  "m_MEDICA_FORME_PA_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_FORME_ST_TBL_NAME,  "m_MEDICA_FORME_ST_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_FORME_AD_TBL_NAME,  "m_MEDICA_FORME_AD_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_FORME_PK,           "m_MEDICA_FORME_PK",                  &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_FORME_LIBELLE,      "m_MEDICA_FORME_LIBELLE",             &line , err); if (err.length())     goto SetConfBase_Error;

  pt =  SetConfBase_SetProperties(pt , m_MEDICA_ATC_TBL_NAME,       "m_MEDICA_ATC_TBL_NAME",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_ATC_CODE,           "m_MEDICA_ATC_CODE",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_ATC_LIBELLE,        "m_MEDICA_ATC_LIBELLE",           &line , err); if (err.length())     goto SetConfBase_Error;

  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_PERSO_TBL_NAME, "m_MEDICA_POSO_PERSO_TBL_NAME",    &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_TBL_NAME,       "m_MEDICA_POSO_TBL_NAME",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_PK,             "m_MEDICA_POSO_PK",                &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_EXTRA_PK,       "m_MEDICA_POSO_EXTRA_PK",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_CIP,            "m_MEDICA_POSO_CIP",               &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_LIBELLE,        "m_MEDICA_POSO_LIBELLE",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_AGE_MIN,        "m_MEDICA_POSO_AGE_MIN",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_AGE_MAX,        "m_MEDICA_POSO_AGE_MAX",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_SEXE,           "m_MEDICA_POSO_SEXE",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_TERRAIN,        "m_MEDICA_POSO_TERRAIN",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_POIDS_MIN,      "m_MEDICA_POSO_POIDS_MIN",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_POIDS_MAX,      "m_MEDICA_POSO_POIDS_MAX",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_DOSE_MIN,       "m_MEDICA_POSO_DOSE_MIN",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_DOSE_MAX,       "m_MEDICA_POSO_DOSE_MAX",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_DOSE_UNIT,      "m_MEDICA_POSO_DOSE_UNIT",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_DOSE_LIMIT_MAX, "m_MEDICA_POSO_DOSE_LIMIT_MAX",    &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_FACT_CORP_QU,   "m_MEDICA_POSO_FACT_CORP_QU",      &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_FACT_CORP_UNIT, "m_MEDICA_POSO_FACT_CORP_UNIT",    &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_PERIODE,        "m_MEDICA_POSO_PERIODE",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_EQUI_COEFF,     "m_MEDICA_POSO_EQUI_COEFF",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_UNIT_COEFF,     "m_MEDICA_POSO_UNIT_COEFF",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_NB_PRISES,      "m_MEDICA_POSO_NB_PRISES",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_REPART_PRISES,  "m_MEDICA_POSO_REPART_PRISES",     &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_DIVERS,         "m_MEDICA_POSO_DIVERS",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_PENDANT,        "m_MEDICA_POSO_PENDANT",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt =  SetConfBase_SetProperties(pt , m_MEDICA_POSO_SECABILITE,     "m_MEDICA_POSO_SECABILITE",        &line , err); if (err.length())     goto SetConfBase_Error;

  return 1;

SetConfBase_Error:
  qDebug(err);
  return 0;
}
//----------------------------------------------------- GotoDebug -----------------------------------------------------------
int CTheraBase::GotoDebug()
{return 1;
}
//--------------------------------------------- SetConfBase_SetProperties ------------------------------------------------
char  *CTheraBase::SetConfBase_SetProperties(char *pt, QString &propertie, const char *token, int *line , QString &err)
{
 err  = "";
 long len_token = strlen (token);
 while(*pt)
    {SKIP_BLANK_CAR(pt);
     if (strncmp(pt, token, len_token)==0)
        {
         pt         += len_token;
         err         = "";
         propertie   = "";
         //............. chercher premier delimiteur ...............................
          while(*pt && *pt!='\r' && *pt !='\n' && *pt !='\'' ) pt++;
          if (*pt=='\'')         // si delimiteur pas trouve erreur
             {pt++;               // passer le delimiteur
              char *end = pt;
              //............. chercher deuxiéme delimiteur ...............................
              while(*end && *end!='\r' && *end !='\n' && *end !='\'')end++;
              if (*end=='\'')
                 {propertie.setLatin1(pt, end-pt);
                  return end +1;
                 }
              else
                {err  = "Syntax Error: SetConfBase_SetProperties()\r\n End delimiter \"'\"  not find at line N°: ";
                 err += QString::number(*line);
                 return 0;
                }
             }
           else
             {err  = "Syntax Error: SetConfBase_SetProperties()\r\n Start delimiter \"'\"  not find at line N°: ";
              err += QString::number(*line);
              return 0;
             }
        } //endif (strncmp(pt, token, len_token)==0)

     NEXT_LINE(pt);
     if (line) *line++;
    }
 err  = "Syntax Error: SetConfBase_SetProperties()\r\n Token : ";
 err += token;
 err += " not find at line N°: ";
 err += QString::number(*line);
 return pt;
}

//--------------------------------- Utf8_Query ------------------------------------------------------------
QString CTheraBase::Utf8_Query(QSqlQuery &cur, int field)
{QCString     cString  = cur.value(field).toCString ();
 char *ptr             =  cString.data();
 if (CGestIni::IsUtf8( ptr ))
    {return QString::fromUtf8 ( ptr );
    }
 else
    {return QString(ptr) ;
    }
}

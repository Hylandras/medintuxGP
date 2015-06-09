/***************************************************************************
 *   Copyright (C) 2004 by Sevin Roland                                    *
 *   ro@vaio_ro                                                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <stdlib.h>


#include <qapplication.h>
#include <qfileinfo.h>
#include <qfile.h>
#include <qdir.h>
#include <qpixmap.h>
#include <qtextstream.h>
#include <qsqlcursor.h>
#include <qlineedit.h>

#include "CMedicaBase.h"
#include "CGestIni.h"
#include "CPrtQListBoxItem.h"

//-----------------------------------------------------  CMedicaBase ------------------------------------------------
CMedicaBase::CMedicaBase()
{
  QString path_MedicaBase_ini   =   QFileInfo (qApp->argv()[0]).dirPath (true)+QDir::separator()+"MedicaBase.ini";
  QString path_MedicaBase_cfg   =   QFileInfo (qApp->argv()[0]).dirPath (true)+QDir::separator()+"MedicaBase.cfg";
  Init_Base(path_MedicaBase_ini,  path_MedicaBase_cfg);
}

//-----------------------------------------------------  Init_Base -------------------------------------------------
CMedicaBase::CMedicaBase( QString path_MedicaBase_ini,  QString path_MedicaBase_cfg)
{ Init_Base( path_MedicaBase_ini,  path_MedicaBase_cfg);
}

//-----------------------------------------------------  Init_Base -------------------------------------------------
void CMedicaBase::Init_Base( QString &path_MedicaBase_ini,  QString &path_MedicaBase_cfg)
{

  //.................. lire le fichier ini pour en retirer les paramêtres de connexion ...............................
  CGestIni::Param_UpdateFromDisk(path_MedicaBase_ini, m_IniParam);
  m_ModeBase = 0;
  //................................... DATASEMP  ...............................................................
  if (CGestIni::Param_ReadParam(  m_IniParam, "Connexion",  "Parametres SrceBase",
                                  &m_DriverName, &m_BaseName, &m_UserName, &m_PassWord, &m_HostName, &m_Port) !=0
     )  // zero = pas d'erreur
     {   m_DriverName  = "QMYSQL3";
         m_BaseName    = "DrTux";
         m_UserName    = "root";
         m_PassWord    = "";
         m_HostName    = "";
         m_Port        = "3306";
     }
   m_DataBaseSrce =  BaseConnect(m_DriverName,   // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                 m_BaseName,     // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                 m_UserName,     // = "root"
                                 m_PassWord,     // = ""
                                 m_HostName,     // = "localhost"
                                 m_Port,         // = "3306"
                                 DATASEMP_BASE);

   m_MaxList = 100;
   if (CGestIni::Param_ReadParam(  m_IniParam, "GeneralConfig", "Nb_Max_List", &m_HostName) ==0) m_MaxList =   m_HostName.toInt();  // provisoire le m_HostName
   #ifdef _NOBODY_KNOW
        ExtraConnect(m_IniParam);
   #endif //#ifdef _NOBODY_KNOW
   //................................... GET_BASE ......................................................................
   if (CGestIni::Param_ReadParam(  m_IniParam, "Connexion", "Parametres Get_Base",
                                  &m_DriverName, &m_BaseName, &m_UserName, &m_PassWord, &m_HostName, &m_Port) !=0
      )  // zero = pas d'erreur
      {   m_DriverName  = "QMYSQL3";
          m_BaseName    = "DrTux";
          m_UserName    = "root";
          m_PassWord    = "";
          m_HostName    = "";
          m_Port        = "3306";
      }

   m_DataGet_Base =      BaseConnect(m_DriverName,   // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                     m_BaseName,     // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                     m_UserName,     // = "root"
                                     m_PassWord,     // = ""
                                     m_HostName,     // = "localhost"
                                     m_Port,         // = "3306"
                                     GET_BASE);
  //     [Connexion]
  //         Parametres MedicaBase = QMYSQL3 , MedicaTuxTest , root ,  , localhost , 3306
  //         Parametres SrceBase = QMYSQL3 , DatasempTest , root ,  , localhost , 3306
  //         Parametres Get_Base = QMYSQL3 , BaseGetTest , root ,  , localhost , 3306
  //         InUseBase = Parametres MedicaBase , noMedicaBase
  //................................... recuperer le nom de la base courante à activer........................................
  QString mode;
  if (CGestIni::Param_ReadParam(  m_IniParam, "Connexion", "InUseBase", &m_InUseBase, &mode) !=0 )  // zero = pas d'erreur
     {   m_InUseBase  = "MedicaBase";
     }
  //................................... MEDICA_BASE ( Base d'usage courant) ......................................................................
  if (CGestIni::Param_ReadParam(  m_IniParam, "Connexion", m_InUseBase,
                                  &m_DriverName, &m_BaseName, &m_UserName, &m_PassWord, &m_HostName, &m_Port) !=0
     )  // zero = pas d'erreur
     {   m_DriverName  = "QMYSQL3";
         m_BaseName    = "MedicaTux";
         m_UserName    = "root";
         m_PassWord    = "";
         m_HostName    = "";
         m_Port        = "3306";
     }

  //...............  Mode de recherche des infos CMedicaBase::MedicaBase ou CMedicaBase::Datasemp .............................
  if (mode == "MedicaBase" || m_DataBaseSrce==0)
     {Medica_SetBaseMode(CMedicaBase::MedicaBase);
      m_MedicaBase =    m_DataGet_Base;         // l'index des med est deja dans la BaseGet (dans le datasemp il est externe dans la base MedicaTuxTest)
     }
  else
     {Medica_SetBaseMode(CMedicaBase::Datasemp);
      m_MedicaBase =    BaseConnect(m_DriverName,   // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                    m_BaseName,     // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                    m_UserName,     // = "root"
                                    m_PassWord,     // = ""
                                    m_HostName,     // = "localhost"
                                    m_Port,         // = "3306"
                                    MEDICA_BASE);
     }

 Datasemp_SetHTMLFontProperty("Arial","24","18","16","16");
 //................. fichier de config des bases.......................................
 QString str = "";;
 if (CGestIni::Param_UpdateFromDisk(path_MedicaBase_cfg, str )<=0)
    {qDebug (TR("Impossible de lire le fichier de configuration : ") + path_MedicaBase_cfg);
     return;
    }
 QString err("");
 SetConfBase(str,  err);
 if (err.length()) {qDebug(err);                         return;}

 /*
;==================== BaseGet ======================================
[Connexion]
    Parametres MedicaBase = QMYSQL3 , MedicaTuxTest , root ,  , 192.168.100.3 , 3306
    Parametres SrceBase = QMYSQL3 , DatasempTest , root ,  , 192.168.100.3 , 3306
    Parametres Get_Base = QMYSQL3 , BaseGetTest , root ,  , 192.168.100.3 , 3306
    InUseBase = Parametres Get_Base , MedicaBase
;==================== BaseDatasempt =================================
[Connexion]
    Parametres MedicaBase = QMYSQL3 , MedicaTuxTest , root ,  , 192.168.100.3 , 3306
    Parametres SrceBase = QMYSQL3 , DatasempTest , root ,  , 192.168.100.3 , 3306
    Parametres Get_Base = QMYSQL3 , BaseGetTest , root ,  , 192.168.100.3 , 3306
    InUseBase = Parametres MedicaBase , noMedicaBase


  //.................. lire le fichier ini pour en retirer les paramêtres de connexion ...............................
  CGestIni::Param_UpdateFromDisk(path_MedicaBase_ini, m_IniParam);
  m_ModeBase = 0;
  //................................... DATASEMP  ...............................................................
  if (CGestIni::Param_ReadParam(  m_IniParam, "Connexion",  "Parametres SrceBase",
                                  &m_DriverName, &m_BaseName, &m_UserName, &m_PassWord, &m_HostName, &m_Port) !=0
     )  // zero = pas d'erreur
     {   m_DriverName  = "QMYSQL3";
         m_BaseName    = "DrTux";
         m_UserName    = "root";
         m_PassWord    = "";
         m_HostName    = "";
         m_Port        = "3306";
     }
   m_DataBaseSrce =  BaseConnect(m_DriverName,   // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                 m_BaseName,     // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                 m_UserName,     // = "root"
                                 m_PassWord,     // = ""
                                 m_HostName,     // = "localhost"
                                 m_Port,         // = "3306"
                                 DATASEMP_BASE);

   m_MaxList = 100;
   if (CGestIni::Param_ReadParam(  m_IniParam, "GeneralConfig", "Nb_Max_List", &m_HostName) ==0) m_MaxList =   m_HostName.toInt();  // provisoire le m_HostName
   #ifdef _NOBODY_KNOW
        ExtraConnect(m_IniParam);
   #endif //#ifdef _NOBODY_KNOW
   //................................... GET_BASE ......................................................................
   if (CGestIni::Param_ReadParam(  m_IniParam, "Connexion", "Parametres Get_Base",
                                  &m_DriverName, &m_BaseName, &m_UserName, &m_PassWord, &m_HostName, &m_Port) !=0
      )  // zero = pas d'erreur
      {   m_DriverName  = "QMYSQL3";
          m_BaseName    = "DrTux";
          m_UserName    = "root";
          m_PassWord    = "";
          m_HostName    = "";
          m_Port        = "3306";
      }

   m_DataGet_Base =      BaseConnect(m_DriverName,   // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                     m_BaseName,     // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                     m_UserName,     // = "root"
                                     m_PassWord,     // = ""
                                     m_HostName,     // = "localhost"
                                     m_Port,         // = "3306"
                                     GET_BASE);
  if (m_DataGet_Base->isOpen()==FALSE && m_DataGet_Base->open()==FALSE)
     {qDebug(TR("BaseGet non accessible %1").arg(QString::number((unsigned long)m_DataGet_Base)));
     }
  //................................... recuperer le nom de la base courante à activer........................................
  QString mode;
  if (CGestIni::Param_ReadParam(  m_IniParam, "Connexion", "InUseBase", &m_InUseBase, &mode) !=0 )  // zero = pas d'erreur
     {   m_InUseBase  = "MedicaBase";
     }

  //...............  Mode de recherche des infos CMedicaBase::MedicaBase ou CMedicaBase::Datasemp .............................
  if (mode == "MedicaBase")   Medica_SetBaseMode(CMedicaBase::MedicaBase);
  else                        Medica_SetBaseMode(CMedicaBase::Datasemp);
  //................................... MEDICA_BASE ( Base d'usage courant) ......................................................................
  if (CGestIni::Param_ReadParam(  m_IniParam, "Connexion", m_InUseBase,
                                  &m_DriverName, &m_BaseName, &m_UserName, &m_PassWord, &m_HostName, &m_Port) !=0
     )  // zero = pas d'erreur
     {   m_DriverName  = "QMYSQL3";
         m_BaseName    = "MedicaTux";
         m_UserName    = "root";
         m_PassWord    = "";
         m_HostName    = "";
         m_Port        = "3306";
     }

 m_MedicaBase =    BaseConnect(m_DriverName,   // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                               m_BaseName,     // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                               m_UserName,     // = "root"
                               m_PassWord,     // = ""
                               m_HostName,     // = "localhost"
                               m_Port,         // = "3306"
                               MEDICA_BASE);
 Datasemp_SetHTMLFontProperty("Arial","24","18","16","16");
 //................. fichier de config des bases.......................................
 QString str = "";;
 if (CGestIni::Param_UpdateFromDisk(path_MedicaBase_cfg, str )<=0)
    {qDebug (TR("Impossible de lire le fichier de configuration : ") + path_MedicaBase_cfg);
     return;
    }
 QString err("");
 SetConfBase(str,  err);
 if (err.length()) {qDebug(err);                         return;}
 */
}

//-----------------------------------------------------  ~CMedicaBase -------------------------------------------
CMedicaBase::~CMedicaBase()
{QSqlDatabase::removeDatabase (MEDICA_BASE);
 m_MedicaBase = 0;
 QSqlDatabase::removeDatabase (DATASEMP_BASE);
 m_DataBaseSrce = 0;
 QSqlDatabase::removeDatabase (GET_BASE);
 m_DataGet_Base = 0;
 #ifdef _NOBODY_KNOW
        ExtraDeconnect();
 #endif //#ifdef _NOBODY_KNOW
}

//-----------------------------------------------------  BaseConnect -------------------------------------------
QSqlDatabase*  CMedicaBase::BaseConnect(const char* driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                        const char* DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                        const char* user,          // = "root"
                                        const char* pasword,       // = ""
                                        const char* hostname,      // = "localhost"
                                        const QString &port,       // = "port"
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
        defaultDB->setPort( port.toInt());
        if ( ! defaultDB->open() )
           {QString qstr = "";
            qstr += QString("Failed to open database: ") + QString(driver) + "  " + QString(DataBaseName) + "\n" +
                     defaultDB->lastError().driverText() + "\n" + defaultDB->lastError().databaseText();
            qWarning(qstr);
            if (errMess) *errMess = qstr;
            return 0;
           }
        return defaultDB;
    }

//-----------------------------------------------------  SetConfBase -------------------------------------------
int CMedicaBase::SetConfBase(const char* confFile, QString err)
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
int CMedicaBase::GotoDebug()
{return 1;
}
//--------------------------------------------- SetConfBase_SetProperties ------------------------------------------------
char  *CMedicaBase::SetConfBase_SetProperties(char *pt, QString &propertie, const char *token, int *line , QString &err)
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
                {err  = "Syntax Error: SetConfBase_SetProperties()\n End delimiter \"'\"  not find at line N°: ";
                 err += QString::number(*line);
                 return 0;
                }
             }
           else
             {err  = "Syntax Error: SetConfBase_SetProperties()\n Start delimiter \"'\"  not find at line N°: ";
              err += QString::number(*line);
              return 0;
             }
        } //endif (strncmp(pt, token, len_token)==0)

     NEXT_LINE(pt);
     if (line) *line++;
    }
 err  = "Syntax Error: SetConfBase_SetProperties()\n Token : ";
 err += token;
 err += " not find at line N°: ";
 err += QString::number(*line);
 return pt;
}

//--------------------------------- Utf8_Query ------------------------------------------------------------
QString CMedicaBase::Utf8_Query(QSqlQuery &cur, int field)
{QCString     cString  = cur.value(field).toCString ();
 char *ptr             =  cString.data();
 if (CGestIni::IsUtf8( ptr , cString.length()))
    {return QString::fromUtf8 ( ptr );
    }
 else
    {return QString(ptr) ;
    }
}
//----------------------------------------------------- Medica_SetBaseMode ---------------------------------------------------------------
void CMedicaBase::Medica_SetBaseMode(int mode)
{m_ModeBase |= mode;
}          // pour mode limité (base Get)

//----------------------------------------------------- Medica_GetBaseMode ---------------------------------------------------------------
int CMedicaBase::Medica_GetBaseMode()
{return m_ModeBase;
}
//----------------------------------------------------- save_TraitementEnCours ---------------------------------------------------------------
int CMedicaBase::save_TraitementEnCours(QString /* pk_doss */,
                                        QString /* numGUID */,
                                        QString /* user */,
                                        QString /* signUser */,
                                        QPL_CPosologie /* ordoList */,
                                        QSqlDatabase   * /* dataBaseDst */ )
{return 0;
}
//----------------------------------------------------- Medica_GetDatasempVersion ---------------------------------------------------------------
//           VERS_MODE mode soit CMedicaBase::Data  et alors sera retourné la version des tables datasemp
//                               CMedicaBase::Index et alors sera retourné la version de l'index MedicaBase
QString CMedicaBase::Medica_GetDatasempVersion(VERS_MODE mode)
{   QString version;
    if (m_DataGet_Base==0)                                                 return QObject::tr("ERROR : CMedicaBase::Medica_GetDatasempVersion() m_DataGet_Base =0");
    if (m_DataGet_Base->isOpen()==FALSE && m_DataGet_Base->open()==FALSE)  return QObject::tr("ERROR : CMedicaBase::Medica_GetDatasempVersion() m_DataGet_Base can't be opened");;
    QSqlQuery sqlQuery ("SELECT DatasempVersion FROM version " , m_DataGet_Base );
    if (sqlQuery.isActive() && sqlQuery.next())
       {version = sqlQuery.value(0).toString();
        int pos = version.find('|');
        if (pos != -1)
           {if       (mode == CMedicaBase::Data)       version = version.mid(pos + 1);
            else if  (mode == CMedicaBase::Index)      version = version.left(pos);
           }
       }
    else               version = "";
    m_DataGet_Base->close();
    return version;
}

//----------------------------------------------------- Medica_SetDatasempVersion ---------------------------------------------------------------
//           VERS_MODE mode soit CMedicaBase::Data  et alors sera retourné la version des tables datasemp
//                               CMedicaBase::Index et alors sera retourné la version de l'index MedicaBase
QString CMedicaBase::Medica_SetDatasempVersion(QString version, VERS_MODE mode)
{   if      (mode == CMedicaBase::Data)   version =  Medica_GetDatasempVersion(CMedicaBase::Index) + "|" + version;
    else if (mode == CMedicaBase::Index)  version =  version  + "|" + Medica_GetDatasempVersion(CMedicaBase::Data);
    else                                  version =  version  + "|" + version;
    if (m_DataGet_Base->isOpen()==FALSE && m_DataGet_Base->open()==FALSE)  return 0;

    QString   requete  = "DELETE FROM version";
    QSqlQuery sqlQuery (requete, m_DataGet_Base);
    OutSQL_error( sqlQuery, "SetDatasempIndexVersion()", requete);

    requete = "INSERT INTO `version` (`DatasempVersion`) VALUES ('"+version+"')";
    sqlQuery.exec(requete);
    OutSQL_error( sqlQuery, "SetDatasempIndexVersion()", requete);

    m_DataGet_Base->close();
    return version;
}

//----------------------------------------------------- Medica_CreateTablePosologies ---------------------------------------------------------------
bool CMedicaBase::Medica_CreateTablePosologies(const QString &table_name, int dropBefore /*=1*/)
{bool ret;
 QString requeteDst = "";
 QSqlQuery* pSqlQueryDst = 0;
 //............................ effacer la table si elle existe .....................................
 if (dropBefore)
    {requeteDst     = requeteDst + "DROP TABLE IF EXISTS " + table_name;
     pSqlQueryDst   = new QSqlQuery(requeteDst , m_MedicaBase );
     ret = OutSQL_error(*pSqlQueryDst, "Medica_CreateTablePosologies(): Error drop table : " + table_name, requeteDst);
     if (pSqlQueryDst)  delete  pSqlQueryDst;
     if (ret==FALSE) return FALSE;
     requeteDst = " CREATE TABLE `" + table_name + "` (";
    }
 else
    {requeteDst = " CREATE TABLE IF NOT EXISTS `" + table_name + "` (";
    }
 //............................ creer la table  .....................................

 requeteDst += "    `" +            m_MEDICA_POSO_PK             + "`       int(11)    NOT NULL auto_increment, "
               "    `" +            m_MEDICA_POSO_EXTRA_PK       + "`       int(11)    default NULL, "
               "    `" +            m_MEDICA_POSO_CIP            + "`       varchar(7) BINARY NOT NULL, "
               "    `" +            m_MEDICA_POSO_LIBELLE        + "`       char(128)  BINARY default NULL , "
               "    `" +            m_MEDICA_POSO_AGE_MIN        + "`       int(11)    default NULL , "
               "    `" +            m_MEDICA_POSO_AGE_MAX        + "`       int(11)    default NULL , "
               "    `" +            m_MEDICA_POSO_SEXE           + "`       char(1)    BINARY default NULL, "
               "    `" +            m_MEDICA_POSO_TERRAIN        + "`       char(64)   BINARY default NULL, "
               "    `" +            m_MEDICA_POSO_POIDS_MIN      + "`       float      default NULL, "
               "    `" +            m_MEDICA_POSO_POIDS_MAX      + "`       float      default NULL, "
               "    `" +            m_MEDICA_POSO_DOSE_MIN       + "`       float      default NULL, "
               "    `" +            m_MEDICA_POSO_DOSE_MAX       + "`       float      default NULL, "
               "    `" +            m_MEDICA_POSO_DOSE_UNIT      + "`       int(11)    default NULL, "
               "    `" +            m_MEDICA_POSO_DOSE_LIMIT_MAX + "`       float      default NULL, "
               "    `" +            m_MEDICA_POSO_FACT_CORP_QU   + "`       float      default NULL, "
               "    `" +            m_MEDICA_POSO_FACT_CORP_UNIT + "`       int(11)    default NULL, "
               "    `" +            m_MEDICA_POSO_PERIODE        + "`       float      default NULL, "
               "    `" +            m_MEDICA_POSO_EQUI_COEFF     + "`       float      default NULL, "
               "    `" +            m_MEDICA_POSO_UNIT_COEFF     + "`       int(11)    default NULL, "
               "    `" +            m_MEDICA_POSO_NB_PRISES      + "`       int(11)    default NULL, "
               "    `" +            m_MEDICA_POSO_REPART_PRISES  + "`       char(128)  BINARY default NULL, "
               "    `" +            m_MEDICA_POSO_DIVERS         + "`       char(32)   BINARY default NULL, "
               "    `" +            m_MEDICA_POSO_PENDANT        + "`       float      default NULL, "
               "    `" +            m_MEDICA_POSO_SECABILITE     + "`       char(1)    BINARY  NULL, "
               "     PRIMARY KEY  ("+ m_MEDICA_POSO_PK + ") "
               "  );";
 pSqlQueryDst = new QSqlQuery(requeteDst , m_MedicaBase );
 ret = OutSQL_error(*pSqlQueryDst, "Medica_CreateTablePosologies(): Error table create : " + table_name , requeteDst);
 if (pSqlQueryDst) delete  pSqlQueryDst;
 return ret;
}

//----------------------------------------------------- Medica_CreateTableIndexProduit ---------------------------------------------------------------
bool CMedicaBase::Medica_CreateTableIndexProduit(const QString &table_name)
{bool ret;
 QString requeteDst = "";
 //............................ effacer la table si elle existe .....................................
 requeteDst         = requeteDst + "DROP TABLE IF EXISTS " + table_name;
 QSqlQuery* pSqlQueryDst = new QSqlQuery(requeteDst , m_MedicaBase );
 ret = OutSQL_error(*pSqlQueryDst, "Medica_CreateTableIndexProduit(): Error drop table : " + table_name, requeteDst);
 if (pSqlQueryDst)  delete  pSqlQueryDst;
 if (ret==FALSE) return FALSE;

 //............................ creer la table  .....................................
 requeteDst  = "";
 requeteDst += " CREATE TABLE `"  + table_name ;
 requeteDst += "` ( `" +            m_MEDICA_INDEX_PK       + "`       int(11)    NOT NULL auto_increment, "
               "    `" +            m_MEDICA_INDEX_LIBELLE  + "`       varchar(45) BINARY default NULL, "
               "    `" +            m_MEDICA_INDEX_CIP      + "`       char(7)  BINARY default NULL , "
               "    `" +            m_MEDICA_INDEX_PK_T00   + "`       int(11)  NOT NULL , "
               "    `" +            m_MEDICA_INDEX_PK_T4B   + "`       int(11)  default NULL , "
               "    `" +            m_MEDICA_INDEX_CD_PROD  + "`       char(6)  BINARY default NULL, "
               "    `" +            m_MEDICA_INDEX_CD_UV    + "`       char(10) BINARY default NULL, "
               "    `" +            m_MEDICA_INDEX_IS_GENE  + "`       char(1)  BINARY default NULL, "
               "    `" +            m_MEDICA_INDEX_IS_DISPO + "`       char(4)  BINARY default NULL, "
               "     PRIMARY KEY  ("+ m_MEDICA_INDEX_PK + ") "
               "  );";
 pSqlQueryDst = new QSqlQuery(requeteDst , m_MedicaBase );
 ret = OutSQL_error(*pSqlQueryDst, "Medica_CreateTableIndexProduit(): Error table create : " + table_name , requeteDst);
 if (pSqlQueryDst) delete  pSqlQueryDst;
 return ret;
}

//----------------------------------------------------- Medica_CreateTableSpecificationProduit ---------------------------------------------------------------
bool CMedicaBase::Medica_CreateTableSpecificationProduit(const QString &table_name)
{bool ret;
 QString requeteDst = "";
 //............................ effacer la table si elle existe .....................................
 requeteDst         = requeteDst + "DROP TABLE IF EXISTS " + table_name;
 QSqlQuery* pSqlQueryDst = new QSqlQuery(requeteDst , m_MedicaBase );
 ret = OutSQL_error(*pSqlQueryDst, "ConvertMysteriousBaseToMedicaTux(): Error drop table : " + table_name, requeteDst);
 if (pSqlQueryDst)  delete  pSqlQueryDst;
 if (ret==FALSE) return FALSE;

 //............................ creer la table  .....................................
 requeteDst  = "";
 requeteDst += " CREATE TABLE `"  + table_name ;
 requeteDst += "` ( `" +            m_MEDICA_SPEC_CIP             + "`       char(7)  BINARY NOT NULL , "
               "    `" +            m_MEDICA_SPEC_UP_UNIT         + "`       int(11)  default NULL , "        // Pk Table forme et unites: (comprimé ampoules etc ...)   UPUnit
               "    `" +            m_MEDICA_SPEC_PA_UNIT         + "`       int(11)  default NULL , "        // Pk Table forme et unites:   Unite de principe actif  (mg ml etc...):                                PAUnit
               "    `" +            m_MEDICA_SPEC_PA_QU           + "`       float    default NULL , "        // Quantite de principe actif par unité de prise (500):                       QPAbyUP
               "    `" +            m_MEDICA_SPEC_SECABILITE      + "`       char(1)  BINARY default NULL, "  // 0 (insecable) 2 (en 2) 3 (en 3)  etc..
               "    `" +            m_MEDICA_SPEC_IS_GENERIC      + "`       char(2)  BINARY default NULL, "  // G  pour genérique R pour réference d'un groupe ATC
               "    `" +            m_MEDICA_SPEC_FORM_STRUC      + "`       int(11)  default NULL, "         // Pk Table forme et unites: forme structurée (solution buvable injectable etc ...)
               "    `" +            m_MEDICA_SPEC_NB_FORME        + "`       int(11)  default NULL, "
               "    `" +            m_MEDICA_SPEC_PRIX_BOITE      + "`       float    default NULL, "
               "    `" +            m_MEDICA_SPEC_REMBOURSEMENT   + "`       char(32) BINARY default NULL, "
               "    `" +            m_MEDICA_SPEC_TABLEAU         + "`       char(1)  BINARY default NULL, "
               "    `" +            m_MEDICA_SPEC_LABO            + "`       char(45) BINARY default NULL, "
               "    `" +            m_MEDICA_SPEC_ATC             + "`       char(7)  BINARY default NULL, "
               "     PRIMARY KEY  ("+ m_MEDICA_SPEC_CIP + ") "
               "  );";
 pSqlQueryDst = new QSqlQuery(requeteDst , m_MedicaBase );
 ret = OutSQL_error(*pSqlQueryDst, "ConvertMysteriousBaseToMedicaTux(): Error table create : " + table_name , requeteDst);
 if (pSqlQueryDst) delete  pSqlQueryDst;
 return ret;
}

//----------------------------------------------------- Medica_CreateTableFormeProduit ---------------------------------------------------------------
bool CMedicaBase::Medica_CreateTableFormeProduit(const QString &table_name, int dropBefore /*=1*/)
{bool ret;
 QString requeteDst = "";
 QSqlQuery* pSqlQueryDst = 0;
 if ( m_MedicaBase==0 )                                              return false;
 if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)   return FALSE;
  //............................ effacer la table si elle existe .....................................
 if (dropBefore)
    {requeteDst         = requeteDst + "DROP TABLE IF EXISTS " + table_name;
     pSqlQueryDst = new QSqlQuery(requeteDst , m_MedicaBase );
     ret = OutSQL_error(*pSqlQueryDst, "Medica_CreateTableFormeProduit(): Error drop table : " + table_name, requeteDst);
     if (pSqlQueryDst)  delete  pSqlQueryDst;
     if (ret==FALSE) return FALSE;
     requeteDst = " CREATE TABLE `" + table_name + "` (";
    }
 else
    {requeteDst = " CREATE TABLE IF NOT EXISTS `" + table_name + "` (";
    }
 //............................ creer la table  .....................................

 requeteDst += "    `" +              m_MEDICA_FORME_PK             + "`       int(11)  NOT NULL , "
               "    `" +              m_MEDICA_FORME_LIBELLE        + "`       char(40) BINARY default NULL , "        // Pk Table forme et unites: (comprimé ampoules etc ...)   UPUnit
               "     PRIMARY KEY  ("+ m_MEDICA_FORME_PK + ") "
               "  );";
 pSqlQueryDst = new QSqlQuery(requeteDst , m_MedicaBase );
 ret = OutSQL_error(*pSqlQueryDst, "Medica_CreateTableFormeProduit(): Error table create : " + table_name , requeteDst);
 if (pSqlQueryDst) delete  pSqlQueryDst;
 return ret;
}

//----------------------------------------------------- Medica_CreateTableFormeProduit ---------------------------------------------------------------
bool CMedicaBase::Medica_CreateTableFormeProduit(const QString &table_name, QStringList &formes)
{bool ret;
 ret = Medica_CreateTableFormeProduit(table_name);
 if (ret)
    {int next = 0;
     for ( QStringList::Iterator it = formes.begin(); it != formes.end(); ++it )
         {++next;
          QSqlCursor cur( table_name, TRUE, m_MedicaBase );
          if ( cur.canInsert ())
            { //............... si OK on recupere le buffer et on le charge avec les données .........
             QSqlRecord *buffer = cur.primeInsert();                         // recuperer le buffer d'insertion
             buffer->setValue( m_MEDICA_FORME_PK,           QString::number(next)    );
             buffer->setValue( m_MEDICA_FORME_LIBELLE,      *it     );
             if ( !cur.insert() )
                {
                }
            }
         }
    }
 return ret;
}

//----------------------------------------------------- Medica_CreateTableATC ---------------------------------------------------------------
bool CMedicaBase::Medica_CreateTableATC(const QString &table_name)
{bool ret;
 QString requeteDst = "";
 //............................ effacer la table si elle existe .....................................
 requeteDst         = requeteDst + "DROP TABLE IF EXISTS " + table_name;
 QSqlQuery* pSqlQueryDst = new QSqlQuery(requeteDst , m_MedicaBase );
 ret = OutSQL_error(*pSqlQueryDst, "Medica_CreateTableATC(): Error drop table : " + table_name, requeteDst);
 if (pSqlQueryDst)  delete  pSqlQueryDst;
 if (ret==FALSE) return FALSE;

 //............................ creer la table  .....................................
 requeteDst  = "";
 requeteDst += " CREATE TABLE `"    + table_name ;
 requeteDst += "` ( `" +              m_MEDICA_ATC_CODE      + "`       char(7)  NOT NULL , "
               "    `" +              m_MEDICA_ATC_LIBELLE   + "`       char(127) BINARY default NULL , "
               "     PRIMARY KEY  ("+ m_MEDICA_ATC_CODE + ") "
               "  );";
 pSqlQueryDst = new QSqlQuery(requeteDst , m_MedicaBase );
 ret = OutSQL_error(*pSqlQueryDst, "Medica_CreateTableATC(): Error table create : " + table_name , requeteDst);
 if (pSqlQueryDst) delete  pSqlQueryDst;
 return ret;
}

//----------------------------------------------------- Medica_GetSpecificationsPrise ---------------------------------------------------------------------------
QString CMedicaBase::Medica_GetSpecificationsPrise(QString pk)
{ QString      requete("SELECT ");
  if ( m_MedicaBase==0 )                                              return QString::null;
  if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)   return QString::null;
  //................. chercher données economiques pour ce code CIP...................................................
  requete +=             m_MEDICA_POSO_REPART_PRISES          +
             " FROM "  + m_MEDICA_POSO_TBL_NAME               + " WHERE " + m_MEDICA_POSO_PK + " ='" + pk + "'";

  QSqlQuery query (requete , m_MedicaBase );
  if (query.isActive() && query.next()) return Utf8_Query(query,0);
  return QString::null;
}

//----------------------------------------------------- Medica_GetCodeATC ---------------------------------------------------------------------------
QString CMedicaBase::Medica_GetCodeATC(QString cip,
                                       QString pkt4b
                                      )
{
 if ((int)(Medica_GetBaseMode() & CMedicaBase::Datasemp))
    {if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE) return "";
     //................... si accelerateurs de recherche non renseignés, les renseigner ..........................
     //                    ultra lent !!!!
     if (pkt4b.length()==0 )
        {//        0  code produit    ___
         //                              |
         QSqlQuery query(QString("SELECT f0 FROM t00 WHERE f2 = '") + cip + "'" , m_DataBaseSrce );
         if (! (query.isActive() && query.next()))                         return "";
         Datasemp_GetField("t4B", "f0", query.value(0).toString(), "f2", &pkt4b);
        }
     QSqlQuery querym(QString("SELECT fA FROM t4B WHERE ge = ") + pkt4b  , m_DataBaseSrce );
     if (querym.isActive() && querym.next())                               return querym.value( 0 ).toString();
    }
 else
    {QString requete("");
     if ( m_MedicaBase==0 )                                                return "";
     if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)     return "";
     requete       += "SELECT " + m_MEDICA_SPEC_ATC      +
                      " FROM "  + m_MEDICA_SPEC_TBL_NAME + " WHERE " + m_MEDICA_SPEC_CIP + " = '" + cip +"'";
     QSqlQuery query(requete , m_MedicaBase );
     //.................. si la requète a un resultat ..............................................
     if (query.isActive() && query.next())                                 return query.value( 0 ).toString();
    }
 return QString("");
}

//----------------------------------------------------- Medica_PosologieListDeserialize ---------------------------------------------------------------------------
long CMedicaBase::Medica_PosologieListDeserialize(const QString &ordoStruct, QPL_CPosologie &list_CPosologie)
{list_CPosologie.clear();
 long    nb = 0;
 int    pos = 0;
 pos = ordoStruct.find("<PosologieList>");
 if (pos==-1) { pos = ordoStruct.find("<Posologie>");
              }
 else           pos += 15;
 while ( (pos = ordoStruct.find("<Posologie>", pos)) != -1)
       {
        QPL_CPosologie::iterator it = list_CPosologie.append(CPosologie());
        pos = (*it).setDataFrom_XML_String(ordoStruct, pos);
        ++nb;
       }
 return nb;
}

//----------------------------------------------------- Medica_PosologieListSerialize ---------------------------------------------------------------------------
long CMedicaBase::Medica_PosologieListSerialize( QString &ordoStruct,  QPL_CPosologie &list_CPosologie)
{ordoStruct = "";
 long    nb = 0;
 //................... Données posologiques ..................................
 if (list_CPosologie.count())
    {ordoStruct += "<PosologieList>\n";         // entete de liste de posologie
     QPL_CPosologie::iterator it;
     for (it = list_CPosologie.begin(); it !=  list_CPosologie.end(); ++it )   // it != m_RecSpeclist.end();
         {ordoStruct += (*it).to_XML_String();
          ++nb;
         }
     ordoStruct += "</PosologieList>\n";         // entete de liste de posologie
    }
 return nb;
}

//----------------------------------------------------- Medica_PosologieListSerialize ---------------------------------------------------------------------------
QString CMedicaBase::Medica_PosologieListSerialize( QPL_CPosologie &list_CPosologie)
{QString ordoStruct;
 Medica_PosologieListSerialize(ordoStruct, list_CPosologie);
 return ordoStruct;
}

//-------------------------------------- ExtractNumber ----------------------------------------------------------------
float CMedicaBase::Medica_ExtractNumber(const QString text)
{QString str("");
 int end = text.length();
 int   i = -1;
 while (++i < end)
       {if (  (text[i]>='0' && text[i]<='9') || text[i]=='.'|| text[i]==',')                       str += text[i];
        else if ((end-i >=4) && text[i]==' '&& text[i+1]=='u' && text[i+2]=='n'&& text[i+3]==' ') {str += "1"; i+=4;}
       }
 return str.toFloat();
}

//------------------------------------------- Medica_initComboDuree --------------------------------------------------------------
void CMedicaBase::Medica_initComboDuree(QComboBox *comboBoxPendant)
{
 // [Duree]
 //   DureePrescription = 1-2-3-4-5-6-7-8-9-10-12-15-21-28-31-62-92-182-365
 //   DureeUnite = Jours
 //   NombreAccessoires = 1-2-3-4-5-6-7-8-9-10-12-15-21-28-31-62-92-182-365
 //   NombreUnite= élément(s)
 //   Duree par defaut = 8
 QString str       = "";
 QString defaut    = TR("(Vide)");
 CGestIni::Param_ReadParam(m_IniParam, "Duree", "DureePrescription", &str);
 if (str.length()==0) str = "1-2-3-4-5-6-7-8-9-10-12-15-21-28-31-62-92-182-365";

 QStringList list = QStringList::split("-", str);
 CGestIni::Param_ReadParam(m_IniParam, "Duree", "Duree par defaut", &str);   // noter dur� par defaut

 comboBoxPendant->clear();
 new CPrtQListBoxItem(    comboBoxPendant->listBox() ,  // list box du combobox
                          defaut,                       // libelle    tr("(Vide)");
                          "0",                          // nombre de jours
                          "",                           // non utilisé
                          "",                           // non utilisé
                          "" );                         // non utilisé
 for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
     {new CPrtQListBoxItem(    comboBoxPendant->listBox() ,                          // list box du combobox
                               Medica_PosologieDureeJourToHtml(*it),                 // libelle
                               *it,                                                  // nombre de jours
                               "",                                                   // non utilisé
                               "",                           // non utilisé
                               "" );                                                 // non utilisé
      if (defaut == TR("(Vide)") && str==*it) defaut = Medica_PosologieDureeJourToHtml(*it);
     }
 comboBoxPendant->setCurrentText ( defaut );
 comboBoxPendant->setEditText ( defaut );
 QListBoxItem * pQListBoxItem = comboBoxPendant->listBox()->findItem ( defaut, Qt::ExactMatch );
 if (pQListBoxItem) comboBoxPendant->listBox()->setCurrentItem (pQListBoxItem ) ;
}

//------------------------------------------- Medica_SetComboOnDuree --------------------------------------------------------------
void CMedicaBase::Medica_SetComboOnDuree(QComboBox *comboBoxPendant, const QString &duree, const char* defaultDuree /* = 0 */)
{
 // [Duree]
 //   DureePrescription = 1-2-3-4-5-6-7-8-9-10-12-15-21-28-31-62-92-182-365
 //   DureeUnite = Jours
 //   NombreAccessoires = 1-2-3-4-5-6-7-8-9-10-12-15-21-28-31-62-92-182-365
 //   NombreUnite= élément(s)
 //   Duree par defaut = 8

QListBox *listBox = comboBoxPendant->listBox ();
int nb           = listBox->count();
double dbl_duree = duree.toDouble();
for (int i=0; i<nb; ++i)
    {
      CPrtQListBoxItem *pCPrtQListBoxItem = (CPrtQListBoxItem*)listBox->item (i);
      if (pCPrtQListBoxItem)
          {QString val1, nb_jrs, val3, val4;
           pCPrtQListBoxItem->GetValue(nb_jrs, val1, val3, val4) ;
           if ( (nb_jrs.toDouble() * 86400) == dbl_duree)
              { listBox->setCurrentItem(i);
                comboBoxPendant->setCurrentText(Medica_PosologieDureeToHtml(duree));
                if (dbl_duree<=0 && defaultDuree) comboBoxPendant->lineEdit()->setText(defaultDuree);
                return;
              }
          }
     }
 dbl_duree = dbl_duree / 86400;
 QString                     str = "";
 if (dbl_duree==365)         str = TR("un an");
 else if (dbl_duree==31)     str = TR("un mois");
 else if (dbl_duree<=1)      str = TR("un jour");
 else                        str = QString::number(dbl_duree) + TR(" jours");
 if (str.stripWhiteSpace().length()==0 && defaultDuree)  str = defaultDuree;
 comboBoxPendant->lineEdit()->setText(str);
}

//------------------------------------------- Medica_GetComboCPrtDuree --------------------------------------------------------------
float CMedicaBase::Medica_GetComboCPrtDuree(QComboBox *comboBoxPendant)
{
 // [Duree]
 //   DureePrescription = 1-2-3-4-5-6-7-8-9-10-12-15-21-28-31-62-92-182-365
 //   DureeUnite = Jours
 //   NombreAccessoires = 1-2-3-4-5-6-7-8-9-10-12-15-21-28-31-62-92-182-365
 //   NombreUnite= élément(s)
 //   Duree par defaut = 8

QListBox *listBox = comboBoxPendant->listBox ();
float dbl_duree = 0;
QString nb_jrs, val1, val3, val4;                                                             // recupérer la valeur associée au pCPrtQListBoxItem
CPrtQListBoxItem *pCPrtQListBoxItem = (CPrtQListBoxItem*)listBox->item (listBox->currentItem ());
if (pCPrtQListBoxItem && pCPrtQListBoxItem->text()== comboBoxPendant->lineEdit()->text())   // si texte de la lineEdit correspond à celui de la liste courante
   {pCPrtQListBoxItem->GetValue(nb_jrs, val1, val3,val4) ;
    dbl_duree = nb_jrs.toFloat()*86400;
   }
 else                                                                                      // sinon se servir de la ligne courante
   {dbl_duree = Medica_dureeMenuTextToSecond( comboBoxPendant->lineEdit()->text() );       // et essayer de convertir
   }
return dbl_duree;
}

//----------------------------------------------------- dureeMenuTextToSecond ----------------------------------------------------------------------------------
float CMedicaBase::Medica_dureeMenuTextToSecond( const QString &text )
{QString t = text.lower();
 float n   = Medica_ExtractNumber(t);
 if (n<=0) return 0;
 if (text.find("m")!= -1)
    {if (n==12) return   86400*365;
     else       return   n*86400*31;
    }
 else if (text.find("a")!= -1)
    {return  n*86400*365;
    }
else if (text.find("se")!= -1)
    {return  n*86400*7;
    }
 else
    {return n*86400;
    }
 return 0;
}

//-------------------------------------- Medica_PosologieListToXMLOrdoStruct --------------------------------------------------------------------------------------------
QString  CMedicaBase::Medica_PosologieListToXMLOrdoStruct( QPL_CPosologie list_CPosologie, const QString &spoids, const QString &staille , const QString &lap_ordo)
{QString result = "";
 if ( lap_ordo.length()==0 )    // vielle version 
    { result =  Medica_PosologieToHtmlOrdo(list_CPosologie, spoids, staille, CMedicaBase::PrintableForm);
      result.insert (0, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\" ?>\n<ordotext>\n");
      result.append ( "\n</ordotext>\n\n<ordoMedicaStruct>\n");
      result.append ( Medica_PosologieListSerialize(list_CPosologie));
      result.append ( "</ordoMedicaStruct>\n");
    }
 else                           // nouvelle version
    {int nextPos = 0;
     //............. recuperer la partie html .........................................
     result  =  "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\" ?>\n"
                "<ordotext>\n";               //....... recuperer partie html que l'on conserve telle qu'elle est .........
     result +=   CGestIni::getXmlData("ordotext", lap_ordo, &nextPos, 1).stripWhiteSpace();      // 1 pour ne pas convertir '&gt;' en '>'  '&lt;' en '<'  '&amp;' en '&'
     result +=  "</ordotext>\n";
     result +=  "<ordoMedicaStruct>\n";       //....... convertir la nouvelle structure en acienne .........
     QString lap_posoList =  CGestIni::getXmlData("Posologie_LAP_List", lap_ordo, &nextPos, 1).stripWhiteSpace();
     result +=   Medica_Has_StructDataToOldXmlStruct( lap_posoList ); // sont inclus "<PosologieList>\n" et "</PosologieList>\n"
     result +=   "\n<Posologie_LAP_List>\n";  //....... ajouter partie xml que l'on conserve telle qu'elle est .........
     result +=   lap_posoList;
     result +=   "\n</Posologie_LAP_List>\n";
     result +=  "</ordoMedicaStruct>\n";
    }
 return result;
}
/*----------------------------------------------------- Medica_DiskDataSplitIn_HtmlData_StructData ------------------------------------------------------*/
// ACTION: separe à partir des données brutes (XML) d'une ordonnance structurée de la base de données,
//        les données Html directement affichables, des données structurées ayant produit cet affichage.
// ENTREE: const char *txt : pointeur sur la zone XML de stockage des données brutes
// SORTIE: QString *stringDST_text   : si pas egal à zero reçoit les données html affichables
//         QString *stringDST_struct : si pas egal à zero reçoit les données structurelles calculables
// RETOUR: long : longueur des données Html

long CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData(const QString &txt, QString *stringDST_text /*=0 */, QString *stringDST_struct /*=0 */)
{if (txt.length()<=0 ) return 0;
 //......................................... scan et analyse du texte ..................................
 int pos_deb = txt.find("<ordotext>");                 if (pos_deb == -1)      return 0;
 pos_deb    += 10;
 int pos_end = txt.find("</ordotext>",pos_deb);        if (pos_end == -1)      return 0;
 if (stringDST_text) *stringDST_text = txt.mid(pos_deb, pos_end - pos_deb).stripWhiteSpace();;

 //............ analyse de la sortie .............................
 pos_deb     = txt.find("<PosologieList>",  pos_end +11); if (pos_deb == -1)    return 0;
 pos_deb    += 15;
 pos_end     = txt.find("</PosologieList>", pos_deb);     if (pos_end == -1)    return 0;

 if (stringDST_struct) *stringDST_struct = txt.mid(pos_deb, pos_end - pos_deb).stripWhiteSpace();

 return pos_end - pos_deb;
}

/*----------------------------------------------------- Medica_DiskDataSplitIn_HtmlData_HAS_StructData ------------------------------------------------------*/
// ACTION: separe à partir des données brutes (XML) d'une ordonnance structurée de la base de données,
//        les données Html directement affichables, des données structurées ayant produit cet affichage.
// ENTREE: const char *txt : pointeur sur la zone XML de stockage des données brutes
// SORTIE: QString *stringDST_text   : si pas egal à zero reçoit les données html affichables
//         QString *stringDST_struct : si pas egal à zero reçoit les données structurelles calculables
// RETOUR: long : longueur des données Html

long CMedicaBase::Medica_DiskDataSplitIn_HtmlData_HAS_StructData(const QString &txt, QString *stringDST_text /*=0 */, QString *stringDST_struct /*=0 */)
{if (txt.length()<=0 ) return 0;
 //......................................... scan et analyse du texte ..................................
 int pos_deb = txt.find("<ordotext>");                 if (pos_deb == -1)      return 0;
 pos_deb    += 10;
 int pos_end = txt.find("</ordotext>",pos_deb);        if (pos_end == -1)      return 0;
 if (stringDST_text) *stringDST_text = txt.mid(pos_deb, pos_end - pos_deb).stripWhiteSpace();;

 //............ analyse de la sortie .............................

 pos_deb     = txt.find("<Posologie_LAP_List>",  pos_end +11); if (pos_deb == -1)    return 0;
 pos_deb    += 20;
 pos_end     = txt.find("</Posologie_LAP_List>", pos_deb);     if (pos_end == -1)    return 0;
 if (stringDST_struct) *stringDST_struct = txt.mid(pos_deb, pos_end - pos_deb).stripWhiteSpace();

 return pos_end - pos_deb;
}

/*----------------------------------------------------- Medica_Has_StructDataToOldXmlStruct ------------------------------------------------------*/
QString   CMedicaBase::Medica_Has_StructDataToOldXmlStruct(const QString &hasXml)
{
   /*
                "<gph>\n"                          // -- DEBUT du header d'une ligne prescriptive ---
                " <gph_html></gph_html>\n"         //        contenu html (accelere l'affichage)
                " <gph_ald></gph_ald>\n"           //        prescrit en ALD "ALD"  sans ALD ""
                " <gph_id></gph_id>\n"             //        id (cip)
                " <gph_it></gph_it>\n"             //        id_type
                " <gph_dr></gph_dr>\n"             //        date et heure de la redaction de la prescription    format dd-MM-yyyy hh:mm:ss
                " <gph_dt></gph_dt>\n"             //        date et heure du debut  de prise de la prescription format dd-MM-yyyy hh:mm:ss // la date de fin est calculee avec <gph_cy>
                " <gph_na></gph_na>\n"             //        nom commercial et usuel du produit
                " <gph_dcl>\n"                     //        - debut de la liste des dci composant le produit
                "  <gph_dc>\n"                     //        -- debut du premier element de la liste des dci
                "    <gph_dcn></gph_dcn>\n"        //        --- nom   du premier element de la liste des dci
                "    <gph_dcp></gph_dcp>\n"        //        --- dose  du premier element de la liste des dci
                "    <gph_dcu></gph_dcu>\n"        //        --- unite du premier element de la liste des dci
                "  </gph_dc>\n"                    //        --fin du premier element de la liste des dci
                " </gph_dcl>\n"                    //        - fin de la liste des dci composant le produit
                " <gph_cy></gph_cy>\n"             //        duree totale de toutes les sequences (J15  S10 M10   M3R2) 'J' pour jours  ou 'S' pour semaines  ou 'M' pour mois  suivi du nbr et falcutatif 'R' pour renouvelable suivi du nbr
                " <gph_sb></gph_sb>\n"             //        substituable 0 = non substituable  1 = substituable
                " <gph_dci></gph_dci>\n"           //        affichage libelle produit : 3=que dci  2=dci+commercial 1=commercial+dci 0=que commercial (par defaut si vide c'est 3)
                " <gph_uf></gph_uf>\n"             //        forme de l'unite de prise
                " <gph_voie></gph_voie>\n"         //        voie d'administration
                " <gph_fmin></gph_fmin>\n"         //        frequence minimum
                " <gph_fmax></gph_fmax>\n"         //        frequence maximum
                " <gph_funit></gph_funit>\n"       //        frequence unite
                " <gph_pmint></gph_pmin>\n"        //        posologie minimum
                " <gph_pmax></gph_pmax>\n"         //        posologie maximum
                " <gph_punit></gph_punit>\n"       //        posologie unite
                " <gph_pfc></gph_pfc>\n"           //        posologie facteur corporel qu de gph_punit par facteur corporel
                " <gph_pfcunit></gph_pfcunit>\n"   //        posologie facteur corporel unite Kg cm
                " <gph_pqbyuf></gph_pqbyuf>\n"     //        quantite d'unite posologique par unite de forme de gph_punit par gph_uf
                " <gph_dmin></gph_dmin>\n"         //        duree minimum
                " <gph_dmax></gph_dmax>\n"         //        duree maximum
                " <gph_dunit></gph_dunit>\n"       //        duree unite
                " <gph_nm></gph_nm>\n"             //        conversion mode en mode numerique UP_QUANTITE MOMENT_MEAL ALL_LITERAL (on se sert de celui-ci car plus rapide a interpreter)
                " <gph_cm></gph_cm>\n"             //        conversion mode en mode literal pour information lisible de celui du mode numerique  UP_QUANTITE MOMENT_MEAL ALL_LITERAL on a pas peur des redondances ! (vive la memoire vive et les HD)
                " <gph_in>\n"                      //        liste des indications
                "  <gph_il></gph_il>\n"            //        premier element de la liste des indications
                " </gph_in>\n"                     //        fin de la liste des indications <gph_il> </gph_il>
                " <gph_co></gph_co>\n"             //        commentaire global en fin de prescription
                "</gph>\n"                         // -- FIN du header d'une ligne prescriptive ---
                "<gps>\n"                          // -- DEBUT de la liste des sequences pescriptives
                " <gpp></gpp>\n"                   //        premiere sequence prescriptive
                "</gps>\n"                         // -- FIN de la liste des sequences pescriptives
                "<gpi_engine></gpi_engine>\n"      //        systeme de donnees a l'origine de cette ligne prescriptive
                "<gpi_user></gpi_user>\n"          //        utilisateur a l'origine de cette ligne prescriptive
*/

 QString stringDST = "<PosologieList>\n";
 QStringList  list = CGestIni::getXmlDataList("OrdoLine", hasXml);
 int             i = 0;
 int             s = 0;
 for ( i = 0; i<(int)list.size();++i)
     { QString grammar          = list[i];
       QString schemaPrise      = "";
       QString q_min            = "";
       QString q_max            = "";
       int nextPos              = 0;
       QString    ald           = CGestIni::getXmlData(     "gph_ald",    grammar, &nextPos);
       QString    cip           = CGestIni::getXmlData(     "gph_id",     grammar, &nextPos);
       QString    idType        = CGestIni::getXmlData(     "gph_it",     grammar, &nextPos);
       //QString    s_dateRed     = CGestIni::getXmlData(     "gph_dr",    grammar, &nextPos).left(10);
       //QString    s_dateDeb     = CGestIni::getXmlData(     "gph_dt",    grammar, &nextPos).left(10);
       QString commercialName    = CGestIni::getXmlData(     "gph_na",    grammar, &nextPos);
       //QString compoXmlParts    = CGestIni::getXmlData(     "gph_dcl",   grammar, &nextPos);
       QString cycle              = CGestIni::getXmlData(     "gph_cy",    grammar, &nextPos);
       //QString substituable     = CGestIni::getXmlData(     "gph_sb",    grammar, &nextPos);
       //int     dciMode          = CGestIni::getXmlData(     "gph_dci",   grammar, &nextPos).toInt();
       QString up_forme         = CGestIni::getXmlData(     "gph_uf",    grammar, &nextPos);
       //QString voie             = CGestIni::getXmlData(     "gph_voie",  grammar, &nextPos);
       //QString conv_mod_T       = CGestIni::getXmlData(     "gph_cm",    grammar, &nextPos);
       //QString conv_mod_N       = CGestIni::getXmlData(     "gph_nm",    grammar, &nextPos);
       QString comment          = CGestIni::getXmlData(     "gph_co",    grammar, &nextPos);
       QStringList seqList      = CGestIni::getXmlDataList( "gpp",       grammar, &nextPos);

       QString nb_prises = "0";                  // nombre de fois par jour 
       for ( s=0; s < 1; /* seqList.count(); */ ++s)       // < 1  car l'ancien systeme ne connait pas la crise des sequences multiples
           { //....... reperer si la sequence comporte une forme locale ..............
             //        et auquel cas l'utiliser
             QString sequence        = seqList[s];
             int     pos_deb         = sequence.find('[');
             if (pos_deb != -1 )
                { up_forme           = sequence.left(pos_deb);
                  sequence           = sequence.mid(pos_deb+1);
                }
             schemaPrise = Medica_sequenceToOldMinMaxPoso(sequence, q_min,  q_max, nb_prises);
           }
       if (q_min.length()==0 && q_max.length())
          {q_min = q_max;
           q_max = "";
          }
       QString posoAdjust = "1";
       if (schemaPrise.length()==0 && q_min.length() && q_max.length())  // si  1 a 4 par jour
          {posoAdjust = "-1";
           nb_prises  = "0";
          }
       int  posoType  = comment.length() ? CPosologie::Textuelle|CPosologie::Structuree : CPosologie::Structuree;
       QString result = "";
               result   += "<Posologie>\n";
               result   += "<numVersion>"      "2"                                                          "</numVersion>\n";
               result   += "<cip>"             + cip                                      +                 "</cip>\n";
               result   += "<pk>"               ""                                                          "</pk>\n";
               result   += "<libelle>"          ""                                                          "</libelle>\n";
               result   += "<sexe>"             "M"                                                         "</sexe>\n";
               result   += "<terrain>"          ""                                                          "</terrain>\n";
               result   += "<doseMin>"         + q_min                                    +                 "</doseMin>\n";
               result   += "<doseMax>"         + q_max                                    +                 "</doseMax>\n";
               result   += "<doseUnit>"        + textTo_UP_Code( up_forme )               +                 "</doseUnit>\n";
               result   += "<doseLimitMax>"     "0"                                                         "</doseLimitMax>\n";
               result   += "<factCorpQU>"       "0"                                                         "</factCorpQU>\n";
               result   += "<factCorpUnit>"     "2"                                                         "</factCorpUnit>\n";
               result   += "<periode>"          "86400"                                                     "</periode>\n";
               result   += "<equiCoeff>"        "0"                                                         "</equiCoeff>\n";
               result   += "<unitCoeff>"        "101"                                                       "</unitCoeff>\n";
               result   += "<nbPrises>"        + nb_prises                               +                  "</nbPrises>\n";
               result   += "<schemaPrise>"     + schemaPrise                             +                  "</schemaPrise>\n";
               result   += "<divers>"          + CPosologie::StrtoXml( commercialName )   +                 "</divers>\n";
               result   += "<extraPk>"          ""                                                          "</extraPk>\n";
               result   += "<note>"            + CPosologie::StrtoXml( comment )         +                  "</note>\n";
               //................. données de réglage ......................................................................
               result   += "<posoAdjust>"      + posoAdjust                              +                  "</posoAdjust>\n";
               result   += "<secabilite>"       "4"                                                         "</secabilite>\n";
               result   += "<numOrdre>"        + QString::number(i+1)                    +                  "</numOrdre>\n";
               result   += "<posoTextuelle>"   + CPosologie::StrtoXml( comment )         +                  "</posoTextuelle>\n";
               result   += "<ald>"             + QString::number(ald=="ALD"?1:0)         +                  "</ald>\n";
               result   += "<posoType>"        + QString::number(posoType)               +                  "</posoType>\n";
               result   += "<duree>"           + QString::number(Medica_dureeTotaleRenouvToSecond(cycle)) + "</duree>\n";
               result   += "<IdTable>"         + QString((idType=="CIP")?"1":"2")        +                  "</IdTable>\n";
               result   += "</Posologie>\n";
               stringDST.append (result);                    // ajouter la structure calculable non modifiée
     }
 //CGestIni::Param_UpdateToDisk("/home/ro/poso.txt", stringDST + "</PosologieList>\n");
 return stringDST + "</PosologieList>\n";
}
//---------------------------- textTo_UP_Code ------------------------------------------------
QString  CMedicaBase::textTo_UP_Code( QString text )
{ if (m_MedicaBase==0)                                                return "";
  if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)   return "";
  QString      result   = "";
  QString      requete  = "";
  text                  = text.lower();
  if (text.endsWith("s")) text = text.left(text.length()-1);   // virer le s de fin  pas de chop en QT3
  text.replace("-"," ");                                       // si nom compose separe par un tiret le remplacer par un espace
  //.............. si nom compose on ne garde que la premiere partie du nom ......................
  //               eliminer les truc genre 'comprime-effervescent' 'comprime effervescent' etc.
  int pos = text.find(" ");
  if (pos!=-1) text = text.left(pos);
  //................. recherches et essais divers...................................................
  requete  =  QString(" SELECT ") +           m_MEDICA_FORME_PK    +
                      " FROM "    + m_MEDICA_FORME_AD_TBL_NAME     + " WHERE " + m_MEDICA_FORME_LIBELLE + " LIKE '" + text + "'";
  QSqlQuery query (requete , m_MedicaBase );
  if (query.isActive() && query.next()) result = Utf8_Query(query,0);
  if (result.length())  return result;
  requete  =  QString(" SELECT ") +           m_MEDICA_FORME_PK    +
                      " FROM "    + m_MEDICA_FORME_AD_TBL_NAME     + " WHERE " + m_MEDICA_FORME_LIBELLE + " LIKE '" + text + "%'";
  query.exec ( requete );
  if (query.isActive() && query.next()) result = Utf8_Query(query,0);
  if (result.length())  return result;
  requete  =  QString(" SELECT ") +           m_MEDICA_FORME_PK    +
                      " FROM "    + m_MEDICA_FORME_AD_TBL_NAME     + " WHERE " + m_MEDICA_FORME_LIBELLE + " LIKE '%" + text + "%'";
  query.exec ( requete );
  if (query.isActive() && query.next()) result = Utf8_Query(query,0);
  if (result.length())  return result;

  return result;
}
//---------------------------- Medica_dureeTotaleRenouvToSecond ------------------------------------------------
// <gph_cy>S3R6</gph_cy>     // JSMA     exples J6 --> pour 6 jours S7 --> pour 7 semaines M3R6  pour 3 mois renouvelable 6 fois
int CMedicaBase::Medica_dureeTotaleRenouvToSecond(QString cycle)
{   if (cycle.length()==0) return 0;
    QString duree        = "";
    int            ret   = 0;
    int            pos   = cycle.find('R');
    if (pos != -1)
       { duree  = cycle.left(pos);
         cycle  = cycle.mid(pos+1);
       }
    else
      { duree = cycle;
        cycle = "";
      }
    if (duree.length() && duree.mid(1).toInt())   // si duree nulle ne pas mettre la mention
      { ret  +=  Medica_dureeTotaleToSecond(duree);
      }
    if (pos != -1)
       { int n = cycle.toInt();
         if (n) ret  = ret * n;
       }
    return ret;
}
//---------------------------- Medica_dureeTotaleToSecond ------------------------------------------------
//  J2
int CMedicaBase::Medica_dureeTotaleToSecond(const QString &seq_days)
{if (seq_days.length()==0) return 0;
 int               t = seq_days[0]|32;
 int         nb      = seq_days.mid(1).toInt();

 switch(t)
    { case 'j': return    86400      *   nb;
      case 's': return    86400 * 7  *   nb;
      case 'm': return    86400 * 31 *   nb;
    }
 return 0;
}

//---------------------------- Medica_sequenceToOldMinMaxPoso ------------------------------------------------
// <gpp>[1;m|2;d|4;s]J8!à la phase initiale en bolus</gpp>
// <gpp>[100-200;]U1!à la phase initiale en bolus</gpp>   // 100 à 200 mg à ne faire q'une fois à la phase initiale en bolus
QString CMedicaBase::Medica_sequenceToOldMinMaxPoso(const QString &sequence, QString &q_min,  QString &q_max,  QString &nb_prises)
{q_min     = "";
 q_max     = "";
 int pos             = sequence.find(']');
 if (pos==-1) return QString("");
 int pcom            = sequence.find('!');
 int len             = -1;    // si pas de commentaire le mid prend tout
 QString commentaire = "";
 if (pcom != -1)
    { commentaire = sequence.mid(pcom+1);
      len = pcom-pos-1;   // -1 pour rester avant ]
    }
 QString seq_time                  = sequence.left(pos).remove("[").stripWhiteSpace();
 return  Medica_sequence_timeToOldString(seq_time,  q_min,  q_max , nb_prises);
}

//---------------------------- Medica_sequence_timeToOldString ------------------------------------------------
// 0.5;m|1;d15r|2;sr30|3;h10-13  -->    <schemaPrise>#rDJ;#rMI;#rSO</schemaPrise>    <schemaPrise>MA;MI;SO</schemaPrise>
QString CMedicaBase::Medica_sequence_timeToOldString(const QString &seq_time,  QString &q_min,  QString &q_max ,  QString &nb_prises)
{QStringList seqTList = QStringList::split("|",seq_time, true);    //  bool allowEmptyEntries
 QString  schemaPrise = "";
 QString  pointPrise  = "";
 int      nb_P        = 0;
 QString          str = "";
 double         f_min = 1234567.0;
 double         f_max = 0;
 QString before       = "";
 QString after        = "";
 int     before_n     = 0;
 int     after_n      = 0;
 int              pos = 0;
 int               p  = 0;
 int                l = 0;
 nb_prises             = "";
 for (int i=0; i< (int)seqTList.count(); ++i)   // l'ancien systeme ne connait qu'une sequence
    {str        = seqTList[i];
     pos        = str.find(';');
     pointPrise = "";
     if (pos   != -1)
        {//.......... CMedicaBase::UP_QUANTITE la quantite d'unite de prise .....................
         //           ici est traite le mode literal ou numerique
         q_max     = str.left   (pos);
         p         = q_max.find ('-');
         if (p != -1)                // cas ou 1-2   (un à deux)
            { q_min  = q_max.left ( p );
              q_max  = q_max.mid  ( p + 1);   
            }
         //.......... on releve le max et min trouve ..........................
         f_min  = qMin( qMin(q_min.toDouble(), q_max.toDouble()),f_min);
         f_max  = qMax( qMax(q_min.toDouble(), q_max.toDouble() +f_max),f_max);

         //........... CMedicaBase::SEQUENCE_HOUR  le moment ou prendre les medocs dans la journee .......................
         //            si specifie en heures, le mode literal
         //            ou numerique sera traite ici
         str    = str.mid(pos+1);   // prendre ce qu'il y a apres le point virgule
         l      = str.length();
         if (l==0)             // mention l apres le point virgule
            {pointPrise   += ""; //QObject::tr(" par jour");          // sera remplacé par l'unite de temps de la periode (jour semaine mois etc...)
            }
         else   // // -CO (avant)   .IA (au debut)   #MA (au cours)  #r (au cours d'un repas) +r (avant un repas) -rDJ  -r||#rDJ;-r||#rSO    .rDJ  #rDJ   +rDJ
            { char t  = str.latin1()[0]|32;    // relever le m d a s c n h
              str     = str.mid(1);
              switch (t)
                 { case 'm': pointPrise = "MA";     break;
                   case 'd': pointPrise = "MI";     break;
                   case 'a': pointPrise = "AM";     break;
                   case 's': pointPrise = "SO";     break;
                   case 'c': pointPrise = "CO";     break;
                   case 'n': pointPrise = "CO";     break;
                   case 'h': pointPrise = "";       break;
                  }
              ++nb_P;
              //............ CMedicaBase::MOMENT_MEAL le moment par rapport au repas .........................
              //             en minutes, le mode literal numerique sera traite ici
              pos  = str.find('r');
              if  (pos != -1)
                  { before   = str.left(pos);
                    after    = str.mid(pos+1);
                    before_n = before.toInt();
                    after_n  = after.toInt();
                    if ( after=="0" && before=="0")
                       {pointPrise =  "#r" + pointPrise; // ret +=  QObject::tr(" au cours du repas");
                       }
                    else if ( before=="0" )
                       {pointPrise =  "-r" + pointPrise; //ret +=  QObject::tr(" avant le repas");
                       }
                    else if (after=="0")
                       {pointPrise =  "+r" + pointPrise; // ret +=  QObject::tr(" après le repas");
                       }
                    else if (before_n==0 && after_n==0)
                       {pointPrise =  "#r" + pointPrise; //ret +=  QObject::tr(" au cours du repas");
                       }
                    else //if (before_n||after_n)
                       {
                        if      (before.length()) pointPrise =  "-r" + pointPrise; // ret += " " + before + QObject::tr(" minutes avant le repas");
                        else if (after.length() ) pointPrise =  "+r" + pointPrise; // ret += " " + after  + QObject::tr(" minutes après le repas");
                        else                      pointPrise =  "#r" + pointPrise; // ret += " " + QObject::tr("au cours du repas");
                       }
                  }
            } // if (str.length())
          schemaPrise += pointPrise + ";" ;
        } // endif (pos   != -1)
    }  // end for (int i=0; i<seqTList.count(); ++i)
 //......... preparer les sorties ...............
 if (f_min>0)          q_min  = QString::number(f_min,'f',2);
 else                  q_min  = "";
 if (f_max>0)          q_max  = QString::number(f_max,'f',2);
 else                  q_max  = "";

 nb_prises  = QString::number(nb_P);

 if (schemaPrise.endsWith(";")) schemaPrise = schemaPrise.left(schemaPrise.length()-1);  // virer le dernier ';'
 return schemaPrise;
}

//-------------------------------------- Medica_PosologieToHtmlOrdo --------------------------------------------------------------------------------------------
QString  CMedicaBase::Medica_PosologieToHtmlOrdo(QPL_CPosologie list_CPosologie       ,
                                                          const QString &spoids        ,
                                                          const QString &staille       ,
                                                          const  int     mode          // = CMedicaBase::InteractiveForm avec les pictogrammes de modification
                                                )
{

 /*
 int                   nb = 0;
 Type_Posologie        poso;
 int                   ald = 0;
 int               id_list = 0;
 QStringList::iterator it;
 value               = "<html><head><meta name=\"qrichtext\" content=\"1\" />"
                       "<title>Prescription</title>"
                       "</head>"
                       "<body style=\"font-size:10pt;font-family:Arial Narrow\">";
 //....................... definir la ligne horizontale ..............................................
 QString h_line;
 if (mode&TYP_DECOD_WITH_MODIF_ZONE)  h_line              ="<center><hr size=\"2\" />";
 else                                 h_line              ="<center>---------------------------------------------"
                                                           "-----------------------------------------------------"
                                                           "-----------------------------------------------------"
                                                           "-----------------------------------------------------<br />";
 //................ rechercher si cette ordo contient une zone ALD ..................................
 for (it = ordo_List.begin(); it != ordo_List.end(); ++it )
     {VidalStringPosoToStructPoso((*it), poso );
      if (poso.Ald==1) {ald = 1; break;}
     }
 //................ si cette ordo contient une zone ALD .............................................
 if (ald)
    {//value              +="<p align=\"center\">";
     value              +="<span style=\"font-family:Arial Narrow;font-size:8pt;color:#000000\">";
     value              += h_line ;
     value              +=TR("Prescriptions relatives au traitement de l'affection de longue durée reconnue (liste ou hors liste)<br />(AFFECTION EXONERANTE)");
     value              += h_line + "</center>";
     value              +="</span>";
     //value              +="</p >";
    }
 nb = 0;
 for (it = ordo_List.begin(); it != ordo_List.end(); ++it )
     {VidalStringPosoToStructPoso((*it), poso );
      if (poso.Ald==1)
         {++ nb;
          VidalDecodePosologieEx(poso, value, id_list, nb, mode);
         }
      ++id_list;
     }

 if (ald)
    {//value              +="<p align=\"center\">";
     value              +="<span style=\"font-family:Arial Narrow;font-size:8pt;color:#000000\">";
     value              += h_line;
     value              +=TR("Prescriptions SANS RAPPORT avec l'affection de longue durée<br />(MALADIES INTERCURRENTES)");
     value              += h_line + "</center>";
     value              +="</span>";
     //value              +="</p >";
    }
 id_list = 0;
 for (it = ordo_List.begin(); it != ordo_List.end(); ++it )
     {VidalStringPosoToStructPoso((*it), poso );
      if (poso.Ald==0)
         {++ nb;
          VidalDecodePosologieEx(poso, value, id_list, nb, mode);
         }
      ++id_list;
     }
 */
 QPL_CPosologie::iterator it;
 QString            result = "";
 int               id_list;
 int                    nb;
 int                  id_l = -1;   // à priori on n'affiche pas le pictogrammes de modification
 QString            h_lineTop    = "<p><hr size=\"2\" />";
 QString            h_linebottom = "<p><hr size=\"2\" /><p>";

 if (list_CPosologie.count() == 0)    return result;
 result               = "<html><head><meta name=\"qrichtext\" content=\"1\" />"
                       "<title>Prescription</title>"
                       "</head>"
                       "<body style=\"font-size:10pt;font-family:Arial Narrow\">";
 //....................... definir la ligne horizontale ..............................................

 //.................. verifier si i y a des medicaments en ALD  .................................................
 int ald = 0;
 for (it = list_CPosologie.begin(); it !=  list_CPosologie.end(); ++it )   // it != m_RecSpeclist.end();
     {if ( (*it).m_ALD != 0) ald++;
     }
 //................ si cette ordo contient une zone ALD .............................................
 if (ald)
    {//result              +="<p align=\"center\">";
     result              +="<center><span style=\"font-family:Arial Narrow;font-size:10pt;color:#000000\">";
     result              += h_lineTop ;
     result              += TR("Prescriptions relatives au traitement de l'affection de longue durée reconnue (liste ou hors liste)"
                                       "<br>(AFFECTION EXONÉRANTE)");
     result              += h_linebottom ;
     result              +="</span></center>";
     //result              +="</p >";
    }
 nb      = 0;
 id_list = 0;
 for (it = list_CPosologie.begin(); it !=  list_CPosologie.end(); ++it )
     {if ((*it).m_ALD==1)
         {(*it).m_NumOrdre = ++ nb;                                 // pour affichage du numéro d'ordre de medicament dans l'ordo
          if (mode & CMedicaBase::InteractiveForm) id_l = id_list;  // si il le faut, afficher les pictogrammes de modification
          result += "<br>"  + Medica_PosologieToPrescriptionHtml(*it, spoids, staille , id_l) + "<br>";
         }
      ++id_list;                        // pour indice du medicament dans la liste (ne sert que pour les ancres des pictogrammes)
     }

 if (ald)
    {//result              +="<p align=\"center\">";
     result              +="<center><span style=\"font-family:Arial Narrow;font-size:10pt;color:#000000\">";
     result              += h_lineTop;
     result              += TR("Prescriptions SANS RAPPORT avec l'affection de longue durée"
                                       "<br>(MALADIES INTERCURRENTES)");
     result              += h_linebottom;
     result              +="</span></center>";
     //result              +="</p >";
    }
 id_list = 0;
 for (it = list_CPosologie.begin(); it !=  list_CPosologie.end(); ++it )
     {if ((*it).m_ALD==0)
         {(*it).m_NumOrdre = ++ nb;                                 // pour affichage du numéro d'ordre de medicament dans l'ordo
          if (mode & CMedicaBase::InteractiveForm) id_l = id_list;  // si il le faut, afficher les pictogrammes de modification
          result +=  "<br>"   + Medica_PosologieToPrescriptionHtml(*it, spoids, staille , id_l)+ "<br>";
         }
      ++id_list;                        // pour indice du medicament dans la liste  (ne sert que pour les ancres des pictogrammes)
     }
 result += "</body></html>";
 return result;
}

//-------------------------------------- Medica_PosologieToPrescriptionHtml --------------------------------------------------------------------------------------------
/*
    //......................... données structurelles .............................................
    //                          provenant de la base médicamenteuse
    //......................... données structurelles .............................................
    //                          provenant de la base médicamenteuse
    QString       m_MEDICA_POSO_CIP;
    QString       m_MEDICA_POSO_PK;
    QString       m_MEDICA_POSO_LIBELLE;
    QString       m_MEDICA_POSO_AGE_MIN;
    QString       m_MEDICA_POSO_AGE_MAX;
    QString       m_MEDICA_POSO_SEXE;
    QString       m_MEDICA_POSO_TERRAIN;
    QString       m_MEDICA_POSO_POIDS_MIN;
    QString       m_MEDICA_POSO_POIDS_MAX;
    QString       m_MEDICA_POSO_DOSE_MIN;
    QString       m_MEDICA_POSO_DOSE_MAX;
    QString       m_MEDICA_POSO_DOSE_UNIT;
    QString       m_MEDICA_POSO_DOSE_LIMIT_MAX;
    QString       m_MEDICA_POSO_FACT_CORP_QU;
    QString       m_MEDICA_POSO_FACT_CORP_UNIT;
    QString       m_MEDICA_POSO_PERIODE;
    QString       m_MEDICA_POSO_EQUI_COEFF;
    QString       m_MEDICA_POSO_UNIT_COEFF;
    QString       m_MEDICA_POSO_NB_PRISES;           // nombre de prises conseillées
    QString       m_MEDICA_POSO_REPART_PRISES;       // description structurée de la répartition des prises
    QString       m_MEDICA_POSO_DIVERS;              // un peu ce que l'on veut
    QString       m_MEDICA_POSO_PENDANT;             // duree traitement conseillée
    QString       m_MEDICA_POSO_SECABILITE;          // secabilite
    QString       m_MEDICA_POSO_EXTRA_PK;            // si present = Pk de la table de poso personnelle d'ou provient la donnée
    QString       m_Note;                            // commentaire concernant cette posologie
    //......................... données fonctionnelles .............................................
    //                          non dépendantes de la base médicamenteuse mais des
    //                          ajustements de la prescription
    int           m_PosoAdjust;                     // ajustement par rapport à fourchette de doses initiales conseillées de 1 à 100 %
    int           m_NumOrdre;                       // numero d'ordre du produit dans l'ordo ou la liste (utile pour affichage)
    QString       m_PosoTextuelle;                  // posologie manuelle ajoutée par l'utilisateur (complément textuel)
    int           m_ALD;                            // à 0 si pas en rapport avec longue maladie
    int           m_PosoType;                       // CPosologie::Nothing  CPosologie::Textuelle  CPosologie::Structuree  CPosologie::AllMode
    int           m_IdTable;                        // indice de la table d'index
*/
QString  CMedicaBase::Medica_PosologieToPrescriptionHtml( CPosologie    &poso          ,
                                                          const QString &spoids        ,
                                                          const QString &staille       ,
                                                                    int id_list        ,   // = -1  utile que pour donner aux ancres l'indice de liste
                                                                QString prodName       ,   // = ""  si présent évitera la recherche couteuse
                                                                QString factCorpUnit       // = ""  si présent évitera la recherche couteuse

                                                        )
{QString upUnit("");
 float fDose(0);
 float poids(  spoids.toFloat()  );
 float taille( staille.toFloat() );
 float fMin(0), fMax(0), coeff(0);
 QString style  = "";
 QString result = "";

 if (CGestIni::Param_ReadParam(  m_IniParam, "Span Style",  "defaut", &style) ==0) result = style.remove("\\") + "<b>";  // zero = pas d'erreur
 else                                                                              result = "<span style=\"font-family:Arial;color:#0000ff\"><b>";


 //................. si prodName et factCorpUnit non donnés, les extraire ................................................
 // dans le cas d'un produit non medicamenteux l'indice de l'index de table stockée dans:  poso.m_IdTable
 if (poso.m_IdTable>1)     prodName = Medica_GetMedicamentNameByCIP(poso.m_MEDICA_POSO_CIP, poso.m_IdTable);
 else if (prodName=="")    prodName = Medica_GetMedicamentNameByCIP(poso.m_MEDICA_POSO_CIP);
 if (factCorpUnit=="") factCorpUnit = Medica_GetUnite(m_MEDICA_FORME_AD_TBL_NAME, poso.m_MEDICA_POSO_FACT_CORP_UNIT);

 //........................... SI coefficient de conversion alors convertir ..............................................
 //                            pour avoir la forme usuelle et utile (comprimés gouttes  cuillères mesures etc )
 if (poso.m_MEDICA_POSO_EQUI_COEFF != "0" && poso.m_MEDICA_POSO_EQUI_COEFF.length() != 0)
    { coeff  = 1/poso.m_MEDICA_POSO_EQUI_COEFF.toFloat();
      upUnit =  Medica_GetUnite(m_MEDICA_FORME_UP_TBL_NAME, poso.m_MEDICA_POSO_UNIT_COEFF);
    }
 else
    { coeff  = 1;
      upUnit =  Medica_GetUnite(m_MEDICA_FORME_AD_TBL_NAME, poso.m_MEDICA_POSO_DOSE_UNIT);
    }
 //..................... multiplier par le coefficient par le poids ou la surface ............................
 if (poso.m_MEDICA_POSO_FACT_CORP_QU != "0" && poso.m_MEDICA_POSO_FACT_CORP_QU.length() != 0)
    {if      (factCorpUnit=="Kg" && poids>0)
             {coeff *= poids / poso.m_MEDICA_POSO_FACT_CORP_QU.toFloat();
             }
     else if (factCorpUnit=="m2" && taille>0 && poids >0)
             {coeff *= sqrt(taille * poids / 3600) / poso.m_MEDICA_POSO_FACT_CORP_QU.toFloat();  //(Mosteller)
             }
    }

 //..................... calculer les valeurs finales Min et Max ............................................
 fMin = poso.m_MEDICA_POSO_DOSE_MIN.toFloat()        * coeff;
 if (poso.m_MEDICA_POSO_DOSE_MAX != "0")
    {fMax   =  poso.m_MEDICA_POSO_DOSE_MAX.toFloat() * coeff;
     fDose  =  fMin + ((fMax - fMin) * (poso.m_PosoAdjust-1) /99 );     // calculer les valeurs finales selon slider
    }
 else
    {fDose  =  fMin;
    }

 //............................. num ordre et nom du produit ..................................................

 if (id_list != -1)
    {                                            result += "<a href=\"#ModifD"+QString::number(id_list)+"\"><img source=\"./TimePicto14-13.png\"></a>&nbsp;&nbsp;";
     if (poso.m_PosoType&CPosologie::Structuree) result += "<a href=\"#ModifX"+QString::number(id_list)+"\"><img source=\"./PosoAdjustOn.png\"></a>&nbsp;&nbsp;";
     else                                        result += "<a href=\"#ModifY"+QString::number(id_list)+"\"><img source=\"./PosoAdjustOff.png\"></a>&nbsp;&nbsp;";
     if (poso.m_PosoType&CPosologie::Textuelle)  result += "<a href=\"#ModifQ"+QString::number(id_list)+"\"><img source=\"./PosoManuelleOn.png\"></a>&nbsp;&nbsp;";
     else                                        result += "<a href=\"#ModifT"+QString::number(id_list)+"\"><img source=\"./PosoManuelleOff.png\"></a>&nbsp;&nbsp;";
     if (poso.m_ALD == 0)                        result += "<a href=\"#ModifN"+QString::number(id_list)+"\"><img source=\"./AldOffPicto14-13.png\"></a>&nbsp;&nbsp;";
     else                                        result += "<a href=\"#ModifW"+QString::number(id_list)+"\"><img source=\"./AldOnPicto14-13.png\"></a>&nbsp;&nbsp;";
                                                 result += "<a href=\"#ModifS"+QString::number(id_list)+"\"><img source=\"./DeletePicto14-13.png\"></a>&nbsp;&nbsp;";
                                                 result += "<a href=\"#ModifZ"+QString::number(id_list)+"\"><img source=\"./Interrogation.png\"></a>&nbsp;&nbsp;";

     result += QString::number(poso.m_NumOrdre) + ") " + "<a href=\"#OrdoMed"+QString::number(id_list) + "==" + QString::number(poso.m_NumOrdre) + "\">";
     result += prodName;
     result += "</a>";
    }
 else
    {result += QString::number(poso.m_NumOrdre)   + ")&nbsp;<u>" + prodName + "</u></b>&nbsp;&nbsp;&nbsp;<i>";
    }
 QString poso_structuree_str = "";
 //........................... si poso calculée ......................................................................
 if (poso.m_PosoType & CPosologie::Structuree )
    {poso_structuree_str += "<br>";
     //...................................... MEDICAMENTS ............................................................
     if ( poso.m_IdTable <= 1 )
        {
         //............... SI NEGATIF alors on exprime de: min à max  comprimés  ..........................................
         if (poso.m_PosoAdjust < 0 )
            {   //..................... calculer les valeurs finales selon repartition de prise ............................
                if (poso.m_MEDICA_POSO_NB_PRISES.toFloat()>0)
                   {fMax  /=  poso.m_MEDICA_POSO_NB_PRISES.toFloat();
                    fMin  /=  poso.m_MEDICA_POSO_NB_PRISES.toFloat();
                   }
                //....................... arrondir selon secabilite ........................................................
                fMin    = Medica_ArondirUniteDePrise(fMin, poso.m_MEDICA_POSO_SECABILITE.toInt() );
                fMax    = Medica_ArondirUniteDePrise(fMax, poso.m_MEDICA_POSO_SECABILITE.toInt() );
                //........................ afficher Qu unite de prise ......................................................
                if (fMin != fMax && fMax !=0 )
                   {poso_structuree_str += "&nbsp;"  + Medica_PosologieUP_FloatValueToStrValue(fMin) +  TR(" à ") +
                                          Medica_PosologieUP_FloatValueToStrValue(fMax) + "&nbsp;" + Medica_AdjustPluriel(upUnit , fMax);
                   }
                else
                   {poso_structuree_str += "&nbsp;"  + Medica_PosologieUP_FloatValueToStrValue(fMax) + "&nbsp;" + Medica_AdjustPluriel(upUnit , fMin);
                   }
            }
          //............. SI POSITIF alors on calcule la valeur positionnée par le curseur:  entre min et max ..............
          else
            {   //..................... calculer les valeurs finales selon repartition de prise ...........................
                if (poso.m_MEDICA_POSO_NB_PRISES.toFloat()>0)
                   {fDose /=  poso.m_MEDICA_POSO_NB_PRISES.toFloat();
                   }
                //....................... arrondir selon secabilite ........................................................
                fDose   = Medica_ArondirUniteDePrise(fDose, poso.m_MEDICA_POSO_SECABILITE.toInt() );
                //........................ afficher Qu unite de prise ......................................................
                poso_structuree_str += Medica_PosologieUP_FloatValueToStrValue(fDose);       //
                poso_structuree_str += "&nbsp;"  + Medica_AdjustPluriel(upUnit , fDose);
            }
         //............................. schéma de prise ..............................................................
         poso_structuree_str += "&nbsp;"  + Medica_PosologieSpecPrisesToHtml(poso);

         //.............. si periode superieure au jour (exple toutes les semaines ) ..................................
         if (poso.m_MEDICA_POSO_REPART_PRISES.length()==0)
            {float par = poso.m_MEDICA_POSO_PERIODE.toFloat();
             if ( par == 86400)         poso_structuree_str +=  TR(" par jour");
             else if ( par == 604800)   poso_structuree_str +=  TR(" par semaine");
             else                       poso_structuree_str +=  Medica_PosologiePeriodeToHtml(poso.m_MEDICA_POSO_PERIODE);
            }
         else if (poso.m_MEDICA_POSO_PERIODE.toFloat() > 86400)
            {poso_structuree_str += Medica_PosologiePeriodeToHtml(poso.m_MEDICA_POSO_PERIODE);
            }

         //.............. si duree traitement specifiée ...............................................................
         if (poso.m_MEDICA_POSO_PENDANT.toFloat()>=86400)
            {poso_structuree_str += TR("&nbsp;pendant&nbsp;") + Medica_PosologieDureeToHtml(poso.m_MEDICA_POSO_PENDANT);
            }
        }
     else
     //...................................... ACCESSOIRES ...................................................
        {if (poso.m_MEDICA_POSO_PENDANT.toFloat() <= 0)
            {poso_structuree_str += TR("Bon pour achat de ")    + QString::number((long)fDose) + TR(" article");
             if ((long)fDose>1) poso_structuree_str += "s";
            }
         else
            {poso_structuree_str += TR("Bon pour location de ") + QString::number((long)fDose) + TR(" article");
             if ((long)fDose>1) poso_structuree_str += "s";
             poso_structuree_str += TR("&nbsp;pendant&nbsp;") + Medica_PosologieDureeToHtml(m_MEDICA_POSO_PENDANT);
            }
        }
    }
 QString poso_textuelle = "";
 //.............. si posologie manuelle .................................................................
 if (poso.m_PosoType & CPosologie::Textuelle)
    {if (id_list != -1)
        {poso_textuelle += "<br><a href=\"#OrdoMan"+QString::number(id_list) + "==" + QString::number(poso.m_NumOrdre) + "\">";
         if (poso.m_PosoTextuelle=="") poso_textuelle += TR("Cliquez ici pour saisir un texte de votre choix");
         else                          poso_textuelle += poso.m_PosoTextuelle;
         poso_textuelle += "</a>";
        }
     else
         {poso_textuelle += "<br>" + poso.m_PosoTextuelle;
         }
    }
 result += poso_structuree_str + poso_textuelle + "</i></span>";
 return result;
}

//-------------------------------------- Medica_PosologieToSimpleText --------------------------------------------------------------------------------------------
QString  CMedicaBase::Medica_PosologieToSimpleText( CPosologie          &poso          ,
                                                          const QString &spoids        ,
                                                          const QString &staille       ,
                                                                QString prodName       ,   // = ""  si présent évitera la recherche couteuse
                                                                QString factCorpUnit       // = ""  si présent évitera la recherche couteuse
                                                  )
{QString upUnit("");
 float fDose(0);
 float poids(  spoids.toFloat()  );
 float taille( staille.toFloat() );
 float fMin(0), fMax(0), coeff(0);

 QString result = ""; 
 //................. si prodName et factCorpUnit non donnés, les extraire ................................................
 // dans le cas d'un produit non medicamenteux l'indice de l'index de table stockée dans:  poso.m_IdTable
 if (prodName.length()==0)   // si le nom du produit est deja renseigne ( conversion LAP --> anciennes ordos), ne pas le chercher a l'extraire de la base
    { if (poso.m_IdTable>1)     prodName     = Medica_GetMedicamentNameByCIP(poso.m_MEDICA_POSO_CIP, poso.m_IdTable);
      else if (prodName=="")    prodName     = Medica_GetMedicamentNameByCIP(poso.m_MEDICA_POSO_CIP);
    }
 if ( factCorpUnit=="" )        factCorpUnit = Medica_GetUnite(m_MEDICA_FORME_AD_TBL_NAME, poso.m_MEDICA_POSO_FACT_CORP_UNIT);

 //........................... SI coefficient de conversion alors convertir ..............................................
 //                            pour avoir la forme usuelle et utile (comprimés gouttes  cuillères mesures etc )
 if (poso.m_MEDICA_POSO_EQUI_COEFF != "0" && poso.m_MEDICA_POSO_EQUI_COEFF.length() != 0)
    { coeff  = 1/poso.m_MEDICA_POSO_EQUI_COEFF.toFloat();
      upUnit =  Medica_GetUnite(m_MEDICA_FORME_UP_TBL_NAME, poso.m_MEDICA_POSO_UNIT_COEFF);
    }
 else
    { coeff  = 1;
      upUnit =  Medica_GetUnite(m_MEDICA_FORME_AD_TBL_NAME, poso.m_MEDICA_POSO_DOSE_UNIT);
    }
 //..................... multiplier par le coefficient par le poids ou la surface ............................
 if (poso.m_MEDICA_POSO_FACT_CORP_QU != "0" && poso.m_MEDICA_POSO_FACT_CORP_QU.length() != 0)
    {if      (factCorpUnit=="Kg" && poids>0)
             {coeff *= poids / poso.m_MEDICA_POSO_FACT_CORP_QU.toFloat();
             }
     else if (factCorpUnit=="m2" && taille>0 && poids >0)
             {coeff *= sqrt(taille * poids / 3600) / poso.m_MEDICA_POSO_FACT_CORP_QU.toFloat();  //(Mosteller)
             }
    }

 //..................... calculer les valeurs finales Min et Max ............................................
 fMin = poso.m_MEDICA_POSO_DOSE_MIN.toFloat()        * coeff;
 if (poso.m_MEDICA_POSO_DOSE_MAX != "0")
    {fMax   =  poso.m_MEDICA_POSO_DOSE_MAX.toFloat() * coeff;
     fDose  =  fMin + ((fMax - fMin) * (poso.m_PosoAdjust-1) /99 );     // calculer les valeurs finales selon slider
    }
 else
    {fDose  =  fMin;
    }

 //............................. num ordre et nom du produit ..................................................
 result += "<br>&nbsp;&nbsp;&nbsp;<b>"+QString::number(poso.m_NumOrdre)   + ")&nbsp;" + prodName + "</b></b>&nbsp;&nbsp;&nbsp;<i>";
 //........................... si poso calculée ......................................................................
 if (poso.m_PosoType & CPosologie::Structuree  )
    {//...................................... MEDICAMENTS ............................................................
     if ( poso.m_IdTable <= 1 )
        {
         //............... SI NEGATIF alors on exprime de: min à max  comprimés  ..........................................
         if (poso.m_PosoAdjust < 0 )
            {   //..................... calculer les valeurs finales selon repartition de prise ............................
                if (poso.m_MEDICA_POSO_NB_PRISES.toFloat()>0)
                   {fMax  /=  poso.m_MEDICA_POSO_NB_PRISES.toFloat();
                    fMin  /=  poso.m_MEDICA_POSO_NB_PRISES.toFloat();
                   }
                //....................... arrondir selon secabilite ........................................................
                fMin    = Medica_ArondirUniteDePrise(fMin, poso.m_MEDICA_POSO_SECABILITE.toInt() );
                fMax    = Medica_ArondirUniteDePrise(fMax, poso.m_MEDICA_POSO_SECABILITE.toInt() );
                //........................ afficher Qu unite de prise ......................................................
                if (fMin != fMax && fMax !=0 )
                   {result += "&nbsp;"  + Medica_PosologieUP_FloatValueToStrValue(fMin) +  TR(" à ") +
                                          Medica_PosologieUP_FloatValueToStrValue(fMax) + "&nbsp;" + Medica_AdjustPluriel(upUnit , fMax);
                   }
                else
                   {result += "&nbsp;"  + Medica_PosologieUP_FloatValueToStrValue(fMax) + "&nbsp;" + Medica_AdjustPluriel(upUnit , fMin);
                   }
            }
          //............. SI POSITIF alors on calcule la valeur positionnée par le curseur:  entre min et max ..............
          else
            {   //..................... calculer les valeurs finales selon repartition de prise ...........................
                if (poso.m_MEDICA_POSO_NB_PRISES.toFloat()>0)
                   {fDose /=  poso.m_MEDICA_POSO_NB_PRISES.toFloat();
                   }
                //....................... arrondir selon secabilite ........................................................
                fDose   = Medica_ArondirUniteDePrise(fDose, poso.m_MEDICA_POSO_SECABILITE.toInt() );
                //........................ afficher Qu unite de prise ......................................................
                result += Medica_PosologieUP_FloatValueToStrValue(fDose);       //
                result += "&nbsp;"  + Medica_AdjustPluriel(upUnit , fDose);
            }
         //............................. schéma de prise ..............................................................
         result += "&nbsp;"  + Medica_PosologieSpecPrisesToHtml(poso);
        }
     else if (poso.m_Note.length())
        {
        }
     else
     //...................................... ACCESSOIRES ...................................................
     {if (fDose<=0) fDose = 1;
         if (poso.m_MEDICA_POSO_PENDANT.toFloat() == 0)
            {result += TR("Bon pour achat de ")    + QString::number(fDose) + TR(" article");
             if (fDose>1) result += "s";
            }
         else
            {result += TR("Bon pour location de ") + QString::number(fDose) + TR(" article");
             if (fDose>1) result += "s";
             result += TR("&nbsp;pendant&nbsp;") + Medica_PosologieDureeToHtml(m_MEDICA_POSO_PENDANT);
            }
        }
    }
 //.............. si posologie manuelle .................................................................
 if (poso.m_PosoType & CPosologie::Textuelle)
    {result +=  QString(" ")+poso.m_PosoTextuelle;
    }
 result.prepend("<span style=\"font-family:Arial Narrow;color:#000000\"><b>");
 result += "</i></span>";
 return result;
}

//----------------------------------------------------- Medica_PosologieUP_FloatValueToStrValue ---------------------------------------------------------------------------
QString  CMedicaBase::Medica_PosologieUP_FloatValueToStrValue(float up)
{if      (up==0.5)  return TR("1/2");
 else if (up==0.25) return TR("1/4");
 else if (up==0.75) return TR("3/4");
 else               return QString::number(up);
}

//----------------------------------------------------- Medica_PosologieUP_UnitToHtml ---------------------------------------------------------------------------
QString  CMedicaBase::Medica_PosologieUP_UnitToHtml(const QString &unit ,const QString &dose)
{return Medica_AdjustPluriel(Medica_GetUnite(m_MEDICA_FORME_AD_TBL_NAME, unit) , dose.toFloat());
}

//----------------------------------------------------- Medica_AdjustPluriel ---------------------------------------------------------------------------
QString  CMedicaBase::Medica_AdjustPluriel(const QString &unit , const float &f_dose)
{int pos;
 if (f_dose<=1                                   ||                        // pas pluriel
     unit==TR("UI anti-Xa")                      ||
     unit==TR("UI")                              ||
     unit==TR("U")                               ||
     unit==TR("MUI")                             ||
     unit==TR("mmol de phosphore")               ||
     unit==TR("mmol")                            ||
     unit==TR("ml")                              ||
     unit==TR("million UI")                      ||
     unit==TR("mg de triméthoprime")             ||
     unit==TR("mg de rifampicine")               ||
     unit==TR("mg de fer métal")                 ||
     unit==TR("mg de chlorhydrate de lidocaïne") ||
     unit==TR("mg d'antimoine")                  ||
     unit==TR("mg")                              ||
     unit==TR("mg/h")                            ||
     unit==TR("UI/h")                            ||
     unit==TR("m2")                              ||
     unit==TR("Kg")                              ||
     unit==TR("IR")                              ||
     unit==TR("µg")                              ||
     unit==TR("g")                               ||
     unit==TR("g de triglycérides")              ||
     unit==TR("g d'azote")                       ||
     unit==TR("g") )   return unit;
 if      ( (pos=unit.find(TR("cuillère ")))           != -1)    return unit.left(pos+8)  + "s " + unit.mid(pos+9);
 else if ( (pos=unit.find(TR("comprimé ")))           != -1)    return unit.left(pos+8)  + "s " + unit.mid(pos+9);
 else if ( (pos=unit.find(TR("application ")))        != -1)    return unit.left(pos+11) + "s " + unit.mid(pos+12);
 else if ( (pos=unit.find(TR("capsule vaginale")))    != -1)    return unit.left(pos)    + TR("capsules vaginales") + unit.mid(pos+16);
 else if ( (pos=unit.find(TR("capsule ")))            != -1)    return unit.left(pos+7)  + "s " + unit.mid(pos+8);
 else if ( (pos=unit.find(TR("bain ")))               != -1)    return unit.left(pos+4)  + "s " + unit.mid(pos+5);
 else if ( (pos=unit.find(TR("dispositif ")))         != -1)    return unit.left(pos+10) + "s " + unit.mid(pos+11);
 else if ( (pos=unit.find(TR("gélule ")))             != -1)    return unit.left(pos+5)  + "s " + unit.mid(pos+6);
 else if ( (pos=unit.find(TR("godet ")))              != -1)    return unit.left(pos+6)  + "s " + unit.mid(pos+7);
 else if ( (pos=unit.find(TR("graduation ")))         != -1)    return unit.left(pos+10) + "s " + unit.mid(pos+11);
 else if ( (pos=unit.find(TR("lavage oculaire")))     != -1)    return unit.left(pos)    + TR("lavages oculaires") + unit.mid(pos+15);
 else if ( (pos=unit.find(TR("lavage ")))             != -1)    return unit.left(pos+6)  + "s " + unit.mid(pos+7);
 else if ( (pos=unit.find(TR("instillation ")))       != -1)    return unit.left(pos+12) + "s " + unit.mid(pos+13);
 else if ( (pos=unit.find(TR("mesure graduée")))      != -1)    return unit.left(pos)    + TR("mesures graduées") + unit.mid(pos+14);
 else if ( (pos=unit.find(TR("mesure ")))             != -1)    return unit.left(pos+6)  + "s " + unit.mid(pos+7);
 else if ( (pos=unit.find(TR("injection ")))          != -1)    return unit.left(pos+9)  + "s " + unit.mid(pos+10);
 else if ( (pos=unit.find(TR("pansement ")))          != -1)    return unit.left(pos+9)  + "s " + unit.mid(pos+10);
 else if ( (pos=unit.find(TR("pulvérisation ")))      != -1)    return unit.left(pos+13) + "s " + unit.mid(pos+14);
 else if ( (pos=unit.find(TR("tablette ")))           != -1)    return unit.left(pos+8)  + "s " + unit.mid(pos+9);
 else if ( (pos=unit.find(TR("gobelet ")))            != -1)    return unit.left(pos+7)  + "s " + unit.mid(pos+8);
 else if ( (pos=unit.find(TR("bâton ")))              != -1)    return unit.left(pos+5)  + "s " + unit.mid(pos+6);
 else if ( (pos=unit.find(TR("pastille ")))           != -1)    return unit.left(pos+8)  + "s " + unit.mid(pos+9);
 else if ( (pos=unit.find(TR("sachet-dose ")))        != -1)    return unit.left(pos+11) + "s " + unit.mid(pos+12);
 else if ( (pos=unit.find(TR("dose ")))               != -1)    return unit.left(pos+4)  + "s " + unit.mid(pos+5);
 else if ( (pos=unit.find(TR("tube ")))               != -1)    return unit.left(pos+4)  + "s " + unit.mid(pos+5);
 else if ( (pos=unit.find(TR("récipient ")))          != -1)    return unit.left(pos+9)  + "s " + unit.mid(pos+10);
 else if ( (pos=unit.find(TR("lyophilisat oral")))    != -1)    return unit.left(pos)    + TR("lyophilisats oraux ")  + unit.mid(pos+16);
 else if ( (pos=unit.find(TR("lyophilisat ")))        != -1)    return unit.left(pos+11) + "s " + unit.mid(pos+12);
 else if ( (pos=unit.find(TR("badigeonnage local")))  != -1)    return unit.left(pos)    + TR("badigeonnages locaux") + unit.mid(pos+18);

 return unit+"s";
}

//-------------------------------------- Medica_ArondirUniteDePrise -------------------------------------------------------
float CMedicaBase::Medica_ArondirUniteDePrise(float value, int secabilite ) // nbr de divisions possibles de l'unite de prise

{return Medica_ArondirUniteDePrise(QString::number(value), secabilite);
}

//-------------------------------------- Medica_ArondirUniteDePrise -------------------------------------------------------
float CMedicaBase::Medica_ArondirUniteDePrise(QString value, int secabilite)
{float fVal(0);
 int virgule = value.find('.');
 if (virgule == -1 || secabilite==-1)                return atof(value);    // pas de partie fractionnaire ou pas de secabilite
 int fract = value.mid(virgule+1,1).toInt(); // recuperer premier car apres la virgule
 //......................... analyse ....................................................................
 switch(secabilite)
 {default:
  case 1:
       if (fract<=7)                   fVal =  atof(value.left(virgule));
       else                            fVal =  atof(value.left(virgule))+1;
       break;
  case 2:
      {if      (fract<=3)              fVal =  atof(value.left(virgule));
       else if (fract>=4 && fract <=7) fVal =  atof(value.left(virgule) +".5");
       else                            fVal =  atoi(value.left(virgule))+1;
       break;
      }
  case 4:
      {if      (fract<=1)              fVal =  atof(value.left(virgule));
       else if (fract>=2 && fract <=3) fVal =  atof(value.left(virgule) +".25");
       else if (fract>=4 && fract <=6) fVal =  atof(value.left(virgule) +".5");
       else if (fract>=7 && fract <=8) fVal =  atof(value.left(virgule) +".75");
       else                            fVal =  atoi(value.left(virgule))+1;
       break;
      }
 }//endswitch(secabilite)
 if (fVal == 0)
    {if      (secabilite==1)   fVal = atof(value);   // on decide que lors des secabilités à 1 on retourne tout si partie entiere = 0
     else if (secabilite==2)   fVal = 0.5;
     else if (secabilite==4)   fVal = 0.25;
    }
 return fVal;
}

//----------------------------------------------------- Medica_GetHtmlPosoSpecPage ---------------------------------------------------------------------------
QString CMedicaBase::Medica_GetHtmlPosoSpecPage(QString cip,
                                                QString name,
                                                QPL_CPosologie &list_CPosologie,
                                                int     tableIndex,             // = 1
                                                QString pkt00  ,                // = QString::null
                                                QString pkt4b  ,                // = QString::null
                                                QString codePD ,                // = QString::null
                                                QString codeUV                  // = QString::null
                                               )
{//................................... recuperer les specifications du produit .............................
 QString up_unit, pa_unit, pa_qu, secabilite, is_generic, formStruct, nbPrises, prixBoite, remboursementSS, tableau, labo, atc, produit;
 if ( (int)(Medica_GetBaseMode() & CMedicaBase::Datasemp) )
    {QString code_prod = Datasemp_GetSpecifications( cip, up_unit, pa_unit, pa_qu, secabilite, is_generic, formStruct,
                                                     nbPrises, prixBoite, remboursementSS, tableau, labo, atc,
                                                     pkt00, pkt4b, codePD, codeUV);
     produit           = Datasemp_GetPA07( code_prod);
    }
 else
    {Medica_GetSpecifications( cip, up_unit, pa_unit, pa_qu, secabilite, is_generic, formStruct, nbPrises, prixBoite, remboursementSS, tableau, labo, atc);
     produit         = Medica_GetLibelleATC(atc);
    }
 //........................ composer la page html ..........................................................
 QString page        = "<html><head><meta name=\"qrichtext\" content=\"1\" />"
                       "<title>" + name + "</title>"
                       "</head>"
                       "<body style=\"font-size:10pt;font-family:Arial Narrow\">";

 //................... titre de la page .....................................
 page += "<p align=\"center\"><a name=\"DebutPage\"><span style=\"font-family:Arial;font-size:18pt;color:#AA0000\"><b><i><u>";
 page +=  name;
 page += TR("</b></i></u></span><br /><span style=\"font-family:Arial;font-size:12pt;color:#AA0000\"><b>Présentation : </b>");

 if ( (int)(Medica_GetBaseMode() & CMedicaBase::Datasemp) )
    {  page += formStruct + " <br> <b>Composition : </b>";
       if (nbPrises.toInt()>0)     {page += nbPrises + " " + up_unit.replace("(s)","") + "(s);";}
       if (produit.length()>0)
          {if (pa_qu.toInt()>1)    {page += TR(" composé(e)(s) de : ") + produit  ;
                                    if (prixBoite.length()!=0 && nbPrises.toInt()>0)
                                       {page += " <br> <b> Prix pour 100 " + pa_unit.replace(QString::fromUtf8 ("µ"),"micro ") + " de Principe Actif : </b>"  +
                                        QString::number(prixBoite.toFloat()*100/(pa_qu.toFloat()*nbPrises.toInt())) + " Euro";
                                       }
                                   }
           else                    {page +=  produit;}
           if (is_generic[0]=='G')  page += TR(" <br> <b> Médicament générique</b>");
           if (is_generic[0]=='R')  page += TR(" <br> <b> Médicament référent générique</b>");
          }
    }
  else
    {  page +=  formStruct + " <br> <b>Composition : </b>";
       if (nbPrises.toInt()>0) {page += nbPrises + " " +  up_unit.replace("(s)","") + "(s)";}
       if (pa_qu.toInt()>1)    {page += TR(" composé(e)(s) de ")+ pa_qu   + " "         +  pa_unit.replace(QString::fromUtf8 ("µ"),"micro ")     +
                                                    " de "               + produit ;
                                if (prixBoite.length()!=0 && nbPrises.toInt()>0)
                                   {page += " <br> <b> Prix pour 100 " + pa_unit.replace(QString::fromUtf8 ("µ"),"micro ") + " de Principe Actif : </b>"  +
                                    QString::number(prixBoite.toFloat()*100/(pa_qu.toFloat()*nbPrises.toInt())) + " Euro";
                                   }
                               }

       else                    {page += " " + produit;}
       if (is_generic[0]=='G') page += TR(" <br> <b> Médicament générique</b>");
       if (is_generic[0]=='R') page += TR(" <br> <b> Médicament référent générique</b>");
    }
 page += "</span></a></p>";
 //................... Données economiques ..................................
 page += "<p align=\"left\"> <hr size=\"4\" />";
 page += "<span style=\"font-family:Arial;font-size:10pt;color:#000000\">";
 if (prixBoite.length()!=0)        page += TR("    <b>Prix_Vente TTC : </b>")            +  prixBoite + TR(" Euro");
 if (remboursementSS.length()!=0)  page += TR("    <b>Remboursement SS : </b>")          +  remboursementSS ;
 page += "<br>";
 if (labo.length()!=0)             page += TR("    <b>Laboratoire : </b>")               +  labo ;
 if (tableau.length()!=0)          page += TR("    <b>Liste : </b>")                     +  tableau ;
 if (cip.length()!=0)              page += TR("    <b>Code CIP : </b>")                  +  cip ;
 page += "</span><br /><hr size=\"2\" />";
 //................... Données posologiques ..................................
 if (list_CPosologie.count())
    {QPL_CPosologie::iterator it;
     int i=1;
     for (it = list_CPosologie.begin(); it !=  list_CPosologie.end(); ++it )   // it != m_RecSpeclist.end();
         {Medica_PosologieToHtmlSpec(i,i, *it, page, tableIndex);
          i++;
         }
    }
 else
    {page += "<span style=\"font-family:Arial;font-size:12pt;color:#0004ff\">";
     page += "<br><b>1)<a href=\"#PosoText"+ cip + "==" + QString::number(tableIndex) + TR("\"> Pas de données sur la posologie, les préciser textuellement</b></a></span>");
     if (cip.stripWhiteSpace().length())
        {page += "<span style=\"font-family:Arial;font-size:12pt;color:#0004ff\">";
         page += "<br><b>2)<a href=\"#PosoNew"+ cip + "==" +QString::number(tableIndex)+ TR("\"> Pas de données sur la posologie, en créer de nouvelles</b></a></span>");
        }
    }
 //................... Fin de la page ........................................................
 page += "</p></body></html>";
 return page;
}


//----------------------------------------------------- Medica_PosologieToHtmlSpec ---------------------------------------------------------------------------
void  CMedicaBase::Medica_PosologieToHtmlSpec(const long num, const long id_list, const CPosologie &poso, QString &page, int tableIndex)
{
  //.......................... intitulé de la posologie ....................................
  page += "<span style=\"font-family:Arial;font-size:12pt;color:#0004ff\"><br>";
  //................... icone selon si poso perso ou autre .................................
  if (poso.m_MEDICA_POSO_EXTRA_PK.length() != 0 &&  poso.m_MEDICA_POSO_EXTRA_PK != "0")
     { page += "<a href=\"#PosoNew"+ poso.m_MEDICA_POSO_CIP + "==" + QString::number(tableIndex) + "\"><img source=\"./IsExtraPk.png\"></a>";
     }
  else
     { page += "<a href=\"#PosoNew"+ poso.m_MEDICA_POSO_CIP + "==" + QString::number(tableIndex) + "\"><img source=\"./IsStandardPk.png\"></a>";
     }
  //................... nom du medicament .................................
  page += "&nbsp;&nbsp;<b>" + QString::number(num) + ");";
  page += "<a href=\"#PosoList"+QString::number(id_list) + "==" + QString::number(num) + "\">";
  if (poso.m_MEDICA_POSO_LIBELLE.stripWhiteSpace().length()==0)
       page += TR(" Indiqué pour le terrain suivant ") + "</b>";
  else
       page +=  poso.m_MEDICA_POSO_LIBELLE + "</b>";
  page += "</a>";
  page += "</span>";
  //.......................... terrain de base .............................................
  page += "<span style=\"font-family:Arial;font-size:10pt;color:#004acc\">";
  //...................... sexe ............................................................
  page += TR("<br>Sexe : ");
  if      (poso.m_MEDICA_POSO_SEXE=="F") page += TR(" Féminin ");
  else if (poso.m_MEDICA_POSO_SEXE=="M") page += TR(" Masculin ");
  else    page += TR(" Indifférent ");
  //...................... age .............................................................
  page += TR("<br>Âge : ");
  if      (poso.m_MEDICA_POSO_AGE_MIN=="0"&&poso.m_MEDICA_POSO_AGE_MAX=="0") page += TR(" Indifférent ");
  else if (poso.m_MEDICA_POSO_AGE_MIN!="0"&&poso.m_MEDICA_POSO_AGE_MAX=="0") page += TR(" au-delà de ")        + Medica_PosologieAgeToHtml(poso.m_MEDICA_POSO_AGE_MIN);
  else if (poso.m_MEDICA_POSO_AGE_MIN=="0"&&poso.m_MEDICA_POSO_AGE_MAX!="0") page += TR(" jusqu'à ")           + Medica_PosologieAgeToHtml(poso.m_MEDICA_POSO_AGE_MAX);
  else     page += TR(" de ")     + Medica_PosologieAgeToHtml(poso.m_MEDICA_POSO_AGE_MIN) + TR(" à ")          + Medica_PosologieAgeToHtml(poso.m_MEDICA_POSO_AGE_MAX);
  //...................... Poids .....................................................
  page += TR("<br>Poids : ");
  if      (poso.m_MEDICA_POSO_POIDS_MIN=="0"&&poso.m_MEDICA_POSO_POIDS_MAX=="0") page += TR(" Indifférent ");
  else if (poso.m_MEDICA_POSO_POIDS_MIN!="0"&&poso.m_MEDICA_POSO_POIDS_MAX=="0") page += TR(" au-delà de ")        + Medica_PosologiePoidsToHtml(poso.m_MEDICA_POSO_POIDS_MIN);
  else if (poso.m_MEDICA_POSO_POIDS_MIN=="0"&&poso.m_MEDICA_POSO_POIDS_MAX!="0") page += TR(" jusqu'à ")           + Medica_PosologiePoidsToHtml(poso.m_MEDICA_POSO_POIDS_MAX);
  else     page += TR(" de ")     + Medica_PosologiePoidsToHtml(poso.m_MEDICA_POSO_POIDS_MIN) + TR(" à ") + Medica_PosologiePoidsToHtml(poso.m_MEDICA_POSO_POIDS_MAX);
  page += "</span>";
  //...................... terrain structuré (rein, foie etc..) ...............................................
  page += "<span style=\"font-family:Arial;font-size:10pt;color:#a50000\">";
  page += Medica_PosologieTerrainToHtml(poso);
  page += "</span>";
  //...................... Posologies ......................................................
  page += "<span style=\"font-family:Arial;font-size:10pt;color:#a50000\">";
  page += TR("<br>Posologie : ");
  if      (poso.m_MEDICA_POSO_DOSE_MIN=="0"&&poso.m_MEDICA_POSO_DOSE_MAX=="0") page += TR(" Pas de posologie indiquée pour ce produit ");
  else if (poso.m_MEDICA_POSO_DOSE_MIN!="0"&&poso.m_MEDICA_POSO_DOSE_MAX=="0")
          {page += poso.m_MEDICA_POSO_DOSE_MIN + " " +
                   Medica_PosologieUP_UnitToHtml(poso.m_MEDICA_POSO_DOSE_UNIT, poso.m_MEDICA_POSO_DOSE_MIN) +
                   Medica_PosologieFactCorpToHtml(poso)                        +
                   Medica_PosologiePeriodeToHtml(poso.m_MEDICA_POSO_PERIODE);
          }
  else if (poso.m_MEDICA_POSO_DOSE_MIN=="0"&&poso.m_MEDICA_POSO_DOSE_MAX!="0") page += TR(" Impossible ");
  else
          {page += TR(" de ") + poso.m_MEDICA_POSO_DOSE_MIN + TR(" à ") + poso.m_MEDICA_POSO_DOSE_MAX +" "+
                   Medica_PosologieUP_UnitToHtml(poso.m_MEDICA_POSO_DOSE_UNIT, poso.m_MEDICA_POSO_DOSE_MAX) +
                   Medica_PosologieFactCorpToHtml(poso)                        +
                   Medica_PosologiePeriodeToHtml(poso.m_MEDICA_POSO_PERIODE);
          }
  page += "</span>";
  //...................... specification de prise ...............................................
  page += "<span style=\"font-family:Arial;font-size:10pt;color:#025600\">";
  page += TR("<br>Spécification de prise : ");
  page += Medica_PosologieSpecPrisesToHtml(poso);
  if (poso.m_MEDICA_POSO_PENDANT.length())
     {float dbl_duree = poso.m_MEDICA_POSO_PENDANT.toFloat() / 86400;
      QString                     str = "";
      if (dbl_duree==365)         str = TR("un an");
      else if (dbl_duree==31)     str = TR("un mois");
      else if (dbl_duree<=1)      str = TR("un jour");
      else                        str = QString::number(dbl_duree) + TR(" jours");
      page += TR("   Durée conseillée : ") + str;
     }
  page += "</span>";
}

//----------------------------------------------------- Medica_PosologieSpecPrisesToHtml ---------------------------------------------------------------------------
QString  CMedicaBase::Medica_PosologieSpecPrisesToHtml(const CPosologie &poso)
{return Medica_PosologieSpecPrisesToHtml( poso.m_MEDICA_POSO_REPART_PRISES, poso.m_MEDICA_POSO_REPART_PRISES.length());
}

//----------------------------------------------------- Medica_PosologieSpecPrisesToHtml ---------------------------------------------------------------------------
// 28800  (8h)  43200 (12h)
// ::J2 28800:43200  Q1/3 && 28800-43200  Q1/3::J2 28800:43200 Q1/3 && 28800-43200
//   ^   ^     ^      ^   ^                   ^
//   |   |     |      |   |                   |
//   |   |     |      |   |                   séparateur de jour
//   |   |     |      |   séparateur de prise dans la journée (soit fractionnaire 1/3 soit pourcentage 50%)
//   |   |     |      fraction de dose totale pour cette tranche horaire
//   |   |     |
//   |   -------- horaires de prescrition dans la journée
//   |            si - on peut remplacer par Matin Midi Soir etc si horaires en rapport
//   |            si : on prend la première valeur comme horaire de prescription
//   |
//    séparateur de jour si suivi de Jn ou rien     --> tous les jours
//                       si suivi de J1 ou 2 ou ... --> numérotation du jour (J1 le premier jour J2 le deuxième etc)
//
// ::28800-43200 Q1/3 && 28800-43200 Q1/3
//          ^             ^
//          |             |
//          |              séparateur de prise
//           si - on peut remplacer par Matin Midi Soir etc si horaires en rapport
//           si : on prend la première valeur comme horaire de prescription
//
//
// ::28800-43200 Q1/3 && 28800-43200 Q1/3   ZJ1-J2-J3-J4-J5/S "5 jours sur sept"
//                                            ^               ^
//                                            |               |
//                                            |               commentaire à insérer
//                                             spécification de fréquence globale (1er 2ème 3ème 4ème 5ème jour de la semaine)
//
//                                           ZJ1-J2-J3-J4-J5/S  (1er 2ème 3ème 4ème 5ème jour de la semaine)
//                                           ZJ1-J2-J3/M        (1er 2ème 3ème jour du mois)
//                                           ZS1-S2/M           (1ère 2ème  semaine du mois)
//                                           ZM1(ZS1-S2/M)-2/A  (1ère 2ème  semaine du premier et deuxième mois de l'année)
// ::00000-86400 SE # Braun # 48 # 20 # unité # 2
//        ^                   ^    ^            ^
//        |                   |    |            | vitesse de la seringue
//        |                   |     dose totale dans la seringue
//        |                    contenance de la seringue
//         horaires de prescrition dans la journée
//
//
QString  CMedicaBase::Medica_PosologieSpecPrisesToHtml(const char *pt_i, long len)
{QString   result(""),val1, val2, unit1, unit2;
 char *pt     =  (char*) pt_i;
 char *end    =  pt + len;
 char *lastEt =  strFindRev(';', pt, len);

 while (pt<end)
 {

  if (pt[0]==';')
     {if (pt==lastEt)             {result += TR(", et");                          ++pt;}
      else                        {result += TR(", ");                            ++pt;}
     }
  else if (pt[0]=='V'&& pt[1]=='S')    {result += TR(" la veille au soir");            pt +=2;}  // VS  -->  (la veille au soir)
  else if (pt[0]=='M'&& pt[1]=='A')    {result += TR(" le matin");                     pt +=2;}
  else if (pt[0]=='S'&& pt[1]=='O')    {result += TR(" le soir");                      pt +=2;}
  else if (pt[0]=='C'&& pt[1]=='O')    {result += TR(" au coucher");                   pt +=2;}
  else if (pt[0]=='A'&& pt[1]=='M')    {result += TR(" l'après midi");                 pt +=2;}
  else if (pt[0]=='M'&& pt[1]=='I')    {result += TR(" à midi");                       pt +=2;}
  else if (pt[0]=='I'&& pt[1]=='A')    {result += TR(" à l'induction anesthésique");   pt +=2;}
  else if (pt[0]=='L'&& pt[1]=='V')    {result += TR(" au lever");                     pt +=2;}
  else if (pt[0]=='T'&& pt[1]=='O')    {result += TR(" à la toilette");                pt +=2;}
  else if (pt[0]=='T'&& pt[1]=='M')    {result += TR(" à la toilette du matin");       pt +=2;}
  else if (pt[0]=='T'&& pt[1]=='S')    {result += TR(" à la toilette du soir");        pt +=2;}
  else if ((*pt=='-' || *pt=='+') && pt[1]>='0' && pt[1]<='9')
     {char op = *pt;
      pt      = Medica_ExtractNumber(pt+1, val1);
      val1    = Medica_MinutesToTime(val1);
      result += " " + val1 + ((op=='+')?TR(" après"):TR(" avant"));
     }
  //................................ zone numérique .............................................................
  //  0,33SO               1800<>3600-CO       1800<>3600-r||7200+rSO     1800<>3600-rDJ;1800<>3600-rSO;
  //  28800HR;72000HR      36000HRMA     2XJO  1800<>3600-rDJ;1800<>3600-rSO;
  else if (*pt>='0' && *pt<='9')
     {pt = Medica_ExtractNumber(pt, val1);                                                         //  2XJO
      if (strncmp(pt , "XJO", 3)==0)  {result += val1 + TR(" fois par jour"); pt +=3;}
      else
        {val1 = Medica_MinutesToTime(val1);
         if      (pt[0]=='H'&& pt[1]=='R')                                                        //  28800HR;72000HR      36000HRMA
            {result +=  TR(" à ")  + val1; pt +=2;
             if       (pt[0]=='M'&& pt[1]=='A')      {result +=  TR(" du matin")          + val1; pt +=2;}
             else if  (pt[0]=='A'&& pt[1]=='M')      {result +=  TR(" de l'après midi")   + val1; pt +=2;}
             else if  (pt[0]=='S'&& pt[1]=='O')      {result +=  TR(" du soir")           + val1; pt +=2;}
            }           //  1800HR
         else if (pt[0]=='H'&& pt[1]=='D')
            {result +=  TR(" sur ")  + val1; pt +=2;
            }
         else if (pt[0]=='<'&& pt[1]=='>')    {result +=  TR(" à")  + val1; pt +=2;}     //  600<>
         else if (pt[0]=='|'&& pt[1]=='|')    {result +=  TR(" ou") + val1; pt +=2;}     //  600||
         else if (pt[0]=='-'|| pt[0]=='+' && pt[1]=='r')                                          //  600-rDJ  600+rDJ  600-r||
            {char op = *pt;
             pt += 2;
             if      (pt[0]=='D'&&pt[1]=='J') {result += " " + val1 + ((op=='+')?TR(" après "):TR(" avant")) + TR(" le petit déjeuner");   pt +=2;}
             else if (pt[0]=='M'&&pt[1]=='A') {result += " " + val1 + ((op=='+')?TR(" après "):TR(" avant")) + TR(" le petit déjeuner");   pt +=2;}
             else if (pt[0]=='M'&&pt[1]=='I') {result += " " + val1 + ((op=='+')?TR(" après "):TR(" avant")) + TR(" le repas de midi");    pt +=2;}
             else if (pt[0]=='S'&&pt[1]=='O') {result += " " + val1 + ((op=='+')?TR(" après "):TR(" avant")) + TR(" le repas du soir");    pt +=2;}
             else if (pt[0]=='|'&&pt[1]=='|') {result += " " + val1 + ((op=='+')?TR(" après "):TR(" avant")) + TR(" ou");                  pt +=2;}
            }
         else if (pt[0]=='-'|| pt[0]=='+')                                                        //   900+TO   900<>1800-CO  3600-CO 1800-oLV
            {char op = *pt; ++pt;
             if      (pt[0]=='M'&&pt[1]=='A') {result += " " + val1 + ((op=='+')?TR(" après "):TR(" avant")) + TR(" le matin");                  pt +=2;}
             else if (pt[0]=='L'&&pt[1]=='V') {result += " " + val1 + ((op=='+')?TR(" après "):TR(" avant")) + TR(" le lever");                  pt +=2;}
             else if (pt[0]=='M'&&pt[1]=='I') {result += " " + val1 + ((op=='+')?TR(" après "):TR(" avant")) + TR(" midi");                      pt +=2;}
             else if (pt[0]=='S'&&pt[1]=='O') {result += " " + val1 + ((op=='+')?TR(" après "):TR(" avant")) + TR(" le soir");                   pt +=2;}
             else if (pt[0]=='T'&&pt[1]=='O') {result += " " + val1 + ((op=='+')?TR(" après "):TR(" avant")) + TR(" la toilette");               pt +=2;}
             else if (pt[0]=='T'&&pt[1]=='M') {result += " " + val1 + ((op=='+')?TR(" après "):TR(" avant")) + TR(" la toilette du matin");      pt +=2;}
             else if (pt[0]=='T'&&pt[1]=='S') {result += " " + val1 + ((op=='+')?TR(" après "):TR(" avant")) + TR(" la toilette du soir");       pt +=2;}
             else if (pt[0]=='I'&&pt[1]=='A') {result += " " + val1 + ((op=='+')?TR(" après "):TR(" avant")) + TR(" l'induction anesthésique");  pt +=2;}
             else if (pt[0]=='C'&&pt[1]=='O') {result += " " + val1 + ((op=='+')?TR(" après "):TR(" avant")) + TR(" le coucher");                pt +=2;}
             else if (pt[0]=='|'&&pt[1]=='|') {result += " " + val1 + ((op=='+')?TR(" après "):TR(" avant")) + TR(" ou");                        pt +=2;}
            }
        }
     }
  //.............................. operateur ................................
  // -CO (avant)   .IA (au debut)   #MA (au cours)  #r (au cours d'un repas) +r (avant un repas) -rDJ  -r||#rDJ;-r||#rSO    .rDJ  #rDJ   +rDJ
  //  -rMI
  else if (*pt=='.' || *pt=='+' || *pt=='-' || *pt=='#')
     {char op = *pt;  ++pt;
      if (*pt=='r')
         {   ++pt;
             if      (pt[0]=='D'&&pt[1]=='J') {result += " " + Medica_OpToTxt(op, "le") + TR(" petit déjeuner");   pt +=2;}
             else if (pt[0]=='M'&&pt[1]=='A') {result += " " + Medica_OpToTxt(op, "le") + TR(" petit déjeuner");   pt +=2;}
             else if (pt[0]=='M'&&pt[1]=='I') {result += " " + Medica_OpToTxt(op, "le") + TR(" repas de midi");    pt +=2;}
             else if (pt[0]=='S'&&pt[1]=='O') {result += " " + Medica_OpToTxt(op, "le") + TR(" repas du soir");    pt +=2;}
             else if (pt[0]=='|'&&pt[1]=='|') {result += " " + Medica_OpToTxt(op, "n")  + TR(" ou");               pt +=2;}
             else                             {result += " " + Medica_OpToTxt(op, "y")  + TR(" repas");            pt +=2;}

         }
      else if (pt[0]=='L'&& pt[1]=='V')    {result += " " + Medica_OpToTxt(op, "le") + TR(" lever");                  pt +=2;}
      else if (pt[0]=='M'&& pt[1]=='A')    {result += " " + Medica_OpToTxt(op, "le") + TR(" matin");                  pt +=2;}
      else if (pt[0]=='S'&& pt[1]=='O')    {result += " " + Medica_OpToTxt(op, "le") + TR(" soir");                   pt +=2;}
      else if (pt[0]=='C'&& pt[1]=='O')    {result += " " + Medica_OpToTxt(op, "le") + TR(" coucher");                pt +=2;}
      else if (pt[0]=='T'&& pt[1]=='O')    {result += " " + Medica_OpToTxt(op, "la") + TR(" toilette");               pt +=2;}
      else if (pt[0]=='T'&& pt[1]=='M')    {result += " " + Medica_OpToTxt(op, "la") + TR(" toilette du matin");      pt +=2;}
      else if (pt[0]=='T'&& pt[1]=='S')    {result += " " + Medica_OpToTxt(op, "la") + TR(" toilette du soir");       pt +=2;}
      else if (pt[0]=='I'&& pt[1]=='A')    {result += " " + Medica_OpToTxt(op, "l'") + TR(" induction anesthésique"); pt +=2;}
      else if (pt[0]=='A'&& pt[1]=='M')    {result += " " + Medica_OpToTxt(op, "l'") + TR(" après midi");             pt +=2;}
     }
  //.............................. r ................................
  //  rSO      rMI;rSO
  //
  else if (*pt=='r')
     {++pt;
      if      (pt[0]=='D'&&pt[1]=='J') {result += " "  + TR(" au petit déjeuner");   pt +=2;}
      else if (pt[0]=='M'&&pt[1]=='A') {result += " "  + TR(" au petit déjeuner");   pt +=2;}
      else if (pt[0]=='M'&&pt[1]=='I') {result += " "  + TR(" au repas de midi");    pt +=2;}
      else if (pt[0]=='S'&&pt[1]=='O') {result += " "  + TR(" au repas du soir");    pt +=2;}
      else if (pt[0]=='|'&&pt[1]=='|') {result += " "  + TR(" au repas ou");         pt +=2;}
     }
  else ++pt;
 }  // end while (pt<end)
 //................................ factoriser au cours du repas ..................................................................................
 return result;
}

//----------------------------------------------------- Medica_OpToTxt ---------------------------------------------------------------------------
QString CMedicaBase::Medica_OpToTxt(int op, const QString &article)
{QString ret("");
 switch(op)
    {case '.': ret = TR(" au début du")  + article; break;
     case '#': ret = TR(" au cours du")  + article; break;
     case '-': ret = TR(" avant le")     + article; break;
     case '+': ret = TR(" après le")     + article; break;
     default: return ret;
    }
 ret.replace("dule","du");
 ret.replace("dul'","de l'");
 ret.replace("lele","le");
 ret.replace("lel'","l'");
 ret.replace("len'","");
 ret.replace("dun'","");
 ret.replace("dula'","de la");
 ret.replace("lela'","la");
 ret.replace("ley'","le");
 ret.replace("duy'","du");
 return ret;
}

//----------------------------------------------------- strFindNext -------------------------------------------------------------------------------------
long  CMedicaBase::strFindNext(char car, const char* pt_i, long len)
{char* f = strFindNext(car, pt_i,  pt_i + len);
 if (f == 0) return -1;
 else        return f-pt_i;
}

//----------------------------------------------------- strFindNext -------------------------------------------------------------------------------------
char  *CMedicaBase::strFindNext(char car, const char* pt_i,  const char* end)
{char *pt  = (char*)pt_i;
 while(pt++ < end )
    {if (*pt==car) return pt;
    }
 return 0;
}

//----------------------------------------------------- strFindRev -------------------------------------------------------------------------------------
char*  CMedicaBase::strFindRev(char car, const char* pt_i, long len)
{char *pt = (char*)pt_i + len;
 while(--pt >= pt_i )
    {if (*pt==car) return pt;
    }
 return 0;
}

//----------------------------------------------------- Medica_ExtractNumber ---------------------------------------------------------------------------
char *CMedicaBase::Medica_ExtractNumber(char *pt, QString &val)
{char *deb = pt;
 while (*pt>='0' && *pt<='9' || *pt<=',') ++pt;
 val.setLatin1 (deb, pt-deb );
 return pt;
}

//----------------------------------------------------- Medica_MinutesToTime ---------------------------------------------------------------------------
QString  CMedicaBase::Medica_MinutesToTime(const QString minutes)
{QString ret("");
 long mn = minutes.toLong();
 long hr = mn/3600;
 mn      = mn-hr*3600;
 if (hr&&mn)  {ret  = QString::number(hr) + TR(" heure"); if (hr>1) ret+="s";
               ret += " " + QString::number(mn);
              }
 else if (hr) {ret  = QString::number(hr)  + TR(" heure");   if (hr>1) ret+="s";}
 else if (mn) {ret  = QString::number(mn)  + TR(" minute");  if (mn>1) ret+="s";}
 return ret;
}

//----------------------------------------------------- Medica_PosologieTerrainToHtml ---------------------------------------------------------------------------
QString  CMedicaBase::Medica_PosologieTerrainToHtml(const CPosologie &poso)
{ return Medica_PosologieTerrainToHtml(poso.m_MEDICA_POSO_TERRAIN);
}

//----------------------------------------------------- Medica_PosologieTerrainToHtml ---------------------------------------------------------------------------
QString  CMedicaBase::Medica_PosologieTerrainToHtml(const QString &terrain)
{ QString var("");
  QString ret("");                      //  $CL:10:25:ml/mn
  int     varMustBeInterpreted = 1;     // les valeurs des variables doivent être interpretées et ajoutées
  QStringList list = QStringList::split ("$", terrain );
  if (list.count()==0) return ret;
  for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
      {QStringList sublist = QStringList::split (":", *it , TRUE);
       int n = sublist.count();
       if (n==4)
          {// " $AL:::"
           // " $CR:::"
           // " $GR:::"
           // " $CL:::"
           // " $BL:::"
           // " CR::0:ml/mn"
           sublist[1].stripWhiteSpace();
           sublist[2].stripWhiteSpace();
           if (*it=="CR::0:ml/mn" ||
               *it=="CR:::"       ||
               *it=="CL:::" )         {ret += TR(" <br>hémodialysé.");             varMustBeInterpreted= 0;}
           else if (*it=="GR:::")     {ret += TR(" <br>Grossesse en cours. ");     varMustBeInterpreted= 0;}
           else if (sublist[0]=="AL") {ret += TR(" <br>Allaitement en cours. ");   varMustBeInterpreted= 0;}
           else if (sublist[0]=="AG") {ret += TR(" <br>Âge : ");
                                       var  = TR("âge");
                                       if (sublist[1].length()==0 && sublist[2].length()==0) {}
                                       else if (sublist[1].length()==0 && sublist[2].length()!=0) ret += " " + var + " &lt; " + Medica_PosologieAgeToHtml(sublist[2]);
                                       else if (sublist[1].length()!=0 && sublist[2].length()==0) ret += " " + var + " &gt; " + Medica_PosologieAgeToHtml(sublist[1]);
                                       else ret += " " + Medica_PosologieAgeToHtml(sublist[1]) + " &lt; "    + var + " &lt; " + Medica_PosologieAgeToHtml(sublist[2]);
                                       varMustBeInterpreted = 0;
                                      }
           else if (sublist[0]=="PS") {ret += TR(" <br>Poids : ");                  var = TR("poids");}
           else if (sublist[0]=="CR") {ret += TR(" <br>Insuffisance rénale : ");    var = TR("créatinémie");}
           else if (sublist[0]=="CR") {ret += TR(" <br>Insuffisance rénale : ");    var = TR("créatinémie");}
           else if (sublist[0]=="GR") {ret += TR(" <br>Grossesse en cours : ");     var = TR("âgée de");}
           else if (sublist[0]=="CL") {ret += TR(" <br>Insuffisance rénale : ");    var = TR("clearance de la créatine");}
           else if (sublist[0]=="BL") {ret += TR(" <br>Insuffisance hépatique : "); var = TR("bilibirubinémie");}
           else if (sublist[0]=="FJ") {ret += TR(" <br>Insuffisance cardiaque : "); var = TR("Frac. Éjection");}
           else varMustBeInterpreted = 0;
           if (varMustBeInterpreted)
              {if (sublist[1].length()==0 && sublist[2].length()==0) {}
               else if (sublist[1].length()==0 && sublist[2].length()!=0)
                    ret = ret + " " + var + " &lt; " + sublist[2] + " " + sublist[3];
               else if (sublist[1].length()!=0 && sublist[2].length()==0)
                    ret = ret + " " + var + " &gt; " + sublist[1] + " " + sublist[3];
               else
                    ret = ret + " " + sublist[1]  + " &lt; "      + var + " &lt; "  + sublist[2] + " " + sublist[3];
              }
           varMustBeInterpreted= 1;    // à priori les valeurs des variables doivent être interpretées
          }
      }
  return ret;
}

//----------------------------------------------------- Medica_PosologieFactCorpToHtml ---------------------------------------------------------------------------
QString  CMedicaBase::Medica_PosologieFactCorpToHtml(const CPosologie &poso)
{QString ret("");
 int nb = (poso.m_MEDICA_POSO_FACT_CORP_QU.toInt());
 QString unit(Medica_GetUnite(m_MEDICA_FORME_AD_TBL_NAME, poso.m_MEDICA_POSO_FACT_CORP_UNIT));
 if (nb == 0 )   return ret;
 if (nb>1) ret = TR(" par ") + poso.m_MEDICA_POSO_FACT_CORP_QU +" "+ unit;
 else      ret = TR(" par ") + unit;
 if      (unit=="Kg")    ret += TR(" de poids, ");
 else if (unit=="m2")    ret += TR(" de surface corporelle, ");
 return ret;
}

//----------------------------------------------------- Medica_PosologiePeriodeToHtml ---------------------------------------------------------------------------
QString  CMedicaBase::Medica_PosologiePeriodeToHtml(const QString s_secondes)
{float f_secondes = s_secondes.toFloat();
 QString ret;
 int jours     = (int) f_secondes/86400;
 int semaines  =  jours / 7;
 int heures    = ((int)f_secondes - (jours*86400)) / 3600;
 if (semaines!=0 && jours == 0)
    {if (semaines==1) ret = TR(" toutes les semaines ");
     else             ret = TR(" toutes les ") + QString::number(semaines) + TR(" semaines ");
    }
 else if (jours !=0 && heures==0)
    {if (jours==1)    ret = TR(" tous les jours")  ;
     else             ret = TR(" tous les ") + QString::number(jours)    + TR(" jours ")  ;
    }
 else if (heures !=0)
    {if (heures==1)   ret = TR(" toutes les heures") ;
     else             ret = TR(" toutes les ") + QString::number(heures)   + TR(" heures");
    }
 return ret;
}

//----------------------------------------------------- Medica_PosologieDureeToHtml ---------------------------------------------------------------------------
QString  CMedicaBase::Medica_PosologieDureeToHtml(const QString s_secondes)
{float f_secondes = s_secondes.toFloat();
 QString ret("");
 int mois      = 0;
 int jours     = (int) f_secondes/86400;
 int ans       = 0;
     ans   = jours/365;
     jours = jours - ans * 365;
     mois  = jours/31;
     jours = jours - mois * 31;
     if (ans !=0)
        {if (ans==1)        ret += TR(" un an") ;
         else               ret += " " + QString::number(ans)      + TR(" ans ")  ;
        }
     if (mois !=0)
        {if (ans !=0)       ret += TR(" et");
         if (mois==1)       ret += TR(" un mois")  ;
         else               ret += " " + QString::number(mois)     + TR(" mois ")  ;
        }
    if (jours !=0)
       { if (ans>0||mois>0) ret += TR(" et");
         if (jours==1)      ret += TR(" un jour")  ;
         else               ret += " " + QString::number(jours)    + TR(" jours ")  ;
       }
 return ret;
}

//----------------------------------------------------- Medica_PosologieDureeJourToHtml ---------------------------------------------------------------------------
QString  CMedicaBase::Medica_PosologieDureeJourToHtml(const QString jours)
{QString n = QString::number(jours.toInt()*86400);
 return Medica_PosologieDureeToHtml(n);
}

//----------------------------------------------------- Medica_PosologiePoidsToHtml ---------------------------------------------------------------------------
QString  CMedicaBase::Medica_PosologiePoidsToHtml(QString s_poids)
{return s_poids + TR(" kg ");
}

//----------------------------------------------------- Medica_PosologieAgeToHtml ---------------------------------------------------------------------------
QString  CMedicaBase::Medica_PosologieAgeToHtml(QString s_month)
{float f_month = s_month.toFloat();
 QString ret;
 int year   = (int) f_month/12;
 int month  = (int) f_month - (year*12);
 if (year==0)         ret = QString::number(month) + TR(" mois ");
 else if (month !=0)  ret = QString::number(year)  + TR(" an") +  ((year>1)?"s et ":" et ") + QString::number(month) + TR(" mois ");
 else                 ret = QString::number(year)  + TR(" an") +  ((year>1)?"s":"");
 return ret;
}

//-----------------------------------------------------  Medica_GetSecabilite -------------------------------------------
QString CMedicaBase::Medica_GetSecabilite( const QString &cip)
{QString    requete ("");
 //................. Preparer la requete .....................................
  if ( m_MedicaBase==0 )                                              return "1";
  if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)   return "1";
  requete       += "SELECT " +  m_MEDICA_SPEC_SECABILITE       +
                   " FROM "  + m_MEDICA_SPEC_TBL_NAME          + " WHERE " + m_MEDICA_SPEC_CIP + " = '" + cip +"'";
  QSqlQuery query(requete , m_MedicaBase );
  //.................. si la requète a un resultat ..............................................
  if (query.isActive() && query.next())
     {return query.value( 0 ).toString();
     } //endif (pSqlQuery && pSqlQuery->isActive())
  return "1";
}

//-----------------------------------------------------  Medica_GetSpecifications -------------------------------------------
int CMedicaBase::Medica_GetSpecifications( QString cip, QStringList &qstringList)
{ QString    requete ("");
 //................. Preparer la requete .....................................
  if ( m_MedicaBase==0 )                                              return 0;
  if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)   return 0;
  requete       += "SELECT " +  m_MEDICA_SPEC_UP_UNIT          + "," +
                                m_MEDICA_SPEC_PA_UNIT          + "," +
                                m_MEDICA_SPEC_PA_QU            + "," +
                                m_MEDICA_SPEC_SECABILITE       + "," +
                                m_MEDICA_SPEC_IS_GENERIC       + "," +
                                m_MEDICA_SPEC_FORM_STRUC       + "," +
                                m_MEDICA_SPEC_NB_FORME         + "," +
                                m_MEDICA_SPEC_PRIX_BOITE       + "," +
                                m_MEDICA_SPEC_REMBOURSEMENT    + "," +
                                m_MEDICA_SPEC_TABLEAU          + "," +
                                m_MEDICA_SPEC_LABO             + "," +
                                m_MEDICA_SPEC_ATC              +
                   " FROM " + m_MEDICA_SPEC_TBL_NAME + " WHERE " + m_MEDICA_SPEC_CIP + " = '" + cip +"'";

  QSqlQuery query(requete , m_MedicaBase );

  //.................. si la requète a un resultat ..............................................
  if (query.isActive() && query.next())
     {qstringList.clear();
      qstringList.append(Utf8_Query(query, 0     ));
      qstringList.append(Utf8_Query(query, 1     ));
      qstringList.append(Utf8_Query(query, 2     ));
      qstringList.append(Utf8_Query(query, 3     ));
      qstringList.append(Utf8_Query(query, 4     ));
      qstringList.append(Utf8_Query(query, 5     ));
      qstringList.append(Utf8_Query(query, 6     ));
      qstringList.append(Utf8_Query(query, 7     ));
      qstringList.append(Utf8_Query(query, 8     ));
      qstringList.append(Utf8_Query(query, 9     ));
      qstringList.append(Utf8_Query(query, 10    ));
      qstringList.append(Utf8_Query(query, 11    ));
      return 1;
     } //endif (pSqlQuery && pSqlQuery->isActive())
  return 0;
}

//----------------------------------------------------- Medica_GetSpecifications ---------------------------------------------------------------------------
QString CMedicaBase::Medica_GetSpecifications(    const QString &cip,
                                                  QString &up_unit,         // ok
                                                  QString &pa_unit,         // ok
                                                  QString &pa_qu,
                                                  QString &secabilite,      // ok
                                                  QString &is_generic,      // ok
                                                  QString &formStruct,      // ok
                                                  QString &nbPrises,        // ok
                                                  QString &prixBoite,       // ok
                                                  QString &remboursementSS, // ok
                                                  QString &tableau,         // ok
                                                  QString &labo,            // ok
                                                  QString &atc              // ok
                                                )
{    QStringList qstringList;
     int i = 0;
     if (Medica_GetSpecifications(   cip, qstringList)==0)
        {up_unit = pa_unit = pa_qu = secabilite = is_generic = formStruct = nbPrises = prixBoite = remboursementSS = tableau = labo = atc ="";
        }
     else
        {up_unit         = Medica_GetUnite(m_MEDICA_FORME_UP_TBL_NAME, qstringList[i++]);
         pa_unit         = Medica_GetUnite(m_MEDICA_FORME_PA_TBL_NAME, qstringList[i++]);
         pa_qu           = qstringList[i++];
         secabilite      = qstringList[i++];
         is_generic      = qstringList[i++];
         formStruct      = Medica_GetUnite(m_MEDICA_FORME_ST_TBL_NAME, qstringList[i++]);
         nbPrises        = qstringList[i++];
         prixBoite       = qstringList[i++];
         remboursementSS = qstringList[i++];
         tableau         = qstringList[i++];
         labo            = qstringList[i++];
         atc             = qstringList[i++];
        }
     return atc;
}

//----------------------------------------------------- Medica_GetLastPrimKey ---------------------------------------------------------------------------
long CMedicaBase::Medica_GetLastPrimKey(const QString &table_name, const QString &primKeyName)
{ long last_pk = 0;
  if ( m_MedicaBase==0 )                                              return 0;
  if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)   return 0;
  //................. chercher données ...................................................
  QSqlQuery query (QString("SELECT ") +  primKeyName    +  " FROM "  + table_name, m_MedicaBase );
  if (query.isActive())
     {
      while(query.next())
         {long pk_toTest = query.value(0).toString().toLong();
          if (pk_toTest > last_pk) last_pk = pk_toTest;
         }
     }
  return last_pk;
}

//----------------------------------------------------- Medica_GetUnite ---------------------------------------------------------------------------
QString CMedicaBase::Medica_GetUnite(const QString &table_name, const QString &code)
{ QString      requete("SELECT ");
  if ( m_MedicaBase==0 )                                              return 0;
  if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)   return 0;
  //................. chercher données economiques pour ce code CIP...................................................
  requete +=             m_MEDICA_FORME_LIBELLE    +
             " FROM "  + table_name                + " WHERE " + m_MEDICA_FORME_PK + " ='" + code + "'";
  QSqlQuery query (requete , m_MedicaBase );
  if (query.isActive() && query.next()) return Utf8_Query(query,0);
  // if (query.isActive() && query.next()) return query.value(0).toString();
  return QString("");
}

//----------------------------------------------------- Medica_UnitePriseAdd ---------------------------------------------------------------------------
int CMedicaBase::Medica_UnitePriseAdd(const QString &table_name, const QString &libelle, const QString &pk)
{if ( m_MedicaBase==0 )                                              return false;
 if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)   return FALSE;
  //............... crer un curseur SQL ..................................................
 QSqlCursor cur(table_name, TRUE, m_MedicaBase);
 if ( !cur.canInsert() )                                             return FALSE;
 //............... si OK on recupere le buffer et on le charge avec les données .........
 QSqlRecord *buffer = cur.primeInsert();                             // recuperer le buffer d'insertion
 buffer->setValue( m_MEDICA_FORME_LIBELLE, libelle );                // y placer les données (libelle)
 buffer->setValue( m_MEDICA_FORME_PK,      pk );                     // y placer les données (clef primaire)
 cur.insert();                                                       // ecrire les données
 return TRUE;
}

//----------------------------------------------------- Medica_GetUniteID ---------------------------------------------------------------------------
QString CMedicaBase::Medica_GetUniteID(const QString &table_name, const QString &libelle)
{ QString      requete("SELECT ");
  if ( m_MedicaBase==0 )                                              return 0;
  if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)   return 0;
  //................. chercher données economiques pour ce code CIP...................................................
  requete +=             m_MEDICA_FORME_PK    +
             " FROM "  + table_name           + " WHERE " + m_MEDICA_FORME_LIBELLE + " ='" + libelle + "'";
  QSqlQuery query (requete , m_MedicaBase );
  if (query.isActive() && query.next()) return query.value(0).toString();
  return QString("");
}

//----------------------------------------------------- Medica_GetUniteList ---------------------------------------------------------------------------
long CMedicaBase::Medica_GetUnite(const QString &table_name, QStringList &unitList)
{ long nb(0);
  if ( m_MedicaBase==0 )                                              return 0;
  if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)   return 0;
  //................. chercher données ...................................................
  QSqlQuery query (QString("SELECT ") +  m_MEDICA_FORME_LIBELLE    +  " FROM "  + table_name + " ORDER BY '" + m_MEDICA_FORME_LIBELLE + "'", m_MedicaBase );
  if (query.isActive())
     {unitList.clear();
      while(query.next())
         {unitList.append(Utf8_Query(query,0));
          ++nb;
         }
     }
  return nb;
}

//----------------------------------------------------- Medica_IsThisFormeExist ---------------------------------------------------------------------------
bool CMedicaBase::Medica_IsThisFormeExist(QStringList &list, const QString &forme, int &next)
{
 next = 0;
 for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
     {++next;
      if (forme== (*it)) return 1;
     }
 list.append(forme);
 ++next;
 return 0;
}

//-----------------------------------------------------  Medica_GetUnitesList -------------------------------------------
int CMedicaBase::Medica_GetUnitesList( LIST_TYPE list_type, QStringList &list, int mode /*=CMedicaBase::WithPK*/)
{ QString      table_name("");
  switch(list_type)
  {case CMedicaBase::UP: table_name = m_MEDICA_FORME_UP_TBL_NAME; break;
   case CMedicaBase::PA: table_name = m_MEDICA_FORME_PA_TBL_NAME; break;
   case CMedicaBase::ST: table_name = m_MEDICA_FORME_ST_TBL_NAME; break;
   case CMedicaBase::AD: table_name = m_MEDICA_FORME_AD_TBL_NAME; break;
  }
 return Medica_GetUnitesList(table_name, list,  mode );
}

//-----------------------------------------------------  Medica_GetUnitesList -------------------------------------------
int CMedicaBase::Medica_GetUnitesList( const QString &table_name, QStringList &list, int mode /*=CMedicaBase::WithPK*/)
{
  QString      requete("SELECT ");
  if ( m_MedicaBase==0 )                                              return 0;
  if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)   return 0;
  //................. chercher données economiques pour ce code CIP...................................................
  requete +=             m_MEDICA_FORME_PK        + "," +
                         m_MEDICA_FORME_LIBELLE   +
             " FROM "  + table_name;
  int nb = 0;
  QSqlQuery query(requete , m_MedicaBase );
  if (query.isActive() )
     {list.clear();
      while (query.next())
            {
             if (mode == CMedicaBase::WithPK) list.append( Utf8_Query(query,1) + "                                                      [~]" +  Utf8_Query(query,0));
             else                             list.append( Utf8_Query(query,1)   );
             ++nb;
            }
     }
  return nb;
}

//----------------------------------------------------- Medica_GetSchemaDePrise ---------------------------------------------------------------------------
int CMedicaBase::Medica_GetSchemaDePrise(const QString &pk_poso, QString &nbPrises, QString &schemaPrise)
{ QString      requete("SELECT ");
  if ( m_MedicaBase==0 )                                              return 0;
  if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)   return 0;
  //................. chercher données economiques pour ce code CIP...................................................
  requete +=             m_MEDICA_POSO_NB_PRISES        + "," +
                         m_MEDICA_POSO_REPART_PRISES    +
             " FROM "  + m_MEDICA_POSO_TBL_NAME         + " WHERE " + m_MEDICA_POSO_PK + " ='" + pk_poso + "'";
  QSqlQuery query (requete , m_MedicaBase );
  if (query.isActive() && query.next())
     {nbPrises    = Utf8_Query(query,0);
      schemaPrise = Utf8_Query(query,1);
      return nbPrises.toInt();
    }
  nbPrises    = "0";
  schemaPrise = "";
  return 0;
}

//----------------------------------------------------- Medica_PercentError ---------------------------------------------------------------------------
int CMedicaBase::Medica_PercentError(const float &val1, const float &val2)
{if   (val1>val2) return (int)  (100*val2/val1);
 else             return (int)  (100*val1/val2);
}

//----------------------------------------------------- Medica_PosologieDeleteToBase ---------------------------------------------------------------------------
bool CMedicaBase::Medica_PosologieDeleteToBase(const QString &primKey, const QString &tableName )
{ //............... ouvrir la base .......................................................
  if ( m_MedicaBase==0 )                                              return false;
  if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)   return FALSE;
  //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;
  QString requete("DELETE FROM ");
          requete += tableName + " WHERE ";
          requete += m_MEDICA_POSO_PK       + "= '" + primKey   + "' ";
  QSqlQuery* pSqlQuery = new QSqlQuery(QString::null , m_MedicaBase );
  if (pSqlQuery) ok = pSqlQuery->exec(requete);
  if (pSqlQuery) delete  pSqlQuery;
  return ok;
}

//----------------------------------------------------- Medica_PosologieDeleteToBaseByCIP ---------------------------------------------------------------------------
bool CMedicaBase::Medica_PosologieDeleteToBaseByCIP(const QString &codeCIP, const QString &tableName )
{ //............... ouvrir la base .......................................................
  if ( m_MedicaBase==0 )                                              return false;
  if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)   return FALSE;
  //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;
  QString requete("DELETE FROM ");
          requete += tableName + " WHERE ";
          requete += m_MEDICA_POSO_CIP       + "= '" + codeCIP   + "' ";
  QSqlQuery* pSqlQuery = new QSqlQuery(QString::null , m_MedicaBase );
  if (pSqlQuery) ok = pSqlQuery->exec(requete);
  if (pSqlQuery) delete  pSqlQuery;
  return ok;
}

//----------------------------------------------------- Medica_GetPosologies ---------------------------------------------------------------------------
void  CMedicaBase::Medica_GetPosologies(const QString &pk, CPosologie &posologie )
{QString      requete("SELECT ");
 if ( m_MedicaBase==0 )                                              return;
 if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)   return;

 //............... requete utilisant la base virtuelle .....................................................
 requete +=  m_MEDICA_POSO_CIP             + "," +         // 0
             m_MEDICA_POSO_PK              + "," +         // 1
             m_MEDICA_POSO_LIBELLE         + "," +         // 2
             m_MEDICA_POSO_AGE_MIN         + "," +         // 3
             m_MEDICA_POSO_AGE_MAX         + "," +         // 4
             m_MEDICA_POSO_SEXE            + "," +         // 5
             m_MEDICA_POSO_TERRAIN         + "," +         // 6
             m_MEDICA_POSO_POIDS_MIN       + "," +         // 7
             m_MEDICA_POSO_POIDS_MAX       + "," +         // 8
             m_MEDICA_POSO_DOSE_MIN        + "," +         // 9
             m_MEDICA_POSO_DOSE_MAX        + "," +         // 10
             m_MEDICA_POSO_DOSE_UNIT       + "," +         // 11
             m_MEDICA_POSO_DOSE_LIMIT_MAX  + "," +         // 12
             m_MEDICA_POSO_FACT_CORP_QU    + "," +         // 13
             m_MEDICA_POSO_FACT_CORP_UNIT  + "," +         // 14
             m_MEDICA_POSO_PERIODE         + "," +         // 15
             m_MEDICA_POSO_EQUI_COEFF      + "," +         // 16
             m_MEDICA_POSO_UNIT_COEFF      + "," +         // 17
             m_MEDICA_POSO_NB_PRISES       + "," +         // 18
             m_MEDICA_POSO_REPART_PRISES   + "," +         // 19
             m_MEDICA_POSO_DIVERS          + "," +         // 20
             m_MEDICA_POSO_PENDANT         + "," +         // 21
             m_MEDICA_POSO_SECABILITE      + "," +         // 22
             m_MEDICA_POSO_EXTRA_PK        + " ";          // 23

 requete += " FROM " + m_MEDICA_POSO_TBL_NAME + " WHERE ";
 requete +=  m_MEDICA_POSO_PK     + "='"  + pk   + "'";

 QSqlQuery query(requete , m_MedicaBase );
 if (query.isActive() && query.next())
    {posologie.setData(     Utf8_Query(query,0),
                            Utf8_Query(query,1),
                            Utf8_Query(query,2),
                            Utf8_Query(query,3),
                            Utf8_Query(query,4),
                            Utf8_Query(query,5),
                            Utf8_Query(query,6),
                            Utf8_Query(query,7),
                            Utf8_Query(query,8),
                            Utf8_Query(query,9),
                            Utf8_Query(query,10),
                            Utf8_Query(query,11),
                            Utf8_Query(query,12),
                            Utf8_Query(query,13),
                            Utf8_Query(query,14),
                            Utf8_Query(query,15),
                            Utf8_Query(query,16),
                            Utf8_Query(query,17),
                            Utf8_Query(query,18),
                            Utf8_Query(query,19),
                            Utf8_Query(query,20),
                            query.value(21).toString(),         // pendant est un chiffre
                            query.value(22).toString(),         // secabilite
                            Utf8_Query(query,23)
                     );

    } //if (pSqlQuery && pSqlQuery->isActive() && pSqlQuery->next())
}

//----------------------------------------------------- Medica_PosologieMergePersoBase ---------------------------------------------------------------------------
long  CMedicaBase::Medica_PosologieMergePersoBase(int mode) // 0 -> on efface pas les poso du même CIP   1 -> on efface les poso du même CIP
{long              nb(0);
 QString cipLastErase("");
 int     doseUnitPk;
 int     factCorpUnitPk;
 int     equiCoeffUnitPk;
 QString    doseUnit(""), factCorpUnit(""), equiCoeffUnit(""),  medName("");
 QString      requete("SELECT ");
 if ( m_MedicaBase==0 )                                              return 0;
 if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)   return 0;
 //................. remplir les listes d'unités destination avec les valeurs dèjà existantes ................................
 //                  (l'ajout d'une nouvelle poso la complete au fur et à mesure)
 Medica_GetUnitesList(m_MEDICA_FORME_UP_TBL_NAME, m_FormeUPList, CMedicaBase::JustLibelle);  // clear() dans la fonction
 Medica_GetUnitesList(m_MEDICA_FORME_AD_TBL_NAME, m_FormeADList, CMedicaBase::JustLibelle);
 //............... Selectionner toutes les posologies perso  .....................................................
 requete +=  m_MEDICA_POSO_CIP             + "," +     // 0
             m_MEDICA_POSO_PK              + "," +     // 1
             m_MEDICA_POSO_LIBELLE         + "," +     // 2
             m_MEDICA_POSO_AGE_MIN         + "," +     // 3
             m_MEDICA_POSO_AGE_MAX         + "," +     // 4
             m_MEDICA_POSO_SEXE            + "," +     // 5
             m_MEDICA_POSO_TERRAIN         + "," +     // 6
             m_MEDICA_POSO_POIDS_MIN       + "," +     // 7
             m_MEDICA_POSO_POIDS_MAX       + "," +     // 8
             m_MEDICA_POSO_DOSE_MIN        + "," +     // 9
             m_MEDICA_POSO_DOSE_MAX        + "," +     // 10
             m_MEDICA_POSO_DOSE_UNIT       + "," +     // 11
             m_MEDICA_POSO_DOSE_LIMIT_MAX  + "," +     // 12
             m_MEDICA_POSO_FACT_CORP_QU    + "," +     // 13
             m_MEDICA_POSO_FACT_CORP_UNIT  + "," +     // 14
             m_MEDICA_POSO_PERIODE         + "," +     // 15
             m_MEDICA_POSO_EQUI_COEFF      + "," +     // 16
             m_MEDICA_POSO_UNIT_COEFF      + "," +     // 17
             m_MEDICA_POSO_NB_PRISES       + "," +     // 18
             m_MEDICA_POSO_REPART_PRISES   + "," +     // 19
             m_MEDICA_POSO_DIVERS          + "," +     // 20
             m_MEDICA_POSO_PENDANT         + "," +     // 21
             m_MEDICA_POSO_SECABILITE      + "," +     // 22
             m_MEDICA_POSO_EXTRA_PK        + " ";      // 23
 requete += " FROM " + m_MEDICA_POSO_PERSO_TBL_NAME ;

 QSqlQuery query(requete , m_MedicaBase );
 if (query.isActive())
    {CPosologie posoSrc;
     while (query.next())
          {//......................... recuperer poso de la table perso .............................
           posoSrc.setData    (Utf8_Query(query,0),
                               Utf8_Query(query,1),
                               Utf8_Query(query,2),
                               Utf8_Query(query,3),
                               Utf8_Query(query,4),
                               Utf8_Query(query,5),
                               Utf8_Query(query,6),
                               Utf8_Query(query,7),
                               Utf8_Query(query,8),
                               Utf8_Query(query,9),
                               Utf8_Query(query,10),
                               Utf8_Query(query,11),
                               Utf8_Query(query,12),
                               Utf8_Query(query,13),
                               Utf8_Query(query,14),
                               Utf8_Query(query,15),
                               Utf8_Query(query,16),
                               Utf8_Query(query,17),
                               Utf8_Query(query,18),
                               Utf8_Query(query,19),
                               Utf8_Query(query,20),
                               query.value(21).toString(),         // pendant est un chiffre
                               query.value(22).toString(),         // secabilite est un chiffre
                               Utf8_Query(query,23)
                             );
         medName= Medica_GetMedicamentNameByCIP(posoSrc.m_MEDICA_POSO_CIP );
         if (medName.length() && posoSrc.m_MEDICA_POSO_CIP.length())
           {//......................... si besoin effacer toutes les poso ......................................
            //                       ayant le memme CIP de la table de destination
            if (mode && cipLastErase != posoSrc.m_MEDICA_POSO_CIP)
               {Medica_PosologieDeleteToBaseByCIP( posoSrc.m_MEDICA_POSO_CIP, m_MEDICA_POSO_TBL_NAME );
                cipLastErase  =  posoSrc.m_MEDICA_POSO_CIP;
               }

            //......................correler la poso à ajouter à celle perso ....................................
            posoSrc.m_MEDICA_POSO_EXTRA_PK = posoSrc.m_MEDICA_POSO_PK ;
            //...................... reindexer les pointeurs d'unite de la table perso .............................................
            //                       dans ceux de le table de destination (les rajouter si-besoin)
            doseUnit      = Medica_GetUnite(m_MEDICA_FORME_AD_PERSO_TBL_NAME, posoSrc.m_MEDICA_POSO_DOSE_UNIT);             // recuperer données textuelles
            factCorpUnit  = Medica_GetUnite(m_MEDICA_FORME_AD_PERSO_TBL_NAME, posoSrc.m_MEDICA_POSO_FACT_CORP_UNIT);
            equiCoeffUnit = Medica_GetUnite(m_MEDICA_FORME_UP_PERSO_TBL_NAME, posoSrc.m_MEDICA_POSO_UNIT_COEFF);

            Medica_IsThisFormeExist(m_FormeADList, doseUnit,      doseUnitPk);       // donnée texte vers primKey de la base des unités Base Destination
            Medica_IsThisFormeExist(m_FormeADList, factCorpUnit,  factCorpUnitPk);
            Medica_IsThisFormeExist(m_FormeUPList, equiCoeffUnit, equiCoeffUnitPk);
            posoSrc.m_MEDICA_POSO_DOSE_UNIT       = QString::number(doseUnitPk);                      // replacer les Pk corrects
            posoSrc.m_MEDICA_POSO_FACT_CORP_UNIT  = QString::number(factCorpUnitPk);
            posoSrc.m_MEDICA_POSO_UNIT_COEFF      = QString::number(equiCoeffUnitPk);
            //......................... ajouter poso ......................................
            Medica_PosologieAppendToBase( posoSrc, m_MEDICA_POSO_TBL_NAME );
            ++nb;
           }
        }
    } //if (pSqlQuery && pSqlQuery->isActive() && pSqlQuery->next())
 Medica_CreateTableFormeProduit(m_MEDICA_FORME_UP_TBL_NAME, m_FormeUPList);
 Medica_CreateTableFormeProduit(m_MEDICA_FORME_AD_TBL_NAME, m_FormeADList);
 return nb;
}

//----------------------------------------------------- Medica_GetPosologies ---------------------------------------------------------------------------
long  CMedicaBase::Medica_GetPosologies(QString cip, QPL_CPosologie &list_CPosologie )
{long              nb(0);

 QString      requete("SELECT ");
 if ( m_MedicaBase==0 )                                              return 0;
 if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)   return 0;

 //............... requete utilisant la base virtuelle .....................................................
 requete +=  m_MEDICA_POSO_CIP             + "," +     // 0
             m_MEDICA_POSO_PK              + "," +     // 1
             m_MEDICA_POSO_LIBELLE         + "," +     // 2
             m_MEDICA_POSO_AGE_MIN         + "," +     // 3
             m_MEDICA_POSO_AGE_MAX         + "," +     // 4
             m_MEDICA_POSO_SEXE            + "," +     // 5
             m_MEDICA_POSO_TERRAIN         + "," +     // 6
             m_MEDICA_POSO_POIDS_MIN       + "," +     // 7
             m_MEDICA_POSO_POIDS_MAX       + "," +     // 8
             m_MEDICA_POSO_DOSE_MIN        + "," +     // 9
             m_MEDICA_POSO_DOSE_MAX        + "," +     // 10
             m_MEDICA_POSO_DOSE_UNIT       + "," +     // 11
             m_MEDICA_POSO_DOSE_LIMIT_MAX  + "," +     // 12
             m_MEDICA_POSO_FACT_CORP_QU    + "," +     // 13
             m_MEDICA_POSO_FACT_CORP_UNIT  + "," +     // 14
             m_MEDICA_POSO_PERIODE         + "," +     // 15
             m_MEDICA_POSO_EQUI_COEFF      + "," +     // 16
             m_MEDICA_POSO_UNIT_COEFF      + "," +     // 17
             m_MEDICA_POSO_NB_PRISES       + "," +     // 18
             m_MEDICA_POSO_REPART_PRISES   + "," +     // 19
             m_MEDICA_POSO_DIVERS          + "," +     // 20
             m_MEDICA_POSO_PENDANT         + "," +     // 21
             m_MEDICA_POSO_SECABILITE      + "," +     // 22
             m_MEDICA_POSO_EXTRA_PK        + " ";      // 23
 requete += " FROM " + m_MEDICA_POSO_TBL_NAME + " WHERE ";
 requete +=  m_MEDICA_POSO_CIP     + "='"  + cip   + "'";

 QSqlQuery query(requete , m_MedicaBase );
 if (query.isActive())
    {list_CPosologie.clear();
     while (query.next())
          {
           list_CPosologie.append(CPosologie(Utf8_Query(query,0),
                                             Utf8_Query(query,1),
                                             Utf8_Query(query,2),
                                             Utf8_Query(query,3),
                                             Utf8_Query(query,4),
                                             Utf8_Query(query,5),
                                             Utf8_Query(query,6),
                                             Utf8_Query(query,7),
                                             Utf8_Query(query,8),
                                             Utf8_Query(query,9),
                                             Utf8_Query(query,10),
                                             Utf8_Query(query,11),
                                             Utf8_Query(query,12),
                                             Utf8_Query(query,13),
                                             Utf8_Query(query,14),
                                             Utf8_Query(query,15),
                                             Utf8_Query(query,16),
                                             Utf8_Query(query,17),
                                             Utf8_Query(query,18),
                                             Utf8_Query(query,19),
                                             Utf8_Query(query,20),
                                             query.value(21).toString(),         // pendant est un chiffre
                                             query.value(22).toString(),         // secabilite
                                             Utf8_Query(query,23)
                                           )
                                  );
         ++nb;
        }
    } //if (pSqlQuery && pSqlQuery->isActive() && pSqlQuery->next())
 return nb;
}

//----------------------------------------------------- Medica_PosologieUpdateToBase ---------------------------------------------------------------------------
bool CMedicaBase::Medica_PosologieUpdateToBase(const CPosologie &poso, const QString &tableName )
{ return     Medica_PosologieUpdateToBase  (poso.m_MEDICA_POSO_PK, //const QString &pk,
                                            poso.m_MEDICA_POSO_LIBELLE, //const QString &libelle,
                                            poso.m_MEDICA_POSO_CIP, //const QString &cip,
                                            poso.m_MEDICA_POSO_AGE_MIN ,//const QString &ageMin,
                                            poso.m_MEDICA_POSO_AGE_MAX ,//const QString &ageMax,
                                            poso.m_MEDICA_POSO_SEXE ,//const QString &sexe,
                                            poso.m_MEDICA_POSO_TERRAIN ,//const QString &terrain,
                                            poso.m_MEDICA_POSO_POIDS_MIN ,//const QString &poidsMin,
                                            poso.m_MEDICA_POSO_POIDS_MAX ,//const QString &poidsMax,
                                            poso.m_MEDICA_POSO_DOSE_MIN ,//const QString &doseMin,
                                            poso.m_MEDICA_POSO_DOSE_MAX ,//const QString &doseMax,
                                            poso.m_MEDICA_POSO_DOSE_UNIT ,//const QString &doseUnitPk,
                                            poso.m_MEDICA_POSO_DOSE_LIMIT_MAX, //const QString &doseLimitMax,
                                            poso.m_MEDICA_POSO_FACT_CORP_QU ,//const QString &factCorpQu,
                                            poso.m_MEDICA_POSO_FACT_CORP_UNIT ,//const QString &factCorpUnitPk,
                                            poso.m_MEDICA_POSO_PERIODE ,//const QString &periode,
                                            poso.m_MEDICA_POSO_EQUI_COEFF ,//const QString &equiCoeff,
                                            poso.m_MEDICA_POSO_UNIT_COEFF ,//const QString &equiCoeffUnitPk,
                                            poso.m_MEDICA_POSO_NB_PRISES ,//const QString &nbPrises,
                                            poso.m_MEDICA_POSO_REPART_PRISES ,//const QString &repart,
                                            poso.m_MEDICA_POSO_DIVERS ,//const QString &divers,
                                            poso.m_MEDICA_POSO_PENDANT ,//const QString &pendant,
                                            poso.m_MEDICA_POSO_SECABILITE ,//const QString &secabilite,
                                            poso.m_MEDICA_POSO_EXTRA_PK ,//const QString &extraPk,
                                            tableName
                                          );
}
//----------------------------------------------------- Medica_PosologieUpdateToBase ---------------------------------------------------------------------------

bool CMedicaBase::Medica_PosologieUpdateToBase(   const QString &primKey,
                                                  const QString &libelle,
                                                  const QString &cip,
                                                  const QString &ageMin,
                                                  const QString &ageMax,
                                                  const QString &sexe,
                                                  const QString &terrain,
                                                  const QString &poidsMin,
                                                  const QString &poidsMax,
                                                  const QString &doseMin,
                                                  const QString &doseMax,
                                                  const QString &doseUnitPk,
                                                  const QString &doseLimitMax,
                                                  const QString &factCorpQu,
                                                  const QString &factCorpUnitPk,
                                                  const QString &periode,
                                                  const QString &equiCoeff,
                                                  const QString &equiCoeffUnitPk,
                                                  const QString &nbPrises,
                                                  const QString &repart,
                                                  const QString &divers,
                                                  const QString &pendant,
                                                  const QString &secabilite,
                                                  const QString &extraPk,
                                                  const QString &tableName
                                                 )
{

 //................................. methode QSqlCursor .................................
 //                         ne pose pas de PB avec valeur avec une apostrophe
 //............... crer un curseur SQL ..................................................
 if ( m_MedicaBase==0 )                                              return false;
 if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)   return FALSE;
 bool ok = FALSE;
 QSqlCursor cur(tableName, TRUE, m_MedicaBase);
 if ( !cur.canUpdate ())                                             return FALSE;
 //............... placer ce curseur sur le bon enregistrement ..........................
 cur.select(m_MEDICA_POSO_PK  + "='" + primKey  + "'" );
  //............... si OK on recupere le buffer et on le charge avec les données .........
 if ( cur.next() )
    {QSqlRecord *buffer = cur.primeUpdate();                             // recuperer le buffer ( pas de delete à la fin !!)
     if (libelle.length())          buffer->setValue( m_MEDICA_POSO_LIBELLE,         libelle  );
     if (cip.length())              buffer->setValue( m_MEDICA_POSO_CIP,             cip                    );
     if (ageMin.length())           buffer->setValue( m_MEDICA_POSO_AGE_MIN,         ageMin.toInt()         );
     if (ageMax.length())           buffer->setValue( m_MEDICA_POSO_AGE_MAX,         ageMax.toInt()         );
     if (sexe.length())             buffer->setValue( m_MEDICA_POSO_SEXE,            sexe                   );
     if (terrain.length())          buffer->setValue( m_MEDICA_POSO_TERRAIN,         terrain                );
     if (poidsMin.length())         buffer->setValue( m_MEDICA_POSO_POIDS_MIN,       poidsMin.toFloat()     );
     if (poidsMax.length())         buffer->setValue( m_MEDICA_POSO_POIDS_MAX,       poidsMax.toFloat()     );
     if (doseMin.length())          buffer->setValue( m_MEDICA_POSO_DOSE_MIN,        doseMin.toFloat()      );
     if (doseMax.length())          buffer->setValue( m_MEDICA_POSO_DOSE_MAX,        doseMax.toFloat()      );
     if (doseUnitPk.length())       buffer->setValue( m_MEDICA_POSO_DOSE_UNIT,       doseUnitPk             );
     if (doseLimitMax.length())     buffer->setValue( m_MEDICA_POSO_DOSE_LIMIT_MAX,  doseLimitMax.toFloat() );
     if (factCorpQu.length())       buffer->setValue( m_MEDICA_POSO_FACT_CORP_QU,    factCorpQu.toFloat()   );
     if (factCorpUnitPk.length())   buffer->setValue( m_MEDICA_POSO_FACT_CORP_UNIT,  factCorpUnitPk         );
     if (periode.length())          buffer->setValue( m_MEDICA_POSO_PERIODE,         periode.toFloat()      );
     if (equiCoeff.length())        buffer->setValue( m_MEDICA_POSO_EQUI_COEFF,      equiCoeff.toFloat()    );
     if (equiCoeffUnitPk.length())  buffer->setValue( m_MEDICA_POSO_UNIT_COEFF,      equiCoeffUnitPk        );
     if (nbPrises.length())         buffer->setValue( m_MEDICA_POSO_NB_PRISES,       nbPrises.toInt()       );
     if (secabilite.length())       buffer->setValue( m_MEDICA_POSO_SECABILITE,      secabilite             );
     if (pendant.length())          buffer->setValue( m_MEDICA_POSO_PENDANT,         pendant.toFloat()      );
     if (repart.length())           buffer->setValue( m_MEDICA_POSO_REPART_PRISES,   repart                 );
     if (divers.length())           buffer->setValue( m_MEDICA_POSO_DIVERS,          divers                 );
     if (extraPk.length())          buffer->setValue( m_MEDICA_POSO_EXTRA_PK,        extraPk                );
     ok    = cur.update();                                               // ecrire le buffer avec les données du header
    }
 if (ok==0) OutSQL_error(cur, "Medica_PosologieUpdateToBase()",0);
 return ok;
}

//----------------------------------------------------- Medica_PosologieAppendToBase ---------------------------------------------------------------------------

bool CMedicaBase::Medica_PosologieAppendToBase(const CPosologie &poso, const QString &tableName )
{ return      Medica_PosologieAppendToBase( poso.m_MEDICA_POSO_LIBELLE, //const QString &libelle,              // 0
                                            poso.m_MEDICA_POSO_CIP, //const QString &cip,
                                            poso.m_MEDICA_POSO_AGE_MIN ,//const QString &ageMin,
                                            poso.m_MEDICA_POSO_AGE_MAX ,//const QString &ageMax,
                                            poso.m_MEDICA_POSO_SEXE ,//const QString &sexe,
                                            poso.m_MEDICA_POSO_TERRAIN ,//const QString &terrain,
                                            poso.m_MEDICA_POSO_POIDS_MIN ,//const QString &poidsMin,
                                            poso.m_MEDICA_POSO_POIDS_MAX ,//const QString &poidsMax,
                                            poso.m_MEDICA_POSO_DOSE_MIN ,//const QString &doseMin,
                                            poso.m_MEDICA_POSO_DOSE_MAX ,//const QString &doseMax,
                                            poso.m_MEDICA_POSO_DOSE_UNIT ,//const QString &doseUnitPk,
                                            poso.m_MEDICA_POSO_DOSE_LIMIT_MAX, //const QString &doseLimitMax,
                                            poso.m_MEDICA_POSO_FACT_CORP_QU ,//const QString &factCorpQu,
                                            poso.m_MEDICA_POSO_FACT_CORP_UNIT ,//const QString &factCorpUnitPk,
                                            poso.m_MEDICA_POSO_PERIODE ,//const QString &periode,
                                            poso.m_MEDICA_POSO_EQUI_COEFF ,//const QString &equiCoeff,
                                            poso.m_MEDICA_POSO_UNIT_COEFF ,//const QString &equiCoeffUnitPk,
                                            poso.m_MEDICA_POSO_NB_PRISES ,//const QString &nbPrises,
                                            poso.m_MEDICA_POSO_REPART_PRISES ,//const QString &repart,
                                            poso.m_MEDICA_POSO_DIVERS ,//const QString &divers,
                                            poso.m_MEDICA_POSO_PENDANT ,//const QString &pendnat,
                                            poso.m_MEDICA_POSO_SECABILITE ,//const QString &secabilite,
                                            poso.m_MEDICA_POSO_EXTRA_PK ,//const QString &extraPk,
                                            tableName
                                          );
}

//----------------------------------------------------- Medica_PosologieAppendToBase ---------------------------------------------------------------------------

bool CMedicaBase::Medica_PosologieAppendToBase(const QString &libelle,
                                               const QString &cip,
                                               const QString &ageMin,
                                               const QString &ageMax,
                                               const QString &sexe,
                                               const QString &terrain,
                                               const QString &poidsMin,
                                               const QString &poidsMax,
                                               const QString &doseMin,
                                               const QString &doseMax,
                                               const QString &doseUnitPk,
                                               const QString &doseLimitMax,
                                               const QString &factCorpQu,
                                               const QString &factCorpUnitPk,
                                               const QString &periode,
                                               const QString &equiCoeff,
                                               const QString &equiCoeffUnitPk,
                                               const QString &nbPrises,
                                               const QString &repart,
                                               const QString &divers,
                                               const QString &pendant,
                                               const QString &secabilite,
                                               const QString &extraPk,
                                               const QString &tableName
                                             )
{        if ( m_MedicaBase==0 )                                              return false;
         if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)   return FALSE;
         QSqlCursor cur( tableName, TRUE, m_MedicaBase );
         if ( cur.canInsert ())
            { //............... si OK on recupere le buffer et on le charge avec les données .........
             /*
               requeteDst += "    `" +            m_MEDICA_POSO_PK             + "`       int(11)    NOT NULL auto_increment, "
               "    `" +            m_MEDICA_POSO_EXTRA_PK       + "`       int(11)    default NULL, "
               "    `" +            m_MEDICA_POSO_CIP            + "`       varchar(7) BINARY NOT NULL, "
               "    `" +            m_MEDICA_POSO_LIBELLE        + "`       char(128)  BINARY default NULL , "
               "    `" +            m_MEDICA_POSO_AGE_MIN        + "`       int(11)    default NULL , "
               "    `" +            m_MEDICA_POSO_AGE_MAX        + "`       int(11)    default NULL , "
               "    `" +            m_MEDICA_POSO_SEXE           + "`       char(1)    BINARY default NULL, "
               "    `" +            m_MEDICA_POSO_TERRAIN        + "`       char(64)   BINARY default NULL, "
               "    `" +            m_MEDICA_POSO_POIDS_MIN      + "`       float      default NULL, "
               "    `" +            m_MEDICA_POSO_POIDS_MAX      + "`       float      default NULL, "
               "    `" +            m_MEDICA_POSO_DOSE_MIN       + "`       float      default NULL, "
               "    `" +            m_MEDICA_POSO_DOSE_MAX       + "`       float      default NULL, "
               "    `" +            m_MEDICA_POSO_DOSE_UNIT      + "`       int(11)    default NULL, "
               "    `" +            m_MEDICA_POSO_DOSE_LIMIT_MAX + "`       float      default NULL, "
               "    `" +            m_MEDICA_POSO_FACT_CORP_QU   + "`       float      default NULL, "
               "    `" +            m_MEDICA_POSO_FACT_CORP_UNIT + "`       int(11)    default NULL, "
               "    `" +            m_MEDICA_POSO_PERIODE        + "`       float      default NULL, "
               "    `" +            m_MEDICA_POSO_EQUI_COEFF     + "`       float      default NULL, "
               "    `" +            m_MEDICA_POSO_UNIT_COEFF     + "`       int(11)    default NULL, "
               "    `" +            m_MEDICA_POSO_NB_PRISES      + "`       int(11)    default NULL, "
               "    `" +            m_MEDICA_POSO_REPART_PRISES  + "`       char(128)  default NULL, "
               "    `" +            m_MEDICA_POSO_DIVERS         + "`       char(32)   default NULL, "
               "     PRIMARY KEY  ("+ m_MEDICA_POSO_PK + ") "
               "  );";*/
             QSqlRecord *buffer = cur.primeInsert();                         // recuperer le buffer d'insertion
             if (libelle.length())        buffer->setValue( m_MEDICA_POSO_LIBELLE,         libelle                );
             if (cip.length())            buffer->setValue( m_MEDICA_POSO_CIP,             cip                    );
             if (ageMin.length())         buffer->setValue( m_MEDICA_POSO_AGE_MIN,         ageMin.toInt()         );
             if (ageMax.length())         buffer->setValue( m_MEDICA_POSO_AGE_MAX,         ageMax.toInt()         );
             if (sexe.length())           buffer->setValue( m_MEDICA_POSO_SEXE,            sexe                   );
             if (terrain.length())        buffer->setValue( m_MEDICA_POSO_TERRAIN,         terrain                );
             if (poidsMin.length())       buffer->setValue( m_MEDICA_POSO_POIDS_MIN,       poidsMin.toFloat()     );
             if (poidsMax.length())       buffer->setValue( m_MEDICA_POSO_POIDS_MAX,       poidsMax.toFloat()     );
             if (doseMin.length())        buffer->setValue( m_MEDICA_POSO_DOSE_MIN,        doseMin.toFloat()      );
             if (doseMax.length())        buffer->setValue( m_MEDICA_POSO_DOSE_MAX,        doseMax.toFloat()      );
             if (doseUnitPk.length())     buffer->setValue( m_MEDICA_POSO_DOSE_UNIT,       doseUnitPk.toInt()     );
             if (doseLimitMax.length())   buffer->setValue( m_MEDICA_POSO_DOSE_LIMIT_MAX,  doseLimitMax.toFloat() );
             if (factCorpQu.length())     buffer->setValue( m_MEDICA_POSO_FACT_CORP_QU,    factCorpQu.toFloat()   );
             if (factCorpUnitPk.length()) buffer->setValue( m_MEDICA_POSO_FACT_CORP_UNIT,  factCorpUnitPk.toInt() );
             if (periode.length())        buffer->setValue( m_MEDICA_POSO_PERIODE,         periode.toFloat()      );
             if (equiCoeff.length())      buffer->setValue( m_MEDICA_POSO_EQUI_COEFF,      equiCoeff.toFloat()    );
             if (equiCoeffUnitPk.length())buffer->setValue( m_MEDICA_POSO_UNIT_COEFF,      equiCoeffUnitPk.toInt());
             if (nbPrises.length())       buffer->setValue( m_MEDICA_POSO_NB_PRISES,       nbPrises.toInt()       );
             if (secabilite.length())     buffer->setValue( m_MEDICA_POSO_SECABILITE,      secabilite             );
             if (pendant.length())        buffer->setValue( m_MEDICA_POSO_PENDANT,         pendant.toFloat()      );
             if (repart.length())         buffer->setValue( m_MEDICA_POSO_REPART_PRISES,   repart                 );
             if (divers.length())         buffer->setValue( m_MEDICA_POSO_DIVERS,          divers                 );
             if (extraPk.length())        buffer->setValue( m_MEDICA_POSO_EXTRA_PK,        extraPk                );

             if ( cur.insert() )  return TRUE;
            }
         OutSQL_error(cur, "Medica_PosologieAppendToBase(): Error insert data : " + m_MEDICA_POSO_TBL_NAME, 0 );
         return FALSE;
}

//----------------------------------------------------- Medica_PosologieAppendToBase ---------------------------------------------------------------------------
// NOTE:  les valeurs suivantes : doseUnit , equiCoeffUnit , factCorpUnit   sont passées en clair et non sous forme de PK de leur tables
//
bool CMedicaBase::Medica_PosologieAppendToBase( const QString &libelle,       // 0
                                                const QString &cip,           // 1
                                                const QString &ageMin,        // 2
                                                const QString &ageMax,        // 3
                                                const QString &sexe,          // 4
                                                const QString &terrain,       // 5
                                                const QString &poidsMin,      // 6
                                                const QString &poidsMax,      // 7
                                                const QString &doseMin,       // 8
                                                const QString &doseMax,       // 9
                                                const QString &doseUnit,      // 10
                                                const QString &doseLimitMax,  // 11
                                                const QString &factCorpQu,    // 12
                                                const QString &factCorpUnit,  // 13
                                                const QString &periode,       // 14
                                                const QString &equiCoeff,     // 15
                                                const QString &equiCoeffUnit, // 16
                                                const QString &nbPrises,      // 17
                                                const QString &repart,        // 18
                                                const QString &divers,        // 19
                                                const QString &pendant,       // 20
                                                const QString &secabilite,    // 21
                                                const QString &extraPk        // 22
                                              )
{   int doseUnitPk;
    int factCorpUnitPk;
    int equiCoeffUnitPk;
    Medica_IsThisFormeExist(m_FormeADList, doseUnit,      doseUnitPk);       // convertir la donnée textuelle en primKey de la base des unités
    Medica_IsThisFormeExist(m_FormeADList, factCorpUnit,  factCorpUnitPk);
    Medica_IsThisFormeExist(m_FormeUPList, equiCoeffUnit, equiCoeffUnitPk);
    return Medica_PosologieAppendToBase(libelle,
                                        cip,
                                        ageMin,
                                        ageMax,
                                        sexe,
                                        terrain,
                                        poidsMin,
                                        poidsMax,
                                        doseMin,
                                        doseMax,
                                        QString::number(doseUnitPk),
                                        doseLimitMax,
                                        factCorpQu,
                                        QString::number(factCorpUnitPk),
                                        periode,
                                        equiCoeff,
                                        QString::number(equiCoeffUnitPk),
                                        nbPrises,
                                        repart,
                                        divers,
                                        pendant,
                                        secabilite,
                                        extraPk,
                                        m_MEDICA_POSO_TBL_NAME
                                       );
}


//----------------------------------------------------- Medica_InsertNewSpec ---------------------------------------------------------------------------
bool CMedicaBase::Medica_InsertNewSpec(          const QString &cip,
                                                 const QString &up_unit,
                                                 const QString &pa_unit,
                                                 const QString &pa_qu,
                                                 const QString &secabilite,
                                                 const QString &is_generic,
                                                 const QString &formStruct,
                                                 const QString &nbFormes,
                                                 const QString &prixBoite,
                                                 const QString &remboursementSS,
                                                 const QString &tableau,
                                                 const QString &labo,
                                                 const QString &atc
                                       )
{        int next;
         QSqlCursor cur( m_MEDICA_SPEC_TBL_NAME, TRUE, m_MedicaBase );
         if ( cur.canInsert ())
            { //............... si OK on recupere le buffer et on le charge avec les données .........
             QSqlRecord *buffer = cur.primeInsert();                         // recuperer le buffer d'insertion
             buffer->setValue( m_MEDICA_SPEC_CIP,            cip             );
             Medica_IsThisFormeExist(m_FormeUPList, up_unit, next);
             buffer->setValue( m_MEDICA_SPEC_UP_UNIT,        QString::number(next)            );

             if (pa_unit[0]>='0' && pa_unit[0]<='9' || pa_unit[0]==',')
                {int pos = 0;
                 while (pa_unit[pos]>='0' && pa_unit[pos]<='9' || pa_unit[pos]==',') ++pos;
                 Medica_IsThisFormeExist(m_FormePAList,   pa_unit.mid(pos), next);
                 buffer->setValue( m_MEDICA_SPEC_PA_UNIT, QString::number(next)         );
                 buffer->setValue( m_MEDICA_SPEC_PA_QU,   pa_unit.left(pos).replace(",",".").toFloat()   );
                }
             else
                {Medica_IsThisFormeExist(m_FormePAList, pa_unit, next);
                 buffer->setValue( m_MEDICA_SPEC_PA_UNIT,        QString::number(next)         );
                 buffer->setValue( m_MEDICA_SPEC_PA_QU,          pa_qu.toFloat()           );
                }
             buffer->setValue( m_MEDICA_SPEC_SECABILITE,     secabilite      );
             buffer->setValue( m_MEDICA_SPEC_IS_GENERIC,     is_generic      );
             Medica_IsThisFormeExist(m_FormeSTList, formStruct, next);
             buffer->setValue( m_MEDICA_SPEC_FORM_STRUC,     QString::number(next)      );
             buffer->setValue( m_MEDICA_SPEC_NB_FORME,       nbFormes        );
             buffer->setValue( m_MEDICA_SPEC_PRIX_BOITE,     prixBoite.toFloat()       );
             buffer->setValue( m_MEDICA_SPEC_REMBOURSEMENT,  remboursementSS );
             buffer->setValue( m_MEDICA_SPEC_TABLEAU,        tableau         );
             buffer->setValue( m_MEDICA_SPEC_LABO,           labo            );
             buffer->setValue( m_MEDICA_SPEC_ATC,            atc             );
             if ( cur.insert() )  return TRUE;
             else
                {/*
                 QString truc("\nCIP :  ");
                 truc +=        " up_unit: "           + up_unit +
                                " pa_qu: "             + pa_qu   +
                                " pa_unit: "           + pa_unit +
                                " secabilite: "        + secabilite +
                                " is_generic: "        + is_generic +
                                " formStruct: "        + formStruct +
                                " nbFormes: "          + nbFormes +
                                " prixBoite: "         + prixBoite +
                                " remboursementSS: "   + remboursementSS +
                                " tableau: "           + tableau +
                                " labo: "              + labo +
                                " atc: "               + atc ;
                 qDebug(truc);
                 OutSQL_error(cur, TR("Medica_InsertNewSpec(): Error insert data : ") + m_MEDICA_SPEC_TBL_NAME, 0 );
                 */
                }
            }
         return FALSE;
}

//-----------------------------------------------------  Medica_GetLibelleATC -------------------------------------------
QString CMedicaBase::Medica_GetLibelleATC( const QString &codeATC)
{ QString      requete("SELECT ");
  if ( m_MedicaBase==0 )                                              return "";
  if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)   return "";
       //................. chercher données economiques pour ce code CIP...................................................
       requete +=              m_MEDICA_ATC_LIBELLE    +
                   " FROM "  + m_MEDICA_ATC_TBL_NAME   + " WHERE " + m_MEDICA_ATC_CODE + " ='" + codeATC + "'";
  QSqlQuery query (requete , m_MedicaBase );
  if (query.isActive() && query.next()) return Utf8_Query(query,0);
  return QString("");
}


//----------------------------------------------------- Medica_FillQListView_ATC ---------------------------------------------------------------
void CMedicaBase::Medica_FillQListView_ATC(QListView *pQListView )
{ 

 if ((int)(Medica_GetBaseMode() & CMedicaBase::Datasemp))
    {Datasemp_FillQListView_ATC(pQListView );
    }
 else
    {
     QListViewItem *curParentItem = 0;
     long              curLen     = 0;
     QString      requete("SELECT ");
     if ( m_MedicaBase==0 )                                              return;
     if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)   return;
         requete +=             m_MEDICA_ATC_LIBELLE  + ","          +  m_MEDICA_ATC_CODE  +
                    " FROM "  + m_MEDICA_ATC_TBL_NAME + " ORDER BY " +  m_MEDICA_ATC_CODE;

     QSqlQuery query (requete , m_MedicaBase );
     if (query.isActive() )
        {pQListView->clear();
         while (query.next())
               {QString atc_libelle = Utf8_Query(query,0);
                QString   atc_code  = Utf8_Query(query,1);
                int            len  = atc_code.length();

                if (len==1)
                   { curParentItem = new QListViewItem (pQListView,    atc_libelle, atc_code);
                     curLen = len;
                   }
                else if (len > curLen)
                   { if (len != 7) curParentItem = new QListViewItem (curParentItem, atc_libelle, atc_code);
                     else                          new QListViewItem (curParentItem, atc_libelle, atc_code);
                     curLen = len;
                   }
                else if (len == curLen)
                   {  new QListViewItem (curParentItem, atc_libelle, atc_code);
                      curLen = len;
                   }
                else if (len < curLen)
                   { //......... remonter tant que le père n'a pas de partie comumune avec le fils ............
                     while( curParentItem->depth() != 0 &&  atc_code.left(curParentItem->text(1).length()) != curParentItem->text(1))
                          {curParentItem = curParentItem->parent();
                          }
                     curParentItem = new QListViewItem (curParentItem, atc_libelle, atc_code);
                     curLen = len;
                   }
               }    // end while (query.next())
        }  // endif (query.isActive() )
    } // if ((int)(Medica_GetBaseMode() & CMedicaBase::Datasemp))
}

//----------------------------------------------------- Medica_SetQListViewOnATC ---------------------------------------------------------------
QListViewItem *CMedicaBase::Medica_SetQListViewOnATC(QListView *pQListView , const QString  &atc_code, int ListMustBeClosed /* = 1 */)
{if (ListMustBeClosed)
    {QListViewItemIterator it( pQListView );
     while ( it.current() )
        {QListViewItem *item = it.current();
         ++it;
         if ( item->isOpen())  item->setOpen (FALSE );
        }
    }
  QString atc_string(TR(" DONNÉES SUR LA CLASSE ATC NON DISPONIBLES "));
  QListViewItem *pQListViewItemOri = pQListView->findItem(atc_code, 1);
  if (pQListViewItemOri==0)  return 0;

  QListViewItem *pQListViewItem = pQListViewItemOri->parent();
  while (pQListViewItem!=0)
        {pQListView->setOpen (pQListViewItem, TRUE );
         atc_string.prepend(pQListViewItem->text(0));
         if (pQListViewItem->depth()==0) break;
         pQListViewItem = pQListViewItem->parent();
        }
  pQListView->ensureItemVisible (pQListViewItemOri);
  pQListView->setSelected(pQListViewItemOri, TRUE);
  return pQListViewItemOri;
}

//----------------------------------------------------- Medica_InsertNewProduitInIndex ---------------------------------------------------------------------------
bool CMedicaBase::Medica_InsertNewProduitInIndex(const QString &libelle,
                                                 const QString &cip,
                                                 const QString &pkt00,
                                                 const QString &pkt4b,
                                                 const QString &code_prod,
                                                 const QString &code_UV,
                                                 const QString &isGene,
                                                 const QString &isDispo)
{        QSqlCursor cur( m_MEDICA_INDEX_MED_TBL_NAME, TRUE, m_MedicaBase );
         if ( cur.canInsert ())
            { //............... si OK on recupere le buffer et on le charge avec les données .........
             QSqlRecord *buffer = cur.primeInsert();                         // recuperer le buffer d'insertion
                                     buffer->setValue( m_MEDICA_INDEX_LIBELLE,      libelle    );
                                     buffer->setValue( m_MEDICA_INDEX_CIP,          cip        );
             if (pkt00.length())     buffer->setValue( m_MEDICA_INDEX_PK_T00,       pkt00      );
             if (pkt4b.length())     buffer->setValue( m_MEDICA_INDEX_PK_T4B,       pkt4b      );
             if (code_prod.length()) buffer->setValue( m_MEDICA_INDEX_CD_PROD,      code_prod  );
             if (code_UV.length())   buffer->setValue( m_MEDICA_INDEX_CD_UV,        code_UV    );
             if (isGene .length())   buffer->setValue( m_MEDICA_INDEX_IS_GENE,      isGene     );
             if (isDispo.length())   buffer->setValue( m_MEDICA_INDEX_IS_DISPO,     isDispo    );
             if ( cur.insert() )  return TRUE;
            }

         OutSQL_error(cur, "INSERT ERROR: Medica_InsertNewProduitInIndex" ,0);
         return FALSE;
}

//----------------------------------------------------- Medica_GetMedicamentListByATC ---------------------------------------------------------------------------
long CMedicaBase::Medica_GetMedicamentListByATC(      QListView *pQlistView ,
                                                      const QString   &pATC ,
                                                      const QString &name   ,
                                                      const QString &isGene ,      // = ""
                                                      const QString &isDispo       // = ""

                                               )
{ if ( m_MedicaBase==0 )                                                 return 0;
  if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)      return 0;

  //................. Preparer la requete .....................................
  // SELECT IndexProd.LibelleMed FROM `IndexProd` inner join `SpecProd` on IndexProd.CodeCIP = SpecProd.CodeCIP where  SpecProd.atc='D10AF02'
  QString requete("SELECT ");
  requete  += m_MEDICA_INDEX_MED_TBL_NAME+"."+m_MEDICA_INDEX_LIBELLE  + "," +
              m_MEDICA_INDEX_MED_TBL_NAME+"."+m_MEDICA_INDEX_CD_PROD  + "," +
              m_MEDICA_INDEX_MED_TBL_NAME+"."+m_MEDICA_INDEX_PK_T00   + "," +
              m_MEDICA_INDEX_MED_TBL_NAME+"."+m_MEDICA_INDEX_PK_T4B   + "," +
              m_MEDICA_INDEX_MED_TBL_NAME+"."+m_MEDICA_INDEX_CIP      + "," +
              m_MEDICA_INDEX_MED_TBL_NAME+"."+m_MEDICA_INDEX_CD_UV    + "," +
              m_MEDICA_INDEX_MED_TBL_NAME+"."+m_MEDICA_INDEX_IS_GENE  + "," +            // 6
              m_MEDICA_INDEX_MED_TBL_NAME+"."+m_MEDICA_INDEX_IS_DISPO + " " +            // 7

              " FROM "  + m_MEDICA_INDEX_MED_TBL_NAME + " INNER JOIN "   +  m_MEDICA_SPEC_TBL_NAME +
              " ON "    + m_MEDICA_INDEX_MED_TBL_NAME+"."+m_MEDICA_INDEX_CIP  + " = " + m_MEDICA_SPEC_TBL_NAME+"."+m_MEDICA_SPEC_CIP +
              " WHERE " + m_MEDICA_SPEC_TBL_NAME+"."+m_MEDICA_SPEC_ATC + " LIKE '" + pATC + "%'";
              " AND ("  + m_MEDICA_INDEX_MED_TBL_NAME+"."+m_MEDICA_INDEX_LIBELLE + " LIKE \"" + name          + "%\" OR "
                        + m_MEDICA_INDEX_MED_TBL_NAME+"."+m_MEDICA_INDEX_LIBELLE + " LIKE \"" + name.upper()  + "%\" );";
  if (isGene !="" && isDispo != "")
     {requete += "  AND "  + m_MEDICA_INDEX_MED_TBL_NAME+"."+m_MEDICA_INDEX_IS_DISPO + "='" + isDispo +
                 "' AND (" + m_MEDICA_INDEX_MED_TBL_NAME+"."+m_MEDICA_INDEX_IS_GENE  + "='G' OR "+  m_MEDICA_INDEX_MED_TBL_NAME+"."+m_MEDICA_INDEX_IS_GENE + "='R')";
     }
  else  if (isGene !="")
     {requete += " AND (" + m_MEDICA_INDEX_MED_TBL_NAME+"."+m_MEDICA_INDEX_IS_GENE  + "='G' OR "+  m_MEDICA_INDEX_MED_TBL_NAME+"."+m_MEDICA_INDEX_IS_GENE + "='R')";
     }
  else  if (isDispo !="")
     {requete += " AND " +m_MEDICA_INDEX_MED_TBL_NAME+"."+ m_MEDICA_INDEX_IS_DISPO + "='" + isDispo + "' ";
     }
  return Medica_GetProductsListFrom_SQL_String(pQlistView, requete );
}

//----------------------------------------------------- Medica_GetMedicamentListByCIP ---------------------------------------------------------------------------
long CMedicaBase::Medica_GetMedicamentListByCIP( QListView *pQlistView , const int  prodFamily, const QString &cip)
{QString table("");
 switch(prodFamily)
    { default:
      case 1: table = m_MEDICA_INDEX_MED_TBL_NAME;   break;
      case 2: table = m_MEDICA_INDEX_DIET_TBL_NAME;  break;
      case 3: table = m_MEDICA_INDEX_VETO_TBL_NAME;  break;
      case 4: table = m_MEDICA_INDEX_PARA_TBL_NAME;  break;
      case 5: table = m_MEDICA_INDEX_ACCS_TBL_NAME;  break;
      case 6: table = m_MEDICA_INDEX_HERBO_TBL_NAME; break;
      case 7: table = m_MEDICA_INDEX_HOMEO_TBL_NAME; break;
    }
 QString    requete  = "";
 //................. Preparer la requete .....................................
 requete       +=      "SELECT " + m_MEDICA_INDEX_LIBELLE  + "," +            // 0
                                   m_MEDICA_INDEX_CD_PROD  + "," +            // 1
                                   m_MEDICA_INDEX_PK_T00   + "," +            // 2
                                   m_MEDICA_INDEX_PK_T4B   + "," +            // 3
                                   m_MEDICA_INDEX_CIP      + "," +            // 4
                                   m_MEDICA_INDEX_CD_UV    + "," +            // 5
                                   m_MEDICA_INDEX_IS_GENE  + "," +            // 6
                                   m_MEDICA_INDEX_IS_DISPO + " " +            // 7
                                   " FROM " + table + " WHERE  " ;
  requete       += m_MEDICA_INDEX_CIP + " LIKE \"" + cip          + "%\" ";
 return Medica_GetProductsListFrom_SQL_String(pQlistView, requete );
}

//----------------------------------------------------- Medica_GetMedicamentList ---------------------------------------------------------------------------
long CMedicaBase::Medica_GetMedicamentList( QListView *pQlistView ,
                                            const QString &name,
                                            const QString &isGene,
                                            const QString &isDispo)
{
 QString    requete  = "";
 //................. Preparer la requete .....................................
 requete       +=      "SELECT " + m_MEDICA_INDEX_LIBELLE  + "," +            // 0
                                   m_MEDICA_INDEX_CD_PROD  + "," +            // 1
                                   m_MEDICA_INDEX_PK_T00   + "," +            // 2
                                   m_MEDICA_INDEX_PK_T4B   + "," +            // 3
                                   m_MEDICA_INDEX_CIP      + "," +            // 4
                                   m_MEDICA_INDEX_CD_UV    + "," +            // 5
                                   m_MEDICA_INDEX_IS_GENE  + "," +            // 6
                                   m_MEDICA_INDEX_IS_DISPO + " " +            // 7
                                   " FROM " + m_MEDICA_INDEX_MED_TBL_NAME + " WHERE ( " ;
  requete       += m_MEDICA_INDEX_LIBELLE + " LIKE \"" + name          + "%\" OR ";
  requete       += m_MEDICA_INDEX_LIBELLE + " LIKE \"" + name.upper()  + "%\" )  ";
  if (isGene !="" && isDispo != "")
     {requete += " AND " + m_MEDICA_INDEX_IS_DISPO + "='" + isDispo + "' AND (" +  m_MEDICA_INDEX_IS_GENE + "='G' OR "+  m_MEDICA_INDEX_IS_GENE + "='R')";
     }
  else  if (isGene !="")
     {requete += " AND (" + m_MEDICA_INDEX_IS_GENE  + "='G' OR "+  m_MEDICA_INDEX_IS_GENE + "='R' )";
     }
  else  if (isDispo !="")
     {requete += " AND " + m_MEDICA_INDEX_IS_DISPO + "='" + isDispo + "' ";
     }

 return Medica_GetProductsListFrom_SQL_String(pQlistView, requete );
}

//----------------------------------------------------- Medica_GetMedicamentNameByCIP ---------------------------------------------------------------------------
QString CMedicaBase::Medica_GetMedicamentNameByCIP( const QString &cip, int prodFamily /* = 1 */)
{QString table("");
 switch(prodFamily)
    { default:
      case 1: table = m_MEDICA_INDEX_MED_TBL_NAME;   break;
      case 2: table = m_MEDICA_INDEX_DIET_TBL_NAME;  break;
      case 3: table = m_MEDICA_INDEX_VETO_TBL_NAME;  break;
      case 4: table = m_MEDICA_INDEX_PARA_TBL_NAME;  break;
      case 5: table = m_MEDICA_INDEX_ACCS_TBL_NAME;  break;
      case 6: table = m_MEDICA_INDEX_HERBO_TBL_NAME; break;
      case 7: table = m_MEDICA_INDEX_HOMEO_TBL_NAME; break;
    }
  if ( m_MedicaBase==0 )                                                       return QString::null;
  if ( m_MedicaBase->isOpen() == FALSE && m_MedicaBase->open() == FALSE )      return QString::null;
  QString    requete("SELECT ");
  //................. Preparer la requete .....................................
             requete  +=  m_MEDICA_INDEX_LIBELLE      + " FROM  " +
                          table                       + " WHERE " +
                          m_MEDICA_INDEX_CIP          + " = '"    + cip + "'";
  QSqlQuery query(requete , m_MedicaBase );
  //.................. si la requète a un resultat ..............................................
  if (query.isActive() && query.next())                                        return Utf8_Query(query,0);
  return QString::null;
}

//----------------------------------------------------- Medica_GetMedicamentDatasempTablesCodesByCIP ---------------------------------------------------------------------------
int CMedicaBase::Medica_GetMedicamentDatasempTablesCodesByCIP( const QString &cip, QStringList &codesList, int prodFamily /* = 1 */)
{QString table("");
 switch(prodFamily)
    { default:
      case 1: table = m_MEDICA_INDEX_MED_TBL_NAME;   break;
      case 2: table = m_MEDICA_INDEX_DIET_TBL_NAME;  break;
      case 3: table = m_MEDICA_INDEX_VETO_TBL_NAME;  break;
      case 4: table = m_MEDICA_INDEX_PARA_TBL_NAME;  break;
      case 5: table = m_MEDICA_INDEX_ACCS_TBL_NAME;  break;
      case 6: table = m_MEDICA_INDEX_HERBO_TBL_NAME; break;
      case 7: table = m_MEDICA_INDEX_HOMEO_TBL_NAME; break;
    }
  if ( m_MedicaBase==0 )                                                       return 0;
  if ( m_MedicaBase->isOpen() == FALSE && m_MedicaBase->open() == FALSE )      return 0;
  QString    requete("SELECT ");
  //................. Preparer la requete .....................................
             requete  +=  m_MEDICA_INDEX_LIBELLE  + "," +            // 0
                          m_MEDICA_INDEX_CD_PROD  + "," +            // 1
                          m_MEDICA_INDEX_PK_T00   + "," +            // 2
                          m_MEDICA_INDEX_PK_T4B   + "," +            // 3
                          m_MEDICA_INDEX_CIP      + "," +            // 4
                          m_MEDICA_INDEX_CD_UV    + "," +            // 5
                          m_MEDICA_INDEX_IS_GENE  + "," +            // 6
                          m_MEDICA_INDEX_IS_DISPO + " " +            // 7
                                                        " FROM  " +
                          table                       + " WHERE " +
                          m_MEDICA_INDEX_CIP          + " = '"    + cip + "'";
  QSqlQuery query(requete , m_MedicaBase );
  //.................. si la requète a un resultat ..............................................
  if (query.isActive() && query.next())
     {codesList.clear();
      codesList.append(Utf8_Query(query,0));
      codesList.append(Utf8_Query(query,1));
      codesList.append(Utf8_Query(query,2));
      codesList.append(Utf8_Query(query,3));
      codesList.append(Utf8_Query(query,4));
      codesList.append(Utf8_Query(query,5));
      codesList.append(Utf8_Query(query,6));
      codesList.append(Utf8_Query(query,7));
      return 1;
     }
  else return 0;
}

//----------------------------------------------------- Medica_GetMedicamentListByFamily ---------------------------------------------------------------------------
// Le premier caractère représente la "section" : 1 = Spécialités ;
//                                                2 = Diététiques ;
//                                                3 = Vétérinaires ;
//                                                4 = Parapharmacie ;
//                                                5 = Accessoires ;
//                                                6 = Divers (herboristerie, etc.) ;
//                                                7 = Homéopathie...
long CMedicaBase::Medica_GetMedicamentListByFamily( QListView *pQlistView     ,
                                                    const int   prodFamily    ,     // = "1"
                                                    const QString &name       ,     // = "A"
                                                    const QString &isGene     ,     //= ""
                                                    const QString &isDispo          //= ""
                                                  )
{QString    table;
 switch(prodFamily)
    { default:
      case 1: table = m_MEDICA_INDEX_MED_TBL_NAME;   break;
      case 2: table = m_MEDICA_INDEX_DIET_TBL_NAME;  break;
      case 3: table = m_MEDICA_INDEX_VETO_TBL_NAME;  break;
      case 4: table = m_MEDICA_INDEX_PARA_TBL_NAME;  break;
      case 5: table = m_MEDICA_INDEX_ACCS_TBL_NAME;  break;
      case 6: table = m_MEDICA_INDEX_HERBO_TBL_NAME; break;
      case 7: table = m_MEDICA_INDEX_HOMEO_TBL_NAME; break;
    }
 QString    requete  = "";
 //................. Preparer la requete .....................................
 requete       +=      "SELECT " + m_MEDICA_INDEX_LIBELLE  + "," +            // 0
                                   m_MEDICA_INDEX_CD_PROD  + "," +            // 1
                                   m_MEDICA_INDEX_PK_T00   + "," +            // 2
                                   m_MEDICA_INDEX_PK_T4B   + "," +            // 3
                                   m_MEDICA_INDEX_CIP      + "," +            // 4
                                   m_MEDICA_INDEX_CD_UV    + "," +            // 5
                                   m_MEDICA_INDEX_IS_GENE  + "," +            // 6
                                   m_MEDICA_INDEX_IS_DISPO + " " +            // 7
                                   " FROM " + table + " WHERE ( " ;
  requete       += m_MEDICA_INDEX_LIBELLE + " LIKE \"" + name          + "%\" OR ";
  requete       += m_MEDICA_INDEX_LIBELLE + " LIKE \"" + name.upper()  + "%\" )  ";
  if (isGene !="" && isDispo != "")
     {requete += " AND " + m_MEDICA_INDEX_IS_DISPO + "='" + isDispo + "' AND (" +  m_MEDICA_INDEX_IS_GENE + "='G' OR "+  m_MEDICA_INDEX_IS_GENE + "='R')";
     }
  else  if (isGene !="")
     {requete += " AND (" + m_MEDICA_INDEX_IS_GENE  + "='G' OR "+  m_MEDICA_INDEX_IS_GENE + "='R' )";
     }
  else  if (isDispo !="")
     {requete += " AND " + m_MEDICA_INDEX_IS_DISPO + "='" + isDispo + "' ";
     }

 return Medica_GetProductsListFrom_SQL_String(pQlistView, requete );
}

//----------------------------------------------------- Medica_GetProductsListFrom_SQL_String --------------------------------------------------------------
int CMedicaBase::Medica_GetProductsListFrom_SQL_String(QListView *pQlistView, const QString &requete , int f /* = CMedicaBase::ClearList */)
{ if ( m_MedicaBase==0 )                                                              return 0;
  if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)                   return 0;
  QSqlQuery query(requete , m_MedicaBase );
  //................ creer les pixmap ici car ................................................
  //                 QPixmap::fromMimeSource( "standards.png" ) est long
  int   nb         = 0;
  QString isGene   = "";
  QString isDispo  = "";
  QPixmap std      =  QPixmap::fromMimeSource( "standards.png" );
  QPixmap sup      =  QPixmap::fromMimeSource( "supprimes.png" );
  QPixmap gen      =  QPixmap::fromMimeSource( "generiques.png" );
  QPixmap ref      =  QPixmap::fromMimeSource( "referent.png" );
  //................ scanner les enregistrements ................................................
  //                 pour remplir la listview
  if (query.isActive())                                         //si la requète a un resultat
     {if (f==CMedicaBase::ClearList) pQlistView->clear();
      while (query.next() && nb <m_MaxList)
        {QListViewItem *element = new  QListViewItem( pQlistView);
         if (element)
            {isGene  =  query.value(6 ).toString();
             isDispo =  query.value(7 ).toString();
             element->setText (0, Utf8_Query(query, 0) );                            // libelle
             //element->setText (0, query.value( 0).toString());     // code produit
             element->setText (1, query.value( 1).toString());     // code produit
             element->setText (2, query.value( 2).toString());     // PrimaryKey t00  (et aussi pk de cette table)
             element->setText (3, query.value( 3).toString());     // PrimaryKey t4B
             element->setText (4, query.value( 4).toString());     // CodeCIP
             element->setText (5, query.value( 5).toString());     // Code_UV
             element->setText (6, isGene);                         // IsGene
             element->setText (7, isDispo);                        // IsDispo
             element->setText (8, QString::number(nb));
             ++nb;
             if      (isDispo != "0")    element->setPixmap ( 0, sup );
             else if (isGene  == "G")    element->setPixmap ( 0, gen );
             else if (isGene  == "R")    element->setPixmap ( 0, ref );
             else                        element->setPixmap ( 0, std );
            }  // end if (element)
        } //end while (pSqlQuery->next())
     } //endif (pSqlQuery && pSqlQuery->isActive())
  //pQlistView->setColumnWidth ( 0, 500 );
  return nb;
}

//----------------------------------------------- Medica_GetNbRecord ---------------------------------------------------
QString CMedicaBase::Medica_GetNbRecord(const QString &table)
{ QString result = "";
  if ( m_MedicaBase==0 )                                            return QString::null;
  if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE) return QString::null;

  //................. SI un nom ou prenom est donné en entrée alors filtrer selon ...................
  QString requete = QString("SELECT COUNT(*) FROM ") + table;
  QSqlQuery query(requete , m_MedicaBase );
  if (query.isActive() && query.next())
     {result = query.value(0).toString();
     }
  return result;
}

//-----------------------------------------------------  ExtractNextNumericalData -------------------------------------------
char*    CMedicaBase::Medica_ExtractNextNumericalData(char* pt, char *end,  QString &val)
{val="";
 while (pt<end && !(*pt >='0' && *pt<='9' || *pt=='.') || *pt=='(') ++pt;
 char *deb = pt;
 while (pt<end &&   *pt >='0' && *pt<='9' || *pt=='.') ++pt;
 val.setLatin1 (deb, pt-deb );
 return pt;
}

//-----------------------------------------------------  ExtractNextTextData -------------------------------------------
char*    CMedicaBase::Medica_ExtractNextTextData(char* pt, char *end,  QString &val)
{val="";
 while (pt<end && *pt==' '|| *pt=='.' || *pt=='=' || *pt=='\t' || (*pt >='0' && *pt<='9' ))     ++pt;
 char *deb = pt;
 while (pt<end &&   !(*pt >='0' && *pt<='9' ) && *pt!=' ' && *pt!='\t' && *pt!=')' && *pt!='.') ++pt;
 val.setLatin1 (deb, pt-deb );
 val =  val.lower();
 if (*pt==')') ++pt;                                                              // si fin = parenthèse de fin la passer
 while (pt<end && *pt==' '|| *pt=='.' || *pt=='=' || *pt=='\t' ) ++pt;            // avancer jusqu'au debut d'un truc valide
 return pt;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////// BASEGET ////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////// ------------- ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------- BaseGet_To_MedicaTux -----------------------------------------------------------------
long CMedicaBase::BaseGet_To_MedicaTux(QProgressBar *pQProgressBar)
{long nb = BaseGet_To_MedicaTuxIndex(pQProgressBar);
 return nb;
}

//----------------------------------------------------- BaseGet_To_MedicaTuxIndex -----------------------------------------------------------------
long CMedicaBase::BaseGet_To_MedicaTuxIndex(QProgressBar *pQProgressBar)
{pQProgressBar->setTotalSteps(2300); // 2326
 long progress = 0;
 m_FormeUPList.clear();
 //........................................preparer la table de destination ................................................
 if (Medica_CreateTableIndexProduit(m_MEDICA_INDEX_MED_TBL_NAME)==FALSE)         return 0;
 if (Medica_CreateTableSpecificationProduit(m_MEDICA_SPEC_TBL_NAME)==FALSE)      return 0;

 if (m_DataGet_Base->isOpen()==FALSE && m_DataGet_Base->open()==FALSE) return 0;

  //................. Preparer la requete .....................................
  QString requete("SELECT "
                  "CIP,"                // 0
                  "VO,"                 // 1
                  "PRINCI,"             // 2
                  "NOM_COMMER,"         // 3
                  "DOS,"                // 4
                  "PRE,"                // 5
                  "SI,"                 // 6
                  "PRIXLIST,"           // 7
                  "LISTCONV,"           // 8
                  "ECARTPRIX,"          // 9
                  "ECT,"                // 10
                  "NOUNI,"              // 11
                  "COD3,"               // 12
                  "CODE_AT2,"           // 13
                  "REPERT,"             // 14
                  "NUSOUSGROU,"         // 15
                  "NUORDRE,"            // 16
                  "FLECHE,"             // 17
                  "PRIX,"               // 18
                  "NOTE_CIP,"           // 19
                  "VERSION,"            // 20
                  "LABORATOIR,"         // 21
                  "SMRINS,"             // 22
                  "TFR "                // 23
                  "FROM get");
  QSqlQuery query(requete , m_DataGet_Base );
  if (query.isActive() )
     {while (query.next())
            {pQProgressBar->setProgress(++progress);
             Medica_InsertNewProduitInIndex( Utf8_Query(query,3),    // Libelle
                                             Utf8_Query(query,0),    // CIP
                                             "",                                // pkt00
                                             "",                                // pkt4b
                                             "",                                // code_prod
                                             "",                                // code_UV
                                             "0",                               // isGene
                                             "0");                              // isDispo
             QString forme = BaseGet_QPA_Unit_to_Unit(Utf8_Query(query,5));
             QString taux;
             QString isGen = Utf8_Query(query,14);
             BaseGet_GetLibelleATC(Utf8_Query(query,13) , &taux);
             QString secabilite("1");
             if      (forme.find(TR(". séc"))!=-1)          secabilite = "2";
             else if (forme.find(TR("quadrisécable"))!=-1)  secabilite = "4";
             else if (forme.find(TR("sécable"))!=-1)        secabilite = "2";
             Medica_InsertNewSpec(Utf8_Query(query,0),                             // cip
                                  forme,                                           // forme de la prise (comprimé etc ....)
                                  Utf8_Query(query,4),                             // quantité de principe actif , unité
                                  "1",                                             // quantité de principe actif
                                  secabilite,                                      // secabilité
                                  isGen,                                           // is_Generic G  pour générique R pour réference d'un groupe ATC
                                  forme + " " + Utf8_Query(query,1),               // forme structurée
                                  Utf8_Query(query,11),                            // nombre unités de prise
                                  Utf8_Query(query,18),                            // prix
                                  taux.remove('%'),                                // taux secu
                                  " ",                                             // tableau
                                  Utf8_Query(query,21),                            // Laboratoire
                                  Utf8_Query(query,13));                           // code ATC
            }
     }
  OutSQL_error(query, "BaseGet_To_MedicaTuxIndex(): Error ", requete);

  Medica_CreateTableFormeProduit(m_MEDICA_FORME_UP_TBL_NAME, m_FormeUPList);
  Medica_CreateTableFormeProduit(m_MEDICA_FORME_PA_TBL_NAME, m_FormePAList);
  Medica_CreateTableFormeProduit(m_MEDICA_FORME_ST_TBL_NAME, m_FormeSTList);
  return progress;
}

//----------------------------------------------------- BaseGet_QPA_Unit_to_Unit ----------------------------------------------------------
QString  CMedicaBase::BaseGet_QPA_Unit_to_Unit(const QString &paq_unit)
{int i = 0;
 while (i < (int)paq_unit.length() && paq_unit[i]>='0'&& paq_unit[i]<='9' || paq_unit[i]==',' || paq_unit[i]==' ' ) ++i;
 return paq_unit.mid(i);
}

//-----------------------------------------------------  BaseGet_GetLibelleATC -------------------------------------------
QString CMedicaBase::BaseGet_GetLibelleATC( const QString &codeATC, QString *tauxSS)
{ if (m_DataGet_Base->isOpen()==FALSE && m_DataGet_Base->open()==FALSE)   return 0;
  //................. chercher données economiques pour ce code CIP...................................................
  QString      requete("SELECT LIBELLE_AT, TAUX");
  requete +=   " FROM get_atc WHERE CODE_AT2 ='" + codeATC + "'";

  QSqlQuery query (requete , m_DataGet_Base );
  if (query.isActive() && query.next())
     {if (tauxSS)
         {QString t = Utf8_Query(query,1);
          if (t.length()==0) *tauxSS = "0";
          else               *tauxSS = Utf8_Query(query,1);
         }
      return Utf8_Query(query,0);
     }
  if (tauxSS) *tauxSS = "0";
  return QString("");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////// DATASEMP ///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////// ------------- ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------- Datasemp_FillQListView_ATC ---------------------------------------------------------------
void CMedicaBase::Datasemp_FillQListView_ATC(QListView *pQListView )
{ if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE) return;
  QListViewItem *curParentItem = 0;
  QListViewItem *item          = 0;
  long                  curLen = 0;
  QString              requete = " SELECT f0, f2 FROM t42 ORDER BY f0";
  QSqlQuery query(requete, m_DataBaseSrce );
  if ( ! query.isActive() ) return;


  while (query.next())
       {       QString atc_code     = Utf8_Query(query,0);
               QString atc_libelle  = query.value(1).toString();
               int             len  = atc_code.length();

               if (len==1)
                  { curParentItem = new QListViewItem (pQListView);
                    curParentItem->setText(0,atc_libelle);
                    curParentItem->setText(1,atc_code);
                    curLen = len;
                  }
               else if (len > curLen)
                  { item =   new QListViewItem (curParentItem);
                    item->setText(0,atc_libelle);
                    item->setText(1,atc_code);
                    if (len != 7) curParentItem = item;
                    curLen = len;
                  }
               else if (len == curLen)
                  { item =   new QListViewItem (curParentItem);
                    item->setText(0,atc_libelle);
                    item->setText(1,atc_code);
                    curLen = len;
                  }
               else if (len < curLen)
                  { //......... remonter tant que le pere n'a pas de partie commune avec le fils ............
                    while( curParentItem->parent() != 0 &&  atc_code.left(curParentItem->text(1).length()) != curParentItem->text(1))
                         {curParentItem = curParentItem->parent();
                         }
                    curParentItem = new QListViewItem (curParentItem);
                    curParentItem->setText(0,atc_libelle);
                    curParentItem->setText(1,atc_code);
                    curLen = len;
                  }
       }
}

//----------------------------------------------- Datasemp_GetComposition ---------------------------------------------------------------------
QString CMedicaBase::Datasemp_GetComposition(const QString &codeProduit)
{if (m_DataBaseSrce==0) return QString("");
 int nb = 0;
 QString ret("<TABLE cellSpacing=\"2\" cellpadding=\"0\" width=416 border=\"1\">"
             "  <TBODY>"
             "   <TR>"
             "     <TD width=100 ><b>Nature du composant</b></TD>"
             "     <TD width=100 ><b>Nom du composant</b></TD>"
             "     <TD width=40  ><b>Dosage</b></TD>"
             "     <TD width=50  ><b>Unit&eacute;</b></TD>"
             "   </TR>"
           );
 QString body("  <TR>"
              "    <TD width=100 >{{nature}} </TD>"
              "    <TD width=100 >{{nom}}    </TD>"
              "    <TD width=40  >{{dose}}   </TD>"
              "    <TD width=50  >{{unite}}  </TD>"
              "  </TR>"
             );
 QString requete = " SELECT    "
                   "  t1C.f1 , "  // titre section
                   "  t27.f2 , "  // libelle substance
                   "  t07.f4 , "  // Quantité dosée
                   "  t24.f1   "  // unite
                   " FROM "
                   "  (((t08 RIGHT JOIN t07 ON (t08.f0 = t07.f0) AND (t08.f1 = t07.f1))"
                   "  LEFT JOIN t27 ON t07.f2 = t27.f0)"
                   "  LEFT JOIN t24 ON t07.f5 = t24.f0)"
                   "  LEFT JOIN t1C ON t08.f2 = t1C.f0"
                   " WHERE"
                   "  (((t07.f0)='#{==}#'))"
                   " ORDER BY"
                   "  t07.f0, t07.f1, t07.f3";

 requete.replace("#{==}#" , codeProduit);
 //................. Preparer la requete .....................................
 if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)              return ret;
 QSqlQuery query(requete, m_DataBaseSrce );
 //.................. si la requète a un resultat ..............................................
 if (query.isActive() )
    {while(query.next())
        {++nb;
         QString toAdd = body;
         toAdd.replace("{{nature}}",   Utf8_Query(query,0));
         toAdd.replace("{{nom}}",      Utf8_Query(query,1));
         QString dose(Utf8_Query(query,2));
         if (dose != "0" && dose != "")
            {toAdd.replace("{{dose}}",     dose);
             toAdd.replace("{{unite}}",    Utf8_Query(query,3));
            }
         else
            {toAdd.replace("{{dose}}",     " ");
             toAdd.replace("{{unite}}",    " ");
            }
         ret += toAdd ;          // type de du composant (principe actif, excipient ...)
        }
    }
 if (nb)  return ret + "  </TBODY></TABLE>";
 else     return QString("");
}

//----------------------------------------------- Datasemp_Update ---------------------------------------------------------------------
QString CMedicaBase::Datasemp_Update(const QString &fname,
                                     long &nb_sqlLine,
                                     UPDATE_MODE mode,          // = CMedicaBase::AllUpdate
                                     QSqlDatabase* pDataBase,   // = 0     si pas egal zero alors les requetes SQL de mise à jour seront executées
                                     const char*   fileOut      // = 0     si pas egal zero alors le fichier SQL de mise à jour sera ecrit
                                    )
{long nb_read = 0;
 QString table;
 QString sql("");
 char *end;
 char *pt;

 //............ ouverture fichier ..................................
 if ( !QFile::exists( fname ) )                     return sql;
 QFile file( fname );
 if ( !file.open( IO_ReadOnly ) )                   return sql;

 //............ creation buffer de lecture ..........................
 long len_buffer_c = file.size();
 char *buffer_c = new char[len_buffer_c];
 if (buffer_c==0)                                   return sql;
 nb_read = file.readBlock ( buffer_c, len_buffer_c);
 file.close();
 if (nb_read==0)                  {delete buffer_c; return sql;}

 //................. lire l'entete ..................................
 QString head("");
 head.setLatin1(buffer_c, 68);
 //................. parser le fichier ..............................
 pt  = buffer_c + 68;                     // + 68 (après le header bloc de debut)
 end = buffer_c + len_buffer_c - 93;      // - 93 (moins le bloc de fin)
 while (pt<end)
 {switch(pt[0])
  {case'[':
         table.setLatin1(pt + 1, 2);  table.prepend("t");
         pt += 15;
         break;
   case '~':
         pt += 2;
         if      (pt[-1]=='M') sql += Datasemp_UpdateInTable(pt, end, table, mode & CMedicaBase::Update, nb_sqlLine, pDataBase);
         else if (pt[-1]=='C') sql += Datasemp_InsertInTable(pt, end, table, mode & CMedicaBase::Insert, nb_sqlLine, pDataBase);
         else if (pt[-1]=='S') sql += Datasemp_DeleteInTable(pt, end, table, mode & CMedicaBase::Remove, nb_sqlLine, pDataBase);
         break;
   default: ++pt;
  }
 }
 if (fileOut)
    {QFile fileW(fileOut);
     if ( !fileW.open( IO_WriteOnly ) )    {delete buffer_c; return sql;}
     fileW.writeBlock (sql , sql.length());
     fileW.close();
    }
 delete buffer_c;
 //.......... on déclare une variable pour le lire ligne à ligne ..................
 return sql;
}

//----------------------------------------------- Datasemp_UpdateInTable ---------------------------------------------------------------------
// ENTREE: ptIn  pointe sur le signe $  ( celui de  ~M$ )
QString CMedicaBase::Datasemp_UpdateInTable(char* &ptIn, char* end, const QString &table, int doIt, long &nb_sqlLine, QSqlDatabase* pDataBase)
{char    *pt      = ptIn;
 QString data_sql = "";
 if (doIt==0)
    {while (pt < end && *pt != '~' && *pt != '[') ++pt;
     ptIn = pt;
     return data_sql;
    }
 //............................... la commence le truc ......................................
 QString key_value, str;
 QString key      ="f";
 data_sql         = "UPDATE TABLE ";
 key +=  pt[1]; pt+=2;
 char   *deb = pt;
 // while (pt < end && *pt != '@' && *pt != '[') ++pt;
 // key_value.setLatin1(deb, pt-deb);
 //................... extraire valeur de la clef ......................................
 key_value = "";
 while (pt < end &&  *pt != '@' && *pt != '[')
       {if (*pt=='#')
           {key_value += str.setLatin1(deb, pt-deb);
            ++pt;
            key_value += Datasemp_DecodeSpecialCar(str.setLatin1(pt, 2));
            pt+=2;
            deb = pt;
           }
       else
           {++pt;
           }
       }
 key_value += str.setLatin1(deb, pt-deb);

 //................... extraire autres valeurs ..........................................
 int     n   = 0;
 data_sql   +=  table + " SET ( ";
 while (pt < end && *pt != '~' && *pt != '[')
       {//                          @1345   Premiére valeur apres le @ est le N°champ,
        //                                  la deuxième jusqu'à fin (@ autre champ ou ~ fin des champs) est la valeur du champ
        if (*pt == '@')
           {if (n++) data_sql += " , ";     // rajouter virgule de separation si besoin
            data_sql += QString("f") + pt[1] + "='"; pt+=2;
            deb = pt;
            while (pt < end && *pt != '~' && *pt != '@' && *pt != '[')
                  {if (*pt=='#')
                      {data_sql += str.setLatin1(deb, pt-deb);
                       ++pt;
                       data_sql += Datasemp_DecodeSpecialCar(str.setLatin1(pt, 2));
                       pt+=2;
                       deb = pt;
                      }
                   else
                      {++pt;
                      }
                  }
            data_sql += str.setLatin1(deb, pt-deb) + "'";
           }
        }
 data_sql += " ) WHERE ( " + key + "='" + key_value + "' );\n";
 if (pDataBase)
    {QSqlQuery query(data_sql, pDataBase);
    }
 ++nb_sqlLine;
 ptIn = pt;
 return data_sql;
}

//----------------------------------------------- Datasemp_InsertInTable ---------------------------------------------------------------------
// ENTREE: ptIn  pointe sur le signe $  ( celui de  ~C$ )
QString CMedicaBase::Datasemp_InsertInTable(char* &ptIn, char* end, const QString &table, int doIt, long &nb_sqlLine, QSqlDatabase* pDataBase)
{char    *pt      = ptIn;
 QString data_sql = "";
 if (doIt==0)
    {while (pt < end && *pt != '~' && *pt != '[') ++pt;
     ptIn = pt;
     return data_sql;
    }
 //............................... la commence le truc ......................................
 data_sql           = "INSERT INTO ";
 data_sql          += table + " ( ";
 int  n             = 0;
 QString field_list = "";
 QString value_list = "'";
 QString str;
 char   *deb;
 while (pt < end && *pt != '~' && *pt != '[')
       {//                          @1345   Premiére valeur apres le @ est le N°champ,
        //                                  la deuxième jusqu'à fin (@ autre champ ou ~ fin des champs) est la valeur du champ
        if (n++) {field_list += " , ";  value_list += " , '";}   // rajouter virgule de separation si besoin
        field_list += QString("f") + pt[1]; pt+=2;
        while (pt < end && *pt != '~' && *pt != '[' && *pt != '$' && *pt==' ') ++pt;
        deb = pt;
        while (pt < end && *pt != '~' && *pt != '@'&& *pt != '[')
              {if (*pt=='#')
                  {value_list += str.setLatin1(deb, pt-deb);
                   ++pt;
                   value_list += Datasemp_DecodeSpecialCar(str.setLatin1(pt, 2));
                   pt+=2;
                   deb = pt;
                  }
              else
                  {++pt;
                  }
             }
        value_list += str.setLatin1(deb, pt-deb) + "'";
      }
 data_sql += field_list + " ) VALUES ( " + value_list + " );\n";
 if (pDataBase)
    {QSqlQuery query(data_sql, pDataBase);
    }
 ++nb_sqlLine;
 ptIn = pt;
 return data_sql;
}

//----------------------------------------------- Datasemp_DeleteInTable ---------------------------------------------------------------------
// ENTREE: ptIn  pointe sur le signe $  ( celui de  ~S$ )
QString CMedicaBase::Datasemp_DeleteInTable(char* &ptIn, char* end, const QString &table, int doIt, long &nb_sqlLine, QSqlDatabase* pDataBase)
{char    *pt      = ptIn;
 QString data_sql = "";
 if (doIt==0)
    {while (pt < end && *pt != '~' && *pt != '[') ++pt;
     ptIn = pt;
     return data_sql;
    }
 //............................... la commence le truc ......................................
 data_sql         = "DELETE FROM ";
 data_sql        +=  table + " WHERE ( ";
 int n            = 0;
 char   *deb;
 QString str;
 while (pt < end && *pt != '~' && *pt != '[')
       {//                          @1345   Premiére valeur apres le @ est le N°champ,
        //                                  la deuxième jusqu'à fin (@ autre champ ou ~ fin des champs) est la valeur du champ
        if (n++) data_sql += " AND ";    // rajouter virgule de separation si besoin
        data_sql          += QString("f") + pt [1] + "='"; pt+=2;
        while (pt < end && *pt != '~' && *pt != '[' && *pt != '$' && *pt==' ') ++pt;
        deb = pt;
        while (pt < end && *pt != '~' && *pt != '[' && *pt != '$')
              {if (*pt=='#')
                  {data_sql += str.setLatin1(deb, pt-deb);
                   ++pt;
                   data_sql += Datasemp_DecodeSpecialCar(str.setLatin1(pt, 2));
                   pt+=2;
                   deb = pt;
                  }
               else
                  {++pt;
                  }
              }
        data_sql += str.setLatin1(deb, pt-deb) + "'";
       }
 data_sql +=  " );\n";
 if (pDataBase)
    {QSqlQuery query(data_sql, pDataBase);
    }
 ++nb_sqlLine;
 ptIn = pt;
 return data_sql;
}

//----------------------------------------------- Datasemp_GetBaseVersion ---------------------------------------------------------------------
QString CMedicaBase::Datasemp_GetBaseVersion( QString *numEdition, QString *packBCArrete, QString *packIRArrete)
{ QString ods("");
  if      (packBCArrete ) *packBCArrete = "";
  if      (packIRArrete)  *packIRArrete = "";
  if      (numEdition)    *numEdition = "";

  //................. Preparer la requete .....................................
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)              return ods;
  QSqlQuery query("SELECT f0,f1 FROM tFB", m_DataBaseSrce );
  //.................. si la requète a un resultat ..............................................
  if (query.isActive())
     {while (query.next())
            {if      (packBCArrete && query.value(0).toString()=="ARRETE PACK BC") *packBCArrete = query.value(1).toString();
             else if (packIRArrete && query.value(0).toString()=="ARRETE PACK IR") *packIRArrete = query.value(1).toString();
             else if (numEdition   && query.value(0).toString()=="EDITION NUMERO") *numEdition   = query.value(1).toString();
             else if (query.value(0).toString()=="ODS")                             ods          = query.value(1).toString();
            }
     }
  QString err;
  OutSQL_error( query, "Datasemp_GetBaseVersion()", "SELECT f0,f1, FROM tFB", &err);
  return ods.remove("#00");
}

//----------------------------------------------- Datasemp_GetDiskVersion ---------------------------------------------------------------------
QString CMedicaBase::Datasemp_GetDiskVersion(const QString &fname)
{QString ods("");
 //............ ouverture fichier ..................................
 if ( !QFile::exists( fname ) )                     return ods;
 QFile file( fname );
 if ( !file.open( IO_ReadOnly ) )                   return ods;

 //............ creation buffer de lecture ..........................
 long len_buffer_c = 36;
 char *buffer_c    = new char[len_buffer_c];
 if (buffer_c==0)                                   return ods;
 long nb_read = file.readBlock ( buffer_c, len_buffer_c);
 file.close();
 if (nb_read!=len_buffer_c)       {delete buffer_c; return ods;}
 ods.setLatin1(buffer_c+20,16);
 delete buffer_c;
 return ods;
}

//----------------------------------------------- Datasemp_DecodeSpecialCar ---------------------------------------------------------------------
QString CMedicaBase::Datasemp_DecodeSpecialCar(const QString &val)
{
 QString DECODE_TBL = " ????????\t\n??\r??"
                      "????????????????"
                      "??\"#$???????????"
                      "????????????????"
                      "@???????????????"
                      "???????????[\\]^_'"
                      "\'???????????????"
                      "???????????{|}~?"
                      "??,§\"_¡?^??«????"
                      "??\'\"\"·--~?s»æ??ÿ"
                      "?¡?£?¥?§?©ª«¬-®-"
                      "°±²³?µ¶·?¹º»???¿"
                      "ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏ"
                      "ÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞß"
                      "ààâãäåæçèéêëìíîï"
                      "ðñòóôõö÷øùúûüýþÿ";

 //
 /*                      //  0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F      00
 char DECODE_TBL[256] = {  0, '?', '?', '?', '?', '?', '?', '?', '?',   9,'\n', '?', '?','\r', '?', '?',\
                       // 10   11   12   13   14   15   16   17   18   19   1A   1B   1C   1D   1E   1F      01
                         '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?',\
                       // 20   21   22   23   24   25   26   27   28   29   2A   2B   1C   2D   2E   2F      02
                         '?', '?','\"', '#', '$', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?',\
                       // 30   31   32   33   34   35   36   37   38   39   3A   3B   3C   3D   3E   3F      03
                         '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?',\
                       // 40   41   42   43   44   45   46   47   48   49   4A   4B   4C   4D   4E   4F      04
                         '@', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?',\
                       // 50   51   52   53   54   55   56   57   58   59   5A   5B   5C   5D   5E   5F      05
                         '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '[','\\', ']', '^', '_',\
                       // 60   61   62   63   64   65   66   67   68   69   6A   6B   6C   6D   6E   6F      06
                        '\'', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?',\
                       // 70   71   72   73   74   75   76   77   78   79   7A   7B   7C   7D   7E   7F      07
                         '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '{', '|', '}', '~', '?',\
                       // 80   81   82   83   84   85   86   87   88   89   8A   8B   8C   8D   8E   8F      08
                         '?', '?', ',', '§','\"', '_', '¡', '?', '^', '?', '?', '«', '?', '?', '?', '?',\
                       // 90   91   92   93   94   95   96   97   98   99   9A   9B   9C   9D   9E   9F      09
                         '?', '?','\'','\"','\"', '·', '-', '-', '~', '?', 's', '»', 'æ', '?', '?', 'ÿ',\
                       // A0   A1   A2   A3   A4   A5   A6   A7   A8   A9   AA   AB   AC   AD   AE   AF      0A
                         '?', '¡', '?', '£', '?', '¥', '?', '§', '?', '©', 'ª', '«', '¬', '-', '®', '-',\
                       // B0   B1   B2   B3   B4   B5   B6   B7   B8   B9   BA   BB   BC   BD   BE   BF      0B
                         '°', '±', '²', '³', '?', 'µ', '¶', '·', '?', '¹', 'º', '»', '?', '?', '?', '¿',\
                       // C0   C1   C2   C3   C4   C5   C6   C7   C8   C9   CA   CB   CC   CD   CE   CF      0C
                         'À', 'Á', 'Â', 'Ã', 'Ä', 'Å', 'Æ', 'Ç', 'È', 'É', 'Ê', 'Ë', 'Ì', 'Í', 'Î', 'Ï',\
                       // D0   D1   D2   D3   D4   D5   D6   D7   D8   D9   DA   DB   DC   DD   DE   DF      0D
                         'Ð', 'Ñ', 'Ò', 'Ó', 'Ô', 'Õ', 'Ö', '×', 'Ø', 'Ù', 'Ú', 'Û', 'Ü', 'Ý', 'Þ', 'ß',\
                       // E0   E1   E2   E3   E4   E5   E6   E7   E8   E9   EA   EB   EC   ED   EE   EF      OE
                         'à', 'à', 'â', 'ã', 'ä', 'å', 'æ', 'ç', 'è', 'é', 'ê', 'ë', 'ì', 'í', 'î', 'ï',\
                       // F0   F1   F2   F3   F4   F5   F6   F7   F8   F9   FA   FB   FC   FD   FE   FF      0F
                         'ð', 'ñ', 'ò', 'ó', 'ô', 'õ', 'ö', '÷', 'ø', 'ù', 'ú', 'û', 'ü', 'ý', 'þ', 'ÿ'
                       }; */

 QString ret = "";
 int index   = val.toInt(0,16);
 if       (  val ==  "EU")                    ret = "euro";
 else if  (  val ==  "9C")                    ret = "oe";
 else if  (  val ==  "8C")                    ret = "OE";
 else if  (  val ==  "85")                    ret = "...";
 else if  (  val ==  "89")                    ret = "°/oo";
 else if  (  val ==  "99")                    ret = "TM";
 else if  (  val ==  "00"||index==0)          ret = "";
 else                                         ret =  (DECODE_TBL.at(index));
 return ret;
/*
QChar p('«');
    QString ret;
    //  0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F      00
 const QChar DECODE_TBL[256] = {  0, '?', '?', '?', '?', '?', '?', '?', '?',   9,'\n', '?', '?','\r', '?', '?',\
                              // 10   11   12   13   14   15   16   17   18   19   1A   1B   1C   1D   1E   1F      01
                              '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?',\
                              // 20   21   22   23   24   25   26   27   28   29   2A   2B   1C   2D   2E   2F      02
                              '?', '?','\"', '#', '$', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?',\
                              // 30   31   32   33   34   35   36   37   38   39   3A   3B   3C   3D   3E   3F      03
                              '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?',\
                              // 40   41   42   43   44   45   46   47   48   49   4A   4B   4C   4D   4E   4F      04
                              '@', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?',\
                              // 50   51   52   53   54   55   56   57   58   59   5A   5B   5C   5D   5E   5F      05
                              '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '[','\\', ']', '^', '_',\
                              // 60   61   62   63   64   65   66   67   68   69   6A   6B   6C   6D   6E   6F      06
                              '\'', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?',\
                              // 70   71   72   73   74   75   76   77   78   79   7A   7B   7C   7D   7E   7F      07
                              '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '{', '|', '}', '~', '?',\
                              // 80   81   82   83   84   85   86   87   88   89   8A   8B   8C   8D   8E   8F      08
                              '?', '?', ',', '§','\"', '_', '¡', '?', '^', '?', '?', '«', '?', '?', '?', '?',\
                              // 90   91   92   93   94   95   96   97   98   99   9A   9B   9C   9D   9E   9F      09
                              '?', '?','\'','\"','\"', '·', '-', '-', '~', '?', 's', '»', 'æ', '?', '?', 'ÿ',\
                              // A0   A1   A2   A3   A4   A5   A6   A7   A8   A9   AA   AB   AC   AD   AE   AF      0A
                              '?', '¡', '?', '£', '?', '¥', '?', '§', '?', '©', 'ª', '«', '¬', '-', '®', '-',\
                              // B0   B1   B2   B3   B4   B5   B6   B7   B8   B9   BA   BB   BC   BD   BE   BF      0B
                              '°', '±', '²', '³', '?', 'µ', '¶', '·', '?', '¹', 'º', '»', '?', '?', '?', '¿',\
                              // C0   C1   C2   C3   C4   C5   C6   C7   C8   C9   CA   CB   CC   CD   CE   CF      0C
                              'À', 'Á', 'Â', 'Ã', 'Ä', 'Å', 'Æ', 'Ç', 'È', 'É', 'Ê', 'Ë', 'Ì', 'Í', 'Î', 'Ï',\
                              // D0   D1   D2   D3   D4   D5   D6   D7   D8   D9   DA   DB   DC   DD   DE   DF      0D
                              'Ð', 'Ñ', 'Ò', 'Ó', 'Ô', 'Õ', 'Ö', '×', 'Ø', 'Ù', 'Ú', 'Û', 'Ü', 'Ý', 'Þ', 'ß',\
                              // E0   E1   E2   E3   E4   E5   E6   E7   E8   E9   EA   EB   EC   ED   EE   EF      OE
                              'à', 'à', 'â', 'ã', 'ä', 'å', 'æ', 'ç', 'è', 'é', 'ê', 'ë', 'ì', 'í', 'î', 'ï',\
                              // F0   F1   F2   F3   F4   F5   F6   F7   F8   F9   FA   FB   FC   FD   FE   FF      0F
                              'ð', 'ñ', 'ò', 'ó', 'ô', 'õ', 'ö', '÷', 'ø', 'ù', 'ú', 'û', 'ü', 'ý', 'þ', 'ÿ'
                          };
    if       (  val ==  "EU") ret = "euro";
    else if  (  val ==  "9C") ret = "oe";
    else if  (  val ==  "8C") ret = "OE";
    else if  (  val ==  "85") ret = "...";
    else if  (  val ==  "89") ret = "°/oo";
    else if  (  val ==  "99") ret = "TM";
    else if  (  val ==  "00") ret = "";
    else                      ret = QChar (DECODE_TBL[val.toInt(0,16)]);
    return ret;
 */
}

//----------------------------------------------- Datasemp_MakeAllIndexProduit ---------------------------------------------------
int CMedicaBase::Datasemp_MakeAllIndexProduit(QProgressBar *pQProgressBar, QTextEdit *logWidget /* = 0 */)
{   int nb(0);
    int ret;
    m_Progress = 0;
    m_Run      = 1;
    ret = Datasemp_MakeIndex(CMedicaBase::MED_TBL_NAME,   pQProgressBar, logWidget);  nb += ret;  //if (ret<=0) return nb;
    ret = Datasemp_MakeIndex(CMedicaBase::DIET_TBL_NAME,  pQProgressBar, logWidget);  nb += ret;  //if (ret<=0) return nb;
    ret = Datasemp_MakeIndex(CMedicaBase::VETO_TBL_NAME,  pQProgressBar, logWidget);  nb += ret;  //if (ret<=0) return nb;
    ret = Datasemp_MakeIndex(CMedicaBase::PARA_TBL_NAME,  pQProgressBar, logWidget);  nb += ret;  //if (ret<=0) return nb;
    ret = Datasemp_MakeIndex(CMedicaBase::ACCS_TBL_NAME,  pQProgressBar, logWidget);  nb += ret;  //if (ret<=0) return nb;
    ret = Datasemp_MakeIndex(CMedicaBase::HERBO_TBL_NAME, pQProgressBar, logWidget);  nb += ret;  //if (ret<=0) return nb;
    ret = Datasemp_MakeIndex(CMedicaBase::HOMEO_TBL_NAME, pQProgressBar, logWidget);  nb += ret;  //if (ret<=0) return nb;
    return nb;
}
//----------------------------------------------- Datasemp_OutMessage ---------------------------------------------------------------------
void  CMedicaBase::Datasemp_OutMessage( QTextEdit *logWidget, const QString &mess)
{if (logWidget==0)  return;
    logWidget->append( mess );
    logWidget->scrollToBottom ();
    //logWidget->moveCursor(QTextEdit::MoveEnd);
    //logWidget->ensureCursorVisible ();
    //hasPendingEvents ();
    //qApp->flush();
    //qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();
    qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();
}
//----------------------------------------------- Datasemp_MakeIndex ---------------------------------------------------------------
int CMedicaBase::Datasemp_MakeIndex(int table_type, QProgressBar *pQProgressBar, QTextEdit *logWidget /* = 0 */ )
{ QString    tableName;
  QString    requete;
  QString    code_prod;
  QString    pk_t4B;
  QString    err;
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)             return  0;
  switch (table_type)
    {case CMedicaBase::MED_TBL_NAME:    tableName = m_MEDICA_INDEX_MED_TBL_NAME;    break;
     case CMedicaBase::DIET_TBL_NAME:   tableName = m_MEDICA_INDEX_DIET_TBL_NAME;   break;
     case CMedicaBase::VETO_TBL_NAME:   tableName = m_MEDICA_INDEX_VETO_TBL_NAME;   break;
     case CMedicaBase::PARA_TBL_NAME:   tableName = m_MEDICA_INDEX_PARA_TBL_NAME;   break;
     case CMedicaBase::ACCS_TBL_NAME:   tableName = m_MEDICA_INDEX_ACCS_TBL_NAME;   break;
     case CMedicaBase::HERBO_TBL_NAME:  tableName = m_MEDICA_INDEX_HERBO_TBL_NAME;  break;
     case CMedicaBase::HOMEO_TBL_NAME:  tableName = m_MEDICA_INDEX_HOMEO_TBL_NAME;  break;
    }
 if (Medica_CreateTableIndexProduit(tableName)==0)                                  return -1;
 Datasemp_OutMessage( logWidget, TR("======================= %1 en cours d'indexation =======================").arg(tableName));
  //................. Preparer la requete ............................................................................
  //                        0   1   2   3   4   5
  requete        = "SELECT fK, ge, f0, fG, f2, f1 FROM t00 WHERE fk LIKE '";
  //        Famille SEMP____^   ^   ^   ^   ^   ^
  //        PrimaryKey _________|   |   |   |   |
  //        Code produit____________|   |   |   |
  //        Libelle Medicament _________|   |   |
  //        code CIP _______________________|   |
  //        code_UV ____________________________|
  requete          += QString::number(table_type) + "%'";
  QSqlQuery query  (requete , m_DataBaseSrce );

  int nb = 0;
  QSqlQuery  sqlQueryPk ( m_DataBaseSrce );                 // pour accélérer un peu la query est instanciée ici
  QSqlCursor cur(tableName, TRUE, m_MedicaBase);            // pour accélérer un peu le cur est instancié ici
  QSqlQuery  queryExt(m_DataBaseSrce);                      // pour accélérer un peu la query est instanciée ici
  QString    requetePk  = "SELECT ge FROM t4B WHERE f0=";
  //.................. si la requète a un resultat ...................................................................
  if (query.isActive())
     {while (query.next()&& m_Run /* && nb<10 */ )
        {    //............... recuperer pk de la t4b correspondant au code produit de la t00 .......................
             //                fait augmenter la routine de quelques minutes à plusieurs heures > 6 heures !!!!
             //                mais accelère considérablement l'accés à la t4B

             code_prod = query.value(2).toString();
             sqlQueryPk.exec (requetePk + code_prod );
             if (sqlQueryPk.isActive() && sqlQueryPk.next())
                 pk_t4B = sqlQueryPk.value( 0).toString();
             else
                 pk_t4B = "";
             ++m_Progress;
             if (pQProgressBar) pQProgressBar->setProgress(m_Progress);
             //............... crer un curseur SQL ...................................................................

             if ( cur.canInsert ())
                {//............... si OK on recupere le buffer et on le charge avec les données ......................
                 QSqlRecord *buffer = cur.primeInsert();       // recuperer le buffer d'insertion (a ne pas killer en fin de routine!!)
                 buffer->setValue( m_MEDICA_INDEX_PK_T00,       query.value( 1).toString());        // y placer les données
                 buffer->setValue( m_MEDICA_INDEX_PK_T4B,       pk_t4B);                            // y placer les données
                 buffer->setValue( m_MEDICA_INDEX_CD_PROD,      code_prod);                         // y placer les données
                 buffer->setValue( m_MEDICA_INDEX_LIBELLE,      query.value( 3).toString());        // y placer les données
                 buffer->setValue( m_MEDICA_INDEX_CIP,          query.value( 4).toString());        // y placer les données
                 buffer->setValue( m_MEDICA_INDEX_CD_UV,        query.value( 5).toString());        // y placer les données
                 buffer->setValue( m_MEDICA_INDEX_IS_GENE,      Datasemp_GetGenericStateEx(query.value( 2).toString(),       queryExt));
                 buffer->setValue( m_MEDICA_INDEX_IS_DISPO,     Datasemp_GetDisponibiliteStateEx(query.value( 5).toString(), queryExt));
                 if (!cur.insert())
                    {
                     OutSQL_error(cur, "Datasemp_MakeIndex(): Error cur.insert", requete, &err);   // ecrire le buffer
                     Datasemp_OutMessage( logWidget, err);
                    }
                }
             else
               {OutSQL_error(cur, "Datasemp_MakeIndex(): Error cur.canInsert", requete, &err);
                Datasemp_OutMessage( logWidget, err);
                return -2;
               }
             nb++;
        } //end while (pSqlQuery->next())
     } //endif (pSqlQuery && pSqlQuery->isActive())
  else if (m_Run) // erreur que si routine s'arrete par erreur SQL et non interruption volontaire (m_Run==0)
    {OutSQL_error(query, "Datasemp_MakeIndex(): Error select t00", requete, &err);
     Datasemp_OutMessage( logWidget, err);
    }
  return nb;
}

//-----------------------------------------------------  Datasemp_GetGenericStateEx -------------------------------------------
QString   CMedicaBase::Datasemp_GetGenericStateEx(const QString &codeProduit, QSqlQuery  &query)
{ query.exec (QString("SELECT f2 FROM t3F WHERE f0 = '") + codeProduit +"'");

  //.................. si la requète a un resultat ..............................................
  if (query.isActive() && query.next())
     {int typ = query.value(0).toInt();
      if (typ&1) return QString("R");
      if (typ&2) return QString("G");
      if (typ&4) return QString("GA");
     }
  return QString("");
}

//-----------------------------------------------------  Datasemp_GetDisponibiliteStateEx -------------------------------------------
QString   CMedicaBase::Datasemp_GetDisponibiliteStateEx(const QString &codeUV, QSqlQuery  &query)
{ query.exec (QString("SELECT f3 FROM t02 WHERE f0 = '") + codeUV +"'" );
  //.................. si la requète a un resultat ..............................................
  if (query.isActive() && query.next())                                                               return query.value(0).toString();
                                                                                                      return QString("");
}

//-----------------------------------------------------  Datasemp_GetSubstanceList -------------------------------------------
long CMedicaBase::Datasemp_GetSubstanceList(QListView       *pQlistView,  const QString   &qstr_nom )
{QString   q_nom     = "a";
 QString    requete  = "";
 int             nb  = 0;
 //................. Preparer la requete .....................................
  if (m_DataBaseSrce == 0 )                                                         return  0;
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)             return  0;
  pQlistView->clear();
  if (qstr_nom != "")
     {q_nom    = qstr_nom;
     }
  requete       += "SELECT f2,f0 FROM t54 WHERE ";            // t54.f0 = code substance
  requete       += "f2 LIKE \"" + q_nom  + "%\" ";            // t54.f2 = nom de la molécule
  QSqlQuery query (requete , m_DataBaseSrce );
  if (query.isActive())
     {while (query.next())
        {if (nb <m_MaxList)
            {new QListViewItem( pQlistView,
                                Utf8_Query(query, 0     ),     // libelle
                                Utf8_Query(query, 1     )      // code substance
                              );
             ++nb;
            } // endif (i <200)
        }//endwhile (pSqlQueryMed->next())
     }//endif (pSqlQueryMed && pSqlQueryMed->isActive())
  return nb;
}

//-----------------------------------------------------  Datasemp_GetIndicationList -------------------------------------------
long CMedicaBase::Datasemp_GetIndicationList(QListView       *pQlistView,   const QString   &qstr_nom )
{ QString   q_nom     = "a";
  QString   q_Nom     = "A";
  QString    requete  = "";
  long            nb  = 0;
  //................. Preparer la requete .....................................
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)             return  0;
  pQlistView->clear();
  if (qstr_nom != "")
     {q_nom    = qstr_nom;
      q_Nom    = qstr_nom.left(1).upper() + qstr_nom.mid(1);  // majusculer le premier car
     }
  //    Domaine d'application_______
  //    code du terme __________    |
  //    Libelle indication _    |   |
  //                        |   |   |
  requete       += "SELECT f1, f0, f4 FROM t50 WHERE ";                               // t50.f1 = libelle indication
  requete       += "(f1 LIKE \"%" + q_nom  + "%\" OR f1 LIKE \"%"+ q_Nom  + "%\")";   // t50.f2 = nom de la molécule
  //requete       += " AND (f4>=1 AND f4<=3)";
  //requete       += " AND f4=1";
  requete       += " AND ((f4=1) OR (f4=3))";
  QSqlQuery query (requete , m_DataBaseSrce );
  if (query.isActive())
     {while (query.next())
        {if (nb <m_MaxList)
            {new QListViewItem( pQlistView,
                                Utf8_Query(query,0),     // libelle
                                Utf8_Query(query,1),     // code du terme
                                Utf8_Query(query,2)      // domaine d'application
                              );
             nb++;
            } // endif (i <200)
        }//endwhile (pSqlQueryMed->next())
     }//endif (pSqlQueryMed && pSqlQueryMed->isActive())
  return nb;
}

//-----------------------------------------------------  Datasemp_GetLibelleATC -------------------------------------------
QString CMedicaBase::Datasemp_GetLibelleATC( const QString &codeATC)
{ if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)   return 0;
  //................. chercher données economiques pour ce code CIP...................................................
  QSqlQuery query (QString("SELECT f2 FROM t42 WHERE f0 ='") + codeATC + "'" , m_DataBaseSrce );
  if (query.isActive() && query.next())
     {return Utf8_Query(query,0);
     }
  return QString("");
}

//----------------------------------------------------- Datasemp_GetSpecifications ---------------------------------------------------------------------------
QString CMedicaBase::Datasemp_GetSpecifications(  const QString &cip,
                                                  QString &up_unit,         // ok
                                                  QString &pa_unit,         // ok
                                                  QString &pa_qu,           // ok
                                                  QString &secabilite,      // ok
                                                  QString &is_generic,      // ok
                                                  QString &formStruct,      // ok
                                                  QString &nbPrises,        // ok
                                                  QString &prixBoite,       // ok
                                                  QString &remboursementSS, // ok
                                                  QString &tableau,         // ok
                                                  QString &labo,            // ok
                                                  QString &atc,             // ok
                                                  QString pkt00,            // = QString::null
                                                  QString pkt4b,            // = QString::null
                                                  QString codePD,           // = QString::null
                                                  QString codeUV            // = QString::null
                                                )
{
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE) return QString::null;
  //................... si accelerateurs de recherche non renseignés, les renseigner ..........................
  //                    ultra lent !!!!
  if (pkt00.length()==0 || pkt4b.length()==0 || codePD.length()==0 || codeUV.length()==0)
     {QString requete("SELECT ");
      // 2  PrimaryKey ________________
      // 1  Code unite de vente ___    |
      // 0  code produit    ___    |   |
      //                       |   |   |
      requete +=             "f0, f1, ge FROM t00 WHERE f2 = '" + cip + "'";
      QSqlQuery query(requete , m_DataBaseSrce );
      if (! (query.isActive() && query.next()))                             return QString::null;
      codePD = Utf8_Query(query,0);
      codeUV = Utf8_Query(query,1);
      pkt00  = Utf8_Query(query,2);
      Datasemp_GetField("t4B", "f0",  codePD, "f2", &pkt4b);
     }
  //............... aller à la pêche dans la t4b .....................................
  QStringList list;
  Datasemp_GetT4B( list, pkt4b);
  formStruct      = Datasemp_GetField("t25", "f0", list[T4B_F1_FORME], "f1");                // forme structuree
  atc             = list[T4B_FA_ATC_CLASS];
  Datasemp_GetFicheEconomique( cip, prixBoite, labo, tableau, remboursementSS, codeUV   );
  /*
  prixBoite       = Datasemp_GetField("t01", "f0",  codeUV,           "f3");   // SELECT fieldToGet FROM table WHERE testField = testValue
  labo            = Datasemp_GetField("t02", "f0",  codeUV,           "f1");   // SELECT fieldToGet FROM table WHERE testField = testValue obtenir code fabriquant
  labo            = Datasemp_GetField("t17", "f0",  labo,             "f2");   // SELECT fieldToGet FROM table WHERE testField = testValue obtenir libelle nom fabriquant
  remboursementSS = Datasemp_GetField("t03", "f0",  codeUV,           "f2");   // SELECT fieldToGet FROM table WHERE testField = testValue obtenir code remboursement
  remboursementSS = Datasemp_GetField("t1D", "f0",  remboursementSS,  "f1");   // SELECT fieldToGet FROM table WHERE testField = testValue obtenir libelle remboursement
  tableau         = Datasemp_GetField("t03", "f0",  codeUV,           "f3");   // SELECT fieldToGet FROM table WHERE testField = testValue obtenir liste
  */
  is_generic      = Datasemp_GetGenericState(codePD, 0);
  pa_unit         = Datasemp_GetDosage(list[T4B_FB_CODE_DOSAGE], list[T4B_F0_CODE_PRODUIT]).remove(" ; ");

  //............... separer ivraie du bon grain .....................................
  pa_qu           = "1";
  if (pa_unit[0]>='0' && pa_unit[0]<='9' || pa_unit[0]=='.' || pa_unit[0]==',')
     {int pos = 0;
      while (pa_unit[pos]>='0' && pa_unit[pos]<='9' || pa_unit[pos]==',' || pa_unit[pos]=='.') ++pos;
      pa_qu       = pa_unit.left(pos).replace(",",".");
      pa_unit     = pa_unit.mid(pos);
     }
  //................. il  en faut un peu de la t00 .........................
  Datasemp_GetT00( list, pkt00);
  int           i = 0;
  up_unit         = Datasemp_GetField("t24", "f0", list[i++], "f1");             // forme de la prise
  nbPrises        = list[i++];                                                   // nb de prises
  i++;
  secabilite      = "1";
  if      (formStruct.find(". séc")!=-1)          secabilite = "2";
  else if (formStruct.find("quadrisécable")!=-1)  secabilite = "4";
  else if (formStruct.find("sécable")!=-1)        secabilite = "2";
  return codePD;
}

//----------------------------------------------------- Datasemp_GetSecabilite ---------------------------------------------------------------------------
QString    CMedicaBase::Datasemp_GetSecabilite(const QString &cip,
                                                     QString  pkt4b             // = QString::null
                                              )
{ QString secabilite("1");
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)    return secabilite;
  //................... si accelerateurs de recherche non renseignés, les renseigner ..........................
  //                    ultra lent !!!!
  if ( pkt4b.length()==0 )
     {QString requete("SELECT ");


      // 0  code produit    ___
      //                       |
      requete +=             "f0 FROM t00 WHERE f2 = '" + cip + "'";
      QSqlQuery query(requete , m_DataBaseSrce );
      if (! (query.isActive() && query.next()))                             return secabilite;
      Datasemp_GetField("t4B", "f0",  Utf8_Query(query,0), "f2", &pkt4b);
     }
  QStringList list;
  Datasemp_GetT4B( list, pkt4b);
  QString formStruct      = Datasemp_GetField("t25", "f0", list[T4B_F1_FORME], "f1");                // forme structuree


  if      (formStruct.find(". séc")!=-1)          secabilite = "2";
  else if (formStruct.find("quadrisécable")!=-1)  secabilite = "4";
  else if (formStruct.find("sécable")!=-1)        secabilite = "2";
  return secabilite;
}

//----------------------------------------------------- Datasemp_GetFicheEconomique ---------------------------------------------------------------------------
void    CMedicaBase::Datasemp_GetFicheEconomique(const QString &code_CIP,
                                                 QString &prixstr,
                                                 QString &labo,
                                                 QString &liste,
                                                 QString &tauxss ,
                                                 QString  codeUV            // = QString::null
                                                )
{ if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)  return ;
  prixstr = labo = tauxss = liste = "";
  //................... si accelerateurs de recherche non renseignés, les renseigner ..........................
  //                    ultra lent !!!!
  if (codeUV.length()==0)
     {QString requete("SELECT ");

      // 1  Code unite de vente _____
      //                            |
      //                           |
      requete +=                " f1  FROM t00 WHERE f2 = '" + code_CIP + "'";
      QSqlQuery query(requete , m_DataBaseSrce );
      if (! (query.isActive() && query.next()))                             return ;
      codeUV = query.value(0).toString();;
     }
  prixstr         = Datasemp_GetField("t01", "f0",  codeUV,           "f3");   // SELECT fieldToGet FROM table WHERE testField = testValue
  labo            = Datasemp_GetField("t02", "f0",  codeUV,           "f1");   // SELECT fieldToGet FROM table WHERE testField = testValue obtenir code fabriquant
  labo            = Datasemp_GetField("t17", "f0",  labo,             "f2");   // SELECT fieldToGet FROM table WHERE testField = testValue obtenir libelle nom fabriquant
  tauxss          = Datasemp_GetField("t03", "f0",  codeUV,           "f2");   // SELECT fieldToGet FROM table WHERE testField = testValue obtenir code remboursement
  tauxss          = Datasemp_GetField("t1D", "f0",  tauxss,           "f1");   // SELECT fieldToGet FROM table WHERE testField = testValue obtenir libelle remboursement
  liste           = Datasemp_GetField("t03", "f0",  codeUV,           "f3");   // SELECT fieldToGet FROM table WHERE testField = testValue obtenir liste
}

//----------------------------------------------------- Datasemp_CIP_To_CodeProduit ---------------------------------------------------------------------------
QString CMedicaBase::Datasemp_CIP_To_CodeProduit(const QString &cip)
{ if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)  return QString::null;
  QString requete("SELECT ");
  // 0  code produit    ___
  //                       |
  requete +=             "f0  FROM t00 WHERE f2 = '" + cip + "'";
  QSqlQuery query(requete , m_DataBaseSrce );
  if (query.isActive() && query.next())    return query.value(0).toString();;
  return QString::null;
}

//----------------------------------------------------- Datasemp_IsExistClassTherapSempChild ---------------------------------------------------------------
void CMedicaBase::Datasemp_IsExistClassTherapSempChild(QListView *pQListView, const QString &codeParent , QListViewItem *pQListViewParentItem)
{QListViewItem *pQListViewItem;

 if (pQListViewParentItem==0) pQListViewItem  =  new QListViewItem(  pQListView );
 else                         pQListViewItem  =  new QListViewItem(  pQListViewParentItem );
 pQListViewItem->setText(0, Datasemp_GetLibelleClassTherapSemp(codeParent));
 pQListViewItem->setText(1, codeParent);

 QSqlQuery query (QString("SELECT f1 FROM t4G WHERE f0='") + codeParent + "'", m_DataBaseSrce );
 if (query.isActive() )
    {while (query.next()) Datasemp_IsExistClassTherapSempChild(pQListView,  Utf8_Query(query,0), pQListViewItem);  // récursivité
    }
}

//-----------------------------------------------------  Datasemp_GetLibelleClassTherapSemp -------------------------------------------
QString CMedicaBase::Datasemp_GetLibelleClassTherapSemp( const QString &code)
{ QSqlQuery query (QString("SELECT f1 FROM t4F WHERE f0='") + code + "'", m_DataBaseSrce );
  if (query.isActive() && query.next()) return Utf8_Query(query,0);
  return QString("");
}

//----------------------------------------------------- Datasemp_FillQListView_ClassTherapSemp ---------------------------------------------------------------
void CMedicaBase::Datasemp_FillQListView_ClassTherapSemp(QListView *pQListView )
{
 // SELECT t4B.f0,t4B.f2 FROM t4H INNER join t4B on t4H.f0=t4B.f0 where t4H.f1='C00024'

 //........................... Si cache des codes root (commencent par R) existe .........................
 //                            s'en servir
 QString rootCode("");
 QSqlQuery query ("SELECT f0 FROM t4G WHERE f0 LIKE 'R%'", m_DataBaseSrce );
 if (query.isActive() )
    {while (query.next())
       {rootCode = Utf8_Query(query,0);
        Datasemp_IsExistClassTherapSempChild(pQListView, rootCode.replace("R","C"));
       }
    }
 if (rootCode.length())                                                  return;            // si cache OK cassos
 //........................... Si pas de cache des codes root (commencent par R)  .........................
 //                            le créer
 /*................................ requete simple (qui tue).......................................
  //                                 3 fois plus lent que ma routine sous jacente
  //                                 qui éliminine les redondances (lastCode)

   QSqlQuery query ("SELECT t4F.f0, t4F.f1 FROM `t4F` left join t4G on t4F.f0 = t4G.f1 where (t4G.f1 Is Null)", m_DataBaseSrce );
   if (query.isActive() )
      {//.......................... pour chaque code de la table ..................................
       //              si pas présent en tant qu'enfant c'est qu'il est une racine
       while (query.next())
        {Datasemp_IsExistClassTherapSempChild(pQListView, query.value(0).toString());
        }
      }
 */
 QString lastCode ="";
 query.exec("SELECT f0 FROM t4G");
 if (query.isActive() )
    {//.......................... pour chaque code de la table ..................................
     //              si pas présent en tant qu'enfant (dans t4G.f1) c'est qu'il est une racine
     //              on en profite pour rajouter les root codes à la table (debutent avec un R)
     //              afin d'accélérer les prochaines fois !!!!
     while (query.next())
        {if (lastCode != query.value(0).toString())
            {QSqlQuery query2 (QString("SELECT f0 FROM t4G WHERE f1='")+ query.value(0).toString() + "'", m_DataBaseSrce );
             if (!(query2.isActive() && query2.next()))
                {rootCode = query.value(0).toString();
                 Datasemp_IsExistClassTherapSempChild(pQListView, rootCode);                  // renseigner la list box avec les classes filles
                 Datasemp_AddRootClassTherapSempCode(rootCode.replace("C","R"));              // renseigner la table avec cache de root codes
                }
             lastCode = query.value(0).toString();
            }
        } // end while (query.next())
    } //end if (query.isActive() )
}

//----------------------------------------------------- Datasemp_AddRootClassTherapSempCode -----------------------------------------------------------------------------------
void CMedicaBase::Datasemp_AddRootClassTherapSempCode(const QString &code)
{ //............... créer un curseur SQL ...................................................................
  QSqlCursor cur("t4G", TRUE, m_DataBaseSrce);
  if ( cur.canInsert ())
     {//............... si OK on recupere le buffer et on le charge avec les données .......................
      QSqlRecord *buffer = cur.primeInsert();     // recuperer le buffer d'insertion (a ne pas killer en fin de routine!!)
      buffer->setValue( "f0",       code);        // y placer les données
      if (!cur.insert()) OutSQL_error(cur, "Datasemp_AddRootClassTherapSempCode(): Error cur.insert", "");   // ecrire le buffer
     }
  else
    {OutSQL_error(cur, "Datasemp_AddRootClassTherapSempCode(): Error cur.canInsert", "");
    }
}
//-----------------------------------------------------  Datasemp_GetMedicamentListBySubstance -------------------------------------------
long CMedicaBase::Datasemp_GetMedicamentListBySubstance( QListView *pQlistView,
                                                         const QString  &code_sbce,
                                                         const QString  &isGene , // = ""
                                                         const QString  &isDispo  // = ""
                                                       )
{ long       nb(0);
  //................. Preparer la requete .....................................
  if ( m_MedicaBase==0 )                                                 return 0;
  if (m_MedicaBase->isOpen()==FALSE   && m_MedicaBase->open()==FALSE)    return 0;
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)  return 0;
  pQlistView->clear();
  QString requete       = "SELECT f0 FROM t07 WHERE f2='" ;       // t07.f0 = code_produit
          requete      +=  code_sbce + "'";                       // t07.f2 = code substance
  QSqlQuery query (requete , m_DataBaseSrce );
  if (query.isActive())
     {while (query.next())
            {
              requete       = "SELECT "; // LibelleMed, CdProd, Pk, Pkt4B, CodeCIP, Code_UV FROM VidalMedList WHERE CdProd='";
              requete      += m_MEDICA_INDEX_LIBELLE  + "," +
                              m_MEDICA_INDEX_CD_PROD  + "," +
                              m_MEDICA_INDEX_PK_T00   + "," +
                              m_MEDICA_INDEX_PK_T4B   + "," +
                              m_MEDICA_INDEX_CIP      + "," +
                              m_MEDICA_INDEX_CD_UV    + "," +
                              m_MEDICA_INDEX_IS_GENE  + "," +
                              m_MEDICA_INDEX_IS_DISPO + " " +
                              " FROM "    + m_MEDICA_INDEX_MED_TBL_NAME +
                              " WHERE "   + m_MEDICA_INDEX_CD_PROD      +  "='" + query.value(0).toString()  + "'  ";
              if (isGene !="" && isDispo != "")
                 {requete +=  "  AND "  +   m_MEDICA_INDEX_IS_DISPO + "='" + isDispo +
                              "' AND (" +   m_MEDICA_INDEX_IS_GENE  + "='G' OR "     + m_MEDICA_INDEX_IS_GENE + "='R')";
                 }
              else  if (isGene !="")
                 {requete +=  "  AND (" +   m_MEDICA_INDEX_IS_GENE  + "='G' OR "     + m_MEDICA_INDEX_IS_GENE + "='R')";
                 }
              else  if (isDispo !="")
                 {requete +=  "  AND "  +   m_MEDICA_INDEX_IS_DISPO + "='" + isDispo + "' ";
                 }
              nb += Medica_GetProductsListFrom_SQL_String(pQlistView, requete , CMedicaBase::NoClearList);
            } //endwhile (pSqlQueryCodeSbce->next())
     }//endif (pSqlQueryCodeSbce && pSqlQueryCodeSbce->isActive())
  return nb;
}

//-----------------------------------------------------  Datasemp_GetMedicamentListByCodeIndic -------------------------------------------
long CMedicaBase::Datasemp_GetMedicamentListByCodeIndic( QListView      *pQlistView,
                                                         const QString  &code_Indic,
                                                         const QString  &isGene , // = ""
                                                         const QString  &isDispo  // = ""
                                                       )
{ long       nb(0);
  //................. Preparer la requete .....................................
  if ( m_MedicaBase==0 )                                                 return 0;
  if (m_MedicaBase->isOpen()==FALSE   && m_MedicaBase->open()==FALSE)    return 0;
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)  return 0;
  pQlistView->clear();
  //         code du terme evoqué  ___________________
  //         code produit           |                 |
  //                                |                 |
  QString  requete       = "SELECT f1 FROM t52 WHERE f0='" ;               // selectionner dans t51 tous les termes de la table 50 liés au groupe
           requete      +=  code_Indic + "' AND f2>=2993 AND f2<=2995";    // 2992="voir aussi"    2993="groupe d'indications" 2994="indications" 2995="synonyme"
  QSqlQuery query (requete , m_DataBaseSrce );
  if (query.isActive())
     {while (query.next())
            { requete       = "SELECT "; // LibelleMed, CdProd, Pk, Pkt4B, CodeCIP, Code_UV FROM VidalMedList WHERE CdProd='";
              requete      += m_MEDICA_INDEX_LIBELLE  + "," +
                              m_MEDICA_INDEX_CD_PROD  + "," +
                              m_MEDICA_INDEX_PK_T00   + "," +
                              m_MEDICA_INDEX_PK_T4B   + "," +
                              m_MEDICA_INDEX_CIP      + "," +
                              m_MEDICA_INDEX_CD_UV    + "," +
                              m_MEDICA_INDEX_IS_GENE  + "," +
                              m_MEDICA_INDEX_IS_DISPO + " " +
                              " FROM "    + m_MEDICA_INDEX_MED_TBL_NAME +
                              " WHERE "   + m_MEDICA_INDEX_CD_PROD      +  "='" + query.value(0).toString()  + "'  ";
              if (isGene !="" && isDispo != "")
                 {requete +=  "  AND "  +   m_MEDICA_INDEX_IS_DISPO + "='" + isDispo +
                              "' AND (" +   m_MEDICA_INDEX_IS_GENE  + "='G' OR "     + m_MEDICA_INDEX_IS_GENE + "='R')";
                 }
              else  if (isGene !="")
                 {requete +=  "  AND (" +   m_MEDICA_INDEX_IS_GENE  + "='G' OR "     + m_MEDICA_INDEX_IS_GENE + "='R')";
                 }
              else  if (isDispo !="")
                 {requete +=  "  AND "  +   m_MEDICA_INDEX_IS_DISPO + "='" + isDispo + "' ";
                 }
              nb += Medica_GetProductsListFrom_SQL_String(pQlistView, requete , CMedicaBase::NoClearList);
            } //endwhile (pSqlQueryCodeSbce->next())
     }//endif (pSqlQueryCodeSbce && pSqlQueryCodeSbce->isActive())
  return nb;
}

//----------------------------------------------------- Datasemp_GetMedicamentListByClassTheraVidal ---------------------------------------------------------------------------
long CMedicaBase::Datasemp_GetMedicamentListByClassTheraVidal(      QListView *pQlistView ,                   // listview resultat des produits
                                                                    QListViewItem *pQListViewItem,            // listViewItem du code selectionné dans la listview classification
                                                                    const QString &name,
                                                                    const QString &isGene , // = ""
                                                                    const QString &isDispo, // = ""
                                                                    QProgressBar *pQProgressBar  // = 0
                                                             )
{
  long progress(0);
  long       nb(0);
  if ( m_MedicaBase==0 )                                                 return 0;
  if (m_MedicaBase->isOpen()==FALSE   && m_MedicaBase->open()==FALSE)    return 0;
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)  return 0;

  //................ explorer tout l'arbre pour en extraire les codes des produits sous jacents ................
  QStringList list;
  Datasemp_GetAllClassTherapSemp_ChildCode(pQListViewItem , list );                 // 41 secondes
  // Datasemp_GetAllClassTherapSemp_ChildCode(list, pQListViewItem->text(1));       // 43 secondes
  pQlistView->clear();
  if (list.size()<=0) return 0;
  if (pQProgressBar) pQProgressBar->setTotalSteps(list.size()); // 2326

  //................. Preparer la requete .................................................................
  QString requete("SELECT ");
  requete  += m_MEDICA_INDEX_LIBELLE  + "," +
              m_MEDICA_INDEX_CD_PROD  + "," +
              m_MEDICA_INDEX_PK_T00   + "," +
              m_MEDICA_INDEX_PK_T4B   + "," +
              m_MEDICA_INDEX_CIP      + "," +
              m_MEDICA_INDEX_CD_UV    + "," +
              m_MEDICA_INDEX_IS_GENE  + "," +
              m_MEDICA_INDEX_IS_DISPO + " " +
              " FROM "    + m_MEDICA_INDEX_MED_TBL_NAME +
              " WHERE ("  + m_MEDICA_INDEX_MED_TBL_NAME +"."+m_MEDICA_INDEX_LIBELLE + " LIKE \"" + name          + "%\" OR "
                          + m_MEDICA_INDEX_MED_TBL_NAME +"."+m_MEDICA_INDEX_LIBELLE + " LIKE \"" + name.upper()  + "%\" );";
  if (isGene !="" && isDispo != "")
      {requete +=  "  AND  " +   m_MEDICA_INDEX_MED_TBL_NAME+"."+m_MEDICA_INDEX_IS_DISPO + "='" + isDispo +
                   "' AND (" +   m_MEDICA_INDEX_MED_TBL_NAME+"."+m_MEDICA_INDEX_IS_GENE  + "='G' OR "+  m_MEDICA_INDEX_MED_TBL_NAME+"."+m_MEDICA_INDEX_IS_GENE + "='R')";
      }
  else  if (isGene !="")
      {requete +=  " AND (" +  m_MEDICA_INDEX_MED_TBL_NAME+"."+m_MEDICA_INDEX_IS_GENE  + "='G' OR "+  m_MEDICA_INDEX_MED_TBL_NAME+"."+m_MEDICA_INDEX_IS_GENE + "='R' )";
      }
  else  if (isDispo !="")
      {requete +=  " AND " +    m_MEDICA_INDEX_MED_TBL_NAME+"."+ m_MEDICA_INDEX_IS_DISPO + "='" + isDispo + "' ";
      }

  //......................... exploitation de la requete .................................................
  QSqlQuery query(requete , m_MedicaBase );
  if (query.isActive())
     {
      while (query.next() )
        {if (pQProgressBar) pQProgressBar->setProgress(++progress);
         if (list.findIndex(query.value(1).toString()) != -1) // si code produit dans liste de la classe therapeutique
            {QListViewItem *element = new  QListViewItem( pQlistView);
               if (element)
                  {QString is_Gene    = Utf8_Query(query, 6     );
                   QString is_Dispo   = Utf8_Query(query, 7     );
                   element->setText (0, Utf8_Query(query, 0     ));     // libelle
                   element->setText (1, Utf8_Query(query, 1     ));     // code produit
                   element->setText (2, Utf8_Query(query, 2     ));     // PrimaryKey t00  (et aussi pk de cette table)
                   element->setText (3, Utf8_Query(query, 3     ));     // PrimaryKey t4B
                   element->setText (4, Utf8_Query(query, 4     ));     // CodeCIP
                   element->setText (5, Utf8_Query(query, 5     ));     // Code_UV
                   element->setText (6, is_Gene);                             // IsGene
                   element->setText (7, is_Dispo);                            // IsDispo
                   element->setText (8, QString::number(nb++));

                   if      (is_Dispo != "0")    element->setPixmap ( 0, QPixmap::fromMimeSource( "supprimes.png" ) );
                   else if (is_Gene == "G")     element->setPixmap ( 0, QPixmap::fromMimeSource( "generiques.png" ) );
                   else if (is_Gene == "R")     element->setPixmap ( 0, QPixmap::fromMimeSource( "referent.png" ) );
                   else                         element->setPixmap ( 0, QPixmap::fromMimeSource( "standards.png" ) );
                  }
            }
        } //end while (pSqlQuery->next())
      }
 return nb;
}

//----------------------------------------------------- Datasemp_GetAllClassTherapSemp_ChildCode ---------------------------------------------------------------
// ACTION: fonction recursive de recherche par classe therapeutique vidal
//         cette fonction se fonde sur l'exploration de l'arbre de la QListView (dependante de l'interface) mais un peu plus rapide
//         que sa soeur qui se fonde sur une requete SQL (independante de l'interface)
void CMedicaBase::Datasemp_GetAllClassTherapSemp_ChildCode(QListViewItem *pQListViewItem, QStringList &list)
{//.................... relever tous les produits correspondant à ce code .....................................
 QSqlQuery query (QString("SELECT t4B.f0 FROM t4H INNER join t4B on t4H.f0=t4B.f0 where t4H.f1='") + pQListViewItem->text(1) +"'", m_DataBaseSrce);
 if (query.isActive() )
    {while (query.next())list.append( Utf8_Query(query,0));
    }
 //....................... si enfants descendre .................................................................
 QListViewItem *childItem = pQListViewItem->firstChild ();
 while (childItem)
    {Datasemp_GetAllClassTherapSemp_ChildCode(childItem, list);   // récursivité
     childItem = childItem->nextSibling ();                       // au voisin
    }
}

//----------------------------------------------------- Datasemp_GetAllClassTherapSemp_ChildCode ---------------------------------------------------------------
// ACTION: fonction recursive de recherche par classe therapeutique vidal
//         cette fonction se fonde sur  une requete SQL (independante de l'interface)  mais un peu moins rapide
//         que sa soeur qui se fonde sur l'exploration de l'arbre de la QListView (dependante de l'interface)
void CMedicaBase::Datasemp_GetAllClassTherapSemp_ChildCode(QStringList &list, const QString &codeParent)
{//.................... relever tous les produits correspondant à ce code .....................................
 QSqlQuery query (QString("SELECT t4B.f0 FROM t4H INNER join t4B on t4H.f0=t4B.f0 where t4H.f1='") + codeParent +"'", m_DataBaseSrce);
 if (query.isActive() )
    {while (query.next())list.append( Utf8_Query(query,0));
    }
 //....................... si enfants descendre .................................................................
 query.exec(QString("SELECT f1 FROM t4G WHERE f0='") + codeParent + "'");
 if (query.isActive() )
    {while (query.next()) Datasemp_GetAllClassTherapSemp_ChildCode(list,  Utf8_Query(query,0));  // récursivité
    }
}

//----------------------------------------------------- Datasemp_GetCodesClassTherapVidal ---------------------------------------------------------------------------
long CMedicaBase::Datasemp_GetCodesClassTherapVidal(const QString &codeProduit,
                                                    QStringList   &codesList)
{long nb = 0;
 if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)  return 0;

 codesList.clear();
 QSqlQuery query (QString("SELECT f1 FROM t4H WHERE f0='") + codeProduit + "'", m_DataBaseSrce);
 if (query.isActive() )
    {while (query.next())
        {codesList.append( query.value(0).toString());
         ++nb;
        }
    }
 return nb;
}

//----------------------------------------------------- Datasemp_SetQListViewOnClassTherapVidal ---------------------------------------------------------------------------
void CMedicaBase::Datasemp_SetQListViewOnClassTherapVidal( QListView *pQListView , QStringList &codesList, int ListMustBeClosed /* = 1 */)
{if (ListMustBeClosed)
    {QListViewItemIterator it( pQListView );
     while ( it.current() )
        {QListViewItem *item = it.current();
         ++it;
         if ( item->isOpen())  item->setOpen (FALSE );
        }
    }

 QListViewItem *pQListViewItem = 0;
 for ( QStringList::Iterator it = codesList.begin(); it != codesList.end(); ++it )
     {pQListViewItem = pQListView->findItem(*it, 1);
      if (pQListViewItem != 0)
         {QListViewItem *pQListViewItemParent = pQListViewItem->parent();
          while (pQListViewItemParent!=0)
                {pQListView->setOpen (pQListViewItemParent, TRUE );
                 pQListViewItemParent = pQListViewItemParent->parent();
                }
         }
     }
 if (pQListViewItem != 0)
    {pQListView->ensureItemVisible (pQListViewItem);
     pQListView->setSelected(pQListViewItem, TRUE);
    }
}

//-----------------------------------------------------  Datasemp_GetGenericState -------------------------------------------
QString   CMedicaBase::Datasemp_GetGenericState(const QString &codeProduit, int f)
{ if (f && m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE) return  QString::null;
  QSqlQuery query (QString("SELECT f2 FROM t3F WHERE f0 = '") + codeProduit +"'" , m_DataBaseSrce );

  //.................. si la requète a un resultat ..............................................
  if (query.isActive() && query.next())
     {int typ = query.value(0).toInt();
      if (typ&1) return QString("R");
      if (typ&2) return QString("G");
      if (typ&4) return QString("GA");
     }
  return QString("");
}

//-----------------------------------------------------  Datasemp_GetDisponibiliteState -------------------------------------------
QString   CMedicaBase::Datasemp_GetDisponibiliteState(const QString &codeUV, int f)
{ if (f && m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)                               return  QString::null;
  QSqlQuery query (QString("SELECT f3 FROM t02 WHERE f0 = '") + codeUV +"'" , m_DataBaseSrce );
  //.................. si la requète a un resultat ..............................................
  if (query.isActive() && query.next())                                                               return Utf8_Query(query,0);
                                                                                                      return QString("");
}

//-----------------------------------------------------  Datasemp_GetGenericRefState -------------------------------------------
QString   CMedicaBase::Datasemp_GetGenericRefState(const QString &codeProduit)
{ if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)                               return  QString::null;
  QSqlQuery query (QString("SELECT f1 FROM t3F WHERE f1 = '") + codeProduit + "'" , m_DataBaseSrce );
  //.................. si la requète a un resultat ..............................................
  if (query.isActive() && query.next())                                                               return Utf8_Query(query,0);
                                                                                                      return QString::null;
}

//-----------------------------------------------------  Datasemp_GetT4B -------------------------------------------
void CMedicaBase::Datasemp_GetT4B(QStringList &qstringList, QString pk)
{
  QString    requete ("");
 //................. Preparer la requete .....................................
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)return ;


  requete       += "SELECT f0,f1,f2,f4,f5,f6,f7,f8,f9,fA,fB,fC,fP FROM t4B WHERE ge = " + pk ;

  QSqlQuery query(requete , m_DataBaseSrce );

  //.................. si la requète a un resultat ..............................................
  if (query.isActive() && query.next())
     {qstringList.clear();
      qstringList.append(Utf8_Query(query, 0     ) );   // f0 Code produit
      qstringList.append(Utf8_Query(query, 1     ) );   // f1 Forme ou nature
      qstringList.append(Utf8_Query(query, 2     ) );   // f2 intitulé complet
      qstringList.append(Utf8_Query(query, 3     ) );   // f4 Classe d'interactions
      qstringList.append(Utf8_Query(query, 4     ) );   // f5 Fiche de caractéristiques
      qstringList.append(Utf8_Query(query, 5     ) );   // f6 Classe chimique
      qstringList.append(Utf8_Query(query, 6     ) );   // f7 Famille SEMP
      qstringList.append(Utf8_Query(query, 7     ) );   // f8 Intitulé abrége
      qstringList.append(Utf8_Query(query, 8     ) );   // f9 Qualificatifs cliniques
      qstringList.append(Utf8_Query(query, 9     ) );   // fA Classe ATC
      qstringList.append(Utf8_Query(query, 10    ) );   // fB Code dosage
      qstringList.append(Utf8_Query(query, 11    ) );   // fC Date de mise à jour de la fiche de caractéristiques
      qstringList.append(Utf8_Query(query, 12    ) );   // fP Flags pour les produits "abstraits
     } //endif (pSqlQuery && pSqlQuery->isActive())
}

//-----------------------------------------------------  Datasemp_GetT4B_by_CodeProduit -------------------------------------------
void CMedicaBase::Datasemp_GetT4B_by_CodeProduit(QStringList &qstringList, QString codeProduit)
{
  QString    requete ("");
 //................. Preparer la requete .....................................
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)return ;


  requete       += "SELECT f0,f1,f2,f4,f5,f6,f7,f8,f9,fA,fB,fC,fP FROM t4B WHERE f0 = " + codeProduit ;

  QSqlQuery query(requete , m_DataBaseSrce );

  //.................. si la requète a un resultat ..............................................
  if (query.isActive() && query.next())
     {qstringList.clear();
      qstringList.append(Utf8_Query(query, 0     ) );   // f0 Code produit
      qstringList.append(Utf8_Query(query, 1     ) );   // f1 Forme ou nature
      qstringList.append(Utf8_Query(query, 2     ) );   // f2 intitulé complet
      qstringList.append(Utf8_Query(query, 3     ) );   // f4 Classe d'interactions
      qstringList.append(Utf8_Query(query, 4     ) );   // f5 Fiche de caractéristiques
      qstringList.append(Utf8_Query(query, 5     ) );   // f6 Classe chimique
      qstringList.append(Utf8_Query(query, 6     ) );   // f7 Famille SEMP
      qstringList.append(Utf8_Query(query, 7     ) );   // f8 Intitulé abrége
      qstringList.append(Utf8_Query(query, 8     ) );   // f9 Qualificatifs cliniques
      qstringList.append(Utf8_Query(query, 9     ) );   // fA Classe ATC
      qstringList.append(Utf8_Query(query, 10    ) );   // fB Code dosage
      qstringList.append(Utf8_Query(query, 11    ) );   // fC Date de mise à jour de la fiche de caractéristiques
      qstringList.append(Utf8_Query(query, 12    ) );   // fP Flags pour les produits "abstraits
     } //endif (pSqlQuery && pSqlQuery->isActive())
}

//-----------------------------------------------------  Datasemp_GetT00 -------------------------------------------
void CMedicaBase::Datasemp_GetT00(QStringList &qstringList, QString pk)
{
  //................. Preparer la requete .....................................
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)              return ;
  qstringList.clear();
  QSqlQuery query(QString("SELECT f7,f8,fJ FROM t00 WHERE ge = '") + pk +"'" , m_DataBaseSrce );
  int i = 0;
  //.................. si la requète a un resultat ..............................................
  if (query.isActive() && query.next())
     {qstringList.append(Utf8_Query(query, i++     ));   // f7 code unite de prise
      qstringList.append(Utf8_Query(query, i++     ));   // f8 nbr unite de prise
      qstringList.append(Utf8_Query(query, i++     ));   // fj code forme structurée
     } //endif (pSqlQuery && pSqlQuery->isActive())
}

//-----------------------------------------------------  Datasemp_GetDosage -------------------------------------------
QString CMedicaBase::Datasemp_GetDosage( const QString &codeDosage)
{QString ret(TR("indéterminée"));
 if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)               return ret;
 QSqlQuery query(QString("SELECT f1 FROM t1C WHERE f0 = ") + codeDosage, m_DataBaseSrce );
 //.................. si la requète a un resultat ..............................................
 if (query.isActive() && query.next())
    {ret = Utf8_Query(query, 0 ).stripWhiteSpace();
     if       (ret.length()==0)   ret = TR("indéterminée");
    }
 return ret;
}

//-----------------------------------------------------  Datasemp_GetDosage -------------------------------------------
QString CMedicaBase::Datasemp_GetDosage( const QString &codeDosage, const QString &code_prod, QString *pa_qu /* = 0 */, QString *pa_unit /* = 0 */, QString *substance /* = 0 */)
{ QString ret(TR("indéterminée"));
  //................. cassos si base non dispo .....................................
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)               return ret;
  //................... si  codeDosage non renseigné essayer avec ..................
  //                    t07 et code produit
  if (codeDosage.length()==0 || codeDosage=="0")
    {return Datasemp_GetDosageT07(code_prod, 1, pa_qu , pa_unit ,substance );
    }
  else
    {if (pa_qu)       *pa_qu     = "";
     if (pa_unit)     *pa_unit   = "";
     if (substance)   *substance = "";

     //................. Preparer la requete .....................................
      QSqlQuery query(QString("SELECT f1 FROM t1C WHERE f0 = ") + codeDosage, m_DataBaseSrce );
      //.................. si la requète a un resultat ..............................................
      if (query.isActive() && query.next())
         {ret = query.value( 0 ).toString().stripWhiteSpace();
          if       (ret.length()==0)   ret = TR("indéterminée");
          else
             {if (pa_qu && pa_unit)
                 {*pa_qu      = "1";
                  *pa_unit    = ret;
                  //....... separer bon grain de l'ivraie ........................
                  if (ret[0]>='0' && ret[0]<='9' || ret[0]==',')
                     {int pos = 0;
                      while (ret[pos]>='0' && ret[pos]<='9' || ret[pos]==',') ++pos;
                      *pa_qu   = ret.left(pos).replace(",",".");
                      *pa_unit = ret.mid(pos);
                     }
                  }
              if (substance)
                 {query.exec(QString("SELECT f2, f1, f4, f5 FROM t07 WHERE f0 ='") + code_prod + "'" );     // recuperer code substance de la monographie
                  if (query.isActive() )
                     {while (query.next())
                          {int code_sect = query.value(1).toInt();
                           if (code_sect == 0)  //................ si subce principale noter code ...................................
                              {QString code_subst(query.value(0).toString());
                               if (substance->length())*substance  += ", ";
                               *substance  +=   Datasemp_GetField("t27", "f0", code_subst , "f2").upper()       + " ";     // principe actif
                              }
                          } // endwhile (query.next())
                     } //endif (query.isActive() )
                 } // endif (substance)
              } // end else if       (ret.length()==0)
         }//endif (query.isActive() && query.next())
    }
  return ret;
}

//-----------------------------------------------------  Datasemp_GetDosageT07 -------------------------------------------
QString CMedicaBase::Datasemp_GetDosageT07( const QString &code_prod, int mode,  QString *pa_quIn /* = 0 */, QString *pa_unitIn /* = 0 */, QString *substanceIn /* = 0 */)
{ QString pa_qu("");
  QString pa_unit("");
  QString substance("");

  //................. cassos si base non dispo .....................................
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)               return pa_qu;
  //................... si  codeDosage non renseigné essayer avec ..................
  //                    t07 et code produit

     //                  3  Code unite de la Quté__
     //                  2  Quantité de sbce___    |
     //                  1  Code Section__     |   |
     //                  0  Code Sbce     |    |   |
     //                               |   |    |   |
     QSqlQuery query(QString("SELECT f2, f1,  f4,  f5 FROM t07 WHERE f0 ='") + code_prod + "'" , m_DataBaseSrce);     // recuperer code substance de la monographie
     if (query.isActive() )
        {while (query.next())
            {int code_sect = query.value(1).toInt();
             if (code_sect == 0)  //................ si subce principale noter code ...................................
                {QString code_subst(query.value(0).toString());
                 if (substance.length()) substance  += ", ";
                 substance                          += Datasemp_GetField("t27", "f0", code_subst , "f2").upper()       + " ";     // principe actif
                 if (pa_qu.length())  pa_qu  += ", ";
                 pa_qu                       += query.value(2).toString();

                 if (pa_unit.length())  pa_unit  += ", ";
                 pa_unit                         += Datasemp_GetField("t24", "f0", Utf8_Query(query,3), "f1") + " ";     // unité de mesure
                }
             if (mode) break;
            }
        }

  if (pa_quIn)       *pa_quIn     = pa_qu;
  if (pa_unitIn)     *pa_unitIn   = pa_unit;
  if (substanceIn)   *substanceIn = substance;
  return pa_qu +" ; "+pa_unit;
}

//-----------------------------------------------------  GetPA08 -------------------------------------------
QString   CMedicaBase::Datasemp_GetPA07(QString code_prod)
{ QString              requete ("");
  QString      principesActifs ("");
  //...................... verifier si bases accessibles .............................
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)   return QString::null;
  //................. chercher code substance pour ce produit...................................................
  //................. chercher code substance pour ce produit...................................................
  //                       ATTENTION: tous les produits n'ont pas forcement une entrée
  //                                  dans cette table --> Infos non disponibles et c'est regrettable
  //     3  Code unite de la Quté__
  //     2  Quantité de sbce___    |
  //     1  Code Section__     |   |
  //     0  Code Sbce     |    |   |
  //                  |   |    |   |
  requete  += "SELECT f2, f1, f4, f5 FROM t07 WHERE f0 ='" + code_prod + "'" ;     // recuperer code substance de la monographie
  QSqlQuery query(requete , m_DataBaseSrce );
  if (query.isActive() )
     {while (query.next())
            {int code_sect = query.value(1).toInt();
             if (code_sect == 0)  //................ si subce principale noter code ...................................
                {QString code_subst(query.value(0).toString());
                 principesActifs +=   Datasemp_GetField("t27", "f0", code_subst , "f2").upper()       + " ";     // principe actif
                 principesActifs +=   query.value(2).toString()                              + " ";              // quantité
                 principesActifs +=   Datasemp_GetField("t24", "f0", query.value(3).toString(), "f1") + " ";     // unité de mesure
                }
            }
     }
  return principesActifs;
}

//-----------------------------------------------------  Datasemp_GetSubtancesListByCodeProd -------------------------------------------
long   CMedicaBase::Datasemp_GetSubtancesListByCodeProd( QListView *pQlistView , const QString &code_prod )
{ QString              requete ("");
  long                      nb (0);
  if (m_DataBaseSrce==0) return 0;
  //...................... verifier si bases accessibles .............................
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)   return 0;
  //................. chercher code substance pour ce produit...................................................
  pQlistView->clear();
  //                       ATTENTION: tous les produits n'ont pas forcement une entrée
  //                                  dans cette table --> Infos non disponibles et c'est regrettable
  //     1  Code Section__
  //     0  Code Sbce      |
  //                  |    |
  requete  += "SELECT f2, f1 FROM t07 WHERE f0 ='" + code_prod + "'" ;     // recuperer code substance de la monographie
  QSqlQuery query(requete , m_DataBaseSrce );
  if (query.isActive() )
     {
      while (query.next())
            {int code_sect = query.value(1).toInt();
             if (code_sect == 0)  //................ si subce principale noter code ...................................
                {QString code_subst(query.value(0).toString());
                 new QListViewItem( pQlistView,
                                    Datasemp_GetField("t27", "f0", code_subst , "f2"),     // libelle
                                    code_subst
                                  );
                 ++nb;
                }
            }
     }
  return nb;
}

//-----------------------------------------------------  Datasemp_GetIndicationsListByCodeProd -------------------------------------------
long   CMedicaBase::Datasemp_GetIndicationsListByCodeProd(QListView *pQlistView , const QString &code_prod)
{ QString              requete ("");
  long                      nb (0);
  if (m_DataBaseSrce==0) return 0;
  //...................... verifier si bases accessibles .............................
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)   return 0;
  //................. lister codes indications pour ce produit...................................................
  pQlistView->clear();
  // code  produit        ___________________
  // codedu terme evoqué   |                 |
  //                       |                 |
  requete      += "SELECT f0 FROM t52 WHERE f1='" ;             // selectionner dans t51 tous les termes de la table 50 liés au groupe
  requete      +=  code_prod + "' AND f2>=2993 AND f2<=2995";    // 2992="voir aussi"    2993="groupe d'indications" 2994="indications" 2995="synonyme"
  QSqlQuery query(requete , m_DataBaseSrce );
  if (query.isActive() )
     {
      while (query.next())
            {QString code_indic(query.value(0).toString());
             new QListViewItem( pQlistView,
                                Datasemp_GetField("t50", "f0", code_indic , "f1"),     // f1 = libelle
                                code_indic
                              );
             ++nb;
            }
     }
  return nb;
}

//-----------------------------------------------------  GetSubtanceList -------------------------------------------
long   CMedicaBase::Datasemp_GetSubtanceList(const QString &code_prod, QStringList &list)
{ QString              requete ("");
  long                      nb (0);
  //...................... verifier si bases accessibles .............................
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)   return 0;
  //................. chercher code substance pour ce produit...................................................
  //                       ATTENTION: tous les produits n'ont pas forcement une entrée
  //                                  dans cette table --> Infos non disponibles et c'est regrettable
  //     3  Code unite de la Quté__
  //     2  Quantité de sbce___    |
  //     1  Code Section__     |   |
  //     0  Code Sbce     |    |   |
  //                  |   |    |   |
  requete  += "SELECT f2, f1, f4, f5 FROM t07 WHERE f0 ='" + code_prod + "'" ;     // recuperer code substance de la monographie
  QSqlQuery query(requete , m_DataBaseSrce );
  if (query.isActive() )
     {while (query.next())
            {int code_sect = query.value(1).toInt();
             if (code_sect == 0)  //................ si subce principale noter code ...................................
                {QString code_subst(query.value(0).toString());
                 list.append (Datasemp_GetField("t27", "f0", code_subst , "f2"));//       + "[?] " +     // principe actif
                              //query.value(1)                                          + "[?] " +     // code section
                              //code_subst
                             //);
                 ++nb;
                }
            }
     }
  return nb;
}

//----------------------------------------------- GetDaily ---------------------------------------------------
void    CMedicaBase::Datasemp_GetDaily(const QString &atc,             // SELECT fieldToGet FROM table WHERE testField = testValue
                              const QString &code_voie,
                                    QString &dailyDose,
                                    QString &dailyUnit,
                                    QString &substance)
{
  dailyDose = "";
  dailyUnit = "";
  substance = "";
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE) return;
  QString requete("SELECT ");
  // 2  Substance dosée (27.0) ____
  // 1  Code Unit Dosage (24.0)    |
  // 0  Dose Qotidienne ___    |   |
  //                       |   |   |
  requete +=             "f2, f3, f4 FROM t43 WHERE f0 = '" + atc + "' AND f1 = '" + code_voie + "'";
  QSqlQuery* pSqlQuery = new QSqlQuery(requete , m_DataBaseSrce );
  if (pSqlQuery && pSqlQuery->isActive() && pSqlQuery->next())
     {dailyDose = pSqlQuery->value(0).toString();
      dailyUnit = Datasemp_GetField("t24", "f0", pSqlQuery->value(1).toString(), "f1");
      substance = Datasemp_GetField("t27", "f0", pSqlQuery->value(2).toString(), "f2");
     }

  if ( pSqlQuery) delete  pSqlQuery;
}

//----------------------------------------------- Datasemp_GetField ---------------------------------------------------
QString CMedicaBase::Datasemp_GetField(const char* table,             // SELECT fieldToGet FROM table WHERE testField = testValue
                                       const char *testField,
                                       const char *testValue,
                                       const char *fieldToGet,
                                       QString    *pk /* = 0 */)
{ QString result = "";
  int          i = 0;
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE) return QString::null;

  //................. SI un nom ou prenom est donné en entrée alors filtrer selon ...................
  QString requete = "SELECT ";
  if (pk) requete = requete + fieldToGet + ", ge FROM " + table + " WHERE " + testField  + "= \"" + testValue + "\"";
  else    requete = requete + fieldToGet + " FROM "     + table + " WHERE " + testField  + "= \"" + testValue + "\"";
  QSqlQuery query(requete , m_DataBaseSrce );
  if (query.isActive())
     {result = "";
      while (query.next())
         {if (i)
             {result = result   + "|@|\n";     // s�arer par |@| si plusieurs valeurs et r�onses retourn�s
              if (pk) *pk = *pk + "|@|\n";     // s�arer par |@| si plusieurs valeurs et r�onses retourn�s
             }
          result += Utf8_Query(query,0);
          if (pk) *pk = Utf8_Query(query,1);
          ++i;
         }
     }
  return result;
}

//-----------------------------------------------------  Datasemp_SetHTMLFontProperty -------------------------------------------
void CMedicaBase::Datasemp_SetHTMLFontProperty(const QString &family,
                                               const QString &titleSize,
                                               const QString &chapitreSize,
                                               const QString &paragraphSize,
                                               const QString &normalText)
{  m_FontFamily         = family;
   m_FontTitleSize      = titleSize;
   m_FontchapitreSize   = chapitreSize;
   m_FontparagraphSize  = paragraphSize;
   m_FontnormalTextSize = normalText;
}

//-----------------------------------------------------  Datasemp_GetMonographieByCIP -------------------------------------------
QString    CMedicaBase::Datasemp_GetMonographieByCIP(const QString &code_CIP)
{ if (m_DataBaseSrce==0)    return QString("");
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE)  return QString::null;
  QString pkt4B("");
  QString requete("");
  // 2  PrimaryKey ________________
  // 1  Code unite de vente ___    |
  // 0  code produit    ___    |   |
  //                       |   |   |
  requete +=       "SELECT f0, f1, ge FROM t00 WHERE f2 = '" + code_CIP + "'";
  QSqlQuery query(requete , m_DataBaseSrce );
  if (! (query.isActive() && query.next()))                             return QString::null;
  QString codePD ( query.value(0).toString() );
  QString codeUV ( query.value(1).toString() );
  Datasemp_GetField("t4B", "f0",  codePD, "f2", &pkt4B);
  return Datasemp_GetMonographie( codeUV, pkt4B );
}

//-----------------------------------------------------  Datasemp_GetMonographie -------------------------------------------
//                  la norme vidal est que les noms des images des mononographies sont en majuscules : src="IMAGES/B00/C01/D35.GIF"
QString   CMedicaBase::Datasemp_GetMonographie(const QString &code_UV, const QString &pkt4B)
{ QString  name("");
  QString  codeMonographie("");
  QString  result("");
  int f = -1;
  if (m_DataBaseSrce==0)    return result;
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE) return result;

  //................. chercher si existe une  monographie vidal pour ce produit...................................................
  //                              recuperer code document de la monographie
  /*
  QSqlQuery query (QString("SELECT f1 FROM t5D WHERE f0 ='") + code_UV + "'" ,          m_DataBaseSrce );
  if (query.isActive() &&query.next()) codeMonographie    = query.value(0).toString();

  query.exec(QString("SELECT f3 FROM t5C WHERE f0 ='") + codeMonographie + "'");
*/
  QSqlQuery query (QString( " SELECT t5C.f3    "
                            " FROM   t5D, t5C  "
                            " WHERE  t5C.f0 = t5D.f1"
                            " AND    t5D.f0 = '%1'").arg( code_UV ) ,          m_DataBaseSrce );

  if (query.isActive() &&query.next())         result     =  query.value(0).toString();
  if ( (f=result.find("<html",0,FALSE)) > 0)   result.remove (0, f );
  if (result != "") return result;

  //................. si pas de monographie vidal pour ce produit...................................................
  //                  creer une page html à partir des infos XML

  query.exec(QString("SELECT f5, f2 FROM t4B WHERE ge =") + pkt4B );
  if (query.isActive() && query.next())
     {result = query.value(0).toString();
      name   = query.value(1).toString();
     }
  if (result!="")
     {Datasemp_Xml_to_html(result, name);
     }
  return result;
}

//-------------------------------------- Datasemp_Xml_to_html -------------------------------------------------------
void CMedicaBase::Datasemp_Xml_to_html( QString& input,
                                        QString  name  // = QString::null
                                      )
{/*
 StructureParser  handler;
 StructureLexical lexical;
 QXmlInputSource source;
 source.setData(input);
 QXmlSimpleReader reader;
 reader.setContentHandler( &handler );
 reader.setLexicalHandler( &lexical );
 reader.parse( &source );
 */
 m_XmlDepth      = -1;
 //................ convertir les retours chariot en trucs non XML (ne doivent pas etres interprétés) ................
 input.replace("<BR/>","{{@}}");
 QDomDocument doc( "mydocument" );
 if ( !doc.setContent( input ) ) return;
 // print out the element names of all elements that are direct children
 // of the outermost element.

    input =  "<html><head><meta name=\"qrichtext\" content=\"1\" /></head>"
             "<body style=\"font-size:16pt;font-family:Arial\">";

    if (name != QString::null)
       {input += "<p align=\"center\"><span style=\"font-family:"+m_FontFamily+";font-size:"+m_FontTitleSize+"pt;font-style:italic;font-weight:600;color:#AA0000\">";
        input += name  +  "</span></p> <hr size=\"2\" />";
       }
    QDomElement docElem = doc.documentElement();
    Datasemp_Xml_ListChild(input, doc.documentElement() );

    input     += "</body></html>";
    input.replace(QString::fromUtf8 (""),"\'");
}

//-------------------------------------- Datasemp_Xml_ListChild -------------------------------------------------------
void CMedicaBase::Datasemp_Xml_ListChild(QString& input, const QDomElement &parentElement )
{   ++m_XmlDepth;
    QString ofset = "";
    for (int i=0; i<m_XmlDepth; i++) ofset += "\t";
    QDomNode node = parentElement.firstChild();
    while ( !node.isNull() )
       {if ( node.isElement() /* && node.nodeName() == "outline" */)
           {QDomElement e   = node.toElement();
            QString text    = e.text();
            QString tagName = e.tagName();
            QString title   = e.attribute("TITRE");
            int         tag = 1;
            if (tagName=="RUBRIQUE")
               {input +=  ofset + "<br /><span style=\"font-family:"+m_FontFamily+";font-size:"+m_FontTitleSize+"pt;font-weight:600;text-decoration:underline;color:#AA0000\">";
                input +=  title + "</span><br />";
               }
            else if (tagName=="SSRUBR")
               {title.replace(">", "&gt;");
                title.replace("<", "&lt;");
                input += ofset + "<span style=\"font-family:"+m_FontFamily+";font-size:"+m_FontchapitreSize+"pt;font-weight:600;text-decoration:underline;color:#000000\">";
                input += title + "</span><br />";
               }
            else if (tagName=="CAS")
               {title.replace(">", "&gt;");
                title.replace("<", "&lt;");
                input += ofset + "<span style=\"font-family:"+m_FontFamily+";font-size:"+m_FontparagraphSize+"pt;font-weight:600;color:#0000FF\">";
                input += title + "</span><br />";
               }
            else tag=0;
            if (tag && text!="")
               {text.replace(">", "&gt;");
                text.replace("<", "&lt;");
                text.replace("{{@}}", " <br>" + ofset); // replacer les retours chariots normaux
                input += ofset + "<span style=\"font-family:"+m_FontFamily+";font-size:"+m_FontnormalTextSize+"pt;color:#00007F\">";
                input += text  + "</span><br />";
               }
            //........... APPEL RECURSIF ...........
            Datasemp_Xml_ListChild( input, node.toElement() );
           }
        node = node.nextSibling();
       }
    --m_XmlDepth;
}

//----------------------------------------------------- Datasemp_ToMedicaTux -----------------------------------------------------------
long CMedicaBase::Datasemp_ToMedicaTux(QProgressBar *pQProgressBar)
{ if (pQProgressBar) pQProgressBar->setTotalSteps(15400); // 103237  15445
  m_Progress = 0;
  m_Run      = 1;
  if ( m_MedicaBase==0 )                                                         return 0;
  if (m_MedicaBase->isOpen()==FALSE && m_MedicaBase->open()==FALSE)              return 0;
  m_FormeUPList.clear();
 //........................................creer la table de specifications ................................................
 if (Medica_CreateTableSpecificationProduit(m_MEDICA_SPEC_TBL_NAME)==FALSE)      return 0;
 //................. SCANNER L'INDEX SRC....................................................................................

 QString  requeteSrc  = "SELECT ";
 requeteSrc +=  m_MEDICA_INDEX_LIBELLE + "," +         // 0
                m_MEDICA_INDEX_CIP     + "," +         // 1
                m_MEDICA_INDEX_PK_T00  + "," +         // 2
                m_MEDICA_INDEX_PK_T4B  + "," +         // 3
                m_MEDICA_INDEX_CD_PROD + "," +         // 4
                m_MEDICA_INDEX_CD_UV   + " " +         // 5
                " FROM " + m_MEDICA_INDEX_MED_TBL_NAME;
  QSqlQuery* pSqlQuery = new QSqlQuery(requeteSrc , m_MedicaBase );
  //................ scanner les enregistrements ................................................
  int nb = 0;
  //.................. si la requète a un resultat ..............................................
  if (pSqlQuery && pSqlQuery->isActive())
     {
      while (pSqlQuery->next() && m_Run) // &&nb < 400)
        {    QString cip, up_unit, pa_unit, pa_qu, secabilite, is_generic, formStruct, nbPrises, prixBoite, remboursementSS, tableau, labo, atc;
             ++m_Progress;
             if (pQProgressBar) pQProgressBar->setProgress(m_Progress);
             cip = pSqlQuery->value( 1 ).toString();
             if (//MysteriousBaseGetSpec(pSqlQuery->value( 2 ).toString(),    // PK_T00
                 //                      pSqlQuery->value( 3 ).toString(),    // PK_T4B
                 //                         cip, up_unit, pa_unit, pa_qu, secabilite, is_generic, formStruct, nbPrises, prixBoite, remboursementSS, tableau, labo, atc)
                 Datasemp_GetSpecifications(cip, up_unit, pa_unit, pa_qu, secabilite, is_generic, formStruct, nbPrises, prixBoite, remboursementSS, tableau, labo, atc,             // ok
                                            pSqlQuery->value( 2 ).toString(),            // PK_T00  = QString::null
                                            pSqlQuery->value( 3 ).toString(),            // PK_T4B  = QString::null
                                            pSqlQuery->value( 4 ).toString(),            // CD_PROD = QString::null
                                            pSqlQuery->value( 5 ).toString()             // CD_UV = QString::null
                                           )
                )
                {Medica_InsertNewSpec(cip, up_unit, pa_unit, pa_qu, secabilite, is_generic, formStruct, nbPrises, prixBoite, remboursementSS, tableau, labo, atc);
                }
             nb++;
        } //end while (pSqlQuery->next())
     } //endif (pSqlQuery && pSqlQuery->isActive())
  OutSQL_error( *pSqlQuery, "Datasemp_ToMedicaTux()", requeteSrc);
  if (pSqlQuery) delete  pSqlQuery;

  Medica_CreateTableFormeProduit(m_MEDICA_FORME_UP_TBL_NAME, m_FormeUPList);
  Medica_CreateTableFormeProduit(m_MEDICA_FORME_PA_TBL_NAME, m_FormePAList);
  Medica_CreateTableFormeProduit(m_MEDICA_FORME_ST_TBL_NAME, m_FormeSTList);
  //..................... placer le numero de version du datasemp au niveau de l'index ..........................
  QString numEdition;
  Datasemp_GetBaseVersion( &numEdition, 0, 0);
  Medica_SetDatasempVersion(numEdition, CMedicaBase::Data);
  return nb;
}

//----------------------------------------------- Datasemp_GetNbRecord ---------------------------------------------------
QString CMedicaBase::Datasemp_GetNbRecord(const QString table)
{ QString result = "";
  if (m_DataBaseSrce->isOpen()==FALSE && m_DataBaseSrce->open()==FALSE) return QString::null;

  //................. SI un nom ou prenom est donné en entrée alors filtrer selon ...................
  QString requete = QString("SELECT COUNT(*) FROM ") + table;
  QSqlQuery query(requete , m_DataBaseSrce );
  if (query.isActive() && query.next())
     {result = query.value(0).toString();
     }
  return result;
}

//-------------------------------------------------- OutSQL_error -----------------------------------------------
int CMedicaBase::OutSQL_error( const QSqlQuery &cur, const char *messFunc /*=0*/, const char *requete /*=0*/,  QString *ret /*=0*/)
{    QSqlError error = cur.lastError();
     QString   qserr("");

     if (error.type() != QSqlError::None)
        {switch (error.type())
           { case    QSqlError::None:           qserr =" - no error occurred: "          ; break;
             case    QSqlError::Connection:     qserr =" - connection error: "           ; break;
             case    QSqlError::Statement:      qserr =" - SQL statement syntax error: " ; break;
             case    QSqlError::Transaction:    qserr =" - transaction failed error: "   ; break;
             case    QSqlError::Unknown:        qserr =" - unknown error: "              ; break;
             default:                           qserr =" - unknown error: "              ; break;
           }
         if (messFunc) qDebug(messFunc);
         qserr        += error.databaseText ();
         qDebug(qserr);
         if (requete) qDebug(requete);
         if (ret) *ret = TR("........Erreur SQL %1 : <br> requete en cause : %2<br>").arg( qserr, requete );
         return FALSE;
        }
     else return TRUE;
//     qDebug(qserr + error.databaseText ());
}

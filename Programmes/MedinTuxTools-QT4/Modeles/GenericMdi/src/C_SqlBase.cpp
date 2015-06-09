/*********************************************************************************
 *                                                                                *
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
 *   Commissariat a l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Federation, 75752 PARIS cedex 15.   *
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

#include "C_SqlBase.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "DataBase.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"

//------------------------ C_SqlBase --------------------------------------------------
C_SqlBase::C_SqlBase(QObject *parent)
    : C_BaseCommon(parent)
{
}
//------------------------ C_SqlBase --------------------------------------------------
QMap <QString,QString> C_SqlBase::getGlobalSettings(const QString &section, const QString &key)
{ QMap <QString,QString> retMap;
  if (section.length()==0)   { outMessage(tr(" C_SqlBase::getGlobalSettings() empty section parameter "));   return retMap;}
  if ( ! database().open() ) { outMessage(tr(" C_SqlBase::getGlobalSettings() database can't be opened' ")); return retMap;}

  QString             requete  = QString (" SELECT "  m_GLOBAL_SETTINGS_KEY       ","        m_GLOBAL_SETTINGS_VALUE
                                          " FROM   "  m_GLOBAL_SETTINGS_TBL_NAME  " WHERE "  m_GLOBAL_SETTINGS_SECTION  "=  '%1'").arg(section);
  if ( key.length() ) requete += QString (" AND "     m_GLOBAL_SETTINGS_KEY       "=  '%1'").arg(key);

  QSqlQuery query( QString::null , database() );
  if ( !query.exec(requete) )
     { outMessage (  tr("\nERROR  : C_SqlBase::getGlobalSettings() \n%1\nREQUEST : %2").arg(query.lastError().text(), requete));
       return retMap;
     }
  while ( query.next() )  retMap[ query.value(0).toString() ] = CGestIni::Utf8_Query(query, 1);
  return retMap;
}

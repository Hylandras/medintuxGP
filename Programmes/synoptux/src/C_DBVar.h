 /********************************* C_DBVar.h ********************************
 *  #include "C_DBVar.h"                                                     *
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
 *   Commissariat �  l'Energie Atomique                                            *
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

// C++ Interface: C_DBVar.h


#ifndef C_DBVar_H
#define C_DBVar_H

#include <QString>
#include <QObject>

#define TR  QObject::tr
//=============================================================== C_DBVar ===========================================================
class C_DBVar
{
public:
    C_DBVar( QString& confData );
    QString       getVersionNumber();
    int           initValues( QString& confData);
    static char  *SetConfBase_SetProperties(char *pt, QString &propertie, const char *token, int *line, QString &err);
    int           areFieldsNamesValids() {return (m_LastError.length()==0);}
    QString       getLastError()         {return  m_LastError;}
public:
QString m_VERSION_NUMBER;               // numero de version de la base de donnee
QString m_BO_TBL_NAME;
QString m_BO_PK;
QString m_BO_LIBELLE;
QString m_BO_COLOR_BG;
QString m_BO_COLOR_TITLE;
QString m_BO_NB_MAXI_PAT;
QString m_BO_TYPE;

QString m_DE_TBL_NAME;
QString m_DE_PK;
QString m_DE_DESTINATION;
QString m_DE_OUT_TYPE;

QString m_CUR_TBL_NAME;
QString m_CUR_PK;
QString m_CUR_REF_BOX_PK;
QString m_CUR_NOM_MED;
QString m_CUR_NOM_PAT;
QString m_CUR_PRENOM_PAT;
QString m_CUR_H_ENTREE;
QString m_CUR_H_SORTIE;
QString m_CUR_GUID_PAT;
QString m_CUR_REPLIER;
QString m_CUR_PROG_ANNEXE;
QString m_CUR_ARGS_ANNEXE;
QString m_CUR_DESTINATION;
QString m_CUR_ANONYME;
QString m_CUR_NOTE;

QString m_CUR_TASK_TBL_NAME;
QString m_CUR_TASK_PK;
QString m_CUR_TASK_REF_CUR_PK;
QString m_CUR_TASK_CODE;
QString m_CUR_TASK_NOTE;
QString m_CUR_TASK_STATE_CUR;
QString m_CUR_TASK_H_START_EXPECT;
QString m_CUR_TASK_H_END_EXPECT;
QString m_CUR_TASK_H_START;
QString m_CUR_TASK_H_END;
QString m_CUR_TASK_PRIOR;
QString m_CUR_TASK_COMMENT;
QString m_CUR_TASK_AGENT;

QString m_STATUS_TBL_NAME;
QString m_STATUS_PK;
QString m_STATUS_LIBELLE;
QString m_STATUS_COLOR;
QString m_STATUS_FINISHED;

QString m_TASK_STATUS_TBL_NAME;
QString m_TASK_STATUS_CODE;
QString m_TASK_STATUS_STATE;

QString m_TRACE_TBL_NAME;
QString m_TRACE_PK;
QString m_TRACE_REF_BOX_PK;
QString m_TRACE_NOM_PAT;
QString m_TRACE_PRENOM_PAT;
QString m_TRACE_SIGNER;
QString m_TRACE_TASK_CODE;
QString m_TRACE_DATETIME;
QString m_TRACE_LIBELLE;
QString m_TRACE_ACTION;
QString m_TRACE_NUM_CUR;
QString m_TRACE_LIBELLE_STATUS;
QString m_TRACE_COMMENT;
QString m_TRACE_AGENT;

QString m_SIGNER_TBL_NAME;
QString m_SIGNER_PK;
QString m_SIGNER_CODE;
QString m_SIGNER_NOM;
QString m_SIGNER_DROITS;
QString m_SIGNER_COLOR;

QString m_TASK_TBL_NAME;
QString m_TASK_PK;
QString m_TASK_LIBELLE;
QString m_TASK_COLOR;
QString m_TASK_MAX_DURATION;
QString m_TASK_ALARM_COLOR;
QString m_TASK_MENU_BUTTON;

private:
   QString      m_LastError;
};

#endif

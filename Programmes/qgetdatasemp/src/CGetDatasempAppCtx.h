/********************************* CGetDatasempAppCtx.h ***************************
 *                                                                                *
 *                                                                                *
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

#ifndef CGETDATASEMPAPP_CTX_H
#define CGETDATASEMPAPP_CTX_H


#include <qdir.h>
#include <qobject.h>
#include "../../MedinTuxTools/CGestIni.h"

class QString;

//================================================= CGetDatasempAppCtx ==============================================================
class CGetDatasempAppCtx
{

public:
     CGetDatasempAppCtx(int & argc,     char ** argv, const QString &name = "");
    ~CGetDatasempAppCtx();

public:
    QString              m_PathAppli;
    QString              m_PathRessources;
    QString              m_NameAppli;
    QString              m_PathExchg;
    QString              m_PathLog;
    QString              m_Command;
    QString              m_PathParent;
    QString              m_GUID;
    QString              m_Display;
    QString              m_ParamData;
    QString              m_ParamPath;
    int                  m_IsPlugin;


};

#endif


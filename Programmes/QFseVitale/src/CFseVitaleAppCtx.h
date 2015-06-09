/********************************* FseVitaleAppCtx.h ******************************
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

#ifndef CFSEAPP_CTX_H
#define CFSEAPP_CTX_H


#include <qdir.h>
#include <qobject.h>

class QString;

//================================================= CFseVitaleAppCtx ==============================================================
class CFseVitaleAppCtx 
{           
    
public:
     CFseVitaleAppCtx(int & argc,     char ** argv, const QString &name = "");
    ~CFseVitaleAppCtx(); 
     
public:
    QString              m_PathAppli;
    QString              m_PathImages;
    QString              m_NameAppli; 
    QString              m_PathExchg; 
    QString              m_PathParent;
    QString              m_PathIni;
    QString              m_LocalParam;
    QString              m_GUID;
    QString              m_Command;
    QString              m_Display;
    QString              m_PathLog;
    QString              m_PathGalss;
    QString              m_CodePorteur;
    QString              m_RessPS;
    QString              m_RessLecteur;
    QString              m_DateConsult;
    int                  m_IsPlugin;

};

#endif


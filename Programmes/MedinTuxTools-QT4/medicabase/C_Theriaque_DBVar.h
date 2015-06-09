 /**************************** C_Theriaque_DBVar.h ********************************
 *  #include "C_Theriaque_DBVar.h"                                                *
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
 *   Commissariat a  l'Energie Atomique                                           *
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

// C++ Interface: C_Theriaque_DBVar.h


#ifndef C_Theriaque_DBVar_H
#define C_Theriaque_DBVar_H


#include <QString>
#include <QObject>

//=============================================================== C_Theriaque_DBVar ===========================================================
class C_Theriaque_DBVar
  {
  public:
    C_Theriaque_DBVar();
    int           initValues( QString& confData);
    QString       getLastError()         {return  m_LastError;}
  protected:
    //........................ MEDICA_TUX  Table index des produits .........................................
    //                         la structure d' l'index est commune
    //
    QString  m_TH_SPECIALITE_TBL_NAME;            // nom  table des specialites
    QString  m_TH_PRESENTATION_TBL_NAME;          // nom  table des presentations

  private:
    static char  *SetConfBase_SetProperties(char *pt, QString &propertie, const char *token, int *line, QString &err);
    int           areFieldsNamesValids() {return (m_LastError.length()==0);}

    QString      m_LastError;
  };

#endif

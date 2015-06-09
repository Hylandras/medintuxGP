/********************************* CMoteurGlo.h ***********************************
 *                                                                                *
 *   #include "CMoteurGlo.h"                                                    *
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
 *   Commissariat �l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la F��ation, 75752 PARIS cedex 15.   *
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

#ifndef  CMOTEUR_GLO_H
#define  CMOTEUR_GLO_H

#include "Global.h"
#include "Rubriques.h"

//=============================================== INCLUDES ==========================================================
#include <qapplication.h>
#include <qvaluelist.h>
#include <qsqldatabase.h>
#include <qstring.h>
#include <qlistview.h>
#include <qlabel.h>
#include <qdir.h>
#include <qpixmap.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtextedit.h>

#include "../../MedinTuxTools/CW_EditText.h"
//===================================== CMoteurGlo =================================================================
class CMoteurGlo
{

 public:
   CMoteurGlo();
   long GetMasquesList(  QListView *pQlistView , const char* folder_name);
   void SetTextEditWithData(   QString file ,  CW_EditText *pQTextEdit);
   int  SetListTyp( int typ , const QString &path, QListView *pQlistView =0  );
   int  PathGlossaireToTyp();
   QDir     m_QDir;
   QString  m_curPath;
   int      m_RubType;

  enum ItemType { File = 0, Dir, Link};

};

#endif


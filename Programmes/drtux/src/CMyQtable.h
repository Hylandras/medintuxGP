/********************************* CMyQTable.h ***********************************
 *                                                                                *
 *   #include "CMyQTable.h"                                                       *
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


#ifndef CMYQTABLE_H
#define CMYQTABLE_H

#include <qwidget.h>
#include <qtable.h> 
#include <MyButton.h> 
#include <qlabel.h> 

class CMyButton;

//====================================== CMyQTable =======================================================
class CMyQTable : public QTable
{
Q_OBJECT  

//........................................... METHODES ...........................................................   
public:
     CMyQTable  ( int numRows, 
                  int numCols, 
		  QWidget *  parent = 0, 
		  const char * name = 0	
		);

     CMyQTable (QWidget*         parent           = 0, 
                const char*      name             = 0
	       );

void AddModele (const char* modele );

public slots:      
 void Slot_ContextMenuRequested ( int row, int col, const QPoint & pos );
 void Slot_Clicked ( int row, int col, int button, const QPoint & mousePos );
 void Slot_ValueChanged ( int row, int col);
signals:
 void Sign_ContextMenuRequested(CMyQTable *pCMyQTable, const char* name, int row, int col, const QPoint & pos );
 void Sign_Clicked(CMyQTable *pCMyQTable, const char* name, int row, int col, int button, const QPoint & mousePos );
 void Sign_ValueChanged ( int row, int col,  CMyQTable *pCMyQTable);
//........................................... DATA ...............................................................
public:
 QString     m_name;
 QStringList m_ModeleList;
 CMyButton*  m_ButtonNew;
 CMyButton*  m_ButtonTools;
 QLabel*     m_Title;
private:
};

#endif






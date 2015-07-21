/********************************* CMyTable.cpp ***********************************
 *                                                                                *
 *   #include "CMyTable.h"                                                        *
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

 #include "CMyQtable.h"
//---------------------------------------- CMyQTable -------------------------------------------------------------
 CMyQTable::CMyQTable  ( int numRows,
			int numCols,
			QWidget    *parent ,    /* = 0  */
			const char *name        /* = 0  */
                      )
        : QTable (numRows, numCols, parent, name)   // important permet que le widget soit enfant du parent
 {m_name            = name;
  m_ButtonNew       = 0;
  m_ButtonTools     = 0;
  m_Title           = 0;
  m_ModeleList.clear();
  connect( this, SIGNAL( contextMenuRequested      ( int , int , const QPoint&  ) )  ,
	   this, SLOT(   Slot_ContextMenuRequested ( int , int , const QPoint&  ) )
         );

  connect( this, SIGNAL(  clicked     ( int , int , int , const QPoint&  ) )  ,
	   this, SLOT(   Slot_Clicked ( int , int , int , const QPoint&  ) )
         );

  connect( this, SIGNAL(  valueChanged ( int , int  ) )  ,
	   this, SLOT(   Slot_ValueChanged (int , int  ) )
         );
 }



 //---------------------------------------- CMyQTable -------------------------------------------------------------
 CMyQTable::CMyQTable (QWidget*         parent      ,   /*  = 0 */
                       const char*      name            /*  = 0 */
	              )
         : QTable ( parent, name)   // important permet que le widget soit enfant du parent
 {m_name            = name;
  m_ButtonNew       = 0;
  m_ButtonTools     = 0;
  m_Title           = 0;
  m_ModeleList.clear();
  connect( this, SIGNAL( contextMenuRequested      ( int , int , const QPoint&  ) )  ,
	   this, SLOT(   Slot_ContextMenuRequested ( int , int , const QPoint&  ) )
         );
  connect( this, SIGNAL(  clicked     ( int , int , int , const QPoint&  ) )  ,
	   this, SLOT(   Slot_Clicked ( int , int , int , const QPoint&  ) )
         );
  connect( this, SIGNAL(  valueChanged ( int , int  ) )  ,
	   this, SLOT(   Slot_ValueChanged (int , int  ) )
         );
 }



//---------------------------------------- AddModele ----------------------------------------------
 void CMyQTable::AddModele (const char* modele )
 {m_ModeleList.append(modele);
 }

//---------------------------------------- Slot_ValueChanged ----------------------------------------------
 void CMyQTable::Slot_ValueChanged ( int row, int col)
 {emit Sign_ValueChanged(row,  col, this);
 }

//---------------------------------------- Slot_Clicked ----------------------------------------------
 void CMyQTable::Slot_Clicked ( int row, int col, int button, const QPoint & mousePos )
 {emit Sign_Clicked(this, m_name,  row,  col,  button,  mousePos);
 }

 //---------------------------------------- Slot_ContextMenuRequested ----------------------------------------------
 void CMyQTable::Slot_ContextMenuRequested ( int row, int col, const QPoint & pos )
 {emit Sign_ContextMenuRequested(this, m_name,  row,  col, pos );
 }


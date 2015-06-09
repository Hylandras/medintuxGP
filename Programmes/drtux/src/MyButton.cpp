/********************************* MyButton.cpp ***********************************
 *                                                                                *
 *   #include "MyButton.h"                                                        *
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

#include "MyButton.h"





//--------------------------------- CMyButton ------------------------------------------------------------------------------

CMyButton::CMyButton (QWidget*         parent          , /* = 0 */

                      const char*      name            , /* = 0 */ 	

		      void*            ptr               /* = 0 */

		     )

    : QPushButton ( parent, name)   // important permet que le widget soit enfant du parent

 {setText(name);

  m_ptr  = ptr;

  m_Name = name;

  connect( this, SIGNAL( clicked() ), this, SLOT( Slot_OnButtonClicked() ) );

 }





//--------------------------------- Slot_OnButtonClicked ------------------------------------------------------------------------------

void CMyButton::Slot_OnButtonClicked()

{

 if (m_ptr==0) emit Sign_ButtonClicked(m_Name);

 else          emit Sign_ButtonClickedPtr(m_Name, m_ptr);

}





     


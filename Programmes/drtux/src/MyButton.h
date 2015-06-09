/********************************** MyButton.h ************************************
 *                                                                                *
 *   #include "MyButton.h"                                                        *
 *   Project: MedinTux    and Data Medical Design                                 *
 *   Copyright (C) 2003 2004 2005 by Sébastien SIVERA & Sevin Roland              *
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

#ifndef CMYBUTTON_H

#define CMYBUTTON_H





#include <qwidget.h>

#include <qpushbutton.h>



//====================================== CMyButton =======================================================

class CMyButton : public QPushButton

{



Q_OBJECT



//........................................... METHODES ...........................................................

public:

     CMyButton (QWidget*         parent           = 0,
                const char*      name             = 0,
                void*            ptr              = 0
               );
    QString getName(){return m_Name;};

public slots:

 void Slot_OnButtonClicked();



signals:

 void Sign_ButtonClickedPtr(const char* name, void *ptr);
 void Sign_ButtonClicked(const char* name);

//........................................... DATA ...............................................................

private:

 void   *m_ptr;
 QString m_Name;

};



#endif






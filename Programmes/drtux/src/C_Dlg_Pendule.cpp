/********************************* C_Dlg_Pendule.cpp *************************

    Project: MedinTux
    Copyright (C) 2005 by Roland Sevin  and Data Medical Design
    E-Mail: roland-sevin@medintux.org

    This program is free software; you can redistribute it and/or modify
    it under the terms of the CeCILL Version 1 du 21/06/2004
    License (GPL compatible)
                               http://www.cecill.info/
    as published by :

    Commissariat � l'Energie Atomique
    - CEA,
                             31-33 rue de la F�d�ration, 75752 PARIS cedex 15.
                             FRANCE
    Centre National de la Recherche Scientifique
    - CNRS,
                             3 rue Michel-Ange, 75794 Paris cedex 16.
                             FRANCE
    Institut National de Recherche en Informatique et en Automatique
    - INRIA,
                             Domaine de Voluceau, Rocquencourt, BP 105, 78153
                             FRANCE

          This program is distributed in the hope that it will be useful,
          but WITHOUT ANY WARRANTY; without even the implied warranty of
          MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
          CeCILL  License (GPL compatible) for more details.

*/

#include <qlayout.h>
#include <math.h>
#include <qframe.h>
#include <qdatetimeedit.h>
#include <qdatetime.h>
#include <qpixmap.h>
#include <qlineedit.h>
#include "C_Dlg_Pendule.h"
#include "../../MedinTuxTools/Theme.h"          // Gestion du thème de l'appli
#include "../../MedinTuxTools/Calendar/C_Horloge.h"

//----------------------------------------------- C_Dlg_Pendule ------------------------------------------------------------------------
C_Dlg_Pendule::C_Dlg_Pendule(QWidget* parent, const char* name, bool modal, WFlags fl )
: Dlg_Pendule(parent,name, modal, fl)
{
 initDialog();
}

//----------------------------------------------- initDialog ---------------------------------------------------------------------------
void C_Dlg_Pendule::initDialog()
{   lineEdit_MinutesRef->hide();
     //.................... l'horloge ..................
     m_pC_Horloge = new C_Horloge(Theme::getPath(Theme::WithSeparator)+"Horloge/HorlogeCal.png", this, "horlogeCal" );
     if (m_pC_Horloge)
        {m_pC_Horloge->setFrameShape( QFrame::StyledPanel );
         m_pC_Horloge->setFrameShadow( QFrame::Raised );
         m_pC_Horloge->setCentralRefLabel(lineEdit_MinutesRef);
         connect( timeEdit_Temps ,    SIGNAL(valueChanged            ( const QTime &  )),   m_pC_Horloge,   SLOT( setTime(const QTime & ) ) );
         connect( m_pC_Horloge ,      SIGNAL(Sign_HorlogeTimeChanged ( const QTime &  )),   timeEdit_Temps, SLOT( setTime(const QTime & ) ) );
         m_pC_Horloge->move(0,0);
        }

    connect( pushButton_Ok ,     SIGNAL( clicked()),  this, SLOT( accept() ) );
    connect( pushButton_Exit ,   SIGNAL( clicked()),  this, SLOT( reject() ) );
  int w = m_pC_Horloge->width()+2;
  int h = m_pC_Horloge->height() + 30;
  //........ replacer les boutons ............
  Dlg_PenduleLayout->addWidget( m_pC_Horloge, 0, 0 );
  setMaximumSize(QSize(w, h));
  setMinimumSize(QSize(w, h));
  resize( QSize(w, h).expandedTo(minimumSizeHint()) );
  timeEdit_Temps->setTime( QTime::currentTime ());
}

//----------------------------------------------- ~C_Dlg_Pendule ------------------------------------------------------------------------
C_Dlg_Pendule::~C_Dlg_Pendule()
{
}

/*$SPECIALIZATION$*/
//----------------------------------------------- reject ------------------------------------------------------------------------
void C_Dlg_Pendule::reject()
{QDialog::reject();
}

//----------------------------------------------- accept ------------------------------------------------------------------------
void C_Dlg_Pendule::accept()
{QDialog::accept();
}




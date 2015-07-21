/********************************* C_Dlg_Pendule.h *************************

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


#ifndef C_DLG_PENDULE_H
#define C_DLG_PENDULE_H

#include <qpushbutton.h>
#include <qstring.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qrect.h>
#include <qdatetime.h>
#include <qdatetimeedit.h>

#include "ui/Dlg_Pendule.h"

class C_Horloge;
//===============================================================  C_PatientRootItem =========================================================================
/*! \class C_Dlg_Pendule
 *  \brief Classe d'affichage d'une saisie conviviale de l'heure
*/
class C_Dlg_Pendule : public Dlg_Pendule
{
  Q_OBJECT

public:
  C_Dlg_Pendule(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  void initDialog();
  ~C_Dlg_Pendule();
  /*$PUBLIC_FUNCTIONS$*/
  QTime getTime(){return timeEdit_Temps->time();};
  void  setTime(const QTime &t){timeEdit_Temps->setTime( t);};
protected:
  C_Horloge *m_pC_Horloge;

protected slots:
  /*$PROTECTED_SLOTS$*/
  virtual void          reject();
  virtual void          accept();
};

#endif


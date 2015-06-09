/********************************* C_Dlg_VigieFiltreConf.h *************************

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


#ifndef C_DLG_VIGIEFILTRECONF_H
#define C_DLG_VIGIEFILTRECONF_H

#include "ui/Dlg_VigieFiltreConf.h"
typedef QMap<QString, QString> OpClProfiles;
//====================================== C_Dlg_VigieFiltreConf =======================================================


/*! \class C_Dlg_VigieFiltreConf
 *  \brief Dialog class for Vigie's filter management
 */

class C_Dlg_VigieFiltreConf : public Dlg_VigieFiltreConf
{
  Q_OBJECT

public:
  C_Dlg_VigieFiltreConf(QString *pfilterName, QString *pfilter, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  ~C_Dlg_VigieFiltreConf();
  /*$PUBLIC_FUNCTIONS$*/

public slots:
  /*$PUBLIC_SLOTS$*/

protected:
  /*$PROTECTED_FUNCTIONS$*/
QString *m_pFilterName;
QString *m_pFilter;

protected slots:
  /*$PROTECTED_SLOTS$*/
  virtual void          reject();
  virtual void          accept();

};

#endif


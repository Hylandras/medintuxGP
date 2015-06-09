 /********************************* main.cpp ****************************************
 \file main.cpp
 \brief (put some short descripion here).

     Project: MedinTux
     Copyright (C) 2008 by
     by Sevin Roland  and Data Medical Design
     E-Mail: roland-sevin@medintux.org

 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat a l'Energie Atomique                                            *
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



#include <qmessagebox.h>
#include <qdialog.h>
#include "CApp.h"
#include "ui/Dlg_MedicaTux.h"

int main( int argc, char ** argv )
{    //......................... initialisation classique Application......................................
    CApp a(PROG_NAME, argc, argv);
    if (G_pCApp==0) return 0;

     Dlg_MedicaTux *dlg  = new Dlg_MedicaTux(0, "Therapeutique_Dial", TRUE, Qt::WStyle_Customize | Qt::WStyle_NormalBorder | Qt::WStyle_Title |Qt::WStyle_MinMax | Qt::WStyle_SysMenu);
     if (dlg ==0)                                                   return 0;
     if (dlg->initDialog(G_pCApp->m_pCMedicaBase, a.m_PathAppli + "Ressources") ==0)    { delete dlg; return 0; }
     a.setMainWidget(dlg);
     dlg->show();
     a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
     a.connect( &a, SIGNAL(messageReceived(const QString&)), dlg, SLOT(SlotMakeVisible(const QString&)));
     a.exec();
     delete dlg;
     return 0;

}

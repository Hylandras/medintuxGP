 /********************************* CMDI_Releve.h ************************************
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 2                     *
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
/*! \file CMDI_Releve.h
    \brief (put some short descripion here).

     #include "CMDI_Releve.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#ifndef CMDI_Releve_H
#define CMDI_Releve_H

#include <qobject.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qtable.h>
#include <qmessagebox.h>
#include <qwidget.h>
#include <qhbox.h>


#include "CMDI_Generic.h"
#include "ComptaMoteurBase.h"
#include "compta.h"
#include "CApp.h"
#include "Listes.h"
#include "ui/cpts_add.h"

/**
	@author Eric Maeker <eric.maeker@free.fr>
*/

class CMDI_Releve : public  CMDI_Generic
{
    Q_OBJECT
public:
    CMDI_Releve( QWidget* parent, const char* name,
		int wflags , ComptaMoteurBase *pComptaMoteurBase);
   ~CMDI_Releve();

public:

public slots:
	void	setTableTo		();
	void	getComboComptes		();
	void 	ActiverRubrique (const char *rubName);
    	void	popupTable( int , int , const QPoint& );

public:
	QPushButton*		btImprime;
	QTable*			tabRlv;
	QLineEdit*		nomUsr;
	COMPTES_LISTE*		m_CompteListe;
	MVTS_LISTE*		m_MvtsListe;
	MVTS_LISTE*		m_MvtsDispo;
	QComboBox*		m_Combo_Comptes;
	ComptaMoteurBase*	m_pComptaMoteurBase;
	QLineEdit*		totalDepenses;
	QLineEdit*		totalRecettes;
	QLineEdit*		soldeCompte;


};

#endif


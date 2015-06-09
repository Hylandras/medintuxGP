 /********************************* EspecesCalculator.h ***************************
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
/*! \file EspecesCalculator.h
    \brief (put some short descripion here).

     #include "EspecesCalculator.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#ifndef ESPECESCALCULATOR_H
#define ESPECESCALCULATOR_H
#include <qobject.h>
#include <qdialog.h>
#include <qwidget.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qcombobox.h>
#include <qhbox.h>
#include <qdatetime.h>
#include <qtable.h>



#include "Defines.h"
#include "CApp.h"
#include "Listes.h"

/**
	@author Eric Maeker <eric.maeker@free.fr>
*/


/*! \brief Fenêtre d'aide au calcul des espèces. */
class EspecesCalculator: public QDialog
{
    Q_OBJECT

public:
    EspecesCalculator( ComptaMoteurBase* pComptaMoteurBase, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );

    ~EspecesCalculator();


	QString		getForBlobDepot		();
	QString 	getDateObserv		();
	void		setDateObserv		(QString dt_str);
	void		setDateObserv		(QDate dt);
	void		getComboComptes		();

public slots:
	double 	calculTotalEspeces	();
	void 	saveDepotEspeces	();
	void 	mois_activated		(int id);
	void 	annee_activated		(int id);

public:
    // DONNEES
    QLineEdit* 		m_LineEdit_Total;
    QPushButton* 	butSauvDepot;
    QPushButton* 	butAnnule;
    QComboBox*	 	m_Combo_Jour;
    QComboBox*	 	m_Combo_Mois;
    QComboBox*	 	m_Combo_Annee;
    QComboBox*	 	m_Combo_ListeComptes;
    COMPTES_LISTE* 	m_Comptes_Liste;		/*!< Liste des comptes de l'utilisateur */
    ComptaMoteurBase* 	m_pComptaMoteurBase;
    QTable*	 	m_tabEsp;



};

#endif

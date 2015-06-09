 /********************************* Actes_ui.h ************************************
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
/*! \file Actes_ui.h

     #include "Actes_ui.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#ifndef ACTES_UI_H
#define ACTES_UI_H

#include <qobject.h>
#include <qwidget.h>
#include <qtable.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qstring.h>
#include <qhbox.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qcombobox.h>
#include <qdatetime.h>

#include "Listes.h"
#include "CApp.h"
#include "ui/acte_modif.h"

/**
	@author Eric Maeker <eric.maeker@free.fr>
*/
class Actes_Ui : public QWidget
{
Q_OBJECT
public:
    Actes_Ui ( ComptaMoteurBase* pComptaMoteurBase, QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

    ~Actes_Ui();

	void	findTypes		();
	void 	setTable		();


public slots:
	void	popupTableActes	(int, int, const QPoint& p);
	void	sauvActe	();
	void	ajouteActe	();
	void	viewTypeActe	(int id);
	void	voirActe	();
	void	modifierActe	();
	void	supprimerActe	();

signals:
	void	sign_Actes_Ui_ActesListeModif();


public:
	ComptaMoteurBase* m_pComptaMoteurBase;
	ACTES_LISTE*	m_pActes_Liste;		/*!< Pointeur qui contient tous les actes disponibles. */
	ACTES_LISTE*	m_pActes_ListeForTable;	/*!< Pointeur qui contient les actes à afficher dans le tableau. */
	QStringList	m_TypesActes;		/*!< Contient la liste des différents types d'actes à partir d'actesListe. */
	QTable*		m_pTableauActes;	/*!< Tableau des actes disponibles. */
	QPushButton*	btAjoute;
//	QPushButton*	btSauve;
	QComboBox*	m_Combo_ListeTypesActes;	/*!< Combo qui contient la liste des types d'acte enregistrée */
	QPopupMenu*	m_Popup_Table;		/*!< Popup du tableau */

};

#endif

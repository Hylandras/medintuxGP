 /********************************* Depots.h ************************************
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
/*! \file Depots.h
    \brief (put some short descripion here).

     #include "Depots.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#ifndef DEPOTS_H
#define DEPOTS_H

#include <qobject.h>
#include <qdialog.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qpopupmenu.h>
#include <qcombobox.h>
#include <qdatetime.h>
#include <qtable.h>
#include <qlineedit.h>
#include <qstring.h>
#include <qhbox.h>
#include <qpainter.h>
#include <qprinter.h>
#include <qfont.h> 
#include <qlabel.h> 
#include <qsimplerichtext.h> 
#include <qpaintdevicemetrics.h>


#include "Listes.h"
#include "CApp.h"
#include "ComptaMoteurBase.h"
#include "EspecesCalculator.h"
#include "ui/depots_validation.h"


/**
	@author Eric Maeker <eric.maeker@free.fr>
*/

class Depots_Ui : public QWidget
{
    Q_OBJECT

public:
    Depots_Ui ( ComptaMoteurBase* pComptaMoteurBase, QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

    ~Depots_Ui ();


	void	setTableWithHonoListe	();
	void	getListForComboDepots	();
	void	createHonoHTMLTable	(QString& retour, bool avecEntete = TRUE, int nbChequesParPages = 0);
	void	getComboComptes		();

public slots:

	void popupTableDepots( int r, int c, const QPoint &p );
	void trieAsc();
	void trieDesc();
	void chqDelete();
	void viewDepot(int id);
	
	
	void fermer();
	void especesCalcul();
	void sauvDepot();
	void createFileDepot();
	void imprimeDepot();

signals:
	void depotsChange1();

public:

    // DONNEES
	ComptaMoteurBase*	m_pComptaMoteurBase;
	HONO_LISTE*	m_Hono_Liste_Depot;	/*!< Pointeur de liste d'honoraires utilisé pour gérer le tableau */
	DEPOTS_LISTE*	m_Depots_Liste;		/*!< Liste des dépots disponible */
	COMPTES_LISTE* 	m_Comptes_Liste;	/*!< Liste des comptes de l'utilisateur */
	QTable*		m_TableauDepot;		/*!< Tableau qui affichera les chèques à remettre ou remis */
	double	 	m_totalDepot;		/*!< Montant total du dépot */
	QLineEdit*	m_LineEdit_Total;	/*!< LineEdit pour afficher le montant total du dépot */
	QLineEdit*	m_LineEdit_nbChq;	/*!< LineEdit pour afficher le nombre de chèques concernés */
	int		m_colTrie;		/*!< Colonne du tableau à trier (pour le popup) */
	bool 	 	m_popupEnable;		/*!< TRUE si on peut obtenir le popup du tableau des honoraires. FALSE cas contraire */
	
	QComboBox*	m_Combo_ListeDepots;	/*!< ComboBox pour afficher les dépots antérieurs + celui en cours*/
	QComboBox*	m_Combo_ListeComptes;	/*!< ComboBox pour afficher les comptes bancaires dispo */
	QPushButton* 	btEsp;
	QPushButton* 	btImp;
	QPushButton* 	btSauv;
	QPushButton* 	btFich;
	QPushButton* 	btAnn;
	QLabel*		m_Label_Periode;
	QPopupMenu*	m_Popup;

};

#endif

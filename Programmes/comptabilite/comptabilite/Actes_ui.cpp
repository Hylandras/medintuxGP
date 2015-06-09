 /********************************* Actes_ui.cpp **********************************
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
/*! \file Actes_ui.cpp
    \class Actes_Ui
    \brief Fenêtre de gestion des actes de l'application principale de comptabilité. Utilisation et mis à jour de la variable G_pCApp->m_ActesListe.

     #include "Actes_ui.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#include "Actes_ui.h"

/*! \class Actes_ui
 *  \brief Widget qui sera affiché par CMDI_Actes et qui gère la création, modification et visualisation des actes disponibles.
 *  \todo lors de l'ajout faciliter le choix du type d'acte avec ceux déjà enregistrés et laisser la possibiliter d'en ajout de nouveaux (de type).
*/
Actes_Ui::Actes_Ui( ComptaMoteurBase* pComptaMoteurBase, QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{// Intialise les variable
  m_pComptaMoteurBase = pComptaMoteurBase;
  setCaption( TR("Gestionnaire des actes.") );
  m_Popup_Table = 0;
  m_pActes_ListeForTable = new ACTES_LISTE;
  if (!m_pActes_ListeForTable) return;
  m_pActes_ListeForTable->clear();
  m_pActes_Liste = new ACTES_LISTE;
  if (!m_pActes_Liste) return;
  m_pActes_Liste->clear();
  m_pComptaMoteurBase->getActesForCombobox(*m_pActes_Liste);

  m_pTableauActes = new QTable(1,5,this); // 5 colonnes
    // structure le tableau
    m_pTableauActes->setSelectionMode(QTable::SingleRow);
    m_pTableauActes->setReadOnly( TRUE );
    m_pTableauActes->setSorting( FALSE );
    m_pTableauActes->setMaximumSize( 32767, 800 );
    m_pTableauActes->setMinimumSize( 590, 100 );
    m_pTableauActes->horizontalHeader()->setLabel( 0, tr( "Nom" ) );
    m_pTableauActes->horizontalHeader()->setLabel( 1, tr( "Type" ) );
    m_pTableauActes->horizontalHeader()->setLabel( 2, tr( "Description" ) );
    m_pTableauActes->horizontalHeader()->setLabel( 3, tr( "Total" ) );
    m_pTableauActes->horizontalHeader()->setLabel( 4, tr( "Tiers" ) );
    // Modifier la géométrie de la table
    m_pTableauActes->setColumnWidth ( 0, 100 );
    m_pTableauActes->setColumnWidth ( 1, 100 );
    m_pTableauActes->setColumnWidth ( 2, 150 );
    m_pTableauActes->setColumnWidth ( 3, 80 );
    m_pTableauActes->setColumnWidth ( 4, 80);
    // Connecte le popup du tableau
    connect (m_pTableauActes ,   SIGNAL ( contextMenuRequested ( int, int, const QPoint & ) ),
	     this,               SLOT   ( popupTableActes ( int, int, const QPoint &)  ) );


  // Dessiner la fenêtre
  QLabel *intro = new QLabel(TR("<b>Gestionnaire des actes</b>"), this);

  QHBox *hb = new QHBox(this);
  hb->setSpacing ( 10 );
  btAjoute  = new QPushButton(TR("Ajouter un Acte"), hb);
  connect (btAjoute,  SIGNAL ( clicked() ), this , SLOT ( ajouteActe() ) );

  // Récupère la liste des Types d'actes
  QHBox *hb0 = new QHBox(this);
  QLabel *lblListeDepots = new QLabel(hb0);
  lblListeDepots->setText(TR("<b>Sélectionnez un type d'acte à filtrer :</b>"));
  m_Combo_ListeTypesActes = new QComboBox(hb0);
  // Insérer ici la liste des types d'actes
  findTypes();

  // Connecte le combo avec chgt de dépôt
  connect ( m_Combo_ListeTypesActes,SIGNAL( activated(int) ),
	    this,	    SLOT  ( viewTypeActe(int) ) );

  QGridLayout *gridChq = new QGridLayout (this, 6 , 2, 15, 6, "ActesLayout");

  gridChq->addMultiCellWidget ( hb0, 0,0, 0, 3);
  gridChq->addMultiCellWidget ( intro, 1,1, 0, 3);
  gridChq->addMultiCellWidget ( m_pTableauActes , 3,3,0,3);
  gridChq->addMultiCellWidget ( hb, 6,6,0,3 );

  setTable();

  adjustSize();
}

/*! \brief Destructeur de la classe. */
Actes_Ui::~Actes_Ui()
{if (G_pCApp->m_Verbose) qWarning("Actes_Ui::Destruct");
 if (m_pActes_ListeForTable)	delete m_pActes_ListeForTable;
 //if (m_pActes_Liste) delete m_pActes_Liste;  // deleted par CApp
 if (m_pTableauActes)		delete m_pTableauActes;
 if (m_Popup_Table)		delete m_Popup_Table;
 if (G_pCApp->m_Verbose) qWarning("Actes_Ui::Destruct Fin");
}

/*! \brief Affiche les paramètres d'un acte.
*/
void Actes_Ui::voirActe()
{ // Vérifie qu'une ligne est bien sélectionnée sinon msg erreur
  if (m_pTableauActes->currentSelection () != -1 && m_pActes_Liste->count() >= 1)
   {// Récupère l'honoraire sélectionné, supprime le et met à jour le tableau
    ACTES_LISTE::iterator  it = m_pActes_ListeForTable->at( m_pTableauActes->currentRow() );
    QString msg;
    msg = TR("Nom : ") + (*it).getNom() + "\n";
    msg += TR("Type : ") + (*it).getType() + "\n";
    msg += TR("Description : ") + (*it).getDesc() + "\n";
    msg += TR("Montant Total : ") + QString::number( (*it).getTotal() ) + " " + TR("€") +"\n";
    msg += TR("Montant Tiers : ") + QString::number( (*it).getTiers() ) + " " + TR("€") +"\n";
    QMessageBox::information(this, TR("Visualisation des actes"), msg , TR("Ok"),"","",1,0); 
   }  // Fin if MsgBox de suppression
}


/*! \brief Affiche la fenêtre la fenêtre de modification d'actes vierge et permet l'ajout d'un acte. La variable G_pCApp->m_ActesListe est mise à jour.
*/
void Actes_Ui::ajouteActe()
{Acte_Modif* pActe_Modif = new Acte_Modif();
 if (pActe_Modif)
 {// si boutton sauvegarde cliqué
  if (pActe_Modif->exec() == QDialog::Accepted)
     {	// Récupère les données et update la base
	Actes a = Actes();
	a.setNom( pActe_Modif->getNom() );
	a.setType( pActe_Modif->getType() );
	a.setDesc( pActe_Modif->getDesc() );
	a.setTotal( pActe_Modif->getTotal().toDouble() );
	a.setTiers( pActe_Modif->getTiers().toDouble() );
	if (m_pComptaMoteurBase->saveActe( &a ) )
	 { m_pComptaMoteurBase->getActesForCombobox( *m_pActes_Liste );
//	   G_pCApp->m_ActesListe = m_pActes_Liste;
	   emit(sign_Actes_Ui_ActesListeModif());
	   findTypes();
	   setTable();
	 }
	else {qWarning(m_pComptaMoteurBase->m_messageErreur);}
     }
  setTable();
  delete pActe_Modif;
 }
}


/*! \brief Affiche la fenêtre de modification des actes et modifie un acte si demandé. Met à jour G_pCApp->m_ActesListe .
*/
void Actes_Ui::modifierActe()
{ // Vérifie qu'une ligne est bien sélectionnée sinon msg erreur
  if (m_pTableauActes->currentSelection () != -1 && m_pActes_Liste->count() >= 1)
   {// Récupère l'honoraire sélectionné, supprime le et met à jour le tableau
    ACTES_LISTE::iterator  it = m_pActes_ListeForTable->at( m_pTableauActes->currentRow() );
    // Afficher la fenêtre de modification des actes
    Acte_Modif* af = new Acte_Modif();
    if (af)
    { af->setNom( (*it).getNom() );
      af->setType( (*it).getType() );
      af->setDesc( (*it).getDesc() );
      af->setTotal( QString::number( (*it).getTotal() ) );
      af->setTiers( QString::number( (*it).getTiers() ) );
      // si boutton sauvegarde cliqué
      if (af->exec() == QDialog::Accepted)
      {	// Récupère les données et update la base
	(*it).setNom( af->getNom() );
	(*it).setType( af->getType() );
	(*it).setDesc( af->getDesc() );
	(*it).setTotal( af->getTotal().toDouble() );
	(*it).setTiers( af->getTiers().toDouble() );
	if (m_pComptaMoteurBase->updateActe( &(*it) ) ) 
	 { m_pComptaMoteurBase->getActesForCombobox( *m_pActes_Liste );
//	   G_pCApp->m_ActesListe = m_pActes_Liste;
	   emit(sign_Actes_Ui_ActesListeModif());
	   findTypes();
	   setTable();
	 }
	else {qWarning(m_pComptaMoteurBase->m_messageErreur);}
       }
      setTable();
      delete af;
    } // Fin if af
   }  // Fin if MsgBox de suppression
}


/*! \brief Supprime un acte de la base de données. La variable G_pCApp->m_ActesListe est mise à jour.
*/
void Actes_Ui::supprimerActe()
{ // Vérifie qu'une ligne est bien sélectionnée sinon msg erreur
  if (m_pTableauActes->currentSelection () != -1 && m_pActes_ListeForTable->count() >= 1)
   {// Récupère l'honoraire sélectionné, supprime le et met à jour le tableau
    ACTES_LISTE::iterator  it = m_pActes_ListeForTable->at( m_pTableauActes->currentRow() );
    // Attention il faut effacer le bon acte !!! lier les deux ACTES_LISTE par l'id de l'acte
    QString msg = TR("Vous allez supprimer l'acte suivant : \n");
    msg += TR("Nom : ") + (*it).getNom() + "\n";
    msg += TR("Type : ") + (*it).getType() + "\n";
    msg += TR("Description : ") + (*it).getDesc() + "\n";
    msg += TR("Montant Total : ") + QString::number( (*it).getTotal() ) + " " + TR("€") +"\n";
    msg += TR("Montant Tiers : ") + QString::number( (*it).getTiers() ) + " " + TR("€") +"\n";
    msg += TR("Confirmez-vous ?");
    if (QMessageBox::warning(this, TR("Suppression d'un acte."), msg, TR("Oui"), TR("Non"),"", 0, 1) == 0)
     {	m_pComptaMoteurBase->deleteActe( (*it).getId() );
	m_pComptaMoteurBase->getActesForCombobox( *m_pActes_Liste );
	//G_pCApp->m_ActesListe = m_pActes_Liste;
	   emit(sign_Actes_Ui_ActesListeModif());
	findTypes();
	setTable();
     }
   }  // Fin if MsgBox de suppression
}


/*! \brief SLOT appelé lorsque clique droit sur le tableau des actes.*/
void Actes_Ui::popupTableActes(int, int, const QPoint& p)
{ if (m_Popup_Table) delete m_Popup_Table;
  m_Popup_Table = new QPopupMenu(this, "popTable");
  if (m_Popup_Table)
   {m_Popup_Table->insertItem (TR("Voir")     , this, SLOT(voirActe( ) ) );
    m_Popup_Table->insertItem (TR("Modifier") , this, SLOT(modifierActe( )  ) );
    m_Popup_Table->insertItem (TR("Supprimer") , this, SLOT(supprimerActe() ) );
    m_Popup_Table->popup(p);
   }
}


/*! \fn void Acte_Selection::findTypes()
    \brief Recherche dans la liste des actes disponibles les différents types d'actes et sauvegarde le résultat dans la vairable protected \em m_TypesActes . Met à jour le combo des types d'actes.
*/
void Actes_Ui::findTypes()
{ QString tmp = "";
  m_TypesActes.clear();
  ACTES_LISTE::iterator it;
  for (it = m_pActes_Liste->begin(); it !=  m_pActes_Liste->end(); ++it )
   { tmp = (*it).getType();
     if (m_TypesActes.isEmpty() )
	{ m_TypesActes.append( tmp ); }
     else
      {	// Vérifier que le type n'est déjà dans la liste
	if ( m_TypesActes.contains( tmp ) == 0 )
	 { m_TypesActes.append ( tmp ); }
       }
   }
  m_TypesActes.append( TR("Tous les actes") );
  m_Combo_ListeTypesActes->clear();
  m_Combo_ListeTypesActes->insertStringList( m_TypesActes );
  m_Combo_ListeTypesActes->setCurrentItem( m_TypesActes.count()-1 );
}


/*! \brief Rempli le tableau des chèques avec la liste des honoraires de la classe. */
void Actes_Ui::setTable()
{ // Efface tout le tableau
  for (int i = 0; i < m_pTableauActes->numRows(); ++i)
   {	m_pTableauActes->clearCell(i, 0);
	m_pTableauActes->clearCell(i, 1);
	m_pTableauActes->clearCell(i, 2);
	m_pTableauActes->clearCell(i, 3);
	m_pTableauActes->clearCell(i, 4);
   }

  if (m_pActes_Liste->count() == 0)
   { m_pTableauActes->setNumRows(1);
     m_pTableauActes->setText(0,0, TR("Aucun chèque") );
     m_pTableauActes->setText(0,1, " " );
     m_pTableauActes->setText(0,2, " " );
     m_pTableauActes->setText(0,3, " " );
     m_pTableauActes->setText(0,4, " " );
   }
  else
   { int r = 0;
     m_pActes_ListeForTable->clear();
     ACTES_LISTE::iterator it;
     m_pTableauActes->setNumRows( m_pActes_Liste->count() );
     for (it = m_pActes_Liste->begin(); it !=  m_pActes_Liste->end(); ++it )
      { if ( (m_Combo_ListeTypesActes->currentText() == TR("Tous les actes") )  || (m_Combo_ListeTypesActes->currentText() == (*it).getType() ) )
       { m_pTableauActes->setText( r,0, (*it).getNom() );
	 m_pTableauActes->setText( r,1, (*it).getType() );
	 m_pTableauActes->setText( r,2, (*it).getDesc() );
	 m_pTableauActes->setText( r,3, QString::number ( (*it).getTotal() ) );
	 m_pTableauActes->setText( r,4, QString::number( (*it).getTiers() ) );
	 r++;
	// Sauvegarde cette liste dans le pointeur m_pActes_ListeForTable
	m_pActes_ListeForTable->append( (*it) );
       } // Fin if
      } // Fin for
     m_pTableauActes->setNumRows( r );
   }
}


void Actes_Ui::sauvActe()
{}


/*! \brief SLOT appelé par le filtre lorsque l'utilisateur clique sur le combo des types d'acte. */
void Actes_Ui::viewTypeActe(int)
{ setTable(); }


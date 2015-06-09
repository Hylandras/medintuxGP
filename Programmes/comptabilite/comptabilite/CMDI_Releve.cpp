 /********************************* CMDI_Releve.cpp **********************************
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
/*! \file CMDI_Releve.cpp
    \brief (put some short descripion here).

     #include "CMDI_Releve.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#include "CMDI_Releve.h"

//---------------------------------- CMDI_Releve -----------------------------------------------------------------------------
CMDI_Releve::CMDI_Releve( QWidget* parent, const char* name,
		int wflags , ComptaMoteurBase *pComptaMoteurBase)
    : CMDI_Generic( parent, name, wflags , pComptaMoteurBase)
{if (G_pCApp->m_Verbose) qWarning("Création de CMDI_Releve");
 m_pComptaMoteurBase = pComptaMoteurBase;
 m_CompteListe = new COMPTES_LISTE;
 if (!m_CompteListe) return ;
 m_CompteListe->clear();
 m_MvtsListe = new MVTS_LISTE;
 if (!m_MvtsListe) return ;
 m_MvtsDispo = new MVTS_LISTE;
 if (!m_MvtsDispo) return ;
 if (!m_pComptaMoteurBase->getMvtsDispo(*m_MvtsDispo))
  { QWidget* l = new QWidget();
    QMessageBox::information(l, TR("Erreur base de données"), m_pComptaMoteurBase->m_messageErreur, TR("Ok"),"","",1,0);
    delete l;
    return ;
  }

 if (!m_pComptaMoteurBase->getComptes( *m_CompteListe ))
 { QMessageBox::information(this, TR("ERREUR"), m_pComptaMoteurBase->m_messageErreur, TR("Ok"),"","", 1,0);
   return ;
 }

 // Crée un widget avec un GridLayout
 QWidget *wgt = new QWidget (this);
 if (!wgt) return ;

 wgt->setMinimumSize( QSize( 100, 250 ) );
 wgt->setMaximumSize( QSize( 1024, 768 ) );
 wgt->setBaseSize( QSize( 0, 0 ) );

 QLabel *lbl = new QLabel(wgt);
 lbl->setText(TR("<b>Relevés de comptes</b>") );
 lbl->setMinimumSize(200,22);

 QHBox* hb0 = new QHBox(this);
 hb0->setSpacing(20);
 QLabel* lblCpt = new QLabel(hb0);
 lblCpt->setText("<p align=\"right\"><b>" + TR("Comptes bancaires") + "</p></b>");
 m_Combo_Comptes = new QComboBox(hb0);
 getComboComptes();
 connect ( m_Combo_Comptes, SIGNAL(activated(int)), this, SLOT(setTableTo() ) );
 

 // insère un le tableau des comptes
 tabRlv = new QTable (1 , 6, wgt);
   // structure le tableau
   tabRlv->setSelectionMode(QTable::SingleRow);
   tabRlv->setReadOnly(TRUE);
   tabRlv->setMinimumSize( 650, 230 );
   tabRlv->setMaximumSize( 900, 700 );

   tabRlv->horizontalHeader()->setLabel( 0, tr( "Libellé" ) );
   tabRlv->horizontalHeader()->setLabel( 1, tr( "Mouvement" ) );
   tabRlv->horizontalHeader()->setLabel( 2, tr( "Date" ) );
   tabRlv->horizontalHeader()->setLabel( 3, tr( "Débit" ) );
   tabRlv->horizontalHeader()->setLabel( 4, tr( "Crédit" ) );
   tabRlv->horizontalHeader()->setLabel( 5, tr( "Tri" ) );


   // Modifier la géométrie de la table
   tabRlv->setColumnWidth ( 0, 150 );
   tabRlv->setColumnWidth ( 1, 150 );
   tabRlv->setColumnWidth ( 2, 100 );
   tabRlv->setColumnWidth ( 3, 100 );
   tabRlv->setColumnWidth ( 4, 100 );
   tabRlv->setColumnWidth ( 5, 100 );
   tabRlv->hideColumn(5);
   tabRlv->setSorting(FALSE);


   // Connecte le popup du tableau
   connect (tabRlv ,   SIGNAL (contextMenuRequested ( int, int, const QPoint & ) ),
	    this, SLOT   (popupTable ( int, int, const QPoint &)  ) 
	    );

  // Insère le Totaux...
  QHBox *hb1 = new QHBox(wgt);
  hb1->setSpacing(10);
  QLabel *lbl2 = new QLabel(hb1);
  lbl2->setText(TR("<b>Total Dépenses</b>"));
  lbl2->setMinimumSize(120,22);
  totalDepenses = new QLineEdit(hb1);
  QLabel *lbl3 = new QLabel(hb1);
  lbl3->setText(TR("<b>Total Recettes</b>"));
  lbl3->setMinimumSize(120,22);
  totalRecettes = new QLineEdit(hb1);
  QLabel *lbl4 = new QLabel(hb1);
  lbl4->setText(TR("<b>Solde</b>"));
  lbl4->setMinimumSize(100,22);
  soldeCompte = new QLineEdit(hb1);

 // Insère quelques bouttons
//   btAjoute = new QPushButton(hb);
//   btAjoute->setText( TR("Nouveau compte") );
//   connect ( btAjoute,  SIGNAL ( clicked()  ) ,
// 	   this,   SLOT   ( slot_Ajouter()  )  );

 QGridLayout *grid = new QGridLayout( wgt, 20, 1, 15, 15, "LayoutCMDI_Releve"); 
 grid->addMultiCellWidget( lbl, 0, 0, 0, 1, Qt::AlignHCenter);
 grid->addMultiCellWidget( hb0, 5, 5, 0, 1);
 grid->addMultiCellWidget( tabRlv, 10,10, 0, 1 , Qt::AlignHCenter);
 grid->addMultiCellWidget( hb1, 15,15, 0, 1 );
// grid->addMultiCellWidget( hb, 20,20, 0,1);
 setTableTo();

 setFocusProxy( wgt );
 setCentralWidget( wgt );
 SetType(TYP_RELEVE);
}


//---------------------------------- ~CMDI_Releve ------------------------------------------------------------------------
CMDI_Releve::~CMDI_Releve()
{if (G_pCApp->m_Verbose) qWarning("CMDI_Releve::Destruct");
// if (btAjoute) delete btAjoute;
//  if (btImprime) delete btImprime;
  if (m_CompteListe) delete m_CompteListe;
  if (m_Combo_Comptes) delete m_Combo_Comptes;
  if (m_MvtsDispo) delete m_MvtsDispo;
  if (m_MvtsListe) delete m_MvtsListe;
//  if (tabRlv) delete tabRlv;
if (G_pCApp->m_Verbose) qWarning("CMDI_Releve::Destruct");
}


/*! \brief Prépare le combobox avec les libellés des comptes bancaires de l'utilisateur.
*/
void CMDI_Releve::getComboComptes()
{ int r = 0;
  int id = 0;
  if (!m_pComptaMoteurBase->getComptes( *m_CompteListe ))
   QMessageBox::information(this, TR("ERREUR"),m_pComptaMoteurBase->m_messageErreur, TR("OK"),"","",1,0);
  else
  { m_Combo_Comptes->clear();
    for (COMPTES_LISTE::iterator it = m_CompteListe->begin(); it != m_CompteListe->end(); ++it)
    { m_Combo_Comptes->insertItem( (*it).getLibelle() ); 
      if ((*it).isDefaut()) id=r;
      r++;
    }
    m_Combo_Comptes->setCurrentItem(id);
  }
}


/*!\brief SLOT appelé lorsque l'utilisateur demande le menu contextuel du tableau des relevés.
*/
void CMDI_Releve::popupTable( int , int , const QPoint& )
{
//  QPopupMenu *pop = new QPopupMenu(this, "popTable");
//  if (pop) 
//  { pop->insertItem (TR("Voir"),      this, SLOT(cptView() ) );
//    pop->insertItem (TR("Modifier") , this, SLOT(cptModif()  ) );
// //   pop->insertItem (TR("Supprimer"), this, SLOT(honoDelete() ) );
//    pop->popup(p);
//  }
}


/*! \brief Rafraîchit la table des comptes avec la liste des comptes passée en pointeur. 
 *  \todo mettre une ligne sur deux en gris clair
*/
void CMDI_Releve::setTableTo()
{ m_MvtsListe->clear();
  if (!m_pComptaMoteurBase->getMvtsForReleve( *m_MvtsListe ))
  { QWidget* l = new QWidget();
    QMessageBox::information(l, TR("Erreur base de données"), m_pComptaMoteurBase->m_messageErreur, TR("Ok"),"","",1,0);
    delete l;
    return ;
  }

  // Récupère le solde initial du compte bancaire en cours
  int idCpt = -1;
  idCpt = (*m_CompteListe->at( m_Combo_Comptes->currentItem()) ).getId();
  double solde = (*m_CompteListe->at( m_Combo_Comptes->currentItem()) ).getSolde();
  if ( solde != 0)
  { Mouvements* m = new Mouvements();
    m->setIdCompte( idCpt );
    m->setDateValeur( "1999-01-31" );
    if (solde < 0 ) { m->setType(1);  solde= -1*solde; m->setTotal( solde );}
    else {m->setType(0); m->setTotal(solde); }
    m->setNom(TR("Solde initial"));
    m_MvtsListe->append( *m );
    delete m;
  }

  if ((m_MvtsListe->count() == 0) || (!m_MvtsListe))
  { tabRlv->setNumRows( 1 );
    tabRlv->setText(0,0, TR("Aucun mouvement") );
    for (int i = 1; i < tabRlv->numCols(); ++i)
 	tabRlv->setText(0,i, " " );
  }
  else
  { int r = 0;
    double recettes, depenses;
    recettes = 0;
    depenses = 0;
    MVTS_LISTE::iterator it;
    tabRlv->setNumRows( m_MvtsListe->count() );
    for (it = m_MvtsListe->begin(); it !=  m_MvtsListe->end(); ++it )
    {if ((*it).getIdCompte() == idCpt)
      {	tabRlv->setText( r,0, (*it).getNom() );
	// Récupère les données de l'honoraire
	if ((*it).getIdParent() != -1) tabRlv->setText( r,1, (*m_MvtsDispo->at( (*it).getIdParent() - 1)).getNom() );
	else if ((*it).getNom() != TR("Solde initial")) tabRlv->setText(r,1,TR("Dépôt"));
	tabRlv->setText( r,2, (*it).getDateValeur().toString("dd-MM-yyyy") );
	tabRlv->setText( r,5, (*it).getDateValeur().toString("yyyy-MM-dd") );

	if ((*it).getType() == 0) // Recettes
	{ 	tabRlv->setText( r,4, QString::number( (*it).getTotal() , 'f', 2)  );
		recettes += (*it).getTotal();
		tabRlv->setText( r,3, " ");
	}
	else
	{	tabRlv->setText( r,3, QString::number( (*it).getTotal()  , 'f', 2)  );
		depenses += (*it).getTotal();
		tabRlv->setText( r,4, " ");
	}
	r++;
      }
     }
    tabRlv->setNumRows( r );
    tabRlv->setSorting(TRUE);
    tabRlv->sortColumn(5, FALSE, TRUE);
    tabRlv->setSorting(FALSE);
    totalDepenses->setText(QString::number(depenses , 'f', 2) );
    totalRecettes->setText(QString::number(recettes , 'f', 2) );
    soldeCompte->setText  (QString::number((recettes - depenses) , 'f', 2));
  }
}



//---------------------------------- ActiverRubrique ---------------------------------------------------------------------------
void CMDI_Releve::ActiverRubrique(const char *rubName)
{if (strncmp(rubName, TR("Releve"), 5)!=0) return;
 GlobalActiverWidget(this);
}



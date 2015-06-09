 /********************************* Depots.cpp **********************************
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
/*! \file Depots.cpp
    \brief (put some short descripion here).

     #include "Depots.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#include "Depots.h"
/*! \class Depots_Ui
  * \brief Affiche une fenêtre de gestion des remises de chèques. Lien avec la base de données, l'imprimante et fichier de savegarde.
*/
Depots_Ui::Depots_Ui( ComptaMoteurBase* pComptaMoteurBase, QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{ if (G_pCApp->m_Verbose) qWarning("Création de Depots_Ui");
  // Intialise les variable
  m_pComptaMoteurBase = pComptaMoteurBase;
  setCaption( TR("Préparateur de remise de chèques") );
  m_Popup = 0;
  m_Hono_Liste_Depot = new HONO_LISTE;
  if (!m_Hono_Liste_Depot) return ;
  m_Hono_Liste_Depot->clear();
  m_Comptes_Liste = new COMPTES_LISTE;
  if (!m_Comptes_Liste) return ;
  m_popupEnable = TRUE;

  m_TableauDepot = new QTable(1,5,this); // 4 colonnes
    // structure le tableau
    m_TableauDepot->setSelectionMode(QTable::SingleRow);
    m_TableauDepot->setReadOnly( TRUE );
    m_TableauDepot->setSorting( FALSE );
    m_TableauDepot->setMaximumSize( 32767, 300 );
    m_TableauDepot->setMinimumSize( 590, 100 );
    m_TableauDepot->setBaseSize( 590, 200 );
    m_TableauDepot->horizontalHeader()->setLabel( 0, tr( "Patient" ) );
    m_TableauDepot->horizontalHeader()->setLabel( 1, tr( "Émetteur" ) );
    m_TableauDepot->horizontalHeader()->setLabel( 2, tr( "Actes" ) );
    m_TableauDepot->horizontalHeader()->setLabel( 3, tr( "Date" ) );
    m_TableauDepot->horizontalHeader()->setLabel( 4, tr( "Montant" ) );
    // Modifier la géométrie de la table
    m_TableauDepot->setColumnWidth ( 0, 150 );
    m_TableauDepot->setColumnWidth ( 1, 150 );
    m_TableauDepot->setColumnWidth ( 2, 80 );
    m_TableauDepot->setColumnWidth ( 3, 80 );
    m_TableauDepot->setColumnWidth ( 4, 80);
    // Connecte le popup du tableau
    connect (m_TableauDepot ,   SIGNAL ( contextMenuRequested ( int, int, const QPoint & ) ),
	     this,               SLOT   ( popupTableDepots ( int, int, const QPoint &)  ) );

  // Récupère les honoraires pour la période à filtrer et met à jour le tableau
  m_LineEdit_Total = new QLineEdit(this);
  m_LineEdit_Total->setReadOnly(TRUE);
  m_totalDepot = 0;
  m_LineEdit_nbChq = new QLineEdit(this);
  m_LineEdit_nbChq->setReadOnly(TRUE);

  // Dessiner la fenêtre
  QLabel *intro = new QLabel(TR("<b>Remise de chèques pour la période filtrée :</b>"), this);
  m_Label_Periode = new QLabel("", this);
  QLabel *menu = new QLabel(TR("<b>Fonctions disponibles :</b>"), this);
  QLabel *lblTotal = new QLabel (TR("Montant total : "), this);
  QLabel *lblNbChq = new QLabel (TR("Nombre de chèques : "), this);

  QHBox *hb = new QHBox(this);
  hb->setSpacing ( 10 );
  btEsp  = new QPushButton(TR("Calcul d'espèces"), hb);
  btImp  = new QPushButton(TR("Imprimer"), hb);
  btSauv = new QPushButton(TR("Valider"), hb);
  btFich = new QPushButton(TR("Sauvegarder"), hb);

  connect (btEsp,  SIGNAL ( clicked() ), this , SLOT ( especesCalcul() ) );
  connect (btFich, SIGNAL ( clicked() ), this , SLOT ( createFileDepot() ) );
  connect (btSauv, SIGNAL ( clicked() ), this , SLOT ( sauvDepot() ) );
  connect (btImp,  SIGNAL ( clicked() ), this , SLOT ( imprimeDepot() ) );

  // Récupère la liste des dépots antérieurs
  QHBox *hb0 = new QHBox(this);
  QLabel *lblListeDepots = new QLabel(hb0);
  lblListeDepots->setText( TR("<b>Sélectionnez le dépôt :</b>") ) ;
  m_Combo_ListeDepots = new QComboBox(hb0);
  // Insérer ici la liste des dépots antérieurs
  getListForComboDepots();
  // Connecte le combo avec chgt de dépôt
  connect ( m_Combo_ListeDepots,SIGNAL( activated(int) ),
	    this,	SLOT  ( viewDepot(int) ) );

  // Récupère la liste des comptes disponibles
  QHBox *hb1 = new QHBox(this);
  QLabel *lblListeComptes = new QLabel(hb1);
  lblListeComptes->setText( TR("<b>Compte bancaire pour ce dépôt :</b>") );
  m_Combo_ListeComptes = new QComboBox(hb1);
  // Insérer ici la liste des Comptes
  getComboComptes();


  QGridLayout *gridChq = new QGridLayout (this, 6 , 2, 15, 6, "DepotsLayout");

  gridChq->addMultiCellWidget ( hb0, 0,0, 0, 3);
  gridChq->addMultiCellWidget ( hb1, 1,1,0,3);
  gridChq->addMultiCellWidget ( intro, 2,2, 0, 3);
  gridChq->addMultiCellWidget ( m_Label_Periode, 3,3, 0, 3);
  gridChq->addMultiCellWidget ( m_TableauDepot , 6,6,0,3);
  gridChq->addWidget ( lblTotal, 10,0);
  gridChq->addWidget ( m_LineEdit_Total, 10,1);
  gridChq->addWidget ( lblNbChq, 10,2);
  gridChq->addWidget ( m_LineEdit_nbChq, 10,3);
  gridChq->addMultiCellWidget ( menu, 11,11, 0, 3);
  gridChq->addMultiCellWidget ( hb, 12,12,0,3 );
  if (!m_pComptaMoteurBase->getHonorairesForDepot(m_Hono_Liste_Depot))
   { QMessageBox::information( this, TR("ERREUR"), m_pComptaMoteurBase->m_messageErreur, TR("OK"), QString::null, QString::null, 0,1);
   }
  else { setTableWithHonoListe(); }

  adjustSize();
}

/*! \brief Destructeur de la classe. */
Depots_Ui::~Depots_Ui()
{ if (G_pCApp->m_Verbose) qWarning("Depots_Ui::Destruct");
  if (m_Hono_Liste_Depot) 	delete m_Hono_Liste_Depot;
  if (m_Depots_Liste) 		delete m_Depots_Liste;
  if (m_Comptes_Liste) 		delete m_Comptes_Liste;
  if (m_TableauDepot) 		delete m_TableauDepot;
  if (btImp) 			delete btImp;
  if (btEsp) 			delete btEsp;
  if (btSauv) 			delete btSauv;
  if (btFich) 			delete btFich;
  if (m_Combo_ListeDepots) 	delete m_Combo_ListeDepots;
  if (m_Popup)			delete m_Popup;
  if (G_pCApp->m_Verbose) qWarning("Depots_Ui::Destruct Fin");
}


/*! \brief Affiche la fenêtre pour la remise d'espèces.
 *  \sa EspecesCalculator
*/
void Depots_Ui::especesCalcul()
{EspecesCalculator *espCalc = new EspecesCalculator(m_pComptaMoteurBase);
 QString depot = "";
 if (espCalc)
 {espCalc->exec();
  if ( espCalc->result() == QDialog::Accepted )
	{ emit(depotsChange1()); getListForComboDepots();}
  delete espCalc;
 }
}


/*! \brief Prépare le combobox avec les libellés des comptes bancaires de l'utilisateur.
*/
void Depots_Ui::getComboComptes()
{ int r = 0;
  int id = 0;
  m_Combo_ListeComptes->clear();

  if (!m_pComptaMoteurBase->getComptes(*m_Comptes_Liste))
  { QMessageBox::information(this, TR("ERREUR"), m_pComptaMoteurBase->m_messageErreur, TR("Ok"),"","",1,0);
    return ;
  }

  for (COMPTES_LISTE::iterator it = m_Comptes_Liste->begin(); it != m_Comptes_Liste->end(); ++it)
  { m_Combo_ListeComptes->insertItem( (*it).getLibelle() ); 
    if ((*it).isDefaut()) id=r;
    r++;
  }
  m_Combo_ListeComptes->insertItem(TR("Aucun Compte"));
  m_Combo_ListeComptes->setCurrentItem(id);
}

/*! \brief SLOT appelé pour fermer la fenêtre des remises de chèques. */
void Depots_Ui::fermer()
{ destroy(); }

/*! \brief SLOT appelé pour sauvegarder dans la base de données la remise de chèques affichée à l'écran.
*/
void Depots_Ui::sauvDepot()
{if (m_Combo_ListeComptes->currentItem() == (m_Combo_ListeComptes->count() - 1))
  QMessageBox::information(this, TR("ERREUR"), TR("Vous devez spécifier un compte bancaire."),TR("Ok"),"","",1,0);
 else if (m_Hono_Liste_Depot->count() == 0)
  QMessageBox::information(this, TR("ERREUR"), TR("Vous devez sélectionner au moins un honoraire."),TR("Ok"),"","",1,0);
 else
 {// Envoie la liste à ComptaMoteurBase pour gestion
   QDate curDt = QDate::currentDate();

  Depots_Validation* pDepots_Validation = new Depots_Validation();
  pDepots_Validation->setParams(m_pComptaMoteurBase, curDt, m_Comptes_Liste, m_Hono_Liste_Depot);
  pDepots_Validation->exec();
  if (pDepots_Validation->result() == QDialog::Accepted)
  { getListForComboDepots();
    setTableWithHonoListe();
  }

/*
  if (m_pComptaMoteurBase->saveDepotToBase(&curDt, &*m_Hono_Liste_Depot, rib , idCpt ) )
  { // Affiche une fenêtre d'information
    QMessageBox::information(this, TR("Sauvegarde de la remise de chèque."), 
	TR("La remise de chèque actuelle vient d'être sauvegardée dans la base de données.")+"\n\n"+
	TR("Nombres de chèques : ")+QString::number(m_Hono_Liste_Depot->count()) + "\n\n" + 
	TR("Montant total de la remise : ") + m_LineEdit_Total->text()+ " Euros.\n\n", 
	TR("Ok"),QString::null, QString::null, 0, 1);
    // Met à jour le tableau et combobox
    getListForComboDepots();
    if (!m_pComptaMoteurBase->getHonorairesForDepot(m_Hono_Liste_Depot))
     { QMessageBox::information( this, TR("ERREUR"), m_pComptaMoteurBase->m_messageErreur, TR("OK"), QString::null, QString::null, 0,1);
     }
    else { setTableWithHonoListe(); emit( depotsChange1() );}

  }
  else QMessageBox::information(this, TR("Erreur lors de la sauvegarde."), 
	m_pComptaMoteurBase->m_messageErreur, 
	TR("Ok"),QString::null, QString::null, 0, 1);
*/
 } // fin if listeCompte != "Aucun Compte"
}


/*! \brief Construit un tableau HTML avec la remise de chèque en cours d'affichage dans le tableau de cette classe.
 * Lorsque cette fonction est utilisée pour l'impression, les sauts de pages sont encodées dans le texte comme "<!-- -->".
 *  \sa createFileDepot .
*/
void Depots_Ui::createHonoHTMLTable(QString& retour, bool avecEntete, int nbChequesParPages)
{ retour = "";
  int nbParPage = nbChequesParPages;
  int nbPages = 0;
  int nbHono = 0;
  int reste = 0;

  retour += "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /></head>\n<body>\n";

  if (avecEntete)
    {	retour += "<p align=\"center\"><b>" + TR("Remise de chèques<br />") + G_pCApp->m_currentUser->getNom() + "</b><br />";
	retour += "</p>\n\n";
	retour += "<p align=\"center\"><b>";
	retour += (*m_Comptes_Liste->at ( m_Combo_ListeComptes->currentItem() ) ).getRib("html");
	retour += "</b><br /></p>\n";
	if (m_Combo_ListeDepots->currentItem() < (m_Combo_ListeDepots->count()-1) )
	{ DEPOTS_LISTE::iterator it = m_Depots_Liste-> at ( m_Combo_ListeDepots->currentItem() );
	  retour += "<p>" + TR("Déposée le ") + (*it).getDate() + "<br />" + TR("Concernant la période : ");
	  retour += (*it).getDateDebut().toString("dd MMMM yyyy") + TR(" au ") + (*it).getDateFin().toString("dd MMMM yyyy") + "<br /></p>\n\n";
	} // Fin if dépot déjà effectué
    } // Fin if entete


  // Prépare le tableau
  if (nbChequesParPages == 0) 
   { nbParPage = m_Hono_Liste_Depot->count();      nbPages = 0;  }
  else
   { nbPages = (int)(m_Hono_Liste_Depot->count() / nbParPage);   }


   for (int i = 0; i < nbPages+1; ++i)
    {// Se positionner sur le bon enregistrement de la liste des honoraires
     HONO_LISTE::iterator it;
     retour += "<table border=1 cellpadding=1 cellspacing=1>\n";
     retour += "<tr>\n  ";
     retour += "<td align=\"center\">" + TR("N°") + "</td>";
     retour += "<td align=\"center\">" + TR("Date de l'acte") + "</td>";
     retour += "<td align=\"center\">" + TR("Nom Émetteur") + "</td>";
     retour += "<td align=\"center\">" + TR("Montant") + "</td>\n";
     retour += "</tr>";

     for ( it=m_Hono_Liste_Depot->at( nbHono ) ; it != m_Hono_Liste_Depot->end(); ++it )
      {	nbHono++;
	retour += "<tr>\n";
	retour += "  <td align=\"center\">" + QString::number(nbHono) + "</td>\n";
	retour += "  <td>" + (*it).getDate().toString("dd MMM yyyy") + "</td>\n";
	retour += "  <td>" + (*it).getEmetteur() + "</td>\n";
	retour += "  <td align=\"center\">" + QString::number( (*it).getChq() ) + "</td>\n";
	retour += "</tr>\n\n";
	reste = (nbHono % nbParPage);
	if ( reste == 0) break;
       }
     retour += "</table>\n<p>&nbsp;</p>\n";
     if (reste == 0) retour += "<!-- -->\n\n\n";
    } // Fin FOR

  // Prépare le tableau du montant total et du nombre de chèques
  retour += "<table border=1 cellpadding=1 cellspacing=1>\n";
  retour += "<tr><td><b>" + TR("Nombre de chèques :") + "</b></td><td align=\"center\"><b>" + QString::number(m_Hono_Liste_Depot->count()) + "</b></td>\n";
  retour += "<tr><td><b>" + TR("MONTANT TOTAL :") + "</b></td><td align=\"center\"><b>" + m_LineEdit_Total->text() + "</b></td>\n";
  retour += "</body></html>";
}



/*! \brief SLOT appelé pour créer un fichier contenant la remise de chèques affichée à l'écran.
*/
void Depots_Ui::createFileDepot()
{if (m_Combo_ListeComptes->currentItem() == (m_Combo_ListeComptes->count() - 1))
  QMessageBox::information(this, TR("ERREUR"), TR("Vous devez spécifier un compte bancaire."),TR("Ok"),"","",1,0);
 else if (m_Hono_Liste_Depot->count() == 0)
  QMessageBox::information(this, TR("ERREUR"), TR("Vous devez sélectionner au moins un honoraire."),TR("Ok"),"","",1,0);
 else
 { // Récupère le chemin vers le répertoire de sauvegarde
  QString nFile = G_pCApp->m_PathSauvegarde + QDir::separator() + "Remise-";

  // Récupère la date de la remise
  QDate dt; // date de la validation de la remise ou date du jour si non encore validé
  if (m_Combo_ListeDepots->currentItem() == (m_Combo_ListeDepots->count()-1) )  dt = QDate::currentDate();
  else 
   { DEPOTS_LISTE::iterator it = m_Depots_Liste-> at ( m_Combo_ListeDepots->currentItem() );
     dt = (*it).getDateValid();
   }
  nFile.append(dt.toString("dd-MM-yyyy")+ ".html");

  // Prépare le fichier de sauvegarde
  QFile* f = new QFile(nFile);
  if (f)
  { if (f->exists()) f->remove();
    if ( f->open( IO_WriteOnly ) )
     {QTextStream stream( f );
      QString tmp = "";
      createHonoHTMLTable( tmp, TRUE);
      stream << tmp;
      f->close();
      QMessageBox::information(this, TR("Fichier enregistré."), TR("La remise de chèques a été enregistrée dans le fichier :")+"<br  /><b>"+nFile+"</b>", TR("Ok"), "", "", 1, 0);
      } // Fin if open
     delete f;
   } // Fin if f
 } // Fin if départ
}

/*! \brief SLOT appelé pour imprimer la remise de chèques affichée à l'écran. 
 *  Prépare un HTML avec le tableau de la remise de chèques et imprime.
 *  \todo À améliorer.
*/
void Depots_Ui::imprimeDepot()
{if (m_Combo_ListeComptes->currentItem() == (m_Combo_ListeComptes->count() - 1))
  QMessageBox::information(this, TR("ERREUR"), TR("Vous devez spécifier un compte bancaire."),TR("Ok"),"","",1,0);
 else if (m_Hono_Liste_Depot->count() == 0)
  QMessageBox::information(this, TR("ERREUR"), TR("Vous devez sélectionner au moins un honoraire."),TR("Ok"),"","",1,0);
 else
 { // Prépare painter et printer
  QStringList pages;
  QPainter *p = new QPainter();
  QPrinter *m_printer = new QPrinter();
  if ((m_printer) && (p))
  {m_printer->setup();
   m_printer->setDocName(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss:ms"));
   QFont m_font( "Arial", 10);
   QColorGroup sp_colorgroup (colorGroup());
   sp_colorgroup.setColor (QColorGroup::Text, QColor(0,0,0) );
   sp_colorgroup.setColor (QColorGroup::BrightText, QColor(0,0,0) );
   sp_colorgroup.setColor (QColorGroup::ButtonText, QColor(0,0,0) );
   sp_colorgroup.setColor (QColorGroup::HighlightedText, QColor(0,0,0)); 


   // Récupère le tableau HTML du dépot avec l'entête, 33 chèques par feuilles
   QString depot = "";
   createHonoHTMLTable( depot, TRUE , 33);
   //depot.replace("\n","");
   pages = QStringList::split("<!-- -->", depot);

   p->begin(m_printer);
   for (uint i=0; i < pages.count(); ++i)
   { QSimpleRichText* txt = new QSimpleRichText(pages[i], m_font);
     //int lft = 50 + (600*i);
     txt->draw(p, 50, 50, QRect(50,50,600,1000) , sp_colorgroup );
     p->drawText( 300, 1000, QString::number(i+1)+" / "+QString::number(pages.count()));

     if (i != (pages.count() - 1)) 
     {	m_printer->newPage(); }
   }
   p->end();
   delete p;
   delete m_printer;
  } // Fin p et printer
 } // Fin if départ
}


/*! \brief Rempli le tableau des chèques avec la liste des honoraires de la classe. */
void Depots_Ui::setTableWithHonoListe()
{  // Efface tout le tableau
  for (int i = 0; i < m_TableauDepot->numRows(); ++i)
   {	m_TableauDepot->clearCell(i, 0);
	m_TableauDepot->clearCell(i, 1);
	m_TableauDepot->clearCell(i, 2);
	m_TableauDepot->clearCell(i, 3);
	m_TableauDepot->clearCell(i, 4);
   }

  if (m_Hono_Liste_Depot->count() == 0)
   { m_TableauDepot->setNumRows(1);
     m_TableauDepot->setText(0,0, TR("Aucun chèque") );
     m_TableauDepot->setText(0,1, " " );
     m_TableauDepot->setText(0,2, " " );
     m_TableauDepot->setText(0,3, " " );
     m_TableauDepot->setText(0,4, " " );
     m_LineEdit_Total->setText( "0" );
     m_LineEdit_nbChq->setText( "0" );
     btImp->hide();
     btSauv->hide();
     btFich->hide();
   }
  else
   { int r = 0;
     m_totalDepot = 0;
     HONO_LISTE::iterator it;
     m_TableauDepot->setNumRows( m_Hono_Liste_Depot->count() );
     for (it = m_Hono_Liste_Depot->begin(); it !=  m_Hono_Liste_Depot->end(); ++it )
      { m_TableauDepot->setText( r,0, (*it).getPatient() );
	m_TableauDepot->setText( r,1, (*it).getEmetteur() );
	m_TableauDepot->setText( r,2, (*it).getActesToString() );
	m_TableauDepot->setText( r,3, (*it).getDate().toString("dd-MM-yyyy") );
	m_TableauDepot->setText( r,4, QString::number( (*it).getChq() ) );
	m_totalDepot += (*it).getChq();
	r++;
      }
     m_LineEdit_Total->setText( QString::number ( m_totalDepot ) );
     m_LineEdit_nbChq->setText( QString::number ( m_Hono_Liste_Depot->count() ) );
   }
  m_Label_Periode->setText(m_pComptaMoteurBase->getFiltrePeriode());
}


/*! \brief SLOT appelé lorsque l'utilisateur demande le menu contextuel du tableau de la remise de chèque.
 */
void Depots_Ui::popupTableDepots( int , int c, const QPoint &p )
{ if (m_Popup) delete m_Popup;
  m_Popup = new QPopupMenu(this, "popTable");
  if (m_Popup)
  { m_colTrie=c;
    m_Popup->insertItem (TR("Trier par ordre croissant")   , this, SLOT(trieAsc( ) ) );
    m_Popup->insertItem (TR("Trier par ordre décroissant") , this, SLOT(trieDesc( )  ) );
    if (m_popupEnable)
     {  m_Popup->insertItem (TR("Supprimer ce chèque de la remise"), this, SLOT(chqDelete() ) ); }
    m_Popup->popup(p);
  }
}


/*! \brief Trie le tableau des chèques en fonction de la colonne demandée de façon ascendante */
void Depots_Ui::trieAsc()
{ m_TableauDepot->setSorting( TRUE );
  m_TableauDepot->sortColumn ( m_colTrie, TRUE, TRUE);
  m_TableauDepot->setSorting( FALSE );
}


/*! \brief Trie le tableau des chèques en fonction de la colonne demandée de façon descendante */
void Depots_Ui::trieDesc()
{ m_TableauDepot->setSorting( TRUE );
  m_TableauDepot->sortColumn ( m_colTrie, FALSE, TRUE);
  m_TableauDepot->setSorting( FALSE );
}


/*! \brief SLOT appelé lorsque l'utilisateur désire effacer un chèque de la remise */
void Depots_Ui::chqDelete()
{ // Vérifie qu'une ligne est bien sélectionnée sinon msg erreur
  if (m_TableauDepot->currentSelection () != -1 && m_Hono_Liste_Depot->count() >= 1)
   {// Récupère l'honoraire sélectionné, supprime le et met à jour le tableau
    HONO_LISTE::iterator  it = m_Hono_Liste_Depot->at( m_TableauDepot->currentRow() );
    m_Hono_Liste_Depot->remove ( it );
    setTableWithHonoListe();
   }  // Fin if MsgBox de suppression
}


/*! \brief Récupère la liste des dépôts disponibles pour l'utilisateur et met à jour le combobox. */
void Depots_Ui::getListForComboDepots()
{ m_Depots_Liste = new DEPOTS_LISTE;
  m_Depots_Liste->clear();
  m_Combo_ListeDepots->clear();
  if (!m_pComptaMoteurBase->getDepotsForList( *m_Depots_Liste ) )
   { m_Combo_ListeDepots->insertItem( TR("Nouveau Dépôt") ); }
  else
   if (m_Depots_Liste->count() == 0) m_Combo_ListeDepots->insertItem( TR("Nouveau depôt") );
   else
   { for (DEPOTS_LISTE::iterator it = m_Depots_Liste->begin(); it != m_Depots_Liste->end(); ++it)
      { if ((*it).getRemarque().find(DEPOT_ESPECES) != -1) m_Combo_ListeDepots->insertItem( TR("Dépôt d'espèces du :") + (*it).getDate());
        else m_Combo_ListeDepots->insertItem(TR("Dépôt du : ") + (*it).getDate() ); }
     m_Combo_ListeDepots->insertItem( TR("Nouveau dépôt") );
   }
  m_Combo_ListeDepots->setCurrentItem( m_Combo_ListeDepots->count()-1 );
}



/*! \brief SLOT appelé par le changement de sélection dans le combo des dépôts. Affiche le dépôt dans le tableau.
*/
void Depots_Ui::viewDepot(int id)
{ if (((id == 0) && m_Depots_Liste->count()==0) ||(id > m_Depots_Liste->count()-1 )) // Retour aux honoraires filtrés
   {m_Hono_Liste_Depot->clear();
    m_Combo_ListeDepots->setCurrentItem( m_Depots_Liste->count() );
    // Récupère les honoraires à déposer selon le filtre
    if ( !m_pComptaMoteurBase->getHonorairesForDepot(m_Hono_Liste_Depot) )
	QMessageBox::warning(this, TR("Erreur"),m_pComptaMoteurBase->m_messageErreur, TR("Ok"),"","",1,0);
    else
    { setTableWithHonoListe();
      btSauv->show();
      btImp->show();
      btFich->show();
      m_Label_Periode->setText(m_pComptaMoteurBase->getFiltrePeriode() );
    }
   }
  else // Afficher les dépôts antérieurs
   { m_Hono_Liste_Depot->clear();
     DEPOTS_LISTE::iterator it = m_Depots_Liste->at (id);
     if (m_pComptaMoteurBase->getHonorairesFromBlobDepot( (*it).getListeIdHono() , *m_Hono_Liste_Depot ))
      {	setTableWithHonoListe();
	btSauv->hide();
	btImp->show();
	btFich->show();
	m_popupEnable = FALSE;
	m_Label_Periode->setText(TR("du ") + (*it).getDateDebut().toString("dd MMM yyyy") + 
			 TR(" au ") + (*it).getDateFin().toString("dd MMM yyyy") );
	int findId = m_pComptaMoteurBase->getIdListeFromIdCpt( *m_Comptes_Liste, (*it).getIdCompte() );
	if (findId != -1) m_Combo_ListeComptes->setCurrentItem( findId );
	else m_Combo_ListeComptes->setCurrentItem( m_Combo_ListeComptes->count() - 1 );
      }
   }
}


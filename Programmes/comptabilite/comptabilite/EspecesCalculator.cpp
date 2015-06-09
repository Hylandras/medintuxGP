 /********************************* EspecesCalculator.cpp **********************************
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
/*! \file EspecesCalculator.cpp
    \brief (put some short descripion here).

     #include "EspecesCalculator.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#include "EspecesCalculator.h"

EspecesCalculator::EspecesCalculator( ComptaMoteurBase* pComptaMoteurBase, QWidget* parent, const char* name, bool modal, WFlags fl)
    : QDialog( parent, name, modal, fl )
{ if (G_pCApp->m_Verbose) qWarning("Création de EspecesCalculator");
  setCaption ( TR("Dépôt d'espèces.") );
  m_Comptes_Liste = new COMPTES_LISTE;
  if (!m_Comptes_Liste) return ;
  m_pComptaMoteurBase = pComptaMoteurBase;
  if (!m_pComptaMoteurBase->getComptes(*m_Comptes_Liste))
  { QMessageBox::information(this, TR("ERREUR"), m_pComptaMoteurBase->m_messageErreur, TR("OK"),"","",1,0);
    return ;
  }

  // Prépare le tableau de calcul des espèces
  m_tabEsp = new QTable();
  if (!m_tabEsp) return ;
    m_tabEsp = new QTable(1,12,this); // 4 colonnes
    // structure le tableau
    m_tabEsp->setSelectionMode(QTable::SingleRow);
    m_tabEsp->setReadOnly(FALSE);
    m_tabEsp->setSorting( FALSE );
    m_tabEsp->setMaximumSize( 32767, 70 );
    m_tabEsp->setMinimumSize( 100, 70 );
    m_tabEsp->setBaseSize( 400, 70 );
    m_tabEsp->horizontalHeader()->setLabel( 0, TR( "500" ) );
    m_tabEsp->horizontalHeader()->setLabel( 1, TR( "200" ) );
    m_tabEsp->horizontalHeader()->setLabel( 2, TR( "100" ) );
    m_tabEsp->horizontalHeader()->setLabel( 3, TR( "50" ) );
    m_tabEsp->horizontalHeader()->setLabel( 4, TR( "20" ) );
    m_tabEsp->horizontalHeader()->setLabel( 5, TR( "10" ) );
    m_tabEsp->horizontalHeader()->setLabel( 6, TR( "5" ) );
    m_tabEsp->horizontalHeader()->setLabel( 7, TR( "2" ) );
    m_tabEsp->horizontalHeader()->setLabel( 8, TR( "1" ) );
    m_tabEsp->horizontalHeader()->setLabel( 9, TR( "0.5" ) );
    m_tabEsp->horizontalHeader()->setLabel( 10, TR( "0.2" ) );
    m_tabEsp->horizontalHeader()->setLabel( 11, TR( "0.1" ) );
    // Modifier la géométrie de la table
    for (int i=0; i < 12; ++i)
     { m_tabEsp->setColumnWidth ( i, 50 ); }
    connect( m_tabEsp, SIGNAL( valueChanged(int, int) ), this, SLOT( calculTotalEspeces() ));


  m_LineEdit_Total = new QLineEdit(this);
  m_LineEdit_Total->setText( QString::null);
  QLabel *intro = new QLabel("<p align=\"center\"><b>"+TR("Dépôts d'espèces.")+"</b></p>" , this );
  intro->setMinimumSize( 300, 22 );

  // Insère les comptes bancaires
  QHBox *hb1 = new QHBox(this);
  QLabel *lblListeComptes = new QLabel(hb1);
  lblListeComptes->setText(TR("<b>Compte bancaire pour ce dépôt :</b>"));
  m_Combo_ListeComptes = new QComboBox(hb1);
  // Insérer ici la liste des dépots antérieurs
  getComboComptes();


  // Insère le choix de la date
  QHBox* hb = new QHBox(this);
  hb->setSpacing(10);
  QLabel* lblDate = new QLabel(hb);
  lblDate->setText(TR("Date"));
  m_Combo_Jour = new QComboBox(hb);
  m_Combo_Mois = new QComboBox(hb);
  m_Combo_Mois->insertItem( TR("Janvier") );
  m_Combo_Mois->insertItem( TR("Février") );
  m_Combo_Mois->insertItem( TR("Mars") );
  m_Combo_Mois->insertItem( TR("Avril") );
  m_Combo_Mois->insertItem( TR("Mai") );
  m_Combo_Mois->insertItem( TR("Juin") );
  m_Combo_Mois->insertItem( TR("Juillet") );
  m_Combo_Mois->insertItem( TR("Août") );
  m_Combo_Mois->insertItem( TR("Septembre") );
  m_Combo_Mois->insertItem( TR("Octobre") );
  m_Combo_Mois->insertItem( TR("Novembre") );
  m_Combo_Mois->insertItem( TR("Décembre") );
  m_Combo_Annee = new QComboBox(hb);
  connect( m_Combo_Annee, SIGNAL( activated(int) ), this , SLOT( annee_activated(int) ) );
  connect( m_Combo_Mois,  SIGNAL( activated(int) ), this , SLOT( mois_activated(int) ) );
  setDateObserv(QDate::currentDate() );


  QLabel *l10 = new QLabel("<p align=\'right\'><b>"+TR("Total :")+"</b></p>" , this );

  QHBox* hb2 = new QHBox(this);
  hb2->setSpacing(20);
  butSauvDepot = new QPushButton(hb2);
  butSauvDepot->setText(TR("Sauvegarder"));
  connect ( butSauvDepot, SIGNAL( clicked() ), this, SLOT( saveDepotEspeces() ) );

  butAnnule = new QPushButton(hb2);
  butAnnule->setText( TR("Quitter") );
  connect( butAnnule, SIGNAL( clicked() ), this, SLOT( reject() ) );


  QGridLayout *gridEsp = new QGridLayout (this, 6 , 2, 15, 15, "EspecesLayout");
  gridEsp->addMultiCellWidget ( intro, 0,0, 0, 1);
  gridEsp->addMultiCellWidget ( hb, 1,1,0,1),
  gridEsp->addMultiCellWidget( hb1, 2,2,0,1);

  gridEsp->addMultiCellWidget ( m_tabEsp , 5,5,0,1);

  gridEsp->addWidget ( l10 , 20,0);
  gridEsp->addWidget ( m_LineEdit_Total, 20,1);

  gridEsp->addMultiCellWidget (hb2, 30,30,0,1);

}


EspecesCalculator::~EspecesCalculator()
{ if (G_pCApp->m_Verbose) qWarning("EspecesCalculator::Destruct");
  if (m_tabEsp) delete m_tabEsp;
  if (m_Comptes_Liste) delete m_Comptes_Liste;
  if (G_pCApp->m_Verbose) qWarning("EspecesCalculator::Destruct Fin");

}


/*! \brief Prépare le combobox avec les libellés des comptes bancaires de l'utilisateur.
*/
void EspecesCalculator::getComboComptes()
{ int r = 0;
  int id = 0;
  for (COMPTES_LISTE::iterator it = m_Comptes_Liste->begin(); it != m_Comptes_Liste->end(); ++it)
  { m_Combo_ListeComptes->insertItem( (*it).getLibelle() ); 
    if ((*it).isDefaut()) id=r;
    r++;
  }
  m_Combo_ListeComptes->setCurrentItem(id);
}

double EspecesCalculator::calculTotalEspeces( )
{ double tmp = 0.0;
  int i = 0;
  int nbCol = m_tabEsp->numCols();
  for (i=0;i < nbCol; ++i)
  { tmp += m_tabEsp->horizontalHeader()->label(i).toDouble() * m_tabEsp->text(0,i).toDouble();  }
  if (tmp >= 0.1) m_LineEdit_Total->setText(QString::number( tmp ) );
  else m_LineEdit_Total->setText("0");
  return tmp;
}


QString EspecesCalculator::getForBlobDepot( )
{ QString tmp = "";
  tmp  = DEPOT_ESPECES;
  for (int i=0;i < m_tabEsp->numCols(); ++i)
  { tmp += m_tabEsp->horizontalHeader()->label(i) + "=";
    if (m_tabEsp->text(0,i) == "") tmp += "0;";
    else tmp += m_tabEsp->text(0,i) +";";
  }
  return tmp;
}


void EspecesCalculator::saveDepotEspeces()
{ bool sort = FALSE;
  QString depot = getForBlobDepot();
  Depots* dp = new Depots();
  if (dp)
  { dp->setNom(TR("Dépôt d'espèces du : ") + QDate::currentDate().toString("dd-MM-yyyy") );
    dp->setDateValidation( getDateObserv() );
    dp->setListeIdHono( depot );
    dp->setTotal( calculTotalEspeces() );
    dp->setIdUser( G_pCApp->m_currentUser->getId() );
    COMPTES_LISTE::iterator it = m_Comptes_Liste->at( m_Combo_ListeComptes->currentItem() );
    dp->setIdCompte( (*it).getId() );
    QString rib = DEPOT_ESPECES;
    rib += TOTAL_DEPOT;
    rib += QString::number(calculTotalEspeces());
    dp->setRemarque(rib);
    if (!m_pComptaMoteurBase->saveDepotEspecesToBase( dp ))
	QMessageBox::information(this, "err", m_pComptaMoteurBase->m_messageErreur,"Ok","","",1,0);
    else sort = TRUE;
    delete dp;
  }
  if (sort) accept();
}


/////////////////////////////////////////////////////////////////////////////
////////////////////// Gestion de la date de valeur /////////////////////////
/////////////////////////////////////////////////////////////////////////////
/*! \brief Lorsque l'utilisateur change de mois, recalcul le nombre de jours et met à jour le combo des jours du mois.
 */
void EspecesCalculator::mois_activated( int id )
{  int nbJours = 0;
   int ms = m_Combo_Mois->currentItem() + 1;
   QString toAdd = "";
   int jj = m_Combo_Jour->currentItem();
   m_Combo_Jour->clear();

   if ((ms == 1) ||(ms == 3) ||(ms == 5) ||(ms == 7) ||(ms == 8) ||(ms == 10) ||(ms == 12) )
	nbJours = 31;
   else if ((ms == 2))
   { // Test année bissextile
       if (m_Combo_Annee->currentText().toInt() % 4 == 0) nbJours = 28;
       else nbJours = 27;	
   }
   else nbJours = 30;

   for (int i = 0; i < nbJours; ++i)
    { toAdd = QString::number(i+1);
       if (i < 9) toAdd.prepend("0");
      m_Combo_Jour->insertItem(toAdd);
    }

   if (jj >= m_Combo_Jour->count()) jj = m_Combo_Jour->count() - 1;
   m_Combo_Jour->setCurrentItem(jj);
}


/*! \brief Si l'utilisateur change d'année et que le mois est sur février -> gérer les années bissextiles et mettre à jour le combo des jours.
 */
void EspecesCalculator::annee_activated( int id )
{ if (m_Combo_Mois->currentItem() == 1)     mois_activated( 1 );  }


/*! \brief A tout moment, retourne la date de l'honoraire au format string : "yyyy-MM-dd"
 */
QString EspecesCalculator::getDateObserv()
{ QString tmp = "";
  tmp = QString::number(m_Combo_Mois->currentItem() + 1);
  if (m_Combo_Mois->currentItem() < 9) tmp.prepend("0");
//  return m_Combo_Jour->currentText() + "-" + tmp + "-" + m_Combo_Annee->currentText();
  return m_Combo_Annee->currentText() + "-" + tmp + "-" + m_Combo_Jour->currentText();
}


/*! \brief A tout moment, met les combobox de la date aux bonnes valeurs. La date est passée en paramètre au format "dd-MM-yyyy".
 */
void EspecesCalculator::setDateObserv(QString dt_str)
{ // Passe du format "dd-MM-yyyy" au format "yyyy-MM-dd"
  QString tmp = m_pComptaMoteurBase->dateForSQL( dt_str );
  QDate dt = QDate::fromString(tmp, Qt::ISODate);
  setDateObserv(dt);
}

/*! \brief A tout moment, met les combobox de la date aux bonnes valeurs. La date est passée en paramètre au format "dd-MM-yyyy".
 */
void EspecesCalculator::setDateObserv(QDate dt)
{ int an = dt.year() - 5;
  m_Combo_Annee->clear();
  for (int i = 0; i < 10; i++)
  { m_Combo_Annee->insertItem(QString::number( an + i ));
  }
  an += 5;
  m_Combo_Annee->setCurrentText(QString::number(an));
  m_Combo_Mois->setCurrentItem(dt.month() - 1);
  mois_activated( dt.month() - 1);
  m_Combo_Jour->setCurrentItem( dt.day() - 1);
}


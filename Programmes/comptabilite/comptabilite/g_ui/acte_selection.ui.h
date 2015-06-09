/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/



/*! \class Acte_Selection
    \brief Widget pour sélectionner un acte parmi la liste définie dans la base de données.
*/
void Acte_Selection::init()
{ // Crée la table des honoraires en bas de la fenêtre
  t = new QTable(1, 7, grPaye);
  t->setGeometry( QRect( 20, 30, 350, 60 ) );
  t->horizontalHeader()->setLabel( 0, tr( "Esp" ) );
  t->horizontalHeader()->setLabel( 1, tr( "Chq" ) );
  t->horizontalHeader()->setLabel( 2, tr( "CB" ) );
  t->horizontalHeader()->setLabel( 3, tr( "DAF" ) );
  t->horizontalHeader()->setLabel( 4, tr( "CMU" ) );
    t->horizontalHeader()->setLabel( 5, tr( "Vir" ) );
  t->horizontalHeader()->setLabel( 6, tr( "Autre" ) );
  for (int i = 0; i < 7; i++)
     { t->setColumnWidth ( i, 51 ); }
  // Connecte les signaux de la table avec les slots adéquats
  connect ( t ,     SIGNAL ( clicked(int,int,int,const QPoint&) ) , 
	    this,   SLOT     ( paiement(int,int,int) ) );
}

void Acte_Selection::destroy()
{ //if (m_pActesListe) delete m_pActeListe;
  if (t) delete t;
}


/*! \fn void Acte_Selection::setActesListe( ACTES_LISTE* al)
    \brief Définit la liste des actes à utiliser pour cette classe.
*/
void Acte_Selection::setActesListe( ACTES_LISTE* al)
{ // Charge les valeurs pour le combobox ACTE
  m_pActeListe = al;
  findTypes();
  CmbType_activated( typesActes[0] );
  putActeParams(0);
}


/*! \fn void Acte_Selection::findTypes()
    \brief Recherche dans la liste des actes disponibles les différents types d'actes et sauvegarde le résultat dans la vairable protected \em typesActes . Met à jour le combo des types d'actes.
*/
void Acte_Selection::findTypes()
{ QString tmp = "";
  typesActes.clear();
  ACTES_LISTE::iterator it;
  for (it = m_pActeListe->begin(); it !=  m_pActeListe->end(); ++it )
   { tmp = (*it).getType();
     if (typesActes.isEmpty() )
	{ typesActes.append( tmp ); CmbType->insertItem( tmp );  }
     else
      {	// Vérifier que le type n'est déjà dans la liste
	if ( typesActes.contains( tmp ) == 0 )
	 { typesActes.append ( tmp ); CmbType->insertItem( tmp );  }
       }
   }
}


/*! \fn void Acte_Selection::putActeParams (int id )
    \brief SLOT. Insère dans les champs adéquats du plugins les variables (descriptif, total, tiers) de l'acte choisi dans le combo.
*/
void Acte_Selection::putActeParams (int id )
{ACTES_LISTE::iterator it = actesSelonType.at(id);
  QString descr = "";
  descr = (*it).m_Desc + "<br />" + (*it).m_Type;
  lblDescrActe->setText(descr);
  descr = TR("Montant Total : ") + QString::number((*it).m_Total);
  descr += TR("  ;  Montant du Tiers : ") + QString::number( (*it).m_Tiers ) ;
  lblMontant->setText(descr);
  montantTotal->setText( QString::number( (*it).m_Total ) );
}


/*! \fn void Acte_Selection::paiement(int r ,int c, int but)
    \brief SLOT appelé lorsque l'utilisateur clique sur une case du tableau de paiement. Les montants sont automatiquement calculés et insérés.
*/
void Acte_Selection::paiement(int r ,int c, int but)
{ if (but == Qt::RightButton)
  { // Mets toutes la ligne à zéro
    for (int i=0; i < 7; i++)
     { t->setText(r,i,QString::null); }

     // Récupère les données inhérentes à l'acte sélectionné
     ACTES_LISTE::iterator it = actesSelonType.at( CmbActe->currentItem() );
     double total = (*it).m_Total; 
     double tiers = (*it).m_Tiers;

     // Mettre dans la case le montant selon : Montant total et Tiers
     if (chkTiersPayant->isChecked())
      { if (c != 3) // S'assurer que l'utilisateur ne clique pas sur DAF...
        { t->setText(r, c, QString::number(tiers + (montantTotal->text().toDouble() - total) ) );  // A Payer
          t->setText(r, 3, QString::number(total-tiers) );  // DAF
        }
        else // tout mettre en DAF
        { t->setText (r, 3, QString::number(total) );
        }
        
		if (c != 4) // S'assurer que l'utilisateur ne clique pas sur DAF...
        { t->setText(r, c, QString::number(tiers + (montantTotal->text().toDouble() - total) ) );  // A Payer
          t->setText(r, 4, QString::number(total-tiers) );  // DAF
        }
		else // tout mettre en DAF
        { t->setText (r, 4, QString::number(total) );
        }

     }  // Fin tiers cliqué


    else 
     { t->setText(r, c, montantTotal->text() );  }
  } // fin Qt::LeftButton

}

void Acte_Selection::Annule_clicked()
{  accept();  }


void Acte_Selection::OK_clicked()
{  QString z = "";
   ACTES_LISTE::iterator it = actesSelonType.at( CmbActe->currentItem() );
   // Emet le signal actePaiementSelectionne( ); avec les données stockées dans une classe Honoraires
    Paiements *p = new Paiements ();
    p->setNom		( (*it).m_Nom );
    p->setType		( (*it).m_Type );
    if (chkTiersPayant->isChecked()) p->setTiers(TRUE);
    p->setEspeces	( t->text (0, 0 ).toDouble() );
    p->setCheque	( t->text (0, 1 ).toDouble() );
    p->setCB		( t->text (0, 2 ).toDouble() );
    p->setDAF		( t->text (0, 3 ).toDouble() );
    p->setAutre		( t->text (0, 6 ).toDouble() );
    p->setCMU		( t->text (0, 4 ).toDouble() );
    p->setVirement	( t->text (0, 5 ).toDouble() );
    if ( p->total() == 0 ) 
     { if ( QMessageBox::question(
            this,
            tr("Pas de montant sélectionné."),
            tr("Vous n'avez pas indiqué de montant à l'acte. <br />Souhaitez-vous enregistrer un acte gratuit ?"),
            tr("&Oui"), tr("&Non"),
            QString::null, 0, 1 ) == 0)
        {  emit actePaiementSelectionne( p );  accept();  }
      }
    else {  emit actePaiementSelectionne( p );  accept();  }
}


/*! \fn void Acte_Selection::CmbType_activated( const QString &str )
    \brief SLOT appelé par le combobox Type d'acte lors d'un changement de type. Fonction = Met à jour le combobox des actes selon le type sélectionné.
*/
void Acte_Selection::CmbType_activated( const QString &str )
{ QString tmp = "";
  actesSelonType.clear();
  ACTES_LISTE::iterator it;
  for (it = m_pActeListe->begin(); it !=  m_pActeListe->end(); ++it )
   { tmp = (*it).m_Type;
     if ( tmp == str )  { actesSelonType.append ( (*it) );  }
   }
  // Met à jour le combo
  updateCmbActe();
}


/*! \fn void Acte_Selection::updateCmbActe()
    \brief Met à jour le combobox des actes après avoir mis à jour la liste des actes selon le type sélectionné.
*/
void Acte_Selection::updateCmbActe()
{ int i = 0;
  CmbActe->clear();
  ACTES_LISTE::iterator it;
  for (it = actesSelonType.begin(); it !=  actesSelonType.end(); ++it )
   { CmbActe->insertItem( (*it).m_Nom , i);   ++i;  }
  CmbActe->setCurrentItem(0);
  putActeParams(0);
}


void Acte_Selection::setPaiement(Paiements &p)
{ QString tmp = "";
  int id = 0 ;
  // Trouve les id des : type et acte
  for ( QStringList::Iterator it = typesActes.begin(); it != typesActes.end(); ++it )
     { if ( (*it) == p.m_Type ) { break; }
       id++;
     }
  CmbType->setCurrentItem( id );
  CmbType_activated( CmbType->currentText() );
  id = 0;
  ACTES_LISTE::iterator ut;
  for (ut = actesSelonType.begin(); ut !=  actesSelonType.end(); ++ut )
   { tmp = (*ut).m_Nom;
     if ( tmp == p.m_Nom )  { break;  }
     id++;
   }
  // Met l'acte à jour
  putActeParams( id );
  CmbActe->setCurrentItem( id );
  if (p.m_Esp > 0)   t->setText(0,0, QString::number ( p.m_Esp) );
  if (p.m_Chq > 0)   t->setText(0,1, QString::number ( p.m_Chq) );
  if (p.m_CB > 0)    t->setText(0,2, QString::number ( p.m_CB) );
  if (p.m_DAF > 0)   t->setText(0,3, QString::number ( p.m_DAF) );
  if (p.m_Autre > 0) t->setText(0,6, QString::number ( p.m_Autre) );
  if (p.m_CMU > 0)   t->setText(0,4, QString::number ( p.m_CMU) );
  if (p.m_Virement > 0)   t->setText(0,5, QString::number ( p.m_Virement) );
  if (p.m_Tiers)     chkTiersPayant->setChecked(TRUE);
}




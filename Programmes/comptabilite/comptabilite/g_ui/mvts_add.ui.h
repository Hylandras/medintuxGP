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



void Mvts_Add::init()
{ m_pComptaMoteurBase = 0;
  m_pUtilisateurActuel = 0;
  m_pMvts_Sauvegarde = 0;
  paramOK = FALSE;
  forUpdate = FALSE;
  seeOnly = FALSE;
  categorieSelected = FALSE;
  mvt = new Mouvements();
  if (!mvt) return ;
  m_Mvts_Liste_Dispo = new MVTS_LISTE;
  if (!m_Mvts_Liste_Dispo) return ;
  m_Mvts_Liste_Dispo->clear();
  m_Comptes_Liste = new COMPTES_LISTE;
  if (!m_Comptes_Liste) return ;
  m_Comptes_Liste->clear();

}

void Mvts_Add::destroy()
{ if (m_Verbose) qWarning("Mvts_Add::Destruct");
  if (mvt) delete mvt;
  if (m_Mvts_Liste_Dispo) delete m_Mvts_Liste_Dispo;
  if (m_Comptes_Liste) delete m_Comptes_Liste;
  if (m_Verbose) qWarning("Mvts_Add::Destruct Fin");
}


/*! \fn void Mvts_Add::setParams()
 *  \brief Avant d'afficher la classe, il faut lui passer les paramètres (patient, guid, praticien...).
 */
void Mvts_Add::setParams(	Utilisateurs*	curUsr, 
				const char*	date,			// format "dd-MM-yyyy"
				ComptaMoteurBase* pMoteurBase,
				bool		 verbose )
{ m_pComptaMoteurBase = pMoteurBase;
  m_pUtilisateurActuel = curUsr;
  m_Verbose = verbose;
  setDateObserv(date);
  m_Mvts_Liste_Dispo->clear();
  if (!m_pComptaMoteurBase->getMvtsDispo(*m_Mvts_Liste_Dispo))
   QMessageBox::information(this, TR("Erreur de base de données"), m_pComptaMoteurBase->m_messageErreur, TR("Ok"),"","",1,0);
  if (!m_pComptaMoteurBase->getComptes(*m_Comptes_Liste))
   QMessageBox::information(this, TR("Erreur de base de données"), m_pComptaMoteurBase->m_messageErreur, TR("Ok"),"","",1,0);
  int defautCpt = 0;
  int i = 0;
  for (COMPTES_LISTE::iterator it=m_Comptes_Liste->begin(); it!=m_Comptes_Liste->end();++it)
   { listeComptes->insertItem( (*it).getLibelle() ); 
     if ((*it).isDefaut()) defautCpt=i;
     i++;
   }
  listeComptes->setCurrentItem(defautCpt);
  paramOK = TRUE;
}



void Mvts_Add::choixCategorie_clicked()
{ QDialog* dlg = new QDialog(this);
  QListView* treeView = new QListView(dlg);
  treeView->addColumn( TR("Libellé") );
  treeView->addColumn( TR("Id") );
  QListViewItem*  recettes = new QListViewItem(treeView, TR("Recettes") );
  QListViewItem*  depenses = new QListViewItem(treeView, TR("Dépenses"));

  QString tmp = "";
  if ((m_Mvts_Liste_Dispo->count() == 0) || (!m_Mvts_Liste_Dispo))
   { //treeView->setNumRows( 1 );
   }
  else
   { MVTS_LISTE::iterator it;

     // Prépare l'affichage du TreeView
     for (it = m_Mvts_Liste_Dispo->begin(); it !=  m_Mvts_Liste_Dispo->end(); ++it )
      { // Prépare l'affichage de le treeView
	int id = (*it).getId();
	if (id < 10) tmp = "0" + QString::number(id);
	else tmp = QString::number(id);
	if ((*it).getType() == 0)  // Recettes
	{ // Test si child ou parent
	   if ( (*it).getIdParent() == 0)
	   {  new QListViewItem(recettes, (*it).getNom(),  tmp ); 
	   }
	   else
	   {	// Récupère le nom du parent pour le retrouver dans le treeView
		QString nom_parent = (*m_Mvts_Liste_Dispo->at( (*it).getIdParent() - 1 )).getNom();
		QListViewItem* parent = treeView->findItem(nom_parent, 0);
		if (parent) new QListViewItem( parent , (*it).getNom() ,  tmp );
		else new QListViewItem( recettes , (*it).getNom() ,  tmp );
	   }
	 } // Fin type = recettes
	else   // Dépenses
	 { // Test si child ou parent
	   if ( (*it).getIdParent() == 0)
	   {  new QListViewItem(depenses, (*it).getNom() ,  tmp ); 
	   }
	   else
	   {	// Récupère le nom du parent pour le retrouver dans le treeView
		QString nom_parent = (*m_Mvts_Liste_Dispo->at( (*it).getIdParent() - 1 )).getNom();
		QListViewItem* parent = treeView->findItem(nom_parent, 0);
		if (parent) new QListViewItem( parent , (*it).getNom() ,  tmp );
		else new QListViewItem( depenses , (*it).getNom() ,  tmp );
	   }
	 } // Fin else type == 0
      } // Fin for
   treeView->insertItem(recettes) ;
   treeView->insertItem(depenses);
   treeView->setSorting(1,TRUE);
   }

  // Ajoute bouttons
  QPushButton* okBut = new QPushButton(TR("Valider"), dlg);
  QPushButton* cancelBut = new QPushButton(TR("Annuler"), dlg);
  connect ( okBut, SIGNAL(clicked()), dlg, SLOT( accept() ) );
  connect ( cancelBut, SIGNAL (clicked() ), dlg, SLOT( reject() ) );
//  connect ( treeView, SIGNAL (doubleClicked()), dlg, SLOT( accept() ) );

  QGridLayout *grid = new QGridLayout( dlg, 5, 5, 11, 6, "Layout_CMDI_Mouvements"); 
  grid->addWidget( treeView, 0, 0);
  grid->addWidget( okBut, 1, 0);
  grid->addWidget( cancelBut, 2, 0);
  dlg->exec();

  // Selon le mode de sortie du dialogue, récupère le mouvement sélectionné
  if (dlg->result() == QDialog::Accepted )
  { id_mvt_dispo = treeView->selectedItem()->text(1).toInt();
    bool gotChild = FALSE;
    // Ne pas permettre la sélection de catégorie primaire (celle possédant une sous-rubrique)
    for (MVTS_LISTE::iterator it_disp = m_Mvts_Liste_Dispo->begin(); it_disp != m_Mvts_Liste_Dispo->end(); ++it_disp)
     { if ( (*it_disp).getIdParent() > 0 )
	if ( (*it_disp).getIdParent() == id_mvt_dispo)
	 { gotChild = TRUE; break; }
     }
    if (!gotChild) { categorie->setText(treeView->selectedItem()->text(0)); categorieSelected = TRUE; }
    else categorieSelected = FALSE;
  }
  else categorieSelected = FALSE;
  delete dlg;
}


void Mvts_Add::setMvt(Mouvements* pMvt)
{libelle->setText( pMvt->getNom() );
 categorie->setText( (*m_Mvts_Liste_Dispo->at( pMvt->getIdParent() - 1 )).getNom() );
 montant->setText( QString::number(pMvt->getTotal()) );
 setDateObserv( pMvt->getDateValeur() );
 id_mvt_dispo = pMvt->getIdParent() ;
 id_mvt_toModif = pMvt->getId();
 categorieSelected = TRUE;
 QString tmp = pMvt->getRemarque();
 if (tmp == PAIEMENT_ESPECES)
  modePaiement->setCurrentItem(1);
 else  if (tmp == PAIEMENT_CHEQUE)
  modePaiement->setCurrentItem(2);
 else  if (tmp == PAIEMENT_VIREMENT)
  modePaiement->setCurrentItem(3);
 else  if (tmp == PAIEMENT_CB)
  modePaiement->setCurrentItem(4);
 else  if (tmp == PAIEMENT_AUTRE)
  modePaiement->setCurrentItem(5);
 m_pMvts_Sauvegarde = pMvt;
}


void Mvts_Add::setSeeOnly(bool b)
{ seeOnly = b; 
  if (seeOnly)
  { choixCategorie->hide();
    
  }
  else 
  { choixCategorie->show();
  }

}


void Mvts_Add::getMouvement()
{if (mvt)
 { if (forUpdate) mvt->setId( id_mvt_toModif );
   mvt->setNom( libelle->text() );
   mvt->setIdUsr( m_pUtilisateurActuel->getId() );
   mvt->setDate( QDate::currentDate().toString("yyyy-MM-dd") );
   mvt->setDateValeur( getDateObserv() ); // format yyyy-MM-dd
   mvt->setTotal( montant->text().toDouble() );
   mvt->setIdParent( id_mvt_dispo );
   mvt->setType ( (*m_Mvts_Liste_Dispo->at( id_mvt_dispo - 1 )).getType() );
   mvt->setIdCompte( (*m_Comptes_Liste->at( listeComptes->currentItem() )).getId() );
   QString tmp;
   switch (modePaiement->currentItem())
   {  case 1 :
	{ // Especes
	  tmp = PAIEMENT_ESPECES;
	  break;
	}
      case 2 :
	{ // Chèques
	  tmp = PAIEMENT_CHEQUE;
	  break;
	}
      case 3 :
	{ // Virement
	  tmp = PAIEMENT_VIREMENT;
	  break;
	}
      case 4 :
	{ // CB
	  tmp = PAIEMENT_CB;
	  break;
	}
      case 5 :
	{ // Autre
	  tmp = PAIEMENT_AUTRE;
	  break;
	}
   }// End switch
   mvt->setRemarque(tmp);
 }

}


void Mvts_Add::buttonOk_clicked()
{if (seeOnly)
 { accept(); }
 else
 {// Vérifie les champs obligatoires : nom, praticien, date
  if (libelle->text().stripWhiteSpace() == "") 
   QMessageBox::information(this, TR("Formulaire incomplet."), TR("Vous devez obligatoirement renseigner le libellé de la transaction."),TR("Ok"),"","",1,0);
  else if ((categorie->text().stripWhiteSpace() == "") || (!categorieSelected))
   QMessageBox::information(this, TR("Formulaire incomplet."), TR("Vous devez obligatoirement renseigner la catégorie de la transaction à l'aide de la fenêtre prévue à cet effet. Cliquez sur le bouton \"Choisir\"."),TR("Ok"),"","",1,0);
  else if (montant->text().stripWhiteSpace() == "")
   QMessageBox::information(this, TR("Formulaire incomplet."), TR("Vous devez obligatoirement renseigner un montant à la transaction."),TR("Ok"),"","",1,0);
  else if (modePaiement->currentItem() == 0)
   QMessageBox::information(this, TR("Formulaire incomplet."), TR("Vous devez obligatoirement spécifier le mode de paiement de la transaction."),TR("Ok"),"","",1,0);
  else
   { QString msg = "";
     getMouvement();
     bool sansErreur = FALSE;
     if (!forUpdate)
     {	sansErreur = m_pComptaMoteurBase->saveMvtToBase(mvt);
	msg = TR("Mouvement sauvegardé dans la base de données sans erreur.");
     }
     else
     {	mvt->traceModifications( m_pMvts_Sauvegarde, m_pUtilisateurActuel, 
				 &(*m_Mvts_Liste_Dispo->at( m_pMvts_Sauvegarde->getIdParent()-1)) ,
				 &(*m_Mvts_Liste_Dispo->at( mvt->getIdParent()-1)), 
				 *m_Comptes_Liste );
	sansErreur = m_pComptaMoteurBase->updateMvt(mvt);
	msg = TR("Mouvement mis à jour dans la base de données sans erreur.");
     }
     if (!sansErreur)
       // une erreur est survenue --> afficher l'erreur.
      {	QMessageBox *b = new QMessageBox(this);
	b->setText( m_pComptaMoteurBase->m_messageErreur );
	b->exec();
	delete b;
      }
     else 
      {	QMessageBox::information(this, TR("Information"), msg , TR("Ok"),"","",1,0);
	accept();
      }
   }
 } // Fin if seeOnly
}

void Mvts_Add::buttonCancel_clicked()
{ reject(); }


void Mvts_Add::buttonHelp_clicked()
{

}

/////////////////////////////////////////////////////////////////////////////
////////////////////// Gestion de la date de valeur /////////////////////////
/////////////////////////////////////////////////////////////////////////////
/*! \fn void Mvts_Add::mois_activated( int  )
 *  \brief Lorsque l'utilisateur change de mois, recalcul le nombre de jours et met à jour le combo des jours du mois.
 */
void Mvts_Add::mois_activated( int  )
{  int nbJours = 0;
   int ms = mois->currentItem() + 1;
   QString toAdd = "";
   int jj = jour->currentItem();
   jour->clear();

   if ((ms == 1) ||(ms == 3) ||(ms == 5) ||(ms == 7) ||(ms == 8) ||(ms == 10) ||(ms == 12) )
	nbJours = 31;
   else if ((ms == 2))
   { // Test année bissextile
       if (annee->currentText().toInt() % 4 == 0) nbJours = 29;
       else nbJours = 28;
   }
   else nbJours = 30;

   for (int i = 0; i < nbJours; ++i)
    { toAdd = QString::number(i+1);
       if (i < 9) toAdd.prepend("0");
      jour->insertItem(toAdd);
    }

   if (jj >= jour->count()) jj = jour->count() - 1;
   jour->setCurrentItem(jj);
}


/*! \fn void Mvts_Add::annee_activated( int  )
 *  \brief Si l'utilisateur change d'année et que le mois est sur février -> gérer les années bissextiles et mettre à jour le combo des jours.
 */
void Mvts_Add::annee_activated( int  )
{ if (mois->currentItem() == 1)     mois_activated( 1 );  }


/*! \fn QString Mvts_Add::getDateObserv()
 *  \brief A tout moment, retourne la date de l'honoraire au format string : "yyyy-MM-dd"
 */
QString Mvts_Add::getDateObserv()
{ QString tmp = "";
  tmp = QString::number(mois->currentItem() + 1);
  if (mois->currentItem() < 9) tmp.prepend("0");
//  return jour->currentText() + "-" + tmp + "-" + annee->currentText();
  return annee->currentText() + "-" + tmp + "-" + jour->currentText();
}


/*! \fn void Mvts_Add::setDateObserv(QString dt_str)
 *  \brief A tout moment, met les combobox de la date aux bonnes valeurs. La date est passée en paramètre au format "dd-MM-yyyy".
 */
void Mvts_Add::setDateObserv(QString dt_str)
{ // Passe du format "dd-MM-yyyy" au format "yyyy-MM-dd"
  QString tmp = m_pComptaMoteurBase->dateForSQL( dt_str );
  QDate dt = QDate::fromString(tmp, Qt::ISODate);
  setDateObserv(dt);
}

/*! \fn void Mvts_Add::setDateObserv(QDate dt)
 *  \brief A tout moment, met les combobox de la date aux bonnes valeurs. La date est passée en paramètre au format "dd-MM-yyyy".
 */
void Mvts_Add::setDateObserv(QDate dt)
{ int an = dt.year() - 5;
  annee->clear();
  for (int i = 0; i < 10; i++)
  { annee->insertItem(QString::number( an + i ));
  }
  an += 5;
  annee->setCurrentText(QString::number(an));
  mois->setCurrentItem(dt.month() - 1);
  mois_activated( dt.month() - 1);
  jour->setCurrentItem( dt.day() - 1);
}


/*! \fn void Mvts_Add::setForUpdateOnly(bool up)
 *  \brief A tout moment, met les combobox de la date aux bonnes valeurs. La date est passée en paramètre au format "dd-MM-yyyy".
 */
void Mvts_Add::setForUpdateOnly(bool up)
{ forUpdate = up; }


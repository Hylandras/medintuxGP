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




void Immobilisations_Add::init()
{ pMB = 0;
  currentUser = 0;
  paramOK = FALSE;
  forUpdate = FALSE;
  seeOnly = FALSE;
  imm = new Immobilisations();
  if (!imm) return ;
  cptListe = new COMPTES_LISTE;
  if (!cptListe) return ;
  cptListe->clear();
  residuel->hide();
}

void Immobilisations_Add::destroy()
{ if (imm) delete imm;
  if (cptListe) delete cptListe;
}


/*! \fn void FormPlugins::setParams()
 *  \brief Avant d'afficher la classe, il faut lui passer les paramètres (patient, guid, praticien...).
 */
void Immobilisations_Add::setParams(	Utilisateurs*	curUsr, 
				const char*	date,			// format "dd-MM-yyyy"
				ComptaMoteurBase* pMoteurBase)
{ pMB = pMoteurBase;
  currentUser = curUsr;
  setDateObserv(date);
  if (!pMB->getComptes(*cptListe))
   QMessageBox::information(this, TR("Erreur de base de données"), pMB->m_messageErreur, TR("Ok"),"","",1,0);
  int defautCpt = 0;
  int i = 0;
  for (COMPTES_LISTE::iterator it=cptListe->begin(); it!=cptListe->end();++it)
   { listeComptes->insertItem( (*it).getLibelle() ); 
     if ((*it).isDefaut()) defautCpt=i;
     i++;
   }
  listeComptes->setCurrentItem(defautCpt);
  paramOK = TRUE;
}


void Immobilisations_Add::setImm(Immobilisations* pImm)
{libelle->setText( pImm->getNom() );
 valeur->setText( QString::number(pImm->getValeur()) );
 setDateObserv( pImm->getDateService() );
 id_imm_toModif = pImm->getId();
 QString tmp = pImm->getRemarque();
 duree->setText( QString::number(pImm->getDuree()) );
 resultatTxt->setText( pImm->getResultat() );
 if (pImm->getMode() == 0) 
  { lineaire->setChecked(TRUE);  degressif->setChecked(FALSE); }
 else { lineaire->setChecked(FALSE);  degressif->setChecked(TRUE); }
 residuel->setText( QString::number(pImm->getResiduel()) );
}


void Immobilisations_Add::setSeeOnly(bool b)
{ seeOnly = b; }


void Immobilisations_Add::getImmobilisation()
{if (imm)
 { if (forUpdate) imm->setId( id_imm_toModif );
   imm->setIdUsr( currentUser->getId() );
   imm->setIdCompte( (*cptListe->at( listeComptes->currentItem() )).getId() );
   imm->setNom( libelle->text() );
   imm->setDateService( getDateObserv() ); // format yyyy-MM-dd
   imm->setDuree( duree->text().toInt() );
   imm->setResultat( resultatTxt->text() );

   if (lineaire->isChecked()) imm->setMode ( 0 );
   else imm->setMode(1);

   imm->setValeur( valeur->text().toInt() );
   imm->setResiduel( residuel->text().toInt() );
   imm->setRemarque("");
 }

}


void Immobilisations_Add::buttonOk_clicked()
{if (seeOnly)
 { accept(); }
 else
 {// Vérifie les champs obligatoires : nom, praticien, date
  if (libelle->text().stripWhiteSpace() == "") 
   QMessageBox::information(this, TR("Formulaire incomplet."), TR("Vous devez obligatoirement renseigner le libellé de la transaction."),TR("Ok"),"","",1,0);
//   else if ((categorie->text().stripWhiteSpace() == "") || (!categorieSelected))
//    QMessageBox::information(this, TR("Formulaire incomplet."), TR("Vous devez obligatoirement renseigner la catégorie de la transaction à l'aide de la fenêtre prévue à cet effet. Cliquez sur le bouton \"Choisir\"."),TR("Ok"),"","",1,0);
//   else if (montant->text().stripWhiteSpace() == "")
//    QMessageBox::information(this, TR("Formulaire incomplet."), TR("Vous devez obligatoirement renseigner un montant à la transaction."),TR("Ok"),"","",1,0);
//   else if (modePaiement->currentItem() == 0)
//    QMessageBox::information(this, TR("Formulaire incomplet."), TR("Vous devez obligatoirement spécifier le mode de paiement de la transaction."),TR("Ok"),"","",1,0);
  else
   { QString msg = "";
     getImmobilisation();
     bool sansErreur = FALSE;
     if (!forUpdate)
     {	sansErreur = pMB->saveImmobToBase(imm);
	msg = TR("Mouvement sauvegardé dans la base de données sans erreur.");
     }
     else
     {	sansErreur = pMB->updateImmob(imm);
	msg = TR("Mouvement mis à jour dans la base de données sans erreur.");
     }
     if (!sansErreur)
       // une erreur est survenue --> afficher l'erreur.
      {	QMessageBox *b = new QMessageBox(this);
	b->setText( pMB->m_messageErreur );
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



void Immobilisations_Add::but_Calculer_clicked()
{ getImmobilisation();
  imm->prepareResultat();
  resultatTxt->setText( imm->getResultat() );
}


void Immobilisations_Add::buttonCancel_clicked()
{ reject(); }


void Immobilisations_Add::buttonHelp_clicked()
{

}

/////////////////////////////////////////////////////////////////////////////
////////////////////// Gestion de la date de valeur /////////////////////////
/////////////////////////////////////////////////////////////////////////////
/*! \fn void Immobilisations_Add::mois_activated( int  )
 *  \brief Lorsque l'utilisateur change de mois, recalcul le nombre de jours et met à jour le combo des jours du mois.
 */
void Immobilisations_Add::mois_activated( int  )
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


/*! \fn void Immobilisations_Add::annee_activated( int  )
 *  \brief Si l'utilisateur change d'année et que le mois est sur février -> gérer les années bissextiles et mettre à jour le combo des jours.
 */
void Immobilisations_Add::annee_activated( int  )
{ if (mois->currentItem() == 1)     mois_activated( 1 );  }


/*! \fn QString FormPlugins::getDateObserv()
 *  \brief A tout moment, retourne la date de l'honoraire au format string : "yyyy-MM-dd"
 */
QString Immobilisations_Add::getDateObserv()
{ QString tmp = "";
  tmp = QString::number(mois->currentItem() + 1);
  if (mois->currentItem() < 9) tmp.prepend("0");
//  return jour->currentText() + "-" + tmp + "-" + annee->currentText();
  return annee->currentText() + "-" + tmp + "-" + jour->currentText();
}


/*! \fn void FormPlugins::setDateObserv(QString dt_str)
 *  \brief A tout moment, met les combobox de la date aux bonnes valeurs. La date est passée en paramètre au format "dd-MM-yyyy".
 */
void Immobilisations_Add::setDateObserv(QString dt_str)
{ // Passe du format "dd-MM-yyyy" au format "yyyy-MM-dd"
  QString tmp = pMB->dateForSQL( dt_str );
  QDate dt = QDate::fromString(tmp, Qt::ISODate);
  setDateObserv(dt);
}

/*! \fn void FormPlugins::setDateObserv(QDate dt)
 *  \brief A tout moment, met les combobox de la date aux bonnes valeurs. La date est passée en paramètre au format "dd-MM-yyyy".
 */
void Immobilisations_Add::setDateObserv(QDate dt)
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


/*! \fn void Immobilisations_Add::setForUpdateOnly(bool up)
 *  \brief A tout moment, met les combobox de la date aux bonnes valeurs. La date est passée en paramètre au format "dd-MM-yyyy".
 */
void Immobilisations_Add::setForUpdateOnly(bool up)
{ forUpdate = up; }


void Immobilisations_Add::lineaire_toggled( bool b )
{ if (b) residuel->show(); else residuel->hide();  }



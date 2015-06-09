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




void Cpts_Add::init()
{ pMB = 0;
  currentUser = 0;
  paramOK = FALSE;
  forUpdate = FALSE;
  seeOnly = FALSE;
  cpt = new Comptes();
  if (!cpt) return ;
}

void Cpts_Add::destroy()
{ if (cpt) delete cpt;
}


/*! \fn void FormPlugins::setParams()
 *  \brief Avant d'afficher la classe, il faut lui passer les paramètres (patient, guid, praticien...).
 */
void Cpts_Add::setParams(	Utilisateurs*	curUsr, 
				ComptaMoteurBase* pMoteurBase)
{ pMB = pMoteurBase;
  currentUser = curUsr;
  paramOK = TRUE;
}

void Cpts_Add::setSeeOnly(bool s)
{ seeOnly = s; }



void Cpts_Add::setCpt(Comptes* pCpt)
{libelle->setText( pCpt->getLibelle() );
 banque->setText( pCpt->getNomBanque() );
 ribBanque->setText( pCpt->getRibBanque() );
 ribGuichet->setText( pCpt->getRibGuichet() );
 ribNumCpt->setText( pCpt->getRibCompte() );
 ribCle->setText( pCpt->getRibCle() );
 solde->setText( QString::number(pCpt->getSolde()) );
 if (pCpt->isDefaut())  defaut->setChecked(TRUE);
 else defaut->setChecked(FALSE);
 titulaire->setText( pCpt->getTitulaire() );
 id_cpt_toModif = pCpt->getId();
}



void Cpts_Add::getCompte()
{if (cpt)
 { if (forUpdate) cpt->setId( id_cpt_toModif );
   cpt->setLibelle( libelle->text() );
   cpt->setNomBanque( banque->text() );
   cpt->setIdUsr( currentUser->getId() );
   cpt->setRib ( ribBanque->text(), ribGuichet->text(), ribNumCpt->text(), ribCle->text() );
   if (defaut->isChecked()) cpt->setRemarque( CPT_PAR_DEFAUT );
   cpt->setTitulaire( titulaire->text() );
   cpt->setSolde( solde->text().toDouble() );
 }
}


void Cpts_Add::buttonOk_clicked()
{ if (seeOnly) accept();
  else
  { // Vérifie les champs obligatoires : nom, praticien, date
    if (libelle->text().stripWhiteSpace() == "") 
     QMessageBox::information(this, TR("Formulaire incomplet."), TR("Vous devez obligatoirement renseigné le libellé du compte."),TR("Ok"),"","",1,0);
    else
    { QString msg = "";
      getCompte();
      bool sansErreur = FALSE;
      if (!forUpdate)
      {	sansErreur = pMB->saveCptToBase(cpt);
 	msg = TR("Compte bancaire sauvegardé dans la base de données sans erreur.");
      }
      else
      {	if ( QMessageBox::information(this, TR("AVERTISSEMENT"), TR("Attention, vous allez modifier les paramètres d'un compte bancaire.\n Ces modifications affecteront tous les mouvements actuellement enregistrés sur ce compte.\nSouhaitez-vous poursuivre ?\n"), TR("Valider"), TR("Annuler"), "", 1,0) == 0)
 	{ sansErreur = pMB->updateCpt(cpt);
 	  msg = TR("Compte Bancaire mis à jour dans la base de données sans erreur.");
	}
	else sansErreur = TRUE;
      }
      if (!sansErreur)
        // une erreur est survenue --> afficher l'erreur.
       { QMessageBox *b = new QMessageBox(this); 
	 b->setText( pMB->m_messageErreur ); 
	 b->exec() ;
	 delete b;
       }
      else if (msg != "")
      {	QMessageBox::information(this, TR("Information"), msg , TR("Ok"),"","",1,0);
	accept();
      }
    }
  } // fin seeonly
}

void Cpts_Add::buttonCancel_clicked()
{ reject(); }


void Cpts_Add::buttonHelp_clicked()
{

}

/*! \fn void Cpts_Add::setForUpdateOnly(bool up)
 *  \brief A tout moment, met les combobox de la date aux bonnes valeurs. La date est passée en paramètre au format "dd-MM-yyyy".
 */
void Cpts_Add::setForUpdateOnly(bool up)
{ forUpdate = up; }


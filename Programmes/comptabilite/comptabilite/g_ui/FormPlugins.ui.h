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


/*! \file
 * Contient les fonctions de la classe FormPlugins  (FormPlugins.ui et FormPlugins.ui.h)
*/
/*! \class FormPlugins
 *  \brief Fenêtre de saisie d'acte utilisée lors de l'utilisation de compta sous forme de plugins de MedinTux.
 *  \author Eric MAEKER, 2007, 2008
 *  \version 1.0 RC 2

 *   La classe construit une fenêtre dialog pour la saisie des actes et honoraires à sauvegarder. Attention AVANT d'exécuter cette classe vous devez définir ses paramètres ( setParams ).

*/


//-------------------------------------------- init -------------------------------------------------
/*! \fn void FormPlugins::init()
 *  \brief Renseigne les champs du formulaire avec les données accessibles. Le reste des données doit être renseigné par l'utilisateur.
 *  \sa CApp
 */
//-------------------------------------------- init -------------------------------------------------
void FormPlugins::init()
{  m_pHonoraires = new Honoraires ();
   m_pHonoraires_Sauvegarde = 0;
   if (!m_pHonoraires) return ;
   m_pActeListe = new ACTES_LISTE;
   if (!m_pActeListe) return;
   forUpdateOnly = FALSE;
   returnToDrTux = TR("Aucun honoraire enregistré");
   //payeListe->clear();
   // Crée la table des honoraires en bas de la fenêtre
   m_QTable_Actes = new QTable ( 5, 3, grActes);
   m_QTable_Actes->setSelectionMode(QTable::SingleRow);
   m_QTable_Actes->setReadOnly(TRUE);
   m_QTable_Actes->setGeometry( QRect( 20, 70, 440, 142 ) );
   m_QTable_Actes->horizontalHeader()->setLabel( 0, tr( "Acte" ) );
   m_QTable_Actes->setColumnWidth ( 0, 100 );
   m_QTable_Actes->horizontalHeader()->setLabel( 1, tr( "Tiers" ) );
   m_QTable_Actes->setColumnWidth ( 1, 50 );
   m_QTable_Actes->horizontalHeader()->setLabel( 2, tr( "Mode de Paiement" ) );
   m_QTable_Actes->setColumnWidth ( 2, 300 );
   // Connecte les signaux de la table avec les slots adéquats
   connect ( m_QTable_Actes,    SIGNAL( contextMenuRequested (int,int,const QPoint&) ),
	     this, SLOT  ( tablePop (int, int, const QPoint&) ) );
   // initialise le combo des années
   int an = QDate::currentDate().year() - 5;
   for (int i = an; i < (an+11); ++i)
    { annee->insertItem(QString::number(i));}
}


void FormPlugins::destroy()
{ if (m_pHonoraires) delete m_pHonoraires;
  if (m_pActeListe) delete m_pActeListe;
}

//---------------------------------------- tablePop -----------------------------------------
/*! \fn void FormPlugins::tablePop(int r, int c, const QPoint &p)
 *  \brief Affiche le menu contextuel du tableau des paiements.
 *  \sa t
 */
void FormPlugins::tablePop(int , int , const QPoint &p)
{ // Affiche un popupmenu avec les fonctions modif et suppression
  if (payeListe.count() == 0)
   { QPopupMenu *pop = new QPopupMenu(this, "popTable");
     pop->insertItem (TR("Ajouter un acte") , this, SLOT( buttonHelp_clicked()  ) );
     pop->popup(p);
   }
  else
   { QPopupMenu *pop = new QPopupMenu(this, "popTable");
     pop->insertItem (TR("Modifier") , this, SLOT(honoModif()  ) );
     pop->insertItem (TR("Supprimer"), this, SLOT(honoDelete() ) );
     pop->popup(p);
   }
}


/*! \fn void FormPlugins::honoDelete()
 *  \brief SLOT appellé lorsque l'utilisateur sélectionne Suppression de paiement dans le menu contextuel du tableau des paiements.
 * Demande la confirmation à l'utilisateur, efface le paiement de la liste des paiements et met à jour le tableau des paiements.
 */
void FormPlugins::honoDelete()
{if (payeListe.count() != 0)
 {QString str="";
  // Récupère le paiement sélectionné
  PAIEMENTS_LISTE::iterator it = payeListe.at( m_QTable_Actes->currentRow() );
  str =  tr("Souhaitez-vous supprimer cet acte de l'honoraire en cours ?<br /><br />");
  str += tr("<b>Acte sélectionné :</b> ") + (*it).getNom() + "<br />";
  str += tr("<b>Montant Total :</b> ") + QString::number( (*it).total() ) + "<br />";
  str += tr("<b>Mode de Paiement :</b> ") + (*it).toString() + "<br /><br />";
  if ( QMessageBox::question(this,tr("Suppression d'une paiement."),str,
            tr("&Oui"), tr("&Non"), QString::null, 0, 1 ) == 0)
        {  payeListe.remove( it ); affichePaiements();  }
 }
}

/*! \fn void FormPlugins::honoModif()
 *  \brief SLOT appellé lorsque l'utilisateur sélectionne la modification de paiement dans le menu contextuel du tableau des paiements.
 * Ouvre la fenêtre de saisie de paiement, la renseigne avec le paiement à modifier et connecte la fenêtre avec la fonction modifPaiements. L'ID du paiement à modifier est sauvegardé dans la variable \em idPaiementToModif .
 */
void FormPlugins::honoModif()
{ Acte_Selection *as = new Acte_Selection(this);
  as->setActesListe (m_pActeListe );
  // Injecter les valeurs de l'honoraire à modifier dans Acte_Selection
  idPaiementToModif = m_QTable_Actes->currentRow();
  PAIEMENTS_LISTE::iterator it = payeListe.at( idPaiementToModif );
  // Prépare les combos pour le type de ce paiement
  as->setPaiement( (*it) );
  connect (as,    SIGNAL ( actePaiementSelectionne( Paiements* ) ),
	   this,  SLOT   ( modifPaiement ( Paiements* ))   );
  as->exec();
  delete as;
}


/*! \fn void FormPlugins::modifPaiement(Paiements* p)
 *  \brief Fonction liée au SLOT honoModif() . Remplace un paiement de la liste à l'ID idPaiementToModif par le paiement passé en paramètre.
 *  \param p = Paiements() à insérer dans la liste des paiements de l'honoraire.
 */
void FormPlugins::modifPaiement(Paiements* p)
{ PAIEMENTS_LISTE::iterator it = payeListe.at( idPaiementToModif );
  it = payeListe.remove( it );
  payeListe.insert(it, *p);
  affichePaiements();
}


Paiements FormPlugins::getActeFromName(const char* nom)
{ Paiements p = Paiements();
  double total;

  // Récupère le nom de l'acte et ses paramètres
  ACTES_LISTE::iterator it;
  int i = -1;
  for(it = m_pActeListe->begin(); it !=  m_pActeListe->end(); ++it )
    {// Recherche en même temps l'item passé en paramètre
      if ((*it).m_Nom == nom) {  m_pActes_Param = &(*it); break;}
      ++i;
    }
 // récupère le nom de l'acte
 p.setNom((*it).m_Nom);
 p.setType((*it).getType());
 // récupère le tiers
 p.setTiers(tiersPayant);
 if (tiersPayant)
  {  total = m_pActes_Param->m_Tiers;
     p.setDAF( m_pActes_Param->m_Total - m_pActes_Param->m_Tiers );
	 p.setCMU( m_pActes_Param->m_Total - m_pActes_Param->m_Tiers );
  }
 else { total = m_pActes_Param->m_Total;  }

 // Mode de paiement passé en paramètre ?
 QString pay = "  ";
 if (modePaiement != "")
 {if 		(modePaiement == "esp")  	p.setEspeces(total);
  else if	(modePaiement == "espèces")	p.setEspeces(total);
  else if	(modePaiement == "chq")		p.setCheque(total);
  else if	(modePaiement == "cheque")	p.setCheque(total);
  else if	(modePaiement == "cb")		p.setCB(total);
  // ATTENTION IL FAUT GERER LES DAF CORRECTEMENT
  else if	(modePaiement == "daf")		p.setDAF(m_pActes_Param->m_Total);
  else if	(modePaiement == "diff")	p.setDAF(m_pActes_Param->m_Total);
  else if	(modePaiement == "différé")	p.setDAF(m_pActes_Param->m_Total);
  else if	(modePaiement == "autre")	p.setAutre(total);
  else if	(modePaiement == "cmu")		p.setCMU(m_pActes_Param->m_Total);
  else if	(modePaiement == "virement") p.setVirement(total);
 }
 // Récupère le tiers et calcul montants
/* if (tiersPayant)
  {  m_QTable_Actes->setText(0,1, "Oui");
     pay += " " + QString::number(m_pActes_Param->m_Total - m_pActes_Param->m_Tiers );
     pay += " ; " + TR("DAF :") + " " +  QString::number(m_pActes_Param->m_Tiers );
  }
 else
  {  m_QTable_Actes->setText(0,1, "Non"); 
     pay += QString::number(m_pActes_Param->m_Total);
  }
*/
 return p;
}


/*! \fn void FormPlugins::getParams()
 *  \brief Récupère les paramètres passés au plugins. Les paramètres sont formaés au sein d'une classe Honoraires.
 */
void FormPlugins::getParams()
{ if ((paramSet_OK) && (nomActe != "") && ( modePaiement != "") )
 { QString tmp;
   QStringList nActes = QStringList::split("|", nomActe);
   // Récupère tous les actes passés en paramètre
   for ( QStringList::Iterator it = nActes.begin(); it != nActes.end(); ++it )
   { tmp = (*it).stripWhiteSpace();
     if (tmp != "")
      payeListe.append ( getActeFromName( tmp ) );
   }
 }
 else 
 { // Efface lblEmetteur et lineeditemetteur
   lblEmetteur->hide();
   emetteur->hide();
 }
}

/*! \fn void FormPlugins::setParams()
 *  \brief Avant d'afficher la classe, il faut lui passer les paramètres (patient, guid, praticien...).
 */
void FormPlugins::setParams(	const char*	patient,
				const char*	GUID,
				Utilisateurs*	curUsr, 
				const char*	date,			// format "dd-MM-yyyy"
				const char*	nom_acte,
				bool		tiers,
				const char*	mode_paiement,
				ComptaMoteurBase* pMoteurBase)
{ m_pComptaMoteurBase = pMoteurBase;
  m_pUtilisateurActuel = curUsr;
  patientGUID = GUID;
  drtux_usrPk = m_pUtilisateurActuel->getDrTuxUsrPk();
  usrComptaId = m_pUtilisateurActuel->getId();

  nomPatient->setText(patient);
  nomPraticien->setText(m_pUtilisateurActuel->getNom());
  setDateObserv(date);

  nomActe      = nom_acte;
  tiersPayant  = tiers;
  modePaiement = mode_paiement;

  // Récupère les actes disponibles
  if (!m_pComptaMoteurBase->getActesForCombobox( *m_pActeListe ) )
   { QWidget *l = new QWidget();
     QMessageBox::information(l, TR("Erreur"), m_pComptaMoteurBase->m_messageErreur ,"Ok","","",1,0);
     delete l;
   }
  paramSet_OK = TRUE;


  // Vérifie si existe un montant du
  int du = 0;
  if (patientGUID != "") m_pComptaMoteurBase->checkDus(m_pUtilisateurActuel, patientGUID, du);
  if (du != 0)
  { lblDu->show();
    montantDu->show();
    montantDu->setText(QString::number(du));
  }
  else
  { lblDu->hide();
    montantDu->hide();
  }

   // Prépare les données passées en paramètres
   getParams();
   affichePaiements();
}

/*! \fn void FormPlugins::setHonoraire(Honoraires* hono)
 *  \brief Définit l'honoraire à afficher dans la fenêtre.
 */
bool FormPlugins::setHonoraire(Honoraires* hono)
{ m_pHonoraires_Sauvegarde = hono;
  if (hono->getRemarque().find(CHEQUE_DEPOSE) != -1) return FALSE;
  if (hono->getRemarque().find(DAF_OK) != -1) return FALSE;
  if (hono->getRemarque().find(CMU_OK) != -1) return FALSE;
  nomPatient->setText(hono->getPatient());
  nomPraticien->setText(hono->getPraticien());
//  dateObserv->setText( hono->getDate().toString("dd-MM-yyyy"));
  hono->getActes(payeListe);
  m_pHonoraires->setRemarque( hono->getRemarque() );
  affichePaiements();
  // Il faut récupérer le champs remarque de l'honoraire...
  if (m_pHonoraires->getEmetteur() != "") emetteur->setText( m_pHonoraires->getEmetteur() );
  m_pHonoraires->setId ( hono->getId() );
  patientGUID = m_pHonoraires->getGUID();
  forUpdateOnly = TRUE;
  return TRUE;
}


/*! \fn void FormPlugins::affichePaiements()
 *  \brief Met à jour le tableau des paiements avec la liste des paiements : payeListe .
 * La variable de classe Honoraires totalHonoraire contient le total des paiements.
 */
void FormPlugins::affichePaiements()
{ int c,r;
  c = m_QTable_Actes->numCols();
  r = m_QTable_Actes->numRows();
  for (int i = 0; i < c; ++i)
    for (int j = 0; j < r; ++j)
	{ m_QTable_Actes->setText( j, i, QString::null); }

  if (payeListe.count() != 0)
  { PAIEMENTS_LISTE::iterator  it;
    QString str = "";
    totalHonoraire.remiseAZero();
    int i=0;
    m_QTable_Actes->setNumRows( payeListe.count() + 1);
    for(it = payeListe.begin(); it !=  payeListe.end(); ++it )
    {	// Calcul le montant total de l'honoraire
	totalHonoraire.m_Esp   += (*it).m_Esp;
	totalHonoraire.m_Chq   += (*it).m_Chq;
	totalHonoraire.m_CB    += (*it).m_CB;
	totalHonoraire.m_DAF   += (*it).m_DAF;
	totalHonoraire.m_Autre += (*it).m_Autre;
	totalHonoraire.m_CMU    += (*it).m_CMU;
    totalHonoraire.m_Virement    += (*it).m_Virement;
	if (!forUpdateOnly) totalHonoraire.setEmetteur( nomPatient->text() );
	else totalHonoraire.setEmetteur(m_pHonoraires->getEmetteur() );

	// Prépare la string à afficher dans le tableau
       m_QTable_Actes->setText(i, 0, (*it).m_Nom );
       if ( (*it).m_Tiers ) m_QTable_Actes->setText(i, 1, "Oui" ); else m_QTable_Actes->setText(i, 1, "Non" );
       m_QTable_Actes->setText(i, 2, (*it).toString() );
       ++i;
     }
   // Affiche le total de l'honoraire
   m_QTable_Actes->setText(i,0, "TOTAL :" + QString::number( totalHonoraire.total() ));
   m_QTable_Actes->setText(i,2, totalHonoraire.toString() );
   // Gestion de l'emetteur du chèque
   if (totalHonoraire.m_Chq > 0)
    { lblEmetteur->show();    emetteur->show();
      if (!forUpdateOnly) emetteur->setText( totalHonoraire.getEmetteur() );
    }
   else { lblEmetteur->hide(); emetteur->hide(); }
 }
 else // Aucun paiement enregistré dans l'honoraire
 { m_QTable_Actes->setText(0,0, tr("Aucun Acte") ); }
}


/*! \fn void FormPlugins::getHonoraire()
 *  \brief Contruit l'honoraire à sauvegarder dans la base de données. L'honoraire est sauvegardé dans la variable m_pHonoraires de cette classe.
 */
void FormPlugins::getHonoraire()
{ QString tmp = "";
  m_pHonoraires->setPatient	( nomPatient->text()   );
  m_pHonoraires->setPraticien	( nomPraticien->text() ); 
  m_pHonoraires->setActes	( payeListe            );
  m_pHonoraires->setDate	( getDateObserv()  );
  m_pHonoraires->setEspeces	(totalHonoraire.getEsp() );
  m_pHonoraires->setCheque	(totalHonoraire.getChq() );
  m_pHonoraires->setCB		(totalHonoraire.getCB() );
  m_pHonoraires->setDAF		(totalHonoraire.getDAF() );
  m_pHonoraires->setAutre	(totalHonoraire.getAutre() );
  m_pHonoraires->setCMU		(totalHonoraire.getCMU() );
  m_pHonoraires->setVirement		(totalHonoraire.getVirement() );
  m_pHonoraires->setIdUsr       (usrComptaId);
  m_pHonoraires->setGUID	(patientGUID);
  m_pHonoraires->setDrTuxUsr	(drtux_usrPk);
  // Si il existe un DAF mettre dans le champs remarque DAF_PAS_OK  (cf ComptaMoteurBase.h)
  // Si paiement par chèque mettre dans le champs remarque CHEQUE_PAS_DEPOSE  (cf ComptaMoteurBase.h)
  if (totalHonoraire.getChq() > 0) { tmp  = CHEQUE_PAS_DEPOSE;}
  if (totalHonoraire.getDAF() > 0) { tmp += DAF_PAS_OK;       }
  if (totalHonoraire.getCMU() > 0) { tmp += CMU_PAS_OK;       }
  m_pHonoraires->setRemarque(tmp);
  if (emetteur->isVisible() )   { m_pHonoraires->setEmetteur( emetteur->text() ); }
}


/*! \fn void FormPlugins::ajoutePaiement ( Paiements* p)
 *  \brief Ajoute un paiement à la liste des paiements.
 *  \param p : Paiements à ajouter.
 */
void FormPlugins::ajoutePaiement ( Paiements* p)
{ payeListe.append( *p );
  affichePaiements();
}


void FormPlugins::buttonHelp_clicked()
{  Acte_Selection *a = new Acte_Selection(this);
   a->setActesListe( m_pActeListe );
   connect (a,    SIGNAL ( actePaiementSelectionne( Paiements* ) ),
	    this, SLOT   ( ajoutePaiement ( Paiements* ))   );
   a->exec();
   delete a;
}


/*! \fn void FormPlugins::buttonOK_clicked()
 *  \brief Sauvegarde de l'honoraire si tout va bien.
 */
void FormPlugins::buttonOK_clicked()
{ // Vérifie les champs obligatoires : nom, praticien, date
  if (nomPatient->text().stripWhiteSpace() == "") 
   QMessageBox::information(this, TR("Formulaire incomplet."), TR("Vous devez obligatoirement renseigné le nom du patient (ou du laboratoire)"),TR("Ok"),"","",1,0);
  else if (nomPraticien->text().stripWhiteSpace() == "")
   QMessageBox::information(this, TR("Formulaire incomplet."), TR("Vous devez obligatoirement renseigné le nom du praticien."),TR("Ok"),"","",1,0);
//  else if (dateObserv->text().stripWhiteSpace() == "")
//   QMessageBox::information(this, TR("Formulaire incomplet."), TR("Vous devez obligatoirement renseigné la date de l'honoraire."),TR("Ok"),"","",1,0);
  else
  if (payeListe.count() != 0)
   { getHonoraire();
     bool sansErreur = FALSE;
     if (!forUpdateOnly) 
     { sansErreur = m_pComptaMoteurBase->saveHonorairesToBase(m_pHonoraires);
     }
     else  // il s'agit d'une modification --> tracer les modifs avant de sauvegarder
     { m_pHonoraires->traceModifications( m_pHonoraires_Sauvegarde, m_pUtilisateurActuel );
       sansErreur = m_pComptaMoteurBase->updateHonoraires(m_pHonoraires);
     }
     if (!sansErreur)
       // une erreur est survenue --> afficher l'erreur.
      {	QMessageBox *b = new QMessageBox(this);
	b->setText( m_pComptaMoteurBase->m_messageErreur );
	b->exec();
	delete b;
      }
     else 
      {	returnToDrTux = TR("Honoraire enregistré : ") + m_pHonoraires->getActesToString() + " " + m_pHonoraires->toString();
	accept();
      }
   }
  else { QMessageBox::warning(this, tr("Erreur"), tr("Attention, vous n'avez sélectionné aucun acte pour cet honoraire.<br /> <br />Pour sauvegarder l'honoraire dans la base de données, veuillez ajouter au minimum un acte."),"&Ok", QString::null, QString::null, 1, 0 );  }
}


/*! \fn QString FormPlugins::getMessage()
 *  \brief Récupère le message final de la classe. "Honoraire enregistré" ou "Erreur".
 */
QString FormPlugins::getMessage()
{ return returnToDrTux; }


/*! \fn void FormPlugins::mois_activated( int )
 *  \brief Lorsque l'utilisateur change de mois, recalcul le nombre de jours et met à jour le combo des jours du mois.
 */
void FormPlugins::mois_activated(int )
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


/*! \fn QString FormPlugins::getDateObserv()
 *  \brief A tout moment, retourne la date de l'honoraire au format string : "dd-MM-yyyy"
 */
QString FormPlugins::getDateObserv()
{  QString tmp = "";
    tmp = QString::number(mois->currentItem() + 1);
    if (mois->currentItem() < 9) tmp.prepend("0");
    return jour->currentText() + "-" + tmp + "-" + annee->currentText();
}


/*! \fn void FormPlugins::setDateObserv(QString dt_str)
 *  \brief A tout moment, met les combobox de la date aux bonnes valeurs. La date est passée en paramètre au format "dd-MM-yyyy".
 */
void FormPlugins::setDateObserv(QString dt_str)
{ // Passe du format "dd-MM-yyyy" au format "yyyy-MM-dd"
  QString tmp = m_pComptaMoteurBase->dateForSQL( dt_str );
  QDate dt = QDate::fromString(tmp, Qt::ISODate);
  setDateObserv(dt);
}

/*! \fn void FormPlugins::setDateObserv(QDate dt)
 *  \brief A tout moment, met les combobox de la date aux bonnes valeurs. La date est passée en paramètre au format "dd-MM-yyyy".
 */
void FormPlugins::setDateObserv(QDate dt)
{ if (! dt.isValid()) dt = QDate::currentDate();

  int an = dt.year() - 5;
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


/*! \fn void FormPlugins::annee_activated( int  )
 *  \brief Si l'utilisateur change d'année et que le mois est sur février -> gérer les années bissextiles et mettre à jour le combo des jours.
 */
void FormPlugins::annee_activated( int  )
{ if (mois->currentItem() == 1)     mois_activated( 1 );
}

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


void Hono_View::init()
{m_Popup = 0;
 m_Hono_Liste = new HONO_LISTE;
 if (!m_Hono_Liste) return ;
 m_Hono_Liste->clear();
 m_pActesListe = new ACTES_LISTE;
 if (!m_pActesListe) return;
 m_pActesListe->clear();

 // Prépare le tableau pour l'affichage
   m_QTable_Hono->setColumnWidth ( 0, 150 );
   m_QTable_Hono->setColumnWidth ( 1, 80 );
   m_QTable_Hono->setColumnWidth ( 2, 80 );
   m_QTable_Hono->setColumnWidth ( 3, 45 );
   m_QTable_Hono->setColumnWidth ( 4, 45 );
   m_QTable_Hono->setColumnWidth ( 5, 45 );
   m_QTable_Hono->setColumnWidth ( 6, 45 );
   m_QTable_Hono->setColumnWidth ( 7, 45 );
   m_QTable_Hono->setColumnWidth ( 8, 45 );
   m_QTable_Hono->setColumnWidth ( 9, 45 );
   m_QTable_Hono->setColumnWidth ( 10, 20 );

}

void Hono_View::destroy()
{ if (m_Verbose) qWarning("CMDI_Honoraires::Destruct");
  if (m_Hono_Liste)	delete m_Hono_Liste;
  if (m_Popup)		delete m_Popup;
  if (m_pActesListe)	delete m_pActesListe;
  //m_pComptaMoteurBase m_pUtilisateurActuel sont deleted par CApp
  if (m_Verbose) qWarning("CMDI_Honoraires::Destruct Fin");
}

void Hono_View::setParams( ComptaMoteurBase* 	pComptaMoteurBase,
			   Utilisateurs*	pUtilisateurActuel,
			   bool			verbose)
{ m_pComptaMoteurBase = pComptaMoteurBase;
  m_pUtilisateurActuel = pUtilisateurActuel;
  m_Verbose = verbose;
  // récupère m_ActesListe
  if (!m_pComptaMoteurBase->getActesForCombobox( *m_pActesListe ) )
   { if (m_Verbose) qWarning("Hono_View::setParams : Impossible de récupérer la liste des actes disponibles.");
     // Ferme l'appli sinon seg fault à cause de la fonction sythèse de CMDI Honoraires
     return ;
   }
   else { if (m_Verbose) qWarning("Hono_View::setParams : Liste des actes disponibles récupérée."); }

  changeTableFilter();
}


/*!\brief SLOT appelé lors d'un clique sur le tableau des honoraires.
 * Vérifie qu'un honoraire est sélectionné dans le tableau. Puis affiche une boîte de dialogue de confirmation enfin efface l'honoraire et met à jour le tableau.
* La modification et/ou la suppression d'un chèque encaissé, d'un daf encaissé ou d'un honoraire validé est interdite.
 */
void Hono_View::m_QTable_Hono_contextMenuRequested( int, int, const QPoint & p)
{bool valide = FALSE;
 bool canBeValidate = FALSE;

 HONO_LISTE::iterator it = m_Hono_Liste->at( m_QTable_Hono->currentRow() );
 if (((*it).getRemarque().find(DAF_OK) != -1) ||
     ((*it).getRemarque().find(CHEQUE_DEPOSE) != -1) ||
	 ((*it).getRemarque().find(CMU_OK) != -1) ||
     ((*it).getValidite() != 0))
 	{ valide = TRUE; }

 if (((*it).getRemarque().find(DAF_PAS_OK) != -1) ||
     ((*it).getRemarque().find(CHEQUE_PAS_DEPOSE) != -1) ||
	 ((*it).getRemarque().find(CMU_PAS_OK) != -1) ||
     ((*it).getValidite() != 1))
 	{ canBeValidate = TRUE; }

 if (m_Popup) delete m_Popup;
 m_Popup = new QPopupMenu(this, "popTable");
 if (m_Popup) 
 { m_Popup->insertItem (TR("Voir"),      this, SLOT(honoView() ) );
   if (!valide)
   { m_Popup->insertItem (TR("Modifier") , this, SLOT(honoModif()  ) );
     m_Popup->insertItem (TR("Supprimer"), this, SLOT(honoDelete() ) );
     if (canBeValidate) m_Popup->insertItem (TR("Valider"), this,   SLOT(honoValide() ) ); 
   }
   m_Popup->popup(p);
 }
}


void Hono_View::but_Ajouter_clicked()
{FormPlugins *mForm = new FormPlugins();
 if (mForm)
 {QString tmp, dt;
  tmp = "";
  dt = QDate::currentDate().toString("dd-MM-yyyy");
  mForm->setParams(tmp,tmp, m_pUtilisateurActuel, dt, tmp, FALSE, tmp, m_pComptaMoteurBase);
  mForm->exec();
  // Met à jour le tableau des honoraires
  if (mForm->result() == QDialog::Accepted)
  { changeTableFilter();
    //emit( honoChange() );
  }
  delete mForm;
 }
}



void Hono_View::but_ImprimeSynthese_clicked()
{ QPainter* m_painter = new QPainter();
  QPrinter* m_printer = new QPrinter();
  if ((m_printer) && (m_painter))
  {if (m_printer->setup())
   {m_printer->setDocName("Synthese_honoraires_"+QDateTime::currentDateTime().toString("dd-MM-yyyy_hh:mm:ss:ms"));
    QFont m_font( "Arial", 10);
    QColorGroup sp_colorgroup (colorGroup());
    sp_colorgroup.setColor (QColorGroup::Text, QColor(0,0,0) );
    sp_colorgroup.setColor (QColorGroup::BrightText, QColor(0,0,0) );
    sp_colorgroup.setColor (QColorGroup::ButtonText, QColor(0,0,0) );
    sp_colorgroup.setColor (QColorGroup::HighlightedText, QColor(0,0,0) );
    QSimpleRichText* toPrint = new QSimpleRichText( m_QText_Synthese->text(), m_font );

    m_painter->begin(m_printer);
    toPrint->draw(m_painter, 50, 50, QRect(50,50,600,1000) , sp_colorgroup );
    m_painter->end();
    delete m_painter;
    delete m_printer;
   }
  }
}

//************************* setTableDesHonorairesTo *************************
/*! \brief Rafraîchit la table des honoraires avec la liste d'honoraires passée en pointeur. 
 *  \todo mettre une ligne sur deux en gris clair, et en rouge les montants dus...
*/
void Hono_View::setTableDesHonorairesTo( HONO_LISTE* hl )
{ if ((hl->count() == 0) || (!hl))
   { m_QTable_Hono->setNumRows( 1 );
     m_QTable_Hono->setText(0,0, TR("Aucun Honoraire enregistré pour cette période") );
     for (int i = 1; i < 9; ++i)
 	m_QTable_Hono->setText(0,i, " " );
   }
  else
   { int r = 0;
     double cb,autre,cmu,vir,daf;
     cb=0;  autre=0;  cmu=0; vir=0;  daf=0;
     HONO_LISTE::iterator it;
     m_QTable_Hono->setNumRows( hl->count() );
     for (it = hl->begin(); it !=  hl->end(); ++it )
      { // Récupère le nom de tous les actes
	m_QTable_Hono->setText( r,1, (*it).getActesToString() );
	// Récupère les données de l'honoraire
	m_QTable_Hono->setText( r,0, (*it).getPatient() );
	m_QTable_Hono->setText( r,2, (*it).getDate().toString("dd-MM-yyyy") );
	m_QTable_Hono->setText( r,3, QString::number( (*it).getEsp() ) );
	m_QTable_Hono->setText( r,4, QString::number( (*it).getChq() ) );
	m_QTable_Hono->setText( r,5, QString::number( (*it).getCB() ) );
	m_QTable_Hono->setText( r,6, QString::number( (*it).getAutre() ) );
	m_QTable_Hono->setText( r,7, QString::number( (*it).getDAF() ) );
	m_QTable_Hono->setText( r,8, QString::number( (*it).getCMU() ) );
	m_QTable_Hono->setText( r,9, QString::number( (*it).getVirement() ) );
	cb += (*it).getCB();
	autre += (*it).getAutre();
	cmu += (*it).getCMU();
	vir += (*it).getVirement();
	daf += (*it).getDAF();
	r++;
      }
    if (cb == 0) m_QTable_Hono->hideColumn(5);
    else m_QTable_Hono->showColumn(5);
    if (daf == 0) m_QTable_Hono->hideColumn(7);
    else m_QTable_Hono->showColumn(7);
    if (autre == 0) m_QTable_Hono->hideColumn(6);
    else m_QTable_Hono->showColumn(6);
    if (cmu == 0) m_QTable_Hono->hideColumn(8);
    else m_QTable_Hono->showColumn(8);
     if (vir == 0) m_QTable_Hono->hideColumn(9);
    else m_QTable_Hono->showColumn(9);
    //m_QTable_Hono->adjustSize();
   }
}


//************************* afficheSynthese *************************
/*! \brief Insère dans le QTextEdit de la fenêtre CMDI_Honoraires la synthèse pour la période filtrée.
 * \return FALSE en cas d'erreur, TRUE si tout c'est bien déroulé.
*/
bool Hono_View::afficheSynthese( )
{if (!m_Hono_Liste) return FALSE;
 QString msg = "";
 if ((m_Hono_Liste->count() == 0) || (!m_Hono_Liste))
   msg = "<p align=\"center\">"+TR("Aucun honoraire pour la période.")+"</p>";
 else
 {int nbActes = 0;
  int nbActesTotal = 0;
  int nbDAF = 0;
  int nbDAFOK = 0;
  int nbCMU = 0;
  int nbCMUOK = 0;
  int nbCheques = 0;
  int nbChequesDeposes = 0;
  int i, j, nbA = 0;
  Tools* tools = new Tools();
  Paiements *paie = new Paiements();

  QString debutLigne = "<td align=\"center\"><b>";
  QString finLigne   = "</td></b>";
  QString lineBreak = "<br /><br />"; 
  QStringList hd, lines;
  hd.clear();
  lines.clear();

  // Entête du rapport
  m_QText_Synthese->clear();
  msg =  "<p align=\"center\"><b>"+TR("Synthèse des honoraires")+"</b></p>";
  msg +=  "<p align=\"center\"><b>"+m_pComptaMoteurBase->m_pUtilisateurActuel->getNom()+"</b></p>";

  msg += "<p>";

  hd << TR("Filtre en cours") << TR("Date de début") << TR("Date de fin");
  lines << "&nbsp;" << m_pComptaMoteurBase->m_Filtre_Date_Debut.toString("dddd dd MMMM yyyy") << m_pComptaMoteurBase->m_Filtre_Date_Fin.toString("dddd dd MMMM yyyy");
  tools->createHTMLTable(hd, lines, msg, 4, 0);
  msg += lineBreak;

  // Récupère les différents types d'actes
  QString tmp = "";
  QStringList typesActes;
  typesActes.clear();
  ACTES_LISTE::iterator it;
  for (it = m_pActesListe->begin(); it !=  m_pActesListe->end(); ++it )
   { tmp = (*it).getType();
     if (typesActes.isEmpty() )
	{ typesActes.append( tmp ); }
     else
      {	// Vérifier que le type n'est déjà dans la liste
	if ( typesActes.contains( tmp ) == 0 )
	 { typesActes.append ( tmp ); }
       }
   }

  // Crée une liste de int à la longueur de la QStringList ci-dessus et initialise la
  QMemArray<int>    nbActesParType( typesActes.count() );
  QMemArray<double> montantActesParType( typesActes.count() );
  double montantTotalActes = 0.0;

  for (i = 0; i < (int) typesActes.count() ; ++i)
   { nbActesParType[i]=0; montantActesParType[i]=0; }

  HONO_LISTE::iterator ithono;
  for (ithono = m_Hono_Liste->begin(); ithono != m_Hono_Liste->end() ; ++ithono)
   {nbA = 0; i = 0; j = 0;
    paie->m_Esp += (*ithono).getEsp();
    paie->m_Chq += (*ithono).getChq();
    if ((*ithono).getChq() > 0) nbCheques++;
    paie->m_DAF  += (*ithono).getDAF();
    if ((*ithono).getDAF() > 0) nbDAF++;
    paie->m_CB   += (*ithono).getCB();
    paie->m_CMU   += (*ithono).getCMU();
	if ((*ithono).getCMU() > 0) nbCMU++;
    paie->m_Virement   += (*ithono).getVirement();
    paie->m_Autre   += (*ithono).getAutre();
    nbActesTotal = (*ithono).m_Actes.contains("|");
    nbActes += nbActesTotal;

    // Calcul le nomnre de chèques déposés et de DAF validés
    if ((*ithono).m_Remarque.find("|") != -1) // alors il y a des infos à prendre en compte
    { if ( (*ithono).m_Remarque.find(CHEQUE_DEPOSE) != -1) nbChequesDeposes++;
      if ( (*ithono).m_Remarque.find(DAF_OK) != -1) nbDAFOK++;
	  if ( (*ithono).m_Remarque.find(CMU_OK) != -1) nbCMUOK++;
    }

    // Calcul le nombre et le montant des actes de chaque honoraires
    PAIEMENTS_LISTE montantActe;
    montantActe.clear();
    (*ithono).getActes(montantActe);
    for (PAIEMENTS_LISTE::iterator it_paie = montantActe.begin() ; it_paie != montantActe.end(); ++it_paie)
    // Calcul le nombre d'actes selon le type d'actes
      { j=0;
	while ((j < (int) nbActesParType.count()) || (typesActes[j] == (*it_paie).getNom() ))
         { if ( typesActes[j] == (*it_paie).getType() ) // Oui trouvé
	     {	nbActesParType[j]++; 
		montantActesParType[j] += (*it_paie).total();
		break;
	     }
	   j++;
         } // Fin while
      }  // Fin for nbA
   } // Fin for m_Hono_Liste

  msg += "<table border=1 cellpadding=2 cellspacing=2 width=100%>";
  QString tabRecap = "<tr>" + debutLigne + TR("Mode de paiement") + finLigne;
  if (paie->getEsp() > 0)	tabRecap += debutLigne + TR("Espèces") +	finLigne;
  if (paie->getChq() > 0)	tabRecap += debutLigne + TR("Chèques") +	finLigne;
  if (paie->getCB() > 0) 	tabRecap += debutLigne + TR("CB") +		finLigne;
  if (paie->getDAF() > 0)	tabRecap += debutLigne + TR("DAF") +		finLigne;
  if (paie->getAutre() > 0)	tabRecap += debutLigne + TR("Autre") +		finLigne;
  if (paie->getCMU() > 0)	tabRecap += debutLigne + TR("CMU") +		finLigne;
  if (paie->getVirement() > 0)	tabRecap += debutLigne + TR("Virement") +		finLigne;
  				tabRecap += debutLigne + TR("TOTAL") +		finLigne;
  tabRecap += "</tr>";

  msg += tabRecap;
  tabRecap.replace( "<b>", "");
  tabRecap.replace( "</b>", "");
  tabRecap.replace( TR("Mode de paiement"), "<b>"+TR("Totaux")+"</b>" );
  tabRecap.replace( TR("Espèces"), QString::number(paie->getEsp()) );
  tabRecap.replace( TR("Chèques"), QString::number(paie->getChq()) );
  tabRecap.replace( TR("CB"), QString::number(paie->getCB()) );
  tabRecap.replace( TR("DAF"), QString::number(paie->getDAF()) );
  tabRecap.replace( TR("Autre"), QString::number(paie->getAutre()) );
  tabRecap.replace( TR("CMU"), QString::number(paie->getCMU()) );
  tabRecap.replace( TR("Virement"), QString::number(paie->getVirement()) );
  tabRecap.replace( TR("Actes"), QString::number(nbActes) );
  tabRecap.replace( TR("TOTAL"), QString::number(paie->total()) );
  msg += tabRecap;
  msg += "</table>";
  msg += lineBreak;
  msg += "<table border=1 cellpadding=2 cellspacing=2 width=100%>";

  tabRecap = "<tr>" + debutLigne + TR("Mode de paiement") + finLigne;
  if (nbCheques > 0)	tabRecap += debutLigne + TR("Nb Chèques") +	finLigne;
  if (nbDAF > 0)	tabRecap += debutLigne + TR("Nb DAF") +		finLigne;
  if (nbCMU > 0)	tabRecap += debutLigne + TR("Nb CMU") +		finLigne;
  if (nbActes > 0 )	tabRecap += debutLigne + TR("Actes") +		finLigne;
  msg += tabRecap;

  tabRecap.replace( "<b>", "");
  tabRecap.replace( "</b>", "");
  tabRecap.replace( TR("Mode de paiement"), "<b>"+TR("Quantité")+"</b>" );
  tabRecap.replace( TR("Nb Chèques"), QString::number(nbCheques) );
  tabRecap.replace( TR("Nb DAF"), QString::number(nbDAF) );
  tabRecap.replace( TR("Nb CMU"), QString::number(nbCMU) );
  tabRecap.replace( TR("Actes"), QString::number(nbActes) );
  msg += tabRecap + "</table>";
  msg += lineBreak + "</p>";

  // Construit le tableau du nombre d'actes selon le type
  msg += "<p align =\"center\"><b>Descriptif des actes</b></p><table border=1 cellpadding=2 cellspacing=2 width=100%>";
  QString line1 = "";
  QString line2 = "";
  QString line3 = "";
  QString line4 = "";

  int nbC, nbV;
  nbC = 0;
  nbV = 0;

  for (i=0; i < (int) nbActesParType.count(); ++i)
  {  montantTotalActes += montantActesParType[i];    }

  for (i=0; i < (int) nbActesParType.count(); ++i)
   { if (nbActesParType[i] != 0)
      {	line1 += debutLigne + typesActes[i] + finLigne;
	line2 += debutLigne + QString::number(nbActesParType[i]) + finLigne;
	line3 += debutLigne + QString::number(montantActesParType[i]) + finLigne;
	line4 += debutLigne + QString::number((montantActesParType[i] / montantTotalActes)*100,'f',2) + " %" +finLigne;
	// Récupère le nombre de C et de V
	if (typesActes[i] == "Consultation") nbC += nbActesParType[i];
	else if (typesActes[i] == "Visite") nbV += nbActesParType[i];
      }
   }
  line2.replace("<b>","");
  line2.replace("</b>","");
  line3.replace("<b>","");
  line3.replace("</b>","");
  line4.replace("<b>","");
  line4.replace("</b>","");


  msg += "<tr><td align=\"center\"><b>Type</b></td>" + line1 + "</tr>";
  msg += "<tr><td align=\"center\"><b>Nombre</b></td>" + line2 + "</tr>";
  msg += "<tr><td align=\"center\"><b>Montant</b></td>" + line3 +"</tr>";
  msg += "<tr><td align=\"center\"><b>Pourcentage du montant total</b></td>" + line4 ;

  msg+= "</tr></table>";

  if ((typesActes.contains("Consultation")) && (typesActes.contains("Visite")))
  { double ratio = ( (double)nbV/ (double)nbC) * 100.0;
    msg += "<p>"+TR("Pourcentage de Visites par rapport aux consultations = ")+QString::number(ratio, 'f', 2)+" %</p>";
  }

 delete tools;
 }

 m_QText_Synthese->setText(msg);
 return TRUE;
}

//************************* changeTableFilter *************************
/*! \brief SLOT activé par la fenêtre de filtrage (Filtre_Hono). Modifie le filtre de la table des honoraires. 
*/
void Hono_View::changeTableFilter( )
{ m_Hono_Liste->clear();
  if (m_pComptaMoteurBase->getHonorairesForTable( m_Hono_Liste ) )
   {	setTableDesHonorairesTo( m_Hono_Liste );
	afficheSynthese();
   }
  else 
   {	m_QTable_Hono->setNumRows(0);	m_QText_Synthese->clear();
	QMessageBox::information( this, TR("Une erreur est survenue..."),
          m_pComptaMoteurBase->m_messageErreur, TR("&OK"), QString::null, QString::null, 1 , 2 );
   }

  if (m_Hono_Liste->count() == 0)  { m_QTable_Hono->setNumRows(0);   m_QText_Synthese->clear();  }
}


/*!\brief SLOT appelé par le popupmenu de la table des honoraires qui ouvre la fenêtre de visualisation des honoraires pour l'honoraire sélectionné. 
*/
void Hono_View::honoValide()
{ // Vérifie qu'une ligne est bien sélectionnée sinon msg erreur
  if (m_QTable_Hono->currentSelection () != -1)
  { // récupère l'enregistrement en cours
    HONO_LISTE::iterator  it = m_Hono_Liste->at( m_QTable_Hono->currentRow() );
    HONO_LISTE* Hono_valide = new HONO_LISTE;
    Hono_valide->clear();
    Hono_valide->append( (*it) );
    QString msg = "";
     msg  = TR("Attention, vous vous apprêtez à valider des honoraires.<br />Cette validation comptable est obligatoire sur le plan légal et interdit par la suite de :<br />- modifier les honoraires validés,<br />- supprimer les honoraires validés.<br /><br /><br /><br /><b>Voulez-vous valider les honoraires filtrés ?</b><br />");
    if (QMessageBox::information( this, TR("Validation comptable d'honoraires"),msg , TR("&OK"), QString::null, QString::null, 0 , 1 ) == 0)
    { int nbUp = 0;
      m_pComptaMoteurBase->valideHonoraires( Hono_valide, m_pUtilisateurActuel, nbUp );
      msg = QString( TR("%1 honoraire(s) validés dans la base de données sans erreur.")).arg(nbUp);
      QMessageBox::information( this, TR("Validation comptable d'honoraires"),msg , TR("&OK"), QString::null, QString::null, 0 , 1 );
      if (nbUp != 0) changeTableFilter();
    }
    delete Hono_valide;
  } // Fin if column selected
}


/*!\brief SLOT appelé par le popupmenu de la table des honoraires qui ouvre la fenêtre de visualisation des honoraires pour l'honoraire sélectionné. 
*/
void Hono_View::honoView()
{ // Vérifie qu'une ligne est bien sélectionnée sinon msg erreur
  if (m_QTable_Hono->currentSelection () != -1)
  { // récupère l'enregistrement en cours
    HONO_LISTE::iterator  it = m_Hono_Liste->at( m_QTable_Hono->currentRow() );
    Hono_Visualisation* hView = new Hono_Visualisation(this);
    hView->setHonoraire( &(*it) );
    hView->exec();
    delete hView;
   } // if hView
}


/*!\brief SLOT appelé par le popupmenu de la table des honoraires qui ouvre la fenêtre de modification des honoraires pour l'honoraire sélectionné. 
 * \todo Encodage UTF8 du nom du praticien...
 * Vérifie qu'un honoraire est sélectionné dans le tableau. Puis affiche une boîte de dialogue de confirmation enfin efface l'honoraire et met à jour le tableau.
*/
void Hono_View::honoModif()
{ // Vérifie qu'une ligne est bien sélectionnée sinon msg erreur
  if (m_QTable_Hono->currentSelection () != -1)
    {// récupère l'honoraire à modifier
     HONO_LISTE::iterator  it = m_Hono_Liste->at( m_QTable_Hono->currentRow() );
     Honoraires sauvegarde = (*it);

     FormPlugins *mForm = new FormPlugins();
     if (mForm)
      {	QString tmp, dt;
	dt = (*it).getDate().toString("dd-MM-yyyy");
	mForm->setParams(tmp,tmp, m_pUtilisateurActuel, dt, tmp, FALSE, tmp, m_pComptaMoteurBase);
	if (!mForm->setHonoraire( &(*it) )) 
	  QMessageBox::information(this, TR("ERREUR"), TR("Attention, vous ne pouvez pas modifier un honoraire contenant :\nun chèque déposé en banque ou \nun paiement différé précédemment validé."),TR("Ok"),"","",1,0);
	else 
	 { mForm->exec();
	   // Met à jour le tableau des honoraires
	   if (mForm->result() == QDialog::Accepted)
	   { // Trace les modifications
	     (*it).traceModifications( &sauvegarde , m_pUtilisateurActuel );
	     changeTableFilter();
	     //emit( honoChange() );
	   }
	 }
        delete mForm;
       } // Fin if mForm
     }
}

/*! \brief Supprime l'honoraire que l'utilisateur a sélectionné dans le tableau des honoraires. Fonction appellée par le menu "Supprimer"
 * Vérifie qu'un honoraire est sélectionné dans le tableau. Puis affiche une boîte de dialogue de confirmation enfin efface l'honoraire et met à jour le tableau.
*/ 
void Hono_View::honoDelete()
{ // Vérifie qu'une ligne est bien sélectionnée sinon msg erreur
  if (m_QTable_Hono->currentSelection () != -1 && m_Hono_Liste)
   {// Récupère l'honoraire sélectionné
    HONO_LISTE::iterator  it = m_Hono_Liste->at( m_QTable_Hono->currentRow() );
    // Ouvrir un msgbox pour la demande de confirmation
    QString msg = TR("Vous avez sélectionné l'honoraire suivant : <br /><br />");
    msg += TR("Patient : ") + (*it).getPatient() + " <br />";
    msg += TR("Date :&nbsp;&nbsp;&nbsp;&nbsp;") + (*it).getDate().toString("dd MMM yyyy") + " <br />";
    msg += TR("Acte :&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;") + (*it).getActesToString() + " <br />";
    msg += TR("Id :&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;") + QString::number( (*it).getId() );
    msg += ".<br /><br />";

    // Vérifie que l'honoraire ne contient ni chèque déposé ni DAF validé
    if ((*it).getRemarque().find(CHEQUE_DEPOSE) != -1)
	{ msg += TR("Attention, vous ne pouvez pas supprimer un honoraire contenant un chèque encaissé.<br />");
	  QMessageBox::information( this, TR("ERREUR : Suppression d'honoraire."),
        	msg, TR("&Annuler"), QString::null, QString::null,
		1 , 2 );
	}
    else if ((*it).getRemarque().find(DAF_OK) != -1)
	{ msg += TR("Attention, vous ne pouvez pas supprimer un honoraire contenant un paiement différé validé.<br />");
	  QMessageBox::information( this, TR("ERREUR : Suppression d'honoraire."),
        	msg, TR("&Annuler"), QString::null, QString::null,
		1 , 2 );
	}
	else if ((*it).getRemarque().find(CMU_OK) != -1)
	{ msg += TR("Attention, vous ne pouvez pas supprimer un honoraire contenant un paiement différé validé.<br />");
	  QMessageBox::information( this, TR("ERREUR : Suppression d'honoraire."),
        	msg, TR("&Annuler"), QString::null, QString::null,
		1 , 2 );
	}
    else
    {	msg += TR("<b>Confirmez-vous la suppression de cet honoraire ?</b>");

	if ( QMessageBox::information( this, TR("Suppression d'honoraire"),
        	msg, TR("&Supprimer"), TR("&Annuler"), QString::null, 
		1 , 2 )  == 0 )
     	{  if (!m_pComptaMoteurBase->deleteHonoraire( (*it).getId() ) )
           { // Message d'erreur
	  	QMessageBox::information( this, TR("Une erreur est survenue..."),
          	m_pComptaMoteurBase->m_messageErreur, TR("&OK"), QString::null, QString::null, 1 , 2 );
	   }
	   else
	   { //rafraîchit la table des honoraires et recalcul la synthèse
	     m_Hono_Liste->remove ( it );
	     setTableDesHonorairesTo( m_Hono_Liste );
	     afficheSynthese();
	     //emit( honoChange() );
   	   }
         }  // Fin if MsgBox de suppression
    } // Fin if remarque
   } // Fin if Selection et m_Hono_Liste
}


void Hono_View::but_Validation_clicked()
{ QString msg = "";
  msg  = TR("Attention, vous vous apprêtez à valider des honoraires.<br />Cette validation comptable est obligatoire sur le plan légal et interdit par la suite de :<br />- modifier les honoraires validés,<br />- supprimer les honoraires validés.<br /><br /><br /><br /><b>Voulez-vous valider les honoraires filtrés ?</b><br />");
  if (QMessageBox::information( this, TR("Validation comptable d'honoraires"),msg , TR("&OK"), QString::null, QString::null, 0 , 1 ) == 0)
  { int nbUp = 0;
    m_pComptaMoteurBase->valideHonoraires( m_Hono_Liste, m_pUtilisateurActuel, nbUp );
    msg = QString( TR("%1 honoraire(s) validés dans la base de données sans erreur.")).arg(nbUp);
    QMessageBox::information( this, TR("Validation comptable d'honoraires"),msg , TR("&OK"), QString::null, QString::null, 0 , 1 );
  }
}

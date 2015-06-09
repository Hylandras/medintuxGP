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


#define TR 	QObject::tr

void Daf_View::init()
{ m_QTable_DAF->setColumnWidth(0, 150);
  m_QTable_DAF->setColumnWidth(1, 100);
  m_QTable_DAF->setColumnWidth(2, 100);
  m_QTable_DAF->setColumnWidth(3, 50);
  m_QTable_DAF->setColumnWidth(4, 100);
  m_QTable_DAF->setColumnWidth(5, 20);
  
  m_pHono_Liste_DAF = 0;
  m_Verbose = FALSE;
  m_pComptaMoteurBase = 0;
  m_Modified = FALSE;
  m_Popup = 0;
}

void Daf_View::destroy()
{ if (m_Verbose) qWarning("DAF_VIEW::Destruct");
  if (m_pHono_Liste_DAF)	delete m_pHono_Liste_DAF;
  // m_pComptaMoteurBase est deleted par CApp
  if (m_Verbose) qWarning("DAF_VIEW::Destruct Fin");
}


void Daf_View::setParams(ComptaMoteurBase* 	pComptaMoteurBase,
			 Utilisateurs*		pUtilisateurActuel,
			 bool			verbose)
{ m_pUtilisateurActuel = pUtilisateurActuel;
  m_Verbose = verbose;
  m_pComptaMoteurBase = pComptaMoteurBase;
  m_pHono_Liste_DAF = new HONO_LISTE;

  setTableTo();
}

void Daf_View::m_QTable_DAF_contextMenuRequested( int, int, const QPoint & p )
{bool valide = FALSE;
 bool canBeValidate = FALSE;

 HONO_LISTE::iterator it = m_pHono_Liste_DAF->at( m_QTable_DAF->currentRow() );
 if (((*it).getRemarque().find(DAF_OK) != -1) ||
     ((*it).getRemarque().find(CHEQUE_DEPOSE) != -1) ||
     ((*it).getValidite() != 0))
 	{ valide = TRUE; }

 if (((*it).getRemarque().find(DAF_PAS_OK) != -1) ||
     ((*it).getRemarque().find(CHEQUE_PAS_DEPOSE) != -1) ||
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


/*!\brief SLOT appelé par le popupmenu de la table des honoraires qui ouvre la fenêtre de visualisation des honoraires pour l'honoraire sélectionné. 
*/
void Daf_View::honoValide()
{ // Vérifie qu'une ligne est bien sélectionnée sinon msg erreur
  if (m_QTable_DAF->currentSelection () != -1)
  { // récupère l'enregistrement en cours
    HONO_LISTE::iterator  it = m_pHono_Liste_DAF->at( m_QTable_DAF->currentRow() );
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
    }
  } // Fin if column selected
}


/*!\brief SLOT appelé par le popupmenu de la table des honoraires qui ouvre la fenêtre de visualisation des honoraires pour l'honoraire sélectionné. 
*/
void Daf_View::honoView()
{ // Vérifie qu'une ligne est bien sélectionnée sinon msg erreur
  if (m_QTable_DAF->currentSelection () != -1)
  { // récupère l'enregistrement en cours
    HONO_LISTE::iterator  it = m_pHono_Liste_DAF->at( m_QTable_DAF->currentRow() );
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
void Daf_View::honoModif()
{ // Vérifie qu'une ligne est bien sélectionnée sinon msg erreur
  if (m_QTable_DAF->currentSelection () != -1)
    {// récupère l'honoraire à modifier
     HONO_LISTE::iterator  it = m_pHono_Liste_DAF->at( m_QTable_DAF->currentRow() );
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
	   { setTableTo();
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
void Daf_View::honoDelete()
{ // Vérifie qu'une ligne est bien sélectionnée sinon msg erreur
  if (m_QTable_DAF->currentSelection () != -1 && m_pHono_Liste_DAF)
   {// Récupère l'honoraire sélectionné
    HONO_LISTE::iterator  it = m_pHono_Liste_DAF->at( m_QTable_DAF->currentRow() );
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
	     m_pHono_Liste_DAF->remove ( it );
	     setTableTo( );
	     //emit( honoChange() );
   	   }
         }  // Fin if MsgBox de suppression
    } // Fin if remarque
   } // Fin if Selection et m_Hono_Liste
}



/*! \brief Rafraîchit la table des honoraires avec la liste d'honoraires passée en pointeur. 
 *  \todo mettre une ligne sur deux en gris clair, et en rouge les montants dus...
*/
void Daf_View::setTableTo( )
{ bool voirTout, voirValides, voirNonValides;
  voirTout=FALSE; voirValides=FALSE; voirNonValides=FALSE;

  switch (m_Combo_Voir->currentItem())
  { case 0 : { voirTout=TRUE; voirValides=FALSE; voirNonValides=FALSE; break; }
    case 1 : { voirTout=FALSE; voirValides=TRUE; voirNonValides=FALSE; break; }
    case 2 : { voirTout=FALSE; voirValides=FALSE; voirNonValides=TRUE; break; }
  }

 // Récupère les honoraires DAF pour la période
 if (!m_pComptaMoteurBase->getHonoraires_DAF_ForGestion( m_pHono_Liste_DAF ) )
 { if (m_Verbose) qWarning(m_pComptaMoteurBase->m_messageErreur); return;}

 for (int i = 0; i < m_QTable_DAF->numRows(); ++i)
 { m_QTable_DAF->clearCell(i, 0);
   m_QTable_DAF->clearCell(i, 1);
   m_QTable_DAF->clearCell(i, 2);
   m_QTable_DAF->clearCell(i, 3);
   m_QTable_DAF->clearCell(i, 4);
 }

 if ((m_pHono_Liste_DAF->count() == 0) || (!m_pHono_Liste_DAF))
 { m_QTable_DAF->setNumRows( 1 );
   m_QTable_DAF->setText(0,0, TR("Aucun DAF enregistré pour cette période") );
   for (int i = 1; i < 9; ++i)
 	m_QTable_DAF->setText(0,i, " " );
 }
 else
 { int r = 0;
   double daf = 0.0;
   HONO_LISTE::iterator it;
   m_QTable_DAF->setNumRows( m_pHono_Liste_DAF->count() );
   for (it = m_pHono_Liste_DAF->begin(); it !=  m_pHono_Liste_DAF->end(); ++it )
   { if ((voirTout) || (((*it).getRemarque().find(DAF_OK) != -1) && (voirValides))
		     || (((*it).getRemarque().find(DAF_PAS_OK) != -1) && (voirNonValides))
	 )
      {
	// Récupère le nom de tous les actes
	//m_QTable_DAF->setText( r,1, (*it).getActesToString() );
	// Récupère les données de l'honoraire
	m_QTable_DAF->setText( r,0, (*it).getPatient() );
	m_QTable_DAF->setText( r,2, (*it).getActesToString() );
	m_QTable_DAF->setText( r,1, (*it).getDate().toString("dd-MM-yyyy") );
	m_QTable_DAF->setText( r,3, QString::number( (*it).getDAF() ) );
    m_QTable_DAF->setItem( r,4, new QCheckTableItem( m_QTable_DAF, TR("Encaisser") ) );
	if ((*it).getRemarque().find(DAF_OK) != -1)
	{ QCheckTableItem* item = dynamic_cast<QCheckTableItem*>(m_QTable_DAF->item(r,4));
	  item->setChecked(TRUE);
	}
	m_QTable_DAF->setText( r,5, " "  );

	daf += (*it).getDAF();
    r++;
      } // Fin if 
    } // Fin for
   if (r == 0)  // Aucun DAF selon filtre combobox
   { m_QTable_DAF->setNumRows( 1 );
     m_QTable_DAF->setText(0,0, TR("Aucun DAF"));
     m_QTable_DAF->setText(0,1, TR("pour la période"));
     m_QTable_DAF->setText(0,2, TR("filtrée"));
     m_QTable_DAF->setText(0,4, "");
     m_QTable_DAF->setText(0,5, "");
     m_QTable_DAF->setText(0,6, "");
   }
   m_LineEdit_TotalDAF->setText(QString::number(daf, 'f', 2));
 } // Fin if count() == 0
}


/*! \brief Check tous les DAF en cours dans le tableau.*/
void Daf_View::but_SelectAll_clicked()
{ for (int i=0; i < m_QTable_DAF->numRows(); ++i)
    { QCheckTableItem* item = dynamic_cast<QCheckTableItem*>(m_QTable_DAF->item(i,4));
      item->setChecked(TRUE);
    }
  m_Modified = TRUE;
}


/*! \brief UnCheck tous les DAF en cours dans le tableau.*/
void Daf_View::but_DeSelect_clicked()
{ for (int i=0; i < m_QTable_DAF->numRows(); ++i)
    { QCheckTableItem* item = dynamic_cast<QCheckTableItem*>(m_QTable_DAF->item(i,4));
      item->setChecked(FALSE);
    }
  m_Modified = TRUE;
}


void Daf_View::m_Combo_Voir_activated( int )
{ setTableTo(); }


void Daf_View::m_QTable_DAF_clicked( int r, int, int, const QPoint & )
{ QCheckTableItem* item = dynamic_cast<QCheckTableItem*>(m_QTable_DAF->item(r,4)); 
  if (item->isChecked()) item->setChecked(FALSE); else item->setChecked(TRUE); 
  m_Modified = TRUE;
}


void Daf_View::but_Encaisser_clicked()
{ // Continue que si modification des checkbox
  if (!m_Modified) return;
  HONO_LISTE::iterator it;
  HONO_LISTE* p_Liste_DAF_Modifie = new HONO_LISTE;
  p_Liste_DAF_Modifie->clear();
  
  // Pour tous les honoraires récupère le statut encaissé ou non encaissé
 for (uint i=0; i < m_pHono_Liste_DAF->count(); ++i)
  { it = m_pHono_Liste_DAF->at ( i );
    // Récupère le checkbox 
    QCheckTableItem* item = dynamic_cast<QCheckTableItem*>(m_QTable_DAF->item(i,4));
   
    // Test modification d'état d'encaissement
    if (item->isChecked()) 
    { if ( (*it).getRemarque().find(DAF_PAS_OK) != -1) p_Liste_DAF_Modifie->append( (*it) ); 
    }
    
  }
  // Met à jour la base de données
  if (p_Liste_DAF_Modifie->count() != 0)
  { int nbUp = 0;
    m_pComptaMoteurBase->encaisseDAF( p_Liste_DAF_Modifie, m_pUtilisateurActuel , nbUp);
    QString msg = QString( TR("%1 DAF viennent d'être encaissés.") ).arg(nbUp);
    QMessageBox::information(this, TR("Encaissement des DAF"), msg , TR("Ok"),"","",1,0);
  }
  delete p_Liste_DAF_Modifie;
  setTableTo();
}

 
 
 
 
 

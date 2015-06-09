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

void Mvts_Ui::init()
{if (G_pCApp->m_Verbose) qWarning("Création de CMDI_Mouvements");

 m_TreeView->addColumn(TR("Libellé"));
 m_TreeView->addColumn(TR("Total"));
 m_TreeView->addColumn(TR("Id"));
 m_TreeView->setColumnWidthMode(2,QListView::Maximum);
 m_ListItem_Recettes = new QListViewItem(m_TreeView, TR("Recettes") );
 m_ListItem_Depenses = new QListViewItem(m_TreeView, TR("Dépenses")); 


   // structure le tableau
   m_QTable_Mvts->setSelectionMode(QTable::SingleRow);
   m_QTable_Mvts->setReadOnly(TRUE);
   m_QTable_Mvts->horizontalHeader()->setLabel( 0, tr( "Libellé" ) );
   m_QTable_Mvts->horizontalHeader()->setLabel( 1, tr( "Mouvement" ) );
   m_QTable_Mvts->horizontalHeader()->setLabel( 2, tr( "Date de valeur" ) );
   m_QTable_Mvts->horizontalHeader()->setLabel( 3, tr( "Débit" ) );
   m_QTable_Mvts->horizontalHeader()->setLabel( 4, tr( "Crédit" ) );
   // Modifier la géométrie de la table
   m_QTable_Mvts->setColumnWidth ( 0, 150 );
   m_QTable_Mvts->setColumnWidth ( 1, 150 );
   m_QTable_Mvts->setColumnWidth ( 2, 100 );
   m_QTable_Mvts->setColumnWidth ( 3, 100);
   m_QTable_Mvts->setColumnWidth ( 4, 100);

   m_QTable_Immob->setSelectionMode(QTable::SingleRow);
   m_QTable_Immob->setReadOnly(TRUE);

   m_QTable_Immob->horizontalHeader()->setLabel( 0, tr( "Libellé" ) );
   m_QTable_Immob->horizontalHeader()->setLabel( 1, tr( "Valeur intiale" ) );
   m_QTable_Immob->horizontalHeader()->setLabel( 2, tr( "Date" ) );
   m_QTable_Immob->horizontalHeader()->setLabel( 3, tr( "Durée" ) );


   // Modifier la géométrie de la table
   m_QTable_Immob->setColumnWidth ( 0, 200 );
   m_QTable_Immob->setColumnWidth ( 1, 150 );
   m_QTable_Immob->setColumnWidth ( 2, 100 );
   m_QTable_Immob->setColumnWidth ( 3, 100 );
   m_QTable_Immob->setSorting(FALSE);

}

/*! \brief Destructeur de la classe. */
void Mvts_Ui::destroy()
{ if (G_pCApp->m_Verbose) qWarning("Mvts_Ui::Destruct");
  if (m_Mvts_Liste_Dispo) 	delete m_Mvts_Liste_Dispo;
  if (m_Mvts_Liste) 		delete m_Mvts_Liste;
  if (m_ListItem_Recettes) 	delete m_ListItem_Recettes;
  if (m_ListItem_Depenses) 	delete m_ListItem_Depenses;
  if (m_Immo_Liste) 		delete m_Immo_Liste;
  if (m_PopupMenu) 		delete m_PopupMenu;
  if (G_pCApp->m_Verbose) qWarning("Mvts_Ui::Destruct Fin");
}

/*! \brief Définit les paramètres nécessaire au bon fonctionnement de la classe. En même temps, prépare les différents tableaux et combobox. */
void Mvts_Ui::setParams(ComptaMoteurBase *pComptaMoteurBase)
{// Intialisation des variables
 m_pComptaMoteurBase = pComptaMoteurBase;
 m_Mvts_Liste_Dispo = new MVTS_LISTE;
 if (!m_Mvts_Liste_Dispo) return ;
 m_Mvts_Liste = new MVTS_LISTE;
 if (!m_Mvts_Liste) return ;
 m_Comptes_Liste = new COMPTES_LISTE;
 if (!m_Comptes_Liste) return ;
 m_Immo_Liste = new IMMO_LISTE;
 if (!m_Immo_Liste) return ;

 m_Comptes_Liste->clear();
 m_PopupMenu = 0;
 m_Mvts_Liste_Dispo->clear();
 m_Mvts_Liste->clear();

 if (!m_pComptaMoteurBase->getMvtsDispo(*m_Mvts_Liste_Dispo))
  { QWidget* l = new QWidget();
    QMessageBox::information(l, TR("Erreur base de données"), m_pComptaMoteurBase->m_messageErreur, TR("Ok"),"","",1,0);
    delete l;
    return ;
  }
 getComboComptes();
 createTreeView();
 getMouvements();
 setTable_ImmobTo();

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////                                        ///////////////////////////////////
/////////////////////////////////        GESTION DES MOUVEMENTS          ///////////////////////////////////
/////////////////////////////////                                        ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \brief Calcul les espèces restantes dans l'année d'exercice actuelle et propose de les intégrés dans les mouvements. */
void Mvts_Ui::integrationEspeces()
{ // Récupère l'année
  QString an = QString::number( m_pComptaMoteurBase->m_Filtre_Date_Debut.year() );
  // Lance l'intégration
  if (!m_pComptaMoteurBase->integrationEspecesRestantes( an ))
  { QMessageBox::information(this, TR("Erreur d'accès à la base de données"), m_pComptaMoteurBase->m_messageErreur, TR("Ok"),"","",1,0); 
  }
}


/*! \brief Prépare le combobox des comptes bancaires disponibles avec les libellés des comptes bancaires de l'utilisateur.
*/
void Mvts_Ui::getComboComptes()
{ int r = 0;
  int id = 0;
  if (!m_pComptaMoteurBase->getComptes( *m_Comptes_Liste ))
   QMessageBox::information(this, TR("ERREUR"),m_pComptaMoteurBase->m_messageErreur, TR("Ok"),"","",1,0);
  else
  { m_Combo_ListeComptes->clear();
    for (COMPTES_LISTE::iterator it = m_Comptes_Liste->begin(); it != m_Comptes_Liste->end(); ++it)
    { m_Combo_ListeComptes->insertItem( (*it).getLibelle() );
      m_Combo_ListeComptes_2->insertItem( (*it).getLibelle() );
      if ((*it).isDefaut()) id=r;
      r++;
    }
    m_Combo_ListeComptes->insertItem(TR("Tous les comptes"));
    m_Combo_ListeComptes_2->insertItem(TR("Tous les comptes"));
    m_Combo_ListeComptes->setCurrentItem(id);
    m_Combo_ListeComptes_2->setCurrentItem(id);
  }
}


/*! \brief Récupère les mouvements enregistrés par l'utilisateur pour la période filtrée et met à jour l'affichage (tableau et treeview). */
bool Mvts_Ui::getMouvements()
{ m_Mvts_Liste->clear();
  if (!m_pComptaMoteurBase->getMvts( *m_Mvts_Liste ))
  { QWidget* l = new QWidget();
    QMessageBox::information(l, TR("Erreur base de données"), m_pComptaMoteurBase->m_messageErreur, TR("Ok"),"","",1,0);
    delete l;
    return FALSE;
  }
 fillTreeView();
 setTableMvtsTo();
 return TRUE;
}


/*! \brief Crée le m_TreeView des mouvements. Cette fonction doit être appelée avant tout autre concernant m_TreeView. */
void Mvts_Ui::createTreeView()
{ QString tmp = "";
  if ((m_Mvts_Liste_Dispo->count() == 0) || (!m_Mvts_Liste_Dispo))
   { //m_TreeView->setNumRows( 1 );
   }
  else
   { MVTS_LISTE::iterator it;

     // Prépare l'affichage du TreeView
     for (it = m_Mvts_Liste_Dispo->begin(); it !=  m_Mvts_Liste_Dispo->end(); ++it )
      { // Prépare l'affichage de le m_TreeView
	int id = (*it).getId();
	if (id < 10) tmp = "0" + QString::number(id);
	else tmp = QString::number(id);
	if ((*it).getType() == 0)  // Recettes
	 { // Test si child ou parent
	   if ( (*it).getIdParent() == 0)
	   {  new QListViewItem(m_ListItem_Recettes, (*it).getNom() , 0, tmp); 
	   }
	   else
	   {	// Récupère le nom du parent pour le retrouver dans le m_TreeView
		QString nom_parent = (*m_Mvts_Liste_Dispo->at( (*it).getIdParent() - 1 )).getNom();
		QListViewItem* parent = m_TreeView->findItem(nom_parent, 0);
		if (parent) new QListViewItem( parent , (*it).getNom(), 0, tmp);
		else new QListViewItem( m_ListItem_Recettes , (*it).getNom(), 0, tmp);
	   }
	 } // Fin type = recettes
	else   // Dépenses
	 { // Test si child ou parent
	   if ( (*it).getIdParent() == 0)
	   {  new QListViewItem(m_ListItem_Depenses, (*it).getNom(), 0, tmp); 
	   }
	   else
	   {	// Récupère le nom du parent pour le retrouver dans le m_TreeView
		QString nom_parent = (*m_Mvts_Liste_Dispo->at( (*it).getIdParent() - 1 )).getNom();
		QListViewItem* parent = m_TreeView->findItem(nom_parent, 0);
		if (parent) new QListViewItem( parent , (*it).getNom(), 0, tmp);
		else new QListViewItem( m_ListItem_Depenses , (*it).getNom(), 0, tmp);
	   }
	 } // Fin else type == 0
      } // Fin for
   m_TreeView->insertItem(m_ListItem_Recettes) ;
   m_TreeView->insertItem(m_ListItem_Depenses);
   }
   m_TreeView->setSorting(2, TRUE);
//   m_TreeView->hideColumn(2);
}

/*! \brief Met à jour l'item de la m_TreeView correspondant au Mouvements avec son montant.*/
void Mvts_Ui::fillListItem( Mouvements* mvt )
{ QString nom_parent = "";
  MVTS_LISTE::iterator rec = m_Mvts_Liste_Dispo->at( mvt->getId() - 1 );
  nom_parent = (*rec).getNom();
  m_TreeView->findItem(nom_parent, 0)->setText(1, QString::number( mvt->getTotal() , 'f', 2) );
  if ((mvt->getTotal() > 0 ) && (!mvt->isParent()) )
  { rec = m_Mvts_Liste_Dispo->at( mvt->getIdParent() - 1 );
    (*rec).addToTotal( mvt->getTotal() );
    m_TreeView->findItem((*rec).getNom(),0)->setText(1, QString::number((*rec).getTotal(), 'f', 2));
  }
}


/*! \brief Remplit le m_TreeView avec les mouvements_disponibles après avoir calculé les totaux de chaque mouvements.*/
void Mvts_Ui::fillTreeView()
{ double total_recettes, total_depenses;
  total_recettes = 0;
  total_depenses = 0;
  MVTS_LISTE::iterator it;

  // Initialise les totaux avant calculs
  for (it = m_Mvts_Liste_Dispo->begin(); it !=  m_Mvts_Liste_Dispo->end(); ++it )
   { (*it).setTotal(0); }

  // Récupère le compte sélectionné
  int idCpt = -1;
  if (m_Combo_ListeComptes->currentItem() != (m_Combo_ListeComptes->count() - 1))
   idCpt = (*m_Comptes_Liste->at( m_Combo_ListeComptes->currentItem()) ).getId();

  // Calcul les totaux
  for (it = m_Mvts_Liste->begin(); it !=  m_Mvts_Liste->end(); ++it )
    { if ((idCpt == -1) || ((*it).getIdCompte() == idCpt))
      { (*m_Mvts_Liste_Dispo->at( (*it).getIdParent() - 1 )).addToTotal( (*it).getTotal() ); 
        if ((*it).getType() == 0) total_recettes += (*it).getTotal();
        else total_depenses += (*it).getTotal();
      }
    }

  // Remplit le m_TreeView
  for (it = m_Mvts_Liste_Dispo->begin(); it !=  m_Mvts_Liste_Dispo->end(); ++it )
    { fillListItem( &(*it) );    }

  m_ListItem_Recettes->setText(1, QString::number(total_recettes, 'f', 2));
  m_ListItem_Depenses->setText(1, QString::number(total_depenses, 'f', 2));
  m_TreeView->hideColumn(2);
}


/*! \brief SLOT appelé lorsque l'utilisateur désire ajouter un mouvement. */
void Mvts_Ui::slot_Ajouter_Mvts()
{ Mvts_Add* m_pMvts_Add = new Mvts_Add();
  m_pMvts_Add->setParams( m_pComptaMoteurBase->m_pUtilisateurActuel, 
			 QDate::currentDate().toString("dd-MM-yyyy"), 
			 m_pComptaMoteurBase, G_pCApp->m_Verbose);
  m_pMvts_Add->setSeeOnly(FALSE);
  m_pMvts_Add->setForUpdateOnly(FALSE);
  m_pMvts_Add->exec();
  if (m_pMvts_Add->result() == QDialog::Accepted)
  { getMouvements();
//    emit ( mvtsChange() );
  }
  delete m_pMvts_Add;
  // Mettre à jour le m_TreeView
  getMouvements();
}

/*! \brief SLOT appelé lorsque l'utilisateur désire imprimer les mouvements enregistrés. 
 *  \todo Mvts_Ui::Impression : Fonction rudimentaire à retravailler.
*/
void Mvts_Ui::slot_Impression_Mvts()
{ QPainter* m_painter = new QPainter();
  QPrinter* m_printer = new QPrinter();
  Tools*    tools     = new Tools();
  if ((tools) && (m_printer) && (m_painter))
  {if (m_printer->setup())
   {MVTS_LISTE::iterator it;
    QStringList hd, lines;
    hd.clear();
    lines.clear();
    QString retour = "";
    QString tmp = "";
    double recettes, depenses;
    recettes = 0; 
    depenses = 0;

    m_printer->setDocName("Synthese_mouvements_"+QDateTime::currentDateTime().toString("dd-MM-yyyy_hh:mm:ss:ms"));
    QFont m_font( "Arial", 10);
    QColorGroup sp_colorgroup (colorGroup());
    sp_colorgroup.setColor (QColorGroup::Text, QColor(0,0,0) );
    sp_colorgroup.setColor (QColorGroup::BrightText, QColor(0,0,0) );
    sp_colorgroup.setColor (QColorGroup::ButtonText, QColor(0,0,0) );
    sp_colorgroup.setColor (QColorGroup::HighlightedText, QColor(0,0,0) );

    retour += "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /></head>\n";
    retour += "<body>\n";
    retour =  "<p align=\"center\"><b>"+TR("Synthèse des mouvements")+"</b></p>";
    retour += "<p align=\"center\"><b>"+G_pCApp->m_currentUser->getNom()+"</b></p>";
    retour += "<p>";

    hd << TR("Filtre en cours") << TR("Date de début") << TR("Date de fin");
    lines << "&nbsp;" << m_pComptaMoteurBase->m_Filtre_Date_Debut.toString("dddd dd MMMM yyyy") << m_pComptaMoteurBase->m_Filtre_Date_Fin.toString("dddd dd MMMM yyyy");
    tools->createHTMLTable(hd, lines, retour, 2, 0);
    retour += "<br /><br />";
    hd.clear();
    lines.clear();

    hd << TR("Catégorie") << TR("Libellé") << TR("Date") << TR("Débit") << TR("Crédit") << TR("Validation");
    QString validite = "";
    for (it = m_Mvts_Liste->begin(); it !=  m_Mvts_Liste->end(); ++it )
     {	tmp = (*m_Mvts_Liste_Dispo->at( (*it).getIdParent() - 1)).getNom();
	if ((*it).getValidite() == 0) validite = TR("Non"); else validite=TR("Oui");
	if ((*it).getType() == 0) // Recettes
	 { lines << tmp << (*it).getNom() << (*it).getDate().toString("dd-MM-yyyy") << "&nbsp;" << QString::number( (*it).getTotal() , 'f', 2) << validite;
	   recettes += (*it).getTotal();
	 }
	else
	 { lines << tmp << (*it).getNom() << (*it).getDate().toString("dd-MM-yyyy") << QString::number( (*it).getTotal() , 'f', 2) << "&nbsp;";
	   depenses += (*it).getTotal();
	 }
	tmp = "";
     }

   lines << "<b>"+TR("TOTAL")+"</b>" << TR("Pour la période filtrée") << "&nbsp;" << QString::number( depenses , 'f', 2) << QString::number( recettes , 'f', 2);
   tools->createHTMLTable(hd, lines, retour, 2,0);
   retour += "<br /><br />\n\n</body></html>";

   QSimpleRichText* toPrint = new QSimpleRichText( retour, m_font );

    m_painter->begin(m_printer);
    toPrint->draw(m_painter, 50, 50, QRect(50,50,600,1000) , sp_colorgroup );
    m_painter->end();
    delete m_painter;
    delete m_printer;
    delete tools;
   }
  }
}


/*! \brief Met à jour le tableau des mouvements enregistrés. Une autre fonction se charge de les récupérés depuis la base de données ( getMouvements ). */
void Mvts_Ui::setTableMvtsTo()
{ if ((m_Mvts_Liste->count() == 0) || (!m_Mvts_Liste))
  { m_QTable_Mvts->setNumRows( 1 );
    m_QTable_Mvts->setText(0,0, TR("Aucun mouvement") );
    m_QTable_Mvts->setText(0,1, TR("pour cette période") );

    for (int i = 2; i < m_QTable_Mvts->numCols(); ++i)
 	m_QTable_Mvts->setText(0,i, " " );
  }
  else
  { int r = 0;
    int idInList = 0;
    double recettes, depenses;
    recettes = 0;
    depenses = 0;
    MVTS_LISTE::iterator it;
    int idCpt = -1;
    if (m_Combo_ListeComptes->currentItem() != (m_Combo_ListeComptes->count() - 1))
       idCpt = (*m_Comptes_Liste->at( m_Combo_ListeComptes->currentItem()) ).getId();
    m_QTable_Mvts->setNumRows( m_Mvts_Liste->count() );
    m_idTable = QMemArray<int> (m_Mvts_Liste->count());
    for (int i=0; i<m_idTable.count();++i)
	{ m_idTable[i]=-1; }

    for (it = m_Mvts_Liste->begin(); it !=  m_Mvts_Liste->end(); ++it )
    { if ( (idCpt == -1) ||  ((*it).getIdCompte() == idCpt) )
      {	m_QTable_Mvts->setText( r,0, (*it).getNom() );
	// Récupère les données de l'honoraire
	m_QTable_Mvts->setText( r,1, (*m_Mvts_Liste_Dispo->at( (*it).getIdParent() - 1)).getNom() );
	m_QTable_Mvts->setText( r,2, (*it).getDateValeur().toString("dd-MM-yyyy") );
	if ((*it).getType() == 0) // Recettes
	{ 	m_QTable_Mvts->setText( r,4, QString::number( (*it).getTotal() , 'f', 2)  );
		m_QTable_Mvts->setText( r,3, " ");
		recettes += (*it).getTotal();
	}
	else
	{	m_QTable_Mvts->setText( r,3, QString::number( (*it).getTotal() , 'f', 2)  );
		m_QTable_Mvts->setText( r,4, " ");
		depenses += (*it).getTotal();
	}
	m_idTable[r] = idInList;
	r++;
      }
      idInList++;
     }
     m_QTable_Mvts->setNumRows(r+1);
     m_QTable_Mvts->setText( r, 0, TR("TOTAL"));
     m_QTable_Mvts->setText( r, 1, " ");
     m_QTable_Mvts->setText( r, 2, " ");
     m_QTable_Mvts->setText( r, 4, QString::number(recettes, 'f', 2));
     m_QTable_Mvts->setText( r, 3, QString::number(depenses, 'f', 2));
  }
}


/*! \brief Slot appelé par le boutton de la classe (but_Valide) dont la fonction est de valider tous les mouvements en cours de filtrage puis de les mettre à jour dans la base de données.
 *  Un message d'information indique que cette étape est irréversible et obligatoire dans la tenue de la comptabilité.
 *  A noter que seuls les mouvements crées par des immobilisations ne seront pas validés.
*/
void Mvts_Ui::but_Valide_clicked()
{ QString msg = "";
  msg  = TR("Attention, vous vous apprêtez à valider les mouvements actuellement affichés sur l'écran.<br />Cette validation comptable est obligatoire sur le plan légal et interdit par la suite de :<br />- modifier les mouvements validés,<br />- supprimer les mouvements validés.<br /><br /><br /><br /><b>Voulez-vous valider les mouvements filtrés ?</b><br />");
  if (QMessageBox::information( this, TR("Validation comptable de mouvements"),msg , TR("&OK"), QString::null, QString::null, 0 , 1 ) == 0)
  { int nbUp = 0;
    int idCpt = (*m_Comptes_Liste->at( m_Combo_ListeComptes->currentItem()) ).getId();
    m_pComptaMoteurBase->valideMouvements( m_Mvts_Liste, 
				     m_pComptaMoteurBase->m_pUtilisateurActuel, 
				     idCpt, nbUp );
    msg = QString( TR("%1 mouvements(s) validés dans la base de données sans erreur.")).arg(nbUp);
    QMessageBox::information( this, TR("Validation comptable de mouvements"),msg , TR("&OK"), QString::null, QString::null, 0 , 1 );
    if (nbUp != 0) getMouvements();
  }
}



/*! \brief SLOT appelé lorsque l'uilisateur sollicite le menu contextuel du tableau des mouvements. */
void Mvts_Ui::popupTable( int , int c, const QPoint &p )
{if (m_PopupMenu) delete m_PopupMenu;

 // Test le mouvement sélectionné pour savoir s'il s'agit d'une immobilisation ou pas
 MVTS_LISTE::iterator  it = m_Mvts_Liste->at( m_idTable[m_QTable_Mvts->currentRow()] );
 bool immob, valide;
 if ( (*it).getIdParent() == ID_IMMOBILISATIONS ) immob = TRUE; else immob = FALSE;
 if ( (*it).getValidite() == 0) valide=FALSE; else valide=TRUE;

 // Créer le m_PopupMenuup selon le résultat d'au-dessus
 m_PopupMenu = new QPopupMenu(this, "m_PopupMenuTable");
 if (m_PopupMenu) 
 { m_colTrie=c;
   m_PopupMenu->insertItem (TR("Voir"),      this, SLOT(mvtView() ) );
   if ((!immob) && (!valide))
   { m_PopupMenu->insertItem (TR("Modifier") , this, SLOT(mvtModif()  ) );
     m_PopupMenu->insertItem (TR("Supprimer"), this, SLOT(mvtDelete() ) );
     m_PopupMenu->insertItem (TR("Valider"), this, SLOT(mvtValide() ) );

   }
    m_PopupMenu->insertItem (TR("Trier par ordre croissant")   , this, SLOT(trieAsc( ) ) );
    m_PopupMenu->insertItem (TR("Trier par ordre décroissant") , this, SLOT(trieDesc( )  ) );
   m_PopupMenu->popup(p);
 }
}


/*! \brief valide le mouvement sélectionné par l'utilisateur. */
void Mvts_Ui::mvtValide()
{ if (m_QTable_Mvts->currentSelection () != -1)
  { // récupère l'enregistrement en cours
    MVTS_LISTE::iterator  it = m_Mvts_Liste->at( m_idTable[m_QTable_Mvts->currentRow()] );
    MVTS_LISTE* Mvts_valide = new MVTS_LISTE;
    Mvts_valide->clear();
    Mvts_valide->append( (*it) );
    QString msg = "";
     msg  = TR("Attention, vous vous apprêtez à valider des mouvements.<br />Cette validation comptable est obligatoire sur le plan légal et interdit par la suite de :<br />- modifier les mouvements validés,<br />- supprimer les mouvements validés.<br /><br /><br /><br /><b>Voulez-vous valider les mouvements filtrés ?</b><br />");
    if (QMessageBox::information( this, TR("Validation comptable des mouvements"),msg , TR("&OK"), QString::null, QString::null, 0 , 1 ) == 0)
    { int nbUp = 0;
      int idCpt = (*m_Comptes_Liste->at( m_Combo_ListeComptes->currentItem()) ).getId();
      m_pComptaMoteurBase->valideMouvements( Mvts_valide, 
					     m_pComptaMoteurBase->m_pUtilisateurActuel, 
					     idCpt, nbUp );
      msg = QString( TR("%1 mouvement(s) validés dans la base de données sans erreur.")).arg(nbUp);
      QMessageBox::information( this, TR("Validation comptable de mouvements"),msg , TR("&OK"), QString::null, QString::null, 0 , 1 );
      if (nbUp != 0) getMouvements();
    }
   delete Mvts_valide;
  } // Fin if column selected
}


/*! \brief Trie le tableau des chèques en fonction de la colonne demandée de façon ascendante */
void Mvts_Ui::trieAsc()
{ m_QTable_Mvts->setSorting( TRUE );
  m_QTable_Mvts->sortColumn ( m_colTrie, TRUE, TRUE);
  m_QTable_Mvts->setSorting( FALSE );
}


/*! \brief Trie le tableau des chèques en fonction de la colonne demandée de façon descendante */
void Mvts_Ui::trieDesc()
{ m_QTable_Mvts->setSorting( TRUE );
  m_QTable_Mvts->sortColumn ( m_colTrie, FALSE, TRUE);
  m_QTable_Mvts->setSorting( FALSE );
}


/*! \brief SLOT appelé lorsque l'utilisateur désire visualiser un mouvement grâce au menu contextuel du tableau des mouvements. */
void Mvts_Ui::mvtView()
{ // Vérifie qu'une ligne est bien sélectionnée sinon msg erreur
  if (m_QTable_Mvts->currentSelection () != -1)
  { // récupère l'enregistrement en cours et le compte auquel il se réfère
    MVTS_LISTE::iterator  it = m_Mvts_Liste->at( m_idTable [ m_QTable_Mvts->currentRow() ] );
    Comptes cpt = (*m_Comptes_Liste->at( m_Combo_ListeComptes->currentItem()) );

    Mvts_Visualisation* pMvts_Visualisation = new Mvts_Visualisation(this);
    pMvts_Visualisation->setMouvement( &(*it), &cpt, *m_Mvts_Liste_Dispo);
    pMvts_Visualisation->exec();
    delete pMvts_Visualisation;
  } // Fin if selection
}

/*! \brief SLOT appelé lorsque l'utilisateur désire modifier un mouvement grâce au menu contextuel du tableau des mouvements. */
void Mvts_Ui::mvtModif()
{ // Vérifie qu'une ligne est bien sélectionnée sinon msg erreur
  if (m_QTable_Mvts->currentSelection () != -1)
  {// récupère l'honoraire à modifier
   MVTS_LISTE::iterator  it = m_Mvts_Liste->at( m_idTable[m_QTable_Mvts->currentRow()] );
   Mvts_Add* m_pMvts_Add = new Mvts_Add();
   if (m_pMvts_Add)
   {	m_pMvts_Add->setParams( m_pComptaMoteurBase->m_pUtilisateurActuel, 
				QDate::currentDate().toString("dd-MM-yyyy"), m_pComptaMoteurBase, G_pCApp->m_Verbose);
	m_pMvts_Add->setForUpdateOnly(TRUE);
	m_pMvts_Add->setMvt( &(*it) );
	m_pMvts_Add->exec();
	// Mettre à jour le m_TreeView
	if (m_pMvts_Add->result() == QDialog::Accepted)
	{ getMouvements();
//	  emit ( mvtsChange() );
	}
	delete m_pMvts_Add;	
       } // Fin if m_pMvtsAdd
     }
}

/*! \brief SLOT appelé lorsque l'utilisateur désire effacer un mouvement grâce au menu contextuel du tableau des mouvements. */
void Mvts_Ui::mvtDelete()
{ // Vérifie qu'une ligne est bien sélectionnée sinon msg erreur
  if (m_QTable_Mvts->currentSelection () != -1 && m_Mvts_Liste)
   {// Récupère l'honoraire sélectionné
    MVTS_LISTE::iterator  it = m_Mvts_Liste->at( m_idTable[m_QTable_Mvts->currentRow()] );
    // Ouvrir un msgbox pour la demande de confirmation
    QString msg = TR("Vous avez sélectionné le mouvement suivant : <br /><br />");
    msg += TR("Libellé : ") + (*it).getNom() + " <br />";
    msg += TR("Date de valeur : ") + (*it).getDateValeur().toString("dd MMM yyyy") + " <br />";
    msg += TR("Catégorie : ") + (*m_Mvts_Liste_Dispo->at( (*it).getIdParent() )).getNom() + " <br />";
    msg += TR("Montant : ") + QString::number( (*it).getTotal() , 'f', 2)  + " <br />";
    msg += TR("Id : ") + QString::number( (*it).getId() );
    msg += ".<br /><br />";

	msg += TR("<b>Confirmez-vous la suppression de mouvement ?</b>");

	if ( QMessageBox::information( this, TR("Suppression"),
        	msg, TR("&Supprimer"), TR("&Annuler"), QString::null, 
		1 , 2 )  == 0 )
     	{  if (!m_pComptaMoteurBase->deleteMvt( (*it).getId() ) )
           { // Message d'erreur
	  	QMessageBox::information( this, TR("Une erreur est survenue..."),
          	m_pComptaMoteurBase->m_messageErreur, TR("&OK"), QString::null, QString::null, 1 , 2 );
	   }
	   else
	   { //rafraîchit la table des honoraires et recalcul la synthèse
	     getMouvements();
//	     emit( mvtsChange() );
   	   }
         }  // Fin if MsgBox de suppression
   } // Fin if Selection et honoListe
}






////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////                                        ///////////////////////////////////
/////////////////////////////////       GESTION DES IMMOBILISATIONS      ///////////////////////////////////
/////////////////////////////////                                        ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*!\brief SLOT appelé lors d'un clique sur le tableau des immobilisations.
 * Vérifie qu'un honoraire est sélectionné dans le tableau. Puis affiche une boîte de dialogue de confirmation enfin efface l'honoraire et met à jour le tableau.
*/
void Mvts_Ui::popupTable_Immob( int , int , const QPoint &p )
{ m_PopupMenu = new QPopupMenu(this, "popTable");
  if (m_PopupMenu) 
  { m_PopupMenu->insertItem (TR("Voir"),      this, SLOT(immView() ) );
    m_PopupMenu->insertItem (TR("Modifier") , this, SLOT(immModif()  ) );
    m_PopupMenu->insertItem (TR("Vendre le bien"), this, SLOT(immVendre() ) );
    m_PopupMenu->popup(p);
  }
}


void Mvts_Ui::immVendre()
{ QMessageBox::information (this, TR("Version 1.00"), TR("Cette fonction n'est pas encore implémentée.\nCette fonction sera disponible dans la prochaine version de ce logiciel.\nMerci de votre patience."),TR("Ok"),"","",1,0);
  // emit ( immobChange() );
}


void Mvts_Ui::slot_Ajouter_Immob()
{ Immobilisations_Add* m_pImmob_Add = new Immobilisations_Add();
  m_pImmob_Add->setParams( G_pCApp->m_currentUser, QDate::currentDate().toString("dd-MM-yyyy"), m_pComptaMoteurBase);
  m_pImmob_Add->setSeeOnly(FALSE);
  m_pImmob_Add->setForUpdateOnly(FALSE);
  m_pImmob_Add->exec();
  if (m_pImmob_Add->result() == QDialog::Accepted)
  { setTable_ImmobTo();
//    emit ( immobChange() );
  }
    delete m_pImmob_Add;
}


void Mvts_Ui::createImmoHTMLTable(QString& retour,bool avecEntete)
{ retour += "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /></head>\n<body>\n";

  if (avecEntete)
    {	retour += "<p align=\"center\"><b>" + TR("Liste des immobilisations enregistrées<br />") + G_pCApp->m_currentUser->getNom() + "</b><br />";
	retour += "</p>\n\n";
	retour += "<p align=\"center\"><b>";
	retour += (*m_Comptes_Liste->at ( m_Combo_ListeComptes->currentItem() ) ).getRib("html");
	retour += "</b><br /></p>\n";
	retour += "<p align=\"center\">" + TR("Date du jour : ") + QDate::currentDate().toString("dddd dd MMMM yyyy") + "</p>\n\n";
    } // Fin if entete


   IMMO_LISTE::iterator it;
   for (it = m_Immo_Liste->begin(); it != m_Immo_Liste->end(); ++it)
    {retour += "<p>" + (*it).getResultat() + "</p>";
     retour += "<p>&nbsp;</p>";
    } // Fin FOR

  retour += "</body></html>";
}


void Mvts_Ui::fichierHTML_Immob()
{ // Récupère le chemin vers le répertoire de sauvegarde
  QString nFile = G_pCApp->m_PathSauvegarde + QDir::separator() + "Immobilisations.html";

  // Prépare le fichier de sauvegarde
  QFile* f = new QFile(nFile);
  if (f)
  { if (f->exists()) f->remove();
    if ( f->open( IO_WriteOnly ) )
     {QTextStream stream( f );
      QString tmp = "";
      createImmoHTMLTable( tmp, TRUE);
      stream << tmp;
      f->close();
      QMessageBox::information(this, TR("Fichier enregistré."), TR("La remise de chèques a été enregistrée dans le fichier :")+"<br  /><b>"+nFile+"</b>", TR("Ok"), "", "", 1, 0);
      } // Fin if open
     delete f;
   } // Fin if f
}


void Mvts_Ui::immModif()
{ // Vérifie qu'une ligne est bien sélectionnée sinon msg erreur
  if (m_QTable_Immob->currentSelection () != -1)
  {// récupère l'honoraire à modifier
   IMMO_LISTE::iterator  it = m_Immo_Liste->at( m_QTable_Immob->currentRow() );
   Immobilisations_Add* m_pImmob_Add = new Immobilisations_Add();
   if (m_pImmob_Add)
   {	m_pImmob_Add->setParams( G_pCApp->m_currentUser, 
				QDate::currentDate().toString("dd-MM-yyyy"), m_pComptaMoteurBase);
	m_pImmob_Add->setForUpdateOnly(TRUE);
	m_pImmob_Add->setImm( &(*it) );
	m_pImmob_Add->exec();
	// Mettre à jour le treeView
	if (m_pImmob_Add->result() == QDialog::Accepted)
	{ setTable_ImmobTo();
//	  emit ( immobChange() );
	}
	delete m_pImmob_Add;	
       } // Fin if m_pMvtsAdd
     }
}


void Mvts_Ui::immView()
{ // Vérifie qu'une ligne est bien sélectionnée sinon msg erreur
  if (m_QTable_Immob->currentSelection () != -1)
  {// récupère l'honoraire à modifier
   IMMO_LISTE::iterator  it = m_Immo_Liste->at( m_QTable_Immob->currentRow() );
   Immobilisations_Add* m_pImmob_Add = new Immobilisations_Add();
   if (m_pImmob_Add)
   {	m_pImmob_Add->setParams( G_pCApp->m_currentUser, 
				QDate::currentDate().toString("dd-MM-yyyy"), m_pComptaMoteurBase);
	m_pImmob_Add->setSeeOnly(TRUE);
	m_pImmob_Add->setImm( &(*it) );
	m_pImmob_Add->exec();
	delete m_pImmob_Add;	
       } // Fin if m_pMvtsAdd
     }
}


/*! \brief Rafraîchit la table des comptes avec la liste des comptes passée en pointeur. 
 *  \todo mettre une ligne sur deux en gris clair
*/
void Mvts_Ui::setTable_ImmobTo()
{ m_Immo_Liste->clear();
  if (!m_pComptaMoteurBase->getImmobilisations( m_Immo_Liste ))
  { QWidget* l = new QWidget();
    QMessageBox::information(l, TR("Erreur base de données"), m_pComptaMoteurBase->m_messageErreur, TR("Ok"),"","",1,0);
    delete l;
    return ;
  }

  int idCpt = -1;
  if (m_Combo_ListeComptes_2->currentItem() != (m_Combo_ListeComptes_2->count() - 1))
   idCpt = (*m_Comptes_Liste->at( m_Combo_ListeComptes->currentItem()) ).getId();

  if ((m_Immo_Liste->count() == 0) || (!m_Immo_Liste))
  { m_QTable_Immob->setNumRows( 1 );
    m_QTable_Immob->setText(0,0, TR("Aucune immobilisation") );
    for (int i = 1; i < m_QTable_Immob->numCols(); ++i)
 	m_QTable_Immob->setText(0,i, " " );
  }
  else
  { int r = 0;
    IMMO_LISTE::iterator it;
    m_QTable_Immob->setNumRows( m_Immo_Liste->count() );
    for (it = m_Immo_Liste->begin(); it !=  m_Immo_Liste->end(); ++it )
    {if ((*it).getIdCompte() == idCpt)
      {	m_QTable_Immob->setText( r,0, (*it).getNom() );
	m_QTable_Immob->setText( r,1, QString::number((*it).getValeur()) );
	m_QTable_Immob->setText( r,2, (*it).getDateService().toString("dd-MM-yyyy") );
	m_QTable_Immob->setText( r,3, QString::number((*it).getDuree()) );
	r++;
      }
     }
    m_QTable_Immob->setNumRows( r );
    m_QTable_Immob->setSorting(TRUE);
    m_QTable_Immob->sortColumn(5, FALSE, TRUE);
    m_QTable_Immob->setSorting(FALSE);
  }
}


void Mvts_Ui::m_Combo_ListeComptes_activated( int )
{ getMouvements(); }


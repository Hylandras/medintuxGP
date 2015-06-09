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

#include <qfiledialog.h>

void Parametres_Ui::init()
{  m_Popup = 0;
   m_Comptes_Liste = 0;
   // Modifier la géométrie de la table
   m_QTable_Comptes->setColumnWidth ( 0, 100 );
   m_QTable_Comptes->setColumnWidth ( 1, 100 );
   m_QTable_Comptes->setColumnWidth ( 2, 100 );
   m_QTable_Comptes->setColumnWidth ( 3, 100 );

   // Connecte le popup du tableau
   connect (m_QTable_Comptes ,   SIGNAL (contextMenuRequested ( int, int, const QPoint & ) ),
	    this, SLOT   (popupTable ( int, int, const QPoint &)  ) 
	    );
}


/*! \brief Destructeur de la classe */
void Parametres_Ui::destroy()
{if (m_Verbose) qWarning("Parametres_Ui::Destruct");
 if (m_Popup)		delete m_Popup;
 if (m_Comptes_Liste)	delete m_Comptes_Liste;
 // m_pComptaMoteurBase et m_pUtilisateurActuel sont deleted par CApp::~CApp
 if (m_Verbose) qWarning("Parametres_Ui::Destruct Fin");
}


void Parametres_Ui::setParams(ComptaMoteurBase* pComptaMoteurBase,
			      Utilisateurs*	pUtilisateurActuel,
			      QString&		pathDeSauvegarde,
			      bool		verbose)
{m_PathSauvegarde = pathDeSauvegarde;
 m_pUtilisateurActuel = pUtilisateurActuel;
 m_pComptaMoteurBase = pComptaMoteurBase;
 m_Verbose = verbose;
 m_Comptes_Liste = new COMPTES_LISTE;
 if (!m_Comptes_Liste) return ;
 m_Comptes_Liste->clear();
 // Met à jour les lineedit de la fenêtre avec les données
 m_LineEdit_NomUsr->setText( m_pUtilisateurActuel->getNom() );
 m_pathDoc->setText(m_PathSauvegarde);
 setTableTo();
}


void Parametres_Ui::selectDirDoc()
{ QString s = QFileDialog::getExistingDirectory(
                    QDir::home().path(),
                    this,
                    TR("Choisissez un répertoire"),
                    TR("Choisissez le répertoire de sauvegarde des documents"),
                    TRUE );
  if (!s.isEmpty() )
   { m_pathDoc->setText( s );
   }
}


/*! \brief sauvegarde les données utilisateurs dans le fichier ini de l'application. 
 *  Attention : Fonction dépendante de CApp  
*/
void Parametres_Ui::sauvParams()
{ // Sauvegarde du chemin vers les documents
  if (CGestIni::Param_WriteParam(&G_pCApp->m_Param,"Sauvegarde","Path",m_pathDoc->text()) == 0)
  { if (m_Verbose)  qWarning("Impossible d'écrire dans le fichier ini le répertoire des documents.");}
  CGestIni::Param_UpdateToDisk( G_pCApp->m_PathIni ,G_pCApp->m_Param);
  G_pCApp->m_PathSauvegarde = m_pathDoc->text();  
}

/*! \brief Créer un nouveau compte bancaire pour l'utilisateur en cours. */
void Parametres_Ui::but_NouveauCompte_clicked()
{Cpts_Add *ca = new Cpts_Add();
 if (ca)
 {ca->setParams(m_pUtilisateurActuel, m_pComptaMoteurBase);
  ca->setSeeOnly(FALSE);
  ca->setForUpdateOnly(FALSE);
  ca->exec();
  // Met à jour le tableau
  if (ca->result() == QDialog::Accepted)
  { setTableTo(); 
    //emit(comptesChange()); 
  }
  delete ca;
 }
}


void Parametres_Ui::cptView()
{ if (m_QTable_Comptes->currentSelection () != -1)
  { // récupère l'enregistrement en cours
    COMPTES_LISTE::iterator  it = m_Comptes_Liste->at( m_QTable_Comptes->currentRow() );
    Cpts_Add *ca = new Cpts_Add();
    if (ca)
    {	ca->setSeeOnly(TRUE);
	ca->setCpt( &(*it) );
	ca->exec();
	delete ca;
    } // Fin if ca
  } // fin if selection
}


void Parametres_Ui::cptModif()
{ if (m_QTable_Comptes->currentSelection () != -1)
  { // récupère l'enregistrement en cours
    COMPTES_LISTE::iterator  it = m_Comptes_Liste->at( m_QTable_Comptes->currentRow() );
    Cpts_Add *ca = new Cpts_Add();
    if (ca)
    {	ca->setSeeOnly(FALSE);
	ca->setParams(G_pCApp->m_currentUser, m_pComptaMoteurBase);
	ca->setForUpdateOnly(TRUE);
	ca->setCpt( &(*it) );
	ca->exec();
	// Met à jour le tableau
	if (ca->result() == QDialog::Accepted ) 
	{ setTableTo();
	  //emit( comptesChange() );
        }
	delete ca;
    } // Fin if ca
  } // fin if selection
}


/*!\brief SLOT appelé lors d'un clique sur le tableau des honoraires.
 * Vérifie qu'un honoraire est sélectionné dans le tableau. Puis affiche une boîte de dialogue de confirmation enfin efface l'honoraire et met à jour le tableau.
*/
void Parametres_Ui::popupTable( int , int , const QPoint &p )
{if (m_Popup) delete m_Popup;
 m_Popup = new QPopupMenu(this, "popTable");
 if (m_Popup) 
 { m_Popup->insertItem (TR("Voir"),      this, SLOT(cptView() ) );
   m_Popup->insertItem (TR("Modifier") , this, SLOT(cptModif()  ) );
//   m_Popup->insertItem (TR("Supprimer"), this, SLOT(honoDelete() ) );
   m_Popup->popup(p);
 }
}



/*! \brief Rafraîchit la table des comptes avec la liste des comptes passée en pointeur. 
 *  \todo mettre une ligne sur deux en gris clair
*/
void Parametres_Ui::setTableTo()
{// Récupère les comptes déjà enregistrés
 if (!m_pComptaMoteurBase->getComptes(*m_Comptes_Liste))
  { QMessageBox::information(this, TR("ERREUR"), m_pComptaMoteurBase->m_messageErreur, TR("Ok"), "","",1,0);
  }
 if ((m_Comptes_Liste->count() == 0) || (!m_Comptes_Liste))
   { m_QTable_Comptes->setNumRows( 1 );
     m_QTable_Comptes->setText(0,0, TR("Aucun comptes enregistré") );
     for (int i = 1; i < 3; ++i)
 	m_QTable_Comptes->setText(0,i, " " );
   }
  else
   { int r = 0;
     COMPTES_LISTE::iterator it;
     m_QTable_Comptes->setNumRows( m_Comptes_Liste->count() );
     for (it = m_Comptes_Liste->begin(); it !=  m_Comptes_Liste->end(); ++it )
      { m_QTable_Comptes->setText( r,0, (*it).getLibelle() );
	m_QTable_Comptes->setText( r,1, (*it).getTitulaire() );
	m_QTable_Comptes->setText( r,2, (*it).getNomBanque() );
	if ((*it).isDefaut() )
	 m_QTable_Comptes->setText( r,3, TR("Par défaut"));
	else  m_QTable_Comptes->setText( r,3, TR(" "));
	r++;
      }
   }
}


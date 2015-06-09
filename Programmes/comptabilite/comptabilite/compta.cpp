/***************************************************************************
 *   Copyright (C) 2007, 2008 by Eric MAEKER   *
 *   eric.maeker@free.fr   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "compta.h"

/*! \class Compta
 * \brief Classe principale de l'application. Gère toutes les CMDI, construit la fenêtre principale.
 * \todo Caractère UTF8 dans la Caption de la fenêtre principale.
*/
Compta::Compta(ComptaMoteurBase* pComptaMoteurBase)
    : QMainWindow( 0, "ComptaTux", WDestructiveClose )
{
 m_pComptaMoteurBase       = pComptaMoteurBase;
 m_currentUser             = m_pComptaMoteurBase->m_pUtilisateurActuel;

 QString nom = m_currentUser->getNom();

 setCaption( TR("Comptabilité libérale pour MedinTux - ") + nom );

 QImage img;
 img.loadFromData( icone, sizeof( icone ), "PNG" );
 setIcon( img );

/*
 //..........Insère la Barre de menus
 QPopupMenu *menu = new QPopupMenu( this );
 menuBar()->insertItem( tr("&Fichier"), menu );
 menu->insertItem( tr("A&ide"), this, SLOT( about() ), Key_F1 );

 QPopupMenu *utilisateur = new QPopupMenu( this );
 menuBar()->insertItem( tr("&Utilisateur"), utilisateur );
 utilisateur->insertItem( tr("A&ide"), this, SLOT( about() ), Key_F1 );

 QPopupMenu *documents = new QPopupMenu( this );
 menuBar()->insertItem( tr("&Documents"), documents );
 documents->insertItem( tr("Afficher le remise de chèque"), this, 
				SLOT( remiseCheques() ), Key_F10 );
 documents->insertItem( tr("Afficher le remise d\'espèces"), this, 
				SLOT( remiseEspeces() ), Key_F11 );

*/

 //.......... Prépare les Widgets à Docker (Filtre puis Menu)
 // Crée un Dock
 QDockWindow *pDock;
 pDock = new QDockWindow (this, "DockMenu");
 pDock->setResizeEnabled(TRUE);
 pDock->setFixedExtentWidth (260);
 pDock->setCloseMode (QDockWindow::Never);
 pDock->setCaption(tr("Filtre"));
 addToolBar( pDock, Qt::DockLeft );

 // Insère le Filtre
 m_pFiltreHono = new Filtre_Hono(pDock, "FormFiltre");
 pDock->setWidget( m_pFiltreHono );
 m_pFiltreHono->setEnabled(TRUE);
 m_pFiltreHono->setParams( m_pComptaMoteurBase );

 // Crée un autre Dock
 QDockWindow *pDock2;
 pDock2 = new QDockWindow (this, "DockMenu");
 pDock2->setResizeEnabled(TRUE);
 pDock2->setFixedExtentWidth (250);
 pDock2->setCloseMode (QDockWindow::Never);
 pDock2->setCaption(tr("Menu"));
 addToolBar( pDock2, Qt::DockLeft );
 // Insère le menu
 m_pFormListMenu = new FormListMenu(pDock2, "FormListMenu");
 pDock2->setWidget( m_pFormListMenu );
 m_pFormListMenu->setEnabled(TRUE);
 // Connecter le menu
    connect	( m_pFormListMenu, SIGNAL( honorairesSelected() ),
		  this,            SLOT  ( honorairesView()     )
		);
    connect	( m_pFormListMenu, SIGNAL( actesSelected() ),
		  this,            SLOT  ( actesView()     )
		);
    connect	( m_pFormListMenu, SIGNAL( banqueSelected() ),
		  this,            SLOT  ( banqueView()     )
		);
    connect	( m_pFormListMenu, SIGNAL( paramSelected() ),
		  this,            SLOT  ( paramView()     )
		);

    connect	( m_pFormListMenu, SIGNAL( gestionDAFSelected() ),
		  this,            SLOT  ( DAFView()     )
		);

	connect	( m_pFormListMenu, SIGNAL( gestionCMUSelected() ),
		  this,            SLOT  ( CMUView()     )
		);

    connect	( m_pFormListMenu, SIGNAL( mouvementsSelected() ),
		  this,            SLOT  ( mouvementsView()     )
		);

    connect	( m_pFormListMenu, SIGNAL( releveSelected() ),
		  this,            SLOT  ( releveView()     )
		);

    connect	( m_pFormListMenu, SIGNAL( livresSelected() ),
		  this,            SLOT  ( livresView()     )
		);

    connect	( m_pFormListMenu, SIGNAL( aboutSelected() ),
		  this,            SLOT  ( about()     )
		);


 //.......... Creation du QVbox central
 QVBox *pQVCentral = new QVBox( this );
 pQVCentral->setMargin( 5 );
 pQVCentral->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
 //.......... Barre d'onglets pour les fenêtres CMDI
 //................... y placer un QWorkspace ..................................................
 //                    qui contiendra et gerera les fenetres MDI
 QWorkspace   *pQWorkspace     = new QWorkspace(pQVCentral);
 m_pQWorkSpaceRub = pQWorkspace;
 pQWorkspace->setScrollBarsEnabled( TRUE );
 setCentralWidget(  pQVCentral );

 //.................. y connecter les onglets rubriques .........................................
 //                   pour qu'il mettent à jour celui actif
 //                   lorsqu'une fenetre est cliquée

 //........ Insérer la fenêtre de visualisation des honoraires CMDI_Honoraires
 CMDI_HonoCreate();
 m_pFiltreHono->mCBox->setCurrentItem(QDate::currentDate().month() - 1);
 m_pFiltreHono->combo_mois_annee_activated( 0 );
 resize( 950, 700 );

}

Compta::~Compta()
{  }

//---------------------------------- setupEditActions ---------------------------------------------------------------------------
// ACTION: met en place la barre de gestion des action texte (couper coller etc ...) avec les menus correspondant

void Compta::setupEditActions()
{
    QToolBar *tb = new QToolBar( this );
    QPopupMenu *menu = new QPopupMenu( this );
    menuBar()->insertItem( tr( "&Fichier" ), menu );
    tb->setLabel( tr("Outils Fichiers") );

}


//---------------------------------- setupTextActions ---------------------------------------------------------------------------
// ACTION: met en place la barre de gestion des attributs texte avec les menus correspondant
void Compta::setupTextActions()
{ 
}

/*! \brief affiche la fenêtre d'aide.
    \todo SLOT AIDE ------ A FAIRE
*/
void Compta::slot_Aide()
{ QWidget *aide = new QWidget();
  QLabel *lbl = new QLabel(aide);
  lbl->setText( TR("Application") );
}



/*!\brief Teste si une des fenêtres rubriques dont le nom est fourni en entrée est ouverte dans le WorkSpace des rubriques.
 * \return CMDI_Generic*: qui est  0 si pas de rubrique de ce nom ouverte dans le WorkSpace des rubriques un pointeur sur cette rubrique dans la liste des rubriques du WorkSpace des rubriques
*/
CMDI_Generic* Compta::IsExistRubrique(int type)
{QWidgetList windows = m_pQWorkSpaceRub->windowList();
 if ( !windows.count() )        return 0;
 int typ = -1;
 for ( int i = 0; i < int(windows.count()); ++i )
     {QWidget *pQwdgRub   = windows.at(i);
      CMDI_Generic *pCMDI = (CMDI_Generic*)pQwdgRub;
      typ                 = pCMDI->GetType();
      if (typ==type)
	{ return pCMDI;}
      delete pCMDI;
     }
 return 0;
}



//-----------------------------------------------------------------
//----------------------  CMDI_HonoCreate -------------------------
//-----------------------------------------------------------------
/*! \brief Crée et Affiche la fenêtre de gestion des honoraires dans le WorkSpace global de l'application.
 *  La fonction retourne le pointeur vers la fenêtre CMDI_Honoraires ainsi créée.
 * \sa CMDI_Honoraires , Hono_View
 */
CMDI_Honoraires* Compta::CMDI_HonoCreate ()
{ CMDI_Honoraires* pCMDI_HV = new CMDI_Honoraires(m_pQWorkSpaceRub,"CMDI_Honoraires", WDestructiveClose, m_pComptaMoteurBase);
  if (pCMDI_HV==0) return 0;
  pCMDI_HV->setCaption(tr("Honoraires"));
  //.......... Connecter la fenêtre aux signaux de l'appli
  // Sign_ActiverRubrique
  connect( this,     SIGNAL ( Sign_ActiverRubrique(const char*)),
           pCMDI_HV, SLOT   ( ActiverRubrique(const char*))
         );
//    connect (pCMDI_HV, SIGNAL ( honoChange() ),
// 	    this,     SLOT   ( emitHonoChange() ) );

  // sign_Filtre_Changed
  connect (	m_pFiltreHono, SIGNAL( sign_Filtre_Changed( ) ),
		pCMDI_HV,	SLOT  ( changeTableFilter( )  )
	  );

  pCMDI_HV->changeTableFilter();
//    connect(pCMDI_HV, SIGNAL( message(const QString&, int) ), statusBar(), SLOT( message(const QString&, int )) );

  pCMDI_HV->setGeometry ( QRect( 0, 0, 600, 390 ) );
  pCMDI_HV->showMaximized();
  return pCMDI_HV;
}


//-----------------------------------------------------------------
//----------------------  CMDI_MouvementsCreate -------------------------
//-----------------------------------------------------------------
/*! \brief Crée et Affiche la fenêtre de gestion des mouvements dans le WorkSpace global de l'application.
 *  La fonction retourne le pointeur vers la fenêtre CMDI_Mouvements ainsi créée.
 * \sa CMDI_Mouvements
 */
CMDI_Mouvements* Compta::CMDI_MouvementsCreate ()
{COMPTES_LISTE*  cptListe = new COMPTES_LISTE;
 if (!cptListe) return 0;
 if (!m_pComptaMoteurBase->getComptes(*cptListe))
  { QMessageBox::information(this, TR("ERREUR"), m_pComptaMoteurBase->m_messageErreur, TR("Ok"),"","",1,0);
    return 0;
  }
  if (cptListe->count() == 0)
  { QMessageBox::information(this, TR("ATTENTION"), TR("Avant de préparer des mouvements financiers, vous devez configurer un compte bancaire."), TR("OK"),"","",1,0);
    return 0;
  }
  delete cptListe;
  //........... Créer la fenêtre Honoraires
  CMDI_Mouvements* pCMDI_HV = new CMDI_Mouvements(m_pQWorkSpaceRub,"CMDI_Mouvements", WDestructiveClose, m_pComptaMoteurBase);
  if (pCMDI_HV==0) return 0;
  pCMDI_HV->setCaption(tr("Mouvements"));
  //.......... Connecter la fenêtre aux signaux de l'appli
  // Sign_ActiverRubrique
  connect( this,     SIGNAL( Sign_ActiverRubrique(const char*)),
           pCMDI_HV, SLOT( ActiverRubrique(const char*))
         );
//    connect (pCMDI_HV, SIGNAL ( mouvementsChange() ),
// 	    this,     SLOT   ( emitMouvementsChange() ) );
// 
   connect (	m_pFiltreHono, SIGNAL( sign_Filtre_Changed( ) ),
 		pCMDI_HV,	SLOT  ( getMouvements( )  )
 	  );
// 
//   connect( this, SIGNAL( cptChange() ), pCMDI_HV, SLOT( getComboComptes() ) );

//    connect(pCMDI_HV, SIGNAL( message(const QString&, int) ), statusBar(), SLOT( message(const QString&, int )) );

  pCMDI_HV->setGeometry ( QRect( 0, 0, 600, 390 ) );
  pCMDI_HV->showMaximized();
  return pCMDI_HV;
}



//-----------------------------------------------------------------
//----------------------  CMDI_ReleveCreate -------------------------
//-----------------------------------------------------------------
/*! \brief Crée et Affiche la fenêtre de gestion des mouvements dans le WorkSpace global de l'application.
 *  La fonction retourne le pointeur vers la fenêtre CMDI_Mouvements ainsi créée.
 * \sa CMDI_Mouvements
 */
CMDI_Releve* Compta::CMDI_ReleveCreate ()
{COMPTES_LISTE*  cptListe = new COMPTES_LISTE;
 if (!cptListe) return 0;
 if (!m_pComptaMoteurBase->getComptes(*cptListe))
  { QMessageBox::information(this, TR("ERREUR"), m_pComptaMoteurBase->m_messageErreur, TR("Ok"),"","",1,0);
    return 0;
  }
  if (cptListe->count() == 0)
  { QMessageBox::information(this, TR("ATTENTION"), TR("Avant d'accéder aux relevés de comptes vous devez créer au moins un compte bancaire."), TR("Ok"),"","",1,0);
    return 0;
  }
  delete cptListe;

  //........... Créer la fenêtre Honoraires
  CMDI_Releve* pCMDI_HV = new CMDI_Releve(m_pQWorkSpaceRub,"CMDI_Releve", WDestructiveClose, m_pComptaMoteurBase);
  if (pCMDI_HV==0) return 0;
  pCMDI_HV->setCaption(tr("Relevé"));
  //.......... Connecter la fenêtre aux signaux de l'appli
  // Sign_ActiverRubrique
  connect( this,     SIGNAL( Sign_ActiverRubrique(const char*)),
           pCMDI_HV, SLOT( ActiverRubrique(const char*))
         );

//   connect( this, SIGNAL( mvtChange() ), pCMDI_HV, SLOT( setTableTo() ) );
// 
//   connect( this, SIGNAL( depChange() ), pCMDI_HV, SLOT( setTableTo() ) );
// 
//   connect( this, SIGNAL( cptChange() ), pCMDI_HV, SLOT( getComboComptes() ) );

//    connect(pCMDI_HV, SIGNAL( message(const QString&, int) ), statusBar(), SLOT( message(const QString&, int )) );

  pCMDI_HV->setGeometry ( QRect( 0, 0, 600, 390 ) );
  pCMDI_HV->showMaximized();
  return pCMDI_HV;
}



//-----------------------------------------------------------------
//----------------------  CMDI_ParamCreate -------------------------
//-----------------------------------------------------------------
/*! \brief Crée et Affiche la fenêtre de gestion des honoraires dans le WorkSpace global de l'application.
 *  La fonction retourne le pointeur vers la fenêtre CMDI_Honoraires ainsi créée.
 * \sa CMDI_Honoraires , Hono_View
 */
CMDI_Param* Compta::CMDI_ParamCreate ()
{ CMDI_Param* pCMDI_HV = new CMDI_Param(m_pQWorkSpaceRub,"CMDI_Param", WDestructiveClose, m_pComptaMoteurBase);
  if (pCMDI_HV==0) return 0;
  pCMDI_HV->setCaption(tr("Paramètres"));
  //.......... Connecter la fenêtre aux signaux de l'appli
  // Sign_ActiverRubrique
  connect( this,     SIGNAL( Sign_ActiverRubrique(const char*)),
           pCMDI_HV, SLOT( ActiverRubrique(const char*))
         );
//    connect (pCMDI_HV, SIGNAL ( comptesChange() ),
// 	    this,     SLOT   ( emitComptesChange() ) );

//    connect(pCMDI_HV, SIGNAL( message(const QString&, int) ), statusBar(), SLOT( message(const QString&, int )) );

  pCMDI_HV->setGeometry ( QRect( 0, 0, 600, 390 ) );
  pCMDI_HV->showMaximized();
  return pCMDI_HV;
}




//-----------------------------------------------------------------
//----------------------  CMDI_DepotsCreate -----------------------
//-----------------------------------------------------------------
/*! \brief Crée et affiche la fenêtre de gestion des dépôts et remises.
*/
CMDI_Depots* Compta::CMDI_DepotsCreate ()
{COMPTES_LISTE*  cptListe = new COMPTES_LISTE;
 if (!cptListe) return 0;
 if (!m_pComptaMoteurBase->getComptes(*cptListe))
  { QMessageBox::information(this, TR("ERREUR"), m_pComptaMoteurBase->m_messageErreur, TR("Ok"),"","",1,0);
    return 0;
  }
  if (cptListe->count() == 0)
  { QMessageBox::information(this, TR("ATTENTION"), TR("Avant de préparer des remises de chèques ou d'espèces, vous devez configurer un compte bancaire."), TR("Ok"),"","",1,0);
    return 0;
  }
  delete cptListe;

 CMDI_Depots* pCMDI_HV = new CMDI_Depots(m_pQWorkSpaceRub,"CMDI_Depots", WDestructiveClose, m_pComptaMoteurBase);
 if (pCMDI_HV==0) return 0;

//    connect(pCMDI_HV, SIGNAL( message(const QString&, int) ), statusBar(), SLOT( message(const QString&, int )) );

 pCMDI_HV->setCaption(tr("Banque"));

    //.................. connecter les fenêtres filles: Sign_ActiverRubrique ......................................
    //                   elles pourront recevoir le message d'activation
 connect( this,     SIGNAL( Sign_ActiverRubrique(const char*)),
             pCMDI_HV, SLOT( ActiverRubrique(const char*))
           );

 connect (	m_pFiltreHono, SIGNAL( sign_Filtre_Changed( ) ),
		pCMDI_HV,	SLOT  ( changeTableFilter( )  )
	  );

//  connect (pCMDI_HV, SIGNAL ( depotsChange() ),
// 	    this,     SLOT   ( emitDepotsChange() ) );
// 
//   connect( this, SIGNAL( honChange() ), pCMDI_HV, SLOT( changeTableFilter() ) );
// 
//   connect( this, SIGNAL( cptChange() ), pCMDI_HV, SLOT( comptesChanged() ) );



 pCMDI_HV->setGeometry ( QRect( 0, 0, 600, 390 ) );
 pCMDI_HV->showMaximized();
 return pCMDI_HV;
}



//-----------------------------------------------------------------
//---------------------- CMDI_ActesCreate -------------------------
//-----------------------------------------------------------------
/*! \brief Crée et Affiche la fenêtre de gestion des actes dans le WorkSpace global de l'application.
 *  La fonction retourne le pointeur vers la fenêtre CMDI_Actes ainsi créée.
 * \sa CMDI_Actes 
*/
CMDI_Actes* Compta::CMDI_ActesCreate ()
{//................... creer la 4 eme fenetre MDI (fenetres Identité) ...........................
 CMDI_Actes* pCMDI_HV = new CMDI_Actes(m_pQWorkSpaceRub,"CMDI_Actes", WDestructiveClose, m_pComptaMoteurBase);

//    connect(pCMDI_HV, SIGNAL( message(const QString&, int) ), statusBar(), SLOT( message(const QString&, int )) );

 pCMDI_HV->setCaption(tr("Actes"));

 //.................. connecter les fenêtres filles: Sign_ActiverRubrique ......................................
 //                   elles pourront recevoir le message d'activation
 connect( this,     SIGNAL( Sign_ActiverRubrique(const char*)),
             pCMDI_HV, SLOT( ActiverRubrique(const char*))
           );
 // Connecter la CMDI avec le filtre pour l'avertir d'un modification de la liste des actes
 connect ( pCMDI_HV,     SIGNAL( sign_ActesListeModif() ),
              m_pFiltreHono, SLOT( slot_ActesListeModif() )
           );

 pCMDI_HV->setGeometry ( QRect( 0, 0, 600, 390 ) );
 pCMDI_HV->showMaximized();
 return pCMDI_HV;
}


//-----------------------------------------------------------------
//---------------------- CMDI_DAFCreate -------------------------
//-----------------------------------------------------------------
/*! \brief Crée et Affiche la fenêtre de gestion des DAF dans le WorkSpace global de l'application.
 *  La fonction retourne le pointeur vers la fenêtre CMDI_DAF ainsi créée.
 * \sa CMDI_DAF 
*/
CMDI_DAF* Compta::CMDI_DAFCreate ()
{//................... creer la 4 eme fenetre MDI (fenetres Identité) ...........................
 CMDI_DAF* pCMDI_HV = new CMDI_DAF(m_pQWorkSpaceRub,"CMDI_DAF", WDestructiveClose, m_pComptaMoteurBase);
 pCMDI_HV->setCaption(tr("DAF"));

 //.................. connecter les fenêtres filles: Sign_ActiverRubrique ......................................
 //                   elles pourront recevoir le message d'activation
 connect( this,     SIGNAL( Sign_ActiverRubrique(const char*)),
             pCMDI_HV, SLOT( ActiverRubrique(const char*))
           );

 connect (	m_pFiltreHono, SIGNAL( sign_Filtre_Changed( ) ),
		pCMDI_HV,	SLOT ( filtreChanged( )  )
	  );


 pCMDI_HV->setGeometry ( QRect( 0, 0, 600, 390 ) );
 pCMDI_HV->showMaximized();
 return pCMDI_HV;
}

//-----------------------------------------------------------------
//---------------------- CMDI_CMUCreate -------------------------
//-----------------------------------------------------------------
/*! \brief Crée et Affiche la fenêtre de gestion des DAF dans le WorkSpace global de l'application.
 *  La fonction retourne le pointeur vers la fenêtre CMDI_DAF ainsi créée.
 * \sa CMDI_DAF 
*/
CMDI_CMU* Compta::CMDI_CMUCreate ()
{//................... creer la 4 eme fenetre MDI (fenetres Identité) ...........................
 CMDI_CMU* pCMDI_HV = new CMDI_CMU(m_pQWorkSpaceRub,"CMDI_CMU", WDestructiveClose, m_pComptaMoteurBase);
 pCMDI_HV->setCaption(tr("CMU"));

 //.................. connecter les fenêtres filles: Sign_ActiverRubrique ......................................
 //                   elles pourront recevoir le message d'activation
 connect( this,     SIGNAL( Sign_ActiverRubrique(const char*)),
             pCMDI_HV, SLOT( ActiverRubrique(const char*))
           );

 connect (	m_pFiltreHono, SIGNAL( sign_Filtre_Changed( ) ),
		pCMDI_HV,	SLOT ( filtreChanged( )  )
	  );


 pCMDI_HV->setGeometry ( QRect( 0, 0, 600, 390 ) );
 pCMDI_HV->showMaximized();
 return pCMDI_HV;
}

//-----------------------------------------------------------------
//---------------------- CMDI_LivresCreate -------------------------
//-----------------------------------------------------------------
/*! \brief Crée et Affiche la fenêtre de gestion des livres et bilans dans le WorkSpace global de l'application.
 *  La fonction retourne le pointeur vers la fenêtre CMDI_Livres ainsi créée.
 * \sa CMDI_Livres 
*/
CMDI_Livres* Compta::CMDI_LivresCreate ()
{COMPTES_LISTE*  cptListe = new COMPTES_LISTE;
 if (!cptListe) return 0;
 if (!m_pComptaMoteurBase->getComptes(*cptListe))
  { QMessageBox::information(this, TR("ERREUR"), m_pComptaMoteurBase->m_messageErreur, TR("Ok"),"","",1,0);
    return 0;
  }
  if (cptListe->count() == 0)
  { QMessageBox::information(this, TR("ATTENTION"), TR("Avant de pouvoir visualiser les livres de compte, vous devez configurer un compte bancaire."), TR("OK"),"","",1,0);
    return 0;
  }
  delete cptListe;

 //................... creer la 4 eme fenetre MDI (fenetres Identité) ...........................
if (G_pCApp->m_Verbose) qWarning("Compta :: Création de CMDI_Livres");
 CMDI_Livres* pCMDI_HV = new CMDI_Livres(m_pQWorkSpaceRub,"CMDI_Livres", WDestructiveClose, m_pComptaMoteurBase);
 pCMDI_HV->setCaption(tr("Livres et Bilans"));

 //.................. connecter les fenêtres filles: Sign_ActiverRubrique ......................................
 //                   elles pourront recevoir le message d'activation
 connect( this,     SIGNAL( Sign_ActiverRubrique(const char*)),
             pCMDI_HV, SLOT( ActiverRubrique(const char*))
           );

//  connect( this,     SIGNAL   ( mvtChange() ),
// 	  pCMDI_HV, SLOT     ( recalculLivres() ) );
// 
//  connect (	m_pFiltreHono,  SIGNAL( sign_Filtre_Changed( ) ),
// 		pCMDI_HV,	SLOT  ( recalculLivres( )  )
// 	  );

 pCMDI_HV->setGeometry ( QRect( 0, 0, 600, 390 ) );
 pCMDI_HV->showMaximized();
 return pCMDI_HV;
}


/*! \brief SLOT activé par le signal émit par la classe FormListMenu lorsque l'utilisateur clique dans la liste de menu.
 *  Crée la fenêtre de gestion des honoraires (CMDI_Honoraires) dans le workspace général de l'application.
 * \sa CMDI_Honoraires , FormListMenu , CMDI_HonoCreate
 */
void Compta::honorairesView()
{ emit Sign_ActiverRubrique(TR("Honoraires") ); 
  CMDI_Honoraires*      pCMDI_HV = (CMDI_Honoraires*) IsExistRubrique( TYP_HONO );
  if (pCMDI_HV==0)   pCMDI_HV = CMDI_HonoCreate ();
  if (pCMDI_HV==0)   return;
  pCMDI_HV->m_IsModified = FALSE;
  GlobalActiverWidget(pCMDI_HV);
}


/*! \brief SLOT activé par le signal émit par la classe FormListMenu lorsque l'utilisateur clique dans la liste de menu.
 *  Crée la fenêtre de gestion des honoraires (CMDI_Honoraires) dans le workspace général de l'application.
 * \sa CMDI_Honoraires , FormListMenu , CMDI_DepotsCreate
 */
void Compta::banqueView()
{ emit Sign_ActiverRubrique(TR("Banque") ); 
  CMDI_Depots*      pCMDI_A = (CMDI_Depots*) IsExistRubrique( TYP_BANQUE );
  if (pCMDI_A==0)   pCMDI_A = CMDI_DepotsCreate ();
  if (pCMDI_A==0)   return;
  pCMDI_A->m_IsModified = FALSE;
  GlobalActiverWidget(pCMDI_A);
}


/*! \brief SLOT activé par le signal émit par la classe FormListMenu lorsque l'utilisateur clique dans la liste de menu.
 *  Crée la fenêtre de gestion des relevés de comptes (CMDI_Releve) dans le workspace général de l'application.
 * \sa CMDI_Releve , FormListMenu , CMDI_ReleveCreate
 */
void Compta::releveView()
{ emit Sign_ActiverRubrique(TR("Releve") ); 
  CMDI_Releve*      pCMDI_A = (CMDI_Releve*) IsExistRubrique( TYP_RELEVE );
  if (pCMDI_A==0)   pCMDI_A = CMDI_ReleveCreate ();
  if (pCMDI_A==0)   return;
  pCMDI_A->m_IsModified = FALSE;
  GlobalActiverWidget(pCMDI_A);
}


/*! \brief SLOT activé par le signal émit par la classe FormListMenu lorsque l'utilisateur clique dans la liste de menu.
 *  Crée la fenêtre de gestion des livres (CMDI_Livres) dans le workspace général de l'application.
 * \sa CMDI_Livres , FormListMenu , CMDI_LivresCreate
 */
void Compta::livresView()
{ emit Sign_ActiverRubrique(TR("Livre") ); 
  CMDI_Livres*      pCMDI_A = (CMDI_Livres*) IsExistRubrique( TYP_LIVRES );
  if (pCMDI_A==0)   pCMDI_A = CMDI_LivresCreate ();
  if (pCMDI_A==0)   return;
  pCMDI_A->m_IsModified = FALSE;
  GlobalActiverWidget(pCMDI_A);
}


/*! \brief SLOT activé par le signal émit par la classe FormListMenu lorsque l'utilisateur clique dans la liste de menu.
 *  Crée la fenêtre de gestion des honoraires (CMDI_Honoraires) dans le workspace général de l'application.
 * \sa CMDI_Honoraires , FormListMenu , CMDI_ActesCreate
 */
void Compta::mouvementsView()
{ emit Sign_ActiverRubrique(TR("Mouvements") ); 
  CMDI_Mouvements*      pCMDI_A = (CMDI_Mouvements*) IsExistRubrique( TYP_MOUVEMENTS );
  if (pCMDI_A==0)   pCMDI_A = CMDI_MouvementsCreate ();
  if (pCMDI_A==0)   return;
//  pCMDI_A->m_IsModified = FALSE;
  GlobalActiverWidget(pCMDI_A);
}


/*! \brief SLOT activé par le signal émit par la classe FormListMenu lorsque l'utilisateur clique dans la liste de menu.
 *  Crée la fenêtre de gestion des honoraires (CMDI_Honoraires) dans le workspace général de l'application.
 * \sa CMDI_Honoraires , FormListMenu , CMDI_ActesCreate
 */
void Compta::actesView()
{ emit Sign_ActiverRubrique(TR("Actes") ); 
  CMDI_Actes*      pCMDI_A = (CMDI_Actes*) IsExistRubrique( TYP_ACTES );
  if (pCMDI_A==0)   pCMDI_A = CMDI_ActesCreate ();
  if (pCMDI_A==0)   return;
  pCMDI_A->m_IsModified = FALSE;
  GlobalActiverWidget(pCMDI_A);
}


/*! \brief SLOT activé par le signal émis par la classe FormListMenu lorsque l'utilisateur clique dans la liste de menu.
 *  Crée la fenêtre de gestion des honoraires (CMDI_DAF) dans le workspace général de l'application.
 */
void Compta::DAFView()
{ emit Sign_ActiverRubrique(TR("DAF") ); 
  CMDI_DAF*      pCMDI_A = (CMDI_DAF*) IsExistRubrique( TYP_DAF );
  if (pCMDI_A==0)   pCMDI_A = CMDI_DAFCreate ();
  if (pCMDI_A==0)   return;
  pCMDI_A->m_IsModified = FALSE;
  GlobalActiverWidget(pCMDI_A);
}

void Compta::CMUView()
{ emit Sign_ActiverRubrique(TR("CMU") ); 
  CMDI_CMU*      pCMDI_A = (CMDI_CMU*) IsExistRubrique( TYP_CMU );
  if (pCMDI_A==0)   pCMDI_A = CMDI_CMUCreate ();
  if (pCMDI_A==0)   return;
  pCMDI_A->m_IsModified = FALSE;
  GlobalActiverWidget(pCMDI_A);
}


/*! \brief SLOT activé par le signal émit par la classe FormListMenu lorsque l'utilisateur clique dans la liste de menu.
 *  Crée la fenêtre de gestion des param (CMDI_Depots) dans le workspace général de l'application.
 * \sa CMDI_Depots , FormListMenu , CMDI_DepotsCreate
 */
void Compta::paramView()
{ emit Sign_ActiverRubrique(TR("Param") ); 
  CMDI_Param*      pCMDI_A = (CMDI_Param*) IsExistRubrique( TYP_PARAM );
  if (pCMDI_A==0)   pCMDI_A = CMDI_ParamCreate ();
  if (pCMDI_A==0)   return;
  pCMDI_A->m_IsModified = FALSE;
  GlobalActiverWidget(pCMDI_A);
}



/*! \brief SLOT activé par le signal currentChanged de la barre d'onglets des rubriques qui permet d'activer le fenêtre CMDI correspondante.
 *  \param pCMDI_Qwidget : le widget à activer
 *  \sa CMDI_Depots , FormListMenu , CMDI_DepotsCreate
 */
void Compta::OnOngletRubriquesChanged(QWidget * pCMDI_Qwidget)
{OnActiverRubrique(pCMDI_Qwidget->name());
}




/*! \brief SLOT activé par le signal Sign_ActiverRubrique(const char*) provenant des Onglets des rubriques afin d'activer la bonne fenetre CMDI lorsqu'un onglet a été cliqué.
 * Cette fonction émet elle même un signal à chaque fenêtre CMDI qui doivent s'activer, si c'est leur onglet qui est responsable du signal (reperable par le parametre: const char* rubName qui le texte de l'onglet).
 *  \param rubName : le nom de la rubrique à activer (Honoraires...)
 */
void Compta::OnActiverRubrique(const char* rubName)
{emit Sign_ActiverRubrique(rubName);   // emettre signal aux rubriques filles
}


void Compta::closeEvent( QCloseEvent* ce )
{	ce->accept();
	return;
}


/*! \brief Affiche la Fenêtre A Propos de l'application. Les informations affichées sont récupérées de la base de données. 
 *  \todo centrer la fenêtre.
*/
void Compta::about()
{ QString params = "";
  QString caption = "";
  QString tmp = "";
  QString msg = "";

  // Récupère les données de la base concernant la version actuelle
  m_pComptaMoteurBase->getVersion(params);
  Param_ReadParam(params, "Version", "nom", &tmp);
  msg += "<p align=center><b>"+QString::fromUtf8(tmp)+"</b></p>";
  caption = tmp;

  Param_ReadParam(params, "Version", "num", &tmp);
  msg += "<p align=center><b>"+TR("Version de la base de données : ")+QString::fromUtf8(tmp)+" ";
  Param_ReadParam(params, "Version", "subv", &tmp);
  msg += QString::fromUtf8(tmp)+"</b><br />";
  Param_ReadParam(params, "Version", "date", &tmp);
  msg += TR("Date de la structure des données : ")+QString::fromUtf8(tmp)+"<br /><br />";
  Param_ReadParam(params, "Version", "creation", &tmp);

  msg += TR("<b>Version du logiciel : ");
  msg += VERSION_CODE;
  msg += "<br />";
  msg += TR("En date du : ");
  msg += DATE_VERSION;
  msg += "</b><br />";

  msg += TR("Créé en : ")+QString::fromUtf8(tmp)+"<br />";
  Param_ReadParam(params, "Version", "auteur", &tmp);
  msg += "Auteur principal : "+QString::fromUtf8(tmp)+"</p>";

  Param_ReadParam(params, "Version", "lien1", &tmp);
  msg += "<p align=center>"+QString::fromUtf8(tmp)+"<br />";
  Param_ReadParam(params, "Version", "lien2", &tmp);
  msg += QString::fromUtf8(tmp) + "</p>";

  Param_ReadParam(params, "Version", "info1", &tmp);
  msg += "<p>"+QString::fromUtf8(tmp)+"<br />";
  Param_ReadParam(params, "Version", "info2", &tmp);
  msg += QString::fromUtf8(tmp)+"<br />";
  Param_ReadParam(params, "Version", "info3", &tmp);
  msg += QString::fromUtf8(tmp)+"<br />";
  Param_ReadParam(params, "Version", "info4", &tmp);
  msg += QString::fromUtf8(tmp)+"<br />";
  Param_ReadParam(params, "Version", "info5", &tmp);
  msg += QString::fromUtf8(tmp)+"</p>";

  Param_ReadParam(params, "Version", "langage", &tmp);
  msg += "<p>" + TR("Ce logiciel est programmé avec : ")+QString::fromUtf8(tmp);
  Param_ReadParam(params, "Version", "licence", &tmp);
  msg += TR(" en open source, sous la licence : ")+QString::fromUtf8(tmp)+".</p>";
  Param_ReadParam(params, "Version", "medintux", &tmp);
  msg += "<p>"+QString(TR("Pour pouvoir utiliser toutes les options de ce logiciel, vous devez utiliser la version %1 de MedinTux.") ).arg(QString::fromUtf8(tmp)) + "</p>";

  msg += "<p>&nbsp;</p>";
  Param_ReadParam(params, "Remerciements", "l1", &tmp);
  msg += TR("<p align=center><b>Remerciements :</b></p><p>")+QString::fromUtf8(tmp)+"<br />";
  Param_ReadParam(params, "Remerciements", "l2", &tmp);
  msg += QString::fromUtf8(tmp)+"<br />";
  Param_ReadParam(params, "Remerciements", "l3", &tmp);
  msg += QString::fromUtf8(tmp)+"</p>";

//  msg = QString::fromUtf8(msg);
  caption      = QString::fromUtf8(caption);
  QDialog* wgt = new QDialog();
  wgt->setFixedSize(600,500);
  wgt->setCaption(caption);

  A_Propos*  pA_Propos = new A_Propos(wgt);
  pA_Propos->textVersion->setText( msg );
  pA_Propos->textVersion->setReadOnly(TRUE);

  QString changelogFile = G_pCApp->m_PathAppli + QDir::separator() + "Changelog.txt";
  //....................... ouvrir le fichier en mode QTextStream ...........................
  if ( !QFile::exists( changelogFile ) )   
   { if (G_pCApp->m_Verbose) qWarning("Compta::about : Impossible d'ouvrir le fichier " + changelogFile); }
  else
  { QFile file( changelogFile );
    if ( !file.open( IO_ReadOnly ) ) 
      { if (G_pCApp->m_Verbose) qWarning("Compta::about : Impossible de lire le fichier " + changelogFile); 
      }
    else
    { CGestIni::Param_UpdateFromDisk(changelogFile, tmp);
      pA_Propos->textChangeLog->setText(tmp);
    } // Fin if file open
   } // Fin if file exists

  changelogFile = G_pCApp->m_PathAppli + QDir::separator() + "todos.txt";
  //....................... ouvrir le fichier en mode QTextStream ...........................
  if ( !QFile::exists( changelogFile ) )   
   { if (G_pCApp->m_Verbose) qWarning("Compta::about : Impossible d'ouvrir le fichier " + changelogFile); }
  else
  { QFile file( changelogFile );
    if ( !file.open( IO_ReadOnly ) ) 
      { if (G_pCApp->m_Verbose) qWarning("Compta::about : Impossible de lire le fichier " + changelogFile); 
      }
    else
    { CGestIni::Param_UpdateFromDisk(changelogFile, tmp);
      pA_Propos->textTodos->setText(tmp);
    } // Fin if file open
   } // Fin if file exists


  // Message d'avertissement
  msg  = TR("<p align=center><b>AVERTISSEMENT</b></p><br /><br />\n");
  msg += TR("<p><span style=\"color:#aa0000\"><b>Ce logiciel <u>ne répond actuellement pas</u> aux exigences d'une comptabilité informatisée comme définie par le Ministère des finances.<br /><br />\nVous utilisez ce logiciel à vos risques. Ce logiciel est mis à disposition SANS garantie. En aucun cas, l'auteur de ce logiciel ne peut être tenu responsable de quelconques répercussions liées à l'utilisation de ce logiciel.<br /><br />Pour votre information, le bulletin officiel des impôts 13-L1-06 numéro 12 du 24 janvier 2006 décrivant les obligations en terme de comptabilité informatisée est joint à l'archive.<br /></b></span>  ");
  pA_Propos->textAvertissement->setText(msg);


  changelogFile = G_pCApp->m_PathAppli + QDir::separator() + "Fonctionnalites.html";
  //....................... ouvrir le fichier en mode QTextStream ...........................
  if ( !QFile::exists( changelogFile ) )   
   { if (G_pCApp->m_Verbose) qWarning("Compta::about : Impossible d'ouvrir le fichier " + changelogFile); }
  else
   {   CGestIni::Param_UpdateFromDisk(changelogFile, tmp);
       pA_Propos->textFonctionnalites->setText(tmp);    
   } // Fin if file exists

  QImage img;
  img.loadFromData( icone, sizeof( icone ), "PNG" );
  QLabel* lblPix = new QLabel(wgt);
  lblPix->setPixmap( img );

  QPushButton* butOk = new QPushButton(wgt);
  butOk->setText( TR("Valider") );
  connect (butOk, SIGNAL(clicked()), wgt, SLOT(accept()));

  QGridLayout *gridAbout = new QGridLayout( wgt, 5, 5, 15, 15, "LayoutCMDI_Honoraires"); 
  gridAbout->addWidget(lblPix, 0, 0);
  gridAbout->addWidget(pA_Propos, 0, 1);
  gridAbout->addMultiCellWidget(butOk, 3, 3, 0, 1);
  wgt->exec();
  delete pA_Propos;
}


void Compta::emitComptesChange()
{ emit ( cptChange() ); }

void Compta::emitDepotsChange()
{ emit ( depChange() ); }

void Compta::emitHonoChange()
{ emit ( honChange() ); }

void Compta::emitMouvementsChange()
{ emit ( mvtChange() ); }


/*! \brief Maximise le widget dans le Workspace principal de l'application.
 *  \param pQWidget : Widget à maximiser.
 */
void GlobalActiverWidget(QWidget *pQWidget)
{pQWidget->showMaximized();
}


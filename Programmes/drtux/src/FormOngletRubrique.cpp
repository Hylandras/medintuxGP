/********************************* FormOngletRubrique.cpp ************************
 *                                                                                *
 *   #include "FormOngletRubrique.h"                                              *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat à l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Fédération, 75752 PARIS cedex 15.   *
 *                            FRANCE                                              *
 *   Centre National de la Recherche Scientifique                                 *
 *   - CNRS,                                                                      *
 *                            3 rue Michel-Ange, 75794 Paris cedex 16.            *
 *                            FRANCE                                              *
 *   Institut National de Recherche en Informatique et en Automatique             *
 *   - INRIA,                                                                     *
 *                            Domaine de Voluceau, Rocquencourt, BP 105, 78153    *
 *                            FRANCE                                              *
 *                                                                                *
 *         This program is distributed in the hope that it will be useful,        *
 *         but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                   *
 *         CeCILL  License (GPL compatible) for more details.                     *
 *                                                                                *
 **********************************************************************************/

#include "FormOngletRubrique.h"

#include <qvariant.h>
#include <qtabwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qtimer.h>
#include <qiconset.h>
#include <qpixmap.h>

//--------------------------- FormOngletRubrique ---------------------------------------------------------------------
/*! \brief Constructs a FormOngletRubrique as a child of 'parent', with the name 'name' and widget flags set to 'f'. */
FormOngletRubrique::FormOngletRubrique( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{   m_OngletToSet    = -1;
    m_OngletToRemove = -1;
    if ( !name )	setName( "FormTabRubrique" );
    setMinimumSize( QSize( 0, 20 ) );
    tabWidget = new QTabWidget( this, "tabWidget" );
    tabWidget->setGeometry( QRect( 10, 0, 1110, 20 ) );
    tabWidget->setMaximumSize( QSize( 32767, 20 ) );
    tabWidget->setTabShape( QTabWidget::Triangular );
    languageChange();
    resize( QSize(1053, 22).expandedTo(minimumSizeHint()) );
    init();
}

//--------------------------- ~FormTabRubrique -----------------------------------------------------------------------
/*!\brief Destructeur de la classe */
FormOngletRubrique::~FormOngletRubrique()
{  // no need to delete child widgets, Qt does it all for us
}

//--------------------------- ~languageChange ------------------------------------------------------------------------
/*! \brief Sets the strings of the subwidgets using the current language. */
void FormOngletRubrique::languageChange()
{//setCaption( tr( "FormOngletRFormTabRubriqueubrique" ) );
}

//--------------------------- init -----------------------------------------------------------------------------------
void FormOngletRubrique::init()
{
}

//--------------------------- addPanel -------------------------------------------------------------------------------
/*! \brief ajoute un onglet nommé \em name. */
void FormOngletRubrique::addPanel( const char *name  , const QPixmap &pixmap)
{QWidget* pQwTab   = new QWidget( tabWidget, name );
    QFont f = pQwTab->font();
    f.setBold( TRUE );
    f.setPointSize( 9 );
    pQwTab->setFont( f );
 tabWidget->addTab (pQwTab, name );
 tabWidget->setTabIconSet ( pQwTab, QIconSet (pixmap) );
    f = tabWidget->font();
    f.setBold( TRUE );
    f.setPointSize( 9 );
    tabWidget->setFont( f );
}

//---------------------------------------------- OnRubriqueActived ------------------------------------------------------------
/*!\brief Réponse au signal:  SIGNAL( windowActivated ( QWidget *  )) provenant du QWorkspace  gestionnaire des fenetres MDI. Ce signal est emis par QWorkspace lorsqu'une fenetre est activée. en reponse nous activons et positionnons l'onglet des rubrique sur la bonne position.
* NOTE: il faut passer par un timer car sinon plantage si appel direct de: m_pFormOngletRubrique->tabWidget->setCurrentPage (i) lors passage de minimised vers autre forme. peut etre lié à une reference circulaire ???
* \param QWidget * pQwdgRub;  fenetre CMDI qui vient d'être activée.
*/
void FormOngletRubrique::OnRubriqueActived(QWidget * pQwdgRub)
{//return;
 if (pQwdgRub==0)          return;
 if (pQwdgRub->name()==0)  return;
 QString nom      =  pQwdgRub->name();  // pWdgRub->name() = "CMDI_Observation" "CMDI_Ident"
 int      nb      =  tabWidget->count();
 QString tablabel =  "";
 for (int i = 0; i < nb; i++)
     {QWidget     *pWdg = tabWidget->page(i);
      if (pWdg)
         {tablabel   = pWdg->name();
          if (pWdg  != tabWidget->currentPage () && tablabel==nom)
             {//................... On est obligé de passer par un timer car ....................................
              //                    si on appelle directement:
              //                    m_pFormOngletRubrique->tabWidget->setCurrentPage (i); cela plante
              //                    lors d'un passage minimised vers autre forme.
              m_OngletToSet = i;                                         // noter l'onglet à activer
              QTimer::singleShot(0, this, SLOT(TimedSetOnglet()) );      // lancer un timer sur la fonction d'activation
             }
         }
     } //endfor (int i = 0; i < nb; i++)
// statusBar()->message(tablabel, 50000 );
}

//---------------------------------------------- TimedSetOnglet ------------------------------------------------------------
/*! \brief Fonction appelée par la fonction ci-dessus par l'intermediaire d'un Timer (seule solution que j'ai trouvée pour éviter un bug).
* \param  * implicite une variable globale de DrTux: m_OngletToSet qui est l'onglet à activer*/
void FormOngletRubrique::TimedSetOnglet()
{if (m_OngletToSet!=-1) tabWidget->setCurrentPage (m_OngletToSet);
 m_OngletToSet = -1;
}

//--------------------------- removePage ----------------------------------------------------------------------------------

void FormOngletRubrique::removePage( int i )
{tabWidget->removePage( tabWidget->page (i));
}

//--------------------------- removePage ----------------------------------------------------------------------------------

void FormOngletRubrique::removePage( const char* name )
{int      nb      =  tabWidget->count();
 QString tablabel =  "";
 for (int i = 0; i < nb; i++)
     {QWidget     *pWdg = tabWidget->page(i);
      if (pWdg)
	 {tablabel   = pWdg->name();
          if (tablabel==name)
	     {//................... On est obligé de passer par un timer car ....................................
	      //                    si on appelle directement:
	      //                    m_pFormOngletRubrique->tabWidget->setCurrentPage (i); cela plante
	      //                    lors d'un passage minimised vers autre forme.
	      m_OngletToRemove = i;                                      // noter l'onglet à detruire
	      QTimer::singleShot(0, this, SLOT(TimedRemoveOnglet()) );	 // lancer un timer sur la fonction d'activation
	     }
         }
     } //endfor (int i = 0; i < nb; i++)
}

//---------------------------------------------- TimedRemoveOnglet ------------------------------------------------------------
/*! \brief Fonction appelée par la fonction ci-dessus par l'intermediaire d'un Timer (seule solution que j'ai trouvée pour éviter un bug).
* \param * implicite une variable globale de DrTux: m_OngletToSet qui est l'onglet à activer
*/
void FormOngletRubrique::TimedRemoveOnglet()
{if (m_OngletToRemove!=-1)
    {QWidget * pQWdg = tabWidget->page(m_OngletToRemove);
     tabWidget->removePage (pQWdg);
     delete pQWdg;
    }
 m_OngletToRemove = -1;
}

//--------------------------- removeAllPage ---------------------------------------
/*! \brief Efface toutes les onglets. */
void FormOngletRubrique::removeAllPage()
{
 int      nb = tabWidget->count();
 QPtrList<QWidget> list;
 //list.setAutoDelete ( TRUE );
 QWidget *pQWidget;
 for (int i  = 0; i < nb; i++)
     {if ( (pQWidget = tabWidget->page(i)) ) list.append(pQWidget);
     } //endfor (int i = 0; i < nb; i++)
 for ( pQWidget = list.first(); pQWidget; pQWidget = list.next() )
     {tabWidget->removePage( pQWidget);
     }
}



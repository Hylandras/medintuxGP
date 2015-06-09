/***************************************************************************
 *   Copyright (C) 2003 by Sevin Roland                                    *
 *   upsis@club-internet.fr                                                *
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

#include "FormOngletRubrique.h"

#include <qvariant.h>
#include <qtabwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qtimer.h>
#include <qapplication.h>
#include <qeventloop.h>
//--------------------------- FormOngletRubrique ---------------------------------------------------------------------
/*
 *  Constructs a FormOngletRubrique as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
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
/*
 *  Destroys the object and frees any allocated resources
 */
FormOngletRubrique::~FormOngletRubrique()
{
    // no need to delete child widgets, Qt does it all for us
}

//--------------------------- ~languageChange ------------------------------------------------------------------------
/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void FormOngletRubrique::languageChange()
{
    setCaption( tr( "FormOngletRFormTabRubriqueubrique" ) );
    //tabWidget->changeTab( tab, tr( "Tab 1" ) );
    //tabWidget->changeTab( tab_2, tr( "Tab 2" ) );
}

//--------------------------- init -----------------------------------------------------------------------------------
void FormOngletRubrique::init()
{//removePage(0);    //enlever les deux premieres tabulations inutiles
 //removePage(1);    //enlever les deux premieres tabulations inutiles
 //m_firstAdd = 0;
}

//--------------------------- addPanel -------------------------------------------------------------------------------
void FormOngletRubrique::addPanel( const char *name )
{QWidget* pQwTab   = new QWidget( tabWidget, name );
 tabWidget->addTab (pQwTab, name );

 //........... pb = on ne peut enlever le 1er paneau ...........................
 //                 que si il en reste au moins 1
 //if (m_firstAdd==0)
 //   {removePage(0);
 //    m_firstAdd = 1;
 //   }
}


//---------------------------------------------- OnRubriqueActived ------------------------------------------------------------
// ACTION: reponse au signal:  SIGNAL( windowActivated ( QWidget *  )) provenant du QWorkspace  gestionnaire des fenetres
//         MDI. Ce signal est emis par QWorkspace lorsqu'une fenetre est activée. en reponse nous activons et positionnons
//         l'onglet des rubrique sur la bonne position.
// NOTE: il faut passer par un timer car sinon plantage si appel direct de: m_pFormOngletRubrique->tabWidget->setCurrentPage (i)
//       lors passge de minimised vers autre forme. peut etre lié à une reference circulaire ???
// ENTREE: QWidget * pQwdgRub;  fenetre CMDI qui vient d'être activée.

void FormOngletRubrique::OnRubriqueActived(QWidget * pQwdgRub)
{//return;
 if (pQwdgRub==0)          return;
 if (pQwdgRub->name()==0)  return;
 QString nom      =  pQwdgRub->name();  // pWdgRub->name() = "CMDI_Observation" "CMDI_Ident"
 nom              =  nom.mid (5);       // "CMDI_Observation" --> "Observation"
 nom              =  nom.left(5);       // "Observation"      --> "Obser"
 int      nb      =  tabWidget->count();
 //int cur_index    =  tabWidget->currentPageIndex ();
 QString tablabel =  "";
 for (int i = 0; i < nb; i++)
     {QWidget     *pWdg = tabWidget->page(i);
      if (pWdg)
	 {tablabel   = pWdg->name();
          tablabel   = tablabel.left(5);                         // QString QTabWidget::tabLabel ( QWidget * w ) const
          if (pWdg  != tabWidget->currentPage () && tablabel==nom)
	     {//................... On est obligé de passer par un timer car ....................................
	      //                    si on appelle directement:
	      //                    m_pFormOngletRubrique->tabWidget->setCurrentPage (i); cela plante
	      //                    lors d'un passage minimised vers autre forme.
	      m_OngletToSet = i;                                         // noter l'onglet à activer
	      QTimer::singleShot(0, this, SLOT(TimedSetOnglet()) );	 // lancer un timer sur la fonction d'activation
	     }
         }
     } //endfor (int i = 0; i < nb; i++)
// statusBar()->message(tablabel, 50000 );
}

//---------------------------------------------- TimedSetOnglet ------------------------------------------------------------
// ACTION: fonction appelée par la fonction ci-dessus par l'intermediaire d'un Timer (seule solution que j'ai trouvée pour
//         eviter un bug).
// ENTREE: implicite une variable globale de DrTux: m_OngletToSet qui est l'onglet à activer

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
{QString nom      =  name;              // pWdgRub->name() = "CMDI_Observation" "CMDI_Ident"
 nom              =  nom.left(5);       // "Observation"      --> "Obser"
 int      nb      =  tabWidget->count();
 QString tablabel =  "";
 for (int i = 0; i < nb; i++)
     {QWidget     *pWdg = tabWidget->page(i);
      if (pWdg)
	 {tablabel   = pWdg->name();
          tablabel   = tablabel.left(5);                         // QString QTabWidget::tabLabel ( QWidget * w ) const
          if (tablabel==nom)
	     {//................... sous Linux on est obligé de passer par un timer car ....................................
	      //                    si on appelle directement:
	      //                    m_pFormOngletRubrique->tabWidget->setCurrentPage (i); cela plante
	      //                    lors d'un passage minimised vers autre forme.
          #if defined(Q_WS_WIN)
              QWidget * pQWdg = tabWidget->page(i);
              tabWidget->removePage (pQWdg);
              delete pQWdg;
          #else 
	          m_OngletToRemove = i;                                      // noter l'onglet à detruire
	          QTimer::singleShot(0, this, SLOT(TimedRemoveOnglet()) );	 // lancer un timer sur la fonction d'activation
              qApp->eventLoop()->processEvents ( QEventLoop::WaitForMore );
              qApp->eventLoop()->processEvents ( QEventLoop::WaitForMore );
          #endif
	     }
         }
     } //endfor (int i = 0; i < nb; i++)
}

//---------------------------------------------- TimedRemoveOnglet ------------------------------------------------------------
// ACTION: fonction appelée par la fonction ci-dessus par l'intermediaire d'un Timer (seule solution que j'ai trouvée pour
//         eviter un bug).
// ENTREE: implicite une variable globale de DrTux: m_OngletToSet qui est l'onglet à activer

void FormOngletRubrique::TimedRemoveOnglet()
{if (m_OngletToRemove!=-1)
    {QWidget * pQWdg = tabWidget->page(m_OngletToRemove);
     tabWidget->removePage (pQWdg);
     delete pQWdg;
    }
 m_OngletToRemove = -1;
}



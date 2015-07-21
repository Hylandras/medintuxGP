/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
 
//--------------------------- init ---------------------------------------------
void FormTabRubrique::init()
{removePage(0);    //enlever les deux premieres tabulations inutiles
 m_firstAdd = 0;
}

//--------------------------- addPanel ----------------------------------------- 
void FormTabRubrique::addPanel( const char *name , QPixmap &pixmap)
{QWidget* pQwTab   = new QWidget( tabWidget, name );
 tabWidget->addTab (pQwTab, name );
 tabWidget->setTabIconSet ( pQwTab, QIconSet (pixmap) ); 
 //........... pb = on ne peut enlever le 1er paneau ...........................
 //                 que si il en reste au moins 1
 if (m_firstAdd==0)
    {removePage(0);
     m_firstAdd = 1;
    }
}

//--------------------------- OnRubriqueActived  -------------------------------
void FormTabRubrique::OnRubriqueActived( QWidget *pWdgRub   )
{
 
 if (pWdgRub==0)          return;
 if (pWdgRub->name()==0)  return;
 QString nom = pWdgRub->name();  // pWdgRub->name() = "CMDI_Observation" "CMDI_Ident"
 int      nb = tabWidget->count();
 nom         = nom.mid (5);     // "CMDI_Observation" --> "Observation"
 nom         = nom.left(5);     // "Observation"      --> "Obser"
 //emit message(nom, 50000 );
 for (int i = 0; i < nb; i++)
     {QWidget     *pWdg = tabWidget->page(i);
      if (pWdg)
	 {QString name_page = pWdg->name();
          name_page  = name_page.left(5);          // QString QTabWidget::tabLabel ( QWidget * w ) const 
          if (pWdg  != tabWidget->currentPage () && name_page==nom)
	     {//tabWidget->showPage (pWdg);
              //tabWidget->setCurrentPage (i);
	     }           
         }            
     } //endfor (int i = 0; i < nb; i++)
}

//--------------------------- removePage --------------------------------------- 
void FormTabRubrique::removePage( int i )
{tabWidget->removePage( tabWidget->page (i));
}


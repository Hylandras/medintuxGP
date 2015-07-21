/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

//=============================================== DEFINE ============================================================

//.................. index des champs .......................................
#define LIST_NOM 	0
#define LIST_NOMJF	1
#define LIST_PRENOM 	2
#define LIST_RUE	3
#define LIST_CP		4
#define LIST_VILLE	5
#define LIST_TEL1	6
#define LIST_TEL2	7
#define LIST_ID_KEY 	8
#define LIST_GUID 	9

#define F_GUID         0

#include "CApp.h"
#include "CMDI_ChoixPatient.h"
//=============================================== PROTOTYPRS ========================================================

//=============================================== IMPLEMENTTION =====================================================

//--------------------------------- Init -------------------------------------------------------------
// ACTION: cette fonction est appelée à la fin du constructeur
void FormRubChoixPatient::init()
{checkBoxAutre->setChecked(FALSE);
 m_LastGUID = "";
 SetInterfaceOnDroits(G_pCApp->m_Droits);
}

//--------------------------------- SetInterfaceOnDroits ---------------------------------------------------------------------------
void FormRubChoixPatient::SetInterfaceOnDroits(const QString &droits)
{if (G_pCApp->IsThisDroitExist( droits, G_pCApp->m_mapNameRubInfos[CMDI_ChoixPatient::S_GetRubName()]+"c") &&
     G_pCApp->IsThisDroitExist( droits, "idc"))   pushButtonNew->show();
 else                                             pushButtonNew->hide();
}

//--------------------------------- InitAllList ---------------------------------------------------------------------------
void FormRubChoixPatient::InitAllList( const QString & qstr_nom, const QString & qstr_prenom)
{
  if (m_pCMoteurBase==0)
     {textLabel_StateList->setText(tr("InitAllList(): m_pCMoteurBase non initialisée"));
      return;
     }
  /*
  if (m_pCMoteurBase->m_DataBase==0)
     {textLabel_StateList->setText(tr("InitAllList(): m_pCMoteurBase->m_DataBase non initialisée"));
      return;
     }
  */
  m_pCMoteurBase->GetPatientList(ListView_Patient,
			         qstr_nom,
				 qstr_prenom,
				 textLabel_StateList
                                );
 QString val1;
 if (READ_USER_PARAM(USER_PARAM, tr("Choix Patients"), tr("Exécution"), &val1)==0)  // zero = pas d'erreur
    {if (val1==QString("Autre instance"))checkBoxAutre->setChecked(TRUE);
    }
}

//--------------------------------- reinitList ------------------------------------------------------------
/*! \brief Recherche dans la liste l'item correspondant à id_doss, le sélectionne et l'affiche.
 *  Les données recherchées sont indépendantes des données affichées.
*/void FormRubChoixPatient::reinitList(const char* /*num_GUID*/,  const char* id_doss,
                                       const char* /*dossNom */,  const char* /*dossPrenom*/)
{lineEditAutoLocator_textChanged( lineEditAutoLocator->text());
 if (*id_doss)
 { CPrtQListViewItem* pItem = 0;
   QListViewItemIterator it( ListView_Patient , QListViewItemIterator::Visible);
   while ( it.current() )
   { pItem  =  (CPrtQListViewItem*) it.current();
     if (pItem->getPkDoc() == id_doss)
       { ListView_Patient->setCurrentItem ( pItem );
         ListView_Patient->setSelected ( pItem, TRUE );
	 break;
       }
     ++it;
   }  // Fin while
 } // Fin if id_doss != ""
}

void FormRubChoixPatient::Slot_SearchFor(QString patient)
{ lineEditAutoLocator->setText( patient );
  lineEditAutoLocator->releaseKeyboard();
}

//--------------------------------- lineEditAutoLocator_textChanged -------------------------------------
/*! \brief lorsque l'utilisateur saisi un caractère dans le champs recherche -> réinitialise la listView
*/
void FormRubChoixPatient::lineEditAutoLocator_textChanged( const QString &qstr_in )
{QString nom;
 QString prenom;
 //.................. rechercher le separateur de nom ; prenom .......................
 int f = qstr_in.findRev( ';' );
 //................... si trouvé separer les deux chaines  ...........................
 if (f!=-1)
    {nom        = qstr_in.left(f);
     prenom     = qstr_in.right(qstr_in.length()-f-1);
    }
 else
    {nom        = qstr_in;
     prenom     = "";
    }
 InitAllList( nom, prenom);
}

//--------------------------------- ListView_Patient_clicked -----------------------------------------------
/*! \brief Slot appelé lorsque l'utilisateur sélectionne un patient.
 *  \sa DrTux::
*/
void FormRubChoixPatient::ListView_Patient_clicked( QListViewItem *qlistViewItem)
{if (qlistViewItem<=0) return;
 CPrtQListViewItem* pItem = (CPrtQListViewItem*)qlistViewItem;

 if (checkBoxAutre->isChecked())
    { emit Sign_DrTuxExeAnother(pItem->getGUID(), pItem->getPkDoc(), pItem->getNom(), pItem->getPrenom());
    }

 //OnDrTuxIdentChange(const char* num_GUID,  const char* id_doss, const char* dossNom ,  const char* dossPrenom )
 // disconnect( ListView_Patient, SIGNAL( selectionChanged( QListViewItem * )),
 //            0,               0 );
 //.......................... faire part au Dr Tux du changement d'identité .....................
 //emit  Sign_IdentChange(qlistViewItem->text(LIST_GUID),   qlistViewItem->text(LIST_ID_KEY),
 //                       qlistViewItem->text(LIST_NOM),    qlistViewItem->text(LIST_PRENOM)
 //                      );
 // connect ( ListView_Patient, SIGNAL (selectionChanged( QListViewItem * )),
 //          this,             SLOT   (ListView_Patient_selectionChanged( QListViewItem * ))
 //        );
}

//--------------------------------- ListView_Patient_selectionChanged -------------------------------------
/*! \brief lorsque l'utilisateur change l'item sélectionné par le biais du clavier ou de la souris, recherche le patient et affiche ses données.
*/
void FormRubChoixPatient::ListView_Patient_selectionChanged( QListViewItem *qlistViewItem )
{if (qlistViewItem<=0) return;
 ListView_Patient->disconnect( SIGNAL(selectionChanged(QListViewItem *)) );
 CPrtQListViewItem* pItem = (CPrtQListViewItem*) qlistViewItem;
 if (pItem==0) return;
//qWarning(pItem->getNom() +pItem->getPrenom() + pItem->getGUID() + pItem->getPkDoc());

 if (checkBoxAutre->isChecked()==FALSE)
    {//.......................... faire part au Dr Tux du changement d'identité ...........................
     emit  Sign_IdentChange(pItem->getGUID(), pItem->getPkDoc(), pItem->getNom(), pItem->getPrenom() );
      //..................... verifier si DrTux accepte .................................................
      QString activeGUID;
      emit Sign_GetActiveGUID(activeGUID);
      if ( pItem->getGUID() != activeGUID )
         {m_LastGUID = activeGUID;
          ListView_Patient->disconnect( SIGNAL(selectionChanged(QListViewItem *)) );
          QTimer::singleShot( 200, this, SLOT(RestaureLastGUID()) );
         }
    }

 connect( ListView_Patient,         SIGNAL( selectionChanged(QListViewItem *) ),
          this,                     SLOT(ListView_Patient_selectionChanged (QListViewItem *))
        );
}

//--------------------------------- RestaureLastGUID -----------------------------------------------------------
/*! \brief non documentée
 *  \todo A TESTER
*/
void FormRubChoixPatient::RestaureLastGUID()
{  CPrtQListViewItem* pItem = 0;
   QListViewItemIterator it( ListView_Patient , QListViewItemIterator::Visible);
   bool trouve = FALSE;
   while ( it.current() )
   { pItem  =  (CPrtQListViewItem*) it.current();
     if (pItem->getGUID() == m_LastGUID)
       { ListView_Patient->setCurrentItem ( pItem );
         ListView_Patient->setSelected ( pItem, TRUE );
         ListView_Patient->ensureItemVisible ( pItem );
	 trouve = TRUE;
	 break;
       }
     ++it;
   }  // Fin while

  if (!trouve)
    {pItem = (CPrtQListViewItem*) ListView_Patient->currentItem ();
     if (pItem)
        {ListView_Patient->setSelected ( pItem , FALSE );
        }
    }

 connect( ListView_Patient,         SIGNAL( selectionChanged(QListViewItem *) ),
          this,                     SLOT( ListView_Patient_selectionChanged (QListViewItem *))
        );
 m_LastGUID = "";
}

//--------------------------------- pushButtonNew_clicked -------------------------------------
void FormRubChoixPatient::pushButtonNew_clicked()
{emit Sign_CreateNewIdent();
}

//--------------------------------- pushButtonDelete_clicked ---------------------------------------
void FormRubChoixPatient::pushButtonDelete_clicked()
{//CPrtQListViewItem *pItem = (CPrtQListViewItem*)ListView_Patient->currentItem ();
 //if (pItem==0) return;
 //emit  Sign_IdentDelete(pItem->getGUID(),  pItem->getPkDoc(),
 //                       pItem->getNom(),  pItem->getPrenom()
 //                      );
 //lineEditAutoLocator_textChanged( lineEditAutoLocator->text());
 }


//--------------------------------- checkBoxAutre_clicked ---------------------------------------
void FormRubChoixPatient::checkBoxAutre_clicked()
{if (checkBoxAutre->isChecked()==TRUE)
    {CGestIni::Param_WriteParam(&USER_PARAM, "Choix Patients", tr("Exécution"), tr("Autre instance") );  // zero = pas d'erreur
    }
 else
    {CGestIni::Param_WriteParam(&USER_PARAM, "Choix Patients", tr("Exécution"), tr("Même instance") );
    }
  m_pCMoteurBase->Param_SavParam(&USER_PARAM, m_User);
}


//--------------------------------- pushButtonInfo_clicked -----------------------------------
void FormRubChoixPatient::pushButtonInfo_clicked()
{QWhatsThis::display ( tr("Pour chercher par le nom et le prénom, il faut séparer le prénom du nom par un point-virgule. Exemple : <b>MARTIN;JEAN</b><br><br>"
			  "Le Symbole pour cent % placé avant ou après le nom ou prénom permet de faire une recherche sur tout ce qui contient l'expression<br>"
			  "Exemple : <b>%MART;%JE</b>cherchera toutes les personnes dont le nom contient <b>MART</b> et dont le prénom contient <b>JE</b>" ));

}

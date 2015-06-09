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


// QWidget utilisé pour filtrer les honoraires
// Affiché dans la fenêtre : Hono_View, ...........
// Emet un signal sign_Filtre_Changed( const char* ) avec le filtre en paramètre

void Filtre_Hono::init()
{ m_pComptaMoteurBase = 0;
  typesActes = new QStringList();
  m_pActeListe = new ACTES_LISTE;
}

void Filtre_Hono::destroy()
{ if (m_pActeListe)		delete m_pActeListe;
  if (typesActes)		delete typesActes;

}


void Filtre_Hono::setParams(ComptaMoteurBase* pComptaMoteurBase)
{ m_pComptaMoteurBase = pComptaMoteurBase;
  QDate d = QDate::currentDate();
  // Mettre les dates pour la période à jour
  periode1->setDate( d );
  periode2->setDate( d );
  // Mettre à jour les combox mois et année
  aCBox->setCurrentText( d.toString("yyyy"));
  mCBox->setCurrentItem( d.month() - 1 );
  // Récupérer les types d'actes disponibles dans la base
  slot_ActesListeModif();
  combo_mois_annee_activated( 1 );
}

/*! \fn void Filtre_Hono::slot_ActesListeModif()
 *  \brief SLOT appelé par la CMDI_Acte lorsque la liste des actes a été modifiée. Met à jour le combo des types d'actes
 */
void Filtre_Hono::slot_ActesListeModif()
{ // Récupère la liste des actes disponibes
  if (!m_pComptaMoteurBase->getActesForCombobox( *m_pActeListe ) )
  { qWarning("Filtre_Hono::slot_ActesListeModif :: Impossible de récupérer la liste des actes disponibles.");
    return ;
  }
  findTypes();
  cmbTypeActe->clear();  
  cmbTypeActe->insertStringList( *typesActes );
}

/*! \fn void Filtre_Hono::findTypes()
    \brief Recherche dans la liste des actes disponibles les différents types d'actes et sauvegarde le résultat dans la vairable protected \em typesActes . Met à jour le combo des types d'actes.
*/
void Filtre_Hono::findTypes()
{ QString tmp = "";
  typesActes->clear();
  ACTES_LISTE::iterator it;
  for (it = m_pActeListe->begin(); it !=  m_pActeListe->end(); ++it )
   { tmp = (*it).getType();
     if (typesActes->isEmpty() )
	{ typesActes->append( tmp );  }
     else
      {	// Vérifier que le type n'est déjà dans la liste
	if ( typesActes->contains( tmp ) == 0 )
	 { typesActes->append ( tmp );  }
       }
   }
  tmp = TR("Type d'actes");
  typesActes->append( tmp );
}



void Filtre_Hono::combo_mois_annee_activated( int )
{ QString dtStr = "";
  QString tmp = "";
  if (mCBox->currentItem() < 9) tmp = "0" + QString::number(mCBox->currentItem() + 1);
  else				tmp = QString::number(mCBox->currentItem() + 1);
  dtStr = aCBox->currentText() + "-" + tmp + "-01";
  m_pComptaMoteurBase->m_Filtre_Date_Debut = QDate::fromString(dtStr, Qt::ISODate);
  m_pComptaMoteurBase->m_Filtre_Date_Fin   = m_pComptaMoteurBase->m_Filtre_Date_Debut.addMonths(1);
  m_pComptaMoteurBase->m_Filtre_Date_Fin   = m_pComptaMoteurBase->m_Filtre_Date_Fin.addDays(-1);
  m_pComptaMoteurBase->m_Filtre_Change = TRUE;
  // Change le pre_CBox et les dates des périodes
  pre_CBox->setCurrentItem( pre_CBox->count() - 1 );
  //periode1->setDate( m_pComptaMoteurBase->m_Filtre_Date_Debut );
  //periode2->setDate( m_pComptaMoteurBase->m_Filtre_Date_Fin );
  // Informe les fenêtres CMDI que le filtre des honoraires est modifié
  emit sign_Filtre_Changed( );
}


void Filtre_Hono::pre_CBox_activated( int id )
{ QString filter;
  // Construit le filtre préprogrammé en fonction du combo "Pré-enregistré "
  switch (id)
  {  case 0 :
	{	// du Jour
		m_pComptaMoteurBase->m_Filtre_Date_Fin   = QDate::currentDate();
		m_pComptaMoteurBase->m_Filtre_Date_Debut = QDate::currentDate();
		m_pComptaMoteurBase->m_Filtre_Change     = TRUE;
		break;
	}
      case 1 :
	{	// de la veille
		m_pComptaMoteurBase->m_Filtre_Date_Fin   = QDate::currentDate();
		m_pComptaMoteurBase->m_Filtre_Date_Debut = m_pComptaMoteurBase->m_Filtre_Date_Fin.addDays(-1);
		m_pComptaMoteurBase->m_Filtre_Date_Fin = m_pComptaMoteurBase->m_Filtre_Date_Debut;
		m_pComptaMoteurBase->m_Filtre_Change     = TRUE;
		break;
	}
      case 2 :
	{	// de la semaine du lundi au dimanche
		m_pComptaMoteurBase->m_Filtre_Date_Fin    = QDate::currentDate();
		int dayWeek 		      = m_pComptaMoteurBase->m_Filtre_Date_Fin.dayOfWeek();
		m_pComptaMoteurBase->m_Filtre_Date_Debut  = m_pComptaMoteurBase->m_Filtre_Date_Fin.addDays(1-dayWeek);
		m_pComptaMoteurBase->m_Filtre_Change      = TRUE;
		break;
	}
      case 3 :
	{	// Année entière
		QString dt_start = aCBox->currentText() + "-01-01";
		QString dt_end   = aCBox->currentText() + "-12-31";
		m_pComptaMoteurBase->m_Filtre_Date_Fin    = QDate::fromString(dt_end  , Qt::ISODate);
		m_pComptaMoteurBase->m_Filtre_Date_Debut  = QDate::fromString(dt_start, Qt::ISODate);
		m_pComptaMoteurBase->m_Filtre_Change      = TRUE;
		break;
	}

   }

  // Informe les fenêtres CMDI que le filtre est modifié
  emit sign_Filtre_Changed( );
}



void Filtre_Hono::periode1_valueChanged( const QDate& )
{ QString filter = "";
  m_pComptaMoteurBase->m_Filtre_Date_Debut   = periode1->date();
  m_pComptaMoteurBase->m_Filtre_Date_Fin     = periode2->date();
  m_pComptaMoteurBase->m_Filtre_Change       = TRUE;
  // Informe les fenêtres CMDI que le filtre est modifié
  emit sign_Filtre_Changed( );
}



void Filtre_Hono::cmbTypeHono_activated( int )
{ if (m_pComptaMoteurBase->m_Filtre_Change) 
  { m_pComptaMoteurBase->m_Filtre_Type_Paiement = cmbTypeHono->currentText();
    emit sign_Filtre_Changed( );
  }
}


void Filtre_Hono::cmbTypeHono2_activated( int id )
{ if (m_pComptaMoteurBase->m_Filtre_Change) 
  {m_pComptaMoteurBase->m_Filtre_Autre   = "";
   switch (id)
    {case 0 : // tous
	{ m_pComptaMoteurBase->m_Filtre_Autre   = "";
	  break;
	}
      case 1 : // encaissés
	{ m_pComptaMoteurBase->m_Filtre_Autre   = " \nAND ( remarque LIKE \'%";
	  m_pComptaMoteurBase->m_Filtre_Autre   += CHEQUE_DEPOSE;
	  m_pComptaMoteurBase->m_Filtre_Autre   +=  "%\'";
	  m_pComptaMoteurBase->m_Filtre_Autre   += " \nOR remarque LIKE \'%";
	  m_pComptaMoteurBase->m_Filtre_Autre   += DAF_OK;
	  m_pComptaMoteurBase->m_Filtre_Autre   +=  "%\' )";
	  m_pComptaMoteurBase->m_Filtre_Autre   += " \nOR remarque LIKE \'%";
	  m_pComptaMoteurBase->m_Filtre_Autre   += CMU_OK;
	  m_pComptaMoteurBase->m_Filtre_Autre   +=  "%\' )";
	  break;
	}
      case 2 : // non encaissés
	{ m_pComptaMoteurBase->m_Filtre_Autre   = " \nAND ( remarque LIKE \'%";
	  m_pComptaMoteurBase->m_Filtre_Autre   += CHEQUE_PAS_DEPOSE;
	  m_pComptaMoteurBase->m_Filtre_Autre   +=  "%\'";
	  m_pComptaMoteurBase->m_Filtre_Autre   += " \nOR remarque LIKE \'%";
	  m_pComptaMoteurBase->m_Filtre_Autre   += DAF_PAS_OK;
	  m_pComptaMoteurBase->m_Filtre_Autre   +=  "%\' )";
	  m_pComptaMoteurBase->m_Filtre_Autre   += " \nOR remarque LIKE \'%";
	  m_pComptaMoteurBase->m_Filtre_Autre   += CMU_PAS_OK;
	  m_pComptaMoteurBase->m_Filtre_Autre   +=  "%\' )";
	  break;
	}
      case 3 : // validés
	{ m_pComptaMoteurBase->m_Filtre_Autre   = " \nAND valide>0";
	  break;
	}
      case 4 : // non validés
	{ m_pComptaMoteurBase->m_Filtre_Autre   = " \nAND valide=0";
	  break;
	}
    }  // end switch
    emit sign_Filtre_Changed( );
//qWarning(G_pCApp->m_ComptaBase->getFiltre(""));
  } // end If
}


void Filtre_Hono::cmbTypeActe_activated( int id )
{ if ( id == (cmbTypeActe->count()-1) ) { m_pComptaMoteurBase->m_Filtre_Autre   = "";}
  else { m_pComptaMoteurBase->m_Filtre_Autre   = " \n AND `acte` LIKE \"%"+cmbTypeActe->currentText()+"%\" "; }
  m_pComptaMoteurBase->m_Filtre_Change = TRUE;
//  qWarning(G_pCApp->m_ComptaBase->getFiltre(""));
  emit sign_Filtre_Changed( );
}



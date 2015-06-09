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

void Depots_Validation::init()
{   // initialise le combo des années
   int an = QDate::currentDate().year() - 5;
   for (int i = an; i < (an+11); ++i)
    { annee->insertItem(QString::number(i));}
}


void Depots_Validation::setParams(ComptaMoteurBase* pComptaMoteurBase,
				  QDate dateDepot,
				  COMPTES_LISTE* pComptes_Liste,
				  HONO_LISTE* pHono_Liste_Depot)
{ Tools* tools = new Tools();
  // Récupère les comptes bancaires
  m_pComptes_Liste = pComptes_Liste;
  m_pComptaMoteurBase = pComptaMoteurBase;
  m_pHono_Liste_Depot = pHono_Liste_Depot;
  COMPTES_LISTE::iterator it;
  int r = 0;
  int id=0;
  for (it=m_pComptes_Liste->begin(); it != m_pComptes_Liste->end(); ++it)
  { m_Combo_Comptes->insertItem( (*it).getLibelle() ); 
    if ((*it).isDefaut()) id=r;
    r++;
  }
  m_Combo_Comptes->setCurrentItem(id);

  // Insère la date
  setDateObserv(dateDepot);

  // Insère les honoraires  
  QString msg = "";
  QStringList hd, lines; hd.clear();  lines.clear();
  hd << TR("Numéro") << TR("Nom") << TR("Date") << TR("Montant");
  HONO_LISTE::iterator itH;
  int nb=0;
  double total = 0.0;
  for (itH=m_pHono_Liste_Depot->begin(); itH!=m_pHono_Liste_Depot->end(); ++itH)
  { nb++;
    lines << QString::number(nb);
    lines << (*itH).getEmetteur();
    lines << (*itH).getDate().toString("dd-MMM-yyyy");
    lines << QString::number((*itH).getChq());
    total += (*itH).getChq();
  }
  tools->createHTMLTable(hd, lines, msg, 4,0);
  delete tools;
  textHono->setText( msg );

  // Indique total et nb de chèques
  m_LineEdit_Total->setText( QString::number(total,'f',2) );
  m_LineEdit_NbCheques->setText( QString::number(nb,'f',2) );
}


void Depots_Validation::getParams(QDate& dateEnregistree,
				  Comptes& pCompte_Selectionne,
				  bool& validationDemandee)
{ if (chkValideHonoraires->isChecked()) 
    validationDemandee = TRUE; 
  else
    validationDemandee = FALSE; 
  pCompte_Selectionne = (*m_pComptes_Liste->at( m_Combo_Comptes->currentItem() ));
}

///////////////////////////////////////////////////////////////////
//////////////////// GESTION DE LA DATE ///////////////////////////
///////////////////////////////////////////////////////////////////

/*! \fn void Depots_Validation::mois_activated( int )
 *  \brief Lorsque l'utilisateur change de mois, recalcul le nombre de jours et met à jour le combo des jours du mois.
 */
void Depots_Validation::mois_activated(int )
{  int nbJours = 0;
   int ms = mois->currentItem() + 1;
   QString toAdd = "";
   int jj = jour->currentItem();
   jour->clear();

   if ((ms == 1) ||(ms == 3) ||(ms == 5) ||(ms == 7) ||(ms == 8) ||(ms == 10) ||(ms == 12) )
	nbJours = 31;
   else if ((ms == 2))
   { // Test année bissextile
       if (annee->currentText().toInt() % 4 == 0) nbJours = 29;
       else nbJours = 28;
   }
   else nbJours = 30;

   for (int i = 0; i < nbJours; ++i)
    { toAdd = QString::number(i+1);
       if (i < 9) toAdd.prepend("0");
      jour->insertItem(toAdd);
    }

   if (jj >= jour->count()) jj = jour->count() - 1;
   jour->setCurrentItem(jj);
}


/*! \fn QString Depots_Validation::getDateObserv()
 *  \brief A tout moment, retourne la date de l'honoraire au format string : "dd-MM-yyyy"
 */
QString Depots_Validation::getDateObserv()
{  QString tmp = "";
    tmp = QString::number(mois->currentItem() + 1);
    if (mois->currentItem() < 9) tmp.prepend("0");
    return jour->currentText() + "-" + tmp + "-" + annee->currentText();
}


/*! \fn void Depots_Validation::setDateObserv(QString dt_str)
 *  \brief A tout moment, met les combobox de la date aux bonnes valeurs. La date est passée en paramètre au format "dd-MM-yyyy".
 */
void Depots_Validation::setDateObserv(QString dt_str)
{ // Passe du format "dd-MM-yyyy" au format "yyyy-MM-dd"
  QString tmp = m_pComptaMoteurBase->dateForSQL( dt_str );
  QDate dt = QDate::fromString(tmp, Qt::ISODate);
  setDateObserv(dt);
}

/*! \fn void Depots_Validation::setDateObserv(QDate dt)
 *  \brief A tout moment, met les combobox de la date aux bonnes valeurs. La date est passée en paramètre au format "dd-MM-yyyy".
 */
void Depots_Validation::setDateObserv(QDate dt)
{ int an = dt.year() - 5;
  annee->clear();
  for (int i = 0; i < 10; i++)
  { annee->insertItem(QString::number( an + i ));
  }
  an += 5;
  annee->setCurrentText(QString::number(an));
  mois->setCurrentItem(dt.month() - 1);
  mois_activated( dt.month() - 1);
  jour->setCurrentItem( dt.day() - 1);
}


/*! \fn void Depots_Validation::annee_activated( int  )
 *  \brief Si l'utilisateur change d'année et que le mois est sur février -> gérer les années bissextiles et mettre à jour le combo des jours.
 */
void Depots_Validation::annee_activated( int  )
{ if (mois->currentItem() == 1)     mois_activated( 1 );
}


void Depots_Validation::but_Valider_clicked()
{ int idCpt = (*m_pComptes_Liste->at ( m_Combo_Comptes->currentItem() ) ).getId();
  QString rib = (*m_pComptes_Liste->at ( m_Combo_Comptes->currentItem() ) ).getRibForBase();
  QDate dt = QDate::fromString( m_pComptaMoteurBase->dateForSQL(getDateObserv()), Qt::ISODate );

  if (m_pComptaMoteurBase->saveDepotToBase(&dt, m_pHono_Liste_Depot, rib , idCpt ) )
  { // Affiche une fenêtre d'information
    QMessageBox::information(this, TR("Sauvegarde de la remise de chèque."), 
	TR("La remise de chèque actuelle vient d'être sauvegardée dans la base de données.")+"\n\n"+
	TR("Nombres de chèques : ")+QString::number(m_pHono_Liste_Depot->count()) + "\n\n" + 
	TR("Montant total de la remise : ") + m_LineEdit_Total->text()+ " Euros.\n\n", 
	TR("Ok"),QString::null, QString::null, 0, 1);
  }
  else QMessageBox::information(this, TR("Erreur lors de la sauvegarde."), 
	m_pComptaMoteurBase->m_messageErreur, 
	TR("Ok"),QString::null, QString::null, 0, 1);
 accept();
}


void Depots_Validation::but_Annuler_clicked()
{ reject();

}

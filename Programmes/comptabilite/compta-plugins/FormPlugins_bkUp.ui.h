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
/*! \file
 * Contient les fonctions de la classe FormPlugins  (FormPlugins.ui et FormPlugins.ui.h)
*/
/*! \class FormPlugins
 *  \brief Fenêtre de saisie d'acte utilisée lors de l'utilisation de compta sous forme de plugins de MedinTux.
 *  \author Eric MAEKER
 *  \version 0.1

 *   La classe construit une fenêtre dialog pour la saisie des actes et honoraires à sauvegarder.

*/

#include "CApp.h"

//-------------------------------------------- init -------------------------------------------------
/*! \fn void FormPlugins::init()
 *  \brief Renseigne les champs du formulaire avec les données accessibles. Le reste des données doit être renseigné par l'utilisateur.
 *  \sa GetIni, CApp
 */
//-------------------------------------------- init -------------------------------------------------
void FormPlugins::init()
{  int id = 0;
   // Récupère les variables de G_CApp
   nomPatient->setText(G_pCApp->m_Param_Patient);
   nomPraticien->setText(G_pCApp->m_Param_Praticien);
   dateObserv->setText(G_pCApp->m_Param_QDate.toString("dd-MM-yyyy"));
   chkTiersPayant->setChecked(G_pCApp->m_Param_Tiers);

   // Charge les valeurs pour le combobox ACTE
   m_pActeListe = G_pCApp->m_ComptaBase->getActesForCombobox();
   ACTES_LISTE::iterator it;
//     if (!m_pActeListe) // Prendre les défauts
   int i = 0;
   for(it = m_pActeListe->begin(); it !=  m_pActeListe->end(); ++it )
     {	// Insère les libellés dans le combo
	libelleActe->insertItem((*it).m_Nom);
	// Et recherche en même temps l'item passé en paramètre
	if ((*it).m_Nom == G_pCApp->m_Param_Acte) {id = i;}
	++i;
     }
   // remets le combo à l'id de l'acte choisi ou à 0 
   // et met à jour les autres champs liés à cet acte (total et tiers)
   libelleActe->setCurrentItem(id);
   putActeParams(id);

  // Mode de payement passé en paramètre ?
  if (G_pCApp->m_NbParam >= 18)  // oui
   {if (G_pCApp->m_Param_ModePayement == "esp")  		    butEsp_clicked();
    else if (G_pCApp->m_Param_ModePayement == "espèces")	butEsp_clicked();
    else if (G_pCApp->m_Param_ModePayement == "chq")		butChq_clicked();
    else if (G_pCApp->m_Param_ModePayement == "cheque")		butChq_clicked();
    else if (G_pCApp->m_Param_ModePayement == "cb")		    butChq_clicked();
    else if (G_pCApp->m_Param_ModePayement == "daf")		butDAF_clicked();
    else if (G_pCApp->m_Param_ModePayement == "diff")		butDAF_clicked();
    else if (G_pCApp->m_Param_ModePayement == "différé")	butDAF_clicked();
    else if (G_pCApp->m_Param_ModePayement == "autre")		butAutre_clicked();
   }
  
}


/*! \fn void FormPlugins::putActeParams (int id )
    \brief SLOT. Insère dans les champs adéquats du plugins les variables (descriptif, total, tiers) de l'acte choisi dans le combo.
*/
void FormPlugins::putActeParams (int id )
{ ACTES_LISTE::iterator it = m_pActeListe->at(id);
  montantTiers->setText(QString::number((*it).m_Tiers));
  montantTotal->setText(QString::number((*it).m_Total));
  lblInfoActe->setText(TR("<u>Descriptif de l'acte :</u><br>")+(*it).m_Desc);
}

/*! \fn void FormPlugins::allToZero()
    \brief Met à zéro tous les champs de payement (espèce, chèque, cb, daf, autre).
*/
void FormPlugins::allToZero()
{  // Remets à zéro toutes ls valeurs financières du formulaire
    QString zero = "0";
    montantChq->setText(zero);
    montantEsp->setText(zero);
    montantDAF->setText(zero);
    montantCB->setText(zero);
    montantAutre->setText(zero);
    montantDu->setText(zero);
}


void FormPlugins::insereValeurActe(QLineEdit &p)
{  // remets à zéro toutes les valeurs financières
  allToZero();
  // Récupère le montant total de l'acte et rempli le champs Espèces
  // test si tiers payant ?
  // si oui ==> CB = montantTiers ; DAF = montantActe - montantCB;
  if (chkTiersPayant->isChecked())
  {  QString tiers , total;
      tiers = montantTiers->text();
      total = montantTotal->text();
      p.setText(tiers);
      double montantTiers = total.toDouble() - tiers.toDouble();
      montantDAF->setText(QString::number(montantTiers));
  }
  else {  p.setText(montantTotal->text()); }
}

//----------------------------- Fonctions bouttons Esp, Chq... ---------------------
void FormPlugins::butEsp_clicked()
{  insereValeurActe(*montantEsp); }
void FormPlugins::butChq_clicked()
{  insereValeurActe(*montantChq); }
void FormPlugins::butCB_clicked()
{  insereValeurActe(*montantCB); }
void FormPlugins::butDAF_clicked()
{ allToZero();  montantDu->setText(montantTotal->text()); }
void FormPlugins::butAutre_clicked()
{  insereValeurActe(*montantAutre); }
void FormPlugins::butDu_clicked()
{  insereValeurActe(*montantDu); }


void FormPlugins::getHonoraire()
{ pHono = new Honoraires (	nomPatient->text(),
				nomPraticien->text(),
				dateObserv->text(),
				libelleActe->currentText(),
				montantEsp->text().toDouble(),
				montantChq->text().toDouble(),
				montantCB->text().toDouble(),
				montantDAF->text().toDouble(),
				montantAutre->text().toDouble()  );

}


void FormPlugins::buttonHelp_clicked()
{  Acte_Selection *a = new Acte_Selection(this);
    a->exec();  
}


void FormPlugins::accept()
{ getHonoraire();
  bool sansErreur = G_pCApp->m_ComptaBase->saveHonorairesToBase(pHono);
  if (!sansErreur)
    // une erreur est survenue --> afficher l'erreur.
    {	QMessageBox *b = new QMessageBox(this);
	b->setText( G_pCApp->m_ComptaBase->m_messageErreur );
	b->exec();
    }
  else {   destroy();   }
}



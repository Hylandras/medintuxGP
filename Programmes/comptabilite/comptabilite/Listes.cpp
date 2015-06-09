 /********************************* Listes.cpp **********************************
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 2                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat a l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Federation, 75752 PARIS cedex 15.   *
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
/*! \file Listes.cpp
    \brief (put some short descripion here).

     #include "Listes.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#include "Listes.h"


/*! \brief Trace les modifications effectuées sur l'honoraire.
 * \param anterieur : Honoraire à son état antérieur à la modification.
 * \param user : Utilisateur à la source de la modification.
 * \return le résultat de l'opération est sauvegardé dans la variable m_Tracabilite de la classe en cours.
*/
void Honoraires::traceModifications(Honoraires* anterieur, Utilisateurs* user)
{ QString msg = "";
  QString tmp = "";
  QStringList hd; 	hd.clear();
  QStringList lines; 	lines.clear();
  msg  = "<p align=center>Modification faite le "+QDate::currentDate().toString("dddd dd MMMM yyyy hh:mm:s")+ "<br />";
  msg += "par " + user->getNom() + " (Id = " + QString::number(user->getId()) +")</p>";

  hd << TR("Item") << TR("Avant Modification") << TR("Après modification") << "&nbsp;";

  if (anterieur->getId() != getId()) tmp = "<b>*</b>"; else tmp ="";
  lines << TR("Id") << QString::number( anterieur->getId() ) << QString::number( getId() ) << tmp;

  if (anterieur->getPatient() != getPatient()) tmp = "<b>*</b>"; else tmp ="";
  lines << TR("Patient") << anterieur->getPatient() << getPatient() << tmp;

  if (anterieur->getPraticien() != getPraticien()) tmp = "<b>*</b>"; else tmp ="";
  lines << TR("Praticien")<< anterieur->getPraticien() << getPraticien() << tmp;

  if (anterieur->getDate() != getDate()) tmp = "<b>*</b>"; else tmp ="";
  lines << TR("Date")<< anterieur->getDate().toString("dddd dd MMMM yyyy")<< getDate().toString("dddd dd MMMM yyyy") << tmp;

  if (anterieur->getActesToString() != getActesToString()) tmp = "<b>*</b>"; else tmp ="";
  lines << TR("Actes") << anterieur->getActesToString()<< getActesToString() << tmp;

  if (anterieur->getGUID() != getGUID()) tmp = "<b>*</b>"; else tmp ="";
  lines << TR("GUID Patient") << anterieur->getGUID() << getGUID() << tmp;

  if (anterieur->toString() != toString()) tmp = "<b>*</b>"; else tmp ="";
  lines << TR("Paiement") << anterieur->toString() << toString() << tmp;

  if (anterieur->getValidite() != getValidite()) tmp = "<b>*</b>"; else tmp ="";
  lines << TR("Validé")<< QString::number( anterieur->getValidite() ) << QString::number( getValidite() ) << tmp;

  Tools* tools = new Tools();
  tools->createHTMLTable(hd, lines, msg, 4, 0);
  delete tools;
  msg += "<br />";
  if (anterieur->getTracabilite() != "") msg.prepend( anterieur->getTracabilite() );
  setTracabilite( msg );
//qWarning(m_Tracabilite); 
}


/*! \brief Trace les modifications effectuées sur le mouvement.
 * \param anterieur : Mouvement à son état antérieur à la modification.
 * \param user : Utilisateur à la source de la modification.
 * \return le résultat de l'opération est sauvegardé dans la variable m_Tracabilite de la classe en cours.
*/
void Mouvements::traceModifications(Mouvements* anterieur, Utilisateurs* user, 
				    Mouvements* pMvts_Dispo_Ant, Mouvements* pMvts_Dispo_Actuel, 
				    COMPTES_LISTE& pComptes_Liste)
{ QString msg = "";
  QString tmp = "";
  QString tmp2, tmp3;  tmp2 =""; tmp3 ="";
  QStringList hd; 	hd.clear();
  QStringList lines; 	lines.clear();
  msg  = "<p align=center>Modification faite le "+QDateTime::currentDateTime().toString("dddd dd MMMM yyyy hh:mm:s")+ "<br />";
  msg += "par " + user->getNom() + " (Id = " + QString::number(user->getId()) +")</p>";

  hd << TR("Item") << TR("Avant Modification") << TR("Après modification") << "&nbsp;";

  if (anterieur->getId() != getId()) tmp = "<b>*</b>"; else tmp ="";
  lines << TR("Id") << QString::number( anterieur->getId() ) << QString::number( getId() ) << tmp;

  if (anterieur->getNom() != getNom()) tmp = "<b>*</b>"; else tmp ="";
  lines << TR("Libellé") << anterieur->getNom() << getNom() << tmp;

  if (anterieur->getIdCompte() != getIdCompte()) tmp = "<b>*</b>"; else tmp ="";
  COMPTES_LISTE::iterator it;
  for (it = pComptes_Liste.begin(); it != pComptes_Liste.end(); ++it)
  { if ( (*it).getId() == anterieur->getIdCompte() )
    { tmp2 = "id = " + QString::number( (*it).getId()) + "<br />" + (*it).getRib("HTML"); }
    if ( (*it).getId() == getIdCompte() )
    { tmp3 = "id = " + QString::number( (*it).getId()) + "<br />" + (*it).getRib("HTML"); }
  }
  lines << TR("Compte bancaire")<< tmp2 << tmp3 << tmp;

  if (anterieur->getIdParent() != getIdParent()) tmp = "<b>*</b>"; else tmp ="";
  lines << TR("Catégorie")<< pMvts_Dispo_Ant->getNom();
  lines << pMvts_Dispo_Actuel->getNom()  << tmp;

  if (anterieur->getTotal() != getTotal()) tmp = "<b>*</b>"; else tmp ="";
  lines << TR("Total")<< QString::number(anterieur->getTotal()) << QString::number(getTotal()) << tmp;

  if (anterieur->getDateValeur() != getDateValeur()) tmp = "<b>*</b>"; else tmp ="";
  lines << TR("Date de valeur")<< anterieur->getDateValeur().toString("dddd dd MMMM yyyy")<< getDateValeur().toString("dddd dd MMMM yyyy") << tmp;

  if (anterieur->getIdImmob() != getIdImmob()) tmp = "<b>*</b>"; else tmp ="";
  lines << TR("Lien avec une immobilisation (id)") << QString::number(anterieur->getIdImmob()) << QString::number(getIdImmob()) << tmp;

  if (anterieur->getValidite() != getValidite()) tmp = "<b>*</b>"; else tmp ="";
  if (anterieur->getValidite() == 0) tmp2 = TR("Non validé"); else tmp2 = TR("Validé");
  if (getValidite() == 0) tmp3 = TR("Non validé"); else tmp3 = TR("Validé");
  lines << TR("Validé")<< tmp2 << tmp3 << tmp;

  if (anterieur->getRemarque().find(PAIEMENT_CHEQUE) != -1 )		tmp2 = TR("Chèque");
  else if (anterieur->getRemarque().find(PAIEMENT_ESPECES) != -1)	tmp2 = TR("Espèces");
  else if (anterieur->getRemarque().find(PAIEMENT_CB) != -1)		tmp2 = TR("CB");
  else if (anterieur->getRemarque().find(PAIEMENT_VIREMENT) != -1)	tmp2 = TR("Virement");
  else if (anterieur->getRemarque().find(PAIEMENT_AUTRE) != -1)		tmp2 = TR("Autre");
  if (getRemarque().find(PAIEMENT_CHEQUE) != -1)		tmp3 = TR("Chèque");
  else if (getRemarque().find(PAIEMENT_ESPECES) != -1)		tmp3 = TR("Espèces");
  else if (getRemarque().find(PAIEMENT_CB) != -1)		tmp3 = TR("CB");
  else if (getRemarque().find(PAIEMENT_VIREMENT) != -1)		tmp3 = TR("Virement");
  else if (getRemarque().find(PAIEMENT_AUTRE) != -1)		tmp3 = TR("Autre");
  if (tmp2 != tmp3) tmp = "<b>*</b>"; else tmp ="";
  lines << TR("Mode de Paiement") << tmp2 << tmp3 << tmp;

  Tools* tools = new Tools();
  tools->createHTMLTable(hd, lines, msg, 4, 0);
  delete tools;
  msg += "<br />";
  if (anterieur->getTracabilite() != "") msg.prepend( anterieur->getTracabilite() );
  setTracabilite( msg );
//qWarning(m_Tracabilite); 
}


/*! \brief Définit l'émetteur d'un chèque pour l'honoraire.
 *  \param em : nom de l'émetteur
 *  Ce paramètre est sauvegardé dans le champs remarque après la macro EMETTEUR. 
 *
 *  Attention, si un emetteur est déjà défini celui-ci est écrasé...
*/
void Honoraires::setEmetteur (const char* em)
{ QString tmp = "";
  int posdeb = m_Remarque.find( EMETTEUR , 0);
  tmp = em;
  // Tous les chèques ont un emetteur qui est par défaut le Patient
  if (tmp == "") { tmp += m_Patient; }
  tmp.replace("|", "!");
  tmp.prepend( EMETTEUR );
  if (posdeb == -1)   // Pas d'émetteur déjà enregistré
  { m_Remarque += tmp;  }
  else
  { // Il existe un émetteur : efface cette référence
    int posfin = posdeb;
    posfin = m_Remarque.find( "|", posdeb+4);
    if (posfin != -1)
    { m_Remarque = m_Remarque.left(posdeb-1) + m_Remarque.right(m_Remarque.length() - posfin);  }
    else 
    { m_Remarque = m_Remarque.left(posdeb); }
    // Ajoute la nouvelle
    m_Remarque += tmp;
  }
}

/*! \brief Récupère l'émetteur d'un chèque pour l'honoraire.
 *  Ce paramètre est sauvegardé dans le champs remarque après la macro EMETTEUR. 
*/
QString Honoraires::getEmetteur ()
{ QString tmp = "";
  int posdeb = m_Remarque.find(EMETTEUR, 0);
  if (posdeb != -1) // Trouvé
  { // Récupère le nom
    posdeb += 4;
    int posfin = posdeb;
    QString trouve = "";
    posfin = m_Remarque.find( "|", posdeb);
    if (posfin == -1)
    { posfin = m_Remarque.length();  }
    tmp = m_Remarque.mid(posdeb,posfin-posdeb);
  }
  return tmp;
}

/*! \brief Définit le montant du chèque ainsi que l'émetteur (par défaut le patient).
 *  Ce paramètre est sauvegardé dans le champs remarque après la macro EMETTEUR. 
*/
void Honoraires::setCheque(double n)
{ m_Cheque   = n;
  if (m_Remarque.find( EMETTEUR ) == -1) setEmetteur(m_Patient);
}


/*! \brief Définit la liste des actes correspondant à l'honoraire.
 *  \param pay : liste des paiements
 *  Le paramètre "Actes" de l'honoraire est intégralement remplacé par la liste des actes avec leur paiement. Ce paramètres est ainsi construit : \n \n

 * NOM_ACTE//TYPE_ACTE//TIERS//ESPECES//CHEQUES//CB//DAF//AUTRE//DU|

 * \todo ATTENTION m_Actes doit être limité à 1000 caractères (limite de la base)
*/
void Honoraires::setActes(PAIEMENTS_LISTE &pay)
{ PAIEMENTS_LISTE::iterator 	it;
  for (it = pay.begin(); it !=  pay.end(); ++it )
   { m_Actes += 	(*it).m_Nom;
     m_Actes +=	"//" +	(*it).m_Type;
     if ( (*it).m_Tiers ) m_Actes += "//O"; else m_Actes += "//N";
     m_Actes += "//" + QString::number(  (*it).m_Esp	);
     m_Actes += "//" + QString::number(  (*it).m_Chq	);
     m_Actes += "//" + QString::number(  (*it).m_CB	);
     m_Actes += "//" + QString::number(  (*it).m_DAF	);
     m_Actes += "//" + QString::number(  (*it).m_Autre	);
     m_Actes += "//" + QString::number(  (*it).m_CMU	);
	 m_Actes += "//" + QString::number(  (*it).m_Virement	);
     m_Actes += "|";
   }
}


/*! \brief Remplit le champs Actes de l'honoraire avec ce qui est récupéré depuis la base de données.
 *  Le paramètre "Actes" de l'honoraire est intégralement remplacé par la liste des actes avec leur paiement. Ce paramètres est ainsi construit : \n \n
 * NOM_ACTE//TYPE_ACTE//TIERS//ESPECES//CHEQUES//CB//DAF//AUTRE//DU|
 * \todo ATTENTION m_Actes doit être limité à 1000 caractères (limite de la base)
*/
void Honoraires::setActes(const char* recup_depuis_la_base)
{ m_Actes = recup_depuis_la_base; }


//--------------------------- getActes ---------------------------
/*! \brief Récupère la liste des actes correspondant à l'honoraire dans une liste de \em Paiements.
 *  \param pay : liste des paiements
*/
void Honoraires::getActes(PAIEMENTS_LISTE &pay)
{ PAIEMENTS_LISTE::iterator	it;
  if ( m_Actes.find("|") != -1) // il existe au moins un acte dans l'honoraire
   {	QString tiers;
	bool    boolTiers;
	QStringList act = QStringList::split( "|" , m_Actes );
	for ( uint i = 0  ;  i < act.count()   ; ++i)
	  {  QStringList p = QStringList::split ( "//" , act[i]);
	     if (p[3] == "O") boolTiers = TRUE; else boolTiers = FALSE;
	     pay.append( Paiements ( p[0],p[1], boolTiers, p[3].toDouble(), p[4].toDouble(), p[5].toDouble(), p[6].toDouble(), p[7].toDouble(), p[8].toDouble(), p[8].toDouble() ) );
	  }
   }
  else pay.clear();
}

//--------------------------- getActesToString ---------------------------
/*! \brief Récupère la liste des actes correspondant à l'honoraire dans une QString pour un affichage intelligible.
*/
QString Honoraires::getActesToString()
{ QString tmp = "";
  PAIEMENTS_LISTE *pay = new PAIEMENTS_LISTE;
  pay->clear();
  getActes( *pay );
  PAIEMENTS_LISTE::iterator  it;
  for (it = pay->begin(); it !=  pay->end(); ++it )
   { // Récupère le nom de tous les actes
     tmp += (*it).getNom() + "+";
   }
  // Enlève le dernier caractère '+'
  return tmp.left( tmp.length() - 1 );
}


//--------------------------- getTypesActes ---------------------------
/*! \brief Récupère la liste des types d'actes séparés par un | , correspondant à l'honoraire dans une QString.
*/
QString Honoraires::getTypesActes()
{ QString tmp = "";
  PAIEMENTS_LISTE *pay = new PAIEMENTS_LISTE;
  pay->clear();
  getActes( *pay );
  PAIEMENTS_LISTE::iterator  it;
  for (it = pay->begin(); it !=  pay->end(); ++it )
   { // Récupère le nom de tous les actes
     tmp += (*it).getType() + "|";
   }
  // Enlève le dernier caractère '+'
  return tmp.left( tmp.length() - 1 );
}


//--------------------------- getRemarqueLisible ---------------------------
/*! \brief Transforme le champs remarque codé SQL en format lisible pour l'être humain.
 * \return La traduction du champs Remarque de l'honoraire.
*/
QString Honoraires::getRemarqueLisible()
{ QString tmp = "";
  if (m_Remarque.find( CHEQUE_DEPOSE ) != -1 )  tmp  = TR("Chèque déposé en banque en date du :");
  if (m_Remarque.find( CHEQUE_PAS_DEPOSE ) != -1 )  tmp += TR("Chèque non déposé en banque \n");
  if (m_Remarque.find( DAF_OK ) != -1 )  tmp += TR("Paiement différé validé. \n");
  if (m_Remarque.find( DAF_PAS_OK ) != -1 )  tmp += TR("Paiement différé non validé. \n");
  if (m_Remarque.find( CMU_OK ) != -1 )  tmp += TR("Paiement différé validé. \n");
  if (m_Remarque.find( CMU_PAS_OK ) != -1 )  tmp += TR("Paiement différé non validé. \n");
  return tmp;
}


//--------------------------- dateForSQL ---------------------------
/*! \brief Transforme une date à la française dd-MM-yyyy en date pour SQL yyyy-MM-dd.
 * \return Retourne la date passée en entrée sous le format dd-MM-yyyy sous forme yyyy-MM-dd
*/
QString Honoraires::dateForSQL(const QString &date)
{int end = date.length();
 int   i = -1;
 QString str;
 while (++i < end) { if (  (date[i]>='0' && date[i]<='9') ) str += date[i]; }
 if (str.length() ==6 )  str.insert ( 4, "20" ); // date forme 011207 --> 01122007
 if (str.length() !=8 )  return "format invalide";

 return str.mid(4,4)+"-"+str.mid(2,2)+"-"+str.mid(0,2);
}


/*! \brief Renvoie la date de l'honoraire sous le format QDate.
*/
QDate Honoraires::getDate()
{ QDate d;
  d = QDate::fromString(dateForSQL(m_Date), Qt::ISODate);
  return d;
}



//--------------------------- toString ---------------------------
/*! \brief Retourne le mode de paiement et les montants sous forme lisible pour l'être humain.
 * \return Retourne le mode de paiement et les montants sous forme lisible pour l'être humain.
*/
QString Honoraires::toString()
{QString str = "";
 if (total() == 0) str = "Acte Gratuit";
 if (m_Especes > 0)  str += "Esp = "   + QString::number( m_Especes ) + "  ";
 if (m_Cheque > 0)   str += "Chq = "   + QString::number( m_Cheque) + "  ";
 if (m_CB > 0)       str += "CB = "    + QString::number( m_CB ) + "  ";
 if (m_Autre > 0)    str += "Autre = " + QString::number( m_Autre ) + "  ";
 if (m_DAF > 0)      str += "DAF = "   + QString::number( m_DAF ) + "  ";
 if (m_CMU > 0)       str += "Du = "    + QString::number( m_CMU ) + "  ";
 if (m_Virement > 0)       str += "Virement = "    + QString::number( m_Virement ) + "  ";
 //if (m_Remarque != " ")  str += "Remarque : "+m_Remarque;
 return str;
}



QString Paiements::toString()
{QString str = "";
 if (total() == 0) str = "Acte Gratuit";
 if (m_Esp > 0)   str += "Esp = "   + QString::number( m_Esp ) + "  ";
 if (m_Chq > 0)   str += "Chq = "   + QString::number( m_Chq) + "  ";
 if (m_CB > 0)    str += "CB = "    + QString::number( m_CB ) + "  ";
 if (m_Autre > 0) str += "Autre = " + QString::number( m_Autre ) + "  ";
 if (m_DAF > 0)   str += "DAF = "   + QString::number( m_DAF ) + "  ";
 if (m_CMU > 0)    str += "CMU = "    + QString::number( m_CMU ) + "  ";
 if (m_Virement > 0)    str += "Virement = "    + QString::number( m_Virement ) + "  ";
 return str;
}


/*! \brief Définit le RIB de l'utilisateur. Attention les données sont vérifiées AVANT d'être enregistrées dans la base. */
void Comptes::setRib(const char* banq, const char* guich, const char* compte, const char* cle)
{ m_Rib_Banque = banq;
  m_Rib_Guichet = guich;
  m_Rib_Compte = compte;
  m_Rib_Cle = cle;
}

/*! \brief Construit le RIB de l'utilisateur directement exploitable dans les documents. 
 *  \param type = format choisi de sortie html ou pas
*/
QString Comptes::getRib(const char* type)
{ QString tmp = type;
  QString ligne = "";
  if (tmp.lower() == "html") ligne = "<br />";
  else ligne = " ";
  tmp  = "Banque : " + getNomBanque() + ligne + "RIB : ";
  tmp += getRibBanque() + " " + getRibGuichet() + " " + getRibCompte() + " " + getRibCle() + ligne;
  return tmp;
}

/*! \brief Construit le RIB de l'utilisateur pour la sauvegarde dans le champs remarque des dépôts et autres. 
 * Le format est le suivant : |rib12345123451234567890112|cpt1
*/
QString Comptes::getRibForBase()
{ QString tmp = "";
  tmp  = RIB_DEPOT;
  tmp += getRibBanque() + getRibGuichet() + getRibCompte() + getRibCle();
  tmp += ID_CPT_DEPOT;
  tmp += QString::number(getId());
  return tmp;
}

/*! \brief Récupère le montant total du dépôt. Cette fonction est utilisée lors de la récupération des dépôts pour la construction des relevés de comptes.
 *  Ce paramètre est sauvegardé dans le champs remarque après la macro TOTAL_DEPOT.
*/
QString Mouvements::getTotalDepot(QString& rmq)
{ QString tmp = "";
  int posdeb = rmq.find(TOTAL_DEPOT, 0);
  if (posdeb != -1) // Trouvé
  { // Récupère le nom
    posdeb += 4;
    int posfin = posdeb;
    QString trouve = "";
    posfin = m_Remarque.find( "|", posdeb);
    if (posfin == -1)
    { posfin = rmq.length();  }
    tmp = rmq.mid(posdeb,posfin);
  }
  return tmp;
}


/*! \brief Prépare le tableau HTML qui présente l'immobilisation selon les paramètres. Le tableau est sauvegardé dans la variable 'resultat' de la classe.
*/
void Immobilisations::prepareResultat()
{ m_Resultat  = "";
  QStringList hd, lines;
  hd.clear();
  lines.clear();
  Tools* 	tools = new Tools();
  Mouvements*	mvts  = new Mouvements();
  m_Mvts_Liste.clear();

  hd 	<< TR("Immobilisations") << TR("Date de mise en service")  <<  TR("Valeur hors taxe");
  lines	<< 	m_Nom		 << m_DateService.toString("dd-MM-yyyy") <<  QString::number(m_Valeur);

  if (m_Mode == 0) // linéaire ==> insérer la valeur résiduelle
  { hd <<  TR("Valeur résiduelle");
    lines << QString::number(m_Residuel);
  }

  tools->createHTMLTable(hd, lines, m_Resultat, 4, 0);

  m_Resultat.append("<br /><br />");

  // Préparation du tableau des immobilisations
  hd.clear();
  hd << TR("Année")  <<  TR("Taux")  <<  TR("Valeur annuité")  <<  TR("Valeur nette comptable");
  lines.clear();

  // Calcul de l'immobilisation
  if (m_Mode == 0) 				// LINEAIRE
  { double taux = (100.0 / m_Duree);
    int annee = m_DateService.year();
    int val = m_Valeur - m_Residuel;
    int vnc = m_Valeur;
    QMemArray<int> annuite = QMemArray<int> (m_Duree);

    // Calcul de l'année incomplete sur 360 jours
    if ((m_DateService.month() != 1) || (m_DateService.day() != 1))
    { QDate d2( annee, 1, 1 );
      int nbDays = d2.daysTo( m_DateService ) + 1; // Il faut compter le jour de mise en service en plus
      double tmp = ((double)(360 - nbDays)/360.0);
      annuite[0] = (int)((val/m_Duree) * tmp);
      vnc = vnc - annuite[0];
      lines <<  QString::number(annee)  <<  QString::number(taux)  <<  QString::number(annuite[0])  <<  QString::number(vnc);
      QString rmq = "";
      rmq += VNC_IMMOBILISATION;
      rmq += QString::number(vnc);
      rmq += TAUX_IMMOBILISATION;
      rmq += QString::number(taux);
      mvts->setNom(TR("Immobilisation : ") + m_Nom);
      mvts->setRemarque(rmq);
      mvts->setDateValeur( m_DateService );
      mvts->setDate( m_DateService );
      mvts->setType( 1 );
      mvts->setIdParent( ID_IMMOBILISATIONS );
      mvts->setIdCompte( m_IdCompte ); // IdUsr est géré par ComptaMoteurBase
      mvts->setTotal( annuite[0] ) ;
      m_Mvts_Liste.append( *mvts );
    }
    else
    { annuite[0] = (val/m_Duree);
      vnc = vnc - annuite[0];
      lines <<  QString::number(annee)  <<  QString::number(taux)  <<  QString::number(annuite[0])  <<  QString::number(vnc);
      QString rmq = "";
      rmq += VNC_IMMOBILISATION;
      rmq += QString::number(vnc);
      rmq += TAUX_IMMOBILISATION;
      rmq += QString::number(taux);
      mvts->setNom(TR("Immobilisation : ") + m_Nom);
      mvts->setRemarque(rmq);
      mvts->setDateValeur( m_DateService );
      mvts->setDate( m_DateService );
      mvts->setType( 1 );
      mvts->setIdParent( ID_IMMOBILISATIONS );
      mvts->setIdCompte( m_IdCompte ); // IdUsr est géré par ComptaMoteurBase
      mvts->setTotal( annuite[0] ) ;
      m_Mvts_Liste.append( *mvts );
    }
    annee++;

    for (int i=1; i < (m_Duree); ++i)
    { annuite[i] = (val/m_Duree);
      vnc = vnc - annuite[i];
      lines <<  QString::number(annee)  <<  QString::number(taux)  <<  QString::number(annuite[i])  <<  QString::number(vnc);
      QString rmq = "";
      rmq += VNC_IMMOBILISATION;
      rmq += QString::number(vnc);
      rmq += TAUX_IMMOBILISATION;
      rmq += QString::number(taux);
      mvts = new Mouvements();
      mvts->setNom(TR("Immobilisation : ") + m_Nom);
      mvts->setRemarque(rmq);
      mvts->setDateValeur( QDate(annee, 1, 1) );
      mvts->setDate( m_DateService );
      mvts->setType( 1 );
      mvts->setIdParent( ID_IMMOBILISATIONS );
      mvts->setIdCompte( m_IdCompte ); // IdUsr est géré par ComptaMoteurBase
      mvts->setTotal( annuite[i] ) ;
      m_Mvts_Liste.append( *mvts );
      delete mvts;
      annee++;
    }

    // Calcul du reste à immobiliser
    if ((vnc - m_Residuel) > 0)
     { //vnc = vnc - val;
       annee--;
       lines <<  QString::number(annee+1)  <<  QString::number(taux)  <<  QString::number(vnc-m_Residuel)  <<  QString::number(vnc-(vnc-m_Residuel));
      QString rmq = "";
      rmq += VNC_IMMOBILISATION;
      rmq += QString::number((vnc-(vnc-m_Residuel)));
      rmq += TAUX_IMMOBILISATION;
      rmq += QString::number(taux);
      mvts = new Mouvements();
      mvts->setNom(TR("Immobilisation : ") + m_Nom);
      mvts->setRemarque(rmq);
      mvts->setDateValeur( QDate(annee, 1, 1) );
      mvts->setDate( m_DateService );
      mvts->setType( 1 );
      mvts->setIdParent( ID_IMMOBILISATIONS );
      mvts->setIdCompte( m_IdCompte ); // IdUsr est géré par ComptaMoteurBase
      mvts->setTotal( (vnc-m_Residuel ) ) ;
      m_Mvts_Liste.append( *mvts );
      delete mvts;
     }
  }  // Fin mode linéaire
  else if (m_Mode == 1) 			// MODE DEGRESSIF
  { int nbMoisDebut = m_DateService.month();  // Nombre de mois écoulés avant la date d'acquisition
    int annee = m_DateService.year();
    // Lorsque le taux dégressif est inférieur au taux linéaire calculé à partir de chaques annuités -> on prend le taux linéaire.
    // Ex : durée 5 Ans		Année	TauxDeg	TauxLin
    //				0	35%	20%
    //				1	35%	25%
    //				2	35%	33%
    //				3	35%	50%  --> on prend le taux linéaire
    //				4	35%	100% --> on prend le taux linéaire
    double tauxDeg = 0.0;	// Taux dégressif
    double tauxLin = 0.0;	// Taux linéaire sur le résiduel
    double taux = 0.0;		// Taux utilisé pour les calculs
    int val = m_Valeur;
    int vnc = val;
    int annuiteAdd = 0;
    QMemArray<int> annuite = QMemArray<int> (m_Duree);

    if (m_Duree <= 4) taux = 1.25;
    else if ( (m_Duree == 5) || (m_Duree == 6)) taux = 1.75;
    else taux = 2.25;
    tauxDeg = ((100.0 / m_Duree) / 100.0) * taux;
    taux = tauxDeg;

    // Calcul de l'année incomplete sur 12 mois
    if (nbMoisDebut > 0)
    { double tmp = (13 - nbMoisDebut) / 12.0;  // Il faut compte le mois de l'acquisition
      annuite[0] = (int)(val * taux * (tmp));
      vnc = vnc - annuite[0];
      lines <<  QString::number(annee)  <<  QString::number(tauxDeg)  <<  QString::number(annuite[0])  <<  QString::number(vnc);
      QString rmq = "";
      rmq += VNC_IMMOBILISATION;
      rmq += QString::number(vnc);
      rmq += TAUX_IMMOBILISATION;
      rmq += QString::number(taux);
      mvts = new Mouvements();
      mvts->setNom(TR("Immobilisation : ") + m_Nom);
      mvts->setRemarque(rmq);
      mvts->setDateValeur( QDate(annee, 1, 1) );
      mvts->setDate( m_DateService );
      mvts->setType( 1 );
      mvts->setIdParent( ID_IMMOBILISATIONS );
      mvts->setIdCompte( m_IdCompte ); // IdUsr est géré par ComptaMoteurBase
      mvts->setTotal( annuite[0] ) ;
      m_Mvts_Liste.append( *mvts );
      delete mvts;
    }
    else
    { annuite[0] = (int)(val * taux);
      vnc = vnc - annuite[0];
      lines <<  QString::number(annee)  <<  QString::number(taux)  <<  QString::number(annuite[0])  <<  QString::number(vnc);
      QString rmq = "";
      rmq += VNC_IMMOBILISATION;
      rmq += QString::number(vnc);
      rmq += TAUX_IMMOBILISATION;
      rmq += QString::number(taux);
      mvts = new Mouvements();
      mvts->setNom(TR("Immobilisation : ") + m_Nom);
      mvts->setRemarque(rmq);
      mvts->setDateValeur( QDate(annee, 1, 1) );
      mvts->setDate( m_DateService );
      mvts->setType( 1 );
      mvts->setIdParent( ID_IMMOBILISATIONS );
      mvts->setIdCompte( m_IdCompte ); // IdUsr est géré par ComptaMoteurBase
      mvts->setTotal( annuite[0] ) ;
      m_Mvts_Liste.append( *mvts );
      delete mvts;
    }
    annee++;
    annuiteAdd = annuite[0];

    // Calcul années complètes
    for (int i=1; i < m_Duree; ++i)
    { if ((m_Duree-i) == 0 ) tauxLin = 1;
      else tauxLin = ((100.0 / (m_Duree - i))/100);
      if (tauxLin > tauxDeg)  	taux = tauxLin;
      else			taux = tauxDeg;

      annuite[i] = (int)((val - annuiteAdd ) * taux);
      vnc = vnc - annuite[i];
      annuiteAdd += annuite[i];
      lines <<  QString::number(annee)  <<  QString::number(taux)  <<  QString::number(annuite[i])  <<  QString::number(vnc);
      QString rmq = "";
      rmq += VNC_IMMOBILISATION;
      rmq += QString::number(vnc);
      rmq += TAUX_IMMOBILISATION;
      rmq += QString::number(taux);
      mvts = new Mouvements();
      mvts->setNom(TR("Immobilisation : ") + m_Nom);
      mvts->setRemarque(rmq);
      mvts->setDateValeur( QDate(annee, 1, 1) );
      mvts->setDate( m_DateService );
      mvts->setType( 1 );
      mvts->setIdParent( ID_IMMOBILISATIONS );
      mvts->setIdCompte( m_IdCompte ); // IdUsr est géré par ComptaMoteurBase
      mvts->setTotal( annuite[i] ) ;
      m_Mvts_Liste.append( *mvts );
      delete mvts;
      annee++;
    }
  }
  // Prépare le tableau des amortissements
  tools->createHTMLTable(hd, lines ,m_Resultat, 4, 0);
  delete tools;
}

void Immobilisations::setMouvements(MVTS_LISTE& il)
{ MVTS_LISTE::iterator it;
  if (il.count() == 0)
   { m_Mouvements = ""; m_Mvts_Liste.clear(); }
  else for (it = il.begin(); it != il.end(); ++it)
  { m_Mouvements = QString::number( (*it).getId() ) + ";" ;
    m_Mvts_Liste.append( (*it) );
  }
}


bool Immobilisations::getMouvements(MVTS_LISTE& il)
{ MVTS_LISTE::iterator it;
  il.clear();
  if (m_Mvts_Liste.count() == 0)
   { m_Mouvements = ""; }
  else for (it = m_Mvts_Liste.begin(); it != m_Mvts_Liste.end(); ++it)
  { il.append( (*it) );
  }
 return TRUE;
}

void Mouvements::setIdImmob(int n)
{ QString tmp = "";
  int posdeb = m_Remarque.find( IS_IMMOBILISATION , -1);
  tmp = QString::number(n);
  // Tous les chèques ont un emetteur qui est par défaut le Patient
  tmp.prepend( IS_IMMOBILISATION );

  if (posdeb == -1)   // Pas d'id déjà enregistré
  { m_Remarque += tmp + "_";  }
  else
  { // Il existe un id : efface cette référence
    int posfin = posdeb;
    posfin = m_Remarque.find( "|", posdeb+2);
    if (posfin != -1)
    { m_Remarque = m_Remarque.left(posdeb-1) + m_Remarque.right(m_Remarque.length() - posfin);  }
    else 
    { m_Remarque = m_Remarque.left(posdeb); }
    // Ajoute la nouvelle
    tmp += "_";
    m_Remarque += tmp;
  }
  m_IdImmob = n ;
}


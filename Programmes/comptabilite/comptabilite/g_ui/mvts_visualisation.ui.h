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


void Mvts_Visualisation::setMouvement(Mouvements* mvt, Comptes* cpt, MVTS_LISTE& pMvts_Dispo )
{ Tools* tools = new Tools();
  QString msg = "";
  QString tmp = "";
  QStringList hd, lines;  	hd.clear();		lines.clear();
  msg  = "<p align=center><b>"+TR("Visualisation du mouvement")+"</b></p>";
  hd << TR("Item") << TR("Valeur");
  lines << TR("Libellé") << mvt->getNom();

  tmp = (*pMvts_Dispo.at ( mvt->getIdParent() - 1) ).getNom();
  lines << TR("Catégorie") << tmp;
  lines << TR("Libellé du compte bancaire") << cpt->getLibelle();
  lines << TR("RIB du compte bancaire") << cpt->getRib("HTML");


  if (mvt->getType() == 0) tmp = TR("Recettes"); else tmp = TR("Dépenses");
  lines << TR("Type") << tmp;
  lines << TR("Date de Valeur") << mvt->getDateValeur().toString("dd MMMM yyyy");
  lines << TR("Montant") << QString::number(mvt->getTotal(), 'f', 2);

  if (mvt->getValidite() == 0)
  { lines << TR("Valide") << TR("Non validé"); }
  else
  { QString valide = "";
    tools->getIntoRemarque(mvt->getRemarque() , HONO_VALIDE, valide );
    QDate dt = QDate::fromString(valide.left(4) + "-" + valide.mid(4,2) + "-" + valide.mid(6,2), Qt::ISODate);
    QString val = QString(TR("Le %1 par %2"))
		   .arg(dt.toString("dddd dd MMMM yyyy"), valide.right(valide.length() - 8));
    lines << TR("Validé") << val;
  }

  lines << "<b>"+TR("Informations Complémentaires")+"</b>" << "&nbsp;";
  if (mvt->getRemarque().find(PAIEMENT_CHEQUE) != -1 )		tmp = TR("Chèque");
  else if (mvt->getRemarque().find(PAIEMENT_ESPECES) != -1)	tmp = TR("Espèces");
  else if (mvt->getRemarque().find(PAIEMENT_CB) != -1)		tmp = TR("CB");
  else if (mvt->getRemarque().find(PAIEMENT_VIREMENT) != -1)	tmp = TR("Virement");
  else if (mvt->getRemarque().find(PAIEMENT_AUTRE) != -1)	tmp = TR("Autre");
  lines << TR("Mode de paiement") << tmp;


  tools->createHTMLTable(hd, lines, msg, 4,1 );
  textMvts->setText( msg );

  if (mvt->getTracabilite() != "")
  { msg = "<p align=center><b>"+TR("Traçabilité des modifications")+"</b></p>";
    msg.append( mvt->getTracabilite() );
    textTrace->setText( msg );
  }

  delete tools;

}


void Mvts_Visualisation::but_Valide_clicked()
{ accept(); }

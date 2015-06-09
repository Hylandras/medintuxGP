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


void Hono_Visualisation::setHonoraire( Honoraires* pHonoraires )
{   QString tmp, msg;
    tmp = "";   msg = "";
    msg = "<p align=center><b>"+TR("Visualisation d'honoraires.")+"</b></p>";
    QStringList hd;    hd.clear();
    QStringList lines; lines.clear();
    Tools* tools = new Tools();
    hd << TR("Item") << TR("Valeur");
    lines << TR("Patient") << pHonoraires->getPatient();
    lines << TR("Date") << pHonoraires->getDate().toString("dd MMMM yyyy");
    lines << TR("Actes") << pHonoraires->getActesToString();
    lines << TR("Montant Total") << QString::number( pHonoraires->total() );
    lines << TR("Mode de Paiement") << pHonoraires->toString();
    lines << "<b>"+TR("Informations Complémentaires")+"</b>" << "&nbsp;",
    tmp = pHonoraires->getRemarque();

    if (pHonoraires->getChq() > 0)
    {lines << TR("Emetteur du chèque") << pHonoraires->getEmetteur();
     if (tmp.find( CHEQUE_DEPOSE ) != -1) 
     { lines << TR("Chèque") << TR("Déposé.");
     }
     else
     { lines << TR("Chèque") << TR("Non déposé.");
     }
     } // Fin if chq 

    if (pHonoraires->getDAF() > 0)
    {lines << TR("Paiement Différé") ;
     if (tmp.find(DAF_OK) != -1) 
     { lines << TR("Encaissé.");
     }
     else
     { lines << TR("Non encaissé.");
     }
    }
 
	if (pHonoraires->getCMU() > 0)
    {lines << TR("Paiement Différé") ;
     if (tmp.find(CMU_OK) != -1) 
     { lines << TR("Encaissé.");
     }
     else
     { lines << TR("Non encaissé.");
     }
    }

    if (pHonoraires->getValidite() == 0)
    {lines << TR("Validé") << TR("Non");
    }
    else
    {QString valide = "";
     tools->getIntoRemarque(tmp, HONO_VALIDE, valide );
     QDate dt = QDate::fromString(valide.left(4) + "-" + valide.mid(4,2) + "-" + valide.mid(6,2), Qt::ISODate);
     QString val = QString(TR("Le %1 par %2"))
		   .arg(dt.toString("dddd dd MMMM yyyy"), valide.right(valide.length() - 8));
     lines << TR("Validé") << val;
    }
    
   tools->createHTMLTable(hd, lines, msg, 4,1 );
   
   if (pHonoraires->getTracabilite() != "") 
   { msg.append( "<p><b>"+TR("Traçabilité des modifications.</b></p>") );
     msg.append( pHonoraires->getTracabilite() );
   }
   textEdit->setText( msg );
}
    
 

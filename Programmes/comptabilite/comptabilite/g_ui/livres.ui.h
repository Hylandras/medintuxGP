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


void GrandLivre_BilanAnnuel::setParams(ComptaMoteurBase* pMB, QString& pathSave)
{ m_pComptaMoteurBase = pMB;
  m_PathSauvegarde = pathSave;
}

void GrandLivre_BilanAnnuel::butSauvGdLivre_clicked()
{ QString nFile = m_PathSauvegarde + QDir::separator();
  nFile += "Grand_Livre-" + QString::number(m_pComptaMoteurBase->m_Filtre_Date_Debut.year()) + ".html" ;
  QString tmp = "";
  m_pComptaMoteurBase->getBilanAnnuel(QString::number(m_pComptaMoteurBase->m_Filtre_Date_Debut.year()), tmp, TRUE);

  // Prépare le fichier de sauvegarde
  QFile* f = new QFile(nFile);
  if (f)
  { if (f->exists()) f->remove();
    if ( f->open( IO_WriteOnly ) )
     {QTextStream stream( f );
       stream << tmp ;
      f->close();
      QMessageBox::information(this, TR("Fichier enregistré."), TR("Le Grand Livre a été enregistrée dans le fichier&nbsp;:  ")+"<br  /><b>"+nFile+"</b>", TR("Ok"), "", "", 1, 0);
      } // Fin if open
     delete f;
   } // Fin if f
}


void GrandLivre_BilanAnnuel::butSauvBilanAnnuel_clicked()
{ QString nFile = m_PathSauvegarde + QDir::separator();
  nFile += "Bilan_Annuel-" + QString::number(m_pComptaMoteurBase->m_Filtre_Date_Debut.year()) + ".html" ;
  QString tmp = "";
  m_pComptaMoteurBase->getBilanAnnuel(QString::number(m_pComptaMoteurBase->m_Filtre_Date_Debut.year()), tmp, FALSE);

  // Prépare le fichier de sauvegarde
  QFile* f = new QFile(nFile);
  if (f)
  { if (f->exists()) f->remove();
    if ( f->open( IO_WriteOnly ) )
     {QTextStream stream( f );
       stream << tmp;
      f->close();
      QMessageBox::information(this, TR("Fichier enregistré."), TR("Le Grand Livre a été enregistrée dans le fichier&nbsp;:  ")+"<br  /><b>"+nFile+"</b>", TR("Ok"), "", "", 1, 0);
      } // Fin if open
     delete f;
   } // Fin if f
}


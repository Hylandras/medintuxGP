/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include "../../MedinTuxTools/CGestIni.h"

//--------------------------------------- pushButtonOK_clicked ---------------------------------
void Dlg_InsertAncre::pushButtonOK_clicked()
{QString text(textBrowserView->text());
 CHtmlTools::ElimineAncresRedondantes(text);
 m_Edit->setText(text);  //textBrowserView->text()
 QFile::remove(m_SavPath );
 accept();
}

//--------------------------------------- closeEvent ---------------------------------
void Dlg_InsertAncre::closeEvent ( QCloseEvent * e )
{QFile::remove(m_SavPath );
 e->accept();
}

//--------------------------------------- initDialog ---------------------------------
void Dlg_InsertAncre::initDialog( MyEditText *edit , QString &path, int *pAnchorSpaceToUnderscore)
{ m_Edit                     = edit;
  m_pAnchorSpaceToUnderscore = pAnchorSpaceToUnderscore;
  if (*m_pAnchorSpaceToUnderscore ) checkBox_RemoveSpace->setChecked (TRUE );
  //............. créer le fichier d'annulation ........................................................
  if (path.length()==0) path = "noname.html";
  int pos        =  path.findRev(QDir::separator());
  m_SavPath      =  path.left(pos+1);            // inclure le separateur si il y est
  if (pos==0)                                    // si pas de chemin defini alors utiliser celui de l'executable
     {m_SavPath  = CGestIni::Construct_PathBin_Module("QLightPad",  QFileInfo(qApp->argv()[0]).dirPath (true));
      m_SavPath  = m_SavPath.left(m_SavPath .findRev (QDir::separator())+1);
     }
  m_SavPath     += "~" + path.mid(pos+1);    // rajouter un tilde devant le nom du fichier
  QString text   = edit->text();
  CHtmlTools::ElimineAncresRedondantes(text);   // virer le QT Surplus
  QFile file(m_SavPath );
  if ( file.open( IO_WriteOnly ) )  file.writeBlock(text, text.length());

  //............. recuperer paramètres de selection de l'éditeur de texte ..............................
  int  paraFrom,  indexFrom,  paraTo,  indexTo;
  int cur_para,   cur_index;
  edit->getSelection ( &paraFrom, &indexFrom, &paraTo, &indexTo);
  edit->getCursorPosition (&cur_para, &cur_index );
  //............. recuperer texte de l'editeur pour le placer dans le browser ..........................
  textBrowserView->mimeSourceFactory()->addFilePath(edit->m_Path );
  CHtmlTools::setDefaultTextAttributs(   textBrowserView,
					 edit->m_DefaultBackgroundColor ,
					 edit->m_PathImage.length()?edit->m_Path + QDir::separator() + edit->m_PathImage:0,
					 edit->m_DefaultTextColor,
					 edit->m_DefaultFont);
  textBrowserView->setText(text );
  CHtmlTools::ListerAncres(textBrowserView->text(),  listView_Ancres);


 connect( this,            SIGNAL( linkClicked( const QString& ) ),                   textBrowserView, SIGNAL( linkClicked( const QString& ) ) );
 connect( this,            SIGNAL( anchorClicked( const QString&, const QString& ) ), textBrowserView, SIGNAL( anchorClicked( const QString&, const QString& ) ) );
 connect( textBrowserView, SIGNAL( selectionChanged() ), this,            SLOT(   selectionChanged() ) );

 setAnchorText(cur_para , cur_index, paraFrom,  indexFrom,  paraTo,  indexTo);
 //adjustSize();
 line1->setFrameStyle ( QFrame::HLine );
 line1->setFrameShadow (QFrame::Sunken );

 
}

//--------------------------------------- selectionChanged ---------------------------------
void Dlg_InsertAncre::checkBox_RemoveSpace_stateChanged( int state )
{*m_pAnchorSpaceToUnderscore = state;
}

//--------------------------------------- selectionChanged ---------------------------------
void Dlg_InsertAncre::selectionChanged()
{ int  paraFrom,  indexFrom,  paraTo,  indexTo;
  int  cur_para,  cur_index;
  textBrowserView->getSelection ( &paraFrom, &indexFrom, &paraTo, &indexTo);
  textBrowserView->getCursorPosition (&cur_para, &cur_index );
  setAnchorText(cur_para , cur_index, paraFrom,  indexFrom,  paraTo,  indexTo);
}


//--------------------------------------- setAnchorText ---------------------------------
void Dlg_InsertAncre::setAnchorText(int &cur_para , int &cur_index, int &paraFrom,  int &indexFrom,  int &paraTo,  int &indexTo)
{//.................. placer curseur texte meme endroit que dans editeur ......................
 textBrowserView->setCursorPosition (cur_para, cur_index );
 textBrowserView->setSelection ( paraFrom,  indexFrom,  paraTo,  indexTo);
 textBrowserView->ensureCursorVisible();
 lineEdit_Ancre->setText(textBrowserView->selectedText() );

}

//--------------------------------------- listView_Ancres_clicked ---------------------------------
void Dlg_InsertAncre::listView_Ancres_clicked( QListViewItem * pQListViewItem)
{if(pQListViewItem==0) return;
 QString ancre_txt = pQListViewItem->text(0);
 ancre_txt.replace('>',"&gt;");
 ancre_txt.replace('<',"&lt;");
 //emit  linkClicked   (m_SavPath.mid(m_SavPath.findRev(QDir::separator())+1) + "#"+ ancre_txt);
 //emit  anchorClicked (ancre_txt,    m_SavPath.mid(m_SavPath.findRev(QDir::separator())+1) + "#"+ ancre_txt);
 textBrowserView->scrollToAnchor ( ancre_txt );
 //qDebug(tr("Texte de l'ancre : %1\r\nfichier : %2").arg( ancre_txt, m_SavPath.mid(m_SavPath.findRev(QDir::separator())+1) + "#"+ ancre_txt ));
}


//--------------------------------------- textBrowserView_anchorClicked ---------------------------------
void Dlg_InsertAncre::textBrowserView_anchorClicked( const QString &, const QString &link )
{textBrowserView->setSource(link);
 //qDebug(link);
}

//--------------------------------------- pushButtonAddAnchorDelete_clicked ---------------------------------
void Dlg_InsertAncre::pushButtonAddAnchorDelete_clicked()
{  QPtrList <QListViewItem> *pQPtrList = new QPtrList <QListViewItem> ;
   pQPtrList->setAutoDelete( TRUE );           // comme � les items s'auto effaceront en sortie de fonction
   QListViewItemIterator it( listView_Ancres );
   //....................... recuperer la chaine du texte ...........................
   QString* pTxt = new QString(textBrowserView->text());
   if (pTxt==0)                                                            return;
   QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );

   while ( it.current() ) 
         {  QListViewItem *pQListViewItem = it.current();
            if (pQListViewItem->isSelected())
               {pQPtrList->append(pQListViewItem);       // on l'ajoute �la liste autodetruite
                QString ancre_txt = pQListViewItem->text(0);
                ancre_txt.replace('>',"&gt;");
                ancre_txt.replace('<',"&lt;");
                CHtmlTools::EffacerAncre(*pTxt, ancre_txt);
               }
            ++it;
        }
  delete pQPtrList;
  QApplication::restoreOverrideCursor ();

  //................ sauver le document modifié ...........................................
  QFile file(m_SavPath);
  if ( file.open( IO_WriteOnly ) ) file.writeBlock(*pTxt, pTxt->length());
  //................ positionner le browser dessus ...........................................
  textBrowserView->setText(*pTxt);
  delete pTxt;                      // plus besoin
  //................. regenerer la liste des ancres pour contrôle ....................................
  lineEdit_Ancre->setText("");
  listView_Ancres->clear();
  CHtmlTools::ListerAncres(textBrowserView->text(),  listView_Ancres);

  /*
  QListViewItem * pQListViewItem =  listView_Ancres->selectedItem();
  if(pQListViewItem==0)                                                   return;
  QString* pTxt = new QString(textBrowserView->text());
  if (pTxt==0)                                                            return;
  QString ancre_txt = pQListViewItem->text(0);
  ancre_txt.replace('>',"&gt;");
  ancre_txt.replace('<',"&lt;");
  CHtmlTools::EffacerAncre(*pTxt, ancre_txt);
  //................ sauver le document modifié ...........................................
  QFile file(m_SavPath);
  if ( file.open( IO_WriteOnly ) ) file.writeBlock(*pTxt, pTxt->length());

  //................ positionner le browser dessus ...........................................
  textBrowserView->setText(*pTxt);
  delete pTxt;                      // plus besoin
  //................. regenerer la liste des ancres pour contrôle ....................................
  lineEdit_Ancre->setText("");
  listView_Ancres->clear();
  CHtmlTools::ListerAncres(textBrowserView->text(),  listView_Ancres);
  */
}

//--------------------------------------- pushButtonAddAnchor_clicked ---------------------------------
void Dlg_InsertAncre::pushButtonAddAnchor_clicked()
{ //.......................  si pas d'ancre cassos ..............................................
  if (lineEdit_Ancre->text().length()==0)                                return;
  QString ancre_txt = lineEdit_Ancre->text().replace(':',',').stripWhiteSpace();
  if (checkBox_RemoveSpace->isChecked()) ancre_txt.replace(' ','_');
  ancre_txt.replace('>',"&gt;");
  ancre_txt.replace('<',"&lt;");
  int  paraFrom,  indexFrom,  paraTo,  indexTo;

  //.............. placer le curseur sur le debut de la selection ...............................
  //               c'est là que l'on va inserer l'ancre
  textBrowserView->getSelection ( &paraFrom, &indexFrom, &paraTo, &indexTo);
  textBrowserView->setCursorPosition (paraFrom, indexFrom );
  //.............. marquer l'endroit de l'ancre .................................................
  textBrowserView->setSelection (paraFrom, indexFrom, paraFrom, indexFrom );
  textBrowserView->setCursorPosition (paraFrom, indexFrom );
  textBrowserView->insert("~~##µµ##~~");
  //.............. recuperer le texte du browser pour le manipuler ..............................
  QString* pTxt = new QString(textBrowserView->text());
  if (pTxt==0)                                                           return;
  CHtmlTools::ElimineAncresRedondantes(*pTxt);   // pas la peine de garder Les ancres parasites de QT
  //................ rechercher si cette ancre n'existe pas déjà ................................
  //                 et l'effacer
  QListViewItem *pQListViewItem = listView_Ancres->findItem (ancre_txt, 0);
  if (pQListViewItem)
     {if (QMessageBox::information( this, tr ( "Insertion d'une ancre :"),
				          tr ( " une ancre : \r\n")     +
					  ancre_txt                    +
			                  tr(  "\r\n portant ce nom existe déjà\r\n"
				               " Faut-il l'effacer et la remplacer par la nouvelle\r\n"
				               " ou annuler l'opération en cours ??") ,
					  tr("&Remplacer"), tr("Annu&ler"), 0,
					  0, 1 ) ==1 ) {delete  pTxt;     return;}
      CHtmlTools::EffacerAncre(*pTxt, ancre_txt);
      delete pQListViewItem;
     }
  //................ inserer la nouvelle ancre ..................................................
  QString toInsert = "<a name=\"";
  toInsert        +=  ancre_txt;
  toInsert        +="\">";
  toInsert        +="</a>";
  pTxt->replace("~~##µµ##~~", toInsert);

  //................ replacer le texte dans l'editeur ...........................................
  textBrowserView->setText(*pTxt);
  textBrowserView->setCursorPosition (paraFrom, indexFrom );
  textBrowserView->ensureCursorVisible();

  //................ sauver le document modifié ...........................................
  QFile file(m_SavPath);
  if ( file.open( IO_WriteOnly ) )
     {file.writeBlock(*pTxt, pTxt->length());
     }
  delete pTxt;                      // plus besoin

  //................. regenerer la liste des ancres pour contrôle ....................................
  lineEdit_Ancre->setText("");
  listView_Ancres->clear();
  CHtmlTools::ListerAncres(textBrowserView->text(),  listView_Ancres);

}



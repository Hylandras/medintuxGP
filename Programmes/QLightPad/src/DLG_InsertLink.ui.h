/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include "../../MedinTuxTools/CGestIni.h"
#include <qapplication.h>
#include <qcursor.h>

//--------------------------------------- closeEvent ---------------------------------
void Dlg_InsertLink::init()
{m_pDLG_Browser = 0;
}

//--------------------------------------- initDialog ---------------------------------
void Dlg_InsertLink::initDialog( MyEditText *edit , QMAP_EDIT_FNAME &file_names)
{ m_Edit         = edit;
  m_Path         = *file_names.find(edit);
  //............. créer le fichier d'annulation ........................................................    
  if (m_Path.length()==0) m_Path = "noname.html";
  int pos        =  m_Path.findRev(QDir::separator());
  m_SavPath      =  m_Path.left(pos+1);            // inclure le separateur si il y est
  if (pos==0)                                      // si pas de chemin defini alors utiliser celui de l'executable
     {m_SavPath  = CGestIni::Construct_PathBin_Module("QLightPad",  QFileInfo(qApp->argv()[0]).dirPath (true));
      m_SavPath  = m_SavPath.left(m_SavPath .findRev (QDir::separator())+1);
     }
  m_SavPath     += "~" + m_Path.mid(pos+1);    // rajouter un tilde devant le nom du fichier
  QFile file(m_SavPath );	
  if ( file.open( IO_WriteOnly ) )  file.writeBlock(edit->text(), edit->text().length()); 
  //........................ recuperer texte selectionne ..................................................
  QString linkText = edit->selectedText(); 
  CHtmlTools::HtmlToAscii(linkText);
  textEdit_Lien->setText(linkText);
  textEdit_Cible->setText("");
  //........................ lister les liens de cette page ...............................................
  CHtmlTools::ListerLiens(edit->text(), listView_Link);
  //........................ relever toutes les ancres des fichiers presents ..............................  
  QMAP_EDIT_FNAME::Iterator it;
  for ( it = file_names.begin(); it != file_names.end(); ++it ) 
      {QListViewItem* pQListViewItem = new QListViewItem (listView_AllAncres,it.data());     
       if (pQListViewItem)
	  {CHtmlTools::ListerAncres(it.key()->text(), pQListViewItem);
           pQListViewItem->setOpen (TRUE );
	  }
      }
} 

//--------------------------------------- closeEvent ---------------------------------
void Dlg_InsertLink::closeEvent ( QCloseEvent * e )
{if (m_pDLG_Browser) delete m_pDLG_Browser;
 m_pDLG_Browser = 0;
 QFile::remove(m_SavPath );
 e->accept();
}

//--------------------------------------- pushButton_OK_clicked ---------------------------------
void Dlg_InsertLink::pushButton_OK_clicked()
{if (m_pDLG_Browser) delete m_pDLG_Browser;
 m_pDLG_Browser = 0;
 QFile::remove(m_SavPath );
 if (textEdit_Lien->text().length()==0 ||textEdit_Cible->text().length()==0) reject();
 else                                                                        accept();  
}

//--------------------------------------- listView_Link_clicked ---------------------------------
void Dlg_InsertLink::listView_Link_clicked( QListViewItem * )
{ 
}

//--------------------------------------- listView_Link_doubleClicked ---------------------------------
void Dlg_InsertLink::listView_Link_doubleClicked( QListViewItem *pQListViewItem )
{if (pQListViewItem==0) return;
 if (m_pDLG_Browser==0) m_pDLG_Browser = new Dlg_Browser (this, "My_QTextBrowser", FALSE);
 if (m_pDLG_Browser==0) return;
 //.................. rajouter le chemin des pages au lien ................
 int pos = m_SavPath.findRev(QDir::separator()); 
 QString link = pQListViewItem->text(0);
 if (link[0]=='#')         link.prepend(m_SavPath.mid (pos+1));      // rajouter le nom de la page (celle actuelle)
 if (link.find(':')==-1)   link.prepend(m_SavPath.left(pos+1));      // ne rajouter le chemin des pages que si pas lien "http" ou "mailto" etc
 //................ comparer au fichier d'entrée ..........................
 //                 si idem rajouter le tilde au lien car
 //                 le travail se fait sur le provisoire
 pos =    link.find('#'); 
 QString fname = (pos==-1)?link:link.left(pos); // virer l'ancre si elle y est
 if (fname == m_Path)
    {pos =    link.findRev(QDir::separator());  // reperer le debut du nom de fichier
     link.insert(pos+1, "~");                   // y inserer le tilde
     m_pDLG_Browser->textBrowser1->mimeSourceFactory()->addFilePath(m_Edit->m_Path ); 
     CHtmlTools::setDefaultTextAttributs(m_pDLG_Browser->textBrowser1, 
					 m_Edit->m_DefaultBackgroundColor ,
					 m_Edit->m_PathImage.length()?m_Edit->m_Path + QDir::separator() + m_Edit->m_PathImage:0,
					 m_Edit->m_DefaultTextColor,
					 m_Edit->m_DefaultFont);
    }
 m_pDLG_Browser->textBrowser1->setSource(link);  
 m_pDLG_Browser->show();
}

//--------------------------------------- listView_AllAncres_clicked ---------------------------------
void Dlg_InsertLink::listView_AllAncres_clicked( QListViewItem *pQListViewItem )
{if (pQListViewItem==0) return;
 QString fname;
 QString target("#");
 QListViewItem *pQListViewItemParent = pQListViewItem->parent();
 if (pQListViewItemParent ==0) {fname = pQListViewItem->text(0);        target  = "";}
 else                          {fname = pQListViewItemParent->text(0);  target += pQListViewItem->text(0);}
 textEdit_Cible->setText(fname.mid(fname.findRev(QDir::separator())+1) + target);
}

//--------------------------------------- listView_AllAncres_doubleClicked ---------------------------------
void Dlg_InsertLink::listView_AllAncres_doubleClicked( QListViewItem *pQListViewItem )
{if (pQListViewItem==0) return;
 if (m_pDLG_Browser==0) m_pDLG_Browser = new Dlg_Browser (this, "My_QTextBrowser", FALSE);
 if (m_pDLG_Browser==0) return; 
 QString link = MakeLinkString (pQListViewItem);
 
 //................ comparer au fichier d'entrée ..........................
 //                 si idem rajouter le tilde au lien car
 //                 le travail se fait sur le provisoire
 int pos       = link.find('#'); 
 QString fname = (pos==-1)?link:link.left(pos); // virer l'ancre si elle y est
 if (fname == m_Path)
    {pos =    link.findRev(QDir::separator());  // reperer le debut du nom de fichier
     link.insert(pos+1, "~");                   // y inserer le tilde
     m_pDLG_Browser->textBrowser1->mimeSourceFactory()->addFilePath(m_Edit->m_Path ); 
     CHtmlTools::setDefaultTextAttributs(m_pDLG_Browser->textBrowser1, 
					 m_Edit->m_DefaultBackgroundColor ,
					 m_Edit->m_PathImage.length()?m_Edit->m_Path + QDir::separator() + m_Edit->m_PathImage:0,
					 m_Edit->m_DefaultTextColor,
					 m_Edit->m_DefaultFont);
    }
 m_pDLG_Browser->textBrowser1->setSource(link);
 m_pDLG_Browser->show();
}

//---------------------------- MakeLinkString -----------------------------------------------------------------------
// ACTION: part de l'item: QListViewItem *qlistViewItem   et remonte de parents en parents jusqu'à la racine
//         tout en constituant le chemin ainsi formé par les nom de ces items
// SORTIE: QString    contenant le chemin de l'item: QListViewItem *qlistViewItem

QString Dlg_InsertLink::MakeLinkString (QListViewItem *qlistViewItem)
{QString path  = qlistViewItem->text(0);
 qlistViewItem = qlistViewItem->parent();
 while (qlistViewItem!=0)
       {path.insert(0, '#');
        path.insert(0,qlistViewItem->text(0));
	if (qlistViewItem->depth()==0) break;
        qlistViewItem = qlistViewItem->parent();	
       }
 return path; 
}

//---------------------------- pushButtonAddPage_clicked -----------------------------------------------------------------------
void Dlg_InsertLink::pushButtonAddPage_clicked()
{
 //................. choisir le fichier image src à insérer .................................................
  QString fs = QFileDialog::getOpenFileName(
                    m_Path,
                    tr( "HTML-Files (*.htm *.html *.HTML *.HTM);;All Files (*)" ),
                    this,
                    "open file dialog",
                    tr("Choose a HTML file for the target link") );
  if ( fs.isEmpty() )                    return;
  if ( !QFile::exists( fs ) )            return;  
  QFile file( fs );  
  if ( !file.open( IO_ReadOnly ) )       return;

  QString src = QTextStream(&file).read();
  QListViewItem* pQListViewItem = new QListViewItem (listView_AllAncres, fs);     
  if (pQListViewItem)
     {CHtmlTools::ListerAncres(src, pQListViewItem);
      pQListViewItem->setOpen (TRUE );
     }
}

//---------------------------- pushButton_LinkDel_clicked -----------------------------------------------------------------------
void Dlg_InsertLink::pushButton_LinkDel_clicked()
{  QString txt = "";
   QPtrList <QListViewItem> *pQPtrList = new QPtrList <QListViewItem> ;
   pQPtrList->setAutoDelete( TRUE );           // comme � les items s'auto effaceront en sortie de fonction
   QListViewItemIterator it( listView_Link );
   QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
   txt = m_Edit->text();
   while ( it.current() ) 
         {  QListViewItem *pQListViewItem = it.current();
            if (pQListViewItem->isSelected())
               {pQPtrList->append(pQListViewItem);       // on l'ajoute �la liste autodetruite
                CHtmlTools::EffacerLien(txt, atoi(pQListViewItem->text(2)));
               }
            ++it;
        }
  delete pQPtrList;
  m_Edit->setText(txt);
  QApplication::restoreOverrideCursor ();
  listView_Link->clear();
  CHtmlTools::ListerLiens(txt, listView_Link);
 /*    
 QListViewItem *pQListViewItem = listView_Link->currentItem ();
 if (pQListViewItem==0) return;
 QString txt(m_Edit->text());
 CHtmlTools::EffacerLien(txt, atoi(pQListViewItem->text(2)));
 m_Edit->setText(txt);
 listView_Link->clear();
 CHtmlTools::ListerLiens(txt, listView_Link);
 */
}






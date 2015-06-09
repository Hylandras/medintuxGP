/****************************************************************************
 ** ui.h extension file, included from the uic-generated form implementation.
 **
 ** If you wish to add, delete or rename functions or slots use
 ** Qt Designer which will update this file, preserving your code. Create an
 ** init() function in place of a constructor, and a destroy() function in
 ** place of a destructor.
 *****************************************************************************/
 #include <qdir.h>
 #include <qpixmap.h>
 #include <qfile.h>
 #include <qfileinfo.h>
 #include <qfileinfo.h>
#include "../../MedinTuxTools/CGestIni.h"
#include "../../MedinTuxTools/Theme.h"
 //------------------------------ init ----------------------------------------------
 void FormDlgListOff_Listes::init()
 {m_ItemName ="";
  //listBoxListes->header()->hide();
  //.................. signals and slots connections ................................
  connect( but_Annule,    SIGNAL( clicked() ), this, SLOT( but_Annule_clicked() ) );
  connect( but_Valide,    SIGNAL( clicked() ), this, SLOT( but_Valide_clicked() ) );
  connect( lineEdit_Find, SIGNAL( textChanged(const QString&) ), this, SLOT( lineEdit_Find_textChanged(const QString&) ) );

 }

 
//------------------------------ initDialog ----------------------------------------
 void FormDlgListOff_Listes:: setPath(const char* path)
 {initDialog(path);
 }

//------------------------------ initDialog ----------------------------------------
 void FormDlgListOff_Listes::initDialog( QString pathListItem)
 {   if (CGestIni::IsUtf8(pathListItem, pathListItem.length())) pathListItem = QString::fromUtf8(pathListItem);
     QDir dir ( pathListItem );
     dir.convertToAbs();
     dir.setFilter( QDir::All | QDir::NoSymLinks );
     listBoxListes->clear();
     //............ exploration de la liste ......................
     const QFileInfoList * pQFileInfoList = dir.entryInfoList();
     if (pQFileInfoList==0) return ;
     QFileInfoListIterator it( *pQFileInfoList );
     QFileInfo     *fi;
     while ( (fi = it.current()) != 0 )
           {QString fname = fi->fileName().latin1();
           if      (fname == "..")
                   {
                   }
           else if (fi->isDir() && fname != "." && fname.lower() != "cvs")
                   {listBoxListes->insertItem (Theme::getIcon("16x16/folder.png"), fname);
                   }
           else if (fname[0]!='.' && fi->isFile())
                   {QString ext;
                    int pos_ext  = fname.findRev('.');
                    if (pos_ext != -1)
                       {listBoxListes->insertItem (Theme::getIcon("16x16/html.png") ,fname);
                       }
                    else
                       {listBoxListes->insertItem (Theme::getIcon("16x16/item.png"),fname);
                       }
                   }
            ++it;
           }
  m_ItemName ="";
 }
 //------------------------------ lineEdit_Find_textChanged ----------------------------------------
 void FormDlgListOff_Listes::lineEdit_Find_textChanged(const QString& text_in)
{if (text_in.length()==0) return;
 QListBoxItem * pQListBoxItem = 0;
 QString text = text_in;
 if (text.startsWith("*") && text.endsWith("*"))
    {pQListBoxItem= listBoxListes->findItem (text.remove('*'), Qt::Contains );
    }
 else if (text.endsWith("*"))
    {pQListBoxItem= listBoxListes->findItem (text.remove('*'), Qt::EndsWith );
    }
 else
    {pQListBoxItem= listBoxListes->findItem (text.remove('*'), Qt::Contains );
    }
 if (pQListBoxItem)
    {listBoxListes->setCurrentItem (pQListBoxItem ) ;
     listBoxListes->setSelected ( pQListBoxItem, TRUE ); 
     listBoxListes->ensureCurrentVisible () ;
    }
}

 //------------------------------ listBoxListes_clicked ----------------------------------------
 void FormDlgListOff_Listes::listBoxListes_clicked( QListBoxItem * pQListBoxItem)
 {if ( pQListBoxItem ==0) m_ItemName ="";
  else                    m_ItemName = pQListBoxItem->text ();
  accept();
 }


 //------------------------------ listBoxListes_returnPressed ----------------------------------------
 void FormDlgListOff_Listes::listBoxListes_returnPressed( QListBoxItem *pQListBoxItem )
 {listBoxListes_clicked(pQListBoxItem);
 }


 //------------------------------ but_Annule_clicked ----------------------------------------
void FormDlgListOff_Listes::but_Annule_clicked()
{ reject();
}


 //------------------------------ but_Valide_clicked ----------------------------------------
void FormDlgListOff_Listes::but_Valide_clicked()
{ if (listBoxListes->selectedItem () ) m_ItemName = listBoxListes->selectedItem()->text();
  else m_ItemName = "";
  accept();
}

 //------------------------------ getSelectedItem ----------------------------------------
QString FormDlgListOff_Listes::getSelectedItem()
{ return m_ItemName ;
}

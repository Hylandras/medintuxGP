/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

//------------------------------ init ----------------------------------------
void Dlg_InsertList::init()
{  //.................. signals and slots connections ................................
  connect( pushButtonOk,          SIGNAL( clicked() ),                      this, SLOT( pushButtonOk_clicked() ) );
  connect( lineEdit_Find,         SIGNAL( textChanged(const QString&) ),    this, SLOT( lineEdit_Find_textChanged(const QString& ) ) );
  connect( listBox_InsertList,    SIGNAL( returnPressed( QListBoxItem *) ), this, SLOT( listBox_InsertList_returnPressed( QListBoxItem * ) ) );
  connect( listBox_InsertList,    SIGNAL( doubleClicked( QListBoxItem *) ), this, SLOT( listBox_InsertList_doubleClicked( QListBoxItem * ) ) );
  lineEdit_Find->setFocus();
}

//------------------------------ initDialog ----------------------------------------
void Dlg_InsertList::initDialog(QStringList &pQStringList)
{initDialogField(pQStringList, -1, "S");
}

//------------------------------ initDialogField ----------------------------------------
void Dlg_InsertList::initDialogField(QStringList &pQStringList, int nbField, const QString &modeSel)
{if (modeSel=="M") listBox_InsertList->setSelectionMode ( QListBox::Extended ) ;
  for ( QStringList::Iterator it = pQStringList.begin(); it != pQStringList.end(); ++it )
    {QString toInsert = *it;
        if (nbField > 0)
        {int   i = 0;
         int pos = 0;
         while ( i<nbField && (pos = toInsert.find("::", pos+2)) != -1)
               {++i;
               };
         if (pos != -1) toInsert = toInsert.left(pos);
        }
     listBox_InsertList->insertItem (toInsert.replace("::"," "));
    }
}
//------------------------------ lineEdit_Find_textChanged ----------------------------------------
void Dlg_InsertList::lineEdit_Find_textChanged(const QString& text_in)
{if (text_in.length()==0) return;
 QListBoxItem * pQListBoxItem = 0;
 QString text = text_in;
 if (text.startsWith("*") && text.endsWith("*"))
    {pQListBoxItem= listBox_InsertList->findItem (text.remove('*'), Qt::Contains );
    }
 else if (text.endsWith("*"))
    {pQListBoxItem= listBox_InsertList->findItem (text.remove('*'), Qt::EndsWith );
    }
 else
    {pQListBoxItem= listBox_InsertList->findItem (text.remove('*'), Qt::BeginsWith );
    }
 if (pQListBoxItem)
    {listBox_InsertList->setCurrentItem (pQListBoxItem ) ;
     listBox_InsertList->setSelected ( pQListBoxItem, TRUE ); 
     listBox_InsertList->ensureCurrentVisible () ;
    }
}

//------------------------------ listBox_InsertList_doubleClicked ----------------------------------------
void Dlg_InsertList::listBox_InsertList_doubleClicked( QListBoxItem *pQListBoxItem )
{if (pQListBoxItem) accept();
}

//------------------------------ pushButtonOk_clicked ----------------------------------------
void Dlg_InsertList::pushButtonOk_clicked()
{accept();
}

//------------------------------ listBox_InsertList_returnPressed ----------------------------------------
void Dlg_InsertList::listBox_InsertList_returnPressed( QListBoxItem *pQListBoxItem )
{if (pQListBoxItem) accept();
}

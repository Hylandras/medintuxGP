/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

void Dlg_InsertList::initDialog(QStringList &pQStringList)
{listBox_InsertList->insertStringList (pQStringList);
 /*
 for ( QStringList::Iterator it = pQStringList->begin(); it != pQStringList->end(); ++it ) 
     {listBox_InsertList->insertItem (*it);       
     }
*/
}




void Dlg_InsertList::listBox_InsertList_doubleClicked( Q3ListBoxItem *pQListBoxItem )
{if (pQListBoxItem) accept();
}




void Dlg_InsertList::pushButtonOk_clicked()
{accept();
}


void Dlg_InsertList::listBox_InsertList_returnPressed( Q3ListBoxItem *pQListBoxItem )
{if (pQListBoxItem) accept();
}

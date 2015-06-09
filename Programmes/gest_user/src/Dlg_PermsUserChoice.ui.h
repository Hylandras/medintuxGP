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
//--------------------------------- init -------------------------------------------------------------
void Dlg_PermsUserChoice::init()
{QString dumy("");
 //G_pCApp->m_pCMoteurBase->GetUserList( ListView_UserList, dumy, dumy, FALSE);


 radioButtonCommence->setChecked (TRUE);
 radioButtonContient->setChecked (FALSE);
 lineEditNom->setFocus();
}
//--------------------------------- initDialog -------------------------------------------------------------
void Dlg_PermsUserChoice::initDialog(CMoteurBase *pCMoteurBase, QStringList* pUserList, const QString &uniqueUser)
{m_pUserList    = pUserList;                   // pointeur sur liste des utilisateurs deja presents
 m_pCMoteurBase = pCMoteurBase;
 m_pCMoteurBase->GetUserList(ListView_UserList, *m_pUserList,"", uniqueUser);
 m_pUserList->clear();
 QStringList userList;
 m_pCMoteurBase->GetUserListType(userList);
 comboBox_Specialites->insertStringList (userList);
 comboBox_Specialites->insertItem ( tr("Toutes spécialités"), 0 );
 QListViewItem *pcurrentItem =  ListView_UserList->firstChild();
 if (pcurrentItem>0)
    {ListView_UserList->setCurrentItem(pcurrentItem);
     ListView_UserList->setSelected (pcurrentItem, TRUE );
    }
}

//--------------------------------- pushButtonOk_clicked -------------------------------------------------------------
void Dlg_PermsUserChoice::pushButtonOk_clicked()
{QListViewItemIterator it( ListView_UserList );
 while ( it.current() )
       {if ( it.current()->isSelected() ) m_pUserList->append(it.current()->text(3));  // liste de PK des user
        ++it;
       }
 accept();
}

//--------------------------------- setListViewOnUser -------------------------------------------------------------
void Dlg_PermsUserChoice::setListViewOnUser(const QString &user)
{

 QListViewItem *pQListViewItem =  ListView_UserList->findItem ( user, 0, Qt::ExactMatch);
 if ( pQListViewItem )
    {ListView_UserList->clearSelection ();
     lineEditNom->setText(user);
     ListView_UserList->setCurrentItem ( pQListViewItem );
     ListView_UserList->setSelected ( pQListViewItem, TRUE );
     ListView_UserList->ensureItemVisible ( pQListViewItem );
    }
}


//--------------------------------- pushButtonAnnuler_clicked -------------------------------------------------------------
void Dlg_PermsUserChoice::pushButtonAnnuler_clicked()
{reject();
}

//--------------------------------- ListView_UserList_doubleClicked -------------------------------------------------------------
void Dlg_PermsUserChoice::ListView_UserList_doubleClicked( QListViewItem*)
{pushButtonOk_clicked();
}


//--------------------------------- lineEditNom_textChanged -------------------------------------------------------------
void Dlg_PermsUserChoice::lineEditNom_textChanged( const QString &text )
{
 QListViewItem *pQListViewItem = 0;
 if (radioButtonCommence->isChecked ())
     pQListViewItem = ListView_UserList->findItem ( text, 0, Qt::BeginsWith);
 else
     pQListViewItem = ListView_UserList->findItem ( text, 0, Qt::Contains);
 if (pQListViewItem == 0) return;
 ListView_UserList->setSelected ( pQListViewItem, TRUE );
 ListView_UserList->ensureItemVisible ( pQListViewItem );
}


//--------------------------------- comboBox_Specialites_activated -------------------------------------------------------------
void Dlg_PermsUserChoice::comboBox_Specialites_activated( const QString &str )
{ListView_UserList->clear();
 if (str==tr("Toutes spécialités")) m_pCMoteurBase->GetUserList(ListView_UserList, *m_pUserList);
 else                                 m_pCMoteurBase->GetUserList(ListView_UserList, *m_pUserList, str);
}

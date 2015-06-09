
/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
//----------------------------- initDialog -----------------------------------------------------
// mode = 1   => modification du mot de passe
// mode = 2   => juste saisie du mot de passe
#define PASS_CHANGE    1
#define PASS_GET       2


void DlgPassChange::initDialog(CMoteurBase* pCMoteurBase, const char* user)
{m_pCMoteurBase  =  pCMoteurBase;
 m_User          =  user;
 m_PassWord      = "";
 QString dumy("");
 QString userQualite="";
 setCaption(tr("Choix de l'utilisateur"));

 lineEdit_User->setText(m_User);
 lineEdit_Pass->setText(m_PassWord);
 m_pCMoteurBase->GetUserList(ListView_UserList, dumy, dumy, TRUE, &userQualite  );
 QListViewItem *pQListViewItem = ListView_UserList->findItem(m_User,0);
 if (pQListViewItem)
    {ListView_UserList->setCurrentItem ( pQListViewItem );
     ListView_UserList->setSelected ( pQListViewItem , TRUE );
    }
 else
    {pQListViewItem = ListView_UserList->firstChild ();
     if (pQListViewItem)
        {ListView_UserList->setCurrentItem ( pQListViewItem );
         ListView_UserList->setSelected ( pQListViewItem , TRUE );
	 On_ListView_UserList_Clicked(pQListViewItem);
        }
    }
 connect( ListView_UserList, SIGNAL( clicked(QListViewItem *) ), this, SLOT( On_ListView_UserList_Clicked(QListViewItem *) ) );
 lineEdit_Pass->setFocus();
}


void DlgPassChange::On_ListView_UserList_Clicked(QListViewItem *pQListViewItem)
{if (pQListViewItem==0) return;
 m_User          =  pQListViewItem->text(0);
 m_PassWord      = "";
 lineEdit_User->setText(m_User);
 lineEdit_Pass->setText(m_PassWord);
 lineEdit_Pass->setFocus();
}



void DlgPassChange::lineEdit_User_textChanged( const QString &qstr )
{//if (qstr != lineEdit_Pass->text()) pushButtonOK->setEnabled(FALSE);
 //else                               pushButtonOK->setEnabled(TRUE);
}



void DlgPassChange::lineEdit_Pass_textChanged( const QString &qstr )
{//if (m_Mode==PASS_GET) return;
 //if (qstr != lineEditPass1->text()) pushButtonOK->setEnabled(FALSE);
 //else                               pushButtonOK->setEnabled(TRUE);
}



void DlgPassChange::pushButtonOK_clicked()
{
  m_PassWord = lineEdit_Pass->text();                      // recuperer saisie non criptée
  m_User     = lineEdit_User->text();
 //m_pCMoteurBase->PassWordEncode(m_PassWord );             // encoder le password
 //m_pCMoteurBase->SaveUserPassWord(m_User, m_PassWord );   // le sauvegarder dans la base
 accept();
}




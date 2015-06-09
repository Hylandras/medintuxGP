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

void DlgPassChange::initDialog(CMoteurBase* pCMoteurBase, const char* user, const char* pass, int mode)
{m_pCMoteurBase  =  pCMoteurBase;
 m_User          =  user;
 m_PassWord      =  pass;
 m_Mode          =  mode;
 if (m_Mode==PASS_GET)
    {textLabel2->hide();
     lineEditPass2->hide();
     lineEditPass1->setFocus();
     setCaption(tr("Entrez le mot de passe pour: ")   + user);
    }
 else if (m_Mode==USER_PASS_GET       )
    {setCaption(tr("Entrez vos identifiants "));
     lineEditPass1->setText(user);
     if (m_User.length()) lineEditPass2->setFocus();
     else                 lineEditPass1->setFocus();
     lineEditPass1->setEchoMode ( QLineEdit::Normal );
     textLabel1->setText(tr("Utilisateur  "));
     textLabel2->setText(tr("Mot de passe "));
    }
 else
    { setCaption(tr("Modifiez le mot de passe de: ") + user);
      lineEditPass1->setFocus();
      checkBoxMemorise->hide();
    }
}

void DlgPassChange::initDialog(CMoteurBase* pCMoteurBase, const char* user, int mode)
{ initDialog(pCMoteurBase,  user,"", mode);
}


void DlgPassChange::lineEditPass1_textChanged( const QString &qstr )
{if (m_Mode>PASS_CHANGE) return;
 if (qstr != lineEditPass2->text()) pushButtonOK->setEnabled(FALSE);
 else                               pushButtonOK->setEnabled(TRUE);
}


void DlgPassChange::lineEditPass2_textChanged( const QString &qstr )
{if (m_Mode>PASS_CHANGE ) return;
 if (qstr != lineEditPass1->text()) pushButtonOK->setEnabled(FALSE);
 else                               pushButtonOK->setEnabled(TRUE);
}


void DlgPassChange::pushButtonOK_clicked()
{
 m_PassWord = lineEditPass1->text();                      // recuperer saisie non criptée
 if (m_Mode==USER_PASS_GET )
    {m_User     = lineEditPass1->text();
     m_PassWord = lineEditPass2->text();
    }
 if (m_Mode==PASS_CHANGE)
    {//m_pCMoteurBase->PassWordEncode(m_PassWord );             // encoder le password
     //m_pCMoteurBase->SaveUserPassWord(m_User, m_PassWord );   // le sauvegarder dans la base
    }
 accept();
}



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
void Dlg_NewUser::init()
{QString path_img = G_pCApp->m_PathAppli + "Images" + QDir::separator();
 labelIconePs->setPixmap ( QPixmap(path_img +"Carte_CPS.png"));
 lineEdit_Login->setFocus();
}

void Dlg_NewUser::initDialog(CMoteurBase *pCMoteurBase,const QString &cps, QString *login, QString *passWord)
{m_pCMoteurBase = pCMoteurBase;
 m_pPassWord    = passWord;
 m_pLogin        = login;
 lineEdit_Login->setText(*login);
 lineEdit_Pass_one->setText(*passWord);
 lineEdit_Pass_conf->setText(*passWord);
 textEdit_infos->setText(cps);
}

void Dlg_NewUser::pushButtonOk_clicked()
{QString passWordA  = lineEdit_Pass_one->text();
 QString passWordB  = lineEdit_Pass_conf->text();
 if (lineEdit_Login->text().length()==0)
    {QMessageBox::critical( this, tr ("Erreur dans la saisie du Login"),
                                  tr ("Le login doit impérativement faire au moins un caractère !!") ,
                                  tr("&Annuler"),0, 0,
                            1, 1
			  );
     //reject();
    }
 else if (m_pCMoteurBase->GetUserPrimKey(lineEdit_Login->text()).length()>0)
    {QMessageBox::critical( this, tr ("Erreur dans la saisie du Login"),
                                  tr ("Le login choisi existe déjà, veuillez recommencer") ,
                                  tr("&Annuler"),0, 0,
                            1, 1
			  );
     //reject();
    }
 else if (passWordA.length()== 0 && passWordB.length()== 0)
    {*m_pPassWord  = "";
     *m_pLogin     = lineEdit_Login->text();
     accept();
    }
 else if (passWordA != passWordB)
    {
     QMessageBox::critical( this, tr ("Erreur dans la saisie du mot de passe"),
                                  tr ("Le mot de passe et celui de confirmation ne sont pas les mêmes") ,
                                  tr("&Annuler"),0, 0,
                            1, 1
			  );
     //reject();
    }
 else
    {
     *m_pLogin     = lineEdit_Login->text();
     *m_pPassWord  = lineEdit_Pass_conf->text();
     accept();
    }
}

void Dlg_NewUser::pushButtonCancel_clicked()
{reject();
}

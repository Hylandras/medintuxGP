//
// C++ Implementation:
//
// Description:
//
//
// Author: Sevin Roland <upsis@club-internet.fr>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <qsplitter.h>

    #include "C_Dlg_ChoixUser.h"
    #include "CMoteur_Base.h"
    #include "Global.h"
    #include "qfiledialog.h"
    #include "qprocess.h"
    #include "qdir.h"
    #include "qmessagebox.h"
    #include "CApp.h"


//-------------------------------------------- C_Dlg_ChoixUser ---------------------------------------
C_Dlg_ChoixUser::C_Dlg_ChoixUser(CMoteurBase* pCMoteurBase, QString user, QString sign_user, QString *pParam,
                                 QWidget* parent, const char* name, bool modal, WFlags fl)
: DlgChoixUser(parent,name, modal,fl)
{
    // signals and slots connections
    connect( pushButtonAnnuler, SIGNAL( clicked() ), this, SLOT( pushButtonAnnuler_clicked() ) );
    connect( pushButtonOK, SIGNAL( clicked() ), this, SLOT( pushButtonOK_clicked() ) );
    connect( ListView_UserList, SIGNAL( clicked(QListViewItem*) ), this, SLOT( ListView_UserList_clicked(QListViewItem*) ) );
    connect( ListView_UserList, SIGNAL( doubleClicked(QListViewItem*) ), this, SLOT( ListView_UserList_doubleClicked(QListViewItem*) ) );
    connect( pushButtonConfig, SIGNAL( clicked() ), this, SLOT( pushButtonConfig_clicked() ) );
    connect( lineEditPassWord, SIGNAL( textChanged(const QString&) ), this, SLOT( lineEditPassWord_textChanged(const QString&) ) );

 setProperty( "icon", QPixmap( G_pCApp->m_PathImages + "icones/pingoin_tete.png" ) );
 m_pCMoteurBase               = pCMoteurBase;
 m_User                       = user;
 m_SignUser                   = sign_user;
 m_pParam                     = pParam;
 QListViewItem *qlistViewItem = 0;
 if (m_pCMoteurBase==0) return;
 QString dumy("");
 m_pCMoteurBase->GetUserList( ListView_UserList, dumy, dumy);
 qlistViewItem = getUserItem(user, sign_user);
setCaption(tr("Utilisateur en cours : ") + user+"::"+sign_user);
 if (qlistViewItem)
    {ListView_UserList->setCurrentItem (qlistViewItem );
     ListView_UserList->setSelected (qlistViewItem, TRUE );

     lineEditPassWord->setFocus ();
   }
 else
   {qlistViewItem = ListView_UserList->firstChild();
    if (qlistViewItem>0)
       {ListView_UserList->setCurrentItem(qlistViewItem);
        ListView_UserList->setSelected (qlistViewItem, TRUE );
       }
   }

 //.................... desactiver changement d'utilisateur si ....................
 //                     un mot de passe existe pour lui
 //                     ne sera reactivé que lorsque il sera correct
 QString cripted_password;
 if (m_pCMoteurBase->GetUserPrimKey(m_User, &cripted_password) != "")
    {if (cripted_password !="")
        {pushButtonConfig->setEnabled(FALSE);
         pushButtonOK->setEnabled(FALSE);
        }
    }

//.................... positionner le spliter....................
 QValueList <int> list;
 int w = width() / 3;
 list.append(w);
 list.append(width() - w);
 splitterCentral->setSizes (list );
}

//-------------------------------------------- C_Dlg_ChoixUser ---------------------------------------
C_Dlg_ChoixUser::~C_Dlg_ChoixUser()
{
}

/*$SPECIALIZATION$*/
//-------------------------------------------- getUserItem ---------------------------------------
QListViewItem * C_Dlg_ChoixUser::getUserItem( const QString & user, const QString & sign_user )
{QListViewItemIterator it( ListView_UserList );
    while ( it.current() )
          {QListViewItem *pQListViewItem = it.current();
           if ( pQListViewItem->text(0) == user )
              { QListViewItem *pQListViewItemParent = pQListViewItem->parent();
                if (pQListViewItemParent==0) pQListViewItemParent = pQListViewItem;
                if (pQListViewItemParent->text(0) == sign_user) return pQListViewItem;
              }
            ++it;
           } //endwhile ( it.current() )
    return 0;
}

//-------------------------------------------- getSignUser ---------------------------------------
QListViewItem * C_Dlg_ChoixUser::getSignUser( QListViewItem * pQListViewItem )
{
 if (pQListViewItem)
    {QListViewItem *pQListViewItemParent = pQListViewItem->parent();
     if (pQListViewItemParent==0) return pQListViewItem;
     else                         return pQListViewItemParent;
    }
 return 0;
}

//------------------------------------ getSignUser --------------------------------------
QListViewItem * C_Dlg_ChoixUser::getSignUser()
{return getSignUser( ListView_UserList->selectedItem());
}

//-------------------------------------------- lineEditPassWord_textChanged ---------------------------------------
void C_Dlg_ChoixUser::lineEditPassWord_textChanged( const QString & password )
{QListViewItem * qlistViewItem = ListView_UserList->currentItem ( );
 if (qlistViewItem == 0)  return;
 QString user   = qlistViewItem->text(0);

 if (m_pCMoteurBase->VerifyUserPassWord(user, password))
    {pushButtonConfig->setEnabled(TRUE);
     pushButtonOK->setEnabled(TRUE);
    }
 else
    {pushButtonConfig->setEnabled(FALSE);
     pushButtonOK->setEnabled(FALSE);
    }
}

//-------------------------------------------- pushButtonConfig_clicked ---------------------------------------
void C_Dlg_ChoixUser::pushButtonConfig_clicked()
{       QListViewItem * qlistViewItem = ListView_UserList->currentItem ( );
        if (qlistViewItem)      m_User = qlistViewItem->text(0);
        QString cripted_password;
        m_pCMoteurBase->GetUserPrimKey(m_User, &cripted_password);

        QString  fileName = CGestIni::Construct_Name_Exe("gest_user", QFileInfo (qApp->argv()[0]).dirPath (true));
        QProcess*  proc = new QProcess( this );
        if (proc == 0)                        return;
        proc->addArgument( fileName );
        proc->addArgument( m_User );                      // Nom de l'utilisateur
        proc->addArgument( QString("#") + cripted_password );
        proc->start();
        delete proc;

}

//-------------------------------------------- ListView_UserList_doubleClicked ---------------------------------------
void C_Dlg_ChoixUser::ListView_UserList_doubleClicked( QListViewItem * pQListViewItem )
{if (pQListViewItem<=0) return;
 m_User = pQListViewItem->text(0);
 if (m_User !="") accept();
 else             reject();
}

//-------------------------------------------- ListView_UserList_clicked ---------------------------------------
void C_Dlg_ChoixUser::ListView_UserList_clicked( QListViewItem * pQListViewItem )
{if (pQListViewItem<=0) return;

 m_User = pQListViewItem->text(0);
 QListViewItem *pQListViewItemParent = pQListViewItem->parent();
 if (pQListViewItemParent==0) pQListViewItemParent = pQListViewItem;
 m_SignUser = pQListViewItemParent->text(0);

 lineEditPassWord->setText("");
 lineEditPassWord->setFocus ();
 lineEditPassWord_textChanged("");
 setCaption(tr("Utilisateur en cours : ") + m_User+"::"+m_SignUser);
}

//-------------------------------------------- pushButtonOK_clicked ---------------------------------------
void C_Dlg_ChoixUser::pushButtonOK_clicked()
{QListViewItem   *pQListViewItem = ListView_UserList->currentItem ( );
 if (pQListViewItem<=0) return;

 m_User = pQListViewItem->text(0);
 QListViewItem *pQListViewItemParent = pQListViewItem->parent();
 if (pQListViewItemParent==0) pQListViewItemParent = pQListViewItem;
 m_SignUser = pQListViewItemParent->text(0);

 if (pushButtonConfig->isEnabled () == TRUE)
    {if (m_User !="" && m_SignUser !="") accept();
     else                                reject();
    }
 else
    {QMessageBox::critical( this, tr("Erreur : Mot de passe"),
                                  tr("Mot de passe incorrect veuillez recommencer") ,
                                  tr("Annuler")
                          ) ;
    }
}

//-------------------------------------------- pushButtonAnnuler_clicked ---------------------------------------
void C_Dlg_ChoixUser::pushButtonAnnuler_clicked()
{reject();
}

/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include "../../MedinTuxTools/Theme.h"  // Gestion du thème de l'appli

//------------------------------------ init --------------------------------------
void DockFormUserList::init()
{m_pQPopupMenu                 = 0;
 m_CurrentPopupItem            = "";
 m_RetPopupItem                = "";
 m_pQListViewLastSelectedItem  = 0;
 m_pCMoteurBase                = G_pCApp->m_pCMoteurBase;
 m_pFlashQTimer                = 0;
 /*
 16x16/delete_Icon.png
 16x16/SaveProfile.png
 16x16/LoadProfile.png
 16x16/NewWithExistant.png
 16x16/groupe.png
 16x16/Left.png
 16x16/Down.png
 32x32/pingoin_tete.png
 */
 pushButtonUserDelete->setIconSet ( QIconSet (Theme::getIcon("16x16/delete_Icon.png"))) ;
 pushButtonNewUser->setIconSet ( QIconSet (Theme::getIcon("32x32/pingoin_tete.png"))) ;
 pushButtonSetDefault->setIconSet ( QIconSet (Theme::getIcon("16x16/Left.png"))) ;
 pushButton_Appartenance->setIconSet ( QIconSet (Theme::getIcon("16x16/groupe.png"))) ;
 pushButton_NewWhithExistant->setIconSet ( QIconSet (Theme::getIcon("16x16/NewWithExistant.png"))) ;
 pushButton_PlaceProfil->setIconSet ( QIconSet (Theme::getIcon("16x16/ProfilGroupe.png"))) ;
 pushButton_NewFromFileOnDisk->setIconSet ( QIconSet (Theme::getIcon("16x16/loadIdent.png"))) ;
 pushButton_SaveIdentOnDisk->setIconSet ( QIconSet (Theme::getIcon("16x16/saveIdent.png"))) ;
 InitUserList();
 SetUnLogged();
 //.............................. si un utilisateur est déjà identifié au démarrage .........................
#ifdef SESAMVITALE_VERSION
 if ( G_pCApp->IsAppStartWithUser() || G_pCApp->m_pCps!=0)
    { QObject::disconnect ( ListView_UserList, SIGNAL(selectionChanged(QListViewItem *)), this, SLOT(ListView_UserList_selectionChanged (QListViewItem *)));
      if (G_pCApp->m_pCps==0) m_pQListViewLastSelectedItem = SetUser(G_pCApp->m_User);
      connect( ListView_UserList,        SIGNAL( selectionChanged(QListViewItem *) ),     this, SLOT(ListView_UserList_selectionChanged (QListViewItem *)) );
      SetLogged();
    }
#else
 if ( G_pCApp->IsAppStartWithUser())
    { QObject::disconnect ( ListView_UserList, SIGNAL(selectionChanged(QListViewItem *)), this, SLOT(ListView_UserList_selectionChanged (QListViewItem *)));
      m_pQListViewLastSelectedItem = SetUser(G_pCApp->m_User);
      connect( ListView_UserList,        SIGNAL( selectionChanged(QListViewItem *) ),     this, SLOT(ListView_UserList_selectionChanged (QListViewItem *)) );
      SetLogged();
    }
#endif
}

//------------------------------------ SetUnLogged --------------------------------------
void DockFormUserList::SetUnLogged()
{QListViewItem *pcurrentItem =  ListView_UserList->firstChild();
 if (pcurrentItem>0)
    {ListView_UserList->setCurrentItem( pcurrentItem );
     ListView_UserList->setSelected (pcurrentItem, FALSE );
     m_pQListViewLastSelectedItem  = 0;
     pushButtonUserDelete->hide();
     pushButtonNewUser->hide();
     pushButtonSetDefault->hide();
     pushButton_Appartenance->hide();
     pushButton_NewWhithExistant->hide();
     pushButton_PlaceProfil->hide();

     textLabelMessage->show();
     if (m_pFlashQTimer==0) m_pFlashQTimer = new QTimer( this , "timer_SetUnLogged");
     if (m_pFlashQTimer )
        {connect( m_pFlashQTimer, SIGNAL(timeout()), this, SLOT(timerDone()) );
         m_pFlashQTimer->start( 300 ); // 2 seconds
        }
    }
}
//------------------------------------ SetLogged --------------------------------------
void DockFormUserList::SetLogged()
{    if (m_pFlashQTimer!=0) {delete m_pFlashQTimer; m_pFlashQTimer = 0;}
     pushButtonUserDelete->show();
     pushButton_Appartenance->show();
     pushButtonNewUser->show();
     pushButtonSetDefault->show();
     pushButton_NewWhithExistant->show();
     pushButton_PlaceProfil->show();

     textLabelMessage->hide();
}
//------------------------------------ timerDone ---------------------------------------------
void DockFormUserList::timerDone()
{if (textLabelMessage->paletteForegroundColor()==QColor("red"))
     textLabelMessage->setPaletteForegroundColor ( QColor("black") );
 else
     textLabelMessage->setPaletteForegroundColor ( QColor("red") );
}

//------------------------------------ pushButtonSetDefault_clicked --------------------------------------
void DockFormUserList::pushButtonSetDefault_clicked()
{QString data_ini="";
 pushButtonSetDefault->setIconSet ( QIconSet (Theme::getIcon("16x16/Down.png"))) ;
 //.................. creer liste des items du menu .......................
 QStringList popItem;
 popItem.append(tr("Lancement &automatique du programme avec l'utilisateur : ")+ G_pCApp->m_User);
 popItem.append(tr("Annuler le lancement automatique du programme avec un utilisateur"));
 QString popRet       = DoPopupList(popItem);
 CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathIni , data_ini);
 if (popRet==tr("Annuler le lancement automatique du programme avec un utilisateur"))
    {CGestIni::Param_WriteParam( &data_ini, "last login", "login data",  "", "");
    }
 else if (popRet.length())
    {CGestIni::Param_WriteParam( &data_ini, "last login", "login data",  G_pCApp->m_User, QString("#") + G_pCApp->m_CriptedPassWord);
    }
 CGestIni::Param_UpdateToDisk(G_pCApp->m_PathIni, data_ini);
 pushButtonSetDefault->setIconSet ( QIconSet (Theme::getIcon("16x16/Left.png"))) ;
}
//------------------------------------ ListView_UserList_selectionChanged --------------------------------------
void DockFormUserList::ListView_UserList_selectionChanged( QListViewItem *pQListViewItem )
{QObject::disconnect ( ListView_UserList, SIGNAL(selectionChanged(QListViewItem *)), this, SLOT(ListView_UserList_selectionChanged (QListViewItem *)));
 ListView_UserList_LineChanged( pQListViewItem );
 connect( ListView_UserList,        SIGNAL( selectionChanged(QListViewItem *) ),
           this,                     SLOT(ListView_UserList_selectionChanged (QListViewItem *))
        );
}

//------------------------------------ ListView_UserList_LineChanged --------------------------------------
void DockFormUserList::ListView_UserList_LineChanged( QListViewItem *pQListViewItem )
{if (pQListViewItem<=0) return;
 QListViewItem * pQListViewItemParent = getSignUser(pQListViewItem);
 if (G_pCApp->AuthentifyAndSelectUser(this, pQListViewItem->text(0), pQListViewItemParent->text(0)))
     {emit Sign_UserSelected(pQListViewItem->text(3)); // PrimKey
      SetLogged();
      m_pQListViewLastSelectedItem  = pQListViewItem;     // memoriser le  dernier item selectionneé pour le replacer si echec lors d'un changement futur d'utilisateur
     }
 else
    {m_pQListViewItemTmp = pQListViewItem;                        // faut bien que le slot : RestaureOldUser() connaisse la valeur du pQListViewItem
     QTimer::singleShot( 50, this, SLOT(RestaureOldUser()) );     // obligatoire sinon boucle ....
    }
}
//------------------------------------ getSignUser --------------------------------------
QListViewItem *DockFormUserList::getSignUser(QListViewItem *pQListViewItem)
{
 if (pQListViewItem)
    {QListViewItem *pQListViewItemParent = pQListViewItem->parent();
     if (pQListViewItemParent==0) return pQListViewItem;
     else                         return pQListViewItemParent;
    }
 return 0;
}
//------------------------------------ RestaureOldUser --------------------------------------
void DockFormUserList::RestaureOldUser()
{    QObject::disconnect ( ListView_UserList, SIGNAL(selectionChanged(QListViewItem *)), this, SLOT(ListView_UserList_selectionChanged (QListViewItem *)));
     if (m_pQListViewLastSelectedItem)
        {ListView_UserList->setSelected (m_pQListViewLastSelectedItem, TRUE );
         ListView_UserList->ensureItemVisible(m_pQListViewLastSelectedItem);
        }
     else
        {ListView_UserList->setSelected (m_pQListViewItemTmp, FALSE );
        }
     connect( ListView_UserList,         SIGNAL( selectionChanged(QListViewItem *) ),
              this,                     SLOT(ListView_UserList_selectionChanged (QListViewItem *))
            );
}
//--------------------------------- DoPopupList ----------------------------------------------------------------------
QString DockFormUserList::DoPopupList(QStringList &list)
{m_CurrentPopupItem = "";
 m_RetPopupItem     = "";
 if (m_pQPopupMenu)          return m_RetPopupItem;
 m_pQPopupMenu      = new QPopupMenu(0, "MyPopupMenu" );
 if (m_pQPopupMenu==0)       return m_RetPopupItem;
 //............................... chercher une zone de liste deroulante (bleue) .....................
 for ( QStringList::Iterator it =list.begin(); it != list.end(); ++it )
    { if (*(it)=="---------")  m_pQPopupMenu->insertSeparator();
      else                     m_pQPopupMenu->insertItem( *(it),    this, SLOT( GetPopupItem()),  0 );
    }
 //................. connecter le menu .................................................................
 connect( m_pQPopupMenu, SIGNAL( activated( int)),
          this,          SLOT(OnMenuActivated( int))
        );

 m_pQPopupMenu->exec( QCursor::pos() /* mapToGlobal (  QPoint ( x, y) )*/ );
 delete  m_pQPopupMenu;
 m_pQPopupMenu = 0;
 return  m_RetPopupItem;
}

//--------------------------------- OnMenuActivated  -------------------------------------------------------------
void DockFormUserList::OnMenuActivated (int id)
{if (id != -1 && m_pQPopupMenu)
    {m_CurrentPopupItem = m_pQPopupMenu->text(id);
    }
}

//--------------------------------- GetPopupItem  -------------------------------------------------------------
void DockFormUserList::GetPopupItem ()
{m_RetPopupItem  = m_CurrentPopupItem;
}
//------------------------------------ InitUserList --------------------------------------
void DockFormUserList::InitUserList()
{QString dumy("");
 m_pCMoteurBase->GetUserList( ListView_UserList, dumy, dumy, FALSE);
 QListViewItem *pcurrentItem =  ListView_UserList->firstChild();
 if (pcurrentItem>0)
    {ListView_UserList->setCurrentItem(pcurrentItem);
   }
}

//------------------------------------ SetUser --------------------------------------
QListViewItem *DockFormUserList::SetUser(const QString &user)
{QListViewItem *qlistViewItem=0;
 if (user.length())
    {qlistViewItem  =  ListView_UserList->findItem (user, 0 );
    }
 else return 0;
 if (qlistViewItem)
    {ListView_UserList->setCurrentItem (qlistViewItem );
   }
 else
   {qlistViewItem = ListView_UserList->firstChild();
    if (qlistViewItem>0)
       {ListView_UserList->setCurrentItem(qlistViewItem);
       }
   }
 return qlistViewItem ;
}

//------------------------------------ pushButton_Appartenance_clicked --------------------------------------
void DockFormUserList::pushButton_Appartenance_clicked()
{    QListViewItem *pQListViewItem = ListView_UserList->currentItem ();
     if (pQListViewItem==0) return;
     Dlg_Appartenance *pDlg_Appartenance = new Dlg_Appartenance(0, "Dlg_PermsUserChoice", TRUE);
     if ( pDlg_Appartenance )
        {QStringList userList;
         pDlg_Appartenance->initDialog(pQListViewItem->text(0));
         pDlg_Appartenance->exec();
         delete pDlg_Appartenance;
       }
}

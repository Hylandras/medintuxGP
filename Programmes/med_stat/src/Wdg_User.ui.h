/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

//------------------------------------ init --------------------------------------
void Wdg_User::init()
{m_pCMoteurBase      = G_pCApp->m_pCMoteurBase;
 m_CurrentPopupItem  = "";
 m_RetPopupItem      = "";
 m_pQPopupMenu       = 0;
 m_pQtimer           = 0;
 SetData();
 QString path_img = G_pCApp->m_PathAppli + "Images/";
 ListView_UserList->setPaletteBackgroundPixmap ( QPixmap (path_img +"BackLinesBluesPetit.png") );

 pushButtonCPS->setPixmap ( QPixmap(path_img +"Carte_CPS_Butt.png") );
 pushButtonEditUser->setIconSet ( QIconSet (QPixmap(path_img  + "pingoin_teteNew.png"))) ;
 pushButtonSetDefault->setIconSet ( QIconSet (QPixmap(path_img  + "Left.png"))) ;

 textLabelMessage->hide();
 #ifdef SESAMVITALE_VERSION
 #else
  pushButtonCPS->hide();
 #endif
}

//------------------------------------ destroy --------------------------------------
void Wdg_User::destroy()
{if (m_pQtimer==0) delete m_pQtimer;
}

//------------------------------------ SetData --------------------------------------
void Wdg_User::SetData()
{QString dumy("");
 QString   userQualite = InitComboBoxUserTyp();
 if (userQualite==tr("Tout utilisateur"))userQualite="";     // si vide alors c'est tousles utilisateurs
 m_pCMoteurBase->GetUserList( ListView_UserList, dumy, dumy, TRUE, &userQualite);
 QListViewItem *pcurrentItem =  ListView_UserList->firstChild();
 if (pcurrentItem>0)   ListView_UserList->setCurrentItem(pcurrentItem);
}


//------------------------------------ InitComboBoxUserTyp --------------------------------------
QString Wdg_User::InitComboBoxUserTyp()
{//....................... recuperer le type d'utilisateur par defaut .............................

 QString   userTyp;
 QString   data_ini;
 CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathIni , data_ini);
 if (CGestIni::Param_ReadParam(data_ini, "Utilisateurs", "type par defaut", &userTyp)!=0)  // zero = pas d'erreur
    {userTyp = "";
    }
 comboBoxUserTyp->clear();
 QStringList userList;
 m_pCMoteurBase->GetUserListType( userList );
 userList.prepend(tr("Tout utilisateur"));
 comboBoxUserTyp->insertStringList ( userList );
 if (userTyp.length())  // zero = pas d'erreur
    {QObject::disconnect( comboBoxUserTyp, SIGNAL(activated( const QString & )), this, SLOT(comboBoxUserTyp_activated( const QString& )) );
     setComboOnValue(comboBoxUserTyp, userTyp);
     connect( comboBoxUserTyp, SIGNAL(activated( const QString & )), this, SLOT(comboBoxUserTyp_activated( const QString& )) );
    }
 return userTyp;
}

//------------------------------------ comboBoxUserTyp_activated --------------------------------------
void Wdg_User::comboBoxUserTyp_activated( const QString &userQualite )
{QString data_ini;
 CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathIni , data_ini);
 CGestIni::Param_WriteParam(&data_ini, "Utilisateurs", "type par defaut",userQualite);
 CGestIni::Param_UpdateToDisk(G_pCApp->m_PathIni, data_ini);

 QString dumy("");
 if (userQualite==tr("Tout utilisateur"))m_pCMoteurBase->GetUserList( ListView_UserList, dumy, dumy,TRUE, 0);     // si vide alors c'est tousles utilisateurs
 else                                    m_pCMoteurBase->GetUserList( ListView_UserList, dumy, dumy,TRUE, &userQualite);

 //QListViewItem *pcurrentItem =  ListView_UserList->firstChild();
 //if (pcurrentItem>0)   ListView_UserList->setCurrentItem(pcurrentItem);.
 emit Sign_ComboUserTypClicked(userQualite);
}

//------------------------------------ setComboOnValue --------------------------------------
void Wdg_User::setComboOnValue(QComboBox *pQComboBox, const QString &value)
{int nb = pQComboBox->count();
 for (int i=0; i<nb; ++i)
     {QString str = pQComboBox->text(i);
      if (str== value)
         {pQComboBox->setCurrentItem (i);
          return;
         }
     }
}
//------------------------------------ getSignUser --------------------------------------
QListViewItem *Wdg_User::getSignUser()
{return getSignUser( ListView_UserList->selectedItem());
}

//------------------------------------ getSignUser --------------------------------------
QListViewItem *Wdg_User::getSignUser(QListViewItem *pQListViewItem)
{
 if (pQListViewItem)
    {QListViewItem *pQListViewItemParent = pQListViewItem->parent();
     if (pQListViewItemParent==0) return pQListViewItem;
     else                         return pQListViewItemParent;
    }
 return 0;
}

//------------------------------------ getUserItem --------------------------------------
QListViewItem *Wdg_User::getUserItem(const QString &user, const QString &sign_user)
{   QListViewItemIterator it( ListView_UserList );
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

//------------------------------------ SetUnLogged --------------------------------------
void Wdg_User::SetUnLogged()
{QListViewItem *pcurrentItem =  ListView_UserList->firstChild();
 if (pcurrentItem>0)
    {ListView_UserList->setCurrentItem( pcurrentItem );
     ListView_UserList->setSelected (pcurrentItem, FALSE );
     m_pQListViewLastSelectedItem  = 0;
     pushButtonEditUser->hide();
     pushButtonSetDefault->hide();
     textLabelMessage->show();
     if (m_pQtimer ) {delete m_pQtimer ; m_pQtimer = 0;}
     if (m_pQtimer==0) m_pQtimer = new QTimer( this , "timer_SetUnLogged");
     if (m_pQtimer )
        {connect( m_pQtimer, SIGNAL(timeout()), this, SLOT(timerDone()) );
         m_pQtimer->start( 300 ); // 2 seconds
        }
    }
}
//------------------------------------ SetLogged --------------------------------------
void Wdg_User::SetLogged()
{    if (m_pQtimer!=0)
        {m_pQtimer->stop();
         delete m_pQtimer;
         m_pQtimer = 0;
        }
     pushButtonEditUser->show();
     pushButtonSetDefault->show();
     textLabelMessage->hide();
}
//------------------------------------ timerDone ---------------------------------------------
void Wdg_User::timerDone()
{if (textLabelMessage->paletteForegroundColor()==QColor("red"))
     textLabelMessage->setPaletteForegroundColor ( QColor("black") );
 else
     textLabelMessage->setPaletteForegroundColor ( QColor("red") );
}

//------------------------------------ SetUser ---------------------------------------------
void Wdg_User::SetUser(const char* user, const char* sign_user)
{/*
 QListViewItem *qlistViewItem=0;
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
 */

 //QListViewItem *qlistViewItem=0;
 //ListView_UserList->disconnect( SIGNAL(currentChanged(QListViewItem *)) );
 //QObject::disconnect ( ListView_UserList, SIGNAL(currentChanged(QListViewItem *)), this, SLOT(ListView_UserList_currentChanged (QListViewItem *)));
 //QObject::disconnect ( ListView_UserList, SIGNAL(clicked(QListViewItem *)),        this, SLOT(ListView_UserList_clicked (QListViewItem *)));
 //QObject::disconnect ( ListView_UserList, SIGNAL(selectionChanged(QListViewItem *)), this, SLOT(ListView_UserList_selectionChanged (QListViewItem *)));
 //QObject::disconnect ( ListView_UserList, SIGNAL(clicked(QListViewItem *)),          this, SLOT(ListView_UserList_clicked (QListViewItem *)));
/*
 if (user != "") qlistViewItem  =  ListView_UserList->findItem (user, 0 );
 if (qlistViewItem)
    {ListView_UserList->setCurrentItem (qlistViewItem );
     ListView_UserList->setSelected (qlistViewItem, TRUE );
     ListView_UserList->ensureItemVisible(qlistViewItem);
   }
 else
   {qlistViewItem = ListView_UserList->firstChild();
    if (qlistViewItem>0)
       {ListView_UserList->setCurrentItem(qlistViewItem);
        ListView_UserList->setSelected (qlistViewItem, TRUE );
        ListView_UserList->ensureItemVisible(qlistViewItem);
       }
   }
*/
/*
connect( ListView_UserList,        SIGNAL(clicked(QListViewItem *) ),
          this,                     SLOT(ListView_UserList_clicked (QListViewItem *))
        );
 connect( ListView_UserList,        SIGNAL( selectionChanged(QListViewItem *) ),
          this,                     SLOT(ListView_UserList_selectionChanged (QListViewItem *))
        );
*/
 /*
 connect( ListView_UserList,        SIGNAL( currentChanged(QListViewItem *) ),
          this,                     SLOT(ListView_UserList_currentChanged (QListViewItem *))
        );
 */
}
//------------------------------------ SetUserItem ---------------------------------------------
void Wdg_User::SetUserItem(QListViewItem *pQListViewItem)
{if (pQListViewItem==0) return;
 QObject::disconnect ( ListView_UserList, SIGNAL(selectionChanged(QListViewItem *)), this, SLOT(ListView_UserList_selectionChanged (QListViewItem *)));
 ListView_UserList->setSelected (pQListViewItem, TRUE );
 m_pQListViewLastSelectedItem = pQListViewItem;
  connect( ListView_UserList,        SIGNAL( selectionChanged(QListViewItem *) ),
           this,                     SLOT(ListView_UserList_selectionChanged (QListViewItem *))
        );
}
//------------------------------------ IsUserInList --------------------------------------
bool Wdg_User::IsUserInList(const char* user)
{if ( ListView_UserList->findItem (user, 0 )) return TRUE;
 return FALSE;
}

//------------------------------------ GetSelectedUser --------------------------------------
QString Wdg_User::GetSelectedUser()
{QListViewItem* pQListViewItem = ListView_UserList->selectedItem ();
 if (pQListViewItem) return pQListViewItem->text(0);
 pQListViewItem = ListView_UserList->currentItem ();
 if (pQListViewItem) return pQListViewItem->text(0);
 pQListViewItem = ListView_UserList->firstChild();
 if (pQListViewItem) return pQListViewItem->text(0);
 return QString("");
}

//------------------------------------ ListView_UserList_selectionChanged --------------------------------------
void Wdg_User::ListView_UserList_selectionChanged( QListViewItem *pQListViewItem )
{QObject::disconnect ( ListView_UserList, SIGNAL(selectionChanged(QListViewItem *)), this, SLOT(ListView_UserList_selectionChanged (QListViewItem *)));
 ListView_UserList_LineChanged( pQListViewItem );
 connect( ListView_UserList,        SIGNAL( selectionChanged(QListViewItem *) ),
           this,                     SLOT(ListView_UserList_selectionChanged (QListViewItem *))
        );
}

//------------------------------------ ListView_UserList_LineChanged --------------------------------------
void Wdg_User::ListView_UserList_LineChanged( QListViewItem *pQListViewItem )
{if (pQListViewItem<=0) return;
 QListViewItem * pQListViewItemParent = getSignUser(pQListViewItem);

 if (G_pCApp->AuthentifyAndSelectUser(this, pQListViewItem->text(0), pQListViewItemParent->text(0)))
     {emit Sign_UserSelected(pQListViewItem, pQListViewItemParent);
      SetLogged();
      m_pQListViewLastSelectedItem  = pQListViewItem;     // memoriser le  dernier item selectionneé pour le replacer si echec lors d'un changement futur d'utilisateur
     }
 else
    {m_pQListViewItemTmp = pQListViewItem;                        // faut bien que le slot : RestaureOldUser() connaisse la valeur du pQListViewItem
     QTimer::singleShot( 50, this, SLOT(RestaureOldUser()) );     // obligatoire sinon boucle ....
    }
}
//------------------------------------ RestaureOldUser --------------------------------------
void Wdg_User::RestaureOldUser()
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
//------------------------------------ pushButtonEditUser_clicked --------------------------------------
void Wdg_User::pushButtonEditUser_clicked()
{emit Sign_UserMustBeEdited();
}

//------------------------------------ pushButtonEditUser_clicked --------------------------------------
void Wdg_User::pushButtonCPS_clicked()
{emit Sign_CPS_IsClicked();
}




//------------------------------------ pushButtonSetDefault_clicked --------------------------------------
void Wdg_User::pushButtonSetDefault_clicked()
{QString data_ini="";
 pushButtonSetDefault->setIconSet ( QIconSet (QPixmap(G_pCApp->m_PathAppli + "/Images/Down.png"))) ;
 //.................. creer liste des items du menu .......................
 QStringList popItem;
 popItem.append(tr("Lancement &automatique du programme avec l'utilisateur : ")+ G_pCApp->m_User);
 popItem.append(tr("Annuler le lancement automatique du programme avec un utilisateur"));
 QString popRet       = DoPopupList(popItem);
 CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathIni , data_ini);
 if (popRet==tr("Annuler le lancement automatique du programme avec un utilisateur"))
    {CGestIni::Param_WriteParam( &data_ini, "Derniere Session", "Utilisateur",  "","");
     CGestIni::Param_WriteParam( &data_ini, "Derniere Session", "Password",   "");
    }
 else if (popRet.length())
    {CGestIni::Param_WriteParam( &data_ini, "Derniere Session", "Utilisateur",  G_pCApp->m_User, G_pCApp->m_SignUser);
     CGestIni::Param_WriteParam( &data_ini, "Derniere Session", "Password",   QString("#") + G_pCApp->m_CriptedPassWord);
    }
 CGestIni::Param_UpdateToDisk(G_pCApp->m_PathIni, data_ini);
 pushButtonSetDefault->setIconSet ( QIconSet (QPixmap(G_pCApp->m_PathAppli + "/Images/Left.png"))) ;
}

//--------------------------------- DoPopupList ----------------------------------------------------------------------
QString Wdg_User::DoPopupList(QStringList &list)
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
void Wdg_User::OnMenuActivated (int id)
{if (id != -1 && m_pQPopupMenu)
    {m_CurrentPopupItem = m_pQPopupMenu->text(id);
    }
}

//--------------------------------- GetPopupItem  -------------------------------------------------------------
void Wdg_User::GetPopupItem ()
{m_RetPopupItem  = m_CurrentPopupItem;
}



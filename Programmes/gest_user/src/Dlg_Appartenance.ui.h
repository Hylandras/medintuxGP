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
#include "Dlg_PermsDefine.h"
#include <qsqlquery.h>
//--------------------------------- init -------------------------------------------------------------
void Dlg_Appartenance::init()
{m_pQPopupMenu      = 0;
 m_CurrentPopupItem = "";
 m_RetPopupItem     = "";
 radioButtonCommence->setChecked (TRUE);
 radioButtonContient->setChecked (FALSE);
 lineEditNom->setFocus();
}

//--------------------------------- initDialog -------------------------------------------------------------
void Dlg_Appartenance::initDialog(const QString &user)
{m_User = user;
 setCaption(tr("Utilisateurs signataires auxquels appartient : ") + user);
 textLabel_info->setText(tr("<b>Sélectionnez dans la liste des utilisateurs signataires ci-dessous<br>"
                            "ceux pour lesquels vous voulez définir ou modifier les droits<br>"
                            "délégués de l'utilisateur : <font color=\"#ff0000\"> %1 </font><br>"
                            "(multi-sélection possible et menu des actions possibles avec le bouton droit)</b>").arg(user));
 G_pCApp->m_pCMoteurBase->PermsUserFatherGetQlistView( ListView_UserList, user , G_pCApp->m_PathAppli + "/Images/");
 setIcon( QPixmap (G_pCApp->m_PathAppli + "/Images/groupe.png" ) );
 textLabel_icone->setPixmap ( QPixmap (G_pCApp->m_PathAppli + "/Images/groupe.png" ) );
}

//--------------------------------- pushButtonOk_clicked -------------------------------------------------------------
void Dlg_Appartenance::pushButtonOk_clicked()
{ QListViewItemIterator it( ListView_UserList );
  QString friendGUID = G_pCApp->m_pCMoteurBase->GetFieldValue( G_pCApp->m_pCMoteurBase->m_USER_IDENT_TBL_NAME,
                                                               G_pCApp->m_pCMoteurBase->m_USER_IDENT_GUID,
                                                               G_pCApp->m_pCMoteurBase->m_USER_IDENT_LOGIN,
                                                               m_User );

 while ( it.current() )
       {  QListViewItem *pQListViewItem = it.current();
          QString signGUID = G_pCApp->m_pCMoteurBase->GetFieldValue( G_pCApp->m_pCMoteurBase->m_USER_IDENT_TBL_NAME,
                                                                     G_pCApp->m_pCMoteurBase->m_USER_IDENT_GUID,
                                                                     G_pCApp->m_pCMoteurBase->m_USER_IDENT_LOGIN,
                                                                     pQListViewItem->text(0));
          //.................... si un pk a été placé (enregistement existant) et droits anulé (effacement) .........................
          if (pQListViewItem->text(3).length()==0 && pQListViewItem->text(6).length())
             {  G_pCApp->m_pCMoteurBase->PermsUserDeleteFromPk( pQListViewItem->text(6) );
                pQListViewItem->setText(3, "" );
             }
          //.................... si pas de pk  ( pas d'enregistement existant) et droits positionnés (creation) .........................
          else if (pQListViewItem->text(3).length() && pQListViewItem->text(6).length()==0)
             { QString pk = G_pCApp->m_pCMoteurBase->PermsUserCreate(pQListViewItem->text(3) , signGUID, friendGUID,  " ");
               pQListViewItem->setText(6, pk );
             }
          //.................... si un pk a été placé (enregistement existant) et droits positionnés (modification) .........................
          else if (pQListViewItem->text(3).length() && pQListViewItem->text(6).length())
             { G_pCApp->m_pCMoteurBase->PermsUserUpdate(pQListViewItem->text(6) , pQListViewItem->text(3) , signGUID, friendGUID,  " ");
             }
          ++it;
       }
 accept();
}

//--------------------------------- pushButtonAnnuler_clicked -------------------------------------------------------------
void Dlg_Appartenance::pushButtonAnnuler_clicked()
{reject();
}

//--------------------------------- lineEditNom_textChanged -------------------------------------------------------------
void Dlg_Appartenance::lineEditNom_textChanged( const QString &text  )
{QListViewItem *pQListViewItem = 0;
 if (radioButtonCommence->isChecked ())
     pQListViewItem = ListView_UserList->findItem ( text, 0, Qt::BeginsWith);
 else
     pQListViewItem = ListView_UserList->findItem ( text, 0, Qt::Contains);
 if (pQListViewItem == 0) return;
 ListView_UserList->setSelected ( pQListViewItem, TRUE );
 ListView_UserList->ensureItemVisible ( pQListViewItem );
}

//--------------------------------- DoAssistantDroits-------------------------------------------------------------
bool Dlg_Appartenance::DoAssistantDroits(QString &droits)
{ int ret = FALSE;
  Dlg_PermsDefine *dlg = new Dlg_PermsDefine(this, "MyDlgList",TRUE);
  if (dlg)
     {dlg->InitDialog(&droits, 1);
      if (dlg->exec() == QDialog::Accepted)
        {ret = TRUE;
        }
     delete dlg;
   }
  return ret;
}

//--------------------------------- SetListOnDroits-------------------------------------------------------------
void Dlg_Appartenance::SetListOnDroits(const QString &droits)
{  QListViewItemIterator it( ListView_UserList );
   while ( it.current() )
         {QListViewItem *pQListViewItem = it.current();
          if (pQListViewItem->isSelected() )
             {pQListViewItem->setText(3, droits );                                            //  droits delegues
             if (droits.length())
                 {pQListViewItem->setPixmap (0, QPixmap( G_pCApp->m_PathAppli + "/Images/CheckBoxOn.png") ) ;
                 }
              else
                 {pQListViewItem->setPixmap (0, QPixmap( G_pCApp->m_PathAppli + "/Images/CheckBoxOff.png") ) ;
                 }
             }
           ++it;
         }
}

//--------------------------------- ListView_UserList_contextMenuRequested  -------------------------------------------------------------
void Dlg_Appartenance::ListView_UserList_contextMenuRequested( QListViewItem *pQListViewItem, const QPoint &, int  )
{if (pQListViewItem==0) return;
 QStringList popItem;
 popItem.append(tr("Définir ou modifier les droits délégués"));
 popItem.append( "---------");
 popItem.append(tr("Retirer la délégation de droits"));
 popItem.append(tr("Placer les droits délégués au maximum"));
 popItem.append( "---------");
 QString popRet  = DoPopupList(popItem);
 QString droits  = pQListViewItem->text(3);
 pQListViewItem->setSelected(TRUE);          // recuperer chaine droits de la zone de saisie
 if (popRet.length())
    {if (popRet==tr("Retirer la délégation de droits"))
        {SetListOnDroits("");
         return;
        }
     if (popRet==tr("Placer les droits délégués au maximum"))
        {droits =  G_pCApp->GetListDroitsPossibles();
         droits = droits.remove("-adm");
         droits = droits.remove("-sgn");
         droits = droits.remove("-med");
         SetListOnDroits(droits);
         return;
        }
     if ( popRet==tr("Définir ou modifier les droits délégués"))
        {if (DoAssistantDroits(droits)) SetListOnDroits(droits);
         return;
        }
   }
}

//--------------------------------- DoPopupList ----------------------------------------------------------------------
QString Dlg_Appartenance::DoPopupList(QStringList &list)
{
 //m_pQListViewItem->setSelected (TRUE);   // reselectionner l'item au dessus duquel se retrouve la souris
 m_CurrentPopupItem = "";
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
 connect( m_pQPopupMenu, SIGNAL( activated( int)),  this,          SLOT(OnMenuActivated( int))    );

 m_pQPopupMenu->exec( QCursor::pos() /* mapToGlobal (  QPoint ( x, y) )*/ );
 delete  m_pQPopupMenu;
 m_pQPopupMenu = 0;
 return  m_RetPopupItem;
}

//--------------------------------- OnMenuActivated  -------------------------------------------------------------
void Dlg_Appartenance::OnMenuActivated (int id)
{if (id != -1 && m_pQPopupMenu)
    {m_CurrentPopupItem = m_pQPopupMenu->text(id);
    }
}

//--------------------------------- GetPopupItem  -------------------------------------------------------------
void Dlg_Appartenance::GetPopupItem ()
{m_RetPopupItem  = m_CurrentPopupItem;
}

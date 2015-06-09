 /********************************* C_Frm_UserList.cpp ****************************
 *  #include "C_Frm_UserList.h"                                                   *
 * ...............................................................................*
 *   Project   :  MedinTux  (typographie made in france)                          *
 *   Copyright : (C) 2004-2005-2006-2007-2008-2009-2010 and for the eternity      *
 *   by        :  Sevin Roland     from   MedinTux project                        *
 *   E-Mail    : roland-sevin@medintux.org                                        *
 *   Web site  : www.medintux.org                                                 *
 * ...............................................................................*
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat �  l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Fédération, 75752 PARIS cedex 15.   *
 *                            FRANCE                                              *
 *   Centre National de la Recherche Scientifique                                 *
 *   - CNRS,                                                                      *
 *                            3 rue Michel-Ange, 75794 Paris cedex 16.            *
 *                            FRANCE                                              *
 *   Institut National de Recherche en Informatique et en Automatique             *
 *   - INRIA,                                                                     *
 *                            Domaine de Voluceau, Rocquencourt, BP 105, 78153    *
 *                            FRANCE                                              *
 *                                                                                *
 *         This program is distributed in the hope that it will be useful,        *
 *         but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                   *
 *         CeCILL  License (GPL compatible) for more details.                     *
 *                                                                                *
 **********************************************************************************/

#include "C_Frm_UserList.h"

#include <QTreeWidgetItemIterator>
#include <QList>
#include <QHeaderView>

#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/Theme/Theme.h"
#include "../../MedinTuxTools-QT4/CMoteur_Base.h"
#include "../../MedinTuxTools-QT4/Theme/ThemePopup.h"

//------------------------ C_Frm_UserList ---------------------------------------
C_Frm_UserList::C_Frm_UserList(CMoteurBase *pCMoteurBase, QWidget *parent)
  :  QFrame(parent), m_pCMoteurBase(pCMoteurBase)
{   m_pQtimer                    = 0;
    m_LastSelectedUser           = "";
    m_pStateTimer                = 1;
    m_ui.setupUi(this);
//.......................... tenir compte de la version ....................
#ifdef SESAM_VERSION
    QPixmap px = Theme::getIcon("UserList/Carte_CPS_Butt.png");
    m_ui.pushButtonCPS->setIcon        (px );
    m_ui.pushButtonCPS->setIconSize(QSize(px.width(),px.height()));
#else
    m_ui.pushButtonCPS->hide();
#endif

    m_ui.pushButtonEditUser->setIcon   ( Theme::getIcon( "UserList/pingoin_teteNew.png")) ;
    m_ui.pushButtonSetDefault->setIcon ( Theme::getIcon( "UserList/Left.png")) ;
    m_ui.pushButtonSetUnloged->setIcon ( Theme::getIcon( "UserList/exit.png")) ;
    //............. cacher les colonnes du Pk et GUID ....................
    QHeaderView *pQHeaderView = m_ui.ListView_UserList->header();
    pQHeaderView->hideSection (3);
    pQHeaderView->hideSection (4);
    m_ui.ListView_UserList->setAlternatingRowColors (TRUE );

    //................... se mettre en mode non identifié ..................
    SetUnLogged();
    //................................... les connexions ..................................
    connect( m_ui.ListView_UserList,    SIGNAL( itemChanged( QTreeWidgetItem * , int)),       this, SLOT( ListView_UserList_LineChanged( QTreeWidgetItem * , int )) );
    connect( m_ui.ListView_UserList,    SIGNAL( itemClicked( QTreeWidgetItem * , int)),       this, SLOT( ListView_UserList_LineChanged( QTreeWidgetItem * , int )) );
    connect( m_ui.ListView_UserList,    SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int)), this, SLOT( Slot_ListView_UserList_DoubleClicked( QTreeWidgetItem * , int)) );
    connect( m_ui.comboBoxUserTyp,      SIGNAL( activated( const QString & )),                this, SLOT( comboBoxUserTyp_activated( const QString& )) );
    connect( m_ui.pushButtonEditUser,   SIGNAL( clicked() ),                                  this, SLOT( pushButtonEditUser_clicked() ) );
    connect( m_ui.pushButtonSetDefault, SIGNAL( clicked() ),                                  this, SLOT( pushButtonSetDefault_clicked() ) );
    connect( m_ui.pushButtonCPS,        SIGNAL( clicked() ),                                  this, SLOT( pushButtonCPS_clicked() ) );
    connect( m_ui.pushButtonSetUnloged, SIGNAL( clicked() ),                                  this, SLOT( pushButtonSetUnloged_clicked() ) );
    //........................ positionner l'utilisateur par defaut .....................
    setDefaultParameter();
}

//------------------------ ~C_Frm_UserList ---------------------------------------
C_Frm_UserList::~C_Frm_UserList()
{if (m_pQtimer==0) delete m_pQtimer;
}
//------------------------------------ Slot_ListView_UserList_DoubleClicked --------------------------------------
void C_Frm_UserList::Slot_ListView_UserList_DoubleClicked( QTreeWidgetItem * , int)
{pushButtonEditUser_clicked();
}

//------------------------------------ pushButtonSetUnloged_clicked --------------------------------------
void C_Frm_UserList::pushButtonSetUnloged_clicked()
{int ret = 1;
 emit Sign_UserMustBeUnloged(ret);
 if (ret) SetUnLogged();
}

//------------------------------------ pushButtonEditUser_clicked --------------------------------------
void C_Frm_UserList::pushButtonEditUser_clicked()
{QStringList lst = m_LastSelectedUser.split("@", QString::KeepEmptyParts);
 if (lst.count()>=2)
    {emit Sign_UserMustBeEdited(lst[0], lst[1]);
    }
 else
    {emit Sign_UserMustBeEdited("", "");
    }
}

//------------------------------------ pushButtonEditUser_clicked --------------------------------------
void C_Frm_UserList::pushButtonCPS_clicked()
{emit Sign_CPS_IsClicked();
}

//------------------------------------ pushButtonSetDefault_clicked --------------------------------------
void C_Frm_UserList::pushButtonSetDefault_clicked()
{   m_ui.pushButtonSetDefault->setIcon ( QIcon (Theme::getIcon( "UserList/Down.png")));
    //.................. creer liste des items du menu .......................
    QStringList optionList;
    optionList<<"=1="+tr("&Automatic start of the program with the user: ")+ G_pCApp->m_User;
    optionList<<"=2="+tr("Cancel automatic start of the program with a user");
    QString option = ThemePopup(optionList,this).DoPopupList();
    if (option.length()==0) return;
    G_pCApp->saveLastUserOn_Ini(option=="1");
    m_ui.pushButtonSetDefault->setIcon ( QIcon (Theme::getIcon( "UserList/Left.png"))) ;
}

//------------------------------------ setDefaultUser --------------------------------------
void C_Frm_UserList::setDefaultParameter()
{QString specialite="";
 //............ rechercher si pas d'uilisateur par defaut ....................
 if (G_pCApp->IsAppStartWithUser())
     {specialite = m_pCMoteurBase->GetUserSpecialite( G_pCApp->m_User);  // chercher la specialite de cet utilisateur
      initUserListOnUserType(specialite);                                // initialiser la liste des utilisateurs sur cette specialite
      initComboBoxUserTyp();                                             // initialiser la liste des specialites
      setComboBoxOnUserTyp_WithoutSignals(specialite);                   // positonner la combo des specialite dessus

      QTreeWidgetItem *pQTreeWidgetItem =  getUserItem(G_pCApp->m_User, G_pCApp->m_SignUser); // rechercher l'item dans la liste des utilisateurs
      if (pQTreeWidgetItem)
         { SetLogged();
           SetUserItem_WhitoutEmitSignals(pQTreeWidgetItem);          // n'envoie pas le signal de changement d'user (qui demande le pass!!)
           m_LastSelectedUser  = itemTo_SignUser_User_Name(pQTreeWidgetItem);
           return;
         }
     }
 //........... ici on n'a pas trouve d'utilisateur par defaut .........................
 specialite = getDefaultComboBoxUserTyp();                    // rechercher la specialite par defaut (celle de Manager.ini)
 initUserListOnUserType(specialite);                          // initialiser la liste des utilisateurs sur cette specialite
 initComboBoxUserTyp();                                       // initialiser la liste des specialites
 setComboBoxOnUserTyp_WithoutSignals(specialite);             // positonner la combo des specialite dessus
}

//------------------------------------ SetUserItem_WhitoutEmitSignals ---------------------------------------------
void C_Frm_UserList::SetUserItem_WhitoutEmitSignals(QTreeWidgetItem *pQTreeWidgetItem)
{if (pQTreeWidgetItem==0) return;
 QObject::disconnect( m_ui.ListView_UserList, SIGNAL(itemChanged( QTreeWidgetItem * , int)), this, SLOT(ListView_UserList_LineChanged( QTreeWidgetItem * , int )) );
 QObject::disconnect( m_ui.ListView_UserList, SIGNAL(itemClicked( QTreeWidgetItem * , int)), this, SLOT(ListView_UserList_LineChanged( QTreeWidgetItem * , int )) );

 pQTreeWidgetItem->setSelected (TRUE );
 m_ui.ListView_UserList->scrollToItem ( pQTreeWidgetItem );
 m_LastSelectedUser  = itemTo_SignUser_User_Name(pQTreeWidgetItem);
 connect( m_ui.ListView_UserList, SIGNAL(itemChanged( QTreeWidgetItem * , int)), this, SLOT(ListView_UserList_LineChanged( QTreeWidgetItem * , int )) );
 connect( m_ui.ListView_UserList, SIGNAL(itemClicked( QTreeWidgetItem * , int)), this, SLOT(ListView_UserList_LineChanged( QTreeWidgetItem * , int )) );
}

//------------------------------------ ListView_UserList_LineChanged --------------------------------------
void C_Frm_UserList::ListView_UserList_LineChanged( QTreeWidgetItem *pQListViewItem , int )
{if (pQListViewItem<=0) return;
 QTreeWidgetItem * pQListViewItemParent = getSignUser(pQListViewItem);

 if (G_pCApp->AuthentifyAndSelectUser(this, pQListViewItem->text(0), pQListViewItemParent->text(0)))
    { emit Sign_UserSelected(pQListViewItem, pQListViewItemParent);
      SetLogged();
      m_LastSelectedUser  = itemTo_SignUser_User_Name(pQListViewItem);     // memoriser le  dernier item selectionneé pour le replacer si echec lors d'un changement futur d'utilisateur
    }
 else
    { restaureOldUser();
    }
}

//------------------------------------ itemTo_SignUser_User_Name --------------------------------------
QString C_Frm_UserList::itemTo_SignUser_User_Name(QTreeWidgetItem *pQListViewItem)
{QTreeWidgetItem *pQListViewItemParent = pQListViewItem->parent();
 if (pQListViewItemParent==0)
    {pQListViewItemParent = pQListViewItem;
    }
 return pQListViewItem->text(0)+"@"+pQListViewItemParent->text(0);
}

//------------------------------------ getUserItem --------------------------------------
QTreeWidgetItem *C_Frm_UserList::getUserItem(const QString &user, const QString &sign_user)
{   QTreeWidgetItemIterator it(m_ui.ListView_UserList);
     while (*it)
       { QTreeWidgetItem *pQListViewItem = (*it);
         if ( pQListViewItem->text(0) == user )
              { QTreeWidgetItem *pQListViewItemParent = pQListViewItem->parent();
                if (pQListViewItemParent==0) pQListViewItemParent = pQListViewItem;
                if (pQListViewItemParent->text(0) == sign_user) return pQListViewItem;
              }
         ++it;
       }
 return 0;
}

//------------------------------------ reinitOn_SignUser_User_Name --------------------------------------
bool C_Frm_UserList::reinitOn_SignUser_User_Name(const QString &user_in, const QString &sign_user)
{     QString       user = user_in;
      QString specialite = m_pCMoteurBase->GetUserSpecialite(user);     // chercher la specialite de cet utilisateur
      initUserListOnUserType(specialite);                                // initialiser la liste des utilisateurs sur cette specialite
      initComboBoxUserTyp();                                             // initialiser la liste des specialites
      setComboBoxOnUserTyp_WithoutSignals(specialite);                   // positonner la combo des specialite dessus

      QTreeWidgetItem *pQTreeWidgetItem =  getUserItem(user, sign_user);   // rechercher l'item dans la liste des utilisateurs
      if (pQTreeWidgetItem)
         { SetLogged();
           SetUserItem_WhitoutEmitSignals(pQTreeWidgetItem);               // n'envoie pas le signal de changement d'user (qui demande le pass!!)
           m_LastSelectedUser  = itemTo_SignUser_User_Name(pQTreeWidgetItem);
           return TRUE;
         }
      SetUnLogged();
      return FALSE;
}

//------------------------------------ restaureOldUser --------------------------------------
void C_Frm_UserList::restaureOldUser()
{    bool notFond = TRUE;
     QObject::disconnect ( m_ui.ListView_UserList, SIGNAL(itemChanged(QTreeWidgetItem *, int )), this, SLOT(ListView_UserList_LineChanged (QTreeWidgetItem *, int )));
     if (m_LastSelectedUser.length())
        {QStringList lst = m_LastSelectedUser.split("@", QString::KeepEmptyParts);
         QTreeWidgetItem *pQTreeWidgetItem = getUserItem(lst[0], lst[1]);
         if (pQTreeWidgetItem)
            { pQTreeWidgetItem->setSelected ( TRUE );
              m_ui.ListView_UserList->setCurrentItem (pQTreeWidgetItem );
              notFond = FALSE;
              m_ui.ListView_UserList->scrollToItem ( pQTreeWidgetItem );
            }
        }
     if (notFond)
        {QList<QTreeWidgetItem *> list = m_ui.ListView_UserList->selectedItems ();
         for (int i=0; i<list.count(); ++i)
             {list[i]->setSelected(FALSE);
             }
        }
     connect( m_ui.ListView_UserList, SIGNAL( itemChanged(QTreeWidgetItem *, int ) ), SLOT(ListView_UserList_LineChanged (QTreeWidgetItem *, int)) );
}

//------------------------------------ getSignUser --------------------------------------
QTreeWidgetItem *C_Frm_UserList::getSignUser(QTreeWidgetItem *pQListViewItem)
{
 if (pQListViewItem)
    {QTreeWidgetItem *pQListViewItemParent = pQListViewItem->parent();
     if (pQListViewItemParent==0) return pQListViewItem;
     else                         return pQListViewItemParent;
    }
 return 0;
}

//------------------------------------ getDefaultComboBoxUserTyp --------------------------------------
QString C_Frm_UserList::getDefaultComboBoxUserTyp()
{QString   userTyp;
 if (G_pCApp->readParam( "Utilisateurs", "type par defaut", &userTyp) == QString::null) return userTyp.trimmed();// zero = pas d'erreur
 return QString::null;
}

//------------------------------------ initComboBoxUserTyp --------------------------------------
void C_Frm_UserList::initComboBoxUserTyp()
{m_ui.comboBoxUserTyp->clear();
 QStringList userList;
 m_pCMoteurBase->GetUserListType( userList );
 userList.prepend(tr("Any user"));
 m_ui.comboBoxUserTyp->insertStringList ( userList );
}

//------------------------------------ setComboBoxOnUserTyp_WithoutSignals --------------------------------------
void C_Frm_UserList::setComboBoxOnUserTyp_WithoutSignals(QString userTyp)
{ if (userTyp.length()==0) userTyp = tr("Any user");     // si l
  QObject::disconnect( m_ui.comboBoxUserTyp, SIGNAL(activated( const QString & )), this, SLOT(comboBoxUserTyp_activated( const QString& )) );
  setComboOnValue(m_ui.comboBoxUserTyp, userTyp);
  connect(  m_ui.comboBoxUserTyp, SIGNAL(activated( const QString & )),            this, SLOT(comboBoxUserTyp_activated( const QString& )) );
}

//------------------------------------ setComboOnValue --------------------------------------
void C_Frm_UserList::setComboOnValue(QComboBox *pQComboBox, const QString &value)
{int nb = pQComboBox->count();
 for (int i=0; i<nb; ++i)
     {QString str = pQComboBox->text(i);
      if (str== value)
         {pQComboBox->setCurrentItem (i);
          return;
         }
     }
}
//------------------------------------ comboBoxUserTyp_activated --------------------------------------
void C_Frm_UserList::comboBoxUserTyp_activated(const  QString &userQualite_in )
{QString userQualite;
 G_pCApp->writeParam( "Utilisateurs", "type par defaut",userQualite_in);
 G_pCApp->updateIniParamToDisk();

 if    (userQualite_in==tr("Any user")) userQualite = "";
 else                                   userQualite = userQualite_in;
 initUserListOnUserType(userQualite);
 restaureOldUser();
 emit Sign_ComboUserTypClicked(userQualite);
}

//------------------------------------ SetUnLogged --------------------------------------
void C_Frm_UserList::SetUnLogged()
{QList<QTreeWidgetItem *> list = m_ui.ListView_UserList->selectedItems ();
 for (int i=0; i<list.count(); ++i)
     {list[i]->setSelected(FALSE);
     }
  m_ui.pushButtonEditUser->hide();
  m_ui.pushButtonSetDefault->hide();
  m_ui.pushButtonSetUnloged->hide();
  m_ui.textLabelMessage->show();
  if (m_pQtimer ) {delete m_pQtimer ; m_pQtimer = 0;}
  if (m_pQtimer==0) m_pQtimer = new QTimer( this , "timer_SetUnLogged");
  if (m_pQtimer )
     {connect( m_pQtimer, SIGNAL(timeout()), this, SLOT(timerDone()) );
      m_pQtimer->start( 300 ); // 2 seconds
     }
}

//------------------------------------ SetLogged --------------------------------------
void C_Frm_UserList::SetLogged()
{    if (m_pQtimer!=0)
        {m_pQtimer->stop();
         delete m_pQtimer;
         m_pQtimer = 0;
        }
     m_ui.pushButtonEditUser->show();
     m_ui.pushButtonSetDefault->show();
     m_ui.textLabelMessage->hide();
     m_ui.pushButtonSetUnloged->show();
}

//------------------------------------ timerDone ---------------------------------------------
void C_Frm_UserList::timerDone()
{m_pStateTimer ^= 1;
 QString style  = "";
 //QString style = "QPushButton { border: 1px solid #8f8f91; border-radius: 0px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);}"    // style normal
 //                "QPushButton:pressed {                    border-radius: 0px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa);}";   // style selectionne
           style = "QPushButton { border: 10px solid #8f8f91; border-radius: 10px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);}"    // style normal
                   "QPushButton:pressed {                    border-radius: 10px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa);}";   // style selectionne
 /*
 style = "QPushButton#evilButton {"
        "background-color: red;"
        "border-style: outset;"
        "border-width: 2px;"
        "border-radius: 10px;"
        "border-color: red;"
        "font: bold 14px;"
        "min-width: 10em;"
        "padding: 6px;}";
 */
 if (m_pStateTimer)
     /*
     style = "QPushButton {"
        "background-color: red;"
        "border-style: outset;"
        "border-width: 2px;"
        "border-radius: 10px;"
        "border-color: red;"
        "font: bold 14px;"
        "min-width: 10em;"
        "padding: 6px;}";
     */
     m_ui.textLabelMessage->setText(QString("<img src=\"") + Theme::getPath(TRUE) + "UserList/UserList.png\">" + "<span style=\" font-weight:600; color:#000000;\">"+tr("To start, please get identified by selecting a user from the list below")+"</span>");
 else
    /*
     style = "QPushButton {"
        "background-color: gray;"
        "border-style: outset;"
        "border-width: 2px;"
        "border-radius: 10px;"
        "border-color: red;"
        "font: bold 14px;"
        "min-width: 10em;"
        "padding: 6px;}";
   */

    m_ui.textLabelMessage->setText(QString("<img src=\"") + Theme::getPath(TRUE) + "UserList/UserListRed.png\">" + "<span style=\" font-weight:600; color:#ff0000;\">"+tr("To start, please get identified by selecting a user from the list below")+"</span>");
 //m_ui.textLabelMessage->setStyleSheet(style);
}
//------------------------ initUserListOnUserType ---------------------------------------
void C_Frm_UserList::initUserListOnUserType(const QString &specialite_in)
{
     QObject::disconnect( m_ui.ListView_UserList, SIGNAL(itemChanged( QTreeWidgetItem * , int)), this, SLOT(ListView_UserList_LineChanged( QTreeWidgetItem * , int )) );
     QObject::disconnect( m_ui.ListView_UserList, SIGNAL(itemClicked( QTreeWidgetItem * , int)), this, SLOT(ListView_UserList_LineChanged( QTreeWidgetItem * , int )) );

     QString specialite = specialite_in;
     if (specialite == tr("Any user")) specialite = "";   // si vide alors c'est tous les utilisateurs qui seront explores
     G_pCApp->m_pCMoteurBase->GetUserList(m_ui.ListView_UserList,
                                          "",                     // pas de filtre sur le nom  (on les veut tous)
                                          "",                     // pas de filtre sur le  prenom (on les veut tous)
                                          TRUE,                   // ok pour remplie aussi les utilisateurs delegues
                                          specialite              // filtre sur la specialite
                                        );
     connect( m_ui.ListView_UserList, SIGNAL(itemChanged( QTreeWidgetItem * , int)), this, SLOT(ListView_UserList_LineChanged( QTreeWidgetItem * , int )) );
     connect( m_ui.ListView_UserList, SIGNAL(itemClicked( QTreeWidgetItem * , int)), this, SLOT(ListView_UserList_LineChanged( QTreeWidgetItem * , int )) );
}

//------------------------ changeEvent ---------------------------------------
void C_Frm_UserList::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui.retranslateUi(this);
        break;
    default:
        break;
    }
}


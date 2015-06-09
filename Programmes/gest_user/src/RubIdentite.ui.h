/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <qpixmap.h> 
#include "Dlg_PermsDefine.h"
#include "../../MedinTuxTools/Theme.h"  // Gestion du thème de l'appli
//=============================================== IMPLEMENTTION =====================================================
//--------------------------------- init --------------------------------------------------------------
void FormRubIdent::init()
{m_pQPopupMenu      = 0;
 m_CurrentPopupItem = "";
 m_RetPopupItem     = "";

 frameEtablissement->hide();
 m_CriptedPassWord = G_pCApp->m_CriptedPassWord;
 connect( lineEdit_Login,              SIGNAL(lostFocus()),
           this,                       SLOT  (OnLineEdit_Login_lostFocus())
           );
 QPixmap m_PixMapMoins = Theme::getIcon("16x16/Moins.png");
 QPixmap m_PixMapPlus  = Theme::getIcon("16x16/Plus.png");
 pushButtonSaveNew->setIconSet ( QIconSet (Theme::getIcon("22x22/filesave.png"))) ;
 pushButtonAppartenance->setIconSet ( QIconSet (Theme::getIcon("16x16/groupe.png"))) ;
 pushButtonAddDroit->setIconSet ( QIconSet ( m_PixMapPlus )) ;
 pushButtonRemoveDroit->setIconSet ( QIconSet ( m_PixMapMoins )) ;        
 pushButtonAnnulerCreation->setIconSet ( QIconSet (Theme::getIcon("exit.png"))) ;      
 if (G_pCApp->m_Droits.find("adm")==-1)
    {pushButtonAddDroit->setEnabled(FALSE);
     pushButtonRemoveDroit->setEnabled(FALSE);
     pushButtonAppartenance->setEnabled(FALSE);
    }
 else
   {pushButtonAddDroit->setEnabled(TRUE);
    pushButtonRemoveDroit->setEnabled(TRUE);
    pushButtonAppartenance->setEnabled(TRUE);
   }

connect( lineEdit_UserTitre    , SIGNAL( textChanged ( const QString & ) ),  this,   SLOT(   OnTextChanged_lineEdit_UserTitre(const QString & )));
connect( lineEdit_Nom          , SIGNAL( textChanged ( const QString & ) ),  this,   SLOT(   OnTextChanged_lineEdit_Nom(const QString & )));
connect( lineEdit_Prenom       , SIGNAL( textChanged ( const QString & ) ),  this,   SLOT(   OnTextChanged_lineEdit_Prenom(const QString & )));
connect( lineEdit_Login        , SIGNAL( textChanged ( const QString & ) ),  this,   SLOT(   OnTextChanged_lineEdit_Login(const QString & )));
connect( comboBoxSexe          , SIGNAL( activated   ( const QString & ) ),  this,   SLOT(   OnTextChanged_comboBoxSexe( const QString &)));
connect( lineEdit_Qualif       , SIGNAL( textChanged ( const QString & ) ),  this,   SLOT(   OnTextChanged_lineEdit_Qualif(const QString & )));
connect( lineEdit_NumOrdinal   , SIGNAL( textChanged ( const QString & ) ),  this,   SLOT(   OnTextChanged_lineEdit_NumOrdinal(const QString & )));
connect( lineEdit_NumRpps      , SIGNAL( textChanged ( const QString & ) ),  this,   SLOT(   OnTextChanged_lineEdit_NumRpps(const QString & )));
connect( lineEdit_ClefRpps     , SIGNAL( textChanged ( const QString & ) ),  this,   SLOT(   OnTextChanged_lineEdit_ClefRpps(const QString & )));
connect( lineEdit_Convention   , SIGNAL( textChanged ( const QString & ) ),  this,   SLOT(   OnTextChanged_lineEdit_Convention(const QString & )));
connect( lineEdit_Droits       , SIGNAL( textChanged (                 ) ),  this,   SLOT(   OnTextChanged_lineEdit_Droits( )));
connect( textEdit_Adresse      , SIGNAL( textChanged ( ) )                ,  this,   SLOT(   OnTextChanged_textEdit_Adresse( )));
connect( lineEdit_CdPostal     , SIGNAL( textChanged ( const QString & ) ),  this,   SLOT(   OnTextChanged_lineEdit_CdPostal(const QString & )));
connect( lineEdit_Ville        , SIGNAL( textChanged ( const QString & ) ),  this,   SLOT(   OnTextChanged_lineEdit_Ville(const QString & )));

connect( comboBoxTelTyp_1->lineEdit()  , SIGNAL( textChanged ( const QString & ) ),  this,   SLOT(   OnTextChanged_comboBoxTelTyp_1(const QString & )));
connect( comboBoxTelTyp_2->lineEdit()  , SIGNAL( textChanged ( const QString & ) ),  this,   SLOT(   OnTextChanged_comboBoxTelTyp_2(const QString & )));
connect( comboBoxTelTyp_3->lineEdit()  , SIGNAL( textChanged ( const QString & ) ),  this,   SLOT(   OnTextChanged_comboBoxTelTyp_3(const QString & )));
connect( lineEdit_Tel_1                , SIGNAL( textChanged ( const QString & ) ),  this,   SLOT(   OnTextChanged_lineEdit_Tel_1(const QString & )));
connect( lineEdit_Tel_2                , SIGNAL( textChanged ( const QString & ) ),  this,   SLOT(   OnTextChanged_lineEdit_Tel_2(const QString & )));
connect( lineEdit_Tel_3                , SIGNAL( textChanged ( const QString & ) ),  this,   SLOT(   OnTextChanged_lineEdit_Tel_3(const QString & )));
connect( lineEdit_Code_1               , SIGNAL( textChanged ( const QString & ) ),  this,   SLOT(   OnTextChanged_lineEdit_Code_1(const QString & )));
connect( lineEdit_Code_2               , SIGNAL( textChanged ( const QString & ) ),  this,   SLOT(   OnTextChanged_lineEdit_Code_2(const QString & )));
connect( lineEdit_Code_3               , SIGNAL( textChanged ( const QString & ) ),  this,   SLOT(   OnTextChanged_lineEdit_Code_3(const QString & )));
connect( lineEdit_Email                , SIGNAL( textChanged ( const QString & ) ),  this,   SLOT(   OnTextChanged_lineEdit_Email(const QString & )));
connect( listViewFriendUserList        , SIGNAL( mouseButtonClicked ( int, QListViewItem*, const QPoint &, int )),
         this,                           SLOT( On_listViewFriendUserList_mouseButtonClicked ( int, QListViewItem*, const QPoint &, int )));
         
 connect( comboBoxTelTyp_1->lineEdit(), SIGNAL( lostFocus ()), this,     SLOT(   On_ComboBoxLostFocus_TelTyp_1()) );
 connect( comboBoxTelTyp_2->lineEdit(), SIGNAL( lostFocus ()), this,     SLOT(   On_ComboBoxLostFocus_TelTyp_2()) );
 connect( comboBoxTelTyp_3->lineEdit(), SIGNAL( lostFocus ()), this,     SLOT(   On_ComboBoxLostFocus_TelTyp_3()) );
 
 InitAllComboTelType();

 textBrowserInfoPerms->mimeSourceFactory()->setExtensionType("html", "text/xml;charset=UTF-8");
 textBrowserInfoPerms->mimeSourceFactory()->addFilePath (G_pCApp->m_PathAppli + "/Images" );
 textBrowserInfoPerms->setSource (G_pCApp->m_PathAppli + "/Images/InfoPermsUser.html" );
}

//--------------------------------- On_listViewFriendUserList_mouseButtonClicked  -----------------------------------------------------------------------
void  FormRubIdent::On_listViewFriendUserList_mouseButtonClicked ( int button, QListViewItem * pQListViewItem, const QPoint &p, int c)
{if (button != 1) return;
 if (c==3 || c==4)
    {listViewFriendUserList_contextMenuRequested( pQListViewItem, p, c );
    }
}

//--------------------------------- On_ComboBoxLostFocus_TelTyp_1 -----------------------------------------------------------------------
void  FormRubIdent::On_ComboBoxLostFocus_TelTyp_1()
{QString str =  comboBoxTelTyp_1->lineEdit()->text().stripWhiteSpace();
 AddThisItemIfNotExist(comboBoxTelTyp_1, str);
 AddThisItemIfNotExist(comboBoxTelTyp_2, str);
 AddThisItemIfNotExist(comboBoxTelTyp_3, str);
}

//--------------------------------- On_ComboBoxLostFocus_TelTyp_2 -----------------------------------------------------------------------
void  FormRubIdent::On_ComboBoxLostFocus_TelTyp_2()
{QString str =  comboBoxTelTyp_2->lineEdit()->text().stripWhiteSpace();
 AddThisItemIfNotExist(comboBoxTelTyp_1, str);
 AddThisItemIfNotExist(comboBoxTelTyp_2, str);
 AddThisItemIfNotExist(comboBoxTelTyp_3, str);
}

//--------------------------------- On_ComboBoxLostFocus_TelTyp_3 -----------------------------------------------------------------------
void  FormRubIdent::On_ComboBoxLostFocus_TelTyp_3()
{QString str =  comboBoxTelTyp_3->lineEdit()->text().stripWhiteSpace();
 AddThisItemIfNotExist(comboBoxTelTyp_1, str);
 AddThisItemIfNotExist(comboBoxTelTyp_2, str);
 AddThisItemIfNotExist(comboBoxTelTyp_3, str);
}

//--------------------------------- InitAllComboTelType -----------------------------------------------------------------------
void  FormRubIdent::InitAllComboTelType()
{ if (G_pCApp->m_pCMoteurBase->OpenBase()==0)   return ;
     QStringList itemList;
     QString val_1, val_2, val_3;

       {
        //.................. fabriquer la requete ........................
        //                   de selection de l'enregistrement
        //                   correspondant � la cle
        QString requete ("SELECT ");
        requete     += G_pCApp->m_pCMoteurBase->m_USER_IDENT_TEL_TYPE_1      + ","
                     + G_pCApp->m_pCMoteurBase->m_USER_IDENT_TEL_TYPE_2      + ","
                     + G_pCApp->m_pCMoteurBase->m_USER_IDENT_TEL_TYPE_3      + " " ;

        requete += " FROM " + G_pCApp->m_pCMoteurBase->m_USER_IDENT_TBL_NAME;
        //char *pt = (char*) (const char*) requete;
        QSqlQuery query(requete, G_pCApp->m_pCMoteurBase->m_DataBase);
        if (query.isActive())
           {while (query.next())
                  { val_1 = G_pCApp->m_pCMoteurBase->Utf8_Query(query,0).stripWhiteSpace();
                    val_2 = G_pCApp->m_pCMoteurBase->Utf8_Query(query,1).stripWhiteSpace();
                    val_3 = G_pCApp->m_pCMoteurBase->Utf8_Query(query,2).stripWhiteSpace();

                    if (val_1.length() && itemList.findIndex ( val_1 )==-1) itemList.append(val_1);
                    if (val_2.length() && itemList.findIndex ( val_2 )==-1) itemList.append(val_2);
                    if (val_3.length() && itemList.findIndex ( val_3 )==-1) itemList.append(val_3);
                  }
           }
        else  OutSQL_error(query , "InitAllComboTelType()", requete );
       }
     G_pCApp->m_pCMoteurBase->CloseBase();
     comboBoxTelTyp_1->insertStringList ( itemList );
     comboBoxTelTyp_2->insertStringList ( itemList );
     comboBoxTelTyp_3->insertStringList ( itemList );
}

//--------------------------------- AddThisItemIfNotExist -----------------------------------------------------------------------
void  FormRubIdent::AddThisItemIfNotExist(QComboBox *pQComboBox, const QString &str)
{ if (str.length()==0) return;
  QListBox         *pQListBox =  pQComboBox->listBox ();
  QListBoxItem *pQListBoxItem =  pQListBox->findItem ( str, Qt::ExactMatch );
  if (pQListBoxItem==0) pQComboBox->insertItem (str);
}

//--------------------------------- OnTextChanged_lineEdit_Tel_1  -------------------------------------------------------------
void FormRubIdent::OnTextChanged_lineEdit_Tel_1(const QString & ){NotifyIdentChangedToGestUser("");}
//--------------------------------- OnTextChanged_lineEdit_Tel_2  -------------------------------------------------------------
void FormRubIdent::OnTextChanged_lineEdit_Tel_2(const QString & ){NotifyIdentChangedToGestUser("");}
//--------------------------------- OnTextChanged_lineEdit_Tel_3  -------------------------------------------------------------
void FormRubIdent::OnTextChanged_lineEdit_Tel_3(const QString & ){NotifyIdentChangedToGestUser("");}
//--------------------------------- OnTextChanged_lineEdit_Code_1  -------------------------------------------------------------
void FormRubIdent::OnTextChanged_lineEdit_Code_1(const QString & ){NotifyIdentChangedToGestUser("");}
//--------------------------------- OnTextChanged_lineEdit_Code_2  -------------------------------------------------------------
void FormRubIdent::OnTextChanged_lineEdit_Code_2(const QString & ){NotifyIdentChangedToGestUser("");}
//--------------------------------- OnTextChanged_lineEdit_Code_3  -------------------------------------------------------------
void FormRubIdent::OnTextChanged_lineEdit_Code_3(const QString & ){NotifyIdentChangedToGestUser("");}
//--------------------------------- OnTextChanged_lineEdit_Email  -------------------------------------------------------------
void FormRubIdent::OnTextChanged_lineEdit_Email(const QString & ) {NotifyIdentChangedToGestUser("");}
//--------------------------------- OnTextChanged_comboBoxTelTyp_1  -------------------------------------------------------------
void FormRubIdent::OnTextChanged_comboBoxTelTyp_1(const QString & ){NotifyIdentChangedToGestUser("");}
//--------------------------------- OnTextChanged_comboBoxTelTyp_2  -------------------------------------------------------------
void FormRubIdent::OnTextChanged_comboBoxTelTyp_2(const QString & ){NotifyIdentChangedToGestUser("");}
//--------------------------------- OnTextChanged_comboBoxTelTyp_3  -------------------------------------------------------------
void FormRubIdent::OnTextChanged_comboBoxTelTyp_3(const QString & ){NotifyIdentChangedToGestUser("");}
//--------------------------------- OnTextChanged_lineEdit_UserTitre  -------------------------------------------------------------
void FormRubIdent::OnTextChanged_lineEdit_UserTitre(const QString & ) {NotifyIdentChangedToGestUser("");}
//--------------------------------- OnTextChanged_lineEdit_Nom  -------------------------------------------------------------------
void FormRubIdent::OnTextChanged_lineEdit_Nom(const QString & ) { NotifyIdentChangedToGestUser("");}
//--------------------------------- OnTextChanged_lineEdit_Prenom  -------------------------------------------------------------
void FormRubIdent::OnTextChanged_lineEdit_Prenom(const QString & ) { NotifyIdentChangedToGestUser("");}
//--------------------------------- OnTextChanged_lineEdit_Login  -------------------------------------------------------------
void FormRubIdent::OnTextChanged_lineEdit_Login(const QString & )     { NotifyIdentChangedToGestUser("");}
//--------------------------------- OnTextChanged_comboBoxSexe  -------------------------------------------------------------
void FormRubIdent::OnTextChanged_comboBoxSexe(const QString & ) { NotifyIdentChangedToGestUser("");}
//--------------------------------- lineEdit_Qualif  -------------------------------------------------------------
void FormRubIdent::OnTextChanged_lineEdit_Qualif(const QString & ) { NotifyIdentChangedToGestUser("");}
//--------------------------------- lineEdit_NumOrdinal  -------------------------------------------------------------
void FormRubIdent::OnTextChanged_lineEdit_NumOrdinal(const QString & ) { NotifyIdentChangedToGestUser("");}
//--------------------------------- lineEdit_NumRpps -------------------------------------------------------------
void FormRubIdent::OnTextChanged_lineEdit_NumRpps(const QString & ) { NotifyIdentChangedToGestUser("");}
//--------------------------------- lineEdit_ClefRpps -------------------------------------------------------------
void FormRubIdent::OnTextChanged_lineEdit_ClefRpps(const QString & ) { NotifyIdentChangedToGestUser("");}
//--------------------------------- lineEdit_Convention  -------------------------------------------------------------
void FormRubIdent::OnTextChanged_lineEdit_Convention(const QString & ) {NotifyIdentChangedToGestUser("");}
//--------------------------------- lineEdit_Droits  -------------------------------------------------------------
void FormRubIdent::OnTextChanged_lineEdit_Droits()
{setPermsInterfaceOnDroits(lineEdit_Droits->text());
 NotifyIdentChangedToGestUser("");
}
//--------------------------------- textEdit_Adresse  -------------------------------------------------------------
void FormRubIdent::OnTextChanged_textEdit_Adresse() { NotifyIdentChangedToGestUser("");}
//--------------------------------- lineEdit_CdPostal  -------------------------------------------------------------
void FormRubIdent::OnTextChanged_lineEdit_CdPostal(const QString & ) {NotifyIdentChangedToGestUser("");; }
//--------------------------------- lineEdit_Ville  -------------------------------------------------------------
void FormRubIdent::OnTextChanged_lineEdit_Ville(const QString & ) { NotifyIdentChangedToGestUser("");}
//--------------------------------- NotifyIdentChangedToGestUser  -------------------------------------------------------------
void FormRubIdent::NotifyIdentChangedToGestUser(const QString & ) { pushButtonSaveNew->setEnabled(TRUE); emit Sign_RubChanged();}


//--------------------------------- listViewFriendUserList_contextMenuRequested  -------------------------------------------------------------
void FormRubIdent::listViewFriendUserList_contextMenuRequested( QListViewItem *pQListViewItem, const QPoint &, int pos )
{if (pQListViewItem<=0)
    {pushButtonAddPermsUser_clicked();
     return;
    }
         QString data_ini="";
         QStringList popItem;
         popItem.append(tr("Définir ou modifier les droits délégués..."));
         popItem.append( "---------");
         popItem.append(tr("Placer les droits au maximum"));
         popItem.append(tr("Placer les droits minimum"));
         popItem.append( "---------");
         popItem.append(tr("Retirer la délégation de droits"));
         popItem.append( "---------");

         QString chaineDroits = G_pCApp->GetListDroitsPossibles();
         QString popRet       = DoPopupList(popItem);
         if (popRet.length())
            {QString droits       = pQListViewItem->text(5);               // recuperer chaine droits de la zone de saisie
             if (popRet==tr("Placer les droits minimum"))
                 {droits = "";
                  SetPermsUserListDroits(droits);
                  NotifyIdentChangedToGestUser("");
                  return;
                 }
             if (popRet==tr("Placer les droits au maximum"))
                {chaineDroits = chaineDroits.remove("-adm");
                 chaineDroits = chaineDroits.remove("-sgn");
                 chaineDroits = chaineDroits.remove("-med");
                 SetPermsUserListDroits(chaineDroits);
                 NotifyIdentChangedToGestUser("");
                 return;
                }
             if (popRet==tr("Retirer la délégation de droits"))
                 {pushButtonRemovePermsUser_clicked();
                  return;
                 }
             if (popRet==tr("Définir ou modifier les droits délégués..."))
                 {if (DoAssistantDroits(droits, 1))
                     {droits = droits.remove("-adm");
                      droits = droits.remove("-sgn");
                      droits = droits.remove("-med");
                      SetPermsUserListDroits(droits);
                      NotifyIdentChangedToGestUser("");
                     }
                  return;
                 }
            }
}

//------------------------------------------- pushButtonAddPermsUser_clicked ------------------------------------------------------------------------
void FormRubIdent::pushButtonAddPermsUser_clicked()
{exeAssistantUserChoice( );
}

//------------------------------------------- SavePermsUserList ------------------------------------------------------------------------
void FormRubIdent::SavePermsUserList(QString signGUID)
{if (m_NumGUID.length()<=0) return;
 QListViewItemIterator it( listViewFriendUserList );
 while ( it.current() )
       {QListViewItem *item = it.current();
        QString pk = m_pCMoteurBase->PermsUserCreateOrUpdate(item->text(6) ,        // pk   (si "" alors ce sera une creation et pk sera retourné
                                                             item->text(5) ,        // chaine des droits
                                                             signGUID     ,         // GUID du signataire (le maitre à bord)
                                                             item->text(7) ,        // GUID de l'utilisateur autorisé (le larbin)
                                                             item->text(8));        // blobs
        if (pk.length()) item->setText(6, pk);
        ++it;
       }
}

//------------------------------------------- pushButtonRemovePermsUser_clicked ------------------------------------------------------------------------
void FormRubIdent::SetPermsUserListDroits(const QString &droits)
{   QListViewItemIterator it( listViewFriendUserList );
    while ( it.current() )
       {if ( it.current()->isSelected() ) it.current()->setText(5, droits);
        ++it;
       }
}

//------------------------------------------- pushButtonRemovePermsUser_clicked ------------------------------------------------------------------------
void FormRubIdent::pushButtonRemovePermsUser_clicked()
{if (QMessageBox::warning( this, tr("Effacement d'un Utilisateur : "),
                                 tr("<b><u>ATTENTION</b></u> :  vous êtes sur le point\n"
                                    "            d'effacer un utilisateur de la liste\n"
                                    "            des autorisés. Cette action immédiate, est définitive"),
                                 tr("Annuler"),tr("Effacer")
                         ) ==1
    )
    {
     //............... relever tous les pointeurs des items ..................
     QPtrList<QListViewItem> list;
     list.setAutoDelete ( TRUE );
     QListViewItemIterator it( listViewFriendUserList );
     while ( it.current() )
           {if ( it.current()->isSelected() )  {list.append( it.current() );
                                                m_pCMoteurBase->PermsUserDeleteFromPk(it.current()->text(6) );
                                               }
            ++it;
           }
    }
}

//------------------------------------------- lineEdit_Droits_textChanged ------------------------------------------------------------------------
void FormRubIdent::lineEdit_Droits_textChanged( const QString &droits )
{setPermsInterfaceOnDroits(droits);
}

//------------------------------------------- setPermsInterfaceOnDroits ------------------------------------------------------------------------
void FormRubIdent::setPermsInterfaceOnDroits(QString droits)
{if (droits=="*") droits = lineEdit_Droits->text();
 int indTab = tabWidgetUser->indexOf (Tab_FriendUser) ;
 //................................... droit signataire ..................................................
 if ( droits.find("adm")!=-1 || droits.find("sgn")!=-1)
    {tabWidgetUser->page( indTab )->setEnabled(TRUE);
    }
 else
    {tabWidgetUser->page( indTab )->setEnabled(FALSE);
    }
}

//------------------------------------------- getCurrentPageID_ByTitle ------------------------------------------------------------------------
int FormRubIdent::getCurrentPageID_ByTitle(QTabWidget *pQTabWidget, const QString &title)
{int nb = pQTabWidget->count();
 for ( int i=0; i<nb; ++i)
     {if (pQTabWidget->label(i) == title) return i;
     }
 return -1;
}

//--------------------------------- OnLineEdit_Login_lostFocus-------------------------------------------------------------
void FormRubIdent::OnLineEdit_Login_lostFocus()           // pour que lineEdit_Login soit éditable c'est qu'on est forcément en mode creation
{QString      login    = lineEdit_Login->text();
 QString     testLogin = m_pCMoteurBase->GetFieldValue(m_pCMoteurBase->m_USER_IDENT_TBL_NAME,
                                                       m_pCMoteurBase->m_USER_IDENT_LOGIN,
                                                       m_pCMoteurBase->m_USER_IDENT_LOGIN,
                                                       login );
 if (testLogin.length())
    { QMessageBox::critical( this, tr("Erreur: Cet identifiant existe déjà"),
                                   tr("Cet identifiant existe déjà \nVeuillez en choisir un autre"),
                                   tr("Annuler")
                       ) ;
     lineEdit_Login->setFocus();
    }

}

//--------------------------------- pushButtonAnnulerCreation_clicked -------------------------------------------------------------
void FormRubIdent::pushButtonAnnulerCreation_clicked()
{ QObject::disconnect ( lineEdit_Login, SIGNAL(lostFocus()),
                        this, SLOT (OnLineEdit_Login_lostFocus()) );
  emit Sign_CancelButtonClicked();
  connect( lineEdit_Login,              SIGNAL(lostFocus()),
           this,                        SLOT  (OnLineEdit_Login_lostFocus())
           );
}

//--------------------------------- pushButtonSaveNew_clicked-------------------------------------------------------------
void FormRubIdent::pushButtonSaveNew_clicked()
{    QObject::disconnect ( lineEdit_Login, SIGNAL(lostFocus()), this,   SLOT (OnLineEdit_Login_lostFocus()) );
     if (pushButtonAnnulerCreation->isVisible())       // que si mode création vérifier si l'identifiant n'est pas déjà utilisé
        {QString      login    = lineEdit_Login->text();
         QString     testLogin = m_pCMoteurBase->GetFieldValue(m_pCMoteurBase->m_USER_IDENT_TBL_NAME,
                                                               m_pCMoteurBase->m_USER_IDENT_LOGIN,
                                                               m_pCMoteurBase->m_USER_IDENT_LOGIN,
                                                               login );
         if (testLogin.length())
            {    QMessageBox::critical( this, tr("Erreur: Cet identifiant existe déjà"),
                                              tr("Cet identifiant existe déjà\nVeuillez en choisir un autre"),
                                              tr("Annuler")
                                       ) ;

                 connect( lineEdit_Login,   SIGNAL(lostFocus()), this,    SLOT  (OnLineEdit_Login_lostFocus()) );
                 return ;
            }
       }
    emit Sign_SaveButtonClicked();
    SavePermsUserList(m_NumGUID);
    connect( lineEdit_Login,              SIGNAL(lostFocus()),   this,    SLOT  (OnLineEdit_Login_lostFocus()) );
   pushButtonSaveNew->setEnabled(FALSE);
}

//--------------------------------- SetData-------------------------------------------------------------

void FormRubIdent::SetData()
{if (m_pCMoteurBase==0) return;
 //.......................... creation d'un nouvel utilisateur ...............................
 if (m_ID_Doss=="")
    {clearData();
     //pushButtonSaveNew->show();
    }
 else
 //......................... lire les données d'un utilisateur deja present ..................
    {GetUserIdentiteFromPrimKey(m_ID_Doss);
     //pushButtonSaveNew->hide();
    }
 lineEdit_Login->setEnabled(FALSE);
 pushButtonSaveNew->setEnabled(FALSE);
}

//--------------------------------- SetPassWord -------------------------------------------------------------
void FormRubIdent::SetPassWord(const char* password)
{m_CriptedPassWord = password;
}

//--------------------------------- QueryToFormulaire -------------------------------------------------------------
/*
   QString      m_USER_IDENT_TBL_NAME;           // nom de la table des données des RUBRIQUES
   QString      m_USER_IDENT_NOM;                // Nom
   QString      m_USER_IDENT_PRENOM;             // Prenom
   QString      m_USER_IDENT_TITRE;              // Titre( Dr Mr Md Pr ...)
   QString      m_USER_IDENT_LOGIN;              // Identifiant d'utilisateur
   QString      m_USER_IDENT_PASSWORD;           // mot de passe
   QString      m_USER_IDENT_DROIT;              // Droits de cet utilisateur
   QString      m_USER_IDENT_NUM_ORDRE;          // Numéro d'ordre (si il y en a un)
   QString      m_USER_IDENT_PRIMKEY;            // clef primaire de cet enregistrement
   QString      m_USER_IDENT_GUID;               // Identifiant unique de cet utilisateur
   QString      m_USER_IDENT_CONVENTION;         // convention exple "01 MEDECINE GENERALE"
   QString      m_USER_IDENT_CODE_POST;          // Code postal
   QString      m_USER_IDENT_VILLE;              // Ville d'exercice
   QString      m_USER_IDENT_RUE;                // Rue d'exercice
   QString      m_USER_IDENT_QUALITE;            // Type d'intervenant (exple: Medecin Urgentiste)
*/
int FormRubIdent::GetUserIdentiteFromPrimKey( QString &primKey)
{
   QString requete("SELECT ");
   //................... ajouter les champs à selectionner .......................
   requete  += m_pCMoteurBase->m_USER_IDENT_TITRE        + "," +      // 0
               m_pCMoteurBase->m_USER_IDENT_NOM          + "," +      // 1
               m_pCMoteurBase->m_USER_IDENT_PRENOM       + "," +      // 2
               m_pCMoteurBase->m_USER_IDENT_LOGIN        + "," +      // 3
               m_pCMoteurBase->m_USER_IDENT_PASSWORD     + "," +      // 4
               m_pCMoteurBase->m_USER_IDENT_QUALITE      + "," +      // 5
               m_pCMoteurBase->m_USER_IDENT_DROITS       + "," +      // 6
               m_pCMoteurBase->m_USER_IDENT_SEXE         + "," +      // 7
               m_pCMoteurBase->m_USER_IDENT_NUM_ORDRE    + "," +      // 8
               m_pCMoteurBase->m_USER_IDENT_NUM_RPPS     + "," +      // 9
               m_pCMoteurBase->m_USER_IDENT_CLEF_RPPS    + "," +      // 10
               m_pCMoteurBase->m_USER_IDENT_CONVENTION   + "," +      // 11
               m_pCMoteurBase->m_USER_IDENT_RUE          + "," +      // 12
               m_pCMoteurBase->m_USER_IDENT_CODE_POST    + "," +      // 13
               m_pCMoteurBase->m_USER_IDENT_VILLE        + "," +      // 14
               m_pCMoteurBase->m_USER_IDENT_GUID         + "," +      // 15
               m_pCMoteurBase->m_USER_IDENT_TEL_1         + "," +      // 16
               m_pCMoteurBase->m_USER_IDENT_TEL_2         + "," +      // 17
               m_pCMoteurBase->m_USER_IDENT_TEL_3         + "," +      // 18
               m_pCMoteurBase->m_USER_IDENT_EMAIL         + "," +      // 19
               m_pCMoteurBase->m_USER_IDENT_NOTE          + "," +      // 20
               m_pCMoteurBase->m_USER_IDENT_CODE_REGIME   + "," +      // 21
               m_pCMoteurBase->m_USER_IDENT_CHER          + "," +      // 22
               m_pCMoteurBase->m_USER_IDENT_TEL_TYPE_1    + "," +      // 23
               m_pCMoteurBase->m_USER_IDENT_TEL_TYPE_2    + "," +      // 24
               m_pCMoteurBase->m_USER_IDENT_TEL_TYPE_3    + "," +      // 25
               m_pCMoteurBase->m_USER_IDENT_TEL_ABR_1     + "," +      // 26
               m_pCMoteurBase->m_USER_IDENT_TEL_ABR_2     + "," +      // 27
               m_pCMoteurBase->m_USER_IDENT_TEL_ABR_3     ;            // 28

 //............... continuer à construire la requete ...........................
 requete  += " FROM "     + m_pCMoteurBase->m_USER_IDENT_TBL_NAME + " WHERE ";
 requete  += m_pCMoteurBase->m_USER_IDENT_PRIMKEY + " ='"         + primKey + "'";
 // CGestIni::Param_UpdateToDisk("C:/requete.txt",requete);
 if (m_pCMoteurBase->OpenBase()==0)     return 0;
 QSqlQuery  query(requete , m_pCMoteurBase->m_DataBase );
 if (query.isActive() && query.next())
    {lineEdit_UserTitre->setText(m_pCMoteurBase->Utf8_Query(query, 0));
     lineEdit_Nom->setText(m_pCMoteurBase->Utf8_Query(query,1));
     lineEdit_Prenom->setText(m_pCMoteurBase->Utf8_Query(query,2));
     lineEdit_Login->setText(m_pCMoteurBase->Utf8_Query(query,3));
     m_CriptedPassWord = m_pCMoteurBase->Utf8_Query(query,4);
     lineEdit_Qualif->setText(m_pCMoteurBase->Utf8_Query(query,5));
     QString droits = m_pCMoteurBase->Utf8_Query(query,6);   // recuperer chaine droits du disque
     G_pCApp->AddDroit( droits , "med");                     // placer le droit medintux de base pour etre certain qu'il y est
     lineEdit_Droits->setText(droits);
     QString sex = m_pCMoteurBase->Utf8_Query(query,7);
     if (sex=="M") comboBoxSexe->setCurrentItem (0);
     else          comboBoxSexe->setCurrentItem (1);
     lineEdit_NumOrdinal->setText(m_pCMoteurBase->Utf8_Query(query,8));
     lineEdit_NumRpps->setText(m_pCMoteurBase->Utf8_Query(query,9));
     lineEdit_ClefRpps->setText(m_pCMoteurBase->Utf8_Query(query,10));
     lineEdit_Convention->setText(m_pCMoteurBase->Utf8_Query(query,11));
     textEdit_Adresse->setText(m_pCMoteurBase->Utf8_Query(query,12));
     lineEdit_CdPostal->setText(m_pCMoteurBase->Utf8_Query(query,13));
     lineEdit_Ville->setText(m_pCMoteurBase->Utf8_Query(query,14));
     m_NumGUID = m_pCMoteurBase->Utf8_Query(query,15);

     lineEdit_Tel_1->setText(m_pCMoteurBase->Utf8_Query(query, 16));
     lineEdit_Tel_2->setText(m_pCMoteurBase->Utf8_Query(query, 17));
     lineEdit_Tel_3->setText(m_pCMoteurBase->Utf8_Query(query, 18));
     lineEdit_Email->setText(m_pCMoteurBase->Utf8_Query(query, 19));
     comboBoxTelTyp_1->lineEdit()->setText(m_pCMoteurBase->Utf8_Query(query,23));
     comboBoxTelTyp_2->lineEdit()->setText(m_pCMoteurBase->Utf8_Query(query,24));
     comboBoxTelTyp_3->lineEdit()->setText(m_pCMoteurBase->Utf8_Query(query,25));

     lineEdit_Code_1->setText(m_pCMoteurBase->Utf8_Query(query,26));
     lineEdit_Code_2->setText(m_pCMoteurBase->Utf8_Query(query,27));
     lineEdit_Code_3->setText(m_pCMoteurBase->Utf8_Query(query,28));

     if (droits.find("sgn")!=-1 || droits.find("adm")!=-1)
        {G_pCApp->m_pCMoteurBase->PermsUserGetList( listViewFriendUserList, m_NumGUID, Theme::getPath()+"16x16/" );
        }
     else
        {listViewFriendUserList->clear();
        }
    } //if (pSqlQuery && pSqlQuery->isActive() && pSqlQuery->next())
 m_pCMoteurBase->CloseBase();
 return 1;
}

//--------------------------------- FormulaireToQSqlRecord -------------------------------------------------------------
void FormRubIdent::FormulaireToQSqlRecord( QSqlRecord *buffer , int keepMedUser)
{ buffer->setValue( m_pCMoteurBase->m_USER_IDENT_NOM,        lineEdit_Nom->text()         );  // y placer les données
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_PRENOM,     lineEdit_Prenom->text()      );  // y placer les données
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_TITRE,      lineEdit_UserTitre->text()   );  // y placer les données
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_LOGIN,      lineEdit_Login->text()       );  // y placer les données
  QString sex = "M"; if (comboBoxSexe->currentText()!=tr("Masculin"))  sex = "F";
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_SEXE,       sex      );                  // y placer les données
  QString droits = lineEdit_Droits->text();                   // recuperer chaine droits de la zone de saisie
  if (keepMedUser) G_pCApp->AddDroit( droits , "med");                         // placer le droit medintux de base pour etre certain qu'il y est
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_DROITS,     droits                       );  // y placer les données
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_NUM_ORDRE,  lineEdit_NumOrdinal->text()  );  // y placer les données
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_NUM_RPPS,   lineEdit_NumRpps->text()     );  // y placer les données
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_CLEF_RPPS,  lineEdit_ClefRpps->text()    );  // y placer les données
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_GUID,       m_NumGUID                    );  // y placer les données
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_CONVENTION, lineEdit_Convention->text()  );  // y placer les données
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_RUE,        textEdit_Adresse->text()     );  // y placer les données
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_CODE_POST,  lineEdit_CdPostal->text()    );  // y placer les données
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_VILLE,      lineEdit_Ville->text()       );  // y placer les données
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_QUALITE,    lineEdit_Qualif->text()      );  // y placer les données
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_PASSWORD,   m_CriptedPassWord      );        // y placer les données


  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_TEL_1, lineEdit_Tel_1->text());
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_TEL_2, lineEdit_Tel_2->text());
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_TEL_3, lineEdit_Tel_3->text());
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_EMAIL, lineEdit_Email->text());
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_TEL_TYPE_1, comboBoxTelTyp_1->lineEdit()->text());
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_TEL_TYPE_2, comboBoxTelTyp_2->lineEdit()->text());
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_TEL_TYPE_3, comboBoxTelTyp_3->lineEdit()->text());

  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_TEL_ABR_1, lineEdit_Code_1->text());
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_TEL_ABR_2, lineEdit_Code_2->text());
  buffer->setValue( m_pCMoteurBase->m_USER_IDENT_TEL_ABR_3, lineEdit_Code_3->text());
}

//--------------------------------- UpdateUserDataRecord -------------------------------------------------------------
int FormRubIdent::UpdateUserDataRecord(int keepMedUser )
{//................................. methode QSqlCursor .................................
 //                         ne pose pas de PB avec valeur avec une apostrophe
 //............... crer un curseur SQL ..................................................
 if (m_pCMoteurBase->OpenBase()==0)  return FALSE;
 int ok = FALSE;
 QSqlCursor cur(m_pCMoteurBase->m_USER_IDENT_TBL_NAME, TRUE, m_pCMoteurBase->m_DataBase);
 if ( !cur.canUpdate ()) {m_pCMoteurBase->CloseBase(); return FALSE;}
 //............... placer ce curseur sur le bon enregistrement ..........................
 QSqlError error;
 cur.select(m_pCMoteurBase->m_USER_IDENT_GUID  + "='" + m_NumGUID  + "'" );
  //............... si OK on recupere le buffer et on le charge avec les données .........
 if ( cur.next() )
    {QSqlRecord *buffer = cur.primeUpdate();                             // recuperer le buffer ( pas de delete à la fin !!)
     FormulaireToQSqlRecord(buffer, keepMedUser );
     ok    = cur.update();                                               // ecrire le buffer avec les données du header
     error = cur.lastError();
    }
 if (ok==0 && error.type() != QSqlError::None)
    {QString qserr;
     switch (error.type())
        { case    QSqlError::None:           qserr =" - no error occurred"          ; break;
          case    QSqlError::Connection:     qserr =" - connection error "          ; break;
          case    QSqlError::Statement:      qserr =" - SQL statement syntax error" ; break;
          case    QSqlError::Transaction:    qserr =" - transaction failed error"   ; break;
          case    QSqlError::Unknown:        qserr =" - unknown error "             ; break;
          default:                           qserr =" - unknown error "             ; break;
        }
     qDebug(qserr + error.databaseText ());
    }
  //......................... fermer la base ..............................................
  m_pCMoteurBase->CloseBase();
  return ok;
}

//--------------------------------- clearData()-------------------------------------------------------------
void FormRubIdent::clearData()
{
     //lineEdit_UserTitre->setText("");
     lineEdit_Nom->setText("");
     lineEdit_Prenom->setText("");
     lineEdit_Login->setText("");
     //lineEdit_PassWord->setText("");
     //lineEdit_Qualif->setText("");
     //lineEdit_Droits->setText("");
     lineEdit_NumOrdinal->setText("");
     //lineEdit_Convention->setText("");
     //textEdit_Adresse->setText("");
     //lineEdit_CdPostal->setText("");
     //lineEdit_Ville->setText("");
     m_NumGUID         = "";
     m_CriptedPassWord = "";
     pushButtonPassWord->setEnabled(FALSE);
    listViewFriendUserList->clear();
    pushButtonSaveNew->setEnabled(FALSE);
}



//--------------------------------- pushButtonPassWord_clicked-------------------------------------------------------------
void FormRubIdent::pushButtonPassWord_clicked()
{
 #define PASS_CHANGE    1
 #define PASS_GET       2

 QString       user = m_pCMoteurBase->GetFieldValue(m_pCMoteurBase->m_USER_IDENT_TBL_NAME,
                                                    m_pCMoteurBase->m_USER_IDENT_LOGIN,
                                                    m_pCMoteurBase->m_USER_IDENT_PRIMKEY,
                                                    m_ID_Doss);

//.............. afficher première fois pour demander mot de passe ..............................
//                (on ne peut le changer que si deja correct)
 DlgPassChange *dlg = new DlgPassChange(this);         //checkBoxMemorise
 if (dlg == 0)             return;

 //...................... reafficher le dialogue en mode modification ..........................
 dlg = new DlgPassChange(this);
 dlg->initDialog(m_pCMoteurBase, user, PASS_CHANGE);
 if (dlg->exec()==QDialog::Accepted)
    {m_CriptedPassWord        = dlg->m_PassWord;
     m_pCMoteurBase->PassWordEncode(m_CriptedPassWord );                                                    // encoder le password
     if ( m_NumGUID!="") m_pCMoteurBase->SaveUserPassWord(lineEdit_Login->text(), m_CriptedPassWord);       // le sauvegarder utilisateur déjà existant
     emit Sign_PassWordChanged(m_CriptedPassWord);                                                          // avertir tout le monde du changement
     NotifyIdentChangedToGestUser("");
    }
 delete dlg;
}

//--------------------------------- exeAssistantUserChoice -------------------------------------------------------------
void FormRubIdent::exeAssistantUserChoice( )
{   //.................. creer la liste des utilisateurs deja presents .......................
    QStringList userList;
    QListViewItemIterator it( listViewFriendUserList );
    while ( it.current() )
       {userList.append(it.current()->text(0));
        ++it;
       }
    if (lineEdit_Login->text().length()) userList.append(lineEdit_Login->text());
     //.................. lancer le dialogue sans les utilisateurs déjà présents .......................
     Dlg_PermsUserChoice *pDlg_PermsUserChoice = new Dlg_PermsUserChoice(0, "Dlg_PermsUserChoice", TRUE);
     if ( pDlg_PermsUserChoice )
        {
         pDlg_PermsUserChoice->initDialog(m_pCMoteurBase, &userList,"");
         pDlg_PermsUserChoice->exec();
         delete pDlg_PermsUserChoice;
         for ( QStringList::Iterator list_it = userList.begin(); list_it != userList.end(); ++list_it )
             { QStringList fiedList;
               fiedList.append( m_pCMoteurBase->m_USER_IDENT_LOGIN);
               fiedList.append( m_pCMoteurBase->m_USER_IDENT_NOM);
               fiedList.append( m_pCMoteurBase->m_USER_IDENT_PRENOM);
               fiedList.append( m_pCMoteurBase->m_USER_IDENT_GUID);

               QSqlQuery*  pQSqlQuery =  m_pCMoteurBase->GetUserIdentiteFromPrimKey(*list_it ,fiedList );
               if (pQSqlQuery!=0)
                  {  QString login = m_pCMoteurBase->Utf8_Query(*pQSqlQuery, 0  );
                     //.................... rajouter le login dans la liste ...................................................
                     QListViewItem *pQListViewItem = new QListViewItem( listViewFriendUserList,
                                                                        login,                                              // login
                                                                        m_pCMoteurBase->Utf8_Query(*pQSqlQuery, 1 ),        // nom
                                                                        m_pCMoteurBase->Utf8_Query(*pQSqlQuery, 2  ),       // prenom
                                                                        "",                                                 // +
                                                                        "",                                                 // -
                                                                        "",                                                 // droits
                                                                        "",                                                 // pk
                                                                        m_pCMoteurBase->Utf8_Query(*pQSqlQuery, 3  )        // GUID du permsUser
                                                                      );
                     if (pQListViewItem)
                        { pQListViewItem->setPixmap (3, m_PixMapPlus  ) ;
                          pQListViewItem->setPixmap (4, m_PixMapMoins ) ;
                        }
                    delete pQSqlQuery;
                  }  //end if (pQSqlQuery!=0)
              } //end for ( QStringList::Iterator list_it = userList.begin(); it != userList.end(); ++it )
        if (userList.count()) NotifyIdentChangedToGestUser("");
        m_pCMoteurBase->CloseBase();
      } //endif ( pDlg_PermsUserChoice )
}

//--------------------------------- pushButtonAddDroit_clicked-------------------------------------------------------------
void FormRubIdent::pushButtonAddDroit_clicked()
{QString     userDroit = m_pCMoteurBase->GetFieldValue(m_pCMoteurBase->m_USER_IDENT_TBL_NAME,
                                                        m_pCMoteurBase->m_USER_IDENT_DROITS,
                                                        m_pCMoteurBase->m_USER_IDENT_LOGIN,
                                                        lineEdit_Login->text() );
 QString data_ini = "";
 QStringList popItem;
 popItem.append(tr("Assistant de gestion des droits..."));
 popItem.append( "---------");
 popItem.append(tr("Placer tous les droits au maximum"));
 popItem.append( "---------");
 //QString chaineDroits = G_pCApp->GetListDroitsPossibles(&popItem,0);
 QString chaineDroits = G_pCApp->GetListDroitsPossibles();
 QString popRet       = DoPopupList(popItem);
 if (popRet.length())
    {QString droits   = lineEdit_Droits->text();               // recuperer chaine droits de la zone de saisie
     if (popRet==tr("Placer tous les droits au maximum"))
        {lineEdit_Droits->setText( chaineDroits );
         return;
        }
     if (popRet==tr("Assistant de gestion des droits..."))
        {Dlg_PermsDefine *dlg = new Dlg_PermsDefine(this, "MyDlgList",TRUE);
         if (dlg)
            {dlg->InitDialog(&droits, 0);
             if (dlg->exec() == QDialog::Accepted)
                {if ( !(pushButtonAnnulerCreation->isVisible())   &&       // si l'on est pas en mode creation (nouvel utilisateur)
                        m_pCMoteurBase->UserDroitsCount("adm")<=1 &&       // et qu'il ne reste plus qu'un seul administrateur
                        userDroit.find("adm") != -1               &&       // qui si celui en edition (y en a 1 seul et celui en édition en est un)
                        droits.find("adm") == -1                           // et droit admin non présent
                    )
                    {QMessageBox::warning( this, tr("Effacement d'un droit administrateur : "),
                                 tr("<b><u>OPÉRATION NON PERMISE</u></b> : <br>Vous essayez d'enlever le droit administrateur<br>"
                                    "           au seul utilisateur possédant encore ce droit.<br>"
                                    "           il faut au moins un utilisateur administrateur,<br>"
                                    "           afin de pouvoir gérer les autres utilisateurs.<br>"
                                    "           Le droit \"<b>adm</b>\" va donc être rajouté"),
                                 tr("Ok"));
                     droits += "-adm";                                  // placer en droit minimum l'administrateur
                    }
                 lineEdit_Droits->setText(droits);
                }
             delete dlg;
            }
         return;
        }
     }
}

//--------------------------------- pushButtonRemoveDroit_clicked-------------------------------------------------------------
void FormRubIdent::pushButtonRemoveDroit_clicked()
{ QString     userDroit = m_pCMoteurBase->GetFieldValue(m_pCMoteurBase->m_USER_IDENT_TBL_NAME,
                                                        m_pCMoteurBase->m_USER_IDENT_DROITS,
                                                        m_pCMoteurBase->m_USER_IDENT_LOGIN,
                                                        lineEdit_Login->text() );
 QString data_ini="";
 QStringList popItem;
 popItem.append(tr("Assistant de gestion des droits..."));
 popItem.append( "---------");
 popItem.append(tr("Placer les droits minimum"));
 popItem.append( "---------");

 QString popRet  = DoPopupList(popItem);
 QString droits  = lineEdit_Droits->text();               // recuperer chaine droits de la zone de saisie
 if (popRet.length())
    {
     if (popRet==tr("Placer les droits minimum"))
        {droits = "-med";
         if ( !(pushButtonAnnulerCreation->isVisible()) &&       // si l'on est pas en mode creation (nouvel utilisateur)
              m_pCMoteurBase->UserDroitsCount("adm")<=1 &&       // et qu'il ne reste plus qu'un seul administrateur
              userDroit.find("adm") != -1                        // qui si celui en edition (y en a 1 seul et celui en édition en est un)
            )
            {QMessageBox::warning( this, tr("Effacement d'un droit administrateur : "),
                                 tr("<b><u>OPÉRATION NON PERMISE</u></b> : <br>Vous essayez d'enlever le droit administrateur<br>"
                                    "           au seul utilisateur possédant encore ce droit.<br>"
                                    "           il faut au moins un utilisateur administrateur,<br>"
                                    "           afin de pouvoir gérer les autres utilisateurs.<br>"
                                    "           Le droit \"<b>adm</b>\" va donc être rajouté"),
                                 tr("Ok"));
             droits += "-adm";                                  // placer en droit minimum l'administrateur
            }
         lineEdit_Droits->setText(droits);
         return;
        }
     if (popRet==tr("Assistant de gestion des droits..."))
        {if (DoAssistantDroits(droits, 0))
            {if ( !(pushButtonAnnulerCreation->isVisible())       &&       // si l'on est pas en mode creation (nouvel utilisateur)
                        m_pCMoteurBase->UserDroitsCount("adm")<=1 &&       // et qu'il ne reste plus qu'un seul administrateur
                        userDroit.find("adm") != -1               &&       // qui si celui en edition (y en a 1 seul et celui en édition en est un)
                        droits.find("adm") == -1                           // et droit admin non présent
                    )
                     {QMessageBox::warning( this, tr("Effacement d'un droit administrateur : "),
                                 tr("<b><u>OPÉRATION NON PERMISE</u></b> : <br>Vous essayez d'enlever le droit administrateur<br>"
                                    "           au seul utilisateur possédant encore ce droit.<br>"
                                    "           il faut au moins un utilisateur administrateur,<br>"
                                    "           afin de pouvoir gérer les autres utilisateurs.<br>"
                                    "           Le droit \"<b>adm</b>\" va donc être rajouté"),
                                 tr("Ok"));
                      droits += "-adm";                                  // placer en droit minimum l'administrateur
                     }
                 lineEdit_Droits->setText(droits);
            }
         return;
        }
    }
}

//--------------------------------- DoPopupList ----------------------------------------------------------------------
QString FormRubIdent::DoPopupList(QStringList &list)
{

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
void FormRubIdent::OnMenuActivated (int id)
{if (id != -1 && m_pQPopupMenu)
    {m_CurrentPopupItem = m_pQPopupMenu->text(id);
    }
}

//--------------------------------- GetPopupItem  -------------------------------------------------------------
void FormRubIdent::GetPopupItem ()
{m_RetPopupItem  = m_CurrentPopupItem;
}


//--------------------------------- pushButtonAppartenance_clicked  -------------------------------------------------------------
void FormRubIdent::pushButtonAppartenance_clicked()
{Dlg_Appartenance *pDlg_Appartenance = new Dlg_Appartenance(0, "Dlg_PermsUserChoice", TRUE);
     if ( pDlg_Appartenance )
        {QStringList userList;
         pDlg_Appartenance->initDialog(lineEdit_Login->text());
         pDlg_Appartenance->exec();
         delete pDlg_Appartenance;
       }
}
//--------------------------------- DoAssistantDroits-------------------------------------------------------------
bool FormRubIdent::DoAssistantDroits(QString &droits, int mode)
{ int ret = FALSE;
  Dlg_PermsDefine *dlg = new Dlg_PermsDefine(this, "MyDlgList",TRUE);
  if (dlg)
     {dlg->InitDialog(&droits, mode);
      if (dlg->exec() == QDialog::Accepted)
        { ret = TRUE;
        }
     delete dlg;
   }
  return ret;
}
//--------------------------------- OutSQL_error ------------------------------------------------------------
void FormRubIdent::OutSQL_error(QSqlQuery &cur, const char *messFunc /*=0*/, const char *requete /*=0*/)
{    QSqlError error = cur.lastError();
     QString   qserr("");

     if (error.type() != QSqlError::None)
        {switch (error.type())
           { case    QSqlError::None:           qserr =" - no error occurred: "          ; break;
             case    QSqlError::Connection:     qserr =" - connection error: "           ; break;
             case    QSqlError::Statement:      qserr =" - SQL statement syntax error: " ; break;
             case    QSqlError::Transaction:    qserr =" - transaction failed error: "   ; break;
             case    QSqlError::Unknown:        qserr =" - unknown error: "              ; break;
             default:                           qserr =" - unknown error: "              ; break;
           }
         if (messFunc) qDebug(messFunc);
         qDebug(qserr + error.databaseText ());
         if (requete) qDebug(requete);
        }
//     qDebug(qserr + error.databaseText ());
}

/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <qtextcodec.h>
#include <qiconset.h>
#include <qdatetimeedit.h>
#include <qheader.h>
#include <qaction.h> 
//.................. index des champs .......................................
#define LIST_NOM       0
#define LIST_PRENOM    1
#define LIST_ID_KEY    2
#define LIST_GUID      3
#define F_GUID         0

#define NEW_IDENTITE 1
#define MODE_SAISIE_EN_COURS      1
#define MODE_SAISIE_NON_EN_COURS  0
#include "CDlg_MetaFieldDefine.h"
#include "CMetaFiltre.h"
#include "CDlg_ExportConfig.h"
#include "../../MedinTuxTools/Theme.h"

//----------------------------------------- init ----------------------------------------------------------------------
void Wdg_Central::init()
{   m_pCMoteurBase      = G_pCApp->m_pCMoteurBase;
    m_ID_Doss           = "";
    m_pQPopupMenu       = 0;
    m_IdentIsClear      = 1;
    G_pCApp->m_DenombreEnabled = true;
    G_pCApp->m_ResultEnabled   = true;
    listView_Sql_IdentFields->setSorting ( -1 );
    listView_Def_MetaFields->setSorting ( -1 );
    listView_Sql_MetaFields->setSorting ( -1 );

    m_CDenombreList.setAutoDelete ( TRUE );
    m_CMetaFieldList.setAutoDelete ( TRUE );

    m_PAGE_EPP  = tr("Evaluation Statistique");
    m_PAGE_EPP  = tabWidgetTypeEvaluation->tabLabel ( tabWidgetTypeEvaluation->page(1) );  //"P&rescription";

    tabWidgetTypeEvaluation->setCurrentPage( 0 );
    tabWidget_Dossiers->setCurrentPage( 0 );
    //................................ connexion .........................................................
    connect( mle_deb,                  SIGNAL( Sig_FocusOut()),                  this,     SLOT(   OnOutFocusDateHeure())  );
    connect( mle_fin,                  SIGNAL( Sig_FocusOut()),                  this,     SLOT(   OnOutFocusDateHeure())  );
    connect( mle_HeureDeb,             SIGNAL( Sig_FocusOut()),                  this,     SLOT(   OnOutFocusDateHeure())  );
    connect( mle_HeureFin,             SIGNAL( Sig_FocusOut()),                  this,     SLOT(   OnOutFocusDateHeure())  );
    connect( pushButtonRunStat,        SIGNAL( clicked()),                       this,     SLOT(   pushButtonRunStat_clicked())  );
    connect( ListView_DossiersRetenus, SIGNAL( doubleClicked (QListViewItem *)), this,     SLOT(   Slot_ListView_Dossiers_retenus_doubleClicked(QListViewItem *))  );
    
    connect( pushButton_Result_MaskList,         SIGNAL( clicked()),                         this,     SLOT( Slot_pushButton_Result_MaskList_Clicked())       );
    connect( pushButton_Results_Add_Field,       SIGNAL( clicked()),                         this,     SLOT( Slot_pushButton_Results_Add_Field_Clicked())     );
    connect( pushButton_Results_Del_Field,       SIGNAL( clicked()),                         this,     SLOT( Slot_pushButton_Results_Del_Field_Clicked())     );
    connect( pushButton_Results_Export,          SIGNAL( clicked()),                         this,     SLOT( Slot_ConfigExport()) );
    // connect( pushButton_Results_Config,          SIGNAL( clicked()),                         this,     SLOT( Slot_ConfigExport()) );
    connect( listView_Result_Fields,             SIGNAL( doubleClicked(QListViewItem *)),    this,     SLOT( Slot_listView_Result_Fields_DoubleClicked(QListViewItem* )) );

    QHeader *pQHeader = listView_Result_Fields->header();
    while ( pQHeader->count()>0 )
          { pQHeader->removeLabel ( 0 );
          } 
    pushButton_Results_Config->hide();  // plus besoin car pushButton_Results_Export() passe par le configurateur)
    //.................. images des boutons ..............................................................
    QString path_img = G_pCApp->m_PathAppli + "Images/";

    pushButton_MaskListSqlField->setPixmap ( QPixmap(path_img +"DownLitle.png") );
    pushButton_AddListIdentField->setPixmap ( QPixmap(path_img +"Plus.png") );
    pushButton_DelListIdentField->setPixmap ( QPixmap(path_img +"Moins.png") );

    pushButton_MaskListMetaField->setPixmap ( QPixmap(path_img +"DownLitle.png") );
    pushButton_AddListMetaField->setPixmap ( QPixmap(path_img +"Plus.png") );
    pushButton_DelListMetaField->setPixmap ( QPixmap(path_img +"Moins.png") );

    pushButton_AddDefMetaField->setPixmap ( QPixmap(path_img +"Plus.png") );
    pushButton_DelDefMetaField->setPixmap ( QPixmap(path_img +"Moins.png") );

    pushButton_UpDefMetaField->setPixmap ( QPixmap(path_img +"Haut.png") );
    pushButton_DownDefMetaField->setPixmap ( QPixmap(path_img +"Bas.png") );

    pushButton_MaskListClassDenombre->setPixmap ( QPixmap(path_img +"DownLitle.png") );
    pushButton_AddClassField->setPixmap ( QPixmap(path_img +"Plus.png") );
    pushButton_DelClassField->setPixmap ( QPixmap(path_img +"Moins.png") );

    pushButton_Result_MaskList->setPixmap ( QPixmap(path_img +"DownLitle.png") );
    pushButton_Results_Add_Field->setPixmap ( QPixmap(path_img +"Plus.png") );
    pushButton_Results_Del_Field->setPixmap ( QPixmap(path_img +"Moins.png") );

    pushButton_Results_Export->setIconSet ( Theme::getIconSet("filesaveExport.png" ) );
    pushButton_Results_Config->setIconSet ( Theme::getIconSet("config_export.png" ) );

    //....................................... texte d'information..............................................................
    QString txt;
    textBrowserInfoPerms->mimeSourceFactory()->setExtensionType("html", "text/xml;charset=UTF-8");
    textBrowserInfoPerms->mimeSourceFactory()->addFilePath (G_pCApp->m_PathAppli + "/Images" );
    textBrowserInfoPerms->setSource (G_pCApp->m_PathAppli + "/Images/InfoMetaChamps.html" );
    CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathAppli + "/Images/InfoMetaChamps.html", txt);
    txt.replace("meta name=\"qrichtext\" content=\"charset=utf-8\"",    "meta name=\"qrichtext\" content=\"1\"");
    textBrowserInfoPerms->setText(txt);
    //initFieldsList();
    //.................................................................................................................
    ///////////////////////////////////////////////////INIT ONGLET STATISTIQUES //////////////////////////////////////
    //........................ images des differents gadjets.................................

    m_GoDatePopup.setPixmap (QPixmap(path_img +"DownExpandArrow.xpm"),    QIconSet::Automatic);
    m_Cadenas.setPixmap (QPixmap(path_img +"Cadenas.png"),                QIconSet::Automatic);
    m_Standard.setPixmap (QPixmap(path_img +"standards.png"),             QIconSet::Automatic);

    //........................ dates et heure des stat.................................
    QDate dateDeb(QDate::currentDate());
    QDate dateFin(QDate::currentDate());
    dateDeb = dateDeb.addDays (-1);
    //dateFin   = dateFin.addDays (1);
    mle_deb->setText(dateDeb.toString("ddMMyyyy"));
    mle_fin->setText(dateFin.toString("ddMMyyyy"));
    mle_HeureDeb->setText("0800");
    mle_HeureFin->setText("0800");
    pushButtonDateFin->setIconSet (m_GoDatePopup);
    pushButtonDateDeb->setIconSet (m_GoDatePopup);

    //............................ Placer le spliter dans la bonne position .................................
    QValueList <int> list;
    list.append(60);
    list.append(height()-60);
    //splitter3->setSizes(list);
    list.clear();
    list.append(150);
    list.append(width()-150);
    //splitter3->setSizes(list);
    SetInterfaceOnDroits(G_pCApp->m_Droits, MODE_SAISIE_NON_EN_COURS);
}

//------------------------------------------- getCurrentPageID_ByTitle ------------------------------------------------------------------------
int Wdg_Central::getCurrentPageID_ByTitle(QTabWidget *pQTabWidget, const QString &title)
{int nb = pQTabWidget->count();
 for ( int i=0; i<nb; ++i)
     {if (pQTabWidget->label(i) == title) return i;
     }
 return -1;
}

//--------------------------------- SetInterfaceOnMode -----------------------------------------------------------------------
//    #define MODE_SAISIE_EN_COURS      1
//    #define MODE_SAISIE_NON_EN_COURS  0
void  Wdg_Central::SetInterfaceOnMode(const QString &droits, int mode)
{if (G_pCApp->IsThisDroitExist(droits, "idv"))
    {
    }
}

//--------------------------------- SetInterfaceOnDroits -----------------------------------------------------------------------
//    #define MODE_SAISIE_EN_COURS      1
//    #define MODE_SAISIE NON_EN COURS  0
void  Wdg_Central::SetInterfaceOnDroits(const QString &droits, int mode)
{//-idv-idc-idg
 if (G_pCApp->IsThisDroitExist(droits, "idv"))
    {
    }
 SetInterfaceOnMode(droits,  mode);
}
//----------------------------------------- setButtonOnContexte ----------------------------------------------------------------------
void Wdg_Central::setButtonOnContexte(int mode )
{
}
//----------------------------------------- destroy ----------------------------------------------------------------------
void Wdg_Central::destroy()
{   
}

//----------------------------------------- tabWidget_Central_currentChanged ----------------------------------------------------------------------
void Wdg_Central::tabWidget_Central_currentChanged( QWidget *pageWidget )
{if (pageWidget==0) return;
}

//----------------------------------------- NaturalToSqlDate ------------------------------------------------------------------------
QString Wdg_Central::NaturalToSqlDate(const QString &inDate )    // 05061956
{char    *pt = (char*)(const char*) inDate;
 QString ret = "";
 if (pt)
    { while ( *pt )  {if (*pt>='0'&&*pt<='9')  {ret += *pt;} ++pt; }  // eliminer tout ce qui n'est pas numérique '-' et autre '/'
      ret = ret.mid(4,4) + ret.mid(2,2) + ret.mid(0,2);
    }
 return ret;
}
//----------------------------------------- SqlDateToNatural ------------------------------------------------------------------------
QString Wdg_Central::SqlDateToNatural(const QString &inDate ) // 19560506
{ char    *pt = (char*)(const char*) inDate;
 QString ret = "";
 if (pt)
    { while ( *pt )  {if (*pt>='0'&&*pt<='9')  {ret += *pt;} ++pt; }  // eliminer tout ce qui n'est pas numérique '-' et autre '/'
      ret= ret.mid(6,2) + ret.mid(4,2) + ret.mid(0,4);
    }
 return ret;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// LISTE PATIENT ///////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------- setUser ------------------------------------------------------------------------
void Wdg_Central::setUser( const QString &userName, const QString &signUser)
{m_UserName = userName;
 m_SignUser = signUser;
}

//----------------------------------------------- closeEvent -------------------------------------------------------------
void Wdg_Central::closeEvent( QCloseEvent* ce )
{   //ce->accept();
    ce->ignore();

}

//--------------------------------- ClearDialog -------------------------------------------------------------------------------------------
void Wdg_Central::ClearDialog()
{   //.......................... effacer dossier en cours ...........................
    G_pCApp->m_LastNom     ="";
    G_pCApp->m_LastPrenom  ="";
    G_pCApp->m_LastGUID    ="";
    G_pCApp->m_LastPK      ="";
    G_pCApp->m_LastTel     ="";

}

//--------------------------------- ToNum ---------------------------------------------------------------------------------
QString Wdg_Central::ToNum(QString in)
{for(uint i=0; i<in.length(); )
    {if (in.at(i)<'0' || in.at(i)>'9') in.remove(i,1);
        else ++i;
    }
    return in;
}

//--------------------------------- DoPopupList ----------------------------------------------------------------------
QString Wdg_Central::DoPopupList(QStringList &list)
{   m_CurrentPopupItem = "";
    m_RetPopupItem     = "";
    if (m_pQPopupMenu) return m_RetPopupItem;
    m_pQPopupMenu      = new QPopupMenu(0, "MyPopupMenu" );
    if (m_pQPopupMenu==0)       return m_RetPopupItem;
    //............................... chercher une zone de liste deroulante (bleue) .....................
    for ( QStringList::Iterator it =list.begin(); it != list.end(); ++it )
        { m_pQPopupMenu->insertItem( *(it),    this, SLOT( GetPopupItem()),  CTRL+Key_L );
        }
    //................. connecter le menu .................................................................
    connect( m_pQPopupMenu, SIGNAL( activated( int)),
             this,          SLOT(OnMenuActivated( int))
             );

    m_pQPopupMenu->exec(QCursor::pos());
    delete  m_pQPopupMenu;
    m_pQPopupMenu = 0;
    return  m_RetPopupItem;
}

//--------------------------------- OnMenuActivated  -------------------------------------------------------------
void Wdg_Central::OnMenuActivated (int id)
{if (id != -1 && m_pQPopupMenu)
    {m_CurrentPopupItem = m_pQPopupMenu->text(id);
    }
}

//--------------------------------- GetPopupItem  -------------------------------------------------------------
void Wdg_Central::GetPopupItem ()
{m_RetPopupItem  = m_CurrentPopupItem;
}
//--------------------------------- Slot_ListView_Dossiers_retenus_doubleClicked -----------------------------------------------------------------------------
void Wdg_Central::Slot_ListView_Dossiers_retenus_doubleClicked( QListViewItem *pQListViewItem )
{ 
   if (G_pCApp->IsThisDroitExist(G_pCApp->m_Droits, "idg")==0)
      {QMessageBox::warning ( this, tr("Lancement de dossier :"),
                                     tr( "Vos droits sont insuffisants pour lancer un dossier"),
                                     tr("&Annuler"),0, 0,
                                     1, 1
                                     );
       return;
      }
    if ( pQListViewItem == 0) return;
    AccederDossier(pQListViewItem->text(0),    // Nom assuré
                   pQListViewItem->text(1),    // Prenom assuré
                   pQListViewItem->text(2),    // numero PrimKey
                   pQListViewItem->text(3),    // GUID dossier
                   G_pCApp->m_User,            // login de l'utilisateur en cours
                   G_pCApp->m_SignUser);       // login de l'utilisateur signataire en cours

}
//--------------------------------- AccederDossier -----------------------------------------------------------------------------
void Wdg_Central::AccederDossier( QString nom,  QString prenom, const QString &primKey, const QString &guid, const QString &user, const QString &sign_user)
{  if (G_pCApp->IsThisDroitExist(G_pCApp->m_Droits, "idg")==0)
      {QMessageBox::warning ( this, tr("Lancement de dossier :"),
                                     tr( "Vos droits sont insuffisants pour lancer un dossier"),
                                     tr("&Annuler"),0, 0,
                                     1, 1
                                     );
       return;
      }

    // pDrTux = new DrTux(argv[1],       // utilisateur
    //                    argv[2],       // num_doss (GUID dossier)
    //                    argv[3],       // ID_Doss (fchgnrl2002_nom_prenom WHERE ID_PrimKey = ID_Doss)
    //                    argv[4],       // nom dossier
    //                    argv[5]        // prenom dossier
    //                   );
    QString usedBy;
    if ( m_pCMoteurBase->IsDossierVerrouille( guid, usedBy ))
    {//.............. si l'utilisateur du dossier est le même que celui du poste de travail...................................
        //               alors le dossier est déjà lancé sur le poste de travail local
        //               on le lance et alors il se met en premier plan (car gère comme cela l'instance unique)
        // sinon on demande si l'on veut lancer le dossier présent sur un autre poste avec avertisement des
        // conséquences
        if (m_pCMoteurBase->m_UserName + "@" + m_pCMoteurBase->m_HostName != usedBy &&
            AlertVerrou(usedBy, nom, prenom)!=0)
           {return;
           }
    }
    //................ Lancer le process .................................................................
    QProcess  proc;
    QString pathExe  = CGestIni::Construct_Name_Exe("drtux",QFileInfo (qApp->argv()[0]).dirPath (true));
    proc.addArgument( pathExe );
    proc.addArgument( user);                                        // login de l'utilisateur
    proc.addArgument( sign_user );                                  // login du signataire
    proc.addArgument( guid );                                       // GUID dossier
    proc.addArgument( primKey);                                     // numero PrimKey
    proc.addArgument( nom.replace("'","~@~") );                                        // Nom assuré
    proc.addArgument( prenom.replace("'","~@~") );                                     // Prenom assuré
    /*
    QStringList arg_lst = proc.arguments ();
    for ( QStringList::Iterator it = arg_lst.begin(); it != arg_lst.end(); ++it )
        { qDebug( *it );
        }
    */
    QString str = CGestIni::Construct_Name_File_Ini("Manager",QFileInfo (qApp->argv()[0]).dirPath (true), "");
    proc.addArgument(str );
    // fichier ini
    str = CGestIni::Construct_Name_File_Ini("Manager",QFileInfo (qApp->argv()[0]).dirPath (true), "DataBase.cfg");
    proc.addArgument( str );
    // fichier cfgBase
    if ( !proc.start() )
       {qDebug("ListView_Patient_doubleClicked(): Echec de l'ouverture du process");
       }
   // pushButtonAcceder->setIconSet (m_Cadenas);
}

//--------------------------------------- AlertVerrou -------------------------------------------------------------------------------
int  Wdg_Central::AlertVerrou(const QString &userBy, const QString &dossNom, const QString &dossPrenom)
{   QString value;
    CGestIni::Param_ReadParam(m_LocalParam, "Gestion des dossiers", "Acces Concurrent", &value);   // path editeur de texte non défini
    if (value.lower() == tr("non possible"))
    {  QMessageBox::information( this, tr(  "OUVERTURE CONFLICTUELLE"),
                                 tr(  " <b><u>ATTENTION</b></u> ! Ce dossier <font color=\"#e80d0d\"><b>")          + dossNom     + " " + dossPrenom  +
                                 tr(  "</b></font> est déjà ouvert sur le poste suivant : <b>" )   + userBy  +
                                 tr(  "</b><br> Son accès n'est possible qu'en lecture seulement,<br>"
                                      "il se mettra automatiquement en mode écriture<br>"
                                      "dés qu'il sera disponible sur votre poste") ,
                                 //"&nbsp;&nbsp;&nbsp;&nbsp; ° <b>Quitter pour <u>Annuler</u></b> l'opération en cours" ) ,
                                 tr("Annu&ler"),0 , 0,
                                 1, 1 );
        return 1;
    }
    return QMessageBox::information( this, tr("OUVERTURE CONFLICTUELLE"),
                                     tr ( " <b><u>ATTENTION</b></u> ! Ce dossier <b>")          + dossNom     + " " + dossPrenom  +
                                     tr(  "</b> est déjà ouvert sur le poste suivant : <b>" )   + userBy  +
                                     tr(  "</b><br> Faut-il tenter tout de même l'ouverture ?<br>") ,
                                     //"&nbsp;&nbsp;&nbsp;&nbsp; ° <b>Quitter pour <u>Annuler</u></b> l'opération en cours" ) ,   1111b0
                                     tr("&Ouvrir"), tr("Annu&ler"), 0,
                                     1, 1 );
}

//=======================================================================================================================================
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// ONGLET STATISTIQUES /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//=======================================================================================================================================

//----------------------------------- GetPreselectDateList ----------------------------------
int Wdg_Central::GetPreselectDateList(QStringList & rQStringList)
{QString      preselectStr;
 CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathAppli + "/Images/PreselectDates.txt", preselectStr);
 if (preselectStr.length()==0) return 0;
 preselectStr = preselectStr.stripWhiteSpace();
 preselectStr = preselectStr.remove("\r");
 preselectStr = preselectStr.remove("\n");
 rQStringList = QStringList::split (';', preselectStr);
 return rQStringList.count();
}

//----------------------------------- pushButtonDateDeb_clicked() ----------------------------------
void Wdg_Central::pushButtonDateDeb_clicked()
{   Dlg_Calendar          *dlg  = new Dlg_Calendar(this, "Calendar_Dial", FALSE);
    if (dlg ==0)                                                   return;
    dlg->setCaption(tr("Date de début"));
    dlg->move ( QCursor::pos().x(), QCursor::pos().y() );
    dlg->setMenuPreselectDateOn();
    dlg->Month_setIcon(    QPixmap(G_pCApp->m_PathAppli + "Images/Left_Blue.png") ,  QPixmap(G_pCApp->m_PathAppli + "Images/Right_Blue.png") );
    dlg->Year_setIcon(     QPixmap(G_pCApp->m_PathAppli + "Images/Left_Yelow.png") , QPixmap(G_pCApp->m_PathAppli + "Images/Right_Yelow.png") );
    dlg->DateMenu_setIcon( QPixmap(G_pCApp->m_PathAppli + "Images/DatePreselect.png"));
    QStringList  preselectList;
    if (GetPreselectDateList(preselectList))  dlg->setMenuPreSelectDate(preselectList);
    QString text = mle_deb->text();
    int jour, mois, annee;
    jour  = text.mid(0,2).toInt();
    mois  = text.mid(2,2).toInt();
    annee = text.mid(4,4).toInt();
    if (QDate::isValid ( annee, mois, jour ) )
    {dlg->setDate(QDate(annee, mois, jour));
    }
    if (dlg->exec()== QDialog::Accepted )
    {mle_deb->setText(dlg->getDate().toString("ddMMyyyy"));
     mle_HeureDeb->setText(dlg->timeEdit_Heure->time ().toString("hhmm"));
    }
    delete dlg;
    QString tab_name = tabWidgetTypeEvaluation->tabLabel ( tabWidgetTypeEvaluation->currentPage () );
    textEdit_Sql_IdentSelect->setText(ConstructRequeteIdentSelect());
}


//----------------------------------- pushButtonDateFin_clicked() ----------------------------------
void Wdg_Central::pushButtonDateFin_clicked()
{   Dlg_Calendar          *dlg  = new Dlg_Calendar(this, "Calendar_Dial", FALSE);
    if (dlg ==0)                                                   return;
    dlg->setCaption(tr("Date de fin"));
    dlg->move ( QCursor::pos().x(), QCursor::pos().y() );
    dlg->setMenuPreselectDateOn();
    dlg->Month_setIcon(    QPixmap(G_pCApp->m_PathAppli + "Images/Left_Blue.png") ,  QPixmap(G_pCApp->m_PathAppli + "Images/Right_Blue.png") );
    dlg->Year_setIcon(     QPixmap(G_pCApp->m_PathAppli + "Images/Left_Yelow.png") , QPixmap(G_pCApp->m_PathAppli + "Images/Right_Yelow.png") );
    dlg->DateMenu_setIcon( QPixmap(G_pCApp->m_PathAppli + "Images/DatePreselect.png"));
    QStringList  preselectList;
    if (GetPreselectDateList(preselectList))  dlg->setMenuPreSelectDate(preselectList);

    QString text = mle_fin->text();
    int jour, mois, annee;
    jour  = text.mid(0,2).toInt();
    mois  = text.mid(2,2).toInt();
    annee = text.mid(4,4).toInt();
    if (QDate::isValid ( annee, mois, jour ) )
    {dlg->setDate(QDate(annee, mois, jour));
    }
    if (dlg->exec()== QDialog::Accepted )
    {mle_fin->setText(dlg->getDate().toString("ddMMyyyy"));
     mle_HeureFin->setText(dlg->timeEdit_Heure->time ().toString("hhmm"));
    }
    delete dlg;
    QString tab_name = tabWidgetTypeEvaluation->tabLabel ( tabWidgetTypeEvaluation->currentPage () );
    textEdit_Sql_IdentSelect->setText(ConstructRequeteIdentSelect());

}

//-----------------------------------mle_debut_lostFocus()----------------------------------
void Wdg_Central::mle_debut_lostFocus()
{
    int jour, mois, annee;
    QDate dateDebut;

    if (mle_deb->text().length() == 8) {
        jour  = mle_deb->text().mid(0,2).toInt();
        mois  = mle_deb->text().mid(2,2).toInt();
        annee = mle_deb->text().mid(4,4).toInt();
        if ( dateDebut.setYMD( annee , mois , jour ) != true) {
            QMessageBox mb( tr("Date incohérente"),
                            tr("La date de début n'est pas correcte\n"),
                            QMessageBox::Critical,
                            QMessageBox::Ok | QMessageBox::Default,0,0);
            mb.exec();
            mle_deb->setFocus();
        } else {
            //lanceRecherche();
        }
    } else {
        //lanceRecherche();
    }
}

//-----------------------------------mle_fin_lostFocus()----------------------------------
void Wdg_Central::mle_fin_lostFocus()
{
    int jour, mois, annee;
    QDate dateFin;

    if (mle_fin->text().length() == 8) {
        jour  = mle_fin->text().mid(0,2).toInt();
        mois  = mle_fin->text().mid(2,2).toInt();
        annee = mle_fin->text().mid(4,4).toInt();
        if ( dateFin.setYMD( annee , mois , jour ) != true) {
            QMessageBox mb( tr("Date incohérente"),
                            tr("La date de fin n'est pas correcte\n"),
                            QMessageBox::Critical,
                            QMessageBox::Ok | QMessageBox::Default,0,0);
            mb.exec();
            mle_fin->setFocus();
        } else {
            //lanceRecherche();
        }
    } else {
        //lanceRecherche();
    }
}
//----------------------------------------- pushButtonRunStat_clicked -----------------------------------------------------
void Wdg_Central::pushButtonRunStat_clicked()
{  EPP_Do();
}

//----------------------------------------- ListView_PatientRejetes_doubleClicked -----------------------------------------------------
void Wdg_Central::ListView_PatientRejetes_doubleClicked( QListViewItem *pQListViewItem )
{if ( pQListViewItem == 0) return;
    QString guid = pQListViewItem->text(5);
    AccederDossier(pQListViewItem->text(0),                           // Nom assuré
                   pQListViewItem->text(1),                           // Prenom assuré
                   m_pCMoteurBase->GetDossPrimKeyFromGUID(guid ),     // numero PrimKey
                   guid ,                                             // GUID dossier
                   pQListViewItem->text(3),                           // Nom de l'utilisateur
                   G_pCApp->m_SignUser);                              // Nom de l'utilisateur signataire
}

//----------------------------------------- ListView_PatientRejetes_returnPressed -----------------------------------------------------
void Wdg_Central::ListView_PatientRejetes_returnPressed( QListViewItem *pQListViewItem )
{ListView_PatientRejetes_doubleClicked( pQListViewItem );
}


//----------------------------------------- ListView_PatientRejetes_spacePressed -----------------------------------------------------
void Wdg_Central::ListView_PatientRejetes_spacePressed( QListViewItem *pQListViewItem )
{ListView_PatientRejetes_doubleClicked( pQListViewItem );
}

//----------------------------------------- checkBoxWarning_stateChanged -----------------------------------------------------
void Wdg_Central::checkBoxWarning_stateChanged( int )
{    return;
}

////////////////////////////////////////////////////// Définition du SQL-filtre //////////////////////////////////////////////////////
//----------------------------------------- pushButton_MaskListSqlField_clicked ---------------------------------------------------------
void Wdg_Central::pushButton_MaskListSqlField_clicked()
{if (pushButton_AddListIdentField->isVisible())
    {pushButton_AddListIdentField->hide();
     pushButton_DelListIdentField->hide();
     pushButton_DelAllListIdentField->hide();
     comboBox_RequetesLib->hide();
     listView_Sql_IdentFields->hide();
     pushButton_EPP_Charger->hide();
     pushButton_EPP_Enregistrer->hide();
     textEdit_Sql_IdentSelect->hide();
     pushButton_MaskListSqlField->setPixmap ( QPixmap(G_pCApp->m_PathAppli + "Images/LeftLitle.png"  ));
    }
 else
    {pushButton_AddListIdentField->show();
     pushButton_DelListIdentField->show();
     pushButton_DelAllListIdentField->show();
     comboBox_RequetesLib->show();
     listView_Sql_IdentFields->show();
     pushButton_EPP_Charger->show();
     pushButton_EPP_Enregistrer->show();
     textEdit_Sql_IdentSelect->show();
     pushButton_MaskListSqlField->setPixmap ( QPixmap(G_pCApp->m_PathAppli + "Images/DownLitle.png"  ));
    }

}
//----------------------------------------- listView_Sql_IdentFields_rightButtonPressed ---------------------------------------------------------
void Wdg_Central::listView_Sql_IdentFields_rightButtonPressed( QListViewItem *, const QPoint &, int )
{

}

//----------------------------------------- listView_Sql_IdentFields_contextMenuRequested ---------------------------------------------------------
void Wdg_Central::listView_Sql_IdentFields_contextMenuRequested( QListViewItem *pQListViewItem, const QPoint &, int pos)
{SetListeChamp(listView_Sql_IdentFields, pQListViewItem, pos);
 textEdit_Sql_IdentSelect->setText(ConstructRequeteIdentSelect());
}


void Wdg_Central::listView_Sql_IdentFields_doubleClicked( QListViewItem *, const QPoint &, int )
{

}
//----------------------------------------- listView_Sql_IdentFields_mouseButtonClicked ---------------------------------------------------------
void Wdg_Central::listView_Sql_IdentFields_mouseButtonClicked( int, QListViewItem *pQListViewItem, const QPoint &pts, int pos)
{

}

//------------------------------------ GetFieldType --------------------------------------------
// retourne le type possible du champ passe en entree
QVariant::Type Wdg_Central::GetFieldType( const QString &fieldName)
{QVariant::Type ret = QVariant::Invalid;
 ret = TestFieldsType(fieldName, m_pCMoteurBase->m_DOSS_INDEX_TBL_NAME);
 if (ret != QVariant::Invalid) return ret;
 ret = TestFieldsType(fieldName, m_pCMoteurBase->m_DOSS_IDENT_TBL_NAME);
 if (ret != QVariant::Invalid) return ret;
 ret = TestFieldsType(fieldName, m_pCMoteurBase->m_DOSS_RUB_HEAD_TBL_NAME);
 if (ret != QVariant::Invalid) return ret;
 ret = TestFieldsType(fieldName, m_pCMoteurBase->m_DOSS_RUB_DATA_TBL_NAME);
 if (ret != QVariant::Invalid) return ret;
 return ret;
}

//------------------------------------ TestFieldsType --------------------------------------------
QVariant::Type Wdg_Central::TestFieldsType(const QString &fieldName, const QString &tableName)
{
 QVariant::Type ret = QVariant::Invalid;
 int               i = 0;
 int       nb_fields = 0;
 if (m_pCMoteurBase->m_DataBase->isOpen()==FALSE && m_pCMoteurBase->m_DataBase->open()==FALSE) return ret;
 QSqlCursor *pQSqlCursor = new QSqlCursor(tableName, TRUE, m_pCMoteurBase->m_DataBase );
 if (pQSqlCursor)
    {nb_fields = pQSqlCursor->count();
     for (i=0; i<nb_fields; ++i) if (pQSqlCursor->fieldName ( i ) == fieldName) ret = pQSqlCursor->field(i)->type();
     delete pQSqlCursor;
    }
 m_pCMoteurBase->m_DataBase->close();
 return ret;
}

//------------------------------------ initFieldsList --------------------------------------------
void Wdg_Central::initFieldsList()
{m_FieldsList_IndexList.clear();
 GetFieldsList(m_FieldsList_IndexList,     m_pCMoteurBase->m_DOSS_INDEX_TBL_NAME);

 m_FieldsList_FicheIdent.clear();
 GetFieldsList(m_FieldsList_FicheIdent,    m_pCMoteurBase->m_DOSS_IDENT_TBL_NAME);

 m_FieldsList_DossHeader.clear();
 GetFieldsList(m_FieldsList_DossHeader,    m_pCMoteurBase->m_DOSS_RUB_HEAD_TBL_NAME);

 m_FieldsList_DossData.clear();
 GetFieldsList(m_FieldsList_DossData,      m_pCMoteurBase->m_DOSS_RUB_DATA_TBL_NAME);

 //GetFieldsList(m_FieldsList_Note,          const QString &tableName);
 //GetFieldsList(m_FieldsList_Intervenants,  const QString &tableName);
}

//------------------------------------ GetFieldsList --------------------------------------------
void Wdg_Central::GetFieldsList(QStringList &fieldList, const QString &tableName)
{if (m_pCMoteurBase->m_DataBase->isOpen()==FALSE && m_pCMoteurBase->m_DataBase->open()==FALSE)  return ;
 int         i = 0;
 int nb_fields = 0;
 QSqlCursor *pQSqlCursor = new QSqlCursor(tableName, TRUE, m_pCMoteurBase->m_DataBase );
 if (pQSqlCursor)
    {nb_fields = pQSqlCursor->count();
     for (i=0; i<nb_fields; ++i)  fieldList.append (pQSqlCursor->fieldName ( i ));
     delete pQSqlCursor;
    }
 m_pCMoteurBase->m_DataBase->close();
}

//------------------------------------ OnOutFocusDateHeure --------------------------------------------
void Wdg_Central::OnOutFocusDateHeure()
{   QString tab_name = tabWidgetTypeEvaluation->tabLabel ( tabWidgetTypeEvaluation->currentPage () );
    ConstructRequeteIdentSelect();
}
//------------------------------------ ConstructRequeteIdentSelect --------------------------------------------
QString Wdg_Central::ConstructRequeteIdentSelect()
{   QDate dateDeb  (QDate::currentDate());
    QDate dateFin  (QDate::currentDate());

    int jour, mois, annee;
    QString text = mle_deb->text();
    jour  = text.mid(0,2).toInt();
    mois  = text.mid(2,2).toInt();
    annee = text.mid(4,4).toInt();
    if (QDate::isValid ( annee, mois, jour ) ) dateDeb.setYMD (annee , mois  , jour);
    mle_deb->setText(dateDeb.toString("ddMMyyyy"));
    text  = mle_fin->text();
    jour  = text.mid(0,2).toInt();
    mois  = text.mid(2,2).toInt();
    annee = text.mid(4,4).toInt();
    if (QDate::isValid ( annee, mois, jour ) ) dateFin.setYMD (annee , mois  , jour);
    mle_fin->setText(dateFin.toString("ddMMyyyy"));

    //........................ la requete .....................................................................
    // les dates de la requete sont volontairement placées un jour avant et apres la fourchette de selection
    // car il peut arriver que certains dossiers soient créés après la garde et donc les heures strictement concernées
    // par la garde peuvent ne plus s'appliquer: exemple je vois le patient à 7h45 et apres la fin de garde (8h00)
    // par exemple à 9h je crée le dossier. Donc le test entre 8h la veille et 8h en fin de garde ne fonctionnerai
    // pas.
    //QDate reqDateDeb = dateDeb.addDays(-1); QString reqDateDebStr = reqDateDeb.toString("yyyyMMdd");
    //QDate reqDateFin = dateFin.addDays(+1); QString reqDateFinStr = reqDateFin.toString("yyyyMMdd");

    QString reqDateDebStr = dateDeb.toString("yyyyMMdd");
    QString reqDateFinStr = dateFin.toString("yyyyMMdd");

    QString requete  = "SELECT * FROM ";
            requete += m_pCMoteurBase->m_DOSS_RUB_HEAD_TBL_NAME + " INNER JOIN " +
                       m_pCMoteurBase->m_DOSS_INDEX_TBL_NAME    + " ON "         +
                       m_pCMoteurBase->m_DOSS_RUB_HEAD_TBL_NAME + "." +m_pCMoteurBase->m_DOSS_RUB_HEAD_GUID          + "=" + m_pCMoteurBase->m_DOSS_INDEX_TBL_NAME    +"."+m_pCMoteurBase->m_DOSS_INDEX_GUID          + " INNER JOIN " +
                       m_pCMoteurBase->m_DOSS_IDENT_TBL_NAME    + " ON "         +
                       m_pCMoteurBase->m_DOSS_RUB_HEAD_TBL_NAME + "." +m_pCMoteurBase->m_DOSS_RUB_HEAD_GUID          + "=" + m_pCMoteurBase->m_DOSS_IDENT_TBL_NAME    +"."+m_pCMoteurBase->m_DOSS_IDENT_GUID          + " INNER JOIN " +
                       m_pCMoteurBase->m_DOSS_RUB_DATA_TBL_NAME + " ON "         +
                       m_pCMoteurBase->m_DOSS_RUB_HEAD_TBL_NAME + "." +m_pCMoteurBase->m_DOSS_RUB_HEAD_REF_BLOBS_PK  + "=" + m_pCMoteurBase->m_DOSS_RUB_DATA_TBL_NAME +"."+m_pCMoteurBase->m_DOSS_RUB_DATA_PRIMKEY    + " WHERE "      +
                       m_pCMoteurBase->m_DOSS_RUB_HEAD_DATE     + " >= '" + reqDateDebStr  + mle_HeureDeb->text() +"00' AND " + m_pCMoteurBase->m_DOSS_RUB_HEAD_DATE + "<= '" + reqDateFinStr + mle_HeureFin->text()+"00'";
            requete += listView_Sql_IdentFieldsToSQL();
 return requete;
}
//"SELECT * FROM RubriquesHead INNER JOIN IndexNomPrenom ON RubriquesHead.RbDate_IDDos=IndexNomPrenom.FchGnrl_IDDos INNER JOIN
// RubriquesBlobs ON RubriquesHead.RbDate_RefBlobs_PrimKey=RubriquesBlobs.RbDate_PrimKey
// WHERE RbDate_Date >= '$debut' AND RbDate_Date <= '$fin' AND RbDate_TypeRub='20030000' AND RbDate_CreateUser='$user' AND CAST(RubriquesBlobs.RbDate_DataRub AS CHAR) LIKE '%s' ORDER BY RbDate_Date"

//----------------------------------- listView_Sql_IdentFieldsToSQL ------------------------------------------------------------------------
// popItem.append(tr("  ) "));
// popItem.append(tr("  ( "));
// popItem.append(tr("  ) et ("));
// popItem.append(tr("  ) ou ("));

QString Wdg_Central::listView_Sql_IdentFieldsToSQL()
{QString                      result = "";
 QString                      str    = "";
 QListViewItem *pQListViewItemBefore = 0;
 if (listView_Sql_IdentFields->childCount() <= 0) return result +  " ORDER BY " + m_pCMoteurBase->m_DOSS_RUB_HEAD_TBL_NAME + "." +m_pCMoteurBase->m_DOSS_RUB_HEAD_GUID + " ;";
 result = " AND ";
 QListViewItem *last_pQListViewItem =  listView_Sql_IdentFields->lastItem();
 QListViewItemIterator it( listView_Sql_IdentFields );
 while ( it.current() )
    { QListViewItem *pQListViewItem = it.current();
      //........ traiter premier séparateur .............
      str = pQListViewItem->text(0);
      // #define DELIMITER_TYPE_LIST  "  Vide #  ) #  ( #  ) et (#  ) ou ("
      switch(CMetaFiltre::GetDelimiterList_ID(str))
         {case 1:        //     )
               result += tr(")");
               return result +  " ORDER BY " + m_pCMoteurBase->m_DOSS_RUB_HEAD_TBL_NAME + "." +m_pCMoteurBase->m_DOSS_RUB_HEAD_GUID + " ;";
               //break;
          case 2:        //     (
               result += tr("(`")       + pQListViewItem->text(1) + "`" + interpreteOperateurComparaison(pQListViewItem->text(2), valueInterprete(pQListViewItem->text(1), pQListViewItem->text(3)))+interpreteOperateurEtOu(pQListViewItem->text(4));
               break;
          case 3:        //     ) et (
               result += tr(") AND (`") + pQListViewItem->text(1) + "`" + interpreteOperateurComparaison(pQListViewItem->text(2), valueInterprete(pQListViewItem->text(1), pQListViewItem->text(3)))+interpreteOperateurEtOu(pQListViewItem->text(4));
               break;
          case 4:       //      ) ou (
               result += tr(") OR (`")  + pQListViewItem->text(1) + "`" + interpreteOperateurComparaison(pQListViewItem->text(2), valueInterprete(pQListViewItem->text(1), pQListViewItem->text(3)))+interpreteOperateurEtOu(pQListViewItem->text(4));
               break;
          default:
               if (pQListViewItemBefore && (pQListViewItemBefore->text(4).contains(tr("et"),FALSE) != -1 || pQListViewItemBefore->text(4).contains(tr("ou"),FALSE) != -1))
                  {result += "`"        + pQListViewItem->text(1) + "`" + interpreteOperateurComparaison(pQListViewItem->text(2), valueInterprete(pQListViewItem->text(1), pQListViewItem->text(3)))+interpreteOperateurEtOu(pQListViewItem->text(4));
                  }
               break;
        }
      pQListViewItemBefore = pQListViewItem;
      ++it;
    }
 return result;
}

//----------------------------------- valueInterprete ------------------------------------------------------------------------
QString Wdg_Central::valueInterprete(const QString &fieldName, const QString &value)
{if (fieldName==m_pCMoteurBase->m_DOSS_RUB_HEAD_TYPE) return CMetaField::RubNameToRubType(value);
 else                                                 return value;
}

//----------------------------------- GetItemBefore ------------------------------------------------------------------------
QListViewItem * Wdg_Central::GetItemBefore(QListView *pQListView, QListViewItem *pQListViewItemToCheck)
{QListViewItem *pQListViewItemBefore = 0;
 QListViewItemIterator it( pQListView );
 while ( it.current() )
    { QListViewItem *pQListViewItem = it.current();
      if (pQListViewItem == pQListViewItemToCheck) return pQListViewItemBefore;
      pQListViewItemBefore = pQListViewItem;
      ++it;
    }
 return pQListViewItemBefore;
}
//----------------------------------- interpreteOperateurComparaison ------------------------------------------------------------------------
//           popItem.append(tr("  >               "));
//           popItem.append(tr("  <               "));
//           popItem.append(tr("  <=              "));
//           popItem.append(tr("  >=              "));
//           popItem.append(tr(" est égal à       "));
//           popItem.append(tr(" contient         "));
//           popItem.append(tr(" est différent de "));
//           popItem.append(tr(" ne contient pas  "));
//           #define OPERATEUR_LIST "Vide#>#<#<=#">=#est égal#contient#commence par#finit par#est différent de#ne contient pas"
QString Wdg_Central::interpreteOperateurComparaison(const QString &op, const QString &val)
{
      QString result = "";
      switch(CMetaField::GetListOperateur_ID(op))
         { case 0:           // Vide
                break;
           case 1:           // >
                result += tr(" > '") + val + "' ";
                break;
           case 2:           // <
                result += tr(" < '") + val + "' ";
                break;
           case 3:           // <=
                result += tr(" <= '") + val + "' ";
                break;
           case 4:           // >=
                result += tr(" >= '") + val + "' ";
                break;
           case 5:           // est égal
                result += tr(" = '") + val + "' ";
                break;
           case 6:           // contient
                result += tr(" LIKE '%") + val + "%' ";
                break;
           case 7:           // commence par
                result += tr(" LIKE '%") + val + "' ";
                break;
           case 8:           // finit par
                result += tr(" LIKE '") + val + "%' ";
                break;
           case 9:           // est différent de
                result += tr(" <> '") + val + "' ";
                break;
           case 10:          // ne contient pas
                result += tr(" NOT LIKE '%") + val + "%' ";
                break;
         }
 return result;
}

//----------------------------------- interpreteOperateurEtOu ------------------------------------------------------------------------
// popItem.append(tr("  ) "));
// popItem.append(tr("  ( "));
// popItem.append(tr("  ) et ("));
// popItem.append(tr("  ) ou ("));
// #define LOGIC_OP_LIST        "  Vide #  et               #  ou               #  sans             "
QString Wdg_Central::interpreteOperateurEtOu(const QString &op)
{     int id = CMetaFiltreLine::GetListLogicOper_ID(op);
      QString result = "";
      switch(id)
         {case 1: result += tr(" AND "); break;
          case 2: result += tr(" OR  "); break;
         }
      return result;
}

//-------------------------------------------- pushButton_AddListIdentField_clicked -------------------------------------------------------
void Wdg_Central::pushButton_AddListIdentField_clicked()
{pushButton_AddListField_clicked(listView_Sql_IdentFields);
 textEdit_Sql_IdentSelect->setText(ConstructRequeteIdentSelect());
}

//----------------------------------------- pushButton_DelAllListIdentField_clicked ---------------------------------------------------------
void Wdg_Central::pushButton_DelAllListIdentField_clicked()
{listView_Sql_IdentFields->clear();
 textEdit_Sql_IdentSelect->setText(ConstructRequeteIdentSelect());
}

////////////////////////////////////////////////////// Définition du méta-filtre //////////////////////////////////////////////////////
//----------------------------------------- pushButton_MaskListMetaField_clicked ---------------------------------------------------------
void Wdg_Central::pushButton_MaskListMetaField_clicked()
{if (pushButton_AddListMetaField->isVisible())
    {pushButton_AddListMetaField->hide();
     pushButton_DelListMetaField->hide();
     pushButton_DelAllListMetaField->hide();
     listView_Sql_MetaFields->hide();
     pushButton_AddDefMetaField->hide();
     pushButton_DelDefMetaField->hide();
     pushButton_DelAllDefMetaField->hide();
     pushButton_UpDefMetaField->hide();
     pushButton_DownDefMetaField->hide();
     listView_Def_MetaFields->hide();
     pushButton_MaskListMetaField->setPixmap ( QPixmap(G_pCApp->m_PathAppli + "Images/LeftLitle.png"  ));
    }
 else
    {pushButton_AddListMetaField->show();
     pushButton_DelListMetaField->show();
     pushButton_DelAllListMetaField->show();
     listView_Sql_MetaFields->show();
     pushButton_AddDefMetaField->show();
     pushButton_DelDefMetaField->show();
     pushButton_DelAllDefMetaField->show();
     pushButton_UpDefMetaField->show();
     pushButton_DownDefMetaField->show();
     listView_Def_MetaFields->show();
     pushButton_MaskListMetaField->setPixmap ( QPixmap(G_pCApp->m_PathAppli + "Images/DownLitle.png"  ));
    }
}
//----------------------------------------- listView_Sql_MetaFields_contextMenuRequested ---------------------------------------------------------
void Wdg_Central::listView_Sql_MetaFields_contextMenuRequested( QListViewItem *pQListViewItem, const QPoint &, int pos )
{SetListeChamp(listView_Sql_MetaFields, pQListViewItem, pos);
}



void Wdg_Central::listView_Sql_MetaFields_rightButtonPressed( QListViewItem *, const QPoint &, int )
{

}
//----------------------------------------- listView_Sql_MetaFields_mouseButtonClicked ---------------------------------------------------------
void Wdg_Central::listView_Sql_MetaFields_mouseButtonClicked( int, QListViewItem *pQListViewItem, const QPoint &pts, int pos )
{
}

//----------------------------------------- GetMetaFieldsList ---------------------------------------------------------
void Wdg_Central::GetMetaFieldsList(QStringList &list)
{
 QListViewItemIterator it( listView_Def_MetaFields );
 while ( it.current() )
    { QListViewItem *pQListViewItem = it.current();
     list.append( pQListViewItem->text(0) );
      ++it;
    }
}

//----------------------------------------- pushButton_AddListMetaField_clicked ---------------------------------------------------------
void Wdg_Central::pushButton_AddListMetaField_clicked()
{pushButton_AddListField_clicked(listView_Sql_MetaFields);
}

//----------------------------------------- pushButton_DelListMetaField_clicked ---------------------------------------------------------
void Wdg_Central::pushButton_DelListMetaField_clicked()
{QListViewItem *pQListViewItem = listView_Sql_MetaFields->currentItem ();
 if (pQListViewItem == 0) return;
 delete pQListViewItem ;
}

//----------------------------------------- pushButton_DelAllListMetaField_clicked ---------------------------------------------------------
void Wdg_Central::pushButton_DelAllListMetaField_clicked()
{listView_Sql_MetaFields->clear();
}

////////////////////////////////////////////////////// Définition des méta-champs //////////////////////////////////////////////////////////////
//----------------------------------------- listView_Def_MetaFields_rightButtonPressed ---------------------------------------------------------
void Wdg_Central::listView_Def_MetaFields_rightButtonPressed( QListViewItem *, const QPoint &, int )
{

}
//----------------------------------------- listView_Def_MetaFields_contextMenuRequested ---------------------------------------------------------
void Wdg_Central::listView_Def_MetaFields_contextMenuRequested( QListViewItem *pQListViewItem, const QPoint &, int pos )
{QStringList popItem;
 QString     popRet;
 int created = CDlg_MetaFieldDefine::modify;
 //.............................. notons que si il y a création d'un meta-champ .............................
 //                               avec  pQListViewItem(1) et pQListViewItem(2) vide
 //                               c'est qu'il est nouvellement cree
 if (pQListViewItem<=0)
    {pQListViewItem = AddDefMetaField();
     created        = CDlg_MetaFieldDefine::create;
    }

QString rubrique =  pQListViewItem->text(1);
CDlg_MetaFieldDefine *dlg = new CDlg_MetaFieldDefine(listView_Def_MetaFields, pQListViewItem ,  this, "CDlg_MetaFieldDefine");
if (dlg==0)            return;
if (dlg->exec()== QDialog::Accepted )
   {
   }
else
   {if (created==CDlg_MetaFieldDefine::create) delete pQListViewItem;
   }
listView_Def_MetaFields->setSorting ( 1 );
listView_Def_MetaFields->setSortColumn (1) ;
listView_Def_MetaFields->sort();
listView_Def_MetaFields->setSorting ( -1 );

}

//----------------------------------------- listView_Def_MetaFields_mouseButtonClicked ---------------------------------------------------------
void Wdg_Central::listView_Def_MetaFields_mouseButtonClicked( int, QListViewItem *, const QPoint &pts, int pos )
{
}

//----------------------------------------- pushButton_AddDefMetaField_clicked ---------------------------------------------------------
void Wdg_Central::pushButton_AddDefMetaField_clicked()
{QPoint pt;
 listView_Def_MetaFields_contextMenuRequested( 0, pt, 0 );
}

//----------------------------------------------------- AddDefMetaField ---------------------------------------------------------
QListViewItem * Wdg_Central::AddDefMetaField()
{QListViewItem *last_pQListViewItem =  listView_Def_MetaFields->lastItem();
 QListViewItem *pQListViewItem      = 0;
 QString texte;
 if (last_pQListViewItem)
    {texte           = last_pQListViewItem->text(0);
     texte           = incrementeStr(texte);
    }
 else
    {texte          = tr("Méta_champ_1 ");
    }
 if (last_pQListViewItem)
    {pQListViewItem = new QListViewItem(listView_Def_MetaFields, last_pQListViewItem, texte);
    }
 else
    {pQListViewItem = new QListViewItem(listView_Def_MetaFields, texte);
    }
 if (pQListViewItem) {listView_Def_MetaFields->setCurrentItem (pQListViewItem );
                      listView_Def_MetaFields->setSelected (pQListViewItem , TRUE);
                     }
 return pQListViewItem;
}

//----------------------------------------- incrementeStr ---------------------------------------------------------
QString Wdg_Central::incrementeStr(QString texte)
{QRegExp rx( "[0-9]" );
 int len = 0;
 int pos = rx.match ( texte, 0, &len );
 if (pos == -1) return texte;
 QString str  = texte.mid(pos,len);
 texte = texte.left(pos) + QString::number( str.toInt()+1);
 return texte;
}

//----------------------------------------- pushButton_DelDefMetaField_clicked ---------------------------------------------------------
void Wdg_Central::pushButton_DelDefMetaField_clicked()
{QListViewItem *pQListViewItem = listView_Def_MetaFields->currentItem ();
 if (pQListViewItem == 0) return;
 delete pQListViewItem ;
}


//----------------------------------------- pushButton_DelAllDefMetaField_clicked ---------------------------------------------------------
void Wdg_Central::pushButton_DelAllDefMetaField_clicked()
{listView_Def_MetaFields->clear();
}


//----------------------------------------- pushButton_UpDefMetaField_clicked ---------------------------------------------------------
void Wdg_Central::pushButton_UpDefMetaField_clicked()
{

}

//----------------------------------------- pushButton_DownDefMetaField_clicked ---------------------------------------------------------
void Wdg_Central::pushButton_DownDefMetaField_clicked()
{

}

////////////////////////////////////////////////////// Définition des Classes de comptage //////////////////////////////////////////////////////

//----------------------------------------- pushButton_MaskListMetaField_clicked ---------------------------------------------------------
void Wdg_Central::pushButton_MaskListClassDenombre_clicked()
{if (pushButton_AddClassField->isVisible())
    {pushButton_AddClassField->hide();
     pushButton_DelClassField->hide();
     pushButton_DelAllClassField->hide();
     listView_ClassDenombreList->hide();
     textBrowserInfoPerms->hide();
     textEdit_SelectDossiers->hide();
     G_pCApp->m_DenombreEnabled = false;
     pushButton_MaskListClassDenombre->setPixmap ( QPixmap(G_pCApp->m_PathAppli + "Images/LeftLitle.png"  ));
    }
 else
    {pushButton_AddClassField->show();
     pushButton_DelClassField->show();
     pushButton_DelAllClassField->show();
     listView_ClassDenombreList->show();
     textBrowserInfoPerms->show();
     textEdit_SelectDossiers->show();
     G_pCApp->m_DenombreEnabled   = true;
     pushButton_MaskListClassDenombre->setPixmap ( QPixmap(G_pCApp->m_PathAppli + "Images/DownLitle.png"  ));
    }

}
//----------------------------------------- listView_ClassDenombreList_rightButtonPressed ---------------------------------------------------------
void Wdg_Central::listView_ClassDenombreList_rightButtonPressed( QListViewItem *, const QPoint &, int )
{
}

//----------------------------------------- listView_ClassDenombreList_contextMenuRequested ---------------------------------------------------------
void Wdg_Central::listView_ClassDenombreList_contextMenuRequested( QListViewItem *pQListViewItem, const QPoint &, int )
{//QListViewItem *pQListViewItem      =  listView_ClassDenombreList->currentItem ();
 QListViewItem *last_pQListViewItem =  listView_ClassDenombreList->lastItem();

 QString                  nom   = "";
 QString                  champ = "";
 bool                      ok   = TRUE;
 if (pQListViewItem )    {nom   = pQListViewItem->text(0);
                          champ = pQListViewItem->text(1);
                         }
 CDenombre          *pCDenombre = CDenombreGetByName(nom);       // reperer la classe initiale qui va être modifiée (selon son nom)
 CDlg_NewClassDenombre* dlg = new CDlg_NewClassDenombre(&nom, &champ, listView_Def_MetaFields ,this, "CDlg_NewClassDenombre");
 if (dlg==0)            return;
 if (dlg->exec()== QDialog::Accepted && nom.length() && champ.length())
    { //.................................... si modification d'une classe déjà existante .......................
      CDenombre          *pCDenombreMod = CDenombreGetByName(nom);   // verifier si le nouveau nom n'exite pas déjà
      if (pQListViewItem && pCDenombre)
         {if (pCDenombreMod && pCDenombre != pCDenombreMod)               // on en a trouvé une avec le meme nom et c'est pas la meme alors ne pas toucher
             {QMessageBox::critical (0, QObject::tr("Mofifier une classe de dénombrement"),
                               QObject::tr("Cette classe de dénombrement existe déjà\r\nVeuillez recommencer avec d'autres valeurs"),
                               QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
              ok = FALSE;
             }
          if (ok)
             {pCDenombre->SetName( nom ) ;
              pCDenombre->SetFieldName( champ );
              pQListViewItem->setText(0,nom);
              pQListViewItem->setText(1,champ);
             }
         }
     else  //.................................... si création d'une classe .......................
         {if (pCDenombreMod)               // on en a trouvé une avec le meme nom et c'est pas la meme alors ne pas toucher
             {QMessageBox::critical (0, QObject::tr("Créer une classe de dénombrement"),
                               QObject::tr("Cette classe de dénombrement existe déjà\r\nVeuillez recommencer avec d'autres valeurs"),
                               QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
             }
          else
             { if (last_pQListViewItem)
                  {pQListViewItem = new QListViewItem(listView_ClassDenombreList, last_pQListViewItem, nom, champ);
                  }
               else
                  {pQListViewItem = new QListViewItem(listView_ClassDenombreList, nom, champ);
                  }
               if (pQListViewItem) {listView_ClassDenombreList->setCurrentItem (pQListViewItem );
                                    listView_ClassDenombreList->setSelected (pQListViewItem , TRUE);
                                    m_CDenombreList.append(new CDenombre(nom, champ));
                                   }
             }
         }
    }
 delete dlg;

}

//----------------------------------------- listView_ClassDenombreList_mouseButtonClicked ---------------------------------------------------------
void Wdg_Central::listView_ClassDenombreList_mouseButtonClicked( int, QListViewItem * , const QPoint &, int )
{
}
//----------------------------------------- pushButton_AddClassField_clicked ---------------------------------------------------------
void Wdg_Central::pushButton_AddClassField_clicked()
{QPoint pt;
 listView_ClassDenombreList_contextMenuRequested(0, pt, 0);
}
//----------------------------------------- pushButton_DelClassField_clicked ---------------------------------------------------------
void Wdg_Central::pushButton_DelClassField_clicked()
{QListViewItem *pQListViewItem  =  listView_ClassDenombreList->currentItem ();
 if (pQListViewItem==0) return;
 CDenombre          *pCDenombre = CDenombreGetByName(pQListViewItem->text(0));
 if (pCDenombre == 0)
    {QMessageBox::critical (0, QObject::tr("Effacer une classe de dénombrement"),
                               QObject::tr("Cette classe de dénombrement n'existe pas\r\n désynchronisation de l'affichage et des données"),
                               QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
    }
 else
    {m_CDenombreList.removeRef ( pCDenombre );
    }
 delete pQListViewItem;      // synchro ou pas il faut la virer
}

//----------------------------------------- pushButton_DelAllClassField ---------------------------------------------------------
void Wdg_Central::pushButton_DelAllClassField_clicked()
{m_CDenombreList.clear();
 listView_ClassDenombreList->clear();
}

//----------------------------------------- listView_ClassDenombreList_selectionChanged ---------------------------------------------------------
void Wdg_Central::listView_ClassDenombreList_selectionChanged( QListViewItem * )
{
}


//----------------------------------------- listView_ClassDenombreList_currentChanged ---------------------------------------------------------
void Wdg_Central::listView_ClassDenombreList_currentChanged( QListViewItem *pQListViewItem )
{if (pQListViewItem==0) return;
 // ClassDenombreListToListViewTest( listView_DenombreTest, CDenombreGetByName(pQListViewItem->text(0)) );
}

//----------------------------------------- GetCurrentClassDenombre ---------------------------------------------------------
CDenombre * Wdg_Central::GetCurrentClassDenombre()
{QListViewItem *pQListViewItem  =  listView_ClassDenombreList->currentItem ();
 if (pQListViewItem==0) return 0;
 return  CDenombreGetByName(pQListViewItem->text(0));
}

////////////////////////////////////////////////////// Définition des tests des Classes de comptage //////////////////////////////////////////////////////
void Wdg_Central::listView_DenombreTest_rightButtonPressed( QListViewItem *, const QPoint &, int )
{

}


void Wdg_Central::listView_DenombreTest_rightButtonClicked( QListViewItem *, const QPoint &, int )
{

}


//----------------------------------------- listView_DenombreTest_contextMenuRequested ---------------------------------------------------------
void Wdg_Central::listView_DenombreTest_contextMenuRequested( QListViewItem *pQListViewItem, const QPoint &, int pos)
{/*
 QStringList popItem;
 QString     popRet;
 // QListViewItem *last_pQListViewItem =  listView_DenombreTest->lastItem();
 CDenombre*              pCDenombre = GetCurrentClassDenombre();
 if (pCDenombre==0)     return;

 if (pQListViewItem<=0)
    {if (last_pQListViewItem ) pQListViewItem = new QListViewItem(listView_DenombreTest, last_pQListViewItem, "");
                               pQListViewItem = new QListViewItem(listView_DenombreTest, "");
     pos = 0;
    }
 switch(pos)
   {
    case 0:          // test validant
         {
          // popItem.append(tr("  >               "));
          // popItem.append(tr("  <               "));
          // popItem.append(tr("  <=              "));
          // popItem.append(tr("  >=              "));
          // popItem.append(tr(" est égal à       "));
          // popItem.append(tr(" contient         "));
          // popItem.append(tr(" commence par     "));
          // popItem.append(tr(" finit par        "));
          // popItem.append(tr(" est différent de "));
          // popItem.append(tr(" ne contient pas  "));

          popItem           = CDenombreValidTest::GetPossiblesTestList();
          QString popRet    = DoPopupList(popItem);
          if (popRet.length())    pQListViewItem->setText(pos, popRet);
         }
         break;
    case 1:          // valeur
         {QString            texte =  pQListViewItem->text(pos);
          if (CGenTools::DialogGetString(this, tr("Saisir la valeur qui sera évaluée par l'opérateur : ") + pQListViewItem->text(0), texte,QCursor::pos().x(), QCursor::pos().y()) == QDialog::Accepted)
             { pQListViewItem->setText(pos, texte );
             }
         }
         break;
    case 2:          // expr regulière
         {QString            texte =  pQListViewItem->text(pos);
          if (CGenTools::DialogGetString(this, tr("Saisir une expression régulière (facultative) ") , texte,QCursor::pos().x(), QCursor::pos().y()) == QDialog::Accepted)
             { pQListViewItem->setText(pos, texte );
             }
         }
      break;
    case 3:          // opérateur logique
         {
          // popItem.append(tr("  Vide "            ));
          // popItem.append(tr("  et               "));
          // popItem.append(tr("  ou               "));
          // popItem.append(tr("  sans             "));

          CMetaFiltreLine::GetListLogicOper(popItem);
          QString popRet       = DoPopupList(popItem);
          if (popRet == tr("  Vide ")) pQListViewItem->setText(pos, "");
          else if (popRet.length())    pQListViewItem->setText(pos, popRet);
         }
      break;
   }
 // ListViewTest_To_ClassDenombreList( listView_DenombreTest, pCDenombre );
*/
}

//----------------------------------------- ClassDenombreList_To_ListViewTest ---------------------------------------------------------
void Wdg_Central::ClassDenombreListToListViewTest( QListView *pQListView, CDenombre  *pCDenombre )
{if (pCDenombre == 0)   return;
 CDenombreValidTest *pCDenombreValidTest = 0;
 QListViewItem      *pQListViewItem      = 0;
 QListViewItem      *last_pQListViewItem = 0;
 pQListView->clear();
 for ( pCDenombreValidTest = pCDenombre->m_ValidTestList.first(); pCDenombreValidTest; pCDenombreValidTest = pCDenombre->m_ValidTestList.next() )
     {
      if (last_pQListViewItem)
         {pQListViewItem = new QListViewItem(pQListView, last_pQListViewItem, pCDenombreValidTest->m_Test,
                                                                              pCDenombreValidTest->m_RefValue,
                                                                              pCDenombreValidTest->m_RegExp,
                                                                              pCDenombreValidTest->m_LogicOp);
         }
      else
         {pQListViewItem = new QListViewItem(pQListView, pCDenombreValidTest->m_Test,
                                                         pCDenombreValidTest->m_RefValue,
                                                         pCDenombreValidTest->m_RegExp,
                                                         pCDenombreValidTest->m_LogicOp);
         }
      last_pQListViewItem = pQListViewItem;
     }
 if (pQListViewItem)
    {pQListView->setCurrentItem (pQListViewItem );
     pQListView->setSelected (pQListViewItem , TRUE);
    }
}

//----------------------------------------- ListViewTest_To_ClassDenombreList ---------------------------------------------------------
void Wdg_Central::ListViewTest_To_ClassDenombreList( QListView *pQListView, CDenombre  *pCDenombre )
{if (pCDenombre == 0)   return;
 pCDenombre->m_ValidTestList.clear();
 QListViewItemIterator it( pQListView );
 while ( it.current() )
    { QListViewItem *pQListViewItem = it.current();
      CDenombreValidTest *pCDenombreValidTest = new CDenombreValidTest(pQListViewItem->text(0),
                                                                       pQListViewItem->text(1),
                                                                       pQListViewItem->text(2),
                                                                       pQListViewItem->text(3)
                                                                      );
      if (pCDenombreValidTest && pCDenombreValidTest->isValid())  pCDenombre->m_ValidTestList.append(pCDenombreValidTest);
      ++it;
    }
}

//----------------------------------------- listView_DenombreTest_mouseButtonClicked ---------------------------------------------------------
void Wdg_Central::listView_DenombreTest_mouseButtonClicked( int, QListViewItem *, const QPoint &, int )
{

}
//----------------------------------------- pushButton_AddTestClassDenombre_clicked ---------------------------------------------------------
void Wdg_Central::pushButton_AddTestClassDenombre_clicked()
{

}


//----------------------------------------- pushButton_DelTestClassDenombre_clicked ---------------------------------------------------------
void Wdg_Central::pushButton_DelTestClassDenombre_clicked()
{

}


//----------------------------------------- pushButton_DelAllListDenombreTest_clicked ---------------------------------------------------------
void Wdg_Central::pushButton_DelAllListDenombreTest_clicked()
{

}

//----------------------------------------- CDenombreGetByName ---------------------------------------------------------
CDenombre* Wdg_Central::CDenombreGetByName(const QString &name)
{CDenombre *pCDenombre;
 for ( pCDenombre = m_CDenombreList.first(); pCDenombre; pCDenombre = m_CDenombreList.next() )
     {if (pCDenombre->GetName() == name) return pCDenombre;
     }
 return 0;
}

////////////////////////////////////////////////////// Parties communes aux deux listes de champs //////////////////////////////////////
//---------------------------------------------------- SetListeChamp -------------------------------------------------------------------
void Wdg_Central::SetListeChamp(QListView* pQListView, QListViewItem *pQListViewItem, int pos )
{QStringList popItem;
 QString     popRet;
 if (pQListViewItem<=0) return;
 switch(pos)
   {case 0:          // Operateur de delimitation
         {CMetaFiltre::GetDelimiterList(popItem);
          QString popRet       = DoPopupList(popItem);
          if (popRet == tr("  Vide ")) pQListViewItem->setText(pos, "");
          else if (popRet.length())    pQListViewItem->setText(pos, popRet);
         }
         break;
    case 1:          // champ
      if (IfAjoutListePossible(pQListView))
         {if (pQListView==listView_Sql_MetaFields)
             {GetMetaFieldsList(popItem);
             }
          else
             {popItem.append(m_pCMoteurBase->m_DOSS_INDEX_NOM);
              popItem.append(m_pCMoteurBase->m_DOSS_INDEX_PRENOM);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_NSS);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_SEX);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_JFNOM);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_NB_ENF);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_RUE);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_CODE_POST);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_VILLE);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_CEDEX);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_PAYS);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_TEL1);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_TEL2);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_TEL3);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_TEL1_TYP);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_TEL2_TYP);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_TEL3_TYP);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_EMAIL);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_NUMSS);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_IS_ASS);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_NOM_ASS);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_PRENOM_ASS);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_PROF);
              popItem.append(m_pCMoteurBase->m_DOSS_IDENT_TITRE);
              popItem.append(m_pCMoteurBase->m_DOSS_RUB_HEAD_NOM_DATE);        // Court texte descriptif de l'enregistrement.
              popItem.append(m_pCMoteurBase->m_DOSS_RUB_HEAD_DATE);            // date de l'enregistrement
              popItem.append(m_pCMoteurBase->m_DOSS_RUB_HEAD_FIN);             // date de fin de l'enregistrement
              popItem.append(m_pCMoteurBase->m_DOSS_RUB_HEAD_TYPE);            // Type de rubrique
              popItem.append(m_pCMoteurBase->m_DOSS_RUB_HEAD_USER);            // utilisateur associé � rubrique
              popItem.append(m_pCMoteurBase->m_DOSS_RUB_HEAD_SIGN_USER);       // utilisateur associé signataire de la rubrique
              popItem.append(m_pCMoteurBase->m_DOSS_INDEX_GUID);
             }
          QString popRet       = DoPopupList(popItem);
          if (popRet.length()) pQListViewItem->setText(pos, popRet);
         }
         break;
    case 2:          // Operateur
      if (IfAjoutListePossible(pQListView))
         {GetListOperateur(popItem);
          QString popRet  = DoPopupList(popItem);
          if (popRet == tr("  Vide ")) pQListViewItem->setText(pos, "");
          else if (popRet.length())    pQListViewItem->setText(pos, popRet);
         }
         break;
    case 3:          // valeur
      if (IfAjoutListePossible(pQListView))
         {QString            texte =  pQListViewItem->text(pos);
          QVariant::Type field_Typ = GetFieldType( pQListViewItem->text(1) );
          if (field_Typ == QVariant::DateTime )
             {GetDateTime( texte );
              pQListViewItem->setText(pos, texte );
             }
          else if ( field_Typ == QVariant::Date)
             {GetDateTime( texte );
              pQListViewItem->setText(pos, texte );
             }
          else if (pQListViewItem->text(1)==m_pCMoteurBase->m_DOSS_RUB_HEAD_TYPE)
             {popItem         = CMetaField::GetPossiblesRubTypesList(1);
              QString popRet  = DoPopupList(popItem);
              if (popRet.length()) pQListViewItem->setText(pos, CMetaField::RubNameToRubType(popRet) );
             }
          else if (CGenTools::DialogGetString(this, tr("Saisir la valeur qui sera évaluée par l'opérateur : ") + pQListViewItem->text(2), texte,QCursor::pos().x(), QCursor::pos().y()) == QDialog::Accepted)
             { pQListViewItem->setText(pos, texte );
             }
         }
         break;
    case 4:          // et ou
      if (IfAjoutListePossible(pQListView))
         {CMetaFiltreLine::GetListLogicOper(popItem);
          QString popRet       = DoPopupList(popItem);
          if (popRet == tr("  Vide ")) pQListViewItem->setText(pos, "");
          else if (popRet.length())    pQListViewItem->setText(pos, popRet);
         }
      break;
    case 5:          // type
      if (IfAjoutListePossible(pQListView))
         {CMetaFiltreLine::GetListConversion(popItem);
          QString popRet       = DoPopupList(popItem);
          if (popRet.length())    pQListViewItem->setText(pos, popRet);
         }
         break;
   }
}

//------------------------------------------------------------ GetListOperateur_ID-------------------------------------------------------
int Wdg_Central::GetListOperateur_ID(QString operateur)
{return CMetaField::GetListOperateur_ID(operateur);
}

//------------------------------------------------------------ GetListOperateur-------------------------------------------------------
void Wdg_Central::GetListOperateur(QStringList &opList)
{CMetaField::GetListOperateur(opList);
}

//-------------------------------------------- pushButton_AddListField_clicked -------------------------------------------------------
void Wdg_Central::pushButton_AddListField_clicked(QListView* pQListView)
{//m_pCMoteurBase->GotoDebug();
 QListViewItem *last_pQListViewItem =  pQListView->lastItem();
 QListViewItem *pQListViewItem      = 0;
 QString                   str      = "";
 if (last_pQListViewItem )
    { if (IfAjoutListePossible(last_pQListViewItem))
         {int id = CMetaFiltreLine::GetListLogicOper_ID(last_pQListViewItem->text(4));
          if (id>0)  // si pas vide
             { str = "N";
             }
          else
             { QStringList popItem;
               CMetaFiltre::GetDelimiterList(popItem);
               str       = DoPopupList(popItem);
             }
         if (str.length())
            { if (str =="N") str = "";
              pQListViewItem = new QListViewItem(pQListView,last_pQListViewItem,str);
            }
        }
    }
 else
    {pQListViewItem = new QListViewItem(pQListView,"  ( ");
    }
 if (pQListViewItem) {pQListView->setCurrentItem (pQListViewItem );
                      pQListView->setSelected (pQListViewItem , TRUE);
                     }
}

//----------------------------------------- IfAjoutListePossible ---------------------------------------------------------
int Wdg_Central::IfAjoutListePossible(QListView* pQListView)
{QListViewItem *last_pQListViewItem =  pQListView->lastItem();
 QListViewItem *pQListViewItem      =  pQListView->currentItem();

 if (pQListViewItem && last_pQListViewItem && last_pQListViewItem == pQListViewItem)
    return IfAjoutListePossible(last_pQListViewItem);
 else
    return 1;
}

//----------------------------------------- IfAjoutListePossible ---------------------------------------------------------
int Wdg_Central::IfAjoutListePossible(QListViewItem* last_pQListViewItem)
{if (last_pQListViewItem && last_pQListViewItem->text(0)==tr("  ) "))
    {QMessageBox::critical (0, QObject::tr("Ajouter un champ à la liste"),
                               QObject::tr("Ajout impossible car cette liste est close par une parenthèse fermante \")\""),
                               QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
     return 0;
    }
 return 1;
}

//----------------------------------------- pushButton_DelListIdentField_clicked ---------------------------------------------------------
void Wdg_Central::pushButton_DelListIdentField_clicked()
{QListViewItem *pQListViewItem = listView_Sql_IdentFields-> currentItem ();
 if (pQListViewItem == 0) return;
 delete pQListViewItem ;
 if (listView_Sql_IdentFields->childCount() <= 0) textEdit_Sql_IdentSelect->setText("");
 else                                             textEdit_Sql_IdentSelect->setText(ConstructRequeteIdentSelect());
}

//----------------------------------- GetDateTime -----------------------------------------------------------------------------------------
void Wdg_Central::GetDateTime(QString &dateTime)
    {   Dlg_Calendar          *dlg  = new Dlg_Calendar(this, "Calendar_Dial", FALSE);
        if (dlg ==0)                                                   return;
        dlg->setDateTimeNumeric_yyyyMMjj_Order(dateTime );
        dlg->setCaption(tr("Choix dans la date"));
        dlg->move ( QCursor::pos().x(), QCursor::pos().y() );
        if (dlg->exec()== QDialog::Accepted )
           {dateTime = dlg->get_SQL_DateTime();
           }
        delete dlg;
    }

//////////////////////////////////////////////////////   ENREGISTRER   CHARGER   ////////////////////////////////////////////////////////
//----------------------------------------- pushButton_EPP_Enregistrer_clicked ---------------------------------------------------------
void Wdg_Central::pushButton_EPP_Enregistrer_clicked()
{   QString fn = G_pCApp->m_PathAppli + "/Modeles";
    QFileDialog *fd = new QFileDialog (fn,
                                       tr( " Fichiers de texte (*.TXT *.txt *.STAT *.stat);;"
                                           " Tous les fichiers (*)"),
                                       this,
                                       "SaveDlg"
                                      ) ;
    if (fd==0)                                             return;
    fd->setMode(QFileDialog::AnyFile );    // selection multiple
    if ( fd->exec() == QDialog::Accepted )
       {QString fn = fd->selectedFile();
        if ( !fn.isEmpty() )
           {CGestIni::Param_UpdateToDisk(fn, EPP_Serialize(" "));
           }
       }
    delete fd;
}

//----------------------------------------- pushButton_EPP_Charger_clicked ---------------------------------------------------------
void Wdg_Central::pushButton_EPP_Charger_clicked()
{
    QString      fn = G_pCApp->m_PathAppli + "/Modeles";
    QFileDialog *fd = new QFileDialog (fn,
                                       tr( "Fichiers texte (*.TXT *.txt *.STAT *.stat);;"
                                           "Tous les fichiers (*)" ),
                                       this,
                                       "OuvrirDlg"
                                      ) ;
    if (fd==0)                                             return;
    if ( fd->exec() == QDialog::Accepted )
       {QString fn = fd->selectedFile();
        QString data;
        CGestIni::Param_UpdateFromDisk(fn, data);
        //m_pCMoteurBase->GotoDebug();
        EPP_UnSerialize(data);
       }
    delete fd;
}
 
/////////////////////////////////////////////   SERIALISATION DESERIALISATION   ////////////////////////////////////////////////////////
//----------------------------------------- EPP_Serialize ------------------------------------------------------------------------------
QString Wdg_Central::EPP_Serialize(const QString &in_ofset)
{QString result = "";
 QString  ofset = in_ofset;
          ofset = ofset.prepend("\n");
        result += QListView_Serialize(listView_Sql_IdentFields, "  ", "Filtre_SQL");
        result += ofset + "<Sql_Result_Data>" + CDenombre::StrStringToXML(textEdit_Sql_IdentSelect->text())       + "</Sql_Result_Data>";
        result += QListView_Serialize(listView_Sql_MetaFields,  "  ", "Filtre_Meta");
        result += QListView_Serialize(listView_Def_MetaFields,  "  ", "MetaFieldDef");
        result += PRT_LIST_CDenombreSerialize("  ");
        result += resultHeaderSerialize("  ");
        result += DateSerialize("  ");

 return result;
}

//----------------------------------------- EPP_UnSerialize ---------------------------------------------------------
int Wdg_Central::EPP_UnSerialize(const QString &data)
{int          pos = 0;
 QString strToGet = "";
 m_pCMoteurBase->GotoDebug();
 pos = QListView_UnSerialize(listView_Sql_IdentFields, data, "Filtre_SQL",  pos);
 pos = GetXMLValue(data, "<Sql_Result_Data>", strToGet, pos);textEdit_Sql_IdentSelect->setText( XMLToStrString(strToGet));
 pos = QListView_UnSerialize(listView_Sql_MetaFields, data, "Filtre_Meta",  pos);
 pos = QListView_UnSerialize(listView_Def_MetaFields, data, "MetaFieldDef", pos);
 pos = PRT_LIST_CDenombreUnSerialize(data, pos);
 pos = resultHeaderUnSerialize(data, pos);
 //pos = DateUnSerialize(data, pos);
 //........................ reactualiser les affichages ................................................................
 QListViewItem *pQListViewItem =  listView_ClassDenombreList->firstChild ();
 if (pQListViewItem) {listView_ClassDenombreList->setCurrentItem (pQListViewItem );
                      listView_ClassDenombreList->setSelected (pQListViewItem , TRUE);
                      // ClassDenombreListToListViewTest( listView_DenombreTest, CDenombreGetByName(pQListViewItem->text(0)) );
                     }
 return pos;
}

//----------------------------------------- DateSerialize ---------------------------------------------------------
QString Wdg_Central::DateSerialize(const QString &in_ofset)
{QString  result   = "";
 QString   ofset   = in_ofset;
           ofset   = ofset.prepend("\n");
           result += ofset + "<Date Heure>";
           result += ofset + "  <Date de debut>"  + mle_deb->text()      +  "</Date de debut>";
           result += ofset + "  <Date de fin>"    + mle_fin->text()      +  "</Date de fin>";
           result += ofset + "  <Heure de debut>" + mle_HeureDeb->text() +  "</Heure de debut>";
           result += ofset + "  <Heure de fin>"   + mle_HeureFin->text() +  "</Heure de fin>";
           result += ofset + "</Date Heure>";
 return result;
}

//----------------------------------------- QListView_UnSerialize ---------------------------------------------------------
int Wdg_Central::DateUnSerialize(const QString &xmlData, int pos_bloc)
{int          pos       = 0;
 QString xmlTag         = "";
 QString strToGet       = "";
 QString  dataToExamine = "";
 xmlTag = "<Date Heure>";
 if ( (pos_bloc = GetXMLValue(xmlData, xmlTag,     dataToExamine, pos_bloc))  ==-1 )             // tenter avec curseur
    { if ( (pos_bloc = GetXMLValue(xmlData, xmlTag, dataToExamine, 0 )) ==-1 ) {m_LastError = " ERREUR DateUnSerialize() : donnée \"" +xmlTag + "\" non retrouvée.";  return -1;}     // si echec tenter du d�ut
    }
 xmlTag = "<Date de debut>";
 if ( (pos = GetXMLValue(dataToExamine, xmlTag, strToGet, pos))==-1 ) {m_LastError = " ERREUR DateUnSerialize() : donnée \"" +xmlTag + "\" non retrouvée.";           return -1;}     // si echec tenter du d�ut
 mle_deb->setText(strToGet);
 xmlTag = "<Date de fin>";
 if ( (pos = GetXMLValue(dataToExamine, xmlTag, strToGet, pos))==-1 ) {m_LastError = " ERREUR DateUnSerialize() : donnée \"" +xmlTag + "\" non retrouvée.";           return -1;}     // si echec tenter du d�ut
 mle_fin->setText(strToGet);
 xmlTag = "<Heure de debut>";
 if ( (pos = GetXMLValue(dataToExamine, xmlTag, strToGet, pos))==-1 ) {m_LastError = " ERREUR DateUnSerialize() : donnée \"" +xmlTag + "\" non retrouvée.";           return -1;}     // si echec tenter du d�ut
 mle_HeureDeb->setText(strToGet);
 xmlTag = "<Heure de fin>";
 if ( (pos = GetXMLValue(dataToExamine, xmlTag, strToGet, pos))==-1 ) {m_LastError = " ERREUR DateUnSerialize() : donnée \"" +xmlTag + "\" non retrouvée.";           return -1;}     // si echec tenter du d�ut
 mle_HeureFin->setText(strToGet);
 return pos;
}

//----------------------------------------- QListView_Serialize ---------------------------------------------------------
QString Wdg_Central::QListView_Serialize(QListView *pQListView, const QString &in_ofset, const QString &prefix_Name)
{QString  result = "";
 QString   ofset = in_ofset;
           ofset = ofset.prepend("\n");
 int     nb_item = pQListView->childCount();
         result += ofset + "<" + prefix_Name + "_List>";
         result += ofset + "  <" + prefix_Name + "_List_count>"+ QString::number( nb_item ) + "</" + prefix_Name + "_List_count>";
 int     nb_col  = pQListView->header ()->count();
         result += ofset + "  <" + prefix_Name + "_List_NbCol>"+ QString::number(nb_col )   + "</" + prefix_Name + "_List_NbCol>";

 int        nb   = 0;
 QListViewItemIterator it( pQListView );
 while ( it.current() )
    { QListViewItem *pQListViewItem = it.current();
      result += ofset + "  <" + prefix_Name + "_List_Line>";
      for (int i = 0; i<nb_col; ++i)
          {QString tag_deb = "<" + prefix_Name + "_Item" + QString::number(nb) + "_Col" + QString::number(i) + ">";
           QString tag_end = tag_deb; tag_end.insert(1,"/");
           result += ofset + "    " + tag_deb + CDenombre::StrStringToXML(pQListViewItem->text(i)) + tag_end;
          }
      result += ofset + "  </" + prefix_Name + "_List_Line>";
      ++it;
      ++nb;
    }
 result += ofset + "</" + prefix_Name + "_List>";
 return result;
}

//----------------------------------------- QListView_UnSerialize ---------------------------------------------------------
int Wdg_Central::QListView_UnSerialize(QListView *pQListView,  const QString &xmlData, const QString &prefix_Name, int pos_bloc)
{          m_LastError = "";
 QString        xmlTag = "";
 QString dataToExamine = "";
 QString   strToGet    = "";
 int       pos         = 0;

 xmlTag = "<" + prefix_Name + "_List>";
 if ( (pos_bloc = GetXMLValue(xmlData, xmlTag,     dataToExamine, pos_bloc))  ==-1 )             // tenter avec curseur
    { if ( (pos_bloc = GetXMLValue(xmlData, xmlTag, dataToExamine, 0 )) ==-1 ) {m_LastError = " ERREUR QListView_UnSerialize() : donnée \"" +xmlTag + "\" non retrouvée.";  return -1;}     // si echec tenter du d�ut
    }

 xmlTag = "<" + prefix_Name + "_List_count>";
 if ( (pos = GetXMLValue(dataToExamine, xmlTag, strToGet, pos))==-1 ) {m_LastError = " ERREUR QListView_UnSerialize() : donnée \"" +xmlTag + "\" non retrouvée.";  return -1;}     // si echec tenter du d�ut
 int nb_item  = strToGet.toInt();

 xmlTag = "<" + prefix_Name + "_List_NbCol>";
 if ( (pos = GetXMLValue(dataToExamine, xmlTag, strToGet, pos))==-1 ) {m_LastError = " ERREUR QListView_UnSerialize() : donnée \"" +xmlTag + "\" non retrouvée.";  return -1;}     // si echec tenter du d�ut
 int nb_col    = strToGet.toInt();          // nombre de colonnes indiquées dans la structure XML
 int nb_col_LV = pQListView->columns();     // nombre de colonnes indiquées de la listview à charger
 pQListView->clear();
 QListViewItem *pQListViewItem       = 0;
 QListViewItem *pQListViewItemAfter  = 0;
 for (int nb=0; nb<nb_item; ++nb)
     {if (nb == 0)   pQListViewItem = new QListViewItem ( pQListView );
      else           pQListViewItem = new QListViewItem ( pQListView,  pQListViewItemAfter);
      pQListViewItemAfter = pQListViewItem;

      for (int i = 0; i<nb_col; ++i)
          {xmlTag = "<" + prefix_Name + "_Item" + QString::number(nb) + "_Col" + QString::number(i) + ">";
           if ( (pos = GetXMLValue(dataToExamine, xmlTag, strToGet, pos))==-1 ) {m_LastError = " ERREUR QListView_UnSerialize() : donnée \"" +xmlTag + "\" non retrouvée.";  return -1;}     // si echec tenter du d�ut
           if (pQListViewItem && i<nb_col_LV) pQListViewItem->setText(i, XMLToStrString(strToGet) );
          }
     }
 return pos_bloc;
}

//----------------------------------------- PRT_LIST_CDenombreSerialize ---------------------------------------------------------
QString Wdg_Central::PRT_LIST_CDenombreSerialize(const QString &in_ofset)
{QString result = "";
 QString  ofset = in_ofset;
          ofset = ofset.prepend("\n");
 CDenombre *pCDenombre;
      result    += ofset + "<CDenombre_List>";
      result    += ofset + "  <CDenombre_List_count>" + QString::number(m_CDenombreList.count() ) +  "</CDenombre_List_count>";
 for ( pCDenombre = m_CDenombreList.first(); pCDenombre; pCDenombre = m_CDenombreList.next() )
     {result    += ofset + "  <CDenombre_List_data>";
      result    += ofset +   pCDenombre->Serialize(in_ofset+"    ");
      result    += ofset + "  </CDenombre_List_data>";
     }
      result    += ofset + "</CDenombre_List>";
 return result;
}

//----------------------------------------- PRT_LIST_CDenombreUnSerialize ---------------------------------------------------------
int Wdg_Central::PRT_LIST_CDenombreUnSerialize(const QString &data, int pos_bloc)
{          m_LastError = "";
 QString        xmlTag = "";
 QString dataToExamine = "";
 QString   strToGet    = "";
 int       pos         = 0;
 m_CDenombreList.clear();
 char *pt = (char*)(const char*) data;
 pt += pos_bloc;
 xmlTag = "<CDenombre_List>";
 if ( (pos_bloc = GetXMLValue(data, xmlTag,     dataToExamine, pos_bloc))  ==-1 )             // tenter avec curseur
    { if ( (pos_bloc = GetXMLValue(data, xmlTag, dataToExamine, 0 )) ==-1 ) {m_LastError = " ERREUR PRT_LIST_CDenombreUnSerialize() : donnée \"" +xmlTag + "\" non retrouvée.";  return -1;}     // si echec tenter du d�ut
    }

 xmlTag = "<CDenombre_List_count>";
 if ( (pos = GetXMLValue(dataToExamine, xmlTag, strToGet, pos))==-1 ) {m_LastError = " ERREUR PRT_LIST_CDenombreUnSerialize() : donnée \"" +xmlTag + "\" non retrouvée.";  return -1;}     // si echec tenter du d�ut
 int nb_item  = strToGet.toInt();

 listView_ClassDenombreList->clear();
 QListViewItem *pQListViewItem       = 0;
 QListViewItem *pQListViewItemAfter  = 0;
 for (int nb=0; nb<nb_item; ++nb)
     {QString xmlDef_CDenombre;
      if ( (pos = GetXMLValue(dataToExamine, "<CDenombre_List_data>", xmlDef_CDenombre, pos))==-1 ) {m_LastError = " ERREUR PRT_LIST_CDenombreUnSerialize() : donnée \"<CDenombre_List_data>\" non retrouvée.";  return -1;}     // si echec tenter du d�ut
      CDenombre *pCDenombre = new CDenombre(xmlDef_CDenombre);
      if (pCDenombre->isValid())
         { m_CDenombreList.append(pCDenombre);
           if (nb == 0)   pQListViewItem = new QListViewItem ( listView_ClassDenombreList, pCDenombre->GetName(), pCDenombre->GetFieldName() );
           else           pQListViewItem = new QListViewItem ( listView_ClassDenombreList,  pQListViewItemAfter,  pCDenombre->GetName(), pCDenombre->GetFieldName());
           pQListViewItemAfter = pQListViewItem;
         }
     }
 return pos_bloc;
}

//------------------------------------------------ GetXMLValue --------------------------------------------------------
// ACTION extrait la valeur contenue entre deux balises XML
// ENTREE : const QString &dataToExamine : texte a analyser
//          const QString &xmlToken : token de la balise XML doit �re de  la forme : "<xxxxxxxxxx>"
//          QString &valueToGet     : QString dans laquelle d�oser le r�ultat
//          int pos /* = -1 */   : position �laquelle rechercher la recherche si -1 (valeur par d�aut) la recherche se fait �partir du d�ut
//                                 si l'on est certain que les tokens sont ordonn� alors l'on peut se servr du retour de la fonction
//                                 pour positionner pos de fa�n �acc��er le traitement.
// RETOUR : int position situ� apr� le token fermant, permet d'avancer sur le token suivant.

int  Wdg_Central::GetXMLValue(const QString &dataToExamine, const QString &xmlToken_in, QString &valueToGet, int pos /* = -1 */)
{valueToGet       = "";
 QString xmlToken = xmlToken_in;
 int deb = dataToExamine.find(xmlToken, pos);
 if (deb == -1) {m_LastError = TR("ERREUR Wdg_Central::GetXMLValue() Mention de début manquante : \"" + xmlToken + "\""); return -1;}
 deb += xmlToken.length();
 int end = dataToExamine.find(xmlToken.insert(1,"/"), deb);
 if (end == -1) {m_LastError = TR("ERREUR Wdg_Central::GetXMLValue() Mention de  fin  manquante : \"" + xmlToken +"\"");  return -1;}

 valueToGet = dataToExamine.mid(deb , end - deb );
 return end + xmlToken.length();
}

//---------------------------------- StrStringToXML ---------------------------------------------------------------------------
QString Wdg_Central::StrStringToXML( const QString &txt_in)
{QString txt = txt_in;
 txt.replace(">", "&gt;");
 txt.replace("<", "&lt;");
 return txt;
}
//---------------------------------- XMLToStrString ---------------------------------------------------------------------------
QString Wdg_Central::XMLToStrString( QString &txt)
{txt.replace("&gt;",">");
 txt.replace("&lt;","<");
 return txt;
}

////////////////////////////////////////////////// COMPTAGE ///////////////////////////////////////////////////////////
//------------------------------------ initFieldsList -----------------------------------------------------------------
void Wdg_Central::initFieldsList(QStringList &list)
{GetFieldsList(list,      m_pCMoteurBase->m_DOSS_RUB_HEAD_TBL_NAME);
 GetFieldsList(list,      m_pCMoteurBase->m_DOSS_INDEX_TBL_NAME);
 GetFieldsList(list,      m_pCMoteurBase->m_DOSS_IDENT_TBL_NAME);
 GetFieldsList(list,      m_pCMoteurBase->m_DOSS_RUB_DATA_TBL_NAME);

 //GetFieldsList(m_FieldsList_Note,          const QString &tableName);
 //GetFieldsList(m_FieldsList_Intervenants,  const QString &tableName);
}

//---------------------------------- EPP_Do ---------------------------------------------------------------------------
//    cette fonction ne delivre du gaz que sur option.
typedef QMap <QString, QString> MAP_METAFIELD_VALUE;
void Wdg_Central::EPP_Do()
{   if ( textEdit_Sql_IdentSelect->text().stripWhiteSpace().length() == 0 )
       { int ret = QMessageBox::information( this, tr("Requête vide"),
                                             tr ( "La zone de requête est vide.<br>"
                                                   "En choisir une ?"
                                               ) ,
                                             tr("&Choisir"), tr("&Quitter"), 0,
                                             1, 1 );
             if (ret==1) return;
             pushButton_EPP_Charger_clicked();
             if ( textEdit_Sql_IdentSelect->text().stripWhiteSpace().length() == 0 ) return;
       }
    ListView_DossiersRetenus->clear();
    ListView_PatientRejetes->clear();
    //.............. recuperer la liste des champs pour ...............................................................
    //               avoir leur indice pour recuperer value
    QStringList sqlFieldList;
    initFieldsList(sqlFieldList);
    CMetaField *pCMetaField;
    QMap <QString,QString> last_map_fieldName_value;
    listView_Result_Fields->clear();
    QMap <int, MAP_METAFIELD_VALUE >         map_metaname_records;
    QMap <QString, QString>                  map_metanames_values;

    //........................ executer la requete SQL ..............................................................
    if (m_pCMoteurBase->m_DataBase->isOpen()==FALSE && m_pCMoteurBase->m_DataBase->open()==FALSE)  return ;  // si point = erreur d'ouverture de la base
    QString requete = textEdit_Sql_IdentSelect->text();
    QSqlQuery query(requete , m_pCMoteurBase->m_DataBase );

    //........................ créer la liste des meta-field à partir de la list-view .................................
    //                         des définitions de métafield
    m_CMetaFieldList.clear();
    QListViewItemIterator it( listView_Def_MetaFields );
    while ( it.current() )
          { QListViewItem *pQListViewItem = it.current();
            pCMetaField = new CMetaField(pQListViewItem->text(0),
                                         pQListViewItem->text(1),
                                         pQListViewItem->text(2),
                                         G_pCApp->m_pCMedicaBase
                                        );
            if (pCMetaField && pCMetaField->isValid() != -1)
               {m_CMetaFieldList.append(pCMetaField);
               }
           ++it;
         }
    //........................ créer le MetaFiltre avec la listes des meta-champs et de la liste view ...............
    //                         de definition du meta-filtre
    CMetaFiltre metaFiltre(&m_CMetaFieldList, listView_Sql_MetaFields);

    //.......................... relier les classes de dénombrage avec les meta-champs...........................
    //                           ou les indices de champ SQL pour permettre l'extraction future
    //                           des données.
    CDenombre *pCDenombre;
    for ( pCDenombre   = m_CDenombreList.first(); pCDenombre; pCDenombre = m_CDenombreList.next() )
        {pCDenombre->m_mapResultCount.clear();
         QString field_name = pCDenombre->m_FieldWork.mid(2);
         if (pCDenombre->m_FieldWork[0]=='*')
            {for ( pCMetaField   = m_CMetaFieldList.first(); pCMetaField; pCMetaField = m_CMetaFieldList.next() )
                 {if (pCMetaField->m_Name == field_name)
                     {pCDenombre->m_pCMetaField = pCMetaField;
                      pCDenombre->m_Field_ID    = -1;
                     }
                 } // endp for (CMetaField   = m_CMetaFieldList.first() ...
            }
         else
            {pCDenombre->m_Field_ID    = sqlFieldList.findIndex (pCDenombre->m_FieldWork.mid(2));
             pCDenombre->m_pCMetaField = 0;
            }
        }

    //........................ executer la requete SQL ..............................................................
    //    if (m_pCMoteurBase->m_DataBase->isOpen()==FALSE && m_pCMoteurBase->m_DataBase->open()==FALSE)  return ;  // si point = erreur d'ouverture de la base
    //    QString requete = textEdit_Sql_IdentSelect->text();
    //    QSqlQuery query(requete , m_pCMoteurBase->m_DataBase );
    //........................ recuperer index des champs dans la query ................................
    //                        dont on va avoir besoin pour créer la liste
    if (checkBox_DossiersRetenus->isChecked()) ListView_DossiersRetenus->clear();
    if (checkBox_DossiersRejetes->isChecked()) ListView_PatientRejetes->clear();
    int   id_Nom            = sqlFieldList.findIndex ("FchGnrl_NomDos");
    int   id_Prenom         = sqlFieldList.findIndex ("FchGnrl_Prenom");
    int   id_GUID           = sqlFieldList.findIndex ("FchGnrl_IDDos");
    int   id_Pk             = sqlFieldList.findIndex ("ID_PrimKey");
    int   id_Blobs          = sqlFieldList.findIndex ("RbDate_DataRub");
    int   id_RubType        = sqlFieldList.findIndex ("RbDate_TypeRub");
    int   id_DtNss          = sqlFieldList.findIndex ("FchPat_Nee");
    int   id_User           = sqlFieldList.findIndex ("RbDate_CreateUser");
    int   id_DateRub        = sqlFieldList.findIndex ("RbDate_Date");
    int   id_record         = sqlFieldList.findIndex ("RbDate_PrimKey");
    long  nbRecord          = 0;
    long  nbRecordTotal     = 0;
    QString        texte    = "";
    QString     lastGUID    = "";
    QString      lastNom    = "";
    QString   lastPrenom    = "";
    QString   last_id_pk    = "";
    QString last_record_id  = "";
    int          record_id  = -1;
    QString        curGUID  = "";
    textEdit_SelectDossiers->setText("");

    //.................. Recuperer la liste de la requete ..........................................
    if ( query.isActive() )
    {
        QByteArray qByteArray;
        while (query.next())
        {   ++nbRecordTotal;
            curGUID             = query.value(id_GUID).toString();
            record_id           = query.value(id_record).toInt();
            //.................................. SI CHANGEMENT DE DOSSIER .........................................................
            //                                   alors tous les metachamps devraient etre renseignes
            //                                   alors on peut filtrer et eventuellement compter
            if (curGUID != lastGUID)
               { if (lastGUID.length())     // si pas la première fois
                    { //...................... les métachamps charges, actionner le meta-filtre ..................................
                      // PB : les premières rubriques sont des prescriptions pour lesquelles les metachamps sont renseignés
                      //       et doivent etre evalues, mais le filtre d'inclusion se fait sur un métachamp de la rubrique
                      //       observation, non encore renseigné du fait quelle arrive après, le test d'inclusion ne peut donc
                      //       se faire et les éléments de la rubrique presccription ne  sont pas evalues.
                      // IDEE : Les blobs sont tries de facon à ce qu'ils soient tous pour le meme patient à la suite
                      //        les uns des autres, on detecte les changements de dossier avec : curGUID != lastGUID
                      //        et alors l'on traite le meta filtre et on compte.
                      //        le filtre s'opere donc sur un dossier dont tous les blobs auront au préalable renseigné
                      //        tous les méta champs.
                      // A CE STADE : m_CMetaFieldList (que CMetaFiltre connait par un pointeur dessus) liste tous les meta champ
                      //              qui contiennent toutes les valeurs extraites qu'elles verifient ou non le test
                       bool ret_metaFiltre =  metaFiltre.DoTest();

                       //................ si liste de dossiers retenus à remplir la renseigner ....................................
                       if (checkBox_DossiersRetenus->isChecked() && ret_metaFiltre /*&& lastGUID != curGUID */)
                          { new QListViewItem ( ListView_DossiersRetenus,
                                                lastNom,
                                                lastPrenom,
                                                last_id_pk,
                                                lastGUID
                                              );
                          }
                      //...................... sortir les resultats dans la listview ............................
                      if ( ret_metaFiltre && G_pCApp->m_ResultEnabled)
                         { QHeader       *pQHeader   = listView_Result_Fields->header();
                           int            nb_section = pQHeader->count();
                           int                    nb = 0;
                           int                   max = G_pCApp->m_nb_max_results.toInt();
                           QMap <int, MAP_METAFIELD_VALUE >::Iterator it;
                           for ( it = map_metaname_records.begin(); it != map_metaname_records.end() && nb<max; ++it )
                               { if ( metaFiltre.recordTestState( it.key()) == false) continue;

                                 QListViewItem *pQListViewItem  = new QListViewItem(listView_Result_Fields);
                                 ++ nb;
                                 QMap <QString,QString> field_name_value  = it.data();
                                 for ( int section   = 0; section < nb_section; ++section)
                                     { //qDebug(tr("cur_section = %1").arg(QString::number(section)));
                                       QString field_name = pQHeader->label ( section ); // le nom de la section est le nom du champ ou metachamp
                                       QChar   field_typ  = field_name[0];
                                       field_name         = field_name.mid(2);
                                       if ( field_typ =="*" )    // METACHAMP
                                          { QString value = field_name_value[field_name];
                                            pQListViewItem->setText( section, value) ;
                                          }
                                       else
                                          { QString value = last_map_fieldName_value[ field_name ];
                                            pQListViewItem->setText(section, value) ;
                                          }
                                     } // for ( int section   = 0; section < nb_section; ++section)
                               } // for ( it = map_metaname_records.begin(); it != map_metaname_records.end() && nb<max; ++it )
                         } // if ( ret_metaFiltre && pushButton_Results_Add_Field->isVisible())

                      //................ si le metafiltre autorise à traiter ......................................................
                      //                 actionner les denombrements pour compter
                      if (ret_metaFiltre && G_pCApp->m_DenombreEnabled)
                         { 
                           for ( pCDenombre   = m_CDenombreList.first(); pCDenombre; pCDenombre = m_CDenombreList.next() )
                               { pCDenombre->DoResult( last_map_fieldName_value );
                               }
                         }
                    }// endif (currentID.length())

                 for ( pCMetaField   = m_CMetaFieldList.first(); pCMetaField; pCMetaField = m_CMetaFieldList.next() )
                     { pCMetaField->m_ExtractedValues.clear();
                     }
                 lastGUID   = curGUID;
                 lastNom    = CGestIni::Utf8_Query(query, id_Nom );
                 lastPrenom = CGestIni::Utf8_Query(query, id_Prenom );
                 last_id_pk = query.value(id_Pk).toString();
                 //.......... on mape toutes les sorties de la query sur leur nom de champ ..................
                 //           n'est utile que pour retrouver les valeurs fixes de l'identite
                 last_map_fieldName_value.clear();
                 for ( int i=0; i<sqlFieldList.size(); ++i )
                     {  QString field_name = sqlFieldList[i];
                        QString value      = CGestIni::Utf8_Query(query, i );
                        last_map_fieldName_value[field_name] = value ;
                     }
                 // last_query = query;
               } // if (curGUID != lastGUID)
            //..................... RENSEIGNER LES META CHAMPS ....................................................
            //                     recuperer le blobs que si le type de rubrique 
            //                     que query presente existe dans un des meta-champ
            char *ptr            = 0;
            long len             = 0;
            QString rub_type     = query.value(id_RubType).toString();
            int         curs_pos = 0;

            //...................... CHARGER LES META CHAMPS (SI CONCERNES) AVEC LES VALEURS DE CE BLOB ....................................................
            //                       ALIMENTE LE META FILTRE
            //                       passer en revue tous les meta-champs pour trouver celui corespondant au blobs
            //                      si trouve charger le blobs et extraire les données de ce blobs correspondant aux meta-champs
            //                      et charger tous les meta-champs correspondant au type avec les données du blobs
            map_metanames_values.clear();
            for ( pCMetaField   = m_CMetaFieldList.first(); pCMetaField; pCMetaField = m_CMetaFieldList.next() )
                { bool err = FALSE;

                  if ( pCMetaField->m_Rubrique == "99900000")      // Age Patient
                     { // pCMetaField->m_ExtractedValues.clear();
                       QDate     tmp_dt = query.value(id_DtNss).toDate();
                       map_metanames_values.insert(pCMetaField->m_Name, pCMetaField->GetValue(QString::number( CMoteurBase::CalculeDiffAge_Num(&tmp_dt)), curs_pos, record_id));
                       if (pCMetaField->m_LastError[0]=='W')               { if (checkBoxWarning->isChecked())           err = TRUE; }
                       else if      ( pCMetaField->m_LastError.length()>0) { if (checkBox_DossiersRejetes->isChecked())  err = TRUE; }
                       ++nbRecord;
                    }
                  else if (rub_type == pCMetaField->m_Rubrique)                                                                 // on a trouve le premier meta-champ associé à ce blobs
                     { // pCMetaField->m_ExtractedValues.clear();
                       //.................. lire le blobs si pas deja fait..........................................
                       if (ptr==0 )            // on lit le blobs si pas déjà fait
                          { qByteArray = query.value(id_Blobs).toByteArray();
                            ptr        = qByteArray.data();
                            len        = qByteArray.size()-4;
                            texte      = "";
                          } // end if (texte.length()==0 )
                       if (ptr && len>6)
                          { if (strncmp(ptr, "<htm", 4)==0 )     // <?xm  [Ant <htm
                               { if (CGestIni::IsUtf8( ptr )) texte = QString::fromUtf8 ( ptr , len + 4);
                                 else                         texte = ptr;
                                 CHtmlTools::HtmlToAscii(texte, CHtmlTools::StrictASCII);
                               }
                            else if (strncmp(ptr, "<?xm", 4)==0 || strncmp(ptr, "[Ant", 4)==0)
                               { if (CGestIni::IsUtf8( ptr )) texte = QString::fromUtf8 ( ptr , len + 4);
                                 else                         texte = ptr;
                               }
                            else
                               { texte = CMoteurBase::RTF_ToASCII(ptr+4, len);
                               }
                          } //end if (ptr && len>6)

                       //.................. pour ce blobs remplir le meta-champ .......................................
                       QString value = pCMetaField->GetValue(texte, curs_pos, record_id);
                       map_metanames_values.insert(pCMetaField->m_Name,value );

                       if (pCMetaField->m_LastError[0]=='W')               { if (checkBoxWarning->isChecked())           err = TRUE; }
                       else if      ( pCMetaField->m_LastError.length()>0) { if (checkBox_DossiersRejetes->isChecked())  err = TRUE; }

                       ++nbRecord;
                     } //endif (rub_type == pCMetaField->m_Rubrique)
                  else
                     { //pCMetaField->m_ResultList.clear();
                     }
                  if ( err )
                     { new QListViewItem (ListView_PatientRejetes,
                                          CGestIni::Utf8_Query(query, id_Nom ),
                                          CGestIni::Utf8_Query(query, id_Prenom ),
                                          query.value(id_DateRub).toString().left(10),
                                          query.value( id_User ).toString(),
                                          pCMetaField->m_LastError,
                                          query.value(id_GUID).toString()
                                        );
                     }//endif (pCMetaField->m_LastError.length()>0)
                } // endfor ( pCMetaField   = m_CMetaFieldList.first();
             map_metaname_records.insert( record_id , map_metanames_values );
        } // end while (query.next())
     //................................. ne pas oublier le DERNIER DOSSIER ..................................
     if (lastGUID.length())     // si pas la première fois (les méta champs ne sont pas renseignés cas aucun query valide)
        { //...................... les métachamps charges, actionner le meta-filtre ..................................
          query.last();
          bool ret_metaFiltre =  metaFiltre.DoTest();
          //................ si liste de dossiers retenus à remplir la renseigner ....................................
          if (checkBox_DossiersRetenus->isChecked() && ret_metaFiltre /*&& lastGUID != curGUID */)
             {new QListViewItem ( ListView_DossiersRetenus,
                                  lastNom,
                                  lastPrenom,
                                  last_id_pk,
                                  lastGUID
                                );
              }
          //...................... sortir les resultats dans la listview ............................
          if ( ret_metaFiltre  && G_pCApp->m_ResultEnabled )
             {  QHeader       *pQHeader        = listView_Result_Fields->header();
                int            nb_section      = pQHeader->count();
                QMap <int, MAP_METAFIELD_VALUE >::Iterator it;
                for ( it = map_metaname_records.begin(); it != map_metaname_records.end(); ++it ) 
                    { if ( metaFiltre.recordTestState( it.key()) == false) continue;

                      QListViewItem *pQListViewItem  = new QListViewItem(listView_Result_Fields);
                      QMap <QString,QString> field_name_value  = it.data();
                      for ( int section   = 0; section < nb_section; ++section)
                          { //qDebug(tr("cur_section = %1").arg(QString::number(section)));
                            QString field_name = pQHeader->label ( section ); // le nom de la section est le nom du champ ou metachamp
                            QChar   field_typ  = field_name[0];
                            field_name         = field_name.mid(2);
                            if ( field_typ =="*" )    // METACHAMP
                               { QString value = field_name_value[field_name];
                                 pQListViewItem->setText( section, value) ;
                               }
                            else
                               { QString value = last_map_fieldName_value[ field_name ];
                                 pQListViewItem->setText(section, value) ;
                               }
                          } //for ( int section   = 0; section < nb_section; ++section)
                    } //for ( it = map_metaname_records.begin(); it != map_metaname_records.end(); ++it ) 
             } //if ( ret_metaFiltre  && pushButton_Results_Add_Field->isVisible() )

          //................ si le metafiltre autorise à traiter ......................................................
          //                 actionner les denombrements pour compter
          if (ret_metaFiltre && G_pCApp->m_DenombreEnabled)
             {for ( pCDenombre   = m_CDenombreList.first(); pCDenombre; pCDenombre = m_CDenombreList.next() )
                  { pCDenombre->DoResult(last_map_fieldName_value);
                  }
             }
         }// endif (currentID.length())
    } //endif ( query.isActive() )


 //.................................. sortir les resultats ......................................
 //                                   des denombrements
 textEdit_SelectDossiers->clear();
 for ( pCDenombre   = m_CDenombreList.first(); pCDenombre; pCDenombre = m_CDenombreList.next() )
     {texte = pCDenombre->OutResult();
      textEdit_SelectDossiers->append ( texte );
     }
 m_pCMoteurBase->m_DataBase->close();
}
///////////////////////////////////// MISE EN FORME RESULTATS /////////////////////////////////////////////////////////////
//----------------------------------------- resultHeaderSerialize ---------------------------------------------------------
QString Wdg_Central::resultHeaderSerialize(const QString &in_ofset)
{QString    result = "";
 QString     ofset = in_ofset;
             ofset = ofset.prepend("\n");
 QHeader *pQHeader = listView_Result_Fields->header();
      result    += ofset + "<ResHeader_List>";
      result    += ofset + "  <ResHeader_List_count>" + QString::number( listView_Result_Fields->columns() ) +  "</ResHeader_List_count>";
for ( int column = 0; column < listView_Result_Fields->columns(); ++column)
       { 
         QString name = listView_Result_Fields->columnText ( column );
         result      += ofset + "  <ResHeader_List_Label>" + name + "</ResHeader_List_Label>";
       } 
      result    += ofset + "</ResHeader_List>";
 return result;
} 

//----------------------------------------- resultHeaderUnSerialize ---------------------------------------------------------
int Wdg_Central::resultHeaderUnSerialize(const QString &data, int pos_bloc)
{          m_LastError = "";
 QString        xmlTag = "";
 QString dataToExamine = "";
 QString   strToGet    = "";
 int       pos         = 0;
 xmlTag = "<ResHeader_List>";
 if ( (pos_bloc = GetXMLValue(data, xmlTag,     dataToExamine, pos_bloc))  ==-1 )             // tenter avec curseur
    { if ( (pos_bloc = GetXMLValue(data, xmlTag, dataToExamine, 0 )) ==-1 ) {m_LastError = " ERREUR resultHeaderUnSerialize() : donnée \"" +xmlTag + "\" non retrouvée.";  return -1;}     // si echec tenter du d�ut
    }
 xmlTag = "<ResHeader_List_count>";
 if ( (pos = GetXMLValue(dataToExamine, xmlTag, strToGet, pos))==-1 ) {m_LastError = " ERREUR resultHeaderUnSerialize() : donnée \"" +xmlTag + "\" non retrouvée.";  return -1;}     // si echec tenter du d�ut
 int nb_item  = strToGet.toInt();
 if ( nb_item <= 0 ) return pos_bloc;
 
 //.............. effacer le header ....................
 while ( listView_Result_Fields->columns()>0 )
       { listView_Result_Fields->removeColumn (0);
       }
 //.............. renseigner le header ....................
 for (int nb=0; nb<nb_item; ++nb)
     {QString xmlDef_Label;
      if ( (pos = GetXMLValue(dataToExamine, "<ResHeader_List_Label>", xmlDef_Label, pos))==-1 ) {m_LastError = " ERREUR resultHeaderUnSerialize() : donnée \"<ResHeader_List_Label>\" non retrouvée.";  return -1;}     // si echec tenter du d�ut
      listView_Result_Fields->addColumn (xmlDef_Label);
     }
 return pos_bloc;
}
//--------------------------------------- Slot_pushButton_Result_MaskList_Clicked ------------------------------------------
void Wdg_Central::Slot_pushButton_Result_MaskList_Clicked()
{if (pushButton_Results_Add_Field->isVisible())
    {pushButton_Results_Add_Field->hide();
     pushButton_Results_Del_Field->hide();
     pushButton_Results_Export->hide();
     listView_Result_Fields->hide();
     pushButton_Results_Config->hide();
     G_pCApp->m_OptionExportRun->setEnabled ( false );
     pushButton_Result_MaskList->setPixmap ( QPixmap(G_pCApp->m_PathAppli + "Images/LeftLitle.png"  ));
     G_pCApp->m_ResultEnabled   = false;
    }
 else
    {pushButton_Results_Add_Field->show();
     pushButton_Results_Del_Field->show();
     pushButton_Results_Export->show();
     listView_Result_Fields->show();
     pushButton_Results_Config->show();
     G_pCApp->m_OptionExportRun->setEnabled ( true );
     pushButton_Result_MaskList->setPixmap ( QPixmap(G_pCApp->m_PathAppli + "Images/DownLitle.png"  ));
     G_pCApp->m_ResultEnabled   = true;
    }
}

//--------------------------------------- Slot_pushButton_Results_Add_Field_Clicked ------------------------------------------
void Wdg_Central::Slot_pushButton_Results_Add_Field_Clicked()
{   
 QStringList popItem;
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_INDEX_NOM);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_INDEX_PRENOM);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_INDEX_GUID);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_INDEX_PRIM_KEY);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_NSS);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_SEX);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_JFNOM);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_NB_ENF);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_RUE);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_CODE_POST);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_VILLE);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_CEDEX);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_PAYS);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TEL1);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TEL2);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TEL3);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TEL1_TYP);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TEL2_TYP);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TEL3_TYP);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_EMAIL);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_NUMSS);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_IS_ASS);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_NOM_ASS);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_PRENOM_ASS);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_PROF);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TITRE);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_RUB_HEAD_NOM_DATE);        // Court texte descriptif de l'enregistrement.
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_RUB_HEAD_DATE);   // date de l'enregistrement
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_RUB_HEAD_FIN);  // duree de fin de l'enregistrement
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_RUB_HEAD_TYPE);   // Type de rubrique
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_RUB_HEAD_USER);   // utilisateur associé à rubrique
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_RUB_HEAD_SIGN_USER);       // utilisateur associé signataire de la rubrique

 QListViewItemIterator it( listView_Def_MetaFields );
 while ( it.current() )
    { QListViewItem *pQListViewItem = it.current();
      popItem.append( tr( "* ") + pQListViewItem->text(0) );
      ++it;
    }
QString popRet       = DoPopupList(popItem);
if ( popRet.length())
   { listView_Result_Fields->addColumn ( popRet );
   }
}

//--------------------------------------- Slot_pushButton_Results_Del_Field_Clicked ------------------------------------------
void Wdg_Central::Slot_pushButton_Results_Del_Field_Clicked()
{ QHeader       *pQHeader        = listView_Result_Fields->header();
  int            nb_section      = pQHeader->count();
  int               section      = 0;
  QStringList popItem;
  for ( section   = 0; section < nb_section; ++section)
      { QString field_name = pQHeader->label ( section ); // le nom de la section est le nom du champ ou metachamp
        popItem.append( field_name );
      }
  QString popRet       = DoPopupList(popItem);
  if ( popRet.length())
     { section =  popItem.findIndex ( popRet );
       if ( section== -1) return;
       listView_Result_Fields->removeColumn ( section );
     }
}

//--------------------------------------- Slot_listView_Result_Fields_DoubleClicked ------------------------------------------
void Wdg_Central::Slot_listView_Result_Fields_DoubleClicked(QListViewItem* pQListViewItem)
{
}

//--------------------------------------- Slot_ConfigExport --------------------------------------------------------------
void Wdg_Central::Slot_ConfigExport()
{ CDlg_ExportConfig* dlg = new CDlg_ExportConfig( this, "CDlg_ExportConfig" );
  if (dlg==0)            return;
  if (dlg->exec()== QDialog::Accepted )
    { Slot_ResultExport();
    }
  delete dlg;
}
//----------------------------------------- Slot_ResultExport -----------------------------------------------------
void Wdg_Central::Slot_ResultExport()
{        if ( listView_Result_Fields->childCount() == 0 )
            { int ret = QMessageBox::information( this, tr("Liste des résultats vide"),
                                        tr ( "La Liste des résultats est vide.<br>"
                                             "Lancer les statistiques ?"
                                           ) ,
                                             tr("&Lancer"), tr("&Quitter"), 0,
                                             1, 1 );
             if (ret==1) return;
             EPP_Do();
            }
         bool                     ok   = false;
         QString                  file = G_pCApp->m_ExportFileName;
         if ( file.length()==0 )  file = G_pCApp->m_PathAppli + "Resultats/export_data.txt";
         if ( QDir(file).isRelative() ) file.prepend(G_pCApp->m_PathAppli);
         /*
         QFileDialog *fd = new QFileDialog ( QFileInfo(file).dirPath ( true ),
                                             tr( " Fichiers de texte (*.TXT *.txt *.xml *.XML *csv *CSV);;"
                                                 " Tous les fichiers (*)"),
                                             this,
                                             "SaveDlg"
                                           ) ;
         if (fd==0)                                             return;
         fd->setMode(QFileDialog::AnyFile );    // selection multiple
         fd->setSelection ( QFileInfo(file).fileName () );
         if ( fd->exec() == QDialog::Accepted )
            { QString fn = fd->selectedFile();
              if ( !fn.isEmpty() )
                 {  file = CGestIni::AbsoluteToRelativePath(G_pCApp->m_PathAppli, fn);
                    if ( file.startsWith("/") )file = file.mid(1);
                    G_pCApp->m_ExportFileName = file;
                    CGestIni::Param_WriteParam( &G_pCApp->m_LocalParam, "Export config", "File Name",    G_pCApp->m_ExportFileName);
                    CGestIni::Param_UpdateToDisk(G_pCApp->m_PathIni, G_pCApp->m_LocalParam);
                    ok = true;
                 }
            }
         delete fd;
         if ( ok==false ) return;
         */
         QString result  = "";
         QString line    = G_pCApp->m_ExportDebLine;
         //.............. si existe deja confirmer ......................
         if ( QFile::exists(file) )
            { int ret = QMessageBox::information( this, tr("Fichier existant"),
                                        tr ( "Ce fichier '<b>%1</b>' existe déjà.<br>"
                                             "Ecraser le fichier existant ?"
                                           ).arg( QFileInfo(file).fileName () ) ,
                                             tr("&Ecraser"), tr("&Quitter"), 0,
                                             1, 1 );
             if (ret==1) return;
            } 
         //................ exporter le header ..........................................
         QHeader       *pQHeader        = listView_Result_Fields->header();
         int            nb_section      = pQHeader->count();
         for ( int section   = 0; section < nb_section; ++section)
             { QString field_name = pQHeader->label ( section ).mid(2); // le nom de la section est le nom du champ ou metachamp
               line  +=  G_pCApp->m_ExportDebField + field_name + G_pCApp->m_ExportEndField;
             }
         line   += QString(G_pCApp->m_ExportEndLine).replace("EOL","\r\n");
         result += line;
         //............... exporter le contenu de la listview ................
         QListViewItemIterator it( listView_Result_Fields );
         while ( it.current() ) 
               { line = G_pCApp->m_ExportDebLine;
                 QListViewItem *pQListViewItem = it.current();
                 for ( int section   = 0; section < nb_section; ++section)
                     { line  +=  G_pCApp->m_ExportDebField + pQListViewItem->text(section) + G_pCApp->m_ExportEndField;
                     }
                 line   += QString(G_pCApp->m_ExportEndLine).replace("EOL","\r\n");
                 result += line;
                 ++it;
               }
         CGestIni::Param_UpdateToDisk(file, result);
}

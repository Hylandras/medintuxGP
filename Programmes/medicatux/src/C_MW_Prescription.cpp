#include "C_MW_Prescription.h"
#include <QtGlobal>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QTreeWidgetItemIterator>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QLineEdit>
#include <QWebSettings>
#include <QDesktopServices>
#include <QPrintDialog>
#include <QPrinter>
#include <QVariant>
#include <QDate>
#include <QWebFrame>
#include <QList>
#include <QTimer>
#include <QNetworkDiskCache>
#include <QFileInfo>
#include <QWebElementCollection>
#include <QWebElement>
#include <QProgressBar>
#include <QLabel>
#include <QMessageBox>
#include <QSignalMapper>
#include <QTextDocument>
#include <QColorDialog>
#include <QProgressDialog>
#include <QScrollBar>
#include <QHeaderView>
#include <QFontDialog>
#include <QToolBox>
#include <QProgressBar>
#include <QStyledItemDelegate>
#include <QInputDialog>
#include <QFileDialog>
#include <QMoveEvent>
#include <QDockWidget>

#include "ui_C_MW_Prescription.h"
#include "../../MedinTuxTools-QT4/Theme/Theme.h"
#include "../../MedinTuxTools-QT4/Theme/ThemePopup.h"
#include "../../MedinTuxTools-QT4/C_Utils_Log.h"
#include "../../MedinTuxTools-QT4/C_Utils_Html.h"
#include "../../MedinTuxTools-QT4/C_Login/C_Dlg_Login.h"

#include "../../MedinTuxTools-QT4/C_DragQTreeWidget/C_DragQTreeWidget.h"
// #include "../../MedinTuxTools-QT4/medicabase/C_BaseMedica.h"
#include "../../MedinTuxTools-QT4/medicabase/C_BDM_TheriaquePlugin.h"
#include "../../MedinTuxTools-QT4/medicabase/C_BDM_GenericPlugin.h"
#include "../../MedinTuxTools-QT4/medicabase/C_BDM_GouvPlugin.h"
#include "../../MedinTuxTools-QT4/medicabase/C_BDM_DatasempPlugin.h"
#include "../../MedinTuxTools-QT4/medicabase/C_Frm_Prescription.h"
#include "../../MedinTuxTools-QT4/medicabase/C_PatientCtx.h"
#include "../../MedinTuxTools-QT4/medicabase/C_UserCtx.h"
#include "../../MedinTuxTools-QT4/medicabase/C_LevelItemDelegate.h"
#include "../../MedinTuxTools-QT4/medicabase/C_Dlg_NotesInfos.h"
#include "../../MedinTuxTools-QT4/medicabase/C_PopupDial_Info.h"
#include "../../MedinTuxTools-QT4/univers/C_DateTools.h"

#include "../../MedinTuxTools-QT4/C_PatientBase/C_PatientBase.h"
#include "CApp.h"

// #include <extensionsystem/pluginspec.h>
// typedef QList<ExtensionSystem::PluginSpec *> PluginSpecSet;

#define PREF_CONNEXION_SUFFIX "_Prefered_connexions"
#define PATIENT CApp::pCApp()->m_pC_PatientCtx
#define USER    CApp::pCApp()->m_pC_UserCtx
//================================ C_MW_Prescription =====================================================

//------------------------------------ C_MW_Prescription -------------------------------
C_MW_Prescription::C_MW_Prescription(QWidget *parent) :
    QMainWindow(parent),
    m_pGUI(new Ui::MainWindow)
{   //this->setAttribute(Qt::WA_DeleteOnClose);           // fait planter
#ifndef QT_5
    QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
#endif

    m_pGUI->setupUi(this);
    CApp::pCApp()->declareMainWindow(this);
    m_webView_Mono                = 0; // monographie
    m_webView_Biblio              = 0; // bibliographie
    m_pC_BDM_Api                  = 0;
    m_pC_PatientBase              = 0;
    m_pC_Frm_Prescription         = 0;
    m_Notifications               = 0;
    m_runMacroForAll              = 0;
    m_Apropos_Proc                = 0;
    m_pC_PopupDial_NotesInfos     = 0;
    m_pC_PopupDial_Info           = 0;
    m_drugListMax                 = qMax(200, CApp::pCApp()->readUniqueParam( "medicatux session", "drug list limit").toInt());
    m_lastExportPath              = CApp::pCApp()->pathAppli()+"Ressources/exportList.xml";
    m_lastSqlPath                 = CApp::pCApp()->pathAppli()+"Ressources/database.sql";
    m_lastMonographiePath         = CApp::pCApp()->pathAppli();
    m_isFirst_tab_Ident           = true;
    m_isFirst_tab_Preferences     = true;
    m_isToolPtoductsInitialised   = false;
    m_DrugListFilter_SignalMapper = 0;
    m_menuFichiers                = 0;


#ifdef Q_WS_MAC
    //setUnifiedTitleAndToolBarOnMac ( true );
#endif

    init_main_menu((CApp::pCApp()->readUniqueParam("medicatux session", "main menu")=="true"));
    QDockWidget *dock = 0;
    //............. le dock des notifications et alertes .........................
    dock = new QDockWidget(tr("Alertes et notifications"), this);
    dock->setObjectName("QDockWidget_Notifications");
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    m_Notifications = new QTextEdit(this);
    dock->setWidget(m_Notifications);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    m_pGUI->webView_Help->hide();
    m_pGUI->label_ListMedoc->setText("");
    m_pGUI->textEdit_changeLog->hide(); // cacher le texte d'information

    m_pGUI->pushButton_CustomListMenu->setIcon( Theme::getIcon("medicatux/menu_left.png") ) ;
    m_pGUI->pushButton_CustomListMenu->setFlat(true);
    m_pGUI->pushButton_menuFavoris->setIcon( Theme::getIcon("medicatux/menu_left.png") ) ;
    m_pGUI->pushButton_menuFavoris->setFlat(true);

    m_pGUI->toolButton_erase_All->setIcon( Theme::getIcon(CApp::pCApp()->applicationName()+"/delete_Icon.png") ) ;

    m_pGUI->toolBox_ListesProduits->setItemIcon ( "page_CustomList",  Theme::getIcon("medicatux/toolbox_custom.png") );
    m_pGUI->toolBox_ListesProduits->setItemIcon ( "page_Favoris",     Theme::getIcon("medicatux/toolbox_withposo.png") );
    m_pGUI->toolBox_ListesProduits->setItemIcon ( "page_Historique",  Theme::getIcon("medicatux/toolbox_historique.png") );

    //..........//////// navigateur internet ////////....................
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptCanOpenWindows,true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard,true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::AutoLoadImages, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalStorageEnabled, true);

    //................................... bandeau de boutons du module de prescription ..................................................
    connect( m_pGUI->toolButton_tout_Ald,    SIGNAL(clicked ( bool )),              this, SLOT(Slot_toolButton_tout_Ald_clicked ( bool )) );
    connect( m_pGUI->toolButton_non_Ald,     SIGNAL(clicked ( bool )),              this, SLOT(Slot_toolButton_non_Ald_clicked ( bool )) );
    connect( m_pGUI->toolButton_erase_All,   SIGNAL(clicked ( bool )),              this, SLOT(Slot_toolButton_erase_All_clicked ( bool )) );
    connect( m_pGUI->toolButton_Annuler,     SIGNAL(clicked ( bool )),              this, SLOT(Slot_toolButton_Annuler_clicked ( bool )) );
    connect( m_pGUI->toolButton_Ok,          SIGNAL(clicked ( bool )),              this, SLOT(Slot_toolButton_Ok_clicked ( bool )) );
    connect( m_pGUI->toolButton_Duration,    SIGNAL(clicked ( bool )),              this, SLOT(Slot_toolButton_Duration_clicked( bool )) );
    connect( m_pGUI->toolButton_dci,         SIGNAL(clicked ( bool )),              this, SLOT(Slot_toolButton_dci_clicked( bool )) );
    //................................... icone bouton DCI ...........................................................................
    // "dci-"+QString::number(nb)+".png";
    QString icone = CApp::pCApp()->readUniqueParam("medicatux session", "dci mode");
    if ( icone.length()==0 )
       {  icone         = "dci-3";       // DCI exclusif
          CApp::pCApp()->writeParam("medicatux session", "dci mode", icone.toLatin1());
       }
    m_dci_mode    =  icone.mid(4).toInt();
    m_pGUI->toolButton_dci->setIcon(Theme::getIcon("medicatux/"+icone+".png"));
    //................................... boutons de zoom du module de prescription ..................................................
    m_pGUI->toolButton_zoomUp->setIcon(Theme::getIcon("medicatux/viewmag+.png"));
    m_pGUI->toolButton_zoomDw->setIcon(Theme::getIcon("medicatux/viewmag-.png"));

    connect( m_pGUI->toolButton_zoomUp,       SIGNAL(released ( )),              this, SLOT(Slot_toolButton_zoomUpReleased (  )) );
    connect( m_pGUI->toolButton_zoomDw,       SIGNAL(released ( )),              this, SLOT(Slot_toolButton_zoomDwReleased (  )) );
    m_prescription_zoom_menu = new QMenu(this);
    m_prescription_zoom_menu->addAction(tr("1  - Mode r\303\251duit"));
    m_prescription_zoom_menu->addAction(tr("2  - Mode d\303\251ploy\303\251 "));
    m_prescription_zoom_menu->addSeparator();
    m_prescription_zoom_menu->addAction(tr("3  - Enregistrer la position courante comme mode r\303\251duit "));
    m_prescription_zoom_menu->addAction(tr("4  - Enregistrer la position courante comme mode d\303\251ploy\303\251 "));
    m_prescription_zoom_menu->addSeparator();
    m_prescription_zoom_menu->addAction(tr("5  - Choisir une couleur pour la zone non ALD"));
    m_prescription_zoom_menu->addAction(tr("6  - Choisir une couleur pour la zone ALD"));

    m_pGUI->toolButton_zoomDw->setMenu ( m_prescription_zoom_menu );
    m_pGUI->toolButton_zoomDw->setPopupMode ( QToolButton::MenuButtonPopup );
    connect( m_prescription_zoom_menu,   SIGNAL(triggered ( QAction* )),   this, SLOT(Slot_prescription_zoom_menu_triggered ( QAction* )) );

    //.................................. drag and drop .....................................................
    //                                   treeWidget_Produits doit etre promu en C_DragQTreeWidget avec designer
    m_pGUI->treeWidget_Produits->setMimeType("text/medintux_prd_drag");
    m_pGUI->treeWidget_Produits->setAlternatingRowColors ( true );
    connect( m_pGUI->treeWidget_Produits->header(),    SIGNAL(sortIndicatorChanged ( int , Qt::SortOrder  )),      this, SLOT(Slot_sortIndicator_Changed( int , Qt::SortOrder )) );


    m_pGUI->treeWidget_Favoris->setMimeType("text/medintux_prd_drag");
    m_pGUI->treeWidget_Favoris->setAlternatingRowColors ( true );
    m_pGUI->treeWidget_Favoris->setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_pGUI->treeWidget_CustomDrugsList->setMimeType("text/medintux_prd_drag");
    m_pGUI->treeWidget_CustomDrugsList->setAlternatingRowColors ( true );
    m_pGUI->treeWidget_CustomDrugsList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_pGUI->treeWidget_Accessoires->setMimeType("text/medintux_prd_drag");
    m_pGUI->treeWidget_Accessoires->setAlternatingRowColors ( true );

    m_pGUI->treeWidget_PatientDrugs->setMimeType("text/medintux_prd_drag");
    m_pGUI->treeWidget_PatientDrugs->setAlternatingRowColors ( true );
    m_pGUI->treeWidget_PatientDrugs->setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_pGUI->pushButton_debListPatient->hide();
    m_pGUI->pushButton_finListPatient->hide();

    connect( m_pGUI->lineEdit_Indications,    SIGNAL(textChanged ( const QString & )),                 this, SLOT(Slot_lineEdit_Indications_textChanged (const QString &)) );
    connect( m_pGUI->lineEdit_DrugsSearch,    SIGNAL(textChanged ( const QString & )),                 this, SLOT(Slot_lineEdit_DrugsSearch_textChanged (const QString &)) );
    connect( m_pGUI->lineEdit_DrugsSearch,    SIGNAL(Sign_keyPressEvent(QKeyEvent *, int &)),          this, SLOT(Slot_lineEditAutoLocator_keyPressEvent(QKeyEvent *, int &)) );

    connect( m_pGUI->treeWidget_Produits,        SIGNAL(Sign_giveMeDatas(QString &, QTreeWidgetItem* )), this, SLOT(Slot_giveDragtreeWidget_ProduitsItem_List_Data(QString &, QTreeWidgetItem*)));
    connect( m_pGUI->treeWidget_Favoris,         SIGNAL(Sign_giveMeDatas(QString &, QTreeWidgetItem* )), this, SLOT(Slot_giveDragtreeWidget_FavorisItem_List_Data(QString &, QTreeWidgetItem*)));
    connect( m_pGUI->treeWidget_CustomDrugsList, SIGNAL(Sign_giveMeDatas(QString &, QTreeWidgetItem* )), this, SLOT(Slot_giveDragtreeWidget_CustomDrugsListItem_List_Data(QString &, QTreeWidgetItem*)));
    connect( m_pGUI->treeWidget_Accessoires,     SIGNAL(Sign_giveMeDatas(QString &, QTreeWidgetItem* )), this, SLOT(Slot_giveDragtreeWidget_AccessoiresItem_List_Data(QString &, QTreeWidgetItem*)));
    connect( m_pGUI->treeWidget_PatientDrugs,    SIGNAL(Sign_giveMeDatas(QString &, QTreeWidgetItem* )), this, SLOT(Slot_giveDragtreeWidget_PatientDrugsItem_List_Data(QString &, QTreeWidgetItem*)));

    connect( m_pGUI->treeWidget_Produits,        SIGNAL(Sign_keyPressEvent(QKeyEvent *, int &, C_DragQTreeWidget*)), this, SLOT(Slot_keyPressEvent_TreeWidgetProducts(QKeyEvent *, int &, C_DragQTreeWidget*)));
    connect( m_pGUI->treeWidget_Favoris,         SIGNAL(Sign_keyPressEvent(QKeyEvent *, int &, C_DragQTreeWidget*)), this, SLOT(Slot_keyPressEvent_TreeWidgetProducts(QKeyEvent *, int &, C_DragQTreeWidget*)));
    connect( m_pGUI->treeWidget_CustomDrugsList, SIGNAL(Sign_keyPressEvent(QKeyEvent *, int &, C_DragQTreeWidget*)), this, SLOT(Slot_keyPressEvent_TreeWidgetProducts(QKeyEvent *, int &, C_DragQTreeWidget*)));
    connect( m_pGUI->treeWidget_Accessoires,     SIGNAL(Sign_keyPressEvent(QKeyEvent *, int &, C_DragQTreeWidget*)), this, SLOT(Slot_keyPressEvent_TreeWidgetProducts(QKeyEvent *, int &, C_DragQTreeWidget*)));
    connect( m_pGUI->treeWidget_PatientDrugs,    SIGNAL(Sign_keyPressEvent(QKeyEvent *, int &, C_DragQTreeWidget*)), this, SLOT(Slot_keyPressEvent_TreeWidgetProducts(QKeyEvent *, int &, C_DragQTreeWidget*)));

    connect( m_pGUI->treeWidget_PatientDrugs,     SIGNAL(itemClicked ( QTreeWidgetItem * , int  )),        this, SLOT(Slot_treeWidget_PatientDrugs_itemClicked ( QTreeWidgetItem * , int   )) );
    connect( m_pGUI->treeWidget_PatientDrugs,     SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int  )),  this, SLOT(Slot_treeWidget_PatientDrugs_itemDoubleClicked ( QTreeWidgetItem * , int   )) );
    // connect( m_pGUI->treeWidget_Produits,     SIGNAL(Sign_giveMeDatas(QString &, QTreeWidgetItem* )),  this, SLOT(Slot_giveDragtreeWidget_ProduitsItemData(QString &, QTreeWidgetItem*)));
    m_pGUI->treeWidget_PatientDrugs->setContextMenuPolicy(Qt::CustomContextMenu);
    //connect( m_pGUI->treeWidget_PatientDrugs,     SIGNAL(customContextMenuRequested(QPoint)),              this, SLOT( Slot_Widget_PatientDrugs_ContextMenuRequested(QPoint)));

    connect( m_pGUI->treeWidget_Produits,     SIGNAL(currentItemChanged ( QTreeWidgetItem * , QTreeWidgetItem * )),  this, SLOT(Slot_treeWidget_Produits_currentItemChanged ( QTreeWidgetItem * , QTreeWidgetItem *   )) );
    connect( m_pGUI->treeWidget_Produits,     SIGNAL(itemClicked ( QTreeWidgetItem * , int  )),                      this, SLOT(Slot_treeWidget_Produits_itemClicked ( QTreeWidgetItem * , int   )) );
    connect( m_pGUI->treeWidget_Produits,     SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int  )),                this, SLOT(Slot_treeWidget_Produits_itemDoubleClicked ( QTreeWidgetItem * , int   )) );
    // connect( m_pGUI->treeWidget_Produits,     SIGNAL(Sign_giveMeDatas(QString &, QTreeWidgetItem* )),  this, SLOT(Slot_giveDragtreeWidget_ProduitsItemData(QString &, QTreeWidgetItem*)));
    m_pGUI->treeWidget_Produits->setContextMenuPolicy(Qt::CustomContextMenu);
    connect( m_pGUI->treeWidget_Produits,     SIGNAL(customContextMenuRequested(QPoint)),                            this, SLOT(Slot_Widget_Produits_ContextMenuRequested(QPoint)));

    connect( m_pGUI->treeWidget_Favoris,     SIGNAL(itemClicked ( QTreeWidgetItem * , int  )),        this, SLOT(Slot_treeWidget_Favoris_itemClicked ( QTreeWidgetItem * , int   )) );
    connect( m_pGUI->treeWidget_Favoris,     SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int  )),  this, SLOT(Slot_treeWidget_Favoris_itemDoubleClicked ( QTreeWidgetItem * , int   )) );
    // connect( m_pGUI->treeWidget_Favoris,     SIGNAL(Sign_giveMeDatas(QString &, QTreeWidgetItem* )),  this, SLOT(Slot_giveDragtreeWidget_FavorisItemData(QString &, QTreeWidgetItem*)));
    m_pGUI->treeWidget_Favoris->setContextMenuPolicy(Qt::CustomContextMenu);
    connect( m_pGUI->treeWidget_Favoris,     SIGNAL(customContextMenuRequested(QPoint)),              this, SLOT(Slot_Widget_treeWidget_Favoris_ContextMenuRequested(QPoint)));
    connect( m_pGUI->lineEdit_FavorisSearch,      SIGNAL(textChanged ( const QString & )),        this,   SLOT(Slot_lineEdit_lineEdit_FavorisSearch_textChanged (const QString &)) );
    connect( m_pGUI->lineEdit_FavorisSearch,      SIGNAL(Sign_keyPressEvent(QKeyEvent *, int &)), this,   SLOT(Slot_lineEdit_lineEdit_FavorisSearch_keyPressEvent(QKeyEvent *, int &)) );

    connect( m_pGUI->treeWidget_CustomDrugsList,     SIGNAL(itemClicked ( QTreeWidgetItem * , int  )),        this, SLOT(Slot_treeWidget_CustomDrugsList_itemClicked ( QTreeWidgetItem * , int   )) );
    connect( m_pGUI->treeWidget_CustomDrugsList,     SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int  )),  this, SLOT(Slot_treeWidget_CustomDrugsList_itemDoubleClicked ( QTreeWidgetItem * , int   )) );
    // connect( m_pGUI->treeWidget_CustomDrugsList,     SIGNAL(Sign_giveMeDatas(QString &, QTreeWidgetItem* )),  this, SLOT(Slot_giveDragtreeWidget_CustomDrugsListItemData(QString &, QTreeWidgetItem*)));
    m_pGUI->treeWidget_CustomDrugsList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect( m_pGUI->treeWidget_CustomDrugsList,     SIGNAL(customContextMenuRequested(QPoint)),              this, SLOT(Slot_Widget_treeWidget_CustomDrugsList_ContextMenuRequested(QPoint)));

    connect( m_pGUI->treeWidget_Accessoires,          SIGNAL(itemClicked ( QTreeWidgetItem * , int  )),        this, SLOT(Slot_treeWidget_Accessoires_itemClicked ( QTreeWidgetItem * , int   )) );
    connect( m_pGUI->treeWidget_Accessoires,          SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int  )),  this, SLOT(Slot_treeWidget_Accessoires_itemDoubleClicked ( QTreeWidgetItem * , int   )) );
    // connect( m_pGUI->treeWidget_Accessoires,          SIGNAL(Sign_giveMeDatas(QString &, QTreeWidgetItem* )),  this, SLOT(Slot_giveDragtreeWidget_AccessoiresItemData(QString &, QTreeWidgetItem*)));
    m_pGUI->treeWidget_Accessoires->setContextMenuPolicy(Qt::CustomContextMenu);
    connect( m_pGUI->treeWidget_Accessoires,          SIGNAL(customContextMenuRequested(QPoint)),              this, SLOT(Slot_treeWidget_Accessoires_ContextMenuRequested(QPoint)));

    connect( m_pGUI->treeWidget_Indications, SIGNAL(itemClicked ( QTreeWidgetItem * , int  )),        this, SLOT(Slot_treeWidget_Indications_itemClicked ( QTreeWidgetItem * , int   )) );
    connect( m_pGUI->treeWidget_Composition, SIGNAL(itemClicked ( QTreeWidgetItem * , int  )),        this, SLOT(Slot_treeWidget_Composition_itemClicked ( QTreeWidgetItem * , int   )) );

    connect( m_pGUI->treeWidget_ATC,              SIGNAL(itemClicked ( QTreeWidgetItem * , int  )),   this,   SLOT(Slot_treeWidget_ATC_itemClicked ( QTreeWidgetItem * , int   )) );
    connect( m_pGUI->pushButton_menuFavoris,      SIGNAL(clicked ( bool )),                           this,   SLOT(Slot_pushButton_pushButton_CustomListMenu(bool)) );
    connect( m_pGUI->pushButton_CustomListMenu,   SIGNAL(clicked ( bool )),                           this,   SLOT(Slot_pushButton_pushButton_CustomListMenu(bool)) );
    connect( m_pGUI->comboBox_customList,         SIGNAL(activated ( const QString &)) ,              this,   SLOT(Slot_comboBox_customList_activated( const QString &)) );
    connect( m_pGUI->lineEdit_CustomSearch,       SIGNAL(textChanged ( const QString & )),            this,   SLOT(Slot_lineEdit_CustomSearch_textChanged (const QString &)) );
    connect( m_pGUI->lineEdit_CustomSearch,       SIGNAL(Sign_keyPressEvent(QKeyEvent *, int &)),     this,   SLOT(Slot_lineEdit_CustomSearch_keyPressEvent(QKeyEvent *, int &)) );

    connect( m_pGUI->lineEdit_AccessoiresSearch,      SIGNAL(textChanged ( const QString & )),        this,   SLOT(Slot_lineEdit_AccessoiresSearch_textChanged (const QString &)) );
    connect( m_pGUI->lineEdit_AccessoiresSearch,      SIGNAL(Sign_keyPressEvent(QKeyEvent *, int &)), this,   SLOT(Slot_lineEdit_AccessoiresSearch_keyPressEvent(QKeyEvent *, int &)) );
    connect( m_pGUI->comboBox_FamillesProduits,       SIGNAL(activated (int )) ,                      this,   SLOT(Slot_comboBox_FamillesProduits_activated( int )) );

    //............................... le systeme de gestion des onglets ...........................................................
    connect( m_pGUI->tab_MainTabWidget,             SIGNAL(currentChanged (int )) ,                 this,   SLOT(Slot_tabWidget_Main_currentChanged( int )) );
    QLabel *pTabCornerWidget = new QLabel(this);
    pTabCornerWidget->setPixmap (Theme::getIcon("medicatux/tabcorner.png") );  // WARNING c'est la largeur de l'icone (vide) qui determiner la largeur du widget container
    pTabCornerWidget->setFrameStyle ( QFrame::NoFrame );

    CMyColorButton *pCMyColorButton;
    pCMyColorButton     = new CMyColorButton(QApplication::palette ( this ).color(this->backgroundRole()), Theme::getIcon("medicatux/tabSet.png"), QRect(2,2,16,16), pTabCornerWidget);
    connect( pCMyColorButton,                    SIGNAL(clicked ( bool )),                       this,   SLOT(Slot_m_Button_TabCorner_clicked(bool)) );

    pCMyColorButton     = new CMyColorButton(QApplication::palette ( this ).color(this->backgroundRole()), Theme::getIcon("medicatux/tabDockMenu.png"), QRect(20,2,16,16), pTabCornerWidget);
    connect( pCMyColorButton,                    SIGNAL(released ()),                            this,   SLOT(Slot_DockMenu()) );

    pCMyColorButton     = new CMyColorButton(QApplication::palette ( this ).color(this->backgroundRole()), Theme::getIcon("medicatux/tabInf.png"), QRect(38,2,16,16), pTabCornerWidget);
    connect( pCMyColorButton,                    SIGNAL(released ()),                            this,   SLOT(Slot_m_action_APropos()) );

    m_pGUI->tab_MainTabWidget->setCornerWidget ( pTabCornerWidget);

    //............................... le systeme des listes de classification ...........................................................
    connect( m_pGUI->toolBox_classifications,         SIGNAL(currentChanged ( int )),                 this,   SLOT(Slot_toolBox_classifications_currentChanged(int)) );
    connect( m_pGUI->toolBox_ListesProduits,          SIGNAL(currentChanged ( int )),                 this,   SLOT(Slot_toolBox_ListesProduits_currentChanged(int)) );

    //.................. completer statusBar avec progress et label ......................
    m_pQProgressBar  = new QProgressBar(this);
    m_pQLabel        = new QLabel(this);
    m_pQProgressBar->setMaximumWidth(200);
    statusBar()->addWidget(m_pQProgressBar);
    statusBar()->addWidget(m_pQLabel);
    m_pQLabel->setText("");

    //.............. on cree le controleur de l'API medicamenteuse ....................................................
    m_pC_BDM_Api = new C_BDM_Api( CGestIni::Param_UpdateFromDisk(CApp::pCApp()->pathAppli()+"C_BDM_Api.cfg"),
                                  "MAIN_",
                                  CApp::pCApp()->iniParam(),
                                  CApp::pCApp()->pathTheme() + CApp::pCApp()->applicationName()+"/",
                                  CApp::pCApp()->pathAppli(),
                                  CApp::pCApp()->pathIni(),
                                  this);
    m_pC_BDM_Api->setLogWidget(m_pGUI->textEdit_Monitor);
    m_pC_BDM_Api->setProgressWidget(m_pQProgressBar);
    m_pC_BDM_Api->setStatusWidget(m_pQLabel);

    //............................ recuperer et creer le contexte patient ................................
    PATIENT = new C_PatientCtx(CGestIni::Param_UpdateFromDisk(CApp::pCApp()->patientCtxFileName()), //"%{buildDir}/Ressources/ComFiles/30DC82DD-7632-2C46-9DC3-BC1AC0ECD20A-C_PatientCtx.xml"),  // le fichier des donnees
                                         CGestIni::Param_UpdateFromDisk(CApp::pCApp()->pathAppli()+"/Ressources/ComFiles/casalta.NABM"),                                           // le contenu du fichier de conversion labo
                                         CApp::pCApp()->pathAppli()+"/Ressources/ComFiles/casalta.NABM");                                                                          // le non du fichier de conversion labo
    //............................ recuperer et creer le contexte utilisateur (prescripteur) ................................
    USER    = new C_UserCtx( CGestIni::Param_UpdateFromDisk( CApp::pCApp()->userCtxFileName())); //CApp::pCApp()->pathAppli() +"Ressources/ComFiles/UserCtx.xml") ); //CApp::pCApp()->userCtxFileName()));

    // ........................... brancher le controleur de l'API sur le contexte patient et utilisateur........................................
    if (m_pC_BDM_Api)
       { m_pC_BDM_Api->setPatientContexte(PATIENT);
         m_pC_BDM_Api->setUserContexte(USER);
       }
    //............................. brancher le controleur de l'API medicamenteuse sur le plugin de base de donnee .................
    QString namePlugin = CApp::pCApp()->readUniqueParam("medicatux data source", "name");
    set_drugFilterInterfaceFromIniFile();  // le faire avant remplissage des medicaments
    set_BDM_Plugin(namePlugin, 0);  // entraine un remplissage de la liste des medicaments

    //............................ creer la feuille d'elaboration des ordonnances ................................
    QFont fnt = setFontFromIniFile();
    add_C_Frm_Prescription( fnt, getInputGrammarList() );  // a faire absolument apres initialisation m_pC_BDM_PluginI et  m_pC_BDM_Api
    display_HAS_Status();                                  // a faire apres initialisation de m_pC_Frm_Prescription (faite dans add_C_Frm_Prescription)
    //............ titre interface et liste ........................
    setWindowTitle();       // le faire apres le contexte patient

    //................ placer le gestionnaire de cookie jar .............................
    m_pCookieJar = new CookieJar(CApp::pCApp()->pathAppli()+"/Ressources/cookies.txt", this, m_pGUI->textEdit_Monitor);
    CApp::pCApp()->setQProgressBar(m_pQProgressBar);        // positionne m_pC_Utils_Html
    CApp::pCApp()->setLogWidget(m_pGUI->textEdit_Monitor);  // positionne C_ro_Core::setLogWidget()   dont est derivee C_Macro dont est co-derivee CApp

    setPositionsFromIniFile();
    //......... on active les fenetres avec QTimer (ubuntu unity oblige) .........
    QTimer::singleShot(5, this, SLOT(Slot_ActivateInfoWindow())); // pour avoir la fenetre en avant plan sur ubuntu unity
    CApp::pCApp()->outMessage(" m_lastArg : " + CApp::pCApp()->m_lastArg);

}

//--------------------------------------- Slot_actionConfig_triggered ---------------------------------------------------
void C_MW_Prescription::Slot_actionConfig_triggered( bool )
{  m_pGUI->tab_MainTabWidget->activeTabWidgetByObjectName( "tab_Monitor");
   QWidget *pTab_QWidget = m_pGUI->tab_MainTabWidget->tabWidgetByTabObjectName("tab_Monitor");
   if (pTab_QWidget) m_pGUI->tab_MainTabWidget->setCurrentWidget(pTab_QWidget);
}

//--------------------------------------- Slot_lineEdit_Nom_textChanged ---------------------------------------------------
void C_MW_Prescription::Slot_lineEdit_Nom_textChanged (const QString &txt)
{   if ( PATIENT==0 && txt.length()==0 )                  return;
    PATIENT->set_usual_name(txt);
    setWindowTitle();
}
//--------------------------------------- Slot_lineEdit_Prenom_textChanged ---------------------------------------------------
void C_MW_Prescription::Slot_lineEdit_Prenom_textChanged (const QString &txt)
{   if ( PATIENT==0 && txt.length()==0 )                  return;
    PATIENT->set_forename(txt);
    setWindowTitle();
}
//--------------------------------------- Slot_lineEdit_taille_editingFinished---------------------------------------------------
void C_MW_Prescription::Slot_lineEdit_taille_editingFinished ()
{   QString txt = m_pGUI->lineEdit_taille->text();
    if ( PATIENT==0 && txt.length()==0 )                  return;
    disconnect( m_pGUI->lineEdit_taille,          SIGNAL(editingFinished ()),    this,   SLOT(Slot_lineEdit_taille_editingFinished ()) );
    PATIENT->set_taille(txt);
    double surface = C_PatientCtx::calc_surfaceMosteller(PATIENT->tailleToDouble(), PATIENT->poidsToDouble());
    PATIENT->set_surface(QString::number(surface,'f',2));
    emit Sign_PatientCtxChanged(PATIENT);
    m_pGUI->lineEdit_taille->setText(PATIENT->taille());
    connect( m_pGUI->lineEdit_taille,             SIGNAL(editingFinished ()),    this,   SLOT(Slot_lineEdit_taille_editingFinished ()) );

    //if (m_pC_Frm_Prescription) m_pC_Frm_Prescription->Slot_evaluateAllInteractions();
}
//--------------------------------------- Slot_lineEdit_poids_textChanged ---------------------------------------------------
void C_MW_Prescription::Slot_lineEdit_poids_textChanged (const QString &txt)
{   if ( PATIENT==0 && txt.length()==0 ) return;
    PATIENT->set_poids(txt);
    double surface = C_PatientCtx::calc_surfaceMosteller(PATIENT->tailleToDouble(), PATIENT->poidsToDouble());
    PATIENT->set_surface(QString::number(surface,'f',2));
    emit Sign_PatientCtxChanged(PATIENT);
    //if (m_pC_Frm_Prescription) m_pC_Frm_Prescription->Slot_evaluateAllInteractions();
    setWindowTitle();
}
//--------------------------------------- Slot_lineEdit_sexe_textChanged ---------------------------------------------------
void C_MW_Prescription::Slot_lineEdit_sexe_textChanged (const QString &txt)
{   if ( PATIENT==0)                                           return;
    if (txt.length()) PATIENT->set_sexe(txt.at(0).toUpper());
    else              PATIENT->set_sexe("");
    // emit Sign_PatientCtxChanged(m_pC_PatientCtx);
    if (m_pC_Frm_Prescription) m_pC_Frm_Prescription->Slot_evaluateAllInteractions();
}
//--------------------------------------- Slot_lineEdit_DateNaissance_editingFinished ---------------------------------------------------
void C_MW_Prescription::Slot_lineEdit_DateNaissance_editingFinished ()
{   QString txt = m_pGUI->lineEdit_DateNaissance->text();
    if ( PATIENT==0 && txt.length()==0 )               return;
    disconnect( m_pGUI->lineEdit_DateNaissance,      SIGNAL(editingFinished()),        this,   SLOT(Slot_lineEdit_DateNaissance_editingFinished()) );
    QDate dt = C_DateTools::dateOfBirthControl(txt);
    if ( ! dt.isValid() )
       { m_pGUI->lineEdit_DateNaissance->setText("");
         return;
       }
    m_pGUI->lineEdit_DateNaissance->setText(dt.toString("dd-MM-yyyy"));
    PATIENT->set_ageOrDateOfBirth (dt);
    // emit Sign_PatientCtxChanged(m_pC_PatientCtx);
    if (m_pC_Frm_Prescription) m_pC_Frm_Prescription->Slot_evaluateAllInteractions();
    setWindowTitle();
    connect( m_pGUI->lineEdit_DateNaissance,      SIGNAL(editingFinished ()),        this,   SLOT(Slot_lineEdit_DateNaissance_editingFinished()) );
}

//------------------------ Slot_FactCorpoChanged ---------------------------------------
void C_MW_Prescription::Slot_FactCorpoChanged(C_PatientCtx *pC_PatientCtx)
{   if ( PATIENT==0 && pC_PatientCtx==0 ) return;
    disconnect( m_pGUI->lineEdit_taille,             SIGNAL(editingFinished ()),               this,   SLOT(Slot_lineEdit_taille_editingFinished ()) );
    disconnect( m_pGUI->lineEdit_poids,              SIGNAL(textChanged ( const QString & )),  this,   SLOT(Slot_lineEdit_poids_textChanged (const QString &)) );
    PATIENT->set_poids(pC_PatientCtx->poids());
    PATIENT->set_taille(pC_PatientCtx->taille());
    PATIENT->set_surface(pC_PatientCtx->surface());
    m_pGUI->lineEdit_poids->setText(pC_PatientCtx->poids());
    m_pGUI->lineEdit_taille->setText(pC_PatientCtx->taille());
    connect( m_pGUI->lineEdit_taille,                SIGNAL(editingFinished ()),               this,   SLOT(Slot_lineEdit_taille_editingFinished ()) );
    connect( m_pGUI->lineEdit_poids,                 SIGNAL(textChanged ( const QString & )),  this,   SLOT(Slot_lineEdit_poids_textChanged (const QString &)) );
}

//--------------------------------------- Slot_ActivateMainWindow ---------------------------------------------------
void C_MW_Prescription::Slot_ActivateMainWindow()
{   //............ ne surtout pas changer les sequences ....................
    //             suivantes (seules a mettre les fenetres en avant plan sur Mac)
    if ( CApp::pCApp()->m_lastArg.startsWith("shared_memory_id=") )
       { //setWindowFlags( windowFlags()| Qt::WindowStaysOnTopHint);
         setWindowState( windowState()  & ~Qt::WindowMinimized );
       }
    else
       {
         setWindowState( windowState() & ~Qt::WindowMinimized ) ;
       }
    show();
    activateWindow();
    raise();
}
//------------------------ raise -----------------------------------------
/*! \brief set windows in front
 */
void C_MW_Prescription::raise()
{ activateWindow();
  QMainWindow::raise();
}
//--------------------------------------- Slot_ActivateMainWindow ---------------------------------------------------
void C_MW_Prescription::Slot_ActivateInfoWindow()
{
 QTimer::singleShot(5, this, SLOT(Slot_ActivateMainWindow())); // pour avoir la fenetre en avant plan sur ubuntu unity
}
//--------------------------------------- Slot_toolButton_dci_clicked ---------------------------------------------------
void C_MW_Prescription::Slot_toolButton_dci_clicked( bool )
{     QStringList optionList;
      optionList << "=0=#medicatux/dci-0.png#"   + tr("prescrire en mode commercial exclusivement");
      optionList << "=1=#medicatux/dci-1.png#"   + tr("prescrire en mode commercial puis DCI");
      optionList << "=3=#medicatux/dci-3.png#"   + tr("prescrire en mode DCI exclusivement");
      optionList << "=2=#medicatux/dci-2.png#"   + tr("prescrire en mode DCI puis commercial");
      QString option = ThemePopup(optionList,this, " border: 1px solid #8f8f91; border-radius: 6px; font-size: 11px;").DoPopupList();
      if (option.length()==0) return;

      Slot_setDCI_Mode(option.toInt());
      option.prepend("dci-");
      CApp::pCApp()->writeParam("medicatux session", "dci mode", option.toLatin1());
      m_pGUI->toolButton_dci->setIcon(Theme::getIcon("medicatux/"+option+".png"));
}

//--------------------------------------- Slot_setDCI_Mode ---------------------------------------------------
//    enum dci_flag  {COM  = 0, COM_DCI = 1, DCI_COM = 2, DCI = 3 };
void C_MW_Prescription::Slot_setDCI_Mode(int mode)
{ m_dci_mode = mode;
  if ( m_pC_Frm_Prescription ==0 ) return;
  m_pC_Frm_Prescription->setDCI_Mode( mode);
}

//--------------------------------- getInputGrammarList -----------------------------------------------------------------------
QStringList C_MW_Prescription::getInputGrammarList()
{ QStringList grammarList;
  QString grammarInputData = CGestIni::Param_UpdateFromDisk( CApp::pCApp()->grammarFileName() );
  if (grammarInputData.length()==0) return grammarList;
  grammarList = CGestIni::getXmlDataList("OrdoLine", grammarInputData);
  return grammarList;
}
//--------------------------------- Slot_toolButton_tout_Ald_clicked -----------------------------------------------------------------------
void C_MW_Prescription::Slot_toolButton_tout_Ald_clicked ( bool )
{
 if (m_pC_Frm_Prescription==0) return;
     m_pC_Frm_Prescription->set_Ald_On_All_Products("ALD");
}
//--------------------------------- Slot_toolButton_non_Ald_clicked -----------------------------------------------------------------------
void C_MW_Prescription::Slot_toolButton_non_Ald_clicked ( bool )
{if (m_pC_Frm_Prescription==0) return;
    m_pC_Frm_Prescription->set_Ald_On_All_Products("");

}
//--------------------------------- Slot_toolButton_erase_All_clicked -----------------------------------------------------------------------
void C_MW_Prescription::Slot_toolButton_erase_All_clicked ( bool )
{   if (m_pC_Frm_Prescription==0) return;
    m_pC_Frm_Prescription->delete_All_Products();
}
//--------------------------------- Slot_toolButton_Annuler_clicked -----------------------------------------------------------------------
void C_MW_Prescription::Slot_toolButton_Annuler_clicked ( bool )
{   close();
}
//--------------------------------- Slot_toolButton_Ok_clicked -----------------------------------------------------------------------
void C_MW_Prescription::Slot_toolButton_Ok_clicked ( bool )
{    //........... enregistrer les ordo ..................
    QString mask         = CGestIni::Param_UpdateFromDisk(CApp::pCApp()->pathAppli()+"/Ressources/OrdoMask/OrdoMask.xml");
    ordoToHtml(mask);
    close();
}
//--------------------------------- Slot_toolButton_Duration_clicked -----------------------------------------------------------------------
void C_MW_Prescription::Slot_toolButton_Duration_clicked( bool )
{   if (m_pC_Frm_Prescription==0) return;
    QMenu       *pQMenu       = new QMenu(this);  pQMenu->setFont(this->font());
    QString      options_text = CGestIni::Param_UpdateFromDisk( CApp::pCApp()->pathAppli()+"/Ressources/DurationPopup.txt" ).remove('\r');
    QStringList  options_list = options_text.split('\n', QString::SkipEmptyParts);
    if ( options_list.size()==0 ) return;
    for (int i=0; i< options_list.size(); ++i)
        { ThemePopup::addOptionMenu( pQMenu, options_list.at(i) );
        }
    QAction *QAction = pQMenu->exec(this->cursor().pos());
    if (QAction)
       { m_pC_Frm_Prescription->set_Duration_On_All_Products( QAction->data().toString() );
       }
}

//--------------------------------- Slot_toolBox_ListesProduits_currentChanged -----------------------------------------------------------------------
void C_MW_Prescription::Slot_toolBox_ListesProduits_currentChanged(int index)
{ QWidget *pQWidget          = m_pGUI->toolBox_ListesProduits->widget ( index );
  set_toolBox_ListesProduits_item(pQWidget->objectName() , C_MW_Prescription::notBeSet);
  fill_only_showed_treeWidgets( pQWidget->objectName() );
}
//------------------------ fill_only_showed_treeWidgets ---------------------------------------
void C_MW_Prescription::fill_only_showed_treeWidgets(const QString &objName)
{//......... remplissage des listes ......................
    if (objName=="page_Produits")
       { initDrugListFilter();
         if (m_pGUI->treeWidget_Produits->topLevelItemCount()==0)             Slot_lineEdit_DrugsSearch_textChanged ("");
         m_pGUI->lineEdit_DrugsSearch->setFocus();
       }
    else if (objName=="page_CustomList")
       {  if ( m_pGUI->treeWidget_CustomDrugsList->topLevelItemCount()==0 )   fillCustomDrugListCombo("BDM");
          m_pGUI->lineEdit_CustomSearch->setFocus();
       }
    else if (objName=="page_Accessoires")
       { if ( m_pGUI->treeWidget_Accessoires->topLevelItemCount()==0 )        Datasemp_fill_treeWidget_Accessoires(m_pGUI->treeWidget_Accessoires, m_pGUI->comboBox_FamillesProduits->currentIndex()+2);
         m_pGUI->lineEdit_AccessoiresSearch->setFocus();
       }
    else if (objName=="page_Favoris")
       { if ( m_pGUI->treeWidget_Favoris->topLevelItemCount()==0)             fill_treeWidget_Favoris();
         m_pGUI->lineEdit_FavorisSearch->setFocus();
       }
    else if (objName=="page_Historique")
       { if ( m_pGUI->treeWidget_PatientDrugs->topLevelItemCount()==0)        fill_treeWidget_Historique();
         m_pGUI->lineEdit_HistoriqueSearch->setFocus();
       }
}
//--------------------------------- fill_treeWidget_Historique -----------------------------------------------------------------------
void C_MW_Prescription::fill_treeWidget_Historique()
{ if (PATIENT == 0)   return;
  //if (m_pC_BDM_Api    == 0 )  return;
  QList < C_BDM_DrugListRecord >  drugsList;
  for (int i=0; i< PATIENT->prescriptionsList().size(); ++i)
      { QString grammar = PATIENT->prescriptionsList().at(i);
        int nextPos              = 0;
        QString status           = CGestIni::getXmlData(     "status",    grammar, &nextPos);
        QString id               = CGestIni::getXmlData(     "gph_id",    grammar, &nextPos);
        QString id_type          = CGestIni::getXmlData(     "gph_it",    grammar, &nextPos);
        QString dt_fin           = CGestIni::getXmlData(     "gph_df",    grammar, &nextPos);
        //QString pk               = CGestIni::getXmlData(     "gph_pk",    grammar, &nextPos);
        //QString    s_dateRed     = CGestIni::getXmlData(     "gph_dr",    grammar, &nextPos).left(10);
        //QString    s_dateDeb     = CGestIni::getXmlData(     "gph_dt",    grammar, &nextPos).left(10);
        QString commercialName   = CGestIni::getXmlData(     "gph_na",    grammar, &nextPos);
        if (commercialName.length()==0 && id_type=="CIP")
           { commercialName =  m_pC_BDM_Api->CIP_to_DrugName(id);
            // QStringList C_BDM_Api::get_DCI_list_FromCIP(const QString &cip)
           }
        QString compoXmlParts    = CGestIni::getXmlData(     "gph_dcl",   grammar, &nextPos);
        QStringList dciList      = CGestIni::getXmlDataList( "gph_dc",    compoXmlParts);
        dciList <<"";
        dciList <<"";
        dciList <<"";
        drugsList.append( C_BDM_DrugListRecord( id,
                                                id_type,
                                                commercialName,
                                                CGestIni::getXmlData( "gph_dcn", dciList[0]),
                                                CGestIni::getXmlData( "gph_dcn", dciList[1]),
                                                CGestIni::getXmlData( "gph_dcn", dciList[2]),
                                                status.contains('F') ? "CTX_F" : "CTX_I",
                                                "DT="+dt_fin,                 // dans l'historique on se sert du champ langue pour placer la date de fin
                                                "ucd",
                                                "ucd_price",
                                                "",         //sera rempli a la volee lors d'un clic  m_pC_BDM_Api->ATC_from_CIP(id),
                                                "",
                                                "VH"
                                              )
                        );
      }
  fill_treeWidget_ProductsFromDrugList(m_pGUI->treeWidget_PatientDrugs, drugsList);
}

//--------------------------------- Slot_toolBox_classifications_currentChanged -----------------------------------------------------------------------
void C_MW_Prescription::Slot_toolBox_classifications_currentChanged(int index)
{ if (index==-1) return;
  QWidget *pQWidget  = m_pGUI->toolBox_classifications->widget ( index );
  if (pQWidget==0) return;
  QString  name      = pQWidget->objectName();
  QString id         = "";
  QTreeWidget  *pQTreeWidget = currrentProductListTreeWidget();
  if ( pQTreeWidget     == 0 ) return;
  QTreeWidgetItem *pQTreeWidgetItem = pQTreeWidget->currentItem();
  if ( name == "page_ATC" )
     { if ( pQTreeWidgetItem == 0 ) return;
       setListATC_onCode (pQTreeWidgetItem->text( COL_ATC ) );
     }
  else if ( name == "page_Indications" )
     { if ( pQTreeWidgetItem ) id  = pQTreeWidgetItem->text( COL_ID );
       fillTreeWidgetIndications( id );
     }
  else if ( name == "page_Composition" )
     { if ( pQTreeWidgetItem ) id  = pQTreeWidgetItem->text( COL_ID );
       fillTreeWidgetComposition( id );
     }
}

//--------------------------------- Slot_m_Button_TabCorner_clicked -----------------------------------------------------------------------
void C_MW_Prescription::Slot_m_Button_TabCorner_clicked(bool)
{   m_pGUI->tab_MainTabWidget->doPageChoiceMenu( this );
}

//--------------------------------- currrentProductListTreeWidget -----------------------------------------------------------------------
QTreeWidget  *C_MW_Prescription::currrentProductListTreeWidget()
{   QWidget      *pQWidget     = m_pGUI->toolBox_ListesProduits->currentWidget();
    if      (pQWidget->objectName()=="page_Produits")    return m_pGUI->treeWidget_Produits;
    else if (pQWidget->objectName()=="page_CustomList")  return m_pGUI->treeWidget_CustomDrugsList;
    else if (pQWidget->objectName()=="page_Accessoires") return m_pGUI->treeWidget_Accessoires;
    else if (pQWidget->objectName()=="page_Favoris")     return m_pGUI->treeWidget_Favoris;
    else if (pQWidget->objectName()=="page_Historique")  return m_pGUI->treeWidget_PatientDrugs;
    return 0;
}
//--------------------------------- ordoToHtml -----------------------------------------------------------------------
QString C_MW_Prescription::ordoToHtml( const QString &mask /* = "" */ )
{   QString html      = "";
    QString  xml      = "";
    if ( mask.length() )
       {int nextPos          = 0;
        QString deb          = CGestIni::getXmlData( "start_doc",               mask, &nextPos);
        QString is_ald_txt   = CGestIni::getXmlData( "is_ald_part",             mask, &nextPos);
        QString no_ald_txt   = CGestIni::getXmlData( "no_ald_part",             mask, &nextPos);
        QString lineMask     = CGestIni::getXmlData( "product_line",            mask, &nextPos);
        QString allerg_intro = CGestIni::getXmlData( "allergie_debut_bloc",     mask, &nextPos);
        QString allerg_Line  = CGestIni::getXmlData( "allergie_line",           mask, &nextPos);
        QString allerg_fin   = CGestIni::getXmlData( "allergie_fin_bloc",       mask, &nextPos);
        QString end          = CGestIni::getXmlData( "end_doc",                 mask, &nextPos);
        QString html         = deb;
        int mentionIsALD     = 0;
        int mentionNonALD    = 0;
        int i                = 0;
        QString allerg_bloc  = "";
        int allerg_nb        = PATIENT->allergiesList().size();
        //................. faire la liste des allergies .........................
        if ( allerg_nb )
           {  QStringList a_lst =  PATIENT->allergiesList();
              allerg_bloc       = allerg_intro;
              for (int i=0; i<allerg_nb; ++i)
                  { QString a_line          = allerg_Line;                 //   0       1     2         3      4      5     6
                    QStringList a_lst_items = a_lst[i].split('|');         // "3533524|CIP|AMOXICILLINE|  |Allergique|  |08-12-2014|"
                    a_line                  = a_line.replace( "{{allergie}}" , a_lst_items[2] );
                    allerg_bloc += a_line;
                  }
              allerg_bloc += allerg_fin;
           }
        //................. liste des medicaments .........................
        QStringList grammar_list = m_pC_Frm_Prescription->grammarList();
        for ( i=0; i<grammar_list.size(); ++i )
            {  QString grammar = grammar_list.at(i);
               if ( C_PosologieGrammar::ald( grammar ).length())
                  { if (mentionIsALD == 0)                  html += is_ald_txt;
                    ++mentionIsALD;
                  }
               else
                  { if (mentionIsALD && mentionNonALD==0 )  html += no_ald_txt;
                    ++mentionNonALD;
                  }
               xml  += "<OrdoLine>"+grammar+"</OrdoLine>\n\n";
               html += C_PosologieGrammar::fuse  ( grammar               ,
                                                   lineMask              ,
                                                   m_pC_Frm_Prescription->imagePath("NonSubstituable.png"),
                                                   QString::number(i+1)
                                                 );
            } //end for ( i=0; i<grammar_list.size(); ++i )
        html  += end;
        //........... mentions nombre de produit prescrits...........
        html   = html.replace("{{PRODUCT_COUNT}}",    QString::number(grammar_list.size()));   // PRODUCT_COUNT
        if ( grammar_list.size() )
            html   = html.replace("{{PRODUCT_NUMBER}}",   C_PosologieGrammar::numberToLiteral(grammar_list.size()) );
        else
            html   = html.replace("{{PRODUCT_NUMBER}}",   "aucun" );
        //........... mentions versions LAP et source de donnees ...........
        html   = html.replace("{{DATA_SRC}}",          m_pC_BDM_Api->dataSourceVersion());
        html   = html.replace("{{LAP_VERSION}}",       tr("%1 %2 %3").arg(CApp::pCApp()->ApplicationVersion(),__DATE__,__TIME__));
        //...........inserer liste des allergies ...........
        html   = html.replace("{{BLOC_ALLERGIE}}",     allerg_bloc);

        QString guid = "noPatient";
        if (PATIENT && PATIENT->get_guid().length()) guid = PATIENT->get_guid();
        if (CApp::pCApp()->return_mode()=="ORDO")  // genere une ordonnance prete a l'emploi dans documents MedinTux
           { QString ret_string = QString( "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\n"
                                           "<ordotext>\n%1"
                                           "</ordotext>\n"
                                           "<ordoMedicaStruct>\n"
                                           "<Posologie_LAP_List>\n\n%2"
                                           "</Posologie_LAP_List>\n"
                                           "</ordoMedicaStruct>\n>").arg(html,xml);
             CGestIni::Param_UpdateToDisk( QString ( CApp::pCApp()->pathAppli()+"/Ressources/ComFiles/Ordo_%1.ord" ).arg(guid), ret_string);
           }
        else
           { CGestIni::Param_UpdateToDisk( QString ( CApp::pCApp()->pathAppli()+"/Ressources/ComFiles/Ordo_%1_.html" ).arg(guid), html);
             CGestIni::Param_UpdateToDisk( QString ( CApp::pCApp()->pathAppli()+"/Ressources/ComFiles/Ordo_%1_.xml"  ).arg(guid),  xml);
           }
        return html;
       }

    QString     css  =   CGestIni::Param_UpdateFromDisk ( CApp::pCApp()->pathAppli()+"/Ressources/Ordonnance.css" );
    if (css.length()==0)
        { css   =
                       "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
                       "<html>"
                       "<head>"
                       "    <meta name=\"qrichtext\" content=\"1\" />"
                       "    <style type=\"text/css\">"
                       "        span.mainName {"
                       "         color:#000000;"                                    // 1 color dci
                       "         font-family: arial, verdana, sans-serif;"
                       "         text-decoration: underline;"
                       "         font-size: 10px;"
                       "         font-weight: bold;"
                       "        }"
                       "        span.optionalName {"
                       "          color:#000000;"                                   // 2 color commercial name
                       "          font-family: arial, verdana, sans-serif;"
                       "          font-style: italic;"
                       "          font-size: 10px;"
                       "        }"
                       "        span.texteNormal {"
                       "          color:#000000;"                                   // 3 color text normal
                       "          font-family: arial, verdana, sans-serif;"
                       "          font-size: 10px;"
                       "        }"
                       "        span.numOrdre {"
                       "          color:#000000;"                                   // 4 color num Ordre
                       "          font-family: arial, verdana, sans-serif;"
                       "          font-size: 10px;"
                       "          font-weight: bold;"
                       "        }"

                       "        span.drugForme {"
                       "          color:#000000;"                                   // 5 color forme
                       "          font-family: arial, verdana, sans-serif;"
                       "          font-size: 10px;"
                       "          font-weight: bold;"
                       "        }"
                       "    </style>"
                       "</head>";
        }  // endif (css.length()==0)
    //.................. une methode (on recupere toute la liste )............................
    int mentionIsALD  = 0;
    int mentionNonALD = 0;
    int i             = 0;
    QStringList grammar_list = m_pC_Frm_Prescription->grammarList("ALL");
    for ( i=0; i<grammar_list.size(); ++i )
        {  QString grammar = grammar_list.at(i);
           if ( C_PosologieGrammar::ald( grammar ).length())
              { if (mentionIsALD == 0)                  html += CGestIni::Param_UpdateFromDisk(CApp::pCApp()->pathAppli()+"/Ressources/MentionsALD/AvecALD.html");
                ++mentionIsALD;
              }
           else
              { if (mentionIsALD && mentionNonALD==0 )  html += CGestIni::Param_UpdateFromDisk(CApp::pCApp()->pathAppli()+"/Ressources/MentionsALD/SansALD.html");
                ++mentionNonALD;
              }
           xml  += "<OrdoLine>\n"+grammar+"</OrdoLine>\n\n";
           html += C_PosologieGrammar::toHtml(  grammar,
                                                m_pC_Frm_Prescription->imagePath("NonSubstituable.png"),
                                                css ,               // css par defaut
                                                QColor(Qt::black),
                                                QString::number(i+1)
                                             );
        } //end for ( i=0; i<grammar_list.size(); ++i )
    QString guid = "noPatient";
    if (PATIENT && PATIENT->get_guid().length()) guid = PATIENT->get_guid();
    // if (CApp::pCApp()->return_mode()=="ORDO")
    //   {"<ordoMedicaStruct\n>
    //
    //    "<Posologie_LAP_List>\n"
    //  }
    // else
       {
            // <HTML_Data>
         CGestIni::Param_UpdateToDisk( QString ( CApp::pCApp()->pathAppli()+"/Ressources/ComFiles/Ordo_%1.html" ).arg(guid), html);
         CGestIni::Param_UpdateToDisk( QString ( CApp::pCApp()->pathAppli()+"/Ressources/ComFiles/Ordo_%1.xml"  ).arg(guid),  xml);
       }
    return html;
}

//--------------------------------- Slot_tabWidget_Main_currentChanged -----------------------------------------------------------------------
void C_MW_Prescription::Slot_tabWidget_Main_currentChanged( int index)
{   if (m_pC_Frm_Prescription==0) return;
    if (index ==-1)               return;

    QWidget *pTab_QWidget =  m_pGUI->tab_MainTabWidget->widget(index);
    if (pTab_QWidget==0)          return;
    QString tabName       = pTab_QWidget->objectName();
    if (tabName=="tab_Prescription")
       { statusBar()->hide();
       }
    else if (tabName=="tab_Ident")
       {  if (m_isFirst_tab_Ident)
             { //......... renseigner l'interface avec le contexte d'entree (patient et user prescripteur) ..................
               //................................ saisie des variables biologiques ...........................................................
               m_isFirst_tab_Ident = false;
               patientCtxToInterface(PATIENT);
               userCtxToInterface(USER);
               connect( m_pGUI->lineEdit_taille,             SIGNAL(editingFinished()),                      this,   SLOT(Slot_lineEdit_taille_editingFinished()) );
               connect( m_pGUI->lineEdit_sexe,               SIGNAL(textChanged ( const QString & )),        this,   SLOT(Slot_lineEdit_sexe_textChanged (const QString &)) );
               connect( m_pGUI->lineEdit_poids,              SIGNAL(textChanged ( const QString & )),        this,   SLOT(Slot_lineEdit_poids_textChanged (const QString &)) );
               connect( m_pGUI->lineEdit_DateNaissance,      SIGNAL(editingFinished ()),                     this,   SLOT(Slot_lineEdit_DateNaissance_editingFinished ()) );
               connect( m_pGUI->lineEdit_Nom,                SIGNAL(textChanged ( const QString & )),        this,   SLOT(Slot_lineEdit_Nom_textChanged (const QString &)) );
               connect( m_pGUI->lineEdit_Prenom,             SIGNAL(textChanged ( const QString & )),        this,   SLOT(Slot_lineEdit_Prenom_textChanged (const QString &)) );
             }
          statusBar()->hide();
       }
    else if (tabName=="tab_bibliographie")
       {  if (m_webView_Biblio==0)
             { delete m_pGUI->webView_Help;
               m_webView_Biblio = new C_QWebView(m_pGUI->tab_bibliographie);
               m_webView_Biblio->setObjectName(QString::fromUtf8("m_webView_Biblio"));
               m_webView_Biblio->setUrl(QUrl("about:blank"));
               m_webView_Biblio->settings()->setLocalStoragePath ( CApp::pCApp()->pathAppli()+"/Ressources/web/" );
               m_webView_Biblio->page()->setLinkDelegationPolicy ( QWebPage::DelegateAllLinks );      //QWebPage::DelegateAllLinks
               m_pGUI->gridLayout_15->addWidget(m_webView_Biblio, 3, 0, 1, 2);

               //..........//////// barre de menus ////////....................
               m_pGUI->pushButton_RetrySite->setIcon        ( Theme::getIcon(CApp::pCApp()->applicationName()+"/RelancerModele.png") ) ;
               m_pGUI->pushButton_UpdateCache->setIcon      ( Theme::getIcon(CApp::pCApp()->applicationName()+"/UpdateInBase.png") ) ;
               m_pGUI->pushButton_DefineLogin->setIcon      ( Theme::getIcon(CApp::pCApp()->applicationName()+"/login.png") ) ;
               m_pGUI->pushButton_AllItems->setIcon         ( Theme::getIcon(CApp::pCApp()->applicationName()+"/loop.png") ) ;
               //................. <navigateur web> ............................................
               m_pGUI->pushButton_Home->setIcon    ( Theme::getIcon("22x22/home.png") ) ;
               m_pGUI->pushButton_Print->setIcon   ( Theme::getIcon("22x22/fileprint.png") ) ;
               m_pGUI->pushButton_find->setIcon    ( Theme::getIcon("22x22/edit-find.png") ) ;
               m_pGUI->pushButton_Back->setIcon    ( m_webView_Biblio->pageAction(QWebPage::Back)->icon() ) ;
               m_pGUI->pushButton_Reload->setIcon  ( m_webView_Biblio->pageAction(QWebPage::Reload)->icon() ) ;
               m_pGUI->pushButton_Forward->setIcon ( m_webView_Biblio->pageAction(QWebPage::Forward)->icon() ) ;
               m_pGUI->pushButton_Stop->setIcon    ( m_webView_Biblio->pageAction(QWebPage::Stop)->icon() ) ;

               connect( m_webView_Biblio,                SIGNAL(urlChanged (  const QUrl &  )),     this,  SLOT(Slot_webView_urlChanged ( const QUrl &  )));
               connect( m_webView_Biblio,                SIGNAL(loadStarted (  )),                  this,  SLOT(Slot_On_webView_Biblio_loadStarted ()) );
               connect( m_webView_Biblio,                SIGNAL(loadProgress (int)),                this,  SLOT(Slot_On_webView_Biblio_loadProgress (int)) );
               connect( m_webView_Biblio,                SIGNAL(loadFinished (bool  )),             this,  SLOT(Slot_On_webView_Biblio_loadFinished (bool)) );
               connect( m_webView_Biblio,                SIGNAL(statusBarMessage(const QString &)), this,  SLOT(Slot_On_webView_Biblio_statusBarMessage (const QString &)) );
               connect( m_webView_Biblio,                SIGNAL(linkClicked ( const QUrl &  )),     this,  SLOT(Slot_webView_Biblio_linkClicked ( const QUrl &  )));  // marche po

               connect( m_pGUI->pushButton_Home,         SIGNAL(clicked ( bool )),                  this,  SLOT(Slot_actionWebHome  (bool)) );
               connect( m_pGUI->pushButton_Print,        SIGNAL(clicked ( bool )),                  this,  SLOT(Slot_actionWebPrint (bool)) );
               connect( m_pGUI->pushButton_find,         SIGNAL(clicked ( bool )),                  this,  SLOT(Slot_actionWebFind  (bool)) );

               connect( m_pGUI->pushButton_Back,         SIGNAL(clicked ( bool )),      m_webView_Biblio,  SLOT(back ()) );
               connect( m_pGUI->pushButton_Reload,       SIGNAL(clicked ( bool )),      m_webView_Biblio,  SLOT(reload ()) );
               connect( m_pGUI->pushButton_Forward,      SIGNAL(clicked ( bool )),      m_webView_Biblio,  SLOT(forward ()) );
               connect( m_pGUI->pushButton_Stop,         SIGNAL(clicked ( bool )),      m_webView_Biblio,  SLOT(stop ()) );

               connect( m_pGUI->pushButton_DefineLogin,  SIGNAL(clicked ( bool )),                  this,  SLOT(Slot_pushButton_DefineLogin (bool)) );
               connect( m_pGUI->pushButton_RetrySite,    SIGNAL(clicked ( bool )),                  this,  SLOT(Slot_On_pushButton_RetrySite(bool)) );
               connect( m_pGUI->pushButton_UpdateCache,  SIGNAL(clicked ( bool )),                  this,  SLOT(Slot_On_UpdateCache(bool)) );
               connect( m_pGUI->pushButton_AllItems,     SIGNAL(clicked ( bool )),                  this,  SLOT(Slot_On_pushButton_AllItems(bool)) );
               connect( m_pGUI->lineEdit_url,            SIGNAL(returnPressed()),                   this,  SLOT(Slot_LocationEdit_returnPressed()));
               connect( m_pGUI->lineEdit_find,           SIGNAL(returnPressed()),                   this,  SLOT(Slot_WebFindEdit_returnPressed()));
               connect( m_pGUI->lineEdit_find,           SIGNAL(textChanged ( const QString &  ) ), this,  SLOT(Slot_WebFindEdit_textChanged (const QString & )) );
               m_webView_Biblio->page()->networkAccessManager()->setCookieJar(m_pCookieJar);
               CApp::pCApp()->setQNetworkAccessManager(m_webView_Biblio->page()->networkAccessManager()); // positionne m_pC_Utils_Html
               CApp::pCApp()->setQWebView(m_webView_Biblio);
               init_comboBox_BiblioURL();
             }
         statusBar()->show();
       }
    else if (tabName=="tab_Monographie")
       {  if (m_webView_Mono==0)
             {
               delete m_pGUI->webView_HAS;
               m_webView_Mono = new C_QWebView(m_pGUI->tab_Monographie);
               m_webView_Mono->setObjectName(QString::fromUtf8("webView_HAS"));
               m_webView_Mono->setUrl(QUrl("about:blank"));
               m_webView_Mono->page()->setLinkDelegationPolicy ( QWebPage::DelegateAllLinks );      //QWebPage::DelegateAllLinks
               m_webView_Mono->settings()->setAttribute( QWebSettings::JavascriptEnabled, true);
               m_webView_Mono->settings()->setLocalStoragePath ( CApp::pCApp()->pathAppli()+"/Ressources/HtmlCache/" );
               m_pGUI->gridLayout_5->addWidget(m_webView_Mono, 0, 0, 1, 1);
               m_webView_Mono->page()->networkAccessManager()->setCookieJar(m_pCookieJar);
               connect( m_webView_Mono,                  SIGNAL(linkClicked ( const QUrl &  )),      this, SLOT(Slot_webView_Mono_linkClicked ( const QUrl &  )));  // marche po
             }
          QTreeWidgetItem *pQTreeWidgetItem = currrentProductListTreeWidget()->currentItem();  //getSelectedListViewItem( currrentProductListTreeWidget() );
          if (pQTreeWidgetItem==0) return;
          treeWidget_Produits_itemClicked ( pQTreeWidgetItem, 0  );
          statusBar()->show();
       }
    else if (tabName=="tab_ordonnance")
       {  QString mask         = CGestIni::Param_UpdateFromDisk(CApp::pCApp()->pathAppli()+"/Ressources/OrdoMask/OrdoMask.xml");
          m_pGUI->textEdit_ordonnance->setText(ordoToHtml(mask));
          statusBar()->hide();
       }
    else if (tabName=="tab_Monitor")
       {  paramToInterface();
          statusBar()->hide();
       }
}
//--------------------------------- paramToInterface ----------------------------------------------------------------------------
void  C_MW_Prescription::paramToInterface()
{
    if (m_pGUI->comboBox_choixBase->count()==0)   // .. sert d'indicateur de premiere fois ..............
       { QString namePlugin = "";
         //........... liste des plugins .............................................
         m_pGUI->comboBox_choixBase->clear();
         m_pGUI->comboBox_choixBase->addItem("Patient_Base");
         m_pGUI->comboBox_choixBase->addItems(CApp::pCApp()->getParamList("DBM_connectors","name",1));     // charger le combo avec la liste des plugins mentionnes dans le .ini : 'Datasemp' 'Theriaque' 'C_BDM_GenericPlugin'
         QString plugin     = CApp::pCApp()->readUniqueParam("medicatux data source", "name");             // reperer celui de la liste qui est celui a installer : DATASEMP THERIAQUE C_BDM_GENERICPLUGIN
         int index          = m_pGUI->comboBox_choixBase->findText ( plugin, Qt::MatchFixedString );
         if (index != -1)
            { m_pGUI->comboBox_choixBase->setCurrentIndex(index);                                       // placer la liste des plugins sur celui par defaut
              namePlugin = m_pGUI->comboBox_choixBase->itemText(index);                                 // recuperer celui courant : Datasemp Theriaque C_BDM_GenericPlugin
              pluginParamConnexionToInterface(namePlugin);                                              // placer les parametres de connexion du plugin dans l'interface
            }
         //........................................ checkBox_linkStdBase ...........................................
         m_pGUI->checkBox_linkStdBase->setChecked( (CApp::pCApp()->readUniqueParam("medicatux session", "link standard dbm")=="true"));
         //........................................ checkBox_MainMenu ...........................................
         m_pGUI->checkBox_MainMenu->setChecked( (CApp::pCApp()->readUniqueParam("medicatux session", "main menu")=="true"));

         //....................................... combo du choix plugin ...........................................................................
         connect( m_pGUI->comboBox_choixBase,                  SIGNAL(activated ( const QString & )),     this,   SLOT(Slot_On_comboBox_choixBase(const QString &)) );
         connect( m_pGUI->pushButton_PosoEditorFontChoice,     SIGNAL(clicked ( bool )),                  this,   SLOT(Slot_On_pushButton_PosoEditorFontChoice(bool)) );
         connect( m_pGUI->pushButton_GeneralFontChoice,        SIGNAL(clicked ( bool )),                  this,   SLOT(Slot_On_pushButton_GeneralFontChoice(bool)) );
         //.................... activer les boutons et connexion favoris........................................
         m_pGUI->pushButton_ip_fav_add->setIcon(Theme::getIcon("16x16/add.png"));
         m_pGUI->pushButton_ip_fav_del->setIcon(Theme::getIcon("16x16/Vigie_annule.png"));
         connect( m_pGUI->pushButton_ip_fav_add,            SIGNAL(clicked ( bool )),                  this,   SLOT(Slot_On_pushButton_ip_fav_add(bool)) );
         connect( m_pGUI->pushButton_ip_fav_del,            SIGNAL(clicked ( bool )),                  this,   SLOT(Slot_On_pushButton_ip_fav_del(bool)) );
         connect( m_pGUI->pushButton_ip_connect,            SIGNAL(clicked ( bool )),                  this,   SLOT(Slot_On_pushButton_ip_connect(bool)) );
         connect( m_pGUI->pushButton_ip_apply,              SIGNAL(clicked ( bool )),                  this,   SLOT(Slot_On_pushButton_ip_apply(bool)) );
         connect( m_pGUI->comboBox_ip_fav ,                 SIGNAL(activated ( const QString &  )),    this,   SLOT(Slot_comboBox_ip_fav_Activated(const QString & )) );
         connect( m_pGUI->pushButton_InitHitList,           SIGNAL(clicked ( bool )),                  this,   SLOT(Slot_On_pushButton_InitHitList( bool )) );
         connect( m_pGUI->pushButton_initDrugListe,         SIGNAL(clicked ( bool )),                  this,   SLOT(Slot_On_pushButton_initDrugListe( bool)) );
         connect( m_pGUI->pushButton_logDel,                SIGNAL(clicked ( bool )),                  this,   SLOT(Slot_On_pushButton_logDel( bool)) );
         connect( m_pGUI->pushButton_FindMonoPath,          SIGNAL(clicked ( bool )),                  this,   SLOT(Slot_On_pushButton_FindMonoPath( bool)) );
         connect( m_pGUI->lineEdit_pathImagesMonographie,   SIGNAL(editingFinished ()),                this,   SLOT(Slot_On_lineEdit_pathImagesMonographie_editingFinished()) );
         connect( m_pGUI->lineEdit_ip_scrutation_time,      SIGNAL(editingFinished ()),                this,   SLOT(Slot_On_lineEdit_ip_scrutation_time_editingFinished()) );
         connect( m_pGUI->lineEdit_resultsLimit,            SIGNAL(editingFinished ()),                this,   SLOT(Slot_On_lineEdit_resultsLimit_editingFinished()) );
         connect( m_pGUI->lineEdit_DrugListLimit,           SIGNAL(editingFinished ()),                this,   SLOT(Slot_On_lineEdit_DrugListLimit_editingFinished()) );
         connect( m_pGUI->lineEdit_QSP_inSeconds,           SIGNAL(editingFinished ()),                this,   SLOT(Slot_On_lineEdit_QSP_inSeconds_editingFinished()) );
         connect( m_pGUI->checkBox_linkStdBase,             SIGNAL(stateChanged ( int )),              this,   SLOT(Slot_On_checkBox_linkStdBase_stateChanged( int )) );
         connect( m_pGUI->pushButton_SQL_Load,              SIGNAL(clicked ( bool )),                  this,   SLOT(Slot_On_pushButton_SQL_Load( bool )) );
         connect( m_pGUI->checkBox_MainMenu,                SIGNAL(stateChanged ( int )),              this,   SLOT(Slot_On_checkBox_MainMenu_stateChanged( int )) );
         m_pGUI->checkBox_logFile->hide();
       }
}
//-------------------------------- Slot_On_pushButton_SQL_Load ------------------------------------------------------
void C_MW_Prescription::Slot_On_pushButton_SQL_Load( bool )
{  if (m_pC_BDM_Api==0) return;

   QString driver     = m_pGUI->lineEdit_ip_driver->text().trimmed();
   QString user       = m_pGUI->lineEdit_ip_login->text().trimmed();
   QString password   = m_pGUI->lineEdit_ip_password->text().trimmed();
   QString hostname   = m_pGUI->lineEdit_ip_hostname->text().trimmed();
   QString port       = m_pGUI->lineEdit_ip_port->text().trimmed();
   QString fileName   = QFileDialog::getOpenFileName(  this, tr("inport Sql data"),
                                                        m_lastSqlPath,
                                                        tr("SQL files (*.SQL *.sql)"));
   if (fileName.length()==0)   return;
   m_lastSqlPath   = fileName;
   QString schema  = C_BaseCommon::getBaseNameToSetFromSQLFile( fileName );
   QString     err = C_BaseCommon::isThisSchemaExists(driver,schema,user, password,hostname,port );
   QDateTime dtDeb = QDateTime::currentDateTime();
   if (err.length())
      { m_pC_BDM_Api->outMessage( tr("================= start database initialisation <font color=#ffa200>%2</font> at %1 =====").arg(dtDeb.toString("dd-MM-yyyy hh:mm:ss"),schema));
        m_pC_BDM_Api->outMessage( tr(".            <font color=#ff0000>ERROR</font>   : Slot_On_pushButton_SQL_Load() %1 ").arg(err));
        m_pC_BDM_Api->outMessage( tr(".            creating shema : <font color=#ffa200>%1</font>              ").arg(schema));
        err = C_BaseCommon::createBase(driver, "mysql", user, password, hostname, port, schema );
        if (err.length())
           { m_pC_BDM_Api->outMessage(tr(".            <font color=#ff0000>ERROR</font> in creating shema : <font color=#ffa200>%1</font><br/>%2              ").arg(schema,err));
           }
      }
   else
      { int ret = QMessageBox::warning ( this, tr("Reinit database"), tr("Do you realy want to reinit this existing database ?<br/> <font color=#ff0000><b>%1</b></font> ").arg(schema), QMessageBox::YesToAll|QMessageBox::Cancel, QMessageBox::Cancel );
        if (ret == QMessageBox::Cancel) return;
        dtDeb = QDateTime::currentDateTime();
        m_pC_BDM_Api->outMessage( tr("================= start database initialisation <font color=#ffa200>%2</font> at %1 =====").arg(dtDeb.toString("dd-MM-yyyy hh:mm:ss"),schema));
      }
   statusBar()->show();
   //................................ on y va ...........................................
   if (err.length()==0)
      {  C_BaseCommon *pC_Base = new C_BaseCommon(this);
         if (pC_Base)
            { pC_Base->setProgressWidget(m_pQProgressBar);
              pC_Base->setLogWidget(m_pGUI->textEdit_Monitor);
              if ( pC_Base->BaseConnect("TEMP_BASE_FACTORY", driver, schema, user ,password ,hostname ,port.toInt(), DB_TIME_TO_RECONNECT ,&err)
                 )
                 { m_pC_BDM_Api->outMessage(tr(". SQL file : %1").arg(fileName));
                   pC_Base->executeSQL( fileName );
                 }
              else
                 { m_pC_BDM_Api->outMessage(tr(".          <font color=#ff0000>ERROR</font> in  connexion to shema : <font color=#ffa200>%1</font><br/> %2             ").arg(schema,err));
                 }
            }  // if (pC_Base)
      } //if (err.length()==0)
   //................................ on affiche le temps mis ...........................
   long    s = dtDeb.secsTo (QDateTime::currentDateTime());
   long    h = s/3600; s -=  h*3600;
   long    m = s/60;   s -=  m*60;
   m_pC_BDM_Api->outMessage( QObject::tr( "=== End of SQL initialisation <font color=#ffa200>%2</font> in : <font color=#ff0000><b>%1</b></font> ==")
                                           .arg(QString::number(h) + "h" + QString::number(m) + "mn" + QString::number(s) + "s",schema));
   statusBar()->hide();
}

//-------------------------------- init_main_menu ------------------------------------------------------
void C_MW_Prescription::init_main_menu(int state)
{  if (state && m_menuFichiers==0 )
      {
        //.......... les menus ...............
        QAction* action;
        QMenu  *menuAbout;

        m_menuFichiers = menuBar()->addMenu(tr("&File"));
        menuAbout      = menuBar()->addMenu(tr("&Help"));

        //................................ options MENU fichiers ....................................
        action = new QAction(this);
        action->setObjectName("actionQuitter");
        action->setText(tr("Quit"));
        action->setShortcutContext(Qt::ApplicationShortcut);
        m_menuFichiers->addAction(action);
        connect( action,                            SIGNAL(triggered ( bool )),                                            this, SLOT(Slot_toolButton_Annuler_clicked (bool)) );

        action = new QAction(this);
        action->setObjectName("actionQuitterConfirm");
        action->setText(tr("Quit and confirm"));
        action->setShortcutContext(Qt::ApplicationShortcut);
        m_menuFichiers->addAction(action);
        connect( action,                            SIGNAL(triggered ( bool )),                                            this, SLOT(Slot_toolButton_Ok_clicked (bool)) );

        action = new QAction(this);
        action->setObjectName("actionConfig");
        action->setText(tr("Preferences"));
        action->setShortcutContext(Qt::ApplicationShortcut);
        menuAbout->addAction(action);
        connect( action,                            SIGNAL(triggered ( bool )),                                            this, SLOT(Slot_actionConfig_triggered (bool)) );

        //................................options  MENU Help ....................................
        action = new QAction(this);
        action->setObjectName("actionApropos");
        action->setText(tr("A Propos"));
        action->setShortcutContext(Qt::ApplicationShortcut);
        menuAbout->addAction(action);
        connect( action,                            SIGNAL(triggered ( bool )),                                            this, SLOT(Slot_m_action_APropos (bool)) );

        action = new QAction(this);
        action->setObjectName("actionSignalement");
        action->setText(tr("Signalement de pharmaco-vigilance"));
        action->setShortcutContext(Qt::ApplicationShortcut);
        menuAbout->addAction(action);
        connect( action,                            SIGNAL(triggered ( bool )),                                            this, SLOT(Slot_m_action_Signalement (bool)) );

        action = new QAction(this);
        action->setObjectName("actionAbout");
        action->setText(tr("About"));
        action->setShortcutContext(Qt::ApplicationShortcut);
        menuAbout->addAction(action);
        connect( action,                            SIGNAL(triggered ( bool )),                                            this, SLOT(Slot_m_action_APropos (bool)) );

      }
    QWidget *menu =  menuWidget();
    if (menu==0) return;
    if (state) menu->setVisible(true);
    else       menu->setVisible(false);
}
//-------------------------------- Slot_RestoreNotification_triggered ------------------------------------------------------
void C_MW_Prescription::Slot_DockMenu()
{
    QMenu *dockMenu = createPopupMenu();
    dockMenu->exec(QCursor::pos ());
}

//-------------------------------- Slot_On_checkBox_linkStdBase_stateChanged ------------------------------------------------------
void C_MW_Prescription::Slot_On_checkBox_MainMenu_stateChanged( int state)
{   CApp::pCApp()->writeParam("medicatux session", "main menu", (state==Qt::Checked)?"true":"false");
    CApp::pCApp()->updateIniParamToDisk();
    init_main_menu(state);
}
//-------------------------------- Slot_On_checkBox_linkStdBase_stateChanged ------------------------------------------------------
void C_MW_Prescription::Slot_On_checkBox_linkStdBase_stateChanged( int state)
{   CApp::pCApp()->writeParam("medicatux session", "link standard dbm", (state==Qt::Checked)?"true":"false");
    CApp::pCApp()->updateIniParamToDisk();
}

//-------------------------------- Slot_On_lineEdit_DrugListLimit_editingFinished ------------------------------------------------------
void C_MW_Prescription::Slot_On_lineEdit_DrugListLimit_editingFinished()
{   //.............. ecrire dans le .ini  ..........................
    m_drugListMax = qMax(200, m_pGUI->lineEdit_DrugListLimit->text().toInt() );                               // on borne
    m_pGUI->lineEdit_DrugListLimit->setText(QString::number(m_drugListMax));                                               // on restitue
    CApp::pCApp()->writeParam("medicatux session", "drug list limit", m_pGUI->lineEdit_DrugListLimit->text().toLatin1() ); // on perenise
    CApp::pCApp()->updateIniParamToDisk();
}
//-------------------------------- Slot_On_lineEdit_QSP_inSeconds_editingFinished ------------------------------------------------------
void C_MW_Prescription::Slot_On_lineEdit_QSP_inSeconds_editingFinished()
{   //.............. ecrire dans le .ini  ..........................
    int defaultDuration = qMax(86400,m_pGUI->lineEdit_QSP_inSeconds->text().toInt());
    if (m_pC_BDM_Api) m_pC_BDM_Api->setDefaultDurationInSeconds(defaultDuration);
    m_pGUI->lineEdit_QSP_inSeconds->setText(QString::number(defaultDuration));
    CApp::pCApp()->writeParam("medicatux session", "default duration in seconds", m_pGUI->lineEdit_QSP_inSeconds->text().toLatin1() );
    CApp::pCApp()->updateIniParamToDisk();
}
//-------------------------------- Slot_On_lineEdit_ip_scrutation_time_editingFinished ------------------------------------------------------
void C_MW_Prescription::Slot_On_lineEdit_ip_scrutation_time_editingFinished()
{   //.............. on cherche le plugin de base de donnee dans la liste.................
    QString namePlugin = m_pGUI->comboBox_choixBase->currentText().trimmed();
    if ( namePlugin.length()==0 ) return;
    //.............. ecrire dans le .ini les parametres de connexion de cette base Therapeutique ..........................
    QString section = "Connexion_" + namePlugin;
    CApp::pCApp()->writeParam(section.toLatin1(), "TimeToReconnect", m_pGUI->lineEdit_ip_scrutation_time->text().toLatin1() );
    CApp::pCApp()->updateIniParamToDisk();
}
//-------------------------------- Slot_On_lineEdit_resultsLimit_editingFinished ------------------------------------------------------
void C_MW_Prescription::Slot_On_lineEdit_resultsLimit_editingFinished()
{   //.............. on cherche le plugin de base de donnee dans la liste.................
    QString namePlugin = m_pGUI->comboBox_choixBase->currentText().trimmed();
    if ( namePlugin.length()==0 ) return;
    //.............. ecrire dans le .ini les parametres de connexion de cette base Therapeutique ..........................
    QString section = "Connexion_" + namePlugin;
    CApp::pCApp()->writeParam(section.toLatin1(), "max results", m_pGUI->lineEdit_resultsLimit->text().toLatin1() );
    CApp::pCApp()->updateIniParamToDisk();
}
//-------------------------------- pluginParamConnexionToInterface ------------------------------------------------------
void C_MW_Prescription::pluginParamConnexionToInterface(const QString &namePlugin) // Datasemp Theriaque C_BDM_GenericPlugin
{
              QWidget *pTab_QWidget =  m_pGUI->tab_MainTabWidget->currentWidget();
              if (pTab_QWidget->objectName()!="tab_Monitor") return;
              if (namePlugin.length()==0)                    return;
              QString section = "Connexion_"+namePlugin;
              QString driver,schema,login,password,hostname,port;
              CApp::pCApp()->readParam(section.toLatin1(), "Parametres",&driver,&schema,&login,&password,&hostname,&port);

              m_pGUI->lineEdit_ip_driver->setText(driver);
              m_pGUI->lineEdit_ip_schema->setText(schema);
              m_pGUI->lineEdit_ip_login->setText(login);
              m_pGUI->lineEdit_ip_password->setText(CApp::pCApp()->decodedPassword(password));
              m_pGUI->lineEdit_ip_hostname->setText(hostname);
              m_pGUI->lineEdit_ip_port->setText(port);

              QString value = CApp::pCApp()->readUniqueParam(section.toLatin1(), "TimeToReconnect");
              if (value.length()) m_pGUI->lineEdit_ip_scrutation_time->setText(value);
              value = CApp::pCApp()->readUniqueParam(section.toLatin1(), "PathImageSource");
              if (value.length()) m_pGUI->lineEdit_pathImagesMonographie->setText(value);
              value = CApp::pCApp()->readUniqueParam(section.toLatin1(), "max results");
              if (value.length()) m_pGUI->lineEdit_resultsLimit->setText(value);
              m_pGUI->pushButton_ip_apply->setText(tr("Apply to : %1").arg(namePlugin));
              //....................................... les favoris ...........................................................................
              init_comboBox_ip_fav(namePlugin);
}
//-------------------------------- init_comboBox_ip_fav ------------------------------------------------------
void C_MW_Prescription::init_comboBox_ip_fav(const QString &namePlugin)
{ //................. charger les IP favoris ................................................
  QString defStr        = "";
  QStringList paramList = CApp::pCApp()->getParamList( namePlugin + PREF_CONNEXION_SUFFIX ,"Parametres", 1,1);
  if (paramList.size()==0) paramList.clear();

  //............... charger la combo des serveurs TCP IP et la placer sur celui par defaut ...........................................
  //   Parametres  = QMYSQL3 , DrTuxTest , root ,  , localhost , 3306
  m_pGUI->comboBox_ip_fav->clear();
  QStringList      lst;
  for (int i=0; i<paramList.size();++i)
      { lst    = paramList[i].split(",");
        //       driver               schema                log                    passw                hostname              port
        defStr = lst[0].trimmed()+","+lst[1].trimmed()+","+lst[2].trimmed()+","+ CApp::pCApp()->encodedPassword(lst[3].trimmed())+","+lst[4].trimmed()+","+lst[5].trimmed();
        if (m_pGUI->comboBox_ip_fav->findText(defStr) == -1)  m_pGUI->comboBox_ip_fav->insertItem(0, defStr );    // comboBox_ip_fav
      }
  defStr =  m_pGUI->lineEdit_ip_driver->text().trimmed()                                   +"," +
            m_pGUI->lineEdit_ip_schema->text().trimmed()                                   +"," +
            m_pGUI->lineEdit_ip_login->text().trimmed()                                    +"," +
            CApp::pCApp()->encodedPassword(m_pGUI->lineEdit_ip_password->text().trimmed()) +"," +
            m_pGUI->lineEdit_ip_hostname->text().trimmed()                                 +"," +
            m_pGUI->lineEdit_ip_port->text().trimmed();
  m_pGUI->comboBox_ip_fav->setCurrentIndex(m_pGUI->comboBox_ip_fav->findText(defStr));

  //................. sauvegarder les listes avec password encodes ..................................................
  if (namePlugin.length()==0 || m_pGUI->comboBox_ip_fav->count()==0) return;
  CApp::pCApp()->replaceComboboxListInParam(m_pGUI->comboBox_ip_fav,   namePlugin + PREF_CONNEXION_SUFFIX,       "Parametres");
  CApp::pCApp()->updateIniParamToDisk();
}
//-------------------------------- Slot_On_pushButton_FindMonoPath ------------------------------------------------------
void C_MW_Prescription::Slot_On_pushButton_FindMonoPath( bool )
{ QString path = m_pGUI->lineEdit_pathImagesMonographie->text().trimmed();
  if ( QDir::isRelativePath ( path ) ) path  = QDir::cleanPath ( path.prepend(CApp::pCApp()->pathAppli() ) );
  QString dir  = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                  path,
                                                  QFileDialog::ShowDirsOnly
                                                  | QFileDialog::DontResolveSymlinks);
  if (dir.length()==0|| ! QFile::exists(dir) ) return;
  m_pGUI->lineEdit_pathImagesMonographie->setText(dir);
  Slot_On_lineEdit_pathImagesMonographie_editingFinished();
}
//-------------------------------- Slot_On_lineEdit_pathImagesMonographie_editingFinished ------------------------------------------------------
void C_MW_Prescription::Slot_On_lineEdit_pathImagesMonographie_editingFinished()
{   QString dir = m_pGUI->lineEdit_pathImagesMonographie->text();
    if ( dir.length()==0 )     return;
    if ( QDir::isRelativePath ( dir ) ) dir  = QDir::cleanPath ( dir.prepend(CApp::pCApp()->pathAppli() ) );
    if ( ! QFile::exists(dir) ) return;
    dir = CGestIni::AbsoluteToRelativePath(CApp::pCApp()->pathAppli(), dir);
    m_pGUI->lineEdit_pathImagesMonographie->setText(dir);
    //.............. on cherche le plugin de base de donnee dans la liste.................
    QString namePlugin = m_pGUI->comboBox_choixBase->currentText().trimmed();
    if ( namePlugin.length()==0 ) return;
    //.............. ecrire dans le .ini les parametres de connexion de cette base Therapeutique ..........................
    QString section = "Connexion_" + namePlugin;
    CApp::pCApp()->writeParam(section.toLatin1(), "PathImageSource", dir.toLatin1() );
    CApp::pCApp()->updateIniParamToDisk();
}
//-------------------------------- Slot_On_pushButton_logDel ------------------------------------------------------
void C_MW_Prescription::Slot_On_pushButton_logDel( bool )
{ m_pGUI->textEdit_Monitor->clear();
}
//-------------------------------- Slot_On_pushButton_InitHitList ------------------------------------------------------
void C_MW_Prescription::Slot_On_pushButton_InitHitList( bool )
{   int ret = QMessageBox::warning ( this, tr("Reinit prefereds lists"), tr("Do you realy want to clear your prefereds list ?"), QMessageBox::YesToAll|QMessageBox::Cancel, QMessageBox::Cancel );
    if (ret == QMessageBox::Cancel) return;
    if (m_pC_BDM_Api)
       { m_pC_BDM_Api->reinitHitsList();
         fill_treeWidget_Favoris();
       }
}

//-------------------------------- Slot_On_pushButton_initDrugListe ------------------------------------------------------
void C_MW_Prescription::Slot_On_pushButton_initDrugListe( bool)
{   if (m_pC_BDM_Api) m_pC_BDM_Api->updateDrugLists();
    Slot_lineEdit_DrugsSearch_textChanged (""); //on remet a jour la liste
}
//-------------------------------- test_ip_connect ------------------------------------------------------
int  C_MW_Prescription::test_ip_connect()
{  QString driver   = m_pGUI->lineEdit_ip_driver->text().trimmed();
   QString schema   = m_pGUI->lineEdit_ip_schema->text().trimmed();
   QString user     = m_pGUI->lineEdit_ip_login->text().trimmed();
   QString password = m_pGUI->lineEdit_ip_password->text().trimmed();
   QString hostname = m_pGUI->lineEdit_ip_hostname->text().trimmed();
   QString port     = m_pGUI->lineEdit_ip_port->text().trimmed();
   if ( m_pC_BDM_Api )
      { m_pC_BDM_Api->outMessage(tr("======================= Connexion's parameters test ================================"));
        m_pC_BDM_Api->outMessage(tr(".         Driver   : '%1'").arg(driver));
        m_pC_BDM_Api->outMessage(tr(".         Schema   : '%1'").arg(schema));
        m_pC_BDM_Api->outMessage(tr(".         Login    : '%1'").arg(user));
        m_pC_BDM_Api->outMessage(tr(".         Password : '%1'").arg(password));
        m_pC_BDM_Api->outMessage(tr(".         Hostname : '%1'").arg(hostname));
        m_pC_BDM_Api->outMessage(tr(".         Port     : '%1'").arg(port));
      }
   QString error = C_BaseCommon::isThisSchemaExists( driver, schema,  user,  password,  hostname,   port);
   if ( m_pC_BDM_Api )
      { if (error.length()) { m_pC_BDM_Api->outMessage(error.replace("\n","\n.                                   "));
                              m_pC_BDM_Api->outMessage(tr(".            <font color=#ff0000>ERROR   : this connection is not accessible</font>"));
                              return 0;
                            }
        else                { m_pC_BDM_Api->outMessage(tr(".            <font color=#00c0ff>SUCCESS : this connection is accessible </font>"));
                              return 1;
                            }
      }
   return 0;
}
//-------------------------------- Slot_On_pushButton_ip_fav_add ------------------------------------------------------
void C_MW_Prescription::Slot_On_pushButton_ip_fav_add(bool)
{QString defStr =  m_pGUI->lineEdit_ip_driver->text().trimmed()                                   +"," +
                   m_pGUI->lineEdit_ip_schema->text().trimmed()                                   +"," +
                   m_pGUI->lineEdit_ip_login->text().trimmed()                                    +"," +
                   CApp::pCApp()->encodedPassword(m_pGUI->lineEdit_ip_password->text().trimmed()) +"," +
                   m_pGUI->lineEdit_ip_hostname->text().trimmed()                                 +"," +
                   m_pGUI->lineEdit_ip_port->text().trimmed();
 m_pGUI->comboBox_ip_fav->insertItem(0, defStr );
 m_pGUI->comboBox_ip_fav->setCurrentIndex(0);
 //................. sauvegarder les listes avec password encodes ..................................................
 QString namePlugin = m_pGUI->comboBox_choixBase->currentText().trimmed();
 if (namePlugin.length()==0 || m_pGUI->comboBox_ip_fav->count()==0) return;
 CApp::pCApp()->replaceComboboxListInParam(m_pGUI->comboBox_ip_fav,   namePlugin + PREF_CONNEXION_SUFFIX,       "Parametres");
 CApp::pCApp()->updateIniParamToDisk();
}
//-------------------------------- Slot_On_pushButton_ip_fav_del ------------------------------------------------------
void C_MW_Prescription::Slot_On_pushButton_ip_fav_del(bool)
{ int index = m_pGUI->comboBox_ip_fav->currentIndex();
  if (index==-1) return;
  m_pGUI->comboBox_ip_fav->removeItem(index);
  index = qMin(index,m_pGUI->comboBox_ip_fav->count()-1);
  m_pGUI->comboBox_ip_fav->setCurrentIndex(index);
  //................. sauvegarder les listes avec password encodes ..................................................
  QString namePlugin = m_pGUI->comboBox_choixBase->currentText().trimmed();
  if (namePlugin.length()==0 || m_pGUI->comboBox_ip_fav->count()==0) return;
  CApp::pCApp()->replaceComboboxListInParam(m_pGUI->comboBox_ip_fav,   namePlugin + PREF_CONNEXION_SUFFIX,       "Parametres");
  CApp::pCApp()->updateIniParamToDisk();
}
//------------------------ Slot_comboBox_ip_fav_Activated ---------------------------------------
//       driver               shema                 log                    passw                hostname              port
void C_MW_Prescription::Slot_comboBox_ip_fav_Activated( const QString &str  )
{ QStringList lst    = str.split(",");
  if (lst.size()<6) return;
  m_pGUI->lineEdit_ip_driver->setText(lst[0]);
  m_pGUI->lineEdit_ip_schema->setText(lst[1]);
  m_pGUI->lineEdit_ip_login->setText(lst[2]);
  m_pGUI->lineEdit_ip_password->setText(lst[3]);
  m_pGUI->lineEdit_ip_hostname->setText(lst[4]);
  m_pGUI->lineEdit_ip_port->setText(lst[5]);
}
//-------------------------------- Slot_On_comboBox_choixBase ------------------------------------------------------
void C_MW_Prescription::Slot_On_comboBox_choixBase(const QString &namePlugin)
{   //................... si base C_BDM_Api ....................
    //                    ne pas traiter comme un plugin
    if (namePlugin == "Patient_Base")
      { pluginParamConnexionToInterface(namePlugin);
        return;
      }
   // set_BDM_Plugin(namePlugin, 0); initile de basculer la base : le seul moyen est le bouton "Appliquer"
   pluginParamConnexionToInterface(namePlugin);
   setWindowTitle();
}
//-------------------------------- Slot_On_pushButton_ip_apply ------------------------------------------------------
/*    [Connexion_Theriaque]
      Parametres = QMYSQL3 , theriaque , root ,  , localhost , 3306
      TimeToReconnect   = 3600000
      PathImageSource   = Ressources/TheriaqueHtmlCache
      max results = 50
*/
void C_MW_Prescription::Slot_On_pushButton_ip_apply(bool)
{  //............ appliquer que si connexion valide .....................................
   if ( test_ip_connect()==0 )   return;
   //.............. on cherche le plugin de base de donnee dans la liste.................
   QString namePlugin = m_pGUI->comboBox_choixBase->currentText().trimmed();
   if ( namePlugin.length()==0 ) return;

   QString driver     = m_pGUI->lineEdit_ip_driver->text().trimmed();
   QString schema     = m_pGUI->lineEdit_ip_schema->text().trimmed();
   QString user       = m_pGUI->lineEdit_ip_login->text().trimmed();
   QString password   = m_pGUI->lineEdit_ip_password->text().trimmed();
   QString hostname   = m_pGUI->lineEdit_ip_hostname->text().trimmed();
   QString port       = m_pGUI->lineEdit_ip_port->text().trimmed();
   QString schema_std = CApp::pCApp()->readUniqueParam( "Connexion_Patient_Base", "Parametres", 1 );  // recuperer le schema de la base standard (seul argument a conserver lorsque paremetres lies)

   //.............. ecrire dans le .ini les parametres de connexion de cette base Therapeutique ..........................
   QString section = "Connexion_" + namePlugin;
   CApp::pCApp()->writeParam(section.toLatin1(), "Parametres",driver.toLatin1(),schema.toLatin1(),user.toLatin1(),CApp::pCApp()->encodedPassword(password).toLatin1(),hostname.toLatin1(),port.toLatin1());
   //.............. si base standard liee placer aussi les parametres dans le .ini ..........................
   if ( m_pGUI->checkBox_linkStdBase->isChecked() )
      { CApp::pCApp()->writeParam("Connexion_Patient_Base", "Parametres", driver.toLatin1(), schema_std.toLatin1(),user.toLatin1(),CApp::pCApp()->encodedPassword(password).toLatin1(),hostname.toLatin1(),port.toLatin1());
      }
   CApp::pCApp()->updateIniParamToDisk();
   //................... si base standard C_BDM_Api comme choix dans la combobox....................
   //                    ne pas traiter comme un plugin
   if ( namePlugin == "Patient_Base" )
      {  m_pC_BDM_Api->changeDataBaseConnexion(driver, schema, user, password, hostname, port);   // la le schema est celui specifie dans les parametres saisis
         Slot_lineEdit_DrugsSearch_textChanged ("");
         return;
      }
   //.............. si base standard liee la placer aussi sur les parametres..........................
   if ( m_pGUI->checkBox_linkStdBase->isChecked() )
      { m_pC_BDM_Api->changeDataBaseConnexion(driver, schema_std, user, password, hostname, port);
        //Slot_lineEdit_DrugsSearch_textChanged ("");
      }
   //.............. basculer le systeme sur la base therapeutique ..........................
   set_BDM_Plugin(namePlugin, 1);   // 1 pour recreer le plugin (force a tenir compte du fichier de configuration qui vient d'etre mis a jour)
   pluginParamConnexionToInterface(namePlugin);
   fill_treeWidget_Favoris();
   setWindowTitle();
}

//-------------------------------- Slot_On_pushButton_ip_connect ------------------------------------------------------
void C_MW_Prescription::Slot_On_pushButton_ip_connect(bool)
{  test_ip_connect();
}
//-------------------------------- set_BDM_Plugin ------------------------------------------------------
/*! \brief connect API to source datas via C_BDM_InitPluginI interface which provides data to  C_BDM_Api
 *  \param QString namePlugin of C_BDM_InitPluginI data source ( 'theriaque' 'datasemp')
 *  \param int oldMustBeDeleted
 *         if 1 the instance plugin must be deleted an a new instance created (not only get a already instancied in m_pC_BDM_PluginI_Map)
 *         if 0 get if exists a already instancied in m_pC_BDM_PluginI_Map(faster)
 *  \return C_BDM_PluginI * pointer on plugin or zero if error
 */

C_BDM_PluginI *C_MW_Prescription::set_BDM_Plugin(const QString &_namePlugin, int oldMustBeDeleted  /*=0*/)   // theriaque datasemp C_BDM_GENERICPLUGIN
{   QTime debTime       = QTime::currentTime();
    QTime debGTime      = QTime::currentTime();
    QString namePlugin  = _namePlugin.toUpper().trimmed();
    C_BDM_PluginI *pC_BDM_PluginI = 0;

    //.............. si besoin (oldMustBeDeleted !=0)  effacer l'eventuelle instance ..........................
    if ( oldMustBeDeleted !=0 )
       {    m_pC_BDM_Api->outMessage(tr("=============== SET DATABASE delete if exists %1' =====================").arg( _namePlugin ));
            QMap<QString, C_BDM_PluginI*>::const_iterator it = CApp::pCApp()->m_pC_BDM_PluginI_Map.find(_namePlugin.toUpper());
            if (it != CApp::pCApp()->m_pC_BDM_PluginI_Map.constEnd() )   // >>>>> si trouve le tuer et recreer une instance
               { C_BDM_PluginI* pC_BDM_PluginI = it.value();
                 if ( pC_BDM_PluginI )
                    { CApp::pCApp()->m_pC_BDM_PluginI_Map.remove(_namePlugin.toUpper());
                      delete pC_BDM_PluginI;
                      m_pC_BDM_Api->outMessage(tr("\n.           '%1' is deleted").arg(_namePlugin ));
                    }
               }
       }

    m_pC_BDM_Api->outMessage(tr("=============== SET DATABASE '%1' =====================").arg(_namePlugin ));
    //.............. on cherche le plugin de base de donnee dans la liste.................
    QMap<QString, C_BDM_PluginI*>::const_iterator it = CApp::pCApp()->m_pC_BDM_PluginI_Map.find(namePlugin);
    //.............. si non trouve on initialise le plugin ...............................
    if (it == CApp::pCApp()->m_pC_BDM_PluginI_Map.constEnd() )  // si pas trouve le creer et le rajouter a la liste
       {
            QString confData =   QString("[BDM_Configuration]\n"
                                         "     PathAppli     = %2\n"
                                         "     PathIni       = %3\n").arg( CApp::pCApp()->pathAppli() , CApp::pCApp()->pathIni());
            if (namePlugin=="THERIAQUE")
               { debTime  = QTime::currentTime();
                 pC_BDM_PluginI = new C_BDM_TheriaquePlugin(confData, "MAIN_", 0, m_pGUI->textEdit_Monitor, m_pQProgressBar);
                 m_pC_BDM_Api->outMessage(tr("Database '%1' creation en : %2").arg(namePlugin,   QTime (0, 0, 0, 0 ).addMSecs(debTime.msecsTo(QTime::currentTime())).toString("mm:ss:zzz") ));
               }
            else if (namePlugin=="C_BDM_GENERICPLUGIN")
               { debTime  = QTime::currentTime();
                 pC_BDM_PluginI =  new C_BDM_GenericPlugin(confData, "MAIN_", 0, m_pGUI->textEdit_Monitor, m_pQProgressBar);
                 m_pC_BDM_Api->outMessage(tr("Database '%1' creation en : %2").arg(namePlugin,   QTime (0, 0, 0, 0 ).addMSecs(debTime.msecsTo(QTime::currentTime())).toString("mm:ss:zzz") ));
               }
            else if (namePlugin=="C_BDM_GOUVPLUGIN")
               { debTime  = QTime::currentTime();
                 pC_BDM_PluginI =  new C_BDM_GouvPlugin(confData, "MAIN_", 0, m_pGUI->textEdit_Monitor, m_pQProgressBar);
                 m_pC_BDM_Api->outMessage(tr("Database '%1' creation en : %2").arg(namePlugin,   QTime (0, 0, 0, 0 ).addMSecs(debTime.msecsTo(QTime::currentTime())).toString("mm:ss:zzz") ));
               }
            else if (namePlugin=="DATASEMP")
               { debTime  = QTime::currentTime();
                 pC_BDM_PluginI =  new C_BDM_DatasempPlugin(confData, "MAIN_", 0, m_pGUI->textEdit_Monitor, m_pQProgressBar);
                 m_pC_BDM_Api->outMessage(tr("Database '%1' creation en : %2").arg(namePlugin,   QTime (0, 0, 0, 0 ).addMSecs(debTime.msecsTo(QTime::currentTime())).toString("mm:ss:zzz") ));
               }
            else if (namePlugin.length()==0)
               { m_pC_BDM_Api->outMessage(tr("failure in data source initialisation :  <b>empty plugin name</b>"));
               }
            else
               { m_pC_BDM_Api->outMessage(tr("failure in data source <b>%1</b> unknow name").arg(namePlugin));
               }

            //............................... CASSOS SI NOM INCONNU ................................
            if (pC_BDM_PluginI==0)  return 0;
            if (pC_BDM_PluginI->isInitialised())
               {  CApp::pCApp()->m_pC_BDM_PluginI_Map.insert(namePlugin, pC_BDM_PluginI);
               }
            else
               {  pC_BDM_PluginI = 0;
                  m_pC_BDM_Api->outMessage(tr("failure in drug data source <b>%1</b> initialisation").arg(namePlugin));
               }
       }
    //.............. si trouve on prend le plugin ...............................
    else
      {pC_BDM_PluginI = it.value();
      }

    //....................... activer ce plugin dans tous les objets qui en dependent ..................
    if ( pC_BDM_PluginI && pC_BDM_PluginI->isInitialised() && m_pC_BDM_Api)
       {
          m_pC_BDM_Api->connectToDataSource( pC_BDM_PluginI );             // donner au controleur le plugin
          CApp::pCApp()->set_BDM_Plugin(pC_BDM_PluginI);                   // set_BDM_Plugin est defini dans C_Macro.h dont CApp est derive
          m_pC_BDM_Api->addOwner(pC_BDM_PluginI->owner());                 // n'est rajoute que si n'existe pas
          if (m_pC_BDM_Api->drugsList_Size()==0)   //  cas du plugin generique (pas de base medicamenteuse)
             { m_pGUI->toolBox_ListesProduits->unactiveItemByObjectName("page_Accessoires");
               m_pGUI->toolBox_ListesProduits->unactiveItemByObjectName("page_Produits");
               m_pGUI->toolBox_classifications->unactiveItemByObjectName("page_Composition");
               m_pGUI->toolBox_classifications->unactiveItemByObjectName("page_ATC");
             }
          else
             { m_pGUI->toolBox_ListesProduits->activeItemByObjectName("page_Produits");
               m_pGUI->toolBox_classifications->activeItemByObjectName("page_Composition");
               m_pGUI->toolBox_classifications->activeItemByObjectName("page_ATC");
               if (namePlugin=="DATASEMP")
                  { m_pGUI->toolBox_ListesProduits->activeItemByObjectName("page_Accessoires");
                    m_pGUI->toolBox_ListesProduits->setItemIcon ( "page_Accessoires", Theme::getIcon("medicatux/toolbox_accessoires.png") ); // pourquoi pert il son icone ??
                  }
               else
                  { m_pGUI->toolBox_ListesProduits->unactiveItemByObjectName("page_Accessoires");
                  }
               int indexPage = m_pGUI->toolBox_ListesProduits->toolWidgetIndexByTabObjectName("page_Produits");
               m_pGUI->toolBox_ListesProduits->setItemText ( indexPage, tr("Products %1").arg(_namePlugin) );
               m_pGUI->toolBox_ListesProduits->setItemIcon ( indexPage, m_pC_BDM_Api->plugin_icon() );
               debTime  = QTime::currentTime();
               //......... remplissage des listes ......................
               m_pC_BDM_Api->fill_ATC_treeWidget(m_pGUI->treeWidget_ATC);  // ATC doit toujours etre remplie
               m_pC_BDM_Api->outMessage(tr("Database '%1' fill_ATC_treeWidget() en : %2").arg(namePlugin, QTime (0, 0, 0, 0 ).addMSecs(debTime.msecsTo(QTime::currentTime())).toString("mm:ss:zzz") ));
             }
          debTime  = QTime::currentTime();
          fill_only_showed_treeWidgets( m_pGUI->toolBox_ListesProduits->currentWidget()->objectName() );
          m_pC_BDM_Api->outMessage(tr("Database '%1' Slot_lineEdit_DrugsSearch_textChanged() en : %2").arg(namePlugin, QTime (0, 0, 0, 0 ).addMSecs(debTime.msecsTo(QTime::currentTime())).toString("mm:ss:zzz") ));
          //......... ecrire dans fichier .ini avec le plugin choisi ................................
          QString section = "medicatux data source";
          CApp::pCApp()->writeParam(section.toLatin1(), "name", namePlugin.toLatin1());
          CApp::pCApp()->updateIniParamToDisk();      //  IMPORTANT ne pas oublier de dumper sur le disque car Slot_evaluateAllInteractions() cree le thread d'interraction avec les infos du disque
          if (m_pC_Frm_Prescription)
             { m_pC_Frm_Prescription->set_Thread_DBM_Plugin(  _namePlugin , oldMustBeDeleted );
               m_pC_Frm_Prescription->Slot_evaluateAllInteractions();
             }
          m_pC_BDM_Api->outMessage(tr("Database '%1' connect\303\251e en : %2").arg(namePlugin,   QTime (0, 0, 0, 0 ).addMSecs(debGTime.msecsTo(QTime::currentTime())).toString("mm:ss:zzz") ));
       }
    else
       {pC_BDM_PluginI = 0;
       }
    return pC_BDM_PluginI;
}

/*
//------------------------ Slot_checkBox_traceEnabled_stateChanged ---------------------------------------
void C_MW_Prescription::Slot_checkBox_traceEnabled_stateChanged( int state)
{m_pGUI->textEdit_Monitor->setEnabled(state==Qt::Checked);
 QString etat = (state==Qt::Checked)?"ok":"no";
 CApp::pCApp()->writeParam(CApp::pCApp()->applicationName()+" session", "LogFTP", etat.toLatin1());
 CApp::pCApp()->updateIniParamToDisk();
}
*/

//--------------------------------- prescriptionToXML ----------------------------------------------------------------------------
QString  C_MW_Prescription::prescriptionToXML(C_Frm_Prescription *m_pC_Frm_Prescription)
{   Q_UNUSED (m_pC_Frm_Prescription);
    return "";
}

//--------------------------------- Slot_webView_Biblio_linkClicked -----------------------------------------------------------------------
void C_MW_Prescription::Slot_webView_Biblio_linkClicked ( const QUrl & url )
{ if ( URL_ExecSpecialSyntax ( url.toString().trimmed() ,CApp::pCApp()->pathAppli()) ) return;
  if ( m_webView_Biblio )  m_webView_Biblio->load ( url );
}
//--------------------------------- getCustomMonographiePropertys -----------------------------------------------------------------------
int  C_MW_Prescription::getCustomMonographiePropertys ( QString &title , QString &author, QDateTime &date_time, QString &text)
{ /*
  if (m_pC_PopupDial_NotesInfos==0)
     { //QPoint        pos = mapFromGlobal (QCursor::pos());
       QPoint        pos (m_webView_Biblio->x(),m_webView_Biblio->y());
       QRect initialRect = QRect(pos.x(),pos.y(),1,1);    // m_Button_Indic->x()+m_Button_Indic->width()
       QRect deployRect  = QRect(pos.x(),pos.y(),100,200);
       QColor color      = QApplication::palette ( this ).color(this->backgroundRole());
       m_pC_PopupDial_NotesInfos = new    C_PopupDial_NotesInfos(this, title, author, date_time, color ,initialRect, deployRect);
       connect( m_pC_PopupDial_NotesInfos,       SIGNAL(Sign_finished(const QString &)) ,   this,   SLOT(Slot_PopupDial_NotesInfos_finished( const QString & )) );
       m_pC_PopupDial_NotesInfos->show();
     }
  m_pC_PopupDial_NotesInfos->setToDeployRect ();
  */
    C_Dlg_NotesInfos *p_dlg = new C_Dlg_NotesInfos(this,title,author,date_time, text);
    if (p_dlg && p_dlg->exec()==1)
       { title     = p_dlg->title();
         author    = p_dlg->author();
         date_time = p_dlg->date_time();
         text      = p_dlg->texte(); qDebug()<<text;
         delete p_dlg;
         return 1;
       }
    delete p_dlg;
    return 0;
}
/*
//--------------------------------- Slot_PopupDial_NotesInfos_finished -----------------------------------------------------------------------
void  C_MW_Prescription::Slot_PopupDial_NotesInfos_finished( const QString &direction )
{if (direction=="TO_INITIAL")
    { m_pC_PopupDial_NotesInfos->deleteLater();
      m_pC_PopupDial_NotesInfos = 0;
    }
}
*/
//--------------------------------- URL_ExecSpecialSyntax -----------------------------------------------------------------------
int  C_MW_Prescription::URL_ExecSpecialSyntax ( QString urlStr , const QString &pathAppli)
{   qDebug()<<urlStr;
    if (urlStr.startsWith("exec:"))    // modify:#ancre_id_[1-index]_   remove:#ancre_id_[1-index]_
       {urlStr = urlStr.mid(5);
        QStringList argList = urlStr.split("||");
        urlStr = argList[0]; if (!argList.isEmpty()) argList.removeAt(0);
        if ( QDir(urlStr).isRelative()) urlStr.prepend(pathAppli);
        urlStr = QDir::cleanPath(urlStr) + F_EXE;
#ifdef Q_WS_MAC
        QString module = urlStr;
        int     posDeb = module.lastIndexOf("/");
        if (posDeb != -1)
           { ++posDeb;
             module = module.mid(posDeb);
           }
        urlStr += ".app/Contents/MacOS/" + module;
#endif
        QProcess::startDetached(urlStr, argList);
        return 1;
       }
    else if (urlStr.startsWith("remove:#ancre"))
       { QString  pk =  CGestIni::CutStrRight( urlStr, "remove:#ancre_id_[");
                  pk =  CGestIni::CutStrLeft(pk, "-");
         if (pk.length()==0) return 1;
         int ret          = QMessageBox::warning( this, tr("Delete a note"),
                                                  "Delete a note",
                                                  QMessageBox::Cancel
                                                | QMessageBox::Apply,
                                                  QMessageBox::Cancel);
         if (ret==QMessageBox::Apply)
            {m_pC_BDM_Api->delete_CustomMonographie(pk);
             show_ProductMonographie(m_last_drugListRecord);
            }
       }
    else if (urlStr.startsWith("modify:#ancre"))
       { QString  pk =  CGestIni::CutStrRight( urlStr, "modify:#ancre_id_[");
                  pk =  CGestIni::CutStrLeft(pk, "-");
         if (pk.length()==0) return 1;
         QByteArray datas;
         QString   libelle;
         QString   author;
         QDateTime date_time;
         QString   texte  =  m_pC_BDM_Api->get_CustomMonographie( pk, libelle,  author,  date_time);
         if ( getCustomMonographiePropertys ( libelle , author, date_time, texte) )
            { datas = texte.toLatin1();
              if (datas.isEmpty())
                 { QMessageBox msgBox;
                    msgBox.setText(tr ("Nothing in Note Editor."));
                    msgBox.exec();
                    return 1;
                 }
              m_pC_BDM_Api->update_CustomMonographie(pk, libelle, author, date_time, datas);
              show_ProductMonographie(m_last_drugListRecord);
            }
       }
    else if (urlStr.startsWith("new:#ancre"))
       { QString title       = "Titre de la note";
         QString author      = "Anonyme";
         QDateTime date_time = QDateTime::currentDateTime();
         QString   texte     = "Entrez ici le texte de la note....";
         if (USER)
            {author =USER->get_usual_name() + ";"+USER->get_forename();
            }
         QByteArray datas;
         if (getCustomMonographiePropertys ( title , author, date_time, texte))
            { datas = texte.toLatin1();
              if (datas.isEmpty())
                 { QMessageBox msgBox;
                    msgBox.setText(tr ("Nothing in Note Editor."));
                    msgBox.exec();
                    return 1;
                 }
              m_pC_BDM_Api->add_Monographie_to_CustomMonographie( title, author, datas, date_time, m_last_drugListRecord);
              show_ProductMonographie(m_last_drugListRecord);
            }
       }
    else if (urlStr.contains("Code_CIP="))
       { int              p = urlStr.indexOf("Code_CIP=");
         QString        cip = "";
         if ( p != -1 ) cip = urlStr.mid(p+9);
         QList < C_BDM_DrugListRecord > records = m_pC_BDM_Api->selectProductsList (cip, C_BDM_PluginI::cip_filter);
         if ( records.size() )
            { show_ProductMonographie(records[0]);
              m_pGUI->lineEdit_DrugsSearch->setText(cip);
              set_toolBox_ListesProduits_item ( "page_Produits" );
            }
        }
    else if (urlStr.contains("ref_mono:"))
       { int               p = urlStr.indexOf("ref_mono:");
         QString        file = "";
         if ( p != -1 ) file = urlStr.mid(p+9);

         QString folder = CApp::pCApp()->readUniqueParam("Connexion_Datasemp", "PathReferences");
         if ( !folder.endsWith('/')) folder += '/';
         file.prepend(folder);
         if ( QDir(file).isRelative()) file.prepend(pathAppli);
         file = QDir::cleanPath(file);
         QDesktopServices::openUrl (file.prepend("file:") );
       }
    else if (urlStr.startsWith("mailto:"))
       {urlStr      = urlStr.mid(7);
        int posArg  = urlStr.indexOf("?");                 //"mailto:%1?subject=%2&body=%3" la liste d'arguments commence apr?s '?' et sont separes par '&'
        if (posArg != -1)
           {++posArg;
            QString   argMailTo = urlStr.mid(posArg);
            QStringList argList = argMailTo.split("&");
            QString       token = "";
            QString       value = "";
            int           nbArg = argList.count();
            int               i = 0;
            urlStr              = urlStr.left(posArg);
            while( i<nbArg)
                {int posEgal  = argList[i].indexOf("=");
                 token        = "";
                 value        = "";
                 if (posEgal != -1)
                    {++posEgal;
                     token = argList[i].left(posEgal);
                     value = argList[i].mid(posEgal);
                    }
                 if (value.startsWith("file:")||value.startsWith("$File"))
                    {value = value.mid(5);
                     if ( QDir(value).isRelative()) value.prepend(pathAppli);
                     value = QDir::cleanPath(value);
                     value = CGestIni::Param_UpdateFromDisk(value);
                    }
                 urlStr += token+value;
                 ++i;
                 if (i<nbArg) urlStr += "&";
                }
            }
        QDesktopServices::openUrl (urlStr.prepend("mailto:") );
        return 1;
       }
    return 0;
}

//--------------------------------------- Datasemp_fill_treeWidget_Accessoires -------------------------------------------------------
/*
# define COL_PROD_NAME    0
# define COL_DCI_1        1
# define COL_DCI_2        2
# define COL_DCI_3        3
# define COL_PRIX         4
# define COL_ATC          5
# define COL_ID           6
# define COL_CIS          7
# define COL_ORG          8
# define COL_ID_TYP       9
# define COL_SPEC_KEY     10
# define COL_GENERIQUE    11
# define COL_STATUT       12
# define COL_NB_UP        13
 */
int C_MW_Prescription::Datasemp_fill_treeWidget_Accessoires(QTreeWidget *pQTreeWidget, int familleIndex, const QString &text  /* = "" */)
{pQTreeWidget->clear();
 if (m_pC_BDM_Api==0)                           return 0;
 if (m_pC_BDM_Api->plugin_name() != "Datasemp") return 0;
 QTreeWidgetItem *item = 0;
 QString   familleSemp = "";
 QString       requete = 0;
 int   fromHomeopathie = 0;
 if ( familleIndex >= 7 )                              // 7-BOIRON 0G 8-WELEDA 0H 9-LEHNING 0I
    {
      //................. Preparer la requete ............................................................................
      static QString tables[3] = {"t0G","t0H","t0I"};
      //                                                                     homeopathie 1X  --> f1 pointe sur t0g.f0 pour boiron et t0h.f0 pour weleda
      //                                                     fk = 1 = Specialites ; 2 = Dietetiques ; 3 = Veterinaires ; 4 = Parapharmacie ; 5 = Accessoires ; 6 = Divers (herboristerie, etc.) ; 7 = Homeeopathie
      //                                            |         0   1   2   3   4   5
                           requete  =  QString(     " SELECT fB, ge, f0, fG, f2, f1 FROM %1 ").arg(tables[familleIndex-7]);
      //                                     Code Vignette __^   ^   ^   ^   ^   ^     // ne sert pas est juste pour utiliser la query(0)
      //                                     PrimaryKey _________|   |   |   |   |
      //                                     Code produit____________|   |   |   |
      //                                     Libelle Medicament _________|   |   |
      //                                     code CIP _______________________|   |
      //                                     code_UV ____________________________|
      if (text.length())  requete  +=                " WHERE (f3 like '%" + text + "%' OR f3 like '%"+ text.toUpper() + "%')";
                          requete  +=                " limit 0,300";
      fromHomeopathie               = 1;
    }
 else
    { familleSemp = QString::number(familleIndex);
      //................. Preparer la requete ............................................................................
      //                                                                     homeopathie 1X  --> f1 pointe sur t0g.f0 pour boiron et t0h.f0 pour weleda
      //                                                     fk = 1 = Sp\303\251cialit\303\251s ; 2 = Di\303\251t\303\251tiques ; 3 = V\303\251t\303\251rinaires ; 4 = Parapharmacie ; 5 = Accessoires ; 6 = Divers (herboristerie, etc.) ; 7 = Hom\303\251opathie
      //                                            |         0   1   2   3   4   5   6
                           requete  =               " SELECT fK, ge, f0, fG, f2, f1, f8 FROM t00 WHERE fk like '" + familleSemp + "%' ";
      //                                     Famille SEMP____^   ^   ^   ^   ^   ^   ^
      //                                     PrimaryKey _________|   |   |   |   |   |
      //                                     Code produit____________|   |   |   |   |
      //                                     Libelle Medicament _________|   |   |   |
      //                                     code CIP _______________________|   |   |
      //                                     code_UV ____________________________|   |
      //                                     nbr UP  ________________________________|
      if (text.length())  requete  +=                " AND (f3 like '%" + text + "%' OR f3 like '%"+ text.toUpper() + "%')";
                          requete  +=                " limit 0,300";
      fromHomeopathie               = 0;
    }
 QSqlQuery query(m_pC_BDM_Api->get_database());
 if ( ! query.exec(requete) )
    {  // m_pC_BDM_Api->outSQL_error( *pQSqlQuery, QObject::tr("ERREUR : C_MW_Prescription::Datasemp_fill_treeWidget_Accessoires() can't execute requete"), requete, __FILE__, __LINE__);
       return 0;
    }
 QIcon datasempIcone  = fromHomeopathie?Theme::getIcon("medicatux/homeopathie.png"):Theme::getIcon("medicatux/toolbox_accessoires.png");
 if (query.isActive() )
    {while (query.next())
           { QString id      = query.value(4).toString();                      // m_BDM_POSO_HIT_PK
             QString id_type = "cip";                  // cip
             if ( id.length()==0 )
                { id      = query.value(2).toString();
                  id_type = "DS_NPRD";                  // numero de produit
                }
             item =   new QTreeWidgetItem (pQTreeWidget);
             if ( item )  //  paranoiac attitude
                { item->setText( COL_PROD_NAME, CGestIni::Utf8_Query(query, 3) );   // libelle
                  item->setText( COL_DCI_1     , "");                         // DCI_1
                  item->setText( COL_DCI_2     , "");                         // DCI_2
                  item->setText( COL_DCI_3     , "");                         // DCI_3
                  item->setText( COL_PRIX      , "");                         // Prix
                  item->setText( COL_ATC       , "");                         // ATC
                  item->setText( COL_ID        , id);                         // ID CIP
                  item->setText( COL_ORG       , "DS_A");                     // PROVENANCE DS_A pour D ata S emp et A Accessoires
                  item->setText( COL_ID_TYP    , id_type);                    // ID Type
                  item->setText( COL_GENERIQUE , "");                         // generique   G R ou S
                  item->setText( COL_STATUT    , "VH");                       // statut  Ville Hopital VH ou H
                  if ( fromHomeopathie )
                     {  item->setText( COL_NB_UP     , 0); }                        // nombre d'unite de prise
                  else
                     {  item->setText( COL_NB_UP     , query.value(6).toString());} // nombre d'unite de prise
                 item->setIcon( COL_PROD_NAME , datasempIcone);
                } // end if (item)  //  paranoiac attitude
           }    // end while (query.next())
    }  // endif (query.isActive() )

 return 1;
}

//--------------------------------------- Slot_pushButton_exportCustomList -------------------------------------------------------
void C_MW_Prescription::Slot_pushButton_exportCustomList(bool)
{ exportDrugsList(m_pGUI->treeWidget_CustomDrugsList, 1);
}
//--------------------------------------- Slot_pushButton_importCustomList -------------------------------------------------------
void C_MW_Prescription::Slot_pushButton_importCustomList(bool)
{ importDrugsList(m_pGUI->treeWidget_CustomDrugsList, 0);
}
//--------------------------------------- importDrugsList -------------------------------------------------------
int C_MW_Prescription::importDrugsList(QTreeWidget *pQTreeWidget, bool clearList /* = 0*/)
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Export products list"),
                                                        m_lastExportPath,
                                                        tr("Xml files (*.XML *.xml)"));
  if (fileName.length()==0)   return 0;
  m_lastExportPath         = fileName;

  QString          dataXML = CGestIni::Param_UpdateFromDisk( fileName );
  QStringList drugsXmlList = CGestIni::getXmlDataList("C_BDM_DrugListRecord", dataXML);
  QString         userName = "BDM";
  QString         listName = m_pGUI->comboBox_customList->currentText();

  int                   nb = drugsXmlList.size();
  int                    i = 0;
  QList < C_BDM_DrugListRecord > drugsList;
  for ( i = 0; i < nb; ++i)
      { drugsList.append( C_BDM_DrugListRecord::unSerialize(drugsXmlList[i]) );
      }
  if  (drugsList.size())
      { if (clearList) pQTreeWidget->clear();
        add_Product_to_CustomProductsList(userName, listName, drugsList);
      }
  return nb;
}

//--------------------------------------- exportDrugsList -------------------------------------------------------
void C_MW_Prescription::exportDrugsList(QTreeWidget *pQTreeWidget, bool selectMode /* = 0*/)
{
 QString fileName = QFileDialog::getSaveFileName(this, tr("Import products list"),
                                                   m_lastExportPath,
                                                   tr("Xml files (*.XML *.xml)"));
 if (fileName.length()==0) return ;
 m_lastExportPath = fileName;

 QString dataXML =  "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n"
                    "<!-- Projet    : MedinTux                              -->\n"
                    "<!-- Module    : medicabase                            -->\n"
                    "<!-- Objet     : C_BDM_DrugListRecordArray             -->\n"
                    "<!-- guru mail : sevin-roland@medintux.org             -->\n"
                    "<!-- Copyright : (C) 2004-to-2013 and for the eternity -->\n";
  dataXML +=        "<C_BDM_DrugListRecordArray>\n"
                    "   <version>1</version>\n";
  QList < C_BDM_DrugListRecord > drugsList = makeDrugsListRecords_From_QTreeWidgetProducts(pQTreeWidget,  selectMode);
  for (int i=0; i<drugsList.size(); ++i)
      {dataXML += drugsList[i].serialize(0,"   ");    // serialisation sans le header
      }
  dataXML +=        "</C_BDM_DrugListRecordArray>\n";

  CGestIni::CGestIni::Param_UpdateToDisk(fileName,dataXML.toLatin1());
}

//--------------------------------- Slot_lineEdit_lineEdit_FavorisSearch_textChanged -----------------------------------------------------------------------
void  C_MW_Prescription::Slot_lineEdit_lineEdit_FavorisSearch_textChanged (const QString &text)
{  fill_treeWidget_Favoris(text);
}
//--------------------------------- Slot_lineEdit_lineEdit_FavorisSearch_keyPressEvent -----------------------------------------------------------------------
void  C_MW_Prescription::Slot_lineEdit_lineEdit_FavorisSearch_keyPressEvent(QKeyEvent *event, int &callStandardEvent)
{ lineEdit_searchProducts_keyPressEvent(m_pGUI->treeWidget_Favoris, event, callStandardEvent);
}

//--------------------------------------- Slot_comboBox_FamillesProduits_activated -------------------------------------------------------
void C_MW_Prescription::Slot_comboBox_FamillesProduits_activated( int familleIndex)
 {Datasemp_fill_treeWidget_Accessoires(m_pGUI->treeWidget_Accessoires, familleIndex+2);
 }
//--------------------------------------- Slot_lineEdit_AccessoiresSearch_textChanged -------------------------------------------------------
void C_MW_Prescription::Slot_lineEdit_AccessoiresSearch_textChanged (const QString &text)
{Datasemp_fill_treeWidget_Accessoires(m_pGUI->treeWidget_Accessoires, m_pGUI->comboBox_FamillesProduits->currentIndex()+2, text);
}
//--------------------------------------- Slot_lineEdit_AccessoiresSearch_keyPressEvent -------------------------------------------------------
void C_MW_Prescription::Slot_lineEdit_AccessoiresSearch_keyPressEvent(QKeyEvent *event, int &callStandardEvent)
{lineEdit_searchProducts_keyPressEvent(m_pGUI->treeWidget_Accessoires, event, callStandardEvent);
}

//--------------------------------------- Slot_lineEdit_CustomSearch_textChanged -------------------------------------------------------
void C_MW_Prescription::Slot_lineEdit_CustomSearch_textChanged (const QString &text)
{   if (m_pC_BDM_Api==0) return;
    QString         userName = "BDM";
    QString         listName = m_pGUI->comboBox_customList->currentText();

    QList < C_BDM_DrugListRecord > drugsList = m_pC_BDM_Api->selectCustomProductsList( userName,  listName, text, (C_BDM_PluginI::flags)  ( (int)C_BDM_PluginI::nom_filter
                                                                                                                                           |(int)C_BDM_PluginI::atc_filter
                                                                                                                                           |(int)C_BDM_PluginI::dci_filter
                                                                                                                                           |(int)C_BDM_PluginI::cip_filter
                                                                                                                                           |(int)C_BDM_PluginI::dc3_filter
                                                                                                                                          )
                                                                                     );
    m_pGUI->treeWidget_CustomDrugsList->clear();
    fill_treeWidget_ProductsFromDrugList(m_pGUI->treeWidget_CustomDrugsList, drugsList);
}

//--------------------------------------- Slot_lineEdit_CustomSearch_keyPressEvent -------------------------------------------------------
void C_MW_Prescription::Slot_lineEdit_CustomSearch_keyPressEvent(QKeyEvent *event, int &callStandardEvent)
{ lineEdit_searchProducts_keyPressEvent(m_pGUI->treeWidget_CustomDrugsList, event, callStandardEvent);
}

//--------------------------------------- lineEdit_searchProducts_keyPressEvent -------------------------------------------------------
void C_MW_Prescription::lineEdit_searchProducts_keyPressEvent(QTreeWidget *pQTreeWidget, QKeyEvent *event, int &callStandardEvent)
{   QTreeWidgetItem * pQTreeWidgetItem = 0;
    switch(event->key())
      {case Qt::Key_Return:
       case Qt::Key_Enter:
             pQTreeWidgetItem = getSelectedListViewItem( pQTreeWidget );
             if ( pQTreeWidgetItem==0 && pQTreeWidget->topLevelItemCount()==1 )
                { pQTreeWidgetItem = pQTreeWidget->topLevelItem(COL_PROD_NAME);
                  if (pQTreeWidgetItem) pQTreeWidgetItem->setSelected(true);
                }
             if (event->modifiers () == Qt::ControlModifier)
                { Slot_toolButton_Ok_clicked (true );
                }
             else
                {
                  Slot_add_product_In_C_Frm_Prescription(pQTreeWidgetItem);
                  //.......... reselectionner la ligne d'edition pour preparer la nouvelle saisie ....................
                  if      (pQTreeWidget == m_pGUI->treeWidget_Accessoires)      m_pGUI->lineEdit_AccessoiresSearch->selectAll();
                  else if (pQTreeWidget == m_pGUI->treeWidget_Produits)         m_pGUI->lineEdit_DrugsSearch->selectAll();
                  else if (pQTreeWidget == m_pGUI->treeWidget_CustomDrugsList)  m_pGUI->lineEdit_CustomSearch->selectAll();
                  else if (pQTreeWidget == m_pGUI->treeWidget_Favoris)          m_pGUI->lineEdit_FavorisSearch->selectAll();
                  else if (pQTreeWidget == m_pGUI->treeWidget_PatientDrugs)     m_pGUI->lineEdit_HistoriqueSearch->selectAll();
                  callStandardEvent = 0;
                }
             break;
       case Qt::Key_Down:
            {pQTreeWidgetItem = getSelectedListViewItem(pQTreeWidget);
             if (pQTreeWidgetItem)
                {QTreeWidgetItem * pQTreeWidgetItemBelow = pQTreeWidget->itemBelow( pQTreeWidgetItem );
                 if (pQTreeWidgetItemBelow)
                    {pQTreeWidgetItem->setSelected(false);
                     pQTreeWidgetItemBelow->setSelected(true);
                     pQTreeWidget->scrollToItem (pQTreeWidgetItemBelow);
                    }
                }
             else if (pQTreeWidget->topLevelItemCount ())
                {pQTreeWidgetItem = pQTreeWidget->topLevelItem(COL_PROD_NAME);
                 if (pQTreeWidgetItem) pQTreeWidgetItem->setSelected(true);
                }
             callStandardEvent = 0;
            }
            break;
       case Qt::Key_Up:
            {pQTreeWidgetItem = getSelectedListViewItem(pQTreeWidget);
             if (pQTreeWidgetItem)
                {QTreeWidgetItem * pQTreeWidgetItemAbove = pQTreeWidget->itemAbove( pQTreeWidgetItem );
                 if (pQTreeWidgetItemAbove)
                    {pQTreeWidgetItem->setSelected(false);
                     pQTreeWidgetItemAbove->setSelected(true);
                     pQTreeWidget->scrollToItem (pQTreeWidgetItemAbove);
                    }
                }
             else if (pQTreeWidget->topLevelItemCount ())
                {pQTreeWidgetItem = pQTreeWidget->topLevelItem(COL_PROD_NAME);
                 if (pQTreeWidgetItem) pQTreeWidgetItem->setSelected(true);
                }
             callStandardEvent = 0;
            }
            break;
      }
}

//--------------------------------------- Slot_Widget_Produits_ContextMenuRequested -------------------------------------------------------
void C_MW_Prescription::Slot_Widget_Produits_ContextMenuRequested(QPoint )
{drugsList_ContextMenuRequested(  m_pGUI->treeWidget_Produits);
}
//--------------------------------------- Slot_Widget_treeWidget_Favoris_ContextMenuRequested -------------------------------------------------------
void C_MW_Prescription::Slot_Widget_treeWidget_Favoris_ContextMenuRequested(QPoint )
{drugsList_ContextMenuRequested(  m_pGUI->treeWidget_Favoris);
}
//--------------------------------------- Slot_Widget_treeWidget_CustomDrugsList_ContextMenuRequested -------------------------------------------------------
void C_MW_Prescription::Slot_Widget_treeWidget_CustomDrugsList_ContextMenuRequested(QPoint )
{drugsList_ContextMenuRequested(  m_pGUI->treeWidget_CustomDrugsList);
}
//--------------------------------------- Slot_treeWidget_Accessoires_ContextMenuRequested -------------------------------------------------------
void C_MW_Prescription::Slot_treeWidget_Accessoires_ContextMenuRequested(QPoint)
{drugsList_ContextMenuRequested(  m_pGUI->treeWidget_Accessoires);
}
//--------------------------------------- Slot_treeWidget_PatientDrugs_ContextMenuRequested -------------------------------------------------------
void C_MW_Prescription::Slot_treeWidget_PatientDrugs_ContextMenuRequested(QPoint)
{drugsList_ContextMenuRequested(  m_pGUI->treeWidget_Accessoires);
}
//--------------------------------------- Slot_pushButton_pushButton_CustomListMenu -------------------------------------------------------
void C_MW_Prescription::Slot_pushButton_pushButton_CustomListMenu(bool)
{  //pushButton_menuFavoris
  QWidget *pQWidget  = m_pGUI->toolBox_ListesProduits->currentWidget ();
  QString ob_name = pQWidget->objectName();
  if (ob_name == "page_Favoris")
     { m_pGUI->pushButton_menuFavoris->setIcon( Theme::getIcon("medicatux/menu_down.png") ) ;
       drugsList_ContextMenuRequested( m_pGUI->treeWidget_Favoris);
       m_pGUI->pushButton_menuFavoris->setIcon( Theme::getIcon("medicatux/menu_left.png") ) ;
     }
  else
    { m_pGUI->pushButton_CustomListMenu->setIcon( Theme::getIcon("medicatux/menu_down.png") ) ;
      drugsList_ContextMenuRequested( m_pGUI->treeWidget_CustomDrugsList);
      m_pGUI->pushButton_CustomListMenu->setIcon( Theme::getIcon("medicatux/menu_left.png") ) ;
    }
}

//--------------------------------------- drugsList_ContextMenuRequested -------------------------------------------------------
void C_MW_Prescription::drugsList_ContextMenuRequested(QTreeWidget *pQTreeWidget)
{   QWidget *pQWIdget                  = m_pGUI->toolBox_ListesProduits->currentWidget();
    QString  pageName                  = pQWIdget->objectName();

    QTreeWidgetItem * pQTreeWidgetItem = pQTreeWidget->currentItem();
    QString userName                   = "BDM";
    QString listName                   = m_pGUI->comboBox_customList->currentText();
    int                              i = 0;
    QStringList optionList;
    optionList     << tr("$ContextMenuProducts");             // titre

    if (pQTreeWidgetItem)
       { optionList     << "=1=#medicatux/add_to_prescription.png#"   + tr("Add this product to current prescription");
         optionList     << "=3=#medicatux/monographie.png#"           + tr("show monographie for this product");
         optionList     << "-----------";
         if (pageName == "page_CustomList")
            { optionList << "=4=#medicatux/objectDel.png#"            + tr("Remove selected items from custom drugs list '%1'.").arg(listName);
            }
         if (m_pGUI->comboBox_customList->count())
            { optionList << ">customDrugsList<"+tr("#medicatux/sequenceadd.png#Add this item to custom drugs list");   // menu d'appel du sous menu
              for (int id=0; id<m_pGUI->comboBox_customList->count();++id)                                             // options du sous menu
                  { QString item = m_pGUI->comboBox_customList->itemText(id);  // nom de la custom drugs list
                    optionList << "<customDrugsList>=_"+item+"="+ item;        // l'identification de l'option (entre deux signes =) est : _ suivi du nom de la liste
                  }
            }
       }
    if (pageName == "page_CustomList")
       { if (m_pGUI->comboBox_customList->count())
            { optionList << "-----------";
              optionList << "=5=#medicatux/edit.png#"                      + tr("create new free item for custom drugs list '%1'.").arg(listName);
              if (pQTreeWidget->selectedItems().size())
                  optionList << "=6=#medicatux/export_list.png#"           + tr("Export selected items from  '%1'.").arg(listName);
              optionList << "=7=#medicatux/import_list.png#"               + tr("Import items for '%1'.").arg(listName);
            }
         optionList << "-----------";
         optionList << "=8=#medicatux/create_list.png#"               + tr("Create a new custom drugs list.");
         optionList << "=9=#medicatux/remove_list.png#"               + tr("Remove custom drugs list '%1'.").arg(listName);
       }
    else if (pageName == "page_Favoris")
       { optionList << "-----------";
         if (pQTreeWidget->selectedItems().size())
             optionList << "=10=#medicatux/export_list.png#"               + tr("Export selected items.");
         optionList << "=11=#medicatux/import_list.png#"               + tr("Import products");
       }

    if (optionList.size()==0) return;

    optionList     << "-----------";
    optionList     << "=0=#medicatux/QuitterMenu.png#"         + tr("Quit this menu.");
    //........................ si commence par _ alors une liste custom ..............................
    //                         a ete cliquee
    QString option = ThemePopup(optionList,this, " border: 1px solid #8f8f91; border-radius: 6px; font-size: 11px;").DoPopupList();
    if (option.startsWith('_') && pQTreeWidgetItem)
       { QList < C_BDM_DrugListRecord > drugsList =  makeDrugsListRecords_From_QTreeWidgetProducts(pQTreeWidget, 1);
         add_Product_to_CustomProductsList(userName, option.mid(1), drugsList);
         return;
       }
    switch(option.toInt())
    {  case 1:             //...... Add this product to current prescription ......
            { QList<QTreeWidgetItem *> itemsList = pQTreeWidget->selectedItems();
              for ( i=0; i<itemsList.size(); ++i )
                  {treeWidget_Produits_itemDoubleClicked ( itemsList.at(i) , 0  );
                  }
            } break;
       case 3:             //...... show monographie for this product ......
            { show_ProductMonographie(C_BDM_DrugListRecord_From_QTreeWidgetProductsItem (pQTreeWidgetItem));
            } break;
       case 4:             //...... Remove this item from this list ......
            {
             QList < C_BDM_DrugListRecord > drugsList = makeDrugsListRecords_From_QTreeWidgetProducts(pQTreeWidget, 1);    // 1 car on veut tous ceux selectionnes
             QString                          message = "";
             int                                   nb = drugsList.size();
             if (nb > 1)
                {if (nb>10)
                    { message = tr("Remove all the %1 selecteds products.<br />"
                                   "from <font color=#0000ff><b>'%2'</b></font> drugs list .Do you want this?").arg(QString::number(nb), listName);
                    }
                 else
                    { QString productsStr = "<font size=7pts color=#ff0000><b>";
                      for ( i=0; i<nb; ++i )
                          { productsStr += "<br />&nbsp;&nbsp;&nbsp;&nbsp;'" + drugsList.at(i).commercialName() + "'";
                          }
                      productsStr += "<br /></b></font>";
                      message = tr("Remove all the following %1 selected products.<br />"
                                   "from <font color=#0000ff><b>'%2'</b></font> drugs list .Do you want this?").arg(productsStr, listName);
                    }
                }
             else
                {message = tr("This product <font color=#ff0000><b>'%1'</b></font> must be deleted.<br />"
                               "from <font color=#0000ff><b>'%2'</b></font> drugs list .Do you want this?").arg(pQTreeWidgetItem->text(COL_PROD_NAME), listName);
                }
             int ret          = QMessageBox::warning( this, tr("Delete a product"),
                                                      message,
                                                      QMessageBox::Cancel
                                                    | QMessageBox::Apply,
                                                      QMessageBox::Cancel);
             if (ret==QMessageBox::Apply)
                { for (i=0; i<nb; ++i)
                      { m_pC_BDM_Api->del_Product_from_CustomProductsList(userName, listName, drugsList.at(i));
                      }
                  changeCurrentCustomList(userName, listName);     // on recharge la liste
                } // endif (ret==QMessageBox::Apply)
            } break;
       case 5:             //...... create new item for this list ......
            {  bool ok;
               QString drugName = QInputDialog::getText(this, tr("give your custom drugname"),
                                                        tr("Give your new custom drug name:"), QLineEdit::Normal,
                                                        "", &ok);
               if (ok && !drugName.isEmpty())
                 { QString id      = m_pC_BDM_Api->GUID_Create();
                   QString id_type = "FRT";
                   /*    C_BDM_DrugListRecord(QString id,
                         QString id_type,
                         QString commercialName,
                         QString dci_1Name,
                         QString dc2_1Name,
                         QString dc3_1Name,
                         QString owner,
                         QString lang,
                         QString ucd,
                         QString ucd_price,
                         QString atc,
                         QString generique,
                         QString statut,
                         QString schema_posologique
                        )
                    */
                   C_BDM_DrugListRecord c_bdm_druglistrecord(id,id_type, drugName,"","","","OW","fr","","","","","VH");
                   QList < C_BDM_DrugListRecord > drugsList;
                   drugsList.append(c_bdm_druglistrecord);
                   add_Product_to_CustomProductsList(userName, listName, drugsList );
                 }
            } break;
       case 6:             //...... Export selected items for this list ......
            { Slot_pushButton_exportCustomList(true);
            } break;
       case 7:             //...... Import items for this list ......
            { Slot_pushButton_importCustomList(true);
            } break;
       case 8:             //...... Create a new list ......
            {  Slot_pushButton_addCustomList(true);
            } break;
       case 9:             //...... Remove this list ......
            { Slot_pushButton_delCustomList(true);
            } break;
       case 10:             //...... Export selected items. ......
            { export_selected_prefered_items();
            } break;
       case 11:             //...... Import products. ......
            { import_prefered_products();
            } break;
    }
}

//--------------------------------------- export_selected_prefered_items -------------------------------------------------------
int  C_MW_Prescription::export_selected_prefered_items()
{   //.................... si existe pas d'items cassos.........................
    if (m_pC_BDM_Api == 0)                   return 0;

    int     nb                         = m_pGUI->treeWidget_Favoris->topLevelItemCount();
    QTreeWidgetItem  *pQTreeWidgetItem = 0;
    if (nb ==0)            return 0;
    //.................... preparer la liste des ID .........................
    //                     pour l'appel selectFavorisList()
    int nb_selected   = 0;
    QString id_list   = "ID_LIST('";
    for (int i=0; i<nb; ++i)
       { pQTreeWidgetItem = m_pGUI->treeWidget_Favoris->topLevelItem (i);
         if (pQTreeWidgetItem->isSelected())
            { id_list +=  pQTreeWidgetItem->text(COL_ID) + "','";
              ++nb_selected;
            }
       }
    id_list.chop(2);    // virer le dernier   ,'
    id_list += ")";
    //.................... si existe pas d'items selectionnes cassos.........................
    if (nb_selected ==0) return 0;
    //.................... selectionner le ficher de sortie .........................
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export selected items"),
                                                           m_lastExportPath,
                                                           tr("Xml Files (*.xml *.XML)"));
    if (fileName.length()==0) return 0;
    m_lastExportPath = fileName;
    //..................... interroger la base de donnees ......................
    QList < C_BDM_DrugListRecord >  drugsList = m_pC_BDM_Api->selectFavorisList (id_list);
    nb                                        = drugsList.size();
    C_BDM_DrugListRecord   drugRecord;

    //................ version avec nom des medoc ..............................
    QString outXml = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n"
                     "<!-- Projet    : MedinTux                              -->\n"
                     "<!-- Module    : medicabase                            -->\n"
                     "<!-- Objet     : ExportPreferedsDrugsList              -->\n"
                     "<!-- guru mail : sevin-roland@medintux.org             -->\n"
                     "<!-- Copyright : (C) 2004-to-2013 and for the eternity -->\n";
    outXml   += "<ExportPreferedsDrugsList>\n";
    for (int i = 0; i < nb; ++i )
        { drugRecord = drugsList.at(i);
          outXml   += "   <PreferedDrug>\n";
          outXml                  += C_BDM_DrugListRecord::serialize(&drugRecord,0,"          ", CGestIni::noConvert);
          QStringList grammarList  =  m_pC_BDM_Api->selectPosologieHitList(drugRecord.id(), drugRecord.id_type(), 20);
          outXml   += "   <GrammarList>\n";
          for ( int u=0; u<grammarList.size(); ++u )
              { outXml   += "      <ItemGrammar>\n";
                outXml   +=  grammarList[u].replace("\\n","\n");
                outXml   += "      </ItemGrammar>\n";
              }
          outXml   += "   </GrammarList>\n";
          outXml   += "   </PreferedDrug>\n\n";
        }
    outXml   += "</ExportPreferedsDrugsList>";
    CGestIni::CGestIni::Param_UpdateToDisk(fileName, outXml.toLatin1());
    /*................ version minimale qui peut suffire ..............................
    QString outXml = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n"
                     "<!-- Projet    : MedinTux                              -->\n"
                     "<!-- Module    : medicabase                            -->\n"
                     "<!-- Objet     : ExportPreferedsDrugsList              -->\n"
                     "<!-- guru mail : sevin-roland@medintux.org             -->\n"
                     "<!-- Copyright : (C) 2004-to-2013 and for the eternity -->\n";
    outXml   += "<ExportPreferedsDrugsList>\n";
    for (int i = 0; i < nb; ++i )
        { drugRecord = drugsList.at(i);
          outXml                  += C_BDM_DrugListRecord::serialize(&drugRecord,0,"          ", CGestIni::noConvert);
          QStringList grammarList  =  m_pC_BDM_Api->selectPosologieHitList(drugRecord.id(), drugRecord.id_type(), 20);
          for ( int u=0; u<grammarList.size(); ++u )
              { outXml   += "      <ItemGrammar>\n";
                outXml   +=  grammarList[u].replace("\\n","\n");
                outXml   += "      </ItemGrammar>\n";
              }
        }
    outXml   += "</ExportPreferedsDrugsList>";
    CGestIni::CGestIni::Param_UpdateToDisk(fileName, outXml.toLatin1());
    */
    return 1;
}

//--------------------------------------- import_prefered_products -------------------------------------------------------
int  C_MW_Prescription::import_prefered_products()
{ QString fileName = QFileDialog::getOpenFileName( this, tr("Export products list"),
                                                   m_lastExportPath,
                                                   tr("Xml files (*.XML *.xml)"));
 if (fileName.length()==0)   return 0;
 m_lastExportPath           = fileName;
 int            increased   = 0;
 QString          dataXml   = CGestIni::Param_UpdateFromDisk( fileName );
 QStringList grammarList    = CGestIni::getXmlDataList("ItemGrammar", dataXml);
 for ( int i=0; i<grammarList.size(); ++i )
     { QString grammar                = grammarList[i];
       int  nextPos                   = 0;
       QString drug_id                = CGestIni::getXmlData("gph_id",     grammar, &nextPos);
       QString drug_id_type           = CGestIni::getXmlData("gph_it",     grammar, &nextPos);
       QStringList grammarProductList = m_pC_BDM_Api->selectPosologieHitList(drug_id, drug_id_type, 100 );
       int ret                        = C_PosologieGrammar::isExistsThis_HitGrammar_InList(grammar, grammarProductList);
       if (ret==0)
          {  CGestIni::setXmlData("gph_pk", QString(""), grammar);
             m_pC_BDM_Api->increasePosologieHit(grammar, drug_id, drug_id_type , C_BDM_Api::FIX_HIT );
             ++increased;
          }
     }  // for ( int i=0; i<grammarList.size(); ++i )
 if (increased) fill_treeWidget_Favoris();
 return 0;
}
//--------------------------------------- add_Product_to_CustomProductsList -------------------------------------------------------
int  C_MW_Prescription::add_Product_to_CustomProductsList(const QString &userName, const QString &listName, QList < C_BDM_DrugListRecord > &drugsList)
{//............ activer eventuellement la liste ..........................
 if (m_pGUI->comboBox_customList->currentText() != listName) Slot_comboBox_customList_activated(listName);
 //............ ajouter la liste de drogue en base ........................
 if ( m_pC_BDM_Api->add_Product_to_CustomProductsList(userName, listName, drugsList) )
    { //.......... ajouter les nouveaux a la treewidget ......................
      QList < QTreeWidgetItem* >  itemsList;
      if (  fill_treeWidget_ProductsFromDrugList(m_pGUI->treeWidget_CustomDrugsList, drugsList, &itemsList))
         {
            set_toolBox_ListesProduits_item (  "page_CustomList");
            //............. selectionner les nouveaux arrivants ....................
            for (int i = 0; i < itemsList.size(); ++i)
                { QTreeWidgetItem *item = itemsList.at(i);
                  item->setSelected(true);
                  // m_pGUI->treeWidget_CustomDrugsList->setCurrentItem(item);
                }
            return itemsList.size();
         }
    }
 return 0;
}
//--------------------------------------- Slot_keyPressEvent_TreeWidgetProducts -------------------------------------------------------
void C_MW_Prescription::Slot_keyPressEvent_TreeWidgetProducts(QKeyEvent *event, int &callStandardEvent, C_DragQTreeWidget* pQTreeWidget)
{   QTreeWidgetItem * pQTreeWidgetItem = 0;
    switch(event->key())
      {case Qt::Key_Return:
       case Qt::Key_Enter:
             pQTreeWidgetItem = getSelectedListViewItem( pQTreeWidget );
             if (pQTreeWidgetItem==0 && pQTreeWidget->topLevelItemCount()==1 )
                {pQTreeWidgetItem = pQTreeWidget->topLevelItem(COL_PROD_NAME);
                 if (pQTreeWidgetItem) pQTreeWidgetItem->setSelected(true);
                }
             Slot_add_product_In_C_Frm_Prescription(pQTreeWidgetItem);
             callStandardEvent = 0;
             break;
      }  // end switch(event->key())
}
//--------------------------------------- Slot_treeWidget_Produits_currentItemChanged ---------------------------------------------------
void C_MW_Prescription::Slot_treeWidget_Produits_currentItemChanged ( QTreeWidgetItem *pQTreeWidgetItem , QTreeWidgetItem * previous    )
{
    Q_UNUSED(previous);
    if (pQTreeWidgetItem==0) return;
    QWidget *pTab_QWidget =  m_pGUI->tab_MainTabWidget->currentWidget();
    if (pTab_QWidget==0)  return;
    if (pTab_QWidget->objectName() != "tab_Monographie") return;
    QString cip       = pQTreeWidgetItem->text(COL_ID);
    m_pGUI->label_ListMedoc->setText(cip);
    show_ProductMonographie(C_BDM_DrugListRecord_From_QTreeWidgetProductsItem (pQTreeWidgetItem));
}
//--------------------------------------- Slot_treeWidget_Produits_itemClicked -------------------------------------------------------
void C_MW_Prescription::Slot_treeWidget_Produits_itemClicked ( QTreeWidgetItem *pQTreeWidgetItem , int c  )
{treeWidget_Produits_itemClicked (  pQTreeWidgetItem, c);
}
//--------------------------------------- Slot_treeWidget_Favoris_itemClicked --------------------------------------------------
void C_MW_Prescription::Slot_treeWidget_Favoris_itemClicked       ( QTreeWidgetItem *pQTreeWidgetItem , int c  )
{treeWidget_Produits_itemClicked (  pQTreeWidgetItem, c);
}
//--------------------------------------- Slot_treeWidget_CustomDrugsList_itemClicked -------------------------------------------------------
void C_MW_Prescription::Slot_treeWidget_CustomDrugsList_itemClicked ( QTreeWidgetItem *pQTreeWidgetItem , int c  )
{treeWidget_Produits_itemClicked (  pQTreeWidgetItem, c);
}
//--------------------------------------- Slot_treeWidget_Accessoires_itemClicked -------------------------------------------------------
void C_MW_Prescription::Slot_treeWidget_Accessoires_itemClicked ( QTreeWidgetItem *pQTreeWidgetItem , int c   )
{treeWidget_Produits_itemClicked (  pQTreeWidgetItem, c);
}
//--------------------------------------- Slot_treeWidget_PatientDrugs_itemClicked -------------------------------------------------------
void C_MW_Prescription::Slot_treeWidget_PatientDrugs_itemClicked ( QTreeWidgetItem *pQTreeWidgetItem , int c   )
{treeWidget_Produits_itemClicked (  pQTreeWidgetItem, c);
}

//--------------------------------------- setListATC_onCode -------------------------------
void C_MW_Prescription::setListATC_onCode ( const QString &atc )
{
    QString toFind                 = "";
    //...................... se positionner sur le code ATC .......................
    QTreeWidgetItemIterator it(m_pGUI->treeWidget_ATC);
    while (*it)
      { QTreeWidgetItem *pQTreeWidgetItem = (*it);
        toFind=pQTreeWidgetItem->text(1);
        if (toFind==atc)
           { m_pGUI->treeWidget_ATC->scrollToItem(pQTreeWidgetItem, QAbstractItemView::PositionAtCenter);   // QAbstractItemView::EnsureVisible
             pQTreeWidgetItem->setSelected(true);
             pQTreeWidgetItem->setExpanded(true);
           }
        else
           { pQTreeWidgetItem->setSelected(false);
             pQTreeWidgetItem->setExpanded(false);
           }
        ++it;
      }
    //............ replacer la scroll bar ATC a gauche ............................
    //             (car se centre au milieu apres selection item)
    m_pGUI->treeWidget_ATC->horizontalScrollBar()->setSliderPosition(0);
}

//--------------------------------------- treeWidget_Produits_itemClicked -------------------------------
void C_MW_Prescription::treeWidget_Produits_itemClicked ( QTreeWidgetItem * pQTreeWidgetItem, int /* c */  )
{   if (pQTreeWidgetItem==0)       return;

    QString cip       = pQTreeWidgetItem->text(COL_ID);
    QWidget *pQWIdget = m_pGUI->tab_MainTabWidget->currentWidget();
    QString tabName   = pQWIdget ? pQWIdget->objectName() : "";
    m_pGUI->label_ListMedoc->setText(cip);
    if (tabName      == "tab_bibliographie" )
       {
         getBiblioForItemEnCours(0, pQTreeWidgetItem);    //0 car  ne pas forcer (si deja en base recupere celui en base)
       }
    else if (tabName == "tab_Monographie" )
       { show_ProductMonographie(C_BDM_DrugListRecord_From_QTreeWidgetProductsItem (pQTreeWidgetItem));
       }

    pQWIdget = m_pGUI->toolBox_classifications->currentWidget();
    if (pQWIdget == m_pGUI->toolBox_classifications->toolWidgetByTabObjectName("page_Indications"))
       {  fillTreeWidgetIndications( cip );
       }
    else if (pQWIdget == m_pGUI->toolBox_classifications->toolWidgetByTabObjectName( "page_Composition"))
       {  fillTreeWidgetComposition(cip);
       }
    else if (pQWIdget == m_pGUI->toolBox_classifications->toolWidgetByTabObjectName( "page_ATC"))
       { QString atc =  pQTreeWidgetItem->text(COL_ATC);
         if (atc.length()==0 && cip.length() && m_pC_BDM_Api) atc = m_pC_BDM_Api->ATC_from_CIP(cip);
         setListATC_onCode ( atc);
       }
}

//------------------------------------ Slot_treeWidget_Produits_itemDoubleClicked --------------------------------------------------
 void C_MW_Prescription::Slot_treeWidget_Produits_itemDoubleClicked ( QTreeWidgetItem *pQTreeWidgetItem , int c )
 {   treeWidget_Produits_itemDoubleClicked (pQTreeWidgetItem , c  );
 }
//------------------------------------ Slot_treeWidget_CustomDrugsList_itemDoubleClicked -------------------------------------------------------
void C_MW_Prescription::Slot_treeWidget_CustomDrugsList_itemDoubleClicked ( QTreeWidgetItem *pQTreeWidgetItem , int c  )
{   treeWidget_Produits_itemDoubleClicked (pQTreeWidgetItem , c  );
}
//------------------------------------ Slot_treeWidget_Favoris_itemDoubleClicked --------------------------------------------------
 void C_MW_Prescription::Slot_treeWidget_Favoris_itemDoubleClicked ( QTreeWidgetItem *pQTreeWidgetItem , int  c )
 {   treeWidget_Produits_itemDoubleClicked (pQTreeWidgetItem , c  );
 }
//--------------------------------------- Slot_treeWidget_Accessoires_itemDoubleClicked -------------------------------------------------------
void C_MW_Prescription::Slot_treeWidget_Accessoires_itemDoubleClicked ( QTreeWidgetItem *pQTreeWidgetItem , int c  )
{    treeWidget_Produits_itemDoubleClicked (pQTreeWidgetItem , c  );
}
//--------------------------------------- Slot_treeWidget_PatientDrugs_itemDoubleClicked -------------------------------------------------------
void C_MW_Prescription::Slot_treeWidget_PatientDrugs_itemDoubleClicked ( QTreeWidgetItem *pQTreeWidgetItem , int c  )
{    treeWidget_Produits_itemDoubleClicked (pQTreeWidgetItem , c  );
}
//------------------------------------ treeWidget_Produits_itemDoubleClicked --------------------------------------------------
void C_MW_Prescription::treeWidget_Produits_itemDoubleClicked ( QTreeWidgetItem *pQTreeWidgetItem , int  )
{   if (pQTreeWidgetItem==0) return;
    QWidget                 *pTab_QWidget =  m_pGUI->tab_MainTabWidget->tabWidgetByTabObjectName(  "tab_Prescription");
    if ( pTab_QWidget == 0 ) pTab_QWidget =  m_pGUI->tab_MainTabWidget->activeTabWidgetByObjectName( "tab_Prescription");
    if ( pTab_QWidget == 0 ) return;
    m_pGUI->tab_MainTabWidget->setCurrentWidget ( pTab_QWidget );
    Slot_add_product_In_C_Frm_Prescription(pQTreeWidgetItem);
}

//------------------------------------ Slot_giveDragtreeWidget_ProduitsItem_List_Data -------------------------------
void C_MW_Prescription::Slot_giveDragtreeWidget_ProduitsItem_List_Data(QString &data, QTreeWidgetItem *)
{   giveDragtreeWidget_CustomDrugsListItem_List_Data(data, m_pGUI->treeWidget_Produits);
}
//------------------------------------ Slot_giveDragtreeWidget_FavorisItem_List_Data --------------------------------------------------
void C_MW_Prescription::Slot_giveDragtreeWidget_FavorisItem_List_Data   (QString &data, QTreeWidgetItem * )
{   giveDragtreeWidget_CustomDrugsListItem_List_Data(data, m_pGUI->treeWidget_Favoris);
}
//--------------------------------------- Slot_giveDragtreeWidget_CustomDrugsListItem_List_Data -------------------------------------------------------
void C_MW_Prescription::Slot_giveDragtreeWidget_CustomDrugsListItem_List_Data(QString &data, QTreeWidgetItem *)
{   giveDragtreeWidget_CustomDrugsListItem_List_Data(data, m_pGUI->treeWidget_CustomDrugsList);
}
//--------------------------------------- Slot_giveDragtreeWidget_AccessoiresItem_List_Data -------------------------------------------------------
void C_MW_Prescription::Slot_giveDragtreeWidget_AccessoiresItem_List_Data(QString &data, QTreeWidgetItem *)
{   giveDragtreeWidget_CustomDrugsListItem_List_Data(data, m_pGUI->treeWidget_Accessoires);
}
//--------------------------------------- Slot_giveDragtreeWidget_PatientDrugsItem_List_Data -------------------------------------------------------
void C_MW_Prescription::Slot_giveDragtreeWidget_PatientDrugsItem_List_Data(QString &data, QTreeWidgetItem *)
{   giveDragtreeWidget_CustomDrugsListItem_List_Data(data, m_pGUI->treeWidget_PatientDrugs);
}
//--------------------------------------- giveDragtreeWidget_CustomDrugsListItem_List_Data -------------------------------------------------------
void C_MW_Prescription::giveDragtreeWidget_CustomDrugsListItem_List_Data(QString &dataXML, QTreeWidget *pQTreeWidget)
{
    dataXML  = "<C_BDM_DrugListRecordArray>\n";
    QList < C_BDM_DrugListRecord > drugsList = makeDrugsListRecords_From_QTreeWidgetProducts(pQTreeWidget,  1);
    for (int i=0; i<drugsList.size(); ++i)
        {dataXML += drugsList[i].serialize(0,"   ");    // serialisation sans le header
        }
    dataXML += "</C_BDM_DrugListRecordArray>\n";
}

//--------------------------------------- Slot_pushButton_addCustomList -------------------------------------------------------
void C_MW_Prescription::Slot_pushButton_addCustomList(bool)
{ if (  m_pC_BDM_Api==0 )                                                return;
  //if (! m_pC_BDM_Api->isValid() )                                        return;
  QString userName = "BDM";
  bool ok;
  QString listName = QInputDialog::getText(this, tr("give your custom drugs list name"),
                                           tr("Give your new custom drugs list name:"), QLineEdit::Normal,
                                           "", &ok);
  if (ok && !listName.isEmpty())
    { m_pC_BDM_Api->createCustomProductsList(userName, listName);
      CApp::pCApp()->writeParam((CApp::pCApp()->applicationName() +"_TabManager").toLatin1(), "Custom Products List", listName.toLatin1());  // afin que fillCustomDrugListCombo se positionne dessus
      fillCustomDrugListCombo(userName);
    }
}

//--------------------------------------- Slot_pushButton_delCustomList -------------------------------------------------------
void C_MW_Prescription::Slot_pushButton_delCustomList(bool)
{ if (  m_pC_BDM_Api==0 )                                                return;
  //if (! m_pC_BDM_Api->isValid() )                                        return;
  QString userName = "BDM";
  QString listName = m_pGUI->comboBox_customList->currentText();
  int ret          = QMessageBox::warning( this, tr("Delete a custom drug list"),
                                           tr("The custom drug list <font color=#ff0000><b>'%1'</b></font> must be deleted.<br />"
                                           "Do you want this?").arg(listName),
                                           QMessageBox::Cancel
                                         | QMessageBox::Apply,
                                           QMessageBox::Cancel);
  if (ret==QMessageBox::Apply)
     { m_pC_BDM_Api->deleteCustomProductsList( userName, listName);
       fillCustomDrugListCombo(userName);
     }
}

//--------------------------------------- Slot_comboBox_customList_activated -------------------------------------------------------
void C_MW_Prescription::Slot_comboBox_customList_activated( const QString &listName)
{ setCustomDrugListComboOn_ListName("BDM", listName);
  CApp::pCApp()->writeParam((CApp::pCApp()->applicationName() +"_TabManager").toLatin1(), "Custom Products List", listName.toLatin1());
  CApp::pCApp()->updateIniParamToDisk();
}

//--------------------------------------- makeDrugsListRecords_From_QTreeWidgetProducts -------------------------------------------------------
QList < C_BDM_DrugListRecord > C_MW_Prescription::makeDrugsListRecords_From_QTreeWidgetProducts(QTreeWidget *pQTreeWidget, int selectMode /* = 0*/)
{//........... creer la liste des enregistrements a partir de la QTreeWidget des produits ....................
 int                             nb = pQTreeWidget->topLevelItemCount();
 QTreeWidgetItem  *pQTreeWidgetItem = 0;
 QList < C_BDM_DrugListRecord > drugsList;

 for (int i=0; i<nb; ++i)
    { pQTreeWidgetItem = pQTreeWidget->topLevelItem (i);
      if ( pQTreeWidgetItem )
         { if (selectMode)
              {
                if (pQTreeWidgetItem->isSelected()) drugsList.append( C_BDM_DrugListRecord_From_QTreeWidgetProductsItem (pQTreeWidgetItem) );
              }
           else
              { drugsList.append( C_BDM_DrugListRecord_From_QTreeWidgetProductsItem (pQTreeWidgetItem) );
              }
         }
    }
 return drugsList;
}

//--------------------------------------- C_BDM_DrugListRecord_From_QTreeWidgetProductsItem -------------------------------------------------------
/*
    C_BDM_DrugListRecord(QString id,                0
                         QString id_type,           1
                         QString commercialName,    2
                         QString dci_1Name,         3
                         QString dc2_1Name,         4
                         QString dc3_1Name,         5
                         QString owner,             6
                         QString lang,              7
                         QString ucd,               8
                         QString ucd_price,         9
                         QString atc,               10
                         QString generique,         11
                         QString statut             12
                        )

# define COL_NAME         0
# define COL_DCI_1        1
# define COL_DCI_2        2
# define COL_DCI_3        3
# define COL_PRIX         4
# define COL_ATC          5
# define COL_ID           6
# define COL_CIS          7
# define COL_ORG          8
# define COL_ID_TYP       9
# define COL_SPEC_KEY     10
# define COL_GENERIQUE    11
# define COL_STATUT       12
 */
C_BDM_DrugListRecord C_MW_Prescription::C_BDM_DrugListRecord_From_QTreeWidgetProductsItem (QTreeWidgetItem  *pQTreeWidgetItem)
{ return     C_BDM_DrugListRecord( pQTreeWidgetItem->text(COL_ID),         // 0  id
                                   pQTreeWidgetItem->text(COL_ID_TYP),     // 1  id_type
                                   pQTreeWidgetItem->text(COL_PROD_NAME),  // 2  commercialName
                                   pQTreeWidgetItem->text(COL_DCI_1),      // 3  dci_1Name
                                   pQTreeWidgetItem->text(COL_DCI_2),      // 4  dci_2Name
                                   pQTreeWidgetItem->text(COL_DCI_3),      // 5  dci_3Name
                                   pQTreeWidgetItem->text(COL_ORG),        // 6  owner
                                   "fr",                                   // 7  lang
                                   pQTreeWidgetItem->text(COL_NB_UP),      // 8  ucd
                                   pQTreeWidgetItem->text(COL_PRIX),       // 9  ucd_price
                                   pQTreeWidgetItem->text(COL_ATC),        // 10 atc
                                   pQTreeWidgetItem->text(COL_GENERIQUE),  // 11 generique
                                   pQTreeWidgetItem->text(COL_STATUT)      // 12 statut
                                 );
}

//--------------------------------------- setCustomDrugListComboOn_ListName -------------------------------------------------------
void C_MW_Prescription::setCustomDrugListComboOn_ListName(const QString &_userName, const QString &listName)
{int index = m_pGUI->comboBox_customList->findText ( listName );
 if (index != -1)
    {changeCurrentCustomList(_userName, listName);
     m_pGUI->comboBox_customList->setCurrentIndex(index);
     index = m_pGUI->toolBox_ListesProduits->toolWidgetIndexByTabObjectName("page_CustomList");
     m_pGUI->toolBox_ListesProduits->setItemText ( index, tr("Products list for %1").arg(listName) );
    }
}

//--------------------------------------- changeCurrentCustomList -------------------------------------------------------
void C_MW_Prescription::changeCurrentCustomList(const QString &_userName, const QString &listName)
{
   QList < C_BDM_DrugListRecord > drugsList = m_pC_BDM_Api->selectCustomProductsList(_userName,  listName,"", C_BDM_PluginI::no_filter);
   m_pGUI->treeWidget_CustomDrugsList->clear();
   fill_treeWidget_ProductsFromDrugList(m_pGUI->treeWidget_CustomDrugsList, drugsList);
}

//--------------------------------------- fillCustomDrugListCombo -------------------------------------------------------
void C_MW_Prescription::fillCustomDrugListCombo(const QString &_userName)
{ if (  m_pC_BDM_Api==0 )                                                return;
  if (! m_pC_BDM_Api->isValid() )                                        return;

#ifdef Q_OS_MAC
   QString userName     = _userName;
#endif
#ifdef Q_WS_WIN
   QString userName     = _userName.toLower();
#endif
#ifdef Q_OS_LINUX
   QString userName     = _userName;
#endif
  QStringList customList = m_pC_BDM_Api->getListCustomProductsList(userName);
  QString listName       = "";
  m_pGUI->comboBox_customList->clear();
  if ( customList.size() )
     { m_pGUI->comboBox_customList->insertItems(0, customList);
       listName = CApp::pCApp()->readUniqueParam((CApp::pCApp()->applicationName() +"_TabManager").toLatin1(), "Custom Products List");
       if (listName.length()==0 || m_pGUI->comboBox_customList->findText ( listName ) == -1)
           { listName = customList.at(0);
             CApp::pCApp()->writeParam((CApp::pCApp()->applicationName() +"_TabManager").toLatin1(), "Custom Products List", listName.toLatin1());
           }
       setCustomDrugListComboOn_ListName( _userName, listName);
     }
}

//--------------------------------------- fillTreeWidgetComposition -------------------------------------------------------
void C_MW_Prescription::fillTreeWidgetComposition(const QString &cip)
{QWidget *pQWidget = m_pGUI->toolBox_classifications->toolWidgetByTabObjectName( "page_Composition");
 if (  pQWidget != m_pGUI->toolBox_classifications->currentWidget () )  return;           // si pas la page courante cela ne la concerne pas
 if (  m_pC_BDM_Api==0 )                                                return;
 if (! m_pC_BDM_Api->isValid() )                                        return;
 QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
 m_pGUI->treeWidget_Composition->clear();
 //.................. composition DCI .....................................................................
 //              C_InterResultPair.Code1()     is the quantity
 //              C_InterResultPair.CodeTyp1()  is the quantity unit
 //              C_InterResultPair.Term1()     is the composant name (DCI)
 QList<C_CompositionItem> compositionList = m_pC_BDM_Api->getCompositionFromCIP(cip);
 QTreeWidgetItem                *item     = 0;
 C_CompositionItem    compositionItem;
 for (int i = 0; i < compositionList.size(); ++i)
     { compositionItem   =  compositionList.at(i);
       item =   new QTreeWidgetItem (m_pGUI->treeWidget_Composition);
       item->setText(0,compositionItem.name());          // libelle
       item->setText(1,compositionItem.amount());        // quantite
       item->setText(2,compositionItem.unit());          // unite
     }
 QApplication::restoreOverrideCursor();
}
//--------------------------------------- Slot_treeWidget_Composition_itemClicked -------------------------------------------------------
void C_MW_Prescription::Slot_treeWidget_Composition_itemClicked ( QTreeWidgetItem *pQTreeWidgetItem , int   )
{   if ( pQTreeWidgetItem == 0 ) return;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QList < C_BDM_DrugListRecord >  drugsList;
    QTime                        debTime     = QTime::currentTime();
    m_pC_BDM_Api->outMessage(tr("-------------- '%1' SELECT PRODUCTS FROM COMPOSITION --------------").arg(m_pC_BDM_Api->plugin_name()));
    drugsList += m_pC_BDM_Api->selectProductsList ("%"+pQTreeWidgetItem->text(0)//.remove("BASE").
                                                                                 .remove("ACIDE")
                                                                                 .remove("ANHYDRE")
                                                                                 .remove("TRIHYDRATE")
                                                                                 .remove("MONOHYDRATE")
                                                                                 .remove("EXTRAIT")
                                                                                 .remove("SODIQUE")
                                                                                 .remove("PULVERISE").trimmed(),
                                                   C_BDM_PluginI::dca_filter  , 7000 );
    m_pGUI->treeWidget_Produits->clear();
    int nb = fill_treeWidget_ProductsFromDrugList(m_pGUI->treeWidget_Produits, drugsList);
    if ( nb )
       { set_toolBox_ListesProduits_item ( "page_Produits" );
       }
    m_pGUI->label_ListMedoc->setText(tr("%1 sur %2 produit(s)").arg(QString::number(nb),QString::number(m_pC_BDM_Api->drugsList_Size())));
    m_pC_BDM_Api->outMessage(tr("Plugin '%1' Slot_treeWidget_Composition_itemClicked() en : %2").arg(m_pC_BDM_Api->plugin_name(),   QTime (0, 0, 0, 0 ).addMSecs(debTime.msecsTo(QTime::currentTime())).toString("mm:ss:zzz") ));
    QApplication::restoreOverrideCursor();
}

//--------------------------------------- fill_treeWidget_ProductsFromDrugList -------------------------------------------------------
int C_MW_Prescription::fill_treeWidget_ProductsFromDrugList(QTreeWidget *pQTreeWidget, const QList < C_BDM_DrugListRecord >  &drugsList, QList < QTreeWidgetItem* >  *pItemssList /* = 0 */)
{int                    nb           = drugsList.size();
 QTreeWidgetItem       *item         = 0;
 C_BDM_DrugListRecord   drugRecord;
 QIcon datasempIcone    = Theme::getIcon("medicatux/VidalItem.png");
 QIcon theriaqueIcone   = Theme::getIcon("medicatux/TheriaqueItem.png");
 QIcon freeDrugIcone    = Theme::getIcon("medicatux/edit.png");
 QIcon accessoireIcone  = Theme::getIcon("medicatux/toolbox_accessoires.png");
 QIcon historiqueIcone  = Theme::getIcon("medicatux/masculin.png");
 QIcon ttt_fondIcone    = Theme::getIcon("medicatux/ttt_fondIcone.png");
 QIcon favoricip        = Theme::getIcon("medicatux/favoricip.png");
 if (PATIENT && PATIENT->get_sexe().left(1).toUpper()=="F") historiqueIcone = Theme::getIcon("medicatux/feminin.png");

 for (int i = 0; i < nb; ++i )
    { drugRecord = drugsList.at(i);
      item =   new QTreeWidgetItem (pQTreeWidget);
      if (item)  //  paranoiac attitude
         { item->setText(0,drugRecord.commercialName());   // libelle
           item->setText(1,drugRecord.dci_1Name());        // DCI_1
           item->setText(2,drugRecord.dci_2Name());        // DCI_2
           item->setText(3,drugRecord.dci_3Name());        // DCI_3
           item->setText(4,drugRecord.ucd_price());        // Prix
           item->setText(5,drugRecord.atc());              // ATC
           item->setText(6,drugRecord.id());               // ID CIP
           item->setText(8,drugRecord.owner());            // PROVENANCE
           item->setText(9,drugRecord.id_type());          // ID Type
           item->setText(11,drugRecord.generique());       // generique   G R ou S
           item->setText(12,drugRecord.statut());          // statut  Ville Hopital VH ou H
           if (pItemssList) pItemssList->append(item);

           if (drugRecord.owner()=="DS_A")       item->setIcon(0,accessoireIcone);
           else if (drugRecord.owner()=="DS")    item->setIcon(0,datasempIcone);
           else if (drugRecord.owner()=="TH")    item->setIcon(0,theriaqueIcone);
           else if (drugRecord.owner()=="CTX_I") item->setIcon(0,historiqueIcone);
           else if (drugRecord.owner()=="CTX_F") item->setIcon(0,ttt_fondIcone);
           else if (drugRecord.id_type()=="CIP") item->setIcon(0,favoricip);
           else                                  item->setIcon(0,freeDrugIcone);
           if ( drugRecord.lang().startsWith("DT=") )
              { if ( QDateTime::currentDateTime() > QDateTime::fromString(drugRecord.lang().mid(3),"dd-MM-yyyy hh:mm:ss")  )
                   { item->setBackground ( 0, QBrush(Qt::BDiagPattern) );
                   }
              }
        } // end if (item)  //  paranoiac attitude
    }
 return nb;
}


//--------------------------------------- Slot_treeWidget_Indications_itemClicked -------------------------------------------------------
#define INDIC_COL_ICONE   0
#define INDIC_COL_LIBELLE 0
#define INDIC_COL_SMR     1
#define INDIC_COL_DATE    2
#define INDIC_COL_COMMENT 3
#define INDIC_COL_CODE    4
#define INDIC_COL_TYPE    5

void C_MW_Prescription::Slot_treeWidget_Indications_itemClicked ( QTreeWidgetItem *pQTreeWidgetItem , int c   )
{if ( pQTreeWidgetItem == 0 ) return;
 //........................... si ASMR SMR .............................
 //                           afficher la fenetre dinfos
 QString asmr = pQTreeWidgetItem->text(INDIC_COL_SMR);  // peut contenir effectivement A ou S + niveau ASMR ou SMR   ou prix de remboursement si il existe (les deux ensembles impossibles)
 if ( asmr.length() && c==1)
    {
     if ( asmr.startsWith("S")||asmr.startsWith("A") )   // niveau asm smr
        { QString text = "";
          QList <C_BDM_IndicationRecord> indic_list = m_pC_BDM_Api->getIndicationsFromCIP( pQTreeWidgetItem->text(INDIC_COL_CIP), C_BDM_PluginI::ASMR|C_BDM_PluginI::SMR );
          if (indic_list.size()==0) return;
          text += tr("<p align=\"center\"><span style=\"font-weight:400;font-size:9.00pt;color:#791a1a\">Avis de la commission de transparence<br/>"
                     "concernant les SMR et ASM du <br/>"
                     "<b><u>%1</u></b><br/></span></p><hr /><span style=\"font-weight:400;font-size:9.00pt;color:#000000\">").arg(m_pC_BDM_Api->CIP_to_DrugName(indic_list[0].cip()));
          for ( int i=0; i<indic_list.size(); ++i )
              { QString level_type = indic_list[i].levelTyp();
                if ( level_type=="A" )
                    text += tr("<b><u>ASMR</b></u> en date du <b><u>%1</u> Niveau : </b> %2<br/>").arg(indic_list[i].date(), indic_list[i].levelLibelle());
                else if ( level_type=="S" )
                    text += tr("<b><u>SMR</b></u> en date du <b><u>%1</u> Niveau : </b> %2<br/>").arg(indic_list[i].date(), indic_list[i].levelLibelle());
                else
                    continue;
                text += tr("%1<br/>").arg(indic_list[i].indication());
                // if (indic_list[i].refundLink())
                // qDebug() << " indic_list : " << indic_list[i].refundLink();
                //text += tr("%1<br/>").arg(indic_list[i].levelLibelle());
                //text += tr("%1<br/>").arg(indic_list[i].descriptif());
                text += "<hr />";
              }
          text += "</span>";
          QPoint pos = QCursor::pos();
          pos = mapFromGlobal(pos);
          int deltaH = this->height()-(pos.y()+300);
          pos.setY( pos.x()+deltaH );
          pos.setY( 0 );
          if ( m_pC_PopupDial_Info==0 )
             { m_pC_PopupDial_Info = new C_PopupDial_Info(this,text,QColor("#fffeeb"),QRect(pos,QSize(400,0)),QRect(pos,QSize(400,300)));
               m_pC_PopupDial_Info->show();
               m_pC_PopupDial_Info->setToDeployRect();
             }
          else
             { m_pC_PopupDial_Info->setText(text);
               m_pC_PopupDial_Info->setToDeployRect();
             }
          return;
        }  // if ( asmr.startsWith("S")||asmr.startsWith("A") )
     else
        { //...........recuperer la liste des indications remboursables pour ce code indication ....................
          QList <C_BDM_IndicationRecord> indic_list = m_pC_BDM_Api->getRefundedIndications( pQTreeWidgetItem->text(INDIC_COL_CODE),   pQTreeWidgetItem->text(INDIC_COL_TYPE) );
          if (indic_list.size()==0) return;
          //............. creer la liste des CIP a partir des resultats ........
          QStringList   cipList;
          for ( int i=0; i<indic_list.size(); ++i )
              {  cipList << indic_list[i].cip();
              }
          //............. selectionner les produits correspondant a la liste des CIP ........
          selectProductsFromCipList (cipList);
        }
    }
 //........................... INDICATION .............................
 //                            afficher la liste des produits pour
 if (c==1)
    {
    }
 else
    { //...........recuperer la liste des indications pour ce code indication ....................
      QStringList                     cipList  = m_pC_BDM_Api->getCIPFromIndication( pQTreeWidgetItem->text(INDIC_COL_CODE), pQTreeWidgetItem->text(INDIC_COL_TYPE) );
      //............. selectionner les produits correspondant a la liste des CIP ........
      selectProductsFromCipList (cipList);
    }
}

//--------------------------------------- selectProductsFromCipList -------------------------------------------------------
void C_MW_Prescription::selectProductsFromCipList (const QStringList &cipList)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QList < C_BDM_DrugListRecord >  drugsList;
    QTime                        debTime     = QTime::currentTime();
    // QStringList                     cipList  = m_pC_BDM_Api->getCIPFromIndication( pQTreeWidgetItem->text(2), pQTreeWidgetItem->text(3) );
    m_pC_BDM_Api->outMessage(tr("-------------- '%1' SELECT PRODUCTS FROM CIP LIST --------------").arg(m_pC_BDM_Api->plugin_name()));
    m_pC_BDM_Api->outMessage(tr("Plugin '%1' Slot_treeWidget_Indications_itemClicked() selectProductsFromCipList() en : %2").arg(m_pC_BDM_Api->plugin_name(),   QTime (0, 0, 0, 0 ).addMSecs(debTime.msecsTo(QTime::currentTime())).toString("mm:ss:zzz") ));
    debTime                                  = QTime::currentTime();
    QString cipListStr = cipList.join("','");
    if (cipList.size())
        {  drugsList += m_pC_BDM_Api->selectProductsList ("'"+cipListStr+"'", C_BDM_PluginI::cip_list  , 7000 );
        }
    m_pC_BDM_Api->outMessage(tr("Plugin '%1' Slot_treeWidget_Indications_itemClicked() selectProductsFromCipList() en : %2").arg(m_pC_BDM_Api->plugin_name(),   QTime (0, 0, 0, 0 ).addMSecs(debTime.msecsTo(QTime::currentTime())).toString("mm:ss:zzz") ));
    debTime                                  = QTime::currentTime();
    m_pGUI->treeWidget_Produits->clear();
    int nb = fill_treeWidget_ProductsFromDrugList(m_pGUI->treeWidget_Produits, drugsList);
    if (nb)
       {  set_toolBox_ListesProduits_item ( "page_Produits" );
       }
    m_pGUI->label_ListMedoc->setText(tr("%1 sur %2 produit(s)").arg(QString::number(nb),QString::number(m_pC_BDM_Api->drugsList_Size())));
    m_pC_BDM_Api->outMessage(tr("Plugin '%1' selectProductsFromCipList() fill treeWidget_Produits en : %2").arg(m_pC_BDM_Api->plugin_name(),   QTime (0, 0, 0, 0 ).addMSecs(debTime.msecsTo(QTime::currentTime())).toString("mm:ss:zzz") ));
    QApplication::restoreOverrideCursor();
}

//--------------------------------------- Slot_lineEdit_Indications_textChanged -------------------------------------------------------
void C_MW_Prescription::Slot_lineEdit_Indications_textChanged (const QString &text)
{    if (m_pC_BDM_Api==0||text.length()<=2) return;
     QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
     QList <C_BDM_IndicationRecord> retList = m_pC_BDM_Api->getIndicationsFromText(text,0);
     QApplication::restoreOverrideCursor();
     if (retList.size()==0) return;
     fillTreeWidgetIndications(retList);
}

//--------------------------------------- fillTreeWidgetIndications -------------------------------------------------------
void C_MW_Prescription::fillTreeWidgetIndications(const QString &cip)
{   if (m_pC_BDM_Api==0) return;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    //............ rajouter les indications liees au produit en entree ..................
    QList <C_BDM_IndicationRecord>    retList = m_pC_BDM_Api->getIndicationsFromCIP( cip, C_BDM_PluginI::ALL_INDIC | C_BDM_PluginI::REFUND );
    QApplication::restoreOverrideCursor();
    //if (retList.size()==0) return;
    fillTreeWidgetIndications(retList);
}

//--------------------------------------- fillTreeWidgetIndications -------------------------------------------------------
#define INDIC_COL_ICONE    0
#define INDIC_COL_LIBELLE  0
#define INDIC_COL_SMR      1
#define INDIC_COL_DATE     2
#define INDIC_COL_COMMENT  3
#define INDIC_COL_CODE     4
#define INDIC_COL_TYPE     5
#define INDIC_COL_REFUNDED 6
#define INDIC_COL_CIP      7
void C_MW_Prescription::fillTreeWidgetIndications(QList <C_BDM_IndicationRecord> &retList)
{ QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  m_pGUI->treeWidget_Indications->clear();
  //............ rajouter les indications liees au terrain du patient ..................
   // fillTreeWidgetIndicationsFromPatientContext();
   m_pC_BDM_Api->fillTreeWidgetIndicationsFromPatientContext(m_pGUI->treeWidget_Indications,Theme::getIcon("medicatux/terrain_indic.png"));
  //............ rajouter les indications liees a al liste en entree ..................
 QTreeWidgetItem                    *item  = 0;
 C_LevelItemDelegate *pC_LevelItemDelegate = 0;
 // int pageIndex                             = toolWidgetIndexByTabObjectName(m_pGUI->toolBox_classifications, "page_Indications");
 // m_pGUI->toolBox_classifications->setItemText ( pageIndex, tr("Indications : ") + name.left(12)+"..." );
 if (retList.size())
    { for (int i = 0; i < retList.size(); ++i )
          { C_BDM_IndicationRecord indicationRecord   = retList.at(i);
            QString codeTyp                           = indicationRecord.codeTyp();
            QString refunded                          = indicationRecord.refundLink();
            item                                      = new QTreeWidgetItem (m_pGUI->treeWidget_Indications);
            pC_LevelItemDelegate                      = new C_LevelItemDelegate(m_pGUI->treeWidget_Indications);
            m_pGUI->treeWidget_Indications->setItemDelegate( pC_LevelItemDelegate );
            if (indicationRecord.refundLink().length())
                item->setText(INDIC_COL_SMR,indicationRecord.refundLink());    // un tarif commence par un chiffre et ne sera pas interprete comme un progressBar
            else
                item->setText(INDIC_COL_SMR,indicationRecord.level());         // un niveau sera dessine comme progressBar tel car commence par une lettre
            item->setText(INDIC_COL_LIBELLE,indicationRecord.indication());    // libelle
            item->setText(INDIC_COL_CODE,indicationRecord.code());             // code
            item->setText(INDIC_COL_TYPE,codeTyp);                             // codeTyp
            item->setText(INDIC_COL_CIP,indicationRecord.cip());               // code cip
            item->setText(INDIC_COL_COMMENT,indicationRecord.descriptif());    // commentaire
            item->setText(INDIC_COL_REFUNDED,refunded);                        // tarif si indication remboursable
            if ( refunded.length() )
               { QString tooltip_text = tr("<html><head/><body>Le&nbsp;produit&nbsp;<b>%1</b>&nbsp;est&nbsp;remboursable&nbsp;pour&nbsp;cette&nbsp;indication<br/>"
                                           "\303\240 &nbsp;hauteur&nbsp;de&nbsp;<b>%2&nbsp;euros</b><br/>"
                                           "Cliquez&nbsp;pour&nbsp;s\303\251lectionner&nbsp;tous&nbsp;ceux&nbsp;remboursables&nbsp;pour&nbsp;cette&nbsp;indication.</html></body>")
                                        .arg(indicationRecord.descriptif().replace(" ","&nbsp;"),refunded);
                 item->setToolTip(1,tooltip_text );
               }
             item->setToolTip(0,indicationRecord.indication()+"<br/>"+indicationRecord.descriptif());

            if (codeTyp=="CIM")
               {item->setIcon(INDIC_COL_ICONE,Theme::getIcon("medicatux/CimItem.png"));
               }
            else if (codeTyp=="DS_I")
               {item->setIcon(INDIC_COL_ICONE,Theme::getIcon("medicatux/VidalItem.png"));
               }
            else if (codeTyp=="TH_I")
               {item->setIcon(INDIC_COL_ICONE,Theme::getIcon("medicatux/TheriaqueItem.png"));
               }
            else if (codeTyp=="DS_S"||codeTyp=="DS_A")
               { item->setIcon(INDIC_COL_ICONE,Theme::getIcon("medicatux/HasItem.png"));
                 QString tooltip_text = tr("<html><head/><body>Cliquez&nbsp;pour&nbsp;le&nbsp;d\303\251tail&nbsp;des&nbsp;informations.</html></body>");
                 item->setToolTip(1,tooltip_text );
               }

          }
    }
 QApplication::restoreOverrideCursor();
}
//--------------------------------------- fillTreeWidgetIndicationsFromPatientContext -------------------------------------------------------
void C_MW_Prescription::fillTreeWidgetIndicationsFromPatientContext()
{ QTreeWidgetItem                    *item  = 0;
  if (PATIENT)
    {QList <C_LifeEvent> eventLifeList = PATIENT->lifeEventList();
     QIcon item_icone = Theme::getIcon("medicatux/terrain_indic.png");
     for (int i = 0; i < eventLifeList.size(); ++i)
         {C_LifeEvent lifeEvent = eventLifeList.at(i);
          QString codeTyp       = lifeEvent.codeTyp();
          if ( codeTyp=="CIM" )
             { item   = new QTreeWidgetItem (m_pGUI->treeWidget_Indications);
               item->setIcon(0,item_icone);
               item->setText(0,lifeEvent.libelle());   // libelle
               item->setText(1,"");                    // niveau y en a pas
               item->setText(2,lifeEvent.code());      // code
               item->setText(3,codeTyp);               // code type
               item->setText(5,lifeEvent.comment());   // commentaire
             }
         }
    }
}

//--------------------------------------- Slot_On_pushButton_GeneralFontChoice -------------------------------------------------------
void C_MW_Prescription::Slot_On_pushButton_GeneralFontChoice(bool)
{   QString pixSize    = "11";
    QString family     = "Arial";
    QFont   initialFnt = this->font();

#ifdef Q_OS_MAC
    QString section    = "medicatux MacFontParam";
#endif
#ifdef Q_WS_WIN
    QString section    = "medicatux WinFontParam";
#endif
#ifdef Q_OS_LINUX
    QString section    = "medicatux LinFontParam";
#endif
    if (CApp::pCApp()->readParam(section.toLatin1(), "interface", &pixSize, &family)==QString::null)  // zero = pas d'erreur
       { initialFnt.setFamily(family);
         initialFnt.setPixelSize(qMax(8,qMin(pixSize.toInt(),40)));
       }
    bool   ok;
    QFont  font = QFontDialog::getFont(&ok, initialFnt, this);
    if (ok)
       { this->setFont(font);
        QFontMetrics fm( font );
        int pixSize = fm.leading()+fm.height();
        CApp::pCApp()->writeParam(section.toLatin1(), "interface", QString::number(pixSize).toLatin1(), font.family().toLatin1());
       }
}

//--------------------------------------- Slot_On_pushButton_PosoEditorFontChoice ----------------------------------------------------------
void C_MW_Prescription::Slot_On_pushButton_PosoEditorFontChoice(bool)
{   QString pixSize    = "11";
    QString family     = "Arial";
    QFont   initialFnt = this->font();
#ifdef Q_OS_MAC
    QString section    = "medicatux MacFontParam";
#endif
#ifdef Q_WS_WIN
    QString section    = "medicatux WinFontParam";
#endif
#ifdef Q_OS_LINUX
    QString section    = "medicatux LinFontParam";
#endif
    if (CApp::pCApp()->readParam(section.toLatin1(), "posologieEditor", &pixSize, &family)==QString::null)  // zero = pas d'erreur
       { initialFnt.setFamily(family);
         initialFnt.setPixelSize(qMax(8,qMin(pixSize.toInt(),40)));
       }
    bool   ok;
    QFont  font = QFontDialog::getFont(&ok, initialFnt, this);
    if (ok)
       { if (m_pC_Frm_Prescription) m_pC_Frm_Prescription->setFont(font);
         QFontMetrics fm( font );
         int pixSize = fm.leading()+fm.height();
         CApp::pCApp()->writeParam(section.toLatin1(), "posologieEditor", QString::number(pixSize).toLatin1(), font.family().toLatin1());
       }
}

//--------------------------------------- recordPositions -------------------------------------------------------------------------------
void C_MW_Prescription::recordPositions()
{   QRect rect = this->geometry();
    int x  =  rect.x();
    int y  =  rect.y();
    int w  =  rect.width();
    int h  =  rect.height();
    qDebug() << "Save X: " << QString::number(x) << " Y: " << QString::number(y) << " W: " << QString::number(w) << " H: " << QString::number(h);
    //................. enregistrer position fenetre principale .........................
    CApp::pCApp()->writeParam("medicatux geometry", "MainWindow",         QString::number(x).toLatin1(), QString::number(y).toLatin1(), QString::number(w).toLatin1(),  QString::number(h).toLatin1());
    CApp::pCApp()->writeParam("medicatux geometry", "MainWindow_state",   this->saveState().toHex());

    CApp::pCApp()->writeParam("medicatux session" , "drug list limit", QString::number(m_drugListMax).toLatin1());

    //................. enregistrer spliters .........................
    QList<int>  list = m_pGUI->splitter_g->sizes();
    CApp::pCApp()->writeParam("medicatux geometry", "splitter_g", QString::number(list[0]).toLatin1(), QString::number(list[1]).toLatin1());
    list = m_pGUI->splitter_d->sizes();
    CApp::pCApp()->writeParam("medicatux geometry", "splitter_d", QString::number(list[0]).toLatin1(), QString::number(list[1]).toLatin1());
    //................... enregistrer la position des tabulations ...................
    writeMainTabWidgetConfig();
    //................... enregistrer la fenetre d'affichage des interactions ...................
    // le faire dans le C_MW_Prescription::event() QEvent::Close car ici m_pC_Frm_Prescription est deja mort
    //................... enregistrer les headers de la liste des medocs ...................
    /* marche pas ??!!
    QHeaderView *pQHeaderView = m_pGUI->treeWidget_Produits->header ();
    QByteArray             ba = pQHeaderView->saveState();
    CApp::pCApp()->writeParam("medicatux geometry", "list products header", ba.toHex());
    */
    //................... enregistrer la config listview des medocs ..................
#ifdef Q_OS_MAC
    QString sufix     = "_mac.dsp";
#endif
#ifdef Q_WS_WIN
    QString sufix     = "_win.dsp";
#endif
#ifdef Q_OS_LINUX
    QString sufix     = "_lin.dsp";
#endif
    save_treeWidgetState(sufix, m_pGUI->treeWidget_Produits);
    save_treeWidgetState(sufix, m_pGUI->treeWidget_Favoris);
    save_treeWidgetState(sufix, m_pGUI->treeWidget_Indications);
    save_treeWidgetState(sufix, m_pGUI->treeWidget_Composition);
    save_treeWidgetState(sufix, m_pGUI->treeWidget_ATC);
    save_treeWidgetState(sufix, m_pGUI->treeWidget_CustomDrugsList);
    save_treeWidgetState(sufix, m_pGUI->treeWidget_Accessoires);
    save_treeWidgetState(sufix, m_pGUI->treeWidget_PatientDrugs);
}
//--------------------------------------- Slot_sortIndicator_Changed ----------------------------------------------------------
void C_MW_Prescription::Slot_sortIndicator_Changed( int logical_index, Qt::SortOrder order)
{   if ( logical_index != COL_PROD_NAME ) return;
    if ( USER == 0)                       return;
    CApp::pCApp()->writeParam( "medicatux alphatbetic order" , "alphatbetic_order_" + USER->get_Login().toLatin1(),  QString::number( order ).toLatin1() );
}

//--------------------------------------- save_treeWidgetState ----------------------------------------------------------
void C_MW_Prescription::save_treeWidgetState(const QString &sufix, QTreeWidget *pQTreeWidget)
{   QString nomFicDPS = CApp::pCApp()->pathAppli()+"Ressources/list_"+pQTreeWidget->objectName()+"_header" + sufix;
    QFile file( nomFicDPS );
    file.setFileName( nomFicDPS );
    if ( file.open( QIODevice::WriteOnly ) )
       { file. write (pQTreeWidget->header()->saveState());
         file.close();
       }
}
//--------------------------------------- restore_treeWidgetState ----------------------------------------------------------
void C_MW_Prescription::restore_treeWidgetState(const QString &sufix, QTreeWidget *pQTreeWidget)
{   QString nomFicDPS = CApp::pCApp()->pathAppli()+"Ressources/list_"+pQTreeWidget->objectName()+"_header" + sufix;
    if ( QFile::exists(nomFicDPS) )
       { QFile file( nomFicDPS );
         if ( file.open( QIODevice::ReadOnly ) )
            { pQTreeWidget->header()->restoreState(file.readAll());
              file.close();
            }
       }
}
//--------------------------------------- setPositionsFromIniFile ----------------------------------------------------------
void C_MW_Prescription::setPositionsFromIniFile()
{   QString val1, val2, val3, val4;
    if (CApp::pCApp()->readParam("medicatux geometry", "MainWindow", &val1, &val2, &val3, &val4)==QString::null)  // zero = pas d'erreur
       { this->setGeometry(val1.toInt(),val2.toInt(),val3.toInt(),val4.toInt());
       }

    QList <int> list;
    if (CApp::pCApp()->readParam("medicatux geometry", "splitter_d", &val1, &val2)==QString::null)  // zero = pas d'erreur
       {list.clear();
        list.append(val1.toInt());
        list.append(val2.toInt());
        m_pGUI->splitter_d->setSizes(list);
      }
    if (CApp::pCApp()->readParam("medicatux geometry", "splitter_g", &val1, &val2)==QString::null)  // zero = pas d'erreur
       {list.append(val1.toInt());
        list.append(val2.toInt());
        m_pGUI->splitter_g->setSizes(list);
      }
    //...... charge la config listview des medocs .................
#ifdef Q_OS_MAC
    QString sufix     = "_mac.dsp";
#endif
#ifdef Q_WS_WIN
    QString sufix     = "_win.dsp";
#endif
#ifdef Q_OS_LINUX
    QString sufix     = "_lin.dsp";
#endif
    restore_treeWidgetState(sufix, m_pGUI->treeWidget_Produits);
    restore_treeWidgetState(sufix, m_pGUI->treeWidget_Favoris);
    restore_treeWidgetState(sufix, m_pGUI->treeWidget_Indications);
    restore_treeWidgetState(sufix, m_pGUI->treeWidget_Composition);
    restore_treeWidgetState(sufix, m_pGUI->treeWidget_ATC);
    restore_treeWidgetState(sufix, m_pGUI->treeWidget_CustomDrugsList);
    restore_treeWidgetState(sufix, m_pGUI->treeWidget_Accessoires);
    restore_treeWidgetState(sufix, m_pGUI->treeWidget_PatientDrugs);
    m_pGUI->treeWidget_Indications->header()->showSection(3);
    m_pGUI->treeWidget_Indications->header()->showSection(4);
    //.......... forcer ces etats ..........................
    m_pGUI->treeWidget_PatientDrugs->setSortingEnabled(true);
    m_pGUI->treeWidget_Accessoires->setSortingEnabled(true);
    m_pGUI->treeWidget_CustomDrugsList->setSortingEnabled(true);
    m_pGUI->treeWidget_Favoris->setSortingEnabled(true);
    m_pGUI->treeWidget_Produits->setSortingEnabled(true);

    // setFontFromIniFile(); deja fait lors creation C_Frm_Prescription
    if ( USER )
       {  QString sOrder = CApp::pCApp()->readUniqueParam("medicatux alphatbetic order" , "alphatbetic_order_" + USER->get_Login() );
          if ( sOrder.length() )
             {  QHeaderView *pQHeaderView = m_pGUI->treeWidget_Produits->header();
                disconnect( m_pGUI->treeWidget_Produits->header(),    SIGNAL(sortIndicatorChanged ( int , Qt::SortOrder  )),      this, SLOT(Slot_sortIndicator_Changed( int , Qt::SortOrder )) );
                pQHeaderView->setSortIndicator(COL_PROD_NAME, (Qt::SortOrder) sOrder.toInt());
                connect( m_pGUI->treeWidget_Produits->header(),       SIGNAL(sortIndicatorChanged ( int , Qt::SortOrder  )),      this, SLOT(Slot_sortIndicator_Changed( int , Qt::SortOrder )) );
             }
       }
    setMainTabWidgetConfig();
    //.................. restorer l'espace de travail ....................................
    QByteArray saveStat = QByteArray::fromHex (CApp::pCApp()->readUniqueParam("medicatux geometry","MainWindow_state").toLatin1());
    restoreState( saveStat );
}
//--------------------------------- setFontFromIniFile -----------------------------------------------------------------------
QFont C_MW_Prescription::setFontFromIniFile()
{    //....... config polices .....................
    QString pixSize    = "11";
    QString family     = "Arial";
    QFont   initialFnt = this->font();
#ifdef Q_OS_MAC
    QString section    = "medicatux MacFontParam";
#endif
#ifdef Q_WS_WIN
    QString section    = "medicatux WinFontParam";
#endif
#ifdef Q_OS_LINUX
    QString section    = "medicatux LinFontParam";
#endif
    if (CApp::pCApp()->readParam(section.toLatin1(), "interface", &pixSize, &family)==QString::null)  // zero = pas d'erreur
       { initialFnt.setPixelSize(pixSize.toInt());
         initialFnt.setFamily(family);
         this->setFont(initialFnt);
       }
    if (CApp::pCApp()->readParam(section.toLatin1(), "posologieEditor", &pixSize, &family)==QString::null)  // zero = pas d'erreur
       { initialFnt.setPixelSize(pixSize.toInt());
         initialFnt.setFamily(family);
         if (m_pC_Frm_Prescription) m_pC_Frm_Prescription->setFont(initialFnt);
       }
    return  initialFnt;  // retiurne la fonte de m_pC_Frm_Prescription
}

//--------------------------------- setMainTabWidgetConfig -----------------------------------------------------------------------
void C_MW_Prescription::setMainTabWidgetConfig()
{   QByteArray section                = (CApp::pCApp()->applicationName() +"_TabManager").toLatin1();
    QString defaultTab                = CApp::pCApp()->readUniqueParam(  section, "defaultTab");
    QString defaultProductsList       = CApp::pCApp()->readUniqueParam(  section, "defaultProductsList");
    QString defaultClassificationList = CApp::pCApp()->readUniqueParam(  section, "defaultClassificationList");

    //......... recuperer la liste des onglets a inactiver .................................................
    //          attention : c'est le texte du titre de l'onglet qui est stocke dans la liste
    //                      et non le objectName
    int       index   = -1;
    QStringList list  = CApp::pCApp()->getParamList( section , "inactiveTab", 1);
    for (int i = 0; i<list.size(); ++i)
        { m_pGUI->tab_MainTabWidget->tabWidgetByTabText(list[i], &index);
          m_pGUI->tab_MainTabWidget->Slot_CloseRequested(index);
        }

    //............. activer l'onglet par defaut ..............................................................
    //              attention : la c'est l'objectName qui est stocke
    m_pGUI->tab_MainTabWidget->setCurrentItem ( defaultTab );
    //.............. si celui par defaut est le meme que celui deja affiche alors ............................
    //               le signal currentChanged ( int ) ne sera pas envoye (car pas de changement)
    //               DONC appeler le slot pour forcer la mise a jour du contenu la premiere fois
    //               au premier passage sinon ne sera pas fait.
    if ( defaultTab  == m_pGUI->tab_MainTabWidget->currentWidget()->objectName() ) // si meme alors le
       { m_pGUI->tab_MainTabWidget->tabWidgetByTabObjectName(defaultTab,&index);
         Slot_tabWidget_Main_currentChanged(index);
       }
    //............. activer la liste des produits par defaut ..............................................................
    //              attention : la c'est l'objectName qui est stocke dans le .ini
    set_toolBox_ListesProduits_item ( defaultProductsList );
    //............. activer la liste des classifications par defaut ..............................................................
    //              attention : la c'est l'objectName qui est stocke dans le .ini
    set_toolBox_classifications_item ( defaultClassificationList );
}
//--------------------------------- set_toolBox_classifications_item -----------------------------------------------------------------------
void C_MW_Prescription::set_toolBox_classifications_item(const QString &name)
{    m_pGUI->toolBox_classifications->setCurrentItem ( name );
     if ( name == "page_ATC" )
        {
    #if QT_5
            m_pGUI->treeWidget_ATC->header()->setSectionResizeMode (0, QHeaderView::Interactive ); // QHeaderView::Stretch  QHeaderView::Interactive QHeaderView::ResizeToContents
     #else
            m_pGUI->treeWidget_ATC->header()->setResizeMode (0, QHeaderView::Interactive ); // QHeaderView::Stretch  QHeaderView::Interactive QHeaderView::ResizeToContents
     #endif
//            m_pGUI->treeWidget_ATC->header()->resizeSection (0, 400 );
        }
}
//--------------------------------- set_drugFilterInterfaceFromIniFile -----------------------------------------------------------------------
void C_MW_Prescription::set_drugFilterInterfaceFromIniFile()
{
    //................................. filtre de la liste des drogues ........................................
    m_pGUI->checkBox_Dci_filter->setChecked((CApp::pCApp()->readUniqueParam("medicatux filter", "dci").toUpper().startsWith('A')));  // A comme activated ou actif ou active
    m_pGUI->checkBox_Dc3_filter->setChecked((CApp::pCApp()->readUniqueParam("medicatux filter", "dc3").toUpper().startsWith('A')));  // A comme activated ou actif ou active
    m_pGUI->checkBox_Atc_filter->setChecked((CApp::pCApp()->readUniqueParam("medicatux filter", "atc").toUpper().startsWith('A')));
    m_pGUI->checkBox_Nom_filter->setChecked((CApp::pCApp()->readUniqueParam("medicatux filter", "nom").toUpper().startsWith('A')));
    m_pGUI->checkBox_Cip_filter->setChecked((CApp::pCApp()->readUniqueParam("medicatux filter", "cip").toUpper().startsWith('A')));  // cip
    m_pGUI->checkBox_Ucd_filter->setChecked((CApp::pCApp()->readUniqueParam("medicatux filter", "ucd").toUpper().startsWith('A')));  // prix
    m_pGUI->checkBox_Gen_filter->setChecked((CApp::pCApp()->readUniqueParam("medicatux filter", "gen").toUpper().startsWith('A')));  // generique
    m_pGUI->checkBox_Hop_filter->setChecked((CApp::pCApp()->readUniqueParam("medicatux filter", "hop").toUpper().startsWith('A')));  // hopital
    m_pGUI->checkBox_Vil_filter->setChecked((CApp::pCApp()->readUniqueParam("medicatux filter", "vil").toUpper().startsWith('A')));  // ville
    initDrugListFilter();   // lit l'interface et positionne la variable de filtre en fonction
}

//--------------------------------- set_toolBox_ListesProduits_item -----------------------------------------------------------------------
void C_MW_Prescription::set_toolBox_ListesProduits_item(const QString &name, int item_mustBeSet /*= C_MW_Prescription::mustBeSet*/)
{   if ( item_mustBeSet == C_MW_Prescription::mustBeSet ) m_pGUI->toolBox_ListesProduits->setCurrentItem ( name );
    if ( name == "page_Produits" )
       {if (m_isToolPtoductsInitialised == true) return;
        //................................. filtre de la liste des drogues ........................................
        /*
        m_pGUI->checkBox_Dci_filter->setChecked((CApp::pCApp()->readUniqueParam("medicatux filter", "dci").toUpper().startsWith('A')));  // A comme activated ou actif ou active
        m_pGUI->checkBox_Dc3_filter->setChecked((CApp::pCApp()->readUniqueParam("medicatux filter", "dc3").toUpper().startsWith('A')));  // A comme activated ou actif ou active
        m_pGUI->checkBox_Atc_filter->setChecked((CApp::pCApp()->readUniqueParam("medicatux filter", "atc").toUpper().startsWith('A')));
        m_pGUI->checkBox_Nom_filter->setChecked((CApp::pCApp()->readUniqueParam("medicatux filter", "nom").toUpper().startsWith('A')));
        m_pGUI->checkBox_Cip_filter->setChecked((CApp::pCApp()->readUniqueParam("medicatux filter", "cip").toUpper().startsWith('A')));  // cip
        m_pGUI->checkBox_Ucd_filter->setChecked((CApp::pCApp()->readUniqueParam("medicatux filter", "ucd").toUpper().startsWith('A')));  // prix
        m_pGUI->checkBox_Gen_filter->setChecked((CApp::pCApp()->readUniqueParam("medicatux filter", "gen").toUpper().startsWith('A')));  // generique
        m_pGUI->checkBox_Hop_filter->setChecked((CApp::pCApp()->readUniqueParam("medicatux filter", "hop").toUpper().startsWith('A')));  // hopital
        m_pGUI->checkBox_Vil_filter->setChecked((CApp::pCApp()->readUniqueParam("medicatux filter", "vil").toUpper().startsWith('A')));  // ville
        */
        //.................................. mapper les signaux des checkBox de la liste des drogues .......................................
        m_DrugListFilter_SignalMapper = new QSignalMapper(this);
        connect(m_DrugListFilter_SignalMapper, SIGNAL(mapped(const QString &)),  this, SLOT(Slot_checkBox_DrugList_filter_stateChanged(const QString &)));

        connect(m_pGUI->checkBox_Dci_filter,   SIGNAL(stateChanged ( int  )),   m_DrugListFilter_SignalMapper, SLOT(map()));
        m_DrugListFilter_SignalMapper->setMapping(m_pGUI->checkBox_Dci_filter, "dci");
        connect(m_pGUI->checkBox_Dc3_filter,   SIGNAL(stateChanged ( int  )),   m_DrugListFilter_SignalMapper, SLOT(map()));
        m_DrugListFilter_SignalMapper->setMapping(m_pGUI->checkBox_Dc3_filter, "dc3");
        connect(m_pGUI->checkBox_Atc_filter,   SIGNAL(stateChanged ( int  )),   m_DrugListFilter_SignalMapper, SLOT(map()));
        m_DrugListFilter_SignalMapper->setMapping(m_pGUI->checkBox_Atc_filter, "atc");
        connect(m_pGUI->checkBox_Nom_filter,   SIGNAL(stateChanged ( int  )),   m_DrugListFilter_SignalMapper, SLOT(map()));
        m_DrugListFilter_SignalMapper->setMapping(m_pGUI->checkBox_Nom_filter, "nom");
        connect(m_pGUI->checkBox_Ucd_filter,   SIGNAL(stateChanged ( int  )),   m_DrugListFilter_SignalMapper, SLOT(map()));
        m_DrugListFilter_SignalMapper->setMapping(m_pGUI->checkBox_Ucd_filter, "ucd");
        connect(m_pGUI->checkBox_Cip_filter,   SIGNAL(stateChanged ( int  )),   m_DrugListFilter_SignalMapper, SLOT(map()));
        m_DrugListFilter_SignalMapper->setMapping(m_pGUI->checkBox_Cip_filter, "cip");
        connect(m_pGUI->checkBox_Gen_filter,   SIGNAL(stateChanged ( int  )),   m_DrugListFilter_SignalMapper, SLOT(map()));
        m_DrugListFilter_SignalMapper->setMapping(m_pGUI->checkBox_Gen_filter, "gen");
        connect(m_pGUI->checkBox_Vil_filter,   SIGNAL(stateChanged ( int  )),   m_DrugListFilter_SignalMapper, SLOT(map()));
        m_DrugListFilter_SignalMapper->setMapping(m_pGUI->checkBox_Vil_filter, "vil");
        connect(m_pGUI->checkBox_Hop_filter,   SIGNAL(stateChanged ( int  )),   m_DrugListFilter_SignalMapper, SLOT(map()));
        m_DrugListFilter_SignalMapper->setMapping(m_pGUI->checkBox_Hop_filter, "hop");
        m_isToolPtoductsInitialised = true;
       }
    else if ( name == "page_Accessoires" )
       { //............. activer la combo accessoires ..............................................................
         QByteArray section                = (CApp::pCApp()->applicationName() +"_TabManager").toLatin1();
         QString defaultAccessoryList      = CApp::pCApp()->readUniqueParam(  section, "defaultAccessoryList");
         int index                         = m_pGUI->comboBox_FamillesProduits->findText(defaultAccessoryList);
         if (index != -1) { m_pGUI->comboBox_FamillesProduits->setCurrentIndex(index);
                            Slot_comboBox_FamillesProduits_activated( index);
                          }
       }
    else if ( name == "page_CustomList" )
       { if (m_pGUI->comboBox_customList->count()==0)  fillCustomDrugListCombo("BDM");
       }
}

//--------------------------------- writeMainTabWidgetConfig -----------------------------------------------------------------------
void C_MW_Prescription::writeMainTabWidgetConfig()
{ QStringList list     = m_pGUI->tab_MainTabWidget->removedList();
  QByteArray section   = (CApp::pCApp()->applicationName() +"_TabManager").toLatin1();
  QWidget *pQWidget    = m_pGUI->toolBox_ListesProduits->currentWidget();
  if (pQWidget)
     { CApp::pCApp()->writeParam(  section, "defaultProductsList", pQWidget->objectName().toLatin1());
     }
  pQWidget = m_pGUI->toolBox_classifications->currentWidget();
  if (pQWidget)
     { CApp::pCApp()->writeParam(  section, "defaultClassificationList", pQWidget->objectName().toLatin1());
     }
  CApp::pCApp()->writeParam(   section, "defaultAccessoryList", m_pGUI->comboBox_FamillesProduits->currentText().toLatin1());
  CApp::pCApp()->writeParam(   section, "defaultTab",           m_pGUI->tab_MainTabWidget->currentWidget()->objectName().toLatin1());
  CApp::pCApp()->replaceList(  section, "inactiveTab",          list );
}

//------------------------ Slot_PrescriptionWantGeometry ---------------------------------------
void C_MW_Prescription::Slot_PrescriptionWantGeometry(QRect &application_rect, QRect &wdg_rect)
{ QString val1, val2, val3, val4;
  application_rect = rect(); //this->geometry();         // ok on peut toujours repondre a ca
  wdg_rect = QRect(-1,-1,-1,-1);               // par defaut non connu
  if (  CApp::pCApp()->readParam("medicatux geometry", "interaction window", &val1, &val2, &val3, &val4)==QString::null)  // zero = pas d'erreur
     {  wdg_rect = QRect(val1.toInt(),val2.toInt(),val3.toInt(),val4.toInt());
     }
}

//------------------------ display_HAS_Status ---------------------------------------
void C_MW_Prescription::display_HAS_Status()
{ if (m_pC_Frm_Prescription==0) return;
  m_pC_Frm_Prescription->display_HAS_Status();
}

//------------------------ setWindowTitle ---------------------------------------
void C_MW_Prescription::setWindowTitle()
{QString title = CApp::pCApp()->applicationName()  + " (" +  m_pC_BDM_Api->dataSourceVersion() + ") ";
 if (PATIENT && PATIENT->get_usual_name().length())
    {
     title += PATIENT->get_usual_name() + " " + PATIENT->get_forename() +
              tr(" \303\242ge : ") + PATIENT->get_age() +
              tr(" poids : ") + PATIENT->poids() +
              " sexe : "  + PATIENT->get_sexe();

    }
 QMainWindow::setWindowTitle(title);
}
//------------------------ userCtxToInterface ---------------------------------------
void C_MW_Prescription::userCtxToInterface(C_UserCtx *pC_UserCtx)
{if (pC_UserCtx==0) return;
 m_pGUI->lineEdit_UserNom->setText(pC_UserCtx->get_usual_name());
 m_pGUI->lineEdit_UserPrenom->setText(pC_UserCtx->get_forename() );
 m_pGUI->lineEdit_UserTitre->setText(pC_UserCtx->get_Titre() );
 m_pGUI->lineEdit_UserAdresse->setText(pC_UserCtx->get_Adresse() );
 m_pGUI->lineEdit_UserCodePostal->setText(pC_UserCtx->get_CodePostal() );
 m_pGUI->lineEdit_UserVille->setText(pC_UserCtx->get_Ville() );
 m_pGUI->lineEdit_UserEmail->setText(pC_UserCtx->get_Email() );
 m_pGUI->lineEdit_UserRPPS->setText(pC_UserCtx->get_Nu_RPPS() );
 m_pGUI->lineEdit_UserClefRPPS->setText(pC_UserCtx->get_Nu_ClefRPPS() );
 m_pGUI->lineEdit_UserNumOrdinal->setText(pC_UserCtx->get_Nu_ordinal() );
 m_pGUI->lineEdit_UserTel1->setText(pC_UserCtx->get_Tel1() );
 m_pGUI->lineEdit_UserTel2->setText(pC_UserCtx->get_Tel2() );
 m_pGUI->lineEdit_UserTel3->setText(pC_UserCtx->get_Tel3() );
}

//------------------------ patientCtxToInterface ---------------------------------------
void C_MW_Prescription::patientCtxToInterface(C_PatientCtx *pC_PatientCtx)
{QString dateStr = "";
 if (pC_PatientCtx==0) return;
 dateStr = pC_PatientCtx->get_dateOfBirth("dd-MM-yyyy");
 m_pGUI->lineEdit_Nom->setText(pC_PatientCtx->get_usual_name());
 m_pGUI->lineEdit_NomNaissance->setText(pC_PatientCtx->get_young_name());
 m_pGUI->lineEdit_Prenom->setText(pC_PatientCtx->get_forename());
 m_pGUI->lineEdit_DateNaissance->setText(dateStr);
 m_pGUI->lineEdit_poids->setText(pC_PatientCtx->poids());
 m_pGUI->lineEdit_taille->setText(pC_PatientCtx->taille());
 QString sexe = pC_PatientCtx->get_sexe().left(1);
 if (sexe.length())
     m_pGUI->lineEdit_sexe->setText( (sexe=="M")?tr("Masculin"):tr("F\303\251minin"));
 else
     m_pGUI->lineEdit_sexe->setText("");
 //m_pGUI->label_DtNaiss->setText(tr("\303\242g\303\251 de <b>%1</b> n\303\251%2 le ").arg(C_DateTools::CalculeDiffAge(pC_PatientCtx->get_dateOfBirth()),(sexe=="M")?"":"e"));
 QList <C_LifeEvent> lifeEventList =  pC_PatientCtx->lifeEventList();
 m_pGUI->treeWidget_ATCD->clear();
 for (int i=0; i<lifeEventList.size(); ++i)
     {QTreeWidgetItem *item = new QTreeWidgetItem(m_pGUI->treeWidget_ATCD);
      item->setText(0,lifeEventList.at(i).code());
      item->setText(1,lifeEventList.at(i).codeTyp());
      item->setText(2,lifeEventList.at(i).libelle());
      item->setText(3,lifeEventList.at(i).AldSportVigilanceProperty());
      item->setText(4,lifeEventList.at(i).medintuxClass());
      item->setText(5,lifeEventList.at(i).comment());
      item->setText(6,lifeEventList.at(i).dateDeb());
      item->setText(7,lifeEventList.at(i).dateFin());
      m_pGUI->treeWidget_ATCD->addTopLevelItem(item);
     }
}
//------------------------ Slot_retranslateUi ---------------------------------------
void C_MW_Prescription::Slot_retranslateUi()
{
}
//------------------------------------ C_MW_Prescription -------------------------------
C_MW_Prescription::~C_MW_Prescription()
{   delete m_pGUI;
    //if (m_pC_PatientCtx)   delete m_pC_PatientCtx;
    //if (m_pC_UserCtx)      delete m_pC_UserCtx;
}
//------------------------ moveEvent -----------------------------------------
void C_MW_Prescription::moveEvent ( QMoveEvent * event )
{ Q_UNUSED(event);
  // if ( m_pC_Frm_Prescription && m_pC_Frm_Prescription->alertView() )
  //   {   QPoint pos = m_pC_Frm_Prescription->alertView()->pos() + (event->pos() - event->oldPos());
  //       m_pC_Frm_Prescription->alertView()->move(pos);
  //   }
}

//------------------------ event -----------------------------------------
bool C_MW_Prescription::event ( QEvent * e )
{
 /*
 if (e->type()==QEvent::Move)
    { QMoveEvent ( const QPoint & pos, const QPoint & oldPos )
      m_infoTexEdit

    }
 */
 if (e->type()==QEvent::Close)
    {QRect rect;
     if ( m_pC_Frm_Prescription )
        { rect = m_pC_Frm_Prescription-> get_displayInteractionWindowGeometry();
          //................... enregistrer la fenetre d'affichage des interactions ...................
          if ( rect.x() != -1 )
             { CApp::pCApp()->writeParam("medicatux geometry", "interaction window", QString::number(rect.x()).toLatin1(),     QString::number(rect.y()).toLatin1(),
                                                                                     QString::number(rect.width()).toLatin1(), QString::number(rect.height()).toLatin1()
                                        );
             }
          m_pC_Frm_Prescription->Slot_close();
        }
     Slot_Apropos_Proc_finished     (0,  QProcess::NormalExit);
//     Slot_Signalement_Proc_finished (0,  QProcess::NormalExit);
    }
 return QMainWindow::event(e);
}
//------------------------ Slot_Apropos_Proc_finished -----------------------------------------
void C_MW_Prescription::Slot_Apropos_Proc_finished (int,  QProcess::ExitStatus)
{if (m_Apropos_Proc)
    {m_Apropos_Proc->terminate();
     m_Apropos_Proc->waitForFinished (5000);
     delete m_Apropos_Proc;
     m_Apropos_Proc = 0;
    }
}

//------------------------ add_C_Frm_Prescription -----------------------------------------
int C_MW_Prescription::add_C_Frm_Prescription(const QFont &fnt, const QStringList &grammarList)
{ m_pC_Frm_Prescription = new C_Frm_Prescription( CApp::pCApp()->pathTheme() + CApp::pCApp()->applicationName()+"/",
                                                  m_pC_BDM_Api,
                                                  m_pGUI->scrollArea_Prescription,
                                                  this );
 QString section     = "medicatux products config";
 QString value_ald   = CApp::pCApp()->readUniqueParam(section, "background color ald");
 QString value_std   = CApp::pCApp()->readUniqueParam(section, "background color std");
 int childs_w        = CApp::pCApp()->readUniqueParam(section, "last width").toInt();
 int childs_h        = CApp::pCApp()->readUniqueParam(section, "last height").toInt();

 m_pC_Frm_Prescription->setChildsColors(value_std, value_ald);
 m_pC_Frm_Prescription->setChildsSize(childs_w, childs_h);
 m_pC_Frm_Prescription->setDCI_Mode(m_dci_mode);
 m_pC_Frm_Prescription->set_notificatorView( m_Notifications );

 m_pGUI->scrollArea_Prescription->setWidget(m_pC_Frm_Prescription);
 connect( m_pC_Frm_Prescription,  SIGNAL( Sign_child_LeaveLastEditZone()),                   this, SLOT( Slot_child_LeaveLastEditZone() ));
 connect( m_pC_Frm_Prescription,  SIGNAL( Sign_PrescriptionWantGeometry(QRect &, QRect &)),  this, SLOT( Slot_PrescriptionWantGeometry(QRect &, QRect &) ));
 connect( m_pC_Frm_Prescription,  SIGNAL( Sign_ProductNumberClicked( C_Frm_Produit* )),      this, SLOT( Slot_ProductNumberClicked( C_Frm_Produit* ) ));
 connect( m_pC_Frm_Prescription,  SIGNAL( Sign_FavorisAdded( C_Frm_Produit* )),              this, SLOT( Slot_FavorisAdded( C_Frm_Produit* ) ));       // mettre a jour la liste des favoris
 connect( m_pC_Frm_Prescription,  SIGNAL( Sign_FactCorpoChanged( C_PatientCtx * )),          this ,SLOT( Slot_FactCorpoChanged(C_PatientCtx *) ));
 connect( m_pC_Frm_Prescription,  SIGNAL( Sign_UpdateTotalPrice(  const QString & )),        this ,SLOT( Slot_UpdateTotalPrice(  const QString & ) ));
 connect( this,                   SIGNAL( Sign_PatientCtxChanged(C_PatientCtx *) ),  m_pC_Frm_Prescription ,SLOT( Slot_PatientCtxChanged(C_PatientCtx *) ));

 m_pC_Frm_Prescription->setFont(fnt);
 if ( grammarList.length() )
    { m_pC_Frm_Prescription->setGrammarList(grammarList);
    }
 return 0;
}

//------------------------ Slot_UpdateTotalPrice ---------------------------------------
void C_MW_Prescription::Slot_UpdateTotalPrice(  const QString &total_price )
{ m_pGUI->lineEdit_PrixTotal->setText(total_price);
}

//------------------------ Slot_add_product_In_C_Frm_Prescription ---------------------------------------
/*
    C_BDM_DrugListRecord(QString id,
                         QString id_type,
                         QString commercialName,
                         QString dci_1Name,
                         QString dc2_1Name,
                         QString dc3_1Name,
                         QString owner,
                         QString lang,
                         QString ucd,
                         QString ucd_price,
                         QString atc,
                         QString generique,
                         QString statut
                        )
                        */
void C_MW_Prescription::Slot_add_product_In_C_Frm_Prescription(QTreeWidgetItem * pQTreeWidgetItem)
{ if (m_pC_Frm_Prescription==0)   return;
  if (pQTreeWidgetItem==0)        return;
  QString xmlData   = "";
  if ( can_use_API(pQTreeWidgetItem) )                               // produits concernes par m_pC_BDM_Api
     { QList < C_BDM_DrugListRecord > drugList = m_pC_BDM_Api->selectProductsList (pQTreeWidgetItem->text(COL_ID), C_BDM_PluginI::cip_filter);
       if (drugList.count())
          { xmlData = drugList.first().serialize();
            m_pC_Frm_Prescription->appendNewObject(xmlData, true);
          }
     }
  else                                                               // produits de provenance obscure
     { C_BDM_DrugListRecord c_bdm_druglistrecord = C_BDM_DrugListRecord_From_QTreeWidgetProductsItem (pQTreeWidgetItem);
       xmlData = c_bdm_druglistrecord.serialize();
       m_pC_Frm_Prescription->appendNewObject(xmlData, true);
     }
}

//------------------------ can_use_API ---------------------------------------
bool C_MW_Prescription::can_use_API( QTreeWidgetItem * pQTreeWidgetItem )
{if (m_pC_BDM_Api==0)                   return false;
 if (m_pC_BDM_Api->drugsList_Size()<=0) return false;
 return ( pQTreeWidgetItem->text(COL_ID_TYP)=="CIP" ) ;
}

//------------------------ Slot_child_LeaveLastEditZone ---------------------------------------
void C_MW_Prescription::Slot_child_LeaveLastEditZone()
{m_pGUI->lineEdit_DrugsSearch->setFocus();
 m_pGUI->lineEdit_DrugsSearch->selectAll();
}

//--------------------------------- Slot_lineEditAutoLocator_keyPressEvent -----------------------------------------------------------------------
void C_MW_Prescription::Slot_lineEditAutoLocator_keyPressEvent(QKeyEvent *event, int &callStandardEvent)
{ lineEdit_searchProducts_keyPressEvent(m_pGUI->treeWidget_Produits, event, callStandardEvent);
}

//--------------------------------- getSelectedListViewItem -------------------------------------------------------------
QTreeWidgetItem * C_MW_Prescription::getSelectedListViewItem(QTreeWidget *pQTreeWidget)
{ if (pQTreeWidget==0) return 0;
  QList<QTreeWidgetItem *> list = pQTreeWidget->selectedItems ();
  int nb                        = list.size();
  if (nb) return list[nb-1];               // on prend le dernier de la selection
  return 0;
}

//------------------------------------ Slot_m_action_Signalement -------------------------------
void C_MW_Prescription::Slot_m_action_Signalement (bool)
{Slot_m_action_Signalement ();
}
//------------------------------------ Slot_m_action_Signalement -------------------------------
/*
 * 2.2 Mode mini navigateur web :

- Appel : Premier argument : Si cet argument comporte la mention BrowserOnly,
                               APropos se comportera comme un navigateur internet et
                               le deuxieme argument sera l'URL (adresse internet de la page a afficher).

          Deuxieme argument : Si le premier argument comporte la mention BrowserOnly.
                               APropos se comportera comme un navigateur internet et
                               le deuxieme argument sera l'URL (adresse internet de la page a afficher).

[medicatux links]
  pharmacovigilance =
*/
void C_MW_Prescription::Slot_m_action_Signalement ()
{   //......................... completer les autres arguments .........................................
    QString pharmacovigilance_link = CApp::pCApp()->readUniqueParam("medicatux links" , "pharmacovigilance" );
    QDesktopServices::openUrl ( pharmacovigilance_link );
}
//------------------------------------ Slot_m_action_APropos -------------------------------
void C_MW_Prescription::Slot_m_action_APropos (bool)
{Slot_m_action_APropos ();
}
//------------------------------------ Slot_m_action_APropos -------------------------------
void C_MW_Prescription::Slot_m_action_APropos ()
{
    QString macAdr;
    QString ipAdr  = CApp::pCApp()->get_Current_IP_Adr(&macAdr);
    CGestIni::Param_UpdateToDisk(CApp::pCApp()->pathAppli()+"Ressources/Changements.html",
                                 m_pGUI->textEdit_changeLog->toHtml().replace("{{IP_ADR}}",  ipAdr)
                                                                     .replace("{{MAC_ADR}}", macAdr)
                                                                     .replace("{{PATH_INI}}",CApp::pCApp()->pathIni())
                                 );
    QString pathExeAPropos     = CGestIni::Construct_Name_Exe("APropos",       QFileInfo (qApp->arguments()[0]).path());
    QStringList argList;
    //......................... completer les autres arguments .........................................
    argList << CApp::pCApp()->applicationName();                                                            // 1  nom du module
    argList << tr("Module for drugs prescription");                                                         // 2  description courte
    argList << CApp::pCApp()->ApplicationAndQtVersion(tr("%1:%2").arg(__DATE__,__TIME__));                  // 3  numero de version
    argList << CApp::pCApp()->pathAppli()+"Ressources/Changements.html";                                    // 4  fichiers decrivant les changements
    argList <<"";                                                                                           // 5  Icone par defaut
    argList <<"";                                                                                           // 6  aide en ligne (vide pour prendre celle par defaut)
    argList << CApp::pCApp()->pathAppli()+"Ressources/APropos/A_propos.html";                               // 7  apropos (on met une chaine vide pour qu'il prenne celui par d?faut)
    argList << (m_pC_BDM_Api?m_pC_BDM_Api->dataSourceVersion():tr("Drug DB version not available"));        // 8  numero de version de la base de donnee
    if (m_Apropos_Proc==0)
       { m_Apropos_Proc = new QProcess(this);
         if (m_Apropos_Proc)
            {connect( m_Apropos_Proc, SIGNAL(finished ( int,  QProcess::ExitStatus)),  this, SLOT(Slot_Apropos_Proc_finished (int,  QProcess::ExitStatus)) );
             m_Apropos_Proc->start(pathExeAPropos, argList);
             m_Apropos_Proc->waitForStarted  (4000);
            }
       }
}

//------------------------------------ Slot_toolButton_zoomUpReleased -------------------------------
void C_MW_Prescription::Slot_toolButton_zoomUpReleased (  )
{if (m_pC_Frm_Prescription==0) return;
 m_pC_Frm_Prescription->setZoom(5);
 QSize size      = m_pC_Frm_Prescription->getChildsSize();
 QString section = "medicatux products config";
 CApp::pCApp()->writeParam(section.toLatin1(), "last height", QString::number(size.height()).toLatin1());
 CApp::pCApp()->writeParam(section.toLatin1(), "last width",  QString::number(size.width()).toLatin1());
}

//------------------------------------ Slot_toolButton_zoomDwReleased -------------------------------
void C_MW_Prescription::Slot_toolButton_zoomDwReleased (  )
{if (m_pC_Frm_Prescription==0) return;
 m_pC_Frm_Prescription->setZoom(-5);
 QString section = "medicatux products config";
 QSize size      = m_pC_Frm_Prescription->getChildsSize();
 CApp::pCApp()->writeParam(section.toLatin1(), "last height", QString::number(size.height()).toLatin1());
 CApp::pCApp()->writeParam(section.toLatin1(), "last width",  QString::number(size.width()).toLatin1());
}
//------------------------------------ Slot_prescription_zoom_menu_triggered -------------------------------
void C_MW_Prescription::Slot_prescription_zoom_menu_triggered ( QAction* pQAction)
{ if (m_pC_Frm_Prescription==0) return;
   // tr("1 - Mode r\303\251duit");
   // tr("2 - Mode d\303\251ploy\303\251 ");
   // tr("3 - Enregistrer la position courante comme mode r\303\251duit");
   // tr("4 - Enregistrer la position courante comme mode d\303\251ploy\303\251");
   // tr("5 - Choisir une couleur pour la zone non ALD");
   // tr("6 - Choisir une couleur pour la zone ALD");
   // tr("7 - Cacher les graduations");
   // tr("8 - Montrer les graduations");
   // tr("9 - d\303\251sactiver 'apres-midi'"));
   // tr("10- activer 'apres-midi'"));
   // tr("11- d\303\251sactiver 'coucher'"));
   // tr("12- activer 'coucher'"));
   // tr("13- d\303\251sactiver 'nuit'"));
   // tr("14- activer 'nuit'"));

  QSize size      = m_pC_Frm_Prescription->getChildsSize();
  int option      = pQAction->text ().left(2).trimmed().toInt();
  QString value   = "";
  QString section = "medicatux products config";
  if (option == 1)
    { value = CApp::pCApp()->readUniqueParam(section, "reduct height");
      if (value.length()==0) return;
      m_pC_Frm_Prescription->setChildsSize(0,value.toInt());
      CApp::pCApp()->writeParam(section.toLatin1(), "last height", value.toLatin1());
    }
  else if (option == 2)
    { value = CApp::pCApp()->readUniqueParam(section, "deploy height");
      if (value.length()==0) return;
      m_pC_Frm_Prescription->setChildsSize(0,value.toInt());
      CApp::pCApp()->writeParam(section.toLatin1(), "last height", value.toLatin1());
    }
  else if (option == 3)
    { CApp::pCApp()->writeParam(section.toLatin1(), "reduct height", QString::number(size.height()).toLatin1());
    }
  else if (option == 4)
    { CApp::pCApp()->writeParam(section.toLatin1(), "deploy height", QString::number(size.height()).toLatin1());
    }
  else if (option == 5)
    { QString value_ald   = CApp::pCApp()->readUniqueParam(section, "background color ald");
      QString value_std   = CApp::pCApp()->readUniqueParam(section, "background color std");
      QColor      color   = QColorDialog::getColor ( QColor(value_std.remove('#').prepend('#')), this );
      if (color.isValid())
         { value_std = color.name();
           CApp::pCApp()->writeParam(section.toLatin1(), "background color std", value_std.toLatin1());
           m_pC_Frm_Prescription->setChildsColors(value_std, value_ald);
         }
    }
  else if (option == 6)
    { QString value_ald   = CApp::pCApp()->readUniqueParam(section, "background color ald");
      QString value_std   = CApp::pCApp()->readUniqueParam(section, "background color std");
      QColor      color   = QColorDialog::getColor ( QColor(value_ald.remove('#').prepend('#')), this );
      if (color.isValid())
         { value_ald = color.name();
           CApp::pCApp()->writeParam(section.toLatin1(), "background color ald", value_ald.toLatin1());
           m_pC_Frm_Prescription->setChildsColors(value_std, value_ald);
         }
    }
}

//------------------------------------ Slot_onQuit -------------------------------
void C_MW_Prescription::Slot_onQuit()
{   if (m_webView_Biblio) m_webView_Biblio->stop();
    m_runMacroForAll = 0;
    if (m_pC_Frm_Prescription)m_pC_Frm_Prescription->Slot_close();
    if (CApp::pCApp()->u_Html()) CApp::pCApp()->u_Html()->breakHttpProcess();
    Slot_On_webView_Biblio_loadFinished(true);
    recordPositions();
    if ( CApp::pCApp()->readUniqueParam( CApp::pCApp()->applicationName() +" session", "log_file").toLower()[0] != 'n')
        CGestIni::Param_UpdateToDisk(CApp::pCApp()->pathAppli()+"/Ressources/Log"+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")+".html", m_pGUI->textEdit_Monitor->toHtml());
}

//------------------------------------ Slot_treeWidget_ATC_itemClicked -------------------------------
void C_MW_Prescription::Slot_treeWidget_ATC_itemClicked ( QTreeWidgetItem *pQTreeWidgetItem , int   )
{if (pQTreeWidgetItem==0) return;
 if (m_pC_BDM_Api==0)     return;
 QWidget                 *pTab_QWidget =  m_pGUI->toolBox_ListesProduits->toolWidgetByTabObjectName("page_Produits");   // zero si retire
 if ( pTab_QWidget == 0 ) return;
 m_pGUI->toolBox_ListesProduits->setCurrentWidget ( pTab_QWidget );
 int nb = m_pC_BDM_Api->selectProductsList(m_pGUI->treeWidget_Produits, pQTreeWidgetItem->text(1), C_BDM_PluginI::atc_filter , 20000 /* m_drugListMax */);
 m_pGUI->label_ListMedoc->setText(tr("%1 sur %2 produit(s)").arg(QString::number(nb),QString::number(m_pC_BDM_Api->drugsList_Size())));
}

//------------------------------------ Slot_pushButton_DefineLogin -------------------------------
void C_MW_Prescription::Slot_pushButton_DefineLogin (bool)
{
        int index =   m_pGUI->comboBox_BiblioURL->currentIndex();
        if (index==-1)           return;

        QString biblioType  = ""; // m_pGUI->comboBox_BiblioURL->itemData(index).toString();
        QString biblioUrl   = ""; // m_pGUI->comboBox_BiblioURL->itemText(index);
        QString biblioLogin = "";
        QString biblioPassw = "";
        get_BiblioType_Url_Login_Pass_FromComboBiblio(index, &biblioType, &biblioUrl, &biblioLogin, &biblioPassw);
        C_Dlg_Login *dlg    = new C_Dlg_Login(biblioLogin, biblioPassw, this);
        if (!dlg)             return;
        if (dlg->exec()==QDialog::Accepted)
           {biblioLogin = dlg->get_Login();    // dlg->get_CriptedLogin("#");
            biblioPassw = dlg->get_Password(); //.dlg->get_CriptedPassword("#");
            m_pGUI->comboBox_BiblioURL->removeItem (index );  // virer l'ancien item
            m_pGUI->comboBox_BiblioURL->insertItem(index, QIcon(CApp::pCApp()->pathTheme() + CApp::pCApp()->applicationName()+"/"+ biblioType+".png"), biblioUrl, QVariant(biblioType+"|"+biblioLogin+"|"+biblioPassw));
            m_pGUI->comboBox_BiblioURL->setCurrentIndex(index);
            comboBox_BiblioURLToIniFile();
           }
        delete dlg;
}

//------------------------------------ init_comboBox_BiblioURL -------------------------------
void C_MW_Prescription::init_comboBox_BiblioURL()
{
 QStringList lst;
 QStringList prmList;
 QString  biblioType = "";
 QString  url        = "";
 QString  login      = "";
 QString  passw      = "";
 QString  str        = "";

 CGestIni::Param_GetList( CApp::pCApp()->pathIni(), QString("prescription Biblio sites"), lst );
 for (int i=0; i<lst.count(); ++i)
     {str        = lst[i];
      prmList    = str.split(',');
      biblioType = prmList[0].remove(".png").trimmed();
      url        = prmList[1].trimmed();
      if (prmList.count()>2) {login = prmList[2].trimmed(); if (login.length() && login.at(0)=='#') login = CApp::pCApp()->criptedToUnCripted(login.mid(1));}
      if (prmList.count()>3) {passw = prmList[3].trimmed(); if (passw.length() && passw.at(0)=='#') passw = CApp::pCApp()->criptedToUnCripted(passw.mid(1));}
      m_pGUI->comboBox_BiblioURL->addItem(QIcon(CApp::pCApp()->pathTheme()+CApp::pCApp()->applicationName()+"/"+ biblioType+".png"), url, QVariant(biblioType+"|"+login+"|"+passw));
     }
 m_pGUI->comboBox_BiblioURL->setCurrentIndex(0);
}

//------------------------------------ get_BiblioType_Url_Login_Pass_FromComboBiblio -------------------------------
void C_MW_Prescription::get_BiblioType_Url_Login_Pass_FromComboBiblio(int comboIndex, QString *p_bibliotype, QString *p_url, QString *p_login,QString *p_passw)
{
  QStringList   clist  =  m_pGUI->comboBox_BiblioURL->itemData(comboIndex).toString().split('|');
  if (p_bibliotype)   *p_bibliotype = clist[0];
  if (p_url       )   *p_url        = m_pGUI->comboBox_BiblioURL->itemText(comboIndex);
  if (p_login     )   *p_login      = clist[1];    // {*p_login      = clist[1]; if (p_login->length() && p_login->at(0)=='#') *p_login = CApp::pCApp()->criptedToUnCripted(p_login->mid(1));}
  if (p_passw     )   *p_passw      = clist[2];    // {*p_passw      = clist[2]; if (p_passw->length() && p_passw->at(0)=='#') *p_passw = CApp::pCApp()->criptedToUnCripted(p_passw->mid(1));}
}

//------------------------------------ comboBox_BiblioURLToIniFile -------------------------------
void C_MW_Prescription::comboBox_BiblioURLToIniFile()
{
    CApp::pCApp()->removeSectionParam("prescription Biblio sites");

    int              nb =   m_pGUI->comboBox_BiblioURL->count();
    QString biblioType  = "";
    QString biblioUrl   = "";
    QString biblioLogin = "";
    QString biblioPassw = "";

    for (int index = 0; index < nb; ++index)
       { get_BiblioType_Url_Login_Pass_FromComboBiblio(index, &biblioType, &biblioUrl,&biblioLogin, &biblioPassw);
         CApp::pCApp()->writeParam("prescription Biblio sites",
                                    tr("site %1").arg(QString::number(index)).toLatin1(),
                                    biblioType.toLatin1(),
                                    biblioUrl.toLatin1(),
                                    (CApp::pCApp()->unCriptedToCripted(biblioLogin,"#")).toLatin1(),
                                    (CApp::pCApp()->unCriptedToCripted(biblioPassw,"#")).toLatin1());
       }
    CApp::pCApp()->updateIniParamToDisk();
}

//------------------------------------ Slot_On_pushButton_AllItems -------------------------------
void C_MW_Prescription::Slot_On_pushButton_AllItems(bool)
{QString biblioType    = "";
 QString biblioUrl     = "";
 QString biblioLogin   = "";
 QString biblioPassw   = "";
 int index             =   m_pGUI->comboBox_BiblioURL->currentIndex();
 if (index==-1)           return;
 //......... desenabler les controles ..............
 m_pGUI->treeWidget_Produits->setEnabled(false);
 m_pGUI->pushButton_RetrySite->setEnabled(false);
 m_pGUI->pushButton_UpdateCache->setEnabled(false);
 //......... sauf celui d'arret .....................
 m_pGUI->pushButton_AllItems->setEnabled(true);
 get_BiblioType_Url_Login_Pass_FromComboBiblio(index, &biblioType, &biblioUrl, &biblioLogin, &biblioPassw);
 QString lastItemFile  = CApp::pCApp()->pathAppli() + "Ressources/web/" + biblioType + "/LastItem.txt";
 int          nb_total = 0;
 int     lastItemIndex = 0;
 int     stopIndex     = 0;
 int                 i = 0;
 if ( m_runMacroForAll==0 )
    {
     m_runMacroForAll  = 1;
     nb_total          = m_pC_BDM_Api->selectProductsList(m_pGUI->treeWidget_Produits, "", getDrugListFilter());
     stopIndex         = nb_total;
     m_pGUI->label_ListMedoc->setText(tr("%1 sur %2 produit(s)").arg(QString::number(nb_total),QString::number(m_pC_BDM_Api->drugsList_Size())));
     QStringList  lst  = CGestIni::Param_UpdateFromDisk(lastItemFile).split('|',QString::SkipEmptyParts);
     if (lst.count()> 0) lastItemIndex     = lst[0].toInt();
     if (lst.count()> 1) stopIndex         = lst[1].toInt();
     if (lastItemIndex > 0)
        {
         if (QMessageBox::question ( this, tr("Reprendre"), tr("Reprendre au dernier index produit trait\303\251 :%1 (Confirmez) ou du d\303\251but index : 0 (Annulez)").arg(lastItemIndex),QMessageBox::Ok, QMessageBox::Cancel) != QMessageBox::Ok)
            { lastItemIndex = 0;
             // CApp::pCApp()->BDM_getDB()->biblio_DeleteRecords("", "fr",  biblioType);
            }
         if (stopIndex != nb_total)
            { if (QMessageBox::question ( this, tr("Interrompre"), tr("Interrompre au produit d'index %1 (Confirmez) ou en fin de liste (Annulez)").arg(stopIndex),QMessageBox::Ok, QMessageBox::Cancel) != QMessageBox::Ok)
                 {stopIndex = nb_total;
                 }
            }
        }
     m_pGUI->pushButton_AllItems->setIcon ( Theme::getIcon(CApp::pCApp()->applicationName()+"/loopRun.png") );
    }
 else
    {m_runMacroForAll = 0;
     C_Utils_Log::outMessage( m_pGUI->textEdit_Monitor, tr("Proc\303\251dure interrompue au dernier index en cours : %1").arg(CGestIni::Param_UpdateFromDisk(lastItemFile).toInt()));
    }

 QString    refNbTryStr = "";
 int             curTry = 1;
 int           refNbTry = 3;
 QString          state = "";
 QTreeWidgetItemIterator it (m_pGUI->treeWidget_Produits);
 while(m_runMacroForAll && *it)
    { state                              = "";
      QTreeWidgetItem * pQTreeWidgetItem = (*it);                           // nom du composant
      m_pGUI->treeWidget_Produits->setCurrentItem(pQTreeWidgetItem);
      m_pGUI->treeWidget_Produits->setItemSelected(pQTreeWidgetItem, true);
      m_pGUI->treeWidget_Produits->scrollToItem(pQTreeWidgetItem);
      if (i>=lastItemIndex)
         { state       = runMacroForCurrentItem(pQTreeWidgetItem).trimmed();
           refNbTryStr = CApp::pCApp()->getVarValue("$NombreEssais").trimmed();
           if ( refNbTryStr.length()) refNbTry = refNbTryStr.toInt();
           CGestIni::Param_UpdateToDisk(lastItemFile, QString::number(i));
           C_Utils_Log::outMessage( m_pGUI->textEdit_Monitor, tr("Dernier index en cours : %1").arg(i));
           m_pQLabel->setText(tr("Trait\303\251(s) : %1 sur %2").arg(QString::number(i),QString::number(nb_total)));
         }
      if ( state.length() && ++curTry <= refNbTry)  // si  erreur on re-essaye en decrementant le nbr d'essais
         {C_Utils_Log::outMessage( m_pGUI->textEdit_Monitor, tr(".        %1eme tentative car \303\251chec de la pr\303\251c\303\251dente, index en cours : %2").arg(QString::number(curTry), QString::number(i)));
         }
      else
         {++i;
          ++it;
          if (i>=stopIndex) break;
          curTry  = 1;
         }
    }
 m_runMacroForAll = 0;
 m_pGUI->pushButton_AllItems->setIcon         ( Theme::getIcon(CApp::pCApp()->applicationName()+"/loop.png") );
 m_pGUI->pushButton_RetrySite->show();
 //......... enabler les controles ..............
 m_pGUI->treeWidget_Produits->setEnabled(true);
 m_pGUI->pushButton_RetrySite->setEnabled(true);
 m_pGUI->pushButton_UpdateCache->setEnabled(true);
 m_pGUI->pushButton_AllItems->setEnabled(true);

 if (m_webView_Biblio) m_webView_Biblio->stop();
 Slot_On_webView_Biblio_loadFinished(true);
}

//------------------------------------ runMacroForCurrentItem -------------------------------
QString C_MW_Prescription::runMacroForCurrentItem()
{   QWidget *pQWidget                   =  m_pGUI->toolBox_ListesProduits->currentWidget();
    QTreeWidgetItem *pQTreeWidgetItem   =  0;
    if (pQWidget->objectName()=="page_Produits")
       { pQTreeWidgetItem = m_pGUI->treeWidget_Produits->currentItem();
       }
    else if (pQWidget->objectName()=="page_Favoris")
       { pQTreeWidgetItem = m_pGUI->treeWidget_Favoris->currentItem();
       }
    else if (pQWidget->objectName()=="page_CustomList")
       { pQTreeWidgetItem = m_pGUI->treeWidget_CustomDrugsList->currentItem();
       }

    if (pQTreeWidgetItem==0) return"";
    return runMacroForCurrentItem(pQTreeWidgetItem);
}
//------------------------------------ runMacroForCurrentItem -------------------------------
QString C_MW_Prescription::runMacroForCurrentItem(QTreeWidgetItem *pQTreeWidgetItem)
{if (pQTreeWidgetItem==0) return QString::null;
 int index =   m_pGUI->comboBox_BiblioURL->currentIndex();
 if (index==-1)           return QString::null;
 QString biblioType  = ""; // m_pGUI->comboBox_BiblioURL->itemData(index).toString();
 QString biblioUrl   = ""; // m_pGUI->comboBox_BiblioURL->itemText(index);
 QString biblioLogin = "";
 QString biblioPassw = "";
 get_BiblioType_Url_Login_Pass_FromComboBiblio(index, &biblioType, &biblioUrl,&biblioLogin, &biblioPassw);

 QString  dci1           = pQTreeWidgetItem->text(COL_DCI_1);
 QString  dci2           = pQTreeWidgetItem->text(COL_DCI_2);
 QString  dci3           = pQTreeWidgetItem->text(COL_DCI_3);
 QString  atc            = pQTreeWidgetItem->text(COL_ATC);
 QString  cip            = pQTreeWidgetItem->text(COL_ID);
 QString  cis            = ""; // CApp::pCApp()->BDM_getDB()->get_CIS_From_CIP(cip);           // id est CIP en attendant mieux
 QString  rcp            = ""; // CApp::pCApp()->BDM_getDB()->get_RCP_From_CIS(cis);
 if (m_pC_BDM_Api)
    {     cis            = m_pC_BDM_Api->get_CIS_and_RCP_From_CIP(cip, &rcp);
      if ( atc.length()==0 && cip.length() )
          atc            = m_pC_BDM_Api->ATC_from_CIP(cip);
    }
 QString macro           = CGestIni::Param_UpdateFromDisk(CApp::pCApp()->pathAppli() + "Ressources/web/" + biblioType + ".txt");
 QString specName        = pQTreeWidgetItem->text(COL_PROD_NAME);
 index                   = specName.indexOf("/");
 if (index==-1) index    = specName.indexOf(",");
 if (index!=-1) specName = specName.left(index);

 macro.replace("{{DCI1}}",        dci1);
 macro.replace("{{DCI2}}",        dci2);
 macro.replace("{{DCI3}}",        dci3);
 macro.replace("{{ATC}}",         atc);
 macro.replace("{{RCP}}",         rcp);
 macro.replace("{{CIS}}",         cis);
 macro.replace("{{CIP7}}",        cip);
 macro.replace("{{PASSWORD}}",    biblioPassw);
 macro.replace("{{LOGIN}}",       biblioLogin);
 macro.replace("{{ITEM_NAME}}",   C_Utils_Html::protectSymbolByAntiSlash(C_Utils_Html::protectSymbolByAntiSlash(C_Utils_Html::protectSymbolByAntiSlash(pQTreeWidgetItem->text(COL_PROD_NAME)),'('),')'));
 macro.replace("{{SPEC_NAME}}",   specName);
 macro.replace("{{MACRO NAME}}",  biblioType + ".txt");
 return  CApp::pCApp()->run(&macro) ;

 //C_Utils_Log::outMessage( m_pGUI->textEdit_Monitor, test);
}

//------------------------------------ Slot_On_UpdateCache -------------------------------
void C_MW_Prescription::Slot_On_UpdateCache(bool)
{QTreeWidgetItem *pQTreeWidgetItem = m_pGUI->treeWidget_Produits->currentItem();
 if (pQTreeWidgetItem==0) return ;
 int index =   m_pGUI->comboBox_BiblioURL->currentIndex();
 if (index==-1)           return ;
 //......... desenabler les controles ..............
 m_pGUI->treeWidget_Produits->setEnabled(false);
 m_pGUI->pushButton_RetrySite->setEnabled(false);
 m_pGUI->pushButton_UpdateCache->setEnabled(false);
 m_pGUI->pushButton_AllItems->setEnabled(false);
 QString biblioType  = ""; // m_pGUI->comboBox_BiblioURL->itemData(index).toString();
 QString biblioUrl   = ""; // m_pGUI->comboBox_BiblioURL->itemText(index);
 QString biblioLogin = "";
 QString biblioPassw = "";
 get_BiblioType_Url_Login_Pass_FromComboBiblio(index, &biblioType, &biblioUrl,&biblioLogin, &biblioPassw);
 QString  dci1          = pQTreeWidgetItem->text(COL_DCI_1);
 QString  dci2          = pQTreeWidgetItem->text(COL_DCI_2);
 QString  dci3          = pQTreeWidgetItem->text(COL_DCI_3);
 QString  atc           = pQTreeWidgetItem->text(COL_ATC);
 QString  cip           = pQTreeWidgetItem->text(COL_ID);
 QString  cis           = ""; // CApp::pCApp()->BDM_getDB()->get_CIS_From_CIP(cip);           // id est CIP en attendant mieux
 QString  rcp           = ""; // CApp::pCApp()->BDM_getDB()->get_RCP_From_CIS(cis);
 if (m_pC_BDM_Api)
    {     cis           = m_pC_BDM_Api->get_CIS_and_RCP_From_CIP(cip, &rcp);
      if ( atc.length()==0 && cip.length() )
           atc          = m_pC_BDM_Api->ATC_from_CIP(cip);
    }
 QString macroFileName  = biblioType + "_Update.txt";
 QString macro          = CGestIni::Param_UpdateFromDisk(CApp::pCApp()->pathAppli() + "Ressources/web/" + macroFileName);
 QString specName       = pQTreeWidgetItem->text(COL_PROD_NAME);
 index                  = specName.indexOf("/");
 if (index==-1) index   = specName.indexOf(",");
 if (index!=-1)specName = specName.left(index);

 if (macro.length()==0)
    {C_Utils_Log::outMessage( m_pGUI->textEdit_Monitor, tr("Macro : %1").arg(macroFileName));
    }
 else
   {
     macro.replace("{{DCI1}}",        dci1);
     macro.replace("{{DCI2}}",        dci2);
     macro.replace("{{DCI3}}",        dci3);
     macro.replace("{{ATC}}",         atc);
     macro.replace("{{RCP}}",         rcp);
     macro.replace("{{CIS}}",         cis);
     macro.replace("{{CIP7}}",        cip);
     macro.replace("{{PASSWORD}}",    biblioPassw);
     macro.replace("{{LOGIN}}",       biblioLogin);
     macro.replace("{{ITEM_NAME}}",   C_Utils_Html::protectSymbolByAntiSlash(C_Utils_Html::protectSymbolByAntiSlash(C_Utils_Html::protectSymbolByAntiSlash(pQTreeWidgetItem->text(COL_PROD_NAME)),'('),')'));
     macro.replace("{{MACRO NAME}}",  macroFileName);
     macro.replace("{{SPEC_NAME}}",   specName);
    /*QString ret = */ CApp::pCApp()->run(&macro);
   }
 //......... enabler les controles ..............
 m_pGUI->treeWidget_Produits->setEnabled(true);
 m_pGUI->pushButton_RetrySite->setEnabled(true);
 m_pGUI->pushButton_UpdateCache->setEnabled(true);
 m_pGUI->pushButton_AllItems->setEnabled(true);
}

//------------------------------------ Slot_On_pushButton_RetrySite -------------------------------
void C_MW_Prescription::Slot_On_pushButton_RetrySite(bool)
{   QWidget *pQWidget                   =  m_pGUI->toolBox_ListesProduits->currentWidget();
    QTreeWidgetItem *pQTreeWidgetItem   =  0;
    if (pQWidget->objectName()=="page_Produits")
       { pQTreeWidgetItem = m_pGUI->treeWidget_Produits->currentItem();
       }
    else if (pQWidget->objectName()=="page_Favoris")
       { pQTreeWidgetItem = m_pGUI->treeWidget_Favoris->currentItem();
       }
    if (pQTreeWidgetItem==0) return;
    getBiblioForItemEnCours(1, pQTreeWidgetItem);   // forcer le reload (meme si deja en base)
}

//------------------------------------ getBiblioForItemEnCours -------------------------------
void C_MW_Prescription::getBiblioForItemEnCours(bool forceCache, QTreeWidgetItem *pQTreeWidgetItem)
{
    //QTreeWidgetItem *pQTreeWidgetItem = m_pGUI->treeWidget_Produits->currentItem();
    if (pQTreeWidgetItem==0) return;
    int index =   m_pGUI->comboBox_BiblioURL->currentIndex();
    if (index==-1)          return;
    //......... desenabler les controles ..............
    m_pGUI->treeWidget_Produits->setEnabled(false);
    m_pGUI->pushButton_RetrySite->setEnabled(false);
    m_pGUI->pushButton_UpdateCache->setEnabled(false);
    m_pGUI->pushButton_AllItems->setEnabled(false);
    QString ret         = "";
    QString biblioType  = ""; // m_pGUI->comboBox_BiblioURL->itemData(index).toString();
    QString biblioUrl   = ""; // m_pGUI->comboBox_BiblioURL->itemText(index);
    QString biblioLogin = "";
    QString biblioPassw = "";
    get_BiblioType_Url_Login_Pass_FromComboBiblio(index, &biblioType, &biblioUrl,&biblioLogin, &biblioPassw);

    QString  specName        = pQTreeWidgetItem->text(COL_PROD_NAME);
    index                    = specName.indexOf("/");
    if (index==-1) index     = specName.indexOf(",");
    if (index!=-1) specName  = specName.left(index);
    QString  dci1            = pQTreeWidgetItem->text(COL_DCI_1);
    QString  dci2            = pQTreeWidgetItem->text(COL_DCI_2);
    QString  dci3            = pQTreeWidgetItem->text(COL_DCI_3);
    QString  atc             = pQTreeWidgetItem->text(COL_ATC);
    QString  cip             = pQTreeWidgetItem->text(COL_ID);
    QString  cis             = ""; // CApp::pCApp()->BDM_getDB()->get_CIS_From_CIP(cip);           // id est CIP en attendant mieux
    QString  rcp             = ""; // CApp::pCApp()->BDM_getDB()->get_RCP_From_CIS(cis);
    if (m_pC_BDM_Api)
       {     cis             = m_pC_BDM_Api->get_CIS_and_RCP_From_CIP(cip, &rcp);
         if ( atc.length()==0 && cip.length() )
             atc             = m_pC_BDM_Api->ATC_from_CIP(cip);
       }
    biblioUrl.replace("{{DCI1}}",dci1);
    biblioUrl.replace("{{DCI2}}",dci2);
    biblioUrl.replace("{{DCI3}}",dci3);
    biblioUrl.replace("{{ATC}}",atc);
    biblioUrl.replace("{{RCP}}",rcp);
    biblioUrl.replace("{{CIS}}",cis);
    biblioUrl.replace("{{CIP7}}",cip);
    biblioUrl.replace("{{ITEM_NAME}}",pQTreeWidgetItem->text(COL_PROD_NAME));
    biblioUrl.replace("{{SPEC_NAME}}",specName);
    //........................ chercher enregistrement biblio ..............................
    C_BiblioData cb;
    //                         pour ce type de medoc
    // cb   = CApp::pCApp()->BDM_getDB()->biblio_getDataFrom_TypeLangId(pQTreeWidgetItem->text(COL_ID), "fr", biblioType);
    if (cb.m_pk.length())                                                                                          //........... voir si  y a pas des donnees a mettre en cache .......
       { if (cb.m_data.length()==0 || forceCache)                                                                                // il ya des donnees de header (dont l'url) mais pas dans le blob
            {C_Utils_Log::outMessage( m_pGUI->textEdit_Monitor, tr(" Head found but data not Found in database :id=%1 lang=%2 biblioType=%3 %1").arg(pQTreeWidgetItem->text(COL_ID),"fr",biblioType));
             ret = runMacroForCurrentItem();
            }
        else  // ......... on prend le contenu du cache ..............................
            { QString baseUrl = CApp::pCApp()->pathAppli() + "Ressources/web/"+biblioType+"/";
              baseUrl = baseUrl.prepend("file:");
              if (m_webView_Biblio) m_webView_Biblio->setHtml(cb.m_data, QUrl(baseUrl));
              // CGestIni::Param_UpdateToDisk("/home/ro/html.html", cb.m_data);
              m_pGUI->lineEdit_url->setText(cb.m_url);
              C_Utils_Log::outMessage( m_pGUI->textEdit_Monitor, tr(" Find in database :id=%1 lang=%2 biblioType=%3 %1").arg(pQTreeWidgetItem->text(COL_ID),"fr",biblioType));
              //......... enabler les controles ..............
              m_pGUI->treeWidget_Produits->setEnabled(true);
              m_pGUI->pushButton_RetrySite->setEnabled(true);
              m_pGUI->pushButton_UpdateCache->setEnabled(true);
              m_pGUI->pushButton_AllItems->setEnabled(true);
              return;
            }
       }
    //....................... pas de biblio pour ce topic ...................................
    //                        faut le creer
    else
      { C_Utils_Log::outMessage( m_pGUI->textEdit_Monitor, tr(" Nothing Found in database :id=%1 lang=%2 biblioType=%3 %1").arg(pQTreeWidgetItem->text(COL_ID),"fr",biblioType));
        ret = runMacroForCurrentItem();
      }
    C_Utils_Log::outMessage( m_pGUI->textEdit_Monitor, tr("Retour : %1").arg(ret));
    m_pGUI->lineEdit_url->setText(biblioUrl);
    if ( ! can_use_API( pQTreeWidgetItem ) )
       { Slot_LocationEdit_returnPressed();
       }
    //......... enabler les controles ..............
    m_pGUI->treeWidget_Produits->setEnabled(true);
    m_pGUI->pushButton_RetrySite->setEnabled(true);
    m_pGUI->pushButton_UpdateCache->setEnabled(true);
    m_pGUI->pushButton_AllItems->setEnabled(true);
}

//------------------------------------ fill_treeWidget_Favoris --------------------------------------------------
int C_MW_Prescription::fill_treeWidget_Favoris(const QString &text /*=""*/)
{    QList < C_BDM_DrugListRecord >  drugsList;
     if (m_pC_BDM_Api==0)      return 0;
     m_pGUI->treeWidget_Favoris->clear();
     drugsList =  m_pC_BDM_Api->selectFavorisList (text, C_BDM_PluginI::cip_filter ,10000);  // au delas de 10000 on ne peut plus appeler
     fill_treeWidget_ProductsFromDrugList(m_pGUI->treeWidget_Favoris, drugsList);
    return m_pGUI->treeWidget_Favoris->topLevelItemCount();
}

//------------------------ Slot_FavorisAdded ---------------------------------------------------------------------------------------
// ACTION :      // mettre a jour la liste des favoris
void C_MW_Prescription::Slot_FavorisAdded(C_Frm_Produit *)
{        fill_treeWidget_Favoris();
}
//------------------------------------ Slot_checkBox_DrugList_filter_stateChanged --------------------------------------------------
void C_MW_Prescription::Slot_checkBox_DrugList_filter_stateChanged(const QString &mappedText)
{
    QCheckBox *pQCheckBox = qobject_cast<QCheckBox *>(m_DrugListFilter_SignalMapper->mapping (mappedText ));
    if (pQCheckBox->isChecked()) CApp::pCApp()->writeParam("medicatux filter",mappedText.toLatin1(),tr("actif").toLatin1() );
    else                         CApp::pCApp()->writeParam("medicatux filter",mappedText.toLatin1(),tr("non actif").toLatin1() );
    initDrugListFilter();
    Slot_lineEdit_DrugsSearch_textChanged (m_pGUI->lineEdit_DrugsSearch->text()); //on remet a jour la liste
}

//------------------------------------ Slot_ProductNumberClicked --------------------------------------------------
void C_MW_Prescription::Slot_ProductNumberClicked(C_Frm_Produit *pC_Frm_Produit)
{ show_ProductMonographie(*pC_Frm_Produit);
}
//------------------------------------ show_ProductMonographie --------------------------------------------------
void C_MW_Prescription::show_ProductMonographie(const C_BDM_DrugListRecord & drugListRecord)
{
 QWidget                 *pTab_QWidget = m_pGUI->tab_MainTabWidget->tabWidgetByTabObjectName("tab_Monographie");      // zero si retire
 if ( pTab_QWidget == 0 ) pTab_QWidget = m_pGUI->tab_MainTabWidget->activeTabWidgetByObjectName("tab_Monographie");   // aller chercher dans la liste des retires
 if ( pTab_QWidget == 0 ) return;
 m_pGUI->tab_MainTabWidget->setCurrentWidget ( pTab_QWidget );
 int      i           = 0;
 QString id           = drugListRecord.id();
 QTime tm_deb         = QTime::currentTime();
 QString html         = m_pC_BDM_Api->getMonographieFromCIP(id);                      // recuperer monographie standard de la bdm medicamenteuse
 C_Utils_Log::outMessage( m_pGUI->textEdit_Monitor, tr(" Time to get BDM    Monographie of %1 = %2").arg(id,  QTime (0, 0, 0, 0 ).addMSecs(tm_deb.msecsTo(QTime::currentTime())).toString("mm:ss:zzz") ));
 QTime tm_tmp         = QTime::currentTime();
 QStringList htmlList = m_pC_BDM_Api-> selectCustomMonographie(drugListRecord);       // recuperer la liste des notes personnelles
 bool  noLast_hr_tag  = html.length()==0 || htmlList.size()==0;                       // flag d'ajout du tag <hr/>
 //.......... inserer les liens des pages custom ...............................
 QString link_str     = "";  // liens crees sur les notes personnelles
 QString part_str     = "";  // texte correspondant a une note
 QString cible        = "";  // texte correspondant la cible d'une note
 int      pos_deb     = -1;
 int      pos_end     = -1;

 if (htmlList.size())           // on parse toutes les notes pour en extraire les infos pour creer les liens
    {
     // html  +=  "<hr/><p align=\"center\"><font color=#0000ff><b>NOTES COMPL&#201;MENTAIRES PERSONNELLES</b></font></p>";
     for (i=0; i<htmlList.size(); ++i)    // on merge toutes les notes recuperees par la requete SQL en installant les liens adequats
         { part_str = htmlList.at(i);                            // recuperer la liste des notes
           pos_deb  = part_str.indexOf("<a name=\"ancre_id_[");
           if ( pos_deb != -1 )
              { pos_deb += 19;
                pos_end  = part_str.indexOf("]_\"></a><br />",pos_deb);
              }
           if ( pos_end != -1 )
              { cible     = part_str.mid(pos_deb, pos_end-pos_deb);
                link_str += "&nbsp;&nbsp;&nbsp;" // ATTENTION en 4.8.5 conversion possible des majuscules en minuscules ==> nom des ancres toujours en minuscules
                            "&nbsp;&nbsp;<a href=\"modify:#ancre_id_[" + cible + "]_\"><img src=\"./EditPicto.png\"> modifier</a>"   +  // ce lien sera a interpreter dans URL_ExecSpecialSyntax
                            "&nbsp;&nbsp;<a href=\"remove:#ancre_id_[" + cible + "]_\"><img src=\"./RemovePicto.png\"> effacer</a>"  +  // ce lien sera a interpreter dans URL_ExecSpecialSyntax
                            "&nbsp;&nbsp;<a href=\"#ancre_id_["        + cible + "]_\"><img src=\"./NotePicto.png\"> voir la note : <font size=2px color=#ff0000>" + CGestIni::CutStrRight( cible,"-" ) + "</font></a>"+ // ce lien est actif
                            "<br />";    // lien sur la cible, le du lien texte est coupe apres les informations du pk
                html     +=  part_str + "<br />"; // joindre la note a la page
              }
         }
    }
 html  = "<a name=\"monographie_start\"></a>"             // placer lancre de debut de page
         "<hr/><p align=\"center\"><font color=#0000ff><b>NOTES COMPL&#201;MENTAIRES PERSONNELLES</b></font>&nbsp;&nbsp;"
         "<a href=\"new:#ancre_id_[note]_\"><img src=\"./NewNotePicto.png\"> ajouter</a></p><hr/>" + link_str + (noLast_hr_tag?"":"<hr/>") + html;  // on place les liens au debut de page puis la monographie standard suivieie des notes
 C_Utils_Log::outMessage( m_pGUI->textEdit_Monitor, tr(" Time to get CUSTOM Monographie of %1 = %2").arg(id,  QTime (0, 0, 0, 0 ).addMSecs(tm_tmp.msecsTo(QTime::currentTime())).toString("mm:ss:zzz") ));
 C_Utils_Log::outMessage( m_pGUI->textEdit_Monitor, tr(" Time to get TOTAL  Monographie of %1 = %2").arg(id,  QTime (0, 0, 0, 0 ).addMSecs(tm_deb.msecsTo(QTime::currentTime())).toString("mm:ss:zzz") ));
 if (m_webView_Mono) m_webView_Mono->setHtml(html, QUrl::fromLocalFile(m_pC_BDM_Api->plugin_image_source() ));
 m_last_drugListRecord = drugListRecord;
}

//------------------------------------ initDrugListFilter --------------------------------------------------
void C_MW_Prescription::initDrugListFilter()
{   if (m_pC_BDM_Api==0) return;
    int flags = C_BDM_Api::no_filter;
    if (m_pGUI->checkBox_Atc_filter->isChecked()) flags |= C_BDM_Api::atc_filter;
    if (m_pGUI->checkBox_Nom_filter->isChecked()) flags |= C_BDM_Api::nom_filter;
    if (m_pGUI->checkBox_Ucd_filter->isChecked()) flags |= C_BDM_Api::ucd_filter;
    if (m_pGUI->checkBox_Dci_filter->isChecked()) flags |= C_BDM_Api::dci_filter;
    if (m_pGUI->checkBox_Dc3_filter->isChecked()) flags |= C_BDM_Api::dc3_filter;
    if (m_pGUI->checkBox_Cip_filter->isChecked()) flags |= C_BDM_Api::cip_filter;
    if (m_pGUI->checkBox_Gen_filter->isChecked()) flags |= C_BDM_Api::gen_filter;
    if (m_pGUI->checkBox_Hop_filter->isChecked()) flags |= C_BDM_Api::hop_filter;
    if (m_pGUI->checkBox_Vil_filter->isChecked()) flags |= C_BDM_Api::vil_filter;
    if (!flags) flags |= C_BDM_Api::nom_filter;
    setDrugListFilter( (C_BDM_PluginI::flags)flags );
}
//-------------------------------- setDrugListFilter ------------------------------------------------------
void  C_MW_Prescription::setDrugListFilter(C_BDM_PluginI::flags flags)
{m_drugListFilter = flags;
}
//-------------------------------- getDrugListFilter ------------------------------------------------------
C_BDM_PluginI::flags C_MW_Prescription::getDrugListFilter()
{ return m_drugListFilter;
}
//------------------------------------ Slot_lineEdit_DrugsSearch_textChanged -------------------------------
void C_MW_Prescription::Slot_lineEdit_DrugsSearch_textChanged (const QString &text)
{if (m_pC_BDM_Api==0) return;
 int nb = m_pC_BDM_Api->selectProductsList(m_pGUI->treeWidget_Produits, text, getDrugListFilter(),m_drugListMax);
 m_pGUI->label_ListMedoc->setText(tr("%1 sur %2 produit(s)").arg(QString::number(nb),QString::number(m_pC_BDM_Api->drugsList_Size())));
}

//------------------------------------ Slot_m_action_Deplie -------------------------------
void C_MW_Prescription::Slot_m_action_Deplie (bool)
{expand_QTreeWidget_GUI ( true );
}

//------------------------------------ Slot_m_action_Replie -------------------------------
void C_MW_Prescription::Slot_m_action_Replie (bool)
{  expand_QTreeWidget_GUI ( false );
}
//------------------------------------ expand_QTreeWidget_GUI -------------------------------
void C_MW_Prescription::expand_QTreeWidget_GUI ( bool state)
{   expand_QTreeWidget(m_pGUI->treeWidget_ATC,state);
}
//------------------------------------ expand_QTreeWidget -------------------------------
void C_MW_Prescription::expand_QTreeWidget( QTreeWidget *pQTreeWidget, bool state)
{  QTreeWidgetItemIterator it(pQTreeWidget);
    while (*it)
     { (*it)->setExpanded(state);
       ++it;
     }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// WEB VIEW ///////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//--------------------------------- Slot_webView_linkClicked -----------------------------------------------------------------------
void C_MW_Prescription::Slot_webView_linkClicked ( const QUrl & url )
{if (URL_ExecSpecialSyntax ( url.toString().trimmed() ,CApp::pCApp()->pathAppli())) return;
if (m_webView_Biblio)  m_webView_Biblio->load (url );
}
/*
//--------------------------------- Slot_webView_Mono_urlChanged -----------------------------------------------------------------------
void C_MW_Prescription::Slot_webView_Mono_urlChanged( const QUrl & url )
{// m_webView_Mono->load (url );
}
*/
//--------------------------------- Slot_webView_Mono_linkClicked -----------------------------------------------------------------------
void C_MW_Prescription::Slot_webView_Mono_linkClicked( const QUrl & url )
{ QString urlText = url.toString();
 if (URL_ExecSpecialSyntax (urlText.trimmed() ,CApp::pCApp()->pathAppli())) return;
 //qDebug()<<urlText;
 int pos = 0;
 if ( ( pos=urlText.indexOf("Drug_CIP=") ) != -1)
    { int nb = m_pC_BDM_Api->selectProductsList(m_pGUI->treeWidget_Produits, urlText.mid(pos+9), getDrugListFilter());
      m_pGUI->label_ListMedoc->setText(tr("%1 sur %2 produit(s)").arg(QString::number(nb),QString::number(m_pC_BDM_Api->drugsList_Size())));
    }
  if (m_webView_Mono)  m_webView_Mono->load (url );
}

//--------------------------------- Slot_On_webView_Biblio_statusBarMessage -----------------------------------------------------------------------
void C_MW_Prescription::Slot_On_webView_Biblio_statusBarMessage (const QString &text)
{m_pQLabel->setText(text);
}

//--------------------------------- Slot_On_webView_Biblio_loadStarted -----------------------------------------------------------------------
void C_MW_Prescription::Slot_On_webView_Biblio_loadStarted()
{m_pQProgressBar->setValue(0);
 m_pQProgressBar->show();
 m_pQLabel->show();
}

//--------------------------------- Slot_On_webView_Biblio_loadProgress -----------------------------------------------------------------------
void C_MW_Prescription::Slot_On_webView_Biblio_loadProgress(int progress)
{m_pQProgressBar->setValue(progress);
 m_pGUI->pushButton_Stop->setEnabled(true);
 m_pQProgressBar->show();
 m_pQLabel->show();
}

//--------------------------------- Slot_On_webView_Biblio_loadFinished -----------------------------------------------------------------------
void C_MW_Prescription::Slot_On_webView_Biblio_loadFinished(bool )
{//m_pQProgressBar->hide();
 //m_pQLabel->hide();
 m_pGUI->pushButton_Stop->setEnabled(false);
 CApp::pCApp()->setEndPageDisplay(true);
}

//--------------------------------- Slot_LocationEdit_returnPressed -----------------------------------------------------------------------
void C_MW_Prescription::Slot_LocationEdit_returnPressed()
{   QUrl url = QUrl(m_pGUI->lineEdit_url->text());
    if (m_webView_Biblio) m_webView_Biblio->setUrl (QUrl(url));
}
//--------------------------------- Slot_WebFindEdit_returnPressed -----------------------------------------------------------------------
void C_MW_Prescription::Slot_WebFindEdit_returnPressed()
{Slot_actionWebFind (true);
}

//--------------------------------- Slot_WebFindEdit_textChanged -----------------------------------------------------------------------
void C_MW_Prescription::Slot_WebFindEdit_textChanged (const QString &text )
{if (m_webView_Biblio==0) return;
 m_webView_Biblio->findText ("",   QWebPage::HighlightAllOccurrences );  // clear the selection
 m_webView_Biblio->findText (text, QWebPage::HighlightAllOccurrences );
}

//--------------------------------- Slot_actionWebFind -----------------------------------------------------------------------
void C_MW_Prescription::Slot_actionWebFind (bool)
{if (m_webView_Biblio) m_webView_Biblio->findText (m_pGUI->lineEdit_find->text(), QWebPage::FindWrapsAroundDocument );
}
//--------------------------------- Slot_actionWebPrint -----------------------------------------------------------------------
void C_MW_Prescription::Slot_actionWebPrint (bool)
{
 #ifndef QT_NO_PRINTER
     QPrinter printer(QPrinter::HighResolution);
     QPrintDialog *printDialog = new QPrintDialog(&printer, this);
     if (printDialog->exec() == QDialog::Accepted)
        {     if (m_webView_Biblio) m_webView_Biblio->print ( &printer );
        }
 #endif // QT_NO_PRINTER
}

//--------------------------------- Slot_actionWebHome -----------------------------------------------------------------------
void C_MW_Prescription::Slot_actionWebHome (bool)
{   QString  pathPageHelp = QString("../../Doc/%1/index.html").arg(CApp::pCApp()->applicationName());
    if ( QDir(pathPageHelp).isRelative()) {pathPageHelp.prepend(CApp::pCApp()->pathAppli()); pathPageHelp = QDir::cleanPath(pathPageHelp);}
    if (m_webView_Biblio) m_webView_Biblio->setUrl (QUrl(pathPageHelp));
}

//--------------------------------- Slot_webView_urlChanged -----------------------------------------------------------------------
void C_MW_Prescription::Slot_webView_urlChanged ( const QUrl &url  )
{m_pGUI->lineEdit_url->setText(url.toString());
}


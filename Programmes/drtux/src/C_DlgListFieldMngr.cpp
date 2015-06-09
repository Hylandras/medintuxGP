 /*********************** C_DlgListFieldMngr.cpp ***************************
 *  #include "C_DlgListFieldMngr.h"           QT3                          *
 * ........................................................................*
 *   Project   :  MedinTux  (typographie made in france)                   *
 *   Copyright :  (C) 2005-2006-2007-2008-2009-2010 and for the eternity   *
 *   by        :  Sevin Roland     from   MedinTux project                 *
 *   E-Mail    :  roland-sevin@medintux.org                                *
 *   Web site  :  www.medintux.org                                         *
 * ........................................................................*
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "../../drtux/src/C_DlgListFieldMngr.h"

#include <qdir.h>
#include <qpixmap.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qlistbox.h>
#include <qapplication.h>
#include <qobject.h>
#include <qevent.h>
#include <qcursor.h>
#include <qwhatsthis.h>
#include <qfontdatabase.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qmessagebox.h>
#include <qsplitter.h>
#include <qcolordialog.h>

#ifndef QLIGHTPAD
  #include "../../drtux/src/CApp.h"
  #include "../../drtux/src/CMDI_Prescription.h"
  #include "../../drtux/src/CMDI_Observation.h"
  #include "../../drtux/src/CMDI_Terrain.h"
  #include "../../drtux/src/CMDI_Ident.h"
  #include "../../drtux/src/CMDI_ChoixPatient.h"
  #include "../../drtux/src/CMDI_Generic.h"
  #include "../../drtux/src/ui/DlgListOff_Listes.h"
  #include "../../drtux/src/ui/DLG_InsertList.h"
#else
  #include "ui/DLG_InsertList.h"
  #include "ui/DlgListOff_Listes.h"
#endif

#include "../../drtux/src/C_KeyPressEater.h"


 //#include "ui/DlgGetString.h"
 //#include "ui/DlgListFieldMngr.h"
 //#include "ui/DlgMyMessageBox.h"

#include "../../MedinTuxTools/Theme.h"
#include "../../MedinTuxTools/ThemePopup.h"
#include "../../MedinTuxTools/CHtmlTools.h"
#include "../../MedinTuxTools/CPreview.h"
#include "../../MedinTuxTools/CGenTools.h"
#include "../../MedinTuxTools/CGestIni.h"
#include "../../MedinTuxTools/CW_EditText.h"

#define INSERTION_ROOT            "/Champs d'insertions"

//-------------------------------- C_DlgListFieldMngr --------------------------------------
C_DlgListFieldMngr::C_DlgListFieldMngr(QWidget* parent, const char* name, bool modal, WFlags fl)
: FormDlgListFieldMngr(parent,name, modal,fl)
{   m_pRet = 0;
    //............. remplir combo des polices ..................................
    QFontDatabase db;
    comboBoxFont->insertStringList( db.families() );
    connect( comboBoxFont,  SIGNAL( activated( const QString & ) ), textEditField, SLOT( setFamily( const QString & ) ) );
    comboBoxFont->lineEdit()->setText( QApplication::font().family() );

    //............. remplir combo des tailles ..................................
    QValueList<int> sizes = db.standardSizes();
    QValueList<int>::Iterator its = sizes.begin();
    for ( ; its != sizes.end(); ++its )
              comboBoxSize->insertItem( QString::number( *its ) );
    connect( comboBoxSize, SIGNAL( activated( const QString & ) ),  this,         SLOT( textSize( const QString & ) ) );
    comboBoxSize->lineEdit()->setText( QString::number( QApplication::font().pointSize() ) );

    //............. positionner les boutons d'enrichissement du texte ...........
    pushButtonColor->setPaletteForegroundColor (QColor(7,0,0));
    pushButtonColor->setPaletteBackgroundColor (QColor(7,0,0));
    buttonGroupJustif->setExclusive ( TRUE ) ;
    pushButtonSave->setDisabled(TRUE);
    textEditField->setEnabled (FALSE );
    doConnections( textEditField );
    m_OldpQlistViewItem = 0;
    m_pQListViewItem    =  0;
    pushButton_Quitter->setText(tr("&Quitter"));
    pushButton_Ok->hide();
    QWhatsThis ::remove ( listViewList );
    connect( listViewList,                    SIGNAL( doubleClicked(QListViewItem*) ),                                 this, SLOT( listViewList_doubleClicked(QListViewItem*) ) );
    connect( listViewList,                    SIGNAL( mouseButtonPressed (int, QListViewItem*, const QPoint&, int ) ), this, SLOT( listViewList_clicked(int, QListViewItem*, const QPoint&, int) ) );
    connect( listViewList,                    SIGNAL( returnPressed(QListViewItem*) ),                                 this, SLOT( listViewList_returnPressed(QListViewItem*) ) );
    connect( listView_RubMenus,               SIGNAL( clicked ( QListViewItem *  )) ,                                  this, SLOT( listView_RubMenus_clicked(QListViewItem*) ) );
    connect( lineEditFindItem,                SIGNAL( textChanged(const QString&) ),                                   this, SLOT( lineEditFindItem_textChanged(const QString&) ) );
    connect( pushButtonColor,                 SIGNAL( clicked() ), this, SLOT( pushButtonColor_clicked() ) );
    connect( buttonGroupJustif,               SIGNAL( pressed(int) ), this, SLOT( buttonGroupJustif_pressed(int) ) );
    connect( pushButtonItalique,              SIGNAL( toggled(bool) ), this, SLOT( pushButtonItalique_toggled(bool) ) );
    connect( pushButtonBold,                  SIGNAL( toggled(bool) ), this, SLOT( pushButtonBold_toggled(bool) ) );
    connect( pushButtonUnderline,             SIGNAL( toggled(bool) ), this, SLOT( pushButtonUnderline_toggled(bool) ) );
    connect( pushButtonSave,                  SIGNAL( clicked() ), this, SLOT( pushButtonSave_clicked() ) );
    connect( textEditField,                   SIGNAL( textChanged() ), this, SLOT( textEditField_textChanged() ) );
    connect( textEditField,                   SIGNAL( currentAlignmentChanged(int) ), this, SLOT( alignmentChanged(int) ) );
    connect( textEditField,                   SIGNAL( currentColorChanged(const QColor&) ), this, SLOT( colorChanged(const QColor&) ) );
    connect( textEditField,                   SIGNAL( currentFontChanged(const QFont&) ), this, SLOT( fontChanged(const QFont&) ) );
    connect( pushButtonSetMasque,             SIGNAL( clicked() ), this, SLOT( pushButtonSetMasque_clicked() ) );
    connect( pushButtonImportMenu,            SIGNAL( clicked() ), this, SLOT( pushButtonImportMenu_clicked() ) );
    connect( pushButtonImportMenuLocalize,    SIGNAL( clicked() ), this, SLOT( pushButtonImportMenuLocalize_clicked() ) );
    connect( pushButton_InsertImage,          SIGNAL( clicked() ), this, SLOT( pushButton_InsertImage_clicked() ) );
    connect( listViewList,                    SIGNAL( spacePressed(QListViewItem*) ), this, SLOT( listViewList_spacePressed(QListViewItem*) ) );
    //connect( listViewList,            SIGNAL( rightButtonClicked(QListViewItem*,const QPoint&,int) ),   this, SLOT( listViewList_rightButtonClicked(QListViewItem*,const QPoint&,int) ) );
    //connect( listViewList,            SIGNAL( contextMenuRequested(QListViewItem*,const QPoint&,int) ), this, SLOT( listViewList_contextMenuRequested(QListViewItem*,const QPoint&,int) ) );
    connect( pushButtonSetTextField,  SIGNAL( clicked() ),                                              this, SLOT( pushButtonSetTextField_clicked() ) );
    connect( lineEditFindItem,        SIGNAL( returnPressed() ),                                        this, SLOT( lineEditFindItem_returnPressed() ) );
    connect( pushButton_Quitter,      SIGNAL( clicked() ),                                              this, SLOT( pushButton_Quitter_clicked() ) );
    connect( pushButton_Ok,           SIGNAL( clicked() ),                                              this, SLOT( pushButton_Ok_clicked() ) );
    #ifdef QLIGHTPAD
    listView_RubMenus->hide();
    textLabel_MenusFixes->hide();
    #endif
}

//-------------------------------- C_DlgListFieldMngr --------------------------------------
C_DlgListFieldMngr::~C_DlgListFieldMngr()
{
}

//------------------------------------------------------- setReturnVal -------------------------------------
void C_DlgListFieldMngr::setReturnVal(QString *ret)
{m_pRet    = ret;
    pushButton_Quitter->setText(tr("&Annuler"));
    pushButton_Ok->show();
}

//------------------------------------------------------- setReturnValList -------------------------------------
void C_DlgListFieldMngr::setReturnValList(QString *ret)
{m_pRet    = ret;
    pushButton_Ok->setText(tr("&Changer la liste déroulante"));
    pushButton_Ok->show();
}
//------------------------------------------------------- selectAndOpenItem -------------------------------------
void C_DlgListFieldMngr::selectAndOpenItem(QString itemName)
{ QListViewItem *pQListViewItem = FindDepthItem(itemName, 0);
    if (pQListViewItem)
    {listViewList->setCurrentItem(pQListViewItem);
              listViewList->ensureItemVisible (pQListViewItem);
              listViewList_doubleClicked(pQListViewItem);
              QListViewItem * myChild = pQListViewItem->firstChild();
              while( myChild )
              { listViewList->ensureItemVisible (myChild);
                  myChild = myChild->nextSibling();
              }
              listViewList->ensureItemVisible (pQListViewItem);
    }
}

//------------------------------------------------------- pushButtonImportMenuLocalize_clicked -------------------------------------
void C_DlgListFieldMngr::pushButtonImportMenuLocalize_clicked()
{m_PathMenuLib  = Slot_ImportMenuModulesLocalize(m_PathMenuLib +  "*;;;*");
}


//------------------------------ Slot_ImportMenuModulesLocalize ------------------------
QString C_DlgListFieldMngr::Slot_ImportMenuModulesLocalize(QString path_in)
{QString path = path_in;
    while ( (path.length()==0) || (!QFile::exists( path )) )
    {if (path.endsWith("*;;;*"))
              {path=path.remove("*;;;*");    //enlever le truc forçant l'entree dans la boucle (même si répertoire existe)
              }
              //................... on ne signale la non validite que si il n'y a pas  eu forçage .............................
              //                    (si il y a eu forçage c'est que forcement c'est non valide)
              else if ( QMessageBox::warning( this, tr("La Librairie des modules de menus est non trouvée"),
              	                              tr ( "L'emplacement (ou répertoire) de la librairie des modules de menus est non valide. Veuillez localiser le répertoire de ces librairies..."),
              	                              tr("&Localiser la librairie"), tr("&Annuler"), 0,
              	                              0, 1 ) == 1 ) return path_in.remove("*;;;*");;      // si echec on retourne celui d'entree propre
              path = QFileDialog::getExistingDirectory ( path, this, "OuvrirDlg", tr( " Rechercher le répertoire de votre librairie de menus" ) );
#if defined(Q_WS_WIN)
              if (CGenTools::DialogGetString(this, tr("Valider le chemin habituel de votre librairie de menus"), path)!= QDialog::Accepted)
                  return path_in.remove("*;;;*");       // si echec on retourne celui d'entree propre
#endif
              if (path.length()==0)                    return  path_in.remove("*;;;*");     // si echec on retourne celui d'entree propre
    }
    return path;
}


//------------------------------------------------------- pushButtonImportMenu_clicked -------------------------------------
void C_DlgListFieldMngr::pushButtonImportMenu_clicked()
{   m_pQListViewItem =listViewList->currentItem ();
    if (m_pQListViewItem==0) m_pQListViewItem =listViewList->selectedItem ();
    if (m_pQListViewItem==0) return;
    m_OldpQlistViewItem = m_pQListViewItem;
    if ( (m_PathMenuLib.length()==0) || (!QFile::exists( m_PathMenuLib )) )
    {         m_PathMenuLib  = m_PathDrTux.remove("drtux/bin/");
              m_PathMenuLib += "Librairie de menus";  // astuce du *;;;* pour forcer l'entree dans la boucle meme si repertoire valide
              m_PathMenuLib = Slot_ImportMenuModulesLocalize(m_PathMenuLib+"*;;;*");
              if( (m_PathMenuLib.length()==0) || (!QFile::exists( m_PathMenuLib )) ) return;
    }

    //........................... liste des modules .............
    FormDlgListOff_Listes *dlg = new FormDlgListOff_Listes(this,"RefList_Dial",TRUE);
    if (dlg==0) return;
    dlg->initDialog(m_PathMenuLib );
    dlg-> exec();
    QString itemName = dlg->m_ItemName;
    delete dlg;
    //.................. reperer les modules .........................................
    if (itemName  !="")
    {         QDir dir (m_PathMenuLib + "/" + itemName );;
              dir.setFilter( QDir::Files | QDir::NoSymLinks );
              //............ exploration de la liste ......................
              const QFileInfoList * pQFileInfoList = dir.entryInfoList();
              if (pQFileInfoList==0) return ;
              QFileInfoListIterator it( *pQFileInfoList );
              QFileInfo     *fi;
              QString menuName = "";
              while ( (fi = it.current()) != 0 )
              {QString fname = fi->fileName();//.latin1()
                  if      (fname == "..")
                  {
                  }
                  else if (fi->isDir() && fname != "." )
                  {
                  }
                  else if (fi->isFile() && fi->extension()=="html")
                  {menuName = fi->baseName();
                      break;
                  }
                  ++it;
              }
              if (menuName.length()==0)
              {   if (QFileInfo(itemName).extension(FALSE) != "pqt") ActionCreateNewRef(itemName);
                  CHtmlTools::Copy_Dir(m_PathMenuLib + "/" + itemName ,                          m_PathGlossaire + "Champs d'insertions"  , CHtmlTools::copyDir,"","_ex");   // copier les elements lies � ce menu
                  CHtmlTools::Copy_Dir(m_PathMenuLib + "/" + itemName ,                          m_PathGlossaire + "ImagesFolder"         , CHtmlTools::copyFiles, "png;jpg;gif;bnp"); // copier les eventuelles images
                  CHtmlTools::Copy_Dir(m_PathMenuLib + "/" + itemName + "/ToPutInGlossaire._ex", m_PathGlossaire                          , CHtmlTools::copyDir);
                  CHtmlTools::Copy_Dir(m_PathMenuLib + "/" + itemName + "/ToPutInMenus._ex",     m_PathDrTux+"Ressources/Menus"           , CHtmlTools::copyDir);
                  CHtmlTools::Copy_Dir(m_PathMenuLib + "/" + itemName + "/ToPutInMedinTux._ex",  QDir::cleanDirPath(m_PathDrTux+"../../") , CHtmlTools::copyDir);
                  if ( QFile::exists(  m_PathMenuLib + "/" + itemName + "/base_update.sql"))
                     { //................... mise à jour SQL ...............................................................
#ifndef QLIGHTPAD
                       QSqlQuery query(QString::null, G_pCApp->m_pCMoteurBase->m_DataBase);
                       qDebug(query.lastError().text());
                       QString sql_txt;
                       CGestIni::Param_UpdateFromDisk( m_PathMenuLib + "/" + itemName + "/base_update.sql", sql_txt );
                       QStringList queryList = QStringList::split(";",sql_txt);
                       for (int i=0; i< (int)queryList.size(); ++i)
                           { sql_txt = queryList[i];
                             query.exec(sql_txt);
                             qDebug(query.lastError().text());
                           }
#endif
                     }
              }
              else
              {if (MenuActionCreateNewSimple(menuName, m_PathMenuLib + "/" + itemName ))                                                   // creer le menu
                  {   CHtmlTools::Copy_Dir(m_PathMenuLib + "/" + itemName ,                          m_PathGlossaire + "Champs d'insertions" , CHtmlTools::copyDir,"","_ex");   // copier les elements lies � ce menu
                      CHtmlTools::Copy_Dir(m_PathMenuLib + "/" + itemName ,                          m_PathGlossaire + "ImagesFolder"        , CHtmlTools::copyFiles, "png;jpg;gif;bnp"); // copier les eventuelles images
                      CHtmlTools::Copy_Dir(m_PathMenuLib + "/" + itemName + "/ToPutInGlossaire._ex", m_PathGlossaire                         , CHtmlTools::copyDir);
                      CHtmlTools::Copy_Dir(m_PathMenuLib + "/" + itemName + "/ToPutInMenus._ex",     m_PathDrTux+"Ressources/Menus"          , CHtmlTools::copyDir);
                      CHtmlTools::Copy_Dir(m_PathMenuLib + "/" + itemName + "/ToPutInMedinTux._ex",  QDir::cleanDirPath(m_PathDrTux+"../../"), CHtmlTools::copyDir);
                      QString text = "";
                      CGestIni::Param_UpdateFromDisk(m_PathListes + "/" + GetPathItem (m_OldpQlistViewItem), text);
                      textEditField->setText(text);
                      textEditField->setEnabled (TRUE);
                      pushButtonSave->setDisabled(TRUE);
                  }
              }
    }
}
//------------------------------------------------------- initDialog -------------------------------------
void C_DlgListFieldMngr::initDialog(const QString &drTuxParam, const QString &pathGlossaire, const QString &pathDrTux )
{   QFileInfo qfi(qApp->argv()[0]);
    m_PathGlossaire   = pathGlossaire;
    m_DrTuxParam      = drTuxParam;
    m_PathListes      = pathGlossaire + tr(INSERTION_ROOT);
    m_PathDrTux       = pathDrTux;
    m_pRet            = 0;
    QString pathAppli =  CGestIni::Construct_PathBin_Module("drtux",  qfi.dirPath (true));
    // ............................... initialiser le chemin des librairies de modules menu ..................................................
    m_PathMenuLib  = CGestIni::Param_ReadUniqueParam( m_DrTuxParam, "MenuContextuel", "PathLib");
    if (m_PathMenuLib.length())
    {m_PathMenuLib = m_PathMenuLib.replace("$Glossaire", m_PathGlossaire);
              if (QDir(m_PathMenuLib).isRelative()) m_PathMenuLib.prepend(pathAppli);
              m_PathMenuLib = QDir::cleanDirPath(m_PathMenuLib);
    }

    if (m_PathMenuLib .length()==0 || (!QFile::exists( m_PathMenuLib )) )
    {m_PathMenuLib  = m_PathDrTux.remove("drtux/bin/");
              m_PathMenuLib += "Librairie de menus";
    }

    if ( !m_PathDrTux.endsWith ("/") )     m_PathDrTux     += "/";
    if ( !m_PathListes.endsWith ("/") )    m_PathListes    += "/";
    if ( !m_PathGlossaire.endsWith ("/") ) m_PathGlossaire += "/";
    m_PathGetFile = m_PathGlossaire;

    textEditField->setTextFormat( RichText );
    textEditField->mimeSourceFactory()->addFilePath(m_PathDrTux     + "Ressources/RubriquesTextes");
    textEditField->mimeSourceFactory()->addFilePath(m_PathGlossaire + "ImagesFolder");
    textEditField->setWordWrap ( QTextEdit::NoWrap );
    // Theme::getIcon( "ActesClassants_Icon.png")
    // Theme::getIconRightArrow();
    // Theme::getIconDownArrow()
    //............. positionner le bouton de menu d'outils...........
    pushButtonSetMasque->setIconSet ( Theme::getIconRightArrow());
    pushButtonSetTextField->setPixmap (Theme::getIcon( "22x22/CreateList_Item.png") );
    pushButtonSave->setIconSet (Theme::getIconSave());
    pushButton_InsertImage->setPixmap (Theme::getIcon( "22x22/ic-flower3.png"));
    pushButtonBold->setIconSet (Theme::getIconTextBold());
    pushButtonItalique->setIconSet (Theme::getIconTextItalic());
    pushButtonUnderline->setIconSet (Theme::getIconTextUnderline());
    pushButtonLeft->setIconSet  (Theme::getIconTextLeft());
    pushButtonCenter->setIconSet (Theme::getIconTextCenter());
    pushButtonRight->setIconSet (Theme::getIconTextRight());
    pushButtonImportMenu->setPixmap (Theme::getIcon( "22x22/ImportMenu.png") );
    pushButtonImportMenuLocalize->setPixmap (Theme::getIcon( "22x22/ImportMenuLocalize.png") );
    C_KeyPressEater *keyPressEater = new C_KeyPressEater( listViewList, this ,"myMachinEvent" );
    connect( keyPressEater,  SIGNAL( Sign_F2_Pressed() ), this, SLOT( On_lineEditFindItem_F2_Pressed() ) );
    connect( keyPressEater,  SIGNAL( Sign_F6_Pressed() ), this, SLOT( On_lineEditFindItem_F6_Pressed() ) );
    lineEditFindItem->installEventFilter( keyPressEater );
    listViewList->installEventFilter( keyPressEater );
    this->installEventFilter( keyPressEater );
    //................ charger la BlackList ........................................
    QString black_str      = CGestIni::Param_UpdateFromDisk(m_PathGlossaire+"/ignoreList.txt");
    black_str              = black_str.replace('\r',"").stripWhiteSpace();
    m_black_list           = QStringList::split('\n',black_str);

    //................ remplir la lisview ........................................
    fillListView(m_PathListes);

    //...................... initialiser les menus des rubriques ....................................
    init_listView_RubMenus();
    //.................... regler largeur du spliter ..............................................
    QValueList <int> list;
    int w = width() / 3;
    list.append(w);
    list.append(width() - w);
    splitterMedian->setSizes (list );
}
//----------------------------------------------------- init_listView_RubMenus -------------------------------------------------------------------
void C_DlgListFieldMngr::fillListView(const QString &path)
{    //............ exploration de la liste ......................
    long nb           = 0;
    QDir dir ( path  );
    dir.convertToAbs();
    dir.setFilter( QDir::All | QDir::NoSymLinks );
    listViewList->clear();

    const QFileInfoList * pQFileInfoList = dir.entryInfoList();
    if (pQFileInfoList==0) return ;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo     *fi;
    QListViewItem *element;
    while ( (fi = it.current()) != 0 )
    {         element = 0;
              QString fname = fi->fileName();//.latin1()
              if (CGestIni::isInBlackList(m_black_list, fname))     {++it;   continue;}
              if (fname == ".." || fname == ".")                    {++it;   continue;}
              if (fi->isDir())
              {   element = new QListViewItem( listViewList, fname, "D"    );
                  element->setPixmap ( 0, Theme::getIcon( "16x16/folder.png"));
                  ++nb ;
              }
              else if (fi->isFile())
              {   element    = new QListViewItem( listViewList, fname);
                  QString ext = QFileInfo(fname).extension().lower();
                  if ( ext.length() )
                  {   if (ext=="html"||ext=="htm"||ext=="txt"||ext=="xml"||ext=="rtf")
                      {   element    = new QListViewItem( listViewList, fname, "T"   );
                          element->setPixmap ( 0, Theme::getIcon( "22x22/file_text.png"));
                          ++nb ;
                      }
                  }
                  else
                  {   element    = new QListViewItem( listViewList, fname, "F"   );
                      element->setPixmap ( 0, Theme::getIcon( "22x22/file_simple.png") );
                      ++nb ;
                  }
              }
              ++it;
    }
}

//----------------------------------------------------- init_listView_RubMenus -------------------------------------------------------------------
void C_DlgListFieldMngr::listView_RubMenus_clicked(QListViewItem* pQListViewItem)
{if (pQListViewItem == 0) return;
 QString menu = pQListViewItem->text(1).stripWhiteSpace();
 if (menu.length()  == 0) return;
 lineEditFindItem->setText(menu);
 //lineEditFindItem_textChanged( const QString &qtxt );
}

//----------------------------------------------------- init_listView_RubMenus -------------------------------------------------------------------
void C_DlgListFieldMngr::init_listView_RubMenus()
{
#ifndef QLIGHTPAD
    QStringList  rubList = G_pCApp->get_PossiblesRubNameList();
    listView_RubMenus->clear();
    for (int i=0; i< (int)rubList.count(); ++i)
    {
        QListViewItem *pQTreeWidgetItem = new QListViewItem( listView_RubMenus );
        if (pQTreeWidgetItem)
        {   QString        rubName = rubList[i];
            int                pos = rubName.find('|');
            if (pos != -1) rubName = rubName.left(pos).stripWhiteSpace();
            QString           menu = CGestIni::Param_ReadUniqueParam( m_DrTuxParam, "MenuContextuel", rubName);
            listView_RubMenus->insertItem(pQTreeWidgetItem);
            pQTreeWidgetItem->setText(0, rubName);
            pQTreeWidgetItem->setText(1, menu);
            pQTreeWidgetItem->setPixmap (0, Theme::getPath()+ "16x16/type_"+rubName+".png" );
        }
    }
#endif
}

//------------------------------------------------------- accept -------------------------------------
void C_DlgListFieldMngr::accept()
{if (m_pRet) *m_pRet = listViewList->currentItem()->text(0);
    if (saveTextIfModified()) QDialog::accept();
}

//------------------------------------------------------- reject -------------------------------------
void C_DlgListFieldMngr::reject()
{if (m_pRet) *m_pRet =  "";
    if (saveTextIfModified()) QDialog::reject();
}

//------------------------------------------------------- setFontEdit -------------------------------------
void C_DlgListFieldMngr::setFontEdit(QFont &font)
{textEditField->setFont(font);
}

//------------------------------------------------------- doConnections -------------------------------------
void C_DlgListFieldMngr::doConnections( QTextEdit *e )
{   connect( e, SIGNAL( currentFontChanged( const QFont & ) ),
             this, SLOT( fontChanged( const QFont & ) ) );
    connect( e, SIGNAL( currentColorChanged( const QColor & ) ),
             this, SLOT( colorChanged( const QColor & ) ) );
    connect( e, SIGNAL( currentAlignmentChanged( int ) ),
             this, SLOT( alignmentChanged( int ) ) );
}
//------------------------------------------------------- fontChanged -------------------------------------
void C_DlgListFieldMngr::fontChanged( const QFont &f )
{   pushButtonUnderline->disconnect (SIGNAL(toggled(bool)));
    pushButtonBold->disconnect (SIGNAL(toggled(bool)));
    pushButtonItalique->disconnect (SIGNAL(toggled(bool)));

    comboBoxFont->lineEdit()->setText( f.family() );
    comboBoxSize->lineEdit()->setText( QString::number( f.pointSize() ) );
    if (pushButtonUnderline->state ()       == QButton::Off && f.underline())     pushButtonUnderline->toggle ();
    else  if (pushButtonUnderline->state () == QButton::On  && !f.underline())    pushButtonUnderline->toggle ();
    if (pushButtonBold->state ()            == QButton::Off && f.bold())          pushButtonBold->toggle ();
    else  if (pushButtonBold->state ()      == QButton::On  && !f.bold())         pushButtonBold->toggle ();
    if (pushButtonItalique->state ()        == QButton::Off && f.italic())        pushButtonItalique->toggle ();
    else  if (pushButtonItalique->state ()  == QButton::On  && !f.italic())       pushButtonItalique->toggle ();

    connect( pushButtonUnderline,  SIGNAL(toggled(bool)), this,  SLOT( pushButtonUnderline_toggled(bool)) );
    connect( pushButtonBold,       SIGNAL(toggled(bool)), this,  SLOT( pushButtonBold_toggled(bool)) );
    connect( pushButtonItalique,   SIGNAL(toggled(bool)), this,  SLOT( pushButtonItalique_toggled(bool)) );

}
//------------------------------------------------------- colorChanged -------------------------------------
void C_DlgListFieldMngr::colorChanged( const QColor &c )
{   pushButtonColor->setPaletteForegroundColor (c);
    pushButtonColor->setPaletteBackgroundColor (c);
}
//------------------------------------------------------- alignmentChanged -------------------------------------
void C_DlgListFieldMngr::alignmentChanged( int a )
{    if ( ( a == AlignAuto ) || ( a & AlignLeft ))
    buttonGroupJustif->setButton ( 0 );
    else if ( ( a & AlignHCenter ) )
        buttonGroupJustif->setButton ( 1 );
    else if ( ( a & AlignRight ) )
        buttonGroupJustif->setButton ( 2 );
}

//------------------------------------------------------- textUnderline -------------------------------------
void C_DlgListFieldMngr::pushButtonUnderline_toggled( bool state)
{textEditField->setUnderline( state  );
}
//------------------------------------------------------- textItalic -------------------------------------
void C_DlgListFieldMngr::pushButtonItalique_toggled( bool state)
{textEditField->setItalic( state );
}
//------------------------------------------------------- textBold -------------------------------------
void C_DlgListFieldMngr::pushButtonBold_toggled( bool state)
{textEditField->setBold( state  );
}

//----------------------------------------- textSize --------------------------------------------------------------
void C_DlgListFieldMngr::textSize( const QString &p )
{textEditField->setPointSize( p.toInt() );
    textEditField->viewport()->setFocus();
}
//------------------------------------------------------- textColor -------------------------------------
void C_DlgListFieldMngr::pushButtonColor_clicked()
{QColor col = QColorDialog::getColor( textEditField->color(), this );
    if ( !col.isValid() )       return;
    textEditField->setColor( col );
    //QPixmap pix( 16, 16 );
    //pix.fill( black );
    pushButtonColor->setPaletteForegroundColor (col);
    pushButtonColor->setPaletteBackgroundColor (col);
}
//------------------------------------ buttonGroupJustif_pressed --------------------------------------------------
void C_DlgListFieldMngr::buttonGroupJustif_pressed( int id)
{switch(id)
 {case 0:        // left
  textEditField->setAlignment( AlignLeft );
        break;
    case 1:        // center
        textEditField->setAlignment( AlignHCenter );
        break;
    case 2:        // right
        textEditField->setAlignment( AlignRight );
        break;
    }
}

//------------------------------------ pushButton_InsertImage_clicked --------------------------------------------------
void C_DlgListFieldMngr::pushButton_InsertImage_clicked()
{ //QString path_dst = G_pCApp->m_PathAppli + "Ressources" + QDir::separator() + "RubriquesTextes";
    //QString path_dst = m_PathDrTux  + "Ressources/RubriquesTextes";
    QString path_dst = m_PathGlossaire + "ImagesFolder";
    //................. choisir le fichier image src à insérer .................................................
    QFileDialog *fd = new QFileDialog  ( path_dst,
        		                 tr( "Fichiers images") + QString("*.png *.png *.jpg *.gif *.bmp ;;") + tr("Tous les fichiers (*)" ),
        		                 this,
        		                 "OuvrirDlg"
        		                 ) ;
    if (fd==0)                                            return;
    CPreview* p = new CPreview;
    //fd->setMode(QFileDialog::ExistingFiles);   //preview mode ne fonctionne pas avec ce mode !!
    fd->setInfoPreviewEnabled( TRUE );
    fd->setInfoPreview( p, p );
    fd->setPreviewMode( QFileDialog::Info );
    if ( ! (fd->exec() == QDialog::Accepted) )
    {delete fd;
        return;
    }
    QString filename = fd->selectedFile();
    delete fd;
    if ( !filename.isEmpty() && QFile::exists( filename ))
    {filename = CHtmlTools::Copy_File(filename, path_dst);
        CHtmlTools::insertHtml(textEditField, QString("<img src=\"./") + filename + "\">",0);  // important de laisser l'espace du d�ut
    }

}

//----------------------------------------- AddSubFolder --------------------------------------------------------------
void C_DlgListFieldMngr::AddSubFolder (QListViewItem *qlistViewItem,  QString list_name)
{   if (list_name == "") return;
    QDir dir (m_PathListes + QDir::separator() + list_name);
    dir.convertToAbs();
    dir.setFilter( QDir::All | QDir::NoSymLinks );

    //............ exploration de la liste ......................
    const QFileInfoList * pQFileInfoList = dir.entryInfoList();
    if (pQFileInfoList==0) return;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo     *fi;
    QListViewItem *element;
    while ( (fi = it.current()) != 0 )
    {   element = 0;
        QString fname = fi->fileName();//.latin1()
        if (CGestIni::isInBlackList(m_black_list, fname))     {++it;   continue;}
        if (fname == ".." || fname == ".")                    {++it;   continue;}
        if (fi->isDir())
           { element = new QListViewItem( qlistViewItem, fname, "D");
             element->setPixmap ( 0, Theme::getIcon( "16x16/folder.png")   );
           }
        else if (fi->isFile())
           {
             QString ext = QFileInfo(fname).extension().lower();
             if (ext.length())
                { if (ext=="html"||ext=="htm"||ext=="txt"||ext=="xml"||ext=="rtf")
                     { element    = new QListViewItem( qlistViewItem, fname, "T");
                       element->setPixmap ( 0, Theme::getIcon( "22x22/file_text.png")   );
                     }
                }
             else
                { element    = new QListViewItem( qlistViewItem, fname, "F");
                  element->setPixmap ( 0, Theme::getIcon( "22x22/file_simple.png")  );
                }
           }
        ++it;
    }
}


//------------------------------------ MenuActionModifier --------------------------------------------------
void C_DlgListFieldMngr::MenuActionModifier()
{if (m_pQListViewItem==0) return;
    listViewList_doubleClicked(listViewList->currentItem());
    textEditField->setFocus();
}

//----------------------------------------- listViewList_doubleClicked -----------------------------------------------------------------
void C_DlgListFieldMngr::listViewList_doubleClicked( QListViewItem *qlistViewItem)
{if (qlistViewItem==0) return;
    QString qstr;
    if (qlistViewItem->text(0)=="") return;
    saveTextIfModified();
    //.......................On est deja sur un item enfant: depth()>0 ................................................
    //                   alors si c'est un dossier le selectionner dans la liste
    if (qlistViewItem->depth()>0)
    {if (qlistViewItem->text(1)=="D")
        {   QListViewItem *pQListViewItem = FindDepthItem(qlistViewItem->text(0),0);
            if (pQListViewItem)
            {listViewList->setCurrentItem(pQListViewItem);
                listViewList->ensureItemVisible (pQListViewItem);
                if (pQListViewItem->childCount ()<=0)
                {AddSubFolder (pQListViewItem,  pQListViewItem->text(0));
        	}
                if (pQListViewItem->isOpen()) pQListViewItem->setOpen (FALSE);
                else                          pQListViewItem->setOpen (TRUE);
                textEditField->setText("");
                textEditField->setEnabled (FALSE );
                m_OldpQlistViewItem=0;
            }
        }
        else if (qlistViewItem->text(1)=="T")
        {
            QString  path = m_PathListes + QDir::separator() + GetPathItem (qlistViewItem);
            QFile file (path );
            if (file.open( IO_ReadOnly )==FALSE){    return;}
            QString text = "";
            CGestIni::Param_UpdateFromDisk(path, text);
            textEditField->setText(text);
            textEditField->setEnabled (TRUE);
            m_OldpQlistViewItem=qlistViewItem;
            //delete[]text;
        }
        else if (qlistViewItem->text(1)=="F")
        {   textEditField->setText("");
            textEditField->setEnabled (TRUE);
            m_OldpQlistViewItem=qlistViewItem;
        }
    }
    else
    {if (qlistViewItem->childCount ()<=0)
        {AddSubFolder (qlistViewItem,  qlistViewItem->text(0));
        }
        if (qlistViewItem->isOpen()) qlistViewItem->setOpen (FALSE);
        else                         qlistViewItem->setOpen (TRUE);
        textEditField->setText("");
        textEditField->setEnabled (FALSE );
        m_OldpQlistViewItem=0;
    }
    pushButtonSave->setDisabled(TRUE);
}

//---------------------------- GetPathItem -----------------------------------------------------------------------
// ACTION: part de l'item: QListViewItem *qlistViewItem   et remonte de parents en parents jusqu'à la racine
//         tout en constituant le chemin ainsi formé par les nom de ces items
// SORTIE: QString    contenant le chemin de l'item: QListViewItem *qlistViewItem

QString C_DlgListFieldMngr::GetPathItem (QListViewItem *qlistViewItem)
{QString path  = qlistViewItem->text(0);
    qlistViewItem = qlistViewItem->parent();
    while (qlistViewItem!=0)
    {   path.insert(0, QDir::separator());
        path.insert(0,qlistViewItem->text(0));
        if (qlistViewItem->depth()==0) break;
        qlistViewItem = qlistViewItem->parent();
    }
    return path;
}

//----------------------------------------- FindFirstSteepItem -------------------------------------------------------------------------
QListViewItem *C_DlgListFieldMngr::FindDepthItem(QString name, int depth)
{QListViewItemIterator it( listViewList);
    while ( it.current() )
    {QListViewItem *item = it.current();
        ++it;
        if ( item->depth()==depth &&  item->text(0)==name) return item;
    }
    return 0;
}

//----------------------------------------- textEditField_textChanged -----------------------------------------------------------------
void C_DlgListFieldMngr::textEditField_textChanged()
{/*
 QListViewItem *qlistViewItem = listViewList->currentItem ();
 if (qlistViewItem ==0 )                           goto textEditField_textChangedInactiv;   // rien de selectionné
 if (qlistViewItem->text(1) =="D")                 goto textEditField_textChangedInactiv;   // dossier
 if (qlistViewItem->text(0).findRev(".") == -1)    goto textEditField_textChangedActiv;     // pas d'extension
 if (qlistViewItem->text(0).findRev(".txt")==-1)   goto textEditField_textChangedInactiv;   // extension non text

textEditField_textChangedActiv:
 if (isEnabled()==FALSE) textEditField->setEnabled (TRUE);
*/
    pushButtonSave->setEnabled(TRUE);
    /*
 return;

textEditField_textChangedInactiv:
   textEditField->setText("");
   textEditField->setEnabled (FALSE);
   pushButtonSave->setShown(FALSE);
*/
}

//------------------------------------ On_lineEditFindItem_F2_Pressed --------------------------------------------------
void C_DlgListFieldMngr::On_lineEditFindItem_F2_Pressed()
{QListViewItem *pQListViewItem  = listViewList->currentItem ();
    if (pQListViewItem == 0) return;
    listViewList_contextMenuRequested(pQListViewItem , QCursor::pos() , 0);
}
//------------------------------------ On_lineEditFindItem_F6_Pressed --------------------------------------------------
void C_DlgListFieldMngr::On_lineEditFindItem_F6_Pressed()
{//QListViewItem *pQListViewItem  = listViewList->currentItem ();
    //if (pQListViewItem == 0) return;
    MenuActionModifier();
    //listViewList_contextMenuRequested(pQListViewItem , QCursor::pos() , 0);
}
//------------------------------------ listViewList_contextMenuRequested --------------------------------------------------
void C_DlgListFieldMngr::listViewList_contextMenuRequested( QListViewItem *qlistViewItem, const QPoint &pos, int v)
{listViewList_rightButtonClicked( qlistViewItem, pos, v );
}

//------------------------------------ listViewList_rightButtonClicked --------------------------------------------------
void C_DlgListFieldMngr::listViewList_rightButtonClicked( QListViewItem *qlistViewItem, const QPoint &pos, int )
{if (qlistViewItem==0) return;
    //listViewList->disconnect( SIGNAL( clicked(QListViewItem* , const QPoint &, int)) );

    ThemePopup *pQPopupMenu = new ThemePopup(this, "MyPopupMenu" );

    //pThemePopup->insertItem( QPixmap( undo ),          ("&Undo"),     this, SLOT(undo()),    CTRL+Key_Z );
    //pThemePopup->insertItem( QPixmap( redo ),          ("&Redo"),     this, SLOT( redo()),   CTRL+Key_Y );
    //pThemePopup->insertItem( QPixmap( editcut ),       ("&Cut"),      this, SLOT( cut()),    CTRL+Key_X );
    //pThemePopup->insertItem( QPixmap( editcopy ),      ("C&opy"),     this, SLOT( copy()),   CTRL+Key_C );
    //pThemePopup->insertItem( QPixmap( editpaste ),     ("&Paste"),    this, SLOT( paste()),  CTRL+Key_C );
    m_pQListViewItem = qlistViewItem;
    if (qlistViewItem->depth()==0 && qlistViewItem->childCount ()<=0)
    {AddSubFolder (qlistViewItem,  qlistViewItem->text(0));
        qlistViewItem->setOpen (TRUE);
    }

    pQPopupMenu->insertItem( tr("Créer et &ajouter un nouvel élément à une liste"),        this, SLOT( MenuActionNewSimple()),  CTRL+Key_N );
    pQPopupMenu->insertSeparator ();
    pQPopupMenu->insertItem( tr("Créer et ajouter une &référence à une autre liste"),      this, SLOT( MenuActionNewRef()),     CTRL+Key_F );
    pQPopupMenu->insertItem( tr("&Choisir une liste déjà existante, comme référence"),     this, SLOT( MenuActionListOffRef()), CTRL+Key_H );
    pQPopupMenu->insertItem( tr("Créer une nouvelle &liste déroulante"),                   this, SLOT( MenuActionNewListe()),   CTRL+Key_L );
    pQPopupMenu->insertSeparator ();
    pQPopupMenu->insertItem( tr("&Renommer cet élément ou cette liste"),                   this, SLOT( MenuActionRename()),     CTRL+Key_R );
    pQPopupMenu->insertItem( tr("&Effacer cet élément ou cette liste"),                    this, SLOT( MenuActionDelete()),     CTRL+Key_D );
    pQPopupMenu->insertItem( tr("&Modifier le contenu de cet élément"),                    this, SLOT( MenuActionModifier()),   CTRL+Key_M );
    pQPopupMenu->insertSeparator ();
    pQPopupMenu->insertItem( tr("&Vérifier si cette liste est référencée par d'autres"),   this, SLOT( MenuActionIsExist()),    CTRL+Key_E );
    pQPopupMenu->insertItem( tr("&Importer un menu de la librairie de menus"),             this, SLOT( pushButtonImportMenu_clicked()),    CTRL+Key_I );
    if (qlistViewItem->depth()>0 && qlistViewItem->text(1)=="D")
    {   pQPopupMenu->insertItem( tr("&Se positionner sur cette liste"),                    this, SLOT( MenuActionGotoListe()),  CTRL+Key_P );
    }
#ifndef QLIGHTPAD
    //................ sous menu des rubriques ...............................
    if (listView_RubMenus->childCount() && qlistViewItem->text(1)=="D")
    {pQPopupMenu->insertSeparator ();
     QStringList  rubList           = G_pCApp->get_PossiblesRubNameList();
     QStringList  optionsMenuList;
     for (int i=0; i< (int) rubList.count(); ++i)
         {  QString        rubName = rubList[i];
            int                pos = rubName.find('|');
            if (pos!=-1)   rubName = rubName.left(pos);
            optionsMenuList.append(rubName);
         }
     if (optionsMenuList.count())
     {  /*  \note  avant appel de cette fonction il convient de la connecter comme suit a
         *                                un SLOT a actionner lors selection d'une option:
         *                     connect ( G_pCApp, SIGNAL(Sign_popup_HierarchOptionSelected()) , this, SLOT(Slot_OnMenuActionSelected()));
         *         puis dans le slot de la deconnecter :
         *                  disconnect ( G_pCApp, SIGNAL(Sign_popup_HierarchOptionSelected()) , this, SLOT(Slot_OnMenuActionSelected()));
        */
         m_subOptMenu = "";
         G_pCApp->addPopupHierarchique(optionsMenuList, pQPopupMenu,  &m_subOptMenu, tr("Placer ce menu '%1' comme menu fixe d'une rubrique").arg(qlistViewItem->text(0)));
         connect ( G_pCApp, SIGNAL(Sign_popup_HierarchOptionSelected()) , this, SLOT(Slot_OnMenuActionSelected()));
     }
    }
#endif
    pQPopupMenu->exec(pos);
    delete pQPopupMenu;
    m_pQListViewItem = 0;
}

//------------------------------------ Slot_OnMenuActionSelected --------------------------------------------------
void C_DlgListFieldMngr::Slot_OnMenuActionSelected()
{

#ifndef QLIGHTPAD
  disconnect ( G_pCApp, SIGNAL(Sign_popup_HierarchOptionSelected()) , this, SLOT(Slot_OnMenuActionSelected()));
  if (m_subOptMenu.length()==0) return;
  if (m_pQListViewItem==0)      return;
  QString           rubName = m_subOptMenu;
  int                   pos = rubName.find('(');
  if (pos!=-1)      rubName = rubName.mid(pos+1).remove(')');
  CGestIni::Param_WriteParam(&m_DrTuxParam, "MenuContextuel", rubName , m_pQListViewItem->text(0));
  G_pCApp->m_DrTuxParam     = m_DrTuxParam;            // mettre à jour le global a l'application
  CGestIni::Param_UpdateToDisk(G_pCApp->m_PathDrTuxIni, G_pCApp->m_DrTuxParam);
  G_pCApp->m_MapMenuRubriques[rubName] = m_pQListViewItem->text(0);
  init_listView_RubMenus();
#endif
}

//------------------------------------ MenuActionGotoListe --------------------------------------------------
void C_DlgListFieldMngr::MenuActionGotoListe()
{if (m_pQListViewItem==0) return;
    listViewList_doubleClicked(m_pQListViewItem);
}

//------------------------------- Slot_RestaureConnection --------------------------------------------------------------------------
void C_DlgListFieldMngr::Slot_RestaureConnection()
{/*
 connect( ListView_Glossaire,         SIGNAL( selectionChanged(QListViewItem *) ),
          this,                       SLOT(ListView_Glossaire_selectionChanged (QListViewItem *))
        );
 connect( ListView_Glossaire,         SIGNAL( doubleClicked(QListViewItem *) ),
          this,                       SLOT(ListView_Glossaire_doubleClicked (QListViewItem *))
        );
 connect( ListView_Glossaire,         SIGNAL( clicked(QListViewItem *) ),
          this,                       SLOT(ListView_Glossaire_clicked (QListViewItem *))
        );

    connect( listViewList,               SIGNAL( clicked(QListViewItem* , const QPoint &, int) ),
             this,                       SLOT(listViewList_clicked (QListViewItem* , const QPoint &, int))
             );
*/
}

//------------------------------------ CreateSousPopupRef --------------------------------------------------
void C_DlgListFieldMngr::MenuActionListOffRef()
{
    FormDlgListOff_Listes *dlg = new FormDlgListOff_Listes(this,"RefList_Dial",TRUE);
    if (dlg==0) return;
    dlg->initDialog(m_PathGlossaire+"/Champs d'insertions");
    dlg-> exec();
    if (dlg->m_ItemName !="")
    {ActionNewRef(dlg->m_ItemName);
    }
    delete dlg;
}


//------------------------------------ MenuActionNewListe --------------------------------------------------
void C_DlgListFieldMngr::MenuActionNewListe()
{if (m_pQListViewItem==0) return;

    //............. si on est à l'interieur d'une liste ...........................................
    //              il faut y creer une nouvelle reference
    if (m_pQListViewItem->depth()==1)
    {MenuActionNewRef();
        return;
    }
    //............. si on n'est pas sur un element d'une liste ....................................
    //             il faut creer simplement une nouvelle liste à la racine
    QString  qsName = tr("Nouvelle liste");
    if (CGenTools::DialogGetString(this,  tr("Veuillez donner le nom de cette nouvelle liste"), qsName) ==  QDialog::Accepted)
    { qsName = qsName.stripWhiteSpace();
        if (qsName=="")
        {CGenTools::MessageAlert(this, tr("ATTENTION ! "),tr("Le nom du dossier est vide.<br>Veuillez choisir un autre nom"), tr("Annu&ler"));
            return;
        }
        if (qsName.find(":") != -1 || qsName.find("\\") != -1 || qsName.find("/") != -1)
        {CGenTools::MessageAlert(this, tr("ATTENTION ! "),tr("Les noms de listes ne doivent pas contenir les caractères ':' '/' '\\' .<br>Veuillez choisir un autre nom."), tr("Annu&ler"));
        }
        //........................ a la racine des listes ................................................
        //                         il faut juste créer le repertoire de la liste
        if (IsThisFolderExist(m_PathListes, qsName))
        {CGenTools::MessageAlert(this, tr("ATTENTION ! "),tr("Ce dossier existe déjà.<br>Veuillez choisir un autre nom."), tr("Annu&ler"));
        }
        else
        {   if  (!IsThisFileNameCompatible(qsName)) return;
            QDir dir;
            if (dir.mkdir (m_PathListes + QDir::separator() + qsName)==FALSE)
            {QString mess  = tr("Ce dossier<br><b>");
                mess += m_PathListes + QDir::separator() + qsName;
                mess += QString("</b><br>") + tr(" n'a pu être créé.");
                CGenTools::MessageAlert(this, tr("ERREUR :"),mess, tr("Annu&ler"));
            }
            else
            {saveTextIfModified();
                m_OldpQlistViewItem = 0;
                QListViewItem *element = new QListViewItem( listViewList, qsName, "D");
                if (element)
                {   element->setPixmap ( 0, Theme::getIcon( "16x16/folder.png")   );
                    listViewList->ensureItemVisible (element );
                    listViewList->setCurrentItem (element );
                }
            }
        }
    }
}


//------------------------------------ MenuActionNewRef --------------------------------------------------
void C_DlgListFieldMngr::MenuActionNewRef()
{  QString qsname(tr("Nouvelle référence"));
    ActionNewRef(qsname);
}

//------------------------------------ MenuActionNewRef --------------------------------------------------
void C_DlgListFieldMngr::ActionNewRef(QString  &qsName)
{
    if (m_pQListViewItem==0) return;
    if (CGenTools::DialogGetString(this, tr("Veuillez donner le nom de cette nouvelle référence"), qsName) ==  QDialog::Accepted)
    { qsName = qsName.stripWhiteSpace();
        ActionCreateNewRef(qsName);
    }
}

//------------------------------------ ActionCreateNewRef --------------------------------------------------
void C_DlgListFieldMngr::ActionCreateNewRef(QString  &qsName)
{   QListViewItem *qlistViewItem;
    QListViewItem *element;

    if (qsName=="")
    {CGenTools::MessageAlert(this, tr("ATTENTION ! "),tr("Le nom de la référence est vide.<br>Veuillez choisir un autre nom"), tr("Annu&ler"));
        return;
    }
    QString path =  m_PathListes;
    //........................ Si on est DANS une liste ................................................
    //                         il faut créer le repertoire référence dans la liste et celui de la racine
    if (m_pQListViewItem->depth()==1)
    {qlistViewItem = m_pQListViewItem->parent();
        if (qlistViewItem==0)           return;
        if (qlistViewItem->depth() !=0) return;
    }
    else
    {qlistViewItem = m_pQListViewItem;
    }
    path += QDir::separator() + qlistViewItem->text(0);      // le repertoire de travail et creation est DANS la liste
    //........... créer la reference dans la liste ..............................................
    if (IsThisFolderExist(path, qsName))
    {CGenTools::MessageAlert(this, tr("ATTENTION ! "),tr("Une référence à une cette liste existe déjà<br>"
                        		                 "dans cette liste, elle ne sera donc pas créée."), "&Valider");
    }
    else
    {   if  (!IsThisFileNameCompatible(qsName)) return;
        QDir dir;
        if (dir.mkdir (path + QDir::separator() + qsName)==FALSE)
        {   QString mess  = tr("La référence à la liste :<br><b>");
            mess += path + QDir::separator() + qsName;
            mess += tr("</b><br> n'a pu être créée.");
            CGenTools::MessageAlert(this, tr("ERREUR :"),mess, tr("Annu&ler"));
            return;
        }
        //.............. mettre à jour le contenu des listes (que si creation)......................................
        element = new QListViewItem( qlistViewItem, qsName, "D");    // rajouter la référence à la liste à la liste en cours
        if (element)
        {   element->setPixmap ( 0,  Theme::getIcon( "16x16/folder.png")    );
            qlistViewItem->setOpen (TRUE);
            listViewList->ensureItemVisible (element );
            listViewList->setCurrentItem (element );
        }
    }
    //........... créer si besoin la liste à laquelle se réfère la réference....................
    if (IsThisFolderExist(m_PathListes, qsName)==FALSE)     // que si n'existe pas déjà
    {
        if  (!IsThisFileNameCompatible(qsName)) return;
        QDir dir;
        if (dir.mkdir (m_PathListes + QDir::separator() + qsName)==FALSE)
        {QString mess  = tr("Cette liste<br><b>");
            mess += m_PathListes + QDir::separator() + qsName;
            mess += tr("</b><br> n'a pu être créée");
            CGenTools::MessageAlert(this, tr("ERREUR :"),mess, tr("Annu&ler"));
            return;
        }
        //.............. mettre à jour le contenu des listes (que si creation)......................................
        saveTextIfModified();
        element = new QListViewItem( listViewList, qsName, "D"); // rajouter la liste crée à la racine de la listbox
        if (element)
        {m_OldpQlistViewItem = 0;
            element->setPixmap ( 0,  Theme::getIcon( "16x16/folder.png")    );
        }
    }
}

//------------------------------------ IsThisFileNameCompatible --------------------------------------------------
bool C_DlgListFieldMngr::IsThisFileNameCompatible(const QString &qsName)
{if ( qsName.find(":") != -1 )
    {CGenTools::MessageAlert(this, tr("ATTENTION ! "),tr("Les noms de listes ne doivent pas contenir le caractère ':'  .<br>Veuillez choisir un autre nom."), tr("Annu&ler"));
        return FALSE;
    }
    return TRUE;
}

//------------------------------------ MenuActionNewSimple --------------------------------------------------
void C_DlgListFieldMngr::MenuActionNewSimple()
{QString  qsName = tr("élément nouveau ");
    if (m_pQListViewItem==0) return;
    if (CGenTools::DialogGetString(this, tr("Veuillez donner le nom de ce nouvel élément"), qsName) ==  QDialog::Accepted)
    {MenuActionCreateNewSimple(qsName, "");
    }
}

//------------------------------------ MenuActionCreateNewSimple--------------------------------------------------
int C_DlgListFieldMngr::MenuActionCreateNewSimple(QString  qsName, const QString& dirSrc)
{       int           ret = 0;
    QString       ext = "";
    if (m_pQListViewItem==0) return ret;
    saveTextIfModified();
    QListViewItem *qlistViewItem;
    QString  path;
    QString  itemType = "F";
    if (dirSrc.length())
    {ext      = ".html";
        itemType = "T";
    }
    qsName            = qsName.stripWhiteSpace();
    if (qsName=="")
    {CGenTools::MessageAlert(this, tr("ATTENTION ! "),tr("Le nom du fichier est vide.<br>Veuillez choisir un nom."), tr("Annu&ler"));
        return 0;
    }
    if (m_pQListViewItem->depth()==0)        // SI NOM DE LA LISTE est selectionné OK c'est bon
    {   qlistViewItem = m_pQListViewItem;
    }
    else if (m_pQListViewItem->depth()==1)   // SI ITEM DE LISTE REMONTER AU PERE pour avoir nom de la liste
    {   qlistViewItem = m_pQListViewItem->parent();
        if (qlistViewItem==0)           return ret ;
        if (qlistViewItem->depth() !=0) return ret ;
    }
    else
    {return ret ;
    }
    path  = m_PathListes + QDir::separator() + qlistViewItem->text(0);
    path += QDir::separator() + qsName + ext;
    if (IsThisFileExist(path))
    {   CGenTools::MessageAlert(this, tr("ATTENTION ! "),tr("Ce fichier existe déjà.<br>Veuillez choisir un autre nom"), tr("Annu&ler"));
        return ret;
    }
    if  (!IsThisFileNameCompatible(qsName)) return ret ;
    if (dirSrc.length())
    {CHtmlTools::Copy_File(dirSrc + QDir::separator() + qsName+".html", m_PathListes + QDir::separator() + qlistViewItem->text(0));

    }
    else
    { QFile qfile(path);
        if (qfile.open(IO_ReadWrite)==FALSE)
        {   qfile.close();
        }
    }
    if ( ! QFile ::exists (path ) )
    {
        QString mess  = tr("Le fichier<br><b>");
        mess += path;
        mess += tr("</b><br> n'a pu être créé");
        CGenTools::MessageAlert(this, tr("ERREUR :"),mess, tr("Annu&ler"));
    }
    else
    {
        QListViewItem *element = new QListViewItem( qlistViewItem, qsName + ext, itemType );
        if (element)
        {   if (itemType == "F") element->setPixmap ( 0,   Theme::getIcon( "22x22/file_simple.png")     );
            else                 element->setPixmap ( 0,   Theme::getIcon( "22x22/file_text.png")       );
            qlistViewItem->setOpen (TRUE);
            listViewList->ensureItemVisible (element );
            listViewList->setCurrentItem (element );
            m_OldpQlistViewItem = element;
            ret = 1;
        }
    }
    return ret;
}

//------------------------------------ DeleteListContent -------------------------------------------------
int C_DlgListFieldMngr::DeleteListContent(QString path, QString  name_ref)
{   int nb    = 0;
    int ok;
    path = QDir::cleanDirPath (path );
    QDir dir (path);
    dir.convertToAbs();
    dir.setFilter( QDir::All | QDir::NoSymLinks );

    //............ exploration de la liste ......................
    const QFileInfoList * pQFileInfoList = dir.entryInfoList();
    if (pQFileInfoList==0) return 0;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo     *fi;
    while ( (fi = it.current()) != 0 )
    {   QString fname = fi->fileName();//.latin1()
        if (CGestIni::isInBlackList(m_black_list, fname))     {++it;   continue;}
        if (fname == ".." || fname == ".")                    {++it;   continue;}
        if (fi->isDir())
        {   ok = 1;
            if (name_ref && strcmp(name_ref, fname)!=0) ok = 0;
            if (ok) {dir.rmdir (QString(path) + QDir::separator() + fname); ++nb ;}
        }
        else if (fi->isFile())
        {   ok = 1;
            if (name_ref && strcmp(name_ref,fname)!=0) ok = 0;
            if (ok) {dir.remove (QString(path) + QDir::separator() + fname); ++nb;}
        }
        ++it;
    }
    return nb;
}

//------------------------------------ MenuActionDelete --------------------------------------------------
void C_DlgListFieldMngr::MenuActionDelete()
{   QListViewItem *qlistViewItem;
    if (m_pQListViewItem==0) return;

    QString  qsMessage = tr("Confirmez-vous l'effacement ");
    int      depth = m_pQListViewItem->depth();
    QString   text = m_pQListViewItem->text(0);
    if (depth==0)
    {   qsMessage += tr("d'une liste entière :<br /><b>");
        qsMessage += m_pQListViewItem->text(0);
        qsMessage += tr("</b><br />  ATTENTION CETTE ACTION EST IRRÉVERSIBLE <br />");
        if (CGenTools::MessageAlert(this, tr("Effacement d'une liste"),qsMessage, tr("&Effacer"), tr("Annu&ler"))==100)
        {   DeleteListContent(m_PathListes + QDir::separator() + m_pQListViewItem->text(0), 0);
            QDir dir;
            if (dir.rmdir (m_PathListes + QDir::separator() + m_pQListViewItem->text(0))==FALSE)
            {   QString mess  = tr("Ce dossier<br><b>");
                mess += QString(INSERTION_ROOT) + QDir::separator() + m_pQListViewItem->text(0);
                mess += tr("</b><br> n'a pu être effacé.");
                CGenTools::MessageAlert(this, tr("ERREUR :"),mess, tr("Annu&ler"));
            }
            else
            {   delete m_pQListViewItem;
                m_pQListViewItem = 0;
                m_OldpQlistViewItem = 0;
            }
        }
    }
    else
    {   if (m_pQListViewItem->text(1)=="D")    qsMessage += tr("de cette référence à une autre liste :<br /><b>");
        if (m_pQListViewItem->text(1)=="T")    qsMessage += tr("de cet élément avec son texte attaché :<br /><b>");
        if (m_pQListViewItem->text(1)=="F")    qsMessage += tr("de cet élément :<br /><b>");
        qsMessage += m_pQListViewItem->text(0);
        qsMessage += tr("</b><br />  ATTENTION CETTE ACTION EST IRRÉVERSIBLE <br />");
        if ( CGenTools::MessageAlert(this, tr("Effacement"),qsMessage, tr("&Effacer"), tr("Annu&ler"))==100)
        {   qlistViewItem = m_pQListViewItem->parent();
            if (qlistViewItem==0)           return;
            if (qlistViewItem->depth() !=0) return;
            if (DeleteListContent(m_PathListes + QDir::separator() + qlistViewItem->text(0), // nom de la liste (folder)
        	                  m_pQListViewItem->text(0)) <=0                                   // nom de l'item à effacer dans cette liste
                )
            {   QString mess  = tr("Cet élément<br><b>");
                mess += m_PathListes + QDir::separator() +qlistViewItem->text(0) + QDir::separator() + m_pQListViewItem->text(0);
                mess += tr("</b><br> n'a pu être effacé.");
                CGenTools::MessageAlert(this, tr("ERREUR :"),mess, tr("Annu&ler"));
            }
            else
            {   delete m_pQListViewItem;
                m_pQListViewItem = 0;
                m_OldpQlistViewItem = 0;
            }
        }
    }
}

//------------------------------------ MenuActionRename --------------------------------------------------
void C_DlgListFieldMngr::MenuActionRename()
{if (m_pQListViewItem==0) return;

    int pos;
    QString  old_path = m_PathListes + QDir::separator() + GetPathItem (m_pQListViewItem);
    QString  qsName   = m_pQListViewItem->text(0);
    QString  new_name = "";
    if (CGenTools::DialogGetString(this, tr("Veuillez donner le nouveau nom de cet élément."),qsName) ==  QDialog::Accepted)
    {   qsName = qsName.stripWhiteSpace();
        if (qsName=="")
        {   CGenTools::MessageAlert(this, tr("ATTENTION !"),tr("Le nouveau nom est vide.<br>Opération annulée."), tr("Annu&ler"));
            return;
        }
        if (m_pQListViewItem->text(1)=="T")
        {if (qsName.findRev(".html")==-1) qsName += ".html";
        }
        //CMoteurBase::GotoDebug();
        pos = old_path.findRev(QDir::separator ());
        if (pos != -1) new_name = old_path.left(pos) + QDir::separator () + qsName;
        else           new_name = qsName;
        if (IsThisFileExist(new_name))
        {CGenTools::MessageAlert(this, tr("ATTENTION !"),tr("Un fichier portant ce nom existe déjà.<br>Opération annulée"), tr("Annu&ler"));
            return;
        }
        if  (!IsThisFileNameCompatible(qsName)) return;          // ne tester que  qsName et non le chemin complet qui peut contenir C:
        QDir dir;
        if (dir.rename ( old_path, new_name)==FALSE)
        {   QString mess  = tr("Cet élément<br>");
            mess += new_name ;
            mess += tr("<br> n'a pu être renommé.");
            CGenTools::MessageAlert(this, tr("ERREUR :"),mess, tr("Annu&ler"));
            return;
        }
        m_pQListViewItem->setText(0, qsName);
    }
}

//------------------------------------ MenuActionIsExist --------------------------------------------------
void C_DlgListFieldMngr::MenuActionIsExist()
{   if (m_pQListViewItem==0) return;

    if (m_pQListViewItem->text(1) != "D")
    {CGenTools::MessageAlert(this, tr("ATTENTION !"),tr("Cet élément n'est pas une liste.<br>Opération annulée."), tr("Annu&ler"));
        return;
    }
    QString listFolder = "";
    long nb            = 0;
    QDir dir (m_PathListes );
    dir.convertToAbs();
    dir.setFilter( QDir::All | QDir::NoSymLinks );
    //............ exploration de la liste ......................
    const QFileInfoList * pQFileInfoList = dir.entryInfoList();
    if (pQFileInfoList==0) return ;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo     *fi;
    while ( (fi = it.current()) != 0 )
    {   QString fname = fi->fileName();//.latin1()
        if (CGestIni::isInBlackList(m_black_list, fname))     {++it;   continue;}
        if (fname == ".." || fname == ".")                    {++it;   continue;}
        if (fi->isDir())
           { if (IsThisFolderExist(m_PathListes + QDir::separator () + fname, m_pQListViewItem->text(0) ))
                { listFolder += "&nbsp;&nbsp;&nbsp;&nbsp;\"" + fname + "\"<br>";
                  ++nb ;
                }
           }
        else if (fi->isFile())
           {
           }
        ++it;
    }
    if (nb==0) CGenTools::MessageAlert(this, QString(tr("UTILISATION DE LA LISTE :")) + m_pQListViewItem->text(0),
        	                       QString(tr("Cette liste : \"")) + m_pQListViewItem->text(0) + tr("\" n'est référencée par aucune autre."),
        	                       "OK");
    else       CGenTools::MessageAlert(this, QString(tr("UTILISATION DE LA LISTE :")) + m_pQListViewItem->text(0),
        	                       QString(tr("Cette liste :&nbsp;&nbsp;\"")) + m_pQListViewItem->text(0) + tr("\"&nbsp;&nbsp; est réferencée par: <br><br> ") + listFolder  + "<br><br>",
        	                       "&Valider");
}

//------------------------------------ listViewList_clicked --------------------------------------------------
void C_DlgListFieldMngr::listViewList_clicked(  int button, QListViewItem * qlistViewItem, const QPoint & pos, int c  )
{   if (qlistViewItem==0)           return;
    if (button==2)
    {listViewList_rightButtonClicked(qlistViewItem, pos, c );
        return;
    }
    //listViewList_doubleClicked(qlistViewItem);
    QString qstr;
    if (qlistViewItem->text(0)=="") return;
    saveTextIfModified();
    //.......................On est deja sur un item enfant: depth()>0 ................................................
    //                   alors si c'est un dossier le selectionner dans la liste
    if (qlistViewItem->depth()>0)
    {if (qlistViewItem->text(1)=="D")
        {   textEditField->setText("");
            textEditField->setEnabled (FALSE );
            m_OldpQlistViewItem=0;

        }
        else if (qlistViewItem->text(1)=="T")
        {QString  text = "";
            CGestIni::Param_UpdateFromDisk(m_PathListes + QDir::separator() + GetPathItem (qlistViewItem), text);
            textEditField->setText(text);
            textEditField->setEnabled (TRUE);
            m_OldpQlistViewItem=qlistViewItem;
        }
        else if (qlistViewItem->text(1)=="F")
        {textEditField->setText("");
            textEditField->setEnabled (TRUE);
            m_OldpQlistViewItem=qlistViewItem;
        }
    }
    else
    {if (qlistViewItem->childCount ()<=0)
        {AddSubFolder (qlistViewItem,  qlistViewItem->text(0));
        }
        if (qlistViewItem->isOpen()) qlistViewItem->setOpen (FALSE);
        else                         qlistViewItem->setOpen (TRUE);
        textEditField->setText("");
        textEditField->setEnabled (FALSE );
        m_OldpQlistViewItem=0;
    }
    pushButtonSave->setDisabled(TRUE);
}

//------------------------------------ IsThisFolderExist --------------------------------------------------
bool C_DlgListFieldMngr::IsThisFolderExist(const char *path, const char* folderName )
{QDir dir(QString(path)+ QDir::separator() + folderName);
    return dir.exists();
}
//------------------------------------ IsThisFolderExist --------------------------------------------------
bool C_DlgListFieldMngr::IsThisFileExist(const char* path )
{QDir dir;
    return dir.exists(path );
}
//------------------------------------ listViewList_returnPressed --------------------------------------------------
void C_DlgListFieldMngr::listViewList_returnPressed( QListViewItem *qlistViewItem )
{listViewList_doubleClicked(qlistViewItem);
}

//------------------------------------ pushButtonSave_clicked --------------------------------------------------
void C_DlgListFieldMngr::pushButtonSave_clicked()
{   QListViewItem *qlistViewItem = listViewList->currentItem ();
    if (qlistViewItem ==0)          return;
    if (qlistViewItem->depth() !=1) return;

    saveFile(m_PathListes + QDir::separator() + GetPathItem (qlistViewItem) , textEditField->text(), qlistViewItem  );
}
//----------------------------------------- saveTextIfModified -----------------------------------------------------------------
bool C_DlgListFieldMngr::saveTextIfModified()
{   bool ret = FALSE;
    QListViewItem *qlistViewItemToBeRemoved = 0;
    if (textEditField->isModified () && m_OldpQlistViewItem )
    {if ( QMessageBox::warning( this, tr("FICHIER MODIFIÉ"),
        	                tr ( "Faut-il&nbsp;enregistrer&nbsp;les&nbsp;modifications&nbsp;de&nbsp;:&nbsp;<b>%1</b><br>"
        	                     "avant de modifier le nouveau document ?").arg(m_OldpQlistViewItem->text(0)),
        	                tr("&Enregistrer"), tr("&Non"), 0,
        	                0, 1 ) == 0 )
        {   QString path = m_PathListes + QDir::separator() + GetPathItem (m_OldpQlistViewItem);
            QString text = textEditField->text();
            QFile qfile(path);
            if (qfile.open(IO_ReadWrite)==FALSE)
            {   QString mess  = tr("Le fichier : <br><b>" );
                mess += path;
                mess += tr("</b><br> n'a pu être ouvert en écriture, <br> veuillez vérifier si il est permis d'y écrire");
                CGenTools::MessageAlert(this, tr("ERREUR :"),mess, tr("Annu&ler"));
            }
            else if (text !="")
            {
                path        = QDir::cleanDirPath ( path );
                QString ext = QFileInfo(path).extension();
                path        = QFileInfo(path).dirPath() + "/"+QFileInfo(path).baseName() + ".html";
                qfile.setName (path);
                //..... si ce fichier n'a pas l'extension html (mais va la prendre) ..........
                //      et qu'il existe déjà un fichier  du meme nom avec l'extension html
                //      alors ce fichier est déjà dans la liste d'item et il faut le virer
                //      de cette liste
                if (ext != "html" && IsThisFileExist(path))
                {qlistViewItemToBeRemoved = listViewList->findItem (QFileInfo (qfile).fileName(), 0);
                }

                qfile.remove();
                if (qfile.open(IO_WriteOnly)==FALSE)
                {  QString mess  = tr("Le fichier : <br><b>" );
                    mess += path;
                    mess += tr("</b><br> n'a pu être recréé");
                    CGenTools::MessageAlert(this, tr( "ERREUR :" ),mess, tr( "Annu&ler" ));
        	}
                else
                {if (text.left(6) != "<html>")
                    {if (text.length())
                        {text.prepend("<html><qt><head><meta name=\"qrichtext\" content=\"1\" /></head><body>"
        	                      "<p><font color=#000000 face=\"Arial\">");
                            text += "</p></font></body></qt></html>";
                	}
                    }
                    QTextStream ts( &qfile );
                    ts << text;

                    m_OldpQlistViewItem->setText(0,  QFileInfo (qfile).fileName());
                    m_OldpQlistViewItem->setText(1, "T");
                    m_OldpQlistViewItem->setPixmap (0, Theme::getIcon( "22x22/file_text.png") );
                    pushButtonSave->setDisabled(TRUE);
                    textEditField->setModified(FALSE);
                    ret = TRUE;
                }
            }
            qfile.close();
        }
        else { ret = TRUE;}
    }
    else { ret = TRUE;}
    m_OldpQlistViewItem = 0;
    if (qlistViewItemToBeRemoved) delete qlistViewItemToBeRemoved;
    return ret;
}

//------------------------------------ saveFile --------------------------------------------------
void C_DlgListFieldMngr::saveFile( QString path, QString text, QListViewItem *qlistViewItem )
{
    QListViewItem *qlistViewItemToBeRemoved = 0;
    QFile qfile(path);
    if (qfile.open(IO_ReadWrite)==FALSE)
    {   QString mess  = tr("Le fichier : <br><b>");
        mess += path;
        mess += tr("</b><br> n'a pu être ouvert en écriture, <br> veuillez vérifier si il est permis d'y écrire");
        CGenTools::MessageAlert(this, tr("ERREUR :"),mess, tr("Annu&ler"));
    }
    else if (text  != "")
    {   qfile.remove();
        path        = QDir::cleanDirPath ( path );
        QString ext = QFileInfo(path).extension();
        path        = QFileInfo(path).dirPath() + "/"+QFileInfo(path).baseName() + ".html";

        qfile.setName (path);
        //..... si ce fichier n'a pas l'extension .html (mais va la prendre) ..........
        //      et qu'il existe déjà un fichier  du meme nom avec l'extension html
        //      alors ce fichier est déjà dans la liste d'item et il faut le virer
        //      de cette liste
        if (ext != "html" && IsThisFileExist(path))
        {   QString itemName = QFileInfo (qfile).fileName();
            qlistViewItemToBeRemoved = listViewList->findItem (itemName, 0);
        }

        if (qfile.open(IO_WriteOnly)==FALSE)
        {   QString mess  = tr("Le fichier : <br><b>");
            mess += path;
            mess += tr("</b><br> n'a pu être recréé");
            CGenTools::MessageAlert(this, tr("ERREUR :"),mess, tr("Annu&ler"));
        }
        else
        {if (text.left(6) != "<html>")
            {if (text.length())
                {text.prepend("<html><qt><head><meta name=\"qrichtext\" content=\"1\" /></head><body>"
                              "<p><font color=#000000 face=\"Arial\">");
                    text += "</p></font></body></qt></html>";
                }
            }
            QTextStream ts( &qfile );
            ts << text;

            qlistViewItem->setText(0, QFileInfo (qfile).fileName() );
            qlistViewItem->setText(1, "T");
            qlistViewItem->setPixmap (0, Theme::getIcon( "22x22/file_text.png") );
            pushButtonSave->setDisabled(TRUE);
            textEditField->setModified(FALSE);
            qfile.close();
        }
    }
    else
    {qfile.close();
    }
    if (qlistViewItemToBeRemoved) delete qlistViewItemToBeRemoved;
}
//------------------------------------ lineEditFindItem_textChanged --------------------------------------------------
void C_DlgListFieldMngr::lineEditFindItem_textChanged( const QString &qtxt )
{if (qtxt=="")                return;
    QListViewItem *qlistViewItem = FindApproximativeList(qtxt,0);
    if (qlistViewItem==0)        return;
    saveTextIfModified();
    listViewList->ensureItemVisible (qlistViewItem );
    listViewList->setCurrentItem (qlistViewItem );
    m_OldpQlistViewItem = qlistViewItem;
}

//------------------------------------ findApproximativeList --------------------------------------------------
QListViewItem *C_DlgListFieldMngr::FindApproximativeList(const QString &name, int depth)
{
    QListViewItemIterator it( listViewList);
    while ( it.current() )
    {QListViewItem *item = it.current();
        ++it;
        //if ( item->depth()==depth &&  item->text(0).contains (name, FALSE )) return item;
        if ( item->depth()==depth)
        {QString item_name = item->text(0).left(name.length());
            if (QString::compare(item_name.lower() , name.lower())==0) return item;
            //item->text(0).contains (name, FALSE )) return item;
        }
    }
    return 0;
}

//------------------------------------ pushButtonSetMasque_clicked --------------------------------------------------
void C_DlgListFieldMngr::pushButtonSetMasque_clicked()
{    ThemePopup *pQPopupMenu = new ThemePopup(this, "MyPopupMenu" );
    pushButtonSetMasque->setIconSet (Theme::getIconDownArrow());
    //pQPopupMenu->insertItem( QPixmap( undo ),          ("&Undo"),     this, SLOT(undo()),    CTRL+Key_Z );
    //pQPopupMenu->insertItem( QPixmap( redo ),          ("&Redo"),     this, SLOT( redo()),   CTRL+Key_Y );
    //pQPopupMenu->insertItem( QPixmap( editcut ),       ("&Cut"),      this, SLOT( cut()),    CTRL+Key_X );
    //pQPopupMenu->insertItem( QPixmap( editcopy ),      ("C&opy"),     this, SLOT( copy()),   CTRL+Key_C );
    //pQPopupMenu->insertItem( QPixmap( editpaste ),     ("&Paste"),    this, SLOT( paste()),  CTRL+Key_C );

    pQPopupMenu->insertItem(Theme::getIcon( "22x22/CreateList_Item.png"),       tr("C&réer une liste déroulante avec le texte sélectionné."),       this, SLOT( MenuActionListNewRef()),      CTRL+Key_F );
    pQPopupMenu->insertItem(Theme::getIcon( "22x22/InsererChampInsertion.png"), tr("I&nsérer au curseur un champ d'insertion ou d'extraction."),    this, SLOT( MenuActionInsertField()),     CTRL+Key_E );
    pQPopupMenu->insertItem(Theme::getIcon( "22x22/InsertList_Item.png"),       tr("&Insérer au curseur une référence à une liste déroulante."),    this, SLOT( MenuActionListGet()),         CTRL+Key_N );
    pQPopupMenu->insertItem(Theme::getIcon( "22x22/filefind.png"),              tr("In&sérer au curseur le chemin d'un fichier choisi avec le sélecteur de fichiers."),    this, SLOT( MenuActionListGetFile()),         CTRL+Key_N );

    pQPopupMenu->exec(QCursor::pos());
    delete pQPopupMenu;
    pushButtonSetMasque->setIconSet (Theme::getIconRightArrow());
}
//-------------------------------------------------- HtmlToAscii -----------------------------------------------------------------
QString C_DlgListFieldMngr::HtmlToAscii(QString str)
{int posDeb=0;
    int posEnd=0;
    str.replace("<br />","\r\n");
    str.replace("</p>","\r\n\r\n");
    str.replace("<li>","\t");
    str.replace("</li>","\r\n");
    str.replace("</ul>","\r\n");
    str.replace("</div>","\r\n");
    str.replace("</div>","\r\n");
    str.replace("&lt;","<");
    str.replace("&gt;",">");
    do {posDeb = str.find("<",posDeb);
        if (posDeb !=-1)
        {posEnd =  str.find(">", posDeb+1);
            if (posEnd != -1) str.remove(posDeb, posEnd-posDeb+1);
        }
        else break;
    } while (1);
    return str;
}
//------------------------------------ MenuActionListGet --------------------------------------------------
void C_DlgListFieldMngr::MenuActionListGetFile()
{    QString s = QFileDialog::getOpenFileName(
                                               QFileInfo(m_PathGetFile).dirPath (),
                                               tr("Fichiers html (*.html *.HTML);;Ordonnances(*.ORD *.ord);;Fichiers textes( *.txt *.TXT);; Autres(*.*)"),
                                               this,
                                               tr("Sélecteur de fichiers"),
                                               tr("Choisissez un fichier") );
     if (s.length()==0) return;
     m_PathGetFile  = s;
     if (s.startsWith(m_PathGlossaire))
        {s = QString("$Glossaire/") + s.mid(m_PathGlossaire.length());
        }
     s = s.replace("(","\\(");
     s = s.replace(")","\\)");
     QColor old_col = textEditField->color();
     textEditField->setColor( QColor(0x00,0x00,0x00) );
     if (textEditField->hasSelectedText()) textEditField->del();
     else                                  textEditField->insert(QString(" "));
     textEditField->setColor( QColor(0x00,0x00,0x00) );
     textEditField->insert( s );
     textEditField->setColor( old_col );
}
//------------------------------------ MenuActionListGet --------------------------------------------------
void C_DlgListFieldMngr::MenuActionListGet()
{   FormDlgListOff_Listes *dlg = new FormDlgListOff_Listes(this,"RefList_Dial",TRUE);
    if (dlg==0) return;
    dlg->initDialog(m_PathGlossaire+"/Champs d'insertions");
    dlg-> exec();
    if (dlg->m_ItemName !="")
    {   QColor old_col = textEditField->color();
        textEditField->setColor( QColor(0x00,0x00,0x00) );
        if (textEditField->hasSelectedText()) textEditField->del();
        else                                  textEditField->insert(QString(" "));
        textEditField->setColor( QColor(0x00,0x00,0xFF) );
        textEditField->insert( dlg->m_ItemName );
        textEditField->setColor( old_col );
    }
    delete dlg;
}

//------------------------------------ MenuActionInsertField --------------------------------------------------
void C_DlgListFieldMngr::MenuActionInsertField()
{   QStringList fieldList;
    fieldList.append(tr("$start_title \"Placer ici le titre que vous souhaiter afficher\" $end_title"));
    fieldList.append(tr("$no_style"));
    MakeInsertFieldList(m_PathDrTux + "/Ressources/FusionListe.txt", fieldList, 0);
    MakeInsertFieldList(m_PathDrTux + "/Ressources/ActionListe.txt", fieldList, 0);
    Dlg_InsertList *dlg = new Dlg_InsertList(this, "InsertList", TRUE);
    if (dlg == 0)                          return;
    dlg->initDialog( fieldList );
    if (dlg->exec() != QDialog::Accepted)   {delete dlg; return;}

    QListBoxItem *pQListBoxItem  = dlg->listBox_InsertList->selectedItem();
    if (pQListBoxItem)
    {   QColor old_col = textEditField->color();
        textEditField->setColor( QColor(0x00,0x00,0x00) );
        if (textEditField->hasSelectedText()) textEditField->del();
        else                                  textEditField->insert(QString(" "));
        textEditField->setColor( QColor(0x00,0x00,0x00) );
        textEditField->insert( pQListBoxItem->text() );
        textEditField->setColor( old_col );
    }
    delete dlg;
}

//---------------------------- MakeInsertFieldList -------------------------------------------------
void C_DlgListFieldMngr::MakeInsertFieldList(const char *path, QStringList &fieldList, int mustBeClear )
{
    QString text;
    int isUtf8;
    CGestIni::Param_UpdateFromDisk(path, text, &isUtf8);
    if (text.length()<=0) return;
    char *pt        = (char*)(const char*) text;
    char *start     = pt;
    char *deb       = pt;
    long len        = 0;
    if (mustBeClear) fieldList.clear();
    while (*pt)
    { if (*pt=='\r' || *pt=='\n')
        {len = pt - deb;
            if (len>0)  fieldList.append(text.mid( (deb-start), len ).stripWhiteSpace());
            if      (*pt=='\r' &&  pt[1]=='\n' ) pt+=2;
            else if (*pt=='\n' &&  pt[1]=='\r' ) pt+=2;
            else                                 ++pt;
            deb = pt;
        }
        else
        {++pt;
        } //end if (*pt=='\r' || *pt=='\n')
    }
}
//------------------------------------ MenuActionListNewRef --------------------------------------------------
void C_DlgListFieldMngr::MenuActionListNewRef()
{   QString qsName(HtmlToAscii(textEditField->selectedText ()));
    // QListViewItem *qlistViewItem;
    QListViewItem *element;

    if (CGenTools::DialogGetString(this, tr("Veuillez donner le nom de cette nouvelle liste déroulante"), qsName) ==  QDialog::Accepted)
    { qsName = qsName.stripWhiteSpace();
        if (qsName=="")
        {CGenTools::MessageAlert(this, tr("ATTENTION !"),tr("Le nom de la référence est vide.<br>Veuillez choisir un nom"), tr("Annu&ler"));
            return;
        }

        //........... créer si besoin la liste à laquelle se réfère la réference....................
        if (IsThisFolderExist(m_PathListes, qsName)==FALSE)     // que si n'existe pas déjà
        {   QDir dir;
            if  (!IsThisFileNameCompatible(qsName)) return;
            if (dir.mkdir (m_PathListes + QDir::separator() + qsName)==FALSE)
            {  QString mess  = tr("Cette liste<br><b>");
                mess += m_PathListes + QDir::separator() + qsName;
                mess += tr("</b><br> n'a pu être créée.");
                CGenTools::MessageAlert(this, tr("ERREUR :"),mess, tr("Annu&ler"));
                return;
            }
            //.............. mettre à jour le contenu des listes (que si creation)......................................
            saveTextIfModified();
            element = new QListViewItem( listViewList, qsName, "D"); // rajouter la liste crée à la racine de la listbox
            if (element)
            {   element->setPixmap ( 0, Theme::getIcon( "16x16/folder.png")  );
                listViewList->ensureItemVisible (element);
                listViewList->setCurrentItem (m_OldpQlistViewItem );
                listViewList->setSelected (m_OldpQlistViewItem , TRUE);
            }
        }
        QColor old_col = textEditField->color();
        if (textEditField->hasSelectedText()) textEditField->del();
        textEditField->setColor( QColor(0x00,0x00,0xFF) );
        textEditField->insert(qsName);
        textEditField->setColor( old_col );
    }
}

//------------------------------------ listViewList_spacePressed --------------------------------------------------
void C_DlgListFieldMngr::listViewList_spacePressed( QListViewItem *pQListViewItem )
{   m_pQListViewItem = pQListViewItem;
    MenuActionRename();
}

//------------------------------------ pushButtonSetTextField_clicked --------------------------------------------------
void C_DlgListFieldMngr::pushButtonSetTextField_clicked()
{if (textEditField->hasSelectedText())  MenuActionListNewRef();
}


//------------------------------------ lineEditFindItem_returnPressed --------------------------------------------------
void C_DlgListFieldMngr::lineEditFindItem_returnPressed()
{QListViewItem *pQListViewItem = listViewList->currentItem ();
    if (pQListViewItem ) listViewList_returnPressed( pQListViewItem );
}


//------------------------------------ pushButton_Quitter_clicked --------------------------------------------------
void C_DlgListFieldMngr::pushButton_Quitter_clicked()
{reject();
}


//------------------------------------ pushButton_Ok_clicked --------------------------------------------------
void C_DlgListFieldMngr::pushButton_Ok_clicked()
{accept();
}

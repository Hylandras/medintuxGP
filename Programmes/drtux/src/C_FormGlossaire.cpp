//
// C++ Implementation:
/********************************* C_FormGlossaire.cpp *****************************************

    Project: MedinTux
    Copyright (C) 2005 by Roland Sevin  and Data Medical Design
    E-Mail: medintux@medintux.org

    This program is free software; you can redistribute it and/or modify
    it under the terms of the CeCILL Version 1 du 21/06/2004
    License (GPL compatible)
                               http://www.cecill.info/
    as published by :

    Commissariat � l'Energie Atomique
    - CEA,
                             31-33 rue de la F�d�ration, 75752 PARIS cedex 15.
                             FRANCE
    Centre National de la Recherche Scientifique
    - CNRS,
                             3 rue Michel-Ange, 75794 Paris cedex 16.
                             FRANCE
    Institut National de Recherche en Informatique et en Automatique
    - INRIA,
                             Domaine de Voluceau, Rocquencourt, BP 105, 78153
                             FRANCE

          This program is distributed in the hope that it will be useful,
          but WITHOUT ANY WARRANTY; without even the implied warranty of
          MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
          CeCILL  License (GPL compatible) for more details.
*/

#include "C_FormGlossaire.h"

#include <qvariant.h>
#include <qpoint.h>
#include <qtimer.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qsplitter.h>
#include <qheader.h>
#include <qlistview.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qfiledialog.h>
#include <qlistview.h>
#include <qcursor.h>
#include <qinputdialog.h>

#include "../../MedinTuxTools/CHtmlTools.h"
#include "../../MedinTuxTools/CW_EditText.h"
#include "../../MedinTuxTools/CPrtQListViewItem.h"

#include "CApp.h"
#include "drtux.h"
#include "MyEditText.h"
#include "CMDI_Observation.h"
#include "CMDI_Prescription.h"
//#include "CMDI_Documents.h"

//======================================================== C_FormGlossaire =======================================================

//----------------------------------------- C_FormGlossaire --------------------------------
/*! \brief Constructeur de la classe. */
C_FormGlossaire::C_FormGlossaire( QWidget* parent /* = 0 */, const char* name /* = 0 */, WFlags fl /* = 0 */ )
    : FormGlossaire(parent,name,fl)
{   QGridLayout *mainFrameLayout = new QGridLayout( frame_main, 1, 1, 0, 0, "mainFrameLayout"); 
    m_splitter = new QSplitter( frame_main, "m_splitter" );
    m_splitter->setOrientation( QSplitter::Vertical );
    m_ListView_Glossaire = new C_DragQListView( &m_curPath, m_splitter, "m_ListView_Glossaire" );
    m_ListView_Glossaire->addColumn( tr( "            Navigation                                   " ) );
    m_ListView_Glossaire->addColumn( tr( "type" ) );
    m_ListView_Glossaire->header()->hide();
    m_ListView_Glossaire->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    m_ListView_Glossaire->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    m_ListView_Glossaire->setAcceptDrops( TRUE );
    m_ListView_Glossaire->setFrameShape( QListView::StyledPanel );
    m_ListView_Glossaire->setFrameShadow( QListView::Sunken );
    m_ListView_Glossaire->setRootIsDecorated( FALSE );

/*
QSizePolicy::Fixed - The QWidget::sizeHint() is the only acceptable alternative, so the widget can never grow or shrink (e.g. the vertical direction of a push button). 
QSizePolicy::Minimum - The sizeHint() is minimal, and sufficient. The widget can be expanded, but there is no advantage to it being larger (e.g. the horizontal direction of a push button). It cannot be smaller than the size provided by sizeHint(). 
QSizePolicy::Maximum - The sizeHint() is a maximum. The widget can be shrunk any amount without detriment if other widgets need the space (e.g. a separator line). It cannot be larger than the size provided by sizeHint(). 
QSizePolicy::Preferred - The sizeHint() is best, but the widget can be shrunk and still be useful. The widget can be expanded, but there is no advantage to it being larger than sizeHint() (the default QWidget policy). 
QSizePolicy::Expanding - The sizeHint() is a sensible size, but the widget can be shrunk and still be useful. The widget can make use of extra space, so it should get as much space as possible (e.g. the horizontal direction of a slider). 
QSizePolicy::MinimumExpanding - The sizeHint() is minimal, and sufficient. The widget can make use of extra space, so it should get as much space as possible (e.g. the horizontal direction of a slider). 
QSizePolicy::Ignored - th
*/

    m_textEditView = new CW_EditText( m_splitter, "m_textEditView" );
    //FormGlossaireLayout->addWidget( m_splitter, 1, 0 );
    mainFrameLayout->addWidget( m_splitter, 0, 0 );

    //m_textEditView->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Expanding ); 
    //m_ListView_Glossaire->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Maximum );
    //m_splitter->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Expanding );
    connect( pushButtonBack,   SIGNAL( clicked() ), this, SLOT( pushButtonBack_clicked() ) );
    connect( pushButtonHome,   SIGNAL( clicked() ), this, SLOT( pushButtonHome_clicked() ) );
    connect( pushButtonMenu,   SIGNAL( clicked() ), this, SLOT( pushButtonMenu_clicked() ) );
    connect( pushButtonNewDir, SIGNAL( clicked() ), this, SLOT( pushButtonNewDir_clicked() ) );
    connect( m_ListView_Glossaire,    SIGNAL( mouseButtonClicked ( int , QListViewItem * , const QPoint & , int  )),this,  SLOT( On_mouseButtonClicked ( int , QListViewItem * , const QPoint & , int  ) ) );
    connect( m_ListView_Glossaire,    SIGNAL( doubleClicked(QListViewItem*) ),    this,    SLOT( ListView_Glossaire_doubleClicked(QListViewItem*) ) );
    connect( m_ListView_Glossaire,    SIGNAL( returnPressed(QListViewItem*) ),    this,    SLOT( ListView_Glossaire_returnPressed(QListViewItem*) ) );
    connect( m_ListView_Glossaire,    SIGNAL( selectionChanged(QListViewItem*) ), this,    SLOT( ListView_Glossaire_selectionChanged(QListViewItem*) ) );
    connect( pushButtonEdit,          SIGNAL( clicked() ), this,                           SLOT( pushButtonEdit_clicked() ) );
    connect( but_ConfFiltre_HideShow, SIGNAL( clicked() ), this,                           SLOT( but_ConfFiltre_HideShow_clicked() ) );
    connect( m_ListView_Glossaire, SIGNAL( contextMenuRequested(QListViewItem*,const QPoint&,int) ), this, SLOT( ListView_Glossaire_contextMenuRequested(QListViewItem*,const QPoint&,int) ) );
    pushButtonNewDir->setPixmap(Theme::getIcon("16x16/NewDir.png"));
    pushButtonMenu->setPixmap(Theme::getIcon("16x16/ThereIsMenuHere.png"));
    pushButtonHome->setPixmap(Theme::getIcon("22x22/home.png"));
    pushButtonEdit->setPixmap(Theme::getIcon("22x22/edit.png"));
    pushButtonLocalise->setPixmap(Theme::getIcon("whereIsGlossaire.png" ));
    pushButtonBack->setPixmap(Theme::getIcon("22x22/previous.png" ));

    m_textEditView->setTextFormat(Qt::RichText);

    QString pathAppli = QFileInfo (qApp->argv()[0]).dirPath (true) + "/";
    m_textEditView->mimeSourceFactory()->addFilePath(pathAppli + "Ressources/RubriquesTextes");
    m_textEditView->show();

    GetMasquesList(m_ListView_Glossaire, G_pCApp->m_PathGlossaire + RUBNAME_OBSERVATION );
    m_pLastQListViewItem     = 0;
    m_pThemePopup            = 0;
    m_PopupItem              = "";
    m_PopupFilePath          = "";
    m_curPath                = G_pCApp->m_PathGlossaire + RUBNAME_DOCUMENTS;
   
    m_DefaultMaskDirectory   = m_curPath;
    m_RubType                = TYP_OBSERVATION;
    m_curPath                = QDir::cleanDirPath(m_QDir.absPath ());
    //m_DefaultMaskDirectory   = QDir ( m_DefaultMaskDirectory ).absPath();
    m_DefaultMaskDirectory   = m_curPath;
    m_QDir.setPath ( m_curPath );
    textLabel_PathGlossaire->setText(G_pCApp->m_PathGlossaire);
    //pushButtonBack->hide ();
    //pushButtonHome->hide ();
    //pushButtonMenu->hide ();
    //pushButtonNewDir->hide ();
    //pushButtonEdit->hide ();
    //pushButtonLocalise->hide ();
    frame_header->hide();
#ifdef Q_OS_MACX
    pushButtonBack->setFlat (TRUE);
    pushButtonHome->setFlat (TRUE);
    pushButtonMenu->setFlat (TRUE);
    pushButtonNewDir->setFlat (TRUE);
    pushButtonEdit->setFlat (TRUE);
    pushButtonLocalise->setFlat (TRUE);
#endif

}

//----------------------------------------- ~C_FormGlossaire --------------------------------
C_FormGlossaire::~C_FormGlossaire()
{
}

//----------------------------------------- pushButtonMenu_clicked --------------------------------
void C_FormGlossaire::pushButtonMenu_clicked()
{G_pCApp->m_pDrTux->Slot_MenuRubriques();
}
//----------------------------------------- but_ConfFiltre_HideShow_clicked --------------------------------
void C_FormGlossaire::but_ConfFiltre_HideShow_clicked()
{   


    //QString mode = tr("non");
    //if (frame_ConfigHeader->isVisible()) {frame_ConfigHeader->hide();  mode = tr("non");}
    //else                                 {frame_ConfigHeader->show();  mode = tr("oui");}
    // WRITE_USER_PARAM(&USER_PARAM, "Dock_Vigie", tr("Tools visibility"), mode);

    QRect rect = frame_header->geometry();
    if (frame_header->isVisible())
       { frame_header->hide();
       }
   else  
       { frame_header->show();
       }
}
//----------------------------------------- pushButtonNewDir_clicked --------------------------------
void C_FormGlossaire::pushButtonNewDir_clicked()
{if (!QDir(m_curPath).exists())
    {G_pCApp->CouCou( tr("Le Glossaire est inutile et ne sert à rien pour la rubrique : ") + GetCurrentRubName());
     return;
    }
 QString curDir = m_curPath;
 QFileInfo fi(curDir);
 if (fi.isFile()) curDir = fi.dirPath();
 QDir dir (curDir);
 if (!dir.exists())   return;
 QString ret = QInputDialog::getText (tr("Nouveau dossier dans le Glossaire"), tr("Nom du dossier à créer : "), QLineEdit::Normal, QString::null, 0, this, "pushButtonNewDir_clicked()" );
 if (ret.length()==0) return;
 dir.mkdir(ret.replace(':','_'));
 Slot_GlossaireRefresh();
}

//----------------------------------------- pushButtonDelDir_clicked --------------------------------
void C_FormGlossaire::pushButtonDelDir_clicked()
{QListViewItem * qlistViewItem = m_ListView_Glossaire->currentItem ();
 if (qlistViewItem<=0)               return;
 if (qlistViewItem->text(0)[0]=='.') return;
 if ( QMessageBox::warning( this,           (" Effacer un répertoire"),
                                             tr ( "<u>ATTENTION :</u>  L'effacement du répertoire : <br><b>")   +
                                             qlistViewItem->text(0)+
                                             tr(  "</b><br>et de son contenu, sera définitif et non réversible.<br>"),
                                             tr("&effacer"), tr("Annu&ler"), 0,
                                             1, 1 ) == 1)
          {return;
          }
 QString curDir = m_curPath;
 QFileInfo fi(curDir);
 if (fi.isFile()) curDir = fi.dirPath();
 curDir.replace ('\\','/');
 if ( !curDir.endsWith("/")) curDir += "/";
 curDir += qlistViewItem->text(0);
 CHtmlTools::eraseDirectory( curDir);
 Slot_GlossaireRefresh();
}

//------------------------------- Slot_PopupActionRenameDir  --------------------------------------------------------
void C_FormGlossaire::Slot_PopupActionRenameDir()
{QListViewItem * qlistViewItem = m_ListView_Glossaire->currentItem ();
 if (qlistViewItem<=0)               return;
 if (qlistViewItem->text(0)[0]=='.') return;
 QString newDir = m_curPath;
 QFileInfo fi(newDir);
 if (fi.isFile()) newDir = fi.dirPath();
 newDir.replace ('\\','/');
 if ( !newDir.endsWith("/")) newDir += "/";
 bool ok;
 QString ret = QInputDialog::getText (tr("Renommer un répertoire"), tr("Nouveau nom du répertoire : "), QLineEdit::Normal, qlistViewItem->text(0), &ok, this, "pushButtonNewDir_clicked()" );
 if ( ok && !ret.isEmpty() )
    {QDir dir(newDir);
     dir.rename(newDir + qlistViewItem->text(0), newDir + ret.replace(':','_'));
     Slot_GlossaireRefresh();
    }
}

//----------------------------------------- dragClose --------------------------------
void C_FormGlossaire::dragClose()
{m_ListView_Glossaire->dragClose();
}

//------------------------------- setInterfaceOnPermisions -------------------------------------------------------------
void C_FormGlossaire::setInterfaceOnPermisions(const QString &droits)
{if (G_pCApp->IsThisDroitExist(droits, "gld"))      pushButtonLocalise->setEnabled (  TRUE );
 else                                               pushButtonLocalise->setEnabled (  FALSE );
 if (G_pCApp->IsThisDroitExist(droits, "glc"))
                                                   {pushButtonEdit->setEnabled (    TRUE );
                                                    pushButtonNewDir->setEnabled (  TRUE );
                                                   }
 else                                              {pushButtonEdit->setEnabled (    FALSE );
                                                    pushButtonNewDir->setEnabled (  FALSE );
                                                   }
}


//------------------------------- GetCurrentFileName ------------------------------------------------------
/*! \brief retourne le nom de fichier actuellement sélectionné . */
QString C_FormGlossaire::GetCurrentFileName()
{
 QListViewItem  *pQListViewItem = m_ListView_Glossaire->currentItem();
 if ( pQListViewItem == 0 || pQListViewItem->text(0)[0] == '.')
    {pQListViewItem = m_ListView_Glossaire->firstChild();
     while( pQListViewItem && pQListViewItem->text(0)[0] == '.')
          {pQListViewItem = pQListViewItem->nextSibling();
          }
     }
 else
     {return m_curPath + "/" + pQListViewItem->text(0);
     }
 if ( pQListViewItem == 0 )  return QString::null;
 QString fileName = pQListViewItem->text(0);
 if (fileName[0] == '.')     return QString::null;
 m_ListView_Glossaire->setSelected ( pQListViewItem, TRUE );
 return m_curPath + "/" + fileName;
}

//------------------------------- ListView_Glossaire_clicked -------------------------------------------
void C_FormGlossaire::ListView_Glossaire_clicked()
{ListView_Glossaire_clicked( m_ListView_Glossaire->currentItem () );
}

//------------------------------- On_mouseButtonClicked -------------------------------------------
void C_FormGlossaire::On_mouseButtonClicked (  int button, QListViewItem * qlistViewItem, const QPoint & , int  )
{if (qlistViewItem<=0) return;
 if (button == 2)      return;
 //.......................... faire part au Dr Tux du changement d'identité ............................................
 if (qlistViewItem->text(1)=="D"||qlistViewItem->text(1)=="B")
    {GetMasquesList(m_ListView_Glossaire, m_curPath+"/"+qlistViewItem->text(0));
     emit Sign_DirectoryChanged(m_curPath, m_DefaultMaskDirectory);
    }
 else if (qlistViewItem->text(1)=="F")
    {//if (m_pLastQListViewItem==qlistViewItem) return;
     m_ListView_Glossaire->disconnect( SIGNAL(selectionChanged(QListViewItem *)) );
     SetTextEditWithData( qlistViewItem->text(0) , m_textEditView);
     emit Sign_FileChanged(m_curPath + "/" + qlistViewItem->text(0), PathGlossaireToTyp());
     QTimer::singleShot( 200, this, SLOT(Slot_RestaureConnection()) );
    }
 m_pLastQListViewItem=qlistViewItem;
}

//------------------------------- ListView_Glossaire_clicked -------------------------------------------
void C_FormGlossaire::ListView_Glossaire_clicked( QListViewItem * qlistViewItem)
{if (qlistViewItem<=0) return;

 //.......................... faire part au Dr Tux du changement d'identité ............................................
 if (qlistViewItem->text(1)=="D"||qlistViewItem->text(1)=="B")
    {GetMasquesList(m_ListView_Glossaire, m_curPath+"/"+qlistViewItem->text(0));
     emit Sign_DirectoryChanged(m_curPath, m_DefaultMaskDirectory);
    }
 else if (qlistViewItem->text(1)=="F")
    {//if (m_pLastQListViewItem==qlistViewItem) return;
     m_ListView_Glossaire->disconnect( SIGNAL(selectionChanged(QListViewItem *)) );
     SetTextEditWithData( qlistViewItem->text(0) , m_textEditView);
     emit Sign_FileChanged(m_curPath + "/" + qlistViewItem->text(0), PathGlossaireToTyp());
     QTimer::singleShot( 200, this, SLOT(Slot_RestaureConnection()) );
    }
 //m_pLastQListViewItem=qlistViewItem;
}

//------------------------------- Slot_RestaureConnection ----------------------------------------------
/*! \brief recrée les connections du listView */
void C_FormGlossaire::Slot_RestaureConnection()
{connect( m_ListView_Glossaire,         SIGNAL( selectionChanged(QListViewItem *) ),
          this,                         SLOT(ListView_Glossaire_selectionChanged (QListViewItem *))
        );
 connect( m_ListView_Glossaire,         SIGNAL( doubleClicked(QListViewItem *) ),
          this,                         SLOT(ListView_Glossaire_doubleClicked (QListViewItem *))
        );
}

//------------------------------- Slot_GlossaireRefresh ------------------------------------------------
void C_FormGlossaire::Slot_GlossaireRefresh()
{ GetMasquesList(m_ListView_Glossaire, m_curPath);
}

//------------------------------- SetCurrentPath ------------------------------------------------
void C_FormGlossaire::SetCurrentPath(const QString &path, const QString &defaultMaskDirectory)
{   m_curPath = QDir::cleanDirPath( QDir( path ).absPath() );
    textLabel_PathGlossaire->setText( m_curPath );
if (defaultMaskDirectory=="?")  m_DefaultMaskDirectory = m_curPath; //  ? force m_DefaultMaskDirectory=m_curPath
    else if (defaultMaskDirectory.length())   m_DefaultMaskDirectory = QDir::cleanDirPath(QDir ( defaultMaskDirectory ).absPath());
    GetMasquesList(m_ListView_Glossaire, m_curPath);
}
//------------------------------- GetCurrentPath ------------------------------------------------
void C_FormGlossaire::GetCurrentPath(QString &path, QString &defaultMaskDirectory)
{   path                 = m_curPath;
    defaultMaskDirectory = m_DefaultMaskDirectory;
}

//------------------------------- pushButtonBack_clicked ----------------------------------------
void C_FormGlossaire::pushButtonBack_clicked()
{if (!QDir(m_curPath).exists())
    {G_pCApp->CouCou( tr("Le Glossaire est inutile et ne sert à rien pour la rubrique : ") + GetCurrentRubName());
     return;
    }
 if (m_curPath == m_DefaultMaskDirectory)
    { G_pCApp->CouCou( tr("Vous ne pouvez pas remonter d'un niveau car vous êtes déjà à la racine du répertoire de la rubrique : ") + GetCurrentRubName());
      return;
    }
 GetMasquesList(m_ListView_Glossaire, m_curPath+"/..");
}

//------------------------------- pushButtonHome_clicked --------------------------------------
void C_FormGlossaire::pushButtonHome_clicked()
{QString rubName = GetCurrentRubName();
 if (!QDir(m_curPath).exists())
    {G_pCApp->CouCou( tr("Le Glossaire est inutile et ne sert à rien pour la rubrique : ") + rubName);
     return;
    }
 if (rubName.length()) GetMasquesList(m_ListView_Glossaire, G_pCApp->m_PathGlossaire + rubName);  // ne pas utiliser pCMDI_Generic->GetRubName()
}
//------------------------------- ListView_Glossaire_doubleClicked ----------------------------------------
QString C_FormGlossaire::GetCurrentRubName()
{if (m_DefaultMaskDirectory.endsWith("Vigie")) return QString("Vigie");
 CMDI_Generic *pCMDI_Generic =  G_pCApp->GetCurrentRubrique();
 if (pCMDI_Generic) return pCMDI_Generic->m_RubName;
 return QString::null;
}

//------------------------------- ListView_Glossaire_doubleClicked ----------------------------------------
void C_FormGlossaire::ListView_Glossaire_doubleClicked( QListViewItem *qlistViewItem )
{if (!QDir(m_curPath).exists())
    {G_pCApp->CouCou( tr("Le Glossaire est inutile et ne sert à rien pour la rubrique : ") + GetCurrentRubName());
     return;
    }
 if (qlistViewItem==0)                                          return;
 if (qlistViewItem->text(0)=="..")                              return;
 QString fileName = m_curPath+"/"+qlistViewItem->text(0);
 if (QFileInfo (fileName ).isDir())                             return;
 m_ListView_Glossaire->disconnect( SIGNAL(doubleClicked(QListViewItem *)) );
 if (  !QFile::exists( fileName  ))                             return;
 int noInquire =  0;
 int type      = G_pCApp->detectTypeRubFromDoc("", fileName, &noInquire);
 if (type==0) type = PathGlossaireToTyp();
 //emit Sign_FileSelected(fileName, type );
 G_pCApp->m_pDrTux->OnGlossaireFileClicked(fileName, type, noInquire );
 connect( m_ListView_Glossaire,         SIGNAL( doubleClicked(QListViewItem *) ), this,  SLOT(ListView_Glossaire_doubleClicked (QListViewItem *)) );
}

//------------------------------- ListView_Glossaire_returnPressed  ------------------------------------------
void C_FormGlossaire::ListView_Glossaire_returnPressed( QListViewItem *qlistViewItem )
{ListView_Glossaire_contextMenuRequested(qlistViewItem,QPoint(0,0),0);
}

//------------------------------- ListView_Glossaire_selectionChanged  --------------------------------------------------------
void C_FormGlossaire::ListView_Glossaire_selectionChanged( QListViewItem *qlistViewItem )
{if (qlistViewItem<=0) return;
 if (m_pLastQListViewItem==qlistViewItem) return;
 if (qlistViewItem->text(1)=="F")
    {m_ListView_Glossaire->disconnect( SIGNAL(selectionChanged(QListViewItem *)) );
     SetTextEditWithData( qlistViewItem->text(0) , m_textEditView);
     emit Sign_FileChanged(m_curPath + "/" + qlistViewItem->text(0), PathGlossaireToTyp());
     QTimer::singleShot( 200, this, SLOT(Slot_RestaureConnection()) );
    }
 m_pLastQListViewItem=qlistViewItem;
}

//------------------------------- pushButtonEdit_clicked  --------------------------------------------------------
void C_FormGlossaire::pushButtonEdit_clicked()
{if (!QDir(m_curPath).exists())
    {G_pCApp->CouCou( tr("Le Glossaire est inutile et ne sert à rien pour la rubrique : ") + GetCurrentRubName());
     return;
    }
 QListViewItem * qlistViewItem = m_ListView_Glossaire->currentItem ();
 if (qlistViewItem<=0) return;
 emit Sign_FileToEdit(m_curPath + "/" + qlistViewItem->text(0) ,"Modifier", PathGlossaireToTyp());
}


//------------------------------- ListView_Glossaire_contextMenuRequested  --------------------------------------------------------
void C_FormGlossaire::ListView_Glossaire_contextMenuRequested( QListViewItem *pQListViewItem, const QPoint &, int )
{if (!QDir(m_curPath).exists())
    {G_pCApp->CouCou( tr("Le Glossaire est inutile et ne sert à rien pour la rubrique : ") + GetCurrentRubName());
     return;
    }
 if (pQListViewItem !=0 && pQListViewItem->text(1)=="F")    DoGlossairePopupMenu (m_curPath + "/" + pQListViewItem->text(0));
 else
     {ThemePopup *pThemePopup = new ThemePopup(0, "MyCustomPopupMenu" );
      if (pThemePopup==0)       return ;
      //....................... que certains repertoires autorises à creer un nouveau document ........................
      //                        (ceux des rubriques possédant un repertoire glossaire à leur nom)
      if (G_pCApp->IsThisDroitExist(G_pCApp->m_Droits,"glc") && QDir(m_curPath).exists() )
         {if (pQListViewItem !=0 && pQListViewItem->text(0).length())
             {pThemePopup->insertItem(Theme::getIconSet("16x16/EnterInRep.png"), tr("Entrer dans ce réperto&ire        ")+"   \t I",          this, SLOT( ListView_Glossaire_clicked()));
             }
          pThemePopup->insertItem(Theme::getIconSet("16x16/NewDoc.png"), tr("Cré&er et éditer un nouveau modèle") +"  \t E",          this, SLOT( Slot_PopupActionNewModele()));
          pThemePopup->insertSeparator ( ) ;
          pThemePopup->insertItem(Theme::getIconSet("16x16/NewDir.png"), tr("&Nouveau répertoire               ")+"   \t N",          this, SLOT( pushButtonNewDir_clicked()));
          if (pQListViewItem !=0 && pQListViewItem->text(0).length() )
             {if (pQListViewItem->text(0)[0]!='.')
                 { pThemePopup->insertItem(Theme::getIconSet("16x16/rename.png"), tr("&Renommer ce répertoire           ")+"   \t R",          this, SLOT( Slot_PopupActionRenameDir()));
                   pThemePopup->insertItem(Theme::getIconSet("16x16/DelDir.png"), tr("&Détruire ce répertoire           ")+"   \t D",          this, SLOT( pushButtonDelDir_clicked()));
                 }
             }
          //................. connecter le menu .................................................................
          connect( pThemePopup, SIGNAL( activated( int)),     this,        SLOT(OnMenuActivated( int))    );
          connect( pThemePopup, SIGNAL( highlighted ( int)),  this,        SLOT(OnMenuHighlighted( int))  );
          connect( pThemePopup, SIGNAL( aboutToHide ()),      this,        SLOT(OnMenuAboutToHide ())     );
          m_pThemePopup     = pThemePopup;
          m_PopupFilePath   = m_curPath;
          pThemePopup->exec(QCursor::pos());
         }
       else
         {G_pCApp->CouCou( tr("Cette action est sans résultat car le Glossaire est inutile et ne sert à rien pour cette rubrique"));
         }
       delete  pThemePopup;
      //........... parano mais ça se soigne ................
      m_pThemePopup   = 0;
      m_PopupFilePath = "";
      m_PopupItem     = "";
     }
}

//----------------------------------------- DoGlossairePopupMenu -----------------------------------------------------------------
// ACTION: recherche lors de l'appuis du bouton droit si il y a une zone de liste (bleue #0000ff)
//         et auquel cas rajoute les items de cette liste au popup menu classique.
ThemePopup * C_FormGlossaire::DoGlossairePopupMenu ( QString filepath)
{ThemePopup *pThemePopup = new ThemePopup(0, "MyCustomPopupMenu" );
 if (pThemePopup==0)    return 0;

 MyEditText *pMyEditText = G_pCApp->m_pDrTux->currentEditor();
 if (pMyEditText)
    {if (pMyEditText->isReadOnly())  // l'éditeur en lecture seulement
        {
          if ( (pMyEditText->GetTypeRubrique()==TYP_OBSERVATION  &&  G_pCApp->IsThisDroitExist(G_pCApp->m_Droits, "obc")) ||
               (pMyEditText->GetTypeRubrique()==TYP_PRESCRIPTION &&  G_pCApp->IsThisDroitExist(G_pCApp->m_Droits, "orc")) ||
               (pMyEditText->GetTypeRubrique()==TYP_PRESCRIPTION &&  G_pCApp->IsThisDroitExist(G_pCApp->m_Droits, "doc"))
             )
             { pThemePopup->insertItem(Theme::getIconSet("NewDoc.png"),  tr("Créer un n&ouveau document avec ce modèle    ")  +"       \t O",    this, SLOT( Slot_PopupActionNewDoc()));
             }
          else
             {pThemePopup->insertItem( tr(" Droits insuffisants pour créer un document avec ce modèle "),          this, SLOT( Slot_PopupActionDumy()));
             }
        }
     else
        { pThemePopup->insertItem(Theme::getIconSet("NewDoc.png"),         tr("Créer une n&ouvelle page avec ce modèle    ")  +"       \t O",          this, SLOT( Slot_PopupActionNewDoc()));
          pThemePopup->insertItem(Theme::getIconSet("NewDocPlusCurs.png"), tr("&Ajouter le contenu de ce modèle au curseur")  +"       \t A",          this, SLOT( Slot_PopupActionAddDoc()));
          pThemePopup->insertItem(Theme::getIconSet("NewDocPlus.png"),     tr("&Tout remplacer par ce modèle")                +"       \t T",          this, SLOT( Slot_PopupActionReplaceDoc()));
        }
    }
 if (G_pCApp->IsThisDroitExist(G_pCApp->m_Droits,"glc"))
    { pThemePopup->insertSeparator ( ) ;
      //....................... que certains repertoires autorises à creer un nouveau document (ceux des rubriques) ........................
      int typ = G_pCApp->PathDocumentToType( m_curPath  );
      if (typ == TYP_DOCUMENTS || typ == CMDI_Prescription::S_GetType() || typ == TYP_OBSERVATION)
         {pThemePopup->insertItem(Theme::getIconSet("16x16/NewDoc.png"), tr("Cré&er et éditer un nouveau modèle   ")+"       \t E",          this, SLOT( Slot_PopupActionNewModele()));
         }
      pThemePopup->insertItem(Theme::getIconSet("16x16/edit.png"),    tr("&Modifier et éditer ce modèle            ")+"       \t M",          this, SLOT( Slot_PopupActionEditDoc()));
      pThemePopup->insertItem(Theme::getIconSet("16x16/rename.png"),  tr("&Renommer ce modèle                      ")+"       \t R",          this, SLOT( Slot_PopupActionRenameDoc()));
      pThemePopup->insertItem(Theme::getIconSet("16x16/DelDoc.png"),  tr("&Détruire ce modèle                      ")+"       \t D",          this, SLOT( Slot_PopupActionDeleteDoc()));
      pThemePopup->insertSeparator ( ) ;
      pThemePopup->insertItem(Theme::getIconSet("16x16/default.png"), tr("&Placer ce modèle comme celui par défaut ")+"       \t N",          this, SLOT( Slot_pushDefault_clicked()));
      pThemePopup->insertSeparator ( ) ;
      pThemePopup->insertItem(Theme::getIconSet("16x16/NewDir.png"),  tr("&Nouveau répertoire                      ")+"       \t N",          this, SLOT( pushButtonNewDir_clicked()));
    }
 //................. connecter le menu .................................................................
 connect( pThemePopup, SIGNAL( activated( int)),     this,        SLOT(OnMenuActivated( int))  );
 connect( pThemePopup, SIGNAL( highlighted ( int)),  this,        SLOT(OnMenuHighlighted( int))  );
 connect( pThemePopup, SIGNAL( aboutToHide ()),      this,        SLOT(OnMenuAboutToHide ())  );
 m_pThemePopup     = pThemePopup;
 m_PopupFilePath   = filepath;
 pThemePopup->setActiveItem (0);
 pThemePopup->exec(QCursor::pos());
 delete  pThemePopup;
 //........... parano mais ça se soigne ................
 m_pThemePopup   = 0;
 m_PopupFilePath = "";
 m_PopupItem     = "";
 return  pThemePopup;
}

//------------------------------- Slot_PopupActionDumy  --------------------------------------------------------
void C_FormGlossaire::Slot_PopupActionDumy()
{ 
}

//------------------------------- Slot_pushDefault_clicked  --------------------------------------------------------
void C_FormGlossaire::Slot_pushDefault_clicked()
{ QString rubName = GetCurrentRubName(); 
  if (rubName.length()==0) return;
  QString value = m_PopupFilePath;
  if (value.startsWith(G_pCApp->m_PathGlossaire))
     {value = QString("$Glossaire/") + value.mid(G_pCApp->m_PathGlossaire.length());
     }
  CGestIni::Param_WriteParam(&G_pCApp->m_DrTuxParam, rubName , "Defaut", value) ;
  CGestIni::Param_UpdateToDisk(G_pCApp->m_PathDrTuxIni, G_pCApp->m_DrTuxParam);
}

//------------------------------- Slot_PopupActionNewModele  --------------------------------------------------------
void C_FormGlossaire::Slot_PopupActionNewModele()
{QString   path    =  m_curPath ;
 QString    ext    = ".html";
 QString  filtre   = tr( " Fichiers HTML (*.htm *.html);;  Tous les fichiers (*)");
 //.................. selectionner le fichier .......................................
 QFileDialog fd (path,  filtre,  this, "SaveDlg" ) ;
 fd.setCaption(tr("Saisissez le nom du document à créer"));
 fd.setMode(QFileDialog::AnyFile );    // selection multiple
 if ( fd.exec() != QDialog::Accepted )           return;
 QString fn = fd.selectedFile();
 if ( fn.isEmpty() )                             return;
 fn         = QFileInfo(fn).dirPath() + "/" + QFileInfo(fn).baseName() + ext;
 QFile file( fn);
 if (QDir().exists ( fn ) )
    {if ( QMessageBox::warning( this,       (" Document existant"),
                                             tr ( "ATTENTION :  Ce fichier : ")   +
                                             QFileInfo(fn).fileName() +
                                             tr(  "\r\n Existe déjà.\r\n"
                                                  " Faut il créer le nouveau document avec ce non,\r\n et écraser celui existant ?") ,
                                             tr("&Créer et écraser"), tr("Annu&ler"), 0,
                                             1, 1 ) == 1)
          {return;
          }
    }
 else
    {
    }
 if ( !file.open( IO_WriteOnly ) )               return;
 QString txt = "<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body style=\"font-size:10pt;font-family:Sans\"></body></html>";
 QTextStream ts( &file );
 CHtmlTools::QRichTextToHtml(txt);
 ts << txt;
 m_PopupFilePath = fn;
 emit Sign_FileToEdit(fn,  tr("Modifier"), PathGlossaireToTyp());
 GetMasquesList(m_ListView_Glossaire, m_curPath);
}

//------------------------------- Slot_PopupActionNewDoc  --------------------------------------------------------
void C_FormGlossaire::Slot_PopupActionNewDoc()
{emit Sign_FileToEdit(m_PopupFilePath,  tr("Créer"), PathGlossaireToTyp());
}
//------------------------------- Slot_PopupActionAddDoc  --------------------------------------------------------
void C_FormGlossaire::Slot_PopupActionAddDoc()
{emit Sign_FileToEdit(m_PopupFilePath,  tr("Ajouter"), PathGlossaireToTyp());
}
//------------------------------- Slot_PopupActionReplaceDoc  --------------------------------------------------------
void C_FormGlossaire::Slot_PopupActionReplaceDoc()
{emit Sign_FileToEdit(m_PopupFilePath,  tr("Tout"), PathGlossaireToTyp());
}
//------------------------------- Slot_PopupActionEditDoc  --------------------------------------------------------
void C_FormGlossaire::Slot_PopupActionEditDoc()
{emit Sign_FileToEdit(m_PopupFilePath,  tr("Modifier"), PathGlossaireToTyp());
}
//------------------------------- Slot_PopupActionRenameDoc  --------------------------------------------------------
void C_FormGlossaire::Slot_PopupActionRenameDoc()
{emit Sign_FileToEdit(m_PopupFilePath,  tr("Renommer"), PathGlossaireToTyp());
}
//------------------------------- Slot_PopupActionDeleteDoc  --------------------------------------------------------
void C_FormGlossaire::Slot_PopupActionDeleteDoc()
{emit Sign_FileToEdit(m_PopupFilePath,  tr("Détruire"), PathGlossaireToTyp());
}

//------------------------------- OnMenuActivated  --------------------------------------------------------
void C_FormGlossaire::OnMenuActivated( int id)
{if (id != -1 && m_pThemePopup)
    {m_PopupItem   = m_pThemePopup->text(id);
    }
}

//------------------------------- OnMenuHighlighted  --------------------------------------------------------
void C_FormGlossaire::OnMenuHighlighted( int id)
{if (id != -1 && m_pThemePopup)
    {m_PopupItem     = m_pThemePopup->text(id);
    }
}

//------------------------------- OnMenuAboutToHide  --------------------------------------------------------
void C_FormGlossaire::OnMenuAboutToHide( )
{
}

//------------------------------------- SetListTyp -------------------------------------------
int C_FormGlossaire::SetListTyp( int typ , const QString &path, QListView *pQlistView /* =0 */ )
{if (m_RubType == typ) return typ;   // pas la peine de changer car on y est deja et peut etre positionn�dans un sous dossier
 int   old_typ           = m_RubType;
 m_RubType               = typ;
 m_curPath               = path;
 m_DefaultMaskDirectory  = path;
 if (pQlistView) GetMasquesList(pQlistView , m_curPath);
 return old_typ;
}

//------------------------------------- GetMasquesObsList ------------------------------------
/*! \brief non documenté.
*/
long C_FormGlossaire::GetMasquesList(  QListView *pQlistView , const QString &path_in)
{   int        i         = 0;
    int   isHome         = 0;
    m_pLastQListViewItem = 0;
    //................ charger la BlackList ........................................
    QString black_str      = CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathGlossaire+"/ignoreList.txt");
    black_str              = black_str.replace('\r',"").stripWhiteSpace();
    QStringList black_list = QStringList::split('\n',black_str);
    //.............. ne pas reculer au delas du permis ..............................
    QString path = path_in;
    if ( !path.startsWith(m_DefaultMaskDirectory))
       { path = m_DefaultMaskDirectory;         // le chemin remonte plus haut que le chemin par defaut de la rubrique
         G_pCApp->CouCou( tr("Vous êtes déjà à la racine du répertoire de cette rubrique"));
       }
    else
       {QString test = path;                                                              // tester si pas de la forme /Glossaire/RubriqueName/../
        test.replace ('\\','/');
        if (test.endsWith("/")) test = test.left(test.length()-1);
        test         = test.remove(m_DefaultMaskDirectory);
        test         = test.remove("/..");
        if (test.length()==0) path = m_DefaultMaskDirectory;
       }
    //................ allons y  .....................................................
    m_QDir.setPath ( path );
    m_QDir.convertToAbs();
    m_curPath = m_QDir.path();
    pQlistView->clear();
    m_QDir.setFilter( QDir::All | QDir::NoSymLinks );

    if (m_curPath == m_DefaultMaskDirectory) {isHome = 1;}

    //............ exploration de la liste ......................
    const QFileInfoList * pQFileInfoList = m_QDir.entryInfoList();
    if (pQFileInfoList==0) return 0;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo     *fi;
    C_DragQListViewItem *element;
    while ( (fi = it.current()) != 0 )
          {element = 0;
           QString fname = fi->fileName();
           QString ext   = QFileInfo(fname).extension().lower();
           if      (fname == "..")
                   {if (isHome==0)
                       {element = new C_DragQListViewItem( pQlistView);
                        element->setText(0,fname);
                        element->setText(1,"B");
                        element->setPixmap ( 0, Theme::getIcon("22x22/previous.png")   );
                        m_curPath = m_QDir.absPath ();
                       }
                   }
           else if (fi->isDir() && fname[0] != '.' && ext.length()==0 && CGestIni::isInBlackList(black_list, fname)==0 )
                   {element = new C_DragQListViewItem( pQlistView );
                    element->setText(0,fname);
                    element->setText(1,"D");
                    element->setPixmap ( 0, Theme::getIconListFolder().pixmap(QIconSet::Automatic,TRUE,QIconSet::On) );
                   }
           else if (fi->isFile() && fname[0] != '.' && CGestIni::isInBlackList(black_list, fname)==0)
                   {if (ext.length())
                       {if (ext=="html"||ext=="htm"||ext=="txt"||ext=="xml"||ext=="rtf")
                           {element = new C_DragQListViewItem( pQlistView );
                            element->setText(0,fname);
                            element->setText(1,"F");
                            element->setPixmap ( 0, Theme::getIconListHTML().pixmap(QIconSet::Automatic,TRUE,QIconSet::On)   );
                           }
                        else if (ext=="ord")
                           {element = new C_DragQListViewItem( pQlistView );
                            element->setText(0,fname);
                            element->setText(1,"F");
                            element->setPixmap ( 0, Theme::getIcon("16x16/type_Prescription.png")   );
                           }
                       }
                   }

           if (element) {pQlistView->insertItem ( element ); element->setDragEnabled ( TRUE );}
           ++it;
           i++;
          }
    pQlistView->setSorting (1);
    textLabel_PathGlossaire->setText( m_curPath );
    return 1;
}

//-------------------------------- PathGlossaireToTyp ----------------------------------------------------
/*! \brief Retourne le type: GLO_TYP_RUB_OBSERV ou GLO_TYP_RUB_PRESCR selon le path courant en cours utile pour permettre d'envoyer un document as le bon editeur de  texte en foction de sa localisation d'origine.
*/
int C_FormGlossaire::PathGlossaireToTyp()
{int typ = G_pCApp->PathDocumentToType( m_curPath  );
 if (typ) return typ;
 else     return m_RubType;
}

//------------------------------------- SetTextEditWithData -----------------------------------------------
/*! \brief Affiche le contenu du document dans la zone de previsualisation */
void C_FormGlossaire::SetTextEditWithData(  QString file , CW_EditText *pQTextEdit)
{QApplication::setOverrideCursor ( QCursor(Qt::WaitCursor) );
 QString stringDST, str;
 QString qspath =  m_curPath + "/" + file;
 CGestIni::Param_UpdateFromDisk(qspath, str);
 QString ext = CGenTools::GetExtension(qspath).lower();
 if (ext == ".rtf" )
    {CHtmlTools::RtfToQtextEdit_HTML(str, str.length(), stringDST);
    }
 else if ( ext == ".ord" )
    {CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData(str, &stringDST, 0);    // y isoler et recuperer les données textuelle
    }
 else
    {stringDST = str;
    }
 //disconnect( m_ListView_Glossaire,      SIGNAL(Sign_ItemClicked( QListViewItem *)), this, SLOT( ListView_Glossaire_clicked(QListViewItem *) ) );
 //.............. document courant .....................................................
 CRubRecord *pCRubRecord     = 0;
 int         rub_type        = 0;
 QString     rubName         = "";
 G_pCApp->m_pDrTux->GetCurrentRubrique(&rubName,&rub_type, &pCRubRecord);   // operation couteuse qu'il vaut mieux evierer

 //............. creer la liste des documents �l'affichage ...........................
 DOCUMENT_DISPLAY_MAP  currentDocDisplayMap;
 QString user   = G_pCApp->m_pDrTux->MapActiveID_Doc(currentDocDisplayMap);   // retour = utilisateur le plus probable
 //m_ListView_Glossaire->setAcceptDrops( FALSE );
 QString userPk = G_pCApp->m_pCMoteurBase->GetUserPrimKey(user);
 G_pCApp->m_VAR_MAP.insert("$SCRIPT_STATUS","$PREVISUALISATION");
 //m_ListView_Glossaire->dragClose();
 //C_DragQListViewItem *pC_DragQListViewItem = (C_DragQListViewItem*) m_ListView_Glossaire->currentItem ();
 //if (pC_DragQListViewItem){pC_DragQListViewItem->setDragEnabled(FALSE);}
 CDevilCrucible *pfusion = new CDevilCrucible(&stringDST                     ,          // 0 texte du document �fusionner
                                               G_pCApp->m_pCMoteurBase       ,          // 1 moteur de base de donn�s (faut bien acc�er aux fonctions)
                                               G_pCApp->m_pCMedicaBase       ,          // 2 moteur de base de données medicamenteuses(faut bien accéder aux fonctions)
                                              &G_pCApp->m_pDrTux->m_RubList  ,          // 3 liste des documents composant le dossier patient
                                               G_pCApp->m_ID_Doss            ,          // 4 il faut les renseignements sur le patient
                                              &currentDocDisplayMap          ,          // 5 liste des rubriques courantes (affich�s)
                                               userPk                        ,          // 6 utilisateur responsable du document
                                               pCRubRecord                   ,          // 7 pointeur sur la rubrique en cours de modif (si c'est une rubrique) zero sinon
                                               &G_pCApp->m_VAR_MAP           ,          // 8 pointeur sur la liste des variables (zero par defaut)
                                               CDevilCrucible::JustForWatch             // 9 defaut = CDevilCrucible::DoAll indique que c'est juste pour la visualisation ou to faire
                                             );
 if (  pfusion )
    {pfusion-> DoMixture();
     delete pfusion;
    }
 pQTextEdit->setText(stringDST);
 G_pCApp->m_VAR_MAP.insert("$SCRIPT_STATUS","$PREVISUALISATION_END");
 //m_ListView_Glossaire->setAcceptDrops( TRUE );
 //if (pC_DragQListViewItem) pC_DragQListViewItem->setDragEnabled(TRUE);
 //connect( m_ListView_Glossaire,      SIGNAL(Sign_ItemClicked( QListViewItem *)), this, SLOT( ListView_Glossaire_clicked(QListViewItem *) ) );

 QApplication::restoreOverrideCursor();
}

//------------------------------- OnGlossaireRubriqueActived ----------------------------------------------
/*! \brief Reponse au signal:  SIGNAL( windowActivated ( QWidget *  )) provenant du QWorkspace  gestionnaire des fenetres MDI. Ce signal est emis par QWorkspace lorsqu'une fenetre est activée. en reponse nous activons et positionnons le bon chemin et path racine dans la liste.
 *  \param QWidget * pQwdgRub;  fenetre CMDI qui vient d'être activée.
*/
void C_FormGlossaire::OnGlossaireRubriqueActived(QWidget *pQwdgRub)
{if (pQwdgRub==0)          return;
 CMDI_Generic *pCMDI_Generic = (CMDI_Generic*)pQwdgRub;
 if (m_DefaultMaskDirectory != pCMDI_Generic->GetDefaultMaskDirectory())
    {SetCurrentPath(pCMDI_Generic->GetCurrentMaskDirectory(), pCMDI_Generic->GetDefaultMaskDirectory());
    }
}

//------------------------------------- On_Sign_MouseEnter -----------------------------------------------
/*! \brief Slot recevant la notification d'un connecte que la souris entre dans ce connecté  */
void C_FormGlossaire::On_Sign_MouseEnter(QString &pathDocuments, QString &defaultMaskDirectory)
{//....................... si on entre dans la vigie et glossaire non positionné dessus, le positionner ...............................
 bool forcePath=FALSE;
 if (defaultMaskDirectory[0]=='#')
    {forcePath            = TRUE;
     defaultMaskDirectory = defaultMaskDirectory.mid(1);
    }
 QString vigie_DefMaskDirectory = G_pCApp->m_PathGlossaire+"Vigie";
 if (defaultMaskDirectory   == vigie_DefMaskDirectory )
    { if (m_DefaultMaskDirectory != defaultMaskDirectory || forcePath ) SetCurrentPath(pathDocuments, defaultMaskDirectory);
      return;
    }
 //................... si on entre dans autre chose que la vigie et que l'ancien chemin etait celui de la vigie .......................
 //                    alors se positionner sur la rubrique courante
 if (m_DefaultMaskDirectory == vigie_DefMaskDirectory)
    {CMDI_Generic *pCMDI_Generic = G_pCApp->GetCurrentRubrique();
     if (pCMDI_Generic)
        {SetCurrentPath(pCMDI_Generic->GetCurrentMaskDirectory(), pCMDI_Generic->GetDefaultMaskDirectory());
        }
     else
        {SetCurrentPath(pathDocuments, defaultMaskDirectory);  // si changement de rubrique on reactive le glossaire
        }
    }
}

//------------------------------------- On_Sign_MouseLeave -----------------------------------------------
/*! \brief Slot recevant la notification d'un connecte que la souris sort de ce connecté */
void C_FormGlossaire::On_Sign_MouseLeave(QString &pathDocuments, QString &defaultMaskDirectory)
{if (m_DefaultMaskDirectory == defaultMaskDirectory) GetCurrentPath(pathDocuments, defaultMaskDirectory);
}

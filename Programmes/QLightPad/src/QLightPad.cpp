
/************************** QLightPad.cpp *********************************
 *   Copyright (C) 2004 by Sevin Roland                                    *
 *   upsis@club-internet.fr                                                *
 *                                                                         *
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

#include <qtextedit.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qapplication.h>
#include <qfontdatabase.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qfileinfo.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qprinter.h>
#include <qpaintdevicemetrics.h>
#include <qsimplerichtext.h>
#include <qcolordialog.h>
#include <qpainter.h>
#include <qmessagebox.h>
#include <qclipboard.h>
#include <qlabel.h>
#include <qtextbrowser.h>
#include <qcursor.h>
#include <qptrlist.h>
#include <qtextcodec.h>
#include <qiconset.h>
#include <qeventloop.h>

#include "QLightPad.h"
#include "MyEditText.h"
#include "../../MedinTuxTools/CPreview.h"
#include "CMyTabWidget.h"

#include "ui/DLG_InsertLink.h"
#include "ui/DLG_InsertAncre.h"
#include "ui/DLG_InsertEmail.h"
#include "ui/DLG_BodyAttributs.h"
#include "ui/DLG_SaveRequest.h"
#include "ui/DlgListOff_Listes.h"

#include "../../drtux/src/C_DlgListFieldMngr.h"

#include "../../MedinTuxTools/CHtmlTools.h"
#include "../../MedinTuxTools/CGenTools.h"
#include "../../MedinTuxTools/CGestIni.h"
#include "../../MedinTuxTools/Theme.h"
#include "../../MedinTuxTools/ThemePopup.h"

// "Si Dieu existe, j'espère qu'il a une bonne excuse." -+- W. Allen -+-
extern QString             GlobalPathAppli;
static char NUM_VERSION[]     = "==##@@==2.16.000==@@##==";
//------------------------------------------------------- QLightPad -------------------------------------
QLightPad::QLightPad( QWidget *parent, const char *name ,  int argc, char ** argv)
    : QMainWindow( parent, name )
{   m_EditTabMenuSelected     = 0;
    m_Dlg_A_Propos            = 0;
    m_AppMustQuit             = FALSE;
    m_AnchorSpaceToUnderscore = 1;
    m_NUM_VERSION             = NUM_VERSION;
    m_Apropos_Proc            = 0;
    setupFileActions();
    setupEditActions();
    setupToolsActions();
    setupHelpActions();
    setupTextActions();

    m_tabWidget = new CMyTabWidget( this );
    connect( m_tabWidget, SIGNAL( currentChanged( QWidget * ) ),
             this,        SLOT( editorChanged( QWidget * ) ) );
    connect( m_tabWidget, SIGNAL(Sig_RigthButton(int)),
             this,        SLOT( tabRightClicked(int ) ) );
    setCentralWidget( m_tabWidget );

    //.......................... determiner mode de démarrage avec ou sans gestionnaire de liste deroulantes .......
    m_PathGlossaire = "";
    m_PathDrTux     = "";
    m_docToLoadList.clear();

    if ( qApp->argc() == 1 )
       {
        //load( "example.html" );
       }
   else
       {int i;
        for ( i = 1; i < argc; ++i )
            {QString argStr;
             #if   defined(Q_WS_WIN)
                 argStr =  argv[ i ];
             #elif defined(Q_WS_X11)
                 if (CGestIni::IsUtf8(argv[ i ])) argStr =  QString::fromUtf8 (argv[ i ]);
                 else                             argStr =  argv[ i ];
             #elif defined(Q_WS_MAC)
                 if (CGestIni::IsUtf8(argv[ i ])) argStr =  QString::fromUtf8 (argv[ i ]);
                 else                             argStr =  argv[ i ];
             #endif
             if (argStr.left(11)=="$Glossaire=")
                {m_PathGlossaire = argStr.mid(11).replace("\\","/");
                 if (m_PathGlossaire.endsWith("/") ) m_PathGlossaire = m_PathGlossaire.left(m_PathGlossaire.length()-1);
                }
             else if (argStr.left(7)=="$DrTux=")
                {m_PathDrTux = argStr.mid(7).replace("\\","/");
                 if (m_PathDrTux.endsWith("/") ) m_PathDrTux = m_PathDrTux.left(m_PathDrTux.length()-1);
                }
            else if (argStr.left(12)=="$InsertList=")
                {QString st = argStr.mid(12);
                 MakeInsertFieldList(st, m_InsertFieldList,1);
                 st = st.replace("FusionListe.txt", "ActionListe.txt");
                 MakeInsertFieldList(st, m_InsertFieldList, 0);
                }
            }

        for (  i = 1; i < argc; ++i )
            {QString argStr;
             #if   defined(Q_WS_WIN)
                 argStr =  argv[ i ];
             #elif defined(Q_WS_X11)
                 if (CGestIni::IsUtf8(argv[ i ])) argStr =  QString::fromUtf8 (argv[ i ]);
                 else                             argStr =  argv[ i ];
             #elif defined(Q_WS_MAC)
                 if (CGestIni::IsUtf8(argv[ i ])) argStr =  QString::fromUtf8 (argv[ i ]);
                 else                             argStr =  argv[ i ];
             #endif

             if (argStr.left(1)!="$") {load( argStr ); m_docToLoadList<<argStr;}
            }
       }
  if (m_PathGlossaire.length())
     {setupDrTuxPopupActions();
      m_tabWidget->setCurrentPage ( 0 );
     }
  //......................... theme ................................................
 if (m_PathDrTux.length())  //  noter que m_PathDrTux n'a pas le / de fin
     {
     }
 m_PathTheme =  GlobalPathAppli + "../../Themes/Default/"; 	// valeur par défaut
 m_PathTheme = QDir::cleanDirPath(m_PathTheme) + "/";
 Theme::setPath(m_PathTheme);

  m_debug = 0;
  #if   defined(Q_WS_WIN)
     if (QFile::exists("C:/MedinTuxRo.txt"))                         m_debug = 1;
  #elif defined(Q_WS_X11)
     if (QFile::exists("/home/ro/MedinTuxRo.txt"))                   m_debug = 1;
  #elif defined(Q_WS_MAC)
     if (QFile::exists("/Utilisateurs/ro/Documents/MedinTuxRo.txt")) m_debug = 1;
  #endif
  
  setIcon( QPixmap (GlobalPathAppli + "QLightPadSys/QLightPad.png"));
}
//------------------------------------------------------- QLightPad -------------------------------------
QLightPad::~QLightPad()
{Slot_SauverLesMeubles();
}
//------------------------------------------------------- tabRightClicked -------------------------------------
void QLightPad::tabRightClicked(int id)
{MyEditText *edit = (MyEditText*) m_tabWidget->page( id );
 if (edit==0) return;
 createTabPopup(edit);
}

//------------------------------------ createTabPopup -------------------------------------------------------------
void QLightPad::createTabPopup(MyEditText *edit)
{QPopupMenu *pQPopupMenu = new QPopupMenu(this, "MyPopupMenu" );
 m_EditTabMenuSelected = edit;

 pQPopupMenu->insertItem(QPixmap(GlobalPathAppli +"QLightPadSys/panel-force-quit.png" ),
                         tr("Fermer "),                  this, SLOT( MenuActionFermer()),                  CTRL+Key_K );
 pQPopupMenu->insertItem(QPixmap(GlobalPathAppli +"QLightPadSys/CloseAllOther.png" ),
                         tr("Fermer tous les autres "),  this, SLOT( MenuActionFermerTousLesAutres()),     0 );
 pQPopupMenu->exec(QCursor::pos());
 delete pQPopupMenu;
}

//------------------------------------ MenuActionFermer --------------------------------------------------------
void QLightPad::MenuActionFermer()
{closeEditor(m_EditTabMenuSelected);
 m_EditTabMenuSelected = 0;
}

//------------------------------------ MenuActionFermerTousLesAutres ----------------------------------------------------
void QLightPad::MenuActionFermerTousLesAutres()
{closeAll(CSaveRequest::SaveCurrentRequest, m_EditTabMenuSelected);
 m_EditTabMenuSelected = 0;
}

//------------------------------------------------------- fileExit -------------------------------------
void QLightPad::fileExit()
{ if (closeAll() != CSaveRequest::AbortRequest) close (FALSE );   // qApp->quit();
}

//---------------------------------------------- closeEvent --------------------------------------------------------------------------------
void QLightPad::closeEvent( QCloseEvent* ce )
{if (closeAll()  == CSaveRequest::AbortRequest)   ce->ignore();
 else if (m_Dlg_A_Propos)
     {m_AppMustQuit = TRUE;
      m_Dlg_A_Propos->close(); //CoolPopupStop(m_pCCoolPopup);
     }
     //if (m_Dlg_A_Propos) delete m_Dlg_A_Propos;
 else{
      ce->accept();
     }
}

//------------------------------------------------------- closeAll -------------------------------------
int QLightPad::closeAll(int mode /*=CSaveRequest::SaveCurrentRequest*/, MyEditText* editToNotClose /*= 0*/)
{QPtrList <MyEditText> editorList;
 //.............. relever liste de tous les éditeurs présents ........................
 int nb = m_tabWidget->count();
 for (int i=0; i<nb; ++i)  editorList.append((MyEditText*) m_tabWidget->page(i));

 //.............. les effacer tous sauf celui indiqué ........................
 for (int u=0; u < nb; ++u)
     {MyEditText *edit = editorList.at(u);
      if (edit != editToNotClose) mode = closeEditor(edit, mode);
     }
 return mode;
}

//------------------------------------------------------- setModifiedAll -------------------------------------
void QLightPad::setModifiedAll(bool mode)
{int nb = m_tabWidget->count();
 for (int i=0; i<nb; ++i)  ((MyEditText*) m_tabWidget->page(i))->setModified(mode);
}

//------------------------------------------------------- setupFileActions -------------------------------------
void QLightPad::setupFileActions()
{
    QToolBar *tb = new QToolBar( this );
    tb->setLabel( tr("Outils Fichiers") );
    QPopupMenu *menu = new QPopupMenu( this );
    menuBar()->insertItem( tr( "&Fichiers" ), menu );

    QAction *a;
    a = new QAction( tr( "Nouveau document" ), QPixmap(GlobalPathAppli +"QLightPadSys/filenew.png" ), tr( "&Nouveau document..." ), CTRL + Key_N, this, "fileNew" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileNew() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Ouvrir" ), QPixmap(GlobalPathAppli +"QLightPadSys/fileopen.png" ), tr( "&Ouvrir..." ), CTRL + Key_O, this, "fileOpen" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileOpen() ) );
    a->addTo( tb );
    a->addTo( menu );
    menu->insertSeparator();
    a = new QAction( tr( "Enregistrer" ), QPixmap(GlobalPathAppli +"QLightPadSys/filesave.png" ), tr( "Enregi&strer..." ), CTRL + Key_S, this, "fileSave" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileSave() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Positionner l'entête meta" ), QPixmap(GlobalPathAppli +"QLightPadSys/ImageSaveMeta.png" ), tr( "Positionner l'entête meta..." ), CTRL + Key_S, this, "fileSaveMeta" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileSaveMeta() ) );
    a->addTo( tb );
    a->addTo( menu );

    a = new QAction( tr( "Enregistrer Sous" ), tr( "&Enregistrer Sous..." ), 0, this, "fileSaveAs" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileSaveAs() ) );
    a->addTo( menu );
    menu->insertSeparator();
    a = new QAction( tr( "Imprimer" ), QPixmap(GlobalPathAppli +"QLightPadSys/fileprint.png" ), tr( "Im&primer..." ), CTRL + Key_P, this, "filePrint" );
    connect( a, SIGNAL( activated() ), this, SLOT( filePrint() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Fermer" ), QPixmap(GlobalPathAppli +"QLightPadSys/panel-force-quit.png" ), tr( "&Fermer..." ), CTRL + Key_K, this, "fileClose" );
    //a = new QAction( tr( "Close" ), tr( "&Close" ), 0, this, "fileClose" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileClose() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Quitter" ), tr( "&Quitter" ), CTRL + Key_Q, this, "fileExit" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileExit() ) );
    a->addTo( menu );
}
//------------------------------------------------------- setupToolsActions -------------------------------------
void QLightPad::setupToolsActions()
{
    QToolBar *tb = new QToolBar( this );
    tb->setLabel( tr("Outils d'insertion" ));
    QPopupMenu *menu = new QPopupMenu( this );
    menuBar()->insertItem( tr( "&Outils" ), menu );
    QAction *a;
    a = new QAction( tr( "Insérer une image" ), QPixmap(GlobalPathAppli +"QLightPadSys/ic-flower3.png" ), tr( "&Insérer une image..." ), ALT + Key_I, this, "InsererImage" );
    connect( a, SIGNAL( activated() ), this, SLOT( InsererImage() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Insérer un lien" ), QPixmap(GlobalPathAppli +"QLightPadSys/Lien.png" ), tr( "Insérer un &lien..." ), CTRL + Key_L, this, "InsererLien" );
    connect( a, SIGNAL( activated() ), this, SLOT( InsererLien() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Insérer une une adresse Email" ), QPixmap(GlobalPathAppli +"QLightPadSys/Email.png" ), tr( "Insérer une adresse &Email..." ), CTRL + Key_M, this, "InsererEmail" );
    connect( a, SIGNAL( activated() ), this, SLOT( InsererEmail() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Insérer une ancre" ), QPixmap(GlobalPathAppli +"QLightPadSys/Anchor00.png" ), tr( "Insérer une &ancre..." ), CTRL + Key_H, this, "InsererAncre" );
    connect( a, SIGNAL( activated() ), this, SLOT( InsererAncre() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Insérer une ligne" ), QPixmap(GlobalPathAppli +"QLightPadSys/InsererLigne.png" ), tr( "Insérer une li&gne..." ), CTRL + Key_G, this, "InsererLigne" );
    connect( a, SIGNAL( activated() ), this, SLOT( InsererLigne() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Insérer une Liste" ), QPixmap(GlobalPathAppli +"QLightPadSys/InsererListe.png" ), tr( "Insérer une lis&te..." ), CTRL + Key_T, this, "InsererListe" );
    connect( a, SIGNAL( activated() ), this, SLOT( InsererListe() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Insérer des espaces insécables" ), QPixmap(GlobalPathAppli +"QLightPadSys/InsererInsecables.png" ), tr( "Insérer des espaces insécables" ), ALT + Key_S, this, "InsererInsecables" );
    connect( a, SIGNAL( activated() ), this, SLOT( InsererInsecables() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Retirer les espaces insécables" ), QPixmap(GlobalPathAppli +"QLightPadSys/RetirerInsecables.png" ), tr( "Retirer les espaces insécables" ), ALT + Key_R, this, "RetirerInsecables" );
    connect( a, SIGNAL( activated() ), this, SLOT( RetirerInsecables() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Insérer une Puce" ), QPixmap(GlobalPathAppli +"QLightPadSys/InsererPuce.png" ), tr( "Insérer une &Puce..." ), CTRL + Key_J, this, "InsererPuce" );
    connect( a, SIGNAL( activated() ), this, SLOT( InsererPuce() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Texte sélectionné en mode script" ), QPixmap(GlobalPathAppli +"QLightPadSys/CodePre.png" ), tr( "Texte sélectionné en mode script" ), 0, this, "CodePre" );
    connect( a, SIGNAL( activated() ), this, SLOT( CodePre() ) );
    a->addTo( tb );
    a->addTo( menu );
}

//------------------------------------------------------- setupEditActions -------------------------------------
void QLightPad::setupEditActions()
{
    QToolBar *tb = new QToolBar( this );
    tb->setLabel( tr("Outils d'édition") );
    QPopupMenu *menu = new QPopupMenu( this );
    menuBar()->insertItem( tr( "&Édition" ), menu );

    QAction *a;
    a = new QAction( tr( "Annuler" ), QPixmap(GlobalPathAppli +"QLightPadSys/undo.png" ), tr( "Annu&ler" ), CTRL + Key_Z, this, "editUndo" );
    connect( a, SIGNAL( activated() ), this, SLOT( editUndo() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Refaire" ), QPixmap(GlobalPathAppli +"QLightPadSys/redo.png" ), tr( "&Refaire" ), CTRL + Key_Y, this, "editRedo" );
    connect( a, SIGNAL( activated() ), this, SLOT( editRedo() ) );
    a->addTo( tb );
    a->addTo( menu );
    menu->insertSeparator();
    a = new QAction( tr( "Copier" ), QPixmap(GlobalPathAppli +"QLightPadSys/editcopy.png" ), tr( "&Copier" ), CTRL + Key_C, this, "editCopy" );
    connect( a, SIGNAL( activated() ), this, SLOT( editCopy() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Couper" ), QPixmap(GlobalPathAppli +"QLightPadSys/editcut.png" ), tr( "Cou&per" ), CTRL + Key_X, this, "editCut" );
    connect( a, SIGNAL( activated() ), this, SLOT( editCut() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Coller" ), QPixmap(GlobalPathAppli +"QLightPadSys/editpaste.png" ), tr( "&Coller" ), CTRL + Key_V, this, "editPaste" );
    connect( a, SIGNAL( activated() ), this, SLOT( editPaste() ) );
    a->addTo( tb );
    a->addTo( menu );
    menu->insertSeparator();
    a = new QAction( tr( "Paramètres par défaut du texte" ), QPixmap(GlobalPathAppli +"QLightPadSys/DocConfig.png" ), tr( "Paramètres par défaut du texte..." ), CTRL + Key_K, this, "ParametresParDefaut" );
    //a = new QAction( tr( "Paramètres par défaut du texte..." ), tr( "Paramètres par défaut du texte..." ), 0, this, "ParametresParDefaut" );
    connect( a, SIGNAL( activated() ), this, SLOT( ParametresParDefaut() ) );
    a->addTo( tb );
    a->addTo( menu );
    /*
    m_actionAutoRet = new QAction( tr( "Retour automatique sur la fenêtre" ), QPixmap::fromMimeSource( "AutoRet.png" ), tr( "Retour automatique sur la fenêtre..." ), CTRL + Key_K, this, "AutoRet" );
    connect( m_actionAutoRet, SIGNAL( activated() ), this, SLOT( setAutoRet() ) );
    m_actionAutoRet->addTo( tb );
    m_actionAutoRet->addTo( menu );
    m_actionAutoRet->setToggleAction( TRUE );
    */
}

//------------------------------------------------------- setupTextActions -------------------------------------
void QLightPad::setupTextActions()
{
    QToolBar *tb = new QToolBar( this );
    tb->setLabel( tr("Outils Aspect texte") );
    QPopupMenu *menu = new QPopupMenu( this );
    menuBar()->insertItem( tr( "F&ormat" ), menu );

    m_comboStyle = new QComboBox( FALSE, tb );
    m_comboStyle->insertItem( tr("Standard") );
    m_comboStyle->insertItem( tr("Bullet List (Disc)") );
    m_comboStyle->insertItem( tr("Bullet List (Circle)") );
    m_comboStyle->insertItem( tr("Bullet List (Square)") );
    m_comboStyle->insertItem( tr("Liste Ordonnée (Decimal)") );
    m_comboStyle->insertItem( tr("Liste Ordonnée (Alpha Minus)") );
    m_comboStyle->insertItem( tr("Liste Ordonnée (Alpha Majusc)") );
    connect( m_comboStyle, SIGNAL( activated( int ) ),
             this,         SLOT( textStyle( int ) ) );

    m_comboFont = new QComboBox( TRUE, tb );
    QFontDatabase db;
    m_comboFont->insertStringList( db.families() );
    connect( m_comboFont, SIGNAL( activated( const QString & ) ),
             this,        SLOT( textFamily( const QString & ) ) );
    m_comboFont->lineEdit()->setText( QApplication::font().family() );

    m_comboSize = new QComboBox( TRUE, tb );
    QValueList<int> sizes = db.standardSizes();
    QValueList<int>::Iterator it = sizes.begin();
    for ( ; it != sizes.end(); ++it )
        m_comboSize->insertItem( QString::number( *it ) );
    connect( m_comboSize, SIGNAL( activated( const QString & ) ),
             this,        SLOT( textSize( const QString & ) ) );
    m_comboSize->lineEdit()->setText( QString::number( QApplication::font().pointSize() ) );

    m_actionTextBold = new QAction( tr( "Gras" ), QPixmap(GlobalPathAppli +"QLightPadSys/textbold.png" ), tr( "&Gras" ), CTRL + Key_B, this, "textBold" );
    connect( m_actionTextBold, SIGNAL( activated() ), this, SLOT( textBold() ) );
    m_actionTextBold->addTo( tb );
    m_actionTextBold->addTo( menu );
    m_actionTextBold->setToggleAction( TRUE );
    m_actionTextItalic = new QAction( tr( "Italique" ), QPixmap(GlobalPathAppli +"QLightPadSys/textitalic.png" ), tr( "&Italique" ), CTRL + Key_I, this, "textItalic" );
    connect( m_actionTextItalic, SIGNAL( activated() ), this, SLOT( textItalic() ) );
    m_actionTextItalic->addTo( tb );
    m_actionTextItalic->addTo( menu );
    m_actionTextItalic->setToggleAction( TRUE );
    m_actionTextUnderline = new QAction( tr( "Souligné" ), QPixmap(GlobalPathAppli +"QLightPadSys/textunder.png" ), tr( "&Souligné" ), CTRL + Key_U, this, "textUnderline" );
    connect( m_actionTextUnderline, SIGNAL( activated() ), this, SLOT( textUnderline() ) );
    m_actionTextUnderline->addTo( tb );
    m_actionTextUnderline->addTo( menu );
    m_actionTextUnderline->setToggleAction( TRUE );
    menu->insertSeparator();

    QActionGroup *grp = new QActionGroup( this );
    grp->setExclusive( TRUE );
    connect( grp, SIGNAL( selected( QAction* ) ), this, SLOT( textAlign( QAction* ) ) );

    m_actionAlignLeft = new QAction( tr( "Gauche" ), QPixmap(GlobalPathAppli +"QLightPadSys/textleft.png" ), tr( "&Gauche" ), CTRL + Key_L, grp, "textLeft" );
    m_actionAlignLeft->addTo( tb );
    m_actionAlignLeft->addTo( menu );
    m_actionAlignLeft->setToggleAction( TRUE );
    m_actionAlignCenter = new QAction( tr( "Centrée" ), QPixmap(GlobalPathAppli +"QLightPadSys/textcenter.png" ), tr( "&Centrée" ), CTRL + Key_E, grp, "textCenter" );
    m_actionAlignCenter->addTo( tb );
    m_actionAlignCenter->addTo( menu );
    m_actionAlignCenter->setToggleAction( TRUE );
    m_actionAlignRight = new QAction( tr( "Droite" ), QPixmap(GlobalPathAppli +"QLightPadSys/textright.png" ), tr( "&Droite" ), CTRL + Key_R, grp, "textRight" );
    m_actionAlignRight->addTo( tb );
    m_actionAlignRight->addTo( menu );
    m_actionAlignRight->setToggleAction( TRUE );
    m_actionAlignJustify = new QAction( tr( "Justification" ), QPixmap(GlobalPathAppli +"QLightPadSys/textjustify.png" ), tr( "&Justification" ), CTRL + Key_J, grp, "textjustify" );
    m_actionAlignJustify->addTo( tb );
    m_actionAlignJustify->addTo( menu );
    m_actionAlignJustify->setToggleAction( TRUE );

    menu->insertSeparator();

    QPixmap pix( 16, 16 );
    pix.fill( black );
    m_actionTextColor = new QAction( tr( "Couleur" ), pix, tr( "&Couleur..." ), 0, this, "textColor" );
    connect( m_actionTextColor, SIGNAL( activated() ), this, SLOT( textColor() ) );
    m_actionTextColor->addTo( tb );
    m_actionTextColor->addTo( menu );
}

//------------------------------------------------------- setupHelpActions -------------------------------------
void QLightPad::setupHelpActions()
{
    QToolBar *tb = new QToolBar( this );
    tb->setLabel( tr("Outils aide et informations") );
    QPopupMenu *menu = new QPopupMenu( this );
    menuBar()->insertItem( tr( "&À propos" ), menu );
    QAction *a;
    a = new QAction( tr( "À propos" ), QPixmap(GlobalPathAppli +"QLightPadSys/help.png" ), tr( "À propos de Q&LightPad" ),0, this, "aPropos" );
    //connect( a, SIGNAL( activated() ), this, SLOT( aPropos() ) );
    connect( a, SIGNAL( activated() ), this, SLOT(Slot_actionApropos() ) );
    a->addTo( tb );
    a->addTo( menu );
    //a = new QAction( tr( "À propos de &Qt" ), QPixmap(GlobalPathAppli +"QLightPadSys/QTbutton.png" ), tr( "À propos de &QT" ),0, this, "aboutQt" );
    //connect( a, SIGNAL( activated() ), this, SLOT( aboutQt() ) );
    //a->addTo( tb );
    //a->addTo( menu );
}
//--------------------------------- Slot_SauverLesMeubles --------------------------------------------------------------------------------
void QLightPad::Slot_SauverLesMeubles()
{tryToStopAPropos();
}

//----------------------------------- Slot_actionApropos -----------------------------------------------------------------------
void QLightPad::Slot_actionApropos()
{QTimer::singleShot ( 100, this,SLOT(Slot_actionAproposDisplay()) );
}
//----------------------------------- Slot_actionAproposDisplay -----------------------------------------------------------------------
void QLightPad::Slot_actionAproposDisplay()
{        //CGestIni::Param_UpdateToDisk(G_pCApp->m_PathAppli+"Ressources/Changements.html",textEdit_Changements->text());
         QString pathExeAPropos     = CGestIni::Construct_Name_Exe("APropos", QFileInfo (qApp->argv()[0]).dirPath (true));
         QString pathBinRessources  = CGestIni::Construct_PathBin_Module("APropos", QFileInfo (qApp->argv()[0]).dirPath (true))+"Ressources/";
         QStringList argList;
         //......................... completer les autres arguments .........................................
         if (m_Apropos_Proc==0)
            {//m_action_A_Propos->setDisabled(TRUE);
             m_Apropos_Proc = new QProcess(this);
             m_Apropos_Proc->addArgument( pathExeAPropos);                                               // 1  nom du module
             m_Apropos_Proc->addArgument("QLightPad");                                                       // 1  nom du module
             m_Apropos_Proc->addArgument("Editeur de texte pour MedinTux");                        // 2  description courte
             m_Apropos_Proc->addArgument(m_NUM_VERSION.remove("@").remove("#").remove("=") + " Qt : "+QT_VERSION_STR);    // 3  numero de version
             m_Apropos_Proc->addArgument(GlobalPathAppli +"QLightPadSys/Changements.html");            // 4  fichiers d�crivant les changements
             m_Apropos_Proc->addArgument(Theme::getPath(Theme::WithSeparator)+"32x32/QLightPad.png");        // 5  Icone du programme
             m_Apropos_Proc->addArgument(GlobalPathAppli+"../../Doc/QLightPad.html");            // 6  aide en ligne

             m_Apropos_Proc->start();
             SLEEP(1);
             qApp->processEvents ();
             while ( m_Apropos_Proc->isRunning () /* && QFile::exists(pathBinRessources+"~A_propos.html")*/)
                   { //qDebug(QString::number(procState).toAscii());
                     qApp->processEvents ( QEventLoop::WaitForMore );
                   }
             if (m_Apropos_Proc) delete m_Apropos_Proc;
             m_Apropos_Proc = 0;
             QFile::remove(pathBinRessources+"~A_propos.html");
             //m_action_A_Propos->setDisabled(FALSE);
            }
}
//--------------------------------------- tryToStopAPropos ----------------------------------------------------------
void QLightPad::tryToStopAPropos()
{if (m_Apropos_Proc==0) return;
 m_Apropos_Proc->kill();   // terminate() ne fonctionne pas
}
//------------------------------------------------------- setupHelpActions -------------------------------------
void QLightPad::setupDrTuxPopupActions()
{
    QToolBar *tb = new QToolBar( this );
    tb->setLabel( tr("Outils de gestion des popups menus de DrTUX") );
    QPopupMenu *menu = new QPopupMenu( this );
    menuBar()->insertItem( tr( "&Gestion des listes" ), menu );
    QAction *a;
    a = new QAction( tr( "Outil de gestion des listes  déroulantes..." ), QPixmap(GlobalPathAppli +"QLightPadSys/DoList_Item.png" ), tr( "Outil de gestion des listes déroulantes..." ),Key_F8, this, "Outil de gestion des istes déroulantes..." );
    connect( a, SIGNAL( activated() ), this, SLOT( gererListeMenu() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Créer une liste déroulante avec le texte sélectionné" ), QPixmap(GlobalPathAppli +"QLightPadSys/CreateList_Item.png"), tr( "Créer une liste déroulante avec le texte sélectionné" ),Key_F10, this, "creerZoneListe" );
    connect( a, SIGNAL( activated() ), this, SLOT( creerZoneMenu() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Insérer un champ d'insertion..." ), QPixmap(GlobalPathAppli +"QLightPadSys/InsererChampInsertion.png" ), tr( "Insérer un champ d'insertion..." ),0, this, "Insérer un champ d'insertion..." );
    connect( a, SIGNAL( activated() ), this, SLOT( InsererChampInsertion() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Insérer une zone de liste déroulante déjà existante" ), QPixmap(GlobalPathAppli +"QLightPadSys/InsertList_Item.png"), tr( "Insérer une zone de liste déroulante déjà existante" ),0, this, "InsererZoneListe" );
    connect( a, SIGNAL( activated() ), this, SLOT( InsererZoneMenu() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Ajouter un item à une liste déroulante déjà existante" ), QPixmap(GlobalPathAppli +"QLightPadSys/AddList_Item.png"), tr( "Ajouter un item à une liste déroulante déjà existante" ),Key_F9, this, "AjoutItemZoneListe" );
    connect( a, SIGNAL( activated() ), this, SLOT( AjouterItemZoneMenu() ) );
    a->addTo( tb );
    a->addTo( menu );
    a = new QAction( tr( "Tester cette zone de liste déroulante" ), QPixmap(GlobalPathAppli +"QLightPadSys/PopupList_Item.png" ), tr( "Tester cette zone de liste déroulante..." ),Key_F2, this, "Tester cette zone de liste déroulante..." );
    connect( a, SIGNAL( activated() ), this, SLOT( testerZoneMenu() ) );
    a->addTo( tb );
    a->addTo( menu );
    /*
    a = new QAction( tr( "Insérer une fonction..." ), QPixmap(GlobalPathAppli +"QLightPadSys/Fonction_Icon.png" ), tr( "Insérer une fonction..." ),0, this, "Insérer une fonction..." );
    connect( a, SIGNAL( activated() ), this, SLOT( InsererFonction() ) );
    a->addTo( tb );
    a->addTo( menu );
    */

    moveDockWindow ( tb, Qt::DockLeft );
}

//------------------------------------------------------- RetirerInsecables -------------------------------------
void QLightPad::RetirerInsecables()
{QString txt_s;
 MyEditText *edit = (MyEditText*)currentEditor();
 if (!edit)                             return;
 if (edit->hasSelectedText())
    {txt_s = edit->selectedText().remove("<!--StartFragment-->");
     CHtmlTools::findReplace( txt_s, "&nbsp;"," ");
     char *pt    = (char*)(const char*)txt_s;
     char *ptDeb = pt;
     while (*pt)
          {if ((char)*pt == (char)0xA0) *pt = 0x20;
           ++pt;
          }
     txt_s = QString(ptDeb);
     CHtmlTools::insertHtml(edit, txt_s, 0);
    }
}

//------------------------------------------------------- InsererInsecables -------------------------------------
void QLightPad::InsererInsecables()
{QString txt_s;
 MyEditText *edit = (MyEditText*)currentEditor();
 if (!edit)                             return;
 if (edit->hasSelectedText())
    {txt_s = edit->selectedText().remove("<!--StartFragment-->");
     CHtmlTools::findReplace( txt_s, " ", "&nbsp;");
    }
 else
    {if (CGenTools::DialogGetString(this, tr("insérer un texte avec des espaces insécables"), txt_s) != QDialog::Accepted)
        {txt_s = "";
        }
     txt_s.replace(" ","&nbsp;");
    }
 CHtmlTools::insertHtml(edit, txt_s, 0);
}

//---------------------------- CodePre -------------------------------------------------
void QLightPad::CodePre()
{MyEditText *edit = (MyEditText*)currentEditor();
  if (!edit)                             return;
  if (edit->m_Path=="")                  return;
  if (edit->hasSelectedText()==FALSE)
     {QMessageBox::warning ( this, tr(" Insertion de liste"),
                                   tr ( " ATTENTION !\r\n      Pour cette action, il faut sélectionner une ou plusieurs lignes : \r\n      "),
                                   tr("&Valider"), 0, 0, 1, 1 );
                                         return;
     }


  QString txt_s = edit->selectedText().remove("<!--StartFragment-->");
  txt_s         = txt_s.remove('\n');
  txt_s         = txt_s.remove('\r');
  CHtmlTools::HtmlToAscii(txt_s, CHtmlTools::StrictASCII|CHtmlTools::KeepBR);
  txt_s         = "<code>" + txt_s + "</code>";
  CHtmlTools::insertHtml(edit, txt_s);
  /*
  edit->setUpdatesEnabled( FALSE );                      // inhiber l'affichage (faut pas montrer la selection)

  int deb_para, deb_index, end_para, end_index;
  edit->getSelection ( &deb_para, &deb_index, &end_para, &end_index );

  edit->insertAt ( "~#@@#~~",    end_para, end_index );  // commencer par la fin hé hé !!!
  edit->insertAt ( "#__#~~",     deb_para, deb_index );

  QString text      = edit->text();
  int  pos_deb      = text.find("#__#~~");           if (pos_deb == -1)       return;
  int  pos_end      = text.find("~#@@#~~")+7;        if (pos_end ==  6)       return;
  QString textToPre = text.mid(pos_deb, pos_end - pos_deb);
  textToPre         = textToPre.remove('\n');
  textToPre         = textToPre.remove('\r');
  CHtmlTools::HtmlToAscii(textToPre, CHtmlTools::StrictASCII);
  textToPre.replace("#__#~~",  "<code>");
  textToPre.replace("~#@@#~~", "</code>");

  text.replace( pos_deb, pos_end-pos_deb, textToPre );
  edit->setText(text);
  edit->setCursorPosition (deb_para, deb_index );
  edit->setUpdatesEnabled( TRUE );
  */
}

//---------------------------- InsererFonction -------------------------------------------------
void QLightPad::InsererFonction()
{
}
//---------------------------- InsererZoneMenu -------------------------------------------------
void QLightPad::InsererZoneMenu()
{MyEditText *pMyEditText = currentEditor();
 if (pMyEditText==0)  return;
 FormDlgListOff_Listes *dlg = new FormDlgListOff_Listes(this,"RefList_Dial",TRUE);
 if (dlg==0) return;
 dlg->initDialog(m_PathGlossaire+"/Champs d'insertions");
 dlg-> exec();
 if (dlg->m_ItemName !="")
    {QColor old_col = pMyEditText->color();
      if (pMyEditText->hasSelectedText()) pMyEditText->del();
      pMyEditText->setColor( QColor(0x00,0x00,0xFF) );
      pMyEditText->insert(dlg->m_ItemName);
      pMyEditText->setColor( old_col );
    }
 delete dlg;
}
//---------------------------- MakeInsertFieldList -------------------------------------------------
void QLightPad::MakeInsertFieldList(const char *path, QStringList &fieldList, int mustBeClear )
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
//---------------------------------------------------- AjouterItemZoneMenu ---------------------------------------------------------------
void QLightPad::AjouterItemZoneMenu()
{MyEditText *pMyEditText = currentEditor();
 if (pMyEditText==0)  return;
 pMyEditText->PopupItemAddItem();
}

//---------------------------------------------------- InsererChampInsertion ---------------------------------------------------------------
void QLightPad::InsererChampInsertion()
{MyEditText *pMyEditText = currentEditor();
 if (pMyEditText==0)  return;
 pMyEditText->InsererChampInsertion();
}

//---------------------------------------------------- creerZoneMenu ---------------------------------------------------------------
void QLightPad::creerZoneMenu()
{MyEditText *pMyEditText = currentEditor();
 if (pMyEditText==0)  return;
 pMyEditText->PopupCreateList();

}

//---------------------------------------------------- creerZoneMenu ---------------------------------------------------------------
void QLightPad::testerZoneMenu()
{MyEditText *pMyEditText = currentEditor();
 if (pMyEditText==0)  return;
 pMyEditText->DoContextListPopup();
}

//---------------------------------------------------- creerZoneMenu ---------------------------------------------------------------
void QLightPad::gererListeMenu()
{//MyEditText *edit = (MyEditText*)currentEditor();
 //edit->m_AutoRet  =  m_actionAutoRet->isOn() ? -1 : 0;
 //edit->setWordWrap ( m_actionAutoRet->isOn() ? QTextEdit::WidgetWidth : QTextEdit::NoWrap );
 MyEditText *pMyEditText = currentEditor();
 if (pMyEditText)
    {pMyEditText->m_list_name     =  pMyEditText->GetLastList();
     pMyEditText->ListManager();
    }
 else
   {
/*
    FormDlgListFieldMngr *dlg = new FormDlgListFieldMngr(this, "ListManager_Dial", TRUE, WStyle_Customize | WStyle_NormalBorder | WStyle_Title |WStyle_MinMax | WStyle_SysMenu);
    if (dlg ==0) return;
    dlg->initDialog("",m_PathGlossaire, m_PathDrTux);
    dlg->exec();
    delete dlg;
*/
    C_DlgListFieldMngr *dlg = new C_DlgListFieldMngr(this, "ListManager_Dial", TRUE, WStyle_Customize | WStyle_NormalBorder | WStyle_Title |WStyle_MinMax | WStyle_SysMenu);
    if (dlg ==0) return;
    dlg->initDialog("", m_PathGlossaire, m_PathDrTux);
    dlg->exec();
    //recordPathModulesMenus(dlg->m_PathMenuLib);
    delete dlg;
   }
}

//---------------------------------------------------- setAutoRet ---------------------------------------------------------------
void QLightPad::setAutoRet()
{//MyEditText *edit = (MyEditText*)currentEditor();
 //edit->m_AutoRet  =  m_actionAutoRet->isOn() ? -1 : 0;
 //edit->setWordWrap ( m_actionAutoRet->isOn() ? QTextEdit::WidgetWidth : QTextEdit::NoWrap );

}

//---------------------------------------------------- aPropos ---------------------------------------------------------------
void QLightPad::aPropos()
{if (m_Dlg_A_Propos)    return;
 m_Dlg_A_Propos = new Dlg_A_Propos(this, "Dlg_A_Propos", FALSE);
 if (m_Dlg_A_Propos==0) return;
 QString infoSysteme = "";
 if (m_debug)
    {infoSysteme = m_docToLoadList.join ( "<br>" );
    }
 m_Dlg_A_Propos->setInfoSysteme(infoSysteme);
 connect( m_Dlg_A_Propos, SIGNAL( Sign_DlgClosed() ), this, SLOT( aProposDelete() ) );
 m_Dlg_A_Propos->show();
}

//---------------------------------------------------- aProposDelete ---------------------------------------------------------------
void QLightPad::aProposDelete()
{delete m_Dlg_A_Propos;
 m_Dlg_A_Propos = 0;
 if (m_AppMustQuit == TRUE) close();
}

//---------------------------------------- aboutQt -------------------------------------------------------------------------------
void QLightPad::aboutQt()
{QMessageBox::aboutQt( this, tr("QLightPad par SEVIN Roland avec  QT") );
}

//---------------------------------------------------------------------------- ParametresParDefaut ---------------------------------------
void QLightPad::ParametresParDefaut()
{ MyEditText *edit = (MyEditText*)currentEditor();
  if (!edit)                             return;

  Dlg_BodyAttributs *dlg = new Dlg_BodyAttributs(this, "DefaultParam", TRUE);
  if (dlg == 0)                          return;
  dlg->initDialog( edit );
  if (dlg->exec() != QDialog::Accepted)   {delete dlg; return;}
  edit->setModified(TRUE);
  delete dlg;
}

//---------------------------------------------------------------------------- InsererLien ---------------------------------------
void QLightPad::InsererLien()
{
  MyEditText *edit = (MyEditText*)currentEditor();
  if (!edit)                             return;
  if (edit->m_Path=="")                  return;

  //............................. noter la sélection ...............................
  int paraFrom, indexFrom, paraTo, indexTo;
  edit->getSelection ( &paraFrom, &indexFrom, &paraTo, &indexTo );
  int deb_para, deb_index;
  edit->getCursorPosition (&deb_para, &deb_index );
  //......................... executer dialogue ...................................
  Dlg_InsertLink* dlg = new Dlg_InsertLink(this, "InsererLien", TRUE);
  if (dlg == 0)                          return;
  dlg->initDialog( edit , m_FileNames);
  if (dlg->exec()!= QDialog::Accepted)   {delete dlg; return;}
  //................... replacer la sélection initiale ...............................
  edit->setSelection ( paraFrom,  indexFrom,  paraTo,  indexTo);

  //................ recuperer valeurs..............................................
  /*
  QString linkName     =  dlg->textEdit_Cible->text();
  QString linkText     =  dlg->textEdit_Lien->text();

  //............................... inserer le texte .................................
  // "<A href=\"ch01.htm#01010300\">01.01.03</A>"
  //edit->setUpdatesEnabled( FALSE );                      // inhiber l'affichage (faut pas montrer la selection)
  //....................... creer le lien ............................................
  QString toInsert = "<a href=\"";
  toInsert        +=  linkName;
  toInsert        +="\">";
  toInsert        +=  linkText;
  toInsert        +="</a>";
  //....................... placer le lien ............................................
  edit->insert("~~##__##~~");
  QString* pTxt = new QString(edit->text());
  if (pTxt ==0)        return;
  pTxt->replace("~~##__##~~", toInsert);
  edit->setText(*pTxt);
  delete pTxt;
  edit->setUpdatesEnabled( TRUE );                          // desinhiber l'affichage (faut pas montrer la selection)
  */
  CHtmlTools::insertHtml(edit, QString("<a href=\"") + dlg->textEdit_Cible->text() + "\">" + dlg->textEdit_Lien->text() + "</a>");
  delete dlg;
  edit->setCursorPosition (deb_para, deb_index );
}

//---------------------------------------------------------------------------- InsererEmail ---------------------------------------
void QLightPad::InsererEmail()
{ MyEditText *edit = (MyEditText*)currentEditor();
  if (!edit)                             return;
  //................ recuperer selection car le dialogue les annule ................
  int  paraFrom,  indexFrom,  paraTo,  indexTo;
  edit->getSelection ( &paraFrom, &indexFrom, &paraTo, &indexTo);
  //................. creer le dialogue ............................................
  Dlg_InsertEmail* dlg = new Dlg_InsertEmail(this,"EmailDlg",TRUE);
  if (dlg == 0)                          return;
  //................. placer la sélection comme intitulé ............................
  QString intituleText = edit->selectedText();
  CHtmlTools::HtmlToAscii(intituleText);
  //................. si contient arobace alors aussi comme cible ...................
  QString targetName (tr("email@fournisseur.acces.internet"));
  if (intituleText.find('@') != -1) targetName =  intituleText;
  dlg->textEdit_Intitule->setText(intituleText);
  dlg->textEdit_Cible->setText(targetName);
  if (dlg->exec()!= QDialog::Accepted)   {delete dlg; return;}
  //................... replacer la sélection initiale ...............................
  edit->setSelection ( paraFrom,  indexFrom,  paraTo,  indexTo);
  //............................. insérer  l'Email ...................................
  CHtmlTools::insertHtml(edit, QString("<a href=\"mailto:") + dlg->textEdit_Cible->text() + "\">" + dlg->textEdit_Intitule->text() + "</a>");
  delete dlg;
}

//---------------------------------------------------------------------------- InsererAncre ---------------------------------------
void QLightPad::InsererAncre()
{ MyEditText *edit = (MyEditText*)currentEditor();
  if (!edit)                             return;
  Dlg_InsertAncre* dlg = new Dlg_InsertAncre(this, "DlgInsertAnchor",TRUE);
  if (dlg == 0)                          return;
  dlg->initDialog(edit, *m_FileNames.find(edit), &m_AnchorSpaceToUnderscore);
  dlg->exec();
  delete dlg;
}

//---------------------------------------------------------------------------- InsererLigne ---------------------------------------
void QLightPad::InsererLigne()
{ MyEditText *edit = (MyEditText*)currentEditor();
  if (!edit)                             return;
  CHtmlTools::insertHtml(edit, "<hr>",0);
}

//---------------------------------------------------------------------------- InsererListe ---------------------------------------
void QLightPad::InsererListe()
{ MyEditText *edit = (MyEditText*)currentEditor();
  if (!edit)                             return;
  if (edit->m_Path=="")                  return;
  if (edit->hasSelectedText()==FALSE)
     {QMessageBox::warning ( this, tr(" Insertion de liste"),
                                   tr ( " ATTENTION !\r\n      Pour cette action, il faut sélectionner une ou plusieurs lignes : \r\n      "),
                                   tr("&Valider"), 0, 0, 1, 1 );
                                         return;
     }

  //............................... inserer le texte ..........................................
  QString txt = edit->selectedText();
  edit->setUpdatesEnabled( FALSE );                      // inhiber l'affichage (faut pas montrer la selection)
  //<ul>...</ul>
  int deb_para, deb_index, end_para, end_index;
  edit->getSelection ( &deb_para, &deb_index, &end_para, &end_index );

  edit->insertAt ( "~~##@@##~~", end_para, end_index );  // commencer par la fin hé hé !!!
  edit->insertAt ( "~~##__##~~", deb_para, deb_index );

  QString* pTxt = new QString(edit->text());
  if (pTxt ==0)        return;
  pTxt->replace("~~##__##~~", "<ul type=\"disc\">");
  pTxt->replace("~~##@@##~~", "</ul>");

  edit->setText(*pTxt);
  delete pTxt;
  edit->setCursorPosition (deb_para, deb_index );
  edit->setUpdatesEnabled( TRUE );                      // desinhiber l'affichage (faut pas montrer la selection)

  /*
  int deb_para, deb_index, end_para, end_index;
  edit->getSelection ( &deb_para, &deb_index, &end_para, &end_index );
  edit->removeSelection ();
  edit->setCursorPosition (end_para, end_index );
  insertHtml(edit, "</ul>");
  edit->setCursorPosition (deb_para, deb_index );
  insertHtml(edit, "<ul>");
  */
  /*
  //<!--StartFragment-->
  QString txt = edit->selectedText().remove("<!--StartFragment-->");
  txt.prepend("<!--StartFragment--><ul type=\"disc\"><li style=\"margin-to:12px\">machin</li>\n<li>truc");
  txt.append("</li>");
  insertHtml(edit, "<!--StartFragment--><ul type=\"disc\">\n<li style=\"margin-to:12px\">machin</li>\n<li>truc</li></ul>");
  */
}
//---------------------------------------------------------------------------- InsererPuce ---------------------------------------
void QLightPad::InsererPuce()
{ MyEditText *edit = (MyEditText*)currentEditor();
  if (!edit)                             return;
  if (edit->m_Path=="")                  return;
  if (edit->hasSelectedText()==FALSE)
     {QMessageBox::warning ( this, tr(" Insertion de liste marquée"),
                                   tr ( " ATTENTION !\r\n      Pour cette action, il faut sélectionner une ou plusieurs lignes : \r\n      "),
                                   tr("&Valider"), 0, 0, 1, 1 );
                                         return;
     }
  //............................... inserer le texte ..........................................
  edit->setUpdatesEnabled( FALSE );                      // inhiber l'affichage (faut pas montrer la selection)
  //<ul>...</ul>
  int deb_para, deb_index, end_para, end_index;
  edit->getSelection ( &deb_para, &deb_index, &end_para, &end_index );
  edit->insertAt ( "~~##@@##~~", end_para, end_index );     // commencer par la fin hé hé !!!
  edit->insertAt ( "~~##__##~~", deb_para, deb_index );

  QString* pTxt = new QString(edit->text());
  if (pTxt ==0)        return;
  pTxt->replace("~~##__##~~", "<li>");
  pTxt->replace("~~##@@##~~", "</li>");

  edit->setText(*pTxt);
  delete pTxt;
  edit->setCursorPosition (deb_para, deb_index );
  edit->setUpdatesEnabled( TRUE );                      // desinhiber l'affichage (faut pas montrer la selection)
  //insertHtml(edit, QString("<li>") +  edit->selectedText() + "</li>");  // marche pô
}

//---------------------------------------------------------------------------- InsererImage ---------------------------------------
void QLightPad::InsererImage()
{ MyEditText *edit = (MyEditText*)currentEditor();
  if (!edit)                             return;
  if (edit->m_Path=="")                  return;
  //................. choisir le fichier image src à insérer .................................................
  QFileDialog *fd = new QFileDialog  ( edit->m_Path,
                                       tr( "IMG-Files (*.png *.png *.jpg *.gif *.bmp );;All Files (*)" ),
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
     {if (m_PathGlossaire.length()) filename = CHtmlTools::Copy_File(filename, m_PathGlossaire + "/ImagesFolder");
      else                          filename = CHtmlTools::Copy_File(filename, edit->m_Path);
      CHtmlTools::insertHtml(edit, QString("<img src=\"") + filename + "\">",0);
     }
}

//---------------------------------------------------------------------------- load ---------------------------------------
void QLightPad::load( const QString &f )
{   QString style("");


    if ( !QFile::exists( f ) ) return;
    MyEditText *edit = new MyEditText( m_PathGlossaire, m_PathDrTux , &m_InsertFieldList, m_tabWidget);
    doConnections( edit );
    // m_tabWidget->addTab( edit, QIconSet (QPixmap(GlobalPathAppli +"QLightPadSys/LedUTF8.png" )), QFileInfo( f ).fileName() );

    QFile file( f );
    if ( !file.open( IO_ReadOnly ) )        return;
    // QTextStream ts( &file );
    //................ deviner le codec cot cot .........................
    /*
    char buffer[512];
    int l = file.readBlock(buffer,512);
    QTextCodec* codec = QTextCodec::codecForContent(buffer, l);
    if ( codec )
       {QMessageBox::information(this,"Encoding",QString("Codec: ")+codec->name());
        ts.setCodec( codec );
       }
       */
    // QString src = ts.read();
    QString src ="";
    CGestIni::Param_UpdateFromDisk(f, src, &edit->m_IsUTF8);
    if (edit->m_IsUTF8)
        m_tabWidget->addTab( edit, QIconSet (QPixmap(GlobalPathAppli +"QLightPadSys/LedUTF8.png" )), QFileInfo( f ).fileName() );
    else
        m_tabWidget->addTab( edit,  QFileInfo( f ).fileName() );
    QString txt = "";
    if (CHtmlTools::IsRtf(src))
       {CHtmlTools::RtfToQtextEdit_HTML(src, src.length(), txt);
       }
    else
       {txt = src;
       }
    if ( !QStyleSheet::mightBeRichText( txt ) )
       {txt = QStyleSheet::convertFromPlainText( txt, QStyleSheetItem::WhiteSpacePre );
        txt = txt.remove("<p>");
        edit->m_DefaultFont = QFont("Courier", 9);
       }
    else
       {CHtmlTools::ElimineAncresRedondantes(txt);
        txt.replace("<plaintext>","<pre>");
        txt.replace("</plaintext>","</pre>");
        txt.replace("&rsquo;","'");
        txt.replace("&ndash;","-");
        txt.replace("meta name=\"qrichtext\" content=\"charset=utf-8\"",   // obligé d'etre en content=\"1\" pour que les tabulations fonctionnent !!
                    "meta name=\"qrichtext\" content=\"1\"");
        CHtmlTools::tagPreToHtml( txt );
        style = CHtmlTools::getTextDefaultAttribut(txt, "style");
        CHtmlTools::QT_style_To_QFont( style, edit->m_DefaultFont);
       }
    edit->m_Path = QFileInfo( f ).dirPath ( TRUE );
    edit->mimeSourceFactory()->addFilePath(edit->m_Path );
    edit->setFont (edit->m_DefaultFont);
    edit->setText( txt,  edit->m_Path );
    m_FileNames.replace( edit, f ); // je suppose que si la key edit n'est pas trouvée, un nouveau couple est créé !!!
    edit->m_PathImage              = CHtmlTools::getTextDefaultAttribut(txt, "background");
    edit->m_DefaultTextColor       = CHtmlTools::getTextDefaultAttribut(txt, "text");
    edit->m_DefaultBackgroundColor = CHtmlTools::getTextDefaultAttribut(txt, "bgcolor");
    edit->setModified(FALSE);
    m_tabWidget->showPage( edit );
    edit->viewport()->setFocus();
    //m_actionAutoRet->setOn( (edit->wordWrap()==QTextEdit::WidgetWidth));
}

//------------------------------------------------------- fileSave -------------------------------------
void QLightPad::fileSave()
{MyEditText *edit =  currentEditor();
 if (!edit) return;
 fileSave(edit, edit->m_Filter);
}
//------------------------------------------------------- fileSaveMeta -------------------------------------
void QLightPad::fileSaveMeta()
{MyEditText *edit =  currentEditor();
 if (!edit)
     { QMessageBox::warning ( this, tr(" Choix du metatag html"),
                                    tr ( " ATTENTION !\n      Pour cette action, il faut un texte en cours d'édition.\n      "),
                                    tr("&Ok"), 0, 0, 1, 1 );
       return;
     }
 QString metaTags     = CGestIni::Param_UpdateFromDisk(GlobalPathAppli +"QLightPadSys/metaTags.txt").remove("\r");
 QStringList metaList = QStringList::split ("\n", metaTags, FALSE );
 QStringList popList;
 //............ reperer utf8 ou pas ........................
 QString txt (edit->text());
 edit->m_IsUTF8 = CGestIni::IsUtf8( txt ); 
 //........... creer la liste en fonction .................
 for (int i=0; i<(int)metaList.size(); ++i)
    { QString item = metaList[i];
      // if (edit->m_IsUTF8)
         {if (item.startsWith("utf-8|")) popList.append(item.mid(6));
         }
      // else
         {if (item.startsWith("latin1|")) popList.append(item.mid(7));
         }
    }
  //.............. actionner le popup ...................
 ThemePopup *pPopup = new ThemePopup(popList, this, "lePopup");
 if (pPopup==0) return;
 QString option = pPopup->DoPopupList();
 if (option.length()==0) return;
 fileSave(edit, option);
}

//------------------------------------------------------- fileSave -------------------------------------
void QLightPad::fileSave(MyEditText *edit, const QString &metaTag)
{
    if ( !edit )                             return;
    QString fn;
    if ( m_FileNames.find( edit ) == m_FileNames.end() )
       {fileSaveAs();
       }
    else
       {//............... relever extension du fichier ....................................
        QString       ext ("");
        QString    f_name (*m_FileNames.find( edit ));
        int           pos = f_name.findRev ('.');
        if (pos !=-1)
           {ext = f_name.mid(pos);
            f_name = f_name.left(pos);
           }
        //............... relever extension du ifltre selectionné .........................
        QString filter_ext = "";
        int deb = edit->m_Filter.findRev("*");
        int end = pos;
        if (deb != -1)
           {deb +=  1;        // passer le *.
            end  = edit->m_Filter.find(")", pos);
            if (end !=-1) filter_ext = edit->m_Filter.mid(deb, end-deb);
           }
        //............. si filtre est "*" (tous les fichers) alors.........................
        //              c'est l'extension du fichier qui prime
        //              sinon c'est celle du filtre qui est retenue
        if (filter_ext == "*" || filter_ext.length()<=0)
           {if (ext.length() <=0 ) ext = ".html";
           }
        else
           {ext = filter_ext;
           }
        f_name += ext;
        if (ext.left(4).lower() != ".htm" && ext.lower()!= ".rtf" )
           {QFile file( f_name);
            if ( !file.open( IO_WriteOnly ) )    return;
            QString txt (edit->text());
            int pos = txt.find("<p>");       // virer le header et le <p> qui va avec
            if (pos != -1)
               {pos += 3;
                txt  = txt.mid(pos, txt.length()-pos);
               }
            CHtmlTools::HtmlToAscii(txt, CHtmlTools::StrictASCII);
            file.writeBlock (txt , txt.length());
            txt.replace("<","&lt;");
            txt.replace(">","&gt;");
            //txt.replace(" ","&nbsp;");
            txt.replace("\n","<br />");
            txt.replace("\r\n","<p>");
            edit->setFont (QFont("Courier", 9));
            edit->setText(txt);
            QString dstDir = QFileInfo(f_name).dirPath (true);
            edit->m_Path = dstDir;
            edit->mimeSourceFactory()->addFilePath(edit->m_Path );
            m_tabWidget->setTabLabel( edit, QFileInfo(f_name).fileName() );
            m_FileNames.replace( edit, f_name );
            edit->setModified(FALSE);
            edit->m_IsUTF8 = CGestIni::IsUtf8( txt );
            if ( edit->m_IsUTF8 )
               { m_tabWidget->setTabIconSet ( edit, QIconSet (QPixmap(GlobalPathAppli +"QLightPadSys/LedUTF8.png" )) ) ;
               }
            else
               {m_tabWidget->setTabIconSet ( edit, QIconSet (QPixmap("")) ) ;
               }
            return;
           }
        else
           {//..................... on rajoute de force l'extension html ..........................
            if (! (ext.lower()==".htm" || ext.lower()==".html"))
               {if (pos==-1) f_name += ".html";
                else         f_name  = f_name.left(pos) + ".html";
               }
            QFile file( f_name);
            if ( !file.open( IO_WriteOnly ) )    return;
            QTextStream ts( &file );

            QString txt (edit->text());
            CHtmlTools::ElimineAncresRedondantes(txt);
            CHtmlTools::setTextDefaultAtribut(txt, "background",    edit->m_PathImage);
            CHtmlTools::setTextDefaultAtribut(txt, "text",          edit->m_DefaultTextColor);
            CHtmlTools::setTextDefaultAtribut(txt, "bgcolor",       edit->m_DefaultBackgroundColor);
            QString codec = ts.codec()->name();
            if (metaTag.length())
               {txt.replace("meta name=\"qrichtext\" content=\"1\"",metaTag);
                if (codec.lower() == "utf-8") m_tabWidget->setTabIconSet ( edit, QIconSet (QPixmap(GlobalPathAppli +"QLightPadSys/LedUTF8.png" )) ) ;
                else                          m_tabWidget->setTabIconSet ( edit, QIconSet (QPixmap("")) ) ;
               }
            else if (codec.lower() == "utf-8")
               {txt.replace("meta name=\"qrichtext\" content=\"1\"","meta name=\"qrichtext\" content=\"charset=utf-8\"");
                edit->m_IsUTF8 = 1;
                m_tabWidget->setTabIconSet ( edit, QIconSet (QPixmap(GlobalPathAppli +"QLightPadSys/LedUTF8.png" )) ) ;
               }
            CHtmlTools::QRichTextToHtml(txt);
            ts << txt;
            //................... si le path d'ecriture du texte n'est pas le meme ...........................
            //                    que celui du texte actuel (celui du chargement) alors changement de repertoire
            //                    alors recopier les images
            QString dstDir = QFileInfo(f_name).dirPath (true);
            if (edit->m_Path != dstDir )
               {//.............. copier le fond ...............................
                if (edit->m_PathImage.length())
                   {CHtmlTools::Copy_File(edit->m_Path + QDir::separator() + edit->m_PathImage, dstDir);
                   }
                //.............. copier les images ............................
                QStringList imgList;
                CHtmlTools::getTextImageList(txt, imgList);
                for ( QStringList::Iterator it = imgList.begin(); it != imgList.end(); ++it )
                    {CHtmlTools::Copy_File(edit->m_Path + QDir::separator()+*(it), dstDir);
                    }
               }
            edit->m_Path = dstDir;
            edit->mimeSourceFactory()->addFilePath(edit->m_Path );
            m_tabWidget->setTabLabel( edit, QFileInfo(f_name).fileName());
            m_FileNames.replace( edit, f_name );
            edit->setModified(FALSE);
           }
       }
}

//------------------------------------------------------- currentEditor -------------------------------------
MyEditText *QLightPad::currentEditor() const
{ if ( m_tabWidget->currentPage() && m_tabWidget->currentPage()->inherits( "MyEditText" ) )
        return (MyEditText*)m_tabWidget->currentPage();
    return 0;
}

//------------------------------------------------------- fileNew -------------------------------------
void QLightPad::fileNew()
{   MyEditText *edit = new MyEditText(  m_PathGlossaire, m_PathDrTux , &m_InsertFieldList, m_tabWidget );
    edit->setTextFormat( RichText );
    doConnections( edit );
    m_tabWidget->addTab( edit, tr( "sans_nom" ) );
    m_tabWidget->showPage( edit );
    edit->viewport()->setFocus();
    edit->mimeSourceFactory()->addFilePath(edit->m_Path );
}

//------------------------------------------------------- doConnections -------------------------------------
void QLightPad::doConnections( QTextEdit *e )
{
    connect( e, SIGNAL( currentFontChanged( const QFont & ) ),
             this, SLOT( fontChanged( const QFont & ) ) );
    connect( e, SIGNAL( currentColorChanged( const QColor & ) ),
             this, SLOT( colorChanged( const QColor & ) ) );
    connect( e, SIGNAL( currentAlignmentChanged( int ) ),
             this, SLOT( alignmentChanged( int ) ) );
}

//------------------------------------------------------- fileOpen -------------------------------------
void QLightPad::fileOpen()
{   QString fn;
    MyEditText *edit = (MyEditText*)currentEditor();
    if (edit==0)
        fn = CGestIni::Construct_PathBin_Module("QLightPad",  QFileInfo(qApp->argv()[0]).dirPath (true));
    else
        fn = edit->m_Path;
    QFileDialog *fd = new QFileDialog (fn,
                                       tr( "Fichiers texte (*.htm *.html *.HTM *.HTML *.TXT *.txt *.rtf *.RTF);;"
                                           "Fichiers de source (*.c *.cpp *.h *.ui.h *.ui);;"
                                           "Tous les fichiers (*)" ),
                                       this,
                                       "OuvrirDlg"
                                      ) ;
    if (fd==0)                                             return;
    fd->setMode(QFileDialog::ExistingFiles);    // selection multiple
    if ( fd->exec() == QDialog::Accepted )
       {QStringList listFile = fd->selectedFiles ();
        for ( QStringList::Iterator it = listFile.begin(); it != listFile.end(); ++it )
            {if ( !(*(it)).isEmpty() ) load( *(it) );
            }
       }
    delete fd;
}

//------------------------------------------------------- fileSaveAs -------------------------------------
void QLightPad::fileSaveAs()
{   QString fn;
    MyEditText *edit = (MyEditText*)currentEditor();
    if (edit==0)
        fn = CGestIni::Construct_PathBin_Module("QLightPad",  QFileInfo(qApp->argv()[0]).dirPath (true));
    else
        fn = edit->m_Path;
    QFileDialog *fd = new QFileDialog (fn,
                                       tr( " Fichiers HTML (*.htm *.html);;"
                                           " Fichiers de texte (*.asc *.txt);;"
                                           " Fichiers ASCII (*.cpp *.h *.ui.h *.ui *.c);;"
                                           " Tous les fichiers (*)"),
                                       this,
                                       "SaveDlg"
                                      ) ;
    if (fd==0)                                             return;
    fd->setMode(QFileDialog::AnyFile );    // selection multiple
    if ( fd->exec() == QDialog::Accepted )
       {QString fn = fd->selectedFile();
        // QString sf = fd->selectedFilter();
        if ( !fn.isEmpty() )
           {m_FileNames.replace( edit, fn );   // je suppose que si la key edit n'est pas trouvée, un nouveau couple est créé !!!
            edit->m_Filter = fd->selectedFilter();
            fileSave();
           }
       }
    delete fd;
}

//------------------------------------------------------- filePrint -------------------------------------
void QLightPad::filePrint()
{
    if ( !currentEditor() )     return;
#ifndef QT_NO_PRINTER
    QPrinter printer( QPrinter::HighResolution );
    printer.setFullPage(TRUE);
    if ( printer.setup( this ) ) {
        QPainter p( &printer );
        // Check that there is a valid device to print to.
        if ( !p.device() ) return;
        QPaintDeviceMetrics metrics( p.device() );
        int dpiy = metrics.logicalDpiY();
        int margin = (int) ( (2/2.54)*dpiy ); // 2 cm margins
        QRect body( margin, margin, metrics.width() - 2*margin, metrics.height() - 2*margin );
        QFont font( currentEditor()->QWidget::font() );
        font.setPointSize( 10 ); // we define 10pt to be a nice base size for printing

        QSimpleRichText richText( currentEditor()->text(), font,
                                  currentEditor()->context(),
                                  currentEditor()->styleSheet(),
                                  currentEditor()->mimeSourceFactory(),
                                  body.height() );
        richText.setWidth( &p, body.width() );
        QRect view( body );
        int page = 1;
        do {
            richText.draw( &p, body.left(), body.top(), view, colorGroup() );
            view.moveBy( 0, body.height() );
            p.translate( 0 , -body.height() );
            p.setFont( font );
            p.drawText( view.right() - p.fontMetrics().width( QString::number( page ) ),
                        view.bottom() + p.fontMetrics().ascent() + 5, QString::number( page ) );
            if ( view.top()  >= richText.height() )
                break;
            printer.newPage();
            page++;
        } while (TRUE);
    }
#endif
}

//------------------------------------------------------- fileClose -------------------------------------
void QLightPad::fileClose()
{ closeEditor(currentEditor());
}

//------------------------------------------------------- closeEditor -------------------------------------
/*
enum SavRequest
    {   NoSaveCurrentRequest   = 0x0002,
        NoSaveALLRequest       = 0x0003,
        AbortRequest           = 0x0004,
        SaveCurrentRequest     = 0x0000,
        SaveALLRequest         = 0x0001
    };
*/
int QLightPad::closeEditor(MyEditText *edit, int saveRequest /* = QLightPad::NoSaveRequest */)
{   int ret  = saveRequest;
    if (edit ==0) return CSaveRequest::AbortRequest;   //
    if (edit->isModified() && (saveRequest==CSaveRequest::SaveCurrentRequest || saveRequest==CSaveRequest::SaveALLRequest ) )
       {if (saveRequest==CSaveRequest::SaveCurrentRequest)   // pour CSaveRequest::SaveALLRequest on pose pas la question
           {Dlg_SaveRequest *dlg = new Dlg_SaveRequest(this,"SaveRequestDlg",TRUE);
            if (dlg)
               {QString fname = *m_FileNames.find(edit);
                if (fname.length()==0) fname = tr("sans_nom");
                dlg->textLabelModifiedFile->setText(fname);
                dlg->exec();
                ret = dlg->m_RetCode;
                delete dlg;
               }
           }
        if (ret==CSaveRequest::SaveCurrentRequest||ret==CSaveRequest::SaveALLRequest)    //QLightPad::SaveRequest = 0
           {fileSave(edit);
           }
        if (ret==CSaveRequest::NoSaveCurrentRequest) ret = CSaveRequest::SaveCurrentRequest; // remettre en jeux la question
       }
    if (ret != CSaveRequest::AbortRequest)
       {delete edit;
        m_FileNames.remove(edit);
        if ( currentEditor() )  currentEditor()->viewport()->setFocus();
       }
    return ret;
}

//------------------------------------------------------- editUndo -------------------------------------
void QLightPad::editUndo()
{   if ( !currentEditor() )   return;
    currentEditor()->undo();
}

//------------------------------------------------------- editRedo -------------------------------------
void QLightPad::editRedo()
{   if ( !currentEditor() )   return;
    currentEditor()->redo();
}

//------------------------------------------------------- editCut -------------------------------------
void QLightPad::editCut()
{   if ( !currentEditor() ) return;
    currentEditor()->cut();
}

//------------------------------------------------------- editCopy -------------------------------------
void QLightPad::editCopy()
{   if ( !currentEditor() )  return;
    currentEditor()->copy();
}

//------------------------------------------------------- editPaste -------------------------------------
void QLightPad::editPaste()
{   if ( !currentEditor() ) return;
    currentEditor()->paste();
}

//------------------------------------------------------- textBold -------------------------------------
void QLightPad::textBold()
{   if ( !currentEditor() )    return;
    currentEditor()->setBold( m_actionTextBold->isOn() );
}

//------------------------------------------------------- textUnderline -------------------------------------
void QLightPad::textUnderline()
{   if ( !currentEditor() )    return;
    currentEditor()->setUnderline( m_actionTextUnderline->isOn() );
}

//------------------------------------------------------- textItalic -------------------------------------
void QLightPad::textItalic()
{   if ( !currentEditor() )   return;
    currentEditor()->setItalic( m_actionTextItalic->isOn() );
}

//------------------------------------------------------- textFamily -------------------------------------
void QLightPad::textFamily( const QString &f )
{   if ( !currentEditor() )     return;
    currentEditor()->setFamily( f );
    currentEditor()->viewport()->setFocus();
}

//------------------------------------------------------- textSize -------------------------------------
void QLightPad::textSize( const QString &p )
{   if ( !currentEditor() )     return;
    currentEditor()->setPointSize( p.toInt() );
    currentEditor()->viewport()->setFocus();
}

//------------------------------------------------------- textStyle -------------------------------------
void QLightPad::textStyle( int i )
{   if ( !currentEditor() )             return;
    if ( i == 0 )        currentEditor()->setParagType( QStyleSheetItem::DisplayBlock,    QStyleSheetItem::ListDisc );
    else if ( i == 1 )   currentEditor()->setParagType( QStyleSheetItem::DisplayListItem, QStyleSheetItem::ListDisc );
    else if ( i == 2 )   currentEditor()->setParagType( QStyleSheetItem::DisplayListItem, QStyleSheetItem::ListCircle );
    else if ( i == 3 )   currentEditor()->setParagType( QStyleSheetItem::DisplayListItem, QStyleSheetItem::ListSquare );
    else if ( i == 4 )   currentEditor()->setParagType( QStyleSheetItem::DisplayListItem, QStyleSheetItem::ListDecimal );
    else if ( i == 5 )   currentEditor()->setParagType( QStyleSheetItem::DisplayListItem, QStyleSheetItem::ListLowerAlpha );
    else if ( i == 6 )   currentEditor()->setParagType( QStyleSheetItem::DisplayListItem, QStyleSheetItem::ListUpperAlpha );
    currentEditor()->viewport()->setFocus();
}

//------------------------------------------------------- textColor -------------------------------------
void QLightPad::textColor()
{   if ( !currentEditor() ) return;
    QColor col = QColorDialog::getColor( currentEditor()->color(), this );
    if ( !col.isValid() )       return;
    currentEditor()->setColor( col );
    QPixmap pix( 16, 16 );
    pix.fill( black );
    m_actionTextColor->setIconSet( pix );
}

//------------------------------------------------------- textAlign -------------------------------------
void QLightPad::textAlign( QAction *a )
{   if ( !currentEditor() )               return;
    if      ( a == m_actionAlignLeft )      currentEditor()->setAlignment( AlignLeft );
    else if ( a == m_actionAlignCenter )    currentEditor()->setAlignment( AlignHCenter );
    else if ( a == m_actionAlignRight )     currentEditor()->setAlignment( AlignRight );
    else if ( a == m_actionAlignJustify )   currentEditor()->setAlignment( AlignJustify );
}

//------------------------------------------------------- fontChanged -------------------------------------
void QLightPad::fontChanged( const QFont &f )
{   m_comboFont->lineEdit()->setText( f.family() );
    m_comboSize->lineEdit()->setText( QString::number( f.pointSize() ) );
    m_actionTextBold->setOn( f.bold() );
    m_actionTextItalic->setOn( f.italic() );
    m_actionTextUnderline->setOn( f.underline() );
}

//------------------------------------------------------- colorChanged -------------------------------------
void QLightPad::colorChanged( const QColor &c )
{   QPixmap pix( 16, 16 );
    pix.fill( c );
    m_actionTextColor->setIconSet( pix );
}

//------------------------------------------------------- alignmentChanged -------------------------------------
void QLightPad::alignmentChanged( int a )
{   if ( ( a == AlignAuto ) || ( a & AlignLeft )) m_actionAlignLeft->setOn( TRUE );
    else if ( ( a & AlignHCenter ) )              m_actionAlignCenter->setOn( TRUE );
    else if ( ( a & AlignRight ) )                m_actionAlignRight->setOn( TRUE );
    else if ( ( a & AlignJustify ) )              m_actionAlignJustify->setOn( TRUE );
}

//------------------------------------------------------- editorChanged -------------------------------------
void QLightPad::editorChanged( QWidget * )
{   MyEditText *edit = currentEditor();
    if ( !edit )                 return;
    bool state = edit->isModified();
    fontChanged(      edit->font() );
    colorChanged(     edit->color() );
    alignmentChanged( edit->alignment() );
    //m_actionAutoRet->setOn( (edit->wordWrap()==QTextEdit::WidgetWidth));
    edit->setModified(state);
}

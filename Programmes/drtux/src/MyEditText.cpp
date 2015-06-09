/********************************* MyEditText.cpp *********************************
 *                                                                                *
 *   #include "MyEditText.h"                                                      *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat à l'Energie Atomique                                            *
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

#include "MyEditText.h"
#include <qfiledialog.h>
#include <qimage.h>
#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qpixmap.h>
#include <qclipboard.h>
#include <qlistview.h>
#include <qcursor.h>
#include <qtimer.h>
#include <qtooltip.h>
#include <qtabwidget.h>
#include <qapplication.h>
#include <qtextstream.h>
#include <qfileinfo.h>

#include "CApp.h"

#include "../../drtux/src/C_DlgListFieldMngr.h"

#include "../../MedinTuxTools/CHtmlTools.h"
#include "../../MedinTuxTools/CGenTools.h"
#include "../../MedinTuxTools/CGestIni.h"
#include "C_Dlg_ImageProperty.h"
#include "drtux.h"

#define IS_HTML 23000


//====================================== CMyThemePopup =======================================================
CMyThemePopup::CMyThemePopup ( QWidget * parent, const char * name  )
    : ThemePopup ( parent ,  name  )
{
}
//---------------------------------- event -------------------------------------------------------------------
/*! \brief Interception des evenements souris pour emettre le signal: Sign_MouseOutPopup();  lorsque la souris est en dehors du ThemePopup   (sert à effacer la bulle dans MyEditText).
*/
bool CMyThemePopup::event ( QEvent * ev )
{
 if ( ev->type() == QEvent::MouseMove &&
      !QRect(QPoint(0,0), frameSize()).contains ( ((QMouseEvent*)ev)->pos() )
    ) emit Sign_MouseOutPopup();

 return ThemePopup::event (ev);
}

//====================================== CMyBulleWidget ==========================================================================
CMyBulleWidget::CMyBulleWidget ( const QString &text  ,    // texte de la bulle (peut être du HTML)
                                 int          wAdj    ,    // largeur de la zone à eviter (celle du popup)
                                 int             x    ,    // Position gauche de la zone à eviter (celui du popup)
                                 int             y    ,    // Position haute de la zone à eviter (celui du popup)
                                 QWidget    *parent   ,    // parent du widget
                                 const  char *name    ,    // nom du widget
                                 WFlags           f   ,    // flags du widget
                                 int          width   ,    // largeur de la bulle
                                 int            ,// hauteur de la bulle
                                 int           zoom        // zoom d'affichage
                                ) : QWidget( parent, name , f)
{
 m_pQTextEdit = new QTextEdit (this, name);
 if ( m_pQTextEdit)
    {m_pQTextEdit->zoomTo (zoom);
     //m_pQTextEdit->zoomTo (zoom);
     int deb = text.find("$start_title", 0,FALSE);
     int end = text.find("$end_title",   0,FALSE);
     if ( deb !=-1 && end != -1 )
        { deb += 12;
          m_pQTextEdit->setText(text.mid(deb, end-deb));
        }
     else
        { m_pQTextEdit->setText(text);
        }
     //............ ajuster les positions d'affichage ..............
     int h     = m_pQTextEdit->heightForWidth (width);
     int pos_x = x + wAdj;
     if (pos_x + width  >= parent->width())   pos_x =  x - width - 10;
     if (    y + h      >= parent->height())      y =  parent->height() - h - 10;
     m_pQTextEdit->setGeometry (0, 0, width, h+10);
     setGeometry (pos_x, y, width + 1, h + 10 + 1);

     //............ afficher ..............
     m_pQTextEdit->setPaletteBackgroundColor (QColor(255,249,140 ));
     setPaletteBackgroundColor (QColor(4,4,4 ));
     m_pQTextEdit->setWordWrap(QTextEdit::NoWrap);
     m_pQTextEdit->setReadOnly (TRUE );
     QString pathAppli =  CGestIni::Construct_PathBin_Module("drtux",  QFileInfo(qApp->argv()[0]).dirPath (true));
     m_pQTextEdit->mimeSourceFactory()->addFilePath(pathAppli + "Ressources/RubriquesTextes");
     m_pQTextEdit->mimeSourceFactory()->addFilePath(G_pCApp->m_PathGlossaire + "ImagesFolder");
     m_pQTextEdit->show();
     show();
    }
}

//====================================== MyEditText ==========================================================================
MyEditText::~MyEditText ()
{if (m_pQtextMenuBulle)
    {delete m_pQtextMenuBulle;
     m_pQtextMenuBulle = 0;
    }
}

//---------------------------------- InitParam -------------------------------------------------------------------
/*! \brief Initialise les paramètres nécessaires à la classe MyEditText */
void MyEditText::InitParam()
{            m_PathAppli        = CGestIni::Construct_PathBin_Module("drtux",  QFileInfo(qApp->argv()[0]).dirPath (true));
             m_pThemePopup      = 0;
             m_pQtextMenuBulle  = 0;
             m_State            = 1;
             m_style                   = "";
             m_DefaultBackgroundColor  = "";
             m_DefaultTextColor        = "";
             m_list_name               = "";
             m_pRetLastListInUse       = 0;
             /*m_WhatsThisItem         = "";*/
             setTextFormat(Qt::RichText);
             setWordWrap(QTextEdit::FixedPixelWidth);
             setWrapColumnOrWidth (793-37 );   //( corresponds A4 width  21*72/2.54 )
    //................ charger la BlackList ........................................
    QString black_str      = CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathGlossaire+"/ignoreList.txt");
    black_str              = black_str.replace('\r',"").stripWhiteSpace();
    m_black_list           = QStringList::split('\n',black_str);

    //............................ insertion des items supplémentaires ................................
    QTextEdit::mimeSourceFactory()->addFilePath(m_PathAppli     + "Ressources/RubriquesTextes");
    QTextEdit::mimeSourceFactory()->addFilePath(G_pCApp->m_PathGlossaire + "ImagesFolder");
    //......................... interligne des paragraphes le plus petit possible ..............................
    //                          comme celui des retours chariot
    QStyleSheet      *sheet  = styleSheet() ;
    QStyleSheetItem  *stylep = sheet->item("p");
    stylep->setDisplayMode(QStyleSheetItem::DisplayBlock);
    stylep->setMargin(QStyleSheetItem::MarginVertical, 0);
    m_Zoom = 0;
}

//---------------------------------- event -------------------------------------------------------------------
/*! \brief Surcharge de la fonction clavier de QTextEdit, afin de corriger l'anomalie de fonctionnement inversé de la touche enter et ctrl+enter.
 *   QKeyEvent( Type type, int key, int ascii, int state,
 *              const QString& text=QString::null, bool autorep=FALSE, ushort count=1 )
*/
bool MyEditText::event ( QEvent * ev )
{//if ( ev->type() == QEvent::MouseButtonRelease)
 //   {QMouseEvent *m = (QMouseEvent*)ev;
 //    int buton = m->button ();
 //   }
 //if (ev->type ()==QEvent::MouseMove)
 //   {QMouseEvent *m = (QMouseEvent*)ev;
 //    qDebug(tr("TXT X : ")+QString::number(m->x())+"  "+tr("Y : ")+QString::number(m->y()));
 //   }
 bool valToret;
 if ( ev->type() == QEvent::KeyPress )
    {QKeyEvent   *k = (QKeyEvent *)ev;
     int      state = k->state();
     int        car = k->ascii();
     int        key = k->key();
    // qDebug(QString::number(car));
    //.............. Control A .....................
    // if (state==Qt::ControlButton && car == 'A' || car == 'a')
    //    {selectAll();
    //     QKeyEvent keyEvent (QKeyEvent::None, 0, 0, 0,  k->text(),  0, 0 );
    //     keyEvent.accept();
    //     return QTextEdit::event (&keyEvent);
    //    }
     //............. inverser le fonctionnement de la TOUCHE ENTER................................
     //              car d'origine: ENTER        --> fin de paragraphe avec double interligne
     //                             ENTER + CTRL --> fin de ligne avec interligne normal
     //              et cela ne me va pas
     if (key==Qt::Key_Delete) // Qt::Key_Delete Qt::Key_Backspace Qt::Key_Clear
        { emit Signal_Key_Delete_Pressed(key);
        }
     else if (key==Qt::Key_Backspace)
        { emit Signal_Key_Backspace_Pressed(key);
        }
     else if (car==13 || car==10)
        {if (state!=Qt::ControlButton) state = Qt::ControlButton;
         else                          state = Qt::NoButton;
         QKeyEvent keyEvent (ev->type(), key, car, state,
                             k->text(),  k->isAutoRepeat(), k->count() );
         if ( k->isAccepted ()) keyEvent.accept();
         else                   keyEvent.ignore();
         if (hasSelectedText ())
            {emit Signal_Replace_Before();
             valToret =  QTextEdit::event (ev);
             emit Signal_Replace_After();
             return valToret;
            }
         return QTextEdit::event (&keyEvent);
        }   //endif (car==13)
     else if (state != Qt::ControlButton && car)
        { if (hasSelectedText ())
             {emit Signal_Replace_Before();
              valToret =  QTextEdit::event (ev);
              emit Signal_Replace_After();
              return valToret;
             }
        }
    }  //endif ( ev->type() == QEvent::KeyPress )
 return QTextEdit::event (ev);
}

//----------------------------------------- del -----------------------------------------------------------------
/*! \brief surcharge du SLOT del() pour emettre le signal 'Signal_Del()' afin de permettre quelques taches avant effacement d'un bloc
*/
void MyEditText::del()
{emit Signal_Del_Before();
 QTextEdit::del();
 emit Signal_Del_After();
}
//----------------------------------------- cut -----------------------------------------------------------------
/*! \brief surcharge du SLOT cut() pour emettre le signal 'Signal_Cut()' afin de permettre quelques taches avant effacement d'un bloc
*/
void MyEditText::cut()
{emit Signal_Cut_Before();
 QTextEdit::cut();
 emit Signal_Cut_After();
}

//----------------------------------------- paste -----------------------------------------------------------------
/*! \brief surcharge du SLOT paste() pour emettre le signal 'Signal_Paste()' afin de permettre quelques taches avant effacement d'un bloc
*/
void MyEditText::paste()
{emit Signal_Paste_Before();
 QTextEdit::paste();
 emit Signal_Paste_After();
}

//----------------------------------------- moveCursor -----------------------------------------------------------------
// ACTION: permet à notre classe d'acceder à la methode protégée: QTextEdit::moveCursor( (CursorAction)action,  select);
void MyEditText::moveCursor (int action, bool select )
{QTextEdit::moveCursor( (CursorAction)action,  select);
}

//----------------------------------------- createPopupMenu -----------------------------------------------------------------
/*! \brief Recherche lors de l'appel du menu contextuel la liste directement au dessus du curseur et crée le popup selon les droits de l'utilisateur. Les listes sont repérées par un code couleur bleu (#0000ff).
*/
QPopupMenu * MyEditText::createPopupMenu ( const QPoint &pos )   // CMyThemePopup
{m_ImageName                = "";
 m_ListItem                 = "";
 m_CurrentPopupItem         = "";
 CMyThemePopup *pThemePopup = new CMyThemePopup(0, "MyPopupMenu" );
 if (pThemePopup==0)            return 0;

 //............................... créer le Popup classique .........................................
 CMyThemePopup *popEdit = new CMyThemePopup(pThemePopup);
 pThemePopup->insertSeparator ();

 popEdit->insertSeparator ();
 popEdit->insertItem(   Theme::getIcon("PingouinsSimple.png" ),
                        tr("Bonjour de la part du DrTux"), this, SLOT( CouCou())      );

 if ( isReadOnly() )
    {   pThemePopup->insertItem( Theme::getIconForbidden(), tr(" Droits insuffisants pour modifier le document courant (verrouillé) "),   this, SLOT( Slot_PopupActionDumy()),       CTRL+Key_I );
        pThemePopup->insertSeparator ();
    }
 else
    {   popEdit->insertItem(    Theme::getIconUndo(),
                                tr("&Annuler"), this, SLOT( undo()), CTRL+Key_Z );
        popEdit->insertItem(    Theme::getIconRedo(),
                                tr("&Refaire"), this, SLOT( redo()), CTRL+Key_Y );
        popEdit->insertItem(    Theme::getIconCut(),
                                tr("&Couper"), this, SLOT( cut()),   CTRL+Key_X );
        popEdit->insertItem(    Theme::getIconCopy(),
                                tr("C&opier"), this, SLOT( copy()),  CTRL+Key_C );
        popEdit->insertItem(    Theme::getIconPaste(),
                                tr("Co&ller"), this, SLOT( paste()), CTRL+Key_V );
        popEdit->insertItem(    Theme::getIconSelectAll(),
                                tr("&Tout sélectionner"), this, SLOT( selectAll()), CTRL+Key_A );
        ConnectPopup(popEdit);
        pThemePopup->insertItem(Theme::getIcon("PingouinsSimple.png"), tr("&Edition"), popEdit);
    }

 //............................... chercher une zone de liste deroulante (bleue) ..........................
 //                               et  rajouter (si il y en a) les items de la liste
 QString keepLastList;
 if (! isReadOnly() )
    {QPoint qPoint = pos;
     m_list_name   = GetLastList(-1, -1, &qPoint).stripWhiteSpace ();     //chercher si une liste existe avant la position du curseur
     keepLastList  = m_list_name;
     pThemePopup->insertSeparator();
     GetListItem (pThemePopup, m_list_name );
     //............... si Listes à inclure d'office ..............................................
     GetSpecialRubList ( pThemePopup, m_TypeRubrique);
    }

 //............... si zone de liste proposer modification ..............................................
 if ( (!isReadOnly()) && m_list_name != "" && G_pCApp->IsThisDroitExist(G_pCApp->m_Droits,"glc"))
    {int id = 0;
     CMyThemePopup *forLists = new CMyThemePopup(pThemePopup);
     pThemePopup->insertSeparator ();

     forLists->insertSeparator ();
     forLists->insertItem(      Theme::getIcon("CreateList_Item.png" ),
                                tr("C&réer une liste déroulante avec le texte sélectionné"), this,
                                SLOT( PopupCreateList()),      CTRL+Key_R  );

     forLists->insertItem(      Theme::getIcon("AddList_Item.png" ),
                                tr("A&jouter un item avec le texte sélectionné"), this,
                                SLOT( PopupItemAddItem()),     CTRL+Key_J  );

     forLists->insertSeparator ();
     id = forLists->insertItem( Theme::getIcon("ToolGestList_Item.png" ),
                                tr("Gestionnaire de listes ..."),    this,
                                SLOT( ListManager( )),  CTRL+Key_L );
     forLists->setWhatsThis( id , keepLastList);

     id = forLists->insertItem( Theme::getIcon("ToolList_Item.png" ),
                                tr("Mise à jour du menu"),    this,
                                SLOT( UpdateMenu( )), CTRL+Key_U );
     forLists->setWhatsThis( id , keepLastList);
     id = forLists->insertItem( Theme::getIcon("ToolList_Item.png" ),
                                tr("Créer le cache entier"),    this,
                                SLOT( TotalCacheCreator( )), CTRL+Key_U );
     ConnectPopup(forLists);
     id = pThemePopup->insertItem(Theme::getIcon("ToolList_Item.png"), "Gestion des listes", forLists);
    }
 //........................ créer menus spécifiques aux rubriques et codes dans la classe de la rubrique ...............................................................
 QString      iconFile;
 QString      menuStringItem;
 QKeySequence accel;
 emit Sign_Get_Custom_Menu("$StartRequest", iconFile, menuStringItem, accel);
 if (menuStringItem.length())       pThemePopup->insertSeparator ();
 while (menuStringItem.length())
   {pThemePopup->insertItem(QPixmap( iconFile ),  menuStringItem,  this, SLOT( PopupItemExeCustomItem()),  accel  );
    emit Sign_Get_Custom_Menu("$NextRequest", iconFile, menuStringItem, accel);
   }

 //........................ créer menus spécifiques aux images ...............................................................
 m_ImageName = GetImageNameNearCursor();
 if (m_ImageName.length())
    {pThemePopup->insertSeparator ();
     pThemePopup->insertItem(Theme::getIcon("ic-flower3.png" ),  tr("Propriétés de l'image ... ") ,       this,  SLOT( PopupItemModifierImage()),  CTRL+Key_I  );
     pThemePopup->insertItem(Theme::getIcon("ImageSave.png" ),   tr("Sauvegarder l'image sous... ") ,     this,  SLOT( PopupItemSaveImage()),      CTRL+Key_I  );

     QString listName = G_pCApp->getListNameRubriqueMenu("ListImages");
     if (listName.length())
        { pThemePopup->insertSeparator ();
          GetListItem (pThemePopup, listName );
        }
    }
 //................. connecter le menu .................................................................
 ConnectPopup(pThemePopup);
 m_pThemePopup = pThemePopup;
 return  pThemePopup;
//  delete pThemePopup;
}

//------------------------------------------------- MyMoveCursor -----------------------------------------
void MyEditText::MyMoveCursor(int toMove)
{ m_toMove = toMove;
  QTimer::singleShot(10, this, SLOT(Slot_MyMoveCursor())); 
}
void MyEditText::Slot_MyMoveCursor()
{moveCursor (m_toMove);
}
//------------------------------------------------- GetImageNameNearCursor -----------------------------------------
QString MyEditText::GetImageNameNearCursor()
{    int ori_index;
     int ori_para;
     int deb_para;
     int deb_index;
     int old_deb_par;
     int old_deb_index;
     int old_end_par;
     int old_end_index;
     QString qs_text = "";
     QString imgName = "";
     //................. si positions non correctes ................................................
     //                  on utilise la position courante
     getCursorPosition (&ori_para, &ori_index );
     getSelection(&old_deb_par, &old_deb_index, &old_end_par, &old_end_index);

     //................. se positionner là où le boutton droit est appuyé ..........................
     setUpdatesEnabled( FALSE );                      // inhiber l'affichage (faut pas montrer la selection)

     //.................. faire bouger le curseur un coup en avant et arrière ...............
     //                   seul moyen pour que le curseur "prenne la fonte"
     //                   et de la noter avant de l'envoyer dans des zones inconnues
     moveCursor (QTextEdit::MoveBackward);  //MoveBackward, MoveForward
     moveCursor (QTextEdit::MoveForward);   //MoveBackward, MoveForward
     m_LastFont  = currentFont();            //relever la police de caractère à l'endroit de l'insertion
     m_LastColor = color();

     //.................. se mettre en fin de document .......................
     moveCursor (QTextEdit::MoveEnd);
     getCursorPosition (&deb_para, &deb_index );
     setSelection(deb_para, deb_index, ori_para, ori_index);
     qs_text = selectedText().remove("<!--StartFragment-->"); // recuperer le texte selectionné et virer le tag rajouter par 'selectedText()'
     removeSelection();                        // deselectionner
     setCurrentFont(m_LastFont);
     setColor(m_LastColor);
     setCursorPosition (ori_para, ori_index );
     imgName = CHtmlTools::checkIfImageAfter(qs_text);
     if (imgName.length() !=0 ) {setUpdatesEnabled( TRUE ); return imgName;}

     //.................. se mettre en debut de document .......................
     moveCursor (QTextEdit::MoveHome);            // chercher avant le curseur
     //....................... recuperer la position de début de document .................................
     //                        la recherche se fait en reculant ou avançant à partir du curseur texte
     getCursorPosition (&deb_para, &deb_index );
     setSelection(deb_para, deb_index, ori_para, ori_index);
     qs_text = selectedText().remove("<!--StartFragment-->"); // recuperer le texte selectionne et virer le tag rajoute par 'selectedText()'

     //........................ replacer tout comme avant ............................................
     if (old_deb_par != -1)
        {setSelection(old_deb_par, old_deb_index, old_end_par, old_end_index);
        }
     else
        {removeSelection();                        // deselectionner
         setCurrentFont(m_LastFont);
         setColor(m_LastColor);
        }
     // removeSelection();                        // deselectionner
     // setCurrentFont(m_LastFont);
     // setColor(m_LastColor);
     setCursorPosition (ori_para, ori_index );
     imgName = CHtmlTools::checkIfImageBefore(qs_text);

     setUpdatesEnabled( TRUE );
     return imgName;
}

//---------------------------------- SaveImage -----------------------------------------------------------------------------
/*! \brief sauvegarde l'image dont le nom est placé en entree  */
void   MyEditText::SaveImage(QString &imageName)
{
 QString        fn = "";
 QString  outParam = "";
 CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathDrTuxIni, outParam );
 CGestIni::Param_ReadParam( outParam, "Documents", "Path documents", &fn);
 if (!fn.endsWith("/") ) fn += '/' ;
 fn       += imageName;
 int pos   = fn.findRev('.');;
 if ( pos != -1) {fn = fn.left(pos) + ".png";}
 QString fileName  = QFileDialog::getSaveFileName(fn ,
                                          tr( "Fichiers IMAGE (*.jpg *.jpeg *.png *.PNG *.gif *.GIF *.JPEG *.JPG);;"
                                             "Tous les fichiers (*)" ),
                                          this,
                                         "save file dialog",
                                         tr("Choisissez le nom du fichier pour sauvegarder votre image") );
 if (fileName.length() == 0 ) return;
 CGestIni::Param_WriteParam( &outParam, "Documents", "Path documents", QFileInfo(fileName).dirPath ( TRUE ));
 CGestIni::Param_UpdateToDisk(G_pCApp->m_PathDrTuxIni, outParam);

 QPixmap retPixMap;
 if (G_pCApp->m_C_ImageList.getPixmap( imageName , retPixMap))
    {   QImageIO iio;
        QImage   im;
        im = retPixMap; // convert to image
        iio.setImage( im );
        iio.setFileName( fileName );
        iio.setFormat( "PNG" );
        iio.write();
     }
}
//--------------------------------- ImageProperty -------------------------------------------------------------
void MyEditText::ImageProperty(QString &imageName)
{C_Dlg_ImageProperty* dlg = new C_Dlg_ImageProperty(this, imageName, &G_pCApp->m_C_ImageList, this, "C_Dlg_ImageProperty");
 if (dlg==0) return;
 dlg->exec();
 delete dlg;
}
//------------------------------------------------- PopupItemModifierImage ----------------------------------------------------------------
void MyEditText::PopupItemModifierImage()
{ImageProperty(m_ImageName);
}
//------------------------------------------------- PopupItemSaveImage ----------------------------------------------------------------
void MyEditText::PopupItemSaveImage()
{SaveImage(m_ImageName);
}

//------------------------------------------------- Slot_PopupActionDumy ----------------------------------------------------------------
void MyEditText::Slot_PopupActionDumy()
{
}
//------------------------------------------------- PopupItemFusionItem ----------------------------------------------------------------
void MyEditText::PopupItemFusionItem ()
{
}

//------------------------------------------------- PopupItemAddItem ----------------------------------------------------------------
/*! \brief SLOT appelé par le popupmenu de la zone de texte lorsque l'utilisateur sélectionne : Ajouter un item à la liste.
 * Cette fonction crée donc, s'il n'existe pas déjà l'item renseigné par l'utilisateur au sein de la liste en cours. En dernier lieu, efface le répertoire de cache pour forcer sa mise à jour.
*/
void MyEditText::PopupItemAddItem ()
{m_list_name        = GetLastList().stripWhiteSpace();
 QString pathImg    = Theme::getPath();
 if (m_list_name.length()<=0)
    {CGenTools::MessageAlert(this, tr("ERREUR !"),tr("Pas de liste déroulante trouvée<br>dans les 5 lignes précédentes."), tr("Annuler"));
     return;
    }
 int ret  = 150;         // ret = 100 pour annuler
 QString  path  = G_pCApp->m_PathGlossaire  + INSERTION_ROOT; // = G_pCApp->m_PathGlossaire + INSERTION_ROOT + QDir::separator() + GetPathItem (m_pQListViewItem);
 if ( !IsThisFolderExist(path, m_list_name ))
    {QString mess  = tr("<center><img source=\"./AlertLogoViergeAttention.png\"></center><p>La liste déroulante :<br><b>");
             mess += path + QDir::separator() + "<font color=\"#e80d0d\">" + m_list_name + "</font></b><br>";
             mess += tr("n'existe pas. Veuillez la créer avant d'y rajouter un item" );
     ret           = CGenTools::MyMessageAlert(this, tr("Erreur : liste déroulante"), pathImg, mess, tr("Annu&ler"), tr("Créer la liste")); // ret == 100 pour annuler 0 pour fenetre fermée
   }
 if (ret ==200)
    {QDir dir;
     if (dir.mkdir (path + QDir::separator()  + m_list_name)==FALSE)
        {QString mess  = tr("<center><img source=\"./AlertLogoViergeAttention.png\"></center><p>La liste déroulante : <br><b>");
                 mess += path + QDir::separator() + "<font color=\"#e80d0d\">" + m_list_name + "</font></b><br>";
                 mess += tr("n'a pu être créée.<br> Vérifiez si vous avez les droits en écriture,<br> Dans le répertoire : <br><b>");
                 mess += "<font color=\"#e80d0d\">" + path + "</font></b>";
         CGenTools::MyMessageAlert(this, tr("ERREUR : Création d'une liste"), pathImg, mess, tr("Annu&ler"));
         ret=100;    // annuler
        }
    }

 if (ret==100||ret==0) return;

 path += QDir::separator() + m_list_name;
 QString qsName = selectedText().stripWhiteSpace();
 CHtmlTools::HtmlToAscii( qsName, CHtmlTools::StrictASCII );
 if (qsName.length()<=0) qsName = tr("Élément nouveau");


 if (CGenTools::DialogGetString(this, tr("Ajouter un item à la liste : ") + m_list_name, qsName) ==  QDialog::Accepted)
    {qsName=qsName.stripWhiteSpace();
     if (qsName.length()==0)
        {CGenTools::MessageAlert(this, tr("ATTENTION !"),tr("Le nom du fichier est vide.<br>Veuillez donner un nom."), tr("Annu&ler"));
         return;
        }

     if (IsThisFileExist(path + QDir::separator() + qsName) )
        {CGenTools::MessageAlert(this, tr("ATTENTION !"),tr("Ce fichier existe déjà.<br>Veuillez choisir un autre nom"), tr("Annuler"));
         return;
        }
     QFile qfile(path+QDir::separator() + qsName.stripWhiteSpace());
     if (qfile.open(IO_ReadWrite)==FALSE)
        {QString mess  = tr("<center><img source=\"./AlertLogoViergeAttention.png\"></center><p>Le fichier correspondant au nouvel item :<br><b>");
                 mess += path + QDir::separator() + "<font color=\"#e80d0d\">" + qsName + "</font></b><br>";
                 mess += tr("n'a pu être créé.<br> Vérifiez si vous avez les droits<br>en écriture dans le répertoire : <br><b>");
                 mess += "<font color=\"#e80d0d\">" + path + "</font></b>";
         CGenTools::MyMessageAlert(this, tr("ERREUR : Ajout d'un Item "), pathImg, mess, tr("Annu&ler"));
        }
    }
  // Force la mise à jour du cache
  deleteCache();
}

//------------------------------------ CreateList --------------------------------------------------
/*! \brief Crée à partir du texte sélectionné une nouvelle liste déroulante (zone bleue) et efface le répertoire cache pour forcer la mise à jour des listes déjà en cache. */
void MyEditText::PopupCreateList()
{   QString qsName     = selectedText().stripWhiteSpace();
    QString pathImg    = Theme::getPath();
    CHtmlTools::HtmlToAscii( qsName, CHtmlTools::StrictASCII );

    if (CGenTools::DialogGetString(this, tr("Veuillez donner le nom de cette nouvelle liste déroulante"), qsName) ==  QDialog::Accepted)
       {qsName = qsName.stripWhiteSpace();
        if (qsName.length()==0)
           {CGenTools::MessageAlert(this, tr("ATTENTION !"),tr("Le nom de cette nouvelle liste déroulante est vide.<br>Veuillez choisir un nom"), tr("Annu&ler"));
            return;
           }
        QString pathListes = G_pCApp->m_PathGlossaire + INSERTION_ROOT;
        //........... créer si besoin la liste à laquelle se réfère la réference....................
       if ( IsThisFolderExist(pathListes, qsName) )
          {QString  mess  = tr("<center><img source=\"./AlertLogoViergeAttention.png\"></center><p>La liste déroulante :<br><b>");
                    mess += pathListes + QDir::separator() + "<font color=\"#e80d0d\">" + qsName + "</font></b><br>";
                    mess += tr(" Existe déjà !!.");
           CGenTools::MyMessageAlert(this, tr("ERREUR : Création d'une liste"), pathImg, mess, tr("Annu&ler"));
           return;
          }
        QDir dir;
        if (dir.mkdir (pathListes + QDir::separator() + qsName)==FALSE)
           {QString mess  = tr("<center><img source=\"./AlertLogoViergeAttention.png\"></center><p>La liste déroulante : <br><b>");
                    mess += pathListes + QDir::separator() + "<font color=\"#e80d0d\">" + qsName + "</font></b><br>";
                    mess += tr("n'a pu être créée.<br> Vérifiez si vous avez les droits<br>en écriture dans le répertoire : <br><b>");
                    mess += "<font color=\"#e80d0d\">" + pathListes + "</font></b>";
            CGenTools::MyMessageAlert(this, tr("ERREUR : Création d'une liste"), pathImg, mess, tr("Annu&ler"));
            return;
          }
        QColor old_col = color();
        if (hasSelectedText()) del();
        setColor( QColor(0x00,0x00,0xFF) );
        insert(qsName);
        setColor( old_col );
    }
  deleteCache();
}

//------------------------------------------------- PopupItemExeCustomItem ---------------------------------------
void MyEditText::PopupItemExeCustomItem ()
{//QString command("{{Enregistrer comme un document type}}");
 emit Sign_ExeCommand(m_CurrentPopupItem);
}

//------------------------------------ IsThisFileExist --------------------------------------------------
/*! \brief Teste si le fichier passé en paramètre existe */
bool MyEditText::IsThisFileExist(const char* path )
{QDir dir;
 return dir.exists(path );
}

//------------------------------- setText --------------------------------------------------
/*! \brief Injecte le texte ptr en gérant l'encodage
 *  \todo Gérer en fonction du type de document pour enlever des CMDI les fonctions de travail de texte (documents, prescriptions).
*/
void MyEditText::setText ( const char* ptr )
{
 // char *p = strstr(ptr,"</HTML_Data>");
 // if (p) *p=0;
 if (CGestIni::IsUtf8( ptr ))
    {
     setText(QString::fromUtf8 ( ptr ));
    }
 else
    {setText( QString(ptr) );
    }
}

//-------------------------------- setText ---------------------------------------------
/*! \brief Injecte dans le QTextEdit le texte passé en paramètre.
*  \todo Gérer en fonction du type de document pour enlever des CMDI les fonctions de travail de texte (documents, prescriptions).
*/
void MyEditText::setText ( const QString & txt )
{setUpdatesEnabled (FALSE );
 hide();
 QString path(m_PathAppli + "Ressources" + QDir::separator() + "RubriquesTextes");
 if (mimeSourceFactory()->filePath().findIndex (path)==-1) mimeSourceFactory()->addFilePath(path);
 m_style                   = CHtmlTools::getTextDefaultAttribut(txt, "style");
 m_DefaultBackgroundColor  = CHtmlTools::getTextDefaultAttribut(txt, "bgcolor");
 m_DefaultTextColor        = CHtmlTools::getTextDefaultAttribut(txt, "text");
 if (m_DefaultBackgroundColor.length()>0) setPaper(QBrush( QColor(m_DefaultBackgroundColor)));
 else                                     setPaper(QBrush( QColor("#FFF")));
 QFont      font;
 //........ il semblerait que positionner la fonte avant le texte ...............
 //         fait planter dans certaines conditions ???
 if (CHtmlTools::QT_style_To_QFont(m_style , font, "Arial","10")) setFont(font);
 QTextEdit::setText(txt);
 if (m_Zoom) zoomTo (m_Zoom);
 setUpdatesEnabled (TRUE );
 if (m_State) show();
}

/*! \brief cette fonction renvoie le texte à insérer dans le CRubRecord. Pour récupérer le texte affiché uniquement utilisez text().
 *  \param CRubRecord* : sera nécessaire s'il s'agit d'une ordonnance structurée.
 *  Cette fonction calcule directement, selon le type de rubrique en cours, le texte à insérer dans la base de données.
*/
QString MyEditText::getText( CRubRecord* pCRubRecord /*=0 */)
// #define TYP_IDENTITE              1
// #define TYP_CHOIX_PATIENT         2
// #define TYP_PARAM                 5
// #define TYP_TERRAIN               20060000	NON GÉRÉ PAR CETTE CLASSE
// #define TYP_DOCUMENTS             20080000	OK
// #define TYP_OBSERVATION           20030000	OK
// #define TYP_PRESCRIPTION          20020000   OK
// #define TYP_GROUPEDVIEW	     19840000
//
//
//
// //...................... nouveaux sous types ...........................................................
// #define TYP_ORDO_CALC_CURENT      20099999  // pour que date ordo soit la date courante lors appel DrTux::AddNewRecordToRubrique()
//
// #define TYP_ORDO_CALC             20020100
// #define TYP_ORDONNANCE            20020200
// #define TYP_CERTIFICAT            20020300
// #define TYP_COURRIER              20020500
// #define TYP_URG_OBSERV            20040000
{
// qWarning("MyEditText::getText() "+QString::number(m_TypeRubrique));
 switch (m_TypeRubrique)
  { case TYP_OBSERVATION :
	return QTextEdit::text();
	break;

    case TYP_PRESCRIPTION :
	return QTextEdit::text();
	break;

    case TYP_ORDO_LAP :
	if (pCRubRecord) return getText_IfTypeIsOrdoLap(pCRubRecord);
	break;

    case TYP_ORDO_CALC :
	if (pCRubRecord) return getText_IfTypeIsOrdoCalc(pCRubRecord);
	break;

    case TYP_DOCUMENTS :
	return getText_IfTypeIsDocuments();
	break;

  }
  return TR("Erreur");
}

QString MyEditText::getText_IfTypeIsOrdoLap(CRubRecord* pCRubRecord)
{// Attention il faut d'abord relire le rubrecord pour récupérer la structure car elle n'apparait pas dans le QTextEdit
// qWarning ("MyEditText::getText_IfTypeIsOrdoCalc");
 QString              data;
 QString              stringStruct;
 QString              stringDST;

 G_pCApp->m_pCMoteurBase->GetDataFromRubList(data, pCRubRecord);
//  ptr        = (char*)data;

 if (CGestIni::IsUtf8( data ))
    {  CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData(QString::fromUtf8 ( data ), 0, &stringStruct);    // y isoler et recuperer les données calculables
    }
 else  CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData( data , 0, &stringStruct);

 stringDST       = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\" ?>\r\n<ordotext>\r\n";
 stringDST.append (QTextEdit::text());           // ajouter le texte modifé
 stringDST.append ( "\r\n</ordotext>\r\n\r\n<ordoMedicaStruct>");
 stringDST.append (stringStruct);                    // ajouter la structure calculable non modifiée
 stringDST.append ( "</ordoMedicaStruct>\r\n");
 return stringDST;
}

QString MyEditText::getText_IfTypeIsOrdoCalc(CRubRecord* pCRubRecord)
{// Attention il faut d'abord relire le rubrecord pour récupérer la structure car elle n'apparait pas dans le QTextEdit
// qWarning ("MyEditText::getText_IfTypeIsOrdoCalc");
 QString              data;
 QString              stringStruct;
 QString              stringDST;

 G_pCApp->m_pCMoteurBase->GetDataFromRubList(data, pCRubRecord);
//  ptr        = (char*)data;

 if (CGestIni::IsUtf8( data ))
 {  CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData(QString::fromUtf8 ( data ), 0, &stringStruct);    // y isoler et recuperer les données calculables
 }
 else  CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData( data , 0, &stringStruct);

 stringDST       = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\" ?>\r\n<ordotext>\r\n";
 stringDST.append (QTextEdit::text());           // ajouter le texte modifé
 stringDST.append ( "\r\n</ordotext>\r\n\r\n<ordoMedicaStruct>");
 stringDST.append (stringStruct);                    // ajouter la structure calculable non modifiée
 stringDST.append ( "</ordoMedicaStruct>\r\n");
 return stringDST;
}

QString MyEditText::getText_IfTypeIsDocuments(int *len /*=0*/)
{
// qWarning ("MyEditText::getText_IfTypeIsDocuments");
QString     imgName;
 QString     text = QTextEdit::text();
 QString     encodedData, html_Data;
 QStringList imgListAlreadyDump;
 QPixmap     retPixMap;
 QString     imageFile;
 //................. cette version ne place que les images de la liste ....................
 //                  auquelles fait référence le texte
 long       from   = 0;
 long    deb_tag   = 0;
 while (from>=0 && from < (long)text.length() )
    {deb_tag     = CHtmlTools::IsThisTagImage(text, from , &imgName );   // rechercher le prochain tag image du texte
     if (deb_tag>=0                                   &&                 // doit etre trouvé un tag image dans le texte
         imgListAlreadyDump.findIndex(imgName)==-1    &&                 // ne doit pas déjà avoir été dumpé (peut  y avoir plusieurs références à la même image)
         G_pCApp->m_C_ImageList.getPixmap( imgName , retPixMap, &imageFile)                   // doit exister dans la liste des images
        )
        { imgListAlreadyDump.append(imgName);                       // placer dans la liste des images déjà dumpées
          encodedData.append("      <name>\r\n          ");
          encodedData.append(imgName+"\r\n");
          encodedData.append("      </name>\r\n");
          encodedData.append("      <fileName>\r\n          ");
          encodedData.append(imageFile+"\r\n");
          encodedData.append("      </fileName>\r\n");
          encodedData.append("      <ext_Type>\r\n          ");
          encodedData.append("PNG\r\n");
          encodedData.append("      </ext_Type>\r\n");
          encodedData.append("      <base64>\r\n");
          encodedData.append(encodePixmapToBase64(retPixMap)+"\r\n");
          encodedData.append("      </base64>\r\n");
        }
     //<img height=400 src=Image-0001h400w500 width=500 >
    } //end while (from>=0 && from<txt.length())     essai2.html
 encodedData.append("    </ImageListe>\r\n");
 encodedData.append("</MedinTux_Multimedia_data>\r\n");
 //.................. le construire à la fin car les noms ..................
 //                   des images sont modifiés
 html_Data.append("<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\" ?>\r\n<MedinTux_Multimedia_data>\r\n");
 html_Data.append("    <HTML_Data>\r\n");
 html_Data.append(text);
 html_Data.append("    </HTML_Data>\r\n");
 html_Data.append("    <ImageListe>\r\n");
 encodedData.prepend(html_Data);
 if (len) *len = encodedData.length();
 return encodedData;
}


//----------------------------- decodeStringTo_pQPixmap -----------------------------------
/*! \brief Fonction qui décode un texte base64 en image...
 *  \param in : QCString ou QByteArray ou QString qui contient l'image encodée en base64
 *  \return Renvoie un pointeur vers le QPixmap décodé.
*/
QPixmap *MyEditText::decodeStringTo_pQPixmap( const QString& in )
{ QByteArray ba_dec;
  QCString base64encoded = QCString( in );
  QCodecs::base64Decode( base64encoded , ba_dec);
  return new QPixmap ( ba_dec );
}

//----------------------------- decodeStringTo_QPixmap -----------------------------------
/*! \brief Fonction qui décode un texte base64 en image...
 *  \param in : QCString ou QByteArray ou QString qui contient l'image encodée en base64
 *  \return Renvoie un pointeur vers le QPixmap décodé.
*/
QPixmap MyEditText::decodeStringTo_QPixmap( const QString& in )
{ QByteArray ba_dec;
  QCString base64encoded = QCString( in );
  QCodecs::base64Decode( base64encoded , ba_dec);
  return QPixmap ( ba_dec );
}

//----------------------------- encodePixmapToBase64 -----------------------------------
/*! \brief Encode en base64 la QPixmap passée en paramètre */
QString  MyEditText::encodePixmapToBase64(const QPixmap& pixIn)
{ QByteArray ba_enc;
  QBuffer buffer( ba_enc );
  buffer.open( IO_WriteOnly );
  pixIn.save( &buffer, "PNG" ); // writes pixmap into ba in PNG format
  return QString ( QCodecs::base64Encode( ba_enc , TRUE) );
}
//----------------------------- encodePixmapToBase64 -----------------------------------
/*! \brief Encode en base64 la QPixmap passée en paramètre */
QString  MyEditText::encodePixmapToBase64(QPixmap* pQPixmap)
{ if (pQPixmap==0) return "";
  QByteArray ba_enc;
  QBuffer buffer( ba_enc );
  buffer.open( IO_WriteOnly );
  pQPixmap->save( &buffer, "PNG" ); // writes pixmap into ba in PNG format
  return QString ( QCodecs::base64Encode( ba_enc , TRUE) );
}


//------------------------------------------------- CouCou ----------------------------------------------------
/*! \brief Affiche un popup amusant */
void MyEditText::CouCou()
{G_pCApp->CouCou( "", "$Theme/MedinTux.png", 100);
}

//------------------------------------- GetSpecialRubList -----------------------------------------------------
/*! \brief cree la liste affectee aux rubriques celles presentes dans la section  [MenuContextuel] du fichier.ini
 */
void MyEditText::GetSpecialRubList (   ThemePopup *pThemePopup, int rubType)
{QString supList = G_pCApp->getListNameRubriqueMenu(rubType);
 if (supList.length())
    { pThemePopup->insertSeparator ();
      GetListItem (pThemePopup, supList );
    }
}

//------------------------------------- DoContextListPopup -----------------------------------------------------
/*! \brief Crée le menu multi-niveau selon le type de document en cours.
 */
int MyEditText::DoContextListPopup(QString *pListInUse /*=0*/)
{m_pRetLastListInUse = pListInUse;
 if ( isReadOnly() )    return 0;
 m_ListItem             =  "";
 CMyThemePopup *pThemePopup = new CMyThemePopup(0, "MyPopupMenu" );
 if (pThemePopup==0)    return 0;

 //............................... chercher une zone de liste deroulante (bleue) .....................
 m_list_name = GetLastList();     //chercher si une liste existe cinq lignes avant le curseur texte
 QString keepLastList = m_list_name;

 //............... rajouter (si il y en a) les items de la liste .......................................
 GetListItem (pThemePopup, m_list_name );
 //............... si Listes à inclure d'office ..............................................
 GetSpecialRubList ( pThemePopup, m_TypeRubrique);
 //............... si zone de liste proposer modification ..............................................
 if (G_pCApp->IsThisDroitExist(G_pCApp->m_Droits, "glc") )
    { CMyThemePopup *forLists = new CMyThemePopup(pThemePopup);
      forLists->insertSeparator ();
      int id = forLists->insertItem( Theme::getIcon("ToolGestList_Item.png" ),
                                        tr("Gestionnaire de listes ..."),    this,
                                        SLOT( ListManager( )),  CTRL+Key_L );
      forLists->setWhatsThis( id , keepLastList);

      id = forLists->insertItem( Theme::getIcon("ToolList_Item.png" ),
                                        tr("Mise à jour du menu"),    this,
                                        SLOT( UpdateMenu( )), CTRL+Key_U );
      forLists->setWhatsThis( id , keepLastList);
      id = forLists->insertItem( Theme::getIcon("ToolList_Item.png" ),
                                        tr("Créer le cache entier"),    this,
                                        SLOT( TotalCacheCreator( )), CTRL+Key_U );
      ConnectPopup(forLists);
      id = pThemePopup->insertItem(Theme::getIcon("ToolList_Item.png"), "Gestion des listes", forLists);
    }
 //................. connecter le menu .................................................................
 ConnectPopup(pThemePopup);
 m_pThemePopup = pThemePopup;
 pThemePopup->exec(QCursor::pos());
 delete  pThemePopup;
 return 1;
}

//----------------------------------------- ConnectPopup -----------------------------------------------------------------
/*! \brief Recherche en arriere du curseur une zone de liste bleue #0000ff et si trouvée la retourne.
 * \param *pThemePopup: ThemePopup poineur sur le menu à connecter
*/

void MyEditText::ConnectPopup(ThemePopup *pThemePopup )
{     connect( pThemePopup, SIGNAL( activated( int)),      this,        SLOT(OnMenuActivated( int)) );
      connect( pThemePopup, SIGNAL( highlighted ( int)),   this,        SLOT(OnMenuHighlighted( int)) );
      connect( pThemePopup, SIGNAL( aboutToHide ()),       this,        SLOT(OnMenuAboutToHide ()) );
      connect( pThemePopup, SIGNAL( Sign_MouseOutPopup()), this,        SLOT(TimedDestroyMenuBulle()) );
}

//----------------------------------------- GetLastList -----------------------------------------------------------------
/*! \brief Recherche en arriere du curseur une zone de liste bleue #0000ff et si trouvée la retourne.
 * \param *pos: QPos = position du curseur texte à partir duquel doit se faire la recherche si zero alors la recherche se fait à partir des valeurs de:
 * \param ori_index:  int, qui si -1 la recherche se fait à partir de pos qui si zero se fera à partir de la position courante.
 * \param ori_para:  int, qui si -1 la recherche se fait à partir de pos qui si zero se fera à partir de la position courante.
 * \return Retourne le nom de la zone de liste sinon une chaine vide ""
*/
QString MyEditText::GetLastList( int ori_index /*=-1*/,  int ori_para /*=-1*/, QPoint *pos /*=0*/)
{//.................. recuperer position de depart .............................................
 //int ori_index;
 //int ori_para;
 int deb_para;
 int deb_index;
 int old_deb_par;
 int old_deb_index;
 int old_end_par;
 int old_end_index;
 //................. si positions non correctes ................................................
 //                  on utilise la position courante
 if ( (ori_index==-1|| ori_para==-1) && pos==0) getCursorPosition (&ori_para, &ori_index );

 //................. sauvegarder l'ancienne selection ..........................................
 getSelection(&old_deb_par, &old_deb_index, &old_end_par, &old_end_index);
 //................. se positionner là où le boutton droit est appuyé ..........................
 setUpdatesEnabled( FALSE );                      // inhiber l'affichage (faut pas montrer la selection)

 if (pos)
    {placeCursor (*pos);                          // placer le curseur texte sur position X et Y
     ori_index     = charAt(*pos, &ori_para);     // recuperer index et paragraphe de cette position
    }
 //.................. faire bouger le curseur un coup en avant et arrière ...............
 //                   seul moyen pour que le curseur "prenne la fonte"
 //                   et de la noter avant de l'envoyer dans des zones inconnues
 moveCursor (QTextEdit::MoveBackward);  //MoveBackward, MoveForward
 moveCursor (QTextEdit::MoveForward);   //MoveBackward, MoveForward
 m_LastFont  = currentFont();            //relever la police de caractère à l'endroit de l'insertion
 m_LastColor = color();
 //.................. se mettre en debut de document .......................
 moveCursor (QTextEdit::MoveHome);
 //....................... recuperer la position de début de document .................................
 //                        la recherche se fait en reculant à partir du curseur texte
 getCursorPosition (&deb_para, &deb_index );
 if (old_deb_par != -1)   // Si une selection existe: la recherche se fait AVANT (afin d'eviter d'eventuels mots clefs inside)
     setSelection(deb_para, deb_index, old_deb_par, old_deb_index);
 else                     // Sinon on recherche le mot clef à partir du curseur texte
     setSelection(deb_para, deb_index, ori_para, ori_index);
 QString qs_text = this->selectedText();                     // recuperer le texte selectionné
 //........................ replacer tout comme avant ............................................
 if (old_deb_par != -1)
    {setSelection(old_deb_par, old_deb_index, old_end_par, old_end_index);
    }
 else
    {removeSelection();                        // deselectionner
     setCurrentFont(m_LastFont);
     setColor(m_LastColor);
    }
 if (pos) placeCursor (*pos);                  // replacer le curseur à son emplacement d'origine
 else     setCursorPosition (ori_para, ori_index );


 //....................... analyse des lignes precedentes ......................................
 int f                  = qs_text.findRev("color:#0000ff");       // chercher debut du bleu en partant de la fin
 if (f != -1)
    {qs_text            =  qs_text.mid(f+13);
     f                  =  qs_text.find("</span>");              // chercher fin du bleu
     if (f!=-1) qs_text =  qs_text.left(f);
     long    len        =  qs_text.length();
     char *p_txt        =  new char[len+1];
     char *pt           =  p_txt;
     strcpy(p_txt, qs_text);
     if (pt)
        {qs_text = "";
         p_txt[len]=0;
         while(*pt && *pt!='>') ++pt;  //aller en fin du 1er tag color:# etc ...
         if (*pt=='>') ++pt;
         while(*pt)
             {//........... si debut de tag le sauter ..........................
              if (*pt=='<')
                 {++pt;
                  while(*pt && pt[0]!='/' && pt[1]!='>')  ++pt;     // aller en fin de tag
                  while (pt[0]=='/' || pt[0]=='>')++pt;             // sauter les marques de fin
                 }
              else
                 {qs_text += pt[0];
                  ++pt;
                 }
             }
          delete []p_txt;
        }
     else
       {qs_text = "";
       }
    } // end if (int f = g_text.findRev("color:#0000ff") !=-1)
 else
    {qs_text = "";
    }
 setUpdatesEnabled( TRUE );      // retablir l'affichage
 qs_text.replace("&nbsp;"," ");
 return qs_text.replace((char)0xa0," ");
}



//----------------------------------------- PasteListItem ------------------------------------------------------
/*! \brief Fonction appelée par le menu déroulant des listes lorsqu'est activé un ITEM quelqu'il soit.
 * 1) récupère la(es) liste(s) et l'item sélectionnés \n
 * 2) récupère le type de liste (TYP_OBSERV...) \n
 * 3) insère l'item selon son type (liste, item complexe ou item simple.\n
 * \sa PasteFolderItem
*/
void MyEditText::PasteListItem ( )
{ // Récupère la(es) liste(s) et l'item
  //QString copyOf_m_ListItem = m_ListItem;     // "Destination{&}Hospitalisation{&}�Sce des Urgences"
  int pos_ext               = m_ListItem.findRev('.');
  int pos                   = m_ListItem.find("#");
  if (pos!=-1)   m_ListItem = m_ListItem.mid(pos+1);
  QStringList        fields = QStringList::split( HISTORIQUE_SPLITTER, m_ListItem , false);
  int              nbfields = fields.count();
  // regarde s'il existe une chaîne de liste avant l'item
  if (nbfields > 2) // oui
     { int i = 0;
       do { i++;
            m_ListItem = fields[i];
            PasteFolderItem();
          }
          while (i != (nbfields - 2));
     }  // testé = OK
  // int type_liste;
  // récupère la liste et l'item
  // Avec la méthode des listes imbriquées il n'est plus nécessaire de tester le type de liste
  // puisque whatsThis de l'item contient la liste mère.
  if (nbfields >= 2)
     { m_ListItem  = fields[nbfields-1];
       m_list_name = fields[nbfields-2];
     }
  // Teste si l'item est textuel ou un simple
  if (pos_ext != -1)  // c'est un item textuel
   {    // teste l'existence du fichier avant d'appeler la fonction
        QString path = G_pCApp->m_PathGlossaire  + INSERTION_ROOT + QDir::separator() + m_list_name + QDir::separator() + m_ListItem.prepend((pos!=-1)?"#":"");
        if (QFile::exists ( path ) )  // reste à tester la taille du fichier
           { PasteTextItem(""); }
        else
           {    // force la mise à jour du cache de la liste
                m_ListItem = fields[0];  // liste mère
                UpdateMenu();
           }
   }
  else   // c'est un item simple (soit folder, soit fichier)
   { pos_ext                   = m_ListItem.find(ITEM_IS_FOLDER);
     if (pos_ext == -1)  // c'est un item (testé OK)
       {  // insère l'item même s'il n'existe plus sur le disque
          setCurrentFont(m_LastFont);
          setUnderline(FALSE);
          if (m_LastColor == QColor(0x00,0x00,0xFF)) setColor(QColor(0x00,0x00,0x00));
                                             else    setColor(m_LastColor);
          insert(QString(" ") + m_ListItem + " ");
          //if (noStyle)  specialInsert(text);
          //else          CHtmlTools::insertHtml(this, text);

          // teste l'existence du fichier pour voir s'il faut forcer la MAJ du cache
          QString path = G_pCApp->m_PathGlossaire  + INSERTION_ROOT + QDir::separator() + m_list_name + QDir::separator() + m_ListItem;
          if (!QFile::exists ( path ) )  // reste à tester la taille du fichier
               { // force la mise à jour du cache de la liste
                 m_ListItem = fields[0];  // liste mère
                 UpdateMenu();
               }
        }
     else  // c'est une liste (ITEM_IS_FOLDER)
       { m_ListItem = m_ListItem.left(pos_ext) ;
         PasteFolderItem();
       }
   }
}

//----------------------------------------- PasteFolderItem -----------------------------------------------------
/*! \brief Insère dans le texte, le texte de l'item du popup selectionné (normalement nom d'une reference à une liste) en le mettant en bleu. Vérifie au préalable qu'un item de remplacement du nom de la liste ne soit définit. S'il l'est alors insère le contenu du fichier.
 * Attention cette fonction ne doit être appelée qu'avec des m_ListItem simples (juste la liste à insérer, pas l'intégralité du whatThis des items). \n
 * soit ce fichier n'existe pas et c'est l'item qui est inséré dans le texte. \n
 * Le nom de la liste à insérer est stockée dans m_ListItem.
*/
void MyEditText::PasteFolderItem ( )
{
 int Item_Compose = m_ListItem.find(HISTORIQUE_SPLITTER);
 if (Item_Compose == -1)        // test si m_ListItem est un item simple (pour être certain !!!)
  {  // teste l'existence du fichier FILE_NAME_REPLACOR
     QString path = "";
     path               = G_pCApp->m_PathGlossaire  + INSERTION_ROOT + QDir::separator() + m_ListItem;
     QDir dir ( path );
     // teste l'existence de la liste sur le disque dur (nécessaire à cause du fichier cache...
     if (dir.exists())
        {   setCurrentFont(m_LastFont);
            setUnderline(FALSE);
            if (m_LastColor == QColor(0x00,0x00,0xFF)) setColor(QColor(0x00,0x00,0x00));
            else                                       setColor(m_LastColor);
            insert(QString(" "));
            setColor(QColor(0x00,0x00,0xFF));
            insert(m_ListItem);
            if (m_LastColor == QColor(0x00,0x00,0xFF)) setColor(QColor(0x00,0x00,0x00));
            else                                       setColor(m_LastColor);
            insert(QString(" "));
        }
  }
}


//----------------------------------------- ListManager ----------------------------------------------------
/*! \brief Ouvre la boîte de dialogue du Gestionnaire des listes déroulantes.
 * \todo à pluginiser...
 */
void MyEditText::ListManager()
{C_DlgListFieldMngr *dlg = new C_DlgListFieldMngr(0,"ListManager_Dial",TRUE, WStyle_Customize | WStyle_NormalBorder | WStyle_Title |WStyle_MinMax | WStyle_SysMenu);
 if (dlg ==0) return;
 dlg->initDialog(G_pCApp->m_DrTuxParam, G_pCApp->m_PathGlossaire, G_pCApp->m_PathAppli);
 QFont font;
 if (CHtmlTools::QT_style_To_QFont(m_style , font, "Arial","10")) dlg->setFontEdit(font);
  //........ il semblerait que positionner la fonte avant le texte ...............
 //         fait planter dans certaines conditions ???
 dlg->selectAndOpenItem(m_ListItem);
 if (m_pRetLastListInUse) dlg->setReturnValList(m_pRetLastListInUse);
 dlg->exec();
 G_pCApp->m_pDrTux->recordPathModulesMenus(dlg->m_PathMenuLib);
 deleteCache();
 delete dlg;
}

//----------------------------------------- deleteCache -----------------------------------------------------------------
/*! \brief Efface l'intégralité du cache des listes. Cela oblige MyEditText à mettre à jour les fichiers de cache pour chaque listes appelées. */
void MyEditText::deleteCache()
{ // Ouvre le répertoire de cache
  QString pathCache = G_pCApp->m_PathGlossaire + INSERTION_ROOT + QDir::separator () + DIR_CACHE_LISTES;
  QDir  dirCache  (pathCache);
  dirCache.convertToAbs();
  // Efface les fichiers du répertoire
  const QFileInfoList * files = dirCache.entryInfoList();
  if ( files )
     { QFileInfoListIterator it( *files );
       QFileInfo * fi;
       while( (fi=it.current()) != 0 )
          {	++it;
	        if (fi->fileName() != "." && fi->fileName() != "..")
	           { QFile::remove( fi->filePath() );
               }
          }
     }
}

//----------------------------------------- UpdateMenu -----------------------------------------------------------------
/*! \brief Fonction à utiliser pour effacer le cache du menu en cours et ainsi le mettre à jour. */
void MyEditText::UpdateMenu()
{ // à ce stade m_ListItem contient le WhatsThis de l'item sélectionné
  // Prépare le fichier Cache
  QString prefix    = G_pCApp->m_PathGlossaire + INSERTION_ROOT + QDir::separator() + DIR_CACHE_LISTES + QDir::separator();
  QString fileCache = prefix + m_ListItem;
  // Teste l'existence de ce fichier cache
  if (QFile::exists ( fileCache ) ) { QFile::remove ( fileCache );  }

  // Met à jour si nécessaire les listes d'Observation et de Prescription
  QMap<QString,QString>::Iterator it;
  for ( it = G_pCApp->m_MapMenuRubriques.begin(); it != G_pCApp->m_MapMenuRubriques.end(); ++it )
      {fileCache = prefix + it.data();
       if (QFile::exists ( fileCache ) ) { QFile::remove ( fileCache );  }
      }
}


//------------------------------------------- GetListForCache -----------------------------------------------------------
/*! \brief Créer l'arborescence de la liste et l'introduit dans le fichier cache.
 * \param &lst : Liste qui contient les items préparés pour le cache
 * \param &list_name : Nom de la liste
 * \return &lst : Liste construite du menu popup
 * Les items du cache sont ainsi composé : \n
 * $> Nom de la liste /;/ Icone (F,I,T) /;/ WhatsThis du popup
*/
int MyEditText::GetListForCache (       QStringList &lst,
                                        QString &list_name,
                                        const QString &parent_liste /*=QString::null*/,
                                        int nbSubMenu /*=0*/)
{
  list_name = list_name.stripWhiteSpace ();
  if (list_name == "") return 0;
  QString path = G_pCApp->m_PathGlossaire + INSERTION_ROOT + QDir::separator() + list_name;

  QDir dir (path);
  if (!dir.exists())
    {  lst += "<EOList>";  return 0; }

  dir.convertToAbs();
  dir.setFilter( QDir::All | QDir::NoSymLinks );

  const QFileInfoList * pQFileInfoList = dir.entryInfoList();
  if (pQFileInfoList==0) return 0;
  QFileInfoListIterator it( *pQFileInfoList );
  QFileInfo     *fi;
  while ( (fi = it.current()) != 0 )
    {QString fname = fi->fileName();//.latin1();
     if (CGestIni::isInBlackList(m_black_list, fname))     {++it;   continue;}
     if (fname == ".." || fname == ".")                    {++it;   continue;}
     else if (fi->isDir())            // *** c'est une autre liste ***
             {  // Génère l'historique de navigation dans le menu
                QString Histo = parent_liste + list_name + HISTORIQUE_SPLITTER;

                // Création de l'entrée dans le cache sur le mode :  ($> en début de ligne pour les listes)
                // $>list_name;nom à placer dans le menu;QPixmap;contenu du WhatsThis (historique de navigation)
                QString wT = Histo + fname;// + ITEM_IS_FOLDER;// + HISTORIQUE_SPLITTER + "0";
                QString itemForCache = "$>"+fname+";F;"+wT;

                // Insère l'item dans la QStringList qui constitue le cache
                lst += itemForCache;

                int limite = nbSubMenu + 1;                                             // Si nbSousMenu < NBSOUSMENUMAX alors
                if (limite < NBMAXSOUSMENU)                                             //   Récupère les éléments de la sous-liste
                     {  GetListForCache(lst,fname, Histo, limite); }                    //   parent_liste permet de retrouver l'historique
                else {  /* insère une fin de liste */                                   //   des listes (répertoires)
                        lst += "<EOList>";
                     }
             }
     else  // if (fi->isFile())                                                       // *** c'est un item ***
        {  QString ext = fi->extension();
           if (ext.lower() == "txt" || ext.lower() == "html" || ext.lower() == "htm" || ext.lower() == "rtf" || ext == "")
            {   // Construit l'historique dans le WhatsThis de l'item
                QString Histo = parent_liste + list_name + HISTORIQUE_SPLITTER +
                                                fname ;//+ HISTORIQUE_SPLITTER + "0";

                // si fname avec extension ==> trouver l'icone adéquat
                QString ItemPixmap;
                if (ext != "")
                        {  // Enlève l'extension à fname
                           fname = fi->baseName();
                           ItemPixmap = "T";
                        }
                else ItemPixmap = "I";

                // Création de l'entrée dans le cache sur le mode :  ($| en début de ligne pour les items)
                // $|list_name;nom à placer dans le menu;QPixmap;contenu du WhatsThis (historique de navigation)
                QString wT = Histo;
                QString itemForCache = "$|"+fname+";"+ItemPixmap+";"+wT;

                // Insère l'item dans la QStringList qui constitue le cache
                lst += itemForCache;
            }
          }  // end else if (fi->isFile())
        ++it;
    } // fin while
  // Insérer un marqueur de fin de répertoire pour pouvoir construire les sous-menus
  lst += "<EOList>";
  return 0;
}

//------------------------------------------- TotalCacheCreator -----------------------------------------------------------
/*! \brief Créer l'intégralité du cache (toutes les listes manquantes sont créées)
 * \return  Nombre de listes construites
*/
int MyEditText::TotalCacheCreator ()
{ //int i=0;
  QString pathCache = G_pCApp->m_PathGlossaire + INSERTION_ROOT + QDir::separator () + DIR_CACHE_LISTES;
  QString pathIns   = G_pCApp->m_PathGlossaire + INSERTION_ROOT;
  QDir dirRootIns (pathIns);
  QDir  dirCache  (pathCache);
  dirCache.convertToAbs();
  dirRootIns.convertToAbs();

  // si le répertoire cache n'existe pas le créer
  if (!dirCache.exists())
        { dirRootIns.mkdir(DIR_CACHE_LISTES); }

  // scrute les listes dans le répertoire INSERTION_ROOT
  dirRootIns.setFilter( QDir::Dirs | QDir::NoSymLinks );

  const QFileInfoList * pQFileInfoList = dirRootIns.entryInfoList();
  if (pQFileInfoList==0) return 0;
  QFileInfoListIterator it( *pQFileInfoList );
  QFileInfo     *fi;
  while ( (fi = it.current()) != 0 )
    { QString fname =  fi->fileName();
      if (CGestIni::isInBlackList(m_black_list, fname))     {++it; continue;}
      if (fname == "." || fname == "..")                    {++it; continue;}

      if ( !QFile::exists ( pathCache + QDir::separator() + fname ) )
         { // le fichier n'existe pas ==> le construire pour l'utiliser
          QStringList liste;
          GetListForCache(liste, fname ,"",0);

          QFile file( pathCache + QDir::separator() + fname );
          if ( file.open( IO_WriteOnly ) )
             {
                QTextStream stream( &file );
                for ( QStringList::Iterator it = liste.begin(); it != liste.end(); ++it )
                        stream << *it << endl;
                file.close();
             }
         }
      ++it;
    } // fin while
 return 0;
}


//------------------------------------------- GoPopup -----------------------------------------------------------
/*! \brief Créer le popup à partir de la QStingList passée en paramètre. Cette QStringList est la copie du fichier cache.
 * \param &lst            Liste qui contient les items préparés pour le cache
 * \param *m_goPopup      le popup
 * \param i               itérator sur la liste
 * \return lst             Liste construite du menu popup
*/
int MyEditText::GoPopup  (QStringList &lst,
                          ThemePopup  &m_goPopup,
                          int length,
                          int i /*=0*/)
{
  QString itemName = lst[i];
  // pour chaque ligne de lst faire
//  if (itemName == "<EOList>") return 0;  //attention problèmatique !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  // tant qu'il y a des items ds lst, et tant que l'on est pas en fin de liste
  while (itemName != "<EOList>" && (i < length))
  { // récupère les items
    QString itemPixmap  = "";
    QString itemWT      = "";
    QStringList lstItem = QStringList::split( ";", itemName );
    itemName            = lstItem[0];
    itemPixmap         += lstItem[1];
    itemWT = lstItem[2];
    if (itemName.left(2) == "$|")   // Item
    { itemName.replace("$|", "");
      int id = m_goPopup.insertItem(Theme::getIconForMyEditText(itemPixmap), itemName, this, SLOT( PasteListItem() ) );
      m_goPopup.setWhatsThis(id , itemWT);
    }
    else                              // Liste
    { itemName.replace("$>", "");
      i++;
      // crée le sous-menu et insère le dans le menu en cours
      if (lst[i] != "<EOList>")  // la liste doit avoir au moins un item pour être un sous-menu
      { CMyThemePopup * sub = new CMyThemePopup(&m_goPopup);
        sub->insertSeparator();
        itemWT += ITEM_IS_FOLDER;
        int id = sub->insertItem(Theme::getIconForMyEditText(itemPixmap), itemName, this, SLOT( PasteListItem() ) );
        sub->setWhatsThis(id , itemWT);
        sub->insertSeparator();
        i  = GoPopup (lst, *sub, length, i);
        ConnectPopup(sub); // connecte le sous-menu
        id = m_goPopup.insertItem(Theme::getIconForMyEditText(itemPixmap), itemName, sub);
        m_goPopup.setWhatsThis( id , itemWT);
      }
      else   // sinon elle est introduit comme simple item folder
      {    int id = m_goPopup.insertItem( Theme::getIconForMyEditText(itemPixmap),
                                          itemName, this, SLOT( PasteListItem() ) );
           m_goPopup.setWhatsThis(id , itemWT);
       }
    }
    i++;
    // redéfinir itemName
    itemName = lst[i];
  } // fin while
 return i;
} // fin GoPopup


//----------------------------------------- GetListItem -----------------------------------------------------------------
/*! \brief Rajoute au popup: ThemePopup *pThemePopup  passé en entrée, autant d'items, qu'en comporte la liste déroulante: QString &list_name, sans inclure l'item FILE_NAME_REPLACOR (qui correspond au fichier qui remplace le répertoire (cf PasteFolderItem() )
 * \param ThemePopup *pThemePopup: pointeur sur le popup auquel doit etre rajouté les items de liste.
 * \param &list_name:      nom de la liste déroulante dont il faut rajouter les items au popup
 * \return int  qui est le nombre d'items rajoutés.
*/
int MyEditText::GetListItem (   ThemePopup *pThemePopup, QString &list_name )
{ int i = 0;
  list_name = list_name.stripWhiteSpace ();
  if (list_name == "") return 0;
  QString path = G_pCApp->m_PathGlossaire + INSERTION_ROOT + QDir::separator() + list_name;

  // Prépare le fichier Cache
  QString fileCache = G_pCApp->m_PathGlossaire + INSERTION_ROOT + QDir::separator() + DIR_CACHE_LISTES + QDir::separator() + list_name;

  // Teste l'existence de ce fichier cache
  QStringList liste;
  if (!QFile::exists ( fileCache ) )
     { // le fichier n'existe pas ==> le construire pour l'utiliser
       GetListForCache(liste, list_name ,"",0);

       QFile file( fileCache );
       if ( file.open( IO_WriteOnly ) )
          {
             QTextStream stream( &file );
             for ( QStringList::Iterator it = liste.begin(); it != liste.end(); ++it )
                     stream << *it << endl;
             file.close();
          }
     }
  // utilise le fichier cache pour construire le menu
  QFile file( fileCache );
  if ( file.open( IO_ReadOnly ) )
     {
       QTextStream stream( &file );
       QString line;
       while ( !stream.atEnd() )
          {
             line = stream.readLine(); // line of text excluding '\n'
             liste.append(line);
          }
       file.close();
     }
  // Construit le PopupMenu
  int length = liste.count();
  GoPopup(liste,*pThemePopup, length, 0);

  return i;
}

//----------------------------------------- OnMenuAboutToHide ----------------------------------------------------
/*! \brief Slot appelée lorsque le popup est sur le point de disparaître.
 * Détruit la bulle d'aide m_pQtextMenuBulle
*/
void MyEditText::OnMenuAboutToHide ()
{if (m_pQtextMenuBulle) delete m_pQtextMenuBulle;
 m_pQtextMenuBulle = 0;
}

//----------------------------------------- OnMenuHighlighted ----------------------------------------------------
/*! \brief Lors du passage de la souris au dessus d'une option de menu, vérifie si c'est elle correspond à un fichier texte et auquel cas l'affiche en bulle d'aide à côté du menu
*/
void MyEditText::OnMenuHighlighted ( int id )
{if (id != -1 && m_pThemePopup)
  {m_CurrentPopupItem    = m_pThemePopup->text(id);
   int param             = m_pThemePopup->whatsThis(id).find(ITEM_IS_FOLDER);
   if (param == -1)  // ce n'est pas une liste
     {m_ListItem         = m_pThemePopup->whatsThis(id);
      QStringList fields = QStringList::split( HISTORIQUE_SPLITTER, m_ListItem , false);
      int nbfields       = fields.count();
      if (nbfields >= 2)
         {    m_ListItem  = fields[nbfields-1];
              m_list_name = fields[nbfields-2];
         }
      QString bulleTxt = "";
      QString path     = G_pCApp->m_PathGlossaire + INSERTION_ROOT + QDir::separator() + m_list_name + QDir::separator() + m_ListItem;
      QString ext      = QFileInfo(path).extension().lower();
      if (ext.length()) CGestIni::Param_UpdateFromDisk(path, bulleTxt);
      bulleTxt = bulleTxt.stripWhiteSpace();
      if (ext[0] != 'h')
         {bulleTxt = bulleTxt.remove("\r"); bulleTxt = bulleTxt.replace("\n","<br>");
         }
      if (bulleTxt.length())
        {if (bulleTxt.left(6) != "<html>")
            {bulleTxt.prepend("<html><qt><head><meta name=\"qrichtext\" content=\"1\" /></head><body>"
                              "<font color=#000000 face=\"Arial\">");
             bulleTxt += "</font></body></qt></html>";
            }
         TimedDestroyMenuBulle();
         QPoint pxy(m_pThemePopup->x(), m_pThemePopup->y());
         pxy = G_pCApp->m_pDrTux->mapFromParent ( pxy );
         m_pQtextMenuBulle = new CMyBulleWidget ( bulleTxt      ,
                                                  m_pThemePopup->width()    ,
                                                  pxy.x(), //m_pThemePopup->x()        ,
                                                  pxy.y(), //m_pThemePopup->y()        ,
                                                  (QWidget*) G_pCApp->m_pDrTux  ,
                                                  "CMyBulle"                ,
                                                  Qt::WStyle_NormalBorder); //Qt::WType_Popup
        }
      else
       {TimedDestroyMenuBulle();
       }
     }  // fin if param != ITEM_IS_FOLDER
  } // fin premier if
}

//----------------------------------------- OnMenuActivated ------------------------------------------------------
/*! \brief Slot activé lorsque une option du popup est survolée, il est noté alors le texte de l'option popup dans la variable de la classe: m_ListItem.  cela permet de noter en fait le nom de la liste deroulante, et de le connaitre lors de la selection.
*/
void MyEditText::OnMenuActivated (int id)
{if (id != -1 && m_pThemePopup)
    { m_ListItem   = m_pThemePopup->whatsThis(id);
    }
}

//----------------------------------------- TimedDestroyMenuBulle -------------------------------------------------
void MyEditText::TimedDestroyMenuBulle()
{if (m_pQtextMenuBulle)
    {delete m_pQtextMenuBulle;
     m_pQtextMenuBulle = 0;
    }
}

//----------------------------------------- PasteTextItem ---------------------------------------------------------
/*! \brief Insère dans le texte, le texte du fichier dont le nom est l'item du popup selectionné (normalement nom dun fichier) */
void MyEditText::PasteTextItem (const QString &ext)
{QString path = G_pCApp->m_PathGlossaire + QString(INSERTION_ROOT) + QDir::separator() + m_list_name + QDir::separator() + m_ListItem + ext;
 PasteFileItem (path);
}

//----------------------------------------- PasteFileItem ----------------------------------------------------------
/*! \brief Insère dans le texte, le contenu du fichier dont le nom est l'item du popup selectionné (normalement nom dun fichier).
 * \todo Bug avec les listes de remplacement qui ne contiennent rien...
*/
void MyEditText::PasteFileItem(QString path)
{int ori_para, ori_index, deb_para, deb_index, end_para, end_index;
 int noStyle = -1;
  //...................... on lit le texte dans un bloc memoire .............................
  QString text = "";
  CGestIni::Param_UpdateFromDisk(path, text);
  getCursorPosition (&ori_para, &ori_index );
  getSelection(&deb_para, &deb_index, &end_para, &end_index);

  long text_len =text.length();
  if (text_len )
     {    noStyle = text.find("$no_style", 0,FALSE);
          if (noStyle == -1) noStyle = 0;
          else             { text.remove(noStyle, 9);
                             noStyle = 1;
                           }
     }
  if ( QStyleSheet::mightBeRichText( text ) )
     {    //...... eliminer les saloperies après le dernier tag HTML ..............
          while (text_len>0 && text.at(text_len -1 ) != '>')
                {--text_len;
                }
          text.truncate (text_len );
          //............... voir si faut pas eliminer un titre ....................
          int deb = text.find("$start_title", 0,FALSE);
          int end = text.find("$end_title",   0,FALSE);
          if ( deb !=-1 && end != -1 ) text.remove(deb,end-deb+10);
          //................ voir si ya pas une commande ..........................
          text.remove("\\<br />");
          DoMixture(text);
     }
  else
     {    htmlConvert( text, MyEditText::HtmlConvertSp, this);
     }    //end else if if ( !QStyleSheet::mightBeRichText( text ) )
  if (text.find("$noInsert")!=-1) return;
  if (text.length())
     {   setCursorPosition (ori_para, ori_index );
         setSelection(deb_para, deb_index, end_para, end_index);
         cut();
         moveCursor (QTextEdit::MoveBackward);  //MoveBackward, MoveForward
         moveCursor (QTextEdit::MoveForward);   //MoveBackward, MoveForward
     }
  //QFont font = this->font();
  //qDebug(QString::number(font.pointSize ()) );

#ifdef Q_WS_X11
  //setUpdatesEnabled (FALSE );
  if (noStyle)  specialInsert(text);
  else          CHtmlTools::insertHtml(this, text);
  //zoomOut ();
  //setUpdatesEnabled (TRUE );
  //zoomIn();
  //zoomTo (m_Zoom);
  //ensureCursorVisible();
#else
  //setUpdatesEnabled (FALSE );
  if (noStyle)  specialInsert(text);
  else          CHtmlTools::insertHtml(this, text,1);
  //zoomIn ();
  //setUpdatesEnabled (TRUE );
  //zoomOut();
#endif
}

//-----------------------------------------------------  DoMixture -----------------------------------------------------
/*! \brief Vérifie que le texte à insérer ne contient pas de commande spéciale (plugins, champs d'insertion...)
 * \sa CDevilCrucible
*/
void MyEditText::DoMixture(QString &text)
{int pos_end       =  0;
 int len           =  0;
 int pos_deb       =  0;
 int pos_max       =  text.length();
 QString command   = "";
 int initialLen    =  0;
 int maxBoucle     = 30000;
 //........ il vaut mieux recuperer la rubrique avant Slot_ExeMixture () .......
 //         car si zero Slot_ExeMixture() la recherche a chaque fois
 //         et l'operation est longue
 int  rub_type               = 0;
 CRubRecord *pCRubRecord     = 0;
 QString     rubName         = "";
 G_pCApp->m_pDrTux->GetCurrentRubrique(&rubName,&rub_type, &pCRubRecord);   // operation couteuse qu'il vaut mieux eviter de faire a chaque Slot_ExePlugin ou Slot_ExeMixture (cas ou pCRubRecord = 0) 
 DOCUMENT_DISPLAY_MAP  map_id = G_pCApp->m_pDrTux->GetMapActiveID_Doc();    // operation couteuse on la fait avant la boucle et on passe en parametres a Slot_ExeMixture et Slot_ExePlugin

 while ( (pos_deb = text.find("{{")) != -1 && pos_deb < pos_max && --maxBoucle>0)
       {pos_end        =  CGestIni::findFermant(&text, pos_deb + 2, pos_max, "{{", "}}");
        len            =  pos_end - pos_deb +2;
        command        =  text.mid(pos_deb, len);
        initialLen     =  command.length();
        if (initialLen)
           {    G_pCApp->m_pDrTux->Slot_ExePlugin(command,  pCRubRecord, map_id);
            if (command==tr("!=!"))
               {command =  text.mid(pos_deb, len);  // recuperer la commande
                G_pCApp->m_pDrTux->Slot_ExeMixture(command, pCRubRecord, map_id);
               }
            text.replace(pos_deb, len, command);
           }
        pos_max       +=  command.length() - initialLen;    //  delta_len
       }
  if (maxBoucle == 0) text = tr("<br> <b>Erreur : </b> il semblerait que l'objet receveur du menu actuel n'existe pas"
                                "<br>surement un défaut de connexion au message : Sign_Exe_Plugin()");
}

//-------------------------------------------------- specialInsert ---------------------------------------------------
/*! \brief Insère un texte html en ne conservant que les retours chariot, tabulations et la couleur bleue des menus deroulants le reste du style du texte est celui de l'endroit ou le texte est inséré
*/
void MyEditText::specialInsert(QString &str)
{
 // ............................. reperer les couleurs bleues et les marquer ..................................................
 //                               et faire peter tous les autres tags
 int posDeb=0;
 int posEnd=0;
 str.remove("\r");
 str.remove("\n");
 while ( (posDeb = str.find("<",posDeb) ) != -1)
        {posEnd =  str.find(">", posDeb+1);
         if (posEnd != -1)
            {QString tag = str.mid(posDeb, posEnd-posDeb).lower();
             if (tag == "<br /" || tag=="<br")
                 {str.replace(posDeb, posEnd-posDeb+1, "\n");
                 }
             else if ((str.mid(posDeb, posEnd-posDeb)).find("color:#0000ff") != -1)
                 {str.replace(posDeb, posEnd-posDeb+1, "~@=@~");
                  posDeb = str.find("</span>", posDeb);
                  if (posDeb !=-1) str.replace(posDeb, 7 , "~@#@~");
                 }
             else str.remove(posDeb, posEnd-posDeb+1);
            }
        }

 str.replace("&lt;","<");
 str.replace("&gt;",">");
 str.replace("&amp;","&");
 str.replace("&nbsp;"," ");
 // .............................inserer en remarquant les couleurs bleues .......................................
 posDeb = 0;
 posEnd = 0;
 setCurrentFont(m_LastFont);           // placer les attributs textes normaux
 setColor(m_LastColor);

 do {posDeb  = str.find("~@=@~",posEnd);
     if (posDeb !=-1)
        {insert(str.mid(posEnd, posDeb-posEnd));         // inserer ce qui est avant la couleur bleue
         posEnd = posDeb + 5;
         setColor(QColor(0x00,0x00,0xFF));               // placer couleur bleue
         posDeb =  str.find("~@#@~", posEnd);
         if (posDeb != -1)
            { insert(str.mid(posEnd, posDeb-posEnd));     // inserer ce qui concerne la couleur bleue
              setCurrentFont(m_LastFont);                 // replacer les attributs textes normaux
              setColor(m_LastColor);
              posEnd = posDeb + 5;
            }
        }
     else if  ( posEnd != -1 )                       // inserer la fin apres la derniere couleur bleue
        {setCurrentFont(m_LastFont);
         setColor(m_LastColor);
         insert(str.mid(posEnd));
         break;
        }
     else                                            //  y en a pas --> inserer tout
        {insert(str);
         break;
        }
    } while (1);
}


//------------------------------------- htmlConvert ----------------------------------------------------------------
/*! \brief Converti le texte HTML passé en entrée pour qu'il convienne au QTextEdit.
*/
void MyEditText::htmlConvert( QString &htmlStr, int flag /* = HtmlConvert*/ , const QTextEdit *pQText_Edit /*=0*/)
{//..................... si ajout header html ou conversion html ..........................................
 if (flag == MyEditText::AddHtmlHeader ||
     flag == MyEditText::HtmlConvert   ||
     flag == MyEditText::HtmlConvertSp       // rajoute un espace devant si 1 seule ligne (nb_ligne==0)
    )
    {//.......................... si conversion html ........................................................
     if (flag == MyEditText::HtmlConvert|| flag == MyEditText::HtmlConvertSp)
            {htmlStr.replace('<',"&lt;");
         htmlStr.replace('>',"&gt;");
         long         p    = 0;
         long        deb   = p;
         int    nb_ligne   = 0;
         while (p<(long)htmlStr.length())
                {char cc = htmlStr.at(p).latin1();    //
                 if (cc=='\r' || cc=='\n')
                { deb = p;
                      //............. passer les retours chariots ......................................
                      if      (cc=='\r' &&  htmlStr.at(p+1)=='\n' ) p +=2;
                      else if (cc=='\n' &&  htmlStr.at(p+1)=='\r' ) p +=2;
                      else                                       ++p;
                      htmlStr.replace(deb, p-deb,"<br>");
                      ++nb_ligne;
                    }
             else
                    {++p;
                }
            } //end while (p<(long)text.length())
             if (flag == HtmlConvertSp && nb_ligne==0) htmlStr.prepend(' ');
            }
     if (pQText_Edit)
            {int u = pQText_Edit->underline();
             int b = pQText_Edit->bold();
             int i = pQText_Edit->italic();
             htmlStr.prepend ( QString("<html><qt><head><meta name=\"qrichtext\" content=\"1\" /></head><body><span style=\""
                                                   "font-family:")    + pQText_Edit->currentFont().family()                      + ";"
                                                   "font-size:"       + QString::number(pQText_Edit->currentFont().pointSize())  + "pt;"
                                                   "font-weight:"     + QString::number(pQText_Edit->currentFont().weight())     + "\">" +
                                                    (u?"<u>":"")      +
                                                    (b?"<b>":"")      +
                                                    (i?"<i>":"")
                                       );

              htmlStr.append  ( QString((u?"</u>":""))                +
                                                (b?"</b>":"")                 +
                                                (i?"</i>":"")                 +
                                                "</span></body></qt></html>"
                              );
             }
     else
             {htmlStr.prepend ( "<html><qt><head><meta name=\"qrichtext\" content=\"1\" /></head><body>");
              htmlStr.append  ( "</body></qt></html>");
         }
    }
}


//------------------------------------ IsThisFolderExist --------------------------------------------------
/*! \brief Test si un répertoire existe. */
bool MyEditText::IsThisFolderExist(const char *path, const char* folderName )
{ QDir dir(QString(path)+ "/" + folderName);
  return dir.exists();
}



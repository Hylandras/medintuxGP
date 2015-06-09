
/*************************************************************************** 
 *   Copyright (C) 2003 by Sevin Roland                                    * 
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
#include <qapplication.h> 
#include <qfileinfo.h> 
#include "MyEditText.h" 
 
#ifdef FOR_MEDINTUX 
 
 #include <qpainter.h> 
 #include <qlineedit.h> 
 #include <qpushbutton.h> 
 #include <qdir.h> 
 #include <qfont.h> 
 #include <qsimplerichtext.h> 
 #include <qpalette.h> 
 #include <qlistbox.h> 
 #include <qcursor.h> 
 #include <qmessagebox.h> 
 #include "../../MedinTuxTools/DLG_InsertList.h" 
 #include "../../MedinTuxTools/DlgGetString.h" 
 #include "../../drtux/src/DlgListFieldMngr.h" 
 #include "../../MedinTuxTools/DlgMyMessageBox.h" 
 #include "../../MedinTuxTools/CHtmlTools.h" 
 #include "../../MedinTuxTools/CGenTools.h" 
 #include "../../MedinTuxTools/CGestIni.h" 
 #include "../../MedinTuxTools/DlgListOff_Listes.h" 
#define IS_HTML 23000 
 
//====================================== CMyQPopupMenu ======================================================= 
CMyQPopupMenu::CMyQPopupMenu ( QWidget * parent, const char * name  ) 
    :QPopupMenu ( parent ,  name  ) 
{ 
} 
//---------------------------------- event ------------------------------------------------------------------- 
// ACTION: interception des evenements souris pour emettre le signal: Sign_MouseOutPopup();  lorsque la souris 
//         est en dehors du QPopupMenu   (sert à effacer la bulle dans MyEditText) 
bool CMyQPopupMenu::event ( QEvent * ev ) 
{ 
 if ( ev->type() == QEvent::MouseMove && 
      !QRect(QPoint(0,0), frameSize()).contains ( ((QMouseEvent*)ev)->pos() ) 
    ) emit Sign_MouseOutPopup(); 
 
 return QPopupMenu::event (ev); 
} 
 
 
 
//====================================== CMyBulleWidget ========================================================================== 
CMyBulleWidget::CMyBulleWidget ( const QString &text  ,    // texte de la bulle (peut être du HTML) 
                                 int          wAdj    ,    // largeur de la zone à eviter (celle du popup) 
                                 int             x    ,    // Position gauche de la zone à eviter (celui du popup) 
                                 int             y    ,    // Position haute de la zone à eviter (celui du popup) 
                                 const QString &pathDrTux , 
                                 QWidget    *parent   ,    // parent du widget 
                                 const  char *name    ,    // nom du widget 
                                 WFlags           f   ,    // flags du widget 
                                 int          width   ,    // largeur de la bulle 
                                 int        /* height */  ,    // hauteur de la bulle 
                                 int           zoom        // zomm d'affichage 
                                ) : QWidget( parent, name , f) 
{ 
 m_pQTextEdit = new QTextEdit (this, name); 
 if (m_pQTextEdit) 
    {m_pQTextEdit->zoomTo (zoom); 
     m_pQTextEdit->zoomTo (zoom); 
     int deb = text.find("$start_title", 0,FALSE); 
     int end = text.find("$end_title",   0,FALSE); 
     if ( deb !=-1 && end != -1 ) 
        { deb += 12; 
          m_pQTextEdit->setText(text.mid(deb, end-deb)); 
        } 
     else 
        {m_pQTextEdit->setText(text); 
        } 
     int h     = m_pQTextEdit->heightForWidth (width); 
     int pos_x = x + wAdj; 
     if (pos_x + width >= qApp->desktop()->width()) pos_x =  x - width - 10; 
     m_pQTextEdit->setGeometry (0, 0, width, h+10); 
     setGeometry (pos_x, y, width + 1, h + 10 + 1); 
     m_pQTextEdit->setPaletteBackgroundColor (QColor(255,249,140 )); 
     setPaletteBackgroundColor (QColor(4,4,4 )); 
     m_pQTextEdit->setWordWrap(QTextEdit::NoWrap); 
     m_pQTextEdit->setReadOnly ( TRUE ); 
     if (pathDrTux.length()) m_pQTextEdit->mimeSourceFactory()->addFilePath(pathDrTux + "/Ressources/RubriquesTextes"); 
     //QString pathAppli = QFileInfo (qApp->argv()[0]).dirPath (true) + QDir::separator(); 
     //m_pQTextEdit->mimeSourceFactory()->addFilePath(pathAppli + "Ressources" + QDir::separator() + "RubriquesTextes"); 
     m_pQTextEdit->show(); 
     show(); 
    } 
} 
 
#endif 
 
 
//====================================== MyEditText ========================================================================== 
 
//---------------------------------- MyEditText ------------------------------------------------------------------- 
 MyEditText::MyEditText( const QString     & text, 
                         const QString     & pathGlossaire, 
                         const QString     & pathDrTux, 
                         QStringList       * pInsertFieldList, 
                         const QString     & context   , 
                         QWidget           * parent    , 
                         const char        * name ) 
                         :QTextEdit(text, context, parent, name) 
            { initDefaultVar(pathGlossaire, pathDrTux, pInsertFieldList); 
            } 
 
  MyEditText:: MyEditText( const QString     & pathGlossaire, 
                           const QString     & pathDrTux, 
                           QStringList       * pInsertFieldList, 
                           QWidget           * parent    , 
                           const char        * name      ) 
                          :QTextEdit(parent, name) 
            {initDefaultVar(pathGlossaire, pathDrTux, pInsertFieldList); 
            } 
//---------------------------------- initDefaultVar ------------------------------------------------------------------- 
void MyEditText::initDefaultVar(const QString & pathGlossaire, const QString & pathDrTux,  QStringList *pInsertFieldList) 
{   setTextFormat(Qt::RichText); 
    m_Path                    = QFileInfo (qApp->argv()[0]).dirPath (true); 
    m_PathAppli               = m_Path + "/"; 
    m_PathImage               = ""; 
    m_DefaultTextColor        = ""; 
    m_DefaultBackgroundColor  = ""; 
    m_Filter                  = ""; 
    m_pQPopupMenu             = 0; 
    m_pCCoolPopup             = 0; 
    m_pQtextMenuBulle         = 0; 
    m_State                   = 1; 
    m_style                   = ""; 
    m_DefaultBackgroundColor  = ""; 
    m_DefaultTextColor        = ""; 
    m_list_name               = ""; 
    m_Zoom                    = 0; 
    m_PathGlossaire           = pathGlossaire; 
    m_PathDrTux               = pathDrTux; 
    m_PopupItemAddItem        = tr("Ajouter un item"); 
    m_PopupItemFusionItem     = tr("Insérer un champ de fusion"); 
    m_pInsertFieldList        = pInsertFieldList; 
    m_IsUTF8                  = 0; 
    if (m_PathDrTux.length()) mimeSourceFactory()->addFilePath(m_PathDrTux + "/Ressources/RubriquesTextes"); 
    //......................... interligne des paragraphes le plus petit possible .............................. 
    //                          comme celui des retours chariot 
    QStyleSheet      *sheet  = styleSheet() ; 
    QStyleSheetItem  *stylep = sheet->item("p"); 
    stylep->setDisplayMode(QStyleSheetItem::DisplayBlock); 
    stylep->setMargin(QStyleSheetItem::MarginVertical, 0); 
} 
 
 
//---------------------------------- event ----------------------------------------------------------------------------- 
// QKeyEvent( Type type, int key, int ascii, int state, 
//            const QString& text=QString::null, bool autorep=FALSE, ushort count=1 ) 
// ACTION: surcharge de la fonction clavier de QTextEdit, afin de corriger l'anomalie de fonctionnement inversé 
//        de la touche enter et ctrl+enter. 
 
bool MyEditText::event ( QEvent * ev ) 
{//if ( ev->type() == QEvent::MouseButtonRelease) 
 //   {QMouseEvent *m = (QMouseEvent*)ev; 
 //    int buton = m->button (); 
 //   } 
 
 if ( ev->type() == QEvent::KeyPress ) 
    { 
 
     QKeyEvent   *k = (QKeyEvent *)ev; 
     int      state = k->state(); 
     int        car = k->ascii(); 
    //.............. Control A ..................... 
     if (state==Qt::ControlButton && car == 1) 
        {selectAll(); 
         QKeyEvent keyEvent (QKeyEvent::None, 0, 0, 0,  k->text(),  0, 0 ); 
         keyEvent.accept(); 
         return QTextEdit::event (&keyEvent); 
        } 
     //............. inverser le fonctionnement de la TOUCHE ENTER................................ 
     //              car d'origine: ENTER        --> fin de paragraphe avec double interligne 
     //                             ENTER + CTRL --> fin de ligne avec interligne normal 
     //              et cela ne me va pas 
     if (car==13 || car==10) 
        {//int      state = k->state(); 
         /*...................... exploration des touches ........................................ 
         QString   qstr = ""; 
         QString    str = ""; 
         if (k->text().length()>0) 
            {char buft[32]; 
             memcpy(buft, k->text(), k->text().length()); 
             str += " \r\ntext: " + qstr.setNum( buft[0] ); 
            } 
         else                      str += "\r\ntext: empty"; 
         qstr.sprintf("KeyPressed: %d \r\nAscii: %d \r\nState:",k->key(), k->ascii() ); 
 
         switch (state&Qt::KeyButtonMask) 
                {case Qt::NoButton:        qstr+=" - no button  pressed.";                          break; 
                 case Qt::LeftButton:      qstr+=" - set if the left button is pressed.";           break; 
                 case Qt::RightButton:     qstr+=" - the right button.";                            break; 
                 case Qt::MidButton:       qstr+=" - the middle button.";                           break; 
                 case Qt::ShiftButton:     qstr+=" - a Shift key on the keyboard is also pressed."; break; 
                 case Qt::ControlButton:   qstr+=" - a Ctrl key on the keyboard is also pressed.";  break; 
                 case Qt::AltButton:       qstr+=" - an Alt key on the keyboard is also pressed.";  break; 
                 case Qt::MetaButton:      qstr+=" - a Meta key on the keyboard is also pressed.";  break; 
                 case Qt::Keypad:          qstr+=" - a keypad button is pressed.";                  break; 
                 case Qt::KeyButtonMask:   qstr+=" - a mask for ShiftButton, ControlButton, AltButton and MetaButton.";break; 
                 case Qt::MouseButtonMask: qstr+=" - a mask for LeftButton, RightButton and MidButton.";break; 
                } 
         qstr += str; 
         qstr += "\r\nNum K State: " + str.setNum(state,16); 
         setText(qstr); 
         */ 
     if (car==10) car = 13; 
         if (state!=Qt::ControlButton) state = Qt::ControlButton; 
         else                          state = Qt::NoButton; 
         QKeyEvent keyEvent (ev->type(), k->key(), car, state, 
                             k->text(),  k->isAutoRepeat(), k->count() ); 
         if ( k->isAccepted ()) keyEvent.accept(); 
         else                   keyEvent.ignore(); 
         return QTextEdit::event (&keyEvent); 
        }   //endif (car==13) 
    }  //endif ( ev->type() == QEvent::KeyPress ) 
 
 return QTextEdit::event (ev); 
} 
 
//----------------------------------------- moveCursor ----------------------------------------------------------------- 
// ACTION: permet à notre classe d'acceder à la methode protégée: QTextEdit::moveCursor( (CursorAction)action,  select); 
 
void MyEditText::moveCursor (int action, bool select ) 
{QTextEdit::moveCursor( (CursorAction)action,  select); 
} 
 
 
#ifdef FOR_MEDINTUX 
//----------------------------------------- createPopupMenu ----------------------------------------------------------------- 
// ACTION: recherche lors de l'appui du bouton droit si il y a une zone de liste (bleue #0000ff) 
//         et auquel cas rajoute les items de cette liste au popup menu classique. 
 
QPopupMenu * MyEditText::createPopupMenu ( const QPoint & pos )   // CMyQPopupMenu 
{m_ListItem=""; 
 CMyQPopupMenu *pQPopupMenu = new CMyQPopupMenu(0, "MyPopupMenu" ); 
 if (pQPopupMenu==0)    return 0; 
 
 //............................... chercher une zone de liste deroulante (bleue) ..................... 
 QPoint        qpoint(pos); 
 m_list_name = GetLastList(-1, -1, &qpoint).stripWhiteSpace ();     //chercher si une liste existe cinq lignes avant 
 
 //............................... créer le Popup classique ......................................... 
 
 pQPopupMenu->insertItem( QPixmap(m_PathAppli +"QLightPadSys/PingouinsSimple.png" ),     tr("Bonjour de la part de QLightPad"), this, SLOT( CouCou())      ); 
 pQPopupMenu->insertItem( QPixmap(m_PathAppli +"QLightPadSys/undo.png" ),                tr("&Annuler"),                        this, SLOT( undo()),      CTRL+Key_Z ); 
 pQPopupMenu->insertItem( QPixmap(m_PathAppli +"QLightPadSys/redo.png" ),                tr("&Refaire"),                        this, SLOT( redo()),      CTRL+Key_Y ); 
 pQPopupMenu->insertItem( QPixmap(m_PathAppli +"QLightPadSys/editcut.png" ) ,            tr("&Couper"),                         this, SLOT( cut()),       CTRL+Key_X ); 
 pQPopupMenu->insertItem( QPixmap(m_PathAppli +"QLightPadSys/editcopy.png" ),            tr("C&opier"),                         this, SLOT( copy()),      CTRL+Key_C ); 
 pQPopupMenu->insertItem( QPixmap(m_PathAppli +"QLightPadSys/editpaste.png" ),           tr("Co&ller"),                         this, SLOT( paste()),     CTRL+Key_V ); 
 pQPopupMenu->insertItem( QPixmap(m_PathAppli +"QLightPadSys/filetAllSelect_Icon.png" ), tr("&Tout sélectionner"),              this, SLOT( selectAll()), CTRL+Key_A ); 
 //QPixmap(GlobalPathAppli +"QLightPadSys/PopupList_Item.xpm" ) 
 //............... rajouter (si il y en a) les items de la liste ....................................... 
 GetListItem (pQPopupMenu, m_list_name ); 
 //............... si Listes à inclure d'office .............................................. 
 //if (m_PopupListObserv.length() && m_TypeRubrique==TYP_OBSERVATION) 
 //   {pQPopupMenu->insertSeparator (); 
 //    GetListItem (pQPopupMenu, m_PopupListObserv , TYP_OBSERVATION); 
 //   } 
 //if (m_PopupListPrescr.length() && m_TypeRubrique==TYP_PRESCRIPTION) 
 //   {pQPopupMenu->insertSeparator (); 
 //    GetListItem (pQPopupMenu, m_PopupListPrescr , TYP_PRESCRIPTION); 
 //   } 
 bool sep = FALSE; 
 //............... si zone de liste proposer modification .............................................. 
 if (m_PathDrTux.length() && m_list_name != "") 
    {if ( !sep) pQPopupMenu->insertSeparator (); 
     sep = TRUE; 
     pQPopupMenu->insertItem(QPixmap(m_PathAppli + "QLightPadSys/DoList_Item.png" ), tr("Modifier cette liste"),    this, SLOT( ListManager()),  CTRL+Key_L ); 
     if (/*m_PopupItemFusionItem.length()||*/m_PopupItemAddItem.length()) 
        {                                                                                                                                                                                                            // PopupItemAddItem 
         //if (m_PopupItemFusionItem.length()) pQPopupMenu->insertItem( m_PopupItemFusionItem,    this, SLOT( PopupItemFusionItem()),  0 ); 
                                                pQPopupMenu->insertItem(QPixmap(m_PathAppli + "QLightPadSys/CreateList_Item.png"),        tr("C&réer une liste déroulante avec le texte sélectionné."),             this, SLOT( PopupCreateList()),      CTRL+Key_F ); 
         if (m_PopupItemFusionItem.length())    pQPopupMenu->insertItem(QPixmap(m_PathAppli + "QLightPadSys/InsererChampInsertion.png" ), m_PopupItemFusionItem,                                                    this, SLOT( InsererChampInsertion()),  0 ); 
                                                pQPopupMenu->insertItem(QPixmap(m_PathAppli + "QLightPadSys/InsertList_Item.png" ),       tr("Insérer une zone de liste déroulante déjà existante"),                this, SLOT( InsererZoneMenu()),  0 ); 
         if (m_PopupItemAddItem.length())       pQPopupMenu->insertItem(QPixmap(m_PathAppli + "QLightPadSys/AddList_Item.png" ),          m_PopupItemAddItem,                                                       this, SLOT( PopupItemAddItem()),  0 ); 
 
        } 
    } 
 //............... si indicateur de sauvegarde du modele .............................................. 
 if (m_PopupItemAddDocType.length()) 
    {if ( !sep) pQPopupMenu->insertSeparator (); 
     sep = TRUE; 
     pQPopupMenu->insertItem(QPixmap(m_PathAppli + "QLightPadSys/backup_section.png" ), m_PopupItemAddDocType,    this, SLOT( PopupItemAddDocType()),  0 ); 
    } 
 //................. connecter le menu ................................................................. 
 connect( pQPopupMenu, SIGNAL( activated( int)), 
          this,        SLOT(OnMenuActivated( int)) 
        ); 
 connect( pQPopupMenu, SIGNAL( highlighted ( int)), 
          this,        SLOT(OnMenuHighlighted( int)) 
        ); 
 connect( pQPopupMenu, SIGNAL( aboutToHide ()), 
          this,        SLOT(OnMenuAboutToHide ()) 
        ); 
 //................. connecter le popup pour detruire la bulle si souris est en dehors ................... 
 //                  ultra important car si on laisse la bulle affichée lorsque la 
 //                  souris est en dehors et que souris au dessus de la bulle avec molette --> plantage 
 connect( pQPopupMenu, SIGNAL( Sign_MouseOutPopup()), 
          this,        SLOT(   TimedDestroyMenuBulle()) 
        ); 
 m_pQPopupMenu = pQPopupMenu; 
 return  pQPopupMenu; 
} 
 
//------------------------------------------------- PopupItemAddItem ---------------------------------------------------------------- 
void MyEditText::PopupItemAddItem () 
{m_list_name = GetLastList(); 
 if (m_list_name.length()<=0) 
    {CGenTools::MessageAlert(this, tr("ERREUR !"),tr("Pas de liste déroulante trouvée<br>dans les 5 lignes précédentes."), tr("Annuler")); 
     return; 
    } 
 QString  path; // = G_pCDrTuxApp->m_PathGlossaire + INSERTION_ROOT + QDir::separator() + GetPathItem (m_pQListViewItem); 
 QString qsName = selectedText (); 
 CHtmlTools::HtmlToAscii( qsName, CHtmlTools::StrictASCII ); 
 if (qsName.length()<=0) qsName = tr("Élément nouveau "); 
 
 
 if (CGenTools::DialogGetString(this, tr("Ajouter un item à la liste :" + m_list_name), qsName) ==  QDialog::Accepted) 
    {if (qsName=="") 
        {CGenTools::MessageAlert(this, tr("ATTENTION !"),tr("Le nom du fichier est vide.<br>Veuillez donner un nom."), tr("Annu&ler")); 
         return; 
        } 
 
     path  = m_PathGlossaire + INSERTION_ROOT + QDir::separator() + m_list_name; 
     path += QDir::separator() + qsName; 
     if (IsThisFileExist(path)) 
        {CGenTools::MessageAlert(this, tr("ATTENTION !"),tr("Ce fichier existe déjà.<br>Veuillez choisir un autre nom"), tr("Annuler")); 
         return; 
        } 
     QFile qfile(path); 
     if (qfile.open(IO_ReadWrite)==FALSE) 
        {QString mess  = tr("Le fichier<br>"); 
                 mess += path; 
                 mess += tr("<br> n'a pu être créé."); 
         CGenTools::MessageAlert(this, tr("Erreur :"),mess, tr("Annu&ler")); 
        } 
    } 
} 
 
//------------------------------------ PopupCreateList -------------------------------------------------- 
void MyEditText::PopupCreateList() 
{   QString qsName = selectedText (); 
    CHtmlTools::HtmlToAscii( qsName, CHtmlTools::StrictASCII ); 
    // QListViewItem *qlistViewItem; 
    QListViewItem *element; 
 
    if (CGenTools::DialogGetString(this, tr("Veuillez donner le nom de cette nouvelle liste déroulante"), qsName) ==  QDialog::Accepted) 
    {if (qsName=="") 
        {CGenTools::MessageAlert(this, tr("ATTENTION !"),tr("Le nom de la référence est vide.<br>Veuillez choisir un nom"), tr("Annu&ler")); 
            return; 
        } 
        QString pathListes = m_PathGlossaire + INSERTION_ROOT; 
        //........... créer si besoin la liste à laquelle se réfère la réference.................... 
        if (IsThisFolderExist(pathListes, qsName)==FALSE)     // que si n'existe pas déjà 
           {QDir dir; 
            if (dir.mkdir (pathListes + QDir::separator() + qsName)==FALSE) 
               {QString mess  = tr("Cette liste<br>"); 
                mess += pathListes + QDir::separator() + qsName; 
                mess += tr("<br> n'a pu être créée.<br> peut être existe-t-elle déjà."); 
                CGenTools::MessageAlert(this, tr("ERREUR :"),mess, tr("Annu&ler")); 
                return; 
               } 
           } 
        QColor old_col = color(); 
        if (hasSelectedText()) del(); 
        setColor( QColor(0x00,0x00,0xFF) ); 
        insert(qsName); 
        setColor( old_col ); 
 
    } 
} 
 
//------------------------------------------------- PopupItemAddDocType ---------------------------------------------------------------- 
void MyEditText::PopupItemAddDocType () 
{QString command("Enregistrer comme un document type"); 
 emit Sign_ExePlugin(command); 
} 
 
//------------------------------------ IsThisFileExist -------------------------------------------------- 
bool MyEditText::IsThisFileExist(const char* path ) 
{QDir dir; 
 return dir.exists(path ); 
} 
//------------------------------------------------- setText ------------------------------------------------------------------------ 
void MyEditText::setText ( const char* ptr ) 
{if (CGestIni::IsUtf8( ptr )) 
    {setText(QString::fromUtf8 ( ptr )); 
    } 
 else 
    {setText( QString(ptr) ); 
    } 
} 
//------------------------------------------------- setText ------------------------------------------------------------------------ 
void MyEditText::setText ( const QString & txt ) 
{setUpdatesEnabled (FALSE ); 
 hide(); 
 QString path(m_PathAppli + "Ressources/RubriquesTextes"); 
 if (mimeSourceFactory()->filePath().findIndex (path)==-1) mimeSourceFactory()->addFilePath(path); 
 m_style                   = CHtmlTools::getTextDefaultAttribut(txt, "style"); 
 m_DefaultBackgroundColor  = CHtmlTools::getTextDefaultAttribut(txt, "bgcolor"); 
 m_DefaultTextColor        = CHtmlTools::getTextDefaultAttribut(txt, "text"); 
 if (m_DefaultBackgroundColor.length()>0) setPaper(QBrush( QColor(m_DefaultBackgroundColor))); 
 else                                     setPaper(QBrush( QColor("#FFF"))); 
 QFont      font; 
 QTextEdit::setText(txt); 
 
 //........ il semblerait que positionner la fonte avant le texte ............... 
 //         fait planter dans certaines conditions ??? 
 if (CHtmlTools::QT_style_To_QFont(m_style , font, "Arial","10")) setFont(font); 
 //if (m_Zoom) zoomTo (m_Zoom); 
 setUpdatesEnabled (TRUE ); 
 if (m_State) show(); 
} 
 
//------------------------------------------------- CouCou ---------------------------------------------------------------- 
void MyEditText::CouCou() 
{if (m_pCCoolPopup) delete m_pCCoolPopup; 
 
 m_pCCoolPopup = new CCoolPopup( m_PathAppli + "QLightPadSys/QLightPad.png",0 ); 
 connect( m_pCCoolPopup,        SIGNAL( Sign_PopupUp(CCoolPopup *) ), 
          this,                 SLOT(CouCouUp (CCoolPopup *)) 
        ); 
 connect( m_pCCoolPopup,        SIGNAL( Sign_PopupDown(CCoolPopup *) ), 
          this,                 SLOT(CouCouStop(CCoolPopup *)) 
        ); 
} 
 
//--------------------------------------- CouCouUp ------------------------------------------------------------------------ 
void MyEditText::CouCouUp(CCoolPopup * pCCoolPopup) 
{pCCoolPopup->Disappear(); 
} 
//--------------------------------------- CouCouStop ------------------------------------------------------------------------ 
void MyEditText::CouCouStop(CCoolPopup * pCCoolPopup) 
{if (m_pCCoolPopup==pCCoolPopup) 
    {pCCoolPopup->disconnect( SIGNAL(Sign_PopupUp(CCoolPopup *)) ); 
     pCCoolPopup->disconnect( SIGNAL(Sign_PopupDown(CCoolPopup *)) ); 
     QTimer::singleShot(0, this, SLOT(CouCouDestroy()) ); 
    } 
} 
//--------------------------------------- CouCouDestroy ------------------------------------------------------------------------ 
void MyEditText::CouCouDestroy() 
{if (m_pCCoolPopup) delete m_pCCoolPopup; 
 m_pCCoolPopup = 0; 
} 
 
//------------------------------------- DoContextListPopup ---------------------------------------------------------------- 
int MyEditText::DoContextListPopup() 
{m_ListItem=""; 
 CMyQPopupMenu *pQPopupMenu = new CMyQPopupMenu(0, "MyPopupMenu" ); 
 if (pQPopupMenu==0)    return 0; 
 //............................... chercher une zone de liste deroulante (bleue) ..................... 
 m_list_name = GetLastList();     //chercher si une liste existe cinq lignes avant le curseur texte 
 
 //............................... créer le Popup classique ......................................... 
 
 //pQPopupMenu->insertItem( QPixmap( undo ),          ("&Undo"),     this, SLOT(undo()),    CTRL+Key_Z ); 
 //pQPopupMenu->insertItem( QPixmap( redo ),          ("&Redo"),     this, SLOT( redo()),   CTRL+Key_Y ); 
 //pQPopupMenu->insertItem( QPixmap( editcut ),       ("&Cut"),      this, SLOT( cut()),    CTRL+Key_X ); 
 //pQPopupMenu->insertItem( QPixmap( editcopy ),      ("C&opy"),     this, SLOT( copy()),   CTRL+Key_C ); 
 //pQPopupMenu->insertItem( QPixmap( editpaste ),     ("&Paste"),    this, SLOT( paste()),  CTRL+Key_C ); 
 //pQPopupMenu->insertItem( ("&Select all"),    this, SLOT( selectAll()),  CTRL+Key_A ); 
 
 //............... rajouter (si il y en a) les items de la liste ....................................... 
 GetListItem (pQPopupMenu, m_list_name ); 
 //............... si Listes à inclure d'office .............................................. 
 //if (m_PopupListObserv.length() && m_TypeRubrique==TYP_OBSERVATION) 
 //   {pQPopupMenu->insertSeparator (); 
 //    GetListItem (pQPopupMenu, m_PopupListObserv , TYP_OBSERVATION); 
 //   } 
 //if (m_PopupListPrescr.length() && m_TypeRubrique==TYP_PRESCRIPTION) 
 //   {pQPopupMenu->insertSeparator (); 
 //    GetListItem (pQPopupMenu, m_PopupListPrescr , TYP_PRESCRIPTION); 
 //   } 
 //............... si zone de liste proposer modification .............................................. 
 if (m_list_name != "") 
    {pQPopupMenu->insertSeparator (); 
    } 
 pQPopupMenu->insertItem(QPixmap(m_PathAppli + "QLightPadSys/DoList_Item.png" ), tr("Gestionnaire de listes ..."),    this, SLOT( ListManager()),  CTRL+Key_L ); 
 //................. connecter le menu ................................................................. 
 connect( pQPopupMenu, SIGNAL( activated( int)), 
          this,        SLOT(OnMenuActivated( int)) 
        ); 
 connect( pQPopupMenu, SIGNAL( highlighted ( int)), 
          this,        SLOT(OnMenuHighlighted( int)) 
        ); 
 connect( pQPopupMenu, SIGNAL( aboutToHide ()), 
          this,        SLOT(OnMenuAboutToHide ()) 
        ); 
 //................. connecter le popup pour detruire la bulle si souris est en dehors ................... 
 //                  ultra important car si on laisse la bulle affichée lorsque la 
 //                  souris est en dehors et que souris au dessus de la bulle avec molette --> plantage 
 connect( pQPopupMenu, SIGNAL( Sign_MouseOutPopup()), 
          this,        SLOT(   TimedDestroyMenuBulle()) 
        ); 
 m_pQPopupMenu = pQPopupMenu; 
 pQPopupMenu->exec(QCursor::pos()); 
 delete  pQPopupMenu; 
 return 1; 
} 
 
//----------------------------------------- GetLastList ----------------------------------------------------------------- 
// ACTION: remonte 5 lignes en arriere du curseur à la recherche d'une zone de liste bleue #0000ff et si trouvée 
// ENTREE: QPoint *pos: position du curseur texte à partir duquel doit se faire la recherche 
//                      si zero alors la recherche se fait à partir des valeurs de: 
//         int ori_index:  qui si -1 la recherche se fait à partir de pos qui si zero se fera à partir de la 
//                         position courante. 
//         int ori_para:   qui si -1 la recherche se fait à partir de pos qui si zero se fera à partir de la 
//                         position courante. 
// RETOUR: retourne le nom de la zone de liste sinon  chaine vide "" 
 
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
 QFont m_LastFont(currentFont()); 
 
 //.................. reculer de 5 lignes et se mettre en debut de ligne ....................... 
 moveCursor (QTextEdit::MoveUp); 
 moveCursor (QTextEdit::MoveUp); 
 moveCursor (QTextEdit::MoveUp); 
 moveCursor (QTextEdit::MoveUp); 
 moveCursor (QTextEdit::MoveUp); 
 moveCursor (QTextEdit::MoveLineStart, TRUE); 
 //....................... recuperer la position 5 lignes avant ................................. 
 //                        la recherche se fait en reculant à partir du curseur texte 
 getCursorPosition (&deb_para, &deb_index ); 
 if (old_deb_par != -1)   // Si une selection existe: la recherche se fait AVANT (afin d'eviter d'eventuels mots clefs inside) 
     setSelection(deb_para, deb_index, old_deb_par, old_deb_index); 
 else                     // Sinon on recherche le mot clef à partir du curseur texte 
     setSelection(deb_para, deb_index, ori_para, ori_index); 
 QString qs_text = this->selectedText();                     // recuperer le texte selectionné 
 //........................ replacer tout comme avant ............................................ 
 if (old_deb_par != -1) 
     setSelection(old_deb_par, old_deb_index, old_end_par, old_end_index); 
 else 
    {removeSelection();                        // deselectionner 
     setCurrentFont(m_LastFont); 
    } 
 if (pos) placeCursor (*pos);                  // replacer le curseur à son emplacement d'origine 
 else     setCursorPosition (ori_para, ori_index ); 
 
 
 //....................... analyse des 5 lignes precedentes ...................................... 
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
    } // end if (int f = g_text.findRev("color:#0000ff") !=-1)      // chercher debut du bleu en partant de la fin 
 else 
    {qs_text = ""; 
    } 
 setUpdatesEnabled( TRUE );      // retablir l'affichage 
 qs_text.replace("&nbsp;"," "); 
 return qs_text.replace((char)0xa0," "); 
} 
 
//----------------------------------------- ListManager ----------------------------------------------------------------- 
// ACTION: affiche la boite de dialogue Gestionnaire de listes déroulantes 
 
void MyEditText::ListManager() 
{FormDlgListFieldMngr *dlg = new FormDlgListFieldMngr(0,"ListManager_Dial",TRUE, WStyle_Customize | WStyle_NormalBorder | WStyle_Title |WStyle_MinMax | WStyle_SysMenu); 
 if (dlg ==0) return; 
 dlg->initDialog(m_PathGlossaire, m_PathDrTux); 
 QFont font; 
 if (CHtmlTools::QT_style_To_QFont(m_style , font, "Arial","10")) dlg->setFontEdit(font); 
  //........ il semblerait que positionner la fonte avant le texte ............... 
 //         fait planter dans certaines conditions ??? 
 
 QListViewItem *pQListViewItem = dlg->FindDepthItem(m_list_name,0); 
 if (pQListViewItem) 
    {dlg->listViewList->setCurrentItem(pQListViewItem); 
     dlg->listViewList->ensureItemVisible (pQListViewItem); 
     dlg->listViewList_doubleClicked(pQListViewItem); 
    } 
 dlg->exec(); 
 delete dlg; 
} 
 
//---------------------------- InsererZoneMenu ------------------------------------------------- 
void MyEditText::InsererZoneMenu() 
{FormDlgListOff_Listes *dlg = new FormDlgListOff_Listes(this,"RefList_Dial",TRUE); 
 if (dlg==0) return; 
 dlg->initDialog(m_PathGlossaire+"/Champs d'insertions"); 
 dlg-> exec(); 
 if (dlg->m_ItemName !="") 
    {QColor old_col = color(); 
      if (hasSelectedText()) del(); 
      setColor( QColor(0x00,0x00,0xFF) ); 
      insert(dlg->m_ItemName); 
      setColor( old_col ); 
    } 
 delete dlg; 
} 
 
//------------------------------------ IsThisFolderExist -------------------------------------------------- 
bool MyEditText::IsThisFolderExist(const char *path, const char* folderName ) 
{  QDir dir(QString(path)+ QDir::separator() + folderName); 
    return dir.exists(); 
} 
 
//----------------------------------------- GetListItem ----------------------------------------------------------------- 
// ACTION: rajoute au popup: QPopupMenu *pQPopupMenu  passé en entrée, autant d'items, qu'en comporte la liste déroulante: 
//          QString &list_name. 
// ENTREE: QPopupMenu *pQPopupMenu: pointeur sur le popup auquel doit etre rajouté les items de liste. 
//         QString &list_name:      nom de la liste déroulante dont il faut rajouter les items au popup 
// SORTIE: int  qui est le nombre d'items rajoutés. 
 
 
int MyEditText::GetListItem (QPopupMenu *pQPopupMenu, QString &list_name , int list_type /*=0*/) 
{   int i = 0; 
    list_name = list_name.stripWhiteSpace (); 
    if (list_name == "") return 0; 
    QString path = m_PathGlossaire + INSERTION_ROOT + QDir::separator() + list_name; 
    QDir dir (path); 
    dir.convertToAbs(); 
    dir.setFilter( QDir::All | QDir::NoSymLinks ); 
 
    //............ exploration de la liste ...................... 
    const QFileInfoList * pQFileInfoList = dir.entryInfoList(); 
    if (pQFileInfoList==0) return 0; 
    QFileInfoListIterator it( *pQFileInfoList ); 
    QFileInfo     *fi; 
    while ( (fi = it.current()) != 0 ) 
          {QString fname = fi->fileName().latin1(); 
           if      (fname == "..") 
                   { 
                   } 
           else if (fi->isDir() && fname != "." && fname.lower() != "cvs") 
                   {m_ListItem = fname; 
                    if (i==0) pQPopupMenu->insertSeparator (); // inserer un separateur avant le premier element 
                    pQPopupMenu->insertItem(QPixmap(m_PathAppli +"QLightPadSys/folder_Icon.png" ), fname, this, SLOT( PasteFolderItem())); 
                    m_ListItem = fname; 
                    i++; 
                   } 
           else if (fi->isFile()) 
                   {m_ListItem   = fname; 
                    int pos_ext  = fname.findRev('.'); 
                    if (pos_ext != -1) 
                       {QString ext  = fname.mid(pos_ext+1,1).lower(); 
                        if (i==0) pQPopupMenu->insertSeparator (); // inserer un separateur avant le premier element 
                        //if       (list_type==0) 
                        //         pQPopupMenu->insertItem(QPixmap(m_PathAppli +"QLightPadSys/filetxt_Icon.png" ), fname.left(pos_ext), this, SLOT( PasteTextItem())); 
                        if (ext=="h"){int id = pQPopupMenu->insertItem(QPixmap(m_PathAppli + "QLightPadSys/filehtm_Icon.png" ),  fname.left(pos_ext), this, SLOT( PasteTextItemH())); 
                                      pQPopupMenu->setItemParameter ( id, IS_HTML ); 
                                     } 
                        else          pQPopupMenu->insertItem(QPixmap(m_PathAppli + "QLightPadSys/filetxt_Icon.png" ),fname.left(pos_ext), this, SLOT( PasteTextItemT())); 
                        //else if  (list_type==TYP_OBSERVATION) 
                        //         pQPopupMenu->insertItem(QPixmap (filetxt_Icon), fname.left(pos_ext), this, SLOT( PasteTextItemObserv())); 
                        //else if  (list_type==TYP_PRESCRIPTION) 
                        //         pQPopupMenu->insertItem(QPixmap (filetxt_Icon), fname.left(pos_ext), this, SLOT( PasteTextItemPrescr())); 
                        i++; 
                       } 
                    else 
                       {if (i==0) pQPopupMenu->insertSeparator (); // inserer un separateur avant le premier element 
                        pQPopupMenu->insertItem(QPixmap(m_PathAppli +"QLightPadSys/filenew_Icon.png" ), fname, this, SLOT( PasteListItem())); 
                        i++; 
                       } 
                   }// endelse if (fi->isFile()) 
           ++it; 
          } 
 
    return i; 
} 
 
//----------------------------------------- OnMenuAboutToHide ----------------------------------------------------------------- 
void MyEditText::OnMenuAboutToHide () 
{if (m_pQtextMenuBulle) delete m_pQtextMenuBulle; 
 m_pQtextMenuBulle = 0; 
} 
 
//----------------------------------------- OnMenuHighlighted ----------------------------------------------------------------- 
// ACTION: lors du passage de la souris au dessus d'une option de menu, verifie si c'est elle correspond à un 
//         fichier texte et auquel cas l'affiche en bulle d'aide à coté du menu 
 
void MyEditText::OnMenuHighlighted ( int id ) 
{if (id != -1 && m_pQPopupMenu) 
    {long nb_lign     = 0; 
     int param        = m_pQPopupMenu->itemParameter (id); 
     m_ListItem       = m_pQPopupMenu->text(id); 
     QString path     = m_PathGlossaire + INSERTION_ROOT + QDir::separator() + m_list_name + QDir::separator() + m_ListItem; 
     QString bulleTxt = ""; 
     if (param == IS_HTML) 
        {CGestIni::Param_UpdateFromDisk(path+".html", bulleTxt); 
        } 
     else 
         {bulleTxt = CGenTools::qstringLoad(path+".txt", CR_TO_HTML, &nb_lign); 
          if (bulleTxt.left(6) != "<html>") 
             {if (bulleTxt.length()) 
                 {bulleTxt.prepend("<html><qt><head><meta name=\"qrichtext\" content=\"1\" /></head><body>" 
                                  "<p><font color=#000000 face=\"Arial\">"); 
                  bulleTxt += "</p></font></body></qt></html>"; 
                 } 
             } 
         } 
     if (bulleTxt!="") 
        {TimedDestroyMenuBulle(); 
         m_pQtextMenuBulle = new CMyBulleWidget (bulleTxt      , 
                                                 m_pQPopupMenu->width()-38    , 
                                                 m_pQPopupMenu->x()        , 
                                                 m_pQPopupMenu->y()        , 
                                                 m_PathDrTux               , 
                                                 this, //qApp->mainWidget () ,   //(QWidget*) G_pCDrTuxApp->m_pDrTux 
                                                 "CMyBulle"                , 
                                                  Qt::WStyle_NormalBorder); //Qt::WType_Popup 
        } 
     else 
       {TimedDestroyMenuBulle(); 
       } 
    } 
 
} 
 
//----------------------------------------- OnMenuActivated ----------------------------------------------------------------- 
// ACTION: Slot activé lorsque une option du popup est survolée, il est noté alors le texte de l'option popup dans la 
//         variable de la classe: m_ListItem.  cela permet de noter en fait le nom de la liste deroulante, et de le connaitre 
//         lors de la selection. 
 
void MyEditText::OnMenuActivated (int id) 
{if (id != -1 && m_pQPopupMenu) 
    {m_ListItem   = m_pQPopupMenu->text(id); 
    } 
} 
 
//----------------------------------------- TimedDestroyMenuBulle ------------------------------------------------------------------- 
void MyEditText::TimedDestroyMenuBulle() 
{if (m_pQtextMenuBulle) 
    {delete m_pQtextMenuBulle; 
     m_pQtextMenuBulle = 0; 
    } 
} 
//----------------------------------------- PasteListItem ------------------------------------------------------------------- 
// ACTION: insere dans le texte, le texte de l'item du popup selectionné (normalement element simple d'une liste) 
 
void MyEditText::PasteListItem () 
{InsertString(m_ListItem, " "); 
} 
 
//----------------------------------------- PasteFolderItem ----------------------------------------------------------------- 
// ACTION: insere dans le texte, le texte de l'item du popup selectionné (normalement nom d'une reference à une liste) 
//         en le mettant en bleu 
 
void MyEditText::PasteFolderItem () 
{setCurrentFont(m_LastFont); 
 setUnderline(FALSE); 
 setColor(QColor(0x00,0x00,0x00)); 
 insert(QString(" ")); 
 setColor(QColor(0x00,0x00,0xFF)); 
 setUnderline(FALSE); 
 insert(m_ListItem); 
 setColor(QColor(0x00,0x00,0x00)); 
 setUnderline(FALSE); 
} 
//----------------------------------------- PasteTextItemH ------------------------------------------------------------------ 
// ACTION: insere dans le texte, le texte du fichier dont le nom est l'item du popup selectionné (normalement nom dun fichier) 
void MyEditText::PasteTextItemH () 
{PasteTextItem (".html"); 
} 
//----------------------------------------- PasteTextItemT ------------------------------------------------------------------ 
// ACTION: insere dans le texte, le texte du fichier dont le nom est l'item du popup selectionné (normalement nom dun fichier) 
void MyEditText::PasteTextItemT () 
{PasteTextItem (".txt"); 
} 
//----------------------------------------- PasteTextItem ------------------------------------------------------------------ 
// ACTION: insere dans le texte, le texte du fichier dont le nom est l'item du popup selectionné (normalement nom dun fichier) 
void MyEditText::PasteTextItem (const QString &ext) 
{QString path = m_PathGlossaire + QString(INSERTION_ROOT) + QDir::separator() + m_list_name + QDir::separator() + m_ListItem + ext; 
 PasteFileItem (path); 
} 
 
//----------------------------------------- PasteFileItem ------------------------------------------------------------------ 
// ACTION: insere dans le texte, le texte du fichier dont le nom est l'item du popup selectionné (normalement nom dun fichier) 
void MyEditText::PasteFileItem(QString path) 
{int ori_para, ori_index, deb_para, deb_index, end_para, end_index; 
  //...................... on lit le texte dans un bloc memoire ............................. 
  QString text = ""; 
  CGestIni::Param_UpdateFromDisk(path, text); 
  getCursorPosition (&ori_para, &ori_index ); 
  getSelection(&deb_para, &deb_index, &end_para, &end_index); 
 
  long text_len =text.length(); 
  if ( QStyleSheet::mightBeRichText( text ) ) 
     {    int  isResolv   =  0; 
          long pos_deb    =  0; 
          long pos_end    =  0; 
          QString command = ""; 
          //...... eliminer les saloperies après le dernier tag HTML .............. 
          while (text_len>0 && text[text_len -1 ] != '>') --text_len; 
          text.truncate (text_len ); 
          //............... voir si faut pas eliminer un titre .................... 
          int deb = text.find("$start_title", 0,FALSE); 
          int end = text.find("$end_title",   0,FALSE); 
          if ( deb !=-1 && end != -1 ) text.remove(deb,end-deb+10); 
 
          //................ voir si ya pas une commande .......................... 
          pos_deb = text.find("{{"); 
          if (pos_deb != -1) isResolv = 1;  // y a une commande à resoudre 
          while (pos_deb !=-1) 
             {pos_end = text.find("}}", pos_deb); 
              if (pos_end != -1) 
                 {command = text.mid(pos_deb+2, pos_end - pos_deb - 2); 
                  if (command.length()>0) 
                     {emit Sign_ExePlugin(command); 
                     } 
                  if (command.length() != 0) ++isResolv; 
                  text.replace(pos_deb, pos_end - pos_deb+2, command); 
                 } 
              pos_deb = text.find("{{", pos_deb+2); 
             } 
          if (isResolv==1) text = "";  // si toujours 1 aucune commande résolue 
     } 
  else 
     {htmlConvert( text, MyEditText::HtmlConvertSp, this); 
     } //end else if if ( !QStyleSheet::mightBeRichText( text ) ) 
  if (text.length()) 
     {setCursorPosition (ori_para, ori_index ); 
      setSelection(deb_para, deb_index, end_para, end_index); 
      cut(); 
      moveCursor (QTextEdit::MoveBackward);  //MoveBackward, MoveForward 
      moveCursor (QTextEdit::MoveForward);   //MoveBackward, MoveForward 
     } 
  //QFont font = this->font(); 
  //qDebug(QString::number(font.pointSize ()) ); 
#ifdef Q_WS_X11 
  //setUpdatesEnabled (FALSE ); 
  CHtmlTools::insertHtml(this, text); 
  //zoomOut (); 
  //setUpdatesEnabled (TRUE ); 
  //zoomIn(); 
  //zoomTo (m_Zoom); 
  //ensureCursorVisible(); 
#else 
  //setUpdatesEnabled (FALSE ); 
  char *pt = (char*)(const char*) text; 
  CHtmlTools::insertHtml(this, text); 
  //zoomIn (); 
  //setUpdatesEnabled (TRUE ); 
  //zoomOut(); 
#endif 
} 
 
//------------------------------------- htmlConvert ---------------------------------------------------------------- 
void MyEditText::htmlConvert( QString &htmlStr, int flag /* = HtmlConvert*/ , const QTextEdit *pQText_Edit /*=0*/) 
{//..................... si ajout header html ou conversion html .......................................... 
 if (flag == MyEditText::AddHtmlHeader || 
     flag == MyEditText::HtmlConvert   || 
     flag == MyEditText::HtmlConvertSp       // rajoute un espace devant si 1 seule ligne (nb_ligne==0) 
    ) 
    {//.......................... si conversion html ........................................................ 
     if (flag == MyEditText::HtmlConvert|| flag == MyEditText::HtmlConvertSp) 
            {htmlStr.replace('<',"<"); 
         htmlStr.replace('>',">"); 
         long         p    = 0; 
         long        deb   = p; 
         int    nb_ligne   = 0; 
         while (p<(long)htmlStr.length()) 
                {char cc = htmlStr.at(p).latin1(); 
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
 
//------------------------------------- EmuleReturnKeyPress ---------------------------------------------------------------- 
// ACTION: simule l'action de l'appui de la touche return cela permet de corriger le fameux bug 
//         du double interligne 
 
void MyEditText::EmuleReturnKeyPress() 
{//............. on simule un appui sur return (avec bug CRLF corrigé) ............ 
  QKeyEvent keyEvent  ( QEvent::KeyPress,   Qt::Key_Enter, '\n', Qt::ControlButton, "\n"); 
  QTextEdit::event (&keyEvent); 
  QKeyEvent keyEvent2 ( QEvent::KeyRelease, Qt::Key_Enter, '\n', Qt::ControlButton, "\n"); 
  QTextEdit::event (&keyEvent2); 
} 
 
//------------------------------------- InsertString ---------------------------------------------------------------- 
// ACTION: insère à l'emplacement du curseur, la chaine de caractère passée en entrée, avec 
//         interpretation CAD dès qu'une zone de ce texte, est entre gillements, elle est considérée 
//         comme une zone de liste déroulante et est mise en bleue: "color:#0000ff" 
// ENTREE: const char *str: contient la chaine originale. 
//         const char *prefix: prefix à ajouter au devant de la chaine. 
 
void MyEditText::InsertString(const char *str, const char *prefix /* = 0 */) 
{QString string  = str; 
 setUnderline(FALSE); 
 setColor(QColor(0x00,0x00,0x00)); 
 
 if (prefix) insert(QString(prefix)+string); 
 else        insert(string); 
} 
 
 
 
//---------------------------- DialogGetString ----------------------------------------------- 
int MyEditText::DialogGetString(QWidget * parent, const char* title, QString &string) 
{CGenTools tools; 
 return tools.DialogGetString(parent, title,string); 
 /* 
 FormDlgGetString*dlg = new FormDlgGetString(parent,"GetString_Dial",TRUE); 
 if (dlg ==0) return QDialog::Rejected; 
 dlg->setCaption(title); 
 dlg->lineEditString->setText(string); 
 if (dlg->exec()==QDialog::Accepted) 
    {string = dlg->lineEditString->text(); 
     delete dlg; 
     return QDialog::Accepted; 
    } 
 delete dlg; 
 return QDialog::Rejected; 
 */ 
} 
 
//---------------------------------------------------- InsererChampInsertion --------------------------------------------------------------- 
void MyEditText::InsererChampInsertion() 
{Dlg_InsertList *dlg = new Dlg_InsertList(this, "InsertList", TRUE); 
 if (dlg == 0)                          return; 
 dlg->initDialog( *m_pInsertFieldList ); 
 if (dlg->exec() != QDialog::Accepted)   {delete dlg; return;} 
 QListBoxItem *pQListBoxItem  = dlg->listBox_InsertList->selectedItem(); 
 if (pQListBoxItem)  InsertString(pQListBoxItem->text()); 
 delete dlg; 
} 
 
//-------------------------------- MessageAlert ------------------------------------------------------------------------- 
int MyEditText::MessageAlert(QWidget * parent, const char* title, const char* message, const char* button1, const char* button2,const char* button3,const char* button4) 
{CGenTools tools; 
 return tools.MessageAlert(parent, title, message, button1, button2, button3, button4); 
} 
 
//---------------------------- qstringLoad ------------------------------------------------- 
QString MyEditText::qstringLoad(const char *path, int flag/*=0*/, long *nb_ligne/*=0*/) 
{CGenTools tools; 
 return tools.qstringLoad(path, flag, nb_ligne); 
} 
 
#endif 





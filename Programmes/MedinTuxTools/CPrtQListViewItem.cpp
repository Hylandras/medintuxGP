/********************************* CPrtQListViewItem.cpp **************************
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
// C++ Implementation: CPrtQListViewItem.cpp
//
// Description:
//
//
// Author: Eric MAEKER <eric.maeker.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "CPrtQListViewItem.h"
#include <qfontmetrics.h>
#include <qwidget.h>
#include <qobject.h>
#include <qlistview.h>
#include <qtimer.h>
#include <qheader.h>
#include <qmessagebox.h>
#include <qapplication.h>




//=============================================================== C_DragQListView ===========================================================

static const int GS_autoopenTime = 750;


//-------------------------------- C_DragQListView ----------------------------------------------------
C_DragQListView::C_DragQListView( QString *ptr, QWidget *parent, const char *name )
    : QListView( parent, name )  /*, m_oldCurrent( 0 ), m_Prop_0(""), m_Prop_1(""), m_Prop_2(""), m_pCurPath(ptr), m_movedItem(0), m_dropItem( 0 ), m_mousePressed( FALSE ) */
{   m_oldCurrent = 0;
    m_Prop_0=""; 
    m_Prop_1=""; 
    m_Prop_2="";
    m_pCurPath=ptr; 
    m_movedItem=0; 
    m_dropItem= 0 ; 
    m_mousePressed= FALSE;
    m_autoopen_timer = new QTimer( this );
    setAcceptDrops( TRUE );
    viewport()->setAcceptDrops( TRUE );
    removeState(TO_DEBUG);
    connect( m_autoopen_timer, SIGNAL( timeout() ),   this, SLOT( openFolder() ) );
}
//-------------------------------- C_DragQListView ----------------------------------------------------
C_DragQListView::C_DragQListView( QWidget *parent, const char *name )
    : QListView( parent, name )  /*, m_oldCurrent( 0 ),  m_Prop_0(""), m_Prop_1(""), m_Prop_2(""), m_pCurPath(0), m_movedItem(0), m_dropItem( 0 ), m_mousePressed( FALSE )*/
{   m_oldCurrent = 0;
    m_Prop_0=""; 
    m_Prop_1=""; 
    m_Prop_2="";
    m_pCurPath=0; 
    m_movedItem=0; 
    m_dropItem= 0 ; 
    m_mousePressed= FALSE;
    m_autoopen_timer = new QTimer( this );
    setAcceptDrops( TRUE );
    viewport()->setAcceptDrops( TRUE );
    removeState(TO_DEBUG);
    connect( m_autoopen_timer, SIGNAL( timeout() ),   this, SLOT( openFolder() ) );
}
//-------------------------------- contentsDragEnterEvent ----------------------------------------------------
void C_DragQListView::contentsDragEnterEvent( QDragEnterEvent *e )
{   m_oldCurrent     = currentItem();
    QListViewItem *i = itemAt( contentsToViewport(e->pos()) );
    if ( i )
       {m_dropItem = i;
        m_autoopen_timer->start( GS_autoopenTime );
       }
}

//-------------------------------- contentsDragMoveEvent ----------------------------------------------------
void C_DragQListView::contentsDragMoveEvent( QDragMoveEvent *e )
{   //if ( ! (QUriDrag::canDecode(e) || QTextDrag::canDecode(e)||e->provides ( "medintux/rubdrag" ) ) ) {   e->ignore();  return; }
    QPoint vp = contentsToViewport( e->pos() );
    QListViewItem *i = itemAt( vp );
    bool dragBreak = FALSE;
    emit Sign_DragMoveEvent(i, dragBreak);
    if (dragBreak) {e->ignore(e->answerRect() );  return; }
    //if ( i ) {
        setSelected( i, TRUE );
        e->accept();
        if ( i != m_dropItem )
           {
            m_autoopen_timer->stop();
            m_dropItem = i;
            m_autoopen_timer->start( GS_autoopenTime );
          }
        switch ( e->action() ) {
        case QDropEvent::Copy:
            break;
        case QDropEvent::Move:
            e->acceptAction();
            break;
        case QDropEvent::Link:
            e->acceptAction();
            break;
        default:
            ;
        }
   // } else {
   //     e->ignore();
   //     m_autoopen_timer->stop();
   //     m_dropItem = 0;
   // }
}

//-------------------------------- contentsDragLeaveEvent ----------------------------------------------------
void C_DragQListView::contentsDragLeaveEvent( QDragLeaveEvent *    /*e*/ )
{   m_autoopen_timer->stop();
    m_dropItem  = 0;
    m_movedItem = 0;
    if (m_oldCurrent) setCurrentItem( m_oldCurrent );
    if (m_oldCurrent) setSelected(    m_oldCurrent, TRUE );
}
//-------------------------------- dragClose ----------------------------------------------------
void C_DragQListView::dragClose()
{   m_mousePressed = FALSE;
    m_autoopen_timer->stop();
    m_dropItem  = 0;
    m_movedItem = 0;
}

//-------------------------------- contentsDropEvent ----------------------------------------------------
void C_DragQListView::contentsDropEvent( QDropEvent *e )
{   m_autoopen_timer->stop();
    QString           verb = "";
    QListViewItem *itemDst = itemAt( contentsToViewport(e->pos()) );
    //if ( itemDst )
        if (e->provides ( "medintux/rubdrag" )  )
           { QString dropStr(e->encodedData ( "medintux/rubdrag" ));
             switch ( e->action() )
                    {case QDropEvent::Copy:
                     verb = "Copy";
                     break;
                     case QDropEvent::Move:
                     verb = "Move";
                     e->acceptAction();
                     break;
                     case QDropEvent::Link:
                     verb = "Link";
                     e->acceptAction();
                     break;
                     default:
                     verb = "Unknown";
                    }
              if (m_movedItem)
                {//............. attention la liste peut avoir ete reorganisee avec Sign_ItemDraged_On_AnotherItem() ..............
                 //              les pointeurs des items ne sont plus valides !!! dont plantage si reutilises apres
                 if (IsStatePlaced(TO_DEBUG))
                    {if ( itemDst ) qDebug(tr ("\n---------------- Rub Drop item :       on item     : '") + itemDst->text(0) + "----------------\n'" + m_movedItem->text(0)  );
                     else           qDebug(tr ("\n---------------- Rub Drop item :       on listview : '") + this->name(0)    + "----------------\n'" + m_movedItem->text(0)  );
                    }
                 if (m_movedItem != itemDst)
                    {emit Sign_ItemDraged_On_AnotherItem(m_movedItem , itemDst , verb);
                     e->accept();
                    }
                 else e->ignore();
                }
             else
                {//............. attention la liste peut avoir ete reorganisee avec Sign_TextDraged_On_Item() ..............
                 //              les pointeurs des items ne sont plus valides !!! dont plantage si reutilises apres
                 if (IsStatePlaced(TO_DEBUG))
                    {if ( itemDst ) qDebug(tr ("\n---------------- Rub Drop item :       on item     : '") + itemDst->text(0) + "----------------\n'" + dropStr  );
                     else           qDebug(tr ("\n---------------- Rub Drop item :       on listview : '") + this->name(0)    + "----------------\n'" + dropStr  );
                    }
                 if (dropStr.length() )
                    {emit Sign_TextDraged_On_Item(itemDst ,            dropStr , verb);
                     e->accept();
                    }
                 else e->ignore();
                }
           }
        else if (e->provides ( "medintux/itemOrga" )  )
           { QString dropStr(e->encodedData ( "medintux/itemOrga" ));
             switch ( e->action() )
                    {case QDropEvent::Copy:
                     verb = "Copy";
                     break;
                     case QDropEvent::Move:
                     verb = "Move";
                     e->acceptAction();
                     break;
                     case QDropEvent::Link:
                     verb = "Link";
                     e->acceptAction();
                     break;
                     default:
                     verb = "Unknown";
                    }
              if (m_movedItem)
                {//............. attention la liste peut avoir ete reorganisee avec Sign_ItemDraged_On_AnotherItem() ..............
                 //              les pointeurs des items ne sont plus valides !!! dont plantage si reutilises apres
                 if (IsStatePlaced(TO_DEBUG))
                    {if ( itemDst ) qDebug(tr ("Drop item : '") + m_movedItem->text(0) + ("' on item     : '") + itemDst->text(0) + "'");
                     else           qDebug(tr ("Drop item : '") + m_movedItem->text(0) + ("' on listview : '") + this->name(0)    + "'");
                    }
                 if (m_movedItem != itemDst)
                    {
                     emit Sign_ItemDraged_On_AnotherItem(m_movedItem , itemDst , verb);
                     e->accept();
                    }
                 else e->ignore();
                }
             else
                {//............. attention la liste peut avoir ete reorganisee avec Sign_TextDraged_On_Item() ..............
                 //              les pointeurs des items ne sont plus valides !!! dont plantage si reutilises apres
                 if (IsStatePlaced(TO_DEBUG))
                    { if ( itemDst ) qDebug(tr ("Drop itemtext : '") + dropStr + ("' on item     : '") + itemDst->text(0) + "'");
                      else           qDebug(tr ("Drop itemtext : '") + dropStr + ("' on listview : '") + this->name(0)    + "'");
                    }
                 if (dropStr.length() )
                    {emit Sign_TextDraged_On_Item(itemDst ,            dropStr , verb);
                     e->accept();
                    }
                 else e->ignore();
                }
           }
        else if ( QUriDrag::canDecode(e) )
           { QStrList lst;
             QUriDrag::decode( e, lst );
             switch ( e->action() )
                    {case QDropEvent::Copy:
                     verb = "Copy";
                     break;
                     case QDropEvent::Move:
                     verb = "Move";
                     e->acceptAction();
                     break;
                     case QDropEvent::Link:
                     verb = "Link";
                     e->acceptAction();
                     break;
                     default:
                     verb = "Unknown";
                    }
             //............. attention la liste peut avoir ete reorganisee avec Sign_FilesDraged_On_Item() ..............
             //              les pointeurs des items ne sont plus valides !!! dont plantage si reutilises apres
             if (IsStatePlaced(TO_DEBUG))
                {if ( itemDst ) qDebug(tr ("Drop file on item     : '")+lst.at(0)+"'");
                 else           qDebug(tr ("Drop file on listview : '")+lst.at(0)+"'");
                }

             if (lst.count())
                {emit Sign_FilesDraged_On_Item(itemDst, lst, verb);
                 e->accept();
                }
             else e->ignore();
           }
        else if ( QTextDrag::canDecode(e) )
           { QString dropStr;
             QTextDrag::decode ( e, dropStr );
             switch ( e->action() )
                    {case QDropEvent::Copy:
                     verb = "Copy";
                     break;
                     case QDropEvent::Move:
                     verb = "Move";
                     e->acceptAction();
                     break;
                     case QDropEvent::Link:
                     verb = "Link";
                     e->acceptAction();
                     break;
                     default:
                     verb = "Unknown";
                    }
              if (m_movedItem)
                {//............. attention la liste peut avoir ete reorganisee avec Sign_ItemDraged_On_AnotherItem() ..............
                 //              les pointeurs des items ne sont plus valides !!! dont plantage si reutilises apres
                 if (m_movedItem != itemDst)
                    {emit Sign_ItemDraged_On_AnotherItem(m_movedItem , itemDst , verb);
                     e->accept();
                    }
                 else e->ignore();
                 if (IsStatePlaced(TO_DEBUG))
                    {if ( itemDst ) qDebug(tr ("Drop text item : '") + m_movedItem->text(0) + ("' on item     : '") + itemDst->text(0) + "'");
                     else           qDebug(tr ("Drop text item : '") + m_movedItem->text(0) + ("' on listview : '") + this->name(0)    + "'");
                    }
                }
             else
                {//............. attention la liste peut avoir ete reorganisee avec Sign_TextDraged_On_Item() ..............
                 //              les pointeurs des items ne sont plus valides !!! dont plantage si reutilises apres
                 if (IsStatePlaced(TO_DEBUG))
                    {if ( itemDst ) qDebug(tr ("Drop text : '") + dropStr + ("' on item     : '") + itemDst->text(0) + "'");
                     else           qDebug(tr ("Drop text : '") + dropStr + ("' on listview : '") + this->name(0)    + "'");
                    }
                 if (dropStr.length() )
                     {emit Sign_TextDraged_On_Item(itemDst ,            dropStr , verb);
                      e->accept();
                     }
                 else e->ignore();
                }
           }
        else
           {e->ignore();
           }
  m_movedItem = 0;
}

//-------------------------------- contentsMousePressEvent ----------------------------------------------------
void C_DragQListView::contentsMousePressEvent( QMouseEvent* e )
{   //if ( ! (QUriDrag::canDecode(e) || QTextDrag::canDecode(e)||e->provides ( "medintux/rubdrag" ) ) ) {   e->ignore();  return; }

    QListView::contentsMousePressEvent(e);
    QPoint p( contentsToViewport( e->pos() ) );
    QListViewItem *i = itemAt( p );
    if ( i && e->button()==Qt::LeftButton)
       {
        // if the user clicked into the root decoration of the item, don't try to start a drag!
        if ( p.x() > header()->cellPos( header()->mapToActual( 0 ) ) +
             treeStepSize() * ( i->depth() + ( rootIsDecorated() ? 1 : 0) ) + itemMargin() ||
             p.x() < header()->cellPos( header()->mapToActual( 0 ) ) )
           {m_presspos     = e->pos();
            m_mousePressed = TRUE;
            m_movedItem    = i;
            emit Sign_ItemClicked(i);
           }
    }
}

//-------------------------------- contentsMouseMoveEvent ----------------------------------------------------
void C_DragQListView::contentsMouseMoveEvent( QMouseEvent* e )
{   //if ( ! (QUriDrag::canDecode(e) || QTextDrag::canDecode(e)||e->provides ( "medintux/rubdrag" ) ) ) {   e->ignore();  return; }
        //bool dragBreak = FALSE;
        //emit Sign_DragMoveEvent(item, dragBreak);
        //if (dragBreak) {e->ignore();  return; }

    if ( m_mousePressed && ( m_presspos - e->pos() ).manhattanLength() > QApplication::startDragDistance() )
       {
        m_mousePressed      = FALSE;
        QString source      = "";
        QListViewItem *item = itemAt( contentsToViewport(m_presspos) );
        if ( item )  source = item->text(0) ; // fullPath(item);

            if ( m_pCurPath && m_pCurPath->length() && QFile::exists(*m_pCurPath+"/"+source) )
               {QUriDrag* ud   = new QUriDrag(viewport());
                ud->setFileNames( *m_pCurPath+"/"+source );
                if ( ud->drag() )
                    QMessageBox::information( this, "Drag source",
                    QString("Delete ") + QDir::convertSeparators(source), "Not implemented" );
               }
            else if (source.length())
               {  //if ( m_pCurPath) qDebug(*m_pCurPath + "/" + source);
                  QStoredDrag *d = new QStoredDrag( "medintux/itemOrga", this );
                  d->setEncodedData ( QCString(source) );
                  d->dragCopy();
               }
            else
               {QTextDrag* td = new QTextDrag ( viewport() );
                td->setText("");
                if ( td->drag() )
                    QMessageBox::information( this, "Drag source",
                    QString("Delete ") + QDir::convertSeparators(source), "Not implemented" );
               }
           //}
       }
}

//-------------------------------- contentsMouseReleaseEvent ----------------------------------------------------
void C_DragQListView::contentsMouseReleaseEvent( QMouseEvent *e )
{   m_mousePressed = FALSE;
    m_movedItem    = 0;
    QListView::contentsMouseReleaseEvent(e);
}

//-------------------------------- openFolder ----------------------------------------------------
void C_DragQListView::openFolder()
{
    m_autoopen_timer->stop();
    if ( m_dropItem && !m_dropItem->isOpen() )
       {m_dropItem->setOpen( TRUE );
        m_dropItem->repaint();
       }
}


//=============================================================== C_DragQListViewItem ===========================================================
//-------------------------------- C_DragQListViewItem ----------------------------------------------------
C_DragQListViewItem::C_DragQListViewItem(QListViewItem * pQListViewItem  )
    : QListViewItem(pQListViewItem)
{
  m_LightBackground = FALSE;
  setDragEnabled ( TRUE );
  columnStyleInitialise( pQListViewItem->listView ()->columns() );
  m_State = 0;
}

//-------------------------------- C_DragQListViewItem ----------------------------------------------------
C_DragQListViewItem::C_DragQListViewItem(QListView     * listview   )
    : QListViewItem(listview)
{ m_LightBackground = FALSE;
  setDragEnabled ( TRUE );
  columnStyleInitialise( listview->columns() );
  m_State = 0;
}
//-------------------------------- C_DragQListViewItem ----------------------------------------------------
C_DragQListViewItem::C_DragQListViewItem(QListViewItem * pQListViewItem ,
				         QListViewItem * after ,
				         const QString libelle )
    : QListViewItem(pQListViewItem, after, libelle)
{
  m_LightBackground = FALSE;
  setDragEnabled ( TRUE );
  columnStyleInitialise( pQListViewItem->listView()->columns() );
  m_State = 0;
}

//-------------------------------- C_DragQListViewItem ----------------------------------------------------
C_DragQListViewItem::C_DragQListViewItem(QListView     * listview ,
				         QListViewItem * after ,
				         const QString libelle )
    : QListViewItem(listview, after, libelle)
{ m_LightBackground = FALSE;
  setDragEnabled ( TRUE );
  columnStyleInitialise( listview->columns() );
  m_State = 0;
}

//-------------------------------- columnStyleInitialise ----------------------------------------------------
/*! \brief Prépare un tableau de booléen qui contiendront le style à utiliser : bold, italic.
 *  \sa columnIsBold , columnIsItalic
*/
void C_DragQListViewItem::columnStyleInitialise(int nbColumns)
{ m_columnIsBold.resize(nbColumns);
  m_columnIsItalic.resize(nbColumns);
  m_columnIsUnderline.resize(nbColumns);
  for (int i = 0; i< nbColumns;++i)
  { m_columnIsBold[i]      = FALSE;
    m_columnIsItalic[i]    = FALSE;
    m_columnIsUnderline[i] = FALSE;
  }
}

//-------------------------------- paint ----------------------------------------------------
/*! \brief Réimplémentation de la fonction paint du ListViewItem pour l'esthétisme du bestiaux.
 *  Permet d'utiliser les tags HTML : <b></b> , <i></i>  et <u></u>  pour l'ensemble de la colonne uniquement.
*/
void C_DragQListViewItem::paintCell( QPainter * p, const QColorGroup & cg, int column, int width, int align )
{ QString tmp = "";
  // On déclare une nouvelle couleur
  QColor bk, txt;
  QFont font = p->font();

  // Mettre en gras la ligne si le document à été modifié et reste à sauvegarder
  //if (m_Modified)
  //{ font.setBold( TRUE );
  //  p->setFont( font );
  //}
  if (get_Prop_0()[0]=='R') m_columnIsBold[column]      = TRUE;
  if (get_Prop_0()[1]=='A') m_columnIsUnderline[column] = TRUE;
  // Recherche les <b> et <i> dans le libelle de la colonne
  tmp = QListViewItem::text(column);
  if (tmp.find("<b>") != -1)
  { // sauvegarde le style à utiliser
    m_columnIsBold[column] = TRUE;
    // enleve les tags html
    tmp.replace("<b>","");
    tmp.replace("</b>","");
    QListViewItem::setText(column, tmp);
  }
  if (QListViewItem::text(column).find("<i>") != -1)
  { // sauvegarde le style à utiliser
    m_columnIsItalic[column] = TRUE;
    // enleve les tags html
    tmp.replace("<i>","");
    tmp.replace("</i>","");
    QListViewItem::setText(column, tmp);
  }
  if (QListViewItem::text(column).find("<u>") != -1)
  { // sauvegarde le style à utiliser
    m_columnIsUnderline[column] = TRUE;
    // enleve les tags html
    tmp.replace("<u>","");
    tmp.replace("</u>","");
    QListViewItem::setText(column, tmp);
  }

  if (m_columnIsBold[column])
  { font.setBold( TRUE );
    p->setFont( font );
  }
  if (m_columnIsItalic[column])
  { font.setItalic( TRUE );
    p->setFont( font );
  }
  if (m_columnIsUnderline[column])
  { font.setUnderline( TRUE );
    p->setFont( font );
  }

  // En fonction de la ligne on change le background
  int pos    = QListViewItem::itemPos();
  int height = QListViewItem::height () ;
  if (((pos / height) % 2) == 0)
  { bk = Theme::getRapidAccess_LighterBackground();
    m_LightBackground=TRUE;
  }
  else
  { bk = Theme::getRapidAccess_DarkerBackground();
    m_LightBackground=FALSE;
  }

  // En fonction du type on change la couleur de la police
  //if (m_Type.left(4) == "2003")      txt = Theme::getRapidAccess_FontColor_Obs();
  //else if (m_Type.left(4) == "2002") txt = Theme::getRapidAccess_FontColor_Prescr();
  //else if (m_Type.left(4) == "2008") txt = Theme::getRapidAccess_FontColor_Doc();
  //else if (m_Type.left(4) == "2006") txt = Theme::getRapidAccess_FontColor_Terrain();

  // On sauvegarde la palette de couleurs
  QColorGroup _cg( cg );
  QColor old_bk  = _cg.background();
  QColor old_txt = _cg.text();

  // On modifie la palette de couleurs en définissant
  // notre nouvelle couleur comme couleur de texte

  _cg.setColor( QColorGroup::Base, bk );
  _cg.setColor( QColorGroup::Text, txt );
  _cg.setColor( QColorGroup::HighlightedText, txt );
  _cg.setColor( QColorGroup::Highlight, Theme::getRapidAccess_HighLightColor() );



  // On appelle la méthode virtuelle de l'ancêtre avec la nouvelle palette
  QListViewItem::paintCell( p, _cg, column, width, align );

  _cg.setColor( QColorGroup::Base, old_bk );
  //p->drawLine ( rect.width()-2, 0, rect.width()-2, rect.height() );
  if (IsStatePlaced(1)&&column>=CP_Deb && column<=CP_Fin)      // placé dans les classurs
     {QPen _pen =  p->pen ();
      p->setPen(Qt::SolidLine);
      p->setPen ( QColor("#DADADA") );
      //p->drawLine ( 0, 0, 0, rect.height() );
      //int w = QListViewItem::width()-1;
      //int w = QListViewItem::width (  p->fontMetrics(), QListViewItem::listView(), column );
      int w = QListViewItem::listView()->columnWidth (column )-1;
      int h = QListViewItem::height()-1;
      p->drawLine ( 0, 0, w, 0);  // horizontal haut
      p->drawLine ( 0, h, w, h);  // horizontal bas
      p->drawLine ( 0, 0, 0, h);  // vertical gauche
      p->drawLine ( w, 0, w, h);  // vertical droit
      p->setPen(_pen);
     }
 else if (get_Prop_0()[0]=='R')
    { QPen _pen =  p->pen ();
      p->setPen(Qt::SolidLine);
      p->setPen ( QColor("#000000") );
      //p->drawLine ( 0, 0, 0, rect.height() );
      //int w = QListViewItem::width()-1;
      //int w = QListViewItem::width (  p->fontMetrics(), QListViewItem::listView(), column );
      int w = QListViewItem::listView()->columnWidth (column )-1;
      int h = QListViewItem::height()-1;
      p->drawLine ( 0, 0, w, 0);  // horizontal haut
      p->setPen ( QColor("#DADADA") );
      p->drawLine ( 0, h, w, h);  // horizontal bas
      //p->drawLine ( 0, 0, 0, h);  // vertical gauche
      //p->drawLine ( w, 0, w, h);  // vertical droit
      //p->setPen(_pen);
    }
}

//=============================================================== CPrtQListViewItem ===========================================================
/*! \class CPrtQListViewItem
 * \brief réimplémentation de la classe QListViewItem dont elle hérite pour améliorer l'affichage. Une réécriture de la fonction compare permet de trier la colonne des date correctement.
 * Attention cette fonction est dépendante de la classe Theme pour les couleurs.
*/


//-------------------------------- CPrtQListViewItem ----------------------------------------------------
CPrtQListViewItem::CPrtQListViewItem(QListViewItem * listview ,
				     QListViewItem * after ,
				     const QString libelle )
    : QListViewItem(listview, after, libelle)
{ // initialise les valeurs
  m_Libelle         = libelle;
  m_signUser        = "";
  m_User            = "";
  m_LightBackground = FALSE;
  m_ID_Rublist      = "";
  m_SubType         = "";
  m_Type            = "";
  m_pkDoc           = "";
  m_GUID            = "";
  m_NomPatient      = "";
  m_PrenomPatient   = "";
  m_Modified        = FALSE;
  columnStyleInitialise( listview->listView()->columns() );
}

CPrtQListViewItem::CPrtQListViewItem(QListView     * listview ,
				     QListViewItem * after ,
				     const QString libelle )
    : QListViewItem(listview, after, libelle) /*, m_Libelle(libelle) */
{ // initialise les valeurs
  m_Libelle         = libelle;
  m_signUser        = "";
  m_User            = "";
  m_LightBackground = FALSE;
  m_ID_Rublist      = "";
  m_SubType         = "";
  m_Type            = "";
  m_pkDoc           = "";
  m_GUID            = "";
  m_NomPatient      = "";
  m_PrenomPatient   = "";
  m_Modified        = FALSE;
  columnStyleInitialise( listview->columns() );
}


//-------------------------------- CPrtQListViewItem ----------------------------------------------------
CPrtQListViewItem::CPrtQListViewItem(QListView * listview,
				     const QString &libelle ,
 				     const QString &iD_Rublist,
				     const QString &date,
				     const QString &pkDoc,
				     const QString &subType,
				     const QString &type,
				     const QString &signUser,
				     const QString &user)
    : QListViewItem(listview)  /*, m_Type(type), m_ID_Rublist(iD_Rublist), m_User(user), m_SubType(subType), m_Libelle(libelle) */
{ QString tmp     = "";
  m_Type          = type;
  m_ID_Rublist    = iD_Rublist;
  m_User          = user;
  m_SubType       = subType;
  m_Libelle       = libelle;
  m_pkDoc         = pkDoc;
  m_signUser      = signUser;
  m_GUID          = "";
  m_NomPatient    = "";
  m_PrenomPatient = "";
  if (date != "" ) m_Date = QDateTime::fromString(date, Qt::ISODate);


  if (m_Type.left(4) == "2003") tmp = "O";
  else if (m_Type.left(4) == "2002") tmp = "P";
  else if (m_Type.left(4) == "2008") tmp = "D";

  // Elimine les terrains
  if (m_Type.left(4) != "2006")
  { switch (listview->columns())
    { case 1 : { QListViewItem::setText(0, m_Libelle );
		 break ;
	       }
      case 3 : { QListViewItem::setText(0, tmp );
		 QListViewItem::setText(1,m_Date.toString("dd MMM yyyy") );
		 QListViewItem::setText(2,m_Libelle );
		 break ;
	       }
      case 5 : { //QListViewItem::setText(TYPE_COLUMN, tmp );
		 QListViewItem::setText(LISTVIEW_DATE_COLUMN,m_Date.toString("dd MMM yyyy") );
		 QListViewItem::setText(LISTVIEW_LIBELLE_COLUMN,m_Libelle );
		 break ;
	       }

    } // end switch
  }
  m_Modified=FALSE;
  columnStyleInitialise( listview->columns() );
}


//-------------------------------- CPrtQListViewItem ----------------------------------------------------
CPrtQListViewItem::CPrtQListViewItem(QListViewItem * listview,
				     const QString &libelle ,
 				     const QString &iD_Rublist,
				     const QString &date,
				     const QString &pkDoc,
				     const QString &subType,
				     const QString &type,
				     const QString &signUser,
				     const QString &user)
    : QListViewItem(listview) /* , m_Type(type), m_ID_Rublist(iD_Rublist), m_User(user), m_SubType(subType), m_Libelle(libelle) */
{ QString tmp     = "";
  m_Type          = type;
  m_ID_Rublist    = iD_Rublist;
  m_User          = user;
  m_SubType       = subType;
  m_Libelle       = libelle;
  m_pkDoc         = pkDoc;
  m_signUser      = signUser;
  m_GUID          = "";
  m_NomPatient    = "";
  m_PrenomPatient = "";
  if (date != "" ) m_Date = QDateTime::fromString(date, Qt::ISODate);

  if (m_Type.left(4) == "2003") tmp = "O";
  else if (m_Type.left(4) == "2002") tmp = "P";
  else if (m_Type.left(4) == "2008") tmp = "D";

  // Elimine les terrains
  if (m_Type.left(4) != "2006")
  { switch (listview->listView()->columns())
    { case 1 : { QListViewItem::setText(0, m_Libelle );
		 break ;
	       }
      case 3 : { QListViewItem::setText(0, tmp );
		 QListViewItem::setText(1,m_Date.toString("dd MMM yyyy") );
		 QListViewItem::setText(2,m_Libelle );
		 break ;
	       }
    } // end switch
  }
  m_Modified=FALSE;
  columnStyleInitialise( listview->listView()->columns() );
}

/*! \brief Destructeur de la classe qu'il faut réimplémenter sinon erreur lors des deletes d'items.
*/
CPrtQListViewItem::~CPrtQListViewItem()
{/* while (QListViewItem::childCount() != 0)
  { CPrtQListViewItem* pCprt = (CPrtQListViewItem*) QListViewItem::firstChild();
  delete pCprt;
  }*/
}

/*! \brief Prépare un tableau de booléen qui contiendront le style à utiliser : bold, italic.
 *  \sa columnIsBold , columnIsItalic
*/
void CPrtQListViewItem::columnStyleInitialise(int nbColumns)
{ columnIsBold.resize(nbColumns);
  columnIsItalic.resize(nbColumns);
  columnIsUnderline.resize(nbColumns);
  for (int i = 0; i< nbColumns;++i)
  { columnIsBold[i] = FALSE;
    columnIsItalic[i] = FALSE;
    columnIsUnderline[i] = FALSE;
  }
}

//-------------------------------- compare ----------------------------------------------------
/*! \brief Réimplentation de la fonction pour permettre le trie par date.
*/
int CPrtQListViewItem::compare( QListViewItem *i, int col, bool asc ) const
// Returns < 0 if this item is less than i, 0 if they are equal and > 0 if this item is greater than i.
{  CPrtQListViewItem* pCPrtQListViewItem = (CPrtQListViewItem*)i;
   int retour = 0;
   if( col == LISTVIEW_DATE_COLUMN ) // Si colonne de la date
   { if (m_Date > pCPrtQListViewItem->m_Date)  retour = 1;
     else if (m_Date == pCPrtQListViewItem->m_Date) retour = 0;
     else retour = -1;
     return retour;
   }
   else
      return QListViewItem::compare( i, col, asc );
}


//-------------------------------- setModified ----------------------------------------------------
/*! \brief définit l'item courant comme modifié. Sera utilisé pour gérer les rubrecords à sauvegarder dans la base par CMDI_GroupedView.
*/
void CPrtQListViewItem::setModified( bool m )
{ m_Modified = m; }


//-------------------------------- paint ----------------------------------------------------
/*! \brief Réimplémentation de la fonction paint du ListViewItem pour l'esthétisme du bestiaux.
 *  Permet d'utiliser les tags HTML : <b></b> , <i></i>  et <u></u>  pour l'ensemble de la colonne uniquement.
*/
void CPrtQListViewItem::paintCell( QPainter * p, const QColorGroup & cg, int column, int width, int align )
{ QString tmp = "";
  // On déclare une nouvelle couleur
  QColor bk, txt;
  QFont font = p->font();

  // Mettre en gras la ligne si le document à été modifié et reste à sauvegarder
  if (m_Modified)
  { font.setBold( TRUE );
    p->setFont( font );
  }

  // Recherche les <b> et <i> dans le libelle de la colonne
  tmp = QListViewItem::text(column);
  if (tmp.find("<b>") != -1)
  { // sauvegarde le style à utiliser
    columnIsBold[column] = TRUE;
    // enleve les tags html
    tmp.replace("<b>","");
    tmp.replace("</b>","");
    QListViewItem::setText(column, tmp);
  }
  if (QListViewItem::text(column).find("<i>") != -1)
  { // sauvegarde le style à utiliser
    columnIsItalic[column] = TRUE;
    // enleve les tags html
    tmp.replace("<i>","");
    tmp.replace("</i>","");
    QListViewItem::setText(column, tmp);
  }
  if (QListViewItem::text(column).find("<u>") != -1)
  { // sauvegarde le style à utiliser
    columnIsUnderline[column] = TRUE;
    // enleve les tags html
    tmp.replace("<u>","");
    tmp.replace("</u>","");
    QListViewItem::setText(column, tmp);
  }

  if (columnIsBold[column])
  { font.setBold( TRUE );
    p->setFont( font );
  }
  if (columnIsItalic[column])
  { font.setItalic( TRUE );
    p->setFont( font );
  }
  if (columnIsUnderline[column])
  { font.setUnderline( TRUE );
    p->setFont( font );
  }

  // En fonction de la ligne on change le background
  int pos    = QListViewItem::itemPos();
  int height = QListViewItem::height () ;
  if (((pos / height) % 2) == 0)
  { bk = Theme::getRapidAccess_LighterBackground();
    m_LightBackground=TRUE;
  }
  else
  { bk = Theme::getRapidAccess_DarkerBackground();
    m_LightBackground=FALSE;
  }

  // En fonction du type on change la couleur de la police
  if (m_Type.left(4) == "2003")      txt = Theme::getRapidAccess_FontColor_Obs();
  else if (m_Type.left(4) == "2002") txt = Theme::getRapidAccess_FontColor_Prescr();
  else if (m_Type.left(4) == "2008") txt = Theme::getRapidAccess_FontColor_Doc();
  else if (m_Type.left(4) == "2006") txt = Theme::getRapidAccess_FontColor_Terrain();

  // On sauvegarde la palette de couleurs
  QColorGroup _cg( cg );
  QColor old_bk  = _cg.background();
  QColor old_txt = _cg.text();

  // On modifie la palette de couleurs en définissant
  // notre nouvelle couleur comme couleur de texte

  _cg.setColor( QColorGroup::Base, bk );
  _cg.setColor( QColorGroup::Text, txt );
  _cg.setColor( QColorGroup::HighlightedText, txt );
  _cg.setColor( QColorGroup::Highlight, Theme::getRapidAccess_HighLightColor() );



  // On appelle la méthode virtuelle de l'ancêtre avec la nouvelle palette
  QListViewItem::paintCell( p, _cg, column, width, align );

  _cg.setColor( QColorGroup::Base, old_bk );

}


//-------------------------------- SetValue ----------------------------------------------------
void CPrtQListViewItem::setValue(const QString &iD_Rublist, const QString &type, const QString &user, const QString &subType)
{m_Type              = type;
 m_ID_Rublist        = iD_Rublist;
 m_User              = user;
 m_SubType           = subType;
}

//-------------------------------- GetValue ----------------------------------------------------
void CPrtQListViewItem::getValue(QString &iD_Rublist, QString &type, QString &user, QString &subType)
{type            = m_Type;
 iD_Rublist      = m_ID_Rublist;
 user            = m_User;
 subType         = m_SubType;
}

//-------------------------------- get_ID_Rublist ----------------------------------------------------
QString CPrtQListViewItem::get_ID_Rublist()
{ return m_ID_Rublist; }
//-------------------------------- GetPkDoc ----------------------------------------------------
QString CPrtQListViewItem::getPkDoc()
{ return m_pkDoc; }
//-------------------------------- GetType ----------------------------------------------------
QString CPrtQListViewItem::getType()
{ return m_Type; }
//-------------------------------- GetUser ----------------------------------------------------
QString CPrtQListViewItem::getUser()
{ return m_User; }
//-------------------------------- GetSubType ----------------------------------------------------
QString CPrtQListViewItem::getSubType()
{ return m_SubType; }
//-------------------------------- GetLibelle ----------------------------------------------------
QString CPrtQListViewItem::getLibelle()
{ return m_Libelle; }
//-------------------------------- getGUID ----------------------------------------------------
QString CPrtQListViewItem::getGUID()
{ return m_GUID; }
//-------------------------------- getNom ----------------------------------------------------
QString CPrtQListViewItem::getNom()
{ return m_NomPatient; }
//-------------------------------- getPrenom ----------------------------------------------------
QString CPrtQListViewItem::getPrenom()
{ return m_PrenomPatient; }
//-------------------------------- getAtcd ----------------------------------------------------
void* CPrtQListViewItem::getAtcd()
{ return m_pAtcd; }
//-------------------------------- GetBackgroundColor ----------------------------------------------------
bool CPrtQListViewItem::isLightBackground()
{ return m_LightBackground; }


//-------------------------------- setNom ----------------------------------------------------
void CPrtQListViewItem::setNom(QString n)
{ m_NomPatient = n; }
//-------------------------------- setPrenom ----------------------------------------------------
void CPrtQListViewItem::setPrenom(QString n)
{ m_PrenomPatient = n; }
//-------------------------------- setGUID ----------------------------------------------------
void CPrtQListViewItem::setGUID(QString n)
{ m_GUID = n; }
//-------------------------------- SetPkDoc ----------------------------------------------------
void CPrtQListViewItem::setPkDoc(QString n)
{ m_pkDoc = n; }
//-------------------------------- Set_ID_Rublist ----------------------------------------------------
void CPrtQListViewItem::set_ID_Rublist(QString& n)
{ m_ID_Rublist = n; }
void CPrtQListViewItem::set_ID_Rublist(int n)
{ m_ID_Rublist = QString::number(n); }
//-------------------------------- SetType ----------------------------------------------------
void CPrtQListViewItem::setType(QString& n)
{ m_Type = n; }
//-------------------------------- SetUser ----------------------------------------------------
void CPrtQListViewItem::setUser(QString& n)
{ m_User = n; }

void CPrtQListViewItem::setSignUser(QString& n)
{ m_signUser = n; }

//-------------------------------- SetSubType ----------------------------------------------------
void CPrtQListViewItem::setSubType(QString& n)
{ m_SubType = n; }
//-------------------------------- GetBackgroundColor ----------------------------------------------------
void CPrtQListViewItem::setLightBackground(bool c)
{ m_LightBackground = c; }
//-------------------------------- setAtcd ----------------------------------------------------
void  CPrtQListViewItem::setAtcd(void* pAtcd)
{ m_pAtcd = pAtcd; }

void CPrtQListViewItem::setDate(QString& n)
{ m_Date = QDateTime::fromString(n, Qt::ISODate); }

void CPrtQListViewItem::setDate(QDateTime& dt)
{ m_Date = dt; }

QString CPrtQListViewItem::getDate(QString& format)
{ return m_Date.toString( format ); }

QDateTime CPrtQListViewItem::getDate()
{ return m_Date; }


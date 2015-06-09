
/***************************************************************************
 *   Copyright (C) 2004 by Sébastien SIVERA & Sevin Roland                 *
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

#include "CCoolPopup.h"
#include <qdesktopwidget.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qbitmap.h>
#include <qsize.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qlabel.h>
#include <qapplication.h>

//====================================== C_MyLabel =======================================================

//------------------------------------------ C_MyLabel ----------------------------------------------------------------------
C_MyLabel::C_MyLabel (QWidget*         parent ,  const char*      name ,    WFlags f )
: QWidget (parent, name, f)
{
 m_Text          = "";
 m_ForeTextColor = QColor("#000");
}

//------------------------------------------ setText ----------------------------------------------------------------------
void C_MyLabel::setText(const QString &text)
{m_Text = text;
}

//------------------------------------------ setPixmap ----------------------------------------------------------------------
void C_MyLabel::setPixmap(const QPixmap &pixmap, int x /*=-1 */, int y /*=-1 */, int w /*=-1 */, int h /*=-1 */)
{if (w==-1) w = pixmap.width();
 if (h==-1) h = pixmap.height();
 m_Pixmap     = pixmap;
 bitBlt ( &m_Pixmap,  0,  0, &pixmap, x, y, w,  h, Qt::CopyROP, FALSE);
}

//------------------------------------------ setTextColor ----------------------------------------------------------------------
void C_MyLabel::setTextColor(const QColor &color)
{m_ForeTextColor = color;
}

//------------------------------------------ paintEvent ----------------------------------------------------------------------
void C_MyLabel::paintEvent( QPaintEvent *e )
{QPainter  p(this);
 QRect ur = e->rect();            // rectangle to update
 int dx =  ur.x();
 int dy =  ur.y();;
 bitBlt ( this,  dx,  dy, &m_Pixmap, dx, dy, ur.width(),  ur.height(), Qt::CopyROP, FALSE);

 int delta = 10;
 QRect rect(x()+delta,y()+delta,width()-delta*2,height()-delta*2);
 p.setFont (font());
 p.setPen( m_ForeTextColor );
 p.drawText (rect, Qt::AlignJustify|Qt::WordBreak|Qt::ExpandTabs, m_Text) ; //,  -1, 0, QTextParag ** internal = 0 Qt::AlignJustify
}


//====================================== CCoolPopup =======================================================
//------------------------------------------ CCoolPopup ----------------------------------------------------------------------
CCoolPopup::CCoolPopup  ( const QString pathImage /*= ""         */ ,
                          int           mode      /*=  WithFrame */ ,
                          int           textX     /*=0           */ ,
                          int           textY     /*=0           */ ,
                          int           textW     /*=0           */ ,
                          int           textH     /*=0           */ ,
                          const char*   text      /*=""              */ ,
                          QColor        textCol   /*=0           */ ,
                          QColor        textBack  /*=0           */ ,
                          long          tempo     /*=20          */ ,
                          int           growBy    /*=3           */ ,
                          const char   *name      /*=0            */ ,
                          WFlags        f         /*= WDest ructiveClose |
                                                      WType_TopLevel    |
                                                      WStyle_Customize  |
                                                      WStyle_StaysOnTop |
                                                      WStyle_NoBorder   */
                          )
    : QWidget (QApplication::desktop()->screen(0), name, f)
{   //........................Defini la taille originale de la fenetre ...........................
    m_Mode           = mode;         //
    m_GrowBy         = growBy;
    m_QLabel_InfosXY = 0;
    m_Text           = text;
    m_TextX          = textX;
    m_TextY          = textY;
    m_TextW          = textW;
    m_TextH          = textH;
    m_pC_MyLabel     = 0;
    m_QLabel_Txt     = 0;
    m_pDisplayTimer  = 0;

    // Calcule le point d'affichage du coin supérieur gauche de la fenêtre
    // en excluant la barre des taches de windows
    QDesktopWidget *pDesktop = QApplication::desktop();
    m_Screen.setX(pDesktop->availableGeometry().width()-2); // -2 pour pas coller le bord de l'écran
    m_Screen.setY(pDesktop->availableGeometry().height());
    // passe les nouveaux paramètres à la fenête
    // le bureau devient son parent

   if (m_Mode&WithFrame)
       {//................. un label pour l'image et cadre...........................
        m_QLabel_InfosXY = new QLabel(this);
        m_QLabel_InfosXY->setBackgroundMode ( Qt::FixedColor  ) ;
        m_QLabel_InfosXY->setFrameShape(QFrame::Box);
        m_QLabel_InfosXY->setFrameShadow(QFrame::Raised);
        m_QLabel_InfosXY->setLineWidth(2);
        m_QLabel_InfosXY->setMidLineWidth(2);
        m_QLabel_InfosXY->show();
     }
   if (m_Text!="")
      {if (m_Mode&WithTransLabel)
          {m_pC_MyLabel = new C_MyLabel(this);
           m_pC_MyLabel->setBackgroundMode ( Qt::NoBackground ) ; //Qt::FixedColor Qt::NoBackground
           m_pC_MyLabel->setText(m_Text);
           m_pC_MyLabel->setTextColor(textCol);
           m_pC_MyLabel->setGeometry(m_TextX, m_TextY, m_TextW, m_TextH);
           m_pC_MyLabel->show();
         }
       else
         {m_QLabel_Txt = new QLabel(this);
          m_QLabel_Txt->setBackgroundMode ( Qt::FixedColor ) ; //Qt::FixedColor Qt::NoBackground
          m_QLabel_Txt->setPaletteBackgroundColor (textBack );
          m_QLabel_Txt->setPaletteForegroundColor ( textCol  );
          m_QLabel_Txt->setText(m_Text);
          m_QLabel_Txt->setGeometry(m_TextX, m_TextY, m_TextW, m_TextH);
          m_QLabel_Txt->show();
         }
      }

    //................. charger l'image  ..............................
   setImage(pathImage);
   if (m_pC_MyLabel)  m_pC_MyLabel->setPixmap(m_Pixmap, m_TextX, m_TextY, m_TextW, m_TextH);
    //................. creation du timer  ..............................
    m_Tempo         = tempo;
    m_pDisplayTimer = new QTimer(this);
    if (m_pDisplayTimer) {
        // connect le signal timeout avec le slot d'affichage de la fenêtre
        connect( m_pDisplayTimer, SIGNAL(timeout()), this, SLOT(timerDisplayDone()) );
        // demarre le timer
        m_pDisplayTimer->start(m_Tempo, FALSE);
        // defini le mouvement à "Aucun"
        m_GrowUp = 0;
        Appear();
    }
}

//------------------------------------------ ~CCoolPopup ----------------------------------------------------------------------
CCoolPopup::~CCoolPopup()
{ disconnect( m_pDisplayTimer, SIGNAL(timeout()), this, SLOT(timerDisplayDone()) );
  if (m_QLabel_InfosXY) delete m_QLabel_InfosXY;  // effacer car ne font pas partie de l'appli
  if (m_pC_MyLabel)     delete m_pC_MyLabel;      // effacer car ne font pas partie de l'appli
  if (m_QLabel_Txt)     delete m_QLabel_Txt;      // effacer car ne font pas partie de l'appli
  if (m_pDisplayTimer)  delete m_pDisplayTimer;
qDebug("tou m'a touer");
}

//------------------------------------------ setText ----------------------------------------------------------------------
void CCoolPopup::setFont ( const QFont &ft )
{ if (m_pC_MyLabel) m_pC_MyLabel->setFont (ft);
  QWidget::setFont (ft);
}
//------------------------------------------ setTextColor ----------------------------------------------------------------------
void CCoolPopup::setTextColor(const QColor &textCol)
{ if (m_pC_MyLabel) m_pC_MyLabel->setTextColor(textCol);
}
//------------------------------------------ setText ----------------------------------------------------------------------
void CCoolPopup::setTextGeometry( int textX , int textY , int textW /* = -1 */ , int textH /* = -1 */ )
{if (m_pC_MyLabel==0) return;
 m_TextX = textX;
 m_TextY = textY;
 if (textW != -1) m_TextW = textW;
 if (textH != -1) m_TextH = textH;
 m_pC_MyLabel->setGeometry(m_TextX, m_TextY, m_TextW, m_TextH);
}

//------------------------------------------ setText ----------------------------------------------------------------------
void CCoolPopup::setText(const QString& text,
                          QColor        textCol   /*="#00F"       */ ,
                          QColor        textBack  /*=" #FFF"       */
                        )
{if (text.length()<=0 && m_pC_MyLabel==0)     return;
 if (m_Mode & CCoolPopup::WithTransLabel)
    {if (m_pC_MyLabel == 0 )
        {m_pC_MyLabel = new C_MyLabel(this);
         m_pC_MyLabel->setBackgroundMode ( Qt::NoBackground ) ;
         //m_pC_MyLabel->setPaletteBackgroundColor (textBack );
         m_pC_MyLabel->setPaletteForegroundColor ( textCol  );
        }
     if (   m_pC_MyLabel == 0) return;
     m_pC_MyLabel->setText(text);
     m_pC_MyLabel->show();
    }
 else
    {if (m_QLabel_Txt == 0)
        {m_QLabel_Txt = new QLabel(this);
         m_QLabel_Txt->setBackgroundMode ( Qt::FixedColor ) ;
         m_QLabel_Txt->setPaletteBackgroundColor (textBack );
         m_QLabel_Txt->setPaletteForegroundColor ( textCol  );
        }
     if (   m_QLabel_Txt == 0) return;
     m_QLabel_Txt->setText(text);
     m_QLabel_Txt->show();
    }
 m_Text = text;

}

//------------------------------------------ setImage ----------------------------------------------------------------------
QPixmap CCoolPopup::setImage(const QString path_image /* = "" */ , int mustAppear /*=0*/)
{if (m_Mode&WithFrame) return    setLabelImage(path_image  , mustAppear );
 else                  return    setBackgroundImage(path_image  , mustAppear );
}

//------------------------------------------ setLabelImage ----------------------------------------------------------------------
QPixmap CCoolPopup::setLabelImage(const QString path_image /* = "" */ , int mustAppear /*=0*/)
{
    QPixmap image;
    QFile file( path_image );
    image.load (path_image);
    hide();
    m_QLabel_InfosXY->setPixmap(image);
    m_QLabel_InfosXY->adjustSize();
    m_ImgHeight      = m_QLabel_InfosXY->frameGeometry().height(); // 251;
    m_ImgWidth       = m_QLabel_InfosXY->frameGeometry().width();  // 254;
    setGeometry(0,0,0,0);
    m_GrowUp = 0;
    if (mustAppear == CCoolPopup::AppearFlag) Appear();
    return image;
}
//------------------------------------------ setBackgroundImage ----------------------------------------------------------------------
QPixmap CCoolPopup::setBackgroundImage(const QString path_image /* = "" */ , int mustAppear /*=0*/)
{
    QImage image;
    QFile file( path_image );
    image= QImage(path_image);
    hide();
    QPixmap p;
    p.convertFromImage( image );

    if ( !p.mask() )
       {
            if ( image.hasAlphaBuffer() )
               {
                QBitmap bm;
                bm = image.createAlphaMask();
                p.setMask( bm );
               }
            else
               {QBitmap bm;
                bm = image.createHeuristicMask();
                p.setMask( bm );
               }
        }

    setBackgroundPixmap( p );
    m_Pixmap = p;
    if ( p.mask() ) setMask( *p.mask() );
    m_ImgHeight      = p.size().height(); // 251;
    m_ImgWidth       = p.size().width();  // 254;

    setGeometry(0,0,0,0);
    m_GrowUp = 0;
    if (mustAppear == CCoolPopup::AppearFlag) Appear();
    return p;
}

//------------------------------------------ Appear ----------------------------------------------------------------------
void CCoolPopup::Appear()
{
    m_GrowUp = m_GrowBy;   // defini le mouvement à "Ouvrir"
    show();                // montre la fenêtre
    if (m_pDisplayTimer->isActive() == FALSE) m_pDisplayTimer->start(m_Tempo, FALSE); // si le timer est arrêté alors le redemarrer
}

//------------------------------------------ Disappear ----------------------------------------------------------------------
void CCoolPopup::Disappear()
{
    m_GrowUp = -m_GrowBy;    // defini le mouvement à "Fermer"
    if (m_pDisplayTimer->isActive() == FALSE) m_pDisplayTimer->start(m_Tempo, FALSE); // si le timer est arrêté alors le redemarre
}

//------------------------------------------ timerDisplayDone ----------------------------------------------------------------------
void CCoolPopup::timerDisplayDone()
{   if (m_GrowUp == 0)
       {m_pDisplayTimer->stop();
        return;
       }
    //........... ajouter le mouvement ...................
    int localHeight = height() + m_GrowUp;

    if (localHeight >= m_ImgHeight)
       {m_GrowUp    = 0;
        localHeight = m_ImgHeight;
        emit Sign_PopupUp(this);
       }
    //......... si on atteint la limite inf .............
    else if (localHeight <= 0)
       {m_GrowUp    = 0;
        localHeight = 0;
       }

    //........... affiche fenêtre aux nouvelles dimensions .............
    if (m_Mode&WithTransLabel)
       {if (m_pC_MyLabel) m_pC_MyLabel->move( m_TextX, m_TextY);
       }
    else
       {if (m_QLabel_Txt) m_QLabel_Txt->move( m_TextX, m_TextY);
       }

    setGeometry(m_Screen.x()-m_ImgWidth-2, m_Screen.y()-localHeight, m_ImgWidth,localHeight);
    //......... si on atteint la limite sup .............
    //............ cache la fenetre si elle est complètement fermée .....
    //           (car même à 0, une ligne est toujours affichée ???)
    if (localHeight <=0)
       {emit Sign_PopupDown(this);
        hide();
       }
}

//------------------------------------------ mouseReleaseEvent ----------------------------------------------------------------------
void CCoolPopup::mouseReleaseEvent( QMouseEvent *e )
{
    if (e->button() == QMouseEvent::LeftButton) {
        emit Sign_Clicked();
        emit Sign_Clicked(this);
    }
}


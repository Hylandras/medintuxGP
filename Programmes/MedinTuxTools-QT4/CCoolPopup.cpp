 /********************************* CCoolPopup.cpp *************************
 *  #include "CCoolPopup.h"              QT4-ready                         *
 * ........................................................................*
 *   Project   :  MedinTux  (typographie made in france)                   *
 *   Copyright : (C) 2005-2006-2007-2008-2009-2010 and for the eternity    *
 *   by        :  Sevin Roland     from   MedinTux project                 *
 *   E-Mail    : roland-sevin@medintux.org                                 *
 *   Web site  : www.medintux.org                                          *
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

#include "CCoolPopup.h"
#include <Qt>
#include <QDesktopWidget>
#include <QPixmap>
#include <QImage>
#include <QBitmap>
#include <QSize>
#include <QFile>
#include <QLabel>
#include <QApplication>
#include <QPaintEvent>
#include <QFrame>
#include <QMouseEvent>

//====================================== CCoolPopup =======================================================
//------------------------------------------ CCoolPopup ----------------------------------------------------------------------
CCoolPopup::CCoolPopup  ( const QString   &pathImage       /*= ""         */ ,
                          int             mode       /*=  WithFrame */ ,
                          int             textX      /*=0           */ ,
                          int             textY      /*=0           */ ,
                          int             textW      /*=0           */ ,
                          int             textH      /*=0           */ ,
                          const QString  &text       /*=""          */ ,
                          QColor          textCol    /*=0           */ ,
                          QColor          textBack   /*=0           */ ,
                          long            tempo      /*=20          */ ,
                          int             growBy     /*=3           */ ,
                          const char     */*name       =0           */ ,
                          const QString  &styleSheet /*= ""         */ ,
                          Qt::WindowFlags            /*f         = WDestructiveClose |
                                                      WType_TopLevel    |
                                                      WStyle_Customize  |
                                                      WStyle_StaysOnTop |
                                                      WStyle_NoBorder   */
                          )
    : QWidget (QApplication::desktop()->screen(0),  Qt::CustomizeWindowHint | Qt::FramelessWindowHint |Qt::WindowStaysOnTopHint)
{   //........................Defini la taille originale de la fenetre ...........................
    setAttribute ( Qt::WA_TranslucentBackground );
    setWindowModality ( Qt::WindowModal );
    //m_Mode           = mode|WithFrame;         //
    m_Mode           = mode;         //
    m_GrowBy         = growBy;
    m_QLabel_InfosXY = 0;
    m_Text           = text;
    m_TextQColor     = textCol;
    m_TextX          = textX;
    m_TextY          = textY;
    m_TextW          = textW;
    m_TextH          = textH;
    m_QLabel_Txt     = 0;
    m_pDisplayTimer  = 0;
    //if (m_Mode&WithTransLabel) m_Mode &= m_Mode^WithTransLabel;
    //if (m_Mode&WithTransLabel) setAttribute(Qt::WA_OpaquePaintEvent, TRUE);
    //m_Mode|=WithFrame;
    // Calcule le point d'affichage du coin supérieur gauche de la fenêtre
    // en excluant la barre des taches de windows
    QDesktopWidget *pDesktop = QApplication::desktop();
    m_Screen.setX(pDesktop->availableGeometry().width()-2); // -2 pour pas coller le bord de l'écran
    m_Screen.setY(pDesktop->availableGeometry().height());
    // passe les nouveaux paramètres �  la fenête
    // le bureau devient son parent

   if (m_Mode&WithFrame)
       {//................. un label pour l'image et cadre...........................
        m_QLabel_InfosXY = new QLabel(this);
        m_QLabel_InfosXY->setFrameShape(QFrame::Box);
        m_QLabel_InfosXY->setFrameShadow(QFrame::Raised);
        m_QLabel_InfosXY->setLineWidth(2);
        m_QLabel_InfosXY->setMidLineWidth(2);
        m_QLabel_InfosXY->show();
     }
   else
     {

     }
   if (m_Text.length())
      {if ( !(m_Mode&WithTransLabel) || m_Mode&WithFrame)
         { QPalette palette;
           m_QLabel_Txt = new QLabel(this);
           //m_QLabel_Txt->setPaletteBackgroundColor (textBack );
           palette.setColor(m_QLabel_Txt->backgroundRole(), textBack);
           m_QLabel_Txt->setPalette(palette);

           //m_QLabel_Txt->setPaletteForegroundColor (textCol  );
           palette.setColor(m_QLabel_Txt->foregroundRole(), textCol);
           m_QLabel_Txt->setPalette(palette);

          m_QLabel_Txt->setText(m_Text);
          m_QLabel_Txt->move(m_TextX, m_TextY);
          m_QLabel_Txt->resize(m_TextW, m_TextH);
          m_QLabel_Txt->setAlignment ( Qt::AlignLeft );
          m_QLabel_Txt->setAlignment ( Qt::AlignTop );
          m_QLabel_Txt->show();
         }
      }
   //................. charger l'image  ..............................
   setImage(pathImage);
   //................. positionner le style   ..............................
   setStyleSheet(styleSheet);
   //................. creation du timer  ..............................
   m_Tempo         = tempo;
   m_pDisplayTimer = new QTimer(this);
   if (m_pDisplayTimer)
      {
       // connect le signal timeout avec le slot d'affichage de la fenêtre
       connect( m_pDisplayTimer, SIGNAL(timeout()), this, SLOT(timerDisplayDone()) );
       // demarre le timer
       m_pDisplayTimer->start(m_Tempo);
       // defini le mouvement �  "Aucun"
       m_GrowUp = 0;
       Appear();
      }
}

//------------------------------------------ ~CCoolPopup ----------------------------------------------------------------------
CCoolPopup::~CCoolPopup()
{ if (m_QLabel_InfosXY) delete m_QLabel_InfosXY;  // effacer car ne font pas partie de l'appli
  if (m_QLabel_Txt)     delete m_QLabel_Txt;      // effacer car ne font pas partie de l'appli
  if (m_pDisplayTimer)  delete m_pDisplayTimer;
  //qDebug("tou m'a touer");
}
//------------------------------------------ paintEvent ----------------------------------------------------------------------
void CCoolPopup::paintEvent( QPaintEvent *  /*e*/ )
{QPainter  p;
 if (p.begin(this))
    { p.drawPixmap (0, 0, m_Pixmap);
      if (m_Mode&WithTransLabel)
         {int delta = 10;
          QRect rect(delta,delta,width()-delta*2,height()-delta*2);
          p.setFont (font());
          p.setPen( m_TextQColor );
          p.drawText (rect, Qt::AlignJustify|Qt::TextWordWrap|Qt::TextExpandTabs, m_Text);
         }
    }
 p.end();
}

//------------------------------------------ setStyleSheet ----------------------------------------------------------------------
void CCoolPopup::setStyleSheet(const QString &styleSheet)
{QString styleFrame,styleLabel;
 if (styleSheet.length()== 0 ) return;
 int pos = -1;
 if ( (pos=styleSheet.indexOf('|'))!= -1)
    {styleFrame  = styleSheet.left(pos);
     styleLabel  = styleSheet.mid(pos+1);
    }
 else
    {styleFrame  = styleSheet;
    }
 if (m_QLabel_Txt     && styleLabel.length()) m_QLabel_Txt->setStyleSheet(styleLabel);
 if (m_QLabel_InfosXY && styleFrame.length()) m_QLabel_InfosXY->setStyleSheet(styleFrame);
}

//------------------------------------------ setText ----------------------------------------------------------------------
void CCoolPopup::setFont ( const QFont &ft )
{ QWidget::setFont (ft);
}
//------------------------------------------ setTextColor ----------------------------------------------------------------------
void CCoolPopup::setTextColor(const QColor &textCol)
{ m_TextQColor = textCol;
  if (m_QLabel_Txt)
     {QPalette palette;
      palette.setColor(m_QLabel_Txt->foregroundRole(), textCol);
      m_QLabel_Txt->setPalette(palette);
     }
}
//------------------------------------------ setText ----------------------------------------------------------------------
void CCoolPopup::setTextGeometry( int textX , int textY , int textW /* = -1 */ , int textH /* = -1 */ )
{m_TextX = textX;
 m_TextY = textY;
 if (textW != -1) m_TextW = textW;
 if (textH != -1) m_TextH = textH;
}

//------------------------------------------ setText ----------------------------------------------------------------------
void CCoolPopup::setText(const QString& text,
                          QColor        textCol   /*="#00F"       */ ,
                          QColor        textBack  /*=" #FFF"       */
                        )
{if (text.length()<=0)     return;
 if ( ! (m_Mode & CCoolPopup::WithTransLabel) )
    {if (m_QLabel_Txt == 0)
        { m_QLabel_Txt = new QLabel(this);

          QPalette palette;
          palette.setBrush(m_QLabel_Txt->backgroundRole(), QColor(textBack));
          m_QLabel_Txt->setPalette(palette);


          palette.setColor(m_QLabel_Txt->foregroundRole(), QColor(textCol));
          m_QLabel_Txt->setPalette(palette);

        }
     if (   m_QLabel_Txt == 0) return;
     m_QLabel_Txt->setText(text);
     m_QLabel_Txt->show();
    }
 m_Text = text;
}

//------------------------------------------ setImage ----------------------------------------------------------------------
QPixmap CCoolPopup::setImage(const QString path_image /* = "" */ , int mustAppear /*=0*/)
{if (m_Mode&WithFrame) return    setLabelImage(path_image  ,      mustAppear );
 else                  return    setBackgroundImage(path_image  , mustAppear );
}

//------------------------------------------ setLabelImage ----------------------------------------------------------------------
QPixmap CCoolPopup::setLabelImage(const QString path_image /* = "" */ , int mustAppear /*=0*/)
{
    QPixmap image(path_image);
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
    QPixmap pixmap(path_image);
    m_Pixmap = pixmap;
    //................... determiner et fabriquer le masque si necessaire ...........
    if ( pixmap.hasAlphaChannel ())
       {pixmap.setMask( QBitmap::fromImage (QImage(path_image).createAlphaMask()));
       }
    else
       {pixmap.setMask( QBitmap::fromImage (QImage(path_image).createHeuristicMask()));
       }
    if ( ! pixmap.mask().isNull() ) setMask( pixmap.mask() );

    QPalette palette;
    palette.setBrush(QPalette::Shadow, QBrush(pixmap));
    setPalette(palette);
    m_ImgHeight      = pixmap.size().height(); // 251;
    m_ImgWidth       = pixmap.size().width();  // 254;

    setGeometry(0,0,0,0);
    m_GrowUp = 0;
    if (mustAppear == CCoolPopup::AppearFlag) Appear();
    return pixmap;
}

//------------------------------------------ Appear ----------------------------------------------------------------------
void CCoolPopup::Appear()
{
    m_GrowUp = m_GrowBy;   // defini le mouvement �  "Ouvrir"
    show();                // montre la fenêtre
    if (m_pDisplayTimer->isActive() == FALSE) m_pDisplayTimer->start(m_Tempo); // si le timer est arrêté alors le redemarrer
}

//------------------------------------------ Disappear ----------------------------------------------------------------------
void CCoolPopup::Disappear()
{
    m_GrowUp = -m_GrowBy;    // defini le mouvement �  "Fermer"
    if (m_pDisplayTimer->isActive() == FALSE) m_pDisplayTimer->start(m_Tempo); // si le timer est arrêté alors le redemarre
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
    setGeometry(m_Screen.x()-m_ImgWidth-2, m_Screen.y()-localHeight, m_ImgWidth,localHeight);
    //......... si on atteint la limite sup .............
    //............ cache la fenetre si elle est complètement fermée .....
    //           (car même �  0, une ligne est toujours affichée ???)
    if (localHeight <=0)
       {emit Sign_PopupDown(this);
        hide();
       }
}

//------------------------------------------ mouseReleaseEvent ----------------------------------------------------------------------
void CCoolPopup::mouseReleaseEvent( QMouseEvent *e )
{
    if (e->button() == Qt::LeftButton) {
        emit Sign_Clicked();
        emit Sign_Clicked(this);
    }
}


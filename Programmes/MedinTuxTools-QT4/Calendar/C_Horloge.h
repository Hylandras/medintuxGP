// C++ Implementation:
/********************************* C_Horloge.h ***********************

    Project: MedinTux
    Copyright (C) 2005 by Roland Sevin  and Data Medical Design
    E-Mail: roland-sevin@medintux.org

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

#ifndef C_Horloge_H
#define C_Horloge_H

#include <qpushbutton.h>
#include <qstring.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qrect.h>
#include <qdatetime.h>
#include <q3frame.h>
#include <qpixmap.h>
//Added by qt3to4:
#include <QEvent>
#include <QPaintEvent>

class QPixmap;

//====================================== C_Horloge_Label =======================================================
class C_Horloge_Label: public QLabel
{
 Q_OBJECT
 //........................................... METHODES ...........................................................
 public:
     C_Horloge_Label      (QPixmap *        pBackPixmap,
                           QWidget*         parent           = 0,
                           const char*      name             = 0,
                           Qt::WFlags           f                = 0
                          )
           :QLabel(parent,name,f)
           {m_pBackPixmap = pBackPixmap;
           };

 protected:
     //........................................... DATA ...............................................................
     QPixmap* m_pBackPixmap;

//------------------------------------------ paintEvent ----------------------------------------------------------------------
void paintEvent( QPaintEvent *e )
{        QPainter  p(this);
         QRect ur  = e->rect();            // rectangle to update
         int   dx  =  ur.x();
         int   dy  =  ur.y();;
         int marge = 0;

         bitBlt ( this,  dx,  dy,  m_pBackPixmap, dx+x(), dy+y(), ur.width(),  ur.height());               //   Fond
         QRect rect(marge,marge,width()-marge*2,height()-marge*2);
         p.setFont (font());
         //p.drawRect (rect );
         int align(alignment()|Qt::TextWordWrap|Qt::TextExpandTabs);     // Qt::AlignJustify,  -1, 0, QTextParag ** internal = 0 Qt::AlignJustify
         QRect bRect =  p.boundingRect ( rect, align , text() );   // rectangle entourant le texte
         rect.setY (  (rect.height()-bRect.height())/2 );         // centrage vertical
         p.setPen( QColor(255,255,255) );   // texte en blanc
         p.drawText (rect, align, text()) ;
         rect.setX (  rect.x()+1 );         // decalage x texte / ombre
         rect.setY (  rect.y()+1 );         // decalage y texte / ombre
         p.setPen( foregroundColor() );     // texte de couleur normale
         p.drawText (rect, align, text());
}

};

//===============================================================  C_HWdg =========================================================================
/*! \class C_HWdg
 *  \brief Classe des boutons s'affichant dans la pendule
*/
class  C_HM_Wdg : public QWidget
{
  Q_OBJECT

public:
  C_HM_Wdg(const QString &text,  const QString &pixmapPath, QPixmap *pBackPixmap,  QWidget* parent = 0, const char* name = 0 )
         : QWidget ( parent, name )
        {m_text        = text;
         m_Pixmap      = QPixmap(pixmapPath);
         m_pBackPixmap = pBackPixmap;
        }
  ~C_HM_Wdg(){};
  /*$PUBLIC_FUNCTIONS$*/
QString  m_text;
QPixmap  m_Pixmap;
QPixmap* m_pBackPixmap;


//void bitBlt ( QPaintDevice * dst, int dx, int dy, const QPaintDevice * src, int sx, int sy, int sw, int sh, Qt::RasterOp rop, bool ignoreMask )
//----------------------------------------------- eventFilter ---------------------------------------------------
void paintEvent ( QPaintEvent *e )
{       QPainter paint( this );
        QRect ur   = e->rect();            // rectangle to update
        int dx     =  ur.x();
        int dy     =  ur.y();
        int deltaX = (width()   - m_Pixmap.width())/2;
        int deltay = (height()  - m_Pixmap.height())/2;
        bitBlt ( this,  dx,         dy,         m_pBackPixmap, dx+x(), dy+y(), ur.width(),  ur.height()); //   Fond
        bitBlt ( this,  dx+deltaX,  dy+deltay, &m_Pixmap,      dx,     dy,     ur.width(),  ur.height()); //   image
}

public slots:
  /*$PUBLIC_SLOTS$*/

//----------------------------------------------- eventFilter ---------------------------------------------------
bool event( QEvent *e )
   {if (e->type ()==QEvent::Enter) mouseEnter ();  //Enter = 10, Leave = 11
    if (e->type ()==QEvent::Leave) mouseLeave ();  //Enter = 10, Leave = 11
    if (e->type ()==QEvent::MouseButtonPress)   emit Sign_HeureClicked(m_text);;  //Enter = 10, Leave = 11
    if (e->type ()==QEvent::MouseButtonRelease) emit Sign_HeureClicked(m_text);;  //Enter = 10, Leave = 11
    return QWidget::event(e);
   }


protected slots:
//----------------------------------------------- mouseEnter ---------------------------------------------------
/*! \brief recupere l'evenement lorsque la souris rentre dans la rubrique
 */
void mouseEnter ()
{emit Sign_MouseEnter(m_text);
}

//----------------------------------------------- mouseLeave ---------------------------------------------------
/*! \brief recupere l'evenement lorsque la souris sort de la rubrique
 */
void mouseLeave ()
{emit Sign_MouseLeave(m_text);
}

signals:
   void  Sign_HeureClicked( QString );
   void  Sign_MouseEnter(QString);
   void  Sign_MouseLeave(QString);
};

//====================================== C_Horloge_Label =======================================================
class C_Horloge: public Q3Frame
{
 Q_OBJECT
 //........................................... METHODES ...........................................................
 public:
      C_Horloge            (const QString   &iconesPath,
                            QWidget*         parent           = 0,
                            const char*      name             = 0,
                            Qt::WFlags           f                = 0,
                            int              w_widget         = 0,
                            int              h_widget         = 0
                           );
     ~C_Horloge();
     void   setCentralRefLabel(QWidget *pQLabel);
     QTime  getTime(){return m_QTime;};
     //........................................... DATA ...............................................................
     C_Horloge_Label *m_pC_Horloge_Label;         /*!< label d'affichage de l'heure du centre de l'horloge  */
     QPixmap          m_BackPixmap;               /*!< Pixmap de fond de l'horloge  */
     QWidget*         m_pRefLabel;                /*!< QLabel de réference pour affichage central (facultatif)  */
     QString          m_iconesPath;               /*!< chemin des icones de l'horloge */
     QTime            m_QTime;                    /*!< heure de l'horloge */
public slots:
     void setTime(const QTime &t );
protected slots:
     void On_HeuresClicked(QString hr);
     void On_MinutesClicked(QString mn);
     void On_MouseMinuteLeave(QString );
     void On_MouseMinuteEnter(QString mn);
     void On_MouseHeureLeave(QString );
     void On_MouseHeureEnter(QString hr);
signals:
     void Sign_HorlogeTimeChanged(const QTime &);
};
#endif

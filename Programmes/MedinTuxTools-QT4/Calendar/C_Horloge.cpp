// C++ Implementation:
/********************************* C_Horloge.cpp ***********************

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

#include <qlayout.h>
#include <math.h>
#include <q3frame.h>
#include <qdatetime.h>
#include <qpixmap.h>
#include <qlineedit.h>
#include <qfileinfo.h>

#include "C_Horloge.h"



//----------------------------------------------- C_Horloge ------------------------------------------------------------------------
C_Horloge::C_Horloge(const QString &iconesPath, QWidget* parent, const char* name, Qt::WFlags fl , int w_widget, int h_widget)
:Q3Frame(parent,name,fl)
{   m_iconesPath     = iconesPath;
    if (QFileInfo ( iconesPath ).isFile())
       { m_BackPixmap     = QPixmap(m_iconesPath);
         m_iconesPath     = QFileInfo ( iconesPath ).dirPath()+"/";
       }
    else m_BackPixmap     = QPixmap(m_iconesPath+"Horloge.png");
    m_pRefLabel      = 0;
    m_QTime          = QTime::currentTime();
    if (w_widget==0) w_widget = m_BackPixmap.width();
    if (h_widget==0) h_widget = m_BackPixmap.height();
    setPaletteBackgroundPixmap( m_BackPixmap );
    //...................... affichage central de l'heure .....................................
    m_pC_Horloge_Label = new C_Horloge_Label( &m_BackPixmap, this, "m_pC_Horloge_Label" );
    int lx = (w_widget - 70)/2;
    int ly = (h_widget - 21)/2;
    m_pC_Horloge_Label->setGeometry( QRect( lx, ly, 70, 21 ) );
    QFont m_pC_Horloge_Label_font(  m_pC_Horloge_Label->font() );
    m_pC_Horloge_Label_font.setBold( TRUE );
    m_pC_Horloge_Label_font.setPointSize( 12 );
    m_pC_Horloge_Label->setFont( m_pC_Horloge_Label_font );
    m_pC_Horloge_Label->setAlignment( int( Qt::AlignHCenter ) );
    m_pC_Horloge_Label->setPaletteForegroundColor ( QColor( 37, 125, 241 ) );

    C_HM_Wdg *pC_HWdg_AM;
    C_HM_Wdg *pC_HWdg_PM;
    float cosu , sinu , ang;
    int a = 0;
    int w = w_widget;
    int h = h_widget;
    int n = 0;
    int   xt  = w/2 - 8;
    int   yt  = h/2 - 8;
    //int   xop = w/2 - 30;
    int   yop = h/2 - 30;
    //int   xoa = w/2 - 55;
    int   yoa = h/2 - 55;
    float x   = 0;
    float y   = 0;
    for (a=3600+1800; a>1800; a -= 300)  // a<3600;
        {ang        = a*3.1416/1800;
         cosu       = cos(ang);
         sinu       = sin(ang);
         QString fn = QString::number(n);       if (n<10) {fn = fn.prepend("0");} fn = fn.prepend("h-")+".png";
         pC_HWdg_AM = new C_HM_Wdg(QString::number(n),    m_iconesPath + fn, &m_BackPixmap,         this, "pushButtonDateDeb" );
                 fn = QString::number(n+12);                                      fn = fn.prepend("p-")+".png";
         pC_HWdg_PM = new C_HM_Wdg(QString::number(n+12), m_iconesPath + fn, &m_BackPixmap,         this, "pushButtonDateDeb" );
         connect( pC_HWdg_AM ,     SIGNAL( Sign_HeureClicked( QString ) ),     this, SLOT( On_HeuresClicked(QString )  ) );
         connect( pC_HWdg_PM ,     SIGNAL( Sign_HeureClicked( QString ) ),     this, SLOT( On_HeuresClicked(QString )  ) );
         connect( pC_HWdg_PM ,     SIGNAL( Sign_MouseEnter(QString)),          this, SLOT( On_MouseHeureEnter(QString) ) );
         connect( pC_HWdg_PM ,     SIGNAL( Sign_MouseLeave(QString)),          this, SLOT( On_MouseHeureLeave(QString) ) );
         connect( pC_HWdg_AM ,     SIGNAL( Sign_MouseEnter(QString)),          this, SLOT( On_MouseHeureEnter(QString) ) );
         connect( pC_HWdg_AM ,     SIGNAL( Sign_MouseLeave(QString)),          this, SLOT( On_MouseHeureLeave(QString) ) );
         ++n;
         if (pC_HWdg_AM && pC_HWdg_PM)
            {pC_HWdg_AM->setMaximumSize( QSize( 16, 16 ) );
             pC_HWdg_PM->setMaximumSize( QSize( 16, 16 ) );
             x =  (int)((float)(cosu)+(float)(sinu*yop));
             y =  (int)((float)(cosu*yop)-(float)(sinu));
             x = x+xt;
             y = y+yt;
             pC_HWdg_AM->move ( x, y );
             x =  (int)((float)(cosu)+(float)(sinu*yoa));
             y =  (int)((float)(cosu*yoa)-(float)(sinu));
             x = x+xt;
             y = y+yt;
             pC_HWdg_PM->move ( x, y );
            }
        }
    yoa = h/2 - 14;
    n   = 0;
    xt  = w/2 - 5;
    yt  = h/2 - 5;
    for (a=3600+1800; a>1800; a -= 60)  // a<3600;
        {ang        = a*3.1416/1800;
         cosu       = cos(ang);
         sinu       = sin(ang);
         QString nb = QString::number(n);
         if (nb.endsWith("0") || nb.endsWith("5"))   pC_HWdg_PM = new C_HM_Wdg(nb, m_iconesPath + "CinqMinutes.png", &m_BackPixmap, this, "pushButtonDateDeb" );
         else                                        pC_HWdg_PM = new C_HM_Wdg(nb, m_iconesPath + "Minutes.png",     &m_BackPixmap, this, "pushButtonDateDeb" );
         connect( pC_HWdg_PM ,     SIGNAL( Sign_HeureClicked( QString)),  this, SLOT( On_MinutesClicked(QString ) ) );
         connect( pC_HWdg_PM ,     SIGNAL( Sign_MouseEnter(QString)),     this, SLOT( On_MouseMinuteEnter(QString) ) );
         connect( pC_HWdg_PM ,     SIGNAL( Sign_MouseLeave(QString)),     this, SLOT( On_MouseMinuteLeave(QString) ) );
         ++n;
         if (pC_HWdg_AM && pC_HWdg_PM)
            {pC_HWdg_PM->setMaximumSize( QSize( 10, 10 ) );
             x =  (int)((float)(cosu)+(float)(sinu*yoa));
             y =  (int)((float)(cosu*yoa)-(float)(sinu));
             x = x+xt;
             y = y+yt;
             pC_HWdg_PM->move ( x, y );
            }
        }
  setTime( QTime::currentTime() );
  setMaximumSize(QSize(w_widget, h_widget));
  setMinimumSize(QSize(w_widget, h_widget));
  resize( QSize(w_widget, h_widget).expandedTo(minimumSizeHint()) );
}
//----------------------------------------------- ~C_Horloge ------------------------------------------------------------------------
C_Horloge::~C_Horloge()
{
}
//----------------------------------------------- setCentralRefLabel ------------------------------------------------------------------------
void C_Horloge::setCentralRefLabel(QWidget *pQLabel)
{   m_pRefLabel = pQLabel;
    int lx = (width()  - m_pRefLabel->width())/2;
    int ly = (height() - m_pRefLabel->height())/2;

    m_pC_Horloge_Label->setGeometry( QRect( lx, ly, m_pRefLabel->width(), m_pRefLabel->height() ) );
    m_pC_Horloge_Label->setFont(  m_pRefLabel->font()  );
    m_pC_Horloge_Label->setAlignment( int( Qt::AlignHCenter ) );
    m_pC_Horloge_Label->setPaletteForegroundColor ( m_pRefLabel->foregroundColor () );
}

/*$SPECIALIZATION$*/

//----------------------------------------------- On_timeEdit_TempsValueChanged ------------------------------------------------------------------------
void C_Horloge::setTime(const QTime &t )
{m_QTime = t;
 QString hr = QString::number(t.hour());
 QString mn = QString::number(t.minute());
 if (hr.length()<=1) hr.prepend("0");
 if (mn.length()<=1) mn.prepend("0");
  m_pC_Horloge_Label->setText(hr+":"+mn);
}

//----------------------------------------------- On_MouseHeureEnter ------------------------------------------------------------------------
void C_Horloge::On_MouseHeureEnter(QString hr)
{QString mn = QString::number(m_QTime.minute());
 if (hr.length()<=1) hr.prepend("0");
 if (mn.length()<=1) mn.prepend("0");
 m_pC_Horloge_Label->setText(hr+":"+mn);
}
//----------------------------------------------- On_MouseHeureLeave ------------------------------------------------------------------------
void C_Horloge::On_MouseHeureLeave(QString )
{QString hr = QString::number(m_QTime.hour());
 QString mn = QString::number(m_QTime.minute());
 if (hr.length()<=1) hr.prepend("0");
 if (mn.length()<=1) mn.prepend("0");
 m_pC_Horloge_Label->setText(hr+":"+mn);
}

//----------------------------------------------- On_MouseMinuteEnter ------------------------------------------------------------------------
void C_Horloge::On_MouseMinuteEnter(QString mn)
{QString hr = QString::number(m_QTime.hour());
 if (hr.length()<=1) hr.prepend("0");
 if (mn.length()<=1) mn.prepend("0");
 m_pC_Horloge_Label->setText(hr+":"+mn);
}
//----------------------------------------------- On_MouseMinuteLeave ------------------------------------------------------------------------
void C_Horloge::On_MouseMinuteLeave(QString )
{QString hr = QString::number(m_QTime.hour());
 QString mn = QString::number(m_QTime.minute());
 if (hr.length()<=1) hr.prepend("0");
 if (mn.length()<=1) mn.prepend("0");
 m_pC_Horloge_Label->setText(hr+":"+mn);
}

//----------------------------------------------- On_MinutesClicked ------------------------------------------------------------------------
void C_Horloge::On_MinutesClicked(QString mn)
{int   h = m_QTime.hour();
 int   s = m_QTime.second();
 m_QTime.setHMS (h, mn.toInt(),s);
 QString hr = QString::number(h);
 if (hr.length()<=1) hr.prepend("0");
 if (mn.length()<=1) mn.prepend("0");
 m_pC_Horloge_Label->setText(hr+":"+mn);
 emit Sign_HorlogeTimeChanged(m_QTime);
}

//----------------------------------------------- On_HeuresClicked ------------------------------------------------------------------------
void C_Horloge::On_HeuresClicked(QString hr)
{int   m = m_QTime.minute();
 int   s = m_QTime.second();
 m_QTime.setHMS (hr.toInt(),m,s );
 QString mn = QString::number(m);
 if (hr.length()<=1) hr.prepend("0");
 if (mn.length()<=1) mn.prepend("0");
 m_pC_Horloge_Label->setText(hr+":"+mn);
 emit Sign_HorlogeTimeChanged(m_QTime);
}




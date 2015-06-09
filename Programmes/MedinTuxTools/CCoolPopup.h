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

#ifndef CCOOLPOPUP_H
#define CCOOLPOPUP_H

#include <qdialog.h>
#include <qtimer.h>
#include <qlabel.h>


#include <qwidget.h>

#include <qpainter.h>
#include <qpixmap.h>
#include <qfont.h>
#include <qlabel.h>
#include <qsimplerichtext.h>
#include <qpaintdevicemetrics.h>
#include <qprinter.h>
#include <qpalette.h>

//====================================== CViewPageWidget =======================================================
class C_MyLabel: public QWidget
{
 Q_OBJECT
 //........................................... METHODES ...........................................................
 public:
     C_MyLabel      (QWidget*         parent           = 0,
                     const char*      name             = 0,
                     WFlags           f                = 0
                    );
    //~CViewPageWidget();
     void setText(const QString &text);
     void setPixmap(const QPixmap &pixmap, int x=0, int y=0, int w=-1, int h=-1);
     void setTextColor(const QColor &color);
 protected:
   void             paintEvent( QPaintEvent *e );
 //........................................... DATA ...............................................................
 QString m_Text;
 QColor  m_ForeTextColor;
 QPixmap m_Pixmap;
};

//================================================================ PopupWindow =====================================================
class CCoolPopup : public QWidget
{
    Q_OBJECT

public:

    enum
    {   NoAppearFlag         = 0x0000,
        AppearFlag           = 0x0001
    };
    enum
    {   WithFrame            = 0x0001,
        WithTransLabel       = 0x0002
    };

                  CCoolPopup( const QString pathImage = ""             ,
			      int           mode      = WithFrame      ,
			      int           textX     = 0              ,
			      int           textY     = 0              ,
			      int           textW     = 0              ,
			      int           textH     = 0              ,
			      const char*   text      = ""             ,
			      const QColor  textCol   = "#00F"    ,
			      const QColor  textBack  = "#FFF"    ,
			      long          tempo     = 20             ,
			      int           growBy    = 3              ,
			      const char   *name      = 0              ,
			      WFlags        f         = WDestructiveClose |
                                                        WType_TopLevel    |
                                                        WStyle_Customize  |
                                                        WStyle_StaysOnTop |
                                                        WStyle_NoBorder
			  );
    ~CCoolPopup();

    QPixmap setImage(const QString path_image="", int mustAppear =0);
    void    setText(const QString& text, const QColor  textCol   = "#00F" , const QColor  textBack  = "#FFF"  );
    void    setTextGeometry(int textX , int textY , int textW = -1 , int textH = -1 );

public slots:
    void Appear();
    void Disappear();
    virtual void setFont ( const QFont &ft );
    virtual void setTextColor(const QColor &textCol);
signals:
    void Sign_Clicked();
    void Sign_Clicked(CCoolPopup*);
    void Sign_PopupDown(CCoolPopup* );
    void Sign_PopupUp(CCoolPopup* );

protected:
    void     mouseReleaseEvent( QMouseEvent *e );

    QTimer     *m_pDisplayTimer;
    QLabel     *m_QLabel_InfosXY;
    C_MyLabel  *m_pC_MyLabel;
    QLabel     *m_QLabel_Txt;
    QString  m_Text;
    int      m_Mode;
    int      m_GrowUp;
    int      m_ImgHeight;
    int      m_ImgWidth;
    long     m_Tempo;
    int      m_GrowBy;
    int      m_TextX;
    int      m_TextY;
    int      m_TextW;
    int      m_TextH;

private:
    QPixmap setLabelImage(const QString path_image="", int mustAppear =0);
    QPixmap setBackgroundImage(const QString path_image  = ""  , int mustAppear =0);
    QPoint  m_Screen;
    QPixmap m_Pixmap;
private slots:
    void timerDisplayDone();

};

#endif //CCOOLPOPUP_H

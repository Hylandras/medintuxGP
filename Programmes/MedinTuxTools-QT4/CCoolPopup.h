 /********************************* CCoolPopup.h ***************************
 *  #include "CCoolPopup.h"           QT4-ready                            *
 * ........................................................................*
 *   Project   :  MedinTux  (typographie made in france)                   *
 *   Copyright :  (C) 2005-2006-2007-2008-2009-2010 and for the eternity   *
 *   by        :  Sevin Roland     from   MedinTux project                 *
 *   E-Mail    :  roland-sevin@medintux.org                                *
 *   Web site  :  www.medintux.org                                         *
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

#ifndef CCOOLPOPUP_H
#define CCOOLPOPUP_H

#include <QTimer>
#include <QLabel>
#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QFont>
#include <QMouseEvent>
#include <QPaintEvent>

//================================================================ PopupWindow =====================================================
class CCoolPopup : public QWidget
{
    Q_OBJECT

public:

    enum mode
    {   NoAppearFlag         = 0x0000,
        AppearFlag           = 0x0001
    };
    enum contour
    {   WithFrame            = 0x0001,
        WithTransLabel       = 0x0002
    };

                  CCoolPopup( const QString  &pathImage  = ""             ,
                              int             mode       = CCoolPopup::WithFrame      ,
                              int             textX      = 0              ,
                              int             textY      = 0              ,
                              int             textW      = 0              ,
                              int             textH      = 0              ,
                              const QString  &text       = ""             ,
                              const QColor    textCol    = "#00F"         ,
                              const QColor    textBack   = "#FFF"         ,
                              long            tempo      = 20             ,
                              int             growBy     = 3              ,
                              const char     *name       = 0              ,
                              const QString  &styleSheet = ""             ,
                              Qt::WindowFlags      f     =  //Qt::WA_DeleteOnClose
                                                            Qt::Window|Qt::SplashScreen
                                                            //| Qt::WindowStaysOnTopHint
                                                            //| Qt::FramelessWindowHint
			  );
                  /*
                  Qt::Widget	0x00000000	This is the default type for QWidget. Widgets of this type are child widgets if they have a parent, and independent windows if they have no parent. See also Qt::Window and Qt::SubWindow.
                  Qt::Window	0x00000001	Indicates that the widget is a window, usually with a window system frame and a title bar, irrespective of whether the widget has a parent or not. Note that it is not possible to unset this flag if the widget does not have a parent.
                  Qt::Dialog	0x00000002 | Window	Indicates that the widget is a window that should be decorated as a dialog (i.e., typically no maximize or minimize buttons in the title bar). This is the default type for QDialog. If you want to use it as a modal dialog, it should be launched from another window, or have a parent and used with the QWidget::windowModality property. If you make it modal, the dialog will prevent other top-level windows in the application from getting any input. We refer to a top-level window that has a parent as a secondary window.
                  Qt::Sheet	0x00000004 | Window	Indicates that the window is a Macintosh sheet. Since using a sheet implies window modality, the recommended way is to use QWidget::setWindowModality(), or QDialog::open(), instead.
                  Qt::Drawer	0x00000006 | Window	Indicates that the widget is a Macintosh drawer.
                  Qt::Popup	0x00000008 | Window	Indicates that the widget is a pop-up top-level window, i.e. that it is modal, but has a window system frame appropriate for pop-up menus.
                  Qt::Tool	0x0000000a | Window	Indicates that the widget is a tool window. A tool window is often a small window with a smaller than usual title bar and decoration, typically used for collections of tool buttons. If there is a parent, the tool window will always be kept on top of it. If there isn't a parent, you may consider using Qt::WindowStaysOnTopHint as well. If the window system supports it, a tool window can be decorated with a somewhat lighter frame. It can also be combined with Qt::FramelessWindowHint.

                  On Mac OS X, tool windows correspond to the Floating class of windows. This means that the window lives on a level above normal windows; it impossible to put a normal window on top of it. By default, tool windows will disappear when the application is inactive. This can be controlled by the Qt::WA_MacAlwaysShowToolWindow attribute.
                  Qt::ToolTip	0x0000000c | Window	Indicates that the widget is a tooltip. This is used internally to implement tooltips.
                  Qt::SplashScreen	0x0000000e | Window	Indicates that the window is a splash screen. This is the default type for QSplashScreen.
                  Qt::Desktop	0x00000010 | Window	Indicates that this widget is the desktop. This is the type for QDesktopWidget.
                  Qt::SubWindow	0x00000012	Indicates that this widget is a sub-window, such as a QMdiSubWindow widget.
                  */
    ~CCoolPopup();
    void    setStyleSheet(const QString &styleSheet);
    QPixmap setImage(const QString path_image="", int mustAppear =0);
    void    setText(const QString& text, const QColor  textCol   = "#00F" , const QColor  textBack  = "#FFF"  );
    void    setTextGeometry(int textX , int textY , int textW = -1 , int textH = -1 );
    void    paintEvent( QPaintEvent *e );
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
    QColor   m_TextQColor;
private:
    QPixmap setLabelImage(const QString path_image="", int mustAppear =0);
    QPixmap setBackgroundImage(const QString path_image  = ""  , int mustAppear =0);
    QPoint  m_Screen;
    QPixmap m_Pixmap;
private slots:
    void timerDisplayDone();

};

#endif //CCOOLPOPUP_H


/************************* CMaskedLineEdit.h ********************************
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

#ifndef CMASKEDLINEEDIT_H
#define CMASKEDLINEEDIT_H

#include <qwidget.h>
#include <qlineedit.h>
//#include <qwidgetplugin.h> n'existe plus
#include <q3popupmenu.h>
//Added by qt3to4:
#include <QEvent>

#define Min(a, b)             ((a) < (b) ? (a) : (b))
#define Max(a, b)             ((a) > (b) ? (a) : (b))



class  CMaskedLineEdit : public QLineEdit    //QT_WIDGET_PLUGIN_EXPORT devant  CMaskedLineEdit
{
    Q_OBJECT
    Q_PROPERTY( int        mixMode          READ getMixMode        WRITE setMixMode )
    Q_PROPERTY( uint       carMask          READ getCarMask        WRITE setCarMask )
    Q_PROPERTY( QString    control          READ getValid          WRITE setValid )
    Q_PROPERTY( QString    mask             READ getTemplate       WRITE setTemplate )
    Q_PROPERTY( QString    numKeyboardMap   READ getNumKeyboardMap WRITE setNumKeyboardMap )
    Q_PROPERTY( QString    text             READ text              WRITE setText )

public:
    CMaskedLineEdit( QWidget *parent = 0, const char *name = 0);
   ~CMaskedLineEdit();

    enum CtrlState
    {   NoCtrlKey        = 0x0000,
        Ctrl_Key         = 0x0001,
        Shift_Key        = 0x0002,
        Alt_Key          = 0x0004
    };

    //................... Property Methodes....................................
    uint               getCarMask ( )         const;
    QString            getTemplate ()         const;
    QString            getValid ()            const;
    int                getMixMode()           const;
    QString            getNumKeyboardMap ()   const;
    QString            text ()                const;
    QString            getMixText ()          const;
    //................... common Methodes....................................
protected:
    bool          event ( QEvent * ev );
    QChar         IsCarValid( const QChar &valid, QChar c);
    int           OnControlKey(int controlState, int key);
    int           OnChar(QEvent * ev);
    int           CursPosInText_To_PosInMixedString(int curspos);
    int           CursPosInMixedString_To_PosInText(int &cursposMixed);
    int           DoMixedString(QString& result);
    Q3PopupMenu*   createPopupMenu ( const QPoint & pos );
    bool          GetSelection ( int *debMix, int *endMix  );
    void          ClearSelection ();
    void          setMixText(const QString& result);
    int           getEndMixText();
    int           GetLastNotMaskCarFromEnd(const QString &txt);
public slots:
    //................... Property SLOTS....................................
    void               setTemplate       ( const QString &templ );
    void               setValid          ( const QString &Valid );
    void               setText           ( const QString &text );
    void               setCarMask        ( const uint   &carMask );
    void               setNumKeyboardMap ( const QString &numKeyboardMap );
    void               setMixMode(const int &mixmod);
    void               setCursorPosition (int cursPos);
    //................... common  SLOTS....................................
    void          paste();
    void          copy();
    void          extendedCopy();
    void          cut();
    void          cutAll();
    void          clear ();

private:
    int                 m_ControlIsDown;
    QString             m_Valid;
    QString             m_Template;
    QString             m_Text;
    int                 m_LenMax;
    int                 m_CursPos;
    uchar               m_CarMask;
    QChar               m_LastCarToInsert;

    QString             m_NumKeyboardMap;
    int                 m_debMix;
    int                 m_endMix;
    bool                m_IsSel;
    int                 m_MixMode;   // 0=Normal   1 le texte est coupé au premier caractère non rempli
signals:
    void Sig_CarEvent(QEvent * ev);
    void Sig_KeyDown();
    void Sig_KeyUp();
    void Sig_FocusOut();
    void Sig_FocusIn();
};

#endif


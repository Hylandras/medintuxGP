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

#ifndef CCLICKABLELED_H
#define CCLICKABLELED_H

#include <QPixmap>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
//================================================================ C_ClickableLed =====================================================
class C_ClickableLed : public QWidget /* QHBoxLayout*/
{
    Q_OBJECT

public:

    enum ledColor
    {   Red       = 0x0000,
        Gray      = 0x0001,
        Green     = 0x0002
    };

    C_ClickableLed( QWidget * parent = 0,
                   ledColor color = Gray);
    ~C_ClickableLed();
    void    setLedPixmap(const QPixmap &pixmap);
    void    setLedPixmap(const QString &pixmapPath);
    void    setText(const QString &text);
    QString text();
    void    setLedColor(int color);
    //ledColor asLedColor();
private:
    QHBoxLayout *m_HBoxLayout;
    QLabel      *m_PixMapLabel;
    QLabel      *m_TextLabel;
signals:

    void clicked();

protected:

    void mouseReleaseEvent ( QMouseEvent * e );

};

#endif //CCLICKABLELED_H


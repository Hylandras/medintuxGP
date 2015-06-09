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

#ifndef FORMONGLETRUBRIQUE_H
#define FORMONGLETRUBRIQUE_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QTabWidget;

class FormOngletRubrique : public QWidget
{
    Q_OBJECT

public:
     FormOngletRubrique( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~FormOngletRubrique();

    QTabWidget* tabWidget;

    virtual void addPanel( const char * name );
    virtual void removePage( int i );
    
public slots:
    virtual void OnRubriqueActived( QWidget * pWdgRub );
    virtual void removePage( const char* name );
    
signals:
    void message(const QString&, int );

protected:
    
    int  m_OngletToSet;
    int  m_OngletToRemove;
protected slots:
    virtual void languageChange();
    void TimedSetOnglet();
    void TimedRemoveOnglet();
private:
    void init();

};

#endif // FORMONGLETRUBRIQUE_H

/***************************************************************************
 *   Copyright (C) 2009 by Docteur Pierre-Marie Desombre.   *
 *   pm.desombre@medsyn.fr  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QWidget>
#include "ui_configdialog.h"

class config:public QWidget,public Ui::ConfigDialog
{
    Q_OBJECT
    public:
        config(QWidget * parent = 0);
        ~config();
        QObject * m_widgetparent;
    signals:
        void resetUserCombo();
    protected slots:
        void enregistreconfig();
        void enregistrerutilisateurpref();
        void purgeSauvegardeDossiers();
        void setTitlesCombo();
        void feelEdits(const QString & title);
        void deleteTitle();
        void addTitle();
    private :
        QString m_resources;
};


#endif



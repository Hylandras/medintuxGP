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
#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H
#include <QtGui>
#include <QtCore>
#include <QtSql>
#include "ui_textdialog.h"

class textdialog:public QWidget,public Ui::TextDialog
{
    Q_OBJECT
    public:
        textdialog(QString &/*name*/,QString &/*surname*/,QString &/*user*/,QStringList &/*texte*/,QString &/*resutext*/);
        ~textdialog();
        QString m_nompatient;
        QString m_prenompatient;
        QString m_resutext;
        QString m_userpref;
        QString m_texte;
        QStringList m_textelist;
        QStringList m_titres2;
        QString fonctiontitre(QStringList&);
        QTextDocument *m_document;
    protected slots:
        void enregistretexte();
        void ouvrirmanager();
        void modifiertexte();
        void effacerFichier();
    private:
        QString m_resources ;
};

#endif


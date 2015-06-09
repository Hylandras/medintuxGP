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
#include <QtGui>
#include <QtCore>
#include <QApplication>
#include <QtDebug>
#include "CApp.h"
#include "apidialog.h"
#include "textdialog.h"
#include "connexion.h"



 //-------------------------------------------------- main ------------------------------------
int main(int argc,char* argv[]){
    CApp app(NAME_APPLI,argc,argv);

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);
    QTranslator myappTranslator;
    myappTranslator.load(QString("apitux_")+ QLocale::system().name(),CApp::pCApp()->pathAppli()+"Ressources/lang");
    app.installTranslator(&myappTranslator);

    QFile f(CApp::pCApp()->pathAppli()+"errorLog.txt");
    f.remove();
    CApp::pCApp()->addLibraryPath(CApp::pCApp()->pathAppli());
     /*QTranslator translator;
     translator.load("apitux_en.qm");
     app.installTranslator(&translator);*/
    qDebug() << "avant create";
    QStringList library ;
    QString strLib;
    QString str;
    library = CApp::pCApp()->libraryPaths();
    foreach(str,library){
            strLib += str+"\n";
           }
    qDebug() << "libraries = "+strLib;
    if( CApp::pCApp()->createConnection()==false){
        QMessageBox msgdb;
        msgdb.setWindowTitle(QObject::trUtf8("Erreur ouverture de base"));
        msgdb.setText(QObject::trUtf8("Impossible de se connecter Ã  la base "));
        msgdb.setIcon(QMessageBox::Critical);
        msgdb.exec();
        if(QMessageBox::Ok){
	        return false;
        }
    }
    qDebug() << "main";
    apidialog *wdg = new apidialog(0);
    //wdg.show();
    return CApp::pCApp()->exec();
}

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
#include "postview.h"
#include "CApp.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

PostView::PostView(QWidget * parent,QString lastid,QString user)
{
    setupUi(this);
    m_lastid = lastid;
    m_user = user;
    if (!searchLastResults())
    {
          QMessageBox::warning(0,trUtf8("Erreur"),trUtf8("Impossible de retrouver toutes les données."),QMessageBox::Ok);
        }
    displayResults();
    connect(quitButton,SIGNAL(pressed()),this,SLOT(close()));
}

PostView::~PostView(){}

bool PostView::searchLastResults()
{
    enum HeadsEnum {BLOBHEADPRIMKEY=0,TITLE};
    enum BlobsEnum {BLOBPRIMKEY=0,IDDOS};
    QSqlQuery query(CApp::pCApp()->database());
    QString req = QString("select %1,%2 from %3 ")
    .arg("RbDate_RefBlobs_PrimKey","RbDate_NomDate","RubriquesHead");
    req += QString("where %1 > '%2' ").arg("RbDate_PrimKey",m_lastid);
    req += QString("and %1 = '%2' ").arg("RbDate_TypeRub","20080000");
    req += QString("and %1 = '%2'").arg("RbDate_CreateUser",m_user);
    qDebug() << __FILE__ << QString::number(__LINE__) << " requete =" << req ;
    if (!query.exec(req))
    {
          qWarning() << __FILE__ << QString::number(__LINE__) << query.lastError().text() ;
          return false;          
        }
    while (query.next())
    {
        QString blobprimkey = query.value(BLOBHEADPRIMKEY).toString();
        QString title = query.value(TITLE).toString();
        m_hashOfHeads.insert(blobprimkey,title);
        }
    QStringList blobprimkeylist;
    blobprimkeylist = m_hashOfHeads.keys();
    foreach(QString primkey,blobprimkeylist){
        QSqlQuery queryblobs(CApp::pCApp()->database());
        QString reqblob = QString("select %1,%2 from %3 ").arg("RbDate_DataRub","RbDate_IDDos","RubriquesBlobs");
        reqblob += QString("where %1 = '%2'").arg("RbDate_PrimKey",primkey);
        if (!queryblobs.exec(reqblob))
        {
              qWarning() << __FILE__ << QString::number(__LINE__) << queryblobs.lastError().text() ;
              return false;              
            }
        while (queryblobs.next())
        {
            QString blob = queryblobs.value(BLOBPRIMKEY).toString();
            QString iddos = queryblobs.value(IDDOS).toString();
            m_hashprimkeyblob.insert(primkey,blob);
            m_hashprimkeyiddossier.insert(primkey,iddos);
            }
        }//foreach
    return true;
}

void PostView::displayResults()
{
    QString docString;
    docString = "<html><body><font size = 3 color = blue>";
    qDebug() << __FILE__ << QString::number(__LINE__) << "m_hashOfHeads.count()  =" << QString::number(m_hashOfHeads.count()) ;
    for (int i = 0; i < m_hashOfHeads.count(); ++i)
    {
          QString primkey = m_hashOfHeads.keys()[i];
          QString dossier = m_hashprimkeyiddossier.value(primkey);
          QString namefirstname = getNameFromId(dossier);
          qDebug() << __FILE__ << QString::number(__LINE__) << " namefirstname =" << namefirstname ;
          QString head = m_hashOfHeads.value(primkey);
          QString blob = m_hashprimkeyblob.value(primkey);
          docString += "<font color = red>"+namefirstname+" : "+dossier+"</font><br/>";
          docString += QString("par l'utilisateur : ");
          docString += "<font color = green>"+m_user+"</font><br/>";
          docString += head+"<br/><br/>";
          docString += blob+"<br/>";
          docString += "<br/><br/><br/>";          
          //qDebug() << __FILE__ << QString::number(__LINE__) << " docString =" << docString ;
        }
    docString += "</font></body></html>";
    textEdit->setHtml(docString);
}

QString PostView::getNameFromId(const QString iddossier)
{
    enum NameEnum {NAME=0,FIRSTNAME};
    QString name;
    QSqlQuery query(CApp::pCApp()->database());
    QString req = QString("select %1,%2 from %3 ").arg("FchGnrl_NomDos","FchGnrl_Prenom","IndexNomPrenom");
    req += QString("where %1 = '%2'").arg("FchGnrl_IDDos",iddossier);
    if (!query.exec(req))
    {
          qWarning() << __FILE__ << QString::number(__LINE__) << query.lastError().text() ;
        }
    while (query.next())
    {
        QString n = query.value(NAME).toString();
        QString f = query.value(FIRSTNAME).toString();
        name = n+" "+f;
        }
        
    return name;
}


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
#include "preview.h"

#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QTextDocument>
#include <QDebug>
#include "CApp.h"

//------------------------------------------------ Preview ---------------------------------------------------------
Preview::Preview(QWidget * parent)
{
    setupUi(this);
    m_resources = CApp::pCApp()->pathAppli()+"Ressources";
    setCombo();
    qDebug() << __FILE__ << QString::number(__LINE__) << " file =" << comboBox->currentText() ;
    displayFile(comboBox->currentText());
    connect(quitButton,SIGNAL(pressed()),this,SLOT(close()));
    connect(comboBox,SIGNAL(highlighted(const QString&)),this,SLOT(displayFile(const QString&)));
    connect(applyButton,SIGNAL(pressed()),this,SLOT(modifytext()));
    
}

//------------------------------------------------ ~Preview ---------------------------------------------------------
Preview::~Preview(){}

//------------------------------------------------ displayFile ---------------------------------------------------------
void Preview::displayFile(const QString & fileName)
{
    QStringList textlist;
    QString text;
    QFile file(m_resources+QDir::separator()+"apitemp"+QDir::separator()+fileName);
    if (!file.open(QIODevice::ReadWrite|QIODevice::Text))
       {
          qWarning() << __FILE__ << QString::number(__LINE__) << "unable to open file : " << fileName ;
       }
    QTextStream stream(&file);
    while (!stream.atEnd())
        {
          QString line = stream.readLine();
          textlist << line;
        }
    text = textlist.join("\n");
    QTextDocument *doc = new QTextDocument;


    text.replace("\n\r","\n");
    text.replace("\n","<br/>");
    text.replace("\r","<br/>");
    text.replace("\t","&nbsp;&nbsp;&nbsp;&nbsp; ");
    text.replace("\'","\\'");

    QString texte;
    texte = CApp::pCApp()->convertToHtml(text);
    /*
    switch(version){
        case 12 :
             qWarning() << __FILE__ << QString::number(__LINE__) << "version 12" ;
             texte = QString("<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\" ?><MedinTux_Multimedia_data>"
                     "<HTML_Data><html><head><meta name=\"qrichtext\" content=\"1\" /></head>"
                     "<body style=\"font-size:%1pt;font-family:%2\"><p>%3</p></body></html></HTML_Data><ImageListe></ImageListe></MedinTux_Multimedia_data>")
                     .arg( CApp::pCApp()->result_point(),
                           CApp::pCApp()->result_family(),
                           text);
             break;
        case 14 :
             qWarning() << __FILE__ << QString::number(__LINE__) << "version 14" ;
             texte = QString("<html><head><meta name=\"qrichtext\" content=\"1\" />""</head>"
                             "<body style=\"font-size:%1pt;font-family:%2\"><p>%3</p></body></html>")
                             .arg( CApp::pCApp()->result_point(),
                                   CApp::pCApp()->result_family(),
                                   text);
             break;
        default :
             break;
        }
    */
    doc->setHtml(texte);;
    textEdit->setDocument(doc);
}

//------------------------------------------------ setCombo ---------------------------------------------------------
void Preview::setCombo()
{
    QDir dir(m_resources+"/apitemp");
    if (!dir.exists())
    {
          QMessageBox::warning(0,trUtf8("Erreur"),m_resources+"/apitemp"+trUtf8("n'existe pas.'"),QMessageBox::Ok);
        }
    dir.setFilter(QDir::Files);
    QStringList fileslist;
    fileslist = dir.entryList();
    int i = 0;
    foreach(QString file,fileslist){
        QString simplifiedNameFile = file;
        if (file.contains(QDir::separator()))
        {
              simplifiedNameFile = file.split(QDir::separator()).last();
            }
         m_hashIntFiles.insert(i,simplifiedNameFile);
        comboBox->addItem(simplifiedNameFile);//TODO classer items
        }
}

//------------------------------------------------ modifytext ---------------------------------------------------------
void Preview::modifytext(){
  QString texte;
  texte = textEdit->toPlainText();
  QString fileName = comboBox->currentText();
  QString resutext = m_resources+QDir::separator()+"apitemp"+QDir::separator()+fileName;
  QFile file(resutext);
  if(!file.open(QIODevice::ReadWrite|QIODevice::Text)){
	QMessageBox::warning(0,tr("erreur"),trUtf8("resutext ne peut pas être ouvert !"),QMessageBox::Ok);
	}
  QTextStream tamise(&file);
  const char*codec = "ISO-8859-1";
  tamise.setCodec(codec);
  tamise << texte;
  file.close();
}

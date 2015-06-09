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
#include "configdialog.h"
#include "CApp.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSettings>
#include <QSqlError>
#include <QString>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QMessageBox>

config::config(QWidget * parent)
{
  setupUi(this);
  m_resources = CApp::pCApp()->pathAppli()+"Ressources";
  comboBox->setEditable(true);
  comboBox->setInsertPolicy(QComboBox::NoInsert);

  QStringList listecombobox;
  QSqlQuery query(CApp::pCApp()->database());
  if  (!query.exec("SELECT Login, ID_PrimKey FROM Personnes"))
      {
        qWarning() << __FILE__ << QString::number(__LINE__) << query.lastError().text() ;
      }
  while(query.next()){
      QString login   = query.value(0).toString();
      QString primkey = query.value(1).toString();
      QString total   = login+":"+primkey;
      listecombobox  << total;
      }
  comboBox->addItems(listecombobox);
  setTitlesCombo();
  connect(pushButton,SIGNAL(pressed()),this,SLOT(enregistreconfig()));
  connect(pushButton_2,SIGNAL(pressed()),this,SLOT(close()));
  connect(pushButton_3,SIGNAL(pressed()),this,SLOT(enregistrerutilisateurpref()));
  connect(pushButton_4,SIGNAL(pressed()),this,SLOT(purgeSauvegardeDossiers()));
  connect(deleteButton,SIGNAL(pressed()),this,SLOT(deleteTitle()));
  connect(addButton,SIGNAL(pressed()),this,SLOT(addTitle()));
  connect(titlesComboBox,SIGNAL(highlighted(const QString &)),this,SLOT(feelEdits(const QString &)));
}

config::~config(){}

void config::enregistreconfig()
{
    QString nombre = indexEdit->text();
    QString terme = choosenwordEdit->text();
    QString titre = titleEdit->text();
    QString ligne = nombre+"="+terme+"="+titre;
    QFile file(m_resources+"/titres.ini");
    if(!file.open(QIODevice::ReadWrite|QIODevice::Append))
    {
        QMessageBox::warning(0,tr("erreur"),trUtf8("erreur ouverture fichier titres.ini !"),QMessageBox::Ok);
        }
    QTextStream stream(&file);
    stream << ligne+"\n";
    file.close();
    if(file.errorString() == "Unknown error"){
        QMessageBox::information(0,tr("information"),trUtf8(" titres.ini a été renseigné !"),QMessageBox::Close);
        }
    setTitlesCombo();
}

void config::enregistrerutilisateurpref(){
    QString utilisateurchoisi = comboBox->currentText();
    QStringList listeutilisateurini;
    QString fileiniName = m_resources+"/apitux.ini";
    QFile utilisateurini(fileiniName);
    if(!utilisateurini.open(QIODevice::ReadWrite | QIODevice::Text)){
        QMessageBox::warning(0,tr("Erreur"),trUtf8("apitux.ini ne peut pas être ouvert\npar configdialog."),QMessageBox::Ok);
        }
    QSettings set(fileiniName,QSettings::IniFormat);
    set.setValue("users/login",utilisateurchoisi);
    QMessageBox::information(0,tr(""),trUtf8("L'utilisateur préférentiel a été renseigné."),QMessageBox::Ok);
    qDebug() << __FILE__ << QString::number(__LINE__) << "emit resetUserCombo";
    emit close();
    emit resetUserCombo();
}

void config::purgeSauvegardeDossiers(){
    QString warningText = trUtf8("Voulez vous vraiment effacer les fichiers sauvegardés dans apitempSauve ?");
    QDir dir(m_resources+"/apitempSauve");
    QMessageBox mess;
    mess.setWindowTitle("Attention !");
    mess.setIcon(QMessageBox::Warning);
    mess.setText(warningText);
    mess.setStandardButtons(QMessageBox::Ok|QMessageBox::Abort);
    mess.setDefaultButton(QMessageBox::Abort);
    if(mess.exec() == QMessageBox::Ok){       
       QStringList listOfFiles;
       listOfFiles = dir.entryList();
       if(listOfFiles.size()>0){
          QString strFile;
          foreach(strFile,listOfFiles){
              if(strFile != "." && strFile != ".."){
                 QFile file(m_resources+"/apitempSauve/"+strFile);
  	             file.remove();
	             }
              }
          }
      }
    if (dir.entryList(QDir::Files).size()<1)
    {
          QMessageBox::information(0,trUtf8("Information"),trUtf8("Les fichiers ont été effacés "
                                   "dans le répertoire apitempSauve."),QMessageBox::Ok);
        }
}

void config::setTitlesCombo()
{
    titlesComboBox->clear();
    QStringList list;
    QFile file(m_resources+"/titres.ini");
    if (!file.open(QIODevice::ReadOnly))
    {
          qWarning() << __FILE__ << QString::number(__LINE__) << "unable to open file titres.ini" ;
        }
    QTextStream s(&file);
    while (!s.atEnd())
    {
        QString line = s.readLine();
        if (!line.isEmpty())
        {
              list << line;
            }
        }
    titlesComboBox->addItems(list);
}

void config::deleteTitle()
{
    QString titletoremove = titlesComboBox->currentText();
    QStringList list;
    QFile file(m_resources+"/titres.ini");
    if (!file.open(QIODevice::ReadWrite|QIODevice::Text))
    {
          qWarning() << __FILE__ << QString::number(__LINE__) << "unable to open file titres.ini" ;
        }
    QTextStream s(&file);
    while (!s.atEnd())
    {
        QString line = s.readLine();
        if (!line.isEmpty())
        {
              list << line;
            }
        }
    file.remove();    
    list.removeAll(titletoremove);
    QFile filenew(m_resources+"/titres.ini");
    if (!filenew.open(QIODevice::ReadWrite|QIODevice::Text))
    {
          qWarning() << __FILE__ << QString::number(__LINE__) << "unable to open file titres.ini" ;
        }
    QTextStream stream(&filenew);
    foreach(QString title,list){
        stream << title+"\n";
        }
    filenew.close();
    setTitlesCombo();
    indexEdit->clear();
    choosenwordEdit->clear();
    titleEdit->clear();
}

void config::addTitle()
{
    indexEdit->clear();
    choosenwordEdit->clear();
    titleEdit->clear();
    indexEdit->setFocus();
}

void config::feelEdits(const QString & title)
{
   enum EditsIndexes {INDEX=0,WORD,TITLE};
   QString text = title;
   QStringList list;
   if (text.contains("="))
   {
         list = text.split("=");
       }
    if (list.size()<3)
    {
          QMessageBox::warning(0,trUtf8("Erreur"),trUtf8("Votre titre doit comporter une ligne, "
                    "un mot choisi et un titre choisi !"),QMessageBox::Ok);
          return;
        }
    else
    {
        indexEdit->setText(list[INDEX]);
        choosenwordEdit->setText(list[WORD]);
        titleEdit->setText(list[TITLE]);
        }
}

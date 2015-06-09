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
#include "apidialog.h"
#include "textdialog.h"
#include "preview.h"
#include "postview.h"
#include "CApp.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/Theme/Theme.h"
#include <QtGui>
#include <QtCore>
#include <QtSql>
#include <QtDebug>


enum WarnDebug{WarnDebugMessage=false};

//////////////constructeur///////////////////////////////////:
apidialog::apidialog(QWidget * parent, Qt::WindowFlags f )
    :QDialog(parent,f)
{
  setupUi(this);
  textEdit_changeLog->hide();
  if (CApp::pCApp()->hideInfoWidget()) textEdit->hide();
  setAttribute(Qt::WA_DeleteOnClose);
  m_compteur             = 0;
  m_Apropos_Proc         = 0;
  m_namesIntegrated      = "";
  m_config               = new config(this);
  postvisuButton->setEnabled(false);  
  //codecs

  codecLabel->setText("codec");
  enum HtmlCodes{NUMBERZERO = 0,NUMBER_ONE,CODE,HTML_NUMBER,CHAR,HTML_NUMERIC_CODE,HTML_CHAR_CODE,EXPLANATION};
  QFile htmlFile(CApp::pCApp()->pathAppli()+"Ressources/codecs/html");
  if (!htmlFile.open(QIODevice::ReadOnly))
  {
  	  qWarning() << __FILE__ << QString::number(__LINE__) << "unable to open htmlFile" ;
      }
  QTextStream streamHtml(&htmlFile);
  while (!streamHtml.atEnd())
  {
  	QString line = streamHtml.readLine();
  	if (line.contains("&#"))
  	{
  		QStringList listLine = line.split("_")  ;
  		if (WarnDebugMessage)
  		
  		qDebug() << __FILE__ << QString::number(__LINE__) << " line =" << listLine.join(",") ;
  		m_hashHtml.insert(listLine[CHAR],listLine[HTML_NUMERIC_CODE]);  		
  	    }  	
      }
  //liste lignes d'entêtes
  QFile titresini(CApp::pCApp()->pathAppli()+"Ressources/titres.ini");
  if(!titresini.open(QIODevice::ReadWrite|QIODevice::Text))
  {
      QMessageBox::warning(0,tr("erreur"),trUtf8("erreur ouverture fichier titres.ini !"),QMessageBox::Ok);
      }
  QTextStream streamtitres(&titresini);
  while(!streamtitres.atEnd())
  {
      QString lignetitre = streamtitres.readLine();
      m_titres << lignetitre;
	  }
  m_resources = CApp::pCApp()->pathAppli()+"Ressources";
  pushButton_Apropos->setIcon(Theme::getIcon("22x22/help.png"));

  usersBox->setEditable(1);
  usersBox->setInsertPolicy(QComboBox::NoInsert);
  qDebug() << __FILE__ << " " << QString::number(__LINE__);
  setUsersCombo();
  searchOfLastIds();
  QDir dir(m_resources+"/apitemp");
  QStringList filters;
  filters << "*.txt" << "*.rtf" << "*.doc" << "*.hpr" << "*.odt" << "*.abw";
  dir.setNameFilters(filters);
  dir.setFilter(QDir::Files);
  int numberOfFilesToIntegrate = dir.entryList().size();
  QString labelText = trUtf8("Il y a ")+QString::number(numberOfFilesToIntegrate)+trUtf8(" fichier%1 \303\240 int\303\251grer.")
                                                                                          .arg(numberOfFilesToIntegrate>1?"s":""); // ajuster le pluriel
  label->setWordWrap(true);
  label->setStyleSheet("*{ color:red }");
  label->setText(labelText);
  radioButton -> setChecked(true);
  connect(pushButton,         SIGNAL(pressed()),  this, SLOT(dansdocuments()));
  connect(pushButton_2,       SIGNAL(pressed()),  this, SLOT(close()));
  connect(pushButton_3,       SIGNAL(pressed()),  this, SLOT(configdialog()));
  connect(preVisuButton,      SIGNAL(pressed()),  this, SLOT(preView()));
  connect(postvisuButton,     SIGNAL(pressed()),  this, SLOT(postView()));
  connect(pushButton_Apropos, SIGNAL(pressed()),  this, SLOT(Slot_APropos()));
  if (!connect(m_config,SIGNAL(resetUserCombo()), this, SIGNAL(sigUsersCombo())))
  {
        qWarning() << __FILE__ << QString::number(__LINE__) << "unable to connect resetUserCombo to sigUsersCombo" ;
      }
  if (!connect(this,SIGNAL(sigUsersCombo()),this,SLOT(setUsersCombo())))
  {
        qWarning() << __FILE__ << QString::number(__LINE__) << "unable to connect sigUsersCombo to setUsersCombo" ;
      }

  QTimer::singleShot(500, this, SLOT(Slot_ActivateMainWindow())); // pour avoir la fenetre en avant plan sur ubuntu unity
}
//--------------------------------------- Slot_ActivateMainWindow ---------------------------------------------------
void apidialog::Slot_ActivateMainWindow()
{   show();
    setWindowState(windowState() & ~Qt::WindowMinimized);
    activateWindow();
    raise();
}
//------------------------ apidialog -----------------------------------------
apidialog::~apidialog(){
    m_compteur  = 0 ;
    m_texteblob = "";
    m_login     = "";
    m_listelogin.clear();
    m_titres    .clear();
    }

//------------------------ event -----------------------------------------
bool apidialog::event ( QEvent * e )
{if (e->type()==QEvent::Close)
    {Slot_Apropos_Proc_finished (0,  QProcess::NormalExit);
    }
 return QWidget::event(e);
}
//------------------------ Slot_Apropos_Proc_finished -----------------------------------------
void apidialog::Slot_Apropos_Proc_finished (int,  QProcess::ExitStatus)
{if (m_Apropos_Proc)
    {m_Apropos_Proc->terminate();
     m_Apropos_Proc->waitForFinished (5000);
     delete m_Apropos_Proc;
     m_Apropos_Proc = 0;
    }
}
//------------------------------------ Slot_APropos -------------------------------
void apidialog::Slot_APropos ()
{   QString macAdr;
    QString ipAdr  = CApp::pCApp()->get_Current_IP_Adr(&macAdr);
    CGestIni::Param_UpdateToDisk(CApp::pCApp()->pathAppli()+"Ressources/Changements.html",
                                 textEdit_changeLog->toHtml().replace("{{IP_ADR}}",  ipAdr)
                                                             .replace("{{MAC_ADR}}", macAdr)
                                                             .replace("{{PATH_INI}}",CApp::pCApp()->pathIni())
                                 );
    QString pathExeAPropos     = CGestIni::Construct_Name_Exe("APropos",       QFileInfo (qApp->argv()[0]).path());
    QStringList argList;
    //......................... completer les autres arguments .........................................
    argList << CApp::pCApp()->applicationName();                                                            // 1  nom du module
    argList << tr("Module for Apicrypt cripted messages integration<br/>"
                  "original code by <b>Docteur Pierre-Marie Desombre</b><br/>"
                  "MedinTux adaptation by Roland Sevin");                                                   // 2  description courte
    argList << CApp::pCApp()->ApplicationAndQtVersion(tr("%1:%2<br/>").arg(__DATE__,__TIME__));             // 3  numero de version
    argList << CApp::pCApp()->pathAppli()+"Ressources/Changements.html";                                    // 4  fichiers decrivant les changements
    argList <<"";                                                                                           // 5  Icone par defaut
    argList <<"";                                                                                           // 6  aide en ligne (vide pour prendre celle par defaut)
    argList <<"";                                                                                           // 7  apropos (on met une chaine vide pour qu'il prenne celui par d?faut)
    argList <<"";                                                                                           // 8  numero de version de la base de donnee
    if (m_Apropos_Proc==0)
       { 
         m_Apropos_Proc = new QProcess(this);
         if (m_Apropos_Proc)
            {connect( m_Apropos_Proc, SIGNAL(finished ( int,  QProcess::ExitStatus)),  this, SLOT(Slot_Apropos_Proc_finished (int,  QProcess::ExitStatus)) );
             m_Apropos_Proc->start(pathExeAPropos, argList);
             m_Apropos_Proc->waitForStarted  (4000);
            }
       }
}
////////////////////////////////////////////////////////////////////////////////
//fonction d'int\303\251gration dans documents
///////////////////////////////////////////////////////////////////////////////
bool apidialog::dansdocuments(){
  label->setText("");
  QString user;
  QString userandprimkey = usersBox->currentText();
  QStringList listuserprimkey;
  listuserprimkey = userandprimkey.split(":");
  if (listuserprimkey.size() > 0)
  {
        user = listuserprimkey[0];
      }
  QDir dir(m_resources+"/apitemp");
  if(radioButton_2 -> isChecked()){
      dir.setPath(m_resources+"/classeur");
    }
  QStringList filters;
  filters << "*.txt" << "*.rtf" << "*.doc" << "*.hpr" << "*.odt" << "*.abw";
  dir.setNameFilters(filters);
  dir.setFilter(QDir::Files);
  QStringList listresutext ;
  listresutext = dir.entryList();
  if(listresutext.size() == 0){
      QMessageBox::warning(0,tr("erreur"),trUtf8("erreur: il n'y a pas de fichier \303\240 int\303\251grer !"),QMessageBox::Close);
      return false;
  }
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  //boucle
  for(int i=0;i<listresutext.size();i++){
      QString texteavantblob;
      QString resutext = m_resources+"/apitemp/"+listresutext[i];
      if(radioButton_2 -> isChecked()){
          resutext = m_resources+"/classeur/"+listresutext[i];
      }
      QFile resutextfile(resutext);
      if(!resutextfile.open(QIODevice::ReadWrite|QIODevice::Text)){
           QMessageBox::warning(0,tr("erreur"),trUtf8("erreur ouverture fichier !"),QMessageBox::Ok);
	      }
      QStringList listedutexte;
      QTextStream stream(&resutextfile);
      /*char * codec = choosenCodec();
      QString codecString(codec);
      codecLabel->setText(codecString);
      stream.setCodec(codec);*///("ISO-8859-1");
      while(!stream.atEnd()){
          QString ligne = stream.readLine();
          if (ligne.contains("<"))
          {
          	ligne.replace("<","&#60");
          	}
         //ligne.replace("\n","<br/>");
         //ligne.replace("\r\n","<br/>");
         listedutexte << ligne;
         texteavantblob += ligne+"\n";//<br/>";
	  }
      texteavantblob = replaceInHtml(texteavantblob);
      if (WarnDebugMessage)
      qDebug() << __FILE__ << QString::number(__LINE__) << texteavantblob  ;
      QString nom = listedutexte[1];
      QString prenom = listedutexte[2];
      qDebug() << __FILE__ << QString::number(__LINE__);

      if(radioButton_2 -> isChecked()){
          QStringList listFichier = listresutext[i].split("_");
          nom = listFichier[0];
          prenom = listFichier[1];
      }
    if(nom.contains("'")){//integration des particularites semantiques du nom pour la syntaxe mysql
        nom.replace("'","''");
    }
    if(prenom.contains("'")){//integration des particularites semantiques du nom pour la syntaxe mysql
        prenom.replace("'","''");
    }
    QString name;
    QString surname;
    QString fchgnrliddos;
    QStringList listeiddos;
    QDateTime dateTime = QDateTime::currentDateTime();
    QSqlQuery query(CApp::pCApp()->database());
    if(!query.exec("SELECT FchGnrl_NomDos , FchGnrl_Prenom , FchGnrl_IDDos "
                   "from IndexNomPrenom WHERE FchGnrl_NomDos = '"+nom+"' AND FchGnrl_Prenom = '"+prenom+"';")){
        qWarning() << __FILE__ << " " << QString::number(__LINE__) << " " << query.lastError().text();
        }
  while(query.next())
	{
      name = query.value(0).toString();
      surname = query.value(1).toString();
      fchgnrliddos = query.value(2).toString();
      listeiddos << fchgnrliddos;
	}

  if(listeiddos.size() > 1 || listeiddos.size() == 0)
	{qDebug() << __FILE__ << QString::number(__LINE__);
      textdialog *text = new textdialog(name,surname,user,listedutexte,resutext);//nom,pr\303\251nom,utilisateur,texte,chemin du texte.
      text->show();
      qDebug() << __FILE__ << QString::number(__LINE__);
      break;
		}
  else{
      //int\303\251gration du texte dans rubriqueblobs
      m_texteblob = CApp::pCApp()->convertToHtml(texteavantblob);
      /*
      switch(version){
          case 12 :
              qDebug() << __FILE__ << QString::number(__LINE__) << " use of 12 version "  ;
              m_texteblob = QString("<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone"
                    "=\"yes\" ?><MedinTux_Multimedia_data><HTML_Data><html><head><meta "
                    "name=\"qrichtext\" content=\"1\" /></head><body style=\"font-size:%1pt;font-family:"
                    "%2\"><p>%3</p></body></html></HTML_Data><ImageListe></ImageListe>"
                    "</MedinTux_Multimedia_data>").arg( CApp::pCApp()->result_point(),
                                                        CApp::pCApp()->result_family(),
                                                        texteavantblob);
              break;
          case 14 :
              qDebug() << __FILE__ << QString::number(__LINE__) << " use of 14 version "  ;
              m_texteblob = QString("<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body style=\"font-size:%1pt;font-family:%2\"><p>%3</p></body></html>")
                                   .arg( CApp::pCApp()->result_point(),
                                         CApp::pCApp()->result_family(),
                                         texteavantblob);
              break;
          default :
          break;
      }
      */
      QSqlQuery query1(CApp::pCApp()->database());
      if(!query1.exec("INSERT INTO RubriquesBlobs (RbDate_DataRub,RbDate_IDDos) VALUES ('"+m_texteblob+"','"+fchgnrliddos+"')")){
          qWarning() << __FILE__ << " " << QString::number(__LINE__) << " " << query1.lastError().text();
          }
      //int\303\251gration donn\303\251es dans rubriqueheads
      QString id_head = query1.lastInsertId().toString();
      QString titre_head = fonctiontitre(listedutexte);
      qDebug() << "titre ="+titre_head << " " << __FILE__ << QString::number(__LINE__);
      if(radioButton_2 -> isChecked()){
          titre_head = resutext.split("_")[2];
          titre_head = titre_head.split(".")[0];
          }
      if(titre_head == ""){
	      titre_head = "docs_externes";
	      }
      QDateTime datetime = QDateTime::currentDateTime();
      QString dateheure = datetime.toString("yyyy-MM-dd hh:mm:ss");
      //datetime = QDate::currentDateTime();
      QSqlQuery query2(CApp::pCApp()->database());
      if(!query2.exec("INSERT INTO RubriquesHead"
                      "(RbDate_PrimKey,RbDate_IDDos,RbDate_TypeRub,RbDate_NomDate,RbDate_Date,RbDate_CreateUser,RbDate_CreateSignUser"
                      ",RbDate_RefBlobs_Primkey)"
                      " VALUES (NULL,'"+fchgnrliddos+"',20080000,'"+titre_head+"','"+dateheure+"','"+user+"','"+user+"','"+id_head+"')")){
          qWarning() << __FILE__ << " " << QString::number(__LINE__) << " " << query2.lastError().text();
          }
    if(query1.numRowsAffected()<1){
        QMessageBox::warning(0,tr("erreur"),""+query1.lastError().text()+"",QMessageBox::Ok);
        return false;
        }
    else if(query2.numRowsAffected()<1){
        QMessageBox::warning(0,tr("erreur"),""+query2.lastError().text()+"",QMessageBox::Ok);
        return false;
    }
    else{
        m_compteur += query2.numRowsAffected();
        m_dossCumulatedList << fchgnrliddos;
        resutextfile . copy(m_resources+"/apitempSauve/"+name+"_"+surname+"_"+dateTime.toString("yyyy-MM-dd-hh-mm-ss")+"_"+listresutext[i]);
        m_texteblob = "";
        name = "";
        surname = "";
        fchgnrliddos = "";
        listeiddos.clear();
        resutextfile.remove();
        }
    }//else

  }//for
  QApplication::restoreOverrideCursor();
  QString strDoss;
  foreach(strDoss,m_dossCumulatedList){
      qDebug() << __FILE__ << " " << QString::number(__LINE__) << " doss = " << strDoss;
      QSqlQuery queryResult(CApp::pCApp()->database());
      if(!queryResult.exec("SELECT FchGnrl_NomDos , FchGnrl_Prenom  FROM IndexNomPrenom WHERE FchGnrl_IDDos = '"+strDoss+"'")){
          qWarning() << __FILE__ << " " << QString::number(__LINE__) << " " << queryResult.lastError().text();
          }
      while(queryResult.next()){
          qDebug() << __FILE__ << " " << QString::number(__LINE__);
          m_namesIntegrated += queryResult.value(0).toString()+" "+queryResult.value(1).toString()+"\n";
          }
      }
    QString stringMess = m_namesIntegrated;
    qWarning() << __FILE__ << " " << QString::number(__LINE__) << " names = " << stringMess;
    QMessageBox::information(0,tr("information"),trUtf8("Int\303\251gration "
    "de ")+QString::number(m_compteur)+trUtf8(" documents\ndans la base.\n")
    +trUtf8("Les noms des patients sont : \n")+stringMess,QMessageBox::Ok);

    m_namesIntegrated = "";
    m_compteur = 0;
    postvisuButton->setEnabled(true);
  return true;
}

void apidialog::configdialog(){
    m_config->show();
}

QString apidialog::fonctiontitre(QStringList &stringlist){

  QString titrechoisi = "";
  qDebug() << __FILE__ << QString::number(__LINE__) << " nombre de titres =" << QString::number(m_titres.size()) ;
	for(int i=0; i < m_titres.size(); i++)
	{
	    if(m_titres[i].contains("=")){
	       QStringList titresplit = m_titres[i].split("=");
	       int ligne = titresplit[0].toInt();
	       QString terme = titresplit[1];
	       QString titre = titresplit[2];
	       if (WarnDebugMessage)
	       qDebug() << __FILE__ << QString::number(__LINE__) << " titre =" << titre ;
           if (stringlist.size()<ligne+1)
           {
                 qWarning() << __FILE__ << QString::number(__LINE__) << "stringlist.size()<ligne+1" ;
                 continue;
               }
           if(stringlist[ligne].contains(terme))
		  {
	           titrechoisi = titre;
		                }
			    }
			}//for
  return titrechoisi;
}

void apidialog::changeLabel(){
    QDir dir(CApp::pCApp()->pathAppli()+"classeur");
    QStringList list;
    list = dir.entryList();
    list.removeAll(".");
    list.removeAll("..");
    int numberOfFiles = list.size();
    qDebug() << __FILE__ << QString::number(__LINE__) << "numberOfFiles" << QString::number(numberOfFiles);
    QString labelText = trUtf8("Il y a ")+QString::number(numberOfFiles)+trUtf8(" fichier%1 \303\240 int\303\251grer.")
                                                                                .arg(numberOfFiles>1?"s":"");  // ajuster le pluriel
    label->setWordWrap(true);
    label->setStyleSheet("*{ color:red }");
    label->setText(labelText);
}

/*char * apidialog::choosenCodec()
{
    char * codec;// = new char("UTF8");
    QFile codecFile(CApp::pCApp()->pathAppli()+"codecs");
    if (!codecFile.open(QIODevice::ReadOnly))
    {
    	  qWarning() << __FILE__ << QString::number(__LINE__) << "unable to open codecs " ;
        }
    QTextStream stream(&codecFile);
    while (!stream.atEnd())
    {
    	QString line = stream.readLine();
    	QString *ptrLine = new QString(line);
    	QByteArray* array = new QByteArray(ptrLine->toLocal8Bit());
    	if (!line.contains("#"))
    	{
    		codec = array->data()  ;
    	    }
        }
    return codec;
}*/

QString apidialog::replaceInHtml(const QString & text)
{
    QString htmlString;
    htmlString = text;
    //htmlString.replace("\r","<br/>");
    //htmlString.replace("\n","<br/>");
    htmlString.replace("\t"," ");
    htmlString.replace("\'","\\'");
    if (WarnDebugMessage)
    qDebug() << __FILE__ << QString::number(__LINE__) << " htmlString =" << htmlString ;
    for (int i = 0; i < m_hashHtml.count(); ++i)
    {
    	  QString key = m_hashHtml.keys()[i];
    	  QString htmlValue = m_hashHtml.value(key);
    	  //htmlValue.replace("&#","\\");
    	  htmlString.replace(key,htmlValue);
    	  if (WarnDebugMessage)    	  
    	  qDebug() << __FILE__ << QString::number(__LINE__) << " key =" << key ;
    	  if (WarnDebugMessage)
    	  qDebug() << __FILE__ << QString::number(__LINE__) << "htmlValue  =" << htmlValue ;
        }
    htmlString.replace("\r\n","\n");
    htmlString.replace("\n","<br/>");
    if (WarnDebugMessage)
    qDebug() << __FILE__ << QString::number(__LINE__) << " htmlString after =" << htmlString ;
    return htmlString;
}

void apidialog::preView()
{
    Preview *p = new Preview(this);
    p->show();
}

void apidialog::setUsersCombo()
{
    qDebug() << __FILE__ << QString::number(__LINE__) << " in setUsersCombo " ;
    usersBox->clear();
    m_listelogin.clear();
    //choix user preferentiel et liste d'utilisateurs
    QString fileIni = CApp::pCApp()->pathAppli()+"Ressources/apitux.ini";
    QFile user(fileIni);
    if(!user.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(0,tr("erreur"),trUtf8("erreur ouverture fichier apitux.ini !"),QMessageBox::Ok);
	    }
    QSettings set(fileIni,QSettings::IniFormat);
    //choix user
    QString utilisateur = set.value("users/login").toString();
    m_listelogin << utilisateur;
    QSqlQuery query(CApp::pCApp()->database());
    if (!query.exec("SELECT Login,ID_PrimKey FROM Personnes"))
    {
        qWarning() << __FILE__ << QString::number(__LINE__) << query.lastError().text() ;
        }
    while(query.next()){
       m_login = query.value(0).toString();
       QString idPrimkey = query.value(1).toString();
       m_listelogin << m_login+":"+idPrimkey;
       }
    usersBox->addItems(m_listelogin);
}

void apidialog::postView()
{
    enum UserEnum {LOGIN=0,PRIMKEYUSER};
    QString userall = usersBox->currentText();
    QString user;
    QStringList list;
    list = userall.split(":");
    if (list.size()>1)
    {
          user = list[LOGIN];
        }
    else
    {
        qWarning() << __FILE__ << QString::number(__LINE__) << "wrong choice of user" ;
        return;
        }
    PostView *post = new PostView(this,m_lastHeadsId,user);
    post->show();
}

void apidialog::searchOfLastIds()
{
    enum UserEnum {LOGIN=0,PRIMKEYUSER};
    int lastid = 0;
    QString user;
    QString userText = usersBox->currentText();
    qDebug() << __FILE__ << QString::number(__LINE__) << " userText =" << userText  ;
    QStringList listUser;
    listUser = userText.split(":");
    qDebug() << __FILE__ << QString::number(__LINE__) << " listUser size =" << QString::number(listUser.size()) ;
    if (listUser.size()>1)
    {
          user = listUser[LOGIN];
        }
    else
    {
        qWarning() << __FILE__ << QString::number(__LINE__) << "wrong choice of user" ;
        return;
        }

    QSqlQuery qHeads(CApp::pCApp()->database());
    QString reqHeads = QString("select %1 from %2 where %3 = '%4'").arg("RbDate_PrimKey","RubriquesHead","RbDate_CreateUser",user);
    if (!qHeads.exec(reqHeads))
    {
          qWarning() << __FILE__ << QString::number(__LINE__) << qHeads.lastError().text() ;
          return;
        }
    while (qHeads.next())
    {
        int id = qHeads.value(0).toInt();
        if (id>lastid)
        {
              lastid = id;
            }
        }
    m_lastHeadsId = QString::number(lastid);
    qDebug() << __FILE__ << QString::number(__LINE__) << " last id =" << m_lastHeadsId ;
}

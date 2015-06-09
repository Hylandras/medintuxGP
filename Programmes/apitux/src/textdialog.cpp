/***************************************************************************
 *   Copyright (C) 2009 by Docteur Pierre-Marie Desombre.                  *
 *   pm.desombre@medsyn.fr                                                 *
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
#include <QtSql>

#include "CApp.h"
#include "textdialog.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"


//------------------------------------------------ textdialog ---------------------------------------------------------
textdialog::textdialog(QString &name,QString &surname,QString &user,QStringList &texte,QString &resutext){
  QSqlDatabase db = QSqlDatabase::database();
  setupUi(this);
  pushButton_4 -> hide();
  setAttribute(Qt::WA_DeleteOnClose);
  //liste lignes d'entêtes
  comboBox_3->setEditable(true);
  comboBox_3->setInsertPolicy(QComboBox::InsertAtTop);
  m_resources = CApp::pCApp()->pathAppli()+"Ressources";
  QFile titresini2(m_resources+"/titres.ini");
  if(!titresini2.open(QIODevice::ReadWrite|QIODevice::Text))
		{
                 QMessageBox::warning(0,tr("erreur"),trUtf8("erreur ouverture fichier titres.ini !"),QMessageBox::Ok);
		}
  QStringList listetitres;
  QTextStream streamtitres2(&titresini2);
  while(!streamtitres2.atEnd()){
      QString lignetitre = streamtitres2.readLine();
      listetitres << ""+lignetitre+"";
				}
  m_titres2 = listetitres;
  comboBox_3->addItems(listetitres);
  //recup\303\251ration des arguments du constructeur
  m_nompatient     = name;
  m_prenompatient  = surname;
  m_resutext       = resutext;
  m_userpref       = user;
  m_textelist      = texte;
  m_texte          = texte.join("\n");

  QSqlQuery query(CApp::pCApp()->database());
  query.exec("SELECT FchGnrl_IDDos FROM IndexNomPrenom WHERE FchGnrl_NomDos = '"+m_nompatient+"' AND FchGnrl_Prenom = '"+m_prenompatient+"'");
  QStringList listiddos;
  while(query.next())
	{
  QString iddos = query.value(0).toString();
  listiddos << ""+iddos+"";
	}
  int taillelist = listiddos.size();
  if(taillelist == 0){
      pushButton->setEnabled(false);
      pushButton->hide();
      }

  //construction tableWidget
  tableWidget->setRowCount(taillelist);
  tableWidget->setColumnCount(7);
  tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Nom") << trUtf8("Pr\303\251nom") << trUtf8("n\303\251(e) le:") << trUtf8("lieu") << tr("CP") << trUtf8("Ville") << tr("guid"));
  for(int i=0;i<taillelist;i++)
	{
    QString naissance;
    QString adresse;
    QString cp;
    QString ville;
    QSqlQuery query1(CApp::pCApp()->database());
    query1.exec("SELECT FchPat_Nee,FchPat_Adresse,FchPat_CP,FchPat_Ville FROM fchpat WHERE FchPat_GUID_Doss = '"+listiddos[i]+"'");
    while(query1.next())	{
        naissance = query1.value(0).toString();
        adresse = query1.value(1).toString();
        cp = query1.value(2).toString();
        ville = query1.value(3).toString();
        }
  QTableWidgetItem *itemnom = new QTableWidgetItem(""+m_nompatient+"");
  tableWidget->setItem(i,0,itemnom);
  QTableWidgetItem *itemprenom = new QTableWidgetItem(""+m_prenompatient+"");
  tableWidget->setItem(i,1,itemprenom);
  QTableWidgetItem *itemnaissance = new QTableWidgetItem(""+naissance+"");
  tableWidget->setItem(i,2,itemnaissance);
  QTableWidgetItem *itemadresse = new QTableWidgetItem(""+adresse+"");
  tableWidget->setItem(i,3,itemadresse);
  QTableWidgetItem *itemcp = new QTableWidgetItem(""+cp+"");
  tableWidget->setItem(i,4,itemcp);
  QTableWidgetItem *itemville = new QTableWidgetItem(""+ville+"");
  tableWidget->setItem(i,5,itemville);
  QTableWidgetItem *itemguid = new QTableWidgetItem(""+listiddos[i]+"");
  tableWidget->setItem(i,6,itemguid);
  tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableWidget->show();
	}

  m_document = new QTextDocument;
  //m_document->setPlainText(m_texte);
  QString texteH = m_texte;
  texteH.replace("\r\n","\n");
  texteH.replace("\n","<br/>");
  texteH.replace("\r","<br/>");
  texteH.replace("\t"," ");
  texteH.replace("\'","\\'");

  m_document->setHtml(CApp::pCApp()->convertToHtml(texteH));
  textEdit->setDocument(m_document);
  textEdit->show();
  pushButton_2  -> setShortcut(QKeySequence::InsertParagraphSeparator);
  connect(pushButton,   SIGNAL(pressed()),     this, SLOT(enregistretexte()));
  connect(pushButton_2, SIGNAL(pressed()),     this, SLOT(close()));
  connect(pushButton_3, SIGNAL(pressed()),     this, SLOT(ouvrirmanager()));
  connect(pushButton_5, SIGNAL(pressed()),     this, SLOT(effacerFichier()));
  connect(textEdit,     SIGNAL(textChanged()), this, SLOT(modifiertexte()));
}//constructeur

//------------------------------------------------ ~textdialog ---------------------------------------------------------
textdialog::~textdialog(){
     m_nompatient   = "";
     m_prenompatient= "";
     m_resutext     = "";
     m_userpref     = "";
     m_texte        = "";
     m_textelist.clear();
     m_titres2  .clear();
     delete m_document;
    }

//------------------------------------------------ enregistretexte ---------------------------------------------------------
void textdialog::enregistretexte(){
  QString avanttexte = m_texte;
          avanttexte.replace("\n","<br/>");
          avanttexte.replace("\r","<br/>");
          avanttexte.replace("\t"," ");
          avanttexte.replace("\'","\\'");

  QString texte;
  texte = CApp::pCApp()->convertToHtml(avanttexte);
  /*
  switch(version){
      case 12 :
           qWarning() << __FILE__ << QString::number(__LINE__) << "version 12" ;
           texte = QString("<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\" ?><MedinTux_Multimedia_data>"
                   "<HTML_Data><html><head><meta name=\"qrichtext\" content=\"1\" /></head>"
                   "<body style=\"font-size:%1pt;font-family:%2\"><p>%3</p></body></html></HTML_Data><ImageListe></ImageListe></MedinTux_Multimedia_data>")
                   .arg( CApp::pCApp()->result_point(),
                         CApp::pCApp()->result_family(),
                         avanttexte);
           break;
      case 14 :
           qWarning() << __FILE__ << QString::number(__LINE__) << "version 14" ;
           texte = QString("<html><head><meta name=\"qrichtext\" content=\"1\" />""</head>"
                           "<body style=\"font-size:%1pt;font-family:%2\"><p>%3</p></body></html>")
                           .arg( CApp::pCApp()->result_point(),
                                 CApp::pCApp()->result_family(),
                                 avanttexte);
           break;
      default :
           break;
      }
  */
  int row = tableWidget->currentRow();
  if(row == -1){QMessageBox::warning(0,tr("erreur"),trUtf8("vous n'avez pas s\303\251lectionn\303\251 de patient !"),QMessageBox::Reset);}
  QTableWidgetItem *itemguid = tableWidget->item(row,6);
  QString guid = itemguid->text();
  QSqlQuery query(CApp::pCApp()->database());
  if (!query.exec("INSERT INTO RubriquesBlobs (RbDate_DataRub,RbDate_IDDos) VALUES ('"+texte+"','"+guid+"')"))
     {
        qWarning() << __FILE__  << QString::number(__LINE__) << Q_FUNC_INFO << query.lastError().text() ;
     }
  
  //int\303\251gration donn\303\251es dans rubriqueheads
  int id_head = query.lastInsertId().toInt();
  QString titre_head = fonctiontitre(m_textelist);
  if(titre_head.isEmpty()){
	titre_head = "docs_externes";
		}
  QDateTime datetime = QDateTime::currentDateTime();
  QString dateheure = datetime.toString("yyyy-MM-dd hh:mm:ss");
  QSqlQuery query2(CApp::pCApp()->database());
  if (!  query2.exec("INSERT INTO RubriquesHead"
                     "(RbDate_PrimKey,RbDate_IDDos,RbDate_TypeRub,RbDate_NomDate,RbDate_Date,RbDate_CreateUser,RbDate_CreateSignUser,RbDate_RefBlobs_Primkey)"
                     " VALUES (NULL,'"+guid+"',20080000,'"+titre_head+"','"+dateheure+"','"+m_userpref+"','"+m_userpref+"','"+QString::number(id_head)+"')"))
  {
        qWarning() << __FILE__ << QString::number(__LINE__)  << Q_FUNC_INFO << query2.lastError().text() ;
      }
  m_textelist.clear();
  if((m_nompatient.isEmpty()==false) && (m_prenompatient.isEmpty()==false))
  {
     QFile file(m_resutext);
     file.remove();}
     emit close();
    }

//------------------------------------------------ fonctiontitre ---------------------------------------------------------
  QString textdialog::fonctiontitre(QStringList &texte){
  QString titrechoisi;

  QStringList textelist = texte;


	for(int i=0;i<m_titres2.size();i++)
			{
	QStringList titresplit = m_titres2[i].split("=");
	int ligne = titresplit[0].toInt();
	QString terme = titresplit[1];
	QString titre = titresplit[2];
	if(textelist[ligne].contains(""+terme+""))
		{
	titrechoisi += titre;
		}
	else {titrechoisi += "";}
			}//for
    return titrechoisi;
}

//------------------------------------------------ ouvrirmanager ---------------------------------------------------------
void textdialog::ouvrirmanager(){
    QString managerDir = CApp::pCApp()->pathAppli()+"../../Manager";
    QDir dirManager;
    if (!dirManager.exists(managerDir))
       {
          QMessageBox::warning(0,trUtf8("Erreur"),trUtf8("Vous devez placer le r\303\251pertoire apitux \ndans Medintux/programmes \303\240 côt\303\251 de Manager."),QMessageBox::Ok);
       }
    
    QString manager = CGestIni::Construct_Name_Exe("Manager", QFileInfo (qApp->argv()[0]).path());
    qWarning() << __FILE__ << QString::number(__LINE__) << Q_FUNC_INFO << " Lancement de Manager : "<< manager;
    QProcess *process = new QProcess;
    process->start(manager);
}

//------------------------------------------------ modifiertexte ---------------------------------------------------------
void textdialog::modifiertexte(){
  QString texte;
  texte = textEdit->toPlainText();
  QFile file(m_resutext);
  if(!file.open(QIODevice::ReadWrite|QIODevice::Text)){
	QMessageBox::warning(0,tr("erreur"),trUtf8("resutext ne peut pas être ouvert !"),QMessageBox::Ok);
	}
  QTextStream tamise(&file);
  tamise.setCodec("ISO-8859-1");
  tamise << texte;
  file.close();
}

//------------------------------------------------ effacerFichier ---------------------------------------------------------
void textdialog::effacerFichier(){
  QFile file(m_resutext);
  QString warningText = "Voulez vous vraiment effacer le fichier : "+m_resutext+" ?";
  QMessageBox mess;
  mess.setWindowTitle("Attention !");
  mess.setIcon(QMessageBox::Warning);
  mess.setText(warningText);
  mess.setStandardButtons(QMessageBox::Ok|QMessageBox::Abort);
  mess.setDefaultButton(QMessageBox::Abort);
  if(mess.exec() == QMessageBox::Ok){
	file.remove();
	emit close();
	}
}


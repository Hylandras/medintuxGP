#include "dlgdesti.h"
#include "ui_dlgdesti.h"
#include "CApp.h"
#include "C_BaseSynoptux.h"

#define DESTINATIONS "synopt_destinations"
#define ENCOURS      "synopt_encours"
#define NOM_APPLI    "SynopTux"

DlgDesti::DlgDesti(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgDesti)
{
    ui->setupUi(this);

    connect (ui->pushButton_valider,    SIGNAL(clicked()),   this, SLOT(ValideDesti()));
    connect (ui->pushButton_annuler,    SIGNAL(clicked()),   this, SLOT(AnnuleDesti()));
    connect (ui->pushButton_modifHeure, SIGNAL(clicked()),   this, SLOT(ModifHeure()));
    connect (ui->pushButton_quitter,    SIGNAL(clicked()),   this, SLOT(close()));
    connect (ui->lineEdit_desti,        SIGNAL(textChanged(const QString &)),this, SLOT (majListeDesti()));
    connect (ui->treeWidget_desti,      SIGNAL(itemClicked(QTreeWidgetItem*,int)),   this, SLOT (selectDesti( QTreeWidgetItem * , int )));
}

DlgDesti::~DlgDesti()
{
    delete ui;
}

//-----------------------------------------changeEvent----------------------------------------------------------------
void DlgDesti::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
//-----------------------------------------selectDesti----------------------------------------------------------------
void DlgDesti::selectDesti(QTreeWidgetItem * , int)
{
QTreeWidgetItem *item = ui->treeWidget_desti->currentItem();

    ui->lineEdit_desti->setText(item->text(0));
    ui->pushButton_annuler->setEnabled(true);
    if (dTypeSaisie == "ListeSeule")
        ValideDesti();
}
//-------------------------------ValideEtat----------------------------------------------------
void DlgDesti::ValideDesti()
{
QString     requete;
QSqlQuery   queryd ( CApp::pCApp()->getDB()->database());

    // Mise à jour de la destination dans la table Encours
    requete = "UPDATE " ENCOURS " SET "
              " EC_Destination    = :EC_Destination,"
              " EC_HeureSortie    = :EC_HeureSortie "
              " WHERE EC_PK = '" + dNumEnCours + "'" ;

    queryd.prepare(requete);
    queryd.bindValue(":EC_Destination",    ui->lineEdit_desti->text());
    queryd.bindValue(":EC_HeureSortie",    ui->dateTimeEdit_HeureSortie->dateTime().toString("yyyy-MM-dd hh:mm:ss"));

    if (!queryd.exec() )
            {QString noerr;
            noerr.setNum(queryd.lastError().type());
            QMessageBox::warning ( this, tr("Status management"),
                         "<b>" + tr("Error when updating box table: ") + ui->lineEdit_desti->text() +  "</b><br /><br />" +
                         tr("Error = (") +  noerr + ") " + queryd.lastError().text());
            } // fin if erreur exec insert

    if (dTypeSaisie == "ListeSeule")
        accept();

    // AJout de la nelle destination dans la table des destinations
    requete = "SELECT   DE_Destination "
                " FROM    " DESTINATIONS
                " WHERE DE_Destination = '" + ui->lineEdit_desti->text() + "'"
                " AND   DE_TypeSortie  = '" + dTypeDeSortie + "'";

    QSqlQuery query(requete, CApp::pCApp()->getDB()->database());
    if (!query.isActive() ||  !query.next())
        { // Cette destination n 'existe pas >> on l'ajoute
        requete = "INSERT INTO " DESTINATIONS " (DE_Destination, DE_TypeSortie ) "
                  " VALUES (:DE_Destination, :DE_TypeSortie ) ";
        query.prepare(requete);
        query.bindValue(":DE_Destination",  ui->lineEdit_desti->text());
        query.bindValue(":DE_TypeSortie",   dTypeDeSortie);

        if (!query.exec() )
                {QString noerr;
                noerr.setNum(query.lastError().type());
                QMessageBox::warning ( this, tr("Status management"),
                             "<b>" + tr("Error when updating destinations table: ") + ui->lineEdit_desti->text() +  "</b><br /><br />" +
                             tr("Error = (") +  noerr + ") " + query.lastError().text());
                } // fin if erreur exec insert
        } // fin if n'existe pas
    ChargeDesti(dNumEnCours);

}
//-------------------------------AnnuleDesti----------------------------------------------------
void DlgDesti::AnnuleDesti()
{
QString     requete;
QSqlQuery   query ( CApp::pCApp()->getDB()->database());

    requete = "DELETE FROM " DESTINATIONS
              " WHERE DE_Destination = '" + ui->lineEdit_desti->text() + "'"
              " AND   DE_TypeSortie  = '" + dTypeDeSortie + "'" ;
    query.prepare(requete);
    if (!query.exec() )
            {QString noerr;
            noerr.setNum(query.lastError().type());
            QMessageBox::warning ( this, tr("Destinations management"),
                         "<b>" + tr("Error when updating etats table: ") + ui->lineEdit_desti->text() +  "</b><br><br>" +
                         tr("Error = (") +  noerr + ") " + query.lastError().text());
            }
    ChargeDesti(dNumEnCours);
}

//-------------------------------ChargeDesti----------------------------------------------------
void DlgDesti::ChargeDesti(QString NumEncours)
{
    //QString         requete;
    //QTreeWidgetItem *pItem = 0;
    dNumEnCours = NumEncours;
    ui->treeWidget_desti->clear();
    ui->lineEdit_desti->clear();
    ui->dateTimeEdit_HeureSortie->setDateTime(QDateTime::currentDateTime());
    ui->pushButton_annuler->setEnabled(false);

    if (dTypeDeSortie == tr("Exit"))
        this->setWindowTitle(tr("Destinations when exiting service"));
    else
        this->setWindowTitle(tr("Destinations when exiting temporarily"));
    /* inutile RS

    // lecture de la destination actuelle dans la base
    requete = "SELECT   EC_Destination, EC_HeureSortie "
              " FROM    " ENCOURS
              " WHERE EC_PK = " + dNumEnCours ;

    QSqlQuery querye(requete, CApp::pCApp()->getDB()->database());
    while (querye.isActive() &&  querye.next())
        { pItem = new QTreeWidgetItem() ;
        // plus utile RS ui->lineEdit_desti->setText(querye.value(0).toString());
        ui->dateTimeEdit_HeureSortie->setDateTime(QDateTime::currentDateTime());
        }
    */
    // remplit le treewidget de selection avec les destinations connues dans la base
    majListeDesti();

}
//-----------------------------------majListeDesti-------------------------------------------------
void DlgDesti::majListeDesti()
{
QString requete;
QTreeWidgetItem *pItem = 0;

    requete =   " SELECT DE_Destination "
                " FROM " DESTINATIONS
                " WHERE  DE_TypeSortie  = '" + dTypeDeSortie + "'" ;
    if (ui->lineEdit_desti->text().length() > 0)
        requete.append(" AND DE_Destination like '" +  ui->lineEdit_desti->text() + "%' ORDER BY DE_Destination LIMIT 0,100 ");
    requete.append(" ORDER BY DE_Destination");
   QSqlQuery query(requete, CApp::pCApp()->getDB()->database());
   ui->treeWidget_desti->clear();
   while (query.isActive() &&  query.next())
        {pItem = new QTreeWidgetItem() ;
         pItem->setText(0, query.value(0).toString());
         ui->treeWidget_desti->addTopLevelItem(pItem);
        }
}
//-----------------------------------ModifHeure-------------------------------------------------
void DlgDesti::ModifHeure()
{

    if (dExeCalendrier.length() < 1) return;
    #ifdef Q_WS_WIN
    if (dExeCalendrier.right(4) != ".exe") dExeCalendrier.append(".exe");
    #endif
    QString nomFicRetourDate  = QCoreApplication::applicationDirPath() + "/ficDate";

    QDate   dateIn   = QDate::currentDate();
    QFile::remove(nomFicRetourDate);
    QStringList argList;
    //......................... completer les autres arguments .........................................
    argList <<(" ");                                // 1  Chemin d'un eventuel fichier de configuration pour XXXX.ini l'executable ou %
    argList <<(" ");                                // 2  Chemin de l'executable appelant
    argList <<(nomFicRetourDate);                   // 3  Chemin du fichier d'echange texte masque d'entrée dont l'executable devra modifier les valeurs de retour
    argList <<(" ");                                // 4  GUID du patient
    argList <<(" ");                                // 5  PrimKey de l'observation en cours
    argList <<(" ");                                // 6  PrimKey du terrain en cours
    argList <<(" ");                                // 7  PrimKey de l'ordonnance en cours
    argList <<(" ");                                // 8  reservé
    argList <<(" ");                                // 9  reservé
    argList <<(" ");                                // 10 reservé
    argList <<(" ");                                // 11 reservé
    argList <<("6");                                // 12 Nb autres parametres d'appels specifiques au plugin
    argList << dateIn.toString("ddMMyyyy");         // 13 Date jour en entrée
    argList << "ddMMyyyy";                          // 14 format date de sortie
    argList << dateIn.toString("ddMMyyyy");         // 15 format date de sortie
    argList << dateIn.toString("ddMMyyyy");         // 16 format date de sortie
    argList << "";                                  // 17 format date de sortie
    argList << "days|debDate|endDate|hours";        // 18 format date de sortie

    //................................. executer le process .............................................
    QProcess   proc;
    proc.start(dExeCalendrier, argList);
    proc.waitForFinished(60000);

    //............lire le fichier d'echange ..........................
    if ( !QFile::exists( nomFicRetourDate ) )     return ;

    QFile *fileDate = new QFile(nomFicRetourDate);
    if (!fileDate->open(QIODevice::ReadOnly))
        {QMessageBox::information(this, NOM_APPLI, tr("Impossible to open file %1:<br />%2.")
                                 .arg(nomFicRetourDate).arg(fileDate->errorString()));
        return;
        }

    QByteArray dateAdm = fileDate->readAll();

    fileDate->close();
    delete fileDate;
    if (dateAdm.length() < 1)
         return ;
    dateIn.setDate(dateAdm.mid(4,4).toInt(), dateAdm.mid(2,2).toInt(), dateAdm.mid(0,2).toInt());
    ui->dateTimeEdit_HeureSortie->setDate(dateIn);

}


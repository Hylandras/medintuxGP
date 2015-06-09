#include "dlgetat.h"
#include "ui_dlgetat.h"
#include <QtGui>
#include "CApp.h"
#include "C_BaseSynoptux.h"

#define ETATS "synopt_etats"

//-------------------------------DlgEtat----------------------------------------------------
DlgEtat::DlgEtat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgEtat)
{
    ui->setupUi(this);

    connect (ui->pushButton_QuitterEtat,   SIGNAL(clicked()),   this, SLOT(accept()));
    connect (ui->pushButton_ValidEtat,     SIGNAL(clicked()),   this, SLOT(ValideEtat()));
    connect (ui->pushButton_annuler,       SIGNAL(clicked()),   this, SLOT(AnnuleEtat()));
    connect (ui->pushButton_CouleurFond,   SIGNAL(clicked()),    this, SLOT(ChoixCouleurFond()));
    connect (ui->treeWidget_Etat,          SIGNAL(itemClicked(QTreeWidgetItem*,int)),   this, SLOT (selectEtat( QTreeWidgetItem * , int )));
}

//------------------------------- ~DlgEtat----------------------------------------------------
DlgEtat::~DlgEtat()
{
    delete ui;
}

//-------------------------------changeEvent----------------------------------------------------
void DlgEtat::changeEvent(QEvent *e)
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
//-----------------------------------------selectEtat----------------------------------------------------------------
void DlgEtat::selectEtat(QTreeWidgetItem * , int)
{
QTreeWidgetItem *item = ui->treeWidget_Etat->currentItem();

EtatEnCours = item->text(0);

QString requete;

requete = "SELECT   ET_Code_etat, ET_Libelle_etat, ET_Couleur_etat, ET_Tache_terminee "   // 0-1-2-3
          " FROM    " ETATS
          " WHERE  ET_Code_etat = '" + EtatEnCours + "'" ;
    QSqlQuery query(requete, CApp::pCApp()->getDB()->database());
    while (query.isActive() &&  query.next())
        {ui->lineEdit_CodeEtat->setText( query.value(0).toString());
        ui->lineEdit_NomEtat->setText(   query.value(1).toString());
        ui->lineEdit_CouleurFondCode->setText(query.value(2).toString());
        ui->pushButton_CouleurFond->setStyleSheet("background-color:" + query.value(2).toString()  + ";");
        ui->checkBox_Tache_Terminee->setChecked(query.value(3).toBool());
        }
    ui->pushButton_annuler->setEnabled(true);
    ui->pushButton_ValidEtat->setEnabled(true);
    ui->lineEdit_CodeEtat->setEnabled(false);
}
//-------------------------------ValideEtat----------------------------------------------------
void DlgEtat::ValideEtat()
{
QString     requete;
QSqlQuery   query ( CApp::pCApp()->getDB()->database());

    if (EtatEnCours.length() == 0)            // on est en création ....
        {
        if (ui->lineEdit_CodeEtat->text().length()== 0) return;

        requete = "INSERT INTO " ETATS " ("
                  " ET_Code_etat, ET_Libelle_etat,   ET_Couleur_etat, ET_Tache_terminee) "
                  " VALUES ("
                  " :ET_Code_etat, :ET_Libelle_etat,   :ET_Couleur_etat, :ET_Tache_terminee ) ";

        query.prepare(requete);
        query.bindValue(":ET_Code_etat",        ui->lineEdit_CodeEtat->text());
        query.bindValue(":ET_Libelle_etat",     ui->lineEdit_NomEtat->text());
        query.bindValue(":ET_Couleur_etat",     ui->lineEdit_CouleurFondCode->text());
        query.bindValue(":ET_Tache_terminee",   ui->checkBox_Tache_Terminee->isChecked());
        }
    else        // on est en modif de etat
        {
         requete = "UPDATE " ETATS " SET "
                  " ET_Libelle_etat    = :ET_Libelle_etat,"
                  " ET_Couleur_etat    = :ET_Couleur_etat, "
                  " ET_Tache_terminee  = :ET_Tache_terminee "
                  " WHERE ET_Code_etat = '" + EtatEnCours + "'";

         query.prepare(requete);
         query.bindValue(":ET_Libelle_etat",    ui->lineEdit_NomEtat->text());
         query.bindValue(":ET_Couleur_etat",    ui->lineEdit_CouleurFondCode->text());
         query.bindValue(":ET_Tache_terminee",  ui->checkBox_Tache_Terminee->isChecked());
       }// fin else Modif

    if (!query.exec() )
       {QString zbid = query.lastQuery ();
        QString noerr;
        noerr.setNum(query.lastError().type());
        QMessageBox::warning ( this, tr("Status management"),
                         "<b>" + tr("Error when updating etats table: ") + ui->lineEdit_CodeEtat->text() +  "</b><br /><br />" +
                         tr("Error = (") +  noerr + ") " + query.lastError().text());
       } // fin if erreur exec insert

    ChargeEtat();
    ClearFormulaire();
}
//-------------------------------AnnuleEtat----------------------------------------------------
void DlgEtat::AnnuleEtat()
{
QString     requete;
QSqlQuery   query ( CApp::pCApp()->getDB()->database());

    requete = "DELETE FROM " ETATS " WHERE ET_Code_etat = '" + EtatEnCours + "'";
    query.prepare(requete);
    if (!query.exec() )
            {QString zbid = query.lastQuery ();
            QString noerr;
            noerr.setNum(query.lastError().type());
            QMessageBox::warning ( this, tr("Status management"),
                         "<b>" + tr("Error when updating etats table: ") + ui->lineEdit_CodeEtat->text() +  "</b><br /><br />" +
                         tr("Error = (") +  noerr + ") " + query.lastError().text());
            }
    ChargeEtat();
    ClearFormulaire();
}
//-------------------------------------------------------------------------------------
void DlgEtat::ClearFormulaire()
{
    ui->lineEdit_CodeEtat->clear();
    ui->lineEdit_NomEtat->clear();
    ui->lineEdit_CouleurFondCode->clear();
    ui->pushButton_CouleurFond->setStyleSheet("background-color: white;");

}
//-------------------------------ChargeEtat----------------------------------------------------
void DlgEtat::ChargeEtat()
{
    QString requete;
    QString zbid;

    ui->treeWidget_Etat->setColumnWidth(0,100);
    ui->treeWidget_Etat->setColumnWidth(1,150);
    QTreeWidgetItem *pItem = 0;
    ui->treeWidget_Etat->clear();
//
    requete = " SELECT   ET_Code_etat, ET_Libelle_etat, ET_Couleur_etat "   // 0-1-2
              " FROM    " ETATS
              " ORDER BY ET_Code_etat";

    QSqlQuery query(requete, CApp::pCApp()->getDB()->database());
    while (query.isActive() &&  query.next())
        { pItem = new QTreeWidgetItem() ;
        pItem->setText(0, query.value(0).toString());
        pItem->setText(1, query.value(1).toString());

        ui->treeWidget_Etat->addTopLevelItem(pItem);
        }
    ui->pushButton_annuler->setEnabled(false);
    ui->lineEdit_CodeEtat->setEnabled(true);
    EtatEnCours = "";
}

//-------------------------------ChoixCouleurFond----------------------------------------------------
void DlgEtat::ChoixCouleurFond()
{
    QColor color;
    color.setNamedColor(ui->lineEdit_CouleurFondCode->text());
    color = QColorDialog::getColor(color, this);
    if (color.isValid()) {
        ui->pushButton_CouleurFond->setStyleSheet("background-color:" + color.name()  + ";");
        ui->lineEdit_CouleurFondCode->setText(color.name());
        }
}


#include "dlgbox.h"
#include "ui_dlgbox.h"
#include <QtGui>
#include "CApp.h"
#include "C_BaseSynoptux.h"

#define BOX "synopt_box"

DlgBox::DlgBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgBox)
{
    ui->setupUi(this);

    connect (ui->pushButton_QuitterBox,   SIGNAL(clicked()),   this, SLOT(accept()));
    connect (ui->pushButton_ValidBox,     SIGNAL(clicked()),   this, SLOT(ValideBox()));
    connect (ui->pushButton_annuler,      SIGNAL(clicked()),   this, SLOT(AnnuleBox()));
    connect (ui->pushButton_CouleurFond,  SIGNAL(clicked()),    this, SLOT(ChoixCouleurFond()));
    connect (ui->pushButton_CouleurTitre, SIGNAL(clicked()),    this, SLOT(ChoixCouleurTitre()));
    connect (ui->treeWidget_Box,          SIGNAL(itemClicked(QTreeWidgetItem*,int)),   this, SLOT (selectBox( QTreeWidgetItem * , int )));

    ui->treeWidget_Box->setColumnWidth(0,50);
    ui->treeWidget_Box->setColumnWidth(1,150);
    ui->treeWidget_Box->setColumnWidth(2,50);

}

DlgBox::~DlgBox()
{
    delete ui;
}

void DlgBox::changeEvent(QEvent *e)
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
//-----------------------------------------selectBox----------------------------------------------------------------
void DlgBox::selectBox(QTreeWidgetItem * , int)
{
QTreeWidgetItem *item = ui->treeWidget_Box->currentItem();

BoxEnCours = item->text(0);

QString requete;

requete = "SELECT   BO_Code, BO_Libelle, BO_CouleurBG, BO_CouleurTitre, "   // 0-1-2-3
          " BO_Nb_Maxi_Pat, BO_Type "                                       // 4-5
          " FROM    " BOX
          " WHERE  BO_Code = '" + BoxEnCours + "'" ;

    QSqlQuery query(requete, CApp::pCApp()->getDB()->database());
    while (query.isActive() &&  query.next())
        {ui->lineEdit_CodeBox->setText( query.value(0).toString());
        ui->lineEdit_NomBox->setText(   query.value(1).toString());
        ui->lineEdit_CouleurFondCode->setText(query.value(2).toString());
        ui->pushButton_CouleurFond->setStyleSheet("background-color:" + query.value(2).toString()  + ";");
        ui->lineEdit_CouleurTitreCode->setText(query.value(3).toString());
        ui->pushButton_CouleurTitre->setStyleSheet("background-color:" + query.value(3).toString()  + ";");
        ui->lineEdit_NbreMaxPat->setText(query.value(4).toString());
        for (int ind=0; ind < ui->comboBox_type->count(); ind ++)
            {if (ui->comboBox_type->itemText(ind) == query.value(5).toString())
                ui->comboBox_type->setCurrentIndex(ind);
            }
        }
    ui->pushButton_annuler->setEnabled(true);
    ui->pushButton_ValidBox->setEnabled(true);
    ui->lineEdit_CodeBox->setEnabled(false);
}
//-------------------------------ValideBox----------------------------------------------------
void DlgBox::ValideBox()
{
QString     requete;
QSqlQuery   query ( CApp::pCApp()->getDB()->database() );

    if (BoxEnCours.length() == 0)            // on est en création ....
        {
        if (ui->lineEdit_CodeBox->text().length()== 0) return;

        requete = "INSERT INTO " BOX " ("
                  " BO_Code, BO_Libelle,   BO_CouleurBG, BO_CouleurTitre, "
                  " BO_Nb_Maxi_Pat,   BO_Type) "
                  " VALUES ("
                  " :BO_Code, :BO_Libelle,   :BO_CouleurBG, :BO_CouleurTitre, "
                  " :BO_Nb_Maxi_Pat,   :BO_Type ) ";

        query.prepare(requete);
        query.bindValue(":BO_Code",         ui->lineEdit_CodeBox->text());
        query.bindValue(":BO_Libelle",      ui->lineEdit_NomBox->text());
        query.bindValue(":BO_CouleurBG",    ui->lineEdit_CouleurFondCode->text());
        query.bindValue(":BO_CouleurTitre", ui->lineEdit_CouleurTitreCode->text());
        query.bindValue(":BO_Nb_Maxi_Pat",  ui->lineEdit_NbreMaxPat->text().toInt());
        query.bindValue(":BO_Type",         ui->comboBox_type->currentText());


        }
    else        // on est en modif de banque
        {
        requete = "UPDATE " BOX " SET "
                  " BO_Libelle      = :BO_Libelle,"
                  " BO_CouleurBG    = :BO_CouleurBG,"
                  " BO_CouleurTitre = :BO_CouleurTitre,"
                  " BO_Nb_Maxi_Pat  = :BO_Nb_Maxi_Pat, "
                  " BO_Type         = :BO_Type "
                  " WHERE BO_Code = '" + BoxEnCours + "'";

        query.prepare(requete);
        } // fin else Modif
    query.bindValue(":BO_Libelle",      ui->lineEdit_NomBox->text());
    query.bindValue(":BO_CouleurBG",    ui->lineEdit_CouleurFondCode->text());
    query.bindValue(":BO_CouleurTitre", ui->lineEdit_CouleurTitreCode->text());
    query.bindValue(":BO_Nb_Maxi_Pat",  ui->lineEdit_NbreMaxPat->text().toInt());
    query.bindValue(":BO_Type",         ui->comboBox_type->currentText());

    if (!query.exec() )
            {QString zbid = query.lastQuery ();
             QString noerr;
             noerr.setNum(query.lastError().type());
             QMessageBox::warning ( this, tr("Box management"),
                         "<b>" + tr("Error when updating box table: ") + ui->lineEdit_CodeBox->text() +  "</b><br /><br />" +
                         tr("Error = (") +  noerr + ") " + query.lastError().text());
            } // fin if erreur exec insert

    ChargeBox();
    ClearFormulaire();
}
//-------------------------------AnnuleBox----------------------------------------------------
void DlgBox::AnnuleBox()
{
QString     requete;
QSqlQuery   query ( CApp::pCApp()->getDB()->database() );

    requete = "DELETE FROM " BOX " WHERE BO_Code = '" + BoxEnCours + "'";
    query.prepare(requete);
    if (!query.exec() )
            {QString zbid = query.lastQuery ();
             QString noerr;
             noerr.setNum(query.lastError().type());
             QMessageBox::warning ( this, tr("Places management"),
                         "<b>" + tr("Error when updating places table ") + ui->lineEdit_CodeBox->text() +  "</b><br /><br />" +
                         tr("Error = (") +  noerr + ") " + query.lastError().text());
            }
    ChargeBox();
    ClearFormulaire();
}
//-------------------------------------------------------------------------------------
void DlgBox::ClearFormulaire()
{
    ui->lineEdit_CodeBox->clear();
    ui->lineEdit_NomBox->clear();
    ui->lineEdit_CouleurFondCode->clear();
    ui->pushButton_CouleurFond->setStyleSheet("background-color: white;");
    ui->lineEdit_CouleurTitreCode->clear();
    ui->pushButton_CouleurTitre->setStyleSheet("background-color: white;");
}
//-------------------------------ChargeBox----------------------------------------------------
void DlgBox::ChargeBox()
{
    QString requete;


    QTreeWidgetItem *pItem = 0;


    ui->treeWidget_Box->clear();
//
    requete = "SELECT   BO_Code, BO_Libelle, BO_CouleurBG, BO_CouleurTitre, "   // 0-1-2-3
              " BO_Nb_Maxi_Pat, BO_Type "                                       // 4-5
              " FROM    " BOX
              " ORDER BY BO_Code";

    QSqlQuery query(requete, CApp::pCApp()->getDB()->database());
    while (query.isActive() &&  query.next())
        { pItem = new QTreeWidgetItem() ;
        pItem->setText(0, query.value(0).toString());
        pItem->setText(1, query.value(1).toString());
        pItem->setText(2, query.value(5).toString());

        ui->treeWidget_Box->addTopLevelItem(pItem);
        }
    ui->pushButton_annuler->setEnabled(false);
    ui->lineEdit_CodeBox->setEnabled(true);
    BoxEnCours = "";
}
void DlgBox::ChoixCouleurTitre()
{
    QColor color;
    color.setNamedColor(ui->lineEdit_CouleurTitreCode->text());
    color = QColorDialog::getColor(color, this);
    if (color.isValid()) {
        ui->pushButton_CouleurTitre->setStyleSheet("background-color:" + color.name() + ";");
        ui->lineEdit_CouleurTitreCode->setText(color.name());
        }
}
void DlgBox::ChoixCouleurFond()
{
    QColor color;
    color.setNamedColor(ui->lineEdit_CouleurFondCode->text());
    color = QColorDialog::getColor(color, this);
    if (color.isValid()) {
        ui->pushButton_CouleurFond->setStyleSheet("background-color:" + color.name()  + ";");
        ui->lineEdit_CouleurFondCode->setText(color.name());
        }
}


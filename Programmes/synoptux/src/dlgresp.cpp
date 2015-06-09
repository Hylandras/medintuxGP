#include "dlgresp.h"
#include "ui_dlgresp.h"
#include <QtGui>
#include "CApp.h"
#include "C_BaseSynoptux.h"

#define DATA_BASE_SYNOPTUX    CApp::pCApp()->getDB()->database()
#define BASE_SYNOPTUX         CApp::pCApp()->getDB()

#define RESPONSABLES "responsables"
//---------------------------------------- DlgResp ---------------------------------------------------
DlgResp::DlgResp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgResp)
{
    ui->setupUi(this);
    connect (ui->pushButton_QuitterResp,   SIGNAL(clicked()),                           this, SLOT(accept()));
    connect (ui->pushButton_ValidResp,     SIGNAL(clicked()),                           this, SLOT(ValideResp()));
    connect (ui->pushButton_annuler,       SIGNAL(clicked()),                           this, SLOT(AnnuleResp()));
    connect (ui->pushButton_CouleurFond,   SIGNAL(clicked()),                           this, SLOT(ChoixCouleurFond()));
    connect (ui->treeWidget_Resp,          SIGNAL(itemClicked(QTreeWidgetItem*,int)),   this, SLOT (selectResp( QTreeWidgetItem * , int )));
    ui->pushButton_annuler->hide();
}

//---------------------------------------- ~DlgResp ---------------------------------------------------
DlgResp::~DlgResp()
{
    delete ui;
}

//---------------------------------------- changeEvent ---------------------------------------------------
void DlgResp::changeEvent(QEvent *e)
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
//-----------------------------------------selectResp----------------------------------------------------------------
void DlgResp::selectResp(QTreeWidgetItem * , int)
{
QTreeWidgetItem *item = ui->treeWidget_Resp->currentItem();

RespEnCours = item->text(0);

QString requete;
requete = QString("SELECT   %1, %2, %3 "   // 0 RE_Code -1 RE_Nom -2 RE_Couleur
                  " FROM    %4"
                  " WHERE   %1 = '%5'").arg(BASE_SYNOPTUX->m_SIGNER_CODE,
                                            BASE_SYNOPTUX->m_SIGNER_NOM,
                                            BASE_SYNOPTUX->m_SIGNER_COLOR,
                                            BASE_SYNOPTUX->m_SIGNER_TBL_NAME,
                                            RespEnCours) ;
    QSqlQuery query(requete, DATA_BASE_SYNOPTUX);
    while (query.isActive() &&  query.next())
        {ui->lineEdit_CodeResp->setText( query.value(0).toString());
        ui->lineEdit_NomResp->setText(   query.value(1).toString());
        ui->lineEdit_CouleurFondCode->setText(query.value(2).toString());
        ui->pushButton_CouleurFond->setStyleSheet("background-color:" + query.value(2).toString()  + ";");
        }
    ui->pushButton_annuler->setEnabled(true);
    ui->pushButton_ValidResp->setEnabled(true);
    ui->lineEdit_CodeResp->setEnabled(false);
}
//-------------------------------ValideResp----------------------------------------------------
void DlgResp::ValideResp()
{
    QString     requete;
    QSqlQuery   query ( DATA_BASE_SYNOPTUX );

    if (RespEnCours.length() == 0)            // on est en création ....
       {
        if (ui->lineEdit_CodeResp->text().length()== 0) return;
        requete = QString ("INSERT INTO %1 ("
                           " %2, %3, %4) "
                           " VALUES ("
                           " :%2, :%3,   :%4 ) ").arg(BASE_SYNOPTUX->m_SIGNER_TBL_NAME,
                                                      BASE_SYNOPTUX->m_SIGNER_CODE,
                                                      BASE_SYNOPTUX->m_SIGNER_NOM,
                                                      BASE_SYNOPTUX->m_SIGNER_COLOR);

        query.prepare(requete);
        query.bindValue(QString(":%1").arg(BASE_SYNOPTUX->m_SIGNER_CODE),      ui->lineEdit_CodeResp->text());
        query.bindValue(QString(":%1").arg(BASE_SYNOPTUX->m_SIGNER_NOM),       ui->lineEdit_NomResp->text());
        query.bindValue(QString(":%1").arg(BASE_SYNOPTUX->m_SIGNER_COLOR),     ui->lineEdit_CouleurFondCode->text());
       }
    else        // on est en modif de resp
       {
        requete = QString("UPDATE %1 SET "
                          " %2    = :%2,"
                          " %3    = :%3 "
                          " WHERE %4 = '%5'").arg(BASE_SYNOPTUX->m_SIGNER_TBL_NAME,  // %1
                                                  BASE_SYNOPTUX->m_SIGNER_NOM,       // %2
                                                  BASE_SYNOPTUX->m_SIGNER_COLOR,     // %3
                                                  BASE_SYNOPTUX->m_SIGNER_CODE,      // %4
                                                  RespEnCours);                      // %5

        query.prepare(requete);
        query.bindValue(QString(":%1").arg(BASE_SYNOPTUX->m_SIGNER_NOM),       ui->lineEdit_NomResp->text());
        query.bindValue(QString(":%1").arg(BASE_SYNOPTUX->m_SIGNER_COLOR),     ui->lineEdit_CouleurFondCode->text());
       } // fin else Modif

    if (!query.exec() )
       {QString zbid = query.lastQuery ();
        QString noerr;
        noerr.setNum(query.lastError().type());
        QMessageBox::warning ( this, tr("Persons in charge management"),
                         "<b>" + tr("Error when updating responsables table: ") + ui->lineEdit_CodeResp->text() +  "</b><br /><br />" +
                         tr("Error = (") +  noerr + ") " + query.lastError().text());
       } // fin if erreur exec insert

    ChargeResp();
    ClearFormulaire();
}
//-------------------------------AnnuleResp----------------------------------------------------
void DlgResp::AnnuleResp()
{
QString     requete;
QSqlQuery   query ( DATA_BASE_SYNOPTUX );

    requete = QString("DELETE FROM %1 WHERE %2 = '%3'").arg(BASE_SYNOPTUX->m_SIGNER_TBL_NAME,
                                                            BASE_SYNOPTUX->m_SIGNER_CODE,
                                                            RespEnCours);
    query.prepare(requete);
    if (!query.exec() )
       {QString zbid = query.lastQuery ();
        QString noerr;
        noerr.setNum(query.lastError().type());
        QMessageBox::warning ( this, tr("Persons in charge management"),
                     "<b>" + tr("Error when updating responsables table: ") + ui->lineEdit_CodeResp->text() +  "</b><br /><br />" +
                     tr("Error = (") +  noerr + ") " + query.lastError().text());
       }
    ChargeResp();
    ClearFormulaire();
}
//-------------------------------------------------------------------------------------
void DlgResp::ClearFormulaire()
{
    ui->lineEdit_CodeResp->clear();
    ui->lineEdit_NomResp->clear();
    ui->lineEdit_CouleurFondCode->clear();
    ui->pushButton_CouleurFond->setStyleSheet("background-color: white;");

}
//-------------------------------ChargeResp----------------------------------------------------
void DlgResp::ChargeResp()
{
    QString requete;
    QString zbid;

    ui->treeWidget_Resp->setColumnWidth(0,100);
    ui->treeWidget_Resp->setColumnWidth(1,150);
    QTreeWidgetItem *pItem = 0;
    ui->treeWidget_Resp->clear();
//
    requete = QString("SELECT    %1, %2, %3 "   // 0 Re_Code -1 RE_Nom -2 RE_Couleur
                      " FROM     %4 WHERE %5 LIKE '%6' AND %5 LIKE '%7'"
                      " ORDER BY %2").arg(BASE_SYNOPTUX->m_SIGNER_CODE,
                                          BASE_SYNOPTUX->m_SIGNER_NOM,
                                          BASE_SYNOPTUX->m_SIGNER_COLOR,
                                          BASE_SYNOPTUX->m_SIGNER_TBL_NAME,BASE_SYNOPTUX->m_SIGNER_DROITS,"%med%","%sgn%" );
    QSqlQuery query(requete, DATA_BASE_SYNOPTUX );
    while (query.isActive() &&  query.next())
        { pItem = new QTreeWidgetItem() ;
          pItem->setText(0, query.value(0).toString());
          pItem->setText(1, query.value(1).toString());

          ui->treeWidget_Resp->addTopLevelItem(pItem);
        }
    ui->pushButton_annuler->setEnabled(false);
    ui->lineEdit_CodeResp->setEnabled(true);
    RespEnCours = "";
}

//-------------------------------ChoixCouleurFond----------------------------------------------------
void DlgResp::ChoixCouleurFond()
{
    QColor color;
    color.setNamedColor(ui->lineEdit_CouleurFondCode->text());
    color = QColorDialog::getColor(color, this);
    if (color.isValid()) {
        ui->pushButton_CouleurFond->setStyleSheet("background-color:" + color.name()  + ";");
        ui->lineEdit_CouleurFondCode->setText(color.name());
        }
}


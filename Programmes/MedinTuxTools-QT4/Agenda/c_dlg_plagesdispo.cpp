#include "c_dlg_plagesdispo.h"
#include "ui_c_dlg_plagesdispo.h"

C_Dlg_PlagesDispo::C_Dlg_PlagesDispo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::C_Dlg_PlagesDispo)
{
    ui->setupUi(this);

    ui->TreeWidget_PlagesDispo->setColumnWidth(0,120);
    ui->TreeWidget_PlagesDispo->setColumnWidth(1,75);
    ui->TreeWidget_PlagesDispo->setColumnWidth(2,75);
    ui->TreeWidget_PlagesDispo->setColumnWidth(3,75);
    ui->TreeWidget_PlagesDispo->setColumnWidth(4,70);
    ui->TreeWidget_PlagesDispo->setColumnHidden(5,true);

    connect( ui->TreeWidget_PlagesDispo,  SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int  )), this, SLOT(Slot_TreeWidgetPlages_DoubleClicked( QTreeWidgetItem * , int)) );

    m_sDatePlage  = "";
    m_sHeurePlage = "";
}

C_Dlg_PlagesDispo::~C_Dlg_PlagesDispo()
{
    delete ui;
}

void C_Dlg_PlagesDispo::Slot_TreeWidgetPlages_DoubleClicked( QTreeWidgetItem * qTreeWidgetItemPlage, int col)
{
    m_sDatePlage  = qTreeWidgetItemPlage->text(5);
    m_sHeurePlage = qTreeWidgetItemPlage->text(1);
    m_Col = col;
    accept();
}

void C_Dlg_PlagesDispo::changeEvent(QEvent *e)
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

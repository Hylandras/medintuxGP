#include "dlgcss.h"
#include "ui/ui_dlgcss.h"

DlgCSS::DlgCSS(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgCSS)
{
    ui->setupUi(this);
}

DlgCSS::~DlgCSS()
{
    delete ui;
}

void DlgCSS::changeEvent(QEvent *e)
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

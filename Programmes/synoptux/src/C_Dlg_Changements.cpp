#include "C_Dlg_Changements.h"
#include "ui_C_Dlg_Changements.h"

C_Dlg_Changements::C_Dlg_Changements(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::C_Dlg_Changements)
{
    ui->setupUi(this);
}

C_Dlg_Changements::~C_Dlg_Changements()
{
    delete ui;
}
QString C_Dlg_Changements::getText()
{
 return ui->textEdit_Changements->toHtml();
}

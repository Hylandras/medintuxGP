#include "C_Wdg_Box.h"

C_Wdg_Box::C_Wdg_Box(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::C_Wdg_Box)
{    m_UniqueCode = "";
    //m_ButtonNomMedResponsable = 0;
    //m_horizonLayoutResp       = 0;
    ui->setupUi(this);
}

C_Wdg_Box::~C_Wdg_Box()
{   emit Sign_byebyeBox( this );
    delete ui;
}

void C_Wdg_Box::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

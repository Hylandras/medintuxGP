#include "C_PopupDial_Info.h"
#include "ui_C_PopupDial_Info.h"

//----------------------------------- C_PopupDial_Info -------------------------------------------
C_PopupDial_Info::C_PopupDial_Info(                        QWidget       *parent,
                                                           const QString &text,
                                                           const QColor  &backGround,
                                                           const QRect   &initialRect,
                                                           const QRect   &deployRect) :
   C_PopupDial(parent, backGround, initialRect, deployRect)
{   setupUi(this);
    setFont(parent->font());
    QRect dRect = QRect (deployRect.x(),
                         deployRect.y(),
                         width(),
                         deployRect.height());
    webView_info->setStyleSheet( QString("background-color: %1;").arg(backGround.name()));
    setText(text);
    setRectanngles(initialRect, dRect);
    setToInitialRect(C_PopupDial::FAST);
    connect( this, SIGNAL( Sign_deployed (int&)),     this , SLOT( Slot_Dial_InfoDeployed(int&) ));


}
//------------------------------------ setText --------------------------------------------------------
void C_PopupDial_Info::setText( const QString &text )
{ webView_info->setHtml(text);
}

//------------------------------------ setFont --------------------------------------------------------
void C_PopupDial_Info::setFont(const QFont &fnt)
{
 C_PopupDial::setFont(fnt);
 m_fGrd  =  fnt;
 /*
 label_poids->setFont(m_fGrd);
 lineEdit_poids->setFont(m_fGrd);
 pushButton_cancel->setFont(m_fGrd);
 label_kg_x->setFont(m_fGrd);
 pushButton_UPA->setFont(m_fGrd);
 pushButton_UFC->setFont(m_fGrd);
 label_QPA->setFont(m_fGrd);
 label_QFC->setFont(m_fGrd);
 label_QPA_by_UP_0->setFont(m_fGrd);
 label_QPA_by_UP_1->setFont(m_fGrd);
 lineEdit_QPA_by_UP->setFont(m_fGrd);
 */
}
//----------------------------------- ~C_PopupDial_Info -------------------------------------------
C_PopupDial_Info::~C_PopupDial_Info()
{
}
//------------------------------------------------------ Slot_Dial_InfoDeployed ----------------------------------------------------------------------
void C_PopupDial_Info::Slot_Dial_InfoDeployed(int &ok)
{   ok=1;
 /*
 QPoint pos = mapFromGlobal (QCursor::pos());
 if (pos.x() < m_initialRect.width() - 30)      // si sur la zone edition ne pas deployer
    { ok=0;
     return;
    }

 ok = 1;
 */
}

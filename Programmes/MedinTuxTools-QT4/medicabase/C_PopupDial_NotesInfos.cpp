#include "C_PopupDial_NotesInfos.h"
#include "ui_C_PopupDial_NotesInfos.h"

//----------------------------------- C_PopupDial_NotesInfos -------------------------------------------
C_PopupDial_NotesInfos::C_PopupDial_NotesInfos( QWidget   *parent,
                                                const QString   &title,
                                                const QString   &author,
                                                const QDateTime &dateTime,
                                                const QColor    &backGround,
                                                const QRect     &initialRect,
                                                const QRect     &deployRect
                                           ) :
    C_PopupDial(parent, backGround, initialRect, deployRect)
{   m_Title    = title;
    m_Author   = author;
    m_DateTime = dateTime;
    setupUi(this);
    lineEdit_Titre->setText(m_Title);
    lineEdit_Author->setText(m_Author);
    dateTimeEdit_note->setDate(m_DateTime.date());
    dateTimeEdit_note->setTime(m_DateTime.time());
    setFont(parent->font());

    QRect dRect = QRect (initialRect.x(),
                         initialRect.y(),
                         width(),
                         height());

    setRectanngles(initialRect, dRect);
    setToInitialRect(C_PopupDial::FAST);
    connect( this, SIGNAL( Sign_deployed (int&)),     this , SLOT( Slot_PopupDial_NotesInfosDeployed(int&) ));
}


//------------------------------------------------------ Slot_DialDateDeployed ----------------------------------------------------------------------
void C_PopupDial_NotesInfos::Slot_PopupDial_NotesInfosDeployed(int &ok)
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
//------------------------------------ setFont --------------------------------------------------------
void C_PopupDial_NotesInfos::setFont(const QFont &fnt)
{
 C_PopupDial::setFont(fnt);
 m_fGrd  =  fnt;

 dateTimeEdit_note->setFont(m_fGrd);
 lineEdit_Titre->setFont(m_fGrd);
 lineEdit_Author->setFont(m_fGrd);
 //buttonBox_sortie->setFont(m_fGrd);
 label_Property->setFont(m_fGrd);
 label_Title->setFont(m_fGrd);
 label_Author->setFont(m_fGrd);
 label_DateTime->setFont(m_fGrd);
 //buttonBox_sortie->setFont(m_fGrd);
}
//----------------------------------- ~C_PopupDial_NotesInfos -------------------------------------------
C_PopupDial_NotesInfos::~C_PopupDial_NotesInfos()
{
}


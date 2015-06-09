#include "C_Dlg_NotesInfos.h"
#include <QDebug>
#include "../../MedinTuxTools-QT4/Theme/Theme.h"

//------------------------------------ C_Dlg_NotesInfos --------------------------------------------------------
C_Dlg_NotesInfos::C_Dlg_NotesInfos( QWidget         *parent,
                                    const QString   &title,
                                    const QString   &author,
                                    const QDateTime &dateTime,
                                    const QString   &text
                                  )
: QDialog(parent)
{   m_Title    = title;
    m_Author   = author;
    m_DateTime = dateTime;
    m_Texte    = text;
    setupUi(this);
    lineEdit_Titre->setText(m_Title);
    lineEdit_Author->setText(m_Author);
    textEdit_Note->setText(m_Texte);
    dateTimeEdit_note->setDate(m_DateTime.date());
    dateTimeEdit_note->setTime(m_DateTime.time());
    setFont(parent->font());
    label_icone->setPixmap(Theme::getIcon("medicatux/NewNotePicto.png"));
    connect( pushButton_annuler,  SIGNAL(clicked ( bool )),         this,   SLOT(Slot_pushButton_pushButton_annuler (bool)) );
    connect( pushButton_ok,       SIGNAL(clicked ( bool )),         this,   SLOT(Slot_pushButton_ok(bool)) );
}
//------------------------------------ Slot_pushButton_pushButton_annuler --------------------------------------------------------
void C_Dlg_NotesInfos::Slot_pushButton_pushButton_annuler (bool)
{   done ( 0 );
}
//------------------------------------ Slot_pushButton_ok --------------------------------------------------------
void C_Dlg_NotesInfos::Slot_pushButton_ok(bool)
{   m_Title    = lineEdit_Titre->text();
    m_Author   = lineEdit_Author->text();
    m_Texte    = textEdit_Note->toHtml();
    m_DateTime = dateTimeEdit_note->dateTime();
    done ( 1 );
}

//------------------------------------ setFont --------------------------------------------------------
void C_Dlg_NotesInfos::setFont(const QFont &fnt)
{
 m_fGrd  =  fnt;
 dateTimeEdit_note->setFont(m_fGrd);
 lineEdit_Titre->setFont(m_fGrd);
 lineEdit_Author->setFont(m_fGrd);
 label_Property->setFont(m_fGrd);
 label_Title->setFont(m_fGrd);
 label_Author->setFont(m_fGrd);
 label_DateTime->setFont(m_fGrd);
 pushButton_annuler->setFont(m_fGrd);
 pushButton_ok->setFont(m_fGrd);
 textEdit_Note->setFont(m_fGrd);
}
//----------------------------------- ~C_Dlg_NotesInfos -------------------------------------------
C_Dlg_NotesInfos::~C_Dlg_NotesInfos()
{
}

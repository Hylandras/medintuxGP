#ifndef C_DLG_NOTESINFOS_H
#define C_DLG_NOTESINFOS_H

#include <QObject>
#include <QDialog>
#include "ui_C_Dlg_NotesInfos.h"
namespace Ui {
               class C_Dlg_NotesInfos;
             }
//========================== C_DlgNotesInfos ====================================
class C_Dlg_NotesInfos : public QDialog, public Ui::C_Dlg_NotesInfos
{
    Q_OBJECT

public:
    explicit C_Dlg_NotesInfos(           QWidget         *parent,
                                         const QString   &title,
                                         const QString   &author,
                                         const QDateTime &dateTime,
                                         const QString   &text
                             );
    ~C_Dlg_NotesInfos();
    QString title()       {return m_Title;}
    QString author()      {return m_Author;}
    QString   texte()     {return m_Texte;}
    QDateTime date_time() {return m_DateTime;}

protected:
    void setFont(const QFont &fnt);

private:
    QString    m_Title;
    QString    m_Author;
    QDateTime  m_DateTime;
    QString    m_Texte;
    QFont      m_fGrd;

private slots:
void Slot_pushButton_pushButton_annuler (bool);
void Slot_pushButton_ok (bool);
};
#endif // C_DLG_NOTESINFOS_H

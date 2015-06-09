#ifndef C_DLGNOTESINFOS_H
#define C_DLGNOTESINFOS_H

#include <QObject>
#include <QWidget>
#include "../../MedinTuxTools-QT4/C_PopupDial/C_PopupDial.h"
#include "ui_C_PopupDial_NotesInfos.h"
namespace Ui {
               class C_PopupDial_NotesInfos;
             }
//========================== C_DlgNotesInfos ====================================
class C_PopupDial_NotesInfos : public C_PopupDial, public Ui::C_PopupDial_NotesInfos
{
    Q_OBJECT

public:
    explicit C_PopupDial_NotesInfos(     QWidget         *parent,
                                         const QString   &title,
                                         const QString   &author,
                                         const QDateTime &pateTime,
                                         const QColor    &backGround,
                                         const QRect     &initialRect,
                                         const QRect     &deployRect);
    ~C_PopupDial_NotesInfos();
protected:
    void setFont(const QFont &fnt);
private:
    QString    m_Title;
    QString    m_Author;
    QDateTime  m_DateTime;
    QFont      m_fGrd;
private slots:
   void Slot_PopupDial_NotesInfosDeployed(int &ok);
};


#endif // C_DLGNOTESINFOS_H

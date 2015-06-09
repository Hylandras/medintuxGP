#ifndef C_POPUPDIAL_INFOCHOICE_H
#define C_POPUPDIAL_INFOCHOICE_H
namespace Ui {
               class C_PopupDial_Info;
             }
#include <QObject>
#include <QWidget>
#include "../../MedinTuxTools-QT4/C_PopupDial/C_PopupDial.h"
#include "../../MedinTuxTools-QT4/medicabase/C_Frm_Prescription.h"
#include "ui_C_PopupDial_Info.h"
//========================== C_PopupDial_Info ====================================
class C_PopupDial_Info : public C_PopupDial, public Ui::C_PopupDial_Info
{
    Q_OBJECT
public:
    explicit C_PopupDial_Info(            QWidget       *parent,
                                          const QString &text,
                                          const QColor  &backGround,
                                          const QRect   &initialRect,
                                          const QRect   &deployRect);
    ~C_PopupDial_Info();

    void setText(const QString &text);
protected:
    void setFont(const QFont &fnt);

private:
    QFont                   m_fGrd;

signals:

public slots:
    void Slot_Dial_InfoDeployed(int &ok);

};

#endif // C_POPUPDIAL_INFOCHOICE_H


#ifndef C_POPUPDIAL_INDICATIONCHOICE_H
#define C_POPUPDIAL_INDICATIONCHOICE_H
namespace Ui {
               class C_PopupDial_IndicationChoice;
             }
#include <QObject>
#include <QWidget>
#include "../../MedinTuxTools-QT4/C_PopupDial/C_PopupDial.h"
#include "../../MedinTuxTools-QT4/medicabase/C_Frm_Prescription.h"
#include "ui_C_PopupDial_IndicationChoice.h"
//========================== C_PopupDial_IndicationChoice ====================================
class C_PopupDial_IndicationChoice : public C_PopupDial, public Ui::C_PopupDial_IndicationChoice
{
    Q_OBJECT
public:
    explicit C_PopupDial_IndicationChoice(C_Frm_PosologieHitList *p_C_Frm_PosologieHitList,
                                          CMyQLineEdit           *pCMyQLineEdit,
                                          const QColor            &backGround,
                                          const QRect             &initialRect,
                                          const QRect             &deployRect);
    ~C_PopupDial_IndicationChoice();
protected:
    void setFont(const QFont &fnt);
    void fillTreeWidgetIndications(const QString &cip);
private:
    QFont                   m_fGrd;
    // QString                 m_cur_grammar;
    C_Frm_PosologieHitList *m_pC_Frm_PosologieHitList;
    CMyQLineEdit           *m_pCMyQLineEdit;
    //QColor                  m_backGround;
signals:

public slots:
    void Slot_textChanged();
    void Slot_Dial_IndicationChoiceDeployed(int &ok);
    void Slot_treeWidget_Indications_itemClicked ( QTreeWidgetItem * , int   );
    void Slot_treeWidget_Indications_itemDoubleClicked ( QTreeWidgetItem * , int   );

};

#endif // C_POPUPDIAL_INDICATIONCHOICE_H


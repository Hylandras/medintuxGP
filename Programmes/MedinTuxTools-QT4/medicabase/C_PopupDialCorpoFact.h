#ifndef C_POPUPDIALCORPOFACT_H
#define C_POPUPDIALCORPOFACT_H

#include <QWidget>
#include "../../MedinTuxTools-QT4/C_PopupDial/C_PopupDial.h"
#include "../../MedinTuxTools-QT4/medicabase/C_Frm_Prescription.h"
#include "ui_C_PopupDialCorpoFact.h"
namespace Ui {
               class C_PopupDialCorpoFact;
             }
class ThemePopup;
//========================== C_PopupDialCorpoFact ====================================
class C_PopupDialCorpoFact : public C_PopupDial, public Ui::C_PopupDialCorpoFact
{
    Q_OBJECT

public:
    explicit C_PopupDialCorpoFact(C_Frm_PosologieHitList  *pC_Frm_PosologieHitList,
                                  const QColor    &backGround,
                                  const QRect     &initialRect,
                                  const QRect     &deployRect);
    ~C_PopupDialCorpoFact();
     QString interfaceToGrammar( QString &grammar );
     void    grammarToInterface(const QString &grammar, int mode=-1);
     void    setMode(int mode);
     void    setLabelFactCorpo();
     void    disconnectControls();
     void    connectControls();
     QString        get_QuPA_by_FC();        // 10
     QString        get_QuPA_by_FC_Unit();   // mg
     QString        get_QuFC();              // 3
     QString        get_QuFC_Unit();         // Kg           --> 10 mg par 3 kg de poids
     QString        get_QuPA_by_UP();        // 500 mg par comprime
     C_PatientCtx * patientContexte(){return m_C_PatientCtx;}

protected:
    void setFont(QFont fnt);
private:
   C_Frm_PosologieHitList *m_pC_Frm_PosologieHitList;
   C_PatientCtx           *m_C_PatientCtx;
   QFont                   m_fGrd;
   QString                 m_cur_grammar;
   bool                    m_IsPopupRun;
   ThemePopup             *m_add_Menu;
   ThemePopup             *m_poids_Menu;
   QString                 m_unitForme;
   QString                 m_unitPoso;
   QString                 m_qPA_min_By_FactCorp;
   QString                 m_qPA_max_By_FactCorp;
   QString                 m_facteurCorpo;
   QString                 m_unite_facteurCorpo;
   QString                 m_qPA_ByUnitPrise;
private slots:
   void Slot_pushButton_UFC_clicked ( bool );
   void Slot_pushButton_mode_clicked( bool );
   void Slot_DialCorpoFactDeployed(int &ok);
   void Slot_Dial_CorpoFactMinimized(int &ok);
   void Slot_contentFactCorpoChanged(const QString &txt);
   void Slot_contentChanged(const QString &txt);
   void Slot_PatientCtxChanged(C_PatientCtx *pC_PatientCtx);
   void Slot_checkBox_secable_stateChanged ( int );
signals:
   void Sign_FactCorpoChanged( C_PatientCtx *p );
   void Sign_contentChanged(   C_PopupDialCorpoFact*);
};

#endif // C_POPUPDIALCORPOFACT_H

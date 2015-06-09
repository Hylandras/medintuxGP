#ifndef C_DLG_LOGIN_H
#define C_DLG_LOGIN_H

#include <QDialog>
namespace Ui {
    class C_Dlg_Login;
}

class QTimer;
//===========================================  C_Dlg_Login ===================================
class C_Dlg_Login : public QDialog
{
    Q_OBJECT

public:

    enum Visibility  {Show          = 0,
                      OnEdit        = 1,
                      Hide          = 2,
                      Cached        = 3,  // 2+3
                      LoginReadOnly = 4
                     };
    enum ConfirmMode  {CloseOnConfirm = 0,
                       StayOnConfirm  = 1,
                      };

    explicit C_Dlg_Login(const QString &login, const QString &password, QWidget *parent = 0 ,
                         C_Dlg_Login::Visibility    loginState   =  C_Dlg_Login::Show,
                         C_Dlg_Login::Visibility    passState    =  C_Dlg_Login::Cached,
                         C_Dlg_Login::ConfirmMode   confirmMode  =  C_Dlg_Login::CloseOnConfirm
                         );
    ~C_Dlg_Login();

    //---------------------------------------- get_Login -----------------------------------------
    QString  get_Login();
    //---------------------------------------- get_Password -----------------------------------------
    QString  get_Password();
    //---------------------------------------- get_CriptedLogin -----------------------------------------
    QString  get_CriptedLogin(const QString &prefix = "");
    //---------------------------------------- get_CriptedPassword -----------------------------------------
    QString  get_CriptedPassword(const QString &prefix = "");
    //---------------------------------------- IsPasswordMustBeRecord -----------------------------------------
    bool IsPasswordMustBeRecord();
    //------------------------------------------------------ setFocusOnPass -------------------------------------------------------------------
    void setFocusOnPass();
    //------------------------------------------------------ setDefaultOnOk -------------------------------------------------------------------
    void setDefaultOnOk();

protected:
    //---------------------------------------- eventFilter -----------------------------------------
# if defined(Q_OS_MAC)
    bool eventFilter(QObject *obj, QEvent *ev);
    bool                      m_CapsLock;
#endif
    void setIconCapsSate();
private:
    //---------------------------------------- set_LoginOnState -----------------------------------------
    void set_LoginOnState     (C_Dlg_Login::Visibility state);
    //---------------------------------------- set_PasswordOnState -----------------------------------------
    void set_PasswordOnState  (C_Dlg_Login::Visibility state);
    //---------------------------------------- getModeEditState -----------------------------------------
    void getModeEditState ();
    //---------------------------------------- nextModeEditStateLogin -----------------------------------------
    C_Dlg_Login::Visibility nextModeEditStateLogin (C_Dlg_Login::Visibility state);
    //---------------------------------------- nextModeEditStatePassword -----------------------------------------
    C_Dlg_Login::Visibility nextModeEditStatePassword (C_Dlg_Login::Visibility state);
private slots:
    //---------------------------------------- Slot_pushButton_LoginHideShow -----------------------------------------
    void Slot_pushButton_LoginHideShow  (bool);
    //---------------------------------------- Slot_pushButton_PasswordHideShow -----------------------------------------
    void Slot_pushButton_PasswordHideShow  (bool);
    //---------------------------------------- Slot_pushButton_LoginOk -----------------------------------------
    void Slot_pushButton_LoginOk  (bool);
    //---------------------------------------- Slot_pushButton_LoginCancel -----------------------------------------
    void Slot_pushButton_LoginCancel  (bool);
    //---------------------------------------- Slot_lineEdit_Login_cursorPositionChanged -----------------------------------------
    void Slot_lineEdit_Login_cursorPositionChanged (int , int);
    //---------------------------------------- Slot_lineEdit_Password_cursorPositionChanged -----------------------------------------
    void Slot_lineEdit_Password_cursorPositionChanged (int , int);
    //---------------------------------------- Slot_TimerIsTimeOut -----------------------------------------
    void Slot_TimerIsTimeOut();

private:
    QString                   m_nonCriptedLogin;
    QString                   m_nonCriptedPassword;
    C_Dlg_Login::Visibility   m_stateLoginEyes;
    C_Dlg_Login::Visibility   m_statePasswordEyes;
    QTimer                   *m_pQTimer;
    int                       m_TimerPasswordCount;
    int                       m_TimerLoginCount;
    int                       m_TimerMajCount;
    int                       m_maskPassState;          // si sur on le mode visible permanent est interdit
    int                       m_maskLoginState;
    C_Dlg_Login::ConfirmMode  m_confirmMode;
    Ui::C_Dlg_Login          *m_pGUI;
signals:
    void Sign_confirm(bool);
};

#endif // C_DLG_LOGIN_H

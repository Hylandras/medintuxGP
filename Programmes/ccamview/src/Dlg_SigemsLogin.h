#ifndef DLG_SIGEMSLOGIN_H
#define DLG_SIGEMSLOGIN_H

#include <qvariant.h>


#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Dlg_SigemsLogin
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButtonOK;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *textLabel1_2;
    QLineEdit *lineEdit_Login;
    QHBoxLayout *hboxLayout1;
    QLabel *textLabel1;
    QLineEdit *lineEdit_Password;

    void setupUi(QDialog *Dlg_SigemsLogin)
    {
        if (Dlg_SigemsLogin->objectName().isEmpty())
            Dlg_SigemsLogin->setObjectName(QString::fromUtf8("Dlg_SigemsLogin"));
        Dlg_SigemsLogin->resize(464, 80);
        gridLayout = new QGridLayout(Dlg_SigemsLogin);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushButtonOK = new QPushButton(Dlg_SigemsLogin);
        pushButtonOK->setObjectName(QString::fromUtf8("pushButtonOK"));

        gridLayout->addWidget(pushButtonOK, 0, 1, 1, 1);

        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        textLabel1_2 = new QLabel(Dlg_SigemsLogin);
        textLabel1_2->setObjectName(QString::fromUtf8("textLabel1_2"));
        textLabel1_2->setWordWrap(false);

        hboxLayout->addWidget(textLabel1_2);

        lineEdit_Login = new QLineEdit(Dlg_SigemsLogin);
        lineEdit_Login->setObjectName(QString::fromUtf8("lineEdit_Login"));

        hboxLayout->addWidget(lineEdit_Login);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        textLabel1 = new QLabel(Dlg_SigemsLogin);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        textLabel1->setFrameShape(QFrame::NoFrame);
        textLabel1->setFrameShadow(QFrame::Plain);
        textLabel1->setWordWrap(false);

        hboxLayout1->addWidget(textLabel1);

        lineEdit_Password = new QLineEdit(Dlg_SigemsLogin);
        lineEdit_Password->setObjectName(QString::fromUtf8("lineEdit_Password"));
        lineEdit_Password->setEchoMode(QLineEdit::Password);

        hboxLayout1->addWidget(lineEdit_Password);


        vboxLayout->addLayout(hboxLayout1);


        gridLayout->addLayout(vboxLayout, 0, 0, 1, 1);

#ifndef QT_NO_SHORTCUT
        textLabel1_2->setBuddy(lineEdit_Login);
        textLabel1->setBuddy(lineEdit_Password);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(lineEdit_Login, lineEdit_Password);
        QWidget::setTabOrder(lineEdit_Password, pushButtonOK);

        retranslateUi(Dlg_SigemsLogin);

        QMetaObject::connectSlotsByName(Dlg_SigemsLogin);
    } // setupUi

    void retranslateUi(QDialog *Dlg_SigemsLogin)
    {
        Dlg_SigemsLogin->setWindowTitle(QApplication::translate("Dlg_SigemsLogin", "Param\303\250tres de connexion Sigems", 0, QApplication::UnicodeUTF8));
        pushButtonOK->setText(QApplication::translate("Dlg_SigemsLogin", "&Valider", 0, QApplication::UnicodeUTF8));
        pushButtonOK->setShortcut(QApplication::translate("Dlg_SigemsLogin", "Alt+V", 0, QApplication::UnicodeUTF8));
        textLabel1_2->setText(QApplication::translate("Dlg_SigemsLogin", "&Identifiant :", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("Dlg_SigemsLogin", "&Mot de Passe :", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Dlg_SigemsLogin);
    } // retranslateUi

};

namespace Ui {
    class Dlg_SigemsLogin: public Ui_Dlg_SigemsLogin {};
} // namespace Ui

QT_END_NAMESPACE

class Dlg_SigemsLogin : public QDialog, public Ui::Dlg_SigemsLogin
{
    Q_OBJECT

public:
    Dlg_SigemsLogin(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~Dlg_SigemsLogin();

protected slots:
    virtual void languageChange();

};

#endif // DLG_SIGEMSLOGIN_H

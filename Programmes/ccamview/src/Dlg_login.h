#ifndef DLG_LOGIN_H
#define DLG_LOGIN_H

#include <qvariant.h>


#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_Dlg_Login
{
public:
    QGridLayout *gridLayout;
    QLabel *textLabel1;
    QLineEdit *lineEdit_Utilisateur;
    QLineEdit *lineEdit_MotDePasse;
    QLabel *textLabel2;
    QPushButton *bouton_OK;
    QSpacerItem *spacer1;
    QPushButton *bouton_Quitter;
    QLabel *textLabel1_2;

    void setupUi(QDialog *Dlg_Login)
    {
        if (Dlg_Login->objectName().isEmpty())
            Dlg_Login->setObjectName(QString::fromUtf8("Dlg_Login"));
        Dlg_Login->resize(330, 170);
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Dlg_Login->sizePolicy().hasHeightForWidth());
        Dlg_Login->setSizePolicy(sizePolicy);
        Dlg_Login->setMinimumSize(QSize(330, 170));
        Dlg_Login->setMaximumSize(QSize(330, 170));
        gridLayout = new QGridLayout(Dlg_Login);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        textLabel1 = new QLabel(Dlg_Login);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        QFont font;
        font.setBold(true);
        textLabel1->setFont(font);
        textLabel1->setWordWrap(false);

        gridLayout->addWidget(textLabel1, 0, 0, 1, 1);

        lineEdit_Utilisateur = new QLineEdit(Dlg_Login);
        lineEdit_Utilisateur->setObjectName(QString::fromUtf8("lineEdit_Utilisateur"));

        gridLayout->addWidget(lineEdit_Utilisateur, 0, 1, 1, 2);

        lineEdit_MotDePasse = new QLineEdit(Dlg_Login);
        lineEdit_MotDePasse->setObjectName(QString::fromUtf8("lineEdit_MotDePasse"));
        lineEdit_MotDePasse->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(lineEdit_MotDePasse, 1, 1, 1, 2);

        textLabel2 = new QLabel(Dlg_Login);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        textLabel2->setFont(font);
        textLabel2->setFrameShape(QFrame::NoFrame);
        textLabel2->setFrameShadow(QFrame::Plain);
        textLabel2->setWordWrap(false);

        gridLayout->addWidget(textLabel2, 1, 0, 1, 1);

        bouton_OK = new QPushButton(Dlg_Login);
        bouton_OK->setObjectName(QString::fromUtf8("bouton_OK"));
        bouton_OK->setAutoDefault(true);
        bouton_OK->setDefault(true);

        gridLayout->addWidget(bouton_OK, 2, 0, 1, 1);

        spacer1 = new QSpacerItem(147, 16, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacer1, 2, 1, 1, 1);

        bouton_Quitter = new QPushButton(Dlg_Login);
        bouton_Quitter->setObjectName(QString::fromUtf8("bouton_Quitter"));

        gridLayout->addWidget(bouton_Quitter, 2, 2, 1, 1);

        textLabel1_2 = new QLabel(Dlg_Login);
        textLabel1_2->setObjectName(QString::fromUtf8("textLabel1_2"));
        textLabel1_2->setFrameShape(QFrame::NoFrame);
        textLabel1_2->setWordWrap(false);

        gridLayout->addWidget(textLabel1_2, 3, 0, 1, 3);

#ifndef QT_NO_SHORTCUT
        textLabel1->setBuddy(lineEdit_Utilisateur);
        textLabel2->setBuddy(lineEdit_MotDePasse);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(lineEdit_Utilisateur, lineEdit_MotDePasse);
        QWidget::setTabOrder(lineEdit_MotDePasse, bouton_OK);
        QWidget::setTabOrder(bouton_OK, bouton_Quitter);

        retranslateUi(Dlg_Login);

        QMetaObject::connectSlotsByName(Dlg_Login);
    } // setupUi

    void retranslateUi(QDialog *Dlg_Login)
    {
        Dlg_Login->setWindowTitle(QApplication::translate("Dlg_Login", "CCAM VIEW", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("Dlg_Login", "&Utilisateur", 0, QApplication::UnicodeUTF8));
        textLabel2->setText(QApplication::translate("Dlg_Login", "&Mot de passe", 0, QApplication::UnicodeUTF8));
        bouton_OK->setText(QApplication::translate("Dlg_Login", "&Valider", 0, QApplication::UnicodeUTF8));
        bouton_OK->setShortcut(QApplication::translate("Dlg_Login", "Alt+V", 0, QApplication::UnicodeUTF8));
        bouton_Quitter->setText(QApplication::translate("Dlg_Login", "&Quitter", 0, QApplication::UnicodeUTF8));
        bouton_Quitter->setShortcut(QApplication::translate("Dlg_Login", "Alt+Q", 0, QApplication::UnicodeUTF8));
        textLabel1_2->setText(QApplication::translate("Dlg_Login", "<p align=\"center\">( lors de la premi\303\250re utilisation entrez <b>admin</b> <br>comme utilisateur, sans mot de passe )</p>", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Dlg_Login);
    } // retranslateUi

};

namespace Ui {
    class Dlg_Login: public Ui_Dlg_Login {};
} // namespace Ui

QT_END_NAMESPACE

class Dlg_Login : public QDialog, public Ui::Dlg_Login
{
    Q_OBJECT

public:
    Dlg_Login(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~Dlg_Login();

protected slots:
    virtual void languageChange();

};

#endif // DLG_LOGIN_H

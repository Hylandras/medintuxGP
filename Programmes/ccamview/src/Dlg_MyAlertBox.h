#ifndef DLG_MYALERTBOX_H
#define DLG_MYALERTBOX_H

#include <qvariant.h>


#include <Qt3Support/Q3MimeSourceFactory>
#include <Qt3Support/Q3TextEdit>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Dlg_MyAlertBox
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *vboxLayout;
    QPushButton *pushButton1;
    QPushButton *pushButton2;
    QPushButton *pushButton3;
    QPushButton *pushButton4;
    Q3TextEdit *textEditMesage;

    void setupUi(QDialog *Dlg_MyAlertBox)
    {
        if (Dlg_MyAlertBox->objectName().isEmpty())
            Dlg_MyAlertBox->setObjectName(QString::fromUtf8("Dlg_MyAlertBox"));
        Dlg_MyAlertBox->resize(541, 219);
        gridLayout = new QGridLayout(Dlg_MyAlertBox);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        pushButton1 = new QPushButton(Dlg_MyAlertBox);
        pushButton1->setObjectName(QString::fromUtf8("pushButton1"));

        vboxLayout->addWidget(pushButton1);

        pushButton2 = new QPushButton(Dlg_MyAlertBox);
        pushButton2->setObjectName(QString::fromUtf8("pushButton2"));

        vboxLayout->addWidget(pushButton2);

        pushButton3 = new QPushButton(Dlg_MyAlertBox);
        pushButton3->setObjectName(QString::fromUtf8("pushButton3"));

        vboxLayout->addWidget(pushButton3);

        pushButton4 = new QPushButton(Dlg_MyAlertBox);
        pushButton4->setObjectName(QString::fromUtf8("pushButton4"));

        vboxLayout->addWidget(pushButton4);


        gridLayout->addLayout(vboxLayout, 0, 1, 1, 1);

        textEditMesage = new Q3TextEdit(Dlg_MyAlertBox);
        textEditMesage->setObjectName(QString::fromUtf8("textEditMesage"));
        QFont font;
        textEditMesage->setFont(font);
        textEditMesage->setTextFormat(Qt::AutoText);
        textEditMesage->setUndoDepth(0);
        textEditMesage->setReadOnly(true);

        gridLayout->addWidget(textEditMesage, 0, 0, 1, 1);


        retranslateUi(Dlg_MyAlertBox);

        QMetaObject::connectSlotsByName(Dlg_MyAlertBox);
    } // setupUi

    void retranslateUi(QDialog *Dlg_MyAlertBox)
    {
        Dlg_MyAlertBox->setWindowTitle(QApplication::translate("Dlg_MyAlertBox", "Form1", 0, QApplication::UnicodeUTF8));
        pushButton1->setText(QApplication::translate("Dlg_MyAlertBox", "pushButton1", 0, QApplication::UnicodeUTF8));
        pushButton2->setText(QApplication::translate("Dlg_MyAlertBox", "pushButton1", 0, QApplication::UnicodeUTF8));
        pushButton3->setText(QApplication::translate("Dlg_MyAlertBox", "pushButton1", 0, QApplication::UnicodeUTF8));
        pushButton4->setText(QApplication::translate("Dlg_MyAlertBox", "pushButton1", 0, QApplication::UnicodeUTF8));
        textEditMesage->setText(QApplication::translate("Dlg_MyAlertBox", "texte pour voir", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Dlg_MyAlertBox);
    } // retranslateUi

};

namespace Ui {
    class Dlg_MyAlertBox: public Ui_Dlg_MyAlertBox {};
} // namespace Ui

QT_END_NAMESPACE

class Dlg_MyAlertBox : public QDialog, public Ui::Dlg_MyAlertBox
{
    Q_OBJECT

public:
    Dlg_MyAlertBox(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~Dlg_MyAlertBox();

protected slots:
    virtual void languageChange();

};

#endif // DLG_MYALERTBOX_H

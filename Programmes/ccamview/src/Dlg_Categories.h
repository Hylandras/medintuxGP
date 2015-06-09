#ifndef DLG_CATEGORIES_H
#define DLG_CATEGORIES_H

#include <qvariant.h>


#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Dlg_Categories
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *textLabel2;
    QComboBox *combo_Categorie;
    QHBoxLayout *hboxLayout1;
    QLabel *textLabel3;
    QLineEdit *lineEdit_LibelleUsuel;
    QHBoxLayout *hboxLayout2;
    QPushButton *bouton_OK;
    QPushButton *bouton_Annuler;

    void setupUi(QDialog *Dlg_Categories)
    {
        if (Dlg_Categories->objectName().isEmpty())
            Dlg_Categories->setObjectName(QString::fromUtf8("Dlg_Categories"));
        Dlg_Categories->resize(428, 116);
        gridLayout = new QGridLayout(Dlg_Categories);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        textLabel2 = new QLabel(Dlg_Categories);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        QFont font;
        font.setBold(true);
        textLabel2->setFont(font);
        textLabel2->setWordWrap(false);

        hboxLayout->addWidget(textLabel2);

        combo_Categorie = new QComboBox(Dlg_Categories);
        combo_Categorie->setObjectName(QString::fromUtf8("combo_Categorie"));
        combo_Categorie->setEditable(true);

        hboxLayout->addWidget(combo_Categorie);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        textLabel3 = new QLabel(Dlg_Categories);
        textLabel3->setObjectName(QString::fromUtf8("textLabel3"));
        textLabel3->setFont(font);
        textLabel3->setWordWrap(false);

        hboxLayout1->addWidget(textLabel3);

        lineEdit_LibelleUsuel = new QLineEdit(Dlg_Categories);
        lineEdit_LibelleUsuel->setObjectName(QString::fromUtf8("lineEdit_LibelleUsuel"));

        hboxLayout1->addWidget(lineEdit_LibelleUsuel);


        vboxLayout->addLayout(hboxLayout1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        bouton_OK = new QPushButton(Dlg_Categories);
        bouton_OK->setObjectName(QString::fromUtf8("bouton_OK"));

        hboxLayout2->addWidget(bouton_OK);

        bouton_Annuler = new QPushButton(Dlg_Categories);
        bouton_Annuler->setObjectName(QString::fromUtf8("bouton_Annuler"));

        hboxLayout2->addWidget(bouton_Annuler);


        vboxLayout->addLayout(hboxLayout2);


        gridLayout->addLayout(vboxLayout, 0, 0, 1, 1);

#ifndef QT_NO_SHORTCUT
        textLabel2->setBuddy(combo_Categorie);
        textLabel3->setBuddy(lineEdit_LibelleUsuel);
#endif // QT_NO_SHORTCUT

        retranslateUi(Dlg_Categories);

        QMetaObject::connectSlotsByName(Dlg_Categories);
    } // setupUi

    void retranslateUi(QDialog *Dlg_Categories)
    {
        Dlg_Categories->setWindowTitle(QApplication::translate("Dlg_Categories", "Modification de l'acte", 0, QApplication::UnicodeUTF8));
        textLabel2->setText(QApplication::translate("Dlg_Categories", "&Cat\303\251gorie", 0, QApplication::UnicodeUTF8));
        textLabel3->setText(QApplication::translate("Dlg_Categories", "&Libell\303\251 usuel", 0, QApplication::UnicodeUTF8));
        bouton_OK->setText(QApplication::translate("Dlg_Categories", "&Valider", 0, QApplication::UnicodeUTF8));
        bouton_OK->setShortcut(QApplication::translate("Dlg_Categories", "Alt+V", 0, QApplication::UnicodeUTF8));
        bouton_Annuler->setText(QApplication::translate("Dlg_Categories", "Annu&ler", 0, QApplication::UnicodeUTF8));
        bouton_Annuler->setShortcut(QApplication::translate("Dlg_Categories", "Alt+L", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Dlg_Categories);
    } // retranslateUi

};

namespace Ui {
    class Dlg_Categories: public Ui_Dlg_Categories {};
} // namespace Ui

QT_END_NAMESPACE

class Dlg_Categories : public QDialog, public Ui::Dlg_Categories
{
    Q_OBJECT

public:
    Dlg_Categories(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~Dlg_Categories();

protected slots:
    virtual void languageChange();

};

#endif // DLG_CATEGORIES_H

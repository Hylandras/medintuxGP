#ifndef DLG_PRATICIENS_H
#define DLG_PRATICIENS_H

#include <qvariant.h>


#include <Qt3Support/Q3GroupBox>
#include <Qt3Support/Q3Header>
#include <Qt3Support/Q3ListView>
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
#include "CMoteurCCAM_Base.h"

QT_BEGIN_NAMESPACE

class Ui_Dlg_Praticiens
{
public:
    QGridLayout *gridLayout;
    QLineEdit *lineEdit_Nom;
    QLineEdit *lineEdit_Prenom;
    QLabel *textLabel2;
    QLabel *textLabel3;
    QLabel *textLabel6;
    QLabel *textLabel7;
    QLineEdit *lineEdit_Nid;
    QLineEdit *lineEdit_Spec;
    Q3ListView *listview_praticiens;
    QHBoxLayout *hboxLayout;
    QPushButton *bouton_OK;
    QPushButton *bouton_New;
    QPushButton *pushButtonDelete;
    QPushButton *bouton_Quitter;
    Q3GroupBox *groupBoxMedinTux;
    QGridLayout *gridLayout1;
    QGridLayout *gridLayout2;
    QVBoxLayout *vboxLayout;
    QLineEdit *lineEdit_LogMedinTux;
    QLineEdit *lineEdit_MotDePasse_MedinTux;
    QVBoxLayout *vboxLayout1;
    QLabel *textLabel1_2;
    QLabel *textLabel4_2;
    QVBoxLayout *vboxLayout2;
    QLabel *textLabel1_3;
    QHBoxLayout *hboxLayout1;
    QLabel *textLabel5_2;
    QLineEdit *lineEdit_ConfMot_MedinTux;
    Q3GroupBox *groupBox1;
    QGridLayout *gridLayout3;
    QGridLayout *gridLayout4;
    QVBoxLayout *vboxLayout3;
    QLabel *textLabel1;
    QLabel *textLabel4;
    QVBoxLayout *vboxLayout4;
    QLineEdit *lineEdit_Code;
    QLineEdit *lineEdit_MotDePasse;
    QGridLayout *gridLayout5;
    QHBoxLayout *hboxLayout2;
    QLabel *textLabel5;
    QLineEdit *lineEdit_ConfMot;
    QLabel *textLabel2_2;

    void setupUi(QDialog *Dlg_Praticiens)
    {
        if (Dlg_Praticiens->objectName().isEmpty())
            Dlg_Praticiens->setObjectName(QString::fromUtf8("Dlg_Praticiens"));
        Dlg_Praticiens->resize(632, 361);
        gridLayout = new QGridLayout(Dlg_Praticiens);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lineEdit_Nom = new QLineEdit(Dlg_Praticiens);
        lineEdit_Nom->setObjectName(QString::fromUtf8("lineEdit_Nom"));
        lineEdit_Nom->setMaxLength(30);

        gridLayout->addWidget(lineEdit_Nom, 1, 1, 1, 1);

        lineEdit_Prenom = new QLineEdit(Dlg_Praticiens);
        lineEdit_Prenom->setObjectName(QString::fromUtf8("lineEdit_Prenom"));
        lineEdit_Prenom->setMaxLength(30);

        gridLayout->addWidget(lineEdit_Prenom, 2, 1, 1, 1);

        textLabel2 = new QLabel(Dlg_Praticiens);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        textLabel2->setWordWrap(false);

        gridLayout->addWidget(textLabel2, 1, 0, 1, 1);

        textLabel3 = new QLabel(Dlg_Praticiens);
        textLabel3->setObjectName(QString::fromUtf8("textLabel3"));
        textLabel3->setWordWrap(false);

        gridLayout->addWidget(textLabel3, 2, 0, 1, 1);

        textLabel6 = new QLabel(Dlg_Praticiens);
        textLabel6->setObjectName(QString::fromUtf8("textLabel6"));
        textLabel6->setWordWrap(false);

        gridLayout->addWidget(textLabel6, 1, 2, 1, 1);

        textLabel7 = new QLabel(Dlg_Praticiens);
        textLabel7->setObjectName(QString::fromUtf8("textLabel7"));
        textLabel7->setWordWrap(false);

        gridLayout->addWidget(textLabel7, 2, 2, 1, 1);

        lineEdit_Nid = new QLineEdit(Dlg_Praticiens);
        lineEdit_Nid->setObjectName(QString::fromUtf8("lineEdit_Nid"));
        lineEdit_Nid->setMaxLength(32);

        gridLayout->addWidget(lineEdit_Nid, 1, 3, 1, 1);

        lineEdit_Spec = new QLineEdit(Dlg_Praticiens);
        lineEdit_Spec->setObjectName(QString::fromUtf8("lineEdit_Spec"));
        lineEdit_Spec->setMaxLength(2);

        gridLayout->addWidget(lineEdit_Spec, 2, 3, 1, 1);

        listview_praticiens = new Q3ListView(Dlg_Praticiens);
        listview_praticiens->addColumn(QApplication::translate("Dlg_Praticiens", "Code", 0, QApplication::UnicodeUTF8));
        listview_praticiens->header()->setClickEnabled(true, listview_praticiens->header()->count() - 1);
        listview_praticiens->header()->setResizeEnabled(true, listview_praticiens->header()->count() - 1);
        listview_praticiens->addColumn(QApplication::translate("Dlg_Praticiens", "Mot de Passe", 0, QApplication::UnicodeUTF8));
        listview_praticiens->header()->setClickEnabled(true, listview_praticiens->header()->count() - 1);
        listview_praticiens->header()->setResizeEnabled(true, listview_praticiens->header()->count() - 1);
        listview_praticiens->addColumn(QApplication::translate("Dlg_Praticiens", "Nom", 0, QApplication::UnicodeUTF8));
        listview_praticiens->header()->setClickEnabled(true, listview_praticiens->header()->count() - 1);
        listview_praticiens->header()->setResizeEnabled(true, listview_praticiens->header()->count() - 1);
        listview_praticiens->addColumn(QApplication::translate("Dlg_Praticiens", "Pr\303\251nom", 0, QApplication::UnicodeUTF8));
        listview_praticiens->header()->setClickEnabled(true, listview_praticiens->header()->count() - 1);
        listview_praticiens->header()->setResizeEnabled(true, listview_praticiens->header()->count() - 1);
        listview_praticiens->addColumn(QApplication::translate("Dlg_Praticiens", "Num\303\251ro d'identification", 0, QApplication::UnicodeUTF8));
        listview_praticiens->header()->setClickEnabled(true, listview_praticiens->header()->count() - 1);
        listview_praticiens->header()->setResizeEnabled(true, listview_praticiens->header()->count() - 1);
        listview_praticiens->addColumn(QApplication::translate("Dlg_Praticiens", "Sp\303\251cialit\303\251", 0, QApplication::UnicodeUTF8));
        listview_praticiens->header()->setClickEnabled(true, listview_praticiens->header()->count() - 1);
        listview_praticiens->header()->setResizeEnabled(true, listview_praticiens->header()->count() - 1);
        listview_praticiens->addColumn(QApplication::translate("Dlg_Praticiens", "Pk", 0, QApplication::UnicodeUTF8));
        listview_praticiens->header()->setClickEnabled(true, listview_praticiens->header()->count() - 1);
        listview_praticiens->header()->setResizeEnabled(true, listview_praticiens->header()->count() - 1);
        listview_praticiens->setObjectName(QString::fromUtf8("listview_praticiens"));

        gridLayout->addWidget(listview_praticiens, 0, 0, 1, 4);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        bouton_OK = new QPushButton(Dlg_Praticiens);
        bouton_OK->setObjectName(QString::fromUtf8("bouton_OK"));

        hboxLayout->addWidget(bouton_OK);

        bouton_New = new QPushButton(Dlg_Praticiens);
        bouton_New->setObjectName(QString::fromUtf8("bouton_New"));

        hboxLayout->addWidget(bouton_New);

        pushButtonDelete = new QPushButton(Dlg_Praticiens);
        pushButtonDelete->setObjectName(QString::fromUtf8("pushButtonDelete"));

        hboxLayout->addWidget(pushButtonDelete);

        bouton_Quitter = new QPushButton(Dlg_Praticiens);
        bouton_Quitter->setObjectName(QString::fromUtf8("bouton_Quitter"));

        hboxLayout->addWidget(bouton_Quitter);


        gridLayout->addLayout(hboxLayout, 5, 0, 1, 4);

        groupBoxMedinTux = new Q3GroupBox(Dlg_Praticiens);
        groupBoxMedinTux->setObjectName(QString::fromUtf8("groupBoxMedinTux"));
        groupBoxMedinTux->setColumnLayout(0, Qt::Vertical);
        groupBoxMedinTux->layout()->setSpacing(6);
        groupBoxMedinTux->layout()->setMargin(11);
        gridLayout1 = new QGridLayout();
        QBoxLayout *boxlayout = qobject_cast<QBoxLayout *>(groupBoxMedinTux->layout());
        if (boxlayout)
            boxlayout->addLayout(gridLayout1);
        gridLayout1->setAlignment(Qt::AlignTop);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        gridLayout2 = new QGridLayout();
        gridLayout2->setSpacing(6);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        lineEdit_LogMedinTux = new QLineEdit(groupBoxMedinTux);
        lineEdit_LogMedinTux->setObjectName(QString::fromUtf8("lineEdit_LogMedinTux"));
        lineEdit_LogMedinTux->setMaxLength(6);
        lineEdit_LogMedinTux->setModified(false);
        lineEdit_LogMedinTux->setReadOnly(false);

        vboxLayout->addWidget(lineEdit_LogMedinTux);

        lineEdit_MotDePasse_MedinTux = new QLineEdit(groupBoxMedinTux);
        lineEdit_MotDePasse_MedinTux->setObjectName(QString::fromUtf8("lineEdit_MotDePasse_MedinTux"));
        lineEdit_MotDePasse_MedinTux->setMaxLength(32);
        lineEdit_MotDePasse_MedinTux->setReadOnly(true);

        vboxLayout->addWidget(lineEdit_MotDePasse_MedinTux);


        gridLayout2->addLayout(vboxLayout, 0, 1, 1, 1);

        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setSpacing(6);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        textLabel1_2 = new QLabel(groupBoxMedinTux);
        textLabel1_2->setObjectName(QString::fromUtf8("textLabel1_2"));
        textLabel1_2->setWordWrap(false);

        vboxLayout1->addWidget(textLabel1_2);

        textLabel4_2 = new QLabel(groupBoxMedinTux);
        textLabel4_2->setObjectName(QString::fromUtf8("textLabel4_2"));
        textLabel4_2->setWordWrap(false);

        vboxLayout1->addWidget(textLabel4_2);


        gridLayout2->addLayout(vboxLayout1, 0, 0, 1, 1);


        gridLayout1->addLayout(gridLayout2, 0, 0, 1, 1);

        vboxLayout2 = new QVBoxLayout();
        vboxLayout2->setSpacing(6);
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        textLabel1_3 = new QLabel(groupBoxMedinTux);
        textLabel1_3->setObjectName(QString::fromUtf8("textLabel1_3"));
        textLabel1_3->setWordWrap(false);

        vboxLayout2->addWidget(textLabel1_3);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        textLabel5_2 = new QLabel(groupBoxMedinTux);
        textLabel5_2->setObjectName(QString::fromUtf8("textLabel5_2"));
        textLabel5_2->setWordWrap(false);

        hboxLayout1->addWidget(textLabel5_2);

        lineEdit_ConfMot_MedinTux = new QLineEdit(groupBoxMedinTux);
        lineEdit_ConfMot_MedinTux->setObjectName(QString::fromUtf8("lineEdit_ConfMot_MedinTux"));
        lineEdit_ConfMot_MedinTux->setMaxLength(32);
        lineEdit_ConfMot_MedinTux->setReadOnly(true);

        hboxLayout1->addWidget(lineEdit_ConfMot_MedinTux);


        vboxLayout2->addLayout(hboxLayout1);


        gridLayout1->addLayout(vboxLayout2, 0, 1, 1, 1);


        gridLayout->addWidget(groupBoxMedinTux, 4, 0, 1, 4);

        groupBox1 = new Q3GroupBox(Dlg_Praticiens);
        groupBox1->setObjectName(QString::fromUtf8("groupBox1"));
        groupBox1->setColumnLayout(0, Qt::Vertical);
        groupBox1->layout()->setSpacing(6);
        groupBox1->layout()->setMargin(11);
        gridLayout3 = new QGridLayout();
        QBoxLayout *boxlayout1 = qobject_cast<QBoxLayout *>(groupBox1->layout());
        if (boxlayout1)
            boxlayout1->addLayout(gridLayout3);
        gridLayout3->setAlignment(Qt::AlignTop);
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        gridLayout4 = new QGridLayout();
        gridLayout4->setSpacing(6);
        gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
        vboxLayout3 = new QVBoxLayout();
        vboxLayout3->setSpacing(6);
        vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
        textLabel1 = new QLabel(groupBox1);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        textLabel1->setWordWrap(false);

        vboxLayout3->addWidget(textLabel1);

        textLabel4 = new QLabel(groupBox1);
        textLabel4->setObjectName(QString::fromUtf8("textLabel4"));
        textLabel4->setWordWrap(false);

        vboxLayout3->addWidget(textLabel4);


        gridLayout4->addLayout(vboxLayout3, 0, 0, 1, 1);

        vboxLayout4 = new QVBoxLayout();
        vboxLayout4->setSpacing(6);
        vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
        lineEdit_Code = new QLineEdit(groupBox1);
        lineEdit_Code->setObjectName(QString::fromUtf8("lineEdit_Code"));
        lineEdit_Code->setMaxLength(6);

        vboxLayout4->addWidget(lineEdit_Code);

        lineEdit_MotDePasse = new QLineEdit(groupBox1);
        lineEdit_MotDePasse->setObjectName(QString::fromUtf8("lineEdit_MotDePasse"));
        lineEdit_MotDePasse->setMaxLength(32);

        vboxLayout4->addWidget(lineEdit_MotDePasse);


        gridLayout4->addLayout(vboxLayout4, 0, 1, 1, 1);


        gridLayout3->addLayout(gridLayout4, 0, 0, 1, 1);

        gridLayout5 = new QGridLayout();
        gridLayout5->setSpacing(6);
        gridLayout5->setObjectName(QString::fromUtf8("gridLayout5"));
        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        textLabel5 = new QLabel(groupBox1);
        textLabel5->setObjectName(QString::fromUtf8("textLabel5"));
        textLabel5->setWordWrap(false);

        hboxLayout2->addWidget(textLabel5);

        lineEdit_ConfMot = new QLineEdit(groupBox1);
        lineEdit_ConfMot->setObjectName(QString::fromUtf8("lineEdit_ConfMot"));
        lineEdit_ConfMot->setMaxLength(32);

        hboxLayout2->addWidget(lineEdit_ConfMot);


        gridLayout5->addLayout(hboxLayout2, 1, 0, 1, 1);

        textLabel2_2 = new QLabel(groupBox1);
        textLabel2_2->setObjectName(QString::fromUtf8("textLabel2_2"));
        textLabel2_2->setWordWrap(false);

        gridLayout5->addWidget(textLabel2_2, 0, 0, 1, 1);


        gridLayout3->addLayout(gridLayout5, 0, 1, 1, 1);


        gridLayout->addWidget(groupBox1, 3, 0, 1, 4);

#ifndef QT_NO_SHORTCUT
        textLabel2->setBuddy(lineEdit_Nom);
        textLabel3->setBuddy(lineEdit_Prenom);
        textLabel6->setBuddy(lineEdit_Nid);
        textLabel7->setBuddy(lineEdit_Spec);
        textLabel1_2->setBuddy(lineEdit_LogMedinTux);
        textLabel4_2->setBuddy(lineEdit_MotDePasse_MedinTux);
        textLabel5_2->setBuddy(lineEdit_ConfMot_MedinTux);
        textLabel1->setBuddy(lineEdit_Code);
        textLabel4->setBuddy(lineEdit_MotDePasse);
        textLabel5->setBuddy(lineEdit_ConfMot);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(lineEdit_Nom, lineEdit_Prenom);
        QWidget::setTabOrder(lineEdit_Prenom, lineEdit_Nid);
        QWidget::setTabOrder(lineEdit_Nid, lineEdit_Spec);
        QWidget::setTabOrder(lineEdit_Spec, lineEdit_Code);
        QWidget::setTabOrder(lineEdit_Code, lineEdit_MotDePasse);
        QWidget::setTabOrder(lineEdit_MotDePasse, lineEdit_ConfMot);
        QWidget::setTabOrder(lineEdit_ConfMot, lineEdit_LogMedinTux);
        QWidget::setTabOrder(lineEdit_LogMedinTux, lineEdit_MotDePasse_MedinTux);
        QWidget::setTabOrder(lineEdit_MotDePasse_MedinTux, lineEdit_ConfMot_MedinTux);
        QWidget::setTabOrder(lineEdit_ConfMot_MedinTux, bouton_OK);
        QWidget::setTabOrder(bouton_OK, bouton_New);
        QWidget::setTabOrder(bouton_New, pushButtonDelete);
        QWidget::setTabOrder(pushButtonDelete, bouton_Quitter);
        QWidget::setTabOrder(bouton_Quitter, listview_praticiens);

        retranslateUi(Dlg_Praticiens);
        QObject::connect(bouton_Quitter, SIGNAL(clicked()), Dlg_Praticiens, SLOT(bouton_Quitter_clicked()));
        QObject::connect(listview_praticiens, SIGNAL(clicked(Q3ListViewItem*)), Dlg_Praticiens, SLOT(listview_praticiens_clicked(Q3ListViewItem*)));
        QObject::connect(lineEdit_Code, SIGNAL(textChanged(QString)), Dlg_Praticiens, SLOT(lineEdit_Code_textChanged(QString)));
        QObject::connect(lineEdit_Nom, SIGNAL(textChanged(QString)), Dlg_Praticiens, SLOT(lineEdit_Nom_textChanged(QString)));
        QObject::connect(lineEdit_Prenom, SIGNAL(textChanged(QString)), Dlg_Praticiens, SLOT(lineEdit_Prenom_textChanged(QString)));
        QObject::connect(lineEdit_Nid, SIGNAL(textChanged(QString)), Dlg_Praticiens, SLOT(lineEdit_Nid_textChanged(QString)));
        QObject::connect(lineEdit_Spec, SIGNAL(textChanged(QString)), Dlg_Praticiens, SLOT(lineEdit_Spec_textChanged(QString)));
        QObject::connect(lineEdit_MotDePasse, SIGNAL(textChanged(QString)), Dlg_Praticiens, SLOT(lineEdit_MotDePasse_textChanged(QString)));
        QObject::connect(lineEdit_ConfMot, SIGNAL(textChanged(QString)), Dlg_Praticiens, SLOT(lineEdit_ConfMot_textChanged(QString)));
        QObject::connect(bouton_OK, SIGNAL(clicked()), Dlg_Praticiens, SLOT(bouton_OK_clicked()));
        QObject::connect(bouton_New, SIGNAL(clicked()), Dlg_Praticiens, SLOT(bouton_New_clicked()));
        QObject::connect(pushButtonDelete, SIGNAL(clicked()), Dlg_Praticiens, SLOT(pushButtonDelete_clicked()));

        QMetaObject::connectSlotsByName(Dlg_Praticiens);
    } // setupUi

    void retranslateUi(QDialog *Dlg_Praticiens)
    {
        Dlg_Praticiens->setWindowTitle(QApplication::translate("Dlg_Praticiens", "Saisie / Modification : Fiches Praticiens", 0, QApplication::UnicodeUTF8));
        textLabel2->setText(QApplication::translate("Dlg_Praticiens", "&Nom", 0, QApplication::UnicodeUTF8));
        textLabel3->setText(QApplication::translate("Dlg_Praticiens", "&Pr\303\251nom", 0, QApplication::UnicodeUTF8));
        textLabel6->setText(QApplication::translate("Dlg_Praticiens", "N.&Identification", 0, QApplication::UnicodeUTF8));
        textLabel7->setText(QApplication::translate("Dlg_Praticiens", "&Sp\303\251cialit\303\251", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit_Nid->setProperty("toolTip", QVariant(QApplication::translate("Dlg_Praticiens", "Si trois prmier caract\303\250res sont 999 alors l'utilisateur est type secr\303\251taire", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
        listview_praticiens->header()->setLabel(0, QApplication::translate("Dlg_Praticiens", "Code", 0, QApplication::UnicodeUTF8));
        listview_praticiens->header()->setLabel(1, QApplication::translate("Dlg_Praticiens", "Mot de Passe", 0, QApplication::UnicodeUTF8));
        listview_praticiens->header()->setLabel(2, QApplication::translate("Dlg_Praticiens", "Nom", 0, QApplication::UnicodeUTF8));
        listview_praticiens->header()->setLabel(3, QApplication::translate("Dlg_Praticiens", "Pr\303\251nom", 0, QApplication::UnicodeUTF8));
        listview_praticiens->header()->setLabel(4, QApplication::translate("Dlg_Praticiens", "Num\303\251ro d'identification", 0, QApplication::UnicodeUTF8));
        listview_praticiens->header()->setLabel(5, QApplication::translate("Dlg_Praticiens", "Sp\303\251cialit\303\251", 0, QApplication::UnicodeUTF8));
        listview_praticiens->header()->setLabel(6, QApplication::translate("Dlg_Praticiens", "Pk", 0, QApplication::UnicodeUTF8));
        bouton_OK->setText(QApplication::translate("Dlg_Praticiens", "&Enregistrer les modifications", 0, QApplication::UnicodeUTF8));
        bouton_OK->setShortcut(QApplication::translate("Dlg_Praticiens", "Alt+E", 0, QApplication::UnicodeUTF8));
        bouton_New->setText(QApplication::translate("Dlg_Praticiens", "&Cr\303\251er un nouvel utilisateur", 0, QApplication::UnicodeUTF8));
        bouton_New->setShortcut(QApplication::translate("Dlg_Praticiens", "Alt+C", 0, QApplication::UnicodeUTF8));
        pushButtonDelete->setText(QApplication::translate("Dlg_Praticiens", "E&ffacer", 0, QApplication::UnicodeUTF8));
        pushButtonDelete->setShortcut(QApplication::translate("Dlg_Praticiens", "Alt+F", 0, QApplication::UnicodeUTF8));
        bouton_Quitter->setText(QApplication::translate("Dlg_Praticiens", "&Quitter", 0, QApplication::UnicodeUTF8));
        bouton_Quitter->setShortcut(QApplication::translate("Dlg_Praticiens", "Alt+Q", 0, QApplication::UnicodeUTF8));
        groupBoxMedinTux->setTitle(QApplication::translate("Dlg_Praticiens", "MedinTux", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit_MotDePasse_MedinTux->setProperty("toolTip", QVariant(QApplication::translate("Dlg_Praticiens", "Ce mot de passe ne peut \303\252tre modifi\303\251 qu'au sein du gestionnaire d'utilisateurs de MedinTux", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
        textLabel1_2->setText(QApplication::translate("Dlg_Praticiens", "&Login", 0, QApplication::UnicodeUTF8));
        textLabel4_2->setText(QApplication::translate("Dlg_Praticiens", "Mo&t de Passe", 0, QApplication::UnicodeUTF8));
        textLabel1_3->setText(QString());
        textLabel5_2->setText(QApplication::translate("Dlg_Praticiens", "Conf&irmer le mot de passe", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit_ConfMot_MedinTux->setProperty("toolTip", QVariant(QApplication::translate("Dlg_Praticiens", "Ce mot de passe ne peut \303\252tre modifi\303\251 qu'au sein du gestionnaire d'utilisateurs de MedinTux", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
        groupBox1->setTitle(QApplication::translate("Dlg_Praticiens", " SIGEMS ", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("Dlg_Praticiens", "C&ode (6 car.)", 0, QApplication::UnicodeUTF8));
        textLabel4->setText(QApplication::translate("Dlg_Praticiens", "&Mot de Passe", 0, QApplication::UnicodeUTF8));
        textLabel5->setText(QApplication::translate("Dlg_Praticiens", "Confirmer le mot de &passe", 0, QApplication::UnicodeUTF8));
        textLabel2_2->setText(QString());
        Q_UNUSED(Dlg_Praticiens);
    } // retranslateUi

};

namespace Ui {
    class Dlg_Praticiens: public Ui_Dlg_Praticiens {};
} // namespace Ui

QT_END_NAMESPACE

class Dlg_Praticiens : public QDialog, public Ui::Dlg_Praticiens
{
    Q_OBJECT

public:
    Dlg_Praticiens(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~Dlg_Praticiens();

    CMoteurCCAM_Base* m_pCMoteurCCAM_Base;

    virtual int initDialog( CMoteurCCAM_Base * pCMoteurCCAM_Base );
    virtual int VerifSIGEMSValiditation();

public slots:
    virtual void bouton_Quitter_clicked();
    virtual void listview_praticiens_clicked( Q3ListViewItem * pQListViewItem );
    virtual void lineEdit_Code_textChanged( const QString & );
    virtual void AfficheInfosMedecins( const QString & nidMEDECIN );
    virtual void lineEdit_Nom_textChanged( const QString & );
    virtual void lineEdit_Prenom_textChanged( const QString & );
    virtual void lineEdit_Nid_textChanged( const QString & text );
    virtual void lineEdit_Spec_textChanged( const QString & );
    virtual void lineEdit_MotDePasse_textChanged( const QString & );
    virtual void lineEdit_ConfMot_textChanged( const QString & );
    virtual void bouton_OK_clicked();
    virtual void EffaceSaisie();
    virtual void bouton_New_clicked();
    virtual void pushButtonDelete_clicked();

protected slots:
    virtual void languageChange();

};

#endif // DLG_PRATICIENS_H

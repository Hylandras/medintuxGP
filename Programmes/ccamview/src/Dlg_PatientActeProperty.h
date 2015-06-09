#ifndef DLG_PATIENTACTEPROPERTY_H
#define DLG_PATIENTACTEPROPERTY_H

#include <qvariant.h>


#include <Qt3Support/Q3DateEdit>
#include <Qt3Support/Q3Frame>
#include <Qt3Support/Q3GroupBox>
#include <Qt3Support/Q3Header>
#include <Qt3Support/Q3ListView>
#include <Qt3Support/Q3MimeSourceFactory>
#include <Qt3Support/Q3TextEdit>
#include <Qt3Support/Q3TimeEdit>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dlg_PatientActeProperty
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QPushButton *pushButtonLastDate;
    QPushButton *pushButtonLastDate_Jour;
    Q3DateEdit *dateEdit_DateActe;
    QLabel *textLabel2_2;
    Q3TimeEdit *timeEdit_HeureActe;
    QLabel *textLabel4;
    QComboBox *comboBoxPraticien;
    QGridLayout *gridLayout1;
    Q3GroupBox *groupBox2;
    QGridLayout *gridLayout2;
    QGridLayout *gridLayout3;
    QComboBox *comboBox_Phase;
    QComboBox *comboBox_Activite;
    Q3GroupBox *groupBoxComboMod;
    QGridLayout *gridLayout4;
    QGridLayout *gridLayout5;
    QComboBox *combo_Mod02;
    QComboBox *combo_Mod03;
    QComboBox *combo_Mod01;
    QComboBox *combo_Mod04;
    QHBoxLayout *hboxLayout1;
    QPushButton *pushButtonOK;
    QPushButton *pushButtonCreerNewActe;
    QPushButton *pushButtonCancel;
    Q3ListView *listViewCodeAsso;
    QGridLayout *gridLayout6;
    QLabel *textLabelAsso;
    QFrame *lineLeft;
    QFrame *lineRight;
    Q3Frame *frame4;
    QGridLayout *gridLayout7;
    QGridLayout *gridLayout8;
    QGridLayout *gridLayout9;
    QLabel *textLabel_TarifCCAM;
    QLabel *textLabel1_2;
    QGridLayout *gridLayout10;
    QLabel *textLabel1;
    QLabel *textLabel_CodeCCAM;
    QGridLayout *gridLayout11;
    QLabel *textLabel3;
    Q3TextEdit *textEdit_LibelleActe;
    QLabel *text_Libelle_Modif;

    void setupUi(QDialog *Dlg_PatientActeProperty)
    {
        if (Dlg_PatientActeProperty->objectName().isEmpty())
            Dlg_PatientActeProperty->setObjectName(QString::fromUtf8("Dlg_PatientActeProperty"));
        Dlg_PatientActeProperty->resize(745, 470);
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(4));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Dlg_PatientActeProperty->sizePolicy().hasHeightForWidth());
        Dlg_PatientActeProperty->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(Dlg_PatientActeProperty);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        pushButtonLastDate = new QPushButton(Dlg_PatientActeProperty);
        pushButtonLastDate->setObjectName(QString::fromUtf8("pushButtonLastDate"));

        hboxLayout->addWidget(pushButtonLastDate);

        pushButtonLastDate_Jour = new QPushButton(Dlg_PatientActeProperty);
        pushButtonLastDate_Jour->setObjectName(QString::fromUtf8("pushButtonLastDate_Jour"));

        hboxLayout->addWidget(pushButtonLastDate_Jour);

        dateEdit_DateActe = new Q3DateEdit(Dlg_PatientActeProperty);
        dateEdit_DateActe->setObjectName(QString::fromUtf8("dateEdit_DateActe"));

        hboxLayout->addWidget(dateEdit_DateActe);

        textLabel2_2 = new QLabel(Dlg_PatientActeProperty);
        textLabel2_2->setObjectName(QString::fromUtf8("textLabel2_2"));
        QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(4), static_cast<QSizePolicy::Policy>(5));
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(textLabel2_2->sizePolicy().hasHeightForWidth());
        textLabel2_2->setSizePolicy(sizePolicy1);
        textLabel2_2->setFrameShape(QFrame::NoFrame);
        textLabel2_2->setFrameShadow(QFrame::Plain);
        textLabel2_2->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
        textLabel2_2->setWordWrap(false);

        hboxLayout->addWidget(textLabel2_2);

        timeEdit_HeureActe = new Q3TimeEdit(Dlg_PatientActeProperty);
        timeEdit_HeureActe->setObjectName(QString::fromUtf8("timeEdit_HeureActe"));

        hboxLayout->addWidget(timeEdit_HeureActe);

        textLabel4 = new QLabel(Dlg_PatientActeProperty);
        textLabel4->setObjectName(QString::fromUtf8("textLabel4"));
        sizePolicy1.setHeightForWidth(textLabel4->sizePolicy().hasHeightForWidth());
        textLabel4->setSizePolicy(sizePolicy1);
        textLabel4->setFrameShape(QFrame::NoFrame);
        textLabel4->setFrameShadow(QFrame::Plain);
        textLabel4->setWordWrap(false);

        hboxLayout->addWidget(textLabel4);

        comboBoxPraticien = new QComboBox(Dlg_PatientActeProperty);
        comboBoxPraticien->setObjectName(QString::fromUtf8("comboBoxPraticien"));
        QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(0));
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(comboBoxPraticien->sizePolicy().hasHeightForWidth());
        comboBoxPraticien->setSizePolicy(sizePolicy2);
        comboBoxPraticien->setMinimumSize(QSize(50, 0));
        comboBoxPraticien->setEditable(false);
        comboBoxPraticien->setAutoCompletion(true);

        hboxLayout->addWidget(comboBoxPraticien);


        gridLayout->addLayout(hboxLayout, 0, 0, 1, 1);

        gridLayout1 = new QGridLayout();
        gridLayout1->setSpacing(6);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        groupBox2 = new Q3GroupBox(Dlg_PatientActeProperty);
        groupBox2->setObjectName(QString::fromUtf8("groupBox2"));
        QSizePolicy sizePolicy3(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(0));
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(groupBox2->sizePolicy().hasHeightForWidth());
        groupBox2->setSizePolicy(sizePolicy3);
        groupBox2->setLineWidth(1);
        groupBox2->setMidLineWidth(0);
        groupBox2->setColumnLayout(0, Qt::Vertical);
        groupBox2->layout()->setSpacing(0);
        groupBox2->layout()->setMargin(0);
        gridLayout2 = new QGridLayout();
        QBoxLayout *boxlayout = qobject_cast<QBoxLayout *>(groupBox2->layout());
        if (boxlayout)
            boxlayout->addLayout(gridLayout2);
        gridLayout2->setAlignment(Qt::AlignTop);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        gridLayout3 = new QGridLayout();
        gridLayout3->setSpacing(6);
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        comboBox_Phase = new QComboBox(groupBox2);
        comboBox_Phase->setObjectName(QString::fromUtf8("comboBox_Phase"));
        comboBox_Phase->setMaximumSize(QSize(150, 32767));

        gridLayout3->addWidget(comboBox_Phase, 0, 0, 1, 1);

        comboBox_Activite = new QComboBox(groupBox2);
        comboBox_Activite->setObjectName(QString::fromUtf8("comboBox_Activite"));
        sizePolicy3.setHeightForWidth(comboBox_Activite->sizePolicy().hasHeightForWidth());
        comboBox_Activite->setSizePolicy(sizePolicy3);
        comboBox_Activite->setMaximumSize(QSize(150, 32767));

        gridLayout3->addWidget(comboBox_Activite, 0, 1, 1, 1);


        gridLayout2->addLayout(gridLayout3, 0, 0, 1, 1);


        gridLayout1->addWidget(groupBox2, 0, 0, 1, 1);

        groupBoxComboMod = new Q3GroupBox(Dlg_PatientActeProperty);
        groupBoxComboMod->setObjectName(QString::fromUtf8("groupBoxComboMod"));
        sizePolicy3.setHeightForWidth(groupBoxComboMod->sizePolicy().hasHeightForWidth());
        groupBoxComboMod->setSizePolicy(sizePolicy3);
        groupBoxComboMod->setFrameShape(Q3GroupBox::GroupBoxPanel);
        groupBoxComboMod->setFrameShadow(Q3GroupBox::Plain);
        groupBoxComboMod->setFlat(false);
        groupBoxComboMod->setColumnLayout(0, Qt::Vertical);
        groupBoxComboMod->layout()->setSpacing(0);
        groupBoxComboMod->layout()->setMargin(0);
        gridLayout4 = new QGridLayout();
        QBoxLayout *boxlayout1 = qobject_cast<QBoxLayout *>(groupBoxComboMod->layout());
        if (boxlayout1)
            boxlayout1->addLayout(gridLayout4);
        gridLayout4->setAlignment(Qt::AlignTop);
        gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
        gridLayout5 = new QGridLayout();
        gridLayout5->setSpacing(6);
        gridLayout5->setObjectName(QString::fromUtf8("gridLayout5"));
        combo_Mod02 = new QComboBox(groupBoxComboMod);
        combo_Mod02->setObjectName(QString::fromUtf8("combo_Mod02"));
        combo_Mod02->setMaximumSize(QSize(120, 32767));

        gridLayout5->addWidget(combo_Mod02, 0, 1, 1, 1);

        combo_Mod03 = new QComboBox(groupBoxComboMod);
        combo_Mod03->setObjectName(QString::fromUtf8("combo_Mod03"));
        combo_Mod03->setMaximumSize(QSize(120, 32767));

        gridLayout5->addWidget(combo_Mod03, 0, 2, 1, 1);

        combo_Mod01 = new QComboBox(groupBoxComboMod);
        combo_Mod01->setObjectName(QString::fromUtf8("combo_Mod01"));
        combo_Mod01->setMaximumSize(QSize(120, 32767));

        gridLayout5->addWidget(combo_Mod01, 0, 0, 1, 1);

        combo_Mod04 = new QComboBox(groupBoxComboMod);
        combo_Mod04->setObjectName(QString::fromUtf8("combo_Mod04"));
        combo_Mod04->setMaximumSize(QSize(120, 32767));

        gridLayout5->addWidget(combo_Mod04, 0, 3, 1, 1);


        gridLayout4->addLayout(gridLayout5, 0, 0, 1, 1);


        gridLayout1->addWidget(groupBoxComboMod, 0, 1, 1, 1);


        gridLayout->addLayout(gridLayout1, 1, 0, 1, 1);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        pushButtonOK = new QPushButton(Dlg_PatientActeProperty);
        pushButtonOK->setObjectName(QString::fromUtf8("pushButtonOK"));
        pushButtonOK->setDefault(true);

        hboxLayout1->addWidget(pushButtonOK);

        pushButtonCreerNewActe = new QPushButton(Dlg_PatientActeProperty);
        pushButtonCreerNewActe->setObjectName(QString::fromUtf8("pushButtonCreerNewActe"));
        pushButtonCreerNewActe->setDefault(false);

        hboxLayout1->addWidget(pushButtonCreerNewActe);

        pushButtonCancel = new QPushButton(Dlg_PatientActeProperty);
        pushButtonCancel->setObjectName(QString::fromUtf8("pushButtonCancel"));

        hboxLayout1->addWidget(pushButtonCancel);


        gridLayout->addLayout(hboxLayout1, 5, 0, 1, 1);

        listViewCodeAsso = new Q3ListView(Dlg_PatientActeProperty);
        listViewCodeAsso->addColumn(QApplication::translate("Dlg_PatientActeProperty", "Code CCAM", 0, QApplication::UnicodeUTF8));
        listViewCodeAsso->header()->setClickEnabled(true, listViewCodeAsso->header()->count() - 1);
        listViewCodeAsso->header()->setResizeEnabled(true, listViewCodeAsso->header()->count() - 1);
        listViewCodeAsso->addColumn(QApplication::translate("Dlg_PatientActeProperty", "Activit\303\251", 0, QApplication::UnicodeUTF8));
        listViewCodeAsso->header()->setClickEnabled(true, listViewCodeAsso->header()->count() - 1);
        listViewCodeAsso->header()->setResizeEnabled(true, listViewCodeAsso->header()->count() - 1);
        listViewCodeAsso->addColumn(QApplication::translate("Dlg_PatientActeProperty", "Tarif de base", 0, QApplication::UnicodeUTF8));
        listViewCodeAsso->header()->setClickEnabled(true, listViewCodeAsso->header()->count() - 1);
        listViewCodeAsso->header()->setResizeEnabled(true, listViewCodeAsso->header()->count() - 1);
        listViewCodeAsso->addColumn(QApplication::translate("Dlg_PatientActeProperty", "Type", 0, QApplication::UnicodeUTF8));
        listViewCodeAsso->header()->setClickEnabled(true, listViewCodeAsso->header()->count() - 1);
        listViewCodeAsso->header()->setResizeEnabled(true, listViewCodeAsso->header()->count() - 1);
        listViewCodeAsso->addColumn(QApplication::translate("Dlg_PatientActeProperty", "Libell\303\251", 0, QApplication::UnicodeUTF8));
        listViewCodeAsso->header()->setClickEnabled(true, listViewCodeAsso->header()->count() - 1);
        listViewCodeAsso->header()->setResizeEnabled(true, listViewCodeAsso->header()->count() - 1);
        listViewCodeAsso->addColumn(QApplication::translate("Dlg_PatientActeProperty", "R\303\250gle association", 0, QApplication::UnicodeUTF8));
        listViewCodeAsso->header()->setClickEnabled(true, listViewCodeAsso->header()->count() - 1);
        listViewCodeAsso->header()->setResizeEnabled(true, listViewCodeAsso->header()->count() - 1);
        listViewCodeAsso->setObjectName(QString::fromUtf8("listViewCodeAsso"));
        QSizePolicy sizePolicy4(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(7));
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(listViewCodeAsso->sizePolicy().hasHeightForWidth());
        listViewCodeAsso->setSizePolicy(sizePolicy4);

        gridLayout->addWidget(listViewCodeAsso, 4, 0, 1, 1);

        gridLayout6 = new QGridLayout();
        gridLayout6->setSpacing(6);
        gridLayout6->setObjectName(QString::fromUtf8("gridLayout6"));
        textLabelAsso = new QLabel(Dlg_PatientActeProperty);
        textLabelAsso->setObjectName(QString::fromUtf8("textLabelAsso"));
        QSizePolicy sizePolicy5(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(1));
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(textLabelAsso->sizePolicy().hasHeightForWidth());
        textLabelAsso->setSizePolicy(sizePolicy5);
        textLabelAsso->setAlignment(Qt::AlignJustify|Qt::AlignCenter|Qt::AlignBottom|Qt::AlignTop|Qt::AlignRight|Qt::AlignLeft);
        textLabelAsso->setWordWrap(true);

        gridLayout6->addWidget(textLabelAsso, 0, 1, 1, 1);

        lineLeft = new QFrame(Dlg_PatientActeProperty);
        lineLeft->setObjectName(QString::fromUtf8("lineLeft"));
        lineLeft->setFrameShape(QFrame::HLine);
        lineLeft->setFrameShadow(QFrame::Sunken);
        lineLeft->setLineWidth(1);

        gridLayout6->addWidget(lineLeft, 0, 0, 1, 1);

        lineRight = new QFrame(Dlg_PatientActeProperty);
        lineRight->setObjectName(QString::fromUtf8("lineRight"));
        lineRight->setFrameShape(QFrame::HLine);
        lineRight->setFrameShadow(QFrame::Sunken);
        lineRight->setLineWidth(1);

        gridLayout6->addWidget(lineRight, 0, 2, 1, 1);


        gridLayout->addLayout(gridLayout6, 3, 0, 1, 1);

        frame4 = new Q3Frame(Dlg_PatientActeProperty);
        frame4->setObjectName(QString::fromUtf8("frame4"));
        sizePolicy.setHeightForWidth(frame4->sizePolicy().hasHeightForWidth());
        frame4->setSizePolicy(sizePolicy);
        frame4->setFrameShape(QFrame::StyledPanel);
        frame4->setFrameShadow(QFrame::Raised);
        gridLayout7 = new QGridLayout(frame4);
        gridLayout7->setSpacing(0);
        gridLayout7->setMargin(0);
        gridLayout7->setObjectName(QString::fromUtf8("gridLayout7"));
        gridLayout8 = new QGridLayout();
        gridLayout8->setSpacing(6);
        gridLayout8->setObjectName(QString::fromUtf8("gridLayout8"));
        gridLayout9 = new QGridLayout();
        gridLayout9->setSpacing(0);
        gridLayout9->setMargin(0);
        gridLayout9->setObjectName(QString::fromUtf8("gridLayout9"));
        textLabel_TarifCCAM = new QLabel(frame4);
        textLabel_TarifCCAM->setObjectName(QString::fromUtf8("textLabel_TarifCCAM"));
        textLabel_TarifCCAM->setMinimumSize(QSize(100, 0));
        QFont font;
        font.setPointSize(13);
        font.setBold(true);
        textLabel_TarifCCAM->setFont(font);
        textLabel_TarifCCAM->setFrameShape(QFrame::Box);
        textLabel_TarifCCAM->setFrameShadow(QFrame::Raised);
        textLabel_TarifCCAM->setAlignment(Qt::AlignJustify|Qt::AlignCenter|Qt::AlignBottom|Qt::AlignTop|Qt::AlignRight|Qt::AlignLeft);
        textLabel_TarifCCAM->setWordWrap(true);

        gridLayout9->addWidget(textLabel_TarifCCAM, 0, 1, 1, 1);

        textLabel1_2 = new QLabel(frame4);
        textLabel1_2->setObjectName(QString::fromUtf8("textLabel1_2"));
        textLabel1_2->setMaximumSize(QSize(32767, 20));
        textLabel1_2->setAlignment(Qt::AlignVCenter|Qt::AlignBottom|Qt::AlignTop|Qt::AlignRight);
        textLabel1_2->setWordWrap(true);

        gridLayout9->addWidget(textLabel1_2, 0, 0, 1, 1);


        gridLayout8->addLayout(gridLayout9, 1, 0, 1, 1);

        gridLayout10 = new QGridLayout();
        gridLayout10->setSpacing(0);
        gridLayout10->setMargin(0);
        gridLayout10->setObjectName(QString::fromUtf8("gridLayout10"));
        textLabel1 = new QLabel(frame4);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        textLabel1->setMaximumSize(QSize(32767, 20));
        textLabel1->setAlignment(Qt::AlignJustify|Qt::AlignCenter|Qt::AlignBottom|Qt::AlignTop|Qt::AlignRight|Qt::AlignLeft);
        textLabel1->setWordWrap(true);

        gridLayout10->addWidget(textLabel1, 0, 0, 1, 1);

        textLabel_CodeCCAM = new QLabel(frame4);
        textLabel_CodeCCAM->setObjectName(QString::fromUtf8("textLabel_CodeCCAM"));
        textLabel_CodeCCAM->setMinimumSize(QSize(100, 0));
        textLabel_CodeCCAM->setFont(font);
        textLabel_CodeCCAM->setFrameShape(QFrame::Box);
        textLabel_CodeCCAM->setFrameShadow(QFrame::Raised);
        textLabel_CodeCCAM->setAlignment(Qt::AlignJustify|Qt::AlignCenter|Qt::AlignBottom|Qt::AlignTop|Qt::AlignRight|Qt::AlignLeft);
        textLabel_CodeCCAM->setWordWrap(true);

        gridLayout10->addWidget(textLabel_CodeCCAM, 0, 1, 1, 1);


        gridLayout8->addLayout(gridLayout10, 0, 0, 1, 1);


        gridLayout7->addLayout(gridLayout8, 0, 0, 1, 1);

        gridLayout11 = new QGridLayout();
        gridLayout11->setSpacing(6);
        gridLayout11->setObjectName(QString::fromUtf8("gridLayout11"));
        textLabel3 = new QLabel(frame4);
        textLabel3->setObjectName(QString::fromUtf8("textLabel3"));
        textLabel3->setMaximumSize(QSize(32767, 20));
        textLabel3->setAlignment(Qt::AlignCenter|Qt::AlignBottom|Qt::AlignTop);
        textLabel3->setWordWrap(true);

        gridLayout11->addWidget(textLabel3, 0, 0, 1, 1);

        textEdit_LibelleActe = new Q3TextEdit(frame4);
        textEdit_LibelleActe->setObjectName(QString::fromUtf8("textEdit_LibelleActe"));
        QSizePolicy sizePolicy6(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(0));
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(textEdit_LibelleActe->sizePolicy().hasHeightForWidth());
        textEdit_LibelleActe->setSizePolicy(sizePolicy6);
        textEdit_LibelleActe->setLineWidth(1);
        textEdit_LibelleActe->setMargin(0);
        textEdit_LibelleActe->setUndoDepth(0);
        textEdit_LibelleActe->setReadOnly(true);
        textEdit_LibelleActe->setUndoRedoEnabled(false);

        gridLayout11->addWidget(textEdit_LibelleActe, 1, 0, 1, 1);


        gridLayout7->addLayout(gridLayout11, 0, 1, 1, 1);


        gridLayout->addWidget(frame4, 2, 0, 1, 1);

        text_Libelle_Modif = new QLabel(Dlg_PatientActeProperty);
        text_Libelle_Modif->setObjectName(QString::fromUtf8("text_Libelle_Modif"));
        text_Libelle_Modif->setFrameShape(QFrame::NoFrame);
        text_Libelle_Modif->setWordWrap(false);

        gridLayout->addWidget(text_Libelle_Modif, 6, 0, 1, 1);

        QWidget::setTabOrder(dateEdit_DateActe, comboBoxPraticien);
        QWidget::setTabOrder(comboBoxPraticien, comboBox_Phase);
        QWidget::setTabOrder(comboBox_Phase, comboBox_Activite);
        QWidget::setTabOrder(comboBox_Activite, combo_Mod01);
        QWidget::setTabOrder(combo_Mod01, combo_Mod02);
        QWidget::setTabOrder(combo_Mod02, combo_Mod03);
        QWidget::setTabOrder(combo_Mod03, combo_Mod04);
        QWidget::setTabOrder(combo_Mod04, pushButtonOK);
        QWidget::setTabOrder(pushButtonOK, pushButtonCancel);
        QWidget::setTabOrder(pushButtonCancel, textEdit_LibelleActe);

        retranslateUi(Dlg_PatientActeProperty);

        QMetaObject::connectSlotsByName(Dlg_PatientActeProperty);
    } // setupUi

    void retranslateUi(QDialog *Dlg_PatientActeProperty)
    {
        Dlg_PatientActeProperty->setWindowTitle(QApplication::translate("Dlg_PatientActeProperty", "Propri\303\251t\303\251 de l'acte", 0, QApplication::UnicodeUTF8));
        pushButtonLastDate->setText(QApplication::translate("Dlg_PatientActeProperty", "Date :", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pushButtonLastDate->setProperty("toolTip", QVariant(QApplication::translate("Dlg_PatientActeProperty", "Place la derni\303\250re date saisie", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
        pushButtonLastDate_Jour->setText(QApplication::translate("Dlg_PatientActeProperty", "J", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pushButtonLastDate_Jour->setProperty("toolTip", QVariant(QApplication::translate("Dlg_PatientActeProperty", "Place la date et l'heure actuelle", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
        textLabel2_2->setText(QApplication::translate("Dlg_PatientActeProperty", "Heure :", 0, QApplication::UnicodeUTF8));
        textLabel4->setText(QApplication::translate("Dlg_PatientActeProperty", "Praticien  :", 0, QApplication::UnicodeUTF8));
        groupBox2->setTitle(QApplication::translate("Dlg_PatientActeProperty", "Phase et activit\303\251", 0, QApplication::UnicodeUTF8));
        comboBox_Phase->clear();
        comboBox_Phase->insertItems(0, QStringList()
         << QApplication::translate("Dlg_PatientActeProperty", "Phase 0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Dlg_PatientActeProperty", "Phase 1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Dlg_PatientActeProperty", "Phase 2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Dlg_PatientActeProperty", "Phase 3", 0, QApplication::UnicodeUTF8)
        );
        comboBox_Activite->clear();
        comboBox_Activite->insertItems(0, QStringList()
         << QApplication::translate("Dlg_PatientActeProperty", "Activit\303\251 1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Dlg_PatientActeProperty", "Activit\303\251 2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Dlg_PatientActeProperty", "Activit\303\251 3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Dlg_PatientActeProperty", "Activit\303\251 4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Dlg_PatientActeProperty", "Activit\303\251 5", 0, QApplication::UnicodeUTF8)
        );
        groupBoxComboMod->setTitle(QApplication::translate("Dlg_PatientActeProperty", "Modificateurs", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        groupBoxComboMod->setProperty("toolTip", QVariant(QString()));
#endif // QT_NO_TOOLTIP
        combo_Mod02->clear();
        combo_Mod02->insertItems(0, QStringList()
         << QApplication::translate("Dlg_PatientActeProperty", "modif 0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Dlg_PatientActeProperty", "modif D", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Dlg_PatientActeProperty", "modif B", 0, QApplication::UnicodeUTF8)
        );
        combo_Mod03->clear();
        combo_Mod03->insertItems(0, QStringList()
         << QApplication::translate("Dlg_PatientActeProperty", "modif 0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Dlg_PatientActeProperty", "modif D", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Dlg_PatientActeProperty", "modif B", 0, QApplication::UnicodeUTF8)
        );
        combo_Mod01->clear();
        combo_Mod01->insertItems(0, QStringList()
         << QApplication::translate("Dlg_PatientActeProperty", "modif 0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Dlg_PatientActeProperty", "modif D", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Dlg_PatientActeProperty", "modif B", 0, QApplication::UnicodeUTF8)
        );
        combo_Mod04->clear();
        combo_Mod04->insertItems(0, QStringList()
         << QApplication::translate("Dlg_PatientActeProperty", "modif 0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Dlg_PatientActeProperty", "modif D", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Dlg_PatientActeProperty", "modif B", 0, QApplication::UnicodeUTF8)
        );
        pushButtonOK->setText(QApplication::translate("Dlg_PatientActeProperty", "&Valider", 0, QApplication::UnicodeUTF8));
        pushButtonOK->setShortcut(QApplication::translate("Dlg_PatientActeProperty", "Alt+V", 0, QApplication::UnicodeUTF8));
        pushButtonCreerNewActe->setText(QApplication::translate("Dlg_PatientActeProperty", "&Cr\303\251er acte Anesth\303\251sie", 0, QApplication::UnicodeUTF8));
        pushButtonCreerNewActe->setShortcut(QApplication::translate("Dlg_PatientActeProperty", "Alt+C", 0, QApplication::UnicodeUTF8));
        pushButtonCancel->setText(QApplication::translate("Dlg_PatientActeProperty", "Annu&ler", 0, QApplication::UnicodeUTF8));
        pushButtonCancel->setShortcut(QApplication::translate("Dlg_PatientActeProperty", "Alt+L", 0, QApplication::UnicodeUTF8));
        listViewCodeAsso->header()->setLabel(0, QApplication::translate("Dlg_PatientActeProperty", "Code CCAM", 0, QApplication::UnicodeUTF8));
        listViewCodeAsso->header()->setLabel(1, QApplication::translate("Dlg_PatientActeProperty", "Activit\303\251", 0, QApplication::UnicodeUTF8));
        listViewCodeAsso->header()->setLabel(2, QApplication::translate("Dlg_PatientActeProperty", "Tarif de base", 0, QApplication::UnicodeUTF8));
        listViewCodeAsso->header()->setLabel(3, QApplication::translate("Dlg_PatientActeProperty", "Type", 0, QApplication::UnicodeUTF8));
        listViewCodeAsso->header()->setLabel(4, QApplication::translate("Dlg_PatientActeProperty", "Libell\303\251", 0, QApplication::UnicodeUTF8));
        listViewCodeAsso->header()->setLabel(5, QApplication::translate("Dlg_PatientActeProperty", "R\303\250gle association", 0, QApplication::UnicodeUTF8));
        textLabelAsso->setText(QApplication::translate("Dlg_PatientActeProperty", "Actes associ\303\251s et compl\303\251mentaires", 0, QApplication::UnicodeUTF8));
        textLabel_TarifCCAM->setText(QApplication::translate("Dlg_PatientActeProperty", ". . . . . . . . . . .", 0, QApplication::UnicodeUTF8));
        textLabel1_2->setText(QApplication::translate("Dlg_PatientActeProperty", "Tarif :", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("Dlg_PatientActeProperty", "Code CCAM :", 0, QApplication::UnicodeUTF8));
        textLabel_CodeCCAM->setText(QApplication::translate("Dlg_PatientActeProperty", ". . . . . . . . . . .", 0, QApplication::UnicodeUTF8));
        textLabel3->setText(QApplication::translate("Dlg_PatientActeProperty", "Libell\303\251 de l'acte :", 0, QApplication::UnicodeUTF8));
        text_Libelle_Modif->setText(QString());
        Q_UNUSED(Dlg_PatientActeProperty);
    } // retranslateUi

};

namespace Ui {
    class Dlg_PatientActeProperty: public Ui_Dlg_PatientActeProperty {};
} // namespace Ui

QT_END_NAMESPACE

class Dlg_PatientActeProperty : public QDialog, public Ui::Dlg_PatientActeProperty
{
    Q_OBJECT

public:
    Dlg_PatientActeProperty(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~Dlg_PatientActeProperty();

protected slots:
    virtual void languageChange();

};

#endif // DLG_PATIENTACTEPROPERTY_H

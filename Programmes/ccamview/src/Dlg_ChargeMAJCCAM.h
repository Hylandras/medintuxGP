#ifndef DLG_CHARGEMAJCCAM_H
#define DLG_CHARGEMAJCCAM_H

#include <qvariant.h>


#include <Qt3Support/Q3Header>
#include <Qt3Support/Q3ListView>
#include <Qt3Support/Q3MimeSourceFactory>
#include <Qt3Support/Q3ProgressBar>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_Dlg_ChargeMAJCCAM
{
public:
    QGridLayout *gridLayout;
    Q3ListView *listview_Affichage;
    QHBoxLayout *hboxLayout;
    QPushButton *pushButton_Integrer;
    QSpacerItem *spacer1;
    QPushButton *pushButton_Annuler;
    QHBoxLayout *hboxLayout1;
    QLabel *EtiqBarreProg;
    Q3ProgressBar *BarreProgression;

    void setupUi(QDialog *Dlg_ChargeMAJCCAM)
    {
        if (Dlg_ChargeMAJCCAM->objectName().isEmpty())
            Dlg_ChargeMAJCCAM->setObjectName(QString::fromUtf8("Dlg_ChargeMAJCCAM"));
        Dlg_ChargeMAJCCAM->resize(525, 541);
        Dlg_ChargeMAJCCAM->setSizeGripEnabled(true);
        gridLayout = new QGridLayout(Dlg_ChargeMAJCCAM);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        listview_Affichage = new Q3ListView(Dlg_ChargeMAJCCAM);
        listview_Affichage->addColumn(QApplication::translate("Dlg_ChargeMAJCCAM", "var name", 0, QApplication::UnicodeUTF8));
        listview_Affichage->header()->setClickEnabled(true, listview_Affichage->header()->count() - 1);
        listview_Affichage->header()->setResizeEnabled(true, listview_Affichage->header()->count() - 1);
        listview_Affichage->addColumn(QApplication::translate("Dlg_ChargeMAJCCAM", "nom donnee", 0, QApplication::UnicodeUTF8));
        listview_Affichage->header()->setClickEnabled(true, listview_Affichage->header()->count() - 1);
        listview_Affichage->header()->setResizeEnabled(true, listview_Affichage->header()->count() - 1);
        listview_Affichage->addColumn(QApplication::translate("Dlg_ChargeMAJCCAM", "type donnee", 0, QApplication::UnicodeUTF8));
        listview_Affichage->header()->setClickEnabled(true, listview_Affichage->header()->count() - 1);
        listview_Affichage->header()->setResizeEnabled(true, listview_Affichage->header()->count() - 1);
        listview_Affichage->addColumn(QApplication::translate("Dlg_ChargeMAJCCAM", "debut", 0, QApplication::UnicodeUTF8));
        listview_Affichage->header()->setClickEnabled(true, listview_Affichage->header()->count() - 1);
        listview_Affichage->header()->setResizeEnabled(true, listview_Affichage->header()->count() - 1);
        listview_Affichage->addColumn(QApplication::translate("Dlg_ChargeMAJCCAM", "longueur", 0, QApplication::UnicodeUTF8));
        listview_Affichage->header()->setClickEnabled(true, listview_Affichage->header()->count() - 1);
        listview_Affichage->header()->setResizeEnabled(true, listview_Affichage->header()->count() - 1);
        listview_Affichage->addColumn(QApplication::translate("Dlg_ChargeMAJCCAM", "Contenu fichier", 0, QApplication::UnicodeUTF8));
        listview_Affichage->header()->setClickEnabled(true, listview_Affichage->header()->count() - 1);
        listview_Affichage->header()->setResizeEnabled(true, listview_Affichage->header()->count() - 1);
        listview_Affichage->setObjectName(QString::fromUtf8("listview_Affichage"));
        listview_Affichage->setLineWidth(1);
        listview_Affichage->setAllColumnsShowFocus(true);
        listview_Affichage->setShowSortIndicator(true);

        gridLayout->addWidget(listview_Affichage, 0, 0, 1, 1);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        pushButton_Integrer = new QPushButton(Dlg_ChargeMAJCCAM);
        pushButton_Integrer->setObjectName(QString::fromUtf8("pushButton_Integrer"));

        hboxLayout->addWidget(pushButton_Integrer);

        spacer1 = new QSpacerItem(26, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer1);

        pushButton_Annuler = new QPushButton(Dlg_ChargeMAJCCAM);
        pushButton_Annuler->setObjectName(QString::fromUtf8("pushButton_Annuler"));

        hboxLayout->addWidget(pushButton_Annuler);


        gridLayout->addLayout(hboxLayout, 1, 0, 1, 1);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        EtiqBarreProg = new QLabel(Dlg_ChargeMAJCCAM);
        EtiqBarreProg->setObjectName(QString::fromUtf8("EtiqBarreProg"));
        EtiqBarreProg->setWordWrap(false);

        hboxLayout1->addWidget(EtiqBarreProg);

        BarreProgression = new Q3ProgressBar(Dlg_ChargeMAJCCAM);
        BarreProgression->setObjectName(QString::fromUtf8("BarreProgression"));
        BarreProgression->setMinimumSize(QSize(0, 12));
        BarreProgression->setMaximumSize(QSize(32767, 12));

        hboxLayout1->addWidget(BarreProgression);


        gridLayout->addLayout(hboxLayout1, 2, 0, 1, 1);


        retranslateUi(Dlg_ChargeMAJCCAM);

        QMetaObject::connectSlotsByName(Dlg_ChargeMAJCCAM);
    } // setupUi

    void retranslateUi(QDialog *Dlg_ChargeMAJCCAM)
    {
        Dlg_ChargeMAJCCAM->setWindowTitle(QApplication::translate("Dlg_ChargeMAJCCAM", "Mise \303\240 jour de la Base CCAM", 0, QApplication::UnicodeUTF8));
        listview_Affichage->header()->setLabel(0, QApplication::translate("Dlg_ChargeMAJCCAM", "var name", 0, QApplication::UnicodeUTF8));
        listview_Affichage->header()->setLabel(1, QApplication::translate("Dlg_ChargeMAJCCAM", "nom donnee", 0, QApplication::UnicodeUTF8));
        listview_Affichage->header()->setLabel(2, QApplication::translate("Dlg_ChargeMAJCCAM", "type donnee", 0, QApplication::UnicodeUTF8));
        listview_Affichage->header()->setLabel(3, QApplication::translate("Dlg_ChargeMAJCCAM", "debut", 0, QApplication::UnicodeUTF8));
        listview_Affichage->header()->setLabel(4, QApplication::translate("Dlg_ChargeMAJCCAM", "longueur", 0, QApplication::UnicodeUTF8));
        listview_Affichage->header()->setLabel(5, QApplication::translate("Dlg_ChargeMAJCCAM", "Contenu fichier", 0, QApplication::UnicodeUTF8));
        pushButton_Integrer->setText(QApplication::translate("Dlg_ChargeMAJCCAM", "Int\303\251grer le fichier", 0, QApplication::UnicodeUTF8));
        pushButton_Annuler->setText(QApplication::translate("Dlg_ChargeMAJCCAM", "Annuler", 0, QApplication::UnicodeUTF8));
        EtiqBarreProg->setText(QApplication::translate("Dlg_ChargeMAJCCAM", "Travail en cours", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Dlg_ChargeMAJCCAM);
    } // retranslateUi

};

namespace Ui {
    class Dlg_ChargeMAJCCAM: public Ui_Dlg_ChargeMAJCCAM {};
} // namespace Ui

QT_END_NAMESPACE

class Dlg_ChargeMAJCCAM : public QDialog, public Ui::Dlg_ChargeMAJCCAM
{
    Q_OBJECT

public:
    Dlg_ChargeMAJCCAM(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~Dlg_ChargeMAJCCAM();

protected slots:
    virtual void languageChange();

};

#endif // DLG_CHARGEMAJCCAM_H

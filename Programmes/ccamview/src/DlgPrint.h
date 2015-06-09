#ifndef DLGPRINT_H
#define DLGPRINT_H

#include <qvariant.h>


#include <Qt3Support/Q3Frame>
#include <Qt3Support/Q3GroupBox>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>

QT_BEGIN_NAMESPACE

class Ui_DlgPrint
{
public:
    Q3Frame *frameToolBar;
    Q3GroupBox *groupBox2;
    QLineEdit *lineEdit_PageFin;
    QLineEdit *lineEdit_PageDeb;
    QLineEdit *lineEditZoom;
    QPushButton *pushButtonPrint;
    QPushButton *pushButtonConfigurer;
    QLabel *textLabel1__2_2_3_3;
    QLabel *textLabel1__2_2_3_3_2;
    QLabel *textLabel_2_2;
    QLabel *textLabel_2;
    QPushButton *pushButtonQuitter;
    Q3GroupBox *groupBox1;
    Q3Frame *frame6;
    QLineEdit *lineEdit_Head_H;
    QLabel *textLabel1__2;
    QSlider *slider_Head_H;
    Q3Frame *frame7;
    QLineEdit *lineEdit_Marg_Left;
    QLabel *textLabel1__2_2_3;
    QSlider *slider_Marg_Left;
    Q3Frame *frame9;
    QLineEdit *lineEdit_Marg_Right;
    QLabel *textLabel1__2_2_3_2;
    QSlider *slider_Marg_Right;
    Q3Frame *frame11;
    QLineEdit *lineEdit_Marg_Bottom;
    QLabel *textLabel1__2_2_2;
    QSlider *slider_Marg_Bottom;
    Q3Frame *frame10;
    QLineEdit *lineEdit_Foot_H;
    QLabel *textLabel1__2_2;
    QSlider *slider_Foot_H;
    QLineEdit *lineEditResoX;
    QLineEdit *lineEditResoY;
    QPushButton *pushButtonSave;
    QLabel *textLabel1__2_2_2_2_3_2;
    QLabel *textLabel1__2_2_2_2_3;
    QLabel *textLabel1__2_2_2_2;
    QLabel *textLabel1__2_2_2_2_2;
    Q3Frame *frame4;
    QLineEdit *lineEdit_Marg_Top;
    QLabel *textLabel1_;
    QSlider *slider_Marg_Top;
    QPushButton *pushButtonNext;
    QPushButton *pushButtonPrevious;
    QLabel *textLabel_NbPage;

    void setupUi(QDialog *DlgPrint)
    {
        if (DlgPrint->objectName().isEmpty())
            DlgPrint->setObjectName(QString::fromUtf8("DlgPrint"));
        DlgPrint->resize(673, 611);
        frameToolBar = new Q3Frame(DlgPrint);
        frameToolBar->setObjectName(QString::fromUtf8("frameToolBar"));
        frameToolBar->setGeometry(QRect(450, 7, 220, 600));
        frameToolBar->setFrameShape(QFrame::StyledPanel);
        frameToolBar->setFrameShadow(QFrame::Raised);
        groupBox2 = new Q3GroupBox(frameToolBar);
        groupBox2->setObjectName(QString::fromUtf8("groupBox2"));
        groupBox2->setGeometry(QRect(0, 410, 210, 135));
        lineEdit_PageFin = new QLineEdit(groupBox2);
        lineEdit_PageFin->setObjectName(QString::fromUtf8("lineEdit_PageFin"));
        lineEdit_PageFin->setGeometry(QRect(160, 105, 35, 21));
        lineEdit_PageDeb = new QLineEdit(groupBox2);
        lineEdit_PageDeb->setObjectName(QString::fromUtf8("lineEdit_PageDeb"));
        lineEdit_PageDeb->setGeometry(QRect(100, 105, 35, 21));
        lineEditZoom = new QLineEdit(groupBox2);
        lineEditZoom->setObjectName(QString::fromUtf8("lineEditZoom"));
        lineEditZoom->setGeometry(QRect(75, 40, 70, 22));
        pushButtonPrint = new QPushButton(groupBox2);
        pushButtonPrint->setObjectName(QString::fromUtf8("pushButtonPrint"));
        pushButtonPrint->setGeometry(QRect(15, 65, 180, 35));
        pushButtonConfigurer = new QPushButton(groupBox2);
        pushButtonConfigurer->setObjectName(QString::fromUtf8("pushButtonConfigurer"));
        pushButtonConfigurer->setGeometry(QRect(20, 15, 170, 20));
        textLabel1__2_2_3_3 = new QLabel(groupBox2);
        textLabel1__2_2_3_3->setObjectName(QString::fromUtf8("textLabel1__2_2_3_3"));
        textLabel1__2_2_3_3->setGeometry(QRect(27, 40, 45, 21));
        textLabel1__2_2_3_3->setWordWrap(false);
        textLabel1__2_2_3_3_2 = new QLabel(groupBox2);
        textLabel1__2_2_3_3_2->setObjectName(QString::fromUtf8("textLabel1__2_2_3_3_2"));
        textLabel1__2_2_3_3_2->setGeometry(QRect(155, 40, 30, 21));
        textLabel1__2_2_3_3_2->setWordWrap(false);
        textLabel_2_2 = new QLabel(groupBox2);
        textLabel_2_2->setObjectName(QString::fromUtf8("textLabel_2_2"));
        textLabel_2_2->setGeometry(QRect(140, 100, 20, 20));
        textLabel_2_2->setWordWrap(false);
        textLabel_2 = new QLabel(groupBox2);
        textLabel_2->setObjectName(QString::fromUtf8("textLabel_2"));
        textLabel_2->setGeometry(QRect(10, 100, 85, 20));
        textLabel_2->setWordWrap(false);
        pushButtonQuitter = new QPushButton(frameToolBar);
        pushButtonQuitter->setObjectName(QString::fromUtf8("pushButtonQuitter"));
        pushButtonQuitter->setGeometry(QRect(5, 555, 210, 35));
        groupBox1 = new Q3GroupBox(frameToolBar);
        groupBox1->setObjectName(QString::fromUtf8("groupBox1"));
        groupBox1->setGeometry(QRect(5, 45, 210, 360));
        frame6 = new Q3Frame(groupBox1);
        frame6->setObjectName(QString::fromUtf8("frame6"));
        frame6->setGeometry(QRect(10, 75, 190, 45));
        frame6->setFrameShape(QFrame::StyledPanel);
        frame6->setFrameShadow(QFrame::Raised);
        lineEdit_Head_H = new QLineEdit(frame6);
        lineEdit_Head_H->setObjectName(QString::fromUtf8("lineEdit_Head_H"));
        lineEdit_Head_H->setGeometry(QRect(120, 5, 66, 21));
        textLabel1__2 = new QLabel(frame6);
        textLabel1__2->setObjectName(QString::fromUtf8("textLabel1__2"));
        textLabel1__2->setGeometry(QRect(0, 0, 110, 17));
        textLabel1__2->setWordWrap(false);
        slider_Head_H = new QSlider(frame6);
        slider_Head_H->setObjectName(QString::fromUtf8("slider_Head_H"));
        slider_Head_H->setGeometry(QRect(5, 25, 180, 16));
        slider_Head_H->setOrientation(Qt::Horizontal);
        frame7 = new Q3Frame(groupBox1);
        frame7->setObjectName(QString::fromUtf8("frame7"));
        frame7->setGeometry(QRect(10, 125, 190, 45));
        frame7->setFrameShape(QFrame::StyledPanel);
        frame7->setFrameShadow(QFrame::Raised);
        lineEdit_Marg_Left = new QLineEdit(frame7);
        lineEdit_Marg_Left->setObjectName(QString::fromUtf8("lineEdit_Marg_Left"));
        lineEdit_Marg_Left->setGeometry(QRect(120, 5, 66, 21));
        textLabel1__2_2_3 = new QLabel(frame7);
        textLabel1__2_2_3->setObjectName(QString::fromUtf8("textLabel1__2_2_3"));
        textLabel1__2_2_3->setGeometry(QRect(5, 5, 110, 21));
        textLabel1__2_2_3->setWordWrap(false);
        slider_Marg_Left = new QSlider(frame7);
        slider_Marg_Left->setObjectName(QString::fromUtf8("slider_Marg_Left"));
        slider_Marg_Left->setGeometry(QRect(5, 25, 180, 16));
        slider_Marg_Left->setOrientation(Qt::Horizontal);
        frame9 = new Q3Frame(groupBox1);
        frame9->setObjectName(QString::fromUtf8("frame9"));
        frame9->setGeometry(QRect(10, 175, 190, 45));
        frame9->setFrameShape(QFrame::StyledPanel);
        frame9->setFrameShadow(QFrame::Raised);
        lineEdit_Marg_Right = new QLineEdit(frame9);
        lineEdit_Marg_Right->setObjectName(QString::fromUtf8("lineEdit_Marg_Right"));
        lineEdit_Marg_Right->setGeometry(QRect(120, 5, 66, 21));
        textLabel1__2_2_3_2 = new QLabel(frame9);
        textLabel1__2_2_3_2->setObjectName(QString::fromUtf8("textLabel1__2_2_3_2"));
        textLabel1__2_2_3_2->setGeometry(QRect(5, 5, 110, 21));
        textLabel1__2_2_3_2->setWordWrap(false);
        slider_Marg_Right = new QSlider(frame9);
        slider_Marg_Right->setObjectName(QString::fromUtf8("slider_Marg_Right"));
        slider_Marg_Right->setGeometry(QRect(5, 25, 180, 16));
        slider_Marg_Right->setOrientation(Qt::Horizontal);
        frame11 = new Q3Frame(groupBox1);
        frame11->setObjectName(QString::fromUtf8("frame11"));
        frame11->setGeometry(QRect(10, 275, 190, 45));
        frame11->setFrameShape(QFrame::StyledPanel);
        frame11->setFrameShadow(QFrame::Raised);
        lineEdit_Marg_Bottom = new QLineEdit(frame11);
        lineEdit_Marg_Bottom->setObjectName(QString::fromUtf8("lineEdit_Marg_Bottom"));
        lineEdit_Marg_Bottom->setGeometry(QRect(120, 5, 66, 21));
        textLabel1__2_2_2 = new QLabel(frame11);
        textLabel1__2_2_2->setObjectName(QString::fromUtf8("textLabel1__2_2_2"));
        textLabel1__2_2_2->setGeometry(QRect(5, 5, 110, 21));
        textLabel1__2_2_2->setWordWrap(false);
        slider_Marg_Bottom = new QSlider(frame11);
        slider_Marg_Bottom->setObjectName(QString::fromUtf8("slider_Marg_Bottom"));
        slider_Marg_Bottom->setGeometry(QRect(5, 25, 180, 16));
        slider_Marg_Bottom->setOrientation(Qt::Horizontal);
        frame10 = new Q3Frame(groupBox1);
        frame10->setObjectName(QString::fromUtf8("frame10"));
        frame10->setGeometry(QRect(10, 225, 190, 45));
        frame10->setFrameShape(QFrame::StyledPanel);
        frame10->setFrameShadow(QFrame::Raised);
        lineEdit_Foot_H = new QLineEdit(frame10);
        lineEdit_Foot_H->setObjectName(QString::fromUtf8("lineEdit_Foot_H"));
        lineEdit_Foot_H->setGeometry(QRect(120, 5, 66, 21));
        textLabel1__2_2 = new QLabel(frame10);
        textLabel1__2_2->setObjectName(QString::fromUtf8("textLabel1__2_2"));
        textLabel1__2_2->setGeometry(QRect(5, 5, 110, 17));
        textLabel1__2_2->setWordWrap(false);
        slider_Foot_H = new QSlider(frame10);
        slider_Foot_H->setObjectName(QString::fromUtf8("slider_Foot_H"));
        slider_Foot_H->setGeometry(QRect(5, 25, 180, 16));
        slider_Foot_H->setOrientation(Qt::Horizontal);
        lineEditResoX = new QLineEdit(groupBox1);
        lineEditResoX->setObjectName(QString::fromUtf8("lineEditResoX"));
        lineEditResoX->setGeometry(QRect(130, 323, 40, 17));
        QFont font;
        font.setPointSize(7);
        lineEditResoX->setFont(font);
        lineEditResoY = new QLineEdit(groupBox1);
        lineEditResoY->setObjectName(QString::fromUtf8("lineEditResoY"));
        lineEditResoY->setGeometry(QRect(130, 340, 40, 17));
        lineEditResoY->setFont(font);
        pushButtonSave = new QPushButton(groupBox1);
        pushButtonSave->setObjectName(QString::fromUtf8("pushButtonSave"));
        pushButtonSave->setGeometry(QRect(7, 325, 30, 30));
        pushButtonSave->setMaximumSize(QSize(32, 32767));
        const QIcon icon = qt_get_icon(image0_ID);
        pushButtonSave->setIcon(icon);
        textLabel1__2_2_2_2_3_2 = new QLabel(groupBox1);
        textLabel1__2_2_2_2_3_2->setObjectName(QString::fromUtf8("textLabel1__2_2_2_2_3_2"));
        textLabel1__2_2_2_2_3_2->setGeometry(QRect(105, 325, 20, 17));
        textLabel1__2_2_2_2_3_2->setFont(font);
        textLabel1__2_2_2_2_3_2->setAlignment(Qt::AlignJustify|Qt::AlignCenter|Qt::AlignBottom|Qt::AlignTop|Qt::AlignRight|Qt::AlignLeft);
        textLabel1__2_2_2_2_3_2->setWordWrap(true);
        textLabel1__2_2_2_2_3 = new QLabel(groupBox1);
        textLabel1__2_2_2_2_3->setObjectName(QString::fromUtf8("textLabel1__2_2_2_2_3"));
        textLabel1__2_2_2_2_3->setGeometry(QRect(105, 340, 20, 17));
        textLabel1__2_2_2_2_3->setFont(font);
        textLabel1__2_2_2_2_3->setAlignment(Qt::AlignJustify|Qt::AlignCenter|Qt::AlignBottom|Qt::AlignTop|Qt::AlignRight|Qt::AlignLeft);
        textLabel1__2_2_2_2_3->setWordWrap(true);
        textLabel1__2_2_2_2 = new QLabel(groupBox1);
        textLabel1__2_2_2_2->setObjectName(QString::fromUtf8("textLabel1__2_2_2_2"));
        textLabel1__2_2_2_2->setGeometry(QRect(50, 325, 55, 30));
        textLabel1__2_2_2_2->setFont(font);
        textLabel1__2_2_2_2->setAlignment(Qt::AlignJustify|Qt::AlignCenter|Qt::AlignBottom|Qt::AlignTop|Qt::AlignRight|Qt::AlignLeft);
        textLabel1__2_2_2_2->setWordWrap(true);
        textLabel1__2_2_2_2_2 = new QLabel(groupBox1);
        textLabel1__2_2_2_2_2->setObjectName(QString::fromUtf8("textLabel1__2_2_2_2_2"));
        textLabel1__2_2_2_2_2->setGeometry(QRect(175, 325, 25, 30));
        textLabel1__2_2_2_2_2->setFont(font);
        textLabel1__2_2_2_2_2->setAlignment(Qt::AlignJustify|Qt::AlignCenter|Qt::AlignBottom|Qt::AlignTop|Qt::AlignRight|Qt::AlignLeft);
        textLabel1__2_2_2_2_2->setWordWrap(true);
        frame4 = new Q3Frame(groupBox1);
        frame4->setObjectName(QString::fromUtf8("frame4"));
        frame4->setGeometry(QRect(10, 25, 190, 45));
        frame4->setFrameShape(QFrame::StyledPanel);
        frame4->setFrameShadow(QFrame::Raised);
        lineEdit_Marg_Top = new QLineEdit(frame4);
        lineEdit_Marg_Top->setObjectName(QString::fromUtf8("lineEdit_Marg_Top"));
        lineEdit_Marg_Top->setGeometry(QRect(115, 5, 70, 21));
        textLabel1_ = new QLabel(frame4);
        textLabel1_->setObjectName(QString::fromUtf8("textLabel1_"));
        textLabel1_->setGeometry(QRect(5, 4, 100, 21));
        textLabel1_->setWordWrap(false);
        slider_Marg_Top = new QSlider(frame4);
        slider_Marg_Top->setObjectName(QString::fromUtf8("slider_Marg_Top"));
        slider_Marg_Top->setGeometry(QRect(5, 26, 180, 16));
        QFont font1;
        slider_Marg_Top->setFont(font1);
        slider_Marg_Top->setOrientation(Qt::Horizontal);
        slider_Marg_Top->setTickPosition(QSlider::NoTicks);
        pushButtonNext = new QPushButton(frameToolBar);
        pushButtonNext->setObjectName(QString::fromUtf8("pushButtonNext"));
        pushButtonNext->setGeometry(QRect(180, 5, 36, 35));
        const QIcon icon1 = qt_get_icon(image1_ID);
        pushButtonNext->setIcon(icon1);
        pushButtonPrevious = new QPushButton(frameToolBar);
        pushButtonPrevious->setObjectName(QString::fromUtf8("pushButtonPrevious"));
        pushButtonPrevious->setGeometry(QRect(5, 5, 36, 35));
        const QIcon icon2 = qt_get_icon(image2_ID);
        pushButtonPrevious->setIcon(icon2);
        textLabel_NbPage = new QLabel(frameToolBar);
        textLabel_NbPage->setObjectName(QString::fromUtf8("textLabel_NbPage"));
        textLabel_NbPage->setGeometry(QRect(45, 10, 135, 25));
        textLabel_NbPage->setFrameShape(QFrame::NoFrame);
        textLabel_NbPage->setAlignment(Qt::AlignCenter);
        textLabel_NbPage->setWordWrap(true);
#ifndef QT_NO_SHORTCUT
        textLabel1__2_2_3_3->setBuddy(lineEditZoom);
        textLabel_2_2->setBuddy(lineEdit_PageFin);
        textLabel_2->setBuddy(lineEdit_PageDeb);
        textLabel1__2->setBuddy(lineEdit_Head_H);
        textLabel1__2_2_3->setBuddy(lineEdit_Marg_Left);
        textLabel1__2_2_3_2->setBuddy(lineEdit_Marg_Right);
        textLabel1__2_2_2->setBuddy(lineEdit_Marg_Bottom);
        textLabel1__2_2->setBuddy(lineEdit_Foot_H);
        textLabel1__2_2_2_2_3_2->setBuddy(lineEditResoX);
        textLabel1__2_2_2_2_3->setBuddy(lineEditResoY);
        textLabel1_->setBuddy(lineEdit_Marg_Top);
#endif // QT_NO_SHORTCUT

        retranslateUi(DlgPrint);

        QMetaObject::connectSlotsByName(DlgPrint);
    } // setupUi

    void retranslateUi(QDialog *DlgPrint)
    {
        DlgPrint->setWindowTitle(QApplication::translate("DlgPrint", "Medintux - Impression", 0, QApplication::UnicodeUTF8));
        groupBox2->setTitle(QApplication::translate("DlgPrint", "Impression", 0, QApplication::UnicodeUTF8));
        lineEdit_PageFin->setText(QString());
        lineEdit_PageDeb->setText(QString());
        pushButtonPrint->setText(QApplication::translate("DlgPrint", "&Imprimer", 0, QApplication::UnicodeUTF8));
        pushButtonPrint->setShortcut(QApplication::translate("DlgPrint", "Alt+I", 0, QApplication::UnicodeUTF8));
        pushButtonConfigurer->setText(QApplication::translate("DlgPrint", "&Configurer", 0, QApplication::UnicodeUTF8));
        pushButtonConfigurer->setShortcut(QApplication::translate("DlgPrint", "Alt+C", 0, QApplication::UnicodeUTF8));
        textLabel1__2_2_3_3->setText(QApplication::translate("DlgPrint", "T&aille", 0, QApplication::UnicodeUTF8));
        textLabel1__2_2_3_3_2->setText(QApplication::translate("DlgPrint", "%", 0, QApplication::UnicodeUTF8));
        textLabel_2_2->setText(QApplication::translate("DlgPrint", "&\303\240 :", 0, QApplication::UnicodeUTF8));
        textLabel_2->setText(QApplication::translate("DlgPrint", "De &la page :", 0, QApplication::UnicodeUTF8));
        pushButtonQuitter->setText(QApplication::translate("DlgPrint", "&Quitter", 0, QApplication::UnicodeUTF8));
        pushButtonQuitter->setShortcut(QApplication::translate("DlgPrint", "Alt+Q", 0, QApplication::UnicodeUTF8));
        groupBox1->setTitle(QApplication::translate("DlgPrint", "Mise en page", 0, QApplication::UnicodeUTF8));
        textLabel1__2->setText(QApplication::translate("DlgPrint", "Hauteur &en-t\303\252te", 0, QApplication::UnicodeUTF8));
        textLabel1__2_2_3->setText(QApplication::translate("DlgPrint", "Marge &gauche", 0, QApplication::UnicodeUTF8));
        textLabel1__2_2_3_2->setText(QApplication::translate("DlgPrint", "Marge &droite", 0, QApplication::UnicodeUTF8));
        textLabel1__2_2_2->setText(QApplication::translate("DlgPrint", "Marge &basse", 0, QApplication::UnicodeUTF8));
        textLabel1__2_2->setText(QApplication::translate("DlgPrint", "Hauteur &Pied", 0, QApplication::UnicodeUTF8));
        lineEditResoX->setText(QApplication::translate("DlgPrint", "96", 0, QApplication::UnicodeUTF8));
        lineEditResoY->setText(QApplication::translate("DlgPrint", "96", 0, QApplication::UnicodeUTF8));
        pushButtonSave->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushButtonSave->setProperty("toolTip", QVariant(QApplication::translate("DlgPrint", "Permet de sauver les modifications des param\303\250tres d'impression", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        pushButtonSave->setProperty("whatsThis", QVariant(QApplication::translate("DlgPrint", "Permet de sauver les modifications", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        textLabel1__2_2_2_2_3_2->setText(QApplication::translate("DlgPrint", "&X", 0, QApplication::UnicodeUTF8));
        textLabel1__2_2_2_2_3->setText(QApplication::translate("DlgPrint", "&Y", 0, QApplication::UnicodeUTF8));
        textLabel1__2_2_2_2->setText(QApplication::translate("DlgPrint", "R\303\251solution  imprimante", 0, QApplication::UnicodeUTF8));
        textLabel1__2_2_2_2_2->setText(QApplication::translate("DlgPrint", "DPI", 0, QApplication::UnicodeUTF8));
        textLabel1_->setText(QApplication::translate("DlgPrint", "Marge &haute", 0, QApplication::UnicodeUTF8));
        pushButtonNext->setText(QString());
        pushButtonPrevious->setText(QString());
        textLabel_NbPage->setText(QApplication::translate("DlgPrint", "Nombre de pages", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(DlgPrint);
    } // retranslateUi


protected:
    enum IconID
    {
        image0_ID,
        image1_ID,
        image2_ID,
        unknown_ID
    };
    static QPixmap qt_get_icon(IconID id)
    {
    static const unsigned char image0_data[] = { 
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x16,
    0x08, 0x06, 0x00, 0x00, 0x00, 0xc4, 0xb4, 0x6c, 0x3b, 0x00, 0x00, 0x01,
    0x67, 0x49, 0x44, 0x41, 0x54, 0x38, 0x8d, 0xb5, 0x95, 0x3f, 0x4b, 0xc3,
    0x50, 0x14, 0xc5, 0xcf, 0x7b, 0xb5, 0x24, 0x94, 0x74, 0xca, 0x54, 0x03,
    0xc2, 0x5b, 0x3b, 0x39, 0x67, 0xe9, 0xe8, 0xa2, 0x1f, 0xc0, 0x7c, 0x06,
    0x07, 0xa7, 0x6a, 0xa0, 0x88, 0x83, 0x12, 0x9c, 0x84, 0x4e, 0x9d, 0xdc,
    0x9d, 0x44, 0x71, 0x70, 0xe9, 0x1a, 0xe8, 0x6c, 0x75, 0x70, 0x51, 0x44,
    0x9c, 0x2c, 0x14, 0x3a, 0x3d, 0x49, 0x73, 0x1d, 0x8a, 0x31, 0xff, 0x6d,
    0x9a, 0xe6, 0x42, 0x08, 0xf7, 0x9e, 0xfb, 0x7e, 0x39, 0x79, 0x97, 0xbc,
    0x30, 0xd7, 0x75, 0x51, 0x45, 0x6c, 0xc4, 0x0b, 0xa6, 0x69, 0xd2, 0x2a,
    0x20, 0xd7, 0x75, 0x59, 0x38, 0xe7, 0xe1, 0xe4, 0xe1, 0xe9, 0x98, 0x2e,
    0xee, 0xda, 0x20, 0xa2, 0x42, 0xd7, 0xf9, 0x8d, 0xc0, 0x60, 0x18, 0x35,
    0x14, 0x38, 0xee, 0xf5, 0x7a, 0xb4, 0x73, 0xf8, 0x85, 0xdb, 0xab, 0x17,
    0x74, 0xf7, 0x00, 0xdb, 0xb6, 0x33, 0xdd, 0x39, 0x8e, 0x13, 0xe8, 0x8e,
    0xe3, 0x60, 0x26, 0x5f, 0xd1, 0x68, 0x2e, 0xde, 0xf6, 0xd7, 0x79, 0x00,
    0x96, 0x52, 0x62, 0xee, 0x4b, 0x10, 0x79, 0xc1, 0x82, 0xbc, 0x88, 0xeb,
    0xbe, 0x1f, 0xd5, 0x13, 0x7b, 0xdc, 0xd9, 0x5f, 0xdc, 0x8b, 0x38, 0x4e,
    0x8b, 0x04, 0x38, 0xcb, 0x51, 0x51, 0x3d, 0x13, 0x1c, 0x76, 0x1c, 0x76,
    0x18, 0xcf, 0x4b, 0x3b, 0xfe, 0x2f, 0x8f, 0x07, 0xcf, 0x55, 0x4b, 0x44,
    0xaa, 0x63, 0xfb, 0x9a, 0xa5, 0x95, 0xcb, 0x83, 0xef, 0x2f, 0x75, 0x8c,
    0x47, 0x93, 0xa5, 0x00, 0x5b, 0x6d, 0xc0, 0x3a, 0x59, 0x12, 0x3c, 0x1e,
    0x4d, 0xb0, 0x6d, 0x6e, 0x62, 0x3a, 0xfb, 0x4c, 0x68, 0xbb, 0x9d, 0x03,
    0xb4, 0x5a, 0x2d, 0x08, 0x21, 0x70, 0x74, 0x66, 0xe1, 0xfd, 0x39, 0xfd,
    0x81, 0x99, 0x7b, 0x9c, 0x06, 0x05, 0x00, 0x45, 0x51, 0x60, 0x18, 0x06,
    0x34, 0x4d, 0x43, 0xad, 0x96, 0xb5, 0x7a, 0x85, 0xe1, 0x11, 0x11, 0x54,
    0x55, 0x45, 0xbd, 0x5e, 0xcf, 0xed, 0x2b, 0x0c, 0xd6, 0x75, 0x1d, 0x9c,
    0x73, 0x30, 0x96, 0x3f, 0xe0, 0x00, 0x2c, 0xa5, 0x04, 0x91, 0x9f, 0xd7,
    0x0b, 0x00, 0x10, 0x42, 0x40, 0x55, 0x55, 0x70, 0xfe, 0xe7, 0x69, 0xee,
    0x01, 0xde, 0x77, 0xb4, 0x2f, 0x18, 0x5e, 0xbf, 0xdf, 0x67, 0x83, 0xa1,
    0x49, 0x8d, 0xe6, 0x22, 0xf7, 0x64, 0x3a, 0xb8, 0x7b, 0x6a, 0x25, 0x6a,
    0x93, 0x0f, 0xe0, 0xed, 0x31, 0x7a, 0x26, 0xb3, 0xf8, 0x1f, 0x64, 0x5d,
    0x07, 0x7d, 0x02, 0xbc, 0xae, 0xa8, 0xec, 0x93, 0xae, 0x0c, 0xfc, 0x03,
    0x55, 0xb6, 0xa3, 0xfc, 0xd0, 0x1a, 0x67, 0xf4, 0x00, 0x00, 0x00, 0x00,
    0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};

    static const unsigned char image1_data[] = { 
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x16,
    0x08, 0x06, 0x00, 0x00, 0x00, 0xc4, 0xb4, 0x6c, 0x3b, 0x00, 0x00, 0x01,
    0x58, 0x49, 0x44, 0x41, 0x54, 0x38, 0x8d, 0xed, 0xd5, 0xcf, 0x2b, 0x84,
    0x41, 0x1c, 0xc7, 0xf1, 0xf7, 0xf3, 0xec, 0x93, 0x44, 0x7e, 0xdd, 0x08,
    0xc9, 0x79, 0x9d, 0x1c, 0xa8, 0x49, 0xdb, 0x86, 0x56, 0x8a, 0xda, 0x92,
    0x12, 0xee, 0x84, 0xdc, 0xac, 0x8b, 0xfc, 0x1b, 0xc2, 0xc5, 0x05, 0x27,
    0xe5, 0xe0, 0x42, 0x7b, 0x50, 0x34, 0xa7, 0x3d, 0x70, 0x76, 0x59, 0x2b,
    0x17, 0x2d, 0x9e, 0xd3, 0xee, 0x26, 0x9e, 0x71, 0x58, 0x97, 0x6d, 0xe6,
    0xd9, 0x1d, 0xb2, 0x07, 0xe5, 0x5b, 0x73, 0x9b, 0xef, 0xab, 0x4f, 0xdf,
    0x99, 0x79, 0x1e, 0x47, 0x4a, 0x49, 0x3d, 0xca, 0xad, 0x8b, 0xfa, 0x0f,
    0xff, 0x18, 0xbe, 0x3b, 0x5c, 0x52, 0x0b, 0x53, 0x42, 0xfd, 0x2a, 0x2c,
    0x84, 0x50, 0xb1, 0x68, 0x27, 0xa9, 0x31, 0x98, 0x1c, 0xad, 0x8d, 0x7b,
    0x26, 0x20, 0x3c, 0x85, 0xc3, 0x50, 0x2f, 0x6c, 0x25, 0x20, 0x99, 0x10,
    0xea, 0xf4, 0x42, 0x3a, 0x56, 0xb0, 0x10, 0x42, 0x3d, 0x1e, 0xcc, 0x11,
    0x14, 0xf2, 0x86, 0xad, 0x11, 0x3e, 0x8a, 0xcf, 0xb4, 0x36, 0x40, 0x32,
    0xea, 0xd2, 0xd3, 0x12, 0x30, 0x33, 0x21, 0xd4, 0xc9, 0xb9, 0x19, 0xd7,
    0x12, 0x53, 0xc8, 0xf3, 0x9e, 0x4b, 0x87, 0x05, 0x29, 0x37, 0xa8, 0x80,
    0xc1, 0x2e, 0xd8, 0x88, 0x43, 0x7c, 0x44, 0xa8, 0xcb, 0x6b, 0x1d, 0xd7,
    0x66, 0xec, 0x59, 0xae, 0x26, 0x0f, 0x66, 0x07, 0x5c, 0x8e, 0xe7, 0x61,
    0xd1, 0x70, 0xa0, 0x1a, 0xec, 0x02, 0x11, 0xdb, 0xa5, 0x02, 0x86, 0xbb,
    0x61, 0x33, 0x06, 0xd3, 0xe3, 0x95, 0xb8, 0x36, 0x8a, 0x08, 0x60, 0x75,
    0x9f, 0xbe, 0xca, 0x7f, 0x83, 0xbe, 0x36, 0xbd, 0xc7, 0x08, 0xdb, 0xd6,
    0x6b, 0x09, 0x1e, 0x7c, 0x38, 0xba, 0x81, 0xb3, 0x74, 0xe5, 0x9c, 0x35,
    0xd8, 0xf6, 0x62, 0xbf, 0x94, 0x20, 0xe7, 0xc3, 0x7e, 0x06, 0xd6, 0xf7,
    0xf4, 0xc3, 0xd3, 0x60, 0x0f, 0x08, 0x2c, 0x93, 0x86, 0xa1, 0x46, 0x38,
    0xc8, 0xa6, 0xb9, 0x7d, 0xd2, 0x37, 0xf6, 0xb7, 0x43, 0x47, 0x63, 0x19,
    0xbd, 0xf7, 0x61, 0x37, 0x03, 0x6b, 0x21, 0xa8, 0x06, 0x4b, 0x29, 0x9d,
    0xe6, 0x2a, 0x2f, 0xaf, 0x98, 0x82, 0xac, 0x0f, 0x3b, 0x19, 0x58, 0xad,
    0x82, 0x1a, 0x13, 0x4b, 0x69, 0x6e, 0x10, 0x42, 0x28, 0x97, 0x32, 0xba,
    0x52, 0x03, 0x85, 0x6f, 0x7c, 0x84, 0xa4, 0x94, 0xce, 0xf6, 0x15, 0x2c,
    0x5b, 0xa0, 0x00, 0xce, 0x9f, 0xfb, 0xe7, 0x7d, 0x02, 0xc4, 0xed, 0x74,
    0xe0, 0x88, 0x96, 0x8a, 0x05, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e,
    0x44, 0xae, 0x42, 0x60, 0x82
};

    static const unsigned char image2_data[] = { 
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x16,
    0x08, 0x06, 0x00, 0x00, 0x00, 0xc4, 0xb4, 0x6c, 0x3b, 0x00, 0x00, 0x01,
    0x70, 0x49, 0x44, 0x41, 0x54, 0x38, 0x8d, 0xdd, 0xd5, 0xbd, 0x4b, 0xc3,
    0x40, 0x18, 0xc7, 0xf1, 0xef, 0x35, 0xf5, 0x1f, 0x90, 0x22, 0xbe, 0xe0,
    0xaa, 0xe0, 0xa4, 0x54, 0x2c, 0x8d, 0xc1, 0x97, 0x22, 0x15, 0x44, 0x14,
    0x3a, 0xb8, 0x68, 0x71, 0x70, 0x10, 0xdc, 0x05, 0x51, 0x27, 0xdd, 0x5c,
    0xd4, 0x51, 0x41, 0xfc, 0x0b, 0x5c, 0x9c, 0x9d, 0x6f, 0x10, 0x47, 0x51,
    0x71, 0x10, 0x71, 0x28, 0x52, 0x31, 0x2a, 0x96, 0x1a, 0xdb, 0xa4, 0xe7,
    0xe0, 0x0b, 0xad, 0xa9, 0xe6, 0x44, 0xbb, 0x78, 0x70, 0xcb, 0x93, 0x27,
    0x9f, 0xfb, 0x85, 0xbb, 0x24, 0x42, 0x4a, 0x49, 0x2d, 0x46, 0xa8, 0x26,
    0xea, 0xff, 0x86, 0x4d, 0xd3, 0x54, 0x7f, 0x0e, 0x4f, 0xa5, 0x92, 0x6a,
    0x67, 0xba, 0xfe, 0x47, 0x78, 0x20, 0x3c, 0x32, 0xd4, 0xa7, 0x56, 0x26,
    0xa3, 0x74, 0x46, 0x6c, 0x5d, 0x13, 0x80, 0xf0, 0x77, 0x17, 0x53, 0x23,
    0x09, 0xb5, 0x9a, 0xee, 0x26, 0xda, 0x2c, 0x70, 0xaf, 0x5f, 0x6b, 0xdf,
    0xa5, 0x96, 0x52, 0x8a, 0x40, 0x78, 0x62, 0x34, 0xa1, 0xb6, 0xe6, 0x7a,
    0xe9, 0x6a, 0xf2, 0x70, 0xef, 0xce, 0x09, 0x1b, 0x70, 0xb1, 0xd2, 0x4c,
    0x5d, 0xa4, 0xc3, 0xd7, 0x9b, 0xbf, 0x3c, 0xa0, 0x7d, 0x4d, 0x23, 0xf1,
    0x40, 0x9f, 0xa5, 0x36, 0x66, 0x62, 0x74, 0x36, 0x14, 0x29, 0xdd, 0x1c,
    0x23, 0x0a, 0xf6, 0x6b, 0x63, 0x3e, 0x83, 0x7b, 0x95, 0xf1, 0xf5, 0x9f,
    0x65, 0xfd, 0x86, 0x0f, 0x4e, 0xa7, 0x92, 0x6a, 0x6f, 0x7e, 0x90, 0x9e,
    0x16, 0x45, 0xd1, 0x3e, 0x43, 0x15, 0xef, 0xbf, 0x7a, 0xa8, 0x8f, 0x61,
    0x54, 0xa9, 0x55, 0xc0, 0xe3, 0xc3, 0x03, 0x6a, 0x7b, 0x36, 0x4e, 0xac,
    0xf1, 0x19, 0x2f, 0x7b, 0x42, 0xa8, 0xa0, 0xb7, 0x61, 0x81, 0xb0, 0xa0,
    0x44, 0x7f, 0xab, 0x87, 0x67, 0x9f, 0x62, 0x68, 0xa2, 0x5f, 0xc1, 0x15,
    0xc7, 0x2d, 0xfb, 0xe8, 0xb2, 0xb9, 0x7f, 0x04, 0xce, 0x2d, 0xe1, 0xb7,
    0x55, 0x75, 0x66, 0x60, 0x62, 0x29, 0xa5, 0x30, 0x4d, 0x53, 0x3d, 0x39,
    0xb0, 0x60, 0x81, 0x21, 0xaa, 0xdc, 0xa1, 0x99, 0xd8, 0xb7, 0x79, 0xef,
    0xb8, 0x01, 0x2c, 0x5a, 0x10, 0xd2, 0xc0, 0xb5, 0xe0, 0x72, 0x3c, 0x04,
    0x2c, 0xbd, 0xe1, 0x97, 0x0f, 0x70, 0xe7, 0x80, 0xaa, 0xf2, 0x7a, 0xe4,
    0x1c, 0x4d, 0xb8, 0x1c, 0x17, 0xc0, 0xb2, 0x05, 0x0f, 0x0e, 0xc4, 0x77,
    0x83, 0xd3, 0x07, 0xc2, 0x9f, 0xf1, 0xb1, 0xb6, 0xca, 0xfa, 0xaf, 0xe0,
    0x72, 0x3c, 0x57, 0xd4, 0x47, 0x41, 0xf3, 0xb3, 0x29, 0xa5, 0x14, 0xeb,
    0x87, 0xfa, 0x28, 0x80, 0xa8, 0xd5, 0xcf, 0xf4, 0x05, 0x92, 0x5c, 0x89,
    0x7a, 0x30, 0x42, 0x50, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e,
    0x44, 0xae, 0x42, 0x60, 0x82
};

    switch (id) {
        case image0_ID:  { QImage img; img.loadFromData(image0_data, sizeof(image0_data), "PNG"); return QPixmap::fromImage(img); }
        case image1_ID:  { QImage img; img.loadFromData(image1_data, sizeof(image1_data), "PNG"); return QPixmap::fromImage(img); }
        case image2_ID:  { QImage img; img.loadFromData(image2_data, sizeof(image2_data), "PNG"); return QPixmap::fromImage(img); }
        default: return QPixmap();
    } // switch
    } // icon

};

namespace Ui {
    class DlgPrint: public Ui_DlgPrint {};
} // namespace Ui

QT_END_NAMESPACE

class DlgPrint : public QDialog, public Ui::DlgPrint
{
    Q_OBJECT

public:
    DlgPrint(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~DlgPrint();

protected slots:
    virtual void languageChange();

};

#endif // DLGPRINT_H

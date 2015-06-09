#include "C_Frm_Prescription.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/Theme/ThemePopup.h"
#include "../../MedinTuxTools-QT4/Theme/Theme.h"
#include "../../MedinTuxTools-QT4/medicabase/C_BDM_Api.h"
#include "../../MedinTuxTools-QT4/medicabase/C_PopupDialCorpoFact.h"
#include "../../MedinTuxTools-QT4/medicabase/C_PopupDial_IndicationChoice.h"
#include "../../MedinTuxTools-QT4/medicabase/C_PopupDial_TerrainChoice.h"
#include "../../MedinTuxTools-QT4/medicabase/C_BDM_Control.h"
#include "../../MedinTuxTools-QT4/C_QWebView/C_QWebView.h"

#include <math.h>

#include <QFont>
#include <QObject>

#include <QPainter>
#include <QFrame>
#include <QRubberBand>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QTimer>
#include <QToolTip>
#include <QDebug>
#include <QTreeWidgetItem>
#include <QPalette>
#include <QScrollArea>
#include <QApplication>
#include <QPointer>
#include <QCursor>
#include <QTextEdit>
#include <QLabel>
#include <QToolButton>
#include <QMenu>
#include <QTimer>
#include <QCheckBox>
#include <QTextDocument>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QMultiMap>
#include <QScrollBar>
#include <QWebView>

#define SIZE_BORDER_DAY    8
#define W_PRODUIT          555
#define H_PRODUIT          100
#define FIRST_OBJ_POS_Y    2
#define FIRST_OBJ_POS_X    2
#define SPACE_BETWEEN_PROD 6
#define HANDLE_MOVE_WIDTH  16
#define H_GRAD             5
#define ITEM_HIT_H         30
#define ITEM_HIT_NBTOVIEW  8
#define HIT_TOOL_H         230
#define HIT_W_MMASCN       300
#define SEGMENT_OFSET      50
#define OFSET_DROIT        35

#define DETAILED_PERIODDAYS     tr("mode p\303\251riodes prescritives") //tr("d\303\251tailler les dur\303\251es")
#define WEEKDAYS_TO_INCLUDE     tr("mode semainier : inclusion (le...)")
#define WEEKDAYS_TO_EXCLUDE     tr("mode semainier : exclusion (sauf..)")
#define WEEKDAYS_EXCLUDE_WE     tr("mode semainier : sauf Sam. Dim.")
/*
SIG ou MITTE : posologie
DIE ou QD : 1 fois par jour
BID : 2 fois par jour
TID : 3 fois par jour
QID : 4 fois par jour
QH : chaque heure
Q4H : toutes les 4 heures
Q4-6H : toutes les 4 \303\240 6 heures
QSP (X jour(s)) : quantit\303\251 suffisante pour X jour(s) > nombre de jour(s)
[modifier]Formes
ONG : onguent
LOT : lotion
CP : comprim\303\251
CAP : capsule
CR : cr\303\250me
AMP : ampoule
SUSP : suspension
SUPP : suppositoire
INJ : injection
SC : sous-cutan\303\251
IM : injection intra musculaire
IV : injection intra veineuse
CR VAG : cr\303\250me vaginale
APPL : application
PO : par voie orale = per os
PRN : au besoin
ALD : \303\240 la demande
AU : aux 2 oreilles
AD : oreille droite
AS : oreille gauche
OU : aux 2 yeux
OS : ?il gauche
OD : ?il droit
CAT : cuill\303\250re \303\240 th\303\251
CAS : cuill\303\250re \303\240 soupe
[modifier]Moment de la prise de la m\303\251dication
HS : au coucher
CC : en mangeant
AC : avant les repas
PC : apr\303\250s les repas
AM : le matin
*/
//======================== C_PopupDialDate =============================

C_PopupDialDate::C_PopupDialDate          ( QWidget                  *parent,
                                            const QRect              &initialRect,
                                            const QRect              &deployRect,
                                            C_Frm_PosologieHitList   *pC_Frm_PosologieHitList,  // il nous faut des icones
                                            const QDateTime          &dateTime
                                          )
     : C_PopupDial(parent, pC_Frm_PosologieHitList->background().lighter(125), initialRect, deployRect)
{       setAlpha(240);
        m_pC_BitMapCollection = pC_Frm_PosologieHitList->bitMapCollection();
        m_duree               = 0;
        m_dureeUnit           = 0;
        m_marge               = pC_Frm_PosologieHitList->marge();
        m_backGround          = pC_Frm_PosologieHitList->background().lighter(115);
        m_fGrd                = pC_Frm_PosologieHitList->font();
        QLabel      *pQlabel  = 0;
        int pos_x             = 1;
        int pos_y             = 1;
        int h                 = initialRect.height()-m_marge;
        QRect          rect;
        CMyButton      *pCMyButton = 0;

        /////////////////////////// la barre superieure////////////////////////////
        //.................... l'editeur de date time .............................
        m_pQDateTimeEdit_Deb      = new QDateTimeEdit(dateTime, this);
        m_pQDateTimeEdit_Deb->setGeometry(pos_x,pos_y,105,initialRect.height());
        m_pQDateTimeEdit_Deb->setDisplayFormat ("dd-MM-yyyy hh:mm:ss" );
        m_pQDateTimeEdit_Deb->setButtonSymbols ( QAbstractSpinBox::NoButtons );
        m_pQDateTimeEdit_Deb->setFont(m_fGrd);
        connect( m_pQDateTimeEdit_Deb,         SIGNAL(dateTimeChanged ( const QDateTime &  )  ),    this , SLOT( Slot_editDateTime_DebChanged(const QDateTime &) ));

        //.................... l'affichage QSP .............................
        pos_x               += m_pQDateTimeEdit_Deb->width() + m_marge;
        rect                 = QRect(pos_x, pos_y, 25, h);
        m_labelQSP           = new QLabel(tr("QSP"), this); m_labelQSP->setGeometry(rect);         m_labelQSP->setFont(m_fGrd);

        pos_x               += m_labelQSP->width() + m_marge ;
        rect                 = QRect(pos_x, 0, 16, h+1);
        m_imageQSP           = new QLabel(this); m_imageQSP->setGeometry(rect);                    m_imageQSP->setFont(m_fGrd);

        pos_x               += m_imageQSP->width() + m_marge ;
        rect                 = QRect(pos_x, pos_y, 55, h);
        m_textQSP            = new QLabel(this); m_textQSP->setGeometry(rect);                     m_textQSP->setFont(m_fGrd);
        //setTitle_QSP(m_duree, m_dureeUnit);

        //.................... l'affichage renouvelable .............................
        pos_x               += m_textQSP->width() + m_marge ;
        rect                 = QRect(pos_x, pos_y, 16, h);
        m_imageRenouv        = new QLabel(this); m_imageRenouv->setGeometry(rect);                 m_imageRenouv->setFont(m_fGrd);
        //m_imageRenouv->setPixmap(m_pC_BitMapCollection->pixmap("renouv-3.png"));

        //.................... l'affichage literalite .............................
        pos_x               += m_imageRenouv->width() + m_marge ;
        rect                 = QRect(pos_x, pos_y, 16, h);
        m_imageLiter         = new QLabel(this); m_imageLiter->setGeometry(rect);                  m_imageLiter->setFont(m_fGrd);
        //m_imageLiter->setPixmap(m_pC_BitMapCollection->pixmap("textMode.png"));

        //.................... l'affichage substituable .............................
        pos_x               += m_imageLiter->width() + m_marge ;
        rect                 = QRect(pos_x, pos_y, 16, h);
        m_imageSusbtituable  = new QLabel(this); m_imageSusbtituable->setGeometry(rect);           m_imageSusbtituable->setFont(m_fGrd);

        //.................... l'affichage dci .............................
        pos_x               += m_imageLiter->width() + m_marge ;
        rect                 = QRect(pos_x, pos_y, 16, h);
        m_imageDci           = new QLabel(this); m_imageDci->setGeometry(rect);                    m_imageDci->setFont(m_fGrd);

        //m_imageSusbtituable->setPixmap(m_pC_BitMapCollection->pixmap("substituable-0.png"));
        //m_imageSusbtituable->setObjectName("0");

        /////////////////////////// les tools/////////////////////////////////////
        QString title_modele =
                "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
                "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">"
                "p, li { white-space: pre-wrap; }"
                "</style></head><body style=\" font-family:'%6'; font-size:%3px; font-weight:400; font-style:normal;\">"
                "<p align=\"left\" style=\" background-color:%4; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                "<!-- <img src=\"%1\" alt=\"jours\" height=\"16\" width=\"16\">-->&nbsp;"
                "<span style=\" font-size:%3px; text-decoration: underline;\"><b>%2</b></span></p></body></html>";
       //.................... l'affichage calendrier .............................
        m_pQCalendarWidget = new QCalendarWidget(this);
        m_pQCalendarWidget->setGridVisible(true);
        m_pQCalendarWidget->setDateEditEnabled ( true );
        m_pQCalendarWidget->setNavigationBarVisible (false);
        m_pQCalendarWidget->setHorizontalHeaderFormat ( QCalendarWidget::SingleLetterDayNames);
        m_pQCalendarWidget->setSelectionMode (QCalendarWidget::SingleSelection );
        m_pQCalendarWidget->setFont(m_fGrd);
        rect                 = QRect(m_marge, m_pQDateTimeEdit_Deb->height()*2 , deployRect.width()-185, deployRect.height()-m_pQDateTimeEdit_Deb->height()*5);
        m_pQCalendarWidget->setGeometry(rect);
        connect( m_pQCalendarWidget,         SIGNAL(clicked ( const QDate &  )  ),    this , SLOT( Slot_calendarDateChanged(const QDate &) ));

        //.................... le titre des outils de saisie QSP jours....................................................................
        pos_x                = m_pQCalendarWidget->x() + m_pQCalendarWidget->width()+m_marge;
        pos_y                = m_pQDateTimeEdit_Deb->height();
        rect                 = QRect(pos_x, pos_y, 176, h);
        QString title        = QString(title_modele)
                .arg(m_pC_BitMapCollection->imagePath("-qsp-0j.png"),   // 1
                     tr("QSP en nombre de jours"),                      // 2
                     QString::number(m_fGrd.pixelSize()),               // 3
                     m_backGround.lighter(110).name(),                  // 4
                     m_fGrd.family());                                  // 5
        pQlabel = new QLabel(title, this); pQlabel->setGeometry(rect);   pQlabel->setFont(m_fGrd);

        //........ zone d'edition du nombre de jours ......................
        rect  = QRect(pQlabel->x()+pQlabel->width()-31, pos_y+1, 30, h);
        m_Ed_jours = new QLineEdit(this); m_Ed_jours->setGeometry(rect);
        m_Ed_jours->setFont(m_fGrd);
        connect( m_Ed_jours,  SIGNAL( editingFinished () ),      this , SLOT( Slot_jours_editingFinished_clicked() ));

        //............ on cree  le bouton zero jour a gauche du titre (efface la mention QSP) ........................
        rect  = QRect(pQlabel->x()-20, pos_y, 16, 16);
        pCMyButton = new CMyButton(CMyButton::FLAT_REAL, m_pC_BitMapCollection->m_Button_delete,  rect,  this, "|j0");
        connect( pCMyButton,  SIGNAL( Sign_ButtonClicked (const QString&) ),      this , SLOT( Slot_Qsp_button_clicked(const QString&) ));

        //.................... la table des jours ....................................................................
        pos_x               += 16;  // on decale les boutons sous la barre de titre de 16 a droite
        pos_y               += pQlabel->height();
        for (int i=1; i<11;++i)
            {rect  = QRect(pos_x, pos_y, 16, 16);  pos_x += 16;
             pCMyButton = new CMyButton(CMyButton::FLAT_REAL, dureeToIcone(i, 0),  rect,  this, tr("|j%1").arg(QString::number(i)));
             connect( pCMyButton,  SIGNAL( Sign_ButtonClicked (const QString&) ),      this , SLOT( Slot_Qsp_button_clicked(const QString&) ));
            }
        pos_y +=16;
        pos_x  = m_pQCalendarWidget->x() + m_pQCalendarWidget->width()+m_marge + 16;   // tout replacer a gauche
        for (int i=11; i<21;++i)
            {rect  = QRect(pos_x, pos_y, 16, 16); pos_x += 16;
             pCMyButton = new CMyButton(CMyButton::FLAT_REAL,  dureeToIcone(i, 0),  rect,  this, tr("|j%1").arg(QString::number(i)));
             connect( pCMyButton,  SIGNAL( Sign_ButtonClicked (const QString&) ),      this , SLOT( Slot_Qsp_button_clicked(const QString&) ));
            }
        pos_y +=16;
        pos_x  = m_pQCalendarWidget->x() + m_pQCalendarWidget->width()+m_marge + 16;  // tout replacer a gauche
        for (int i=21; i<31;++i)
            {rect  = QRect(pos_x, pos_y, 16, 16); pos_x += 16;
             pCMyButton = new CMyButton(CMyButton::FLAT_REAL, dureeToIcone(i, 0),  rect,  this, tr("|j%1").arg(QString::number(i)));
             connect( pCMyButton,  SIGNAL( Sign_ButtonClicked (const QString&) ),      this , SLOT( Slot_Qsp_button_clicked(const QString&) ));
            }

        //.................... le titre des outils de saisie QSP semaine....................................................................
        pos_x                = m_pQCalendarWidget->x() + m_pQCalendarWidget->width()+m_marge;
        pos_y               += 16 ;
        rect                 = QRect(pos_x, pos_y, 176, h);
        title                = QString(title_modele)
                .arg(m_pC_BitMapCollection->imagePath("-qsp-0s.png"),   // 1
                     tr("QSP en nombre de semaines"),                   // 2
                     QString::number(m_fGrd.pixelSize()),               // 3
                     m_backGround.lighter(110).name(),                  // 4
                     m_fGrd.family());                                  // 5
        pQlabel = new QLabel(title, this); pQlabel->setGeometry(rect);   pQlabel->setFont(m_fGrd);

        //........ zone d'edition du nombre de semaines ......................
        rect  = QRect(pQlabel->x()+pQlabel->width()-31, pos_y+1, 30, h);
        m_Ed_semaines = new QLineEdit(this); m_Ed_semaines->setGeometry(rect);
        m_Ed_semaines->setFont(m_fGrd);
        connect( m_Ed_semaines,  SIGNAL( editingFinished () ),      this , SLOT( Slot_semaines_editingFinished_clicked() ));

        //.................... la table des semaines ....................................................................
        pos_x               += 16;  // on decale les boutons sous la barre de titre de 16 a droite
        pos_y               += pQlabel->height();
        for (int i=1; i<11;++i)
            {rect  = QRect(pos_x, pos_y, 16, 16);  pos_x += 16;
             pCMyButton = new CMyButton(CMyButton::FLAT_REAL, dureeToIcone(i, 1),  rect,  this, tr("|s%1").arg(QString::number(i)));
             connect( pCMyButton,  SIGNAL( Sign_ButtonClicked (const QString&) ),      this , SLOT( Slot_Qsp_button_clicked(const QString&) ));
            }
        //.................... le titre des outils de saisie QSP mois....................................................................
        pos_x                = m_pQCalendarWidget->x() + m_pQCalendarWidget->width()+m_marge;
        pos_y               += 16 ;
        rect                 = QRect(pos_x, pos_y, 176, h);
        title                = QString(title_modele)
                .arg(m_pC_BitMapCollection->imagePath("-qsp-0m.png"),   // 1
                     tr("QSP en nombre de mois"),                       // 2
                     QString::number(m_fGrd.pixelSize()),               // 3
                     m_backGround.lighter(110).name(),                  // 4
                     m_fGrd.family());                                  // 5
        pQlabel = new QLabel(title, this); pQlabel->setGeometry(rect);   pQlabel->setFont(m_fGrd);

        //........ zone d'edition du nombre de mois ......................
        rect  = QRect(pQlabel->x()+pQlabel->width()-31, pos_y+1, 30, h);
        m_Ed_mois = new QLineEdit(this); m_Ed_mois->setGeometry(rect);
        m_Ed_mois->setFont(m_fGrd);
        connect( m_Ed_mois,  SIGNAL( editingFinished () ),      this , SLOT( Slot_mois_editingFinished_clicked() ));

        //.................... la table des mois ....................................................................
        pos_x               += 16;  // on decale les boutons sous la barre de titre de 16 a droite
        pos_y               += pQlabel->height();
        for (int i=1; i<11;++i)
            {rect  = QRect(pos_x, pos_y, 16, 16);  pos_x += 16;
             pCMyButton = new CMyButton(CMyButton::FLAT_REAL, dureeToIcone(i, 2),  rect,  this, tr("|m%1").arg(QString::number(i)));
             connect( pCMyButton,  SIGNAL( Sign_ButtonClicked (const QString&) ),      this , SLOT( Slot_Qsp_button_clicked(const QString&) ));
            }
        //.................... le titre des outils de saisie renouvelable....................................................................
        pos_x                = m_pQCalendarWidget->x() + m_pQCalendarWidget->width()+m_marge;
        pos_y               += 16 ;
        rect                 = QRect(pos_x, pos_y, 80, h);
        title                = QString(title_modele)
                .arg(m_pC_BitMapCollection->imagePath("-renouv-0.png"), // 1
                     tr("renouvelable"),                                // 2
                     QString::number(m_fGrd.pixelSize()),               // 3
                     m_backGround.lighter(110).name(),                  // 4
                     m_fGrd.family());                                  // 5
        pQlabel = new QLabel(title, this); pQlabel->setGeometry(rect);          pQlabel->setFont(m_fGrd);

        //.................... les options renouvelables ....................................................................
        pos_x               += 16; // on decale les boutons sous la barre de titre de 16 a droite
        pos_y               += pQlabel->height();
        for (int i=0; i<4;++i)
            {rect  = QRect(pos_x, pos_y, 16, 16);  pos_x += 16;
             pCMyButton = new CMyButton(CMyButton::FLAT_REAL, renouvToIcone(i),  rect,  this, tr("|%1").arg(QString::number(i)));
             connect( pCMyButton,  SIGNAL( Sign_ButtonClicked (const QString&) ),      this , SLOT( Slot_renouv_button_clicked(const QString&) ));
            }
        //.................... le titre des outils de saisie substituable....................................................................
        pos_x                = m_pQCalendarWidget->x() + m_pQCalendarWidget->width() + m_marge + 98;
        pos_y               -= pQlabel->height();   //pQlabel->height(); se mettre sur la meme ligne que renouvelable donc remonter
        rect                 = QRect(pos_x, pos_y, 80, h);
        title                = QString(title_modele)
                .arg(m_pC_BitMapCollection->imagePath("-substituable-0.png"), // 1
                     tr("substituable"),                                      // 2
                     QString::number(m_fGrd.pixelSize()),                     // 3
                     m_backGround.lighter(110).name(),                        // 4
                     m_fGrd.family());                                        // 5
        pQlabel = new QLabel(title, this); pQlabel->setGeometry(rect);          pQlabel->setFont(m_fGrd);

        //.................... les boutons substituables ....................................................................
        pos_x               += 16; // on decale les boutons sous la barre de titre de 16 a droite
        pos_y               += pQlabel->height();
        for (int i=0; i<2;++i)
            {rect  = QRect(pos_x, pos_y, 16, 16);  pos_x += 16;
             pCMyButton = new CMyButton(CMyButton::FLAT_REAL, substituableToIcone(i),  rect,  this, tr("|%1").arg(QString::number(i)));
             connect( pCMyButton,  SIGNAL( Sign_ButtonClicked (const QString&) ),      this , SLOT( Slot_substituable_button_clicked(const QString&) ));
            }
        //.................... le titre des outils de saisie literalite....................................................................
        pos_x                = m_pQCalendarWidget->x() + m_pQCalendarWidget->width()+m_marge;
        pos_y               += pQlabel->height(); //+=16 ;
        rect                 = QRect(pos_x, pos_y, 176, h);
        title                = QString(title_modele)
                .arg(m_pC_BitMapCollection->imagePath("-textMode.png"),  // 1
                     tr("texte / num."),                                 // 2
                     QString::number(m_fGrd.pixelSize()),                // 3
                     m_backGround.lighter(110).name(),                   // 4
                     m_fGrd.family());                                   // 5
        pQlabel = new QLabel(title, this); pQlabel->setGeometry(rect);          pQlabel->setFont(m_fGrd);

        //.................... les boutons de la literalite ....................................................................
        pos_x               += 16;   // on decale les boutons sous la barre de titre de 16 a droite
        pos_y               += pQlabel->height();
        for (int i=0; i<3;++i)
            {rect  = QRect(pos_x, pos_y, 16, 16);  pos_x += 16;
             pCMyButton = new CMyButton(CMyButton::FLAT_REAL, literaliteToIcone(i),  rect,  this, tr("|%1").arg(QString::number(i)));
             connect( pCMyButton,  SIGNAL( Sign_ButtonClicked (const QString&) ),    this , SLOT( Slot_literalite_button_clicked(const QString&) ));
            }
        //.................... le titre des outils de saisie DCI....................................................................
        pos_x                = m_pQCalendarWidget->x() + m_pQCalendarWidget->width() + m_marge + 98;
        pos_y               -= pQlabel->height();   // += pQlabel->height(); se mettre sur la meme ligne que literalite donc remonter
        rect                 = QRect(pos_x, pos_y, 80, h);
        title                = QString(title_modele)
                .arg(m_pC_BitMapCollection->imagePath("-dci-0.png"),  // 1
                     tr("d\303\251nomination"),                              // 2
                     QString::number(m_fGrd.pixelSize()),                // 3
                     m_backGround.lighter(110).name(),                   // 4
                     m_fGrd.family());                                   // 5
        pQlabel = new QLabel(title, this); pQlabel->setGeometry(rect);          pQlabel->setFont(m_fGrd);

        //.................... les boutons de la DCI ....................................................................
        pos_x               += 0;   // on decale les boutons sous la barre de titre de 16 a droite
        pos_y               += pQlabel->height();
        for (int i=0; i<4;++i)
            {rect  = QRect(pos_x, pos_y, 16, 16);  pos_x += 20;
             pCMyButton = new CMyButton(CMyButton::FLAT_REAL, dciToIcone(i),  rect,  this, tr("|%1").arg(QString::number(i)));
             connect( pCMyButton,  SIGNAL( Sign_ButtonClicked (const QString&) ),      this , SLOT( Slot_dci_button_clicked(const QString&) ));
            }
        //.................... le boutons de la date de fin du traitement ....................................................................
        pos_x                 = m_marge;
        pos_y                 = m_pQCalendarWidget->y()+m_pQCalendarWidget->height() + m_marge*2;
        //rect  = QRect(pos_x, pos_y, h, h);

        //m_Button_EndDate      = new CMyColorButton(pC_Frm_PosologieHitList->background(), rect,  this, tr(">|m_Button_EndDate"));
        //connect( pCMyButton,  SIGNAL( Sign_ButtonClicked (const QString&) ),    this , SLOT( Slot_Button_EndDate_clicked(const QString&) ));

        //.................... le titre de la date de fin....................................................................
        pos_x                = m_marge;
        pos_y                = m_pQCalendarWidget->y()+m_pQCalendarWidget->height() + m_marge*2;
        rect                 = QRect(pos_x, pos_y, 176, h);
        title                = QString(title_modele)
                    .arg(m_pC_BitMapCollection->imagePath("-textMode.png"),  // 1
                         tr("choix dans la date de fin"),                    // 2
                         QString::number(m_fGrd.pixelSize()),                // 3
                         m_backGround.lighter(110).name(),                   // 4
                         m_fGrd.family());                                   // 5
        pQlabel = new QLabel(title, this); pQlabel->setGeometry(rect);          pQlabel->setFont(m_fGrd);
        //.......... la zone d'edition de la date de fin ..............................................
        pos_y += h + m_marge;
        m_pQDateTimeEdit_End  = new QDateTimeEdit(dateTime, this);
        m_pQDateTimeEdit_End->setGeometry(pos_x,pos_y,105,initialRect.height());
        m_pQDateTimeEdit_End->setDisplayFormat ("dd-MM-yyyy hh:mm:ss" );
        m_pQDateTimeEdit_End->setButtonSymbols ( QAbstractSpinBox::NoButtons );
        m_pQDateTimeEdit_End->setFont(m_fGrd);
        connect( m_pQDateTimeEdit_End,         SIGNAL(dateTimeChanged ( const QDateTime &  )  ),    this , SLOT( Slot_editDateTime_EndChanged(const QDateTime &) ));

        connect( this, SIGNAL( Sign_deployed (int&)),     this , SLOT( Slot_DialDateDeployed(int&) ));
}
//------------------------------------------------------ Slot_DialDateDeployed ----------------------------------------------------------------------
void C_PopupDialDate::Slot_DialDateDeployed(int &ok)
{   ok=1;
    QPoint pos = mapFromGlobal (QCursor::pos());
    if (pos.x() > m_pQDateTimeEdit_Deb->width() + 50)      // si sur la zone edition ne pas deployer
       {ok=0;
        return;
       }
    raise();
    ok = 1;

}

//------------------------------------------------------ event ----------------------------------------------------------------------
bool C_PopupDialDate::event ( QEvent * ev )
{
 if (ev->type()==QEvent::HoverMove)
    { //QPoint pos = mapFromGlobal (QCursor::pos());
      if ( /* pos.x()<m_pQDateTimeEdit_Deb->width() + 30 && */ ! isDeploy() )
         {setToDeployRect();
         }
    }
 return C_PopupDial::event (ev);
}

//------------------------------------------------------ cycleToInterface ---------------------------------------------------------------------------------
void  C_PopupDialDate::cycleToInterface(const QString &cycle)
{
    int            pos   = cycle.indexOf('R');
    if (pos != -1)
       { Slot_Qsp_button_clicked(     cycle.left(pos) );
         Slot_renouv_button_clicked(  cycle.mid(pos+1) );
       }
    else
      { Slot_Qsp_button_clicked(cycle);
        Slot_renouv_button_clicked("0");
      }
}
//------------------------------------------------------ interfaceToCycle ---------------------------------------------------------------------------------
QString  C_PopupDialDate::interfaceToCycle()
{QString ret =  m_imageQSP->objectName();
 QString ren =  m_imageRenouv->objectName();
 if (ren.mid(1) != "0") return ret+"R"+ren;
 else                   return ret;
}

//------------------------------------------------------ interfaceToDateTimeString ---------------------------------------------------------------------------------
QString  C_PopupDialDate::interfaceToDateTimeString(const QString &format /* = "dd-MM-yyyy hh:mm:ss" */)
{ return m_pQDateTimeEdit_Deb->dateTime().toString(format);
}
//------------------------------------------------------ dateTime ---------------------------------------------------------------------------------
QDateTime  C_PopupDialDate::dateTime()
{return m_pQDateTimeEdit_Deb->dateTime();
}

//------------------------------------------------------ isSubstituable ---------------------------------------------------------------------------------
bool  C_PopupDialDate::isSubstituable()
{return (m_imageSusbtituable->objectName()=="1");
}
//------------------------------------------------------ setSubstituable ---------------------------------------------------------------------------------
void  C_PopupDialDate::setSubstituable(bool state /* = true */)
{if (state) Slot_substituable_button_clicked("1");
 else       Slot_substituable_button_clicked("0");
}
//------------------------------------------------------ literaliteToInterface ---------------------------------------------------------------------------------
void C_PopupDialDate::literaliteToInterface(const QString &literalite)
{if (literalite.contains("ALL_LITERAL"))       Slot_literalite_button_clicked("2");
 else if (literalite.contains("UP_QUANTITE"))  Slot_literalite_button_clicked("1");
 else                                          Slot_literalite_button_clicked("0");
}
//------------------------------------------------------ literaliteToInterface ---------------------------------------------------------------------------------
void C_PopupDialDate::literaliteToInterface(int literalite)
{if      (literalite & C_PosologieGrammar::ALL_LITERAL)       Slot_literalite_button_clicked("2");
 else if (literalite& C_PosologieGrammar::UP_QUANTITE)        Slot_literalite_button_clicked("1");
 else                                                         Slot_literalite_button_clicked("0");
}
//------------------------------------------------------ literaliteToString ---------------------------------------------------------------------------------
/*
if (conv_mod.contains("UP_QUANTITE"))        must_be_literal |= C_PosologieGrammar::UP_QUANTITE;
if (conv_mod.contains("DURATION"))           must_be_literal |= C_PosologieGrammar::DURATION;
if (conv_mod.contains("SEQUENCE_HOUR"))      must_be_literal |= C_PosologieGrammar::SEQUENCE_HOUR;
if (conv_mod.contains("MOMENT_MEAL"))        must_be_literal |= C_PosologieGrammar::MOMENT_MEAL;
if (conv_mod.contains("ALL_LITERAL"))        must_be_literal  = C_PosologieGrammar::ALL_LITERAL;
if (conv_mod.contains("NOT_LITERAL"))        must_be_literal  = C_PosologieGrammar::UP_JUST_FRACTION;
*/
QString  C_PopupDialDate::literaliteToString(int &numResult)
{ QString strResult   = "";
          numResult   =  C_PosologieGrammar::ALL_LITERAL;
 switch (m_imageLiter->objectName().toInt())
 {case 0: { strResult = "NOT_LITERAL"; numResult  = 0; break;}
  case 1: { strResult = "UP_QUANTITE"; numResult  = C_PosologieGrammar::UP_QUANTITE; break;}
  case 2: { strResult = "ALL_LITERAL"; numResult  = C_PosologieGrammar::ALL_LITERAL; break;}
 }
 return strResult;
}
//------------------------------------------------------ dciToIcone---------------------------------------------------------------------------------
QPixmap  C_PopupDialDate::dciToIcone(int nb)
{QString name = "dci-"+QString::number(nb)+".png";
 return m_pC_BitMapCollection->pixmap(name);
}

//------------------------------------------------------ Slot_dci_button_clicked ---------------------------------------------------------------------------------
void  C_PopupDialDate::Slot_dci_button_clicked(const QString &text)
{dciToInterface(text);
 emit Sign_contentChanged(this);
}

//------------------------------------------------------ dciToInterface ---------------------------------------------------------------------------------
void  C_PopupDialDate::dciToInterface(const QString &text)
{m_imageDci->setPixmap(m_pC_BitMapCollection->pixmap(QString("dci-%1.png").arg(text)));
 m_imageDci->setObjectName(text);
}

//------------------------------------------------------ C_PopupDialDate ---------------------------------------------------------------------------------
QString  C_PopupDialDate::dciToString()
{return m_imageDci->objectName();
}

//------------------------------------------------------ literaliteToInt ---------------------------------------------------------------------------------
int  C_PopupDialDate::literaliteToInt()
{int ret = C_PosologieGrammar::ALL_LITERAL;
 switch (m_imageLiter->objectName().toInt())
 {case 0: ret = C_PosologieGrammar::UP_JUST_FRACTION;                                    break;
  case 1: ret = C_PosologieGrammar::UP_JUST_FRACTION | C_PosologieGrammar::UP_QUANTITE;  break;
  case 2: ret = C_PosologieGrammar::ALL_LITERAL;                                         break;
 }
 return ret;
}
//------------------------------------------------------ literaliteToIcone---------------------------------------------------------------------------------
QPixmap  C_PopupDialDate::literaliteToIcone(int nb)
{QString name = "textMode-"+QString::number(nb)+".png";
 return m_pC_BitMapCollection->pixmap(name);
}
//------------------------------------------------------ Slot_literalite_button_clicked ---------------------------------------------------------------------------------
void  C_PopupDialDate::Slot_literalite_button_clicked(const QString &text)
{m_imageLiter->setPixmap(m_pC_BitMapCollection->pixmap(QString("textMode-%1.png").arg(text)));
 m_imageLiter->setObjectName(text);
 emit Sign_contentChanged(this);
}
//------------------------------------------------------ substituableToIcone---------------------------------------------------------------------------------
QPixmap  C_PopupDialDate::substituableToIcone(int nb)
{QString name = "substituable-"+QString::number(nb)+".png";
 return m_pC_BitMapCollection->pixmap(name);
}
//------------------------------------------------------ Slot_substituable_button_clicked ---------------------------------------------------------------------------------
void  C_PopupDialDate::Slot_substituable_button_clicked(const QString&text)
{m_imageSusbtituable->setPixmap(m_pC_BitMapCollection->pixmap(QString("substituable-%1.png").arg(text)));
 m_imageSusbtituable->setObjectName(text);
 emit Sign_contentChanged(this);
}
//------------------------------------------------------ renouvToIcone---------------------------------------------------------------------------------
QPixmap  C_PopupDialDate::renouvToIcone(int nb)
{QString name = "renouv-"+QString::number(nb)+".png";
 return m_pC_BitMapCollection->pixmap(name);
}
//------------------------------------------------------ Slot_renouv_button_clicked ---------------------------------------------------------------------------------
void  C_PopupDialDate::Slot_renouv_button_clicked(const QString &text)
{m_imageRenouv->setPixmap(m_pC_BitMapCollection->pixmap(QString("renouv-%1.png").arg(text)));
 m_imageRenouv->setObjectName(text);
 emit Sign_contentChanged(this);
}
//------------------------------------------------------ Slot_Qsp_button_clicked ---------------------------------------------------------------------------------
void  C_PopupDialDate::Slot_Qsp_button_clicked(const QString &text)
{disconnect( m_Ed_jours,     SIGNAL( editingFinished () ),   this , SLOT( Slot_jours_editingFinished_clicked() ));
 disconnect( m_Ed_semaines,  SIGNAL( editingFinished () ),   this , SLOT( Slot_semaines_editingFinished_clicked() ));
 disconnect( m_Ed_mois,      SIGNAL( editingFinished () ),   this , SLOT( Slot_mois_editingFinished_clicked() ));
 m_Ed_jours->clear();
 m_Ed_semaines->clear();
 m_Ed_mois->clear();

 QString duree = text.mid(1);
 switch (text.toLatin1()[0]|32)
 {case 'j': {setTitle_QSP(duree.toInt(), 0); m_Ed_jours->setText(duree);    break;}
  case 's': {setTitle_QSP(duree.toInt(), 1); m_Ed_semaines->setText(duree); break;}
  case 'm': {setTitle_QSP(duree.toInt(), 2); m_Ed_mois->setText(duree);     break;}
  case 'a': {setTitle_QSP(duree.toInt(), 3); m_Ed_mois->setText(duree);     break;}
  default : setTitle_QSP(0, 0);                    break;
 }
 m_imageQSP->setObjectName(text);
 emit Sign_contentChanged(this);
 connect( m_Ed_jours,     SIGNAL( editingFinished () ),   this , SLOT( Slot_jours_editingFinished_clicked() ));
 connect( m_Ed_semaines,  SIGNAL( editingFinished () ),   this , SLOT( Slot_semaines_editingFinished_clicked() ));
 connect( m_Ed_mois,      SIGNAL( editingFinished () ),   this , SLOT( Slot_mois_editingFinished_clicked() ));

}
//------------------------------------------------------ Slot_jours_editingFinished_clicked ----------------------------------------------------------------------
void C_PopupDialDate::Slot_jours_editingFinished_clicked()
{   if ( m_Ed_jours->text().toInt()<=0)
       { return;
       }
    Slot_Qsp_button_clicked("j"+m_Ed_jours->text());
}
//------------------------------------------------------ Slot_semaines_editingFinished_clicked ----------------------------------------------------------------------
void C_PopupDialDate::Slot_semaines_editingFinished_clicked()
{   if ( m_Ed_semaines->text().toInt()<=0)
       { return;
       }
    Slot_Qsp_button_clicked("s"+m_Ed_semaines->text());
}
//------------------------------------------------------ Slot_mois_editingFinished_clicked ----------------------------------------------------------------------
void C_PopupDialDate::Slot_mois_editingFinished_clicked()
{   if ( m_Ed_mois->text().toInt()<=0)
       { return;
       }
    Slot_Qsp_button_clicked("m"+m_Ed_mois->text());
}
//------------------------------------------------------ setEditDateTime---------------------------------------------------------------------------------
QPixmap  C_PopupDialDate::dureeToIcone(int duree, int dureeUnit)
{QString name = "qsp-"+QString::number(duree);
 switch (dureeUnit)
 {case 0: name += "j.png"; break;
  case 1: name += "s.png"; break;
  case 2: name += "m.png"; break;
  case 3: name += "a.png"; break;
 }
 return m_pC_BitMapCollection->pixmap(name);
}
//------------------------------------------------------ setTitle_QSP ---------------------------------------------------------------------------------
void C_PopupDialDate::setTitle_QSP(int duree, int dureeUnit)
{if (duree==0 && dureeUnit==0)
    {  m_labelQSP->hide();
       m_imageQSP->hide();
       m_textQSP->hide();
    }
 else
    {  m_labelQSP->show();
       m_imageQSP->show();
       m_textQSP->show();
    }
 m_imageQSP->setPixmap(dureeToIcone( duree, dureeUnit ));
 m_textQSP->setText(dureeToTexte(    duree, dureeUnit ));

 //...................... date de fin ...........................
 disconnect( m_pQDateTimeEdit_End,         SIGNAL(dateTimeChanged ( const QDateTime &  )  ),    this , SLOT( Slot_editDateTime_EndChanged(const QDateTime &) ));
 switch (dureeUnit)
  { case 0:  m_pQDateTimeEdit_End->setDateTime(m_pQDateTimeEdit_Deb->dateTime().addDays(duree));   break;
    case 1:  m_pQDateTimeEdit_End->setDateTime(m_pQDateTimeEdit_Deb->dateTime().addDays(duree*7)); break;
    case 2:  m_pQDateTimeEdit_End->setDateTime(m_pQDateTimeEdit_Deb->dateTime().addMonths(duree)); break;
    case 3:  m_pQDateTimeEdit_End->setDateTime(m_pQDateTimeEdit_Deb->dateTime().addYears(duree));  break;
  }
 connect( m_pQDateTimeEdit_End,         SIGNAL(dateTimeChanged ( const QDateTime &  )  ),    this , SLOT( Slot_editDateTime_EndChanged(const QDateTime &) ));

}
//------------------------------------------------------ dureeToTexte [static] ---------------------------------------------------------------------------------
QString  C_PopupDialDate::dureeToTexte(int duree, int dureeUnit)
{ QString ret = "";
  switch (dureeUnit)
   { case 0:  return  duree<=1?tr(" jour")    :  tr(" jours");
     case 1:  return  duree<=1?tr(" semaine") :  tr(" semaines");
     case 2:  return  duree<=1?tr(" mois")    :  tr(" mois");       // on double pour la traduction
     case 3:  return  duree<=1?tr(" an")      :  tr(" ans");
   }
  return ret;
}
//------------------------------------------------------ setEditDateTime ---------------------------------------------------------------------------------
void C_PopupDialDate::setEditDateTime(const QDateTime& dt)
{   m_pQDateTimeEdit_Deb->setDateTime(dt);
    // m_pQCalendarWidget->setCurrentPage ( dt.date().year(), dt.date().month() ); se fera avec le signal dateTimeChanged
}
//------------------------------------------------------ Slot_editDateTime_DebChanged ---------------------------------------------------------------------------------
void C_PopupDialDate::Slot_editDateTime_DebChanged(const QDateTime &dt)
{ if ( !dt.isValid() ) return;
  QDate old_dt_deb = m_pQCalendarWidget->selectedDate ();
  m_pQCalendarWidget->setCurrentPage ( dt.date().year(), dt.date().month() );
  m_pQCalendarWidget->setSelectedDate(dt.date());
  emit Sign_contentChanged(this);
  //........ date de fin ............
  disconnect( m_pQDateTimeEdit_End,         SIGNAL(dateTimeChanged ( const QDateTime &  )  ),    this , SLOT( Slot_editDateTime_EndChanged(const QDateTime &) ));
  int delta_j = old_dt_deb.daysTo(dt.date());
  QDate old_dt_end  = m_pQDateTimeEdit_End->date();
  m_pQDateTimeEdit_End->setDate(old_dt_end.addDays(delta_j));
  connect( m_pQDateTimeEdit_End,            SIGNAL(dateTimeChanged ( const QDateTime &  )  ),    this , SLOT( Slot_editDateTime_EndChanged(const QDateTime &) ));
}

//------------------------------------------------------ Slot_editDateTime_EndChanged ---------------------------------------------------------------------------------
void C_PopupDialDate::Slot_editDateTime_EndChanged(const QDateTime &dt_end)
{ if ( !dt_end.isValid() )  return;
  QDate dt_deb = m_pQCalendarWidget->selectedDate ();
  if (dt_deb>dt_end.date()) return;
  int nb_j     = dt_deb.daysTo(dt_end.date());
  setTitle_QSP(nb_j, 0);
  QString s_nb_j = QString::number(nb_j);
  m_imageQSP->setObjectName("J"+s_nb_j);
  disconnect( m_Ed_jours,     SIGNAL( editingFinished () ),   this , SLOT( Slot_jours_editingFinished_clicked() ));
  disconnect( m_Ed_semaines,  SIGNAL( editingFinished () ),   this , SLOT( Slot_semaines_editingFinished_clicked() ));
  disconnect( m_Ed_mois,      SIGNAL( editingFinished () ),   this , SLOT( Slot_mois_editingFinished_clicked() ));
  m_Ed_jours->setText(s_nb_j);
  m_Ed_semaines->clear();
  m_Ed_mois->clear();
  connect( m_Ed_jours,     SIGNAL( editingFinished () ),   this , SLOT( Slot_jours_editingFinished_clicked() ));
  connect( m_Ed_semaines,  SIGNAL( editingFinished () ),   this , SLOT( Slot_semaines_editingFinished_clicked() ));
  connect( m_Ed_mois,      SIGNAL( editingFinished () ),   this , SLOT( Slot_mois_editingFinished_clicked() ));
  emit Sign_contentChanged(this);
}

//------------------------------------------------------ Slot_Button_EndDate_clicked ---------------------------------------------------------------------------------
void C_PopupDialDate::Slot_Button_EndDate_clicked(const QString &)
{

}
//------------------------------------------------------ Slot_calendarDateChanged ---------------------------------------------------------------------------------
void C_PopupDialDate::Slot_calendarDateChanged(const QDate& dt)
{   QDate old_dt_deb  = m_pQDateTimeEdit_Deb->date();
    int delta_j       = old_dt_deb.daysTo(dt);

    //........ date de debut ............
    disconnect(m_pQDateTimeEdit_Deb,  SIGNAL(dateTimeChanged( const QDateTime & )), 0, 0);
    m_pQDateTimeEdit_Deb->setDateTime(QDateTime(dt,m_pQDateTimeEdit_Deb->dateTime().time()));
    connect( m_pQDateTimeEdit_Deb,    SIGNAL(dateTimeChanged ( const QDateTime &  )  ),    this , SLOT( Slot_editDateTime_DebChanged(const QDateTime &) ));
    //........ date de fin ............
    disconnect( m_pQDateTimeEdit_End,         SIGNAL(dateTimeChanged ( const QDateTime &  )  ),    this , SLOT( Slot_editDateTime_EndChanged(const QDateTime &) ));
    m_pQDateTimeEdit_End->setDate(m_pQDateTimeEdit_End->date().addDays(delta_j));
    connect( m_pQDateTimeEdit_End,            SIGNAL(dateTimeChanged ( const QDateTime &  )  ),    this , SLOT( Slot_editDateTime_EndChanged(const QDateTime &) ));
    emit Sign_contentChanged(this);
}

//====================================== CMyQTextEdit ===================================================================================================

void  CMyQTextEdit::mousePressEvent ( QMouseEvent * e )
{   emit Sign_mousePressEvent ( e , this);
    e->ignore();
}
//------------------------------------------------------ event ---------------------------------------------------------------------------------
void CMyQTextEdit:: mouseDoubleClickEvent( QMouseEvent * e )
{   emit Sign_mouseDoubleClicked ( e , this);
    e->ignore();
}
//====================================== CMyQLineEdit ==========================================================================================
//------------------------------------------------------ event ---------------------------------------------------------------------------------
bool CMyQLineEdit::event ( QEvent * e )
{
    if ( e->type() ==  QEvent::KeyPress )
       {C_Frm_Posologie *pC_Frm_Posologie = (C_Frm_Posologie*)m_ptr;
        if (pC_Frm_Posologie && pC_Frm_Posologie->get_HitList())
           {
            QKeyEvent                    *k = (QKeyEvent *)e;
            int                         key = k->key();
            // Qt::KeyboardModifiers modifiers = k->modifiers();
            if (m_caractersMask[0]=='9' &&                                // ultra crade mais pour l'instant on fait avec
                ! ( (key>=Qt::Key_0 && key<=Qt::Key_9)              ||
                     key==Qt::Key_Comma      || key==Qt::Key_Period ||
                     key==Qt::Key_Return     || key==Qt::Key_Enter  ||
                     key==Qt::Key_Down       || key==Qt::Key_Up     ||
                     key==Qt::Key_Back       || key==Qt::Key_Tab    ||
                     key==Qt::Key_Backspace  || key==Qt::Key_Delete ||
                     key==Qt::Key_Left       || key==Qt::Key_Right  ||
                     key==Qt::Key_Minus
                     )
                  )
               {e->accept();
                return true;
               }
            switch (key)
               {
                case Qt::Key_Down:
                     // emit Sign_UpKey (objectName(), this, m_ptr);
                     // QTimer::singleShot(200, this, SLOT(updateCaption()));
                     // m_direction = C_Frm_PosologieHitList::NEXT;
                     // pC_Frm_Posologie->get_HitList()->setWidgetEditFocus();
                     e->accept();               // indispensable sous linux
                     return true;

                case Qt::Key_Up:
                     // emit Sign_UpKey (objectName(), this, m_ptr);
                     // m_direction = C_Frm_PosologieHitList::PREVIOUS;
                     // pC_Frm_Posologie->get_HitList()->setWidgetEditFocus(C_Frm_PosologieHitList::PREVIOUS);
                     e->accept();              // indispensable sous linux
                     return true;

               case Qt::Key_Return:
               case Qt::Key_Enter:
                     //emit Sign_returnPressed( objectName(), this, m_ptr , modifiers);
                     e->accept();
                     return true;
               } // end switch (key()) c = IsCarValid(m_Valid[m_CursPos].latin1(), k->text()[0].latin1());
          }
       }
    return QLineEdit::event (e);
}

//------------------------------------------------------ mousePressEvent -----------------------------------------------------------------------
void CMyQLineEdit::mousePressEvent ( QMouseEvent * e )
{   QLineEdit::mousePressEvent ( e );
}
//------------------------------------------------------ focusOutEvent --------------------------------------------------------------------------
void CMyQLineEdit::focusOutEvent ( QFocusEvent * e )
{   QLineEdit::focusOutEvent(e);
    emit Sign_focusOut ( objectName(), this, m_ptr);
}
//------------------------------------------------------ focusInEvent --------------------------------------------------------------------------
void CMyQLineEdit::focusInEvent ( QFocusEvent * e )
{   QTimer::singleShot(1,this, SLOT(selectAll()));
    emit Sign_focusIn  ( objectName(), this, m_ptr);
    QLineEdit::focusInEvent(e);
}
//------------------------------------------------------ setCaractersMask --------------------------------------------------------------------------
void CMyQLineEdit::setCaractersMask(const QString &mask)
{m_caractersMask = mask;
}

//======================== C_PopupDialPosoPoint ================================================================================================
C_PopupDialPosoPoint::C_PopupDialPosoPoint ( QWidget                  *parent,
                                             const QRect              &initialRect,
                                             const QRect              &deployRect,
                                             C_Frm_PosologieHitList  *pC_Frm_PosologieHitList,  // il nous faut des icones
                                             const QString            &choice,
                                             const QString            &pointName
                                           )
 : C_PopupDial(parent, pC_Frm_PosologieHitList->background().lighter(125), initialRect, deployRect)
{

    //setAlpha(240);
    m_choice              = choice;
    m_Momentchoice        = "";
    m_pointName           = pointName;
    m_buttonH             = pC_Frm_PosologieHitList->buttonH();
    m_marge               = pC_Frm_PosologieHitList->marge();
    m_pC_BitMapCollection = pC_Frm_PosologieHitList->bitMapCollection();
    m_backGround          = pC_Frm_PosologieHitList->background();
    m_fGrd                = pC_Frm_PosologieHitList->font();

    int pos_debCpZone     = (SEGMENT_OFSET-m_marge-m_marge)*2 + 10;
    int pos_x             = pos_debCpZone;
    int pos_y             = 0;
    int h                 = initialRect.height();
    int w                 = 0;
    int deltY             = (h -m_buttonH)/2;
    //........... la joulie image de la periode .............................................
    QLabel *image= new QLabel(this);
    if      (m_pointName==tr("matin"))     {image->setPixmap (m_pC_BitMapCollection->m_back_repart_matin );   m_numPoint = 0;}
    else if (m_pointName==tr("midi"))      {image->setPixmap (m_pC_BitMapCollection->m_back_repart_midi );    m_numPoint = 1;}
    else if (m_pointName==tr("A.midi"))    {image->setPixmap (m_pC_BitMapCollection->m_back_repart_amidi );   m_numPoint = 2;}
    else if (m_pointName==tr("soir"))      {image->setPixmap (m_pC_BitMapCollection->m_back_repart_soir );    m_numPoint = 3;}
    else if (m_pointName==tr("coucher"))   {image->setPixmap (m_pC_BitMapCollection->m_back_repart_coucher ); m_numPoint = 4;}
    else if (m_pointName==tr("nuit"))      {image->setPixmap (m_pC_BitMapCollection->m_back_repart_nuit );    m_numPoint = 5;}
    image->setGeometry(1,m_buttonH,44,105);
    //........... le bouton de la periode matin midi etc .............................................
    m_MainButton                 = new CMyColorButton(m_backGround, this,tr("%1|HitList : m_Button_PointPoso").arg(m_pointName));
    m_MainButton->setFont(m_fGrd);
    m_MainButton->setGeometry(0,0,SEGMENT_OFSET-m_marge-m_marge,h);
    //........... la zone edition quantite .............................................
    QRect rect              = QRect(pos_x, pos_y+deltY, 50, m_buttonH);
    m_LineEditQuantite      = new CMyQLineEdit(rect,"",this,"m_LineEditQuantite",this);
    m_LineEditQuantite->setObjectName("HitList : m_LineEditQuantite");
    connect( m_LineEditQuantite, SIGNAL( Sign_textChanged (const QString &, CMyQLineEdit *, void *)  ),  this , SLOT( Slot_textChanged(const QString &, CMyQLineEdit *, void *) ));

    //........... le bouton effacement  .............................................
    m_Button_delete                = new CMyColorButton(m_backGround, m_pC_BitMapCollection->m_Button_delete, QRect(pos_x + 52, pos_y+deltY,m_buttonH, m_buttonH),this,   tr("|HitList : m_Button_UnCpx"));
    connect( m_Button_delete,         SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonComprimeClicked(const QString&) ));
    //........... les boutons 0 cp  .............................................
    pos_x                          = pos_debCpZone;
    pos_y                          = m_MainButton->height() + 1;
    w                              = m_pC_BitMapCollection->m_Button_UnCp.width()  + 1;
    h                              = m_pC_BitMapCollection->m_Button_UnCp.height() + 1;
    m_Button_UnCp_0                = new CMyColorButton(m_backGround, m_pC_BitMapCollection->m_Button_UnCp,         QRect(pos_x, pos_y, w+36, h),  this, tr("X 0|HitList : m_Button_UnCpx0"));
    m_Button_UnCp_0->setFont(m_fGrd);            pos_x += m_Button_UnCp_0->width(); m_Button_UnCp_0->setEnabled(false);
    m_Button_UnQuartCp_0           = new CMyColorButton(m_backGround, m_pC_BitMapCollection->m_Button_UnQuartCp,    QRect(pos_x, pos_y, w, h),     this, tr("|HitList : m_Button_UnQuartCpx0.25"));
    m_Button_UnQuartCp_0->setFont(m_fGrd);       pos_x += m_Button_UnQuartCp_0->width();
    m_Button_UnDemiCp_0            = new CMyColorButton(m_backGround, m_pC_BitMapCollection->m_Button_UnDemiCp,     QRect(pos_x, pos_y, w, h),     this, tr("|HitList : m_Button_UnDemiCpx0.50"));
    m_Button_UnDemiCp_0->setFont(m_fGrd);        pos_x += m_Button_UnDemiCp_0->width();
    m_Button_TroisQuartCp_0        = new CMyColorButton(m_backGround, m_pC_BitMapCollection->m_Button_TroisQuartCp, QRect(pos_x, pos_y, w, h),     this,  tr("|HitList : m_Button_TroisQuartCpx0.75"));
    m_Button_TroisQuartCp_0->setFont(m_fGrd);
    connect( m_Button_UnCp_0,         SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonComprimeClicked(const QString&) ));
    connect( m_Button_UnQuartCp_0,    SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonComprimeClicked(const QString&) ));
    connect( m_Button_UnDemiCp_0,     SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonComprimeClicked(const QString&) ));
    connect( m_Button_TroisQuartCp_0, SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonComprimeClicked(const QString&) ));

    //........... les boutons 1 cp  .............................................
    pos_x                          = pos_debCpZone;
    pos_y                         += h+2;
    m_Button_UnCp_1                = new CMyColorButton(m_backGround, m_pC_BitMapCollection->m_Button_UnCp,         QRect(pos_x, pos_y, w+36, h),  this, tr("X 1|HitList : m_Button_UnCpx1"));
    m_Button_UnCp_1->setFont(m_fGrd);            pos_x += m_Button_UnCp_1->width();
    m_Button_UnQuartCp_1           = new CMyColorButton(m_backGround, m_pC_BitMapCollection->m_Button_UnQuartCp,    QRect(pos_x, pos_y, w, h),     this, tr("|HitList : m_Button_UnQuartCpx1.25"));
    m_Button_UnQuartCp_1->setFont(m_fGrd);       pos_x += m_Button_UnQuartCp_1->width();
    m_Button_UnDemiCp_1            = new CMyColorButton(m_backGround, m_pC_BitMapCollection->m_Button_UnDemiCp,     QRect(pos_x, pos_y, w, h),     this, tr("|HitList : m_Button_UnDemiCpx1.50"));
    m_Button_UnDemiCp_1->setFont(m_fGrd);       pos_x += m_Button_UnDemiCp_1->width();
    m_Button_TroisQuartCp_1        = new CMyColorButton(m_backGround, m_pC_BitMapCollection->m_Button_TroisQuartCp, QRect(pos_x, pos_y, w, h),     this,  tr("|HitList : m_Button_TroisQuartCpx1.75"));
    m_Button_TroisQuartCp_1->setFont(m_fGrd);
    connect( m_Button_UnCp_1,         SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonComprimeClicked(const QString&) ));
    connect( m_Button_UnQuartCp_1,    SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonComprimeClicked(const QString&) ));
    connect( m_Button_UnDemiCp_1,     SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonComprimeClicked(const QString&) ));
    connect( m_Button_TroisQuartCp_1, SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonComprimeClicked(const QString&) ));

    //........... les boutons 2 cp  .............................................
    pos_x                          = pos_debCpZone;
    pos_y                         += h+2;
    m_Button_UnCp_2                = new CMyColorButton(m_backGround, m_pC_BitMapCollection->m_Button_UnCp,         QRect(pos_x, pos_y,w+36, h),this,  tr("X 2|HitList : m_Button_UnCpx2"));
    m_Button_UnCp_2->setFont(m_fGrd);           pos_x += m_Button_UnCp_2->width();
    m_Button_UnQuartCp_2           = new CMyColorButton(m_backGround, m_pC_BitMapCollection->m_Button_UnQuartCp,    QRect(pos_x, pos_y,  w, h), this,  tr("|HitList : m_Button_UnQuartCpx2.25"));
    m_Button_UnQuartCp_2->setFont(m_fGrd);      pos_x += m_Button_UnQuartCp_2->width();
    m_Button_UnDemiCp_2            = new CMyColorButton(m_backGround, m_pC_BitMapCollection->m_Button_UnDemiCp,     QRect(pos_x, pos_y,  w, h), this,  tr("|HitList : m_Button_UnDemiCpx2.50"));
    m_Button_UnDemiCp_2->setFont(m_fGrd);       pos_x += m_Button_UnDemiCp_2->width();
    m_Button_TroisQuartCp_2        = new CMyColorButton(m_backGround, m_pC_BitMapCollection->m_Button_TroisQuartCp, QRect(pos_x, pos_y, w, h),  this,  tr("|HitList : m_Button_TroisQuartCpx2.75"));
    m_Button_TroisQuartCp_2->setFont(m_fGrd);
    connect( m_Button_UnCp_2,         SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonComprimeClicked(const QString&) ));
    connect( m_Button_UnQuartCp_2,    SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonComprimeClicked(const QString&) ));
    connect( m_Button_UnDemiCp_2,     SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonComprimeClicked(const QString&) ));
    connect( m_Button_TroisQuartCp_2, SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonComprimeClicked(const QString&) ));

    //........... les boutons 3 cp  .............................................
    pos_x                          = pos_debCpZone;
    pos_y                         += h+2;
    m_Button_UnCp_3                = new CMyColorButton(m_backGround, m_pC_BitMapCollection->m_Button_UnCp,         QRect(pos_x, pos_y, w+36, h), this, tr("X 3|HitList : m_Button_UnCpx3"));
    m_Button_UnCp_3->setFont(m_fGrd);           pos_x += m_Button_UnCp_3->width();
    m_Button_UnQuartCp_3           = new CMyColorButton(m_backGround, m_pC_BitMapCollection->m_Button_UnQuartCp,    QRect(pos_x, pos_y,  w, h),   this, tr("|HitList : m_Button_UnQuartCpx3.25"));
    m_Button_UnQuartCp_3->setFont(m_fGrd);      pos_x += m_Button_UnQuartCp_3->width();
    m_Button_UnDemiCp_3            = new CMyColorButton(m_backGround, m_pC_BitMapCollection->m_Button_UnDemiCp,     QRect(pos_x, pos_y,  w, h),   this, tr("|HitList : m_Button_UnDemiCpx3.50"));
    m_Button_UnDemiCp_3->setFont(m_fGrd);       pos_x += m_Button_UnDemiCp_3->width();
    m_Button_TroisQuartCp_3        = new CMyColorButton(m_backGround, m_pC_BitMapCollection->m_Button_TroisQuartCp, QRect(pos_x, pos_y, w, h),    this, tr("|HitList : m_Button_TroisQuartCpx3.75"));
    m_Button_TroisQuartCp_3->setFont(m_fGrd);
    connect( m_Button_UnCp_3,         SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonComprimeClicked(const QString&) ));
    connect( m_Button_UnQuartCp_3,    SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonComprimeClicked(const QString&) ));
    connect( m_Button_UnDemiCp_3,     SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonComprimeClicked(const QString&) ));
    connect( m_Button_TroisQuartCp_3, SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonComprimeClicked(const QString&) ));

    //........... les boutons 4 cp  .............................................
    pos_x                          = pos_debCpZone;
    pos_y                         += h+2;
    m_Button_UnCp_4                = new CMyColorButton(m_backGround, m_pC_BitMapCollection->m_Button_UnCp,         QRect(pos_x, pos_y, w+36, h), this, tr("X 4|HitList : m_Button_UnCpx4"));
    m_Button_UnCp_4->setFont(m_fGrd);           pos_x += m_Button_UnCp_4->width();
    m_Button_UnQuartCp_4           = new CMyColorButton(m_backGround, m_pC_BitMapCollection->m_Button_UnQuartCp,    QRect(pos_x, pos_y,  w, h),   this, tr("|HitList : m_Button_UnQuartCpx4.25"));
    m_Button_UnQuartCp_4->setFont(m_fGrd);      pos_x += m_Button_UnQuartCp_4->width();
    m_Button_UnDemiCp_4            = new CMyColorButton(m_backGround, m_pC_BitMapCollection->m_Button_UnDemiCp,     QRect(pos_x, pos_y,  w, h),   this, tr("|HitList : m_Button_UnDemiCpx4.50"));
    m_Button_UnDemiCp_4->setFont(m_fGrd);       pos_x += m_Button_UnDemiCp_4->width();
    m_Button_TroisQuartCp_4        = new CMyColorButton(m_backGround, m_pC_BitMapCollection->m_Button_TroisQuartCp, QRect(pos_x, pos_y,  w, h),   this, tr("|HitList : m_Button_TroisQuartCpx4.75"));
    m_Button_TroisQuartCp_4->setFont(m_fGrd);
    connect( m_Button_UnCp_4,         SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonComprimeClicked(const QString&) ));
    connect( m_Button_UnQuartCp_4,    SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonComprimeClicked(const QString&) ));
    connect( m_Button_UnDemiCp_4,     SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonComprimeClicked(const QString&) ));
    connect( m_Button_TroisQuartCp_4, SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonComprimeClicked(const QString&) ));

    //........... on cree la zone de saisie de la distance e nmn en rapport au  repas .............................................
    rect                         = QRect(m_Button_TroisQuartCp_4->x(), deltY, 20, m_buttonH);
    m_LineEditTimeMeal           = new CMyQLineEdit(rect,"",this,"m_LineEditTimeMeal",this);
    m_LineEditTimeMeal->setObjectName("HitList : m_LineEditTimeMeal");
    connect( m_LineEditTimeMeal, SIGNAL( Sign_textChanged (const QString &, CMyQLineEdit *, void *)  ),  this , SLOT( Slot_textChanged(const QString &, CMyQLineEdit *, void *) ));

    //........... on cree les widgets du moment par rappot aux repas et on les mape .............................................
    pos_x                        = m_Button_TroisQuartCp_4->x()+m_Button_TroisQuartCp_4->width()+10;
    pos_y                        = m_Button_delete->y();
    h                            = m_pC_BitMapCollection->m_Button_UnCp.height()+1;
    w                            = 110;
    m_Button_RepasAvant          = new CMyColorButton(m_backGround,m_pC_BitMapCollection->m_Button_RepasAvant,   QRect(pos_x, pos_y, w, h),this,  tr("avant le repas|HitList : m_Button_RepasAvantDial-202"));
    m_buttonMap[m_Button_RepasAvant->objectName()] = m_Button_RepasAvant;            m_Button_RepasAvant->setFont(m_fGrd);
    connect( m_Button_RepasAvant, SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonMomentMealClicked(const QString&) ));

    m_Button_RepasPendant        = new CMyColorButton(m_backGround,m_pC_BitMapCollection->m_Button_RepasPendant, QRect(pos_x, pos_y, w, h),this,  tr("pendant le repas|HitList : m_Button_RepasPendantDial-203"));
    m_buttonMap[m_Button_RepasPendant->objectName()] = m_Button_RepasPendant;        m_Button_RepasPendant->setFont(m_fGrd);
    connect( m_Button_RepasPendant, SIGNAL( Sign_ButtonClicked (const QString&)  ),  this , SLOT( Slot_ButtonMomentMealClicked(const QString&) ));

    m_Button_RepasApres          = new CMyColorButton(m_backGround,m_pC_BitMapCollection->m_Button_RepasApres,   QRect(pos_x, pos_y, w, h),this,  tr("apr\303\250s le repas|HitList : m_Button_RepasApresDial-204"));
    m_buttonMap[m_Button_RepasApres->objectName()] = m_Button_RepasApres;            m_Button_RepasApres->setFont(m_fGrd);
    connect( m_Button_RepasApres, SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_ButtonMomentMealClicked(const QString&) ));

    m_Button_RepasNoChoice       = new CMyColorButton(m_backGround,m_pC_BitMapCollection->m_Button_Repas,        QRect(pos_x, pos_y, w, h), this, tr("non concern\303\251|HitList : m_Button_RepasNoChoiceDial-201"));
    m_buttonMap[m_Button_RepasNoChoice->objectName()] = m_Button_RepasNoChoice;      m_Button_RepasNoChoice->setFont(m_fGrd);
    connect( m_Button_RepasNoChoice, SIGNAL( Sign_ButtonClicked (const QString&)  ), this , SLOT( Slot_ButtonMomentMealClicked(const QString&) ));

    setMomentMealChoice(m_Momentchoice);

    connect( this, SIGNAL( Sign_deployed  (int&)),        this , SLOT( Slot_PosoPointDeployed  (int&)  ));
    connect( this, SIGNAL( Sign_minimized (int&)),        this , SLOT( Slot_PosoPointMinimized (int&) ));
}
//-------------------------------------- Slot_textChanged ----------------------------------------------------------------
void C_PopupDialPosoPoint::Slot_textChanged(const QString &text, CMyQLineEdit *pCMyQLineEdit, void * ptr)
{ Q_UNUSED(text);
  Q_UNUSED(pCMyQLineEdit);
  Q_UNUSED(ptr);
  emit Sign_contentChanged(this);
}
//------------------------ setMomentTimeMeal ---------------------------------------
void C_PopupDialPosoPoint::setMomentTimeMeal(const QString &tps)
{m_LineEditTimeMeal->setText(tps);
}
//-------------------------------------- setQuantite ----------------------------------------------------------------
void C_PopupDialPosoPoint::setQuantite(const QString &text)
{m_LineEditQuantite->setText(text);
}
//-------------------------------------- setQuantite ----------------------------------------------------------------
void C_PopupDialPosoPoint::setQuantite(const DOUBLE_PAIR &val, int mode)
{
    QString res_qu;
    QString s_v1 = C_PosologieGrammar::adjustFractionalPart( C_PosologieGrammar::p_Str(val.v1, 2), mode);
    QString s_v2 = C_PosologieGrammar::adjustFractionalPart( C_PosologieGrammar::p_Str(val.v2, 2), mode);
    if (val.isPair && val.v1!=0)
       { res_qu = s_v1 + "-" + s_v2;
       }
    else
       { res_qu = s_v2;
       }
    m_LineEditQuantite->setText(res_qu);
}
//-------------------------------------- interfaceToGrammar ----------------------------------------------------------------
// <gpp>[1;m|2;d|4;s]J8</gpp>   <gpp>[0.5;m|1;d15r|2;sr30|3;h10-13]J8</gpp>
QString C_PopupDialPosoPoint::interfaceToGrammar()
{
 //.............. quantites ...........................
 QString ret = getQuantite().trimmed();//  + m_MomentMealMap["matin"]+"|";
 if (ret.length()==0) return "";
 switch(m_numPoint)
     { case 0:  ret += ";m"; break;
       case 1:  ret += ";d"; break;
       case 2:  ret += ";a"; break;
       case 3:  ret += ";s"; break;
       case 4:  ret += ";c"; break;
       case 5:  ret += ";n"; break;
       default: ret += ";m"; break;
     }
 //......... recuperer le temps avant ou apres le repas ...........
 QString mmt = m_LineEditTimeMeal->text().trimmed();
 if (mmt.length()==0) mmt = "0";
 //......... recuperer la position avant ou apres le repas ou pendant ...........
 int numMeal = m_Momentchoice.right(3).toInt();
 switch(numMeal)
     { case 202:  ret +=  mmt +"r" ; break;          // avant le repas
       case 203:  ret += "0r0"     ; break;          // pendant le repas
       case 204:  ret += "r"  + mmt; break;          // apres le repas
     }
 return ret;
}

//-------------------------------------- Slot_ButtonMomentMealClicked ----------------------------------------------------------------
void C_PopupDialPosoPoint::Slot_ButtonMomentMealClicked(const QString &objectName)
{  if (this->isDeploy())
       setMomentMealChoice(objectName);
   else
    {
       QMenu *pQMenu      = new QMenu(m_Button_RepasNoChoice); pQMenu->setFont(m_fGrd);
       pQMenu->setObjectName("HitList : m_Button_RepartRepas menu");
       ThemePopup::addOptionMenu(pQMenu,tr("201 - repas sans importance"));
       ThemePopup::addOptionMenu(pQMenu,tr("202 - avant le repas"));
       ThemePopup::addOptionMenu(pQMenu,tr("203 - pendant le repas"));
       ThemePopup::addOptionMenu(pQMenu,tr("204 - apr\303\250s le repas"));

       QAction *QAction = pQMenu->exec(this->cursor().pos());
       if (QAction)
          {setMomentMeal(QAction->data().toInt());
          }
    }
   emit Sign_contentChanged(this);
}


//------------------------ setMomentMeal ---------------------------------------
void C_PopupDialPosoPoint::setMomentMeal(int num)
{switch (num)
  {case 201:  setMomentMealChoice(m_Button_RepasNoChoice->objectName());  break;
   case 202:  setMomentMealChoice(m_Button_RepasAvant->objectName());     break;
   case 203:  setMomentMealChoice(m_Button_RepasPendant->objectName());   break;
   case 204:  setMomentMealChoice(m_Button_RepasApres->objectName());     break;
 }
}

//-------------------------------------- setMomentMealChoice ----------------------------------------------------------------
void C_PopupDialPosoPoint::setMomentMealChoice(const QString &choice)
{   //......... verifier que la clef est dans la map ..............................
    QMap<QString, CMyColorButton*>::const_iterator i = m_buttonMap.find(choice);
    if (i==m_buttonMap.end()) m_Momentchoice = "HitList : m_Button_RepasNoChoiceDial-201";        // si pas trouve on en prend un qui existe
    else                      m_Momentchoice = choice;
    CMyColorButton     *cur_button    = 0;
    //....... on recupere ceux a afficher en 1er ...................
    CMyColorButton     *first_button  = m_buttonMap[m_Momentchoice];   // on recupere donc celui du choix pour le mettre en premier
    int pos_y                 = 0;
    int pos_x                 = first_button->x();
    int h                     = m_pC_BitMapCollection->m_Button_UnCp.height() + 1;
    //.............. on affiche celui selectionne en premier .....................
    first_button->move( pos_x,        pos_y);
    pos_y                     = m_MainButton->height()  + 1;
    //.............. on affiche les autres .......................................
    i = m_buttonMap.constBegin();
    while (i != m_buttonMap.constEnd())
       { cur_button = i.value();
         if ( cur_button != first_button )
            { pos_y       += h+ 2;
              cur_button->move( pos_x,       pos_y);
            }
         ++i;
       }
}
//------------------------------------------------------ Slot_ButtonComprimeClicked ----------------------------------------------------------------------
void C_PopupDialPosoPoint::Slot_ButtonComprimeClicked(const QString &name)
{ int pos = name.indexOf("Cpx");
  if (pos != -1)
     { QString quantite = name.mid(pos+3);
       m_LineEditQuantite->setText(quantite);
     }
}

//------------------------------------------------------ setToInitialRect ----------------------------------------------------------------------
void C_PopupDialPosoPoint::setToDeployRect()
{   QTimer::singleShot(10, m_LineEditQuantite, SLOT(setFocus()));
    C_PopupDial::setToDeployRect();
    QPoint pt = this->parentWidget()->mapToGlobal(QPoint(m_deployRect.x()+30, m_deployRect.y()));
    pt.setY(QCursor::pos().y());
    QCursor::setPos( pt);    //QPoint(m_MainButton->width()/2,m_MainButton->height()/2)
}

//------------------------------------------------------ event ----------------------------------------------------------------------
bool C_PopupDialPosoPoint::event ( QEvent * ev )
{  if (ev->type()==QEvent::HoverMove)
    { QPoint pos = mapFromGlobal (QCursor::pos());
      if (pos.x()<SEGMENT_OFSET && ! isDeploy() )
         {setToDeployRect();
         }
    }
 return C_PopupDial::event (ev);
}

//------------------------------------------------------ Slot_PosoPointDeployed ----------------------------------------------------------------------
void C_PopupDialPosoPoint::Slot_PosoPointDeployed(int &ok)
{   //m_MainButton->hide()
    QPoint pos = mapFromGlobal (QCursor::pos());
    if (pos.x()>SEGMENT_OFSET)      // si sur la zone edition ne pas deployer
       {ok=0;
        return;
       }

    //raise();
    emit Sign_PosoPointDeployed(m_pointName);
    ok = 1;
}
//------------------------------------------------------ Slot_PosoPointMinimized ----------------------------------------------------------------------
void C_PopupDialPosoPoint::Slot_PosoPointMinimized(int &ok)
{    ok = 1;
     //m_MainButton->show();;
    //emit Sign_deployed(m_pointName);
}

//=========================== C_Frm_Sequence =====================================================================================================
C_Frm_Sequence::C_Frm_Sequence (  C_Frm_PosologieHitList *pC_Frm_PosologieHitList,
                                  QRect                   geometry_rect       // c'est le parent qui decide de la geometrie
                               )
  : QFrame(pC_Frm_PosologieHitList)
{   m_buttonH             = pC_Frm_PosologieHitList->buttonH();
    m_marge               = pC_Frm_PosologieHitList->marge();
    m_cur_grammar         = pC_Frm_PosologieHitList->grammar();
    m_pC_BitMapCollection = pC_Frm_PosologieHitList->bitMapCollection();
    m_backGround          = pC_Frm_PosologieHitList->background().darker(115);
    m_borderColor         = m_backGround.darker(200);
    m_fGrd                = pC_Frm_PosologieHitList->font();
    int margeX            = SEGMENT_OFSET;
    QStringList  additionnalOptions;
    //......................... quelques variables ...........................................
    this->setFont(m_fGrd);
    this->setGeometry(geometry_rect);
    QRect rect;
    int pos_y            = 0;
    int pos_x            = margeX;
    int  h               = 0;
    int  w               = 0;
    int daySegment_w     = HIT_W_MMASCN/6;
    QMenu        *pQMenu = 0;

    //................... la zone de saisie de la repartition jour ..............................................
    pos_x                = m_marge ;
    pos_y                = 0;
    QString          txt = tr(" dose totale par jour.");
    rect                 = QRect(pos_x, pos_y, (daySegment_w*2)-m_marge, m_buttonH);
    QLabel      *pQlabel = new QLabel(txt, this); pQlabel->setGeometry(rect);
    pQlabel->setObjectName("HitList : pQlabel_total/j"); pQlabel->setFont(m_fGrd);

    pos_x                = (daySegment_w*3)-m_marge;
    rect                 = QRect(pos_x, pos_y, daySegment_w-m_marge, m_buttonH);
    m_LineEdit_TotalJour = new CMyQLineEdit(rect,"",this,"m_LineEdit_TotalJour",this);
    m_LineEdit_TotalJour->setObjectName("HitList : m_LineEdit_TotalJour");   m_LineEdit_TotalJour->setFont(m_fGrd);
    connect( m_LineEdit_TotalJour, SIGNAL( Sign_textChanged (const QString &, CMyQLineEdit *, void *)  ),  this , SLOT( Slot_textChanged(const QString &, CMyQLineEdit *, void *) ));

    pos_x               += rect.width()+m_marge;
    rect                 = QRect(pos_x, pos_y, daySegment_w*2, m_buttonH);
                     txt = C_PosologieGrammar::unitPriseForm(m_cur_grammar).remove("(s)"); // sera surcharge eventuellement par la grammaire
    m_LineEdit_Forme     = new CMyQLineEdit(rect,txt,this,"HitList : m_LineEdit_Forme",this);   m_LineEdit_Forme->setFont(m_fGrd);
    connect( m_LineEdit_Forme, SIGNAL( Sign_textChanged (const QString &, CMyQLineEdit *, void *)  ),  this , SLOT( Slot_textChanged(const QString &, CMyQLineEdit *, void *) ));

    //................... la zone de saisie de la voie ..............................................
    pos_x               += rect.width()+m_marge;
    rect                 = QRect(pos_x, pos_y, daySegment_w*1, m_buttonH);
                     txt = tr(" par voie ");
    pQlabel    = new QLabel(txt, this); pQlabel->setGeometry(rect);
    pQlabel->setObjectName("HitList : pQlabel_voie"); pQlabel->setFont(m_fGrd);

    pos_x               += rect.width()+m_marge;
    rect                 = QRect(pos_x, pos_y, daySegment_w*2, m_buttonH);
                     txt = C_PosologieGrammar::drugRoute(m_cur_grammar);  // sera surcharge eventuellement par la grammaire
    m_LineEdit_Voie      = new CMyQLineEdit(rect,txt,this,"HitList : m_LineEdit_Voie",this);   m_LineEdit_Voie->setFont(m_fGrd);
    connect( m_LineEdit_Voie, SIGNAL( Sign_textChanged (const QString &, CMyQLineEdit *, void *)  ),  this , SLOT( Slot_textChanged(const QString &, CMyQLineEdit *, void *) ));

    pos_x               += rect.width();
    rect                 = QRect(pos_x, pos_y+1, m_pC_BitMapCollection->m_Button_MenuDown.width(), m_buttonH-2);
    m_Button_Popup_Voie  = new CMyColorButton(m_backGround,m_pC_BitMapCollection->m_Button_MenuDown,rect,this, tr("|HitList : m_Button_Popup_Voie"));     m_Button_Popup_Voie->setFont(m_fGrd);
    // m_Button_Popup_Voie  = new CMyButton(1,m_pC_BitMapCollection->m_Button_MenuDown,rect,this, tr("|HitList : m_Button_Popup_Voie"));     m_Button_Popup_Voie->setFont(m_fGrd);
    m_Button_Popup_Voie->setFlat(true);
    pQMenu               = new QMenu(m_Button_Popup_Voie); pQMenu->setFont(m_fGrd);
    pQMenu->setObjectName("HitList : m_Button_Popup_Voie menu");
    additionnalOptions  = m_pC_BitMapCollection->getFileList("add_PopupVoies.txt");
    for (int i=0; i<additionnalOptions.size(); ++i)
        {ThemePopup::addOptionMenu(pQMenu,additionnalOptions.at(i));
        }
    m_Button_Popup_Voie->setMenu ( pQMenu );
    connect( pQMenu, SIGNAL( triggered ( QAction *  )  ),    this , SLOT( Slot_MenuTriggered( QAction *  ) ));

    //................... la zone des parametres corporels ...............................................................................
    /*
    pos_x                = margeX + HIT_W_MMASCN + m_marge;
    rect                 = QRect(pos_x, pos_y, daySegment_w*2, m_buttonH);
    m_C_PopupDialCorpoFact = new C_PopupDialCorpoFact(this, m_backGround,QRect(pos_x,pos_y,geometry_rect.width()-pos_x-m_marge,m_buttonH),QRect(pos_x-200,pos_y,0,0));
    */
    //................... le bouton de menu de la repartition matin midi soir.............................................................
    pos_x                = m_marge ;
    pos_y               += m_buttonH;
    rect                 = QRect(pos_x, pos_y, (daySegment_w*3)-m_marge, m_buttonH);
                 pQlabel = new QLabel(tr(" r\303\251partition journali\303\250re "), this); pQlabel->setGeometry(rect);
    pQlabel->setObjectName("HitList : pQlabel_repart/j"); pQlabel->setFont(m_fGrd);

    pos_x                = (daySegment_w*3)-m_marge;
    rect                 = QRect(pos_x, pos_y, (daySegment_w*3)-m_marge, m_buttonH);
    m_Button_Repart      = new CMyColorButton(m_backGround,rect,this, tr("libre dans la journ\303\251e|HitList : m_Button_Repart")); m_Button_Repart->setFont(m_fGrd);
    pQMenu               = new QMenu(m_Button_Repart); pQMenu->setFont(m_fGrd);
    pQMenu->setObjectName("HitList : m_Button_Repart menu");
    ThemePopup::addOptionMenu(pQMenu, tr("101  - libre dans la journ\303\251e"));
    ThemePopup::addOptionMenu(pQMenu, tr("102  - 1 fois par jour : le matin"));
    ThemePopup::addOptionMenu(pQMenu, tr("103  - 1 fois par jour : \303\240 midi"));
    ThemePopup::addOptionMenu(pQMenu, tr("104  - 1 fois par jour : l'apr\303\250s-midi"));
    ThemePopup::addOptionMenu(pQMenu, tr("105  - 1 fois par jour : le soir"));
    ThemePopup::addOptionMenu(pQMenu, tr("106  - 1 fois par jour : au coucher"));
    ThemePopup::addOptionMenu(pQMenu, tr("107  - 1 fois par jour : dans la nuit"));
    pQMenu->addSeparator();
    ThemePopup::addOptionMenu(pQMenu, tr("108  - 2 fois par jour : le matin et \303\240  midi"));
    ThemePopup::addOptionMenu(pQMenu, tr("109  - 2 fois par jour : le matin et le soir"));
    ThemePopup::addOptionMenu(pQMenu, tr("110  - 2 fois par jour : le matin et l'apr\303\250s-midi"));
    ThemePopup::addOptionMenu(pQMenu, tr("111  - 2 fois par jour : le matin et au coucher"));
    ThemePopup::addOptionMenu(pQMenu, tr("112  - 2 fois par jour : \303\240  midi et le soir"));
    ThemePopup::addOptionMenu(pQMenu, tr("113  - 2 fois par jour : au coucher et dans la nuit"));
    pQMenu->addSeparator();
    ThemePopup::addOptionMenu(pQMenu, tr("114 - 3 fois par jour : matin midi soir"));
    ThemePopup::addOptionMenu(pQMenu, tr("115 - 4 fois par jour : matin midi soir coucher"));
    ThemePopup::addOptionMenu(pQMenu, tr("116 - 5 fois par jour : matin midi apr\303\250s-midi soir coucher"));
    ThemePopup::addOptionMenu(pQMenu, tr("117 - 6 fois par vingt quatre heures : matin midi apr\303\250s-midi soir coucher nuit"));
    m_Button_Repart->setMenu ( pQMenu );
    connect( pQMenu, SIGNAL( triggered ( QAction *  )  ),    this , SLOT( Slot_MenuTriggered( QAction *  ) ));

    //................... le bouton de menu de la repartition repas.............................................................
    pos_x               += m_Button_Repart->width()+m_marge;
    rect                 = QRect(pos_x, pos_y, daySegment_w-m_marge-m_marge-m_marge, m_buttonH);
                 pQlabel = new QLabel(tr(" / repas "), this); pQlabel->setGeometry(rect);
    pQlabel->setObjectName("HitList : pQlabel_repart/repas"); pQlabel->setFont(m_fGrd);
    pos_x                = margeX + HIT_W_MMASCN + m_marge;
    rect                 = QRect(pos_x, pos_y, geometry_rect.width()-pos_x-m_marge, m_buttonH);
    m_Button_RepartRepas = new CMyColorButton(m_backGround,rect,this, tr("repas sans importance|HitList : m_Button_Repart")); m_Button_RepartRepas->setFont(m_fGrd);
    pQMenu               = new QMenu(m_Button_RepartRepas); pQMenu->setFont(m_fGrd);
    pQMenu->setObjectName("HitList : m_Button_RepartRepas menu");
    ThemePopup::addOptionMenu(pQMenu,tr("201 - repas sans importance"));
    pQMenu->addSeparator();
    ThemePopup::addOptionMenu(pQMenu,tr("202 - avant le repas"));
    ThemePopup::addOptionMenu(pQMenu,tr("203 - pendant le repas"));
    ThemePopup::addOptionMenu(pQMenu,tr("204 - apr\303\250s le repas"));
    m_Button_RepartRepas->setMenu ( pQMenu );
    connect( pQMenu, SIGNAL( triggered ( QAction *  )  ),    this , SLOT( Slot_MenuTriggered( QAction *  ) ));

    //................... la zone de saisie de la repartition semaine ..............................................
    /*
    #define DETAILED_PERIODDAYS     tr("d\303\251tailler les dur\303\251es")
    #define WEEKDAYS_TO_INCLUDE     tr("semainier : inclusion (le...)")
    #define WEEKDAYS_TO_EXCLUDE     tr("semainier : exclusion (sauf..)")
    #define WEEKDAYS_EXCLUDE_WE     tr("semainier : sauf Samedi Dimanche")
    */
    int maxW         = 0;
    pos_x            = margeX + HIT_W_MMASCN + m_marge;
    pos_y            = m_buttonH*2+m_marge; //m_LineEditNuit->y() - m_buttonH;   // on se cale sur le debut de la zone des rectangles de couleur
    rect             = QRect(pos_x, pos_y, geometry_rect.width()-pos_x-m_marge, m_buttonH);
    m_check_MenuBut  = new CMyColorButton(m_backGround,rect,this, tr("jours non d\303\251taill\303\251s|HitList : m_check_MenuBut 501"));  m_check_MenuBut->setFont(m_fGrd);  pos_y += m_buttonH;
    pQMenu           = new QMenu(m_check_MenuBut); pQMenu->setFont(m_fGrd);
    pQMenu->setObjectName("HitList : m_check_MenuBut menu");
    ThemePopup::addOptionMenu(pQMenu,"501 - "+DETAILED_PERIODDAYS);
    pQMenu->addSeparator();
    ThemePopup::addOptionMenu(pQMenu,"502 - "+WEEKDAYS_TO_INCLUDE);
    ThemePopup::addOptionMenu(pQMenu,"503 - "+WEEKDAYS_TO_EXCLUDE);
    ThemePopup::addOptionMenu(pQMenu,"504 - "+WEEKDAYS_EXCLUDE_WE);
    m_check_MenuBut->setMenu ( pQMenu );
    connect( pQMenu, SIGNAL( triggered ( QAction *  )  ),    this , SLOT( Slot_MenuTriggered( QAction *  ) ));

    m_check_Lundi    = new QCheckBox (tr("lundi"), this );          m_check_Lundi->setObjectName("m_check_Lundi");       m_check_Lundi->move(pos_x,pos_y);    pos_y += m_buttonH; maxW = qMax(maxW,m_check_Lundi->width());
    m_check_Lundi->setObjectName("HitList : m_check_Lundi");
    m_check_Mardi    = new QCheckBox (tr("mardi"), this );          m_check_Mardi->setObjectName("m_check_Mardi");       m_check_Mardi->move(pos_x,pos_y);    pos_y += m_buttonH; maxW = qMax(maxW,m_check_Mardi->width());
    m_check_Mardi->setObjectName("HitList : m_check_Mardi");
    m_check_Mercredi = new QCheckBox (tr("mercredi"), this );       m_check_Mercredi->setObjectName("m_check_Mercredi"); m_check_Mercredi->move(pos_x,pos_y); pos_y += m_buttonH; maxW = qMax(maxW,m_check_Mercredi->width());
    m_check_Mercredi->setObjectName("HitList : m_check_Mercredi");
    m_check_Jeudi    = new QCheckBox (tr("jeudi"), this );          m_check_Jeudi->setObjectName("m_check_Jeudi");       m_check_Jeudi->move(pos_x,pos_y);    pos_y += m_buttonH; maxW = qMax(maxW,m_check_Jeudi->width());
    m_check_Jeudi->setObjectName("HitList : m_check_Jeudi");
    m_check_Vendredi = new QCheckBox (tr("vendredi"), this );       m_check_Vendredi->setObjectName("m_check_Vendredi"); m_check_Vendredi->move(pos_x,pos_y); pos_y += m_buttonH; maxW = qMax(maxW,m_check_Vendredi->width());
    m_check_Vendredi->setObjectName("HitList : m_check_Vendredi");
    m_check_Samedi   = new QCheckBox (tr("samedi"), this );         m_check_Samedi->setObjectName("m_check_Samedi");     m_check_Samedi->move(pos_x,pos_y);   pos_y += m_buttonH; maxW = qMax(maxW,m_check_Samedi->width());
    m_check_Samedi->setObjectName("HitList : m_check_Samedi");
    m_check_Dimanche = new QCheckBox (tr("dimanche"), this );       m_check_Dimanche->setObjectName("m_check_Dimanche"); m_check_Dimanche->move(pos_x,pos_y);                     maxW = qMax(maxW,m_check_Dimanche->width());
    m_check_Dimanche->setObjectName("HitList : m_check_Dimanche");

    //................... la zone de saisie de la repartition tous les..............................................
    pos_x           = margeX + HIT_W_MMASCN + m_marge;
    pos_y           = m_buttonH*4+m_marge; //m_LineEditNuit->y() - m_buttonH;   // on se cale sur le debut de la zone des rectangles de couleur
    rect            = QRect(pos_x, pos_y, daySegment_w+daySegment_w/3, m_buttonH);
    m_EveryQuBut    = new CMyColorButton(m_backGround,rect,this, tr("tous les|HitList : m_EveryQuBut"));  m_EveryQuBut->setGeometry(rect); m_EveryQuBut->setFont(m_fGrd);
    pQMenu          = new QMenu(m_EveryQuBut); pQMenu->setFont(m_fGrd);
    pQMenu->setObjectName("HitList : m_EveryQuBut menu");
    ThemePopup::addOptionMenu(pQMenu,tr("700 -  non pr\303\251cis\303\251"));
    additionnalOptions = m_pC_BitMapCollection->getFileList("add_PopupTousLes.txt");
    for (int i=0; i<additionnalOptions.size(); ++i)
        {ThemePopup::addOptionMenu(pQMenu,additionnalOptions.at(i));
        }
    m_EveryQuBut->setMenu ( pQMenu );
    connect( pQMenu, SIGNAL( triggered ( QAction *  )  ),    this , SLOT( Slot_MenuTriggered( QAction *  ) ));

    pos_x          += m_EveryQuBut->width()+m_marge;
    rect            = QRect(pos_x, pos_y, daySegment_w/2, m_buttonH);
    m_EveryQuEdit   = new CMyQLineEdit(rect,"",this,"m_EveryQuEdit",this);
    m_EveryQuEdit->setObjectName("HitList : m_EveryQuEdit"); m_EveryQuEdit->setFont(m_fGrd);  m_EveryQuEdit->move(pos_x, pos_y);    pos_x += m_EveryQuEdit->width() + m_marge;
    connect( m_EveryQuEdit, SIGNAL( Sign_textChanged (const QString &, CMyQLineEdit *, void *)  ),  this , SLOT( Slot_textChanged(const QString &, CMyQLineEdit *, void *) ));

    rect            = QRect(pos_x, pos_y, geometry_rect.width()-pos_x-m_marge, m_buttonH);
    m_EveryUnitBut  = new CMyColorButton(m_backGround,rect,this, tr("jours|HitList : m_EveryUnitBut")); m_EveryUnitBut->setFont(m_fGrd);
    pQMenu          = new QMenu(m_EveryUnitBut); pQMenu->setFont(m_fGrd);
    pQMenu->setObjectName("HitList : m_EveryUnitBut menu");
    ThemePopup::addOptionMenu(pQMenu,tr("600 - heures"));
    ThemePopup::addOptionMenu(pQMenu,tr("601 - jours"));
    ThemePopup::addOptionMenu(pQMenu,tr("602 - semaines"));
    ThemePopup::addOptionMenu(pQMenu,tr("603 - mois"));
    ThemePopup::addOptionMenu(pQMenu,tr("604 - ans"));
    m_EveryUnitBut->setMenu ( pQMenu );
    connect( pQMenu, SIGNAL( triggered ( QAction *  )  ),    this , SLOT( Slot_MenuTriggered( QAction *  ) ));

    //................... la zone de saisie de la quantite suffisante ..............................................
    pos_x                = margeX + HIT_W_MMASCN + m_marge;
    pos_y                = m_buttonH*5+m_marge; //m_LineEditNuit->y() - m_buttonH;   // on se cale sur le debut de la zone des rectangles de couleur
    rect                 = QRect(pos_x, pos_y, daySegment_w+daySegment_w/3, m_buttonH);
    m_Button_Pendant     = new CMyColorButton(m_backGround,rect,this, tr("pendant|HitList : m_Button_Pendant")); m_Button_Pendant->setFont(m_fGrd);
    pQMenu               = new QMenu(m_Button_Pendant); pQMenu->setFont(m_fGrd);
    pQMenu->setObjectName("HitList : m_Button_Pendant menu");
    ThemePopup::addOptionMenu(pQMenu,tr("300 -  non pr\303\251cis\303\251"));
    ThemePopup::addOptionMenu(pQMenu,tr("301 -  un "));
    ThemePopup::addOptionMenu(pQMenu,tr("302 -  deux "));
    ThemePopup::addOptionMenu(pQMenu,tr("303 -  trois "));
    ThemePopup::addOptionMenu(pQMenu,tr("304 -  quatre "));
    ThemePopup::addOptionMenu(pQMenu,tr("305 -  cinq "));
    ThemePopup::addOptionMenu(pQMenu,tr("306 -  six "));
    ThemePopup::addOptionMenu(pQMenu,tr("307 -  sept "));
    ThemePopup::addOptionMenu(pQMenu,tr("308 -  huit "));
    ThemePopup::addOptionMenu(pQMenu,tr("310 -  dix "));
    ThemePopup::addOptionMenu(pQMenu,tr("312 -  douze "));
    ThemePopup::addOptionMenu(pQMenu,tr("315 -  quinze "));
    ThemePopup::addOptionMenu(pQMenu,tr("321 -  vingt et un "));
    additionnalOptions = m_pC_BitMapCollection->getFileList("add_PopupPendant.txt");
    for (int i=0; i<additionnalOptions.size(); ++i)
        {ThemePopup::addOptionMenu(pQMenu,additionnalOptions.at(i));
        }

    m_Button_Pendant->setMenu ( pQMenu );
    connect( pQMenu, SIGNAL( triggered ( QAction *  )  ),    this , SLOT( Slot_MenuTriggered( QAction *  ) ));

    pos_x               += m_Button_Pendant->width()+m_marge;
    rect                 = QRect(pos_x, pos_y, daySegment_w/2, m_buttonH);
    m_LineEdit_Duree     = new CMyQLineEdit(rect,"",this,"m_LineEdit_Duree",this);
    m_LineEdit_Duree->setObjectName("HitList : m_LineEdit_Duree");   m_LineEdit_Duree->setFont(m_fGrd);
    connect( m_LineEdit_Duree, SIGNAL( Sign_textChanged (const QString &, CMyQLineEdit *, void *)  ),  this , SLOT( Slot_textChanged(const QString &, CMyQLineEdit *, void *) ));

    pos_x               += m_LineEdit_Duree->width()+m_marge;
    rect                 = QRect(pos_x, pos_y, geometry_rect.width()-pos_x-m_marge, m_buttonH);
    m_Button_DureeUnite  = new CMyColorButton(m_backGround,rect,this, tr("jour(s)|HitList : m_Button_DureeUnite")); m_Button_DureeUnite->setFont(m_fGrd);
    pQMenu               = new QMenu(m_Button_DureeUnite); pQMenu->setFont(m_fGrd);
    pQMenu->setObjectName("HitList : m_Button_DureeUnite menu");
    ThemePopup::addOptionMenu(pQMenu,tr("401 - jour(s)"));
    ThemePopup::addOptionMenu(pQMenu,tr("407 - semaines"));
    ThemePopup::addOptionMenu(pQMenu,tr("430 - mois"));
    ThemePopup::addOptionMenu(pQMenu,tr("465 - ann\303\251e(s)"));
    m_Button_DureeUnite->setMenu ( pQMenu );
    connect( pQMenu, SIGNAL( triggered ( QAction *  )  ),    this , SLOT( Slot_MenuTriggered( QAction *  ) ));

    setCheckBoxConnexionState(true);

    //..................... la zone de saisie du commentaire ...................................................................
    pos_x                = m_marge;
    pos_y                = qMax ( m_pC_BitMapCollection->getCalculatedPointPosoZoneH(m_marge) , m_buttonH * 8 ) + m_buttonH*2+m_marge;
    rect                 = QRect(pos_x, pos_y, daySegment_w-m_marge, m_buttonH);
                 pQlabel = new QLabel(tr("Note :"), this); pQlabel->setGeometry(rect);
    pQlabel->setObjectName("HitList : pQlabel_note"); pQlabel->setFont(m_fGrd);

    pos_x               += pQlabel->width() + m_marge;
    rect                 = QRect(pos_x, pos_y, geometry_rect.width()-pos_x-m_marge-m_marge, m_buttonH);
    m_LineEdit_comm      = new CMyQLineEdit(rect,"",this,"m_LineEdit_comm",this);
    connect( m_LineEdit_comm, SIGNAL( Sign_textChanged (const QString &, CMyQLineEdit *, void *)  ),  this , SLOT( Slot_textChanged(const QString &, CMyQLineEdit *, void *) ));
    m_LineEdit_comm->setAlignment ( Qt::AlignLeft );

    //..................... les zones de saisie des quantites par periode journaliere (matin midi etc) ..........................
    h                 = m_pC_BitMapCollection->m_Button_RepasAvant.height()+1;
    rect              = QRect(SEGMENT_OFSET, m_buttonH + m_buttonH + m_marge , geometry_rect.width()-SEGMENT_OFSET-m_marge-m_marge , m_pC_BitMapCollection->getCalculatedPointPosoZoneH(m_marge));
    pos_y             = rect.y();
    pos_x             = m_marge + m_marge;
    w                 = m_check_Lundi->x()-10;
    //............................. m_PopupPosoPoint_Matin .................................
    m_PopupPosoPoint_Matin = new C_PopupDialPosoPoint (   this,
                                                          QRect(pos_x,pos_y,w,h),
                                                          rect,
                                                          pC_Frm_PosologieHitList,  // il nous faut des icones
                                                          "",
                                                          tr("matin")
                                                      );
    pos_y += h+2;
    connect( m_PopupPosoPoint_Matin, SIGNAL( Sign_PosoPointDeployed (const QString&)),       this , SLOT( Slot_PosoPointDeployed(const QString&) ));
    connect( m_PopupPosoPoint_Matin, SIGNAL( Sign_contentChanged (C_PopupDialPosoPoint*)),   this , SLOT( Slot_PosoPointChanged(C_PopupDialPosoPoint*) ));
    //............................. m_PopupPosoPoint_Midi .................................
    m_PopupPosoPoint_Midi = new C_PopupDialPosoPoint (    this,
                                                          QRect(pos_x,pos_y,w,h),
                                                          rect,
                                                          pC_Frm_PosologieHitList,  // il nous faut des icones
                                                          "",
                                                          tr("midi")
                                                      );
    pos_y += h+2;
    connect( m_PopupPosoPoint_Midi, SIGNAL( Sign_PosoPointDeployed (const QString&)),        this , SLOT( Slot_PosoPointDeployed(const QString&) ));
    connect( m_PopupPosoPoint_Midi, SIGNAL( Sign_contentChanged (C_PopupDialPosoPoint*)),    this , SLOT( Slot_PosoPointChanged(C_PopupDialPosoPoint*) ));
    //............................. m_PopupPosoPoint_AMidi .................................
    m_PopupPosoPoint_AMidi = new C_PopupDialPosoPoint (  this,
                                                         QRect(pos_x,pos_y,w,h),
                                                         rect,
                                                         pC_Frm_PosologieHitList,  // il nous faut des icones
                                                         "",
                                                         tr("A.midi")
                                                      );
    pos_y +=  h+2;
    connect( m_PopupPosoPoint_AMidi, SIGNAL( Sign_PosoPointDeployed (const QString&)),       this , SLOT( Slot_PosoPointDeployed(const QString&) ));
    connect( m_PopupPosoPoint_AMidi, SIGNAL( Sign_contentChanged (C_PopupDialPosoPoint*)),   this , SLOT( Slot_PosoPointChanged(C_PopupDialPosoPoint*) ));
    //............................. m_PopupPosoPoint_Soir .................................
    m_PopupPosoPoint_Soir = new C_PopupDialPosoPoint (    this,
                                                          QRect(pos_x,pos_y,w,h),
                                                          rect,
                                                          pC_Frm_PosologieHitList,  // il nous faut des icones
                                                          "",
                                                          tr("soir")
                                                      );
    pos_y +=  h+2;
    connect( m_PopupPosoPoint_Soir, SIGNAL( Sign_PosoPointDeployed (const QString&)),        this , SLOT( Slot_PosoPointDeployed(const QString&) ));
    connect( m_PopupPosoPoint_Soir, SIGNAL( Sign_contentChanged (C_PopupDialPosoPoint*)),    this , SLOT( Slot_PosoPointChanged(C_PopupDialPosoPoint*) ));
    //............................. m_PopupPosoPoint_Coucher .................................
    m_PopupPosoPoint_Coucher = new C_PopupDialPosoPoint (this,
                                                         QRect(pos_x,pos_y,w,h), //QRect(m_marge,pos_y,SEGMENT_OFSET-m_marge-m_marge,m_buttonH),
                                                         rect,
                                                         pC_Frm_PosologieHitList,  // il nous faut des icones
                                                         "",
                                                         tr("coucher")
                                                      );
    pos_y +=  h+2;
    connect( m_PopupPosoPoint_Coucher, SIGNAL( Sign_PosoPointDeployed (const QString&)),     this , SLOT( Slot_PosoPointDeployed(const QString&) ));
    connect( m_PopupPosoPoint_Coucher, SIGNAL( Sign_contentChanged (C_PopupDialPosoPoint*)), this , SLOT( Slot_PosoPointChanged(C_PopupDialPosoPoint*) ));
    //............................. m_PopupPosoPoint_Nuit .................................
    m_PopupPosoPoint_Nuit = new C_PopupDialPosoPoint (    this,
                                                          QRect(pos_x,pos_y,w,h),
                                                          rect,
                                                          pC_Frm_PosologieHitList,  // il nous faut des icones
                                                          "",
                                                          tr("nuit")
                                                      );
    pos_y +=  h+2;
    connect( m_PopupPosoPoint_Nuit, SIGNAL( Sign_PosoPointDeployed (const QString&)),        this , SLOT( Slot_PosoPointDeployed(const QString&) ));
    connect( m_PopupPosoPoint_Nuit, SIGNAL( Sign_contentChanged (C_PopupDialPosoPoint*)),    this , SLOT( Slot_PosoPointChanged(C_PopupDialPosoPoint*) ));

    clearInterface();
}

//------------------------ setCheckBoxConnexionState ---------------------------------------
void C_Frm_Sequence::setCheckBoxConnexionState(bool state)
{ if (state)
    { connect( m_check_Lundi,    SIGNAL( stateChanged ( int  )  ),    this , SLOT( Slot_checkStateChanged( int ) ));
      connect( m_check_Mardi,    SIGNAL( stateChanged ( int  )  ),    this , SLOT( Slot_checkStateChanged( int ) ));
      connect( m_check_Mercredi, SIGNAL( stateChanged ( int  )  ),    this , SLOT( Slot_checkStateChanged( int ) ));
      connect( m_check_Jeudi,    SIGNAL( stateChanged ( int  )  ),    this , SLOT( Slot_checkStateChanged( int ) ));
      connect( m_check_Vendredi, SIGNAL( stateChanged ( int  )  ),    this , SLOT( Slot_checkStateChanged( int ) ));
      connect( m_check_Samedi,   SIGNAL( stateChanged ( int  )  ),    this , SLOT( Slot_checkStateChanged( int ) ));
      connect( m_check_Dimanche, SIGNAL( stateChanged ( int  )  ),    this , SLOT( Slot_checkStateChanged( int ) ));
    }
  else
    {
      disconnect( m_check_Lundi,    SIGNAL( stateChanged ( int  )  ),    0 , 0 );
      disconnect( m_check_Mardi,    SIGNAL( stateChanged ( int  )  ),    0 , 0 );
      disconnect( m_check_Mercredi, SIGNAL( stateChanged ( int  )  ),    0 , 0 );
      disconnect( m_check_Jeudi,    SIGNAL( stateChanged ( int  )  ),    0 , 0 );
      disconnect( m_check_Vendredi, SIGNAL( stateChanged ( int  )  ),    0 , 0 );
      disconnect( m_check_Samedi,   SIGNAL( stateChanged ( int  )  ),    0 , 0 );
      disconnect( m_check_Dimanche, SIGNAL( stateChanged ( int  )  ),    0 , 0 );
    }
}
//------------------------ Slot_checkStateChanged ---------------------------------------
void C_Frm_Sequence::Slot_checkStateChanged( int )
{emit Sign_contentChanged(this);
}

//------------------------ Slot_textChanged ---------------------------------------
void C_Frm_Sequence::Slot_textChanged(const QString &, CMyQLineEdit *, void *)
{ emit Sign_contentChanged(this);
}

//------------------------ Slot_PosoPointChanged ---------------------------------------
void C_Frm_Sequence::Slot_PosoPointChanged(C_PopupDialPosoPoint*)
{ emit Sign_contentChanged(this);
}

//------------------------ Slot_PosoPointDeployed -----------------------------------------------
// recu lorsqu'une PosoPoint se deploie il convient alors de plier les autres
void C_Frm_Sequence::Slot_PosoPointDeployed(const QString &pointName)
{//.......... reduction des autres dialogues ......................................
 if (m_PopupPosoPoint_Matin->name()   != pointName)     m_PopupPosoPoint_Matin->setToInitialRect();
 if (m_PopupPosoPoint_Midi->name()    != pointName)     m_PopupPosoPoint_Midi->setToInitialRect();
 if (m_PopupPosoPoint_AMidi->name()   != pointName)     m_PopupPosoPoint_AMidi->setToInitialRect();
 if (m_PopupPosoPoint_Soir->name()    != pointName)     m_PopupPosoPoint_Soir->setToInitialRect();
 if (m_PopupPosoPoint_Coucher->name() != pointName)     m_PopupPosoPoint_Coucher->setToInitialRect();
 if (m_PopupPosoPoint_Nuit->name()    != pointName)     m_PopupPosoPoint_Nuit->setToInitialRect();
}
/*
//------------------------ addSubMenu ---------------------------------------
void C_Frm_Sequence::addSubMenu(QMenu* pQMenu, const QString &subOptionsTitle, const QStringList &subOptionsList, const QFont &fGrd)
{   QAction *pQAction = 0;
    QString      num  = "";
    QString    title  = subOptionsTitle;
    int pos           = title.indexOf('-');
    if (pos != -1)
       { num   = title.left(pos).trimmed();
         title = title.mid(pos+1).trimmed();
       }

    QMenu *pSubQMenu  = pQMenu->addMenu(title);
    pSubQMenu->setObjectName( tr("HitList : sub_menu %1").arg(title));
    pSubQMenu->setFont(fGrd);

    for (int i=0; i<subOptionsList.size(); ++i)
        {if (subOptionsList[i].startsWith("-----"))
            {pQAction = pSubQMenu->addSeparator();
            }
         else
            {
              pQAction = pSubQMenu->addAction(subOptionsList[i]);
              pQAction->setObjectName( tr("HitList : sub_menu_action %1").arg( title + "@" + subOptionsList[i]));
              pQAction->setData(num+"."+QString::number(i));
            }
        }
}
*/
//------------------------ Slot_MenuTriggered ---------------------------------------
void C_Frm_Sequence::Slot_MenuTriggered( QAction *pQAction  )
{QString idMenu      = pQAction->data().toString();
 QString idOptionSub = "";
 int pos = idMenu.indexOf('.');

 if (pos != -1)
    {idOptionSub = idMenu.mid(pos+1);
     idMenu      = idMenu.left(pos);
    }
 int id = idMenu.left(1).toInt();
 switch(id)
   {case 1: controlMenuDayRepart(      pQAction->objectName(), idMenu.toInt(), idOptionSub.toInt() );  break;
    case 2: setMomentMeal(             pQAction->objectName(), idMenu.toInt(), idOptionSub.toInt() );  break;
    case 3: controlMenuQSP(            pQAction->objectName(), idMenu.toInt(), idOptionSub.toInt() );  break;
    case 4: controlMenuQSP_Unit(       pQAction->text(),       idMenu.toInt(), idOptionSub.toInt() );  break;
    case 5: controlMenuSequence(       pQAction->text(),       idMenu.toInt(), idOptionSub.toInt() );  break;
    case 6: controlMenuEveryTimeUnit(  pQAction->text(),       idMenu.toInt(), idOptionSub.toInt() );  break;
    case 7: controlMenuEveryTimeValue( pQAction->text(),       idMenu.toInt(), idOptionSub.toInt() );  break;
    case 8: controlMenuVoie(           pQAction->text(),       idMenu.toInt(), idOptionSub.toInt() );  break;
  }
}
//------------------------ controlMenuVoie ---------------------------------------
void C_Frm_Sequence::controlMenuVoie(const QString& optionName, int idMenu, int )
{    Q_UNUSED(idMenu);
     m_LineEdit_Voie->setText(optionName);
}

//------------------------ controlMenuEveryTimeValue ---------------------------------------
/*
701 - tous les jours
701 - tous les 2 jours
*/
void C_Frm_Sequence::controlMenuEveryTimeValue(const QString& optionName, int idMenu, int )
{
  QString unit  = "";
  QString value = "";
  if (idMenu==700)
     { m_EveryQuEdit->setText("");
       return;
     }
  int pos = optionName.length();
  while ( pos > 0 && optionName[pos-1] != ' ' ) --pos;
  if ( pos > 0 )
     { unit = optionName.mid(pos);
       m_EveryUnitBut->setText(unit);
     }
  int endValue = pos;
  while ( pos > 0 && (optionName[pos-1] == ' ' || (optionName[pos-1] >= '0' && optionName[pos-1] <= '9') ) ) --pos;
  if ( pos > 0 )
     { value = optionName.mid(pos,endValue-pos).trimmed();
       if (value.length()==0) value='1';
       m_EveryQuEdit->setText(value);
     }
}

//------------------------ controlMenuEveryTimeUnit ---------------------------------------
void C_Frm_Sequence::controlMenuEveryTimeUnit(const QString& optionName, int , int )
{ m_EveryUnitBut->setText(optionName);
  emit Sign_contentChanged(this);
}

//------------------------ controlMenuSequence ---------------------------------------
void C_Frm_Sequence::controlMenuSequence(const QString& optionName, int idMenu, int)
{switch(idMenu)
 {setCheckBoxConnexionState(false);
  case 501: setWeekDaysCheckBoxVisibility(false);    // chek box invisible :  on rend visible la saisie de tous les ... et on cache les check box
            setWeekDaysCheckBoxEnabled( true );
            m_check_MenuBut->setObjectName(tr("%1|HitList : m_check_MenuBut 505").arg(optionName));
            setDureeControlsEnabled( true );
            break;
  case 502: setWeekDaysCheckBoxVisibility(true);     // chek box visible : on rend visible les check box et on cache la saisie de tous les ...
            setWeekDaysCheckBoxEnabled( true );      // tr("502 - ne prendre que pour les jours ci-dessous")
            setDureeControlsEnabled( false );
            m_check_MenuBut->setObjectName(tr("%1|HitList : m_check_MenuBut 502").arg(optionName));
            break;
  case 503: setWeekDaysCheckBoxVisibility(true);     // chek box visible :  on rend visible les check box et on cache la saisie de tous les ...
            setWeekDaysCheckBoxEnabled( true );      // tr("503 - exclure les jours ci-dessous")
            m_check_MenuBut->setObjectName(tr("%1|HitList : m_check_MenuBut 503").arg( optionName ));
            setDureeControlsEnabled( false );
            break;
  case 504: setWeekDaysCheckBoxVisibility(true);     // chek box visible :  on rend visible les check box et on cache la saisie de tous les ...
            setWeekDaysCheckBoxEnabled( true );
            setWeekDaysCheckBox("sd",   false);      // tr("504 - sauf samedi dimanche")
            m_check_MenuBut->setObjectName(tr("%1|HitList : m_check_MenuBut 504").arg(optionName));
            setDureeControlsEnabled( false );
            break;
 }
 m_check_MenuBut->setColor(m_backGround);
 setCheckBoxConnexionState(true);
 emit Sign_contentChanged(this);
}

//------------------------ setDureeControlsEnabled ---------------------------------------
void C_Frm_Sequence::setDureeControlsEnabled(bool state)
{
 m_Button_Pendant->setEnabled(state);
 m_LineEdit_Duree->setEnabled(state);
 m_Button_DureeUnite->setEnabled(state);

}
//------------------------ controlMenuQSP_Unit ---------------------------------------
void C_Frm_Sequence::controlMenuQSP_Unit(const QString& optionName, int idMenu, int )
{   if (idMenu==466)                      // si prise unique mettre 1 comme duree
       {m_Button_DureeUnite->setText(optionName);
        m_LineEdit_Duree->setText("1");   // emet le  Sign_contentChanged(this);
        return;
       }
    m_Button_DureeUnite->setText(optionName);
    emit Sign_contentChanged(this);
}
//------------------------ controlMenuQSP ---------------------------------------
void C_Frm_Sequence::controlMenuQSP(const QString & , int idMenu, int )
{   idMenu -= 300;
    if (idMenu==0)        m_LineEdit_Duree->setText("");
    else                  m_LineEdit_Duree->setText(QString::number(idMenu));
    // emit Sign_contentChanged(this); inutile car changement notifie par le m_LineEdit_Duree changetext
}

//------------------------ setMomentMeal ---------------------------------------
void C_Frm_Sequence::setMomentMeal(const QString & , int idMenu, int )
{   m_PopupPosoPoint_Matin->setMomentMeal(   idMenu );;
    m_PopupPosoPoint_Midi->setMomentMeal(    idMenu );
    m_PopupPosoPoint_AMidi->setMomentMeal(   idMenu );
    m_PopupPosoPoint_Soir->setMomentMeal(    idMenu );
    m_PopupPosoPoint_Coucher->setMomentMeal( idMenu );
    m_PopupPosoPoint_Nuit->setMomentMeal(    idMenu );
   // emit Sign_contentChanged(this); inutile car changement notifie par le m_PopupPosoPoint
}

//------------------------ controlMenuDayRepart ---------------------------------------
void C_Frm_Sequence::controlMenuDayRepart(const QString& optionName, int idMenu, int /*idOptionSub*/)
{
    Q_UNUSED(optionName);
    clearDaysZoneEditors();
    QString  s_quantite  = m_LineEdit_TotalJour->text().trimmed();
    DOUBLE_PAIR val_pair = C_PosologieGrammar::strTo_DOUBLE_PAIR(s_quantite);

    if ( val_pair.v1==0 && val_pair.v2==0 ) return;    //TODO ALERTER

    int gph_psecable   = CGestIni::getXmlData("gph_psecable",  m_cur_grammar).toInt();

    // DOUBLE_PAIR   d1=C_PosologieGrammar::pair_divBy(val_pair,1);
    DOUBLE_PAIR   d2=C_PosologieGrammar::pair_divBy(val_pair,2);
    DOUBLE_PAIR   d3=C_PosologieGrammar::pair_divBy(val_pair,3);
    DOUBLE_PAIR   d4=C_PosologieGrammar::pair_divBy(val_pair,4);
    DOUBLE_PAIR   d5=C_PosologieGrammar::pair_divBy(val_pair,5);
    DOUBLE_PAIR   d6=C_PosologieGrammar::pair_divBy(val_pair,6);

    switch(idMenu)
    {case 102:      m_PopupPosoPoint_Matin->setQuantite(val_pair,gph_psecable);    break;
     case 103:      m_PopupPosoPoint_Midi->setQuantite(val_pair,gph_psecable);     break;
     case 104:      m_PopupPosoPoint_AMidi->setQuantite(val_pair,gph_psecable);    break;
     case 105:      m_PopupPosoPoint_Soir->setQuantite(val_pair,gph_psecable);     break;
     case 106:      m_PopupPosoPoint_Coucher->setQuantite(val_pair,gph_psecable);  break;
     case 107:      m_PopupPosoPoint_Nuit->setQuantite(val_pair,gph_psecable);     break;

     case 108:      m_PopupPosoPoint_Matin->setQuantite(d2,gph_psecable);
                    m_PopupPosoPoint_Midi->setQuantite(d2,gph_psecable);     break;
     case 109:      m_PopupPosoPoint_Matin->setQuantite(d2,gph_psecable);
                    m_PopupPosoPoint_Soir->setQuantite(d2,gph_psecable);     break;
     case 110:      m_PopupPosoPoint_Matin->setQuantite(d2,gph_psecable);
                    m_PopupPosoPoint_AMidi->setQuantite(d2,gph_psecable);    break;
     case 111:      m_PopupPosoPoint_Matin->setQuantite(d2,gph_psecable);
                    m_PopupPosoPoint_Coucher->setQuantite(d2,gph_psecable);  break;
     case 112:      m_PopupPosoPoint_Midi->setQuantite(d2,gph_psecable);
                    m_PopupPosoPoint_Soir->setQuantite(d2,gph_psecable);     break;
     case 113:      m_PopupPosoPoint_Coucher->setQuantite(d2,gph_psecable);
                    m_PopupPosoPoint_Nuit->setQuantite(d2,gph_psecable);     break;
     case 114:      m_PopupPosoPoint_Matin->setQuantite(d3,gph_psecable);
                    m_PopupPosoPoint_Midi->setQuantite(d3,gph_psecable);
                    m_PopupPosoPoint_Soir->setQuantite(d3,gph_psecable);     break;
     case 115:      m_PopupPosoPoint_Matin->setQuantite(d4,gph_psecable);
                    m_PopupPosoPoint_Midi->setQuantite(d4,gph_psecable);
                    m_PopupPosoPoint_Soir->setQuantite(d4,gph_psecable);
                    m_PopupPosoPoint_Coucher->setQuantite(d4,gph_psecable);  break;
     case 116:      m_PopupPosoPoint_Matin->setQuantite(d5,gph_psecable);
                    m_PopupPosoPoint_Midi->setQuantite(d5,gph_psecable);
                    m_PopupPosoPoint_AMidi->setQuantite(d5,gph_psecable);
                    m_PopupPosoPoint_Soir->setQuantite(d5,gph_psecable);
                    m_PopupPosoPoint_Coucher->setQuantite(d5,gph_psecable);  break;
     case 117:      m_PopupPosoPoint_Matin->setQuantite(d6,gph_psecable);
                    m_PopupPosoPoint_Midi->setQuantite(d6,gph_psecable);
                    m_PopupPosoPoint_AMidi->setQuantite(d6,gph_psecable);
                    m_PopupPosoPoint_Soir->setQuantite(d6,gph_psecable);
                    m_PopupPosoPoint_Coucher->setQuantite(d6,gph_psecable);
                    m_PopupPosoPoint_Nuit->setQuantite(d6,gph_psecable);     break;
    }
    emit Sign_contentChanged(this);
}

//------------------------ adjustDecPartString ---------------------------------------
QString C_Frm_Sequence::adjustDecPartString(const QStringList &vList, double divide)
{ QString result    = "";
  QString   ret     = "";
  int       end     =  vList.size();
  for (int i=0; i<end; ++i)
     {double  f_quantite = vList[i].toFloat();
      int     decPart    = 0;
      result             = QString::number(f_quantite/divide,'f',2);
      int          pos   = result.indexOf('.');
      if (pos==-1) pos   = result.indexOf(',');
      if (pos==-1) continue;
      if ( i )   ret    +="-";
      decPart            = result.mid(pos+1).toInt();    //decPart  = (result.mid(pos+1)+"00").left(2).toInt();
      if      (decPart>=0  && decPart<21)            ret  += result.left(pos);
      else if (decPart>=21 && decPart<35)            ret  += result.left(pos)+".25";
      else if (decPart>=35 && decPart<65)            ret  += result.left(pos)+".50";
      else if (decPart>=65 && decPart<85)            ret  += result.left(pos)+".75";
      else if (decPart>=85 )                         ret  += QString::number(result.left(pos).toInt()+1);
     }
  return ret;
}
//------------------------ sequenceToInterface ---------------------------------------
//"sous-cutanee@injection(s)[0.75;m|155.122;d15r|2;sr30|3;h10-13]/4\n"
void C_Frm_Sequence::sequenceToInterface(const QString &sequence_in)
{QString    sequence = sequence_in;
 QString     res_qu  = "";
 int         pos_deb = sequence.indexOf('@');
 if (pos_deb != -1)
    {m_LineEdit_Voie->setText ( sequence.left(pos_deb));
     sequence = sequence.mid   ( pos_deb+1 );
    }
             pos_deb = sequence.indexOf('[');
 if (pos_deb != -1)
    { m_LineEdit_Forme->setText ( sequence.left(pos_deb).remove("(s)") );
      sequence = sequence.mid   ( pos_deb+1 );
    }
 int             pos = sequence.indexOf(']');
 if (pos==-1 ) return ;

 int pcom            = sequence.indexOf('!');
 int len             = -1;    // si pas de commentaire le mid prend tout
 QString commentaire = "";
 if (pcom != -1)
    { commentaire = sequence.mid(pcom+1);
      m_LineEdit_comm->setText(commentaire);
      len = pcom-pos-1;
    }
 QString seq_days      = sequence.mid(pos + 1, len);
 QString seq_time      = sequence.left(pos).remove("[").trimmed();
 QStringList   seqList = seq_time.split('|');
 DOUBLE_PAIR ret,r;
 ret.v1     = 0;
 ret.v2     = 0;
 ret.isPair = 0;
 for (int i=0; i<seqList.count(); ++i)
     { r       = sequenceTimeToInterface(seqList[i]);
       ret.v1     += r.v1;
       ret.v2     += r.v2;
       ret.isPair += r.isPair;
     }
 if (ret.isPair && ret.v1!=0)
    { res_qu = C_PosologieGrammar::p_Str(ret.v1, 2) + "-" + C_PosologieGrammar::p_Str(ret.v2, 2);
    }
 else
    { res_qu = C_PosologieGrammar::p_Str(ret.v2, 2);
    }
 m_LineEdit_TotalJour->setText(res_qu);
 sequenceDaysToInterface(seq_days);
 return ;
}

//------------------------ sequenceTimeToInterface ---------------------------------------
//"155.122;d15r"
//"3;h10-13"
DOUBLE_PAIR C_Frm_Sequence::sequenceTimeToInterface(const QString &sequence)
{   DOUBLE_PAIR ret;
    QString r        = "";
    QString before   = "";
    QString after    = "";
    int          pos = sequence.indexOf(';');
    QString       qu = sequence.left(pos);    ++pos;  // recuperer quantite et passer le ';'
    QString        m = sequence.mid(pos,1);   ++pos;  // recupere le type de periode (m d a s c n h) et passer son symbole

    // m est une de ces valeurs : m d a s c n h
    if (m.length()==0)  // cas  paire ou fourchette de valeur exple "1-3;"   pas de specification de moment de prise
       { ret = C_PosologieGrammar::strTo_DOUBLE_PAIR(qu);
       }
    else if (m=="h")     //"3;h10-13" //"3;h10"
       {m    = sequence.mid(pos);  // h10-13   h10 d15r sr15 mr
        pos  = m.indexOf('-');
        if (pos != -1)
           {  before   = m.left(pos);
              after    = m.mid(pos+1);
              ret      = sequenceHoursToInterface(before, after, qu);
           }
        else
           {  ret      = sequenceHoursToInterface(m, "", qu);
           }
       }
    else   // m est une de ces valeurs  m d a s c n
       {r              = sequence.mid(pos);        // recuperer sequence du repas
        pos            = r.indexOf('r');
        int momentMeal = 201;    // "201 - repas sans importance"
        QString    tps = "";
        if  (pos != -1)
            { before       = r.left(pos);
              after        = r.mid(pos+1);
              int before_n = before.toInt();
              int after_n  = after.toInt();
              if ( after=="0" && before=="0" )       momentMeal = 203;          // "203 - pendant le repas"
              else if ( before=="0" )                momentMeal = 202;          // "202 - avant le repas""
              else if ( after=="0" )                 momentMeal = 204;          // "204 - apr\303\250s le repas"
              else if ( before_n==0 && after_n==0 )  momentMeal = 203;          // "203 - pendant le repas"
              else //if (before_n||after_n)
                 {if      (before.length()) {tps =  before;  momentMeal = 202;}    // " minutes avant le repas"
                  else if (after.length() ) {tps =  after;   momentMeal = 204;}    // " minutes apr\303\250s le repas"
                  else                      {tps =  "";      momentMeal = 203;}    // " au cours du repas"
                 }
            }
        char c = m[0].toLatin1()|32;
        switch(c)
           {
           case 'm':
                m_PopupPosoPoint_Matin->setQuantite(qu);
                m_PopupPosoPoint_Matin->setMomentMeal(momentMeal);
                m_PopupPosoPoint_Matin->setMomentTimeMeal(tps);
                break;
           case 'd':
                m_PopupPosoPoint_Midi->setQuantite(qu);
                m_PopupPosoPoint_Midi->setMomentMeal(momentMeal);
                m_PopupPosoPoint_Midi->setMomentTimeMeal(tps);
                break;
           case 'a':
                m_PopupPosoPoint_AMidi->setQuantite(qu);
                m_PopupPosoPoint_AMidi->setMomentMeal(momentMeal);
                m_PopupPosoPoint_AMidi->setMomentTimeMeal(tps);
                break;
           case 's':
                m_PopupPosoPoint_Soir->setQuantite(qu);
                m_PopupPosoPoint_Soir->setMomentMeal(momentMeal);
                m_PopupPosoPoint_Soir->setMomentTimeMeal(tps);
                break;
           case 'c':
                m_PopupPosoPoint_Coucher->setQuantite(qu);
                m_PopupPosoPoint_Coucher->setMomentMeal(momentMeal);
                m_PopupPosoPoint_Coucher->setMomentTimeMeal(tps);
                break;
           case 'n':
                m_PopupPosoPoint_Nuit->setQuantite(qu);
                m_PopupPosoPoint_Nuit->setMomentMeal(momentMeal);
                m_PopupPosoPoint_Nuit->setMomentTimeMeal(tps);
                break;
           }
        ret.v1       = 0;
        ret.v2       = qu.toDouble();
        ret.isPair   = 0;
       }
    return ret;
}

//------------------------ sequenceHoursToInterface ---------------------------------------
//"/4"
DOUBLE_PAIR C_Frm_Sequence::sequenceHoursToInterface(const QString &before, const QString &after, const QString &value)
{
 DOUBLE_PAIR ret;
 ret.unit      = "heure";
 int h_deb     = before.toInt();
 int h_fin     = after.toInt();
 QString hours = "";
 if (before.length() && after.length())
    {if (h_deb>=6 && h_fin<11)
        {m_PopupPosoPoint_Matin->setQuantite(value);
        }
     else if (h_deb>=11 && h_fin<14)
       {m_PopupPosoPoint_Midi->setQuantite(value);
       }
     else if (h_deb>=14 && h_fin<19)
       {m_PopupPosoPoint_AMidi->setQuantite(value);
       }
     else if (h_deb>=19 && h_fin<21)
       {m_PopupPosoPoint_Soir->setQuantite(value);
       }
     else if (h_deb>=21 && h_fin<24)
       {m_PopupPosoPoint_Coucher->setQuantite(value);
       }
     else if (h_deb>=0 && h_fin<6)
       {m_PopupPosoPoint_Nuit->setQuantite(value);
       }
     ret.v1      = before.toDouble();
     ret.v2      = after.toDouble();
     ret.isPair  = 1;
     return ret;
    }
 else if (before.length())
   {hours = before;
   }
 else if (after.length())
   {hours = after;
   }
 if  (hours.length())
     {h_deb = hours.toInt();
      if (h_deb>=6 && h_deb<11)
        {m_PopupPosoPoint_Matin->setQuantite(value);
        }
      else if (h_deb>=11 && h_deb<14)
        {m_PopupPosoPoint_Midi->setQuantite(value);
        }
      else if (h_deb>=14 && h_deb<19)
        {m_PopupPosoPoint_AMidi->setQuantite(value);
        }
      else if (h_deb>=19 && h_deb<21)
        {m_PopupPosoPoint_Soir->setQuantite(value);
        }
      else if (h_deb>=21 && h_deb<24)
        {m_PopupPosoPoint_Coucher->setQuantite(value);
        }
      else if (h_deb>=0 && h_deb<6)
        {m_PopupPosoPoint_Nuit->setQuantite(value);
        }
     }
 ret.v1      = 0;
 ret.v2      = hours.toDouble();
 ret.isPair  = 0;
 return ret;
}


//------------------------ sequenceDaysToInterface ---------------------------------------
/*
#define DETAILED_PERIODDAYS     "d\303\251tailler les dur\303\251es"
#define WEEKDAYS_TO_INCLUDE     "semainier : inclusion (le...)"
#define WEEKDAYS_TO_EXCLUDE     "semainier : exclusion (sauf..)"
#define WEEKDAYS_EXCLUDE_WE     "semainier : sauf Samedi Dimanche"
*/
void C_Frm_Sequence::sequenceDaysToInterface(const QString &_seq_days)
{   if (_seq_days.length()==0) return;
    int       pos = 0;    // position de la quantite de J ou S ou M ou H ou A a priori apres le caractere J ou S ou M ou H
    char        t = _seq_days[pos].toLatin1()|32;
    QString  s_qu = "";
    //........ la periode de la sequence (U3S: U12M: U21J: ...) .......................
    //         si precisee doit etre suivie apres les deux points : de la duree de la sequence
    if ( t == 'u' )
       {
         pos               = _seq_days.indexOf(':');
         char unit_tousles = 0;
         if ( pos != -1 )
            {  s_qu         = _seq_days.mid(1,pos-2);
               unit_tousles = _seq_days[pos-1].toLatin1()|32;
            }
         switch (unit_tousles)
                { case 'h':  m_EveryUnitBut->setText(tr("heures"));    m_EveryQuEdit->setText(s_qu); break;
                  case 'j':  m_EveryUnitBut->setText(tr("jours"));     m_EveryQuEdit->setText(s_qu); break;
                  case 's':  m_EveryUnitBut->setText(tr("semaines"));  m_EveryQuEdit->setText(s_qu); break;
                  case 'm':  m_EveryUnitBut->setText(tr("mois"));      m_EveryQuEdit->setText(s_qu); break;
                  case 'a':  m_EveryUnitBut->setText(tr("ans"));       m_EveryQuEdit->setText(s_qu); break;
                }
         ++pos;                                      // se placer apres les deux points
         if ( pos+2 > _seq_days.length() )   return; // ==> CASSOS si pas de sequence d'au moins 2 car (la lettre clef et sa duree) cassos car sinon bug
         t = _seq_days[pos].toLatin1()|32;           // recuperer lettre clef apres les deux points
      }
    //........ la duree de la sequence (J32 M4 S8 ...) .......................
    s_qu = _seq_days.mid(  pos + 1 );
    switch (t)
    { case 'i': setWeekDaysCheckBox(s_qu, true);                           // IlmMjvsd I comme Include
                controlMenuSequence( WEEKDAYS_TO_INCLUDE , 502);           // ne prendre que pour les jours ci-dessous
                return;
      case 'e': setWeekDaysCheckBox(s_qu, false);                           // ElmMjvsd E comme Exclude
                controlMenuSequence( WEEKDAYS_TO_EXCLUDE, 503);             // exclure les jours ci-dessous
                return;
      case 'j': m_Button_DureeUnite->setText(tr("jour(s)"));                m_LineEdit_Duree->setText(s_qu);   break;
      case 's': m_Button_DureeUnite->setText(tr("semaine(s)"));             m_LineEdit_Duree->setText(s_qu);   break;
      case 'm': m_Button_DureeUnite->setText(tr("mois"));                   m_LineEdit_Duree->setText(s_qu);   break;
      case 'a': m_Button_DureeUnite->setText(tr("ann\303\251e(s)"));        m_LineEdit_Duree->setText(s_qu);   break;
    }
    controlMenuSequence( DETAILED_PERIODDAYS ,501);                       // jours non d\303\251taill\303\251s
}

//------------------------ clearInterface ---------------------------------------
void C_Frm_Sequence::clearInterface()
{
    clearDaysZoneEditors();

    m_LineEdit_Duree->clear();
    m_LineEdit_TotalJour->clear();
    m_LineEdit_comm->clear();
    m_EveryQuEdit->clear();
    //........ detail des jours inactif .............................
    setWeekDaysCheckBox("lmMjvsd", true);
    controlMenuSequence(DETAILED_PERIODDAYS, 501);                       // jours non d\303\251taill\303\251s
}
//------------------------ weekDaysCheckBoxToString ---------------------------------------
QString C_Frm_Sequence::weekDaysCheckBoxToString(char mode)
{   QString ret = "";    // Qt::CheckStatecheckState ()
    if (mode=='I')
       { if ( m_check_Lundi->isChecked())      ret += "l";
         if ( m_check_Mardi->isChecked())      ret += "m";
         if ( m_check_Mercredi->isChecked())   ret += "M";
         if ( m_check_Jeudi->isChecked())      ret += "j";
         if ( m_check_Vendredi->isChecked())   ret += "v";
         if ( m_check_Samedi->isChecked())     ret += "s";
         if ( m_check_Dimanche->isChecked())   ret += "d";
       }
    else
       {if ( ! (m_check_Lundi->isChecked()))      ret += "l";
        if ( ! (m_check_Mardi->isChecked()))      ret += "m";
        if ( ! (m_check_Mercredi->isChecked()))   ret += "M";
        if ( ! (m_check_Jeudi->isChecked()))      ret += "j";
        if ( ! (m_check_Vendredi->isChecked()))   ret += "v";
        if ( ! (m_check_Samedi->isChecked()))     ret += "s";
        if ( ! (m_check_Dimanche->isChecked()))   ret += "d";
       }
    return ret;
}
//------------------------ setWeekDaysCheckBox ---------------------------------------
int C_Frm_Sequence::setWeekDaysCheckBox(const QString &daysList, bool isInclude)
{    bool inverse = isInclude^1;
     setCheckBoxConnexionState(false);
     m_check_Lundi->setChecked(inverse);
     m_check_Mardi->setChecked(inverse);
     m_check_Mercredi->setChecked(inverse);
     m_check_Jeudi->setChecked(inverse);
     m_check_Vendredi->setChecked(inverse);
     m_check_Samedi->setChecked(inverse);
     m_check_Dimanche->setChecked(inverse);

    //const char *pt = daysList.toLatin1().constData();   // pas fiable du tout le coup du pt
    int        len = daysList.length();
    for (int i=0; i<len;++i)
      { switch(daysList.toLatin1()[i])      // attention dans les specification des jours de la semaine la case compte M et m
         { case 'l': m_check_Lundi->setChecked(isInclude);     break;
           case 'm': m_check_Mardi->setChecked(isInclude);     break;
           case 'M': m_check_Mercredi->setChecked(isInclude);  break;
           case 'j': m_check_Jeudi->setChecked(isInclude);     break;
           case 'v': m_check_Vendredi->setChecked(isInclude);  break;
           case 's': m_check_Samedi->setChecked(isInclude);    break;
           case 'd': m_check_Dimanche->setChecked(isInclude);  break;
         }
      }
    setCheckBoxConnexionState(true);
    return len;
}
//------------------------ setWeekDaysCheckBoxVisibility ---------------------------------------
void  C_Frm_Sequence::setWeekDaysCheckBoxVisibility( bool visibility)
{ m_check_Lundi->setVisible(visibility);
  m_check_Mardi->setVisible(visibility);
  m_check_Mercredi->setVisible(visibility);
  m_check_Jeudi->setVisible(visibility);
  m_check_Vendredi->setVisible(visibility);
  m_check_Samedi->setVisible(visibility);
  m_check_Dimanche->setVisible(visibility);

  visibility = (visibility==false);
  m_EveryQuEdit->setVisible(visibility);
  m_EveryUnitBut->setVisible(visibility);
  m_EveryQuBut->setVisible(visibility);
  m_Button_Pendant->setVisible(visibility);
  m_LineEdit_Duree->setVisible(visibility);
  m_Button_DureeUnite->setVisible(visibility);
}
//------------------------ setWeekDaysCheckBox ---------------------------------------
void  C_Frm_Sequence::setWeekDaysCheckBoxEnabled( bool isEnabled)
{ m_check_Lundi->setEnabled(isEnabled);
  m_check_Mardi->setEnabled(isEnabled);
  m_check_Mercredi->setEnabled(isEnabled);
  m_check_Jeudi->setEnabled(isEnabled);
  m_check_Vendredi->setEnabled(isEnabled);
  m_check_Samedi->setEnabled(isEnabled);
  m_check_Dimanche->setEnabled(isEnabled);
}
//------------------------ clearDaysZoneEditors ---------------------------------------
void C_Frm_Sequence::clearDaysZoneEditors()
{
    m_PopupPosoPoint_Matin->setQuantite("");;
    m_PopupPosoPoint_Midi->setQuantite("");
    m_PopupPosoPoint_AMidi->setQuantite("");
    m_PopupPosoPoint_Soir->setQuantite("");
    m_PopupPosoPoint_Coucher->setQuantite("");
    m_PopupPosoPoint_Nuit->setQuantite("");

    m_PopupPosoPoint_Matin->setMomentTimeMeal("");;
    m_PopupPosoPoint_Midi->setMomentTimeMeal("");
    m_PopupPosoPoint_AMidi->setMomentTimeMeal("");
    m_PopupPosoPoint_Soir->setMomentTimeMeal("");
    m_PopupPosoPoint_Coucher->setMomentTimeMeal("");
    m_PopupPosoPoint_Nuit->setMomentTimeMeal("");
}
//------------------------ interfaceToGrammar ---------------------------------------
QString C_Frm_Sequence::interfaceToGrammar( QString &forme,  QString &voie)
{QString tagDeb      = " <gpp>";
 QString tagEnd      = "</gpp>\n";
 QString   ret       = tagDeb;
 QString pointsPoso  = "";
 voie        = m_LineEdit_Voie->text();     // relever la forme de m_LineEdit_Voie
 forme       = m_LineEdit_Forme->text();    // relever la forme de m_LineEdit_Forme
 ret   += voie+"@"+forme+"[";
 //............... recuperer les points de prise ...................
 if (m_PopupPosoPoint_Matin->getQuantite().length())
    {pointsPoso += m_PopupPosoPoint_Matin->interfaceToGrammar()+"|";
    }
 if (m_PopupPosoPoint_Midi->getQuantite().length())
    {pointsPoso += m_PopupPosoPoint_Midi->interfaceToGrammar()+"|";
    }
 if (m_PopupPosoPoint_AMidi->getQuantite().length())
    {pointsPoso += m_PopupPosoPoint_AMidi->interfaceToGrammar()+"|";
    }
 if (m_PopupPosoPoint_Soir->getQuantite().length())
    {pointsPoso += m_PopupPosoPoint_Soir->interfaceToGrammar()+"|";
    }
 if (m_PopupPosoPoint_Coucher->getQuantite().length())
    {pointsPoso += m_PopupPosoPoint_Coucher->interfaceToGrammar()+"|";
    }
 if (m_PopupPosoPoint_Nuit->getQuantite().length())
    {pointsPoso += m_PopupPosoPoint_Nuit->interfaceToGrammar()+"|";
    }
 if (pointsPoso.endsWith('|')) pointsPoso = pointsPoso.left(pointsPoso.length()-1);
 //........... si points de prise vides recuperer dose totale ....................
 if (pointsPoso.length()==0)
    {pointsPoso  =  m_LineEdit_TotalJour->text().trimmed();
     if (pointsPoso.length()==0) return "";                        // aucune quantite definie la posologie est inexploitable
     pointsPoso += ";";
    }
 ret += pointsPoso +"]";
 //............ soit on tient compte de la saisie de la duree de la sequence ..............
 if (m_Button_DureeUnite->isEnabled())
    { QString duree   = m_LineEdit_Duree->text();
      QString tousles = m_EveryQuEdit->text();

      //  .... placer l'eventuelle mention de periodicite "tous les" ...........
      if ( tousles.length() )         // la mention 'tous les' d'une sequence
         {  ret +="U" + tousles + m_EveryUnitBut->text().at(0).toUpper() + ":";  // U3S:M6  toutes les 3 semaines pendant 6 mois
         }

      if ( duree.length() )       // la mention 'pendant' d'une sequence
         {  ret += m_Button_DureeUnite->text().at(0).toUpper() + duree;          // M6  pendant 6 mois
         }
     }
    //............ soit on tient compte de la saisie des jours de la semaine ..............
    else
     { QString days = "";
       int idMenu = m_check_MenuBut->objectName().right(3).toInt();
       switch (idMenu)
          {case 502: days = weekDaysCheckBoxToString('I');
                     if (days.length()) ret += 'I' + days; break;              // "502 - ne prendre que pour les jours ci-dessous"
           case 504:                                                           // "504 - sauf samedi dimanche"
           case 503: days = weekDaysCheckBoxToString('E');
                     if (days.length()) ret += 'E' + days; break;              // "502 - exclure les jours ci-dessous"
          }   // end switch (idMenu)
     } // end else if (m_Button_DureeUnite->isEnabled())

 if (m_LineEdit_comm->text().length()) ret += "!" + m_LineEdit_comm->text();
 ret += tagEnd;
 return ret;
}

//------------------------------------------------------ event ----------------------------------------------------------------------
bool C_Frm_Sequence::event ( QEvent * ev )
{
return QFrame::event (ev);
}

//------------------------------------------------------ paintEvent ----------------------------------------------------------------------
void C_Frm_Sequence::paintEvent ( QPaintEvent * /*event*/ )
{   QPainter p(this);
    //........... cadre a bords ronds .................................
    QRect rect;
    rect = this->rect();
    rect.setWidth  (this->rect().width() -1); // -1 car bord droit et bas masques par le widget
    rect.setHeight (this->rect().height()-1); // -1 car bord droit et bas masques par le widget
    p.setPen (m_borderColor.lighter(150));
    p.drawRoundedRect (rect ,6, 6 );

    rect = this->rect();
    rect.setWidth  (this->rect().width() -3); // -1 car bord droit et bas masques par le widget
    rect.setHeight (this->rect().height()-3); // -1 car bord droit et bas masques par le widget
    rect.translate(1,1);
    p.setPen (m_borderColor.darker(120));
    QPainterPath path;
    path.addRoundedRect (rect, 6, 6);
    p.setBrush(m_backGround);
    p.drawPath(path);
}
//------------------------------------------------------ setBackGround ---------------------------------------------------------------------------------
void C_Frm_Sequence::setBackGround(QColor backGroundColor)
{   m_backGround=backGroundColor;
    m_Button_Pendant->setColor(backGroundColor);
    m_Button_DureeUnite->setColor(backGroundColor);
    m_Button_Repart->setColor(backGroundColor);
    m_Button_RepartRepas->setColor(backGroundColor);
    m_check_MenuBut->setColor(backGroundColor);
}

//======================== C_Wdg_Stack_Sequence =========================================================================================================
C_Wdg_Stack_Sequence::C_Wdg_Stack_Sequence (  QRect                   geometry_rect,            // c'est le parent qui decide de la geometrie
                                              C_Frm_PosologieHitList *pC_Frm_PosologieHitList,              // le pere
                                              const QString           &objectName,              //  = "C_Wdg_Stack_Sequence",
                                              C_Frm_Sequence         *pC_Frm_Sequence           //  = 0 le premier widget
                                           )
 : QStackedWidget(pC_Frm_PosologieHitList)
{setGeometry(geometry_rect);
 m_pC_Frm_PosologieHitList = pC_Frm_PosologieHitList;
 if (pC_Frm_Sequence) this->addWidget(pC_Frm_Sequence);
 setObjectName(objectName);
}

//------------------------------------------------------ setBackGround ---------------------------------------------------------------------------------
void C_Wdg_Stack_Sequence::setBackGround( const QColor &backGroundColor )
{for (int i=0; i<count(); ++i)
    {sequenceWidget(i)->setBackGround(backGroundColor);
    }
}

//-------------------------------- sequenceWidget -------------------------------------------------------------------------------------------------------
C_Frm_Sequence *C_Wdg_Stack_Sequence::sequenceWidget(int index)
{ if (index > count()-1) return 0;
  return   (C_Frm_Sequence*)  widget ( index );
}

//------------------------------------------------------ sequenceAdd ---------------------------------------------------------------------------------
C_Frm_Sequence * C_Wdg_Stack_Sequence::sequenceAdd( QRect rect,  C_Frm_PosologieHitList *pC_Frm_PosologieHitList )
{   int index = count();
    if (index==5)           return 0;
    C_Frm_Sequence *pC_Frm_Sequence = new C_Frm_Sequence(pC_Frm_PosologieHitList, rect);
    if (pC_Frm_Sequence==0) return 0;

    pC_Frm_Sequence->setObjectName( QString("HitList : pC_Frm_Seq_%1").arg(QString::number(index)));
    pC_Frm_Sequence->show();
    addWidget( pC_Frm_Sequence  );
    connect( pC_Frm_Sequence, SIGNAL( Sign_contentChanged (C_Frm_Sequence*)), this , SIGNAL( Sign_contentChanged(C_Frm_Sequence*) ));
    return pC_Frm_Sequence;
}
//------------------------------------------------------ sequenceRemove ---------------------------------------------------------------------------------
int C_Wdg_Stack_Sequence::sequenceRemove(int index /* = -1 */)
{   if (count()<=0) return -1;
    int curIndex = index;
    if (curIndex == -1) curIndex = currentIndex();
    C_Frm_Sequence *pC_Frm_Sequence = (C_Frm_Sequence*)widget ( curIndex );
    removeWidget( pC_Frm_Sequence );
    delete pC_Frm_Sequence;
    int lastIndex = count()-1;
    curIndex = qMin(lastIndex, curIndex);
    setCurrentIndex (curIndex);
    return curIndex;
}

//------------------------------------------------------ sequenceAllRemove ---------------------------------------------------------------------------------
void C_Wdg_Stack_Sequence::sequenceAllRemove()
{   C_Frm_Sequence *pC_Frm_Sequence = 0;
    int             nb              = count();
    //............ relever tous les widgets .....................
    QList <C_Frm_Sequence*> toDeleteList;
    for (int i=0; i<nb; ++i)
        { pC_Frm_Sequence = (C_Frm_Sequence*)widget ( i );
          toDeleteList.append(pC_Frm_Sequence);
        }
    //............ effacer tous les widgets .....................
    for (int i = 0; i < toDeleteList.size(); ++i)
       { pC_Frm_Sequence = toDeleteList.at(i);
         removeWidget( pC_Frm_Sequence );
         delete pC_Frm_Sequence;
       }
}
//---------------------------------- interfaceToGrammar -----------------------------------------------------------------------------------------------------
QString C_Wdg_Stack_Sequence::interfaceToGrammar( QStringList &formesList, QStringList &voiesList)
{QString sequencesSum = "";
 QString forme;
 QString voie;
 formesList.clear();
 voiesList.clear();
 for (int i=0; i<count(); ++i)
    { sequencesSum += sequenceWidget(i)->interfaceToGrammar(forme, voie);
      formesList << forme ;
      voiesList  << voie;
    }
 formesList.removeDuplicates();
 voiesList.removeDuplicates();
 return sequencesSum;
}
//---------------------------------- grammarToInterface -----------------------------------------------------------------------------------------------------
void C_Wdg_Stack_Sequence::grammarToInterface( const QString &grammar)
{QStringList seqList   = CGestIni::getXmlDataList( "gpp",    grammar);
 int n = qMin(seqList.size(), count());
 for (int i=0; i<n; ++i)
    { sequenceWidget(i)->sequenceToInterface(seqList[i]);
    }
}
//====================================== C_Frm_PosologieHitList ==========================================================================================
C_Frm_PosologieHitList::C_Frm_PosologieHitList (const QStringList   &grammarHitList      ,
                                                const QString       &grammar_in          ,          // la grammaire d'entree SI PRESENTE ET NON VIDE C'EST ELLE QUI SERA INSTALLE
                                                C_Frm_Posologie     *pC_Frm_Posologie    ,          // la posologie en cours
                                                const QRect         &geometry_rect       ,
                                                int                  h_button            ,
                                                const QColor        &backGroundColor     ,
                                                C_BitMapCollection  *pC_BitMapCollection ,          // il nous faut des icones
                                                C_BDM_Api           *pC_BDM_Api          ,          // un pointeur sur l'API medicamenteuse
                                                QWidget*             parent              , /* = 0 */
                                                const char*          name                , /* = 0 */
                                                QList<C_CompositionItem> *p_compositionList
                                               )
   : C_PopupDial(parent,backGroundColor,QRect(geometry_rect.x(),geometry_rect.y(),1,1),geometry_rect)  //QFrame (parent)
{  m_status                        = "";
   m_cip                           = pC_Frm_Posologie->c_frm_produit()->id();
   m_idType                        = pC_Frm_Posologie->c_frm_produit()->id_type();
   m_pC_Frm_Posologie              = pC_Frm_Posologie;
   m_cur_grammar                   = grammar_in;
   m_pC_BitMapCollection           = pC_BitMapCollection;
   m_pC_BDM_Api                    = pC_BDM_Api;
   m_pCompositionList              = p_compositionList;
   m_Grid_H                        = ITEM_HIT_H;
   m_FocusWidget                   = 0;
   m_background_color              = backGroundColor;
   m_marge                         = 2;
   m_buttonH                       = h_button;
   m_pC_PopupDial_IndicationChoice = 0;
   m_pC_PopupDial_TerrainChoice    = 0;
   int nbItem                      = grammarHitList.size();
   QMenu *pQMenu                   = 0;
   int pos_x                       = 0;
   int pos_y                       = 0;
   QRect  rect;

   setObjectName(name);
   setFocusPolicy (Qt::ClickFocus );
   //.............. le bouton de fermeture ..............................................
   m_ButtonHitClose = new CMyButton( pC_BitMapCollection->m_ButtonHitClose,this,"|HitList : m_ButtonHitClose" );
   m_ButtonHitClose->setGeometry(geometry_rect.width()-m_marge-pC_BitMapCollection->m_ButtonHitClose.width(),
                                 m_marge,
                                 pC_BitMapCollection->m_ButtonHitClose.width(),
                                 pC_BitMapCollection->m_ButtonHitClose.width());
   m_ButtonHitClose->setFlat(true);
   //................ police de caracteres...........................
   m_fGrd            = font(); m_fGrd.setPixelSize(10); m_fGrd.setBold(false); m_fGrd.setItalic(false);
   setFont(m_fGrd);
   //................... fenetre principale ....................................................................
   setGeometry(geometry_rect);
   //................... la zone des boutons des sequences ..............................................
   pos_x                = m_marge;
   pos_y                = m_marge + m_buttonH;
   rect                 = QRect(pos_x, pos_y, m_buttonH*4-5, m_buttonH);  // "D'abord" etant plus long on met  m_buttonH*4
   m_Button_Seq_0       = new CMyColorButton(backGroundColor,rect,this, tr("D'abord|HitList : m_Button_Seq_0")); m_Button_Seq_0->move(pos_x,pos_y);     pos_x += m_Button_Seq_0->width(); m_Button_Seq_0->setFont(m_fGrd);
   rect                 = QRect(pos_x, pos_y, m_buttonH*3-5, m_buttonH);
   m_Button_Seq_1       = new CMyColorButton(backGroundColor,rect,this, tr("puis|HitList : m_Button_Seq_1"));    m_Button_Seq_1->move(pos_x,pos_y);     pos_x += m_Button_Seq_1->width(); m_Button_Seq_1->setFont(m_fGrd);
   m_Button_Seq_2       = new CMyColorButton(backGroundColor,rect,this, tr("puis|HitList : m_Button_Seq_2"));    m_Button_Seq_2->move(pos_x,pos_y);     pos_x += m_Button_Seq_2->width(); m_Button_Seq_2->setFont(m_fGrd);
   m_Button_Seq_3       = new CMyColorButton(backGroundColor,rect,this, tr("puis|HitList : m_Button_Seq_3"));    m_Button_Seq_3->move(pos_x,pos_y);     pos_x += m_Button_Seq_3->width(); m_Button_Seq_3->setFont(m_fGrd);
   m_Button_Seq_4       = new CMyColorButton(backGroundColor,rect,this, tr("puis|HitList : m_Button_Seq_4"));    m_Button_Seq_4->move(pos_x,pos_y);     pos_x += m_Button_Seq_4->width(); m_Button_Seq_4->setFont(m_fGrd);
   connect( m_Button_Seq_0, SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_On_ButtonSeq_Clicked(const QString&) ));
   connect( m_Button_Seq_1, SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_On_ButtonSeq_Clicked(const QString&) ));
   connect( m_Button_Seq_2, SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_On_ButtonSeq_Clicked(const QString&) ));
   connect( m_Button_Seq_3, SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_On_ButtonSeq_Clicked(const QString&) ));
   connect( m_Button_Seq_4, SIGNAL( Sign_ButtonClicked (const QString&)  ),    this , SLOT( Slot_On_ButtonSeq_Clicked(const QString&) ));

   rect                = QRect(pos_x, pos_y, m_buttonH, m_buttonH);
   m_Button_Seq_plus   = new CMyButton(m_pC_BitMapCollection->m_ButtonSequenceAdd,rect,this, tr("|HitList : m_Button_Seq_plus"));
   m_Button_Seq_plus->setFlat(true);
   connect( m_Button_Seq_plus, SIGNAL( clicked (bool)  ),  this , SLOT( Slot_On_ButtonSequenceAdd_Clicked(bool) ));

   pos_x              += m_Button_Seq_plus->width();
   rect                = QRect(pos_x, pos_y, m_buttonH, m_buttonH);
   m_Button_Seq_del    = new CMyButton(m_pC_BitMapCollection->m_ButtonSequenceDel,rect,this, tr("|HitList : m_Button_Seq_del"));
   m_Button_Seq_del->setFlat(true);
   connect( m_Button_Seq_del, SIGNAL( clicked (bool)  ),  this , SLOT( Slot_On_ButtonSequenceDel_Clicked(bool) ));
   //................... la zone des parametres corporels ...............................................................................
   pos_x                  = HIT_W_MMASCN + m_marge;
   //rect                   = QRect(pos_x, pos_y, daySegment_w*2, m_buttonH);
   m_C_PopupDialCorpoFact = new C_PopupDialCorpoFact(this, backGroundColor,QRect(pos_x,pos_y,geometry_rect.width()-pos_x-m_marge,m_buttonH),QRect(pos_x-200,pos_y,0,0));

   //.............. le composant de la date et duree totale literalite DCI/commercial renouvelable etc. de la prescription ..............................................
   pos_x             = 94; //m_Button_Seq_del->x()+m_Button_Seq_del->width()+m_marge;
   int w_date        = m_ButtonHitClose->x()-pos_x-m_marge;
   pos_y             = 0;
   m_C_PopupDialDate = new C_PopupDialDate(this,QRect(pos_x+94. ,pos_y, w_date-94, m_buttonH + m_marge),
                                                QRect(pos_x ,pos_y, w_date,
                                                      m_buttonH+m_marge+m_marge  +   // hauteur rectangle initial
                                                      16*7                       +   // 7 rangees de pixmap de 16x16
                                                      (m_buttonH+m_marge)*5      +   // 5 titres des pixmap de 16x16
                                                       m_marge*4),                +   // une marge basse convenable
                                                this,QDateTime::currentDateTime());

   //................. la pile des sequences ..........................................................
   pos_y += m_buttonH + m_marge+m_marge+m_marge + m_buttonH;
   rect   = QRect(0, pos_y, geometry_rect.width(), m_pC_BitMapCollection->getCalculatedSequenceH(m_buttonH, m_marge));
   m_pC_Wdg_Stack_Sequence = new C_Wdg_Stack_Sequence( rect, this,"HitList : m_pC_Wdg_Stack_Sequence" );
   adjustEnableStateStackButton();

   //................... la zone de saisie de la note .............................................................
   pos_y           += m_pC_Wdg_Stack_Sequence->height()+m_marge+m_marge;
   pos_x            = m_marge;

   m_Button_Note    = new CMyColorButton(backGroundColor,this, tr("commentaire|HitList : m_Button_Note"));
   m_Button_Note->setGeometry(QRect(0,0,m_Button_Note->width(),m_buttonH)); m_Button_Note->setFont(m_fGrd);
   m_Button_Note->move(pos_x,pos_y); pos_x += m_Button_Note->width()+m_marge;
   pQMenu = new QMenu(m_Button_Note); pQMenu->setFont(m_fGrd);

   pQMenu->setObjectName("HitList : m_Button_Note menu ");
   ThemePopup::addOptionMenu(pQMenu,tr("101 - si besoin "));
   ThemePopup::addOptionMenu(pQMenu,tr("102 - en cas de douleurs "));
   ThemePopup::addOptionMenu(pQMenu,tr("103 - \303\240 dissoudre dans un verre d'eau "));
   ThemePopup::addOptionMenu(pQMenu,tr("104 - \303\240 adapter selon I.N.R. "));
   ThemePopup::addOptionMenu(pQMenu,tr("105 - absorber avec un grand verre d'eau "));
   QStringList additionnalOptions = m_pC_BitMapCollection->getFileList("add_PopupComment.txt");
   for (int i=0; i<additionnalOptions.size(); ++i)
       {ThemePopup::addOptionMenu(pQMenu,additionnalOptions.at(i));
       }
   m_Button_Note->setMenu ( pQMenu );
   connect( pQMenu, SIGNAL( triggered ( QAction *  )  ),    this , SLOT( Slot_HitListMenuTriggered( QAction *  ) ));

   rect             = QRect(pos_x,pos_y,this->width()-pos_x-m_marge-m_marge,m_buttonH);
   m_LineEditNote   = new CMyQLineEdit(rect,"",this,"m_LineEditNote",this);
   m_LineEditNote->setObjectName("HitList : m_LineEditNote");
   m_LineEditNote->setAlignment ( Qt::AlignLeft );
   m_LineEditNote->setToolTip(tr("Saisissez ici un commentaire de votre choix."));

   //................... la zone de saisie de l'indication .............................................................
   pos_y            = m_LineEditNote->y() + m_LineEditNote->height();
   pos_x            = m_marge;

   m_Button_Indic   = new CMyColorButton(backGroundColor,this, tr("indication|HitList : m_Button_Indic"));
   m_Button_Indic->setGeometry(QRect(0,0,m_Button_Indic->width(),m_buttonH)); m_Button_Indic->setFont(m_fGrd);
   m_Button_Indic->move(pos_x,pos_y); pos_x += m_Button_Indic->width() + m_marge;
   m_Button_Indic->setToolTip(tr("cliquez sur ce bouton pour modifier l'indication li\303\251e \303\240 cette prescription."));

   m_Button_IndicType   = new CMyColorButton(backGroundColor,this, tr("indication|HitList : m_Button_IndicType"));
   m_Button_IndicType->setGeometry(QRect(0,0,16,m_buttonH)); m_Button_IndicType->setFont(m_fGrd);
   m_Button_IndicType->move(pos_x,pos_y); pos_x += m_Button_IndicType->width();

   rect             = QRect(pos_x,pos_y,this->width()-pos_x-m_marge-m_marge-60,m_buttonH);

   m_LineEditIndic   = new CMyQLineEdit(rect,"",this,"m_LineEditIndic",this);
   m_LineEditIndic->setObjectName("HitList : m_LineEditIndic");
   m_LineEditIndic->setAlignment ( Qt::AlignLeft );
   m_LineEditIndic->setReadOnly ( true );
   m_LineEditIndic->setToolTip(tr("Pour modifier l'indication li\303\251e \303\240 cette prescription, veuillez cliquer sur le bouton 'indication'"));

   pos_x += m_LineEditIndic->width();
   rect             = QRect(pos_x,pos_y,this->width()-pos_x-m_marge-m_marge,m_buttonH);
   m_LineEdit_IndicRefund   = new CMyQLineEdit(rect,"",this,"m_LineEdit_IndicRefund",this);
   m_LineEdit_IndicRefund->setObjectName("HitList : m_LineEdit_IndicRefund");
   m_LineEdit_IndicRefund->setAlignment ( Qt::AlignLeft );
   m_LineEdit_IndicRefund->setReadOnly ( true );
   m_LineEdit_IndicRefund->setToolTip(tr("Affiche si il existe, le tarif remboursable li\303\251e \303\240 l'indication"));

   connect( m_Button_Indic, SIGNAL( clicked (bool)  ),  this , SLOT( Slot_ButtonIndic_Clicked(bool) ));
   //................... la zone de saisie du terrain .............................................................
   pos_y            = m_LineEditIndic->y() + m_LineEditIndic->height();
   pos_x            = m_marge;

   m_Button_Terrain   = new CMyColorButton(backGroundColor,this, tr("Terrain|HitList : m_Button_Terrain"));
   m_Button_Terrain->setGeometry(QRect(0,0,m_Button_Terrain->width(),m_buttonH)); m_Button_Terrain->setFont(m_fGrd);
   m_Button_Terrain->move(pos_x,pos_y); pos_x += m_Button_Terrain->width()+m_marge;
   m_Button_Terrain->setToolTip(tr(" cliquez sur ce bouton pour modifier le terrain associ\303\251e \303\240 cette prescription"));

   rect             = QRect(pos_x,pos_y,this->width()-pos_x-m_marge-m_marge,m_buttonH);

   m_LineEditTerrain   = new CMyQLineEdit(rect,"",this,"m_LineEditTerrain",this);
   m_LineEditTerrain->setObjectName("HitList : m_LineEditTerrain");
   m_LineEditTerrain->setAlignment ( Qt::AlignLeft );

   connect( m_Button_Terrain, SIGNAL( clicked (bool)  ),  this , SLOT( Slot_Button_Button_Terrain_Clicked(bool) ));

   //................... les boutons ajouter effacer favoris ...................................................
   pos_y     = m_Button_Terrain->y() + m_Button_Terrain->height() + m_marge;
   int but_w = (geometry_rect.width()-m_marge*2)/2 ;
   int but_x = m_marge;
   m_Button_Add_Fav = new CMyColorButton(backGroundColor,this,tr("Ajouter aux favoris|HitList : m_Button_Add"));
   m_Button_Add_Fav->setGeometry(but_x,pos_y,but_w,h_button);            but_x += but_w;  m_Button_Add_Fav->setFont(m_fGrd);
   m_Button_Add_Fav->setToolTip(tr("Ajouter la prescription actuelle dans la liste des favoris"));

   m_Button_Del_Fav = new CMyColorButton(backGroundColor,this,tr("Retirer un favori|HitList : m_Button_Del"));
   m_Button_Del_Fav->setGeometry(but_x,pos_y,but_w,h_button);            but_x += but_w;  m_Button_Del_Fav->setFont(m_fGrd);
   m_Button_Del_Fav->setToolTip(tr("Effacer la prescription actuellement s\303\251lectionn\303\251e dans la liste des favoris"));

   connect( m_ButtonHitClose, SIGNAL( clicked (bool)  ),  this , SLOT( Slot_ButtonHitClose_Clicked(bool) ));
   connect( m_Button_Add_Fav, SIGNAL( clicked (bool)  ),  this , SLOT( Slot_Button_Add_Fav_Clicked(bool) ));
   connect( m_Button_Del_Fav, SIGNAL( clicked (bool)  ),  this , SLOT( Slot_Button_Del_Fav_Clicked(bool) ));
   //................... la liste des favoris ...............................
   pos_y                += m_buttonH + m_marge;
   m_pQListWidgetFavoris = new QListWidget(this);
   //QScrollBar * scrollBar = new QScrollBar(m_pQListWidgetFavoris);
   //m_pQListWidgetFavoris->setHorizontalScrollBar (scrollBar);
   //scrollBar->setGeometry(1,1,1,1);
   m_pQListWidgetFavoris->setGeometry( QRect(m_marge,
                                             pos_y,                                // ofset Y apres la zone de boutons
                                             geometry_rect.width()          - m_marge * 2,
                                             geometry_rect.height() - pos_y - m_marge * 2 ));    // hauteur - cet ofset Y de la zone de bouton
   for (int i=0; i<nbItem; ++i)
       { addFavoriWidgetItem(grammarHitList[i], i);
       }
   //............................ installer la grammaire .................................................
   QString unitPriseForm   = m_pC_BDM_Api->getPriseUnitFormFromCIP(m_cip).remove("(s)");    // celle de l'API surcharge celle de la grammaire
   //QString voie            = m_pC_BDM_Api->getDrugRouteFromCIP ( m_cip );    // celle de l'API surcharge celle de la grammaire
   QString quPA_by_UP      = "";
   QString unitPA          = "";
   if (m_pCompositionList->size())
      { unitPA     = m_pCompositionList->at(0).unit();
        quPA_by_UP = m_pCompositionList->at(0).amount();
      }
   if (nbItem)
      { m_pQListWidgetFavoris->setCurrentRow(0);
        m_FocusWidget = setWidgetEditFocus((CMyQTextEdit*) m_pQListWidgetFavoris->itemWidget(m_pQListWidgetFavoris->item(0)),  m_FocusWidget);       // se positionner sur le premier de la liste
        if ( m_cur_grammar.length()==0 )                // installer la grammaire que si elle n'est pas deja installee
           { m_cur_grammar = grammarHitList[0];
             if (unitPriseForm.length()) C_PosologieGrammar::setUnitPriseForm(m_cur_grammar, unitPriseForm);
             CGestIni::setXmlData("gph_punit" ,   unitPA,      m_cur_grammar);
             if ( CGestIni::getXmlData("gph_pqbyuf" , m_cur_grammar).length()==0 )
                { CGestIni::setXmlData("gph_pqbyuf" , quPA_by_UP,                        m_cur_grammar);
                }
             adjustFavoriToGrammar(m_cur_grammar);     // met a jour m_cur_grammar
           }
      }
   else if ( m_cur_grammar.length()==0 )                                                   // si non definie en entree en mettre une par defaut
      { m_cur_grammar        = C_PosologieGrammar::empty_skeleton_grammar();
        QString dci_list_xml = compositionToXml( m_pCompositionList );
        CGestIni::setXmlData("gph_na" ,     c_frm_produit()->commercialName(),        m_cur_grammar);
        CGestIni::setXmlData("gph_dcl",     dci_list_xml,                             m_cur_grammar, 1);
        CGestIni::setXmlData("gph_punit" ,  unitPA,                                   m_cur_grammar);
        CGestIni::setXmlData("gph_voie" ,   m_pC_BDM_Api->getDrugRouteFromCIP(m_cip), m_cur_grammar);
        if ( CGestIni::getXmlData("gph_pqbyuf",  m_cur_grammar).length()==0 )
           { CGestIni::setXmlData("gph_pqbyuf" , quPA_by_UP,                    m_cur_grammar);
           }
        if (unitPriseForm.length()) C_PosologieGrammar::setUnitPriseForm(m_cur_grammar, unitPriseForm);
      }

   grammarToInterface(m_cur_grammar);
   //........................... connexions a faire apres grammarToInterface() .....................................
   //                            pour eviter les reentrees
   //connect( m_pQListWidgetFavoris ,  SIGNAL( itemClicked ( QListWidgetItem *  )  ),                          this , SLOT( Slot_QListWidgetFavoris_itemClicked(QListWidgetItem * ) )); // marche po
   connect( m_pC_Wdg_Stack_Sequence, SIGNAL( Sign_contentChanged(C_Frm_Sequence*)  ),                        this , SLOT  ( Slot_SequenceChanged(C_Frm_Sequence*) ));
   connect( m_LineEditIndic,         SIGNAL( Sign_textChanged (const QString &, CMyQLineEdit *, void *)  ),  this , SLOT  ( Slot_textChanged(const QString &, CMyQLineEdit *, void *) ));
   connect( m_LineEditNote,          SIGNAL( Sign_textChanged (const QString &, CMyQLineEdit *, void *)  ),  this , SLOT  ( Slot_textChanged(const QString &, CMyQLineEdit *, void *) ));
   connect( m_C_PopupDialDate,       SIGNAL( Sign_contentChanged(C_PopupDialDate*)  ),                       this , SLOT  ( Slot_PopupDialDateChanged(C_PopupDialDate*) ));
   connect( m_C_PopupDialCorpoFact,  SIGNAL( Sign_contentChanged(C_PopupDialCorpoFact*)  ),                  this , SLOT  ( Slot_PopupDialCorpoFactChanged(C_PopupDialCorpoFact*) ));
   connect( m_C_PopupDialCorpoFact,  SIGNAL( Sign_FactCorpoChanged( C_PatientCtx *)),                        this , SIGNAL( Sign_FactCorpoChanged( C_PatientCtx *) ));
   connect( this,                    SIGNAL( Sign_PatientCtxChanged(C_PatientCtx *)),      m_C_PopupDialCorpoFact , SLOT( Slot_PatientCtxChanged(C_PatientCtx *) ));
   //........................... on ouvre le magasin................................................................
   setToDeployRect();
}
//------------------------------ set_ald ---------------------------------------------------------------------------------------------------------
void   C_Frm_PosologieHitList::set_ald (const QString &ald)
{C_PosologieGrammar::setAld(m_cur_grammar, ald);
}
//------------------------------ Slot_HitListMenuTriggered ---------------------------------------------------------------------------------------------------------
void C_Frm_PosologieHitList::Slot_HitListMenuTriggered( QAction *pQAction )
{   QString idMenu      = pQAction->data().toString();
    QString idOptionSub = "";
    int pos = idMenu.indexOf('.');     //203.25           // menu prineal et separe par un point option sous menu
    if (pos != -1)
       {idOptionSub = idMenu.mid(pos+1);
        idMenu      = idMenu.left(pos);
       }
    int id = idMenu.toInt()/100;
    switch(id)
      {
       case 1:m_LineEditNote->setText(pQAction->text()); break;
       case 2:
           {indicationChoice(m_LineEditIndic);
           }
           break;    // options pour les indications
      }
}
//------------------------------ Slot_ButtonIndic_Clicked ---------------------------------------------------------------------------------------------------------
void C_Frm_PosologieHitList::Slot_ButtonIndic_Clicked(bool)
{ indicationChoice(m_LineEditIndic);
}
//------------------------------ Slot_Button_Button_Terrain_Clicked ---------------------------------------------------------------------------------------------------------
void C_Frm_PosologieHitList::Slot_Button_Button_Terrain_Clicked(bool)
{ terrainChoice(m_LineEditTerrain);
}
//------------------------------ terrainChoice ---------------------------------------------------------------------------------------------------------
void C_Frm_PosologieHitList::terrainChoice(CMyQLineEdit *pCMyQLineEdit)
{
 if (m_pC_PopupDial_TerrainChoice==0)
    {QRect initialRect = QRect(m_Button_Terrain->x()+m_Button_Terrain->width(),m_Button_Terrain->y(),1,1);    // m_Button_Indic->x()+m_Button_Indic->width()
     QRect deployRect  = QRect(m_Button_Terrain->x()+m_Button_Terrain->width(),m_buttonH*2,250, m_pC_BitMapCollection->getCalculatedTabZone_H(m_buttonH,m_marge) + m_pC_BitMapCollection->getCalculatedSequenceH(m_buttonH, m_marge)+m_marge*2);
     m_pC_PopupDial_TerrainChoice = new    C_PopupDial_TerrainChoice(this, pCMyQLineEdit, this->background(),initialRect, deployRect);
     m_pC_PopupDial_TerrainChoice->show();
    }
 m_pC_PopupDial_TerrainChoice->setToDeployRect ();

}
//------------------------------ indicationChoice ---------------------------------------------------------------------------------------------------------
void C_Frm_PosologieHitList::indicationChoice(CMyQLineEdit *pCMyQLineEdit)
{if (m_pC_PopupDial_IndicationChoice==0)
    {QRect initialRect = QRect(m_Button_Indic->x()+m_Button_Indic->width(),m_Button_Indic->y(),1,1);    // m_Button_Indic->x()+m_Button_Indic->width()
     QRect deployRect  = QRect(m_Button_Indic->x()+m_Button_Indic->width(),0,300, m_pC_BitMapCollection->getCalculatedTabZone_H(m_buttonH,m_marge) + m_pC_BitMapCollection->getCalculatedSequenceH(m_buttonH, m_marge) + m_buttonH+m_marge*3);
     m_pC_PopupDial_IndicationChoice = new    C_PopupDial_IndicationChoice(this, pCMyQLineEdit, this->background(),initialRect, deployRect);
     m_pC_PopupDial_IndicationChoice->show();
    }
 m_pC_PopupDial_IndicationChoice->setToDeployRect ();
}

//------------------------------------------------------ addFavoriWidgetItem ---------------------------------------------------------------------------------
CMyQTextEdit *C_Frm_PosologieHitList::addFavoriWidgetItem(const QString &grammar, int index)
{//........... on rajoute le nouveau .................
    QListWidgetItem *wdg_item    = new QListWidgetItem(m_pQListWidgetFavoris);
    wdg_item->setSizeHint( QSize (m_pQListWidgetFavoris->width()-15 ,m_Grid_H));

    CMyQTextEdit    *pQTextEdit  = new CMyQTextEdit(computeTextHit( grammar, index ),
                                                    m_pQListWidgetFavoris,
                                                    QString("HitList : pQTextEditItem %1").arg(QString::number(index)),
                                                    wdg_item);
    pQTextEdit->setCustomString(grammar);
    pQTextEdit->setReadOnly ( true );
    pQTextEdit->setGeometry( 0, 0, m_pQListWidgetFavoris->width(), m_Grid_H );
    setItemListBackGround(pQTextEdit, false);
    pQTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pQTextEdit->document()->setDocumentMargin(0.0);
    m_pQListWidgetFavoris->addItem(wdg_item);
    m_pQListWidgetFavoris->setItemWidget(wdg_item, pQTextEdit);
    connect( pQTextEdit, SIGNAL( Sign_mousePressEvent    ( QMouseEvent *, void *)  ),   this , SLOT( Slot_HitItem_mousePressEvent   ( QMouseEvent *, void *) ));
    connect( pQTextEdit, SIGNAL( Sign_mouseDoubleClicked ( QMouseEvent *, void *)  ),   this , SLOT( Slot_HitItem_mouseDoubleClicked( QMouseEvent *, void *) ));
    return pQTextEdit;
}

//--------------------------------- clearHitList ------------------------------------------------------------------------------------------------------
void C_Frm_PosologieHitList::clearHitList()
{   m_pQListWidgetFavoris->clear();
    m_FocusWidget = 0;
}

//--------------------------------- loadHitList ------------------------------------------------------------------------------------------------------
int C_Frm_PosologieHitList::loadHitList(int *nbFromHit /* = 0 */)
{   clearHitList();
    //................ la liste des favoris custom ............................................
    QStringList grammarList   = m_pC_BDM_Api->selectPosologieHitList(m_cip, m_idType);
    if (nbFromHit) *nbFromHit = grammarList.size();
    //................ rajouter la liste des favoris de la base de donnees ....................
    QString tmpGrammar        =  C_PosologieGrammar::empty_skeleton_grammar();
    QString quPA_by_UP        = "";
    QString unitPA            = "";
    if (m_pCompositionList->size())
       { unitPA     = m_pCompositionList->at(0).unit();
         quPA_by_UP = m_pCompositionList->at(0).amount();
       }
    QString dci_list_xml = C_Frm_PosologieHitList::compositionToXml(*m_pCompositionList, "CIP");  //compositionToXml( &m_compositionList );
    CGestIni::setXmlData("gph_na" ,     c_frm_produit()->commercialName(),  tmpGrammar);
    CGestIni::setXmlData("gph_id",      m_cip,                              tmpGrammar, 1);
    CGestIni::setXmlData("gph_it",      m_idType,                           tmpGrammar, 1);
    CGestIni::setXmlData("gph_dcl",     dci_list_xml,                       tmpGrammar, 1);
    CGestIni::setXmlData("gph_punit" ,  unitPA,                             tmpGrammar);
    CGestIni::setXmlData("gph_pqbyuf" , quPA_by_UP,                         tmpGrammar);
    grammarList.append( m_pC_BDM_Api->getPosologiesFromCIP (m_pC_BDM_Api->patientContexte(), tmpGrammar) );

    //................... le contenu de la surface qui doit scroller .............................................
    int              nbItem = grammarList.size();
    for (int i=0; i<nbItem; ++i)
        { addFavoriWidgetItem(grammarList[i], i);
        }
    //............................ installer la grammaire .................................................
    QString unitPriseForm = m_pC_BDM_Api->getPriseUnitFormFromCIP(m_cip);
    if (nbItem)
       { m_cur_grammar = grammarList[0];                                                                   // l'installer comme celle en cours
       }
    else
       { m_cur_grammar = C_PosologieGrammar::empty_skeleton_grammar();
       }
    C_PosologieGrammar::setUnitPriseForm(m_cur_grammar, unitPriseForm);
    grammarToInterface(m_cur_grammar);
    return nbItem;
}

//--------------------------------- Slot_PopupDialCorpoFactChanged ------------------------------------------------------------------------------------------------------
void C_Frm_PosologieHitList::Slot_PopupDialCorpoFactChanged(C_PopupDialCorpoFact* pC_PopupDialCorpoFact)
{       if (pC_PopupDialCorpoFact==0) return;
        double poids   = 0.0;
        double surface = 0.0;
        pC_PopupDialCorpoFact->interfaceToGrammar( m_cur_grammar );                                          // placer les infos des facteurs corporels dans la grammaire
        QStringList formesList , voiesList;
        QString sequencesListXML = m_pC_Wdg_Stack_Sequence->interfaceToGrammar( formesList , voiesList) ;    // recuperer la liste des sequences XML a adapter au poids/surface
        C_PatientCtx *pat_ctxt   = pC_PopupDialCorpoFact->patientContexte();
        if (pat_ctxt)
           { poids   = pat_ctxt->poidsToDouble();
             surface = pat_ctxt->surfaceToDouble();
           }
        //..................... si facteur corporel :ajuster les sequences XML selon le poids surface etc......................
        /*QString posoTotale = */C_PosologieGrammar::adjustGrammarSequencesToCorporalFactor( m_cur_grammar, poids, surface, sequencesListXML, C_PosologieGrammar::ADJUST_SEQUENCES);
        m_pC_Wdg_Stack_Sequence->grammarToInterface( sequencesListXML );           // mettre a jour la liste des sequences
        CGestIni::setXmlData("gps" , sequencesListXML, m_cur_grammar, 1);          // replacer la liste des sequences XML modifiee dans les widgets des sequences
        //...................... on affiche ...................................................................................
        emit Sign_contentChanged(this);                                            // on affiche
}
//--------------------------------- Slot_PopupDialDateChanged ------------------------------------------------------------------------------------------------------
void C_Frm_PosologieHitList::Slot_PopupDialDateChanged(C_PopupDialDate*)
{ emit Sign_contentChanged(this);
}

//--------------------------------- Slot_SequenceChanged ------------------------------------------------------------------------------------------------------
void C_Frm_PosologieHitList::Slot_SequenceChanged(C_Frm_Sequence*)
{ emit Sign_contentChanged(this);
}
//--------------------------------- Slot_textChanged ------------------------------------------------------------------------------------------------------
void C_Frm_PosologieHitList::Slot_textChanged(const QString &, CMyQLineEdit *, void *)
{ emit Sign_contentChanged(this);
}

//--------------------------------- moveY ------------------------------------------------------------------------------------------------------
void C_Frm_PosologieHitList::moveY(int dy)
{translateGeometry (0, dy);
}

//------------------------ grammarToInterface ---------------------------------------
void C_Frm_PosologieHitList::grammarToInterface(QString grammar)
{  // int n = 0;
   disconnect(this, SIGNAL(Sign_contentChanged(C_Frm_PosologieHitList *)), 0, 0);
   clearInterface();
   if (grammar.length()==0) return;
   //CGestIni::Param_UpdateToDisk("/Users/rolandsevin/Documents/grammar-in.xml",grammar);
   double poids           = 0.0;
   double surface         = 0.0;
   C_PatientCtx *pat_ctxt = c_bdm_api()->patientContexte();
   if (pat_ctxt)
      { poids   = pat_ctxt->poidsToDouble();
        surface = pat_ctxt->surfaceToDouble();
      }
   int  nextPos                      = 0;
   m_status                          = CGestIni::getXmlData("status",      grammar, &nextPos);
   QString dateTime                  = CGestIni::getXmlData("gph_dt",      grammar, &nextPos);
   QString cycle                     = CGestIni::getXmlData("gph_cy",      grammar, &nextPos);
   QString substituable              = CGestIni::getXmlData("gph_sb",      grammar, &nextPos);
   QString dciMode                   = CGestIni::getXmlData("gph_dci",     grammar, &nextPos);
   // QString up_forme                  = CGestIni::getXmlData("gph_uf",      grammar, &nextPos);
   // QString qPA_min_By_FactCorp       = CGestIni::getXmlData("gph_pmin",    grammar, &nextPos);
   // QString qPA_max_By_FactCorp       = CGestIni::getXmlData("gph_pmax",    grammar, &nextPos);
   // QString unitePoso                 = CGestIni::getXmlData("gph_punit",   grammar, &nextPos);  // mg
   // QString facteurCorpo              = CGestIni::getXmlData("gph_pfc",     grammar, &nextPos);  // 10
   // QString unite_facteurCorpo        = CGestIni::getXmlData("gph_pfcunit", grammar, &nextPos);  // kg m2
   // QString qPA_ByUnitPrise           = CGestIni::getXmlData("gph_pqbyuf",  grammar, &nextPos);  // 500 mg (par comprime)
   QString literalite_N              = CGestIni::getXmlData("gph_nm",      grammar, &nextPos);
   QString terrainStr                = CGestIni::getXmlData("gph_tr",      grammar, &nextPos);
   QString indic_str                 = CGestIni::getXmlData("gph_in",      grammar, &nextPos);
   QString comment                   = CGestIni::getXmlData("gph_co",      grammar, &nextPos);
   QString sequencesListXML          = CGestIni::getXmlData("gps",         grammar, &nextPos);

   //..................... si facteur corporel :ajuster la poso totale au poids/surface................................................
   //                      les sequences XML ne seront pas modifiees ( C_PosologieGrammar::JUST_ADJUST_TOTAL_AMOUNT )
   QString posoTotale      = C_PosologieGrammar::adjustGrammarSequencesToCorporalFactor( grammar, poids, surface, sequencesListXML, C_PosologieGrammar::JUST_ADJUST_TOTAL_AMOUNT);

   //..................... fabriquer la haine du terrain avec la structure du terrain................................................
   QString terrain = C_PosologieGrammar::get_terrainReadyDisplayString(terrainStr);
   m_LineEditTerrain->setText(terrain);
   m_LineEditTerrain->setFreeData(terrainStr);  // noter les donnees XML du terrain

   m_C_PopupDialCorpoFact->grammarToInterface( grammar );

   m_LineEditNote->setText(comment);

   m_C_PopupDialDate->setEditDateTime(QDateTime::fromString(dateTime,"dd-MM-yyy hh:ss"));
   if (cycle.length()==0) cycle = "J"+QString::number(m_pC_BDM_Api->defaultDurationInSeconds()/86400);
   m_C_PopupDialDate->cycleToInterface(cycle);
   m_C_PopupDialDate->literaliteToInterface(literalite_N.toInt());
   m_C_PopupDialDate->setSubstituable((substituable.length()==0)?1:substituable.toInt());       // si vide on positionne la valeur par defaut a 1 (est substituable)
   if (m_pC_Frm_Posologie->c_frm_produit()->id_type() != "CIP")  // produits custom ou accessoires datasemp
       m_C_PopupDialDate->dciToInterface((dciMode.length()==0)?"0":dciMode);                    // si vide on positionne la valeur par defaut a 0 (est en nom commercial car seul dispo dans ces cas)
   else
       m_C_PopupDialDate->dciToInterface((dciMode.length()==0)?"2":dciMode);                    // si vide on positionne la valeur par defaut a 3 (est en dci)
   //................... interface indication .................................
   QStringList indicList   = C_PosologieGrammar::get_indicationsList(indic_str);
   indication_stringToInterface(indicList.size()?indicList[0]:"");

   //............... les widgets des sequences .........................
   QStringList seqList  = CGestIni::getXmlDataList("gpp", sequencesListXML);
   if (seqList.count()==0)                       // si liste vide on ajoute une sequence vide pour demarrer
      { Slot_On_ButtonSequenceAdd_Clicked(true);
        C_Frm_Sequence *pC_Frm_Sequence = m_pC_Wdg_Stack_Sequence->sequenceWidget(0);
        if (pC_Frm_Sequence && pC_Frm_Sequence->doseTotale().length()==0) pC_Frm_Sequence->set_doseTotale(posoTotale);
      }
   else
      {
        for (int i=0; i<seqList.count(); ++i)
            {  C_Frm_Sequence *pC_Frm_Sequence = sequenceAdd();
               pC_Frm_Sequence->sequenceToInterface(seqList[i]);
               if (posoTotale.length() && pC_Frm_Sequence->doseTotale().length()==0) pC_Frm_Sequence->set_doseTotale(posoTotale);
            }
      }
   connect( this, SIGNAL( Sign_contentChanged(C_Frm_PosologieHitList*) ),   m_pC_Frm_Posologie , SLOT( Slot_HitList_contentChanged(C_Frm_PosologieHitList*) ));
   emit Sign_contentChanged(this);
}
//------------------------------- indication_stringToInterface -----------------------------------------------------------------
void  C_Frm_PosologieHitList::indication_stringToInterface(const QString &indic_str)
{   QString indic_refund    = "";
    QString indic_code      = "";
    QString indic_type      = "";
    QString indic_libelle   = "";
    QString indic_icone     = "medicatux/indic_free.png";

    if ( indic_str.length() )
       { int nb_sep   = indic_str.count('|');
         if ( nb_sep >= 2 )
            {   QStringList indic_parts         = indic_str.split('|');
                indic_refund                    = "";
                indic_code                      = indic_parts[0];
                indic_type                      = indic_parts[1];
                indic_libelle                   = indic_parts[2];
                if ( nb_sep >= 3 ) indic_refund = indic_parts[3];
                m_LineEditIndic->setText(indic_libelle);
                if ( indic_type.length()==0 ) indic_type="F"; // texte libre (toujours qq chose sinon indic_type.toLatin1()[0] plabte)
                char c = indic_type.toLatin1()[0];
                switch(c)
                { case 'S' : indic_icone     = "medicatux/indic_has.png";       break;
                  case 'A' : indic_icone     = "medicatux/indic_has.png";       break;
                  case 'D' : indic_icone     = "medicatux/indic_vidal.png";     break;
                  case 'C' : indic_icone     = "medicatux/indic_cim10.png";     break;
                  case 'T' : indic_icone     = "medicatux/indic_theriaque.png"; break;
                  default  : indic_icone     = "medicatux/indic_free.png";      break;
                }
            }
         else
            { indic_libelle = indic_str;
              indic_icone     = "medicatux/indic_free.png";
            }
       }
    m_Button_IndicType->setIcon(Theme::getIcon(indic_icone));
    m_Button_Indic->set_propr_1(indic_code);
    m_Button_Indic->set_propr_2(indic_type);
    if (indic_libelle==tr("Texte libre \303\240 modifier...."))    m_LineEditIndic->setText( "" );
    else                                                           m_LineEditIndic->setText( indic_libelle );
    m_LineEdit_IndicRefund->setText(indic_refund);
    m_Button_IndicType->setText("");
}
//------------------------------- interfaceToIndication_string -----------------------------------------------------------------
QString  C_Frm_PosologieHitList::interfaceToIndication_string()
{ //               code                           type code                    text                        remboursable (prix)
  QString str = m_Button_Indic->prop_1()+"|"+ m_Button_Indic->prop_2()+"|"+m_LineEditIndic->text()+"|"+m_LineEdit_IndicRefund->text();
  return str;
}

//------------------------------- setDuration -----------------------------------------------------------------
void  C_Frm_PosologieHitList::setDuration( const QString &duration_syntax )
{   if (m_C_PopupDialDate==0) return;
    m_C_PopupDialDate->cycleToInterface(duration_syntax);
}
//------------------------------- setDCI_Mode -----------------------------------------------------------------
//    enum dci_flag  {COM  = 0, COM_DCI = 1, DCI_COM = 2, DCI = 3 };
void C_Frm_PosologieHitList::setDCI_Mode(int mode)
{   if (m_C_PopupDialDate==0) return;
    m_C_PopupDialDate->Slot_dci_button_clicked(QString::number(mode));
}

//-------------------------------- adjustFavoriToGrammar -------------------------------------------------------------------------------------------------------
/*! \brief when a favorite posology is choosed, it is necessary to adapt the weight/surface dependant doses
 *  with patient context data, and modify grammar with modified values.
 * \note this method change inplicit m_cur_grammar member
 *  \param grammar  input grammar to adapt t corporeal factor (weight/surface)
 *  \return modified grammar
 */
QString C_Frm_PosologieHitList::adjustFavoriToGrammar(const QString &grammar)
{   double poids           = 0.0;
    double surface         = 0.0;

    C_PatientCtx *pat_ctxt = c_bdm_api()->patientContexte();
    if (pat_ctxt)
      { poids   = pat_ctxt->poidsToDouble();
        surface = pat_ctxt->surfaceToDouble();
      }
    QString sequencesListXML  = CGestIni::getXmlData("gps", grammar);     // recuperer la liste des sequences XML a adapter au poids/surface
    //..................... si facteur corporel :ajuster les sequences XML selon le poids surface etc......................
    C_PosologieGrammar::adjustGrammarSequencesToCorporalFactor( grammar, poids, surface, sequencesListXML, C_PosologieGrammar::ADJUST_SEQUENCES);
    m_cur_grammar = grammar;
    CGestIni::setXmlData("gps" , sequencesListXML, m_cur_grammar, 1);    // replacer la liste des sequences XML modifiee
    return m_cur_grammar;
}
//-------------------------------- compositionToXml -------------------------------------------------------------------------------------------------------
QString C_Frm_PosologieHitList::compositionToXml(QList<C_CompositionItem> *pCompositionList)
{  QString dci_list_xml    = "" ;
   if (c_frm_produit()->id_type()=="CIP" && pCompositionList->size())
      { for (int i = 0; i < pCompositionList->size(); ++i)
            { C_CompositionItem   compositionItem  =  m_pCompositionList->at(i);
              dci_list_xml  += "  <gph_dc>\n"
                               "    <gph_dcn>"  + compositionItem.name()   +"</gph_dcn>  <gph_dcp>"  + compositionItem.amount() +"</gph_dcp>  <gph_dcu>"  + compositionItem.unit()   +"</gph_dcu>\n"
                               "  </gph_dc>\n";
            }
      }
   else
      { return     "  <gph_dc>\n"
                   "  </gph_dc>\n";
      }
   return dci_list_xml;
}
//-------------------------------- compositionToXml [static]-------------------------------------------------------------------------------------------------------
QString C_Frm_PosologieHitList::compositionToXml(const QList<C_CompositionItem> &compositionList, const QString &id_type)
{  QString dci_list_xml    = "" ;
   if ( id_type == "CIP" && compositionList.size())
      { for (int i = 0; i < compositionList.size(); ++i)
            { C_CompositionItem   compositionItem  =  compositionList.at(i);
              dci_list_xml  += "  <gph_dc>\n"
                               "    <gph_dcn>"  + compositionItem.name()   +"</gph_dcn>  <gph_dcp>"  + compositionItem.amount() +"</gph_dcp>  <gph_dcu>"  + compositionItem.unit()   +"</gph_dcu>\n"
                               "  </gph_dc>\n";
            }
      }
   else
      { return     "  <gph_dc>\n"
                   "  </gph_dc>\n";
      }
   return dci_list_xml;
}
//-------------------------------- interfaceToGrammar -------------------------------------------------------------------------------------------------------
/*! \brief return human readable string from posologie grammar.
 *  \param const QString &grammar_expression     xml grammar input string as
 *      <gph>
 *         <gph_id>301546</gph_id>
 *         <gph_it>e</gph_it>
 *         <gph_pk>123</gph_pk>
 *         <gph_uf>comprim\303\251</gph_uf>
 *         <gph_nm>5</gph_nm>                        // conversion mode en mode numerique UP_QUANTITE MOMENT_MEAL ALL_LITERAL (on se sert de celui-ci car plus rapide a interpreter)
 *         <gph_cm>UP_QUANTITE DURATION</gph_cm>     // traduction textuelle de <gph_nm>5</gph_nm> information lisible de celle numerique  UP_QUANTITE DURATION on a pas peur des redondances ! (vive la memoire vive et les HD)
 *         <gph_in>
 *             <gph_il>I13.1|CIM|insuffisance r\303\251nale et cardiopathie hypertensive</gph_il>
 *             <gph_il>P96.0|CIM|insuffisance r\303\251nale cong\303\251nitale</gph_il>
 *         </gph_in>
 *         <gph_co>\303\240 prendre avec un grand verre d'eau sans respirer pendant 30 minutes</gph_co>
 *      </gph>
 *      <gps>
 *         <gpp>[1;m|2;d|4;s]J8</gpp>
 *         <gpp>[0.5;m|1;d15r|2;sr30|3;h10-13]J4\</gpp>
 *         <gpp>[0.5;m|1;s]J3</gpp>
 *      </gps>
 *  \param int must_be_literal  undefined if -1 (the grammar string try to be used for this and if not possible it's defined to C_PosologieGrammar::NOT_LITERAL)
 *         otherwise it's must be a combination of : C_PosologieGrammar::UP_QUANTITE
 *                                                   C_PosologieGrammar::UP_JUST_FRACTION   //  conversion uniquement de 0.25 en un quart 0.5 en un demi 0.75 en trois quart
 *                                                   C_PosologieGrammar::DURATION
 *                                                   C_PosologieGrammar::SEQUENCE_HOUR
 *                                                   C_PosologieGrammar::MOMENT_MEAL

 *         this parameter define how numerics values must be converted in literal string.
 *  \param QString &_up_forme      if == "" the grammar string try to be used to define, this parameter is the drug unit form.
 *  \return a french human readable string (for other langs the source code is there after ...)
                "<gph>\n"                          // -- DEBUT du header d'une ligne prescriptive ---
                " <atc></atc>\n"                   //        code ATC du produit
                " <stup></stup>\n"                 //        1/si supefiant  0/sinon
                " <rfid></rfid>\n"                 //        1/si rembourse pour cette indication zero sinon
                " <price></price>\n"               //        cout total hors remboursement de la prescription ( prix udv * nb_udv )
                " <total_price></total_price>\n"                 //        cout total rembourse de la prescription
                " <nb_udv></nb_udv>\n"             //        nombre de boites
                " <status></status>\n"             //        proprietes type F/traitement de fond et autres a venir
                " <smr></smr>\n"                   //        niveau du service medical rendu
                " <smr_l></smr_l>\n"               //        libelle du service medical rendu
                " <gph_html></gph_html>\n"         //        cache du contenu html d'une ligne (accelere l'affichage lors reprise d'une ordo car ne passe pas par l'interpreteur)
                " <gph_ald></gph_ald>\n"           //        prescrit en ALD "ALD"  sans ALD ""
                " <gph_id></gph_id>\n"             //        id un code d'identification unique du produit (exemple : CIP du medicament )
                " <gph_it></gph_it>\n"             //        id_type "CIP"  pour les medicaments CIP compatibles controle interaction, "cip" pour les autres disposant d'un CIP (accessoire veto etc..)
                " <gph_dr></gph_dr>\n"             //        date et heure de la redaction de la prescription    format dd-MM-yyyy hh:mm:ss
                " <gph_dt></gph_dt>\n"             //        date et heure du debut de prise de la prescription  format dd-MM-yyyy hh:mm:ss
                " <gph_df></gph_df>\n"             //        date et heure de fin de prise de la prescription    format dd-MM-yyyy hh:mm:ss (la date de fin peut etre calculee avec <gph_cy>)
                " <gph_na></gph_na>\n"             //        nom commercial et usuel du produit
                " <gph_dcl>\n"                     //        - debut de la liste des dci composant le produit
                "  <gph_dc>\n"                     //        -- debut du premier element de la liste des dci
                "    <gph_dcn></gph_dcn>\n"        //        --- nom   du premier element de la liste des dci
                "    <gph_dcp></gph_dcp>\n"        //        --- dose  du premier element de la liste des dci
                "    <gph_dcu></gph_dcu>\n"        //        --- unite du premier element de la liste des dci
                "  </gph_dc>\n"                    //        --fin du premier element de la liste des dci
                " </gph_dcl>\n"                    //        - fin de la liste des dci composant le produit
                " <gph_cy></gph_cy>\n"             //        duree totale de toutes les sequences (J15  S10 M10   U45M:M3R2) 'J' pour jours  ou 'S' pour semaines  ou 'M' pour mois  suivi du nbr et falcutatif 'R' pour renouvelable suivi du nbr
                " <gph_sb></gph_sb>\n"             //        substituable 0 = non substituable  1 = substituable
                " <gph_dci></gph_dci>\n"           //        affichage libelle produit : 3=que dci  2=dci+commercial 1=commercial+dci 0=que commercial (par defaut si vide c'est 3)
                " <gph_uf></gph_uf>\n"             //        forme galenique de l'unite de prise
                " <gph_voie></gph_voie>\n"         //        voie d'administration
                " <gph_fmin></gph_fmin>\n"         //        frequence minimum
                " <gph_fmax></gph_fmax>\n"         //        frequence maximum
                " <gph_funit></gph_funit>\n"       //        frequence unite
                " <gph_pmin></gph_pmin>\n"         //        posologie minimum
                " <gph_pmax></gph_pmax>\n"         //        posologie maximum
                " <gph_punit></gph_punit>\n"       //        posologie unite
                " <gph_pfc></gph_pfc>\n"           //        posologie facteur corporel qu de gph_punit par facteur corporel
                " <gph_pfcunit></gph_pfcunit>\n"   //        posologie facteur corporel unite Kg cm
                " <gph_pqbyuf></gph_pqbyuf>\n"     //        quantite d'unite  posologique par unite de forme de gph_punit par gph_uf
                " <gph_dmin></gph_dmin>\n"         //        duree minimum
                " <gph_dmax></gph_dmax>\n"         //        duree maximum
                " <gph_dunit></gph_dunit>\n"       //        duree unite
                " <gph_nm></gph_nm>\n"             //        conversion mode en mode numerique UP_QUANTITE MOMENT_MEAL ALL_LITERAL (on se sert de celui-ci car plus rapide a interpreter)
                " <gph_cm></gph_cm>\n"             //        conversion mode en mode literal pour information lisible de celui du mode numerique  UP_QUANTITE MOMENT_MEAL ALL_LITERAL on a pas peur des redondances ! (vive la memoire vive et les HD)
                " <gph_tr>\n"                      //        - debut de la liste des terrains por lesquels s'applique cette psologie
                "  <gph_trl>\n"                    //        -- debut du premier element de la liste des terrains
                    "<gph_trn></gph_trn>"          //        --- nom du premier element du teraain (correspond a une variable du contexte : age, poids, taille, valeur biologique ...)
                    "<gph_trs></gph_trs>"          //        --- borne inferieure
                    "<gph_tri></gph_tri>"          //        --- borne superieure
                    "<gph_tru></gph_tru>\n"        //        --- unite des bornes (an, kg, m2 cm2, mg ml UI ...)
                "  </gph_trl>\n"                   //        --fin du premier element de la liste des terrains
                " </gph_tr>\n"                     //        - fin de la liste des terrains por lesquels s'applique cette psologie
                " <gph_in>\n"                      //        - debut de la liste liste des indications
                "  <gph_il></gph_il>\n"            //        -- premier element de la liste des indications
                " </gph_in>\n"                     //        - fin de la liste des indications <gph_il> </gph_il>
                " <gph_co></gph_co>\n"             //        commentaire global en fin de prescription
                "</gph>\n"                         // -- FIN du header d'une ligne prescriptive ---
                "<gps>\n"                          // -- DEBUT de la liste des sequences prescriptives
                " <gpp></gpp>\n"                   //        premiere sequence prescriptive si plusieurs elle sont seront separees par le terme 'puis, ' dans le texte resultant
                "</gps>\n"                         // -- FIN de la liste des sequences prescriptives
                "<gpi_engine></gpi_engine>\n"      //        systeme de donnees a l'origine de cette ligne prescriptive
                "<gpi_user></gpi_user>\n"          //        utilisateur a l'origine de cette ligne prescriptive
 */

QString C_Frm_PosologieHitList::interfaceToGrammar()
{
 //............. formater liste des indications .....................
 QString   indications = interfaceToIndication_string().trimmed();
 QStringList indicList = indications.split("%%", QString::SkipEmptyParts);
 indications           = "";
 if ( indicList.size() )
    { for (int i=0; i<indicList.size(); ++i)
          { indications +="  <gph_il>"+indicList[i]+"</gph_il>\n";
          }
    }

 QStringList formesList;
 QStringList voiesList;
 QString grammarSequence = m_pC_Wdg_Stack_Sequence->interfaceToGrammar( formesList , voiesList) ;   // formes contiendra les formes indiquees dans des sequences, separees par |
 QString dci_list_xml    = " <gph_dcl>\n" + compositionToXml( m_pCompositionList ) + " </gph_dcl>\n";
 int     literalite_N    = 0;
 QString literalite_T    =  m_C_PopupDialDate->literaliteToString( literalite_N );
 QString s_prix          = c_frm_produit()->ucd_price().replace(',','.');
         s_prix          = C_BDM_Api::deleteUnsignificativesZeros(s_prix);
 double prix             = s_prix.toDouble();
 QString duration        =  m_C_PopupDialDate->interfaceToCycle().toUpper();
 int     durationInDays  = (int)C_PosologieGrammar::totalGlobalDurationInDays(  duration , 1);
 double     nb_up_total  = C_PosologieGrammar::qu_total_max_UP(durationInDays*1440, grammarSequence);
 double     nb_up_by_box = c_frm_produit()->nb_up().toDouble();
 if (nb_up_by_box==1)    // corriger bug des specialites telles que CARBOPLATINE ou nb_up_by_box est 1 (1 flacon) au lieu de 10 ml (on injecte des ml et non un flacon !!)
    { nb_up_by_box = 0;
    }
 double nb_boites        = nb_up_by_box>0?ceil ( nb_up_total / nb_up_by_box ):prix>0?0:0;
        prix            *= nb_boites;
 QString userInfo     = "    <m_Login>"        + m_pC_BDM_Api->userContexte()->get_Login()         + "</m_Login>\n"         +   // on garde les memes tag que ceux du jeton d'appel
                        "    <m_usual_name>"   + m_pC_BDM_Api->userContexte()->get_usual_name()    + "</m_usual_name>\n"    +
                        "    <m_forename>"     + m_pC_BDM_Api->userContexte()->get_forename()      + "</m_forename>\n"      +
                        "    <m_Nu_RPPS>"      + m_pC_BDM_Api->userContexte()->get_Nu_RPPS()       + "</m_Nu_RPPS>\n"       +
                        "    <m_Nu_ClefRPPS>"  + m_pC_BDM_Api->userContexte()->get_Nu_ClefRPPS()   + "</m_Nu_ClefRPPS>\n";
 QString ret          = QString (
                       "<gph>\n"
                       " <atc>%1</atc>\n"                   //     code ATC du produit
                       " <stup>%2</stup>\n"                 //     1/si supefiant  0/sinon
                       " <rfid>%3</rfid>"                   //     remboursable pour cette indication
                       " <price>%4</price>\n"               //     cout d'une boite
                       " <total_price>%5</total_price>\n"   //     cout total de la prescription ( prix udv * nb_udv )
                       " <nb_udv>%6</nb_udv>\n"             //     nb total d'unite de prise
                       " <nb_boites>%7</nb_boites>\n"       //     nb de boites de la prescription
                       ).arg( m_pC_BDM_Api->ATC_from_CIP( cip() ) ,
                              QString::number(m_pC_BDM_Api->isStupefiant( cip() )),
                              m_LineEdit_IndicRefund->text(),
                              s_prix,          // es tu la
                              QString::number(prix,'f',2),
                              QString::number(nb_up_total,'f',2),
                              QString::number( (int)nb_boites)
                            ) +

                       QString (
                       " <status>%1</status>\n"             //        propriete satut : F pour traitement de fond et autres \303\240 venir
                       " <smr></smr>\n"                     //        niveau du service medical rendu SMR
                       " <smr_l></smr_l>\n"                 //        libelle du service medical rendu SMR
                       " <gph_html></gph_html>\n"           //        contenu html (accelere l'affichage et sera mis a jour par C_Frm_Posologie.grammar() )
                       " <gph_ald>%2</gph_ald>\n"           //        signale si le produit est en ALD 1 ou non ALD 0
                       " <gph_pk></gph_pk>\n"               //        pk    (ne sert que pour les favoris)
                       " <gph_id>%3</gph_id>\n"             //        id
                       " <gph_it>%4</gph_it>\n"             //        id_type
                       " <gph_dr>%5</gph_dr>\n"             //        date et heure de la redaction de la prescription    format yyyy-MM-dd hh:mm:ss
                       " <gph_dt>%6</gph_dt>\n"             //        date et heure du debut  de prise de la prescription format yyyy-MM-dd hh:mm:ss // la date de fin est calculee avec <gph_cy>
                       " <gph_df>%7</gph_df>\n"             //        date et heure de fin  de prise de la prescription format yyyy-MM-dd hh:mm:ss // la date de fin est calculee avec <gph_cy>
                       " <gph_na>%8</gph_na>\n"             //        nom commercial et usuel du produit
                       )
                       .arg( m_status                   ,                                                           // 1   propriete : F pour traitement de fond et autres \303\240 venir
                             c_frm_produit()->ald()     ,                                                           // 2   ald
                             c_frm_produit()->id()      ,                                                           // 3   identifiant produit (le plus souvent e)
                             c_frm_produit()->id_type() ,                                                           // 4   type d'identifiant produit (le plus souvent "CIP")
                             QDateTime::currentDateTime().toString ("dd-MM-yyyy hh:mm:ss"),                         // 5   date et heure de la redaction
                             m_C_PopupDialDate->dateTime().toString("dd-MM-yyyy hh:mm:ss"),                         // 6   date et heure du debut de prise
                             m_C_PopupDialDate->dateTime().addDays(durationInDays).toString("dd-MM-yyyy hh:mm:ss"), // 7   date et heure de fin de prise
                             c_frm_produit()->commercialName()                                                      // 8   nom commercial
                           )        +
                       dci_list_xml +
                       QString (
                       " <gph_cy>%1</gph_cy>\n"             //        duree totale de toutes les sequences (J15  S10 M10   M3R2) 'J' pour jours  ou 'S' pour semaines  ou 'M' pour mois  suivi du nbr et falcutatif 'R' pour renouvelable suivi du nbr
                       " <gph_sb>%2</gph_sb>\n"             //        substituable 0 = non substituable  1 = substituable
                       " <gph_dci>%3</gph_dci>\n"           //        affichage libelle produit : 3=que dci  2=dci+commercial 1=commercial+dci 0=que commercial (par defaut si vide c'est 3)
                       " <gph_uf>%4</gph_uf>\n"             //        forme de l'unite de prise
                       " <gph_voie>%5</gph_voie>\n"         //        voie d'administration
                     ).arg (
                              duration,                                                   // 1   duree totale sous forme de pseudo code grammatical J15  S10 M10   M3R2
                              m_C_PopupDialDate->isSubstituable()?"1":"0",                // 2   substituable
                              m_C_PopupDialDate->dciToString(),                           // 3   type affichage libelle produit : 3=que dci  2=dci+commercial 1=commercial+dci 0=que commercial
                              formesList.join("|"),                                       // 4   unite de forme
                              voiesList.join(",")                                         // 5   voie d'administration
                           )+
                       QString (
                       " <gph_fmin></gph_fmin>\n"           //        frequence minimum
                       " <gph_fmax></gph_fmax>\n"           //        frequence maximum
                       " <gph_funit></gph_funit>\n"         //        frequence unite
                       " <gph_pmin>%1</gph_pmin>\n"         //        posologie minimum                                                         50 a
                       " <gph_pmax>%2</gph_pmax>\n"         //        posologie maximum                                                         60
                       " <gph_punit>%3</gph_punit>\n"       //        posologie unite                                                           mg
                       " <gph_pfc>%4</gph_pfc>\n"           //        posologie facteur corporel qu de gph_punit par facteur corporel           par 1
                       " <gph_pfcunit>%5</gph_pfcunit>\n"   //        posologie facteur corporel unite Kg cm                                    Kg de poids
                       " <gph_pqbyuf>%6</gph_pqbyuf>\n"     //        quantite d'unite posologique par unite de forme de gph_punit par gph_uf   500 mg par comprime
                       " <gph_psecable>%7</gph_psecable>\n" //        secabilite : -1/non secable 4/secable en 4 etc...
                       ).arg(
                             CGestIni::CutStrLeft (m_C_PopupDialCorpoFact->get_QuPA_by_FC(), "-"),    // 50  a
                             CGestIni::CutStrRight(m_C_PopupDialCorpoFact->get_QuPA_by_FC(), "-"),    // 60
                             m_C_PopupDialCorpoFact->get_QuPA_by_FC_Unit(),                           // mg
                             m_C_PopupDialCorpoFact->get_QuFC(),                                      // 3
                             m_C_PopupDialCorpoFact->get_QuFC_Unit(),                                 // Kg           --> 10 mg par 3 kg de poids
                             m_C_PopupDialCorpoFact->lineEdit_QPA_by_UP->text(),
                             (m_C_PopupDialCorpoFact->checkBox_secable->isChecked()?"4":"-1")
                            ) +
                       QString (
                       " <gph_dmin>%1</gph_dmin>\n"         //        duree minimum
                       " <gph_dmax>%2</gph_dmax>\n"         //        duree maximum
                       " <gph_dunit>%3</gph_dunit>\n"       //        duree unite
                       " <gph_nm>%4</gph_nm>\n"             //        conversion mode en mode numerique UP_QUANTITE MOMENT_MEAL ALL_LITERAL (on se sert de celui-ci car plus rapide a interpreter)
                       " <gph_cm>%5</gph_cm>\n"             //        conversion mode en mode literal pour information lisible de celui du mode numerique  UP_QUANTITE MOMENT_MEAL ALL_LITERAL on a pas peur des redondances ! (vive la memoire vive et les HD)
                       ).arg("",                            // 1   duree minimum
                             "",                            // 2   duree maximum
                             "",                            // 3   duree unite
                             QString::number(literalite_N), // 4   literalite numerique
                             literalite_T                   // 5   literalite textuelle (pour information du mode numerique)
                            )+
                        QString (
                       " <gph_tr>%1</gph_tr>\n"           //        liste des terrains de la posologie
                       " <gph_in>%2</gph_in>\n"           //        la liste des indications <gph_il> </gph_il>
                       " <gph_co>%3</gph_co>\n"           //        commentaire global en fin de prescription
                       "</gph>\n"                         // -- FIN du header d'une ligne prescriptive ---
                       "<gps>\n"                          // -- DEBUT de la liste des sequences pescriptives
                       "%4"                               //        premiere sequence prescriptive
                       "</gps>\n"                         // -- FIN de la liste des sequences pescriptives
                       ).arg(m_LineEditTerrain->getFreeData(),  // 1  donnees du terrain
                             indications,                       // 2   liste des indications
                             m_LineEditNote->text(),            // 3   commentaire global
                             grammarSequence                    // 4   sequences
                            )+
                         QString (
                        "<gpi_engine>%1</gpi_engine>\n"     //        systeme de donnees a l'origine de cette ligne prescriptive
                        "<gpi_user>\n%2</gpi_user>\n"       //        donn\303\251s utilisateur a l'origine de cette ligne prescriptive
                        ).arg( m_pC_BDM_Api->dataSourceVersion(),                          // 1   systeme de donnees a l'origine de cette ligne prescriptive
                               userInfo                                                    // 2   informations utilisateur a l'origine de cette ligne prescriptive
                             );
             // m_pC_BDM_Api->outMessage(tr(" Grammar Sequence : %1  <br/>%2").arg(c_frm_produit()->commercialName(), grammarSequence));
 return ret;
}

//------------------------------------------------------ clearInterface ---------------------------------------------------------------------------------
void C_Frm_PosologieHitList::clearInterface()
{m_pC_Wdg_Stack_Sequence->sequenceAllRemove();
 m_LineEditIndic->clear();
 m_LineEditNote->clear();
}

//------------------------------------------------------ setBackGround ---------------------------------------------------------------------------------
void C_Frm_PosologieHitList::setBackGround(QColor backGroundColor)
{   m_background_color=backGroundColor;

    m_Button_Indic->setColor(backGroundColor);
    m_Button_Note->setColor(backGroundColor);
    m_Button_Seq_0->setColor(backGroundColor);
    m_Button_Seq_1->setColor(backGroundColor);
    m_Button_Seq_2->setColor(backGroundColor);
    m_Button_Seq_3->setColor(backGroundColor);
    m_Button_Seq_4->setColor(backGroundColor);
    m_Button_Add_Fav->setColor(backGroundColor);
    m_Button_Del_Fav->setColor(backGroundColor);

    m_pC_Wdg_Stack_Sequence->setBackGround(backGroundColor);

}
//------------------------------------------------------ Slot_On_ButtonSequenceAdd_Clicked ---------------------------------------------------------------------------------
void C_Frm_PosologieHitList::Slot_On_ButtonSequenceAdd_Clicked(bool)
{sequenceAdd();
}

//------------------------------------------------------ sequenceAdd ---------------------------------------------------------------------------------
C_Frm_Sequence * C_Frm_PosologieHitList::sequenceAdd()
{   QRect                      rect = QRect(0, m_buttonH+m_marge+m_marge,this->width(),(m_buttonH*9)+m_marge);
    C_Frm_Sequence* pC_Frm_Sequence = m_pC_Wdg_Stack_Sequence->sequenceAdd(rect, this);
    if (pC_Frm_Sequence)
       { Slot_On_ButtonSeq_Clicked(pC_Frm_Sequence->objectName());
       }
    return pC_Frm_Sequence;
}

//------------------------------------------------------ Slot_On_ButtonSequenceDel_Clicked ---------------------------------------------------------------------------------
void C_Frm_PosologieHitList::Slot_On_ButtonSequenceDel_Clicked(bool)
{   int curIndex = m_pC_Wdg_Stack_Sequence->sequenceRemove();
    activeButtonSeq(curIndex);
}
//------------------------------------------------------ activeButtonSeq ---------------------------------------------------------------------------------
void C_Frm_PosologieHitList::activeButtonSeq(int index)
{ if (index != -1)
     { QString buttonName = QString("HitList : pC_Frm_Seq_%1").arg(QString::number(index));
       Slot_On_ButtonSeq_Clicked(buttonName);
     }
  else
     { m_Button_Seq_0->setEnabled(false);
       m_Button_Seq_1->setEnabled(false);
       m_Button_Seq_2->setEnabled(false);
       m_Button_Seq_3->setEnabled(false);
       m_Button_Seq_4->setEnabled(false);
     }
}

//------------------------------------------------------ Slot_On_ButtonSeq_Clicked ---------------------------------------------------------------------------------
void C_Frm_PosologieHitList::Slot_On_ButtonSeq_Clicked(const QString &buttonName)
{ int deltY = 5;
  int pos_y = m_marge + this->m_buttonH;
  m_Button_Seq_0->move(m_Button_Seq_0->x(), pos_y);
  m_Button_Seq_1->move(m_Button_Seq_1->x(), pos_y);
  m_Button_Seq_2->move(m_Button_Seq_2->x(), pos_y);
  m_Button_Seq_3->move(m_Button_Seq_3->x(), pos_y);
  m_Button_Seq_4->move(m_Button_Seq_4->x(), pos_y);
  //............... metre sidable or enbled .............
  adjustEnableStateStackButton();

  if (buttonName.endsWith("_Seq_0"))
     { m_pC_Wdg_Stack_Sequence->setCurrentIndex (0);
       m_Button_Seq_0->move(m_Button_Seq_0->x(), pos_y+deltY);
     }
  else if(buttonName.endsWith("_Seq_1"))
     { m_pC_Wdg_Stack_Sequence->setCurrentIndex (1);
       m_Button_Seq_1->move(m_Button_Seq_1->x(), pos_y+deltY);
     }
  else if(buttonName.endsWith("_Seq_2"))
     { m_pC_Wdg_Stack_Sequence->setCurrentIndex (2);
       m_Button_Seq_2->move(m_Button_Seq_2->x(), pos_y+deltY);
     }
  else if(buttonName.endsWith("_Seq_3"))
     { m_pC_Wdg_Stack_Sequence->setCurrentIndex (3);
       m_Button_Seq_3->move(m_Button_Seq_3->x(), pos_y+deltY);
     }
  else if(buttonName.endsWith("_Seq_4"))
     { m_pC_Wdg_Stack_Sequence->setCurrentIndex (4);
       m_Button_Seq_4->move(m_Button_Seq_4->x(), pos_y+deltY);
     }
}
//--------------------------------- adjustEnableStateStackButton  ------------------------------------------------------------------------------------------------------
void C_Frm_PosologieHitList::adjustEnableStateStackButton()
{  for (int i=0; i<5; ++i)
    { QWidget *pQWidget = m_pC_Wdg_Stack_Sequence->widget ( i );
      switch (i)
      { case 0: m_Button_Seq_0->setEnabled(pQWidget!=0); break;
        case 1: m_Button_Seq_1->setEnabled(pQWidget!=0); break;
        case 2: m_Button_Seq_2->setEnabled(pQWidget!=0); break;
        case 3: m_Button_Seq_3->setEnabled(pQWidget!=0); break;
        case 4: m_Button_Seq_4->setEnabled(pQWidget!=0); break;
      }
    }
}

//------------------------------------------------------ computeTextHit ---------------------------------------------------------------------------------
QString C_Frm_PosologieHitList::computeTextHit( const QString &grammar, int index )
{ QString type;
  int nextPos        = 0;
  QString status     = CGestIni::getXmlData("status",      grammar, &nextPos);
  QString terrainStr = CGestIni::getXmlData("gph_tr",      grammar, &nextPos);
  QString indicStr   = CGestIni::getXmlData("gph_in",      grammar, &nextPos);

  QString indication = C_PosologieGrammar::get_indication    ( indicStr,    index, C_PosologieGrammar::LIBELLE, &type).trimmed();
  QString terrain    = C_PosologieGrammar::get_terrainReadyDisplayString(terrainStr);

  QString modele =
                  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
                  "<html><head><meta name=\"qrichtext\" content=\"1\" />"
                  "  <style type=\"text/css\">"
                  "  span.numItem {"
                  "     color:%6;"
                  "     font-family: arial, verdana, sans-serif;"
                  "     font-size: 10px;"
                  "     font-weight: bold;"
                  "     background:%5;"
                  "}"
                  "  span.indication {"
                  "     color:#0000FF;"
                  "     font-family: arial, verdana, sans-serif;"
                  // "     text-decoration: underline;"
                  "     font-size: 10px;"
                  "}"
                  "  span.texteNormal {"
                  "     color:%4;"
                  "     font-family: arial, verdana, sans-serif;"
                  "     font-size: 10px;"
                  "}"
                  "  </style>"
                  "</head><body style=\" font-size:12px; font-weight:400; font-style:normal;\">"
                  "<span class=\"numItem\">%1&nbsp;</span>&nbsp;&nbsp;"
                  "<span class=\"indication\"><b><u>Terrain</u></b> : %7 <b><u>Indication</u></b> : %2</span>&nbsp;&nbsp;"
                  "<span class=\"texteNormal\">%3</span>"
                  "</body></html>";

 bool isCustomHit = status!="P";
 return modele.arg(QString::number(index+1),                                                             // 1
                   indication,                                                                           // 2
                   C_PosologieGrammar::toHtml(grammar),                                                  // 3       // si on indique le mode de conversion on l'impose, sinon c'est celui de la grammaire qui vaut
                   m_background_color.darker(200).name(),                                                // 4 couleur texte normal
                   isCustomHit?m_background_color.darker(120).name():"#f8cfde",                          // 5 couleur du fond du numero
                   isCustomHit?"#FF0000":"#FE0000",                                                      // 6 couleur du texte du numero
                   terrain                                                                               // 7 terrain
                 );
}
//------------------------------------------------------ Slot_ButtonHitClose_Clicked ---------------------------------------------------------------------------------
void C_Frm_PosologieHitList::Slot_ButtonHitClose_Clicked()   // le meme que ci-dessous mais sans parametre entre (peut etre appele par QTimer::singleShoot)
{   Slot_ButtonHitClose_Clicked(true);
}
//------------------------------------------------------ Slot_ButtonHitClose_Clicked ---------------------------------------------------------------------------------
void C_Frm_PosologieHitList::Slot_ButtonHitClose_Clicked(bool)
{   // qDebug()<< Q_FUNC_INFO;
    emit Sign_UpdateInteractions(interfaceToGrammar());  // surtout le faire ICI avant emit Sign_ButtonHitCloseClicked() qui detruit l'objet !!!!
    emit Sign_ButtonHitCloseClicked();                   // surtout le faire ICI apres emit Sign_UpdateInteractions() qui a besoin de l'objet
    //setToInitialRect();
}
//-------------------------------- literaliteToInt -------------------------------------------------------------------------------------------------------
int C_Frm_PosologieHitList::literaliteToInt()
{return m_C_PopupDialDate->literaliteToInt();
}
//------------------------------------------------------ c_frm_produit ---------------------------------------------------------------------------------
C_Frm_Produit *C_Frm_PosologieHitList::c_frm_produit()     // TODO dependance toxique \303\240 virer
{ return m_pC_Frm_Posologie->c_frm_produit();
}
//------------------------------------------------------ c_frm_posologie ---------------------------------------------------------------------------------
C_Frm_Posologie *C_Frm_PosologieHitList::c_frm_posologie()  // TODO dependance toxique \303\240 virer
{ return m_pC_Frm_Posologie;
}
//------------------------------------------------------ Slot_Button_Add_Fav_Clicked ---------------------------------------------------------------------------------
void C_Frm_PosologieHitList::Slot_Button_Add_Fav_Clicked(bool)
{   //............... si d'origine posos de la BDM ....................
    //         QString status = "P";        enlever l'attribut 'P'  (empeche l'enregistrement des hit donc d'un favori)
    //
    if (m_status=="P")  m_status = m_status.remove('P');  // le faire avant interfaceToGrammar() qui se sert de m_status pour posionner le tag <status>
    QString grammar      = interfaceToGrammar();
    QString     id       = "";
    QString     id_type  = "";
    QString          pk  = "";
    if (grammar.length())
       { //................... si ce favoris existe deja cassos ................
         int index = isExistsThisHit(grammar);
         if (index)
            { QToolTip::showText (QCursor::pos(), tr("Ce favoris existe d\303\251j\303\240 en %1, et ne sera donc pas rajout\303\251.").arg(QString::number(index)), this) ;
              return;
            }
         id      = C_PosologieGrammar::id     (grammar);
         id_type = C_PosologieGrammar::idType (grammar);
         pk      = m_pC_BDM_Api->increasePosologieHit(grammar, id, id_type);
         if ( pk.length() )
            { CGestIni::setXmlData("gph_pk", pk, grammar);          // noter le pk dans la grammaire (evite la recreation lors click dessus)
              //......... on fait la place du nouveau ................................
              QRect rect = geometry();
              rect.setHeight(rect.height()+m_Grid_H);
              setGeometry(rect);

              rect = m_pQListWidgetFavoris->geometry();
              rect.setHeight(rect.height()+m_Grid_H);
              m_pQListWidgetFavoris->setGeometry(rect);
              //........... on rajoute le nouveau .................
              CMyQTextEdit *pQTextEdit = addFavoriWidgetItem(grammar, m_pQListWidgetFavoris->count());
              m_FocusWidget = setWidgetEditFocus(pQTextEdit, m_FocusWidget);
              emit Sign_FavorisAdded();                // mettre a jour la liste des favoris dans l'interface principale
            }
       }  // if (grammar.length())
}

//------------------------------------------------------ getGrammarListFromIntemList ---------------------------------------------------------------------------------
QStringList C_Frm_PosologieHitList::getGrammarListFromIntemList()
{ QStringList       retList;
  CMyQTextEdit  *pQTextEdit = 0;
  int                    nb = m_pQListWidgetFavoris->count();
  for (int i=0; i<nb; ++i)
      { QListWidgetItem *pQListWidgetItem = m_pQListWidgetFavoris->item(i);
        if (pQListWidgetItem==0) continue;   // paranoiac attitude
        pQTextEdit = (CMyQTextEdit*) m_pQListWidgetFavoris->itemWidget(pQListWidgetItem);
        if (pQTextEdit==0)       continue;   // paranoiac attitude
        retList.append(pQTextEdit->getCustomString());
      }
  return retList;
}

//------------------------------------------------------ isExistsThisHit ---------------------------------------------------------------------------------
int C_Frm_PosologieHitList::isExistsThisHit(const QString &grammar)
{ QStringList        grammarList = getGrammarListFromIntemList();
  return C_PosologieGrammar::isExistsThis_HitGrammar_InList(grammar, grammarList);
}

//------------------------------------------------------ Slot_Button_Del_Fav_Clicked ---------------------------------------------------------------------------------
void C_Frm_PosologieHitList::Slot_Button_Del_Fav_Clicked(bool)
{
    if (m_FocusWidget==0)                                           return;
    QString                     grammar = m_FocusWidget->getCustomString();
    QListWidgetItem   *pQListWidgetItem = (QListWidgetItem*) m_FocusWidget->getCustomPtr();
    if (pQListWidgetItem==0)                                        return;
    int       index = m_pQListWidgetFavoris->row(pQListWidgetItem);
    if (m_pQListWidgetFavoris->takeItem(index) != pQListWidgetItem) return;   // ultra paranoiac attitude
    delete m_FocusWidget;
    m_FocusWidget   = 0;
    delete pQListWidgetItem;
    QString      pk =  C_PosologieGrammar::pk(grammar);
    if (pk.length())
       { m_pC_BDM_Api->deleteItemPosologieHit( pk );
       }
    int nbHits = 0;
    loadHitList(&nbHits);
    //......... on reduit la place de un ................................
    QRect rect = geometry();
    rect.setHeight(rect.height()-m_Grid_H);
    setGeometry(rect);

    rect = m_pQListWidgetFavoris->geometry();
    rect.setHeight(rect.height()-m_Grid_H);
    m_pQListWidgetFavoris->setGeometry(rect);

    //................ cassos si tous les hits on ete effaces .......................
    int nbItem   = m_pQListWidgetFavoris->count();         // pas egal a zero meme si plus de hits si il y en a provenant de la BDM commerciale
    if (nbHits==0)
       { emit Sign_FavorisAdded();                         // mettre a jour la liste des favoris dans l'interface principale
        if (nbItem==0) return;                             // >>>>> CASSOS car pas d'index a repositionner si liste vide cassos
       }
    //........... se repositionner sur l'index  suivant ...............
    //            en fait c'est celui de l'item qui vient d'etre efface...
    //            car apres effacement il pointe sur le prochain
    CMyQTextEdit *pCMyQTextEdit = 0;
    index                       = qMin(nbItem-1,index);
    pQListWidgetItem            = m_pQListWidgetFavoris->item ( index );
    pCMyQTextEdit               = (CMyQTextEdit*) m_pQListWidgetFavoris->itemWidget ( pQListWidgetItem );   // on retourne l'editeur en cours.
    m_FocusWidget               = setWidgetEditFocus(pCMyQTextEdit,  0);                                    // zero car le widget ayant le precedent focus n'existe plus
    m_pQListWidgetFavoris->setCurrentItem ( pQListWidgetItem );                                             // on le place comme item en cours dans la liste
    grammarToInterface(pCMyQTextEdit->getCustomString());                                                   // ou replace l'interface sur sa grammaire

}
//--------------------------------- Slot_HitItem_mousePressEvent ------------------------------------------------------------------------------------------------------
void C_Frm_PosologieHitList::Slot_HitItem_mousePressEvent(QMouseEvent * e , void *ptr)
{  Q_UNUSED(e);
   CMyQTextEdit* pQTextEdit  = (CMyQTextEdit*) ptr;                 // le signal est emis par le pQTextEdit associe au QListWidgetItem de la list view
   if (pQTextEdit == 0)    return;    // paranoiac attitude

   QListWidgetItem *pQListWidgetItem = (QListWidgetItem*) pQTextEdit->getCustomPtr();
   if (pQListWidgetItem==0) return;   // paranoiac attitude
   QString grammar = pQTextEdit->getCustomString();
   adjustFavoriToGrammar(grammar);    // met a jour m_cur_grammar
   grammarToInterface(m_cur_grammar);
   QString pk      =  m_pC_BDM_Api->increasePosologieHit(grammar, m_cip, m_idType);
   m_FocusWidget   = setWidgetEditFocus(pQTextEdit, m_FocusWidget);
   emit Sign_UpdateInteractions(interfaceToGrammar());  // surtout le faire ICI avant emit Sign_ButtonHitCloseClicked() qui detruit l'objet !!!!
}
//--------------------------------- Slot_HitItem_mouseDoubleClicked ------------------------------------------------------------------------------------------------------
void C_Frm_PosologieHitList::Slot_HitItem_mouseDoubleClicked( QMouseEvent *, void *)
{ //Slot_HitItem_mousePressEvent( e , ptr); pas la peine on est deja passe par la
  QTimer::singleShot(10,this, SLOT(Slot_ButtonHitClose_Clicked())); // si appel direct de Slot_ButtonHitClose_Clicked(true) --> plantage fatal
}

//------------------------------------------------------ setNext_or_PreviousEdit_Widget ---------------------------------------------------------------------------------
CMyQTextEdit *C_Frm_PosologieHitList::setNext_or_PreviousEdit_Widget(int sens)
{   CMyQTextEdit          *pCMyQTextEdit        = 0;
    QListWidgetItem     *pQListWidgetItem       = 0;
    int                         nbItem          = m_pQListWidgetFavoris->count();
    if (nbItem==0)                         return 0;
    //........... pas d'item selectionne on se place le premier de la liste ..................
    if (m_FocusWidget==0)
       {pQListWidgetItem = m_pQListWidgetFavoris->item ( 0 );
        m_pQListWidgetFavoris->setCurrentItem ( pQListWidgetItem );                                   // on le place comme item en cours dans la liste
        pCMyQTextEdit    =  (CMyQTextEdit*) m_pQListWidgetFavoris->itemWidget ( pQListWidgetItem );   // on retourne l'editeur en cours.
        return  pCMyQTextEdit;
       }
    //........... sinon item suivant ou precedent ..................
    pQListWidgetItem = (QListWidgetItem*) m_FocusWidget->getCustomPtr();
    if (pQListWidgetItem==0) return 0; // paranoiac attitude
    int   index = m_pQListWidgetFavoris->row (pQListWidgetItem);
    if (sens<0 && index > 0        ) index += sens;
    if (sens>0 && index < nbItem-1 ) index += sens;
    pQListWidgetItem = m_pQListWidgetFavoris->item ( index );
    m_pQListWidgetFavoris->setCurrentItem ( pQListWidgetItem );                                   // on le place comme item en cours dans la liste
    pCMyQTextEdit    =  (CMyQTextEdit*) m_pQListWidgetFavoris->itemWidget ( pQListWidgetItem );   // on retourne l'editeur en cours.
    grammarToInterface(pCMyQTextEdit->getCustomString());
    return  pCMyQTextEdit;
}
//------------------------------------------------------ setWidgetEditFocus ---------------------------------------------------------------------------------
CMyQTextEdit *C_Frm_PosologieHitList::setWidgetEditFocus(CMyQTextEdit *pQWidgetNew, CMyQTextEdit *pQWidgetOld)
{
 if (pQWidgetOld)
    { setItemListBackGround(pQWidgetOld, false);
    }
 if (pQWidgetNew==0) return 0;
 setItemListBackGround(pQWidgetNew,true);
 return pQWidgetNew;
}
//------------------------------------------------------ setItemListBackGround ---------------------------------------------------------------------------------
void C_Frm_PosologieHitList::setItemListBackGround(CMyQTextEdit *pQWidgetNew, bool isSelected)
{ if (isSelected) pQWidgetNew->setStyleSheet(QString("background-color: %1").arg(palette().color(QPalette::Base).name()));
  else            pQWidgetNew->setStyleSheet(QString("background-color: %1;").arg( background().lighter(110).name() )); //  arg(palette().color(QPalette::Window).darker(150).name()));
}

//------------------------------------------------------ setWidgetEditFocus ---------------------------------------------------------------------------------
void C_Frm_PosologieHitList::setWidgetEditFocus(int direction)
{if (direction == C_Frm_PosologieHitList::NEXT)  m_FocusWidget = setWidgetEditFocus(setNext_or_PreviousEdit_Widget(1),      m_FocusWidget);
 else                                            m_FocusWidget = setWidgetEditFocus(setNext_or_PreviousEdit_Widget(-1),     m_FocusWidget);
}

//------------------------------------------------------ mousePressEvent ---------------------------------------------------------------------------------
void C_Frm_PosologieHitList::mousePressEvent ( QMouseEvent *  )
{   /*
    QList <QObject *> objList = m_QFrameWidgetList->children();
    int nbItem                = objList.count();
    for (int i=0; i<nbItem; ++i)
        {QWidget* pQWidget = (QWidget*)objList[i];
         if (pQWidget->underMouse())
            {int index = objList.indexOf(pQWidget);
             m_FocusWidget = setWidgetEditFocus(pQWidget, m_FocusWidget);
             grammarToInterface(m_grammarList[index]);
             ev->accept();
             return ;
            }
        }
    */
}

//------------------------------------------------------ event ---------------------------------------------------------------------------------
bool C_Frm_PosologieHitList::event ( QEvent * ev )
{if ( ev->type() ==  QEvent::KeyRelease )
    {
     QKeyEvent                    *k = (QKeyEvent *)ev;
     // Qt::KeyboardModifiers modifiers = k->modifiers();
     switch (k->key())
         {case Qt::Key_Down:
               setWidgetEditFocus(C_Frm_PosologieHitList::NEXT);
               break;
          case Qt::Key_Up:
               setWidgetEditFocus(C_Frm_PosologieHitList::PREVIOUS);
               break;
          case Qt::Key_Return:
          case Qt::Key_Enter:
               break;
         } // end switch (key())
    }
 return QFrame::event (ev);
}

//------------------------------------------------------ paintEvent ----------------------------------------------------------------------
void C_Frm_PosologieHitList::paintEvent ( QPaintEvent * /*event*/ )
{
    QPainter p(this);
    //........... cadre a bords ronds .................................
    QRect rect = this->rect();
    rect.setWidth  (this->rect().width() -1); // -1 car bord droit et bas masques par le widget
    rect.setHeight (this->rect().height()-1); // -1 car bord droit et bas masques par le widget

    p.setPen (m_background_color.darker(160));
    QPainterPath path;
    path.addRoundedRect (rect, 6, 6);
    p.setBrush(m_background_color);
    p.drawPath(path);

    // p.drawPixmap (m_marge, m_marge + m_buttonH + m_buttonH + m_marge , m_pC_BitMapCollection->m_back_repart );

}
//====================================================== C_Frm_Posologie ======================================================================
//------------------------------------------------------ C_Frm_Posologie ----------------------------------------------------------------------
C_Frm_Posologie::C_Frm_Posologie (  C_BDM_Api           *pC_BDM_Api,          // pointeur sur l'API medicamenteuse
                                    C_Frm_Produit       *pC_Frm_Produit,      // on differencie le produit a traiter du parent (permet d'avoir pour parent n'importe quel autre objet)
                                    C_BitMapCollection  *pC_BitMapCollection, // il nous faut des icones
                                    QRect                geometry_rect,       // c'est le parent qui decide de la geometrie
                                    QWidget             *parent,              // le pere
                                    int                  isWithHitList
                                 )

         : QFrame(parent) //, C_PosologieGrammar(pC_Frm_Produit->schema_posologique())
{
  m_pC_BDM_Api          = pC_BDM_Api;
  m_pC_Frm_Produit      = pC_Frm_Produit;
  m_pC_BitMapCollection = pC_BitMapCollection;
  m_GrabIsOn            = 0;
  m_Hdeb_mn             = 0*60;
  m_Hfin_mn             = 24*60;
  m_first_time          = 7*60;
  m_grad_pos_y          = geometry_rect.height();
  m_grammar             = pC_Frm_Produit->schema_posologique();
  //................. si forme non definie on tente de la renseigner ......................
  if ( pC_Frm_Produit->id_type()=="CIP" )
     { m_compositionList     = pC_BDM_Api->getCompositionFromCIP(pC_Frm_Produit->id());

       if ( m_grammar.length() && CGestIni::getXmlData("gph_uf", m_grammar).remove("(s)").length()==0 )
          {  //......... TODO : rajouter les entrees xml si absentes .................................
             CGestIni::setXmlData("gph_uf", m_pC_BDM_Api->getPriseUnitFormFromCIP(pC_Frm_Produit->id()).remove("(s)"),  m_grammar);
          }
     }

  m_HitList             = 0;
  int           h_but   = 0;
    //............... rectangle du composant .............................
  setGeometry(geometry_rect);
  //................ largeur de la graduation ...........................
  QFont  fGrd            = font();  fGrd.setBold(false); fGrd.setItalic(false);
  QFontMetrics fntMetGrd = QFontMetrics(fGrd);
  QRect        boundRect = fntMetGrd.boundingRect ("X");
  h_but                  = boundRect.top()-boundRect.bottom();
  m_buttonH              = boundRect.height();
  fGrd.setPixelSize(10);
  m_grad_w               = geometry_rect.width() ; //m_pC_BitMapCollection->m_ButtonSequenceList.width()*3; //-boundRect.width()*3;  // 24 pour laisser la place des icones a droite
  m_grad_pos_y           = geometry_rect.height(); //-16;  // ligne de graduation 16 pixels au dessus de la ligne basse

  //..................... editeur de texte ......................................................
  m_textEdit     =  new QTextEdit(this);   m_textEdit->setFont(fGrd); m_textEdit->setReadOnly(true);
  m_textEdit->setVerticalScrollBarPolicy   ( Qt::ScrollBarAlwaysOff );
  m_textEdit->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
  m_textEdit->setTabChangesFocus ( true );
  m_textEdit->setGeometry(0,0,m_grad_w, m_grad_pos_y);

  setMouseTracking (true );

  //............. creer la liste des favoris .................................
  if (isWithHitList==C_Frm_Posologie::OPEN_POSO_EDITOR)         // on ouvre l'editeur de posologie
     { open_HitList();
     }
  else if (isWithHitList==C_Frm_Posologie::GENERATE_HTML_FROM_GRAMMAR)    // on ouvre pas l'editeur de posologie et on interprete la grammaire --> html (cache non utilisable: on vient de dropper un produit qui n'en a pas)
     { open_HitList();          // et bien si on cree l'editeur car trop complique de faire autrement (faut juste ne pas le montrer et le detruire ensuite)
     }
  else                          //  on ouvre pas l'editeur de posologie et on se sert directement du cache htmel de la grammaire (produit provenant d'une ordonnance)
     { m_textEdit->setHtml( CGestIni::getXmlData("gph_html", m_grammar) ); // on affiche en direct sans interpreter
     }
}

//------------------------ ~C_Frm_Posologie -------------------------------------------------------
C_Frm_Posologie::~C_Frm_Posologie()
{ close_HitList(1);
}

//------------------------------- getHitsList -------------------------------------
QStringList C_Frm_Posologie::getHitsList()
{
    //................recuperer la liste des favoris ...........................................
    QStringList lst         = m_pC_BDM_Api->selectPosologieHitList(m_pC_Frm_Produit->id(), m_pC_Frm_Produit->id_type());
    QString idType          =  c_frm_produit()->id_type();
    if ( idType != "CIP" ) return lst;
    //................installer la liste des posologies de la BDM ...........................................
    QString cip             =  c_frm_produit()->id();
    QString tmpGrammar      =  C_PosologieGrammar::empty_skeleton_grammar();
    QString quPA_by_UP      = "";
    QString unitPA          = "";
    if (m_compositionList.size())
       { unitPA     = m_compositionList.at(0).unit();
         quPA_by_UP = m_compositionList.at(0).amount();
       }
    QString dci_list_xml = C_Frm_PosologieHitList::compositionToXml(m_compositionList, "CIP");  //compositionToXml( &m_compositionList );
    CGestIni::setXmlData("gph_na" ,     c_frm_produit()->commercialName(),  tmpGrammar);
    CGestIni::setXmlData("gph_id",      cip,                                tmpGrammar, 1);
    CGestIni::setXmlData("gph_it",      idType,                             tmpGrammar, 1);
    CGestIni::setXmlData("gph_dcl",     dci_list_xml,                       tmpGrammar, 1);
    // QString unitPriseForm   =  m_pC_BDM_Api->getPriseUnitFormFromCIP(cip).remove("(s)");    // celle de l'API surcharge celle de la grammaire
    CGestIni::setXmlData("gph_punit" ,  unitPA,                             tmpGrammar);
    CGestIni::setXmlData("gph_pqbyuf" , quPA_by_UP,                         tmpGrammar);
    // if (unitPriseForm.length()) C_PosologieGrammar::setUnitPriseForm(tmpGrammar, unitPriseForm);

    CGestIni::setXmlData("stup", QString::number(m_pC_BDM_Api->isStupefiant ( cip ) ),  tmpGrammar);
    CGestIni::setXmlData("atc",  m_pC_BDM_Api->ATC_from_CIP(cip),                       tmpGrammar);

    QStringList dbm_posoList = m_pC_BDM_Api->getPosologiesFromCIP (m_pC_BDM_Api->patientContexte(), tmpGrammar);
    lst.append(dbm_posoList);
    return lst;
}

//------------------------------- setDuration -----------------------------------------------------------------
void  C_Frm_Posologie::setDuration( const QString &duration_syntax )
{ if (m_HitList)
     { m_HitList->setDuration( duration_syntax );
     }
  else if (m_grammar.length())
    { CGestIni::setXmlData("gph_cy", duration_syntax ,  m_grammar);
      QString html   = toHtml(m_grammar, m_pC_Frm_Produit->backGroundColor());
      m_textEdit->setHtml(html);
    }
}

//------------------------------- setDCI_Mode -----------------------------------------------------------------
//  <gph_dci>%3</gph_dci>   // affichage libelle produit : 3=que dci  2=dci+commercial 1=commercial+dci 0=que commercial (par defaut si vide c'est 3)
//   enum dci_flag  {COM  = 0, COM_DCI = 1, DCI_COM = 2, DCI = 3 };

void C_Frm_Posologie::setDCI_Mode(int mode)
{if (m_HitList)
    { m_HitList->setDCI_Mode( mode );
    }
 else if (m_grammar.length())
   { CGestIni::setXmlData("gph_dci", QString::number(mode) ,  m_grammar);
     QString html   = toHtml(m_grammar, m_pC_Frm_Produit->backGroundColor());
     m_textEdit->setHtml(html);
   }
}

//------------------------------- open_HitList -------------------------------------

void C_Frm_Posologie::open_HitList()
{   if ( m_HitList )
       { m_HitList->raise();
         m_HitList->setToDeployRect();
       }
    else
       { QStringList grammarHitList = getHitsList();
         int              PosoHit_H = m_pC_BitMapCollection->getCalculatedHitsList_H(m_buttonH, 2, ITEM_HIT_H , qMin(grammarHitList.size(),ITEM_HIT_NBTOVIEW));

         QRect geometry( this->geometry().x()           ,
                          parentWidget()->geometry().y() + parentWidget()->height(),
                          this->geometry().width()+20,
                          PosoHit_H + 4
                        );
          m_HitList = new C_Frm_PosologieHitList( grammarHitList,
                                                  m_grammar,                    // si non vide c'est elle qui sera installee sinon celle par defaut de la liste des favoris sinon une vide
                                                  this,
                                                  geometry,
                                                  m_buttonH,
                                                  m_pC_Frm_Produit->backGroundColor(),
                                                  m_pC_BitMapCollection,
                                                  m_pC_BDM_Api,
                                                  this->parentWidget()->parentWidget(),  // gni ?
                                                  QString ("HitList : %1 ").arg(m_pC_Frm_Produit->commercialName()).toLatin1(),
                                                  &m_compositionList
                                                 );
          //........................ les connexions sont apres pour eviter un raffraich a chaque modif des zones d'edition ...............................
          //                         lors de leur initialisation sur les valeurs
          connect( m_HitList,      SIGNAL( Sign_FavorisAdded()),                                 this ,      SIGNAL( Sign_FavorisAdded() ));                                   // mettre a jour la liste des favoris dans l'interface prineale
          connect( m_HitList,      SIGNAL( Sign_contentChanged(C_Frm_PosologieHitList*) ),       this ,      SLOT(   Slot_HitList_contentChanged(C_Frm_PosologieHitList*) ));    // on met a jour la grammaire
          connect( m_HitList,      SIGNAL( Sign_ButtonHitCloseClicked())                 ,       this ,      SLOT(   Slot_close_HitList() ));                                    // on detruit la m_HitList
          connect( m_HitList,      SIGNAL( Sign_UpdateInteractions(const QString &))     ,       this ,      SLOT(   Slot_UpdateInteractions(const QString &) ));                // mettre a jour les interactions
          connect( m_HitList,      SIGNAL( Sign_FactCorpoChanged( C_PatientCtx *)),              this ,      SIGNAL( Sign_FactCorpoChanged( C_PatientCtx *) ));
          connect( this,           SIGNAL( Sign_PatientCtxChanged(C_PatientCtx *)),         m_HitList ,      SIGNAL( Sign_PatientCtxChanged(C_PatientCtx *) ));

          //........................ les connexions etant apres pour eviter un raffraich a chaque modif des zones d'edition ...............................
          //                         il convient de mettre a jour l'affichage de l'editeur de texte a la mano ici
          Slot_HitList_contentChanged( m_HitList );
          m_HitList->show();
       }
    emit Sign_HitList_Opened( m_HitList );     // faire fermer tous les autres editeurs de posologie sauf celui qui vient d'etre cree ( m_HitList )
}

//------------------------------- Slot_UpdateInteractions -------------------------------------
void C_Frm_Posologie::Slot_UpdateInteractions(const QString &grammar)
{ emit Sign_UpdateInteractions(grammar);
}

//------------------------------- Slot_close_HitList -------------------------------------
void C_Frm_Posologie::Slot_close_HitList()
{   close_HitList(1);
}
//------------------------------- close_HitList -------------------------------------
void C_Frm_Posologie::close_HitList(int fast /* = 0 */)
{   if (m_HitList)
       { m_HitList->setToInitialRect(fast);
         delete m_HitList;                     // on se met en mode destructeur car interface
         m_HitList = 0;                        // trop lente si plusieurs editeurs ouverts
       }
}
//------------------------------- numOrdre --------------------------------------------------------
QString C_Frm_Posologie::numOrdre()
{ if (m_pC_Frm_Produit) return m_pC_Frm_Produit->numOrdre();
  return "";
}
//------------------------------- updateNumOrdreInHtmlView ----------------------------------------
// on ne va pas regenerer toute l'interpr\303\251tation de la grammaire, .
// on recupere l'html de la view on replace directement dedans le numero d'ordre et
// on replace le html dans la view
// color:#496975;">)</span>
void C_Frm_Posologie::updateNumOrdreInHtmlView( QString numOrdre )
{ QString html = m_textEdit->document()->toHtml ();
  int  pos_end = html.indexOf(")</span>");
  int  pos_deb = pos_end;
  if (pos_end==-1)  return;

  while (pos_deb>0)
  {if (html.at(pos_deb)=='>') break;
   --pos_deb;
  }
 if (pos_deb==0)   return;
 ++pos_deb;                   // passer le signe '>'
 html = html.replace(pos_deb,pos_end-pos_deb, numOrdre);
 m_textEdit->setHtml(html);
}

//------------------------ grammar ---------------------------------------
QString C_Frm_Posologie::grammar(int complete /* = 1 */)
{   if ( complete == 0 ) return m_grammar;
    if (c_frm_produit()->id_type()=="CIP")
       { //......... reperer si une indication n'a pas un SMR ...........................
        //           et le noter dans le SMR global de la ligne prescriptive
         QString indicStr      = "";     // recuperer indication
         QString level         = "";
         QStringList indicList = C_PosologieGrammar::get_indicationsList(m_grammar);
         if (indicList.size())
            { indicStr  = indicList.at(0);
              indicList = indicStr.split('|');      // on reprend indicList car faut pas deconner a toujours recreer des variables...
              if (indicList.size()>=3)
                 {if (indicList.at(1)=="SMR")
                     {  level = indicList.at(0);
                        level = level.remove('S');
                        CGestIni::setXmlData("smr", level,  m_grammar, 1);
                        CGestIni::setXmlData("smr_l",  C_BDM_Api::level_SMR_ToLibelle(level.toInt()),  m_grammar, 1);
                     }
                 }
            } // endif (indicList.size())
       }
    CGestIni::setXmlData("gph_html", m_textEdit->document()->toHtml(),  m_grammar,1);   // placer le contenu de l'editeur ici pour accelerer
    return  m_grammar;
}
//------------------------ set_ald ---------------------------------------
void   C_Frm_Posologie::set_ald(const QString &ald, const QColor &backGroundColor)
{   C_PosologieGrammar::setAld(m_grammar,ald);
    if ( m_HitList )
       {  m_HitList->setBackGround( backGroundColor );       // la couleur peut avoir change selon ald ou non ald
          m_HitList->set_ald(ald);
          m_HitList->update();
       }
}
//------------------------------- Slot_HitList_contentChanged -------------------------------------
void C_Frm_Posologie::Slot_HitList_contentChanged( C_Frm_PosologieHitList *pC_Frm_PosologieHitList )
{   m_grammar      = pC_Frm_PosologieHitList->interfaceToGrammar();
    QString html   = toHtml(m_grammar, pC_Frm_PosologieHitList->background());
    m_textEdit->setHtml(html);
    emit Sign_contentChanged( c_frm_produit() );
}
//------------------------------- toHtml ------------------------------------------------
QString  C_Frm_Posologie::toHtml(const QString &grammar, const QColor &background)
{  return  C_PosologieGrammar::toHtml (   grammar,
                                           m_pC_BitMapCollection->imagePath("NonSubstituable.png"),
                                           "" ,   // on prend le css par defaut
                                           background,
                                           numOrdre()
                                       );
}
//------------------------------- serialize ------------------------------------------------
QString  C_Frm_Posologie::serialize(const QString &ofset /*=""*/)
{   Q_UNUSED(ofset);
    return "";
}
//------------------------------- Slot_adjustToSize ------------------------------------------------
void C_Frm_Posologie::adjustToSize(const QRect &geometry_rect)
{  //............... rectangle du composant .............................
   setGeometry(geometry_rect);
   //............... ajuster hauteur editeur selon graduation ..................
   m_grad_pos_y           = geometry_rect.height();

   //..................... editeur de texte..........................................................
   m_textEdit->setGeometry(0,0,m_grad_w, m_grad_pos_y);

   if (m_HitList)
      {
       QRect deployRect  = QRect( geometry_rect.x()           ,
                                  parentWidget()->geometry().y() + parentWidget()->height(),
                                  geometry_rect.width() + 18,
                                  m_HitList->height() );
       QRect initialRect = QRect( geometry_rect.x()           ,
                                  parentWidget()->geometry().y() + parentWidget()->height(),
                                  1                 ,
                                  1 );

       m_HitList ->setGeometry( initialRect , deployRect );
      }
}

//------------------------ clearInterface ---------------------------------------
void C_Frm_Posologie::clearInterface()
{   m_textEdit->setText("");
}
//------------------------------------ is_HitListDeploy ------------------------------------------------
bool     C_Frm_Posologie::is_HitListDeploy()
{if ( m_HitList )
    {return m_HitList->height()>50;                // on bouge eventuellement son dialogue associe
    }
 return false;
}
//------------------------------- Slot_adjustToSize -------------------------------------
void C_Frm_Posologie::Slot_adjustToSize()
{ adjustToSize(this->geometry());
}

//------------------------ timeToPos_x ---------------------------------------
int C_Frm_Posologie::timeToPos_x(int nb_mn)
{  return (nb_mn*m_grad_w)/(m_Hfin_mn - m_Hdeb_mn);
}

//------------------------ paintEvent ---------------------------------------
void C_Frm_Posologie::paintEvent ( QPaintEvent * /*event*/ )
{
}

//---------------------------- mouseMoveEvent ------------------------------------------------
void    C_Frm_Posologie::mouseMoveEvent ( QMouseEvent * event )
{mouseMoveEvent ( event , 0 );
}

//---------------------------- mouseMoveEvent ------------------------------------------------
void    C_Frm_Posologie::mouseMoveEvent ( QMouseEvent * event , QWidget * /* pQWidget */)
{  QFrame::mouseMoveEvent ( event );
}

//---------------------------- mouseReleaseEvent ------------------------------------------------
void C_Frm_Posologie::mouseReleaseEvent ( QMouseEvent * /*event*/ )
{ releaseMouse();                // degraber la souris
  if (m_GrabIsOn==0) return;     // cassos si rien a faire
  m_GrabIsOn  = 0;
}

//---------------------------- mousePressEvent ------------------------------------------------
void C_Frm_Posologie::mousePressEvent(QMouseEvent *event)
{   if  (event->button() == Qt::LeftButton)
        {if (m_GrabIsOn==0)
            {
             m_startPosY       = event->pos().y();
             /*
             if (event->pos().x()<10)
                { //grabMouse(QCursor(Qt::ClosedHandCursor));
                  m_GrabIsOn = MIDLE_START;
                  //pC_Frm_Prescription->objectStartsMoving(this);
                  update();
                }
              */
             }
         else
             {/*
              QDrag *drag         = new QDrag(this);
              QMimeData *mimeData = new QMimeData;
              QRect rect(-4,0,width(),height());

              QPixmap pix = QPixmap::grabWidget (this, rect );
              mimeData->setText("Drag day");
              mimeData->setImageData(pix);
              drag->setMimeData(mimeData);
              drag->setPixmap(pix);
              Qt::DropAction dropAction = drag->exec();
              */
             }
         }
    else if ( event->button() == Qt::RightButton  && m_GrabIsOn==0)
         { //................ menu ....................
           /*
           ((C_Frm_Day*)parent())->Slot_StopTimer(1);   // le timer ser debloque lors du release button
           QString       ret =  ((C_Frm_Prescription*)parent())->doProduitMenu(this->Produit_instance(),1);
           // "Copy" "Replace" "Cut" "Type :" "Status :"  "Modify"  "Open" "Create" "Anomymize" "Quit"
           if (ret.length())
              {
                if (ret.indexOf("Modify") != -1)
                   {emit Sign_RendezVousChangeClicked(this);
                   }
               else if (ret.indexOf("Status :") != -1)
                  { QString statut = ret.remove("Status :");
                    m_State = statut.trimmed();
                    setWidgetOnRdv(*this);                // on reajuste le widget sur les nouvelles donn\303\251es
                    setWidgetStyleOnRdv(*this);
                    RDV_Update();
                  }
               else if (ret.indexOf("Type :") != -1)
                  { QString type = ret.remove("Type :");
                    m_Type = type.trimmed();
                    setWidgetOnRdv(*this);                // on reajuste le widget sur les nouvelles donn\303\251es
                    setWidgetStyleOnRdv(*this);
                    RDV_Update();
                  }
               else if (ret.indexOf("Cut") != -1)  // il ne  peut pas se detruire lui meme et sortir ensuite de sa fonction donc QTIMER
                  {QTimer::singleShot(10, this, SLOT(Slot_cut()) );
                  }
               else if (ret.indexOf("Copy") != -1)
                    Slot_copy();
               else if (ret.indexOf("Replace") != -1)
                    replaceWithCopy();
               else if (ret.indexOf("Modify") != -1)
                  {emit Sign_RendezVousChangeClicked(this);
                  }
               else if (ret.indexOf("Anomymize") != -1)
                  {m_Nom="";
                   m_Prenom="";
                   m_GUID="";
                   m_Tel="";
                   m_Note="";
                   m_Where="";
                   setWidgetOnRdv(*this);                // on reajuste le widget sur les nouvelles donn\303\251es
                   setWidgetStyleOnRdv(*this);
                   RDV_Update();
                  }
               else if (ret.indexOf("Delete") != -1)
                  {QTimer::singleShot(10, this, SLOT(Slot_delete()) );
                  }
               else if (ret.indexOf("Open") != -1)
                  {((C_Frm_Day*)parent())->Slot_ButtonAccederClicked(0, this);
                  }
               else if (ret.indexOf("Create") != -1)
                  {((C_Frm_Day*)parent())->Slot_ButtonAccederClicked(0, this);
                  }
               else if (ret.indexOf("Find Appointments") != -1)
                  {((C_Frm_Day*)parent())->Chercher_les_RDV_dun_patient();
                  }
               else if (ret.startsWith("AppointmentsListFor"))
                  {((C_Frm_Day*)parent())->Chercher_les_RDV_dun_patient(ret.mid(19));
                  }
               // CZF deb
               else if (ret.startsWith("PrintAppointments"))
                  {QDateTime Dtrdv = QDateTime::fromString(ret.mid(17,10) + " 00:00:00","yyyy-MM-dd hh:mm:ss");
                   ((C_Frm_Day*)parent())->Imprimer_les_RDV_dun_medecin(Dtrdv, ret.mid(28));
                  }
               // CZF fin
              }
           */
         event->accept();
         // ((C_Frm_Day*)parent())->Slot_StopTimer(0);
        }
 }
//------------------------------------ Slot_PatientCtxChanged ------------------------------------------------
/*! \brief this Slot is called when to notify that pC_PatientCtx contextis changed
 *  \param C_PatientCtx *pC_PatientCtx.  changed context
 */
void C_Frm_Posologie::Slot_PatientCtxChanged(C_PatientCtx *pC_PatientCtx)
{
 emit Sign_PatientCtxChanged(pC_PatientCtx);
}
//====================================================== C_Frm_Produit ======================================================================

//------------------------------------------------------ C_Frm_Produit ----------------------------------------------------------------------
C_Frm_Produit::C_Frm_Produit (  C_OrdoLineRecord  const  &ordoLineRecord     ,       // data
                                C_BDM_Api           *pC_BDM_Api,
                                C_BitMapCollection  *pC_BitMapCollection,
                                QRect                geometry_rect  ,
                                QWidget             *parent       ,
                                const QString       &str_background_noALD, /* = "#ffe3be" */
                                const QString       &str_background_isALD, /* = "#f5ffbe" */
                                int                  isWithHitList
                             )

         : QFrame(parent), C_OrdoLineRecord(ordoLineRecord)
{ m_deleted                          = false;
  m_pC_BDM_Api                       = pC_BDM_Api;
  m_background_noALD                 = QColor(str_background_noALD);
  m_background_isALD                 = QColor(str_background_isALD);
  m_GrabIsOn                         = 0;
  m_pC_Frm_Posologie                 = 0;
  m_pC_BitMapCollection              = pC_BitMapCollection;

  QFont  fGrd            = font(); fGrd.setPixelSize(10); fGrd.setBold(false); fGrd.setItalic(false);
  QFontMetrics fntMetGrd = QFontMetrics(fGrd);
  QRect        boundRect = fntMetGrd.boundingRect ("X");
  int buttonH            = boundRect.height();

  this->setObjectName("C_Frm_Produit");      // pour le reperer ensuite

  //................ on initialise les positions graphiques .............
  setGeometry(geometry_rect);
  //..................... recuperer parent qui maintient la collection de bitmap .....................
  C_Frm_Prescription *pC_Frm_Prescription = (C_Frm_Prescription*)this->parentWidget();   // parent; :: adjust_Vertical_PositionToGrid();
  if (pC_Frm_Prescription==0) return;
  //..................... bouton effacement ..........................................................
  m_ButtonDelete    = new CMyButton(pC_Frm_Prescription->m_objectsDel, this, "", this);
  QRect     rect    =  pC_Frm_Prescription->m_objectsDel.rect(); // rectangle icone
  m_ButtonDelete->setGeometry(geometry_rect.width()-rect.width()-3,2, rect.width(),rect.height());
  m_ButtonDelete->setFlat(true);
  m_ButtonDelete->setContentsMargins(0,0,rect.width(),rect.height());
  connect( m_ButtonDelete, SIGNAL( Sign_ButtonClickedPtr (const QString&, void *)  ),  pC_Frm_Prescription , SLOT( Slot_ButtonDeleteClicked(const QString&, void *) ));
  setMouseTracking (true );
  //..................... bouton numero ..........................................................
  m_ButtonNumber    = new CMyButton(this, "", this);
  m_ButtonNumber->setGeometry(0,0,16,16);
  m_ButtonNumber->setText(numOrdre());
  connect( m_ButtonNumber, SIGNAL( Sign_ButtonClickedPtr (const QString&, void *)  ),  pC_Frm_Prescription , SLOT( Slot_ButtonNumberClicked(const QString&, void *) ));

  //..................... l'editeur de posologie .....................................................
  int h_posoEdit = geometry_rect.height() -4; // -m_ButtonNumber->height()-2;
  int w_posoEdit = geometry_rect.width()-HANDLE_MOVE_WIDTH-OFSET_DROIT;
  m_pC_Frm_Posologie = new C_Frm_Posologie( pC_BDM_Api,
                                            this,
                                            pC_Frm_Prescription,
                                            QRect( HANDLE_MOVE_WIDTH,
                                                   2,   //m_ButtonNumber->height()-2,
                                                   w_posoEdit,
                                                   h_posoEdit),
                                                   this,
                                                   isWithHitList);
  connect( m_pC_Frm_Posologie,      SIGNAL(Sign_FavorisAdded()),                       this                , SLOT( Slot_FavorisAdded() ));                           // mettre a jour la liste des favoris dans l'interface prineale
  connect( m_pC_Frm_Posologie,      SIGNAL(Sign_UpdateInteractions(const QString &)),  this                , SLOT( Slot_UpdateInteractions(const QString &) ));      // mettre a jour les interactions
  connect( m_pC_Frm_Posologie,      SIGNAL(Sign_contentChanged( C_Frm_Produit* )),     pC_Frm_Prescription , SLOT( Slot_contentChanged( C_Frm_Produit* ) ));         // mettre a jour le prix total
  //................... les zones d'affichage du prix ...............................................
  int pos_x   = m_pC_Frm_Posologie->x() + m_pC_Frm_Posologie->width()  + 2;
  int pos_y   = m_pC_Frm_Posologie->y() + m_pC_Frm_Posologie->height() - buttonH*1 -4;
  int       w = this->width()-pos_x-4;
        rect  = QRect(pos_x,pos_y,w,buttonH);

  //..................... bouton NoSecure..........................................................
  if (  ordoLineRecord.id_type() != "CIP" )   // si produit hors API
     { rect = QRect(m_pC_Frm_Posologie->x()+m_pC_Frm_Posologie->width()+5,
                    m_ButtonDelete->y()+m_ButtonDelete->height()+1,
                    23,
                    23);
       m_ButtonNoSecure = new CMyButton(CMyButton::FLAT_REAL, m_pC_BitMapCollection->m_ButtonNoSecure,  rect,  this);
       m_ButtonNoSecure->setFlat(true);
       m_ButtonNoSecure->setToolTip(tr("Ce produit provient d'une origine non s\303\251curis\303\251e.<br/>"
                                       "Il ne fait l'objet d'aucun contr\303\264le de s\303\251curit\303\251 sur les allergies, interactions, terrain etc....<br/>"
                                       "Il est vous est donc recommand\303\251 d'\303\252tre particuli\303\250rement vigilant lors de sa prescription."));
       connect( m_ButtonNoSecure, SIGNAL( Sign_ButtonClicked (const QString&)  ),  this , SLOT(Slot_ButtonNoSecure(const QString&) ));
     }

  setFocusPolicy(Qt::ClickFocus);
  connect( qApp, SIGNAL( focusChanged(QWidget * , QWidget * )  ),  this , SLOT( Slot_focusChanged(QWidget * , QWidget * ) ));

  setMouseTracking (true );
  show();
}

//------------------------------- getTotalPrice -----------------------------------------------------------------
double C_Frm_Produit::getTotalPrice()
{ if ( m_pC_Frm_Posologie == 0 )              return 0;
  QString grammar          =  m_pC_Frm_Posologie->grammar(0);
  QString totalPrice       = CGestIni::getXmlData(     "total_price",  grammar);
  return totalPrice.toDouble();
}

//------------------------------- setDuration -----------------------------------------------------------------
void  C_Frm_Produit::setDuration( const QString &duration_syntax )
{
 if (m_pC_Frm_Posologie) m_pC_Frm_Posologie->setDuration( duration_syntax );
}
//------------------------------- adjustToSize -----------------------------------------------------------------
void C_Frm_Produit::adjustToSize(const QRect &rect)
{   setGeometry(rect);
    //..................... l'editeur de posologie .....................................................
    int h_posoEdit = rect.height()- 4; // m_ButtonNumber->height()-2;
    int w_posoEdit = rect.width() - HANDLE_MOVE_WIDTH-OFSET_DROIT;
    if (m_pC_Frm_Posologie) m_pC_Frm_Posologie->adjustToSize(QRect( HANDLE_MOVE_WIDTH, 2 /*m_ButtonNumber->height()-2*/ ,  w_posoEdit,  h_posoEdit ));
}
//------------------------ C_Frm_Produit ---------------------------------------
C_Frm_Produit::C_Frm_Produit(  C_Frm_Produit  const  &c_Frm_Produit)
    : QFrame(c_Frm_Produit.parentWidget()), C_OrdoLineRecord(c_Frm_Produit.ordoLineRecord())
{ m_pC_BDM_Api = c_Frm_Produit.m_pC_BDM_Api;
  m_deleted    = false;
  setGeometry(c_Frm_Produit.rect());
}

//------------------------ ~C_Frm_Produit -----------------------------------------------------------------
C_Frm_Produit::~C_Frm_Produit()
{
}

//------------------------------- Slot_UpdateInteractions -------------------------------------
void C_Frm_Produit::Slot_UpdateInteractions(const QString &grammar)
{ emit Sign_UpdateInteractions(grammar);
}
//------------------------------- Slot_FavorisAdded -----------------------------------------------------------------
void C_Frm_Produit::Slot_FavorisAdded()         // mettre a jour la liste des favoris dans l'interface prineale
{ emit Sign_FavorisAdded(this);
}
//------------------------------- Slot_ButtonNoSecure -----------------------------------------------------------------
void C_Frm_Produit::Slot_ButtonNoSecure(const QString&)
{   C_Frm_Prescription *pC_Frm_Prescription = (C_Frm_Prescription*)this->parentWidget();   // parent; :: adjust_Vertical_PositionToGrid();
    if (pC_Frm_Prescription==0) return;
    QPoint pos = QCursor::pos(); pos.setX(pos.x()+30);
    pC_Frm_Prescription->displayInfo(m_pC_BDM_Api->noSecureProductText(commercialName()));
}
//------------------------ Slot_focusChanged ---------------------------------------
void C_Frm_Produit::Slot_focusChanged(QWidget * /*old*/, QWidget * /*now*/)
{   /*
    C_Frm_PosologieHitList *pHitList = 0;
    if (m_pC_Frm_Posologie) pHitList = m_pC_Frm_Posologie->get_HitList();
    if (old) qDebug()<< tr("old : %1").arg(old->objectName());
    if (now) qDebug()<< tr("now : %1").arg(now->objectName());
    if (now && pHitList &&  pHitList->isAncestorOf(now)
        || now == pHitList) return;
    if (now && now->objectName().startsWith("HitList") ) return;
    //........................ le focus est sur now ..............................
    if ( isAncestorOf(now) )                             // si le produit est un parent de now cela peut etre la liste
       {                                                 // comme un objet du produit comme un de la liste
        qDebug()<< tr("ce produit '%1' est l'ancetre de now '%2'").arg(this->commercialName(), now->objectName());
        //........... si un clic concerne la liste on la deploie eventeullement ...................
        if (pHitList)
           {if (now == pHitList )                    return; // si clic sur la liste alors cassos on fait rien
            if ( pHitList->isAncestorOf(now) )               // si on clique sur un oblet de la liste (elle en est l'ancetre) alor cassos
               {  qDebug()<< tr(" la liste de ce produit '%1' est l'ancetre de now '%2' on laisse faire").arg(this->commercialName(), now->objectName());
                                                     return;
                  //m_pC_Frm_Posologie->close_HitList();
                  // qDebug()<< tr("close hitlist : %1").arg(this->commercialName());
               }
            if (pHitList && pHitList->height()<10)          // si non deployee la redeployer et cassos
               {m_pC_Frm_Posologie->open_HitList();  return;
               }
            return;
           }
       }
    if (m_pC_Frm_Posologie) m_pC_Frm_Posologie->close_HitList();
    */
}

//------------------------------- setColors -----------------------------------------------------------------
void   C_Frm_Produit::setColors(const QString &_std_color, const QString &_ald_color)
{QString std_color = _std_color;
 QString ald_color = _ald_color;
 std_color         = std_color.remove('#');
 ald_color         = ald_color.remove('#');
 if (std_color.length()) m_background_noALD = QColor(std_color.prepend('#'));
 if (ald_color.length()) m_background_isALD = QColor(ald_color.prepend('#'));
 update();
}

//------------------------ ordoLineRecord ---------------------------------------
C_OrdoLineRecord C_Frm_Produit::ordoLineRecord() const
{return C_OrdoLineRecord   ( id(),
                             id_type(),
                             commercialName(),
                             dci_1Name(),
                             dci_2Name(),
                             dci_3Name(),
                             owner(),
                             lang(),
                             nb_up(),
                             ucd_price(),
                             atc(),
                             generique(),
                             statut(),
                             //........ C_OrdoLineRecord .............
                             ordo_key(),
                             numOrdre(),
                             secureState(),
                             date_debut(),
                             date_redac(),
                             date_fin(),
                             pk_patient(),
                             guid_patient(),
                             code_indication(),
                             type_code_indication(),
                             smr(),
                             smr_indication(),
                             voie_admin(),
                             ald(),
                             substituable(),
                             renouvelable(),
                             quantite_totale(),
                             forme_galenique(),
                             unite_de_prise(),
                             quantite_par_up(),
                             unite_quantite(),
                             note_textuelle_libre(),
                             schema_posologique()
                            );
}

//------------------------ setOrderNumber ---------------------------------------
void     C_Frm_Produit::setOrderNumber(int num)
{QString txt = QString::number(num);
 m_ButtonNumber->setText(QString::number(num));
 set_numOrdre(txt);
}

//------------------------ paintEvent ---------------------------------------
void C_Frm_Produit::paintEvent ( QPaintEvent * /*event*/ )
{   QPainter p(this);
    QColor   background_color = backGroundColor();

    p.setPen (background_color.darker(160));
    QPainterPath path;
    QRect rect = this->rect();
    rect.setWidth  (this->rect().width() -1); // -1 car bord droit et bas masques par le widget
    rect.setHeight (this->rect().height()-1); // -1 car bord droit et bas masques par le widget
    path.addRoundedRect (rect, 6, 6);
    p.setBrush(background_color);
    p.drawPath(path);
    //............ dessin de la poignee de deplacement a gauche ...............
    p.fillRect ( 1, 4, HANDLE_MOVE_WIDTH-1, height()-8, background_color.lighter(118) );
    //............ dessin de la zonne parametrage a droite ....................
    p.setOpacity (0.50 );
    p.drawPixmap (rect.width()-16, rect.height()-20, Theme::getIcon("medicatux/prescriptor.png"));
    // QFrame::paintEvent(event); surtout pas !!
}

//---------------------------- mouseMoveEvent ------------------------------------------------
void    C_Frm_Produit::mouseMoveEvent ( QMouseEvent * event )
{mouseMoveEvent ( event , 0 );
}

//---------------------------- mouseMoveEvent ------------------------------------------------
void    C_Frm_Produit::mouseMoveEvent ( QMouseEvent * event , QWidget * /* pQWidget */)
{C_Frm_Prescription *pC_Frm_Prescription = (C_Frm_Prescription*)this->parentWidget();   // parent; :: adjust_Vertical_PositionToGrid();
 if (pC_Frm_Prescription==0) return;
 if (m_GrabIsOn)
    {if ( m_GrabIsOn==MIDLE_START)
        {pC_Frm_Prescription->objectDoMoving(this);
        }
    }
  else
    { if (event->x()<HANDLE_MOVE_WIDTH-1)
         { setCursor(Qt::OpenHandCursor);
         }
      else
         {setCursor(Qt::ArrowCursor);
         }
    }
  QFrame::mouseMoveEvent ( event );
}

//---------------------------- mouseReleaseEvent ------------------------------------------------
void C_Frm_Produit::mouseReleaseEvent ( QMouseEvent * /*event*/ )
{releaseMouse();                // degraber la souris
 if (m_GrabIsOn==0) return;     // cassos si rien a faire
 //................. appel du parent pour terminer le move ..............................
 C_Frm_Prescription *pC_Frm_Prescription = (C_Frm_Prescription*)this->parentWidget();
 if (pC_Frm_Prescription==0) return;
 pC_Frm_Prescription->objectStopsMoving(this);
 m_GrabIsOn  = 0;
}

//---------------------------- mousePressEvent ------------------------------------------------
void C_Frm_Produit::mousePressEvent(QMouseEvent *event)
{   C_Frm_Prescription *pC_Frm_Prescription = (C_Frm_Prescription*)this->parentWidget();
    if (pC_Frm_Prescription==0) return;

    if  (event->button() == Qt::LeftButton)
        {if (m_GrabIsOn==0)
            {
             m_startPosY       = event->pos().y();
             if (event->pos().x()<HANDLE_MOVE_WIDTH-1)
                { //grabMouse(QCursor(Qt::ClosedHandCursor));
                  m_GrabIsOn = MIDLE_START;
                  pC_Frm_Prescription->objectStartsMoving(this);
                  update();
                }
              else if (m_pC_Frm_Posologie)
                { m_pC_Frm_Posologie->open_HitList();
                }
             }
         else
             {/*
              QDrag *drag         = new QDrag(this);
              QMimeData *mimeData = new QMimeData;
              QRect rect(-4,0,width(),height());

              QPixmap pix = QPixmap::grabWidget (this, rect );
              mimeData->setText("Drag day");
              mimeData->setImageData(pix);
              drag->setMimeData(mimeData);
              drag->setPixmap(pix);
              Qt::DropAction dropAction = drag->exec();
              */
             }
         }
    else if ( event->button() == Qt::RightButton  && m_GrabIsOn==0)
         { //................ menu ....................
           /*
           ((C_Frm_Day*)parent())->Slot_StopTimer(1);   // le timer ser debloque lors du release button
           QString       ret =  ((C_Frm_Prescription*)parent())->doProduitMenu(this->Produit_instance(),1);
           // "Copy" "Replace" "Cut" "Type :" "Status :"  "Modify"  "Open" "Create" "Anomymize" "Quit"
           if (ret.length())
              {
                if (ret.indexOf("Modify") != -1)
                   {emit Sign_RendezVousChangeClicked(this);
                   }
               else if (ret.indexOf("Status :") != -1)
                  { QString statut = ret.remove("Status :");
                    m_State = statut.trimmed();
                    setWidgetOnRdv(*this);                // on reajuste le widget sur les nouvelles donn\303\251es
                    setWidgetStyleOnRdv(*this);
                    RDV_Update();
                  }
               else if (ret.indexOf("Type :") != -1)
                  { QString type = ret.remove("Type :");
                    m_Type = type.trimmed();
                    setWidgetOnRdv(*this);                // on reajuste le widget sur les nouvelles donn\303\251es
                    setWidgetStyleOnRdv(*this);
                    RDV_Update();
                  }
               else if (ret.indexOf("Cut") != -1)  // il ne  peut pas se detruire lui meme et sortir ensuite de sa fonction donc QTIMER
                  {QTimer::singleShot(10, this, SLOT(Slot_cut()) );
                  }
               else if (ret.indexOf("Copy") != -1)
                    Slot_copy();
               else if (ret.indexOf("Replace") != -1)
                    replaceWithCopy();
               else if (ret.indexOf("Modify") != -1)
                  {emit Sign_RendezVousChangeClicked(this);
                  }
               else if (ret.indexOf("Anomymize") != -1)
                  {m_Nom="";
                   m_Prenom="";
                   m_GUID="";
                   m_Tel="";
                   m_Note="";
                   m_Where="";
                   setWidgetOnRdv(*this);                // on reajuste le widget sur les nouvelles donn\303\251es
                   setWidgetStyleOnRdv(*this);
                   RDV_Update();
                  }
               else if (ret.indexOf("Delete") != -1)
                  {QTimer::singleShot(10, this, SLOT(Slot_delete()) );
                  }
               else if (ret.indexOf("Open") != -1)
                  {((C_Frm_Day*)parent())->Slot_ButtonAccederClicked(0, this);
                  }
               else if (ret.indexOf("Create") != -1)
                  {((C_Frm_Day*)parent())->Slot_ButtonAccederClicked(0, this);
                  }
               else if (ret.indexOf("Find Appointments") != -1)
                  {((C_Frm_Day*)parent())->Chercher_les_RDV_dun_patient();
                  }
               else if (ret.startsWith("AppointmentsListFor"))
                  {((C_Frm_Day*)parent())->Chercher_les_RDV_dun_patient(ret.mid(19));
                  }
               // CZF deb
               else if (ret.startsWith("PrintAppointments"))
                  {QDateTime Dtrdv = QDateTime::fromString(ret.mid(17,10) + " 00:00:00","yyyy-MM-dd hh:mm:ss");
                   ((C_Frm_Day*)parent())->Imprimer_les_RDV_dun_medecin(Dtrdv, ret.mid(28));
                  }
               // CZF fin
              }
           */
         event->accept();
         // ((C_Frm_Day*)parent())->Slot_StopTimer(0);
        }
 }
//------------------------------------ move ------------------------------------------------
void     C_Frm_Produit::move(int x, int y)
{ int initialY = this->y();
  QFrame::move(x,y);                         // on bouge le composant lui meme
  if ( m_pC_Frm_Posologie )                  // on bouge eventuellement son dialogue associe
     { C_Frm_PosologieHitList*   hitList = m_pC_Frm_Posologie->get_HitList();
       if (hitList)
          {hitList->setBackGround(backGroundColor());       // la couleur peut avoir change selon ald ou non ald
           hitList->moveY( this->y()-initialY);
          }
     }
}
//------------------------------------ set_ald ------------------------------------------------
void     C_Frm_Produit::set_ald( const QString &ald )
{  C_OrdoLineRecord::set_ald( ald) ;
   if ( m_pC_Frm_Posologie )
      { m_pC_Frm_Posologie->set_ald( ald, backGroundColor() );
      }
}
//------------------------------------ is_HitListDeploy ------------------------------------------------
bool     C_Frm_Produit::is_HitListDeploy()
{if ( m_pC_Frm_Posologie )
    {return m_pC_Frm_Posologie->is_HitListDeploy();                // on bouge eventuellement son dialogue associe
    }
 return false;
}
//------------------------------------ close_HitList ------------------------------------------------
void     C_Frm_Produit::close_HitList(int fast /* = 0 */)
{if ( m_pC_Frm_Posologie )  m_pC_Frm_Posologie->close_HitList(fast);                // on bouge eventuellement son dialogue associe
}
//------------------------------------ open_HitList ------------------------------------------------
void     C_Frm_Produit::open_HitList()
{if ( m_pC_Frm_Posologie )  m_pC_Frm_Posologie->open_HitList();
}

//------------------------ serialize ---------------------------------------
QString C_Frm_Produit::serialize(const QString &ofset  /*=""*/)
{  Q_UNUSED(ofset);
   return "";
}
//------------------------ grammar ---------------------------------------
QString C_Frm_Produit::grammar()
{ if ( m_pC_Frm_Posologie )
     {   return  m_pC_Frm_Posologie->grammar();
     }
  return "";
}

// =================================== C_Frm_Prescription ================================================
//------------------------------------ C_Frm_Prescription ------------------------------------------------
C_Frm_Prescription::C_Frm_Prescription(const QString &imagePath, C_BDM_Api *pC_BDM_Api , QScrollArea *pQScrollArea , QWidget *parent /*=0 */)
    : QFrame(parent), C_BitMapCollection(imagePath)
{m_pC_BDM_Api            = pC_BDM_Api;
 m_pQScrollArea          = pQScrollArea;
 m_C_Frm_Produit_H       = H_PRODUIT;
 m_C_Frm_Produit_W       = W_PRODUIT;
 m_infoTexEdit           = 0;
 m_inWorks               = 0;
 m_SpaceBetweenProduits  = SPACE_BETWEEN_PROD;
 m_Grid_H                = m_C_Frm_Produit_H + m_SpaceBetweenProduits;
 m_Nb_Grid_H             = 60;
 m_mimeType              = "text/medintux_prd_drag";
 m_mimeTypeIconPath      = "";
 m_pC_InteractionsThread = 0;
 m_RequestInteractEval   = 0;
 setGeometry(0, 0, m_C_Frm_Produit_W + 20, m_Grid_H * m_Nb_Grid_H + FIRST_OBJ_POS_Y);
 setMouseTracking (true );
 setAcceptDrops (true);
 //connect( this, SIGNAL( close () ),  this , SLOT( Slot_close() ));
}
//----------------------------------------- ~C_Frm_Prescription ----------------------------------------------------------
C_Frm_Prescription::~C_Frm_Prescription()
{
 if (m_infoTexEdit)
    {m_infoTexEdit->close();
     m_infoTexEdit->deleteLater();
    }
}
//------------------------------- Slot_contentChanged -------------------------------------
void C_Frm_Prescription::Slot_contentChanged( C_Frm_Produit * pC_Frm_Produit =0 )
{    double total_price = 0;
     QList <C_Frm_Produit*> childsList = C_Frm_ProduitList();
     for (int i = 0; i < childsList.size(); ++i)
         { pC_Frm_Produit = childsList.at(i);
           if ( !pC_Frm_Produit->isToDeleteLater() ) total_price += pC_Frm_Produit->getTotalPrice();
         }
    emit Sign_UpdateTotalPrice( QString::number(total_price,'f',2) );
}

//------------------------------- Slot_UpdateInteractions -------------------------------------
void C_Frm_Prescription::Slot_UpdateInteractions(const QString &grammar)
{   Q_UNUSED(grammar);
    Slot_evaluateAllInteractions();
}
//----------------------------------------- setGrammarList  ----------------------------------------------------------
void C_Frm_Prescription::setGrammarList(const QStringList &grammarList, int interactionsMustBeeEvaluated /* = C_Frm_Prescription::EVALUATE_INTERACTIONS */)
{
 if (grammarList.size()==0) return;
 int pos_y         =  FIRST_OBJ_POS_Y;
 if (  C_PosologieGrammar::ald (grammarList.at(0)) != "ALD" ) pos_y += m_Grid_H;  // si pas de produit ald on commence une case plus loin
 pos_y += m_SpaceBetweenProduits/2;

 for (int i = 0; i<grammarList.size(); ++i)
     {QString grammar =  grammarList.at(i);
      C_OrdoLineRecord ordoLine;
      int nextPos = 0;
      ordoLine.set_ucd_price      ( CGestIni::getXmlData(      "price",     grammar, &nextPos) );
      ordoLine.set_statut         ( CGestIni::getXmlData(      "status",    grammar, &nextPos) );

      ordoLine.set_ald            ( CGestIni::getXmlData(      "gph_ald",   grammar, &nextPos) );
      ordoLine.set_id             ( CGestIni::getXmlData(      "gph_id",    grammar, &nextPos) );
      ordoLine.set_id_type        ( CGestIni::getXmlData(      "gph_it",    grammar, &nextPos) );
      ordoLine.set_date_redac     ( QDateTime::fromString(CGestIni::getXmlData(      "gph_dr",    grammar, &nextPos).left(10),"dd-MM-yyyy hh:mm:ss") );
      ordoLine.set_date_debut     ( QDateTime::fromString(CGestIni::getXmlData(      "gph_dt",    grammar, &nextPos).left(10),"dd-MM-yyyy hh:mm:ss") );
      ordoLine.set_commercialName ( CGestIni::getXmlData(      "gph_na",    grammar, &nextPos) );
      //............... traiter la dci ............................
      QStringList dci_NameList    = CGestIni::getXmlDataList( "gph_dcn",     CGestIni::getXmlData(      "gph_dcl",   grammar, &nextPos) );
      dci_NameList << "" << "" << "";             // on assure au moins une liste de trois elements
      ordoLine.set_dci_1Name(dci_NameList[0]),
      ordoLine.set_dci_2Name(dci_NameList[1]),
      ordoLine.set_dci_3Name(dci_NameList[2]),
      //............... traiter le reste ............................
      ordoLine.set_substituable         ( CGestIni::getXmlData(      "gph_sb",   grammar, &nextPos));
      ordoLine.set_forme_galenique      ( CGestIni::getXmlData(      "gph_uf",   grammar, &nextPos));
      ordoLine.set_voie_admin           ( CGestIni::getXmlData(      "gph_voie", grammar, &nextPos));
      ordoLine.set_note_textuelle_libre ( CGestIni::getXmlData(      "gph_co",   grammar, &nextPos));
      ordoLine.set_owner                ( CGestIni::getXmlData(      "m_Login",  CGestIni::getXmlData( "gpi_user" , grammar , &nextPos ) ) );
      ordoLine.set_lang("fr");
      ordoLine.set_numOrdre(QString::number(i+1)) ;
      ordoLine.set_schema_posologique(grammar);

      createProduct(ordoLine,  pos_y ,  0 );
      pos_y += m_Grid_H;
     }
 if ( interactionsMustBeeEvaluated == C_Frm_Prescription::EVALUATE_INTERACTIONS ) Slot_evaluateAllInteractions();
}
//----------------------------------------- set_Ald_On_All_Products  ----------------------------------------------------------
void C_Frm_Prescription::set_Ald_On_All_Products(const QString &ald)
{   QStringList grammar_list = grammarList();
    for ( int i=0; i<grammar_list.size(); ++i )
        {CGestIni::setXmlData("gph_ald" , ald,  grammar_list[i]);
        }
    delete_All_Products(         C_Frm_Prescription::DO_NOT_EVALUATE_INTERACTIONS );
    setGrammarList(grammar_list, C_Frm_Prescription::DO_NOT_EVALUATE_INTERACTIONS );
}
//----------------------------------------- set_Duration_On_All_Products  ----------------------------------------------------------
void C_Frm_Prescription::set_Duration_On_All_Products(const QString &duration_syntax)
{   QList <C_Frm_Produit*> childsList = C_Frm_ProduitList();
    for (int i = 0; i < childsList.size(); ++i)
        { C_Frm_Produit *pC_Frm_Produit = childsList.at(i);
          pC_Frm_Produit->setDuration( duration_syntax );
        }
    Slot_evaluateAllInteractions();
}
//----------------------------------------- set_notificatorView  ----------------------------------------------------------
void C_Frm_Prescription::set_notificatorView( QTextEdit *pQTextEdit )
{ m_infoTexEdit = pQTextEdit;
}
//----------------------------------------- notificatorView  ----------------------------------------------------------
QTextEdit *C_Frm_Prescription::notificatorView()
{ return m_infoTexEdit;
}
//----------------------------------------- Slot_close  ----------------------------------------------------------
void C_Frm_Prescription::Slot_close()
{
}
//----------------------------------------- displayInfo ----------------------------------------------------------
void C_Frm_Prescription::displayInfo(const  QString &info)
{ m_infoTexEdit->setHtml(info);
}

//----------------------------------------- get_displayInteractionWindowGeometry ----------------------------------------------------------
QRect C_Frm_Prescription::get_displayInteractionWindowGeometry()
{
  QRect retRect = QRect(-1,-1,-1,-1);
  if (m_infoTexEdit)
     { retRect = m_infoTexEdit->geometry();
     }
  return retRect;
}

//----------------------------------------- setZoom ----------------------------------------------------------
void C_Frm_Prescription::setZoom(int steep)
{//................... ajuster la feuille de stockage des objets .................................
 m_C_Frm_Produit_H += steep;
 setChildsSize(0, m_C_Frm_Produit_H);
}

//------------------------------- getChildsSize -----------------------------------------------------------------
QSize C_Frm_Prescription::getChildsSize()
{return QSize(m_C_Frm_Produit_W, m_C_Frm_Produit_H);
}
//------------------------------- setDCI_Mode -----------------------------------------------------------------
//    enum dci_flag  {COM  = 0, COM_DCI = 1, DCI_COM = 2, DCI = 3 };
void C_Frm_Prescription::setDCI_Mode(int mode)
{   m_dci_mode = mode;
    QList <C_Frm_Produit*> childsList  = C_Frm_ProduitList();
    for (int i = 0; i < childsList.size(); ++i)
        {   C_Frm_Produit *pC_Frm_Produit = childsList.at(i);
            if ( pC_Frm_Produit->m_pC_Frm_Posologie )
               { pC_Frm_Produit->m_pC_Frm_Posologie->setDCI_Mode( mode);
               }
        }
}

//------------------------------- setChildsColors -----------------------------------------------------------------
void   C_Frm_Prescription::setChildsColors(const QString &_std_color, const QString &_ald_color)
{QString std_color = _std_color;
 QString ald_color = _ald_color;
 std_color = std_color.remove('#').prepend('#');      // on s'assure que '#' est correctement mis
 ald_color = ald_color.remove('#').prepend('#');      // on s'assure que '#' est correctement mis

 if (std_color.length()>1) m_str_background_noALD = std_color;
 if (ald_color.length()>1) m_str_background_isALD = ald_color;

 QList <C_Frm_Produit*> childsList = C_Frm_ProduitList();
 for (int i = 0; i < childsList.size(); ++i)
     { C_Frm_Produit *pC_Frm_Produit = childsList.at(i);
       pC_Frm_Produit->setColors(m_str_background_noALD, m_str_background_isALD);
     }
}

//------------------------------- setChildsSize -----------------------------------------------------------------
void C_Frm_Prescription::setChildsSize(int childs_w, int childs_h)
{   if (childs_h) m_C_Frm_Produit_H  = childs_h;
    if (childs_w) m_C_Frm_Produit_W  = childs_w;

    m_Grid_H = m_C_Frm_Produit_H + m_SpaceBetweenProduits;
    setGeometry(0, 0, m_C_Frm_Produit_W + 20, m_Grid_H * m_Nb_Grid_H + FIRST_OBJ_POS_Y);
    //................. repositionner tous les fils ..................................................
    QList <C_Frm_Produit*> childsList = C_Frm_ProduitList();
    //......... trier la liste selon le numero d'ordre ...............................................
    //          avec l'astuce de la QMap
    QMap<int, C_Frm_Produit*> map;
    foreach (C_Frm_Produit* pC_Frm_Produit, childsList)
         { map.insert(pC_Frm_Produit->numOrdre().toInt(), pC_Frm_Produit);
         }
    //......... recuperer la liste triee .............................................................
    //          et redimenssionner les fils
    int pos_y  = FIRST_OBJ_POS_Y;
    childsList = map.values();
    for (int i = 0; i < childsList.size(); ++i)
        { C_Frm_Produit *pC_Frm_Produit = childsList.at(i);
          if (i==0 && pC_Frm_Produit->ald().length()==0) pos_y += m_Grid_H;
          pC_Frm_Produit->adjustToSize(QRect(FIRST_OBJ_POS_X, pos_y, m_C_Frm_Produit_W, m_C_Frm_Produit_H));
          pos_y += m_Grid_H;
        }
}

//-------------------------------- event ---------------------------------------------------------
bool     C_Frm_Prescription::event ( QEvent * e )
{
  return QFrame::event(e);
}

//-------------------------------- mouseMoveEvent ---------------------------------------------------------
void C_Frm_Prescription::mouseMoveEvent(QMouseEvent *event)
{    /*
    if (event->buttons() & Qt::LeftButton)
       {
        int distance = (event->pos() - m_startDragPos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
           {//QTreeWidgetItem *item = getFirstSelectedItem();
            //if (item)
               {QString data = m_mimeDatas;
                //emit Sign_giveMeDatas(data, item);
                if (data.length())
                   {
                    QMimeData *mimeData = new QMimeData;
                    QDrag *drag         = new QDrag(this);
                    //if (m_mimeTypeIconPath.length())   drag->setDragCursor ( QPixmap (m_mimeTypeIconPath), Qt::CopyAction);  // marche po
                    mimeData->setData(m_mimeType, data.toLatin1());
                    drag->setMimeData(mimeData);
                    if (m_mimeTypeIconPath.length()) drag->setPixmap(QPixmap(m_mimeTypeIconPath));
                    if (drag->exec(Qt::CopyAction) == Qt::CopyAction)
                       {event->accept();
                        //return;
                       }
                  }
             }
           }
        }
     */
    QFrame::mouseMoveEvent(event);
}

//---------------------------- dragEnterEvent ------------------------------------------------
void C_Frm_Prescription::dragEnterEvent(QDragEnterEvent *event)
{if (event->mimeData()->hasFormat("text/medintux_prd_drag"))
    {//................ recuperer la chaine xml dropee .................................
     set_DropedXmlDatas (QString(event->mimeData()->data ( "text/medintux_prd_drag" ))); // recuperer les donnees venant du drag and drop
     event->acceptProposedAction ();
     emit Sign_dragEnterEvent(); // C_Frm_Prescription
    }
 }
//---------------------------- dragMoveEvent ------------------------------------------------
void C_Frm_Prescription::dragMoveEvent(QDragMoveEvent * event)
 {
    if (event->mimeData()->hasFormat("text/medintux_prd_drag"))
       {
        //........... faire scroller si besoin .......................................
        QPoint pos_g = mapFromGlobal ( QCursor::pos() );
        m_pQScrollArea->ensureVisible(10,pos_g.y());
        //........... afficher tool tip du D&D ASTUCE ................................
        //            pour que le tool tip suive la souris il faut
        //            qu'il se rafraichisse continuellement, pour cela
        //            seul le changement de contenu textuel opere ce rafraichissement
        //            donc pour ce faire j'affiche la position de la souris avec la
        //            couleur du fond, afin de masquer cet affichage changeant continuellement.
        QColor backColor   = QToolTip::palette ().color(QPalette::ToolTipBase);                                     // recuperer couleur fond du tool tip
        QPoint     pos     = QCursor::pos(); pos.setX(pos.x()+30);                                                  // positionner le tool tip
        QString    tm_str  = "<html>"
                             "<head>"
                             "  <meta name=\"qrichtext\" content=\"charset=utf-8\" />"
                             "</head>"
                             "<body text=\"#000000\" style=\"font-size:9pt;font-family:MS Shell Dlg\"><ul>"     +
                             dragGetListDrugsName().replace(" ","&nbsp;") +  // affichage du nom des medocs
                             "</ul><font color=\"" + backColor.name() + "\">" +QString::number(pos.y()+pos.x()) +
                             "</font></body></html>";   // affichage invisible de la position pour creer un changement du contenu textuel afin de raffraichir l'affichage lors des d\303\251placements

        QToolTip::showText (pos,  tm_str, this, rect() );
        event->acceptProposedAction ();
       }
 }

//---------------------------- dragGetListDrugsName ------------------------------------------------
QString C_Frm_Prescription::dragGetListDrugsName()
{QStringList listName = CGestIni::getXmlDataList("m_commercialName", get_DropedXmlDatas());
 QString result       = "";
 for (int i=0; i<listName.size();++i)
     {result += "<li>" + listName.at(i)+"</li>";   // si <br />     ca ne le fait pas !!!!
     }
 return result;
}

//---------------------------- dragLeaveEvent ------------------------------------------------
void C_Frm_Prescription::dragLeaveEvent ( QDragLeaveEvent *  )
{   QToolTip::hideText();
}
//---------------------------- dropEvent ------------------------------------------------
void C_Frm_Prescription::dropEvent(QDropEvent *event)
{   //qDebug()<<"dropEvent : "<< event->format();
    if (event->mimeData()->hasFormat("text/medintux_prd_drag"))
       {event->acceptProposedAction ();
        QToolTip::hideText();
        QTimer::singleShot(10, this, SLOT(Slot_Drop_Prd ()));
       }
    /*
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))      // "application/x-qabstractitemmodeldatalist"
        { event->acceptProposedAction ();
          QToolTip::hideText();
          QTreeWidget   *tree = dynamic_cast<QTreeWidget *>(event->source());
          QByteArray itemData = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
          QDataStream stream(&itemData, QIODevice::ReadOnly);

          int r, c;
          QMap<int, QVariant> v;
          stream >> r >> c >> v;
          QTreeWidgetItem *item = tree->topLevelItem(r);
          if( item )
            {  qDebug()<<"dropEventItem : "<< item->text(0);
              //itemDropped(item);
            }
         QTimer::singleShot(10, this, SLOT(Slot_Drop_Prd ()));
        }
     */
 }
//------------------------ Slot_Drop_Prd -----------------------------------------------------
void C_Frm_Prescription::Slot_Drop_Prd ()
{   QPoint         pos            = mapFromGlobal ( QCursor::pos() );
    int            pos_y          = pos.y();
    QString     xml_datas         = get_DropedXmlDatas();
    QStringList xml_productsList  = CGestIni::getXmlDataList("C_BDM_DrugListRecord", xml_datas);
    int                        nb = xml_productsList.size();
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    for (int i=0; i<nb; ++i)
        { if (i<nb-1) appendNewObject(  xml_productsList.at(i), pos_y,
                                        C_Frm_Posologie::GENERATE_HTML_FROM_GRAMMAR,
                                        C_Frm_Prescription::DO_NOT_EVALUATE_INTERACTIONS);
          else        appendNewObject(  xml_productsList.at(i), pos_y,                      //  ouvrir l'editeur et evaluer les interactions au dernier ajout
                                        C_Frm_Posologie::OPEN_POSO_EDITOR,
                                        C_Frm_Prescription::EVALUATE_INTERACTIONS);
          pos_y += m_Grid_H;
        }
     QApplication::restoreOverrideCursor();
}

//------------------------ idExists ----------------------------------------------------
QList <C_Frm_Produit*>  C_Frm_Prescription::idExists(const QString &id)
{   QList <C_Frm_Produit*> retChildsList;
    QList <C_Frm_Produit*> childsList = C_Frm_ProduitList();
    for (int i = 0; i < childsList.size(); ++i)
        {C_Frm_Produit *pC_Frm_Produit = childsList.at(i);
         if (pC_Frm_Produit->id()==id)retChildsList.append(pC_Frm_Produit);
        }
    return retChildsList;
}
//------------------------ nextFreePosY -----------------------------------------------------
int C_Frm_Prescription::nextFreePosY( bool not_ALD)
{   QList <C_Frm_Produit*> childsList = C_Frm_ProduitList();
    int nb = childsList.size();
    int pos_y = FIRST_OBJ_POS_Y + (childsList.size() * m_Grid_H) + m_Grid_H / 2;
    if (nb==0 && not_ALD) pos_y += m_Grid_H;
    return  pos_y;
}
//------------------------ appendNewObject -----------------------------------------------------
C_Frm_Produit* C_Frm_Prescription::appendNewObject( const QString &xmlData, bool not_ALD,
                                                    int isWithHitList                   , /* = C_Frm_Posologie::OPEN_POSO_EDITOR */
                                                    int interactionsMustBeeEvaluated      /* = C_Frm_Prescription::EVALUATE_INTERACTIONS*/
                                                  )
{  return appendNewObject(xmlData,  nextFreePosY(not_ALD), isWithHitList, interactionsMustBeeEvaluated);
}
//------------------------ appendNewObject -----------------------------------------------------
C_Frm_Produit* C_Frm_Prescription::appendNewObject( const QString &xmlData, int pos_y,
                                                    int isWithHitList                , /* = C_Frm_Posologie::OPEN_POSO_EDITOR        */
                                                    int interactionsMustBeeEvaluated   /* = C_Frm_Prescription::EVALUATE_INTERACTIONS */
                                                  )
{ return  appendNewObject(C_OrdoLineRecord(xmlData),  pos_y, isWithHitList, interactionsMustBeeEvaluated);
}
//------------------------ appendNewObject -----------------------------------------------------
C_Frm_Produit* C_Frm_Prescription::appendNewObject( C_OrdoLineRecord ordoLine, int pos_y,
                                                    int isWithHitList                   , /* = C_Frm_Posologie::OPEN_POSO_EDITOR */
                                                    int interactionsMustBeeEvaluated      /* = C_Frm_Prescription::EVALUATE_INTERACTIONS*/
                                                   )
{
    bool           is_in_Ald      = is_posY_in_ALD_Zone(pos_y);
    pos_y                         = adjust_Vertical_PositionToGrid(pos_y);
    C_Frm_Produit *pC_Frm_Produit = 0;

    //.................. verifier si ce medicament n'est pas deja present .................................
    if ( idExists(ordoLine.id()).size())
       { if (QMessageBox::question ( this,
                                     tr("Redondance"),
                                     tr("Ce produit : <b>%1</b> <br />"
                                        "est d\303\251j\303\240 pr\303\251sent, confirmez vous son ajout").arg(ordoLine.commercialName()),
                                     QMessageBox::Cancel | QMessageBox::Ok,
                                     QMessageBox::Cancel
                                   ) == QMessageBox::Cancel) return 0;
       }
    //.................. faire la place du nouvel arrivant .................................
    //                   detecter si relache sur la premiere case ALD vide
    //                   auquel cas ne pas deplacer les sous jacents vers le bas
    QList <C_Frm_Produit*> listObjects = objectListAt(pos_y, C_Frm_Prescription::JUST_ONE_POSITION_UNDER_Y);  // liste des objets deja pr\303\251sents sur cette case (exclusion de celui en cours de deplacement)

    if (listObjects.count())   // si un objet sous la souris d\303\251placer tous ceux sous jacent vers le bas pour faire la place du nouveau arrivant
       { pC_Frm_Produit = listObjects.at(0);
         deplace_Objects(pos_y + m_Grid_H,                  // on deplace tout ce qui se situe apres ca
                                 m_Grid_H);                 // d'une hauteur de grille vers le bas
         pos_y =  pC_Frm_Produit->y() + m_Grid_H;
       }

    // ..................calcul de la position la plus basse avant de creer l'objet ....................
    int nb_Prd;
    int nb_Ald = count_ALD(&nb_Prd); // au moins une case ALD vide
    if (nb_Ald==0) ++nb_Ald;         // car si pas d'ald on decale tout de 1 vers le bas
    pos_y = qMin(pos_y, (nb_Ald + nb_Prd) * m_Grid_H + FIRST_OBJ_POS_Y + m_SpaceBetweenProduits/2);

    if (is_in_Ald) ordoLine.set_ald("ALD");
    //.................. creer le nouvel arrivant  dans la place faite .................................
    pC_Frm_Produit = createProduct(ordoLine,  pos_y, isWithHitList );
    if (pC_Frm_Produit)
       { rearangeAllNumberOrder();
         update();
         clear_DropedXmlDatas ();  // effacer les donnees venant du drag and drop
         //.............. tester interactions de toute la liste de medicaments ....................................
         if ( interactionsMustBeeEvaluated == C_Frm_Prescription::EVALUATE_INTERACTIONS ) QTimer::singleShot(10, this, SLOT(Slot_evaluateAllInteractions()));
       }
    return pC_Frm_Produit;
}

//------------------------------------ createProduct ------------------------------------------------
/*! \brief create a product widget
 *  \param  const C_OrdoLineRecord &ordoLine  datas
 *  \param  int pos_y  position
 *  \param  int isWithHitList  1/if HitList must be created 0/in not
 *  \return C_Frm_Produit* a pointer on created product 0 if not
 */
C_Frm_Produit *C_Frm_Prescription::createProduct(const C_OrdoLineRecord &ordoLine, int pos_y, int isWithHitList /* = 1 */ )
{   //.................. creer le nouvel arrivant  dans la place faite .................................
    C_Frm_Produit *pC_Frm_Produit= new C_Frm_Produit( ordoLine,
                                                      m_pC_BDM_Api,
                                                      this,
                                                      QRect(FIRST_OBJ_POS_X, pos_y, m_C_Frm_Produit_W, m_C_Frm_Produit_H),
                                                      this,
                                                      m_str_background_noALD,
                                                      m_str_background_isALD,
                                                      isWithHitList
                                                    );

    //.................. placer si besoin le caractere ALD selon emplacement............................
    //                   dans lequel le nouvel arrivant se met
    if ( pC_Frm_Produit )
       { connect(  pC_Frm_Produit, SIGNAL( Sign_FavorisAdded(C_Frm_Produit*)  ),       this , SLOT( Slot_FavorisAdded(C_Frm_Produit*) ));          // mettre a jour la liste des favoris dans l'interface prineale
         connect(  pC_Frm_Produit, SIGNAL( Sign_UpdateInteractions(const QString &)),  this , SLOT( Slot_UpdateInteractions(const QString &) ));   // mettre a jour les interactions pour une grammaire

         if ( pC_Frm_Produit->m_pC_Frm_Posologie )
            { pC_Frm_Produit->m_pC_Frm_Posologie->setDCI_Mode(m_dci_mode);
              connect( pC_Frm_Produit->m_pC_Frm_Posologie, SIGNAL( Sign_child_LeaveLastEditZone ()  ),    this , SIGNAL( Sign_child_LeaveLastEditZone()    ));
              if (pC_Frm_Produit->m_pC_Frm_Posologie)
                 { //connect( pC_Frm_Produit->m_pC_Frm_Posologie->m_HitList, SIGNAL( Sign_deployed(C_PopupDial *) ),          this , SLOT( Slot_HitListDeployed(C_PopupDial *) ));
                   connect( pC_Frm_Produit->m_pC_Frm_Posologie ,           SIGNAL( Sign_HitList_Opened( C_PopupDial * ) ),  this , SLOT(   Slot_HitListDeployed(C_PopupDial *) ));
                   connect( pC_Frm_Produit->m_pC_Frm_Posologie,            SIGNAL( Sign_FactCorpoChanged( C_PatientCtx *)), this , SIGNAL( Sign_FactCorpoChanged( C_PatientCtx *) ));
                   connect( this, SIGNAL( Sign_PatientCtxChanged(C_PatientCtx *)),      pC_Frm_Produit->m_pC_Frm_Posologie , SIGNAL( Sign_PatientCtxChanged(C_PatientCtx *) ));
                   if (isWithHitList==C_Frm_Posologie::OPEN_POSO_EDITOR) Slot_HitListDeployed(pC_Frm_Produit->m_pC_Frm_Posologie->m_HitList);      // replier les autres m_HitList
                 }
            }
         Slot_contentChanged();
       }
    return pC_Frm_Produit;
}
//------------------------------------ Slot_PatientCtxChanged ------------------------------------------------
/*! \brief this Slot is called when to notify that pC_PatientCtx context is changed
 *  \param C_PatientCtx *pC_PatientCtx.  changed context
 */
void C_Frm_Prescription::Slot_PatientCtxChanged(C_PatientCtx *pC_PatientCtx)
{ emit Sign_PatientCtxChanged(pC_PatientCtx);
  Slot_evaluateAllInteractions();
}
//------------------------------------ Slot_HitListDeployed ------------------------------------------------
/*! \brief this Slot is called when a m_HitList is deployed to close all other m_HitList.
 *  \param  C_PopupDial *pC_PopupDial.  deployed dialog (only this one must be open)
 */
void C_Frm_Prescription::Slot_HitListDeployed(C_PopupDial *pC_PopupDial)
{   C_Frm_PosologieHitList *pC_Frm_PosologieHitList = (C_Frm_PosologieHitList*) pC_PopupDial;
    QList <C_Frm_Produit*> childsList  = C_Frm_ProduitList();
    for (int i = 0; i < childsList.size(); ++i)
        {   C_Frm_Produit *pC_Frm_Produit = childsList.at(i);
            if (pC_Frm_Produit->m_pC_Frm_Posologie && pC_Frm_Produit->m_pC_Frm_Posologie->m_HitList)
               { C_Frm_PosologieHitList *cur_pC_Frm_PosologieHitList = pC_Frm_Produit->m_pC_Frm_Posologie->m_HitList;
                 if (cur_pC_Frm_PosologieHitList != pC_Frm_PosologieHitList)
                    { pC_Frm_Produit->close_HitList(1);
                      //cur_pC_Frm_PosologieHitList->setToInitialRect();
                    }
               }
        }
    m_pQScrollArea->ensureVisible ( pC_PopupDial->x(), pC_PopupDial->y() ,0,pC_PopupDial->deployRect().height()+10);
}
//------------------------------------ close_All_HitList ------------------------------------------------
/*! \brief  close all m_HitList.
 */
void C_Frm_Prescription::close_All_HitList(int fast /* = 0 */)
{   QList <C_Frm_Produit*> childsList  = C_Frm_ProduitList();
    for (int i = 0; i < childsList.size(); ++i)
        {   C_Frm_Produit *pC_Frm_Produit = childsList.at(i);
            if (pC_Frm_Produit->m_pC_Frm_Posologie && pC_Frm_Produit->m_pC_Frm_Posologie->m_HitList)
               { C_Frm_PosologieHitList *cur_pC_Frm_PosologieHitList = pC_Frm_Produit->m_pC_Frm_Posologie->m_HitList;
                 cur_pC_Frm_PosologieHitList->setToInitialRect(fast);
               }
        }
}
//------------------------------------ get_HitListDeployed ------------------------------------------------
/*! \brief return the deployed m_HitList.
 *  \return  C_PopupDial *pC_PopupDial.  deployed dialog zeron only
 */
C_PopupDial *C_Frm_Prescription::get_HitListDeployed()
{   QList <C_Frm_Produit*> childsList  = C_Frm_ProduitList();
    for (int i = 0; i < childsList.size(); ++i)
        {   C_Frm_Produit *pC_Frm_Produit = childsList.at(i);
            if (pC_Frm_Produit->is_HitListDeploy()) return pC_Frm_Produit->m_pC_Frm_Posologie->m_HitList;
        }
    return 0;
}
//------------------------------------ set_Thread_DBM_Plugin ------------------------------------------------
/*! \brief set user BDM plugin interface (class interface for unified commercial database).
 *  \param  const QString &_namePlugin name of plugin (Datasemp, Theriaque ...)
 *  \param int oldMustBeDeleted
 *         if 1 the instance plugin must be deleted an a new instance created (not only get a already instancied in m_pC_BDM_PluginI_Map)
 *         if 0 get if exists a already instancied in m_pC_BDM_PluginI_Map(faster)
 */
void C_Frm_Prescription::set_Thread_DBM_Plugin(const QString &_namePlugin, int oldMustBeDeleted /*=0*/)
{   if (m_pC_InteractionsThread==0) return;
    if (oldMustBeDeleted)
       { m_mutex.lock();
         while ( m_pC_InteractionsThread->isRunning() );
         m_RequestInteractEval   = 1;
         delete m_pC_InteractionsThread;
         // m_pC_InteractionsThread->deleteLater();
         m_pC_InteractionsThread = 0;
         m_RequestInteractEval   = 0;
         Slot_evaluateAllInteractions();  // recree le thread
         m_mutex.unlock();
         return;
       }
    m_pC_InteractionsThread->set_Thread_BDM_Plugin(_namePlugin, 0);
}

//------------------------------------ Slot_evaluateAllInteractions ------------------------------------------------
void C_Frm_Prescription::Slot_evaluateAllInteractions()
{  if ( m_pC_InteractionsThread==0 )
      { m_pC_InteractionsThread = new C_InteractionsThread( m_pC_BDM_Api->pathAppli()         ,
                                                            m_pC_BDM_Api->pathIni()           ,
                                                            m_pC_BDM_Api->pathImage()         ,
                                                            m_pC_BDM_Api->patientContexte()   ,
                                                            m_pC_BDM_Api->userContexte()      ,
                                                            m_pC_BDM_Api->logWidget()         ,
                                                            this
                                                          );
        connect( m_pC_InteractionsThread ,           SIGNAL( finished () ),  this , SLOT( Slot_InteractionsThread_finished() ));
      }
   //.......... si job pas fini on attend la fin ................................................
   if ( m_pC_InteractionsThread->isRunning() ) {m_RequestInteractEval = 1; return;}
   //............ avant de demmarrer renseigner le contexte en cours ............................
   m_pC_InteractionsThread->setContext( m_pC_BDM_Api->patientContexte(),   // must be persistant while run()
                                        m_pC_BDM_Api->userContexte(),      // must be persistant while run()
                                        grammarList()                      // current patient ordonnance to be modified
                                      );
   if ( m_inWorks ==0 )
      { m_inWorks = new QLabel( m_infoTexEdit );
        m_inWorks->setGeometry(1,1,300,300);
        m_inWorks->setTextFormat(Qt::RichText);
        QString text = QString ("<table cellSpacing=\"0\"  cellpadding=\"4\" width=100% border=\"1\">"    // #FF8000
                                "<tbody>"
                                "<tr>"
                                "<td width=100% align=\"left\" bgcolor=\"#fffdeb\">")+    // work-inprogress
                                "<p align=\"center\"><span font-size:14pt;font-style:italic;font-weight:600;color:#FF0000\">"
                                "<b>EVALUATIONS EN COURS....</b><br>" +
                                QString("<img src=\"%1\" alt=\"jours\" height=\"100\" width=\"100\">").arg(Theme::getPath()+"medicatux/work-inprogress-01.png")+
                                "</span></p>"
                                "</td>"
                                "</tbody>"
                                "</table>";
        m_inWorks->setText(text);
      }
   else
   // qDebug() <<" width : "<< QString::number(m_infoTexEdit->width());
     { m_inWorks->move(m_infoTexEdit->x(),m_infoTexEdit->y()+ (m_infoTexEdit->height()/2) - 150);
       m_inWorks->resize(m_infoTexEdit->width(),300);
     }
   m_inWorks->show();

   m_pC_InteractionsThread->start();
}

//------------------------------------ Slot_InteractionsThread_finished ------------------------------------------------
void C_Frm_Prescription::Slot_InteractionsThread_finished()
{ if ( m_pC_InteractionsThread->html_result().length() )
     {  QPoint pos = QCursor::pos(); pos.setX(pos.x()+30);
        displayInfo(m_pC_InteractionsThread->html_result());
        m_inWorks->hide();
     }
  if ( m_RequestInteractEval > 0 )
     { m_RequestInteractEval = 0;
       Slot_evaluateAllInteractions();
     }
}

//------------------------------------ display_HAS_Status ------------------------------------------------
void C_Frm_Prescription::display_HAS_Status()
{if ( ! m_pC_BDM_Api->is_plugin_HAS_certified() )
    {QPoint pos = QCursor::pos(); pos.setX(pos.x()+30);
     displayInfo(m_pC_BDM_Api->HAS_NotCertifiedAlertText());
    }
}
//------------------------------------ cipList ------------------------------------------------
QStringList C_Frm_Prescription::cipList()
{ QList <C_Frm_Produit*> childsList  = C_Frm_ProduitList();
  QStringList cip_list;
  C_Frm_Produit *pC_Frm_Produit;
  for (int i = 0; i < childsList.size(); ++i)
      {   pC_Frm_Produit = childsList.at(i);
          if (pC_Frm_Produit->id().length()&&pC_Frm_Produit->id_type()=="CIP") cip_list.append(pC_Frm_Produit->id());
      }
  return cip_list;
}

//------------------------------------ grammarList ------------------------------------------------
QStringList C_Frm_Prescription::grammarList(const QString &toMatch /* = "" */, int matchWith  /*= C_Frm_Prescription::ALL_PRODUCTS */)
{
    //................. recuperer tous les fils ..................................................
    QList <C_Frm_Produit*> childsList = C_Frm_ProduitList();
    //......... trier la liste selon le numero d'ordre ...............................................
    //          avec l'astuce de la QMap
    QMap<int, C_Frm_Produit*> map;
    foreach (C_Frm_Produit* pC_Frm_Produit, childsList)
         { map.insert(pC_Frm_Produit->numOrdre().toInt(), pC_Frm_Produit);
         }
    //......... recuperer la liste triee et l'exploiter.............................................................
    childsList = map.values();

    QStringList grammar_list;
    C_Frm_Produit *pC_Frm_Produit;
    for (int i = 0; i < childsList.size(); ++i)
      {   pC_Frm_Produit  = childsList.at(i);
          QString grammar = pC_Frm_Produit->grammar();
          if (grammar.length())
             { switch (matchWith)
                      {
                       case C_Frm_Prescription::ALL_PRODUCTS:                                                  grammar_list << grammar; break;
                       case C_Frm_Prescription::MATCH_ALD_STATUS:   if (toMatch == pC_Frm_Produit->ald())      grammar_list << grammar; break;
                       case C_Frm_Prescription::CIP_PRODUCTS:       if (toMatch == pC_Frm_Produit->id_type())  grammar_list << grammar; break;
                      }
             }
      }
    return grammar_list;
}

//------------------------------------ objectStartsMoving ------------------------------------------------
void C_Frm_Prescription::objectStartsMoving(C_Frm_Produit *pC_Frm_Produit)
{int  pos_y                     = adjust_Vertical_PositionToGrid(pC_Frm_Produit);
 m_startDragPos                 = QPoint(pC_Frm_Produit->x(), pos_y);
 m_lastDragPosY                 = pos_y;
 m_HitList_deployedBeforeMoving = get_HitListDeployed();
 close_All_HitList(1);
 pC_Frm_Produit->raise ();    // mettre l'objet en avant plan
}
//------------------------------------ objectDoMoving ------------------------------------------------
void C_Frm_Prescription::objectDoMoving(C_Frm_Produit *pC_Frm_Produit)
{int pos_y = adjust_Vertical_PositionToGrid(pC_Frm_Produit, true);
 if (pos_y == m_lastDragPosY) return;
 //............. combler le trou laisse par le deplacement en .............................
 //              deplacant les objets sous jacents vers le haut
 int  nb_ald  = count_ALD();
 bool to_move = true;        // a priori il faut deplacer tous les objets sous jacents vers le haut pour combler la place libre de celui a deplacer
 if ( m_lastDragPosY == adjust_Vertical_PositionToGrid(FIRST_OBJ_POS_Y) && nb_ald<=1) to_move = false; // sauf si il est seul sur la premiere case ALD (la laisser alors vide)
 if (to_move) deplace_Objects( m_lastDragPosY + m_Grid_H,  // on deplace tout ce qui se situe apres (+ m_Grid_H) notre objet.
                                               -m_Grid_H); // d'une hauteur de grille vers le haut
 m_lastDragPosY = pos_y;          // noter le deplacement vertical
 //............. faire le trou de la nouvelle place .............................
 //              deplacant les objets sous jacents vers le bas
 int  final_Y = pC_Frm_Produit->y();                                 // on recupere la place de l'objet avant qu'elle ne bouge lors des deplacements ulterieurs
 to_move = true;
 //.......... detecter si relache sur la premiere case ALD vide ............................
 //           auquel cas ne pas deplacer les sous jacents vers le bas
 QList <C_Frm_Produit*> excludedObjects;
 excludedObjects.append(pC_Frm_Produit);          // ne pas tenir compte de celui en cours (car on teste ce qu'il y a AVANT son larguage)
 if ( pos_y == adjust_Vertical_PositionToGrid(FIRST_OBJ_POS_Y) )                                                                     // si relache sur premiere case (case ALD) verifier si vide
    { QList <C_Frm_Produit*> listAldObjects = objectListAt(pos_y, C_Frm_Prescription::JUST_ONE_POSITION_UNDER_Y, &excludedObjects);  // liste des objets deja pr\303\251sents sur cette case (exclusion de celui en cours de deplacement)
      if (listAldObjects.count() == 0) to_move = false;
    }
 //.......... taguer le caractere ALD ou non de l'objet ...............
 //           selon sa zone d'aterrissage
 if (is_posY_in_ALD_Zone()) pC_Frm_Produit->set_ald("ALD");
 else                       pC_Frm_Produit->set_ald("");

 //........... deplacer les sous jacents vers le bas ......................
 if (to_move) deplace_Objects( pos_y  ,  m_Grid_H);                 // on deplace tout ce qui se situe apres ca d'une hauteur de grille vers le bas

 //............ ne pas depasser la derniere position basse possible .......................
 pC_Frm_Produit->move(pC_Frm_Produit->x(), final_Y);

 rearangeAllNumberOrder();  // marche pas en temps reel si deplacement vers le bas !!!
 update();
}
//------------------------------------ objectStopsMoving ------------------------------------------------
void C_Frm_Prescription::objectStopsMoving(C_Frm_Produit *pC_Frm_Produit)
{//............ ajuster a la derniere position basse possible .......................
 int nb          = count_GridToDraw() - 1 ;
 int pos_Y_max   = nb * m_Grid_H + FIRST_OBJ_POS_Y + m_SpaceBetweenProduits/2;
 if ( pC_Frm_Produit->y() > pos_Y_max)                       // si depassement on ajuste
    { pC_Frm_Produit->move(pC_Frm_Produit->x(),pos_Y_max );
    }
 if (m_HitList_deployedBeforeMoving)  m_HitList_deployedBeforeMoving->setToDeployRect();
 m_HitList_deployedBeforeMoving = 0;
 rearangeAllNumberOrder();
 update();
}

//------------------------------------ C_Frm_ProduitList ------------------------------------------------
QList <C_Frm_Produit*> C_Frm_Prescription::C_Frm_ProduitList()
{ QList <QObject*>        objectList = this->children();
  QList <C_Frm_Produit*>  childsList;
  QObject *obj = 0;
  for (int i = 0; i < objectList.size(); ++i)
      {obj = objectList.at(i);
       if (obj->objectName()=="C_Frm_Produit") childsList.append((C_Frm_Produit*)obj);
      }
  return  childsList;
}

//------------------------------------ echoListObjects ------------------------------------------------
void C_Frm_Prescription::echoListObjects(const QString txt)
{   QList <C_Frm_Produit*> childsList = C_Frm_ProduitList();
    qDebug()<<tr("============== LIST OBJECTS nb_objects:%1 %2=============").arg(QString::number(childsList.size()),txt);
    for (int i = 0; i < childsList.size(); ++i)
        {C_Frm_Produit *pC_Frm_Produit = childsList.at(i);
         qDebug()<<tr("* top>> %3|=|%4 bottom<< OBJECT:%1 num:%2 ").arg(pC_Frm_Produit->commercialName(),pC_Frm_Produit->numOrdre(),
                                                                 QString::number(pC_Frm_Produit->y()),
                                                                 QString::number(pC_Frm_Produit->y()+pC_Frm_Produit->height()));
        }
}

//------------------------------------ rearangeNumberOrder ------------------------------------------------
void C_Frm_Prescription::rearangeAllNumberOrder(QList<C_Frm_Produit*> *pListProduitExcluded  /* = 0 */)
{   //............... on cree la liste des objets sous jacents a y_ref .................
    int nb_Prd = 0;
    int nb     = 0;
    int nb_Ald = count_ALD(&nb_Prd );             // relever le nombre de produits
    int  pos_y = FIRST_OBJ_POS_Y + m_Grid_H/2;    // partir du debut de la grille
    int last_y = 0;
    int  end_y = (nb_Ald+nb_Prd+1) * m_Grid_H;    // calculer la fin des cases
    //qDebug()<<tr("==================== %1 ========================").arg(QString::number(nb_Ald+nb_Prd+1));

    while(pos_y < end_y)
      { //.......... on examine tous les objets fils (sauf ceux exclus) .....................
        //           et on les stocke si concernes par la position pos_y
        QList <C_Frm_Produit*> childsList = C_Frm_ProduitList();
        //qDebug()<<tr("---------- pos_y:%1 end_y:%2 nb_objects:%3---------------").arg(QString::number(pos_y),QString::number(end_y),QString::number(childsList.size()));
        for (int i = 0; i < childsList.size(); ++i)
            {C_Frm_Produit *pC_Frm_Produit = childsList.at(i);
             //............. si objet exclu ne pas le traiter ...............
             if (pListProduitExcluded && pListProduitExcluded->contains(pC_Frm_Produit) )
                {continue;      // objet exclu passer au suivant
                }
             //qDebug()<<tr("* OBJECT:%1 num:%2 top:%3 bottom:%4").arg(pC_Frm_Produit->commercialName(),pC_Frm_Produit->numOrdre(),
             //                                                        QString::number(pC_Frm_Produit->y()),
             //                                                        QString::number(pC_Frm_Produit->y()+pC_Frm_Produit->height()));
             if ( pos_y >= pC_Frm_Produit->y() && pos_y <= pC_Frm_Produit->y() + pC_Frm_Produit->height() )
                { if ( pos_y != last_y)
                     { ++nb;
                       last_y = pos_y;
                     }
                 //qDebug()<<tr(".   before num:%2 top:%3 bottom:%4").arg(pC_Frm_Produit->numOrdre(),
                 //                                                                 QString::number(pC_Frm_Produit->y()),
                 //                                                                 QString::number(pC_Frm_Produit->y()+pC_Frm_Produit->height()));
                 pC_Frm_Produit->setOrderNumber(nb);
                 if (pC_Frm_Produit->m_pC_Frm_Posologie) pC_Frm_Produit->m_pC_Frm_Posologie->updateNumOrdreInHtmlView( QString::number(nb) );
                 //qDebug()<<tr(".   after  num:%2 top:%3 bottom:%4").arg(pC_Frm_Produit->numOrdre(),
                 //                                                                 QString::number(pC_Frm_Produit->y()),
                 //                                                                 QString::number(pC_Frm_Produit->y()+pC_Frm_Produit->height()));
                }
            }
        pos_y +=  m_Grid_H;
      }
}

//------------------------------------ objectListAt ------------------------------------------------
/*! \brief return objects list under y_ref to y_end positions
 *  \param  y_ref start Y position.
 *  \param  y_end end   Y position. if -1 then alls positions below y_ref, if zero just one position under y_ref
 *  \param  QList<C_Frm_Produit*> *pListProduitExcluded if not zero this objects from this list will be excluded from result list.
 *  \return QList <C_Frm_Produit*>  result list
 */
QList <C_Frm_Produit*> C_Frm_Prescription::objectListAt(int y_ref, int y_end /*= 0*/, QList<C_Frm_Produit*> *pListProduitExcluded  /* = 0 */)
{   //............... on cree la liste des objets sous jacents a y_ref .................
    int nb_Prd = 0;
    int nb_Ald = count_ALD(&nb_Prd );                                // relever le nombre de produits
    int  pos_y = y_ref + m_Grid_H/2;                                 // partir du milieu de la grille
    int  end_y = y_end;
    if      (end_y ==-1)  end_y = (nb_Ald+nb_Prd+1) * m_Grid_H;   // calculer la fin des cases
    else if (end_y == 0)  end_y = pos_y+1;                        // juste position sous la souris
    QList <C_Frm_Produit*> C_Frm_ProduitList;
    while(pos_y < end_y)
      { //.......... on examine tous les objets fils (sauf ceux exclus) .....................
        //           et on les stocke si concernes par la position pos_y
        QList <C_Frm_Produit*> childsList = this->C_Frm_ProduitList();
        for (int i = 0; i < childsList.size(); ++i)
            {C_Frm_Produit *pC_Frm_Produit = childsList.at(i);
             //............. si objet exclu ne pas l'ajouter ...............
             if (pListProduitExcluded && pListProduitExcluded->contains(pC_Frm_Produit) )
                {continue;      // objet exclu passer au suivant
                }
             if ( pos_y >= pC_Frm_Produit->y() && pos_y <= pC_Frm_Produit->y() + pC_Frm_Produit->height() )
                {C_Frm_ProduitList.append(pC_Frm_Produit);
                }
            }
        pos_y +=  m_Grid_H;
      }
    return C_Frm_ProduitList;
}

//------------------------------------ deplace_Objects ------------------------------------------------
void C_Frm_Prescription::deplace_Objects(int y_ref, int ofset_y)
{   C_Frm_Produit          *pC_Frm_Produit   = 0;
    //................. recuperer la liste des objets sous jacents ..................................
    QList <C_Frm_Produit*> C_Frm_ProduitList = objectListAt( y_ref, C_Frm_Prescription::ALL_POSITIONS_BELOW_Y); // -1 pour toutes les position
    //............... on deplace les objets de la liste .................
    for (int i = 0; i < C_Frm_ProduitList.size(); ++i)
        {   pC_Frm_Produit = C_Frm_ProduitList.at(i);
            pC_Frm_Produit->move(pC_Frm_Produit->x(),pC_Frm_Produit->y()+ofset_y);
        }
}

//------------------------------------ is_posY_in_ALD_Zone -----------------------------------------------
bool C_Frm_Prescription::is_posY_in_ALD_Zone()
{QPoint pos = mapFromGlobal ( QCursor::pos() );
 return is_posY_in_ALD_Zone( pos.y() );
}
//------------------------------------ is_posY_in_ALD_Zone -----------------------------------------------
bool C_Frm_Prescription::is_posY_in_ALD_Zone(int y)
{int nb_Ald = qMax(1,count_ALD());
 if (y<nb_Ald*m_Grid_H+FIRST_OBJ_POS_Y && y>0)  return true;
 return false;
}

//------------------------------------ adjust_Vertical_PositionToGrid -----------------------------------------------
int C_Frm_Prescription::adjust_Vertical_PositionToGrid(C_Frm_Produit *pC_Frm_Produit /* = 0 */, bool scroll_if_out_limits /*=false */)
{ QPoint pos = mapFromGlobal ( QCursor::pos() );
  if (scroll_if_out_limits)
     {m_pQScrollArea->ensureVisible(10,pos.y());
     }
  return adjust_Vertical_PositionToGrid(pos.y() , pC_Frm_Produit );
}

//------------------------------------ adjust_Vertical_PositionToGrid -----------------------------------------------
int C_Frm_Prescription::adjust_Vertical_PositionToGrid(int y, C_Frm_Produit *pC_Frm_Produit /* =0 */)
{y -=  FIRST_OBJ_POS_Y;
 y  =  y / m_Grid_H;
 y *=  m_Grid_H;
 y +=  m_SpaceBetweenProduits/2;
 if (pC_Frm_Produit)
    {pC_Frm_Produit->move(pC_Frm_Produit->x(), y + FIRST_OBJ_POS_Y);
    }
 return y + FIRST_OBJ_POS_Y;
}

//------------------------------------ count_ALD -----------------------------------------------
int C_Frm_Prescription::count_ALD(int *nb_Prd /* = 0 */)
{   QList <C_Frm_Produit*> childsList  = C_Frm_ProduitList();
    int nb_Ald                         = 0;
    C_Frm_Produit *pC_Frm_Produit;
    for (int i = 0; i < childsList.size(); ++i)
        {   pC_Frm_Produit = childsList.at(i);
            if (pC_Frm_Produit->ald().length()) ++nb_Ald;
        }
    if (nb_Prd) *nb_Prd = childsList.size()-nb_Ald;
    return nb_Ald;
}

//------------------------------------ count_GridToDraw ------------------------------------------------
int C_Frm_Prescription::count_GridToDraw(int *nb_ald_toRet /* = 0 */)
{int nb_Prd;
 int nb_Ald = qMax(1,count_ALD(&nb_Prd )); // au moins une case ALD vide
     nb_Prd = qMax(1,nb_Prd);              // au moins une case non ALD vide
 if (nb_ald_toRet) *nb_ald_toRet = nb_Ald;
 return       nb_Prd + nb_Ald;
}

//------------------------------------ paintEvent -----------------------------------------------
void  C_Frm_Prescription::paintEvent ( QPaintEvent * /* event */ )
{
QPainter p(this);
QFont  fGrd  = font();
int w        = width();
int ofs_y    = FIRST_OBJ_POS_Y ;
int h_pixFnt = qMin(m_Grid_H-m_Grid_H/4,30);

QColor grad_color   = palette ().color(QPalette::Dark);
QFontMetrics fntMetGrd(fGrd);
QRect        boundRect;
QString      text;
int          h_txt;
int          x_txt = FIRST_OBJ_POS_X+2;

//............. dessiner les cases ....................
int nb_Ald = 0;
int nb     = count_GridToDraw(&nb_Ald);
int y_end  = m_Nb_Grid_H  * m_Grid_H + ofs_y; // version minimale de deux lignes nb * m_Grid_H + ofs_y;
int pos_y  = ofs_y;
while (pos_y <= y_end)
      {  p.setPen   ( grad_color);
         p.drawLine ( 0, pos_y, w, pos_y);
         pos_y += m_Grid_H;
      }  //endfor (int y = 0; y < y_der; y += 15)

//......... tracer le texte de la premiere case ALD .....................
fGrd.setPixelSize(h_pixFnt); fGrd.setBold(true); fGrd.setItalic(true); p.setFont ( fGrd );
fntMetGrd = QFontMetrics(fGrd);

text      = tr("zone ALD");
boundRect = fntMetGrd.boundingRect ( text );
h_txt     = boundRect.height();

pos_y = ofs_y +h_txt +(m_Grid_H-h_txt)/2;
p.setPen (grad_color.darker(160));
p.drawText ( QPoint(x_txt,  pos_y),   text );
p.setPen (grad_color.darker(90));
//p.setPen (grad_color.lighter ( 150 ));
p.drawText ( QPoint(x_txt+2,  pos_y+2), text );
fGrd.setPixelSize(11); p.setFont ( fGrd );
p.setPen (grad_color.darker(160));
p.drawText ( QPoint(x_txt+2+boundRect.width(),  pos_y ), tr("  (d\303\251posez les m\303\251dicaments ALD ici)") );

//............ se deplacer a la derniere case ..............................
int  nb_Prd;
count_ALD( &nb_Prd );
nb = nb_Prd+nb_Ald;
pos_y = qMax(1,nb)*m_Grid_H+ofs_y  +h_txt+(m_Grid_H-h_txt)/2;


//......... tracer le texte de la derniere case non ALD .....................
fGrd.setPixelSize(h_pixFnt); fGrd.setBold(true); fGrd.setItalic(true); p.setFont ( fGrd );
text      = tr("hors ALD");
boundRect = fntMetGrd.boundingRect ( text );
h_txt     = boundRect.height();

p.setPen (grad_color.darker(160));
p.drawText ( QPoint(x_txt,  pos_y), text );
p.setPen (grad_color.darker(90));
//p.setPen (grad_color.lighter ( 150 ));
p.drawText ( QPoint(x_txt+2,  pos_y+2), text );
fGrd.setPixelSize(11); p.setFont ( fGrd );
p.setPen (grad_color.darker(160));
p.drawText ( QPoint(x_txt+2+boundRect.width(),  pos_y ), tr("  (d\303\251posez les m\303\251dicaments hors ALD ici)") );
}

//------------------------ Slot_ButtonNumberClicked ---------------------------------------
void C_Frm_Prescription::Slot_ButtonNumberClicked(const QString &, void *ptr)
{emit Sign_ProductNumberClicked((C_Frm_Produit*)ptr);
}
//------------------------ Slot_FavorisAdded ---------------------------------------
void C_Frm_Prescription::Slot_FavorisAdded(C_Frm_Produit *pC_Frm_Produit)
{emit Sign_FavorisAdded(pC_Frm_Produit);      // mettre a jour la liste des favoris dans l'interface prineale
}

//------------------------ Slot_ButtonDeleteClicked ---------------------------------------
void C_Frm_Prescription::Slot_ButtonDeleteClicked(const QString&, void *ptr)
{   if (ptr==0) return;
    C_Frm_Produit *pC_Frm_Produit = (C_Frm_Produit*)ptr;
    int  pos_y   = adjust_Vertical_PositionToGrid(pC_Frm_Produit->y()); // ajuster position d'aterrissage a la case de la grille
    bool to_move = true;                                                // a priori faut deplacer les sous jacents vers le bas pour faire la place
    //.......... detecter si effacement sur la premiere case ............................
    //           auquel cas ne pas deplacer les sous jacents vers le haut
    //           car cette case doit rester vide
    if ( pos_y == adjust_Vertical_PositionToGrid(FIRST_OBJ_POS_Y) && count_ALD()==1)  to_move = false;
    //........... deplacer les sous jacents vers le bas ......................
    if (to_move) deplace_Objects( pos_y  ,                  // on deplace tout ce qui se situe apres ca
                                 -m_Grid_H);                // d'une hauteur de grille vers le haut
    pC_Frm_Produit->deleteLater();
    pC_Frm_Produit->setToDeleteLater();
    QList <C_Frm_Produit*> excludedObjects;
    excludedObjects.append(pC_Frm_Produit);   // ne pas tenir compte de celui en cours (car pas encore delete le sera qu'apres retour event)
    rearangeAllNumberOrder(&excludedObjects);
    update();
    Slot_contentChanged( );
    QTimer::singleShot(10, this, SLOT(Slot_evaluateAllInteractions()));
}

//------------------------ serialize ---------------------------------------
QString C_Frm_Prescription::serialize(const QString &ofset /* = "" */)
{   QString xml = "";
    QList <C_Frm_Produit*> childsList  = C_Frm_ProduitList();

    C_Frm_Produit *pC_Frm_Produit;
    for (int i = 0; i < childsList.size(); ++i)
        {   pC_Frm_Produit = childsList.at(i);
            xml += pC_Frm_Produit->serialize(ofset);
        }
    return xml;
}
//------------------------ delete_All_Products ---------------------------------------
void C_Frm_Prescription::delete_All_Products(int interactionsMustBeeEvaluated /* = C_Frm_Prescription::EVALUATE_INTERACTIONS */)
{
    QList <C_Frm_Produit*> childsList  = C_Frm_ProduitList();
    if (childsList.size()==0) return;
    C_Frm_Produit *pC_Frm_Produit;
    for (int i = 0; i < childsList.size(); ++i)
        {   pC_Frm_Produit = childsList.at(i);
            pC_Frm_Produit->deleteLater();
            pC_Frm_Produit->setToDeleteLater();
        }
    update();
    Slot_contentChanged( );
    if ( interactionsMustBeeEvaluated == C_Frm_Prescription::EVALUATE_INTERACTIONS ) QTimer::singleShot(10, this, SLOT(Slot_evaluateAllInteractions()));
}

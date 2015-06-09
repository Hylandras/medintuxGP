 /********************************* C_Dlg_Agenda.h ********************************
 *  #include "C_Dlg_Agenda.h"                                                     *
 * ...............................................................................*
 *   Project   :  MedinTux  (typographie made in france)                          *
 *   Copyright : (C) 2004-2005-2006-2007-2008-2009-2010 and for the eternity      *
 *   by        :  Sevin Roland     from   MedinTux project                        *
 *   E-Mail    : roland-sevin@medintux.org                                        *
 *   Web site  : www.medintux.org                                                 *
 * ...............................................................................*
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat �  l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Fédération, 75752 PARIS cedex 15.   *
 *                            FRANCE                                              *
 *   Centre National de la Recherche Scientifique                                 *
 *   - CNRS,                                                                      *
 *                            3 rue Michel-Ange, 75794 Paris cedex 16.            *
 *                            FRANCE                                              *
 *   Institut National de Recherche en Informatique et en Automatique             *
 *   - INRIA,                                                                     *
 *                            Domaine de Voluceau, Rocquencourt, BP 105, 78153    *
 *                            FRANCE                                              *
 *                                                                                *
 *         This program is distributed in the hope that it will be useful,        *
 *         but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                   *
 *         CeCILL  License (GPL compatible) for more details.                     *
 *                                                                                *
 **********************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QDate>
#include <QDateTime>
#include <QList>
#include <QLabel>
#include <QPushButton>
#include <QSize>
#include <QFrame>
#include <QList>
#include <QMouseEvent>
#include <QPoint>
#include <QPaintEvent>
#include <QPainter>
#include <QRubberBand>
#include <QTextEdit>
#include <QPalette>
#include <QToolTip>
#include <QFont>
#include <QColor>
#include <QBoxLayout>
#include <QMenu>
#include <QtWebKit/QWebView>
#include <QTimer>
#include <QTextEdit>
#include <QtGlobal>
#include <QMap>
#include <QAnimationGroup>
#include <QPropertyAnimation>
//...... definie si agenda non stand alone .......
#ifndef AGENDA_IN_GUI
  // #include "../../src/ui/ui_Dlg_MainWindow.h"
#endif

#include "CMoteurAgenda.h"
#include "C_RendezVous.h"
#include "../../MedinTuxTools-QT4/Theme/Theme.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/C_GoogleAPI/C_GoogleAPI.h"

#include "c_dlg_plagesdispo.h"                  // CZA
#include "ui_c_dlg_plagesdispo.h"               // CZA
#include "C_Dlg_ChercheRDV.h"                   // CZE 2
#include "ui_C_Dlg_ChercheRDV.h"                // CZE 2

#define SIZE_BORDER_DAY            5         // largeur de la zone sensible au niveau des bordure pour etirement rendez-vous
#define DAY_OFS_X                 20         // ofset affichage dans le jour replie
#define W_OFSET                    0         // ofset lateral affichage du jour
#define W_RDV_OFSET                35        // ofset lateral affichage du rendez vous

#define DAY_HEIGHT                 m_pCMoteurAgenda->GetBaseDaySimpleHeight() //45        // 45 hauteur totale d'un jour
#define DOUBLE_DAY_HEIGHT          m_pCMoteurAgenda->GetBaseDayDoubleHeight() // 90 //75

#define LINE_RESUME_HEIGHT         24        // 20 hauteur totale d'une ligne de resume
#define LINE_RESUME_XMARG          10         // marge d'affichage afin d'eviter que les rectangles debordent

#define FIRST_DAY_POS_Y            m_pCMoteurAgenda->GetOfsetGraduation()         // ofset vertical d'affichage de la zone des widgets de rendez vous
#define NB_DAYS_TO_SEE             31        // nombre de jours par defaut a afficher
#define BEFORE_DAYS                8         // nombre de jours par defaut a afficher avant
#define HEAD_RESUME_OFY            5         // ofset de debut d'affichage de la zone de resume
namespace Ui
{
    class MainWindowClass;
}
//====================================== C_BitMapCollection =======================================================
class C_BitMapCollection
 {

 public:
     C_BitMapCollection(const QString & pathImage,int dayWidth, int dayHeight)
     {m_HeadOpenDay_Pixmap        = QPixmap(pathImage + "HeadOpenDay.png").scaled(dayWidth, dayHeight);
      m_HeadCloseDay_Pixmap       = QPixmap(pathImage + "HeadCloseDay.png").scaled(dayWidth,dayHeight);
      m_HeadSatDay_Pixmap         = QPixmap(pathImage + "HeadSatDay.png").scaled(dayWidth,  dayHeight);
      m_HeadSunDay_Pixmap         = QPixmap(pathImage + "HeadSunDay.png").scaled(dayWidth,  dayHeight);
      m_HeadUnWorkDay             = QPixmap(pathImage + "HeadUnWorkDay.png").scaled(dayWidth,  dayHeight);
      m_HeadResume_Pixmap         = QPixmap(pathImage + "Head_Resume.png").scaled(dayWidth-21, LINE_RESUME_HEIGHT);

      m_ButtonExpand_Pixmap       = QPixmap(pathImage + "Left.png");
      m_ButtonNewRDV_Pixmap       = QPixmap(pathImage + "NewDoc.png");
      m_ButtonSave_Pixmap         = QPixmap(pathImage + "save.png");
      m_ButtonExpand_Close_Pixmap = QPixmap(pathImage + "Down.png"); // icone pour le jour ouvert
      m_ButtonExpand_Open_Pixmap  = QPixmap(pathImage + "Left.png"); // icone pour le jour ferme
      m_AgendaHourIndic_Pixmap    = QPixmap(pathImage + "AgendaHourIndic.png");
      m_AgendaTimeArrow_Pixmap    = QPixmap(pathImage + "timeArrow.png") ;
      m_ButtonAcceder_Pixmap      = QPixmap(pathImage + "AgendaLancerDoss.png");
      m_Copier_Pixmap             = QPixmap(pathImage + "copier.png");
      m_Paste_Pixmap              = QPixmap(pathImage + "editpaste.png");
      m_Cut_Pixmap                = QPixmap(pathImage + "Couper.png");
      m_RdvCancel_Pixmap          = QPixmap(pathImage + "Rdv_annule.png");
      m_Configure_Pixmap          = QPixmap(pathImage + "configure.png");
      m_IdentityDelete_Pixmap     = QPixmap(pathImage + "identityDelete.png");
      m_QuitterMenu_Pixmap        = QPixmap(pathImage + "QuitterMenu.png");
      m_ButtonDelete_Pixmap       = QPixmap(pathImage + "Rdv_annule.png");
      m_ButtonChange_Pixmap       = QPixmap(pathImage + "AgendaModif.png");
      m_ButtonCreateDoss          = QPixmap(pathImage + "AgendaCreateDoss.png");
      m_MenuRendezvousDel         = QPixmap(pathImage + "RendezvousDel.png");
      m_ButtonGoogle_Pixmap       = QPixmap(pathImage + "Google.png");
      m_ButtonGotoWeek_Pixmap     = QPixmap(pathImage + "gotoWeek.png");
      m_GetPatientRdv             = QPixmap(pathImage + "GetPatientRdv.png");
      m_PrintListRdv              = QPixmap(pathImage + "PrintListRdv.png");
      QStringList list  = CGestIni::listDirectory(pathImage + "Statuts", ".png");
      for (int i = 0; i < list.size(); ++i)
          {QString file   = pathImage + "Statuts/" + list[i];
           QString statut = QFileInfo(file).baseName();
           m_StatutsPixmap.insert(statut, QPixmap(file));
          }
     }
     void resizeBitMapToDayHeight(int dayWidth, int dayHeight)
     {   m_HeadOpenDay_Pixmap        = m_HeadOpenDay_Pixmap.scaled(dayWidth,   dayHeight);
         m_HeadCloseDay_Pixmap       = m_HeadCloseDay_Pixmap.scaled(dayWidth,  dayHeight);
         m_HeadSatDay_Pixmap         = m_HeadSatDay_Pixmap.scaled(dayWidth,    dayHeight);
         m_HeadSunDay_Pixmap         = m_HeadSunDay_Pixmap.scaled(dayWidth,    dayHeight);
         m_HeadUnWorkDay             = m_HeadUnWorkDay.scaled(dayWidth,        dayHeight);
         m_HeadResume_Pixmap         = m_HeadResume_Pixmap.scaled(dayWidth-21, LINE_RESUME_HEIGHT);
     }

     QMap <QString , QPixmap> m_StatutsPixmap;
     QPixmap m_HeadOpenDay_Pixmap;
     QPixmap m_HeadCloseDay_Pixmap;
     QPixmap m_HeadSatDay_Pixmap;
     QPixmap m_HeadSunDay_Pixmap;
     QPixmap m_HeadResume_Pixmap;
     QPixmap m_HeadUnWorkDay;

     QPixmap m_ButtonExpand_Pixmap;
     QPixmap m_ButtonNewRDV_Pixmap;
     QPixmap m_ButtonSave_Pixmap;
     QPixmap m_ButtonExpand_Close_Pixmap; // icone pour le jour ouvert
     QPixmap m_ButtonExpand_Open_Pixmap; // icone pour le jour ferme
     QPixmap m_AgendaHourIndic_Pixmap;
     QPixmap m_AgendaTimeArrow_Pixmap;
     QPixmap m_ButtonAcceder_Pixmap;
     QPixmap m_Copier_Pixmap;
     QPixmap m_Paste_Pixmap;
     QPixmap m_RdvCancel_Pixmap;
     QPixmap m_Replace_Pixmap;
     QPixmap m_Cut_Pixmap;
     QPixmap m_Configure_Pixmap;
     QPixmap m_IdentityDelete_Pixmap;
     QPixmap m_QuitterMenu_Pixmap;
     QPixmap m_ButtonDelete_Pixmap;
     QPixmap m_ButtonChange_Pixmap;
     QPixmap m_ButtonCreateDoss;
     QPixmap m_MenuRendezvousDel;
     QPixmap m_ButtonGoogle_Pixmap;
     QPixmap m_ButtonGotoWeek_Pixmap;
     QPixmap m_GetPatientRdv;
     QPixmap m_PrintListRdv;

 };

//====================================== C_QMenuRdv =======================================================
// ce menu affiche en haut un widget QLabel permettant d'afficher des infos, notamment specifiques aux
// rendez-vous avec une case a gauche avec le nom du patient, et une a droite coloree du type de rendez-vous
// il permet aussi d'afficher un texte simple
// pour disposer de ces proprietes, l'appel doit obligatoirement se faire avec un QWidget *parent valide (pas a zero)
class C_QMenuRdv : public QMenu
 {
     Q_OBJECT
 public:
  //---------------------------- C_QMenuRdv() ------------------------------------------------
  C_QMenuRdv(const QString &title, QWidget *parent):QMenu(title, parent)
    {initTextTools(parent, title);
    }
  //---------------------------- C_QMenuRdv() ------------------------------------------------
  C_QMenuRdv(QWidget *parent):QMenu(parent)
     {initTextTools(parent, "");
     }
  //---------------------------- ~C_QMenuRdv() ------------------------------------------------
  ~C_QMenuRdv()
     {if (m_Title && m_QLabelIsToDelete) delete m_Title;
     }

  //---------------------------- initTextTools ------------------------------------------------
  // juste pour ajuster le widget a la largeur du texte
  void initTextTools(QWidget *parent, const QString &title)
  {setStyleSheet("font-size: 12px");
   m_Title            = 0;
   m_QLabelIsToDelete = 0;
   if (parent)
      {m_Title         = new QLabel (parent);
       m_Title->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
       m_Title->setWindowFlags(Qt::ToolTip);  //Qt::Popup
       if (title.length()) setLabelText(title);
       else                m_Title->setText("");
       m_QLabelIsToDelete = 1;
      }
   m_isLabelUpdate = 0;
  }

  //---------------------------- paintEvent ------------------------------------------------
  // juste pour ajuster le widget a la largeur du texte
  void paintEvent ( QPaintEvent * event )
         {//QPainter p(this);
          if (m_Title && m_isLabelUpdate==0)  // recuperer la largeur du menu pour la placer sur le label
             {m_Title->setGeometry(x(),y()-22, width(), 17);
              m_Title->move(x(),y()-m_Title->height()-3);
              m_Title->show();
              m_isLabelUpdate = 1;
             }
          QMenu::paintEvent(event);
         }
  //---------------------------- setLabelText ------------------------------------------------
  void setLabelText(const QString &txt)
    {if (m_Title==0) return;
     if (txt.contains("</TABLE>"))
        {m_Title->setText(txt);
        }
     else
        {m_Title->setText(QString( "<TABLE cellSpacing=\"0\"  cellpadding=\"0\" width=100% border=\"1\">"
                                   "<TBODY>"
                                   "<TR>"
                                   "<TD width=100% align=\"center\" bgcolor=\"#FFFFC0\">"
                                   "%1"
                                   "</TD><"
                                   "</TR>"
                                   "</TBODY>"
                                   "</TABLE>").arg(txt)
                         );
        }
    }

  //---------------------------- setLabelRdvText ------------------------------------------------
  void setLabelRdvText(const QString &txtCase1, const QString &txtCase2, const QString &colorCase2)
    { if (m_Title==0) return;
      m_Title->setText(QString("<TABLE cellSpacing=\"0\"  cellpadding=\"0\" width=100% border=\"1\">"
                        "<TBODY>"
                        "<TR>"
                        "<TD width=80% align=\"center\" bgcolor=\"#FFFFC0\">"
                        "%1"
                        "</TD><"
                        "TD width=20% align=\"center\" bgcolor=\"%3\">%2"
                        "</TD>"
                        "</TR>"
                        "</TBODY>"
                        "</TABLE>").arg(txtCase1, txtCase2, colorCase2)
                       );
    }

//---------------------------- setLabelText ------------------------------------------------
void setLabel(QLabel *pQLabel)
  { if (m_QLabelIsToDelete) delete m_Title;
    m_QLabelIsToDelete = 0;
    m_Title            = pQLabel;
  }

 QLabel *m_Title;
 int m_isLabelUpdate;
 int m_QLabelIsToDelete;
};

class QTextEdit;
//====================================== C_AgendaEdit =======================================================
class C_AgendaEdit : public QTextEdit
 {
     Q_OBJECT

 public:
     C_AgendaEdit(QWidget *parent = 0, const QString backColor = "#c3c3c3")
             :QTextEdit(parent), m_backColor(backColor)
     {
     }
     void contextMenuEvent ( QContextMenuEvent * event )
         { QMenu *menu = createStandardContextMenu();
           //menu->addAction(tr("My Menu Item"));
           menu->setStyleSheet(QString("border: 1px solid #8f8f91; border-radius: 6px; font-size: 9px; background-color: #f1f1f1"));   // "background-color: #f1f1f1; font-size: 9px;"
           menu->exec(event->globalPos());
           //delete menu;
         }
     void setMenuBackgroundColor(const QString &color){m_backColor = color;}
 signals:
     void Sign_focusInEvent (QFocusEvent *);
     void Sign_focusOutEvent(QFocusEvent *);
 protected:
     void focusInEvent  ( QFocusEvent * event )
         {    emit Sign_focusInEvent(event);
              QTextEdit::focusInEvent(event);
         }
     void focusOutEvent ( QFocusEvent * event )
         {    emit Sign_focusOutEvent(event);
              QTextEdit::focusOutEvent(event);
         }
     QString m_backColor;
 };

//====================================== C_RdvHeureButton =======================================================
class C_RdvHeureButton : public QPushButton
{
Q_OBJECT
//........................................... METHODES ...........................................................
public:
     C_RdvHeureButton (QWidget*         parent           = 0)
         : QPushButton (parent)
     {
     }
     //------------------------ paintEvent ---------------------------------------
void paintEvent ( QPaintEvent * event )
    { QPushButton::paintEvent(event);
      QPainter  p(this);
      QFont   f  = font();
      f.setPointSize(7); f.setBold(TRUE); p.setFont (f);
      p.setPen (Qt::black);
      p.drawText ( QPoint(5,9), m_text );

    }
void    setText ( const QString & text ){m_text = text;update();}
QString text () const {return m_text;}
private:
QString m_text;
};

//====================================== CMyButton =======================================================
class CMyButton : public QPushButton
{
Q_OBJECT
//........................................... METHODES ...........................................................
public:
     CMyButton (QWidget*         parent           = 0,
                const char*      name             = 0,
                void*            ptr              = 0

           )
         : QPushButton (name , parent)
     {
         setText(name);
         m_ptr  = ptr;
         m_Name = name;
         connect( this, SIGNAL( clicked() ), this, SLOT( Slot_OnButtonClicked() ) );
     }
     CMyButton (const QIcon    & icon,
                QWidget*         parent           = 0,
                const QString&   name             = 0,
                void*            ptr              = 0
               )
         : QPushButton (icon, name, parent)   // important permet que le widget soit enfant du parent
     {setText(name);
      m_ptr  = ptr;
      m_Name = name;
      setIconSize ( QSize(16,16) );
      connect( this, SIGNAL( clicked() ), this, SLOT( Slot_OnButtonClicked() ) );
     }
    QString GetName(){return m_Name;}


signals:
 void Sign_ButtonClickedPtr(const char* name, void *ptr);
 void Sign_ButtonClicked(const char* name);

public slots:
   void Slot_OnButtonClicked()
    {if (m_ptr==0) emit Sign_ButtonClicked(m_Name.toLatin1());
     else          emit Sign_ButtonClickedPtr(m_Name.toLatin1(), m_ptr);
    }

//........................................... DATA ...............................................................
private:
 void   *m_ptr;
 QString m_Name;
};

//======================== C_Frm_Rdv =============================
class C_Frm_Rdv : public QFrame, public C_RendezVous
{
    Q_OBJECT

public:
    enum mode {
        NO_GRAB,
        BOTTOM_START,
        TOP_START,
        MIDLE_START
    };
    enum adjustMode {
        JustTime,
        TimeAndGraphic
       };
    C_Frm_Rdv(         C_RendezVous *pC_RendezVous ,
                      //........... gui .................
                      int                resoPix   = 1,
                      int             timeGranu    = 5,
                      QRubberBand    *pQRubberBand = 0,
                      CMoteurAgenda*pCMoteurAgenda = 0,
                      MAP_COLOR *colorProfils      = 0,
                      C_BitMapCollection  *pC_BitMapCollection = 0,
                      int minHeight=15, QWidget *parent = 0);
    ~C_Frm_Rdv();
    void     setWidgetStyleOnRdv(const C_RendezVous &rdv);
    void     setWidgetOnRdv(const C_RendezVous &rdv);
    int      getHeight();
    void     mousePressEvent(QMouseEvent *event);
    void     mouseReleaseEvent ( QMouseEvent * event );
    void     mouseMoveEvent ( QMouseEvent * event );
    void     mouseMoveEvent ( QMouseEvent * event , QWidget *pQWidget );
    bool     eventFilter(QObject *obj, QEvent *event);
    int             getMagnetisme();
    int             adjustToMagnetisme(int value);
    void            adjustWidgetToMagnetisme(C_Frm_Rdv::adjustMode mode = C_Frm_Rdv::TimeAndGraphic);
    bool            isCopyExist();
    C_RendezVous    getCopy();
    int      getResoPixByMinutes(){return m_PixByMinute;}
    QString  getRdvColor();
    void     setResoPixByMinutes(int resoPix){m_PixByMinute = resoPix;}
    QString  computeTextButton();
    QString  getQWhatsThisString();
    QTime    posY_toTime(int posy);
    int      time_toPosY(const QTime &tm);
    void     dragEnterEvent(QDragEnterEvent *event);
    void     dragMoveEvent(QDragMoveEvent * event);
    void     dropEvent(QDropEvent *event);
    void     dragLeaveEvent ( QDragLeaveEvent *event  );
    void     setGeometry ( int x, int y, int w, int h );
    void     placeInfoEdit (int h);
    void     RDV_Update();
    void     replaceWithCopy();

    static   QString      serialize(C_Frm_Rdv *pC_Frm_Rdv);
    static   C_RendezVous unSerialize(const QString & data);

    void     paintEvent ( QPaintEvent * event );

    //........ objets publics ...............
    QLabel             *m_textLabel_Nom;
    C_RdvHeureButton   *m_button_HeureDuree;
    CMyButton          *m_ButtonAcceder;
    C_AgendaEdit       *m_InfoEdit;
public slots:
   void  Slot_EditTimeOut();
   void  Slot_Drop_Rdv ();
   void  Slot_textLabel_HeureClicked();
   void  Slot_ButtonChangeClicked(const char*, void *ptr);
   void  Slot_InfoEdit_cursorPositionChanged ();
   void  Slot_InfoEdittextChanged();
   void  Slot_InfoEditFocusOutEvent(QFocusEvent *event);
   void            Slot_copy();
   void            Slot_cut();
   void            Slot_delete();

private:
  int            m_Width;
  int            m_EditTimerActive;
  int            m_Magnetisme;
  int            m_MinHeight;
  int            m_PixByMinute;
  int            m_widget_w;
  CMoteurAgenda *m_pCMoteurAgenda;
  MAP_COLOR     *m_pColorProfils;
  C_RendezVous         m_C_RendezVousSav;
  C_BitMapCollection  *m_pBMC;
  int            m_PosXSav;
  int            m_PosYSav;
  int            m_WidthSav;
  int            m_HeightSav;
  CMyButton     *m_ButtonChange;
  CMyButton     *m_ButtonDelete;
  int                 m_GrabIsOn;;
  int                 m_startPosY;
  QRubberBand        *m_pQRubberBand;
  QColor              m_background_color;

signals:
  void Sign_DeleteRendezVous (const char*, void *);
  void Sign_RendezVousChangeClicked(C_Frm_Rdv*);
  void Sign_agenda_GetInfoFromUser(QString &, QString &, QString &, QString &);
  void Sign_ReplaceRdvByDroppedRdv(C_Frm_Rdv *);
  void Sign_StopTimer(int state);
};

//======================== C_Frm_RdvList =============================
class C_Frm_RdvList : public QList <C_Frm_Rdv*>
{
public:

    //------------------------ C_Frm_RdvList -------------------------------
    C_Frm_RdvList( const C_Frm_RdvList& l )
        : QList<C_Frm_Rdv*>(l)
    {
    }
    C_Frm_RdvList()
         : QList<C_Frm_Rdv*>()
    {
    }
    //------------------------ ~C_Frm_RdvList -------------------------------
    ~C_Frm_RdvList(){clear();}

    //------------------------ append_C_Frm_Rdv -------------------------------
    C_Frm_Rdv* append_C_Frm_Rdv(C_Frm_Rdv* pC_Frm_Rdv)
    {append ( pC_Frm_Rdv );
     return pC_Frm_Rdv;
    }

    //------------------------ clear -------------------------------
    void clear()
    { while (!isEmpty())
        {C_Frm_Rdv* pC_Frm_Rdv = takeFirst();
         delete pC_Frm_Rdv;
        }
    }
};

//======================== C_RendezVousList =============================
class C_RendezVousList : public QList <C_RendezVous*>
{
public:
    //------------------------ C_RendezVous -------------------------------
    C_RendezVousList( const C_RendezVousList& l )
        : QList<C_RendezVous*>(l)
    {
    }
    C_RendezVousList()
         : QList<C_RendezVous*>()
    {
    }
    //------------------------ ~C_RendezVousList -------------------------------
    ~C_RendezVousList(){clear();}
    //------------------------ appendRdv -------------------------------
    C_RendezVous* appendRdv(C_RendezVous* pC_RendezVous)
       {append ( pC_RendezVous );
        return pC_RendezVous;
       }

    //------------------------ removeIndex -------------------------------
    void removeIndex ( int i )
    {C_RendezVous* pC_RendezVous = at(i);
     removeAt(i);
     delete pC_RendezVous;
    }

    //------------------------ clear -------------------------------
    void clear()
    {while (!isEmpty())
        {   C_RendezVous* pC_RendezVous = takeFirst();
            delete pC_RendezVous;
        }
    }

};

//======================== C_Frm_Day =============================
class C_Frm_Day : public QFrame, public C_Frm_RdvList
{
    Q_OBJECT

public:

    C_Frm_Day(CMoteurAgenda*pCMoteurAgenda,
              MAP_COLOR *colorProfils,
              C_BitMapCollection *pC_BitMapCollection,
              QDate date,
              const QString &signUser,
              const QString &user,
              const QString &userNomPrenom,
              QWidget *parent,
              int timeGranu,
              int x = W_OFSET,
              int y = 0,
              int resoPix=1,
              int day_expand=0);  // CZA);
    ~C_Frm_Day();
    void            toGoogle(C_GoogleAPI *pC_GoogleAPI);
    int             getHeight(){return m_Height;}
    int             computeDayHeight();
    int             getNbMinutesToseeInResume();
    int             getNbMinutesToseeInResume(const QTime &tpsDeb, const QTime &tpsEnd);
    QRect           computeResumeRdvGeometry(C_RendezVous *pRdv, int startTimeInMinutes, int nbTotalMinutesToSee, int widthSegment, int lineH, int nbLine, QRect &rect2, int rectH = 11 );
    QPoint          computeResumePosXYFromTimeInMinute(int timeInMinutes, int startTimeInMinutes, int nbTotalMinutesToSee, int widthSegment, int lineH, int nbLine);
    int             RecreateRendezVousListWidget(C_RendezVousList& rendezVousList);
    void            readjustListWidgetPositions();
    void            ReArangeIfDayHeightChange();
    int             MaskDayToAgendaDay (C_RendezVousList &rendezVousList);
    void            moveWidget(int x, int y){QFrame::move(x,y);}
    const QDate&    getDate(){return m_Date;}
    bool            isCopyExist();
    C_RendezVous    getCopy();
    void            copyRdv(const C_RendezVous &rdv);

    void            dropEvent(QDropEvent *event);
    void            dragEnterEvent(QDragEnterEvent *event);
    void            dragMoveEvent(QDragMoveEvent * event);
    void            dragLeaveEvent ( QDragLeaveEvent *  );
    int             getLastDroppedData(QString &nom, QString &prenom, QString &tel, QString &guid, QString &where);

    int             isDayExpand(){return m_IsDayExpand; /*return (m_Height!=DAY_HEIGHT);*/}
    void            mousePressEvent ( QMouseEvent * event );
    void            On_Day_mousePressEvent ( QMouseEvent * event );
    QString         doRdvMenu(C_RendezVous *pRdvDst, int isOptionDetruire   = 0 );
    QMouseEvent    *On_Rdv_MouseEvent(QMouseEvent * event, C_Frm_Rdv *pC_Frm_Rdv);
    int             rdvPropertyDialog(C_RendezVous *rdvDst);
    void            paintEvent ( QPaintEvent * event );
    QTime           getStartTime();
    QTime           getStopTime();
    QTime           posY_toTime(int posY);
    int             time_toPosY(const QTime &time);
    int             posY_toMinutes(int posY);
    int             getMagnetisme(){return m_Magnetisme;}
    int             adjustToMagnetisme(int value);
    int             getResoPixByMinutes(){return m_PixByMinute;}
    void            setResoPixByMinutes(int resoPix){m_PixByMinute = resoPix;}
    void            newRDVAtThisDate(QDateTime dateTime, int duree=15, const QString &type ="", const QString &nom ="", const QString &prenom ="", const QString &tel ="", const QString &guid ="",const QString &where ="", const QString &pk ="");
    int             get_C_Frm_RdvBeforeAndAfter(int y_pos, C_Frm_Rdv **pC_Frm_RdvBefore,  C_Frm_Rdv **pC_Frm_RdvAfter=0);
    C_RendezVous    getRdvAfterThisTimeInMinutes( int minutesRef, int *mnRet  =0 );
    C_RendezVous    getRdvBeforeThisTimeInMinutes(int minutesRef, int *mnRet =0 );
    void            generate_cacheRDV_List_FromDayWidget();
    C_RendezVous   *getFirstRdv(const C_RendezVousList &rendezVousList);
    C_RendezVous   *getLastRdv();
    C_Frm_Rdv*      appendNewRDVItem(C_RendezVous *pC_RendezVous);
    bool            event(QEvent * event );
    C_RendezVous    getResumeRdvFromXY ( int x, int y );
    C_RendezVous    getRdvUnderMouse(int y_pos);
    QString         getRdvColor(const C_RendezVous & rdv);
    void            replaceRdvByRdv(C_Frm_Rdv *pC_Frm_Rdv_Dst, const C_RendezVous &rdvSrc);
    int             XYToMinutesInResume ( int x, int y, int *retNbMnToEnd  = 0 );
    C_RendezVous   *getRdvUnderMouseInResume ( int x, int y , int *index  = 0 );
    void            Chercher_les_RDV_dun_patient(QString nom_prenom_guid  = ""  );
    void            ExpandDialog();
    void            Imprimer_les_RDV_dun_medecin(QDateTime date_rdv, QString code_user);
    void            Imprimer_ce_RDV( QString loginMed, QString primkey_rdv );         // PRINTRDV
    int             getStateLook(){return m_DontRecreateWidget;}
    void            changeGUID (const QString &old_guid, const QString &new_guid);

    //.......... DATA  public ........
    C_RendezVous    m_DropedRdv;
    CMyButton       *m_ButtonExpand;

private:
  QDate            m_Date;
  QString          m_HeureDeb;
  QString          m_HeureFin;
  QString          m_User;
  QString          m_SignUser;
  QString          m_UserNomPrenom;
  int              m_BaseDayHeight;
  int              m_Height;
  int              m_DayLF;
  int              m_Magnetisme;
  CMyButton       *m_ButtonNewRDV;
  CMyButton       *m_ButtonSave;
  CMyButton       *m_ButtonGoogle;
  CMoteurAgenda   *m_pCMoteurAgenda;
  MAP_COLOR       *m_pColorProfils;
  C_BitMapCollection  *m_pBMC;
  C_RendezVousList m_cacheRDV_List;
  int              m_PixByMinute;
  QTimer          *m_RefreshTimer;
  int              m_rafraich;
  int              m_SaveStateGrab;
  int              m_DontRecreateWidget;
  int              m_IsDayExpand;
  int              m_Width;
  QRubberBand     *m_pQRubberBand;

public slots:
  void OnButtonSaveClickedPtr (const char*,  void *);
  void OnButtonExpandClickedPtr(const char*, void *ptr);
  void OnButtonNewRDVClickedPtr(const char*, void*);
  //............... le boutton responsable est fils ..................
  //                void * est un pointeur dessus
  void Slot_ButtonRDVDeleteClicked(const char*, void *);
  void Slot_RendezVousChangeClicked(C_Frm_Rdv*);
  void Slot_ButtonAccederClicked(const char*, void *);
  void Slot_ReplaceRdvByDroppedRdv(C_Frm_Rdv *);
  void Slot_RefreshView();
  void Slot_StopTimer(int state);
  void Slot_Drop_Rdv ();

signals:
  void Sign_AgendaMustBeReArange();
  void Sign_AgendaMustDisplayFromThisDate(const QDate &  ); // CZA
  void Sign_agenda_GetInfoFromUser(QString &, QString &, QString &, QString &);
  void Sign_LauchPatient(const QString &, C_RendezVous *);

};

//======================== C_Frm_DayList =============================
class C_Frm_DayList : public QList <C_Frm_Day*>
{
public:
    //------------------------ C_Frm_DayList -------------------------------
    C_Frm_DayList( const C_Frm_DayList& l )
        : QList<C_Frm_Day*>(l)
    {
    }
    C_Frm_DayList()
         : QList<C_Frm_Day*>()
    {
    }
    //------------------------ ~C_Frm_DayList -------------------------------
    ~C_Frm_DayList(){clear();}
    //------------------------ appendDay -------------------------------
    int appendDay(C_Frm_Day* pC_Frm_Day)
    {append ( pC_Frm_Day );
     return pC_Frm_Day->getHeight();
    }
    //------------------------ clear -------------------------------
    void clear()
    { while (!isEmpty())
        {C_Frm_Day* pC_Frm_Day = takeFirst();
         delete pC_Frm_Day;
        }
    }
};

//======================== C_Frm_Agenda =============================
class QTreeWidgetItem;
class QWebView;

class C_Frm_Agenda : public QFrame, public C_Frm_DayList
{
    Q_OBJECT
public:
    enum paint_mode {
        DISABLED   = 0,
        FOR_OTHERS = 1,
        NORMAL     = 2
    };
    enum syncMode {
            notNomade        = 0,
            fullReplication  = 1,        // le nomade est soit connecte au serveur soit deconnecte (nomade)
            fullSynchro      = 2,        //.le nomade est toujours deconnecte et parfois se synchronise au reseau de reference
            progMasterUpdate = 3,
            scriptOnly       = 4
        };
    enum syncGoogle {
            noSynchro          = 0,
            selectedDay        = 1,
            severalMonth       = 2        // le nomade est soit connecte au serveur soit deconnecte (nomade)
        };

    C_Frm_Agenda(const QDate &date             = QDate::currentDate(),
                 QWidget *parent               = 0,
                 const QString &pathAppli      ="",
                 const QString &localParam     ="",
                 const QString &signUser       ="admin",
                 const QString &user           ="",
                 const QString &userNomPrenom  ="",
                 const QString &droits         ="-sgn-agc-agm",
                 const QString &googleUser     ="",
                 const QString &googlePass     =""
                );
    ~C_Frm_Agenda();
    void baseReConnect(         const QString &driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                const QString &baseToConnect, // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                const QString &user,          // = "root"
                                const QString &pasword,       // = ""
                                const QString &hostname,      // = "localhost"
                                const QString &port           // = "port"
                      );
    int          getPaintMode(){return m_PaintMode;}
    QWebView    *getWebView();
    //void         paintEvent ( QPaintEvent * event );
    QDate    getStartDate(){ return m_StartDate;}
    void     setStartDate(const QDate &date){m_StartDate = date;}
    void     setMagnetisme(int mag)        { m_Magnetisme = mag;}
    int      getMagnetisme()        { return m_Magnetisme;}
    int      getResoPixByMinutes(){return m_PixByMinute;}
    void     setResoPixByMinutes(int resoPix){m_PixByMinute = resoPix;}
    QString  getUser()    {return m_User;}
    QString  getSignUser(){return m_SignUser; }
    QString  getTitleTemplate();
    void     getFreeSpace();
    int      getAgendaButtonBoxHeight();
    int      getTitleHeight();
    void     resizeBitMapHeader(const QString &WeekOrDay, int dayWitdth);
    int      Get_C_Frm_Day_FromDate( QDate dt, int firstDayInWeek  = 0  );
    //.............. google .........................
    void     setGoogleLoginParam (const QString &googleUser, const QString &googlePass );
    QString  getGoogleUser (){return m_googleUser;}
    QString  getGooglePass (){return m_googlePass;}
    void     toGoogleSynchonization(int months);
    void     toGoogle(const QDateTime &dateDeb, const QDateTime &dateEnd);
    //...............................................
    void     changeGUID (const QString &old_guid, const QString &new_guid);
    void     setUser(const QString &user)        { m_User     = user;}
    void     setSignUser(const QString &signUser){ m_SignUser = signUser; }
    void     changePixelParMinute ( int pixelParMinute );
    void     changeMagnetisme ( int magnetisme );
    void     changeStartTime ( const QString &time );
    void     changeEndTime ( const QString &time );
    void     changeRafraichissement(int rafraichissement);
    void     changeRepresentation(int representation);
    void     changeModifConfirm(int value);
    void     changeAgendaWidth(int value);
    void     setAgendaMode_WeekOrDayOrMonth(QString WeekOrDay);
    QString  getAgendaMode_WeekOrDayOrMonth() ;
    void     reinitAgendaOnDate(QDate dateDeb);
    void     reinitAgendaOnUser(const QString& user, const QString &droits);
    CMoteurAgenda *GetCMoteurAgenda(){return m_pCMoteurAgenda;}
    void     creerRDVFactices(const QString &user);
    void     Chercher_les_RDV_dun_patient (QString nom_prenom_guid  ="" );
    void     Imprimer_les_RDV_dun_medecin(QDateTime date_rdv, QString code_user );
    //............ animation .......................
    void     createAnimations();
    void     animateBottom();
    void     animateRight();
    void     animateLeft();
    void     doAnimation(QPropertyAnimation *pQPropertyAnimation, int vaitForEnd=1);
    QDate                m_tmpDateToStart;
    QMap<int, int>       m_WeekExpandMapState;

public slots:
    void   OnButtonGoogleClickedPtr (const char*, void *);
    void   On_AgendaMustDisplayFromThisDate(const QDate & newDate);        // CZA
    void   On_AgendaMustBeReArange();
    void   Slot_startToDate();
    void   Slot_ExpandWeek();
    void   Slot_GoogleAPI_Sync();
private:
    void GoogleAPI_Sync_CurrentDay();
    void GoogleAPI_Sync_BetweenTwoDate();

    QString              m_googleUser;
    QString              m_googlePass;
    QString              m_StylePopup;
    QDate                m_StartDate;
    int                  m_StartBefore;
    int                  m_Magnetisme;
    QString              m_PathAppli;
    QString              m_LocalParam;
    int                  m_IsGestionNomadisme;
    int                  m_IsNomadeActif;
    CMoteurAgenda       *m_pCMoteurAgenda;
    QString              m_SignUser;
    QString              m_User;
    QString              m_UserNomPrenom;
    QString              m_Droits;
    int                  m_PaintMode;
    MAP_COLOR            m_ColorProfils;
    C_BitMapCollection  *m_pBMC;
    int                  m_PixByMinute;
    QString              m_BackgroundMessage;
    QWebView            *m_pQWebView;
    C_GoogleAPI         *m_pC_GoogleAPI;
    int                  m_AjustGoogleAgendaTime;

    QMap <int, QLabel*>  m_MonthLabelList;      // key = annee : 2000 *10000 + month number
    QString              m_MonthLabelCss;
    QWidget             *m_viewport;
    QAnimationGroup     *m_animationGroup;
    void                *m_ptrOnDay;        /*!< temporary used for google synchro >*/
    QDateTime            m_dateDebSync;     /*!< temporary used for google synchro >*/
    QDateTime            m_dateEndSync;     /*!< temporary used for google synchro >*/
    int                  m_syncGoogle;      /*!< this day or month google synchro >*/

signals:
    void Sign_agenda_GetInfoFromUser(QString &, QString &, QString &, QString &);
    void Sign_LauchPatient(const QString &, C_RendezVous *);
    void Sign_ReinitModeWeekDayMonth_OnDate(const QDate & date, const QString &mode, C_Frm_Agenda* pC_Frm_Agenda);
};

/*
#ifdef AGENDA_IN_GUI
//======================== C_Dlg_MainWindow =============================
class C_Dlg_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
     C_Dlg_MainWindow(QWidget *parent = 0);
     C_Dlg_MainWindow(const QString &pathAppli, const QString &localParam, const QString &signUser, QWidget *parent = 0);
    ~C_Dlg_MainWindow();
     C_Frm_Agenda *getFrm_Agenda(){return m_pC_Frm_Agenda;}
public slots:
     void On_pushButtonDateDebClicked();
private:
    Ui::MainWindowClass *ui;
    QFrame       *m_pMultiAgendaFrame;
    QBoxLayout   *m_pAgendaQLayout;
    C_Frm_Agenda *m_pC_Frm_Agenda;
    C_RendezVous  m_C_RendezVousCopy;
};
#endif
*/
#endif // MAINWINDOW_H

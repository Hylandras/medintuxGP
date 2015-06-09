#ifndef C_FRM_PRESCRIPTION_H
#define C_FRM_PRESCRIPTION_H

#include "C_BDM_Api.h"
#include <QFrame>
#include <QPointer>
#include <QPushButton>
#include <QLineEdit>
#include <QEvent>
#include <QTextEdit>
#include <QScrollArea>
#include <QPropertyAnimation>
#include <QApplication>
#include <QStackedWidget>
#include <QListWidgetItem>
#include <QListWidget>
#include <QCalendarWidget>
#include <QDateEdit>
#include <QTableWidget>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include "../../MedinTuxTools-QT4/C_PopupDial/C_PopupDial.h"

class QRubberBand;
class QCheckBox;
class QRect;
class QToolButton;
class C_PopupDialCorpoFact;

//====================================== CMyQTextEdit =======================================================
class CMyQTextEdit : public QTextEdit
{
Q_OBJECT
//........................................... METHODES ...........................................................
public:
     CMyQTextEdit (const QString   &text            = "",
                   QWidget*        parent           = 0 ,
                   const QString  &name             = 0 ,
                   void*           ptr              = 0
          )
        : QTextEdit (name , parent)
    {   setText(text);
        setObjectName(name);
        m_ptr            = ptr;
        m_customString   = "";
    }
     void     setCustomString(const QString &str) { m_customString = str;}
     QString  getCustomString()                   {return m_customString;}
     void    *getCustomPtr()                      {return m_ptr;}
     void     mousePressEvent ( QMouseEvent * e );
     void     mouseDoubleClickEvent ( QMouseEvent * event );
//........................................... DATA ...............................................................
signals:
     void Sign_mousePressEvent    ( QMouseEvent * e , void *ptr);
     void Sign_mouseDoubleClicked ( QMouseEvent * e , void *ptr);
private:
  void      *m_ptr;
  QString    m_customString;
};

//====================================== CMyQLineEdit =======================================================
class CMyQLineEdit : public QLineEdit
{
Q_OBJECT
//........................................... METHODES ...........................................................
public:
     CMyQLineEdit (const QString   &text            = "",
                   QWidget*        parent           = 0 ,
                   const char*     name             = 0 ,
                   void*           ptr              = 0
          )
        : QLineEdit (name , parent)
    {   setText(text);
        setObjectName(name);
        m_ptr      = ptr;
        setFocusPolicy(Qt::StrongFocus);
        setAlignment ( Qt::AlignHCenter );
        connect( this, SIGNAL( returnPressed ()  ),              this , SLOT( Slot_returnPressed() ));
        connect( this, SIGNAL( textChanged (const QString&)  ),  this , SLOT( Slot_textChanged(const QString&) ));
    }

     CMyQLineEdit (const QRect     &rect       ,
                   const QString   &text   = "",
                   QWidget*         parent = 0 ,
                   const QString    &name  = 0 ,
                   void*            ptr    = 0
                  )
         : QLineEdit (name , parent)
    {   setText(text);
        setObjectName(name);
        m_ptr      = ptr;
        setGeometry(rect);
        setFocusPolicy(Qt::StrongFocus);
        setAlignment ( Qt::AlignHCenter );
        connect( this, SIGNAL( returnPressed ()  ),  this , SLOT( Slot_returnPressed() ));
        connect( this, SIGNAL( textChanged (const QString&)  ),  this , SLOT( Slot_textChanged(const QString&) ));
    }
     QString getFreeData(){return m_free_data;}
     void    setFreeData(const QString &value){m_free_data=value;}
     bool  event           ( QEvent      * e );
     void  focusInEvent    ( QFocusEvent * e );
     void  focusOutEvent   ( QFocusEvent * e );
     void  mousePressEvent ( QMouseEvent * e );
     void  setCaractersMask(const QString &mask);
signals:
     void Sign_returnPressed(const QString &name, CMyQLineEdit *pCMyQLineEdit, void *ptr, Qt::KeyboardModifiers modifiers);
     void Sign_focusIn      (const QString &name, CMyQLineEdit *pCMyQLineEdit, void *ptr);
     void Sign_focusOut     (const QString &name, CMyQLineEdit *pCMyQLineEdit, void *ptr);
     void Sign_textChanged  (const QString &text, CMyQLineEdit *pCMyQLineEdit, void *ptr);
public slots:
     void Slot_returnPressed()
     { emit Sign_returnPressed( objectName(), this, m_ptr, 0);
     }
     void Slot_textChanged(const QString &text)
     { emit Sign_textChanged( text, this, m_ptr);
     }

//........................................... DATA ...............................................................
private:
  QString m_caractersMask;
  void   *m_ptr;
  QString m_free_data;
};
//====================================== CMyButton =======================================================
class CMyButton : public QPushButton
{
Q_OBJECT
public:
    enum flatitiude {
                      FLAT_REAL     = 0,
                      RELIEF        = 1
                   };
//........................................... METHODES ...........................................................
public:
    //------------------- CMyButton ------------------------------
     CMyButton (QWidget*        parent           = 0,
                const QString   &name            = 0,
                void*           ptr              = 0

          )
        : QPushButton (name , parent)
     {  m_flat = CMyButton::RELIEF;
        setObjectName(name);
        m_ptr  = ptr;
        this->setFlat(true);
        connect( this, SIGNAL( clicked() ), this, SLOT( Slot_OnButtonClicked() ) );
    }

     //------------------- CMyButton ------------------------------
     CMyButton (const QRect     &rect      ,
                QWidget*         parent = 0,
                const QString    &name  = 0, // "buton text , object name"
                void*            ptr    = 0
           )
         : QPushButton (name , parent)
     {
         m_flat = CMyButton::RELIEF;
         setObjectName(name);
         m_ptr  = ptr;
         setGeometry(rect);
         this->setFlat(true);
         connect( this, SIGNAL( clicked() ), this, SLOT( Slot_OnButtonClicked() ) );
     }
     //------------------- CMyButton ------------------------------
     CMyButton (int   flatitude,
                const QIcon     &icon,
                const QRect     &rect,
                QWidget*         parent           = 0,
                const QString&   name             = 0,
                void*            ptr              = 0
               )
         : QPushButton (icon, name, parent)   // important permet que le widget soit enfant du parent
     {m_flat = flatitude;
      setObjectName(name);
      m_ptr  = ptr;
      setIconSize ( QSize(rect.width(),rect.height()));
      setGeometry(rect);
      this->setFlat(true);
      connect( this, SIGNAL( clicked() ), this, SLOT( Slot_OnButtonClicked() ) );
     }
     //------------------- CMyButton ------------------------------
     CMyButton (const QIcon     &icon,
                const QRect     &rect,
                QWidget*         parent           = 0,
                const QString&   name             = 0,
                void*            ptr              = 0
               )
         : QPushButton (icon, name, parent)   // important permet que le widget soit enfant du parent
     {m_flat = CMyButton::RELIEF;
      setObjectName(name);
      m_ptr  = ptr;
      setIconSize ( QSize(rect.width(),rect.height()));
      setGeometry(rect);
      this->setFlat(true);
      connect( this, SIGNAL( clicked() ), this, SLOT( Slot_OnButtonClicked() ) );
     }
  //------------------- CMyButton ------------------------------
  CMyButton (const QIcon    & icon,
             QWidget*         parent           = 0,
             const QString&   name             = 0,
             void*            ptr              = 0
            )
         : QPushButton (icon, name, parent)   // important permet que le widget soit enfant du parent
     {m_flat = CMyButton::RELIEF;
      setObjectName(name);
      m_ptr  = ptr;
      setIconSize ( QSize(16,16) );
      this->setFlat(true);
      connect( this, SIGNAL( clicked() ), this, SLOT( Slot_OnButtonClicked() ) );
     }
//------------------- setObjectName ------------------------------
void setObjectName ( const QString & name )
{int pos = name.indexOf(',');
 if (pos==-1)
     pos = name.indexOf('|');
 if (pos != -1)
    {QPushButton::setObjectName(name.mid(pos+1));
     setText(name.left(pos));
    }
 else
    {QPushButton::setObjectName(name);
     setText(name);
    }
 if (m_flat==CMyButton::RELIEF)
    { QPalette pal = QApplication::palette (this->metaObject()->className()) ;
      QColor color = pal.color(QPalette::Active, QPalette::Button); //.darker(160);
      setColor(color);
    }
}
//---------------------- setColor ----------------------------
void setColor(const QColor &color)
{ setStyleSheet(color);
}
//---------------------- setStyleSheet -----------------------
void setStyleSheet(const QColor &color)
{ QPushButton::setStyleSheet( QString (
        //" background-color: %1;"
        " border: 1px solid %1;"
        " background: qlineargradient(    x1: 0, y1: 0, x2: 0, y2: 1,"
        "                                 stop: 0 %2,   stop: 0.4 %3,"
        "                                 stop: 0.5 %4, stop: 1.0 %5);"
        //" border-style: outset;"
        " border-radius: 3px;").arg(color.darker(200).name(),
                                    color.lighter(200).name(),
                                    color.darker(100).name(),
                                    color.darker(100).name(),
                                    color.darker(160).name()
                                   )
       );
}
void set_propr_1 (const QString &value){m_prop_1 = value; }
void set_propr_2 (const QString &value){m_prop_2 = value; }
void set_propr_3 (const QString &value){m_prop_3 = value; }
QString prop_1() { return m_prop_1;}
QString prop_2() { return m_prop_2;}
QString prop_3() { return m_prop_3;}

signals:
 void Sign_ButtonClickedPtr(const QString &name, void *ptr, CMyButton* pCMyButton);
 void Sign_ButtonClickedPtr(const QString &name, void *ptr);
 void Sign_ButtonClicked(   const QString &name);

public slots:
   void Slot_OnButtonClicked()
    {if (m_ptr==0) emit Sign_ButtonClicked   (this->objectName());
     else          emit Sign_ButtonClickedPtr(this->objectName(), m_ptr);
    }

//........................................... DATA ...............................................................
private:
 void   *m_ptr;
 int     m_flat;
 QString m_prop_1;
 QString m_prop_2;
 QString m_prop_3;

};
//====================================== CMyColorButton =======================================================
class CMyColorButton : public CMyButton
{
Q_OBJECT
//........................................... METHODES ...........................................................
public:
    //------------------- CMyButton ------------------------------
     CMyColorButton ( QColor          color               ,
                      QWidget*        parent           = 0,
                      const QString   &name            = 0,
                      void*           ptr              = 0
          )
        : CMyButton (parent , name, ptr)
    {setColor (color );
    }

     //------------------- CMyColorButton ------------------------------
     CMyColorButton ( QColor           color     ,
                      const QRect     &rect      ,
                      QWidget*         parent = 0,
                      const QString    &name  = 0, // "buton text , object name"
                      void*            ptr    = 0
                    )
         : CMyButton (rect, parent, name , ptr)
     {setColor (color );
     }
     //------------------- CMyColorButton ------------------------------
     CMyColorButton ( QColor           color               ,
                      const QIcon     &icon                ,
                      const QRect     &rect                ,
                      QWidget*         parent           = 0,
                      const QString&   name             = 0,
                      void*            ptr              = 0
               )
         : CMyButton (icon, rect, parent, name, ptr)   // important permet que le widget soit enfant du parent
     {setColor (color );
     }
  //------------------- CMyColorButton ------------------------------
  CMyColorButton ( QColor           color               ,
                   const QIcon    & icon                ,
                   QWidget*         parent           = 0,
                   const QString&   name             = 0,
                   void*            ptr              = 0
            )
         : CMyButton (icon, parent, name, ptr)   // important permet que le widget soit enfant du parent
     {setColor (color );
     }
//------------------- setColor ------------------------------
void setColor ( const QColor & color )
{
 //QPalette pal     = QApplication::palette (this->metaObject()->className()) ;
 //QColor bordColor = pal.color(QPalette::Active, QPalette::Button).darker(160);
 setStyleSheet(color);

// setStyleSheet("QPushButton{ background-color: #ff0000 }"
//               "QPushButton:pressed{ background-color: #0000ff }");
}

//---------------------- setStyleSheet -----------------------
void setStyleSheet(const QColor &color)
{  QPushButton::setStyleSheet( QString (
        //" background-color: %1;"
        " border: 1px solid %1;"
        " background: qlineargradient(    x1: 0, y1: 0, x2: 0, y2: 1,"
        "                                 stop: 0 %2,   stop: 0.4 %3,"
        "                                 stop: 0.5 %4, stop: 1.0 %5);"
        //" border-style: outset;"
        " border-radius: 3px;").arg(color.darker(200).name(),
                                    color.lighter(200).name(),
                                    color.darker(100).name(),
                                    color.darker(140).name(),
                                    color.darker(160).name()
                                   )

                 );
}
};

//====================================== C_BitMapCollection =======================================================
class C_BitMapCollection
 {
 public:
     C_BitMapCollection(const QString & pathImage)
     {m_objectsDel          = QPixmap(pathImage + "objectDel.png");
      m_ButtonMatin         = QPixmap(pathImage + "matin.png");
      m_ButtonMidi          = QPixmap(pathImage + "midi.png");
      m_ButtonApresMidi     = QPixmap(pathImage + "apresmidi.png");
      m_ButtonSoir          = QPixmap(pathImage + "soir.png");
      m_ButtonCoucher       = QPixmap(pathImage + "coucher.png");
      m_ButtonNuit          = QPixmap(pathImage + "nuit.png");
      m_ButtonSequenceList  = QPixmap(pathImage + "deploylist.png");
      m_ButtonDeploy        = QPixmap(pathImage + "deploylist.png");
      m_ButtonSequenceAdd   = QPixmap(pathImage + "sequenceadd.png");
      m_ButtonSequenceDel   = QPixmap(pathImage + "sequencedel.png");
      m_ButtonHitClose      = QPixmap(pathImage + "hitclose-16x16.png");

      m_back_repart         = QPixmap(pathImage + "back_repart.png");
      m_back_repart_matin   = QPixmap(pathImage + "back_repart_matin.png");
      m_back_repart_midi    = QPixmap(pathImage + "back_repart_midi.png");
      m_back_repart_amidi   = QPixmap(pathImage + "back_repart_amidi.png");
      m_back_repart_soir    = QPixmap(pathImage + "back_repart_soir.png");
      m_back_repart_coucher = QPixmap(pathImage + "back_repart_coucher.png");
      m_back_repart_nuit    = QPixmap(pathImage + "back_repart_nuit.png");

      m_ci_assinterdit      = QPixmap(pathImage + "ci_assinterdit.png");
      m_ci_deconseille      = QPixmap(pathImage + "ci_deconseille.png");
      m_ci_precautions      = QPixmap(pathImage + "ci_precautions.png");
      m_ci_eff_indesir      = QPixmap(pathImage + "ci_eff_indesir.png");
      m_ButtonAllergies     = QPixmap(pathImage + "ci_allergies.png");
      m_ButtonNoSecure      = QPixmap(pathImage + "no_secure.png");

      m_ci_terrain_abs      = QPixmap(pathImage + "ci_terrain_abs.png");
      m_ci_terrain_dec      = QPixmap(pathImage + "ci_terrain_dec.png");
      m_ci_terrain_pre      = QPixmap(pathImage + "ci_terrain_pre.png");
      m_ci_terrain_mis      = QPixmap(pathImage + "ci_terrain_mis.png");
      m_ci_terrain_inf      = QPixmap(pathImage + "ci_terrain_inf.png");

      m_ci_vigilance_1      = QPixmap(pathImage + "ci_vigilance_1.png");
      m_ci_vigilance_2      = QPixmap(pathImage + "ci_vigilance_2.png");
      m_ci_vigilance_3      = QPixmap(pathImage + "ci_vigilance_3.png");

      m_ci_sport            = QPixmap(pathImage + "ci_sport_01.png");

      m_Button_Repas        =  QPixmap(pathImage + "repas.png");
      m_Button_RepasAvant   =  QPixmap(pathImage + "repasAvant.png");
      m_Button_RepasPendant =  QPixmap(pathImage + "repasPendant.png");
      m_Button_RepasApres   =  QPixmap(pathImage + "repasApres.png");

      m_Button_delete       =  QPixmap(pathImage + "objectDel.png");
      m_Button_UnCp         =  QPixmap(pathImage + "UnCp-18x18.png");
      m_Button_UnQuartCp    =  QPixmap(pathImage + "UnQuartCp-18x18.png");
      m_Button_UnDemiCp     =  QPixmap(pathImage + "UnDemiCp-18x18.png");
      m_Button_TroisQuartCp =  QPixmap(pathImage + "TroisQuartCp-18x18.png");

      m_Button_MenuLeft     =  QPixmap(pathImage + "menu_left.png");
      m_Button_MenuDown     =  QPixmap(pathImage + "menu_down.png");
      m_NonSubstituable     =  QPixmap(pathImage + "NonSubstituable.png");

      m_imagePath           = pathImage;
      m_back_repartResized  = 0;

     }

     void resize_back_repart(int _width, int _height)
     {   if (m_back_repartResized) return;
         int width  = (_width  ==-1) ? m_back_repart.width() : _width;
         int height = (_height ==-1) ? m_back_repart.height(): _height;
         m_back_repart        = m_back_repart.scaled(width,   height);
         m_back_repartResized = 1;
     }
     //------------------------------- pixmap -----------------------------------------------------------------
QPixmap pixmap(const QString &name)        {return QPixmap (m_imagePath+name);}
     //------------------------------- imagePath -----------------------------------------------------------------
QString imagePath(const QString &name="")  {return m_imagePath+name;}
     //------------------------------- terrainLevelToPixmap -----------------------------------------------------------------
QPixmap terrainLevelToPixmap(int level) const
     {switch (level)
         {
          case 0:     return m_ci_terrain_inf;
          case 1:     return m_ci_terrain_mis;
          case 2:     return m_ci_terrain_pre;
          case 3:     return m_ci_terrain_dec;
          case 4:     return m_ci_terrain_abs;
          default:    return QPixmap();
        }
     }
//------------------------------- getFileList -----------------------------------------------------------------
QStringList getFileList(const QString &list_name)
{ QString        path = m_imagePath + list_name;
  QString        data = CGestIni::Param_UpdateFromDisk(path).remove('\r');
  return data.split('\n');
}
     //------------------------------- terrainLevelToPixmapPath -----------------------------------------------------------------
QString terrainLevelToPixmapPath(int level) const
     {switch (level)
         {
          case 0:     return m_imagePath+"ci_terrain_inf.png";
          case 1:     return m_imagePath+"ci_terrain_mis.png";
          case 2:     return m_imagePath+"ci_terrain_pre.png";
          case 3:     return m_imagePath+"ci_terrain_dec.png";
          case 4:     return m_imagePath+"ci_terrain_abs.png";
          case 6:     return m_imagePath+"ci_terrain_gro.png";
          case 7:     return m_imagePath+"ci_terrain_all.png";
          case 8:     return m_imagePath+"ci_vigilance_010.png";
          case 9:     return m_imagePath+"ci_vigilance_020.png";
          case 10:    return m_imagePath+"ci_vigilance_030.png";
          case 11:    return m_imagePath+"ci_sport_01.png";

          default:    return "";
         }
     }
//------------------------------- allergieLevelToPixmap -----------------------------------------------------------------
QPixmap allergieLevelToPixmap(int level) const
{switch (level)
    {
     case 1:     return m_ButtonAllergies;
     default:    return m_ButtonAllergies;
   }
}
//------------------------------- allergieLevelToPixmapPath -----------------------------------------------------------------
QString allergieLevelToPixmapPath(int level) const
{switch (level)
    {
     case 1:     return m_imagePath+"ci_allergies.png";
     default:    return m_imagePath+"ci_allergies.png";
    }
}

//------------------------------- afssapsLevelToPixmap -----------------------------------------------------------------
QPixmap afssapsLevelToPixmap(int level) const
     {switch (level)
         {
         case 4:     return m_ci_assinterdit;
         case 3:     return m_ci_deconseille;
         case 2:     return m_ci_precautions;
         default:    return m_ci_eff_indesir;
        }
     }
     //------------------------------- afssapsLevelToPixmapPath -----------------------------------------------------------------
QString afssapsLevelToPixmapPath(int level) const
     {
       switch (level)
            {
             case 4:     return m_imagePath+"ci_assinterdit.png";;
             case 3:     return m_imagePath+"ci_deconseille.png";
             case 2:     return m_imagePath+"ci_precautions.png";
             default:    return m_imagePath+"ci_eff_indesir.png";
            }
     }
//------------------------------- getCalculatedTabZone_H -----------------------------------------------------------------
// pourquoi la : parce que la collection de bit map est accessible de partout et connait les haateurs de bitmap
// buttonH     saisie dose globale
// buttonH     repartition par jour
// marge
// posoZone    zone des jours de posologie
// marge
// buttonH     commentaire
// marge
int     getCalculatedTabZone_H(int buttonH, int marge)
{ return buttonH*2 + marge*2;
}//------------------------------- getCalculatedHitsList_H -----------------------------------------------------------------
// pourquoi la : parce que la collection de bit map est accessible de partout et connait les haateurs de bitmap
// buttonH     saisie dose globale
// buttonH     repartition par jour
// marge
// posoZone    zone des jours de posologie
// marge
// buttonH     commentaire
// marge
int     getCalculatedHitsList_H(int buttonH, int marge, int item_h , int nb_item)
{ return getCalculatedTabZone_H(buttonH, marge) + buttonH*4 + marge*7 + getCalculatedSequenceH( buttonH, marge) + nb_item * item_h;
}
//------------------------------- getCalculatedSequenceH -----------------------------------------------------------------
// pourquoi la : parce que la collection de bit map est accessible de partout et connait les haateurs de bitmap
// buttonH     saisie dose globale
// buttonH     repatition par jour
// marge
// posoZone    zone des jours de posologie
// marge
// buttonH     commentaire
// marge
int     getCalculatedSequenceH(int buttonH, int marge)
{ return buttonH*3 + marge*3 + qMax (getCalculatedPointPosoZoneH(marge),buttonH*8);
}

//------------------------------- getCalculatedPointPosoZoneH -----------------------------------------------------------------
// pourquoi la : parce que la collection de bit map est accessible de partout et connait les hauteurs de bitmap
int     getCalculatedPointPosoZoneH(int marge)
{ return (m_Button_UnCp.height() + 3) * 5 + m_Button_RepasAvant.height() + 1 + marge;
}

 public:
     QPixmap m_objectsDel;
     QPixmap m_ButtonMatin;
     QPixmap m_ButtonMidi;
     QPixmap m_ButtonApresMidi;
     QPixmap m_ButtonSoir;
     QPixmap m_ButtonCoucher;
     QPixmap m_ButtonNuit;
     QPixmap m_ButtonDeploy;
     QPixmap m_ButtonSequenceList;
     QPixmap m_ButtonSequenceAdd;
     QPixmap m_ButtonSequenceDel;
     QPixmap m_ButtonHitClose;

     QPixmap m_back_repart;
     QPixmap m_back_repart_matin;
     QPixmap m_back_repart_midi;
     QPixmap m_back_repart_amidi;
     QPixmap m_back_repart_soir;
     QPixmap m_back_repart_coucher;
     QPixmap m_back_repart_nuit;

     QPixmap m_Button_Repas;
     QPixmap m_Button_RepasAvant;
     QPixmap m_Button_RepasPendant;
     QPixmap m_Button_RepasApres;

     QPixmap m_ButtonAllergies;
     QPixmap m_ButtonNoSecure;

     QPixmap m_ci_assinterdit;
     QPixmap m_ci_deconseille;
     QPixmap m_ci_precautions;
     QPixmap m_ci_eff_indesir;

     QPixmap m_ci_terrain_abs;
     QPixmap m_ci_terrain_dec;
     QPixmap m_ci_terrain_pre;
     QPixmap m_ci_terrain_mis;
     QPixmap m_ci_terrain_inf;

     QPixmap m_ci_vigilance_1;
     QPixmap m_ci_vigilance_2;
     QPixmap m_ci_vigilance_3;

     QPixmap m_Button_delete;
     QPixmap m_Button_UnCp;
     QPixmap m_Button_UnQuartCp;
     QPixmap m_Button_UnDemiCp;
     QPixmap m_Button_TroisQuartCp;

     QPixmap m_Button_MenuLeft;
     QPixmap m_Button_MenuDown;

     QPixmap m_NonSubstituable;

     QPixmap m_ci_sport;

     QString m_imagePath;
     //QMap <QString, C_WordGenrePair> m_WordsGenreMap;
 private:
      int m_back_repartResized;

 };

class C_Frm_PosologieHitList;

//======================== C_PopupDialDate =============================
class C_PopupDialDate : public C_PopupDial
{
    Q_OBJECT
public:
     C_PopupDialDate    ( QWidget                  *parent,
                          const QRect              &initialRect,
                          const QRect              &deployRect,
                          C_Frm_PosologieHitList   *pC_Frm_PosologieHitList,  // il nous faut des icones
                          const QDateTime          &dateTime
                        );
     ~C_PopupDialDate()    {}
     void           setEditDateTime(const QDateTime& dt);
     QDateTime      dateTime();
     void           setTitle_QSP(  int duree, int dureeUnit);
     static QString dureeToTexte(  int duree, int dureeUnit);
     QPixmap        dureeToIcone(  int duree, int dureeUnit );
     QString        interfaceToDateTimeString(const QString &format = "dd-MM-yyyy hh:ss");
     //......... dci ....................................
     QPixmap        dciToIcone(int nb);
     void           dciToInterface(const QString &text);
     QString        dciToString();
     //......... renouvelable .............
     QPixmap        renouvToIcone( int nb);
     void           cycleToInterface(const QString &cycle);
     QString        interfaceToCycle();
     //.......... substituable ..............
     QPixmap        substituableToIcone(int nb);
     void           setSubstituable(bool state = true);
     bool           isSubstituable();
     //........... literalite .....................
     QPixmap        literaliteToIcone(int nb);
     QString        literaliteToString(int &numResult);
     void           literaliteToInterface(const QString &literalite);
     void           literaliteToInterface(int literalite);
     int            literaliteToInt();

     bool           event ( QEvent * ev );
     // void           mousePressEvent ( QMouseEvent * event ) ;

public slots:
    //  void Slot_setToDeployRect();
    void Slot_jours_editingFinished_clicked();
    void Slot_semaines_editingFinished_clicked();
    void Slot_mois_editingFinished_clicked();
    void Slot_editDateTime_DebChanged(const QDateTime& dateTime);
    void Slot_editDateTime_EndChanged(const QDateTime& dt_end);
    void Slot_Button_EndDate_clicked(const QString&);
    void Slot_dci_button_clicked(const QString &text);
    void Slot_calendarDateChanged(const QDate& date);
    void Slot_Qsp_button_clicked(const QString& text);
    void Slot_renouv_button_clicked(const QString&text);
    void Slot_substituable_button_clicked(const QString&text);
    void Slot_literalite_button_clicked(const QString&text);
    void Slot_DialDateDeployed(int &ok);

private:
    QCalendarWidget     *m_pQCalendarWidget;
    QDateTimeEdit       *m_pQDateTimeEdit_Deb;
    QDateTimeEdit       *m_pQDateTimeEdit_End;
    CMyColorButton      *m_Button_EndDate;
    int                  m_marge;
    int                  m_duree;
    int                  m_dureeUnit;
    QFont                m_fGrd;
    C_BitMapCollection  *m_pC_BitMapCollection;
    QLabel              *m_labelQSP;
    QLabel              *m_imageQSP;
    QLabel              *m_textQSP;
    QLabel              *m_imageRenouv;
    QLabel              *m_textRenouv;
    QLabel              *m_imageLiter;
    QLabel              *m_imageSusbtituable;
    QLabel              *m_imageDci;

    QTableWidget        *m_tableJours;
    QColor               m_backGround;
    QLineEdit           *m_Ed_jours;
    QLineEdit           *m_Ed_semaines;
    QLineEdit           *m_Ed_mois;
signals:
    void Sign_contentChanged(C_PopupDialDate* pC_PopupDialDate);
};
class C_Frm_PosologieHitList;

//======================== C_PopupDialPosoPoint =============================
class C_PopupDialPosoPoint : public C_PopupDial
{
    Q_OBJECT
public:
    C_PopupDialPosoPoint (QWidget                  *parent,
                          const QRect              &initialRect,
                          const QRect              &deployRect,
                          C_Frm_PosologieHitList  *pC_Frm_PosologieHitList,  // il nous faut des icones
                          const QString            &choice,
                          const QString            &pointName
                );
    ~C_PopupDialPosoPoint(){}

    QString choice(){return m_choice;}
    QString name() { return m_pointName;}
    bool    event ( QEvent * ev );
    void    setToDeployRect();
    void    setQuantite(const QString &text);
    void    setQuantite(const DOUBLE_PAIR &val, int mode);
    void    setMomentTimeMeal(const QString &tps);
    QString getQuantite(){return m_LineEditQuantite->text();}
    QString interfaceToGrammar();
    void    setMomentMealChoice(const QString &choice);
    void    setMomentMeal(int num);

private:
    QString             m_choice;
    QString             m_Momentchoice;
    QString             m_pointName;
    int                 m_numPoint;

    CMyColorButton     *m_MainButton;
    CMyQLineEdit       *m_LineEditQuantite;
    CMyQLineEdit       *m_LineEditTimeMeal;

    CMyColorButton     *m_Button_RepasAvant;
    CMyColorButton     *m_Button_RepasPendant;
    CMyColorButton     *m_Button_RepasApres;
    CMyColorButton     *m_Button_RepasNoChoice;

    CMyColorButton    *m_Button_delete;

    CMyColorButton    *m_Button_UnCp_0;
    CMyColorButton    *m_Button_UnQuartCp_0;
    CMyColorButton    *m_Button_UnDemiCp_0;
    CMyColorButton    *m_Button_TroisQuartCp_0;

    CMyColorButton    *m_Button_UnCp_1;
    CMyColorButton    *m_Button_UnQuartCp_1;
    CMyColorButton    *m_Button_UnDemiCp_1;
    CMyColorButton    *m_Button_TroisQuartCp_1;

    CMyColorButton    *m_Button_UnCp_2;
    CMyColorButton    *m_Button_UnQuartCp_2;
    CMyColorButton    *m_Button_UnDemiCp_2;
    CMyColorButton    *m_Button_TroisQuartCp_2;

    CMyColorButton    *m_Button_UnCp_3;
    CMyColorButton    *m_Button_UnQuartCp_3;
    CMyColorButton    *m_Button_UnDemiCp_3;
    CMyColorButton    *m_Button_TroisQuartCp_3;

    CMyColorButton    *m_Button_UnCp_4;
    CMyColorButton    *m_Button_UnQuartCp_4;
    CMyColorButton    *m_Button_UnDemiCp_4;
    CMyColorButton    *m_Button_TroisQuartCp_4;

   int                 m_buttonH ;
   int                 m_marge;
   C_BitMapCollection *m_pC_BitMapCollection;
   QColor              m_backGround;
   QFont               m_fGrd;

   QMap <QString, CMyColorButton*>         m_buttonMap;

private slots:
   void Slot_PosoPointDeployed(int &ok);
   void Slot_PosoPointMinimized(int &ok);
   void Slot_ButtonMomentMealClicked(const QString &objectName);
   void Slot_ButtonComprimeClicked(const QString &name);
   void Slot_textChanged(const QString &text, CMyQLineEdit *pCMyQLineEdit, void * ptr);

signals:
    void Sign_PosoPointDeployed(const QString &pointName);
    void Sign_contentChanged(C_PopupDialPosoPoint* pC_PopupDialPosoPoint);
};

//======================== C_Frm_Sequence =============================
class C_Frm_Produit;
class QTextEdit;
class C_Frm_PosologieHitList;
class C_Frm_Sequence : public QFrame  //, C_PosologieGrammar
{
    Q_OBJECT
public:
    C_Frm_Sequence (  C_Frm_PosologieHitList *pC_Frm_PosologieHitList,
                      QRect                   geometry_rect        // c'est le parent qui decide de la geometrie
                   );
    ~C_Frm_Sequence(){}
    void sequenceToInterface(const QString &sequence);

    DOUBLE_PAIR sequenceTimeToInterface(const QString &sequence);
    DOUBLE_PAIR sequenceHoursToInterface(const QString &before, const QString &after, const QString &value);
    void  sequenceDaysToInterface(const QString &_seq_days);
    QString interfaceToGrammar(QString &forme, QString &voie);
    QString grammar(){return m_cur_grammar;}
    void  clearInterface();
    void  setBorderColor(const QColor &color){m_borderColor = color;}
    void  clearDaysZoneEditors();
    void  setBackGround(QColor backGroundColor);
    bool  event ( QEvent * ev );
    void  paintEvent ( QPaintEvent * event );
    //void  addSubMenu(QMenu* pQMenu, const QString &subOptionsTitle, const QStringList &subOptionsList, const QFont &fGrd);
    void  setMomentMeal(const QString& optionName, int idMenu, int idOptionSub);
    void  controlMenuDayRepart(const QString& optionName, int idMenu, int idOption);
    void  controlMenuQSP(const QString& optionName, int idMenu, int idOptionSub);
    void  controlMenuQSP_Unit(const QString& optionName, int idMenu, int idOptionSub);
    void  controlMenuEveryTimeUnit(const QString &optionName, int idMenu, int);
    void  controlMenuEveryTimeValue(const QString &optionName, int idMenu, int);
    void  controlMenuVoie(const QString &optionName, int idMenu, int);

    void    setCheckBoxConnexionState(bool state);
    void    controlMenuSequence(const QString &optionName, int idMenu, int=0);
    void    setWeekDaysCheckBoxVisibility( bool visibility);
    void    setWeekDaysCheckBoxEnabled( bool isEnabled);
    int     setWeekDaysCheckBox(const QString &daysList, bool isInclude);
    void    setDureeControlsEnabled(bool state);
    QString weekDaysCheckBoxToString(char mode);
    QString adjustDecPartString(const QStringList &vList, double divide);
    void       set_doseTotale(const QString &dose){m_LineEdit_TotalJour->setText(dose);}
    QString    doseTotale(){return m_LineEdit_TotalJour->text();}
public:
    QPropertyAnimation *m_animation;

private:

    CMyColorButton    *m_check_MenuBut;
    QCheckBox         *m_check_Lundi;
    QCheckBox         *m_check_Mardi;
    QCheckBox         *m_check_Mercredi;
    QCheckBox         *m_check_Jeudi;
    QCheckBox         *m_check_Vendredi;
    QCheckBox         *m_check_Samedi;
    QCheckBox         *m_check_Dimanche;

    CMyQLineEdit      *m_EveryQuEdit;
    CMyColorButton    *m_EveryUnitBut;
    CMyColorButton    *m_EveryQuBut;

    CMyColorButton    *m_Button_Pendant;
    CMyQLineEdit      *m_LineEdit_Duree;
    CMyColorButton    *m_Button_DureeUnite;

    CMyQLineEdit      *m_LineEdit_TotalJour;
    CMyQLineEdit      *m_LineEdit_Forme;
    CMyColorButton    *m_Button_Repart;
    CMyColorButton    *m_Button_RepartRepas;

    CMyQLineEdit      *m_LineEdit_Voie;
    CMyColorButton    *m_Button_Popup_Voie;

    C_PopupDialPosoPoint *m_PopupPosoPoint_Matin;
    C_PopupDialPosoPoint *m_PopupPosoPoint_Midi;
    C_PopupDialPosoPoint *m_PopupPosoPoint_AMidi;
    C_PopupDialPosoPoint *m_PopupPosoPoint_Soir;
    C_PopupDialPosoPoint *m_PopupPosoPoint_Coucher;
    C_PopupDialPosoPoint *m_PopupPosoPoint_Nuit;

    CMyQLineEdit         *m_LineEdit_comm;

  QString                m_cur_grammar;
  C_BitMapCollection    *m_pC_BitMapCollection;
  QColor                 m_backGround;
  QColor                 m_borderColor;
  int                    m_marge;
  int                    m_buttonH;
  QFont                  m_fGrd;
  // C_PopupDialCorpoFact  *m_C_PopupDialCorpoFact;

public slots:
  void Slot_checkStateChanged( int );
  void Slot_MenuTriggered( QAction *pQAction  );
  void Slot_PosoPointDeployed(const QString &pointName);
  void Slot_PosoPointChanged(C_PopupDialPosoPoint*);
  void Slot_textChanged(const QString &, CMyQLineEdit *, void *);
signals:
  void Sign_contentChanged(C_Frm_Sequence*);
  void Sign_PatientCtxChanged(C_PatientCtx *pC_PatientCtx);
};

class C_Frm_PosologieHitList;
//======================== C_Wdg_Stack_Sequence ========================================================================
class C_Wdg_Stack_Sequence : public QStackedWidget  //, C_PosologieGrammar
{
    Q_OBJECT
public:
    C_Wdg_Stack_Sequence (QRect                     geometry_rect                             ,  // c'est le parent qui decide de la geometrie
                          C_Frm_PosologieHitList  * pC_Frm_PosologieHitList                   ,  // le pere
                          const QString           & objectName      = "C_Wdg_Stack_Sequence"  ,
                          C_Frm_Sequence          * pC_Frm_Sequence = 0                          // le premier widget
                         );
    ~C_Wdg_Stack_Sequence(){}
     QString         interfaceToGrammar(QStringList &formesList , QStringList &voiesList);
     void            grammarToInterface( const QString &grammar);
     void            setBackGround(const QColor &backGroundColor);
     C_Frm_Sequence *sequenceWidget(int index);
     C_Frm_Sequence *sequenceAdd(QRect rect, C_Frm_PosologieHitList *pC_Frm_PosologieHitList );
     int             sequenceRemove(int index = -1);
     void            sequenceAllRemove();

 signals:
     void Sign_contentChanged(C_Frm_Sequence*);
     void Sign_PatientCtxChanged(C_PatientCtx *pC_PatientCtx);

 private:
     C_Frm_PosologieHitList *m_pC_Frm_PosologieHitList;
};

class C_Frm_Produit;
class C_Frm_Posologie;
class C_PopupDial_IndicationChoice;
class C_PopupDial_TerrainChoice;
//====================================== C_Frm_PosologieHitList =======================================================
class C_Frm_PosologieHitList : public C_PopupDial
{
Q_OBJECT
//........................................... METHODES ...........................................................
public:
    enum direction {
                      NEXT     = 0,
                      PREVIOUS = -1
                   };

     C_Frm_PosologieHitList (const QStringList   &grammarHitList,
                             const QString       &grammar_in,
                             C_Frm_Posologie     *pC_Frm_Posologie ,             // la posologie en cours
                             const QRect          &geometry_rect  ,
                             int                  h_button        ,
                             const QColor        &backGroundColor,
                             C_BitMapCollection  *pC_BitMapCollection, // il nous faut des icones
                             C_BDM_Api           *pC_BDM_Api,          // un pointeur sur l'API medicamenteuse
                             QWidget*             parent    ,
                             const char*          name      ,
                             QList<C_CompositionItem> *p_compositionList
                            );
     //-------------------------------- adjustFavoriToGrammar -------------------------------------------------------------------------------------------------------
     /*! \brief when a favorite posology is choosed, it is necessary to adapt the weight/surface dependant doses
      *  with patient context data, and modify grammar with modified values.
      * \note this method change inplicit m_cur_grammar member
      * \param grammar  input grammar to adapt t corporeal factor (weight/surface)
      * \return modified grammar
      */
     QString             adjustFavoriToGrammar(const QString &grammar);
     void                grammarToInterface(QString grammar);
     void                clearInterface();
     QString             grammar() {return m_cur_grammar;}
     QString             interfaceToGrammar();
     void                setDuration( const QString &duration_syntax );
     void                setDCI_Mode(int mode);
     void                indication_stringToInterface(const QString &indic_str);
     QString             interfaceToIndication_string();
     QColor              background()        {return m_background_color;}
     C_BitMapCollection *bitMapCollection()  {return m_pC_BitMapCollection;}
     int                 marge   ()          {return m_marge;}
     int                 buttonH ()          {return m_buttonH;}
     QFont               font()              {return m_fGrd;}
     int                 literaliteToInt();
     void                setBackGround(QColor backGroundColor);
     void                set_ald(const QString &ald);
     void                moveY(int y);
     QString             cip() {return m_cip;}
     C_BDM_Api          *c_bdm_api(){return m_pC_BDM_Api;}
     static  QString    compositionToXml( const QList<C_CompositionItem> &compositionList, const QString &id_type );

 private:
     C_Frm_Produit   *c_frm_produit();      // TODO dependance a virer
     C_Frm_Posologie *c_frm_posologie();    // TODO dependance a virer
     //............. la liste des favoris ......................
     void               clearHitList();
     int                loadHitList(int *nbFromHit = 0);
     CMyQTextEdit      *setNext_or_PreviousEdit_Widget(int sens);
     CMyQTextEdit      *setWidgetEditFocus(CMyQTextEdit *pQWidgetNew, CMyQTextEdit *pQWidgetOld);
     void               editWidgetFavorisToInterface(CMyQTextEdit *pCMyQTextEdit);
     void               setWidgetEditFocus(int direction);
     QStringList        getGrammarListFromIntemList();
     CMyQTextEdit      *addFavoriWidgetItem(const QString &grammar, int index);
     void               setItemListBackGround(CMyQTextEdit *pQWidgetNew, bool isSelected);
     QString            computeTextHit(const QString &grammar, int index );
     int                isExistsThisHit( const QString  &grammar);
     void               indicationChoice(CMyQLineEdit *pCMyQLineEdit);
     void               terrainChoice(CMyQLineEdit *pCMyQLineEdit);
     QString            compositionToXml(QList<C_CompositionItem> *pCompositionList);
     //.............. les evenements .............................
     bool     event ( QEvent * ev );
     void     mousePressEvent ( QMouseEvent * event );
     void     paintEvent ( QPaintEvent * event );
     //......... la gestion des sequences .....................
     C_Frm_Sequence  *sequenceAdd();
     void             adjustEnableStateStackButton();
     void             activeButtonSeq(int index);
public slots:
     void Slot_HitListMenuTriggered( QAction *pQAction  );
     void Slot_Button_Button_Terrain_Clicked(bool);
     void Slot_ButtonIndic_Clicked(bool);
     void Slot_Button_Del_Fav_Clicked(bool);
     void Slot_Button_Add_Fav_Clicked(bool);
     void Slot_ButtonHitClose_Clicked();
     void Slot_ButtonHitClose_Clicked(bool);
     void Slot_HitItem_mousePressEvent   ( QMouseEvent *e, void *ptr );
     void Slot_HitItem_mouseDoubleClicked( QMouseEvent *e, void *ptr );
     void Slot_On_ButtonSeq_Clicked     ( const QString &buttonName);
     void Slot_On_ButtonSequenceAdd_Clicked(bool);
     void Slot_On_ButtonSequenceDel_Clicked(bool);
     void Slot_textChanged(const QString &, CMyQLineEdit *, void *);
     void Slot_SequenceChanged(C_Frm_Sequence*);
     void Slot_PopupDialDateChanged(C_PopupDialDate*);
     void Slot_PopupDialCorpoFactChanged(C_PopupDialCorpoFact*pC_PopupDialCorpoFact);
     // void Slot_On_UpKeyEdit(const QString &name, CMyQLineEdit *pCMyQLineEdit, void *ptr);
     // void Slot_On_DwKeyEdit(const QString &name, CMyQLineEdit *pCMyQLineEdit, void *ptr);
signals:

     void Sign_HitList_ButtonDelete     ( const QString &grammar, int index );
     void Sign_HitList_ButtonAppend     ( QString &grammar );
     void Sign_ButtonHitCloseClicked    ( );
     void Sign_UpdateInteractions       ( const QString &grammar );
     void Sign_contentChanged           ( C_Frm_PosologieHitList* );
     void Sign_FavorisAdded             ( );
     void Sign_FactCorpoChanged         ( C_PatientCtx *pC_PatientCtx );
     void Sign_PatientCtxChanged        ( C_PatientCtx *pC_PatientCtx );

//........................................... DATA ...............................................................
private:
  QString             m_status;             /*!< proprietes type F/traitement de fond et autres a venir> */
  QString             m_cip;
  QString             m_idType;
  QListWidget        *m_pQListWidgetFavoris;
  int                 m_Grid_H;
  int                 m_marge;
  int                 m_buttonH;
  QString             m_cur_grammar;      /*!< grammaire de la posologie en cours */
  CMyQTextEdit       *m_FocusWidget;
  QColor              m_background_color;

  CMyQLineEdit       *m_LineEditNote;
  CMyQLineEdit       *m_LineEditIndic;
  CMyQLineEdit       *m_LineEditTerrain;

  CMyButton          *m_Button_Seq_0;
  CMyButton          *m_Button_Seq_1;
  CMyButton          *m_Button_Seq_2;
  CMyButton          *m_Button_Seq_3;
  CMyButton          *m_Button_Seq_4;
  CMyButton          *m_Button_Seq_5;
  CMyButton          *m_Button_Seq_6;
  CMyButton          *m_Button_Seq_7;
  CMyButton          *m_Button_Seq_8;
  CMyButton          *m_Button_Seq_9;
  CMyButton          *m_Button_Seq_plus;
  CMyButton          *m_Button_Seq_del;
  CMyButton          *m_Button_Note;
  CMyButton          *m_Button_Indic;
  CMyQLineEdit       *m_LineEdit_IndicRefund;
  CMyButton          *m_Button_IndicType;
  CMyButton          *m_Button_Terrain;


  CMyButton             *m_Button_Add_Fav;
  CMyButton             *m_Button_Del_Fav;
  CMyButton             *m_ButtonHitClose;

  C_PopupDialDate              *m_C_PopupDialDate;
  C_PopupDialCorpoFact         *m_C_PopupDialCorpoFact;

  C_Wdg_Stack_Sequence         *m_pC_Wdg_Stack_Sequence;
  C_BitMapCollection           *m_pC_BitMapCollection;
  QFont                         m_fGrd;
  C_BDM_Api                    *m_pC_BDM_Api;
  C_Frm_Posologie              *m_pC_Frm_Posologie;
  C_PopupDial_IndicationChoice *m_pC_PopupDial_IndicationChoice;
  C_PopupDial_TerrainChoice    *m_pC_PopupDial_TerrainChoice;
  QList<C_CompositionItem>     *m_pCompositionList;
};

class C_Frm_Produit;
//======================== C_Frm_Posologie =============================
class C_Frm_Posologie : public QFrame //, C_PosologieGrammar
{
    Q_OBJECT

public:
    enum mode_create {
        READ_HTML_IN_GRAMMAR_CACHE  = 0,
        OPEN_POSO_EDITOR            = 1,
        GENERATE_HTML_FROM_GRAMMAR  = 2
    };
    C_Frm_Posologie(  C_BDM_Api           *pC_BDM_Api,          // pointeur sur l'API medicamenteuse
                      C_Frm_Produit       *pC_Frm_Produit,      // on differencie le produit a traiter du parent (permet d'avoir pour parent n'importe quel autre objet)
                      C_BitMapCollection  *pC_BitMapCollection, // il nous faut des icones
                      QRect                geometry_rect,       // c'est le parent qui decide de la geometrie
                      QWidget             *parent,              // le pere
                      int                  isWithHitList        // visibilite de la grille horizontale
                     );
    ~C_Frm_Posologie();
    void     paintEvent        ( QPaintEvent * event );
    void     mousePressEvent   ( QMouseEvent * event);
    void     mouseReleaseEvent ( QMouseEvent * event );
    void     mouseMoveEvent    ( QMouseEvent * event );
    void     mouseMoveEvent    ( QMouseEvent * event , QWidget *pQWidget );
    int      timeToPos_x(int nb_mn);
    bool     is_HitListDeploy();
    void     clearInterface();
    void     adjustToSize(const QRect &geometry_rect);
    void     updateNumOrdreInHtmlView( QString numOrdre );
    QString  numOrdre();
    QString  serialize(const QString &ofset ="");
    QString  toHtml(const QString &grammar, const QColor &background);
    void     set_ald(const QString &ald, const QColor &backGroundColor);
    QString  grammar(int complete = 1 );
    QStringList              getHitsList();
    void                     open_HitList();
    void                     close_HitList(int fast = 0);
    C_Frm_Produit           *c_frm_produit() { return m_pC_Frm_Produit; }
    C_Frm_PosologieHitList  *get_HitList()   { return m_HitList; }
    void                     setDuration( const QString &duration_syntax );
    void                     setDCI_Mode(int mode);
    QTextEdit            *m_textEdit;
    int                   m_buttonH;
    QPropertyAnimation   *m_animation;
    QList <CMyQLineEdit*> m_ListEnterFocusEdit;
    QString               m_grammar;
private:

  int             m_Hdeb_mn;
  int             m_Hfin_mn;
  int             m_first_time;
  int             m_grad_pos_y;
  int             m_grad_w;
  int             m_GrabIsOn;
  int             m_startPosY;

  QList<C_CompositionItem> m_compositionList;
  C_BDM_Api               *m_pC_BDM_Api;
  C_Frm_Produit           *m_pC_Frm_Produit;
  C_BitMapCollection      *m_pC_BitMapCollection;
public :
  C_Frm_PosologieHitList  *m_HitList;

public slots:
  void Slot_close_HitList();
  void Slot_adjustToSize();
  void Slot_HitList_contentChanged( C_Frm_PosologieHitList *pC_Frm_PosologieHitList );
  void Slot_UpdateInteractions(const QString &grammar);
  //------------------------------------ Slot_PatientCtxChanged ------------------------------------------------
  /*! \brief this Slot is called when to notify that pC_PatientCtx contextis changed
   *  \param C_PatientCtx *pC_PatientCtx.  changed context
   */
  void            Slot_PatientCtxChanged(C_PatientCtx *);

signals:
  void Sign_child_LeaveLastEditZone();
  void Sign_FavorisAdded();
  void Sign_HitList_Opened(C_PopupDial *pC_PopupDial);
  void Sign_UpdateInteractions(const QString &grammar);
  void Sign_FactCorpoChanged ( C_PatientCtx *pC_PatientCtx);
  void Sign_PatientCtxChanged( C_PatientCtx *pC_PatientCtx);
  void Sign_contentChanged( C_Frm_Produit*);

};

//======================== C_Frm_Produit =============================
class C_Frm_Produit : public QFrame, public C_OrdoLineRecord
{
    Q_OBJECT

public:
    enum mode {
        NO_GRAB,
        BOTTOM_START,
        TOP_START,
        MIDLE_START
    };

    C_Frm_Produit(  C_Frm_Produit     const  &c_Frm_Produit);
    C_Frm_Produit(  C_OrdoLineRecord  const  &ordoLineRecord ,
                    C_BDM_Api           *pC_BDM_Api,
                    C_BitMapCollection  *pC_BitMapCollection,
                    QRect                geometry_rect,
                    QWidget             *parent       ,
                    const QString       &background_noALD = "#ffe3be",
                    const QString       &background_isALD = "#f5ffbe",
                    int                  isWithHitList    = 1   // ouvrir la HitList a la creation
                   );
    ~C_Frm_Produit();

    void              paintEvent        ( QPaintEvent * event );
    void              mousePressEvent   ( QMouseEvent * event);
    void              mouseReleaseEvent ( QMouseEvent * event );
    void              mouseMoveEvent    ( QMouseEvent * event );
    void              mouseMoveEvent    ( QMouseEvent * event , QWidget *pQWidget );
    void              move(int x, int y);
    C_OrdoLineRecord  ordoLineRecord() const;
    void              setOrderNumber(int num);
    bool              isInDragAction() {return m_GrabIsOn;}
    QColor            backGroundColor(){QColor                background_color = ald().length()?m_background_isALD:m_background_noALD;
                                        if (isInDragAction()) background_color = background_color.darker(110);
                                        return                background_color;
                                       }
    void              adjustToSize(const QRect &rect);
    void              setColors(const QString &_std_color, const QString &_ald_color);

    void              set_ald(const QString &ald);

    bool              is_HitListDeploy();
    void              close_HitList(int fast=0);
    void              open_HitList();
    double            getTotalPrice();
    void              setDuration( const QString &duration_syntax );
    QString           serialize(const QString &ofset = "");
    QString           grammar();
    bool              isToDeleteLater(){ return m_deleted;}
    void              setToDeleteLater(){m_deleted=true;}
    //............ public members datas ..............................
    CMyButton          *m_ButtonDelete;
    CMyButton          *m_ButtonNumber;
    C_Frm_Posologie    *m_pC_Frm_Posologie;

private:
    int                 m_GrabIsOn;
    int                 m_startPosY;
    QColor              m_background_isALD;
    QColor              m_background_noALD;
    CMyButton          *m_ButtonNoSecure;
    C_BDM_Api          *m_pC_BDM_Api;
    C_BitMapCollection *m_pC_BitMapCollection;
    bool                m_deleted;

public slots:
  void              Slot_FavorisAdded();
  void              Slot_focusChanged(QWidget * old, QWidget * now)  ;
  void              Slot_ButtonNoSecure(const QString&);
  void              Slot_UpdateInteractions(const QString &grammar);
signals:
  void              Sign_FavorisAdded(C_Frm_Produit*);
  void              Sign_UpdateInteractions(const QString &grammar);

};

class C_InteractionsThread;
class C_QWebView;
//============================= C_Frm_Prescription ==============================================
class C_Frm_Prescription : public QFrame , public C_BitMapCollection
{

 Q_OBJECT
public:
    enum mode {
        ALL_POSITIONS_BELOW_Y     = -1,
        JUST_ONE_POSITION_UNDER_Y =  0,
        ALL_PRODUCTS              =  1,
        CIP_PRODUCTS              =  2,
        MATCH_ALD_STATUS          =  3
    };
    enum mode_eval {
        EVALUATE_INTERACTIONS        = 1,
        DO_NOT_EVALUATE_INTERACTIONS = 0
    };

    C_Frm_Prescription(const QString &imagePath, C_BDM_Api *pC_BDM_Api , QScrollArea *pQScrollArea , QWidget *parent=0 );
    ~C_Frm_Prescription();
    QStringList              grammarList(const QString &toMatch = "", int matchWith  = C_Frm_Prescription::ALL_PRODUCTS );
    void                     setGrammarList(const QStringList &grammarList, int interactionsMustBeeEvaluated = C_Frm_Prescription::EVALUATE_INTERACTIONS );
    bool                     event ( QEvent * e );
    void                     paintEvent    ( QPaintEvent *event );
    void                     mouseMoveEvent( QMouseEvent *event );
    int                      adjust_Vertical_PositionToGrid(C_Frm_Produit *pC_Frm_Produit  = 0, bool scroll_if_out_limits=false);
    int                      adjust_Vertical_PositionToGrid(int y, C_Frm_Produit *pC_Frm_Produit  = 0 );
    void                     objectStartsMoving(C_Frm_Produit *pC_Frm_Produit);
    void                     objectDoMoving(C_Frm_Produit *pC_Frm_Produit);
    void                     objectStopsMoving(C_Frm_Produit *pC_Frm_Produit);
    void                     deplace_Objects(int y_ref, int ofset_y);
    void                     rearangeAllNumberOrder(QList<C_Frm_Produit*> *pListProduitExcluded   = 0 );
    int                      count_ALD(int *nb_Prd = 0 );
    int                      count_GridToDraw(int *nb_ald_toRet  = 0 );
    bool                     is_posY_in_ALD_Zone();
    bool                     is_posY_in_ALD_Zone(int y);
    void                     echoListObjects(const QString txt);
    void                     setZoom(int steep);
    void                     setDCI_Mode(int mode);
    int                      nextFreePosY ( bool not_ALD );
    C_Frm_Produit *          appendNewObject( const QString &xmlData, bool not_ALD ,
                                              int isWithHitList                 = C_Frm_Posologie::OPEN_POSO_EDITOR,
                                              int interactionsMustBeeEvaluated  = C_Frm_Prescription::EVALUATE_INTERACTIONS);
    C_Frm_Produit *          appendNewObject( const QString &xmlData,    int pos_y ,
                                              int isWithHitList                 = C_Frm_Posologie::OPEN_POSO_EDITOR,
                                              int interactionsMustBeeEvaluated  = C_Frm_Prescription::EVALUATE_INTERACTIONS);
    C_Frm_Produit *          appendNewObject( C_OrdoLineRecord ordoLine, int pos_y ,
                                              int isWithHitList                 = C_Frm_Posologie::OPEN_POSO_EDITOR,
                                              int interactionsMustBeeEvaluated  = C_Frm_Prescription::EVALUATE_INTERACTIONS);
    void                     set_Ald_On_All_Products(const QString &ald);
    void                     delete_All_Products(int interactionsMustBeeEvaluated  = C_Frm_Prescription::EVALUATE_INTERACTIONS );
    void                     set_Duration_On_All_Products(const QString &duration_syntax);
    //------------------------------------ createProduct ------------------------------------------------
    /*! \brief create a product widget
     *  \param  const C_OrdoLineRecord &ordoLine  datas
     *  \param  int pos_y  position
     *  \param  int isWithHitList  1/if HitList must be created 0/in not
     *  \return C_Frm_Produit* a pointer on created product 0 if not
     */
    C_Frm_Produit           *createProduct(const C_OrdoLineRecord &ordoLine, int pos_y, int isWithHitList  = C_Frm_Posologie::OPEN_POSO_EDITOR  );
    QList <C_Frm_Produit*>   idExists(const QString &id);
    QList <C_Frm_Produit*>   C_Frm_ProduitList();
    QStringList              cipList();
    QSize                    getChildsSize();
    void                     setChildsSize(int childs_w, int childs_h);
    void                     setChildsColors(const QString &std_color, const QString &ald_color);
    void                     setPixmapNonSubstituable(const QPixmap &pixmapNonSubstituable) {m_NonSubstituable = pixmapNonSubstituable;}
    void                     displayInfo(const  QString &info);
    QString                  serialize(const QString &ofset = "");

    QRect                    get_displayInteractionWindowGeometry();
    //------------------------------------ set_Thread_DBM_Plugin ------------------------------------------------
    /*! \brief set user BDM plugin interface (class interface for unified commercial database).
     *  \param  const QString &_namePlugin name of plugin (Datasemp, Theriaque ...)
     *  \param int oldMustBeDeleted
     *         if 1 the instance plugin must be deleted an a new instance created (not only get a already instancied in m_pC_BDM_PluginI_Map)
     *         if 0 get if exists a already instancied in m_pC_BDM_PluginI_Map(faster)
     */
    void                     set_Thread_DBM_Plugin(const QString &_namePlugin, int oldMustBeDeleted = 0);
    QString                  evaluateAllInteractions();
    void                     display_HAS_Status();
    //------------------------------------ close_All_HitList ------------------------------------------------
    /*! \brief  close all m_HitList.
     */
    void  close_All_HitList(int fast  = 0 );
    //------------------------------------ get_HitListDeployed ------------------------------------------------
    /*! \brief return the deployed m_HitList.
     *  \return  C_PopupDial *pC_PopupDial.  deployed dialog zeron only
     */
    C_PopupDial *get_HitListDeployed();
    //------------------------------------ objectListAt ------------------------------------------------
    /*! \brief return objects list under y_ref to y_end positions
     *  \param  y_ref start Y position.
     *  \param  y_end end   Y position. if -1 then alls positions below y_ref, if zero just one position under y_ref
     *  \param  QList<C_Frm_Produit*> *pListProduitExcluded if not zero objects from this list will be excluded from result list.
     *  \return QList <C_Frm_Produit*>  result list
     */
    QList <C_Frm_Produit*>   objectListAt(int y_ref, int y_end = 0, QList<C_Frm_Produit*> *pListProduitExcluded   = 0 );
    //------------------------- gestion du drag & drop ---------------------------------------
    void                     dropEvent(       QDropEvent      *event );
    void                     dragEnterEvent(  QDragEnterEvent *event );
    void                     dragMoveEvent(   QDragMoveEvent  *event );
    void                     dragLeaveEvent ( QDragLeaveEvent *event );
    QString                  get_DropedXmlDatas()                        {return m_XmlDatas;}
    QString                  dragGetListDrugsName();
    void                     set_DropedXmlDatas(const QString &xmlDatas) {m_XmlDatas=xmlDatas;}
    void                     clear_DropedXmlDatas()                      {m_XmlDatas="";}

private:

    C_BDM_Api            *m_pC_BDM_Api;                    /*!< pointeur sur C_BDM_Api api des la base de donnees medicamenteuse */
    QScrollArea          *m_pQScrollArea;                  /*!< pointeur sur l'objet QScrollArea (faire scroller lors deplacements des C_Frm_Produit)  */
    QString               m_str_background_isALD;          /*!< couleur par defaut des objets enfants ald */
    QString               m_str_background_noALD;          /*!< couleur par defaut des objets enfants non ald */
    QPoint                m_startDragPos;                  /*!< position de demarrage lors deplacementC_Frm_Produit        */
    int                   m_lastDragPosY;                  /*!< derniere position en cours lors deplacementC_Frm_Produit   */
    QString               m_mimeType;                      /*!< type donnees recues lors du drag & drop     */
    QString               m_XmlDatas;                      /*!< donnees xml recues lors du drag & drop      */
    QString               m_mimeTypeIconPath;              /*!< chemin des icones                 (definie) */
    QTextEdit            *m_infoTexEdit;                   /*!< fenetre des interractions */
    QLabel               *m_inWorks;                       /*!< fenetre du travail en cours */
    int                   m_Nb_Grid_H;                     /*!< nombre de cases maximum           (definie) */
    int                   m_SpaceBetweenProduits;          /*!< marge entre deux case             (definie) */
    int                   m_C_Frm_Produit_W;               /*!< hauteur d'un objet C_Frm_Produit  (definie) */
    int                   m_C_Frm_Produit_H;               /*!< largeur d'un objet C_Frm_Produit  (definie) */
    int                   m_Grid_H;                        /*!< hauteur d'une case (calculee avec m_C_Frm_Produit_H + m_SpaceBetweenProduits) */
    C_PopupDial          *m_HitList_deployedBeforeMoving;  /*!< stocke si l'objet etait deploye avant son deplacement  */
    C_InteractionsThread *m_pC_InteractionsThread;         /*!< pointeur sur le thread d'interactions  */
    int                   m_RequestInteractEval;           /*!< si a 1 une nouvelle evaluation doit se faire en sortie de thread  */
    QMutex                m_mutex;                         /*!< pour la synchro du thread interraction */
    int                   m_dci_mode;                      /*!< mode de DCI par defaut COM=0, COM_DCI=1, DCI_COM=2, DCI=3  */
    //QPointer<C_Frm_Produit>  m_pC_Frm_ProduitDrag;
public slots:
    void set_notificatorView(QTextEdit *pQTextEdit );
    QTextEdit      *notificatorView();
    void            Slot_close();
    void            Slot_UpdateInteractions(const QString &grammar);
    void            Slot_evaluateAllInteractions();
    void            Slot_InteractionsThread_finished();
    void            Slot_Drop_Prd();
    void            Slot_ButtonDeleteClicked(const QString&,  void*);
    void            Slot_ButtonNumberClicked(const QString&,  void*);
    void            Slot_FavorisAdded(C_Frm_Produit *pC_Frm_Produit);
    void            Slot_contentChanged(C_Frm_Produit *);
    //------------------------------------ Slot_HitListDeployed ------------------------------------------------
    /*! \brief Slot is called when a m_HitList is deployed to close all other m_HitList.
     *  \param  C_PopupDial *pC_PopupDial.  deployed dialog (only this one must be open)
     */
    void            Slot_HitListDeployed(C_PopupDial *pC_PopupDial);
    //------------------------------------ Slot_PatientCtxChanged ------------------------------------------------
    /*! \brief this Slot is called when to notify that pC_PatientCtx contextis changed
     *  \param C_PatientCtx *pC_PatientCtx.  changed context
     */
    void            Slot_PatientCtxChanged(C_PatientCtx *);
signals:
    void Sign_PatientCtxChanged(C_PatientCtx *pC_PatientCtx);
    void Sign_dragEnterEvent();
    void Sign_child_LeaveLastEditZone();
    void Sign_ProductNumberClicked( C_Frm_Produit* );
    void Sign_PrescriptionWantGeometry(QRect &,QRect &); /*!< rectangle application, rectangle widget si connu (membres a -1 si non definis)  */
    void Sign_FavorisAdded(C_Frm_Produit*);
    void Sign_FactCorpoChanged( C_PatientCtx *pC_PatientCtx );
    void Sign_UpdateTotalPrice(  const QString &total_price );
};


#endif // C_FRM_PRESCRIPTION_H

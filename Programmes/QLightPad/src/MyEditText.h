/*************************** MyEditText.h **********************************
 *   #include "MyEditText.h"                                               *
 *   Copyright (C) 2003 by Sevin Roland  for DrTux by Medi'n Tux           *
 *   upsis@club-internet.fr                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef MYEDITTEXT_H
#define MYEDITTEXT_H

#include <qobject.h>
#include <qwidget.h>
#include <qtextedit.h>
#include <qimage.h>
#include <qfont.h>
#include <qcolor.h>
#include "../../MedinTuxTools/CCoolPopup.h"

#ifdef FOR_MEDINTUX

 #include <qpopupmenu.h>
 #define TOOL_TIP_WIDTH  400
 #define TOOL_TIP_HEIGHT 330


 #ifdef Q_WS_WIN
    #define PROG_NAME "drtux.exe"
    #define F_EXT  ".exe"
    #define INSERTION_ROOT            "\\Champs d'insertions"
    #define MASQUES_OBSERV_ROOT       "\\Observation"
    #define MASQUES_PRESCR_ROOT       "\\Prescription"
    #define MASQUES_PRESCR_CERTIF     "\\Prescription\\Certificats\\"
    #define MASQUES_PRESCR_ORDO       "\\Prescription\\Ordonnances\\"
    #define MASQUES_PRESCR_COURRIER   "\\Prescription\\Courriers\\"
    #define CHEMIN_CIM10_URG          "\\CIM10Urg"
    #define CHEMIN_TERRAIN            "\\Terrain"
#else
    #define PROG_NAME "drtux"
    #define F_EXT  ""
    #define INSERTION_ROOT            "/Champs d'insertions"
    #define MASQUES_OBSERV_ROOT       "/Observation"
    #define MASQUES_PRESCR_ROOT       "/Prescription"
    #define MASQUES_PRESCR_CERTIF     "/Prescription/Certificats/"
    #define MASQUES_PRESCR_ORDO       "/Prescription/Ordonnances/"
    #define MASQUES_PRESCR_COURRIER   "/Prescription/Courriers/"
    #define CHEMIN_CIM10_URG          "/CIM10Urg"
    #define CHEMIN_TERRAIN            "/Terrain"
#endif

 //====================================== CMyQPopupMenu =======================================================
 class CMyQPopupMenu : public  QPopupMenu
 {
  Q_OBJECT
  //........................................... METHODES ...........................................................
  public:
      CMyQPopupMenu ( QWidget * parent = 0, const char * name = 0 );
  protected:
      bool         event ( QEvent * e );
  signals:
   void Sign_MouseOutPopup();
 };


 //====================================== CMyBulleWidget =======================================================
 class CMyBulleWidget : public  QWidget
 {
  Q_OBJECT
  //........................................... METHODES ...........................................................
  public:
      CMyBulleWidget (const QString  &text    ,
                     int            wAdj      ,
                     int              x       ,
                     int              y       ,
                     const QString &pathDrTux ,
                     QWidget    *parent = 0   ,
                     const char   *name = 0   ,
                     WFlags           f = 0   ,
                     int          width  = TOOL_TIP_WIDTH    ,
                     int          height = TOOL_TIP_HEIGHT   ,
                     int          zoom  = 8
                    );
     //~CMyBulleWidget ();
      QTextEdit* m_pQTextEdit;
 protected:


 };
#endif

//====================================== MyEditText =============================================================
class MyEditText : public QTextEdit
{

 Q_OBJECT

 public:
   //................... constructeurs et destructeurs....................
   MyEditText( const QString & text,
               const QString & pathGlossaire,
               const QString & pathDrTux,
               QStringList   * pInsertFieldList=0,
               const QString & context   = QString::null,
               QWidget       * parent    = 0,
               const char    * name      = 0);
   MyEditText( const QString & pathGlossaire,
               const QString & pathDrTux,
               QStringList   * pInsertFieldList=0,
               QWidget       * parent    = 0,
               const char    * name      = 0);
   virtual ~MyEditText(){
                        }

  //...................  methodes ........................................
    void                 initDefaultVar(const QString & pathGlossaire, const QString & pathDrTux, QStringList *pInsertFieldList=0);
    virtual bool         event ( QEvent * e );
    virtual void         moveCursor ( int action, bool select =0);

  //...................  DATAS ........................................
  QString  m_Path;
  QString  m_PathAppli;
  QString  m_PathImage;
  QString  m_DefaultTextColor;
  QString  m_DefaultBackgroundColor;
  QFont    m_DefaultFont;
  QString  m_Filter;
  int      m_IsUTF8;
#ifdef FOR_MEDINTUX


   enum insertMode {
        noAddHtmlHeader,
        AddHtmlHeader,
        HtmlConvert,
        HtmlConvertSp
     };
   //...................  methodes ........................................

   QPopupMenu    *createPopupMenu ( const QPoint & pos );
   QString        GetLastList(int ori_index=-1,  int ori_para=-1, QPoint *pos=0);
   int            GetListItem (QPopupMenu *pQPopupMenu, QString &list_name , int list_type=0);
   void           InsertString(const char *str, const char* prefix  = 0 );
   void           EmuleReturnKeyPress();
   void           htmlConvert ( QString &htmlStr, int flag = MyEditText::HtmlConvert , const QTextEdit *pQTextEdit =0);
   void           InitParam();
   bool           IsThisFileExist(const char* path );
   void           SetTypeRubrique(int rub_type){m_TypeRubrique = rub_type; };
   int            DialogGetString(QWidget * parent, const char* title, QString &string);
   int            MessageAlert(QWidget * parent, const char* title, const char* message, const char* button1=0, const char* button2=0,const char* button3=0,const char* button4=0);
   QString        qstringLoad(const char *path, int flag=0, long *nb_ligne=0);
   bool           IsThisFolderExist(const char *path, const char* folderName );
   void           specialInsert(QString &str);
   //...................  datas ...........................................
   QString         m_list_name;
   QString         m_style;
   QString         m_local_param;
   int             m_State;
   QStringList    *m_pInsertFieldList;
   QFont           m_LastFont;
   QColor          m_LastColor;
   QString         m_PathGlossaire;
 public slots:
      void    CouCou();
      void    setText ( const char* ptr );
      void    setText ( const QString & txt );
      void    setText ( const QString & txt , const QString & context)
              {QTextEdit::setText (txt, context);
              };
      void    ListManager();
      void    PasteListItem ();
      void    PasteFolderItem ();
      void    PasteTextItemT ();
      void    PasteTextItemH ();
      void    PasteTextItem (const QString &ext);
      void    PasteFileItem(QString path);

      void    OnMenuAboutToHide ();
      void    OnMenuActivated (int id);
      void    OnMenuHighlighted (int id);
      int     DoContextListPopup();
      void    TimedDestroyMenuBulle();
      void    CouCouUp(CCoolPopup * pCCoolPopup);
      void    CouCouStop(CCoolPopup * pCCoolPopup);
      void    CouCouDestroy();
      void    PopupCreateList();
      void    PopupItemAddDocType();
      void    InsererChampInsertion();
      void    InsererZoneMenu();
      void    PopupItemAddItem();
      void    SetZoom(int zoom){m_Zoom=zoom; zoomTo(m_Zoom);};

 protected:
   int             m_Zoom;
   QString         m_ListItem;
   QString         m_PopupItemFusionItem;
   QString         m_PopupItemAddItem;
   QString         m_PopupItemAddDocType;
   QString         m_PopupListObserv;
   QString         m_PopupListPrescr;

   QString         m_PathDrTux;
   CMyQPopupMenu  *m_pQPopupMenu;
   CCoolPopup     *m_pCCoolPopup;
   CMyBulleWidget *m_pQtextMenuBulle;
   int             m_TypeRubrique;

 signals:
   void message(const QString&, int);
   void Sign_ExePlugin( QString&);
#endif
};

#endif

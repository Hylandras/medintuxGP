/********************************** MyEditText.h **********************************
 *                                                                                                          *
 *    #include "MyEditText.h"                                                                        *
 *    Project: MedinTux     and Data Medical Design                                            *
 *    Copyright (C) 2003 2004 2005 by Sébastien SIVERA & Sevin Roland                  *
 *    E-Mail: data.medical.design@club-internet.fr                                            *
 *                                                                                                          *
 *    This program is free software; you can redistribute it and/or modify            *
 *    it under the terms of the CeCILL Version 1 du 21/06/2004                            *
 *    License (GPL compatible)                                                                      *
 *                                        http://www.cecill.info/                                    *
 *    as published by :                                                                                *
 *                                                                                                          *
 *    Commissariat à l'Energie Atomique                                                          *
 *    - CEA,                                                                                              *
 *                                     31-33 rue de la Fédération, 75752 PARIS cedex 15.    *
 *                                     FRANCE                                                             *
 *    Centre National de la Recherche Scientifique                                            *
 *    - CNRS,                                                                                             *
 *                                     3 rue Michel-Ange, 75794 Paris cedex 16.                *
 *                                     FRANCE                                                             *
 *    Institut National de Recherche en Informatique et en Automatique                 *
 *    - INRIA,                                                                                            *
 *                                     Domaine de Voluceau, Rocquencourt, BP 105, 78153     *
 *                                     FRANCE                                                             *
 *                                                                                                          *
 *            This program is distributed in the hope that it will be useful,          *
 *            but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *            MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                         *
 *            CeCILL  License (GPL compatible) for more details.                            *
 *                                                                                                          *
 **********************************************************************************/
 #ifndef MYEDITTEXT_H
 #define MYEDITTEXT_H

 #include <qobject.h>
 #include <qwidget.h>
 #include <qtextedit.h>
 #include <qstring.h>
 #include <qsqldatabase.h>

 #include <qpainter.h>
 #include <qfileinfo.h>
 #include <qdir.h>
 #include <qfont.h>
 #include <qcolor.h>
 #include <qsimplerichtext.h>
 #include <qpalette.h>
 #include <qbuffer.h>

 #include "CApp.h"
 #include "Global.h"
 #include "../../MedinTuxTools/ThemePopup.h"
 #include "../../MedinTuxTools/Codecs/qmdcodec.h"

 #define TOOL_TIP_WIDTH  450
 #define TOOL_TIP_HEIGHT 330
/*! Macro : Séparateur d'items pour le cache */
 #define HISTORIQUE_SPLITTER "{&}"
/*! Macro : Définition d'un répertoire dans le système de cache */
 #define ITEM_IS_FOLDER ";F;"
/*! Macro : répertoire de pour le cache des menus */
 #define DIR_CACHE_LISTES "cache"
/*! Macro : nombre maximal de menus imbriqués à présenter. */
 #define NBMAXSOUSMENU 10

 //====================================== CMyThemePopup =======================================================
 class CMyThemePopup : public  ThemePopup
 {
  Q_OBJECT
  //........................................... METHODES ...........................................................
  public:
        CMyThemePopup ( QWidget * parent = 0, const char * name = 0 );
 protected:
        bool            event ( QEvent * e );
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
                            int                wAdj     ,
                            int                  x      ,
                            int                  y      ,
                            QWidget     *parent = 0 ,
                            const char    *name = 0 ,
                            WFlags              f = 0 ,
                            int             width  = TOOL_TIP_WIDTH     ,
                            int             height = TOOL_TIP_HEIGHT    ,
                            int             zoom  = 9
                          );
      //~CMyBulleWidget ();
        QTextEdit* m_pQTextEdit;
 protected:


 };




 //====================================== MyEditText ===================================================
/*! \class MyEditText
 *  \brief Réimplémentation de la classe QEditText simple éditeur de texte de Qt pour permettre l'utilisation des menus multi-veaux (entr'autres).
 *
 * <b>A comprendre :</b> \n \n
 * 1) Chaque rubrique (Observation, Prescription, Documents, Images) a sa propre liste de menus par défaut qui sera à chaque fois présentée. Ces listes sont définis dans le fichier ini de drtux (ou manager si vous démarrez drtux depuis manager).\n \n
 * 1bis) les liste de menus sont repérable par la couleur de leur police de caractère (bleu).\n \n
 * 2) Les menus sont stockés sur le disque dur dans le répertoire Glossaire Champs d'insertion (accès par INSERTION_ROOT). Lorsqu'il s'agit d'un répertoire un niveau supplémentaire est crée, s'il s'agit d'un fichier contenant du texte (rtf, txt ou html) un item complexe est inséré dans le menu. Sinon le fichier ne contient rien (aucune extension au fichier) alors il s'agit d'un item qui sera inséré tel quel dans le menu et texte.\n \n
 * 2bis) Mode d'insertion dans le texte : Les répertoires (liste de menus) sont insérés dans le texte en BLEU, les items simple sont insérés tel quel, les fichiers composés sont insérés après interprétation du script contenu dans ce même texte.\n \n
 * 3) Les fichiers de remplacement : lorsqu'il s'agit d'un répertoire (d'un niveau) si ce même répertoire contient un fichier nommé liste_remplacement.html alors ce fichier sera inséré à la place du nom du répertoire. Ce qui permet une très grande flexibilité dans la construction de votre arborescence. Cette fonction peut être utilisée dans liste nommée "Motif de consultation" dont les premiers niveaux représente la spécialité concerné par le motif, le fichier liste_remplacement.html contient alors une fonction de script {{::intitule(spécialité, change)}} qui permet de modifier en temps réel le nom du document. Le nom du répertoire est inséré en bleu dans ce fichier de remplacement et le tour est joué.\n \n
 * 4) Pour gagner un maximum de temps, un fichier cache est construit pour chacunes des listes utilisées au moment de leur premier accès. Ce système de cache peut être mis à jour.\n \n
 * 5) Les demande d'effacement, remplacement de texte sont intercepter pour gérer les cas où une image sera concernée.\n \n
 * \n
 * <b>Fonctionnement des menus multi-niveaux :</b>
 * - Lors de l'initialisation (InitParam()) les menus par défaut des rubriques sont récupérées.
 * - m_list_name contient la liste de travail au moment de l'appel du menu.
 * - Le menu et initialement créé en cache puis récupéré depuis le cache ( GetListForCache() ). Le cache peut être intégralement crée grâce à la fonction TotalCacheCreator(). Le cache est mis à jour en effaçant simplement tous les fichiers contenus dans le répertoire de cache : deleteCache(). Le cache est stocké dans Glossaire / DIR_CACHE_LISTES.
 * - Le menu est créé par la fonction GoPopup() , PopupCreateList(), createPopupMenu().
 * - ListManager() permet d'afficher le widget de gestion des listes.
 * - Les slots OnMenuAboutToHide(), OnMenuActivated(), OnMenuHighlighted() permettent d'afficher la bulle d'aide (CMyBulleWidget) et de capturer l'item du menu sélectionné.
 * - Les fonctions d'insertions dans le texte : PasteFileItem(), PasteFolderItem(), PasteListItem().
 *
*/

 class MyEditText : public QTextEdit
 {

  Q_OBJECT

  public:
      //................... constructeurs et destructeurs....................
      MyEditText(const QString & text,
                     QSqlDatabase  * pDataBase  = 0,
                     const QString & context     = QString::null,
                     QWidget         * parent      = 0,
                     const char     * name         = 0)
                    :QTextEdit(text, context, parent, name)
                 {m_DataBase            = pDataBase;
                  //....................... definir le chemin de l'application .............................
                  setMouseTracking ( TRUE );
                  InitParam();
                 }

      MyEditText(QSqlDatabase  * pDataBase = 0,
                     QWidget         * parent     = 0,
                     const char     * name        = 0)
                    :QTextEdit(parent, name)
                 {m_DataBase            = pDataBase;
                  //....................... definir le chemin de l'application .............................
                  setMouseTracking ( TRUE );
                  InitParam();
                 }

      ~MyEditText ();

    enum insertMode {
            noAddHtmlHeader,
            AddHtmlHeader,
          HtmlConvert,
          HtmlConvertSp
      };



	/////////////////////////////////////////////////////////
	/////////////////      METHODES         ////////////////////
	/////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////
	/////////////////      METHODES         ////////////////////
	/////////////////      ET SLOTS         ////////////////////
	/////////////////      GESTION          ////////////////////
	/////////////////        LISTES          ////////////////////
	/////////////////////////////////////////////////////////
    void               GetSpecialRubList (   ThemePopup *pThemePopup, int rubType);
    QString            GetLastList(int ori_index=-1,  int ori_para=-1, QPoint *pos=0);
    int                GetListForCache (      QStringList &lst,
                                              QString &list_name,
                                              const QString &parent_liste =QString::null,
                                              int nbSubMenu =0);
    int                GetListItem (ThemePopup *pThemePopup, QString &list_name );

    void              deleteCache();
    bool              IsThisFileExist(const char* path );
    bool              IsThisFolderExist(const char *path, const char* folderName );


	/////////////////////////////////////////////////////////
	/////////////////      METHODES         ////////////////////
	/////////////////        IMAGES          ////////////////////
	/////////////////////////////////////////////////////////

    QString           GetImageNameNearCursor();
    QString           GetLastImageName(){return m_ImageName;};
    void              SaveImage(QString &imageName);
    void              ImageProperty(QString &imageName);




	/////////////////////////////////////////////////////////
	/////////////////      METHODES         ////////////////////
	/////////////////     GENERALES         ////////////////////
	/////////////////////////////////////////////////////////

    void              InitParam();
    void              SetTypeRubrique(int rub_type){m_TypeRubrique = rub_type;};
    int               GetTypeRubrique( ){return m_TypeRubrique;};
    void              specialInsert(QString &str);
    void              DoMixture(QString &text);
    bool              event ( QEvent * e );
    QPopupMenu       *createPopupMenu ( const QPoint & pos );
    void              moveCursor ( int action, bool select =0);
    int                GoPopup            (      QStringList &lst,
                                                     ThemePopup  &m_goPopup,
                                                     int length,
                                                     int i = 0);
    void              htmlConvert ( QString &htmlStr, int flag = MyEditText::HtmlConvert , const QTextEdit *pQTextEdit =0);
    int                DoContextListPopup(QString *pListInUse =0);
    void              ConnectPopup(ThemePopup *pThemePopup );
    void              MyMoveCursor(int toMove);

	/////////////////////////////////////////////////////////
	/////////////////      DONNES            ////////////////////
	/////////////////////////////////////////////////////////

    QString            m_list_name;                     /*!< Liste de travail au moment de l'appel du popup */
    QString            m_PathAppli;                     /*!< Chemin vers l'application */
    QString            m_style;
    QString            m_DefaultBackgroundColor;        /*!< Couleur de fond */
    QString            m_DefaultTextColor;              /*!< Couleur du texte */
    QString            m_local_param;                   /*!< ? */
    QFont              m_LastFont;                      /*!< Police de caractères du texte située
                                                           avant le curseur au moment de l'appel du popup */
    QColor             m_LastColor;                     /*!< Couleur de caractères du texte située
                                                           avant le curseur au moment de l'appel du popup */
    int                m_State;                        /*!< Etat du MyEditText */
    QSqlDatabase      *m_DataBase;                     /*!< Non utilisée */
    CMyBulleWidget    *m_pQtextMenuBulle;              /*!< Bulle d'aide du menu (buggé) */
    QStringList        m_black_list;                   /*!< liste des termes a ne pas inclure dans un menu */


	/////////////////////////////////////////////////////////
	/////////////////         SLOTS          ////////////////////
	/////////////////////////////////////////////////////////
 public slots:
	/////////////////////////////////////////////////////////
	/////////////////         SLOTS          ////////////////////
	/////////////////      GENERAUX         ////////////////////
	/////////////////////////////////////////////////////////
        void     CouCou();
        void     SetZoom(int zoom){m_Zoom=zoom; zoomTo(m_Zoom);};
        void     del();
        void     cut();
        void     paste();

        void     setText ( const char* ptr );
        void     setText ( const QString & txt );

        QString getText( CRubRecord* pCRubRecord = 0 );
        QString getText_IfTypeIsDocuments(int *len = 0);
        QString getText_IfTypeIsOrdoCalc(CRubRecord* pCRubRecord);
        QString getText_IfTypeIsOrdoLap(CRubRecord* pCRubRecord);
  QPixmap*                    decodeStringTo_pQPixmap( const QString& in );
  QPixmap                     decodeStringTo_QPixmap( const QString& in );
  QString                     encodePixmapToBase64( const QPixmap& pixIn);
  QString                     encodePixmapToBase64( QPixmap* pQPixmap);


	/////////////////////////////////////////////////////////
	/////////////////         SLOTS          ////////////////////
	/////////////////      GESTION          ////////////////////
	/////////////////        LISTES          ////////////////////
	/////////////////////////////////////////////////////////

        void     UpdateMenu();
        int      TotalCacheCreator();
        void     ListManager();
        void     PasteListItem ();
        void     PasteFolderItem ();
        void     PasteTextItem (const QString &ext);
        void     PasteFileItem (QString path);
        void     PopupItemFusionItem();
        void     PopupItemAddItem();
        void     PopupCreateList();
        void     PopupItemExeCustomItem();


        void     OnMenuAboutToHide ();
        void     OnMenuActivated (int id);
        void     OnMenuHighlighted (int id);
        void     TimedDestroyMenuBulle();
        void     PopupItemModifierImage();
        void     PopupItemSaveImage();
        void     Slot_PopupActionDumy();
        void     Slot_MyMoveCursor();

protected:
     //virtual void mouseMoveEvent( QMouseEvent *e );
     //virtual bool eventFilter ( QObject * o, QEvent * e );

protected:
    int                     m_Zoom;			        /*!< Niveau de zoom du TextEdit */
    QString                 m_ListItem;             /*!< Item principal pour menu hiérarchique */
    QString                 m_ImageName;		    /*!< nom de l'image */
    QString                 m_CurrentPopupItem;     /*!< Nom de l'item sélectionné dans le popup */
    CMyThemePopup          *m_pThemePopup;	     	/*!< Menu Popup pour les listes hiérarchiques */
     int                    m_TypeRubrique;		    /*!< Type de la rubrique associée à ce widget */
    QString                *m_pRetLastListInUse;    /*!< ponteur sur QString ou retourner la derniere liste en usage*/
    int                     m_toMove;
 signals:
    void message(const QString&, int);
    void Sign_ExeCommand( QString&);
    void Sign_Exe_Plugin( QString&,  CRubRecord *pCRubRecord);
    void Sign_Exe_Mixture( QString&, CRubRecord *pCRubRecord);
    void Signal_Key_Delete_Pressed(int );
    void Signal_Key_Backspace_Pressed(int );
    void Signal_Del_Before();
    void Signal_Del_After();
    void Signal_Cut_Before();
    void Signal_Cut_After();
    void Signal_Paste_Before();
    void Signal_Paste_After();
    void Signal_Replace_Before();
    void Signal_Replace_After();
    void Sign_Get_Custom_Menu(const QString &, QString&, QString &, QKeySequence &);

 };

 #endif


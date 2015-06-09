 /********************************* CPrtQListViewItem.h ***************************
 *  #include "CPrtQListViewItem.h"                                                *
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

#ifndef CPrtQListViewItem_H
#define CPrtQListViewItem_H

#include <qstring.h>
#include <q3listview.h>
#include <qstring.h>
#include <qpainter.h>
#include <qcolor.h>
#include <qstyle.h>
#include <qdatetime.h>
#include <qpixmap.h>
#include <qevent.h>
#include <qmime.h>
#include <q3dragobject.h>
#include <qobject.h>
//Added by qt3to4:
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QDragLeaveEvent>
#include <QMouseEvent>
#include <Q3StrList>
#include <Q3MemArray>
#include <QDragEnterEvent>

#include "Theme.h"


#define LISTVIEW_TYPE_COLUMN            0
#define LISTVIEW_PRIORITY_COLUMN        1
#define LISTVIEW_ETAT_COLUMN            2

#define LISTVIEW_DATE_COLUMN            3
#define LISTVIEW_LIBELLE_COLUMN         4


#define TO_DELETE   1
#define TO_UPDATE   2
#define TO_MASK     4
#define TO_DEBUG    8

//=============================================================== C_DragQListViewItem ===========================================================
class C_DragQListView : public Q3ListView
{

        Q_OBJECT

public:
    C_DragQListView( QString *ptr, QWidget *parent = 0, const char *name = 0 );
    C_DragQListView( QWidget *parent = 0, const char *name = 0 );
public:
    void set_RootPathPtr(QString *ptr){m_pCurPath = ptr;};
    void set_RootPath(const QString &prop){*m_pCurPath = prop;};
    void set_Prop_0(const QString &prop){m_Prop_0 = prop;};
    void set_Prop_1(const QString &prop){m_Prop_1 = prop;};
    void set_Prop_2(const QString &prop){m_Prop_2 = prop;};


    QString get_RootPath(){ return *m_pCurPath;};
    QString get_Prop_0(){ return m_Prop_0;};
    QString get_Prop_1(){ return m_Prop_1;};
    QString get_Prop_2(){ return m_Prop_2;};


    int  setState(int state)      {m_State |= state;   return m_State;}
    int  removeState(int state)   {m_State &= ~state;  return m_State;}
    bool IsStatePlaced(int state) { return m_State & state;}
    int  getState()               { return m_State;}
    void dragClose();

protected:
    void contentsDragEnterEvent( QDragEnterEvent *e );
    void contentsDragMoveEvent( QDragMoveEvent *e );
    void contentsDragLeaveEvent( QDragLeaveEvent *e );
    void contentsDropEvent( QDropEvent *e );
    void contentsMouseMoveEvent( QMouseEvent *e );
    void contentsMousePressEvent( QMouseEvent *e );
    void contentsMouseReleaseEvent( QMouseEvent *e );

    QString m_Prop_0;
    QString m_Prop_1;
    QString m_Prop_2;

private:
    QString        *m_pCurPath;
    Q3ListViewItem *m_oldCurrent;
    Q3ListViewItem *m_dropItem;
    QTimer         *m_autoopen_timer;
    QPoint          m_presspos;
    Q3ListViewItem *m_movedItem;
    bool            m_mousePressed;
    int             m_State;                /*!< etat de l'item */

public slots:
    void openFolder();

signals:
    void Sign_DragMoveEvent(Q3ListViewItem *, bool &dragBreak);
    void Sign_ItemClicked(Q3ListViewItem *);
    void Sign_ItemDraged_On_AnotherItem(Q3ListViewItem *, Q3ListViewItem *,        const QString&);
    void Sign_FilesDraged_On_Item(Q3ListViewItem *,       Q3StrList &,             const QString&);
    void Sign_TextDraged_On_Item(Q3ListViewItem *,        const QString  &,       const QString&);

};

#define CP_Deb 3
#define CP_Fin 7
//=============================================================== C_DragQListViewItem ===========================================================
class C_DragQListViewItem : public Q3ListViewItem
{
  // Q_OBJECT

public:
   C_DragQListViewItem(Q3ListView * listview );

   C_DragQListViewItem(Q3ListViewItem * pQListViewItem );

   C_DragQListViewItem(Q3ListView * listview  ,
                 Q3ListViewItem * after ,
                 const QString libelle = QString::null);

   C_DragQListViewItem(Q3ListViewItem * pQListViewItem  ,
                 Q3ListViewItem * after ,
                 const QString libelle = QString::null);


   ~C_DragQListViewItem(){};

   void paintCell( QPainter * p, const QColorGroup & cg, int column, int width, int align );
   void columnStyleInitialise(int nbColumns);
   QString get_Prop_0(){ return m_Prop_0;};
   QString get_Prop_1(){ return m_Prop_1;};
   QString get_Prop_2(){ return m_Prop_2;};
   QString get_Prop_3(){ return m_Prop_3;};

   void set_Prop_0(const QString &prop){m_Prop_0 = prop;};
   void set_Prop_1(const QString &prop){m_Prop_1 = prop;};
   void set_Prop_2(const QString &prop){m_Prop_2 = prop;};
   void set_Prop_3(const QString &prop){m_Prop_3 = prop;};

   int  setState(int state)      {m_State |= state;   return m_State;}
   int  removeState(int state)   {m_State &= ~state;  return m_State;}
   bool IsStatePlaced(int state) { return m_State & state;}
   int  getState()               { return m_State;}

protected:
   bool                 m_LightBackground;      /*!< Couleur du background de la QListViewItem (TRUE=light, FALSE=dark) */
   Q3MemArray<bool>      m_columnIsBold;         /*!< Sauvegarde le style de chaque colonnes */
   Q3MemArray<bool>      m_columnIsItalic;       /*!< Sauvegarde le style de chaque colonnes */
   Q3MemArray<bool>      m_columnIsUnderline;    /*!< Sauvegarde le style de chaque colonnes */
   QString              m_Prop_0;               /*!< propriete de stockage 1 */
   QString              m_Prop_1;               /*!< propriete de stockage 2 */
   QString              m_Prop_2;               /*!< propriete de stockage 3 */
   QString              m_Prop_3;               /*!< propriete de stockage 4 */
   int                  m_State;                /*!< propriete de flag */
private:

};



//=============================================================== CPrtQListViewItem ===========================================================
//#define SPACE_TXT 9
class CPrtQListViewItem : public Q3ListViewItem
{
public:

   CPrtQListViewItem(Q3ListView * listview ,
                 const QString &libelle ,
                 const QString &idRubList,
                 const QString &date,
                 const QString &pkDoc,
                 const QString &subType,
                 const QString &type,
                 const QString &signUser,
                 const QString &user);

   CPrtQListViewItem(Q3ListViewItem * listview ,
                 const QString &libelle ,
                 const QString &idRubList,
                 const QString &date,
                 const QString &pkDoc,
                 const QString &subType,
                 const QString &type,
                 const QString &signUser,
                 const QString &user);

   CPrtQListViewItem(Q3ListViewItem * listview ,
                 Q3ListViewItem * after ,
                 const QString libelle = QString::null );

   CPrtQListViewItem(Q3ListView     * listview ,
                 Q3ListViewItem * after ,
                 const QString libelle = QString::null );

   ~CPrtQListViewItem();


   void    setValue(const QString &iD_Rublist, const QString  &type, const QString &user, const QString &subType) ;
   void    getValue(QString &iD_Rublist, QString  &type, QString &user, QString &subType) ;

   QString      get_ID_Rublist() ;
   QString      getPkDoc() ;
   QString      getLibelle();
   QString      getType() ;
   QString      getUser() ;
   QString      getSubType();
   QString      getNom();
   QString      getPrenom();
   QString      getGUID();

   QString      getDate(QString& format);
   QDateTime      getDate();
   bool            isLightBackground();


   void            setPkDoc(QString n);
   void            setNom (QString n);
   void            setPrenom (QString n);
   void            setGUID (QString n);
   void            set_ID_Rublist(QString& n);
   void            set_ID_Rublist(int n);
   void            setType(QString& n);
   void            setUser(QString& n);
   void            setSignUser(QString& n);
   void            setSubType(QString& n);
   void            setDate(QString& n);
   void            setDate(QDateTime& dt);
   void            setLightBackground(bool c);
   void            setModified( bool m );

   // Réimplémentation du trie  pour les dates essentiellement
   int compare( Q3ListViewItem *i, int col, bool asc ) const;
   // Réimplémentation pour l'esthétique
   void paintCell( QPainter * p, const QColorGroup & cg, int column, int width, int align );

   // Conserve la mise en forme de chaque colonnes
   void            columnStyleInitialise(int nbColumns);


 //................. DATA .....................................


protected:
   QString      m_Type;                  /*!< Type de la rubrique. \sa CRubList.h */
   QString      m_SubType;            /*!< Sous-type de la rubrique. */
   QString      m_ID_Rublist;            /*!< Position de l'item dans la RubList. */
   QString      m_Libelle;            /*!< Libelle de la rubrique. */
   QString      m_pkDoc;            /*!< Clé primaire de la rubrique. */
   QString      m_User;                  /*!< Utilisateur Actuel */
   QString      m_signUser;            /*!< Utilisateur signataire de la rubrique */
   QDateTime    m_Date;                  /*!< Date de la rubrique */
   QString      m_GUID;                  /*!< GUID Patient si besoin */
   QString      m_NomPatient;            /*!< Nom du Patient si besoin */
   QString      m_PrenomPatient;      /*!< Prénom du Patient si besoin */

   bool            m_LightBackground;      /*!< Couleur du background de la QListViewItem (TRUE=light, FALSE=dark) */
   bool            m_Modified;            /*!< Signale une modification dans le contenu du document associé �  cet item */
   Q3MemArray<bool>      columnIsBold;      /*!< Sauvegarde le style de chaque colonnes */
   Q3MemArray<bool>      columnIsItalic;      /*!< Sauvegarde le style de chaque colonnes */
   Q3MemArray<bool>      columnIsUnderline;      /*!< Sauvegarde le style de chaque colonnes */

};

#endif

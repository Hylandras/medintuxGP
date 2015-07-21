//
// C++ Interface: C_ListViewATCDManager
//
// Description:
//
//
// Author: Sevin Roland <roland-sevin@medintux.org>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef C_ListViewATCDManager_H
#define C_ListViewATCDManager_H
#include <qobject.h>

#include "C_Atcd.h"

class CPrtQListViewItem;
/**
        @author Sevin Roland <roland-sevin@medintux.org>
*/

class C_ListViewATCDManager:public QObject
{

   Q_OBJECT


public :
        C_ListViewATCDManager(QObject * parent , const char * name , QListView*   pQListView, Atcd_Code* pAtcd_Code, int *pIsModified, int *pIsModifiable, QString *pTypeATCD_Selectionne );
        ~C_ListViewATCDManager() {}
        void          Add_popMenu_ATCD_Type(QPopupMenu* pQPopupMenu);
        void          ATCD_setAldOnOff(const QString &state);
        void          listView_ATCD_doubleClicked( QListViewItem * pQListViewItem);
        void          dellAllSelectedATCD();
        Atcd_Element*      get_Selected_Atcd_Element( CPrtQListViewItem *pCPrtQListViewItem = 0 );
        CPrtQListViewItem* get_CurrentItem();
        QDate         ATCD_selectDate(const QString &date, const QString &title);
        void          ATCD_MenuActionMultiDel(int sendModifMessage);
        int           ATCD_EditListViewItem(CPrtQListViewItem* pCPrtQListViewItem);
public:
        QListView*              m_pQListView;                 /*!< libellé de l'ATCD */
        Atcd_Code*              m_pAtcd_Code;                 /*!< Rubrique de l'ATCD */
        QStringList             m_ATCD_FamilyList;
        QStringList             m_ATCD_GenreList;
        QListViewItem*          m_pQListViewItem;
        int*                    m_pIsModified;
        int*                    m_pIsModifiable;
        QString*                m_pTypeATCD_Selectionne;


public slots:
        void Slot_listView_ATCD_contextMenuRequested( QListViewItem *qlistViewItem, const QPoint &pos, int c );
        void ATCD_MenuActionNewCIM10();
        void ATCD_MenuActionNewAllergie();
        void ATCD_MenuActionNewTexteLibre();
        void ATCD_MenuActionMultiDel();
        void ATCD_MenuActionSetDateDeb();
        void ATCD_MenuActionDelDateDeb();
        void ATCD_MenuActionSetDateFin();
        void ATCD_MenuActionDelDateFin();
        void ATCD_MenuActionSetCommentaire();
        void ATCD_setAldOn();
        void ATCD_setSportOn();
        void ATCD_setVigilanceOn();
        void ATCD_setAldOff();
        void ATCD_MenuActionSetFamilleGenre();

signals:

};
#endif

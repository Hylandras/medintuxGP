 /********************************* C_Frm_UserList.h ******************************
 *  #include "C_Frm_UserList.h"                                                   *
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
 *   Commissariat √  l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la F√©d√©ration, 75752 PARIS cedex 15.   *
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

#ifndef C_FRM_USERLIST_H
#define C_FRM_USERLIST_H

#include "ui_C_Frm_UserList.h"
#include "../../Manager/src/CApp.h"
#include <QTimer>
//===================================== C_Frm_UserList ========================================================================
class CMoteur_Base;
class C_Frm_UserList : public QFrame
{
    Q_OBJECT
    Q_DISABLE_COPY(C_Frm_UserList)
public:
    explicit C_Frm_UserList(CMoteurBase *pCMoteurBase, QWidget *parent = 0);
            ~C_Frm_UserList();
    void             initUserListOnUserType(const QString &specialite);
    void             initComboBoxUserTyp();
    void             setDefaultParameter();
    void             SetUnLogged();
    void             SetLogged();
    QTreeWidgetItem *getSignUser(QTreeWidgetItem *pQListViewItem);
    QTreeWidgetItem *getUserItem(const QString &user, const QString &sign_user);
    void             setComboBoxOnUserTyp_WithoutSignals(QString userTyp);
    void             setComboOnValue(QComboBox *pQComboBox, const QString &value);
    void             initOnUser(const QString   &userQualite="");
    void             SetUserItem_WhitoutEmitSignals(QTreeWidgetItem *pQListViewItem);
    QString          getDefaultComboBoxUserTyp();
    QString          itemTo_SignUser_User_Name(QTreeWidgetItem *pQListViewItem);
    bool             reinitOn_SignUser_User_Name(const QString &user, const QString &sign_user);
    void             setInitialFocusOnList(){m_ui.ListView_UserList->setFocus();}
protected:
    virtual void changeEvent(QEvent *e);
protected slots:
    void         Slot_ListView_UserList_DoubleClicked( QTreeWidgetItem * , int);
    void         timerDone();
    void         ListView_UserList_LineChanged( QTreeWidgetItem * , int );
    void         comboBoxUserTyp_activated( const QString &userQualite );
    void         restaureOldUser();
    void         pushButtonSetDefault_clicked();
    void         pushButtonEditUser_clicked();
    void         pushButtonCPS_clicked();
    void         pushButtonSetUnloged_clicked();
private:
   Ui::C_Frm_UserList   m_ui;
   QTimer              *m_pQtimer;
   CMoteurBase         *m_pCMoteurBase;
   QString              m_LastSelectedUser;
   bool                 m_pStateTimer;           /*!< stocke un etat pour faire clignoter l'invitation √  selectionner un utilisateur*/
signals:
   void Sign_UserSelected(QTreeWidgetItem*, QTreeWidgetItem*);
   void Sign_ComboUserTypClicked(const QString &);
   void Sign_UserMustBeEdited(const QString&, const QString&);
   void Sign_CPS_IsClicked();
   void Sign_UserMustBeUnloged(int &);
};

#endif // C_FRM_USERLIST_H

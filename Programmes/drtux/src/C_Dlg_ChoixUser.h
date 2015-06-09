//
// C++ Interface:
//
// Description:
//
//
// Author: Sevin Roland <upsis@club-internet.fr>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef C_DLG_CHOIXUSER_H
#define C_DLG_CHOIXUSER_H

#include "ui/DlgChoixUser.h"
class CMoteurBase;
class C_Dlg_ChoixUser : public DlgChoixUser
{
  Q_OBJECT

public:
  C_Dlg_ChoixUser(CMoteurBase* pCMoteurBase, QString user, QString sign_user, QString *pParam,
                  QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  ~C_Dlg_ChoixUser();
  /*$PUBLIC_FUNCTIONS$*/
  virtual QListViewItem * getUserItem( const QString & user, const QString & sign_user );
  virtual QListViewItem * getSignUser( QListViewItem * pQListViewItem );
  virtual QListViewItem * getSignUser();

public slots:
  /*$PUBLIC_SLOTS$*/
  virtual void          lineEditPassWord_textChanged( const QString & password );
  virtual void          pushButtonConfig_clicked();
  virtual void          ListView_UserList_doubleClicked( QListViewItem * pQListViewItem );
  virtual void          ListView_UserList_clicked( QListViewItem * pQListViewItem );
  virtual void          pushButtonOK_clicked();
  virtual void          pushButtonAnnuler_clicked();
public:
    QString       m_User;
    QString       m_SignUser;
protected:
  /*$PROTECTED_FUNCTIONS$*/

    CMoteurBase  *m_pCMoteurBase;
    QString      *m_pParam;

protected slots:
  /*$PROTECTED_SLOTS$*/

};

#endif


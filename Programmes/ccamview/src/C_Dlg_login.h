//
// C++ Interface: 
//
// Description: 
//
//
// Author: Sevin Roland <ro@upsis>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef C_DLG_LOGIN_H
#define C_DLG_LOGIN_H

#include "Dlg_login.h"
//............ from UI ...............
#include "CMoteurCCAM_Base.h"
#include "Dlg_Praticiens.h"

class C_Dlg_login : public Dlg_Login
{
  Q_OBJECT

public:
  C_Dlg_login(CMoteurCCAM_Base* pCMoteurCCAM_Base, const QString &last_user, const QString &password,
              QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WindowFlags fl = 0 );
  ~C_Dlg_login();
  /*$PUBLIC_FUNCTIONS$*/
  // virtual int           initDialog( CMoteurCCAM_Base * pCMoteurCCAM_Base, const QString & last_user, const QString & password );

public slots:
  /*$PUBLIC_SLOTS$*/
  virtual void          lineEdit_MotDePasse_textChanged( const QString & );
  virtual void          bouton_Quitter_clicked();
  virtual void          bouton_OK_clicked();

protected:
  /*$PROTECTED_FUNCTIONS$*/
  CMoteurCCAM_Base* m_pCMoteurCCAM_Base;
protected slots:
  /*$PROTECTED_SLOTS$*/

};

#endif


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

#ifndef C_DLG_SIGEMSLOGIN_H
#define C_DLG_SIGEMSLOGIN_H

#include "Dlg_SigemsLogin.h"

class C_Dlg_SigemsLogin : public Dlg_SigemsLogin
{
  Q_OBJECT

public:
  C_Dlg_SigemsLogin(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WindowFlags fl = 0 );
  ~C_Dlg_SigemsLogin();
  /*$PUBLIC_FUNCTIONS$*/

public slots:
  /*$PUBLIC_SLOTS$*/
  virtual void          pushButtonOK_clicked();

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/

};

#endif


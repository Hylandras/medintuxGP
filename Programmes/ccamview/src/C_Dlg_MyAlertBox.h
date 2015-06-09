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

#ifndef C_DLG_MYALERTBOX_H
#define C_DLG_MYALERTBOX_H

#include "Dlg_MyAlertBox.h"

class C_Dlg_MyAlertBox : public Dlg_MyAlertBox
{
  Q_OBJECT

public:
  C_Dlg_MyAlertBox(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WindowFlags fl = 0 );
  ~C_Dlg_MyAlertBox();
  /*$PUBLIC_FUNCTIONS$*/

public slots:
  /*$PUBLIC_SLOTS$*/
  virtual void          pushButton4_clicked();
  virtual void          pushButton3_clicked();
  virtual void          pushButton2_clicked();
  virtual void          pushButton1_clicked();

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/

};

#endif


//
// C++ Implementation: 
//
// Description: 
//
//
// Author: Sevin Roland <ro@upsis>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "C_Dlg_SigemsLogin.h"

C_Dlg_SigemsLogin::C_Dlg_SigemsLogin(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
: Dlg_SigemsLogin(parent,name, modal,fl)
{connect( pushButtonOK, SIGNAL( clicked() ), this, SLOT( pushButtonOK_clicked() ) );
}

C_Dlg_SigemsLogin::~C_Dlg_SigemsLogin()
{
}

/*$SPECIALIZATION$*/
void C_Dlg_SigemsLogin::pushButtonOK_clicked()
{    accept();
}




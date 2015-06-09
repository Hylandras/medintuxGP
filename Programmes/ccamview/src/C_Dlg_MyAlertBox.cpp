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


#include "C_Dlg_MyAlertBox.h"

C_Dlg_MyAlertBox::C_Dlg_MyAlertBox(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
: Dlg_MyAlertBox(parent,name, modal,fl)
{  // signals and slots connections
    connect( pushButton1, SIGNAL( clicked() ), this, SLOT( pushButton1_clicked() ) );
    connect( pushButton2, SIGNAL( clicked() ), this, SLOT( pushButton2_clicked() ) );
    connect( pushButton3, SIGNAL( clicked() ), this, SLOT( pushButton3_clicked() ) );
    connect( pushButton4, SIGNAL( clicked() ), this, SLOT( pushButton4_clicked() ) );

}

C_Dlg_MyAlertBox::~C_Dlg_MyAlertBox()
{
}

/*$SPECIALIZATION$*/
void C_Dlg_MyAlertBox::pushButton4_clicked()
{done (400);
}

void C_Dlg_MyAlertBox::pushButton3_clicked()
{done (300);
}

void C_Dlg_MyAlertBox::pushButton2_clicked()
{done (200);
}

void C_Dlg_MyAlertBox::pushButton1_clicked()
{done (100);
}




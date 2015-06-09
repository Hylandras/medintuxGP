/***************************************************************************
 *   Copyright (C) 2005 by Sevin Roland                                    *
 *   ro@vaio_ro                                                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "cdlg_passwordget.h"
#include "CApp.h"
#include "CMoteur_Base.h"
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
//------------------------------------------------------ CDlg_PasswordGet -------------------------------------------------------------------
CDlg_PasswordGet::CDlg_PasswordGet(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
    : Dlg_PasswordGet(parent,name, modal,fl) , m_pPassToGet(0)
{lineEditPassword->setFocus();
 connect( pushButtonOK,      SIGNAL( clicked() ), this, SLOT( accept() ) );
 connect( pushButtonAnnuler, SIGNAL( clicked() ), this, SLOT( reject() ) );

// pushButtonOK
// pushButtonAnnuler
// checkBoxMemorise
}
//------------------------------------------------------ CDlg_PasswordGet -------------------------------------------------------------------
CDlg_PasswordGet::CDlg_PasswordGet(QString *passToGet, QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
    : Dlg_PasswordGet(parent,name, modal,fl), m_pPassToGet(passToGet)
{lineEditPassword->setFocus();
 connect( pushButtonOK,      SIGNAL( clicked() ), this, SLOT( accept() ) );
 connect( pushButtonAnnuler, SIGNAL( clicked() ), this, SLOT( reject() ) );

}

//------------------------------------------------------ ~CDlg_PasswordGet -------------------------------------------------------------------
CDlg_PasswordGet::~CDlg_PasswordGet()
{}

/*$SPECIALIZATION$*/
//------------------------------------------------------ reject -------------------------------------------------------------------
void CDlg_PasswordGet::reject()
{ QDialog::reject();
}

//------------------------------------------------------ accept -------------------------------------------------------------------
void CDlg_PasswordGet::accept()
{ if (m_pPassToGet)
  {QString pass = lineEditPassword->text();
   CMoteurBase::PassWordEncode(pass);
   *m_pPassToGet = pass;
  }
  QDialog::accept();
}

//------------------------------------------------------ IsPasswordMustBeRecord -------------------------------------------------------------------
bool CDlg_PasswordGet::IsPasswordMustBeRecord()
{ return checkBoxMemorise->isChecked();
}


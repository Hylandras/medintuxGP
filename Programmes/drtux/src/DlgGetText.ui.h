/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/






void DlgGetText::pushButtonOK_clicked()
{
 accept();
}


void DlgGetText::pushButtonCancel_clicked()
{reject();
}


void DlgGetText::pushButtonClearAndQuit_clicked()
{textEdit->setText("");
 accept();
}

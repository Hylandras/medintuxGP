/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/



void Dlg_SaveRequest::pushButtonSaveAll_clicked()
{m_RetCode=CSaveRequest::SaveALLRequest;
 accept();
}


void Dlg_SaveRequest::pushButtonSaveCurrent_clicked()
{m_RetCode=CSaveRequest::SaveCurrentRequest;
 accept();
}


void Dlg_SaveRequest::pushButtonCancelAll_clicked()
{m_RetCode=CSaveRequest::NoSaveALLRequest;
 accept();
}


void Dlg_SaveRequest::pushButtonCancelCurrent_clicked()
{m_RetCode=CSaveRequest::NoSaveCurrentRequest;
 accept();
}


void Dlg_SaveRequest::pushButtonAbort_clicked()
{m_RetCode=CSaveRequest::AbortRequest;
 reject();
}

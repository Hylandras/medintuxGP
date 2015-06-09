/***************************************************************************
**
** Copyright (C) 2003-2004 Trolltech AS.  All rights reserved.
**
** Licensees holding valid Qt Enterprise Edition licenses may use this
** file in accordance with the Qt Solutions License Agreement provided
** with the Solution.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
** PURPOSE.
**
** Please email sales@trolltech.com for information
** about Qt Solutions License Agreements.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
*/
#include "qtsingleapplication.h"
#include <qt_windows.h>

class QtSingletonSysPrivate
{
public:
    QWidget *listener;
};

void QtSingleApplication::sysInit()
{
    sysd = new QtSingletonSysPrivate;
    sysd->listener = 0;
}

void QtSingleApplication::sysCleanup()
{
    delete sysd;
}

void QtSingleApplication::initialize( bool activate )
{
    if (sysd->listener)
	return;

    sysd->listener = new QWidget;
    sysd->listener->setCaption(id());

    insertChild(sysd->listener);

    if ( activate )
	connect(this, SIGNAL(messageReceived(const QString&)),
		this, SLOT(activateMainWidget()));
}

bool QtSingleApplication::isRunning() const
{
    HWND hwnd;
    QT_WA( {
	hwnd = ::FindWindow(0, (TCHAR*)id().ucs2());
    }, {
	hwnd = ::FindWindowA(0, id().local8Bit());
    } )
    return hwnd != 0;
}

bool QtSingleApplication::sendMessage( const QString &message, int timeout )
{
    HWND hwnd;
    QT_WA( {
	hwnd = ::FindWindow(0, (TCHAR*)id().ucs2());
    }, {
	hwnd = ::FindWindowA(0, id().local8Bit());
    } )
    if ( !hwnd )
	return FALSE;

    COPYDATASTRUCT data;
    data.dwData = 0;
    data.cbData = (message.length()+1) * sizeof(QChar);
    data.lpData = (void*)message.ucs2();
    DWORD result;
    LRESULT res;
    QT_WA( {
	res = SendMessageTimeout(hwnd, WM_COPYDATA, 0/*hwnd sender*/, (LPARAM)&data, 
				 SMTO_ABORTIFHUNG,timeout,&result);
    }, {
	res = SendMessageTimeoutA(hwnd, WM_COPYDATA, 0/*hwnd sender*/, (LPARAM)&data, 
				  SMTO_ABORTIFHUNG,timeout,&result);
    } )
    return res != 0;
}

bool QtSingleApplication::winEventFilter( MSG *msg )
{
    if (msg->message != WM_COPYDATA || msg->hwnd != sysd->listener->winId() )
	return QApplication::winEventFilter(msg);

    COPYDATASTRUCT *data = (COPYDATASTRUCT*)msg->lParam;
    QString message = QString::fromUcs2((unsigned short*)data->lpData);

    emit messageReceived( message );

    return TRUE;
}

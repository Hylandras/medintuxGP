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
#include <qpaintdevice.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>

class QtSingletonSysPrivate
{
public:
    Atom selAtom;
    Atom typAtom;
    QWidget *listener;
    QByteArray xpcs;

    QString login() const;
    char *toXPCS(const QString &str) const; // X Portable Character Set
};

char *QtSingletonSysPrivate::toXPCS(const QString &str) const
{
    char *tmp = new char[str.local8Bit().length()+1];
    strcpy(tmp, str.local8Bit());
    for (size_t i = 0; i < strlen(tmp); ++i) {
	if (xpcs.find(tmp[i]) == -1)
	    tmp[i] = '-';
    }
    return tmp;
}

QString QtSingletonSysPrivate::login() const
{
    struct passwd *pwd = getpwuid(getuid());
    if (pwd) {
	return QString(pwd->pw_name);
    }
    return QString();
}

void QtSingleApplication::sysInit()
{
    sysd = new QtSingletonSysPrivate;
    sysd->selAtom = None;
    sysd->typAtom = None;
    sysd->listener = 0;
    sysd->xpcs.resize(98);
    sysd->xpcs.duplicate("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~ \t\n", 98);
}

void QtSingleApplication::sysCleanup()
{
    delete sysd;
}

void QtSingleApplication::initialize(bool activate)
{
    if (sysd->selAtom != None)
	return;

    char *login = sysd->toXPCS(sysd->login()+id());
    sysd->selAtom = XInternAtom(QPaintDevice::x11AppDisplay(),
				login,
				False);
    sysd->typAtom = XInternAtom(QPaintDevice::x11AppDisplay(),
				"QtSingleApplicationType",
				False);
    delete[] login;

    if (sysd->selAtom != None) {
	sysd->listener = new QWidget;
	insertChild(sysd->listener);
	XSetSelectionOwner(QPaintDevice::x11AppDisplay(),
			   sysd->selAtom,
			   sysd->listener->winId(),
			   CurrentTime);
    }

    if (activate)
	connect(this, SIGNAL(messageReceived(const QString&)),
		this, SLOT(activateMainWidget()));
}

bool QtSingleApplication::isRunning() const
{
    char *login = sysd->toXPCS(sysd->login()+id());
    Atom tmp = XInternAtom(QPaintDevice::x11AppDisplay(),
			   login,
			   True);
    delete[] login;

    if (tmp != None) {
	WId wid = XGetSelectionOwner(QPaintDevice::x11AppDisplay(),
				     tmp);
	return (wid != None);
    }
    return FALSE;
}

bool QtSingleApplication::sendMessage(const QString &message, int)
{
    if (isRunning()) {
	char *login = sysd->toXPCS(sysd->login()+id());
	Atom sel = XInternAtom(QPaintDevice::x11AppDisplay(),
			       login,
			       True);
	Atom typ = XInternAtom(QPaintDevice::x11AppDisplay(),
				"QtSingleApplicationType",
				True);
	delete[] login;

	if (sel != None && typ != None) {
	    WId wid = XGetSelectionOwner(QPaintDevice::x11AppDisplay(),
					 sel);
	    if (wid != 0) {
		QByteArray  umsg = message.utf8();
		XChangeProperty(QPaintDevice::x11AppDisplay(),
				wid,
				sel, typ, 8,
				PropModeReplace,
				reinterpret_cast<unsigned char *>(umsg.data()),
				umsg.size());
	    }
	    return TRUE;
	}
    }
    return FALSE;
}

bool QtSingleApplication::x11EventFilter(XEvent *msg)
{
    const long int maxlen = 1024;

    if (sysd->listener != 0 && msg->type == PropertyNotify) {
	XPropertyEvent pev = msg->xproperty;
	if (pev.window == sysd->listener->winId()) {
	    Atom actualType;
	    int  actualFormat;
	    unsigned long  nitems;
	    unsigned long  bafter;
	    unsigned char* data = new unsigned char[maxlen];
	    XGetWindowProperty(QPaintDevice::x11AppDisplay(),
			       pev.window,
			       sysd->selAtom,
			       0L, maxlen,
			       False,
			       sysd->typAtom,
			       &actualType,
			       &actualFormat,
			       &nitems,
			       &bafter,
			       &data);
	    if (actualFormat == 8 && actualType == sysd->typAtom) {
		QString str = QString::fromUtf8(reinterpret_cast<char *>(data),
						nitems);
		delete[] data;
		emit messageReceived(str);

		return TRUE;
	    }
	    delete[] data;
	}
    }
    return QApplication::x11EventFilter(msg);
}

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
#include <qstring.h>
  
// #include <qt_mac.h>           // ne fonctionne pas sous OQS 10.8       Lion
#include <Carbon/Carbon.h>       // a remplacer par cela sous OQS 10.8    Lion

#include "qtsingleapplication.h"

extern QString cfstring2qstring(CFStringRef);
extern CFStringRef qstring2cfstring(const QString &);
class QtSingletonSysPrivate
{
public:
    inline QtSingletonSysPrivate() { port = 0; }
    CFMessagePortRef port;
};

class QtSingletonPrivate
{
};

bool QtSingleApplication::isRunning() const
{
    return !!sysd->port;
}

void QtSingleApplication::sysInit()
{
    sysd = new QtSingletonSysPrivate;
}

void QtSingleApplication::sysCleanup()
{
    delete sysd;
}

CFDataRef MyCallBack(CFMessagePortRef, SInt32, CFDataRef data, void *info)
{
    CFIndex index = CFDataGetLength(data);
    const UInt8 *p = CFDataGetBytePtr(data);
    QByteArray ba(index);
    for (int i = 0; i < index; ++i)
        ba[i] = p[i];
    QString message(ba);
    emit static_cast<QtSingleApplication *>(info)->messageReceived(message);
    return 0;
}

void QtSingleApplication::initialize(bool activate)
{
    CFStringRef cfstr = qstring2cfstring(id());
    CFMessagePortContext context;
    context.version = 0;
    context.info = this;
    context.retain = 0;
    context.release = 0;
    context.copyDescription = 0;
    sysd->port = CFMessagePortCreateLocal(kCFAllocatorDefault, cfstr, MyCallBack, &context, 0);
    CFRunLoopRef runloop = CFRunLoopGetCurrent();
    if (sysd->port && runloop) {
        CFRunLoopSourceRef source = CFMessagePortCreateRunLoopSource(0, sysd->port, 0);
        if (source)
            CFRunLoopAddSource(runloop, source, kCFRunLoopCommonModes);
        CFRelease(source);
    }
    CFRelease(cfstr);
    if (activate) {
	connect(this, SIGNAL(messageReceived(const QString&)),
		this, SLOT(activateMainWidget()));
    }
}

bool QtSingleApplication::sendMessage(const QString &message, int timeout)
{
    CFStringRef cfstr = qstring2cfstring(id());
    CFMessagePortRef myport = CFMessagePortCreateRemote(kCFAllocatorDefault, cfstr);
    if (!myport)
        return false;
    CFRelease(cfstr);
    static SInt32 msgid = 0;
    CFDataRef data = CFDataCreate(0, (UInt8*)message.latin1(), message.length());
    CFDataRef reply = 0;
    SInt32 result = CFMessagePortSendRequest(myport, ++msgid, data, timeout / 1000,
                                             timeout / 1000, 0, &reply);
    return result == kCFMessagePortSuccess;
    CFRelease(data);
    if (!reply)
        CFRelease(reply);
}

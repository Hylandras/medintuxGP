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
#ifndef QTSINGLEAPPLICATION_H
#define QTSINGLEAPPLICATION_H

#include <qapplication.h>
#ifdef Q_WS_MAC
      //#include <qt_mac.h>      // ne fonctionne pas sous OQS 10.8       Lion
	  #include <Carbon/Carbon.h> // a remplacer par cela sous OQS 10.8    Lion
#endif

class QtSingletonPrivate;
class QtSingletonSysPrivate;

class QtSingleApplication : public QApplication
{
    Q_OBJECT
public:
    QtSingleApplication( const QString &id, int &argc, char **argv, Type = GuiClient );
    ~QtSingleApplication();

    bool isRunning() const;
    QString id() const;

    void initialize( bool activate = TRUE );

protected:
#if defined(Q_WS_WIN)
    bool winEventFilter( MSG *msg );
#elif defined(Q_WS_X11)
    bool x11EventFilter( XEvent *msg );
#endif
 
public slots:
    bool sendMessage( const QString &message, int timeout = 5000 );
    virtual void activateMainWidget();

signals:
    void messageReceived( const QString& message );

private:
#ifdef Q_WS_MAC
    friend CFDataRef MyCallBack(CFMessagePortRef, SInt32, CFDataRef, void *);
#endif
    QtSingletonPrivate *d;
    QtSingletonSysPrivate *sysd;

    void sysInit();
    void sysCleanup();
};

#endif //QTSINGLEAPPLICATION_H

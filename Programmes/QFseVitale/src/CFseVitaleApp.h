/********************************* CFseVitaleApp.h ********************************
 *                                                                                *
 *                                                                                *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat à l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Fédération, 75752 PARIS cedex 15.   *
 *                            FRANCE                                              *
 *   Centre National de la Recherche Scientifique                                 *
 *   - CNRS,                                                                      *
 *                            3 rue Michel-Ange, 75794 Paris cedex 16.            *
 *                            FRANCE                                              *
 *   Institut National de Recherche en Informatique et en Automatique             *
 *   - INRIA,                                                                     *
 *                            Domaine de Voluceau, Rocquencourt, BP 105, 78153    *
 *                            FRANCE                                              *
 *                                                                                *
 *         This program is distributed in the hope that it will be useful,        *
 *         but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                   *
 *         CeCILL  License (GPL compatible) for more details.                     *
 *                                                                                *
 **********************************************************************************/
#ifndef CFSEAPP_H
#define CFSEAPP_H

#include "CVitaleReadThread.h"
#include <qapplication.h>
#include <qdir.h>
#include <qfile.h>

#include "../../MedinTuxTools/CCoolPopup.h"
#include "CFseVitaleAppCtx.h"

class QString;
//================================================= CMedinTuxApp ==============================================================
class CFseVitaleApp : public QApplication ,  public CFseVitaleAppCtx
{

   Q_OBJECT

public:
    CFseVitaleApp(int & argc, char ** argv, const QString &appName = "");
    ~CFseVitaleApp(){m_pCVitaleReadThread    = 0;}
    void initialize( bool activate = TRUE );
    int  Run();
    void CouCouUp(const QString command,  QString text= QString::null);
    void CouCouUp(const QString& pathImage, int x, int y, int w, int h,const QString &text, const QColor &fore, const QColor &back);
    void CouCouDown();
    QString  GetNumeroFacture();
    QString  LoadVirtualCarteCps();
    QString  LoadVirtualCarteVitale();
    QString  LoadVirtualCarteVitale(const QString &fileName);
public slots:
    bool sendMessage( const QString &message, int timeout = 5000 );
    virtual void activateMainWidget();
    void CouCouIsUp(CCoolPopup * pCCoolPopup);
    void CouCouStop(CCoolPopup * pCCoolPopup);
    void CouCouDestroy();
    void stopVitaleThread();
    void Slot_OnTimer();
signals:
    void messageReceived( const QString& message );
    void Sign_PopupUp(CCoolPopup *);
    void Sign_PopupDown(CCoolPopup *);
    void Sign_PopupStop();
public:
    CVitaleReadThread*   m_pCVitaleReadThread;
    CCoolPopup*          m_pCCoolPopup;
    int                  m_count;
    QString              m_PathTheme;

};

#endif


extern CFseVitaleApp* G_pCFseVitaleApp;


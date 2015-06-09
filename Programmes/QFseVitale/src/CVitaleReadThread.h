/*********************************  CVitaleReadThread.h **************************
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
#ifndef CVITALE_READ_THREAD_H
#define CVITALE_READ_THREAD_H

#include <qobject.h>
#define TR QObject::tr

#include <qthread.h>
#include <qstring.h>
#include <qfile.h>
//================================================= CVitaleReadThread ==============================================================
class CVitaleReadThread : public QThread
{
public:
    CVitaleReadThread(const QString &pathExchFile,
                      const QString &cmd         = "READ_VITALE",
                      const QString &pathLog     = "/usr/local/galss/",
                      const QString &pathGalss   = "/usr/local/galss/galsvlux",
                      const QString &codePorteur = "1234",
                      const QString &ressPS      = "CPS",
                      const QString &ressLecteur = "Log_SV",
                      const QString &dateConsult = "");
    ~CVitaleReadThread();
    void start();
    void stop();
    int            IsThreadRun(){return m_Run;};
    virtual void   run()
                      { readSesamVitale();
                      };
    void readSesamVitale();
    int  getCount(){return m_Count;};
    void toExchgFile( QString stringlog);
public slots:

private:
    int      m_Run;
    int      m_Count;
    QString  m_Command;
    QString  m_PathLog;
    QString  m_PathGalss;
    QString  m_CodePorteur;
    QString  m_RessPS;
    QString  m_RessLecteur;
    QString  m_DateConsult;
    QString  m_LastErreur;
    QFile   *m_RetFile;
};

#endif

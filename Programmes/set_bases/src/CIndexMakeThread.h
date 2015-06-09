/********************************* CIndexMakeThread.cpp ***************************
 *                                                                                *
 *   #include "CIndexMakeThread.h"                                                *
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
#ifndef C_INDEXMAKETHREAD_H
#define C_INDEXMAKETHREAD_H

#include <qthread.h>
#include <qstring.h>
#include <qtextedit.h>
#include <qmutex.h>

#include "../../MedinTuxTools/CMedicaBase.h"

//================================================= CIndexMakeThread ==============================================================
class CIndexMakeThread : public QThread
{

public:
                   CIndexMakeThread(CMedicaBase *pCMedicaBase, QTextEdit *logWidget  = 0);
                  ~CIndexMakeThread();
    void           stop();
    void           start();
    int            IsRun();
    QString        GetCurrentStatus();
    long           GetCurrentProgress();
    long           GetTotalProgress();

    virtual void   run();

private:

    long         m_position;
    int          m_Run;
    CMedicaBase *m_pCMedicaBase;
    QString      m_Status;
    long         m_NbTotal;
    QTextEdit   *m_pQTextEditLogWidget;
    QMutex       m_mutex;
};

#endif




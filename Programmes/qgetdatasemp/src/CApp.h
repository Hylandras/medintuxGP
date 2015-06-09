/********************************* CApp.h *****************************************
 *                                                                                *
 *   #include "CApp.h"                                                            *
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

#ifndef C_APP_H
#define C_APP_H

#include <qobject.h>
#include <qstring.h>
#include <qtextedit.h>
#include "../../MedinTuxTools/CMedicaBase.h"
#include "../../MedinTuxTools/CSetupBase.h"
#include "../../MedinTuxTools/CMedicaBase.h"
#include "../../MedinTuxTools/C_Zlib/C_Zlib.h"

#include "../../MedinTuxTools/CW_EditText.h"

#include "CGetDatasempAppCtx.h"
#include <qapplication.h>
#include <qsqldatabase.h>
#include <qtextedit.h>
#include <qprogressbar.h>
#include <qprogressdialog.h>
#include <qthread.h>
#include <qsqlquery.h>
#include <qfile.h>


//================================================= CApp ==============================================================
class CApp : public QApplication , public CGetDatasempAppCtx, public CSetupBase
{
 public:

 enum UPDATE_MODE
    {   NoMode              = 0,
        Remove              = 1,
        Insert              = 2,
        Update              = 4,
        MajFiles            = 7,
        SqlFiles            = 8,
        AllUpdate           = 15,
        Force               = 16

    };
    CApp(int & argc, char ** argv);
    ~CApp();
    static int GotoDebug();
    QString read_line( QFile *file, char *buffer, int &len_read, Q_ULONG maxlen);
    void    ParseSQL_Dump(QSqlDatabase *dataBase, const QString &fname,   QProgressBar *pQProgressBar=0, CW_EditText *logWidget=0 );
    int     OutSQL_error( const QSqlQuery &cur, const char *messFunc =0, const char *requete =0);
    void    SetProgressPosition(long pos){m_position=pos;};
    long    GetProgressPosition(){return m_position/1000;};
    int     Decompresse( QString filename);
    void    displayProgression(QProgressBar * pQProgressBar);
    void    Datasemp_OutMessage( CW_EditText *logWidget, const QString &mess);

private:
    long          m_position;
    QString       m_LastError;
    QSqlDatabase *m_MySQL_DataBase;
};

extern CApp* G_pCApp;

#endif


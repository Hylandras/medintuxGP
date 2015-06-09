/********************************* CUpdateThread.h ********************************
 *                                                                                *
 *   #include "CUpdateThread.h"                                                   *
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
#ifndef C_CUpdateThread_H
#define C_CUpdateThread_H


#include <qsqldatabase.h>
#include <qthread.h>
#include <qtextedit.h>
#include <qstring.h>
//================================================= CUpdateThread ==============================================================
class CUpdateThread : public QThread
{

public:
    long    Init(QSqlDatabase *dataBase, const QString &srcPath, const QString &mainDir, const QString &dataDir, const QString &docDir, const QString &structDir, QTextEdit *logWidget);
    void    Datasemp_SetBases(QSqlDatabase *dataBase, const QString &srcPath);
    long    Datasemp_UpdateTables(QSqlDatabase *dataBase, const QString &fname) ;
    long    Datasemp_UpdateDocu(QSqlDatabase *dataBase, const QString &fname);
    long    GetCurrentLine();
    int     IsFini(){return m_Fini;};
    QString GetCurrentLog(){return m_OutLog;};
    long    GetCurrentProgress(long *nbTotal=0);
    QString GetCurrentStatus();
    void    StopSetBases();
    int     Datasemp_CreationTables(QSqlDatabase *dataBase, const QString &f_name);

    virtual void   run()
                      { //m_Fini = 1;
                        Datasemp_SetBases(m_dataBase, m_srcPath);
                      };
private:
    QSqlDatabase    *m_dataBase;
    QString          m_srcPath;
    QString          m_mainDir;
    QString          m_dataDir;
    QString          m_docsDir;
    QString          m_structDir;
    QString          m_OutLog;
    QTextEdit       *m_logWidget;

    long m_position;
    long m_NbPosition;
    long m_Continue;
    long m_CurrentLine;
    int  m_Fini;
    QStringList m_list_create;
    QStringList m_list_update;
    QStringList m_list_docs;
    QString m_Satus;
};
#endif

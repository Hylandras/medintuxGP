
/********************************* $FILES$.h ********************************
 *                                                                                *
 *                                                                                *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 2006 by Sevin Roland  and Data Medical Design   *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat ? l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la F?d?ration, 75752 PARIS cedex 15.   *
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

#ifndef C_DLG_QGETDATASEMP_H
#define C_DLG_QGETDATASEMP_H

#include "ui/Dlg_QGetDatasemp.h"
#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>
#include <qsqldatabase.h>
#include <qprocess.h>
#include <qtimer.h>
#include "CFtpTools.h"
#include "CFtpInfo.h"
#include "../../MedinTuxTools/CMedicaBase.h"

class C_Dlg_QGetDatasemp : public Dlg_QGetDatasemp
{
  Q_OBJECT

public:
  C_Dlg_QGetDatasemp(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  ~C_Dlg_QGetDatasemp();

  enum ASK_MODE {
        askIfSame,
        forceIfSame
    };
  /*$PUBLIC_FUNCTIONS$*/
  virtual UINT16        HexToUINT( const char * str );
  virtual QString       UINTtoHex( UINT16 val );
  virtual QString       PassWordDecode( QString str_to_decode );
  virtual QString       PassWordEncode( QString & pass );
  virtual QString       GetDatasempDataVersion();
  virtual QString       GetDatasempIndexVersion();
  virtual int           InitCMedicaBase();
  virtual QString       errorToString( CFtp::Error error );
  virtual QString       commandToString( CFtp::Command command );
  virtual QString       GetDatasempVersionFromFileName( QString name );
  virtual long          GetLastVersionLocalDownloadFile( QString & fileName, QString ident_patern );
  virtual long          RecupererLastFTP_VersionFile( const QString & action, QString & fileName, const QString & ident_patern, CFtpInfo & last_info );
  virtual long          GetNbProgressFromFileName( const QString & fileName );
  virtual QString       GetLastDatasempNumVersionFromLocalFile(QString ident_patern);
  virtual void          UploaderFile(const QString &action, QString &lastFileOnServeur, QString ident_patern);
  virtual QString       setNbSteepFromLastVersionDatasempLocalFile(const QString &action, QString ident_patern);
public slots:
  /*$PUBLIC_SLOTS$*/
  virtual void          pushButton_ComputeClefAcces_clicked();
  virtual void          pushButton_tester_clicked();
  virtual void          pushButton6_clicked();
  virtual void          pushButton_Upload_clicked();
  virtual void          SetDatasempDataVersion( QString version );
  virtual void          SetDatasempIndexVersion( QString version );
  virtual void          Save_Param();
  virtual void          pushButton_Connexion_clicked();
  virtual void          pushButtonLocal_clicked();
  virtual void          slotFTP_Finished( int id, bool error );
  virtual void          slotFTP_Started( int id );
  virtual void          setProgress(int,int);
  virtual void          toLog( const QString & str );
  virtual void          slot_LlistInfo( const QUrlInfo & objet );
  virtual void          slotFTP_state_changed( int state );
  virtual void          ListerContenu( const QString & action );
  virtual QString       SetFileNameToNbProgress( QString filename, long nb );
  virtual void          IntegrerFileDatasemp( const QString & action, QString & fileName, long last_version , ASK_MODE askMode   = C_Dlg_QGetDatasemp::askIfSame  );
  virtual void          IntegrerFileMedica( const QString & action, QString & fileName, long last_version ,   ASK_MODE askMode   = C_Dlg_QGetDatasemp::askIfSame );
  virtual void          VerifyUserDroits( const QString & action, const QString & fileName );
  virtual void          TelechargeUserList( const QString & action, const QString & fileName );
  virtual void          TelechargeFile( const QString & action, QString & fileName, long last_version, CFtpInfo & last_info );
  virtual void          Chdir( const QString & action );
  virtual void          Login( const QString & action );
  virtual void          Deconnecter( const QString & action );
  virtual void          Connecter( const QString & action );
  virtual void          slot_DoSequenceur();
  virtual void          LocalMedintuxActionList_Set();
  virtual void          LocalActionList_Set();
  virtual void          FtpActionList_Set();
  virtual void          FtpMedintuxActionList_Set();
  virtual void          FtpActionUpload_Set();
  virtual void          SetIniFileOnDatasemp();
  virtual void          ResetSequenceur();

protected:
  /*$PROTECTED_FUNCTIONS$*/
    int          m_DroitsOk;
    QString      m_Debug;
    QString      m_DefaultStringClefAcces;
    QString      m_UsersList;
    QString      m_DatasempKey;
    QString      m_LastMedinTuxFile;
    long         m_LastVersionMedinTux;
    QString      m_LastDatasempFile;
    long         m_LastVersionDatasemp;
    CMedicaBase *m_pCMedicaBase;
    QFile        m_DownloadFile;
    QFile        m_UploadFile;
    long         m_LastVersionMedica;
    QString      m_LastMedicaFile;
    int          m_ActionIndex;
    QTimer       m_ActionTimer;
    QString      m_CurrentAction;
    QStringList  m_ActionList;
    CommandMap   m_CommandMap;
    QVL_FileInfo m_QVL_FileInfo;
    QString      m_FtpFolder;
    QString      m_FtpLogPass;
    QString      m_FtpLogName;
    QString      m_FtpPort;
    QString      m_FtpAdress;
    CFtpTools   *m_pFtpTools;
    CFtpInfo     m_LastDatasempInfo;
    CFtpInfo     m_LastIndexMedica;

protected slots:
  /*$PROTECTED_SLOTS$*/

};

#endif


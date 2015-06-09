/***************************************************************************
 *   Copyright (C) 2004 by Sevin Roland                                    *
 *   upsis@club-internet.fr                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef C_DLGMAINDIALOG_H
#define C_DLGMAINDIALOG_H

#include "Dlg_MainDialog.h"


#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>
#include <qsqldatabase.h>
#include <qprocess.h>
#include <qtimer.h>
#include <qobject.h>
#include "CUpdateThread.h"
#include "../../MedinTuxTools/CMedicaBase.h"
#include "CIndexMakeThread.h"
#include "../../drtux/src/CMoteur_Base.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class CW_EditText;
class QLabel;
class QProgressBar;
class QPushButton;
class QTabWidget;
class QWidget;
class QGroupBox;
class QCheckBox;
class QLineEdit;
class QComboBox;
class QProcess;

class C_DlgMainDialog : public Dlg_MainDialog
{
  Q_OBJECT

public:
  C_DlgMainDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  ~C_DlgMainDialog();
  /*$PUBLIC_FUNCTIONS$*/
  virtual void          putInFront();
  virtual QString       keepOnlyNumber(const QString &strIn);
  virtual QString       GetDatasempBaseVersion();
  virtual QString       GetDatasempIndexVersion();
  virtual QString       GetMAJVersion();
  virtual int           setMajVersionButton();
  virtual int           GetDatasempMode();
  virtual QSqlDatabase * DataSempBaseInit();
  virtual bool          IsThisExecutableExist( const QString & exe_name );
  virtual bool          IsExecutablesExist(QStringList &exeNotFoundList);
  virtual QString       getTargetSrcPath( const QString & target );
  virtual QString       IsSesamVersionExist();
  virtual void          setBase(const QString &sqlFile  = "" );
  virtual int           comboBox_IP_Fav_insertItem( const QString & item_txt , QComboBox *pQComboBox);
  virtual QString       TestBases(  const QString &driver, const QString &base, const QString &userName,  const QString &passWord,  const QString &hostName,  const QString &port);
  virtual void          StopCIndexMakeThread();
  virtual QString       getExtension(const QString &file);
  virtual void          copyTables_Api(QString src_Dir,  QString dest_Dir);
  virtual void          copyLinux_Api(QString src_Dir,  QString bin_Dir,  QString cpp_Dir, QString sesam_Dir);
  virtual void          copyWindw_Api(QString src_Dir,  QString bin_Dir,  QString cpp_Dir, QString sesam_Dir);
  virtual bool          TestAndSet_SesamVIniFiles(const QString &fname, const QString &sesam_Dir, const QString &src_Dir, const QString &dest_Dir);
  virtual bool          TestAndSet_HeaderDevFiles(const QString &fname, const QString &src_Dir,   const QString &cpp_Dir);
  virtual void          reinitLog(int mode = 0);
  virtual void          toLog( const QString &mess);
  virtual void          logOutCompil(const QByteArray &data);
  virtual void          abortedMessDisplay(const QString &base);
  virtual void          setGalsINI();
  virtual void          destroy(bool destroyWindow = TRUE, bool destroySubWindows = TRUE);
  virtual QString       PluginExe(       QObject* pQObject, const char* pluginScript, int waitFlag  = CMoteurBase::endWait );
  virtual QString       PluginExe(       QObject          *pQObject,
                                         const QString    &pathPlugin,  // Chemin de l'executable plugin �actionner sans l'extension .exe
                                         const QString    &pathAppli,   // Chemin de l'executable appelant (DrTux)
                                         const QString    &pathIni,     // Chemin d'un eventuel fichier de configuration pour XXXX.ini pour l'executable plugin ou %
                                         const QString    &maskExch,    // texte du fichier d'exchange
                                         const QString    &guid,
                                         const QString    &dosPk,
                                         const QString    &terPk,
                                         const QString    &ordPk ,
                                         QStringList      &param,
                                         int               waitFlag  = CMoteurBase::endWait );
  bool                  fichiersinipmd(QString &userName,QString &passWord,QString &hostName,QString &pathsetbases);
  QString               PassWordEncode(QString &pass);
  QString               PassWordDecode(QString str_to_decode);
  QString               UINTtoHex( UINT16 val);
  UINT16                HexToUINT( const char *str);
  QString               tryToFindSdkQt4();
  QString               tryToFindQt3();
  QStringList           GetListBasesInMySql(QString *err, const QStringList &baseListToSet);
  QString               m_SousAppleSeMettreEnModeAdmin;
  QString               m_SousLinuxSeMettreEnModeAdmin;
  QString               m_PluginRun;
  QString               m_MessageNoConnect;
  QString               m_CurrentCompil;
  QWidget              *m_pTabQWidgetMaster;
  QWidget              *m_pTabQWidgetNomade;
  QWidget              *m_pTabQWidgetReverse;
  QWidget              *m_pTabQWidgetBases;
  QWidget              *m_pTabQWidgetCompil;
  QWidget              *m_pTabQWidgetSesam;
  QWidget              *m_pTabQWidgetTherap;
  int                   m_MakeExeRun;
  int                   m_compil_In;
  QStringList           m_compilList;
  QProcess             *m_Apropos_Proc;

public slots:
  /*$PUBLIC_SLOTS$*/
  virtual void          Slot_TimedCompilation();
  virtual void          Slot_TimedSetBase();
  virtual void          Slot_close();
  virtual void          Slot_SauverLesMeubles();
  virtual void          Slot_actionApropos();
  virtual void          Slot_actionAproposDisplay();
  virtual void          tryToStopAPropos();
  virtual void          pushButton_Load_SQL_clicked();
  virtual void          pushButton_Qt3_clicked();
  virtual void          pushButton_Qt4Sdk_clicked();
  virtual void          checkBox_Nomadisme_stateChanged(int );
  virtual void          pushButton_LireVitale_clicked();
  virtual void          pushButtonMakeSQL_clicked();
  virtual void          eraseDirectory( QString dir_to_erase );
  virtual void          copyDirectory( QString & src_PrefixDir, QString dir_to_explore, QString images_DstDir );
  virtual void          copyDatasempImages();
  virtual void          pushButtonImagesCopy_clicked();
  virtual void          SetDatasempIndexVersion( QString & version );
  virtual void          SetButtonIndexVidal( int mode );
  virtual void          pushButton_MajBases_clicked();
  virtual void          pushButtonConnexionAppl_clicked();
  virtual void          pushButtonSwitchGetDatasemp_clicked();
  virtual void          StopCUpdateThread();
  virtual void          checkBox_SQL_MAJ_stateChanged( int );
  virtual void          checkBox_STD_MAJ_stateChanged( int );
  virtual void          timerDisplayDone();
  virtual void          verifieDatasempDir( const QString & mainDir, QString & dir );
  virtual void          pushButton_SetDatasemp_clicked();
  virtual void          pushButtonJustOneTable_clicked();
  virtual void          pushButton_LancerDrTux_clicked();
  virtual void          pushButton_SetBases_clicked();
  virtual void          updateExit();
  virtual void          updateText();
  virtual void          updateError();
  virtual void          execute( const QString & command , const QString curDir ="");
  virtual void          Compilation( const QString & path , const QString & target);
  virtual void          pushButton_compilation_clicked();

  virtual void          pushButtonRemove_clicked();
  virtual void          pushButton_FavPlus_clicked();
  virtual void          comboBox_IP_Fav_highlighted( const QString & text );
  virtual void          comboBox_IP_Fav_activated( const QString & text );
  virtual void          pushButtonRemove_2_clicked();
  virtual void          pushButton_FavPlus_2_clicked();
  virtual void          comboBox_IP_Fav_2_highlighted( const QString & text );
  virtual void          comboBox_IP_Fav_2_activated( const QString & text );
  virtual void          pushButtonRemove_3_clicked();
  virtual void          pushButton_FavPlus_3_clicked();
  virtual void          comboBox_IP_Fav_3_highlighted( const QString & text );
  virtual void          comboBox_IP_Fav_3_activated( const QString & text );

  virtual void          fillFavoris(QComboBox *pQComboBox);
  virtual void          saveFavoris(QComboBox *pQComboBox);


  virtual void          SetTitle( QString n_Version );
  virtual void          pushButton_InstallSesam_clicked();
  virtual void          Slot_pushButton_setGalsINI_clicked();
  virtual void          pushButton_IndexerDatasemp_clicked();
  virtual void          IndexTimerDisplayDone();

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/

};

#endif


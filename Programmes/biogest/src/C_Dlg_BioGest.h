#ifndef C_Dlg_BioGest_H
#define C_Dlg_BioGest_H

#include <QDebug>
#include <QMap>
#include <QMainWindow>
#include "../../MedinTuxTools-QT4/C_Utils_Html.h"
#include "../../MedinTuxTools-QT4/C_QWebView/C_QWebView.h"
#include "ui/ui_C_Dlg_BioGest.h"
#include "C_BaseBioGest.h"

namespace Ui {
    class MainWindow;
    class QMenuBar;
    class QToolBar;
    class QAction;
}
    class QTimer;
    class QTreeWidget;
    class QUrl;
    class QTreeWidgetItem;
    class CookieJar;
    class QNetworkDiskCache;
    class QProgressBar;
    class QLabel;
    class QWebView;
    class QSignalMapper;
    class C_ScruteFTP;
    class CPatient;
    class C_ScruteDB;
    class C_ClickableLed;
//==================================  C_Dlg_BioGest ============================================================
class C_Dlg_BioGest : public QMainWindow
{
    Q_OBJECT

public:
    explicit C_Dlg_BioGest(QWidget *parent = 0);
    ~C_Dlg_BioGest();
     void setStatusLabeMessage(const QString & mess);
     Ui::MainWindow     *m_pGUI;


private:
     QString             getHtmlResultsFromPatientPk(const QString & pk);
     QString             decodedPassword(QString pass);
     QString             encodedPassword(QString pass);
     C_QWebView         *m_webView;
     C_ScruteFTP        *m_pC_ScruteFTP;
     C_ScruteDB         *m_pC_ScruteDB;
     C_ClickableLed     *m_FtpStateIndicator;
     C_ClickableLed     *m_HelpIndicator;
     QLabel             *m_pQLabel;
     QWidget            *m_Wdg_HprimHtml;
     QWidget            *m_Wdg_SitesLabo;
     QWidget            *m_Wdg_HprimBrut;
     QWidget            *m_Wdg_ConfigTab;
     QWidget            *m_Wdg_ChangeTab;
     QProcess           *m_Apropos_Proc;
     QProcess           *m_MaskEditor_Proc;
     QStringList         m_servicesList;
public slots:
     int  Slot_fill_CPatientMap(QMap <QString,CPatient> &mapPk_Cpatient);
     void Slot_treeWidget_Patients_itemClicked( QTreeWidgetItem * , int);
     void closeEvent(QCloseEvent *event);
private slots:
    void Slot_retranslateUi();
    void Slot_onQuit();
    void Slot_lauch_C_ScruteFTP();
    void Slot_timerStateIndicator_clicked();
    void Slot_pushButton_logErase_Clicked();
    void Slot_m_HelpIndicator_clicked();
    void Slot_dateEdit_Deb_dateChanged ( const QDate &  );
    void Slot_dateEdit_Fin_dateChanged ( const QDate &  );
    void Slot_FTP_status(int status);
    void Slot_comboBox_modeles_Activated(const QString &str );
    void Slot_comboBox_ftp_fav_Activated(const QString &str );
    void Slot_comboBox_ip_fav_Activated( const QString &str );
    void Slot_treeWidget_Patients_itemDoubleClicked( QTreeWidgetItem * , int);
    void Slot_treeWidget_Patients_itemSelectionChanged ();
    void Slot_treeWidget_Patients_customContextMenuRequested (const QPoint &);
    void Slot_checkBox_ftp_daemon_enabled_stateChanged( int );
    void Slot_checkBox_traceEnabled_stateChanged( int state);
    void Slot_checkBox_justPatientsWithResults_stateChanged( int state);
    void Slot_checkBox_ftp_distantsDelete_stateChanged( int state);
    void Slot_checkBox_ftp_localsDelete_stateChanged( int state);
    void Slot_lineEdit_ftp_scrutation_time_editingFinished();
    void Slot_lineEdit_ip_scrutation_time_editingFinished();
    void Slot_lineEdit_resultsLimit_editingFinished();
    void Slot_pushpushButton_MaskEdit_Clicked();
    void Slot_pushButtonDateDeb_Clicked();
    void Slot_pushButtonDateFin_Clicked();
    void Slot_fillPatientList();
    void Slot_comboBox_Service_Activated(const QString &text );
    void Slot_checkBox_filtrerDates_stateChanged( int state);
    void Slot_lineEdit_PatientSearch_textChanged(const QString &);
    void Slot_pushButton_playHprim_Clicked();
    void Slot_FilesAnalyseFinished();
    void Slot_DB_AnalyseFinished(const QString &message);
    void Slot_pushButton_ip_connect_Clicked();
    void Slot_CouCouDestroy();

};

#endif // C_Dlg_BioGest_H

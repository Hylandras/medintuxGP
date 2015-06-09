#ifndef APIDIALOG_H
#define APIDIALOG_H
#include "ui_apidialog.h"
#include "configdialog.h"
#include <QDialog>
#include <QtGui>
#include <QtCore>
#include <QtSql>
#include <QProcess>

class apidialog : public QDialog, public Ui::ApiDialog
{
    Q_OBJECT
    public :
        apidialog(QWidget * parent = 0, Qt::WindowFlags f = 0 );
        ~apidialog();
    protected :
        bool event ( QEvent * e );
    signals:
        void sigUsersCombo();
    public slots:
        void setUsersCombo();
    protected slots:
        void Slot_ActivateMainWindow();
        bool dansdocuments();
        void configdialog();
        void changeLabel();
        void preView();
        void postView();
        void Slot_APropos();
        void Slot_Apropos_Proc_finished (int,  QProcess::ExitStatus);
    private :
        int         m_compteur;
        QString     m_namesIntegrated;
        QString     m_texteblob;
        QString     m_login;
        QString     m_resources;
        QStringList m_listelogin;
        QStringList m_titres;
        QStringList m_dossCumulatedList;
        QHash<QString,QString> m_hashHtml;
        config *m_config;
        QString m_lastHeadsId;
        QString m_lastBlobId;
        QString fonctiontitre(QStringList&);
        QString replaceInHtml(const QString & text);
        QProcess           *m_Apropos_Proc;
        void searchOfLastIds();
        //char * choosenCodec();
};

#endif

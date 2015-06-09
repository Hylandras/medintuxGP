#ifndef POSTVIEW_H
#define POSTVIEW_H

#include "ui_postview.h"
#include <QWidget>
#include <QHash>

class PostView:public QWidget,public Ui::PostviewWidget
{
    Q_OBJECT
    public:
        PostView (QWidget * parent = 0, QString lastid = QString(), QString user = QString());
        ~PostView ();

    private:
        QString m_lastid;
        QString m_user;
        QHash<QString,QString> m_hashOfHeads;
        QHash<QString,QString> m_hashprimkeyblob;
        QHash<QString,QString> m_hashprimkeyiddossier;
        bool searchLastResults();
        void displayResults(); 
        QString getNameFromId(const QString iddossier);
};

#endif


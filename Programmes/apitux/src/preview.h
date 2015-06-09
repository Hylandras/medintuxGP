#ifndef PREVIEW_H
#define PREVIEW_H

#include "ui_preview.h"
#include <QWidget>
#include <QHash>

class Preview:public QWidget,public Ui::PreviewWidget
{
    Q_OBJECT
    public:
        Preview (QWidget * parent = 0);
        ~Preview ();
    protected slots :
        void displayFile(const QString&);
        void modifytext();
    private:
        void setCombo();
        QString m_resources;
        QHash<int,QString> m_hashIntFiles;
};

#endif


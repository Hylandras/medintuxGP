/****************************************************************************
** Form interface generated from reading ui file '..\..\MedinTuxTools\DlgGetString.ui'
**
** Created: jeu. 12. mai 07:19:22 2011
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FORMDLGGETSTRING_H
#define FORMDLGGETSTRING_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QLineEdit;
class QTextBrowser;

class FormDlgGetString : public QDialog
{
    Q_OBJECT

public:
    FormDlgGetString( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~FormDlgGetString();

    QPushButton* pushButtonOK;
    QPushButton* pushButtonCancel;
    QLineEdit* lineEditString;
    QTextBrowser* textBrowserInfo;

public slots:
    virtual void pushButtonCancel_pressed();
    virtual void pushButtonOK_clicked();

protected:
    QGridLayout* FormDlgGetStringLayout;
    QSpacerItem* spacer1;

protected slots:
    virtual void languageChange();

};

#endif // FORMDLGGETSTRING_H

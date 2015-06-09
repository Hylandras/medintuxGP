/****************************************************************************
** Form interface generated from reading ui file 'Dlg_GetCodePorteur.ui'
**
** Created: dim. 17. avr. 09:34:48 2011
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef DLG_GETCODEPORTEUR_H
#define DLG_GETCODEPORTEUR_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;
class QLineEdit;

class Dlg_GetCodePorteur : public QDialog
{
    Q_OBJECT

public:
    Dlg_GetCodePorteur( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~Dlg_GetCodePorteur();

    QLabel* labelIconePs;
    QPushButton* pushButtonCancel;
    QPushButton* pushButtonOk;
    QLineEdit* lineEditGetCode;
    QLabel* textLabel2;

public slots:
    virtual void pushButtonOk_clicked();
    virtual void pushButtonCancel_clicked();

protected:
    QGridLayout* Dlg_GetCodePorteurLayout;
    QGridLayout* layout1;

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // DLG_GETCODEPORTEUR_H

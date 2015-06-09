/****************************************************************************
** Form interface generated from reading ui file '..\..\MedinTuxTools\Dlg_PasswordGet.ui'
**
** Created: jeu. 12. mai 07:19:21 2011
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef DLG_PASSWORDGET_H
#define DLG_PASSWORDGET_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QCheckBox;
class QPushButton;
class QLineEdit;
class QLabel;

class Dlg_PasswordGet : public QDialog
{
    Q_OBJECT

public:
    Dlg_PasswordGet( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~Dlg_PasswordGet();

    QCheckBox* checkBoxMemorise;
    QPushButton* pushButtonOK;
    QPushButton* pushButtonAnnuler;
    QLineEdit* lineEditPassword;
    QLabel* textLabel_NbCarPass;

protected:
    QGridLayout* Dlg_PasswordGetLayout;
    QGridLayout* layout4;
    QHBoxLayout* layout3;

protected slots:
    virtual void languageChange();

};

#endif // DLG_PASSWORDGET_H

/****************************************************************************
** Form interface generated from reading ui file 'RubObservation.ui'
**
** Created: ven avr 23 21:52:31 2010
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FORMRUBOBSERVATION_H
#define FORMRUBOBSERVATION_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QLabel;
class QComboBox;

class FormRubObservation : public QWidget
{
    Q_OBJECT

public:
    FormRubObservation( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~FormRubObservation();

    QPushButton* textLabel_NbRub;
    QLabel* textLabel_Age;
    QPushButton* pushButtonSave;
    QLabel* textLabel1;
    QPushButton* pushButtonDelete;
    QLabel* textLabel_UserName;
    QPushButton* pushButtonPrint;
    QPushButton* pushButtonRubDateChange;
    QComboBox* comboBox_RubName;

protected:
    QGridLayout* FormRubObservationLayout;

protected slots:
    virtual void languageChange();

};

#endif // FORMRUBOBSERVATION_H

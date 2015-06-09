/****************************************************************************
** Form interface generated from reading ui file '..\..\MedinTuxTools\Calendar\Dlg_Calendar.ui'
**
** Created: lun. 24. août 23:41:33 2009
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef DLG_CALENDAR_H
#define DLG_CALENDAR_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>
#include <qvbox.h>
#include <qiconset.h>
#include "../../MedinTuxTools/Calendar/MyDateTable.h"
#include "../../MedinTuxTools/Calendar/C_Horloge.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class CMaskedLineEdit;
class QPushButton;
class QComboBox;
class QLabel;
class QTimeEdit;

class Dlg_Calendar : public QDialog
{
    Q_OBJECT

public:
    Dlg_Calendar( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~Dlg_Calendar();

    QPushButton* pushButtonMonthPlus;
    QPushButton* pushButtonYearMoins;
    CMaskedLineEdit* cMaskedLineEditDate;
    QComboBox* comboBoxAn;
    QPushButton* pushButtonYearPlus;
    QComboBox* comboBoxMois;
    QPushButton* pushButtonMonthMoins;
    QPushButton* pushButtonMenu;
    QLabel* textLabel_Heure;
    QTimeEdit* timeEdit_Heure;

    C_Horloge *m_pC_Horloge;
    QStringList m_PopupListItem;
    QString m_CurrentPopupItem;
    QString m_RetPopupItem;
    Q3PopupMenu *m_pQPopupMenu;
    MyDateTable *m_pMyDateTable;

    virtual int setDateTimeNumeric_yyyyMMjj_Order( const QString & dateTime );
    virtual QDateTime getDateTime();
    virtual QString get_SQL_DateTime();
    virtual QString DoPopupList( QStringList & list );

public slots:
    virtual void DateMenu_setIcon( const QPixmap & rQPixmap );
    virtual void Month_setIcon( const QPixmap & moinsQPixmap, const QPixmap & plusQPixmap );
    virtual void Year_setIcon( const QPixmap & moinsQPixmap, const QPixmap & plusQPixmap );
    virtual void setDateDeb( QDate * pdate );
    virtual int setDateTimeNatural_jjMMyyyy_Order( const QString & dateTime );
    virtual void setHeure( const QString & heure );
    virtual void setHeure( int h, int m, int s, int ms );
    virtual void setHeure( const QTime & time );
    virtual void SelectDateAll();
    virtual void Slot_OnReturnPressed();
    virtual void Slot_OnTableDoubleClicked();
    virtual void Slot_OnTableClicked();
    virtual void Slot_OnTextDateChanged( const QString & );
    virtual void setDate( const QDate & date );
    virtual void Slot_OndateChanged( QDate date );
    virtual void AdjustInterfaceToDate( const QDate & date );
    virtual QDate getDate();
    virtual void pushButtonYearPlus_clicked();
    virtual void pushButtonMonthPlus_clicked();
    virtual void pushButtonYearMoins_clicked();
    virtual void pushButtonMonthMoins_clicked();
    virtual void comboBoxMois_activated( const QString & );
    virtual void comboBoxAn_activated( const QString & an );
    virtual void comboBoxAn_textChanged( const QString & an );
    virtual void setComboAnOnDate( const QDate & date );
    virtual void setComboAnOnDate_Offset( const QDate & date, int neg_Offset, int pos_Offset );
    virtual void setMenuPreselectDateOn();
    virtual void setMenuPreselectDateOff();
    virtual void setMenuPreSelectDate( const QStringList & itemList );
    virtual void Slot_pushButtonMenu_clicked();
    virtual void OnMenuActivated( int id );
    virtual void GetPopupItem();

protected:
    QGridLayout* Dlg_CalendarLayout;
    QGridLayout* layout3;
    QGridLayout* layout2;
    QHBoxLayout* layout3_2;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;
    QPixmap image1;
    QPixmap image2;
    QPixmap image3;
    QPixmap image4;

    void init();

};

#endif // DLG_CALENDAR_H

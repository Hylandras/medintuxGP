#ifndef C_DLG_PLAGESDISPO_H
#define C_DLG_PLAGESDISPO_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
    class C_Dlg_PlagesDispo;
}

class C_Dlg_PlagesDispo : public QDialog {
    Q_OBJECT
public:
    C_Dlg_PlagesDispo(QWidget *parent = 0);
    ~C_Dlg_PlagesDispo();

     Ui::C_Dlg_PlagesDispo *ui;

     QString    m_sDatePlage;
     QString    m_sHeurePlage;
     int        m_Col;

 public slots:
     void Slot_TreeWidgetPlages_DoubleClicked( QTreeWidgetItem * , int);


protected:
    void changeEvent(QEvent *e);

private:

};

#endif // C_DLG_PLAGESDISPO_H


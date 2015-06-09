#ifndef C_DLG_CHANGEMENTS_H
#define C_DLG_CHANGEMENTS_H

#include <QDialog>

namespace Ui {
    class C_Dlg_Changements;
}

class C_Dlg_Changements : public QDialog
{
    Q_OBJECT

public:
    explicit C_Dlg_Changements(QWidget *parent = 0);
    ~C_Dlg_Changements();
    QString getText();
private:
    Ui::C_Dlg_Changements *ui;
};

#endif // C_DLG_CHANGEMENTS_H

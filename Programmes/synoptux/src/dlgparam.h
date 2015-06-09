#ifndef DLGPARAM_H
#define DLGPARAM_H

#include <QDialog>

namespace Ui {
    class DlgParam;
}

class DlgParam : public QDialog {
    Q_OBJECT
public:
    DlgParam(QWidget *parent = 0);
    ~DlgParam();

    void   ChargeParam(QString typeParam);


protected:
    void changeEvent(QEvent *e);

private slots:
    void EcrireParam();

private:
    Ui::DlgParam *ui;

    QString nomficPARAM;
};

#endif // DLGPARAM_H

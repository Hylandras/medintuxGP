#ifndef DLGRESP_H
#define DLGRESP_H

#include <QDialog>
#include <QtSql>
#include "ui_dlgresp.h"


namespace Ui {
    class DlgResp;
}

class DlgResp : public QDialog {
    Q_OBJECT
public:
     DlgResp(QWidget *parent = 0);
    ~DlgResp();

     Ui::DlgResp *ui;
     QString         RespEnCours;

public slots:
    void   ChargeResp();

protected:
    void changeEvent(QEvent *e);

protected slots:
    void ValideResp();
    void AnnuleResp();
    void ClearFormulaire();
    void ChoixCouleurFond();
    void selectResp( QTreeWidgetItem * , int );

};

#endif // DLGRESP_H

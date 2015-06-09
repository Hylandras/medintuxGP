#ifndef DLGBOX_H
#define DLGBOX_H

#include <QDialog>
#include <QtSql>
#include "ui_dlgbox.h"


namespace Ui {
    class DlgBox;
}

class DlgBox : public QDialog {
    Q_OBJECT
public:
    DlgBox(QWidget *parent = 0);
    ~DlgBox();

     Ui::DlgBox *ui;
     QString         BoxEnCours;

public slots:
    void   ChargeBox();

protected:
    void changeEvent(QEvent *e);

protected slots:
    void ValideBox();
    void AnnuleBox();
    void ClearFormulaire();
    void ChoixCouleurFond();
    void ChoixCouleurTitre();
    void selectBox( QTreeWidgetItem * , int );

};

#endif // DLGBOX_H

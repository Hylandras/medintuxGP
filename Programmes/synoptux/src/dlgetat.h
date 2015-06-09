#ifndef DLGETAT_H
#define DLGETAT_H

#include <QDialog>
#include <QtSql>
#include "ui_dlgetat.h"


namespace Ui {
    class DlgEtat;
}

class DlgEtat : public QDialog {
    Q_OBJECT
public:
     DlgEtat(QWidget *parent = 0);
    ~DlgEtat();

     Ui::DlgEtat *ui;
     QString         EtatEnCours;

public slots:
    void   ChargeEtat();

protected:
    void changeEvent(QEvent *e);

protected slots:
    void ValideEtat();
    void AnnuleEtat();
    void ClearFormulaire();
    void ChoixCouleurFond();
    void selectEtat( QTreeWidgetItem * , int );

};

#endif // DLGETAT_H

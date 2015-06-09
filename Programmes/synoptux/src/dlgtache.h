#ifndef DLGTACHE_H
#define DLGTACHE_H

#include <QDialog>
#include <QtSql>
#include "ui_dlgtache.h"


namespace Ui {
    class DlgTache;
}

class DlgTache : public QDialog {
    Q_OBJECT
public:
    DlgTache(QWidget *parent = 0);
    ~DlgTache();

     Ui::DlgTache *ui;
     QString         TacheEnCours;

public slots:
    void   ChargeTache();

protected:
    void changeEvent(QEvent *e);

protected slots:
    void Slot_MakeMacro();
    void ValideTache();
    void ValideEtats();
    void AnnuleTache();
    void ClearFormulaire();
    void ChoixCouleurFond();
    void ChoixCouleurAlarme();
    void selectTache( QTreeWidgetItem * , int );
    void selectEtat( QTreeWidgetItem * , int );


};

#endif // DLGTACHE_H

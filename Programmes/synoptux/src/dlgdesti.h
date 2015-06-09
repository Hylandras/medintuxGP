#ifndef DLGDESTI_H
#define DLGDESTI_H


#include <QDialog>
#include <QtSql>
#include <QTreeWidgetItem>
#include <QMessageBox>

namespace Ui {
    class DlgDesti;
}

class DlgDesti : public QDialog {
    Q_OBJECT
public:
    DlgDesti(QWidget *parent = 0);
    ~DlgDesti();

    Ui::DlgDesti *ui;

    void ChargeDesti(QString NumEncours);
    QString         dNumEnCours;
    QString         dTypeDeSortie;
    QString         dTypeSaisie;
    QString         dExeCalendrier;

protected:
    void changeEvent(QEvent *e);

protected slots:
    void ValideDesti();
    void selectDesti(QTreeWidgetItem * , int);
    void AnnuleDesti();
    void majListeDesti();
    void ModifHeure();

private:


};

#endif // DLGDESTI_H

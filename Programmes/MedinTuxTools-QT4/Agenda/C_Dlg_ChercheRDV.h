#ifndef C_DLG_CHERCHERDV_H
#define C_DLG_CHERCHERDV_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "CMoteurAgenda.h"

namespace Ui {
    class C_Dlg_ChercheRDV;
}
//=================================== C_Dlg_ChercheRDV ===========================================
class C_Dlg_ChercheRDV : public QDialog {
    Q_OBJECT
public:
     C_Dlg_ChercheRDV(CMoteurAgenda* pCMoteurAgenda, QString nom_prenom_guid  = "" , QWidget *parent = 0);
    ~C_Dlg_ChercheRDV();

    //void changeEvent(QEvent *e);
    void Slot_createTreeViewPatients();
    void initListePatient( const QString & qstr_nom, const QString & qstr_prenom, const QString & guid);
    void getListePatient();
    void keyPressEvent ( QKeyEvent * event );
    void Valid_Patient_Selected(QTreeWidgetItem *pQTreeWidgetPatientItem);

    QFrame      *m_pQFrameListPatients;
    QTreeWidget *m_pTreeWidgetPatients;
    QLabel      *m_pQLabelStatus;

public slots:
    void Slot_TreeWidgetPatients_DoubleClicked( QTreeWidgetItem *pQTreeWidgetPatientItem , int);
    void Slot_imprimer();
    void Slot_fermer();
    void Slot_actualiserListe();
    void Slot_lineEdit_Prenom_textChanged(const QString &);
    void Slot_lineEdit_Nom_textChanged(const QString &);


protected:
    void changeEvent(QEvent *e);

private:
    Ui::C_Dlg_ChercheRDV   *m_ui;
    CMoteurAgenda          *m_pCMoteurAgenda;
};

#endif // C_DLG_CHERCHERDV_H

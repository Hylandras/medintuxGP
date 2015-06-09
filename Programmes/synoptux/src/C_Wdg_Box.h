#ifndef BOX_H
#define BOX_H

#include <QWidget>
#include "ui_box.h"
#include <QHBoxLayout>
#include <QPushButton>
#define  NB_BOX_MAX 150             // BUGBUG
namespace Ui {
    class C_Wdg_Box : public Ui_Box {};
}

class C_Wdg_Box : public QWidget {
    Q_OBJECT
public:
     C_Wdg_Box(QWidget *parent = 0);
    ~C_Wdg_Box();
    QString  getKey(){return m_UniqueCode;}
    void     setKey(const QString &value){m_UniqueCode=value;}
    Ui::C_Wdg_Box *ui;
    // QHBoxLayout *m_horizonLayoutResp;
    // QPushButton *m_ButtonNomMedResponsable;

protected:
    void changeEvent(QEvent *e);

private:
    QString m_UniqueCode;        // normalement c'est le pk de la table
signals:
    void Sign_byebyeBox(C_Wdg_Box* );
};

#endif // BOX_H

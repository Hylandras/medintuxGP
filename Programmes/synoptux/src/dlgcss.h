#ifndef DLGCSS_H
#define DLGCSS_H

#include <QDialog>

namespace Ui {
    class DlgCSS;
}

class DlgCSS : public QDialog {
    Q_OBJECT
public:
    DlgCSS(QWidget *parent = 0);
    ~DlgCSS();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DlgCSS *ui;
};

#endif // DLGCSS_H

#ifndef PATIENT_H
#define PATIENT_H

#include <QWidget>

namespace Ui {
    class Patient;
}

class Patient : public QWidget {
    Q_OBJECT
public:
    Patient(QWidget *parent = 0);
    ~Patient();

    Ui::Patient *ui;

protected:
    void changeEvent(QEvent *e);

private:

};

#endif // PATIENT_H

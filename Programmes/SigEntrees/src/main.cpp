#include <QtGui/QApplication>
#include "C_Dlg_SigEntrees.h"
#include "CApp.h"

int main(int argc, char *argv[])
{   CApp a(NAME_APPLI, argc, argv);
    if (a.sendMessage("Wake up!")) return 0;
    if (a.pCApp()==0)              return 0;

    C_Dlg_SigEntrees w;
    w.show();
    a.setActivationWindow(&w);
    // QObject::connect(&a, SIGNAL(messageReceived(const QString&)),  &w, SLOT(append(const QString&)));
    QObject::connect(&a, SIGNAL(lastWindowClosed ()),              &w, SLOT(Slot_onQuit()));
    return a.exec();
}

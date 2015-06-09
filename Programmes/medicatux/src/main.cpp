#include <QApplication>
#include <QLibraryInfo>
#include <QTranslator>
#include "C_MW_Prescription.h"
#include "CApp.h"

// %{buildDir}/../bin/Ressources/ComFiles/C_PatientCtx.xml
// la macro NAME_APPLI est definie dans le projet

int main(int argc, char *argv[])
{   CApp a(NAME_APPLI, argc, argv);
    if ( a.instanceMustBeUnique() ) return 0;
    if (a.pCApp()==0) return -1;
    C_MW_Prescription w;
    w.show();
    QObject::connect(&a, SIGNAL(lastWindowClosed ()), &w, SLOT(Slot_onQuit()));
    return a.exec();
}


#include "Dlg_Patch.h"
#include <qtextcodec.h>

int main( int argc, char ** argv ) {
    QApplication a( argc, argv );
    QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
    
    Dlg_Patch * mw = new Dlg_Patch();
    mw->show();
    if (mw->m_AutoTerminate!=2) mw->show();
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    int ret = a.exec();
    delete mw;
    return ret;
}



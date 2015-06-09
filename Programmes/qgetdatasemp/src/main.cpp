
#include <qtextcodec.h>

#include "CApp.h"
#include "C_Dlg_QGetDatasemp.h"

int main( int argc, char ** argv )
{
    CApp* pApp = new CApp( argc, argv );
    QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );

    C_Dlg_QGetDatasemp * mw = new C_Dlg_QGetDatasemp();
    mw->show();
    pApp->connect( pApp, SIGNAL(lastWindowClosed()), pApp, SLOT(quit()) );
    int ret = pApp->exec();
    delete mw;
    delete pApp;
    return ret;
}

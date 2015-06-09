#include <qtextcodec.h>

#include "CApp.h"
#include "C_DlgMainDialog.h"

int main( int argc, char ** argv )
{   CApp* pApp = new CApp( argc, argv );
    QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
    C_DlgMainDialog * mw = new C_DlgMainDialog(0,"dial_set_bases", FALSE, Qt::WStyle_Customize | Qt::WStyle_NormalBorder | Qt::WStyle_Title | Qt::WStyle_SysMenu);
    mw->show();
    pApp->connect( pApp, SIGNAL(lastWindowClosed()), pApp, SLOT(quit()) );
    int ret = pApp->exec();
    delete mw;
    delete pApp;
    return ret;
}

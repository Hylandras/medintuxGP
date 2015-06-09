/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include "../../MedinTuxTools/CGestIni.h"


//----------------------------------------------  init ---------------------------
void Dlg_A_Propos::init()
{m_PathDoc = CGestIni::Construct_PathBin_Module("QLightPad",  QFileInfo(qApp->argv()[0]).dirPath (true)); 
 if (!m_PathDoc.endsWith("/"))   m_PathDoc += "/";
 m_PathDoc            += "QLightPadSys/";
 m_pCCoolPopup         = 0; 
 textBrowserApropos->setSource(m_PathDoc  + "Welcome.html");
 textBrowserGPL->setSource(m_PathDoc  + "Licence_CeCILL_V1-fr.html");  //"GPL.txt");
 showCoolPopup();
}
//----------------------------------------------  setInfoSysteme ---------------------------
void Dlg_A_Propos::setInfoSysteme(const QString & text)
{if (text.length())
    {textBrowser_InfoSysteme->setText(text);
    }
 else
    {tabWidget_DebugInfo->removePage (  tabWidget_DebugInfo->page (2)) ;
    }
}

//----------------------------------------------  showCoolPopup ---------------------------
void Dlg_A_Propos::showCoolPopup()
{if (m_pCCoolPopup) 
    {m_pCCoolPopup->Disappear();
     return;
    }
 m_pCCoolPopup = new CCoolPopup(m_PathDoc  +  "QLightPad.png",  0);
 if (m_pCCoolPopup==0) return;
 connect( m_pCCoolPopup, SIGNAL( Sign_Clicked() ),                  m_pCCoolPopup, SLOT( Disappear() ) );
 connect( m_pCCoolPopup, SIGNAL( Sign_PopupDown(CCoolPopup *) ),    this,          SLOT( CoolPopupStop(CCoolPopup *) ) );
}

//---------------------------------------------- closeEvent --------------------------------------------------------------------------------
void Dlg_A_Propos::closeEvent( QCloseEvent* ce )
{if (m_pCCoolPopup) 
    {m_pCCoolPopup->Disappear(); //CoolPopupStop(m_pCCoolPopup);	
     QTimer::singleShot(3000, this, SLOT(CoolPopupDelete()) ); 
    }
 else     
    {ce->accept(); 
     emit Sign_DlgClosed();                                    
    }
}

//---------------------------------------------------- CoolPopupStop ---------------------------------------------------------------
void Dlg_A_Propos::CoolPopupStop(CCoolPopup * pCCoolPopup)
{if (m_pCCoolPopup==pCCoolPopup)
    {QTimer::singleShot(0, this, SLOT(CoolPopupDelete()) );
    }
}

//--------------------------------------- CoolPopupDelete ------------------------------------------------------------------------
void Dlg_A_Propos::CoolPopupDelete()
{if (m_pCCoolPopup) 
    {m_pCCoolPopup->disconnect( SIGNAL(Sign_Clicked()) ); 
     m_pCCoolPopup->disconnect( SIGNAL(Sign_PopupDown(CCoolPopup *)) );      
     delete m_pCCoolPopup;
    }
 m_pCCoolPopup = 0;   
 close();
}


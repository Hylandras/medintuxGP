/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include "CMDI_ChoixPatient.h"
#include "CMDI_Ident.h"
#include "../../MedinTuxTools/Theme.h"  // Gestion du thème de l'appli

//-------------------------- init ---------------------------------------------------------------------------------------
void Dock_TabRubrique::init()
{
    for ( int i = 0; i < (int) G_pCApp->m_ListRubName.size(); ++i )
        {    QString rubName = G_pCApp->m_ListRubName[i];
             CMyButton*   qw = new CMyButton( this, rubName );
             if (qw)
                {qw->setIconSet ( Theme::getIcon(QString("16x16/type_")+rubName+".png") );
                 #ifdef Q_OS_MACX
				   qw->setFlat(TRUE);
				 #endif
                 m_QPtrButtonList.append(qw);
                 qw->setMaximumSize( QSize( 110, 20 ) );
                 Dock_TabRubriqueLayout->addWidget( qw, i, 0 );
                 //................ Lorsqu'il est cliqué Le bouton emet le signal: Sign_ButtonClicked .........................
                 //                 Nous connectons directement ce signal à celui qu'envoie notre formulaire: Sign_ActiverOrCreateRubrique
                 //                 à celui qui est connecté (DrTux)
                 connect( qw, SIGNAL( Sign_ButtonClicked(const char* ) ), this, SLOT( On_ButtonClicked(const char*) ) );
                }
     }
}
//-------------------------- On_ButtonClicked ---------------------------------------------------------------------------------------
void Dock_TabRubrique::On_ButtonClicked(const char* butTxt )
{emit Sign_ActiverOrCreateRubrique(butTxt , 0);
}

//-------------------------- SetInterfaceOnDroits ---------------------------------------------------------------------------------------
void Dock_TabRubrique::SetInterfaceOnDroits(const QString &droits)
{CMyButton* pCMyButton = 0;
 for ( pCMyButton = m_QPtrButtonList.first(); pCMyButton; pCMyButton = m_QPtrButtonList.next() )
     { if (G_pCApp->IsThisDroitExist( droits, G_pCApp->m_mapNameRubInfos[pCMyButton->getName()]+"v")) pCMyButton->setEnabled(TRUE);
       else                                                                                           pCMyButton->setEnabled(FALSE);
     }
}



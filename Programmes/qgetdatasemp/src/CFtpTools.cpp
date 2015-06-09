/***************
 * CFtpTools.cpp *******************************************************************************
* ***************                                                                              *
* Widget : CFTPTools                                                                           *
* Créé   : Par Roland SEVIN et Sébastien SIVERA                                                *
*                                                                                              *
* Widget qui recupere des fichiers HPRIM d'un répertoire distant et les stocke                 *
* dans un répertoire local                                                                     *
***********************************************************************************************/

#include "CFtpTools.h"
#include <qfile.h>
#include <qmessagebox.h>
#include <qtimer.h>

// ---------------------------- Constructeur et Destructeur ------------------------------------
CFtpTools::CFtpTools(QObject * parent, const char * name /*= 0*/)
    : CFtp( parent , name )
{
    
}

CFtpTools::~CFtpTools()
{
    
}

// -------------------------------------- Connect ----------------------------------------------
void CFtpTools::ConnectFTP(const QString & host,
			   Q_UINT16 port /*= 21*/,
			   const QString & user /*= QString::null*/,
			   const QString & password /*= QString::null*/,
			   const QString & distantfolder /*= QString::null*/)
{
    connectToHost( host , port );
    login( user , password );
    cd( distantfolder );
}

// -------------------------------------- GetListFTP -------------------------------------------
void CFtpTools::GetListFTP()
{
    list();
}

// --------------------------------------- ListAS400 -------------------------------------------
void CFtpTools::ListAS400() 
{
    list();
}

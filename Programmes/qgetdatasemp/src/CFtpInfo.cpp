 /*************
 * CFtpInfo.cpp ********************************************************************************
* *************                                                                                *
* Classe : CFtpInfo                                                                            *
* Créé   : Par Roland SEVIN et Sébastien SIVERA                                                *
*                                                                                              *
* Classe qui stocke les différentes informations sur les fichiers à télécharger                *
* Elle dérivée d'un QUrlInfo, car il contient déjà beaucoup d'informations utiles              *
***********************************************************************************************/

#include "CFtpInfo.h"

// ---------------------------- Constructeur et Destructeur ------------------------------------
CFtpInfo::CFtpInfo()
    : QUrlInfo()
{
    
}

CFtpInfo::CFtpInfo(const QUrlInfo &info)
    : QUrlInfo(info)
{
    
}

CFtpInfo::~CFtpInfo()
{
    
}

// ------------------------------------ setNewName ---------------------------------------------
void CFtpInfo::setNewName( const QString &name )
{
    m_newName = name;
}

// ------------------------------------- newName -----------------------------------------------
QString CFtpInfo::newName() const
{
    return m_newName;
}

// ------------------------------------- setSerial ---------------------------------------------
void CFtpInfo::setSerial(int i)
{
    m_serialNumber = i;
}

// --------------------------------------- serial ----------------------------------------------
int CFtpInfo::serial()
{
    return m_serialNumber;
}

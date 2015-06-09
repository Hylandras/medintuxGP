 /*************
 * CFtpInfo.h **********************************************************************************
* *************                                                                                *
* Classe : CFtpInfo                                                                            *
* Créé   : Par Roland SEVIN et Sébastien SIVERA                                                *
*                                                                                              *
* Classe qui stocke les différentes informations sur les fichiers à télécharger                *
* Elle dérivée d'un QUrlInfo, car il contient déjà beaucoup d'informations utiles              *
***********************************************************************************************/

#ifndef CFTPINFO_H
#define CFTPINFO_H

#include <qurlinfo.h>

class CFtpInfo : public QUrlInfo
{
   
public:
    
    CFtpInfo();
    CFtpInfo(const QUrlInfo &info);
    ~CFtpInfo();
    
    void setNewName( const QString &name );
    QString newName() const;
    void setSerial(int index);
    int serial();
    
protected:
    
    
private:
    
    QString m_newName;
    int     m_serialNumber;
    
};

#endif //CFTPINFO_H

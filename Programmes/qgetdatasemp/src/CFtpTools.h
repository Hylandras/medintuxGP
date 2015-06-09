 /*************
 * CFtpTools.h *********************************************************************************
* *************                                                                                *
* Widget : CFTPTools                                                                           *
* Créé   : Par Roland SEVIN et Sébastien SIVERA                                                *
*                                                                                              *
* Widget qui recupere des fichiers HPRIM d'un répertoire distant et les stocke                 *
* dans un répertoire local                                                                     *
***********************************************************************************************/

#ifndef CFTPTOOLS_H
#define CFTPTOOLS_H

#include <qftp.h>
#include "CFtp.h"
#include <qtimer.h>
#include "CFtpInfo.h"
//..................................... structures pour les commandes ..................................................
//====================================== CommandMap =======================================
#include <qmap.h>

typedef struct _COMMAND
    {QString m_dateDebut;
     QString m_Command;
     QString m_dateFin;
     QString m_Resultat;
     QString m_nomFichier;
    }COMMAND;

typedef QMap < int , COMMAND* > CommandMap;

typedef QValueList < CFtpInfo > QVL_FileInfo;

//====================================== CFtpTools =======================================
class CFtpTools : public CFtp
{
    Q_OBJECT

public:

    CFtpTools(QObject * parent, const char * name = 0);
    ~CFtpTools();

    void ConnectFTP(const QString & host,
                              Q_UINT16 port = 21,
                              const QString & user = QString::null,
                              const QString & password = QString::null,
                              const QString & distantfolder = QString::null);
    void GetListFTP();
    void ListAS400();

protected:

    //void CFtpTools::stateChangedFTP(int state);
    //void CFtpTools::ListFileFTP(const QUrlInfo &objet);
    //void CFtpTools::DownloadFileFTP(const QString &filename);

private:

    QTimer *m_ScanTimer;

};

#endif //CFTPTOOLS_H

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QProcess>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>

#include "../../MedinTuxTools-QT4/CGestIni.h"

#ifdef Q_OS_MACX
#define PATH_SPEC_MAC "../../../"
#define ARCHI_DEP_MAC ".app/Contents/MacOS/" + module
#else
#define PATH_SPEC_MAC ""
#define ARCHI_DEP_MAC ""
#endif
#ifdef Q_WS_WIN
#define F_EXT  ".exe"
#else
#define F_EXT ""
#endif


#define QNETCOPY_USAGE_COMMAND_LINE_ERROR               -11
#define QNETCOPY_SOURCE_FILE_NOT_EXIST_OR_UNREACHABLE   -12
#define QNETCOPY_TARGET_PATH_NOT_EXIST_OR_UNREACHABLE   -13
#define QNETCOPY_FAILED_COPY_FROM_WINDOWS               -14
#define QNETCOPY_FAILED_COPY_FROM_LINUX                 -15
#define QNETCOPY_CANNOT_DELETE_SOURCE_FILE              -16
#define QNETCOPY_SMBCLIENT_NOT_FOUND                    -17 
#define QNETCOPY_FAILED_DELETE_FROM_WINDOWS             -18


void write(const QString &file_ini, const QString &inParam);
QString Construct_PathBin_Module(const QString  &module, const QString & ref_dir);
QString exeBatchFile( QString batchFile, const QString &param /*=""*/ );
int main(int argc, char *argv[])
{
    QCoreApplication l_app(argc, argv);
    QFileInfo qfi(argv[0]);
    QString     pathAppli  = Construct_PathBin_Module("QNetCopy", qfi.path ());
    // get used arguments
    QString l_strFullSourceFilePath;
    QString l_strFullTargetPath;
    bool l_bIsSourceFileDeletedAfterCopy = false;
    if (argc == 3)
    {
        l_strFullSourceFilePath = (QString(argv[1]));
        l_strFullTargetPath = (QString(argv[2]));
    }
    else if (argc == 4)
    {
        l_strFullSourceFilePath = (QString(argv[1]));
        l_strFullTargetPath = (QString(argv[2]));
        l_bIsSourceFileDeletedAfterCopy = (QString(argv[3]).compare("--delete-source-file", Qt::CaseInsensitive) == 0) ? true : false;
    }
    else
    {
        qCritical() << "** Critical Error **\nUsage : QNetCopy <FullSourceFilePath> <FullTargetPath> [option]\n\tOptional : Use \"--delete-source-file\" as option argument if source file has to be removed after copy\n\nPlease don't forget double quotes if path contains space chars !";
        exit(QNETCOPY_USAGE_COMMAND_LINE_ERROR);
    }

    // check full source file path existence
    QFileInfo l_flinfFullSourceFilePath(l_strFullSourceFilePath);
    if (!l_flinfFullSourceFilePath.exists())
    {
        qCritical() << QObject::tr("** Critical Error **\nSpecified full source file path (\"%1\") does not exist or is unreachable.\nPlease verify access to this file !").arg(l_strFullSourceFilePath);
        exit(QNETCOPY_SOURCE_FILE_NOT_EXIST_OR_UNREACHABLE);
    }

#ifdef Q_OS_WIN32
    // check target path existence
    // qDebug()<< "Execution Windows : Commande copy";
    QFileInfo l_flinfFullTargetPath(l_strFullTargetPath);
    if (!l_flinfFullTargetPath.exists())
    {
        qCritical() << QObject::tr("** Critical Error **\nSpecified full target path (\"%1\") does not exist or is unreachable.\nPlease verify access to this path !").arg(l_strFullTargetPath);
        exit(QNETCOPY_TARGET_PATH_NOT_EXIST_OR_UNREACHABLE);
    }

    /* ### WARNING : Following code is not 100% reliable on windows platforms... ###
    // For Windows, simply copy file from source to target
    QString l_strFullTargetFilePath = QObject::tr("%1%2%3").arg(l_flinfFullTargetPath.absolutePath()).arg(QDir::separator()).arg(l_flinfFullSourceFilePath.fileName());
    QFile l_flFullSourceFile(l_flinfFullSourceFilePath.absoluteFilePath());
    if (!l_flFullSourceFile.copy(l_strFullTargetFilePath))
    {
        qCritical() << QObject::tr("** Critical Error **\nCopy (from Windows platform) has failed with following error : %1 !").arg(l_flFullSourceFile.errorString());
        exit(QNETCOPY_FAILED_COPY_FROM_WINDOWS);
    }
    */

    // For Windows, use DOS copy command with source and target arguments
    QProcess    l_procDel;
    QStringList l_lstDelParams;
    l_lstDelParams << QObject::tr("%1%2%3").arg(QDir::toNativeSeparators(l_strFullTargetPath), QDir::separator(), l_flinfFullSourceFilePath.fileName() );
    int l_iDelReturnCode = l_procDel.execute("del", l_lstDelParams);
    QString command =  "del \"" + l_lstDelParams.join(" ") + "\"";
    write(pathAppli + "delete.bat", command);
    QString ret = exeBatchFile( pathAppli + "delete.bat", command);
    write(pathAppli + "delete.log", command + "\n" + ret);

    if (l_iDelReturnCode != 0)
    {
        qCritical() << QObject::tr("** Critical Error **\nDel (from Windows platform) has failed with error code %1 !").arg(l_iDelReturnCode);
        exit(QNETCOPY_FAILED_DELETE_FROM_WINDOWS);
    }

    QProcess    l_procCopy;
    QStringList l_lstCopyParams = QStringList() << "/Y" << QObject::tr("%1%2%3").arg(QDir::toNativeSeparators(l_flinfFullSourceFilePath.absolutePath())).arg(QDir::separator()).arg(l_flinfFullSourceFilePath.fileName()) << QDir::toNativeSeparators(l_strFullTargetPath);
    int l_iCopyReturnCode = l_procCopy.execute("xcopy", l_lstCopyParams);
    write(pathAppli + "xcopy.txt", "xcopy \"" + l_lstCopyParams.join(" ") + "\"");
    if (l_iCopyReturnCode != 0)
    {
        qCritical() << QObject::tr("** Critical Error **\nCopy (from Windows platform) has failed with error code %1 !").arg(l_iCopyReturnCode);
        exit(QNETCOPY_FAILED_COPY_FROM_WINDOWS);
    }
#endif
#ifdef  Q_OS_MAC
    QString pathExe = "";
    if (QFile::exists("/opt/local/bin/smbclient"))      pathExe = "/opt/local/bin/smbclient";   // SmbUp (lion)
    else if (QFile::exists("/usr/bin/smbclient"))       pathExe = "/usr/bin/smbclient";         // leopard
    else {
           qWarning() << "** Warning Error **\nsmbclient not found !";
           exit( QNETCOPY_SMBCLIENT_NOT_FOUND );
         }
    // For mac, Osx lion simply invoke smbclient with right parameters
	// remove source file if specified
    QProcess l_procSmbClient;
    QStringList l_lstParams;
    l_lstParams << l_strFullTargetPath << "-N" << "-d0" << QObject::tr("-c put \"%1\" \"%2\"").arg(l_flinfFullSourceFilePath.absoluteFilePath()).arg(l_flinfFullSourceFilePath.fileName());
    if (l_procSmbClient.execute(pathExe, l_lstParams) != 0)
    {
        qCritical() << QObject::tr("** Critical Error **\nCopy (from mac platform) has failed !");
        exit(QNETCOPY_FAILED_COPY_FROM_LINUX);
    }
#endif
#ifdef Q_OS_LINUX
    // For Linux, simply invoke smbclient with right parameters
    QProcess l_procSmbClient;
    QStringList l_lstSmbClientParams;
    l_lstSmbClientParams << l_strFullTargetPath << "-N" << "-d0" << QObject::tr("-c put \"%1\" \"%2\"").arg(l_flinfFullSourceFilePath.absoluteFilePath()).arg(l_flinfFullSourceFilePath.fileName());
    int l_iSmbClientReturnCode = l_procSmbClient.execute("smbclient", l_lstSmbClientParams);
    if (l_iSmbClientReturnCode != 0)
    {
        qCritical() << QObject::tr("** Critical Error **\nCopy (from Linux platform) has failed with error code %1 !").arg(l_iSmbClientReturnCode);
        exit(QNETCOPY_FAILED_COPY_FROM_LINUX);
    }
#endif

    // remove source file if specified
    if (l_bIsSourceFileDeletedAfterCopy)
    {
        if (!QFile::remove(l_flinfFullSourceFilePath.absoluteFilePath()))
        {
            qWarning() << "** Warning Error **\nSource file cannot be deleted !";
            exit(QNETCOPY_CANNOT_DELETE_SOURCE_FILE);
        }
    }

    // everything is ok !
    return 0;
}
/*! \brief Permet la construction d'un nom complet de fichier executable en  relation avec le module correspondant
 *  \param QString  module : nom du programme dont on cherche a�¿½  construire le chemin complet si egal a�¿½  "" alors le nom du chemin sera extrait de : start_Argv
 *  \param const QString & start_Argv : Chemin complet de demarrage du programme
 *  \param QString *base_name : adresse d'une QString qui dans laquelle si elle est differente de zero sera retourne le nom du programme SANS SON EXTENSION
 *  \return nom du module  avec le chemin complet de demarrage du proc --> nom  ini complet du module
*/
QString Construct_PathBin_Module(const QString  &module, const QString & ref_dir)
{ return  QDir::cleanPath (ref_dir + "/" + PATH_SPEC_MAC + "../../" + module + "/bin/")+ "/" ;
}
void write(const QString &file_ini, const QString &inParam)
{
 QFile file( file_ini);
 if ( !file.open( QIODevice::WriteOnly ) )    return;
 QTextStream ts( &file );
 ts << inParam;
 file.close();
}

//--------------------------------- exeBatchFile -----------------------------------------------------------------------
QString exeBatchFile( QString batchFile, const QString &param /*=""*/ )
{     QString ret = "";
      //Check if cmd.exe is present in Clients system and is located at correct path
      QString path = "";
      if      (QFile::exists("C:\\Windows\\SysWOW64\\cmd.exe"))  path = "C:\\Windows\\SysWOW64\\cmd.exe";
      else if (QFile::exists("C:\\Windows\\system32\\cmd.exe"))  path = "C:\\Windows\\system32\\cmd.exe";
      QFileInfo cmdFile( path );
      if( !cmdFile.exists() )
        { qCritical() <<   "C_MainDialog::exeBatchFile() Failed to find the cmd.exe";
          return ret;
        }

     batchFile = batchFile.replace("/","\\");
     QStringList arguments ;
     arguments << " /c" << batchFile;
     if (param.length()) arguments << param;
     QProcess *process = new QProcess( 0 );
     process->start( cmdFile.absoluteFilePath(), arguments ) ;
     if( !process->waitForStarted(-1) )
       { qCritical() <<  "C_MainDialog::exeBatchFile() Failed to start the process.";
         process->deleteLater();
         // delete process;
         return ret;
       }
     // process->waitForFinished(-1) ;
     qApp->processEvents ();
     while (process->state()== QProcess::Running )
           { qApp->processEvents ( QEventLoop::ExcludeUserInputEvents );
           }
     ret =  process->readAllStandardOutput()   +
            process->readAll()                 +
            process->readAllStandardError();
     // delete process;
     process->deleteLater();
     return ret;
  }

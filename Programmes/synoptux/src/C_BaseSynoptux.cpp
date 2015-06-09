#include "C_BaseSynoptux.h"
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QTreeWidget>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QLabel>
#include <QTreeWidgetItemIterator>
#include <QProgressBar>
#include <QApplication>
#include <QDate>
#include <QUrl>

#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/C_Utils_Log.h"

#include "C_DBVar.h"

#define MAX_READ        32000
#define OFSET           ".&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
//--------------------------- C_BaseSynoptux -----------------------------------------------------------
C_BaseSynoptux::C_BaseSynoptux( const QString &param , QString& confData, QObject *parent) :
    C_BaseCommon(parent), C_DBVar( confData)

{if (!C_DBVar::areFieldsNamesValids())                   C_BaseSynoptux::setLastError(C_DBVar::getLastError());
 if (!BaseConnect(param,"BaseSynoptux","DrTuxTest"))     C_BaseSynoptux::setLastError(C_BaseCommon::getLastError());
}
//--------------------------- setLastError -----------------------------------------------------------
void       C_BaseSynoptux::setLastError(const QString &error){ m_LastError = error;}
//--------------------------- getLastError -----------------------------------------------------------
QString    C_BaseSynoptux::getLastError(){return m_LastError;}



#ifndef C_BASESYNOPTUX_H
#define C_BASESYNOPTUX_H

#include "../../MedinTuxTools-QT4/C_BaseCommon.h"
#include "C_DBVar.h"
#include <QObject>

//====================================== C_BaseSynoptux =====================================
class C_BaseSynoptux : public C_BaseCommon, public C_DBVar
{
    Q_OBJECT

public:
    explicit    C_BaseSynoptux(const QString &param , QString& confData, QObject *parent );
    QString     getLastError();

private:
    void         setLastError(const QString &error);

signals:

public slots:

private:
    QString      m_LastError;
};

#endif // C_BASESYNOPTUX_H

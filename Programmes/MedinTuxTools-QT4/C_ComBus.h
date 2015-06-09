#ifndef C_COMBUS_H
#define C_COMBUS_H

#include <QObject>
//=================================== C_ComBus =====================================================
/*! \class C_ComBus
 *  \brief Class to set a inter process bus communication.
 */

class C_ComBus : public QObject
{
    Q_OBJECT
public:
    explicit C_ComBus(QObject *parent = 0);

signals:

public slots:

};

#endif // C_COMBUS_H

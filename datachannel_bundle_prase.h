#ifndef DATACHANNELBUNDLEPRASE_H
#define DATACHANNELBUNDLEPRASE_H

#include <QDebug>
#include <QString>
#include "SignalServerModule/SignalCallBack.h"
#include "DisplayModule/send_event_callback.h"
class DataChannelBundlePrase
{
public:
    DataChannelBundlePrase();
    void bundleRequest(SingalingData,QString& ret);
    void bundleDataChannel(Event event, QString roomid,QString uid , QString& ret);
    void parseResponse(const QByteArray& data, SingalingData &);
};

#endif // DATACHANNELBUNDLEPRASE_H

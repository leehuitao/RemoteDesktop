#ifndef SIGNALCALLBACK_H
#define SIGNALCALLBACK_H
#include <QString>
#include <QImage>
#include <QMap>
#include <QWebSocket>
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QTimer>

#define JOIN        "room_join"
#define READY       "peer_ready"
#define OFFER       "offer"
#define ANSWER      "answer"
#define CONDIDATA   "candidate"
#define QUIT        "room_quit"
#define SENDMSG     "send_msg"
#define HEART       "heart"
typedef struct {
    QString stataus;
    QString msg;
    QString type;
    QString roomid;
    QString uid;
    QMap<QString,QString> data;
}SingalingData;

class SignalCallBack{
public:
    SignalCallBack(){}
    virtual ~SignalCallBack(){}
    virtual void OnReceiveJoin(SingalingData& data) = 0;
    virtual void OnReceiveReady(SingalingData& data)= 0;
    virtual void OnReceiveOffer(SingalingData& data) = 0;
    virtual void OnReceiveAnswer(SingalingData& data)= 0;
    virtual void OnReceiveCanidate(SingalingData& data)= 0;
};

class SignalTextCallBack{
public:
    SignalTextCallBack(){}
    virtual ~SignalTextCallBack(){}
    virtual void OnConnected() = 0;
    virtual void OnError(QAbstractSocket::SocketError) = 0;
    virtual void OnRecvData(const QString &) = 0;
};

#endif // SIGNALCALLBACK_H

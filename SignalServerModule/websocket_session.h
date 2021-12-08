#ifndef WEBSOCKETSESSION_H
#define WEBSOCKETSESSION_H


#include "SignalCallBack.h"

class WebsocketSession:public QObject
{
    Q_OBJECT
public:
    WebsocketSession();
    void sendData(QString data );
    void setCallbak(SignalTextCallBack*);
    void setWebSocketDefault(QString ip,unsigned short port,int uid);

public Q_SLOTS:
    void recvData(const QString& data);
    void connected();
    void error(QAbstractSocket::SocketError);
    void close();

private:
    QString url_;
    SignalTextCallBack* callback_;
    QWebSocket webclient;
};

#endif // WEBSOCKETSESSION_H

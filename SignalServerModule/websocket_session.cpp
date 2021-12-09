#include "websocket_session.h"

WebsocketSession::WebsocketSession()
{
    callback_ = nullptr;
}

void WebsocketSession::sendData(QString data)
{
    webclient.sendTextMessage(data);
}

void WebsocketSession::setCallbak(SignalTextCallBack * callback)
{
    callback_ = callback;
}

void WebsocketSession::setWebSocketDefault(QString ip, unsigned short port, int uid)
{
    url_ = QString("wss://%1:%2/ws?uid=%3").arg(ip).arg(port).arg(uid);
    connect(&webclient, &QWebSocket::textMessageReceived, this, &WebsocketSession::recvData);
    connect(&webclient, SIGNAL(connected()), this, SLOT(connected()));
    connect(&webclient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    QSslConfiguration config = webclient.sslConfiguration();
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1SslV3);
    webclient.setSslConfiguration(config);
    webclient.open(QUrl(url_));
}

void WebsocketSession::recvData(const QString &data)
{
    if(callback_)
        callback_->OnRecvData(data);
}

void WebsocketSession::connected()
{
    if(callback_)
        callback_->OnConnected();
}

void WebsocketSession::error(QAbstractSocket::SocketError error)
{
    qDebug()<<error;
    if(callback_)
        callback_->OnError(error);
}

void WebsocketSession::close()
{
    webclient.close();
}



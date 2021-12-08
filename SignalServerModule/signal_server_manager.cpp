#include "signal_server_manager.h"
#include "LogModule/romote_log.h"
SignalServerManager * SignalServerManager::signalServerManager = new SignalServerManager;

bool SignalServerManager::initWebSocket(QString ip, unsigned short port, int uid)
{
    websocketClient_ = new WebsocketSession;
    websocketClient_->setWebSocketDefault(ip,port,uid);
    websocketClient_->setCallbak(this);
    return isConnect_;
}

void SignalServerManager::OnConnected()
{
    QLOG_INFO() << "OnConnected";
    signWebsocketConnected();
}

void SignalServerManager::OnError(QAbstractSocket::SocketError error)
{
    QLOG_INFO() << "SocketError "<<error;
}

void SignalServerManager::OnRecvData(const QString & data)
{
    SingalingData data_;
    QLOG_INFO()<<"OnRecvData = "<<data;
    parse.parseJson(data , data_);
    callback(data_);
}

void SignalServerManager::joinRoom(int roomid, int uid)
{
    SingalingData data;
    data.type = "room_join";
    data.roomid = QString::number(roomid);
    data.uid = QString::number(uid);
    uid_ = uid;
    sendData(data);
}

void SignalServerManager::sendData(SingalingData &data)
{
    QString data_;
    data.uid = QString::number(uid_);
    bundle.bundleRequest(data,data_);
    websocketClient_->sendData(data_);
}

void SignalServerManager::callback(const SingalingData & data)
{
    if(data.type == JOIN){
        signReceiveJoin(data);
    }else if(data.type == READY){
        signReceiveReady(data);
    }else if(data.type == OFFER){
        signReceiveOffer(data);
    }else if(data.type == ANSWER){
        signReceiveAnswer(data);
    }else if(data.type == CONDIDATA){
        signReceiveCanidate(data);
    }else{
        QLOG_INFO() << "error type = " << data.type;
    }
}

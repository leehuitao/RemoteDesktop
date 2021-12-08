#ifndef SIGNALSERVERMANAGER_H
#define SIGNALSERVERMANAGER_H
#include "SignalCallBack.h"
#include "parse.h"
#include "websocket_session.h"
#include "bundle.h"
class SignalServerManager :public QObject,public SignalTextCallBack
{
    Q_OBJECT
public:
    static SignalServerManager* Instance(){
        return signalServerManager;
    }
    //初始化websocket
    bool initWebSocket(QString ip,unsigned short port,int uid);
    virtual void OnConnected() override;
    virtual void OnError(QAbstractSocket::SocketError) override;
    virtual void OnRecvData(const QString &) override;

    void joinRoom(int roomid,int uid);

    void sendData(SingalingData & data);

    bool isConnect(){return isConnect_;}
Q_SIGNALS:
    void signWebsocketConnected();
    void signReceiveJoin(SingalingData);
    void signReceiveReady(SingalingData);
    void signReceiveOffer(SingalingData);
    void signReceiveAnswer(SingalingData);
    void signReceiveCanidate(SingalingData);
private:
    void callback(const SingalingData&);
    SignalServerManager() = default;
private:
    Parse parse;
    Bundle bundle;
    static SignalServerManager * signalServerManager;
    WebsocketSession * websocketClient_;
    bool isConnect_ = false;
    int uid_;
};

#endif // SIGNALSERVERMANAGER_H

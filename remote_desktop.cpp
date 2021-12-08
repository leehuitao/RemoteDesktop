#include "remote_desktop.h"
#include "data_control.h"
using namespace remote;
using namespace QsLogging;

RemoteDesktop *RemoteDesktop::remoteDesktop_ = new RemoteDesktop;

bool remote::RemoteDesktop::init(remote::DataCallBack *callback)
{
    callback_ = callback;
    peerConnection = PeerconnectionManager::Create();
    initDesktopCapture();
    initScreentopCapture();
    qRegisterMetaType<SingalingData>("SingalingData");
    //connect(DataControl::Instance(),&DataControl::signRemoteImage,this,&RemoteDesktop::slotRemoteImage);
    connect(DataControl::Instance(),&DataControl::signDesktopLocalImage,this,&RemoteDesktop::slotLocalDesktopImage);
    connect(DataControl::Instance(),&DataControl::signScreenLocalImage,this,&RemoteDesktop::slotScreenImage);
    connect(DataControl::Instance(),&DataControl::signDesktopChooseLocalImage,this,&RemoteDesktop::slotChooseDesktopImage);
    connect(DataControl::Instance(),&DataControl::signScreenChooseLocalImage,this,&RemoteDesktop::slotChoseScreenImage);
    connect(DataControl::Instance(),&DataControl::signOnDataChannel,this,&RemoteDesktop::slotOnDataChannel);
    return 1;
}

void RemoteDesktop::setRemoteParent(QWidget *parent)
{
    remoteDesktopDisplay = new RemoteDesktopDisplay(parent);//显示到外部调用的UI
    remoteDesktopDisplay->setCallback(peerConnection);//让UI能调用到  datachannel
}

void RemoteDesktop::setDisplayDatachannel(RemoteDesktopDisplay *disp)
{
    disp->setCallback(peerConnection);//让UI能调用到  datachannel
}

void RemoteDesktop::startRemoteDesktop()
{

}

void RemoteDesktop::AddLocalTrack(bool isDesktopCapture, const int loaclTrackSourceid)
{
    peerConnection->setLoaclTrackDefault(isDesktopCapture,loaclTrackSourceid);
}

void RemoteDesktop::joinRoom(int roomid, int uid)
{
    SignalServerManager::Instance()->joinRoom(roomid,uid);
    peerConnection->setLoaclId(uid);
}

void RemoteDesktop::setLoaclShared(const QString &sourceid, const QString &sourcetitle)
{
    peerConnection->startCapture();
}

void RemoteDesktop::captureOneImage()
{
    peerConnection->captureOneImage();
}

void RemoteDesktop::sendDataChannel(SingalingData data)
{
    peerConnection->sendDataChannel(data);
}

void RemoteDesktop::getScreenSourceList(QMap<int, QString> &SourceMap)
{
    peerConnection->getScreenSourceList(SourceMap);
}

remote::RemoteDesktop::~RemoteDesktop()
{

}

bool RemoteDesktop::initPeerConnection(const QString &stun, bool dtls)
{
    peerConnection->initPeerConnection(dtls,stun);
    return 1;
}

bool RemoteDesktop::initDesktopCapture()
{
    peerConnection->initDesktopCapture();
    return 1;
}

bool RemoteDesktop::initScreentopCapture()
{
    peerConnection->initScreenCapture();
    return 1;
}

bool RemoteDesktop::initLog(QsLogging::Level level, const QString &logPath, long long maxSizeBytes, int maxOldLogCount)
{
    return RomoteLog::Instance()->initLog(level,logPath,maxSizeBytes,maxOldLogCount);
}

void RemoteDesktop::clearAll()
{
    if(peerConnection != nullptr)
        peerConnection = nullptr;
    peerConnection = PeerconnectionManager::Create();
    initDesktopCapture();
    initScreentopCapture();
}

bool RemoteDesktop::initWebSocket(QString ip, unsigned short port, int uid)
{
    return SignalServerManager::Instance()->initWebSocket(ip,port,uid);
}

void RemoteDesktop::slotRemoteImage(QImage image)
{

}

void RemoteDesktop::slotLocalDesktopImage(QImage image)
{
    signLocalDesktopImage(image);
    if(callback_!= nullptr)
        callback_->OnLocalDesktopImage(image);
}

void RemoteDesktop::slotScreenImage(QImage image)
{
    signScreenImage(image);
    if(callback_!= nullptr)
        callback_->OnLocalScreenImage(image);
}

void RemoteDesktop::slotChooseDesktopImage(int sourceid, QString title, QImage image)
{
    signChooseDesktopImage(sourceid,title,image);
    if(callback_!= nullptr)
        callback_->OnLocalChooseDesktopImage(sourceid,title,image);
}

void RemoteDesktop::slotChoseScreenImage(int sourceid, QString title, QImage image)
{
    signChoseScreenImage(sourceid,title,image);
    if(callback_!= nullptr)
        callback_->OnLocalChoseScreenImage(sourceid,title,image);
}

void RemoteDesktop::slotOnDataChannel(SingalingData data)
{
    signOnDataChannel(data);
    if(callback_!= nullptr)
        callback_->OnDataChannel(data);
}

void RemoteDesktop::slotOnIceConnected()
{
    peerConnection->startCapture();
    signOnIceConnected();
}

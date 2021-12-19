#include "interface.h"
using namespace QsLogging;
Interface *Interface::interface_ = new Interface;

bool Interface::init(/*remote::DataCallBack *callback*/)
{
    peerConnection = PeerconnectionManager::Create();
    initDesktopCapture();
    initScreentopCapture();
    qRegisterMetaType<SingalingData>("SingalingData");
    //connect(DataControl::Instance(),&DataControl::signRemoteImage,this,&Interface::slotRemoteImage);
    connect(DataControl::Instance(),&DataControl::signDesktopLocalImage,this,&Interface::slotLocalDesktopImage);
    connect(DataControl::Instance(),&DataControl::signScreenLocalImage,this,&Interface::slotScreenImage);
    connect(DataControl::Instance(),&DataControl::signDesktopChooseLocalImage,this,&Interface::slotChooseDesktopImage);
    connect(DataControl::Instance(),&DataControl::signScreenChooseLocalImage,this,&Interface::slotChoseScreenImage);
    connect(DataControl::Instance(),&DataControl::signOnDataChannel,this,&Interface::slotOnDataChannel);
    connect(peerConnection,&PeerconnectionManager::signICEConnected,this,&Interface::slotOnIceConnected);

    return 1;
}

void Interface::setRemoteParent(QWidget *parent)
{
//    InterfaceDisplay = new InterfaceDisplay(parent);//显示到外部调用的UI
    //    InterfaceDisplay->setCallback(peerConnection);//让UI能调用到  datachannel
}

void Interface::setDisplayDatachannel(RemoteDesktopDisplay *disp)
{
    disp->setCallback(peerConnection);//让UI能调用到  datachannel
}

void Interface::startRemoteDesktop()
{

}

void Interface::AddLocalTrack(bool isDesktopCapture, const int loaclTrackSourceid)
{
    peerConnection->setLoaclTrackDefault(isDesktopCapture,loaclTrackSourceid);
}

void Interface::joinRoom(int roomid, int uid)
{
    SignalServerManager::Instance()->joinRoom(roomid,uid);
    peerConnection->setLoaclId(uid);
}

void Interface::setLoaclShared(const QString &sourceid, const QString &sourcetitle)
{
    peerConnection->startCapture();
}

void Interface::captureOneImage()
{
    peerConnection->captureOneImage();
}

void Interface::sendDataChannel(SingalingData data)
{
    peerConnection->sendDataChannel(data);
}

void Interface::sendDataChannel(const QString &msg)
{
    peerConnection->sendDataChannel(msg);
}

void Interface::getScreenSourceList(QMap<int, QString> &SourceMap)
{
    peerConnection->getScreenSourceList(SourceMap);
}

Interface::~Interface()
{

}

bool Interface::initPeerConnection(const std::string& turnip ,const std::string& turnport,const std::string& turnusername ,const std::string& turnpwd, bool dtls)
{
    peerConnection->initPeerConnection(dtls,turnip,turnport,turnusername,turnpwd);
    return 1;
}

bool Interface::initDesktopCapture()
{
    peerConnection->initDesktopCapture();
    return 1;
}

bool Interface::initScreentopCapture()
{
    peerConnection->initScreenCapture();
    return 1;
}

bool Interface::initLog(QsLogging::Level level, const QString &logPath, long long maxSizeBytes, int maxOldLogCount)
{
    return RomoteLog::Instance()->initLog(level,logPath,maxSizeBytes,maxOldLogCount);
}

void Interface::clearAll()
{
    if(peerConnection != nullptr)
        peerConnection = nullptr;
    peerConnection = PeerconnectionManager::Create();
    initDesktopCapture();
    initScreentopCapture();
}

bool Interface::initWebSocket(QString ip, unsigned short port, int uid)
{
    return SignalServerManager::Instance()->initWebSocket(ip,port,uid);
}

void Interface::slotRemoteImage(QImage image)
{

}

void Interface::slotLocalDesktopImage(QImage image)
{
    signLocalDesktopImage(image);
    if(callback_!= nullptr)
        callback_->OnLocalDesktopImage(image);
}

void Interface::slotScreenImage(QImage image)
{
    signScreenImage(image);
    if(callback_!= nullptr)
        callback_->OnLocalScreenImage(image);
}

void Interface::slotChooseDesktopImage(int sourceid, QString title, QImage image)
{
    signChooseDesktopImage(sourceid,title,image);
    if(callback_!= nullptr)
        callback_->OnLocalChooseDesktopImage(sourceid,title,image);
}

void Interface::slotChoseScreenImage(int sourceid, QString title, QImage image)
{
    signChoseScreenImage(sourceid,title,image);
    if(callback_!= nullptr)
        callback_->OnLocalChoseScreenImage(sourceid,title,image);
}

void Interface::slotOnDataChannel(SingalingData data)
{
    signOnDataChannel(data);
    if(callback_!= nullptr)
        callback_->OnDataChannel(data);
}

void Interface::slotOnIceConnected()
{
    peerConnection->startCapture();
    signOnIceConnected();
}

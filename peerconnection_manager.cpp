#include "peerconnection_manager.h"

const char kVideoLabel[] = "video_label";
PeerconnectionManager::PeerconnectionManager()
{
    connect(SignalServerManager::Instance(),&SignalServerManager::signReceiveJoin,this,&PeerconnectionManager::slotReceiveJoin);
    connect(SignalServerManager::Instance(),&SignalServerManager::signReceiveReady,this,&PeerconnectionManager::slotReceiveReady);
    connect(SignalServerManager::Instance(),&SignalServerManager::signReceiveOffer,this,&PeerconnectionManager::slotReceiveOffer);
    connect(SignalServerManager::Instance(),&SignalServerManager::signReceiveAnswer,this,&PeerconnectionManager::slotReceiveAnswer);
    connect(SignalServerManager::Instance(),&SignalServerManager::signReceiveCanidate,this,&PeerconnectionManager::slotReceiveCanidate);
    time = new QTimer(this);
    connect(time,&QTimer::timeout,this,&PeerconnectionManager::timeout);
    heart.type = HEART;
}

void PeerconnectionManager::createOffer()
{
    if (connection_.get())
        connection_->CreateOffer(this, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
}

void PeerconnectionManager::createAnswer()
{
    if (connection_.get())
        connection_->CreateAnswer(this, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
}

rtc::scoped_refptr<PeerconnectionManager> PeerconnectionManager::Create()
{
    rtc::scoped_refptr<PeerconnectionManager> cpt = new rtc::RefCountedObject<PeerconnectionManager>();
    return cpt;
}

void PeerconnectionManager::Destroy()
{

}

void PeerconnectionManager::startCapture()
{
        if(isDesktopCapture_)
            deskCapture_->startCaptrue(loaclTrackSourceid_);
        else
            screenCapture_->startCaptrue(loaclTrackSourceid_);
}

void PeerconnectionManager::captureOneImage()
{
    if(isDesktopCapture_)
        deskCapture_->captureOneFrame();
    else
        screenCapture_->captureOneFrame();
}

void PeerconnectionManager::getScreenSourceList(QMap<int, QString> &SourceMap)
{
    if(isDesktopCapture_){}
        //deskCapture_->getScreenSourceList(SourceMap);
    else
        screenCapture_->getScreenSourceList(SourceMap);
}

void PeerconnectionManager::OnSignalingChange(PeerConnectionInterface::SignalingState new_state)
{
    QLOG_INFO()<<"OnSignalingChange new_state = "<<new_state;
}

void PeerconnectionManager::OnAddTrack(rtc::scoped_refptr<RtpReceiverInterface> receiver, const std::vector<rtc::scoped_refptr<MediaStreamInterface> > &streams)
{
    if(receiver->track().release()->kind() == webrtc::MediaStreamTrackInterface::kVideoKind){
        auto* video_track = static_cast<webrtc::VideoTrackInterface*>(receiver->track().release());
        video_track->AddOrUpdateSink(DataControl::Instance(),rtc::VideoSinkWants());
    }
}

void PeerconnectionManager::OnRemoveTrack(rtc::scoped_refptr<RtpReceiverInterface> receiver)
{
    //QLOG_INFO << __FUNCTION__<<receiver;
}

void PeerconnectionManager::OnDataChannel(rtc::scoped_refptr<DataChannelInterface> channel)
{
    QLOG_INFO()<<"OnDataChannel";
    channel->RegisterObserver(DataControl::Instance());
}

void PeerconnectionManager::OnRenegotiationNeeded()
{
    QLOG_INFO ()<< __FUNCTION__;
}

void PeerconnectionManager::OnIceConnectionChange(PeerConnectionInterface::IceConnectionState new_state)
{

    if( new_state = webrtc::PeerConnectionInterface::kIceConnectionConnected){
        QLOG_INFO ()<< __FUNCTION__<<new_state;
        if(isAccusedEnd_)
            signICEConnected();
    }
}

void PeerconnectionManager::OnIceGatheringChange(PeerConnectionInterface::IceGatheringState new_state)
{
    QLOG_INFO () << __FUNCTION__<<new_state;
}

void PeerconnectionManager::OnIceCandidate(const IceCandidateInterface *candidate)
{
    QLOG_INFO()<<"--------------------------------------------OnIceCandidate";
    std::string sdp;
    if (!candidate->ToString(&sdp)) {
        return;
    }
    std::string mid = candidate->sdp_mid();
    int index = candidate->sdp_mline_index();
    SingalingData data;
    data.data["candidate"] = QString::fromStdString(sdp);
    data.data["sdpMLineIndex"] = QString::number(index);
    data.data["sdpMid"] = QString::fromStdString(mid);
    data.type = "candidate";
    data.uid = uid_;
    data.roomid = roomid_;
    SignalServerManager::Instance()->sendData(data);
}

void PeerconnectionManager::OnIceCandidateError(const std::string &host_candidate, const std::string &url, int error_code, const std::string &error_text)
{
    RTC_LOG(LERROR) << __FUNCTION__<<host_candidate<<url<<error_code<<error_text;
}

void PeerconnectionManager::OnIceConnectionReceivingChange(bool receiving)
{
    //RTC_LOG(LERROR) << __FUNCTION__<<receiving;
}

void PeerconnectionManager::OnSuccess(SessionDescriptionInterface *desc)
{
    QLOG_INFO()<<__FUNCTION__;
    std::string sdp;
    desc->ToString(&sdp);
    SingalingData data;
    std::string type_str = webrtc::SdpTypeToString(desc->GetType());
    data.data["sdp"] = QString::fromStdString(sdp);
    data.data["type"] = QString::fromStdString(desc->type());
    data.type = QString::fromStdString(type_str);
    data.uid = uid_;
    data.roomid = roomid_;

    if (type_str == "offer") {
        offerdesc = desc;
        SignalServerManager::Instance()->sendData(data);
    }
    if (type_str == "answer") {
        connection_->SetLocalDescription(DummySetSessionDescriptionObserver::Create(),desc);
        SignalServerManager::Instance()->sendData(data);
    }
}

void PeerconnectionManager::OnFailure(RTCError error)
{
    //RTC_LOG(LERROR) << __FUNCTION__<<error;
}

void PeerconnectionManager::sendDataChannel(SingalingData data)
{
    if(pDataChannel == nullptr)
        return;
    QString req;
    bundle.bundleRequest(data,req);
    std::string str = std::string((const char *)req.toLocal8Bit());
    pDataChannel->Send(webrtc::DataBuffer(str));
}

void PeerconnectionManager::sendDataChannel(const QString &msg)
{
    MsgDataBuffer buffer;
    buffer.header.cmd = 1;
    buffer.body = msg.toUtf8().data();
    rtc::CopyOnWriteBuffer writeBuffer;
    writeBuffer.SetData<char>(buffer.toByte().data(),buffer.toByte().size());
    pDataChannel->Send(webrtc::DataBuffer(writeBuffer,1));
}

void PeerconnectionManager::setLoaclTrackDefault(bool isDesktopCapture, const int loaclTrackSourceid)
{
    isDesktopCapture_ = isDesktopCapture;
    loaclTrackSourceid_ = loaclTrackSourceid;
    AddLocalTrack(isDesktopCapture_,loaclTrackSourceid_);
}

void PeerconnectionManager::SendMouseEvent(Event event)
{
    if(pDataChannel == nullptr)
        return;
    QString req;
    bundle.bundleDataChannel(event,roomid_,uid_,req);
    std::string str = std::string((const char *)req.toLocal8Bit());
    pDataChannel->Send(webrtc::DataBuffer(str));
}

void PeerconnectionManager::SendKeywordEvent(Event event)
{
    if(pDataChannel == nullptr)
        return;
//    QString req;
//    bundle.bundleDataChannel(event,roomid_,uid_,req);
//    std::string str = std::string((const char *)req.toLocal8Bit());
//    pDataChannel->Send(webrtc::DataBuffer(str));
    sendDataChannel("123");
}

void PeerconnectionManager::slotReceiveJoin(SingalingData data)
{
    roomid_ = data.roomid;
    createOffer();
    //开启心跳检测
    time->start(20*1000);
}

void PeerconnectionManager::slotReceiveReady(SingalingData data)
{
    QLOG_INFO()<<__FUNCTION__<<data.type;
}

void PeerconnectionManager::slotReceiveOffer(SingalingData data)
{
    if(data.uid == uid_)
        return;
    std::string type_str = "offer";
    webrtc::SdpParseError error;
    std::string sdp = std::string(data.data["sdp"].toLocal8Bit().data());
    std::unique_ptr<webrtc::SessionDescriptionInterface> session_description = webrtc::CreateSessionDescription(webrtc::SdpType::kOffer, sdp, &error);
    QLOG_INFO() << " Received offer sdp:" << data.data;

    connection_->SetRemoteDescription(DummySetSessionDescriptionObserver::Create(),session_description.release());
    connection_->CreateAnswer(this, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
}

void PeerconnectionManager::slotReceiveAnswer(SingalingData data)
{
    if(data.uid == uid_)
        return;
    std::string type_str = "answer";
    webrtc::SdpParseError error;
    std::string sdp = std::string(data.data["sdp"].toLocal8Bit().data());
    std::unique_ptr<webrtc::SessionDescriptionInterface> session_description = webrtc::CreateSessionDescription(webrtc::SdpType::kAnswer, sdp, &error);
    connection_->SetLocalDescription(DummySetSessionDescriptionObserver::Create(),offerdesc);
    connection_->SetRemoteDescription(DummySetSessionDescriptionObserver::Create(),session_description.release());
    QLOG_INFO() << "data.uid"<<data.uid<<"data.type"<<data.type<<" Received answer sdp:" <<  data.data;
}

void PeerconnectionManager::slotReceiveCanidate(SingalingData data)
{
    webrtc::SdpParseError error;
    std::string sdp_mid = std::string(data.data["sdpMid"].toLocal8Bit().data());
    int sdp_m_index = data.data["sdpMLineIndex"].toInt();
    std::string sdp = std::string(data.data["candidate"].toLocal8Bit().data());
    QLOG_INFO()<< " CreateIceCandidate" ;
    std::unique_ptr<webrtc::IceCandidateInterface> candidate(
                webrtc::CreateIceCandidate(sdp_mid, sdp_m_index, sdp, &error)
                );
    if (!candidate.get()) {
        return;
    }
    if (!connection_->AddIceCandidate(candidate.get())) {
        return;
    }
}

void PeerconnectionManager::timeout()
{
    SignalServerManager::Instance()->sendData(heart);
}

void PeerconnectionManager::initDesktopCapture()
{
    deskCapture_ = DesktopImp::Create(DataControl::Instance());
}

void PeerconnectionManager::initScreenCapture()
{
    screenCapture_ = ScreenImp::Create(DataControl::Instance());
}

void PeerconnectionManager::AddTrack(VideoTrackSourceInterface *source, std::string StreamId)
{
    rtc::scoped_refptr<webrtc::VideoTrackInterface> video_track_(
                factory_->CreateVideoTrack(rtc::CreateRandomUuid(), source));
    if (connection_.get()== nullptr)
    {
        QLOG_ERROR() << "Failed AddTrack you must init connection.";
        return;
    }
    auto result_or_error = connection_->AddTrack(video_track_, {StreamId});
    if (!result_or_error.ok()) {
        QLOG_ERROR() << "Failed AddTrack."<<result_or_error.error().message();
    }
}

void PeerconnectionManager::AddLocalTrack(bool isDesktopCapture, const int loaclTrackSourceid)
{
    std::string StreamId = "0";
    rtc::scoped_refptr<webrtc::VideoTrackInterface> video_track;
    if(isDesktopCapture)
        video_track = factory_->CreateVideoTrack(kVideoLabel,deskCapture_);
    else
        video_track = factory_->CreateVideoTrack(kVideoLabel,screenCapture_);
    connection_->AddTrack(video_track,{StreamId});
}

//创建数据通道
void PeerconnectionManager::CreateDataChannel()
{
    std::string label = "datacannel";
    webrtc::DataChannelInit init;
    pDataChannel = connection_->CreateDataChannel(label,&init);
    if(pDataChannel != nullptr)
        pDataChannel->RegisterObserver(DataControl::Instance());
}

bool PeerconnectionManager::initPeerConnection(bool dtls, const std::string &turnip, const std::string &turnport, const std::string &turnusername, const std::string &turnpwd)
{
    rtc::WinsockInitializer winsock_init;
    rtc::Win32SocketServer w32_ss;
    rtc::Win32Thread w32_thread(&w32_ss);
    rtc::ThreadManager::Instance()->SetCurrentThread(&w32_thread);
    rtc::InitializeSSL();
    static rtc::Thread *SignalingThread;
    SignalingThread = rtc::Thread::Create().release();
    SignalingThread->SetName("SignalingThread",nullptr);
    SignalingThread->Start();
    factory_ = webrtc::CreatePeerConnectionFactory(
                nullptr /* network_thread */,
                nullptr /* worker_thread */,
                SignalingThread /* signaling_thread */,
                nullptr /* default_adm */,
                webrtc::CreateBuiltinAudioEncoderFactory(),
                webrtc::CreateBuiltinAudioDecoderFactory(),
                webrtc::CreateBuiltinVideoEncoderFactory(),
                webrtc::CreateBuiltinVideoDecoderFactory(), nullptr /* audio_mixer */,
                nullptr /* audio_processing */);
    if (!factory_.get()) {
        return false;
    }
    webrtc::PeerConnectionInterface::RTCConfiguration config;
    config.enable_dtls_srtp = 1;//datachannel 通道是否打开
    config.continual_gathering_policy = webrtc::PeerConnectionInterface::GATHER_ONCE;
    webrtc::PeerConnectionInterface::IceServer server;
    std::string turnurl;
    turnurl = "turn:" + turnip;
    server.uri=turnurl;
    server.username = turnusername;
    server.password = turnpwd;
    config.servers.push_back(server);
    connection_ = factory_->CreatePeerConnection(
                config, nullptr, nullptr, this);

    if (connection_.get() == nullptr) {
        return false;
    }
    //创建数据通道
    CreateDataChannel();
    QLOG_INFO()<<__FUNCTION__<<"SUCESS" <<"dtls = "<<dtls << "stunurl"<<QString::fromStdString(turnip);
    return true;
}

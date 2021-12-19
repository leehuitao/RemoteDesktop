#ifndef PEERCONNECTIONMANAGER_H
#define PEERCONNECTIONMANAGER_H
#include "SignalServerModule/SignalCallBack.h"
#include "api/jsep.h"
#include "api/peer_connection_interface.h"
#include "pc/video_track_source.h"
#include "api/create_peerconnection_factory.h"
#include "api/audio_codecs/builtin_audio_decoder_factory.h"
#include "api/audio_codecs/builtin_audio_encoder_factory.h"
#include "api/video_codecs/builtin_video_decoder_factory.h"
#include "api/video_codecs/builtin_video_encoder_factory.h"
#include <QObject>
#include <QTimer>
#include <api/jsep.h>
#include "webrtc/modules/video_capture/video_capture_impl.h"
#include "api/video/video_frame_buffer.h"
#include "modules/desktop_capture/desktop_capturer.h"
#include "modules/desktop_capture/desktop_capture_options.h"
#include "rtc_base/ssl_adapter.h"
#include "rtc_base/win32_socket_init.h"
#include "rtc_base/win32_socket_server.h"
#include "DesktopModule/desktopcapture/desktop_imp.h"
#include "DesktopModule/screencapture/screen_imp.h"
#include "SignalServerModule/signal_server_manager.h"
#include "data_control.h"
#include "datachannel_bundle_prase.h"
#include "DisplayModule/send_event_callback.h"
#include "message_protocol_define.h"
#define JOIN        "room_join"
#define READY       "peer_ready"
#define OFFER       "offer"
#define ANSWER      "answer"
#define CONNDIDATA   "condidata"
#define QUIT        "room_quit"
#define SENDMSG     "send_msg"
class DummySetSessionDescriptionObserver
        : public webrtc::SetSessionDescriptionObserver {
public:
    static DummySetSessionDescriptionObserver* Create() {
        return new rtc::RefCountedObject<DummySetSessionDescriptionObserver>();
    }
    virtual void OnSuccess()
    {
        QLOG_INFO ()<< __FUNCTION__;
    }
    virtual void OnFailure(webrtc::RTCError error) {
        QLOG_INFO ()<< __FUNCTION__ << " " << int(error.type())<< ": "
                      << error.message();
    }
};

using namespace  webrtc;
class PeerconnectionManager :public QObject,
        public webrtc::PeerConnectionObserver,
        public webrtc::CreateSessionDescriptionObserver,
        public SendEventCallBack
{
    Q_OBJECT
public:
    PeerconnectionManager();
    bool initPeerConnection(bool dtls,const std::string& turnip ,const std::string& turnport,
                            const std::string& turnusername ,const std::string& turnpwd );
    static rtc::scoped_refptr<PeerconnectionManager> Create();
public:

    void Destroy();
    void startCapture();
    void setLoaclId(int uid){uid_ = QString::number(uid);}
    void captureOneImage();
    void getScreenSourceList(QMap<int,QString>&  SourceMap);
    void sendDataChannel(SingalingData);
    void sendDataChannel(const  QString & msg);
public:
    //PeerConnectionObserver
    void OnSignalingChange(
            webrtc::PeerConnectionInterface::SignalingState new_state) override;
    void OnAddTrack(
            rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
            const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>&
            streams) override;
    void OnRemoveTrack(
            rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver) override;
    void OnDataChannel(
            rtc::scoped_refptr<webrtc::DataChannelInterface> channel) override;
    void OnRenegotiationNeeded() override;
    void OnIceConnectionChange(
            webrtc::PeerConnectionInterface::IceConnectionState new_state) override;
    void OnIceGatheringChange(
            webrtc::PeerConnectionInterface::IceGatheringState new_state) override;
    void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override;
    virtual void OnIceCandidateError(const std::string& host_candidate,
                                     const std::string& url,
                                     int error_code,
                                     const std::string& error_text)override;
    void OnIceConnectionReceivingChange(bool receiving) override;
public:
    // CreateSessionDescriptionObserver implementation.
    void OnSuccess(webrtc::SessionDescriptionInterface* desc) override;
    void OnFailure(webrtc::RTCError error) override;

public:
    // rtc::VideoSinkInterface<webrtc::VideoFrame>
    void setLoaclTrackDefault(bool isDesktopCapture,const int loaclTrackSourceid);
public:
    // 鼠标键盘发送事件
    void SendMouseEvent(Event event) override;
    void SendKeywordEvent(Event event) override;
    void setIsAccusedEnd(bool isAccusedEnd){
        isAccusedEnd_ = isAccusedEnd;
    }
Q_SIGNALS:
    void signICEConnect();
    void signOnDataChannel(char*);
    void signOnReciverVideo(QImage);
    void signICEConnected();
public Q_SLOTS:
    //信号服务器数据接收
    void slotReceiveJoin(SingalingData data);
    void slotReceiveReady(SingalingData data);
    void slotReceiveOffer(SingalingData data);
    void slotReceiveAnswer(SingalingData data);
    void slotReceiveCanidate(SingalingData data);
    void timeout();
    void initDesktopCapture();
    void initScreenCapture();

private:
    void AddTrack(webrtc::VideoTrackSourceInterface* source,std::string StreamId);
    void createOffer();
    void createAnswer();
    void AddLocalTrack(bool isDesktopCapture,const int loaclTrackSourceid);
    void CreateDataChannel();
private:
    QString roomid_;
    QString uid_;
    rtc::scoped_refptr<webrtc::PeerConnectionInterface>			connection_;
    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface>	factory_;
    QTimer*                                                     time;
    SingalingData                                               heart;
    std::unique_ptr<uint8_t[]>                                  m_image;
    std::unique_ptr<uint8_t[]>                                  m_yuv;
    webrtc::SessionDescriptionInterface *                       offerdesc = nullptr;
    webrtc::SessionDescriptionInterface *                       answerdesc = nullptr;
    rtc::scoped_refptr<ScreenImp>                               screenCapture_ =  nullptr;
    rtc::scoped_refptr<DesktopImp>                              deskCapture_ =  nullptr;
    rtc::scoped_refptr<DataChannelInterface>                    pDataChannel= nullptr;
    bool                                                        isDesktopCapture_ = false;
    int                                                         loaclTrackSourceid_ = 0;
    DataChannelBundlePrase                                      bundle;
    //是否为被控端
    bool                                                        isAccusedEnd_;
};

#endif // PEERCONNECTIONMANAGER_H

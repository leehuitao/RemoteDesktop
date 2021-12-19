#ifndef DATACONTROL_H
#define DATACONTROL_H
#include "DesktopModule/DesktopCallback.h"
#include <QObject>
#include "webrtc/modules/video_capture/video_capture_impl.h"
#include "api/peer_connection_interface.h"
#include "pc/video_track_source.h"
#include "api/create_peerconnection_factory.h"
#include "datachannel_bundle_prase.h"
#include "SignalServerModule/SignalCallBack.h"
#include "DisplayModule/send_event_callback.h"
#include "databuffer_task_thread.h"
class ImageDataCallBack{
public:
    ImageDataCallBack(){}
    virtual ~ImageDataCallBack(){}
    virtual void OnImage(uint8_t * data,int w ,int h)=0;
    virtual void OnMouseEvent(Event event)=0;
};
class DataControl :public QObject,
        public DesktopQImageCallBack,
        public ScreenQImageCallBack,
        public rtc::VideoSinkInterface<webrtc::VideoFrame>,
        public webrtc::DataChannelObserver
{
    Q_OBJECT
public:
    static DataControl * Instance(){
        return  dataControl_;
    }
public:
    virtual void OnDesktopLocalImage(QImage image)  override;
    virtual void OnDesktopChooseLocalImage(int sourceid,QString sourcetitle,QImage image)  override;
    virtual void OnScreenLocalImage(QImage image )  override;
    virtual void OnScreenChooseLocalImage(int sourceid,QString sourcetitle, QImage image)  override;
    void OnFrame(const webrtc::VideoFrame& frame) override;
    //DataChannelObserver  callback
    virtual void OnStateChange() override;
    virtual void OnMessage(const webrtc::DataBuffer& buffer) override;
    void setCallback(ImageDataCallBack *);
    void  setImageSize(int w, int h){
        ui_width = w;
        ui_height = h;
    }
Q_SIGNALS:
    void signDesktopLocalImage(QImage);
    void signScreenLocalImage(QImage);
    void signRemoteImage(QImage);
    void signDesktopChooseLocalImage(int , QString ,QImage);
    void signScreenChooseLocalImage(int , QString ,QImage);
    void signOnDataChannel(SingalingData);
    void signOnCursorEvent(int);
private:

    DataControl();
private:
    static DataControl *            dataControl_;
    std::unique_ptr<uint8_t[]>      m_image;
    DataChannelBundlePrase          prase;
    int                             ui_width = 1920;
    int                             ui_height = 1080;
    ImageDataCallBack *             callback_;
    DataBufferTaskThread *          m_dataBufferTaskThread;
};

#endif // DATACONTROL_H

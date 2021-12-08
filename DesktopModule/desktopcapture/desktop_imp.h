#ifndef DESKTOPIMP_H
#define DESKTOPIMP_H
#include <QThread>
#include <QObject>
#include "modules/desktop_capture/desktop_capturer.h"
#include "modules/desktop_capture/desktop_capture_options.h"
#include "media/base/adapted_video_track_source.h"
#include "webrtc/modules/video_capture/video_capture_impl.h"
#include "api/video/video_frame_buffer.h"
#include "libyuv.h"
#include "webrtc/api/video/i420_buffer.h"
#include "api/video/i420_buffer.h"
#include "third_party/libyuv/include/libyuv.h"
#include "rtc_base/ssl_adapter.h"
#include "api/peer_connection_interface.h"
#include "api/create_peerconnection_factory.h"
#include "LogModule/romote_log.h"
#include "../DesktopCallback.h"
#include <QTimer>
#include "modules/desktop_capture/differ_vector_sse2.h"
#include "desktop_source_list.h"
/*
* 应用截屏
*/
using namespace webrtc;
class DesktopImp: public webrtc::DesktopCapturer::Callback,
        public rtc::AdaptedVideoTrackSource,
        public UpdateSourceCallBack
{
public:

    DesktopImp(DesktopQImageCallBack * callback);

    static rtc::scoped_refptr<DesktopImp> Create(DesktopQImageCallBack * callback);

    void setCaptrueDefault(int sourceid,int retType) = delete ;

    void startCaptrue(const int sourceid,int frames = 16);

    void stopCaptrue();

    virtual void OnCaptureResult(webrtc::DesktopCapturer::Result result, std::unique_ptr<webrtc::DesktopFrame> frame) override;

    bool is_screencast() const override;

    void init();

    void captureOneFrame();

    absl::optional<bool> needs_denoising() const override;

    webrtc::MediaSourceInterface::SourceState state() const override;

    bool remote() const override;

    virtual void OnDesktopSourceUpdate(QMap<int,QString> sourceList) override;
private Q_SLOTS:
    void timeout();
private:
    void updateChooseImage(const int index);
private:
    int sourceid_ =1;
    int retType_ = 0;
    bool startflag = true;
    int selectSource_= 0;
    std::unique_ptr<webrtc::DesktopCapturer> desk_capture_ = nullptr;
    DesktopQImageCallBack * callback_ = nullptr;
    QTimer *time;
    DesktopSourceList * sourceList_;
    QMap<int,QString> sourceMap_;
    QList<int> sourceidList;
    QList<QString> sourcetitleList;
    int sourceSize = 0;
    int nowSelectIndex = 0;
};

#endif // DESKTOPIMP_H

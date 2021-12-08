#ifndef SCREENIMP_H
#define SCREENIMP_H
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
#include "screen_source_list.h"
#include <QThread>
#include "screen_thread.h"
/*
*桌面截屏
*/
using namespace webrtc;
class ScreenImp: public webrtc::DesktopCapturer::Callback,
        public rtc::AdaptedVideoTrackSource,
        public UpdateSourceCallBack,
        public QThread,
        public CapThreadObServer
{
public:

    ScreenImp(ScreenQImageCallBack * callback);

    static rtc::scoped_refptr<ScreenImp> Create(ScreenQImageCallBack * callback);

    void setCaptrueDefault(int sourceid,int retType) = delete ;
    /**
     * @brief 开启本地桌面流
     * @param sourceid 需要获取的本地源 id
     * @param frames 帧数默认16
     */
    void startCaptrue(const int sourceid,int frames = 16);

    void start();
    void stopCaptrue();

    virtual void OnCaptureResult(webrtc::DesktopCapturer::Result result, std::unique_ptr<webrtc::DesktopFrame> frame) override;

    bool is_screencast() const override;

    void init();

    void captureOneFrame();

    absl::optional<bool> needs_denoising() const override;

    webrtc::MediaSourceInterface::SourceState state() const override;

    bool remote() const override;

    void OnScreenSourceUpdate(QMap<int,QString> sourceList) override;

    virtual void CaptureFrame() override;

    void getScreenSourceList(QMap<int,QString>&  SourceMap);
private:

    void updateChooseImage(const int index);

private Q_SLOTS:
    void timeout();
private:
    int sourceid_ =1;
    int retType_ = 0;
    bool startflag = true;
    int selectSource_= 0;
    std::unique_ptr<webrtc::DesktopCapturer> screen_capture_ = nullptr;
    ScreenQImageCallBack * callback_ = nullptr;
    QTimer *time;
    ScreenSourceList *sourceList_;
    QMap<int,QString> sourceMap_;
    QList<int> sourceidList;
    QList<QString> sourcetitleList;
    int sourceSize = 0;
    int nowSelectIndex = 0;
    capthread* capthread_ = nullptr;
};
#endif // SCREENIMP_H

#include "desktop_imp.h"
#include <QImage>
rtc::scoped_refptr<DesktopImp> DesktopImp::Create(DesktopQImageCallBack *callback)
{
    rtc::scoped_refptr<DesktopImp> cpt = new rtc::RefCountedObject<DesktopImp>(callback);
    return cpt;
}

void DesktopImp::startCaptrue(const int sourceid, int frames)
{
    sourceid_ = sourceid;
    startflag = true;
    desk_capture_->SelectSource(selectSource_);
    desk_capture_->Start(this);
    time->start(1000/frames);
}

void DesktopImp::stopCaptrue()
{
    time->stop();
    startflag = false;
}

void DesktopImp::captureOneFrame()
{
    if(desk_capture_ != nullptr)
        desk_capture_->CaptureFrame();
}

void DesktopImp::OnCaptureResult(DesktopCapturer::Result result, std::unique_ptr<DesktopFrame> frame)
{
    if(startflag){
        if(result != webrtc::DesktopCapturer::Result::SUCCESS ){
            QLOG_ERROR()<<"OnCaptureResult ERROR!";
            return;
        }

        int width = frame->size().width();
        int height = frame->size().height();

        rtc::scoped_refptr<webrtc::I420Buffer> buffer = webrtc::I420Buffer::Create(width, height);
        int stride = width;
        uint8_t* yplane = buffer->MutableDataY();
        uint8_t* uplane = buffer->MutableDataU();
        uint8_t* vplane = buffer->MutableDataV();
        int ret = libyuv::ConvertToI420(frame->data(), 0,
                                        yplane, stride,
                                        uplane, (stride + 1) / 2,
                                        vplane, (stride + 1) / 2,
                                        0, 0,
                                        width, height,
                                        width, height,
                                        libyuv::kRotate0, libyuv::FOURCC_ARGB);
        webrtc::VideoFrame i420frame = webrtc::VideoFrame(buffer, 0, 0, webrtc::kVideoRotation_0);

        if(!ret){
            this->OnFrame(i420frame);
        }else {
            QLOG_ERROR()<<"libyuv::ConvertToI420 ERROR!";
        }
        if(callback_!= nullptr){
            QImage image((uchar*)frame->data(), frame->size().width(), frame->size().height(), QImage::Format_ARGB32);
            callback_->OnDesktopLocalImage(image);
        }
    }else{
        QImage image((uchar*)frame->data(), frame->size().width(), frame->size().height(), QImage::Format_ARGB32);
        callback_->OnDesktopChooseLocalImage(sourceidList.at(nowSelectIndex),sourcetitleList.at(nowSelectIndex),image);
        updateChooseImage(nowSelectIndex++);
    }
}

bool DesktopImp::is_screencast() const
{
    return true;
}

void DesktopImp::init()
{
    webrtc::DesktopCaptureOptions opts = webrtc::DesktopCaptureOptions::CreateDefault();
#ifdef WIN32
    opts.set_allow_directx_capturer(true);
#endif
    desk_capture_ =webrtc::DesktopCapturer::CreateWindowCapturer(opts);
    webrtc::WindowId wid = (webrtc::WindowId)GetDesktopWindow();
    desk_capture_->SetExcludedWindow(wid);
    time = new QTimer;
}

absl::optional<bool> DesktopImp::needs_denoising() const
{
    return false;
}

MediaSourceInterface::SourceState DesktopImp::state() const
{
    return webrtc::MediaSourceInterface::kLive;
}

bool DesktopImp::remote() const
{
    return false;
}

void DesktopImp::OnDesktopSourceUpdate(QMap<int, QString> sourceList)
{
    sourceMap_ = sourceList;
    QMap<int, QString>::iterator iter = sourceMap_.begin();
    while (iter != sourceMap_.end())
    {
        sourceidList.append(iter.key());
        sourcetitleList.append(iter.value());
        iter++;

    }
    sourceSize = sourceidList.size();
    if(desk_capture_ != nullptr && !startflag){
        nowSelectIndex = 0;
        updateChooseImage(nowSelectIndex);
    }
}

void DesktopImp::timeout()
{
    captureOneFrame();
}

DesktopImp::DesktopImp(DesktopQImageCallBack *callback)
{
    callback_ = callback;
    init();
    sourceList_ = new DesktopSourceList(this);
}

void DesktopImp::updateChooseImage(const int index)
{
    if(index > sourceidList.size()-1||startflag)
        return;
    desk_capture_->SelectSource(sourceidList.at(index));
    captureOneFrame();
}

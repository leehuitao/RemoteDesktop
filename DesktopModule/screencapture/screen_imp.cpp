#include "screen_imp.h"

#include <QImage>
#include <QAtomicInt>
#include <QDebug>
#include "modules/desktop_capture/desktop_and_cursor_composer.h"
rtc::scoped_refptr<ScreenImp> ScreenImp::Create(ScreenQImageCallBack *callback)
{
    rtc::scoped_refptr<ScreenImp> cpt = new rtc::RefCountedObject<ScreenImp>(callback);
    return cpt;
}

void ScreenImp::startCaptrue(const int sourceid,int frames)
{
    qDebug()<<__FUNCTION__;
    sourceid_ = sourceid;
    startflag = true;
    webrtc::DesktopCaptureOptions opts = webrtc::DesktopCaptureOptions::CreateDefault();
    opts.set_allow_directx_capturer(true);
    screen_capture_ =webrtc::DesktopCapturer::CreateScreenCapturer(opts);
//            std::unique_ptr<webrtc::DesktopCapturer>(
//                new webrtc::DesktopAndCursorComposer(
//                    webrtc::DesktopCapturer::CreateScreenCapturer(opts), opts));//=webrtc::DesktopCapturer::CreateScreenCapturer(opts);
    webrtc::WindowId wid = (webrtc::WindowId)GetDesktopWindow();
    screen_capture_->SetExcludedWindow(wid);
    screen_capture_->SelectSource(sourceid_);
    screen_capture_->Start(this);
}

void ScreenImp::start()
{
    capthread_->Start();
}

void ScreenImp::stopCaptrue()
{
    time->stop();
    startflag = false;
}

void ScreenImp::captureOneFrame()
{
    if(screen_capture_ != nullptr)
        screen_capture_->CaptureFrame();
}

void ScreenImp::OnCaptureResult(DesktopCapturer::Result result, std::unique_ptr<DesktopFrame> frame)
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
            callback_->OnScreenLocalImage(image);
        }
    }else{
        QImage image((uchar*)frame->data(),frame->size().width(), frame->size().height(), QImage::Format_ARGB32);
        callback_->OnScreenChooseLocalImage(sourceidList.at(nowSelectIndex),sourcetitleList.at(nowSelectIndex),image);
        updateChooseImage(nowSelectIndex++);
    }

}

bool ScreenImp::is_screencast() const
{
    return true;
}

void ScreenImp::init()
{
    qDebug()<<__FUNCTION__;

    time = new QTimer;

}

absl::optional<bool> ScreenImp::needs_denoising() const
{
    return false;
}

MediaSourceInterface::SourceState ScreenImp::state() const
{
    return webrtc::MediaSourceInterface::kLive;
}

bool ScreenImp::remote() const
{
    return false;
}

void ScreenImp::OnScreenSourceUpdate(QMap<int, QString> sourceList)
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
    if(screen_capture_ != nullptr && !startflag){
        nowSelectIndex = 0;
        updateChooseImage(nowSelectIndex);
    }
}

void ScreenImp::CaptureFrame()
{
    captureOneFrame();
}

void ScreenImp::getScreenSourceList(QMap<int, QString> &SourceMap)
{
    sourceList_->getScreenSourceList(SourceMap);
}

ScreenImp::ScreenImp( ScreenQImageCallBack *callback)
{
    qDebug()<<__FUNCTION__;
    callback_ = callback;
    init();
    sourceList_ = new ScreenSourceList(this);
}

void ScreenImp::updateChooseImage(const int index)
{
    if(index > sourceidList.size()-1||startflag)
        return;
    screen_capture_->SelectSource(sourceidList.at(index));
    captureOneFrame();
}

void ScreenImp::timeout()
{
    captureOneFrame();
}

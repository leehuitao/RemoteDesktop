#include "desktop_source_list.h"

DesktopSourceList::DesktopSourceList(UpdateSourceCallBack *callback)
{
    callback_ = callback;
    webrtc::DesktopCaptureOptions opts = webrtc::DesktopCaptureOptions::CreateDefault();
    desktop_capture_ =webrtc::DesktopCapturer::CreateScreenCapturer(opts);
    updateTimer = new QTimer(this);
    connect(updateTimer,&QTimer::timeout,this,&DesktopSourceList::updateSourceList);
    updateTimer->start(1000);
}

void DesktopSourceList::updateSourceList()
{
    QMutexLocker lock(&mutex);
    QMap<int,QString> cache(SourceMap_);
    SourceMap_.clear();
    webrtc::DesktopCapturer::SourceList* sources = new webrtc::DesktopCapturer::SourceList;
    desktop_capture_->GetSourceList(sources);
    for(int i =0 ;i<int(sources->size());i++){
        SourceMap_[int(sources->at(i).id)] = QString::fromStdString(sources->at(i).title);
    }
    callback_->OnDesktopSourceUpdate(SourceMap_);
}

void DesktopSourceList::compareMap()
{

}

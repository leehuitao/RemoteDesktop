#include "screen_source_list.h"
#include <QDebug>
ScreenSourceList::ScreenSourceList(UpdateSourceCallBack * callback)
{
    callback_ = callback;
    webrtc::DesktopCaptureOptions opts = webrtc::DesktopCaptureOptions::CreateDefault();
    screen_capture_ =webrtc::DesktopCapturer::CreateScreenCapturer(opts);
    updateTimer = new QTimer(this);
    connect(updateTimer,&QTimer::timeout,this,&ScreenSourceList::updateSourceList);
    //updateTimer->start(1000);
    updateSourceList();
}

void ScreenSourceList::getScreenSourceList(QMap<int, QString> &SourceMap)
{
    updateSourceList();
    SourceMap = SourceMap_;
}

void ScreenSourceList::updateSourceList()
{
    QMutexLocker lock(&mutex);
    QMap<int,QString> cache(SourceMap_);
    SourceMap_.clear();
    webrtc::DesktopCapturer::SourceList* sources = new webrtc::DesktopCapturer::SourceList;
    screen_capture_->GetSourceList(sources);
    for(int i =0 ;i<int(sources->size());i++){
        SourceMap_[int(sources->at(i).id)] = QString::fromStdString(sources->at(i).title);
        qDebug()<<"SourceMap_ "<<SourceMap_;
    }
    if(callback_ != nullptr){}
        //callback_->OnScreenSourceUpdate(SourceMap_);
}

void ScreenSourceList::compareMap()
{

}

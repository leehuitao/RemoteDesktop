#include "data_control.h"
#include "api/video/video_frame_buffer.h"
#include "webrtc/modules/video_capture/video_capture_impl.h"
#include <QDebug>
#include <string.h>
#include <stdio.h>
#include <api/video/i420_buffer.h>
DataControl * DataControl::dataControl_ = new DataControl;
void DataControl::OnDesktopLocalImage(QImage image)
{
    //qDebug()<<__FUNCTION__;
    signDesktopLocalImage(image);
}

void DataControl::OnDesktopChooseLocalImage(int sourceid, QString sourcetitle, QImage image)
{
    //qDebug()<<__FUNCTION__;
    signDesktopChooseLocalImage(sourceid,sourcetitle,image);
}

void DataControl::OnScreenLocalImage(QImage image)
{
    //qDebug()<<__FUNCTION__;
    signScreenLocalImage(image);
}

void DataControl::OnScreenChooseLocalImage(int sourceid, QString sourcetitle, QImage image)
{
    //qDebug()<<__FUNCTION__;
    signScreenChooseLocalImage(sourceid,sourcetitle,image);
}

//控制方接收数据
void DataControl::OnFrame(const webrtc::VideoFrame &frame)
{
    int frameWidth = frame.width();
    int frameHeight = frame.height();
    if (ui_height != frameWidth || ui_width != frameHeight) {
        rtc::scoped_refptr<webrtc::I420Buffer> scaled_buffer =
                webrtc::I420Buffer::Create(ui_width, ui_height);
        scaled_buffer->ScaleFrom(*frame.video_frame_buffer()->ToI420());
        webrtc::VideoFrame::Builder new_frame_builder =
                webrtc::VideoFrame::Builder()
                .set_video_frame_buffer(scaled_buffer)
                .set_rotation(webrtc::kVideoRotation_0)
                .set_timestamp_us(frame.timestamp_us())
                .set_id(frame.id());
        if (frame.has_update_rect()) {
            webrtc::VideoFrame::UpdateRect new_rect =
                    frame.update_rect().ScaleWithFrame(frame.width(), frame.height(), 0,
                                                       0, frame.width(), frame.height(),
                                                       ui_width, ui_height);
            new_frame_builder.set_update_rect(new_rect);
        }
        auto newf = new_frame_builder.build();
        m_image.reset(new uint8_t[frameWidth*frameHeight*8]);
        webrtc::ConvertFromI420(newf,webrtc::VideoType::kARGB,0,m_image.get());
        int frameWidth = newf.width();
        int frameHeight = newf.height();
        if(callback_)
            callback_->OnImage(m_image.get(),frameWidth,frameHeight);
    }else{
        m_image.reset(new uint8_t[frameWidth*frameHeight*8]);
        webrtc::ConvertFromI420(frame,webrtc::VideoType::kARGB,0,m_image.get());
        if(callback_)
            callback_->OnImage(m_image.get(),frameWidth,frameHeight);
    }

}

void DataControl::OnStateChange()
{

}

void DataControl::OnMessage(const webrtc::DataBuffer &buffer)
{
    SingalingData data;
    char * ch = new char[buffer.size()];
    memcpy(ch,(char*)buffer.data.data(),buffer.size());
    std::string str(ch);
    QString newS = QString::fromStdString(str);
    auto index = newS.lastIndexOf("}");
    //auto index = str.find_first_of("}");
    newS = newS.mid(0,index+1);
    QByteArray response(newS.toLocal8Bit().data(),int(newS.size()));
    prase.parseResponse(response,data);
    qDebug()<<"datachannel recive "<<newS <<"total ="<< newS.size()<<"index = "<<index;
    if(data.data["type"].toInt() != MouseCursorChange){
        Event event;
        event.type = data.data["type"].toInt();
        event.posx = data.data["posx"].toInt();
        event.posy = data.data["posy"].toInt();
        event.delta = data.data["delta"].toInt();
        event.keyword = data.data["keyword"].toInt();
        event.coursorShape = data.data["coursorShape"].toInt();
        if(callback_)
            callback_->OnMouseEvent(event);

    }else{
        signOnCursorEvent(data.data["coursorShape"].toInt());
    }

}

void DataControl::setCallback(ImageDataCallBack *callback)
{
    callback_ = callback;
}

DataControl::DataControl()
{

}

#include "databuffer_task_thread.h"
#include "message_protocol_define.h"
DataBufferTaskThread::DataBufferTaskThread(QObject *parent) : QThread(parent)
{

}

void DataBufferTaskThread::addTask(webrtc::DataBuffer databuffer)
{
    m_mutex.lock();
    webrtc::DataBuffer child(databuffer);
    m_taskList.append(&databuffer);
    m_mutex.unlock();
    m_Condition.notify_all();
}

void DataBufferTaskThread::popTask(webrtc::DataBuffer * databuffer)
{
    m_mutex.lock();
    if(m_taskList.size() == 0 )
        return;
    databuffer = m_taskList.first();
    m_taskList.removeFirst();
    m_mutex.unlock();
}

void DataBufferTaskThread::run()
{
    QDir folder;
    int size = 0;
    int cmd = 0;
    while(!m_ThreadStop){
        m_mutex.lock();
        if(m_taskList.size() <= 0)
        {
            m_Condition.wait(&m_mutex);
            m_mutex.unlock();
        }
        webrtc::DataBuffer * databuffer = nullptr;
        popTask(databuffer);
        if(databuffer == nullptr)
            continue;
        auto buffer = databuffer->data;
        const char * data = buffer.data<char>();
        QByteArray arr(data);
        QDataStream packet(arr);
        packet >> size >> cmd;//解析包头
        MsgDataBuffer msgDataBuffer(arr,size,cmd);
        if(cmd == 1){
            qDebug()<<"recv msg = "<<msgDataBuffer.body;
            signRecvMsg(msgDataBuffer.body);
        }else if(cmd == 10){
            SingalingData data;
            std::string str(msgDataBuffer.body);
            QString newS = QString::fromStdString(str);
            auto index = newS.lastIndexOf("}");
            //auto index = str.find_first_of("}");
            newS = newS.mid(0,index+1);
            QByteArray response(newS.toLocal8Bit().data(),int(newS.size()));
            m_dataPrase.parseResponse(response,data);
            qDebug()<<"datachannel recive "<<newS <<"total ="<< newS.size()<<"index = "<<index;
            if(data.data["type"].toInt() != MouseCursorChange){
                Event event;
                event.type = data.data["type"].toInt();
                event.posx = data.data["posx"].toInt();
                event.posy = data.data["posy"].toInt();
                event.delta = data.data["delta"].toInt();
                event.keyword = data.data["keyword"].toInt();
                event.coursorShape = data.data["coursorShape"].toInt();
                signRecvMouseEvent(event);
                qDebug()<<"recv MouseEvent = "<<msgDataBuffer.body;

            }else{
                signCursorEvent(data.data["coursorShape"].toInt());
                qDebug()<<"recv CursorEvent = "<<msgDataBuffer.body;
            }
        }
    }
}


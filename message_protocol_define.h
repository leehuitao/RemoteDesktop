#ifndef MESSAGE_PROTOCOL_DEFINE_H
#define MESSAGE_PROTOCOL_DEFINE_H
#include <QByteArray>
#include <QDataStream>
#include <QIODevice>

struct DataHeader{
    int         packsize;
    int         cmd;//1 消息 2文件开始传输 3文件传输 4文件继续传输  10 鼠标键盘事件
};

struct MsgDataBuffer{
    DataHeader  header;
    QByteArray  body;
    MsgDataBuffer(){}
    MsgDataBuffer(QByteArray buffer , int size ,int cmd){
        header.packsize = size;
        header.cmd = cmd;
        char *rbytes = new char[size - sizeof (DataHeader)];
        memset(rbytes,0,size-sizeof (DataHeader));
        memcpy(rbytes,buffer.data()+sizeof (DataHeader),size-sizeof (DataHeader));
        QByteArray arr;
        arr.append(rbytes,size-sizeof (DataHeader));
        arr.resize(size-sizeof (DataHeader));
        body.append(arr);
        delete [] rbytes;
    }
    MsgDataBuffer(QString msg){
        header.cmd = 1;
        body = msg.toUtf8().data();
    }
    QByteArray toByte(){
        QByteArray m_buffer;
        QDataStream packet(&m_buffer,QIODevice::WriteOnly);
        int size = sizeof (DataHeader)+ body.size();
        packet<<size<<header.cmd;
        m_buffer += body.data();
        return m_buffer;
    }
};


#endif // MESSAGE_PROTOCOL_DEFINE_H

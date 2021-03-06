#ifndef DATABUFFERTASKTHREAD_H
#define DATABUFFERTASKTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <queue>
#include <iostream>
#include <QMutex>
#include <QMutexLocker>
#include <QDir>
#include <QImage>
#include <QWaitCondition>
#include <QList>
#include <api/data_channel_interface.h>
#include <DisplayModule/send_event_callback.h>
#include "datachannel_bundle_prase.h"

class DataBufferTaskThread : public QThread
{
    Q_OBJECT
public:
    explicit DataBufferTaskThread(QObject *parent = nullptr);

    void addTask(webrtc::DataBuffer);

    ~DataBufferTaskThread() override {
        m_ThreadStop = 1;
        m_Condition.notify_all();
    }
protected:
    void run() override;

      void popTask(webrtc::DataBuffer *);

Q_SIGNALS:
    //鼠标键盘事件
    void signRecvMouseEvent(Event);
    // 光标改变
    void signCursorEvent(int);
    //收到普通消息
    void signRecvMsg(QString);
    /**
      signRecvFile 文件接收成功信号
      @ param path  文件存储路径
    */
    void signRecvFile(QString path);
public Q_SLOTS:

private:
    QList< webrtc::DataBuffer *>                    m_taskList;
    QMutex                                          m_mutex;
    QWaitCondition                                  m_Condition;
    bool                                            m_ThreadStop;
    DataChannelBundlePrase                          m_dataPrase;
};

#endif // DATABUFFERTASKTHREAD_H

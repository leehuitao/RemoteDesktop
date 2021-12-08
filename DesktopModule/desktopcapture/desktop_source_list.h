#ifndef DESKTOPSOURCELIST_H
#define DESKTOPSOURCELIST_H
#include "modules/desktop_capture/desktop_capturer.h"
#include "modules/desktop_capture/desktop_capture_options.h"
#include <QMap>
#include <QObject>
#include <QAtomicInt>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>
#include "../DesktopCallback.h"

class DesktopSourceList :public QObject
{
public:
    DesktopSourceList(UpdateSourceCallBack *);

private Q_SLOTS:
    void updateSourceList();

private:
    void compareMap();
private:
    UpdateSourceCallBack *callback_;
    QMutex mutex;
    QTimer *updateTimer;
    QMap<int,QString>  SourceMap_;
    std::unique_ptr<webrtc::DesktopCapturer> desktop_capture_= nullptr;
};

#endif // DESKTOPSOURCELIST_H

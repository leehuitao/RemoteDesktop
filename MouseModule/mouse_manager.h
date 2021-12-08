#ifndef MOUSEMANAGER_H
#define MOUSEMANAGER_H

#include <QObject>
#include "DisplayModule/send_event_callback.h"
class MouseManager : public QObject
{
    Q_OBJECT
public:
    explicit MouseManager(SendEventCallBack*);
    void mouseMoveEvent(int eventx ,int eventy);         //移动
    void mousePressEvent(int eventx ,int eventy,uint8_t mousekind);        //单击
    void mouseReleaseEvent(int eventx ,int eventy,uint8_t mousekind);      //释放
    void mouseDoubleClickEvent(int eventx ,int eventy,uint8_t mousekind);  //双击
    void wheelEvent(int eventx ,int eventy,int eventdelta);             //滑轮
    void mouseCursorEvent(int cursor);
    void setWH(int w,int h){
        w_ = w;
        h_= h;
    }
private:
    SendEventCallBack* callback_ = nullptr;
    int w_ = 1920;
    int h_ = 1080;
};

#endif // MOUSEMANAGER_H

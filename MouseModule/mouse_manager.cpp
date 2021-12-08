#include "mouse_manager.h"
#include <QDataStream>
#include <QIODevice>
#include <QDebug>
#include <QCursor>
//type {
//0 MOUSEEVENTF_ABSOLUTE 是否使用绝对坐标

//1 MOUSEEVENTF_LEFTDOWN 鼠标左键按下

//2 MOUSEEVENTF_LEFTUP 鼠标左键松开

//3 MOUSEEVENTF_MIDDLEDOWN 鼠标中键按下

//4 MOUSEEVENTF_MIDDLEUP 鼠标中键松开

//5 MOUSEEVENTF_MOVE 鼠标移动

//6 MOUSEEVENTF_RIGHTDOWN 鼠标右键按下

//7 MOUSEEVENTF_RIGHTUP 鼠标右键按下

//8 MOUSEEVENTF_WHEEL 鼠标滑轮

//9 MOUSEEVENTF_DOUBLECLICKED 左键双击
//}
MouseManager::MouseManager(SendEventCallBack *callback ):callback_(callback)
{

}

void MouseManager::mouseMoveEvent(int eventx, int eventy)
{
    Event mouseEvent;
    mouseEvent.type = MouseMove;
    double x = eventx;
    double y = eventy;
    mouseEvent.posx = uint16_t((x / w_ )*1920);
    mouseEvent.posy = uint16_t((y / h_) *1080);
    qDebug()<< mouseEvent.posx<<mouseEvent.posy;
    callback_->SendMouseEvent(mouseEvent);
}

void MouseManager::mousePressEvent(int eventx, int eventy, uint8_t mousekind)
{
    Event mouseEvent;
    double x = eventx;
    double y = eventy;
    mouseEvent.posx = uint16_t((x / w_) *1920);
    mouseEvent.posy = uint16_t((y / h_) *1080);
    if(mousekind==0)//left
        mouseEvent.type = MouseLeftDown;
    else if(mousekind==1)//right
        mouseEvent.type = MouseRightDown;
    else if(mousekind==2)//wheel
        mouseEvent.type = MouseWheelDown;
    callback_->SendMouseEvent(mouseEvent);
}

void MouseManager::mouseReleaseEvent(int eventx, int eventy, uint8_t mousekind)
{
    Event mouseEvent;
    double x = eventx;
    double y = eventy;
    mouseEvent.posx = uint16_t((x / w_ )*1920);
    mouseEvent.posy = uint16_t((y / h_) *1080);
    if(mousekind==0)//left
        mouseEvent.type = MouseLeftUp;
    else if(mousekind==1)//right
        mouseEvent.type = MouseRightUp;
    else if(mousekind==2)//wheel
        mouseEvent.type = MouseWheelUp;
    callback_->SendMouseEvent(mouseEvent);
}

void MouseManager::mouseDoubleClickEvent(int eventx, int eventy, uint8_t mousekind)
{
    if(mousekind == 0){//left
        Event mouseEvent;
        mouseEvent.type = MouseLeftDoubleClicked;
        double x = eventx;
        double y = eventy;
        mouseEvent.posx =  uint16_t((x / w_ )*1920);
        mouseEvent.posy = uint16_t((y / h_) *1080);
        callback_->SendMouseEvent(mouseEvent);
    }

}

void MouseManager::wheelEvent(int eventx, int eventy, int eventdelta)
{
    Event mouseEvent;
    mouseEvent.type = MouseWheel;
    double x = eventx;
    double y = eventy;
    mouseEvent.posx = uint16_t((x / w_ )*1920);
    mouseEvent.posy = uint16_t((y / h_) *1080);
    mouseEvent.delta = eventdelta;
    callback_->SendMouseEvent(mouseEvent);
}

void MouseManager::mouseCursorEvent(int cursor)
{
    Event mouseEvent;
    mouseEvent.type = MouseCursorChange;
    mouseEvent.posx = 0;
    mouseEvent.posy = 0;
    mouseEvent.coursorShape = cursor;
    callback_->SendMouseEvent(mouseEvent);
}

#ifndef SEND_EVENT_CALLBACK_H
#define SEND_EVENT_CALLBACK_H

#include <QByteArray>
#include <QString>
/**
 * @brief  鼠标键盘 事件 发送
 * @param type    指令类型
 * @param posx x 坐标
 * @param posy  y 坐标
 * @param delta  滚轮 滑动距离
 * @param keyword 键盘按下
 * @param coursorShape 鼠标样式
 */
struct Event{
    uint8_t             type;
    uint16_t            posx;
    uint16_t            posy;
    int                 delta;
    int                 keyword;
    uint8_t             coursorShape;
};

enum EventType{
    MouseLeftDown           = 1,
    MouseLeftUp             ,
    MouseWheelDown          ,
    MouseWheelUp            ,
    MouseMove               ,
    MouseRightDown          ,
    MouseRightUp            ,
    MouseWheel              ,
    MouseLeftDoubleClicked  ,
    KeywordDown             ,
    KeywordUp               ,
    MouseCursorChange
};
/**
 * @brief 鼠标键盘发送
 */
class SendEventCallBack{
public:
    SendEventCallBack(){}
    virtual ~SendEventCallBack(){}
    /**
     * @brief 鼠标发送 事件
     * @param data 数据包
     */
    virtual void SendMouseEvent(Event event) = 0;
    /**
     * @brief 键盘发送 事件
     * @param data 数据包
     */
    virtual void SendKeywordEvent(Event event) = 0;
};

#endif // SEND_EVENT_CALLBACK_H

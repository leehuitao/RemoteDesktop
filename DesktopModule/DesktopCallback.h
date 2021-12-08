#ifndef DESKTOPCALLBACK_H
#define DESKTOPCALLBACK_H
#include <QImage>
#include <QMap>
#include "modules/desktop_capture/desktop_capturer.h"
/**
 * @brief 应用截图回调
 */
class DesktopQImageCallBack{
public:
    DesktopQImageCallBack(){}
    virtual ~DesktopQImageCallBack(){}
    /**
     * @brief 应用图片返回
     * @param QImage图片
     */
    virtual void OnDesktopLocalImage(QImage) = 0;
    /**
     * @brief 返回选择界面的缩略图
     * @param sourceid应用源id
     * @param sourcetitle应用源title
     * @param QImage 图片
     */
    virtual void OnDesktopChooseLocalImage(int sourceid,QString sourcetitle,QImage) = 0;
};
/**
 * @brief 桌面截图回调
 */
class ScreenQImageCallBack{
public:
    ScreenQImageCallBack(){}
    virtual ~ScreenQImageCallBack(){}
    virtual void OnScreenLocalImage(QImage) = 0;
    virtual void OnScreenChooseLocalImage(int sourceid,QString sourcetitle, QImage) = 0;
};
/**
 * @brief 获取到后桌面源回调
 */
class UpdateSourceCallBack{
public:
    UpdateSourceCallBack(){}
    virtual ~UpdateSourceCallBack(){}
    virtual void OnScreenSourceUpdate(QMap<int,QString>){}
    virtual void OnDesktopSourceUpdate(QMap<int,QString>){}
};

#endif // DESKTOPCALLBACK_H

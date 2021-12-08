#ifndef REMOTE_DESKTOP_H
#define REMOTE_DESKTOP_H

#include <QWidget>
#include "KeywordModule/keyword_manager.h"
#include "MouseModule/mouse_manager.h"
#include "../data_control.h"
#include <QTimer>
#include <atomic>
#include <QMutex>
namespace Ui {
class RemoteDesktopDisplay;
}

class  RemoteDesktopDisplay : public QWidget,
        public ImageDataCallBack
{
    Q_OBJECT

public:
    /**
     * @brief 显示类初始化
     * @param SendEventCallBack 数据通道的回调
     * @param QWidget 显示在哪个界面上
     */
    explicit RemoteDesktopDisplay(QWidget *parent = nullptr);
    ~RemoteDesktopDisplay();
    void setCallback(SendEventCallBack*);
    void mouseMoveEvent(QMouseEvent *event);         //移动
    void mousePressEvent(QMouseEvent *event);        //单击
    void mouseReleaseEvent(QMouseEvent *event);      //释放
    void mouseDoubleClickEvent(QMouseEvent *event);  //双击
    void wheelEvent(QWheelEvent *event);             //滑轮
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);
    int translateKeyCode(int key);
    virtual void OnImage(uint8_t * data,int w ,int h){
        QMutexLocker lock(&mutex);
        QImage pimage(data, w, h, QImage::Format_RGB32);
        //image_ = pimage;
        image_ = pimage.copy(0,0,w,h);
        update();
    }
    virtual void OnMouseEvent(Event event);
    void setIsAccuseEnd(bool isAccusedEnd){
        isAccusedEnd_ = isAccusedEnd;
    }
    void setIsCursor(bool isCursor){
        if(isCursor)
            QWidget::setCursor(QCursor(Qt::BlankCursor));
        else
            QWidget::setCursor(QCursor(Qt::ArrowCursor));
    }

Q_SIGNALS:

protected:
    void paintEvent(QPaintEvent *event) ;
private:
    void sendCursor();
private Q_SLOTS:
    void timeout();
    void onCursorEvent(int );
private:
    Ui::RemoteDesktopDisplay *ui;
    KeywordManager *keyword_ = nullptr;
    MouseManager *mouse_    = nullptr;
    int screenw;
    int screenh;
    QImage image_;
    bool isAccusedEnd_;
    QMutex mutex;
    HCURSOR lastCursor_ = 0;
};

#endif // REMOTE_DESKTOP_H

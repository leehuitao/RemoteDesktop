#include "remote_desktop.h"
#include "ui_remote_desktop.h"
#include <QMouseEvent>
#include <Windows.h>
#include <WinUser.h>
#include <QScreen>
#include <QPainter>
RemoteDesktopDisplay::RemoteDesktopDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RemoteDesktopDisplay)
{
    ui->setupUi(this);
    this->setMouseTracking(true);//设置窗体可响应 Mouse Move
    setFocusPolicy(Qt::StrongFocus);
    QScreen *screen=QGuiApplication::primaryScreen();
    QRect mm=screen->availableGeometry() ;
    screenw = mm.width();
    screenh = mm.height();//后面给windows发送鼠标事件的时候要用到
    qDebug()<<"screenw"<<screenw<<"screenh"<<screenh;
    DataControl::Instance()->setImageSize(this->width(),this->height());// 启动的时候设置一下 需要的图片大小
    DataControl::Instance()->setCallback(this);//修改收到的图片的原始数据
    connect(DataControl::Instance(),&DataControl::signOnCursorEvent,this,&RemoteDesktopDisplay::onCursorEvent);
}

RemoteDesktopDisplay::~RemoteDesktopDisplay()
{
    delete keyword_;
    delete mouse_;
    delete ui;
}

void RemoteDesktopDisplay::setCallback(SendEventCallBack *callback)
{
    keyword_ = new KeywordManager(callback);
    mouse_ = new MouseManager(callback);
}

void RemoteDesktopDisplay::mouseMoveEvent(QMouseEvent *event)
{
    if(isAccusedEnd_)
        return;
    int x = event->x();
    int y = event->y();
    if(mouse_ != nullptr)
        mouse_->mouseMoveEvent(x,y);
}

void RemoteDesktopDisplay::mousePressEvent(QMouseEvent *event)
{
    if(isAccusedEnd_)
        return;
    int x = event->x();
    int y = event->y();
    uint8_t mousekind=0;
    if(event->button()==Qt::LeftButton)//left
        mousekind=0;
    else if(event->button()==Qt::RightButton)//right
        mousekind=1;
    else if(event->button()==Qt::MidButton)//wheel
        mousekind=2;
    if(mouse_ != nullptr)
        mouse_->mousePressEvent(x,y,mousekind);
}

void RemoteDesktopDisplay::mouseReleaseEvent(QMouseEvent *event)
{
    if(isAccusedEnd_)
        return;
    int x = event->x();
    int y = event->y();
    uint8_t mousekind=0;
    if(event->button()==Qt::LeftButton)//left
        mousekind=0;
    else if(event->button()==Qt::RightButton)//right
        mousekind=1;
    else if(event->button()==Qt::MidButton)//wheel
        mousekind=2;
    if(mouse_ != nullptr)
        mouse_->mouseReleaseEvent(x,y,mousekind);
}

void RemoteDesktopDisplay::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(isAccusedEnd_)
        return;
    int x = event->x();
    int y = event->y();
    uint8_t mousekind=0;
    if(event->button()==Qt::LeftButton)//left
        mousekind=0;
    else if(event->button()==Qt::RightButton)//right
        mousekind=1;
    else if(event->button()==Qt::MidButton)//wheel
        mousekind=2;
    if(mouse_ != nullptr)
        mouse_->mouseDoubleClickEvent(x,y,mousekind);
}

void RemoteDesktopDisplay::wheelEvent(QWheelEvent *event)
{
    if(isAccusedEnd_)
        return;
    int x = event->x();
    int y = event->y();
    int delta= event->delta();
    if(mouse_ != nullptr)
        mouse_->wheelEvent(x,y,delta);
}

void RemoteDesktopDisplay::keyPressEvent(QKeyEvent *event)
{
    if(isAccusedEnd_)
        return;
    int keyword = event->key();
    if(keyword_ != nullptr)
        keyword_->keyPressEvent(keyword);
}

void RemoteDesktopDisplay::keyReleaseEvent(QKeyEvent *event)
{
    if(isAccusedEnd_)
        return;
    int keyword = event->key();
    if(keyword_ != nullptr)
        keyword_->keyReleaseEvent(keyword);
}

void RemoteDesktopDisplay::resizeEvent(QResizeEvent *event)
{
    int w = event->size().width();
    int h = event->size().height();
    if(!isAccusedEnd_){// 如果是控制端,设置一下发送鼠标的位置和缩放接收到的图片
        mouse_->setWH(w,h);
        DataControl::Instance()->setImageSize(w,h);//修改收到的图片的原始数据
    }else{

    }


}
#define POSX 65536
#define POSY 64000
void RemoteDesktopDisplay::OnMouseEvent(Event event)
{
    int type;
    switch (event.type) {
    case 0:

        break;
    case 1:
        type = MOUSEEVENTF_LEFTDOWN;
        break;
    case 2:
        type = MOUSEEVENTF_LEFTUP ;
        break;
    case 3:
        type =MOUSEEVENTF_MIDDLEDOWN ;
        break;
    case 4:
        type =MOUSEEVENTF_MIDDLEUP ;
        break;
    case 5:
        type = MOUSEEVENTF_MOVE;
        break;
    case 6:
        type =MOUSEEVENTF_RIGHTDOWN ;
        break;
    case 7:
        type =MOUSEEVENTF_RIGHTUP ;
        break;
    case 8:
        type =MOUSEEVENTF_WHEEL ;
        break;
    default:
        break;
    }
    if(event.type == 9){
        mouse_event(MOUSEEVENTF_LEFTDOWN ,(DWORD)event.posx*POSX/screenw,(DWORD)event.posy*POSY/screenh,(DWORD)event.delta,0);
        mouse_event(MOUSEEVENTF_LEFTUP ,(DWORD)event.posx*POSX/screenw,(DWORD)event.posy*POSY/screenh,(DWORD)event.delta,0);
//        mouse_event(MOUSEEVENTF_LEFTDOWN ,(DWORD)event.posx*POSX/screenw,(DWORD)event.posy*POSY/screenh,(DWORD)event.delta,0);
//        mouse_event(MOUSEEVENTF_LEFTUP ,(DWORD)event.posx*POSX/screenw,(DWORD)event.posy*POSY/screenh,(DWORD)event.delta,0);
    }else if(event.type == 10){
        keybd_event(translateKeyCode(event.keyword),0x45,KEYEVENTF_EXTENDEDKEY | 0, 0);
    }else if(event.type == 11){
        keybd_event(translateKeyCode(event.keyword),0x45,KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    }else if(event.type <= 8 && event.type >0){
        mouse_event(MOUSEEVENTF_ABSOLUTE|(DWORD)type ,(DWORD)event.posx*POSX/screenw,(DWORD)event.posy*POSY/screenh,(DWORD)event.delta,0);
    }
    sendCursor();
}

int RemoteDesktopDisplay::translateKeyCode(int key)
{
    int k = key;
    bool legal = true;
    if(k >= Qt::Key_0 && k <= Qt::Key_9)
    {
    }
    else if(k >= Qt::Key_A && k <= Qt::Key_Z)
    {
    }
    else if(k >= Qt::Key_F1 && k <= Qt::Key_F24)
    {
        k &= 0x000000ff;
        k += 0x40;
    }
    else if(k == Qt::Key_Tab)
    {
        k = 0x09;
    }
    else if(k == Qt::Key_Backspace)
    {
        k = 0x08;
    }
    else if(k == Qt::Key_Return)
    {
        k = 0x0d;
    }
    else if(k <= Qt::Key_Down && k >= Qt::Key_Left)
    {
        int off = k - Qt::Key_Left;
        k = 0x25 + off;
    }
    else if(k == Qt::Key_Shift)
    {
        k = 0x10;
    }
    else if(k == Qt::Key_Control)
    {
        k = 0x11;
    }
    else if(k == Qt::Key_Alt)
    {
        k = 0x12;
    }
    else if(k == Qt::Key_Meta)
    {
        k = 0x5b;
    }
    else if(k == Qt::Key_Insert)
    {
        k = 0x2d;
    }
    else if(k == Qt::Key_Delete)
    {
        k = 0x2e;
    }
    else if(k == Qt::Key_Home)
    {
        k = 0x24;
    }
    else if(k == Qt::Key_End)
    {
        k = 0x23;
    }
    else if(k == Qt::Key_PageUp)
    {
        k = 0x21;
    }
    else if(k == Qt::Key_Down)
    {
        k = 0x22;
    }
    else if(k == Qt::Key_CapsLock)
    {
        k = 0x14;
    }
    else if(k == Qt::Key_NumLock)
    {
        k = 0x90;
    }
    else if(k == Qt::Key_Space)
    {
        k = 0x20;
    }
    else
        legal = false;

    if(!legal)
        return 0;
    return k;
}

void RemoteDesktopDisplay::paintEvent(QPaintEvent *event)
{
    if(isAccusedEnd_)
        return;

    QPainter painter(this);
    QMutexLocker lock(&mutex);
    painter.drawImage(0, 0, image_);
}
HCURSOR GetCurrentCursorHandle()
{
    POINT pt;
    HWND hWnd;
    DWORD dwThreadID, dwCurrentThreadID;
    HCURSOR hCursor = NULL;

    // Find out which window owns the cursor
    GetCursorPos(&pt);
    hWnd = WindowFromPoint(pt);

    // Get the thread ID for the cursor owner.
    dwThreadID = GetWindowThreadProcessId(hWnd, NULL);

    // Get the thread ID for the current thread
    dwCurrentThreadID = GetCurrentThreadId();

    // If the cursor owner is not us then we must attach to
    // the other thread in so that we can use GetCursor() to
    // return the correct hCursor
    if (dwCurrentThreadID != dwThreadID) {

        // Attach to the thread that owns the cursor
        if (AttachThreadInput(dwCurrentThreadID, dwThreadID, TRUE)) {

            // Get the handle to the cursor
            hCursor = GetCursor();

            // Detach from the thread that owns the cursor
            AttachThreadInput(dwCurrentThreadID, dwThreadID, FALSE);
        }
    } else
        hCursor = GetCursor();

    return hCursor;
}
void RemoteDesktopDisplay::sendCursor()
{
    auto cursor_ = GetCurrentCursorHandle();
    if(mouse_ != nullptr && lastCursor_ != cursor_){
        mouse_->mouseCursorEvent(int(cursor_));
        lastCursor_ = cursor_;
    }
}

void RemoteDesktopDisplay::timeout()
{
}

void RemoteDesktopDisplay::onCursorEvent(int cursor)
{
    switch (cursor) {
    case 5:
        setCursor(QCursor(Qt::ArrowCursor));
        break;
    case 7:
        setCursor(QCursor(Qt::IBeamCursor));
        break;
    }
    qDebug()<<"cursor "<<cursor;

}

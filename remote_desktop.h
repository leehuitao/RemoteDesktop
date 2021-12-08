#ifndef REMOTEDESKTOP_H
#define REMOTEDESKTOP_H
#include <QObject>
#include "RemoteDesktop_global.h"
#include "LogModule/romote_log.h"
#include "peerconnection_manager.h"
#include "SignalServerModule/signal_server_manager.h"
#include "DisplayModule/remote_desktop.h"
namespace  remote{
class DataCallBack{
public:
    DataCallBack(){}
    virtual ~DataCallBack(){}
    virtual void OnInitPeerConnection(){}
    virtual void OnInitDesktopCapture(){}
    virtual void OnInitScreentopCapture(){}
    virtual void OnInitWebSocket(){}
    virtual void OnDataChannel(SingalingData){}
    virtual void OnRemoteImage(uint8_t * data, int w, int h) = 0;
    virtual void OnLocalDesktopImage(QImage image) = 0;
    virtual void OnLocalScreenImage(QImage image) = 0;
    virtual void OnLocalChooseDesktopImage(int sourceid,QString title,QImage image) = 0;
    virtual void OnLocalChoseScreenImage(int sourceid,QString title,QImage image) = 0;
};
/**
 * @brief 导出类,所有需要的接口都在这个文件
 */
class RemoteDesktop :public QObject
{
    Q_OBJECT
public:
    static RemoteDesktop* Instance(){
        return remoteDesktop_;
    }
    /**
     * @brief 初始化,传入回调类,包含需要显示的视频流
     */
    bool init(DataCallBack* callback);

    void startRemoteDesktop();

    void AddLocalTrack(bool isDesktopCapture,const int loaclTrackSourceid);

    void joinRoom(int roomid,int uid);

    void setLoaclShared(const QString & sourceid,const QString & sourcetitle);

    void captureOneImage();

    void sendDataChannel(SingalingData);

    void getScreenSourceList(QMap<int,QString>&  SourceMap);
    /**
     * @brief 初始化peerconnection
     * @param stun    打洞服务器地址
     * @param dtls 是否开启dtls
     */
    bool initPeerConnection(const QString &stun,bool dtls);
    /**
     * @brief 初始化日志系统
     * @param level 日志等级
     * @param logPath 日志路径
     * @param maxSizeBytes 每个日志最大字节数
     * @param maxOldLogCount 存储多少个历史文件
     */
    bool initLog(QsLogging::Level level = QsLogging::Level::ErrorLevel,const QString & logPath = "./",long long maxSizeBytes = 1024*512,int maxOldLogCount = 10);
    /**
     * @brief 初始化信号服务器连接
     * @param ip 信号服务器地址
     * @param port 信号服务器端口
     * @param uid 当前用户id
     */
    bool initWebSocket(QString ip,unsigned short port,int uid);

    /**
     * @brief 重置 peerconnection
     */
    void clearAll();

    /**
     * @brief 设置需要显示的UI(使用默认构造)
     */
    void setRemoteParent(QWidget* parent);

    /**
     * @brief 设置UI鼠标事件的回调(由外部提升后的控件设置鼠标事件通道)
     */
    void setDisplayDatachannel(RemoteDesktopDisplay * disp);
private:
    RemoteDesktop() = default;
    ~RemoteDesktop();
    /**
     * @brief 初始化应用截屏
     */
    bool initDesktopCapture();
    /**
     * @brief 初始化桌面截屏
     */
    bool initScreentopCapture();

Q_SIGNALS:
    //图片回调,跟上面的回调对应,实现其中一个就可以
    /**
     * @brief 收到远程流信号
     */
    void signRemoteImage(QImage);
    /**
     * @brief 本地应用截屏流信号
     */
    void signLocalDesktopImage(QImage);
    /**
     * @brief 本地桌面截屏流信号
     */
    void signScreenImage(QImage image);
    /**
     * @brief 本地应用缩略截屏流信号
     * @param sourceid 流id
     * @param title 流的名字
     */
    void signChooseDesktopImage(int sourceid,QString title,QImage image);
    /**
     * @brief 本地桌面缩略截屏流信号
     * @param sourceid 流id
     * @param title 流的名字
     */
    void signChoseScreenImage(int sourceid,QString title,QImage image);

    void signOnDataChannel(SingalingData);

    void signOnIceConnected();
private Q_SLOTS:
    void slotRemoteImage(QImage image);
    void slotLocalDesktopImage(QImage image);
    void slotScreenImage(QImage image);
    void slotChooseDesktopImage(int sourceid,QString title,QImage image);
    void slotChoseScreenImage(int sourceid,QString title,QImage image);
    void slotOnDataChannel(SingalingData);
    void slotOnIceConnected();

private:
    static RemoteDesktop *remoteDesktop_;
    DataCallBack* callback_ = nullptr;
    rtc::scoped_refptr<PeerconnectionManager> peerConnection = nullptr;
    RemoteDesktopDisplay *remoteDesktopDisplay= nullptr;
};

}

#endif // REMOTEDESKTOP_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SignalServerModule/signal_server_manager.h"
#include <QMimeData>
#include <QMouseEvent>
MainWindow::MainWindow(QString signserverip, QString signserverport, QString roomid,
                       QString userid, std::string turnserverip, std::string turnserverport,
                       std::string turnserverusername, std::string turnserverpassword, QString isAccusedEnd_, QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    serverip_(signserverip),
    serverport_(signserverport),
    userid_(userid),
    roomid_(roomid),
    isAccusedEnd(isAccusedEnd_.toInt())

{
    ui->setupUi(this);
    time = new QTimer(this);
    this->setMouseTracking(true);//设置窗体可响应 Mouse Move
    ui->centralwidget->setMouseTracking(1);
    //截图信号槽
    connect(time,&QTimer::timeout,this,&MainWindow::timeout);
    QMap<int, QString> SourceMap_;
    //setWindowFlags( Qt::FramelessWindowHint); //去除顶部
    //初始化日志
    Interface::Instance()->initLog();
    //初始化Interface相关
    Interface::Instance()->init();
    //连接turn服务器 创建数据通道
    Interface::Instance()->initPeerConnection(turnserverip,turnserverport,
                                              turnserverusername,turnserverpassword,0);
    //获取屏幕列表
    Interface::Instance()->getScreenSourceList(SourceMap_);
    //添加本地视频源
    Interface::Instance()->AddLocalTrack(0,0);
    //设置显示数据UI
    Interface::Instance()->setDisplayDatachannel(ui->centralwidget);
    //信号服务器连接 加入房间
    connect(SignalServerManager::Instance(),&SignalServerManager::signWebsocketConnected,this,&MainWindow::slotWebsocketConnected);
    //trun服务器连接成功回调
    connect(Interface::Instance(),&Interface::signOnIceConnected,this,&MainWindow::on_start_capture_clicked);
    on_join_clicked();
    //接受拖放文件
    this->setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnRemoteImage(uint8_t *data, int w, int h)
{
    //ui->widget->setData(data,w,h);
}

void MainWindow::OnLocalDesktopImage(QImage image)
{
    qDebug()<<__FUNCTION__;
}

void MainWindow::OnLocalScreenImage(QImage image)
{
    // ui->label_2->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::OnLocalChooseDesktopImage(int sourceid, QString title, QImage image)
{
    qDebug()<<__FUNCTION__;
}

void MainWindow::OnLocalChoseScreenImage(int sourceid, QString title, QImage image)
{
    qDebug()<<__FUNCTION__;
}

void MainWindow::OnDataChannel(SingalingData data)
{
    qDebug()<<"data"<<data.type<<data.uid<<data.data;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QString name = event->mimeData()->urls().first().toString();
    QString path = event->mimeData()->urls().first().path();

}


#define PAUSE "pause"
#define CONTINUE "continue"
#define JOINROOM "join"
void MainWindow::on_join_clicked()
{
    //加入
    if(ui->join->text() == JOINROOM){
        //设置是否是被控制
        ui->centralwidget->setIsAccuseEnd(isAccusedEnd);
        //设置预览大小
        DataControl::Instance()->setImageSize(ui->centralwidget->width(),ui->centralwidget->height());
        //连接webSocket(信号服务器)
        Interface::Instance()->initWebSocket(serverip_,serverport_.toInt(),userid_.toInt());
        setUishow(0);
        if(isAccusedEnd){
            ui->join->setText(PAUSE);
            //this->hide();
        }
        else{
            ui->centralwidget->setIsCursor(0);
            ui->join->hide();
        }
    //暂停
    }else if(ui->join->text() == PAUSE){
        time->stop();
        ui->join->setText(CONTINUE);
    }else{
        time->start(30);
        ui->join->setText(PAUSE);
    }
    
}

void MainWindow::timeout()
{
    Interface::Instance()->captureOneImage();
}

void MainWindow::slotWebsocketConnected()
{
    Interface::Instance()->joinRoom(roomid_.toInt(), userid_.toInt());
}

void MainWindow::setUishow(bool show)
{
    if(show){
        ui->roomid->show();
        ui->userid->show();
        ui->serverip->show();
        ui->serverport->show();
        ui->label->show();
        ui->label_2->show();
        ui->label_3->show();
        ui->label_4->show();
        ui->comboBox->show();
        ui->join->setText(PAUSE);
    }else{
        ui->roomid->hide();
        ui->userid->hide();
        ui->serverip->hide();
        ui->serverport->hide();
        ui->label->hide();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->comboBox->hide();
        ui->join->setText(CONTINUE);
    }
}

void MainWindow::on_init_clicked()
{
    Interface::Instance()->setLoaclShared(0,0);
}

void MainWindow::on_start_capture_clicked()
{
    Sleep(2);
    time->start(40);
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "interface.h"
#include <QTimer>
#include <QThread>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow,
        public DataCallBack
{
    Q_OBJECT

public:
    //explicit MainWindow(QWidget *parent = nullptr);
    MainWindow(QString signserverip,QString signserverport,QString roomid,QString userid,
               std::string turnserverip,std::string turnserverport,std::string turnserverusername,
               std::string turnserverpassword,QString isAccusedEnd,QWidget *parent = nullptr);
    ~MainWindow() override;
    //DataCallBack
    virtual void OnRemoteImage(uint8_t * data,int w ,int h) override;
    virtual void OnLocalDesktopImage(QImage image) override;
    virtual void OnLocalScreenImage(QImage image) override;
    virtual void OnLocalChooseDesktopImage(int sourceid,QString title,QImage image) override;
    virtual void OnLocalChoseScreenImage(int sourceid,QString title,QImage image) override;
    virtual void OnDataChannel(SingalingData) override;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
private Q_SLOTS:
    void on_start_capture_clicked();
    void on_init_clicked();
    void on_join_clicked();
    void timeout();
    void slotWebsocketConnected();
    void setUishow(bool show);
private:
    Ui::MainWindow *ui;
    QTimer *time;
    QString serverip_;
    QString serverport_;
    QString userid_;
    QString roomid_;
    //是否为被控端
    bool isAccusedEnd;
};

#endif // MAINWINDOW_H

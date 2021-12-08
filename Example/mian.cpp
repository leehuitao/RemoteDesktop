#include <QApplication>
#include "interface.h"
#include "mainwindow.h"
#include <QDebug>
#include <iostream>
#include <QString>
/**
 * @brief  main主体函数
 * @param 0 exe名称
 * @param 1 信号服务ip
 * @param 2 信号服务port
 * @param 3 roomid
 * @param 4 userid
 * @param 5 turnserverip
 * @param 6 turnserverport
 * @param 7 turnserverusername
 * @param 8 turnserverpassword
 * @param 8 isAccusedEnd  是否为被控端
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(argc != 10){
        std::cout<<"commond line error "<<std::endl;
        return 0;
    }
    QString processName             =argv[0];
    QString signserverip            =argv[1];
    QString signserverport          =argv[2];
    QString roomid                  =argv[3];
    QString userid                  =argv[4];
    std::string turnserverip            =argv[5];
    std::string turnserverport          =argv[6];
    std::string turnserverusername      =argv[7];
    std::string turnserverpassword      =argv[8];
    QString isAccusedEnd            =argv[9];
/*    std::cout<<"=================================="<<std::endl;
    for(int i =0 ;i<argc ;i++){
        std::cout<<"i = "<<i<<" value  = "<<argv[i]<<std::endl;
    }
    std::cout<<"=================================="<<std::endl;*/
    MainWindow w(signserverip,signserverport,roomid,userid,
                 turnserverip,turnserverport,turnserverusername,turnserverpassword,
                 isAccusedEnd);
    w.show();
    return a.exec();
}

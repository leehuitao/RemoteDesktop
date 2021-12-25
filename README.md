# RemoteDesktop

#### 介绍
webrtc  远程桌面控制

#### 软件架构
客户端：     webrtc+QT+VS2019
信号服务器： golang  客户端通讯方式 websocket
turn服务：  centos7 turnserver


#### 安装教程
# turnserver 
https://github.com/coturn/coturn

webrtc,的访问流程为

1.首先将自己注册到远程桌面库的接口里
2.初始化peerconnectionManager （无法正常使用构造函数构造 要使用静态函数返回rtc::scoped_refptr<PeerconnectionManager>）
3.初始化桌面截图和应用截图
4.构建PeerConnectionFactory和PeerConnection
注意：windows下是要在构造时声明使用win32socketserver
   初始化PeerConnectionFactory时要添加信号线程不然有些函数无法正常回调
   创建PeerConnection时webrtc::PeerConnectionInterface::RTCConfiguration配置文件中
enable_dtls_srtp为是否打开datachannel通道
servers为打洞服务器和中继服务配置
5.创建datachannel通道
6.设置本地源
7.初始化websocket（信号服务器连接）
8.加入房间
9.收到offer则设置远程流并生成answer返回
10.收到answer则设置远程流
11.本地会生成candidate需要发送到远程
12.这个时候本地和远程就通了

## 快速入门
```
tar -zxvf turnserver.tar.gz
cd turnserver/
cp  turnserver.conf   /etc/
./turnserver
```

默认开放 3478 端口
iceServer 设置
```
{
    iceServers: [
        {
            url: 'turn:192.168.0.151',
            username: 'user',
            credential: '123'
        }
    ]
}
```




#### 使用说明

客户端运行参数说明
1 信号服务ip                
2 信号服务port              
3 roomid                
4 userid                
5 turnserverip          
6 turnserverport        
7 turnserverusername    
8 turnserverpassword    
8 isAccusedEnd  是否为被控端  


*.exe 192.168.101.145 8083 10 10 192.168.101.183 3478 admin bigant.cn 0

#### 参与贡献

1.leehuitao


#### 学习使用


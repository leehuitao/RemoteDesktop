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

1. 尝试直连.
2. 通过stun服务器进行穿透
3. 无法穿透则通过turn服务器中转.


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

#### 参与贡献

1.leehuitao


#### 学习使用


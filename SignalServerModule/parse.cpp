#include "parse.h"

Parse::Parse()
{

}

void Parse::parseJson(const QString &data, SingalingData & signalingData)
{
    QJsonParseError jsonError;
    QJsonDocument parseData = QJsonDocument::fromJson(data.toLocal8Bit().data(), &jsonError);
    if (!parseData.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        QJsonObject object = parseData.object();
        QJsonValue status = object.value("status");  // 获取指定 key 对应的 value
        signalingData.stataus = status.toString();

        QJsonValue msg = object.value("msg");
        signalingData.msg = msg.toString();

        QJsonValue type = object.value("type");
        signalingData.type = type.toString();

        QJsonValue roomid = object.value("roomid");
        signalingData.roomid = roomid.toString();

        QJsonValue uid = object.value("uid");
        signalingData.uid = uid.toString();

        QJsonValue data = object.value("data");
        QJsonObject dataObj = data.toObject();


        if(signalingData.type == JOIN){
        }else if(signalingData.type == READY){
        }else if(signalingData.type == OFFER){
            QJsonValue sdp = dataObj.value("sdp");
            signalingData.data["sdp"] = sdp.toString();
        }else if(signalingData.type == ANSWER){
            QJsonValue sdp = dataObj.value("sdp");
            signalingData.data["sdp"] = sdp.toString();
        }else if(signalingData.type == CONDIDATA){
            QJsonValue sdp = dataObj.value("candidate");
            QJsonValue index = dataObj.value("sdpMLineIndex");
            QJsonValue sdp_mid = dataObj.value("sdpMid");
            qDebug()<<"dataObj"<<dataObj;
            signalingData.data["candidate"] = sdp.toString();
            signalingData.data["sdpMLineIndex"] = QString::number(index.toInt());
            signalingData.data["sdpMid"] = sdp_mid.toString();
        }else{
            qDebug() << "error type = " << signalingData.type;
        }
    }
}

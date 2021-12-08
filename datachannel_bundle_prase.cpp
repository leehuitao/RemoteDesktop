#include "datachannel_bundle_prase.h"

DataChannelBundlePrase::DataChannelBundlePrase()
{

}

void DataChannelBundlePrase::bundleRequest(SingalingData data, QString &ret)
{
    QJsonObject json;
    json.insert("type", data.type);
    json.insert("roomid", data.roomid);
    json.insert("uid", data.uid);
    QJsonObject dataJson;
    QMap<QString,QString>::iterator iter;
    for(iter =data.data.begin();iter!= data.data.end();iter++ ){
        dataJson.insert(iter.key(),iter.value());
    }
    json.insert("data", QJsonValue(dataJson));
    // 构建 JSON 文档
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);
    ret = strJson;
    //qDebug()<<"strJson = "<<strJson;
}

void DataChannelBundlePrase::bundleDataChannel(Event event, QString roomid, QString uid, QString &ret)
{
    QJsonObject json;
    json.insert("type", "channel_msg");
    json.insert("roomid", roomid);
    json.insert("uid", uid);
    QJsonObject dataJson;
    dataJson.insert("type",event.type);
    dataJson.insert("posx",event.posx);
    dataJson.insert("posy",event.posy);
    dataJson.insert("delta",event.delta);
    dataJson.insert("keyword",event.keyword);
    dataJson.insert("coursorShape",event.coursorShape);
    json.insert("data", QJsonValue(dataJson));
    // 构建 JSON 文档
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);
    ret = strJson;
    //qDebug()<<"strJson = "<<strJson;
}

void DataChannelBundlePrase::parseResponse(const QByteArray &data, SingalingData & signalingData)
{
    QJsonParseError jsonError;
    QJsonDocument parseData = QJsonDocument::fromJson(data, &jsonError);
    if (!parseData.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        QJsonObject object = parseData.object();

        QJsonValue type = object.value("type");
        signalingData.type = type.toString();

        QJsonValue roomid = object.value("roomid");
        signalingData.roomid = roomid.toString();

        QJsonValue uid = object.value("uid");
        signalingData.uid = uid.toString();

        QJsonValue data = object.value("data");
        QJsonObject dataObj = data.toObject();

        QStringList keys = dataObj.keys();
        qDebug()<<"keys " <<keys;
        for(auto iter : keys){
            QJsonValue value = dataObj.value(iter);
            signalingData.data[iter] = QString::number(value.toInt());
            qDebug()<<"key = "<<iter<<"value = "<<QString::number(value.toInt());
        }

    }else{
        qDebug()<<"jsonError.error"<<jsonError.error<<jsonError.errorString();
    }
}

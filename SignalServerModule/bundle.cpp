#include "bundle.h"

Bundle::Bundle()
{

}

void Bundle::bundleRequest(SingalingData data, QString &ret)
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
    qDebug()<<"strJson = "<<strJson;
}

#ifndef ROMOTELOG_H
#define ROMOTELOG_H
#include "QsLog.h"
#include "QsLogDest.h"
#include <QLibrary>
#include <QCoreApplication>
#include <QDir>
#include <iostream>

using namespace QsLogging;
//void logFunction(const QString &message, QsLogging::Level level)
//{
//    std::cout << "From log function: " << qPrintable(message) << " " << static_cast<int>(level)
//              << std::endl;
//}
class RomoteLog
{
public:
    static RomoteLog * Instance(){
        return remote;
    }
    bool initLog(QsLogging::Level level = QsLogging::Level::DebugLevel,const QString & logPath = "./",long long maxSizeBytes = 1024*512,int maxOldLogCount = 10);
private:
    RomoteLog() = default;

private:
    static RomoteLog *remote;
};

#endif // ROMOTELOG_H

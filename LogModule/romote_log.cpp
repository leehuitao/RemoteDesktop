#include "romote_log.h"

RomoteLog * RomoteLog::remote = new RomoteLog;

bool RomoteLog::initLog(Level level, const QString &logPath, long long maxSizeBytes, int maxOldLogCount)
{
    // 1. init the logging mechanism
    Logger& logger = Logger::instance();
    logger.setLoggingLevel(level);
    const QString sLogPath(logPath);

    // 2. add two destinations
    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(
      sLogPath, EnableLogRotation, MaxSizeBytes(maxSizeBytes), MaxOldLogCount(maxOldLogCount)));
    DestinationPtr debugDestination(DestinationFactory::MakeDebugOutputDestination());
    //DestinationPtr functorDestination(DestinationFactory::MakeFunctorDestination(&logFunction));
    logger.addDestination(debugDestination);
    logger.addDestination(fileDestination);
    //logger.addDestination(functorDestination);
    return  1;
}

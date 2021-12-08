#ifndef KEYWORDMANAGER_H
#define KEYWORDMANAGER_H

#include <QObject>
#include "DisplayModule/send_event_callback.h"
class KeywordManager : public QObject
{
    Q_OBJECT
public:
    explicit KeywordManager(SendEventCallBack*);
    void keyPressEvent(int keyword);
    void keyReleaseEvent(int keyword);

private:
    SendEventCallBack* callback_ = nullptr;
};

#endif // KEYWORDMANAGER_H

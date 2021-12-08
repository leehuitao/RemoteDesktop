#include "keyword_manager.h"
#include <QDataStream>
KeywordManager::KeywordManager(SendEventCallBack * callback):callback_(callback)
{

}

void KeywordManager::keyPressEvent(int keyword)
{
    Event mouseEvent;
    mouseEvent.type = KeywordDown;
    mouseEvent.keyword = keyword;
    callback_->SendKeywordEvent(mouseEvent);
}

void KeywordManager::keyReleaseEvent(int keyword)
{
    Event mouseEvent;
    mouseEvent.type = KeywordUp;
    mouseEvent.keyword = keyword;
    callback_->SendKeywordEvent(mouseEvent);
}

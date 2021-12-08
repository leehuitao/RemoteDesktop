#ifndef SIGNALSESSION_H
#define SIGNALSESSION_H
#include "signalsession_global.h"
#include <string>

class SIGNALSESSION_EXPORT SignalsessionObserver{
public:
    enum status{
        SIGSESSION_CONNECTING,
        SIGSESSION_CONNECTED,
        SIGSESSION_JIONSESSION,
        SIGSESSION_SUCCESS,
        SIGSESSION_FAILD
    };
    virtual void OnStatus(SignalsessionObserver::status s) = 0;
    virtual void OnUserData(std::string str) = 0;
};

class SIGNALSESSION_EXPORT Signalsession
{
public:
    Signalsession();
    Signalsession(SignalsessionObserver* observer, std::string ip, int port);
    ~Signalsession();
    void SetServer(std::string ip, int port);
    void SetObserver(SignalsessionObserver* observer);
    bool Start();
private:
    SignalsessionObserver*  m_pObserver;
    std::string             m_ip;
    int                     m_port;
};

#endif // SIGNALSESSION_H

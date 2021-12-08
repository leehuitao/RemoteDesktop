#ifndef PARSE_H
#define PARSE_H

#include <QString>
#include "SignalCallBack.h"
class Parse
{
public:
    Parse();
    void parseJson(const QString& data, SingalingData &);
};

#endif // PARSE_H

#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <QObject>

class DisplayManager : public QObject
{
    Q_OBJECT
public:
    explicit DisplayManager(QObject *parent = nullptr);

Q_SIGNALS:

public Q_SLOTS:
};

#endif // DISPLAYMANAGER_H

#ifndef KEYHOOK_H
#define KEYHOOK_H

#include <windows.h>
#include <QSettings>
#include <QDebug>

class KeyHook
{
public:
    KeyHook();
public slots:
    void setHook();
    void unHook();
    void setLock();
    void unlock();
};

#endif // KEYHOOK_H

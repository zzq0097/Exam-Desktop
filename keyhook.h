#ifndef KEYHOOK_H
#define KEYHOOK_H

#include <windows.h>
#include <QDebug>

class KeyHook
{
public:
    KeyHook();
public slots:
    void setHook();
    void unHook();
};

#endif // KEYHOOK_H

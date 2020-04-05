#ifndef KEYHOOK_H
#define KEYHOOK_H

#include <windows.h>
#include <Tlhelp32.h>
#include <QDebug>

class KeyHook
{
public:
    KeyHook();
public slots:
    void EnableDebugPriv();
    void Freeze();
    void unFreeze();
    void setHook();
    void unHook();
};

#endif // KEYHOOK_H

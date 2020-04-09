#ifndef USBMGR_H
#define USBMGR_H

#include <QSettings>
#include <QDebug>

class UsbMgr
{
public:
    UsbMgr();
public slots:
    void disableUSB();
    void enableUSB();
};

#endif // USBMGR_H

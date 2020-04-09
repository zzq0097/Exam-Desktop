#include "usbmgr.h"

UsbMgr::UsbMgr(){}

// 禁用USB
void UsbMgr::disableUSB()
{
    QSettings settings("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\USBSTOR",QSettings::NativeFormat);
    settings.setValue("start", 4);
    qDebug()<<settings.value("start");
}
// 恢复USB
void UsbMgr::enableUSB()
{
    QSettings settings("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\USBSTOR",QSettings::NativeFormat);
    settings.setValue("start", 3);
    qDebug()<<settings.value("start");
}

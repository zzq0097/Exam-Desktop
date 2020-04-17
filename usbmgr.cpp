#include "usbmgr.h"

UsbMgr::UsbMgr(){}

void UsbMgr::disableUSB() // 禁用USB
{
    QSettings settings("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\USBSTOR",QSettings::NativeFormat);
    settings.setValue("start", 4);
    qDebug()<<settings.value("start");
}

void UsbMgr::enableUSB() // 恢复USB
{
    QSettings settings("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\USBSTOR",QSettings::NativeFormat);
    settings.setValue("start", 3);
    qDebug()<<settings.value("start");
}

#include "keyhook.h"

KeyHook::KeyHook(){}

int globlePid = 0;
HHOOK keyHook = NULL;
HHOOK mouseHook = NULL;

LRESULT CALLBACK keyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *)lParam;
    if (pkbhs->vkCode == VK_ESCAPE && GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000){
        qDebug() << "Ctrl+Shift+Esc";
        return 1;
    }
    else if (pkbhs->vkCode == VK_ESCAPE && GetAsyncKeyState(VK_CONTROL) & 0x8000){
        qDebug() << "Ctrl+Esc";
        return 1;
    }
    else if (pkbhs->vkCode == VK_TAB && pkbhs->flags & LLKHF_ALTDOWN){
        qDebug() << "Alt+Tab";
        return 1;
    }
    else if (pkbhs->vkCode == VK_ESCAPE && pkbhs->flags &LLKHF_ALTDOWN){
        qDebug() << "Alt+Esc";
        return 1;
    }
    else if (pkbhs->vkCode == VK_LWIN || pkbhs->vkCode == VK_RWIN){
        qDebug() << "LWIN/RWIN";
        return 1;
    }
    else if (pkbhs->vkCode == VK_F4 && pkbhs->flags & LLKHF_ALTDOWN){
        qDebug() << "Alt+F4";
        return 1;
    }
    return CallNextHookEx(keyHook, nCode, wParam, lParam);
}

// 安装键盘钩子
void KeyHook::setHook()
{
    keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyProc, GetModuleHandle(NULL), 0);
}

// 卸载键盘钩子
void KeyHook::unHook()
{
    UnhookWindowsHookEx(keyHook);
}
// 禁用任务管理器
void KeyHook::setLock()
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",QSettings::NativeFormat);
    settings.setValue("DisableTaskMgr", 1);
    qDebug()<<settings.value("DisableTaskMgr");
}
// 解除禁用任务管理器
void KeyHook::unlock()
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",QSettings::NativeFormat);
    settings.setValue("DisableTaskMgr", 0);
    qDebug()<<settings.value("DisableTaskMgr");
}

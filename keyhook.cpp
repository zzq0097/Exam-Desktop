#include "keyhook.h"

KeyHook::KeyHook()
{

}

int globlePid = 0;
HHOOK keyHook = NULL;
HHOOK mouseHook = NULL;
//键盘钩子过程
LRESULT CALLBACK keyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    //在WH_KEYBOARD_LL模式下lParam 是指向KBDLLHOOKSTRUCT类型地址
    KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *)lParam;
    //如果nCode等于HC_ACTION则处理该消息，如果小于0，则钩子子程就必须将该消息传递给 CallNextHookEx
    //if (nCode == HC_ACTION){
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
    else if (pkbhs->vkCode == VK_DELETE && GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetAsyncKeyState(VK_MENU) & 0x8000){
        qDebug() << "Ctrl+Alt+Del";
        return 1;
    }

    //return 1;//返回1表示截取消息不再传递,返回0表示不作处理,消息继续传递
    //}
    return CallNextHookEx(keyHook, nCode, wParam, lParam);
}

//鼠标钩子过程
//LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam)
//{
//    return 1;
//}

//卸载钩子
void KeyHook::unHook()
{
    UnhookWindowsHookEx(keyHook);
    //  UnhookWindowsHookEx(mouseHook);
}

//安装钩子,调用该函数即安装钩子
void KeyHook::setHook()
{
    //这两个底层钩子,不要DLL就可以全局
    //底层键盘钩子
    keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyProc, GetModuleHandle(NULL), 0);
    //底层鼠标钩子
    //    mouseHook =SetWindowsHookEx( WH_MOUSE_LL,mouseProc,GetModuleHandle(NULL),0);
}

//提升权限
void KeyHook::EnableDebugPriv()
{
    HANDLE hToken;
    LUID sedebugnameValue;
    TOKEN_PRIVILEGES tkp;

    if (!OpenProcessToken(GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        return;
    }
    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue)) {
        CloseHandle(hToken);
        return;
    }
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = sedebugnameValue;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL)) {
        CloseHandle(hToken);
        return;
    }
}

char* ConvertLPWSTRToLPSTR(LPWSTR lpwszStrIn)
{
    LPSTR pszOut = NULL;
    if (lpwszStrIn != NULL)
    {
        int nInputStrLen = wcslen(lpwszStrIn);

        // Double NULL Termination
        int nOutputStrLen = WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, NULL, 0, 0, 0) + 2;
        pszOut = new char[nOutputStrLen];

        if (pszOut)
        {
            memset(pszOut, 0x00, nOutputStrLen);
            WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, pszOut, nOutputStrLen, 0, 0);
        }
    }
    return pszOut;
}

//冻结
void KeyHook::Freeze()
{
    //枚举进程信息
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);
    HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    int processPid;
    //CString strTmp;
    BOOL b = ::Process32First(hProcessSnap, &pe32);
    while (b)
    {
        processPid = pe32.th32ProcessID;
        char *exeFile = ConvertLPWSTRToLPSTR(pe32.szExeFile);
        if (strcmp(exeFile, "winlogon.exe") == 0)
        {
            break;
        }

        delete[] exeFile;

        b = ::Process32Next(hProcessSnap, &pe32);
    }
    ::CloseHandle(hProcessSnap);

    THREADENTRY32 th32;
    th32.dwSize = sizeof(th32);
    HANDLE hThreadSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    globlePid = processPid;
    unsigned long Pid;
    Pid = processPid;
    b = ::Thread32First(hThreadSnap, &th32);
    while (b)
    {
        if (th32.th32OwnerProcessID == Pid)
        {
            HANDLE oth = OpenThread(THREAD_ALL_ACCESS, FALSE, th32.th32ThreadID);
            if (!(::SuspendThread(oth)))
            {
                qDebug() << "freeze successed";
            }
            else
            {
                qDebug() << "freeze failed";
            }
            CloseHandle(oth);
            break;
        }
        ::Thread32Next(hThreadSnap, &th32);
    }
    ::CloseHandle(hThreadSnap);
}

void KeyHook::unFreeze()
{
    unsigned long Pid;
    Pid = globlePid;

    THREADENTRY32 th32;
    th32.dwSize = sizeof(th32);

    HANDLE hThreadSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    BOOL b = ::Thread32First(hThreadSnap, &th32);
    while (b)
    {
        if (th32.th32OwnerProcessID == Pid)
        {
            HANDLE oth = OpenThread(THREAD_ALL_ACCESS, FALSE, th32.th32ThreadID);
            if (::ResumeThread(oth))
            {
                qDebug() << "unfreeze successed";
            }
            else
            {
                qDebug() << "unfreeze failed";
            }
            CloseHandle(oth);
            break;
        }
        ::Thread32Next(hThreadSnap, &th32);
    }
    ::CloseHandle(hThreadSnap);
}

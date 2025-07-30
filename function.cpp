#include "framework.h"


////////////////////////////////////
// 
// システムトレイに格納する処理
// 

extern  BOOL __stdcall SetSystemTray(NOTIFYICONDATA& date,HWND hWnd,DWORD& errorcod) {


    std::wstring TryIconText = L"SetInitializationPC";

    date.cbSize = NOTIFYICONDATA_V3_SIZE;
    date.hWnd = hWnd;
    date.uID = ID_TRAY_APP_ICON;
    date.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    date.uCallbackMessage = WM_TRAYICON;
    date.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcscpy_s(date.szTip, L"SetInitializationPC");

    SetLastError(ERROR_SUCCESS);

    BOOL result = Shell_NotifyIcon(NIM_ADD, &date);

    errorcod = GetLastError();

    return result;
}
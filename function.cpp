#include "framework.h"


////////////////////////////////////////////////////////////////////////////
// 
// システムトレイに格納する処理
//

extern  BOOL __stdcall SetSystemTray(NOTIFYICONDATA& date, HWND hWnd, DWORD& errorcod) {


    std::wstring TryIconText = FILENAME;

    date.cbSize = NOTIFYICONDATA_V3_SIZE;
    date.hWnd = hWnd;
    date.uID = ID_TRAY_APP_ICON;
    date.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    date.uCallbackMessage = WM_TRAYICON;
    date.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcscpy_s(date.szTip, FILENAME);

    SetLastError(ERROR_SUCCESS);

    BOOL result = Shell_NotifyIcon(NIM_ADD, &date);

    errorcod = GetLastError();

    return result;
};


////////////////////////////////////////////////////////////////////////////
// 
// フォルダーやファイル名を追加する関数
//

extern  std::wstring __stdcall Set_Folder_File_Name(std::wstring doubleslash,std::wstring indrc,std::wstring source) {

    std::wstring result = source;

    result.append(doubleslash);
    result.append(indrc);

    return result;
};


////////////////////////////////////////////////////////////////////////////
// 
// 実行ファイルをコピーする関数
//

extern  void __stdcall CopyFile_EXE(std::wstring copydrc,std::wstring& error_str,std::error_code& error) {

    wchar_t ExeFilePath[MAX_PATH];
    std::wstring Path = copydrc + DOUBLESLASH + FILENAME;

    if (GetModuleFileNameW(nullptr, ExeFilePath, MAX_PATH) == 0) {

        error_str = FILE_ERROR_UNKNOWN;
    }
    else {

        if (!std::filesystem::copy_file(ExeFilePath, Path, error)) {

            error_str = FILE_ERROR_UNKNOWN;
        }
    }
};
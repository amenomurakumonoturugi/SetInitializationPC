
////////////////////////////////////////////////////////////////////////////
//
// インクルードファイル
//

#include "framework.h"

bool FirstHideWindow;
std::wstring ExeFileDrc;
std::wstring FileError;


////////////////////////////////////////////////////////////////////////////
//
// システム変数
//

HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名


////////////////////////////////////////////////////////////////////////////
//
// システム関数
//

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


////////////////////////////////////////////////////////////////////////////
//
// main関数
//

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。


    ////////////////////////////////////////////////////////////////////////////
    // 
    //  デバッグモードの判定
    //

#ifdef DEBUG


#else // RELEASE

#define RELEASE

#endif 


    ////////////////////////////////////////////////////////////////////////////
    // 
    //  初期化処理
    //

    FirstHideWindow = false;
    ExeFileDrc = NULLSTR;
    FileError = NULLSTR;
    NeedFileCheck.MemberInitialization();


    ////////////////////////////////////////////////////////////////////////////
    // 
    // 実行ファイルのユーザー名を取得
    //
    
    TCHAR UserName[UNLEN + 1] = {};

    DWORD Length = UNLEN + 1;
    LPDWORD UserNameLength = &Length;

    BOOL IfGetUserName = GetUserName(UserName, UserNameLength);

    if (IfGetUserName) {

        ExeFileDrc = USERDIRECTORY;
        ExeFileDrc.append(DOUBLESLASH);


        ////////////////////////////////////////////////////////////////////////////
        // 
        // 指定のディレクトリを判定
        //

        int count = static_cast<int>(*UserNameLength);

        for (int i = 0; i < (count - 1); i++)
            ExeFileDrc.append(1, UserName[i]);

        ExeFileDrc = Set_Folder_File_Name(DOUBLESLASH, FIRSTFOLDER, ExeFileDrc);
        NeedFileCheck.SetFolderName(ExeFileDrc);


        ////////////////////////////////////////////////////////////////////////////
        // 
        // 最初のディレクトリを判定
        //

        if (NeedFileCheck.FolderExists()) {

            NeedFileCheck.MemberInitialization();

            ExeFileDrc = Set_Folder_File_Name(DOUBLESLASH, SECONDFOLDER, ExeFileDrc);
            NeedFileCheck.SetFolderName(ExeFileDrc);


            ////////////////////////////////////////////////////////////////////////////
            // 
            // 2番目のディレクトリを判定
            //

            if (NeedFileCheck.FolderExists()) {

                NeedFileCheck.MemberInitialization();
                ExeFileDrc = Set_Folder_File_Name(DOUBLESLASH, FILENAME, ExeFileDrc);


                ////////////////////////////////////////////////////////////////////////////
                // 
                // ファイル名のディレクトリを判定
                //

                if (NeedFileCheck.FileExists()) {


                    ////////////////////////////////////////////////////////////////////////////
                    // 
                    // 実行ファイルをコピー
                    //

                    std::error_code Error;

                    CopyFile_EXE(ExeFileDrc, FileError, Error);
                }
            }
            else {

                std::error_code ErrorCode = NeedFileCheck.GetErrorCode();


                ////////////////////////////////////////////////////////////////////////////
                // 
                // フォルダーがない場合
                //

                if (ErrorCode.default_error_condition() == std::errc::no_such_file_or_directory ||
                    ErrorCode == std::error_condition()) {

                    std::error_code Error;

                    if (std::filesystem::create_directory(ExeFileDrc, Error)) {


                        ////////////////////////////////////////////////////////////////////////////
                        // 
                        // 実行ファイルをコピー
                        //

                        CopyFile_EXE(ExeFileDrc, FileError, Error);
                    }
                    else {


                        ////////////////////////////////////////////////////////////////////////////
                        // 
                        // アクセス権限がない場合
                        //

                        if (Error.default_error_condition() == std::errc::permission_denied) {

                            FileError = FILE_ERROR_ACCESS_FAILED;
                        }


                        ////////////////////////////////////////////////////////////////////////////
                        // 
                        // デバイスが存在しない場合
                        //

                        if (Error.default_error_condition() == std::errc::no_such_device_or_address) {

                            FileError = FILE_ERROR_DEVICE_NOT_FAUND;
                        }


                        ////////////////////////////////////////////////////////////////////////////
                        // 
                        // その他の場合
                        //

                        if (Error.default_error_condition() != std::errc::permission_denied &&
                            Error.default_error_condition() != std::errc::no_such_device_or_address) {

                            FileError = FILE_ERROR_UNKNOWN;
                        }
                    }
                }
                else {


                    ////////////////////////////////////////////////////////////////////////////
                    // 
                    // アクセス権限がない場合
                    //

                    if (ErrorCode.default_error_condition() == std::errc::permission_denied) {

                        FileError = FILE_ERROR_ACCESS_FAILED;
                    }

                    ////////////////////////////////////////////////////////////////////////////
                    // 
                    // デバイスが存在しない場合
                    //

                    if (ErrorCode.default_error_condition() == std::errc::no_such_device_or_address) {

                        FileError = FILE_ERROR_DEVICE_NOT_FAUND;
                    }


                    ////////////////////////////////////////////////////////////////////////////
                    // 
                    // その他の場合
                    //

                    if (ErrorCode.default_error_condition() != std::errc::permission_denied &&
                        ErrorCode.default_error_condition() != std::errc::no_such_device_or_address) {

                        FileError = FILE_ERROR_UNKNOWN;
                    }
                }
            }
        }
        else {

            std::error_code ErrorCode = NeedFileCheck.GetErrorCode();


            ////////////////////////////////////////////////////////////////////////////
            // 
            // フォルダーがない場合
            //

            if (ErrorCode.default_error_condition() == std::errc::no_such_file_or_directory ||
                ErrorCode == std::error_condition()) {

                std::error_code Error;
                ExeFileDrc = Set_Folder_File_Name(DOUBLESLASH, SECONDFOLDER, ExeFileDrc);

                if (std::filesystem::create_directories(ExeFileDrc, Error)) {


                    ////////////////////////////////////////////////////////////////////////////
                    // 
                    // 実行ファイルをコピー
                    //

                    CopyFile_EXE(ExeFileDrc, FileError, Error);
                }
                else {


                    ////////////////////////////////////////////////////////////////////////////
                    // 
                    // アクセス権限がない場合
                    //

                    if (Error.default_error_condition() == std::errc::permission_denied) {

                        FileError = FILE_ERROR_ACCESS_FAILED;
                    }


                    ////////////////////////////////////////////////////////////////////////////
                    // 
                    // デバイスが存在しない場合
                    //

                    if (Error.default_error_condition() == std::errc::no_such_device_or_address) {

                        FileError = FILE_ERROR_DEVICE_NOT_FAUND;
                    }


                    ////////////////////////////////////////////////////////////////////////////
                    // 
                    // その他の場合
                    //

                    if (Error.default_error_condition() != std::errc::permission_denied &&
                        Error.default_error_condition() != std::errc::no_such_device_or_address) {

                        FileError = FILE_ERROR_UNKNOWN;
                    }
                }
            }
            else {


                ////////////////////////////////////////////////////////////////////////////
                // 
                // アクセス権限がない場合
                //

                if (ErrorCode.default_error_condition() == std::errc::permission_denied) {

                    FileError = FILE_ERROR_ACCESS_FAILED;
                }

                ////////////////////////////////////////////////////////////////////////////
                // 
                // デバイスが存在しない場合
                //

                if (ErrorCode.default_error_condition() == std::errc::no_such_device_or_address) {

                    FileError = FILE_ERROR_DEVICE_NOT_FAUND;
                }


                ////////////////////////////////////////////////////////////////////////////
                // 
                // その他の場合
                //

                if (ErrorCode.default_error_condition() != std::errc::permission_denied &&
                    ErrorCode.default_error_condition() != std::errc::no_such_device_or_address) {

                    FileError = FILE_ERROR_UNKNOWN;
                }
            }
        }
    }
    else
        return 0;


    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SETINITIALIZATIONPC, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SETINITIALIZATIONPC));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



////////////////////////////////////////////////////////////////////////////
//
// 関数: MyRegisterClass()
// 目的: ウィンドウ クラスを登録します。
//

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SETINITIALIZATIONPC);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


////////////////////////////////////////////////////////////////////////////
//
// 関数: InitInstance(HINSTANCE, int)
// 目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
// コメント:
// 
//      この関数で、グローバル変数でインスタンス ハンドルを保存し、
//      メイン プログラム ウィンドウを作成および表示します。
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, 0, WINDOWWIDTH, WINDOWHEIGHT, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}


////////////////////////////////////////////////////////////////////////////
//
// 関数: WndProc(HWND, UINT, WPARAM, LPARAM)
// 目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    ////////////////////////////////////////////////////////////////////////////
    //
    // システムトレイ用の構造体を宣言と初期化
    //

    static NOTIFYICONDATA SystemTrayIcon = {};

    ////////////////////////////////////////////////////////////////////////////
    //
    // WndProcのswitch
    //

    switch (message)
    {
    case WM_CREATE:
    {
        DWORD ErrorCode = ERROR_SUCCESS;
        BOOL IfInTray = SetSystemTray(SystemTrayIcon,hWnd,ErrorCode);

        FirstHideWindow = true;

        ////////////////////////////////////
        //
        // WM_TIMERを起動
        //

        SetTimer(hWnd, TIMER_ID, TIMERMLLISEC, nullptr);
    }
    break;
    case WM_TIMER:
    {
        if (FirstHideWindow) {

            ShowWindow(hWnd, SW_HIDE);
            FirstHideWindow = false;
        }
    }
    break;
    case WM_SYSCOMMAND:
    {

        if (wParam == SC_MINIMIZE) {

            DWORD ErrorCode = ERROR_SUCCESS;
            BOOL IfInTray = SetSystemTray(SystemTrayIcon,hWnd, ErrorCode);

            ShowWindow(hWnd, SW_HIDE);
            return 0;
        }
        else
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    break;
    case WM_TRAYICON:
    {

        if (lParam == WM_LBUTTONUP) {
            ShowWindow(hWnd, SW_SHOW);
            ShowWindow(hWnd, SW_RESTORE);

            Shell_NotifyIcon(NIM_DELETE, &SystemTrayIcon);
        }
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 選択されたメニューの解析:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_CLOSE:
    {
        //ウィンドウを終了する
        DestroyWindow(hWnd);
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: HDC を使用する描画コードをここに追加してください...

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
    {

        Shell_NotifyIcon(NIM_DELETE, &SystemTrayIcon);

        PostQuitMessage(0);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////
//
// バージョン情報ボックスのメッセージ ハンドラーです。
//

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


////////////////////////////////////////////////////////////////////////////
//
// �C���N���[�h�t�@�C��
//

#include "framework.h"

bool FirstHideWindow;
std::wstring ExeFileDrc;
std::wstring FileError;


////////////////////////////////////////////////////////////////////////////
//
// �V�X�e���ϐ�
//

HINSTANCE hInst;                                // ���݂̃C���^�[�t�F�C�X
WCHAR szTitle[MAX_LOADSTRING];                  // �^�C�g�� �o�[�̃e�L�X�g
WCHAR szWindowClass[MAX_LOADSTRING];            // ���C�� �E�B���h�E �N���X��


////////////////////////////////////////////////////////////////////////////
//
// �V�X�e���֐�
//

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


////////////////////////////////////////////////////////////////////////////
//
// main�֐�
//

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �����ɃR�[�h��}�����Ă��������B


    ////////////////////////////////////////////////////////////////////////////
    // 
    //  �f�o�b�O���[�h�̔���
    //

#ifdef DEBUG


#else // RELEASE

#define RELEASE

#endif 


    ////////////////////////////////////////////////////////////////////////////
    // 
    //  ����������
    //

    FirstHideWindow = false;
    ExeFileDrc = NULLSTR;
    FileError = NULLSTR;
    NeedFileCheck.MemberInitialization();


    ////////////////////////////////////////////////////////////////////////////
    // 
    // ���s�t�@�C���̃��[�U�[�����擾
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
        // �w��̃f�B���N�g���𔻒�
        //

        int count = static_cast<int>(*UserNameLength);

        for (int i = 0; i < (count - 1); i++)
            ExeFileDrc.append(1, UserName[i]);

        ExeFileDrc = Set_Folder_File_Name(DOUBLESLASH, FIRSTFOLDER, ExeFileDrc);
        NeedFileCheck.SetFolderName(ExeFileDrc);


        ////////////////////////////////////////////////////////////////////////////
        // 
        // �ŏ��̃f�B���N�g���𔻒�
        //

        if (NeedFileCheck.FolderExists()) {

            NeedFileCheck.MemberInitialization();

            ExeFileDrc = Set_Folder_File_Name(DOUBLESLASH, SECONDFOLDER, ExeFileDrc);
            NeedFileCheck.SetFolderName(ExeFileDrc);


            ////////////////////////////////////////////////////////////////////////////
            // 
            // 2�Ԗڂ̃f�B���N�g���𔻒�
            //

            if (NeedFileCheck.FolderExists()) {

                NeedFileCheck.MemberInitialization();
                ExeFileDrc = Set_Folder_File_Name(DOUBLESLASH, FILENAME, ExeFileDrc);


                ////////////////////////////////////////////////////////////////////////////
                // 
                // �t�@�C�����̃f�B���N�g���𔻒�
                //

                if (NeedFileCheck.FileExists()) {


                    ////////////////////////////////////////////////////////////////////////////
                    // 
                    // ���s�t�@�C�����R�s�[
                    //

                    std::error_code Error;

                    CopyFile_EXE(ExeFileDrc, FileError, Error);
                }
            }
            else {

                std::error_code ErrorCode = NeedFileCheck.GetErrorCode();


                ////////////////////////////////////////////////////////////////////////////
                // 
                // �t�H���_�[���Ȃ��ꍇ
                //

                if (ErrorCode.default_error_condition() == std::errc::no_such_file_or_directory ||
                    ErrorCode == std::error_condition()) {

                    std::error_code Error;

                    if (std::filesystem::create_directory(ExeFileDrc, Error)) {


                        ////////////////////////////////////////////////////////////////////////////
                        // 
                        // ���s�t�@�C�����R�s�[
                        //

                        CopyFile_EXE(ExeFileDrc, FileError, Error);
                    }
                    else {


                        ////////////////////////////////////////////////////////////////////////////
                        // 
                        // �A�N�Z�X�������Ȃ��ꍇ
                        //

                        if (Error.default_error_condition() == std::errc::permission_denied) {

                            FileError = FILE_ERROR_ACCESS_FAILED;
                        }


                        ////////////////////////////////////////////////////////////////////////////
                        // 
                        // �f�o�C�X�����݂��Ȃ��ꍇ
                        //

                        if (Error.default_error_condition() == std::errc::no_such_device_or_address) {

                            FileError = FILE_ERROR_DEVICE_NOT_FAUND;
                        }


                        ////////////////////////////////////////////////////////////////////////////
                        // 
                        // ���̑��̏ꍇ
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
                    // �A�N�Z�X�������Ȃ��ꍇ
                    //

                    if (ErrorCode.default_error_condition() == std::errc::permission_denied) {

                        FileError = FILE_ERROR_ACCESS_FAILED;
                    }

                    ////////////////////////////////////////////////////////////////////////////
                    // 
                    // �f�o�C�X�����݂��Ȃ��ꍇ
                    //

                    if (ErrorCode.default_error_condition() == std::errc::no_such_device_or_address) {

                        FileError = FILE_ERROR_DEVICE_NOT_FAUND;
                    }


                    ////////////////////////////////////////////////////////////////////////////
                    // 
                    // ���̑��̏ꍇ
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
            // �t�H���_�[���Ȃ��ꍇ
            //

            if (ErrorCode.default_error_condition() == std::errc::no_such_file_or_directory ||
                ErrorCode == std::error_condition()) {

                std::error_code Error;
                ExeFileDrc = Set_Folder_File_Name(DOUBLESLASH, SECONDFOLDER, ExeFileDrc);

                if (std::filesystem::create_directories(ExeFileDrc, Error)) {


                    ////////////////////////////////////////////////////////////////////////////
                    // 
                    // ���s�t�@�C�����R�s�[
                    //

                    CopyFile_EXE(ExeFileDrc, FileError, Error);
                }
                else {


                    ////////////////////////////////////////////////////////////////////////////
                    // 
                    // �A�N�Z�X�������Ȃ��ꍇ
                    //

                    if (Error.default_error_condition() == std::errc::permission_denied) {

                        FileError = FILE_ERROR_ACCESS_FAILED;
                    }


                    ////////////////////////////////////////////////////////////////////////////
                    // 
                    // �f�o�C�X�����݂��Ȃ��ꍇ
                    //

                    if (Error.default_error_condition() == std::errc::no_such_device_or_address) {

                        FileError = FILE_ERROR_DEVICE_NOT_FAUND;
                    }


                    ////////////////////////////////////////////////////////////////////////////
                    // 
                    // ���̑��̏ꍇ
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
                // �A�N�Z�X�������Ȃ��ꍇ
                //

                if (ErrorCode.default_error_condition() == std::errc::permission_denied) {

                    FileError = FILE_ERROR_ACCESS_FAILED;
                }

                ////////////////////////////////////////////////////////////////////////////
                // 
                // �f�o�C�X�����݂��Ȃ��ꍇ
                //

                if (ErrorCode.default_error_condition() == std::errc::no_such_device_or_address) {

                    FileError = FILE_ERROR_DEVICE_NOT_FAUND;
                }


                ////////////////////////////////////////////////////////////////////////////
                // 
                // ���̑��̏ꍇ
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


    // �O���[�o�������������������
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SETINITIALIZATIONPC, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // �A�v���P�[�V�����������̎��s:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SETINITIALIZATIONPC));

    MSG msg;

    // ���C�� ���b�Z�[�W ���[�v:
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
// �֐�: MyRegisterClass()
// �ړI: �E�B���h�E �N���X��o�^���܂��B
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
// �֐�: InitInstance(HINSTANCE, int)
// �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂�
// �R�����g:
// 
//      ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//      ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X �n���h�����i�[����

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
// �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
// �ړI: ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND  - �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT    - ���C�� �E�B���h�E��`�悷��
//  WM_DESTROY  - ���~���b�Z�[�W��\�����Ė߂�
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    ////////////////////////////////////////////////////////////////////////////
    //
    // �V�X�e���g���C�p�̍\���̂�錾�Ə�����
    //

    static NOTIFYICONDATA SystemTrayIcon = {};

    ////////////////////////////////////////////////////////////////////////////
    //
    // WndProc��switch
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
        // WM_TIMER���N��
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
        // �I�����ꂽ���j���[�̉��:
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
        //�E�B���h�E���I������
        DestroyWindow(hWnd);
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: HDC ���g�p����`��R�[�h�������ɒǉ����Ă�������...

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
// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���[�ł��B
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

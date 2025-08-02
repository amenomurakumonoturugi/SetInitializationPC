#pragma once

#include "framework.h"

extern  BOOL __stdcall SetSystemTray(NOTIFYICONDATA& date, HWND hWnd, DWORD& errorcod);
extern  std::wstring __stdcall Set_Folder_File_Name(std::wstring doubleslash, std::wstring indrc, std::wstring source);
extern  void __stdcall CopyFile_EXE(std::wstring copydrc, std::wstring& error_str, std::error_code& error);
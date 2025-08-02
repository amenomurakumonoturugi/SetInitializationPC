#pragma once

#include "framework.h"

class NEEDFILECHECK {

private:

	std::wstring FileName;
	std::wstring FolderName;
	std::error_code ErrorCode;

public:

	void MemberInitialization() {

		FileName = NULLSTR;
		FolderName = NULLSTR;
		ErrorCode.clear();
	};

	void SetFileName(std::wstring in) {

		FileName = in;
	};

	void SetFolderName(std::wstring in) {

		FolderName = in;
	};

	bool FileExists() {

		return std::filesystem::exists(FileName, ErrorCode);
	};

	bool FolderExists() {

		return std::filesystem::exists(FolderName, ErrorCode);
	};

	std::error_code GetErrorCode() const {

		return ErrorCode;
	};
};

extern NEEDFILECHECK NeedFileCheck;

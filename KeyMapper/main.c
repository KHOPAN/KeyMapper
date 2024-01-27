#include <stdio.h>
#include <Windows.h>
#include "definition.h"

int main(int argc, char** argv) {
	HANDLE configFile = CreateFileW(L"keymapper.config.json", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(configFile == INVALID_HANDLE_VALUE) {
		DWORD error = GetLastError();

		if(error == ERROR_FILE_NOT_FOUND) {
			MessageBoxW(NULL, L"File 'keymapper.config.json' not found!", L"Error", MB_OK | MB_DEFBUTTON1 | MB_ICONERROR | MB_SYSTEMMODAL);
			return error;
		}

		DisplayError(error, L"CreateFileW()");
		return error;
	}

	LARGE_INTEGER fileSize;

	if(!GetFileSizeEx(configFile, &fileSize)) {
		DWORD error = GetLastError();
		DisplayError(error, L"GetFileSizeEx()");
		return error;
	}

	BYTE* buffer = malloc((size_t) (fileSize.QuadPart + 1));

	if(!buffer) {
		DisplayError(ERROR_NOT_ENOUGH_MEMORY, L"malloc()");
		return ERROR_NOT_ENOUGH_MEMORY;
	}

	DWORD read = 0;

	if(!ReadFile(configFile, buffer, (DWORD) fileSize.QuadPart, &read, NULL)) {
		DWORD error = GetLastError();
		DisplayError(error, L"ReadFile()");
		return error;
	}

	CloseHandle(configFile);
	buffer[(DWORD) fileSize.QuadPart] = 0;
	ParseJSON(buffer);
	return 0;
}

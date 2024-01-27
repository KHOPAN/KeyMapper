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

	CloseHandle(configFile);
	return 0;
}

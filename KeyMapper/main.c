#include <stdio.h>
#include <Windows.h>
#include "definition.h"

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC ((USHORT) 0x01)
#endif

#ifndef HID_USAGE_GENERIC_KEYBOARD
#define HID_USAGE_GENERIC_KEYBOARD ((USHORT) 0x06)
#endif

int main(int argc, char** argv) {
	HANDLE configFile = CreateFileW(L"D:\\GitHub Repository\\KeyMapper\\Test\\keymapper.config.json", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(configFile == INVALID_HANDLE_VALUE) {
		DWORD error = GetLastError();

		if(error == ERROR_FILE_NOT_FOUND) {
			MessageBoxW(NULL, L"File 'keymapper.config.json' not found!", L"KeyMapper File Error", KEYMAPPER_ERROR);
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

	if(ParseJSON(buffer)) {
		return -1;
	}

	HINSTANCE instance = GetModuleHandleW(NULL);
	WNDCLASSW windowClass = {0};
	windowClass.hInstance = instance;
	windowClass.lpszClassName = L"KeyMapper";
	windowClass.lpfnWndProc = WindowProcedure;

	if(!RegisterClassW(&windowClass)) {
		MessageBoxW(NULL, L"Register window class has failed!", L"KeyMapper Win32 Error", KEYMAPPER_ERROR);
		return -1;
	}

	HWND window = CreateWindowExW(0L, L"KeyMapper", L"KeyMapper", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
	RAWINPUTDEVICE devices[1];
	devices[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	devices[0].usUsage = HID_USAGE_GENERIC_KEYBOARD;
	devices[0].dwFlags = RIDEV_INPUTSINK;
	devices[0].hwndTarget = window;
	RegisterRawInputDevices(devices, 1, sizeof(devices[0]));
	MSG message = {0};

	while(GetMessageW(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}

	FreeMappingData();
	return 0;
}

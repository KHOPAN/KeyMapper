#include <stdio.h>
#include <dwmapi.h>
#include "definition.h"

#define CLASS_NAME L"KeyGetterClass"
#define HID_USAGE_PAGE_GENERIC ((USHORT) 0x01)
#define HID_USAGE_GENERIC_KEYBOARD ((USHORT) 0x06)

HWND keyboard;
HWND keyCode;

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR argument, _In_ int command) {
	WNDCLASSEXW windowClass = {0};
	windowClass.cbSize = sizeof(WNDCLASSEXW);
	windowClass.hInstance = instance;
	windowClass.lpfnWndProc = WindowProcedure;
	windowClass.lpszClassName = CLASS_NAME;
	windowClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH) COLOR_WINDOW;
	windowClass.style = CS_HREDRAW | CS_VREDRAW;

	if(!RegisterClassExW(&windowClass)) {
		MessageBoxW(NULL, L"Unable to register window class", L"KeyGetter Error", MB_OK | MB_ICONERROR | MB_DEFBUTTON1 | MB_SYSTEMMODAL);
		return 1;
	}

	NONCLIENTMETRICS metrics = {0};
	metrics.cbSize = sizeof(metrics);
	SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, metrics.cbSize, &metrics, 0);
	HFONT font = CreateFontIndirectW(&metrics.lfCaptionFont);
	int width = 400;
	int height = metrics.iCaptionHeight + 60;
	HWND window = CreateWindowExW(0L, CLASS_NAME, L"KeyGetter", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, instance, NULL);
	RECT rect;
	DwmGetWindowAttribute(window, DWMWA_EXTENDED_FRAME_BOUNDS, &rect, sizeof(RECT));
	int windowWidth = rect.right - rect.left;
	int keyCodeWidth = metrics.iCaptionHeight;
	keyboard = CreateWindowExW(0L, L"Edit", L"", WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL | ES_READONLY, 10, 10, windowWidth - keyCodeWidth - 30, metrics.iCaptionHeight, window, NULL, NULL, NULL);
	keyCode = CreateWindowExW(0L, L"Edit", L"", WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL | ES_READONLY, windowWidth - keyCodeWidth - 12, 10, keyCodeWidth, metrics.iCaptionHeight, window, NULL, NULL, NULL);
	SendMessageW(keyboard, WM_SETFONT, (LPARAM) font, TRUE);
	SendMessageW(keyCode, WM_SETFONT, (LPARAM) font, TRUE);
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

	return 0;
}

void ProcessRawInput(RAWINPUT* rawInput) {
	RAWINPUTHEADER header = rawInput->header;
	UINT size = 0;
	GetRawInputDeviceInfoW(header.hDevice, RIDI_DEVICENAME, NULL, &size);
	WCHAR* deviceName = malloc((size + 1) * sizeof(WCHAR));
	GetRawInputDeviceInfoW(header.hDevice, RIDI_DEVICENAME, deviceName, &size);
	SetWindowTextW(keyboard, deviceName);
	free(deviceName);
	WCHAR* formatString = L"%d";
	size = _scwprintf(formatString, rawInput->data.keyboard.VKey);

	if(size == -1) {
		MessageBoxW(NULL, L"Error while getting the length of the error message", L"Fatal Error", MB_OK | MB_DEFBUTTON1 | MB_ICONERROR | MB_SYSTEMMODAL);
		return;
	}

	size += 1;
	WCHAR* displayMessage = malloc(sizeof(WCHAR) * size);

	if(displayMessage == NULL) {
		MessageBoxW(NULL, L"Out of memory error. Not enough memory for the error message", L"Memory Error", MB_OK | MB_DEFBUTTON1 | MB_ICONERROR | MB_SYSTEMMODAL);
		return;
	}

	size = swprintf_s(displayMessage, size, formatString, rawInput->data.keyboard.VKey);

	if(size == -1) {
		free(displayMessage);
		MessageBoxW(NULL, L"Error while formatting the error message", L"Fatal Error", MB_OK | MB_DEFBUTTON1 | MB_ICONERROR | MB_SYSTEMMODAL);
		return;
	}

	SetWindowTextW(keyCode, displayMessage);
	free(displayMessage);
}

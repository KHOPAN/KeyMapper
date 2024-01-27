#include "definition.h"

#define CLASS_NAME L"KeyGetterClass"
#define HID_USAGE_PAGE_GENERIC ((USHORT) 0x01)
#define HID_USAGE_GENERIC_KEYBOARD ((USHORT) 0x06)

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

	HWND window = CreateWindowExW(0L, CLASS_NAME, L"KeyGetter", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, NULL, NULL, instance, NULL);
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

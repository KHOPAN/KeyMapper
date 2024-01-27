#include "definition.h"

#define CLASS_NAME L"KeyGetterClass"

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

	CreateWindowExW(0L, CLASS_NAME, L"KeyGetter", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, NULL, NULL, instance, NULL);
	MSG message = {0};

	while(GetMessageW(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}

	return 0;
}

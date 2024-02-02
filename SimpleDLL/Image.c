#include <process.h>
#include "definition.h"

#define CLASS_NAME L"ImageWindowClass"

BOOL globalState;
HINSTANCE globalInstance;
volatile HWND globalWindow;
HBITMAP globalBitmap;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void __cdecl ImageWindowThread(void);

EXPORT(Image) {
	if(argc < 1) {
		MessageBoxW(NULL, L"Path argument mising, please specified the image path", L"Error", MB_OK | MB_ICONERROR | MB_DEFBUTTON1 | MB_SYSTEMMODAL);
		return;
	}

	globalBitmap = LoadImageW(globalInstance, argv[0], IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	globalState = !globalState;
	ShowWindow(globalWindow, globalState ? SW_MAXIMIZE : SW_HIDE);
}

BOOL ImageInitialize(HINSTANCE instance) {
	globalInstance = instance;
	_beginthread((_beginthread_proc_type) ImageWindowThread, 0, NULL);
	return TRUE;
}

BOOL ImageDestroy(HINSTANCE instance) {
	DestroyWindow(globalWindow);
	UnregisterClassW(CLASS_NAME, instance);
	return TRUE;
}

void __cdecl ImageWindowThread(void) {
	WNDCLASSEXW windowClass = {0};
	windowClass.cbSize = sizeof(WNDCLASSEXW);
	windowClass.hInstance = globalInstance;
	windowClass.lpszClassName = CLASS_NAME;
	windowClass.lpfnWndProc = WindowProcedure;
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.hbrBackground = (HBRUSH) COLOR_WINDOWFRAME;
	windowClass.hCursor = NULL;

	if(!RegisterClassExW(&windowClass)) {
		MessageBoxW(NULL, L"Failed to register window class!", L"Error", MB_OK | MB_ICONERROR | MB_DEFBUTTON1 | MB_SYSTEMMODAL);
		return;
	}

	globalWindow = CreateWindowExW(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, CLASS_NAME, L"Image", WS_OVERLAPPED | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, NULL, NULL, globalInstance, NULL);
	MSG message = {0};

	while(GetMessageW(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}
}

LRESULT CALLBACK WindowProcedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
	switch(message) {
	case WM_CLOSE:
		DestroyWindow(window);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SETCURSOR:
		SetCursor(NULL);
		return 0;
	case WM_PAINT: {
		PAINTSTRUCT paintStruct;
		HDC context = BeginPaint(window, &paintStruct);
		HDC memoryContext = CreateCompatibleDC(context);
		HBITMAP oldBitmap = SelectObject(memoryContext, globalBitmap);
		BITMAP bitmap;
		GetObjectW(globalBitmap, sizeof(bitmap), &bitmap);
		BitBlt(context, 0, 0, bitmap.bmWidth, bitmap.bmHeight, memoryContext, 0, 0, SRCCOPY);
		SelectObject(memoryContext, oldBitmap);
		DeleteDC(memoryContext);
		EndPaint(window, &paintStruct);
		return 0;
	}
	}

	return DefWindowProcW(window, message, wparam, lparam);
}

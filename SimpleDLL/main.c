#include <Windows.h>

__declspec(dllexport) void __stdcall SimpleFunction() {
	MessageBoxW(NULL, L"SimpleFunction() has been called!", L"SimpleDLL", MB_OK | MB_ICONINFORMATION | MB_DEFBUTTON1 | MB_SYSTEMMODAL);
}

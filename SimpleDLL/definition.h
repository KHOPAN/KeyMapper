#pragma once

#include <Windows.h>

#define EXPORT(x) __declspec(dllexport) void __stdcall x(BOOL state, int argc, wchar_t** argv)

#ifdef __cplusplus
extern "C" {
#endif
EXPORT(Shutdown);
EXPORT(Image);
BOOL EnablePrivilege(LPCWSTR);
BOOL ImageInitialize(HINSTANCE);
BOOL ImageDestroy(HINSTANCE);
#ifdef __cplusplus
}
#endif

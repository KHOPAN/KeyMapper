#pragma once

#include <Windows.h>

#define EXPORT(x) __declspec(dllexport) void __stdcall x()

#ifdef __cplusplus
extern "C" {
#endif
EXPORT(Shutdown);
BOOL EnablePrivilege(LPCWSTR);
#ifdef __cplusplus
}
#endif

#pragma once

#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif
void DisplayError(DWORD, LPCWSTR);
void ParseJSON(BYTE*);
#ifdef __cplusplus
}
#endif

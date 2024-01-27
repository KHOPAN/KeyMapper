#pragma once

#include <Windows.h>

#define KEYMAPPER_ERROR MB_OK | MB_ICONERROR | MB_DEFBUTTON1 | MB_SYSTEMMODAL

#ifdef __cplusplus
extern "C" {
#endif
void DisplayError(DWORD, LPCWSTR);
void ParseJSON(BYTE*);
#ifdef __cplusplus
}
#endif

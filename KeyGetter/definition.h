#pragma once

#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void ProcessRawInput(RAWINPUT*);
#ifdef __cplusplus
}
#endif

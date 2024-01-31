#pragma once

#include <Windows.h>

#define KEYMAPPER_ERROR MB_OK | MB_ICONERROR | MB_DEFBUTTON1 | MB_SYSTEMMODAL

typedef void (*LibraryFunction) (BOOL, int, char**);

#define TRIGGER_PRESS 0
#define TRIGGER_RELEASE 1

typedef struct {
	USHORT keyCode;
	LibraryFunction function;
	int trigger;
	BOOL state;
} MappingData;

typedef struct {
	WCHAR* keyboardHuid;
	MappingData* mappings;
	size_t mappingSize;
} KeyMapStruct;

typedef struct {
	KeyMapStruct* keyMapStruct;
	size_t keyMapSize;
} KeyMappings;

#ifdef __cplusplus
extern "C" {
#endif
void DisplayError(DWORD, LPCWSTR);
BOOL ParseJSON(BYTE*);
void SetMappingData(KeyMappings);
KeyMappings GetMappingData();
void FreeMappingData();
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void ProcessRawInput(RAWINPUT*);
WCHAR* WidenHeap(CHAR*);
CHAR* FormatHeap(const CHAR* formatString, ...);
#ifdef __cplusplus
}
#endif

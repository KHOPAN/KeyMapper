#pragma once

#include <Windows.h>

#define KEYMAPPER_ERROR MB_OK | MB_ICONERROR | MB_DEFBUTTON1 | MB_SYSTEMMODAL

typedef void (*VoidFunction) (void);

#define TRIGGER_PRESS 0
#define TRIGGER_RELEASE 1
#define TRIGGER_HOLD 2
#define TRIGGER_TOGGLE 3

typedef struct {
	USHORT keyCode;
	VoidFunction function;
	int trigger;
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

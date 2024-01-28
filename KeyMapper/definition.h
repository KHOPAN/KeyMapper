#pragma once

#include <Windows.h>

#define KEYMAPPER_ERROR MB_OK | MB_ICONERROR | MB_DEFBUTTON1 | MB_SYSTEMMODAL

typedef void (*VoidFunction) (void);

typedef struct {
	USHORT keyCode;
	VoidFunction function;
} MappingData;

typedef struct {
	CHAR* keyboardHuid;
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
#ifdef __cplusplus
}
#endif

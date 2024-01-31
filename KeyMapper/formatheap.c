#include <stdio.h>
#include "definition.h"

CHAR* FormatHeap(const CHAR* formatString, ...) {
	va_list list;
	va_start(list, formatString);
	int size = _vscprintf(formatString, list);

	if(size == -1) {
		MessageBoxW(NULL, L"Error while getting the length of the error message", L"Fatal Error", MB_OK | MB_DEFBUTTON1 | MB_ICONERROR | MB_SYSTEMMODAL);
		return NULL;
	}

	size += 1;
	char* displayMessage = malloc(sizeof(char) * size);

	if(displayMessage == NULL) {
		MessageBoxW(NULL, L"Out of memory error. Not enough memory for the error message", L"Memory Error", MB_OK | MB_DEFBUTTON1 | MB_ICONERROR | MB_SYSTEMMODAL);
		return NULL;
	}

	size = vsprintf_s(displayMessage, size, formatString, list);
	va_end(list);

	if(size == -1) {
		free(displayMessage);
		MessageBoxW(NULL, L"Error while formatting the error message", L"Fatal Error", MB_OK | MB_DEFBUTTON1 | MB_ICONERROR | MB_SYSTEMMODAL);
		return NULL;
	}

	return displayMessage;
}

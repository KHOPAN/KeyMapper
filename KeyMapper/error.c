#include <stdio.h>
#include "definition.h"

void DisplayError(DWORD errorCode, LPCWSTR functionName) {
	wchar_t* messageBuffer = NULL;
	DWORD size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR) &messageBuffer, 0, NULL);

	if(size == 0) {
		MessageBoxW(NULL, L"Error while getting the error message text", L"KeyMapper Fatal Error", KEYMAPPER_ERROR);
		return;
	}

	const wchar_t* formatString = L"%ws error ocurred. Error code: %u Message:\n%ws";
	size = _scwprintf(formatString, functionName, errorCode, messageBuffer);

	if(size == -1) {
		MessageBoxW(NULL, L"Error while getting the length of the error message", L"KeyMapper Fatal Error", KEYMAPPER_ERROR);
		return;
	}

	size += 1;
	wchar_t* displayMessage = malloc(sizeof(wchar_t) * size);

	if(displayMessage == NULL) {
		MessageBoxW(NULL, L"Out of memory error. Not enough memory for the error message", L"KeyMapper Memory Error", KEYMAPPER_ERROR);
		return;
	}

	size = swprintf_s(displayMessage, size, formatString, functionName, errorCode, messageBuffer);

	if(size == -1) {
		free(displayMessage);
		MessageBoxW(NULL, L"Error while formatting the error message", L"KeyMapper Fatal Error", KEYMAPPER_ERROR);
		return;
	}

	MessageBoxW(NULL, displayMessage, L"KeyMapper Error", KEYMAPPER_ERROR);
	free(displayMessage);
}

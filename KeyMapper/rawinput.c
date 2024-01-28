#include <stdio.h>
#include "definition.h"

void ProcessRawInput(RAWINPUT* rawInput) {
	HANDLE deviceHandle = rawInput->header.hDevice;
	UINT size = 0;
	GetRawInputDeviceInfoW(deviceHandle, RIDI_DEVICENAME, NULL, &size);
	WCHAR* deviceName = malloc((size + 1) * sizeof(WCHAR));

	if(deviceName == NULL) {
		DisplayError(ERROR_NOT_ENOUGH_MEMORY, L"malloc()");
		return;
	}

	GetRawInputDeviceInfoW(deviceHandle, RIDI_DEVICENAME, deviceName, &size);
	printf("Name: %ws\n", deviceName);
	free(deviceName);
}

#include <stdio.h>
#include "definition.h"

void ProcessKeyboard(KeyMapStruct, RAWKEYBOARD);

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
	KeyMappings mappings = GetMappingData();

	for(size_t x = 0; x < mappings.keyMapSize; x++) {
		KeyMapStruct keyMapping = mappings.keyMapStruct[x];

		if(wcscmp(deviceName, keyMapping.keyboardHuid) == 0) {
			ProcessKeyboard(keyMapping, rawInput->data.keyboard);
			break;
		}
	}

	free(deviceName);
}

void ProcessKeyboard(KeyMapStruct keyMapping, RAWKEYBOARD keyboard) {
	for(int i = 0; i < keyMapping.mappingSize; i++) {
		MappingData mapping = keyMapping.mappings[i];

		if(mapping.keyCode == keyboard.VKey) {
			if(mapping.trigger == TRIGGER_PRESS && keyboard.Flags == 0) {
				mapping.function(FALSE, 0, NULL);
			} else if(mapping.trigger == TRIGGER_RELEASE && keyboard.Flags == 1) {
				mapping.function(FALSE, 0, NULL);
			} else if(mapping.trigger == TRIGGER_HOLD) {
				mapping.function(!keyboard.Flags, 0, NULL);
			} else if(mapping.trigger == TRIGGER_TOGGLE) {
				mapping.state = !mapping.state;
				mapping.function(mapping.state, 0, NULL);
			}

			break;
		}
	}
}

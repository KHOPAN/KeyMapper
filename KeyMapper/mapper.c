#include "definition.h"

KeyMappings keyMappingList;

void SetMappingData(KeyMappings mapping) {
	keyMappingList = mapping;
}

KeyMappings GetMappingData() {
	return keyMappingList;
}

void FreeMappingData() {
	for(size_t x = 0; x < keyMappingList.keyMapSize; x++) {
		KeyMapStruct keyMapping = keyMappingList.keyMapStruct[x];

		for(size_t y = 0; y < keyMapping.keyboardSize; y++) {
			free(keyMapping.keyboardHuid[y]);
		}

		free(keyMapping.keyboardHuid);
		free(keyMapping.mappings);
	}

	free(keyMappingList.keyMapStruct);
}

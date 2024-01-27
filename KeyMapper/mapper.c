#include "definition.h"

KeyMappings keyMappingList;

void SetMappingData(KeyMappings mapping) {
	keyMappingList = mapping;
}

void FreeMappingData() {
	for(size_t i = 0; i < keyMappingList.keyMapSize; i++) {
		KeyMapStruct keyMapping = keyMappingList.keyMapStruct[i];
		free(keyMapping.mappings);
	}

	free(keyMappingList.keyMapStruct);
}

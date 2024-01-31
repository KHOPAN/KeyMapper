#include <string.h>
#include "definition.h"

WCHAR* WidenHeap(CHAR* input) {
	size_t length = strlen(input);
	WCHAR* buffer = malloc(sizeof(WCHAR) * (length + 1));

	if(buffer == NULL) {
		return NULL;
	}

	for(int i = 0; i < length; i++) {
		buffer[i] = input[i];
	}

	buffer[length] = 0;
	return buffer;
}

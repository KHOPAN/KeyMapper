#include <stdio.h>
#include "definition.h"

LRESULT CALLBACK WindowProcedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
	switch(message) {
	case WM_INPUT: {
		HRAWINPUT rawInputHandle = (HRAWINPUT) lparam;
		UINT size = 0;
		GetRawInputData(rawInputHandle, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));

		if(size == 0) {
			return 0;
		}

		BYTE* bytes = malloc(size);

		if(bytes == NULL) {
			return 0;
		}

		GetRawInputData(rawInputHandle, RID_INPUT, bytes, &size, sizeof(RAWINPUTHEADER));
		RAWINPUT* raw = (RAWINPUT*) bytes;

		if(raw == NULL) {
			return 0;
		}

		ProcessRawInput(raw);
		free(bytes);
		return 0;
	}
	}

	return DefWindowProcW(window, message, wparam, lparam);
}

#include <stdio.h>
#include "definition.h"

LRESULT CALLBACK WindowProcedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
	switch(message) {
	case WM_INPUT:
		printf("Input Message\n");
		return 0;
	}

	return DefWindowProcW(window, message, wparam, lparam);
}

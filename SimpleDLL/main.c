#include "definition.h"

BOOL WINAPI DllMain(_In_ HINSTANCE instance, _In_ DWORD reason, _In_ LPVOID reserved) {
	switch(reason) {
	case DLL_PROCESS_ATTACH:
		return ImageInitialize(instance);
	case DLL_PROCESS_DETACH:
		return ImageDestroy(instance);
	}

	return FALSE;
}

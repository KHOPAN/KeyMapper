#include "definition.h"

__declspec(dllexport) void __stdcall Shutdown() {
	EnablePrivilege(SE_SHUTDOWN_NAME);
	InitiateSystemShutdownW(NULL, NULL, 0, TRUE, FALSE);
}

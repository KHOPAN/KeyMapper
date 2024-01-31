#include <stdio.h>
#include "definition.h"

EXPORT(Shutdown) {
	EnablePrivilege(SE_SHUTDOWN_NAME);
	int response = MessageBoxW(NULL, L"Are you sure you want to shutdown?", L"Confirm Shutdown", MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON1 | MB_SYSTEMMODAL);

	if(response != 1) {
		return;
	}

	InitiateSystemShutdownW(NULL, NULL, 0, TRUE, FALSE);
}

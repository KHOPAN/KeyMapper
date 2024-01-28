#include <Windows.h>

__declspec(dllexport) void __stdcall OpenYouTube() {
	system("start chrome https://www.youtube.com");
}

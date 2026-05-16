#include <omc/omctp/config.h>

#if CFG_OS == CFG_OS_WIN32

#include <Windows.h>

int WINAPI WinMain(HINSTANCE p_winInstance, HINSTANCE p_winPrevInstance, LPTSTR p_winCmdLine, int p_winShowCmd)
{
	return 0;
}

#endif // CFG_OS_WIN32, CFG_OS
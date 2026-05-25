#define PROJECT_OMCTP
#define OMCTP_WIN_CPP

#include <omc/omctp/config.h>

#include "main.h"

#if CFG_OS == CFG_OS_WIN32

#ifndef UNICODE
#define UNICODE
#endif
#define NOMINMAX
#include <Windows.h>

int WINAPI wWinMain(_In_ HINSTANCE p_winInstance, _In_opt_ HINSTANCE p_winPrevInstance, _In_ LPWSTR p_cmdLine, _In_ int p_winShowCmd)
{
	omc::omctp::real_omctp_instance game;
	omc::error err = game.main(GetCommandLineA());
	if (err.m_error_code != omc::error::code::success)
		MessageBoxA(nullptr, err.m_message.c_str(), "OMCtp", MB_OK | MB_ICONERROR);

	return (int)err.m_error_code;
}

#endif // CFG_OS_WIN32, CFG_OS
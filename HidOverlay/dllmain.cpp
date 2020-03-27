/**
 @file           dllmain.cpp
****************************************************************************
 Consts

  Implements a HID.dll layer to filter devices from detection

 Copyright (C) 2020 Martin Burri  (bm98@burri-web.org)

<hr>

 @b Project      HidOverlay<br>

 @author         M. Burri
 @date           24-Mar-2020

****************************************************************************
<hr>
 @b Updates
 - dd-mmm-yyyy V. Name: Description

****************************************************************************/

#include "pch.h"


// Defines the entry point for the DLL application.
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_PROCESS_DETACH: // we don't free the orig DLL, assuming the caller shuts down anyway
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}




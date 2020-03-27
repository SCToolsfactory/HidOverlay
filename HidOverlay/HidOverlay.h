/**
 @file           HidOverlay.h
****************************************************************************
 Consts

  Implements a HID.dll layer to filter devices from detection

   Droping the DLL into the applications directory to make use of the search 
   strategy for modules (if a module is not loaded with a path)

    The directory containing the executable file
	The Windows system directory returned by GetWindowsDirectory
	The 16-bit system directory—that is, the System subfolder under the Windows directory
	The Windows directory returned by GetSystemDirectory
	The process' current directory
	The directories listed in the PATH environment variable

	This overlay renamed to Hid.dll reads a file 'HidOverlay.blist' from the callers
	application (exe) directory and collects devices as 'to be ignored'
	aka Blacklisted.

	The format of the blacklist file is line based text with lines such as:

	vid pid comment

	e.g. 
	0x06a3 0x0b6a Saitek X-65F

	file checking is NONE - start in the first line and an NL at the end.
	MAX number of devices to collect is defined in BLACKLIST_SIZE


 Copyright (C) 2020 Martin Burri  (bm98@burri-web.org)

<hr>

 @b Project      HidOverlay<br>

 @author         M. Burri
 @date           26-Mar-2020

****************************************************************************
<hr>
 @b Updates
 - dd-mmm-yyyy V. Name: Description

****************************************************************************/

#pragma once

// INCLUDE ORIGINAL API DEFS
#include "hidsdi.h"
#include "hidpi.h"


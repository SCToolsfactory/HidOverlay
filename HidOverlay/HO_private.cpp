/**
 @file           HO_private.cpp
****************************************************************************
 Consts

  Implements private functions for HID.dll layer

 Copyright (C) 2020 Martin Burri  (bm98@burri-web.org)

<hr>

 @b Project      HidOverlay<br>

 @author         M. Burri
 @date           27-Mar-2020

****************************************************************************
<hr>
 @b Updates
 - dd-mmm-yyyy V. Name: Description

****************************************************************************/

#include "pch.h"

#include <stdio.h>
#include "hidsdi.h" // for HIDD_ATTRIBUTES

#include "HO_private.h"


// Debug and general output - appends the Log file with the string
VOID DebOut(LPCSTR text) {
	static int sequ = 0; // debug sequence

	FILE * logfile;

	if (0 == fopen_s(&logfile, Logfile, "a"))
	{
		fprintf(logfile, "%i        %s \n", ++sequ, text);
		fclose(logfile);
	}
}


// the HID.DLL handle
static HMODULE HidModule = 0;

// a  lookuptable for HID.DLL functions
static FARPROC HFunctionTable[eHidX_LAST];

// a list of blacklisted devices
static HIDD_ATTRIBUTES HidBlackList[BLACKLIST_SIZE]; // enough..

// forward defs..
BOOL LoadHidModule();
VOID LoadBlacklist();
VOID LoadFunctions();


// Initializes this Module - deferred from DLL main - see MS Doc...
// HidModule != 0 is assumed to signal init done
// returns FALSE for failed loading, else TRUE
BOOL InitModule() 
{
	if (HidModule != 0) return TRUE; // already done
	
	// start logging
	DebOut("");
	DebOut("--------------------------------------------------");
	DebOut("Init HidOverlay module");

	if (FALSE == LoadHidModule()) {
		char buffer[200];
		::sprintf_s(buffer, 200, "Loading System HID.DLL failed with error code: 0x%X", GetLastError() );
		DebOut(buffer);
		return FALSE; // System HID.DLL load failed, cannot do anything more..
	}

	LoadFunctions();
	LoadBlacklist();
	return TRUE;
}


// returns the function ptr
// beware no range check etc. it is assumed to do this right in this module
FARPROC GetProcAddress_(HidFunc function) 
{
	if (FALSE == InitModule()) return NULL; // that should really not happen...
	
	return HFunctionTable[function];
}


// returns TRUE if the device is listed
BOOL InBlacklist(PHIDD_ATTRIBUTES attr)
{
	// just scan the list - should be small enough not hog the caller
	for (int i = 0; i < BLACKLIST_SIZE; i++) {
		if ((attr->VendorID == HidBlackList[i].VendorID)
			&& (attr->ProductID == HidBlackList[i].ProductID)) {
			return TRUE;
		}
	}
	return FALSE;
}


// Load the Win System HID.DLL module
// dumps some info into the LOG file (also in release mode)
BOOL LoadHidModule()
{
	CHAR buffer[200];
	int sLen = ::GetSystemDirectoryA(buffer, 200); // from Win System Dir
	if (sLen > 0) {
		::strcat_s(buffer, 200, "\\HID.dll");
		HidModule = ::LoadLibraryA(buffer);
		DEB("Loading System Hid Module ..");
	}
	else
	{
		HidModule = 0;
	}
	// check the outcome
	if (HidModule > 0) {
		DebOut("Loading System Hid Module OK");
		return TRUE;
	}
	else {
		DebOut("Loading System Hid Module FAILED");
		return FALSE;
	}
}


// Load the items to hide from detection
// dumps some info into the LOG file (also in release mode)
VOID LoadBlacklist()
{
	ZeroMemory(HidBlackList, BLACKLIST_SIZE);

	// Load from file
	// line format: "0xVID 0xPID comment \n"
	//   e.g. "0x06a3 0x0b6a Saitek X-65F"

	FILE * blfile;
	char line[250];


	// just to check where we are right now
	char directory[500];
	DebOut("Looking for: "); DebOut(BlacklistName);
	DebOut("Trying to load the config file from this directory:");
	GetCurrentDirectoryA(500, directory);
	DebOut(directory); // this helps to find where to put the BList file

	// try to open the blacklist file
	if (0 == fopen_s(&blfile, BlacklistName, "r"))
	{
		DEB("  about to read..");
		int blIndex = 0;
		while ((blIndex < BLACKLIST_SIZE) && (fgets(line, 250, blfile) != NULL)) {
			DebOut(line);
			// must find two hex tokens (vid pid) the comment is ignored
			int vid, pid;
			if (2 == _snscanf_s(line, 250, "%x %x ", &vid, &pid)) {
				DEB("  found a valid VID PID of a Device");
				HidBlackList[blIndex].VendorID = (USHORT)vid;
				HidBlackList[blIndex].ProductID = (USHORT)pid;
				blIndex++;
			}
		}
		fclose(blfile);
	}
	else
	{
		DebOut("  no file found.. ");
	}

}


// Load all ProcAdresses from the Module - faster than string lookup for every client call
VOID LoadFunctions()
{
	ZeroMemory(HFunctionTable, eHidX_LAST);

	HFunctionTable[eHidD_FlushQueue] = GetProcAddress(HidModule, "HidD_FlushQueue");
	HFunctionTable[eHidD_FreePreparsedData] = GetProcAddress(HidModule, "HidD_FreePreparsedData");
	HFunctionTable[eHidD_GetAttributes] = GetProcAddress(HidModule, "HidD_GetAttributes");
	HFunctionTable[eHidD_GetConfiguration] = GetProcAddress(HidModule, "HidD_GetConfiguration");
	HFunctionTable[eHidD_GetFeature] = GetProcAddress(HidModule, "HidD_GetFeature");
	HFunctionTable[eHidD_GetHidGuid] = GetProcAddress(HidModule, "HidD_GetHidGuid");
	HFunctionTable[eHidD_GetIndexedString] = GetProcAddress(HidModule, "HidD_GetIndexedString");
	HFunctionTable[eHidD_GetInputReport] = GetProcAddress(HidModule, "HidD_GetInputReport");
	HFunctionTable[eHidD_GetManufacturerString] = GetProcAddress(HidModule, "HidD_GetManufacturerString");
	HFunctionTable[eHidD_GetMsGenreDescriptor] = GetProcAddress(HidModule, "HidD_GetMsGenreDescriptor");
	HFunctionTable[eHidD_GetNumInputBuffers] = GetProcAddress(HidModule, "HidD_GetNumInputBuffers");
	HFunctionTable[eHidD_GetPhysicalDescriptor] = GetProcAddress(HidModule, "HidD_GetPhysicalDescriptor");
	HFunctionTable[eHidD_GetPreparsedData] = GetProcAddress(HidModule, "HidD_GetPreparsedData");
	HFunctionTable[eHidD_GetProductString] = GetProcAddress(HidModule, "HidD_GetProductString");
	HFunctionTable[eHidD_GetSerialNumberString] = GetProcAddress(HidModule, "HidD_GetSerialNumberString");
	HFunctionTable[eHidD_SetConfiguration] = GetProcAddress(HidModule, "HidD_SetConfiguration");
	HFunctionTable[eHidD_SetFeature] = GetProcAddress(HidModule, "HidD_SetFeature");
	HFunctionTable[eHidD_SetNumInputBuffers] = GetProcAddress(HidModule, "HidD_SetNumInputBuffers");
	HFunctionTable[eHidD_SetOutputReport] = GetProcAddress(HidModule, "HidD_SetOutputReport");

	HFunctionTable[eHidP_GetButtonCaps] = GetProcAddress(HidModule, "HidP_GetButtonCaps");
	HFunctionTable[eHidP_GetCaps] = GetProcAddress(HidModule, "HidP_GetCaps");
	HFunctionTable[eHidP_GetData] = GetProcAddress(HidModule, "HidP_GetData");
	HFunctionTable[eHidP_GetExtendedAttributes] = GetProcAddress(HidModule, "HidP_GetExtendedAttributes");
	HFunctionTable[eHidP_GetLinkCollectionNodes] = GetProcAddress(HidModule, "HidP_GetLinkCollectionNodes");
	HFunctionTable[eHidP_GetScaledUsageValue] = GetProcAddress(HidModule, "HidP_GetScaledUsageValue");
	HFunctionTable[eHidP_GetSpecificButtonCaps] = GetProcAddress(HidModule, "HidP_GetSpecificButtonCaps");
	HFunctionTable[eHidP_GetSpecificValueCaps] = GetProcAddress(HidModule, "HidP_GetSpecificValueCaps");
	HFunctionTable[eHidP_GetUsageValue] = GetProcAddress(HidModule, "HidP_GetUsageValue");
	HFunctionTable[eHidP_GetUsageValueArray] = GetProcAddress(HidModule, "HidP_GetUsageValueArray");
	HFunctionTable[eHidP_GetUsages] = GetProcAddress(HidModule, "HidP_GetUsages");
	HFunctionTable[eHidP_GetUsagesEx] = GetProcAddress(HidModule, "HidP_GetUsagesEx");
	HFunctionTable[eHidP_GetValueCaps] = GetProcAddress(HidModule, "HidP_GetValueCaps");
	HFunctionTable[eHidP_InitializeReportForID] = GetProcAddress(HidModule, "HidP_InitializeReportForID");
	HFunctionTable[eHidP_MaxDataListLength] = GetProcAddress(HidModule, "HidP_MaxDataListLength");
	HFunctionTable[eHidP_MaxUsageListLength] = GetProcAddress(HidModule, "HidP_MaxUsageListLength");
	HFunctionTable[eHidP_SetData] = GetProcAddress(HidModule, "HidP_SetData");
	HFunctionTable[eHidP_SetScaledUsageValue] = GetProcAddress(HidModule, "HidP_SetScaledUsageValue");
	HFunctionTable[eHidP_SetUsageValue] = GetProcAddress(HidModule, "HidP_SetUsageValue");
	HFunctionTable[eHidP_SetUsageValueArray] = GetProcAddress(HidModule, "HidP_SetUsageValueArray");
	HFunctionTable[eHidP_SetUsages] = GetProcAddress(HidModule, "HidP_SetUsages");
	HFunctionTable[eHidP_TranslateUsagesToI8042ScanCodes] = GetProcAddress(HidModule, "HidP_TranslateUsagesToI8042ScanCodes");
	HFunctionTable[eHidP_UnsetUsages] = GetProcAddress(HidModule, "HidP_UnsetUsages");
	HFunctionTable[eHidP_UsageListDifference] = GetProcAddress(HidModule, "HidP_UsageListDifference");
}

/**
 =file           HO_private.h
****************************************************************************
 Consts

  Defines private functions etc for HID.dll layer

 Copyright (C) 2020 Martin Burri  (bm98=burri-web.org)

<hr>

 =b Project      HidOverlay<br>

 =author         M. Burri
 =date           27-Mar-2020

****************************************************************************
<hr>
 =b Updates
 - dd-mmm-yyyy V. Name: Description

****************************************************************************/

#pragma once
// DLL local stuff

// Size of the 'to be ignored' devices
static const int BLACKLIST_SIZE = 10;
static const char* BlacklistName = "HidOverlay.blist";
static const char* Logfile = "HidOverlay.LOG";

// enumerates the HID DLL functions the the lookup table
enum HidFunc
{
	eHidD_FlushQueue = 1,
	eHidD_FreePreparsedData = 2,
	eHidD_GetAttributes = 3,
	eHidD_GetConfiguration = 4,
	eHidD_GetFeature = 5,
	eHidD_GetHidGuid = 6,
	eHidD_GetIndexedString = 7,
	eHidD_GetInputReport = 8,
	eHidD_GetManufacturerString = 9,
	eHidD_GetMsGenreDescriptor = 10,
	eHidD_GetNumInputBuffers = 11,
	eHidD_GetPhysicalDescriptor = 12,
	eHidD_GetPreparsedData = 13,
	eHidD_GetProductString = 14,
	eHidD_GetSerialNumberString = 15,
	//	HidD_Hello					=16, // ignored - no definition found..
	eHidD_SetConfiguration = 17,
	eHidD_SetFeature = 18,
	eHidD_SetNumInputBuffers = 19,
	eHidD_SetOutputReport = 20,

	eHidP_GetButtonCaps = 21,
	eHidP_GetCaps = 22,
	eHidP_GetData = 23,
	eHidP_GetExtendedAttributes = 24,
	eHidP_GetLinkCollectionNodes = 25,
	eHidP_GetScaledUsageValue = 26,
	eHidP_GetSpecificButtonCaps = 27,
	eHidP_GetSpecificValueCaps = 28,
	eHidP_GetUsageValue = 29,
	eHidP_GetUsageValueArray = 30,
	eHidP_GetUsages = 31,
	eHidP_GetUsagesEx = 32,
	eHidP_GetValueCaps = 33,
	eHidP_InitializeReportForID = 34,
	eHidP_MaxDataListLength = 35,
	eHidP_MaxUsageListLength = 36,
	eHidP_SetData = 37,
	eHidP_SetScaledUsageValue = 38,
	eHidP_SetUsageValue = 39,
	eHidP_SetUsageValueArray = 40,
	eHidP_SetUsages = 41,
	eHidP_TranslateUsagesToI8042ScanCodes = 42,
	eHidP_UnsetUsages = 43,
	eHidP_UsageListDifference = 44,

	eHidX_LAST = 50, // just serves as a const, must be larger than the functions values
};

// returns the function ptr
// beware no range check etc. it is assumed to do this right in this module
extern FARPROC GetProcAddress_(HidFunc function);

// returns TRUE if the device is listed
extern BOOL InBlacklist(PHIDD_ATTRIBUTES attr);

// returns TRUE if the device is listed
extern VOID DebOut(LPCSTR text);

// debug output only for development
// use the DEB macro to limit for DEBUG builds, DebOut() is always written
#if defined _DEBUG
extern VOID DebOut(LPCSTR text);
#define DEB(msg)	DebOut(##msg);
#else
#define DEB(msg)	// ignored #msg
#endif

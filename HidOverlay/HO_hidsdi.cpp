/**
 @file           HI_hidpi.cpp
****************************************************************************
 Consts

  Implements the HidD_ routines for the  HID.dll layer

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


/*
Implement HIDsdi call propagation
*/
#include "pch.h"

#include <stdio.h>   // need sprintf_s

#include "HidOverlay.h"
#include "HO_private.h"


// IMPLEMENTATION of the HidD_ routine propagation
//
// We intercept the Call for Preparsed data (HidD_GetPreparsedData) and return FALSE
// if the device is blacklisted - so the caller cannot 
//  identify it as valid HID device (and should ignore it)
// Deb out is only to see the call sequence of the Caller application

// * * * * * * *
typedef BOOLEAN(WINAPI *PGetAttributes)(_In_ HANDLE, _Out_ PHIDD_ATTRIBUTES);

_Must_inspect_result_
_Success_(return == TRUE)
BOOLEAN __stdcall
HidD_GetAttributes(
	_In_  HANDLE              HidDeviceObject,
	_Out_ PHIDD_ATTRIBUTES    Attributes
)
{
	DEB("HidD_GetAttributes");
	PGetAttributes pF = (PGetAttributes)GetProcAddress_(eHidD_GetAttributes);
	if (NULL != pF) {
		BOOLEAN ret = pF(HidDeviceObject, Attributes); // regular call propagated to the orig DLL
		// INTERCEPTION ALGO - Check if this device is to be intercepted
		if ((ret == TRUE) && InBlacklist(Attributes)) {
			// need to hide this one
			char buffer[100];
			::sprintf_s(buffer, 100, "Intercepted: 0x%04x 0x%04x", Attributes->VendorID, Attributes->ProductID);
			DebOut(buffer); // we write this also in release builds
			return FALSE; // fake return FALSE for devices to be ignored
		}
		return ret; // regular return of the orig call result
	}
	else return FALSE;
}


// * * * * * * *
typedef void(WINAPI *GetHidGuid)(__out  LPGUID);

void __stdcall
HidD_GetHidGuid(
	__out  LPGUID   HidGuid
)
{
	DEB("HidD_GetHidGuid");
	GetHidGuid pF = (GetHidGuid)GetProcAddress_(eHidD_GetHidGuid);
	if (NULL != pF)  pF(HidGuid);
}


// * * * * * * *
typedef BOOLEAN(WINAPI *GetPreparsedData)(_In_ HANDLE, _Out_ _When_(return != 0, __drv_allocatesMem(Mem)) PHIDP_PREPARSED_DATA*);

_Must_inspect_result_
_Success_(return == TRUE)
BOOLEAN __stdcall
HidD_GetPreparsedData(
	_In_   HANDLE                  HidDeviceObject,
	_Out_ _When_(return != 0, __drv_allocatesMem(Mem))  PHIDP_PREPARSED_DATA  * PreparsedData
)
{
	DEB("HidD_GetPreparsedData");
	GetPreparsedData pF = (GetPreparsedData)GetProcAddress_(eHidD_GetPreparsedData);
	if (NULL != pF) {
	/* STRONGER CATCH ALGO
		// If intercepting HidD_GetAttributes above is not enough - try and enable this one
		// Here we intercept and check via VID_PID if the device is blacklisted
		// and return FALSE if the device is in that list
		// use the module handle to get VID_PID and check with HidD_GetAttributes (returns false if intercepted)
		HIDD_ATTRIBUTES attr;
		if (FALSE == HidD_GetAttributes(HidDeviceObject, &attr)) {
			return FALSE; // fake return FALSE for devices to be ignored
		}
	*/
		return pF(HidDeviceObject, PreparsedData); // regular call and return
	}
	else {
		return FALSE;
	}
}


// * * * * * * *
typedef BOOLEAN(WINAPI *FreePreparsedData)(_In_ __drv_freesMem(Mem) PHIDP_PREPARSED_DATA);

_Success_(return == TRUE)
BOOLEAN __stdcall
HidD_FreePreparsedData(
	_In_ __drv_freesMem(Mem) PHIDP_PREPARSED_DATA PreparsedData
)
{
	FreePreparsedData pF = (FreePreparsedData)GetProcAddress_(eHidD_FreePreparsedData);
	if (NULL != pF) return pF(PreparsedData);
	else return FALSE;
}


// * * * * * * *
typedef BOOLEAN(WINAPI *FlushQueue)(_In_ HANDLE);

_Success_(return == TRUE)
BOOLEAN __stdcall
HidD_FlushQueue(
	_In_    HANDLE                HidDeviceObject
)
{
	FlushQueue pF = (FlushQueue)GetProcAddress_(eHidD_FlushQueue);
	if (NULL != pF) return pF(HidDeviceObject);
	else return FALSE;
}


// * * * * * * *
typedef BOOLEAN(WINAPI *GetConfiguration)(_In_ HANDLE, _Out_writes_bytes_(ConfigurationLength) PHIDD_CONFIGURATION, _In_ ULONG);

_Must_inspect_result_
_Success_(return == TRUE)
BOOLEAN __stdcall
HidD_GetConfiguration(
	_In_   HANDLE               HidDeviceObject,
	_Out_writes_bytes_(ConfigurationLength) PHIDD_CONFIGURATION Configuration,
	_In_   ULONG                ConfigurationLength
)
{
	DEB("HidD_GetConfiguration");
	GetConfiguration pF = (GetConfiguration)GetProcAddress_(eHidD_GetConfiguration);
	if (NULL != pF) return pF(HidDeviceObject, Configuration, ConfigurationLength);
	else return FALSE;
}



// * * * * * * *
typedef BOOLEAN(WINAPI *SetConfiguration)(_In_ HANDLE, _In_reads_bytes_(ConfigurationLength) PHIDD_CONFIGURATION, _In_ ULONG);

_Must_inspect_result_
_Success_(return == TRUE)
BOOLEAN __stdcall
HidD_SetConfiguration(
	_In_   HANDLE               HidDeviceObject,
	_In_reads_bytes_(ConfigurationLength) PHIDD_CONFIGURATION Configuration,
	_In_   ULONG                ConfigurationLength
)
{
	SetConfiguration pF = (SetConfiguration)GetProcAddress_(eHidD_SetConfiguration);
	if (NULL != pF) return pF(HidDeviceObject, Configuration, ConfigurationLength);
	else return FALSE;
}




// * * * * * * *
typedef BOOLEAN(WINAPI *GetFeature)(_In_ HANDLE, _Out_writes_bytes_(ReportBufferLength) PVOID, _In_ ULONG);

_Must_inspect_result_
_Success_(return == TRUE)
BOOLEAN __stdcall
HidD_GetFeature(
	_In_    HANDLE   HidDeviceObject,
	_Out_writes_bytes_(ReportBufferLength) PVOID ReportBuffer,
	_In_    ULONG    ReportBufferLength
)
{
	GetFeature pF = (GetFeature)GetProcAddress_(eHidD_GetFeature);
	if (NULL != pF) return pF(HidDeviceObject, ReportBuffer, ReportBufferLength);
	else return FALSE;
}


// * * * * * * *
typedef BOOLEAN(WINAPI *SetFeature)(_In_ HANDLE, _In_reads_bytes_(ReportBufferLength) PVOID, _In_ ULONG);

_Must_inspect_result_
_Success_(return == TRUE)
BOOLEAN __stdcall
HidD_SetFeature(
	_In_    HANDLE   HidDeviceObject,
	_In_reads_bytes_(ReportBufferLength) PVOID ReportBuffer,
	_In_    ULONG    ReportBufferLength
)
{
	SetFeature pF = (SetFeature)GetProcAddress_(eHidD_SetFeature);
	if (NULL != pF) return pF(HidDeviceObject, ReportBuffer, ReportBufferLength);
	else return FALSE;
}


#if (NTDDI_VERSION >= NTDDI_WINXP) 

// * * * * * * *
typedef BOOLEAN(WINAPI *GetInputReport)(_In_ HANDLE, _Out_writes_bytes_(ReportBufferLength) PVOID, _In_ ULONG);

_Must_inspect_result_
_Success_(return == TRUE)
BOOLEAN __stdcall
HidD_GetInputReport(
	_In_    HANDLE   HidDeviceObject,
	_Out_writes_bytes_(ReportBufferLength) PVOID ReportBuffer,
	_In_    ULONG    ReportBufferLength
)
{
	GetInputReport pF = (GetInputReport)GetProcAddress_(eHidD_GetInputReport);
	if (NULL != pF) return pF(HidDeviceObject, ReportBuffer, ReportBufferLength);
	else return FALSE;
}


// * * * * * * *
typedef BOOLEAN(WINAPI *SetOutputReport)(_In_ HANDLE, _In_reads_bytes_(ReportBufferLength) PVOID, _In_ ULONG);

_Must_inspect_result_
_Success_(return == TRUE)
BOOLEAN __stdcall
HidD_SetOutputReport(
	_In_    HANDLE   HidDeviceObject,
	_In_reads_bytes_(ReportBufferLength) PVOID ReportBuffer,
	_In_    ULONG    ReportBufferLength
)
{
	SetOutputReport pF = (SetOutputReport)GetProcAddress_(eHidD_SetOutputReport);
	if (NULL != pF) return pF(HidDeviceObject, ReportBuffer, ReportBufferLength);
	else return FALSE;
}

#endif

// * * * * * * *
typedef BOOLEAN(WINAPI *GetNumInputBuffers)(_In_ HANDLE, _Out_ PULONG);

_Must_inspect_result_
_Success_(return == TRUE)
BOOLEAN __stdcall
HidD_GetNumInputBuffers(
	_In_  HANDLE  HidDeviceObject,
	_Out_ PULONG  NumberBuffers
)
{
	GetNumInputBuffers pF = (GetNumInputBuffers)GetProcAddress_(eHidD_GetNumInputBuffers);
	if (NULL != pF) return pF(HidDeviceObject, NumberBuffers);
	else return FALSE;
}


// * * * * * * *
typedef BOOLEAN(WINAPI *SetNumInputBuffers)(_In_ HANDLE, _In_ ULONG);

_Must_inspect_result_
_Success_(return == TRUE)
BOOLEAN __stdcall
HidD_SetNumInputBuffers(
	_In_ HANDLE HidDeviceObject,
	_In_ ULONG  NumberBuffers
)
{
	SetNumInputBuffers pF = (SetNumInputBuffers)GetProcAddress_(eHidD_SetNumInputBuffers);
	if (NULL != pF) return pF(HidDeviceObject, NumberBuffers);
	else return FALSE;
}


// * * * * * * *
typedef BOOLEAN(WINAPI *GetPhysicalDescriptor)(_In_ HANDLE, _Out_writes_bytes_(BufferLength) PVOID, _In_ ULONG);

_Must_inspect_result_
_Success_(return == TRUE)
BOOLEAN __stdcall
HidD_GetPhysicalDescriptor(
	_In_    HANDLE   HidDeviceObject,
	_Out_writes_bytes_(BufferLength) PVOID Buffer,
	_In_    ULONG    BufferLength
)
{
	DEB("HidD_GetPhysicalDescriptor");
	GetPhysicalDescriptor pF = (GetPhysicalDescriptor)GetProcAddress_(eHidD_GetPhysicalDescriptor);
	if (NULL != pF) return pF(HidDeviceObject, Buffer, BufferLength);
	else return FALSE;
}


// * * * * * * *
typedef BOOLEAN(WINAPI *GetManufacturerString)(_In_ HANDLE, _Out_writes_bytes_(BufferLength) PVOID, _In_ ULONG);

_Must_inspect_result_
_Success_(return == TRUE)
BOOLEAN __stdcall
HidD_GetManufacturerString(
	_In_    HANDLE   HidDeviceObject,
	_Out_writes_bytes_(BufferLength) PVOID Buffer,
	_In_    ULONG    BufferLength
)
{
	DEB("HidD_GetManufacturerString");
	GetManufacturerString pF = (GetManufacturerString)GetProcAddress_(eHidD_GetManufacturerString);
	if (NULL != pF) return pF(HidDeviceObject, Buffer, BufferLength);
	else return FALSE;
}


// * * * * * * *
typedef BOOLEAN(WINAPI *GetProductString)(_In_ HANDLE, _Out_writes_bytes_(BufferLength) PVOID, _In_ ULONG);

_Must_inspect_result_
_Success_(return == TRUE)
BOOLEAN __stdcall
HidD_GetProductString(
	_In_    HANDLE   HidDeviceObject,
	_Out_writes_bytes_(BufferLength) PVOID Buffer,
	_In_    ULONG    BufferLength
)
{
	DEB("HidD_GetProductString");
	GetProductString pF = (GetProductString)GetProcAddress_(eHidD_GetProductString);
	if (NULL != pF) return pF(HidDeviceObject, Buffer, BufferLength);
	else return FALSE;
}


// * * * * * * *
typedef BOOLEAN(WINAPI *GetIndexedString)(_In_ HANDLE, _In_ ULONG, _Out_writes_bytes_(BufferLength) PVOID, _In_ ULONG);

_Must_inspect_result_
_Success_(return == TRUE)
BOOLEAN __stdcall
HidD_GetIndexedString(
	_In_    HANDLE   HidDeviceObject,
	_In_    ULONG    StringIndex,
	_Out_writes_bytes_(BufferLength) PVOID Buffer,
	_In_    ULONG    BufferLength
)
{
	DEB("HidD_GetIndexedString");
	GetIndexedString pF = (GetIndexedString)GetProcAddress_(eHidD_GetIndexedString);
	if (NULL != pF) return pF(HidDeviceObject, StringIndex, Buffer, BufferLength);
	else return FALSE;
}


// * * * * * * *
typedef BOOLEAN(WINAPI *GetSerialNumberString)(_In_ HANDLE, _Out_writes_bytes_(BufferLength) PVOID, _In_ ULONG);

_Must_inspect_result_
_Success_(return == TRUE)
BOOLEAN __stdcall
HidD_GetSerialNumberString(
	_In_    HANDLE   HidDeviceObject,
	_Out_writes_bytes_(BufferLength) PVOID Buffer,
	_In_    ULONG    BufferLength
)
{
	DEB("HidD_GetSerialNumberString");
	GetSerialNumberString pF = (GetSerialNumberString)GetProcAddress_(eHidD_GetSerialNumberString);
	if (NULL != pF) return pF(HidDeviceObject, Buffer, BufferLength);
	else return FALSE;
}


#if (NTDDI_VERSION >= NTDDI_WINXP) 

// * * * * * * *
typedef BOOLEAN(WINAPI *GetMsGenreDescriptor)(_In_ HANDLE, _Out_writes_bytes_(BufferLength) PVOID, _In_ ULONG);

_Must_inspect_result_
_Success_(return == TRUE)
BOOLEAN __stdcall
HidD_GetMsGenreDescriptor(
	_In_    HANDLE   HidDeviceObject,
	_Out_writes_bytes_(BufferLength) PVOID Buffer,
	_In_    ULONG    BufferLength
)
{
	DEB("HidD_GetMsGenreDescriptor");
	GetMsGenreDescriptor pF = (GetMsGenreDescriptor)GetProcAddress_(eHidD_GetMsGenreDescriptor);
	if (NULL != pF) return pF(HidDeviceObject, Buffer, BufferLength);
	else return FALSE;
}

#endif


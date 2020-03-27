/**
 @file           HI_hidpi.cpp
****************************************************************************
 Consts

  Implements the HidP_ routines for the  HID.dll layer 

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
Implement HIDpi call propagation
*/
#include "pch.h"

#include "HidOverlay.h"
#include "HO_private.h"

// IMPLEMENTATION of HidP_calls
// Pure propagation, no interception in this part

// * * * * * * *
typedef NTSTATUS(WINAPI *GetCaps)(_In_ PHIDP_PREPARSED_DATA, _Out_ PHIDP_CAPS);

_Must_inspect_result_
_IRQL_requires_max_(PASSIVE_LEVEL)
NTSTATUS __stdcall
HidP_GetCaps(
	_In_      PHIDP_PREPARSED_DATA      PreparsedData,
	_Out_     PHIDP_CAPS                Capabilities
)
{
	GetCaps pF = (GetCaps)GetProcAddress_(eHidP_GetCaps);
	if (NULL != pF) return pF(PreparsedData, Capabilities);
	else return HIDP_STATUS_INTERNAL_ERROR;
}

// * * * * * * *
typedef NTSTATUS(WINAPI *GetLinkCollectionNodes)(_Out_writes_to_(*LinkCollectionNodesLength, *LinkCollectionNodesLength) PHIDP_LINK_COLLECTION_NODE,
	_Inout_ PULONG, _In_ PHIDP_PREPARSED_DATA);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NTSTATUS __stdcall
HidP_GetLinkCollectionNodes(
	_Out_writes_to_(*LinkCollectionNodesLength, *LinkCollectionNodesLength)     PHIDP_LINK_COLLECTION_NODE LinkCollectionNodes,
	_Inout_   PULONG                     LinkCollectionNodesLength,
	_In_      PHIDP_PREPARSED_DATA       PreparsedData
)
{
	GetLinkCollectionNodes pF = (GetLinkCollectionNodes)GetProcAddress_(eHidP_GetLinkCollectionNodes);
	if (NULL != pF) return pF(LinkCollectionNodes, LinkCollectionNodesLength, PreparsedData);
	else return HIDP_STATUS_INTERNAL_ERROR;
}

// * * * * * * *
typedef NTSTATUS(WINAPI *GetSpecificButtonCaps)(_In_ HIDP_REPORT_TYPE, _In_opt_ USAGE, _In_opt_ USHORT, _In_opt_ USAGE,
	_Out_writes_to_(*ButtonCapsLength, *ButtonCapsLength) PHIDP_BUTTON_CAPS,
	_Inout_ PUSHORT, _In_ PHIDP_PREPARSED_DATA);

_Must_inspect_result_
_IRQL_requires_max_(PASSIVE_LEVEL)
NTSTATUS __stdcall
HidP_GetSpecificButtonCaps(
	_In_       HIDP_REPORT_TYPE     ReportType,
	_In_opt_   USAGE                UsagePage,      // Optional (0 => ignore)
	_In_opt_   USHORT               LinkCollection, // Optional (0 => ignore)
	_In_opt_   USAGE                Usage,          // Optional (0 => ignore)
	_Out_writes_to_(*ButtonCapsLength, *ButtonCapsLength) PHIDP_BUTTON_CAPS ButtonCaps,
	_Inout_    PUSHORT              ButtonCapsLength,
	_In_       PHIDP_PREPARSED_DATA PreparsedData
)
{
	GetSpecificButtonCaps pF = (GetSpecificButtonCaps)GetProcAddress_(eHidP_GetSpecificButtonCaps);
	if (NULL != pF) return pF(ReportType, UsagePage, LinkCollection, Usage, ButtonCaps, ButtonCapsLength, PreparsedData);
	else return HIDP_STATUS_INTERNAL_ERROR;
}


// * * * * * * *
typedef NTSTATUS(WINAPI *GetButtonCaps)(_In_ HIDP_REPORT_TYPE, _Out_writes_to_(*ButtonCapsLength, *ButtonCapsLength) PHIDP_BUTTON_CAPS,
	_Inout_ PUSHORT, _In_ PHIDP_PREPARSED_DATA);

_Must_inspect_result_
_IRQL_requires_max_(PASSIVE_LEVEL)
NTSTATUS __stdcall
HidP_GetButtonCaps(
	_In_       HIDP_REPORT_TYPE     ReportType,
	_Out_writes_to_(*ButtonCapsLength, *ButtonCapsLength) PHIDP_BUTTON_CAPS ButtonCaps,
	_Inout_    PUSHORT              ButtonCapsLength,
	_In_       PHIDP_PREPARSED_DATA PreparsedData
)
{
	GetButtonCaps pF = (GetButtonCaps)GetProcAddress_(eHidP_GetButtonCaps);
	if (NULL != pF) return pF(ReportType, ButtonCaps, ButtonCapsLength, PreparsedData);
	else return HIDP_STATUS_INTERNAL_ERROR;
}


// * * * * * * *
typedef NTSTATUS(WINAPI *GetSpecificValueCaps)(_In_ HIDP_REPORT_TYPE, _In_opt_ USAGE, _In_opt_ USHORT, _In_opt_ USAGE,
	_Out_writes_to_(*ValueCapsLength, *ValueCapsLength) PHIDP_VALUE_CAPS,
	_Inout_ PUSHORT, _In_ PHIDP_PREPARSED_DATA);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NTSTATUS __stdcall
HidP_GetSpecificValueCaps(
	_In_       HIDP_REPORT_TYPE     ReportType,
	_In_opt_   USAGE                UsagePage,      // Optional (0 => ignore)
	_In_opt_   USHORT               LinkCollection, // Optional (0 => ignore)
	_In_opt_   USAGE                Usage,          // Optional (0 => ignore)
	_Out_writes_to_(*ValueCapsLength, *ValueCapsLength)      PHIDP_VALUE_CAPS     ValueCaps,
	_Inout_    PUSHORT              ValueCapsLength,
	_In_       PHIDP_PREPARSED_DATA PreparsedData
)
{
	GetSpecificValueCaps pF = (GetSpecificValueCaps)GetProcAddress_(eHidP_GetSpecificValueCaps);
	if (NULL != pF) return pF(ReportType, UsagePage, LinkCollection, Usage, ValueCaps, ValueCapsLength, PreparsedData);
	else return HIDP_STATUS_INTERNAL_ERROR;
}


// * * * * * * *
typedef NTSTATUS(WINAPI *GetValueCaps)(_In_ HIDP_REPORT_TYPE,
	_Out_writes_to_(*ValueCapsLength, *ValueCapsLength) PHIDP_VALUE_CAPS,
	_Inout_ PUSHORT, _In_ PHIDP_PREPARSED_DATA);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NTSTATUS __stdcall
HidP_GetValueCaps(
	_In_       HIDP_REPORT_TYPE     ReportType,
	_Out_writes_to_(*ValueCapsLength, *ValueCapsLength) PHIDP_VALUE_CAPS ValueCaps,
	_Inout_    PUSHORT              ValueCapsLength,
	_In_       PHIDP_PREPARSED_DATA PreparsedData
)
{
	GetValueCaps pF = (GetValueCaps)GetProcAddress_(eHidP_GetValueCaps);
	if (NULL != pF) return pF(ReportType, ValueCaps, ValueCapsLength, PreparsedData);
	else return HIDP_STATUS_INTERNAL_ERROR;
}


// * * * * * * *
typedef NTSTATUS(WINAPI *GetExtendedAttributes)(_In_ HIDP_REPORT_TYPE, _In_ USHORT, _In_ PHIDP_PREPARSED_DATA,
	_Out_writes_to_(*LengthAttributes, *LengthAttributes) PHIDP_EXTENDED_ATTRIBUTES,
	_Inout_ PULONG);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NTSTATUS __stdcall
HidP_GetExtendedAttributes(
	_In_      HIDP_REPORT_TYPE            ReportType,
	_In_      USHORT                      DataIndex,
	_In_      PHIDP_PREPARSED_DATA        PreparsedData,
	_Out_writes_to_(*LengthAttributes, *LengthAttributes) PHIDP_EXTENDED_ATTRIBUTES Attributes,
	_Inout_   PULONG                      LengthAttributes
)
{
	GetExtendedAttributes pF = (GetExtendedAttributes)GetProcAddress_(eHidP_GetExtendedAttributes);
	if (NULL != pF) return pF(ReportType, DataIndex, PreparsedData, Attributes, LengthAttributes);
	else return HIDP_STATUS_INTERNAL_ERROR;
}


// * * * * * * *
typedef NTSTATUS(WINAPI *InitializeReportForID)(_In_ HIDP_REPORT_TYPE, _In_ UCHAR, _In_ PHIDP_PREPARSED_DATA,
	_Out_writes_bytes_(ReportLength) PCHAR, _In_ ULONG);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NTSTATUS __stdcall
HidP_InitializeReportForID(
	_In_ HIDP_REPORT_TYPE ReportType,
	_In_ UCHAR ReportID,
	_In_ PHIDP_PREPARSED_DATA PreparsedData,
	_Out_writes_bytes_(ReportLength) PCHAR Report,
	_In_ ULONG ReportLength
)
{
	InitializeReportForID pF = (InitializeReportForID)GetProcAddress_(eHidP_InitializeReportForID);
	if (NULL != pF) return pF(ReportType, ReportID, PreparsedData, Report, ReportLength);
	else return HIDP_STATUS_INTERNAL_ERROR;
}


// * * * * * * *
typedef NTSTATUS(WINAPI *SetData)(_In_ HIDP_REPORT_TYPE,
	_Inout_updates_to_(*DataLength, *DataLength) PHIDP_DATA, _Inout_ PULONG, _In_ PHIDP_PREPARSED_DATA,
	_In_reads_bytes_(ReportLength) PCHAR, _In_ ULONG);

_Must_inspect_result_
NTSTATUS __stdcall
HidP_SetData(
	_In_ HIDP_REPORT_TYPE ReportType,
	_Inout_updates_to_(*DataLength, *DataLength) PHIDP_DATA DataList,
	_Inout_ PULONG DataLength,
	_In_ PHIDP_PREPARSED_DATA PreparsedData,
	_In_reads_bytes_(ReportLength) PCHAR Report,
	_In_ ULONG ReportLength
)
{
	SetData pF = (SetData)GetProcAddress_(eHidP_SetData);
	if (NULL != pF) return pF(ReportType, DataList, DataLength, PreparsedData, Report, ReportLength);
	else return HIDP_STATUS_INTERNAL_ERROR;
}


// * * * * * * *
typedef NTSTATUS(WINAPI *GetData)(_In_ HIDP_REPORT_TYPE,
	_Out_writes_to_(*DataLength, *DataLength) PHIDP_DATA, _Inout_ PULONG, _In_ PHIDP_PREPARSED_DATA,
	_Out_writes_bytes_(ReportLength) PCHAR, _In_ ULONG);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NTSTATUS __stdcall
HidP_GetData(
	_In_ HIDP_REPORT_TYPE ReportType,
	_Out_writes_to_(*DataLength, *DataLength) PHIDP_DATA DataList,
	_Inout_ PULONG DataLength,
	_In_ PHIDP_PREPARSED_DATA PreparsedData,
	_Out_writes_bytes_(ReportLength) PCHAR Report,
	_In_ ULONG ReportLength
)
{
	GetData pF = (GetData)GetProcAddress_(eHidP_GetData);
	if (NULL != pF) return pF(ReportType, DataList, DataLength, PreparsedData, Report, ReportLength);
	else return HIDP_STATUS_INTERNAL_ERROR;
}


// * * * * * * *
typedef ULONG(WINAPI *MaxDataListLength)(_In_ HIDP_REPORT_TYPE, _In_ PHIDP_PREPARSED_DATA);

_IRQL_requires_max_(DISPATCH_LEVEL)
ULONG __stdcall
HidP_MaxDataListLength(
	_In_ HIDP_REPORT_TYPE      ReportType,
	_In_ PHIDP_PREPARSED_DATA  PreparsedData
)
{
	MaxDataListLength pF = (MaxDataListLength)GetProcAddress_(eHidP_MaxDataListLength);
	if (NULL != pF) return pF(ReportType, PreparsedData);
	else return FALSE;
}


// * * * * * * *
typedef NTSTATUS(WINAPI *SetUsages)(_In_ HIDP_REPORT_TYPE, _In_ USAGE, _In_opt_ USHORT,
	_Inout_updates_to_(*UsageLength, *UsageLength) PUSAGE, _Inout_  PULONG, _In_ PHIDP_PREPARSED_DATA,
	_In_reads_bytes_(ReportLength) PCHAR, _In_ ULONG);

_Must_inspect_result_
NTSTATUS __stdcall
HidP_SetUsages(
	_In_ HIDP_REPORT_TYPE    ReportType,
	_In_ USAGE   UsagePage,
	_In_opt_ USHORT  LinkCollection,
	_Inout_updates_to_(*UsageLength, *UsageLength) PUSAGE  UsageList,
	_Inout_  PULONG  UsageLength,
	_In_ PHIDP_PREPARSED_DATA  PreparsedData,
	_In_reads_bytes_(ReportLength) PCHAR   Report,
	_In_ ULONG   ReportLength
)
{
	SetUsages pF = (SetUsages)GetProcAddress_(eHidP_SetUsages);
	if (NULL != pF) return pF(ReportType, UsagePage, LinkCollection, UsageList, UsageLength, PreparsedData, Report, ReportLength);
	else return HIDP_STATUS_INTERNAL_ERROR;
}


// * * * * * * *
typedef NTSTATUS(WINAPI *UnsetUsages)(_In_ HIDP_REPORT_TYPE, _In_ USAGE, _In_opt_ USHORT,
	_Inout_updates_to_(*UsageLength, *UsageLength) PUSAGE, _Inout_  PULONG, _In_ PHIDP_PREPARSED_DATA,
	_In_reads_bytes_(ReportLength) PCHAR, _In_ ULONG);

_Must_inspect_result_
NTSTATUS __stdcall
HidP_UnsetUsages(
	_In_ HIDP_REPORT_TYPE      ReportType,
	_In_ USAGE   UsagePage,
	_In_opt_ USHORT  LinkCollection,
	_Inout_updates_to_(*UsageLength, *UsageLength) PUSAGE  UsageList,
	_Inout_  PULONG  UsageLength,
	_In_ PHIDP_PREPARSED_DATA  PreparsedData,
	_In_reads_bytes_(ReportLength) PCHAR   Report,
	_In_ ULONG   ReportLength
)
{
	UnsetUsages pF = (UnsetUsages)GetProcAddress_(eHidP_UnsetUsages);
	if (NULL != pF) return pF(ReportType, UsagePage, LinkCollection, UsageList, UsageLength, PreparsedData, Report, ReportLength);
	else return HIDP_STATUS_INTERNAL_ERROR;
}


// * * * * * * *
typedef NTSTATUS(WINAPI *GetUsages)(_In_ HIDP_REPORT_TYPE, _In_ USAGE, _In_opt_ USHORT,
	_Out_writes_to_(*UsageLength, *UsageLength)  PUSAGE, _Inout_ PULONG, _In_ PHIDP_PREPARSED_DATA,
	_Out_writes_bytes_(ReportLength) PCHAR, _In_ ULONG);

_Must_inspect_result_
NTSTATUS __stdcall
HidP_GetUsages(
	_In_ HIDP_REPORT_TYPE    ReportType,
	_In_ USAGE   UsagePage,
	_In_opt_ USHORT  LinkCollection,
	_Out_writes_to_(*UsageLength, *UsageLength)  PUSAGE UsageList,
	_Inout_    PULONG UsageLength,
	_In_ PHIDP_PREPARSED_DATA PreparsedData,
	_Out_writes_bytes_(ReportLength)    PCHAR Report,
	_In_ ULONG   ReportLength
)
{
	GetUsages pF = (GetUsages)GetProcAddress_(eHidP_GetUsages);
	if (NULL != pF) return pF(ReportType, UsagePage, LinkCollection, UsageList, UsageLength, PreparsedData, Report, ReportLength);
	else return HIDP_STATUS_INTERNAL_ERROR;
}


// * * * * * * *
typedef NTSTATUS(WINAPI *GetUsagesEx)(_In_ HIDP_REPORT_TYPE, _In_opt_ USHORT,
	_Inout_updates_to_(*UsageLength, *UsageLength) PUSAGE_AND_PAGE, _Inout_ ULONG *, _In_ PHIDP_PREPARSED_DATA,
	_In_reads_bytes_(ReportLength) PCHAR, _In_ ULONG);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NTSTATUS __stdcall
HidP_GetUsagesEx(
	_In_    HIDP_REPORT_TYPE    ReportType,
	_In_opt_  USHORT  LinkCollection, // Optional
	_Inout_updates_to_(*UsageLength, *UsageLength) PUSAGE_AND_PAGE  ButtonList,
	_Inout_   ULONG * UsageLength,
	_In_ PHIDP_PREPARSED_DATA PreparsedData,
	_In_reads_bytes_(ReportLength)   PCHAR   Report,
	_In_ ULONG  ReportLength
)
{
	GetUsagesEx pF = (GetUsagesEx)GetProcAddress_(eHidP_GetUsagesEx);
	if (NULL != pF) return pF(ReportType, LinkCollection, ButtonList, UsageLength, PreparsedData, Report, ReportLength);
	else return HIDP_STATUS_INTERNAL_ERROR;
}


// * * * * * * *
typedef ULONG(WINAPI *MaxUsageListLength)(_In_ HIDP_REPORT_TYPE, _In_opt_ USAGE, _In_ PHIDP_PREPARSED_DATA);

_IRQL_requires_max_(PASSIVE_LEVEL)
ULONG __stdcall
HidP_MaxUsageListLength(
	_In_ HIDP_REPORT_TYPE      ReportType,
	_In_opt_ USAGE                 UsagePage, // Optional
	_In_ PHIDP_PREPARSED_DATA  PreparsedData
)
{
	MaxUsageListLength pF = (MaxUsageListLength)GetProcAddress_(eHidP_MaxUsageListLength);
	if (NULL != pF) return pF(ReportType, UsagePage, PreparsedData);
	else return 0;
}


// * * * * * * *
typedef NTSTATUS(WINAPI *SetUsageValue)(_In_ HIDP_REPORT_TYPE, _In_ USAGE, _In_opt_ USHORT, _In_ USAGE, _In_ ULONG,
	_In_ PHIDP_PREPARSED_DATA, _Inout_updates_bytes_(ReportLength) PCHAR, _In_ ULONG);

_Must_inspect_result_
NTSTATUS __stdcall
HidP_SetUsageValue(
	_In_ HIDP_REPORT_TYPE ReportType,
	_In_ USAGE UsagePage,
	_In_opt_ USHORT LinkCollection,
	_In_ USAGE Usage,
	_In_ ULONG UsageValue,
	_In_ PHIDP_PREPARSED_DATA PreparsedData,
	_Inout_updates_bytes_(ReportLength) PCHAR Report,
	_In_ ULONG ReportLength
)
{
	SetUsageValue pF = (SetUsageValue)GetProcAddress_(eHidP_SetUsageValue);
	if (NULL != pF) return pF(ReportType, UsagePage, LinkCollection, Usage, UsageValue, PreparsedData, Report, ReportLength);
	else return HIDP_STATUS_INTERNAL_ERROR;
}


// * * * * * * *
typedef NTSTATUS(WINAPI *SetScaledUsageValue)(_In_ HIDP_REPORT_TYPE, _In_ USAGE, _In_opt_ USHORT, _In_ USAGE, _In_ LONG,
	_In_ PHIDP_PREPARSED_DATA, _Inout_updates_bytes_(ReportLength) PCHAR, _In_ ULONG);

_Must_inspect_result_
NTSTATUS __stdcall
HidP_SetScaledUsageValue(
	_In_ HIDP_REPORT_TYPE ReportType,
	_In_ USAGE UsagePage,
	_In_opt_ USHORT LinkCollection,
	_In_ USAGE Usage,
	_In_ LONG UsageValue,
	_In_ PHIDP_PREPARSED_DATA PreparsedData,
	_Inout_updates_bytes_(ReportLength) PCHAR Report,
	_In_ ULONG ReportLength
)
{
	SetScaledUsageValue pF = (SetScaledUsageValue)GetProcAddress_(eHidP_SetScaledUsageValue);
	if (NULL != pF) return pF(ReportType, UsagePage, LinkCollection, Usage, UsageValue, PreparsedData, Report, ReportLength);
	else return HIDP_STATUS_INTERNAL_ERROR;
}


// * * * * * * *
typedef NTSTATUS(WINAPI *SetUsageValueArray)(_In_ HIDP_REPORT_TYPE, _In_ USAGE, _In_opt_ USHORT, _In_ USAGE,
	_In_reads_bytes_(UsageValueByteLength) PCHAR, _In_ USHORT,
	_In_ PHIDP_PREPARSED_DATA, _Inout_updates_bytes_(ReportLength) PCHAR, _In_ ULONG);

_Must_inspect_result_
NTSTATUS __stdcall
HidP_SetUsageValueArray(
	_In_ HIDP_REPORT_TYPE ReportType,
	_In_ USAGE UsagePage,
	_In_opt_ USHORT LinkCollection,
	_In_ USAGE Usage,
	_In_reads_bytes_(UsageValueByteLength) PCHAR UsageValue,
	_In_ USHORT UsageValueByteLength,
	_In_ PHIDP_PREPARSED_DATA PreparsedData,
	_Inout_updates_bytes_(ReportLength) PCHAR Report,
	_In_ ULONG ReportLength
)
{
	SetUsageValueArray pF = (SetUsageValueArray)GetProcAddress_(eHidP_SetUsageValueArray);
	if (NULL != pF) return pF(ReportType, UsagePage, LinkCollection, Usage, UsageValue, UsageValueByteLength, PreparsedData, Report, ReportLength);
	else return HIDP_STATUS_INTERNAL_ERROR;
}


// * * * * * * *
typedef NTSTATUS(WINAPI *GetUsageValue)(_In_ HIDP_REPORT_TYPE, _In_ USAGE, _In_opt_ USHORT, _In_ USAGE, _Out_ PULONG,
	_In_ PHIDP_PREPARSED_DATA, _In_reads_bytes_(ReportLength) PCHAR, _In_ ULONG);

_Must_inspect_result_
NTSTATUS __stdcall
HidP_GetUsageValue(
	_In_ HIDP_REPORT_TYPE ReportType,
	_In_ USAGE UsagePage,
	_In_opt_ USHORT LinkCollection,
	_In_ USAGE Usage,
	_Out_ PULONG UsageValue,
	_In_ PHIDP_PREPARSED_DATA PreparsedData,
	_In_reads_bytes_(ReportLength) PCHAR Report,
	_In_ ULONG ReportLength
)
{
	GetUsageValue pF = (GetUsageValue)GetProcAddress_(eHidP_GetUsageValue);
	if (NULL != pF) return pF(ReportType, UsagePage, LinkCollection, Usage, UsageValue, PreparsedData, Report, ReportLength);
	else return HIDP_STATUS_INTERNAL_ERROR;
}


// * * * * * * *
typedef NTSTATUS(WINAPI *GetScaledUsageValue)(_In_ HIDP_REPORT_TYPE, _In_ USAGE, _In_opt_ USHORT, _In_ USAGE, _Out_ PLONG,
	_In_ PHIDP_PREPARSED_DATA, _In_reads_bytes_(ReportLength) PCHAR, _In_ ULONG);

_Must_inspect_result_
NTSTATUS __stdcall
HidP_GetScaledUsageValue(
	_In_ HIDP_REPORT_TYPE ReportType,
	_In_ USAGE UsagePage,
	_In_opt_ USHORT LinkCollection,
	_In_ USAGE Usage,
	_Out_ PLONG UsageValue,
	_In_ PHIDP_PREPARSED_DATA PreparsedData,
	_In_reads_bytes_(ReportLength) PCHAR Report,
	_In_ ULONG ReportLength
)
{
	GetScaledUsageValue pF = (GetScaledUsageValue)GetProcAddress_(eHidP_GetScaledUsageValue);
	if (NULL != pF) return pF(ReportType, UsagePage, LinkCollection, Usage, UsageValue, PreparsedData, Report, ReportLength);
	else return HIDP_STATUS_INTERNAL_ERROR;
}


// * * * * * * *
typedef NTSTATUS(WINAPI *GetUsageValueArray)(_In_ HIDP_REPORT_TYPE, _In_ USAGE, _In_opt_ USHORT, _In_ USAGE,
	_Inout_updates_bytes_(UsageValueByteLength) PCHAR, _In_ USHORT,
	_In_ PHIDP_PREPARSED_DATA, _In_reads_bytes_(ReportLength) PCHAR, _In_ ULONG);

_Must_inspect_result_
NTSTATUS __stdcall
HidP_GetUsageValueArray(
	_In_ HIDP_REPORT_TYPE ReportType,
	_In_ USAGE UsagePage,
	_In_opt_ USHORT LinkCollection,
	_In_ USAGE Usage,
	_Inout_updates_bytes_(UsageValueByteLength) PCHAR UsageValue,
	_In_ USHORT UsageValueByteLength,
	_In_ PHIDP_PREPARSED_DATA PreparsedData,
	_In_reads_bytes_(ReportLength) PCHAR Report,
	_In_ ULONG ReportLength
)
{
	GetUsageValueArray pF = (GetUsageValueArray)GetProcAddress_(eHidP_GetUsageValueArray);
	if (NULL != pF) return pF(ReportType, UsagePage, LinkCollection, Usage, UsageValue, UsageValueByteLength, PreparsedData, Report, ReportLength);
	else return HIDP_STATUS_INTERNAL_ERROR;
}


// * * * * * * *
typedef NTSTATUS(WINAPI *UsageListDifference)(_In_reads_(UsageListLength) PUSAGE, _In_reads_(UsageListLength) PUSAGE,
	_Out_writes_(UsageListLength) PUSAGE, _Out_writes_(UsageListLength) PUSAGE, _In_ ULONG);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NTSTATUS __stdcall
HidP_UsageListDifference(
	_In_reads_(UsageListLength) PUSAGE  PreviousUsageList,
	_In_reads_(UsageListLength) PUSAGE  CurrentUsageList,
	_Out_writes_(UsageListLength) PUSAGE  BreakUsageList,
	_Out_writes_(UsageListLength) PUSAGE  MakeUsageList,
	_In_ ULONG    UsageListLength
)
{
	UsageListDifference pF = (UsageListDifference)GetProcAddress_(eHidP_UsageListDifference);
	if (NULL != pF) return pF(PreviousUsageList, CurrentUsageList, BreakUsageList, MakeUsageList, UsageListLength);
	else return HIDP_STATUS_INTERNAL_ERROR;
}


// * * * * * * *
typedef NTSTATUS(WINAPI *UsageAndPageListDifference)(_In_reads_(UsageListLength) PUSAGE_AND_PAGE, _In_reads_(UsageListLength) PUSAGE_AND_PAGE,
	_Out_writes_(UsageListLength) PUSAGE_AND_PAGE, _Out_writes_(UsageListLength) PUSAGE_AND_PAGE, _In_ ULONG);

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
NTSTATUS __stdcall
HidP_UsageAndPageListDifference(
	_In_reads_(UsageListLength) PUSAGE_AND_PAGE PreviousUsageList,
	_In_reads_(UsageListLength) PUSAGE_AND_PAGE CurrentUsageList,
	_Out_writes_(UsageListLength) PUSAGE_AND_PAGE BreakUsageList,
	_Out_writes_(UsageListLength) PUSAGE_AND_PAGE MakeUsageList,
	_In_ ULONG           UsageListLength
)
{
	return HIDP_STATUS_INTERNAL_ERROR; // This was not exported by the x64 System HID.DLL
	/*
	UsageAndPageListDifference pF = (UsageAndPageListDifference)GetProcAddress_(eHidP_UsageAndPageListDifference);
	if (NULL != pF) return pF(PreviousUsageList, CurrentUsageList, BreakUsageList, MakeUsageList, UsageListLength);
	else return HIDP_STATUS_INTERNAL_ERROR;
	*/
}


// * * * * * * *
typedef NTSTATUS(WINAPI *TranslateUsageAndPagesToI8042ScanCodes)(_In_reads_(UsageListLength) PUSAGE_AND_PAGE, _In_ ULONG,
	_In_ HIDP_KEYBOARD_DIRECTION, _Inout_  PHIDP_KEYBOARD_MODIFIER_STATE, _In_ PHIDP_INSERT_SCANCODES, _In_opt_ PVOID);

_Must_inspect_result_
NTSTATUS __stdcall
HidP_TranslateUsageAndPagesToI8042ScanCodes(
	_In_reads_(UsageListLength)     PUSAGE_AND_PAGE ChangedUsageList,
	_In_     ULONG                         UsageListLength,
	_In_     HIDP_KEYBOARD_DIRECTION       KeyAction,
	_Inout_  PHIDP_KEYBOARD_MODIFIER_STATE ModifierState,
	_In_     PHIDP_INSERT_SCANCODES        InsertCodesProcedure,
	_In_opt_ PVOID                         InsertCodesContext
)
{
	return HIDP_STATUS_INTERNAL_ERROR; // This was not exported by the x64 System HID.DLL
	/*
	TranslateUsageAndPagesToI8042ScanCodes pF = (TranslateUsageAndPagesToI8042ScanCodes)GetProcAddress_(eHidP_TranslateUsageAndPagesToI8042ScanCodes);
	if (NULL != pF) return pF(ChangedUsageList, UsageListLength, KeyAction, ModifierState, InsertCodesProcedure, InsertCodesContext);
	else return HIDP_STATUS_INTERNAL_ERROR;
	*/
}


// * * * * * * *
typedef NTSTATUS(WINAPI *TranslateUsagesToI8042ScanCodes)(_In_reads_(UsageListLength) PUSAGE, _In_ ULONG,
	_In_ HIDP_KEYBOARD_DIRECTION, _Inout_ PHIDP_KEYBOARD_MODIFIER_STATE, _In_ PHIDP_INSERT_SCANCODES, _In_opt_ PVOID);

_Must_inspect_result_
NTSTATUS __stdcall
HidP_TranslateUsagesToI8042ScanCodes(
	_In_reads_(UsageListLength)     PUSAGE ChangedUsageList,
	_In_     ULONG                         UsageListLength,
	_In_     HIDP_KEYBOARD_DIRECTION       KeyAction,
	_Inout_  PHIDP_KEYBOARD_MODIFIER_STATE ModifierState,
	_In_     PHIDP_INSERT_SCANCODES        InsertCodesProcedure,
	_In_opt_ PVOID                         InsertCodesContext
)
{

	TranslateUsagesToI8042ScanCodes pF = (TranslateUsagesToI8042ScanCodes)GetProcAddress_(eHidP_TranslateUsagesToI8042ScanCodes);
	if (NULL != pF) return pF(ChangedUsageList, UsageListLength, KeyAction, ModifierState, InsertCodesProcedure, InsertCodesContext);
	else return HIDP_STATUS_INTERNAL_ERROR;
}


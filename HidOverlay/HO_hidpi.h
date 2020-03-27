/*++

Copyright (c) Microsoft Corporation. All rights reserved.

Module Name:

		HIDPI.H

Abstract:

   Public Interface to the HID parsing library.

Environment:

	Kernel & user mode

--*/

#ifndef   __HIDPI_H__
#define   __HIDPI_H__
#include <winapifamily.h>

#pragma region Desktop Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)

#ifdef __cplusplus
extern "C" {
#endif

#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning(disable:4115) // named type definition in parentheses
#pragma warning(disable:4201) // nameless struct/union
#pragma warning(disable:4214) // bit field types other than int

#include <pshpack4.h>

//ADDED BM
#include <windef.h>

	// Please include "hidsdi.h" to use the user space (dll / parser)
	// Please include "hidpddi.h" to use the kernel space parser


	_Must_inspect_result_
		_IRQL_requires_max_(PASSIVE_LEVEL)
		NTSTATUS __stdcall
		HidP_GetCaps(
			_In_      PHIDP_PREPARSED_DATA      PreparsedData,
			_Out_     PHIDP_CAPS                Capabilities
		);
	/*++
	Routine Description:
	   Returns a list of capabilities of a given hid device as described by its
	   preparsed data.

	Arguments:
	   PreparsedData    The preparsed data returned from HIDCLASS.
	   Capabilities     a HIDP_CAPS structure

	Return Value:
	   HIDP_STATUS_SUCCESS
	   HIDP_STATUS_INVALID_PREPARSED_DATA
	--*/

	_Must_inspect_result_
		_IRQL_requires_max_(DISPATCH_LEVEL)
		NTSTATUS __stdcall
		HidP_GetLinkCollectionNodes(
			_Out_writes_to_(*LinkCollectionNodesLength, *LinkCollectionNodesLength)     PHIDP_LINK_COLLECTION_NODE LinkCollectionNodes,
			_Inout_   PULONG                     LinkCollectionNodesLength,
			_In_      PHIDP_PREPARSED_DATA       PreparsedData
		);
	/*++
	Routine Description:
	   Return a list of PHIDP_LINK_COLLECTION_NODEs used to describe the link
	   collection tree of this hid device.  See the above description of
	   struct _HIDP_LINK_COLLECTION_NODE.

	Arguments:
	   LinkCollectionNodes - a caller allocated array into which
					 HidP_GetLinkCollectionNodes will store the information

	   LinKCollectionNodesLength - the caller sets this value to the length of the
					 the array in terms of number of elements.
					 HidP_GetLinkCollectionNodes sets this value to the actual
					 number of elements set. The total number of nodes required to
					 describe this HID device can be found in the
					 NumberLinkCollectionNodes field in the HIDP_CAPS structure.

	--*/

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
		);
	/*++
	Description:
	   HidP_GetButtonCaps returns all the buttons (binary values) that are a part
	   of the given report type for the Hid device represented by the given
	   preparsed data.

	Parameters:
	   ReportType  One of HidP_Input, HidP_Output, or HidP_Feature.

	   UsagePage   A usage page value used to limit the button caps returned to
					those on a given usage page.  If set to 0, this parameter is
					ignored.  Can be used with LinkCollection and Usage parameters
					to further limit the number of button caps structures returned.

	   LinkCollection HIDP_LINK_COLLECTION node array index used to limit the
					  button caps returned to those buttons in a given link
					  collection.  If set to 0, this parameter is
					  ignored.  Can be used with UsagePage and Usage parameters
					  to further limit the number of button caps structures
					  returned.

	   Usage       A usage value used to limit the button caps returned to those
				   with the specified usage value.  If set to 0, this parameter
				   is ignored.  Can be used with LinkCollection and UsagePage
				   parameters to further limit the number of button caps
				   structures returned.

	   ButtonCaps  A _HIDP_BUTTON_CAPS array containing information about all the
				   binary values in the given report.  This buffer is provided by
				   the caller.

	   ButtonCapsLength   As input, this parameter specifies the length of the
					  ButtonCaps parameter (array) in number of array elements.
					  As output, this value is set to indicate how many of those
					  array elements were filled in by the function.  The maximum number of
					  button caps that can be returned is found in the HIDP_CAPS
					  structure.  If HIDP_STATUS_BUFFER_TOO_SMALL is returned,
					  this value contains the number of array elements needed to
					  successfully complete the request.

	   PreparsedData  The preparsed data returned from HIDCLASS.


	Return Value
	HidP_GetSpecificButtonCaps returns the following error codes:
	  HIDP_STATUS_SUCCESS.
	  HIDP_STATUS_INVALID_REPORT_TYPE
	  HIDP_STATUS_INVALID_PREPARSED_DATA
	  HIDP_STATUS_BUFFER_TOO_SMALL (all given entries however have been filled in)
	  HIDP_STATUS_USAGE_NOT_FOUND
	--*/
	_Must_inspect_result_
		_IRQL_requires_max_(PASSIVE_LEVEL)
		NTSTATUS __stdcall
		HidP_GetButtonCaps(
			_In_       HIDP_REPORT_TYPE     ReportType,
			_Out_writes_to_(*ButtonCapsLength, *ButtonCapsLength) PHIDP_BUTTON_CAPS ButtonCaps,
			_Inout_    PUSHORT              ButtonCapsLength,
			_In_       PHIDP_PREPARSED_DATA PreparsedData
		);

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
		);
	/*++
	Description:
	   HidP_GetValueCaps returns all the values (non-binary) that are a part
	   of the given report type for the Hid device represented by the given
	   preparsed data.

	Parameters:
	   ReportType  One of HidP_Input, HidP_Output, or HidP_Feature.

	   UsagePage   A usage page value used to limit the value caps returned to
					those on a given usage page.  If set to 0, this parameter is
					ignored.  Can be used with LinkCollection and Usage parameters
					to further limit the number of value caps structures returned.

	   LinkCollection HIDP_LINK_COLLECTION node array index used to limit the
					  value caps returned to those buttons in a given link
					  collection.  If set to 0, this parameter is
					  ignored.  Can be used with UsagePage and Usage parameters
					  to further limit the number of value caps structures
					  returned.

	   Usage      A usage value used to limit the value caps returned to those
				   with the specified usage value.  If set to 0, this parameter
				   is ignored.  Can be used with LinkCollection and UsagePage
				   parameters to further limit the number of value caps
				   structures returned.

	   ValueCaps  A _HIDP_VALUE_CAPS array containing information about all the
				   non-binary values in the given report.  This buffer is provided
				   by the caller.

	   ValueLength   As input, this parameter specifies the length of the ValueCaps
					  parameter (array) in number of array elements.  As output,
					  this value is set to indicate how many of those array elements
					  were filled in by the function.  The maximum number of
					  value caps that can be returned is found in the HIDP_CAPS
					  structure.  If HIDP_STATUS_BUFFER_TOO_SMALL is returned,
					  this value contains the number of array elements needed to
					  successfully complete the request.

	   PreparsedData  The preparsed data returned from HIDCLASS.


	Return Value
	HidP_GetValueCaps returns the following error codes:
	  HIDP_STATUS_SUCCESS.
	  HIDP_STATUS_INVALID_REPORT_TYPE
	  HIDP_STATUS_INVALID_PREPARSED_DATA
	  HIDP_STATUS_BUFFER_TOO_SMALL (all given entries however have been filled in)
	  HIDP_STATUS_USAGE_NOT_FOUND

	--*/

	_Must_inspect_result_
		_IRQL_requires_max_(DISPATCH_LEVEL)
		NTSTATUS __stdcall
		HidP_GetValueCaps(
			_In_       HIDP_REPORT_TYPE     ReportType,
			_Out_writes_to_(*ValueCapsLength, *ValueCapsLength) PHIDP_VALUE_CAPS ValueCaps,
			_Inout_    PUSHORT              ValueCapsLength,
			_In_       PHIDP_PREPARSED_DATA PreparsedData
		);

	_Must_inspect_result_
		_IRQL_requires_max_(DISPATCH_LEVEL)
		NTSTATUS __stdcall
		HidP_GetExtendedAttributes(
			_In_      HIDP_REPORT_TYPE            ReportType,
			_In_      USHORT                      DataIndex,
			_In_      PHIDP_PREPARSED_DATA        PreparsedData,
			_Out_writes_to_(*LengthAttributes, *LengthAttributes) PHIDP_EXTENDED_ATTRIBUTES Attributes,
			_Inout_   PULONG                      LengthAttributes
		);
	/*++
	Description:
		Given a data index from the value or button capabilities of a given control
		return any extended attributes for the control if any exist.

	Parameters:
		ReportType  One of HidP_Input, HidP_Output, or HidP_Feature.

		DataIndex   The data index for the given control, found in the capabilities
					structure for that control

		PreparsedData   The preparsed data returned from HIDCLASS.

		Attributes  Pointer to a buffer into which the extended attribute data will
					be copied.

		LengthAttributes    Length of the given buffer in bytes.

	Return Value
		HIDP_STATUS_SUCCESS
		HIDP_STATUS_DATA_INDEX_NOT_FOUND
	--*/

	_Must_inspect_result_
		_IRQL_requires_max_(DISPATCH_LEVEL)
		NTSTATUS __stdcall
		HidP_InitializeReportForID(
			_In_ HIDP_REPORT_TYPE ReportType,
			_In_ UCHAR ReportID,
			_In_ PHIDP_PREPARSED_DATA PreparsedData,
			_Out_writes_bytes_(ReportLength) PCHAR Report,
			_In_ ULONG ReportLength
		);
	/*++

	Routine Description:

		Initialize a report based on the given report ID.

	Parameters:

		ReportType  One of HidP_Input, HidP_Output, or HidP_Feature.

		PreparasedData  Preparsed data structure returned by HIDCLASS

		Report      Buffer which to set the data into.

		ReportLength Length of Report...Report should be at least as long as the
					value indicated in the HIDP_CAPS structure for the device and
					the corresponding ReportType

	Return Value

	  HIDP_STATUS_INVALID_REPORT_TYPE    -- if ReportType is not valid.
	  HIDP_STATUS_INVALID_PREPARSED_DATA -- if PreparsedData is not valid
	  HIDP_STATUS_INVALID_REPORT_LENGTH  -- the length of the report packet is not equal
											to the length specified in HIDP_CAPS
											structure for the given ReportType
	  HIDP_STATUS_REPORT_DOES_NOT_EXIST  -- if there are no reports on this device
											for the given ReportType

	--*/

	_Must_inspect_result_
		NTSTATUS __stdcall
		HidP_SetData(
			_In_ HIDP_REPORT_TYPE ReportType,
			_Inout_updates_to_(*DataLength, *DataLength) PHIDP_DATA DataList,
			_Inout_ PULONG DataLength,
			_In_ PHIDP_PREPARSED_DATA PreparsedData,
			_In_reads_bytes_(ReportLength) PCHAR Report,
			_In_ ULONG ReportLength
		);
	/*++

	Routine Description:

		Please Note: Since usage value arrays deal with multiple fields for
					 for one usage value, they cannot be used with HidP_SetData
					 and HidP_GetData.  In this case,
					 HIDP_STATUS_IS_USAGE_VALUE_ARRAY will be returned.

	Parameters:

		ReportType  One of HidP_Input, HidP_Output, or HidP_Feature.

		DataList    Array of HIDP_DATA structures that contains the data values
					that are to be set into the given report

		DataLength  As input, length in array elements of DataList.  As output,
					contains the number of data elements set on successful
					completion or an index into the DataList array to identify
					the faulting HIDP_DATA value if an error code is returned.

		PreparasedData  Preparsed data structure returned by HIDCLASS

		Report      Buffer which to set the data into.

		ReportLength Length of Report...Report should be at least as long as the
					value indicated in the HIDP_CAPS structure for the device and
					the corresponding ReportType

	Return Value
		HidP_SetData returns the following error codes.  The report packet will
			have all the data set up until the HIDP_DATA structure that caused the
			error.  DataLength, in the error case, will return this problem index.

	  HIDP_STATUS_SUCCESS                -- upon successful insertion of all data
											into the report packet.
	  HIDP_STATUS_INVALID_REPORT_TYPE    -- if ReportType is not valid.
	  HIDP_STATUS_INVALID_PREPARSED_DATA -- if PreparsedData is not valid
	  HIDP_STATUS_DATA_INDEX_NOT_FOUND   -- if a HIDP_DATA structure referenced a
											data index that does not exist for this
											device's ReportType
	  HIDP_STATUS_INVALID_REPORT_LENGTH  -- the length of the report packet is not equal
											to the length specified in HIDP_CAPS
											structure for the given ReportType
	  HIDP_STATUS_REPORT_DOES_NOT_EXIST  -- if there are no reports on this device
											for the given ReportType
	  HIDP_STATUS_IS_USAGE_VALUE_ARRAY   -- if one of the HIDP_DATA structures
											references a usage value array.
											DataLength will contain the index into
											the array that was invalid
	  HIDP_STATUS_BUTTON_NOT_PRESSED     -- if a HIDP_DATA structure attempted
											to unset a button that was not already
											set in the Report
	  HIDP_STATUS_INCOMPATIBLE_REPORT_ID -- a HIDP_DATA structure was found with
											a valid index value but is contained
											in a different report than the one
											currently being processed
	  HIDP_STATUS_BUFFER_TOO_SMALL       -- if there are not enough entries in
											a given Main Array Item to report all
											buttons that have been requested to be
											set
	--*/

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
		);
	/*++

	Routine Description:

		Please Note: For obvious reasons HidP_SetData and HidP_GetData will not
		access UsageValueArrays.

	Parameters:
		ReportType  One of HidP_Input, HidP_Output, or HidP_Feature.

		DataList    Array of HIDP_DATA structures that will receive the data
					values that are set in the given report

		DataLength  As input, length in array elements of DataList.  As output,
					contains the number of data elements that were successfully
					set by HidP_GetData.  The maximum size necessary for DataList
					can be determined by calling HidP_MaxDataListLength

		PreparsedData  Preparsed data structure returned by HIDCLASS

		Report      Buffer which to set the data into.

		ReportLength Length of Report...Report should be at least as long as the
					value indicated in the HIDP_CAPS structure for the device and
					the corresponding ReportType

	Return Value
		HidP_GetData returns the following error codes.

	  HIDP_STATUS_SUCCESS                -- upon successful retrieval of all data
											from the report packet.
	  HIDP_STATUS_INVALID_REPORT_TYPE    -- if ReportType is not valid.
	  HIDP_STATUS_INVALID_PREPARSED_DATA -- if PreparsedData is not valid
	  HIDP_STATUS_INVALID_REPORT_LENGTH  -- the length of the report packet is not equal
											to the length specified in HIDP_CAPS
											structure for the given ReportType
	  HIDP_STATUS_REPORT_DOES_NOT_EXIST  -- if there are no reports on this device
											for the given ReportType
	  HIDP_STATUS_BUFFER_TOO_SMALL       -- if there are not enough array entries in
											DataList to store all the indice values
											in the given report.  DataLength will
											contain the number of array entries
											required to hold all data
	--*/

	_IRQL_requires_max_(DISPATCH_LEVEL)
		ULONG __stdcall
		HidP_MaxDataListLength(
			_In_ HIDP_REPORT_TYPE      ReportType,
			_In_ PHIDP_PREPARSED_DATA  PreparsedData
		);
	/*++
	Routine Description:

		This function returns the maximum length of HIDP_DATA elements that
		HidP_GetData could return for the given report type.

	Parameters:

		ReportType  One of HidP_Input, HidP_Output or HidP_Feature.

		PreparsedData    Preparsed data structure returned by HIDCLASS

	Return Value:

		The length of the data list array required for the HidP_GetData function
		call.  If an error occurs (either HIDP_STATUS_INVALID_REPORT_TYPE or
		HIDP_STATUS_INVALID_PREPARSED_DATA), this function returns 0.

	--*/

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
		);
	/*++

	Routine Description:
		This function sets binary values (buttons) in a report.  Given an
		initialized packet of correct length, it modifies the report packet so that
		each element in the given list of usages has been set in the report packet.
		For example, in an output report with 5 LED's, each with a given usage,
		an application could turn on any subset of these lights by placing their
		usages in any order into the usage array (UsageList).  HidP_SetUsages would,
		in turn, set the appropriate bit or add the corresponding byte into the
		HID Main Array Item.

		A properly initialized Report packet is one of the correct byte length,
		and all zeros.

		NOTE: A packet that has already been set with a call to a HidP_Set routine
			  can also be passed in.  This routine then sets processes the UsageList
			  in the same fashion but verifies that the ReportID already set in
			  Report matches the report ID for the given usages.

	Parameters:
		ReportType  One of HidP_Input, HidP_Output or HidP_Feature.

		UsagePage   All of the usages in the usage array, which HidP_SetUsages will
					set in the report, refer to this same usage page.
					If a client wishes to set usages in a report for multiple
					usage pages then that client needs to make multiple calls to
					HidP_SetUsages for each of the usage pages.

		UsageList   A usage array containing the usages that HidP_SetUsages will set in
					the report packet.

		UsageLength The length of the given usage array in array elements.
					The parser will set this value to the position in the usage
					array where it stopped processing.  If successful, UsageLength
					will be unchanged.  In any error condition, this parameter
					reflects how many of the usages in the usage list have
					actually been set by the parser.  This is useful for finding
					the usage in the list which caused the error.

		PreparsedData The preparsed data recevied from HIDCLASS

		Report      The report packet.

		ReportLength   Length of the given report packet...Must be equal to the
					   value reported in the HIDP_CAPS structure for the device
					   and corresponding report type.

	Return Value
		HidP_SetUsages returns the following error codes.  On error, the report packet
		will be correct up until the usage element that caused the error.

	  HIDP_STATUS_SUCCESS                -- upon successful insertion of all usages
											into the report packet.
	  HIDP_STATUS_INVALID_REPORT_TYPE    -- if ReportType is not valid.
	  HIDP_STATUS_INVALID_PREPARSED_DATA -- if PreparsedData is not valid
	  HIDP_STATUS_INVALID_REPORT_LENGTH  -- the length of the report packet is not
											equal to the length specified in
											the HIDP_CAPS structure for the given
											ReportType
	  HIDP_STATUS_REPORT_DOES_NOT_EXIST  -- if there are no reports on this device
											for the given ReportType
	  HIDP_STATUS_INCOMPATIBLE_REPORT_ID -- if a usage was found that exists in a
											different report.  If the report is
											zero-initialized on entry the first
											usage in the list will determine which
											report ID is used.  Otherwise, the
											parser will verify that usage matches
											the passed in report's ID
	  HIDP_STATUS_USAGE_NOT_FOUND        -- if the usage does not exist for any
											report (no matter what the report ID)
											for the given report type.
	  HIDP_STATUS_BUFFER_TOO_SMALL       -- if there are not enough entries in a
											given Main Array Item to list all of
											the given usages.  The caller needs
											to split his request into more than
											one call
	--*/

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
		);
	/*++

	Routine Description:
		This function unsets (turns off) binary values (buttons) in the report.  Given
		an initialized packet of correct length, it modifies the report packet so
		that each element in the given list of usages has been unset in the
		report packet.

		This function is the "undo" operation for SetUsages.  If the given usage
		is not already set in the Report, it will return an error code of
		HIDP_STATUS_BUTTON_NOT_PRESSED.  If the button is pressed, HidP_UnsetUsages
		will unset the appropriate bit or remove the corresponding index value from
		the HID Main Array Item.

		A properly initialized Report packet is one of the correct byte length,
		and all zeros..

		NOTE: A packet that has already been set with a call to a HidP_Set routine
			  can also be passed in.  This routine then processes the UsageList
			  in the same fashion but verifies that the ReportID already set in
			  Report matches the report ID for the given usages.

	Parameters:
		ReportType  One of HidP_Input, HidP_Output or HidP_Feature.

		UsagePage   All of the usages in the usage array, which HidP_UnsetUsages will
					unset in the report, refer to this same usage page.
					If a client wishes to unset usages in a report for multiple
					usage pages then that client needs to make multiple calls to
					HidP_UnsetUsages for each of the usage pages.

		UsageList   A usage array containing the usages that HidP_UnsetUsages will
					unset in the report packet.

		UsageLength The length of the given usage array in array elements.
					The parser will set this value to the position in the usage
					array where it stopped processing.  If successful, UsageLength
					will be unchanged.  In any error condition, this parameter
					reflects how many of the usages in the usage list have
					actually been unset by the parser.  This is useful for finding
					the usage in the list which caused the error.

		PreparsedData The preparsed data recevied from HIDCLASS

		Report      The report packet.

		ReportLength   Length of the given report packet...Must be equal to the
					   value reported in the HIDP_CAPS structure for the device
					   and corresponding report type.

	Return Value
		HidP_UnsetUsages returns the following error codes.  On error, the report
		packet will be correct up until the usage element that caused the error.

	  HIDP_STATUS_SUCCESS                -- upon successful "unsetting" of all usages
											in the report packet.
	  HIDP_STATUS_INVALID_REPORT_TYPE    -- if ReportType is not valid.
	  HIDP_STATUS_INVALID_PREPARSED_DATA -- if PreparsedData is not valid
	  HIDP_STATUS_INVALID_REPORT_LENGTH  -- the length of the report packet is not
											equal to the length specified in
											the HIDP_CAPS structure for the given
											ReportType
	  HIDP_STATUS_REPORT_DOES_NOT_EXIST  -- if there are no reports on this device
											for the given ReportType
	  HIDP_STATUS_INCOMPATIBLE_REPORT_ID -- if a usage was found that exists in a
											different report.  If the report is
											zero-initialized on entry the first
											usage in the list will determine which
											report ID is used.  Otherwise, the
											parser will verify that usage matches
											the passed in report's ID
	  HIDP_STATUS_USAGE_NOT_FOUND        -- if the usage does not exist for any
											report (no matter what the report ID)
											for the given report type.
	  HIDP_STATUS_BUTTON_NOT_PRESSED     -- if a usage corresponds to a button that
											is not already set in the given report
	--*/

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
		);
	/*++

	Routine Description:
		This function returns the binary values (buttons) that are set in a HID
		report.  Given a report packet of correct length, it searches the report
		packet for each usage for the given usage page and returns them in the
		usage list.

	Parameters:
		ReportType One of HidP_Input, HidP_Output or HidP_Feature.

		UsagePage  All of the usages in the usage list, which HidP_GetUsages will
				   retrieve in the report, refer to this same usage page.
				   If the client wishes to get usages in a packet for multiple
				   usage pages then that client needs to make multiple calls
				   to HidP_GetUsages.

		LinkCollection  An optional value which can limit which usages are returned
						in the UsageList to those usages that exist in a specific
						LinkCollection.  A non-zero value indicates the index into
						the HIDP_LINK_COLLECITON_NODE list returned by
						HidP_GetLinkCollectionNodes of the link collection the
						usage should belong to.  A value of 0 indicates this
						should value be ignored.

		UsageList  The usage array that will contain all the usages found in
				   the report packet.

		UsageLength The length of the given usage array in array elements.
					On input, this value describes the length of the usage list.
					On output, HidP_GetUsages sets this value to the number of
					usages that was found.  Use HidP_MaxUsageListLength to
					determine the maximum length needed to return all the usages
					that a given report packet may contain.

		PreparsedData Preparsed data structure returned by HIDCLASS

		Report       The report packet.

		ReportLength  Length (in bytes) of the given report packet


	Return Value
		HidP_GetUsages returns the following error codes:

	  HIDP_STATUS_SUCCESS                -- upon successfully retrieving all the
											usages from the report packet
	  HIDP_STATUS_INVALID_REPORT_TYPE    -- if ReportType is not valid.
	  HIDP_STATUS_INVALID_PREPARSED_DATA -- if PreparsedData is not valid
	  HIDP_STATUS_INVALID_REPORT_LENGTH  -- the length of the report packet is not
											equal to the length specified in
											the HIDP_CAPS structure for the given
											ReportType
	  HIDP_STATUS_REPORT_DOES_NOT_EXIST  -- if there are no reports on this device
											for the given ReportType
	  HIDP_STATUS_BUFFER_TOO_SMALL       -- if the UsageList is not big enough to
											hold all the usages found in the report
											packet.  If this is returned, the buffer
											will contain UsageLength number of
											usages.  Use HidP_MaxUsageListLength to
											find the maximum length needed
	  HIDP_STATUS_INCOMPATIBLE_REPORT_ID -- if no usages were found but usages
											that match the UsagePage and
											LinkCollection specified could be found
											in a report with a different report ID
	  HIDP_STATUS_USAGE_NOT_FOUND        -- if there are no usages in a reports for
											the device and ReportType that match the
											UsagePage and LinkCollection that were
											specified
	--*/

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
		);

	/*++

	Routine Description:
		This function returns the binary values (buttons) in a HID report.
		Given a report packet of correct length, it searches the report packet
		for all buttons and returns the UsagePage and Usage for each of the buttons
		it finds.

	Parameters:
		ReportType  One of HidP_Input, HidP_Output or HidP_Feature.

		LinkCollection  An optional value which can limit which usages are returned
						in the ButtonList to those usages that exist in a specific
						LinkCollection.  A non-zero value indicates the index into
						the HIDP_LINK_COLLECITON_NODE list returned by
						HidP_GetLinkCollectionNodes of the link collection the
						usage should belong to.  A value of 0 indicates this
						should value be ignored.

		ButtonList  An array of USAGE_AND_PAGE structures describing all the
					buttons currently ``down'' in the device.

		UsageLength The length of the given array in terms of elements.
					On input, this value describes the length of the list.  On
					output, HidP_GetUsagesEx sets this value to the number of
					usages that were found.  Use HidP_MaxUsageListLength to
					determine the maximum length needed to return all the usages
					that a given report packet may contain.

		PreparsedData Preparsed data returned by HIDCLASS

		Report       The report packet.

		ReportLength Length (in bytes) of the given report packet.


	Return Value
		HidP_GetUsagesEx returns the following error codes:

	  HIDP_STATUS_SUCCESS                -- upon successfully retrieving all the
											usages from the report packet
	  HIDP_STATUS_INVALID_REPORT_TYPE    -- if ReportType is not valid.
	  HIDP_STATUS_INVALID_PREPARSED_DATA -- if PreparsedData is not valid
	  HIDP_STATUS_INVALID_REPORT_LENGTH  -- the length of the report packet is not
											equal to the length specified in
											the HIDP_CAPS structure for the given
											ReportType
	  HIDP_STATUS_REPORT_DOES_NOT_EXIST  -- if there are no reports on this device
											for the given ReportType
	  HIDP_STATUS_BUFFER_TOO_SMALL       -- if ButtonList is not big enough to
											hold all the usages found in the report
											packet.  If this is returned, the buffer
											will contain UsageLength number of
											usages.  Use HidP_MaxUsageListLength to
											find the maximum length needed
	  HIDP_STATUS_INCOMPATIBLE_REPORT_ID -- if no usages were found but usages
											that match the specified LinkCollection
											exist in report with a different report
											ID.
	  HIDP_STATUS_USAGE_NOT_FOUND        -- if there are no usages in any reports that
											match the LinkCollection parameter
	--*/

	_IRQL_requires_max_(PASSIVE_LEVEL)
		ULONG __stdcall
		HidP_MaxUsageListLength(
			_In_ HIDP_REPORT_TYPE      ReportType,
			_In_opt_ USAGE                 UsagePage, // Optional
			_In_ PHIDP_PREPARSED_DATA  PreparsedData
		);
	/*++
	Routine Description:
		This function returns the maximum number of usages that a call to
		HidP_GetUsages or HidP_GetUsagesEx could return for a given HID report.
		If calling for number of usages returned by HidP_GetUsagesEx, use 0 as
		the UsagePage value.

	Parameters:
		ReportType  One of HidP_Input, HidP_Output or HidP_Feature.

		UsagePage   Specifies the optional UsagePage to query for.  If 0, will
					return all the maximum number of usage values that could be
					returned for a given ReportType.   If non-zero, will return
					the maximum number of usages that would be returned for the
					ReportType with the given UsagePage.

		PreparsedData Preparsed data returned from HIDCLASS

	Return Value:
		The length of the usage list array required for the HidP_GetUsages or
		HidP_GetUsagesEx function call.  If an error occurs (such as
		HIDP_STATUS_INVALID_REPORT_TYPE or HIDP_INVALID_PREPARSED_DATA, this
		returns 0.
	--*/

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
		);
	/*++
	Description:
		HidP_SetUsageValue inserts a value into the HID Report Packet in the field
		corresponding to the given usage page and usage.  HidP_SetUsageValue
		casts this value to the appropriate bit length.  If a report packet
		contains two different fields with the same Usage and UsagePage,
		they can be distinguished with the optional LinkCollection field value.
		Using this function sets the raw value into the report packet with
		no checking done as to whether it actually falls within the logical
		minimum/logical maximum range.  Use HidP_SetScaledUsageValue for this...

		NOTE: Although the UsageValue parameter is a ULONG, any casting that is
			  done will preserve or sign-extend the value.  The value being set
			  should be considered a LONG value and will be treated as such by
			  this function.

	Parameters:

		ReportType  One of HidP_Output or HidP_Feature.

		UsagePage   The usage page to which the given usage refers.

		LinkCollection  (Optional)  This value can be used to differentiate
									between two fields that may have the same
									UsagePage and Usage but exist in different
									collections.  If the link collection value
									is zero, this function will set the first field
									it finds that matches the usage page and
									usage.

		Usage       The usage whose value HidP_SetUsageValue will set.

		UsageValue  The raw value to set in the report buffer.  This value must be within
					the logical range or if a NULL value this value should be the
					most negative value that can be represented by the number of bits
					for this field.

		PreparsedData The preparsed data returned for HIDCLASS

		Report      The report packet.

		ReportLength Length (in bytes) of the given report packet.


	Return Value:
		HidP_SetUsageValue returns the following error codes:

	  HIDP_STATUS_SUCCESS                -- upon successfully setting the value
											in the report packet
	  HIDP_STATUS_INVALID_REPORT_TYPE    -- if ReportType is not valid.
	  HIDP_STATUS_INVALID_PREPARSED_DATA -- if PreparsedData is not valid
	  HIDP_STATUS_INVALID_REPORT_LENGTH  -- the length of the report packet is not
											equal to the length specified in
											the HIDP_CAPS structure for the given
											ReportType
	  HIDP_STATUS_REPORT_DOES_NOT_EXIST  -- if there are no reports on this device
											for the given ReportType
	  HIDP_STATUS_INCOMPATIBLE_REPORT_ID -- the specified usage page, usage and
											link collection exist but exists in
											a report with a different report ID
											than the report being passed in.  To
											set this value, call HidP_SetUsageValue
											again with a zero-initizialed report
											packet
	  HIDP_STATUS_USAGE_NOT_FOUND        -- if the usage page, usage, and link
											collection combination does not exist
											in any reports for this ReportType
	--*/
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
		);

	/*++
	Description:
		HidP_SetScaledUsageValue inserts the UsageValue into the HID report packet
		in the field corresponding to the given usage page and usage.  If a report
		packet contains two different fields with the same Usage and UsagePage,
		they can be distinguished with the optional LinkCollection field value.

		If the specified field has a defined physical range, this function converts
		the physical value specified to the corresponding logical value for the
		report.  If a physical value does not exist, the function will verify that
		the value specified falls within the logical range and set according.

		If the range checking fails but the field has NULL values, the function will
		set the field to the defined NULL value (most negative number possible) and
		return HIDP_STATUS_NULL.  In other words, use this function to set NULL
		values for a given field by passing in a value that falls outside the
		physical range if it is defined or the logical range otherwise.

		If the field does not support NULL values, an out of range error will be
		returned instead.

	Parameters:

		ReportType  One of HidP_Output or HidP_Feature.

		UsagePage   The usage page to which the given usage refers.

		LinkCollection  (Optional)  This value can be used to differentiate
									between two fields that may have the same
									UsagePage and Usage but exist in different
									collections.  If the link collection value
									is zero, this function will set the first field
									it finds that matches the usage page and
									usage.

		Usage       The usage whose value HidP_SetScaledUsageValue will set.

		UsageValue  The value to set in the report buffer.  See the routine
					description above for the different interpretations of this
					value

		PreparsedData The preparsed data returned from HIDCLASS

		Report      The report packet.

		ReportLength Length (in bytes) of the given report packet.


	Return Value:
	   HidP_SetScaledUsageValue returns the following error codes:

	  HIDP_STATUS_SUCCESS                -- upon successfully setting the value
											in the report packet
	  HIDP_STATUS_NULL                   -- upon successfully setting the value
											in the report packet as a NULL value
	  HIDP_STATUS_INVALID_REPORT_TYPE    -- if ReportType is not valid.
	  HIDP_STATUS_INVALID_PREPARSED_DATA -- if PreparsedData is not valid
	  HIDP_STATUS_INVALID_REPORT_LENGTH  -- the length of the report packet is not
											equal to the length specified in
											the HIDP_CAPS structure for the given
											ReportType
	  HIDP_STATUS_VALUE_OUT_OF_RANGE     -- if the value specified failed to fall
											within the physical range if it exists
											or within the logical range otherwise
											and the field specified by the usage
											does not allow NULL values
	  HIDP_STATUS_BAD_LOG_PHY_VALUES     -- if the field has a physical range but
											either the logical range is invalid
											(max <= min) or the physical range is
											invalid
	  HIDP_STATUS_INCOMPATIBLE_REPORT_ID -- the specified usage page, usage and
											link collection exist but exists in
											a report with a different report ID
											than the report being passed in.  To
											set this value, call
											HidP_SetScaledUsageValue again with
											a zero-initialized report packet
	  HIDP_STATUS_USAGE_NOT_FOUND        -- if the usage page, usage, and link
											collection combination does not exist
											in any reports for this ReportType
	--*/
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
		);

	/*++
	Routine Descripton:
		A usage value array occurs when the last usage in the list of usages
		describing a main item must be repeated because there are less usages defined
		than there are report counts declared for the given main item.  In this case
		a single value cap is allocated for that usage and the report count of that
		value cap is set to reflect the number of fields to which that usage refers.

		HidP_SetUsageValueArray sets the raw bits for that usage which spans
		more than one field in a report.

		NOTE: This function currently does not support value arrays where the
			  ReportSize for each of the fields in the array is not a multiple
			  of 8 bits.

			  The UsageValue buffer should have the values set as they would appear
			  in the report buffer.  If this function supported non 8-bit multiples
			  for the ReportSize then caller should format the input buffer so that
			  each new value begins at the bit immediately following the last bit
			  of the previous value

	Parameters:

		ReportType  One of HidP_Output or HidP_Feature.

		UsagePage   The usage page to which the given usage refers.

		LinkCollection  (Optional)  This value can be used to differentiate
									between two fields that may have the same
									UsagePage and Usage but exist in different
									collections.  If the link collection value
									is zero, this function will set the first field
									it finds that matches the usage page and
									usage.

		Usage       The usage whose value array HidP_SetUsageValueArray will set.

		UsageValue  The buffer with the values to set into the value array.
					The number of BITS required is found by multiplying the
					BitSize and ReportCount fields of the Value Cap for this
					control.  The least significant bit of this control found in the
					given report will be placed in the least significan bit location
					of the array given (little-endian format), regardless of whether
					or not the field is byte alligned or if the BitSize is a multiple
					of sizeof (CHAR).

					See the above note for current implementation limitations.

		UsageValueByteLength  Length of the UsageValue buffer (in bytes)

		PreparsedData The preparsed data returned from HIDCLASS

		Report      The report packet.

		ReportLength Length (in bytes) of the given report packet.


	Return Value:
	  HIDP_STATUS_SUCCESS                -- upon successfully setting the value
											array in the report packet
	  HIDP_STATUS_INVALID_REPORT_TYPE    -- if ReportType is not valid.
	  HIDP_STATUS_INVALID_PREPARSED_DATA -- if PreparsedData is not valid
	  HIDP_STATUS_INVALID_REPORT_LENGTH  -- the length of the report packet is not
											equal to the length specified in
											the HIDP_CAPS structure for the given
											ReportType
	  HIDP_STATUS_REPORT_DOES_NOT_EXIST  -- if there are no reports on this device
											for the given ReportType
	  HIDP_STATUS_NOT_VALUE_ARRAY        -- if the control specified is not a
											value array -- a value array will have
											a ReportCount field in the
											HIDP_VALUE_CAPS structure that is > 1
											Use HidP_SetUsageValue instead
	  HIDP_STATUS_BUFFER_TOO_SMALL       -- if the size of the passed in buffer with
											the values to set is too small (ie. has
											fewer values than the number of fields in
											the array
	  HIDP_STATUS_NOT_IMPLEMENTED        -- if the usage value array has field sizes
											that are not multiples of 8 bits, this
											error code is returned since the function
											currently does not handle setting into
											such arrays.
	  HIDP_STATUS_INCOMPATIBLE_REPORT_ID -- the specified usage page, usage and
											link collection exist but exists in
											a report with a different report ID
											than the report being passed in.  To
											set this value, call
											HidP_SetUsageValueArray again with
											a zero-initialized report packet
	  HIDP_STATUS_USAGE_NOT_FOUND        -- if the usage page, usage, and link
											collection combination does not exist
											in any reports for this ReportType
	--*/

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
		);

	/*
	Description
		HidP_GetUsageValue retrieves the value from the HID Report for the usage
		specified by the combination of usage page, usage and link collection.
		If a report packet contains two different fields with the same
		Usage and UsagePage, they can be distinguished with the optional
		LinkCollection field value.

	Parameters:

		ReportType  One of HidP_Input or HidP_Feature.

		UsagePage   The usage page to which the given usage refers.

		LinkCollection  (Optional)  This value can be used to differentiate
									between two fields that may have the same
									UsagePage and Usage but exist in different
									collections.  If the link collection value
									is zero, this function will set the first field
									it finds that matches the usage page and
									usage.

		Usage       The usage whose value HidP_GetUsageValue will retrieve

		UsageValue  The raw value that is set for the specified field in the report
					buffer. This value will either fall within the logical range
					or if NULL values are allowed, a number outside the range to
					indicate a NULL

		PreparsedData The preparsed data returned for HIDCLASS

		Report      The report packet.

		ReportLength Length (in bytes) of the given report packet.


	Return Value:
		HidP_GetUsageValue returns the following error codes:

	  HIDP_STATUS_SUCCESS                -- upon successfully retrieving the value
											from the report packet
	  HIDP_STATUS_INVALID_REPORT_TYPE    -- if ReportType is not valid.
	  HIDP_STATUS_INVALID_PREPARSED_DATA -- if PreparsedData is not valid
	  HIDP_STATUS_INVALID_REPORT_LENGTH  -- the length of the report packet is not
											equal to the length specified in
											the HIDP_CAPS structure for the given
											ReportType
	  HIDP_STATUS_REPORT_DOES_NOT_EXIST  -- if there are no reports on this device
											for the given ReportType
	  HIDP_STATUS_INCOMPATIBLE_REPORT_ID -- the specified usage page, usage and
											link collection exist but exists in
											a report with a different report ID
											than the report being passed in.  To
											set this value, call HidP_GetUsageValue
											again with a different report packet
	  HIDP_STATUS_USAGE_NOT_FOUND        -- if the usage page, usage, and link
											collection combination does not exist
											in any reports for this ReportType
	--*/

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
		);

	/*++
	Description
		HidP_GetScaledUsageValue retrieves a UsageValue from the HID report packet
		in the field corresponding to the given usage page and usage.  If a report
		packet contains two different fields with the same Usage and UsagePage,
		they can be distinguished with the optional LinkCollection field value.

		If the specified field has a defined physical range, this function converts
		the logical value that exists in the report packet to the corresponding
		physical value.  If a physical range does not exist, the function will
		return the logical value.  This function will check to verify that the
		logical value in the report falls within the declared logical range.

		When doing the conversion between logical and physical values, this
		function assumes a linear extrapolation between the physical max/min and
		the logical max/min. (Where logical is the values reported by the device
		and physical is the value returned by this function).  If the data field
		size is less than 32 bits, then HidP_GetScaledUsageValue will sign extend
		the value to 32 bits.

		If the range checking fails but the field has NULL values, the function
		will set UsageValue to 0 and return HIDP_STATUS_NULL.  Otherwise, it
		returns a HIDP_STATUS_OUT_OF_RANGE error.

	Parameters:

		ReportType  One of HidP_Output or HidP_Feature.

		UsagePage   The usage page to which the given usage refers.

		LinkCollection  (Optional)  This value can be used to differentiate
									between two fields that may have the same
									UsagePage and Usage but exist in different
									collections.  If the link collection value
									is zero, this function will retrieve the first
									field it finds that matches the usage page
									and usage.

		Usage       The usage whose value HidP_GetScaledUsageValue will retrieve

		UsageValue  The value retrieved from the report buffer.  See the routine
					description above for the different interpretations of this
					value

		PreparsedData The preparsed data returned from HIDCLASS

		Report      The report packet.

		ReportLength Length (in bytes) of the given report packet.


	Return Value:
	   HidP_GetScaledUsageValue returns the following error codes:

	  HIDP_STATUS_SUCCESS                -- upon successfully retrieving the value
											from the report packet
	  HIDP_STATUS_NULL                   -- if the report packet had a NULL value
											set
	  HIDP_STATUS_INVALID_REPORT_TYPE    -- if ReportType is not valid.
	  HIDP_STATUS_INVALID_PREPARSED_DATA -- if PreparsedData is not valid
	  HIDP_STATUS_INVALID_REPORT_LENGTH  -- the length of the report packet is not
											equal to the length specified in
											the HIDP_CAPS structure for the given
											ReportType
	  HIDP_STATUS_VALUE_OUT_OF_RANGE     -- if the value retrieved from the packet
											falls outside the logical range and
											the field does not support NULL values
	  HIDP_STATUS_BAD_LOG_PHY_VALUES     -- if the field has a physical range but
											either the logical range is invalid
											(max <= min) or the physical range is
											invalid
	  HIDP_STATUS_INCOMPATIBLE_REPORT_ID -- the specified usage page, usage and
											link collection exist but exists in
											a report with a different report ID
											than the report being passed in.  To
											set this value, call
											HidP_GetScaledUsageValue with a
											different report packet
	  HIDP_STATUS_USAGE_NOT_FOUND        -- if the usage page, usage, and link
											collection combination does not exist
											in any reports for this ReportType
	--*/
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
		);

	/*++
	Routine Descripton:
		A usage value array occurs when the last usage in the list of usages
		describing a main item must be repeated because there are less usages defined
		than there are report counts declared for the given main item.  In this case
		a single value cap is allocated for that usage and the report count of that
		value cap is set to reflect the number of fields to which that usage refers.

		HidP_GetUsageValueArray returns the raw bits for that usage which spans
		more than one field in a report.

		NOTE: This function currently does not support value arrays where the
			  ReportSize for each of the fields in the array is not a multiple
			  of 8 bits.

			  The UsageValue buffer will have the raw values as they are set
			  in the report packet.

	Parameters:

		ReportType  One of HidP_Input, HidP_Output or HidP_Feature.

		UsagePage   The usage page to which the given usage refers.

		LinkCollection  (Optional)  This value can be used to differentiate
									between two fields that may have the same
									UsagePage and Usage but exist in different
									collections.  If the link collection value
									is zero, this function will set the first field
									it finds that matches the usage page and
									usage.

	   Usage       The usage whose value HidP_GetUsageValueArray will retreive.

	   UsageValue  A pointer to an array of characters where the value will be
				   placed.  The number of BITS required is found by multiplying the
				   BitSize and ReportCount fields of the Value Cap for this
				   control.  The least significant bit of this control found in the
				   given report will be placed in the least significant bit location
				   of the buffer (little-endian format), regardless of whether
				   or not the field is byte aligned or if the BitSize is a multiple
				   of sizeof (CHAR).

				   See note above about current implementation limitations

	   UsageValueByteLength
				   the length of the given UsageValue buffer.

	   PreparsedData The preparsed data returned by the HIDCLASS

	   Report      The report packet.

	   ReportLength   Length of the given report packet.

	Return Value:

	  HIDP_STATUS_SUCCESS                -- upon successfully retrieving the value
											from the report packet
	  HIDP_STATUS_INVALID_REPORT_TYPE    -- if ReportType is not valid.
	  HIDP_STATUS_INVALID_PREPARSED_DATA -- if PreparsedData is not valid
	  HIDP_STATUS_INVALID_REPORT_LENGTH  -- the length of the report packet is not
											equal to the length specified in
											the HIDP_CAPS structure for the given
											ReportType
	  HIDP_STATUS_NOT_VALUE_ARRAY        -- if the control specified is not a
											value array -- a value array will have
											a ReportCount field in the
											HIDP_VALUE_CAPS structure that is > 1
											Use HidP_GetUsageValue instead
	  HIDP_STATUS_BUFFER_TOO_SMALL       -- if the size of the passed in buffer in
											which to return the array is too small
											(ie. has fewer values than the number of
											fields in the array
	  HIDP_STATUS_NOT_IMPLEMENTED        -- if the usage value array has field sizes
											that are not multiples of 8 bits, this
											error code is returned since the function
											currently does not handle getting values
											from such arrays.
	  HIDP_STATUS_INCOMPATIBLE_REPORT_ID -- the specified usage page, usage and
											link collection exist but exists in
											a report with a different report ID
											than the report being passed in.  To
											set this value, call
											HidP_GetUsageValueArray with a
											different report packet
	  HIDP_STATUS_USAGE_NOT_FOUND        -- if the usage page, usage, and link
											collection combination does not exist
											in any reports for this ReportType
	--*/

	_Must_inspect_result_
		_IRQL_requires_max_(DISPATCH_LEVEL)
		NTSTATUS __stdcall
		HidP_UsageListDifference(
			_In_reads_(UsageListLength) PUSAGE  PreviousUsageList,
			_In_reads_(UsageListLength) PUSAGE  CurrentUsageList,
			_Out_writes_(UsageListLength) PUSAGE  BreakUsageList,
			_Out_writes_(UsageListLength) PUSAGE  MakeUsageList,
			_In_ ULONG    UsageListLength
		);
	/*++
	Routine Description:
		This function will return the difference between a two lists of usages
		(as might be returned from HidP_GetUsages),  In other words, it will return
		return a list of usages that are in the current list but not the previous
		list as well as a list of usages that are in the previous list but not
		the current list.

	Parameters:

		PreviousUsageList   The list of usages before.
		CurrentUsageList    The list of usages now.
		BreakUsageList      Previous - Current.
		MakeUsageList       Current - Previous.
		UsageListLength     Represents the length of the usage lists in array
							elements.  If comparing two lists with a differing
							number of array elements, this value should be
							the size of the larger of the two lists.  Any
							zero found with a list indicates an early termination
							of the list and any usages found after the first zero
							will be ignored.
	--*/

	_Must_inspect_result_
		_IRQL_requires_max_(DISPATCH_LEVEL)
		NTSTATUS __stdcall
		HidP_UsageAndPageListDifference(
			_In_reads_(UsageListLength) PUSAGE_AND_PAGE PreviousUsageList,
			_In_reads_(UsageListLength) PUSAGE_AND_PAGE CurrentUsageList,
			_Out_writes_(UsageListLength) PUSAGE_AND_PAGE BreakUsageList,
			_Out_writes_(UsageListLength) PUSAGE_AND_PAGE MakeUsageList,
			_In_ ULONG           UsageListLength
		);

	//
	// A call back function to give the i8042 scan codes to the caller of
	// the below translation function.
	//
	typedef BOOLEAN(*PHIDP_INSERT_SCANCODES) (
		_In_opt_ PVOID Context,  // Some caller supplied context.
		_In_reads_bytes_(Length) PCHAR NewScanCodes, // A list of i8042 scan codes.
		_In_ ULONG Length // the length of the scan codes.
		);

	_Must_inspect_result_
		NTSTATUS __stdcall
		HidP_TranslateUsageAndPagesToI8042ScanCodes(
			_In_reads_(UsageListLength)     PUSAGE_AND_PAGE ChangedUsageList,
			_In_     ULONG                         UsageListLength,
			_In_     HIDP_KEYBOARD_DIRECTION       KeyAction,
			_Inout_  PHIDP_KEYBOARD_MODIFIER_STATE ModifierState,
			_In_     PHIDP_INSERT_SCANCODES        InsertCodesProcedure,
			_In_opt_ PVOID                         InsertCodesContext
		);
	/*++
	Routine Description:
	Parameters:
	--*/
	_Must_inspect_result_
		NTSTATUS __stdcall
		HidP_TranslateUsagesToI8042ScanCodes(
			_In_reads_(UsageListLength)     PUSAGE ChangedUsageList,
			_In_     ULONG                         UsageListLength,
			_In_     HIDP_KEYBOARD_DIRECTION       KeyAction,
			_Inout_  PHIDP_KEYBOARD_MODIFIER_STATE ModifierState,
			_In_     PHIDP_INSERT_SCANCODES        InsertCodesProcedure,
			_In_opt_ PVOID                         InsertCodesContext
		);
	/*++
	Routine Description:
	Parameters:
	--*/



	//
	// Define NT Status codes with Facility Code of FACILITY_HID_ERROR_CODE
	//

	// FACILITY_HID_ERROR_CODE defined in ntstatus.h
#ifndef FACILITY_HID_ERROR_CODE
#define FACILITY_HID_ERROR_CODE 0x11
#endif

#define HIDP_ERROR_CODES(SEV, CODE) \
        ((NTSTATUS) (((SEV) << 28) | (FACILITY_HID_ERROR_CODE << 16) | (CODE)))

#define HIDP_STATUS_SUCCESS                  (HIDP_ERROR_CODES(0x0,0))
#define HIDP_STATUS_NULL                     (HIDP_ERROR_CODES(0x8,1))
#define HIDP_STATUS_INVALID_PREPARSED_DATA   (HIDP_ERROR_CODES(0xC,1))
#define HIDP_STATUS_INVALID_REPORT_TYPE      (HIDP_ERROR_CODES(0xC,2))
#define HIDP_STATUS_INVALID_REPORT_LENGTH    (HIDP_ERROR_CODES(0xC,3))
#define HIDP_STATUS_USAGE_NOT_FOUND          (HIDP_ERROR_CODES(0xC,4))
#define HIDP_STATUS_VALUE_OUT_OF_RANGE       (HIDP_ERROR_CODES(0xC,5))
#define HIDP_STATUS_BAD_LOG_PHY_VALUES       (HIDP_ERROR_CODES(0xC,6))
#define HIDP_STATUS_BUFFER_TOO_SMALL         (HIDP_ERROR_CODES(0xC,7))
#define HIDP_STATUS_INTERNAL_ERROR           (HIDP_ERROR_CODES(0xC,8))
#define HIDP_STATUS_I8042_TRANS_UNKNOWN      (HIDP_ERROR_CODES(0xC,9))
#define HIDP_STATUS_INCOMPATIBLE_REPORT_ID   (HIDP_ERROR_CODES(0xC,0xA))
#define HIDP_STATUS_NOT_VALUE_ARRAY          (HIDP_ERROR_CODES(0xC,0xB))
#define HIDP_STATUS_IS_VALUE_ARRAY           (HIDP_ERROR_CODES(0xC,0xC))
#define HIDP_STATUS_DATA_INDEX_NOT_FOUND     (HIDP_ERROR_CODES(0xC,0xD))
#define HIDP_STATUS_DATA_INDEX_OUT_OF_RANGE  (HIDP_ERROR_CODES(0xC,0xE))
#define HIDP_STATUS_BUTTON_NOT_PRESSED       (HIDP_ERROR_CODES(0xC,0xF))
#define HIDP_STATUS_REPORT_DOES_NOT_EXIST    (HIDP_ERROR_CODES(0xC,0x10))
#define HIDP_STATUS_NOT_IMPLEMENTED          (HIDP_ERROR_CODES(0xC,0x20))

//
// We blundered this status code.
//
#define HIDP_STATUS_I8242_TRANS_UNKNOWN HIDP_STATUS_I8042_TRANS_UNKNOWN

#include <poppack.h>

#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning(default:4115)
#pragma warning(default:4201)
#pragma warning(default:4214)
#endif

#ifdef __cplusplus
}
#endif

#endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) */
#pragma endregion

#endif

/*++

Copyright (c) 2008 Seiko Instruments Inc.
All rights reserved.

Module Name:

    DevInfo.h

Abstract:

    Define table file for target device 

Author:

Revision History:

--*/

#pragma once


// Define dll file names
#define	DLL_FILE_NAME		TEXT("SII_DPUS_API.DLL")
/*
Legacy dll name. but both dll names are supported. 
#define	DLL_FILE_NAME		TEXT("SII_DPUS445_API.DLL")
*/

// Sample commands
#define	HEADER_CMD		{0x12,'j', 0x00}
#define	FOOTER_CMD		{0x12,'j', 0x0F}
#define	SAMPLE_CMD		{0x12,'v'}

// Define string commands for GetSiiPrinterData()
static LPCTSTR TBL_CMD[] =			
{
	TEXT("AUTO_STATUS_BACK"),
	TEXT("FUNCTION_SET_RESP"),
	TEXT("REMAIN_MEMORY_CAP"),
	TEXT("EXT_RAM_CHECKSUM"),
	TEXT("SEND_VP_VOLTAGE"),
	TEXT("MAINT_CONT_TRANS_FEED"),
	TEXT("MAINT_CONT_TRANS_HEAD"),
	TEXT("MAINT_CONT_TRANS_DRIVE"),
	TEXT("MAINT_CONT_TRANS_FEED_INTEGR"),
	TEXT("MAINT_CONT_TRANS_HEAD_INTEGR"),
	TEXT("MAINT_CONT_TRANS_DRIVE_INTEGR"),
	TEXT("REMAIN_NV_MEMORY_CAP"),
	TEXT("INIT_TEST_PRINT_HEADER"),
	TEXT("READ_DEFAULT"),
	TEXT("GET_RAW_DATA_AUTO_STATUS"),
	TEXT("GET_RAW_DATA_EXEC_RESP"),
	TEXT("GET_RAW_DATA_CMD_RESP"),
	TEXT("")
};


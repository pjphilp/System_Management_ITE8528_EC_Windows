///****************************************************************************
//
//    Copyright 2017 by WinSystems Inc.
//
//    Permission is hereby granted to the purchaser of WinSystems GPIO cards
//    and CPU products incorporating a GPIO device, to distribute any binary
//    file or files compiled using this source code directly or in any work
//    derived by the user from this file. In no case may the source code,
//    original or derived from this file, be distributed to any third party
//    except by explicit permission of WinSystems. This file is distributed
//    on an "As-is" basis and no warranty as to performance or fitness of pur-
//    poses is expressed or implied. In no case shall WinSystems be liable for
//    any direct or indirect loss or damage, real or consequential resulting
//    from the usage of this source code. It is the user's sole responsibility
//    to determine fitness for any considered purpose.
//
///****************************************************************************
//
//    Name       : WinSys_Errors.h
//
//    Project    : Definitions of WinSystems errors for the PX1-C415 ACPI
//                 access into the Embedded Controllers SRAM
//
//    Author     : pjp
//
//    #defines for WinSystems defined errors (follows Microsoft error code
//    syntax found in winerror.h
//
///****************************************************************************
//
//      Date      Revision    Description
//    --------    --------    ---------------------------------------------
//    09/08/17      0.1       PJP - from similar file for different project
//
///****************************************************************************

#pragma once

///////////////////////////////////////////////////////////////////////////////
//
// a typedef for the WinSys error type...
//

typedef uint32_t        WINSYS_ERROR;

//
// top 2 bits define the error type (error, warning, info, success)
//
#define WINDOWS_SUCCESS                         0x0
#define WINDOWS_INFO                            0x1
#define WINDOWS_WARN                            0x2
#define WINDOWS_ERROR                           0x3

//
// bit 29 will be set as these are not Windows errors...
//
#define WINDOWS_CUSTOMER_CODE                   0x1

#define FACILITY_WINSYSTEMS                            ( FACILITY_PIX + 20 )            // first 18 used in system management
#define WINSYS_FACILITY_ACPI_EC_ACCESS                 ( FACILITY_WINSYSTEMS + 1 )

//
// a macro to build the error code from type, customer, facility and error
//
#define WINS_ERROR( TYPE, WINDOWS_CUSTOMER_CODE, FACILITY, ERROR_CODE )     ( ( TYPE << 30 ) | ( WINDOWS_CUSTOMER_CODE << 29 ) | ( FACILITY << 16 ) | ERROR_CODE )

//
// define the errors
//
#define STATUS_SUCCESS                          S_OK
#define STATUS_BAD_PARAMETER                    1
#define STATUS_NULL_POINTER                     2
#define STATUS_BAD_POINTER                      3
#define STATUS_BAD_DEV_HANDLE                   4
#define STATUS_INDEX_OUT_OF_RANGE               5
#define STATUS_ENUMERATION_OUT_OF_RANGE         6
#define STATUS_BURST_ACK_TIMEOUT                7



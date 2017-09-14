//****************************************************************************
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
//    Name       : WDT_Test1.c
//
//    Project    : ACPI Embedded Controller Library
//
//    Author     : pjp, from code originally by PD & Portwell
//
//    Description:
//      Basic test of the ITE8528 WDT using the seconds mode. Shows use of
//      "C" bindings. Also note that this application, when working correctly
//      will cause an ugly reboot of the system, without closing files, etc.
//
///****************************************************************************
//
//      Date      Revision    Description
//    --------    --------    ---------------------------------------------
//    09/07/17      0.1       From PD & Portwell
//
///****************************************************************************

#include <windows.h>
#include <stdio.h>
#include <x86_64_port.h>
#include <WinSys_Errors.h>
#include <ITE8528_EC_Lib.h>

int main()
{
   uint8_t         Seconds = 30;
   WINSYS_ERROR    Status = WDT_SetSecondsCounter( Seconds );

   if ( Status == STATUS_SUCCESS )
       {
          Status = WDT_Start( SECOND_MODE_ENUM );
          Sleep( ( ( Seconds + 1 ) * 1000 ) );                  // sleep for 1 second longer than WDT is set for
          printf("WTF? We should never have gotten here!\n");   // and we should never get here because the system
                                                                // will reset before the sleep expires
          return 1;
       }
   else
       {
          // some kind of error writing the WDT seconds counter register...
          return 1;
       }

   return 0;
}


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
//    Name       : TEMP_Test.cpp
//
//    Project    : ACPI Embedded Controller Library
//
//    Author     : pjp, from code originally by PD & Portwell
//
//    Description:
//      Basic test of the ITE8528 temperature sensors. This app will loop,
//      reading the temperature sensors until a key is pressed. Example of
//      "C++" bindings to the ITE8628 EC library.
//
///****************************************************************************
//
//      Date      Revision    Description
//    --------    --------    ---------------------------------------------
//    09/07/17      0.1       From PD & Portwell
//
///****************************************************************************

#include "stdafx.h"
#include <windows.h>
#include <conio.h>

#include <x86_64_port.h>
#include <WinSys_Errors.h>
#include <ITE8528_EC_Lib.h>

int main()
{
   WINSYS_ERROR    Status;
   bool            Exit = FALSE;
   uint8_t         CPU_Temperature,
                   SYS_Temperature;

   while ( ( ! _kbhit() ) && ( Exit != TRUE ) )
   {
      if ( ( Status = TEMP_GetCPU( &CPU_Temperature ) ) != STATUS_SUCCESS )
          {
             printf("Error reading CPU temperature sensors. Exiting...\n");
             Exit = TRUE;
          }
      else
          {
             if ( ( Status = TEMP_GetSYS( &SYS_Temperature ) ) != STATUS_SUCCESS )
                 {
                    printf("Error reading SYS temperature sensors. Exiting...\n");
                    Exit = TRUE;
                 }
             else
                 {
                    printf("CPU Temperature = %d\n", CPU_Temperature );
                    printf("SYS Temperature = %d\n\n", SYS_Temperature );
                    Sleep( 1000 );                                         // sleep for 1 seconds...
                 }
          }
   }

   return 0;
}



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
//    Name       : PWR_Test.cpp
//
//    Project    : ACPI Embedded Controller Library
//
//    Author     : pjp, from code originally by PD & Portwell
//
//    Description:
//      Basic test of the ITE8528 Power Monitoring. Demonstrates "C++" bindings
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
#include <x86_64_port.h>
#include <WinSys_Errors.h>
#include <inpout32.h>
#include <ITE8528_EC_Lib.h>

WINSYS_ERROR main()
{
   double          Voltage;
   WINSYS_ERROR    Status;

   DlPortWritePortUchar( 0x6E, 0x07 );
   DlPortWritePortUchar( 0x6F, 0x0F );

   Status = PWR_GetVCore( &Voltage );

   if ( Status == STATUS_SUCCESS )
       {
          printf("VCore measurement = %2.5f\n", Voltage );
          if ( ( Status = PWR_Get3p3V( &Voltage ) ) == STATUS_SUCCESS )
              {
                 printf("3.3V measurement = %2.5f\n", Voltage );
                 if ( ( Status = PWR_Get5V( &Voltage ) ) == STATUS_SUCCESS )
                     {
                        printf("5V measurement = %2.5f\n", Voltage );
                        if ( ( Status = PWR_Get12V( &Voltage ) ) == STATUS_SUCCESS )
                            {
                               printf("12V measurement = %2.5f\n", Voltage );
                               if ( ( Status = PWR_GetDimmV( &Voltage ) ) == STATUS_SUCCESS )
                                   {
                                      printf("DIMM voltage measurement = %2.5f\n", Voltage );

                                   }
                               else
                                   {

                                   }
                            }
                        else
                            {

                            }
                     }
                 else
                     {

                     }
              }
          else
              {

              }
       }
   else
       {

       }

   return Status;
}

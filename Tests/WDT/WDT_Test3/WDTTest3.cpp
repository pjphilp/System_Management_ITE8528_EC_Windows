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
//    Name       : WDT_Test3.cpp
//
//    Project    : ACPI Embedded Controller Library
//
//    Author     : pjp, from code originally by PD & Portwell
//
//    Description:
//      Basic test of the ITE8528 WDT using the seconds mode, WDT "petting",
//      and WDT stop/disable. Shows use of "C++" bindings. Also note that this
//      application, when working correctly could cause an ugly reboot of the
//      system, without closing files, etc.
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
   uint8_t         Seconds = 60,
                   Minutes = 0;

   WINSYS_ERROR    Status;

   //
   // set WDT for 60 seconds, go into loop, counting down, and at 57, pet watchdog timer
   // if key is pressed -> X key, demo will exit, leaving WDT running, system will eventually reboot
   //                   -> any other key, demo will stop/disable WDT and exit
   //

   printf("\n");
   printf("This demonstrTion will set the WDT to expire at 60 seconds. After\n");
   printf("setting the WDT, the demo will countdown from 60, and when there are\n");
   printf("3 seconds left on the counter, the WDT will be \"petted\". This sequence\n");
   printf("will continue until the user presses a key on the keyboard. If the key\n");
   printf("is the \"X\" or \"x\" key, the program will exit, leaving the WDT running,\n");
   printf("which will cause the system to reset within 1 minute. If any other key\n");
   printf("is pressed, the demo will stop/disable the WDT and exit, and the system\n");
   printf("WILL NOT RESET due to the WDT.\n\n\n");

   if ( ( Status = WDT_SetSecondsCounter( Seconds ) ) == STATUS_SUCCESS )
       {
          if ( ( Status = WDT_Start( SECOND_MODE_ENUM ) ) == STATUS_SUCCESS )
              {
                 bool      Exit = FALSE;
                 uint8_t   Counter = 0;

                 while ( ( ! _kbhit() ) && ( Exit != TRUE ) )
                 {
                    Sleep( 1000 );      // sleep for 1 second
                    Counter++;

                    if ( Counter == ( Seconds - 3 ) )
                    {
                       //
                       // pet the WDT one second before it is supposed to fire...
                       //

                       if ( ( Status = WDT_PetTimer( Minutes, Seconds ) ) != STATUS_SUCCESS )
                           {
                              Exit = TRUE;
                              printf("Error ""petting"" the watchdog timer. Exiting after disabling the WDT.\n");
                              WDT_Disable();
                           }
                       else
                           {
                              printf("WDT petted\n");
                              Counter = 0;
                           }
                    }
                 }

                 if ( Exit != TRUE )
                 {
                    //
                    // keyboard was hit - figure out what key was pressed...
                    //

                    uint8_t Ch = _getch();

                    if ( ( Ch != 'x' ) && ( Ch != 'X' ) )
                        {
                           printf("Disabling WDT and exiting.\n");
                           WDT_Disable();
                        }
                    else
                        {
                           printf("Exiting without WDT disable.\n");
                        }
                 }
              }
          else
              {
                 printf("Error starting the WDT. Exiting...\n");
                 WDT_Disable();                                     // just in case...
              }
       }
   else
       {
          printf("Error setting WDT mode. Exiting.\n");
       }

   return 0;
}


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
//    Name       : ACPI_EC_Lib.cpp
//
//    Project    : ACPI Embedded Controller Routines
//
//    Author     : pjp, from code originally by PD & Portwell
//
//    Description:
//      This module contains the implementation of the ACPI Embedded Controller
//      functions
//
///****************************************************************************
//
//      Date      Revision    Description
//    --------    --------    ---------------------------------------------
//    09/07/17      0.1       From PD & Portwell
//
///****************************************************************************

#include <windows.h>
#include <x86_64_port.h>
#include <WinSys_Errors.h>
#include <ITE8528_EC_Lib.h>
#include <inpout32.h>



/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*  Function: EC_SetBurtMode                                                  */
/*                                                                            */
/*!\brief  Send command to EC to initiate burst mode                          */
/*                                                                            */
/*!\param   <,void>                                                           */
/*!\return  WINSYS_ERROR    value indicating success or failure               */
/*                                                                            */
/*!\note                                                                      */
/*                                                                            */
/******************************************************************************/
static WINSYS_ERROR EC_SetBurtMode( void )
{
   WINSYS_ERROR         Results = STATUS_SUCCESS;
   uint8_t              Count = 100;
   ACPI_STATUS_UNION    StatusReg;

   do
   {
      DlPortWritePortUshort( ACPI_EC_CMND_REG, ( ( uint8_t ) BURST_ENABLE_CMD ) );
      Sleep( BURST_SLEEP_PERIOD_MILLISECS );
//      StatusReg.Byte = ( uint8_t ) DlPortReadPortUshort( ACPI_EC_DATA_REG );
      StatusReg.Byte = ( uint8_t ) DlPortReadPortUshort( ACPI_EC_CMND_REG );
      Count--;

   } while ( ( StatusReg.Bits.Burst == 0 ) && ( Count > 0 ) );

   if ( ( Count == 0 ) || ( StatusReg.Bits.Burst == 0 ) )
   {
      Results = WINS_ERROR( WINDOWS_ERROR, WINDOWS_CUSTOMER_CODE, WINSYS_FACILITY_ACPI_EC_ACCESS, STATUS_BURST_ACK_TIMEOUT );
   }

   return Results;
}


/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*  Function: EC_WriteByteUsingACPI                                           */
/*                                                                            */
/*!\brief  Writes a byte to the specified offset in the EC's SRAM using the   */
/*         ACPI EC port 0x62/0x66 method                                      */
/*                                                                            */
/*!\param   uint8_t         Offset in EC memory space to write to             */
/*!\param   uint8_t         Value to write to offset in EC memory space       */
/*!\return  WINSYS_ERROR    value indicating success or failure               */
/*                                                                            */
/*!\note  This function makes use of the ACPI EC port 62/66 access            */
/*                                                                            */
/******************************************************************************/
WINSYS_ERROR EC_WriteByteUsingACPI( uint8_t Offset, uint8_t Value )
{
   WINSYS_ERROR         Results;

   if ( ( Results = EC_SetBurtMode() ) == STATUS_SUCCESS )
   {
      DlPortWritePortUshort( ACPI_EC_CMND_REG, WRITE_EC_CMD );
      Sleep( BURST_SLEEP_PERIOD_MILLISECS );
      DlPortWritePortUshort( ACPI_EC_DATA_REG, Offset );
      Sleep( BURST_SLEEP_PERIOD_MILLISECS );

      DlPortWritePortUshort( ACPI_EC_DATA_REG, Value );
   }

   return Results;
}

/******************************************************************************/
/*                                                                            */
/*  Function: EC_ReadByteUsingACPI                                            */
/*                                                                            */
/*!\brief  Reads a byte from the specified offset in the EC's SRAM using the  */
/*         ACPI EC port 0x62/0x66 method                                      */
/*                                                                            */
/*!\param   uint8_t         offset into EC RAM to read byte from              */
/*!\param   puint8_t        pointer to uint8_t to save read byte to           */
/*!\return  WINSYS_ERROR    value indicating success or failure               */
/*                                                                            */
/*!\note    This function makes use of the ACPI EC port 62/66 access          */
/*                                                                            */
/******************************************************************************/
WINSYS_ERROR EC_ReadByteUsingACPI( uint8_t Offset, puint8_t pData )
{
   WINSYS_ERROR         Results;

   if ( pData )
       {
          if ( ( Results = EC_SetBurtMode() ) == STATUS_SUCCESS )
          {
             DlPortWritePortUshort( ACPI_EC_CMND_REG, READ_EC_CMD );
             DlPortWritePortUshort( ACPI_EC_DATA_REG, Offset );

             *pData = ( uint8_t ) DlPortReadPortUshort( ACPI_EC_DATA_REG );
          }
       }
   else
       {
          Results = WINS_ERROR( WINDOWS_ERROR, WINDOWS_CUSTOMER_CODE, WINSYS_FACILITY_ACPI_EC_ACCESS, STATUS_NULL_POINTER );
       }

   return Results;
}

/******************************************************************************/
/*                                                                            */
/*  Function: EC_WriteByteUsingIOSpace                                        */
/*                                                                            */
/*!\brief  Writes a byte to the specified offset in the EC's SRAM  using the  */
/*         EC's host IO Space mapping                                         */
/*                                                                            */
/*!\param   uint8_t         Offset in EC memory space to write to             */
/*!\param   uint8_t         Value to write to offset in EC memory space       */
/*!\return  WINSYS_ERROR    value indicating success or failure               */
/*                                                                            */
/*!\note    The offset specified as a parameter is added to the IO space      */
/*!\note    base address. THIS FUNTION WILL PROBABLY BE REMOVED BEFORE        */
/*!\note    RELEASE                                                           */
/*                                                                            */
/******************************************************************************/
WINSYS_ERROR EC_WriteByteUsingIOSpace( uint8_t Offset, uint8_t Value )
{
   WINSYS_ERROR         Results = STATUS_SUCCESS;

   DlPortWritePortUchar( ( uint16_t )( EC_IO_PORT + ( uint16_t )( Offset ) ), Value );

   return Results;
}

/******************************************************************************/
/*                                                                            */
/*  Function: EC_ReadByteUsingIOSpace                                         */
/*                                                                            */
/*!\brief  Read a byte to the specified offset in the EC's SRAM  using the    */
/*         EC's host IO Space mapping                                         */
/*                                                                            */
/*!\param   uint8_t         Offset in EC memory space to write to             */
/*!\param   uint8_t         Value to write to offset in EC memory space       */
/*!\return  WINSYS_ERROR    value indicating success or failure               */
/*                                                                            */
/*!\note    The offset specified as a parameter is added to the IO space      */
/*!\note    base address. THIS FUNTION WILL PROBABLY BE REMOVED BFORE         */
/*!\note    RELEASE.                                                          */
/*                                                                            */
/******************************************************************************/
WINSYS_ERROR EC_ReadByteUsingIOSpace( uint8_t Offset, puint8_t pData )
{
   WINSYS_ERROR   Results = STATUS_SUCCESS;

   if ( pData )
       {
          *pData = DlPortReadPortUchar( ( uint16_t )( EC_IO_PORT + ( uint16_t )( Offset ) ) );
       }
   else
       {
          Results = WINS_ERROR( WINDOWS_ERROR, WINDOWS_CUSTOMER_CODE, WINSYS_FACILITY_ACPI_EC_ACCESS, STATUS_NULL_POINTER );
       }

   return Results;
}

/******************************************************************************/
/******************************************************************************/
/*                                                                            */
/*  Watch Dog Timer Funtions                                                  */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/*  Function:  WDT_Disable                                                    */
/*                                                                            */
/*!\brief  Disables the watchdog timer                                        */
/*                                                                            */
/*!\param   <void>                                                            */
/*!\return  WINSYS_ERROR    value indicating success or failure               */
/*                                                                            */
/*!\note   All sleep/pauses for EC access are performed in the EC read/write  */
/*!\note   functions                                                          */
/*                                                                            */
/******************************************************************************/
WINSYS_ERROR WDT_Disable( void )
{
   WDT_CONFIG_REG_UNION     WdtConfig;
   WINSYS_ERROR             Results;

   Results = EC_ReadByteUsingACPI( WDT_CONFIG_OFFSET, &WdtConfig.Byte );    // retrieve WDT config register

   if ( Results == STATUS_SUCCESS )
   {
      WdtConfig.Bits.Enable = 0;                                        // clear enable bit to disable
      Results = EC_WriteByteUsingACPI( WDT_CONFIG_OFFSET, WdtConfig.Byte ); // write it back to WDT config register
   }

   return Results;
}

/******************************************************************************/
/*                                                                            */
/*  Function: WDT_Enable                                                      */
/*                                                                            */
/*!\brief  Enables the watchdog timer                                         */
/*                                                                            */
/*!\param   <void>                                                            */
/*!\return  WINSYS_ERROR    value indicating success or failure               */
/*                                                                            */
/*!\note   All sleep/pauses for EC access are performed in the EC read/write  */
/*!\note   functions                                                          */
/*                                                                            */
/******************************************************************************/
WINSYS_ERROR WDT_Enable( void )
{
   WDT_CONFIG_REG_UNION     WdtConfig;
   WINSYS_ERROR             Results;

   Results = EC_ReadByteUsingACPI( WDT_CONFIG_OFFSET, &WdtConfig.Byte );    // retrieve WDT config register

   if ( Results == STATUS_SUCCESS )
   {
      WdtConfig.Bits.Enable = 1;                                        // set enable bit
      Results = EC_WriteByteUsingACPI( WDT_CONFIG_OFFSET, WdtConfig.Byte ); // write it back to WDT config register
   }

   return Results;
}

/******************************************************************************/
/*                                                                            */
/*  Function:  WDT_Start                                                      */
/*                                                                            */
/*!\brief   Starts the WDT in the specified mode (minutes or seconds)         */
/*                                                                            */
/*!\param   WDT_MODE_ENUM_TYPE   Mode to start WDT in                         */
/*!\return  WINSYS_ERROR    value indicating success or failure               */
/*                                                                            */
/*!\note                                                                      */
/*                                                                            */
/******************************************************************************/
WINSYS_ERROR WDT_Start( WDT_MODE_ENUM_TYPE Mode )
{
   WDT_CONFIG_REG_UNION     WdtConfig;
   WINSYS_ERROR             Results;

   Results = EC_ReadByteUsingACPI( WDT_CONFIG_OFFSET, &WdtConfig.Byte );    // retrieve WDT config register

   if ( Results == STATUS_SUCCESS )
   {
      WdtConfig.Bits.Mode = Mode;                                       // set/clear mode bit
      WdtConfig.Bits.Enable = 1;                                        // set enable bit
      Results = EC_WriteByteUsingACPI( WDT_CONFIG_OFFSET, WdtConfig.Byte ); // write it back to WDT config register
   }

   return Results;
}

/******************************************************************************/
/*                                                                            */
/*  Function: WDT_SetSecondsCounter                                           */
/*                                                                            */
/*!\brief  Write the indicated seconds value to the WDT seconds counter       */
/*                                                                            */
/*!\param   uint8_t   Count in seconds to write to seconds counter register   */
/*!\return  WINSYS_ERROR   value indicating success or failure                */
/*                                                                            */
/*!\note                                                                      */
/*                                                                            */
/******************************************************************************/
WINSYS_ERROR WDT_SetSecondsCounter( uint8_t Secs )
{
   WINSYS_ERROR  Results = EC_WriteByteUsingACPI( WDT_SECONDS_COUNTER_OFFSET, Secs );

   return Results;
}

/******************************************************************************/
/*                                                                            */
/*  Function: WDT_SetMinutesCounter                                           */
/*                                                                            */
/*!\brief   Write the indicated minutes value to the WDT minutes counter      */
/*                                                                            */
/*!\param   uint8_t   Count in minutes to write to minutes counter register   */
/*!\return  WINSYS_ERROR   value indicating success or failure                */
/*                                                                            */
/*!\note                                                                      */
/*                                                                            */
/******************************************************************************/
WINSYS_ERROR WDT_SetMinutesCounter( uint8_t Mins )
{
   WINSYS_ERROR  Results = EC_WriteByteUsingACPI( WDT_MINUTES_COUNTER_OFFSET, Mins );

   return Results;
}

/*********************************************************************************/
/*                                                                               */
/*  Function:  WDT_PetTimer                                                      */
/*                                                                               */
/*!\brief  Write the indicated minutes and seconds to the WDT counter registers  */
/*!        causing the WDT to reset for the next countdown.                      */
/*                                                                               */
/*!\param   uint8_t   Count in minutes to write to minutes counter register      */
/*!\param   uint8_t   Count in seconds to write to seconds counter register      */
/*!\return  WINSYS_ERROR   value indicating success or failure                   */
/*                                                                               */
/*!\note   This function "pets" the watchdog timer                               */
/*                                                                               */
/*********************************************************************************/
WINSYS_ERROR WDT_PetTimer( uint8_t Mins, uint8_t Secs )
{
   WINSYS_ERROR  Results = WDT_SetMinutesCounter( Mins );

   if ( Results == STATUS_SUCCESS )
   {
      Results = WDT_SetSecondsCounter( Secs );
   }

   return Results;
}

/*********************************************************************************/
/*********************************************************************************/
/*                                                                               */
/*  Temperature Sensor Funtions                                                  */
/*                                                                               */
/*********************************************************************************/
/*                                                                               */
/*  Function:  TEMP_GetCPU                                                       */
/*                                                                               */
/*!\brief   Reads the CPU temperature sensor                                     */
/*!                                                                              */
/*                                                                               */
/*!\param   puint8_t       pointer to uint8_t value to return temperature in     */
/*!\return  WINSYS_ERROR   value indicating success or failure                   */
/*                                                                               */
/*!\note    The CPU temperature sensor is built into the ITE8528 EC              */
/*                                                                               */
/*********************************************************************************/
WINSYS_ERROR TEMP_GetCPU( puint8_t pTemp )
{
   WINSYS_ERROR Results = STATUS_SUCCESS;

   if ( pTemp )
       {
          uint8_t    TempU8;

          Results = EC_ReadByteUsingACPI( CPU_TEMPERATURE_OFFSET, &TempU8 );    // retrieve temperature sensor byte
          if ( Results == STATUS_SUCCESS )
              {
                 *pTemp = TempU8;
              }
          else
              {

              }
       }
   else
       {
          Results = WINS_ERROR( WINDOWS_ERROR, WINDOWS_CUSTOMER_CODE, WINSYS_FACILITY_ACPI_EC_ACCESS, STATUS_NULL_POINTER );
       }

   return Results;
}

/*********************************************************************************/
/*                                                                               */
/*  Function:  TEMP_GetSYS                                                       */
/*                                                                               */
/*!\brief   Reads the system temperature sensor                                  */
/*!                                                                              */
/*                                                                               */
/*!\param   puint8_t       pointer to uint8_t value to return temperature in     */
/*!\return  WINSYS_ERROR   value indicating success or failure                   */
/*                                                                               */
/*!\note                                                                         */
/*                                                                               */
/*********************************************************************************/
WINSYS_ERROR TEMP_GetSYS( puint8_t pTemp )
{
   WINSYS_ERROR Results = STATUS_SUCCESS;

   if ( pTemp )
       {
          uint8_t    TempU8;

          Results = EC_ReadByteUsingACPI( SYS_TEMPERATURE_OFFSET, &TempU8 );    // retrieve temperature sensor byte
          if ( Results == STATUS_SUCCESS )
              {
                 *pTemp = TempU8;
              }
          else
              {

              }
       }
   else
       {
          Results = WINS_ERROR( WINDOWS_ERROR, WINDOWS_CUSTOMER_CODE, WINSYS_FACILITY_ACPI_EC_ACCESS, STATUS_NULL_POINTER );
       }

   return Results;
}


/*********************************************************************************/
/*                                                                               */
/*  Power Sensors Functions                                                      */
/*                                                                               */
/*********************************************************************************/
/*                                                                               */
/*  Function:  PWR_GetDimmV                                                      */
/*                                                                               */
/*!\brief   Returns the voltage used by the DIMM                                 */
/*                                                                               */
/*!\param   pdouble_t      pointer to double value to return measured voltage in */
/*!\return  WINSYS_ERROR   value indicating success or failure                   */
/*                                                                               */
/*!\note                                                                         */
/*                                                                               */
/*********************************************************************************/
WINSYS_ERROR PWR_GetDimmV( pdouble_t pVolts )
{
   WINSYS_ERROR Results = STATUS_SUCCESS;

   if ( pVolts )
       {
          uint8_t    TempU8_1,
                     TempU8_2;

          Results = EC_ReadByteUsingACPI( VDIMM_L_OFFSET, &TempU8_1 );          // retrieve low byte of voltage
          if ( Results == STATUS_SUCCESS )
              {
                 Results = EC_ReadByteUsingACPI( VDIMM_L_OFFSET, &TempU8_2 );   // retrieve high byte of voltage
                 *pVolts = ( ( double )( ( TempU8_2 << 8 ) + TempU8_1 ) ) * VCORE_SCALE_FACTOR;
              }
          else
              {

              }
       }
   else
       {
          Results = WINS_ERROR( WINDOWS_ERROR, WINDOWS_CUSTOMER_CODE, WINSYS_FACILITY_ACPI_EC_ACCESS, STATUS_NULL_POINTER );
       }

   return Results;
}

/*********************************************************************************/
/*                                                                               */
/*  Function:  PWR_Get12V                                                        */
/*                                                                               */
/*!\brief   Returns the 12 volt sensor reading                                   */
/*                                                                               */
/*!\param   pdouble_t      pointer to double value to return measured voltage in */
/*!\return  WINSYS_ERROR   value indicating success or failure                   */
/*                                                                               */
/*!\note                                                                         */
/*                                                                               */
/*********************************************************************************/
WINSYS_ERROR PWR_Get12V( pdouble_t pVolts )
{
   WINSYS_ERROR Results = STATUS_SUCCESS;

   if ( pVolts )
       {
          uint8_t    TempU8_1,
                     TempU8_2;

          Results = EC_ReadByteUsingACPI( V12_L_OFFSET, &TempU8_1 );           // retrieve low byte of voltage
          if ( Results == STATUS_SUCCESS )
              {
                 Results = EC_ReadByteUsingACPI( V12_L_OFFSET, &TempU8_2 );    // retrieve high byte of voltage
                 *pVolts = ( ( double )( ( TempU8_2 << 8 ) + TempU8_1 ) ) * V12_SCALE_FACTOR;
              }
          else
              {

              }
       }
   else
       {
          Results = WINS_ERROR( WINDOWS_ERROR, WINDOWS_CUSTOMER_CODE, WINSYS_FACILITY_ACPI_EC_ACCESS, STATUS_NULL_POINTER );
       }

   return Results;
}


/*********************************************************************************/
/*                                                                               */
/*  Function:  PWR_Get5V                                                         */
/*                                                                               */
/*!\brief   Returns the 5 volt sensor reading                                    */
/*                                                                               */
/*!\param   pdouble_t      pointer to double value to return measured voltage in */
/*!\return  WINSYS_ERROR   value indicating success or failure                   */
/*                                                                               */
/*!\note                                                                         */
/*                                                                               */
/*********************************************************************************/
WINSYS_ERROR PWR_Get5V( pdouble_t pVolts )
{
   WINSYS_ERROR Results = STATUS_SUCCESS;

   if ( pVolts )
       {
          uint8_t    TempU8_1,
                     TempU8_2;

          Results = EC_ReadByteUsingACPI( V5_L_OFFSET, &TempU8_1 );           // retrieve low byte of voltage
          if ( Results == STATUS_SUCCESS )
              {
                 Results = EC_ReadByteUsingACPI( V5_H_OFFSET, &TempU8_2 );    // retrieve high byte of voltage
                 *pVolts = ( ( double )( ( TempU8_2 << 8 ) + TempU8_1 ) ) * V5_SCALE_FACTOR;
              }
          else
              {

              }
       }
   else
       {
          Results = WINS_ERROR( WINDOWS_ERROR, WINDOWS_CUSTOMER_CODE, WINSYS_FACILITY_ACPI_EC_ACCESS, STATUS_NULL_POINTER );
       }

   return Results;
}

/*********************************************************************************/
/*                                                                               */
/*  Function:  PWR_Get3p3V                                                       */
/*                                                                               */
/*!\brief   Returns the 3.3 volt sensor reading                                  */
/*                                                                               */
/*!\param   pdouble_t      pointer to double value to return measured voltage in */
/*!\return  WINSYS_ERROR   value indicating success or failure                   */
/*                                                                               */
/*!\note                                                                         */
/*                                                                               */
/*********************************************************************************/
WINSYS_ERROR PWR_Get3p3V( pdouble_t pVolts )
{
   WINSYS_ERROR Results = STATUS_SUCCESS;

   if ( pVolts )
       {
          uint8_t    TempU8_1,
                     TempU8_2;

          Results = EC_ReadByteUsingACPI( V3P3V_L_OFFSET, &TempU8_1 );           // retrieve low byte of voltage
          if ( Results == STATUS_SUCCESS )
              {
                 Results = EC_ReadByteUsingACPI( V3P3V_H_OFFSET, &TempU8_2 );    // retrieve high byte of voltage
                 *pVolts = ( ( double )( ( TempU8_2 << 8 ) + TempU8_1 ) ) * V3P3_SCALE_FACTOR;
              }
          else
              {

              }
       }
   else
       {
          Results = WINS_ERROR( WINDOWS_ERROR, WINDOWS_CUSTOMER_CODE, WINSYS_FACILITY_ACPI_EC_ACCESS, STATUS_NULL_POINTER );
       }

   return Results;
}

/*********************************************************************************/
/*                                                                               */
/*  Function:  PWR_GetVCore                                                      */
/*                                                                               */
/*!\brief   Reads the VCore voltage sensor                                       */
/*                                                                               */
/*!\param   pdouble_t      pointer to double value to return measured voltage in */
/*!\return  WINSYS_ERROR   value indicating success or failure                   */
/*                                                                               */
/*!\note                                                                         */
/*                                                                               */
/*********************************************************************************/
WINSYS_ERROR PWR_GetVCore( pdouble_t pVolts )
{
   WINSYS_ERROR Results = STATUS_SUCCESS;

   if ( pVolts )
       {
          uint8_t    TempU8_1,
                     TempU8_2;

          Results = EC_ReadByteUsingACPI( VCORE_L_OFFSET, &TempU8_1 );           // retrieve low byte of voltage
          if ( Results == STATUS_SUCCESS )
              {
                 Results = EC_ReadByteUsingACPI( VCORE_H_OFFSET, &TempU8_2 );    // retrieve high byte of voltage
                 *pVolts = ( ( double )( ( TempU8_2 << 8 ) + TempU8_1 ) ) * VCORE_SCALE_FACTOR;
              }
          else
              {

              }
       }
   else
       {
          Results = WINS_ERROR( WINDOWS_ERROR, WINDOWS_CUSTOMER_CODE, WINSYS_FACILITY_ACPI_EC_ACCESS, STATUS_NULL_POINTER );
       }

   return Results;
}





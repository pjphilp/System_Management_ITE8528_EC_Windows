//****************************************************************************
//
//!\copyright 2017 by WinSystems Inc.
//!
//!  Permission is hereby granted to the purchaser of WinSystems CPU products
//!  to distribute any binary file or files compiled using this source code
//!  directly or in any work derived by the user from this file. In no case
//!  may the source code, original or derived from this file, be distributed
//!  to any third party except by explicit permission of WinSystems. This file
//!  is distributed on an "As-is" basis and no warranty as to performance or
//!  fitness of purposes is expressed or implied. In no case shall WinSystems
//!  be liable for any direct or indirect loss or damage, real or consequential
//!  resulting from the usage of this source code. It is the user's sole re-
//!  sponsibility to determine fitness for any considered purpose.
//
///****************************************************************************
//
//  Filename   ACPI_EC_lib.h
//
//  Project    ACPI Embedded Controller Routines
//
//!\brief      This module contains the definitions used to access the embedded
//!            controller using ACPI commands
//
//!\author     pjp, from code originally by PD
//!
//!\version    0.1
//!
//!\date       09/7/17
//!
//****************************************************************************
#ifndef __ITE8528_EC_LIB_INC
#define __ITE8528_EC_LIB_INC

//////////////////////////////////////////////////////////////////////////////
//
// All ACPI EC port IO accesses are 8 bits...
//
//  These IO addresses are used for secondary access to the EC, and are sometimes documented as 62/66 access
//
#define     ACPI_EC_DATA_REG            0x62    /*!< the ACPI EC Data Register               */
#define     ACPI_EC_CMND_REG            0x66    /*!< the ACPI EC Command Register            */

#define     EC_IO_PORT                  0xE300  /*!< can be determined programmatically - see test project "Decode"   */

//
// The supported ACPI EC Commands
//
typedef enum _ACPI_EC_CMND_ENUM {
                                   READ_EC_CMD = 0x80,          /*!<  read a byte in the address space of the EC      */
                                   WRITE_EC_CMD = 0x81,         /*!<  write a byte in the address space of the EC     */
                                   BURST_ENABLE_CMD = 0x82,     /*!<  request dedicated attention from EC             */
                                   BURST_DISABLE_CMD = 0x83,    /*!<  release the EC from dedicated attention to host */
                                   QUERY_EC_CMD = 0x84,         /*!<  sent when CI_EVT is set in EC status register   */
                                   MAX_EC_CMND = 0xff,          /*!<  force 8 bit enumeration                         */

                                } ACPI_EC_CMND_ENUM;



/*!\struct _ACPI_STATUS_REG_STRUCT
 * \brief  A structure to encapsulate the bits in the ACPI Status Register, which is returned by the EC when a command
 *        is initiated
 */
typedef struct _ACPI_STATUS_REG_STRUCT {
                                          uint8_t      Obf:1;          /*!< 1 = Output Buffer Full, data is ready for host              */
                                          uint8_t      Ibf:1;          /*!< 1 = Input Buffer Full, data is ready for EC                 */
                                          uint8_t      Cmd:1;          /*!< 0 = Byte in Data Register is data  [ only used by EC ]      */
                                          uint8_t      Burst:1;        /*!< 1 = Place EC in burst mode for polled command processing    */
                                          uint8_t      Sci_Evt:1;      /*!< 1 = SCI event is pending                                    */
                                          uint8_t      Smi_Evt:1;      /*!< 1 = SMI event is pending                                    */
                                          uint8_t      :1;             /*!< ignore/unused                                               */

                                       } ACPI_STATUS_REG_STRUCT, *P_ACPI_STATUS_REG_STRUCT;

/*!\union _ACPI_STATUS_REG_UNION
 * \brief  A union to encapsulate the ACPI_STATUS_REG_STRUCT for individual register bit access and access to the whole
 *         register contents as a byte
 */
typedef union _ACPI_STATUS_REG_UNION {
                                        ACPI_STATUS_REG_STRUCT      Bits;       /*!<  allows individual access to the register's bits */
                                        uint8_t                     Byte;       /*!<  the whole regoster as a byte                    */

                                     } ACPI_STATUS_UNION, *P_ACPI_STATUS_UNION;

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// EC SRAM offsets for various "devices"
//
//   The EC's temperature sensors
//

#define CPU_TEMPERATURE_OFFSET              0x00       /*!< CPU Temperature sensor */
#define SYS_TEMPERATURE_OFFSET              0x02       /*!< SYS Temperature        */

//
//   The EC's watchdog timer
//

#define WDT_CONFIG_OFFSET                   0x06       /*!< offset in the EC's SRAM for the WDT configuration register */
#define WDT_MINUTES_COUNTER_OFFSET          0x07       /*!< offset in the EC's SRAM for the WDT minutes counter        */
#define WDT_SECONDS_COUNTER_OFFSET          0x08       /*!< offset in the EC's SRAM for the WDT seconds counter        */


/*!\struct _WDT_CONFIG_REG_STRUCT
 * \brief  A structure to encapsulate the bits in the WDT configuration register
 */
typedef struct _WDT_CONFIG_REG_STRUCT {
                                          uint8_t      Enable:1;       /*!< Enable = 1, Disable = 0                 */
                                          uint8_t      Mode:1;         /*!< Mode = 1 -> WDT is in "minutes" mode,   */
                                                                       /*!< Mode = 0 -> WDT is in "seconds" mode    */
                                          uint8_t         :6;          /*!< Unused                                  */

                                      } WDT_CONFIG_REG_STRUCT, *P_WDT_CONFIG_REG_STRUCT;

/*!\union _WDT_CONFIG_REG_UNION
 * \brief  A union to encapsulate the WDT_CONFIG_REG_STRUCT for individual register bit access and access to the whole
 *         register contents as a byte
 */
typedef union _WDT_CONFIG_REG_UNION {
                                        WDT_CONFIG_REG_STRUCT      Bits;
                                        uint8_t                    Byte;

                                    } WDT_CONFIG_REG_UNION, *P_WDT_CONFIG_REG_UNION;

/*!\enum _WDT_MODE_ENUM_TYPE
 * \brief  An enumeration of the countdown modes supported by the WDT
 *
 */
typedef enum _WDT_MODE_ENUM_TYPE {
                                    SECOND_MODE_ENUM = 0,             /*!<  WDT is in seconds countdown mode */
                                    MINUTE_MODE_ENUM = 1,             /*!<  WDT is in minutes countdown mode */

                                 } WDT_MODE_ENUM_TYPE, *P_WDT_MODE_ENUM_TYPE;

/////////////////////////////
//
// the fan control
//
#define CPU_FAN_H_OFFSET                    0x10       // CPU Fan Speed high
#define CPU_FAN_L_OFFSET                    0x11       // CPU Fan Speed low

#define SMART_FAN_CFG_OFFSET                0x16       // Smart Fan Control Reg
#define SMART_FAN_TARGET_REG1_OFFSET        0x17       // Smart Fan Target Reg 1
#define SMART_FAN_TARGET_REG2_OFFSET        0x18       // Smart Fan Target Reg 2
#define SMART_FAN_TOLERANCE_OFFSET          0x18       // Smart Fan Tolerance Reg

/////////////////////////////
//
// the voltage sensor
//
#define VCORE_H_OFFSET                      0x21    // VCore
#define VCORE_L_OFFSET                      0x20
#define V3P3V_H_OFFSET                      0x23    // +3.3V
#define V3P3V_L_OFFSET                      0x22
#define V5_H_OFFSET                         0x25    // +5V
#define V5_L_OFFSET                         0x24
#define V12_H_OFFSET                        0x31    // +12V
#define V12_L_OFFSET                        0x30
#define VDIMM_H_OFFSET                      0x33    // VDIMM (Memory)
#define VDIMM_L_OFFSET                      0x32

#define VCORE_SCALE_FACTOR                  2.93255
#define V3P3_SCALE_FACTOR                   5.8651
#define V5_SCALE_FACTOR                     9.38416
#define V12_SCALE_FACTOR                    19.3548



//
// EC LPC Device Register
//

#define PRODUCT_NAME_1              0x3D  // EC F/W Product Name 1 Reg
#define EC_CHIP_ID1                 0x85  // EC CHIP ID Byte1

#define EC_CHIP_ID1_REG             0x20  // EC CHIP ID Byte1 Reg

/////////////////////////////////////////////
//
// Define "sleep" periods.
//
//  In "burst" mode, EC will transition to burst disable state if host doesn't issue command
//  within 400 microseconds, subsequent accesses of 50 microseconds each, and total burst
//  time of 1 millisecond...
//

#define BURST_SLEEP_PERIOD_MILLISECS        1       //50

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// function prototypes
//

#ifdef __DLL_BUILD
//
// The DLL project needs to have _DLL_BUILD defined so that this section gets conditionally compiled
// into the build. If its not defined, then the else block below will be used when building applications
// that make use of the DLL
//

extern "C" __declspec( dllexport )   WINSYS_ERROR     EC_WriteByteUsingACPI( uint8_t Offset, uint8_t Value );
extern "C" __declspec( dllexport )   WINSYS_ERROR     EC_ReadByteUsingACPI( uint8_t Offset, puint8_t pData );
extern "C" __declspec( dllexport )   WINSYS_ERROR     EC_WriteByteUsingIOSpace( uint8_t Offset, uint8_t Value );
extern "C" __declspec( dllexport )   WINSYS_ERROR     EC_ReadByteUsingIOSpace( uint8_t Offset, puint8_t pData );

extern "C" __declspec( dllexport )   WINSYS_ERROR     WDT_Disable( void );
extern "C" __declspec( dllexport )   WINSYS_ERROR     WDT_Enable( void );
extern "C" __declspec( dllexport )   WINSYS_ERROR     WDT_Start( WDT_MODE_ENUM_TYPE Mode );
extern "C" __declspec( dllexport )   WINSYS_ERROR     WDT_SetSecondsCounter( uint8_t Secs );
extern "C" __declspec( dllexport )   WINSYS_ERROR     WDT_SetMinutesCounter( uint8_t Mins );
extern "C" __declspec( dllexport )   WINSYS_ERROR     WDT_PetTimer( uint8_t Mins, uint8_t Secs );


extern "C" __declspec( dllexport )   WINSYS_ERROR     PWR_GetDimmV( pdouble_t pVolts );
extern "C" __declspec( dllexport )   WINSYS_ERROR     PWR_Get12V( pdouble_t pVolts );
extern "C" __declspec( dllexport )   WINSYS_ERROR     PWR_Get5V( pdouble_t pVolts );
extern "C" __declspec( dllexport )   WINSYS_ERROR     PWR_Get3p3V( pdouble_t pVolts );
extern "C" __declspec( dllexport )   WINSYS_ERROR     PWR_GetVCore( pdouble_t pVolts );

extern "C" __declspec( dllexport )   WINSYS_ERROR     TEMP_GetCPU( puint8_t pTemp );
extern "C" __declspec( dllexport )   WINSYS_ERROR     TEMP_GetSYS( puint8_t pTemp );

#else
//
// Applications using the DLL need to import the DLL functions using the prototypes below. Also note
// that the prototype usage is different if the application is a C++ app or a C app...
//

#ifdef __CPLUSPLUS
//
// C++ app...
//

extern "C" __declspec( dllimport )    WINSYS_ERROR     EC_WriteByteUsingACPI( uint8_t Offset, uint8_t Value );
extern "C" __declspec( dllimport )    WINSYS_ERROR     EC_ReadByteUsingACPI( uint8_t Offset, puint8_t pData );
extern "C" __declspec( dllimport )    WINSYS_ERROR     EC_WriteByteUsingIOSpace( uint8_t Offset, uint8_t Value );
extern "C" __declspec( dllimport )    WINSYS_ERROR     EC_ReadByteUsingIOSpace( uint8_t Offset, puint8_t pData );

extern "C" __declspec( dllimport )    WINSYS_ERROR     WDT_Disable( void );
extern "C" __declspec( dllimport )    WINSYS_ERROR     WDT_Enable( void );
extern "C" __declspec( dllimport )    WINSYS_ERROR     WDT_Start( WDT_MODE_ENUM_TYPE Mode );
extern "C" __declspec( dllimport )    WINSYS_ERROR     WDT_SetSecondsCounter( uint8_t Secs );
extern "C" __declspec( dllimport )    WINSYS_ERROR     WDT_SetMinutesCounter( uint8_t Mins );
extern "C" __declspec( dllimport )    WINSYS_ERROR     WDT_PetTimer( uint8_t Mins, uint8_t Secs );

extern "C" __declspec( dllimport )    WINSYS_ERROR     PWR_GetDimmV( pdouble_t pVolts );
extern "C" __declspec( dllimport )    WINSYS_ERROR     PWR_Get12V( pdouble_t pVolts );
extern "C" __declspec( dllimport )    WINSYS_ERROR     PWR_Get5V( pdouble_t pVolts );
extern "C" __declspec( dllimport )    WINSYS_ERROR     PWR_Get3p3V( pdouble_t pVolts );
extern "C" __declspec( dllimport )    WINSYS_ERROR     PWR_GetVCore( pdouble_t pVolts );

extern "C" __declspec( dllimport )    WINSYS_ERROR     TEMP_GetCPU( puint8_t pTemp );
extern "C" __declspec( dllimport )    WINSYS_ERROR     TEMP_GetSYS( puint8_t pTemp );

#else

__declspec( dllimport )    WINSYS_ERROR     EC_WriteByteUsingACPI( uint8_t Offset, uint8_t Value );
__declspec( dllimport )    WINSYS_ERROR     EC_ReadByteUsingACPI( uint8_t Offset, puint8_t pData );
__declspec( dllimport )    WINSYS_ERROR     EC_WriteByteUsingIOSpace( uint8_t Offset, uint8_t Value );
__declspec( dllimport )    WINSYS_ERROR     EC_ReadByteUsingIOSpace( uint8_t Offset, puint8_t pData );

__declspec( dllimport )    WINSYS_ERROR     WDT_Disable( void );
__declspec( dllimport )    WINSYS_ERROR     WDT_Enable( void );
__declspec( dllimport )    WINSYS_ERROR     WDT_Start( WDT_MODE_ENUM_TYPE Mode );
__declspec( dllimport )    WINSYS_ERROR     WDT_SetSecondsCounter( uint8_t Secs );
__declspec( dllimport )    WINSYS_ERROR     WDT_SetMinutesCounter( uint8_t Mins );
__declspec( dllimport )    WINSYS_ERROR     WDT_PetTimer( uint8_t Mins, uint8_t Secs );


__declspec( dllimport )    WINSYS_ERROR     PWR_GetDimmV( pdouble_t pVolts );
__declspec( dllimport )    WINSYS_ERROR     PWR_Get12V( pdouble_t pVolts );
__declspec( dllimport )    WINSYS_ERROR     PWR_Get5V( pdouble_t pVolts );
__declspec( dllimport )    WINSYS_ERROR     PWR_Get3p3V( pdouble_t pVolts );
__declspec( dllimport )    WINSYS_ERROR     PWR_GetVCore( pdouble_t pVolts );

__declspec( dllimport )    WINSYS_ERROR     TEMP_GetCPU( puint8_t pTemp );
__declspec( dllimport )    WINSYS_ERROR     TEMP_GetSYS( puint8_t pTemp );

#endif

#endif      // #ifdef _DLL_BUILD

#endif      // #ifndef __ACPI_EC_LIB_INC

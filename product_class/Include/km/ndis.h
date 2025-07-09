/*++

Copyright (c) Microsoft Corporation. All rights reserved.

Module Name:

    ndis.h

Abstract:

    This module defines the structures, macros, and functions available
    to NDIS drivers.

Notes:

    Before including this header, you must define one or more macros.  In all
    examples, "630" can be any version number (as explained later).

    1.  If you are compiling a kernel-mode miniport driver, define:
            #define NDIS_MINIPORT_DRIVER 1
            #define NDIS630_MINIPORT 1
        Additionally, if you are compiling a WDM or WDF (i.e., KMDF) driver,
        you must include wdm.h/wdf.h before including ndis.h, and also define:
            #define NDIS_WDM

    2.  If you are compiling any other kernel-mode code (including protocol
        drivers, lightweight filters, or generic code not using the NDIS
        driver model), define:
            #define NDIS630

    3.  An IM driver, because it is both a protocol and a miniport, should
        follow both 1. and 2. above.

    4.  If you would like to use NDIS definitions from user-mode, do not
        include this ndis.h header.  Instead, include ntddndis.h from the SDK.
        Before including it, include windows.h, and define:
            #define UM_NDIS630

    Definitions with NDIS version numbers may use any of the following:

        Version     First available in
        ------------------------------------------------------------------
        640         Windows 8.1 / Windows Server 2012 R2
        630         Windows 8 / Windows Server 2012
        620         Windows 7 / Windows Server 2008 R2
        61          Windows Vista SP1 / Windows Server 2008 RTM
        60          Windows Vista RTM
        52          Windows Server 2003 R2 / Windows Server 2003 + SNP
        51          Windows XP / Windows Server 2003
        50          Windows 2000
        40          Windows 95

    Code should define only the versions it explicitly supports at runtime.  In
    most cases, this is exactly one version (e.g., your driver only defines
    NDIS630 and no other versions).  But if you have a driver that can register
    either a 6.0 or a 6.20 protocol at runtime based on the results of
    NdisGetVersion(), then you may define support for multiple macros (e.g.,
    define both NDIS60 and NDIS630).

    Your code may inspect the value of the following macros, but you must not
    attempt to define them directly:


        NDIS_MINIPORT_MAJOR_VERSION     For miniports, contains the major
                                        version of the NDIS contract (example:
                                        6 for a 6.20 miniport)

        NDIS_MINIPORT_MINOR_VERSION     For miniports, contains the major
                                        version of the NDIS contract (example:
                                        20 for a 6.20 miniport)

        NDIS_PROTOCOL_MAJOR_VERSION     For protocols or lightweight filters,
        NDIS_FILTER_MAJOR_VERSION       contains the major version of the NDIS
                                        contract (example: 6 for a 6.20 LWF)

        NDIS_PROTOCOL_MINOR_VERSION     For protocols or lightweight filters,
        NDIS_FILTER_MINOR_VERSION       contains the minor version of the NDIS
                                        contract (example: 20 for a 6.20 LWF)

        NDIS_SUPPORT_NDIS6              For all kernel-mode code, this is
        NDIS_SUPPORT_NDIS61             defined if the driver supports that
        NDIS_SUPPORT_NDIS620            version of NDIS.  The versioning is
        NDIS_SUPPORT_NDIS630            cumulative, so for example, a 6.20
                                        miniport will see all of
                                        NDIS_SUPPORT_NDIS620,
                                        NDIS_SUPPORT_NDIS61, and
                                        NDIS_SUPPORT_NDIS6.

    Do not define BINARY_COMPATIBLE, USE_KLOCKS, or WIRELESS_WAN; these are
    deprecated.  Do not define NDIS_WRAPPER; it is reserved for use by NDIS.

--*/

#if !defined(_NDIS_)
#define _NDIS_

#if !defined(NDIS_WDM)
#define NDIS_WDM        0
#endif


//
// Set BINARY_COMPATIBLE to 0 if it is not defined. Error out if user tries
// to build a Win9x binary compatible driver using this DDK.
//

#if !defined(BINARY_COMPATIBLE)
#define BINARY_COMPATIBLE 0
#else
#if (BINARY_COMPATIBLE != 0)
#error Can not build Win9x binary compatible drivers. Please remove the definition for BINARY_COMPATIBLE or set it to 0
#endif
#endif


//
// BEGIN INTERNAL DEFINITIONS
//


#include <ntddk.h>

//
// END INTERNAL DEFINITIONS
//
// The following definitions may be used by NDIS drivers, except as noted.
//

//
//    Copyright (C) Microsoft.  All rights reserved.
//
#ifndef __NET_PNP__
#define __NET_PNP__

#pragma once

//
//  PnP and PM event codes that can be indicated up to transports
//  and clients.
//
typedef enum _NET_PNP_EVENT_CODE
{
    NetEventSetPower,
    NetEventQueryPower,
    NetEventQueryRemoveDevice,
    NetEventCancelRemoveDevice,
    NetEventReconfigure,
    NetEventBindList,
    NetEventBindsComplete,
    NetEventPnPCapabilities,
    NetEventPause,
    NetEventRestart,
    NetEventPortActivation,
    NetEventPortDeactivation,
    NetEventIMReEnableDevice,
    NetEventNDKEnable,
    NetEventNDKDisable,
    NetEventFilterPreDetach,
    NetEventBindFailed,
    NetEventSwitchActivate,
    NetEventMaximum
} NET_PNP_EVENT_CODE, *PNET_PNP_EVENT_CODE;

//
//  Networking PnP event indication structure.
//
typedef struct _NET_PNP_EVENT
{
    //
    //  Event code describing action to take.
    //
    NET_PNP_EVENT_CODE  NetEvent;

    //
    //  Event specific data.
    //
    PVOID               Buffer;

    //
    //  Length of event specific data.
    //
    ULONG               BufferLength;

    //
    //  Reserved values are for use by respective components only.
    //
    //  Note: these reserved areas must be pointer aligned.
    //  

    ULONG_PTR           NdisReserved[4];
    ULONG_PTR           TransportReserved[4];
    ULONG_PTR           TdiReserved[4];
    ULONG_PTR           TdiClientReserved[4];
} NET_PNP_EVENT, *PNET_PNP_EVENT;

//
//  The following structure defines the device power states.
//
typedef enum _NET_DEVICE_POWER_STATE
{
    NetDeviceStateUnspecified = 0,
    NetDeviceStateD0,
    NetDeviceStateD1,
    NetDeviceStateD2,
    NetDeviceStateD3,
    NetDeviceStateMaximum
} NET_DEVICE_POWER_STATE, *PNET_DEVICE_POWER_STATE;

#endif // __NET_PNP__

//
//    Copyright (C) Microsoft.  All rights reserved.
//
#pragma once

//
// Indicate that we're building for NT. NDIS_NT is always used for
// miniport builds.
//

#define NDIS_NT 1

#if defined(NDIS_DOS)
#undef NDIS_DOS
#endif

//
// Define status codes and event log codes.
//

#include <ntstatus.h>
#include <netevent.h>
#include <driverspecs.h>

#pragma warning(push)


#pragma warning(disable:4201) // (nonstandard extension used : nameless struct/union)
#pragma warning(disable:4214) // (extension used : bit field types other than int)


//
// Define a couple of extra types.
//

#if !defined(_WINDEF_)      // these are defined in windows.h too
typedef signed int INT, *PINT;
typedef unsigned int UINT, *PUINT;
#endif

typedef UNICODE_STRING NDIS_STRING, *PNDIS_STRING;
typedef PCUNICODE_STRING PCNDIS_STRING;

//
// Landmarks for PFD and SDV to detect kernel mode drivers
//

__drv_Mode_impl(NDIS_INCLUDED)

//
// Portability extensions
//

#define NDIS_INIT_FUNCTION(_F)      alloc_text(INIT,_F)
#define NDIS_PAGABLE_FUNCTION(_F)   alloc_text(PAGE,_F)
#define NDIS_PAGEABLE_FUNCTION(_F)  alloc_text(PAGE,_F)

//
// This file contains the definition of an NDIS_OID as
// well as #defines for all the current OID values.
//

//
// Define NDIS_STATUS and NDIS_HANDLE here
//
typedef PVOID NDIS_HANDLE, *PNDIS_HANDLE;

typedef _Return_type_success_(return >= 0) int NDIS_STATUS, *PNDIS_STATUS; // note default size

#if (!defined(NDIS_WRAPPER))


#if defined(NTDDI_VERSION)
#if (NTDDI_VERSION > NTDDI_WINBLUE)
#error "unsupported NTDDI_VERSION"
#endif
#endif


#if (defined(_M_IX86) || defined(_M_AMD64))
#  define NDIS_MIN_API 0x400
#else
#  define NDIS_MIN_API 0x630
#endif
#define NDIS_API_VERSION_AVAILABLE(major,minor) ((((0x ## major) << 8) + (0x ## minor)) >= NDIS_MIN_API)

//
//
// error out if driver has defined these values
//
#if (defined(NDIS_MINIPORT_MAJOR_VERSION) || \
    (defined(NDIS_MINIPORT_MINOR_VERSION)) || \
    (defined(NDIS_PROTOCOL_MAJOR_VERSION)) || \
    (defined(NDIS_PROTOCOL_MINOR_VERSION)) || \
    (defined(NDIS_FILTER_MAJOR_VERSION)) || \
    (defined(NDIS_FILTER_MINOR_VERSION)))
#error NDIS: Driver is re-defining NDIS reserved macros
#endif

#if (defined(NDIS_MINIPORT_DRIVER))

//
// for Miniports versions 5.0 and up, provide a consistent way to match
// Ndis version in their characteristics with their makefile defines
//
#if (defined(NDIS640_MINIPORT))
#define NDIS_MINIPORT_MAJOR_VERSION 6
#define NDIS_MINIPORT_MINOR_VERSION 40
#elif (defined(NDIS630_MINIPORT))
#define NDIS_MINIPORT_MAJOR_VERSION 6
#define NDIS_MINIPORT_MINOR_VERSION 30
#elif (defined(NDIS620_MINIPORT))
#define NDIS_MINIPORT_MAJOR_VERSION 6
#define NDIS_MINIPORT_MINOR_VERSION 20
#elif (defined(NDIS61_MINIPORT))
#define NDIS_MINIPORT_MAJOR_VERSION 6
#define NDIS_MINIPORT_MINOR_VERSION 1
#elif (defined(NDIS60_MINIPORT))
#define NDIS_MINIPORT_MAJOR_VERSION 6
#define NDIS_MINIPORT_MINOR_VERSION 0
#elif (defined(NDIS51_MINIPORT))
#define NDIS_MINIPORT_MAJOR_VERSION 5
#define NDIS_MINIPORT_MINOR_VERSION 1
#elif (defined(NDIS50_MINIPORT))
#define NDIS_MINIPORT_MAJOR_VERSION 5
#define NDIS_MINIPORT_MINOR_VERSION 0
#else
#error NDIS: Only NDIS miniport drivers with version >= 5 are supported
#endif

#if !NDIS_API_VERSION_AVAILABLE(NDIS_MINIPORT_MAJOR_VERSION, NDIS_MINIPORT_MINOR_VERSION)
#  error NDIS: This NDIS API version is not supported on the target architecture.
#endif

//
// If a single miniport driver supports multiple NDIS contract versions, it may
// define multiple NDISXX_MINIPORT macros.  The previously- calculated
// NDIS_MINIPORT_MAJOR_VERSION gets the *maximum* version; now calculate the
// *minimum* requested version.  In the common case, only a single
// NDISXX_MINIPORT macro will be defined, and the minimum will be equal to the
// maximum.
//
#if (defined(NDIS50_MINIPORT))
#define NDIS_MINIPORT_MINIMUM_MAJOR_VERSION 5
#define NDIS_MINIPORT_MINIMUM_MINOR_VERSION 0
#elif (defined(NDIS51_MINIPORT))
#define NDIS_MINIPORT_MINIMUM_MAJOR_VERSION 5
#define NDIS_MINIPORT_MINIMUM_MINOR_VERSION 1
#elif (defined(NDIS60_MINIPORT))
#define NDIS_MINIPORT_MINIMUM_MAJOR_VERSION 6
#define NDIS_MINIPORT_MINIMUM_MINOR_VERSION 0
#elif (defined(NDIS61_MINIPORT))
#define NDIS_MINIPORT_MINIMUM_MAJOR_VERSION 6
#define NDIS_MINIPORT_MINIMUM_MINOR_VERSION 1
#elif (defined(NDIS620_MINIPORT))
#define NDIS_MINIPORT_MINIMUM_MAJOR_VERSION 6
#define NDIS_MINIPORT_MINIMUM_MINOR_VERSION 20
#elif (defined(NDIS630_MINIPORT))
#define NDIS_MINIPORT_MINIMUM_MAJOR_VERSION 6
#define NDIS_MINIPORT_MINIMUM_MINOR_VERSION 30
#elif (defined(NDIS640_MINIPORT))
#define NDIS_MINIPORT_MINIMUM_MAJOR_VERSION 6
#define NDIS_MINIPORT_MINIMUM_MINOR_VERSION 40
#endif

//
// Disallow invalid major/minor combination
//
#if ((NDIS_MINIPORT_MAJOR_VERSION == 6) && \
       (NDIS_MINIPORT_MINOR_VERSION != 40) && \
       (NDIS_MINIPORT_MINOR_VERSION != 30) && \
       (NDIS_MINIPORT_MINOR_VERSION != 20) && \
       (NDIS_MINIPORT_MINOR_VERSION != 1) && \
       (NDIS_MINIPORT_MINOR_VERSION != 0))
#error NDIS: Invalid Miniport major/minor version
#elif ((NDIS_MINIPORT_MAJOR_VERSION == 5) && \
       (NDIS_MINIPORT_MINOR_VERSION != 1) && \
       (NDIS_MINIPORT_MINOR_VERSION != 0))
#error NDIS: Invalid Miniport major/minor version
#endif




//
// Make sure the target platform is consistent with miniport version
//
#if  (NDIS_MINIPORT_MINIMUM_MAJOR_VERSION == 6) && \
     ((NDIS_MINIPORT_MINIMUM_MINOR_VERSION == 40 && NTDDI_VERSION < NTDDI_WIN8)  || \
      (NDIS_MINIPORT_MINIMUM_MINOR_VERSION == 30 && NTDDI_VERSION < NTDDI_WIN8)  || \
      (NDIS_MINIPORT_MINIMUM_MINOR_VERSION == 20 && NTDDI_VERSION < NTDDI_WIN7)  || \
      (NDIS_MINIPORT_MINIMUM_MINOR_VERSION == 1 && NTDDI_VERSION < NTDDI_VISTA)  || \
      (NDIS_MINIPORT_MINIMUM_MINOR_VERSION == 0 && NTDDI_VERSION < NTDDI_VISTA))
#error NDIS: Wrong NDIS or DDI version specified
#elif ((NDIS_MINIPORT_MINIMUM_MAJOR_VERSION == 5) && \
       (((NDIS_MINIPORT_MINIMUM_MINOR_VERSION == 1) && (NTDDI_VERSION < NTDDI_WINXP)) || \
         ((NDIS_MINIPORT_MINIMUM_MINOR_VERSION == 0) && (NTDDI_VERSION < NTDDI_WIN2K))))
#error NDIS: Wrong NDIS or DDI version specified
#endif


#endif // NDIS_MINIPORT_DRIVER



#if (defined(NDIS30))
#error NDIS: Only NDIS Protocol drivers versions >= 4 are supported
#endif

//
// for protocol versions 4.0 and up, provide a consistent way to match
// Ndis version in their characteristics with their makefile defines
//
//
// a protocol only or filter driver
//

#if (defined(NDIS640))
#define NDIS_PROTOCOL_MAJOR_VERSION 6
#define NDIS_PROTOCOL_MINOR_VERSION 40
#define NDIS_FILTER_MAJOR_VERSION 6
#define NDIS_FILTER_MINOR_VERSION 40
#elif (defined(NDIS630))
#define NDIS_PROTOCOL_MAJOR_VERSION 6
#define NDIS_PROTOCOL_MINOR_VERSION 30
#define NDIS_FILTER_MAJOR_VERSION 6
#define NDIS_FILTER_MINOR_VERSION 30
#elif (defined(NDIS620))
#define NDIS_PROTOCOL_MAJOR_VERSION 6
#define NDIS_PROTOCOL_MINOR_VERSION 20
#define NDIS_FILTER_MAJOR_VERSION 6
#define NDIS_FILTER_MINOR_VERSION 20
#elif (defined(NDIS61))
#define NDIS_PROTOCOL_MAJOR_VERSION 6
#define NDIS_PROTOCOL_MINOR_VERSION 1
#define NDIS_FILTER_MAJOR_VERSION 6
#define NDIS_FILTER_MINOR_VERSION 1
#elif (defined(NDIS60))
#define NDIS_PROTOCOL_MAJOR_VERSION 6
#define NDIS_PROTOCOL_MINOR_VERSION 0
#define NDIS_FILTER_MAJOR_VERSION 6
#define NDIS_FILTER_MINOR_VERSION 0
#elif (defined(NDIS51))
#define NDIS_PROTOCOL_MAJOR_VERSION 5
#define NDIS_PROTOCOL_MINOR_VERSION 1
#elif (defined(NDIS50))
#define NDIS_PROTOCOL_MAJOR_VERSION 5
#define NDIS_PROTOCOL_MINOR_VERSION 0
#elif(defined(NDIS40))
#define NDIS_PROTOCOL_MAJOR_VERSION 4
#define NDIS_PROTOCOL_MINOR_VERSION 0
#endif // (defined(NDIS60))


//
// If a single protocol/filter driver supports multiple NDIS contract versions,
// it may define multiple NDISXX macros.  The previously- calculated
// NDIS_XXXX_MAJOR_VERSION gets the *maximum* version; now calculate the
// *minimum* requested version.  In the common case, only a single NDISXX macro
// will be defined, and the minimum will be equal to the maximum.
//
#if (defined(NDIS40))
#define NDIS_PROTOCOL_MINIMUM_MAJOR_VERSION 4
#define NDIS_PROTOCOL_MINIMUM_MINOR_VERSION 0
#elif (defined(NDIS50))
#define NDIS_PROTOCOL_MINIMUM_MAJOR_VERSION 5
#define NDIS_PROTOCOL_MINIMUM_MINOR_VERSION 0
#elif (defined(NDIS51))
#define NDIS_PROTOCOL_MINIMUM_MAJOR_VERSION 5
#define NDIS_PROTOCOL_MINIMUM_MINOR_VERSION 1
#elif (defined(NDIS60))
#define NDIS_PROTOCOL_MINIMUM_MAJOR_VERSION 6
#define NDIS_PROTOCOL_MINIMUM_MINOR_VERSION 0
#define NDIS_FILTER_MINIMUM_MAJOR_VERSION 6
#define NDIS_FILTER_MINIMUM_MINOR_VERSION 0
#elif (defined(NDIS61))
#define NDIS_PROTOCOL_MINIMUM_MAJOR_VERSION 6
#define NDIS_PROTOCOL_MINIMUM_MINOR_VERSION 1
#define NDIS_FILTER_MINIMUM_MAJOR_VERSION 6
#define NDIS_FILTER_MINIMUM_MINOR_VERSION 1
#elif (defined(NDIS620))
#define NDIS_PROTOCOL_MINIMUM_MAJOR_VERSION 6
#define NDIS_PROTOCOL_MINIMUM_MINOR_VERSION 20
#define NDIS_FILTER_MINIMUM_MAJOR_VERSION 6
#define NDIS_FILTER_MINIMUM_MINOR_VERSION 20
#elif (defined(NDIS630))
#define NDIS_PROTOCOL_MINIMUM_MAJOR_VERSION 6
#define NDIS_PROTOCOL_MINIMUM_MINOR_VERSION 30
#define NDIS_FILTER_MINIMUM_MAJOR_VERSION 6
#define NDIS_FILTER_MINIMUM_MINOR_VERSION 30
#elif (defined(NDIS640))
#define NDIS_PROTOCOL_MINIMUM_MAJOR_VERSION 6
#define NDIS_PROTOCOL_MINIMUM_MINOR_VERSION 40
#define NDIS_FILTER_MINIMUM_MAJOR_VERSION 6
#define NDIS_FILTER_MINIMUM_MINOR_VERSION 40
#endif


#if (!defined(NDIS_MINIPORT_DRIVER) && !defined (NDIS_PROTOCOL_MAJOR_VERSION))


#if defined(_M_IX86) || defined(_M_AMD64)
//
// For compatibility with Win7 code, if the code doesn't define any version,
// and it's not a miniport, default to NDIS 4.0 protocol driver.
//
#  define NDIS40
#  define NDIS_PROTOCOL_MAJOR_VERSION 4
#  define NDIS_PROTOCOL_MINOR_VERSION 0
#else // it's not x86 or AMD64, so no compatibility to maintain
// Force the code to define *some* version.  6.40 is a good choice.
#  error NDIS: Define version with NDIS### or NDIS###_MINIPORT (see top of NDIS.H)
#endif
#endif // (!defined(NDIS_MINIPORT_DRIVER) && !defined (NDIS_PROTOCOL_MAJOR_VERSION))


#if defined (NDIS_FILTER_MAJOR_VERSION)

#if !NDIS_API_VERSION_AVAILABLE(NDIS_FILTER_MAJOR_VERSION, NDIS_FILTER_MINOR_VERSION)
#  error NDIS: This NDIS API version is not supported on the target architecture.
#endif

//
// disallow invalid major/minor combination
//
#if ((NDIS_FILTER_MAJOR_VERSION == 6) && \
     (NDIS_FILTER_MINOR_VERSION != 40) && \
     (NDIS_FILTER_MINOR_VERSION != 30) && \
     (NDIS_FILTER_MINOR_VERSION != 20) && \
     (NDIS_FILTER_MINOR_VERSION != 1) && \
     (NDIS_FILTER_MINOR_VERSION != 0))
#error NDIS: Invalid Filter version
#endif
#endif // defined (NDIS_FILTER_MAJOR_VERSION)


#if defined (NDIS_PROTOCOL_MAJOR_VERSION)

#if !NDIS_API_VERSION_AVAILABLE(NDIS_PROTOCOL_MAJOR_VERSION, NDIS_PROTOCOL_MINOR_VERSION)
#  error NDIS: This NDIS API version is not supported on the target architecture.
#endif

//
// disallow invalid major/minor combination
//
#if ((NDIS_PROTOCOL_MAJOR_VERSION == 6) && \
     (NDIS_PROTOCOL_MINOR_VERSION != 40) && \
     (NDIS_PROTOCOL_MINOR_VERSION != 30) && \
     (NDIS_PROTOCOL_MINOR_VERSION != 20) && \
     (NDIS_PROTOCOL_MINOR_VERSION != 1) && \
     (NDIS_PROTOCOL_MINOR_VERSION != 0))
#error NDIS: Invalid Protocol version
#elif ((NDIS_PROTOCOL_MAJOR_VERSION == 5) && \
     (NDIS_PROTOCOL_MINOR_VERSION != 1) && (NDIS_PROTOCOL_MINOR_VERSION != 0))
#error NDIS: Invalid Protocol version
#elif ((NDIS_PROTOCOL_MAJOR_VERSION == 4) && (NDIS_PROTOCOL_MINOR_VERSION != 0))
#error NDIS: Invalid Protocol major/minor version
#endif


#if defined(NDIS_STRICT_PROTOCOL_VERSION_CHECK)
//
// Make sure the target platform is consistent with protocol version
//
#if  (NDIS_PROTOCOL_MINIMUM_MAJOR_VERSION == 6) && \
     ((NDIS_PROTOCOL_MINIMUM_MINOR_VERSION == 40 && NTDDI_VERSION < NTDDI_WIN8)  || \
      (NDIS_PROTOCOL_MINIMUM_MINOR_VERSION == 30 && NTDDI_VERSION < NTDDI_WIN8)  || \
      (NDIS_PROTOCOL_MINIMUM_MINOR_VERSION == 20 && NTDDI_VERSION < NTDDI_WIN7)  || \
      (NDIS_PROTOCOL_MINIMUM_MINOR_VERSION == 1 && NTDDI_VERSION < NTDDI_VISTA)  || \
      (NDIS_PROTOCOL_MINIMUM_MINOR_VERSION == 0 && NTDDI_VERSION < NTDDI_VISTA))
#error NDIS: Wrong NDIS or DDI version specified
#elif ((NDIS_PROTOCOL_MINIMUM_MAJOR_VERSION == 5) && \
       (((NDIS_PROTOCOL_MINIMUM_MINOR_VERSION == 1) && (NTDDI_VERSION < NTDDI_WINXP)) || \
         ((NDIS_PROTOCOL_MINIMUM_MINOR_VERSION == 0) && (NTDDI_VERSION < NTDDI_WIN2K))))
#error NDIS: Wrong NDIS or DDI version specified
#endif

#endif // defined (NDIS_PROTOCOL_MAJOR_VERSION)

#else // NDIS_STRICT_PROTOCOL_VERSION_CHECK

//
// make sure the target platform is consistent with protocol version
// but don't stop NDIS protocol drivers older than 6.0 from specifying NTDDI_VISTA
//
#if  ((NDIS_PROTOCOL_MAJOR_VERSION == 6) && (NTDDI_VERSION < NTDDI_VISTA))
#error NDIS: Wrong NDIS or DDI version specified
#endif

#endif // NDIS_STRICT_PROTOCOL_VERSION_CHECK


#endif // !NDIS_WRAPPER

//
// identify Legacy miniport drivers
//
#if !defined(NDIS_LEGACY_MINIPORT)
#if ((defined(NDIS_MINIPORT_DRIVER) && (NDIS_MINIPORT_MAJOR_VERSION < 6)) || NDIS_WRAPPER)
#define NDIS_LEGACY_MINIPORT    1
#else
#define NDIS_LEGACY_MINIPORT    0
#endif // ((defined(NDIS_MINIPORT_DRIVER) && (NDIS_MINIPORT_MAJOR_VERSION < 6)) || NDIS_WRAPPER)
#endif // !defined(NDIS_LEGACY_MINIPORT)


//
// identify Legacy protocol drivers
//
#if !defined(NDIS_LEGACY_PROTOCOL)
#if ((defined(NDIS_PROTOCOL_MAJOR_VERSION) && (NDIS_PROTOCOL_MAJOR_VERSION < 6)) || NDIS_WRAPPER)
#define NDIS_LEGACY_PROTOCOL    1
#else
#define NDIS_LEGACY_PROTOCOL    0
#endif // ((defined(NDIS_PROTOCOL_MAJOR_VERSION) && (NDIS_PROTOCOL_MAJOR_VERSION < 6)) || NDIS_WRAPPER)
#endif // !defined(NDIS_LEGACY_PROTOCOL)


//
// use something to identify legacy (pre NDIS 6 drivers) + NDIS itself
//
#if !defined(NDIS_LEGACY_DRIVER)
#if  (NDIS_LEGACY_MINIPORT || NDIS_LEGACY_PROTOCOL || NDIS_WRAPPER)
#define NDIS_LEGACY_DRIVER     1
#else
#define NDIS_LEGACY_DRIVER     0
#endif  // either protocol is legacy or miniport is legacy or this is NDIS
#endif // !defined(NDIS_LEGACY_DRIVER)


//
// and something to identify Vista+ drivers + NDIS itself
//
#if !defined(NDIS_SUPPORT_NDIS6)
#if  ((defined (NDIS_MINIPORT_MAJOR_VERSION) && (NDIS_MINIPORT_MAJOR_VERSION >= 6)) || \
    (defined (NDIS60)) || NDIS_WRAPPER)
#define NDIS_SUPPORT_NDIS6      1
#else
#define NDIS_SUPPORT_NDIS6      0
#endif
#endif // !defined(NDIS_SUPPORT_NDIS6)


//
// and something to identify Server 2008+ drivers + NDIS itself
//
#if !defined(NDIS_SUPPORT_NDIS61)
#if  (((defined (NDIS_MINIPORT_MAJOR_VERSION) && (NDIS_MINIPORT_MAJOR_VERSION >= 6)) && \
       (defined (NDIS_MINIPORT_MINOR_VERSION) && (NDIS_MINIPORT_MINOR_VERSION >= 1))) || \
      (defined (NDIS61)) || NDIS_WRAPPER)
#define NDIS_SUPPORT_NDIS61      1
#else
#define NDIS_SUPPORT_NDIS61      0
#endif
#endif // !defined(NDIS_SUPPORT_NDIS61)


//
// and something to identify new (Win7 and up) drivers + NDIS itself
//
#if !defined(NDIS_SUPPORT_NDIS620)
#if  (((defined (NDIS_MINIPORT_MAJOR_VERSION) && (NDIS_MINIPORT_MAJOR_VERSION >= 6)) && \
       (defined (NDIS_MINIPORT_MINOR_VERSION) && (NDIS_MINIPORT_MINOR_VERSION >= 20))) || \
      (defined (NDIS620)) || NDIS_WRAPPER)
#define NDIS_SUPPORT_NDIS620      1
#else
#define NDIS_SUPPORT_NDIS620      0
#endif
#endif // !defined(NDIS_SUPPORT_NDIS620)

//
// and something to identify new (Win8 and up) drivers + NDIS itself
//
#if !defined(NDIS_SUPPORT_NDIS630)
#if  (((defined (NDIS_MINIPORT_MAJOR_VERSION) && (NDIS_MINIPORT_MAJOR_VERSION >= 6)) && \
       (defined (NDIS_MINIPORT_MINOR_VERSION) && (NDIS_MINIPORT_MINOR_VERSION >= 30))) || \
      (defined (NDIS630)) || NDIS_WRAPPER)
#define NDIS_SUPPORT_NDIS630      1
#else
#define NDIS_SUPPORT_NDIS630      0
#endif
#endif // !defined(NDIS_SUPPORT_NDIS630)

//
// and something to identify new (Win8.1 and up) drivers + NDIS itself
//
#if !defined(NDIS_SUPPORT_NDIS640)
#if  (((defined (NDIS_MINIPORT_MAJOR_VERSION) && (NDIS_MINIPORT_MAJOR_VERSION >= 6)) && \
       (defined (NDIS_MINIPORT_MINOR_VERSION) && (NDIS_MINIPORT_MINOR_VERSION >= 40))) || \
      (defined (NDIS640)) || NDIS_WRAPPER)
#define NDIS_SUPPORT_NDIS640      1
#else
#define NDIS_SUPPORT_NDIS640      0
#endif
#endif // !defined(NDIS_SUPPORT_NDIS640)


//
// Enable NDIS630 defines for NDIS 640 drivers
//
#if (NDIS_SUPPORT_NDIS640)
#undef NDIS_SUPPORT_NDIS630
#define NDIS_SUPPORT_NDIS630      1
#endif

//
// Enable NDIS620 defines for NDIS 630 drivers
//
#if (NDIS_SUPPORT_NDIS630)
#undef NDIS_SUPPORT_NDIS620
#define NDIS_SUPPORT_NDIS620      1
#endif

//
// Enable NDIS61 defines for NDIS 620 drivers
//
#if (NDIS_SUPPORT_NDIS620)
#undef NDIS_SUPPORT_NDIS61
#define NDIS_SUPPORT_NDIS61      1
#endif

//
// Enable NDIS60 defines for NDIS 61 drivers
//
#if (NDIS_SUPPORT_NDIS61)
#undef NDIS_SUPPORT_NDIS6
#define NDIS_SUPPORT_NDIS6      1
#endif

//
// Enable deprecated NDIS 6.0/1 APIs for NDIS 6.20+ drivers
// that also want to run the same binary on NDIS 6.0/1
// In this case, such a driver would need to define both
// NDIS620_MINIPORT and NDIS60/61_MINIPORT
// Note: We cannot use NDIS_SUPPORT_NDIS6/61 in this check because
// that would be defined even for NDIS 6.20+
//
#if defined(NDIS61_MINIPORT) || defined(NDIS60_MINIPORT) ||   \
    defined(NDIS61) || defined(NDIS60) ||                     \
    defined(NDIS_WRAPPER) || defined(NDIS_LEGACY_DRIVER)
#define NDIS_SUPPORT_60_COMPATIBLE_API      1
#else
#define NDIS_SUPPORT_60_COMPATIBLE_API      0
#endif

//
// These constants may be compared against values returned by NdisGetVersion.
//
#define NDIS_RUNTIME_VERSION_60     (6 << 16)
#define NDIS_RUNTIME_VERSION_61     ((6 << 16) | 1)
#define NDIS_RUNTIME_VERSION_620    ((6 << 16) | 20)
#define NDIS_RUNTIME_VERSION_630    ((6 << 16) | 30)
#define NDIS_RUNTIME_VERSION_640    ((6 << 16) | 40)


#define NDIS_DECLARE_CONTEXT_INNER(datatype,purpose) \
    (sizeof(datatype), __annotation(L"ms-contexttype", purpose, L ## #datatype))

//
// Use these macros to associate your context pointer with its datatype.
// The association will be saved in the PDB so !ndiskd can find it.
// Then !ndiskd will link to your driver's context, making debugging easier.
// There is no runtime overhead or effect of using this.
//

// Miniport Driver context passed to NdisMRegisterMiniportDriver
#define NDIS_DECLARE_MINIPORT_DRIVER_CONTEXT(type) NDIS_DECLARE_CONTEXT_INNER(type, L"ndis-miniport-driver")

// Miniport Adapter context from NDIS_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES
#define NDIS_DECLARE_MINIPORT_ADAPTER_CONTEXT(type) NDIS_DECLARE_CONTEXT_INNER(type, L"ndis-miniport-adapter")

// Miniport AddDevice context from NDIS_MINIPORT_ADD_DEVICE_REGISTRATION_ATTRIBUTES
#define NDIS_DECLARE_MINIPORT_ADDDEVICE_CONTEXT(type) NDIS_DECLARE_CONTEXT_INNER(type, L"ndis-miniport-adddevice")

// Miniport Interrupt context passed to NdisMRegisterInterruptEx
#define NDIS_DECLARE_MINIPORT_INTERRUPT_CONTEXT(type) NDIS_DECLARE_CONTEXT_INNER(type, L"ndis-miniport-interrupt")

// Miniport SG-DMA context passed to NdisMAllocateNetBufferSGList
#define NDIS_DECLARE_MINIPORT_SGLIST_CONTEXT(type) NDIS_DECLARE_CONTEXT_INNER(type, L"ndis-miniport-sglist")

// Miniport Async Shared DMA context passed to NdisMAllocateSharedMemoryAsyncEx
#define NDIS_DECLARE_MINIPORT_SHARED_DMA_ASYNC_CONTEXT(type) NDIS_DECLARE_CONTEXT_INNER(type, L"ndis-miniport-shdma")

// Filter Driver context passed to NdisFRegisterFilterDriver
#define NDIS_DECLARE_FILTER_DRIVER_CONTEXT(type) NDIS_DECLARE_CONTEXT_INNER(type, L"ndis-filter-driver")

// Filter Module context passed to NdisFSetAttributes
#define NDIS_DECLARE_FILTER_MODULE_CONTEXT(type) NDIS_DECLARE_CONTEXT_INNER(type, L"ndis-filter-module")

// Protocol Driver context passed to NdisRegisterProtocolDriver
#define NDIS_DECLARE_PROTOCOL_DRIVER_CONTEXT(type) NDIS_DECLARE_CONTEXT_INNER(type, L"ndis-protocol-driver")

// Open Binding context passed to NdisOpenAdapterEx
#define NDIS_DECLARE_PROTOCOL_OPEN_CONTEXT(type) NDIS_DECLARE_CONTEXT_INNER(type, L"ndis-protocol-open")

// CoNDIS Client AF context passed to NdisClOpenAddressFamilyEx
#define NDIS_DECLARE_CO_CL_AF_CONTEXT(type) NDIS_DECLARE_CONTEXT_INNER(type, L"ndis-co-cl-af")

// CoNDIS Client VC context returned from ProtocolCoCreateVc or passed to NdisCoCreateVc
#define NDIS_DECLARE_CO_CL_VC_CONTEXT(type) NDIS_DECLARE_CONTEXT_INNER(type, L"ndis-co-cl-vc")

// CoNDIS Call Manager AF context returned from ProtocolCmOpenAf or passed to NdisCmOpenAddressFamilyComplete
#define NDIS_DECLARE_CO_CM_AF_CONTEXT(type) NDIS_DECLARE_CONTEXT_INNER(type, L"ndis-co-cm-af")

// CoNDIS Call Manager VC context returned from ProtocolCoCreateVc or passed to NdisCoCreateVc
#define NDIS_DECLARE_CO_CM_VC_CONTEXT(type) NDIS_DECLARE_CONTEXT_INNER(type, L"ndis-co-cm-vc")

// CoNDIS Miniport VC context returned from MiniportCoCreateVc
#define NDIS_DECLARE_CO_MINIPORT_VC_CONTEXT(type) NDIS_DECLARE_CONTEXT_INNER(type, L"ndis-co-miniport-vc")

// Shared Memory Provider context passed in NDIS_SHARED_MEMORY_PROVIDER_CHARACTERISTICS
#define NDIS_DECLARE_SHARED_MEMORY_PROVIDER_CONTEXT(type) NDIS_DECLARE_CONTEXT_INNER(type, L"ndis-shm-provider")

// Shared Memory Allocation context returned from NetAllocateSharedMemory
#define NDIS_DECLARE_SHARED_MEMORY_ALLOCATION_CONTEXT(type) NDIS_DECLARE_CONTEXT_INNER(type, L"ndis-shm-block")


#include <ntddndis.h>

//
// Ndis defines for configuration manager data structures
//
typedef CM_MCA_POS_DATA NDIS_MCA_POS_DATA, *PNDIS_MCA_POS_DATA;
typedef CM_EISA_SLOT_INFORMATION NDIS_EISA_SLOT_INFORMATION, *PNDIS_EISA_SLOT_INFORMATION;
typedef CM_EISA_FUNCTION_INFORMATION NDIS_EISA_FUNCTION_INFORMATION, *PNDIS_EISA_FUNCTION_INFORMATION;

//
// Define an exported function.
//
#ifndef NDIS_WRAPPER
#  define EXPORT
#endif

#if NDIS_SUPPORT_NDIS6
typedef struct _NDIS_GENERIC_OBJECT
{
    NDIS_OBJECT_HEADER  Header;
    PVOID               Caller;
    PVOID               CallersCaller;
    PDRIVER_OBJECT      DriverObject;
} NDIS_GENERIC_OBJECT, *PNDIS_GENERIC_OBJECT;

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
PNDIS_GENERIC_OBJECT
NdisAllocateGenericObject(
    _In_opt_ PDRIVER_OBJECT          DriverObject,
    _In_     ULONG                   Tag,
    _In_     USHORT                  Size
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFreeGenericObject(
    _In_ PNDIS_GENERIC_OBJECT   NdisObject
    );

#endif // NDIS_SUPPORT_NDIS6


//
// Memory manipulation functions.
//
#define NdisMoveMemory(Destination, Source, Length) RtlCopyMemory(Destination, Source, Length)
#define NdisZeroMemory(Destination, Length)         RtlZeroMemory(Destination, Length)
#define NdisEqualMemory(Source1, Source2, Length)   RtlEqualMemory(Source1, Source2, Length)
#define NdisFillMemory(Destination, Length, Fill)   RtlFillMemory(Destination, Length, Fill)
#define NdisRetrieveUlong(Destination, Source)      RtlRetrieveUlong(Destination, Source)
#define NdisStoreUlong(Destination, Value)          RtlStoreUlong(Destination, Value)

#define NDIS_STRING_CONST(x)    {sizeof(L##x)-2, sizeof(L##x), L##x}

//
// On a RISC machine, I/O mapped memory can't be accessed with
// the Rtl routines.
//
#if defined(_M_IX86) || defined(_M_AMD64)

#define NdisMoveMappedMemory(Destination,Source,Length) RtlCopyMemory(Destination,Source,Length)
#define NdisZeroMappedMemory(Destination,Length)        RtlZeroMemory(Destination,Length)

#elif defined (_M_ARM)

VOID
__inline
NdisMoveMappedMemory(
    _Out_writes_bytes_all_(Length) PVOID Destination,
    _In_ PVOID Source,
    _In_ ULONG Length
    )
{
    PUCHAR _Src = (PUCHAR)Source;
    PUCHAR _Dest = (PUCHAR)Destination;
    PUCHAR _End = _Dest + Length;
    while (_Dest < _End)
    {
        *_Dest++ = *_Src++;
    }
}

VOID
__inline
NdisZeroMappedMemory(
    _Out_writes_bytes_all_(Length) PVOID Destination,
    _In_ ULONG Length
    )
{
    PUCHAR _Dest = (PUCHAR)Destination;
    PUCHAR _End = _Dest + Length;
    while (_Dest < _End)
    {
        *_Dest++ = 0;
    }
}

#endif


#define NdisMoveToMappedMemory(Destination,Source,Length)                   \
                            NdisMoveMappedMemory(Destination,Source,Length)
#define NdisMoveFromMappedMemory(Destination,Source,Length)                 \
                            NdisMoveMappedMemory(Destination,Source,Length)


//
// definition of the basic spin lock structure
//

typedef struct _NDIS_SPIN_LOCK
{
    KSPIN_LOCK  SpinLock;
    KIRQL       OldIrql;
} NDIS_SPIN_LOCK, * PNDIS_SPIN_LOCK;


//
// definition of the ndis event structure
//
typedef struct _NDIS_EVENT
{
    KEVENT      Event;
} NDIS_EVENT, *PNDIS_EVENT;


struct _NDIS_WORK_ITEM;
typedef
_IRQL_requires_(PASSIVE_LEVEL)
_IRQL_requires_same_
VOID NDIS_PROC_CALLBACK(
    _In_ struct _NDIS_WORK_ITEM * WorkItem,
    _In_opt_ PVOID Context);
typedef NDIS_PROC_CALLBACK *NDIS_PROC;

//
// Definition of an ndis work-item
//
typedef struct _NDIS_WORK_ITEM
{
    PVOID           Context;
    NDIS_PROC       Routine;
    UCHAR           WrapperReserved[8*sizeof(PVOID)];
} NDIS_WORK_ITEM, *PNDIS_WORK_ITEM;

#define NdisInterruptLatched            Latched
#define NdisInterruptLevelSensitive     LevelSensitive
typedef KINTERRUPT_MODE NDIS_INTERRUPT_MODE, *PNDIS_INTERRUPT_MODE;

//
// Configuration definitions
//

//
// Possible data types
//

typedef enum _NDIS_PARAMETER_TYPE
{
    NdisParameterInteger,
    NdisParameterHexInteger,
    NdisParameterString,
    NdisParameterMultiString,
    NdisParameterBinary
} NDIS_PARAMETER_TYPE, *PNDIS_PARAMETER_TYPE;

typedef struct
{
    USHORT          Length;

    _Field_size_bytes_(Length)
    PVOID           Buffer;
} BINARY_DATA;

//
// To store configuration information
//
typedef struct _NDIS_CONFIGURATION_PARAMETER
{
    NDIS_PARAMETER_TYPE ParameterType;
    union
    {
        ULONG           IntegerData;
        NDIS_STRING     StringData;
        BINARY_DATA     BinaryData;
    } ParameterData;
} NDIS_CONFIGURATION_PARAMETER, *PNDIS_CONFIGURATION_PARAMETER;


//
// Definitions for the "ProcessorType" keyword
//
typedef enum _NDIS_PROCESSOR_TYPE
{
    NdisProcessorX86,
    NdisProcessorMips,
    NdisProcessorAlpha,
    NdisProcessorPpc,
    NdisProcessorAmd64,
    NdisProcessorIA64,
    NdisProcessorArm
} NDIS_PROCESSOR_TYPE, *PNDIS_PROCESSOR_TYPE;

//
// Definitions for the "Environment" keyword
//
typedef enum _NDIS_ENVIRONMENT_TYPE
{
    NdisEnvironmentWindows,
    NdisEnvironmentWindowsNt
} NDIS_ENVIRONMENT_TYPE, *PNDIS_ENVIRONMENT_TYPE;


//
// Possible Hardware Architecture. Define these to
// match the HAL INTERFACE_TYPE enum.
//
typedef enum _NDIS_INTERFACE_TYPE
{
    NdisInterfaceInternal = Internal,
    NdisInterfaceIsa = Isa,
    NdisInterfaceEisa = Eisa,
    NdisInterfaceMca = MicroChannel,
    NdisInterfaceTurboChannel = TurboChannel,
    NdisInterfacePci = PCIBus,
    NdisInterfacePcMcia = PCMCIABus,
    NdisInterfaceCBus = CBus,
    NdisInterfaceMPIBus = MPIBus,
    NdisInterfaceMPSABus = MPSABus,
    NdisInterfaceProcessorInternal = ProcessorInternal,
    NdisInterfaceInternalPowerBus = InternalPowerBus,
    NdisInterfacePNPISABus = PNPISABus,
    NdisInterfacePNPBus = PNPBus,
    NdisInterfaceUSB,
    NdisInterfaceIrda,
    NdisInterface1394,
    NdisMaximumInterfaceType
} NDIS_INTERFACE_TYPE, *PNDIS_INTERFACE_TYPE;

//
// Stuff for PCI configuring
//

typedef CM_PARTIAL_RESOURCE_LIST NDIS_RESOURCE_LIST, *PNDIS_RESOURCE_LIST;


//
// The structure passed up on a WAN_LINE_UP indication
//

typedef struct _NDIS_WAN_LINE_UP
{
    IN ULONG                LinkSpeed;          // 100 bps units
    IN ULONG                MaximumTotalSize;   // suggested max for send packets
    IN NDIS_WAN_QUALITY     Quality;
    IN USHORT               SendWindow;         // suggested by the MAC
    IN UCHAR                RemoteAddress[6];
    IN OUT UCHAR            LocalAddress[6];
    IN ULONG                ProtocolBufferLength;   // Length of protocol info buffer
    IN PUCHAR               ProtocolBuffer;     // Information used by protocol
    IN USHORT               ProtocolType;       // Protocol ID
    IN OUT NDIS_STRING      DeviceName;
} NDIS_WAN_LINE_UP, *PNDIS_WAN_LINE_UP;


//
// The structure passed up on a WAN_LINE_DOWN indication
//

typedef struct _NDIS_WAN_LINE_DOWN
{
    IN UCHAR    RemoteAddress[6];
    IN UCHAR    LocalAddress[6];
} NDIS_WAN_LINE_DOWN, *PNDIS_WAN_LINE_DOWN;

//
// The structure passed up on a WAN_FRAGMENT indication
//

typedef struct _NDIS_WAN_FRAGMENT
{
    IN UCHAR    RemoteAddress[6];
    IN UCHAR    LocalAddress[6];
} NDIS_WAN_FRAGMENT, *PNDIS_WAN_FRAGMENT;

//
// The structure passed up on a WAN_GET_STATS indication
//

typedef struct _NDIS_WAN_GET_STATS
{
    IN  UCHAR   LocalAddress[6];
    OUT ULONG   BytesSent;
    OUT ULONG   BytesRcvd;
    OUT ULONG   FramesSent;
    OUT ULONG   FramesRcvd;
    OUT ULONG   CRCErrors;                      // Serial-like info only
    OUT ULONG   TimeoutErrors;                  // Serial-like info only
    OUT ULONG   AlignmentErrors;                // Serial-like info only
    OUT ULONG   SerialOverrunErrors;            // Serial-like info only
    OUT ULONG   FramingErrors;                  // Serial-like info only
    OUT ULONG   BufferOverrunErrors;            // Serial-like info only
    OUT ULONG   BytesTransmittedUncompressed;   // Compression info only
    OUT ULONG   BytesReceivedUncompressed;      // Compression info only
    OUT ULONG   BytesTransmittedCompressed;     // Compression info only
    OUT ULONG   BytesReceivedCompressed;        // Compression info only
} NDIS_WAN_GET_STATS, *PNDIS_WAN_GET_STATS;


//
// Ndis Buffer is actually an Mdl
//
typedef MDL NDIS_BUFFER, *PNDIS_BUFFER;

#if NDIS_LEGACY_DRIVER
struct _NDIS_PACKET;
typedef NDIS_HANDLE PNDIS_PACKET_POOL;

//
//
// wrapper-specific part of a packet
//
typedef struct _NDIS_PACKET_PRIVATE
{
    UINT                PhysicalCount;  // number of physical pages in packet.
    UINT                TotalLength;    // Total amount of data in the packet.
    PNDIS_BUFFER        Head;           // first buffer in the chain
    PNDIS_BUFFER        Tail;           // last buffer in the chain

    // if Head is NULL the chain is empty; Tail doesn't have to be NULL also

    PNDIS_PACKET_POOL   Pool;           // so we know where to free it back to
    UINT                Count;
    ULONG               Flags;
    BOOLEAN             ValidCounts;
    UCHAR               NdisPacketFlags;    // See fPACKET_xxx bits below
    USHORT              NdisPacketOobOffset;
} NDIS_PACKET_PRIVATE, * PNDIS_PACKET_PRIVATE;

//
// The bits define the bits in the Flags
//
#define NDIS_FLAGS_PROTOCOL_ID_MASK             0x0000000F  // The low 4 bits are defined for protocol-id
                                                            // The values are defined in ntddndis.h
#define NDIS_FLAGS_MULTICAST_PACKET             0x00000010  // don't use
#define NDIS_FLAGS_RESERVED2                    0x00000020  // don't use
#define NDIS_FLAGS_RESERVED3                    0x00000040  // don't use
#define NDIS_FLAGS_DONT_LOOPBACK                0x00000080  // Write only
#define NDIS_FLAGS_IS_LOOPBACK_PACKET           0x00000100  // Read only
#define NDIS_FLAGS_LOOPBACK_ONLY                0x00000200  // Write only
#define NDIS_FLAGS_RESERVED4                    0x00000400  // don't use
#define NDIS_FLAGS_DOUBLE_BUFFERED              0x00000800  // used by ndis
#define NDIS_FLAGS_SENT_AT_DPC                  0x00001000  // the protocol sent this packet at DPC
#define NDIS_FLAGS_USES_SG_BUFFER_LIST          0x00002000  // used by Ndis
#define NDIS_FLAGS_USES_ORIGINAL_PACKET         0x00004000  // used by Ndis
#define NDIS_FLAGS_PADDED                       0x00010000  // used by NDIS
#define NDIS_FLAGS_XLATE_AT_TOP                 0x00020000  // used by NDIS

//
// Low-bits in the NdisPacketFlags are reserved by NDIS Wrapper for internal use
//
#define fPACKET_WRAPPER_RESERVED                0x3F
#define fPACKET_CONTAINS_MEDIA_SPECIFIC_INFO    0x40
#define fPACKET_ALLOCATED_BY_NDIS               0x80

#endif // NDIS_LEGACY_DRIVER

//
// Definition for layout of the media-specific data. More than one class of media-specific
// information can be tagged onto a packet.
//
typedef enum _NDIS_CLASS_ID
{
    NdisClass802_3Priority,
    NdisClassWirelessWanMbxMailbox,
    NdisClassIrdaPacketInfo,
    NdisClassAtmAALInfo

} NDIS_CLASS_ID;

typedef struct _MEDIA_SPECIFIC_INFORMATION
{
    UINT            NextEntryOffset;
    NDIS_CLASS_ID   ClassId;
    UINT            Size;
    UCHAR           ClassInformation[1];

} MEDIA_SPECIFIC_INFORMATION, *PMEDIA_SPECIFIC_INFORMATION;

#if NDIS_LEGACY_DRIVER

typedef struct _NDIS_PACKET_OOB_DATA
{
    union
    {
        ULONGLONG   TimeToSend;
        ULONGLONG   TimeSent;
    };
    ULONGLONG       TimeReceived;
    UINT            HeaderSize;
    UINT            SizeMediaSpecificInfo;
    PVOID           MediaSpecificInformation;

    NDIS_STATUS     Status;
} NDIS_PACKET_OOB_DATA, *PNDIS_PACKET_OOB_DATA;

#define NDIS_GET_PACKET_PROTOCOL_TYPE(_Packet_) ((_Packet_)->Private.Flags & NDIS_PROTOCOL_ID_MASK)

#define NDIS_OOB_DATA_FROM_PACKET(_p)                                   \
                        (PNDIS_PACKET_OOB_DATA)((PUCHAR)(_p) +          \
                        (_p)->Private.NdisPacketOobOffset)
#define NDIS_GET_PACKET_HEADER_SIZE(_Packet)                            \
                        ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +    \
                        (_Packet)->Private.NdisPacketOobOffset))->HeaderSize
#define NDIS_GET_PACKET_STATUS(_Packet)                                 \
                        ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +    \
                        (_Packet)->Private.NdisPacketOobOffset))->Status
#define NDIS_GET_PACKET_TIME_TO_SEND(_Packet)                           \
                        ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +    \
                        (_Packet)->Private.NdisPacketOobOffset))->TimeToSend
#define NDIS_GET_PACKET_TIME_SENT(_Packet)                              \
                        ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +    \
                        (_Packet)->Private.NdisPacketOobOffset))->TimeSent
#define NDIS_GET_PACKET_TIME_RECEIVED(_Packet)                          \
                        ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +    \
                        (_Packet)->Private.NdisPacketOobOffset))->TimeReceived
#define NDIS_GET_PACKET_MEDIA_SPECIFIC_INFO(_Packet,                    \
                                            _pMediaSpecificInfo,        \
                                            _pSizeMediaSpecificInfo)    \
{                                                                       \
    if (!((_Packet)->Private.NdisPacketFlags & fPACKET_ALLOCATED_BY_NDIS) ||\
        !((_Packet)->Private.NdisPacketFlags & fPACKET_CONTAINS_MEDIA_SPECIFIC_INFO))\
    {                                                                   \
        *(_pMediaSpecificInfo) = NULL;                                  \
        *(_pSizeMediaSpecificInfo) = 0;                                 \
    }                                                                   \
    else                                                                \
    {                                                                   \
        *(_pMediaSpecificInfo) =((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +\
                    (_Packet)->Private.NdisPacketOobOffset))->MediaSpecificInformation;\
        *(_pSizeMediaSpecificInfo) = ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +\
                    (_Packet)->Private.NdisPacketOobOffset))->SizeMediaSpecificInfo;\
    }                                                                   \
}

#define NDIS_SET_PACKET_HEADER_SIZE(_Packet, _HdrSize)                  \
                        ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +    \
                        (_Packet)->Private.NdisPacketOobOffset))->HeaderSize = (_HdrSize)
#define NDIS_SET_PACKET_STATUS(_Packet, _Status)                        \
                        ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +    \
                        (_Packet)->Private.NdisPacketOobOffset))->Status = (_Status)
#define NDIS_SET_PACKET_TIME_TO_SEND(_Packet, _TimeToSend)              \
                        ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +    \
                        (_Packet)->Private.NdisPacketOobOffset))->TimeToSend = (_TimeToSend)
#define NDIS_SET_PACKET_TIME_SENT(_Packet, _TimeSent)                   \
                        ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +    \
                        (_Packet)->Private.NdisPacketOobOffset))->TimeSent = (_TimeSent)
#define NDIS_SET_PACKET_TIME_RECEIVED(_Packet, _TimeReceived)           \
                        ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +    \
                        (_Packet)->Private.NdisPacketOobOffset))->TimeReceived = (_TimeReceived)
#define NDIS_SET_PACKET_MEDIA_SPECIFIC_INFO(_Packet,                    \
                                            _MediaSpecificInfo,         \
                                            _SizeMediaSpecificInfo)     \
{                                                                       \
    if ((_Packet)->Private.NdisPacketFlags & fPACKET_ALLOCATED_BY_NDIS) \
    {                                                                   \
        (_Packet)->Private.NdisPacketFlags |= fPACKET_CONTAINS_MEDIA_SPECIFIC_INFO;\
        ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +                    \
                                          (_Packet)->Private.NdisPacketOobOffset))->MediaSpecificInformation = (_MediaSpecificInfo);\
        ((PNDIS_PACKET_OOB_DATA)((PUCHAR)(_Packet) +                    \
                                          (_Packet)->Private.NdisPacketOobOffset))->SizeMediaSpecificInfo = (_SizeMediaSpecificInfo);\
    }                                                                   \
}

//
// packet definition
//
typedef struct _NDIS_PACKET
{
    NDIS_PACKET_PRIVATE Private;

    union
    {
        struct                  // For Connection-less miniports
        {
            UCHAR   MiniportReserved[2*sizeof(PVOID)];
            UCHAR   WrapperReserved[2*sizeof(PVOID)];
        };

        struct
        {
            //
            // For de-serialized miniports. And by implication conn-oriented miniports.
            //
            UCHAR   MiniportReservedEx[3*sizeof(PVOID)];
            UCHAR   WrapperReservedEx[sizeof(PVOID)];
        };

        struct
        {
            UCHAR   MacReserved[4*sizeof(PVOID)];
        };
    };

    ULONG_PTR       Reserved[2];            // For compatibility with Win95
    UCHAR           ProtocolReserved[1];

} NDIS_PACKET, *PNDIS_PACKET, **PPNDIS_PACKET;

#endif // NDIS_LEGACY_DRIVER

//
//  NDIS per-packet information.
//
typedef enum _NDIS_PER_PACKET_INFO
{
    TcpIpChecksumPacketInfo,
    IpSecPacketInfo,
    TcpLargeSendPacketInfo,
    ClassificationHandlePacketInfo,
    NdisReserved,
    ScatterGatherListPacketInfo,
    Ieee8021QInfo,
    OriginalPacketInfo,
    PacketCancelId,
    OriginalNetBufferList,
    CachedNetBufferList,
    ShortPacketPaddingInfo,
    MaxPerPacketInfo
} NDIS_PER_PACKET_INFO, *PNDIS_PER_PACKET_INFO;

#if NDIS_LEGACY_DRIVER
typedef struct _NDIS_PACKET_EXTENSION
{
    PVOID       NdisPacketInfo[MaxPerPacketInfo];
} NDIS_PACKET_EXTENSION, *PNDIS_PACKET_EXTENSION;

#define NDIS_PACKET_EXTENSION_FROM_PACKET(_P)       ((PNDIS_PACKET_EXTENSION)((PUCHAR)(_P) + (_P)->Private.NdisPacketOobOffset + sizeof(NDIS_PACKET_OOB_DATA)))
#define NDIS_PER_PACKET_INFO_FROM_PACKET(_P, _Id)   ((PNDIS_PACKET_EXTENSION)((PUCHAR)(_P) + (_P)->Private.NdisPacketOobOffset + sizeof(NDIS_PACKET_OOB_DATA)))->NdisPacketInfo[(_Id)]
#define NDIS_GET_ORIGINAL_PACKET(_P)                ((PNDIS_PACKET)NDIS_PER_PACKET_INFO_FROM_PACKET(_P, OriginalPacketInfo))
#define NDIS_SET_ORIGINAL_PACKET(_P, _OP)           NDIS_PER_PACKET_INFO_FROM_PACKET(_P, OriginalPacketInfo) = _OP
#define NDIS_GET_PACKET_CANCEL_ID(_P)               NDIS_PER_PACKET_INFO_FROM_PACKET(_P, PacketCancelId)
#define NDIS_SET_PACKET_CANCEL_ID(_P, _cId)         NDIS_PER_PACKET_INFO_FROM_PACKET(_P, PacketCancelId) = _cId


//
// Ndis 5.1 entry points for setting/getting packet's CancelId and cancelling send packets
//

/*
EXPORT
VOID
NdisSetPacketCancelId(
    IN  PNDIS_PACKET    Packet,
    IN  PVOID           CancelId
    );
*/
#define  NdisSetPacketCancelId(_Packet, _CancelId) NDIS_SET_PACKET_CANCEL_ID(_Packet, _CancelId)

/*
EXPORT
PVOID
NdisGetPacketCancelId(
    IN  PNDIS_PACKET    Packet
    );
*/
#define  NdisGetPacketCancelId(_Packet) NDIS_GET_PACKET_CANCEL_ID(_Packet)


typedef struct _NDIS_PACKET_STACK
{
    ULONG_PTR   IMReserved[2];
    ULONG_PTR   NdisReserved[4];
} NDIS_PACKET_STACK, *PNDIS_PACKET_STACK;


#endif // NDIS_LEGACY_DRIVER

//
//  Per-packet information for TcpIpChecksumPacketInfo.
//
typedef struct _NDIS_TCP_IP_CHECKSUM_PACKET_INFO
{
    union
    {
        struct
        {
            ULONG   NdisPacketChecksumV4:1;
            ULONG   NdisPacketChecksumV6:1;
            ULONG   NdisPacketTcpChecksum:1;
            ULONG   NdisPacketUdpChecksum:1;
            ULONG   NdisPacketIpChecksum:1;
        } Transmit;

        struct
        {
            ULONG   NdisPacketTcpChecksumFailed:1;
            ULONG   NdisPacketUdpChecksumFailed:1;
            ULONG   NdisPacketIpChecksumFailed:1;
            ULONG   NdisPacketTcpChecksumSucceeded:1;
            ULONG   NdisPacketUdpChecksumSucceeded:1;
            ULONG   NdisPacketIpChecksumSucceeded:1;
            ULONG   NdisPacketLoopback:1;
        } Receive;

        ULONG   Value;
    };
} NDIS_TCP_IP_CHECKSUM_PACKET_INFO, *PNDIS_TCP_IP_CHECKSUM_PACKET_INFO;


//
//  Per-packet information for Ieee8021QInfo.
//
typedef struct _NDIS_PACKET_8021Q_INFO
{
    union
    {
        struct
        {
            UINT32      UserPriority:3;         // 802.1p priority
            UINT32      CanonicalFormatId:1;    // always 0
            UINT32      VlanId:12;              // VLAN Identification
            UINT32      Reserved:16;            // set to 0
        }   TagHeader;

        PVOID  Value;
    };
} NDIS_PACKET_8021Q_INFO, *PNDIS_PACKET_8021Q_INFO;


#if NDIS_LEGACY_DRIVER
//
//  Old definitions, to be obsoleted.
//
#define Ieee8021pPriority   Ieee8021QInfo
typedef UINT                IEEE8021PPRIORITY;

//
// WAN Packet. This is used by WAN miniports only. This is the legacy model.
// Co-Ndis is the preferred model for WAN miniports
//
typedef struct _NDIS_WAN_PACKET
{
    LIST_ENTRY          WanPacketQueue;
    PUCHAR              CurrentBuffer;
    ULONG               CurrentLength;
    PUCHAR              StartBuffer;
    PUCHAR              EndBuffer;
    PVOID               ProtocolReserved1;
    PVOID               ProtocolReserved2;
    PVOID               ProtocolReserved3;
    PVOID               ProtocolReserved4;
    PVOID               MacReserved1;
    PVOID               MacReserved2;
    PVOID               MacReserved3;
    PVOID               MacReserved4;
} NDIS_WAN_PACKET, *PNDIS_WAN_PACKET;

//
// Routines to get/set packet flags
//

/*++

UINT
NdisGetPacketFlags(
    IN  PNDIS_PACKET    Packet
    );

--*/

#define NdisGetPacketFlags(_Packet)                 ((_Packet)->Private.Flags)

#define NDIS_PACKET_FIRST_NDIS_BUFFER(_Packet)      ((_Packet)->Private.Head)
#define NDIS_PACKET_LAST_NDIS_BUFFER(_Packet)       ((_Packet)->Private.Tail)
#define NDIS_PACKET_VALID_COUNTS(_Packet)           ((_Packet)->Private.ValidCounts)


/*++

VOID
NdisSetPacketFlags(
    IN  PNDIS_PACKET Packet,
    IN  UINT Flags
    );

--*/

#define NdisSetPacketFlags(_Packet, _Flags)     (_Packet)->Private.Flags |= (_Flags)
#define NdisClearPacketFlags(_Packet, _Flags)   (_Packet)->Private.Flags &= ~(_Flags)

#endif // NDIS_LEGACY_DRIVER

#if NDIS_LEGACY_DRIVER

//
// Structure of requests sent via NdisRequest
//

typedef struct _NDIS_REQUEST
{
    UCHAR               MacReserved[4*sizeof(PVOID)];
    NDIS_REQUEST_TYPE   RequestType;
    union _DATA
    {
        struct _QUERY_INFORMATION
        {
            NDIS_OID    Oid;
            PVOID       InformationBuffer;
            UINT        InformationBufferLength;
            UINT        BytesWritten;
            UINT        BytesNeeded;
        } QUERY_INFORMATION;

        struct _SET_INFORMATION
        {
            NDIS_OID    Oid;
            PVOID       InformationBuffer;
            UINT        InformationBufferLength;
            UINT        BytesRead;
            UINT        BytesNeeded;
        } SET_INFORMATION;

    } DATA;
#if (defined(NDIS50) || defined(NDIS51) || defined(NDIS50_MINIPORT) || defined(NDIS51_MINIPORT))
    UCHAR               NdisReserved[9*sizeof(PVOID)];
    union
    {
        UCHAR           CallMgrReserved[2*sizeof(PVOID)];
        UCHAR           ProtocolReserved[2*sizeof(PVOID)];
    };
    UCHAR               MiniportReserved[2*sizeof(PVOID)];
#endif
} NDIS_REQUEST, *PNDIS_REQUEST;

#endif

//
// NDIS Address Family definitions.
//
typedef ULONG           NDIS_AF, *PNDIS_AF;
#define CO_ADDRESS_FAMILY_Q2931             ((NDIS_AF)0x1)  // ATM
#define CO_ADDRESS_FAMILY_PSCHED            ((NDIS_AF)0x2)  // Packet scheduler
#define CO_ADDRESS_FAMILY_L2TP              ((NDIS_AF)0x3)
#define CO_ADDRESS_FAMILY_IRDA              ((NDIS_AF)0x4)
#define CO_ADDRESS_FAMILY_1394              ((NDIS_AF)0x5)
#define CO_ADDRESS_FAMILY_PPP               ((NDIS_AF)0x6)
#define CO_ADDRESS_FAMILY_INFINIBAND        ((NDIS_AF)0x7)
#define CO_ADDRESS_FAMILY_TAPI              ((NDIS_AF)0x800)
#define CO_ADDRESS_FAMILY_TAPI_PROXY        ((NDIS_AF)0x801)

//
// The following is OR'ed with the base AF to denote proxy support
//
#define CO_ADDRESS_FAMILY_PROXY             0x80000000


//
//  Address family structure registered/opened via
//      NdisCmRegisterAddressFamily
//      NdisClOpenAddressFamily
//
typedef struct
{
    NDIS_AF                     AddressFamily;  // one of the CO_ADDRESS_FAMILY_xxx values above
    ULONG                       MajorVersion;   // the major version of call manager
    ULONG                       MinorVersion;   // the minor version of call manager
} CO_ADDRESS_FAMILY, *PCO_ADDRESS_FAMILY;

//
// Definition for a SAP
//
typedef struct
{
    ULONG                       SapType;
    ULONG                       SapLength;
    UCHAR                       Sap[1];
} CO_SAP, *PCO_SAP;

//
// Definitions for physical address.
//

typedef PHYSICAL_ADDRESS NDIS_PHYSICAL_ADDRESS, *PNDIS_PHYSICAL_ADDRESS;
typedef struct _NDIS_PHYSICAL_ADDRESS_UNIT
{
    NDIS_PHYSICAL_ADDRESS       PhysicalAddress;
    UINT                        Length;
} NDIS_PHYSICAL_ADDRESS_UNIT, *PNDIS_PHYSICAL_ADDRESS_UNIT;


/*++

ULONG
NdisGetPhysicalAddressHigh(
    IN  NDIS_PHYSICAL_ADDRESS   PhysicalAddress
    );

--*/

#define NdisGetPhysicalAddressHigh(_PhysicalAddress)            \
        ((_PhysicalAddress).HighPart)

/*++

VOID
NdisSetPhysicalAddressHigh(
    IN  NDIS_PHYSICAL_ADDRESS   PhysicalAddress,
    IN  ULONG                   Value
    );

--*/

#define NdisSetPhysicalAddressHigh(_PhysicalAddress, _Value)    \
     ((_PhysicalAddress).HighPart) = (_Value)


/*++

ULONG
NdisGetPhysicalAddressLow(
    IN  NDIS_PHYSICAL_ADDRESS PhysicalAddress
    );

--*/

#define NdisGetPhysicalAddressLow(_PhysicalAddress)             \
    ((_PhysicalAddress).LowPart)


/*++

VOID
NdisSetPhysicalAddressLow(
    IN  NDIS_PHYSICAL_ADDRESS   PhysicalAddress,
    IN  ULONG                   Value
    );

--*/

#define NdisSetPhysicalAddressLow(_PhysicalAddress, _Value)     \
    ((_PhysicalAddress).LowPart) = (_Value)

//
// Macro to initialize an NDIS_PHYSICAL_ADDRESS constant
//

#define NDIS_PHYSICAL_ADDRESS_CONST(_Low, _High)                \
    { (ULONG)(_Low), (LONG)(_High) }

//
// block used for references...
//
typedef struct _REFERENCE
{
    KSPIN_LOCK                  SpinLock;
    USHORT                      ReferenceCount;
    BOOLEAN                     Closing;
} REFERENCE, * PREFERENCE;

//
// Types of Memory (not mutually exclusive)
//

#define NDIS_MEMORY_CONTIGUOUS      0x00000001
#define NDIS_MEMORY_NONCACHED       0x00000002

//
// Open options
//

//
// This flag has been deprecated
//
#pragma deprecated(NDIS_OPEN_RECEIVE_NOT_REENTRANT)
#define NDIS_OPEN_RECEIVE_NOT_REENTRANT         0x00000001

//
// NDIS_STATUS values used in status indication
//

#define NDIS_STATUS_ONLINE                      ((NDIS_STATUS)0x40010003L)
#define NDIS_STATUS_RESET_START                 ((NDIS_STATUS)0x40010004L)
#define NDIS_STATUS_RESET_END                   ((NDIS_STATUS)0x40010005L)
#define NDIS_STATUS_RING_STATUS                 ((NDIS_STATUS)0x40010006L)
#define NDIS_STATUS_CLOSED                      ((NDIS_STATUS)0x40010007L)
#define NDIS_STATUS_WAN_LINE_UP                 ((NDIS_STATUS)0x40010008L)
#define NDIS_STATUS_WAN_LINE_DOWN               ((NDIS_STATUS)0x40010009L)
#define NDIS_STATUS_WAN_FRAGMENT                ((NDIS_STATUS)0x4001000AL)
#define NDIS_STATUS_MEDIA_CONNECT               ((NDIS_STATUS)0x4001000BL)
#define NDIS_STATUS_MEDIA_DISCONNECT            ((NDIS_STATUS)0x4001000CL)
#define NDIS_STATUS_HARDWARE_LINE_UP            ((NDIS_STATUS)0x4001000DL)
#define NDIS_STATUS_HARDWARE_LINE_DOWN          ((NDIS_STATUS)0x4001000EL)
#define NDIS_STATUS_INTERFACE_UP                ((NDIS_STATUS)0x4001000FL)
#define NDIS_STATUS_INTERFACE_DOWN              ((NDIS_STATUS)0x40010010L)
#define NDIS_STATUS_MEDIA_BUSY                  ((NDIS_STATUS)0x40010011L)
#define NDIS_STATUS_MEDIA_SPECIFIC_INDICATION   ((NDIS_STATUS)0x40010012L)
#define NDIS_STATUS_WW_INDICATION               NDIS_STATUS_MEDIA_SPECIFIC_INDICATION
#define NDIS_STATUS_LINK_SPEED_CHANGE           ((NDIS_STATUS)0x40010013L)
#define NDIS_STATUS_WAN_GET_STATS               ((NDIS_STATUS)0x40010014L)
#define NDIS_STATUS_WAN_CO_FRAGMENT             ((NDIS_STATUS)0x40010015L)
#define NDIS_STATUS_WAN_CO_LINKPARAMS           ((NDIS_STATUS)0x40010016L)
#define NDIS_STATUS_WAN_CO_MTULINKPARAMS        ((NDIS_STATUS)0x40010025L)
//
// new status indication codes used by NDIS 6 drivers
//
#if NDIS_SUPPORT_NDIS6
#define NDIS_STATUS_LINK_STATE                  ((NDIS_STATUS)0x40010017L)
#define NDIS_STATUS_NETWORK_CHANGE              ((NDIS_STATUS)0x40010018L)
#define NDIS_STATUS_MEDIA_SPECIFIC_INDICATION_EX ((NDIS_STATUS)0x40010019L)
#define NDIS_STATUS_PORT_STATE                  ((NDIS_STATUS)0x40010022L)
#define NDIS_STATUS_OPER_STATUS                 ((NDIS_STATUS)0x40010023L)
#define NDIS_STATUS_PACKET_FILTER               ((NDIS_STATUS)0x40010024L)
// Note that 0x40010025L is reserved for NDIS_STATUS_WAN_CO_MTULINKPARAMS

#define NDIS_STATUS_IP_OPER_STATUS              ((NDIS_STATUS)0x40010026L)

//
// offload specific status indication codes
//
#define NDIS_STATUS_OFFLOAD_PAUSE               ((NDIS_STATUS)0x40020001L)
#define NDIS_STATUS_UPLOAD_ALL                  ((NDIS_STATUS)0x40020002L)
#define NDIS_STATUS_OFFLOAD_RESUME              ((NDIS_STATUS)0x40020003L)
#define NDIS_STATUS_OFFLOAD_PARTIAL_SUCCESS     ((NDIS_STATUS)0x40020004L)
#define NDIS_STATUS_OFFLOAD_STATE_INVALID       ((NDIS_STATUS)0x40020005L)
#define NDIS_STATUS_TASK_OFFLOAD_CURRENT_CONFIG ((NDIS_STATUS)0x40020006L)
#define NDIS_STATUS_TASK_OFFLOAD_HARDWARE_CAPABILITIES ((NDIS_STATUS)0x40020007L)
#define NDIS_STATUS_OFFLOAD_ENCASPULATION_CHANGE ((NDIS_STATUS)0x40020008L)
#define NDIS_STATUS_TCP_CONNECTION_OFFLOAD_HARDWARE_CAPABILITIES ((NDIS_STATUS)0x4002000BL)
#if (NDIS_SUPPORT_NDIS61)
#define NDIS_STATUS_HD_SPLIT_CURRENT_CONFIG     ((NDIS_STATUS)0x4002000CL)
#endif // (NDIS_SUPPORT_NDIS61)

#if (NDIS_SUPPORT_NDIS620)
#define NDIS_STATUS_RECEIVE_QUEUE_STATE         ((NDIS_STATUS)0x4002000DL)
#endif

#if (NDIS_SUPPORT_NDIS630)
#define NDIS_STATUS_VF_CONFIG_STATE             ((NDIS_STATUS)0x4002000EL)
#define NDIS_STATUS_RECEIVE_FILTER_CURRENT_CAPABILITIES     ((NDIS_STATUS)0x40020010L)
#define NDIS_STATUS_RECEIVE_FILTER_HARDWARE_CAPABILITIES    ((NDIS_STATUS)0x40020020L)
#define NDIS_STATUS_RECEIVE_FILTER_QUEUE_PARAMETERS         ((NDIS_STATUS)0x40020030L)
#define NDIS_STATUS_NIC_SWITCH_CURRENT_CAPABILITIES         ((NDIS_STATUS)0x40020040L)
#define NDIS_STATUS_NIC_SWITCH_HARDWARE_CAPABILITIES        ((NDIS_STATUS)0x40020050L)
#define NDIS_STATUS_RECEIVE_FILTER_QUEUE_STATE_CHANGE     ((NDIS_STATUS)0x40020070L)
#define NDIS_STATUS_QOS_OPERATIONAL_PARAMETERS_CHANGE       ((NDIS_STATUS)0x400200A0L)
#define NDIS_STATUS_QOS_REMOTE_PARAMETERS_CHANGE            ((NDIS_STATUS)0x400200A1L)
#endif // (NDIS_SUPPORT_NDIS630)

#if (NDIS_SUPPORT_NDIS640)
#define NDIS_STATUS_ISOLATION_PARAMETERS_CHANGE             ((NDIS_STATUS)0x40020080L)
#endif // (NDIS_SUPPORT_NDIS640)

#define NDIS_STATUS_OFFLOAD_IM_RESERVED1        ((NDIS_STATUS)0x40020100L)
#define NDIS_STATUS_OFFLOAD_IM_RESERVED2        ((NDIS_STATUS)0x40020101L)
#define NDIS_STATUS_OFFLOAD_IM_RESERVED3        ((NDIS_STATUS)0x40020102L)

//
// 802.11 specific status indication codes
//
#define NDIS_STATUS_DOT11_SCAN_CONFIRM                  ((NDIS_STATUS)0x40030000L)
#define NDIS_STATUS_DOT11_MPDU_MAX_LENGTH_CHANGED       ((NDIS_STATUS)0x40030001L)
#define NDIS_STATUS_DOT11_ASSOCIATION_START             ((NDIS_STATUS)0x40030002L)
#define NDIS_STATUS_DOT11_ASSOCIATION_COMPLETION        ((NDIS_STATUS)0x40030003L)
#define NDIS_STATUS_DOT11_CONNECTION_START              ((NDIS_STATUS)0x40030004L)
#define NDIS_STATUS_DOT11_CONNECTION_COMPLETION         ((NDIS_STATUS)0x40030005L)
#define NDIS_STATUS_DOT11_ROAMING_START                 ((NDIS_STATUS)0x40030006L)
#define NDIS_STATUS_DOT11_ROAMING_COMPLETION            ((NDIS_STATUS)0x40030007L)
#define NDIS_STATUS_DOT11_DISASSOCIATION                ((NDIS_STATUS)0x40030008L)
#define NDIS_STATUS_DOT11_TKIPMIC_FAILURE               ((NDIS_STATUS)0x40030009L)
#define NDIS_STATUS_DOT11_PMKID_CANDIDATE_LIST          ((NDIS_STATUS)0x4003000AL)
#define NDIS_STATUS_DOT11_PHY_STATE_CHANGED             ((NDIS_STATUS)0x4003000BL)
#define NDIS_STATUS_DOT11_LINK_QUALITY                  ((NDIS_STATUS)0x4003000CL)
#define NDIS_STATUS_DOT11_INCOMING_ASSOC_STARTED            ((NDIS_STATUS)0x4003000DL)
#define NDIS_STATUS_DOT11_INCOMING_ASSOC_REQUEST_RECEIVED   ((NDIS_STATUS)0x4003000EL)
#define NDIS_STATUS_DOT11_INCOMING_ASSOC_COMPLETION         ((NDIS_STATUS)0x4003000FL)
#define NDIS_STATUS_DOT11_STOP_AP                           ((NDIS_STATUS)0x40030010L)
#define NDIS_STATUS_DOT11_PHY_FREQUENCY_ADOPTED             ((NDIS_STATUS)0x40030011L)
#define NDIS_STATUS_DOT11_CAN_SUSTAIN_AP                    ((NDIS_STATUS)0x40030012L)
#define NDIS_STATUS_DOT11_WFD_DISCOVER_COMPLETE             ((NDIS_STATUS)0x40030013L)
#define NDIS_STATUS_DOT11_WFD_GO_NEGOTIATION_REQUEST_SEND_COMPLETE      ((NDIS_STATUS)0x40030014L)
#define NDIS_STATUS_DOT11_WFD_RECEIVED_GO_NEGOTIATION_REQUEST           ((NDIS_STATUS)0x40030015L)
#define NDIS_STATUS_DOT11_WFD_GO_NEGOTIATION_RESPONSE_SEND_COMPLETE     ((NDIS_STATUS)0x40030016L)
#define NDIS_STATUS_DOT11_WFD_RECEIVED_GO_NEGOTIATION_RESPONSE          ((NDIS_STATUS)0x40030017L)
#define NDIS_STATUS_DOT11_WFD_GO_NEGOTIATION_CONFIRMATION_SEND_COMPLETE ((NDIS_STATUS)0x40030018L)
#define NDIS_STATUS_DOT11_WFD_RECEIVED_GO_NEGOTIATION_CONFIRMATION      ((NDIS_STATUS)0x40030019L)
#define NDIS_STATUS_DOT11_WFD_INVITATION_REQUEST_SEND_COMPLETE          ((NDIS_STATUS)0x4003001AL)
#define NDIS_STATUS_DOT11_WFD_RECEIVED_INVITATION_REQUEST               ((NDIS_STATUS)0x4003001BL)
#define NDIS_STATUS_DOT11_WFD_INVITATION_RESPONSE_SEND_COMPLETE         ((NDIS_STATUS)0x4003001CL)
#define NDIS_STATUS_DOT11_WFD_RECEIVED_INVITATION_RESPONSE              ((NDIS_STATUS)0x4003001DL)
#define NDIS_STATUS_DOT11_WFD_PROVISION_DISCOVERY_REQUEST_SEND_COMPLETE ((NDIS_STATUS)0x4003001EL)
#define NDIS_STATUS_DOT11_WFD_RECEIVED_PROVISION_DISCOVERY_REQUEST      ((NDIS_STATUS)0x4003001FL)
#define NDIS_STATUS_DOT11_WFD_PROVISION_DISCOVERY_RESPONSE_SEND_COMPLETE    ((NDIS_STATUS)0x40030020L)
#define NDIS_STATUS_DOT11_WFD_RECEIVED_PROVISION_DISCOVERY_RESPONSE     ((NDIS_STATUS)0x40030021L)
#define NDIS_STATUS_DOT11_WFD_GROUP_OPERATING_CHANNEL                   ((NDIS_STATUS)0x40030022L)
#define NDIS_STATUS_DOT11_OFFLOAD_NETWORK_STATUS_CHANGED                ((NDIS_STATUS)0x40030023L)
#define NDIS_STATUS_DOT11_MANUFACTURING_CALLBACK                        ((NDIS_STATUS)0x40030024L)

//
// Add WWAN specific status indication codes
//
#define NDIS_STATUS_WWAN_DEVICE_CAPS            ((NDIS_STATUS)0x40041000)
#define NDIS_STATUS_WWAN_READY_INFO             ((NDIS_STATUS)0x40041001)
#define NDIS_STATUS_WWAN_RADIO_STATE            ((NDIS_STATUS)0x40041002)
#define NDIS_STATUS_WWAN_PIN_INFO               ((NDIS_STATUS)0x40041003)
#define NDIS_STATUS_WWAN_PIN_LIST               ((NDIS_STATUS)0x40041004)
#define NDIS_STATUS_WWAN_HOME_PROVIDER          ((NDIS_STATUS)0x40041005)
#define NDIS_STATUS_WWAN_PREFERRED_PROVIDERS    ((NDIS_STATUS)0x40041006)
#define NDIS_STATUS_WWAN_VISIBLE_PROVIDERS      ((NDIS_STATUS)0x40041007)
#define NDIS_STATUS_WWAN_REGISTER_STATE         ((NDIS_STATUS)0x40041008)
#define NDIS_STATUS_WWAN_PACKET_SERVICE         ((NDIS_STATUS)0x40041009)
#define NDIS_STATUS_WWAN_SIGNAL_STATE           ((NDIS_STATUS)0x4004100a)
#define NDIS_STATUS_WWAN_CONTEXT_STATE          ((NDIS_STATUS)0x4004100b)
#define NDIS_STATUS_WWAN_PROVISIONED_CONTEXTS   ((NDIS_STATUS)0x4004100c)
#define NDIS_STATUS_WWAN_SERVICE_ACTIVATION     ((NDIS_STATUS)0x4004100d)
#define NDIS_STATUS_WWAN_SMS_CONFIGURATION      ((NDIS_STATUS)0x4004100e)
#define NDIS_STATUS_WWAN_SMS_RECEIVE            ((NDIS_STATUS)0x4004100f)
#define NDIS_STATUS_WWAN_SMS_SEND               ((NDIS_STATUS)0x40041010)
#define NDIS_STATUS_WWAN_SMS_DELETE             ((NDIS_STATUS)0x40041011)
#define NDIS_STATUS_WWAN_SMS_STATUS             ((NDIS_STATUS)0x40041012)
#define NDIS_STATUS_WWAN_DNS_ADDRESS            ((NDIS_STATUS)0x40041013)

#define NDIS_STATUS_WWAN_VENDOR_SPECIFIC	((NDIS_STATUS)0x40043000)

#if (NDIS_SUPPORT_NDIS630)
#define NDIS_STATUS_WWAN_SET_HOME_PROVIDER_COMPLETE         ((NDIS_STATUS)0x40041014L)
#define NDIS_STATUS_WWAN_AUTH_RESPONSE                      ((NDIS_STATUS)0x40041015L)
#define NDIS_STATUS_WWAN_SUPPORTED_DEVICE_SERVICES          ((NDIS_STATUS)0x40041016L)
#define NDIS_STATUS_WWAN_DEVICE_SERVICE_SUBSCRIPTION        ((NDIS_STATUS)0x40041017L)
#define NDIS_STATUS_WWAN_DEVICE_SERVICE_RESPONSE            ((NDIS_STATUS)0x40041018L)
#define NDIS_STATUS_WWAN_DEVICE_SERVICE_EVENT               ((NDIS_STATUS)0x40041019L)
#define NDIS_STATUS_WWAN_USSD                               ((NDIS_STATUS)0x40041020L)
#define NDIS_STATUS_WWAN_DEVICE_SERVICE_SUPPORTED_COMMANDS  ((NDIS_STATUS)0x40041021L)
#define NDIS_STATUS_WWAN_DEVICE_SERVICE_SESSION             ((NDIS_STATUS)0x40041022L)
#define NDIS_STATUS_WWAN_DEVICE_SERVICE_SESSION_WRITE_COMPLETE  ((NDIS_STATUS)0x40041023L)
#define NDIS_STATUS_WWAN_DEVICE_SERVICE_SESSION_READ        ((NDIS_STATUS)0x40041024L)
#define NDIS_STATUS_WWAN_PREFERRED_MULTICARRIER_PROVIDERS   ((NDIS_STATUS)0x40041025L)
#define NDIS_STATUS_WWAN_RESERVED_1                         ((NDIS_STATUS)0x40041026L)
#define NDIS_STATUS_WWAN_RESERVED_2                         ((NDIS_STATUS)0x40041027L)
#define NDIS_STATUS_WWAN_IP_ADDRESS_STATE                   ((NDIS_STATUS)0x40041028L)
#define NDIS_STATUS_WWAN_RESERVED_3             	    ((NDIS_STATUS)0x40041029L)
#endif

//
// End of WWAN specific status indication codes
//

//
// Add WiMAX specific status indication codes
//


#endif NDIS_SUPPORT_NDIS6

//
// Status codes for NDIS 6.20 Power Management
//
#if (NDIS_SUPPORT_NDIS620)
#define NDIS_STATUS_PM_WOL_PATTERN_REJECTED     ((NDIS_STATUS)0x40030051L)
#define NDIS_STATUS_PM_OFFLOAD_REJECTED         ((NDIS_STATUS)0x40030052L)
#define NDIS_STATUS_PM_CAPABILITIES_CHANGE      ((NDIS_STATUS)0x40030053L)
#endif

#if (NDIS_SUPPORT_NDIS630)
#define NDIS_STATUS_PM_HARDWARE_CAPABILITIES    ((NDIS_STATUS)0x40030054L)
#define NDIS_STATUS_PM_WAKE_REASON              ((NDIS_STATUS)0x40030055L)
#endif

//
// status codes returned by drivers
//
#define NDIS_STATUS_SUCCESS                     ((NDIS_STATUS)STATUS_SUCCESS)
#define NDIS_STATUS_PENDING                     ((NDIS_STATUS)STATUS_PENDING)
#define NDIS_STATUS_NOT_RECOGNIZED              ((NDIS_STATUS)0x00010001L)
#define NDIS_STATUS_NOT_COPIED                  ((NDIS_STATUS)0x00010002L)
#define NDIS_STATUS_NOT_ACCEPTED                ((NDIS_STATUS)0x00010003L)
#define NDIS_STATUS_CALL_ACTIVE                 ((NDIS_STATUS)0x00010007L)
#define NDIS_STATUS_INDICATION_REQUIRED         ((NDIS_STATUS)STATUS_NDIS_INDICATION_REQUIRED)
#define NDIS_STATUS_NOT_RESETTABLE              ((NDIS_STATUS)0x80010001L)
#define NDIS_STATUS_SOFT_ERRORS                 ((NDIS_STATUS)0x80010003L)
#define NDIS_STATUS_HARD_ERRORS                 ((NDIS_STATUS)0x80010004L)
#define NDIS_STATUS_BUFFER_OVERFLOW             ((NDIS_STATUS)STATUS_BUFFER_OVERFLOW)
#define NDIS_STATUS_FAILURE                     ((NDIS_STATUS)STATUS_UNSUCCESSFUL)
#define NDIS_STATUS_RESOURCES                   ((NDIS_STATUS)STATUS_INSUFFICIENT_RESOURCES)
#define NDIS_STATUS_CLOSING                     ((NDIS_STATUS)0xC0010002L)
#define NDIS_STATUS_BAD_VERSION                 ((NDIS_STATUS)0xC0010004L)
#define NDIS_STATUS_BAD_CHARACTERISTICS         ((NDIS_STATUS)0xC0010005L)
#define NDIS_STATUS_ADAPTER_NOT_FOUND           ((NDIS_STATUS)0xC0010006L)
#define NDIS_STATUS_OPEN_FAILED                 ((NDIS_STATUS)0xC0010007L)
#define NDIS_STATUS_DEVICE_FAILED               ((NDIS_STATUS)0xC0010008L)
#define NDIS_STATUS_MULTICAST_FULL              ((NDIS_STATUS)0xC0010009L)
#define NDIS_STATUS_MULTICAST_EXISTS            ((NDIS_STATUS)0xC001000AL)
#define NDIS_STATUS_MULTICAST_NOT_FOUND         ((NDIS_STATUS)0xC001000BL)
#define NDIS_STATUS_REQUEST_ABORTED             ((NDIS_STATUS)0xC001000CL)
#define NDIS_STATUS_RESET_IN_PROGRESS           ((NDIS_STATUS)0xC001000DL)
#define NDIS_STATUS_CLOSING_INDICATING          ((NDIS_STATUS)0xC001000EL)
#define NDIS_STATUS_NOT_SUPPORTED               ((NDIS_STATUS)STATUS_NOT_SUPPORTED)
#define NDIS_STATUS_INVALID_PACKET              ((NDIS_STATUS)0xC001000FL)
#define NDIS_STATUS_OPEN_LIST_FULL              ((NDIS_STATUS)0xC0010010L)
#define NDIS_STATUS_ADAPTER_NOT_READY           ((NDIS_STATUS)0xC0010011L)
#define NDIS_STATUS_ADAPTER_NOT_OPEN            ((NDIS_STATUS)0xC0010012L)
#define NDIS_STATUS_NOT_INDICATING              ((NDIS_STATUS)0xC0010013L)
#define NDIS_STATUS_INVALID_LENGTH              ((NDIS_STATUS)0xC0010014L)
#define NDIS_STATUS_INVALID_DATA                ((NDIS_STATUS)0xC0010015L)
#define NDIS_STATUS_BUFFER_TOO_SHORT            ((NDIS_STATUS)0xC0010016L)
#define NDIS_STATUS_INVALID_OID                 ((NDIS_STATUS)0xC0010017L)
#define NDIS_STATUS_ADAPTER_REMOVED             ((NDIS_STATUS)0xC0010018L)
#define NDIS_STATUS_UNSUPPORTED_MEDIA           ((NDIS_STATUS)0xC0010019L)
#define NDIS_STATUS_GROUP_ADDRESS_IN_USE        ((NDIS_STATUS)0xC001001AL)
#define NDIS_STATUS_FILE_NOT_FOUND              ((NDIS_STATUS)0xC001001BL)
#define NDIS_STATUS_ERROR_READING_FILE          ((NDIS_STATUS)0xC001001CL)
#define NDIS_STATUS_ALREADY_MAPPED              ((NDIS_STATUS)0xC001001DL)
#define NDIS_STATUS_RESOURCE_CONFLICT           ((NDIS_STATUS)0xC001001EL)
#define NDIS_STATUS_NO_CABLE                    ((NDIS_STATUS)0xC001001FL)
#define NDIS_STATUS_INVALID_DEVICE_REQUEST      ((NDIS_STATUS)STATUS_INVALID_DEVICE_REQUEST)
#define NDIS_STATUS_NETWORK_UNREACHABLE         ((NDIS_STATUS)STATUS_NETWORK_UNREACHABLE)
#define NDIS_STATUS_DATA_NOT_ACCEPTED           ((NDIS_STATUS)STATUS_DATA_NOT_ACCEPTED)

//
// CO-NDIS specific
//
#define NDIS_STATUS_INVALID_SAP                 ((NDIS_STATUS)0xC0010020L)
#define NDIS_STATUS_SAP_IN_USE                  ((NDIS_STATUS)0xC0010021L)
#define NDIS_STATUS_INVALID_ADDRESS             ((NDIS_STATUS)0xC0010022L)
#define NDIS_STATUS_VC_NOT_ACTIVATED            ((NDIS_STATUS)0xC0010023L)
#define NDIS_STATUS_DEST_OUT_OF_ORDER           ((NDIS_STATUS)0xC0010024L)  // cause 27
#define NDIS_STATUS_VC_NOT_AVAILABLE            ((NDIS_STATUS)0xC0010025L)  // cause 35,45
#define NDIS_STATUS_CELLRATE_NOT_AVAILABLE      ((NDIS_STATUS)0xC0010026L)  // cause 37
#define NDIS_STATUS_INCOMPATIBLE_QOS            ((NDIS_STATUS)0xC0010027L)  // cause 49
#define NDIS_STATUS_INCOMPATABLE_QOS            NDIS_STATUS_INCOMPATIBLE_QOS // legacy spelling error
#define NDIS_STATUS_AAL_PARAMS_UNSUPPORTED      ((NDIS_STATUS)0xC0010028L)  // cause 93
#define NDIS_STATUS_NO_ROUTE_TO_DESTINATION     ((NDIS_STATUS)0xC0010029L)  // cause 3

//
// 802.5 specific
//
#define NDIS_STATUS_TOKEN_RING_OPEN_ERROR       ((NDIS_STATUS)0xC0011000L)


//
// new status codes used in NDIS 6
//
#if NDIS_SUPPORT_NDIS6
#define NDIS_STATUS_SEND_ABORTED                ((NDIS_STATUS)STATUS_NDIS_REQUEST_ABORTED)
#define NDIS_STATUS_PAUSED                      ((NDIS_STATUS)STATUS_NDIS_PAUSED)
#define NDIS_STATUS_INTERFACE_NOT_FOUND         ((NDIS_STATUS)STATUS_NDIS_INTERFACE_NOT_FOUND)
#define NDIS_STATUS_INVALID_PARAMETER           ((NDIS_STATUS)STATUS_INVALID_PARAMETER)
#define NDIS_STATUS_UNSUPPORTED_REVISION        ((NDIS_STATUS)STATUS_NDIS_UNSUPPORTED_REVISION)
#define NDIS_STATUS_INVALID_PORT                ((NDIS_STATUS)STATUS_NDIS_INVALID_PORT)
#define NDIS_STATUS_INVALID_PORT_STATE          ((NDIS_STATUS)STATUS_NDIS_INVALID_PORT_STATE)
#define NDIS_STATUS_INVALID_STATE               ((NDIS_STATUS)STATUS_INVALID_DEVICE_STATE)
#define NDIS_STATUS_MEDIA_DISCONNECTED          ((NDIS_STATUS)STATUS_NDIS_MEDIA_DISCONNECTED)
#define NDIS_STATUS_LOW_POWER_STATE             ((NDIS_STATUS)STATUS_NDIS_LOW_POWER_STATE)

#define NDIS_STATUS_DOT11_AUTO_CONFIG_ENABLED   ((NDIS_STATUS)STATUS_NDIS_DOT11_AUTO_CONFIG_ENABLED)
#define NDIS_STATUS_DOT11_MEDIA_IN_USE          ((NDIS_STATUS)STATUS_NDIS_DOT11_MEDIA_IN_USE)
#define NDIS_STATUS_DOT11_POWER_STATE_INVALID   ((NDIS_STATUS)STATUS_NDIS_DOT11_POWER_STATE_INVALID)

//
// status codes used in NDIS 6.20
//
#if NDIS_SUPPORT_NDIS620
#define NDIS_STATUS_PM_WOL_PATTERN_LIST_FULL        ((NDIS_STATUS)STATUS_NDIS_PM_WOL_PATTERN_LIST_FULL)
#define NDIS_STATUS_PM_PROTOCOL_OFFLOAD_LIST_FULL   ((NDIS_STATUS)STATUS_NDIS_PM_PROTOCOL_OFFLOAD_LIST_FULL)
#endif

//
// status codes used in NDIS 6.30
//
#if NDIS_SUPPORT_NDIS630
#define NDIS_STATUS_BUSY                            ((NDIS_STATUS)STATUS_DEVICE_BUSY)
#define NDIS_STATUS_REINIT_REQUIRED             ((NDIS_STATUS)STATUS_NDIS_REINIT_REQUIRED)
#endif

//
// status codes for offload operations
//
#define NDIS_STATUS_UPLOAD_IN_PROGRESS                  ((NDIS_STATUS)0xC0231001L)
#define NDIS_STATUS_REQUEST_UPLOAD                      ((NDIS_STATUS)0xC0231002L)
#define NDIS_STATUS_UPLOAD_REQUESTED                    ((NDIS_STATUS)0xC0231003L)
#define NDIS_STATUS_OFFLOAD_TCP_ENTRIES                 ((NDIS_STATUS)0xC0231004L)
#define NDIS_STATUS_OFFLOAD_PATH_ENTRIES                ((NDIS_STATUS)0xC0231005L)
#define NDIS_STATUS_OFFLOAD_NEIGHBOR_ENTRIES            ((NDIS_STATUS)0xC0231006L)
#define NDIS_STATUS_OFFLOAD_IP_ADDRESS_ENTRIES          ((NDIS_STATUS)0xC0231007L)
#define NDIS_STATUS_OFFLOAD_HW_ADDRESS_ENTRIES          ((NDIS_STATUS)0xC0231008L)
#define NDIS_STATUS_OFFLOAD_VLAN_ENTRIES                ((NDIS_STATUS)0xC0231009L)
#define NDIS_STATUS_OFFLOAD_TCP_XMIT_BUFFER             ((NDIS_STATUS)0xC023100AL)
#define NDIS_STATUS_OFFLOAD_TCP_RCV_BUFFER              ((NDIS_STATUS)0xC023100BL)
#define NDIS_STATUS_OFFLOAD_TCP_RCV_WINDOW              ((NDIS_STATUS)0xC023100CL)
#define NDIS_STATUS_OFFLOAD_VLAN_MISMATCH               ((NDIS_STATUS)0xC023100DL)
#define NDIS_STATUS_OFFLOAD_DATA_NOT_ACCEPTED           ((NDIS_STATUS)0xC023100EL)
#define NDIS_STATUS_OFFLOAD_POLICY                      ((NDIS_STATUS)0xC023100FL)
#define NDIS_STATUS_OFFLOAD_DATA_PARTIALLY_ACCEPTED     ((NDIS_STATUS)0xC0231010L)
#define NDIS_STATUS_OFFLOAD_REQUEST_RESET               ((NDIS_STATUS)0xC0231011L)
#endif // NDIS_SUPPORT_NDIS6

//
// Status codes for NDIS 6.20 capable Chimney offload miniports.
//
#if (NDIS_SUPPORT_NDIS620)
#define NDIS_STATUS_OFFLOAD_CONNECTION_REJECTED ((NDIS_STATUS)STATUS_NDIS_OFFLOAD_CONNECTION_REJECTED)
#endif

//
// Status codes used for Hyper-V extensible switch.
//
#if (NDIS_SUPPORT_NDIS630)
#define NDIS_STATUS_SWITCH_PORT_REMOVE_VF               ((NDIS_STATUS)0xC0241001L)
#define NDIS_STATUS_SWITCH_NIC_STATUS                   ((NDIS_STATUS)0xC0241002L)
#endif  //(NDIS_SUPPORT_NDIS630)


//
// used in error logging
//

#define NDIS_ERROR_CODE ULONG

#define NDIS_ERROR_CODE_RESOURCE_CONFLICT           EVENT_NDIS_RESOURCE_CONFLICT
#define NDIS_ERROR_CODE_OUT_OF_RESOURCES            EVENT_NDIS_OUT_OF_RESOURCE
#define NDIS_ERROR_CODE_HARDWARE_FAILURE            EVENT_NDIS_HARDWARE_FAILURE
#define NDIS_ERROR_CODE_ADAPTER_NOT_FOUND           EVENT_NDIS_ADAPTER_NOT_FOUND
#define NDIS_ERROR_CODE_INTERRUPT_CONNECT           EVENT_NDIS_INTERRUPT_CONNECT
#define NDIS_ERROR_CODE_DRIVER_FAILURE              EVENT_NDIS_DRIVER_FAILURE
#define NDIS_ERROR_CODE_BAD_VERSION                 EVENT_NDIS_BAD_VERSION
#define NDIS_ERROR_CODE_TIMEOUT                     EVENT_NDIS_TIMEOUT
#define NDIS_ERROR_CODE_NETWORK_ADDRESS             EVENT_NDIS_NETWORK_ADDRESS
#define NDIS_ERROR_CODE_UNSUPPORTED_CONFIGURATION   EVENT_NDIS_UNSUPPORTED_CONFIGURATION
#define NDIS_ERROR_CODE_INVALID_VALUE_FROM_ADAPTER  EVENT_NDIS_INVALID_VALUE_FROM_ADAPTER
#define NDIS_ERROR_CODE_MISSING_CONFIGURATION_PARAMETER EVENT_NDIS_MISSING_CONFIGURATION_PARAMETER
#define NDIS_ERROR_CODE_BAD_IO_BASE_ADDRESS         EVENT_NDIS_BAD_IO_BASE_ADDRESS
#define NDIS_ERROR_CODE_RECEIVE_SPACE_SMALL         EVENT_NDIS_RECEIVE_SPACE_SMALL
#define NDIS_ERROR_CODE_ADAPTER_DISABLED            EVENT_NDIS_ADAPTER_DISABLED

_IRQL_requires_max_(HIGH_LEVEL)
VOID
__inline
NdisAllocateSpinLock(_Out_ PNDIS_SPIN_LOCK SpinLock)
{
    KeInitializeSpinLock(&SpinLock->SpinLock);
}

_IRQL_requires_max_(HIGH_LEVEL)
VOID
__inline
NdisFreeSpinLock(_In_ PNDIS_SPIN_LOCK SpinLock)
{
    // On the NT platform, there is nothing to do here
    UNREFERENCED_PARAMETER(SpinLock);
}

#define NdisAcquireSpinLock(_SpinLock)  KeAcquireSpinLock(&(_SpinLock)->SpinLock, &(_SpinLock)->OldIrql)

#define NdisReleaseSpinLock(_SpinLock)  KeReleaseSpinLock(&(_SpinLock)->SpinLock,(_SpinLock)->OldIrql)

#define NdisDprAcquireSpinLock(_SpinLock)                                   \
{                                                                           \
    KeAcquireSpinLockAtDpcLevel(&(_SpinLock)->SpinLock);                    \
}

#define NdisDprReleaseSpinLock(_SpinLock) KeReleaseSpinLockFromDpcLevel(&(_SpinLock)->SpinLock)

#define NdisGetCurrentSystemTime(_pSystemTime)                              \
    {                                                                       \
        KeQuerySystemTime(_pSystemTime);                                    \
    }

//
// Interlocked support functions
//

#define NdisInterlockedIncrement(Addend)    InterlockedIncrement(Addend)

#define NdisInterlockedDecrement(Addend)    InterlockedDecrement(Addend)

#define NdisInterlockedAddUlong(_Addend, _Increment, _SpinLock) \
    ExInterlockedAddUlong(_Addend, _Increment, &(_SpinLock)->SpinLock)

#define NdisInterlockedInsertHeadList(_ListHead, _ListEntry, _SpinLock) \
    ExInterlockedInsertHeadList(_ListHead, _ListEntry, &(_SpinLock)->SpinLock)

#define NdisInterlockedInsertTailList(_ListHead, _ListEntry, _SpinLock) \
    ExInterlockedInsertTailList(_ListHead, _ListEntry, &(_SpinLock)->SpinLock)

#define NdisInterlockedRemoveHeadList(_ListHead, _SpinLock) \
    ExInterlockedRemoveHeadList(_ListHead, &(_SpinLock)->SpinLock)

#define NdisInterlockedPushEntryList(ListHead, ListEntry, Lock) \
    ExInterlockedPushEntryList(ListHead, ListEntry, &(Lock)->SpinLock)

#define NdisInterlockedPopEntryList(ListHead, Lock) \
    ExInterlockedPopEntryList(ListHead, &(Lock)->SpinLock)



#if NDIS_SUPPORT_60_COMPATIBLE_API

typedef union _NDIS_RW_LOCK_REFCOUNT
{
    ULONG                       RefCount;
    UCHAR                       cacheLine[16];  // This is smaller than a cacheline on most CPUs now
} NDIS_RW_LOCK_REFCOUNT;

typedef struct _NDIS_RW_LOCK
{
    union
    {
        struct
        {
            KSPIN_LOCK          SpinLock;
            PVOID               Context;
        };
        UCHAR                   Reserved[16];
    };

    union
    {
        NDIS_RW_LOCK_REFCOUNT   RefCount[MAXIMUM_PROCESSORS];
        ULONG                   RefCountEx[sizeof(NDIS_RW_LOCK_REFCOUNT)/sizeof(ULONG)
                                           * MAXIMUM_PROCESSORS];
        struct
        {
            KSPIN_LOCK          RefCountLock;
            volatile ULONG      SharedRefCount;
            volatile BOOLEAN    WriterWaiting;
        };
    };
} NDIS_RW_LOCK, *PNDIS_RW_LOCK;

typedef struct _LOCK_STATE
{
    USHORT                      LockState;
    KIRQL                       OldIrql;
} LOCK_STATE, *PLOCK_STATE;

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisInitializeReadWriteLock(
    _Out_ PNDIS_RW_LOCK           Lock
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
_IRQL_raises_(DISPATCH_LEVEL)
EXPORT
VOID
NdisAcquireReadWriteLock(
    _Inout_ _Acquires_lock_(_Curr_)   PNDIS_RW_LOCK Lock,
    _In_                                                BOOLEAN       fWrite,// TRUE -> Write, FALSE -> Read
    _Out_ _IRQL_saves_
    _Requires_lock_not_held_(*_Curr_) _Acquires_lock_(*_Curr_)
                                                        PLOCK_STATE   LockState
    );


_IRQL_requires_(DISPATCH_LEVEL)
EXPORT
VOID
NdisReleaseReadWriteLock(
    _Inout_ _Releases_lock_(_Curr_)        PNDIS_RW_LOCK Lock,
    _In_ _IRQL_restores_ _Requires_lock_held_(*_Curr_) _Releases_lock_(*_Curr_)
                                                        PLOCK_STATE   LockState
    );

#if NDIS_SUPPORT_NDIS6

_IRQL_requires_(DISPATCH_LEVEL)
EXPORT
VOID
NdisDprAcquireReadWriteLock(
    _Inout_ _Acquires_lock_(_Curr_) PNDIS_RW_LOCK Lock,
    _In_ BOOLEAN fWrite,         // TRUE -> Write, FALSE -> Read
    _Out_ _Requires_lock_not_held_(*_Curr_) _Acquires_lock_(*_Curr_)
         PLOCK_STATE LockState
    );

_IRQL_requires_(DISPATCH_LEVEL)
EXPORT    
VOID
NdisDprReleaseReadWriteLock(
    _Inout_ _Releases_lock_(_Curr_) PNDIS_RW_LOCK Lock,
    _In_ _Requires_lock_held_(*_Curr_) _Releases_lock_(*_Curr_)
        PLOCK_STATE LockState
    );

#endif
#endif

#if NDIS_SUPPORT_NDIS620

struct _NDIS_RW_LOCK_EX;
typedef struct _NDIS_RW_LOCK_EX  NDIS_RW_LOCK_EX;
typedef struct _NDIS_RW_LOCK_EX* PNDIS_RW_LOCK_EX;

typedef struct _LOCK_STATE_EX
{
    KIRQL                       OldIrql;
    UCHAR                       LockState;
    UCHAR                       Flags;
} LOCK_STATE_EX, *PLOCK_STATE_EX;

#define NDIS_RWL_AT_DISPATCH_LEVEL      1


__drv_allocatesMem(ndisrwlockex)
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
PNDIS_RW_LOCK_EX
NdisAllocateRWLock(
    _In_ NDIS_HANDLE NdisHandle
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFreeRWLock(
    _In_ __drv_freesMem(ndisrwlockex) PNDIS_RW_LOCK_EX Lock
    );

_Acquires_shared_lock_(Lock)
_IRQL_raises_(DISPATCH_LEVEL)
EXPORT
VOID
NdisAcquireRWLockRead(
    _In_  PNDIS_RW_LOCK_EX Lock,
    _Out_ _IRQL_saves_ PLOCK_STATE_EX   LockState,
    _In_  UCHAR            Flags
    );

_Acquires_exclusive_lock_(Lock)
_IRQL_raises_(DISPATCH_LEVEL)
EXPORT
VOID
NdisAcquireRWLockWrite(
    _In_  PNDIS_RW_LOCK_EX Lock,
    _Out_ _IRQL_saves_ PLOCK_STATE_EX   LockState,
    _In_  UCHAR            Flags
    );

_Releases_lock_(Lock)
_IRQL_requires_(DISPATCH_LEVEL)    
EXPORT
VOID
NdisReleaseRWLock(
    _In_ PNDIS_RW_LOCK_EX Lock,
    _In_ _IRQL_restores_ PLOCK_STATE_EX   LockState
    );

#endif

#define NdisInterlockedAddLargeStatistic(_Addend, _Increment)   \
    ExInterlockedAddLargeStatistic((PLARGE_INTEGER)_Addend, _Increment)

//
// S-List support
//

#define NdisInterlockedPushEntrySList(SListHead, SListEntry, Lock) \
    ExInterlockedPushEntrySList(SListHead, SListEntry, &(Lock)->SpinLock)

#define NdisInterlockedPopEntrySList(SListHead, Lock) \
    ExInterlockedPopEntrySList(SListHead, &(Lock)->SpinLock)

#define NdisInterlockedFlushSList(SListHead) ExInterlockedFlushSList(SListHead)

#define NdisInitializeSListHead(SListHead)  ExInitializeSListHead(SListHead)

#define NdisQueryDepthSList(SListHead)  ExQueryDepthSList(SListHead)

_IRQL_requires_max_(HIGH_LEVEL)
EXPORT
VOID
NdisGetCurrentProcessorCpuUsage(
    _Out_ PULONG                  pCpuUsage
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisGetCurrentProcessorCounts(
    _Out_ PULONG                  pIdleCount,
    _Out_ PULONG                  pKernelAndUser,
    _Out_ PULONG                  pIndex
    );


#if NDIS_LEGACY_DRIVER

/*
NdisGetSystemUpTime is deprecated, use NdisGetSystemUpTimeEx instead.
*/
_IRQL_requires_max_(HIGH_LEVEL)
DECLSPEC_DEPRECATED_DDK
EXPORT
VOID
NdisGetSystemUpTime(
    _Out_ PULONG                  pSystemUpTime
    );

#endif // NDIS_LEGACY_DRIVER

//
// List manipulation
//

/*++

VOID
NdisInitializeListHead(
    IN  PLIST_ENTRY ListHead
    );

--*/
#define NdisInitializeListHead(_ListHead) InitializeListHead(_ListHead)

//
// Configuration Requests
//

#if NDIS_LEGACY_DRIVER

_IRQL_requires_(PASSIVE_LEVEL)
_Success_(*Status >= 0)
EXPORT
VOID
NdisOpenConfiguration(
    _At_(*Status, _Must_inspect_result_)
    _Out_  PNDIS_STATUS     Status,
    _Out_ PNDIS_HANDLE      ConfigurationHandle,
    _In_  NDIS_HANDLE       WrapperConfigurationContext
    );

#endif // NDIS_LEGACY_DRIVER


_IRQL_requires_max_(APC_LEVEL)
_Success_(*Status >= 0)
EXPORT
VOID
NdisOpenConfigurationKeyByName(
    _At_(*Status, _Must_inspect_result_)
    _Out_  PNDIS_STATUS     Status,
    _In_  NDIS_HANDLE       ConfigurationHandle,
    _In_  PNDIS_STRING      SubKeyName,
    _Out_ PNDIS_HANDLE      SubKeyHandle
    );


_IRQL_requires_max_(APC_LEVEL)
_Success_(*Status >= 0)
EXPORT
VOID
NdisOpenConfigurationKeyByIndex(
    _At_(*Status, _Must_inspect_result_)
    _Out_ PNDIS_STATUS            Status,
    _In_  NDIS_HANDLE             ConfigurationHandle,
    _In_  ULONG                   Index,
    _Out_ PNDIS_STRING            KeyName,
    _Out_ PNDIS_HANDLE            KeyHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
_Success_(*Status >= 0)
EXPORT
VOID
NdisReadConfiguration(
    _At_(*Status, _Must_inspect_result_)
    _Out_ PNDIS_STATUS            Status,
    _Out_ PNDIS_CONFIGURATION_PARAMETER *ParameterValue,
    _In_  NDIS_HANDLE             ConfigurationHandle,
    _In_  PNDIS_STRING            Keyword,
    _In_  NDIS_PARAMETER_TYPE     ParameterType
    );

_IRQL_requires_(PASSIVE_LEVEL)
_Success_(*Status >= 0)
EXPORT
VOID
NdisWriteConfiguration(
    _At_(*Status, _Must_inspect_result_)
    _Out_ PNDIS_STATUS            Status,
    _In_  NDIS_HANDLE             ConfigurationHandle,
    _In_  PNDIS_STRING            Keyword,
    _In_  PNDIS_CONFIGURATION_PARAMETER ParameterValue
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisCloseConfiguration(
    _In_  __drv_freesMem(mem) NDIS_HANDLE             ConfigurationHandle
    );


_IRQL_requires_(PASSIVE_LEVEL)
_Success_(*Status >= 0)
EXPORT
VOID
NdisReadNetworkAddress(
    _At_(*Status, _Must_inspect_result_)
    _Out_ PNDIS_STATUS            Status,
    _Outptr_result_bytebuffer_to_(*NetworkAddressLength, *NetworkAddressLength) PVOID *NetworkAddress,
    _Out_ PUINT                   NetworkAddressLength,
    _In_  NDIS_HANDLE             ConfigurationHandle
    );


#if NDIS_LEGACY_MINIPORT

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
ULONG
NdisReadPciSlotInformation(
    _In_ NDIS_HANDLE             NdisAdapterHandle,
    _In_ ULONG                   SlotNumber,
    _In_ ULONG                   Offset,
    _Out_writes_bytes_(Length)
         PVOID                   Buffer,
    _In_ ULONG                   Length
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
ULONG
NdisWritePciSlotInformation(
    _In_ NDIS_HANDLE             NdisAdapterHandle,
    _In_ ULONG                   SlotNumber,
    _In_ ULONG                   Offset,
    _In_reads_bytes_(Length)
         PVOID                   Buffer,
    _In_ ULONG                   Length
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
ULONG
NdisReadPcmciaAttributeMemory(
    _In_ NDIS_HANDLE              NdisAdapterHandle,
    _In_ ULONG                    Offset,
    _Out_writes_bytes_(Length)
         PVOID                    Buffer,
    _In_ ULONG                    Length
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
ULONG
NdisWritePcmciaAttributeMemory(
    _In_ NDIS_HANDLE              NdisAdapterHandle,
    _In_ ULONG                    Offset,
    _In_reads_bytes_(Length)
         PVOID                    Buffer,
    _In_ ULONG                    Length
    );

#endif // NDIS_LEGACY_MINIPORT


#if NDIS_LEGACY_DRIVER
//
// Buffer Pool
//

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisAllocateBufferPool(
    _Out_ PNDIS_STATUS            Status,
    _Out_ PNDIS_HANDLE            PoolHandle,
    _In_  UINT                    NumberOfDescriptors
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFreeBufferPool(
    _In_  NDIS_HANDLE             PoolHandle
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisAllocateBuffer(
    _Out_     PNDIS_STATUS            Status,
    _Out_     PNDIS_BUFFER *          Buffer,
    _In_opt_  NDIS_HANDLE             PoolHandle,
    _In_reads_bytes_(Length)
              PVOID                   VirtualAddress,
    _In_      UINT                    Length
    );

#define NdisFreeBuffer(Buffer)  IoFreeMdl(Buffer)
#endif


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCopyBuffer(
    _Out_ PNDIS_STATUS            Status,
    _Out_ PNDIS_BUFFER *          Buffer,
    _In_  NDIS_HANDLE             PoolHandle,
    _In_  PVOID                   MemoryDescriptor,
    _In_  UINT                    Offset,
    _In_  UINT                    Length
    );


//
//  VOID
//  NdisCopyLookaheadData(
//      IN  PVOID               Destination,
//      IN  PVOID               Source,
//      IN  ULONG               Length,
//      IN  ULONG               ReceiveFlags
//      );
//

#if defined(_M_IX86) || defined(_M_AMD64)
#define NdisCopyLookaheadData(_Destination, _Source, _Length, _MacOptions)  \
        RtlCopyMemory(_Destination, _Source, _Length)
#else
#define NdisCopyLookaheadData(_Destination, _Source, _Length, _MacOptions)  \
    {                                                                       \
        if ((_MacOptions) & NDIS_MAC_OPTION_COPY_LOOKAHEAD_DATA)            \
        {                                                                   \
            RtlCopyMemory(_Destination, _Source, _Length);                  \
        }                                                                   \
        else                                                                \
        {                                                                   \
            PUCHAR _Src = (PUCHAR)(_Source);                                \
            PUCHAR _Dest = (PUCHAR)(_Destination);                          \
            PUCHAR _End = _Dest + (_Length);                                \
            while (_Dest < _End)                                            \
            {                                                               \
                *_Dest++ = *_Src++;                                         \
            }                                                               \
        }                                                                   \
    }
#endif

#if NDIS_LEGACY_DRIVER
//
// Packet Pool
//
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisAllocatePacketPool(
    _Out_ PNDIS_STATUS            Status,
    _Out_ PNDIS_HANDLE            PoolHandle,
    _In_  UINT                    NumberOfDescriptors,
    _In_  UINT                    ProtocolReservedLength
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisAllocatePacketPoolEx(
    _Out_ PNDIS_STATUS            Status,
    _Out_ PNDIS_HANDLE            PoolHandle,
    _In_  UINT                    NumberOfDescriptors,
    _In_  UINT                    NumberOfOverflowDescriptors,
    _In_  UINT                    ProtocolReservedLength
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisSetPacketPoolProtocolId(
    _In_  NDIS_HANDLE             PacketPoolHandle,
    _In_  UINT                    ProtocolId
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
UINT
NdisPacketPoolUsage(
    _In_ NDIS_HANDLE             PoolHandle
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
UINT
NdisPacketSize(
    _In_ UINT                    ProtocolReservedSize
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_HANDLE
NdisGetPoolFromPacket(
    _In_ PNDIS_PACKET            Packet
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
PNDIS_PACKET_STACK
NdisIMGetCurrentPacketStack(
    _In_  PNDIS_PACKET            Packet,
    _Out_ BOOLEAN *               StacksRemaining
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFreePacketPool(
    _In_  NDIS_HANDLE             PoolHandle
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFreePacket(
    _In_  PNDIS_PACKET            Packet
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
_IRQL_requires_min_(DISPATCH_LEVEL)
EXPORT
VOID
NdisDprFreePacket(
    _In_ PNDIS_PACKET            Packet
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
_IRQL_requires_min_(DISPATCH_LEVEL)
EXPORT
VOID
NdisDprFreePacketNonInterlocked(
    _In_ PNDIS_PACKET            Packet
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisAllocatePacket(
    _Out_ PNDIS_STATUS            Status,
    _Out_ PNDIS_PACKET *          Packet,
    _In_  NDIS_HANDLE             PoolHandle
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
_IRQL_requires_min_(DISPATCH_LEVEL)
EXPORT
VOID
NdisDprAllocatePacket(
    _Out_ PNDIS_STATUS            Status,
    _Out_ PNDIS_PACKET*           Packet,
    _In_  NDIS_HANDLE             PoolHandle
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
_IRQL_requires_min_(DISPATCH_LEVEL)
EXPORT
VOID
NdisDprAllocatePacketNonInterlocked(
    _Out_ PNDIS_STATUS            Status,
    _Out_ PNDIS_PACKET *          Packet,
    _In_  NDIS_HANDLE             PoolHandle
    );

// VOID
// NdisReinitializePacket(
//  IN OUT PNDIS_PACKET         Packet
//  );
#define NdisReinitializePacket(Packet)                                      \
{                                                                           \
    (Packet)->Private.Head = (PNDIS_BUFFER)NULL;                            \
    (Packet)->Private.ValidCounts = FALSE;                                  \
}

#endif // NDIS_LEGACY_DRIVER

#define NdisFreeBuffer(Buffer)  IoFreeMdl(Buffer)

#if NDIS_LEGACY_DRIVER
#define NdisQueryBuffer(_Buffer, _VirtualAddress, _Length)                  \
{                                                                           \
    if (ARGUMENT_PRESENT(_VirtualAddress))                                  \
    {                                                                       \
        *(PVOID *)(_VirtualAddress) = MmGetSystemAddressForMdl(_Buffer);    \
    }                                                                       \
    *(_Length) = MmGetMdlByteCount(_Buffer);                                \
}
#endif // NDIS_LEGACY_DRIVER

#define NdisQueryBufferSafe(_Buffer, _VirtualAddress, _Length, _Priority)   \
{                                                                           \
    if (ARGUMENT_PRESENT(_VirtualAddress))                                  \
    {                                                                       \
        *(PVOID *)(_VirtualAddress) = MmGetSystemAddressForMdlSafe(_Buffer, _Priority); \
    }                                                                       \
    *(_Length) = MmGetMdlByteCount(_Buffer);                                \
}

#define NdisQueryBufferOffset(_Buffer, _Offset, _Length)                    \
{                                                                           \
    *(_Offset) = MmGetMdlByteOffset(_Buffer);                               \
    *(_Length) = MmGetMdlByteCount(_Buffer);                                \
}


#if NDIS_LEGACY_DRIVER

#define NdisGetFirstBufferFromPacket(_Packet,                               \
                                     _FirstBuffer,                          \
                                     _FirstBufferVA,                        \
                                     _FirstBufferLength,                    \
                                     _TotalBufferLength)                    \
    {                                                                       \
        PNDIS_BUFFER    _pBuf;                                              \
                                                                            \
        _pBuf = (_Packet)->Private.Head;                                    \
        *(_FirstBuffer) = _pBuf;                                            \
        if (_pBuf)                                                          \
        {                                                                   \
            *(_FirstBufferVA) = MmGetSystemAddressForMdl(_pBuf);            \
            *(_FirstBufferLength) =                                         \
            *(_TotalBufferLength) = MmGetMdlByteCount(_pBuf);               \
            for (_pBuf = _pBuf->Next;                                       \
                 _pBuf != NULL;                                             \
                 _pBuf = _pBuf->Next)                                       \
            {                                                               \
                *(_TotalBufferLength) += MmGetMdlByteCount(_pBuf);          \
            }                                                               \
        }                                                                   \
        else                                                                \
        {                                                                   \
            *(_FirstBufferVA) = 0;                                          \
            *(_FirstBufferLength) = 0;                                      \
            *(_TotalBufferLength) = 0;                                      \
        }                                                                   \
    }

#define NdisGetFirstBufferFromPacketSafe(_Packet,                           \
                                         _FirstBuffer,                      \
                                         _FirstBufferVA,                    \
                                         _FirstBufferLength,                \
                                         _TotalBufferLength,                \
                                         _Priority)                         \
    {                                                                       \
        PNDIS_BUFFER    _pBuf;                                              \
                                                                            \
        _pBuf = (_Packet)->Private.Head;                                    \
        *(_FirstBuffer) = _pBuf;                                            \
        if (_pBuf)                                                          \
        {                                                                   \
            *(_FirstBufferVA) = MmGetSystemAddressForMdlSafe(_pBuf, _Priority); \
            *(_FirstBufferLength) = *(_TotalBufferLength) = MmGetMdlByteCount(_pBuf); \
            for (_pBuf = _pBuf->Next;                                       \
                 _pBuf != NULL;                                             \
                 _pBuf = _pBuf->Next)                                       \
            {                                                               \
                *(_TotalBufferLength) += MmGetMdlByteCount(_pBuf);          \
            }                                                               \
        }                                                                   \
        else                                                                \
        {                                                                   \
            *(_FirstBufferVA) = 0;                                          \
            *(_FirstBufferLength) = 0;                                      \
            *(_TotalBufferLength) = 0;                                      \
        }                                                                   \
    }

#endif // NDIS_LEGACY_DRIVER

#define NDIS_BUFFER_TO_SPAN_PAGES(_Buffer)                                  \
    (MmGetMdlByteCount(_Buffer)==0 ?                                        \
                1 :                                                         \
                (ADDRESS_AND_SIZE_TO_SPAN_PAGES(                            \
                        MmGetMdlVirtualAddress(_Buffer),                    \
                        MmGetMdlByteCount(_Buffer))))

#define NdisGetBufferPhysicalArraySize(Buffer, ArraySize)                   \
    (*(ArraySize) = NDIS_BUFFER_TO_SPAN_PAGES(Buffer))


/*++

NDIS_BUFFER_LINKAGE(
    IN  PNDIS_BUFFER            Buffer
    );

--*/

#define NDIS_BUFFER_LINKAGE(Buffer) ((Buffer)->Next)


#if NDIS_LEGACY_DRIVER

/*++

VOID
NdisRecalculatePacketCounts(
    IN OUT PNDIS_PACKET         Packet
    );

--*/

#define NdisRecalculatePacketCounts(Packet)                                 \
{                                                                           \
    {                                                                       \
        PNDIS_BUFFER TmpBuffer = (Packet)->Private.Head;                    \
        if (TmpBuffer)                                                      \
        {                                                                   \
            while (TmpBuffer->Next)                                         \
            {                                                               \
                TmpBuffer = TmpBuffer->Next;                                \
            }                                                               \
            (Packet)->Private.Tail = TmpBuffer;                             \
        }                                                                   \
        (Packet)->Private.ValidCounts = FALSE;                              \
    }                                                                       \
}


/*++

VOID
NdisChainBufferAtFront(
    IN OUT PNDIS_PACKET         Packet,
    IN OUT PNDIS_BUFFER         Buffer
    );

--*/

#define NdisChainBufferAtFront(Packet, Buffer)                              \
{                                                                           \
    PNDIS_BUFFER TmpBuffer = (Buffer);                                      \
                                                                            \
    for (;;)                                                                \
    {                                                                       \
        if (TmpBuffer->Next == (PNDIS_BUFFER)NULL)                          \
            break;                                                          \
        TmpBuffer = TmpBuffer->Next;                                        \
    }                                                                       \
    if ((Packet)->Private.Head == NULL)                                     \
    {                                                                       \
        (Packet)->Private.Tail = TmpBuffer;                                 \
    }                                                                       \
    TmpBuffer->Next = (Packet)->Private.Head;                               \
    (Packet)->Private.Head = (Buffer);                                      \
    (Packet)->Private.ValidCounts = FALSE;                                  \
}

/*++

VOID
NdisChainBufferAtBack(
    IN OUT PNDIS_PACKET         Packet,
    IN OUT PNDIS_BUFFER         Buffer
    );

--*/

#define NdisChainBufferAtBack(Packet, Buffer)                               \
{                                                                           \
    PNDIS_BUFFER TmpBuffer = (Buffer);                                      \
                                                                            \
    for (;;)                                                                \
    {                                                                       \
        if (TmpBuffer->Next == NULL)                                        \
            break;                                                          \
        TmpBuffer = TmpBuffer->Next;                                        \
    }                                                                       \
    if ((Packet)->Private.Head != NULL)                                     \
    {                                                                       \
        (Packet)->Private.Tail->Next = (Buffer);                            \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        (Packet)->Private.Head = (Buffer);                                  \
    }                                                                       \
    (Packet)->Private.Tail = TmpBuffer;                                     \
    (Packet)->Private.ValidCounts = FALSE;                                  \
}

EXPORT
VOID
NdisUnchainBufferAtFront(
    IN OUT PNDIS_PACKET         Packet,
    OUT PNDIS_BUFFER *          Buffer
    );

EXPORT
VOID
NdisUnchainBufferAtBack(
    IN OUT PNDIS_PACKET         Packet,
    OUT PNDIS_BUFFER *          Buffer
    );


/*++

VOID
NdisQueryPacket(
    IN  PNDIS_PACKET            _Packet,
    OUT PUINT                   _PhysicalBufferCount OPTIONAL,
    OUT PUINT                   _BufferCount OPTIONAL,
    OUT PNDIS_BUFFER *          _FirstBuffer OPTIONAL,
    OUT PUINT                   _TotalPacketLength OPTIONAL
    );

--*/

#pragma warning(push)
#pragma warning(disable:4127)
__inline
VOID
NdisQueryPacket(
    IN  PNDIS_PACKET            _Packet,
    OUT PUINT                   _PhysicalBufferCount OPTIONAL,
    OUT PUINT                   _BufferCount OPTIONAL,
    OUT PNDIS_BUFFER *          _FirstBuffer OPTIONAL,
    OUT PUINT                   _TotalPacketLength OPTIONAL
    )
{
    if ((_FirstBuffer) != NULL)
    {
        PNDIS_BUFFER * __FirstBuffer = (_FirstBuffer);
        *(__FirstBuffer) = (_Packet)->Private.Head;
    }
    if ((_TotalPacketLength) || (_BufferCount) || (_PhysicalBufferCount))
    {
        if (!(_Packet)->Private.ValidCounts)
        {
            PNDIS_BUFFER TmpBuffer = (_Packet)->Private.Head;
            UINT PTotalLength = 0, PPhysicalCount = 0, PAddedCount = 0;
            UINT PacketLength, Offset;

            while (TmpBuffer != (PNDIS_BUFFER)NULL)
            {
                NdisQueryBufferOffset(TmpBuffer, &Offset, &PacketLength);
                UNREFERENCED_PARAMETER(Offset);
                PTotalLength += PacketLength;
                PPhysicalCount += (UINT)NDIS_BUFFER_TO_SPAN_PAGES(TmpBuffer);
                ++PAddedCount;
                TmpBuffer = TmpBuffer->Next;
            }
            (_Packet)->Private.Count = PAddedCount;
            (_Packet)->Private.TotalLength = PTotalLength;
            (_Packet)->Private.PhysicalCount = PPhysicalCount;
            (_Packet)->Private.ValidCounts = TRUE;
        }

        if (_PhysicalBufferCount)
        {
            PUINT __PhysicalBufferCount = (_PhysicalBufferCount);
            *(__PhysicalBufferCount) = (_Packet)->Private.PhysicalCount;
        }
        if (_BufferCount)
        {
            PUINT __BufferCount = (_BufferCount);
            *(__BufferCount) = (_Packet)->Private.Count;
        }
        if (_TotalPacketLength)
        {
            PUINT __TotalPacketLength = (_TotalPacketLength);
            *(__TotalPacketLength) = (_Packet)->Private.TotalLength;
        }
    }
}
#pragma warning(pop)

/*++

VOID
NdisQueryPacketLength(
    IN  PNDIS_PACKET            _Packet,
    OUT PUINT                   _TotalPacketLength OPTIONAL
    );

--*/

#define NdisQueryPacketLength(_Packet,                                      \
                              _TotalPacketLength)                           \
{                                                                           \
    if (!(_Packet)->Private.ValidCounts)                                    \
    {                                                                       \
        NdisQueryPacket(_Packet, NULL, NULL, NULL, _TotalPacketLength);     \
    }                                                                       \
    else *(_TotalPacketLength) = (_Packet)->Private.TotalLength;            \
}

#endif // NDIS_LEGACY_DRIVER


/*++

VOID
NdisGetNextBuffer(
    IN  PNDIS_BUFFER            CurrentBuffer,
    OUT PNDIS_BUFFER *          NextBuffer
    );

--*/

#define NdisGetNextBuffer(CurrentBuffer, NextBuffer)                        \
{                                                                           \
    *(NextBuffer) = (CurrentBuffer)->Next;                                  \
}


#define NdisAdjustBufferLength(Buffer, Length)  (((Buffer)->ByteCount) = (Length))


#if NDIS_SUPPORT_NDIS6
/*
VOID
NdisAdjustMdlLength(
    IN  PMDL                    Mdl,
    IN  UINT                    Length
    );

*/
#define NdisAdjustMdlLength(_Mdl, _Length)  (((_Mdl)->ByteCount) = (_Length))
#endif // NDIS_SUPPORT_NDIS6

#if NDIS_LEGACY_DRIVER

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCopyFromPacketToPacket(
    _In_  PNDIS_PACKET            Destination,
    _In_  UINT                    DestinationOffset,
    _In_  UINT                    BytesToCopy,
    _In_  PNDIS_PACKET            Source,
    _In_  UINT                    SourceOffset,
    _Out_ PUINT                   BytesCopied
    );

EXPORT
VOID
NdisCopyFromPacketToPacketSafe(
    IN  PNDIS_PACKET            Destination,
    IN  UINT                    DestinationOffset,
    IN  UINT                    BytesToCopy,
    IN  PNDIS_PACKET            Source,
    IN  UINT                    SourceOffset,
    OUT PUINT                   BytesCopied,
    IN  MM_PAGE_PRIORITY        Priority
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
__drv_preferredFunction("NdisAllocateMemoryWithTag", "Obsolete")
DECLSPEC_DEPRECATED_DDK
EXPORT
NDIS_STATUS
NdisAllocateMemory(
    _At_(*VirtualAddress, __drv_allocatesMem(Mem))
    _Outptr_result_bytebuffer_(Length)
          PVOID *                          VirtualAddress,
    _In_  UINT                             Length,
    _In_  UINT                             MemoryFlags,
    _In_  NDIS_PHYSICAL_ADDRESS            HighestAcceptableAddress
    );

#endif // NDIS_LEGACY_DRIVER

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisAllocateMemoryWithTag(
    _At_(*VirtualAddress, __drv_allocatesMem(Mem))
    _Outptr_result_bytebuffer_(Length)
          PVOID *                 VirtualAddress,
    _In_  UINT                    Length,
    _In_  ULONG                   Tag
    );

_When_(MemoryFlags==0,
    _IRQL_requires_max_(DISPATCH_LEVEL))
_When_(MemoryFlags==NDIS_MEMORY_NONCACHED,
    _IRQL_requires_max_(APC_LEVEL))
_When_(MemoryFlags==NDIS_MEMORY_CONTIGUOUS,
    _IRQL_requires_(PASSIVE_LEVEL))
EXPORT
VOID
NdisFreeMemory(
    _In_reads_bytes_(Length) __drv_freesMem(Mem)
            PVOID           VirtualAddress,
    _In_    UINT            Length,
    _In_ _Pre_satisfies_(MemoryFlags ==0 || MemoryFlags == NDIS_MEMORY_NONCACHED || MemoryFlags ==NDIS_MEMORY_CONTIGUOUS)
            UINT            MemoryFlags
    );

#if (NDIS_SUPPORT_NDIS620)

EXPORT
VOID
NdisFreeMemoryWithTag(
    IN  PVOID                   VirtualAddress,
    IN  ULONG                   Tag
    );

#endif // (NDIS_SUPPORT_NDIS620)

/*++
VOID
NdisStallExecution(
    IN  UINT                    MicrosecondsToStall
    )
--*/

#define NdisStallExecution(MicroSecondsToStall)     KeStallExecutionProcessor(MicroSecondsToStall)

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisInitializeEvent(
    _Out_  PNDIS_EVENT             Event
);

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisSetEvent(
    _In_  PNDIS_EVENT             Event
);

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisResetEvent(
    _In_  PNDIS_EVENT             Event
);

_When_(MsToWait !=0, _Check_return_)
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
BOOLEAN
NdisWaitEvent(
    _In_  PNDIS_EVENT             Event,
    _In_  UINT                    MsToWait
);


#if NDIS_LEGACY_DRIVER
/*++
VOID
NdisInitializeWorkItem(
    IN  PNDIS_WORK_ITEM         WorkItem,
    IN  NDIS_PROC               Routine,
    IN  PVOID                   Context
    );
--*/

#define NdisInitializeWorkItem(_WI_, _R_, _C_)  \
    {                                           \
        (_WI_)->Context = _C_;                  \
        (_WI_)->Routine = _R_;                  \
    }

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisScheduleWorkItem(
    _In_  __drv_aliasesMem PNDIS_WORK_ITEM         WorkItem
    );

#endif // NDIS_LEGACY_DRIVER

//
// Simple I/O support
//

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisOpenFile(
    _Out_ PNDIS_STATUS            Status,
    _Out_ PNDIS_HANDLE            FileHandle,
    _Out_ PUINT                   FileLength,
    _In_  PNDIS_STRING            FileName,
    _In_  NDIS_PHYSICAL_ADDRESS   HighestAcceptableAddress
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisCloseFile(
    _In_ NDIS_HANDLE             FileHandle
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMapFile(
    _Out_ PNDIS_STATUS            Status,
    _Out_ PVOID *                 MappedBuffer,
    _In_  NDIS_HANDLE             FileHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisUnmapFile(
    _In_  NDIS_HANDLE             FileHandle
    );


//
// Portability extensions
//

/*++
VOID
NdisFlushBuffer(
    IN  PNDIS_BUFFER            Buffer,
    IN  BOOLEAN                 WriteToDevice
    )
--*/

#define NdisFlushBuffer(Buffer,WriteToDevice)                               \
        KeFlushIoBuffers((Buffer),!(WriteToDevice), TRUE)

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
ULONG
NdisGetSharedDataAlignment(
    VOID
    );


//
// Raw Routines
//

//
// Write Port Raw
//

/*++
VOID
NdisRawWritePortUchar(
    IN  ULONG_PTR               Port,
    IN  UCHAR                   Data
    )
--*/
#define NdisRawWritePortUchar(Port,Data)                                    \
        WRITE_PORT_UCHAR((PUCHAR)(Port),(UCHAR)(Data))

/*++
VOID
NdisRawWritePortUshort(
    IN  ULONG_PTR               Port,
    IN  USHORT                  Data
    )
--*/
#define NdisRawWritePortUshort(Port,Data)                                   \
        WRITE_PORT_USHORT((PUSHORT)(Port),(USHORT)(Data))

/*++
VOID
NdisRawWritePortUlong(
    IN  ULONG_PTR               Port,
    IN  ULONG                   Data
    )
--*/
#define NdisRawWritePortUlong(Port,Data)                                    \
        WRITE_PORT_ULONG((PULONG)(Port),(ULONG)(Data))


//
// Raw Write Port Buffers
//

/*++
VOID
NdisRawWritePortBufferUchar(
    IN  ULONG_PTR               Port,
    IN  PUCHAR                  Buffer,
    IN  ULONG                   Length
    )
--*/
#define NdisRawWritePortBufferUchar(Port,Buffer,Length) \
        WRITE_PORT_BUFFER_UCHAR((PUCHAR)(Port),(PUCHAR)(Buffer),(Length))

/*++
VOID
NdisRawWritePortBufferUshort(
    IN  ULONG_PTR               Port,
    IN  PUSHORT                 Buffer,
    IN  ULONG                   Length
    )
--*/
#if defined(_M_IX86) || defined(_M_AMD64)
#define NdisRawWritePortBufferUshort(Port,Buffer,Length)                    \
        WRITE_PORT_BUFFER_USHORT((PUSHORT)(Port),(PUSHORT)(Buffer),(Length))
#else
#define NdisRawWritePortBufferUshort(Port,Buffer,Length)                    \
{                                                                           \
        ULONG_PTR _Port = (ULONG_PTR)(Port);                                \
        PUSHORT _Current = (Buffer);                                        \
        PUSHORT _End = _Current + (Length);                                 \
        for ( ; _Current < _End; ++_Current)                                \
        {                                                                   \
            WRITE_PORT_USHORT((PUSHORT)_Port,*(UNALIGNED USHORT *)_Current);\
        }                                                                   \
}
#endif


/*++
VOID
NdisRawWritePortBufferUlong(
    IN  ULONG_PTR               Port,
    IN  PULONG                  Buffer,
    IN  ULONG                   Length
    )
--*/
#if defined(_M_IX86) || defined(_M_AMD64)
#define NdisRawWritePortBufferUlong(Port,Buffer,Length)                     \
        WRITE_PORT_BUFFER_ULONG((PULONG)(Port),(PULONG)(Buffer),(Length))
#else
#define NdisRawWritePortBufferUlong(Port,Buffer,Length)                     \
{                                                                           \
        ULONG_PTR _Port = (ULONG_PTR)(Port);                                \
        PULONG _Current = (Buffer);                                         \
        PULONG _End = _Current + (Length);                                  \
        for ( ; _Current < _End; ++_Current)                                \
        {                                                                   \
            WRITE_PORT_ULONG((PULONG)_Port,*(UNALIGNED ULONG *)_Current);   \
        }                                                                   \
}
#endif


//
// Raw Read Ports
//

/*++
VOID
NdisRawReadPortUchar(
    IN  ULONG_PTR               Port,
    OUT PUCHAR                  Data
    )
--*/
#define NdisRawReadPortUchar(Port, Data) \
        *(Data) = READ_PORT_UCHAR((PUCHAR)(Port))

/*++
VOID
NdisRawReadPortUshort(
    IN  ULONG_PTR               Port,
    OUT PUSHORT                 Data
    )
--*/
#define NdisRawReadPortUshort(Port,Data) \
        *(Data) = READ_PORT_USHORT((PUSHORT)(Port))

/*++
VOID
NdisRawReadPortUlong(
    IN  ULONG_PTR               Port,
    OUT PULONG                  Data
    )
--*/
#define NdisRawReadPortUlong(Port,Data) \
        *(Data) = READ_PORT_ULONG((PULONG)(Port))


//
// Raw Read Buffer Ports
//

/*++
VOID
NdisRawReadPortBufferUchar(
    IN  ULONG_PTR               Port,
    OUT PUCHAR                  Buffer,
    IN  ULONG                   Length
    )
--*/
#define NdisRawReadPortBufferUchar(Port,Buffer,Length)                      \
        READ_PORT_BUFFER_UCHAR((PUCHAR)(Port),(PUCHAR)(Buffer),(Length))


/*++
VOID
NdisRawReadPortBufferUshort(
    IN  ULONG_PTR               Port,
    OUT PUSHORT                 Buffer,
    IN  ULONG                   Length
    )
--*/
#if defined(_M_IX86) || defined(_M_AMD64)
#define NdisRawReadPortBufferUshort(Port,Buffer,Length)                     \
        READ_PORT_BUFFER_USHORT((PUSHORT)(Port),(PUSHORT)(Buffer),(Length))
#else
#define NdisRawReadPortBufferUshort(Port,Buffer,Length)                     \
{                                                                           \
        ULONG_PTR _Port = (ULONG_PTR)(Port);                                \
        PUSHORT _Current = (Buffer);                                        \
        PUSHORT _End = _Current + (Length);                                 \
        for ( ; _Current < _End; ++_Current)                                \
        {                                                                   \
            *(UNALIGNED USHORT *)_Current = READ_PORT_USHORT((PUSHORT)_Port); \
        }                                                                   \
}
#endif


/*++
VOID
NdisRawReadPortBufferUlong(
    IN  ULONG_PTR               Port,
    OUT PULONG                  Buffer,
    IN  ULONG                   Length
    )
--*/
#if defined(_M_IX86) || defined(_M_AMD64)
#define NdisRawReadPortBufferUlong(Port,Buffer,Length)                      \
        READ_PORT_BUFFER_ULONG((PULONG)(Port),(PULONG)(Buffer),(Length))
#else
#define NdisRawReadPortBufferUlong(Port,Buffer,Length)                      \
{                                                                           \
        ULONG_PTR _Port = (ULONG_PTR)(Port);                                \
        PULONG _Current = (Buffer);                                         \
        PULONG _End = _Current + (Length);                                  \
        for ( ; _Current < _End; ++_Current)                                \
        {                                                                   \
            *(UNALIGNED ULONG *)_Current = READ_PORT_ULONG((PULONG)_Port);  \
        }                                                                   \
}
#endif


//
// Write Registers
//

/*++
VOID
NdisWriteRegisterUchar(
    IN  PUCHAR                  Register,
    IN  UCHAR                   Data
    )
--*/

#if defined(_M_IX86) || defined(_M_AMD64) || defined(_M_ARM)
#define NdisWriteRegisterUchar(Register,Data)                               \
        WRITE_REGISTER_UCHAR((Register),(Data))
#else
#define NdisWriteRegisterUchar(Register,Data)                               \
    {                                                                       \
        WRITE_REGISTER_UCHAR((Register),(Data));                            \
        READ_REGISTER_UCHAR(Register);                                      \
    }
#endif

/*++
VOID
NdisWriteRegisterUshort(
    IN  PUCHAR                  Register,
    IN  USHORT                  Data
    )
--*/

#if defined(_M_IX86) || defined(_M_AMD64) || defined(_M_ARM)
#define NdisWriteRegisterUshort(Register,Data)                              \
        WRITE_REGISTER_USHORT((Register),(Data))
#else
#define NdisWriteRegisterUshort(Register,Data)                              \
    {                                                                       \
        WRITE_REGISTER_USHORT((Register),(Data));                           \
        READ_REGISTER_USHORT(Register);                                     \
    }
#endif

/*++
VOID
NdisWriteRegisterUlong(
    IN  PUCHAR                  Register,
    IN  ULONG                   Data
    )
--*/

#if defined(_M_IX86) || defined(_M_AMD64) || defined(_M_ARM)
#define NdisWriteRegisterUlong(Register,Data)   WRITE_REGISTER_ULONG((Register),(Data))
#else
#define NdisWriteRegisterUlong(Register,Data)                               \
    {                                                                       \
        WRITE_REGISTER_ULONG((Register),(Data));                            \
        READ_REGISTER_ULONG(Register);                                      \
    }
#endif

/*++
VOID
NdisReadRegisterUchar(
    IN  PUCHAR                  Register,
    OUT PUCHAR                  Data
    )
--*/
#if defined(_M_IX86)
#define NdisReadRegisterUchar(Register,Data)    \
        _ReadWriteBarrier();                    \
        *(Data) = *((volatile UCHAR * const)(Register));
#else
#define NdisReadRegisterUchar(Register,Data)    *(Data) = READ_REGISTER_UCHAR((PUCHAR)(Register))
#endif

/*++
VOID
NdisReadRegisterUshort(
    IN  PUSHORT                 Register,
    OUT PUSHORT                 Data
    )
--*/
#if defined(_M_IX86)
#define NdisReadRegisterUshort(Register,Data)   \
        _ReadWriteBarrier();                    \
        *(Data) = *((volatile USHORT * const)(Register))
#else
#define NdisReadRegisterUshort(Register,Data)   *(Data) = READ_REGISTER_USHORT((PUSHORT)(Register))
#endif

/*++
VOID
NdisReadRegisterUlong(
    IN  PULONG                  Register,
    OUT PULONG                  Data
    )
--*/
#if defined(_M_IX86)
#define NdisReadRegisterUlong(Register,Data)    \
        _ReadWriteBarrier();                    \
        *(Data) = *((volatile ULONG * const)(Register))
#else
#define NdisReadRegisterUlong(Register,Data)    *(Data) = READ_REGISTER_ULONG((PULONG)(Register))
#endif

#define NdisEqualString(_String1, _String2, _CaseInsensitive)               \
            RtlEqualUnicodeString(_String1, _String2, _CaseInsensitive)

#define NdisEqualUnicodeString(_String1, _String2, _CaseInsensitive)        \
            RtlEqualUnicodeString(_String1, _String2, _CaseInsensitive)

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID __cdecl
NdisWriteErrorLogEntry(
    _In_  NDIS_HANDLE             NdisAdapterHandle,
    _In_  NDIS_ERROR_CODE         ErrorCode,
    _In_  ULONG                   NumberOfErrorValues,
    ...
    );

_IRQL_requires_(PASSIVE_LEVEL)
_Success_(Destination->Buffer != 0)
EXPORT
VOID
NdisInitializeString(
    _Out_ _At_(Destination->Buffer, __drv_allocatesMem(Mem))
        PNDIS_STRING Destination,
    _In_ _Null_terminated_ PUCHAR Source
    );

#define NdisFreeString(String) NdisFreeMemory((String).Buffer, (String).MaximumLength, 0)

#define NdisPrintString(String) DbgPrint("%ls",(String).Buffer)


/*++

VOID
NdisCreateLookaheadBufferFromSharedMemory(
    IN  PVOID                   pSharedMemory,
    IN  UINT                    LookaheadLength,
    OUT PVOID *                 pLookaheadBuffer
    );

--*/

#define NdisCreateLookaheadBufferFromSharedMemory(_S, _L, _B)   ((*(_B)) = (_S))

/*++

VOID
NdisDestroyLookaheadBufferFromSharedMemory(
    IN  PVOID                   pLookaheadBuffer
    );

--*/

#define NdisDestroyLookaheadBufferFromSharedMemory(_B)


//
// The following declarations are shared between ndismac.h and ndismini.h. They
// are meant to be for internal use only. They should not be used directly by
// miniport drivers.
//

//
// declare these first since they point to each other
//

typedef struct _NDIS_WRAPPER_HANDLE     NDIS_WRAPPER_HANDLE, *PNDIS_WRAPPER_HANDLE;
typedef struct _NDIS_PROTOCOL_BLOCK     NDIS_PROTOCOL_BLOCK, *PNDIS_PROTOCOL_BLOCK;
typedef struct _NDIS_OPEN_BLOCK         NDIS_OPEN_BLOCK, *PNDIS_OPEN_BLOCK;
typedef struct _NDIS_M_DRIVER_BLOCK     NDIS_M_DRIVER_BLOCK, *PNDIS_M_DRIVER_BLOCK;
typedef struct _NDIS_MINIPORT_BLOCK     NDIS_MINIPORT_BLOCK,*PNDIS_MINIPORT_BLOCK;
typedef struct _CO_CALL_PARAMETERS      CO_CALL_PARAMETERS, *PCO_CALL_PARAMETERS;
typedef struct _CO_MEDIA_PARAMETERS     CO_MEDIA_PARAMETERS, *PCO_MEDIA_PARAMETERS;
typedef struct _NDIS_CALL_MANAGER_CHARACTERISTICS *PNDIS_CALL_MANAGER_CHARACTERISTICS;
typedef struct _NDIS_OFFLOAD NDIS_OFFLOAD, *PNDIS_OFFLOAD;
typedef struct _NDIS_AF_LIST            NDIS_AF_LIST, *PNDIS_AF_LIST;
typedef struct _X_FILTER                ETH_FILTER, *PETH_FILTER;
typedef struct _X_FILTER                TR_FILTER, *PTR_FILTER;
typedef struct _X_FILTER                NULL_FILTER, *PNULL_FILTER;

#if NDIS_SUPPORT_NDIS6

typedef USHORT NET_FRAME_TYPE, *PNET_FRAME_TYPE;

#endif NDIS_SUPPORT_NDIS6



//
// Timers.
//

typedef
_IRQL_requires_(DISPATCH_LEVEL)
_IRQL_requires_same_
_Function_class_(NDIS_TIMER_FUNCTION)
VOID
(NDIS_TIMER_FUNCTION) (
    _In_  PVOID                   SystemSpecific1,
    _In_  PVOID                   FunctionContext,
    _In_  PVOID                   SystemSpecific2,
    _In_  PVOID                   SystemSpecific3
    );
typedef NDIS_TIMER_FUNCTION (*PNDIS_TIMER_FUNCTION);

typedef struct _NDIS_TIMER
{
    KTIMER      Timer;
    KDPC        Dpc;
} NDIS_TIMER, *PNDIS_TIMER;


#if NDIS_SUPPORT_NDIS6
__drv_preferredFunction(NdisAllocateTimerObject, "Not supported for NDIS 6.0 drivers in Windows Vista. Use NdisAllocateTimerObject instead.")
#endif NDIS_SUPPORT_NDIS6
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisInitializeTimer(
    _Inout_   PNDIS_TIMER             Timer,
    _In_      PNDIS_TIMER_FUNCTION    TimerFunction,
    _In_opt_   _Points_to_data_
              PVOID                   FunctionContext
    );

#if NDIS_SUPPORT_NDIS6
__drv_preferredFunction(NdisCancelTimerObject, "Not supported for NDIS 6.0 drivers in Windows Vista. Use NdisCancelTimerObject instead.")
#endif NDIS_SUPPORT_NDIS6
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCancelTimer(
    _In_  PNDIS_TIMER             Timer,
    _Out_ _At_(*TimerCancelled, _Must_inspect_result_)
          PBOOLEAN                TimerCancelled
    );

#if NDIS_SUPPORT_NDIS6
__drv_preferredFunction(NdisSetTimerObject, "Not supported for NDIS 6.0 drivers in Windows Vista. Use NdisSetTimerObject instead.")
#endif NDIS_SUPPORT_NDIS6
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisSetTimer(
    _In_  PNDIS_TIMER             Timer,
    _In_  UINT                    MillisecondsToDelay
    );

#if NDIS_SUPPORT_NDIS6
__drv_preferredFunction(NdisSetTimerObject, "Not supported for NDIS 6.0 drivers in Windows Vista. Use NdisSetTimerObject instead.")
#endif NDIS_SUPPORT_NDIS6
EXPORT
VOID
NdisSetPeriodicTimer(
    _In_  PNDIS_TIMER             NdisTimer,
    _In_  UINT                    MillisecondsPeriod
    );

#if NDIS_SUPPORT_NDIS6
__drv_preferredFunction(NdisSetTimerObject, "Not supported for NDIS 6.0 drivers in Windows Vista. Use NdisSetTimerObject instead.")
#endif NDIS_SUPPORT_NDIS6

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisSetTimerEx(
    _In_  PNDIS_TIMER             NdisTimer,
    _In_  UINT                    MillisecondsToDelay,
    _In_  _Points_to_data_
          PVOID                   FunctionContext
    );


#if NDIS_SUPPORT_60_COMPATIBLE_API
//
// System processor count
//

_IRQL_requires_max_(HIGH_LEVEL)
EXPORT
CCHAR
NdisSystemProcessorCount(
    VOID
    );

#endif


_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
PVOID
NdisGetRoutineAddress(
    _In_ PNDIS_STRING     NdisRoutineName
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
UINT
NdisGetVersion(
    VOID
    );


//
// Ansi/Unicode support routines
//


#define NdisInitAnsiString(_as, s)              RtlInitString(_as, s)
#define NdisInitUnicodeString(_us, s)           RtlInitUnicodeString(_us, s)
#define NdisAnsiStringToUnicodeString(_us, _as) RtlAnsiStringToUnicodeString(_us, _as, FALSE)
#define NdisUnicodeStringToAnsiString(_as, _us) RtlUnicodeStringToAnsiString(_as, _us, FALSE)
#define NdisUpcaseUnicodeString(_d, _s)         RtlUpcaseUnicodeString(_d, _s, FALSE)

//
// Non-paged lookaside list support routines
//

_IRQL_requires_max_(DISPATCH_LEVEL)
FORCEINLINE
VOID
NdisInitializeNPagedLookasideList(
    _Out_       PNPAGED_LOOKASIDE_LIST Lookaside,
    _In_opt_    PALLOCATE_FUNCTION Allocate,
    _In_opt_    PFREE_FUNCTION Free,
    _In_        ULONG Flags,
    _In_        SIZE_T Size,
    _In_        ULONG Tag,
    _In_        USHORT Depth
    )
{

#if NTDDI_VERSION >= NTDDI_WIN8

    UINT Version;

    Version = NdisGetVersion();

    if (Version >= NDIS_RUNTIME_VERSION_630)
    {
        Flags |= POOL_NX_ALLOCATION;
    }

#endif

    ExInitializeNPagedLookasideList(
        Lookaside,
        Allocate,
        Free,
        Flags,
        Size,
        Tag,
        Depth);
}

#define NdisDeleteNPagedLookasideList(_L)           ExDeleteNPagedLookasideList(_L)
#define NdisAllocateFromNPagedLookasideList(_L)     ExAllocateFromNPagedLookasideList(_L)
#define NdisFreeToNPagedLookasideList(_L, _E)       ExFreeToNPagedLookasideList(_L, _E)

#if NDIS_LEGACY_DRIVER
/*
NdisSetPacketStatus is deprecated. use NDIS_SET_PACKET_STATUS macro
*/
_IRQL_requires_max_(DISPATCH_LEVEL)
DECLSPEC_DEPRECATED_DDK
EXPORT
VOID
NdisSetPacketStatus(
    _In_          PNDIS_PACKET    Packet,
    _In_          NDIS_STATUS     Status,
    _In_          NDIS_HANDLE     Handle,
    _In_          ULONG           Code
    );

#endif

#define NDIS_MAX_EVENT_LOG_DATA_SIZE ((ERROR_LOG_MAXIMUM_SIZE - sizeof(IO_ERROR_LOG_PACKET) + sizeof(ULONG)) & ~3)

#if NDIS_SUPPORT_60_COMPATIBLE_API && !defined(NDIS_WRAPPER)

#ifdef _WIN64
#define NDIS_MAX_PROCESSOR_COUNT   64
#else
#define NDIS_MAX_PROCESSOR_COUNT   32
#endif

#endif

#if NDIS_SUPPORT_NDIS6

//
// NDIS_RESTART_ATTRIBUTES is used in NDIS_FILTER_RESTART_PARAMETERS,
// NDIS_MINIPORT_RESTART_PARAMETERS and NDIS_PROTOCOL_RESTART_PARAMETERS
//
typedef  struct _NDIS_RESTART_ATTRIBUTES NDIS_RESTART_ATTRIBUTES, *PNDIS_RESTART_ATTRIBUTES;

typedef  struct _NDIS_RESTART_ATTRIBUTES
{
    PNDIS_RESTART_ATTRIBUTES   Next;
    NDIS_OID                   Oid;
    ULONG                      DataLength;
    DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) UCHAR  Data[1];
}NDIS_RESTART_ATTRIBUTES, *PNDIS_RESTART_ATTRIBUTES;



//
// used in all NDIS drivers
//
typedef
_IRQL_requires_(PASSIVE_LEVEL)
_Function_class_(SET_OPTIONS)
NDIS_STATUS
(SET_OPTIONS)(
    _In_  NDIS_HANDLE             NdisDriverHandle,
    _In_  NDIS_HANDLE             DriverContext
    );

typedef SET_OPTIONS (*SET_OPTIONS_HANDLER);
typedef SET_OPTIONS (MINIPORT_SET_OPTIONS);
typedef SET_OPTIONS (PROTOCOL_SET_OPTIONS);
typedef SET_OPTIONS (FILTER_SET_OPTIONS);

#endif // NDIS_SUPPORT_NDIS6

#if NDIS_LEGACY_DRIVER
typedef
NDIS_STATUS
(*WAN_SEND_HANDLER)(
    _In_  NDIS_HANDLE             NdisBindingHandle,
    _In_  NDIS_HANDLE             LinkHandle,
    _In_  PVOID                   Packet
    );

typedef
NDIS_STATUS
(*SEND_HANDLER)(
    _In_  NDIS_HANDLE             NdisBindingHandle,
    _In_  PNDIS_PACKET            Packet
    );

typedef
NDIS_STATUS
(*TRANSFER_DATA_HANDLER)(
    _In_   NDIS_HANDLE             NdisBindingHandle,
    _In_   NDIS_HANDLE             MacReceiveContext,
    _In_   UINT                    ByteOffset,
    _In_   UINT                    BytesToTransfer,
    _Out_ PNDIS_PACKET            Packet,
    _Out_ PUINT                   BytesTransferred
    );

typedef
NDIS_STATUS
(*RESET_HANDLER)(
    _In_  NDIS_HANDLE             NdisBindingHandle
    );

typedef
NDIS_STATUS
(*REQUEST_HANDLER)(
    _In_  NDIS_HANDLE             NdisBindingHandle,
    _In_  PNDIS_REQUEST           NdisRequest
    );

typedef
VOID
(*SEND_PACKETS_HANDLER)(
    _In_ NDIS_HANDLE              MiniportAdapterContext,
    _In_ PPNDIS_PACKET            PacketArray,
    _In_ UINT                     NumberOfPackets
    );

#endif // NDIS_LEGACY_DRIVER

//
// NDIS object types created by NDIS drivers
//


#if NDIS_SUPPORT_NDIS6

#if NDIS_SUPPORT_60_COMPATIBLE_API
#define NDIS_CURRENT_PROCESSOR_NUMBER KeGetCurrentProcessorNumber()
#endif

#define NDIS_CURRENT_IRQL() KeGetCurrentIrql()

#define NDIS_RAISE_IRQL_TO_DISPATCH(_pIrql_)     KeRaiseIrql(DISPATCH_LEVEL, _pIrql_)

#define NDIS_LOWER_IRQL(_OldIrql_, _CurIrql_)                   \
{                                                               \
    if (_OldIrql_ != _CurIrql_) KeLowerIrql(_OldIrql_);         \
}

typedef KMUTEX      NDIS_MUTEX, *PNDIS_MUTEX;

#define NDIS_INIT_MUTEX(_M_)                KeInitializeMutex(_M_, 0xFFFF)
#define NDIS_RELEASE_MUTEX(_M_)             KeReleaseMutex(_M_, FALSE)

#define NDIS_WAIT_FOR_MUTEX(_M_)            KeWaitForSingleObject(_M_,      \
                                                                  Executive,\
                                                                  KernelMode,\
                                                                  FALSE,    \
                                                                  NULL)     \



#if NDIS_SUPPORT_60_COMPATIBLE_API

EXPORT
ULONG
NdisSystemActiveProcessorCount(
    PKAFFINITY ActiveProcessors
    );

#endif

#if NDIS_SUPPORT_NDIS620

EXPORT
USHORT
NdisActiveGroupCount(
    VOID
    );

EXPORT
USHORT
NdisMaxGroupCount(
    VOID
    );

EXPORT
ULONG
NdisGroupMaxProcessorCount(
    USHORT Group
    );

EXPORT
ULONG
NdisGroupActiveProcessorCount(
    USHORT Group
    );

EXPORT
KAFFINITY
NdisGroupActiveProcessorMask(
    USHORT Group
    );

EXPORT
PROCESSOR_NUMBER
NdisCurrentGroupAndProcessor(
    VOID
    );

#if NTDDI_VERSION >= NTDDI_WIN7
FORCEINLINE
ULONG
NdisCurrentProcessorIndex(
    VOID
    )
{
    return KeGetCurrentProcessorIndex();
}
#endif// NTDDI_VERSION >= NTDDI_WIN7

_IRQL_requires_max_(HIGH_LEVEL)
EXPORT
ULONG
NdisProcessorNumberToIndex(
    _In_ PROCESSOR_NUMBER ProcNum
    );

_IRQL_requires_max_(HIGH_LEVEL)
EXPORT    
NTSTATUS
NdisProcessorIndexToNumber(
    _In_  ULONG             ProcIndex,
    _Out_ PPROCESSOR_NUMBER ProcNum
    );

#endif

#define NDIS_CONFIGURATION_OBJECT_REVISION_1               1

//
// Flags for NdisOpenConfigurationEx
//
#define NDIS_CONFIG_FLAG_FILTER_INSTANCE_CONFIGURATION      0x00000001

typedef struct _NDIS_CONFIGURATION_OBJECT
{
    NDIS_OBJECT_HEADER          Header;
    NDIS_HANDLE                 NdisHandle;
    ULONG                       Flags;
} NDIS_CONFIGURATION_OBJECT, *PNDIS_CONFIGURATION_OBJECT;

#define NDIS_SIZEOF_CONFIGURATION_OBJECT_REVISION_1  \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_CONFIGURATION_OBJECT, Flags)

_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisOpenConfigurationEx(
    _In_      PNDIS_CONFIGURATION_OBJECT    ConfigObject,
    _Out_ _When_(return==NDIS_STATUS_SUCCESS, _At_(*ConfigurationHandle, __drv_allocatesMem(mem)))
              PNDIS_HANDLE                  ConfigurationHandle
    );

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
__drv_allocatesMem(Mem)
EXPORT
PVOID
NdisAllocateMemoryWithTagPriority(
    _In_  NDIS_HANDLE             NdisHandle,
    _In_  UINT                    Length,
    _In_  ULONG                   Tag,
    _In_  EX_POOL_PRIORITY        Priority
    );


typedef struct _NDIS_DRIVER_OPTIONAL_HANDLERS
{
    NDIS_OBJECT_HEADER  Header;
} NDIS_DRIVER_OPTIONAL_HANDLERS, *PNDIS_DRIVER_OPTIONAL_HANDLERS;

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT    
NDIS_STATUS
NdisSetOptionalHandlers(
    _In_  NDIS_HANDLE                     NdisHandle,
    _In_  PNDIS_DRIVER_OPTIONAL_HANDLERS  OptionalHandlers
    );

//
// Flags used in NET_PNP_EVENT_NOTIFICATION Flags field
//

#define NET_EVENT_HALT_MINIPORT_ON_LOW_POWER      0x00000001

#define NET_PNP_EVENT_NOTIFICATION_REVISION_1   1

typedef struct _NET_PNP_EVENT_NOTIFICATION
{
    //
    // Caller must set Header to
    //     Type = NDIS_OBJECT_TYPE_DEFAULT
    //     Revision = NET_PNP_EVENT_NOTIFICATION_REVISION_1
    //     Size = sizeof(_NET_PNP_EVENT_NOTIFICATION)
    //
    NDIS_OBJECT_HEADER     Header;

    NDIS_PORT_NUMBER       PortNumber;

    NET_PNP_EVENT          NetPnPEvent;
    ULONG                  Flags;


} NET_PNP_EVENT_NOTIFICATION, *PNET_PNP_EVENT_NOTIFICATION;

#define NDIS_SIZEOF_NET_PNP_EVENT_NOTIFICATION_REVISION_1   \
        RTL_SIZEOF_THROUGH_FIELD(NET_PNP_EVENT_NOTIFICATION, Flags)

//
// new request and status structures
//

#define  NDIS_OID_REQUEST_REVISION_1             1
#define  NDIS_OID_REQUEST_TIMEOUT_INFINITE       0
#define  NDIS_OID_REQUEST_NDIS_RESERVED_SIZE     16

typedef struct _NDIS_OID_REQUEST
{
    //
    // Caller must set Header to
    //     Header.Type = NDIS_OBJECT_TYPE_OID_REQUEST
    //     Header.Revision = NDIS_OID_REQUEST_REVISION_1
    //     Header.Size = NDIS_SIZEOF_OID_REQUEST_REVISION_1
    //
    NDIS_OBJECT_HEADER          Header;
    NDIS_REQUEST_TYPE           RequestType;
    NDIS_PORT_NUMBER            PortNumber;
    UINT                        Timeout; // in Seconds
    PVOID                       RequestId;
    NDIS_HANDLE                 RequestHandle;

    //
    // OID - Information
    //
    union _REQUEST_DATA
    {
        struct _QUERY
        {
            NDIS_OID    Oid;
            PVOID       InformationBuffer;
            UINT        InformationBufferLength;
            UINT        BytesWritten;
            UINT        BytesNeeded;
        } QUERY_INFORMATION;

        struct _SET
        {
            NDIS_OID    Oid;
            PVOID       InformationBuffer;
            UINT        InformationBufferLength;
            UINT        BytesRead;
            UINT        BytesNeeded;
        } SET_INFORMATION;

        struct _METHOD
        {
            NDIS_OID            Oid;
            PVOID               InformationBuffer;
            ULONG               InputBufferLength;
            ULONG               OutputBufferLength;
            ULONG               MethodId;
            UINT                BytesWritten;
            UINT                BytesRead;
            UINT                BytesNeeded;
        } METHOD_INFORMATION;
    } DATA;
    //
    // NDIS Reserved
    //
    UCHAR       NdisReserved[NDIS_OID_REQUEST_NDIS_RESERVED_SIZE * sizeof(PVOID)];
    UCHAR       MiniportReserved[2*sizeof(PVOID)];
    UCHAR       SourceReserved[2*sizeof(PVOID)];
    UCHAR       SupportedRevision;
    UCHAR       Reserved1;
    USHORT      Reserved2;

}NDIS_OID_REQUEST, *PNDIS_OID_REQUEST;

#define NDIS_SIZEOF_OID_REQUEST_REVISION_1     \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_OID_REQUEST, Reserved2)
//
// Macros to set, clear and test NDIS_STATUS_INDICATION Flags
//
#define NDIS_STATUS_INDICATION_SET_FLAG(_StatusIndication, _F)     \
    ((_StatusIndication)->Flags |= (_F))

#define NDIS_STATUS_INDICATION_TEST_FLAG(_StatusIndication, _F)    \
    (((_StatusIndication)->Flags & (_F)) != 0)

#define NDIS_STATUS_INDICATION_CLEAR_FLAG(_StatusIndication, _F)   \
    ((_StatusIndication)->Flags &= ~(_F))

#define  NDIS_STATUS_INDICATION_FLAGS_NDIS_RESERVED    0xFFF

//
// Public flags for NDIS_STATUS_INDICATION
//
#define  NDIS_STATUS_INDICATION_FLAGS_MEDIA_CONNECT_TO_CONNECT		0x1000

#define  NDIS_STATUS_INDICATION_REVISION_1             1

typedef struct _NDIS_STATUS_INDICATION
{
    NDIS_OBJECT_HEADER      Header;
    NDIS_HANDLE             SourceHandle;
    NDIS_PORT_NUMBER        PortNumber;
    NDIS_STATUS             StatusCode;
    ULONG                   Flags;
    NDIS_HANDLE             DestinationHandle;
    PVOID                   RequestId;
    PVOID                   StatusBuffer;
    ULONG                   StatusBufferSize;
    GUID                    Guid;               // optional and valid only if StatusCode = NDIS_STATUS_MEDIA_SPECIFIC_INDICATION
    PVOID                   NdisReserved[4];
}NDIS_STATUS_INDICATION, *PNDIS_STATUS_INDICATION;

#define NDIS_SIZEOF_STATUS_INDICATION_REVISION_1      \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_STATUS_INDICATION, NdisReserved)


//
// Generic Timer support
//

#define NDIS_TIMER_CHARACTERISTICS_REVISION_1 1

typedef struct _NDIS_TIMER_CHARACTERISTICS
{
    NDIS_OBJECT_HEADER                  Header;
    ULONG                               AllocationTag;
    PNDIS_TIMER_FUNCTION                TimerFunction;
    PVOID                               FunctionContext;
} NDIS_TIMER_CHARACTERISTICS, *PNDIS_TIMER_CHARACTERISTICS;

#define NDIS_SIZEOF_TIMER_CHARACTERISTICS_REVISION_1      \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_TIMER_CHARACTERISTICS, FunctionContext)

#if (NDIS_SUPPORT_NDIS620)
#define NDIS_MAX_LOOKAHEAD_SIZE_ACCESSED_UNDEFINED          -1

//
// Bits used in Flags field of NDIS_RESTART_GENERAL_ATTRIBUTES structure
//
#define NDIS_RESTART_GENERAL_ATTRIBUTES_MAX_LOOKAHEAD_ACCESSED_DEFINED  0x00000001
#endif

//
// NDIS_RESTART_GENERAL_ATTRIBUTES is used in NDIS_RESTART_ATTRIBUTES
//

#define NDIS_RESTART_GENERAL_ATTRIBUTES_REVISION_1            1

#if (NDIS_SUPPORT_NDIS620)
#define NDIS_RESTART_GENERAL_ATTRIBUTES_REVISION_2            2
#endif

typedef struct _NDIS_RESTART_GENERAL_ATTRIBUTES
{
    NDIS_OBJECT_HEADER              Header;
    ULONG                           MtuSize;
    ULONG64                         MaxXmitLinkSpeed;
    ULONG64                         MaxRcvLinkSpeed;
    ULONG                           LookaheadSize;
    ULONG                           MacOptions;
    ULONG                           SupportedPacketFilters;
    ULONG                           MaxMulticastListSize;
    PNDIS_RECEIVE_SCALE_CAPABILITIES RecvScaleCapabilities;
    NET_IF_ACCESS_TYPE              AccessType;
    ULONG                           Flags;
    NET_IF_CONNECTION_TYPE          ConnectionType;
    ULONG                           SupportedStatistics;
    ULONG                           DataBackFillSize;
    ULONG                           ContextBackFillSize;
    PNDIS_OID                       SupportedOidList;
    ULONG                           SupportedOidListLength;
#if (NDIS_SUPPORT_NDIS620)
    ULONG                           MaxLookaheadSizeAccessed;
#endif
}NDIS_RESTART_GENERAL_ATTRIBUTES, *PNDIS_RESTART_GENERAL_ATTRIBUTES;

#define NDIS_SIZEOF_RESTART_GENERAL_ATTRIBUTES_REVISION_1    \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_RESTART_GENERAL_ATTRIBUTES, SupportedOidListLength)

#if (NDIS_SUPPORT_NDIS620)
#define NDIS_SIZEOF_RESTART_GENERAL_ATTRIBUTES_REVISION_2    \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_RESTART_GENERAL_ATTRIBUTES, MaxLookaheadSizeAccessed)
#endif

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisAllocateTimerObject(
    _In_  NDIS_HANDLE                               NdisHandle,
    _In_  PNDIS_TIMER_CHARACTERISTICS               TimerCharacteristics,
    _Out_  _At_(*pTimerObject, _Post_ _Points_to_data_)
          PNDIS_HANDLE                              pTimerObject
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
BOOLEAN
NdisSetTimerObject(
    _In_      NDIS_HANDLE                TimerObject,
    _In_      LARGE_INTEGER              DueTime,
    _In_opt_  LONG                       MillisecondsPeriod,
    _In_opt_  PVOID                      FunctionContext
    );

//
// NdisCancelTimerObject must be called at PASSIVE_LEVEL if a nonzero value was
// specified in the MillisecondsPeriod parameter to NdisSetTimerObject.
//
// It is typically a bug not check the return value of NdisCancelTimerObject
// when the timer object is a one-shot timer.
//
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
BOOLEAN
NdisCancelTimerObject(
    _In_  NDIS_HANDLE               TimerObject
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFreeTimerObject(
    _In_  NDIS_HANDLE               TimerObject
    );

EXPORT
NDIS_STATUS
NdisAllocateCloneOidRequest(
    IN NDIS_HANDLE         SourceHandle,
    IN PNDIS_OID_REQUEST   OidRequest,
    IN UINT                PoolTag,
    OUT PNDIS_OID_REQUEST* ClonedOidRequest
    );

EXPORT
VOID
NdisFreeCloneOidRequest(
    IN NDIS_HANDLE         SourceHandle,
    IN PNDIS_OID_REQUEST   Request
    );

_IRQL_requires_max_(HIGH_LEVEL)
EXPORT    
VOID
NdisGetSystemUpTimeEx(
    _Out_ PLARGE_INTEGER                  pSystemUpTime
    );

#if NDIS_SUPPORT_60_COMPATIBLE_API

typedef struct _NDIS_PROCESSOR_INFO
{
    ULONG   CpuNumber;
    ULONG   PhysicalPackageId;
    ULONG   CoreId;
    ULONG   HyperThreadID;
}NDIS_PROCESSOR_INFO, *PNDIS_PROCESSOR_INFO;

#define NDIS_SYSTEM_PROCESSOR_INFO_REVISION_1 1

typedef struct _NDIS_SYSTEM_PROCESSOR_INFO
{
    NDIS_OBJECT_HEADER      Header;
    ULONG                   Flags;
    NDIS_PROCESSOR_VENDOR   ProcessorVendor;
    ULONG                   NumPhysicalPackages;
    ULONG                   NumCores;
    ULONG                   NumCoresPerPhysicalPackage;
    ULONG                   MaxHyperThreadingCpusPerCore;
    ULONG                   RssBaseCpu;
    ULONG                   RssCpuCount;
    _Out_writes_(MAXIMUM_PROC_PER_GROUP) PUCHAR RssProcessors;
    NDIS_PROCESSOR_INFO CpuInfo[MAXIMUM_PROC_PER_GROUP];
}NDIS_SYSTEM_PROCESSOR_INFO, *PNDIS_SYSTEM_PROCESSOR_INFO;

#define NDIS_SIZEOF_SYSTEM_PROCESSOR_INFO_REVISION_1    \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_SYSTEM_PROCESSOR_INFO, CpuInfo)

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisGetProcessorInformation(
    _Inout_ PNDIS_SYSTEM_PROCESSOR_INFO      SystemProcessorInfo
    );

#endif // NDIS_SUPPORT_60_COMPATIBLE_API

#if NDIS_SUPPORT_NDIS620

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisGetRssProcessorInformation(
    _In_                    NDIS_HANDLE                                    NdisHandle,
    _Out_writes_bytes_to_opt_(*Size, *Size) PNDIS_RSS_PROCESSOR_INFO       RssProcessorInfo,
    _Inout_                 PSIZE_T                                        Size
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT    
NDIS_STATUS
NdisGetProcessorInformationEx(
    _In_opt_                NDIS_HANDLE                                    NdisHandle,
    _Out_writes_bytes_to_opt_(*Size, *Size) PNDIS_SYSTEM_PROCESSOR_INFO_EX SystemProcessorInfo,
    _Inout_                 PSIZE_T                                        Size
    );

#endif


//
// Pause reason used in NDIS_MINIPORT_PAUSE_PARAMETERS, NDIS_FILTER_PAUSE_PARAMETERS
// and NDIS_PROTOCOL_PAUSE_PARAMETERS
//
#define NDIS_PAUSE_NDIS_INTERNAL                     0x00000001
#define NDIS_PAUSE_LOW_POWER                         0x00000002
#define NDIS_PAUSE_BIND_PROTOCOL                     0x00000004
#define NDIS_PAUSE_UNBIND_PROTOCOL                   0x00000008
#define NDIS_PAUSE_ATTACH_FILTER                     0x00000010
#define NDIS_PAUSE_DETACH_FILTER                     0x00000020
#define NDIS_PAUSE_FILTER_RESTART_STACK              0x00000040
#define NDIS_PAUSE_MINIPORT_DEVICE_REMOVE            0x00000080

#endif // NDIS_SUPPORT_NDIS6

#if  (NTDDI_VERSION >= NTDDI_VISTA)
#if (NDIS_LEGACY_DRIVER || NDIS_SUPPORT_NDIS6)
//
// Flags used in NDIS_FILTER_INTERFACE
//
#define NDIS_FILTER_INTERFACE_IM_FILTER        0x00000001
#define NDIS_FILTER_INTERFACE_LW_FILTER        0x00000002
#if (NDIS_SUPPORT_NDIS630)
#define NDIS_FILTER_INTERFACE_SEND_BYPASS      0x00000004
#define NDIS_FILTER_INTERFACE_RECEIVE_BYPASS   0x00000008
#endif // (NDIS_SUPPORT_NDIS630)



//
// NDIS_FILTER_INTERFACE is used in NDIS_ENUM_FILTERS
// structure that is used in NdisEnumerateFilterModules
//
#define NDIS_FILTER_INTERFACE_REVISION_1  1
#if (NDIS_SUPPORT_NDIS630)
#define NDIS_FILTER_INTERFACE_REVISION_2  2
#endif // (NDIS_SUPPORT_NDIS630)

typedef struct _NDIS_FILTER_INTERFACE
{

    NDIS_OBJECT_HEADER       Header;
    ULONG                    Flags;
    ULONG                    FilterType;
    ULONG                    FilterRunType;
    NET_IFINDEX              IfIndex;
    NET_LUID                 NetLuid;
    NDIS_STRING              FilterClass;
    NDIS_STRING              FilterInstanceName;
} NDIS_FILTER_INTERFACE, *PNDIS_FILTER_INTERFACE;

#define NDIS_SIZEOF_FILTER_INTERFACE_REVISION_1      \
    RTL_SIZEOF_THROUGH_FIELD(NDIS_FILTER_INTERFACE, FilterInstanceName)
#define NDIS_SIZEOF_FILTER_INTERFACE_REVISION_2      \
    NDIS_SIZEOF_FILTER_INTERFACE_REVISION_1

//
// NDIS_ENUM_FILTERS is used in NdisEnumerateFilterModules
//
#define NDIS_ENUM_FILTERS_REVISION_1           1

typedef struct _NDIS_ENUM_FILTERS
{
    NDIS_OBJECT_HEADER              Header;
    ULONG                           Flags;
    ULONG                           NumberOfFilters;
    ULONG                           OffsetFirstFilter;
    _Field_size_(NumberOfFilters)
    NDIS_FILTER_INTERFACE           Filter[1];
} NDIS_ENUM_FILTERS, *PNDIS_ENUM_FILTERS;

#define NDIS_SIZEOF_ENUM_FILTERS_REVISION_1      \
    RTL_SIZEOF_THROUGH_FIELD(NDIS_ENUM_FILTERS, Filter)

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisEnumerateFilterModules(
    _In_  NDIS_HANDLE         NdisHandle,
    _Out_writes_bytes_to_(InterfaceBufferLength, *BytesWritten)
          PVOID               InterfaceBuffer,
    _In_  ULONG               InterfaceBufferLength,
    _Out_ PULONG              BytesNeeded,
    _Out_ PULONG              BytesWritten
    );

//
// The NdisRegisterDeviceEx is used by NDIS 5 and 6 drivers on NTDDI_VISTA
// These drivers use NdisRegisterDeviceEx to use the new security features
// not available in NdisRegisterDevice or NdisMRegisterDevice
//
#define NDIS_DEVICE_OBJECT_ATTRIBUTES_REVISION_1    1

typedef struct _NDIS_DEVICE_OBJECT_ATTRIBUTES
{
    NDIS_OBJECT_HEADER          Header;
    PNDIS_STRING                DeviceName;
    PNDIS_STRING                SymbolicName;
    PDRIVER_DISPATCH*           MajorFunctions;
    ULONG                       ExtensionSize;
    PCUNICODE_STRING            DefaultSDDLString;
    LPCGUID                     DeviceClassGuid;
} NDIS_DEVICE_OBJECT_ATTRIBUTES, *PNDIS_DEVICE_OBJECT_ATTRIBUTES;

#define NDIS_SIZEOF_DEVICE_OBJECT_ATTRIBUTES_REVISION_1    \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_DEVICE_OBJECT_ATTRIBUTES, DeviceClassGuid)

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisRegisterDeviceEx(
    _In_  NDIS_HANDLE                     NdisHandle,
    _In_  PNDIS_DEVICE_OBJECT_ATTRIBUTES  DeviceObjectAttributes,
    _Out_ PDEVICE_OBJECT *                pDeviceObject,
    _Out_ PNDIS_HANDLE                    NdisDeviceHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT    
VOID
NdisDeregisterDeviceEx(
    _In_  NDIS_HANDLE                    NdisDeviceHandle
    );

_IRQL_requires_max_(HIGH_LEVEL)
EXPORT    
PVOID
NdisGetDeviceReservedExtension(
    _In_  PDEVICE_OBJECT                    DeviceObject
    );

//
// This structure is used by NDIS 5.x drivers that want to use
// NdisRegisterDeviceEx
//
#define NDIS_OBJECT_TYPE_DRIVER_WRAPPER_REVISION_1            1
typedef struct _NDIS_DRIVER_WRAPPER_HANDLE
{
    //
    // Caller must set Header to
    //     Header.Type = NDIS_OBJECT_TYPE_DRIVER_WRAPPER_OBJECT
    //     Header.Revision = NDIS_OBJECT_TYPE_DRIVER_WRAPPER_REVISION_1
    //     Header.Size = sizeof(NDIS_DRIVER_WRAPPER_HANDLE)
    //
    NDIS_OBJECT_HEADER          Header;
    NDIS_HANDLE                 NdisWrapperHandle;

} NDIS_DRIVER_WRAPPER_HANDLE, *PNDIS_DRIVER_WRAPPER_HANDLE;

#define NDIS_SIZEOF_DRIVER_WRAPPER_HANDLE_REVISION_1      \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_DRIVER_WRAPPER_HANDLE, NdisWrapperHandle)

#endif // NDIS_SUPPORT_NDIS6 || NDIS_LEGACY_DRIVER
#endif // NTDDI_VERSION >= NTDDI_VISTA

#if (NDIS_SUPPORT_NDIS620)

//
// shared memory usage types used in NDIS_SHARED_MEMORY_PARAMETERS
//

typedef enum _NDIS_SHARED_MEMORY_USAGE
{
    NdisSharedMemoryUsageUndefined,
    NdisSharedMemoryUsageXmit,
    NdisSharedMemoryUsageXmitHeader,
    NdisSharedMemoryUsageXmitData,
    NdisSharedMemoryUsageReceive,
    NdisSharedMemoryUsageReceiveLookahead,
    NdisSharedMemoryUsageReceivePostLookahead,
    NdisSharedMemoryUsageReceiveHeader,
    NdisSharedMemoryUsageReceiveData,
    NdisSharedMemoryUsageOther,
    NdisSharedMemoryUsageMax
}NDIS_SHARED_MEMORY_USAGE, *PNDIS_SHARED_MEMORY_USAGE;

//
// NDIS_SHARED_MEMORY_PARAMETERS structure describes
// a shared memory allocation request for a receive queue.
// This structure is used in NdisMAllocateSharedMemoryEx API
// as well as ALLOCATE_SHARED_MEMORY_HANDLER entry point
//

//
// flags used in Flags field of NDIS_SHARED_MEMORY_PARAMETERS structure
//
#define NDIS_SHARED_MEM_PARAMETERS_CONTIGUOUS                   0x00000001

// Legacy spelling error
#define NDIS_SHARED_MEM_PARAMETERS_CONTIGOUS  NDIS_SHARED_MEM_PARAMETERS_CONTIGUOUS


//
// shared memory handle used for allocation from HAL
//
#define NDIS_SHARED_MEM_HANDLE_HAL_ALLOCATED        ((NDIS_HANDLE)(NULL))

#define NDIS_SHARED_MEMORY_PARAMETERS_REVISION_1    1

#if (NDIS_SUPPORT_NDIS630)
#define NDIS_SHARED_MEMORY_PARAMETERS_REVISION_2    2
#endif // (NDIS_SUPPORT_NDIS630)

typedef struct _NDIS_SHARED_MEMORY_PARAMETERS
{
    _In_      NDIS_OBJECT_HEADER              Header;
    _In_      ULONG                           Flags;
    _In_      NDIS_RECEIVE_QUEUE_ID           QueueId;
    _Out_     NDIS_HANDLE                     SharedMemoryHandle;
    _In_      NODE_REQUIREMENT                PreferredNode;
    _In_      NDIS_SHARED_MEMORY_USAGE        Usage;
    _In_      ULONG                           Length;
    _Out_writes_bytes_(Length) PVOID                VirtualAddress;
    _In_      ULONG                           SGListBufferLength;
    _Out_writes_bytes_(SGListBufferLength) PSCATTER_GATHER_LIST SGListBuffer;
#if (NDIS_SUPPORT_NDIS630)
    _In_      NDIS_NIC_SWITCH_VPORT_ID        VPortId;
#endif // (NDIS_SUPPORT_NDIS630)
 }NDIS_SHARED_MEMORY_PARAMETERS, *PNDIS_SHARED_MEMORY_PARAMETERS;

#define NDIS_SIZEOF_SHARED_MEMORY_PARAMETERS_REVISION_1     \
    RTL_SIZEOF_THROUGH_FIELD(NDIS_SHARED_MEMORY_PARAMETERS, SGListBuffer)

#if (NDIS_SUPPORT_NDIS630)
#define NDIS_SIZEOF_SHARED_MEMORY_PARAMETERS_REVISION_2     \
    RTL_SIZEOF_THROUGH_FIELD(NDIS_SHARED_MEMORY_PARAMETERS, VPortId)
#endif // (NDIS_SUPPORT_NDIS630)
typedef
NDIS_STATUS
(*ALLOCATE_SHARED_MEMORY_HANDLER)(
    _In_      NDIS_HANDLE                     ProviderContext,
    _In_      PNDIS_SHARED_MEMORY_PARAMETERS  SharedMemoryParameters,
    _Inout_ PNDIS_HANDLE                 pSharedMemoryProviderContext
    );

typedef
VOID
(*FREE_SHARED_MEMORY_HANDLER)(
    _In_  NDIS_HANDLE                     ProviderContext,
    _In_  NDIS_HANDLE                     SharedMemoryProviderContext
    );

#define NDIS_SHARED_MEMORY_PROVIDER_CHAR_SUPPORTS_PF_VPORTS     0x00000001

#define NDIS_SHARED_MEMORY_PROVIDER_CHARACTERISTICS_REVISION_1     1

typedef struct _NDIS_SHARED_MEMORY_PROVIDER_CHARACTERISTICS
{
    NDIS_OBJECT_HEADER                      Header;     // Header.Type = NDIS_OBJECT_TYPE_SHARED_MEMORY_PROVIDER_CHARACTERISTICS
    ULONG                                   Flags;
    NDIS_HANDLE                             ProviderContext;
    ALLOCATE_SHARED_MEMORY_HANDLER          AllocateSharedMemoryHandler;
    FREE_SHARED_MEMORY_HANDLER              FreeSharedMemoryHandler;
} NDIS_SHARED_MEMORY_PROVIDER_CHARACTERISTICS, *PNDIS_SHARED_MEMORY_PROVIDER_CHARACTERISTICS;

#define NDIS_SIZEOF_SHARED_MEMORY_PROVIDER_CHARACTERISTICS_REVISION_1     \
    RTL_SIZEOF_THROUGH_FIELD(NDIS_SHARED_MEMORY_PROVIDER_CHARACTERISTICS, FreeSharedMemoryHandler)

EXPORT
VOID
NdisFreeMemoryWithTagPriority(
    _In_  NDIS_HANDLE             NdisHandle,
    _In_ __drv_freesMem(Mem) PVOID VirtualAddress,
    _In_  ULONG                   Tag
    );

//
// the following structure is used in NDIS_STATUS_RECEIVE_QUEUE_STATE
// status indication
//
#define NDIS_RECEIVE_QUEUE_STATE_REVISION_1       1
typedef struct _NDIS_RECEIVE_QUEUE_STATE
{
    NDIS_OBJECT_HEADER                      Header;
    ULONG                                   Flags;
    NDIS_RECEIVE_QUEUE_ID                   QueueId;
    NDIS_RECEIVE_QUEUE_OPERATIONAL_STATE    QueueState;
}NDIS_RECEIVE_QUEUE_STATE, *PNDIS_RECEIVE_QUEUE_STATE;

#define NDIS_SIZEOF_NDIS_RECEIVE_QUEUE_STATE_REVISION_1     \
    RTL_SIZEOF_THROUGH_FIELD(NDIS_RECEIVE_QUEUE_STATE, QueueState)

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisAllocateSharedMemory(
    _In_    NDIS_HANDLE                     NdisHandle,
    _Inout_ PNDIS_SHARED_MEMORY_PARAMETERS  SharedMemoryParameters,
    _Out_   PNDIS_HANDLE                    pAllocationHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisFreeSharedMemory(
    _In_  NDIS_HANDLE                     NdisHandle,
    _In_  NDIS_HANDLE                     AllocationHandle
    );

//
// NDIS_PROCESS_SG_LIST is a driver supplied callback routine used
// in NdisBuildScatterGatherList
//

typedef
VOID
(NDIS_PROCESS_SG_LIST)(
    _In_  PDEVICE_OBJECT          DeviceObject,
    _In_  PVOID                   Reserved,
    _In_  PSCATTER_GATHER_LIST    ScatterGatherListBuffer,
    _In_  PVOID                   Context
    );

typedef NDIS_PROCESS_SG_LIST (*NDIS_PROCESS_SG_LIST_HANDLER);


//
// NDIS_SCATTER_GATHER_LIST_PARAMETERS is used in NdisBuildScatterGatherList
// API call to build a scatter gather list for a buffer
//

#define NDIS_SCATTER_GATHER_LIST_PARAMETERS_REVISION_1  1

typedef struct _NDIS_SCATTER_GATHER_LIST_PARAMETERS
{
    NDIS_OBJECT_HEADER              Header;
    ULONG                           Flags;
    NDIS_RECEIVE_QUEUE_ID           QueueId;
    NDIS_SHARED_MEMORY_USAGE        SharedMemoryUsage;
    PMDL                            Mdl;
    PVOID                           CurrentVa;
    ULONG                           Length;
    NDIS_PROCESS_SG_LIST_HANDLER    ProcessSGListHandler;
    PVOID                           Context;
    PSCATTER_GATHER_LIST            ScatterGatherListBuffer;
    ULONG                           ScatterGatherListBufferSize;
    ULONG                           ScatterGatherListBufferSizeNeeded;
}NDIS_SCATTER_GATHER_LIST_PARAMETERS, *PNDIS_SCATTER_GATHER_LIST_PARAMETERS;
#define NDIS_SIZEOF_SCATTER_GATHER_LIST_PARAMETERS_REVISION_1    \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_SCATTER_GATHER_LIST_PARAMETERS, ScatterGatherListBufferSizeNeeded)

EXPORT
NDIS_STATUS
NdisBuildScatterGatherList(
    IN  NDIS_HANDLE                             NdisHandle,
    IN  PNDIS_SCATTER_GATHER_LIST_PARAMETERS    SGListParameters
    );

EXPORT
VOID
NdisFreeScatterGatherList(
    IN  NDIS_HANDLE                             NdisHandle,
    IN  PSCATTER_GATHER_LIST                    ScatterGatherListBuffer,
    IN  BOOLEAN                                 WriteToDevice
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
BOOLEAN
NdisSetCoalescableTimerObject(
    _In_      NDIS_HANDLE                TimerObject,
    _In_      LARGE_INTEGER              DueTime,
    _In_opt_  LONG                       MillisecondsPeriod,
    _In_opt_  PVOID                      FunctionContext,
    _In_opt_  ULONG                      Tolerance
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisGetHypervisorInfo(
    _Inout_     PNDIS_HYPERVISOR_INFO       HypervisorInfo
    );

#endif // #if (NDIS_SUPPORT_NDIS620)

#if (NDIS_SUPPORT_NDIS630)

#define NDIS_SRIOV_CONFIG_STATE_PARAMETERS_REVISION_1             1

typedef struct _NDIS_SRIOV_VF_CONFIG_STATE
{
    _In_    NDIS_OBJECT_HEADER          Header;
    _In_    NDIS_SRIOV_FUNCTION_ID      VFId;
    _In_    ULONG                       BlockId;
    _In_    ULONG                       Length;
} NDIS_SRIOV_VF_CONFIG_STATE, *PNDIS_SRIOV_VF_CONFIG_STATE;

#define NDIS_SIZEOF_SRIOV_CONFIG_STATE_REVISION_1      \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_SRIOV_CONFIG_STATE, Length)

//
//
// This structure is used in NDIS_STATUS_RECEIVE_FILTER_QUEUE_STATE_CHANGE
// status indication
//
#define NDIS_RECEIVE_FILTER_QUEUE_STATE_CHANGE_REVISION_1        1
typedef struct _NDIS_RECEIVE_FILTER_QUEUE_STATE_CHANGE
{
    NDIS_OBJECT_HEADER                      Header;
    ULONG                                   QueueId;
}NDIS_RECEIVE_FILTER_QUEUE_STATE_CHANGE, *PNDIS_RECEIVE_FILTER_QUEUE_STATE_CHANGE;

#define NDIS_SIZEOF_RECEIVE_FILTER_QUEUE_STATE_CHANGE_REVISION_1     \
    RTL_SIZEOF_THROUGH_FIELD(NDIS_RECEIVE_FILTER_QUEUE_STATE_CHANGE, QueueId)

//
// This structure is used in NDIS_STATUS_SWITCH_NIC_STATUS
// status indication
//
#define NDIS_SWITCH_NIC_STATUS_INDICATION_REVISION_1       1

typedef struct _NDIS_SWITCH_NIC_STATUS_INDICATION
{
    NDIS_OBJECT_HEADER                      Header;
    ULONG                                   Flags;
    NDIS_SWITCH_PORT_ID                     SourcePortId;
    NDIS_SWITCH_NIC_INDEX                   SourceNicIndex;
    NDIS_SWITCH_PORT_ID                     DestinationPortId;
    NDIS_SWITCH_NIC_INDEX                   DestinationNicIndex;
    PNDIS_STATUS_INDICATION                 StatusIndication;
}NDIS_SWITCH_NIC_STATUS_INDICATION, *PNDIS_SWITCH_NIC_STATUS_INDICATION;

#define NDIS_SIZEOF_SWITCH_NIC_STATUS_REVISION_1     \
    RTL_SIZEOF_THROUGH_FIELD(NDIS_SWITCH_NIC_STATUS_INDICATION, StatusIndication)


#endif // (NDIS_SUPPORT_NDIS630)

#ifndef AFFINITY_MASK
#define AFFINITY_MASK(n) ((KAFFINITY)1 << (n))
#endif


#pragma warning(pop)


//
//    Copyright (C) Microsoft.  All rights reserved.
//
#pragma once

#pragma warning(push)

#pragma warning(disable:4201) // (nonstandard extension used : nameless struct/union)
#pragma warning(disable:4214) // (extension used : bit field types other than int)

#if NDIS_SUPPORT_NDIS6

//
// NET_BUFFER data structures, APIs and macros
//

typedef struct _NET_BUFFER NET_BUFFER, *PNET_BUFFER;
typedef struct _NET_BUFFER_LIST_CONTEXT NET_BUFFER_LIST_CONTEXT, *PNET_BUFFER_LIST_CONTEXT;
typedef struct _NET_BUFFER_LIST NET_BUFFER_LIST, *PNET_BUFFER_LIST;

struct _SCATTER_GATHER_LIST;
typedef struct _SCATTER_GATHER_LIST SCATTER_GATHER_LIST, *PSCATTER_GATHER_LIST;

typedef union _NET_BUFFER_DATA_LENGTH
{
    ULONG   DataLength;
    SIZE_T  stDataLength;
} NET_BUFFER_DATA_LENGTH, *PNET_BUFFER_DATA_LENGTH;


typedef struct _NET_BUFFER_DATA
{
    PNET_BUFFER Next;
    PMDL        CurrentMdl;
    ULONG       CurrentMdlOffset;
#ifdef __cplusplus
    NET_BUFFER_DATA_LENGTH NbDataLength;
#else
    NET_BUFFER_DATA_LENGTH;
#endif
    PMDL        MdlChain;
    ULONG       DataOffset;
} NET_BUFFER_DATA, *PNET_BUFFER_DATA;

typedef union _NET_BUFFER_HEADER
{
#ifdef __cplusplus
    NET_BUFFER_DATA NetBufferData;
#else
    NET_BUFFER_DATA;
#endif
    SLIST_HEADER    Link;

} NET_BUFFER_HEADER, *PNET_BUFFER_HEADER;


#if (NDIS_SUPPORT_NDIS620)

//
// NET_BUFFER_SHARED_MEMORY is used to describe the
// shared memory segments used in each NET_BUFFER.
// for NDIS 6.20, they are used in VM queue capable NICs
// used in virtualization environment
//
typedef struct _NET_BUFFER_SHARED_MEMORY NET_BUFFER_SHARED_MEMORY, *PNET_BUFFER_SHARED_MEMORY;

typedef struct _NET_BUFFER_SHARED_MEMORY
{
    PNET_BUFFER_SHARED_MEMORY   NextSharedMemorySegment;
    ULONG                       SharedMemoryFlags;
    NDIS_HANDLE                 SharedMemoryHandle;
    ULONG                       SharedMemoryOffset;
    ULONG                       SharedMemoryLength;
}NET_BUFFER_SHARED_MEMORY, *PNET_BUFFER_SHARED_MEMORY;

#endif // NDIS_SUPPORT_NDIS620



typedef struct _NET_BUFFER
{
    union
    {
        struct
        {
            PNET_BUFFER Next;
            PMDL        CurrentMdl;
            ULONG       CurrentMdlOffset;
            union
            {
                ULONG   DataLength;
                SIZE_T  stDataLength;
            };

            PMDL        MdlChain;
            ULONG       DataOffset;
        };
        
        SLIST_HEADER    Link;

        // Duplicate of the above union, for source-compatibility
        NET_BUFFER_HEADER NetBufferHeader;
    };

    USHORT          ChecksumBias;
    USHORT          Reserved;
    NDIS_HANDLE     NdisPoolHandle;
    DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT)PVOID NdisReserved[2];
    DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT)PVOID ProtocolReserved[6];
    DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT)PVOID MiniportReserved[4];
    NDIS_PHYSICAL_ADDRESS   DataPhysicalAddress;
#if (NDIS_SUPPORT_NDIS620)
    union
    {
        PNET_BUFFER_SHARED_MEMORY   SharedMemoryInfo;
        PSCATTER_GATHER_LIST        ScatterGatherList;
    };
#endif
}NET_BUFFER, *PNET_BUFFER;

#pragma warning(push)
#pragma warning(disable:4200)   // nonstandard extension used : zero-sized array in struct/union

typedef struct _NET_BUFFER_LIST_CONTEXT
{
    PNET_BUFFER_LIST_CONTEXT    Next;
    USHORT                      Size;
    USHORT                      Offset;
    DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT)     UCHAR      ContextData[];
} NET_BUFFER_LIST_CONTEXT, *PNET_BUFFER_LIST_CONTEXT;

#pragma warning(pop)

typedef enum _NDIS_NET_BUFFER_LIST_INFO
{
    TcpIpChecksumNetBufferListInfo,
    TcpOffloadBytesTransferred = TcpIpChecksumNetBufferListInfo,
    IPsecOffloadV1NetBufferListInfo,
#if (NDIS_SUPPORT_NDIS61)
    IPsecOffloadV2NetBufferListInfo = IPsecOffloadV1NetBufferListInfo,
#endif // (NDIS_SUPPORT_NDIS61)
    TcpLargeSendNetBufferListInfo,
    TcpReceiveNoPush = TcpLargeSendNetBufferListInfo,
    ClassificationHandleNetBufferListInfo,
    Ieee8021QNetBufferListInfo,
    NetBufferListCancelId,
    MediaSpecificInformation,
    NetBufferListFrameType,
    NetBufferListProtocolId = NetBufferListFrameType,
    NetBufferListHashValue,
    NetBufferListHashInfo,
    WfpNetBufferListInfo,
#if (NDIS_SUPPORT_NDIS61)
    IPsecOffloadV2TunnelNetBufferListInfo,
    IPsecOffloadV2HeaderNetBufferListInfo,
#endif  // (NDIS_SUPPORT_NDIS61)

#if (NDIS_SUPPORT_NDIS620)
    NetBufferListCorrelationId,
    NetBufferListFilteringInfo,

    MediaSpecificInformationEx,
    NblOriginalInterfaceIfIndex,
    NblReAuthWfpFlowContext = NblOriginalInterfaceIfIndex,
    TcpReceiveBytesTransferred,

#if (NDIS_SUPPORT_NDIS630)

#if (_AMD64_)
    SwitchForwardingReserved,
    SwitchForwardingDetail,
    VirtualSubnetInfo,
#endif // (_AMD64_)

    IMReserved,
    TcpRecvSegCoalesceInfo,
    RscTcpTimestampDelta,
    TcpSendOffloadsSupplementalNetBufferListInfo = RscTcpTimestampDelta,

#endif // (NDIS_SUPPORT_NDIS630)

#endif // (NDIS_SUPPORT_NDIS620)

#if NDIS_WRAPPER == 1
    NetBufferListInfoReserved1,
    NetBufferListInfoReserved2,
#endif//NDIS_WRAPPER

    MaxNetBufferListInfo
} NDIS_NET_BUFFER_LIST_INFO, *PNDIS_NET_BUFFER_LIST_INFO;


typedef struct _NET_BUFFER_LIST_DATA
{
    PNET_BUFFER_LIST    Next;           // Next NetBufferList in the chain
    PNET_BUFFER         FirstNetBuffer; // First NetBuffer on this NetBufferList
} NET_BUFFER_LIST_DATA, *PNET_BUFFER_LIST_DATA;

typedef union _NET_BUFFER_LIST_HEADER
{
#ifdef __cplusplus
    NET_BUFFER_LIST_DATA NetBufferListData;
#else
    NET_BUFFER_LIST_DATA;
#endif
    SLIST_HEADER            Link;           // used in SLIST of free NetBuffers in the block
} NET_BUFFER_LIST_HEADER, *PNET_BUFFER_LIST_HEADER;


typedef struct _NET_BUFFER_LIST
{
    union
    {
        struct
        {
            PNET_BUFFER_LIST Next;           // Next NetBufferList in the chain
            PNET_BUFFER FirstNetBuffer; // First NetBuffer on this NetBufferList
        };
        
        SLIST_HEADER Link;           // used in SLIST of free NetBuffers in the block        

        // Duplicate of the above, for source-compatibility
        NET_BUFFER_LIST_HEADER NetBufferListHeader;
    };
    
    PNET_BUFFER_LIST_CONTEXT    Context;
    PNET_BUFFER_LIST            ParentNetBufferList;
    NDIS_HANDLE                 NdisPoolHandle;
    DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT)PVOID NdisReserved[2];
    DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT)PVOID ProtocolReserved[4];
    DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT)PVOID MiniportReserved[2];
    PVOID                       Scratch;
    NDIS_HANDLE                 SourceHandle;
    ULONG                       NblFlags;   // public flags
    LONG                        ChildRefCount;
    ULONG                       Flags;      // private flags used by NDIs, protocols, miniport, etc.

    union
    {
        NDIS_STATUS             Status;
        ULONG                   NdisReserved2;
    };

    PVOID                       NetBufferListInfo[MaxNetBufferListInfo];
} NET_BUFFER_LIST, *PNET_BUFFER_LIST;




#define NET_BUFFER_NEXT_NB(_NB)                     ((_NB)->Next)
#define NET_BUFFER_FIRST_MDL(_NB)                   ((_NB)->MdlChain)
#define NET_BUFFER_DATA_LENGTH(_NB)                 ((_NB)->DataLength)
#define NET_BUFFER_DATA_OFFSET(_NB)                 ((_NB)->DataOffset)
#define NET_BUFFER_CURRENT_MDL(_NB)                 ((_NB)->CurrentMdl)
#define NET_BUFFER_CURRENT_MDL_OFFSET(_NB)          ((_NB)->CurrentMdlOffset)

#define NET_BUFFER_PROTOCOL_RESERVED(_NB)           ((_NB)->ProtocolReserved)
#define NET_BUFFER_MINIPORT_RESERVED(_NB)           ((_NB)->MiniportReserved)
#define NET_BUFFER_CHECKSUM_BIAS(_NB)               ((_NB)->ChecksumBias)

#if (NDIS_SUPPORT_NDIS61)
#define NET_BUFFER_DATA_PHYSICAL_ADDRESS(_NB)       ((_NB)->DataPhysicalAddress)
#endif // (NDIS_SUPPORT_NDIS61)

#if (NDIS_SUPPORT_NDIS620)
#define NET_BUFFER_FIRST_SHARED_MEM_INFO(_NB)       ((_NB)->SharedMemoryInfo)
#define NET_BUFFER_SHARED_MEM_NEXT_SEGMENT(_SHI)    ((_SHI)->NextSharedMemorySegment)
#define NET_BUFFER_SHARED_MEM_FLAGS(_SHI)           ((_SHI)->SharedMemoryFlags)
#define NET_BUFFER_SHARED_MEM_HANDLE(_SHI)          ((_SHI)->SharedMemoryHandle)
#define NET_BUFFER_SHARED_MEM_OFFSET(_SHI)          ((_SHI)->SharedMemoryOffset)
#define NET_BUFFER_SHARED_MEM_LENGTH(_SHI)          ((_SHI)->SharedMemoryLength)

#define NET_BUFFER_SCATTER_GATHER_LIST(_NB)         ((_NB)->ScatterGatherList)

#endif // (NDIS_SUPPORT_NDIS620)


#define NET_BUFFER_LIST_NEXT_NBL(_NBL)              ((_NBL)->Next)
#define NET_BUFFER_LIST_FIRST_NB(_NBL)              ((_NBL)->FirstNetBuffer)

#define NET_BUFFER_LIST_FLAGS(_NBL)                 ((_NBL)->Flags)
#define NET_BUFFER_LIST_NBL_FLAGS(_NBL)             ((_NBL)->NblFlags)
#define NET_BUFFER_LIST_PROTOCOL_RESERVED(_NBL)     ((_NBL)->ProtocolReserved)
#define NET_BUFFER_LIST_MINIPORT_RESERVED(_NBL)     ((_NBL)->MiniportReserved)
#define NET_BUFFER_LIST_CONTEXT_DATA_START(_NBL)    ((PUCHAR)(((_NBL)->Context)+1)+(_NBL)->Context->Offset)
#define NET_BUFFER_LIST_CONTEXT_DATA_SIZE(_NBL)     (((_NBL)->Context)->Size)

#define NET_BUFFER_LIST_INFO(_NBL, _Id)             ((_NBL)->NetBufferListInfo[(_Id)])
#define NET_BUFFER_LIST_STATUS(_NBL)                ((_NBL)->Status)


#define NDIS_GET_NET_BUFFER_LIST_CANCEL_ID(_NBL)     (NET_BUFFER_LIST_INFO(_NBL, NetBufferListCancelId))
#define NDIS_SET_NET_BUFFER_LIST_CANCEL_ID(_NBL, _CancelId)            \
    NET_BUFFER_LIST_INFO(_NBL, NetBufferListCancelId) = _CancelId

#define NDIS_GET_NET_BUFFER_LIST_IM_RESERVED(_NBL)        (NET_BUFFER_LIST_INFO(_NBL, IMReserved))
#define NDIS_SET_NET_BUFFER_LIST_IM_RESERVED(_NBL, _Val)  NET_BUFFER_LIST_INFO(_NBL, IMReserved) = (_Val)

#define NDIS_GET_NET_BUFFER_LIST_VLAN_ID(_NBL)            (((NDIS_NET_BUFFER_LIST_8021Q_INFO *) &NET_BUFFER_LIST_INFO((_NBL), Ieee8021QNetBufferListInfo))->TagHeader.VlanId)
#define NDIS_SET_NET_BUFFER_LIST_VLAN_ID(_NBL, _VlanId)   ((NDIS_NET_BUFFER_LIST_8021Q_INFO *) &NET_BUFFER_LIST_INFO((_NBL), Ieee8021QNetBufferListInfo))->TagHeader.VlanId = (_VlanId)
//
//  Per-NBL information for Ieee8021QNetBufferListInfo.
//
typedef struct _NDIS_NET_BUFFER_LIST_8021Q_INFO
{
    union
    {
        struct
        {
            UINT32      UserPriority:3;         // 802.1p priority
            UINT32      CanonicalFormatId:1;    // always 0
            UINT32      VlanId:12;              // VLAN Identification
            UINT32      Reserved:16;            // set to 0 for ethernet
        }TagHeader;

        struct
        {
            UINT32      UserPriority:3;         // 802.1p priority
            UINT32      CanonicalFormatId:1;    // always 0
            UINT32      VlanId:12;              // VLAN Identification
            UINT32      WMMInfo:4;
            UINT32      Reserved:12;            // set to 0 for wireless lan

        }WLanTagHeader;

        PVOID  Value;
    };
} NDIS_NET_BUFFER_LIST_8021Q_INFO, *PNDIS_NET_BUFFER_LIST_8021Q_INFO;

typedef struct _NDIS_NET_BUFFER_LIST_MEDIA_SPECIFIC_INFO
{
    union
    {
        PVOID  MediaSpecificInfo;
        PVOID  NativeWifiSpecificInfo;

        PVOID  Value;
    };

} NDIS_NET_BUFFER_LIST_MEDIA_SPECIFIC_INFO, *PNDIS_NET_BUFFER_LIST_MEDIA_SPECIFIC_INFO;

typedef struct _NDIS_NBL_MEDIA_MEDIA_SPECIFIC_INFORMATION NDIS_NBL_MEDIA_SPECIFIC_INFORMATION, *PNDIS_NBL_MEDIA_SPECIFIC_INFORMATION;


struct _NDIS_NBL_MEDIA_MEDIA_SPECIFIC_INFORMATION
{
    PNDIS_NBL_MEDIA_SPECIFIC_INFORMATION NextEntry;
    ULONG                                Tag;
    DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) UCHAR  Data[1];
};


//
// Switch fields
//
#if (NDIS_SUPPORT_NDIS630)

//
// NDIS_SWITCH_PORT_ID and NDIS_SWITCH_NIC_INDEX are needed
// in user and kernel level headers without a common include,
// so it is defined in multiple places.
//
#ifndef _NDIS_SWITCH_PORT_ID
#define _NDIS_SWITCH_PORT_ID NDIS_SWITCH_PORT_ID
typedef UINT32 NDIS_SWITCH_PORT_ID, *PNDIS_SWITCH_PORT_ID;
typedef USHORT NDIS_SWITCH_NIC_INDEX, *PNDIS_SWITCH_NIC_INDEX;
#else
//
// If already defined, make sure sizes match.
//
C_ASSERT(sizeof(NDIS_SWITCH_PORT_ID) == sizeof(UINT32));
C_ASSERT(sizeof(NDIS_SWITCH_NIC_INDEX) == sizeof(USHORT));
#endif

#define NDIS_SWITCH_DEFAULT_PORT_ID     0
#define NDIS_SWITCH_DEFAULT_NIC_INDEX   0

#if (_AMD64_)

//
// Defines NBL forwarding information
//
typedef union _NDIS_SWITCH_FORWARDING_DETAIL_NET_BUFFER_LIST_INFO
{
    UINT64 AsUINT64;

    struct
    {
        //
        // The number of unused entries available for use by the forwarding
        // extension to define destinations for the NetBufferList. The
        // GrowNetBufferListDestinations API can be used to create more unused
        // entries.
        //
        UINT32                      NumAvailableDestinations:16;
        //
        // The originating switch port Id.
        //
        UINT32                      SourcePortId:16;
        //
        // The originating Nic index on the source port.
        //
        UINT32                      SourceNicIndex:8;
#if (NDIS_SUPPORT_NDIS640)
        //
        // The inbox switch must forward this packet if set to 1.
        // This flag MUST NOT be written to by any extension.
        //
        UINT32                      NativeForwardingRequired:1;
        //  
        // Reserved for internal use.  
        //  
        UINT32                      Reserved1:1;
#else // !(NDIS_SUPPORT_NDIS640)
        //  
        // Reserved for internal use.  
        //  
        UINT32                      Reserved1:2;
#endif // (NDIS_SUPPORT_NDIS640)
        //
        // If set, the packet data resides fully on trusted host memory. If not
        // set, refer to SafePacketDataSize for the amount of host memory in
        // use.
        //
        UINT32                      IsPacketDataSafe:1;
        //
        // If IsPacketDataSafe is not set, this field indicates how much of the
        // packet data is built from trusted host memory, and how much is from
        // shared untrusted memory. The unit is in byte increments from the
        // start of the packet.
        //
        UINT32                      SafePacketDataSize:12;
        //
        // Reserved for internal use.
        //
        UINT32                      Reserved2:9;
    };
} NDIS_SWITCH_FORWARDING_DETAIL_NET_BUFFER_LIST_INFO, *PNDIS_SWITCH_FORWARDING_DETAIL_NET_BUFFER_LIST_INFO;

//
// NET_BUFFER_LIST_INFO structures should fit within ULONG_PTR
//
C_ASSERT(sizeof(NDIS_SWITCH_FORWARDING_DETAIL_NET_BUFFER_LIST_INFO) == sizeof(UINT64));

//
// Macro for accessing NBL switch forwarding detail
//
#define NET_BUFFER_LIST_SWITCH_FORWARDING_DETAIL(_NBL)\
            ((PNDIS_SWITCH_FORWARDING_DETAIL_NET_BUFFER_LIST_INFO)\
            &(NET_BUFFER_LIST_INFO((_NBL), SwitchForwardingDetail)))

//
// Defines NBL Group Network information
//
typedef struct _NDIS_NET_BUFFER_LIST_VIRTUAL_SUBNET_INFO
{
    union
    {
        struct
        {
            //
            // The originating switch port ID for the NBL.
            //
            UINT32  VirtualSubnetId:24;
            UINT32  ReservedVsidBits:8;
            UINT32  Reserved;
        };
        PVOID   Value;
    };
} NDIS_NET_BUFFER_LIST_VIRTUAL_SUBNET_INFO, *PNDIS_NET_BUFFER_LIST_VIRTUAL_SUBNET_INFO;

//
// NET_BUFFER_LIST_INFO structures should fit within ULONG_PTR
//
C_ASSERT(sizeof(NDIS_NET_BUFFER_LIST_VIRTUAL_SUBNET_INFO) <= sizeof(UINT64));

#define NDIS_GET_NET_BUFFER_LIST_VIRTUAL_SUBNET_ID(_NBL)                   \
    ((*((NDIS_NET_BUFFER_LIST_VIRTUAL_SUBNET_INFO *)                       \
    &NET_BUFFER_LIST_INFO((_NBL), VirtualSubnetInfo))).VirtualSubnetId)
#define NDIS_SET_NET_BUFFER_LIST_VIRTUAL_SUBNET_ID(_NBL, _VirtualSubnetId) \
    ((*((NDIS_NET_BUFFER_LIST_VIRTUAL_SUBNET_INFO *)                       \
    &NET_BUFFER_LIST_INFO((_NBL), VirtualSubnetInfo))).VirtualSubnetId =   \
        (_VirtualSubnetId))

#endif // (_AMD64_)

//
// Defines the destination switch port for an NBL within the destination
// array.
//
typedef struct _NDIS_SWITCH_PORT_DESTINATION
{
    //
    // ID of the target switch port.
    //
    NDIS_SWITCH_PORT_ID         PortId;
    //
    // Destination Nic Index.
    //
    NDIS_SWITCH_NIC_INDEX       NicIndex;
    //
    // If set, the NBL will not be delivered to the port referenced in entry.
    //
    USHORT                      IsExcluded:1;
    //
    // If set, the VLAN information of the NBL will be preserved when delivered
    // to the destination.
    //
    USHORT                      PreserveVLAN:1;
    //
    // If set, the Priority information of the will be preserved when delivered
    // to the destination.
    //
    USHORT                      PreservePriority:1;
    //
    // Reserved for internal use.
    //
    USHORT                      Reserved:13;
} NDIS_SWITCH_PORT_DESTINATION, *PNDIS_SWITCH_PORT_DESTINATION;

//
// Defines one or more destination switch ports for an NBL.
//
typedef struct _NDIS_SWITCH_FORWARDING_DESTINATION_ARRAY
{
    NDIS_OBJECT_HEADER            Header;
    //
    // The size of each element in the array.
    //
    UINT32                        ElementSize;
    //
    // The number of total NDIS_SWITCH_DESTINATION_PORT elements in the array.
    //
    UINT32                        NumElements;
    //
    // The number of defined NDIS_SWITCH_DESTINATION_PORT elements in the
    // array.
    //
    UINT32                        NumDestinations;
    //
    // Pointer to the first NDIS_SWITCH_PORT_DESTINATION element in the
    // destination array.
    //
    PVOID                         FirstElement;
} NDIS_SWITCH_FORWARDING_DESTINATION_ARRAY,
    *PNDIS_SWITCH_FORWARDING_DESTINATION_ARRAY;

#define NDIS_SWITCH_FORWARDING_DESTINATION_ARRAY_REVISION_1      1

#define NDIS_SIZEOF_NDIS_SWITCH_FORWARDING_DESTINATION_ARRAY_REVISION_1 \
    RTL_SIZEOF_THROUGH_FIELD(NDIS_SWITCH_FORWARDING_DESTINATION_ARRAY, \
        FirstElement)

#define NDIS_SWITCH_PORT_DESTINATION_AT_ARRAY_INDEX(_DestArray_, _Index_)\
    ((PNDIS_SWITCH_PORT_DESTINATION) \
        (((PUCHAR)(_DestArray_)->FirstElement) + \
            ((_DestArray_)->ElementSize * (_Index_))))

#endif // (NDIS_SUPPORT_NDIS630)

#define NDIS_NBL_ADD_MEDIA_SPECIFIC_INFO(_NBL, _MediaSpecificInfo)                         \
    {                                                                                      \
        PNDIS_NBL_MEDIA_SPECIFIC_INFORMATION HeadEntry = NULL;                             \
        if (NET_BUFFER_LIST_INFO((_NBL), MediaSpecificInformation) != NULL)                \
        {                                                                                  \
            HeadEntry = (PNDIS_NBL_MEDIA_SPECIFIC_INFORMATION)(NET_BUFFER_LIST_INFO((_NBL), MediaSpecificInformation));           \
        }                                                                                  \
        NET_BUFFER_LIST_INFO((_NBL), MediaSpecificInformation) = (_MediaSpecificInfo);     \
        (_MediaSpecificInfo)->NextEntry = HeadEntry;                                       \
    }

#define NDIS_NBL_REMOVE_MEDIA_SPECIFIC_INFO(_NBL, _MediaSpecificInfo)                      \
    {                                                                                      \
        PNDIS_NBL_MEDIA_SPECIFIC_INFORMATION *HeadEntry;                                   \
        HeadEntry = (PNDIS_NBL_MEDIA_SPECIFIC_INFORMATION *)&(NET_BUFFER_LIST_INFO((_NBL), MediaSpecificInformation));             \
        for (; *HeadEntry != NULL; HeadEntry = &(*HeadEntry)->NextEntry)                   \
        {                                                                                  \
            if ((*HeadEntry)->Tag == (_MediaSpecificInfo)->Tag)                            \
            {                                                                              \
                *HeadEntry = (*HeadEntry)->NextEntry;                                      \
                break;                                                                     \
            }                                                                              \
        }                                                                                  \
    }

#define NDIS_NBL_GET_MEDIA_SPECIFIC_INFO(_NBL, _Tag, _MediaSpecificInfo)                   \
    {                                                                                      \
        PNDIS_NBL_MEDIA_SPECIFIC_INFORMATION HeadEntry;                                    \
        (_MediaSpecificInfo) = NULL;                                                       \
        HeadEntry = (PNDIS_NBL_MEDIA_SPECIFIC_INFORMATION)(NET_BUFFER_LIST_INFO((_NBL), MediaSpecificInformation));                \
        for (; HeadEntry != NULL; HeadEntry = HeadEntry->NextEntry)                        \
        {                                                                                  \
            if (HeadEntry->Tag == (_Tag))                                                  \
            {                                                                              \
                (_MediaSpecificInfo) = HeadEntry;                                          \
                break;                                                                     \
            }                                                                              \
        }                                                                                  \
    }

#if NDIS_SUPPORT_NDIS620

typedef struct _NDIS_NBL_MEDIA_SPECIFIC_INFORMATION_EX
{
    NDIS_OBJECT_HEADER                   Header;
    struct _NDIS_NBL_MEDIA_SPECIFIC_INFORMATION_EX* NextEntry;
    ULONG                                Tag;
    PVOID                                Data;
} NDIS_NBL_MEDIA_SPECIFIC_INFORMATION_EX, *PNDIS_NBL_MEDIA_SPECIFIC_INFORMATION_EX;

#define NDIS_NBL_MEDIA_SPECIFIC_INFO_REVISION_1                 1

#define NDIS_SIZEOF_NBL_MEDIA_SPECIFIC_INFO_REVISION_1                                     \
            RTL_SIZEOF_THROUGH_FIELD(NDIS_NBL_MEDIA_SPECIFIC_INFORMATION_EX,Data)

#define NDIS_NBL_ADD_MEDIA_SPECIFIC_INFO_EX(_NBL, _MediaSpecificInfo)                      \
    {                                                                                      \
        PNDIS_NBL_MEDIA_SPECIFIC_INFORMATION_EX HeadEntry;                                 \
                                                                                           \
        HeadEntry = NET_BUFFER_LIST_INFO(_NBL, MediaSpecificInformationEx);                \
        NET_BUFFER_LIST_INFO((_NBL), MediaSpecificInformationEx) = (_MediaSpecificInfo);   \
        (_MediaSpecificInfo)->NextEntry = HeadEntry;                                       \
    }

#define NDIS_NBL_REMOVE_MEDIA_SPECIFIC_INFO_EX(_NBL, _MediaSpecificInfo)                   \
    {                                                                                      \
        PNDIS_NBL_MEDIA_SPECIFIC_INFORMATION_EX *HeadEntry;                                \
        HeadEntry = (PNDIS_NBL_MEDIA_SPECIFIC_INFORMATION_EX *)&(NET_BUFFER_LIST_INFO((_NBL), MediaSpecificInformationEx)); \
        for (; *HeadEntry != NULL; HeadEntry = &(*HeadEntry)->NextEntry)                   \
        {                                                                                  \
            if ((*HeadEntry)->Tag == (_MediaSpecificInfo)->Tag)                            \
            {                                                                              \
                *HeadEntry = (*HeadEntry)->NextEntry;                                      \
                break;                                                                     \
            }                                                                              \
        }                                                                                  \
    }

#define NDIS_NBL_GET_MEDIA_SPECIFIC_INFO_EX(_NBL, _Tag, _MediaSpecificInfo)                \
    {                                                                                      \
        PNDIS_NBL_MEDIA_SPECIFIC_INFORMATION_EX HeadEntry;                                 \
        (_MediaSpecificInfo) = NULL;                                                       \
        HeadEntry = (PNDIS_NBL_MEDIA_SPECIFIC_INFORMATION_EX)(NET_BUFFER_LIST_INFO((_NBL), MediaSpecificInformationEx)); \
        for (; HeadEntry != NULL; HeadEntry = HeadEntry->NextEntry)                        \
        {                                                                                  \
            if (HeadEntry->Tag == (_Tag))                                                  \
            {                                                                              \
                (_MediaSpecificInfo) = HeadEntry;                                          \
                break;                                                                     \
            }                                                                              \
        }                                                                                  \
    }

#endif


/*
Bit  31 - 0 for MS tag
          1 for Vendor tag
Bits 30-16 - Vendor ID (if Bit 31 = 1)
           - Technology ID (if Bit 31 = 0)
Bits 15-0 - Tag ID

*/

//
// Microsoft Media Specific-Info tags
//
//TUNNEL - Technology ID : 1
#define NDIS_MEDIA_SPECIFIC_INFO_TUNDL                      0x00010001
//
// Intel Media Specific Info tags
//
#define NDIS_MEDIA_SPECIFIC_INFO_FCOE                       0x80010000
#define NDIS_MEDIA_SPECIFIC_INFO_EAPOL                      0x80010001
#define NDIS_MEDIA_SPECIFIC_INFO_LLDP                       0x80010002
#define NDIS_MEDIA_SPECIFIC_INFO_TIMESYNC                   0x80010003


#ifndef NDIS_HASH_FUNCTION_MASK
#define NDIS_HASH_FUNCTION_MASK                             0x000000FF
#define NDIS_HASH_TYPE_MASK                                 0x00FFFF00
#endif

//
// The following macros are used by miniport driver and protocol driver to set and get
// the hash value, hash type and hash function.
//
VOID
FORCEINLINE
NET_BUFFER_LIST_SET_HASH_TYPE(
    _In_ PNET_BUFFER_LIST _NBL,
    _In_ volatile ULONG _HashType
    )
{
    (NET_BUFFER_LIST_INFO(_NBL, NetBufferListHashInfo)  =
    UlongToPtr(((PtrToUlong (NET_BUFFER_LIST_INFO(_NBL, NetBufferListHashInfo)) & (~NDIS_HASH_TYPE_MASK)) | ((_HashType) & (NDIS_HASH_TYPE_MASK)))));
}

VOID
FORCEINLINE
NET_BUFFER_LIST_SET_HASH_FUNCTION(
    _In_ PNET_BUFFER_LIST _NBL,
    _In_ volatile ULONG _HashFunction
    )
{
    (NET_BUFFER_LIST_INFO(_NBL, NetBufferListHashInfo)  =
    UlongToPtr(((PtrToUlong(NET_BUFFER_LIST_INFO(_NBL, NetBufferListHashInfo)) & (~NDIS_HASH_FUNCTION_MASK)) | ((_HashFunction) & (NDIS_HASH_FUNCTION_MASK)))));
}

#define NET_BUFFER_LIST_SET_HASH_VALUE(_NBL, _HashValue)         \
    (NET_BUFFER_LIST_INFO(_NBL, NetBufferListHashValue) = UlongToPtr(_HashValue))

#define NET_BUFFER_LIST_GET_HASH_TYPE(_NBL)                      \
    (PtrToUlong(NET_BUFFER_LIST_INFO(_NBL, NetBufferListHashInfo)) & (NDIS_HASH_TYPE_MASK))

#define NET_BUFFER_LIST_GET_HASH_FUNCTION(_NBL)                  \
    (PtrToUlong(NET_BUFFER_LIST_INFO(_NBL, NetBufferListHashInfo)) & (NDIS_HASH_FUNCTION_MASK))

#define NET_BUFFER_LIST_GET_HASH_VALUE(_NBL)                     \
    (PtrToUlong(NET_BUFFER_LIST_INFO(_NBL, NetBufferListHashValue)))

#define NdisSetNetBufferListProtocolId(_NBL,_ProtocolId)   \
    *((PUCHAR)(&NET_BUFFER_LIST_INFO(_NBL, NetBufferListProtocolId))) = _ProtocolId


#if (NDIS_SUPPORT_NDIS620)

//
//  Per-NBL information for NetBufferListFilteringInfo.
//
typedef struct _NDIS_NET_BUFFER_LIST_FILTERING_INFO
{
    union
    {
        struct
        {
            USHORT      FilterId;
#if (NDIS_SUPPORT_NDIS630)
            union
            {
#endif
                USHORT      QueueId;
#if (NDIS_SUPPORT_NDIS630)
                USHORT      VPortId;
            } QueueVPortInfo;
#endif
        } FilteringInfo;
        PVOID  Value;
    };
} NDIS_NET_BUFFER_LIST_FILTERING_INFO, *PNDIS_NET_BUFFER_LIST_FILTERING_INFO;

#define NET_BUFFER_LIST_RECEIVE_FILTER_ID(_NBL)                     \
    (((PNDIS_NET_BUFFER_LIST_FILTERING_INFO)&NET_BUFFER_LIST_INFO(_NBL, NetBufferListFilteringInfo))->FilteringInfo.FilterId)

#if (NDIS_SUPPORT_NDIS630)
#define NET_BUFFER_LIST_RECEIVE_QUEUE_ID(_NBL)                     \
    (((PNDIS_NET_BUFFER_LIST_FILTERING_INFO)&NET_BUFFER_LIST_INFO(_NBL, NetBufferListFilteringInfo))->FilteringInfo.QueueVPortInfo.QueueId)

#define NET_BUFFER_LIST_RECEIVE_FILTER_VPORT_ID(_NBL)                     \
    (((PNDIS_NET_BUFFER_LIST_FILTERING_INFO)&NET_BUFFER_LIST_INFO(_NBL, NetBufferListFilteringInfo))->FilteringInfo.QueueVPortInfo.VPortId)
#else
#define NET_BUFFER_LIST_RECEIVE_QUEUE_ID(_NBL)                     \
    (((PNDIS_NET_BUFFER_LIST_FILTERING_INFO)&NET_BUFFER_LIST_INFO(_NBL, NetBufferListFilteringInfo))->FilteringInfo.QueueId)
#endif

#if (NDIS_SUPPORT_NDIS630)

//
// TcpRecvSegCoalesceInfo
//
typedef union _NDIS_RSC_NBL_INFO
{
    struct
    {
        USHORT CoalescedSegCount;
        USHORT DupAckCount;
    } Info;
    PVOID Value;
} NDIS_RSC_NBL_INFO, *PNDIS_RSC_NBL_INFO;

C_ASSERT(sizeof(NDIS_RSC_NBL_INFO)==sizeof(PVOID));

#define NET_BUFFER_LIST_COALESCED_SEG_COUNT(_NBL)        \
    (( (PNDIS_RSC_NBL_INFO)&NET_BUFFER_LIST_INFO((_NBL), TcpRecvSegCoalesceInfo))->Info.CoalescedSegCount)

#define NET_BUFFER_LIST_DUP_ACK_COUNT(_NBL)        \
    (( (PNDIS_RSC_NBL_INFO)&NET_BUFFER_LIST_INFO((_NBL), TcpRecvSegCoalesceInfo))->Info.DupAckCount)


#endif // (NDIS_SUPPORT_NDIS630)

#endif // (NDIS_SUPPORT_NDIS620)


#define NdisQueryMdl(_Mdl, _VirtualAddress, _Length, _Priority)             \
{                                                                           \
    if (ARGUMENT_PRESENT(_VirtualAddress))                                  \
    {                                                                       \
        *(PVOID *)(_VirtualAddress) = MmGetSystemAddressForMdlSafe(_Mdl, _Priority); \
    }                                                                       \
    *(_Length) = MmGetMdlByteCount(_Mdl);                                   \
}

#define NdisQueryMdlOffset(_Mdl, _Offset, _Length)                          \
{                                                                           \
    *(_Offset) = MmGetMdlByteOffset(_Mdl);                                  \
    *(_Length) = MmGetMdlByteCount(_Mdl);                                   \
}

#define NDIS_MDL_TO_SPAN_PAGES(_Mdl)                                        \
    (MmGetMdlByteCount(_Mdl)==0 ?                                           \
                1 :                                                         \
                (ADDRESS_AND_SIZE_TO_SPAN_PAGES(                            \
                        MmGetMdlVirtualAddress(_Mdl),                       \
                        MmGetMdlByteCount(_Mdl))))

#define NdisGetMdlPhysicalArraySize(_Mdl, _ArraySize)                       \
    (*(_ArraySize) = NDIS_MDL_TO_SPAN_PAGES(_Mdl))


#define NDIS_MDL_LINKAGE(_Mdl) ((_Mdl)->Next)

#define NdisGetNextMdl(_CurrentMdl, _NextMdl)                               \
{                                                                           \
    *(_NextMdl) = (_CurrentMdl)->Next;                                      \
}


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
UCHAR
NdisGetNetBufferListProtocolId(
    _In_ PNET_BUFFER_LIST NetBufferList
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisAdjustNetBufferCurrentMdl(
    _In_  PNET_BUFFER NetBuffer
    );


//
// The flags that can be set at NET_BUFFER_LIST.Flags are defined below.
// Note that, on Vista and Win7, bits 0x3 were reserved by NDIS.  However,
// starting with Win8, these bits are free for protocol use.
//
#if (NDIS_SUPPORT_NDIS630)
#define NBL_FLAGS_PROTOCOL_RESERVED                 0xFFF00003
#define NBL_FLAGS_MINIPORT_RESERVED                 0x0000F000
#define NBL_FLAGS_SCRATCH                           0x000F0000
#define NBL_FLAGS_NDIS_RESERVED                     0x00000FFC
#else // <NDIS_SUPPORT_NDIS630
#define NBL_FLAGS_PROTOCOL_RESERVED                 0xFFF00000
#define NBL_FLAGS_MINIPORT_RESERVED                 0x0000F000
#define NBL_FLAGS_SCRATCH                           0x000F0000
#define NBL_FLAGS_NDIS_RESERVED                     0x00000FFF
#endif // NDIS_SUPPORT_NDIS630

#define NBL_TEST_FLAG(_NBL, _F)                     (((_NBL)->Flags & (_F)) != 0)
#define NBL_SET_FLAG(_NBL, _F)                      ((_NBL)->Flags |= (_F))
#define NBL_CLEAR_FLAG(_NBL, _F)                    ((_NBL)->Flags &= ~(_F))

#define NBL_SET_PROTOCOL_RSVD_FLAG(_NBL, _F)        ((_NBL)->Flags |= ((_F) & NBL_FLAGS_PROTOCOL_RESERVED))
#define NBL_CLEAR_PROTOCOL_RSVD_FLAG(_NBL, _F)      ((_NBL)->Flags &= ~((_F) & NBL_FLAGS_PROTOCOL_RESERVED))
#define NBL_TEST_PROTOCOL_RSVD_FLAG(_NBL, _F)       (((_NBL)->Flags & ((_F) & NBL_FLAGS_PROTOCOL_RESERVED)) != 0)


//
// Define some flags for protocols' own use
//
#define NBL_PROT_RSVD_FLAGS                     NBL_FLAGS_PROTOCOL_RESERVED
#define NBL_SET_PROT_RSVD_FLAG(_NBL, _F)        NBL_SET_PROTOCOL_RSVD_FLAG(_NBL,_F)
#define NBL_CLEAR_PROT_RSVD_FLAG(_NBL, _F)      NBL_CLEAR_PROTOCOL_RSVD_FLAG(_NBL, _F)
#define NBL_TEST_PROT_RSVD_FLAG(_NBL, _F)       NBL_TEST_PROTOCOL_RSVD_FLAG(_NBL, _F)


//
// Flags used in NBL->NblFlags
// 0x01 to 0xFF are reserved for use by NDIS
//
#define NDIS_NBL_FLAGS_SEND_READ_ONLY                   0x00000001
#define NDIS_NBL_FLAGS_RECV_READ_ONLY                   0x00000002

#if (NDIS_SUPPORT_NDIS61)
#define NDIS_NBL_FLAGS_HD_SPLIT                         0x00000100  // Data and header are split
#define NDIS_NBL_FLAGS_IS_IPV4                          0x00000200  // Packet is an IPv4 packet
#define NDIS_NBL_FLAGS_IS_IPV6                          0x00000400  // Packet is an IPv6 packet
#define NDIS_NBL_FLAGS_IS_TCP                           0x00000800  // Packet is a TCP packet
#define NDIS_NBL_FLAGS_IS_UDP                           0x00001000  // Packet is a UDP packet
#define NDIS_NBL_FLAGS_SPLIT_AT_UPPER_LAYER_PROTOCOL_HEADER              0x00002000  // Packet is split at the beginning of upper layer protocol header
#define NDIS_NBL_FLAGS_SPLIT_AT_UPPER_LAYER_PROTOCOL_PAYLOAD             0x00004000  // Packet is split at the beginning of upper layer protocol data (TCP or UDP)
#endif // (NDIS_SUPPORT_NDIS61)

#define NDIS_NBL_FLAGS_IS_LOOPBACK_PACKET               0x00008000  // The NBL is loopback NBL.

#define NdisTestNblFlag(_NBL, _F)                   (((_NBL)->NblFlags & (_F)) != 0)
#define NdisTestNblFlags(_NBL, _F)                  (((_NBL)->NblFlags & (_F)) == (_F))
#define NdisSetNblFlag(_NBL, _F)                    ((_NBL)->NblFlags |= (_F))
#define NdisClearNblFlag(_NBL, _F)                  ((_NBL)->NblFlags &= ~(_F))



#define NET_BUFFER_LIST_POOL_PARAMETERS_REVISION_1 1

typedef struct _NET_BUFFER_LIST_POOL_PARAMETERS
{
    //
    // Set ObjectHeader.Type to NDIS_OBJECT_TYPE_DEFAULT
    //
    NDIS_OBJECT_HEADER      Header;
    UCHAR                   ProtocolId;
    BOOLEAN                 fAllocateNetBuffer;
    USHORT                  ContextSize;
    ULONG                   PoolTag;
    ULONG                   DataSize;
}NET_BUFFER_LIST_POOL_PARAMETERS, *PNET_BUFFER_LIST_POOL_PARAMETERS;

#define NDIS_SIZEOF_NET_BUFFER_LIST_POOL_PARAMETERS_REVISION_1   \
        RTL_SIZEOF_THROUGH_FIELD(NET_BUFFER_LIST_POOL_PARAMETERS, DataSize)

#define NET_BUFFER_POOL_PARAMETERS_REVISION_1 1

typedef struct _NET_BUFFER_POOL_PARAMETERS
{
    //
    // Set ObjectHeader.Type to NDIS_OBJECT_TYPE_DEFAULT
    //
    NDIS_OBJECT_HEADER      Header;
    ULONG                   PoolTag;
    ULONG                   DataSize;
}NET_BUFFER_POOL_PARAMETERS,*PNET_BUFFER_POOL_PARAMETERS;

#define NDIS_SIZEOF_NET_BUFFER_POOL_PARAMETERS_REVISION_1   \
        RTL_SIZEOF_THROUGH_FIELD(NET_BUFFER_POOL_PARAMETERS, DataSize);

//
// Prototypes of the MDL allocation and free callback.
//

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
PMDL
(*NET_BUFFER_ALLOCATE_MDL_HANDLER)(
    _Inout_ PULONG               BufferSize
    );

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
VOID
(*NET_BUFFER_FREE_MDL_HANDLER)(
    _In_  PMDL                    Mdl
    );

_Must_inspect_result_
__drv_allocatesMem(mem)
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_HANDLE
NdisAllocateNetBufferPool(
    _In_opt_ NDIS_HANDLE                  NdisHandle,
    _In_     PNET_BUFFER_POOL_PARAMETERS  Parameters
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFreeNetBufferPool(
    _In_ __drv_freesMem(mem) NDIS_HANDLE             PoolHandle
    );

_Must_inspect_result_
__drv_allocatesMem(mem)
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
PNET_BUFFER
NdisAllocateNetBuffer(
    _In_      NDIS_HANDLE             PoolHandle,
    _In_opt_  PMDL                    MdlChain,
    _In_      ULONG                   DataOffset,
    _In_      SIZE_T                  DataLength
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFreeNetBuffer(
    _In_ __drv_freesMem(mem) PNET_BUFFER             NetBuffer
    );


_Must_inspect_result_
__drv_allocatesMem(mem)
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
PNET_BUFFER
NdisAllocateNetBufferMdlAndData(
    _In_  NDIS_HANDLE              PoolHandle
    );


_Must_inspect_result_
__drv_allocatesMem(mem)
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_HANDLE
NdisAllocateNetBufferListPool(
    _In_opt_  NDIS_HANDLE                         NdisHandle,
    _In_      PNET_BUFFER_LIST_POOL_PARAMETERS    Parameters
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFreeNetBufferListPool(
    _In_  __drv_freesMem(mem) NDIS_HANDLE             PoolHandle
    );

_Must_inspect_result_
__drv_allocatesMem(mem)
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
PNET_BUFFER_LIST
NdisAllocateNetBufferList(
    _In_  NDIS_HANDLE             PoolHandle,
    _In_  USHORT                  ContextSize,
    _In_  USHORT                  ContextBackFill
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFreeNetBufferList(
    _In_  __drv_freesMem(mem) PNET_BUFFER_LIST    NetBufferList
    );

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisRetreatNetBufferDataStart(
    _In_      PNET_BUFFER                      NetBuffer,
    _In_      ULONG                            DataOffsetDelta,
    _In_      ULONG                            DataBackFill,
    _In_opt_  NET_BUFFER_ALLOCATE_MDL_HANDLER  AllocateMdlHandler
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisAdvanceNetBufferDataStart(
    _In_      PNET_BUFFER                  NetBuffer,
    _In_      ULONG                        DataOffsetDelta,
    _In_      BOOLEAN                      FreeMdl,
    _In_opt_  NET_BUFFER_FREE_MDL_HANDLER  FreeMdlHandler
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisRetreatNetBufferListDataStart(
    _In_      PNET_BUFFER_LIST                 NetBufferList,
    _In_      ULONG                            DataOffsetDelta,
    _In_      ULONG                            DataBackFill,
    _In_opt_  NET_BUFFER_ALLOCATE_MDL_HANDLER  AllocateMdlHandler,
    _In_opt_  NET_BUFFER_FREE_MDL_HANDLER      FreeMdlHandler
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisAdvanceNetBufferListDataStart(
    _In_      PNET_BUFFER_LIST              NetBufferList,
    _In_      ULONG                         DataOffsetDelta,
    _In_      BOOLEAN                       FreeMdl,
    _In_opt_  NET_BUFFER_FREE_MDL_HANDLER   FreeMdlMdlHandler
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
_When_(return==0,_At_(NetBufferList->Context, __drv_allocatesMem(mem)))
EXPORT
NDIS_STATUS
NdisAllocateNetBufferListContext(
    _In_  PNET_BUFFER_LIST        NetBufferList,
    _In_  USHORT                  ContextSize,
    _In_  USHORT                  ContextBackFill,
    _In_  ULONG                   PoolTag
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
_At_(NetBufferList->Context, __drv_freesMem(mem))
EXPORT
VOID
NdisFreeNetBufferListContext(
    _In_ PNET_BUFFER_LIST        NetBufferList,
    _In_ USHORT                  ContextSize
    );

_Must_inspect_result_
__drv_allocatesMem(mem)
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
PNET_BUFFER_LIST
NdisAllocateCloneNetBufferList(
    _In_     PNET_BUFFER_LIST         OriginalNetBufferList,
    _In_opt_ NDIS_HANDLE              NetBufferListPoolHandle,
    _In_opt_ NDIS_HANDLE              NetBufferPoolHandle,
    _In_     ULONG                    AllocateCloneFlags
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFreeCloneNetBufferList(
    _In_ __drv_freesMem(mem) PNET_BUFFER_LIST         CloneNetBufferList,
    _In_                     ULONG                    FreeCloneFlags
    );

_Must_inspect_result_
__drv_allocatesMem(mem)
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
PNET_BUFFER_LIST
NdisAllocateFragmentNetBufferList(
    _In_      PNET_BUFFER_LIST        OriginalNetBufferList,
    _In_opt_  NDIS_HANDLE             NetBufferListPool,
    _In_opt_  NDIS_HANDLE             NetBufferPool,
    _In_      ULONG                   StartOffset,
    _In_      ULONG                   MaximumLength,
    _In_      ULONG                   DataOffsetDelta,
    _In_      ULONG                   DataBackFill,
    _In_      ULONG                   AllocateFragmentFlags
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFreeFragmentNetBufferList(
    _In_ __drv_freesMem(mem) PNET_BUFFER_LIST        FragmentNetBufferList,
    _In_                     ULONG                   DataOffsetDelta,
    _In_                     ULONG                   FreeFragmentFlags
    );

_Must_inspect_result_
__drv_allocatesMem(mem)
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
PNET_BUFFER_LIST
NdisAllocateReassembledNetBufferList(
    _In_      PNET_BUFFER_LIST        FragmentNetBufferList,
    _In_opt_  NDIS_HANDLE             NetBufferAndNetBufferListPoolHandle,
    _In_      ULONG                   StartOffset,
    _In_      ULONG                   DataOffsetDelta,
    _In_      ULONG                   DataBackFill,
    _In_      ULONG                   AllocateReassembleFlags
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFreeReassembledNetBufferList(
    _In_ __drv_freesMem(mem) PNET_BUFFER_LIST        ReassembledNetBufferList,
    _In_                     ULONG                   DataOffsetDelta,
    _In_                     ULONG                   FreeReassembleFlags
    );

_Must_inspect_result_
__drv_allocatesMem(mem)
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
PNET_BUFFER_LIST
NdisAllocateNetBufferAndNetBufferList(
    _In_                                NDIS_HANDLE             PoolHandle,
    _In_                                USHORT                  ContextSize,
    _In_                                USHORT                  ContextBackFill,
    _In_opt_  __drv_aliasesMem          PMDL                    MdlChain,
    _In_                                ULONG                   DataOffset,
    _In_                                SIZE_T                  DataLength
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
_At_(AlignOffset, _In_range_(0, AlignMultiple-1))
_Pre_satisfies_(AlignMultiple == 1 || AlignMultiple == 2 || AlignMultiple == 4 ||
AlignMultiple == 8 || AlignMultiple == 16 || AlignMultiple == 32 || AlignMultiple == 64 ||
AlignMultiple ==128 || AlignMultiple ==256 || AlignMultiple ==512 || AlignMultiple == 1024 ||
AlignMultiple == 2048 || AlignMultiple == 4096 || AlignMultiple == 8192)
_Must_inspect_result_
EXPORT
PVOID
NdisGetDataBuffer(
    _In_      PNET_BUFFER  NetBuffer,
    _In_      ULONG        BytesNeeded,
    _Out_writes_bytes_all_opt_(BytesNeeded) PVOID Storage,
    _In_      UINT         AlignMultiple,
    _In_      UINT         AlignOffset
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_HANDLE
NdisGetPoolFromNetBufferList(
    _In_ PNET_BUFFER_LIST     NetBufferList
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_HANDLE
NdisGetPoolFromNetBuffer(
    _In_ PNET_BUFFER          NetBuffer
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisCopyFromNetBufferToNetBuffer(
    _In_  PNET_BUFFER         Destination,
    _In_  ULONG               DestinationOffset,
    _In_  ULONG               BytesToCopy,
    _In_  PNET_BUFFER         Source,
    _In_  ULONG               SourceOffset,
    _Out_ PULONG              BytesCopied
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCopySendNetBufferListInfo(
    _In_ PNET_BUFFER_LIST DestNetBufferList,
    _In_ PNET_BUFFER_LIST SrcNetBufferList
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCopyReceiveNetBufferListInfo(
    _In_ PNET_BUFFER_LIST DestNetBufferList,
    _In_ PNET_BUFFER_LIST SrcNetBufferList
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
ULONG
NdisQueryNetBufferPhysicalCount(
    _In_  PNET_BUFFER             NetBuffer
    );

_Must_inspect_result_
__drv_allocatesMem(mem)
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
PMDL
NdisAllocateMdl(
    _In_  NDIS_HANDLE             NdisHandle,
    _In_reads_bytes_(Length)
          PVOID                   VirtualAddress,
    _In_  UINT                    Length
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFreeMdl(
    _In_  __drv_freesMem(mem) PMDL                     Mdl
    );

//
// The following flags are used in the Send APIs such as Ndis(F)SendNetBufferLists
// and MINIPORT_SEND_NET_BUFFER_LISTS_HANDLER
//
#define NDIS_SEND_FLAGS_DISPATCH_LEVEL                  0x00000001
#define NDIS_SEND_FLAGS_CHECK_FOR_LOOPBACK              0x00000002
#if (NDIS_SUPPORT_NDIS620)
#define NDIS_SEND_FLAGS_SINGLE_QUEUE                    0x00000004
#endif
#if (NDIS_SUPPORT_NDIS630)
#define NDIS_SEND_FLAGS_SWITCH_DESTINATION_GROUP        0x00000010
#define NDIS_SEND_FLAGS_SWITCH_SINGLE_SOURCE            0x00000020
#endif

#define NDIS_TEST_SEND_FLAG(_Flags, _Fl)            (((_Flags) & (_Fl)) == (_Fl))
#define NDIS_SET_SEND_FLAG(_Flags, _Fl)             ((_Flags) |= (_Fl))
#define NDIS_CLEAR_SEND_FLAG(_Flags, _Fl)           ((_Flags) &= ~(_Fl))

#define NDIS_TEST_SEND_AT_DISPATCH_LEVEL(_Flags)             \
        NDIS_TEST_SEND_FLAG((_Flags), NDIS_SEND_FLAGS_DISPATCH_LEVEL)

//
// The following flags are used in the SendComplete APIs such as
// NdisMSendNetBufferListsComplete
//
#define NDIS_SEND_COMPLETE_FLAGS_DISPATCH_LEVEL          0x00000001
#if (NDIS_SUPPORT_NDIS620)
#define NDIS_SEND_COMPLETE_FLAGS_SINGLE_QUEUE            0x00000002
#endif
#if (NDIS_SUPPORT_NDIS630)
#define NDIS_SEND_COMPLETE_FLAGS_SWITCH_SINGLE_SOURCE    0x00000004
#endif

#define NDIS_TEST_SEND_COMPLETE_FLAG(_Flags, _Fl)   (((_Flags) & (_Fl)) == (_Fl))
#define NDIS_SET_SEND_COMPLETE_FLAG(_Flags, _Fl)    ((_Flags) |= (_Fl))

#define NDIS_TEST_SEND_COMPLETE_AT_DISPATCH_LEVEL(_Flags)      \
        NDIS_TEST_SEND_COMPLETE_FLAG((_Flags), NDIS_SEND_COMPLETE_FLAGS_DISPATCH_LEVEL)

//
// The following flags are used in the IndicateReceive APIs such
// as NdisMIndicateReceiveNetBufferLists and RECEIVE_NET_BUFFER_LISTS_HANDLER
//
#define NDIS_RECEIVE_FLAGS_DISPATCH_LEVEL               0x00000001
#define NDIS_RECEIVE_FLAGS_RESOURCES                    0x00000002
#define NDIS_RECEIVE_FLAGS_SINGLE_ETHER_TYPE            0x00000100
#define NDIS_RECEIVE_FLAGS_SINGLE_VLAN                  0x00000200
#define NDIS_RECEIVE_FLAGS_PERFECT_FILTERED             0x00000400
#if (NDIS_SUPPORT_NDIS620)
#define NDIS_RECEIVE_FLAGS_SINGLE_QUEUE                 0x00000800
#define NDIS_RECEIVE_FLAGS_SHARED_MEMORY_INFO_VALID     0x00001000
#define NDIS_RECEIVE_FLAGS_MORE_NBLS                    0x00002000
#endif
#if (NDIS_SUPPORT_NDIS630)
#define NDIS_RECEIVE_FLAGS_SWITCH_DESTINATION_GROUP     0x00004000
#define NDIS_RECEIVE_FLAGS_SWITCH_SINGLE_SOURCE         0x00008000
#endif

#define NDIS_TEST_RECEIVE_FLAG(_Flags, _Fl)         (((_Flags) & (_Fl)) == (_Fl))
#define NDIS_SET_RECEIVE_FLAG(_Flags, _Fl)          ((_Flags) |= (_Fl))

#define NDIS_TEST_RECEIVE_AT_DISPATCH_LEVEL(_Flags)      \
        NDIS_TEST_RECEIVE_FLAG((_Flags), NDIS_RECEIVE_FLAGS_DISPATCH_LEVEL)

#define NDIS_TEST_RECEIVE_CANNOT_PEND(_Flags)         \
        NDIS_TEST_RECEIVE_FLAG((_Flags), NDIS_RECEIVE_FLAGS_RESOURCES)

#define NDIS_TEST_RECEIVE_CAN_PEND(_Flags)         \
            (((_Flags) & NDIS_RECEIVE_FLAGS_RESOURCES) == 0)
//
// The following flags are used in the ReturnNetBufferLists APIs such
// as NdisReturnNetBufferLists and MINIPORT_RETURN_NET_BUFFER_LISTS
//
#define NDIS_RETURN_FLAGS_DISPATCH_LEVEL               0x00000001
#if (NDIS_SUPPORT_NDIS620)
#define NDIS_RETURN_FLAGS_SINGLE_QUEUE                 0x00000002
#endif
#if (NDIS_SUPPORT_NDIS630)
#define NDIS_RETURN_FLAGS_SWITCH_SINGLE_SOURCE         0x00000004
#endif


#define NDIS_TEST_RETURN_FLAG(_Flags, _Fl)          (((_Flags) & (_Fl)) == (_Fl))
#define NDIS_SET_RETURN_FLAG(_Flags, _Fl)           ((_Flags) |= (_Fl))

#define NDIS_TEST_RETURN_AT_DISPATCH_LEVEL(_Flags)      \
        NDIS_TEST_RETURN_FLAG((_Flags),NDIS_RETURN_FLAGS_DISPATCH_LEVEL)


//
// The following flags are used in the NdisAllocateCloneNetBufferList
// and NdisFreeCloneNetBufferList
//

#define NDIS_CLONE_FLAGS_RESERVED                     0x00000001
#define NDIS_CLONE_FLAGS_USE_ORIGINAL_MDLS            0x00000002

#define NDIS_TEST_CLONE_FLAG(_Flags, _Fl)          (((_Flags) & (_Fl)) == (_Fl))
#define NDIS_SET_CLONE_FLAG(_Flags, _Fl)           ((_Flags) |= (_Fl))
#define NDIS_CLEAR_CLONE_FLAG(_Flags, _Fl)         ((_Flags) &= ~(_Fl))


#endif // NDIS_SUPPORT_NDIS6

#pragma warning(pop)


//
//    Copyright (C) Microsoft.  All rights reserved.
//
#pragma once
#pragma warning(push)

#pragma warning(disable:4201) // (nonstandard extension used : nameless struct/union)
#pragma warning(disable:4214) // (extension used : bit field types other than int)

#define NDIS_TASK_OFFLOAD_VERSION 1

#define MAX_HASHES          4
#define TRUNCATED_HASH_LEN  12

#define CRYPTO_SUCCESS                      0
#define CRYPTO_GENERIC_ERROR                1
#define CRYPTO_TRANSPORT_AH_AUTH_FAILED     2
#define CRYPTO_TRANSPORT_ESP_AUTH_FAILED    3
#define CRYPTO_TUNNEL_AH_AUTH_FAILED        4
#define CRYPTO_TUNNEL_ESP_AUTH_FAILED       5
#define CRYPTO_INVALID_PACKET_SYNTAX        6
#define CRYPTO_INVALID_PROTOCOL             7

#if NDIS_LEGACY_DRIVER
typedef struct _NDIS_IPSEC_PACKET_INFO
{
    union
    {
        struct
        {
            NDIS_HANDLE OffloadHandle;
            NDIS_HANDLE NextOffloadHandle;

        } Transmit;

        struct
        {
            ULONG   SA_DELETE_REQ:1;
            ULONG   CRYPTO_DONE:1;
            ULONG   NEXT_CRYPTO_DONE:1;
            ULONG   CryptoStatus;
        } Receive;
    };
} NDIS_IPSEC_PACKET_INFO, *PNDIS_IPSEC_PACKET_INFO;
#endif // NDIS_LEGACY_DRIVER

#if NDIS_SUPPORT_NDIS6
typedef struct _NDIS_IPSEC_OFFLOAD_V1_NET_BUFFER_LIST_INFO
{
    union
    {
        struct
        {
            NDIS_HANDLE OffloadHandle;

        } Transmit;

        struct
        {
            USHORT   SaDeleteReq:1;
            USHORT   CryptoDone:1;
            USHORT   NextCryptoDone:1;
            USHORT   Pad:13;
            USHORT   CryptoStatus;
        } Receive;
    };
} NDIS_IPSEC_OFFLOAD_V1_NET_BUFFER_LIST_INFO, *PNDIS_IPSEC_OFFLOAD_V1_NET_BUFFER_LIST_INFO;
#endif // NDIS_SUPPORT_NDIS6

#if NDIS_LEGACY_DRIVER
//
//  The following defines are used in the Task field above to define
//  the type of task offloading necessary.
//
typedef enum _NDIS_TASK
{
    TcpIpChecksumNdisTask,
    IpSecNdisTask,
    TcpLargeSendNdisTask,
    MaxNdisTask
} NDIS_TASK, *PNDIS_TASK;


typedef enum _NDIS_ENCAPSULATION
{
    UNSPECIFIED_Encapsulation,
    NULL_Encapsulation,
    IEEE_802_3_Encapsulation,
    IEEE_802_5_Encapsulation,
    LLC_SNAP_ROUTED_Encapsulation,
    LLC_SNAP_BRIDGED_Encapsulation

} NDIS_ENCAPSULATION;

//
// Encapsulation header format
//
typedef struct _NDIS_ENCAPSULATION_FORMAT
{
    NDIS_ENCAPSULATION  Encapsulation;              // Encapsulation type
    struct
    {
        ULONG   FixedHeaderSize:1;
        ULONG   Reserved:31;
    } Flags;

    ULONG    EncapsulationHeaderSize;               // Encapsulation header size

} NDIS_ENCAPSULATION_FORMAT,*PNDIS_ENCAPSULATION_FORMAT;


//
// OFFLOAD header structure for OID_TCP_TASK_OFFLOAD
//
typedef struct _NDIS_TASK_OFFLOAD_HEADER
{
    ULONG       Version;                            // set to NDIS_TASK_OFFLOAD_VERSION
    ULONG       Size;                               // Size of this structure
    ULONG       Reserved;                           // Reserved for future use
    ULONG       OffsetFirstTask;                    // Offset to the first
    NDIS_ENCAPSULATION_FORMAT  EncapsulationFormat; // Encapsulation information.
                                                    // NDIS_TASK_OFFLOAD structure(s)

} NDIS_TASK_OFFLOAD_HEADER, *PNDIS_TASK_OFFLOAD_HEADER;


//
//  Task offload Structure, which follows the above header in ndis query
//
typedef struct _NDIS_TASK_OFFLOAD
{
    ULONG       Version;                            //  NDIS_TASK_OFFLOAD_VERSION
    ULONG       Size;                               //  Size of this structure. Used for version checking.
    NDIS_TASK   Task;                               //  Task.
    ULONG       OffsetNextTask;                     //  Offset to the next NDIS_TASK_OFFLOAD
    ULONG       TaskBufferLength;                   //  Length of the task offload information.
    UCHAR       TaskBuffer[1];                      //  The task offload information.
} NDIS_TASK_OFFLOAD, *PNDIS_TASK_OFFLOAD;

//
//  Offload structure for NDIS_TASK_TCP_IP_CHECKSUM
//
typedef struct _NDIS_TASK_TCP_IP_CHECKSUM
{
    struct
    {
        ULONG       IpOptionsSupported:1;
        ULONG       TcpOptionsSupported:1;
        ULONG       TcpChecksum:1;
        ULONG       UdpChecksum:1;
        ULONG       IpChecksum:1;
    } V4Transmit;

    struct
    {
        ULONG       IpOptionsSupported:1;
        ULONG       TcpOptionsSupported:1;
        ULONG       TcpChecksum:1;
        ULONG       UdpChecksum:1;
        ULONG       IpChecksum:1;
    } V4Receive;


    struct
    {
        ULONG       IpOptionsSupported:1;           // This field implies IpExtensionHeaders support
        ULONG       TcpOptionsSupported:1;
        ULONG       TcpChecksum:1;
        ULONG       UdpChecksum:1;

    } V6Transmit;

    struct
    {
        ULONG       IpOptionsSupported:1;           // This field implies IpExtensionHeaders support
        ULONG       TcpOptionsSupported:1;
        ULONG       TcpChecksum:1;
        ULONG       UdpChecksum:1;

    } V6Receive;


} NDIS_TASK_TCP_IP_CHECKSUM, *PNDIS_TASK_TCP_IP_CHECKSUM;

//
//  Off-load structure for NDIS_TASK_TCP_LARGE_SEND
//
#define NDIS_TASK_TCP_LARGE_SEND_V0         0

typedef struct _NDIS_TASK_TCP_LARGE_SEND
{
    ULONG     Version;                      // set to NDIS_TASK_TCP_LARGE_SEND_V0
    ULONG     MaxOffLoadSize;
    ULONG     MinSegmentCount;
    BOOLEAN   TcpOptions;
    BOOLEAN   IpOptions;

} NDIS_TASK_TCP_LARGE_SEND, *PNDIS_TASK_TCP_LARGE_SEND;


typedef struct _NDIS_TASK_IPSEC
{
    struct
    {
        ULONG   AH_ESP_COMBINED;
        ULONG   TRANSPORT_TUNNEL_COMBINED;
        ULONG   V4_OPTIONS;
        ULONG   RESERVED;
    } Supported;

    struct
    {
        ULONG   MD5:1;
        ULONG   SHA_1:1;
        ULONG   Transport:1;
        ULONG   Tunnel:1;
        ULONG   Send:1;
        ULONG   Receive:1;
    } V4AH;

    struct
    {
        ULONG   DES:1;
        ULONG   RESERVED:1;
        ULONG   TRIPLE_DES:1;
        ULONG   NULL_ESP:1;
        ULONG   Transport:1;
        ULONG   Tunnel:1;
        ULONG   Send:1;
        ULONG   Receive:1;
    } V4ESP;

} NDIS_TASK_IPSEC, *PNDIS_TASK_IPSEC;
#endif // NDIS_LEGACY_DRIVER

//
// flags used in NDIS_TASK_IPSEC->Supported.RESERVED and
// NDIS_IPSEC_OFFLOAD_V1->Supported.Flags
//
#define IPSEC_TPT_UDPESP_ENCAPTYPE_IKE                 0x00000001
#define IPSEC_TUN_UDPESP_ENCAPTYPE_IKE                 0x00000002
#define IPSEC_TPTOVERTUN_UDPESP_ENCAPTYPE_IKE          0x00000004
#define IPSEC_TPT_UDPESP_OVER_PURE_TUN_ENCAPTYPE_IKE   0x00000008
#define IPSEC_TPT_UDPESP_ENCAPTYPE_OTHER               0x00000010
#define IPSEC_TUN_UDPESP_ENCAPTYPE_OTHER               0x00000020
#define IPSEC_TPTOVERTUN_UDPESP_ENCAPTYPE_OTHER        0x00000040
#define IPSEC_TPT_UDPESP_OVER_PURE_TUN_ENCAPTYPE_OTHER 0x00000080


#if NDIS_SUPPORT_NDIS6

//
// The definitions for the NDIS_TCP_LARGE_SEND_OFFLOAD_NET_BUFFER_LIST_INFO.Transmit.Type
// A miniport will use this definition to identify what offload type to use
// on the NetBufferList
//
#define NDIS_TCP_LARGE_SEND_OFFLOAD_V1_TYPE     0
#define NDIS_TCP_LARGE_SEND_OFFLOAD_V2_TYPE     1

//
// IP protocol version encoded in IPVersion field of
// NDIS_TCP_LARGE_SEND_OFFLOAD_NET_BUFFER_LIST_INFO->LsoV2Transmit.IPVersion
//
#define NDIS_TCP_LARGE_SEND_OFFLOAD_IPv4     0
#define NDIS_TCP_LARGE_SEND_OFFLOAD_IPv6     1

//
// The maximum length of the headers (MAC+IP+IP option or extension headers+TCP+TCP options)
// when stack does large send offload. If header is bigger than this value, it will not do
// LSO
//
#define NDIS_LARGE_SEND_OFFLOAD_MAX_HEADER_LENGTH   128

//
// This structure is used in the OOB TcpLargeSendNetBufferListInfo.
//
typedef struct _NDIS_TCP_LARGE_SEND_OFFLOAD_NET_BUFFER_LIST_INFO
{
    union
    {
        struct
        {
            ULONG    Unused:30;
            ULONG    Type:1;
            ULONG    Reserved2:1;
        } Transmit;

        struct
        {
            ULONG    MSS:20;
            ULONG    TcpHeaderOffset:10;
            ULONG    Type:1;
            ULONG    Reserved2:1;
        } LsoV1Transmit;

        struct
        {
            ULONG    TcpPayload:30;
            ULONG    Type:1;
            ULONG    Reserved2:1;
        } LsoV1TransmitComplete;

        struct
        {
            ULONG    MSS:20;
            ULONG    TcpHeaderOffset:10;
            ULONG    Type:1;
            ULONG    IPVersion:1;
        } LsoV2Transmit;

        struct
        {
            ULONG    Reserved:30;
            ULONG    Type:1;
            ULONG    Reserved2:1;
        } LsoV2TransmitComplete;

        PVOID Value;
    };
} NDIS_TCP_LARGE_SEND_OFFLOAD_NET_BUFFER_LIST_INFO,*PNDIS_TCP_LARGE_SEND_OFFLOAD_NET_BUFFER_LIST_INFO;


//
//  Per-NetBufferList information for TcpIpChecksumNetBufferListInfo.
//
typedef struct _NDIS_TCP_IP_CHECKSUM_NET_BUFFER_LIST_INFO
{
    union
    {
        struct
        {
            ULONG IsIPv4:1;
            ULONG IsIPv6:1;
            ULONG TcpChecksum:1;
            ULONG UdpChecksum:1;
            ULONG IpHeaderChecksum:1;
            ULONG Reserved:11;
            ULONG TcpHeaderOffset:10;
        } Transmit;

        struct
        {
            ULONG   TcpChecksumFailed:1;
            ULONG   UdpChecksumFailed:1;
            ULONG   IpChecksumFailed:1;
            ULONG   TcpChecksumSucceeded:1;
            ULONG   UdpChecksumSucceeded:1;
            ULONG   IpChecksumSucceeded:1;
            ULONG   Loopback:1;
#if (NDIS_SUPPORT_NDIS630)
            ULONG   TcpChecksumValueInvalid:1;
            ULONG   IpChecksumValueInvalid:1;
#endif // (NDIS_SUPPORT_NDIS630)
        } Receive;

        PVOID   Value;
    };
} NDIS_TCP_IP_CHECKSUM_NET_BUFFER_LIST_INFO, *PNDIS_TCP_IP_CHECKSUM_NET_BUFFER_LIST_INFO;

#if (NDIS_SUPPORT_NDIS630)

//
//  Per-NetBufferList information for TcpSendOffloadsSupplementalNetBufferListInfo.
//
typedef struct _NDIS_TCP_SEND_OFFLOADS_SUPPLEMENTAL_NET_BUFFER_LIST_INFO
{
    union
    {
        struct
        {
            ULONG IsEncapsulatedPacket:1;
            ULONG EncapsulatedPacketOffsetsValid:1;
            ULONG InnerFrameOffset:8;
            ULONG TransportIpHeaderRelativeOffset:6;
            ULONG TcpHeaderRelativeOffset:10;
            ULONG IsInnerIPv6:1;
            ULONG TcpOptionsPresent:1;
            ULONG Reserved:4;
        } EncapsulatedPacketOffsets;

        PVOID   Value;
    };
} NDIS_TCP_SEND_OFFLOADS_SUPPLEMENTAL_NET_BUFFER_LIST_INFO, *PNDIS_TCP_SEND_OFFLOADS_SUPPLEMENTAL_NET_BUFFER_LIST_INFO;

#endif // (NDIS_SUPPORT_NDIS630)

//
// used in NDIS_STATUS_OFFLOAD_ENCASPULATION_CHANGE status indication and
// and OID_OFFLOAD_ENCAPSULATION OID request
//
#define NDIS_OFFLOAD_ENCAPSULATION_REVISION_1          1

typedef struct _NDIS_OFFLOAD_ENCAPSULATION
{
    //
    // Header.Type = NDIS_OBJECT_TYPE_OFFLOAD_ENCAPSULATION;
    // Header.Size = sizeof(NDIS_OFFLOAD_ENCAPSULATION);
    // Header.Revision  = NDIS_OFFLOAD_ENCAPSULATION_REVISION_1;
    //

    NDIS_OBJECT_HEADER                     Header;

    struct
    {
        //
        // A Protocol sets Enable to NDIS_OFFLOAD_SET_ON if it is enabling IPv4 LSO,
        // or XSum offloads
        // otherwise it is set to NDIS_OFFLOAD_SET_NO_CHANGE
        //
        ULONG                               Enabled;

        //
        // If Enabled is TRUE, a Protocol must set this to
        // either NDIS_ENCAPSULATION_IEEE_802_3
        // or NDIS_ENCAPSULATION_IEEE_LLC_SNAP_ROUTED
        //
        ULONG                               EncapsulationType;

        //
        // If Enabled is TRUE, a protocol must set this field to the
        // HeaderSize it uses
        //
        ULONG                               HeaderSize;
    } IPv4;


    struct
    {
        //
        // A Protocol sets Enable to NDIS_OFFLOAD_SET_ON if it is enabling IPv6 LSO,
        // or XSum offloads
        // otherwise it is set to NDIS_OFFLOAD_SET_NO_CHANGE
        //
        ULONG                               Enabled;
        //
        // If Enabled is TRUE, a Protocol must set this to
        // either NDIS_ENCAPSULATION_IEEE_802_3
        // or NDIS_ENCAPSULATION_IEEE_LLC_SNAP_ROUTED
        //
        ULONG                               EncapsulationType;

        //
        // If Enabled is TRUE, a protocol must set this field to the
        // HeaderSize it uses
        //
        ULONG                               HeaderSize;
    } IPv6;

}NDIS_OFFLOAD_ENCAPSULATION, *PNDIS_OFFLOAD_ENCAPSULATION;

#define NDIS_SIZEOF_OFFLOAD_ENCAPSULATION_REVISION_1 RTL_SIZEOF_THROUGH_FIELD(NDIS_OFFLOAD_ENCAPSULATION, IPv6.HeaderSize)

#if (NDIS_SUPPORT_NDIS61)
//
//  Per-NetBufferList information for IPsecOffloadV2NetBufferListInfo.
//
typedef struct _NDIS_IPSEC_OFFLOAD_V2_NET_BUFFER_LIST_INFO
{
    union
    {
        struct
        {
            PVOID   OffloadHandle;
        } Transmit;

        struct
        {
            ULONG   SaDeleteReq:1;
            ULONG   CryptoDone:1;
            ULONG   NextCryptoDone:1;  // Required for transport over tunnel
            ULONG   Reserved:13;
            ULONG   CryptoStatus:16;
        } Receive;
    };
} NDIS_IPSEC_OFFLOAD_V2_NET_BUFFER_LIST_INFO, *PNDIS_IPSEC_OFFLOAD_V2_NET_BUFFER_LIST_INFO;

//
//  Per-NetBufferList information for IPsecOffloadV2TunnelNetBufferListInfo.
//
typedef struct _NDIS_IPSEC_OFFLOAD_V2_TUNNEL_NET_BUFFER_LIST_INFO
{
    struct
    {
        NDIS_HANDLE TunnelHandle;      // Tunnel SA handle in Transport over tunnel
    } Transmit;
} NDIS_IPSEC_OFFLOAD_V2_TUNNEL_NET_BUFFER_LIST_INFO, *PNDIS_IPSEC_OFFLOAD_V2_TUNNEL_NET_BUFFER_LIST_INFO;

//
//  Per-NetBufferList information for IPsecOffloadV2HeaderNetBufferListInfo.
//
typedef struct _NDIS_IPSEC_OFFLOAD_V2_HEADER_NET_BUFFER_LIST_INFO
{
    union
    {
        struct
        {
            ULONG   NextHeader:8;           // Next Header value is the one carried in ESP trailer
            ULONG   PadLength:8;
            ULONG   AhHeaderOffset:8;       // This is offset from beginning of IP Header and is measured in 4 byte increments
            ULONG   EspHeaderOffset:8;      // This is offset from beginning of IP Header and is measured in 4 byte increments
        } Transmit;

        struct
        {
            ULONG   NextHeader:8;
            ULONG   PadLength:8;
            ULONG   HeaderInfoSet:1;
        } Receive;
    };
} NDIS_IPSEC_OFFLOAD_V2_HEADER_NET_BUFFER_LIST_INFO, *PNDIS_IPSEC_OFFLOAD_V2_HEADER_NET_BUFFER_LIST_INFO;

//
// Used in IPSEC_OFFLOAD_V2_ADD_SA.SecAssoc
//
#define IPSEC_OFFLOAD_V2_MAX_EXTENSION_HEADERS        2

//
// used in IPSEC_OFFLOAD_V2_ADD_SA.UdpEspEncapsulation and
// NDIS_IPSEC_OFFLOAD_V2.UdpEsp
//
#define IPSEC_OFFLOAD_V2_UDP_ESP_ENCAPSULATION_NONE                         0x00000000
#define IPSEC_OFFLOAD_V2_UDP_ESP_ENCAPSULATION_TRANSPORT                    0x00000001
#define IPSEC_OFFLOAD_V2_UDP_ESP_ENCAPSULATION_TUNNEL                       0x00000002
#define IPSEC_OFFLOAD_V2_TRANSPORT_OVER_UDP_ESP_ENCAPSULATION_TUNNEL        0x00000004
#define IPSEC_OFFLOAD_V2_UDP_ESP_ENCAPSULATION_TRANSPORT_OVER_TUNNEL        0x00000008

//
// used in IPSEC_OFFLOAD_V2_SECURITY_ASSOCIATION.Flags
//
#define IPSEC_OFFLOAD_V2_ESN_SA                                             0x00000001

//
// used in IPSEC_OFFLOAD_V2_SECURITY_ASSOCIATION.Operation
//
typedef enum _IPSEC_OFFLOAD_V2_OPERATION
{
    IPsecOffloadV2Ah = 1,
    IPsecOffloadV2Esp,
    IPsecOffloadV2Max
} IPSEC_OFFLOAD_V2_OPERATION, *PIPSEC_OFFLOAD_V2_OPERATION;

//
// used in IPSEC_OFFLOAD_V2_SECURITY_ASSOCIATION.Spi
//
typedef ULONG IPSEC_OFFLOAD_V2_SPI_TYPE;

//
// used in AuthenticationAlgorithm and EncryptionAlgorithm fields
// of IPSEC_OFFLOAD_V2_SECURITY_ASSOCIATION structure
//
typedef struct _IPSEC_OFFLOAD_V2_ALGORITHM_INFO
{
    ULONG  Identifier;
    ULONG  KeyLength;
    ULONG  KeyOffsetBytes;
    ULONG  AdditionalInfo;
} IPSEC_OFFLOAD_V2_ALGORITHM_INFO, *PIPSEC_OFFLOAD_V2_ALGORITHM_INFO;

//
// used in IPSEC_OFFLOAD_V2_ADD_SA.SecAssoc
//
typedef struct _IPSEC_OFFLOAD_V2_SECURITY_ASSOCIATION
{
    ULONG                                 Flags;
    IPSEC_OFFLOAD_V2_OPERATION            Operation;        // AH - SA is used for AH, ESP - SA is used for ESP
    IPSEC_OFFLOAD_V2_SPI_TYPE             Spi;
    IPSEC_OFFLOAD_V2_ALGORITHM_INFO       AuthenticationAlgorithm;
    IPSEC_OFFLOAD_V2_ALGORITHM_INFO       EncryptionAlgorithm;
    ULONG                                 SequenceNumberHighOrder;
} IPSEC_OFFLOAD_V2_SECURITY_ASSOCIATION, *PIPSEC_OFFLOAD_V2_SECURITY_ASSOCIATION;

//
// used in IPSEC_OFFLOAD_V2_ADD_SA.Flags
//
#define IPSEC_OFFLOAD_V2_INBOUND          0x00000001
#define IPSEC_OFFLOAD_V2_IPv6             0x00000010

//
// used in OID_TCP_TASK_IPSEC_OFFLOAD_V2_ADD_SA
//
typedef struct _IPSEC_OFFLOAD_V2_ADD_SA IPSEC_OFFLOAD_V2_ADD_SA, *PIPSEC_OFFLOAD_V2_ADD_SA;

#define NDIS_IPSEC_OFFLOAD_V2_ADD_SA_REVISION_1      1

typedef struct _IPSEC_OFFLOAD_V2_ADD_SA
{
    //
    // Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
    // Header.Revision = NDIS_IPSEC_OFFLOAD_V2_ADD_SA_REVISION_1;
    // Header.Size = sizeof(IPSEC_OFFLOAD_V2_ADD_SA);
    //
    NDIS_OBJECT_HEADER            Header;
    PIPSEC_OFFLOAD_V2_ADD_SA      Next;
    ULONG                         NumExtHdrs;
    ULONG                         Flags;

    union
    {
        struct
        {
            IPAddr SrcAddr;
            IPAddr DestAddr;
        } IPv4Endpoints;

        struct
        {
            UCHAR SrcAddr[16];
            UCHAR DestAddr[16];
        } IPv6Endpoints;
    };

    NDIS_HANDLE                   OffloadHandle;
    ULONG                         UdpEspEncapsulation;
    IPSEC_OFFLOAD_V2_SECURITY_ASSOCIATION SecAssoc[IPSEC_OFFLOAD_V2_MAX_EXTENSION_HEADERS];
    ULONG                         KeyLength;
    UCHAR                         KeyData[1];
} IPSEC_OFFLOAD_V2_ADD_SA, *PIPSEC_OFFLOAD_V2_ADD_SA;

#define NDIS_SIZEOF_IPSEC_OFFLOAD_V2_ADD_SA_REVISION_1     \
        RTL_SIZEOF_THROUGH_FIELD(IPSEC_OFFLOAD_V2_ADD_SA, KeyData)

#if (NDIS_SUPPORT_NDIS630)

typedef struct _IPSEC_OFFLOAD_V2_ADD_SA_EX IPSEC_OFFLOAD_V2_ADD_SA_EX, *PIPSEC_OFFLOAD_V2_ADD_SA_EX;

#define NDIS_IPSEC_OFFLOAD_V2_ADD_SA_EX_REVISION_1      1

typedef struct _IPSEC_OFFLOAD_V2_ADD_SA_EX
{
    //
    // Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
    // Header.Revision = NDIS_IPSEC_OFFLOAD_V2_ADD_SA_EX_REVISION_1;
    // Header.Size = NDIS_SIZEOF_IPSEC_OFFLOAD_V2_ADD_SA_EX_REVISION_1;
    //
    _In_ NDIS_OBJECT_HEADER            Header;
    _In_ ULONG                         NumExtHdrs;
    _In_ ULONG                         Flags;

    _In_ union
    {
        struct
        {
            IPAddr SrcAddr;
            IPAddr DestAddr;
        } IPv4Endpoints;

        struct
        {
            UCHAR SrcAddr[16];
            UCHAR DestAddr[16];
        } IPv6Endpoints;
    };

    _Out_ NDIS_HANDLE                  OffloadHandle;
    _In_ ULONG                         UdpEspEncapsulation;
    _In_ IPSEC_OFFLOAD_V2_SECURITY_ASSOCIATION SecAssoc[IPSEC_OFFLOAD_V2_MAX_EXTENSION_HEADERS];
    _In_ ULONG                         KeyLength;
    _In_ ULONG                         KeyOffset;
    _In_ NDIS_SWITCH_PORT_ID           SourceSwitchPortId;
    _In_ USHORT                        VlanId;
} IPSEC_OFFLOAD_V2_ADD_SA_EX, *PIPSEC_OFFLOAD_V2_ADD_SA_EX;

#define NDIS_SIZEOF_IPSEC_OFFLOAD_V2_ADD_SA_EX_REVISION_1     \
        RTL_SIZEOF_THROUGH_FIELD(IPSEC_OFFLOAD_V2_ADD_SA_EX, VlanId)

#endif


//
// used in OID_TCP_TASK_IPSEC_OFFLOAD_V2_DELETE_SA
//
typedef struct _IPSEC_OFFLOAD_V2_DELETE_SA IPSEC_OFFLOAD_V2_DELETE_SA, *PIPSEC_OFFLOAD_V2_DELETE_SA;

#define NDIS_IPSEC_OFFLOAD_V2_DELETE_SA_REVISION_1      1

typedef struct _IPSEC_OFFLOAD_V2_DELETE_SA
{
    //
    // Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
    // Header.Revision = NDIS_IPSEC_OFFLOAD_V2_DELETE_SA_REVISION_1;
    // Header.Size = sizeof(IPSEC_OFFLOAD_V2_DELETE_SA);
    //
    NDIS_OBJECT_HEADER              Header;
    PIPSEC_OFFLOAD_V2_DELETE_SA     Next;
    NDIS_HANDLE                     OffloadHandle;
} IPSEC_OFFLOAD_V2_DELETE_SA, *PIPSEC_OFFLOAD_V2_DELETE_SA;

#define NDIS_SIZEOF_IPSEC_OFFLOAD_V2_DELETE_SA_REVISION_1     \
        RTL_SIZEOF_THROUGH_FIELD(IPSEC_OFFLOAD_V2_DELETE_SA, OffloadHandle)

//
// Structure and defines used in
// OID_TCP_TASK_IPSEC_OFFLOAD_V2_UPDATE_SA
//

#define NDIS_IPSEC_OFFLOAD_V2_UPDATE_SA_REVISION_1      1

typedef struct _IPSEC_OFFLOAD_V2_UPDATE_SA
{
    //
    // Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
    // Header.Revision = NDIS_IPSEC_OFFLOAD_V2_UPDATE_SA_REVISION_1;
    // Header.Size = sizeof(IPSEC_OFFLOAD_V2_UPDATE_SA);
    //
    NDIS_OBJECT_HEADER              Header;
    NDIS_HANDLE                     OffloadHandle;
    IPSEC_OFFLOAD_V2_OPERATION      Operation;
    IPSEC_OFFLOAD_V2_SPI_TYPE       Spi;
    ULONG                           SequenceNumberHighOrder;
} IPSEC_OFFLOAD_V2_UPDATE_SA, *PIPSEC_OFFLOAD_V2_UPDATE_SA;

#define NDIS_SIZEOF_IPSEC_OFFLOAD_V2_UPDATE_SA_REVISION_1     \
        RTL_SIZEOF_THROUGH_FIELD(IPSEC_OFFLOAD_V2_UPDATE_SA, SequenceNumberHighOrder)

#endif //  (NDIS_SUPPORT_NDIS61)

#endif // NDIS_SUPPORT_NDIS6

#pragma warning(pop)



//
//    Copyright (C) Microsoft.  All rights reserved.
//
#pragma once

#if NDIS_LEGACY_PROTOCOL

//
// Function types for NDIS_PROTOCOL_CHARACTERISTICS
//

typedef
VOID
(*OPEN_ADAPTER_COMPLETE_HANDLER)(
    _In_  NDIS_HANDLE             ProtocolBindingContext,
    _In_  NDIS_STATUS             Status,
    _In_  NDIS_STATUS             OpenErrorStatus
    );

typedef
VOID
(*CLOSE_ADAPTER_COMPLETE_HANDLER)(
    _In_  NDIS_HANDLE             ProtocolBindingContext,
    _In_  NDIS_STATUS             Status
    );

typedef
VOID
(*RESET_COMPLETE_HANDLER)(
    _In_  NDIS_HANDLE             ProtocolBindingContext,
    _In_  NDIS_STATUS             Status
    );

typedef
VOID
(*REQUEST_COMPLETE_HANDLER)(
    _In_  NDIS_HANDLE             ProtocolBindingContext,
    _In_  PNDIS_REQUEST           NdisRequest,
    _In_  NDIS_STATUS             Status
    );

typedef
VOID
(*STATUS_HANDLER)(
    _In_  NDIS_HANDLE             ProtocolBindingContext,
    _In_  NDIS_STATUS             GeneralStatus,
    _In_  PVOID                   StatusBuffer,
    _In_  UINT                    StatusBufferSize
    );

typedef
VOID
(*STATUS_COMPLETE_HANDLER)(
    _In_  NDIS_HANDLE             ProtocolBindingContext
    );

typedef
VOID
(*SEND_COMPLETE_HANDLER)(
    _In_  NDIS_HANDLE             ProtocolBindingContext,
    _In_  PNDIS_PACKET            Packet,
    _In_  NDIS_STATUS             Status
    );

typedef
VOID
(*WAN_SEND_COMPLETE_HANDLER) (
    _In_  NDIS_HANDLE             ProtocolBindingContext,
    _In_  PNDIS_WAN_PACKET        Packet,
    _In_  NDIS_STATUS             Status
    );

typedef
VOID
(*TRANSFER_DATA_COMPLETE_HANDLER)(
    _In_  NDIS_HANDLE             ProtocolBindingContext,
    _In_  PNDIS_PACKET            Packet,
    _In_  NDIS_STATUS             Status,
    _In_  UINT                    BytesTransferred
    );

typedef
VOID
(*WAN_TRANSFER_DATA_COMPLETE_HANDLER)(
    VOID
    );

typedef
NDIS_STATUS
(*RECEIVE_HANDLER)(
    _In_  NDIS_HANDLE             ProtocolBindingContext,
    _In_  NDIS_HANDLE             MacReceiveContext,
    _In_  PVOID                   HeaderBuffer,
    _In_  UINT                    HeaderBufferSize,
    _In_  PVOID                   LookAheadBuffer,
    _In_  UINT                    LookaheadBufferSize,
    _In_  UINT                    PacketSize
    );

typedef
NDIS_STATUS
(*WAN_RECEIVE_HANDLER)(
    _In_  NDIS_HANDLE             NdisLinkHandle,
    _In_  PUCHAR                  Packet,
    _In_  ULONG                   PacketSize
    );

typedef
VOID
(*RECEIVE_COMPLETE_HANDLER)(
    _In_  NDIS_HANDLE             ProtocolBindingContext
    );

//
// Function types extensions for NDIS 4.0 Protocols
//
typedef
INT
(*RECEIVE_PACKET_HANDLER)(
    _In_  NDIS_HANDLE             ProtocolBindingContext,
    _In_  PNDIS_PACKET            Packet
    );

typedef
VOID
(*BIND_HANDLER)(
    _Out_ PNDIS_STATUS            Status,
    _In_  NDIS_HANDLE             BindContext,
    _In_  PNDIS_STRING            DeviceName,
    _In_  PVOID                   SystemSpecific1,
    _In_  PVOID                   SystemSpecific2
    );

typedef
VOID
(*UNBIND_HANDLER)(
    _Out_ PNDIS_STATUS            Status,
    _In_  NDIS_HANDLE             ProtocolBindingContext,
    _In_  NDIS_HANDLE             UnbindContext
    );

typedef
NDIS_STATUS
(*PNP_EVENT_HANDLER)(
    _In_  NDIS_HANDLE             ProtocolBindingContext,
    _In_  PNET_PNP_EVENT          NetPnPEvent
    );


typedef
VOID
(*UNLOAD_PROTOCOL_HANDLER)(
    VOID
    );

//
// Protocol characteristics for NDIS 4.0 protocols
//
typedef struct _NDIS40_PROTOCOL_CHARACTERISTICS
{
    UCHAR                           MajorNdisVersion;
    UCHAR                           MinorNdisVersion;
    USHORT                          Filler;
    union
    {
        UINT                        Reserved;
        UINT                        Flags;
    };
    OPEN_ADAPTER_COMPLETE_HANDLER   OpenAdapterCompleteHandler;
    CLOSE_ADAPTER_COMPLETE_HANDLER  CloseAdapterCompleteHandler;
    union
    {
        SEND_COMPLETE_HANDLER       SendCompleteHandler;
        WAN_SEND_COMPLETE_HANDLER   WanSendCompleteHandler;
    };
    union
    {
     TRANSFER_DATA_COMPLETE_HANDLER TransferDataCompleteHandler;
     WAN_TRANSFER_DATA_COMPLETE_HANDLER WanTransferDataCompleteHandler;
    };

    RESET_COMPLETE_HANDLER          ResetCompleteHandler;
    REQUEST_COMPLETE_HANDLER        RequestCompleteHandler;
    union
    {
        RECEIVE_HANDLER             ReceiveHandler;
        WAN_RECEIVE_HANDLER         WanReceiveHandler;
    };
    RECEIVE_COMPLETE_HANDLER        ReceiveCompleteHandler;
    STATUS_HANDLER                  StatusHandler;
    STATUS_COMPLETE_HANDLER         StatusCompleteHandler;
    NDIS_STRING                     Name;

    //
    // Start of NDIS 4.0 extensions.
    //
    RECEIVE_PACKET_HANDLER          ReceivePacketHandler;

    //
    // PnP protocol entry-points
    //
    BIND_HANDLER                    BindAdapterHandler;
    UNBIND_HANDLER                  UnbindAdapterHandler;
    PNP_EVENT_HANDLER               PnPEventHandler;
    UNLOAD_PROTOCOL_HANDLER         UnloadHandler;

} NDIS40_PROTOCOL_CHARACTERISTICS;

#endif

#if NDIS_LEGACY_DRIVER

//
// NDIS 5.0 co-NDIS Protocol handler proto-types - used by clients as well as call manager modules
//
typedef
_IRQL_requires_(PASSIVE_LEVEL)
VOID
(*CO_SEND_COMPLETE_HANDLER)(
    _In_  NDIS_STATUS             Status,
    _In_  NDIS_HANDLE             ProtocolVcContext,
    _In_  PNDIS_PACKET            Packet
    );

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
VOID
(*CO_STATUS_HANDLER)(
    _In_      NDIS_HANDLE             ProtocolBindingContext,
    _In_opt_  NDIS_HANDLE             ProtocolVcContext,
    _In_      NDIS_STATUS             GeneralStatus,
    _In_      PVOID                   StatusBuffer,
    _In_      UINT                    StatusBufferSize
    );

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
UINT
(*CO_RECEIVE_PACKET_HANDLER)(
    _In_  NDIS_HANDLE             ProtocolBindingContext,
    _In_  NDIS_HANDLE             ProtocolVcContext,
    _In_  PNDIS_PACKET            Packet
    );

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
NDIS_STATUS
(*CO_REQUEST_HANDLER)(
    _In_      NDIS_HANDLE          ProtocolAfContext,
    _In_opt_  NDIS_HANDLE          ProtocolVcContext,
    _In_opt_  NDIS_HANDLE          ProtocolPartyContext,
    _Inout_   PNDIS_REQUEST        NdisRequest
    );

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
VOID
(*CO_REQUEST_COMPLETE_HANDLER)(
    _In_      NDIS_STATUS             Status,
    _In_opt_  NDIS_HANDLE             ProtocolAfContext,
    _In_opt_  NDIS_HANDLE             ProtocolVcContext,
    _In_opt_  NDIS_HANDLE             ProtocolPartyContext,
    _In_      PNDIS_REQUEST           NdisRequest
    );

#endif // NDIS_LEGACY_DRIVER

#if NDIS_SUPPORT_NDIS6
typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CO_OID_REQUEST)
NDIS_STATUS
(PROTOCOL_CO_OID_REQUEST)(
    _In_      NDIS_HANDLE             ProtocolAfContext,
    _In_opt_  NDIS_HANDLE             ProtocolVcContext,
    _In_opt_  NDIS_HANDLE             ProtocolPartyContext,
    _Inout_   PNDIS_OID_REQUEST       OidRequest
    );

typedef PROTOCOL_CO_OID_REQUEST (*CO_OID_REQUEST_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CO_OID_REQUEST_COMPLETE)
VOID
(PROTOCOL_CO_OID_REQUEST_COMPLETE)(
    _In_opt_  NDIS_HANDLE             ProtocolAfContext,
    _In_opt_  NDIS_HANDLE             ProtocolVcContext,
    _In_opt_  NDIS_HANDLE             ProtocolPartyContext,
    _In_      PNDIS_OID_REQUEST       OidRequest,
    _In_      NDIS_STATUS             Status
    );

typedef PROTOCOL_CO_OID_REQUEST_COMPLETE (*CO_OID_REQUEST_COMPLETE_HANDLER);

#endif // NDIS_SUPPORT_NDIS6

//
// CO_CREATE_VC_HANDLER and CO_DELETE_VC_HANDLER are synchronous calls
// the following APIs are used by NDIS 6 protocols as well as NDIS 5 protocols
//
typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CO_CREATE_VC)
NDIS_STATUS
(PROTOCOL_CO_CREATE_VC)(
    _In_  NDIS_HANDLE             ProtocolAfContext,
    _In_  NDIS_HANDLE             NdisVcHandle,
    _Out_ PNDIS_HANDLE            ProtocolVcContext
    );

typedef PROTOCOL_CO_CREATE_VC (*CO_CREATE_VC_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CO_DELETE_VC)
NDIS_STATUS
(PROTOCOL_CO_DELETE_VC)(
    _In_  NDIS_HANDLE             ProtocolVcContext
    );

typedef PROTOCOL_CO_DELETE_VC (*CO_DELETE_VC_HANDLER);

typedef
_IRQL_requires_(PASSIVE_LEVEL)
_Function_class_(PROTOCOL_CO_AF_REGISTER_NOTIFY)
VOID
(PROTOCOL_CO_AF_REGISTER_NOTIFY)(
    _In_  NDIS_HANDLE             ProtocolBindingContext,
    _In_  PCO_ADDRESS_FAMILY      AddressFamily
    );
typedef PROTOCOL_CO_AF_REGISTER_NOTIFY (*CO_AF_REGISTER_NOTIFY_HANDLER);

// Compatibility for a legacy spelling error
typedef PROTOCOL_CO_AF_REGISTER_NOTIFY PROTCOL_CO_AF_REGISTER_NOTIFY;


#if NDIS_LEGACY_PROTOCOL

typedef struct _NDIS50_PROTOCOL_CHARACTERISTICS
{
    union
    {
        NDIS40_PROTOCOL_CHARACTERISTICS Ndis40Chars;

        struct
        {
            UCHAR                           MajorNdisVersion;
            UCHAR                           MinorNdisVersion;
            USHORT                          Filler;
            union
            {
                UINT                        Reserved;
                UINT                        Flags;
            };
            OPEN_ADAPTER_COMPLETE_HANDLER   OpenAdapterCompleteHandler;
            CLOSE_ADAPTER_COMPLETE_HANDLER  CloseAdapterCompleteHandler;
            union
            {
                SEND_COMPLETE_HANDLER       SendCompleteHandler;
                WAN_SEND_COMPLETE_HANDLER   WanSendCompleteHandler;
            };
            union
            {
             TRANSFER_DATA_COMPLETE_HANDLER TransferDataCompleteHandler;
             WAN_TRANSFER_DATA_COMPLETE_HANDLER WanTransferDataCompleteHandler;
            };
            
            RESET_COMPLETE_HANDLER          ResetCompleteHandler;
            REQUEST_COMPLETE_HANDLER        RequestCompleteHandler;
            union
            {
                RECEIVE_HANDLER             ReceiveHandler;
                WAN_RECEIVE_HANDLER         WanReceiveHandler;
            };
            RECEIVE_COMPLETE_HANDLER        ReceiveCompleteHandler;
            STATUS_HANDLER                  StatusHandler;
            STATUS_COMPLETE_HANDLER         StatusCompleteHandler;
            NDIS_STRING                     Name;
            
            //
            // Start of NDIS 4.0 extensions.
            //
            RECEIVE_PACKET_HANDLER          ReceivePacketHandler;
            
            //
            // PnP protocol entry-points
            //
            BIND_HANDLER                    BindAdapterHandler;
            UNBIND_HANDLER                  UnbindAdapterHandler;
            PNP_EVENT_HANDLER               PnPEventHandler;
            UNLOAD_PROTOCOL_HANDLER         UnloadHandler;

        };
    };

    //
    // Placeholders for protocol extensions for PnP/PM etc.
    //
    PVOID                           ReservedHandlers[4];

    //
    // Start of NDIS 5.0 extensions.
    //

    CO_SEND_COMPLETE_HANDLER        CoSendCompleteHandler;
    CO_STATUS_HANDLER               CoStatusHandler;
    CO_RECEIVE_PACKET_HANDLER       CoReceivePacketHandler;
    CO_AF_REGISTER_NOTIFY_HANDLER   CoAfRegisterNotifyHandler;

} NDIS50_PROTOCOL_CHARACTERISTICS;

#endif // NDIS_LEGACY_PROTOCOL

#if NDIS_SUPPORT_NDIS6

//
// CONDIS 6.0 protocol's entry points
//

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CO_RECEIVE_NET_BUFFER_LISTS)
VOID
(PROTOCOL_CO_RECEIVE_NET_BUFFER_LISTS)(
    _In_  NDIS_HANDLE             ProtocolBindingContext,
    _In_  NDIS_HANDLE             ProtocolVcContext,
    _In_  PNET_BUFFER_LIST        NetBufferLists,
    _In_  ULONG                   NumberOfNetBufferLists,
    _In_  ULONG                   ReceiveFlags
    );

typedef PROTOCOL_CO_RECEIVE_NET_BUFFER_LISTS (*CO_RECEIVE_NET_BUFFER_LISTS_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CO_SEND_NET_BUFFER_LISTS_COMPLETE)
VOID
(PROTOCOL_CO_SEND_NET_BUFFER_LISTS_COMPLETE)(
    _In_  NDIS_HANDLE             ProtocolVcContext,
    _In_  PNET_BUFFER_LIST        NetBufferLists,
    _In_  ULONG                   SendCompleteFlags
    );

typedef PROTOCOL_CO_SEND_NET_BUFFER_LISTS_COMPLETE (*CO_SEND_NET_BUFFER_LISTS_COMPLETE_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CO_STATUS_EX)
VOID
(PROTOCOL_CO_STATUS_EX)(
    _In_ NDIS_HANDLE              ProtocolBindingContext,
    _In_ NDIS_HANDLE              ProtocolVcContext,
    _In_ PNDIS_STATUS_INDICATION  StatusIndication
    );

typedef PROTOCOL_CO_STATUS_EX (*CO_STATUS_HANDLER_EX);

//
// CoNDIS 6 Client handler
//
typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CL_NOTIFY_CLOSE_AF)
NDIS_STATUS
(PROTOCOL_CL_NOTIFY_CLOSE_AF)(
    _In_ NDIS_HANDLE            ClientAfContext
    );

typedef PROTOCOL_CL_NOTIFY_CLOSE_AF (*CL_NOTIFY_CLOSE_AF_HANDLER);

typedef
_IRQL_requires_(PASSIVE_LEVEL)
_Function_class_(PROTOCOL_CL_OPEN_AF_COMPLETE_EX)
VOID
(PROTOCOL_CL_OPEN_AF_COMPLETE_EX)(
    _In_  NDIS_HANDLE             ProtocolAfContext,
    _In_  NDIS_HANDLE             NdisAfHandle,
    _In_  NDIS_STATUS             Status
    );

typedef PROTOCOL_CL_OPEN_AF_COMPLETE_EX (*CL_OPEN_AF_COMPLETE_HANDLER_EX);

//
// CoNDIS 6 Call manager handler
//

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CM_NOTIFY_CLOSE_AF_COMPLETE)
VOID
(PROTOCOL_CM_NOTIFY_CLOSE_AF_COMPLETE)(
    _In_ NDIS_HANDLE           CallMgrAfContext,
    _In_ NDIS_STATUS           Status
    );

typedef PROTOCOL_CM_NOTIFY_CLOSE_AF_COMPLETE (*CM_NOTIFY_CLOSE_AF_COMPLETE_HANDLER);

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisDeregisterProtocolDriver(
    _In_  NDIS_HANDLE             NdisProtocolHandle
    );


#endif // NDIS_SUPPORT_NDIS6


#if NDIS_LEGACY_PROTOCOL

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisDeregisterProtocol(
    _Out_   PNDIS_STATUS            Status,
    _In_    NDIS_HANDLE             NdisProtocolHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisOpenAdapter(
    _At_(*Status, _Must_inspect_result_)
    _Out_               PNDIS_STATUS            Status,
    _Out_               PNDIS_STATUS            OpenErrorStatus,
    _Out_               PNDIS_HANDLE            NdisBindingHandle,
    _Out_               PUINT                   SelectedMediumIndex,
    _In_reads_(MediumArraySize) PNDIS_MEDIUM   MediumArray,
    _In_                UINT                    MediumArraySize,
    _In_                NDIS_HANDLE             NdisProtocolHandle,
    _In_                NDIS_HANDLE             ProtocolBindingContext,
    _In_                PNDIS_STRING            AdapterName,
    _In_                UINT                    OpenOptions,
    _In_opt_            PSTRING                 AddressingInformation
    );


_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisCloseAdapter(
    _Out_   PNDIS_STATUS            Status,
    _In_    NDIS_HANDLE             NdisBindingHandle
    );


_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisCompleteBindAdapter(
    _In_    NDIS_HANDLE            BindAdapterContext,
    _In_    NDIS_STATUS            Status,
    _In_    NDIS_STATUS            OpenStatus
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisCompleteUnbindAdapter(
    _In_    NDIS_HANDLE            UnbindAdapterContext,
    _In_    NDIS_STATUS            Status
    );

EXPORT
VOID
NdisSetProtocolFilter(
    _At_(*Status, _Must_inspect_result_)
    _Out_   PNDIS_STATUS            Status,
    _In_    NDIS_HANDLE             NdisBindingHandle,
    _In_    RECEIVE_HANDLER         ReceiveHandler,
    _In_    RECEIVE_PACKET_HANDLER  ReceivePacketHandler,
    _In_    NDIS_MEDIUM             Medium,
    _In_    UINT                    Offset,
    _In_    UINT                    Size,
    _In_    PUCHAR                  Pattern
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisGetDriverHandle(
    _In_    NDIS_HANDLE             NdisBindingHandle,
    _Out_   PNDIS_HANDLE            NdisDriverHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisOpenProtocolConfiguration(
    _Out_   PNDIS_STATUS            Status,
    _Out_   PNDIS_HANDLE            ConfigurationHandle,
    _In_    PCNDIS_STRING           ProtocolSection
    );

#endif // NDIS_LEGACY_PROTOCOL

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisReEnumerateProtocolBindings(
    _In_    NDIS_HANDLE             NdisProtocolHandle
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisWriteEventLogEntry(
    _In_     _Points_to_data_
             PVOID                               LogHandle,
    _In_     NDIS_STATUS                         EventCode,
    _In_     ULONG                               UniqueEventValue,
    _In_     USHORT                              NumStrings,
    _In_opt_ PVOID                               StringsList,
    _In_     ULONG                               DataSize,
    _In_reads_bytes_opt_(DataSize)
             PVOID                               Data
    );

#if NDIS_LEGACY_PROTOCOL

//
//  The following routine is used by transports to complete pending
//  network PnP events.
//
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisCompletePnPEvent(
    _In_    NDIS_STATUS     Status,
    _In_    NDIS_HANDLE     NdisBindingHandle,
    _In_    PNET_PNP_EVENT  NetPnPEvent
    );

#endif // NDIS_LEGACY_PROTOCOL

#if NDIS_SUPPORT_NDIS6
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisCompleteNetPnPEvent(
    _In_    NDIS_HANDLE                   NdisBindingHandle,
    _In_    PNET_PNP_EVENT_NOTIFICATION   NetPnPEventNotification,
    _In_    NDIS_STATUS                   Status
    );
#endif //  NDIS_SUPPORT_NDIS6

//
//  The following routine is used by a transport to query the localized
//  friendly instance name of the adapter that they are bound to. There
//  are two variations of this, one uses the binding handle and the other
//  the binding context. Some transports need this before they bind - like
//  TCP/IP for instance.
//

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisQueryAdapterInstanceName(
    _Out_  PNDIS_STRING    pAdapterInstanceName,
    _In_   NDIS_HANDLE     NdisBindingHandle
    );

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisQueryBindInstanceName(
    _Out_ PNDIS_STRING    pAdapterInstanceName,
    _In_  NDIS_HANDLE     BindingContext
    );

//
// The following is used by TDI/NDIS interface as part of Network PnP.
// For use by TDI alone.
//
typedef
NTSTATUS
(*TDI_REGISTER_CALLBACK)(
    _In_  PUNICODE_STRING         DeviceName,
    _Out_ HANDLE  *               TdiHandle
    );

typedef
NTSTATUS
(*TDI_PNP_HANDLER)(
    _In_  PUNICODE_STRING         UpperComponent,
    _In_  PUNICODE_STRING         LowerComponent,
    _In_  PUNICODE_STRING         BindList,
    _In_  PVOID                   ReconfigBuffer,
    _In_  UINT                    ReconfigBufferSize,
    _In_  UINT                    Operation
    );

EXPORT
VOID
NdisRegisterTdiCallBack(
    _In_  TDI_REGISTER_CALLBACK   RegisterCallback,
    _In_  TDI_PNP_HANDLER         PnPHandler
    );

EXPORT
VOID
NdisDeregisterTdiCallBack(
    VOID
    );

#if NDIS_LEGACY_PROTOCOL

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisReset(
    _Out_  PNDIS_STATUS            Status,
    _In_   NDIS_HANDLE             NdisBindingHandle
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisRequest(
    _Out_   PNDIS_STATUS            Status,
    _In_    NDIS_HANDLE             NdisBindingHandle,
    _In_    PNDIS_REQUEST           NdisRequest
    );

#ifdef __cplusplus

#define NdisSend(Status, NdisBindingHandle, Packet)                                     \
{                                                                                       \
    *(Status) =                                                                         \
        (((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->NdisCommonOpenBlock.SendHandler)(     \
            ((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->NdisCommonOpenBlock.BindingHandle, \
        (Packet));                                                                      \
}

#define NdisSendPackets(NdisBindingHandle, PacketArray, NumberOfPackets)                \
{                                                                                       \
    (((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->NdisCommonOpenBlock.SendPacketsHandler)(  \
        (PNDIS_OPEN_BLOCK)(NdisBindingHandle),                                          \
        (PacketArray),                                                                  \
        (NumberOfPackets));                                                             \
}


#define NdisTransferData(Status,                                                        \
                         NdisBindingHandle,                                             \
                         MacReceiveContext,                                             \
                         ByteOffset,                                                    \
                         BytesToTransfer,                                               \
                         Packet,                                                        \
                         BytesTransferred)                                              \
{                                                                                       \
    *(Status) =                                                                         \
        (((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->NdisCommonOpenBlock.TransferDataHandler)( \
            ((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->NdisCommonOpenBlock.BindingHandle, \
            (MacReceiveContext),                                                        \
            (ByteOffset),                                                               \
            (BytesToTransfer),                                                          \
            (Packet),                                                                   \
            (BytesTransferred));                                                        \
}

#else

#define NdisSend(Status, NdisBindingHandle, Packet)                         \
{                                                                           \
    *(Status) =                                                             \
        (((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->SendHandler)(             \
            ((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->BindingHandle,         \
        (Packet));                                                          \
}


#define NdisSendPackets(NdisBindingHandle, PacketArray, NumberOfPackets)    \
{                                                                           \
    (((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->SendPacketsHandler)(          \
        (PNDIS_OPEN_BLOCK)(NdisBindingHandle),                              \
        (PacketArray),                                                      \
        (NumberOfPackets));                                                 \
}

#define NdisTransferData(Status,                                            \
                         NdisBindingHandle,                                 \
                         MacReceiveContext,                                 \
                         ByteOffset,                                        \
                         BytesToTransfer,                                   \
                         Packet,                                            \
                         BytesTransferred)                                  \
{                                                                           \
    *(Status) =                                                             \
        (((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->TransferDataHandler)(     \
            ((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->BindingHandle,         \
            (MacReceiveContext),                                            \
            (ByteOffset),                                                   \
            (BytesToTransfer),                                              \
            (Packet),                                                       \
            (BytesTransferred));                                            \
}

#endif // ifdef __cplusplus
#endif // NDIS_LEGACY_PROTOCOL

#if NDIS_LEGACY_PROTOCOL
//
// Routines to access packet flags
//

/*++

VOID
NdisSetSendFlags(
    IN  PNDIS_PACKET            Packet,
    IN  UINT                    Flags
    );

--*/

#define NdisSetSendFlags(_Packet,_Flags)    (_Packet)->Private.Flags = (_Flags)

/*++

VOID
NdisQuerySendFlags(
    IN  PNDIS_PACKET            Packet,
    OUT PUINT                   Flags
    );

--*/

#define NdisQuerySendFlags(_Packet,_Flags)  *(_Flags) = (_Packet)->Private.Flags

#endif // NDIS_LEGACY_PROTOCOL

#if NDIS_LEGACY_DRIVER
//
// The following is the minimum size of packets a miniport must allocate
// when it indicates packets via NdisMIndicatePacket or NdisMCoIndicatePacket
//
#define PROTOCOL_RESERVED_SIZE_IN_PACKET    (4 * sizeof(PVOID))
#endif // NDIS_LEGACY_DRIVER


#ifdef __cplusplus
#define WanMiniportSend(Status,                                                         \
                        NdisBindingHandle,                                              \
                        NdisLinkHandle,                                                 \
                        WanPacket)                                                      \
{                                                                                       \
    *(Status) =                                                                         \
        ((((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->NdisCommonOpenBlock.WanSendHandler))( \
            ((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->NdisCommonOpenBlock.BindingHandle, \
            (NdisLinkHandle),                                                           \
            (PNDIS_PACKET)(WanPacket));                                                 \
}

#else
#define WanMiniportSend(Status,                                             \
                        NdisBindingHandle,                                  \
                        NdisLinkHandle,                                     \
                        WanPacket)                                          \
{                                                                           \
    *(Status) =                                                             \
        ((((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->WanSendHandler))(        \
            ((PNDIS_OPEN_BLOCK)(NdisBindingHandle))->BindingHandle,         \
            (NdisLinkHandle),                                               \
            (PNDIS_PACKET)(WanPacket));                                     \
}

#endif


#if NDIS_LEGACY_PROTOCOL

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisReturnPackets(
    _In_reads_(NumberOfPackets) PNDIS_PACKET *PacketsToReturn,
    _In_    UINT                               NumberOfPackets
    );

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
PNDIS_PACKET
NdisGetReceivedPacket(
    _In_    NDIS_HANDLE             NdisBindingHandle,
    _In_    NDIS_HANDLE             MacContext
    );

#endif // NDIS_LEGACY_PROTOCOL

//
// Macros to portably manipulate NDIS buffers.
//

#define NdisBufferLength(Buffer)                            MmGetMdlByteCount(Buffer)
#define NdisBufferVirtualAddress(_Buffer)                   MmGetSystemAddressForMdl(_Buffer)
#define NdisBufferVirtualAddressSafe(_Buffer, _Priority)    MmGetSystemAddressForMdlSafe(_Buffer, _Priority)



#if NDIS_LEGACY_PROTOCOL

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCancelSendPackets(
    _In_  NDIS_HANDLE     NdisBindingHandle,
    _In_ _Points_to_data_
          PVOID           CancelId
    );

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisQueryPendingIOCount(
    _In_ _Points_to_data_
               PVOID       NdisBindingHandle,
    _Out_    PULONG      IoCount
    );

#endif // NDIS_LEGACY_PROTOCOL

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
UCHAR
NdisGeneratePartialCancelId(
    VOID
    );


#if NDIS_SUPPORT_NDIS6

//
// NDIS 6.0 protocol's entry points
//
typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_OID_REQUEST_COMPLETE)
VOID
(PROTOCOL_OID_REQUEST_COMPLETE)(
    _In_  NDIS_HANDLE             ProtocolBindingContext,
    _In_  PNDIS_OID_REQUEST       OidRequest,
    _In_  NDIS_STATUS             Status
    );

typedef PROTOCOL_OID_REQUEST_COMPLETE (*OID_REQUEST_COMPLETE_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_RECEIVE_NET_BUFFER_LISTS)
VOID
(PROTOCOL_RECEIVE_NET_BUFFER_LISTS)(
    _In_  NDIS_HANDLE             ProtocolBindingContext,
    _In_  PNET_BUFFER_LIST        NetBufferLists,
    _In_  NDIS_PORT_NUMBER        PortNumber,
    _In_  ULONG                   NumberOfNetBufferLists,
    _In_  ULONG                   ReceiveFlags
    );

typedef PROTOCOL_RECEIVE_NET_BUFFER_LISTS (*RECEIVE_NET_BUFFER_LISTS_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_SEND_NET_BUFFER_LISTS_COMPLETE)
VOID
(PROTOCOL_SEND_NET_BUFFER_LISTS_COMPLETE)(
    _In_  NDIS_HANDLE             ProtocolBindingContext,
    _In_  PNET_BUFFER_LIST        NetBufferList,
    _In_  ULONG                   SendCompleteFlags
    );

typedef PROTOCOL_SEND_NET_BUFFER_LISTS_COMPLETE (*SEND_NET_BUFFER_LISTS_COMPLETE_HANDLER);

#if (NDIS_SUPPORT_NDIS61)
//
// NDIS 6.1 protocol's entry points
//
typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_DIRECT_OID_REQUEST_COMPLETE)
VOID
(PROTOCOL_DIRECT_OID_REQUEST_COMPLETE)(
    _In_  NDIS_HANDLE             ProtocolBindingContext,
    _In_  PNDIS_OID_REQUEST       OidRequest,
    _In_  NDIS_STATUS             Status
    );

typedef PROTOCOL_DIRECT_OID_REQUEST_COMPLETE (*DIRECT_OID_REQUEST_COMPLETE_HANDLER);

#endif // (NDIS_SUPPORT_NDIS61)

//
// structure passed to protocol's BIND_HANDLER_EX
//
#define NDIS_BIND_PARAMETERS_REVISION_1     1

#if (NDIS_SUPPORT_NDIS61)
#define NDIS_BIND_PARAMETERS_REVISION_2     2
#endif // (NDIS_SUPPORT_NDIS61)

#if (NDIS_SUPPORT_NDIS620)
#define NDIS_BIND_PARAMETERS_REVISION_3     3
#endif // (NDIS_SUPPORT_NDIS620)

#if (NDIS_SUPPORT_NDIS630)
#define NDIS_BIND_PARAMETERS_REVISION_4     4
#endif // (NDIS_SUPPORT_NDIS630)

typedef struct _NDIS_BIND_PARAMETERS
{
    NDIS_OBJECT_HEADER              Header;
    PNDIS_STRING                    ProtocolSection;
    PNDIS_STRING                    AdapterName;
    PDEVICE_OBJECT                  PhysicalDeviceObject;
    NDIS_MEDIUM                     MediaType;
    ULONG                           MtuSize;
    ULONG64                         MaxXmitLinkSpeed;
    ULONG64                         XmitLinkSpeed;
    ULONG64                         MaxRcvLinkSpeed;
    ULONG64                         RcvLinkSpeed;
    NDIS_MEDIA_CONNECT_STATE        MediaConnectState;
    NDIS_MEDIA_DUPLEX_STATE         MediaDuplexState;
    ULONG                           LookaheadSize;
    PNDIS_PNP_CAPABILITIES          PowerManagementCapabilities; // 6.20 drivers must use PowerManagementCapabilitiesEx
    ULONG                           SupportedPacketFilters;
    ULONG                           MaxMulticastListSize;
    USHORT                          MacAddressLength;
    UCHAR                           CurrentMacAddress[NDIS_MAX_PHYS_ADDRESS_LENGTH];
    NDIS_PHYSICAL_MEDIUM            PhysicalMediumType;
    PNDIS_RECEIVE_SCALE_CAPABILITIES RcvScaleCapabilities;
    NET_LUID                        BoundIfNetluid;
    NET_IFINDEX                     BoundIfIndex;
    NET_LUID                        LowestIfNetluid;
    NET_IFINDEX                     LowestIfIndex;
    NET_IF_ACCESS_TYPE              AccessType; // NET_IF_ACCESS_BROADCAST for a typical ethernet adapter
    NET_IF_DIRECTION_TYPE           DirectionType; // NET_IF_DIRECTION_SENDRECEIVE for a typical ethernet adapter
    NET_IF_CONNECTION_TYPE          ConnectionType; // NET_IF_CONNECTION_DEDICATED for a typical ethernet adapter
    NET_IFTYPE                      IfType; // IF_TYPE_ETHERNET_CSMACD for a typical ethernet adapter (regardless of speed)
    BOOLEAN                         IfConnectorPresent; // RFC 2665 TRUE if physical adapter
    PNDIS_PORT                      ActivePorts;
    ULONG                           DataBackFillSize;
    ULONG                           ContextBackFillSize;
    ULONG                           MacOptions;
    NET_IF_COMPARTMENT_ID           CompartmentId;
    PNDIS_OFFLOAD                   DefaultOffloadConfiguration;
    PNDIS_TCP_CONNECTION_OFFLOAD    TcpConnectionOffloadCapabilities;
    PNDIS_STRING                    BoundAdapterName;
#if (NDIS_SUPPORT_NDIS61)
    PNDIS_HD_SPLIT_CURRENT_CONFIG   HDSplitCurrentConfig;
#endif // (NDIS_SUPPORT_NDIS61)
#if (NDIS_SUPPORT_NDIS620)
    PNDIS_RECEIVE_FILTER_CAPABILITIES ReceiveFilterCapabilities;
    PNDIS_PM_CAPABILITIES           PowerManagementCapabilitiesEx;
    PNDIS_NIC_SWITCH_CAPABILITIES   NicSwitchCapabilities;
#endif // (NDIS_SUPPORT_NDIS620)
#if (NDIS_SUPPORT_NDIS630)
    BOOLEAN                         NDKEnabled;
    PNDIS_NDK_CAPABILITIES          NDKCapabilities;
    PNDIS_SRIOV_CAPABILITIES        SriovCapabilities;
    PNDIS_NIC_SWITCH_INFO_ARRAY     NicSwitchArray;
#endif // (NDIS_SUPPORT_NDIS630)
}NDIS_BIND_PARAMETERS, *PNDIS_BIND_PARAMETERS;

#define NDIS_SIZEOF_BIND_PARAMETERS_REVISION_1     \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_BIND_PARAMETERS, BoundAdapterName)

#if (NDIS_SUPPORT_NDIS61)
#define NDIS_SIZEOF_BIND_PARAMETERS_REVISION_2     \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_BIND_PARAMETERS, HDSplitCurrentConfig)
#endif // (NDIS_SUPPORT_NDIS61)

#if (NDIS_SUPPORT_NDIS620)
#define NDIS_SIZEOF_BIND_PARAMETERS_REVISION_3     \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_BIND_PARAMETERS, NicSwitchCapabilities)
#endif // (NDIS_SUPPORT_NDIS620)

#if (NDIS_SUPPORT_NDIS630)
#define NDIS_SIZEOF_BIND_PARAMETERS_REVISION_4     \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_BIND_PARAMETERS, NicSwitchArray)
#endif // (NDIS_SUPPORT_NDIS630)

typedef
_IRQL_requires_(PASSIVE_LEVEL)
_Function_class_(PROTOCOL_BIND_ADAPTER_EX)
NDIS_STATUS
(PROTOCOL_BIND_ADAPTER_EX)(
    _In_  NDIS_HANDLE             ProtocolDriverContext,
    _In_  NDIS_HANDLE             BindContext,
    _In_  PNDIS_BIND_PARAMETERS   BindParameters
    );

typedef PROTOCOL_BIND_ADAPTER_EX (*BIND_HANDLER_EX);

typedef
_IRQL_requires_(PASSIVE_LEVEL)
_Function_class_(PROTOCOL_UNBIND_ADAPTER_EX)
NDIS_STATUS
(PROTOCOL_UNBIND_ADAPTER_EX)(
    _In_  NDIS_HANDLE             UnbindContext,
    _In_  NDIS_HANDLE             ProtocolBindingContext
    );

typedef PROTOCOL_UNBIND_ADAPTER_EX (*UNBIND_HANDLER_EX);

typedef
_IRQL_requires_(PASSIVE_LEVEL)
_Function_class_(PROTOCOL_OPEN_ADAPTER_COMPLETE_EX)
VOID
(PROTOCOL_OPEN_ADAPTER_COMPLETE_EX)(
    _In_  NDIS_HANDLE             ProtocolBindingContext,
    _In_  NDIS_STATUS             Status
    );

typedef PROTOCOL_OPEN_ADAPTER_COMPLETE_EX (*OPEN_ADAPTER_COMPLETE_HANDLER_EX);

typedef
_IRQL_requires_(PASSIVE_LEVEL)
_Function_class_(PROTOCOL_CLOSE_ADAPTER_COMPLETE_EX)
VOID
(PROTOCOL_CLOSE_ADAPTER_COMPLETE_EX)(
    _In_  NDIS_HANDLE             ProtocolBindingContext
    );

typedef PROTOCOL_CLOSE_ADAPTER_COMPLETE_EX (*CLOSE_ADAPTER_COMPLETE_HANDLER_EX);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_STATUS_EX)
VOID
(PROTOCOL_STATUS_EX)(
    _In_  NDIS_HANDLE             ProtocolBindingContext,
    _In_  PNDIS_STATUS_INDICATION StatusIndication
    );

typedef PROTOCOL_STATUS_EX (*STATUS_HANDLER_EX);

typedef
_IRQL_requires_(PASSIVE_LEVEL)
_Function_class_(PROTOCOL_NET_PNP_EVENT)
NDIS_STATUS
(PROTOCOL_NET_PNP_EVENT)(
    _In_  NDIS_HANDLE                 ProtocolBindingContext,
    _In_  PNET_PNP_EVENT_NOTIFICATION NetPnPEventNotification
    );

typedef PROTOCOL_NET_PNP_EVENT (*NET_PNP_EVENT_HANDLER);

typedef
_IRQL_requires_(PASSIVE_LEVEL)
_Function_class_(PROTOCOL_UNINSTALL)
VOID
(PROTOCOL_UNINSTALL)(
    VOID
    );
typedef PROTOCOL_UNINSTALL (*UNINSTALL_PROTOCOL_HANDLER);


#define NDIS_PROTOCOL_CO_CHARACTERISTICS_REVISION_1     1

typedef struct _NDIS_PROTOCOL_CO_CHARACTERISTICS
{
    NDIS_OBJECT_HEADER                      Header;     // Header.Type = NDIS_OBJECT_TYPE_PROTOCOL_CO_CHARACTERISTICS
    ULONG                                   Flags;
    CO_STATUS_HANDLER_EX                    CoStatusHandlerEx;
    CO_AF_REGISTER_NOTIFY_HANDLER           CoAfRegisterNotifyHandler;
    CO_RECEIVE_NET_BUFFER_LISTS_HANDLER     CoReceiveNetBufferListsHandler;
    CO_SEND_NET_BUFFER_LISTS_COMPLETE_HANDLER CoSendNetBufferListsCompleteHandler;
} NDIS_PROTOCOL_CO_CHARACTERISTICS, *PNDIS_PROTOCOL_CO_CHARACTERISTICS;

#define NDIS_SIZEOF_PROTOCOL_CO_CHARACTERISTICS_REVISION_1    \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_PROTOCOL_CO_CHARACTERISTICS, CoSendNetBufferListsCompleteHandler)

#define NDIS_PROTOCOL_DRIVER_CHARACTERISTICS_REVISION_1     1
#if (NDIS_SUPPORT_NDIS61)
#define NDIS_PROTOCOL_DRIVER_CHARACTERISTICS_REVISION_2     2
#endif // (NDIS_SUPPORT_NDIS61)

typedef struct _NDIS_PROTOCOL_DRIVER_CHARACTERISTICS
{
    NDIS_OBJECT_HEADER                      Header;
    UCHAR                                   MajorNdisVersion;
    UCHAR                                   MinorNdisVersion;
    UCHAR                                   MajorDriverVersion;
    UCHAR                                   MinorDriverVersion;
    ULONG                                   Flags;
    NDIS_STRING                             Name;
    SET_OPTIONS_HANDLER                     SetOptionsHandler;
    BIND_HANDLER_EX                         BindAdapterHandlerEx;
    UNBIND_HANDLER_EX                       UnbindAdapterHandlerEx;
    OPEN_ADAPTER_COMPLETE_HANDLER_EX        OpenAdapterCompleteHandlerEx;
    CLOSE_ADAPTER_COMPLETE_HANDLER_EX       CloseAdapterCompleteHandlerEx;
    NET_PNP_EVENT_HANDLER                   NetPnPEventHandler;
    UNINSTALL_PROTOCOL_HANDLER              UninstallHandler;
    OID_REQUEST_COMPLETE_HANDLER            OidRequestCompleteHandler;
    STATUS_HANDLER_EX                       StatusHandlerEx;
    RECEIVE_NET_BUFFER_LISTS_HANDLER        ReceiveNetBufferListsHandler;
    SEND_NET_BUFFER_LISTS_COMPLETE_HANDLER  SendNetBufferListsCompleteHandler;
#if (NDIS_SUPPORT_NDIS61)
    DIRECT_OID_REQUEST_COMPLETE_HANDLER     DirectOidRequestCompleteHandler;
#endif // (NDIS_SUPPORT_NDIS61)
} NDIS_PROTOCOL_DRIVER_CHARACTERISTICS, *PNDIS_PROTOCOL_DRIVER_CHARACTERISTICS;

#define NDIS_SIZEOF_PROTOCOL_DRIVER_CHARACTERISTICS_REVISION_1    \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_PROTOCOL_DRIVER_CHARACTERISTICS, SendNetBufferListsCompleteHandler)

#if (NDIS_SUPPORT_NDIS61)
#define NDIS_SIZEOF_PROTOCOL_DRIVER_CHARACTERISTICS_REVISION_2    \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_PROTOCOL_DRIVER_CHARACTERISTICS, DirectOidRequestCompleteHandler)
#endif // (NDIS_SUPPORT_NDIS61)

#endif // NDIS_SUPPORT_NDIS6


#if NDIS_LEGACY_PROTOCOL

#if (defined(NDIS50) || defined(NDIS51))
typedef NDIS50_PROTOCOL_CHARACTERISTICS  NDIS_PROTOCOL_CHARACTERISTICS;
#else
typedef NDIS40_PROTOCOL_CHARACTERISTICS  NDIS_PROTOCOL_CHARACTERISTICS;
#endif


typedef NDIS_PROTOCOL_CHARACTERISTICS *PNDIS_PROTOCOL_CHARACTERISTICS;

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisRegisterProtocol(
    _Out_   PNDIS_STATUS                      Status,
    _Out_   PNDIS_HANDLE                      NdisProtocolHandle,
    _In_    PNDIS_PROTOCOL_CHARACTERISTICS    ProtocolCharacteristics,
    _In_    UINT                              CharacteristicsLength
    );

#endif // NDIS_LEGACY_PROTOCOL

#if NDIS_SUPPORT_NDIS6

_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisRegisterProtocolDriver(
    _In_opt_  NDIS_HANDLE             ProtocolDriverContext,
    _In_      PNDIS_PROTOCOL_DRIVER_CHARACTERISTICS ProtocolCharacteristics,
    _Out_     PNDIS_HANDLE            NdisProtocolHandle
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisSendNetBufferLists(
    _In_                            NDIS_HANDLE             NdisBindingHandle,
    _In_ __drv_aliasesMem           PNET_BUFFER_LIST        NetBufferLists,
    _In_                            NDIS_PORT_NUMBER        PortNumber,
    _In_                            ULONG                   SendFlags
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisReturnNetBufferLists(
    _In_    NDIS_HANDLE             NdisBindingHandle,
    _In_    PNET_BUFFER_LIST        NetBufferLists,
    _In_    ULONG                   ReturnFlags
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCancelSendNetBufferLists(
    _In_  NDIS_HANDLE     NdisBindingHandle,
    _In_ _Points_to_data_
          PVOID           CancelId
    );

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisOidRequest(
    _In_    NDIS_HANDLE             NdisBindingHandle,
    _In_    PNDIS_OID_REQUEST       OidRequest
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCancelOidRequest(
    _In_    NDIS_HANDLE             NdisBindingHandle,
    _In_ _Points_to_data_
            PVOID                   RequestId
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisUnbindAdapter(
    _In_    NDIS_HANDLE                     NdisBindingHandle
    );


//
// structure passed to NdisOpenAdapterEx
//

#define NDIS_OPEN_PARAMETERS_REVISION_1     1
typedef struct _NDIS_OPEN_PARAMETERS
{
    NDIS_OBJECT_HEADER          Header;
    PNDIS_STRING                AdapterName;
    PNDIS_MEDIUM                MediumArray;
    UINT                        MediumArraySize;
    PUINT                       SelectedMediumIndex;
    PNET_FRAME_TYPE             FrameTypeArray;
    UINT                        FrameTypeArraySize;
} NDIS_OPEN_PARAMETERS, *PNDIS_OPEN_PARAMETERS;

#define NDIS_SIZEOF_OPEN_PARAMETERS_REVISION_1    \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_OPEN_PARAMETERS, FrameTypeArraySize)

#define NDIS_SIZEOF_OPEN_PARAMETERS_REVSION_1    \
        NDIS_SIZEOF_OPEN_PARAMETERS_REVISION_1   // Legacy spelling error

_At_(ProtocolBindingContext, __drv_aliasesMem)
_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisOpenAdapterEx(
    _In_    NDIS_HANDLE             NdisProtocolHandle,
    _In_    NDIS_HANDLE             ProtocolBindingContext,
    _In_    PNDIS_OPEN_PARAMETERS   OpenParameters,
    _In_    NDIS_HANDLE             BindContext,
    _Out_   PNDIS_HANDLE            NdisBindingHandle
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCompleteBindAdapterEx(
    _In_    NDIS_HANDLE             BindAdapterContext,
    _In_    NDIS_STATUS             Status
    );

_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisCloseAdapterEx(
    _In_    NDIS_HANDLE             NdisBindingHandle
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCompleteUnbindAdapterEx(
    _In_    NDIS_HANDLE             UnbindContext
    );



#define NDIS_PROTOCOL_PAUSE_PARAMETERS_REVISION_1     1

typedef struct _NDIS_PROTOCOL_PAUSE_PARAMETERS
{
    NDIS_OBJECT_HEADER          Header;
    ULONG                       Flags;
    ULONG                       PauseReason;
} NDIS_PROTOCOL_PAUSE_PARAMETERS, *PNDIS_PROTOCOL_PAUSE_PARAMETERS;

#define NDIS_SIZEOF_PROTOCOL_PAUSE_PARAMETERS_REVISION_1    \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_PROTOCOL_PAUSE_PARAMETERS, PauseReason)


//
// NDIS_PROTOCOL_RESTART_PARAMETERS is used in
// NetEventRestart event indication to the protocols
//
#define NDIS_PROTOCOL_RESTART_PARAMETERS_REVISION_1     1

typedef struct _NDIS_PROTOCOL_RESTART_PARAMETERS
{
    NDIS_OBJECT_HEADER          Header;
    PUCHAR                      FilterModuleNameBuffer;
    ULONG                       FilterModuleNameBufferLength;
    PNDIS_RESTART_ATTRIBUTES    RestartAttributes;
    NET_IFINDEX                 BoundIfIndex;
    NET_LUID                    BoundIfNetluid;
    ULONG                       Flags;
} NDIS_PROTOCOL_RESTART_PARAMETERS, *PNDIS_PROTOCOL_RESTART_PARAMETERS;

#define NDIS_SIZEOF_PROTOCOL_RESTART_PARAMETERS_REVISION_1    \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_PROTOCOL_RESTART_PARAMETERS, Flags)


#if (NDIS_SUPPORT_NDIS630)

#define NDIS_BIND_FAILED_NOTIFICATION_REVISION_1 1

typedef struct _NDIS_BIND_FAILED_NOTIFICATION
{
    // Header.Type = NDIS_OBJECT_TYPE_DEFAULT
    // Header.Revision = NDIS_BIND_FAILED_NOTIFICATION_REVISION_1
    // Header.Size >= NDIS_SIZEOF_NDIS_BIND_FAILED_NOTIFICATION_REVISION_1
    NDIS_OBJECT_HEADER          Header;

    NET_LUID                    MiniportNetLuid;
} NDIS_BIND_FAILED_NOTIFICATION, *PNDIS_BIND_FAILED_NOTIFICATION;

#define NDIS_SIZEOF_NDIS_BIND_FAILED_NOTIFICATION_REVISION_1    \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_BIND_FAILED_NOTIFICATION, MiniportNetLuid)


#endif // NDIS_SUPPORT_NDIS630

#if (NDIS_SUPPORT_NDIS61)

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisDirectOidRequest(
    _In_    NDIS_HANDLE             NdisBindingHandle,
    _In_    PNDIS_OID_REQUEST       OidRequest
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCancelDirectOidRequest(
    _In_    NDIS_HANDLE             NdisBindingHandle,
    _In_ _Points_to_data_
            PVOID                   RequestId
    );
#endif // (NDIS_SUPPORT_NDIS61)

#endif // NDIS_SUPPORT_NDIS6

//
// needed for macros used by legacy protocols
//
#if !defined(NDIS_WRAPPER)
#if NDIS_LEGACY_PROTOCOL
    typedef struct _NDIS_COMMON_OPEN_BLOCK
    {
        PVOID                       MacHandle;          // needed for backward compatibility

        NDIS_HANDLE                 BindingHandle;
        NDIS_HANDLE                 Reserved6;
        NDIS_HANDLE                 Reserved7;
        NDIS_HANDLE                 Reserved8;

        PVOID                       Reserved9;
        NDIS_HANDLE                 Reserved10;

        NDIS_HANDLE                 Reserved11;
        BOOLEAN                     Reserved12;
        BOOLEAN                     Reserved2;
        BOOLEAN                     Reserved3;
        BOOLEAN                     Reserved4;
        PVOID                       Reserved13;
        KSPIN_LOCK                  Reserved5;
        NDIS_HANDLE                 Reserved14;

        //
        // These are referenced by the macros used by protocols to call.
        // All of the ones referenced by the macros are internal NDIS handlers for the miniports
        //
        union
        {
            SEND_HANDLER            SendHandler;
            WAN_SEND_HANDLER        WanSendHandler;
        };
        TRANSFER_DATA_HANDLER       TransferDataHandler;

        //
        // These are referenced internally by NDIS
        //
        SEND_COMPLETE_HANDLER       SendCompleteHandler;
        TRANSFER_DATA_COMPLETE_HANDLER TransferDataCompleteHandler;
        RECEIVE_HANDLER             ReceiveHandler;
        RECEIVE_COMPLETE_HANDLER    ReceiveCompleteHandler;
        WAN_RECEIVE_HANDLER         WanReceiveHandler;
        REQUEST_COMPLETE_HANDLER    RequestCompleteHandler;

        //
        // NDIS 4.0 extensions
        //
        RECEIVE_PACKET_HANDLER      ReceivePacketHandler;
        SEND_PACKETS_HANDLER        SendPacketsHandler;

        //
        // More Cached Handlers
        //
        RESET_HANDLER               ResetHandler;
        REQUEST_HANDLER             RequestHandler;
        RESET_COMPLETE_HANDLER      ResetCompleteHandler;
        STATUS_HANDLER              StatusHandler;
        STATUS_COMPLETE_HANDLER     StatusCompleteHandler;

    }NDIS_COMMON_OPEN_BLOCK, *PNDIS_COMMON_OPEN_BLOCK;
    //
    // one of these per open on an adapter/protocol
    //
    struct _NDIS_OPEN_BLOCK
    {
#ifdef __cplusplus
        NDIS_COMMON_OPEN_BLOCK NdisCommonOpenBlock;
#else
        NDIS_COMMON_OPEN_BLOCK;
#endif

    };

#endif // NDIS_LEGACY_PROTOCOL
#endif // NDIS_WRAPPER




//
//    Copyright (C) Microsoft.  All rights reserved.
//
#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

#include <xfilter.h>

#if defined(_MSC_VER)
#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning(disable:4201) /* nonstandard extension used : nameless struct/union */
#pragma warning(disable:4214) /* nonstandard extension used : bit field types other then int */
#endif // defined(_MSC_VER)

#define NDIS_M_MAX_LOOKAHEAD 526

#if NDIS_LEGACY_MINIPORT

//
// Function types for NDIS_MINIPORT_CHARACTERISTICS
//

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
BOOLEAN
(*W_CHECK_FOR_HANG_HANDLER)(
    _In_  NDIS_HANDLE             MiniportAdapterContext
    );

typedef
_IRQL_requires_max_(HIGH_LEVEL)
VOID
(*W_DISABLE_INTERRUPT_HANDLER)(
    _In_  NDIS_HANDLE             MiniportAdapterContext
    );

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
VOID
(*W_ENABLE_INTERRUPT_HANDLER)(
    _In_  NDIS_HANDLE             MiniportAdapterContext
    );

typedef
_IRQL_requires_(PASSIVE_LEVEL)
VOID
(*W_HALT_HANDLER)(
    _In_  NDIS_HANDLE             MiniportAdapterContext
    );

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
VOID
(*W_HANDLE_INTERRUPT_HANDLER)(
    _In_  NDIS_HANDLE             MiniportAdapterContext
    );

typedef
_IRQL_requires_(PASSIVE_LEVEL)
NDIS_STATUS
(*W_INITIALIZE_HANDLER)(
    _Out_ PNDIS_STATUS            OpenErrorStatus,
    _Out_ PUINT                   SelectedMediumIndex,
    _In_   PNDIS_MEDIUM            MediumArray,
    _In_   UINT                    MediumArraySize,
    _In_   NDIS_HANDLE             MiniportAdapterContext,
    _In_   NDIS_HANDLE             WrapperConfigurationContext
    );

typedef
_IRQL_requires_max_(HIGH_LEVEL)
VOID
(*W_ISR_HANDLER)(
    _Out_ PBOOLEAN                InterruptRecognized,
    _Out_ PBOOLEAN                QueueMiniportHandleInterrupt,
    _In_   NDIS_HANDLE             MiniportAdapterContext
    );

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
NDIS_STATUS
(*W_QUERY_INFORMATION_HANDLER)(
    _In_   NDIS_HANDLE             MiniportAdapterContext,
    _In_   NDIS_OID                Oid,
    _In_   PVOID                   InformationBuffer,
    _In_   ULONG                   InformationBufferLength,
    _Out_ PULONG                  BytesWritten,
    _Out_ PULONG                  BytesNeeded
    );

typedef
_IRQL_requires_(PASSIVE_LEVEL)
NDIS_STATUS
(*W_RECONFIGURE_HANDLER)(
    _Out_ PNDIS_STATUS            OpenErrorStatus,
    _In_   NDIS_HANDLE             MiniportAdapterContext  OPTIONAL,
    _In_   NDIS_HANDLE             WrapperConfigurationContext
    );

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
NDIS_STATUS
(*W_RESET_HANDLER)(
    _Out_ PBOOLEAN                AddressingReset,
    _In_   NDIS_HANDLE             MiniportAdapterContext
    );

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
NDIS_STATUS
(*W_SEND_HANDLER)(
    _In_  NDIS_HANDLE             MiniportAdapterContext,
    _In_  PNDIS_PACKET            Packet,
    _In_  UINT                    Flags
    );

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
NDIS_STATUS
(*WM_SEND_HANDLER)(
    _In_  NDIS_HANDLE             MiniportAdapterContext,
    _In_  NDIS_HANDLE             NdisLinkHandle,
    _In_  PNDIS_WAN_PACKET        Packet
    );

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
NDIS_STATUS
(*W_SET_INFORMATION_HANDLER)(
    _In_   NDIS_HANDLE             MiniportAdapterContext,
    _In_   NDIS_OID                Oid,
    _In_   PVOID                   InformationBuffer,
    _In_   ULONG                   InformationBufferLength,
    _Out_ PULONG                  BytesRead,
    _Out_ PULONG                  BytesNeeded
    );

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
NDIS_STATUS
(*W_TRANSFER_DATA_HANDLER)(
    _Out_ PNDIS_PACKET            Packet,
    _Out_ PUINT                   BytesTransferred,
    _In_  NDIS_HANDLE             MiniportAdapterContext,
    _In_  NDIS_HANDLE             MiniportReceiveContext,
    _In_  UINT                    ByteOffset,
    _In_  UINT                    BytesToTransfer
    );

typedef
NDIS_STATUS
(*WM_TRANSFER_DATA_HANDLER)(
    VOID
    );

//
// Definition for shutdown handler
//

typedef
_IRQL_requires_max_(HIGH_LEVEL)
VOID
(*ADAPTER_SHUTDOWN_HANDLER) (
    _In_  PVOID ShutdownContext
    );

//
// Miniport extensions for NDIS 4.0
//
typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
VOID
(*W_RETURN_PACKET_HANDLER)(
    _In_  NDIS_HANDLE             MiniportAdapterContext,
    _In_  PNDIS_PACKET            Packet
    );

//
// NDIS 4.0 extension
//
typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
VOID
(*W_SEND_PACKETS_HANDLER)(
    _In_  NDIS_HANDLE             MiniportAdapterContext,
    _In_  PPNDIS_PACKET           PacketArray,
    _In_  UINT                    NumberOfPackets
    );

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
VOID
(*W_ALLOCATE_COMPLETE_HANDLER)(
    _In_  NDIS_HANDLE             MiniportAdapterContext,
    _In_  PVOID                   VirtualAddress,
    _In_  PNDIS_PHYSICAL_ADDRESS  PhysicalAddress,
    _In_  ULONG                   Length,
    _In_  PVOID                   Context
    );

#endif // NDIS_LEGACY_MINIPORT


//
// W_CO_CREATE_VC_HANDLER is a synchronous call
//
typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(MINIPORT_CO_CREATE_VC)
NDIS_STATUS
(MINIPORT_CO_CREATE_VC)(
    _In_   NDIS_HANDLE             MiniportAdapterContext,
    _In_   NDIS_HANDLE             NdisVcHandle,
    _Out_ PNDIS_HANDLE            MiniportVcContext
    );

typedef MINIPORT_CO_CREATE_VC (*W_CO_CREATE_VC_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(MINIPORT_CO_DELETE_VC)
NDIS_STATUS
(MINIPORT_CO_DELETE_VC)(
    _In_  NDIS_HANDLE             MiniportVcContext
    );

typedef MINIPORT_CO_DELETE_VC (*W_CO_DELETE_VC_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(MINIPORT_CO_ACTIVATE_VC)
NDIS_STATUS
(MINIPORT_CO_ACTIVATE_VC)(
    _In_  NDIS_HANDLE             MiniportVcContext,
    _Inout_ PCO_CALL_PARAMETERS  CallParameters
    );

typedef MINIPORT_CO_ACTIVATE_VC (*W_CO_ACTIVATE_VC_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(MINIPORT_CO_DEACTIVATE_VC)
NDIS_STATUS
(MINIPORT_CO_DEACTIVATE_VC)(
    _In_  NDIS_HANDLE             MiniportVcContext
    );
typedef MINIPORT_CO_DEACTIVATE_VC (*W_CO_DEACTIVATE_VC_HANDLER);

#if NDIS_LEGACY_MINIPORT

typedef
VOID
(*W_CO_SEND_PACKETS_HANDLER)(
    _In_  NDIS_HANDLE             MiniportVcContext,
    _In_  PPNDIS_PACKET           PacketArray,
    _In_  UINT                    NumberOfPackets
    );

typedef
NDIS_STATUS
(*W_CO_REQUEST_HANDLER)(
    _In_      NDIS_HANDLE             MiniportAdapterContext,
    _In_      NDIS_HANDLE             MiniportVcContext   OPTIONAL,
    _Inout_ PNDIS_REQUEST        NdisRequest
    );

#endif // NDIS_LEGACY_MINIPORT


#if NDIS_SUPPORT_NDIS6

//
// CONDIS 6.0 handlers
//

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(MINIPORT_CO_SEND_NET_BUFFER_LISTS)
VOID
(MINIPORT_CO_SEND_NET_BUFFER_LISTS)(
    _In_  NDIS_HANDLE             MiniportVcContext,
    _In_  PNET_BUFFER_LIST        NetBufferLists,
    _In_  ULONG                   SendFlags
    );

typedef MINIPORT_CO_SEND_NET_BUFFER_LISTS (*W_CO_SEND_NET_BUFFER_LISTS_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(MINIPORT_CO_OID_REQUEST)
NDIS_STATUS
(MINIPORT_CO_OID_REQUEST)(
     _In_      NDIS_HANDLE           MiniportAdapterContext,
     _In_      NDIS_HANDLE           MiniportVcContext OPTIONAL,
     _Inout_ PNDIS_OID_REQUEST NdisRequest
    );

typedef MINIPORT_CO_OID_REQUEST (*W_CO_OID_REQUEST_HANDLER);

#endif // NDIS_SUPPORT_NDIS6

typedef enum _NDIS_DEVICE_PNP_EVENT
{
    NdisDevicePnPEventQueryRemoved,
    NdisDevicePnPEventRemoved,
    NdisDevicePnPEventSurpriseRemoved,
    NdisDevicePnPEventQueryStopped,
    NdisDevicePnPEventStopped,
    NdisDevicePnPEventPowerProfileChanged,
#if NDIS_SUPPORT_NDIS6
    NdisDevicePnPEventFilterListChanged,
#endif // NDIS_SUPPORT_NDIS6
    NdisDevicePnPEventMaximum
} NDIS_DEVICE_PNP_EVENT, *PNDIS_DEVICE_PNP_EVENT;

//
// power profiles
//
typedef enum _NDIS_POWER_PROFILE
{
    NdisPowerProfileBattery,
    NdisPowerProfileAcOnLine
} NDIS_POWER_PROFILE, *PNDIS_POWER_PROFILE;

#if NDIS_LEGACY_MINIPORT

typedef struct _NDIS50_MINIPORT_CHARACTERISTICS
{
    UCHAR                       MajorNdisVersion;
    UCHAR                       MinorNdisVersion;
    USHORT                      Filler;
    UINT                        Reserved;
    W_CHECK_FOR_HANG_HANDLER    CheckForHangHandler;
    W_DISABLE_INTERRUPT_HANDLER DisableInterruptHandler;
    W_ENABLE_INTERRUPT_HANDLER  EnableInterruptHandler;
    W_HALT_HANDLER              HaltHandler;
    W_HANDLE_INTERRUPT_HANDLER  HandleInterruptHandler;
    W_INITIALIZE_HANDLER        InitializeHandler;
    W_ISR_HANDLER               ISRHandler;
    W_QUERY_INFORMATION_HANDLER QueryInformationHandler;
    W_RECONFIGURE_HANDLER       ReconfigureHandler;
    W_RESET_HANDLER             ResetHandler;
    union
    {
        W_SEND_HANDLER          SendHandler;
        WM_SEND_HANDLER         WanSendHandler;
    };
    W_SET_INFORMATION_HANDLER   SetInformationHandler;
    union
    {
        W_TRANSFER_DATA_HANDLER TransferDataHandler;
        WM_TRANSFER_DATA_HANDLER WanTransferDataHandler;
    };

    //
    // Extensions for NDIS 4.0
    //
    W_RETURN_PACKET_HANDLER     ReturnPacketHandler;
    W_SEND_PACKETS_HANDLER      SendPacketsHandler;
    W_ALLOCATE_COMPLETE_HANDLER AllocateCompleteHandler;

    //
    // Extensions for NDIS 5.0
    //
    W_CO_CREATE_VC_HANDLER      CoCreateVcHandler;
    W_CO_DELETE_VC_HANDLER      CoDeleteVcHandler;
    W_CO_ACTIVATE_VC_HANDLER    CoActivateVcHandler;
    W_CO_DEACTIVATE_VC_HANDLER  CoDeactivateVcHandler;
    W_CO_SEND_PACKETS_HANDLER   CoSendPacketsHandler;
    W_CO_REQUEST_HANDLER        CoRequestHandler;
} NDIS50_MINIPORT_CHARACTERISTICS;

#if (((NDIS_MINIPORT_MAJOR_VERSION == 5) &&  (NDIS_MINIPORT_MINOR_VERSION == 1)) || NDIS_WRAPPER)
//
// Miniport extensions for NDIS 5.1
//
typedef VOID
(*W_CANCEL_SEND_PACKETS_HANDLER)(
    _In_  NDIS_HANDLE             MiniportAdapterContext,
    _In_  PVOID                   CancelId
    );

typedef VOID
(*W_PNP_EVENT_NOTIFY_HANDLER)(
    _In_  NDIS_HANDLE             MiniportAdapterContext,
    _In_  NDIS_DEVICE_PNP_EVENT   DevicePnPEvent,
    _In_  PVOID                   InformationBuffer,
    _In_  ULONG                   InformationBufferLength
    );

typedef VOID
(*W_MINIPORT_SHUTDOWN_HANDLER) (
    _In_  NDIS_HANDLE                     MiniportAdapterContext
    );

#endif // (((NDIS_MINIPORT_MAJOR_VERSION == 5) &&  (NDIS_MINIPORT_MINOR_VERSION == 1)) || NDIS_WRAPPER)

#endif // NDIS_LEGACY_MINIPORT

#if NDIS_SUPPORT_NDIS6
typedef
_IRQL_requires_(HIGH_LEVEL)
_Function_class_(MINIPORT_ISR)
BOOLEAN
(MINIPORT_ISR)(
    _In_   NDIS_HANDLE             MiniportInterruptContext,
    _Out_ PBOOLEAN                QueueDefaultInterruptDpc,
    _Out_ PULONG                  TargetProcessors
    );

typedef MINIPORT_ISR (*MINIPORT_ISR_HANDLER);

typedef
_IRQL_requires_(DISPATCH_LEVEL)
_Function_class_(MINIPORT_INTERRUPT_DPC)
VOID
(MINIPORT_INTERRUPT_DPC)(
    _In_  NDIS_HANDLE             MiniportInterruptContext,
    _In_  PVOID                   MiniportDpcContext,
    _In_  PVOID                   ReceiveThrottleParameters,
    _In_  PVOID                   NdisReserved2
    );

typedef MINIPORT_INTERRUPT_DPC (*MINIPORT_INTERRUPT_DPC_HANDLER);

#if NDIS_SUPPORT_NDIS620

typedef struct _NDIS_RECEIVE_THROTTLE_PARAMETERS
{
    _In_  ULONG MaxNblsToIndicate;
    _Out_ ULONG MoreNblsPending:1;
} NDIS_RECEIVE_THROTTLE_PARAMETERS, *PNDIS_RECEIVE_THROTTLE_PARAMETERS;

#define NDIS_INDICATE_ALL_NBLS      (~0ul)

#endif


typedef
_IRQL_requires_(HIGH_LEVEL)
_Function_class_(MINIPORT_DISABLE_INTERRUPT)
VOID
(MINIPORT_DISABLE_INTERRUPT)(
    _In_  NDIS_HANDLE             MiniportInterruptContext
    );

typedef MINIPORT_DISABLE_INTERRUPT (*MINIPORT_DISABLE_INTERRUPT_HANDLER);

typedef
_IRQL_requires_(HIGH_LEVEL)
_Function_class_(MINIPORT_ENABLE_INTERRUPT)
VOID
(MINIPORT_ENABLE_INTERRUPT)(
    _In_  NDIS_HANDLE             MiniportInterruptContext
    );

typedef MINIPORT_ENABLE_INTERRUPT (*MINIPORT_ENABLE_INTERRUPT_HANDLER);
//
// MSI support handlers
//
typedef
_IRQL_requires_(HIGH_LEVEL)
_Function_class_(MINIPORT_MESSAGE_INTERRUPT)
BOOLEAN
(MINIPORT_MESSAGE_INTERRUPT)(
    _In_   NDIS_HANDLE             MiniportInterruptContext,
    _In_   ULONG                   MessageId,
    _Out_ PBOOLEAN                QueueDefaultInterruptDpc,
    _Out_ PULONG                  TargetProcessors
    );

typedef MINIPORT_MESSAGE_INTERRUPT (*MINIPORT_MSI_ISR_HANDLER);

typedef
_IRQL_requires_(DISPATCH_LEVEL)
_Function_class_(MINIPORT_MESSAGE_INTERRUPT_DPC)
VOID
(MINIPORT_MESSAGE_INTERRUPT_DPC)(
    _In_ NDIS_HANDLE             MiniportInterruptContext,
    _In_ ULONG                   MessageId,
    _In_ PVOID                   MiniportDpcContext,
#if NDIS_SUPPORT_NDIS620
    _In_ PVOID                   ReceiveThrottleParameters,
    _In_ PVOID                   NdisReserved2
#else
    _In_ PULONG                  NdisReserved1,
    _In_ PULONG                  NdisReserved2
#endif
    );

typedef MINIPORT_MESSAGE_INTERRUPT_DPC (*MINIPORT_MSI_INTERRUPT_DPC_HANDLER);

typedef
_IRQL_requires_(HIGH_LEVEL)
_Function_class_(MINIPORT_DISABLE_MESSAGE_INTERRUPT)
VOID
(MINIPORT_DISABLE_MESSAGE_INTERRUPT)(
    _In_  NDIS_HANDLE             MiniportInterruptContext,
    _In_  ULONG                   MessageId
    );

typedef MINIPORT_DISABLE_MESSAGE_INTERRUPT (*MINIPORT_DISABLE_MSI_INTERRUPT_HANDLER);

typedef
_IRQL_requires_(HIGH_LEVEL)
_Function_class_(MINIPORT_ENABLE_MESSAGE_INTERRUPT)
VOID
(MINIPORT_ENABLE_MESSAGE_INTERRUPT)(
    _In_  NDIS_HANDLE             MiniportInterruptContext,
    _In_  ULONG                   MessageId
    );
typedef MINIPORT_ENABLE_MESSAGE_INTERRUPT (*MINIPORT_ENABLE_MSI_INTERRUPT_HANDLER);

typedef
_IRQL_requires_(HIGH_LEVEL)
_Function_class_(MINIPORT_SYNCHRONIZE_INTERRUPT)
BOOLEAN
(MINIPORT_SYNCHRONIZE_INTERRUPT)(
    _In_  NDIS_HANDLE         SynchronizeContext
    );
typedef MINIPORT_SYNCHRONIZE_INTERRUPT (*MINIPORT_SYNCHRONIZE_INTERRUPT_HANDLER);
typedef MINIPORT_SYNCHRONIZE_INTERRUPT (MINIPORT_SYNCHRONIZE_MESSAGE_INTERRUPT);
typedef MINIPORT_SYNCHRONIZE_MESSAGE_INTERRUPT (*MINIPORT_SYNCHRONIZE_MSI_INTERRUPT_HANDLER);

typedef enum _NDIS_INTERRUPT_TYPE
{
    NDIS_CONNECT_LINE_BASED = 1,
    NDIS_CONNECT_MESSAGE_BASED
} NDIS_INTERRUPT_TYPE, *PNDIS_INTERRUPT_TYPE;

#define NDIS_MINIPORT_INTERRUPT_REVISION_1          1

typedef struct _NDIS_MINIPORT_INTERRUPT_CHARACTERISTICS
{
    _In_   NDIS_OBJECT_HEADER                      Header;
    _In_   MINIPORT_ISR_HANDLER                    InterruptHandler;
    _In_   MINIPORT_INTERRUPT_DPC_HANDLER          InterruptDpcHandler;
    _In_   MINIPORT_DISABLE_INTERRUPT_HANDLER      DisableInterruptHandler;
    _In_   MINIPORT_ENABLE_INTERRUPT_HANDLER       EnableInterruptHandler;
    _In_   BOOLEAN                                 MsiSupported;
    _In_   BOOLEAN                                 MsiSyncWithAllMessages;
    _In_   MINIPORT_MSI_ISR_HANDLER                MessageInterruptHandler;
    _In_   MINIPORT_MSI_INTERRUPT_DPC_HANDLER      MessageInterruptDpcHandler;
    _In_   MINIPORT_DISABLE_MSI_INTERRUPT_HANDLER  DisableMessageInterruptHandler;
    _In_   MINIPORT_ENABLE_MSI_INTERRUPT_HANDLER   EnableMessageInterruptHandler;
    _Out_ NDIS_INTERRUPT_TYPE                    InterruptType;
    _Out_ PIO_INTERRUPT_MESSAGE_INFO             MessageInfoTable;
} NDIS_MINIPORT_INTERRUPT_CHARACTERISTICS, *PNDIS_MINIPORT_INTERRUPT_CHARACTERISTICS;

#define NDIS_SIZEOF_MINIPORT_INTERRUPT_CHARACTERISTICS_REVISION_1     \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_INTERRUPT_CHARACTERISTICS, MessageInfoTable)

typedef
_IRQL_requires_(PASSIVE_LEVEL)
_Function_class_(MINIPORT_ADD_DEVICE)
NDIS_STATUS
(MINIPORT_ADD_DEVICE)(
    _In_ NDIS_HANDLE              NdisMiniportHandle,
    _In_ NDIS_HANDLE              MiniportDriverContext
    );

typedef MINIPORT_ADD_DEVICE (*MINIPORT_ADD_DEVICE_HANDLER);

typedef
_IRQL_requires_(PASSIVE_LEVEL)
_Function_class_(MINIPORT_REMOVE_DEVICE)
VOID
(MINIPORT_REMOVE_DEVICE)(
    _In_ NDIS_HANDLE              MiniportAddDeviceContext
    );

typedef MINIPORT_REMOVE_DEVICE (*MINIPORT_REMOVE_DEVICE_HANDLER);

typedef
NDIS_STATUS
(MINIPORT_PNP_IRP)(
    _In_ NDIS_HANDLE              MiniportAddDeviceContext,
    _In_ PIRP                     Irp
    );

typedef MINIPORT_PNP_IRP (*MINIPORT_PNP_IRP_HANDLER);

typedef MINIPORT_PNP_IRP (MINIPORT_START_DEVICE);
typedef MINIPORT_PNP_IRP (* MINIPORT_START_DEVICE_HANDLER);
typedef MINIPORT_PNP_IRP (MINIPORT_FILTER_RESOURCE_REQUIREMENTS);
typedef MINIPORT_PNP_IRP (*MINIPORT_FILTER_RESOURCE_REQUIREMENTS_HANDLER);

#define NDIS_MINIPORT_PNP_CHARACTERISTICS_REVISION_1       1

typedef struct _NDIS_MINIPORT_PNP_CHARACTERISTICS
{
     NDIS_OBJECT_HEADER                       Header;
     MINIPORT_ADD_DEVICE_HANDLER              MiniportAddDeviceHandler;
     MINIPORT_REMOVE_DEVICE_HANDLER           MiniportRemoveDeviceHandler;
     MINIPORT_FILTER_RESOURCE_REQUIREMENTS_HANDLER  MiniportFilterResourceRequirementsHandler;
     MINIPORT_START_DEVICE_HANDLER            MiniportStartDeviceHandler;
     ULONG                                    Flags;
} NDIS_MINIPORT_PNP_CHARACTERISTICS, *PNDIS_MINIPORT_PNP_CHARACTERISTICS;

#define NDIS_SIZEOF_MINIPORT_PNP_CHARACTERISTICS_REVISION_1      \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_PNP_CHARACTERISTICS, Flags)

_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisMRegisterInterruptEx(
    _In_   NDIS_HANDLE                                 MiniportAdapterHandle,
    _In_   NDIS_HANDLE                                 MiniportInterruptContext,
    _In_   PNDIS_MINIPORT_INTERRUPT_CHARACTERISTICS    MiniportInterruptCharacteristics,
    _Out_  PNDIS_HANDLE                                NdisInterruptHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisMDeregisterInterruptEx(
    _In_  NDIS_HANDLE                     NdisInterruptHandle
    );

EXPORT
BOOLEAN
NdisMSynchronizeWithInterruptEx(
    _In_ NDIS_HANDLE                              NdisInterruptHandle,
    _In_ ULONG                                    MessageId,
#if (NDIS_SUPPORT_NDIS620)
    _In_ MINIPORT_SYNCHRONIZE_INTERRUPT_HANDLER   SynchronizeFunction,
#else
    _In_ PVOID                                    SynchronizeFunction,
#endif
    _In_ _Points_to_data_
         PVOID                                    SynchronizeContext
    );

#if NDIS_SUPPORT_60_COMPATIBLE_API

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
ULONG
NdisMQueueDpc(
    _In_  NDIS_HANDLE     NdisInterruptHandle,
    _In_  ULONG           MessageId,
    _In_  ULONG           TargetProcessors,
    _In_opt_  _Points_to_data_
          PVOID           MiniportDpcContext
    );

#endif

#if NDIS_SUPPORT_NDIS620

EXPORT
KAFFINITY
NdisMQueueDpcEx(
    IN  NDIS_HANDLE     NdisInterruptHandle,
    IN  ULONG           MessageId,
    IN  PGROUP_AFFINITY TargetProcessors,
    IN  PVOID           MiniportDpcContext
    );

#endif



#endif //  NDIS_SUPPORT_NDIS6

#if NDIS_LEGACY_MINIPORT
#if (((NDIS_MINIPORT_MAJOR_VERSION == 5) &&  (NDIS_MINIPORT_MINOR_VERSION == 1)) || NDIS_WRAPPER)
typedef struct _NDIS51_MINIPORT_CHARACTERISTICS
{
    union
    {
        NDIS50_MINIPORT_CHARACTERISTICS Ndis50Chars;

        struct
        {
            UCHAR                       MajorNdisVersion;
            UCHAR                       MinorNdisVersion;
            USHORT                      Filler;
            UINT                        Reserved;
            W_CHECK_FOR_HANG_HANDLER    CheckForHangHandler;
            W_DISABLE_INTERRUPT_HANDLER DisableInterruptHandler;
            W_ENABLE_INTERRUPT_HANDLER  EnableInterruptHandler;
            W_HALT_HANDLER              HaltHandler;
            W_HANDLE_INTERRUPT_HANDLER  HandleInterruptHandler;
            W_INITIALIZE_HANDLER        InitializeHandler;
            W_ISR_HANDLER               ISRHandler;
            W_QUERY_INFORMATION_HANDLER QueryInformationHandler;
            W_RECONFIGURE_HANDLER       ReconfigureHandler;
            W_RESET_HANDLER             ResetHandler;
            union
            {
                W_SEND_HANDLER          SendHandler;
                WM_SEND_HANDLER         WanSendHandler;
            };
            W_SET_INFORMATION_HANDLER   SetInformationHandler;
            union
            {
                W_TRANSFER_DATA_HANDLER TransferDataHandler;
                WM_TRANSFER_DATA_HANDLER WanTransferDataHandler;
            };
            
            //
            // Extensions for NDIS 4.0
            //
            W_RETURN_PACKET_HANDLER     ReturnPacketHandler;
            W_SEND_PACKETS_HANDLER      SendPacketsHandler;
            W_ALLOCATE_COMPLETE_HANDLER AllocateCompleteHandler;
            
            //
            // Extensions for NDIS 5.0
            //
            W_CO_CREATE_VC_HANDLER      CoCreateVcHandler;
            W_CO_DELETE_VC_HANDLER      CoDeleteVcHandler;
            W_CO_ACTIVATE_VC_HANDLER    CoActivateVcHandler;
            W_CO_DEACTIVATE_VC_HANDLER  CoDeactivateVcHandler;
            W_CO_SEND_PACKETS_HANDLER   CoSendPacketsHandler;
            W_CO_REQUEST_HANDLER        CoRequestHandler;
        };
    };

    //
    // Extensions for NDIS 5.1
    //
    W_CANCEL_SEND_PACKETS_HANDLER   CancelSendPacketsHandler;
    W_PNP_EVENT_NOTIFY_HANDLER      PnPEventNotifyHandler;
    W_MINIPORT_SHUTDOWN_HANDLER     AdapterShutdownHandler;
    PVOID                           Reserved1;
    PVOID                           Reserved2;
    PVOID                           Reserved3;
    PVOID                           Reserved4;
} NDIS51_MINIPORT_CHARACTERISTICS;
#endif // (((NDIS_MINIPORT_MAJOR_VERSION == 5) &&  (NDIS_MINIPORT_MINOR_VERSION == 1)) || NDIS_WRAPPER)

typedef struct _NDIS_MINIPORT_INTERRUPT
{
    PKINTERRUPT                 InterruptObject;
    union {
        volatile BOOLEAN        IsDeregistered;
        KSPIN_LOCK              DpcCountLock;
    };
    PVOID                       Reserved;
    W_ISR_HANDLER               MiniportIsr;
    W_HANDLE_INTERRUPT_HANDLER  MiniportDpc;
    KDPC                        InterruptDpc;
    PNDIS_MINIPORT_BLOCK        Miniport;

    volatile LONG               DpcCount;

    //
    // This is used to tell when all the Dpcs for the adapter are completed.
    //

    KEVENT                      DpcsCompletedEvent;

    BOOLEAN                     SharedInterrupt;
    BOOLEAN                     IsrRequested;

} NDIS_MINIPORT_INTERRUPT, *PNDIS_MINIPORT_INTERRUPT;
#endif // NDIS_LEGACY_MINIPORT

typedef struct _NDIS_MINIPORT_TIMER
{
    KTIMER                      Timer;
    KDPC                        Dpc;
    PNDIS_TIMER_FUNCTION        MiniportTimerFunction;
    PVOID                       MiniportTimerContext;
    PNDIS_MINIPORT_BLOCK        Miniport;
    struct _NDIS_MINIPORT_TIMER *NextTimer;
} NDIS_MINIPORT_TIMER, *PNDIS_MINIPORT_TIMER;

#if NDIS_LEGACY_MINIPORT
typedef
VOID
(*ETH_RCV_INDICATE_HANDLER)(
    _In_  PETH_FILTER             Filter,
    _In_  NDIS_HANDLE             MacReceiveContext,
    _In_  PCHAR                   Address,
    _In_  PVOID                   HeaderBuffer,
    _In_  UINT                    HeaderBufferSize,
    _In_  PVOID                   LookaheadBuffer,
    _In_  UINT                    LookaheadBufferSize,
    _In_  UINT                    PacketSize
    );

typedef
VOID
(*ETH_RCV_COMPLETE_HANDLER)(
    _In_  PETH_FILTER             Filter
    );

typedef
VOID
(*TR_RCV_INDICATE_HANDLER)(
    _In_  PTR_FILTER              Filter,
    _In_  NDIS_HANDLE             MacReceiveContext,
    _In_  PVOID                   HeaderBuffer,
    _In_  UINT                    HeaderBufferSize,
    _In_  PVOID                   LookaheadBuffer,
    _In_  UINT                    LookaheadBufferSize,
    _In_  UINT                    PacketSize
    );

typedef
VOID
(*TR_RCV_COMPLETE_HANDLER)(
    _In_  PTR_FILTER              Filter
    );

typedef
VOID
(*WAN_RCV_HANDLER)(
    _Out_ PNDIS_STATUS            Status,
    _In_  NDIS_HANDLE              MiniportAdapterHandle,
    _In_  NDIS_HANDLE              NdisLinkContext,
    _In_  PUCHAR                   Packet,
    _In_  ULONG                    PacketSize
    );

typedef
VOID
(*WAN_RCV_COMPLETE_HANDLER)(
    _In_ NDIS_HANDLE              MiniportAdapterHandle,
    _In_ NDIS_HANDLE              NdisLinkContext
    );

typedef
VOID
(*NDIS_M_SEND_COMPLETE_HANDLER)(
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  PNDIS_PACKET            Packet,
    _In_  NDIS_STATUS             Status
    );

typedef
VOID
(*NDIS_WM_SEND_COMPLETE_HANDLER)(
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  PVOID                   Packet,
    _In_  NDIS_STATUS             Status
    );

typedef
VOID
(*NDIS_M_TD_COMPLETE_HANDLER)(
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  PNDIS_PACKET            Packet,
    _In_  NDIS_STATUS             Status,
    _In_  UINT                    BytesTransferred
    );

typedef
VOID
(*NDIS_M_SEND_RESOURCES_HANDLER)(
    _In_  NDIS_HANDLE             MiniportAdapterHandle
    );

typedef
VOID
(*NDIS_M_STATUS_HANDLER)(
    _In_  NDIS_HANDLE             MiniportHandle,
    _In_  NDIS_STATUS             GeneralStatus,
    _In_  PVOID                   StatusBuffer,
    _In_  UINT                    StatusBufferSize
    );

typedef
VOID
(*NDIS_M_STS_COMPLETE_HANDLER)(
    _In_  NDIS_HANDLE             MiniportAdapterHandle
    );

typedef
VOID
(*NDIS_M_REQ_COMPLETE_HANDLER)(
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  NDIS_STATUS             Status
    );

typedef
VOID
(*NDIS_M_RESET_COMPLETE_HANDLER)(
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  NDIS_STATUS             Status,
    _In_  BOOLEAN                 AddressingReset
    );

typedef
BOOLEAN
(FASTCALL *NDIS_M_START_SENDS)(
    _In_  PNDIS_MINIPORT_BLOCK    Miniport
    );

//
// Wrapper initialization and termination.
//

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisInitializeWrapper(
    OUT PNDIS_HANDLE            NdisWrapperHandle,
    IN  PVOID                   SystemSpecific1,
    IN  PVOID                   SystemSpecific2,
    IN  PVOID                   SystemSpecific3
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisTerminateWrapper(
    _In_     NDIS_HANDLE             NdisWrapperHandle,
    _In_opt_ PVOID                   SystemSpecific
    );

#endif // NDIS_LEGACY_MINIPORT

#if NDIS_SUPPORT_NDIS6

typedef
_IRQL_requires_(DISPATCH_LEVEL)
_Function_class_(MINIPORT_PROCESS_SG_LIST)
VOID
(MINIPORT_PROCESS_SG_LIST)(
    _In_  PDEVICE_OBJECT          pDO,
    _In_  PVOID                   Reserved,
    _In_  PSCATTER_GATHER_LIST    pSGL,
    _In_  PVOID                   Context
    );

typedef MINIPORT_PROCESS_SG_LIST (*MINIPORT_PROCESS_SG_LIST_HANDLER);
//
// NDIS DMA description structure
//

typedef
_IRQL_requires_(PASSIVE_LEVEL)
_Function_class_(MINIPORT_ALLOCATE_SHARED_MEM_COMPLETE)
VOID
(MINIPORT_ALLOCATE_SHARED_MEM_COMPLETE)(
    _In_  NDIS_HANDLE             MiniportAdapterContext,
    _In_  PVOID                   VirtualAddress,
    _In_  PNDIS_PHYSICAL_ADDRESS  PhysicalAddress,
    _In_  ULONG                   Length,
    _In_  PVOID                   Context
    );
typedef MINIPORT_ALLOCATE_SHARED_MEM_COMPLETE (*MINIPORT_ALLOCATE_SHARED_MEM_COMPLETE_HANDLER);

/*
NDIS_STATUS
NdisMAllocateSharedMemoryAsyncEx(
    IN  NDIS_HANDLE             MiniportDmaHandle,
    IN  ULONG                   Length,
    IN  BOOLEAN                 Cached,
    IN  PVOID                   Context
    );
*/
#define NdisMAllocateSharedMemoryAsyncEx(_H, _L, _C, _X) NdisMAllocateSharedMemoryAsync(_H, _L, _C, _X)
#endif // NDIS_SUPPORT_NDIS6



_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisMAllocateSharedMemoryAsync(
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  ULONG                   Length,
    _In_  BOOLEAN                 Cached,
    _In_  PVOID                   Context
    );

//
// definitions for subordinate (as referred in Master/subordinate) DMA
//

//
// DMA Channel information
//
typedef struct _NDIS_DMA_DESCRIPTION
{
    BOOLEAN     DemandMode;
    BOOLEAN     AutoInitialize;
    BOOLEAN     DmaChannelSpecified;
    DMA_WIDTH   DmaWidth;
    DMA_SPEED   DmaSpeed;
    ULONG       DmaPort;
    ULONG       DmaChannel;
} NDIS_DMA_DESCRIPTION, *PNDIS_DMA_DESCRIPTION;

//
// Internal structure representing an NDIS DMA channel
//
typedef struct _NDIS_DMA_BLOCK
{
    PVOID                   MapRegisterBase;
    KEVENT                  AllocationEvent;
    PVOID                   SystemAdapterObject;
    PVOID                   Miniport;
    BOOLEAN                 InProgress;
} NDIS_DMA_BLOCK, *PNDIS_DMA_BLOCK;


EXPORT
VOID
NdisSetupDmaTransfer(
    OUT PNDIS_STATUS            Status,
    IN  NDIS_HANDLE             NdisDmaHandle,
    IN  PNDIS_BUFFER            Buffer,
    IN  ULONG                   Offset,
    IN  ULONG                   Length,
    IN  BOOLEAN                 WriteToDevice
    );

EXPORT
VOID
NdisCompleteDmaTransfer(
    OUT PNDIS_STATUS            Status,
    IN  NDIS_HANDLE             NdisDmaHandle,
    IN  PNDIS_BUFFER            Buffer,
    IN  ULONG                   Offset,
    IN  ULONG                   Length,
    IN  BOOLEAN                 WriteToDevice
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisMRegisterDmaChannel(
    _Out_ PNDIS_HANDLE            MiniportDmaHandle,
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  UINT                    DmaChannel,
    _In_  BOOLEAN                 Dma32BitAddresses,
    _In_  PNDIS_DMA_DESCRIPTION   DmaDescription,
    _In_  ULONG                   MaximumLength
    );


_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisMDeregisterDmaChannel(
    _In_ NDIS_HANDLE             MiniportDmaHandle
    );

/*++
VOID
NdisMSetupDmaTransfer(
    OUT PNDIS_STATUS            Status,
    IN  NDIS_HANDLE             MiniportDmaHandle,
    IN  PNDIS_BUFFER            Buffer,
    IN  ULONG                   Offset,
    IN  ULONG                   Length,
    IN  BOOLEAN                 WriteToDevice
    )
--*/
#define NdisMSetupDmaTransfer(_S, _H, _B, _O, _L, _M_) \
        NdisSetupDmaTransfer(_S, _H, _B, _O, _L, _M_)

/*++
VOID
NdisMCompleteDmaTransfer(
    OUT PNDIS_STATUS            Status,
    IN  NDIS_HANDLE             MiniportDmaHandle,
    IN  PNDIS_BUFFER            Buffer,
    IN  ULONG                   Offset,
    IN  ULONG                   Length,
    IN  BOOLEAN                 WriteToDevice
    )
--*/
#define NdisMCompleteDmaTransfer(_S, _H, _B, _O, _L, _M_) \
        NdisCompleteDmaTransfer(_S, _H, _B, _O, _L, _M_)

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
ULONG
NdisMReadDmaCounter(
    _In_ NDIS_HANDLE             MiniportDmaHandle
    );


//
// This API has been deprecated
//
DECLSPEC_DEPRECATED_DDK
EXPORT
VOID
NdisUpdateSharedMemory(
    IN  NDIS_HANDLE                     NdisAdapterHandle,
    IN  ULONG                           Length,
    IN  PVOID                           VirtualAddress,
    IN  NDIS_PHYSICAL_ADDRESS           PhysicalAddress
    );


#if NDIS_SUPPORT_NDIS6


//
// Flags used in NDIS_SG_DMA_DESCRIPTION
//

#define NDIS_SG_DMA_64_BIT_ADDRESS      0x00000001

//
// supported revision
//
#define NDIS_SG_DMA_DESCRIPTION_REVISION_1      1

typedef struct _NDIS_SG_DMA_DESCRIPTION
{
    NDIS_OBJECT_HEADER                              Header;
    ULONG                                           Flags;
    ULONG                                           MaximumPhysicalMapping;
    MINIPORT_PROCESS_SG_LIST_HANDLER                ProcessSGListHandler;
    MINIPORT_ALLOCATE_SHARED_MEM_COMPLETE_HANDLER   SharedMemAllocateCompleteHandler;
    ULONG                                           ScatterGatherListSize;
} NDIS_SG_DMA_DESCRIPTION, *PNDIS_SG_DMA_DESCRIPTION;

#define NDIS_SIZEOF_SG_DMA_DESCRIPTION_REVISION_1    \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_SG_DMA_DESCRIPTION, ScatterGatherListSize)

#define NDIS_MINIPORT_INIT_PARAMETERS_REVISION_1     1

typedef struct _NDIS_MINIPORT_INIT_PARAMETERS
{
    _In_  NDIS_OBJECT_HEADER              Header;
    _In_  ULONG                           Flags;
    _In_  PNDIS_RESOURCE_LIST             AllocatedResources;
    _In_  NDIS_HANDLE                     IMDeviceInstanceContext;
    _In_  NDIS_HANDLE                     MiniportAddDeviceContext;
    _In_  NET_IFINDEX                     IfIndex;
    _In_  NET_LUID                        NetLuid;
    _In_  PNDIS_PORT_AUTHENTICATION_PARAMETERS   DefaultPortAuthStates;
    _In_  PNDIS_PCI_DEVICE_CUSTOM_PROPERTIES PciDeviceCustomProperties;
} NDIS_MINIPORT_INIT_PARAMETERS, *PNDIS_MINIPORT_INIT_PARAMETERS;

#define NDIS_SIZEOF_MINIPORT_INIT_PARAMETER_REVISION_1      \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_INIT_PARAMETERS, PciDeviceCustomProperties)

//
// NDIS_MINIPORT_RESTART_PARAMETERS is used in MINIPORT_RESTART handler
//
#define NDIS_MINIPORT_RESTART_PARAMETERS_REVISION_1     1

typedef struct _NDIS_MINIPORT_RESTART_PARAMETERS
{
    NDIS_OBJECT_HEADER          Header;
    PNDIS_RESTART_ATTRIBUTES    RestartAttributes;
    ULONG                       Flags;
} NDIS_MINIPORT_RESTART_PARAMETERS, *PNDIS_MINIPORT_RESTART_PARAMETERS;

#define NDIS_SIZEOF_MINIPORT_RESTART_PARAMETERS_REVISION_1      \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_RESTART_PARAMETERS, Flags)

#define NDIS_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES_REVISION_1     1

#if (NDIS_SUPPORT_NDIS630)
#define NDIS_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES_REVISION_2     2
#endif

typedef struct _NDIS_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES
{
    NDIS_OBJECT_HEADER              Header;
    NDIS_HANDLE                     MiniportAdapterContext;
    ULONG                           AttributeFlags;
    UINT                            CheckForHangTimeInSeconds;
    NDIS_INTERFACE_TYPE             InterfaceType;
} NDIS_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES, *PNDIS_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES;

#define NDIS_SIZEOF_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES_REVISION_1    \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES, InterfaceType)

#if (NDIS_SUPPORT_NDIS630)
#define NDIS_SIZEOF_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES_REVISION_2    \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES, InterfaceType)
#endif

//
// flags used in NDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES->SupportedStatistics
//

#define NDIS_STATISTICS_XMIT_OK_SUPPORTED                       0x00000001
#define NDIS_STATISTICS_RCV_OK_SUPPORTED                        0x00000002
#define NDIS_STATISTICS_XMIT_ERROR_SUPPORTED                    0x00000004
#define NDIS_STATISTICS_RCV_ERROR_SUPPORTED                     0x00000008
#define NDIS_STATISTICS_RCV_NO_BUFFER_SUPPORTED                 0x00000010
#define NDIS_STATISTICS_DIRECTED_BYTES_XMIT_SUPPORTED           0x00000020
#define NDIS_STATISTICS_DIRECTED_FRAMES_XMIT_SUPPORTED          0x00000040
#define NDIS_STATISTICS_MULTICAST_BYTES_XMIT_SUPPORTED          0x00000080
#define NDIS_STATISTICS_MULTICAST_FRAMES_XMIT_SUPPORTED         0x00000100
#define NDIS_STATISTICS_BROADCAST_BYTES_XMIT_SUPPORTED          0x00000200
#define NDIS_STATISTICS_BROADCAST_FRAMES_XMIT_SUPPORTED         0x00000400
#define NDIS_STATISTICS_DIRECTED_BYTES_RCV_SUPPORTED            0x00000800
#define NDIS_STATISTICS_DIRECTED_FRAMES_RCV_SUPPORTED           0x00001000
#define NDIS_STATISTICS_MULTICAST_BYTES_RCV_SUPPORTED           0x00002000
#define NDIS_STATISTICS_MULTICAST_FRAMES_RCV_SUPPORTED          0x00004000
#define NDIS_STATISTICS_BROADCAST_BYTES_RCV_SUPPORTED           0x00008000
#define NDIS_STATISTICS_BROADCAST_FRAMES_RCV_SUPPORTED          0x00010000
#define NDIS_STATISTICS_RCV_CRC_ERROR_SUPPORTED                 0x00020000
#define NDIS_STATISTICS_TRANSMIT_QUEUE_LENGTH_SUPPORTED         0x00040000
#define NDIS_STATISTICS_BYTES_RCV_SUPPORTED                     0x00080000
#define NDIS_STATISTICS_BYTES_XMIT_SUPPORTED                    0x00100000
#define NDIS_STATISTICS_RCV_DISCARDS_SUPPORTED                  0x00200000
#define NDIS_STATISTICS_GEN_STATISTICS_SUPPORTED                0x00400000
#define NDIS_STATISTICS_XMIT_DISCARDS_SUPPORTED                 0x08000000


#define NDIS_MINIPORT_ADD_DEVICE_REGISTRATION_ATTRIBUTES_REVISION_1     1

typedef struct _NDIS_MINIPORT_ADD_DEVICE_REGISTRATION_ATTRIBUTES
{
    NDIS_OBJECT_HEADER              Header;
    NDIS_HANDLE                     MiniportAddDeviceContext;
    ULONG                           Flags;

} NDIS_MINIPORT_ADD_DEVICE_REGISTRATION_ATTRIBUTES,
  *PNDIS_MINIPORT_ADD_DEVICE_REGISTRATION_ATTRIBUTES;

#define NDIS_SIZEOF_MINIPORT_ADD_DEVICE_REGISTRATION_ATTRIBUTES_REVISION_1       \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_ADD_DEVICE_REGISTRATION_ATTRIBUTES, Flags)

#define NDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES_REVISION_1     1

#if (NDIS_SUPPORT_NDIS620)
#define NDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES_REVISION_2     2
#endif

typedef struct _NDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES
{
    NDIS_OBJECT_HEADER              Header;
    ULONG                           Flags;
    NDIS_MEDIUM                     MediaType;
    NDIS_PHYSICAL_MEDIUM            PhysicalMediumType;
    ULONG                           MtuSize;
    ULONG64                         MaxXmitLinkSpeed;
    ULONG64                         XmitLinkSpeed;
    ULONG64                         MaxRcvLinkSpeed;
    ULONG64                         RcvLinkSpeed;
    NDIS_MEDIA_CONNECT_STATE        MediaConnectState;
    NDIS_MEDIA_DUPLEX_STATE         MediaDuplexState;
    ULONG                           LookaheadSize;
    PNDIS_PNP_CAPABILITIES          PowerManagementCapabilities; // 6.20 drivers must use PowerManagementCapabilitiesEx
    ULONG                           MacOptions;
    ULONG                           SupportedPacketFilters;
    ULONG                           MaxMulticastListSize;
    USHORT                          MacAddressLength;
    UCHAR                           PermanentMacAddress[NDIS_MAX_PHYS_ADDRESS_LENGTH];
    UCHAR                           CurrentMacAddress[NDIS_MAX_PHYS_ADDRESS_LENGTH];
    PNDIS_RECEIVE_SCALE_CAPABILITIES RecvScaleCapabilities;
    NET_IF_ACCESS_TYPE              AccessType; // NET_IF_ACCESS_BROADCAST for a typical ethernet adapter
    NET_IF_DIRECTION_TYPE           DirectionType; // NET_IF_DIRECTION_SENDRECEIVE for a typical ethernet adapter
    NET_IF_CONNECTION_TYPE          ConnectionType; // IF_CONNECTION_DEDICATED for a typical ethernet adapter
    NET_IFTYPE                      IfType; // IF_TYPE_ETHERNET_CSMACD for a typical ethernet adapter (regardless of speed)
    BOOLEAN                         IfConnectorPresent; // RFC 2665 TRUE if physical adapter
    ULONG                           SupportedStatistics; // use NDIS_STATISTICS_XXXX_SUPPORTED
    ULONG                           SupportedPauseFunctions; // IEEE 802.3 37.2.1
    ULONG                           DataBackFillSize;
    ULONG                           ContextBackFillSize;
    _Field_size_bytes_(SupportedOidListLength)
    PNDIS_OID                       SupportedOidList;
    ULONG                           SupportedOidListLength;
    ULONG                           AutoNegotiationFlags;
#if (NDIS_SUPPORT_NDIS620)
    PNDIS_PM_CAPABILITIES           PowerManagementCapabilitiesEx;
#endif
} NDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES, *PNDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES;

#define NDIS_SIZEOF_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES_REVISION_1    \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES, AutoNegotiationFlags)

#if (NDIS_SUPPORT_NDIS620)
#define NDIS_SIZEOF_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES_REVISION_2    \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES, PowerManagementCapabilitiesEx)
#endif

#if (NDIS_SUPPORT_NDIS61)

//
// Flags and structure for Header/Data split
//
#define NDIS_HD_SPLIT_ATTRIBUTES_REVISION_1     1

typedef struct _NDIS_HD_SPLIT_ATTRIBUTES
{
    _In_   NDIS_OBJECT_HEADER          Header;
    _In_   ULONG                       HardwareCapabilities;
    _In_   ULONG                       CurrentCapabilities;
    _Out_ ULONG                       HDSplitFlags;
    _Out_ ULONG                       BackfillSize;
    _Out_ ULONG                       MaxHeaderSize;
} NDIS_HD_SPLIT_ATTRIBUTES, *PNDIS_HD_SPLIT_ATTRIBUTES;

#define NDIS_SIZEOF_HD_SPLIT_ATTRIBUTES_REVISION_1     \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_HD_SPLIT_ATTRIBUTES, MaxHeaderSize)

//
// The miniport registers its hardware assist capabilities through this
// structure
//
#define NDIS_MINIPORT_ADAPTER_HARDWARE_ASSIST_ATTRIBUTES_REVISION_1     1

#if (NDIS_SUPPORT_NDIS620)
#define NDIS_MINIPORT_ADAPTER_HARDWARE_ASSIST_ATTRIBUTES_REVISION_2     2
#endif

#if (NDIS_SUPPORT_NDIS630)
#define NDIS_MINIPORT_ADAPTER_HARDWARE_ASSIST_ATTRIBUTES_REVISION_3     3
#endif // (NDIS_SUPPORT_NDIS630)

typedef struct _NDIS_MINIPORT_ADAPTER_HARDWARE_ASSIST_ATTRIBUTES
{
    NDIS_OBJECT_HEADER              Header;
    PNDIS_HD_SPLIT_ATTRIBUTES       HDSplitAttributes;
#if (NDIS_SUPPORT_NDIS620)
    PNDIS_RECEIVE_FILTER_CAPABILITIES   HardwareReceiveFilterCapabilities;
    PNDIS_RECEIVE_FILTER_CAPABILITIES   CurrentReceiveFilterCapabilities;
    PNDIS_NIC_SWITCH_CAPABILITIES       HardwareNicSwitchCapabilities;
    PNDIS_NIC_SWITCH_CAPABILITIES       CurrentNicSwitchCapabilities;
#endif
#if (NDIS_SUPPORT_NDIS630)
    PNDIS_SRIOV_CAPABILITIES        HardwareSriovCapabilities;
    PNDIS_SRIOV_CAPABILITIES        CurrentSriovCapabilities;
    PNDIS_QOS_CAPABILITIES          HardwareQosCapabilities;
    PNDIS_QOS_CAPABILITIES          CurrentQosCapabilities;
#endif // (NDIS_SUPPORT_NDIS630)
} NDIS_MINIPORT_ADAPTER_HARDWARE_ASSIST_ATTRIBUTES, *PNDIS_MINIPORT_ADAPTER_HARDWARE_ASSIST_ATTRIBUTES;

#define NDIS_SIZEOF_MINIPORT_ADAPTER_HARDWARE_ASSIST_ATTRIBUTES_REVISION_1     \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_ADAPTER_HARDWARE_ASSIST_ATTRIBUTES, HDSplitAttributes)

#if (NDIS_SUPPORT_NDIS620)
#define NDIS_SIZEOF_MINIPORT_ADAPTER_HARDWARE_ASSIST_ATTRIBUTES_REVISION_2     \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_ADAPTER_HARDWARE_ASSIST_ATTRIBUTES, CurrentNicSwitchCapabilities)
#endif

#if (NDIS_SUPPORT_NDIS630)
#define NDIS_SIZEOF_MINIPORT_ADAPTER_HARDWARE_ASSIST_ATTRIBUTES_REVISION_3     \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_ADAPTER_HARDWARE_ASSIST_ATTRIBUTES, CurrentQosCapabilities)
#endif // (NDIS_SUPPORT_NDIS630)

#endif // (NDIS_SUPPORT_NDIS61)

//
// The miniport registers its offload capabilities through this
// structure
//
#define NDIS_MINIPORT_ADAPTER_OFFLOAD_ATTRIBUTES_REVISION_1     1
typedef struct _NDIS_MINIPORT_ADAPTER_OFFLOAD_ATTRIBUTES
{
    NDIS_OBJECT_HEADER              Header;
    PNDIS_OFFLOAD                   DefaultOffloadConfiguration;
    PNDIS_OFFLOAD                   HardwareOffloadCapabilities;
    PNDIS_TCP_CONNECTION_OFFLOAD    DefaultTcpConnectionOffloadConfiguration;
    PNDIS_TCP_CONNECTION_OFFLOAD    TcpConnectionOffloadHardwareCapabilities;
} NDIS_MINIPORT_ADAPTER_OFFLOAD_ATTRIBUTES, *PNDIS_MINIPORT_ADAPTER_OFFLOAD_ATTRIBUTES;

#define NDIS_SIZEOF_MINIPORT_ADAPTER_OFFLOAD_ATTRIBUTES_REVISION_1     \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_ADAPTER_OFFLOAD_ATTRIBUTES, TcpConnectionOffloadHardwareCapabilities)

#include <windot11.h>

#define NDIS_MINIPORT_ADAPTER_802_11_ATTRIBUTES_REVISION_1     1
#define NDIS_MINIPORT_ADAPTER_802_11_ATTRIBUTES_REVISION_2     2
#define NDIS_MINIPORT_ADAPTER_802_11_ATTRIBUTES_REVISION_3     3

typedef  struct _NDIS_MINIPORT_ADAPTER_NATIVE_802_11_ATTRIBUTES
{
    NDIS_OBJECT_HEADER Header;

    ULONG                    OpModeCapability;
    ULONG                    NumOfTXBuffers;
    ULONG                    NumOfRXBuffers;
    BOOLEAN                  MultiDomainCapabilityImplemented;
    ULONG                    NumSupportedPhys;
#ifdef __midl
    [size_is(NumSupportedPhys)]
#endif
    PDOT11_PHY_ATTRIBUTES    SupportedPhyAttributes;

    // Attributes specific to the operation modes
    PDOT11_EXTSTA_ATTRIBUTES ExtSTAAttributes;


#if (NDIS_SUPPORT_NDIS620)
    // virtual wifi specific attributes
    PDOT11_VWIFI_ATTRIBUTES VWiFiAttributes;
    // Ext AP specific attributes
    PDOT11_EXTAP_ATTRIBUTES ExtAPAttributes;
#endif // (NDIS_SUPPORT_NDIS620)

#if (NDIS_SUPPORT_NDIS630)
    PDOT11_WFD_ATTRIBUTES   WFDAttributes;

#endif // (NDIS_SUPPORT_NDIS630)

}NDIS_MINIPORT_ADAPTER_NATIVE_802_11_ATTRIBUTES,
  *PNDIS_MINIPORT_ADAPTER_NATIVE_802_11_ATTRIBUTES;

#define NDIS_SIZEOF_MINIPORT_ADAPTER_NATIVE_802_11_ATTRIBUTES_REVISION_1     \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_ADAPTER_NATIVE_802_11_ATTRIBUTES, ExtSTAAttributes)

#if (NDIS_SUPPORT_NDIS620)

#define NDIS_SIZEOF_MINIPORT_ADAPTER_NATIVE_802_11_ATTRIBUTES_REVISION_2     \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_ADAPTER_NATIVE_802_11_ATTRIBUTES, ExtAPAttributes)

#endif // (NDIS_SUPPORT_NDIS620)

#if (NDIS_SUPPORT_NDIS630)

#define NDIS_SIZEOF_MINIPORT_ADAPTER_NATIVE_802_11_ATTRIBUTES_REVISION_3     \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_ADAPTER_NATIVE_802_11_ATTRIBUTES, WFDAttributes)

#define NDIS_MINIPORT_ADAPTER_NDK_ATTRIBUTES_REVISION_1 1

typedef struct _NDIS_MINIPORT_ADAPTER_NDK_ATTRIBUTES
{
    //
    // Header.Type = NDIS_OBJECT_TYPE_MINIPORT_ADAPTER_NDK_ATTRIBUTES;
    // Header.Revision = NDIS_MINIPORT_ADAPTER_NDK_ATTRIBUTES_REVISION_1;
    // Header.Size = sizeof(NDIS_MINIPORT_ADAPTER_NDK_ATTRIBUTES);
    //
    NDIS_OBJECT_HEADER Header;

    //
    // An NDK-capable adapter must indicate whether its ND functionality is enabled or
    // disabled when the adapter is initialized.
    //
    BOOLEAN Enabled;

    //
    // Independent of whether ND functionality of an adapter is currently enabled or disabled,
    // an NDK-capable adapter must always indicate its capabilities during miniport initialization.
    //
    PNDIS_NDK_CAPABILITIES NdkCapabilities;
} NDIS_MINIPORT_ADAPTER_NDK_ATTRIBUTES, *PNDIS_MINIPORT_ADAPTER_NDK_ATTRIBUTES;

#define NDIS_SIZEOF_MINIPORT_ADAPTER_NDK_ATTRIBUTES_REVISION_1    \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_ADAPTER_NDK_ATTRIBUTES, NdkCapabilities)

#endif // (NDIS_SUPPORT_NDIS630)

typedef _Struct_size_bytes_(_Inexpressible_(Header.Size)) union _NDIS_MINIPORT_ADAPTER_ATTRIBUTES
{
    NDIS_OBJECT_HEADER                                Header;
    NDIS_MINIPORT_ADD_DEVICE_REGISTRATION_ATTRIBUTES  AddDeviceRegistrationAttributes;
    NDIS_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES     RegistrationAttributes;
    NDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES          GeneralAttributes;
    NDIS_MINIPORT_ADAPTER_OFFLOAD_ATTRIBUTES          OffloadAttributes;
    NDIS_MINIPORT_ADAPTER_NATIVE_802_11_ATTRIBUTES    Native_802_11_Attributes;
#if (NDIS_SUPPORT_NDIS61)
    NDIS_MINIPORT_ADAPTER_HARDWARE_ASSIST_ATTRIBUTES  HardwareAssistAttributes;
#endif // (NDIS_SUPPORT_NDIS61)
#if (NDIS_SUPPORT_NDIS630)
    NDIS_MINIPORT_ADAPTER_NDK_ATTRIBUTES              NDKAttributes;
#endif // (NDIS_SUPPORT_NDIS630)
} NDIS_MINIPORT_ADAPTER_ATTRIBUTES, *PNDIS_MINIPORT_ADAPTER_ATTRIBUTES;


//
// flags used in AttributeFlags of NDIS_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES
// with NdisMSetMiniportAttributes
//

#define NDIS_MINIPORT_ATTRIBUTES_HARDWARE_DEVICE            0x00000001
#define NDIS_MINIPORT_ATTRIBUTES_NDIS_WDM                   0x00000002
#define NDIS_MINIPORT_ATTRIBUTES_SURPRISE_REMOVE_OK         0x00000004
#define NDIS_MINIPORT_ATTRIBUTES_NOT_CO_NDIS                0x00000008
#define NDIS_MINIPORT_ATTRIBUTES_DO_NOT_BIND_TO_ALL_CO      0x00000010
#define NDIS_MINIPORT_ATTRIBUTES_NO_HALT_ON_SUSPEND         0x00000020
#define NDIS_MINIPORT_ATTRIBUTES_BUS_MASTER                 0x00000040
#define NDIS_MINIPORT_ATTRIBUTES_CONTROLS_DEFAULT_PORT      0x00000080
#if (NDIS_SUPPORT_NDIS630)
#define NDIS_MINIPORT_ATTRIBUTES_NO_PAUSE_ON_SUSPEND        0x00000100
#define NDIS_MINIPORT_ATTRIBUTES_NO_OID_INTERCEPT_ON_NONDEFAULT_PORTS 0x00000200
#define NDIS_MINIPORT_ATTRIBUTES_REGISTER_BUGCHECK_CALLBACK 0x00000400
#endif

//
// NDIS 6.0 miniport's entry points
//

_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisMSetMiniportAttributes(
    _In_  NDIS_HANDLE                             NdisMiniportHandle,
    _In_reads_bytes_((MiniportAttributes->Header.Size))
        PNDIS_MINIPORT_ADAPTER_ATTRIBUTES       MiniportAttributes
    );

typedef
_IRQL_requires_(PASSIVE_LEVEL)
_Function_class_(MINIPORT_INITIALIZE)
NDIS_STATUS
(MINIPORT_INITIALIZE)(
    _In_  NDIS_HANDLE                         NdisMiniportHandle,
    _In_  NDIS_HANDLE                         MiniportDriverContext,
    _In_  PNDIS_MINIPORT_INIT_PARAMETERS      MiniportInitParameters
    );

typedef MINIPORT_INITIALIZE (*MINIPORT_INITIALIZE_HANDLER);

typedef enum _NDIS_HALT_ACTION
{
    NdisHaltDeviceDisabled,
    NdisHaltDeviceInstanceDeInitialized,
    NdisHaltDevicePoweredDown,
    NdisHaltDeviceSurpriseRemoved,
    NdisHaltDeviceFailed,
    NdisHaltDeviceInitializationFailed,
    NdisHaltDeviceStopped
} NDIS_HALT_ACTION, *PNDIS_HALT_ACTION;

typedef
_IRQL_requires_(PASSIVE_LEVEL)
_Function_class_(MINIPORT_HALT)
VOID
(MINIPORT_HALT)(
    _In_  NDIS_HANDLE             MiniportAdapterContext,
    _In_  NDIS_HALT_ACTION        HaltAction
    );

typedef MINIPORT_HALT (*MINIPORT_HALT_HANDLER);


#define NDIS_MINIPORT_PAUSE_PARAMETERS_REVISION_1     1

typedef struct _NDIS_MINIPORT_PAUSE_PARAMETERS
{
    NDIS_OBJECT_HEADER          Header;
    ULONG                       Flags;
    ULONG                       PauseReason;
} NDIS_MINIPORT_PAUSE_PARAMETERS, *PNDIS_MINIPORT_PAUSE_PARAMETERS;

#define NDIS_SIZEOF_MINIPORT_PAUSE_PARAMETERS_REVISION_1     \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_PAUSE_PARAMETERS, PauseReason)

typedef
_IRQL_requires_(PASSIVE_LEVEL)
_Function_class_(MINIPORT_PAUSE)
NDIS_STATUS
(MINIPORT_PAUSE)(
    _In_  NDIS_HANDLE             MiniportAdapterContext,
    _In_  PNDIS_MINIPORT_PAUSE_PARAMETERS   PauseParameters
    );

typedef MINIPORT_PAUSE (*MINIPORT_PAUSE_HANDLER);

typedef
_IRQL_requires_(PASSIVE_LEVEL)
_Function_class_(MINIPORT_RESTART)
NDIS_STATUS
(MINIPORT_RESTART)(
    _In_  NDIS_HANDLE                             MiniportAdapterContext,
    _In_  PNDIS_MINIPORT_RESTART_PARAMETERS       RestartParameters
    );

typedef MINIPORT_RESTART (*MINIPORT_RESTART_HANDLER);

typedef
_IRQL_requires_(PASSIVE_LEVEL)
_Function_class_(MINIPORT_OID_REQUEST)
NDIS_STATUS
(MINIPORT_OID_REQUEST) (
    _In_  NDIS_HANDLE             MiniportAdapterContext,
    _In_  PNDIS_OID_REQUEST       OidRequest
    );

typedef MINIPORT_OID_REQUEST (*MINIPORT_OID_REQUEST_HANDLER);

typedef
_IRQL_requires_(PASSIVE_LEVEL)
_Function_class_(MINIPORT_UNLOAD)
VOID
(MINIPORT_UNLOAD) (
    _In_ PDRIVER_OBJECT           DriverObject
    );
typedef MINIPORT_UNLOAD (*MINIPORT_DRIVER_UNLOAD);

//
// reasons for calling shutdown handler
//
typedef enum _NDIS_SHUTDOWN_ACTION {
    NdisShutdownPowerOff,
    NdisShutdownBugCheck
} NDIS_SHUTDOWN_ACTION, PNDIS_SHUTDOWN_ACTION;

typedef
_When_(ShutdownAction==NdisShutdownPowerOff, _IRQL_requires_(PASSIVE_LEVEL))
_When_(ShutdownAction==NdisShutdownBugCheck, _IRQL_requires_(HIGH_LEVEL))
_Function_class_(MINIPORT_SHUTDOWN)
VOID
(MINIPORT_SHUTDOWN) (
    _In_  NDIS_HANDLE             MiniportAdapterContext,
    _In_  NDIS_SHUTDOWN_ACTION    ShutdownAction
    );

typedef MINIPORT_SHUTDOWN (*MINIPORT_SHUTDOWN_HANDLER);


#define NET_DEVICE_PNP_EVENT_REVISION_1         1

typedef struct _NET_DEVICE_PNP_EVENT
{
    NDIS_OBJECT_HEADER         Header;
    NDIS_PORT_NUMBER           PortNumber;
    NDIS_DEVICE_PNP_EVENT      DevicePnPEvent;
    PVOID                      InformationBuffer;
    ULONG                      InformationBufferLength;
    UCHAR                      NdisReserved[2 * sizeof(PVOID)];

} NET_DEVICE_PNP_EVENT, *PNET_DEVICE_PNP_EVENT;

#define NDIS_SIZEOF_NET_DEVICE_PNP_EVENT_REVISION_1     \
        RTL_SIZEOF_THROUGH_FIELD(NET_DEVICE_PNP_EVENT, NdisReserved)

typedef
_IRQL_requires_(PASSIVE_LEVEL)
_Function_class_(MINIPORT_DEVICE_PNP_EVENT_NOTIFY)
VOID
(MINIPORT_DEVICE_PNP_EVENT_NOTIFY)(
    _In_  NDIS_HANDLE             MiniportAdapterContext,
    _In_  PNET_DEVICE_PNP_EVENT   NetDevicePnPEvent
    );

typedef MINIPORT_DEVICE_PNP_EVENT_NOTIFY (*MINIPORT_DEVICE_PNP_EVENT_NOTIFY_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(MINIPORT_CANCEL_SEND)
VOID
(MINIPORT_CANCEL_SEND)(
    _In_  NDIS_HANDLE             MiniportAdapterContext,
    _In_  PVOID                   CancelId
    );

typedef MINIPORT_CANCEL_SEND (*MINIPORT_CANCEL_SEND_HANDLER);

typedef
_IRQL_requires_(PASSIVE_LEVEL)
_Function_class_(MINIPORT_CHECK_FOR_HANG)
BOOLEAN
(MINIPORT_CHECK_FOR_HANG)(
    _In_  NDIS_HANDLE             MiniportAdapterContext
    );

typedef MINIPORT_CHECK_FOR_HANG (*MINIPORT_CHECK_FOR_HANG_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(MINIPORT_RESET)
NDIS_STATUS
(MINIPORT_RESET)(
    _In_   NDIS_HANDLE             MiniportAdapterContext,
    _Out_ PBOOLEAN                AddressingReset
    );

typedef MINIPORT_RESET (*MINIPORT_RESET_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(MINIPORT_CANCEL_OID_REQUEST)
VOID
(MINIPORT_CANCEL_OID_REQUEST)(
    _In_ NDIS_HANDLE              MiniportAdapterContext,
    _In_ PVOID                    RequestId
    );

typedef MINIPORT_CANCEL_OID_REQUEST (*MINIPORT_CANCEL_OID_REQUEST_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(MINIPORT_SEND_NET_BUFFER_LISTS)
VOID
(MINIPORT_SEND_NET_BUFFER_LISTS)(
    _In_  NDIS_HANDLE             MiniportAdapterContext,
    _In_  PNET_BUFFER_LIST        NetBufferList,
    _In_  NDIS_PORT_NUMBER        PortNumber,
    _In_  ULONG                   SendFlags
    );

typedef MINIPORT_SEND_NET_BUFFER_LISTS (*MINIPORT_SEND_NET_BUFFER_LISTS_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(MINIPORT_RETURN_NET_BUFFER_LISTS)
VOID
(MINIPORT_RETURN_NET_BUFFER_LISTS)(
    _In_  NDIS_HANDLE             MiniportAdapterContext,
    _In_  PNET_BUFFER_LIST        NetBufferLists,
    _In_  ULONG                   ReturnFlags
    );

typedef MINIPORT_RETURN_NET_BUFFER_LISTS (*MINIPORT_RETURN_NET_BUFFER_LISTS_HANDLER);

#if (NDIS_SUPPORT_NDIS61)
//
// NDIS 6.1 miniport's entry points
//

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(MINIPORT_DIRECT_OID_REQUEST)
NDIS_STATUS
(MINIPORT_DIRECT_OID_REQUEST) (
    _In_  NDIS_HANDLE             MiniportAdapterContext,
    _In_  PNDIS_OID_REQUEST       OidRequest
    );

typedef MINIPORT_DIRECT_OID_REQUEST (*MINIPORT_DIRECT_OID_REQUEST_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(MINIPORT_CANCEL_DIRECT_OID_REQUEST)
VOID
(MINIPORT_CANCEL_DIRECT_OID_REQUEST)(
    _In_ NDIS_HANDLE              MiniportAdapterContext,
    _In_ PVOID                    RequestId
    );

typedef MINIPORT_CANCEL_DIRECT_OID_REQUEST (*MINIPORT_CANCEL_DIRECT_OID_REQUEST_HANDLER);


#endif // (NDIS_SUPPORT_NDIS61)

//
// flags used in Flags field of NDIS60_MINIPORT_CHARACTERISTICS
//
#define NDIS_INTERMEDIATE_DRIVER                0x00000001
#define NDIS_WDM_DRIVER                         0x00000002
#if NDIS_SUPPORT_NDIS630
#define NDIS_DRIVER_FLAGS_RESERVED              0x00000008


#endif

#define NDIS_MINIPORT_DRIVER_CHARACTERISTICS_REVISION_1     1

#if (NDIS_SUPPORT_NDIS61)
#define NDIS_MINIPORT_DRIVER_CHARACTERISTICS_REVISION_2     2
#endif // (NDIS_SUPPORT_NDIS61)

typedef struct _NDIS_MINIPORT_DRIVER_CHARACTERISTICS
{
    NDIS_OBJECT_HEADER                          Header;
    UCHAR                                       MajorNdisVersion;
    UCHAR                                       MinorNdisVersion;
    UCHAR                                       MajorDriverVersion;
    UCHAR                                       MinorDriverVersion;
    ULONG                                       Flags;
    SET_OPTIONS_HANDLER                         SetOptionsHandler;
    MINIPORT_INITIALIZE_HANDLER                 InitializeHandlerEx;
    MINIPORT_HALT_HANDLER                       HaltHandlerEx;
    MINIPORT_DRIVER_UNLOAD                      UnloadHandler;
    MINIPORT_PAUSE_HANDLER                      PauseHandler;
    MINIPORT_RESTART_HANDLER                    RestartHandler;
    MINIPORT_OID_REQUEST_HANDLER                OidRequestHandler;
    MINIPORT_SEND_NET_BUFFER_LISTS_HANDLER      SendNetBufferListsHandler;
    MINIPORT_RETURN_NET_BUFFER_LISTS_HANDLER    ReturnNetBufferListsHandler;
    MINIPORT_CANCEL_SEND_HANDLER                CancelSendHandler;
    MINIPORT_CHECK_FOR_HANG_HANDLER             CheckForHangHandlerEx;
    MINIPORT_RESET_HANDLER                      ResetHandlerEx;
    MINIPORT_DEVICE_PNP_EVENT_NOTIFY_HANDLER    DevicePnPEventNotifyHandler;
    MINIPORT_SHUTDOWN_HANDLER                   ShutdownHandlerEx;
    MINIPORT_CANCEL_OID_REQUEST_HANDLER         CancelOidRequestHandler;
#if (NDIS_SUPPORT_NDIS61)
    MINIPORT_DIRECT_OID_REQUEST_HANDLER         DirectOidRequestHandler;
    MINIPORT_CANCEL_DIRECT_OID_REQUEST_HANDLER  CancelDirectOidRequestHandler;
#endif // (NDIS_SUPPORT_NDIS61)
} NDIS_MINIPORT_DRIVER_CHARACTERISTICS, *PNDIS_MINIPORT_DRIVER_CHARACTERISTICS;

#define NDIS_SIZEOF_MINIPORT_DRIVER_CHARACTERISTICS_REVISION_1      \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_DRIVER_CHARACTERISTICS, CancelOidRequestHandler)

#if (NDIS_SUPPORT_NDIS61)
#define NDIS_SIZEOF_MINIPORT_DRIVER_CHARACTERISTICS_REVISION_2      \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_DRIVER_CHARACTERISTICS, CancelDirectOidRequestHandler)
#endif // (NDIS_SUPPORT_NDIS61)

//
// CO NDIS 6.0 handlers
//

#define NDIS_MINIPORT_CO_CHARACTERISTICS_REVISION_1      1

typedef struct _NDIS_MINIPORT_CO_CHARACTERISTICS
{
    NDIS_OBJECT_HEADER                          Header;     // Header.Type = NDIS_OBJECT_TYPE_PROTOCOL_CO_CHARACTERISTICS
    ULONG                                       Flags;
    W_CO_CREATE_VC_HANDLER                      CoCreateVcHandler;
    W_CO_DELETE_VC_HANDLER                      CoDeleteVcHandler;
    W_CO_ACTIVATE_VC_HANDLER                    CoActivateVcHandler;
    W_CO_DEACTIVATE_VC_HANDLER                  CoDeactivateVcHandler;
    W_CO_SEND_NET_BUFFER_LISTS_HANDLER          CoSendNetBufferListsHandler;
    W_CO_OID_REQUEST_HANDLER                    CoOidRequestHandler;

} NDIS_MINIPORT_CO_CHARACTERISTICS, *PNDIS_MINIPORT_CO_CHARACTERISTICS;

#define NDIS_SIZEOF_MINIPORT_CO_CHARACTERISTICS_REVISION_1      \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_CO_CHARACTERISTICS, CoOidRequestHandler)

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMIndicateStatusEx(
    _In_ NDIS_HANDLE              MiniportAdapterHandle,
    _In_ PNDIS_STATUS_INDICATION  StatusIndication
    );

EXPORT
VOID
NdisMCoOidRequestComplete(
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  NDIS_HANDLE             NdisMiniportVcHandle,
    _In_  PNDIS_OID_REQUEST       Request,
    _In_  NDIS_STATUS             Status
    );


EXPORT
NDIS_STATUS
NdisMCmOidRequest(
    _In_     NDIS_HANDLE             NdisAfHandle,
    _In_opt_ NDIS_HANDLE             NdisVcHandle,
    _In_opt_ NDIS_HANDLE             NdisPartyHandle,
    _Inout_  PNDIS_OID_REQUEST       NdisOidRequest
    );


EXPORT
VOID
NdisMCoIndicateStatusEx(
    _In_     NDIS_HANDLE             MiniportAdapterHandle,
    _In_opt_ NDIS_HANDLE             NdisVcHandle,
    _In_     PNDIS_STATUS_INDICATION StatusIndication
    );

#endif // NDIS_SUPPORT_NDIS6


//
// Do not change the structure below !!!
//
typedef struct
{
    union
    {
        PETH_FILTER             EthDB;
        PNULL_FILTER            NullDB;             // Default Filter
    };
    PTR_FILTER                  TrDB;

    PVOID                       YYYDB;

    PVOID                       XXXDB;
} FILTERDBS, *PFILTERDBS;

#if NDIS_LEGACY_MINIPORT
typedef
VOID
(*FILTER_PACKET_INDICATION_HANDLER)(
    _In_  NDIS_HANDLE             Miniport,
    _In_  PPNDIS_PACKET           PacketArray,
    _In_  UINT                    NumberOfPackets
    );

#endif // NDIS_LEGACY_MINIPORT

#if !NDIS_WRAPPER
//
// one of these per mini-port registered on a Driver
//

#if NDIS_LEGACY_MINIPORT

#pragma warning(push)

#pragma warning(disable:4201) // (nonstandard extension used : nameless struct/union)


struct _NDIS_MINIPORT_BLOCK
{
    NDIS_OBJECT_HEADER          Header;
    PNDIS_MINIPORT_BLOCK        NextMiniport;       // used by driver's MiniportQueue
    PNDIS_MINIPORT_BLOCK        BaseMiniport;
    NDIS_HANDLE                 MiniportAdapterContext; // context when calling mini-port functions
    UNICODE_STRING              Reserved4;
    PVOID                       Reserved10;
    NDIS_HANDLE                 OpenQueue;          // queue of opens for this mini-port
    REFERENCE                   ShortRef;           // contains spinlock for OpenQueue

    NDIS_HANDLE                 Reserved12;

    UCHAR                       Padding1;           // DO NOT REMOVE OR NDIS WILL BREAK!!!

    //
    // Synchronization stuff.
    //
    // The boolean is used to lock out several DPCs from running at the same time.
    //
    UCHAR                       LockAcquired;       // EXPOSED via macros. Do not move

    UCHAR                       PmodeOpens;         // Count of opens which turned on pmode/all_local

    //
    //  This is the processor number that the miniport's
    //  interrupt DPC and timers are running on.
    //
    //  Note: This field is no longer used
    //
    UCHAR                       Reserved23;

    KSPIN_LOCK                  Lock;

    PNDIS_REQUEST               Reserved25;

    PVOID                       Interrupt;

    ULONG                       Flags;              // Flags to keep track of the miniport's state.
    ULONG                       PnPFlags;

    //
    // Send information
    //
    LIST_ENTRY                  PacketList;
    PNDIS_PACKET                FirstPendingPacket; // This is head of the queue of packets
                                                    // waiting to be sent to miniport.
    PNDIS_PACKET                ReturnPacketsQueue;

    //
    // Space used for temp. use during request processing
    //
    ULONG                       RequestBuffer;
    PVOID                       Reserved26;

    PNDIS_MINIPORT_BLOCK        PrimaryMiniport;
    PVOID                       Reserved11;

    //
    // context to pass to bus driver when reading or writing config space
    //
    PVOID                       BusDataContext;
    ULONG                       Reserved3;

    //
    // Resource information
    //
    PCM_RESOURCE_LIST           Resources;

    //
    // Watch-dog timer
    //
    NDIS_TIMER                  WakeUpDpcTimer;

    //
    // Needed for PnP. Upcased version. The buffer is allocated as part of the
    // NDIS_MINIPORT_BLOCK itself.
    //
    // Note:
    // the following two fields should be explicitly UNICODE_STRING because
    // under Win9x the NDIS_STRING is an ANSI_STRING
    //
    UNICODE_STRING              Reserved20;
    UNICODE_STRING              SymbolicLinkName;

    //
    // Period of check for hang timer
    //
    ULONG                       CheckForHangSeconds;

    ULONG                       Reserved5;

    //
    // Reset information
    //
    NDIS_STATUS                 ResetStatus;
    NDIS_HANDLE                 ResetOpen;

    //
    // Holds media specific information.
    //
#ifdef __cplusplus
    FILTERDBS                   FilterDbs;          // EXPOSED via macros. Do not move
#else
    FILTERDBS;                                      // EXPOSED via macros. Do not move
#endif

    FILTER_PACKET_INDICATION_HANDLER PacketIndicateHandler;
    NDIS_M_SEND_COMPLETE_HANDLER    SendCompleteHandler;
    NDIS_M_SEND_RESOURCES_HANDLER   SendResourcesHandler;
    NDIS_M_RESET_COMPLETE_HANDLER   ResetCompleteHandler;

    NDIS_MEDIUM                 MediaType;

    //
    // contains mini-port information
    //
    ULONG                       BusNumber;
    NDIS_INTERFACE_TYPE         BusType;
    NDIS_INTERFACE_TYPE         AdapterType;

    PDEVICE_OBJECT              Reserved6;
    PDEVICE_OBJECT              Reserved7;
    PDEVICE_OBJECT              Reserved8;


    PVOID                       MiniportSGDmaBlock;

    //
    // List of registered address families. Valid for the call-manager, Null for the client
    //
    PNDIS_AF_LIST               CallMgrAfList;

    PVOID                       MiniportThread;
    PVOID                       SetInfoBuf;
    USHORT                      SetInfoBufLen;
    USHORT                      MaxSendPackets;

    //
    //  Status code that is returned from the fake handlers.
    //
    NDIS_STATUS                 FakeStatus;

    PVOID                       Reserved24;        // For the filter lock

    PUNICODE_STRING             Reserved9;

    PVOID                       Reserved21;

    UINT                        MacOptions;

    //
    // RequestInformation
    //
    PNDIS_REQUEST               PendingRequest;
    UINT                        MaximumLongAddresses;
    UINT                        Reserved27;
    UINT                        CurrentLookahead;
    UINT                        MaximumLookahead;

    //
    //  For efficiency
    //
    ULONG_PTR                   Reserved1;
    W_DISABLE_INTERRUPT_HANDLER DisableInterruptHandler;
    W_ENABLE_INTERRUPT_HANDLER  EnableInterruptHandler;
    W_SEND_PACKETS_HANDLER      SendPacketsHandler;
    NDIS_M_START_SENDS          DeferredSendHandler;

    //
    // The following cannot be unionized.
    //
    ETH_RCV_INDICATE_HANDLER    EthRxIndicateHandler;   // EXPOSED via macros. Do not move
    TR_RCV_INDICATE_HANDLER     TrRxIndicateHandler;    // EXPOSED via macros. Do not move
    PVOID                       Reserved2;

    ETH_RCV_COMPLETE_HANDLER    EthRxCompleteHandler;   // EXPOSED via macros. Do not move
    TR_RCV_COMPLETE_HANDLER     TrRxCompleteHandler;    // EXPOSED via macros. Do not move
    PVOID                       Reserved22;

    NDIS_M_STATUS_HANDLER       StatusHandler;          // EXPOSED via macros. Do not move
    NDIS_M_STS_COMPLETE_HANDLER StatusCompleteHandler;  // EXPOSED via macros. Do not move
    NDIS_M_TD_COMPLETE_HANDLER  TDCompleteHandler;      // EXPOSED via macros. Do not move
    NDIS_M_REQ_COMPLETE_HANDLER QueryCompleteHandler;   // EXPOSED via macros. Do not move
    NDIS_M_REQ_COMPLETE_HANDLER SetCompleteHandler;     // EXPOSED via macros. Do not move

    NDIS_WM_SEND_COMPLETE_HANDLER WanSendCompleteHandler;// EXPOSED via macros. Do not move
    WAN_RCV_HANDLER             WanRcvHandler;          // EXPOSED via macros. Do not move
    WAN_RCV_COMPLETE_HANDLER    WanRcvCompleteHandler;  // EXPOSED via macros. Do not move

    /********************************************************************************************/
    /****************                                                                  **********/
    /**************** STUFF ABOVE IS POTENTIALLY ACCESSED BY MACROS. ADD STUFF BELOW   **********/
    /**************** SEVERE POSSIBILITY OF BREAKING SOMETHING IF STUFF ABOVE IS MOVED **********/
    /****************                                                                  **********/
    /********************************************************************************************/

};

#pragma warning(pop)


#endif // NDIS_LEGACY_MINIPORT

#endif // NDIS_WRAPPER not defined


#if NDIS_LEGACY_MINIPORT

#ifdef NDIS51_MINIPORT
typedef struct _NDIS51_MINIPORT_CHARACTERISTICS NDIS_MINIPORT_CHARACTERISTICS;
#else
#ifdef NDIS50_MINIPORT
typedef struct _NDIS50_MINIPORT_CHARACTERISTICS NDIS_MINIPORT_CHARACTERISTICS;
#else
#ifdef NDIS40_MINIPORT
typedef struct _NDIS40_MINIPORT_CHARACTERISTICS NDIS_MINIPORT_CHARACTERISTICS;
#else
typedef struct _NDIS30_MINIPORT_CHARACTERISTICS NDIS_MINIPORT_CHARACTERISTICS;
#endif
#endif
#endif

typedef NDIS_MINIPORT_CHARACTERISTICS * PNDIS_MINIPORT_CHARACTERISTICS;
typedef NDIS_MINIPORT_CHARACTERISTICS   NDIS_WAN_MINIPORT_CHARACTERISTICS;
typedef NDIS_WAN_MINIPORT_CHARACTERISTICS * PNDIS_MINIPORT_CHARACTERISTICS;


//
// Routines for intermediate miniport drivers. NDIS 6 IM drivers
// use the same registration deregistration APIs as regular miniports
//

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisIMRegisterLayeredMiniport(
    _In_  NDIS_HANDLE                     NdisWrapperHandle,
    _In_  PNDIS_MINIPORT_CHARACTERISTICS  MiniportCharacteristics,
    _In_  UINT                            CharacteristicsLength,
    _Out_ PNDIS_HANDLE                    DriverHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisIMDeregisterLayeredMiniport(
    _In_ NDIS_HANDLE         DriverHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisMRegisterDevice(
    _In_  NDIS_HANDLE             NdisWrapperHandle,
    _In_  PNDIS_STRING            DeviceName,
    _In_  PNDIS_STRING            SymbolicName,
    _In_reads_(IRP_MJ_PNP) PDRIVER_DISPATCH *MajorFunctions,
    _Out_ PDEVICE_OBJECT      *   pDeviceObject,
    _Out_ NDIS_HANDLE         *   NdisDeviceHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisMDeregisterDevice(
    _In_ NDIS_HANDLE             NdisDeviceHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisMRegisterUnloadHandler(
    _In_  NDIS_HANDLE             NdisWrapperHandle,
    _In_  PDRIVER_UNLOAD          UnloadHandler
    );

#endif // NDIS_LEGACY_MINIPORT


_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisIMAssociateMiniport(
    _In_  NDIS_HANDLE         DriverHandle,
    _In_  NDIS_HANDLE         ProtocolHandle
    );

//
// Operating System Requests
//
typedef UCHAR   NDIS_DMA_SIZE;

#define NDIS_DMA_24BITS             ((NDIS_DMA_SIZE)0)
#define NDIS_DMA_32BITS             ((NDIS_DMA_SIZE)1)
#define NDIS_DMA_64BITS             ((NDIS_DMA_SIZE)2)

#if NDIS_LEGACY_MINIPORT
_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
__drv_preferredFunction("NdisMInitializeScatterGatherDma", "See details in NdisMAllocateMapRegisters documentation")
EXPORT
NDIS_STATUS
NdisMAllocateMapRegisters(
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  UINT                    DmaChannel,
    _In_  NDIS_DMA_SIZE           DmaSize,
    _In_  ULONG                   BaseMapRegistersNeeded,
    _In_  ULONG                   MaximumPhysicalMapping
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisMFreeMapRegisters(
    _In_ NDIS_HANDLE             MiniportAdapterHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisMInitializeScatterGatherDma(
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  BOOLEAN                 Dma64BitAddresses,
    _In_  ULONG                   MaximumPhysicalMapping
    );

#endif // NDIS_LEGACY_MINIPORT

_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisMRegisterIoPortRange(
    _Out_  PVOID *                 PortOffset,
    _In_   NDIS_HANDLE             MiniportAdapterHandle,
    _In_   UINT                    InitialPort,
    _In_   UINT                    NumberOfPorts
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisMDeregisterIoPortRange(
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  UINT                    InitialPort,
    _In_  UINT                    NumberOfPorts,
    _In_  PVOID                   PortOffset
    );

_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisMMapIoSpace(
    _Outptr_result_bytebuffer_(Length) PVOID *                 VirtualAddress,
    _In_                       NDIS_HANDLE             MiniportAdapterHandle,
    _In_                       NDIS_PHYSICAL_ADDRESS   PhysicalAddress,
    _In_                       UINT                    Length
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisMUnmapIoSpace(
    _In_                 NDIS_HANDLE             MiniportAdapterHandle,
    _In_reads_bytes_(Length)  PVOID                   VirtualAddress,
    _In_                 UINT                    Length
    );

#if NDIS_LEGACY_MINIPORT

EXPORT
NDIS_STATUS
NdisMRegisterInterrupt(
    OUT PNDIS_MINIPORT_INTERRUPT Interrupt,
    IN  NDIS_HANDLE             MiniportAdapterHandle,
    IN  UINT                    InterruptVector,
    IN  UINT                    InterruptLevel,
    IN  BOOLEAN                 RequestIsr,
    IN  BOOLEAN                 SharedInterrupt,
    IN  NDIS_INTERRUPT_MODE     InterruptMode
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisMDeregisterInterrupt(
    _In_ PNDIS_MINIPORT_INTERRUPT Interrupt
    );

EXPORT
BOOLEAN
NdisMSynchronizeWithInterrupt(
    IN  PNDIS_MINIPORT_INTERRUPT Interrupt,
#if (NDIS_SUPPORT_NDIS620)
    IN MINIPORT_SYNCHRONIZE_INTERRUPT_HANDLER   SynchronizeFunction,
#else
    IN  PVOID                   SynchronizeFunction,
#endif
    IN  PVOID                   SynchronizeContext
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisMQueryAdapterResources(
    _Out_         PNDIS_STATUS            Status,
    _In_          NDIS_HANDLE             WrapperConfigurationContext,
    _Out_         PNDIS_RESOURCE_LIST     ResourceList,
    _Inout_ PUINT                   BufferSize
    );

//
// Physical Mapping
//
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMStartBufferPhysicalMapping(
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  PNDIS_BUFFER            Buffer,
    _In_  ULONG                   PhysicalMapRegister,
    _In_  BOOLEAN                 WriteToDevice,
    _Out_ PNDIS_PHYSICAL_ADDRESS_UNIT PhysicalAddressArray,
    _Out_ PUINT                   ArraySize
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMCompleteBufferPhysicalMapping(
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  PNDIS_BUFFER            Buffer,
    _In_  ULONG                   PhysicalMapRegister
    );

/*++
//
// This API has been deprecated
//
VOID
NdisMUpdateSharedMemory(
    IN  NDIS_HANDLE             MiniportAdapterHandle,
    IN  ULONG                   Length,
    IN  PVOID                   VirtualAddress,
    IN  NDIS_PHYSICAL_ADDRESS   PhysicalAddress
    );
*/
#define NdisMUpdateSharedMemory(_H, _L, _V, _P)
#pragma deprecated(NdisMUpdateSharedMemory)


#endif // NDIS_LEGACY_MINIPORT

//
// Timers
//
// VOID
// NdisMSetTimer(
//  IN  PNDIS_MINIPORT_TIMER    Timer,
//  IN  UINT                    MillisecondsToDelay
//  );
#define NdisMSetTimer(_Timer, _Delay)   NdisSetTimer((PNDIS_TIMER)_Timer, _Delay)

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMSetPeriodicTimer(
    _In_  PNDIS_MINIPORT_TIMER     Timer,
    _In_  UINT                     MillisecondPeriod
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMInitializeTimer(
    _In_  OUT PNDIS_MINIPORT_TIMER Timer,
    _In_  NDIS_HANDLE              MiniportAdapterHandle,
    _In_  PNDIS_TIMER_FUNCTION     TimerFunction,
    _In_  PVOID                    FunctionContext
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMCancelTimer(
    _In_  PNDIS_MINIPORT_TIMER    Timer,
    _Out_ _At_(*TimerCancelled, _Must_inspect_result_)
          PBOOLEAN                TimerCancelled
    );

_IRQL_requires_max_(APC_LEVEL)
EXPORT
VOID
NdisMSleep(
    _In_  ULONG                   MicrosecondsToSleep
    );

_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
ULONG
NdisMGetDmaAlignment(
    _In_  NDIS_HANDLE MiniportAdapterHandle
    );

//
// Shared memory
//
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisMAllocateSharedMemory(
    _In_  NDIS_HANDLE               MiniportAdapterHandle,
    _In_  ULONG                     Length,
    _In_  BOOLEAN                   Cached,
    _Outptr_result_bytebuffer_(Length) _At_(*VirtualAddress, _Must_inspect_result_)
          PVOID *                   VirtualAddress,
    _Out_ _At_(*PhysicalAddress, _Must_inspect_result_)
          PNDIS_PHYSICAL_ADDRESS    PhysicalAddress
    );



_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMFreeSharedMemory(
    _In_                 NDIS_HANDLE                  MiniportAdapterHandle,
    _In_                 ULONG                        Length,
    _In_                 BOOLEAN                      Cached,
    _In_reads_bytes_(Length)  PVOID                        VirtualAddress,
    _In_                 NDIS_PHYSICAL_ADDRESS        PhysicalAddress
    );

#if NDIS_LEGACY_MINIPORT

//
// Requests Used by Miniport Drivers
//
#define NdisMInitializeWrapper(_a,_b,_c,_d) NdisInitializeWrapper((_a),(_b),(_c),(_d))


_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisMRegisterMiniport(
    _In_ NDIS_HANDLE             NdisWrapperHandle,
    _In_reads_bytes_(CharacteristicsLength)
         PNDIS_MINIPORT_CHARACTERISTICS MiniportCharacteristics,
    _In_ UINT                    CharacteristicsLength
    );


// EXPORT
// NDIS_STATUS
// NdisIMInitializeDeviceInstance(
//  IN  NDIS_HANDLE             DriverHandle,
//  IN  PNDIS_STRING            DriverInstance
//  );
#define NdisIMInitializeDeviceInstance(_H_, _I_)    \
                                NdisIMInitializeDeviceInstanceEx(_H_, _I_, NULL)
#endif // NDIS_LEGACY_MINIPORT

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisIMInitializeDeviceInstanceEx(
    _In_      NDIS_HANDLE             DriverHandle,
    _In_      PNDIS_STRING            DriverInstance,
    _In_opt_  NDIS_HANDLE             DeviceContext
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisIMCancelInitializeDeviceInstance(
    _In_  NDIS_HANDLE             DriverHandle,
    _In_  PNDIS_STRING            DeviceInstance
    );


#if NDIS_LEGACY_MINIPORT
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_HANDLE
NdisIMGetDeviceContext(
    _In_ NDIS_HANDLE             MiniportAdapterHandle
    );

#endif // NDIS_LEGACY_MINIPORT

_IRQL_requires_max_(APC_LEVEL)
EXPORT
NDIS_HANDLE
NdisIMGetBindingContext(
    _In_ NDIS_HANDLE             NdisBindingHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisIMDeInitializeDeviceInstance(
    _In_ NDIS_HANDLE             NdisMiniportHandle
    );


#if NDIS_LEGACY_MINIPORT

EXPORT
VOID
NdisIMCopySendPerPacketInfo(
    OUT PNDIS_PACKET            DstPacket,
    IN  PNDIS_PACKET            SrcPacket
    );

EXPORT
VOID
NdisIMCopySendCompletePerPacketInfo(
    OUT PNDIS_PACKET            DstPacket,
    IN  PNDIS_PACKET            SrcPacket
    );

// EXPORT
// VOID
// NdisMSetAttributes(
//  IN  NDIS_HANDLE             MiniportAdapterHandle,
//  IN  NDIS_HANDLE             MiniportAdapterContext,
//  IN  BOOLEAN                 BusMaster,
//  IN  NDIS_INTERFACE_TYPE     AdapterType
//  );
#define NdisMSetAttributes(_H_, _C_, _M_, _T_)                                      \
                        NdisMSetAttributesEx(_H_,                                   \
                                             _C_,                                   \
                                             0,                                     \
                                             (_M_) ? NDIS_ATTRIBUTE_BUS_MASTER : 0, \
                                             _T_)                                   \


_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisMSetAttributesEx(
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  NDIS_HANDLE             MiniportAdapterContext,
    _In_  UINT                    CheckForHangTimeInSeconds OPTIONAL,
    _In_  ULONG                   AttributeFlags,
    _In_  NDIS_INTERFACE_TYPE     AdapterType  OPTIONAL
    );


#define NDIS_ATTRIBUTE_IGNORE_PACKET_TIMEOUT        0x00000001
#define NDIS_ATTRIBUTE_IGNORE_REQUEST_TIMEOUT       0x00000002
#define NDIS_ATTRIBUTE_IGNORE_TOKEN_RING_ERRORS     0x00000004
#define NDIS_ATTRIBUTE_BUS_MASTER                   0x00000008
#define NDIS_ATTRIBUTE_INTERMEDIATE_DRIVER          0x00000010
#define NDIS_ATTRIBUTE_DESERIALIZE                  0x00000020
#define NDIS_ATTRIBUTE_NO_HALT_ON_SUSPEND           0x00000040
#define NDIS_ATTRIBUTE_SURPRISE_REMOVE_OK           0x00000080
#define NDIS_ATTRIBUTE_NOT_CO_NDIS                  0x00000100
#define NDIS_ATTRIBUTE_USES_SAFE_BUFFER_APIS        0x00000200
#define NDIS_ATTRIBUTE_DO_NOT_BIND_TO_ALL_CO        0x00000400
#define NDIS_ATTRIBUTE_MINIPORT_PADS_SHORT_PACKETS  0x00000800

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisMSetMiniportSecondary(
    _In_  NDIS_HANDLE             MiniportHandle,
    _In_  NDIS_HANDLE             PrimaryMiniportHandle
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisMPromoteMiniport(
    _In_ NDIS_HANDLE             MiniportHandle
    );

#endif // NDIS_LEGACY_MINIPORT

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisMRemoveMiniport(
    _In_  NDIS_HANDLE             MiniportHandle
    );


#if NDIS_LEGACY_MINIPORT

#define NdisMSendComplete(_M, _P, _S)   (*((PNDIS_MINIPORT_BLOCK)(_M))->SendCompleteHandler)(_M, _P, _S)

#define NdisMSendResourcesAvailable(_M) (*((PNDIS_MINIPORT_BLOCK)(_M))->SendResourcesHandler)(_M)

#if (!NDIS_WRAPPER)
#if (!NDIS_SUPPORT_NDIS6)

#define NdisMResetComplete(_M, _S, _A)  (*((PNDIS_MINIPORT_BLOCK)(_M))->ResetCompleteHandler)(_M, _S, _A)
#endif // NDIS_SUPPORT_NDIS6
#endif // !NDIS_WRAPPER

#define NdisMTransferDataComplete(_M, _P, _S, _B)   \
                                        (*((PNDIS_MINIPORT_BLOCK)(_M))->TDCompleteHandler)(_M, _P, _S, _B)

/*++

VOID
NdisMWanSendComplete(
    IN  NDIS_HANDLE             MiniportAdapterHandle,
    IN  PVOID                   Packet,
    IN  NDIS_STATUS             Status
    );

--*/

#define NdisMWanSendComplete(_M_, _P_, _S_)                                             \
                (*((PNDIS_MINIPORT_BLOCK)(_M_))->WanSendCompleteHandler)(_M_, _P_, _S_)

#define NdisMQueryInformationComplete(_M, _S)   \
                                        (*((PNDIS_MINIPORT_BLOCK)(_M))->QueryCompleteHandler)(_M, _S)

#define NdisMSetInformationComplete(_M, _S) \
                                        (*((PNDIS_MINIPORT_BLOCK)(_M))->SetCompleteHandler)(_M, _S)

/*++

VOID
NdisMIndicateReceivePacket(
    IN  NDIS_HANDLE             MiniportAdapterHandle,
    IN  PPNDIS_PACKET           ReceivedPackets,
    IN  UINT                    NumberOfPackets
    );

--*/
#define NdisMIndicateReceivePacket(_H, _P, _N)                                  \
{                                                                               \
    (*((PNDIS_MINIPORT_BLOCK)(_H))->PacketIndicateHandler)(                     \
                        _H,                                                     \
                        _P,                                                     \
                        _N);                                                    \
}

/*++

VOID
NdisMWanIndicateReceive(
    OUT PNDIS_STATUS            Status,
    IN NDIS_HANDLE              MiniportAdapterHandle,
    IN NDIS_HANDLE              NdisLinkContext,
    IN PUCHAR                   Packet,
    IN ULONG                    PacketSize
    );

--*/

#define NdisMWanIndicateReceive(_S_, _M_, _C_, _P_, _Z_)                        \
                (*((PNDIS_MINIPORT_BLOCK)(_M_))->WanRcvHandler)(_S_, _M_, _C_, _P_, _Z_)

/*++

VOID
NdisMWanIndicateReceiveComplete(
    IN NDIS_HANDLE              MiniportAdapterHandle,
    IN NDIS_HANDLE              NdisLinkContext
    );

--*/

#define NdisMWanIndicateReceiveComplete(_M_, _C_)                                   \
                (*((PNDIS_MINIPORT_BLOCK)(_M_))->WanRcvCompleteHandler)(_M_, _C_)

/*++

VOID
NdisMEthIndicateReceive(
    IN  NDIS_HANDLE             MiniportAdapterHandle,
    IN  NDIS_HANDLE             MiniportReceiveContext,
    IN  PVOID                   HeaderBuffer,
    IN  UINT                    HeaderBufferSize,
    IN  PVOID                   LookaheadBuffer,
    IN  UINT                    LookaheadBufferSize,
    IN  UINT                    PacketSize
    )

--*/
#ifdef __cplusplus

#define NdisMEthIndicateReceive( _H, _C, _B, _SZ, _L, _LSZ, _PSZ)               \
{                                                                               \
    (*((PNDIS_MINIPORT_BLOCK)(_H))->EthRxIndicateHandler)(                      \
        ((PNDIS_MINIPORT_BLOCK)(_H))->FilterDbs.EthDB,                          \
        _C,                                                                     \
        _B,                                                                     \
        _B,                                                                     \
        _SZ,                                                                    \
        _L,                                                                     \
        _LSZ,                                                                   \
        _PSZ                                                                    \
        );                                                                      \
}

#else

#define NdisMEthIndicateReceive( _H, _C, _B, _SZ, _L, _LSZ, _PSZ)               \
{                                                                               \
    (*((PNDIS_MINIPORT_BLOCK)(_H))->EthRxIndicateHandler)(                      \
        ((PNDIS_MINIPORT_BLOCK)(_H))->EthDB,                                    \
        _C,                                                                     \
        _B,                                                                     \
        _B,                                                                     \
        _SZ,                                                                    \
        _L,                                                                     \
        _LSZ,                                                                   \
        _PSZ                                                                    \
        );                                                                      \
}

#endif


/*++

VOID
NdisMTrIndicateReceive(
    IN  NDIS_HANDLE             MiniportAdapterHandle,
    IN  NDIS_HANDLE             MiniportReceiveContext,
    IN  PVOID                   HeaderBuffer,
    IN  UINT                    HeaderBufferSize,
    IN  PVOID                   LookaheadBuffer,
    IN  UINT                    LookaheadBufferSize,
    IN  UINT                    PacketSize
    )

--*/
#if (NTDDI_VERSION >= NTDDI_WIN8)

#define NdisMTrIndicateReceive TokenRingIsNotSupportedOnWin8OrLater

#else // WIN8+

#ifdef __cplusplus
#define NdisMTrIndicateReceive( _H, _C, _B, _SZ, _L, _LSZ, _PSZ)                \
{                                                                               \
    (*((PNDIS_MINIPORT_BLOCK)(_H))->TrRxIndicateHandler)(                       \
        ((PNDIS_MINIPORT_BLOCK)(_H))->FilterDbs.TrDB,                                     \
        _C,                                                                     \
        _B,                                                                     \
        _SZ,                                                                    \
        _L,                                                                     \
        _LSZ,                                                                   \
        _PSZ                                                                    \
        );                                                                      \
}

#else

#define NdisMTrIndicateReceive( _H, _C, _B, _SZ, _L, _LSZ, _PSZ)                \
{                                                                               \
    (*((PNDIS_MINIPORT_BLOCK)(_H))->TrRxIndicateHandler)(                       \
        ((PNDIS_MINIPORT_BLOCK)(_H))->TrDB,                                     \
        _C,                                                                     \
        _B,                                                                     \
        _SZ,                                                                    \
        _L,                                                                     \
        _LSZ,                                                                   \
        _PSZ                                                                    \
        );                                                                      \
}
#endif

#endif // WIN8+

/*++

VOID
NdisMEthIndicateReceiveComplete(
    IN  NDIS_HANDLE             MiniportHandle
    );

--*/
#ifdef __cplusplus

#define NdisMEthIndicateReceiveComplete( _H )                                   \
{                                                                               \
    (*((PNDIS_MINIPORT_BLOCK)(_H))->EthRxCompleteHandler)(                      \
                                        ((PNDIS_MINIPORT_BLOCK)_H)->FilterDbs.EthDB);     \
}

#else

#define NdisMEthIndicateReceiveComplete( _H )                                   \
{                                                                               \
    (*((PNDIS_MINIPORT_BLOCK)(_H))->EthRxCompleteHandler)(                      \
                                        ((PNDIS_MINIPORT_BLOCK)_H)->EthDB);               \
}

#endif


/*++

VOID
NdisMTrIndicateReceiveComplete(
    IN  NDIS_HANDLE             MiniportHandle
    );

--*/
#if (NTDDI_VERSION >= NTDDI_WIN8)

#define NdisMTrIndicateReceive TokenRingIsNotSupportedOnWin8OrLater

#else // WIN8+

#ifdef __cplusplus
#define NdisMTrIndicateReceiveComplete( _H )                                    \
{                                                                               \
    (*((PNDIS_MINIPORT_BLOCK)(_H))->TrRxCompleteHandler)(                       \
                                        ((PNDIS_MINIPORT_BLOCK)_H)->FilterDbs.TrDB);      \
}
#else
#define NdisMTrIndicateReceiveComplete( _H )                                    \
{                                                                               \
    (*((PNDIS_MINIPORT_BLOCK)(_H))->TrRxCompleteHandler)(                       \
                                        ((PNDIS_MINIPORT_BLOCK)_H)->TrDB);      \
}
#endif
#endif // WIN8+


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMIndicateStatus(
    _In_  NDIS_HANDLE             MiniportHandle,
    _In_  NDIS_STATUS             GeneralStatus,
    _In_reads_bytes_(StatusBufferSize)
          PVOID                   StatusBuffer,
    _In_  UINT                    StatusBufferSize
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMIndicateStatusComplete(
    _In_ NDIS_HANDLE             MiniportHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisMRegisterAdapterShutdownHandler(
    _In_  NDIS_HANDLE              MiniportHandle,
    _In_  PVOID                    ShutdownContext,
    _In_  ADAPTER_SHUTDOWN_HANDLER ShutdownHandler
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisMDeregisterAdapterShutdownHandler(
    _In_ NDIS_HANDLE             MiniportHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
__drv_preferredFunction("NdisMQueryAdapterResources", "Obsolete")
EXPORT
NDIS_STATUS
NdisMPciAssignResources(
    _In_  NDIS_HANDLE             MiniportHandle,
    _In_  ULONG                   SlotNumber,
    _Out_ PNDIS_RESOURCE_LIST *   AssignedResources
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisIMNotifyPnPEvent(
    _In_  NDIS_HANDLE             MiniportHandle,
    _In_  PNET_PNP_EVENT          NetPnPEvent
    );

#endif // NDIS_LEGACY_MINIPORT


//
// Logging support for miniports
//

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisMCreateLog(
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  UINT                    Size,
    _Out_ PNDIS_HANDLE            LogHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisMCloseLog(
    _In_ NDIS_HANDLE             LogHandle
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisMWriteLogData(
    _In_ NDIS_HANDLE             LogHandle,
    _In_reads_bytes_(LogBufferSize)
         PVOID                   LogBuffer,
    _In_ UINT                    LogBufferSize
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMFlushLog(
    _In_ NDIS_HANDLE             LogHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisMGetDeviceProperty(
    _In_          NDIS_HANDLE          MiniportAdapterHandle,
    _Inout_opt_   PDEVICE_OBJECT *     PhysicalDeviceObject,
    _Inout_opt_   PDEVICE_OBJECT *     FunctionalDeviceObject,
    _Inout_opt_   PDEVICE_OBJECT *     NextDeviceObject,
    _Inout_opt_   PCM_RESOURCE_LIST *  AllocatedResources,
    _Inout_opt_   PCM_RESOURCE_LIST *  AllocatedResourcesTranslated
    );

//
//  Get a pointer to the adapter's localized instance name.
//
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisMQueryAdapterInstanceName(
    _Out_ PNDIS_STRING            pAdapterInstanceName,
    _In_  NDIS_HANDLE             MiniportHandle
    );


#if NDIS_LEGACY_MINIPORT

//
// NDIS 5.0 extensions for miniports
//

_IRQL_requires_max_(DISPATCH_LEVEL)
_IRQL_requires_min_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMCoIndicateReceivePacket(
    _In_  NDIS_HANDLE             NdisVcHandle,
    _In_  PPNDIS_PACKET           PacketArray,
    _In_  UINT                    NumberOfPackets
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMCoIndicateStatus(
    _In_      NDIS_HANDLE             MiniportAdapterHandle,
    _In_opt_  NDIS_HANDLE             NdisVcHandle,
    _In_      NDIS_STATUS             GeneralStatus,
    _In_reads_bytes_opt_(StatusBufferSize)
              PVOID                   StatusBuffer,
    _In_      ULONG                   StatusBufferSize
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
_IRQL_requires_min_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMCoReceiveComplete(
    _In_ NDIS_HANDLE             MiniportAdapterHandle
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
_IRQL_requires_min_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMCoSendComplete(
    _In_  NDIS_STATUS             Status,
    _In_  NDIS_HANDLE             NdisVcHandle,
    _In_  PNDIS_PACKET            Packet
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMCoRequestComplete(
    _In_  NDIS_STATUS             Status,
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  PNDIS_REQUEST           Request
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisMCmRequest(
    _In_      NDIS_HANDLE             NdisAfHandle,
    _In_opt_  NDIS_HANDLE             NdisVcHandle,
    _In_opt_  NDIS_HANDLE             NdisPartyHandle,
    _Inout_   PNDIS_REQUEST           NdisRequest
    );

#endif // NDIS_LEGACY_MINIPORT

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMCoActivateVcComplete(
    _In_  NDIS_STATUS             Status,
    _In_  NDIS_HANDLE             NdisVcHandle,
    _In_  PCO_CALL_PARAMETERS     CallParameters
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMCoDeactivateVcComplete(
    _In_  NDIS_STATUS             Status,
    _In_  NDIS_HANDLE             NdisVcHandle
    );

EXPORT
NDIS_STATUS
NdisMCmRegisterAddressFamily(
    IN  NDIS_HANDLE             MiniportAdapterHandle,
    IN  PCO_ADDRESS_FAMILY      AddressFamily,
    IN  PNDIS_CALL_MANAGER_CHARACTERISTICS CmCharacteristics,
    IN  UINT                    SizeOfCmCharacteristics
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisMCmCreateVc(
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  NDIS_HANDLE             NdisAfHandle,
    _In_  NDIS_HANDLE             MiniportVcContext,
    _Out_ PNDIS_HANDLE            NdisVcHandle
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisMCmDeleteVc(
    _In_ NDIS_HANDLE             NdisVcHandle
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisMCmActivateVc(
    _In_  NDIS_HANDLE             NdisVcHandle,
    _In_  PCO_CALL_PARAMETERS     CallParameters
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisMCmDeactivateVc(
    _In_ NDIS_HANDLE             NdisVcHandle
    );


#if NDIS_LEGACY_MINIPORT

// EXPORT
// VOID
// NdisMCmRequestComplete(
//  IN  NDIS_STATUS             Status,
//  IN  NDIS_HANDLE             NdisAfHandle,
//  IN  NDIS_HANDLE             NdisVcHandle    OPTIONAL,
//  IN  NDIS_HANDLE             NdisPartyHandle OPTIONAL,
//  IN  PNDIS_REQUEST           NdisRequest
//  );
#define NdisMCmRequestComplete(_S_, _AH_, _VH_, _PH_, _R_) \
                                        NdisCoRequestComplete(_S_, _AH_, _VH_, _PH_, _R_)
#endif // NDIS_LEGACY_MINIPORT

#if (NDIS_SUPPORT_NDIS6 || NDIS60)

// EXPORT
// VOID
// NdisMCmOidRequestComplete(
//  IN  NDIS_HANDLE             NdisAfHandle,
//  IN  NDIS_HANDLE             NdisVcHandle    OPTIONAL,
//  IN  NDIS_HANDLE             NdisPartyHandle OPTIONAL,
//  IN  PNDIS_OID_REQUEST       OidRequest,
//  IN  NDIS_STATUS             Status
//  );
#define NdisMCmOidRequestComplete(_AH_, _VH_, _PH_, _R_, _S_) \
                                        NdisCoOidRequestComplete(_AH_, _VH_, _PH_, _R_, _S_)
#endif // NDIS_SUPPORT_NDIS6 || NDIS60


// EXPORT
// VOID
// NdisMCmOpenAddressFamilyComplete(
//  IN  NDIS_STATUS             Status,
//  IN  NDIS_HANDLE             NdisAfHandle,
//  IN  NDIS_HANDLE             CallMgrAfContext
//  );

#define NdisMCmOpenAddressFamilyComplete(_S_, _H_, _C_) \
                                        NdisCmOpenAddressFamilyComplete(_S_, _H_, _C_)
// EXPORT
// NDIS_STATUS
// NdisMCmNotifyCloseAddressFamily(
//   IN  NDIS_HANDLE              NdisAfHandle
// );

#define NdisMCmNotifyCloseAddressFamily(_AH_) \
                                         NdisCmNotifyCloseAddressFamily(_AH_)

// EXPORT
// VOID
// NdisMCmCloseAddressFamilyComplete(
//  IN  NDIS_STATUS             Status,
//  IN  NDIS_HANDLE             NdisAfHandle
//  );

#define NdisMCmCloseAddressFamilyComplete(_S_, _H_)     \
                                        NdisCmCloseAddressFamilyComplete(_S_, _H_)



// EXPORT
// VOID
// NdisMCmRegisterSapComplete(
//  IN  NDIS_STATUS             Status,
//  IN  NDIS_HANDLE             NdisSapHandle,
//  IN  NDIS_HANDLE             CallMgrSapContext
//  );

#define NdisMCmRegisterSapComplete(_S_, _H_, _C_)       \
                                        NdisCmRegisterSapComplete(_S_, _H_, _C_)


// EXPORT
// VOID
// NdisMCmDeregisterSapComplete(
//  IN  NDIS_STATUS             Status,
//  IN  NDIS_HANDLE             NdisSapHandle
//  );

#define NdisMCmDeregisterSapComplete(_S_, _H_)          \
                                        NdisCmDeregisterSapComplete(_S_, _H_)


// EXPORT
// VOID
// NdisMCmMakeCallComplete(
//  IN  NDIS_STATUS             Status,
//  IN  NDIS_HANDLE             NdisVcHandle,
//  IN  NDIS_HANDLE             NdisPartyHandle     OPTIONAL,
//  IN  NDIS_HANDLE             CallMgrPartyContext OPTIONAL,
//  IN  PCO_CALL_PARAMETERS     CallParameters
//  );

#define NdisMCmMakeCallComplete(_S_, _VH_, _PH_, _CC_, _CP_)    \
                                        NdisCmMakeCallComplete(_S_, _VH_, _PH_, _CC_, _CP_)


// EXPORT
// VOID
// NdisMCmCloseCallComplete(
//  IN  NDIS_STATUS             Status,
//  IN  NDIS_HANDLE             NdisVcHandle,
//  IN  NDIS_HANDLE             NdisPartyHandle OPTIONAL
//  );

#define NdisMCmCloseCallComplete(_S_, _VH_, _PH_)       \
                                        NdisCmCloseCallComplete(_S_, _VH_, _PH_)


// EXPORT
// VOID
// NdisMCmAddPartyComplete(
//  IN  NDIS_STATUS             Status,
//  IN  NDIS_HANDLE             NdisPartyHandle,
//  IN  NDIS_HANDLE             CallMgrPartyContext OPTIONAL,
//  IN  PCO_CALL_PARAMETERS     CallParameters
//  );

#define NdisMCmAddPartyComplete(_S_, _H_, _C_, _P_)     \
                                        NdisCmAddPartyComplete(_S_, _H_, _C_, _P_)


// EXPORT
// VOID
// NdisMCmDropPartyComplete(
//  IN  NDIS_STATUS             Status,
//  IN  NDIS_HANDLE             NdisPartyHandle
//  );

#define NdisMCmDropPartyComplete(_S_, _H_)              \
                                        NdisCmDropPartyComplete(_S_, _H_)


// EXPORT
// NDIS_STATUS
// NdisMCmDispatchIncomingCall(
//  IN  NDIS_HANDLE             NdisSapHandle,
//  IN  NDIS_HANDLE             NdisVcHandle,
//  IN  PCO_CALL_PARAMETERS     CallParameters
//  );

#define NdisMCmDispatchIncomingCall(_SH_, _VH_, _CP_)   \
                                        NdisCmDispatchIncomingCall(_SH_, _VH_, _CP_)


// EXPORT
// VOID
// NdisMCmDispatchCallConnected(
//  IN  NDIS_HANDLE             NdisVcHandle
//  );

#define NdisMCmDispatchCallConnected(_H_)               \
                                        NdisCmDispatchCallConnected(_H_)


// EXPORT
// NdisMCmModifyCallQoSComplete(
//  IN  NDIS_STATUS             Status,
//  IN  NDIS_HANDLE             NdisVcHandle,
//  IN  PCO_CALL_PARAMETERS     CallParameters
//  );

#define NdisMCmModifyCallQoSComplete(_S_, _H_, _P_)     \
                                        NdisCmModifyCallQoSComplete(_S_, _H_, _P_)


// EXPORT
// VOID
// VOID
// NdisMCmDispatchIncomingCallQoSChange(
//  IN  NDIS_HANDLE             NdisVcHandle,
//  IN  PCO_CALL_PARAMETERS     CallParameters
//  );

#define NdisMCmDispatchIncomingCallQoSChange(_H_, _P_)  \
                                        NdisCmDispatchIncomingCallQoSChange(_H_, _P_)


// EXPORT
// VOID
// NdisMCmDispatchIncomingCloseCall(
//   IN  NDIS_STATUS             CloseStatus,
//   IN  NDIS_HANDLE             NdisVcHandle,
//   IN  PVOID                   Buffer         OPTIONAL,
//   IN  UINT                    Size
//   );

#define NdisMCmDispatchIncomingCloseCall(_S_, _H_, _B_, _Z_)    \
                                        NdisCmDispatchIncomingCloseCall(_S_, _H_, _B_, _Z_)


//  EXPORT
//  VOID
//  NdisMCmDispatchIncomingDropParty(
//      IN  NDIS_STATUS         DropStatus,
//      IN  NDIS_HANDLE         NdisPartyHandle,
//      IN  PVOID               Buffer      OPTIONAL,
//      IN  UINT                Size
//      );
#define NdisMCmDispatchIncomingDropParty(_S_, _H_, _B_, _Z_)    \
                                        NdisCmDispatchIncomingDropParty(_S_, _H_, _B_, _Z_)


#if NDIS_SUPPORT_NDIS6

_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisMRegisterScatterGatherDma(
    _In_  NDIS_HANDLE                     MiniportAdapterHandle,
    _In_  PNDIS_SG_DMA_DESCRIPTION        DmaDescription,
    _Out_ PNDIS_HANDLE                    NdisMiniportDmaHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisMDeregisterScatterGatherDma(
    _In_  NDIS_HANDLE                     NdisMiniportDmaHandle
    );

//
// flags used in the call to NdisMAllocateNetBufferSGList
//
#define NDIS_SG_LIST_WRITE_TO_DEVICE    0x000000001


_Must_inspect_result_
_IRQL_requires_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisMAllocateNetBufferSGList(
    _In_       NDIS_HANDLE             NdisMiniportDmaHandle,
    _In_       PNET_BUFFER             NetBuffer,
    _In_       _Points_to_data_
               PVOID                   Context,
    _In_       ULONG                   Flags,
    _In_reads_bytes_opt_(ScatterGatherListBufferSize) _Points_to_data_
               PVOID                   ScatterGatherListBuffer,
    _In_       ULONG                   ScatterGatherListBufferSize
    );

_IRQL_requires_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMFreeNetBufferSGList(
    _In_  NDIS_HANDLE             NdisMiniportDmaHandle,
    _In_  PSCATTER_GATHER_LIST    pSGL,
    _In_  PNET_BUFFER             NetBuffer
    );


_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisMRegisterMiniportDriver(
    _In_     PDRIVER_OBJECT                              DriverObject,
    _In_     PUNICODE_STRING                             RegistryPath,
    _In_opt_ NDIS_HANDLE                                 MiniportDriverContext,
    _In_     PNDIS_MINIPORT_DRIVER_CHARACTERISTICS       MiniportDriverCharacteristics,
    _Out_    PNDIS_HANDLE                                NdisMiniportDriverHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisMDeregisterMiniportDriver(
    _In_ NDIS_HANDLE              NdisMiniportDriverHandle
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMPauseComplete(
    _In_  NDIS_HANDLE             MiniportAdapterHandle
    );

EXPORT
VOID
NdisMRestartComplete(
    IN  NDIS_HANDLE             MiniportAdapterHandle,
    IN  NDIS_STATUS             Status
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMOidRequestComplete(
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  PNDIS_OID_REQUEST       OidRequest,
    _In_  NDIS_STATUS             Status
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMSendNetBufferListsComplete(
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  PNET_BUFFER_LIST        NetBufferList,
    _In_  ULONG                   SendCompleteFlags
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMIndicateReceiveNetBufferLists(
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  PNET_BUFFER_LIST        NetBufferList,
    _In_  NDIS_PORT_NUMBER        PortNumber,
    _In_  ULONG                   NumberOfNetBufferLists,
    _In_  ULONG                   ReceiveFlags
    );


//
// IO workitem routines
//

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_HANDLE
NdisAllocateIoWorkItem(
    _In_  NDIS_HANDLE                 NdisObjectHandle
    );

typedef
_IRQL_requires_(PASSIVE_LEVEL)
_IRQL_requires_same_
_Function_class_(NDIS_IO_WORKITEM_FUNCTION)
VOID
(NDIS_IO_WORKITEM_FUNCTION)(
    _In_opt_ PVOID                        WorkItemContext,
    _In_     NDIS_HANDLE                  NdisIoWorkItemHandle
    );
typedef NDIS_IO_WORKITEM_FUNCTION (*NDIS_IO_WORKITEM_ROUTINE);

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisQueueIoWorkItem(
    _In_                       NDIS_HANDLE                 NdisIoWorkItemHandle,
    _In_                       NDIS_IO_WORKITEM_ROUTINE    Routine,
    _In_ _Points_to_data_ PVOID                       WorkItemContext
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFreeIoWorkItem(
    _In_ NDIS_HANDLE            NdisIoWorkItemHandle
    );

//
// CONDIS 6.0 extensions
//

EXPORT
VOID
NdisMCoSendNetBufferListsComplete(
    IN NDIS_HANDLE          NdisVcHandle,
    IN PNET_BUFFER_LIST     NetBufferLists,
    IN ULONG                SendCompleteFlags
    );

EXPORT
VOID
NdisMCoIndicateReceiveNetBufferLists(
    IN NDIS_HANDLE          NdisVcHandle,
    IN PNET_BUFFER_LIST     NetBufferLists,
    IN ULONG                NumberOfNetBufferLists,
    IN ULONG                CoReceiveFlags
    );

_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisMNetPnPEvent(
    _In_  NDIS_HANDLE                 MiniportAdapterHandle,
    _In_  PNET_PNP_EVENT_NOTIFICATION NetPnPEventNotification
    );


#if NDIS_SUPPORT_NDIS6
_IRQL_requires_max_(DISPATCH_LEVEL)
#endif NDIS_SUPPORT_NDIS6
EXPORT
VOID
NdisMResetComplete(
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  NDIS_STATUS             Status,
    _In_  BOOLEAN                 AddressingReset
    );

_Must_inspect_result_
_IRQL_requires_max_(HIGH_LEVEL)
EXPORT
ULONG
NdisMGetBusData(
    _In_ NDIS_HANDLE              NdisMiniportHandle,
    _In_ ULONG                    WhichSpace,
    _In_ ULONG                    Offset,
    _Out_writes_bytes_all_(Length) PVOID  Buffer,
    _In_ ULONG                    Length
    );

_IRQL_requires_max_(HIGH_LEVEL)
EXPORT
ULONG
NdisMSetBusData(
    IN NDIS_HANDLE              NdisMiniportHandle,
    IN ULONG                    WhichSpace,
    IN ULONG                    Offset,
    IN PVOID                    Buffer,
    IN ULONG                    Length
    );

//
// NDIS port APIs
//

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisMAllocatePort(
    _In_  NDIS_HANDLE                   NdisMiniportHandle,
    _Inout_ PNDIS_PORT_CHARACTERISTICS  PortCharacteristics
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisMFreePort(
    _In_  NDIS_HANDLE                   NdisMiniportHandle,
    _In_  NDIS_PORT_NUMBER              PortNumber
    );

#if (NDIS_SUPPORT_NDIS61)
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMDirectOidRequestComplete(
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  PNDIS_OID_REQUEST       OidRequest,
    _In_  NDIS_STATUS             Status
    );

typedef enum _NDIS_MSIX_TABLE_CONFIG
{
    NdisMSIXTableConfigSetTableEntry,
    NdisMSIXTableConfigMaskTableEntry,
    NdisMSIXTableConfigUnmaskTableEntry,
    NdisMSIXTableConfigMax
}NDIS_MSIX_TABLE_OPERATION, *PNDIS_MSIX_TABLE_OPERATION;

#define NDIS_MSIX_CONFIG_PARAMETERS_REVISION_1          1

typedef struct _NDIS_MSIX_CONFIG_PARAMETERS
{
    NDIS_OBJECT_HEADER              Header;
    NDIS_MSIX_TABLE_OPERATION       ConfigOperation;
    ULONG                           TableEntry;
    ULONG                           MessageNumber;
}NDIS_MSIX_CONFIG_PARAMETERS, *PNDIS_MSIX_CONFIG_PARAMETERS;

#define NDIS_SIZEOF_MSIX_CONFIG_PARAMETERS_REVISION_1      \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_MSIX_CONFIG_PARAMETERS, MessageNumber)

_IRQL_requires_max_(HIGH_LEVEL)
EXPORT
NDIS_STATUS
NdisMConfigMSIXTableEntry(
    _In_  NDIS_HANDLE                     NdisMiniportHandle,
    _In_  PNDIS_MSIX_CONFIG_PARAMETERS    MSIXConfigParameters
    );

#if (NDIS_SUPPORT_NDIS630)

//
// New APIs for PF and VF miniport drivers
//
_IRQL_requires_max_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisMEnableVirtualization(
    _In_  NDIS_HANDLE               NdisMiniportHandle,
    _In_  USHORT                    NumVFs,
    _In_  BOOLEAN                   EnableVFMigration,
    _In_  BOOLEAN                   EnableMigrationInterrupt,
    _In_  BOOLEAN                   EnableVirtualization
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
ULONG
NdisMGetVirtualFunctionBusData(
    _In_  NDIS_HANDLE                   NdisMiniportHandle,
    _In_  NDIS_SRIOV_FUNCTION_ID        VFId,
    _Out_writes_bytes_(Length) PVOID    Buffer,
    _In_  ULONG                         Offset,
    _In_  ULONG                         Length
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
ULONG
NdisMSetVirtualFunctionBusData(
    _In_  NDIS_HANDLE               NdisMiniportHandle,
    _In_  NDIS_SRIOV_FUNCTION_ID    VFId,
    _In_reads_bytes_(Length) PVOID  Buffer,
    _In_  ULONG                     Offset,
    _In_  ULONG                     Length
    );

_IRQL_requires_max_(PASSIVE_LEVEL)
EXPORT
VOID
NdisMGetVirtualDeviceLocation (
    _In_        NDIS_HANDLE             NdisMiniportHandle,
    _In_        NDIS_SRIOV_FUNCTION_ID  VFId,
    _Out_       PUSHORT                 SegmentNumber,
    _Out_       PUCHAR                  BusNumber,
    _In_opt_    PVOID                   Reserved,
    _Out_       PUCHAR                  FunctionNumber
    );

_IRQL_requires_max_(PASSIVE_LEVEL)
EXPORT
VOID
NdisMGetVirtualFunctionLocation (
    _In_        NDIS_HANDLE             NdisMiniportHandle,
    _In_        NDIS_SRIOV_FUNCTION_ID  VFId,
    _Out_       PUSHORT                 SegmentNumber,
    _Out_       PUCHAR                  BusNumber,
    _Out_       PUCHAR                  FunctionNumber
    );

_IRQL_requires_max_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisMQueryProbedBars(
    _In_                              NDIS_HANDLE   NdisMiniportHandle,
    _Out_writes_(PCI_TYPE0_ADDRESSES) PULONG        BaseRegisterValues
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMInvalidateConfigBlock(
    _In_  NDIS_HANDLE               NdisMiniportHandle,
    _In_  NDIS_SRIOV_FUNCTION_ID    VFId,
    _In_  ULONGLONG                 BlockMask
    );

_IRQL_requires_max_(APC_LEVEL)
EXPORT
NDIS_STATUS
NdisMWriteConfigBlock(
    _In_  NDIS_HANDLE               NdisMiniportHandle,
    _In_  ULONG                     BlockId,
    _In_reads_bytes_(Length) PVOID  Buffer,
    _In_  ULONG                     Length
    );

_IRQL_requires_max_(APC_LEVEL)
EXPORT
NDIS_STATUS
NdisMReadConfigBlock(
    _In_  NDIS_HANDLE                   NdisMiniportHandle,
    _In_  ULONG                         BlockId,
    _Out_writes_bytes_(Length) PVOID    Buffer,
    _In_  ULONG                         Length
    );

#define NDIS_MAKE_RID(_Segment, _Bus, _Function) (((_Segment) << 16) | ((_Bus) << 8) | (_Function))


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMResetMiniport(
    _In_ NDIS_HANDLE  MiniportAdapterHandle
    );

//
// NDIS 6.30 Selective Suspend Support
//

typedef
_IRQL_requires_(PASSIVE_LEVEL)
NDIS_STATUS
(MINIPORT_IDLE_NOTIFICATION)(
    _In_ NDIS_HANDLE        MiniportAdapterContext,
    _In_ BOOLEAN            ForceIdle
    );
typedef MINIPORT_IDLE_NOTIFICATION (*MINIPORT_IDLE_NOTIFICATION_HANDLER);


typedef
_IRQL_requires_(PASSIVE_LEVEL)
VOID
(MINIPORT_CANCEL_IDLE_NOTIFICATION)(
    _In_ NDIS_HANDLE        MiniportAdapterContext
    );
typedef MINIPORT_CANCEL_IDLE_NOTIFICATION (*MINIPORT_CANCEL_IDLE_NOTIFICATION_HANDLER);


//
// NDIS_MINIPORT_SS_CHARACTERISTICS struct specifies miniport's Selective Suspend handlers
// Used with NdisSetOptionalHandlers function
//
#define NDIS_MINIPORT_SS_CHARACTERISTICS_REVISION_1     1

typedef struct _NDIS_MINIPORT_SS_CHARACTERISTICS {
    //
    // Header.Type = NDIS_OBJECT_TYPE_MINIPORT_SS_CHARACTERISTICS;
    // Header.Revision = NDIS_MINIPORT_SS_CHARACTERISTICS_REVISION_1;
    // Header.Size = NDIS_SIZEOF_MINIPORT_SS_CHARACTERISTICS_REVISION_1;
    //
    NDIS_OBJECT_HEADER                            Header;

    ULONG                                         Flags;    // Reserved
    MINIPORT_IDLE_NOTIFICATION_HANDLER            IdleNotificationHandler;
    MINIPORT_CANCEL_IDLE_NOTIFICATION_HANDLER     CancelIdleNotificationHandler;
} NDIS_MINIPORT_SS_CHARACTERISTICS, *PNDIS_MINIPORT_SS_CHARACTERISTICS;

#define NDIS_SIZEOF_MINIPORT_SS_CHARACTERISTICS_REVISION_1 \
    RTL_SIZEOF_THROUGH_FIELD(NDIS_MINIPORT_SS_CHARACTERISTICS, CancelIdleNotificationHandler)


_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisMIdleNotificationConfirm(
    _In_ NDIS_HANDLE MiniportAdapterHandle,
    _In_ NDIS_DEVICE_POWER_STATE IdlePowerState
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisMIdleNotificationComplete(
    _In_ NDIS_HANDLE MiniportAdapterHandle
    );

//
// NDIS Switch callback handlers
//
typedef PVOID NDIS_SWITCH_CONTEXT;
typedef NDIS_SWITCH_CONTEXT (*PNDIS_SWITCH_CONTEXT);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
NDIS_STATUS
(NDIS_SWITCH_ALLOCATE_NET_BUFFER_LIST_FORWARDING_CONTEXT)(
    _In_    NDIS_SWITCH_CONTEXT NdisSwitchContext,
    _Inout_ PNET_BUFFER_LIST    NetBufferList
    );

typedef NDIS_SWITCH_ALLOCATE_NET_BUFFER_LIST_FORWARDING_CONTEXT
            (*NDIS_SWITCH_ALLOCATE_NET_BUFFER_LIST_FORWARDING_CONTEXT_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
VOID
(NDIS_SWITCH_FREE_NET_BUFFER_LIST_FORWARDING_CONTEXT)(
    _In_    NDIS_SWITCH_CONTEXT    NdisSwitchContext,
    _Inout_ PNET_BUFFER_LIST       NetBufferList
    );

typedef NDIS_SWITCH_FREE_NET_BUFFER_LIST_FORWARDING_CONTEXT
            (*NDIS_SWITCH_FREE_NET_BUFFER_LIST_FORWARDING_CONTEXT_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
NDIS_STATUS
(NDIS_SWITCH_SET_NET_BUFFER_LIST_SOURCE)(
    _In_    NDIS_SWITCH_CONTEXT             NdisSwitchContext,
    _Inout_ PNET_BUFFER_LIST                NetBufferList,
    _In_    NDIS_SWITCH_PORT_ID             PortId,
    _In_    NDIS_SWITCH_NIC_INDEX           NicIndex
    );

typedef NDIS_SWITCH_SET_NET_BUFFER_LIST_SOURCE
            (*NDIS_SWITCH_SET_NET_BUFFER_LIST_SOURCE_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
NDIS_STATUS
(NDIS_SWITCH_ADD_NET_BUFFER_LIST_DESTINATION)(
    _In_    NDIS_SWITCH_CONTEXT             NdisSwitchContext,
    _Inout_ PNET_BUFFER_LIST                NetBufferList,
    _In_    PNDIS_SWITCH_PORT_DESTINATION   Destination
    );

typedef NDIS_SWITCH_ADD_NET_BUFFER_LIST_DESTINATION
            (*NDIS_SWITCH_ADD_NET_BUFFER_LIST_DESTINATION_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
NDIS_STATUS
(NDIS_SWITCH_GROW_NET_BUFFER_LIST_DESTINATIONS)(
    _In_     NDIS_SWITCH_CONTEXT                        NdisSwitchContext,
    _Inout_  PNET_BUFFER_LIST                           NetBufferList,
    _In_     UINT32                                     NumberOfNewDestinations,
    _Outptr_ PNDIS_SWITCH_FORWARDING_DESTINATION_ARRAY* Destinations
    );
typedef NDIS_SWITCH_GROW_NET_BUFFER_LIST_DESTINATIONS
            (*NDIS_SWITCH_GROW_NET_BUFFER_LIST_DESTINATIONS_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
VOID
(NDIS_SWITCH_GET_NET_BUFFER_LIST_DESTINATIONS)(
    _In_     NDIS_SWITCH_CONTEXT                        NdisSwitchContext,
    _Inout_  PNET_BUFFER_LIST                           NetBufferList,
    _Outptr_ PNDIS_SWITCH_FORWARDING_DESTINATION_ARRAY* Destinations
    );

typedef NDIS_SWITCH_GET_NET_BUFFER_LIST_DESTINATIONS
            (*NDIS_SWITCH_GET_NET_BUFFER_LIST_DESTINATIONS_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
NDIS_STATUS
(NDIS_SWITCH_UPDATE_NET_BUFFER_LIST_DESTINATIONS)(
    _In_    NDIS_SWITCH_CONTEXT                       NdisSwitchContext,
    _Inout_ PNET_BUFFER_LIST                          NetBufferList,
    _In_    UINT32                                    NumberOfNewDestinations,
    _In_    PNDIS_SWITCH_FORWARDING_DESTINATION_ARRAY Destinations
    );

typedef NDIS_SWITCH_UPDATE_NET_BUFFER_LIST_DESTINATIONS
            (*NDIS_SWITCH_UPDATE_NET_BUFFER_LIST_DESTINATIONS_HANDLER);

#define NDIS_SWITCH_COPY_NBL_INFO_FLAGS_PRESERVE_DESTINATIONS   0x1

#if (NDIS_SUPPORT_NDIS640)
#define NDIS_SWITCH_COPY_NBL_INFO_FLAGS_PRESERVE_SWITCH_INFO_ONLY  0x2
#endif //(NDIS_SUPPORT_NDIS640)

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
NDIS_STATUS
(NDIS_SWITCH_COPY_NET_BUFFER_LIST_INFO)(
    _In_    NDIS_SWITCH_CONTEXT NdisSwitchContext,
    _Inout_ PNET_BUFFER_LIST    DestNetBufferList,
    _In_    PNET_BUFFER_LIST    SrcNetBufferList,
    _In_    UINT32              Flags
    );

typedef NDIS_SWITCH_COPY_NET_BUFFER_LIST_INFO
            (*NDIS_SWITCH_COPY_NET_BUFFER_LIST_INFO_HANDLER);


typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
NDIS_STATUS
(NDIS_SWITCH_REFERENCE_SWITCH_NIC)(
    _In_    NDIS_SWITCH_CONTEXT                       NdisSwitchContext,
    _In_    NDIS_SWITCH_PORT_ID                       SwitchPortId,
    _In_    NDIS_SWITCH_NIC_INDEX                     SwitchNicIndex
    );

typedef NDIS_SWITCH_REFERENCE_SWITCH_NIC
        (*NDIS_SWITCH_REFERENCE_SWITCH_NIC_HANDLER);


typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
NDIS_STATUS
(NDIS_SWITCH_DEREFERENCE_SWITCH_NIC)(
    _In_    NDIS_SWITCH_CONTEXT                       NdisSwitchContext,
    _In_    NDIS_SWITCH_PORT_ID                       SwitchPortId,
    _In_    NDIS_SWITCH_NIC_INDEX                     SwitchNicIndex
    );

typedef NDIS_SWITCH_DEREFERENCE_SWITCH_NIC
        (*NDIS_SWITCH_DEREFERENCE_SWITCH_NIC_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
NDIS_STATUS
(NDIS_SWITCH_REFERENCE_SWITCH_PORT)(
    _In_    NDIS_SWITCH_CONTEXT                       NdisSwitchContext,
    _In_    NDIS_SWITCH_PORT_ID                       SwitchPortId
    );

typedef NDIS_SWITCH_REFERENCE_SWITCH_PORT
        (*NDIS_SWITCH_REFERENCE_SWITCH_PORT_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
NDIS_STATUS
(NDIS_SWITCH_DEREFERENCE_SWITCH_PORT)(
    _In_    NDIS_SWITCH_CONTEXT                       NdisSwitchContext,
    _In_    NDIS_SWITCH_PORT_ID                       SwitchPortId
    );

typedef NDIS_SWITCH_DEREFERENCE_SWITCH_PORT
        (*NDIS_SWITCH_DEREFERENCE_SWITCH_PORT_HANDLER);

#define NDIS_SWITCH_REPORT_FILTERED_NBL_FLAGS_IS_INCOMING 0x1

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
VOID
(NDIS_SWITCH_REPORT_FILTERED_NET_BUFFER_LISTS)(
    _In_      NDIS_SWITCH_CONTEXT                   NdisSwitchContext,
    _In_      PUNICODE_STRING                       ExtensionGuid,
    _In_      PUNICODE_STRING                       ExtensionFriendlyName,
    _In_      NDIS_SWITCH_PORT_ID                   PortId,
    _In_      UINT32                                Flags,
    _In_      UINT32                                NumberOfNetBufferLists,
    _In_      PNET_BUFFER_LIST                      NetBufferLists,
    _In_opt_  PUNICODE_STRING                       FilterReason
    );

typedef NDIS_SWITCH_REPORT_FILTERED_NET_BUFFER_LISTS (*NDIS_SWITCH_REPORT_FILTERED_NET_BUFFER_LISTS_HANDLER);

#define NDIS_SWITCH_NET_BUFFER_LIST_CONTEXT_TYPE_INFO_REVISION_1      1

typedef struct _NDIS_SWITCH_NET_BUFFER_LIST_CONTEXT_TYPE_INFO
{
    NDIS_OBJECT_HEADER        Header;
    PCHAR                     ContextName;
    GUID*                     ExtensionId;
} NDIS_SWITCH_NET_BUFFER_LIST_CONTEXT_TYPE_INFO, *PNDIS_SWITCH_NET_BUFFER_LIST_CONTEXT_TYPE_INFO;

#define NDIS_SIZEOF_SWITCH_NET_BUFFER_LIST_CONTEXT_TYPE_INFO_REVISION_1 \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_SWITCH_NET_BUFFER_LIST_CONTEXT_TYPE_INFO, ExtensionId)

#define NDIS_DECLARE_SWITCH_NET_BUFFER_LIST_CONTEXT_TYPE(_ContextName, _ExtensionId)\
                                                                           \
extern NDIS_SWITCH_NET_BUFFER_LIST_CONTEXT_TYPE_INFO _ContextName =        \
{                                                                          \
    {                                                                      \
        NDIS_OBJECT_TYPE_DEFAULT,                                          \
        NDIS_SIZEOF_SWITCH_NET_BUFFER_LIST_CONTEXT_TYPE_INFO_REVISION_1,   \
        NDIS_SWITCH_NET_BUFFER_LIST_CONTEXT_TYPE_INFO_REVISION_1,          \
    },                                                                     \
    #_ContextName,                                                         \
    _ExtensionId                                                           \
};                                                                         \

//
// Use NDIS_SWITCH_NET_BUFFER_LIST_CONTEXT_TYPE when defining types
// and calling the APIs, rather than the specific underlying information
// structure used in implementation.
//
#define NDIS_SWITCH_NET_BUFFER_LIST_CONTEXT_TYPE \
            NDIS_SWITCH_NET_BUFFER_LIST_CONTEXT_TYPE_INFO

#define PNDIS_SWITCH_NET_BUFFER_LIST_CONTEXT_TYPE \
            PNDIS_SWITCH_NET_BUFFER_LIST_CONTEXT_TYPE_INFO

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
NDIS_STATUS
(NDIS_SWITCH_SET_NET_BUFFER_LIST_SWITCH_CONTEXT)(
    _In_      NDIS_SWITCH_CONTEXT                       NdisSwitchContext,
    _Inout_   PNET_BUFFER_LIST                          NetBufferList,
    _In_      PNDIS_SWITCH_NET_BUFFER_LIST_CONTEXT_TYPE ContextType,
    _In_      PVOID                                     Context
    );

typedef NDIS_SWITCH_SET_NET_BUFFER_LIST_SWITCH_CONTEXT (*NDIS_SWITCH_SET_NET_BUFFER_LIST_SWITCH_CONTEXT_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
PVOID
(NDIS_SWITCH_GET_NET_BUFFER_LIST_SWITCH_CONTEXT)(
    _In_      NDIS_SWITCH_CONTEXT                       NdisSwitchContext,
    _In_      PNET_BUFFER_LIST                          NetBufferList,
    _In_      PNDIS_SWITCH_NET_BUFFER_LIST_CONTEXT_TYPE ContextType
    );

typedef NDIS_SWITCH_GET_NET_BUFFER_LIST_SWITCH_CONTEXT (*NDIS_SWITCH_GET_NET_BUFFER_LIST_SWITCH_CONTEXT_HANDLER);

#define NDIS_SWITCH_OPTIONAL_HANDLERS_REVISION_1      1

typedef struct _NDIS_SWITCH_OPTIONAL_HANDLERS
{
    //
    // Header.Type = NDIS_OBJECT_TYPE_SWITCH_OPTIONAL_HANDLERS
    // Header.Revision = NDIS_OBJECT_TYPE_SWITCH_OPTIONAL_HANDLERS_REVISION_1
    // Header.Size = NDIS_SIZEOF_SWITCH_OPTIONAL_HANDLERS_REVISION_1
    //
    NDIS_OBJECT_HEADER                                              Header;
    NDIS_SWITCH_ALLOCATE_NET_BUFFER_LIST_FORWARDING_CONTEXT_HANDLER AllocateNetBufferListForwardingContext;
    NDIS_SWITCH_FREE_NET_BUFFER_LIST_FORWARDING_CONTEXT_HANDLER     FreeNetBufferListForwardingContext;
    NDIS_SWITCH_SET_NET_BUFFER_LIST_SOURCE_HANDLER                  SetNetBufferListSource;
    NDIS_SWITCH_ADD_NET_BUFFER_LIST_DESTINATION_HANDLER             AddNetBufferListDestination;
    NDIS_SWITCH_GROW_NET_BUFFER_LIST_DESTINATIONS_HANDLER           GrowNetBufferListDestinations;
    NDIS_SWITCH_GET_NET_BUFFER_LIST_DESTINATIONS_HANDLER            GetNetBufferListDestinations;
    NDIS_SWITCH_UPDATE_NET_BUFFER_LIST_DESTINATIONS_HANDLER         UpdateNetBufferListDestinations;
    NDIS_SWITCH_COPY_NET_BUFFER_LIST_INFO_HANDLER                   CopyNetBufferListInfo;
    NDIS_SWITCH_REFERENCE_SWITCH_NIC_HANDLER                        ReferenceSwitchNic;
    NDIS_SWITCH_DEREFERENCE_SWITCH_NIC_HANDLER                      DereferenceSwitchNic;
    NDIS_SWITCH_REFERENCE_SWITCH_PORT_HANDLER                       ReferenceSwitchPort;
    NDIS_SWITCH_DEREFERENCE_SWITCH_PORT_HANDLER                     DereferenceSwitchPort;
    NDIS_SWITCH_REPORT_FILTERED_NET_BUFFER_LISTS_HANDLER            ReportFilteredNetBufferLists;
    NDIS_SWITCH_SET_NET_BUFFER_LIST_SWITCH_CONTEXT_HANDLER          SetNetBufferListSwitchContext;
    NDIS_SWITCH_GET_NET_BUFFER_LIST_SWITCH_CONTEXT_HANDLER          GetNetBufferListSwitchContext;
} NDIS_SWITCH_OPTIONAL_HANDLERS, *PNDIS_SWITCH_OPTIONAL_HANDLERS;

#define NDIS_SIZEOF_SWITCH_OPTIONAL_HANDLERS_REVISION_1 \
    RTL_SIZEOF_THROUGH_FIELD(NDIS_SWITCH_OPTIONAL_HANDLERS, ReportFilteredNetBufferLists)

#if (NDIS_SUPPORT_NDIS640)

#endif // (NDIS_SUPPORT_NDIS640)

#endif // (NDIS_SUPPORT_NDIS630)

#endif // (NDIS_SUPPORT_NDIS61)

#endif // NDIS_SUPPORT_NDIS6

#if defined (_MSC_VER)
#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning(default:4001) /* nonstandard extension : single line comment */
#pragma warning(default:4201) /* nonstandard extension used : nameless struct/union */
#pragma warning(default:4214) /* nonstandard extension used : bit field types other then int */
#endif
#endif

//
//    Copyright (C) Microsoft.  All rights reserved.
//
#pragma once

typedef struct _CO_CALL_PARAMETERS      CO_CALL_PARAMETERS, *PCO_CALL_PARAMETERS;
typedef struct _CO_MEDIA_PARAMETERS     CO_MEDIA_PARAMETERS, *PCO_MEDIA_PARAMETERS;

//
// CoNdis client only handler proto-types - used by clients of call managers
//
typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CL_OPEN_AF_COMPLETE)
VOID
(PROTOCOL_CL_OPEN_AF_COMPLETE)(
    _In_ NDIS_STATUS             Status,
    _In_ NDIS_HANDLE             ProtocolAfContext,
    _In_ NDIS_HANDLE             NdisAfHandle
    );

typedef PROTOCOL_CL_OPEN_AF_COMPLETE (*CL_OPEN_AF_COMPLETE_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CL_CLOSE_AF_COMPLETE)
VOID
(PROTOCOL_CL_CLOSE_AF_COMPLETE)(
    _In_ NDIS_STATUS             Status,
    _In_ NDIS_HANDLE             ProtocolAfContext
    );

typedef PROTOCOL_CL_CLOSE_AF_COMPLETE (*CL_CLOSE_AF_COMPLETE_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CL_REGISTER_SAP_COMPLETE)
VOID
(PROTOCOL_CL_REGISTER_SAP_COMPLETE)(
    _In_ NDIS_STATUS             Status,
    _In_ NDIS_HANDLE             ProtocolSapContext,
    _In_ PCO_SAP                 Sap,
    _In_ NDIS_HANDLE             NdisSapHandle
    );

typedef PROTOCOL_CL_REGISTER_SAP_COMPLETE (*CL_REG_SAP_COMPLETE_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CL_DEREGISTER_SAP_COMPLETE)
VOID
(PROTOCOL_CL_DEREGISTER_SAP_COMPLETE)(
    _In_  NDIS_STATUS             Status,
    _In_  NDIS_HANDLE             ProtocolSapContext
    );

typedef PROTOCOL_CL_DEREGISTER_SAP_COMPLETE (*CL_DEREG_SAP_COMPLETE_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CL_MAKE_CALL_COMPLETE)
VOID
(PROTOCOL_CL_MAKE_CALL_COMPLETE)(
    _In_      NDIS_STATUS             Status,
    _In_      NDIS_HANDLE             ProtocolVcContext,
    _In_opt_  NDIS_HANDLE             NdisPartyHandle,
    _In_      PCO_CALL_PARAMETERS     CallParameters
    );

typedef PROTOCOL_CL_MAKE_CALL_COMPLETE (*CL_MAKE_CALL_COMPLETE_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CL_CLOSE_CALL_COMPLETE)
VOID
(PROTOCOL_CL_CLOSE_CALL_COMPLETE)(
    _In_      NDIS_STATUS             Status,
    _In_      NDIS_HANDLE             ProtocolVcContext,
    _In_opt_  NDIS_HANDLE             ProtocolPartyContext
    );

typedef PROTOCOL_CL_CLOSE_CALL_COMPLETE (*CL_CLOSE_CALL_COMPLETE_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CL_ADD_PARTY_COMPLETE)
VOID
(PROTOCOL_CL_ADD_PARTY_COMPLETE)(
    _In_  NDIS_STATUS             Status,
    _In_  NDIS_HANDLE             ProtocolPartyContext,
    _In_  NDIS_HANDLE             NdisPartyHandle,
    _In_  PCO_CALL_PARAMETERS     CallParameters
    );

typedef PROTOCOL_CL_ADD_PARTY_COMPLETE (*CL_ADD_PARTY_COMPLETE_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CL_DROP_PARTY_COMPLETE)
VOID
(PROTOCOL_CL_DROP_PARTY_COMPLETE)(
    _In_  NDIS_STATUS             Status,
    _In_  NDIS_HANDLE             ProtocolPartyContext
    );

typedef PROTOCOL_CL_DROP_PARTY_COMPLETE (*CL_DROP_PARTY_COMPLETE_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CL_INCOMING_CALL)
NDIS_STATUS
(PROTOCOL_CL_INCOMING_CALL)(
    _In_     NDIS_HANDLE             ProtocolSapContext,
    _In_     NDIS_HANDLE             ProtocolVcContext,
    _Inout_  PCO_CALL_PARAMETERS     CallParameters
    );

typedef PROTOCOL_CL_INCOMING_CALL (*CL_INCOMING_CALL_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CL_CALL_CONNECTED)
VOID
(PROTOCOL_CL_CALL_CONNECTED)(
    _In_  NDIS_HANDLE             ProtocolVcContext
    );

typedef PROTOCOL_CL_CALL_CONNECTED (*CL_CALL_CONNECTED_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CL_INCOMING_CLOSE_CALL)
VOID
(PROTOCOL_CL_INCOMING_CLOSE_CALL)(
    _In_      NDIS_STATUS             CloseStatus,
    _In_      NDIS_HANDLE             ProtocolVcContext,
    _In_opt_  PVOID                   CloseData,
    _In_opt_  UINT                    Size
    );

typedef PROTOCOL_CL_INCOMING_CLOSE_CALL (*CL_INCOMING_CLOSE_CALL_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CL_INCOMING_DROP_PARTY)
VOID
(PROTOCOL_CL_INCOMING_DROP_PARTY)(
    _In_      NDIS_STATUS             DropStatus,
    _In_      NDIS_HANDLE             ProtocolPartyContext,
    _In_opt_  PVOID                   CloseData,
    _In_opt_  UINT                    Size
    );

typedef PROTOCOL_CL_INCOMING_DROP_PARTY (*CL_INCOMING_DROP_PARTY_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CL_MODIFY_CALL_QOS_COMPLETE)
VOID
(PROTOCOL_CL_MODIFY_CALL_QOS_COMPLETE)(
    _In_  NDIS_STATUS             Status,
    _In_  NDIS_HANDLE             ProtocolVcContext,
    _In_  PCO_CALL_PARAMETERS     CallParameters
    );

typedef PROTOCOL_CL_MODIFY_CALL_QOS_COMPLETE (*CL_MODIFY_CALL_QOS_COMPLETE_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CL_INCOMING_CALL_QOS_CHANGE)
VOID
(PROTOCOL_CL_INCOMING_CALL_QOS_CHANGE)(
    _In_  NDIS_HANDLE             ProtocolVcContext,
    _In_  PCO_CALL_PARAMETERS     CallParameters
    );
typedef PROTOCOL_CL_INCOMING_CALL_QOS_CHANGE (*CL_INCOMING_CALL_QOS_CHANGE_HANDLER);

#if NDIS_LEGACY_PROTOCOL
typedef struct _NDIS_CLIENT_CHARACTERISTICS
{
    UCHAR                           MajorVersion;
    UCHAR                           MinorVersion;

    USHORT                          Filler;
    UINT                            Reserved;

    CO_CREATE_VC_HANDLER            ClCreateVcHandler;
    CO_DELETE_VC_HANDLER            ClDeleteVcHandler;
    CO_REQUEST_HANDLER              ClRequestHandler;
    CO_REQUEST_COMPLETE_HANDLER     ClRequestCompleteHandler;
    CL_OPEN_AF_COMPLETE_HANDLER     ClOpenAfCompleteHandler;
    CL_CLOSE_AF_COMPLETE_HANDLER    ClCloseAfCompleteHandler;
    CL_REG_SAP_COMPLETE_HANDLER     ClRegisterSapCompleteHandler;
    CL_DEREG_SAP_COMPLETE_HANDLER   ClDeregisterSapCompleteHandler;
    CL_MAKE_CALL_COMPLETE_HANDLER   ClMakeCallCompleteHandler;
    CL_MODIFY_CALL_QOS_COMPLETE_HANDLER ClModifyCallQoSCompleteHandler;
    CL_CLOSE_CALL_COMPLETE_HANDLER  ClCloseCallCompleteHandler;
    CL_ADD_PARTY_COMPLETE_HANDLER   ClAddPartyCompleteHandler;
    CL_DROP_PARTY_COMPLETE_HANDLER  ClDropPartyCompleteHandler;
    CL_INCOMING_CALL_HANDLER        ClIncomingCallHandler;
    CL_INCOMING_CALL_QOS_CHANGE_HANDLER ClIncomingCallQoSChangeHandler;
    CL_INCOMING_CLOSE_CALL_HANDLER  ClIncomingCloseCallHandler;
    CL_INCOMING_DROP_PARTY_HANDLER  ClIncomingDropPartyHandler;
    CL_CALL_CONNECTED_HANDLER       ClCallConnectedHandler;

} NDIS_CLIENT_CHARACTERISTICS, *PNDIS_CLIENT_CHARACTERISTICS;
#endif // NDIS_LEGACY_PROTOCOL

//
// CoNdis call-manager only handler proto-types - used by call managers only
//
typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CM_OPEN_AF)
NDIS_STATUS
(PROTOCOL_CM_OPEN_AF)(
    _In_  NDIS_HANDLE             CallMgrBindingContext,
    _In_  PCO_ADDRESS_FAMILY      AddressFamily,
    _In_  NDIS_HANDLE             NdisAfHandle,
    _Out_ PNDIS_HANDLE            CallMgrAfContext
    );

typedef PROTOCOL_CM_OPEN_AF (*CM_OPEN_AF_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CM_CLOSE_AF)
NDIS_STATUS
(PROTOCOL_CM_CLOSE_AF)(
    _In_  NDIS_HANDLE             CallMgrAfContext
    );

typedef PROTOCOL_CM_CLOSE_AF (*CM_CLOSE_AF_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CM_REG_SAP)
NDIS_STATUS
(PROTOCOL_CM_REG_SAP)(
    _In_  NDIS_HANDLE             CallMgrAfContext,
    _In_  PCO_SAP                 Sap,
    _In_  NDIS_HANDLE             NdisSapHandle,
    _Out_ PNDIS_HANDLE            CallMgrSapContext
    );

typedef PROTOCOL_CM_REG_SAP (*CM_REG_SAP_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CM_DEREGISTER_SAP)
NDIS_STATUS
(PROTOCOL_CM_DEREGISTER_SAP)(
    _In_  NDIS_HANDLE             CallMgrSapContext
    );

typedef PROTOCOL_CM_DEREGISTER_SAP (*CM_DEREG_SAP_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CM_MAKE_CALL)
NDIS_STATUS
(PROTOCOL_CM_MAKE_CALL)(
    _In_      NDIS_HANDLE             CallMgrVcContext,
    _Inout_   PCO_CALL_PARAMETERS     CallParameters,
    _In_opt_  NDIS_HANDLE             NdisPartyHandle,
    _Out_opt_ PNDIS_HANDLE            CallMgrPartyContext
    );

typedef PROTOCOL_CM_MAKE_CALL (*CM_MAKE_CALL_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CM_CLOSE_CALL)
NDIS_STATUS
(PROTOCOL_CM_CLOSE_CALL)(
    _In_      NDIS_HANDLE             CallMgrVcContext,
    _In_opt_  NDIS_HANDLE             CallMgrPartyContext,
    _In_opt_  PVOID                   CloseData,
    _In_opt_  UINT                    Size
    );

typedef PROTOCOL_CM_CLOSE_CALL (*CM_CLOSE_CALL_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CM_MODIFY_QOS_CALL)
NDIS_STATUS
(PROTOCOL_CM_MODIFY_QOS_CALL)(
    _In_  NDIS_HANDLE             CallMgrVcContext,
    _In_  PCO_CALL_PARAMETERS     CallParameters
    );

typedef PROTOCOL_CM_MODIFY_QOS_CALL (*CM_MODIFY_CALL_QOS_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CM_INCOMING_CALL_COMPLETE)
VOID
(PROTOCOL_CM_INCOMING_CALL_COMPLETE)(
    _In_  NDIS_STATUS             Status,
    _In_  NDIS_HANDLE             CallMgrVcContext,
    _In_  PCO_CALL_PARAMETERS     CallParameters
    );

typedef PROTOCOL_CM_INCOMING_CALL_COMPLETE (*CM_INCOMING_CALL_COMPLETE_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CM_ACTIVATE_VC_COMPLETE)
VOID
(PROTOCOL_CM_ACTIVATE_VC_COMPLETE)(
    _In_  NDIS_STATUS             Status,
    _In_  NDIS_HANDLE             CallMgrVcContext,
    _In_  PCO_CALL_PARAMETERS     CallParameters
    );

typedef PROTOCOL_CM_ACTIVATE_VC_COMPLETE (*CM_ACTIVATE_VC_COMPLETE_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CM_DEACTIVATE_VC_COMPLETE)
VOID
(PROTOCOL_CM_DEACTIVATE_VC_COMPLETE)(
    _In_  NDIS_STATUS             Status,
    _In_  NDIS_HANDLE             CallMgrVcContext
    );

typedef PROTOCOL_CM_DEACTIVATE_VC_COMPLETE (*CM_DEACTIVATE_VC_COMPLETE_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CM_ADD_PARTY)
NDIS_STATUS
(PROTOCOL_CM_ADD_PARTY)(
    _In_     NDIS_HANDLE             CallMgrVcContext,
    _Inout_  PCO_CALL_PARAMETERS     CallParameters,
    _In_     NDIS_HANDLE             NdisPartyHandle,
    _Out_    PNDIS_HANDLE            CallMgrPartyContext
    );

typedef PROTOCOL_CM_ADD_PARTY (*CM_ADD_PARTY_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(PROTOCOL_CM_DROP_PARTY)
NDIS_STATUS
(PROTOCOL_CM_DROP_PARTY)(
    _In_      NDIS_HANDLE             CallMgrPartyContext,
    _In_opt_  PVOID                   CloseData,
    _In_opt_  UINT                    Size
    );

typedef PROTOCOL_CM_DROP_PARTY (*CM_DROP_PARTY_HANDLER);

#if NDIS_LEGACY_DRIVER

typedef struct _NDIS_CALL_MANAGER_CHARACTERISTICS
{
    UCHAR                           MajorVersion;
    UCHAR                           MinorVersion;
    USHORT                          Filler;
    UINT                            Reserved;

    CO_CREATE_VC_HANDLER            CmCreateVcHandler;
    CO_DELETE_VC_HANDLER            CmDeleteVcHandler;
    CM_OPEN_AF_HANDLER              CmOpenAfHandler;
    CM_CLOSE_AF_HANDLER             CmCloseAfHandler;
    CM_REG_SAP_HANDLER              CmRegisterSapHandler;
    CM_DEREG_SAP_HANDLER            CmDeregisterSapHandler;
    CM_MAKE_CALL_HANDLER            CmMakeCallHandler;
    CM_CLOSE_CALL_HANDLER           CmCloseCallHandler;
    CM_INCOMING_CALL_COMPLETE_HANDLER CmIncomingCallCompleteHandler;
    CM_ADD_PARTY_HANDLER            CmAddPartyHandler;
    CM_DROP_PARTY_HANDLER           CmDropPartyHandler;
    CM_ACTIVATE_VC_COMPLETE_HANDLER CmActivateVcCompleteHandler;
    CM_DEACTIVATE_VC_COMPLETE_HANDLER CmDeactivateVcCompleteHandler;
    CM_MODIFY_CALL_QOS_HANDLER      CmModifyCallQoSHandler;
    CO_REQUEST_HANDLER              CmRequestHandler;
    CO_REQUEST_COMPLETE_HANDLER     CmRequestCompleteHandler;

} NDIS_CALL_MANAGER_CHARACTERISTICS, *PNDIS_CALL_MANAGER_CHARACTERISTICS;

#endif // NDIS_LEGACY_DRIVER

//
// this send flag is used on ATM net cards to set ( turn on ) the CLP bit
// (Cell Loss Priority) bit
//
#define CO_SEND_FLAG_SET_DISCARD_ELIBILITY  0x00000001

//
// the Address structure used on NDIS_CO_ADD_ADDRESS or NDIS_CO_DELETE_ADDRESS
//
typedef struct _CO_ADDRESS
{
    ULONG                       AddressSize;
    UCHAR                       Address[1];
} CO_ADDRESS, *PCO_ADDRESS;

//
// the list of addresses returned from the CallMgr on a NDIS_CO_GET_ADDRESSES
//
typedef struct _CO_ADDRESS_LIST
{
    ULONG                       NumberOfAddressesAvailable;
    ULONG                       NumberOfAddresses;
    CO_ADDRESS                  AddressList;
} CO_ADDRESS_LIST, *PCO_ADDRESS_LIST;

#ifndef FAR
#define FAR
#endif
#include <qos.h>

typedef struct _CO_SPECIFIC_PARAMETERS
{
    ULONG                       ParamType;
    ULONG                       Length;
    UCHAR                       Parameters[1];
} CO_SPECIFIC_PARAMETERS, *PCO_SPECIFIC_PARAMETERS;

typedef struct _CO_CALL_MANAGER_PARAMETERS
{
    FLOWSPEC                    Transmit;
    FLOWSPEC                    Receive;
    CO_SPECIFIC_PARAMETERS      CallMgrSpecific;
} CO_CALL_MANAGER_PARAMETERS, *PCO_CALL_MANAGER_PARAMETERS;


//
// this is the generic portion of the media parameters, including the media
// specific component too.
//
typedef struct _CO_MEDIA_PARAMETERS
{
    ULONG                       Flags;
    ULONG                       ReceivePriority;
    ULONG                       ReceiveSizeHint;
    CO_SPECIFIC_PARAMETERS POINTER_ALIGNMENT      MediaSpecific;
} CO_MEDIA_PARAMETERS, *PCO_MEDIA_PARAMETERS;


//
// definitions for the flags in CO_MEDIA_PARAMETERS
//
#define RECEIVE_TIME_INDICATION 0x00000001
#define USE_TIME_STAMPS         0x00000002
#define TRANSMIT_VC             0x00000004
#define RECEIVE_VC              0x00000008
#define INDICATE_ERRED_PACKETS  0x00000010
#define INDICATE_END_OF_TX      0x00000020
#define RESERVE_RESOURCES_VC    0x00000040
#define ROUND_DOWN_FLOW         0x00000080
#define ROUND_UP_FLOW           0x00000100
//
// define a flag to set in the flags of an Ndis packet when the miniport
// indicates a receive with an error in it
//
#define ERRED_PACKET_INDICATION 0x00000001

//
// this is the structure passed during call-setup
//
typedef struct _CO_CALL_PARAMETERS
{
    ULONG                       Flags;
    PCO_CALL_MANAGER_PARAMETERS CallMgrParameters;
    PCO_MEDIA_PARAMETERS        MediaParameters;
} CO_CALL_PARAMETERS, *PCO_CALL_PARAMETERS;

//
// Definitions for the Flags in CO_CALL_PARAMETERS
//
#define PERMANENT_VC            0x00000001
#define CALL_PARAMETERS_CHANGED 0x00000002
#define QUERY_CALL_PARAMETERS   0x00000004
#define BROADCAST_VC            0x00000008
#define MULTIPOINT_VC           0x00000010

//
// The format of the Request for adding/deleting a PVC
//
typedef struct _CO_PVC
{
    NDIS_HANDLE                 NdisAfHandle;
    CO_SPECIFIC_PARAMETERS      PvcParameters;
} CO_PVC,*PCO_PVC;



//
// NDIS 5.0 Extensions for protocols
//
_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
_IRQL_requires_min_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisCoAssignInstanceName(
    _In_         NDIS_HANDLE             NdisVcHandle,
    _In_         PNDIS_STRING            BaseInstanceName,
    _Out_opt_    PNDIS_STRING            VcInstanceName
    );

#if NDIS_LEGACY_PROTOCOL
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCoSendPackets(
    _In_  NDIS_HANDLE             NdisVcHandle,
    _In_  PPNDIS_PACKET           PacketArray,
    _In_  UINT                    NumberOfPackets
    );

#endif // NDIS_LEGACY_PROTOCOL

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisCoCreateVc(
    _In_      NDIS_HANDLE             NdisBindingHandle,
    _In_opt_  NDIS_HANDLE             NdisAfHandle,   // For CM signaling VCs
    _In_      NDIS_HANDLE             ProtocolVcContext,
    _Inout_   PNDIS_HANDLE            NdisVcHandle
    );

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisCoDeleteVc(
    _In_  NDIS_HANDLE             NdisVcHandle
    );

#if NDIS_LEGACY_PROTOCOL
_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisCoRequest(
    _In_      NDIS_HANDLE             NdisBindingHandle,
    _In_opt_  NDIS_HANDLE             NdisAfHandle,
    _In_opt_  NDIS_HANDLE             NdisVcHandle,
    _In_opt_  NDIS_HANDLE             NdisPartyHandle,
    _Inout_   PNDIS_REQUEST           NdisRequest
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCoRequestComplete(
    _In_      NDIS_STATUS             Status,
    _In_      NDIS_HANDLE             NdisAfHandle,
    _In_opt_  NDIS_HANDLE             NdisVcHandle,
    _In_opt_  NDIS_HANDLE             NdisPartyHandle,
    _In_      PNDIS_REQUEST           NdisRequest
    );

#endif // NDIS_LEGACY_PROTOCOL

#ifndef __NDISTAPI_VAR_STRING_DECLARED
#define __NDISTAPI_VAR_STRING_DECLARED

typedef struct _VAR_STRING
{
    ULONG   ulTotalSize;
    ULONG   ulNeededSize;
    ULONG   ulUsedSize;

    ULONG   ulStringFormat;
    ULONG   ulStringSize;
    ULONG   ulStringOffset;

} VAR_STRING, *PVAR_STRING;

#endif // __NDISTAPI_VAR_STRING_DECLARED


#ifndef __NDISTAPI_STRINGFORMATS_DEFINED
#define __NDISTAPI_STRINGFORMATS_DEFINED

#define STRINGFORMAT_ASCII                          0x00000001
#define STRINGFORMAT_DBCS                           0x00000002
#define STRINGFORMAT_UNICODE                        0x00000003
#define STRINGFORMAT_BINARY                         0x00000004

#endif // __NDISTAPI_STRINGFORMATS_DEFINED

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
_IRQL_requires_min_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisCoGetTapiCallId(
    _In_    NDIS_HANDLE         NdisVcHandle,
    _Inout_ PVAR_STRING         TapiCallId
    );

#if NDIS_LEGACY_PROTOCOL
//
// Client Apis
//
_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisClOpenAddressFamily(
    _In_    NDIS_HANDLE                        NdisBindingHandle,
    _In_    PCO_ADDRESS_FAMILY                 AddressFamily,
    _In_    NDIS_HANDLE                        ProtocolAfContext,
    _In_    PNDIS_CLIENT_CHARACTERISTICS       ClCharacteristics,
    _In_    UINT                               SizeOfClCharacteristics,
    _Out_   PNDIS_HANDLE                       NdisAfHandle
    );

#endif // NDIS_LEGACY_PROTOCOL

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisClCloseAddressFamily(
    _In_  NDIS_HANDLE             NdisAfHandle
    );

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisClRegisterSap(
    _In_  NDIS_HANDLE             NdisAfHandle,
    _In_  NDIS_HANDLE             ProtocolSapContext,
    _In_  PCO_SAP                 Sap,
    _Out_ PNDIS_HANDLE            NdisSapHandle
    );

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisClDeregisterSap(
    _In_  NDIS_HANDLE             NdisSapHandle
    );

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisClMakeCall(
    _In_        NDIS_HANDLE             NdisVcHandle,
    _Inout_     PCO_CALL_PARAMETERS     CallParameters,
    _In_opt_    NDIS_HANDLE             ProtocolPartyContext,
    _Out_opt_   PNDIS_HANDLE            NdisPartyHandle
    );

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisClCloseCall(
    _In_                    NDIS_HANDLE             NdisVcHandle,
    _In_opt_                NDIS_HANDLE             NdisPartyHandle,
    _In_reads_bytes_opt_(Size)   PVOID                   Buffer,
    _In_                    UINT                    Size
    );

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisClModifyCallQoS(
    _In_    NDIS_HANDLE             NdisVcHandle,
    _In_    PCO_CALL_PARAMETERS     CallParameters
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisClIncomingCallComplete(
    _In_    NDIS_STATUS             Status,
    _In_    NDIS_HANDLE             NdisVcHandle,
    _In_    PCO_CALL_PARAMETERS     CallParameters
    );

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisClAddParty(
    _In_      NDIS_HANDLE             NdisVcHandle,
    _In_      NDIS_HANDLE             ProtocolPartyContext,
    _Inout_   PCO_CALL_PARAMETERS     CallParameters,
    _Out_     PNDIS_HANDLE            NdisPartyHandle
    );

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisClDropParty(
    _In_                    NDIS_HANDLE             NdisPartyHandle,
    _In_reads_bytes_opt_(Size)   PVOID                   Buffer,
    _In_opt_                UINT                    Size
    );

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
_IRQL_requires_min_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisClGetProtocolVcContextFromTapiCallId(
    _In_     UNICODE_STRING          TapiCallId,
    _Out_    PNDIS_HANDLE            ProtocolVcContext
    );

//
// Call Manager Apis
//

#if NDIS_LEGACY_DRIVER
_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisCmRegisterAddressFamily(
    _In_    NDIS_HANDLE                          NdisBindingHandle,
    _In_    PCO_ADDRESS_FAMILY                   AddressFamily,
    _In_    PNDIS_CALL_MANAGER_CHARACTERISTICS   CmCharacteristics,
    _In_    UINT                                 SizeOfCmCharacteristics
    );

#endif

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCmOpenAddressFamilyComplete(
    _In_    NDIS_STATUS             Status,
    _In_    NDIS_HANDLE             NdisAfHandle,
    _In_    NDIS_HANDLE             CallMgrAfContext
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCmCloseAddressFamilyComplete(
    _In_    NDIS_STATUS             Status,
    _In_    NDIS_HANDLE             NdisAfHandle
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCmRegisterSapComplete(
    _In_    NDIS_STATUS             Status,
    _In_    NDIS_HANDLE             NdisSapHandle,
    _In_    NDIS_HANDLE             CallMgrSapContext
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCmDeregisterSapComplete(
    _In_    NDIS_STATUS             Status,
    _In_    NDIS_HANDLE             NdisSapHandle
    );

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisCmActivateVc(
    _In_       NDIS_HANDLE             NdisVcHandle,
    _Inout_    PCO_CALL_PARAMETERS     CallParameters
    );

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisCmDeactivateVc(
    _In_    NDIS_HANDLE             NdisVcHandle
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCmMakeCallComplete(
    _In_        NDIS_STATUS             Status,
    _In_        NDIS_HANDLE             NdisVcHandle,
    _In_opt_    NDIS_HANDLE             NdisPartyHandle,
    _In_opt_    NDIS_HANDLE             CallMgrPartyContext,
    _In_        PCO_CALL_PARAMETERS     CallParameters
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCmCloseCallComplete(
    _In_       NDIS_STATUS             Status,
    _In_       NDIS_HANDLE             NdisVcHandle,
    _In_opt_   NDIS_HANDLE             NdisPartyHandle
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCmAddPartyComplete(
    _In_        NDIS_STATUS             Status,
    _In_        NDIS_HANDLE             NdisPartyHandle,
    _In_opt_    NDIS_HANDLE             CallMgrPartyContext,
    _In_        PCO_CALL_PARAMETERS     CallParameters
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCmDropPartyComplete(
    _In_  NDIS_STATUS             Status,
    _In_  NDIS_HANDLE             NdisPartyHandle
    );

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisCmDispatchIncomingCall(
    _In_    NDIS_HANDLE             NdisSapHandle,
    _In_    NDIS_HANDLE             NdisVcHandle,
    _In_    PCO_CALL_PARAMETERS     CallParameters
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCmDispatchCallConnected(
    _In_    NDIS_HANDLE             NdisVcHandle
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCmModifyCallQoSComplete(
    _In_    NDIS_STATUS             Status,
    _In_    NDIS_HANDLE             NdisVcHandle,
    _In_    PCO_CALL_PARAMETERS     CallParameters
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCmDispatchIncomingCallQoSChange(
    _In_    NDIS_HANDLE             NdisVcHandle,
    _In_    PCO_CALL_PARAMETERS     CallParameters
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCmDispatchIncomingCloseCall(
    _In_                    NDIS_STATUS             CloseStatus,
    _In_                    NDIS_HANDLE             NdisVcHandle,
    _In_reads_bytes_opt_(Size)   PVOID                   Buffer,
    _In_                    UINT                    Size
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCmDispatchIncomingDropParty(
    _In_                    NDIS_STATUS             DropStatus,
    _In_                    NDIS_HANDLE             NdisPartyHandle,
    _In_reads_bytes_opt_(Size)   PVOID                   Buffer,
    _In_                    UINT                    Size
    );

#if (NDIS_SUPPORT_NDIS6)

//
// CONDIS 6.0 extensions
//
#define NDIS_CO_CLIENT_OPTIONAL_HANDLERS_REVISION_1     1

typedef struct _NDIS_CO_CLIENT_OPTIONAL_HANDLERS
{
    NDIS_OBJECT_HEADER                      Header;
    ULONG                                   Reserved;

    CO_CREATE_VC_HANDLER                ClCreateVcHandler;
    CO_DELETE_VC_HANDLER                ClDeleteVcHandler;
    CO_OID_REQUEST_HANDLER              ClOidRequestHandler;
    CO_OID_REQUEST_COMPLETE_HANDLER     ClOidRequestCompleteHandler;
    CL_OPEN_AF_COMPLETE_HANDLER_EX      ClOpenAfCompleteHandlerEx;
    CL_CLOSE_AF_COMPLETE_HANDLER        ClCloseAfCompleteHandler;
    CL_REG_SAP_COMPLETE_HANDLER         ClRegisterSapCompleteHandler;
    CL_DEREG_SAP_COMPLETE_HANDLER       ClDeregisterSapCompleteHandler;
    CL_MAKE_CALL_COMPLETE_HANDLER       ClMakeCallCompleteHandler;
    CL_MODIFY_CALL_QOS_COMPLETE_HANDLER ClModifyCallQoSCompleteHandler;
    CL_CLOSE_CALL_COMPLETE_HANDLER      ClCloseCallCompleteHandler;
    CL_ADD_PARTY_COMPLETE_HANDLER       ClAddPartyCompleteHandler;
    CL_DROP_PARTY_COMPLETE_HANDLER      ClDropPartyCompleteHandler;
    CL_INCOMING_CALL_HANDLER            ClIncomingCallHandler;
    CL_INCOMING_CALL_QOS_CHANGE_HANDLER ClIncomingCallQoSChangeHandler;
    CL_INCOMING_CLOSE_CALL_HANDLER      ClIncomingCloseCallHandler;
    CL_INCOMING_DROP_PARTY_HANDLER      ClIncomingDropPartyHandler;
    CL_CALL_CONNECTED_HANDLER           ClCallConnectedHandler;
    CL_NOTIFY_CLOSE_AF_HANDLER          ClNotifyCloseAfHandler;
}NDIS_CO_CLIENT_OPTIONAL_HANDLERS, *PNDIS_CO_CLIENT_OPTIONAL_HANDLERS;

#define NDIS_SIZEOF_CO_CLIENT_OPTIONAL_HANDLERS_REVISION_1  \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_CO_CLIENT_OPTIONAL_HANDLERS, ClNotifyCloseAfHandler)

#define  NDIS_CO_CALL_MANAGER_OPTIONAL_HANDLERS_REVISION_1     1

typedef struct _NDIS_CO_CALL_MANAGER_OPTIONAL_HANDLERS
{
    NDIS_OBJECT_HEADER              Header;
    ULONG                           Reserved;
    CO_CREATE_VC_HANDLER            CmCreateVcHandler;
    CO_DELETE_VC_HANDLER            CmDeleteVcHandler;
    CM_OPEN_AF_HANDLER              CmOpenAfHandler;
    CM_CLOSE_AF_HANDLER             CmCloseAfHandler;
    CM_REG_SAP_HANDLER              CmRegisterSapHandler;
    CM_DEREG_SAP_HANDLER            CmDeregisterSapHandler;
    CM_MAKE_CALL_HANDLER            CmMakeCallHandler;
    CM_CLOSE_CALL_HANDLER           CmCloseCallHandler;
    CM_INCOMING_CALL_COMPLETE_HANDLER CmIncomingCallCompleteHandler;
    CM_ADD_PARTY_HANDLER            CmAddPartyHandler;
    CM_DROP_PARTY_HANDLER           CmDropPartyHandler;
    CM_ACTIVATE_VC_COMPLETE_HANDLER CmActivateVcCompleteHandler;
    CM_DEACTIVATE_VC_COMPLETE_HANDLER CmDeactivateVcCompleteHandler;
    CM_MODIFY_CALL_QOS_HANDLER      CmModifyCallQoSHandler;
    CO_OID_REQUEST_HANDLER          CmOidRequestHandler;
    CO_OID_REQUEST_COMPLETE_HANDLER CmOidRequestCompleteHandler;
    CM_NOTIFY_CLOSE_AF_COMPLETE_HANDLER           CmNotifyCloseAfCompleteHandler;

} NDIS_CO_CALL_MANAGER_OPTIONAL_HANDLERS, *PNDIS_CO_CALL_MANAGER_OPTIONAL_HANDLERS;

#define NDIS_SIZEOF_CO_CALL_MANAGER_OPTIONAL_HANDLERS_REVISION_1  \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_CO_CALL_MANAGER_OPTIONAL_HANDLERS, CmOidRequestCompleteHandler)

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCoSendNetBufferLists(
    _In_    NDIS_HANDLE             NdisVcHandle,
    _In_    PNET_BUFFER_LIST        NetBufferLists,
    _In_    ULONG                   SendFlags
    );

_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisCoOidRequest(
    _In_        NDIS_HANDLE             NdisBindingHandle,
    _In_opt_    NDIS_HANDLE             NdisAfHandle,
    _In_opt_    NDIS_HANDLE             NdisVcHandle,
    _In_opt_    NDIS_HANDLE             NdisPartyHandle,
    _Inout_     PNDIS_OID_REQUEST       OidRequest
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisCoOidRequestComplete(
    _In_        NDIS_HANDLE             NdisAfHandle,
    _In_opt_    NDIS_HANDLE             NdisVcHandle,
    _In_opt_    NDIS_HANDLE             NdisPartyHandle,
    _In_        PNDIS_OID_REQUEST       OidRequest,
    _In_        NDIS_STATUS             Status
    );

_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisClOpenAddressFamilyEx (
    _In_  NDIS_HANDLE             NdisBindingHandle,
    _In_  PCO_ADDRESS_FAMILY      AddressFamily,
    _In_  NDIS_HANDLE             ClientAfContext,
    _Out_ PNDIS_HANDLE            NdisAfHandle
    );

_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisCmRegisterAddressFamilyEx(
    _In_  NDIS_HANDLE             NdisBindingHandle,
    _In_  PCO_ADDRESS_FAMILY      AddressFamily
    );

_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisMCmRegisterAddressFamilyEx(
    _In_  NDIS_HANDLE             MiniportAdapterHandle,
    _In_  PCO_ADDRESS_FAMILY      AddressFamily
    );

_Must_inspect_result_
_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisCmNotifyCloseAddressFamily (
    _In_  NDIS_HANDLE             NdisAfHandle
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisClNotifyCloseAddressFamilyComplete (
    _In_  NDIS_HANDLE              NdisAfHandle,
    _In_  NDIS_STATUS              Status
    );


#endif // NDIS_SUPPORT_NDIS6


//
//    Copyright (C) Microsoft.  All rights reserved.
//
#pragma once

#if NDIS_SUPPORT_NDIS6
//
// Ndis Light Weight filters
//


//
// init / de-init
//

//
// Flags used in NDIS_FILTER_ATTACH_PARAMETERS
//

#define NDIS_FILTER_ATTACH_FLAGS_IGNORE_MANDATORY          0x00000001   // If a mandatory filter fails to attach, it can set the flag
                                                                        // to ask NDIS to ignore it is a mandatory filter


#define NDIS_FILTER_ATTACH_PARAMETERS_REVISION_1     1

#if (NDIS_SUPPORT_NDIS61)
#define NDIS_FILTER_ATTACH_PARAMETERS_REVISION_2     2
#endif // (NDIS_SUPPORT_NDIS61)

#if (NDIS_SUPPORT_NDIS620)
#define NDIS_FILTER_ATTACH_PARAMETERS_REVISION_3     3
#endif // (NDIS_SUPPORT_NDIS620)

#if (NDIS_SUPPORT_NDIS630)
#define NDIS_FILTER_ATTACH_PARAMETERS_REVISION_4     4
#endif // (NDIS_SUPPORT_NDIS630)

typedef struct _NDIS_FILTER_ATTACH_PARAMETERS
{
    _In_    NDIS_OBJECT_HEADER          Header;
    _In_    NET_IFINDEX                 IfIndex;
    _In_    NET_LUID                    NetLuid;
    _In_    PNDIS_STRING                FilterModuleGuidName;
    _In_    NET_IFINDEX                 BaseMiniportIfIndex;
    _In_    PNDIS_STRING                BaseMiniportInstanceName;
    _In_    PNDIS_STRING                BaseMiniportName;
    _In_    NDIS_MEDIA_CONNECT_STATE    MediaConnectState;
    _In_    NET_IF_MEDIA_DUPLEX_STATE   MediaDuplexState;
    _In_    ULONG64                     XmitLinkSpeed;
    _In_    ULONG64                     RcvLinkSpeed;
    _Inout_ NDIS_MEDIUM                 MiniportMediaType;
    _Inout_ NDIS_PHYSICAL_MEDIUM        MiniportPhysicalMediaType;
    _In_    NDIS_HANDLE                 MiniportMediaSpecificAttributes;
    _In_    PNDIS_OFFLOAD               DefaultOffloadConfiguration;
    _In_    USHORT                      MacAddressLength;
    _In_    UCHAR                       CurrentMacAddress[NDIS_MAX_PHYS_ADDRESS_LENGTH];
    _In_    NET_LUID                    BaseMiniportNetLuid;
    _In_    NET_IFINDEX                 LowerIfIndex;
    _In_    NET_LUID                    LowerIfNetLuid;
    _Inout_ ULONG                       Flags;
#if (NDIS_SUPPORT_NDIS61)
    _In_ PNDIS_HD_SPLIT_CURRENT_CONFIG  HDSplitCurrentConfig;
#endif // (NDIS_SUPPORT_NDIS61)

#if (NDIS_SUPPORT_NDIS620)
    _In_ PNDIS_RECEIVE_FILTER_CAPABILITIES ReceiveFilterCapabilities;
    _In_ PDEVICE_OBJECT                 MiniportPhysicalDeviceObject;
    _In_ PNDIS_NIC_SWITCH_CAPABILITIES  NicSwitchCapabilities;
#endif

#if (NDIS_SUPPORT_NDIS630)
    _In_ BOOLEAN                        BaseMiniportIfConnectorPresent;
    _In_ PNDIS_SRIOV_CAPABILITIES       SriovCapabilities;
    _In_ PNDIS_NIC_SWITCH_INFO_ARRAY    NicSwitchArray;
#endif // (NDIS_SUPPORT_NDIS630)
} NDIS_FILTER_ATTACH_PARAMETERS, *PNDIS_FILTER_ATTACH_PARAMETERS;

#define NDIS_SIZEOF_FILTER_ATTACH_PARAMETERS_REVISION_1  \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_FILTER_ATTACH_PARAMETERS, Flags)

#if (NDIS_SUPPORT_NDIS61)
#define NDIS_SIZEOF_FILTER_ATTACH_PARAMETERS_REVISION_2  \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_FILTER_ATTACH_PARAMETERS, HDSplitCurrentConfig)
#endif // (NDIS_SUPPORT_NDIS61)

#if (NDIS_SUPPORT_NDIS620)
#define NDIS_SIZEOF_FILTER_ATTACH_PARAMETERS_REVISION_3  \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_FILTER_ATTACH_PARAMETERS, NicSwitchCapabilities)
#endif // (NDIS_SUPPORT_NDIS620)

#if (NDIS_SUPPORT_NDIS630)
#define NDIS_SIZEOF_FILTER_ATTACH_PARAMETERS_REVISION_4  \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_FILTER_ATTACH_PARAMETERS, NicSwitchArray)
#endif // (NDIS_SUPPORT_NDIS630)

typedef
_IRQL_requires_max_(PASSIVE_LEVEL)
_Function_class_(FILTER_ATTACH)
NDIS_STATUS
(FILTER_ATTACH)(
    _In_  NDIS_HANDLE                     NdisFilterHandle,
    _In_  NDIS_HANDLE                     FilterDriverContext,
    _In_  PNDIS_FILTER_ATTACH_PARAMETERS  AttachParameters
    );

typedef FILTER_ATTACH (*FILTER_ATTACH_HANDLER);

typedef
_IRQL_requires_max_(PASSIVE_LEVEL)
_Function_class_(FILTER_DETACH)
VOID
(FILTER_DETACH)(
    _In_ __drv_freesMem(Mem) NDIS_HANDLE             FilterModuleContext
    );

typedef FILTER_DETACH (*FILTER_DETACH_HANDLER);

typedef
_IRQL_requires_max_(PASSIVE_LEVEL)
_Function_class_(FILTER_SET_MODULE_OPTIONS)
NDIS_STATUS
(FILTER_SET_MODULE_OPTIONS)(
    _In_  NDIS_HANDLE             FilterModuleContext
    );

typedef FILTER_SET_MODULE_OPTIONS (*FILTER_SET_FILTER_MODULE_OPTIONS_HANDLER);


//
// NDIS_FILTER_RESTART_PARAMETERS is used in FILTER_RESTART handler
//
#define NDIS_FILTER_RESTART_PARAMETERS_REVISION_1     1

typedef struct _NDIS_FILTER_RESTART_PARAMETERS
{
    NDIS_OBJECT_HEADER          Header;
    NDIS_MEDIUM                 MiniportMediaType;
    NDIS_PHYSICAL_MEDIUM        MiniportPhysicalMediaType;
    PNDIS_RESTART_ATTRIBUTES    RestartAttributes;
    NET_IFINDEX                 LowerIfIndex;
    NET_LUID                    LowerIfNetLuid;
    ULONG                       Flags;
} NDIS_FILTER_RESTART_PARAMETERS, *PNDIS_FILTER_RESTART_PARAMETERS;

#define NDIS_SIZEOF__FILTER_RESTART_PARAMETERS_REVISION_1  \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_FILTER_RESTART_PARAMETERS, Flags)

typedef
_IRQL_requires_max_(PASSIVE_LEVEL)
_Function_class_(FILTER_RESTART)
NDIS_STATUS
(FILTER_RESTART)(
    _In_  NDIS_HANDLE                     FilterModuleContext,
    _In_  PNDIS_FILTER_RESTART_PARAMETERS RestartParameters
    );
typedef FILTER_RESTART (*FILTER_RESTART_HANDLER);

#define NDIS_FILTER_PAUSE_PARAMETERS_REVISION_1     1

typedef struct _NDIS_FILTER_PAUSE_PARAMETERS
{
    NDIS_OBJECT_HEADER          Header;
    ULONG                       Flags;
    ULONG                       PauseReason;
} NDIS_FILTER_PAUSE_PARAMETERS, *PNDIS_FILTER_PAUSE_PARAMETERS;

#define NDIS_SIZEOF_FILTER_PAUSE_PARAMETERS_REVISION_1   \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_FILTER_PAUSE_PARAMETERS, PauseReason)

typedef
_IRQL_requires_max_(PASSIVE_LEVEL)
_Function_class_(FILTER_PAUSE)
NDIS_STATUS
(FILTER_PAUSE)(
    _In_  NDIS_HANDLE                     FilterModuleContext,
    _In_  PNDIS_FILTER_PAUSE_PARAMETERS   PauseParameters
    );

typedef FILTER_PAUSE (*FILTER_PAUSE_HANDLER);
//
// inbound requests/data
//

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(FILTER_OID_REQUEST)
NDIS_STATUS
(FILTER_OID_REQUEST)(
    _In_  NDIS_HANDLE             FilterModuleContext,
    _In_  PNDIS_OID_REQUEST       OidRequest
    );

typedef FILTER_OID_REQUEST (*FILTER_OID_REQUEST_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(FILTER_CANCEL_OID_REQUEST)
VOID
(FILTER_CANCEL_OID_REQUEST)(
    _In_  NDIS_HANDLE             FilterModuleContext,
    _In_  PVOID                   RequestId
    );

typedef FILTER_CANCEL_OID_REQUEST (*FILTER_CANCEL_OID_REQUEST_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(FILTER_SEND_NET_BUFFER_LISTS)
VOID
(FILTER_SEND_NET_BUFFER_LISTS)(
    _In_  NDIS_HANDLE             FilterModuleContext,
    _In_  PNET_BUFFER_LIST        NetBufferList,
    _In_  NDIS_PORT_NUMBER        PortNumber,
    _In_  ULONG                   SendFlags
    );

typedef FILTER_SEND_NET_BUFFER_LISTS (*FILTER_SEND_NET_BUFFER_LISTS_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(FILTER_CANCEL_SEND_NET_BUFFER_LISTS)
VOID
(FILTER_CANCEL_SEND_NET_BUFFER_LISTS)(
    _In_  NDIS_HANDLE             FilterModuleContext,
    _In_  PVOID                   CancelId
    );

typedef FILTER_CANCEL_SEND_NET_BUFFER_LISTS (*FILTER_CANCEL_SEND_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(FILTER_RETURN_NET_BUFFER_LISTS)
VOID
(FILTER_RETURN_NET_BUFFER_LISTS)(
    _In_  NDIS_HANDLE             FilterModuleContext,
    _In_  PNET_BUFFER_LIST        NetBufferLists,
    _In_  ULONG                   ReturnFlags
    );

typedef FILTER_RETURN_NET_BUFFER_LISTS (*FILTER_RETURN_NET_BUFFER_LISTS_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(FILTER_SEND_NET_BUFFER_LISTS_COMPLETE)
VOID
(FILTER_SEND_NET_BUFFER_LISTS_COMPLETE)(
    _In_  NDIS_HANDLE             FilterModuleContext,
    _In_  PNET_BUFFER_LIST        NetBufferList,
    _In_  ULONG                   SendCompleteFlags
    );

typedef FILTER_SEND_NET_BUFFER_LISTS_COMPLETE (*FILTER_SEND_NET_BUFFER_LISTS_COMPLETE_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(FILTER_RECEIVE_NET_BUFFER_LISTS)
VOID
(FILTER_RECEIVE_NET_BUFFER_LISTS)(
    _In_  NDIS_HANDLE             FilterModuleContext,
    _In_  PNET_BUFFER_LIST        NetBufferLists,
    _In_  NDIS_PORT_NUMBER        PortNumber,
    _In_  ULONG                   NumberOfNetBufferLists,
    _In_  ULONG                   ReceiveFlags
    );

typedef FILTER_RECEIVE_NET_BUFFER_LISTS (*FILTER_RECEIVE_NET_BUFFER_LISTS_HANDLER);

typedef
_IRQL_requires_max_(PASSIVE_LEVEL)
_Function_class_(FILTER_DEVICE_PNP_EVENT_NOTIFY)
VOID
(FILTER_DEVICE_PNP_EVENT_NOTIFY)(
    _In_  NDIS_HANDLE             FilterModuleContext,
    _In_  PNET_DEVICE_PNP_EVENT   NetDevicePnPEvent
    );

typedef FILTER_DEVICE_PNP_EVENT_NOTIFY (*FILTER_DEVICE_PNP_EVENT_NOTIFY_HANDLER);

typedef
_IRQL_requires_max_(PASSIVE_LEVEL)
_Function_class_(FILTER_NET_PNP_EVENT)
NDIS_STATUS
(FILTER_NET_PNP_EVENT)(
    _In_  NDIS_HANDLE                         FilterModuleContext,
    _In_  PNET_PNP_EVENT_NOTIFICATION         NetPnPEventNotification
    );

typedef FILTER_NET_PNP_EVENT (*FILTER_NET_PNP_EVENT_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(FILTER_STATUS)
VOID
(FILTER_STATUS)(
    _In_  NDIS_HANDLE             FilterModuleContext,
    _In_  PNDIS_STATUS_INDICATION StatusIndication
    );

typedef FILTER_STATUS (*FILTER_STATUS_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(FILTER_OID_REQUEST_COMPLETE)
VOID
(FILTER_OID_REQUEST_COMPLETE)(
    _In_  NDIS_HANDLE             FilterModuleContext,
    _In_  PNDIS_OID_REQUEST       OidRequest,
    _In_  NDIS_STATUS             Status
    );

typedef FILTER_OID_REQUEST_COMPLETE (*FILTER_OID_REQUEST_COMPLETE_HANDLER);
#if (NDIS_SUPPORT_NDIS61)
//
// NDIS 6.1 filter's entry points
//
typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(FILTER_DIRECT_OID_REQUEST)
NDIS_STATUS
(FILTER_DIRECT_OID_REQUEST)(
    _In_  NDIS_HANDLE             FilterModuleContext,
    _In_  PNDIS_OID_REQUEST       OidRequest
    );

typedef FILTER_DIRECT_OID_REQUEST (*FILTER_DIRECT_OID_REQUEST_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(FILTER_DIRECT_OID_REQUEST_COMPLETE)
VOID
(FILTER_DIRECT_OID_REQUEST_COMPLETE)(
    _In_  NDIS_HANDLE             FilterModuleContext,
    _In_  PNDIS_OID_REQUEST       OidRequest,
    _In_  NDIS_STATUS             Status
    );

typedef FILTER_DIRECT_OID_REQUEST_COMPLETE (*FILTER_DIRECT_OID_REQUEST_COMPLETE_HANDLER);

typedef
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(FILTER_CANCEL_DIRECT_OID_REQUEST)
VOID
(FILTER_CANCEL_DIRECT_OID_REQUEST)(
    _In_  NDIS_HANDLE             FilterModuleContext,
    _In_  PVOID                   RequestId
    );

typedef FILTER_CANCEL_DIRECT_OID_REQUEST (*FILTER_CANCEL_DIRECT_OID_REQUEST_HANDLER);
#endif (NDIS_SUPPORT_NDIS61)

#define NDIS_FILTER_PARTIAL_CHARACTERISTICS_REVISION_1      1

typedef struct _NDIS_FILTER_PARTIAL_CHARACTERISTICS
{
    NDIS_OBJECT_HEADER                              Header; // Header.Type = NDIS_OBJECT_TYPE_FILTER_PARTIAL_CHARACTERISTICS
    ULONG                                           Flags;
    FILTER_SEND_NET_BUFFER_LISTS_HANDLER            SendNetBufferListsHandler;
    FILTER_SEND_NET_BUFFER_LISTS_COMPLETE_HANDLER   SendNetBufferListsCompleteHandler;
    FILTER_CANCEL_SEND_HANDLER                      CancelSendNetBufferListsHandler;
    FILTER_RECEIVE_NET_BUFFER_LISTS_HANDLER         ReceiveNetBufferListsHandler;
    FILTER_RETURN_NET_BUFFER_LISTS_HANDLER          ReturnNetBufferListsHandler;
} NDIS_FILTER_PARTIAL_CHARACTERISTICS, *PNDIS_FILTER_PARTIAL_CHARACTERISTICS;

#define NDIS_SIZEOF_FILTER_PARTIAL_CHARACTERISTICS_REVISION_1   \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_FILTER_PARTIAL_CHARACTERISTICS, ReturnNetBufferListsHandler)

//
// Filter driver flags
//
#define NDIS_FILTER_DRIVER_MANDATORY                0x00000001

#define NDIS_FILTER_CHARACTERISTICS_REVISION_1      1

#if (NDIS_SUPPORT_NDIS61)
#define NDIS_FILTER_CHARACTERISTICS_REVISION_2      2
#endif // (NDIS_SUPPORT_NDIS61)

typedef struct _NDIS_FILTER_DRIVER_CHARACTERISTICS
{
    NDIS_OBJECT_HEADER                              Header;
    UCHAR                                           MajorNdisVersion;
    UCHAR                                           MinorNdisVersion;
    UCHAR                                           MajorDriverVersion;
    UCHAR                                           MinorDriverVersion;
    ULONG                                           Flags;
    NDIS_STRING                                     FriendlyName;
    NDIS_STRING                                     UniqueName;
    NDIS_STRING                                     ServiceName;
    SET_OPTIONS_HANDLER                             SetOptionsHandler;
    FILTER_SET_FILTER_MODULE_OPTIONS_HANDLER        SetFilterModuleOptionsHandler;
    FILTER_ATTACH_HANDLER                           AttachHandler;
    FILTER_DETACH_HANDLER                           DetachHandler;
    FILTER_RESTART_HANDLER                          RestartHandler;
    FILTER_PAUSE_HANDLER                            PauseHandler;
    FILTER_SEND_NET_BUFFER_LISTS_HANDLER            SendNetBufferListsHandler;
    FILTER_SEND_NET_BUFFER_LISTS_COMPLETE_HANDLER   SendNetBufferListsCompleteHandler;
    FILTER_CANCEL_SEND_HANDLER                      CancelSendNetBufferListsHandler;
    FILTER_RECEIVE_NET_BUFFER_LISTS_HANDLER         ReceiveNetBufferListsHandler;
    FILTER_RETURN_NET_BUFFER_LISTS_HANDLER          ReturnNetBufferListsHandler;
    FILTER_OID_REQUEST_HANDLER                      OidRequestHandler;
    FILTER_OID_REQUEST_COMPLETE_HANDLER             OidRequestCompleteHandler;
    FILTER_CANCEL_OID_REQUEST_HANDLER               CancelOidRequestHandler;
    FILTER_DEVICE_PNP_EVENT_NOTIFY_HANDLER          DevicePnPEventNotifyHandler;
    FILTER_NET_PNP_EVENT_HANDLER                    NetPnPEventHandler;
    FILTER_STATUS_HANDLER                           StatusHandler;
#if (NDIS_SUPPORT_NDIS61)
    FILTER_DIRECT_OID_REQUEST_HANDLER               DirectOidRequestHandler;
    FILTER_DIRECT_OID_REQUEST_COMPLETE_HANDLER      DirectOidRequestCompleteHandler;
    FILTER_CANCEL_DIRECT_OID_REQUEST_HANDLER        CancelDirectOidRequestHandler;
#endif // (NDIS_SUPPORT_NDIS61)
} NDIS_FILTER_DRIVER_CHARACTERISTICS, *PNDIS_FILTER_DRIVER_CHARACTERISTICS;
#define NDIS_SIZEOF_FILTER_DRIVER_CHARACTERISTICS_REVISION_1   \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_FILTER_DRIVER_CHARACTERISTICS, StatusHandler)

#if (NDIS_SUPPORT_NDIS61)
#define NDIS_SIZEOF_FILTER_DRIVER_CHARACTERISTICS_REVISION_2   \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_FILTER_DRIVER_CHARACTERISTICS, CancelDirectOidRequestHandler)
#endif //(NDIS_SUPPORT_NDIS61)

#define NDIS_FILTER_ATTRIBUTES_REVISION_1       1

typedef struct _NDIS_FILTER_ATTRIBUTES
{
    NDIS_OBJECT_HEADER                      Header;
    ULONG                                   Flags;
} NDIS_FILTER_ATTRIBUTES, *PNDIS_FILTER_ATTRIBUTES;

#define NDIS_SIZEOF_FILTER_ATTRIBUTES_REVISION_1    \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_FILTER_ATTRIBUTES, Flags)

_IRQL_requires_max_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisFRegisterFilterDriver(
    _In_     PDRIVER_OBJECT                          DriverObject,
    _In_opt_ NDIS_HANDLE                             FilterDriverContext,
    _In_     PNDIS_FILTER_DRIVER_CHARACTERISTICS     FilterDriverCharacteristics,
    _Out_    PNDIS_HANDLE                            NdisFilterDriverHandle
    );


_IRQL_requires_max_(PASSIVE_LEVEL)
EXPORT
VOID
NdisFDeregisterFilterDriver(
    _In_ NDIS_HANDLE                      NdisFilterDriverHandle
    );

_IRQL_requires_max_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisFSetAttributes(
    _In_  NDIS_HANDLE                     NdisFilterHandle,
    _In_ __drv_aliasesMem NDIS_HANDLE     FilterModuleContext,
    _In_  PNDIS_FILTER_ATTRIBUTES         FilterAttributes
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisFRestartFilter(
    _In_  NDIS_HANDLE             NdisFilterHandle
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFSendNetBufferLists(
    _In_  NDIS_HANDLE             NdisFilterHandle,
    _In_ __drv_aliasesMem PNET_BUFFER_LIST NetBufferList,
    _In_  NDIS_PORT_NUMBER        PortNumber,
    _In_  ULONG                   SendFlags
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFReturnNetBufferLists(
    _In_  NDIS_HANDLE             NdisFilterHandle,
    _In_  PNET_BUFFER_LIST        NetBufferLists,
    _In_  ULONG                   ReturnFlags
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFSendNetBufferListsComplete(
    _In_  NDIS_HANDLE             NdisFilterHandle,
    _In_  PNET_BUFFER_LIST        NetBufferList,
    _In_  ULONG                   SendCompleteFlags
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFIndicateReceiveNetBufferLists(
    _In_  NDIS_HANDLE             NdisFilterHandle,
    _In_  PNET_BUFFER_LIST        NetBufferLists,
    _In_  NDIS_PORT_NUMBER        PortNumber,
    _In_  ULONG                   NumberOfNetBufferLists,
    _In_  ULONG                   ReceiveFlags
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisFOidRequest(
    _In_  NDIS_HANDLE             NdisFilterHandle,
    _In_  PNDIS_OID_REQUEST       OidRequest
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFOidRequestComplete(
    _In_  NDIS_HANDLE             NdisFilterHandle,
    _In_  PNDIS_OID_REQUEST       OidRequest,
    _In_  NDIS_STATUS             Status
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFIndicateStatus(
    _In_  NDIS_HANDLE             NdisFilterHandle,
    _In_  PNDIS_STATUS_INDICATION StatusIndication
    );


_IRQL_requires_max_(PASSIVE_LEVEL)
EXPORT
VOID
NdisFRestartComplete(
    _In_  NDIS_HANDLE             NdisFilterHandle,
    _In_  NDIS_STATUS             Status
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFPauseComplete(
    _In_  NDIS_HANDLE             NdisFilterHandle
    );


_IRQL_requires_max_(PASSIVE_LEVEL)
EXPORT
VOID
NdisFDevicePnPEventNotify(
    _In_  NDIS_HANDLE             NdisFilterHandle,
    _In_  PNET_DEVICE_PNP_EVENT   NetDevicePnPEvent
    );

_IRQL_requires_max_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisFNetPnPEvent(
    _In_  NDIS_HANDLE                 NdisFilterHandle,
    _In_  PNET_PNP_EVENT_NOTIFICATION NetPnPEventNotification
    );


_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFCancelSendNetBufferLists(
    _In_  NDIS_HANDLE     NdisFilterHandle,
    _In_  PVOID           CancelId
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFCancelOidRequest(
    _In_  NDIS_HANDLE             NdisFilterHandle,
    _In_  PVOID                   RequestId
    );

#if (NDIS_SUPPORT_NDIS61)
_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisFDirectOidRequest(
    _In_  NDIS_HANDLE             NdisFilterHandle,
    _In_  PNDIS_OID_REQUEST       OidRequest
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFDirectOidRequestComplete(
    _In_  NDIS_HANDLE             NdisFilterHandle,
    _In_  PNDIS_OID_REQUEST       OidRequest,
    _In_  NDIS_STATUS             Status
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
VOID
NdisFCancelDirectOidRequest(
    _In_  NDIS_HANDLE             NdisFilterHandle,
    _In_  PVOID                   RequestId
    );

#endif // (NDIS_SUPPORT_NDIS61)

#if (NDIS_SUPPORT_NDIS630)

_IRQL_requires_max_(DISPATCH_LEVEL)
EXPORT
NDIS_STATUS
NdisFGetOptionalSwitchHandlers(
    _In_     NDIS_HANDLE                      NdisFilterHandle,
    _Out_    PNDIS_SWITCH_CONTEXT             NdisSwitchContext,
    _Inout_  PNDIS_SWITCH_OPTIONAL_HANDLERS   NdisSwitchHandlers
    );

#endif // (NDIS_SUPPORT_NDIS630)

#endif // NDIS_SUPPORT_NDIS6

//
//    Copyright (C) Microsoft.  All rights reserved.
//
#pragma once

#if NDIS_SUPPORT_NDIS6

//
// NDIS IF data structures, function prototypes and macros
//

typedef
_IRQL_requires_max_(PASSIVE_LEVEL)
_Function_class_(IF_QUERY_OBJECT)
NDIS_STATUS
(IF_QUERY_OBJECT)(
    _In_    NDIS_HANDLE         ProviderIfContext,
    _In_    NET_IF_OBJECT_ID    ObjectId,
    _Inout_ PULONG              pOutputBufferLength,
    _Out_writes_bytes_to_(*pOutputBufferLength, *pOutputBufferLength)
            PVOID               pOutputBuffer
    );

typedef IF_QUERY_OBJECT *IFP_QUERY_OBJECT;

typedef
_IRQL_requires_max_(PASSIVE_LEVEL)
_Function_class_(IF_SET_OBJECT)
NDIS_STATUS
(IF_SET_OBJECT)(
    _In_ NDIS_HANDLE          ProviderIfContext,
    _In_ NET_IF_OBJECT_ID     ObjectId,
    _In_ ULONG                InputBufferLength,
    _In_reads_bytes_(InputBufferLength)
         PVOID                pInputBuffer
    );

typedef IF_SET_OBJECT *IFP_SET_OBJECT;

typedef struct _NDIS_IF_PROVIDER_CHARACTERISTICS
{
    NDIS_OBJECT_HEADER      Header;

    //
    // Generic query and set handlers:
    //
    IFP_QUERY_OBJECT        QueryObjectHandler;
    IFP_SET_OBJECT          SetObjectHandler;

    PVOID                   Reserved1;
    PVOID                   Reserved2;

} NDIS_IF_PROVIDER_CHARACTERISTICS, *PNDIS_IF_PROVIDER_CHARACTERISTICS;

#define NDIS_SIZEOF_IF_PROVIDER_CHARACTERISTICS_REVISION_1   \
        RTL_SIZEOF_THROUGH_FIELD(NDIS_IF_PROVIDER_CHARACTERISTICS, Reserved2)

//
// NET_IF_INFORMATION is passed to NdisIfRegisterInterface
//
typedef struct _NET_IF_INFORMATION
{
    NDIS_OBJECT_HEADER      Header;
    ULONG                   Flags;              // Misc. information
    NET_PHYSICAL_LOCATION   PhysicalLocation;   // physical location on machine
    ULONG                   WanTunnelType;      // tunnelIfEncapsMethod (RFC 2667)
    ULONG                   PortNumber;         // for WAN devices.
    NET_IF_ACCESS_TYPE      AccessType;
    NET_IF_DIRECTION_TYPE   DirectionType;
    NET_IF_CONNECTION_TYPE  ConnectionType;
    BOOLEAN                 ifConnectorPresent;
    USHORT                  PhysAddressLength;  // in bytes (ifPhysAddress). this is -current- mac address
    USHORT                  PhysAddressOffset;  // from beginning of this struct
    USHORT                  PermanentPhysAddressOffset;  // from beginning of this struct
    //
    //  The "friendly name" represents ifDescr:
    //
    USHORT                  FriendlyNameLength; // in bytes
    USHORT                  FriendlyNameOffset; // from beginning of this struct
    GUID                    InterfaceGuid;
    NET_IF_NETWORK_GUID     NetworkGuid;
    ULONG                   SupportedStatistics;
    NDIS_MEDIUM             MediaType;
    NDIS_PHYSICAL_MEDIUM    PhysicalMediumType;
} NET_IF_INFORMATION, *PNET_IF_INFORMATION;


#define NDIS_SIZEOF_NET_IF_INFORMATION_REVISION_1   \
        RTL_SIZEOF_THROUGH_FIELD(NET_IF_INFORMATION, PhysicalMediumType)

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisIfRegisterProvider(
    _In_     PNDIS_IF_PROVIDER_CHARACTERISTICS   ProviderCharacteristics,
    _In_opt_ NDIS_HANDLE                         IfProviderContext,
    _Out_    PNDIS_HANDLE                        pNdisIfProviderHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisIfDeregisterProvider(
    _In_ NDIS_HANDLE          NdisProviderHandle
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisIfAllocateNetLuidIndex(
    _In_   NET_IFTYPE          ifType,
    _Out_  PUINT32             pNetLuidIndex
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisIfFreeNetLuidIndex(
    _In_ NET_IFTYPE           ifType,
    _In_ UINT32               NetLuidIndex
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisIfRegisterInterface(
    _In_   NDIS_HANDLE          NdisProviderHandle,
    _In_   NET_LUID             NetLuid,
    _In_   NDIS_HANDLE          ProviderIfContext,
    _In_   PNET_IF_INFORMATION  pIfInfo,
    _Out_  PNET_IFINDEX         pfIndex
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisIfDeregisterInterface(
    _In_ NET_IFINDEX          ifIndex
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisIfGetInterfaceIndexFromNetLuid(
    _In_   NET_LUID            NetLuid,
    _Out_  PNET_IFINDEX        pIfIndex
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisIfGetNetLuidFromInterfaceIndex(
    _In_   NET_IFINDEX         ifIndex,
    _Out_  PNET_LUID           pNetLuid
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisIfQueryBindingIfIndex(
    _In_   NDIS_HANDLE         NdisBindingHandle,
    _Out_  PNET_IFINDEX        pBoundIfIndex,
    _Out_  PNET_LUID           pBoundIfNetLuid,
    _Out_  PNET_IFINDEX        pLowestIfIndex,
    _Out_  PNET_LUID           pLowestIfNetLuid
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
NDIS_STATUS
NdisIfAddIfStackEntry(
    _In_ NET_IFINDEX          HigherLayerIfIndex,
    _In_ NET_IFINDEX          LowerLayerIfIndex
    );

_IRQL_requires_(PASSIVE_LEVEL)
EXPORT
VOID
NdisIfDeleteIfStackEntry(
    _In_ NET_IFINDEX          HigherLayerIfIndex,
    _In_ NET_IFINDEX          LowerLayerIfIndex
    );

#endif // NDIS_SUPPORT_NDIS6

#endif // _NDIS_


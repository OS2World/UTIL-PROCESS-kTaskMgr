/* $Id: kOS2Info.h,v 1.1 2000/05/23 00:16:50 stknut Exp $
 *
 * OS/2 Information.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 * Project Odin Software License can be found in LICENSE.TXT
 *
 */
#ifndef _kOS2Info_h_
#define _kOS2Info_h_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
*   Structures and Typedefs                                                    *
*******************************************************************************/
typedef union _sysinfo_u
{
     struct _sysinfo_s
     {
         ULONG   cchMaxPathLength;
        ULONG   cMaxTextSessions;
        ULONG   cMaxPMSessions;
        ULONG   cMaxVDMSessions;
        ULONG   ulBootDrive;
        ULONG   fDynPriVariation;
        ULONG   ulMaxWait;
        ULONG   ulMinSlice;
        ULONG   ulMaxSlice;
        ULONG   cbPageSize;
        ULONG   ulVersionMajor;
        ULONG   ulVersionMinor;
        ULONG   ulVersionRevision;
        ULONG   ulMSCount;
        ULONG   ulTimeLow;
        ULONG   ulTimeHigh;
        ULONG   ulTotalPhysicalMem;
        ULONG   ulTotalResidentMem;
        ULONG   ulTotalAvailableMem;
        ULONG   ulMaxPrivateMem;
        ULONG   ulMaxShredMem;
        ULONG   ulTimerInterval;
        ULONG   cchMaxCompLength;
        ULONG   ulForegroundFSSession;
        ULONG   ulForegroundProcess;
        ULONG   ulNumProcessors;
        ULONG   ulMaxHighPrivateMemory;
        ULONG   ulMaxHighSharedMemory;
        ULONG   ulMaxProcesses;
        ULONG   ulVirtualAddressLimit;
        ULONG   ulInt10Enabled;
    } s;
    ULONG ach[50];
} SYSINFO, *PSYSINFO;


/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
extern BOOL     fOS2InfoInited;
extern ULONG    ulBuildLevel;
extern BOOL     fSMPKernel;
extern BOOL     fW4Kernel;
extern BOOL     fUNIKernel;

extern ULONG    ulMaxSysInfoIndex;
extern SYSINFO  SysInfo;


/*******************************************************************************
*   External Functions                                                         *
*******************************************************************************/
BOOL    OS2InfoInit(VOID);
BOOL    OS2UpdateSysInfo(VOID);

#ifdef __cplusplus
}
#endif

#endif

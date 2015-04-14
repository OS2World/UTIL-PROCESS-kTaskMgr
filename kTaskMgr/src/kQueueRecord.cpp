/* $Id: kQueueRecord.cpp,v 1.1 2000/05/23 00:12:46 stknut Exp $
 *
 * kQueueRecord - Container record.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */



/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
#define INCL_DOSQUEUES
#define INCL_DOSERRORS
#define INCL_WIN

/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <os2.h>
#ifdef USE_KLIB
    #include <kAssert.h>
    #include <kLog.h>
    #include <kHeap.h>
#endif
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "kBase.h"
#include "kError.h"
#include "kContainer.h"

#include "kQueueHack.h"
#include "kQuerySysState.h"
#include "kTaskMgrRecordBase.h"
#include "kQueueRecord.h"


/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
const FIELDINFO kQueueRecord::aFieldInfo[] =
{
    {
       sizeof(FIELDINFO),
       CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
       CFA_STRING,
       (PVOID)"hQueue",
       offsetof(kQueueRecord, pszhQueue),
       NULL,
       NULL,
       0L
    }
    ,
    {
       sizeof(FIELDINFO),
       CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
       CFA_STRING,
       (PVOID)"Owner\r\nPid",
       offsetof(kQueueRecord, pszOwnPid),
       NULL,
       NULL,
       0L
    }
    ,
    {
       sizeof(FIELDINFO),
       CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_LEFT,
       CFA_STRING,
       (PVOID)"Flags",
       offsetof(kQueueRecord, pszFlags),
       NULL,
       NULL,
       0L
    }
    ,
    {
       sizeof(FIELDINFO),
       CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_LEFT,
       CFA_STRING,
       (PVOID)"Priority",
       offsetof(kQueueRecord, pszPriority),
       NULL,
       NULL,
       0L
    }
    ,
    {
       sizeof(FIELDINFO),
       CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
       CFA_STRING,
       (PVOID)"#Elements",
       offsetof(kQueueRecord, pszEntries),
       NULL,
       NULL,
       0L
    }
    ,
    {
       sizeof(FIELDINFO),
       CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
       CFA_STRING,
       (PVOID)"Owner\r\n#Opens",
       offsetof(kQueueRecord, pszOwnerOpens),
       NULL,
       NULL,
       0L
    }
    ,
    {
       sizeof(FIELDINFO),
       CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
       CFA_STRING,
       (PVOID)"Memory\r\nUsage",
       offsetof(kQueueRecord, pszUsedMem),
       NULL,
       NULL,
       0L
    }
    ,
    {
       sizeof(FIELDINFO),
       CFA_STRING | CFA_HORZSEPARATOR,
       CFA_STRING,
       (PVOID)"Name",
       offsetof(kQueueRecord, pszName),
       NULL,
       NULL,
       0L
    }
};

const unsigned  kQueueRecord::cFieldInfo = 8;


/**
 * Init function.
 */
VOID    kQueueRecord::init()
{
    kTaskMgrRecordBase::init();
    pszhQueue = &szhQueue[0];
    pszOwnPid = &szOwnPid[0];
    pszFlags = &szFlags[0];
    pszPriority = &szPriority[0];
    pszEntries = &szEntries[0];
    pszOwnerOpens = &szOwnerOpens[0];
    pszUsedMem = &szUsedMem[0];
    pszName = &szName[0];
    ushQueue = 0xFFFF;
}


/**
 * Set the data of the record.
 * @param     pQueEntry     Pointer to queue record from kQueueHack.c.
 */
VOID    kQueueRecord::set(PMYQUEENTRY pQueEntry)
{
    BOOL    fPar = FALSE;
    ushQueue = pQueEntry->que.hQueue;
    sprintf(szhQueue,   "0x%04x", ushQueue);
    sprintf(szOwnPid,   "0x%04x", pQueEntry->que.usOwnPid);

    sprintf(szFlags,    "0x%04x", pQueEntry->que.chFlags);
    if (pQueEntry->que.chFlags & 0x08)
        fPar = (BOOL)strcat(szFlags, " (THUNK");
    if (pQueEntry->que.chFlags & 0x04)
        fPar = (BOOL)strcat(strcat(szFlags, fPar ? " | " : "("), "32BIT");
    if (fPar)
        strcat(szFlags, ")");

    sprintf(szPriority, "0x%04x", pQueEntry->que.usPriority);
    if (pQueEntry->que.usPriority & QUE_LIFO)
        strcat(szPriority, " (LIFO)");
    else if (pQueEntry->que.usPriority & QUE_PRIORITY)
        strcat(szPriority, " (PRIORITY)");
    else
        strcat(szPriority, " (FIFO)");

    sprintf(szEntries,  "0x%04x", pQueEntry->que.cEntries);
    sprintf(szOwnerOpens,"0x%04x", pQueEntry->que.cOpen);
    sprintf(szUsedMem,   "%4d bytes", QueQueryUsedMemoryQueue(pQueEntry));
    szName[0] = '\0';
    strncat(szName, pQueEntry->que.szQueName, sizeof(szName));
}



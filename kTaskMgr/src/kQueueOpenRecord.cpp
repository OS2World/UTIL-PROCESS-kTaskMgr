/* $Id: kQueueOpenRecord.cpp,v 1.1 2000/05/23 00:12:46 stknut Exp $
 *
 * kQueueOpenRecord - Container record.
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
#include "kQueueOpenRecord.h"


/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
const FIELDINFO kQueueOpenRecord::aFieldInfo[] =
{
    {
        sizeof(FIELDINFO),
        CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
        CFA_STRING,
        (PVOID)"hQueue",
        offsetof(kQueueOpenRecord, pszhQueue),
        NULL,
        NULL,
        0L
    }
    ,
    {
        sizeof(FIELDINFO),
        CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
        CFA_STRING,
        (PVOID)"Pid",
        offsetof(kQueueOpenRecord, pszPid),
        NULL,
        NULL,
        0L
    }
    ,
    {
        sizeof(FIELDINFO),
        CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
        CFA_STRING,
        (PVOID)"#Opens",
        offsetof(kQueueOpenRecord, pszPidOpenCnt),
        NULL,
        NULL,
        0L
    }
    ,
    {
        sizeof(FIELDINFO),
        CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
        CFA_STRING,
        (PVOID)"Openflags",
        offsetof(kQueueOpenRecord, pszOpenFlags),
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
        offsetof(kQueueOpenRecord, pszOwnPid),
        NULL,
        NULL,
        0L
    }
    ,
    {
        sizeof(FIELDINFO),
        CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
        CFA_STRING,
        (PVOID)"Queueflags",
        offsetof(kQueueOpenRecord, pszFlags),
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
        offsetof(kQueueOpenRecord, pszPriority),
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
        offsetof(kQueueOpenRecord, pszEntries),
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
        offsetof(kQueueOpenRecord, pszOwnerOpens),
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
        offsetof(kQueueOpenRecord, pszName),
        NULL,
        NULL,
        0L
    }
};

const unsigned  kQueueOpenRecord::cFieldInfo = 10;


/**
 * Init function.
 */
VOID    kQueueOpenRecord::init()
{
    kTaskMgrRecordBase::init();
    usPid = 0xffff;
    ushQueue = 0xffff;
    pszhQueue = &szhQueue[0];
    pszPid = &szPid[0];
    pszPidOpenCnt = &szPidOpenCnt[0];
    pszOpenFlags = &szOpenFlags[0];
    pszOwnPid = &szOwnPid[0];
    pszFlags = &szFlags[0];
    pszPriority = &szPriority[0];
    pszEntries = &szEntries[0];
    pszOwnerOpens = &szOwnerOpens[0];
    pszName = &szName[0];
}


/**
 * Set the data of the record.
 * @param     pQueEntry     Pointer to queue record from kQueueHack.c.
 */
VOID    kQueueOpenRecord::set(PMYQUEOPENENTRY pOpenEntry)
{
    PMYQUEENTRY pQueEntry = QueGetFirstQueueData();
    BOOL    fPar = FALSE;

    /*
     * Find queue info.
     */
    while (pQueEntry != NULL && pQueEntry->que.hQueue != pOpenEntry->hQueue)
        pQueEntry = pQueEntry->pNext;


    ushQueue = pOpenEntry->hQueue;
    sprintf(szhQueue,       "0x%04x", ushQueue);
    usPid = pOpenEntry->usPid;
    sprintf(szPid,          "0x%04x", usPid);
    sprintf(szPidOpenCnt,   "0x%02x", pOpenEntry->chRefCnt);
    sprintf(szOpenFlags,    "0x%02x", pOpenEntry->chFlags);

    if (pQueEntry)
    {
        sprintf(szOwnPid,   "0x%04x", pQueEntry->que.usOwnPid);
        sprintf(szFlags,    "0x%02x", pQueEntry->que.chFlags);
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
        sprintf(szOwnerOpens,"0x%02x", pQueEntry->que.cOpen);
        szName[0] = '\0';
        strncat(szName, pQueEntry->que.szQueName, sizeof(szName));
    }
    else
    {
        strcpy(szOwnPid, "-");
        strcpy(szOwnPid, "-");
        strcpy(szOpenFlags, "-");
        strcpy(szPriority, "-");
        strcpy(szEntries, "-");
        strcpy(szOwnerOpens, "-");
        strcpy(szName, "-");
    }
}



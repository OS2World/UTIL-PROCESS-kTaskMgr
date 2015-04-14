/* $Id: kThreadRecord.cpp,v 1.1 2000/05/23 00:12:46 stknut Exp $
 *
 * kThreadRecord - Thread container record.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
#define INCL_WIN
#define INCL_DOS

/*******************************************************************************
*   Internal Functions                                                         *
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
#include "kQuerySysState.h"
#include "kTaskMgrRecordBase.h"
#include "kThreadRecord.h"

/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
const FIELDINFO kThreadRecord::aFieldInfo[] =
{
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
      CFA_STRING,
      (PVOID)"Tid",
      offsetof(kThreadRecord, pszTid),
      NULL,
      NULL,
      0L
   }
   ,
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR,
      CFA_STRING,
      (PVOID)"Slot",
      offsetof(kThreadRecord, pszSlot),
      NULL,
      NULL,
      0L
   }
   ,
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR,
      CFA_STRING,
      (PVOID)"SleepId",
      offsetof(kThreadRecord, pszSleepId),
      NULL,
      NULL,
      0L
   }
   ,
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR,
      CFA_STRING,
      (PVOID)"Priority",
      offsetof(kThreadRecord, pszPriority),
      NULL,
      NULL,
      0L
   }
   ,
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR,
      CFA_STRING,
      (PVOID)"State",
      offsetof(kThreadRecord, pszState),
      NULL,
      NULL,
      0L
   }
   ,
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR,
      CFA_STRING,
      (PVOID)"Systime",
      offsetof(kThreadRecord, pszSysTime),
      NULL,
      NULL,
      0L
   }
   ,
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR,
      CFA_STRING,
      (PVOID)"Usertime",
      offsetof(kThreadRecord, pszUserTime),
      NULL,
      NULL,
      0L
   }
};

const unsigned  kThreadRecord::cFieldInfo = 7;

const FIELDINFO kThreadRecord::aFieldInfoGlobal[] =
{
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
      CFA_STRING,
      (PVOID)"Tid",
      offsetof(kThreadRecord, pszTid),
      NULL,
      NULL,
      0L
   }
   ,
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR,
      CFA_STRING,
      (PVOID)"Slot",
      offsetof(kThreadRecord, pszSlot),
      NULL,
      NULL,
      0L
   }
   ,
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR,
      CFA_STRING,
      (PVOID)"Pid",
      offsetof(kThreadRecord, pszPid),
      NULL,
      NULL,
      0L
   }
   ,
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR,
      CFA_STRING,
      (PVOID)"SleepId",
      offsetof(kThreadRecord, pszSleepId),
      NULL,
      NULL,
      0L
   }
   ,
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR,
      CFA_STRING,
      (PVOID)"Priority",
      offsetof(kThreadRecord, pszPriority),
      NULL,
      NULL,
      0L
   }
   ,
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR,
      CFA_STRING,
      (PVOID)"State",
      offsetof(kThreadRecord, pszState),
      NULL,
      NULL,
      0L
   }
   ,
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR,
      CFA_STRING,
      (PVOID)"Systime",
      offsetof(kThreadRecord, pszSysTime),
      NULL,
      NULL,
      0L
   }
   ,
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR,
      CFA_STRING,
      (PVOID)"Usertime",
      offsetof(kThreadRecord, pszUserTime),
      NULL,
      NULL,
      0L
   }
};



const unsigned  kThreadRecord::cFieldInfoGlobal = 8;

/**
 * Initiates the struct. (zero fields some field)
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
VOID  kThreadRecord::init()
{
    kTaskMgrRecordBase::init();
    usTid       = (USHORT)~0;
    usSlot      = (USHORT)~0;
    usPid       = (USHORT)~0;
    pszTid      = &szTid[0];
    pszSlot     = &szSlot[0];
    pszPid      = &szPid[0];
    pszSleepId  = &szSleepId[0];
    pszPriority = &szPriority[0];
    pszUserTime = &szUserTime[0];
    pszSysTime  = &szSysTime[0];
    pszState    = &szState[0];
    *pszTid = *pszSlot = *pszPid = *pszSleepId = *pszPriority = *pszUserTime = *pszSysTime = *pszState = '\0';
}


/**
 * Set record information according to the passed in pThreadRec struct.
 * This is also used to update data.
 * @param     pThreadRec    Thread information from QS.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark
 */
VOID  kThreadRecord::set(qsTrec_t *pThreadRec)
{
    ULONG   ulTime;

    /*
     * Set and format
     */
    sprintf(szTid,      "%04x", pThreadRec->tid);
    sprintf(szSlot,     "%04x", pThreadRec->slot);
    sprintf(szSleepId,  "%08x", pThreadRec->sleepid);
    sprintf(szPriority, "%08x", pThreadRec->priority);
    sprintf(szState,    "%08x", pThreadRec->state);

    ulTime = pThreadRec->systime * 32;
    sprintf(szSysTime,  "%02d:%02d:%02d,%03d",
            (ulTime / 3600000),
            (ulTime / 60000) % 60,
            (ulTime / 1000) % 60,
             ulTime % 1000);

    ulTime = pThreadRec->usertime * 32;
    sprintf(szUserTime,  "%02d:%02d:%02d,%03d",
            (ulTime / 3600000),
            (ulTime / 60000) % 60,
            (ulTime / 1000) % 60,
             ulTime % 1000);
}



/**
 * Set record information according to the passed in pThreadRec and pProcData structs.
 * This is also used to update data.
 * @param     pThreadRec    Thread information from QS.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark
 */
VOID  kThreadRecord::set(qsTrec_t *pThreadRec, PPROCESSDATA    pProcData)
{
    set(pThreadRec);
    sprintf(szPid, "0x%04x", pProcData->usPid);
}



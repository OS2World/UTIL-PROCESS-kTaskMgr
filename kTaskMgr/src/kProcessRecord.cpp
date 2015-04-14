/* $Id: kProcessRecord.cpp,v 1.1 2000/05/23 00:12:45 stknut Exp $
 *
 * kProcessRecord - Process container record.
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
#include "kProcessRecord.h"

/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
const FIELDINFO kProcessRecord::aFieldInfo[] =
{
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
      CFA_STRING,
      (PVOID)"Pid",
      offsetof(kProcessRecord, pszPid),
      NULL,
      NULL,
      0L
   }
   ,
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR,
      CFA_STRING,
      (PVOID)"PPid",
      offsetof(kProcessRecord, pszPPid),
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
      offsetof(kProcessRecord, pszSysTime),
      NULL,
      NULL,
      0L
   }
   ,
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR,
      CFA_STRING,
      (PVOID)"Usertime",
      offsetof(kProcessRecord, pszUserTime),
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
      offsetof(kProcessRecord, pszName),
      NULL,
      NULL,
      0L
   }
};

const unsigned  kProcessRecord::cFieldInfo = 5;

/**
 * Initiates the struct. (zero fields some field)
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
VOID  kProcessRecord::init()
{
    kTaskMgrRecordBase::init();
    pProcData = NULL;
}


/**
 * Set record information according to the passed in pProcData struct.
 * This is also used to update data.
 * @param     pProcData  Process information from QS.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark
 */
VOID  kProcessRecord::set(PPROCESSDATA pProcData)
{
    if (this->pProcData == NULL || this->pProcData != pProcData)
    {   /* new data */
        qsLrec_t *pMteData;

        this->pProcData = pProcData;
        usPid = pProcData->usPid;
        pszPid = &szPid[0];
        pszPPid = &szPPid[0];
        pszName = &szName[0];
        pszSysTime = &szSysTime[0];
        pszUserTime = &szUserTime[0];

        /* get process name */
        pMteData = QSGetMteData(pProcData->pProcRec->hMte);
        if (pMteData != NULL)
            strcpy(pszName, (const char*)pMteData->pName);
        if (pMteData == NULL || pszName == NULL)
            strcpy(pszName, "<unknown>");
    }

    /*
     * Set and format
     */
    sprintf(szPid,      "%04x", pProcData->usPid);
    sprintf(szPPid,     "%04x", pProcData->pProcRec->ppid);
    ULONG   ulTime = pProcData->ulSysTime * 32;
    sprintf(szSysTime,  "%02d:%02d:%02d,%03d",
            (ulTime / 3600000),
            (ulTime / 60000) % 60,
            (ulTime / 1000) % 60,
             ulTime % 1000);
    ulTime = pProcData->ulUserTime * 32;
    sprintf(szUserTime,  "%02d:%02d:%02d,%03d",
            (ulTime / 3600000),
            (ulTime / 60000) % 60,
            (ulTime / 1000) % 60,
             ulTime % 1000);
}



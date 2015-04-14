/* $Id: kSFNRecord.cpp,v 1.1 2000/05/23 00:12:46 stknut Exp $
 *
 * kSFNRecord - Container record.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */



/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
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

#include "kQuerySysState.h"
#include "kTaskMgrRecordBase.h"
#include "kSFNRecord.h"


/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
const FIELDINFO kSFNRecord::aFieldInfo[] =
{
    {
       sizeof(FIELDINFO),
       CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
       CFA_STRING,
       (PVOID)"JFN",
       offsetof(kSFNRecord, pszJFN),
       NULL,
       NULL,
       0L
    }
    ,
    {
       sizeof(FIELDINFO),
       CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
       CFA_STRING,
       (PVOID)"SFN",
       offsetof(kSFNRecord, pszSFN),
       NULL,
       NULL,
       0L
    }
    ,
    {
       sizeof(FIELDINFO),
       CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
       CFA_STRING,
       (PVOID)"RefCnt",
       offsetof(kSFNRecord, pszRefCnt),
       NULL,
       NULL,
       0L
    }
    ,
    {
       sizeof(FIELDINFO),
       CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
       CFA_STRING,
       (PVOID)"Flags",
       offsetof(kSFNRecord, pszFlags),
       NULL,
       NULL,
       0L
    }
    ,
    {
       sizeof(FIELDINFO),
       CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
       CFA_STRING,
       (PVOID)"Flags2",
       offsetof(kSFNRecord, pszFlags2),
       NULL,
       NULL,
       0L
    }
    ,
    {
       sizeof(FIELDINFO),
       CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
       CFA_STRING,
       (PVOID)"Mode",
       offsetof(kSFNRecord, pszMode),
       NULL,
       NULL,
       0L
    }
    ,
    {
       sizeof(FIELDINFO),
       CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
       CFA_STRING,
       (PVOID)"Mode2",
       offsetof(kSFNRecord, pszMode2),
       NULL,
       NULL,
       0L
    }
    ,
    {
       sizeof(FIELDINFO),
       CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
       CFA_STRING,
       (PVOID)"Size",
       offsetof(kSFNRecord, pszSize),
       NULL,
       NULL,
       0L
    }
    ,
    {
       sizeof(FIELDINFO),
       CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
       CFA_STRING,
       (PVOID)"hVPB",
       offsetof(kSFNRecord, pszhVPB),
       NULL,
       NULL,
       0L
    }
    ,
    {
       sizeof(FIELDINFO),
       CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
       CFA_STRING,
       (PVOID)"Attr",
       offsetof(kSFNRecord, pszAttr),
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
       offsetof(kSFNRecord, pszName),
       NULL,
       NULL,
       0L
    }

};

const unsigned  kSFNRecord::cFieldInfo = 11;


/**
 * Init function.
 */
VOID    kSFNRecord::init()
{
    kTaskMgrRecordBase::init();
    pszJFN = &szJFN[0];
    pszSFN = &szSFN[0];
    pszRefCnt = &szRefCnt[0];
    pszFlags = &szFlags[0];
    pszFlags2 = &szFlags2[0];
    pszMode = &szMode[0];
    pszMode2 = &szMode2[0];
    pszSize = &szSize[0];
    pszhVPB = &szhVPB[0];
    pszAttr = &szAttr[0];
    pszName = &szName[0];
    usSFN = 0xFFFF;
}


/**
 * Set the data of the record.
 * @param     pSftRec       Pointer to SFT record from QS.
 * @param     pszFilename   Pointer to filename string.
 * @param     usJFN         Optional process filehandle value. Used when showing process handles.
 * @remark    TODO: Better formatting of various flags and attributes!
 */
VOID    kSFNRecord::set(qsSft_t *pSftRec, PCSZ pszFilename, USHORT usJFN /* 0xFFFF */)
{
    this->usJFN = usJFN;
    sprintf(szJFN,      "0x%04x", usJFN);
    usSFN = pSftRec->sfn;
    sprintf(szSFN,      "0x%04x", usSFN);
    sprintf(szRefCnt,   "0x%04x", pSftRec->refcnt);
    sprintf(szFlags,    "0x%04x", pSftRec->flags);
    sprintf(szFlags2,   "0x%04x", pSftRec->flags2);
    sprintf(szMode,     "0x%04x", pSftRec->mode);
    sprintf(szMode2,    "0x%04x", pSftRec->mode2);
    sprintf(szSize,     "%9d",     pSftRec->size);
    sprintf(szhVPB,     "0x%04x", pSftRec->hVPB);
    sprintf(szAttr,     "0x%04x", pSftRec->attr);
    szName[0] = '\0';
    strncat(szName, pszFilename, sizeof(szName));
}



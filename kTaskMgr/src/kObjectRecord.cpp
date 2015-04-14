/* $Id: kObjectRecord.cpp,v 1.1 2000/05/23 00:12:45 stknut Exp $
 *
 * kObjectRecord - Container record.
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
#include "kObjectRecord.h"


/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
const FIELDINFO kObjectRecord::aFieldInfo[] =
{
    {
        sizeof(FIELDINFO),
        CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
        CFA_STRING,
        (PVOID)"No.",
        offsetof(kObjectRecord, pszObjNo),
        NULL,
        NULL,
        0L
    }
    ,
    {
        sizeof(FIELDINFO),
        CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR,
        CFA_STRING,
        (PVOID)"Base",
        offsetof(kObjectRecord, pszBase),
        NULL,
        NULL,
        0L
    }
    ,
    {
        sizeof(FIELDINFO),
        CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR,
        CFA_STRING,
        (PVOID)"Size",
        offsetof(kObjectRecord, pszSize),
        NULL,
        NULL,
        0L
    }
    ,
    {
        sizeof(FIELDINFO),
        CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR,
        CFA_STRING,
        (PVOID)"Flags",
        offsetof(kObjectRecord, pszFlags),
        NULL,
        NULL,
        0L
    }
    ,
    {
        sizeof(FIELDINFO),
        CFA_STRING | CFA_HORZSEPARATOR,
        CFA_STRING,
        (PVOID)"Expanded Flags",
        offsetof(kObjectRecord, pszFlagsExp),
        NULL,
        NULL,
        0L
    }
};

const unsigned  kObjectRecord::cFieldInfo = 5;


/**
 * Init function.
 */
VOID    kObjectRecord::init()
{
    kTaskMgrRecordBase::init();
    pszObjNo = &szObjNo[0];
    pszBase = &szBase[0];
    pszSize = &szSize[0];
    pszFlags = &szFlags[0];
    pszFlagsExp = &szFlagsExp[0];
}


/**
 * Set the data of the record.
 * @param     pObjRec  Pointer to object record from QS.
 * @param     iObjNo   Object number.
 */
VOID    kObjectRecord::set(qsLObjrec_t *pObjRec, int iObjNo)
{
    static struct _ole
    {
        ULONG flFlag;
        PCSZ  pszDesc;
    } aFlagDesc[] =
    {
        {0x00000001L, "OBJREAD"},       /* Readable Object */
        {0x00000002L, "OBJWRITE"},      /* Writeable Object */
        {0x00000004L, "OBJEXEC"},       /* Executable Object */
        {0x00000008L, "OBJRSRC"},       /* Resource Object */
        {0x00000010L, "OBJDISCARD"},    /* Object is Discardable */
        {0x00000020L, "OBJSHARED"},     /* Object is Shared */
        {0x00000040L, "OBJPRELOAD"},    /* Object has preload pages */
        {0x00000080L, "OBJINVALID"},    /* Object has invalid pages */
        {0x00000100L, "OBJZEROFIL"},    /* Object has zero-filled pages */
        {0x00000200L, "OBJRESIDENT"},   /* Object is resident */
        {0x00000300L, "OBJCONTIG"},     /* Object is resident and contiguous */
        {0x00000400L, "OBJDYNAMIC"},    /* Object is permanent and long locable */
        {0x00001000L, "OBJALIAS16"},    /* 16:16 alias required */
        {0x00002000L, "OBJBIGDEF"},     /* Big/Default bit setting */
        {0x00004000L, "OBJCONFORM"},    /* Object is conforming for code */
        {0x00008000L, "OBJIOPL"},       /* Object I/O privilege level */
        {0x40000000L, "OBJMADEPRIV"},   /* Object is made private for debug (now obsolete) */
        {0x80000000L, "OBJALLOC"}       /* Object is allocated used by loader */
    };

    int i;
    ULONG flObj;
    BOOL  fFirst = TRUE;

    this->iObjNo = iObjNo;
    sprintf(szObjNo,    "%d", iObjNo);
    sprintf(szBase,     "0x%08x", pObjRec->oaddr);
    sprintf(szSize,     "0x%08x", pObjRec->osize);
    sprintf(szFlags,    "0x%08x", pObjRec->oflags);

    szFlagsExp[0] = '\0';
    flObj = pObjRec->oflags;
    for (i = 0; i < sizeof(aFlagDesc) / sizeof(aFlagDesc[0]); i++)
        if ((aFlagDesc[i].flFlag & flObj) == aFlagDesc[i].flFlag)
        {
            if (!fFirst)
                strcat(&szFlagsExp[0], " | ");
            else
                fFirst = FALSE;
            strcat(&szFlagsExp[0], aFlagDesc[i].pszDesc);
        }
}



/* $Id: kMemMapRecord.cpp,v 1.1 2000/06/19 20:08:42 stknut Exp $
 *
 * kMemMapRecord - Container record.
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
#include "kMemMapRecord.h"


/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
const FIELDINFO kMemMapRecord::aFieldInfo[] =
{
    {
        sizeof(FIELDINFO),
        CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
        CFA_STRING,
        (PVOID)"Base",
        offsetof(kMemMapRecord, pszBase),
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
        offsetof(kMemMapRecord, pszSize),
        NULL,
        NULL,
        0L
    }
    ,
    {
        sizeof(FIELDINFO),
        CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
        CFA_STRING,
        (PVOID)"hMod/hOB",
        offsetof(kMemMapRecord, pszhMod),
        NULL,
        NULL,
        0L
    }
    ,
    {
        sizeof(FIELDINFO),
        CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR,
        CFA_STRING,
        (PVOID)"Module/Memory name",
        offsetof(kMemMapRecord, pszModName),
        NULL,
        NULL,
        0L
    }
    ,
    {
        sizeof(FIELDINFO),
        CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
        CFA_STRING,
        (PVOID)"No.",
        offsetof(kMemMapRecord, pszObjNo),
        NULL,
        NULL,
        0L
    }
    ,
    {
        sizeof(FIELDINFO),
        CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
        CFA_STRING,
        (PVOID)"Flags/RefCount",
        offsetof(kMemMapRecord, pszFlags),
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
        offsetof(kMemMapRecord, pszFlagsExp),
        NULL,
        NULL,
        0L
    }
};

const unsigned  kMemMapRecord::cFieldInfo = 7;


/**
 * Init function.
 */
VOID    kMemMapRecord::init()
{
    kObjectRecord::init();
    ulBase      = ~0UL;
    pszhMod     = &szhMod[0];
    pszModName  = &szModName[0];
}


/**
 * Set the data of the record.
 * @param       pObjRec     Pointer to object record from QS.
 * @param       iObjNo      Object number.
 * @param       hmod        Modulehandle of the owner module. (-1 if N/A)
 * @param       pszModName  Pointer to modulename. (NULL if N/A)
 */
VOID    kMemMapRecord::set(qsLObjrec_t *pObjRec, int iObjNo, USHORT hMod, PCSZ pszModName)
{
    kObjectRecord::set(pObjRec, iObjNo);

    ulBase = pObjRec->oaddr;
    if (hMod != (USHORT)~0)
        sprintf(pszhMod, "0x%04x", hMod);
    else
        sprintf(pszhMod, "N/A");

    if (pszModName)
        strcpy(this->pszModName, pszModName);
    else
        this->pszModName[0] = '\0';
}


/**
 * Set the data of the record.
 * @param       pMemRec     Pointer to memory record from QS.
 */
VOID    kMemMapRecord::set(qsMrec_t *pMemRec)
{
    ulBase = (pMemRec->sel & ~0x7) << 13;

    this->iObjNo = iObjNo;
    sprintf(szObjNo,    "-", iObjNo);
    sprintf(szBase,     "0x%08x", ulBase);
    sprintf(szSize,     "0x%08x", -1);
    sprintf(szFlags,    "0x%04x", pMemRec->refcnt);

    szFlagsExp[0] = '\0';

    if (pMemRec->hmem != (USHORT)~0)
        sprintf(pszhMod, "0x%04x", pMemRec->hmem);
    else
        sprintf(pszhMod, "N/A");

    if (pMemRec->Memname != '\0')
        strcat(strcpy(this->pszModName, "\\SHAREMEM\\"), &pMemRec->Memname);
    else
        this->pszModName[0] = '\0';
}

/* $Id: kModuleRecord.cpp,v 1.1 2000/05/23 00:12:45 stknut Exp $
 *
 * kModuleRecord - Container record.
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
#include "kModuleRecord.h"


/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
const FIELDINFO kModuleRecord::aFieldInfo[] =
{
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
      CFA_STRING,
      (PVOID)"hMte",
      offsetof(kModuleRecord, pszhMte),
      NULL,
      NULL,
      0L
   }
   ,
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR,
      CFA_STRING,
      (PVOID)"Type",
      offsetof(kModuleRecord, pszType),
      NULL,
      NULL,
      0L
   }
   ,
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
      CFA_STRING,
      (PVOID)"#ImpMods",
      offsetof(kModuleRecord, pszcImpMods),
      NULL,
      NULL,
      0L
   }
   ,
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_RIGHT,
      CFA_STRING,
      (PVOID)"#Objects",
      offsetof(kModuleRecord, pszcObjects),
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
      offsetof(kModuleRecord, pszName),
      NULL,
      NULL,
      0L
   }
};

const unsigned  kModuleRecord::cFieldInfo = 5;


/**
 * Init function.
 */
VOID    kModuleRecord::init()
{
    kTaskMgrRecordBase::init();
    pszhMte = &szhMte[0];
    pszType = &szType[0];
    pszcImpMods = &szcImpMods[0];
    pszcObjects = &szcObjects[0];
    pszName = &szName[0];
}


/**
 * Set record information according to the passed in pProcData struct.
 * @param     pMteRec  MTE info from QS.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
VOID  kModuleRecord::set(qsLrec_s *pMteRec)
{
    /* Save hMte */
    hMte = pMteRec->hmte;

    /*
     * Set and format
     */
    sprintf(szhMte,     "%04x", pMteRec->hmte);
    if (pMteRec->fFlat)
        strcpy(szType, "32-bit (LX)");
    else
        strcpy(szType, "16-bit (NE)");
    sprintf(szcImpMods,  "%04x", pMteRec->ctImpMod);
    sprintf(szcObjects, "%04x", pMteRec->ctObj);
    if (pMteRec->pName != NULL)
        strcpy(szName, (char*)pMteRec->pName);
    else
        strcpy(szName, "<unknown>");
}


/**
 * Set the data of the record.
 * @param     hMte      Module handle.
 * @param     pszName   Module name.
 */
VOID    kModuleRecord::set(USHORT hMte)
{
    this->hMte = hMte;
    sprintf(szhMte,     "0x%04x", hMte);
    if (pszName != NULL)
        strcpy(szName, pszName);
    else
        strcpy(szName, "<unknown>");
    strcpy(szType    , "-");
    strcpy(szcImpMods, "-");
    strcpy(szcObjects, "-");
    strcpy(szName    , "-");
}



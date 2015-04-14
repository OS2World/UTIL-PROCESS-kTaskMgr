/* $Id: kMemMapContainer.cpp,v 1.1 2000/06/19 20:08:42 stknut Exp $
 *
 * kMemMapContainer (kTaskMgr) - generic memory map container.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */

/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
#define INCL_WIN
#define INCL_GPI
#define INCL_BASE


/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <os2.h>
#ifdef USE_KLIB
    #include <kAssert.h>
    #include <kLog.h>
    #include <kHeap.h>
#else
    #include <malloc.h>
#endif
#include <memory.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

#include "kBase.h"
#include "kError.h"
#include "kDlgBase.h"
#include "kMenuBase.h"
#include "kClickDlg.h"
#include "kContainer.h"

#include "kNotebookBase.h"
#include "kNotebookPageBase.h"

#include "kQuerySysState.h"
#include "kOS2Info.h"
#include "kTaskMgrRecordBase.h"
#include "kTaskMgrContainerBase.h"
#include "kDetailsBase.h"

#include "kObjectRecord.h"
#include "kMemMapRecord.h"
#include "kMemMapContainer.h"
/* #include "kMemMapDetails.h" */
#include "kTaskMgr.h"
#include "kTaskMgr_defs.h"


/**
 * Updates the content of the container - create memory map for a single process.
 * @returns   success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL   kMemMapContainer::insertPid()
{
    int             c,                  /* Count of SFNs in this FS record. */
                    i;                  /* Loop variable */
    kMemMapRecord * pCurCnrRec,         /* Current container record. */
                  * pCnrRec;            /* Pointer to first container record. */
    PPROCESSDATA    pProcData;          /* Pointer to process data. */

    ASSERT(usPid < 0xFFFF);

    /*
     * Remove all existing container records!
     */
    removeAllRecords();

    /*
     * Get process data
     */
    pProcData = QSGetProcessData(usPid);
    if (pProcData == NULL)
        return FALSE;

    /* not implemented */
    c = 0;
    pCnrRec = NULL;
    c           = c;
    pCnrRec     = pCnrRec;
    i           = i = 0;
    pCurCnrRec  = pCurCnrRec = NULL;

    #if 0
    /*
     * Insert the record at bottom of the container.
     */
    if (!insertAtTop(pCnrRec, c))
        return FALSE;
    #endif

    /* resort container */
    return enableSorting();
}


/**
 * Updates the content of the container - create system wide memory map.
 * @returns   success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL   kMemMapContainer::insertAll()
{
    qsLrec_t *      pMteData;
    qsLrec_t *      pMteFirst;
    qsMrec_t *      pMemData;
    qsMrec_t *      pMemFirst;
    qsMrec_t *      pShrMemFirst;
    int             cCount;
    kMemMapRecord  *pCurCnrRec, *pCnrRec;

    pMteFirst   = QSGetMteFirstData();
    pMemFirst   = QSGetMemFirstData();
    pShrMemFirst= QSGetShrMemFirstData();

    /*
     * Count the records.
     */
    for (pMteData = pMteFirst, cCount = 0; pMteData; pMteData = (qsLrec_t*)pMteData->pNextRec)
        cCount +=  pMteData->pObjInfo ? pMteData->ctObj : 0;
    for (pMemData = pMemFirst; pMemData; pMemData = (qsMrec_t*)pMemData->MemNextRec)
        cCount++;
    for (pMemData = pShrMemFirst; pMemData; pMemData = (qsMrec_t*)pMemData->MemNextRec)
        cCount++;

    /*
     * Allocate container records for all records.
     */
    pCurCnrRec = pCnrRec = (kMemMapRecord*)allocMiniRec(sizeof(kMemMapRecord), cCount);
    if (pCurCnrRec == NULL)
        return FALSE;               /* FIXME: complain about this! */


    /*
     * Loop thru the objects for each MTE and add them.
     */
    pMteData = pMteFirst;
    while (pMteData)
    {
        int c = (int)pMteData->ctObj;
        if (c > 0 && pMteData->pObjInfo != NULL)
        {
            /*
             * Loop thru the list of processes and update the container.
             */
            for (int i = 0; i < c; i++, pCurCnrRec = (kMemMapRecord*)pCurCnrRec->getNext())
            {
                /*
                 * Init and set the record.
                 */
                pCurCnrRec->init();
                pCurCnrRec->set(&pMteData->pObjInfo[i], i, pMteData->hmte, (PCSZ)pMteData->pName);
            }
        }

        /*
         * Next.
         */
        pMteData = (qsLrec_t*)pMteData->pNextRec;
    }


    /*
     * Loop thru all shared memory.
     */
    pMemData = pShrMemFirst;
    while (pMemData)
    {
        /*
         * Init and set the record.
         */
        pCurCnrRec->init();
        pCurCnrRec->set(pMemData);

        /*
         * Next.
         */
        pMemData = (qsMrec_t*)pMemData->MemNextRec;
        pCurCnrRec = (kMemMapRecord*)pCurCnrRec->getNext();
    }



    /*
     * Loop thru all memory objects listed.
     */
    pMemData = pMemFirst;
    while (pMemData)
    {
        /*
         * Init and set the record.
         */
        pCurCnrRec->init();
        pCurCnrRec->set(pMemData);

        /*
         * Next.
         */
        pMemData = (qsMrec_t*)pMemData->MemNextRec;
        pCurCnrRec = (kMemMapRecord*)pCurCnrRec->getNext();
    }


    /*
     * Insert the record at bottom of the container.
     */
    return insertAtBottom(pCnrRec, cCount) && enableSorting();
}


/**
 * Command events - none yet.
 * @param     usCmd     Control id which send/posted the message.
 * @param     usSource  Source id.
 * @param     fPointer  Mouse pointer flag.
 */
VOID  kMemMapContainer::command(USHORT usCmd, USHORT usSource, BOOL fPointer)
{
    kMemMapRecord *pCurRecord;

    switch (usCmd)
    {
        case IDM_CNR_MMAP_DETAILS:
            pCurRecord = (kMemMapRecord *)getCurRecord();
            #if 0 /* not implemented */
            if (pCurRecord != NULL)
                kMemMapDetails::invoke(pCurRecord->getSFN(), hwndCnr);
            #endif
            pCurRecord = pCurRecord;
            break;

        default:
        kTaskMgrContainerBase::command(usCmd, usSource, fPointer);
    }
    usSource = usSource;
    fPointer = fPointer;
}






/**
 * Constructor.
 * @param     hwndDlg   Handle to dialog window.
 * @param     ulCnrId   ID of the container dialog item in hwndDlg.
 * @param     usPid      = 0xFFFF: Memory map for entire system.
 *                      != 0:      Memory map for the given process - not implemented yet.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
kMemMapContainer::kMemMapContainer(HWND hwndDlg, ULONG ulCnrId, USHORT usPid /* = 0xFFFF*/) throw(kError)
    : kTaskMgrContainerBase(hwndDlg, ulCnrId,
                            "Memory \"Map\"",
                            kMemMapRecord::cFieldInfo,
                            (PFIELDINFO)&kMemMapRecord::aFieldInfo[0],
                            IDM_CNR_MEMMAP_ALL,
                            IDM_CNR_MEMMAP,
                            IDM_CNR_MMAP_DETAILS,
                            0),
    usPid(usPid)
{
    TRACELOG_CONSTRUCT("start");
    TRACELOG_CONSTRUCT("end");
}


/**
 * Destructor.
 */
kMemMapContainer::~kMemMapContainer()
{
    TRACELOG_DESTRUCT("start");
    TRACELOG_DESTRUCT("end");
}


/**
 * Updates the contents of the container.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
VOID  kMemMapContainer::update()
{
    TRACELOG_UPDATE("start");
    if (usPid < 0xFFFF)
        insertPid();
    else
        insertAll();
    TRACELOG_UPDATE("end");
}



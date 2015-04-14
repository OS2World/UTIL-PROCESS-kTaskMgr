/* $Id: kSFNContainer.cpp,v 1.4 2000/06/19 21:14:16 stknut Exp $
 *
 * kSFNContainer (kTaskMgr) - generic SystemFileNumber (SFN) container.
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

#include "kSFNRecord.h"
#include "kSFNContainer.h"
#include "kSFNDetails.h"
#include "kTaskMgr.h"
#include "kTaskMgr_defs.h"


/**
 * Updates the content of the container - inserts SFN a single process.
 * @returns   success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL   kSFNContainer::insertSFNsPid()
{
    int             c,                  /* Count of SFNs in this FS record. */
                    i;                  /* Loop variable */
    kSFNRecord    * pCurCnrRec,         /* Current container record. */
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


    /*
     * Allocate container records for all the objects.
     *
     * BUGUBUG: < fp13
     *      pFSRec isn't set. Calculate it using
     *
     * FEATURE/BUGBUG: cFH isn't correct - it don't match with the number of
     *      entries in the pFSRec array. (qsProc_t)
     *
     *      This is verified! There is a "bug" in the _qsProcFS worker (this
     *      is the function which generates the SFT table pointed to by pFSRec).
     *      cFH is set to JFN_Length by _qsReadPtda. While _qsProcFS skips the
     *      entries containing 0xFFFF (unused handle placeholder).
     *      This way the cFH is really the maximum number of handles and the
     *      size of the pFSRec table has to be determined by checking pointers
     *      to the next structure. Thread records are by specifications
     *      following the FS records, pointer to these is used to do this.
     *
     *      Note. JFN is not equal to the index into the SFN table due to this bug.
     */
    if (pProcData->pProcRec->pFSRec == NULL
        && pProcData->pProcRec->pShrMemRec != NULL)
        pProcData->pProcRec->pFSRec = pProcData->pProcRec->pShrMemRec + pProcData->pProcRec->cShrMem;
    if ((char*)&pProcData->pProcRec->pFSRec[pProcData->pProcRec->cFH] > (char*)pProcData->pProcRec->pThrdRec)
        c = (int)((char*)pProcData->pProcRec->pThrdRec - (char*)pProcData->pProcRec->pFSRec) / sizeof(pProcData->pProcRec->pFSRec[0]);
    else
        c = (int)pProcData->pProcRec->cFH;
    pCurCnrRec = pCnrRec = (kSFNRecord*)allocMiniRec(sizeof(kSFNRecord), c);
    if (pCurCnrRec == NULL)
        return FALSE;               /* FIXME: complain about this! */

    /*
     * Loop thru all SFNs for this MFT (Main(?)FileTable).
     */
    for (i = 0; i < c; i++, pCurCnrRec = (kSFNRecord*)pCurCnrRec->getNext())
    {
        qsSft_t         SFTDummy;       /* Dummy SFT record. (used when FS data isn't found) */
        struct _FSDummy
        {
            qsFrec_t    rec;
            char        name[64];
        }           FSDummy;            /* Dummy FS record with filename. (used when FS data isn't found) */
        qsFrec_t *      pFSData;        /* Pointer to FS data record. */
        qsSft_t *       pSFTData;       /* Pointer to SFT data record. */


        /*
         * Get FS and SFT data.
         */
        pFSData = QSGetSfnData(pProcData->pProcRec->pFSRec[i]);
        if (pFSData != NULL)
        {
            int j;
            /* find the correct SFT record in this FS record */
            for (j = 0, pSFTData = pFSData->pSft; j < pFSData->ctSft; j++)
                if (pSFTData->sfn == pProcData->pProcRec->pFSRec[i])
                    break;
            ASSERT(pSFTData != NULL);
        }
        else
        {   /* create dummy FS and SFT records */
            memset(&SFTDummy, 0, sizeof(SFTDummy));
            SFTDummy.sfn = pProcData->pProcRec->pFSRec[i];
            FSDummy.rec.RecType = QS_FILESYS;
            FSDummy.rec.pNextRec = NULL;
            FSDummy.rec.ctSft = 1;
            FSDummy.rec.pSft = &SFTDummy;
            if (SFTDummy.sfn == 0xffff)
                strcpy(FSDummy.name, "<unused>");
            else if (SFTDummy.refcnt == 0)
                strcpy(FSDummy.name, "<UNC or pipe?>");
            else
                strcpy(FSDummy.name, "<Not found due to BUGBUG in DosQuerySysState / _qsReadMFT ???>");
            pFSData = &FSDummy.rec;
            pSFTData = &SFTDummy;
        }


        /*
         * Init and set the record.
         */
        pCurCnrRec->init();
        pCurCnrRec->set(pSFTData, (char*)pFSData + sizeof(*pFSData), (USHORT)i);
    }


    /*
     * Insert the record at bottom of the container.
     */
    if (!insertAtTop(pCnrRec, c))
        return FALSE;

    /* resort container */
    return enableSorting();
}


/**
 * Updates the content of the container - inserts SFN for the entire system
 * @returns   success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL   kSFNContainer::insertSFNsAll()
{
    qsFrec_t *      pFSData;            /* Pointer to current FS data record. */
    int             cRecords;           /* Count of records to allocate / allocated. */
    kSFNRecord    * pCurCnrRec,
                  * pCnrRec;

    /*
     * Remove all existing container records!
     */
    removeAllRecords();

    /*
     * Get data for this MTE and get the count of files.
     */
    cRecords = 0;
    pFSData = QSGetFSFirstData();
    while (pFSData != NULL && pFSData->RecType == QS_FILESYS)
    {
        /*
         * BUGBUG! - ctSft isn't correct, or rather Sft records are missing!!! (qsFrec_t)
         *      The number of qsSft_t records is always 1, no matter what the
         *      ctSft member says!
         *      This is verified! The defect is located in _qsReadMFT. When
         *      writing the qsSft_t records the record pointer is always
         *      initialized to _pCurCache in each iteration. This is probably
         *      correct, but _pCurCache is _NOT_ changed during the iteration.
         *      With the excelent result that the next Sft record overwrites
         *      the previous one.
         *
         *      Solution: To move the call to _qsUpdatePtrs into the loopbody,
         *                after the Sft record is written.
         *                This way _pCurCache will be updated at end of each
         *                iteration.
         *      This is a problem on Aurora, Warp 4 and Warp 3.
         */
        if (pFSData->ctSft > 1 && &pFSData->pSft[pFSData->ctSft] > pFSData->pNextRec && pFSData->pNextRec != NULL)
        {
            fprintf(stderr, "- start - pFSData->ctSft correction - end -\nold pFSData->ctSft = %d\n", pFSData->ctSft);
            pFSData->ctSft = (((char*)pFSData->pNextRec - (char*)pFSData->pSft)) / SIZEOFQSSFT_T;
            fprintf(stderr, "new pFSData->ctSft = %d\n- end   - pFSData->ctSft correction - end -\n", pFSData->ctSft);
        }

        /* Add; Next record */
        cRecords += pFSData->ctSft;
        pFSData = (qsFrec_t*)pFSData->pNextRec;
    }


    /*
     * Allocate container records for all the objects.
     */
    pCurCnrRec = pCnrRec = (kSFNRecord*)allocMiniRec(sizeof(kSFNRecord), cRecords);
    if (pCurCnrRec == NULL)
        return FALSE;               /* FIXME: complain about this! */


    /*
     * Loop thru all filesystem records insering all system filenumbers.
     */
    pFSData = QSGetFSFirstData();
    while (pFSData != NULL && pFSData->RecType == QS_FILESYS)
    {
        int         i;                  /* Inner SFN loop variable */
        qsSft_t *   pSftRec;            /* Pointer to current Sft data */


        /*
         * Loop thru all SFNs for this MFT (Main(?)FileTable).
         */
        for (i = 0, pSftRec = pFSData->pSft;
             i < pFSData->ctSft;
             i++, pCurCnrRec = (kSFNRecord*)pCurCnrRec->getNext(), pSftRec = (qsSft_t*)((char*)pSftRec + SIZEOFQSSFT_T)
             )
        {
            /*
             * Init and set the record.
             */
            pCurCnrRec->init();
            pCurCnrRec->set(pSftRec, (char*)pFSData + sizeof(*pFSData));
        }

        /*
         * Next filesystem record.
         */
        pFSData = (qsFrec_t*)pFSData->pNextRec;
    }

    /*
     * Insert the records.
     */
    if (!insertAtTop(pCnrRec, cRecords))
        return FALSE;

    /* resort container */
    return enableSorting();
}


/**
 * Command events - none yet.
 * @param     usCmd     Control id which send/posted the message.
 * @param     usSource  Source id.
 * @param     fPointer  Mouse pointer flag.
 */
VOID  kSFNContainer::command(USHORT usCmd, USHORT usSource, BOOL fPointer)
{
    kSFNRecord *pCurRecord;

    switch (usCmd)
    {
        case IDM_CNR_SFN_DETAILS:
            pCurRecord = (kSFNRecord *)getCurRecord();
            if (pCurRecord != NULL)
                kSFNDetails::invoke(pCurRecord->getSFN(), hwndCnr);
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
 * @param     usPid      = 0xFFFF: View all SFNs.
 *                      != 0:      View only SFNs for the given process.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
kSFNContainer::kSFNContainer(HWND hwndDlg, ULONG ulCnrId, USHORT usPid /* = 0xFFFF*/) throw(kError)
    : kTaskMgrContainerBase(hwndDlg, ulCnrId,
                            "System File Numbers (SFN) Overview",
                            kSFNRecord::cFieldInfo -           (usPid < 0xFFFF && ulBuildLevel >= 14040 ? 0 : 1), // see JFN comment below
                            (PFIELDINFO)&kSFNRecord::aFieldInfo[usPid < 0xFFFF && ulBuildLevel >= 14040 ? 0 : 1],
                            IDM_CNR_SFN_ALL,
                            IDM_CNR_SFN,
                            IDM_CNR_SFN_DETAILS,
                            0),
    usPid(usPid)
{
    TRACELOG_CONSTRUCT("start");
    TRACELOG_CONSTRUCT("end");
}


/**
 * Destructor.
 */
kSFNContainer::~kSFNContainer()
{
    TRACELOG_DESTRUCT("start");
    TRACELOG_DESTRUCT("end");
}


/**
 * Updates the contents of the container.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
VOID  kSFNContainer::update()
{
    TRACELOG_UPDATE("start");
    if (usPid < 0xFFFF)
        insertSFNsPid();
    else
        insertSFNsAll();
    TRACELOG_UPDATE("end");
}



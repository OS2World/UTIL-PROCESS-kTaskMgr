/* $Id: kProcessContainer.cpp,v 1.4 2000/06/19 22:24:24 stknut Exp $
 *
 * kProcessContainer - General process container.
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
//#include "kAbout.h"
#include "kNotebookBase.h"
#include "kNotebookPageBase.h"

#include "kQuerySysState.h"
#include "kDetailsBase.h"
#include "kTaskMgrRecordBase.h"
#include "kTaskMgrContainerBase.h"

#include "kThreadRecord.h"
#include "kThreadContainer.h"
#include "kProcessRecord.h"
#include "kProcessContainer.h"
#include "kProcessDetails.h"
#include "kProcessPriority.h"
#include "kTaskMgr.h"
#include "kTaskMgr_defs.h"




/**
 * Updates the container. This update routine is for the container with all processes. (usPid = 0xFFFF)
 * @returns   Success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL    kProcessContainer::kUpdateAllProcesses()
{
    BOOL            fRet;
    PPROCESSDATA    pCur;
    kProcessRecord *apInvalidRecs[256];
    unsigned        cpInvalidRecs = 0;
    kProcessRecord *apDeadRecs[64];
    unsigned        cpDeadRecs = 0;

    /*
     * Update status information.
     */
    if (!QSUpdateStateData())
        return FALSE;

    /*
     * Loop thru the list of processes and update the container.
     */
    pCur = QSGetFirstProcessDataNode();
    disableUpdate();
    while (pCur != NULL)
    {
        kProcessRecord *pRec;
        BOOL            fNew = FALSE;
        PPROCESSDATA    pToFree = NULL;

        /*
         * Find the record. If not found, create a new one.
         */
        pRec = (kProcessRecord *)pCur->pvRecordCore;
        if (pRec == NULL)
        {   /* new records */
            pCur->pvRecordCore = pRec = (kProcessRecord*)allocMiniRec(sizeof(kProcessRecord));
            if (pRec == NULL)
                break;
            pRec->init();
            fNew = TRUE;
        }

        /*
         * Update data?
         */
        if (pCur->fDirty)
        {
            /*
             * Update record data.
             */
            pRec->set(pCur);

            /*
             * Update container content.
             *   If this is a new process we'll have to add it
             *   If this is an existing process we'll have to refresh it.
             */
            if (fNew)
                insertAtBottom(pRec, 1);
            else
            {
                if (cpInvalidRecs >= 256)
                {
                    fRet = invalidateRecords(CMA_TEXTCHANGED,
                                             cpInvalidRecs,
                                             (kCnrMiniRecord **)&apInvalidRecs);
                    ASSERT(fRet);
                    cpInvalidRecs = 0;
                }

                apInvalidRecs[cpInvalidRecs++] = pRec;
            }
        }

        /*
         * Dead? Remove it!
         */
        if (pCur->fDead)
        {
            if (cpDeadRecs >= 64)
            {
                fRet = removeRecords((PPVOID)&apDeadRecs[0], cpDeadRecs);
                ASSERT(fRet);
                cpDeadRecs = 0;
            }
            apDeadRecs[cpDeadRecs++] = pRec;
            pToFree = pCur;
        }


        /* next */
        pCur = pCur->pNext;
        if (pToFree != NULL)
            QSRemoveProcessData(pToFree);
    }

    /*
     * Invalidate records
     */
    if (cpInvalidRecs != 0)
    {
        fRet = invalidateRecords(CMA_TEXTCHANGED,
                                 cpInvalidRecs,
                                 (kCnrMiniRecord **)&apInvalidRecs);
        ASSERT(fRet);
    }

    /*
     * Dead records
     */
    if (cpDeadRecs != 0)
    {
        fRet = removeRecords((PPVOID)&apDeadRecs[0], cpDeadRecs);
        ASSERT(fRet);
    }
    enableUpdate();

    return pCur == NULL;
}



/**
 * Updates the container with children if usPid.
 * @returns   Success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL    kProcessContainer::kUpdateChildProcesses()
{
    PPROCESSDATA        pCur;
    kProcessRecord *    pCnrRecs,
                   *    pCurCnrRec;
    int                 cRecords;

    /*
     * Update status information.
     */
    if (!QSUpdateStateData())
        return FALSE;

    /*
     * Remove all records in container.
     */
    this->removeAllRecords();

    /*
     * Loop thru the list of processes and update the container.
     */
    cRecords = 0;
    pCnrRecs = pCurCnrRec = NULL;
    pCur = QSGetFirstProcessDataNode();
    while (pCur != NULL)
    {
        if (pCur->pProcRec->ppid == usPid)
        {
            /*
             * Allocate and set new process record
             */
            if (pCnrRecs == NULL)
                pCnrRecs = pCurCnrRec = (kProcessRecord*)allocMiniRec(sizeof(kProcessRecord));
            else
                pCurCnrRec = (kProcessRecord*)pCurCnrRec->setNext(allocMiniRec(sizeof(kProcessRecord)));
            if (pCurCnrRec == NULL)
                break;

            pCurCnrRec->init();
            pCurCnrRec->set(pCur);
            cRecords++;
        }

        /* next */
        pCur = pCur->pNext;
    }


    /*
     * Insert records (if any) and return.
     */
    return cRecords > 0 ? insertAtBottom(pCnrRecs, cRecords) : TRUE;
}



/**
 * Updates the container with processes accessing usSFN.
 * @returns   Success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL    kProcessContainer::kUpdateProcessesUsingSFN()
{
    PPROCESSDATA        pCur;
    kProcessRecord *    pCnrRecs,
                   *    pCurCnrRec;
    int                 cRecords;

    /*
     * Update status information.
     */
    if (!QSUpdateStateData())
        return FALSE;

    /*
     * Remove all records in container.
     */
    this->removeAllRecords();

    /*
     * Loop thru the list of processes and update the container.
     */
    cRecords = 0;
    pCnrRecs = pCurCnrRec = NULL;
    pCur = QSGetFirstProcessDataNode();
    while (pCur != NULL)
    {
        int         c;
        int         iSFN;
        PUSHORT     pausSFN;

        /*
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
        if (pCur->pProcRec->pFSRec == NULL
            && pCur->pProcRec->pShrMemRec != NULL)
            pCur->pProcRec->pFSRec = pCur->pProcRec->pShrMemRec + pCur->pProcRec->cShrMem;
        if ((char*)&pCur->pProcRec->pFSRec[pCur->pProcRec->cFH] > (char*)pCur->pProcRec->pThrdRec)
            c = (int)((char*)pCur->pProcRec->pThrdRec - (char*)pCur->pProcRec->pFSRec) / sizeof(pCur->pProcRec->pFSRec[0]);
        else
            c = (int)pCur->pProcRec->cFH;

        pausSFN = pCur->pProcRec->pFSRec;
        for (iSFN = 0; iSFN < c; iSFN++)
        {
            if (pausSFN[iSFN] == usSFN)
            {
                /*
                 * Allocate and set new process record
                 */
                if (pCnrRecs == NULL)
                    pCnrRecs = pCurCnrRec = (kProcessRecord*)allocMiniRec(sizeof(kProcessRecord));
                else
                    pCurCnrRec = (kProcessRecord*)pCurCnrRec->setNext(allocMiniRec(sizeof(kProcessRecord)));
                if (pCurCnrRec == NULL)
                    break;

                pCurCnrRec->init();
                pCurCnrRec->set(pCur);
                cRecords++;
                break; /* only one entry per process! */
            }
        }

        /* next */
        pCur = pCur->pNext;
    }


    /*
     * Insert records (if any) and return.
     */
    return cRecords > 0 ? insertAtBottom(pCnrRecs, cRecords) : TRUE;
}




/**
 * Command events.
 * @param     usCmd     Control id which send/posted the message.
 * @param     usSource  Source id.
 * @param     fPointer  Mouse pointer flag.
 * @remark    dismisses the dialog if DID_OK or DID_CANCEL.
 */
VOID  kProcessContainer::command(USHORT usCmd, USHORT usSource, BOOL fPointer)
{
    ULONG   ulRet;                      /* Return value from WinMessageBox usually. */
    APIRET  rc;                         /* Return code from Dos API. */
    char    szBuffer[1024];             /* String buffer used for formating messages. */
    kProcessRecord *    pCurRecord = (kProcessRecord *)getCurRecord();

    switch (usCmd)
    {
        case IDM_CNR_PROC_DETAILS:
            if (pCurRecord != NULL)
                kProcessDetails::invoke(pCurRecord->getPid(), hwndCnr);
            break;


        case IDM_CNR_PROC_KILL:
            if (pCurRecord != NULL)
            {
                sprintf(szBuffer, "Are you sure you like to kill the process '%s'(pid %#x)?",
                        pCurRecord->getName(),
                        pCurRecord->getPid());
                ulRet = WinMessageBox(HWND_DESKTOP,
                                      hwndCnr,
                                      szBuffer,
                                      "kTaskMgr - Sure?",
                                      0,
                                      MB_YESNO|MB_CUAWARNING);
                if (ulRet == MBID_YES)
                {
                    rc = DosKillProcess(DKP_PROCESS, pCurRecord->getPid());
                    sprintf(szBuffer, "rc=%d", rc);
                    WinMessageBox(HWND_DESKTOP,
                                  hwndCnr,
                                  szBuffer,
                                  "kTaskMgr - debug",
                                  0,
                                  MB_OK);
                }
            }
            break;

        case IDM_CNR_PROC_KILLXF86:
            if (pCurRecord != NULL)
            {
                HFILE   hFastIO;
                ULONG   ulAction;

                rc = DosOpen("/dev/fastio$",
                             &hFastIO,
                             &ulAction,
                             (ULONG)0,
                             FILE_SYSTEM,
                             FILE_OPEN,
                             OPEN_SHARE_DENYNONE | OPEN_FLAGS_NOINHERIT | OPEN_ACCESS_READONLY,
                             NULL);
                if (rc == NO_ERROR)
                {
                    sprintf(szBuffer, "Are you sure you like to kill the process '%s'(pid %#x)?",
                            pCurRecord->getName(),
                            pCurRecord->getPid()
                            );
                    ulRet = WinMessageBox(HWND_DESKTOP,
                                          hwndCnr,
                                          szBuffer,
                                          "kTaskMgr- Sure?",
                                          0,
                                          MB_YESNO|MB_CUAWARNING);
                    if (ulRet == MBID_YES)
                    {
                        ULONG   ulLen;
                        USHORT  usPid = pCurRecord->getPid();
                        rc = DosDevIOCtl(hFastIO,
                                         (ULONG)0x76,
                                         (ULONG)0x65,
                                         &usPid, sizeof(usPid), &ulLen,
                                         NULL, 0, NULL);
                    }
                    DosClose(hFastIO);
                }
                else
                {
                    sprintf(szBuffer,
                            "Failed to open fastio$, rc = %d.\r"
                            "XF86SUP.SYS is probably not installed.",
                            rc);
                    WinMessageBox(HWND_DESKTOP,
                                  hwndCnr,
                                  szBuffer,
                                  kTaskMgr::pszErrorTitle,
                                  0,
                                  MB_CUACRITICAL | MB_OK);
                }
            }
            break;

        case IDM_CNR_PROC_PRIORITY:
            if (pCurRecord != NULL)
            {
                try
                {
                    kProcessPriority *pDlg = new kProcessPriority(pCurRecord->getPid(), hwndCnr);
                    pDlg->showModal();
                }
                catch(kError err)
                {
                    err.logError();
                    err.showError(kTaskMgr::pszErrorTitle);
                }
            }
            break;

        default:
            kTaskMgrContainerBase::command(usCmd, usSource, fPointer);
    }

    usSource = usSource;
    fPointer = fPointer;
}


/**
 * Constructor.
 * @returns
 * @param     hwndDlg   Handle to dialog window.
 * @param     ulCnrId   ID of the container dialog item in hwndDlg.
 * @param     usPid     usPid = 0xFFFF:  View all processes.
 *                      usPid != 0:      View all children of the given process id.
 * @param     usSFN     usSFN = 0xFFFF:  ignored - usPid is used.
 *                      usSFN != 0xFFFF: View all processes using this SFN. usPId is ignored.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
kProcessContainer::kProcessContainer(HWND hwndDlg, ULONG ulCnrId, USHORT usPid/* = 0xFFFF*/, USHORT usSFN) throw(kError)
    : kTaskMgrContainerBase(hwndDlg, ulCnrId,
                            usSFN == 0xFFFF ?
                                usPid == 0xFFFF ? "Process Overview"
                                                : "Child Processes"
                                            : "Processes Refering this SFN"
                            ,
                            kProcessRecord::cFieldInfo,
                            (PFIELDINFO)&kProcessRecord::aFieldInfo[0],
                            IDM_CNR_PROCESS_ALL,
                            IDM_CNR_PROCESS,
                            IDM_CNR_PROC_DETAILS,
                            0),
    usPid(usPid), usSFN(usSFN)
{
    TRACELOG_CONSTRUCT("start");
    TRACELOG_CONSTRUCT("end");
}


/**
 * Destructor.
 */
kProcessContainer::~kProcessContainer()
{
    TRACELOG_DESTRUCT("start");
    TRACELOG_DESTRUCT("end");
}




/**
 * Updates the contents of the container.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
VOID  kProcessContainer::update()
{
    TRACELOG_UPDATE("start");
    /*
     * Insert records
     */
    if (usSFN == 0xFFFF)
    {
        if (usPid == 0xFFFF)
            kUpdateAllProcesses();
        else
            kUpdateChildProcesses();
    }
    else
        kUpdateProcessesUsingSFN();
    TRACELOG_UPDATE("end");
}


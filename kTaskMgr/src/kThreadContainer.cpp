/* $Id: kThreadContainer.cpp,v 1.1 2000/05/23 00:12:46 stknut Exp $
 *
 * kThreadContainer - generic thread container.
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

#include "kQuerySysState.h"
#include "kTaskMgrRecordBase.h"
#include "kTaskMgrContainerBase.h"

#include "kThreadRecord.h"
#include "kThreadContainer.h"
#include "kTaskMgr.h"
#include "kTaskMgr_defs.h"


/**
 * Updates the content of the container.
 * @returns   success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL   kThreadContainer::insertThreadsPid()
{
    PPROCESSDATA    pProcData;
    int             c;

    /*
     * Delete all records in the container!
     */
    removeAllRecords();

    /*
     * Get process and thread data.
     */
    pProcData = QSGetProcessData(usPid);
    if (pProcData == NULL)
        return FALSE;

    /*
     * Process thread information.
     */
    c = pProcData->pProcRec->cTCB;
    if (c > 0 && pProcData->pProcRec->pThrdRec != NULL)
    {
        kThreadRecord  *    pCurCnrRec, *pCnrRec;
        qsTrec_t *          pThread;
        int                 i;

        /*
         * Allocate records.
         */
        pCurCnrRec = pCnrRec = (kThreadRecord*)allocMiniRec(sizeof(kThreadRecord), c);
        if (pCurCnrRec == NULL)
            return FALSE;

        /*
         * Loop thru the thread data and fill the records.
         */
        pThread = pProcData->pProcRec->pThrdRec;
        for (i = 0; i < c; i++, pCurCnrRec = (kThreadRecord*)pCurCnrRec->getNext(), pThread++)
        {
            //assert(pCurCnrRec != NULL);
            pCurCnrRec->init();
            pCurCnrRec->set(pThread);
        }

        /*
         * Insert the records.
         */
        return insertAtBottom(pCnrRec, c);

    }

    return TRUE;
}



/**
 * Updates the content of the container. ALL threads in the system!
 * @returns   success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL   kThreadContainer::insertThreadsAll()
{
    PPROCESSDATA        pProcData;
    int                 c;
    kThreadRecord     * pCurCnrRec,
                      * pCnrRec;
    qsTrec_t *          pThread;
    int                 i;

    /*
     * Delete all records in the container!
     */
    removeAllRecords();

    /*
     * Get process and thread data.
     * And count the number of threads we'll need to allocate records for.
     */
    pProcData = QSGetFirstProcessDataNode();
    if (pProcData == NULL)
        return FALSE;
    c = 0;
    while (pProcData != NULL)
    {
        c += pProcData->pProcRec->cTCB;
        pProcData = pProcData->pNext;
    }

    /*
     * Allocate records.
     */
    pCurCnrRec = pCnrRec = (kThreadRecord*)allocMiniRec(sizeof(kThreadRecord), c);
    if (pCurCnrRec == NULL)
        return FALSE;

    /*
     * Get process and thread data.
     * And start fill in the container records.
     */
    pProcData = QSGetFirstProcessDataNode();
    if (pProcData == NULL)
        return FALSE;

    while (pProcData != NULL)
    {
        /*
         * Loop thru the thread data and fill the records.
         */
        pThread = pProcData->pProcRec->pThrdRec;
        if (pThread != NULL)
        {
            for (i = 0; i < pProcData->pProcRec->cTCB; i++, pCurCnrRec = (kThreadRecord*)pCurCnrRec->getNext(), pThread++)
            {
                //assert(pCurCnrRec != NULL);
                pCurCnrRec->init();
                pCurCnrRec->set(pThread, pProcData);
            }
        }
        else
            for (i = 0; i < pProcData->pProcRec->cTCB; i++, pCurCnrRec = (kThreadRecord*)pCurCnrRec->getNext())
                pCurCnrRec->init();

        /* pNext Process */
        pProcData = pProcData->pNext;
    }

    /*
     * Insert the records.
     */
    return insertAtBottom(pCnrRec, c);
}




/**
 * Command events.
 * @param     usCmd     Control id which send/posted the message.
 * @param     usSource  Source id.
 * @param     fPointer  Mouse pointer flag.
 */
VOID  kThreadContainer::command(USHORT usCmd, USHORT usSource, BOOL fPointer)
{
    kThreadRecord * pCurRecord = (kThreadRecord *)getCurRecord();

    switch (usCmd)
    {
        case IDM_CNR_TRD_DETAILS:
            if (pCurRecord)
            {
                try
                {
                    //new kThreadDetails(pCurRecord->getPid(), hwndCnr)->show();
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
 * @param     usPid     Process Indicator for the process we're to get thread information for.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
kThreadContainer::kThreadContainer(HWND hwndDlg, ULONG ulCnrId, USHORT usPid) throw(kError)
    : kTaskMgrContainerBase(hwndDlg, ulCnrId,
                            "Threads Overview",
                            usPid < 0xffff ?  kThreadRecord::cFieldInfo    :  kThreadRecord::cFieldInfoGlobal,
                            (PFIELDINFO)(usPid < 0xffff ? &kThreadRecord::aFieldInfo[0] : &kThreadRecord::aFieldInfoGlobal[0]),
                            IDM_CNR_THREAD_ALL,
                            IDM_CNR_THREAD,
                            0xffff,
                            0),
    usPid(usPid)
{
    TRACELOG_CONSTRUCT("start");
    TRACELOG_CONSTRUCT("end");
}


/**
 * Destructor.
 */
kThreadContainer::~kThreadContainer()
{
    TRACELOG_DESTRUCT("start");
    TRACELOG_DESTRUCT("end");
}


/**
 * Updates the contents of the container.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
VOID  kThreadContainer::update()
{
    TRACELOG_UPDATE("start");
    if (usPid < 0xffff)
        insertThreadsPid();
    else
        insertThreadsAll();
    TRACELOG_UPDATE("end");
}


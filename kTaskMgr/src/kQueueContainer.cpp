/* $Id: kQueueContainer.cpp,v 1.2 2000/05/23 00:51:26 stknut Exp $
 *
 * kQueueContainer (kTaskMgr) - generic queue container.
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
#include <stdlib.h>

#include "kBase.h"
#include "kError.h"
#include "kDlgBase.h"
#include "kMenuBase.h"
#include "kClickDlg.h"
#include "kContainer.h"
#include "kNotebookBase.h"
#include "kNotebookPageBase.h"

#include "kQuerySysState.h"
#include "kQueueHack.h"
#include "kDetailsBase.h"
#include "kTaskMgrRecordBase.h"
#include "kTaskMgrContainerBase.h"

#include "kQueueRecord.h"
#include "kQueueContainer.h"
#include "kProcessDetails.h"
#include "kTaskMgr.h"
#include "kTaskMgr_defs.h"


/**
 * Updates the content of the container - inserts queue info for a single process.
 * @returns   success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL   kQueueContainer::insertQueuePid()
{
    PMYQUEENTRY     pQueEntry;          /* Pointer to current queue data record. */
    int             cRecords;           /* Count of records to allocate / allocated. */
    kQueueRecord  * pCurCnrRec,
                  * pCnrRec;
    BOOL            fRc;                /* Return code. */

    /*
     * Remove all existing container records!
     */
    removeAllRecords();

    /*
     * Get data for this MTE and get the count of files.
     */
    cRecords = 0;
    pQueEntry = QueGetFirstQueueData();
    while (pQueEntry != NULL)
    {
        /* Add; Next record */
        if (pQueEntry->que.usOwnPid == usPid)
            cRecords++;
        pQueEntry = pQueEntry->pNext;
    }

    /*
     * If not queues for this process stop processing.
     */
    if (cRecords == 0)
        return TRUE;

    /*
     * Allocate container records for all the objects.
     */
    pCurCnrRec = pCnrRec = (kQueueRecord*)allocMiniRec(sizeof(kQueueRecord), cRecords);
    if (pCurCnrRec == NULL)
        return FALSE;               /* FIXME: complain about this! */


    /*
     * Loop thru all queue data records and create container records for each.
     */
    pQueEntry = QueGetFirstQueueData();
    while (pQueEntry != NULL)
    {
        /*
         * Init and set the record.
         */
        if (pQueEntry->que.usOwnPid == usPid)
        {
            pCurCnrRec->init();
            pCurCnrRec->set(pQueEntry);

            /* Advance to the next container record */
            pCurCnrRec = (kQueueRecord *) pCurCnrRec->getNext();
        }

        /*
         * Next queue data record.
         */
        pQueEntry = pQueEntry->pNext;
    }

    /*
     * Insert the conatiner records.
     */
    if (!insertAtTop(pCnrRec, cRecords))
        return FALSE;

    /* resort container */
    fRc = enableSorting();

    /*
     * Update the header with new queue count and
     * memory statistics.
     */
    if (cRecords != this->cQueues)
    {
        sprintf(pszTitle, "Queues Overview - %d queues", cRecords);
        if (setTitle(pszTitle))
            this->cQueues = cRecords;
        else
            fRc = FALSE;
    }

    return fRc;
}


/**
 * Updates the content of the container - inserts queue info for the entire system
 * @returns   success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL   kQueueContainer::insertQueueAll()
{
    PMYQUEENTRY     pQueEntry;          /* Pointer to current queue data record. */
    int             cRecords;           /* Count of records to allocate / allocated. */
    kQueueRecord  * pCurCnrRec,
                  * pCnrRec;
    BOOL            fRc;                /* Return code. */
    ULONG           cb;

    /*
     * Remove all existing container records!
     */
    removeAllRecords();

    /*
     * Get data for this MTE and get the count of files.
     */
    cRecords = 0;
    pQueEntry = QueGetFirstQueueData();
    while (pQueEntry != NULL)
    {
        /* Add; Next record */
        cRecords++;
        pQueEntry = pQueEntry->pNext;
    }


    /*
     * Allocate container records for all the objects.
     */
    pCurCnrRec = pCnrRec = (kQueueRecord*)allocMiniRec(sizeof(kQueueRecord), cRecords);
    if (pCurCnrRec == NULL)
        return FALSE;               /* FIXME: complain about this! */


    /*
     * Loop thru all queue data records and create container records for each.
     */
    pQueEntry = QueGetFirstQueueData();
    while (pQueEntry != NULL)
    {
        /*
         * Init and set the record.
         */
        pCurCnrRec->init();
        pCurCnrRec->set(pQueEntry);

        /*
         * Next queue data record.
         */
        pQueEntry = pQueEntry->pNext;
        pCurCnrRec = (kQueueRecord *) pCurCnrRec->getNext();
    }

    /*
     * Insert the conatiner records.
     */
    if (!insertAtTop(pCnrRec, cRecords))
        return FALSE;

    /* resort container */
    fRc = enableSorting();

    /*
     * Update the header with new queue count and
     * memory statistics.
     */
    cb = QueQueryUsedMemory();
    if (cb != this->cbUsed || cRecords != this->cQueues)
    {
        sprintf(pszTitle,
                "Queues Overview - %d Queues\r\n"
                "%d bytes of memory used. %d bytes available.",
                cRecords, cb, 128*1024 - cb);
        if (setTitle(pszTitle))
        {
            this->cbUsed = cb;
            this->cQueues = cRecords;
        }
        else
            fRc = FALSE;
    }

    return fRc;
}


/**
 * Command events - none yet.
 * @param     usCmd     Control id which send/posted the message.
 * @param     usSource  Source id.
 * @param     fPointer  Mouse pointer flag.
 */
VOID  kQueueContainer::command(USHORT usCmd, USHORT usSource, BOOL fPointer)
{
    kQueueRecord * pCurRecord = (kQueueRecord *)getCurRecord();
    switch (usCmd)
    {
        /*
         * Open the process detail dialog for the owner process.
         */
        case IDM_CNR_QUE_OWNERDETAILS:
            if (pCurRecord != NULL)
            {
                USHORT  usPid;
                PSZ     psz2;
                PCSZ    psz = pCurRecord->getOwnPid();
                usPid = (USHORT)strtol(psz, &psz2, psz[1] == 'x' ? 0x10 : 10);
                kProcessDetails::invoke(usPid, hwndCnr);
            }
            break;

        default:
            kTaskMgrContainerBase::command(usCmd, usSource, fPointer);
    }
}


/**
 * Constructor.
 * @param     hwndDlg   Handle to dialog window.
 * @param     ulCnrId   ID of the container dialog item in hwndDlg.
 * @param     usPid      = 0xFFFF: View all queues.
 *                      != 0:      View only queues used by the given process.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
kQueueContainer::kQueueContainer(HWND hwndDlg, ULONG ulCnrId, USHORT usPid /* = 0xFFFF*/) throw(kError)
    : kTaskMgrContainerBase(hwndDlg, ulCnrId,
                            "",
                            kQueueRecord::cFieldInfo,
                            (PFIELDINFO)&kQueueRecord::aFieldInfo[0],
                            IDM_CNR_QUEUE_ALL,
                            IDM_CNR_QUEUE,
                            IDM_CNR_QUE_OWNERDETAILS,
                            0),
    usPid(usPid), cQueues(~0UL), cbUsed(256*1024)
{
    TRACELOG_CONSTRUCT("start");
    pszTitle = (PSZ)malloc(128);
    if (pszTitle == NULL)
        throw (kError(ERROR_NOT_ENOUGH_MEMORY, kError::dos));
    *pszTitle = '\0';
    TRACELOG_CONSTRUCT("end");
}


/**
 * Destructor.
 */
kQueueContainer::~kQueueContainer()
{
    TRACELOG_DESTRUCT("start");
    if (pszTitle != NULL)
        free(pszTitle);
    pszTitle = NULL;
    TRACELOG_DESTRUCT("end");
}


/**
 * Updates the contents of the container.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
VOID  kQueueContainer::update()
{
    TRACELOG_UPDATE("start");
    QueCopyData(); /* !to be removed! */
    if (usPid < 0xFFFF)
        insertQueuePid();
    else
        insertQueueAll();
    TRACELOG_UPDATE("end");
}



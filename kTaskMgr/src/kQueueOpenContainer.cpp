/* $Id: kQueueOpenContainer.cpp,v 1.2 2000/05/23 00:51:26 stknut Exp $
 *
 * kQueueOpenContainer (kTaskMgr) - generic open queue container.
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

#include "kTaskMgrRecordBase.h"
#include "kTaskMgrContainerBase.h"

#include "kQueueOpenRecord.h"
#include "kQueueOpenContainer.h"
#include "kProcessDetails.h"
#include "kTaskMgr.h"
#include "kTaskMgr_defs.h"


/**
 * Updates the content of the container - inserts queue open info for a single process.
 * @returns   success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL   kQueueOpenContainer::insertQueueOpenPid()
{
    PMYQUEOPENENTRY     pOpenEntry;     /* Pointer to current queue open data record. */
    int                 cRecords;       /* Count of records to allocate / allocated. */
    kQueueOpenRecord  * pCurCnrRec,
                      * pCnrRec;

    /*
     * Remove all existing container records!
     */
    removeAllRecords();

    /*
     * Get data for this MTE and get the count of files.
     */
    cRecords = 0;
    pOpenEntry = QueGetFirstOpenData();
    while (pOpenEntry != NULL)
    {
        /* Add; Next record */
        if (pOpenEntry->usPid == usPid)
            cRecords++;
        pOpenEntry = pOpenEntry->pNext;
    }

    /*
     * If not open queues for this process, stop processing.
     */
    if (cRecords == 0)
        return TRUE;

    /*
     * Allocate container records for all the objects.
     */
    pCurCnrRec = pCnrRec = (kQueueOpenRecord*)allocMiniRec(sizeof(kQueueOpenRecord), cRecords);
    if (pCurCnrRec == NULL)
        return FALSE;               /* FIXME: complain about this! */


    /*
     * Loop thru all queue open data records and create container records for each.
     */
    pOpenEntry = QueGetFirstOpenData();
    while (pOpenEntry != NULL)
    {
        /*
         * Init and set the record.
         */
        if (pOpenEntry->usPid == usPid)
        {
            pCurCnrRec->init();
            pCurCnrRec->set(pOpenEntry);

            /* Advance to the next container record */
            pCurCnrRec = (kQueueOpenRecord *) pCurCnrRec->getNext();
        }

        /*
         * Next queue data record.
         */
        pOpenEntry = pOpenEntry->pNext;
    }

    /*
     * Insert the conatiner records.
     */
    if (!insertAtTop(pCnrRec, cRecords))
        return FALSE;

    /* resort container */
    return enableSorting();
}


/**
 * Updates the content of the container - inserts queue info for the entire system
 * @returns   success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL   kQueueOpenContainer::insertQueueOpenAll()
{
    PMYQUEOPENENTRY     pOpenEntry;     /* Pointer to current queue open data record. */
    int                 cRecords;       /* Count of records to allocate / allocated. */
    kQueueOpenRecord  * pCurCnrRec,
                      * pCnrRec;

    /*
     * Remove all existing container records!
     */
    removeAllRecords();

    /*
     * Get data for this MTE and get the count of files.
     */
    cRecords = 0;
    pOpenEntry = QueGetFirstOpenData();
    while (pOpenEntry != NULL)
    {
        /* Add; Next record */
        cRecords++;
        pOpenEntry = pOpenEntry->pNext;
    }


    /*
     * Allocate container records for all the objects.
     */
    pCurCnrRec = pCnrRec = (kQueueOpenRecord*)allocMiniRec(sizeof(kQueueOpenRecord), cRecords);
    if (pCurCnrRec == NULL)
        return FALSE;               /* FIXME: complain about this! */


    /*
     * Loop thru all queue data records and create container records for each.
     */
    pOpenEntry = QueGetFirstOpenData();
    while (pOpenEntry != NULL)
    {
        /*
         * Init and set the record.
         */
        pCurCnrRec->init();
        pCurCnrRec->set(pOpenEntry);

        /*
         * Next queue data record.
         */
        pOpenEntry = pOpenEntry->pNext;
        pCurCnrRec = (kQueueOpenRecord *) pCurCnrRec->getNext();
    }

    /*
     * Insert the conatiner records.
     */
    if (!insertAtTop(pCnrRec, cRecords))
        return FALSE;

    /* resort container */
    return enableSorting();
}


/**
 * Command events.
 * !IMPORTANT! Must be called by children which overrides it!
 * @param     usCmd     Control id which send/posted the message.
 * @param     usSource  Source id.
 * @param     fPointer  Mouse pointer flag.
 */
VOID  kQueueOpenContainer::command(USHORT usCmd, USHORT usSource, BOOL fPointer)
{
    kQueueOpenRecord * pCurRecord = (kQueueOpenRecord *)getCurRecord();
    switch (usCmd)
    {
        /*
         * Open the process detail dialog for the owner process.
         */
        case IDM_CNR_QUEO_OWNERDETAILS:
            if (pCurRecord != NULL)
            {
                USHORT  usPid;
                PSZ     psz2;
                PCSZ    psz = pCurRecord->getOwnPid();
                usPid = (USHORT)strtol(psz, &psz2, psz[1] == 'x' ? 0x10 : 10);
                kProcessDetails::invoke(usPid, hwndCnr);
            }
            break;

        /*
         * Open the process detail dialog for the client process.
         */
        case IDM_CNR_QUEO_PROCESSDETAILS:
            if (pCurRecord != NULL)
                kProcessDetails::invoke(pCurRecord->getPid(), hwndCnr);
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
kQueueOpenContainer::kQueueOpenContainer(HWND hwndDlg, ULONG ulCnrId, USHORT usPid /* = 0xFFFF*/) throw(kError)
    : kTaskMgrContainerBase(hwndDlg, ulCnrId,
                            "Open Queues Overview",
                            kQueueOpenRecord::cFieldInfo,
                            (PFIELDINFO)&kQueueOpenRecord::aFieldInfo[0],
                            IDM_CNR_QUEUEOPEN_ALL,
                            IDM_CNR_QUEUEOPEN,
                            IDM_CNR_QUEO_PROCESSDETAILS,
                            kQueueOpenRecord::cFieldInfo - 1),
    usPid(usPid)
{
    TRACELOG_CONSTRUCT("start");
    TRACELOG_CONSTRUCT("end");
}


/**
 * Destructor.
 */
kQueueOpenContainer::~kQueueOpenContainer()
{
    TRACELOG_DESTRUCT("start");
    TRACELOG_DESTRUCT("end");
}


/**
 * Updates the contents of the container.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
VOID  kQueueOpenContainer::update()
{
    TRACELOG_UPDATE("start");
    QueCopyData(); /* FIXME !remove this later! */
    if (usPid < 0xFFFF)
        insertQueueOpenPid();
    else
        insertQueueOpenAll();
    TRACELOG_UPDATE("end");
}



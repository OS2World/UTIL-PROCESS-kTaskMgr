/* $Id: kProcessDetailsContainer.cpp,v 1.2 2000/05/23 00:51:26 stknut Exp $
 *
 * kProcessDetailsContainer (kTaskMgr) - Container that lists information on a
 *    given processes.
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

#include "kObjectContainer.h"
#include "kModuleContainer.h"
#include "kDetailsRecord.h"
#include "kModuleDetails.h"
#include "kProcessDetails.h"
#include "kProcessDetailsContainer.h"
#include "kTaskMgr.h"
#include "kTaskMgr_defs.h"


/**
 * Command events - none yet.
 * @param     usCmd     Control id which send/posted the message.
 * @param     usSource  Source id.
 * @param     fPointer  Mouse pointer flag.
 */
VOID  kProcessDetailsContainer::command(USHORT usCmd, USHORT usSource, BOOL fPointer)
{
    switch (usCmd)
    {
        case IDM_CNR_OBJ_PROC_DTL_PPID:
            kProcessDetails::invoke(usPPid, hwndCnr);
            break;

        case IDM_CNR_OBJ_PROC_DTL_HMTE:
            kModuleDetails::invoke(hMte, hwndCnr);
            break;

        default:
            kTaskMgrContainerBase::command(usCmd, usSource, fPointer);
    }
}


/**
 * Initiate the record context menu.
 * @param     usMenuId
 * @param     hwndMnu
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
VOID  kProcessDetailsContainer::initMenu(USHORT usMenuId, HWND hwndMnu)
{
    kDetailsRecord *pCurRecord;

    /* don't work! */
    if (usMenuId == IDM_CNR_PROCESS_DETAILS
        && (pCurRecord = (kDetailsRecord*)getCurRecord()) != NULL)
    {
        int index = pCurRecord->getIndex();
        pMenuRecord->enableMenuItem(IDM_CNR_OBJ_PROC_DTL_PPID, index == 2);
        pMenuRecord->enableMenuItem(IDM_CNR_OBJ_PROC_DTL_HMTE, index == 3);
    }

    hwndMnu = hwndMnu;
}


/**
 * Constructor.
 * @param     hwndDlg   Handle to dialog window.
 * @param     ulCnrId   ID of the container dialog item in hwndDlg.
 * @param     usPid     Process identifier for the process we're to show
 *                      detail info for.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
kProcessDetailsContainer::kProcessDetailsContainer(HWND hwndDlg, ULONG ulCnrId, USHORT usPid /* = 0xFFFF*/) throw(kError)
    : kTaskMgrContainerBase(hwndDlg, ulCnrId,
                            NULL,
                            kDetailsRecord::cFieldInfo,
                            (PFIELDINFO)&kDetailsRecord::aFieldInfo[0],
                            IDM_CNR_PROCESS_DETAILS_ALL,
                            IDM_CNR_PROCESS_DETAILS),
    usPid(usPid), hMte(0xffff), usPPid(0xffff)
{
    TRACELOG_CONSTRUCT("start");
    PPROCESSDATA    pProcData;
    qsLrec_t *      pMteRec;
    kDetailsRecord *pCurCnrRec;
    int             cRecords = 12;
    char            szBuffer[512];



    /*
     * Get information for the QS subsystem.
     * We need Process and module information.
     */
    pProcData = QSGetProcessData(usPid);
    if (pProcData == NULL)
        throw(kError(ERROR_INVALID_PARAMETER, kError::dos));
    pMteRec = QSGetMteData(pProcData->pProcRec->hMte);
    if (pMteRec == NULL)
        throw(kError(ERROR_INVALID_PARAMETER, kError::dos));

    hMte = pProcData->pProcRec->hMte;
    usPPid = pProcData->pProcRec->ppid;

    /*
     * Allocate records.
     */
    pCurCnrRec = pCnrRec = (kDetailsRecord *)allocMiniRec(sizeof(kDetailsRecord), cRecords);
    if (pCurCnrRec == NULL)
        throw(kError(kError::unused, kError::win));

    /*
     * Insert records into the container.
     */
    /* 0 - static */
    pCurCnrRec->init(0);
    pCurCnrRec->set("Name", strdup((PCSZ)pMteRec->pName));
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 1 - static */
    pCurCnrRec->init(1);
    sprintf(szBuffer, "0x%04x", pProcData->pProcRec->pid);
    pCurCnrRec->set("Pid", strdup(szBuffer));
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 2 */
    pCurCnrRec->init(2);
    pCurCnrRec->set("Parent Pid");
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 3 - static */
    pCurCnrRec->init(3);
    sprintf(szBuffer, "0x%04x", pProcData->pProcRec->hMte);
    pCurCnrRec->set("Module handle", strdup(szBuffer));
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 4 */
    pCurCnrRec->init(4);
    pCurCnrRec->set("Type");
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 5 */
    pCurCnrRec->init(5);
    pCurCnrRec->set("Status");
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 6 */
    pCurCnrRec->init(6);
    pCurCnrRec->set("Screen Group Id");
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 7 */
    pCurCnrRec->init(7);
    pCurCnrRec->set("Threads");
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 8 */
    pCurCnrRec->init(8);
    pCurCnrRec->set("Open Files");
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 9 */
    pCurCnrRec->init(9);
    pCurCnrRec->set("Libraries (DLLs)");
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 10 */
    pCurCnrRec->init(10);
    pCurCnrRec->set("32 Bits semaphors");
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 11 */
    pCurCnrRec->init(11);
    pCurCnrRec->set("Shared memory areas");
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    #if 0
    /* 12 - static */
    pCurCnrRec->init(12);
    pCurCnrRec->set("Child processes");
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();
    #endif


    /*
     * Insert the records.
     */
    if (!insertAtBottom(pCnrRec, cRecords))
        throw(kError(kError::unused, kError::win));
    update();

    TRACELOG_CONSTRUCT("end");
}


/**
 * Destructor.
 */
kProcessDetailsContainer::~kProcessDetailsContainer()
{
    TRACELOG_DESTRUCT("start");
    kDetailsRecord *pCurCnrRec = pCnrRec;

    /*
     * Free stuff in records.
     */
    while (pCurCnrRec != NULL)
    {
        PCSZ pszValue;
        pszValue = pCurCnrRec->getValue();
        if (pszValue)
            free((void*)pszValue);
        pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();
    }
    this->removeAllRecords();

    TRACELOG_DESTRUCT("end");
}


/**
 * Updates the contents of the container.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
VOID  kProcessDetailsContainer::update()
{
    TRACELOG_UPDATE("start");
    PPROCESSDATA    pProcData;
    kDetailsRecord *pCurCnrRec = pCnrRec;
    char            szBuffer[512];
    PCSZ            psz;


    /*
     * Get information for the QS subsystem.
     * We need Process information.
     */
    pProcData = QSGetProcessData(usPid);
    if (pProcData == NULL)
        throw(kError(ERROR_INVALID_PARAMETER, kError::dos));

    /*
     * Update the records
     */
    /* 0 - static */
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 1 - static */
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 2 - Parent Pid */
    sprintf(szBuffer, "0x%04x", pProcData->pProcRec->ppid);
    pCurCnrRec->updateValue(szBuffer);
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 3 - static */
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 4 - Process Type */
    switch (pProcData->pProcRec->type)
    {
        case 0: psz = "FullScr (%d)"; break;
        case 1: psz = "VDM (%d)"; break;
        case 2: psz = "VIO (%d)"; break;
        case 3: psz = "PM (%d)"; break;
        case 4: psz = "Detach (%d)"; break;
        default:psz = "unknown (%d)"; break;

    }
    sprintf(szBuffer, psz, pProcData->pProcRec->type);
    pCurCnrRec->updateValue(szBuffer);
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 5 - Process Status */
    szBuffer[0] = '\0';
    if (pProcData->pProcRec->stat & 0x01)
        strcat(szBuffer, "ExitList ");
    if (pProcData->pProcRec->stat & 0x02)
        strcat(szBuffer, "ExitingThread1 ");
    if (pProcData->pProcRec->stat & 0x04)
        strcat(szBuffer, "ExitingAll ");
    if (pProcData->pProcRec->stat & 0x10)
        strcat(szBuffer, "SyncParent ");
    if (pProcData->pProcRec->stat & 0x20)
        strcat(szBuffer, "WaitParent ");
    if (pProcData->pProcRec->stat & 0x40)
        strcat(szBuffer, "Dying ");
    if (pProcData->pProcRec->stat & 0x80)
        strcat(szBuffer, "Embryonic ");
    if (pProcData->pProcRec->stat == 0)
        strcat(szBuffer, "Normal ");
    sprintf(szBuffer + strlen(szBuffer), "(0x%04x)", pProcData->pProcRec->stat);
    pCurCnrRec->updateValue(szBuffer);
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 6 - Screen group id */
    sprintf(szBuffer, "0x%04x", pProcData->pProcRec->sgid);
    pCurCnrRec->updateValue(szBuffer);
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 7 - Threads */
    sprintf(szBuffer, "%d", pProcData->pProcRec->cTCB);
    pCurCnrRec->updateValue(szBuffer);
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 8 - Open Files */
    sprintf(szBuffer, "%d", pProcData->pProcRec->cFH);
    pCurCnrRec->updateValue(szBuffer);
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 9 - Loaded libraries. */
    sprintf(szBuffer, "%d", pProcData->pProcRec->cLib);
    pCurCnrRec->updateValue(szBuffer);
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 10 - 32 Bits semaphors */
    sprintf(szBuffer, "%d", pProcData->pProcRec->c32PSem);
    pCurCnrRec->updateValue(szBuffer);
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 11 - Shared memory areas */
    sprintf(szBuffer, "%d", pProcData->pProcRec->cShrMem);
    pCurCnrRec->updateValue(szBuffer);
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    #if 0
    /* 12 - Child processes */
    sprintf(szBuffer, "%d", pProcData->pProcRec->cLib);
    pCurCnrRec->set("Child processes", strdup(szBuffer));
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();
    #endif


    /*
     * Invalidate all the dirty records.
     */
    pCnrRec->invalidateAllDirtyRecords(this);
    TRACELOG_UPDATE("end");
}

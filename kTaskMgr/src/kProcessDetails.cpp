/* $Id: kProcessDetails.cpp,v 1.2 2000/05/23 00:51:26 stknut Exp $
 *
 * kProcessDetails - Process notebook.
 *
 * Copyright (c) 2000 knut st. osmundsen
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
#include <process.h>
#include <stdio.h>
#include <string.h>

#include "kBase.h"
#include "kError.h"
#include "kDlgBase.h"
#include "kMenuBase.h"
#include "kClickDlg.h"
#include "kContainer.h"
#include "kTaskMgr_defs.h"
#include "kNotebookBase.h"
#include "kNotebookPageBase.h"

#include "kQuerySysState.h"
#include "kQueueHack.h"
#include "kDetailsBase.h"
#include "kTaskMgrRecordBase.h"
#include "kTaskMgrContainerBase.h"
#include "kContainerPageBase.h"

#include "kModuleRecord.h"
#include "kModuleContainer.h"

#include "kObjectRecord.h"

#include "kProcessRecord.h"
#include "kProcessContainer.h"

#include "kQueueRecord.h"
#include "kQueueContainer.h"

#include "kQueueOpenRecord.h"
#include "kQueueOpenContainer.h"

#include "kSFNRecord.h"
#include "kSFNContainer.h"

#include "kThreadRecord.h"
#include "kThreadContainer.h"

#include "kAboutPage.h"
#include "kModulePage.h"
#include "kProcessPage.h"
#include "kProcessDetailsPage.h"
#include "kQueuePage.h"
#include "kQueueOpenPage.h"
#include "kSFNPage.h"
#include "kThreadPage.h"

#include "kProcessDetails.h"
#include "kTaskMgr.h"



/**
 * Constructor.
 * @param     usPid        Process ID to display details for.
 * @param     hwndTaskMgr  Handle of the taskmgr window.
 */
kProcessDetails::kProcessDetails(USHORT usPid, HWND hwndTaskMgr) throw (kError)
: kNotebookBase(DL_KPROCESSDETAILS, NULLHANDLE, KPROCDTL_NTBK, NULLHANDLE),
  kDetailsBase(hwnd),
  hwndOwner(hwndTaskMgr),
  usPid(usPid)
{
    TRACELOG_CONSTRUCT("start");
    PPROCESSDATA    pProcData;
    qsLrec_t *      pMteData;
    USHORT          hMte;
    char szTitle[278];

    /* Get process data */
    pProcData = QSGetProcessData(usPid);
    if (pProcData == NULL)
    {
        _heap_check();
        fprintf(stderr, "throw\n");
        _dump_allocated(32);
        flushall();
        throw(kError(ERROR_INVALID_PARAMETER, kError::dos)); /* FIXME better complaint! */
    }
    pMteData = QSGetMteData(pProcData->pProcRec->hMte);
    if (pMteData != NULL)
        hMte = pMteData->hmte;
    else
        hMte = 0xFFFE;
    /* Set title */
    sprintf(&szTitle[0], "Pid 0x%04x - %.260s",
            usPid, pMteData != NULL && pMteData->pName != NULL ? (char*)pMteData->pName : "<unknown>");
    setTitle(&szTitle[0]);

    /* Create and insert notebook pages */
    insertPage(new kProcessDetailsPage(this, usPid), BKA_MAJOR | BKA_AUTOPAGESIZE, BKA_LAST);
    insertPage(new kThreadPage(this, usPid), BKA_MAJOR, BKA_LAST);
    insertPage(new kProcessPage(this, usPid), BKA_MAJOR, BKA_LAST);
    insertPage(new kModulePage(this, 0xffff, usPid), BKA_MAJOR, BKA_LAST);
    insertPage(new kModulePage(this, hMte, usPid), BKA_MAJOR, BKA_LAST);
    insertPage(new kSFNPage(this, usPid), BKA_MAJOR, BKA_LAST);
    insertPage(new kQueuePage(this, usPid), BKA_MAJOR, BKA_LAST);
    insertPage(new kQueueOpenPage(this, usPid), BKA_MAJOR, BKA_LAST);

    /* Last */
    insertPage(new kAboutPage(this), BKA_MAJOR, BKA_LAST);
    TRACELOG_CONSTRUCT("end");
}



/**
 * Destructor for the kProcessDetails class.
 */
kProcessDetails::~kProcessDetails()
{
    TRACELOG_DESTRUCT("start");
    TRACELOG_DESTRUCT("end");
}


/**
 * Temporary size and focus fix....
 */
BOOL kProcessDetails::show()
{
    SWP swp;
    BOOL fRet = kDlgBase::show();

    /* Temporary size fix  */
    WinQueryWindowPos(hwnd, &swp);
    WinSetWindowPos(hwnd, NULLHANDLE, 0, 0, swp.cx, swp.cy, SWP_SIZE);

    /* Try set focus correctly */
    WinSetFocus(HWND_DESKTOP, hwnd);

    return fRet;
}


/**
 * Updates the dialog contents.
 * @returns   success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark    Currently a dummy stub.
 */
BOOL kProcessDetails::update()
{
    TRACELOG_UPDATE("start");
    TRACELOG_UPDATE("end");
    return FALSE;
}


/**
 * Invokes an modeless process details dialog.
 * @returns     TRUE if dialog was invoked successfully.
 *              FALSE on failure.
 * @param       usMte       Process id.
 * @param       hwndOwner   Handle to owner window.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL kProcessDetails::invoke(USHORT usPid, HWND hwndOwner)
{
    if (usPid == 0)
        return FALSE;
    try
    {
        kProcessDetails *pDlg = new kProcessDetails(usPid, hwndOwner);
        return pDlg->show();
    }
    catch (kError err)
    {
        err.showError(kTaskMgr::pszErrorTitle);
    }
    return FALSE;
}


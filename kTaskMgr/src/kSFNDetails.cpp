/* $Id: kSFNDetails.cpp,v 1.3 2000/05/23 03:37:08 stknut Exp $
 *
 * kSFNDetails - SFN details notebook.
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

#include "kProcessRecord.h"
#include "kProcessContainer.h"

#include "kAboutPage.h"
#include "kProcessPage.h"
#include "kSFNDetailsPage.h"

#include "kSFNDetails.h"
#include "kTaskMgr.h"



/**
 * Constructor.
 * @param     usSFN        SFN to display details for.
 * @param     hwndTaskMgr  Handle of the taskmgr window.
 */
kSFNDetails::kSFNDetails(USHORT usSFN, HWND hwndTaskMgr) throw (kError)
: kNotebookBase(DL_KSFNDETAILS, NULLHANDLE, KSFNDTL_NTBK, NULLHANDLE),
  kDetailsBase(hwnd),
  hwndOwner(hwndTaskMgr),
  usSFN(usSFN)
{
    TRACELOG_CONSTRUCT("start");
    qsFrec_t *      pFSData;
    char            szTitle[278];

    /* Get SFN data */
    pFSData = QSGetSfnData(usSFN);
    if (pFSData == NULL)
        throw(kError(ERROR_INVALID_PARAMETER, kError::dos)); /* FIXME better complaint! */

    /* Set title */
    sprintf(&szTitle[0], "SFN 0x%04x - %.260s",
            usSFN, pFSData + 1);
    setTitle(&szTitle[0]);

    /* Create and insert notebook pages */
    insertPage(new kSFNDetailsPage(this, usSFN), BKA_MAJOR | BKA_AUTOPAGESIZE, BKA_LAST);
    insertPage(new kProcessPage(this, 0, usSFN), BKA_MAJOR | BKA_AUTOPAGESIZE, BKA_LAST);

    /* Last */
    insertPage(new kAboutPage(this), BKA_MAJOR, BKA_LAST);

    TRACELOG_CONSTRUCT("end");
}



/**
 * Destructor for the kSFNDetails class.
 */
kSFNDetails::~kSFNDetails()
{
    TRACELOG_DESTRUCT("start");
    TRACELOG_DESTRUCT("end");
}


/**
 * Temporary size and focus fix....
 */
BOOL kSFNDetails::show()
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
BOOL kSFNDetails::update()
{
    TRACELOG_UPDATE("start");
    TRACELOG_UPDATE("end");
    return FALSE;
}


/**
 * Invokes an modeless SFN details dialog.
 * @returns     TRUE if dialog was invoked successfully.
 *              FALSE on failure.
 * @param       usSFN       SFN.
 * @param       hwndOwner   Handle to owner window.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL kSFNDetails::invoke(USHORT usSFN, HWND hwndOwner)
{
    qsFrec_t *      pFSData;
    int             iSFN;

    /*
     * temp fix until throws are working again.
     */
    pFSData = QSGetSfnData(usSFN);
    if (pFSData == NULL)
        return FALSE;
    for (iSFN = 0; iSFN < pFSData->ctSft; iSFN++)
        if (pFSData->pSft[iSFN].sfn == usSFN)
            break;
    if (usSFN == 0 || iSFN >= pFSData->ctSft)
        return FALSE;

    try
    {
        kSFNDetails *pDlg = new kSFNDetails(usSFN, hwndOwner);
        return pDlg->show();
    }
    catch (kError err)
    {
        err.showError(kTaskMgr::pszErrorTitle);
    }
    return FALSE;
}


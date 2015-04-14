/* $Id: kTaskMgr.cpp,v 1.3 2000/06/19 20:08:42 stknut Exp $
 *
 * kTaskMgr - OS/2 taskmanager
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
#include "kObjectContainer.h"

#include "kMemMapRecord.h"
#include "kMemMapContainer.h"

#include "kProcessRecord.h"
#include "kProcessContainer.h"

#include "kQueueRecord.h"
#include "kQueueContainer.h"

#include "kQueueOpenRecord.h"
#include "kQueueOpenContainer.h"

#include "kSFNRecord.h"
#include "kSFNContainer.h"

#include "kAboutPage.h"
#include "kMemMapPage.h"
#include "kModulePage.h"
#include "kProcessPage.h"
#include "kQueuePage.h"
#include "kQueueOpenPage.h"
#include "kSFNPage.h"
#include "kThreadPage.h"

#include "kTaskMgr.h"
#include "kTaskMgr_defs.h"



/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
/*Note: These are public */
PCSZ   kTaskMgr::pszErrorTitle        = "kTaskMgr - Error";
PCSZ   kTaskMgr::pszInfoTitle         = "kTaskMgr - Information";



/*******************************************************************************
*   Internal Functions                                                         *
*******************************************************************************/
MEMBERTHREAD(kTaskMgr);



/**
 *
 * @returns
 * @param
 * @equiv
 * @precond
 * @methdesc
 * @result
 * @time
 * @sketch
 * @algo
 * @remark
 */
int main(int argc, char **argv)
{
    return kTaskMgr::init(argc, argv);
}



/**
 *
 * @returns
 * @param
 * @equiv
 * @precond
 * @methdesc
 * @result
 * @time
 * @sketch
 * @algo
 * @remark
 */
int kTaskMgr::init(int argc, char **argv)
{
    HAB     hab;
    HMQ     hmq;
    kTaskMgr *pTaskMgr;

    hab = WinInitialize(0);          ASSERT(hab);
    hmq = WinCreateMsgQueue(hab, 0); ASSERT(hmq);

    try
    {
        /* create kTaskMgr window/object */
        pTaskMgr = new kTaskMgr(hab);
        pTaskMgr->showModal();

        delete pTaskMgr;
    }
    catch (kError err)
    {
        err.logError();
        if (err.getErrorNo() != kError::user_cancel)
            err.showError(kTaskMgr::pszErrorTitle);
    }

    /* cleanup */
    WinDestroyMsgQueue(hmq);
    WinTerminate(hab);

    argc = argc;
    argv = argv;

    _dump_allocated(32);

    return 0;
}



/**
 *
 * @returns
 * @param
 * @equiv
 * @precond
 * @methdesc
 * @result
 * @time
 * @sketch
 * @algo
 * @remark
 */
kTaskMgr::kTaskMgr(HAB hab) throw (kError)
    : kNotebookBase(DL_KTASKMGR, NULLHANDLE, KTM_NTBK, HWND_DESKTOP),
    hab(hab)
{
    TRACELOG_CONSTRUCT("start");
    /* Create and insert notebook pages */
    insertPage(new kProcessPage(this), BKA_MAJOR, BKA_AUTOPAGESIZE | BKA_LAST);
    insertPage(new kThreadPage(this), BKA_MAJOR, BKA_AUTOPAGESIZE | BKA_LAST);
    insertPage(new kModulePage(this), BKA_MAJOR, BKA_LAST);
    insertPage(new kSFNPage(this), BKA_MAJOR, BKA_LAST);
    if (QueInit())
    {
        insertPage(new kQueuePage(this), BKA_MAJOR, BKA_LAST);
        insertPage(new kQueueOpenPage(this), BKA_MAJOR, BKA_LAST);
    }
    insertPage(new kMemMapPage(this), BKA_MAJOR, BKA_LAST);

    /* Last */
    insertPage(new kAboutPage(this), BKA_MAJOR, BKA_LAST);

    if (paPages != NULL && cPages != 0)
        WinSetFocus(HWND_DESKTOP, WinWindowFromID(paPages[0].hwnd, KPROCPG_CNR));

    TRACELOG_CONSTRUCT("end");
}



/**
 * Destructor for the kTaskMgr class.
 */
kTaskMgr::~kTaskMgr()
{
    TRACELOG_CONSTRUCT("start");
    TRACELOG_CONSTRUCT("end");
}



/**
 * Temporary(?) size and focus fix....
 */
ULONG kTaskMgr::showModal()
{
    SWP     swp;

    kDlgBase::show();

    /* Temporary size fix  */
    WinQueryWindowPos(hwnd, &swp);
    WinSetWindowPos(hwnd, NULLHANDLE, 0, 0, swp.cx, swp.cy, SWP_SIZE);

    /* Set focus correctly */
    WinSetFocus(HWND_DESKTOP, hwnd);

    return kDlgBase::showModal();
}



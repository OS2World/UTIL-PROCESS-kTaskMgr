/* $Id: kProcessPage.cpp,v 1.3 2000/05/23 03:37:08 stknut Exp $
 *
 * kProcessPage - Process overview dialog. (to be notebook page)
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
#include "kContainer.h"
#include "kNotebookBase.h"
#include "kNotebookPageBase.h"

#include "kOS2Info.h"
#include "kQuerySysState.h"
#include "kDetailsBase.h"
#include "kTaskMgrRecordBase.h"
#include "kTaskMgrContainerBase.h"
#include "kContainerPageBase.h"

#include "kProcessRecord.h"
#include "kProcessContainer.h"
#include "kProcessPage.h"
#include "kProcessDetails.h"
#include "kTaskMgr_defs.h"
#include "kTaskMgr.h"


/**
 * timer event. Update content.
 * @param     idTimer  Timer id.
 */
VOID kProcessPage::timer(ULONG idTimer)
{
    if (idTimer == idTimerRefresh)
        pContainer->update();
}


/**
 * Constructor. Calls base class constructor.
 * @param     hwndParent   Windowhandle of the parent window. Defaults to HWND_DESKTOP.
 * @param     hwndOwner    Windowhandle of the owner window.
 * @param     usPid        == 0xffff:   Show all processes
 *                         <  0xffff:   Show children of usPid.
 * @param     usSFN        == 0xffff:   Ignored
 *                         <  0xffff:   Show processes using this SFN. usPid is ignored.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark    Throws kError on error.
 *            Use the show()/showModal() methods to show the dialog
 */
kProcessPage::kProcessPage(kNotebookBase *pNtbk, USHORT usPid/*= 0xffff*/, USHORT usSFN/*= 0xffff*/) throw (kError)
    : kContainerPageBase(pNtbk,
                         usPid < 0xffff && usSFN == 0xffff ?
                            "Child ~Processes" : "~Processes"
                         )
{
    TRACELOG_CONSTRUCT("start");
    /*
     * Create detail container and fill it.
     */
    pContainer = new kProcessContainer(hwnd, KCNRPG_CNR, usPid, usSFN);
    pContainer->update();

    /*
     * Start timer
     */
    idTimerRefresh = timerStart(1UL, REFRESHRATE*1000);
    ASSERT(idTimerRefresh);
    TRACELOG_CONSTRUCT("end");
}



/**
 * Desctructor stub.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark
 */
kProcessPage::~kProcessPage()
{
    TRACELOG_DESTRUCT("start");
    timerStop(idTimerRefresh);
    TRACELOG_DESTRUCT("end");
}



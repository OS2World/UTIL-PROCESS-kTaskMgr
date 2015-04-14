/* $Id: kQueuePage.cpp,v 1.2 2000/05/23 00:51:26 stknut Exp $
 *
 * kQueuePage (kTaskMgr) - queue overview page.
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

#include "kQuerySysState.h"
#include "kQueueHack.h"
#include "kDetailsBase.h"
#include "kTaskMgrRecordBase.h"
#include "kTaskMgrContainerBase.h"
#include "kContainerPageBase.h"

#include "kQueueRecord.h"
#include "kQueueContainer.h"
#include "kObjectRecord.h"
#include "kObjectContainer.h"
#include "kQueuePage.h"
#include "kTaskMgr_defs.h"
#include "kTaskMgr.h"


/**
 * Create an processes queue page for a single process or for the entire system.
 * @param     pNtbk     Pointer to notebook object.
 * @param     usPid     Process identificator of the process we're to show used queues for.
 *                      Set usPid like 0xffff ot create an global queue page.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark    Throws kError on error.
 */
kQueuePage::kQueuePage(kNotebookBase *pNtbk, USHORT usPid/*= 0xffff*/) throw (kError)
    : kContainerPageBase(pNtbk, "~Queues")
{
    TRACELOG_CONSTRUCT("start");

    /*
     * Create detail container
     */
    pContainer = new kQueueContainer(hwnd, KCNRPG_CNR, usPid);
    pContainer->update();

    TRACELOG_CONSTRUCT("end");
}

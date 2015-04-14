/* $Id: kProcessDetailsPage.cpp,v 1.2 2000/05/23 00:51:26 stknut Exp $
 *
 * kProcessDetailsPage (kTaskMgr) - Process Details page.
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

#include "kDetailsRecord.h"
#include "kProcessDetailsContainer.h"
#include "kProcessDetailsPage.h"
#include "kTaskMgr_defs.h"
#include "kTaskMgr.h"




/**
 * Creat a process details page.
 * @param     pNtbk     Pointer to notebook object.
 * @param     usPid     Process identificator of the process we're detail information on.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark    Throws kError on error.
 */
kProcessDetailsPage::kProcessDetailsPage(kNotebookBase *pNtbk, USHORT usPid) throw (kError)
    : kContainerPageBase(pNtbk, "~Details")
{
    TRACELOG_CONSTRUCT("start");

    /*
     * Create detail container
     */
    pContainer = new kProcessDetailsContainer(hwnd, KCNRPG_CNR, usPid);
    pContainer->update();

    TRACELOG_CONSTRUCT("end");
}


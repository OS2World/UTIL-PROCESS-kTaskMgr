/* $Id: kSFNPage.cpp,v 1.3 2000/06/19 20:08:42 stknut Exp $
 *
 * kSFNPage (kTaskMgr) - System file number overview page.
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

#include "kSFNRecord.h"
#include "kSFNContainer.h"
#include "kObjectRecord.h"
#include "kObjectContainer.h"
#include "kSFNPage.h"
#include "kTaskMgr_defs.h"
#include "kTaskMgr.h"




/**
 * Create an SFN/JFN page.
 * @param     pNtbk     Pointer to notebook object.
 * @param     usPid     Process identificator of the process we're to show SFN/JFNs for.
 *                      0xffff  if entire system.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark    Throws kError on error.
 */
kSFNPage::kSFNPage(kNotebookBase *pNtbk, USHORT usPid/* = 0xffff*/) throw (kError)
    : kContainerPageBase(pNtbk, usPid < 0xffff && ulBuildLevel >= 14040 ? "JFN/~SFN" : "~SFN")
{
    TRACELOG_CONSTRUCT("start");

    /*
     * Create detail container
     */
    pContainer = new kSFNContainer(hwnd, KCNRPG_CNR, usPid);
    pContainer->update();

    TRACELOG_CONSTRUCT("end");
}


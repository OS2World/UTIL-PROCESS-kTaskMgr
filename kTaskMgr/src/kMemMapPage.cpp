/* $Id: kMemMapPage.cpp,v 1.1 2000/06/19 20:08:42 stknut Exp $
 *
 * kSFNPage (kTaskMgr) - Memory map overview page.
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

#include "kObjectRecord.h"
#include "kMemMapRecord.h"
#include "kMemMapContainer.h"
#include "kObjectRecord.h"
#include "kObjectContainer.h"
#include "kMemMapPage.h"
#include "kTaskMgr_defs.h"
#include "kTaskMgr.h"




/**
 * Create an  page.
 * @param     pNtbk     Pointer to notebook object.
 * @param     usPid     0xffff if entire system.
 *                      Process id to show memmap for.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark    Throws kError on error.
 */
kMemMapPage::kMemMapPage(kNotebookBase *pNtbk, USHORT usPid/* = 0xffff*/) throw (kError)
    : kContainerPageBase(pNtbk, "~Memory Map")
{
    TRACELOG_CONSTRUCT("start");

    /*
     * Create detail container
     */
    pContainer = new kMemMapContainer(hwnd, KCNRPG_CNR, usPid);
    pContainer->update();

    TRACELOG_CONSTRUCT("end");
}


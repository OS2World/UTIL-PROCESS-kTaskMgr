/* $Id: kModulePage.cpp,v 1.2 2000/05/23 00:51:26 stknut Exp $
 *
 * kModulePage - Module overview dialog. (to be notebook page)
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
#include "kDetailsBase.h"
#include "kTaskMgrRecordBase.h"
#include "kTaskMgrContainerBase.h"
#include "kContainerPageBase.h"

#include "kModuleRecord.h"
#include "kModuleContainer.h"
#include "kObjectRecord.h"
#include "kObjectContainer.h"
#include "kModulePage.h"
#include "kModuleDetails.h"
#include "kTaskMgr_defs.h"
#include "kTaskMgr.h"




/**
 * Create a import module page.
 * @param     pNtbk     Pointer to notebook object.
 * @param     hMte      Module handle.
 *                      0xffff to show all modules in the system (default) when usPid is 0xffff.
 *            usPid     Process id to show loaded modules for. Ignored if 0xffff (default).
 *                      hMte is ignored when this is < 0xffff.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark    Throws kError on error.
 */
kModulePage::kModulePage(kNotebookBase *pNtbk, USHORT hMte/* = 0xffff*/, USHORT usPid/*= 0xffff*/) throw (kError)
    : kContainerPageBase(pNtbk,
                         usPid < 0xffff ?
                           hMte < 0xffff ? "~All Modules"
                                         : "Loaded ~Modules"
                         : hMte < 0xffff ? "Imported ~Modules"
                                         : "~Modules"
                        )
{
    TRACELOG_CONSTRUCT("start");

    /*
     * Create detail container
     */
    pContainer = new kModuleContainer(hwnd, KCNRPG_CNR, hMte, usPid);
    pContainer->update();

    TRACELOG_CONSTRUCT("end");
}

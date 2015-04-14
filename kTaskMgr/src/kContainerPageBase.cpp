/* $Id: kContainerPageBase.cpp,v 1.2 2000/05/23 00:51:25 stknut Exp $
 *
 * kContainerPageBase (kTaskMgr) - Generalized container page.
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
#include "kTaskMgrContainerBase.h"
#include "kDetailsBase.h"
#include "kContainerPageBase.h"
#include "kTaskMgr_defs.h"
#include "kTaskMgr.h"


/**
 * Forwards to the container.
 * @param     usId     Container id.
 * @param     pRecord  Pointer to the record which is selected by either the key
 */
VOID kContainerPageBase::cnrContextMenu(USHORT usId, PRECORDCORE pRecord)
{
    if (pContainer != NULL)
        pContainer->cnrContextMenu(usId, pRecord);
}


/**
 * Forwards to the container.
 */
VOID kContainerPageBase::cnrEnter(USHORT usId, HWND hwndCnr, PRECORDCORE pRecord, ULONG fKey)
{
    if (pContainer != NULL)
        pContainer->cnrEnter(usId, hwndCnr, pRecord, fKey);
}



VOID  kContainerPageBase::ntfyInserted()
{
    kNotebookPageBase::ntfyInserted();

    /*
     * Set tab text
     */
    pNtbk->setTabText(ulPageId, pszTabText);
}



/**
 * Create a notebook page containing only a container.
 * @param     pNtbk         Pointer to notebook object.
 * @param     pszTabText    Pointer to the text to be placed in the tag/dogear.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark    Throws kError on error.
 */
kContainerPageBase::kContainerPageBase(kNotebookBase *pNtbk, PSZ pszTabText) throw (kError)
    : kNotebookPageBase(DL_KCONTAINERPAGE, NULLHANDLE, pNtbk),
    pContainer(NULL), pszTabText(NULL)
{
    TRACELOG_CONSTRUCT("start");

    /*
     * Container border size
     */
    ptlBorder.x = 5;
    ptlBorder.y = 4;
    WinMapDlgPoints(hwnd, &ptlBorder, 1, TRUE);

    /*
     * Save tab text so we can't set when
     * the page has been inserted into the notebook.
     */
    this->pszTabText = strdup(pszTabText);

    TRACELOG_CONSTRUCT("end");
}


/**
 * Desctructor stub.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark
 */
kContainerPageBase::~kContainerPageBase()
{
    TRACELOG_DESTRUCT("start");

    if (pContainer != NULL)
        delete pContainer;
    pContainer = NULL;

    if (pszTabText != NULL)
        free(pszTabText);
    pszTabText = NULL;

    TRACELOG_DESTRUCT("end");
}


/**
 * Move and size the container and dialog.
 * @param     prctlPage  New size of dialog.
 */
VOID  kContainerPageBase::ntfySized(LONG cx, LONG cy)
{
    /*
     * Resize the dialog
     */
    resizeAndMove(
        0,
        0,
        cx,
        cy);

    /*
     * Resize the container.
     */
    pContainer->resizeAndMove(
        ptlBorder.x,
        ptlBorder.y,
        cx - ptlBorder.x*2,
        cy - ptlBorder.y*2
        );
}


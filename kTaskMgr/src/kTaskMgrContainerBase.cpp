/* $Id: kTaskMgrContainerBase.cpp,v 1.1 2000/05/23 00:12:46 stknut Exp $
 *
 * Container base for kTaskMgr (kTaskMgr)
 *      Defines a common ground for containers in kTaskMgr.
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
#include "kClickDlg.h"
#include "kContainer.h"

#include "kQuerySysState.h"
#include "kOS2Info.h"
#include "kNotebookBase.h"
#include "kNotebookPageBase.h"
#include "kTaskMgrRecordBase.h"
#include "kTaskMgrContainerBase.h"
#include "kTaskMgr.h"
#include "kTaskMgr_defs.h"



/**
 * Menu is closing. Remove emphasis.
 * @param     usMenuId  Menu id.
 * @param     hwndMnu   Handle to menu window.
 */
VOID kTaskMgrContainerBase::menuEnd(USHORT usMenuId, HWND hwndMnu)
{
    setRecordEmphasis(pCurRecord, FALSE, CRA_SOURCE);
    hwndMnu = hwndMnu;
    usMenuId = usMenuId;
}


/**
 * Command events - none yet.
 * @param     usCmd     Control id which send/posted the message.
 * @param     usSource  Source id.
 * @param     fPointer  Mouse pointer flag.
 */
VOID  kTaskMgrContainerBase::command(USHORT usCmd, USHORT usSource, BOOL fPointer)
{
    if (usCmd == IDM_CNR_ALL_REFRESH)
    {
        /*
         * Refresh menu item were selected - update the container!
         */
        update();
    }
    else  if (usCmd >= IDM_CNR_ALL_SORT_FIRST && usCmd <= IDM_CNR_ALL_SORT_LAST)
    {
        USHORT us;

        /*
         * One of the sort menu items were selected.
         *      If it's an uncheck request Then
         *          Set back to default sorting. (if allready default sorting - do nothing - break)
         *      Endif.
         *      Save new sort id.
         *      Update menuitems.
         *      (re)Enable sorting.
         */
        if (usCmd - IDM_CNR_ALL_SORT_FIRST == iSortColumn)
        {
            if (usCmd - IDM_CNR_ALL_SORT_FIRST == iSortDefaultColumn)
                return;
            usCmd = (USHORT)(IDM_CNR_ALL_SORT_FIRST + iSortDefaultColumn);
        }

        iSortColumn = usCmd - IDM_CNR_ALL_SORT_FIRST;
        for (us = IDM_CNR_ALL_SORT_FIRST; us <= IDM_CNR_ALL_SORT_LAST; us++)
            pMenuCnrAll->checkMenuItem(us, usCmd == us);

        /* resort container */
        enableSorting();
    }

    usCmd = usCmd;
    usSource = usSource;
    fPointer = fPointer;
}


/**
 * Record sort callback function - compares two records.
 * @returns   >  0  when pRecord1  >  pRecord2
 *            <  0  when pRecord1  <  pRecord2
 *            == 0  when pRecord1 ==  pRecord2
 * @param     pRecord1  Pointer to first record.
 * @param     pRecord2  Pointer to second record.
 */
SHORT  kTaskMgrContainerBase::sortCallBack(kCnrMiniRecord *pRecord1, kCnrMiniRecord *pRecord2)
{
    if (iSortColumn < cColumns && validateCnrInfo())
    {
        int i = iSortColumn;
        PFIELDINFO pfldinfo = queryDetailFieldInfo();
        for (i = iSortColumn; i > 0 && pfldinfo != NULL; i--)
            pfldinfo = pfldinfo->pNextFieldInfo;

        if (pfldinfo != NULL)
        {
            int iTmp = 0;
            if (pfldinfo->flData & CFA_STRING)
                iTmp = strcmp(*(PSZ*)((ULONG)pRecord1 + pfldinfo->offStruct), *(PSZ*)((ULONG)pRecord2 + pfldinfo->offStruct));
            else if (pfldinfo->flData & CFA_STRING)
                iTmp = (int)*(PULONG)((ULONG)pRecord1 + pfldinfo->offStruct) - (int)*(PULONG)((ULONG)pRecord2 + pfldinfo->offStruct);
            return (SHORT)((iTmp > 0) ? 1 : (iTmp < 0) ? -1 : 0);
        }
    }

    return 0;
}



/**
 * Constructor.
 * @param     hwndDlg   Handle to dialog window.
 * @param     ulCnrId   ID of the container dialog item in hwndDlg.
 *
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
kTaskMgrContainerBase::kTaskMgrContainerBase(HWND           hwndDlg,
                                             ULONG          ulCnrId,
                                             PSZ            pszTitle,
                                             ULONG          cColumns,
                                             PFIELDINFO     paFieldInfo,
                                             USHORT         usMenuCnrAllId,
                                             USHORT         usMenuRecordId /*= 0xffff*/,
                                             USHORT         usEnterId /*= 0xffff */,
                                             ULONG          iSortDefaultColumn /*= 0xffffffffUL */
                                             ) throw(kError)
    : kDetailCnr(WinWindowFromID(hwndDlg, ulCnrId), 0, pszTitle, cColumns, paFieldInfo, TRUE),
    pCurRecord(NULL), pMenuCnrAll(NULL), pMenuRecord(NULL),
    usEnterId(usEnterId), iSortColumn(iSortDefaultColumn), iSortDefaultColumn(iSortDefaultColumn)
{
    TRACELOG_CONSTRUCT("start");

    /*
     * Create menus
     */
    pMenuCnrAll = new kMenuBase(usMenuCnrAllId, NULLHANDLE, hwndCnr);
    if (usMenuRecordId && usMenuRecordId < 0xffff)
        pMenuRecord = new kMenuBase(usMenuRecordId, NULLHANDLE, hwndCnr);

    /*
     * Build sort menu from the field info structure.
     */
    PFIELDINFO  pfldinfo = queryDetailFieldInfo();
    MENUITEM    mi;
    HWND        hwndSortMenu;
    WinSendMsg(pMenuCnrAll->getHwnd(),
               MM_QUERYITEM,
               MPFROM2SHORT(IDM_CNR_ALL_SORT, TRUE),
               &mi);
    hwndSortMenu = mi.hwndSubMenu;

    if (pfldinfo != NULL && hwndSortMenu != NULLHANDLE)
    {
        int i = 0;
        while (pfldinfo != NULL)
        {
            mi.iPosition    = MIT_END;
            mi.afAttribute  = (USHORT)(i == iSortDefaultColumn ? MIA_CHECKED : 0);
            mi.id           = (USHORT)(i + IDM_CNR_ALL_SORT_FIRST);
            mi.hwndSubMenu  = NULLHANDLE;
            if (pfldinfo->flTitle & CFA_BITMAPORICON)
            {
                mi.afStyle  = MIS_BITMAP;
                mi.hItem    = (ULONG)pfldinfo->pTitleData;
                WinSendMsg(hwndSortMenu, MM_INSERTITEM, &mi, NULL);
            }
            else
            {
                CHAR   szMenuText[50];

                mi.afStyle  = MIS_TEXT;
                mi.hItem    = 0;
                if (pfldinfo->pTitleData != NULL)
                {
                    PSZ    psz;
                    int    j;
                    /*
                     * Format menu text. '\r\n' is removed and replaced by a single space.
                     * (TODO. Later a Ctrl+<n> could be inserted too.)
                     */
                    for (j = 0, psz = (PSZ)pfldinfo->pTitleData; *psz != '\0' && j < sizeof(szMenuText) - 1; psz++)
                    {
                        if (*psz != '\r' && *psz != '\n')
                            szMenuText[j++] = *psz;
                        else if (szMenuText[j-1] != ' ')
                            szMenuText[j++] = ' ';
                    }
                    szMenuText[j] = '\0';
                }
                else
                    sprintf(&szMenuText[0], "Column %d", i);
                if ((ULONG)WinSendMsg(hwndSortMenu, MM_INSERTITEM, &mi, &szMenuText[0]) > 42)
                {
                    kError PMError(kError::unused, kError::win);
                    PMError.showError("kTaskMgr - internal error");
                }

            }

            /* next */
            pfldinfo = pfldinfo->pNextFieldInfo;
            i++;
        }
    }

    /*
     * Enable default sorting.
     */
    if (iSortDefaultColumn < (IDM_CNR_ALL_SORT_LAST - IDM_CNR_ALL_SORT_FIRST))
        enableSorting();

    TRACELOG_CONSTRUCT("end");
}


/**
 * Destructor.
 */
kTaskMgrContainerBase::~kTaskMgrContainerBase()
{
    TRACELOG_DESTRUCT("start");
    if (pMenuCnrAll)
        delete pMenuCnrAll;
    pMenuCnrAll = NULL;

    if (pMenuRecord)
        delete pMenuRecord;
    pMenuRecord = NULL;
    TRACELOG_DESTRUCT("end");
}


/**
 * Displays the popup menu for the container.
 * @param     usId     Container id.
 * @param     pRecord  Pointer to the record which is selected by either the key
 */
VOID kTaskMgrContainerBase::cnrContextMenu(USHORT usId, PRECORDCORE pRecord)
{
    if (pMenuCnrAll || pMenuRecord)
    {
        pCurRecord = (kTaskMgrRecordBase *)pRecord;
        setRecordEmphasis(pCurRecord, TRUE, CRA_SOURCE);
        if (pRecord == NULL)
        {
            if (pMenuCnrAll != NULL && !pMenuCnrAll->popup(hwndCnr))
            {
                kError PMError(0, kError::win);
                PMError.showError("kTaskMgr - Internal error");
            }
        }
        else
        {
            if (pMenuRecord != NULL && !pMenuRecord->popup(hwndCnr))
            {
                kError PMError(0, kError::win);
                PMError.showError("kTaskMgr - Internal error");
            }
        }
    }
    usId = usId;
}


/**
 * Enter or double click on record in the container.
 * This action will bring up the detail dialog for the record.
 */
VOID kTaskMgrContainerBase::cnrEnter(USHORT usId, HWND hwndCnr, PRECORDCORE pRecord, ULONG fKey)
{
    if (pRecord != NULL)
    {
        pCurRecord = (kTaskMgrRecordBase *)pRecord;
        if (usEnterId != 0xffff)
            command(usEnterId, 0, 0);
    }
    usId = usId;
    fKey = fKey;
    hwndCnr = hwndCnr;
}


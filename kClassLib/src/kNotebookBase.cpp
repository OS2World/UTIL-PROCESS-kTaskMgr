/* $Id: kNotebookBase.cpp,v 1.1 2000/05/23 00:00:54 stknut Exp $
 *
 * kNotebookBase (kClassLib) - Notebook dialog base class.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */

/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
#define INCL_WIN
#define INCL_DOS
#define INCL_GPI

/*******************************************************************************
*   Internal Functions                                                         *
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

#include "kBase.h"
#include "kError.h"
#include "kDlgBase.h"
#include "kNotebookPageBase.h"
#include "kNotebookBase.h"



/**
 * Gets a page (pointer) from a Page id.
 * @returns   Pointer to page object. NULL if not found or error.
 * @param     ulPageId  Page id.
 */
kNotebookPageBase * kNotebookBase::queryPageFromId(ULONG ulPageId)
{
    int i;
    for (i = 0; i < cPages; i++)
        if (paPages[i].ulPageId == ulPageId)
            return paPages[i].pPage;
    return NULL;
}



/**
 * Gets a page (pointer) from a page handle (hwnd).
 * @returns   Pointer to page object. NULL if not found or error.
 * @param     hwndPage  Window handle of the page.
 */
kNotebookPageBase * kNotebookBase::queryPageFromHwnd(HWND hwndPage)
{
    int i;
    for (i = 0; i < cPages; i++)
        if (paPages[i].hwnd == hwndPage)
            return paPages[i].pPage;
    return NULL;
}


/**
 * Handler of notebook specific controls.
 * Control message.
 * @param     usId           Control id.
 * @param     usNotifyCode   Notification code.
 * @param     ulControlSpec  Controlspecific paramenter.
 */
ULONG kNotebookBase::control(USHORT id, USHORT usNotifyCode, ULONG ulControlSpec)
{
    /* If this is the notebook id we'll handle the notifications. */
    if (id == idNotebook)
        switch (usNotifyCode)
        {
            case BKN_HELP:
                ntbkHelp(ulControlSpec);
                return 0UL;

            case BKN_NEWPAGESIZE:
                ntbkNewPageSize();
                return 0UL;

            case BKN_PAGEDELETED:
                ntbkPageDeleted((PDELETENOTIFY)ulControlSpec);
                return 0UL;

            case BKN_PAGESELECTED:
                ntbkPageSelected((PPAGESELECTNOTIFY)ulControlSpec);
                return 0UL;

            case BKN_PAGESELECTEDPENDING:
                ntbkPageSelectedPending((PPAGESELECTNOTIFY)ulControlSpec);
                return 0UL;
        }
    return kDlgBase::control(id, usNotifyCode, ulControlSpec);
}


/**
 * Resizes the notebook according to the size changes.
 * Note! Forwarding messages to the default dialog proc is probably useful!
 * @param     pswp          New windowframe state.
 */
BOOL  kNotebookBase::adjustWindowPos(PSWP pswp)
{
    /* if size change resize notebook. */
    if (pswp->fl & (SWP_SIZE | SWP_RESTORE | SWP_MAXIMIZE))
    {
        SWP     swp;

        if (pswp->fl & SWP_SIZE)
            memcpy(&swp, pswp, sizeof(swp));
        else if (pswp->fl & SWP_MAXIMIZE)
            WinGetMaxPosition(hwnd, &swp);
        else
        {
            swp.cx = WinQueryWindowUShort(hwnd, QWS_CXRESTORE);
            swp.cy = WinQueryWindowUShort(hwnd, QWS_CYRESTORE);
        }

        WinSetWindowPos(hwndNtbk, HWND_TOP,
                        ptlBorder.x,
                        ptlBorder.y,
                        swp.cx - ptlBorder.x*2,
                        swp.cy - ptlBorder.y*2 - cyTitleBar,
                        SWP_MOVE | SWP_SIZE);
    }

    return kDlgBase::adjustWindowPos(pswp);
}


/**
 * Resizes the notebook according to the size changes.
 * @param     cxOld  Old width.
 * @param     cyOld  Old height.
 * @param     cxNew  New width.
 * @param     cyNew  New height
 * @remark    ARG!!! This event don't occure for dialogs it seems!
 */
VOID kNotebookBase::size(SHORT cxOld, SHORT cyOld, SHORT cxNew, SHORT cyNew)
{
    /*
    SWP swp;
    if (WinQueryWindowPos(hwndNtbk, &swp))
    {
        WinSetWindowPos(hwndNtbk, HWND_TOP,
                        0, 0,
                        cxNew - cxOld + swp.cx,
                        cyNew - cyOld + swp.cy,
                        SWP_MOVE | SWP_SIZE);
    }
    */
    cxOld = cxOld;
    cyOld = cyOld;
    cxNew = cxNew;
    cyNew = cyNew;
}


/**
 * Help is requested.
 * @param     ulPageId  Page id of the focus page.
 */
VOID  kNotebookBase::ntbkHelp(ULONG ulPageId)
{
    kNotebookPageBase *pPage;
    pPage = queryPageFromId(ulPageId);
    if (pPage != NULL)
        pPage->ntfyHelp();
}


/**
 * All pages are notified that the size of the notebook has changed.
 */
VOID  kNotebookBase::ntbkNewPageSize()
{
    int     i;
    BOOL    fRet;
    SWP     swp;
    if (cPages != 0)
    {
        fRet = WinQueryWindowPos(WinQueryWindow(paPages[0].hwnd, QW_PARENT), &swp);
        ASSERT(fRet);

        for (i = 0; i < cPages; i++)
            paPages[i].pPage->ntfySized(swp.cx, swp.cy);

        fRet = fRet;
    }
}


/**
 * Removes the page from the pagetable.
 * @param     pDeleteNotify  Pointer to a delete notify struct.
 */
VOID  kNotebookBase::ntbkPageDeleted(PDELETENOTIFY pDeleteNotify)
{
    int i;
    for (i = 0; i < cPages; i++)
        if (paPages[i].hwnd == pDeleteNotify->hwndPage)
        {   /* found page index, shift down the next pages one entry.*/
            cPages--;
            for (i = i; i < cPages; i++)
                memcpy(&paPages[i], &paPages[i+1], sizeof(paPages[0]));
        }
}


/**
 * Notify the page(s) that they have been selected and deselected.
 * @param     pPageSelectNotify  Notify select struct.
 */
VOID  kNotebookBase::ntbkPageSelected(PPAGESELECTNOTIFY pPageSelectNotify)
{
    kNotebookPageBase *pPage;

    /* the deselected page first */
    pPage = queryPageFromId(pPageSelectNotify->ulPageIdCur);
    if (pPage != NULL)
        pPage->ntfyDeselected();

    /* the selected page last */
    pPage = queryPageFromId(pPageSelectNotify->ulPageIdNew);
    if (pPage != NULL)
        pPage->ntfySelected();
}


/**
 * Notify that a page switch is to be done soon.
 * Check if the current page is willing to loose focus.
 * If it's not willing Then cancel page switch.
 * If it's willing Then ask the new page if it may receive focus.
 * Cancel if it wont have the focus.
 * @param     pPageSelectNotify  Notify select struct.
 */
VOID  kNotebookBase::ntbkPageSelectedPending(PPAGESELECTNOTIFY pPageSelectNotify)
{
    BOOL               fOK = TRUE;
    kNotebookPageBase *pPage;

    /* the deselected page */
    pPage = queryPageFromId(pPageSelectNotify->ulPageIdCur);
    if (pPage != NULL)
        fOK = pPage->ntfyDeselectedPending();

    /* the selected page */
    if (fOK)
    {
        pPage = queryPageFromId(pPageSelectNotify->ulPageIdNew);
        if (pPage != NULL)
            fOK = pPage->ntfySelectedPending();
    }

    /* Cancel the page switching? */
    if (!fOK)
        pPageSelectNotify->ulPageIdNew = 0UL;
}


/**
 * Create a notebook from a dialog template.
 * @param     usResId     Id of the dialog template.
 * @param     hmodRes     Resource module handle. (NULLHANDLE is current .EXE)
 * @param     idNotebook  Notebook control id.
 * @param     hwndOwner   Owner window
 * @param     hwndParent  Parent window.
 * @remark    not implemented.
 */
kNotebookBase::kNotebookBase(ULONG ulResId, HMODULE hmodRes, ULONG idNotebook, HWND hwndOwner,
                             HWND hwndParent/* = HWND_DESKTOP*/) throw (kError)
: kDlgBase(ulResId, hmodRes, hwndOwner, hwndParent),
    paPages(NULL), cPages(0), idNotebook((USHORT)idNotebook)
{
    TRACELOG_CONSTRUCT("start");

    hwndNtbk   = WinWindowFromID(hwnd, idNotebook);
    if (WinWindowFromID(hwnd, FID_TITLEBAR) != NULLHANDLE)
        cyTitleBar = (SHORT)WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR);
    else
        cyTitleBar = 0;
    WinSendMsg(hwnd, WM_QUERYBORDERSIZE, &ptlBorder, NULL);

    TRACELOG_CONSTRUCT("end");
}




/**
 * Create a notebook from a dialog template.
 * @param     pszTitle          Dialog title.
 * @param     id                Window id.
 * @param     flDialogStyle     Dialog style flags.
 * @param     flDialogCreate    Dialog (frame) create flags.
 * @param     x                 x-coordinate.
 * @param     y                 y-coordinate.
 * @param     cx                width
 * @param     cy                hight
 * @param     flNotebookStyle   Notebook style flags.
 * @param     hwndOwner         Owner window
 * @param     hwndParent        Parent window.
 * @remark    not implemented.
 */
kNotebookBase::kNotebookBase(PCSZ pszTitle, ULONG id,  ULONG flDialogStyle, ULONG flDialogCreate,
                             LONG x, LONG y, LONG cx, LONG cy,
                             ULONG flNotebookStyle, HWND hwndOwner, HWND hwndParent/* = HWND_DESKTOP*/) throw (kError)
: kDlgBase(pszTitle, id, flDialogStyle, flDialogCreate, x, y, cx, cy, hwndOwner, hwndParent),
cPages(0), paPages(NULL)
{
    TRACELOG_CONSTRUCT("start");
    RECTL rectl;
    queryRectangle(&rectl);

    /* create the notebook control */
    idNotebook = 100;
    hwndNtbk = WinCreateWindow(
                              hwnd,
                              WC_NOTEBOOK,
                              NULL,
                              flNotebookStyle,
                              0,
                              0,
                              rectl.xRight - rectl.xLeft,
                              rectl.yTop   - rectl.yBottom,
                              hwnd,
                              HWND_TOP,
                              idNotebook,
                              NULL,
                              NULL);
    if (hwndNtbk == NULLHANDLE)
        throw(kError(kError::unused, kError::win));

    /*
     * Get border size and height of the titlebar.
     */
    if (flDialogCreate & FCF_TITLEBAR)
        cyTitleBar = (SHORT)WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR);
    else
        cyTitleBar = 0;
    WinSendMsg(hwnd, WM_QUERYBORDERSIZE, &ptlBorder, NULL);

    TRACELOG_CONSTRUCT("end");
}



/**
 * Destructor.
 */
kNotebookBase::~kNotebookBase()
{
    TRACELOG_DESTRUCT("start");
    int i;
    /* Do not destroy pages, PM will destroy these. */
    /* free page table */
    if (paPages)
        free(paPages);
    paPages = NULL;
    TRACELOG_DESTRUCT("end");
}





/**
 * Inserts a page into the notebook.
 * @returns   success indicator. TRUE / FALSE.
 * @param     usPageStyle  See PMREF.
 * @param     usPageOrder  See PMREF.
 * @param     ulPageId     See PMREF.
 */
BOOL    kNotebookBase::insertPage(kNotebookPageBase *pNtbkPage,
                                  USHORT usPageStyle,
                                  USHORT usPageOrder,
                                  ULONG  ulPageId/* = 0UL*/)
{
    ULONG   aulOS2Ver[2];
    SWP     swp;
    CHAR    szTabText[64];

    /*
     * Reallocate the pages table to make space for the new page.
     */
    PVOID pvaPages = realloc(paPages, (cPages+1) * sizeof(paPages[0]));

    if (pvaPages == NULL)
        return FALSE;
    paPages = (struct _PageEntry*) pvaPages;


    /*
     * Insert the page in the notebook.
     * Add it to paPages too. (Don't "commit" it)
     */
    paPages[cPages].pPage    = pNtbkPage;
    paPages[cPages].hwnd     = pNtbkPage->getHwnd();
    paPages[cPages].ulPageId = (ULONG)WinSendMsg(hwndNtbk,
                                                 BKM_INSERTPAGE,
                                                 MPFROMLONG(ulPageId),
                                                 MPFROM2SHORT(usPageStyle, usPageOrder));
    if (paPages[cPages].ulPageId == 0)
        return FALSE;

    /*
     * Associate the the kNtbkPage window with the new page.
     */
    if (!(BOOL)WinSendMsg(hwndNtbk, BKM_SETPAGEWINDOWHWND,
                          MPFROMLONG(paPages[cPages].ulPageId),
                          MPFROMHWND(paPages[cPages].hwnd))
       )
    {
        WinSendMsg(hwndNtbk, BKM_DELETEPAGE,
                   MPFROMLONG(paPages[cPages].ulPageId),
                   MPFROMLONG(BKA_SINGLE));
        return FALSE;
    }

    /*
     * Set the notebook as owner of the page.
     */
    WinSetOwner(pNtbkPage->getHwnd(), hwndNtbk);

    /*
     * Set tab text.
     */
    if (pNtbkPage->queryTabText(&szTabText[0], sizeof(szTabText)) != NULL)
        setTabText(paPages[cPages].ulPageId, &szTabText[0]);

    /*
     * Finally we commits the new page by incrementing the page count.
     */
    cPages++;

    /*
     * Addjust the tab size? (Warp 3 only)
     *      Get current tab size.
     *
     */
    DosQuerySysInfo(QSV_VERSION_MAJOR, QSV_VERSION_MINOR, &aulOS2Ver[0], sizeof(aulOS2Ver));
    if (aulOS2Ver[0] == 20 && aulOS2Ver[1] < 40)
    {
        static ULONG aclr[7] =
        {
            RGB_BLUE,
            RGB_GREEN,
            RGB_CYAN,
            RGB_RED,
            RGB_PINK,
            RGB_YELLOW,
            RGB_WHITE
        };

        WinSendMsg(hwndNtbk, BKM_SETDIMENSIONS,
                   MPFROM2SHORT(100, 20),
                   MPFROMSHORT(usPageStyle & (BKA_MAJOR | BKA_MINOR)));
        //if (usPageStyle & BKA_MAJOR)


        WinSendMsg(hwndNtbk, BKM_SETTABCOLOR,
                   (MPARAM)paPages[cPages].ulPageId,
                   (MPARAM)aclr[cPages % 7]);
    }

    /*
     * Send a hint of the page size.
     */
    if (WinQueryWindowPos(WinQueryWindow(paPages[cPages-1].hwnd, QW_PARENT), &swp))
        paPages[cPages-1].pPage->ntfySized(swp.cx, swp.cy);

    /*
     * Notify the page that it has been inserted.
     */
    paPages[cPages-1].pPage->ntfyInserted();

    return TRUE;
}


/**
 * Gets the size of the page(fPage=FALSE) or the notebook(fPage=TRUE).
 * @returns   success indicator.
 * @param     pRectl    Pointer to the RECTL structure that contains the coordinates of the rectangle.
 * @param     fPage     If the fPage parameter is TRUE, this structure contains the coordinates of
 *                      a notebook window on input, and on return it contains the coordinates of an
 *                      application page window.
 *                      If the fPage parameter is FALSE, this structure contains the coordinates of
 *                      an application page window on input, and on return it contains the
 *                      coordinates of a notebook window.
 */
BOOL kNotebookBase::queryPageRectangle(PRECTL pRectl, BOOL fPage)
{
    return(BOOL)WinSendMsg(hwndNtbk, BKM_CALCPAGERECT,
                           pRectl, MPFROMLONG(fPage));
}


/**
 * Sets the tab text for the specified page.
 * @returns   success indicator.
 * @param     ulPageId    Page identificator.
 * @param     pszTabText  Pointer to the tab text.
 */
BOOL kNotebookBase::setTabText(ULONG ulPageId, PSZ pszTabText)
{
    return (BOOL)WinSendMsg(hwndNtbk, BKM_SETTABTEXT,
                            MPFROMLONG(ulPageId), pszTabText);
}



/**
 * Addjusts the size of the tabs.
 * @returns   success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark    Changes the tab sizes.
 */
BOOL kNotebookBase::addjustTabSizes(VOID)
{
    HPS     hps;
    int     i;
    int     cxMaj = 0,
            cyMaj = 0,
            cxMin = 0,
            cyMin = 0;

    hps = WinGetPS(hwndNtbk);

    for (i = 0; i < cPages; i++)
    {
        CHAR    szTabText[256];
        if (paPages[i].pPage->queryTabText(szTabText, sizeof(szTabText)))
        {
            POINTL aptl[TXTBOX_COUNT];

            if (GpiQueryTextBox(hps,
                                strlen(szTabText),
                                &szTabText[0],
                                TXTBOX_COUNT,
                                &aptl[0]))
            {
                #if 0
                ULONG ulPageStyle = paPages[i].pPage->queryPageStyle();
                if ((ulPageStyle & BKA_MAJOR)
                    && ap)
                }

                if (us)
                {
                }
                #endif
            }
        }
    }

    return FALSE;
}


/**
 * Gets the page id for a given page.
 * @returns   Page id for the specified page.
 *            0UL on error.
 */
ULONG   kNotebookBase::queryPageId(const kNotebookPageBase * pPage) const
{
    for (int i = 0; i < cPages; i++)
    {
        if (paPages[i].pPage == pPage)
            return paPages[i].ulPageId;
    }

    return 0;
}


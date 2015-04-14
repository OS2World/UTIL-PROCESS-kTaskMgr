/* $Id: kDlgBase.cpp,v 1.1 2000/05/23 00:00:54 stknut Exp $
 *
 * kDlgBase (kClassLib) - Base class for all types of dialogs.
 *
 * Copyright (c) 1999-2000 knut st. osmundsen
 *
 */


/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
#define INCL_WINDIALOGS
#define INCL_WINERRORS
#define INCL_WINWINDOWMGR
#define INCL_WININPUT
#define INCL_WINSTDCNR
#define INCL_WINTIMER
#define INCL_WINFRAMEMGR
#define INCL_DOSERRORS
#define INCL_DOSRESOURCES


/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <os2.h>
#ifdef USE_KLIB
    #include <kAssert.h>
    #include <kLog.h>
    #include <kHeap.h>
#endif
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "kBase.h"
#include "kError.h"
#include "kDlgBase.h"


/*******************************************************************************
*   Internal Functions                                                         *
*******************************************************************************/
static MRESULT _System forwarder(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);



/*********/
/* stubs */
/*********/
#pragma info(none)
VOID  kDlgBase::cnrBeginEdit(USHORT usId, ULONG cb, HWND hwndCnr, PRECORDCORE pRecord, PFIELDINFO pFieldInfo, PSZ *ppszText, ULONG cbText, ULONG id)
{
}

VOID  kDlgBase::cnrCollapseTree(USHORT usId, PRECORDCORE pRecord)
{
}

VOID  kDlgBase::cnrContextMenu(USHORT usId, PRECORDCORE pRecord)
{
}

ULONG kDlgBase::cnrDragAfter(USHORT usId, PRECORDCORE pRecord, PDRAGINFO pDragInfo)
{
    return 0;
}

VOID  kDlgBase::cnrDragLeave(USHORT usId, PRECORDCORE pRecord, PDRAGINFO pDragInfo)
{
}

ULONG kDlgBase::cnrDragOver(USHORT usId, PRECORDCORE pRecord, PDRAGINFO pDragInfo)
{
    return 0;
}

VOID  kDlgBase::cnrDrop(USHORT usId, PRECORDCORE pRecord, PDRAGINFO pDragInfo)
{
}

VOID  kDlgBase::cnrDropNotify(USHORT usId, PRECORDCORE pRecord, PDRAGINFO pDragInfo, HWND hwndTarget)
{
}

VOID  kDlgBase::cnrDropHelp(USHORT usId, PRECORDCORE pRecord, PDRAGINFO pDragInfo)
{
}

VOID  kDlgBase::cnrEmphasis(USHORT usId, HWND hwndCnr, PRECORDCORE pRecord, ULONG fEmphasisMask)
{
}

VOID  kDlgBase::cnrEndEdit(USHORT usId, ULONG cb, HWND hwndCnr, PRECORDCORE pRecord, PFIELDINFO pFieldInfo, PSZ *ppszText, ULONG cbText, ULONG id)
{
}

VOID  kDlgBase::cnrEnter(USHORT usId, HWND hwndCnr, PRECORDCORE pRecord, ULONG fKey)
{
}

VOID  kDlgBase::cnrExpandTree(USHORT usId, PRECORDCORE pRecord)
{
}

VOID  kDlgBase::cnrGridResized(USHORT usId, SHORT sGridRows, SHORT sGridCols)
{
}

VOID  kDlgBase::cnrHelp(USHORT usId, PRECORDCORE pRecord)
{
}

VOID  kDlgBase::cnrInitDrag(USHORT usId, HWND hwndCnr, PRECORDCORE pRecord, LONG x, LONG y, LONG cx, LONG cy)
{
}

VOID  kDlgBase::cnrKillFocus(USHORT usId, HWND hwndCnr)
{
}

VOID  kDlgBase::cnrPickup(USHORT usId, HWND hwndCnr, PRECORDCORE pRecord, LONG x, LONG y, LONG cx, LONG cy)
{
}

VOID  kDlgBase::cnrQueryDelta(USHORT usId, HWND hwndCnr, ULONG fDelta)
{
}

VOID  kDlgBase::cnrReallocPSZ(USHORT usId, ULONG cb, HWND hwndCnr, PRECORDCORE pRecord, PFIELDINFO pFieldInfo, PSZ *ppszText, ULONG cbText, ULONG id)
{
}

VOID  kDlgBase::cnrScroll(USHORT usId, HWND hwndCnr, LONG lScrollInc, ULONG fScroll)
{
}

VOID  kDlgBase::cnrSetFocus(USHORT usId, HWND hwndCnr)
{
}

#pragma info(restore)



/**
 * Control message.
 * @param     id             Control id.
 * @param     usNotifyCode   Notification code.
 * @param     ulControlSpec  Controlspecific paramenter.
 */
ULONG kDlgBase::controlIntern(USHORT usId, USHORT usNotifyCode, ULONG ulControlSpec)
{
    char   szClassName[20];
    LONG   cch;
    HWND   hwndCtrl;
    ULONG  ulClassNumber;
    ULONG  ulRet = 0;

    hwndCtrl = WinWindowFromID(hwnd, usId);
    if (hwndCtrl != NULLHANDLE
        && (cch = WinQueryClassName(hwndCtrl, sizeof(szClassName), &szClassName[0]))
        && cch > 0 && cch <= 4
        && szClassName[0] == '#'
        && (ulClassNumber = atoi(&szClassName[1])) != 0)
    {
        ulClassNumber |= 0xffff0000L;
        switch (ulClassNumber)
        {
            /* container notification messages */
         #define PSZ ULONG //#define WC_CONTAINER         ((PSZ)0xffff0025L)
            case WC_CONTAINER:
         #undef PSZ
                switch (usNotifyCode)
                {
                    case CN_BEGINEDIT:
                        cnrBeginEdit(usId,
                                     ((PCNREDITDATA)ulControlSpec)->cb,
                                     ((PCNREDITDATA)ulControlSpec)->hwndCnr,
                                     ((PCNREDITDATA)ulControlSpec)->pRecord,
                                     ((PCNREDITDATA)ulControlSpec)->pFieldInfo,
                                     ((PCNREDITDATA)ulControlSpec)->ppszText,
                                     ((PCNREDITDATA)ulControlSpec)->cbText,
                                     ((PCNREDITDATA)ulControlSpec)->id);
                        break;

                    case CN_COLLAPSETREE:
                        cnrCollapseTree(usId, (PRECORDCORE)ulControlSpec);
                        break;

                    case CN_CONTEXTMENU:
                        cnrContextMenu(usId, (PRECORDCORE)ulControlSpec);
                        break;

                    case CN_DRAGAFTER:
                        ulRet = cnrDragAfter(usId,
                                             ((PCNRDRAGINFO)ulControlSpec)->pRecord,
                                             ((PCNRDRAGINFO)ulControlSpec)->pDragInfo);
                        break;

                    case CN_DRAGLEAVE:
                        cnrDragLeave(usId,
                                     ((PCNRDRAGINFO)ulControlSpec)->pRecord,
                                     ((PCNRDRAGINFO)ulControlSpec)->pDragInfo);
                        break;

                    case CN_DRAGOVER:
                        ulRet = cnrDragOver(usId,
                                            ((PCNRDRAGINFO)ulControlSpec)->pRecord,
                                            ((PCNRDRAGINFO)ulControlSpec)->pDragInfo);
                        break;

                    case CN_DROP:
                        cnrDrop(usId,
                                ((PCNRDRAGINFO)ulControlSpec)->pRecord,
                                ((PCNRDRAGINFO)ulControlSpec)->pDragInfo);
                        break;

                    case CN_DROPNOTIFY:
                        cnrDropNotify(usId,
                                      ((PCNRLAZYDRAGINFO)ulControlSpec)->pRecord,
                                      ((PCNRLAZYDRAGINFO)ulControlSpec)->pDragInfo,
                                      ((PCNRLAZYDRAGINFO)ulControlSpec)->hwndTarget);
                        break;

                    case CN_DROPHELP:
                        cnrDropHelp(usId,
                                    ((PCNRDRAGINFO)ulControlSpec)->pRecord,
                                    ((PCNRDRAGINFO)ulControlSpec)->pDragInfo);
                        break;

                    case CN_EMPHASIS:
                        cnrEmphasis(usId,
                                    ((PNOTIFYRECORDEMPHASIS)ulControlSpec)->hwndCnr,
                                    ((PNOTIFYRECORDEMPHASIS)ulControlSpec)->pRecord,
                                    ((PNOTIFYRECORDEMPHASIS)ulControlSpec)->fEmphasisMask);
                        break;

                    case CN_ENDEDIT:
                        cnrEndEdit(usId,
                                   ((PCNREDITDATA)ulControlSpec)->cb,
                                   ((PCNREDITDATA)ulControlSpec)->hwndCnr,
                                   ((PCNREDITDATA)ulControlSpec)->pRecord,
                                   ((PCNREDITDATA)ulControlSpec)->pFieldInfo,
                                   ((PCNREDITDATA)ulControlSpec)->ppszText,
                                   ((PCNREDITDATA)ulControlSpec)->cbText,
                                   ((PCNREDITDATA)ulControlSpec)->id);
                        break;

                    case CN_ENTER:
                        cnrEnter(usId,
                                 ((PNOTIFYRECORDENTER)ulControlSpec)->hwndCnr,
                                 ((PNOTIFYRECORDENTER)ulControlSpec)->pRecord,
                                 ((PNOTIFYRECORDENTER)ulControlSpec)->fKey);
                        break;

                    case CN_EXPANDTREE:
                        cnrExpandTree(usId, (PRECORDCORE)ulControlSpec);
                        break;

                    case CN_GRIDRESIZED:
                        cnrGridResized(usId, SHORT1FROMMP(ulControlSpec), SHORT2FROMMP(ulControlSpec));
                        break;

                    case CN_HELP:
                        cnrHelp(usId, (PRECORDCORE)ulControlSpec);
                        break;

                    case CN_INITDRAG:
                        cnrInitDrag(usId,
                                    ((PCNRDRAGINIT)ulControlSpec)->hwndCnr,
                                    ((PCNRDRAGINIT)ulControlSpec)->pRecord,
                                    ((PCNRDRAGINIT)ulControlSpec)->x,
                                    ((PCNRDRAGINIT)ulControlSpec)->y,
                                    ((PCNRDRAGINIT)ulControlSpec)->cx,
                                    ((PCNRDRAGINIT)ulControlSpec)->cy);
                        break;

                    case CN_KILLFOCUS:
                        cnrKillFocus(usId, (HWND)ulControlSpec);
                        break;

                    case CN_PICKUP:
                        cnrPickup(usId,
                                  ((PCNRDRAGINIT)ulControlSpec)->hwndCnr,
                                  ((PCNRDRAGINIT)ulControlSpec)->pRecord,
                                  ((PCNRDRAGINIT)ulControlSpec)->x,
                                  ((PCNRDRAGINIT)ulControlSpec)->y,
                                  ((PCNRDRAGINIT)ulControlSpec)->cx,
                                  ((PCNRDRAGINIT)ulControlSpec)->cy);
                        break;

                    case CN_QUERYDELTA:
                        cnrQueryDelta(usId,
                                      ((PNOTIFYDELTA)ulControlSpec)->hwndCnr,
                                      ((PNOTIFYDELTA)ulControlSpec)->fDelta);
                        break;

                    case CN_REALLOCPSZ:
                        cnrReallocPSZ(usId,
                                      ((PCNREDITDATA)ulControlSpec)->cb,
                                      ((PCNREDITDATA)ulControlSpec)->hwndCnr,
                                      ((PCNREDITDATA)ulControlSpec)->pRecord,
                                      ((PCNREDITDATA)ulControlSpec)->pFieldInfo,
                                      ((PCNREDITDATA)ulControlSpec)->ppszText,
                                      ((PCNREDITDATA)ulControlSpec)->cbText,
                                      ((PCNREDITDATA)ulControlSpec)->id);
                        break;

                    case CN_SCROLL:
                        cnrScroll(usId,
                                  ((PNOTIFYSCROLL)ulControlSpec)->hwndCnr,
                                  ((PNOTIFYSCROLL)ulControlSpec)->lScrollInc,
                                  ((PNOTIFYSCROLL)ulControlSpec)->fScroll);
                        break;

                    case CN_SETFOCUS:
                        cnrSetFocus(usId, (HWND)ulControlSpec);
                        break;

                    default:
                        ulRet = control(usId, usNotifyCode, ulControlSpec);
                        break;
                }
                break; /*case WC_CONTAINER*/

            default:
                ulRet = control(usId, usNotifyCode, ulControlSpec);
                break;
        }/* switch (ulClassNumber) */
      #undef PSZ

    }
    else
        ulRet = control(usId, usNotifyCode, ulControlSpec);

    return ulRet;
}


/**
 * Start timer (interface to WinStartTimer).
 * @returns   Timer id. 0 on error.
 * @param     idTimer       Requested timer id.
 * @param     idTimeout     Time out (ms).
 */
ULONG kDlgBase::timerStart(ULONG idTimer, ULONG dtTimeout)
{
    return WinStartTimer(WinQueryAnchorBlock(hwnd), hwnd, idTimer, dtTimeout);
}



/**
 * Stop timer (interface to WinStopTimer).
 * @returns   success indicator.
 * @param     idTimer  Id of timer to stop.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL kDlgBase::timerStop(ULONG idTimer)
{
    return WinStopTimer(WinQueryAnchorBlock(hwnd), hwnd, idTimer);
}





/**
 * button click or dbl click event.
 * @returns   TRUE if event was handled, FALSE if not.
 * @param     ptsPointerPos  Pointer posision relative to this window.
 * @param     fsHitTestres   Hit test result.
 * @param     fsflags
 * @remark    stub.
 */
BOOL kDlgBase::button1Click(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags)
{
    return(BOOL)WinDefDlgProc(hwnd, WM_BUTTON1CLICK,
                              *(MPARAM *)&ptsPointerPos,
                              MPFROM2SHORT(fsHitTestres, fsFlags));
}



/**
 * button click or dbl click event.
 * @returns   TRUE if event was handled, FALSE if not.
 * @param     ptsPointerPos  Pointer posision relative to this window.
 * @param     fsHitTestres   Hit test result.
 * @param     fsflags
 * @remark    stub.
 */
BOOL kDlgBase::button1DblClick(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags)
{
    return(BOOL)WinDefDlgProc(hwnd, WM_BUTTON1DBLCLK,
                              *(MPARAM *)&ptsPointerPos,
                              MPFROM2SHORT(fsHitTestres, fsFlags));
}



/**
 * button click or dbl click event. WM_BUTTON1DOWN
 * @returns   TRUE if event was handled, FALSE if not.
 * @param     ptsPointerPos  Pointer posision relative to this window.
 * @param     fsHitTestres   Hit test result.
 * @param     fsflags
 * @remark    stub.
 */
BOOL kDlgBase::button1Down(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags)
{
    return(BOOL)WinDefDlgProc(hwnd, WM_BUTTON1DOWN,
                              *(MPARAM *)&ptsPointerPos,
                              MPFROM2SHORT(fsHitTestres, fsFlags));
}



/**
 * button click or dbl click event.
 * @returns   TRUE if event was handled, FALSE if not.
 * @param     ptsPointerPos  Pointer posision relative to this window.
 * @param     fsHitTestres   Hit test result.
 * @param     fsflags
 * @remark    stub.
 */
BOOL kDlgBase::button2Click(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags)
{
    return(BOOL)WinDefDlgProc(hwnd, WM_BUTTON2CLICK,
                              MPFROM2SHORT(ptsPointerPos.x, ptsPointerPos.y),
                              MPFROM2SHORT(fsHitTestres, fsFlags));
}



/**
 * button click or dbl click event.
 * @returns   TRUE if event was handled, FALSE if not.
 * @param     ptsPointerPos  Pointer posision relative to this window.
 * @param     fsHitTestres   Hit test result.
 * @param     fsflags
 * @remark    stub.
 */
BOOL kDlgBase::button2DblClick(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags)
{
    return(BOOL)WinDefDlgProc(hwnd, WM_BUTTON2DBLCLK,
                              MPFROM2SHORT(ptsPointerPos.x, ptsPointerPos.y),
                              MPFROM2SHORT(fsHitTestres, fsFlags));
}



/**
 * button click or dbl click event.
 * @returns   TRUE if event was handled, FALSE if not.
 * @param     ptsPointerPos  Pointer posision relative to this window.
 * @param     fsHitTestres   Hit test result.
 * @param     fsflags
 * @remark    stub.
 */
BOOL kDlgBase::button2Down(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags)
{
    return(BOOL)WinDefDlgProc(hwnd, WM_BUTTON2DOWN,
                              MPFROM2SHORT(ptsPointerPos.x, ptsPointerPos.y),
                              MPFROM2SHORT(fsHitTestres, fsFlags));
}



/**
 * button click or dbl click event.
 * @returns   TRUE if event was handled, FALSE if not.
 * @param     ptsPointerPos  Pointer posision relative to this window.
 * @param     fsHitTestres   Hit test result.
 * @param     fsflags
 * @remark    stub.
 */
BOOL kDlgBase::button3Click(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags)
{
    return(BOOL)WinDefDlgProc(hwnd, WM_BUTTON3CLICK,
                              MPFROM2SHORT(ptsPointerPos.x, ptsPointerPos.y),
                              MPFROM2SHORT(fsHitTestres, fsFlags));
}



/**
 * button click or dbl click event.
 * @returns   TRUE if event was handled, FALSE if not.
 * @param     ptsPointerPos  Pointer posision relative to this window.
 * @param     fsHitTestres   Hit test result.
 * @param     fsflags
 * @remark    stub.
 */
BOOL kDlgBase::button3DblClick(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags)
{
    return(BOOL)WinDefDlgProc(hwnd, WM_BUTTON3DBLCLK,
                              MPFROM2SHORT(ptsPointerPos.x, ptsPointerPos.y),
                              MPFROM2SHORT(fsHitTestres, fsFlags));
}



/**
 * button click or dbl click event.
 * @returns   TRUE if event was handled, FALSE if not.
 * @param     ptsPointerPos  Pointer posision relative to this window.
 * @param     fsHitTestres   Hit test result.
 * @param     fsflags
 * @remark    stub.
 */
BOOL kDlgBase::button3Down(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags)
{
    return(BOOL)WinDefDlgProc(hwnd, WM_BUTTON3DOWN,
                              MPFROM2SHORT(ptsPointerPos.x, ptsPointerPos.y),
                              MPFROM2SHORT(fsHitTestres, fsFlags));
}



/**
 * Default command.
 * @param     usCmd     Control id which send/posted the message.
 * @param     usSource  Source id.
 * @param     fPointer  Mouse pointer flag.
 * @remark    dismisses the dialog if DID_OK or DID_CANCEL.
 */
VOID kDlgBase::command(USHORT usCmd, USHORT usSource, BOOL fPointer)
{
    if (usCmd == DID_CANCEL || usCmd == DID_OK)
        close();

    /* unreferenced */
    usSource = usSource;
    fPointer = fPointer;
}



/**
 * Control message.
 * @param     usId           Control id.
 * @param     usNotifyCode   Notification code.
 * @param     ulControlSpec  Controlspecific paramenter.
 */
ULONG kDlgBase::control(USHORT usId, USHORT usNotifyCode, ULONG ulControlSpec)
{
    /* no action */
    /* unreferenced */
    usId = usId;
    usNotifyCode = usNotifyCode;
    ulControlSpec = ulControlSpec;
    return (ULONG)(PULONG)WinDefDlgProc(hwnd, WM_CONTROL, MPFROM2SHORT(usId, usNotifyCode), MPFROMLONG(ulControlSpec));
}



/**
 * Initdialog message.
 * @returns   TRUE  - on success.
 *            FALSE - on failiure.
 * @param     hwndFocus  Handle to the focus window.
 */
BOOL kDlgBase::initDlg(HWND hwndFocus)
{
    /* no action */

    /* unreferenced */
    hwndFocus = hwndFocus;

    return TRUE;
}



/**
 * Initmenu message. Enables and disables menuitems.
 * @param     usMenuId  Control id of menu.
 * @param     hwndMnu   WindowHandle of menu.
 */
VOID kDlgBase::initMenu(USHORT usMenuId, HWND hwndMnu)
{
    /* no action */

    /* unreferenced */
    usMenuId = usMenuId;
    hwndMnu = hwndMnu;
}



/**
 * Menuend message.
 * @param     usMenuId  Control id of menu.
 * @param     hwndMnu   WindowHandle of menu.
 */
VOID kDlgBase::menuEnd(USHORT usMenuId, HWND hwndMnu)
{
    /* no action */

    /* unreferenced */
    usMenuId = usMenuId;
    hwndMnu = hwndMnu;
}



/**
 * Syscomand message.
 * @param     usCmd     Control id which send/posted the message.
 * @param     usSource  Source id.
 * @param     fPointer  Mouse pointer flag.
 */
VOID kDlgBase::sysCommand(USHORT usCmd, USHORT usSource, BOOL fPointer)
{
    /* send to default window procedure */
    WinDefDlgProc(hwnd, WM_SYSCOMMAND, (MPARAM)usCmd, MPFROM2SHORT(usSource, fPointer));
}


/**
 * WM_CLOSE stub.
 */
VOID kDlgBase::close()
{
    /* send to default window procedure */
    if (fModal)
        WinDefDlgProc(hwnd, WM_CLOSE, NULL, NULL);
    else
        WinDestroyWindow(hwnd);
}



/**
 * WM_TIMER message stub.
 * @param     idTimer  Timer id.
 */
VOID kDlgBase::timer(ULONG idTimer)
{
    idTimer = idTimer;
}


/**
 * WM_SIZE message stub.
 * @param     cxOld  Old width.
 * @param     cyOld  Old height.
 * @param     cxNew  New width.
 * @param     cyNew  New height
 * @remark    Useless for dialogs!?!
 */
VOID kDlgBase::size(SHORT cxOld, SHORT cyOld, SHORT cxNew, SHORT cyNew)
{
    cxOld = cxOld;
    cyOld = cyOld;
    cxNew = cxNew;
    cyNew = cyNew;
}


/**
 * WM_ADJUSTWINDOWPOS stub.
 * @param     pswp          New windowframe state.
 */
BOOL  kDlgBase::adjustWindowPos(PSWP pswp)
{
    return (BOOL)WinDefDlgProc(hwnd, WM_ADJUSTWINDOWPOS, pswp, NULL);
}


/**
 * WM_ADJUSTFRAMEPOS stub.
 * @param     pswp          New windowframe state.
 * @param     hsavewphsvwp  Identifier of the frame window repositioning process.
 */
VOID  kDlgBase::adjustFramePos(PSWP pswp, HSAVEWP hsavewphsvwp)
{
    pswp = pswp;
    hsavewphsvwp = hsavewphsvwp;
}


/**
 * Creates an dialog from a resource dialogtemplate.
 * @param     ulResId      Resource id of the template.
 * @param     hmodRes      Module handle of the module containing the template.
 * @param     hwndOwner    Windowhandle of the owner window.
 * @param     hwndParent   Windowhandle of the parent window. Defaults to HWND_DESKTOP.
 * @remark    Throws kError on error.
 *            Use the show()/showModal() methods to show the dialog
 */
kDlgBase::kDlgBase(ULONG ulResId, HMODULE hmodRes, HWND hwndOwner, HWND hwndParent /*= HWND_DESKTOP*/) throw (kError)
: fModal(FALSE), fDestruction(FALSE)
{
    TRACELOG_CONSTRUCT("start");
    APIRET rc;

    /* get resource pointer */
    rc = DosGetResource(hmodRes, RT_DIALOG, ulResId, (PPVOID)&pDlgTemplate);
    if (rc != NO_ERROR)
        throw (kError(rc, kError::dos));

    /* create dialog from template */
    hwnd = WinCreateDlg(hwndParent, hwndOwner, forwarder, pDlgTemplate, this);
    if (hwnd == NULLHANDLE)
        throw (kError(kError::unused, kError::win));

    TRACELOG_CONSTRUCT("end");
}


/**
 * Creates an dialog from a resource dialogtemplate.
 * @param     pszTitle          Dialog title.
 * @param     id                Dialog id.
 * @param     flDialogStyle     Dialog style flags.
 * @param     flDialogCreate    Dialog (frame) create flags.
 * @param     x                 x-coordinate.
 * @param     y                 y-coordinate.
 * @param     cx                width
 * @param     cy                hight
 * @param     hwndOwner         Owner window
 * @param     hwndParent        Parent window.
 * @remark    Throws kError on error.
 *            Use the show()/showModal() methods to show the dialog
 */
kDlgBase::kDlgBase(PCSZ pszTitle, ULONG id, ULONG flDialogStyle, ULONG flDialogCreate,
                   LONG x, LONG y, LONG cx, LONG cy,
                   HWND hwndOwner, HWND hwndParent/* = HWND_DESKTOP */) throw (kError)
: fModal(FALSE), fDestruction(FALSE)
{
    TRACELOG_CONSTRUCT("start");

    int    cb;                          /* Size of the dialog template. */

    /* flDialogStyle should have WS_CLIPSIBLINGS and WS_SAVEBITS set! */
    flDialogStyle |= WS_CLIPSIBLINGS | WS_SAVEBITS;

    /* make a fake template for this dialog. */
    cb = sizeof(DLGTEMPLATE) + strlen(pszTitle) + 1 + sizeof(ULONG);
    pDlgTemplate = (PDLGTEMPLATE)calloc(cb, 1);
    if (pDlgTemplate == NULL)
        throw (kError(kError::alloc_failed));

    pDlgTemplate->cbTemplate        = (USHORT)cb;
    pDlgTemplate->offadlgti         = offsetof(DLGTEMPLATE, adlgti);
    pDlgTemplate->fsTemplateStatus  = 1;
    pDlgTemplate->iItemFocus        = (USHORT)~0;

    pDlgTemplate->adlgti[0].cchClassName = 0;
    pDlgTemplate->adlgti[0].offClassName = (USHORT)(ULONG)WC_FRAME;
    pDlgTemplate->adlgti[0].cchText      = (USHORT)strlen(pszTitle);
    pDlgTemplate->adlgti[0].offText      = (USHORT)sizeof(DLGTEMPLATE);
    strcpy((char*)(pDlgTemplate+1), pszTitle);
    pDlgTemplate->adlgti[0].flStyle      = flDialogStyle;
    pDlgTemplate->adlgti[0].id           = (USHORT)id;
    pDlgTemplate->adlgti[0].offCtlData   = (USHORT)(pDlgTemplate->adlgti[0].offText + strlen(pszTitle) + 1);
    *(PULONG)((unsigned)(pDlgTemplate+1) + pDlgTemplate->adlgti[0].offCtlData) = flDialogCreate;
    pDlgTemplate->adlgti[0].x       = (SHORT)x;
    pDlgTemplate->adlgti[0].y       = (SHORT)y;
    pDlgTemplate->adlgti[0].cx      = (SHORT)cx;
    pDlgTemplate->adlgti[0].cy      = (SHORT)cy;

    /*
     * Debug stuff
     */
    #if 0
    PDLGTEMPLATE pDlg2;
    DosGetResource(NULLHANDLE, RT_DIALOG, 100, (PPVOID)&pDlg2);
    #endif
    /* create dialog from template */
    hwnd = WinCreateDlg(hwndParent, hwndOwner, forwarder, pDlgTemplate, this);
    if (hwnd == NULLHANDLE)
        throw (kError(kError::unused, kError::win));

    TRACELOG_CONSTRUCT("end");
}


/**
 * Destroy the dialog and release the resource.
 */
kDlgBase::~kDlgBase()
{
    TRACELOG_DESTRUCT("start");

    if (hwnd != NULLHANDLE)
    {
        if (fModal && !(WinQueryWindowUShort(hwnd, QWS_FLAGS) & FF_DLGDISMISSED))
            WinDismissDlg(hwnd, DID_CANCEL);
        WinDestroyWindow(hwnd);
        hwnd = NULLHANDLE;
    }
    DosFreeResource(pDlgTemplate);
    pDlgTemplate = NULL;

    TRACELOG_DESTRUCT("end");
}



/**
 * Hides the dialog.
 * @returns   TRUE - the dialog was hidden.
 *            FALSE - something failed.
 */
BOOL kDlgBase::hide()
{
    return WinSetWindowPos(hwnd, NULLHANDLE, 0,0,0,0, SWP_HIDE);
}



/**
 * Shows the dialog - modelessly.
 * @returns   TRUE - the dialog was shown.
 *            FALSE - failed showing the dialog.
 */
BOOL kDlgBase::show()
{
    fModal = FALSE;
    return WinSetWindowPos(hwnd, NULLHANDLE, 0,0,0,0, SWP_SHOW);
}


/**
 * Shows the dialog modally.
 * @returns   WinProcessDlg return code.
 */
ULONG kDlgBase::showModal()
{
    fModal = TRUE;
    return WinProcessDlg(hwnd);
}


/**
 * Sets the dialog title.
 * @returns   Success indicator.
 * @param     pszTitle  Pointer to string containing the new title.
 */
BOOL  kDlgBase::setTitle(PCSZ pszTitle)
{
    return WinSetWindowText(hwnd, pszTitle);
}


/**
 * Resize the dialog.
 * @returns   Success indicator. TRUE / FALSE.
 * @param     cx  Size in x direction (pixels).
 * @param     cy  Size in y direction (pixels).
 */
BOOL kDlgBase::resize(LONG cx, LONG cy)
{
    return WinSetWindowPos(hwnd, NULLHANDLE, 0, 0, cx, cy, SWP_SIZE);
}


/**
 * Resize and mode the dialog.
 * @returns   Success indicator. TRUE / FALSE.
 * @param     x   x-coordinate for left lower corner.
 * @param     y   y-coordinate for left lower corner.
 * @param     cx  Size in x direction (pixels).
 * @param     cy  Size in y direction (pixels).
 */
BOOL kDlgBase::resizeAndMove(LONG x, LONG y, LONG cx, LONG cy)
{
    return WinSetWindowPos(hwnd, NULLHANDLE, x, y, cx, cy, SWP_SIZE | SWP_MOVE);
}


/**
 * Gets the size rectangle of the window.
 * @returns   Success indicator. TRUE / FALSE.
 * @param     prectl  Pointer to rectangle.
 */
BOOL kDlgBase::queryRectangle(PRECTL prectl)
{
    SWP swp;
    if (WinQueryWindowPos(hwnd, &swp))
    {
        prectl->xLeft   = swp.x;
        prectl->yBottom = swp.y;
        prectl->xRight  = swp.x + swp.cx;
        prectl->yTop    = swp.y + swp.cy;
        return TRUE;
    }
    return FALSE;
}

/**
 * Gets the window handler for this dialog.
 * @returns   Handle to dialog. NULLHANDE on error.
 */
HWND kDlgBase::getHwnd()
{
    return hwnd;
}



/**
 * Forwards messages to kDlgBaseMsgLoop if a QWS_USER is registered.
 * When WM_INITDLG, it stores the this pointer (mp2) in QWS_USER.
 * @returns   depended on msg. See PMREF for more.
 * @param     hwnd  Handle of the(this) window receiving the message.
 * @param     msg   Message id. See PMREF for more.
 * @param     mp1   Message paramenter one. See PMREF for more.
 * @param     mp2   Message paramenter two. See PMREF for more.
 * @remark    Forwarder.
 */
static MRESULT forwarder(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    PVOID pv;

    TRACELOG_MESSAGES(hwnd, msg, mp1, mp2);
    pv = (PVOID)WinQueryWindowULong(hwnd, QWL_USER);
    if (msg == WM_INITDLG)
    {
        pv = mp2;
        WinSetWindowULong(hwnd, QWL_USER, (ULONG)pv);
    }
    if (pv != NULL)
        return ((kDlgBase*)pv)->msgLoop(hwnd, msg, mp1, mp2);
    else
        return WinDefDlgProc(hwnd, msg, mp1, mp2);
}



/**
 * Message loop.
 * @returns   depended on msg. See PMREF for more.
 * @param     hwnd  Handle of the(this) window receiving the message.
 * @param     msg   Message id. See PMREF for more.
 * @param     mp1   Message paramenter one. See PMREF for more.
 * @param     mp2   Message paramenter two. See PMREF for more.
 */
MRESULT kDlgBase::msgLoop(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    MRESULT mr = NULL;
    switch (msg)
    {
        case WM_COMMAND:
            command(SHORT1FROMMP(mp1), SHORT1FROMMP(mp2), SHORT2FROMMP(mp2));
            break;

        case WM_CONTROL:
            mr = (MRESULT)controlIntern(SHORT1FROMMP(mp1), SHORT2FROMMP(mp1), (ULONG)mp2);
            break;

        case WM_DESTROY:
            #if 1
            WinSetWindowULong(hwnd, QWL_USER, NULL);
            if (!fDestruction && !fModal)
            {
                fDestruction = TRUE;
                this->hwnd = NULLHANDLE;
                delete this;
                return NULL;
            }
            #endif
            break;

        case WM_INITDLG:
            mr = (MRESULT)!initDlg((HWND)mp1);
            break;

        case WM_INITMENU:
            initMenu(SHORT1FROMMP(mp1), (HWND)mp2);
            break;

        case WM_MENUEND:
            menuEnd(SHORT1FROMMP(mp1), (HWND)mp2);
            break;

        case WM_BUTTON1CLICK:
            mr = (MRESULT)button1Click(*(PPOINTS)&mp1, SHORT1FROMMP(mp2), SHORT2FROMMP(mp2));
            break;

        case WM_BUTTON1DBLCLK:
            mr = (MRESULT)button1DblClick(*(PPOINTS)&mp1, SHORT1FROMMP(mp2), SHORT2FROMMP(mp2));
            break;

        case WM_BUTTON1DOWN:
            mr = (MRESULT)button1Down(*(PPOINTS)&mp1, SHORT1FROMMP(mp2), SHORT2FROMMP(mp2));
            break;

        case WM_BUTTON2CLICK:
            mr = (MRESULT)button2Click(*(PPOINTS)&mp1, SHORT1FROMMP(mp2), SHORT2FROMMP(mp2));
            break;

        case WM_BUTTON2DBLCLK:
            mr = (MRESULT)button2DblClick(*(PPOINTS)&mp1, SHORT1FROMMP(mp2), SHORT2FROMMP(mp2));
            break;

        case WM_BUTTON2DOWN:
            mr = (MRESULT)button2Down(*(PPOINTS)&mp1, SHORT1FROMMP(mp2), SHORT2FROMMP(mp2));
            break;

        case WM_BUTTON3CLICK:
            mr = (MRESULT)button3Click(*(PPOINTS)&mp1, SHORT1FROMMP(mp2), SHORT2FROMMP(mp2));
            break;

        case WM_BUTTON3DBLCLK:
            mr = (MRESULT)button3DblClick(*(PPOINTS)&mp1, SHORT1FROMMP(mp2), SHORT2FROMMP(mp2));
            break;

        case WM_BUTTON3DOWN:
            mr = (MRESULT)button3Down(*(PPOINTS)&mp1, SHORT1FROMMP(mp2), SHORT2FROMMP(mp2));
            break;

        case WM_SYSCOMMAND:
            sysCommand(SHORT1FROMMP(mp1), SHORT1FROMMP(mp2), SHORT2FROMMP(mp2));
            break;

        case WM_CLOSE:
            this->close();
            break;

        case WM_TIMER:
            timer((ULONG)mp1);
            mr = WinDefDlgProc(hwnd, msg, mp1, mp2);
            break;

        case WM_SIZE:
            size(SHORT1FROMMP(mp1), SHORT2FROMMP(mp1),
                 SHORT1FROMMP(mp2), SHORT2FROMMP(mp2));
            break;

        case WM_ADJUSTWINDOWPOS:
            mr = (MRESULT)adjustWindowPos((PSWP)mp1);
            break;

        case WM_ADJUSTFRAMEPOS:
            adjustFramePos((PSWP)mp1, (HSAVEWP)mp2);
            mr = WinDefDlgProc(hwnd, msg, mp1, mp2);
            break;

        default:
            mr = WinDefDlgProc(hwnd, msg, mp1, mp2);
    }
    return mr;
}



/**
 * Sets the window text for a dialog item - printf styled.
 * @returns   sucessindicator.
 * @param     ulId          Id of the dialog item.
 * @param     pszFormat     Format string. (printf styled);
 * @param     ...           More arguments.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark
 */
BOOL    kDlgBase::setDlgItemText(ULONG ulId, PCSZ pszFormat, ...)
{
    va_list arg;
    CHAR    szBuffer[1024];

    va_start(arg, pszFormat);
    vsprintf(szBuffer, pszFormat, arg);
    va_end(arg);

    return WinSetDlgItemText(hwnd, ulId, szBuffer);
}


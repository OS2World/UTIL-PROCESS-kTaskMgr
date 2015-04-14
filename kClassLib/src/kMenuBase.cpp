/* $Id: kMenuBase.cpp,v 1.1 2000/05/23 00:00:54 stknut Exp $
 *
 * kMenuBase (kClassLib) - implemenation of the menu base class.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */

/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
#define INCL_DOSERRORS
#define INCL_WIN

/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <os2.h>
#ifdef USE_KLIB
    #include <kAssert.h>
    #include <kLog.h>
    #include <kHeap.h>
#endif

#include "kBase.h"
#include "kError.h"
#include "kMenuBase.h"


/**
 * Create a menu from a resource.
 * @param     ulResId      Resource id of the template.
 * @param     hmodRes      Module handle of the module containing the template.
 * @param     hwnd         Handle owner frame. For action menues (!fPopup) frame hwnd.
 * @param     fPopup       TRUE: Popup menu; FALSE: Action menu.
 */
kMenuBase::kMenuBase(ULONG ulResId, HMODULE hmodRes/*= NULLHANDLE*/, HWND hwnd/*= NULLHANDLE*/, BOOL fPopup/* = TRUE*/) throw (kError)
    : hwndOwner(hwnd)
{
    TRACELOG_CONSTRUCT("start");
    /* Load menu */
    hwndMenu = WinLoadMenu(fPopup ? HWND_OBJECT : hwnd, hmodRes, ulResId);
    if (hwndMenu == NULLHANDLE)
        throw(kError(kError::unused, kError::win));
    TRACELOG_CONSTRUCT("end");
}


/**
 * Destroys the menu.
 */
kMenuBase::~kMenuBase()
{
    TRACELOG_DESTRUCT("start");
    //WinDestroyWindow(hwndMenu);
    TRACELOG_DESTRUCT("end");
}



/**
 * BOOL WinCheckMenuItem(HWND hwndMenu, ULONG/USHORT id, BOOL fcheck)
 * Sets the check state of menuitem #id to fcheck.
 * @returns   Success indicator.
 * @param     id      Menuitem id.
 * @param     fcheck  New menu item check state.
 */
BOOL kMenuBase::checkMenuItem(USHORT id, BOOL fcheck)
{
   return ((BOOL)WinSendMsg(hwndMenu, MM_SETITEMATTR,
            MPFROM2SHORT(id, TRUE),
            MPFROM2SHORT(MIA_CHECKED, (USHORT)(fcheck) ? MIA_CHECKED : 0)));
}


/**
 * Returns the check state of menuitem #id.
 * @returns   The check state of menuitem #id.
 * @param     id    Menu id.
 */
BOOL kMenuBase::isMenuItemChecked(USHORT id)
{
   return ((BOOL)WinSendMsg(hwndMenu, MM_QUERYITEMATTR,
           MPFROM2SHORT(id, TRUE),
           MPFROMLONG(MIA_CHECKED)));
}


/**
 * Sets the enable state of menuitem #id to fEnable.
 * @returns   Success indicator.
 * @param     id        Menuitem id.
 * @param     fEnabled  New enabled state.
 */
BOOL kMenuBase::enableMenuItem(USHORT id, BOOL fEnable)
{
    return ((BOOL)WinSendMsg(hwndMenu, MM_SETITEMATTR,
            MPFROM2SHORT(id, TRUE),
            MPFROM2SHORT(MIA_DISABLED, (USHORT)(fEnable) ? 0 : MIA_DISABLED)));
}


/**
 * Returns the enable state of menuitem #id.
 * @returns    The enable state of menuitem #id.
 * @param      id  Menuitem id.
 */
BOOL kMenuBase::isMenuItemEnabled(USHORT id)
{
    return (!(BOOL)WinSendMsg(hwndMenu, MM_QUERYITEMATTR,
            MPFROM2SHORT(id, TRUE),
            MPFROMLONG(MIA_DISABLED)));
}


/**
 * Sets the text of menuitem #id.
 * @returns   Success indicator.
 * @param     id   Menuitem id
 * @param     psz  New menuitem tex
 */
BOOL kMenuBase::setMenuItemText(USHORT id, PSZ psz)
{
    return ((BOOL)WinSendMsg(hwndMenu, MM_SETITEMTEXT,
            MPFROMLONG(id), MPFROMP(psz)));
}


/**
 * Returns TRUE if the specified item is a valid choice.
 * @returns   Returns TRUE if the specified item is a valid choice.
 * @param     id  Menuitem id.
 */
BOOL kMenuBase::isMenuItemValid(USHORT id)
{
    return ((BOOL)WinSendMsg(hwndMenu, MM_ISITEMVALID,
            MPFROM2SHORT(id, TRUE), MPFROMLONG(FALSE)));
}


/**
 *
 * @returns
 * @param     idItem      Selected item.
 * @param     fs          Flags see pmref. If idItem is valid, and neither of
 *                        the PU_SELECTITEM or PU_POSITIONONITEM flags are set
 *                        PU_SELECTITEM is set.
 * @param     hwndParent  Handle of parent. If NULLHANDLE  hwndFrame is used.
 * @param     hwndOwner   Handle of owner. If NULLHANDLE  hwndFrame is used.
 */
BOOL kMenuBase::popup(LONG  idItem, ULONG fs, HWND hwndParent, HWND hwndOwner)
{
    POINTL ptl;

    /* adjust flags. */
    if (idItem > 0xFFFF || idItem == 0)
        idItem &= ~(PU_SELECTITEM | PU_POSITIONONITEM);
    else
        if ((idItem & (PU_SELECTITEM | PU_POSITIONONITEM)) == 0)
            idItem |= PU_SELECTITEM;

    /* adjust hwndParent and hwndOwner*/
    if (hwndParent == NULLHANDLE)
        hwndParent = this->hwndOwner;
    if (hwndOwner == NULLHANDLE)
        hwndOwner = this->hwndOwner;

    /* find mouse posision. */
    WinQueryPointerPos(HWND_DESKTOP, &ptl);
    WinMapWindowPoints(HWND_DESKTOP, this->hwndOwner, &ptl, 1);

    /* popup the menu */
    return WinPopupMenu(hwndParent, hwndOwner, hwndMenu,
                        ptl.x, ptl.y,
                        idItem, fs);
}


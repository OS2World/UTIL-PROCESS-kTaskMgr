/* $Id: kMenuBase.h,v 1.1 2000/05/23 00:00:54 stknut Exp $
 *
 * kMenu (kClassLib) - Menu encapsulation.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */

#ifndef _kMenuBase_h_
#define _kMenuBase_h_



/**
 * Base class for menus.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kMenuBase
{
protected:
    HWND    hwndMenu;
    HWND    hwndOwner;

public:
    kMenuBase(ULONG ulResId, HMODULE hmodRes = NULLHANDLE, HWND hwnd = NULLHANDLE, BOOL fPopup = TRUE) throw (kError);
    virtual ~kMenuBase();

    /** @cat operations on menus */
    BOOL checkMenuItem(USHORT id, BOOL fcheck);
    BOOL isMenuItemChecked(USHORT id);
    BOOL enableMenuItem(USHORT id, BOOL fEnable);
    BOOL isMenuItemEnabled(USHORT id);
    BOOL setMenuItemText(USHORT id, PSZ psz);
    BOOL isMenuItemValid(USHORT id);

    /** @cat usage */
    BOOL popup(LONG  idItem = 0,
               ULONG fs = (PU_HCONSTRAIN | PU_VCONSTRAIN |
                           PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_MOUSEBUTTON2),
               HWND hwndParent = NULLHANDLE,
               HWND hwndOwner = NULLHANDLE);
    BOOL popup(HWND hwndOwner,
               LONG  idItem = 0,
               ULONG fs = (PU_HCONSTRAIN | PU_VCONSTRAIN |
                           PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_MOUSEBUTTON2),
               HWND hwndParent = NULLHANDLE
               )    { return popup(idItem, fs, hwndParent, hwndOwner); }

    /** @cat query */
    HWND    getHwnd()   { return hwndMenu; }
};


#endif


/* $Id: kDetailsBase.cpp,v 1.1 2000/05/23 00:51:25 stknut Exp $
 *
 * Implementation of the detail dialog base class, kDetailsBase.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 * Project Odin Software License can be found in LICENSE.TXT
 *
 */


/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
#define INCL_DOS
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
#else
    #include <malloc.h>
#endif
#include <stdio.h>
#include <string.h>
#include <process.h>

#include "kBase.h"
#include "kError.h"
#include "kDlgBase.h"

#include "kDetailsBase.h"

#pragma pack(1)
typedef struct _SWCNTRL2          /* swctl */
{
 HWND     hwnd;
 HWND     hwndIcon;
 HPROGRAM hprog;
 PID      idProcess;
 ULONG    idSession;
 ULONG    uchVisibility;
 ULONG    fbJump;
 CHAR     szSwtitle[MAXNAMEL+4];
 ULONG    bProgType;
} SWCNTRL2;

typedef SWCNTRL2 *PSWCNTRL2;

typedef struct _SWENTRY2          /* swent */
{
 HSWITCH hswitch;
 SWCNTRL2 swctl;
} SWENTRY2;
typedef SWENTRY2 *PSWENTRY2;

typedef struct _SWBLOCK2          /* swblk */
{
 ULONG    cswentry;
 SWENTRY2 aswentry[1];
} SWBLOCK2;
typedef SWBLOCK2 *PSWBLOCK2;



/**
 * Constructor - adds the dialog to the switch list.
 * @param     hwnd      Handle to the dialog window.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
kDetailsBase::kDetailsBase(HWND hwnd)
{
    TRACELOG_CONSTRUCT("start");

    hwnd = hwnd;
    #if 0
    HSWITCH     hswMain;                /* Handle to main windows switch entry. */
    PSWBLOCK    pswblk = (PSWBLOCK)malloc(4096);


    /* Dump all switch entries. */
    if (WinQuerySwitchList(WinQueryAnchorBlock(hwnd), pswblk, 4096))
    {
        int i;
        fprintf(stderr, "\nSwitch list dump! %d entries\n", pswblk->cswentry);
        for (i = 0; i < pswblk->cswentry; i++)
        {
            char szVisible[128];
            char szJump[64];
            szJump[0] = szVisible[0] = '\0';
            if (pswblk->aswentry[i].swctl.uchVisibility & SWL_VISIBLE)      strcat(szVisible, "SWL_VISIBLE ");
            if (pswblk->aswentry[i].swctl.uchVisibility & SWL_INVISIBLE)    strcat(szVisible, "SWL_INVISIBLE ");
            if (pswblk->aswentry[i].swctl.uchVisibility & SWL_GRAYED)       strcat(szVisible, "SWL_GRAYED ");
            if (pswblk->aswentry[i].swctl.fbJump & SWL_JUMPABLE)            strcat(szJump, "SWL_JUMPABLE ");
            if (pswblk->aswentry[i].swctl.fbJump & SWL_NOTJUMPABLE)         strcat(szJump, "SWL_NOTJUMPABLE ");
            fprintf(stderr,
                    "Entry %d\n"
                    "    hswitch        = %08x\n"
                    "    hwnd           = %08x\n"
                    "    hwndIcon       = %08x\n"
                    "    hprog          = %08x\n"
                    "    idProcess      = %08x\n"
                    "    idSession      = %08x\n"
                    "    uchVisibility  = %08x  %s\n"
                    "    fbJump         = %08x  %s\n"
                    "    szSwtitle      = %s\n"
                    "    bProgType      = %08x\n", i,
                    pswblk->aswentry[i].hswitch,
                    pswblk->aswentry[i].swctl.hwnd,
                    pswblk->aswentry[i].swctl.hwndIcon,
                    pswblk->aswentry[i].swctl.hprog,
                    pswblk->aswentry[i].swctl.idProcess,
                    pswblk->aswentry[i].swctl.idSession,
                    pswblk->aswentry[i].swctl.uchVisibility,
                    szVisible,
                    pswblk->aswentry[i].swctl.fbJump,
                    szJump,
                    pswblk->aswentry[i].swctl.szSwtitle,
                    pswblk->aswentry[i].swctl.bProgType
                    );
        }
    }



    /*
     *  Add to tasklist
     *      Remove switch entry for this dialog window if it exists.
     *      Get main window sitch handle.
     *      Create switch entry for this dialog window.
     */
    hsw = WinQuerySwitchHandle(hwnd, 0);
    if (hsw != NULLHANDLE)
        WinRemoveSwitchEntry(hsw);
    hsw = NULLHANDLE;

    hswMain = WinQuerySwitchHandle(NULLHANDLE, getpid());
    if (hswMain != NULL)
    {
        SWCNTRL swctl = {0};
        WinQuerySwitchEntry(hswMain, &swctl);
        swctl.hwnd = hwnd;
        hsw = WinCreateSwitchEntry(WinQueryAnchorBlock(hwnd), &swctl);
    }
    #endif

    TRACELOG_CONSTRUCT("end");
}


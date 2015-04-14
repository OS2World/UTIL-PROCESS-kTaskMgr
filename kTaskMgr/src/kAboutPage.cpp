/* $Id: kAboutPage.cpp,v 1.1 2000/05/23 00:12:45 stknut Exp $
 *
 * kAboutPage - About Notebook page.
 *
 * Copyright (c) 1999-2000 knut st. osmundsen
 *
 */

/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
#define INCL_WINERRORS
#define INCL_WINDIALOG
#define INCL_WININPUT
#define INCL_WINDIALOGS
#define INCL_WINSTDDLGS
#define INCL_WINFRAMEMGR
#define INCL_WINSYS
#define INCL_WINWINDOWMGR

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
#include "kDlgBase.h"
#include "kClickDlg.h"
#include "kBeliever.h"
#include "kNotebookBase.h"
#include "kNotebookPageBase.h"

#include "kAboutPage.h"
#include "kTaskMgr_defs.h"



/**
 * WM_COMMAND - Popup prayer request dialog.
 * @param     usCmd     Control id which send/posted the message.
 * @param     usSource  Source id.
 * @param     fPointer  Mouse pointer flag.
 * @remark    dismisses the dialog if DID_OK or DID_CANCEL.
 */
VOID  kAboutPage::command(USHORT usCmd, USHORT usSource, BOOL fPointer)
{
    if (usCmd == KABOUTPG_PB_BELIEVERS)
    {
        kBeliever Believer(hwnd);
        Believer.showModal();
    }

    usSource = usSource;
    fPointer = fPointer;
}


/**
 * Creates the about page.
 * @param     pNtbk  Pointer to notebook object.
 */
kAboutPage::kAboutPage(kNotebookBase *pNtbk)
: kNotebookPageBase(DL_KABOUTPAGE, NULLHANDLE, pNtbk)
{
}


/**
 * destructor stub.
 */
kAboutPage::~kAboutPage()
{
}



/**
 * Center the dialog.
 * @param     cx  New client area size - x.
 * @param     cy  New client area size - y.
 */
VOID kAboutPage::ntfySized(LONG cx, LONG cy)
{
    SWP     swp;
    if (WinQueryWindowPos(hwnd, &swp))
    {
        /*
         * Center dialog.
         * This didn't work while all space around the dialog
         * were white and I haven't figured out how to make it colored.
         * So we'll do it a different manner. Blow up the dialog and center
         * all elements.
         */
        #if 0
        WinSetWindowPos(hwnd,
                        NULLHANDLE,
                        cx / 2 - swp.cx / 2,
                        cy / 2 - swp.cy / 2,
                        0,
                        0,
                        SWP_MOVE);
        #else
        HWND  hwndCtrl;
        HENUM henum;

        WinSetWindowPos(hwnd,
                        NULLHANDLE,
                        0,
                        0,
                        cx,
                        cy,
                        SWP_MOVE | SWP_SIZE);

        henum = WinBeginEnumWindows(hwnd);
        while ((hwndCtrl = WinGetNextWindow(henum)) != NULLHANDLE)
        {
            SWP swpCtrl;
            if (WinQueryWindowPos(hwndCtrl, &swpCtrl))
                WinSetWindowPos(hwndCtrl,
                                NULLHANDLE,
                                cx / 2 - swpCtrl.cx / 2,
                                swpCtrl.y + (cy - swp.cy) / 2,
                                0,
                                0,
                                SWP_MOVE);
        }
        WinEndEnumWindows(henum);
        #endif
    }
}



/**
 * When the page is inserted we'll set the background of the
 * notebook page.
 */
VOID  kAboutPage::ntfyInserted()
{
    #if 0 /* Arg! This didn't work - still have with space around the page dialog! */
    LONG lClr;
    HWND hwndParent;

    /* Set page background color */
    lClr = SYSCLR_DIALOGBACKGROUND;
    hwndParent = WinQueryWindow(hwnd, QW_PARENT);
    WinSetPresParam(hwndParent,
                    PP_BACKGROUNDCOLORINDEX,
                    sizeof(lClr),
                    &lClr);

    WinSetPresParam(hwndParent,
                    PP_HILITEBACKGROUNDCOLORINDEX,
                    sizeof(lClr),
                    &lClr);
    #endif
}


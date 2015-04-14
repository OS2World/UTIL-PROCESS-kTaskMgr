/* $Id: kAbout.cpp,v 1.1 2000/05/23 00:12:45 stknut Exp $
 *
 * kAbout dialog.
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
#define INCL_WINFRAMEMGR

/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <os2.h>
#ifdef USE_KLIB
    #include <kAssert.h>
    #include <kLog.h>
    #include <kHeap.h>
#endif
#include "kError.h"
#include "kDlgBase.h"
#include "kClickDlg.h"
#include "kAbout.h"
#include "kTaskMgr_defs.h"

/**
 * Invokes the About dialog.
 * @returns   TRUE / FALSE according to the result.
 * @param     hwndOwner  handle of owner.
 */
BOOL invokeAboutDlg(HWND hwndOwner)
{
   return kClickDlg::invoke(hwndOwner, DL_ABOUT, NULLHANDLE);
}


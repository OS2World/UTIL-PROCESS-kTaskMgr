/* $Id: kClickDlg.cpp,v 1.1 2000/05/23 00:00:54 stknut Exp $
 *
 * kClickDlg (kClassLib) - Implementation of kClickDlg.
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
#include "kBase.h"
#include "kError.h"
#include "kDlgBase.h"
#include "kClickDlg.h"


/**
 * overrides the default buttonNDown events. Dismiss the dialog when this event occurs.
 */
BOOL kClickDlg::button1Down(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags)
{
   ptsPointerPos = ptsPointerPos;
   fsHitTestres = fsHitTestres;
   fsFlags = fsFlags;
   return WinDismissDlg(hwnd, DID_OK);
}



/**
 * overrides the default buttonNDown events. Dismiss the dialog when this event occurs.
 */
BOOL kClickDlg::button2Down(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags)
{
   ptsPointerPos = ptsPointerPos;
   fsHitTestres = fsHitTestres;
   fsFlags = fsFlags;
   return WinDismissDlg(hwnd, DID_OK);
}



/**
 * overrides the default buttonNDown events. Dismiss the dialog when this event occurs.
 */
BOOL kClickDlg::button3Down(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags)
{
   ptsPointerPos = ptsPointerPos;
   fsHitTestres = fsHitTestres;
   fsFlags = fsFlags;
   return WinDismissDlg(hwnd, DID_OK);
}



/**
 * Creates a kClickDlg.
 * @param     hwndOwner  handle to owner window.
 * @param     usId       Dialog resource id.
 * @param     hmod       Resource module handle.
 */
kClickDlg::kClickDlg(HWND hwndOwner, USHORT usId, HMODULE hmod) throw (kError)
   : kDlgBase(usId, hmod, hwndOwner)
{
   /* do noting else */
}



/**
 * Invokes a Click dialog.
 * @returns   TRUE / FALSE according to the result.
 * @param     hwndOwner  handle of owner.
 * @param     usId       Dialog resource id.
 * @param     hmod       Resource module handle.
 */
BOOL kClickDlg::invoke(HWND hwndOwner, USHORT usId, HMODULE hmod)
{
   try
   {
      kClickDlg ClickDlg(hwndOwner, usId, hmod);
      return ClickDlg.showModal();
   }
   catch (kError err)
   {
      err.showError("kClickDlg - Error");
      return FALSE;
   }
}


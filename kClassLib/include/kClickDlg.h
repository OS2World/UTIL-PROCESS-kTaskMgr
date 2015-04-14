/* $Id: kClickDlg.h,v 1.1 2000/05/23 00:00:54 stknut Exp $
 *
 * kClickDlg (kClassLib) - Declaration of the kClickDlg class.
 *
 * Copyright (c) 1999-2000 knut st. osmundsen
 *
 */

#ifndef _kClickDlg_h_
#define _kClickDlg_h_


/**
 * @description General click dialog class.
 * @author      knut st. osmundsen
 */
class kClickDlg : public kDlgBase
{
   protected:
      BOOL button1Down(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags); //WM_BUTTON1CLICK
      BOOL button2Down(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags); //WM_BUTTON2CLICK
      BOOL button3Down(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags); //WM_BUTTON3CLICK

   public:
      kClickDlg(HWND hwndOwner, USHORT usId, HMODULE hmod) throw (kError);
      static BOOL invoke(HWND hwndOwner, USHORT usId, HMODULE hmod);
};

#endif

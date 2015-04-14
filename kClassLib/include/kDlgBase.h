/* $Id: kDlgBase.h,v 1.1 2000/05/23 00:00:54 stknut Exp $
 *
 * kDlgBase (kClassLib) - Declaration of dialog base class.
 *
 * Copyright (c) 1999-2000 knut st. osmundsen
 *
 */

#ifndef _kDlgBase_h_
#define _kDlgBase_h_



/*********************/
/* required includes */
/*********************/
#ifndef INCL_WINDIALOGS
    #error "INCL_WINDIALOGS not defined"
#endif

#ifndef INCL_WINFRAMEMGR
    #error "INCL_WINFRAMEMGR not defined"
#endif


/***************************************/
/* "stub" structs in optional includes */
/***************************************/
#ifndef INCL_WINSTDDRAG_INCLUDED
typedef struct _DRAGINFO
{
    int i;
} DRAGINFO, *PDRAGINFO;
#endif

#ifndef INCL_WINSTDCNR_INCLUDED
typedef struct _RECORDCORE
{
    int i;
} RECORDCORE, *PRECORDCORE;
typedef struct _MINIRECORDCORE
{
    int i;
} MINIRECORDCORE, *PMINIRECORDCORE;
typedef struct _FIELDINFO
{
    int i;
} FIELDINFO, *PFIELDINFO;
#endif



/**
 * @description Base class for OS/2 dialogs.
 * @purpose     A common base class for all dialogs.
 * @author      knut st. osmundsen
 */
class kDlgBase : public kBase
{
private:
    BOOL  fDestruction;                 /* Flag set if we're destroyed by WM_DESTROY. */
    ULONG controlIntern(USHORT id, USHORT usNotifyCode, ULONG ulControlSpec); //WM_CONTROL

protected:
    HWND           hwnd;
    PDLGTEMPLATE   pDlgTemplate;
    BOOL           fModal;

    /**@cat Operations */
    ULONG     timerStart(ULONG idTimer, ULONG dtTimeout);
    BOOL      timerStop(ULONG idTimer);

    /**@cat Mouse events */
    virtual BOOL  button1Click(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags); //WM_BUTTON1CLICK
    virtual BOOL  button1DblClick(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags); //WM_BUTTON1DBLCLK
    virtual BOOL  button1Down(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags); //WM_BUTTON1CLICK
    virtual BOOL  button2Click(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags); //WM_BUTTON2CLICK
    virtual BOOL  button2DblClick(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags); //WM_BUTTON2DBLCLK
    virtual BOOL  button2Down(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags); //WM_BUTTON2CLICK
    virtual BOOL  button3Click(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags); //WM_BUTTON3CLICK
    virtual BOOL  button3DblClick(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags); //WM_BUTTON3DBLCLK
    virtual BOOL  button3Down(POINTS ptsPointerPos, USHORT fsHitTestres, USHORT fsFlags); //WM_BUTTON3CLICK


    /**@cat Common events*/
    virtual VOID  command(USHORT usCmd, USHORT usSource, BOOL fPointer); //WM_COMMAND
    virtual ULONG control(USHORT id, USHORT usNotifyCode, ULONG ulControlSpec); //WM_CONTROL
    virtual BOOL  initDlg(HWND hwndFocus); //WM_INITDLG
    virtual VOID  initMenu(USHORT usMenuId, HWND hwndMnu); //WM_INITMENU
    virtual VOID  menuEnd(USHORT usMenuId, HWND hwndMnu); //WM_ENDMENU
    virtual VOID  sysCommand(USHORT usCmd, USHORT usSource, BOOL fPointer); //WM_SYSCOMMAND
    virtual VOID  close(); //WM_CLOSE
    virtual VOID  timer(ULONG idTimer);
    virtual VOID  size(SHORT cxOld, SHORT cyOld, SHORT cxNew, SHORT cyNew); // WM_SIZE
    virtual BOOL  adjustWindowPos(PSWP pswp); //WM_ADJUSTWINDOWPOS
    virtual VOID  adjustFramePos(PSWP pswp, HSAVEWP hsavewphsvwp); //WM_ADJUSTFRAMEPOS


    /**@cat Container notification virtual functions */
    virtual VOID  cnrBeginEdit(USHORT usId, ULONG cb, HWND hwndCnr, PRECORDCORE pRecord, PFIELDINFO pFieldInfo, PSZ *ppszText, ULONG cbText, ULONG id);
    virtual VOID  cnrCollapseTree(USHORT usId, PRECORDCORE pRecord);
    virtual VOID  cnrContextMenu(USHORT usId, PRECORDCORE pRecord);
    virtual ULONG cnrDragAfter(USHORT usId, PRECORDCORE pRecord, PDRAGINFO pDragInfo);
    virtual VOID  cnrDragLeave(USHORT usId, PRECORDCORE pRecord, PDRAGINFO pDragInfo);
    virtual ULONG cnrDragOver(USHORT usId, PRECORDCORE pRecord, PDRAGINFO pDragInfo);
    virtual VOID  cnrDrop(USHORT usId, PRECORDCORE pRecord, PDRAGINFO pDragInfo);
    virtual VOID  cnrDropNotify(USHORT usId, PRECORDCORE pRecord, PDRAGINFO pDragInfo, HWND hwndTarget);
    virtual VOID  cnrDropHelp(USHORT usId, PRECORDCORE pRecord, PDRAGINFO pDragInfo);
    virtual VOID  cnrEmphasis(USHORT usId, HWND hwndCnr, PRECORDCORE pRecord, ULONG fEmphasisMask);
    virtual VOID  cnrEndEdit(USHORT usId, ULONG cb, HWND hwndCnr, PRECORDCORE pRecord, PFIELDINFO pFieldInfo, PSZ *ppszText, ULONG cbText, ULONG id);
    virtual VOID  cnrEnter(USHORT usId, HWND hwndCnr, PRECORDCORE pRecord, ULONG fKey);
    virtual VOID  cnrExpandTree(USHORT usId, PRECORDCORE pRecord);
    virtual VOID  cnrGridResized(USHORT usId, SHORT sGridRows, SHORT sGridCols);
    virtual VOID  cnrHelp(USHORT usId, PRECORDCORE pRecord);
    virtual VOID  cnrInitDrag(USHORT usId, HWND hwndCnr, PRECORDCORE pRecord, LONG x, LONG y, LONG cx, LONG cy);
    virtual VOID  cnrKillFocus(USHORT usId, HWND hwndCnr);
    virtual VOID  cnrPickup(USHORT usId, HWND hwndCnr, PRECORDCORE pRecord, LONG x, LONG y, LONG cx, LONG cy);
    virtual VOID  cnrQueryDelta(USHORT usId, HWND hwndCnr, ULONG fDelta);
    virtual VOID  cnrReallocPSZ(USHORT usId, ULONG cb, HWND hwndCnr, PRECORDCORE pRecord, PFIELDINFO pFieldInfo, PSZ *ppszText, ULONG cbText, ULONG id);
    virtual VOID  cnrScroll(USHORT usId, HWND hwndCnr, LONG lScrollInc, ULONG fScroll);
    virtual VOID  cnrSetFocus(USHORT usId, HWND hwndCnr);

public:
    /**@cat Constructor/Destructor */
    kDlgBase(ULONG ulResId, HMODULE hmodRes, HWND hwndOwner, HWND hwndParent = HWND_DESKTOP) throw (kError);
    kDlgBase(PCSZ pszTitle, ULONG id, ULONG flDialogStyle, ULONG flDialogCreate,
             LONG x, LONG y, LONG cx, LONG cy,
             HWND hwndOwner, HWND hwndParent = HWND_DESKTOP) throw (kError);
    virtual ~kDlgBase();

    /**@cat public methods */
    BOOL    hide();
    BOOL    show();
    ULONG   showModal();
    BOOL    setTitle(PCSZ pszTitle);
    BOOL    resize(LONG cx, LONG cy);
    BOOL    resizeAndMove(LONG x, LONG y, LONG cx, LONG cy);

    BOOL    queryRectangle(PRECTL prectl);

    /**@cat queries */
    HWND    getHwnd();

    /**@cat public methods */
    MRESULT msgLoop(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

    BOOL    setDlgItemText(ULONG ulId, PCSZ pszFormat, ...);

};

#endif

/* $Id: kNotebookBase.h,v 1.1 2000/05/23 00:00:54 stknut Exp $
 *
 * kNotebook (kClassLib) - kNotebook declarations and definitions.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */

#ifndef _kNotebookBase_h_
#define _kNotebookBase_h_

#ifndef _kNotebookPageBase_h_
class kNotebookPageBase;
#endif


/**
 * Notebook base class. Inherits from kDlgBase.
 * The implementation is restricted to only one notebook in a dialog.
 * This shouldn't normally cause any problems...
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kNotebookBase : public kDlgBase
{
protected:
    /**@cat Data */
    HWND        hwndNtbk;
    USHORT      idNotebook;             /* Id of the Notebook control. */
    unsigned    cPages;                 /* Count of pages in the page table. */
    struct _PageEntry
    {
        ULONG  ulPageId;                /* PageId. */
        HWND   hwnd;                    /* Hwnd of the page. */
        kNotebookPageBase *pPage;       /* Pointer to notebook page object */
    } *paPages;                         /* Pointer to the page table. (malloced!) */

    SHORT       cyTitleBar;             /* Size of title. */
    POINTL      ptlBorder;              /* Size of border. */


    /**@cat Misc Operations */
    kNotebookPageBase * queryPageFromId(ULONG ulPageId);
    kNotebookPageBase * queryPageFromHwnd(HWND hwndPage);

    /**@cat overloaded methods */
    virtual ULONG control(USHORT id, USHORT usNotifyCode, ULONG ulControlSpec); //WM_CONTROL
    virtual VOID  size(SHORT cxOld, SHORT cyOld, SHORT cxNew, SHORT cyNew); //WM_SIZE
    virtual BOOL  adjustWindowPos(PSWP pswp); //WM_ADJUSTWINDOWPOS

    /**@cat virtual methods */
    virtual VOID  ntbkHelp(ULONG ulPageId);
    virtual VOID  ntbkNewPageSize();
    virtual VOID  ntbkPageDeleted(PDELETENOTIFY pDeleteNotify);
    virtual VOID  ntbkPageSelected(PPAGESELECTNOTIFY pPageSelectNotify);
    virtual VOID  ntbkPageSelectedPending(PPAGESELECTNOTIFY pPageSelectNotify);


public:
    kNotebookBase(ULONG ulResId, HMODULE hmodRes, ULONG idNotebook,
                  HWND hwndOwner, HWND hwndParent = HWND_DESKTOP) throw (kError);
    kNotebookBase(PCSZ pszTitle, ULONG id, ULONG flDialogStyle, ULONG flDialogCreate,
                  LONG x, LONG y, LONG cx, LONG cy,
                  ULONG flNotebookStyle, HWND hwndOwner, HWND hwndParent = HWND_DESKTOP) throw (kError);
    virtual ~kNotebookBase();

    /**@cat operations */
    BOOL    insertPage(kNotebookPageBase *pNtbkPage,
                       USHORT usPageStyle,
                       USHORT usPageOrder,
                       ULONG  ulPageId = 0UL);
    BOOL    queryPageRectangle(PRECTL pRectl, BOOL fPage);
    BOOL    setTabText(ULONG ulPageId, PSZ pszTabText);
    BOOL    addjustTabSizes(VOID);

    ULONG   queryPageId(const kNotebookPageBase * pPage) const;
};


#endif

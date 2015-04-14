/* $Id: kNotebookPageBase.h,v 1.1 2000/05/23 00:00:54 stknut Exp $
 *
 * kNotebookPageBase (kClassLib) - base class for notebook pages.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */


#ifndef _kNotebookPageBase_h_
#define _kNotebookPageBase_h_

#ifndef _kNotebookBase_h_
class kNotebookBase;
#endif

/**
 * Base class for notebook pages. This inherits from kDlgBase.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kNotebookPageBase : public kDlgBase
{
protected:
    kNotebookBase * pNtbk;
    ULONG           ulPageId;
    virtual VOID    command(USHORT usCmd, USHORT usSource, BOOL fPointer); //WM_COMMAND

public:
    kNotebookPageBase(ULONG ulResId, HMODULE hmodRes, kNotebookBase *pNtbk) throw (kError);
    ~kNotebookPageBase();

    /**@cat notifies from the notebook */
    virtual VOID  ntfyHelp();
    virtual VOID  ntfySelected();
    virtual BOOL  ntfySelectedPending();
    virtual VOID  ntfyDeselected();
    virtual BOOL  ntfyDeselectedPending();
    virtual VOID  ntfySized(LONG cx, LONG cy);
    virtual VOID  ntfyInserted();

    /**@cat queries used by the notebook */
    virtual PSZ   queryTabText(PSZ pszBuffer, ULONG cchBuffer);
    virtual ULONG queryPageStyle();
};


#endif

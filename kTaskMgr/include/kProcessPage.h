/* $Id: kProcessPage.h,v 1.2 2000/05/23 03:37:08 stknut Exp $
 *
 * kProcessPage - Process overview dialog. (to be notebook page)
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */

#ifndef _kProcessPage_h_
#define _kProcessPage_h_


/**
 * @description The Process overview dialog (later notebook page).
 * @author      knut st. osmundsen
 */
class kProcessPage : public kContainerPageBase
{
protected:
    /** @cat Data members */
    ULONG               idTimerRefresh;


    /** @cat event overrides */
    VOID  timer(ULONG idTimer); //WM_TIMER

public:
    kProcessPage(kNotebookBase *pNtbk, USHORT usPid = 0xffff, USHORT usSFN = 0xffff) throw (kError);
    ~kProcessPage();
};

#endif


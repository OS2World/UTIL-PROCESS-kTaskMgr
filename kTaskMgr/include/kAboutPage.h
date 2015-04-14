/* $Id: kAboutPage.h,v 1.1 2000/05/23 00:16:50 stknut Exp $
 *
 * class kAboutPage declaration.
 *
 * Copyright (c) 1999-2000 knut st. osmundsen
 *
 */

#ifndef _kAboutPage_h_
#define _kAboutPage_h_

/**
 * The about page.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kAboutPage : public kNotebookPageBase
{
protected:
    /**@cat Common events*/
    VOID  command(USHORT usCmd, USHORT usSource, BOOL fPointer); //WM_COMMAND

public:
    /**@cat constructors */
    kAboutPage(kNotebookBase *pNtbk) throw (kError);
    ~kAboutPage();

    /**@cat event handlers */
    virtual VOID  ntfySized(LONG cx, LONG cy);
    virtual VOID  ntfyInserted();
};
#endif


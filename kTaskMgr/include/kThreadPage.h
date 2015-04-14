/* $Id: kThreadPage.h,v 1.1 2000/05/23 00:16:51 stknut Exp $
 *
 * kThreadPage (kTaskMgr) - Threads overview page.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */

#ifndef _kThreadPage_h_
#define _kThreadPage_h_


/**
 * @description The Threads overview page.
 * @author      knut st. osmundsen
 */
class kThreadPage : public kContainerPageBase
{
public:
    kThreadPage(kNotebookBase *pNtbk, USHORT usPid = 0xffff) throw (kError);
};

#endif


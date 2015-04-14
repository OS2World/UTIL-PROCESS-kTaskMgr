/* $Id: kSFNPage.h,v 1.1 2000/05/23 00:16:51 stknut Exp $
 *
 * kSFNPage (kTaskMgr) - System file number overview page.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */

#ifndef _kSFNPage_h_
#define _kSFNPage_h_


/**
 * @description The SFN overview page.
 * @author      knut st. osmundsen
 */
class kSFNPage : public kContainerPageBase
{
public:
    kSFNPage(kNotebookBase *pNtbk, USHORT usPid = 0xffff) throw (kError);
};

#endif


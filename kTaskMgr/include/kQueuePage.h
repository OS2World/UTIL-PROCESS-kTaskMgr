/* $Id: kQueuePage.h,v 1.1 2000/05/23 00:16:51 stknut Exp $
 *
 * kQueuePage (kTaskMgr) - System file number overview page.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */

#ifndef _kQueuePage_h_
#define _kQueuePage_h_


/**
 * @description The Queue overview page.
 * @author      knut st. osmundsen
 */
class kQueuePage : public kContainerPageBase
{
public:
    kQueuePage(kNotebookBase *pNtbk, USHORT usPid = 0xffff) throw (kError);
};

#endif


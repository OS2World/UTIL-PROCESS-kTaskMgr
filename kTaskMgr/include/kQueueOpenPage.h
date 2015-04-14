/* $Id: kQueueOpenPage.h,v 1.1 2000/05/23 00:16:51 stknut Exp $
 *
 * kQueueOpenPage (kTaskMgr) - Open queue overview page.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */

#ifndef _kQueueOpenPage_h_
#define _kQueueOpenPage_h_


/**
 * @description The Queue overview page.
 * @author      knut st. osmundsen
 */
class kQueueOpenPage : public kContainerPageBase
{
public:
    kQueueOpenPage(kNotebookBase *pNtbk, USHORT usPid = 0xffff) throw (kError);
};

#endif


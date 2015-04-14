/* $Id: kProcessDetailsPage.h,v 1.1 2000/05/23 00:16:50 stknut Exp $
 *
 * kProcessDetailsPage (kTaskMgr) - Process Details page.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */

#ifndef _kProcessDetailsPage_h_
#define _kProcessDetailsPage_h_


/**
 * @description The process details page.
 * @author      knut st. osmundsen
 */
class kProcessDetailsPage : public kContainerPageBase
{
public:
    kProcessDetailsPage(kNotebookBase *pNtbk, USHORT usPid) throw (kError);
};

#endif


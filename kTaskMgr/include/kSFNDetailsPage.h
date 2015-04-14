/* $Id: kSFNDetailsPage.h,v 1.1 2000/05/23 00:16:51 stknut Exp $
 *
 * kSFNDetailsPage (kTaskMgr) - SFN Details page.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */

#ifndef _kSFNDetailsPage_h_
#define _kSFNDetailsPage_h_


/**
 * @description The SFN details page.
 * @author      knut st. osmundsen
 */
class kSFNDetailsPage : public kContainerPageBase
{
public:
    kSFNDetailsPage(kNotebookBase *pNtbk, USHORT usSFN) throw (kError);
};

#endif


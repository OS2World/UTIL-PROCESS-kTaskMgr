/* $Id: kMemMapPage.h,v 1.1 2000/06/19 20:08:42 stknut Exp $
 *
 * kMemMapPage (kTaskMgr) - Memory map overview page.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */

#ifndef _kMemMapPage_h_
#define _kMemMapPage_h_


/**
 * @description The memory map overview page.
 * @author      knut st. osmundsen
 */
class kMemMapPage : public kContainerPageBase
{
public:
    kMemMapPage(kNotebookBase *pNtbk, USHORT usPid = 0xffff) throw (kError);
};

#endif


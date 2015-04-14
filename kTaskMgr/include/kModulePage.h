/* $Id: kModulePage.h,v 1.1 2000/05/23 00:16:50 stknut Exp $
 *
 * kModulePage - Module overview dialog. (to be notebook page)
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */

#ifndef _kModulePage_h_
#define _kModulePage_h_


/**
 * @description The Module overview dialog (later notebook page).
 * @author      knut st. osmundsen
 */
class kModulePage : public kContainerPageBase
{
public:
    /*
     * fPid = 0xffff & hMte = 0xffff: View all modules
     * fPid = 0xffff & hMte < 0xffff: View dependants of this module
     * fPid < 0xffff                : View all loaded modules for this processes.
     *                                hMte is ignored.
     */
    kModulePage(kNotebookBase *pNtbk, USHORT hMte = 0xffff, USHORT usPid = 0xffff) throw (kError);
};

#endif


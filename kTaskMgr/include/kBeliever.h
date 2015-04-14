/* $Id: kBeliever.h,v 1.1 2000/05/23 00:16:50 stknut Exp $
 *
 * class kBeliever declaration.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */

#ifndef _kBeliever_h_
#define _kBeliever_h_


/**
 * Believer dialog.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kBeliever : public kClickDlg
{
public:
    kBeliever(HWND hwndOwner) throw (kError);
    ~kBeliever();
};


#endif


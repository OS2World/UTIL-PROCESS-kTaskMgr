/* $Id: kDetailsBase.h,v 1.1 2000/05/23 00:51:25 stknut Exp $
 *
 * Details dialog base class.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 * Project Odin Software License can be found in LICENSE.TXT
 *
 */
#ifndef _kDetailsBase_h_
#define _kDetailsBase_h_


/**
 * Base class for details dialogs.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kDetailsBase : public kBase
{
private:
    HSWITCH     hsw;                    /* Handle to this dialogs switch entry. */

public:
    kDetailsBase(HWND hwnd);

    virtual     BOOL update() = 0;
};


#endif

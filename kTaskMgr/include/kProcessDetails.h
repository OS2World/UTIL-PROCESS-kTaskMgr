/* $Id: kProcessDetails.h,v 1.2 2000/05/23 00:51:25 stknut Exp $
 *
 * kProcessDetails - Process detail notebook.
 *
 * Copyright (c) 2000 knut st. osmundsen
 *
 */

#ifndef _kProcessDetails_h_
#define _kProcessDetails_h_


/**
 * Process detail notebook.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kProcessDetails : public kNotebookBase, public kDetailsBase
{
private:
    /** @cat data members */
    HWND        hwndOwner;              /* handle of the owner? */
    USHORT      usPid;                  /* The Process ID we're displaying details of. */

public:
    /** @cat constructor and destructor */
    kProcessDetails(USHORT usPid, HWND hwndTaskMgr) throw (kError);
    ~kProcessDetails();

    BOOL    show();
    BOOL    update();

    static BOOL invoke(USHORT usPid, HWND hwndOwner);
};

#endif

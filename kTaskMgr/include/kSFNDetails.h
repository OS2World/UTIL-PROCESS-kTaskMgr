/* $Id: kSFNDetails.h,v 1.2 2000/05/23 00:51:25 stknut Exp $
 *
 * kSFNDetails - SFN detail notebook.
 *
 * Copyright (c) 2000 knut st. osmundsen
 *
 */

#ifndef _kSFNDetails_h_
#define _kSFNDetails_h_


/**
 * SFN detail notebook.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kSFNDetails : public kNotebookBase, public kDetailsBase
{
private:
    /** @cat data members */
    HWND        hwndOwner;              /* handle of the owner? */
    USHORT      usSFN;                  /* The SFN we're displaying details of. */

public:
    /** @cat constructor and destructor */
    kSFNDetails(USHORT usSFN, HWND hwndTaskMgr) throw (kError);
    ~kSFNDetails();

    BOOL    show();
    BOOL    update();

    static BOOL invoke(USHORT usSFN, HWND hwndOwner);
};

#endif

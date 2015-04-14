/* $Id: kProcessPriority.h,v 1.1 2000/05/23 00:16:50 stknut Exp $
 *
 * kProcessPriority (kTaskMgr) - Change Process Priority dialog.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kProcessPriority_h_
#define _kProcessPriority_h_



/**
 * Module Detail Dialog.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kProcessPriority : public kDlgBase
{
private:
    /** @cat */
    HWND                hwndOwner;
    kThreadContainer *  pCnrThreads;
    USHORT              usPid;

    /** @cat event overrides */
    VOID    cnrContextMenu(USHORT usId, PRECORDCORE pRecord);
    VOID    cnrEnter(USHORT usId, HWND hwndCnr, PRECORDCORE pRecord, ULONG fKey);
    VOID    command(USHORT usCmd, USHORT usSource, BOOL fPointer); //WM_COMMAND

public:
    /** @cat Constructor and Destructor */
    kProcessPriority(USHORT usPid, HWND hwndOwner) throw(kError);
    ~kProcessPriority();

    BOOL    update();
};

#endif

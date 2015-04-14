/* $Id: kProcessContainer.h,v 1.2 2000/05/23 03:37:08 stknut Exp $
 *
 * kProcessContainer - General process container.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */

#ifndef _kProcessContainer_h_
#define _kProcessContainer_h_




/**
 * Generic module container.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kProcessContainer : public kTaskMgrContainerBase
{
private:
    /** @cat private data */
    USHORT              usPid;          /* 0xFFFF: all, != 0xFFFF: children of this pid. */
    USHORT              usSFN;          /* 0xFFFF: if unused. */

    /** @cat private methods */
    BOOL    kUpdateAllProcesses();
    BOOL    kUpdateChildProcesses();
    BOOL    kUpdateProcessesUsingSFN();

    /** @cat private events */
    VOID    command(USHORT usCmd, USHORT usSource, BOOL fPointer); //WM_COMMAND



public:
    /**
     * usPid = 0xFFFF:  View all processes.
     * usPid != 0:      View all children of the given process id.
     * usSFN != 0xffff: View all processes using the SFN. (usPid is ignored)
     */
    kProcessContainer(HWND hwndDlg, ULONG ulCnrId, USHORT usPid = 0xFFFF, USHORT usSFN = 0xffff) throw (kError);
    ~kProcessContainer();

    VOID  update();
};


#endif


/* $Id: kProcessDetailsContainer.h,v 1.1 2000/05/23 00:16:50 stknut Exp $
 *
 * kProcessDetailsContainer (kTaskMgr) - Process details.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kProcessDetailsContainer_h_
#define _kProcessDetailsContainer_h_




/**
 * Process details container.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kProcessDetailsContainer : public kTaskMgrContainerBase
{
private:
    /** @cat private data */
    kDetailsRecord *pCnrRec;            /* Pointer to the first container record. */
    USHORT          usPid;              /* Process identifier of the process we're displaying details for. */
    USHORT          hMte;               /* Module handle of the executable file. */
    USHORT          usPPid;             /* Parent process id. */

    /** @cat private events */
    VOID    command(USHORT usCmd, USHORT usSource, BOOL fPointer); //WM_COMMAND
    VOID    initMenu(USHORT usMenuId, HWND hwndMnu); //WM_INITMENU

public:
    kProcessDetailsContainer(HWND hwndDlg, ULONG ulCnrId, USHORT usPid) throw (kError);
    ~kProcessDetailsContainer();

    VOID    update();
};


#endif



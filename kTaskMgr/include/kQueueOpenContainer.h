/* $Id: kQueueOpenContainer.h,v 1.1 2000/05/23 00:16:51 stknut Exp $
 *
 * kQueueOpenContainer (kTaskMgr) - generic open queue container.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kQueueOpenContainer_h_
#define _kQueueOpenContainer_h_




/**
 * Generic object container.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kQueueOpenContainer : public kTaskMgrContainerBase
{
private:
    /** @cat private data */
    USHORT          usPid;              /* Process identifier of the process we're displaying used queues for. */
                                        /* Special value 0xFFFF is used to show queue for entire system.  */

    /** @cat private methods */
    BOOL    insertQueueOpenPid();
    BOOL    insertQueueOpenAll();

    /** @cat private events */
    VOID    command(USHORT usCmd, USHORT usSource, BOOL fPointer); //WM_COMMAND

public:
    /**
     * usPid = 0xFFFF: View queue info for the entire system.
     * usPid != 0:     View info on queues used by the given process.
     */
    kQueueOpenContainer(HWND hwndDlg, ULONG ulCnrId, USHORT usPid = 0xFFFF) throw (kError);
    ~kQueueOpenContainer();

    VOID    update();
};


#endif



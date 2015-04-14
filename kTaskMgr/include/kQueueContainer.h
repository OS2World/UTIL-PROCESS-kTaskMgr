/* $Id: kQueueContainer.h,v 1.1 2000/05/23 00:16:51 stknut Exp $
 *
 * kQueueContainer (kTaskMgr) - generic queue container.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kQueueContainer_h_
#define _kQueueContainer_h_




/**
 * Generic object container.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kQueueContainer : public kTaskMgrContainerBase
{
private:
    /** @cat private data */
    USHORT          usPid;              /* Process identifier of the process we're displaying used queues for. */
                                        /* Special value 0xFFFF is used to show queue for entire system.  */
    PSZ             pszTitle;           /* Pointer to title string. */
    ULONG           cbUsed;             /* Latest amount of used memory. Used to minimize update of container title. */
    ULONG           cQueues;            /* Latest number of queues. Used to minimize update of container title. */

    /** @cat private methods */
    BOOL    insertQueuePid();
    BOOL    insertQueueAll();

    /** @cat private events */
    VOID    command(USHORT usCmd, USHORT usSource, BOOL fPointer); //WM_COMMAND

public:
    /**
     * usPid = 0xFFFF: View queue info for the entire system.
     * usPid != 0:     View info on queues used by the given process.
     */
    kQueueContainer(HWND hwndDlg, ULONG ulCnrId, USHORT usPid = 0xFFFF) throw (kError);
    ~kQueueContainer();

    VOID    update();
};


#endif



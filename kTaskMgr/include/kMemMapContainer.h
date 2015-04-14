/* $Id: kMemMapContainer.h,v 1.1 2000/06/19 20:08:42 stknut Exp $
 *
 * kMemMapContainer (kTaskMgr) - generic memory map container.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kMemMapContainer_h_
#define _kMemMapContainer_h_




/**
 * Generic memory map container.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kMemMapContainer : public kTaskMgrContainerBase
{
private:
    /** @cat private data */
    USHORT          usPid;              /* Process identifier of the process we're displaying memmap for. */
                                        /* 0xFFFF: memory map for the entire system. */

    /** @cat private methods */
    BOOL    insertPid();
    BOOL    insertAll();

    /** @cat private events */
    VOID    command(USHORT usCmd, USHORT usSource, BOOL fPointer); //WM_COMMAND


public:
    /**
     * usPid = 0xFFFF: View memory map of the entire system.
     * usPid != 0:     View memory map for the given process.
     */
    kMemMapContainer(HWND hwndDlg, ULONG ulCnrId, USHORT usPid = 0xFFFF) throw (kError);
    ~kMemMapContainer();

    VOID    update();
};


#endif



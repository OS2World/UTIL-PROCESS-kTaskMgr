/* $Id: kSFNContainer.h,v 1.1 2000/05/23 00:16:51 stknut Exp $
 *
 * kSFNContainer (kTaskMgr) - generic SystemFileNumber (SFN) container.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kSFNContainer_h_
#define _kSFNContainer_h_




/**
 * Generic object container.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kSFNContainer : public kTaskMgrContainerBase
{
private:
    /** @cat private data */
    USHORT          usPid;              /* Process identifier of the process we're displaying SFNs for. */
                                        /* Special value 0xFFFF is used to show SFN for all processes.  */

    /** @cat private methods */
    BOOL    insertSFNsPid();
    BOOL    insertSFNsAll();

    /** @cat private events */
    VOID    command(USHORT usCmd, USHORT usSource, BOOL fPointer); //WM_COMMAND


public:
    /**
     * usPid = 0xFFFF: View all SFNs.
     * usPid != 0:     View only SFNs for the given process.
     */
    kSFNContainer(HWND hwndDlg, ULONG ulCnrId, USHORT usPid = 0xFFFF) throw (kError);
    ~kSFNContainer();

    VOID    update();
};


#endif



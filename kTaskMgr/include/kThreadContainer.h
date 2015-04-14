/* $Id: kThreadContainer.h,v 1.1 2000/05/23 00:16:51 stknut Exp $
 *
 * kThreadContainer - generic module container.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kThreadContainer_h_
#define _kThreadContainer_h_




/**
 * Generic module container.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kThreadContainer : public kTaskMgrContainerBase
{
private:
    /** @cat private data */
    USHORT          usPid;              /* Process Indicator we're showing threads for. */

    /** @cat private methods */
    BOOL    insertThreadsPid();
    BOOL    insertThreadsAll();

    /** @cat private events */
    VOID    command(USHORT usCmd, USHORT usSource, BOOL fPointer); //WM_COMMAND


public:
    kThreadContainer(HWND hwndDlg, ULONG ulCnrId, USHORT usPid) throw (kError);
    ~kThreadContainer();

    VOID    update();
};


#endif



/* $Id: kObjectContainer.h,v 1.1 2000/05/23 00:16:50 stknut Exp $
 *
 * kObjectContainer - generic object container.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kObjectContainer_h_
#define _kObjectContainer_h_




/**
 * Generic object container.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kObjectContainer : public kTaskMgrContainerBase
{
private:
    /** @cat private data */
    USHORT          hMTE;               /* Module handle. */

    /** @cat private methods */
    BOOL    insertObjects();

    /** @cat private events */
    VOID    command(USHORT usCmd, USHORT usSource, BOOL fPointer); //WM_COMMAND


public:
    kObjectContainer(HWND hwndDlg, ULONG ulCnrId, USHORT hMTE) throw (kError);
    ~kObjectContainer();

    VOID    update();
};


#endif



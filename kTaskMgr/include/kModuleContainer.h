/* $Id: kModuleContainer.h,v 1.1 2000/05/23 00:16:50 stknut Exp $
 *
 * kModuleContainer - generic module container.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kModuleContainer_h_
#define _kModuleContainer_h_




/**
 * Generic module container.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kModuleContainer : public kTaskMgrContainerBase
{
private:
    /** @cat private data */
    USHORT              hMte;           /* 0xFFFF: all, < 0xFFFF: dependant/imported modules. */
    USHORT              usPid;          /* 0xFFFF when unused. hMTE is not valid while usPid != 0xffff. */

    /** @cat private methods */
    BOOL    insertAllModules();
    BOOL    insertImpModules();
    BOOL    insertLoadedModules();
    BOOL    insertAllAllModules();

    /** @cat private events */
    VOID    command(USHORT usCmd, USHORT usSource, BOOL fPointer); //WM_COMMAND

public:
    /**
     * fPid = 0xffff & hMte = 0xffff: View all modules
     * fPid = 0xffff & hMte < 0xffff: View dependants of this module
     * fPid < 0xffff                : View all loaded modules for this processes.
     *                                hMte is ignored.
     */
    kModuleContainer(HWND hwndDlg, ULONG ulCnrId, USHORT hMte = 0xffff, USHORT usPid = 0xffff) throw (kError);
    ~kModuleContainer();

    VOID  update();
};


#endif


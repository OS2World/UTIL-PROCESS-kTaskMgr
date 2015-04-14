/* $Id: kSFNDetailsContainer.h,v 1.1 2000/05/23 00:16:51 stknut Exp $
 *
 * kSFNDetailsContainer (kTaskMgr) - SFN details container.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kSFNDetailsContainer_h_
#define _kSFNDetailsContainer_h_




/**
 * SFN details container.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kSFNDetailsContainer : public kTaskMgrContainerBase
{
private:
    /** @cat private data */
    kDetailsRecord *pCnrRec;            /* Pointer to the first container record. */
    USHORT          usSFN;              /* SFN we're displaying details for. */

public:
    kSFNDetailsContainer(HWND hwndDlg, ULONG ulCnrId, USHORT usSFN) throw (kError);
    ~kSFNDetailsContainer();

    VOID    update();
};


#endif



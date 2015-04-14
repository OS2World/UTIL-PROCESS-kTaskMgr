/* $Id: kContainerPageBase.h,v 1.1 2000/05/23 00:16:50 stknut Exp $
 *
 * kContainerPageBase (kTaskMgr) - Generalized container notebook page.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */

#ifndef _kContainerPageBase_h_
#define _kContainerPageBase_h_


/**
 * @description Generalized container notebook page.
 * @author      knut st. osmundsen
 */
class kContainerPageBase : public kNotebookPageBase
{
protected:
    /** @cat Data members */
    kTaskMgrContainerBase *     pContainer;
    POINTL                      ptlBorder;
    PSZ                         pszTabText;


    /** @cat event overrides */
    VOID  cnrContextMenu(USHORT usId, PRECORDCORE pRecord);
    VOID  cnrEnter(USHORT usId, HWND hwndCnr, PRECORDCORE pRecord, ULONG fKey);

    VOID  ntfyInserted();

public:
    kContainerPageBase(kNotebookBase *pNtbk, PSZ pszTabText) throw (kError);
    virtual ~kContainerPageBase();

    VOID  ntfySized(LONG cx, LONG cy);
};

#endif


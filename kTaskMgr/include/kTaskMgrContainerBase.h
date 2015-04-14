/* $Id: kTaskMgrContainerBase.h,v 1.1 2000/05/23 00:16:51 stknut Exp $
 *
 * kSFNContainer (kTaskMgr) - generic SystemFileNumber (SFN) container.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kTaskMgrContainer_h_
#define _kTaskMgrContainer_h_


class kTaskMgrRecordBase;
class kMenuBase;


/**
 * Generic object container.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kTaskMgrContainerBase : public kDetailCnr
{
private:
    /** @cat private data */
    kTaskMgrRecordBase *pCurRecord;     /* Pointer to the current record actual for menu action. */
    USHORT              usEnterId;      /* The id of the menuid which occurs when enter is */
                                        /* pressed on a selected container record. */
                                        /* (0xffff means no menu id) */

protected:
    /** @cat protected data */
    kMenuBase *         pMenuCnrAll;    /* Entire container menu. */
    kMenuBase *         pMenuRecord;    /* Single record container menu. */

    ULONG               iSortColumn;    /* Column number (0-based) to sort by. */
    ULONG               iSortDefaultColumn; /* Default sorting column. (0 based) */

private:
    /** @cat private events */
    VOID            menuEnd(USHORT usMenuId, HWND hwndMnu); //WM_MENUEND
    SHORT           sortCallBack(kCnrMiniRecord *pRecord1, kCnrMiniRecord *pRecord2);

protected:
    /** @cat protected events */
    virtual VOID    command(USHORT usCmd, USHORT usSource, BOOL fPointer); //WM_COMMAND

public:
    kTaskMgrContainerBase(HWND           hwndDlg,
                          ULONG          ulCnrId,
                          PSZ            pszTitle,
                          ULONG          cColumns,
                          PFIELDINFO     paFieldInfo,
                          USHORT         usMenuCnrAllId,
                          USHORT         usMenuRecordId = 0xffff,
                          USHORT         usEnterId = 0xffff,
                          ULONG          iSortDefaultColumn  = 0xffffffffUL
                          ) throw (kError);
    virtual ~kTaskMgrContainerBase();

    /** @cat events which sorrily have to be invoked from the outside... */
    VOID            cnrContextMenu(USHORT usId, PRECORDCORE pRecord);
    VOID            cnrEnter(USHORT usId, HWND hwndCnr, PRECORDCORE pRecord, ULONG fKey);

    /**
     * Updates the contents of the container.
     * This method will be invoked by a timer later.
     * You should assume that someone else has updated the basis data.
     * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
     */
    virtual VOID    update() = 0;


    /**@cat queries */
    kTaskMgrRecordBase *    getCurRecord()  { return pCurRecord; }
};


#endif



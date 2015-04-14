/* $Id: kTaskMgrRecordBase.h,v 1.1 2000/05/23 00:16:51 stknut Exp $
 *
 * kTaskMgrRecordBase (kTaskMgr) - Base container record for use in kTaskMgr.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kTaskMgrRecordBase_h_
#define _kTaskMgrRecordBase_h_

class kTaskMgrContainerBase;

/**
 * Base container record for use in kTaskMgr.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kTaskMgrRecordBase : public kCnrMiniRecord
{
private:
    BOOL    fDirty;
    BOOL    fNotDeleted;

public:
    /**@cat public methods */
    VOID    init();

    /**@cat public set and query methods. */
    void    setDirty()          { fDirty = TRUE; }
    void    setClean()          { fDirty = FALSE; }
    void    setNotDeleted()     { fNotDeleted = TRUE; }
    BOOL    isDeleted()         { return fNotDeleted; }
    BOOL    isDirty()           { return fDirty; }

    /**@cat public list operations. Processes all records, starting at this. */
    BOOL    markAllDeleted();
    BOOL    invalidateAllDirtyRecords(kTaskMgrContainerBase *pCnr);
    BOOL    removeAllDeleteRecords(kTaskMgrContainerBase *pCnr);
};

#endif

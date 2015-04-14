/* $Id: kTaskMgrRecordBase.cpp,v 1.1 2000/05/23 00:12:46 stknut Exp $
 *
 * kTaskMgrRecordBase (kTaskMgr) - Base container record for use in kTaskMgr.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */



/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
#define INCL_DOSERRORS
#define INCL_WIN

/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <os2.h>
#ifdef USE_KLIB
    #include <kAssert.h>
    #include <kLog.h>
    #include <kHeap.h>
#endif
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "kBase.h"
#include "kError.h"
#include "kContainer.h"

#include "kQuerySysState.h"
#include "kTaskMgrRecordBase.h"
#include "kTaskMgrContainerBase.h"


/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/

/**
 * Initiate the record.
 * !This have to be called by the children!
 */
VOID  kTaskMgrRecordBase::init()
{
    fNotDeleted = TRUE;
    fDirty = FALSE;
}


/**
 * Marks all records (in the list record list) deleted.
 * This is useful when updating the records in a container.
 *      1) Mark all records deleted using this method.
 *      2) Go thru the new data and mark the records
 *         "not-deleted" when you find them.
 *      3) After all new data has been processed the records
 *         still marked deleted really are deleted. Remove
 *         them by calling removeAllDeletedRecords.
 * @returns   Success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL kTaskMgrRecordBase::markAllDeleted()
{
    kTaskMgrRecordBase *pRecord;
    for (pRecord = this; pRecord != NULL; pRecord = (kTaskMgrRecordBase *)pRecord->getNext())
        pRecord->fNotDeleted = FALSE;
    return TRUE;
}


/**
 * Invalidates all dirty records (ie. call the invalidateRecords
 * method of the container passed in, for each invalid valid record.)
 * The dirty flags are cleared.
 * @returns   success indicator.
 * @param     pCnr  Pointer to the container which own this record and should
 *                  receive the invalidate call(s).
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL kTaskMgrRecordBase::invalidateAllDirtyRecords(kTaskMgrContainerBase *pCnr)
{
    #define INVALIDATEARRAYSIZE   100
    kTaskMgrRecordBase *pRecord;
    kTaskMgrRecordBase *paInvalidRecords[INVALIDATEARRAYSIZE];
    int                 cInvalidRecords = 0;

    for (pRecord = this; pRecord != NULL; pRecord = (kTaskMgrRecordBase *)pRecord->getNext())
    {
        if (pRecord->fDirty)
        {
            pRecord->fDirty = FALSE;
            if (cInvalidRecords >= INVALIDATEARRAYSIZE)
            {
                if (!pCnr->invalidateRecords(CMA_TEXTCHANGED, cInvalidRecords,
                                             (kCnrMiniRecord **)&paInvalidRecords[0]))
                    return FALSE;
                cInvalidRecords = 0;
            }
            paInvalidRecords[cInvalidRecords++] = pRecord;
            pRecord->fDirty = FALSE;
        }
    }

    if (cInvalidRecords > 0)
        return pCnr->invalidateRecords(CMA_TEXTCHANGED, cInvalidRecords,
                                       (kCnrMiniRecord **)&paInvalidRecords[0]);
    return TRUE;
}


/**
 * Removes all the records which is marked deleted (fNotDelete == FALSE),
 * (ie. calls the removeRecords of the container passed, for all the
 * deleted records.)
 * @returns   Success indicator.
 * @param     pCnr  Pointer to the container which own this record and should
 *                  receive the remove call(s).
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL kTaskMgrRecordBase::removeAllDeleteRecords(kTaskMgrContainerBase *pCnr)
{
    #define DELETEARRAYSIZE   100
    kTaskMgrRecordBase *pRecord;
    kTaskMgrRecordBase *paDeletedRecords[DELETEARRAYSIZE];
    int                 cDeletedRecords = 0;

    for (pRecord = this; pRecord != NULL; pRecord = (kTaskMgrRecordBase *)pRecord->getNext())
    {
        if (!pRecord->fNotDeleted)
        {
            if (cDeletedRecords >= INVALIDATEARRAYSIZE)
            {
                if (!pCnr->removeRecords((void**)&paDeletedRecords[0], cDeletedRecords))
                    return FALSE;
                cDeletedRecords = 0;
            }
            paDeletedRecords[cDeletedRecords++] = pRecord;
        }
    }

    if (cDeletedRecords > 0)
        return pCnr->removeRecords((void**)&paDeletedRecords[0], cDeletedRecords);
    return TRUE;
}


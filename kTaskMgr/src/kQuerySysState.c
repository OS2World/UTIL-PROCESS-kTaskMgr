/* $Id: kQuerySysState.c,v 1.2 2000/06/19 20:08:42 stknut Exp $
 *
 * QuerySysState - system interface and data storage location.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */

/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
#define INCL_DOSERRORS
#define INCL_DOSPROCESS

/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

#include "kQuerySysState.h"

/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
static PPROCESSDATA     pProcFirst = NULL;
static PPROCESSDATA     pProcLast = NULL;
static PPROCESSDATA     pProcCur = NULL;
static qsLrec_t *       pMTEs = NULL;
static qsS16Headrec_t * p16Sems = NULL;
static qsS32rec_t *     p32Sems = NULL;
static qsMrec_t *       pMemRecs = NULL;
static qsMrec_t *       pShrMemRecs = NULL;
static qsFrec_t *       pFSRecs = NULL;
static qsGrec_t *       pGlobalRec = NULL;
static unsigned         cBuffers = 2;
static unsigned         cbBuffer = 1024*1024;
static char             achBuffer[2][1024*1024];
static unsigned         iCurBuffer = 1;


/**
 * Updates the status data.
 * @returns   TRUE - success
 *            FALSE - failure.
 * @status
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark    Single threaded!
 */
BOOL            QSUpdateStateData(void)
{
    APIRET      rc;

    /*
     * Find correct buffer, Zero it, and query sys stata.
     */
    iCurBuffer = (iCurBuffer + 1) % cBuffers;
    memset(achBuffer[iCurBuffer], 0, cbBuffer);
    rc = DosQuerySysState(QS_PROCESS | QS_SEMAPHORE | QS_MTE | QS_FILESYS | QS_SHMEMORY, /* information requested */
                          QS_MTE,                   /* Object info for MTEs */
                          0UL,                      /* PID. 0 is all processes. */
                          0UL,                      /* TID. 0 is all threads. (PID=0 too) */
                          achBuffer[iCurBuffer],    /* Pointer to databuffer. */
                          cbBuffer);                /* Size of databuffer. */
    if (rc == NO_ERROR)
    {
        qsPtrRec_t *    pPtrRec;        /* Pointer to pointer record at buffer start. */
        qsPrec_t *      pProcRec;       /* Current process record. */
        PPROCESSDATA    pProcData;

        /*
         * Local data
         */
        pPtrRec = (qsPtrRec_t*)achBuffer[iCurBuffer];
        pProcRec = pPtrRec->pProcRec;

        /*
         * Global data
         */
        pMTEs = pPtrRec->pLibRec;
        p16Sems = pPtrRec->p16SemRec;
        p32Sems = pPtrRec->p32SemRec;
        pMemRecs = pPtrRec->pMemRec;
        pShrMemRecs = pPtrRec->pShrMemRec;
        pFSRecs = pPtrRec->pFSRec;
        pGlobalRec = pPtrRec->pGlobalRec;

        /*
         * Set dead flag before starting to update and add processes.
         * When a process have been found in the new stat data, the
         * dead flag is cleared.
         */
        for (pProcData = pProcFirst; pProcData != NULL; pProcData = pProcData->pNext)
            pProcData->fDead = TRUE;

        /*
         * Loop thru all processes which was found and update or add
         * data to the linked list of processes.
         */
        while (pProcRec != NULL && pProcRec->RecType == QS_PROCESS)
        {
            unsigned long ulSysTime;    /* Used when calculating total time in system code. */
            unsigned long ulUserTime;   /* Used when calculating total time in user code. */
            unsigned      cbProcRec;    /* Size of qsPrec (with all additional data!) */
            int           i;            /* Loop counter (threads) */

            pProcData = QSGetProcessData(pProcRec->pid);
            if (pProcData == NULL)
            {   /*
                 * New process.
                 * Allocate a new process data struct and set to following:
                 *    usPid        to Pid number
                 *    fDirty       to TRUE
                 *    pProcRec     is NULL (indiactes a new process)
                 *    cbProcRec    is 0
                 *    pvRecordCore is NULL (indicates a new process)
                 * And insert the process data.
                 */
                pProcData = (PPROCESSDATA)calloc(sizeof(PROCESSDATA), 1);
                assert(pProcData != NULL); if (pProcData == NULL) break;
                pProcData->usPid = pProcRec->pid;
                pProcData->fDirty = TRUE;
                QSInsertProcessData(pProcData);
            }
            /* This process is not dead! fDead to false! */
            pProcData->fDead = FALSE;

            /*
             * Calc record size for the new data and see if it matches the size
             * of the old data for this process. If is doesn't match, something have been changed!
             */
            cbProcRec = (unsigned)(pProcRec->pThrdRec + pProcRec->cTCB) - (unsigned)pProcRec;
            if (pProcData->cbProcRec != cbProcRec)
            {
                pProcData->cbProcRec = cbProcRec;
                pProcData->fDirty = TRUE;
            }

            /*
             * Count total times and evt. update the pProcData struct.
             */
            for (i = 0, ulSysTime = ulUserTime = 0; i < pProcRec->cTCB; i++)
            {
                ulUserTime += pProcRec->pThrdRec[i].usertime;
                ulSysTime  += pProcRec->pThrdRec[i].systime;
            }
            if (pProcData->ulSysTime != ulSysTime || pProcData->ulUserTime != ulUserTime)
            {
                pProcData->fDirty = TRUE;
                pProcData->ulSysTime = ulSysTime;
                pProcData->ulUserTime = ulUserTime;
            }

            /*
             * Dirty check. If not allready dirty.
             */
            if (!pProcData->fDirty)
            {
                pProcData->fDirty =
                    pProcData->pProcRec->cTCB != pProcRec->cTCB
                    /* ||  FIXME */
                    ;
            }

            /*
             * Set Record pointer
             */
            pProcData->pProcRec = pProcRec;

            /* next - DEPENDS on the ThrdRecs to be last */
            pProcRec = (qsPrec_t*)(unsigned)(pProcRec->pThrdRec + pProcRec->cTCB);
        }
    }

    memset(achBuffer[(iCurBuffer+1) % cBuffers], 0, cbBuffer);

    return rc == NO_ERROR;
}


/**
 * Inserts a process data node.
 * @param     pProcData  Pointer to the node which is to inserted.
 * @status    completely  implemented.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark    The pProcData doubly-linked list is sorted on usPid.
 */
VOID            QSInsertProcessData(PPROCESSDATA pProcData)
{
    PPROCESSDATA pCur;
    BOOL         fDir;                  /* TRUE: Start --> End; FALSE: End --> Start*/
    assert(pProcData != NULL);

    /*
     * Semi optimized start location.
     */
    if (pProcCur == NULL)
        pCur = pProcLast;
    else
        pCur = pProcCur;

    /*
     * Find the right place for the new node in the list.
     */
    if (pCur != NULL)
    {
        if (pCur->usPid <= pProcData->usPid)
        {
            fDir = TRUE;
            while (pCur != NULL && pCur->usPid < pProcData->usPid)
                pCur = pCur->pNext;
        }
        else
        {
            fDir = FALSE;
            while (pCur != NULL && pCur->usPid >= pProcData->usPid)
                pCur = pCur->pPrev;

            /* insert before, so we'll have to goback one node */
            if (pCur != NULL)
                pCur = pCur->pNext;
        }
        assert(pCur == NULL || pCur->usPid != pProcData->usPid);
    }

    /*
     * case pCur == NULL && fDir:
     *      Last in the list. (or empty list)
     * case pCur == NULL && !fDir:
     *      First in the list. (or empty list)
     * case pCur != NULL:
     *      Insert before pCur
     */
    if (pCur == NULL)
    {
        if (fDir)
        {   /* last */
            pProcData->pNext = NULL;
            pProcData->pPrev = pProcLast;
            if (pProcLast != NULL)
                pProcLast->pNext = pProcData;
            else
                pProcFirst = pProcData;
            pProcLast = pProcData;
        }
        else
        {   /* first */
            pProcData->pNext = pProcFirst;
            pProcData->pPrev = NULL;
            if (pProcFirst != NULL)
                pProcFirst->pPrev = pProcData;
            else
                pProcLast = pProcData;
            pProcFirst = pProcData;
        }
    }
    else
    {   /* before */
        pProcData->pNext = pCur;
        pProcData->pPrev = pCur->pPrev;
        if (pCur->pPrev != NULL)
            pCur->pPrev->pNext = pProcData;
        else
            pProcFirst = pProcData;
        pCur->pPrev = pProcData;
    }

    /*
     * Set the last found node as this might optimize next access to the list.
     */
    pProcCur = pProcData;
}


/**
 * Removes a processdata node.
 * This function will free the memory occupied by pProcData!
 * @param     pProcData  Process data node
 * @status
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark
 */
VOID            QSRemoveProcessData(PPROCESSDATA pProcData)
{
    if (pProcData->pPrev != NULL)
        pProcData->pPrev->pNext = pProcData->pNext;
    else
        pProcFirst = pProcData->pNext;
    if (pProcData->pNext != NULL)
        pProcData->pNext->pPrev = pProcData->pPrev;
    else
        pProcLast = pProcData->pPrev;
    if (pProcCur == pProcData)
        pProcCur = pProcData->pPrev;
    free(pProcData);
}



/**
 * Finds a process data node by PID number.
 * @returns   NULL if not found. Pointer to node if found.
 * @param     usPid  Process identificator to find.
 * @status
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark
 */
PPROCESSDATA    QSGetProcessData(USHORT usPid)
{
    PPROCESSDATA pCur;

    /*
     * Semi optimized start location.
     */
    if (pProcCur == NULL)
        pCur = pProcLast;
    else
        pCur = pProcCur;

    /*
     * Find the right place for the new node in the list.
     */
    if (pCur != NULL)
    {
        if (pCur->usPid <= usPid)
        {
            while (pCur != NULL && pCur->usPid < usPid)
                pCur = pCur->pNext;
        }
        else
        {
            while (pCur != NULL && pCur->usPid > usPid)
                pCur = pCur->pPrev;
        }
    }

    return pCur != NULL && pCur->usPid == usPid ? pCur : NULL;
}


/**
 * Gets the first node in the process data list.
 * @returns   Pointer ot the first node in the process data list.
 *            If empty list, NULL is returned.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
PPROCESSDATA    QSGetFirstProcessDataNode(VOID)
{
    return pProcFirst;
}


/**
 * Gets a given MTE (library) record from the data returned from DosQuerySysState.
 * @returns   NULL if not found (or error). Pointer to MTE record if found.
 * @param     USHORT  hMTE.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
qsLrec_t *      QSGetMteData(register USHORT hMTE)
{
    /*
     * Searches lineary (no other option) thru the linked list of
     * Mte (Library) records returned by DosQuerySysState.
     */
    register qsLrec_t *pLrec = pMTEs;
    while (pLrec != NULL)
    {

        /*
         * Bug detected in OS/2 FP13. Probably a problem which occurs
         * in _LDRSysMteInfo when qsCheckCache is calle before writing
         * object info. The result is that the cache flushed and the
         * attempt of updating the qsLrec_t next and object pointer is
         * not done. This used to work earlier and on Aurora AFAIK.
         *
         * The fix for this problem is to check if the pObjInfo is NULL
         * while the number of objects isn't 0 and correct this. pNextRec
         * will also be NULL at this time. This will be have to corrected
         * before we exit the loop or moves to the next record.
         * There is also a nasty alignment of the object info... Hope
         * I got it right. (This aligment seems new to FP13.)
         */
        if (pLrec->pObjInfo == NULL /*&& pLrec->pNextRec == NULL*/ && pLrec->ctObj > 0)
            {
            pLrec->pObjInfo = (qsLObjrec_t*)(
                (char*)pLrec
                + ((sizeof(qsLrec_t)                     /* size of the lib record */
                   + pLrec->ctImpMod * sizeof(short)    /* size of the array of imported modules */
                   + strlen((char*)pLrec->pName) + 1    /* size of the filename */
                   + 3) & ~3));                          /* the size is align on 4 bytes boundrary */
            pLrec->pNextRec = (qsLrec_t*)((char*)pLrec->pObjInfo
                                           + sizeof(qsLObjrec_t) * pLrec->ctObj);
            }
        if (pLrec->hmte == hMTE)
            break;

        pLrec = pLrec->pNextRec;
    }
    return pLrec;
}


/**
 * Get the starting MTE data record.
 * @returns   Pointer to the first MTE data record.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
qsLrec_t *      QSGetMteFirstData(VOID)
{
    return pMTEs;
}


/**
 * Finds and returns the library record for DOSCALL1.DLL.
 * @returns Pointer to DosCall1.dll MTE data record.
 *          NULL pointer.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
qsLrec_t *      QSGetDosCall1Data(VOID)
{
    /*
     * Searches lineary (no other option) thru the linked list of
     * Mte (Library) records returned by DosQuerySysState.
     */
    register qsLrec_t *pLrec = pMTEs;
    while (pLrec != NULL)
    {

        /*
         * Bug detected in OS/2 FP13. Probably a problem which occurs
         * in _LDRSysMteInfo when qsCheckCache is calle before writing
         * object info. The result is that the cache flushed and the
         * attempt of updating the qsLrec_t next and object pointer is
         * not done. This used to work earlier and on Aurora AFAIK.
         *
         * The fix for this problem is to check if the pObjInfo is NULL
         * while the number of objects isn't 0 and correct this. pNextRec
         * will also be NULL at this time. This will be have to corrected
         * before we exit the loop or moves to the next record.
         * There is also a nasty alignment of the object info... Hope
         * I got it right. (This aligment seems new to FP13.)
         */
        if (pLrec->pObjInfo == NULL /*&& pLrec->pNextRec == NULL*/ && pLrec->ctObj > 0)
            {
            pLrec->pObjInfo = (qsLObjrec_t*)(
                (char*)pLrec
                + ((sizeof(qsLrec_t)                     /* size of the lib record */
                   + pLrec->ctImpMod * sizeof(short)    /* size of the array of imported modules */
                   + strlen((char*)pLrec->pName) + 1    /* size of the filename */
                   + 3) & ~3));                          /* the size is align on 4 bytes boundrary */
            pLrec->pNextRec = (qsLrec_t*)((char*)pLrec->pObjInfo
                                           + sizeof(qsLObjrec_t) * pLrec->ctObj);
            }
        if (strstr(pLrec->pName, "DOSCALL1.DLL") != NULL)
            break;

        pLrec = pLrec->pNextRec;
    }
    return pLrec;
}

/**
 * Get the first filesystem record.
 * @returns   NULL pointer if found or error.
 *            Pointer to FS record on success.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
qsFrec_t *      QSGetFSFirstData(VOID)
{
    return pFSRecs;
}


/**
 * Get the FS record for the given system file number.
 * @returns   NULL pointer if found or error.
 *            Pointer to FS record on success.
 * @param     usSFN  System File number.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
qsFrec_t *      QSGetSfnData(USHORT usSFN)
{
    /*
     * Searches lineary (no other option) thru the linked list of
     * FS records returned by DosQuerySysState.
     */
    register qsFrec_t * pCur = pFSRecs;
    while (pCur != NULL && pCur->RecType == QS_FILESYS)
    {
        register int i;
        for (i = 0; i < pCur->ctSft; i++)
            if (pCur->pSft[i].sfn == usSFN)
                return pCur;
        /* next */
        pCur = pCur->pNextRec;
    }


    return NULL;
}


/**
 * Get the first filesystem record.
 * @returns   NULL pointer if found or error.
 *            Pointer to FS record on success.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
qsMrec_t *      QSGetMemFirstData(VOID)
{
    return pMemRecs;
}


/**
 * Get the first filesystem record.
 * @returns   NULL pointer if found or error.
 *            Pointer to FS record on success.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
qsMrec_t *      QSGetShrMemFirstData(VOID)
{
    return pShrMemRecs;
}



/**
 * Debug function which dumps the process list.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
VOID            QSDumpProcessData(VOID)
{
    PPROCESSDATA pProcData = pProcFirst;

        printf("pid   ppid  cTCB  SysTime   UserTime  Dirty  Dead  Name\n");
    while (pProcData != NULL)
    {
        int i;
        qsLrec_t *pMTE = QSGetMteData(pProcData->pProcRec->hMte);

        printf("%04x  %04x  %04x  %000008x  %000008x  %-4d   %-4d  %s\n",
               pProcData->pProcRec->pid,
               pProcData->pProcRec->ppid,
               pProcData->pProcRec->cTCB,
               pProcData->ulSysTime,
               pProcData->ulUserTime,
               pProcData->fDirty,
               pProcData->fDead,
               pMTE != NULL && pMTE->pName != NULL ? pMTE->pName : "<not found>"
               );
        if (pProcData->pProcRec->pFSRec
            && pProcData->pProcRec->cFH > 0)
            for (i = 0; i < pProcData->pProcRec->cFH; i++)
            {
                qsFrec_t * pFSRec = QSGetSfnData(pProcData->pProcRec->pFSRec[i]);
                printf("    sfn %#06x  %s\n", pProcData->pProcRec->pFSRec[i],
                       pFSRec != NULL ? (char*)(pFSRec + 1) : "<not found>"
                       );
            }

        /* next */
        pProcData = pProcData->pNext;
    }
}


/**
 * Debug function which dumps the MFS list.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
VOID            QSDumpFileSystemData(VOID)
{
    qsFrec_t *pCur = pFSRecs;

    while (pCur != NULL && pCur->RecType == QS_FILESYS)
    {
        int i;
        printf("%s  (",
               pCur + 1
               );
        for (i = 0; i < pCur->ctSft; i++)
            printf("%d,", pCur->pSft[i].sfn);
        printf(")\n");
        /* next */
        pCur = pCur->pNextRec;
    }

    pCur = pFSRecs;
    while (pCur != NULL && pCur->RecType == QS_FILESYS)
    {
        int i;
        for (i = 0; i < pCur->ctSft; i++)
            printf("sfn 0x%04x  %s\n", pCur->pSft[i].sfn, pCur + 1);

        /* next */
        pCur = pCur->pNextRec;
    }

}


#if 0
void main(void)
{
    QSUpdateStateData();
    QSDumpProcessData();
    QSDumpFileSystemData();
}
#endif


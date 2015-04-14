/* $Id: kModuleContainer.cpp,v 1.2 2000/05/23 00:51:25 stknut Exp $
 *
 * kModuleContainer - generic module container.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */

/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
#define INCL_WIN
#define INCL_GPI
#define INCL_BASE


/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <os2.h>
#ifdef USE_KLIB
    #include <kAssert.h>
    #include <kLog.h>
    #include <kHeap.h>
#else
    #include <malloc.h>
#endif
#include <memory.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

#include "kBase.h"
#include "kError.h"
#include "kDlgBase.h"
#include "kMenuBase.h"
#include "kClickDlg.h"
#include "kContainer.h"
#include "kNotebookBase.h"
#include "kNotebookPageBase.h"

#include "kQuerySysState.h"
#include "kDetailsBase.h"
#include "kTaskMgrRecordBase.h"
#include "kTaskMgrContainerBase.h"

#include "kModuleRecord.h"
#include "kModuleContainer.h"
#include "kObjectRecord.h"
#include "kObjectContainer.h"
#include "kModuleDetails.h"
#include "kTaskMgr.h"
#include "kTaskMgr_defs.h"


/**
 * Updates the content of the container if ALL modules are to be included. (hMte = 0xFFFF)
 * @returns   success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL   kModuleContainer::insertAllModules()
{
    qsLrec_t *      pCur;
    int             c;

    ASSERT(hMte == 0xFFFF);
    ASSERT(usPid == 0xFFFF);

    /*
     * Loop thru all MTE records and count them
     */
    #if 0
    for (c = 0, pCur = QSGetMteFirstData(); pCur != NULL; pCur = (qsLrec_t*)pCur->pNextRec)
        c++;
    #else
    c = 0;
    pCur = QSGetMteFirstData();
    while (pCur != NULL)
        {
        c++;
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
        if (pCur->pObjInfo == NULL /*&& pCur->pNextRec == NULL*/ && pCur->ctObj != 0)
            {
            pCur->pObjInfo = (qsLObjrec_t*)(
                (char*)pCur
                + ((sizeof(qsLrec_t)                     /* size of the lib record */
                   + pCur->ctImpMod * sizeof(short)    /* size of the array of imported modules */
                   + strlen((char*)pCur->pName) + 1    /* size of the filename */
                   + 3) & ~3UL));                          /* the size is align on 4 bytes boundrary */
            pCur->pNextRec = (qsLrec_t*)((char*)pCur->pObjInfo
                                         + sizeof(qsLObjrec_t) * pCur->ctObj);
            }

        /* next */
        pCur = (qsLrec_t*)pCur->pNextRec;
        }

    #endif

    if (c > 0)
    {
        kModuleRecord  *pCurCnrRec, *pCnrRec;

        /*
         * Allocate container records for all the MTEs
         */
        pCurCnrRec = pCnrRec = (kModuleRecord*)allocMiniRec(sizeof(kModuleRecord), c);
        if (pCurCnrRec == NULL)
            return FALSE;               /* FIXME: complain about this! */

        /*
         * Loop thru the list of processes and update the container.
         */
        pCur = QSGetMteFirstData();
        while (pCur != NULL)
        {
            /*
             * Init and set the record.
             */
            pCurCnrRec->init();
            pCurCnrRec->set(pCur);


            /* next */
            pCur = (qsLrec_t*)pCur->pNextRec;
            pCurCnrRec = (kModuleRecord*)pCurCnrRec->getNext();
        }

        /*
         * Insert the record at bottom of the container.
         */
        return insertAtTop(pCnrRec, c);
    }


    return TRUE;

}


/**
 * Inserts the imported module info into the container for it.
 * @returns   success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL kModuleContainer::insertImpModules()
{
    qsLrec_t *pMteRec;

    ASSERT(hMte < 0xFFFF);
    ASSERT(usPid == 0xFFFF);

    /* Find the module indentified by hMte */
    pMteRec = QSGetMteData(hMte);
    if (pMteRec != NULL)
    {
        int c = (int)pMteRec->ctImpMod;
        int i;

        /* Are there any at all? */
        if (c > 0)
        {
            kModuleRecord * pCnrRec,
                          * pCurCnrRec;
            PUSHORT         pahmte = (PUSHORT)((int)pMteRec + sizeof(qsLrec_t));

            /*
             * Allocate container records.
             */
            pCurCnrRec = pCnrRec = (kModuleRecord*)allocMiniRec(sizeof(kModuleRecord), c);
            if (pCurCnrRec == NULL)
                return FALSE;               /* FIXME - complain about this! */

            /*
             * Fill the records with data.
             */
            for (i = 0; i < c; i++, pCurCnrRec = (kModuleRecord*)pCurCnrRec->getNext())
            {
                qsLrec_t *pMteImpRec;

                /*
                 * Initiate record.
                 */
                pCurCnrRec->init();

                /*
                 * Get Mte record.
                 */
                pMteImpRec = QSGetMteData(pahmte[i]);
                if (pMteImpRec != NULL)
                    pCurCnrRec->set(pMteImpRec);
                else
                    pCurCnrRec->set(pahmte[i]);
            }

            /*
             * Insert records
             */
            return insertAtTop(pCnrRec, c);
        }
    }
    else
        return FALSE;

    return TRUE;
}


/**
 * Inserts the loaded module info into the container for it.
 * @returns   success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL kModuleContainer::insertLoadedModules()
{
    PPROCESSDATA    pProcData;
    qsLrec_t *      pMteRec;
    PUSHORT         pahmte;

    ASSERT(usPid < 0xFFFF);
    ASSERT(hMte == 0xFFFF);

    /*
     * Find the process indentified by usPid
     */
    pProcData = QSGetProcessData(usPid);
    if (pProcData == NULL)
        return FALSE;
    pMteRec = QSGetMteData(hMte);
    pahmte = pProcData->pProcRec->pLibRec;
    if (pahmte != NULL)
    {
        int c = (int)pProcData->pProcRec->cLib;
        int i;

        /* Are there any at all? */
        if (c > 0)
        {
            kModuleRecord * pCnrRec,
                          * pCurCnrRec;

            /*
             * Allocate container records.
             */
            pCurCnrRec = pCnrRec = (kModuleRecord*)allocMiniRec(sizeof(kModuleRecord), c);
            if (pCurCnrRec == NULL)
                return FALSE;               /* FIXME - complain about this! */

            /*
             * Fill the records with data.
             */
            for (i = 0; i < c; i++, pCurCnrRec = (kModuleRecord*)pCurCnrRec->getNext())
            {
                qsLrec_t *pMteImpRec;

                /*
                 * Initiate record.
                 */
                pCurCnrRec->init();

                /*
                 * Get Mte record.
                 */
                pMteImpRec = QSGetMteData(pahmte[i]);
                if (pMteImpRec != NULL)
                    pCurCnrRec->set(pMteImpRec);
                else
                    pCurCnrRec->set(pahmte[i]);
            }

            /*
             * Insert records
             */
            return insertAtTop(pCnrRec, c);
        }
    }
    else
        return FALSE;

    return TRUE;
}



/**
 * Inserts the loaded and imported module info into the container for it.
 * @returns   success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL kModuleContainer::insertAllAllModules()
{
    PPROCESSDATA    pProcData;
    qsLrec_t *      pMteRec;
    PUSHORT         pahmte;
    int             c;
    int             i;
    kModuleRecord * pCnrRec,
                  * pCurCnrRec,
                  * pLastCnrRec;

    ASSERT(usPid < 0xFFFF);
    ASSERT(hMte < 0xFFFF);

    /*
     * Find the process indentified by usPid
     * And the Mte for it.
     */
    pProcData = QSGetProcessData(usPid);
    if (pProcData == NULL)
        return FALSE;
    hMte = pProcData->pProcRec->hMte; //makesure hMte is correct!

    pMteRec = QSGetMteData(hMte);

    /*
     * Count Loaded and exe module.
     */
    c = (int)pProcData->pProcRec->cLib + 1;

    /*
     * Allocate container records.
     */
    pLastCnrRec = pCurCnrRec = pCnrRec = (kModuleRecord*)allocMiniRec(sizeof(kModuleRecord), c);
    if (pCurCnrRec == NULL)
        return FALSE;               /* FIXME - complain about this! */

    /*
     * Fill the record with data.
     * (Executable module and Loaded modules.)
     */
    pCurCnrRec->init();
    if (pMteRec != NULL)
        pCurCnrRec->set(pMteRec);
    else
        pCurCnrRec->set(hMte);
    pCurCnrRec = (kModuleRecord*)pCurCnrRec->getNext();
    pahmte = pProcData->pProcRec->pLibRec;
    ASSERT(pahmte != NULL);
    for (i = 1; i < c; i++, pCurCnrRec = (kModuleRecord*)pCurCnrRec->getNext())
    {
        pMteRec = QSGetMteData(pahmte ? pahmte[i] : (USHORT)0xffff);
        pCurCnrRec->init();
        if (pMteRec != NULL)
            pCurCnrRec->set(pMteRec);
        else
            pCurCnrRec->set(pahmte ? pahmte[i] : (USHORT)0xffff);
        pLastCnrRec = pCurCnrRec;
    }

    ASSERT(pCurCnrRec == NULL);

    /*
     * Now we should load all the imported dlls.
     * We'll loop thru the container records loading imported modules that
     * isn't allready loaded. These new modules are added to the end of the
     * chain of container records and will hence have their imports loaded
     * we get to them.
     */
    pCurCnrRec = pCnrRec;
    while (pCurCnrRec != NULL)
    {
        /*
         * Get library record for this mte from QS and see if there are any
         * imported modules in it.
         */
        pMteRec = QSGetMteData(pCurCnrRec->gethMte());
        if (pMteRec != NULL && pMteRec->ctImpMod != 0)
        {
            /*
             * Loop thru the array of imported module handles and check that
             * they are loaded (present in the container).
             * If they aren't load them.
             */
            pahmte = (PUSHORT)((int)pMteRec + sizeof(qsLrec_t));
            for (i = 0; i < pMteRec->ctImpMod; i++)
            {
                /* look for this mte among the container records */
                kModuleRecord * pSearchCnrRec = pCnrRec;
                while (pSearchCnrRec != NULL && pSearchCnrRec->gethMte() != pahmte[i])
                    pSearchCnrRec = (kModuleRecord *)pSearchCnrRec->getNext();

                if (pSearchCnrRec == NULL)
                {   /* no record for this mte, make one and add it! */
                    qsLrec_t *  pImpMteRec;
                    pLastCnrRec = (kModuleRecord *)pLastCnrRec->setNext(this->allocMiniRec(sizeof(kModuleRecord)));
                    if (pLastCnrRec == NULL)
                    {   /*
                         * Don't panic. Failed to allocate records...
                         * !FIXME! Complain! !TODO!
                         * Add them one we've got and return.
                         */
                        insertAtTop(pCnrRec, c);
                        return FALSE;
                    }
                    c++;

                    /*
                     * Initiate and fill the record.
                     */
                    pLastCnrRec->init();
                    pImpMteRec = QSGetMteData(pahmte[i]);
                    if (pImpMteRec != NULL)
                        pLastCnrRec->set(pImpMteRec);
                    else
                        pLastCnrRec->set(pahmte[i]);
                }
            }
        }

        /*
         * Next module record
         */
        pCurCnrRec = (kModuleRecord *)pCurCnrRec->getNext();
    }

    return insertAtTop(pCnrRec, c);
}



/**
 * Command events.
 * @param     usCmd     Control id which send/posted the message.
 * @param     usSource  Source id.
 * @param     fPointer  Mouse pointer flag.
 * @remark    dismisses the dialog if DID_OK or DID_CANCEL.
 */
VOID  kModuleContainer::command(USHORT usCmd, USHORT usSource, BOOL fPointer)
{
    kModuleRecord * pCurRecord = (kModuleRecord *)getCurRecord();

    switch (usCmd)
    {
        case IDM_CNR_MOD_DETAILS:
            if (pCurRecord)
                kModuleDetails::invoke(pCurRecord->gethMte(), hwndCnr);
            break;

        default:
            kTaskMgrContainerBase::command(usCmd, usSource, fPointer);
    }

    usSource = usSource;
    fPointer = fPointer;
}


/**
 * Constructor.
 * @returns
 * @param     hwndDlg   Handle to dialog window.
 * @param     ulCnrId   ID of the container dialog item in hwndDlg.
 * @param     hMte      hMte = 0xFFFF:  View all modules
 *                      hMte < 0xFFFF: View dependants of this module
 *            usPid
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
kModuleContainer::kModuleContainer(HWND hwndDlg, ULONG ulCnrId, USHORT hMte/*= 0xffff*/, USHORT usPid/* = 0xffff*/) throw(kError)
    : kTaskMgrContainerBase(hwndDlg, ulCnrId,
                            usPid < 0xffff ?
                              hMte == 0xffff ? "Loaded Module Overview"
                                             : "All Modules (Imported, Loaded and Exe) Overview"
                            : hMte == 0xffff ? "Executable Module Overview"
                                             : "Imported Module Overview",
                            kModuleRecord::cFieldInfo,
                            (PFIELDINFO)&kModuleRecord::aFieldInfo[0],
                            IDM_CNR_MODULE_ALL,
                            IDM_CNR_MODULE,
                            IDM_CNR_MOD_DETAILS,
                            0),
    hMte(hMte), usPid(usPid)
{
    TRACELOG_CONSTRUCT("start");
    TRACELOG_CONSTRUCT("end");
}


/**
 * Destructor.
 */
kModuleContainer::~kModuleContainer()
{
    TRACELOG_DESTRUCT("start");
    TRACELOG_DESTRUCT("end");
}

/**
 * Updates the contents of the container.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
VOID  kModuleContainer::update()
{
    TRACELOG_UPDATE("start");
    /*
     * Delete all records in the container!
     */
    removeAllRecords();

    /*
     * Insert records
     */
    if (usPid == 0xffff)
    {
        if (hMte == 0xffff)
            insertAllModules();
        else
            insertImpModules();
    }
    else
    {
        if (hMte == 0xffff)
            insertLoadedModules();
        else
            insertAllAllModules();
    }
    TRACELOG_UPDATE("end");
}


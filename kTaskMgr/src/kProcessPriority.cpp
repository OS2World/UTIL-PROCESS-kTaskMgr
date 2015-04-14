/* $Id: kProcessPriority.cpp,v 1.1 2000/05/23 00:12:45 stknut Exp $
 *
 * kProcessPriority (kTaskMgr) - Change Process Priority dialog.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
#define INCL_DOS
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
#include <stdio.h>

#include "kBase.h"
#include "kError.h"
#include "kDlgBase.h"
#include "kMenuBase.h"
#include "kContainer.h"
#include "kNotebookPageBase.h"
#include "kNotebookBase.h"

#include "kQuerySysState.h"
#include "kTaskMgrRecordBase.h"
#include "kTaskMgrContainerBase.h"

#include "kThreadRecord.h"
#include "kThreadContainer.h"
#include "kProcessPriority.h"
#include "kTaskMgr_Defs.h"
#include "kTaskMgr.h"



/**
 * Forwards this event to the correct container.
 * @param     usId     Container id.
 * @param     pRecord  Pointer to the record which is selected by either the key
 */
VOID kProcessPriority::cnrContextMenu(USHORT usId, PRECORDCORE pRecord)
{
    if (usId == KPROCPRTY_CNR_THREADS)
        pCnrThreads->cnrContextMenu(usId, pRecord);
}


/**
 * Forwards this event to the correct container.
 */
VOID kProcessPriority::cnrEnter(USHORT usId, HWND hwndCnr, PRECORDCORE pRecord, ULONG fKey)
{
    if (usId == KPROCPRTY_CNR_THREADS)
        pCnrThreads->cnrEnter(usId, hwndCnr, pRecord, fKey);
}


/**
 * Command event handler.
 * This method processes the DID_OK and DID_CANCEL command events.
 * @param     usCmd     Control id which send/posted the message.
 * @param     usSource  Source id.
 * @param     fPointer  Mouse pointer flag.
 */
VOID kProcessPriority::command(USHORT usCmd, USHORT usSource, BOOL fPointer)
{
    switch (usCmd)
    {
        case DID_OK:
        {
            APIRET  rc;
            LONG    lDelta;
            ULONG   ulClass;
            CHAR    szBuffer[256];

            /*
             * Start off by collecting data.
             */
            ulClass = (ULONG)WinSendDlgItemMsg(hwnd, KPROCPRTY_CB_CLASS, LM_QUERYSELECTION, (MPARAM)LIT_FIRST, NULL);
            if (ulClass == (ULONG)LIT_NONE)
            {
                //TODO: complain!
                break;
            }

            if (!(BOOL)WinSendDlgItemMsg(hwnd, KPROCPRTY_SPB_DELTA, SPBM_QUERYVALUE, &lDelta, 0))
            {
                //TODO: complain!
                break;
            }


            /*
             * ARG! TODO! FIXME!
             *
             * This didn't seem to work as we're only allowed to change priority on child processes.
             * But, this might turn out as a feature in win32k.sys some time the future.
             *
             * ARG! TODO! FIXME!
             */
            rc = DosSetPriority(PRTYS_PROCESSTREE, ulClass, lDelta, usPid);
            sprintf(szBuffer, "Changed priority to: ulClass=%d, lDelta=%d, rc=%d\n",
                    ulClass, lDelta, rc);
            WinMessageBox(HWND_DESKTOP, hwnd,szBuffer, "kTaskMgr - Info", 0, MB_OK | MB_INFORMATION);
            break;
        }

        case DID_CANCEL:
            close();
            break;
    }

    /* unreferenced */
    usSource = usSource;
    fPointer = fPointer;
}


/**
 * Process change priority dialog.
 * NOTE: This dialog is currently an simple interface to DosSetPriority.
 * @param     usPid        Process identifier of the process requested priority change for.
 * @param     hwndOwner    Handle of the owner.
 */
kProcessPriority::kProcessPriority(USHORT usPid, HWND hwndOwner) throw(kError)
  : kDlgBase(DL_KPROCESS_CHANGE_PRIORITY, NULLHANDLE, NULLHANDLE, HWND_DESKTOP),
    hwndOwner(hwndOwner), pCnrThreads(NULL), usPid(usPid)
{
    TRACELOG_CONSTRUCT("start");
    CHAR            szBuffer[256];
    PPROCESSDATA    pProcData;
    qsLrec_t *      pMteRec;

    /* Find process data. */
    pProcData = QSGetProcessData(usPid);
    if (pProcData == NULL)
        throw(kError(ERROR_INVALID_PARAMETER, kError::dos));

    /* Find hMte record */
    pMteRec = QSGetMteData(pProcData->pProcRec->hMte);
    if (pMteRec == NULL)
        throw(kError(ERROR_INVALID_PARAMETER, kError::dos));

    /*
     * Set title.
     */
    sprintf(&szBuffer[0], "PID %04x - %.240s",
            usPid, pMteRec->pName);
    setTitle(&szBuffer[0]);


    /*
     * Initiate combo box.
     */
    WinSendDlgItemMsg(hwnd, KPROCPRTY_CB_CLASS, LM_INSERTITEM, (MPARAM)PRTYC_NOCHANGE,          "No change");
    WinSendDlgItemMsg(hwnd, KPROCPRTY_CB_CLASS, LM_INSERTITEM, (MPARAM)PRTYC_IDLETIME,          "Idletime Priority Class");
    WinSendDlgItemMsg(hwnd, KPROCPRTY_CB_CLASS, LM_INSERTITEM, (MPARAM)PRTYC_REGULAR,           "Regular Priority Class");
    WinSendDlgItemMsg(hwnd, KPROCPRTY_CB_CLASS, LM_INSERTITEM, (MPARAM)PRTYC_TIMECRITICAL,      "Timecritical Priority Class");
    WinSendDlgItemMsg(hwnd, KPROCPRTY_CB_CLASS, LM_INSERTITEM, (MPARAM)PRTYC_FOREGROUNDSERVER,  "Foregroundserver Priority Class");
    if (pProcData->pProcRec->pThrdRec != NULL)
        WinSendDlgItemMsg(hwnd, KPROCPRTY_CB_CLASS, LM_SELECTITEM,
                          (MPARAM)(pProcData->pProcRec->pThrdRec->priority >> 8),
                          (MPARAM)TRUE);
    WinSetFocus(HWND_DESKTOP, WinWindowFromID(hwnd, KPROCPRTY_CB_CLASS));


    /*
     * Initiate spinbutton.
     */
    WinSendDlgItemMsg(hwnd, KPROCPRTY_SPB_DELTA, SPBM_SETLIMITS, (MPARAM)PRTYD_MAXIMUM, (MPARAM)PRTYD_MINIMUM);
    if (pProcData->pProcRec->pThrdRec != NULL)
        WinSendDlgItemMsg(hwnd, KPROCPRTY_SPB_DELTA, SPBM_SETCURRENTVALUE,
                          (MPARAM)(pProcData->pProcRec->pThrdRec->priority & 0xFF), NULL);


    /*
     * Create the container
     */
    pCnrThreads     = new kThreadContainer(hwnd, KPROCPRTY_CNR_THREADS, usPid);

    /*
     * Set dialog data.
     */
    update();

    TRACELOG_CONSTRUCT("end");
}


/**
 * Destructor stub.
 */
kProcessPriority::~kProcessPriority()
{
    TRACELOG_DESTRUCT("start");
    if (pCnrThreads)
        delete pCnrThreads;
    TRACELOG_DESTRUCT("end");
}



/**
 * Update function - update dialog contents.
 * @returns   Success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL kProcessPriority::update()
{
    TRACELOG_UPDATE("start");
    PPROCESSDATA    pProcData;
    qsLrec_t *      pMteRec;


    /* Find process data. */
    pProcData = QSGetProcessData(usPid);
    if (pProcData == NULL)
        throw(kError(ERROR_INVALID_PARAMETER, kError::dos));

    /* Find hMte record */
    pMteRec = QSGetMteData(pProcData->pProcRec->hMte);
    if (pMteRec == NULL)
        throw(kError(ERROR_INVALID_PARAMETER, kError::dos));


    /*
     * Set field contents
     */
    /*
    setDlgItemText(KMODDTL_TXT_HMTE,        "0x%04x",   hMTE);
    setDlgItemText(KMODDTL_TXT_MODEL,       pMteRec->fFlat ? "32-bit (LX)" : "16-bit (NE)");
    setDlgItemText(KMODDTL_TXT_COBJECTS,    "%d (0x%x)", pMteRec->ctObj, pMteRec->ctObj);
    setDlgItemText(KMODDTL_TXT_CIMPMODULES, "%d (0x%x)", pMteRec->ctImpMod, pMteRec->ctImpMod);
    setDlgItemText(KMODDTL_TXT_NAME,        "%s",        pMteRec->pName != NULL ? (char*)pMteRec->pName : "<unknown>");
    */

    /*
     * Fill the containers.
     */
    pCnrThreads->update();

    TRACELOG_UPDATE("end");
    return TRUE;
}


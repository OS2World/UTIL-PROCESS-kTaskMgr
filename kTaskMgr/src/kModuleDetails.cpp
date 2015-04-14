/* $Id: kModuleDetails.cpp,v 1.2 2000/05/23 00:51:25 stknut Exp $
 *
 * Module Detail Dialog.
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
#include "kNotebookBase.h"
#include "kNotebookPageBase.h"

#include "kQuerySysState.h"
#include "kDetailsBase.h"
#include "kTaskMgrRecordBase.h"
#include "kTaskMgrContainerBase.h"

#include "kObjectRecord.h"
#include "kObjectContainer.h"
#include "kModuleRecord.h"
#include "kModuleContainer.h"
#include "kModuleDetails.h"
#include "kTaskMgr_Defs.h"
#include "kTaskMgr.h"



/**
 * Forwards this event to the correct container.
 * @param     usId     Container id.
 * @param     pRecord  Pointer to the record which is selected by either the key
 */
VOID kModuleDetails::cnrContextMenu(USHORT usId, PRECORDCORE pRecord)
{
    if (usId == KMODDTL_CNR_IMPMODULES)
        pCnrImpModules->cnrContextMenu(usId, pRecord);
    else
        pCnrObjects->cnrContextMenu(usId, pRecord);
}


/**
 * Forwards this event to the correct container.
 */
VOID kModuleDetails::cnrEnter(USHORT usId, HWND hwndCnr, PRECORDCORE pRecord, ULONG fKey)
{
    if (usId == KMODDTL_CNR_IMPMODULES)
        pCnrImpModules->cnrEnter(usId, hwndCnr, pRecord, fKey);
    else
        pCnrObjects->cnrEnter(usId, hwndCnr, pRecord, fKey);
}


/**
 * Resizes the dialog.
 * This will resize the individual components.
 * @param     pswpOld  Old window size.
 * @param     pswpNew  New window size.
 */
VOID kModuleDetails::resize(PSWP pswpOld, PSWP pswpNew)
{
    /* FIXME - not implemented */
    pswpNew = pswpNew;
    pswpOld = pswpOld;
}



/**
 * Module detail dialog.
 * @param     hMTE         Handle of the module which we are to view details for.
 * @param     hwndOwner    Handle of the owner.
 */
kModuleDetails::kModuleDetails(USHORT hMTE, HWND hwndOwner) throw(kError)
  : kDlgBase(DL_KMODULEDETAILS, NULLHANDLE, NULLHANDLE, HWND_DESKTOP),
    kDetailsBase(hwnd),
    hwndOwner(hwndOwner), pCnrObjects(NULL), pCnrImpModules(NULL),
    hMTE(hMTE)
{
    TRACELOG_CONSTRUCT("start");
    CHAR        szBuffer[256];
    qsLrec_t *  pMteRec;

    /* find hMte record */
    pMteRec = QSGetMteData(hMTE);
    if (pMteRec == NULL)
        throw(kError(ERROR_INVALID_PARAMETER, kError::dos));

    /*
     * Set title.
     */
    sprintf(&szBuffer[0], "hMTE %04x - %.240s",
            hMTE, pMteRec->pName);
    setTitle(&szBuffer[0]);


    /*
     * Create the two containers
     */
    pCnrObjects     = new kObjectContainer(hwnd, KMODDTL_CNR_OBJECTS, hMTE);
    pCnrImpModules  = new kModuleContainer(hwnd, KMODDTL_CNR_IMPMODULES, hMTE);

    /*
     * Set dialog data.
     */
    update();

    TRACELOG_CONSTRUCT("end");
}


/**
 * Destructor stub.
 */
kModuleDetails::~kModuleDetails()
{
    TRACELOG_DESTRUCT("start");
    if (pCnrImpModules)
        delete pCnrImpModules;
    if (pCnrObjects)
        delete pCnrObjects;
    TRACELOG_DESTRUCT("end");
}



/**
 * Update function - update dialog contents.
 * @returns   Success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL kModuleDetails::update()
{
    TRACELOG_UPDATE("start");
    qsLrec_t *  pMteRec;

    /* find hMte record */
    pMteRec = QSGetMteData(hMTE);
    if (pMteRec == NULL)
        throw(kError(ERROR_INVALID_PARAMETER, kError::dos));

    /*
     * Set field contents
     */
    setDlgItemText(KMODDTL_TXT_HMTE,        "0x%04x",   hMTE);
    setDlgItemText(KMODDTL_TXT_MODEL,       pMteRec->fFlat ? "32-bit (LX)" : "16-bit (NE)");
    setDlgItemText(KMODDTL_TXT_COBJECTS,    "%d (0x%x)", pMteRec->ctObj, pMteRec->ctObj);
    setDlgItemText(KMODDTL_TXT_CIMPMODULES, "%d (0x%x)", pMteRec->ctImpMod, pMteRec->ctImpMod);
    setDlgItemText(KMODDTL_TXT_NAME,        "%s",        pMteRec->pName != NULL ? (char*)pMteRec->pName : "<unknown>");

    /*
     * Fill the containers.
     */
    pCnrObjects->update();
    pCnrImpModules->update();

    TRACELOG_UPDATE("end");
    return TRUE;
}


/**
 * Invokes an modeless module details dialog.
 * @returns     TRUE if dialog was invoked successfully.
 *              FALSE on failure.
 * @param       hMte        Module handle.
 * @param       hwndOwner   Handle to owner window.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL kModuleDetails::invoke(USHORT hMte, HWND hwndOwner)
{
    try
    {
        kModuleDetails *pDlg = new kModuleDetails(hMte, hwndOwner);
        return pDlg->show();
    }
    catch (kError err)
    {
        err.showError(kTaskMgr::pszErrorTitle);
    }
    return FALSE;
}


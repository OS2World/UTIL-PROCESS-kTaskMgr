/* $Id: kObjectContainer.cpp,v 1.1 2000/05/23 00:12:45 stknut Exp $
 *
 * kObjectContainer - generic object container.
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
#include "kTaskMgrRecordBase.h"
#include "kTaskMgrContainerBase.h"

#include "kObjectRecord.h"
#include "kObjectContainer.h"
#include "kTaskMgr.h"
#include "kTaskMgr_defs.h"


/**
 * Updates the content of the container.
 * @returns   success indicator.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
BOOL   kObjectContainer::insertObjects()
{
    qsLrec_t *      pMteData;
    int             c;

    /*
     * Get data for this MTE and get the object count.
     */
    pMteData = QSGetMteData(hMTE);
    if (pMteData == NULL)
        return FALSE;

    c = (int)pMteData->ctObj;

    if (c > 0 && pMteData->pObjInfo != NULL)
    {
        int i;
        kObjectRecord  *pCurCnrRec, *pCnrRec;

        /*
         * Allocate container records for all the objects.
         */
        pCurCnrRec = pCnrRec = (kObjectRecord*)allocMiniRec(sizeof(kObjectRecord), c);
        if (pCurCnrRec == NULL)
            return FALSE;               /* FIXME: complain about this! */

        /*
         * Loop thru the list of processes and update the container.
         */
        for (i = 0; i < c; i++, pCurCnrRec = (kObjectRecord*)pCurCnrRec->getNext())
        {
            /*
             * Init and set the record.
             */
            pCurCnrRec->init();
            pCurCnrRec->set(&pMteData->pObjInfo[i], i);
        }

        /*
         * Insert the record at bottom of the container.
         */
        return insertAtTop(pCnrRec, c);
    }

    return FALSE;
}



/**
 * Command events - none yet.
 * @param     usCmd     Control id which send/posted the message.
 * @param     usSource  Source id.
 * @param     fPointer  Mouse pointer flag.
 */
VOID  kObjectContainer::command(USHORT usCmd, USHORT usSource, BOOL fPointer)
{
    switch (usCmd)
    {
        default:
            kTaskMgrContainerBase::command(usCmd, usSource, fPointer);
    }
    usSource = usSource;
    fPointer = fPointer;
}



/**
 * Constructor.
 * @param     hwndDlg   Handle to dialog window.
 * @param     ulCnrId   ID of the container dialog item in hwndDlg.
 * @param     usMTE     Module handle.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
kObjectContainer::kObjectContainer(HWND hwndDlg, ULONG ulCnrId, USHORT hMTE) throw(kError)
    : kTaskMgrContainerBase(hwndDlg, ulCnrId,
                            "Object Overview",
                            kObjectRecord::cFieldInfo,
                            (PFIELDINFO)&kObjectRecord::aFieldInfo[0],
                            IDM_CNR_OBJECT_ALL),
    hMTE(hMTE)
{
    TRACELOG_CONSTRUCT("start");
    TRACELOG_CONSTRUCT("end");
}


/**
 * Destructor.
 */
kObjectContainer::~kObjectContainer()
{
    TRACELOG_DESTRUCT("start");
    TRACELOG_DESTRUCT("end");
}


/**
 * Updates the contents of the container - Note that this kind of info doesn't normally change!
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
VOID  kObjectContainer::update()
{
    TRACELOG_UPDATE("start");
    this->insertObjects();
    TRACELOG_UPDATE("end");
}



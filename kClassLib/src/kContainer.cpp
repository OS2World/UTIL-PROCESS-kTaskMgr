/* $Id: kContainer.cpp,v 1.1 2000/05/23 00:00:54 stknut Exp $
 *
 * kContainer (kClassLib) - Implementation of kContainer.
 *
 * Copyright (c) 1999-2000 knut st. osmundsen
 *
 */


/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
#define INCL_WINERRORS
#define INCL_WINSTDCNR
#define INCL_WINSYS
#define INCL_WINMESSAGEMGR
#define INCL_WINWINDOWMGR
#define INCL_WININPUT
#define INCL_WINSCROLLBARS
#define INCL_WINSTATICS

/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <os2.h>
#ifdef USE_KLIB
    #include <kAssert.h>
    #include <kLog.h>
    #include <kHeap.h>
#endif
#include <stdarg.h>

#include "kBase.h"
#include "kError.h"
#include "kContainer.h"



/*******************************************************************************
*   Internal Functions                                                         *
*******************************************************************************/
static MRESULT _System forwarder(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);




/**
 * Gets the next pointer from the mini record core.
 * @returns   Pointer to next record. NULL if last.
 */
kCnrMiniRecord *kCnrMiniRecord::getNext()
{
    return(kCnrMiniRecord*)RecordCore.preccNextRecord;
}



/**
 * Gets the next pointer from the record core.
 * @returns   Pointer to next record. NULL if last.
 */
kCnrRecord *kCnrRecord::getNext()
{
    return (kCnrRecord*)RecordCore.preccNextRecord;
}



/**
 * sets the next pointer int the mini record core.
 * @returns   pRecord.
 * @param     pRecord   New next pointer.
 */
kCnrMiniRecord *kCnrMiniRecord::setNext(kCnrMiniRecord *pRecord)
{
    RecordCore.preccNextRecord = (PMINIRECORDCORE)pRecord;
    return pRecord;
}



/**
 * Sets the next pointer in the record core.
 * @returns   pRecord.
 * @param     pRecord   New next pointer.
 */
kCnrRecord *kCnrRecord::setNext(kCnrRecord *pRecord)
{
    RecordCore.preccNextRecord = (PRECORDCORE)pRecord;
    return pRecord;
}



/**
 * If fValidCnrInfo is clear, the container info is requested.
 * @returns   TRUE / FALSE according to result.
 * @remark    This function must be called before using data from the CnrInfo struct.
 */
BOOL kCnrBase::validateCnrInfo()
{
    if (!fValidCnrInfo)
    {
        if ((int)WinSendMsg(hwndCnr, CM_QUERYCNRINFO, &CnrInfo, (MPARAM)sizeof(CnrInfo)) == 0)
            return FALSE;
        fValidCnrInfo = TRUE;
    }
    return TRUE;
}



/**
 * Sets container control data.
 * @returns   TRUE / FALSE according to the result.
 * @param     flChanged  Flags which specifies which part of the CnrInfo struct which is to be set.
 * @remark    This is an internal function, which of course means you have the
 *            set the value in the CnrInfo struct before calling this function.
 */
BOOL kCnrBase::setCnrInfo(ULONG flChanged)
{
    return(BOOL)WinSendMsg(hwndCnr, CM_SETCNRINFO, &CnrInfo, (MPARAM)flChanged);
}



/**
 * Determins if this container uses minirecordcore.
 * @returns   TRUE - uses minirecordcore, FALSE - uses recordcore.
 */
BOOL kCnrBase::usesMiniRecord()
{
    return WinQueryWindowULong(hwndCnr, QWL_STYLE) & CCS_MINIRECORDCORE;
}



/**
 * Default command.
 * @param     usCmd     Control id which send/posted the message.
 * @param     usSource  Source id.
 * @param     fPointer  Mouse pointer flag.
 */
VOID kCnrBase::command(USHORT usCmd, USHORT usSource, BOOL fPointer)
{
    /* unreferenced */
    usCmd = usCmd;
    usSource = usSource;
    fPointer = fPointer;
}



/**
 * Initmenu message. Enables and disables menuitems.
 * @param     usMenuId  Control id of menu.
 * @param     hwndMnu   WindowHandle of menu.
 */
VOID kCnrBase::initMenu(USHORT usMenuId, HWND hwndMnu)
{
    /* no action */

    /* unreferenced */
    usMenuId = usMenuId;
    hwndMnu = hwndMnu;
}



/**
 * Menuend message.
 * @param     usMenuId  Control id of menu.
 * @param     hwndMnu   WindowHandle of menu.
 */
VOID kCnrBase::menuEnd(USHORT usMenuId, HWND hwndMnu)
{
    /* no action */

    /* unreferenced */
    usMenuId = usMenuId;
    hwndMnu = hwndMnu;
}



/**
 * Wraps in an PM Container control.
 * @param     hwndCnr  Window handle to container control.
 */
kCnrBase::kCnrBase(HWND hwndCnr, BOOL fBorder/* = TRUE*/) throw (kError)
: hwndCnr(hwndCnr), hwndBorder(NULLHANDLE),
pfnwpOrg(NULL),
fValidCnrInfo(FALSE)
{
    TRACELOG_CONSTRUCT("start");

    /* subclass container */
    pfnwpOrg = WinSubclassWindow(hwndCnr, forwarder);
    WinSetWindowULong(hwndCnr, QWL_USER, (ULONG)this);


    /* get Container info */
    validateCnrInfo();
    fUsesMiniRecord = usesMiniRecord();

    /* make border */
    if (fBorder)
    {
        SWP swp;
        WinQueryWindowPos(hwndCnr, &swp);
        hwndBorder = WinCreateWindow(
            WinQueryWindow(hwndCnr, QW_PARENT),
            WC_STATIC,
            NULL,
            SS_FGNDFRAME | WS_VISIBLE,
            swp.x-1,
            swp.y-1,
            swp.cx+2,
            swp.cy+2,
            WinQueryWindow(hwndCnr, QW_OWNER),
            hwndCnr,
            0xFFFFFFFFUL,
            NULL,
            NULL);
    }

    TRACELOG_CONSTRUCT("end");
}


/**
 * Frees all record from the container.
 * @remark    The container records must be either a kCnrMiniRecord or kCnrRecord,
 *            according to container style.
 */
kCnrBase::~kCnrBase()
{
    TRACELOG_DESTRUCT("start");
    removeAllRecords();
    removeAllDetailFieldInfos();
    WinSubclassWindow(hwndCnr, pfnwpOrg);
    hwndCnr = NULLHANDLE;
    fValidCnrInfo = FALSE;
    TRACELOG_DESTRUCT("end");
}



/**
 * Forwards messages to msgLoop if a QWS_USER is registered.
 * When WM_INITDLG, it stores the this pointer (mp2) in QWS_USER.
 * @returns   depended on msg. See PMREF for more.
 * @param     hwnd  Handle of the(this) window receiving the message.
 * @param     msg   Message id. See PMREF for more.
 * @param     mp1   Message paramenter one. See PMREF for more.
 * @param     mp2   Message paramenter two. See PMREF for more.
 * @remark    Forwarder.
 */
static MRESULT _System forwarder(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    kCnrBase *pthis;
    PVOID     pv;

    TRACELOG_MESSAGES(hwnd, msg, mp1, mp2);
    pv = (PVOID)WinQueryWindowULong(hwnd, QWL_USER);
    if (pv != NULL)
    {
        pthis = (kCnrBase*)pv;
        return pthis->msgLoop(hwnd, msg, mp1, mp2);
    }
    else
        return WinDefDlgProc(hwnd, msg, mp1, mp2);
}



/**
 * Container (subclass) message loop.
 * @returns   depended on msg. See PMREF for more.
 * @param     hwnd  Handle of the(this) window receiving the message.
 * @param     msg   Message id. See PMREF for more.
 * @param     mp1   Message paramenter one. See PMREF for more.
 * @param     mp2   Message paramenter two. See PMREF for more.
 * @remark    Looks for WM_CHAR msgs with the context menu
 */
MRESULT kCnrBase::msgLoop(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    BOOL     fPassOn = TRUE;
    MRESULT  mr      = NULL;

    switch (msg)
    {
        case WM_CHAR:
            if (mp2 == (MPARAM)WinQuerySysValue(HWND_DESKTOP, SV_CONTEXTMENU))
            {
                WinPostMsg(hwnd, WM_CONTEXTMENU, NULL, NULL);
                fPassOn = FALSE;
                mr = (MRESULT)TRUE;
            }
            break;

        case WM_COMMAND:
            command(SHORT1FROMMP(mp1), SHORT1FROMMP(mp2), SHORT2FROMMP(mp2));
            break;

        case WM_INITMENU:
            initMenu(SHORT1FROMMP(mp1), (HWND)mp2);
            break;

        case WM_MENUEND:
            menuEnd(SHORT1FROMMP(mp1), (HWND)mp2);
            break;
    }

    if (fPassOn)
        mr = (*pfnwpOrg)(hwnd, msg, mp1, mp2);
    return mr;
}



/**
 * Allocates a (none-minirecordcore) record.
 * @returns   Pointer to allocated record, or NULL pointer on error.
 * @param     cbRecord  Total bytes to allocate.
 * @param     cRecords  Number of records to allocate. Defaults to 1.
 */
kCnrMiniRecord *kCnrBase::allocMiniRec(ULONG cbRecord, ULONG cRecords /*= 1*/)
{
    if (!fUsesMiniRecord || cbRecord < sizeof(MINIRECORDCORE) || cRecords == 0)
        return NULL;
    fValidCnrInfo = FALSE;
    return(kCnrMiniRecord*)WinSendMsg(hwndCnr, CM_ALLOCRECORD,
                                      (MPARAM)(cbRecord - sizeof(MINIRECORDCORE)),
                                      (MPARAM)cRecords);
}



/**
 * Allocates a (none-minirecordcore) record.
 * @returns   Pointer to allocated record, or NULL pointer on error.
 * @param     cbRecord  Total bytes to allocate.
 * @param     cRecords  Number of records to allocate. Defaults to 1.
 */
kCnrRecord *kCnrBase::allocRec(ULONG cbRecord, ULONG cRecords /*= 1*/)
{
    if (fUsesMiniRecord || cbRecord < sizeof(RECORDCORE) || cRecords == 0)
        return NULL;
    fValidCnrInfo = FALSE;
    return(kCnrRecord*)WinSendMsg(hwndCnr, CM_ALLOCRECORD,
                                  (MPARAM)(cbRecord - sizeof(RECORDCORE)),
                                  (MPARAM)cRecords);
}



/**
 * Allocates an array of field info structs.
 * @returns   Pointer to allocated fieldinfos, or NULL pointer on error.
 * @param     cFieldInfo  Count of structs to allocate.
 */
PFIELDINFO kCnrBase::allocDetailFieldInfo(USHORT cFieldInfo)
{
    fValidCnrInfo = FALSE;
    return(PFIELDINFO)WinSendMsg(hwndCnr, CM_ALLOCDETAILFIELDINFO, (MPARAM)cFieldInfo, NULL);
}



/**
 * Frees an array of field info structs.
 * @returns   Pointer to allocated fieldinfos, or NULL pointer on error.
 * @param     pFieldInfo  Pointer to fieldinfo structs to remove.
 * @param     cFieldInfo  Count of structs to allocate.
 */
BOOL kCnrBase::freeDetailFieldInfo(PFIELDINFO pFieldInfo, USHORT cFieldInfo)
{
    fValidCnrInfo = FALSE;
    return(BOOL)WinSendMsg(hwndCnr, CM_FREEDETAILFIELDINFO, pFieldInfo, (MPARAM)cFieldInfo);
}



/**
 * Inserts record at top.
 * @returns   TRUE / FALSE according to the result.
 * @param     pRecord   Pointer to record which is to be inserted.
 * @param     cRecords  Number of records to insert. Defaults to 1.
 * @remark    The container records must be either a kCnrMiniRecord or kCnrRecord,
 *            according to container style. They must also be allocated using either
 *            allocRec or allocMiniRec.
 */
BOOL kCnrBase::insertAtTop(kCnrMiniRecord *pRecord, ULONG cRecords /*=1*/)
{
    RECORDINSERT recordInsert =
    {
        sizeof(recordInsert),   /*  Structure size. */
        (PRECORDCORE)CMA_FIRST, /*  Record order. */
        NULL,                   /*  Pointer to record parent. */
        TRUE,                   /*  Update flag. */
        CMA_TOP,                /*  Record z-order. */
        cRecords                /*  Number of root-level structures. */
    };
    fValidCnrInfo = FALSE;
    return(BOOL)WinSendMsg(hwndCnr, CM_INSERTRECORD, pRecord, &recordInsert);
}



/**
 * Inserts record at bottom.
 * @returns   TRUE / FALSE according to the result.
 * @param     pRecord  Pointer to record which is to be inserted.
 * @param     cRecords  Number of records to insert. Defaults to 1.
 * @remark    The container records must be either a kCnrMiniRecord or kCnrRecord,
 *            according to container style. They must also be allocated using either
 *            allocRec or allocMiniRec.
 */
BOOL kCnrBase::insertAtBottom(kCnrMiniRecord *pRecord, ULONG cRecords /*=1*/)
{
    RECORDINSERT recordInsert =
    {
        sizeof(recordInsert),   /*  Structure size. */
        (PRECORDCORE)CMA_END,   /*  Record order. */
        NULL,                   /*  Pointer to record parent. */
        TRUE,                   /*  Update flag. */
        CMA_BOTTOM,             /*  Record z-order. */
        cRecords                /*  Number of root-level structures. */
    };
    fValidCnrInfo = FALSE;
    return(BOOL)WinSendMsg(hwndCnr, CM_INSERTRECORD, pRecord, &recordInsert);
}




/**
 * Inserts fieldinfo structs into the container.
 * @returns   TRUE / FALSE according to the result.
 * @param     pFieldInfo            Pointer to array of fieldinfo structs to insert.
 * @param     pFieldInfoOrder       CMA_FIRST/CMA_END or pointer to fieldinfo struct.
 * @param     fInvalidateFieldInfo  TRUE: invalidate; FALSE: not invalidate.
 * @param     cFieldInfoInsert      Count of fieldinfo items in the array (pFieldInfo).
 * @remark    The array must be allocated with allocDetailFieldInfo(...).
 */
BOOL kCnrBase::insertDetailFieldInfo(PFIELDINFO pFieldInfo, PFIELDINFO pFieldInfoOrder,
                                     BOOL fInvalidateFieldInfo, ULONG cFieldInfoInsert)
{
    FIELDINFOINSERT FieldInfoInsert =
    {
        sizeof(FIELDINFOINSERT), /* Size of structure.             */
        pFieldInfoOrder,         /* Specifies the order of the     */
        /* FieldInfo structures.          */
        fInvalidateFieldInfo,    /* Invalidate on Insert.          */
        cFieldInfoInsert         /* The number of FieldInfo        */
        /* structures to insert.          */
    };

    fValidCnrInfo = FALSE;
    return(int)WinSendMsg(hwndCnr, CM_INSERTDETAILFIELDINFO, pFieldInfo, &FieldInfoInsert) != 0;
}



/**
 * Frees a single record from the container.
 * @param     ppvRecord  Pointer to the record.
 * @remark    1) The record must be allocated using either allocRec or allocMiniRec.
 *            2) Userdata must be freed before calling this function.
 */
BOOL kCnrBase::removeRecord(void *pvRecord)
{
    if ((int)WinSendMsg(hwndCnr, CM_REMOVERECORD, &pvRecord,
                        MPFROM2SHORT(1, CMA_FREE)) == -1)
        return FALSE;

    fValidCnrInfo = FALSE;
    return TRUE;
}


/**
 * Frees cRecord number of records from the array of recordpointers passed in.
 * @param     ppvRecord  Pointer to an array of record pointers.
 * @param     cRecord    Count of records to free (accepts negative numbers too).
 * @remark    1) They must be allocated using either allocRec or allocMiniRec.
 *            2) Userdata must be freed before calling this function.
 */
BOOL kCnrBase::removeRecords(void **ppvRecord, ULONG cRecord/*= 1*/)
{
    if ((int)WinSendMsg(hwndCnr, CM_REMOVERECORD, ppvRecord,
                        MPFROM2SHORT(cRecord, CMA_FREE)) == -1)
        return FALSE;

    fValidCnrInfo = FALSE;
    return TRUE;
}



/**
 * Frees cRecord number of records from the top of the container.
 * @param     cRecord  Count of records to free (accepts negative numbers too).
 * @remark    1) They must be allocated using either allocRec or allocMiniRec.
 *            2) Userdata must be freed before calling this function.
 */
BOOL kCnrBase::removeRecordAtTop(LONG cRecord)
{
    validateCnrInfo();
    if (fUsesMiniRecord)
    {
        kCnrMiniRecord *pRecord;

        while (cRecord-- > 0 && (pRecord = queryTopRecord()) != NULL)
            if ((int)WinSendMsg(hwndCnr, CM_REMOVERECORD, &pRecord, MPFROM2SHORT(1, CMA_FREE)) == -1)
                return FALSE;
    }
    else
    {
        kCnrRecord *pRecord;

        while (cRecord-- > 0 && (pRecord = (kCnrRecord*)queryTopRecord()) != NULL)
            if ((int)WinSendMsg(hwndCnr, CM_REMOVERECORD, &pRecord, MPFROM2SHORT(1, CMA_FREE)) == -1)
                return FALSE;
    }
    fValidCnrInfo = FALSE;
    return TRUE;
}



/**
 * Frees cRecord number of records from the bottom of the container.
 * @param     cRecord  Count of records to free (accepts negative numbers too).
 * @remark    1) They must be allocated using either allocRec or allocMiniRec.
 *            2) Userdata must be freed before calling this function.
 */
BOOL kCnrBase::removeRecordAtBottom(LONG cRecord)
{
    validateCnrInfo();
    if (fUsesMiniRecord)
    {
        kCnrMiniRecord *pRecord;

        while (cRecord-- > 0 && (pRecord = queryBottomRecord()) != NULL)
            if ((int)WinSendMsg(hwndCnr, CM_REMOVERECORD, &pRecord, MPFROM2SHORT(1, CMA_FREE)) == -1)
                return FALSE;
    }
    else
    {
        kCnrRecord *pRecord;

        while (cRecord-- > 0 && (pRecord = (kCnrRecord*)queryBottomRecord()) != NULL)
            if ((int)WinSendMsg(hwndCnr, CM_REMOVERECORD, &pRecord, MPFROM2SHORT(1, CMA_FREE)) == -1)
                return FALSE;
    }
    fValidCnrInfo = FALSE;
    return TRUE;
}



/**
 * Frees all records from the container.
 * @remark    1) They must be allocated using either allocRec or allocMiniRec.
 *            2) Userdata must be freed before calling this function.
 *            TODO: not error proof.
 */
BOOL kCnrBase::removeAllRecords()
{
    validateCnrInfo();
    if (fUsesMiniRecord)
    {
        kCnrMiniRecord *pRecord;

        while ((pRecord = queryTopRecord()) != NULL)
            if ((int)WinSendMsg(hwndCnr, CM_REMOVERECORD, &pRecord, MPFROM2SHORT(1, CMA_FREE)) == -1)
                return FALSE;
    }
    else
    {
        kCnrRecord *pRecord;

        while ((pRecord = (kCnrRecord*)queryTopRecord()) != NULL)
            if ((int)WinSendMsg(hwndCnr, CM_REMOVERECORD, &pRecord, MPFROM2SHORT(1, CMA_FREE)) == -1)
                return FALSE;
    }
    fValidCnrInfo = FALSE;
    return TRUE;
}



/**
 * Removes an array of fieldinfo structs.
 * @returns   TRUE
 * @param     paFieldInfo Pointer to an array of fieldinfo structs to remove.
 * @param     cFieldInfo  Count of structs to remove.
 * @param     usFlags     Remove flags.
 */
BOOL kCnrBase::removeDetailFieldInfo(PFIELDINFO paFieldInfo[], USHORT cFieldInfo, USHORT usFlags)
{
    fValidCnrInfo = FALSE;
    return(int)WinSendMsg(hwndCnr, CM_REMOVEDETAILFIELDINFO, paFieldInfo, MPFROM2SHORT(cFieldInfo, usFlags)) != -1;
}



/**
 * Frees all fieldinfos from the container.
 * @returns   TRUE / FALSE according to the result.
 */
BOOL kCnrBase::removeAllDetailFieldInfos()
{
    PFIELDINFO pFieldInfo;
    BOOL       fRet = TRUE;

    while ((pFieldInfo = (PFIELDINFO)WinSendMsg(hwndCnr,
                                                CM_QUERYDETAILFIELDINFO,
                                                (MPARAM)CMA_FIRST,
                                                (MPARAM)CMA_FIRST))       != NULL
           && (int)pFieldInfo != -1)
    {
        if (!removeDetailFieldInfo(&pFieldInfo, 1, CMA_FREE))
        {
            fRet = FALSE;
            break;
        }
    }

    fValidCnrInfo = FALSE;

    return fRet;
}



/**
 * Invalidates a record.
 * @returns   TRUE / FALSE according to the result.
 * @param     pRecord  Pointer to the record which is to be invalidated.
 * @param     fFlags   Invalidation flags. defaults to CMA_
 */
BOOL kCnrBase::invalidateRecord(kCnrMiniRecord *pRecord, ULONG fFlags/* = CMA_TEXTCHANGED*/)
{
    return(BOOL)WinSendMsg(hwndCnr, CM_INVALIDATERECORD, &pRecord, MPFROM2SHORT(1, fFlags));
}



/**
 * Invalidates multiple records.
 * @returns   TRUE / FALSE according to the result.
 * @param     fFlags    Invalidation flags.
 * @param     cRecords  Count of records to invalidate.
 * @param     ...       'cRecords' number of Pointers to kCnrMiniRecords
 */
BOOL kCnrBase::invalidateRecords(ULONG fFlags, ULONG cRecords, ...)
{
    va_list pArg;
    BOOL fRet;
    kCnrMiniRecord** papRec = new (kCnrMiniRecord**)[cRecords];
    if (papRec == NULL)
        return FALSE;

    /* fill papRec */
    va_start(pArg, cRecords);
    for (int i = 0; i < cRecords; i++)
        papRec[i] = va_arg(pArg, kCnrMiniRecord *);
    va_end(pArg);

    /* call */
    fRet = invalidateRecords(fFlags, cRecords, papRec);

    delete papRec;

    return fRet;
}



/**
 * Invalidates multiple records.
 * @returns   TRUE / FALSE according to the result.
 * @param     fFlags    Invalidation flags.
 * @param     cRecords  Count of records to invalidate.
 * @param     papRec    Pointer to a record array of 'cRecords' records.
 */
BOOL kCnrBase::invalidateRecords(ULONG fFlags, ULONG cRecords, kCnrMiniRecord **papRec)
{
    return(BOOL)WinSendMsg(hwndCnr, CM_INVALIDATERECORD, papRec, MPFROM2SHORT(cRecords, fFlags));
}



/**
 * Sets the cDelta value of the container.
 * @returns   TRUE / FALSE according to the result.
 * @param     cDelta  New delta value.
 */
BOOL kCnrBase::setDelta(ULONG cDelta)
{
    if (!validateCnrInfo())
        return FALSE;
    if (CnrInfo.cDelta != cDelta)
    {
        CnrInfo.cDelta = cDelta;
        return setCnrInfo(CMA_DELTA);
    }

    return TRUE;
}



/**
 * Sets the flWindowAttr field of the CnrInfo struct.
 * @returns   TRUE / FALSE according to if the update was successful or not.
 * @param     flWindowAttr  New value.
 */
BOOL kCnrBase::setflWindowAttr(ULONG flWindowAttr)
{
    if (!validateCnrInfo())
        return FALSE;

    if (CnrInfo.flWindowAttr != flWindowAttr)
    {
        CnrInfo.flWindowAttr = flWindowAttr;
        return setCnrInfo(CMA_FLWINDOWATTR);
    }

    return TRUE;
}



/**
 * Sets the window title.
 * @returns   TRUE / FALSE according to if the update was successful or not.
 * @param     pszTitle  Pointer to title string.
 */
BOOL kCnrBase::setTitle(PSZ pszTitle)
{
    if (!validateCnrInfo())
        return FALSE;

    CnrInfo.pszCnrTitle = pszTitle;
    return setCnrInfo(CMA_CNRTITLE);
}



/**
 * Sets the pFieldInfoLast field in the CnrInfo struct.
 * @returns   TRUE / FALSE according to if the update was successful or not.
 * @param     pFieldInfoLast  Pointer to the last fieldinfo struct.
 */
BOOL kCnrBase::setLastFieldInfo(PFIELDINFO pFieldInfoLast)
{
    if (!validateCnrInfo())
        return FALSE;

    if (CnrInfo.pFieldInfoLast != pFieldInfoLast)
    {
        CnrInfo.pFieldInfoLast = pFieldInfoLast;
        return setCnrInfo(CMA_PFIELDINFOLAST);
    }

    return TRUE;
}



/**
 * Sets the emphasis on a record.
 * @returns   TRUE / FALSE according to the result.
 * @param     pRecord
 * @param     fChangeEmphasis     TRUE : Set attributes given in fEmphasisAttribute.
 *                                FALSE: Unset attributes given in fEmphasisAttribute.
 * @param     fEmphasisAttribute  Record emphasis attributes.
 */
BOOL kCnrBase::setRecordEmphasis(kCnrMiniRecord *pRecord, BOOL fChangeEmphasis, USHORT fEmphasisAttribute)
{
    return(BOOL)WinSendMsg(hwndCnr, CM_SETRECORDEMPHASIS,
                           pRecord,
                           MPFROM2SHORT(fChangeEmphasis, fEmphasisAttribute));
}



/**
 * Sets the position.
 * @returns   TRUE / FALSE according to the result.
 * @param     ulPos  Position (record number)
 */
BOOL kCnrBase::setPos(ULONG ulPos)
{
    return(BOOL)WinSendMsg(WinWindowFromID(hwndCnr, CID_VSCROLL), SBM_SETPOS, (MPARAM)ulPos, NULL);
}



/**
 * disables update.
 * @returns   TRUE/FALSE according to result.
 * @remark    Intended used with multiple inserts/removs.
 */
BOOL kCnrBase::disableUpdate()
{
    return WinEnableWindowUpdate(hwndCnr, FALSE);
}



/**
 * Enable update.
 * @returns   TRUE/FALSE according to result.
 * @remark    Intended used with multiple inserts/removs.
 */
BOOL kCnrBase::enableUpdate()
{
    return WinEnableWindowUpdate(hwndCnr, TRUE)
    && WinInvalidateRect(hwndCnr, NULL, TRUE)
    && WinInvalidateRect(WinWindowFromID(hwndCnr, CID_VSCROLL), NULL, TRUE);
}


/**
 * Resize the container and border frame according to the swpDelta.
 * @returns   success indicator.
 * @param     cx    New height.
 * @param     cy    New width.
 */
BOOL kCnrBase::resize(LONG cx, LONG cy)
{
    /* container */
    if (WinSetWindowPos(hwndCnr, HWND_TOP,
                        0, 0,
                        cx,
                        cy,
                        SWP_SIZE)
        )
    {
        /* border */
        if (hwndBorder != NULLHANDLE)
        {
            if (WinSetWindowPos(hwndBorder, hwndCnr,
                                0, 0,
                                cx+2,
                                cy+2,
                                SWP_SIZE)
                )
                return TRUE;
        }
        else
            return TRUE;
    }

    return FALSE;
}


/**
 * Resize the container and border frame according to the swpDelta.
 * @returns   success indicator.
 * @param     x    Left corner x-coordinate.
 * @param     y    Left corner y-coordinate.
 */
BOOL kCnrBase::setpos(LONG x, LONG y)
{
    /* container */
    if (WinSetWindowPos(hwndCnr, HWND_TOP,
                        x, y,
                        0, 0,
                        SWP_MOVE)
        )
    {
        /* border */
        if (hwndBorder != NULLHANDLE)
        {
            if (WinSetWindowPos(hwndBorder, hwndCnr,
                                x-1, y-1,
                                0, 0,
                                SWP_MOVE)
                )
                return TRUE;
        }
        else
            return TRUE;
    }

    return FALSE;
}


/**
 * Resize and mode the dialog.
 * @returns   Success indicator. TRUE / FALSE.
 * @param     x   x-coordinate for left lower corner.
 * @param     y   y-coordinate for left lower corner.
 * @param     cx  Size in x direction (pixels).
 * @param     cy  Size in y direction (pixels).
 */
BOOL kCnrBase::resizeAndMove(LONG x, LONG y, LONG cx, LONG cy)
{
    if (WinSetWindowPos(hwndCnr, NULLHANDLE, x, y, cx, cy, SWP_SIZE | SWP_MOVE))
    {
        if (hwndBorder != NULLHANDLE)
        {
            return WinSetWindowPos(hwndBorder, NULLHANDLE, x-1, y-1, cx+2, cy+2, SWP_SIZE | SWP_MOVE);
        }
        else
            return TRUE;
    }
    return FALSE;
}


/**
 * Gets the menu-source marked record in the container (if any).
 * @returns   Pointer to menu-source marked record.
 *            If no such record or error a NULL-pointer is returned.
 * @param     pStartRecord  NULL  - first menu-source record is returned.
 *                          Other - pRecord is used as a the start record for the search.
 */
kCnrMiniRecord *kCnrBase::queryMenuSourceRecord(kCnrMiniRecord *pStartRecord/* = NULL*/)
{
    PVOID pv = WinSendMsg(hwndCnr, CM_QUERYRECORDEMPHASIS,
                          pStartRecord != NULL ? (MPARAM)pStartRecord : (MPARAM)CMA_FIRST,
                          (MPARAM)CRA_SOURCE);
    return(kCnrMiniRecord*)(pv != (void*)-1 ? pv : NULL);
}



/**
 * Gets selected record(s) in the container (if any).
 * @returns   Pointer to selected record.
 *            If no such record or error a NULL-pointer is returned.
 * @param     pStartRecord  NULL  - first selected record is returned.
 *                          Other - pRecord is used as a the start record for the search.
 */
kCnrMiniRecord *kCnrBase::querySelectedRecord(kCnrMiniRecord *pStartRecord/* = NULL*/)
{
    PVOID pv = WinSendMsg(hwndCnr, CM_QUERYRECORDEMPHASIS,
                          pStartRecord == NULL ? (MPARAM)CMA_FIRST : pStartRecord,
                          (MPARAM)CRA_SELECTED);
    return(kCnrMiniRecord*)(pv != (void*)-1 ? pv : NULL);
}



/**
 * Gets in-use record(s) in the container (if any).
 * @returns   Pointer to in-use record.
 *            If no such record or error a NULL-pointer is returned.
 * @param     pStartRecord  NULL  - first in-use record is returned.
 *                          Other - pRecord is used as a the start record for the search.
 */
kCnrMiniRecord  *kCnrBase::queryInUseRecord(kCnrMiniRecord *pStartRecord/* = NULL*/)
{
    PVOID pv = WinSendMsg(hwndCnr, CM_QUERYRECORDEMPHASIS,
                          pStartRecord == NULL ? (MPARAM)CMA_FIRST : pStartRecord,
                          (MPARAM)CRA_INUSE);
    return(kCnrMiniRecord*)(pv != (void*)-1 ? pv : NULL);
}



/**
 * Gets cursored record(s) in the container (if any).
 * @returns   Pointer to cursored record.
 *            If no such record or error a NULL-pointer is returned.
 * @param     pStartRecord  NULL  - first cursored record is returned.
 *                          Other - pRecord is used as a the start record for the search.
 */
kCnrMiniRecord  *kCnrBase::queryCursoredRecord(kCnrMiniRecord *pStartRecord/* = NULL*/)
{
    PVOID pv = WinSendMsg(hwndCnr, CM_QUERYRECORDEMPHASIS,
                          pStartRecord == NULL ? (MPARAM)CMA_FIRST : pStartRecord,
                          (MPARAM)CRA_CURSORED);
    return(kCnrMiniRecord*)(pv != (void*)-1 ? pv : NULL);
}



/**
 * Gets the topmost record in the container.
 * @returns   Pointer to the topmost record in the container.
 *            If no records or error a NULL-pointer is returned.
 */
kCnrMiniRecord *kCnrBase::queryTopRecord()
{
    PVOID pv = WinSendMsg(hwndCnr, CM_QUERYRECORD, NULL, MPFROM2SHORT(CMA_FIRST, CMA_ITEMORDER));
    return(kCnrMiniRecord*)(pv != (void*)-1 ? pv : NULL);
}



/**
 * Gets the bottemmost record in the container.
 * @returns   Pointer the to bottommost record in the container.
 *            If no records or error a NULL-pointer is returned.
 */
kCnrMiniRecord *kCnrBase::queryBottomRecord()
{
    PVOID pv = WinSendMsg(hwndCnr, CM_QUERYRECORD, NULL, MPFROM2SHORT(CMA_LAST, CMA_ITEMORDER));
    return(kCnrMiniRecord*)(pv != (void*)-1 ? pv : NULL);
}



/**
 * Gets the next record from the given record (or top if NULL is given);
 * @returns   Pointer to a kCnrMiniRecord.
 * @param     pStartRecord  Pointer to the start record.(NULL - first record is returned).
 */
kCnrMiniRecord *kCnrBase::queryNextRecord(kCnrMiniRecord *pStartRecord/* = NULL*/)
{
    PVOID pv;

    if (pStartRecord == NULL)
        return queryTopRecord();
    pv = WinSendMsg(hwndCnr, CM_QUERYRECORD, pStartRecord, MPFROM2SHORT(CMA_NEXT, CMA_ITEMORDER));
    return(kCnrMiniRecord*)(pv != (void*)-1 ? pv : NULL);
}



/**
 * Gets the previous record from the given record (or top if NULL is given);
 * @returns   Pointer to a kCnrMiniRecord.
 * @param     pStartRecord  Pointer to the start record.(NULL - last record is returned).
 */
kCnrMiniRecord *kCnrBase::queryPrevRecord(kCnrMiniRecord *pStartRecord/* = NULL*/)
{
    PVOID pv;

    if (pStartRecord == NULL)
        return queryBottomRecord();
    pv = WinSendMsg(hwndCnr, CM_QUERYRECORD, pStartRecord, MPFROM2SHORT(CMA_PREV, CMA_ITEMORDER));
    return(kCnrMiniRecord*)(pv != (void*)-1 ? pv : NULL);
}



/**
 * Gets the number of records in the container.
 * @returns   number of records in the container.
 * @precond   Container is ok, hench the validateCnrInfo call will not fail.
 */
ULONG kCnrBase::getCountOfRecords()
{
    validateCnrInfo();
    return CnrInfo.cRecords;
}



/**
 * Gets the flWindowAttr in the CnrInfo struct.
 * @returns   flWindowAttr.
 * @precond   Container is ok, hench the validateCnrInfo call will not fail.
 */
ULONG kCnrBase::getflWindowAttr()
{
    validateCnrInfo();

    return CnrInfo.flWindowAttr;
}


/**
 * Gets the (detail) field info for the container.
 * @returns   Pointer to requested fieldinfo on success.
 *            NULL on error.
 * @param     flCmd         CMA_FIRST, CMA_LAST, CMA_NEXT or CMA_PREV.
 * @param     pfldInfoBase  When flCmd is CMA_NEXT or CMA_PREV, the next or
 *                          previous records is relative to this.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
PFIELDINFO kCnrBase::queryDetailFieldInfo(USHORT flCmd/* = CMA_FIRST*/, PFIELDINFO pfldInfoBase/* = NULL*/)
{
    return (PFIELDINFO)WinSendMsg(hwndCnr, CM_QUERYDETAILFIELDINFO, pfldInfoBase, (MPARAM)flCmd);
}




















/**
 * Sort callback function.
 * @returns   >  0  when pRecord1  >  pRecord2
 *            <  0  when pRecord1  <  pRecord2
 *            == 0  when pRecord1 ==  pRecord2
 * @param     pRecord1  Pointer to first record.
 * @param     pRecord2  Pointer to second record.
 */
SHORT kDetailCnr::sortCallBack(kCnrMiniRecord *pRecord1, kCnrMiniRecord *pRecord2)
{
    pRecord1 = pRecord1;
    pRecord2 = pRecord2;
    return 1;
}


/**
 * Container sort function callback and forwarder.
 * This function calls the sortCallBack function of the container class.
 * @returns   >  0  when pRecord1  >  pRecord2
 *            <  0  when pRecord1  <  pRecord2
 *            == 0  when pRecord1 ==  pRecord2
 * @param     pRecord1  Pointer to first record.
 * @param     pRecord2  Pointer to second record.
 * @param     pvThis    Pointer to kDetailCnr instance data.
 */
SHORT _System sortForwarder(PMINIRECORDCORE pRecord1, PMINIRECORDCORE pRecord2, PVOID pvThis)
{
    kDetailCnr *pThis = (kDetailCnr*)pvThis;
    if (pThis != NULL)
        return pThis->sortCallBack((kCnrMiniRecord*)pRecord1, (kCnrMiniRecord*)pRecord2);
    return 1;
}



/**
 * Contructor, creates a detail container.
 * @param     hwndCnr       Handle to container control.
 * @param     pszTitle      Pointer to title string.
 * @param     cColumns      Count of coloumns (and fieldinfo structs in paFieldInfo.
 * @param     paFieldInfo   Pointer to an array of fieldinfo structs containing the basis for
 *                          creating fieldinfo structs for the container.
 * @param     fBorder       True if we're to draw a pretty border around the container. (default)
 *                          False if no such border is to drawn.
 */
kDetailCnr::kDetailCnr(HWND hwndCnr, ULONG flWindowAttr, PSZ pszTitle, ULONG cColumns, PFIELDINFO paFieldInfo, BOOL fBorder/* = TRUE*/) throw (kError)
: kCnrBase(hwndCnr, fBorder)
{
    TRACELOG_CONSTRUCT("start");

    this->cColumns = cColumns;

    /* set detail-view */
    flWindowAttr |= CV_DETAIL;
    setflWindowAttr(flWindowAttr);

    /* set title if any */
    if (pszTitle != NULL)
    {
        flWindowAttr |= CA_CONTAINERTITLE;
        setTitle(pszTitle);
        if (!setflWindowAttr(flWindowAttr))
            throw ((kError::unused, kError::win));
    }

    /* make fieldinfo */
    PFIELDINFO pFieldInfo;
    PFIELDINFO pFICur;

    pFICur = pFieldInfo = allocDetailFieldInfo((USHORT)cColumns);
    if (pFieldInfo == NULL)
        throw (kError(kError::unused, kError::win));

    for (int i = 0; i < cColumns; i++, pFICur = pFICur->pNextFieldInfo)
    {
        pFICur->cb         = sizeof(FIELDINFO);           /* size of FIELDINFO struct       */
        pFICur->flData     = paFieldInfo[i].flData;       /* attributes of field's data     */
        pFICur->flTitle    = paFieldInfo[i].flTitle;      /* attributes of field's title    */
        pFICur->pTitleData = paFieldInfo[i].pTitleData;   /* title data (default is string) */
                                                          /* If CFT_BITMAP, must be HBITMAP */
        pFICur->offStruct  = paFieldInfo[i].offStruct;    /* offset from RECORDCORE to data */
        pFICur->cxWidth    = paFieldInfo[i].cxWidth;      /* pointer to user data           */

        /* determin if title on columns */
        if (pFICur->pTitleData != NULL)
            flWindowAttr |= CA_DETAILSVIEWTITLES;
    }

    /* set pFieldInfoLast */
    if (!setLastFieldInfo(pFICur))
    {
        freeDetailFieldInfo(pFieldInfo, (USHORT)cColumns);
        throw ((kError::unused, kError::win));
    }

    /* update flWindowAttr */
    if (flWindowAttr & CA_DETAILSVIEWTITLES)
    {
        if (!setflWindowAttr(flWindowAttr))
        {
            freeDetailFieldInfo(pFieldInfo, (USHORT)cColumns);
            throw ((kError::unused, kError::win));
        }
    }

    /* set fieldinfo */
    if (!insertDetailFieldInfo(pFieldInfo, (PFIELDINFO)CMA_FIRST, TRUE, cColumns))
    {
        freeDetailFieldInfo(pFieldInfo, (USHORT)cColumns);
        throw ((kError::unused, kError::win));
    }

    TRACELOG_CONSTRUCT("end");
}



/**
 * Destructor.
 */
kDetailCnr::~kDetailCnr()
{
    TRACELOG_DESTRUCT("start");
    /* do little - base class destructor does it all! */
    TRACELOG_DESTRUCT("end");
}


/**
 * disables sorting.
 * @returns   Success indicator.
 */
BOOL kDetailCnr::enableSorting()
{
    return (BOOL)WinSendMsg(hwndCnr, CM_SORTRECORD, (MPARAM)sortForwarder, (MPARAM)this);
}


/**
 * disables sorting.
 * @returns   Success indicator.
 */
BOOL kDetailCnr::disableSorting()
{
    return (BOOL)WinSendMsg(hwndCnr, CM_SORTRECORD, (MPARAM)sortForwarder, NULL);
}


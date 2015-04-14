/* $Id: kNotebookPageBase.cpp,v 1.1 2000/05/23 00:00:55 stknut Exp $
 *
 * kNotebookPageBase (kClassLib) - Notebook page base class.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */


/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
#define INCL_WIN
#define INCL_DOS


/*******************************************************************************
*   Internal Functions                                                         *
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

#include "kError.h"
#include "kBase.h"
#include "kDlgBase.h"
#include "kNotebookPageBase.h"
#include "kNotebookBase.h"


/**
 * Default command.
 * prevent from being dismissed by DID_OK and DID_CANCEL.
 * @param     usCmd     Control id which send/posted the message.
 * @param     usSource  Source id.
 * @param     fPointer  Mouse pointer flag.
 */
VOID  kNotebookPageBase::command(USHORT usCmd, USHORT usSource, BOOL fPointer)
{
    switch (usCmd)
    {
        case DID_OK:
        case DID_CANCEL:
            break;
    }

    /* unreferenced */
    usSource = usSource;
    fPointer = fPointer;
}


/**
 * Constructor. Creates a page in a notebook.
 * Note! You'll have to add it to the notebook afterwards.
 * @param     usResId  Dialog resource id.
 * @param     hmodRes  Handle to resource module.
 * @param     pNtbk    Pointer to the notebook these pages are to be a part of.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark
 */
kNotebookPageBase::kNotebookPageBase(ULONG ulResId, HMODULE hmodRes, kNotebookBase *pNtbk) throw (kError)
    : kDlgBase(ulResId, hmodRes, pNtbk->getHwnd(), pNtbk->getHwnd()),
    pNtbk(pNtbk)
{
    TRACELOG_CONSTRUCT("start");
    TRACELOG_CONSTRUCT("end");
}


/**
 * Destructor.
 */
kNotebookPageBase::~kNotebookPageBase()
{
    TRACELOG_DESTRUCT("start");
    pNtbk = NULL;
    TRACELOG_DESTRUCT("end");
}



/**
 * Help is requested for this page...
 */
VOID  kNotebookPageBase::ntfyHelp()
{
}


/**
 * This method is called to notify the page that is has been selected and
 * is now on top.
 */
VOID  kNotebookPageBase::ntfySelected()
{
}


/**
 * This method is called to notify the page that it is about to selected (switched to).
 * This selection is aborted if you return FALSE.
 * This stub does simply return TRUE.
 * @returns   TRUE: selecting me is ok with me!
 *            FALSE: selecting me is _not_ ok. Abort the process.
 */
BOOL  kNotebookPageBase::ntfySelectedPending()
{
    return TRUE;
}


/**
 * This method is called to notify the page that it has been deselected
 * and is not longer on top.
 */
VOID  kNotebookPageBase::ntfyDeselected()
{
}


/**
 * This method is called to notify the page that it is about to deselected (switched from).
 * This deselection is aborted if you return FALSE.
 * This stub does simply return TRUE.
 * @returns   TRUE: deselecting me is ok with me!
 *            FALSE: deselecting me is _not_ ok. Abort the process.
 */
BOOL  kNotebookPageBase::ntfyDeselectedPending()
{
    return TRUE;
}


/**
 * This method is called to notify the page that the size of the notebook has changed.
 * It is called by the Notobook class.
 * @param     cx  New width.
 * @param     cy  New height.
 */
VOID  kNotebookPageBase::ntfySized(LONG cx, LONG cy)
{
    resize(cx, cy);
}


/**
 * Notify the page that it has been inserted into the notebook.
 * Stub.
 */
VOID  kNotebookPageBase::ntfyInserted()
{
    ulPageId = pNtbk->queryPageId(this);
}


/**
 * Gets the tab text for this page.
 * This stub returns the dialog title (if template exists).
 * @returns   pszBuffer on success. NULL on error.
 * @param     pszBuffer  Pointer to string buffer.
 * @param     cchBuffer  Size of the buffer.
 */
PSZ  kNotebookPageBase::queryTabText(PSZ pszBuffer, ULONG cchBuffer)
{
    /*
     * We'll have to get the text from the dialog template.
     * (WinQueryWindowText don't work.)
     */
    if (pDlgTemplate != NULL
        && pDlgTemplate->adlgti[0].cchText != 0
        && pDlgTemplate->adlgti[0].offText != 0xFFFF
        && pDlgTemplate->adlgti[0].offText != 0)
    {
        *pszBuffer = '\0';
        strncat(pszBuffer,
                (char*)((unsigned)(pDlgTemplate) + pDlgTemplate->adlgti[0].offText),
                (size_t)cchBuffer);
        return pszBuffer;
    }
    return NULL;
}



/**
 * Gets the page style for this page.
 * @returns   Page style.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
ULONG kNotebookPageBase::queryPageStyle()
{
    return (USHORT)(ULONG)
        WinSendMsg(pNtbk->getHwnd(),
                   BKM_QUERYPAGESTYLE,
                   (MPARAM)this->ulPageId,
                   NULL);
}



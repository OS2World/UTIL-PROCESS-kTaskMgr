/* $Id: kDetailsRecord.cpp,v 1.2 2000/05/23 03:37:08 stknut Exp $
 *
 * kDetailsRecord - Detail container record.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
#define INCL_WIN
#define INCL_BASE

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
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "kBase.h"
#include "kError.h"
#include "kDlgBase.h"
#include "kNotebookBase.h"
#include "kContainer.h"

#include "kTaskMgrRecordBase.h"
#include "kDetailsRecord.h"
#include "kTaskMgr.h"

/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
const FIELDINFO kDetailsRecord::aFieldInfo[] =
{
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_SEPARATOR | CFA_LEFT | CFA_TOP,
      0,
      NULL,
      offsetof(kDetailsRecord, pszItem),
      NULL,
      NULL,
      0L
   }
   ,
   {
      sizeof(FIELDINFO),
      CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_TOP,
      0,
      NULL,
      offsetof(kDetailsRecord, pszValue),
      NULL,
      NULL,
      0L
   }
};

const unsigned  kDetailsRecord::cFieldInfo = 2;

const PCSZ      kDetailsRecord::pszDefault = "-";


/**
 * Initiates the struct. (zero fields some field)
 * @param       iIndex  Record index. (defaults to -1)
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
VOID  kDetailsRecord::init(int iIndex/*= -1*/)
{
    kTaskMgrRecordBase::init();
    pszItem = pszValue = pszDefault;
    this->iIndex = iIndex;
}


/**
 * Set the one or both of the two string pointers.
 * @param     pszItem   Pointer to the name of an "Item". (If NULL then nothing is changed.)
 * @param     pszValue  "Value" of the item. NULL allowed. (If NULL then nothing is changed.)
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark
 */
VOID  kDetailsRecord::set(PCSZ pszItem, PCSZ pszValue/*= NULL*/)
{
    if (pszItem != NULL)
        this->pszItem = pszItem;
    if (pszValue != NULL)
        this->pszValue = pszValue;
}


/**
 * Updates the value of an item.
 * @returns     TRUE if the value was updated.
 *              FALSE if no update was done.
 * @param       pszNewValue     Pointer to new value.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 * @remark
 */
BOOL    kDetailsRecord::updateValue(PCSZ pszNewValue)
{
    PCSZ    pszOld = getValue();
    if (!pszOld || strcmp(pszNewValue, pszValue))
    {
        size_t  cchNewValue = strlen(pszNewValue);
        if (!pszOld || strlen(pszOld) < cchNewValue)
        {
            pszValue = (PCSZ)realloc((void*)pszOld, cchNewValue + 1);
            if (!pszValue)
            {
                kError  err(ERROR_NOT_ENOUGH_MEMORY, kError::dos);
                pszValue = pszOld;
                err.showError(kTaskMgr::pszErrorTitle);
                return FALSE;
            }
        }

        memcpy((void*)pszValue, (void*)pszNewValue, cchNewValue + 1);
        setDirty();
        return TRUE;
    }
    return FALSE;
}

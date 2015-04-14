/* $Id: kQueueHack.c,v 1.1 2000/05/23 00:12:46 stknut Exp $
 *
 * OS/2 Queue info hacks.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */




/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
#define INCL_BASE

#define FOR_EXEHDR  1
#define WORD        USHORT
#define DWORD       ULONG


/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <os2.h>

#include <newexe.h>
#include <exe386.h>

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "kQuerySysState.h"
#include "kQueueHack.h"
#include "kOS2Info.h"

/*******************************************************************************
*   Structures and Typedefs                                                    *
*******************************************************************************/

/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
BOOL                    fQueInited = FALSE;
static PQUESTATSEG      pQueStatSeg = NULL;
static PMYQUEENTRY      pQueInfoList = NULL;
static PMYQUEOPENENTRY  pQueOpenList = NULL;
static PVOID            pvQueSeg1 = NULL;
static PVOID            pvQueSeg2 = NULL;

/*******************************************************************************
*   External Functions                                                         *
*******************************************************************************/
PVOID   _Optlink    DosSelToFlat(ULONG ul1616Addr);


/*
 * Initiates the global variabels in this module.
 * fQueInited and pQueStatSeg will be set upon return.
 * @returns Success indicator.
 */
BOOL        QueInit(VOID)
{
    qsLrec_t *  pDosCall1 = NULL;
    int         i = 0;

    QSUpdateStateData();
    fQueInited = FALSE;

    pDosCall1 = QSGetDosCall1Data();
    if (pDosCall1 && pDosCall1->pObjInfo)
    {
        BOOL fStatSeg = FALSE;
        BOOL fDataSegs = FALSE;

        /*
         * Find the statseg.
         *  For FP13 this is located right after \SEM\IPCQUEUE.SEM (aligned on
         *  4 byte boundrary), in the first dataobject (16-bit, shared).
         *  So we'll loop thru the segements of DOSCALL1 and search for this
         *  string.
         */
        for (i = 0; i < pDosCall1->ctObj; i++)
        {
            /*
             * Search for first segment.
             */
            if (!fStatSeg &&
                (pDosCall1->pObjInfo[i].oflags & (OBJWRITE | OBJALIAS16 | OBJSHARED))
                == (OBJWRITE | OBJALIAS16 | OBJSHARED))
            {
                static CHAR szIPCQueue[] = "\\SEM\\IPCQUEUE.SEM";
                PCHAR   pach = (PCHAR)pDosCall1->pObjInfo[i].oaddr;
                PCHAR   pachEnd = pach + pDosCall1->pObjInfo[i].osize
                                  - sizeof(QUESTATSEG)
                                  - sizeof(szIPCQueue);

                while (pach < pachEnd && strcmp(pach, szIPCQueue) != 0)
                    pach++;

                /* found ? */
                if (pach < pachEnd)
                {
                    #if 1
                    printf("found %s! at 0x%08x\n", szIPCQueue, pach);
                    #endif
                    if (!fOS2InfoInited && !OS2InfoInit())
                        return FALSE;

                    /*
                     * We have found the IPCQUEUE semaphore name.
                     * By analysing the
                     */
                    if (ulBuildLevel >= 14040)     /* Warp 4 FP13+ and Aurora FP1 (FIXME! What about Aurora GA?) */
                        pQueStatSeg = (PVOID)((ULONG)pach + 0x14);
                    else if (ulBuildLevel >= 9032) /* Warp 4 FP8 - Warp 4 FP12 */
                        pQueStatSeg = (PVOID)((ULONG)pach + 0x14c);
                    else if (ulBuildLevel <= 9031 && ulBuildLevel > 8000) /* Warp 3 to Warp 4 FP7 */
                        pQueStatSeg = (PVOID)((ULONG)pach + 0x6a);
                    else
                        return FALSE;

                    /* Sanity checks:
                     *  fpFirstQue: DosSelToFlat successfully return non-NULL pointer.
                     *            (ie. There have to exists a queue, which isn't an
                     *            unreasonable requirement at this time.)
                     *  pv:       NULL or DosSelToFlat converted it successfully.
                     *  ++ later
                     */
                    if (DosSelToFlat(pQueStatSeg->fpFirstQue) != NULL
                        && (pQueStatSeg->fpFirstOpen == 0UL ||
                            DosSelToFlat(pQueStatSeg->fpFirstOpen) != NULL)
                        )
                    {
                        fStatSeg = TRUE;
                    }
                    #if 1
                    else
                        printf(" !incorrect address! \n");
                    #endif
                }
            }

            /*
             * Find the two queue data segments.
             * They are characterized by being 16-bit, shared and read write segments
             * with a size of 4096 bytes (initially).
             */
            if (!fDataSegs
                && (i + 1) < pDosCall1->ctObj
                && (pDosCall1->pObjInfo[i].oflags & (OBJWRITE | OBJALIAS16 | OBJSHARED))
                    == (OBJWRITE | OBJALIAS16 | OBJSHARED)
                && pDosCall1->pObjInfo[i].osize == 0x1000
                && pDosCall1->pObjInfo[i+1].osize == 0x1000
                )
            {
                pvQueSeg1 = (PVOID)pDosCall1->pObjInfo[i].oaddr;
                pvQueSeg2 = (PVOID)pDosCall1->pObjInfo[i + 1].oaddr;
                fDataSegs = TRUE;
            }
        }

        /*
         * Success full if all three segments are found.
         */
        fQueInited = fDataSegs && fStatSeg;
    }


    return fQueInited;
}


/*
 * Copies the Queue data and Queue Open data to a local copy which we
 * could use without much risk from being swapped out and restared with
 * invalid pointers.
 *
 * Updates pQueInfoList. (Entirely freed and rebuild.)
 */
VOID QueCopyData(VOID)
{
    PQUEENTRY       pQueEntry = NULL;
    PQUEOPENENTRY   pOpenEntry = NULL;
    PMYQUEOPENENTRY pMyOpenEntry = NULL;
    PMYQUEENTRY     pMyEntry = NULL;

    /* free old data */
    while (pQueInfoList != NULL)
    {
        pMyEntry = pQueInfoList;
        pQueInfoList = pQueInfoList->pNext;
        free(pMyEntry);
    }
    pMyEntry = NULL;
    while (pQueOpenList != NULL)
    {
        pMyOpenEntry = pQueOpenList;
        pQueOpenList = pQueOpenList->pNext;
        free(pMyOpenEntry);
    }
    pMyOpenEntry = NULL;

    /*
     * Loop thru the list of Queues.
     *
     * THIS IS SOMEWHAT DANGEROUS SINCE THE WE COULD BE SWAPPED OUT AT ANY TIME!
     */
    pQueEntry = DosSelToFlat(pQueStatSeg->fpFirstQue);
    while (pQueEntry != NULL)
    {
        unsigned cb = sizeof(MYQUEENTRY) + strlen(pQueEntry->szQueName);

        /*
         * allocate new info entry.
         */
        if (pMyEntry)
            pMyEntry = pMyEntry->pNext = malloc(cb);
        else
            pQueInfoList = pMyEntry = malloc(cb);
        if (pMyEntry == NULL) break;
        pMyEntry->pNext = NULL;

        /*
         * Copy queue data
         */
        memcpy(&pMyEntry->que, pQueEntry, cb - 4);

        /* next */
        pQueEntry = DosSelToFlat(pQueEntry->fpNext);
    }

    /*
     * Loop thru the list of Open entries.
     *
     * THIS IS SOMEWHAT DANGEROUS SINCE THE WE COULD BE SWAPPED OUT AT ANY TIME!
     */
    pOpenEntry = DosSelToFlat(pQueStatSeg->fpFirstOpen);
    while (pOpenEntry != NULL)
    {
        /*
         * allocate new info entry.
         */
        if (pMyOpenEntry)
            pMyOpenEntry = pMyOpenEntry->pNext = malloc(sizeof(QUEOPENENTRY));
        else
            pQueOpenList = pMyOpenEntry = malloc(sizeof(QUEOPENENTRY));
        if (pMyOpenEntry == NULL) break;

        /*
         * Copy open data
         */
        memcpy(pMyOpenEntry, pOpenEntry, sizeof(QUEOPENENTRY));
        pMyOpenEntry->pNext = NULL;

        /* next */
        pOpenEntry = DosSelToFlat(pOpenEntry->fpNext);
    }
}


/**
 * Returns pointer to the first queue data entry.
 * @returns Pointer to the first queue data entry.
 *          NULL on error or no data.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
PMYQUEENTRY QueGetFirstQueueData(VOID)
{
    if (fQueInited)
        return pQueInfoList;
    return NULL;
}


/**
 * Returns pointer to the first queue open data entry.
 * @returns Pointer to the first queue open data entry.
 *          NULL on error or no data.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
PMYQUEOPENENTRY QueGetFirstOpenData(VOID)
{
    if (fQueInited)
        return pQueOpenList;
    return NULL;
}


/**
 * This parameter queries the amount of dynamic memory used by the
 * queue system. The total amount of dyniamic memory is 128KB. (So,
 * to find the amount of available memory simply subtract the
 * amount of used from 128KB.)
 * @returns     The amount of used memory in bytes.
 *              0xffffffff if not inititated.
 *              0xfffffffe if DosQueryMem seg 1 failes.
 *              0xfffffffd if DosQueryMem seg 2 failes.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
ULONG  QueQueryUsedMemory(VOID)
{
    if (fQueInited)
    {
        int     off = 0;                /* Offset of the free block. */
        ULONG   cbSeg1 = ~0UL,          /* Size of segment 1. */
                cbSeg2 = ~0UL,          /* Size of segment 2. */
                fl = ~0UL,              /* flags - ignored. */
                cb = 0;                 /* returned size. */
        APIRET  rc = 0;

        /*
         * The first segment is expected to be in use while the second might be unused.
         * We'll first determin the size of the segments by using DosQueryMem.
         */
        fl = ~0UL;
        rc = DosQueryMem(pvQueSeg1, &cbSeg1, &fl);
        if (rc != NO_ERROR || (fl & PAG_READ) != PAG_READ)
            return ~1UL;
        fl = ~0UL;
        rc = DosQueryMem(pvQueSeg2, &cbSeg2, &fl);
        if (rc != NO_ERROR || (fl & PAG_READ) != PAG_READ)
            return ~2UL;

        /*
         * The size of free memory is found at offset 0x0002 and is a dword.
         * (This accoring to "The Design of OS/2" and my own digging.)
         *
         * To determin the used size of the first segment we'll subtract the
         * amount of free memory from the size of the segment.
         *
         * To determin the used size of the second segment we'll first have to
         * see if it used at all. It is unused if it is 4KB, 8 first bytes are
         * zero and that the last 16 bytes are zero.
         */
        cb = 0;
        off = *(PUSHORT)(pvQueSeg1);
        while (off > 0 && off < cbSeg1)
        {
            cb += *(PUSHORT)((int)pvQueSeg1 + off + 2);
            off = *(PUSHORT)((int)pvQueSeg1 + off);
        }
        if (cbSeg2 != 0x1000
            || ((PULONG)pvQueSeg2)[0] != 0
            || ((PULONG)pvQueSeg2)[1] != 0
            || ((PULONG)pvQueSeg2)[0x03fc] != 0
            || ((PULONG)pvQueSeg2)[0x03fd] != 0
            || ((PULONG)pvQueSeg2)[0x03fe] != 0
            || ((PULONG)pvQueSeg2)[0x03ff] != 0
            )
        {
            off = *(PUSHORT)(pvQueSeg2);
            while (off > 0 && off < cbSeg2)
            {
                cb += *(PUSHORT)((int)pvQueSeg2 + off + 2);
                off = *(PUSHORT)((int)pvQueSeg2 + off);
            }
            cb = (cbSeg1 + cbSeg2) - cb;
        }
        else
            cb = cbSeg1 - cb;
        return cb;
    }
    return ~0UL;
}


/**
 * Queries the amount of memory used by a single queue.
 * This memory requiremet count memory for the queue struct (aligned to
 * 4 bytes), open entries and elements. (The (guessed) element size is
 * 24 bytes.)
 * @returns     Amount of memory used by the given queue.
 *              0xffffffff on if not initiated.
 * @param       pQueue  Pointer to valid queue entry.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
ULONG           QueQueryUsedMemoryQueue(PMYQUEENTRY pQueue)
{
    if (fQueInited)
    {
        PMYQUEOPENENTRY     pOpen = pQueOpenList;
        ULONG               cb = sizeof(pQueue->que) + strlen(pQueue->que.szQueName);
        cb = (cb + 3) & ~3;
        cb += pQueue->que.cEntries * 24;

        /* Check the open handle list to count memory used for open structs. */
        while (pOpen != NULL)
        {
            if (pOpen->hQueue == pQueue->que.hQueue)
                cb += sizeof(QUEOPENENTRY);
            pOpen = pOpen->pNext;
        }

        return cb;
    }
    return ~0UL;
}


/**
 * Dumps queue info - debugging helper.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
void QueDumpQueues(void)
{

    /*
     * Loop thru the list of Queues.
     *
     * THIS IS SOMEWHAT DANGEROUS SINCE THE WE COULD BE SWAPPED OUT AT ANY TIME!
     */
    PQUEENTRY pQueEntry = DosSelToFlat(pQueStatSeg->fpFirstQue);
    while (pQueEntry != NULL)
    {
        printf("0x%08x  Name: %s\n",
               pQueEntry, pQueEntry->szQueName);

        printf("    chFlags: 0x%02x  cEntries: %d usPriority: 0x%04x\n",
               pQueEntry->chFlags, pQueEntry->cEntries, pQueEntry->usPriority);


        /* next */
        pQueEntry = DosSelToFlat(pQueEntry->fpNext);
    }
}


#if 0
int main(void)
{
    QSUpdateStateData();
    QueInit();
    printf("fQueInited=%d\n", fQueInited);
    if (fQueInited)
    {
        QueDumpQueues();
    }
    return 0;
}
#endif







/* $Id: kSFNDetailsContainer.cpp,v 1.3 2000/05/23 03:37:08 stknut Exp $
 *
 * kSFNDetailsContainer (kTaskMgr) - Container that lists information on a
 *    given SFN.
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

/* sf_flag definitions (from SG24-4640-00 INF) */
#define SF_ISNET   0x8000   /* True if SFT is for remote */
#define SF_PIPE    0x2000   /* Anonymous Pipe */
#define SF_FCB     0x1000   /* True if SFT is for an FCB */
#define SF_NMPIPE  0x0800   /* True if name pipe */
#define SF_INUSE   0x0400   /* True if sf entry is in use by some thread, that is, busy */
#define SF_BLOCKED 0x0200   /* True if some thread is blocked waiting to use the sf entry */
#define SF_NOJFN   0x0100   /* True if no handle alloc'ed for SFT */

/* sf_flags2 flag definitions (from SG24-4640-00 INF) */
#define SF_FORMAT_MOUNT         0x8000  /* True if a format mount was done, and still in effect */
#define SF_BEGINFORMAT_FAILED   0x4000  /* True if a beginformat ioctl failed  */
#define SSF2_LDRBINARYSEM       0x2000  /* 'ON' if SFT owned by some thread  */
#define SF_SRVRDR               0x1000  /* serving pipe redirection in effect */
#define SFF2_LOCKED_DRIVE       0x0800  /* A LOCK was issued on this direct access handle to lock the drive.  */
#define SFF2_SPOOLED            0x0400  /* File is spooled */
#define SFF2_DATAWRITTEN        0x0200  /* Data written to file  */
#define SFF2_Consistency        0x0180  /* consistency bits */
#define SFF2_CANCELJOB          0x0040  /* spool job has been canceled ;whs  */
#define SFF2_NONSPOOLED         0x0020  /* File is nonspooled; going to printer */
#define SFF2_STPTHINFDN         0x0010  /* SetPathInfo done, don't set archive */
#define sff2_RA_ON              0x0008  /* Readahead started */
#define sff2_UNC                0x0004  /* UNC object */
#define sff2_isfree             0x0002  /* this SFT is on free list (unused) */
#define sff2_RA_BIG             0x0001  /* Big Readahead */


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
#include "kOS2Info.h"
#include "kTaskMgrRecordBase.h"
#include "kTaskMgrContainerBase.h"
#include "kDetailsBase.h"

#include "kDetailsRecord.h"
#include "kSFNDetails.h"
#include "kSFNDetailsContainer.h"
#include "kTaskMgr.h"
#include "kTaskMgr_defs.h"



/**
 * Constructor.
 * @param     hwndDlg   Handle to dialog window.
 * @param     ulCnrId   ID of the container dialog item in hwndDlg.
 * @param     usSFN     SFN we're to show detail info on.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
kSFNDetailsContainer::kSFNDetailsContainer(HWND hwndDlg, ULONG ulCnrId, USHORT usSFN) throw(kError)
    : kTaskMgrContainerBase(hwndDlg, ulCnrId,
                            NULL,
                            kDetailsRecord::cFieldInfo,
                            (PFIELDINFO)&kDetailsRecord::aFieldInfo[0],
                            IDM_CNR_PROCESS_DETAILS_ALL,
                            IDM_CNR_PROCESS_DETAILS),
    usSFN(usSFN)
{
    TRACELOG_CONSTRUCT("start");
    qsFrec_t *      pFSData;
    int             iSFN;
    kDetailsRecord *pCurCnrRec;
    int             cRecords = 9;
    char            szBuffer[512];

    /*
     * Get File information from the QS subsystem.
     */
    pFSData = QSGetSfnData(usSFN);
    if (pFSData == NULL)
        throw(kError(ERROR_INVALID_PARAMETER, kError::dos));
    for (iSFN = 0; iSFN < pFSData->ctSft; iSFN++)
        if (pFSData->pSft[iSFN].sfn == usSFN)
            break;
    if (iSFN >= pFSData->ctSft)
        throw(kError(ERROR_INVALID_PARAMETER, kError::dos));

    /*
     * Allocate records.
     */
    pCurCnrRec = pCnrRec = (kDetailsRecord *)allocMiniRec(sizeof(kDetailsRecord), cRecords);
    if (pCurCnrRec == NULL)
        throw(kError(kError::unused, kError::win));

    /*
     * Insert records into the container.
     */
    /* 0 - static */
    pCurCnrRec->init(0);
    pCurCnrRec->set("Name", strdup((PCSZ)(pFSData+1)));
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 1 - static */
    pCurCnrRec->init(1);
    sprintf(szBuffer, "0x%04x", usSFN);
    pCurCnrRec->set("SFN", strdup(szBuffer));
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 2 - static */
    pCurCnrRec->init(2);
    sprintf(szBuffer, "0x%04x", pFSData->pSft[iSFN].hVPB);
    pCurCnrRec->set("hVPB", strdup(szBuffer));
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 3 */
    pCurCnrRec->init(3);
    pCurCnrRec->set("RefCnt");
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 4 */
    pCurCnrRec->init(4);
    pCurCnrRec->set("Size");
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 5 */
    pCurCnrRec->init(5);
    pCurCnrRec->set("Attributes");
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 6 */
    pCurCnrRec->init(6);
    pCurCnrRec->set("Open Mode (1&2)");
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 7 */
    pCurCnrRec->init(7);
    pCurCnrRec->set("Flags1");
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 8 */
    pCurCnrRec->init(8);
    pCurCnrRec->set("Flags2");
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();


    /*
     * Insert the records.
     */
    if (!insertAtBottom(pCnrRec, cRecords))
        throw(kError(kError::unused, kError::win));
    update();

    TRACELOG_CONSTRUCT("end");
}


/**
 * Destructor.
 */
kSFNDetailsContainer::~kSFNDetailsContainer()
{
    TRACELOG_DESTRUCT("start");
    kDetailsRecord *pCurCnrRec = pCnrRec;

    /*
     * Free stuff in records.
     */
    while (pCurCnrRec != NULL)
    {
        PCSZ pszValue;
        pszValue = pCurCnrRec->getValue();
        if (pszValue)
            free((void*)pszValue);
        pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();
    }
    this->removeAllRecords();

    TRACELOG_DESTRUCT("end");
}


/**
 * Updates the contents of the container.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
VOID  kSFNDetailsContainer::update()
{
    TRACELOG_UPDATE("start");
    qsFrec_t *      pFSData;
    int             iSFN;
    kDetailsRecord *pCurCnrRec = pCnrRec;
    char            szBuffer[1024];
    ULONG           ul;

    /*
     * Get File information from the QS subsystem.
     */
    pFSData = QSGetSfnData(usSFN);
    if (pFSData == NULL)
        throw(kError(ERROR_INVALID_PARAMETER, kError::dos));
    for (iSFN = 0; iSFN < pFSData->ctSft; iSFN++)
        if (pFSData->pSft[iSFN].sfn == usSFN)
            break;
    if (iSFN >= pFSData->ctSft)
        throw(kError(ERROR_INVALID_PARAMETER, kError::dos));


    /*
     * Update the records
     */
    /* 0 - static */
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 1 - static */
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 2 - static */
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 3 - Refcnt */
    sprintf(szBuffer, "0x%04x (%d)", pFSData->pSft[iSFN].refcnt, pFSData->pSft[iSFN].refcnt);
    pCurCnrRec->updateValue(szBuffer);
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 4 - Size */
    sprintf(szBuffer, "%d bytes", pFSData->pSft[iSFN].size);
    pCurCnrRec->updateValue(szBuffer);
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 5 - Attributes */
    szBuffer[0] = '\0';
    if (pFSData->pSft[iSFN].attr & FILE_READONLY )          strcat(szBuffer, "ReadOnly ");
    if (pFSData->pSft[iSFN].attr & FILE_HIDDEN   )          strcat(szBuffer, "Hidden ");
    if (pFSData->pSft[iSFN].attr & FILE_SYSTEM   )          strcat(szBuffer, "System ");
    if (pFSData->pSft[iSFN].attr & FILE_ARCHIVED )          strcat(szBuffer, "Archived ");
    if (pFSData->pSft[iSFN].attr & FILE_DIRECTORY)          strcat(szBuffer, "Directory ");
    sprintf(szBuffer + strlen(szBuffer), "(0x%04x)", pFSData->pSft[iSFN].attr);
    pCurCnrRec->updateValue(szBuffer);
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 6 - Open Mode */
    szBuffer[0] = '\0';
    ul = pFSData->pSft[iSFN].mode + (pFSData->pSft[iSFN].mode2 >> 16);
    if ((ul & 0x7) == OPEN_ACCESS_READONLY)                 strcat(szBuffer, "ReadOnly ");
    else if ((ul & 0x7) == OPEN_ACCESS_WRITEONLY)           strcat(szBuffer, "WriteOnly ");
    else if ((ul & 0x7) == OPEN_ACCESS_READWRITE)           strcat(szBuffer, "ReadWrite ");
    if ((ul & 0x70) == OPEN_SHARE_DENYREADWRITE)            strcat(szBuffer, "DenyReadWrite ");
    else if ((ul & 0x70) == OPEN_SHARE_DENYWRITE)           strcat(szBuffer, "DenyWrite ");
    else if ((ul & 0x70) == OPEN_SHARE_DENYREAD)            strcat(szBuffer, "DenyRead ");
    else if ((ul & 0x70) == OPEN_SHARE_DENYNONE)            strcat(szBuffer, "DenyNone ");
    if (ul & OPEN_FLAGS_NOINHERIT)                          strcat(szBuffer, "NoInherit ");
    if ((ul & 0x0700) == OPEN_FLAGS_NO_LOCALITY)            strcat(szBuffer, "NoLocality ");
    if ((ul & 0x0700) == OPEN_FLAGS_SEQUENTIAL)             strcat(szBuffer, "Squential ");
    if ((ul & 0x0700) == OPEN_FLAGS_RANDOM)                 strcat(szBuffer, "Random ");
    if ((ul & 0x0700) == OPEN_FLAGS_RANDOMSEQUENTIAL)       strcat(szBuffer, "RandomSequential ");
    if (ul & OPEN_FLAGS_NO_CACHE)                           strcat(szBuffer, "NoCache ");
    if (ul & OPEN_FLAGS_FAIL_ON_ERROR)                      strcat(szBuffer, "FailOnError ");
    if (ul & OPEN_FLAGS_WRITE_THROUGH)                      strcat(szBuffer, "WriteThrough ");
    if (ul & OPEN_FLAGS_DASD)                               strcat(szBuffer, "DASD ");
    if (ul & OPEN_FLAGS_NONSPOOLED)                         strcat(szBuffer, "NonSpooled ");
    if (ul & OPEN_SHARE_DENYLEGACY)                         strcat(szBuffer, "DenyLegacy(2GB) ");
    if (ul & OPEN_FLAGS_PROTECTED_HANDLE)                   strcat(szBuffer, "ProtectedHandle ");
    sprintf(szBuffer + strlen(szBuffer), "(0x%08x)", ul);
    pCurCnrRec->updateValue(szBuffer);
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 7 - sf_flags 1 */
    ul = pFSData->pSft[iSFN].flags;
    sprintf(szBuffer, "0x%04x\r\n", ul);
    if (ul & SF_ISNET)          strcat(szBuffer, "SFT is for Remote (SF_ISNET)\r\n");
    if (ul & 0x4000)            strcat(szBuffer, "Bit 14 is set - should be unused...\r\n");
    if (ul & SF_PIPE)           strcat(szBuffer, "Pipe (SF_PIPE)\r\n");
    if (ul & SF_FCB)            strcat(szBuffer, "SFT is for an FCB (SF_FCB)\r\n");
    if (ul & SF_NMPIPE)         strcat(szBuffer, "Named pipe (SF_NMPIPE)\r\n");
    if (ul & SF_INUSE)          strcat(szBuffer, "SFT is use by some thread, ie. busy (SF_INUSE)\r\n");
    if (ul & SF_BLOCKED)        strcat(szBuffer, "Some thread is blocked waiting to use the SFT (SF_BLOCKED)\r\n");
    if (ul & SF_NOJFN)          strcat(szBuffer, "No handle alloc'ed for this SFT (SF_NOJFN)\r\n");
    if ((ul & SF_ISNET) || !(ul & 0x0080))
    {   /* local or remote file */
        if (ul & 0x0040)        strcat(szBuffer, "File is dirty\r\n");
        else                    strcat(szBuffer, "File is clean\r\n");
        sprintf(szBuffer + strlen(szBuffer), "%c (%d)\r\n", (ul & 0x3f) + 'A', ul & 0x3f);
    }
    else if (ul & 0x0080)
    {   /* local device */
        sprintf(szBuffer + strlen(szBuffer), "Local Device (devid=0x%02x)\r\n", ul & 0x2f);
        if (ul & 0x0040)        strcat(szBuffer, "  End-Of-File (input)\r\n");
        if (ul & 0x0020)        strcat(szBuffer, "  Raw mode.\r\n");
        if (ul & 0x0010)        strcat(szBuffer, "  Special device.\r\n");
        if (ul & 0x0008)        strcat(szBuffer, "  Clock device.\r\n");
        if (ul & 0x0004)        strcat(szBuffer, "  Null device.\r\n");
        if (ul & 0x0002)        strcat(szBuffer, "  Console output device.\r\n");
        if (ul & 0x0001)        strcat(szBuffer, "  Console input device.\r\n");
    }
    pCurCnrRec->updateValue(szBuffer);
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /* 8 - sf_flags 2 */
    ul = pFSData->pSft[iSFN].flags2;
    sprintf(szBuffer, "0x%04x\r\n", ul);
    if (ul & SF_FORMAT_MOUNT      )     strcat(szBuffer, "Format mount done and still in effect (SF_FORMAT_MOUNT)\r\n");
    if (ul & SF_BEGINFORMAT_FAILED)     strcat(szBuffer, "A beginformat ioctl failed (SF_BEGINFORMAT_FAILED)\r\n");
    if (ul & SSF2_LDRBINARYSEM    )     strcat(szBuffer, "SFT owned by some thread (SSF2_LDRBINARYSEM)\r\n");
    if (ul & SF_SRVRDR            )     strcat(szBuffer, "Serving pipe redirection in effect (SF_SRVRDR)\r\n");
    if (ul & SFF2_LOCKED_DRIVE    )     strcat(szBuffer, "Drive (DASD) is locked (SFF2_LOCKED_DRIVE)\r\n");
    if (ul & SFF2_SPOOLED         )     strcat(szBuffer, "File is spooled (SFF2_SPOOLED)\r\n");
    if (ul & SFF2_DATAWRITTEN     )     strcat(szBuffer, "Data written to file (SFF2_DATAWRITTEN)\r\n");
    if (ul & SFF2_Consistency     )
        sprintf(szBuffer + strlen(szBuffer), "Consistency bits: 0x%04x\r\n", ul & SFF2_Consistency);
    if (ul & SFF2_CANCELJOB       )     strcat(szBuffer, "Spool job has been canceled (SFF2_CANCELJOB)\r\n");
    if (ul & SFF2_NONSPOOLED      )     strcat(szBuffer, "File is nonspooled; going to printer (SFF2_NONSPOOLED)\r\n");
    if (ul & SFF2_STPTHINFDN      )     strcat(szBuffer, "SetPathInfo is done, don't set archive (SFF2_STPTHINFDN)\r\n");
    if (ul & sff2_RA_ON           )     strcat(szBuffer, "Readahead started (sff2_RA_ON)\r\n");
    if (ul & sff2_UNC             )     strcat(szBuffer, "UNC object (sff2_UNC)\r\n");
    if (ul & sff2_isfree          )     strcat(szBuffer, "unused (on the free list) (sff2_isfree)\r\n");
    if (ul & sff2_RA_BIG          )     strcat(szBuffer, "Big Readahead (sff2_RA_BIG)\r\n");
    pCurCnrRec->updateValue(szBuffer);
    pCurCnrRec = (kDetailsRecord *)pCurCnrRec->getNext();

    /*
     * Invalidate all the dirty records.
     */
    pCnrRec->invalidateAllDirtyRecords(this);
    TRACELOG_UPDATE("end");
}


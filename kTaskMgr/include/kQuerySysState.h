/* $Id: kQuerySysState.h,v 1.2 2000/06/19 20:08:42 stknut Exp $
 *
 * Textmode prototype made for testing purposes of the OS/2 taskmgr.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 * (GPL licensed)
 *
 */

#if !defined(_kQuerySysState_h_)

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(QS_PROCESS)
/*******************************************************************************
*   From the OS/2 4.5 toolkit.                                                 *
*******************************************************************************/

/* defines and structures for DosQuerySysState */
#pragma pack(1)
/* record types */
#define QS_PROCESS      0x0001
#define QS_SEMAPHORE    0x0002
#define QS_MTE          0x0004
#define QS_FILESYS      0x0008
#define QS_SHMEMORY     0x0010
#define QS_DISK         0x0020
#define QS_HWCONFIG     0x0040
#define QS_NAMEDPIPE    0x0080
#define QS_THREAD       0x0100
#define QS_MODVER       0x0200

/* valid EntityList bit settings */
#define QS_SUPPORTED    (QS_PROCESS|QS_SEMAPHORE|QS_MTE|QS_FILESYS|QS_SHMEMORY|QS_MODVER)

/* All structures must be padded to dword boundaries if necessary  */
/* The semicolon that is needed to terminate the structure field   */
/* must be added in the structure definition itself, because H2INC */
/* ignores it in a #define statement. */
#define PADSHORT        USHORT        pad_sh
#define PADCHAR         UCHAR         pad_ch

#define QS_END          0L       /* last FILESYS record */

/* Global Record structure
 * Holds all global system information. Placed first in user buffer
 */
typedef struct qsGrec_s {  /* qsGrec */
        ULONG         cThrds;
        ULONG         c32SSem;
        ULONG         cMFTNodes;
} qsGrec_t;

/* Thread Record structure
 *      Holds all per thread information.
 */
typedef struct qsTrec_s {  /* qsTrec */
        ULONG         RecType;        /* Record Type */
        USHORT        tid;            /* thread ID */
        USHORT        slot;           /* "unique" thread slot number */
        ULONG         sleepid;        /* sleep id thread is sleeping on */
        ULONG         priority;       /* thread priority */
        ULONG         systime;        /* thread system time */
        ULONG         usertime;       /* thread user time */
        UCHAR         state;          /* thread state */
        PADCHAR;
        PADSHORT;
} qsTrec_t;

/* Process Record structure
 *      Holds all per process information.
 *      ________________________________
 *      |       RecType                 |
 *      |-------------------------------|
 *      |       pThrdRec                |----|
 *      |-------------------------------|    |
 *      |       pid                     |    |
 *      |-------------------------------|    |
 *      |       ppid                    |    |
 *      |-------------------------------|    |
 *      |       type                    |    |
 *      |-------------------------------|    |
 *      |       stat                    |    |
 *      |-------------------------------|    |
 *      |       sgid                    |    |
 *      |-------------------------------|    |
 *      |       hMte                    |    |
 *      |-------------------------------|    |
 *      |       cTCB                    |    |
 *      |-------------------------------|    |
 *      |       c32PSem                 |    |
 *      |-------------------------------|    |
 *      |       p32SemRec               |----|---|
 *      |-------------------------------|    |   |
 *      |       c16Sem                  |    |   |
 *      |-------------------------------|    |   |
 *      |       cLib                    |    |   |
 *      |-------------------------------|    |   |
 *      |       cShrMem                 |    |   |
 *      |-------------------------------|    |   |
 *      |       cFS                     |    |   |
 *      |-------------------------------|    |   |
 *      |       p16SemRec               |----|---|----|
 *      |-------------------------------|    |   |    |
 *      |       pLibRec                 |----|---|----|------|
 *      |-------------------------------|    |   |    |      |
 *      |       pShrMemRec              |----|---|----|------|----|
 *      |-------------------------------|    |   |    |      |    |
 *      |       pFSRec                  |----|---|----|------|----|-----|
 *      |-------------------------------|    |   |    |      |    |     |
 *      |       32SemPPRUN[0]           |<---|---|    |      |    |     |
 *      |          .                    |    |        |      |    |     |
 *      |          .                    |    |        |      |    |     |
 *      |          .                    |    |        |      |    |     |
 *      |       32SemPPRUN[c32PSem-1]   |    |        |      |    |     |
 *      |-------------------------------|    |        |      |    |     |
 *      |       16SemIndx[0]            |<---|--------|      |    |     |
 *      |          .                    |    |               |    |     |
 *      |          .                    |    |               |    |     |
 *      |          .                    |    |               |    |     |
 *      |       16SemIndx[c16Sem-1]     |    |               |    |     |
 *      |-------------------------------|    |               |    |     |
 *      |       hmte[0] (or "name str") |<---|---------------|    |     |
 *      |          .                    |    |                    |     |
 *      |          .                    |    |                    |     |
 *      |          .                    |    |                    |     |
 *      |       hmte[cLib-1]            |    |                    |     |
 *      |-------------------------------|    |                    |     |
 *      |       hshmem[0]               |<---|--------------------|     |
 *      |          .                    |    |                          |
 *      |          .                    |    |                          |
 *      |          .                    |    |                          |
 *      |       hshmem[cShrMem-1]       |    |                          |
 *      |-------------------------------|    |                          |
 *      |       fsinfo[0]               |<---|--------------------------|
 *      |          .                    |    |
 *      |          .                    |    |
 *      |          .                    |    |
 *      |       fsinfo[cFS-1]           |    |
 *      |-------------------------------|    |
 *                                      <-----
 *      NOTE that the process name string will be stored in place of hmtes
 *              if MTE information is NOT being requested.
 *      NOTE that following this structure in the user buffer is
 *              an array c32Sems long of PRUN structures for 32 bit sems
 *              an array c16Sems long of indices for 16 bit sems
 *              the process name string
 */
typedef struct qsPrec_s {  /* qsPrec */
        ULONG         RecType;        /* type of record being processed */
        qsTrec_t  FAR *pThrdRec;      /* ptr to thread recs for this proc */
        USHORT        pid;            /* process ID */
        USHORT        ppid;           /* parent process ID */
        ULONG         type;           /* process type */
        ULONG         stat;           /* process status */
        ULONG         sgid;           /* process screen group */
        USHORT        hMte;           /* program module handle for process */
        USHORT        cTCB;           /* # of TCBs in use */
        ULONG         c32PSem;        /* # of private 32-bit sems in use */
        void      FAR *p32SemRec;     /* pointer to head of 32bit sem info */
        USHORT        c16Sem;         /* # of 16 bit system sems in use */
        USHORT        cLib;           /* number of runtime linked libraries */
        USHORT        cShrMem;        /* number of shared memory handles */
        USHORT        cFH;            /* number of open files - BUGBUG see SFNContainer! */
        USHORT   FAR  *p16SemRec;     /* pointer to head of 16 bit sem info */
        USHORT   FAR  *pLibRec;       /* ptr to list of runtime libraries */
        USHORT   FAR  *pShrMemRec;    /* ptr to list of shared mem handles */
        USHORT   FAR  *pFSRec;        /* pointer to list of file handles */
} qsPrec_t;

/*
 *      16 bit system semaphore structure
 *      ________________________________
 *      |       pNextRec                |----|
 *      |-------------------------------|    |
 *      |SysSemData     :               |    |
 *      |       SysSemOwner             |    |
 *      |       SysSemFlag              |    |
 *      |       SysSemRecCnt            |    |
 *      |       SysSemProcCnt           |    |
 *      |-------------------------------|    |
 *      |-------------------------------|    |
 *      |-------------------------------|    |
 *      |       SysSemPtr               |    |
 *      |-------------------------------|    |
 *      |SysSemName:                    |    |
 *      |       "pathname"              |    |
 *      |-------------------------------|    |
 *                                      <-----
 */


/* SysSemFlag values */

#define QS_SYSSEM_WAITING 0x01               /* a thread is waiting on the sem */
#define QS_SYSSEM_MUXWAITING 0x02            /* a thread is muxwaiting on the sem */
#define QS_SYSSEM_OWNER_DIED 0x04            /* the process/thread owning the sem died */
#define QS_SYSSEM_EXCLUSIVE 0x08             /* indicates a exclusive system semaphore */
#define QS_SYSSEM_NAME_CLEANUP 0x10          /* name table entry needs to be removed */
#define QS_SYSSEM_THREAD_OWNER_DIED 0x20     /* the thread owning the sem died */
#define QS_SYSSEM_EXITLIST_OWNER 0x40        /* the exitlist thread owns the sem */

typedef struct qsS16rec_s {   /* qsS16rec */
        ULONG         NextRec;        /* offset to next record in buffer */
                                      /* System Semaphore Table Structure */
        USHORT        SysSemOwner ;   /* thread owning this semaphore */
        UCHAR         SysSemFlag ;    /* system semaphore flag bit field */
        UCHAR         SysSemRefCnt ;  /* number of references to this sys sem */
        UCHAR         SysSemProcCnt ; /* number of requests for this owner */
        UCHAR         SysSemPad ;     /* pad byte to round structure up to word */
        USHORT        pad_sh;
        USHORT        SemPtr;         /* RMP SysSemPtr field */
        char          SemName;        /* start of semaphore name string */
} qsS16rec_t;

typedef struct qsS16Headrec_s {  /* qsS16Hrec */
        ULONG         SRecType;
        ULONG         SpNextRec;      /* overlays NextRec of 1st qsS16rec_t*/
        ULONG         S32SemRec;
        ULONG         S16TblOff;      /* offset of SysSemDataTable */
        ULONG         pSem16Rec;
} qsS16Headrec_t;

/*
 *      System wide Shared Mem information
 *      ________________________________
 *      |       NextRec                 |
 *      |-------------------------------|
 *      |       hmem                    |
 *      |-------------------------------|
 *      |       sel                     |
 *      |-------------------------------|
 *      |       refcnt                  |
 *      |-------------------------------|
 *      |       name                    |
 *      |_______________________________|
 *
 */
typedef struct qsMrec_s {  /* qsMrec */
        ULONG         MemNextRec;       /* offset to next record in buffer */
        USHORT        hmem;             /* handle for shared memory */
        USHORT        sel;              /* selector */
        USHORT        refcnt;           /* reference count */
        char          Memname;          /* start of shared memory name string */
} qsMrec_t;

/*
 *      32 bit system semaphore structure
 *      ________________________________
 *      |       pNextRec                |----|
 *      |-------------------------------|    |
 *      |       QSHUN[0]                |    |
 *      |-------------------------------|    |
 *      |         MuxQ                  |    |
 *      |-------------------------------|    |
 *      |         OpenQ                 |    |
 *      |-------------------------------|    |
 *      |         SemName               |    |
 *      |-------------------------------|<---|
 *      |          .                    |
 *      |          .                    |
 *      |-------------------------------|<---|
 *      |       pNextRec                |----|
 *      |-------------------------------|    |
 *      |       QSHUN[c32SSem-1]        |    |
 *      |-------------------------------|    |
 *      |         MuxQ                  |    |
 *      |-------------------------------|    |
 *      |         OpenQ                 |    |
 *      |-------------------------------|    |
 *      |         SemName               |    |
 *      |-------------------------------|<---|
 */

/*
 *  32- bit Semaphore flags
 */

#define QS_DC_SEM_SHARED   0x0001   //  Shared Mutex, Event or MUX semaphore
#define QS_DCMW_WAIT_ANY   0x0002   //  Wait on any event/mutex to occur
#define QS_DCMW_WAIT_ALL   0x0004   //  Wait on all events/mutexs to occur
#define QS_DCM_MUTEX_SEM   0x0008   //  Mutex semaphore
#define QS_DCE_EVENT_SEM   0x0010   //  Event semaphore
#define QS_DCMW_MUX_SEM    0x0020   //  Muxwait semaphore
#define QS_DC_SEM_PM       0x0040   //  PM Shared Event Semphore
#define QS_DE_POSTED       0x0040   //  event sem is in the posted state
#define QS_DM_OWNER_DIED   0x0080   //  The owning process died
#define QS_DMW_MTX_MUX     0x0100   //  MUX contains mutex sems
#define QS_DHO_SEM_OPEN    0x0200   //  Device drivers have opened this semaphore
#define QS_DE_16BIT_MW     0x0400   //  Part of a 16-bit MuxWait
#define QS_DCE_POSTONE     0x0800   //  Post one flag event semaphore
#define QS_DCE_AUTORESET   0x1000   //  Auto-reset event semaphore

typedef struct qsopenq_s {    /* qsopenq */
        PID           pidOpener;      /* process id of opening process */
        USHORT        OpenCt;         /* number of opens for this process */
} QSOPENQ;
typedef struct qsevent_s {    /* qsevent */
        QSOPENQ       *pOpenQ;        /* pointer to open q entries */
        UCHAR         *pName;         /* pointer to semaphore name */
        ULONG         *pMuxQ;         /* pointer to the mux queue */
        USHORT        flags;
        USHORT        PostCt;         /* # of posts */
} QSEVENT;
typedef struct qsmutex_s {    /* qsmutex */
        QSOPENQ       *pOpenQ;        /* pointer to open q entries */
        UCHAR         *pName;         /* pointer to semaphore name */
        ULONG         *pMuxQ;         /* pointer to the mux queue */
        USHORT        flags;
        USHORT        ReqCt;          /* # of requests */
        USHORT        SlotNum;        /* slot # of owning thread */
        PADSHORT;
} QSMUTEX;
typedef struct qsmux_s {   /* qsmux */
        QSOPENQ         *pOpenQ;        /* pointer to open q entries */
        UCHAR           *pName;         /* pointer to semaphore name */
        void            *pSemRec;       /* array of semaphore record entries */
        USHORT          flags;
        USHORT          cSemRec;        /* count of semaphore records */
        USHORT          WaitCt;         /* # threads waiting on the mux */
        PADSHORT;
} QSMUX;
typedef struct qsshun_s {  /* qsshun */
        QSEVENT         qsSEvt;         /* shared event sem */
        QSMUTEX         qsSMtx;         /* shared mutex sem */
        QSMUX           qsSMux;         /* shared mux sem */
} QSHUN;
typedef struct qsS32rec_s {   /* qsS32rec */
        void            *pNextRec;      /* pointer to next record in buffer */
        QSHUN           qsh;            /* qstate version of SHUN record */
} qsS32rec_t;

/*
 *      System wide MTE information
 *      ________________________________
 *      |       pNextRec                |----|
 *      |-------------------------------|    |
 *      |       hmte                    |    |
 *      |-------------------------------|    |
 *      |       ctImpMod                |    |
 *      |-------------------------------|    |
 *      |       ctObj                   |    |
 *      |-------------------------------|    |
 *      |       pObjInfo                |----|----------|
 *      |-------------------------------|    |          |
 *      |       pName                   |----|----|     |
 *      |-------------------------------|    |    |     |
 *      |       imported module handles |    |    |     |
 *      |          .                    |    |    |     |
 *      |          .                    |    |    |     |
 *      |          .                    |    |    |     |
 *      |-------------------------------| <--|----|     |
 *      |       "pathname"              |    |          |
 *      |-------------------------------| <--|----------|
 *      |       Object records          |    |
 *      |       (if requested)          |    |
 *      |_______________________________|    |
 *                                      <-----
 *      NOTE that if the level bit is set to QS_MTE, the base Lib record will be followed
 *      by a series of object records (qsLObj_t); one for each object of the
 *      module.
 */

typedef struct qsLObjrec_s {  /* qsLOrec */
        ULONG         oaddr;  /* object address */
        ULONG         osize;  /* object size */
        ULONG         oflags; /* object flags */
} qsLObjrec_t;

typedef struct qsLrec_s {     /* qsLrec */
        void  FAR        *pNextRec;      /* pointer to next record in buffer */
        USHORT           hmte;           /* handle for this mte */
        USHORT           fFlat;          /* true if 32 bit module */
        ULONG            ctImpMod;       /* # of imported modules in table */
        ULONG            ctObj;          /* # of objects in module (mte_objcnt)*/
        qsLObjrec_t FAR  *pObjInfo;      /* pointer to per object info if any */
        UCHAR     FAR    *pName;         /* -> name string following struc */
#if 0
        USHORT            ahmte[1];      /* Array of imported module hmtes */
#endif
} qsLrec_t;

/* Used for 9th bit (Extended Module Data Summary)*/
typedef struct qsExLrec_s {   /* qsELrec */
        struct          qsExLrec_s *next; /*  Pointer to next Extended Module Data */
        USHORT          hndmod;           /*  Module Handle */
        USHORT          pid;              /*  Process ID */
        USHORT          type;             /*  Type of Module */
        ULONG           refcnt;           /*  Size of reference array */
        ULONG           segcnt;           /*  Number of segments in module */
        void            *_reserved_;
        UCHAR FAR       *name;            /*  Pointer to Module Name  */
        ULONG           ModuleVersion;    /*  Module version value  */
        UCHAR FAR       *ShortModName;    /*  New Pointer to Module short name */
        ULONG           modref;           /*  Start of array of handles of module */
}qsExLrec_t;

/*
 *      System wide FILE information
 *      ________________________________
 *      |       RecType                 |
 *      |-------------------------------|
 *      |       pNextRec                |-------|
 *      |-------------------------------|       |
 *      |       ctSft                   |       |
 *      |-------------------------------|       |
 *      |       pSft                    |---|   |
 *      |-------------------------------|   |   |
 *      |       name                    |   |   |
 *      |-------------------------------|<--|   |
 *      |       qsSft[0]                |       |
 *      |-------------------------------|       |
 *      |       ...                     |       |
 *      |-------------------------------|       |
 *      |       qsSft[ctSft -1]         |       |
 *      |_______________________________|       |
 *      |       name                    |
 *      |_______________________________|
 *                                      <-------|
 */
typedef struct qsSft_s {   /* qsSft - the size of this should be 0x16! */
        USHORT        sfn;            /* 0 SFN sf_fsi.sfi_selfSFN */
        USHORT        refcnt;         /* 2 sf_ref_count */
        USHORT        flags;          /* 4 sf_flags */
        USHORT        flags2;         /* 6 sf_flags2 */
        USHORT        mode;           /* 8 sf_fsi.sfi_mode - mode of access */
        USHORT        mode2;          /* 10 sf_fsi.sfi_mode2 - mode of access */
        ULONG         size;           /* 12 sf_fsi.sfi_size */
        USHORT        hVPB;           /* 16 sf_fsi.sfi_hVPB handle of volume */
        USHORT        attr;           /* 18 sf_attr */
        PADSHORT;                     /* 20 */
} qsSft_t;

typedef struct qsFrec_s {  /* qsFrec */
        ULONG         RecType;        /* Record Type */
        void          *pNextRec;      /* pointer to next record in buffer */
        ULONG         ctSft;          /* # sft entries for this MFT entry */
        qsSft_t       *pSft;          /* -> start of sft entries in buf */
#if 0
        char          name[1];        /* kso: start of name? */
#endif
} qsFrec_t;


/* Pointer Record Structure
 *      This structure is the first in the user buffer.
 *      It contains pointers to heads of record types that are loaded
 *      into the buffer.
 */

typedef struct qsPtrRec_s {   /* qsPRec */
        qsGrec_t        *pGlobalRec;
        qsPrec_t        *pProcRec;      /* ptr to head of process records */
        qsS16Headrec_t  *p16SemRec;     /* ptr to head of 16 bit sem recds */
        qsS32rec_t      *p32SemRec;     /* ptr to head of 32 bit sem recds */
        qsMrec_t        *pMemRec;       /* ptr to head of shared mem recs */
        qsLrec_t        *pLibRec;       /* ptr to head of mte records */
        qsMrec_t        *pShrMemRec;    /* ptr to head of shared mem records */
        qsFrec_t        *pFSRec;        /* ptr to head of file sys records */
} qsPtrRec_t;

#pragma pack()

#endif

/* IMPORTANT, the sizeof qsSft_t is 0x16 bytes! */
#define SIZEOFQSSFT_T   0x16

/*******************************************************************************
*   Structures and Typedefs                                                    *
*******************************************************************************/
typedef struct ProcessData
{
    USHORT       usPid;                 /* sort key - process identificator. */
    ULONG        ulUserTime;            /* total time in user code */
    ULONG        ulSysTime;             /* total time in system code */
    BOOL         fDead;                 /* Dead flag. */
    BOOL         fDirty;                /* Update flag. Set: update container element. */
    ULONG        cbProcRec;             /* Size of all the data used in pProcRec. */
    qsPrec_t    *pProcRec;              /* Pointer to data returned by DosQuerySysState */
                                        /* Note that this data is voilatile! */
    /* container stuff */
    PVOID        pvRecordCore;          /* Pointer to container record core. */

    struct ProcessData *pNext;          /* Next pointer */
    struct ProcessData *pPrev;          /* Prev pointer */
} PROCESSDATA, *PPROCESSDATA;


/*******************************************************************************
*   Functions                                                                  *
*******************************************************************************/
BOOL            QSUpdateStateData(VOID);
VOID            QSInsertProcessData(PPROCESSDATA pProcData);
VOID            QSRemoveProcessData(PPROCESSDATA pProcData);
PPROCESSDATA    QSGetProcessData(USHORT usPid);
PPROCESSDATA    QSGetFirstProcessDataNode(VOID);
qsLrec_t *      QSGetMteData(register USHORT hMTE);
qsLrec_t *      QSGetMteFirstData(VOID);
qsLrec_t *      QSGetDosCall1Data(VOID);
qsFrec_t *      QSGetFSFirstData(VOID);
qsFrec_t *      QSGetSfnData(USHORT usSFN);
qsMrec_t *      QSGetMemFirstData(VOID);
qsMrec_t *      QSGetShrMemFirstData(VOID);
VOID            QSDumpProcessData(VOID);
VOID            QSDumpFileSystemData(VOID);

APIRET  APIENTRY DosQuerySysState(ULONG EntityList,
                                  ULONG EntityLevel,
                                  PID pid,
                                  TID tid,
                                  PVOID pDataBuf,
                                  ULONG cbBuf);
#ifdef __cplusplus
}
#endif

#endif

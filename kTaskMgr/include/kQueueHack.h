/* $Id: kQueueHack.h,v 1.1 2000/05/23 00:16:51 stknut Exp $
 *
 * OS/2 Queue info hacks.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */

#ifndef _kQueueHack_h_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
*   Structures and Typedefs                                                    *
*******************************************************************************/
#pragma pack(1)
/*
 * Hackish! Guesswork!
 */
typedef struct _QueEntry /* size: 0x21 (variable sized last member!) */
{
    CHAR    chFlags;                    /* 0x00 db  Queue flags. DosCreateQueue flags ++. */
    CHAR    cOpen;                      /* 0x01 db  Open count. (DosOpenQueue) */
    char    ach1[0x02];                 /* 0x02 - */
    USHORT  usPriority;                 /* 0x04 dw  Some priority stuff?  */
    USHORT  hQueue;                     /* 0x06 dw  Queue handle. */
    USHORT  usOwnPid;                   /* 0x08 dw  Owner Process Identifier. */
    USHORT  cEntries;                   /* 0x0a dw  Number of elements in the queue. */
                                        /*          (DosQueryQueue returns this.) */
    ULONG   fpFirst;                    /* 0x0c dd  Pointer to first queue element. */
    ULONG   fpLast;                     /* 0x10 dd  Pointer to last queue element. */
    ULONG   fpNext;                     /* 0x14 dd  Far pointer to next queue entry. */
    ULONG   hRamSem;                    /* 0x18 dd  Seems like this is a Fast-Safe Ram Semaphore. */
    ULONG   hEventSem;                  /* 0x1c dd  Seems like this is an Event Semaphore. (Events are at least posted to it!) */
    CHAR    szQueName[1];               /* 0x20 db  Zero string containing the queue name. */
} QUEENTRY, *PQUEENTRY;


/*
 * Hackish! Guesswork!
 */
typedef struct _QueOpenEntry
{
    CHAR    chFlags;                    /* 0x00  Flags */
    CHAR    chRefCnt;                   /* 0x01  Number of opens by this usPid the hQueue. */
    USHORT  usPid;                      /* 0x02  Process Identifier. */
    USHORT  hQueue;                     /* 0x04  Queue handle. */
    ULONG   fpNext;                     /* 0x06  Pointer to next open record. (Far 16:16) */
} QUEOPENENTRY, *PQUEOPENENTRY;


/*
 * Hackish! Guesswork!
 */
typedef struct _questatseg
{
    CHAR            ch1;
    ULONG           ul1;
    USHORT          us1;
    USHORT          us2;
    ULONG           fpFirstQue;         /* Far 16:16 */
    ULONG           fpFirstOpen;        /* Far 16:16 */
    USHORT          us3;
} QUESTATSEG, *PQUESTATSEG;
#pragma pack()


/*
 * My queue entry.
 */
typedef struct _MyQueEntry
{
    struct _MyQueEntry  *pNext;
    QUEENTRY             que;
} MYQUEENTRY, *PMYQUEENTRY;


/*
 * My Open Entry
 */
#pragma pack(1)
typedef struct _MyQueOpenEntry
{
    CHAR    chFlags;                    /* 0x00  Flags */
    CHAR    chRefCnt;                   /* 0x01  Number of opens by this usPid the hQueue. */
    USHORT  usPid;                      /* 0x02  Process Identifier. */
    USHORT  hQueue;                     /* 0x04  Queue handle. */
    struct _MyQueOpenEntry *pNext;      /* 0x06  Pointer to next open record. (FLAT) */
} MYQUEOPENENTRY, *PMYQUEOPENENTRY;
#pragma pack()



/*******************************************************************************
*   Exported Functions                                                         *
*******************************************************************************/
BOOL            QueInit(VOID);
VOID            QueCopyData(VOID);
PMYQUEENTRY     QueGetFirstQueueData(VOID);
PMYQUEOPENENTRY QueGetFirstOpenData(VOID);
ULONG           QueQueryUsedMemory(VOID);
ULONG           QueQueryUsedMemoryQueue(PMYQUEENTRY pQueue);


#ifdef __cplusplus
}
#endif

#endif

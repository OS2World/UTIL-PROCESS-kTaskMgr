/* $Id: kQueueRecord.h,v 1.1 2000/05/23 00:16:51 stknut Exp $
 *
 * kQueueRecord (kTaskMgr) - Queue container record.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kQueueRecord_h_
#define _kQueueRecord_h_


/**
 * Container data record for queue information.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kQueueRecord : public kTaskMgrRecordBase
{
private:
    USHORT          ushQueue;

    PSZ             pszhQueue;
    CHAR            szhQueue[8];

    PSZ             pszOwnPid;
    CHAR            szOwnPid[8];

    PSZ             pszFlags;
    CHAR            szFlags[40];

    PSZ             pszPriority;
    CHAR            szPriority[20];

    PSZ             pszEntries;
    CHAR            szEntries[8];

    PSZ             pszOwnerOpens;
    CHAR            szOwnerOpens[8];

    PSZ             pszUsedMem;
    CHAR            szUsedMem[16];

    PSZ             pszName;
    CHAR            szName[CCHMAXPATH];

public:

    /**@cat public methods */
    VOID    init();
    VOID    set(PMYQUEENTRY pQueEntry);

    USHORT  gethQueue()       { return ushQueue; }
    PCSZ    getFlags()        { return pszFlags; }
    PCSZ    getEntries()      { return pszEntries; }
    PCSZ    getOwnPid()       { return pszOwnPid; }
    PCSZ    getPriority()     { return pszPriority; }
    PCSZ    getOwnerOpens()   { return pszOwnerOpens; }
    PCSZ    getUsedMem()      { return pszUsedMem; }
    PCSZ    getName()         { return pszName; }


    /**@cat public static data */
    static const FIELDINFO aFieldInfo[];
    static const unsigned  cFieldInfo;
};

#endif

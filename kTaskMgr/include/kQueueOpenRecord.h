/* $Id: kQueueOpenRecord.h,v 1.1 2000/05/23 00:16:51 stknut Exp $
 *
 * kQueueOpenRecord (kTaskMgr) - Open queue container record.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kQueueOpenRecord_h_
#define _kQueueOpenRecord_h_


/**
 * Container data record for queue information.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kQueueOpenRecord : public kTaskMgrRecordBase
{
private:
    USHORT          ushQueue;
    USHORT          usPid;

    PSZ             pszhQueue;
    CHAR            szhQueue[8];

    PSZ             pszPid;
    CHAR            szPid[8];

    PSZ             pszPidOpenCnt;
    CHAR            szPidOpenCnt[8];

    PSZ             pszOpenFlags;
    CHAR            szOpenFlags[8];

    PSZ             pszOwnPid;
    CHAR            szOwnPid[8];

    PSZ             pszFlags;
    CHAR            szFlags[30];

    PSZ             pszPriority;
    CHAR            szPriority[20];

    PSZ             pszEntries;
    CHAR            szEntries[8];

    PSZ             pszOwnerOpens;
    CHAR            szOwnerOpens[8];

    PSZ             pszName;
    CHAR            szName[CCHMAXPATH];

public:

    /**@cat public methods */
    VOID    init();
    VOID    set(PMYQUEOPENENTRY pOpenEntry);

    USHORT  gethQueue()         { return ushQueue; }
    USHORT  getPid()            { return usPid; }
    PCSZ    getPidOpenCnt()     { return pszPidOpenCnt; }
    PCSZ    getOpenFlags()      { return pszOpenFlags; }
    PCSZ    getOwnPid()         { return pszOwnPid; }
    PCSZ    getFlags()          { return pszFlags; }
    PCSZ    getPriority()       { return pszPriority; }
    PCSZ    getEntries()        { return pszEntries; }
    PCSZ    getOwnerOpens()     { return pszOwnerOpens; }
    PCSZ    getName()           { return pszName; }


    /**@cat public static data */
    static const FIELDINFO aFieldInfo[];
    static const unsigned  cFieldInfo;
};

#endif

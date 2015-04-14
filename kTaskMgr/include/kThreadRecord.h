/* $Id: kThreadRecord.h,v 1.1 2000/05/23 00:16:51 stknut Exp $
 *
 * kThreadRecord - Thread container record.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kThreadRecord_h_
#define _kThreadRecord_h_


/**
 * Container data record.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kThreadRecord : public kTaskMgrRecordBase
{
private:
    USHORT      usTid;
    CHAR        szTid[10];
    PSZ         pszTid;
    USHORT      usSlot;
    CHAR        szSlot[10];
    PSZ         pszSlot;
    USHORT      usPid;
    CHAR        szPid[10];
    PSZ         pszPid;
    CHAR        szSleepId[10];
    PSZ         pszSleepId;
    CHAR        szPriority[10];
    PSZ         pszPriority;
    CHAR        szUserTime[16];
    PSZ         pszUserTime;
    CHAR        szSysTime[16];
    PSZ         pszSysTime;
    CHAR        szState[20];
    PSZ         pszState;

public:

    /**@cat public methods */
    VOID        init();
    VOID        set(qsTrec_t *pThreadRec);
    VOID        set(qsTrec_t *pThreadRec, PPROCESSDATA    pProcData);

    USHORT      getTid()    {return usTid;}
    USHORT      getSlot()   {return usSlot;}
    USHORT      getPid()    {return usPid;}

    /**@cat public static data */
    static const FIELDINFO aFieldInfo[];
    static const FIELDINFO aFieldInfoGlobal[];
    static const unsigned  cFieldInfo;
    static const unsigned  cFieldInfoGlobal;
};

#endif

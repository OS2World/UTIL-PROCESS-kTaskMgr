/* $Id: kProcessRecord.h,v 1.1 2000/05/23 00:16:50 stknut Exp $
 *
 * kProcessRecord - Process container record.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kProcessRecord_h_
#define _kProcessRecord_h_


/**
 * Container data record.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kProcessRecord : public kTaskMgrRecordBase
{
private:
    PPROCESSDATA  pProcData;
    USHORT        usPid;
    CHAR          szPid[10];
    PSZ           pszPid;
    CHAR          szPPid[10];
    PSZ           pszPPid;
    CHAR          szName[CCHMAXPATH];
    PSZ           pszName;
    CHAR          szUserTime[16];
    PSZ           pszUserTime;
    CHAR          szSysTime[16];
    PSZ           pszSysTime;

public:

    /**@cat public methods */
    VOID  init();
    VOID  set(PPROCESSDATA pProcData);
    USHORT getPid()             {return usPid;}
    const char * getPPid()      {return pszPPid;}
    const char * getName()      {return pszName;}
    const char * getUserTime()  {return pszUserTime;}
    const char * getSysTime()   {return pszSysTime;}

    /**@cat public static data */
    static const FIELDINFO aFieldInfo[];
    static const unsigned  cFieldInfo;
};


#endif

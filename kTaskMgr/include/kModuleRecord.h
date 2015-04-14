/* $Id: kModuleRecord.h,v 1.1 2000/05/23 00:16:50 stknut Exp $
 *
 * kModuleRecord - Container record.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kModuleRecord_h_
#define _kModuleRecord_h_


/**
 * Container data record for executable modules.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kModuleRecord : public kTaskMgrRecordBase
{
private:
    USHORT        hMte;

    CHAR          szhMte[10];
    PSZ           pszhMte;
    CHAR          szType[10];
    PSZ           pszType;
    CHAR          szcImpMods[10];
    PSZ           pszcImpMods;
    CHAR          szcObjects[10];
    PSZ           pszcObjects;
    CHAR          szName[CCHMAXPATH];
    PSZ           pszName;

public:

    /**@cat public methods */
    VOID            init();
    VOID            set(qsLrec_s *pMteRec);
    VOID            set(USHORT hMte);
    USHORT          gethMte()       {return hMte;}
    const char *    getType()       {return pszType;}
    const char *    getImpMods()    {return pszcImpMods;}
    const char *    getObjects()    {return pszcObjects;}
    const char *    getName()       {return pszName;}

    /**@cat public static data */
    static const FIELDINFO aFieldInfo[];
    static const unsigned  cFieldInfo;
};

#endif

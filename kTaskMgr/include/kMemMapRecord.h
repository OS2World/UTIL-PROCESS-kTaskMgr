/* $Id: kMemMapRecord.h,v 1.1 2000/06/19 20:08:42 stknut Exp $
 *
 * kMemMapRecord - Container record.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kMemMapRecord_h_
#define _kMemMapRecord_h_


/**
 * Container data record for memorymap objects.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kMemMapRecord : public kObjectRecord
{
private:
    ULONG         ulBase;

    CHAR          szhMod[10];
    PSZ           pszhMod;

    CHAR          szModName[CCHMAXPATH];
    PSZ           pszModName;

public:

    /**@cat public methods */
    VOID    init();
    VOID    set(qsLObjrec_t *pObjRec, int iObjNo, USHORT hMod, PCSZ pszModName);
    VOID    set(qsMrec_t *pObjRec);
    USHORT  getBase() {return ulBase;}

    /**@cat public static data */
    static const FIELDINFO aFieldInfo[];
    static const unsigned  cFieldInfo;
};

#endif

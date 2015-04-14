/* $Id: kObjectRecord.h,v 1.2 2000/06/19 20:08:42 stknut Exp $
 *
 * kObjectRecord - Container record.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kObjectRecord_h_
#define _kObjectRecord_h_


/**
 * Container data record for executable objects.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kObjectRecord : public kTaskMgrRecordBase
{
protected:
    int           iObjNo;

    CHAR          szObjNo[10];
    PSZ           pszObjNo;
    CHAR          szBase[16];
    PSZ           pszBase;
    CHAR          szSize[10];
    PSZ           pszSize;
    CHAR          szFlags[10];
    PSZ           pszFlags;
    CHAR          szFlagsExp[100];
    PSZ           pszFlagsExp;

public:

    /**@cat public methods */
    VOID    init();
    VOID    set(qsLObjrec_t *pObjRec, int iObjNo);
    USHORT  getObjNo() {return iObjNo;}

    /**@cat public static data */
    static const FIELDINFO aFieldInfo[];
    static const unsigned  cFieldInfo;
};

#endif

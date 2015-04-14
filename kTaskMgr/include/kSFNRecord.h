/* $Id: kSFNRecord.h,v 1.1 2000/05/23 00:16:51 stknut Exp $
 *
 * kSFNRecord (kTaskMgr) - SFN container record.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kSFNRecord_h_
#define _kSFNRecord_h_


/**
 * Container data record for system file number records (SFTs) - open files.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kSFNRecord : public kTaskMgrRecordBase
{
private:
    USHORT          usJFN;
    USHORT          usSFN;

    PSZ             pszJFN;
    CHAR            szJFN[8];

    PSZ             pszSFN;
    CHAR            szSFN[8];

    PSZ             pszRefCnt;
    CHAR            szRefCnt[8];

    PSZ             pszFlags;
    CHAR            szFlags[8];

    PSZ             pszFlags2;
    CHAR            szFlags2[8];

    PSZ             pszMode;
    CHAR            szMode[8];

    PSZ             pszMode2;
    CHAR            szMode2[8];

    PSZ             pszSize;
    CHAR            szSize[16];

    PSZ             pszhVPB;
    CHAR            szhVPB[8];

    PSZ             pszAttr;
    CHAR            szAttr[8];

    PSZ             pszName;
    CHAR            szName[CCHMAXPATH];

public:

    /**@cat public methods */
    VOID    init();
    VOID    set(qsSft_t *pSftRec, PCSZ pszFilename, USHORT usJFN = 0xFFFF);

    USHORT  getSFN()          { return usSFN; }
    USHORT  getJFN()          { return usJFN; }
    PCSZ    getRefCnt()       { return pszRefCnt; }
    PCSZ    getFlags()        { return pszFlags; }
    PCSZ    getFlags2()       { return pszFlags2; }
    PCSZ    getMode()         { return pszMode; }
    PCSZ    getMode2()        { return pszMode2; }
    PCSZ    getSize()         { return pszSize; }
    PCSZ    gethVPB()         { return pszhVPB; }
    PCSZ    getAttr()         { return pszAttr; }
    PCSZ    getName()         { return pszName; }


    /**@cat public static data */
    static const FIELDINFO aFieldInfo[];
    static const unsigned  cFieldInfo;
};

#endif

/* $Id: kDetailsRecord.h,v 1.1 2000/05/23 00:16:50 stknut Exp $
 *
 * kDetailsRecord - Details container record.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kDetailsRecord_h_
#define _kDetailsRecord_h_


/**
 * Detail container record.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kDetailsRecord : public kTaskMgrRecordBase
{
private:
    int     iIndex;
    PCSZ    pszItem;
    PCSZ    pszValue;

public:
    /**@cat public methods */
    VOID    init(int iIndex = -1);
    VOID    set(PCSZ pszItem, PCSZ pszValue = NULL);

    PCSZ    getItem() const     { return pszItem  != pszDefault ? pszItem  : NULL; }
    PCSZ    getValue() const    { return pszValue != pszDefault ? pszValue : NULL; }
    int     getIndex() const    { return iIndex; }

    BOOL    updateValue(PCSZ pszNewValue);

    /**@cat public static data */
    static const FIELDINFO aFieldInfo[];
    static const unsigned  cFieldInfo;
    static const PCSZ      pszDefault;
};

#endif

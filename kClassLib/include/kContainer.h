/* $Id: kContainer.h,v 1.1 2000/05/23 00:00:54 stknut Exp $
 *
 * kContainer (kClassLib)  - layer to the OS/2 container Windowclass.
 *                           kCnrMiniRecord, kCnrRecord, kContainer and kDetailCnr
 *
 * Copyright (c) 1999-2000 knut st. osmundsen
 *
 */
#ifndef _kContainer_h_
#define _kContainer_h_


/**
 * @description Basis containerrecord class - MINIRECORDCORE.
 * @author      knut st. osmundsen
 */
class kCnrMiniRecord
{
protected:
    MINIRECORDCORE RecordCore;
public:
    kCnrMiniRecord *getNext();
    kCnrMiniRecord *setNext(kCnrMiniRecord *pRecord);
};



/**
 * @description Basis containerrecord class - RECORDCORE.
 * @author      knut st. osmundsen
 */
class kCnrRecord
{
protected:
    RECORDCORE RecordCore;
public:
    kCnrRecord *getNext();
    kCnrRecord *setNext(kCnrRecord *pRecord);
};



/**
 * @description Basis container class.
 * @author      knut st. osmundsen
 */
class kCnrBase : public kBase
{
protected:
    /**@cat private data */
    HWND      hwndCnr;
    HWND      hwndBorder;
    PFNWP     pfnwpOrg;

    /**@cat Container info */
    BOOL      fUsesMiniRecord;
    BOOL      fValidCnrInfo;
    CNRINFO   CnrInfo;

    /**@cat Private methods */
    BOOL      validateCnrInfo();
    BOOL      setCnrInfo(ULONG flChanged);
    BOOL      usesMiniRecord();

protected:
    /**@cat Protected eventhandlers */
    virtual VOID  command(USHORT usCmd, USHORT usSource, BOOL fPointer); //WM_COMMAND
    virtual VOID  initMenu(USHORT usMenuId, HWND hwndMnu); //WM_INITMENU
    virtual VOID  menuEnd(USHORT usMenuId, HWND hwndMnu); //WM_ENDMENU

public:
    /**@cat Constructor/Destructor */
    kCnrBase(HWND hwndCnr, BOOL fBorder = TRUE) throw (kError);
    virtual ~kCnrBase();

    /**@cat Subclass message loop */
    MRESULT msgLoop(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

    /**@cat Alloc/free */
    kCnrMiniRecord  *allocMiniRec(ULONG cbRecord, ULONG cRecords = 1);
    kCnrRecord      *allocRec(ULONG cbRecord, ULONG cRecords = 1);
    PFIELDINFO       allocDetailFieldInfo(USHORT cFieldInfo);
    BOOL             freeDetailFieldInfo(PFIELDINFO pFieldInfo, USHORT cFieldInfo);

    /**@cat Insert-/remove-record methods */
    BOOL insertAtTop(kCnrMiniRecord *pRecord, ULONG cRecords /*=1*/);
    BOOL insertAtBottom(kCnrMiniRecord *pRecord, ULONG cRecords /*=1*/);
    BOOL insertDetailFieldInfo(PFIELDINFO pFieldInfo, PFIELDINFO pFieldInfoOrder,
                               ULONG fInvalidateFieldInfo, ULONG cFieldInfoInsert);
    BOOL removeRecord(void *pvRecord);
    BOOL removeRecords(void **ppvRecord, ULONG cRecord = 1);
    BOOL removeRecordAtTop(LONG cRecord);
    BOOL removeRecordAtBottom(LONG cRecord);
    BOOL removeAllRecords();
    BOOL removeDetailFieldInfo(PFIELDINFO paFieldInfo[], USHORT cFieldInfo, USHORT usFlags);
    BOOL removeAllDetailFieldInfos();


    /**@cat Invalidate(s) */
    BOOL invalidateRecord(kCnrMiniRecord *pRecord, ULONG fFlags = CMA_TEXTCHANGED);
    BOOL invalidateRecords(ULONG fFlags, ULONG cRecords, ...);
    BOOL invalidateRecords(ULONG fFlags, ULONG cRecords, kCnrMiniRecord **papRec);


    /**@cat Sets */
    BOOL setDelta(ULONG cDelta);
    BOOL setflWindowAttr(ULONG flWindowAttr);
    BOOL setTitle(PSZ pszTitle);
    BOOL setLastFieldInfo(PFIELDINFO pFieldInfoLast);
    BOOL setRecordEmphasis(kCnrMiniRecord *pRecord, BOOL fChangeEmphasis, USHORT fEmphasisAttribute);
    BOOL setPos(ULONG ulPos);


    /**@cat Misc */
    BOOL disableUpdate();
    BOOL enableUpdate();
    BOOL resize(LONG cx, LONG cy);
    BOOL setpos(LONG x, LONG y);
    BOOL resizeAndMove(LONG x, LONG y, LONG cx, LONG cy);

    /**@cat Query-record methods */
    kCnrMiniRecord  *queryMenuSourceRecord(kCnrMiniRecord *pStartRecord = NULL);
    kCnrMiniRecord  *querySelectedRecord(kCnrMiniRecord *pStartRecord = NULL);
    kCnrMiniRecord  *queryInUseRecord(kCnrMiniRecord *pStartRecord = NULL);
    kCnrMiniRecord  *queryCursoredRecord(kCnrMiniRecord *pStartRecord = NULL);
    kCnrMiniRecord  *queryTopRecord();
    kCnrMiniRecord  *queryBottomRecord();
    kCnrMiniRecord  *queryNextRecord(kCnrMiniRecord *pStartRecord = NULL);
    kCnrMiniRecord  *queryPrevRecord(kCnrMiniRecord *pStartRecord = NULL);

    /**@cat Other queries */
    ULONG            getCountOfRecords();
    ULONG            getflWindowAttr();
    PFIELDINFO       queryDetailFieldInfo(USHORT flCmd = CMA_FIRST, PFIELDINFO pfldInfoBase = NULL);
};



/**
 * @description Detail container class.
 * @author      knut st. osmundsen
 */
class kDetailCnr : public kCnrBase
{
protected:
    ULONG cColumns;

    virtual  SHORT  sortCallBack(kCnrMiniRecord *pRecord1, kCnrMiniRecord *pRecord2);

public:
    kDetailCnr(HWND hwndCnr, ULONG flWindowAttr, PSZ pszTitle, ULONG cColumns, PFIELDINFO paFieldInfo, BOOL fBorder = TRUE) throw (kError);
    virtual ~kDetailCnr();

    BOOL            enableSorting();
    BOOL            disableSorting();

    friend SHORT _System sortForwarder(PMINIRECORDCORE pRecord1, PMINIRECORDCORE pRecord2, PVOID pvThis);
};

#endif

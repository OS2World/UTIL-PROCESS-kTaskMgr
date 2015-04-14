/* $Id: kModuleDetails.h,v 1.2 2000/05/23 00:51:25 stknut Exp $
 *
 * Module Detail Dialog.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */
#ifndef _kModuleDetails_h_
#define _kModuleDetails_h_



/**
 * Module Detail Dialog.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kModuleDetails : public kDlgBase, public kDetailsBase
{
private:
    /** @cat */
    HWND                hwndOwner;
    kObjectContainer *  pCnrObjects;
    kModuleContainer *  pCnrImpModules;
    USHORT              hMTE;

    /** @cat event overrides */
    VOID  cnrContextMenu(USHORT usId, PRECORDCORE pRecord);
    VOID  cnrEnter(USHORT usId, HWND hwndCnr, PRECORDCORE pRecord, ULONG fKey);

    /** @cat helpers */
    VOID resize(PSWP pswpOld, PSWP pswpNew);

public:
    /** @cat Constructor and Destructor */
    kModuleDetails(USHORT hMte, HWND hwndOwner) throw(kError);
    ~kModuleDetails();

    BOOL    update();

    static BOOL invoke(USHORT hMte, HWND hwndOwner);
};

#endif

/* $Id: kBeliever.cpp,v 1.2 2000/06/19 22:24:24 stknut Exp $
 *
 * kBeliever - a note for believers in Jesus Christ.
 *
 * Copyright (c) 2000 knut st. osmundsen
 *
 */

/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
#define INCL_WINERRORS
#define INCL_WINDIALOG
#define INCL_WININPUT
#define INCL_WINDIALOGS
#define INCL_WINFRAMEMGR

/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <os2.h>
#ifdef USE_KLIB
    #include <kAssert.h>
    #include <kLog.h>
    #include <kHeap.h>
#endif
#include "kBase.h"
#include "kError.h"
#include "kDlgBase.h"
#include "kClickDlg.h"

#include "kBeliever.h"
#include "kTaskMgr_defs.h"


/**
 * Constructor
 * @param     hwndOwner  Handle to owner window.
 * @author    knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
kBeliever::kBeliever(HWND hwndOwner)
    :kClickDlg(hwndOwner, DL_KBELIEVER, NULLHANDLE)
{
    WinSetDlgItemText(hwnd, DL_KBELIEVER_MLE,
"If you Believe that Jesus is The Son of God, and that Hi, Jesus Christ, "
"is your Lord, I would humbly ask you to prayer over my one and only love, "
"Grace, and me. We are having a very hard time right now. She has actually "
"broken our engagement and left me after heavy pressure from her parents. "
"(Her parents are good people, but they don't know me at all. They nearly "
"addmitted that they might been mistaken about me when Grace decided to give up.)\r"
"\r"
"I believe that the Lord Jesus will repair our relationship. He has promised "
"me so!!! Since last release He has answered that Grace and I will be to gethere again. "
"Praise God! But we aren't togethere yet. So, please, continue, pray over us.\r"
"I suggest you pray for love to grow, marriage to come, that His will be done, and that "
"God make hast for our help,\r"
"in the Name of Jesus Christ.\r"
"\r"
"Thanks a lot!\r"
"\r"
"God Bless You.\r"
"\r"
"Your humble brother, knut\r"
"\r"
"(Ps.118)\r"
    );
}


/**
 * Destructor stub.
 */
kBeliever::~kBeliever()
{
}


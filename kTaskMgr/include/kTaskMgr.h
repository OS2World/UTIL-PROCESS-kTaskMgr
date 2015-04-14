/* $Id: kTaskMgr.h,v 1.1 2000/05/23 00:16:51 stknut Exp $
 *
 * Definition of the kTaskMgr class.
 * Other kTaskMgr global definitions.
 *
 * Copyright (c) 2000 knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 *
 */

#ifndef _kTaskMgr_h_
#define _kTaskMgr_h_

/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
#ifdef TRACE_ALL
#   define TRACE_UPDATE
#   define TRACE_INSERT
#endif
#ifdef TRACE
#   ifdef TRACE_UPDATE
#       define TRACELOG_UPDATE(a) kTraceLog(__FUNCTION__, __FILE__, __LINE__, a)
#   else
#       define TRACELOG_UPDATE(a) ((void)0)
#   endif
#   ifdef TRACE_INSERT
#       define TRACELOG_INSERT(a) kTraceLog(__FUNCTION__, __FILE__, __LINE__, a)
#   else
#       define TRACELOG_INSERT(a) ((void)0)
#   endif
#else
#   define TRACELOG_UPDATE(a) ((void)0)
#   define TRACELOG_INSERT(a) ((void)0)
#endif




#define REFRESHRATE     5


/**
 * kTaskMgr - main class.
 * @author      knut st. osmundsen (knut.stange.osmundsen@mynd.no)
 */
class kTaskMgr : public kNotebookBase
{
private:
    HAB hab;

public:
    kTaskMgr(HAB hab) throw (kError);
    ~kTaskMgr();


    /** @cat static init method */
    static int init(int argc, char **argv);

    /** @cat more overrides */
    ULONG   showModal();

    /**@cat static public data */
    static PCSZ   pszErrorTitle;
    static PCSZ   pszInfoTitle;
};

#endif

/* $Id: kBase.h,v 1.1 2000/05/23 00:00:54 stknut Exp $
 *
 * kBase (kClassLib) - kBase class declaration and some basis stuff.
 *
 * Copyright (c) 1999-2000 knut st. osmundsen
 *
 */
#ifndef _kBase_h_
#define _kBase_h_

/**
 *
 * @description
 * @purpose
 * @assums
 * @destruct
 * @spcana
 * @invariant
 * @absfun
 * @limits
 * @maint
 * @author      knut st. osmundsen
 * @valid
 * @approval
 * @version
 * @log
 */
class kBase
{
   protected:
      /**@cat protected methods */
      /*TID beginMemberThread(void(kBase::* pfn)(void), ULONG cbStack); */
};

/* a nice hack to implement beginthread for memberfunctions */
/* put this under "internal functions" in the module you need to start threads in meberfunctions. */
#define MEMBERTHREAD(classname) \
   TID _System beginMemberThread(void(classname::* pfn)(void), ULONG cbStack, classname *pthis)


const char *getWindowMsgDescription(ULONG msg);

/*
 * Trace stuff.
 */
void kTraceLogMessage(const char *pszFile, HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
void kTraceLog(const char *pszFunction, const char *pszFile, int iLine, const char *pszMessage);

#ifdef TRACE_ALL
#   define TRACE
#   define TRACE_CONSTRUCT
#   define TRACE_DESTRUCT
#   define TRACE_MESSAGES
#endif

#ifdef TRACE
#   ifdef TRACE_CONSTRUCT
#       define TRACELOG_CONSTRUCT(a) kTraceLog(__FUNCTION__, __FILE__, __LINE__, a)
#   else
#       define TRACELOG_CONSTRUCT(a) ((void)0)
#   endif
#   ifdef TRACE_DESTRUCT
#       define TRACELOG_DESTRUCT(a) kTraceLog(__FUNCTION__, __FILE__, __LINE__, a)
#   else
#       define TRACELOG_DESTRUCT(a) ((void)0)
#   endif
#   ifdef TRACE_MESSAGES
#       define TRACELOG_MESSAGES(hwnd, msg, mp1, mp2) kTraceLogMessage(__FILE__, hwnd, msg, mp1, mp2)
#   else
#       define TRACELOG_MESSAGES(hwnd, msg, mp1, mp2) ((void)0)
#   endif
#else
#   define TRACELOG_CONSTRUCT(a) ((void)0)
#   define TRACELOG_DESTRUCT(a) ((void)0)
#   define TRACELOG_MESSAGES(hwnd, msg, mp1, mp2) ((void)0)
#endif

/*
 * KLib fake stuff.
 */
#ifndef USE_KLIB
#define ASSERT(a) ((a) ? TRUE : _assertMsg(#a, __FILE__, __FUNCTION__, __LINE__))
BOOL _assertMsg(const char *pszExpr, const char *pszFile, const char *pszFunction,
                ULONG ulLine);
void kDebugLog(char *pszFormat, ...);
#endif

#endif

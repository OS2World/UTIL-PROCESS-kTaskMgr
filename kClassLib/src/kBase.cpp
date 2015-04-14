/* $Id: kBase.cpp,v 1.1 2000/05/23 00:00:54 stknut Exp $
 *
 * kBase (kClassLib) - kBase class implementation and some basis stuff.
 *
 * Copyright (c) 1999-2000 knut st. osmundsen
 *
 */

/*******************************************************************************
*   Defined Constants And Macros                                               *
*******************************************************************************/
#define INCL_DOSERRORS
#define INCL_DOSPROCESS
#define INCL_DOSMISC
#define INCL_PM

#define USE_DOSCREATETHREAD 1


/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <os2.h>
#ifdef USE_KLIB
    #include <kAssert.h>
    #include <kLog.h>
    #include <kHeap.h>
#else
    #include <malloc.h>
#endif
#include <process.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "kBase.h"


/*******************************************************************************
*   Structures and Typedefs                                                    *
*******************************************************************************/
#ifdef USE_DOSCREATETHREAD

struct THREADPARAM
{
   void *pthis;
   void (_Optlink *pfn)(void*);
};


/*******************************************************************************
*   Internal Functions                                                         *
*******************************************************************************/
static void kTraceLogMessage_DumpSWP(PSWP pswp, char *pszBuffer, int cchBuffer);



#pragma handler( beginMemberThreadForwarder )

/**
 * Forwarder function.
 * @param     ulParam  struct THREADPARAM
 * @remark
 */
VOID _System beginMemberThreadForwarder(ULONG ulParam)
{
   struct THREADPARAM *p = (struct THREADPARAM*)ulParam;

   (*p->pfn)(p->pthis);

   free(p);
}

#endif



/**
 * A hack!
 * @returns   Thread id on success, -1 (0xffffffffL) on error
 * @param     pfn
 * @param     cbStack
 * @param     pthis
 * @remark
 */
TID _System beginMemberThread(void (_Optlink *pfn)(void *), ULONG ulDummy1, ULONG ulDummy2, ULONG ulDummy3, ULONG cbStack, void *pthis)
{
   ulDummy1 = ulDummy1;
   ulDummy2 = ulDummy2;
   ulDummy3 = ulDummy3;

   #ifdef USE_DOSCREATETHREAD
      TID    tid;
      APIRET rc;
      struct THREADPARAM *p = (struct THREADPARAM*)malloc(sizeof(struct THREADPARAM));

      p->pthis =pthis;
      p->pfn = pfn;
      rc = DosCreateThread(&tid, beginMemberThreadForwarder, (ULONG)p, CREATE_READY | STACK_COMMITTED, cbStack);
      return rc == NO_ERROR && tid != NULLHANDLE ? tid : 0xffffffffL;
   #else
      return (TID)_beginthread(pfn, NULL, (int)cbStack, pthis);
   #endif
}



/**
 * Debug - get message description.
 * @returns   Pointer to a const description string.
 * @param     msg  Message id.
 */
const char *getWindowMsgDescription(ULONG msg)
{
   #define MSGCASE(a) case a: return #a
   switch (msg)
   {
      MSGCASE(WM_NULL                  );
      MSGCASE(WM_CREATE                );
      MSGCASE(WM_DESTROY               );
      MSGCASE(WM_ENABLE                );
      MSGCASE(WM_SHOW                  );
      MSGCASE(WM_MOVE                  );
      MSGCASE(WM_SIZE                  );
      MSGCASE(WM_ADJUSTWINDOWPOS       );
      MSGCASE(WM_CALCVALIDRECTS        );
      MSGCASE(WM_SETWINDOWPARAMS       );
      MSGCASE(WM_QUERYWINDOWPARAMS     );
      MSGCASE(WM_HITTEST               );
      MSGCASE(WM_ACTIVATE              );
      MSGCASE(WM_SETFOCUS              );
      MSGCASE(WM_SETSELECTION          );
      MSGCASE(WM_PPAINT                );
      MSGCASE(WM_PSETFOCUS             );
      MSGCASE(WM_PSYSCOLORCHANGE       );
      MSGCASE(WM_PSIZE                 );
      MSGCASE(WM_PACTIVATE             );
      MSGCASE(WM_PCONTROL              );
      MSGCASE(WM_COMMAND               );
      MSGCASE(WM_SYSCOMMAND            );
      MSGCASE(WM_HELP                  );
      MSGCASE(WM_PAINT                 );
      MSGCASE(WM_TIMER                 );
      MSGCASE(WM_SEM1                  );
      MSGCASE(WM_SEM2                  );
      MSGCASE(WM_SEM3                  );
      MSGCASE(WM_SEM4                  );
      MSGCASE(WM_CLOSE                 );
      MSGCASE(WM_QUIT                  );
      MSGCASE(WM_SYSCOLORCHANGE        );
      MSGCASE(WM_SYSVALUECHANGED       );
      MSGCASE(WM_APPTERMINATENOTIFY    );
      MSGCASE(WM_PRESPARAMCHANGED      );
      MSGCASE(WM_CONTROL               );
      MSGCASE(WM_VSCROLL               );
      MSGCASE(WM_HSCROLL               );
      MSGCASE(WM_INITMENU              );
      MSGCASE(WM_MENUSELECT            );
      MSGCASE(WM_MENUEND               );
      MSGCASE(WM_DRAWITEM              );
      MSGCASE(WM_MEASUREITEM           );
      MSGCASE(WM_CONTROLPOINTER        );
      MSGCASE(WM_QUERYDLGCODE          );
      MSGCASE(WM_INITDLG               );
      MSGCASE(WM_SUBSTITUTESTRING      );
      MSGCASE(WM_MATCHMNEMONIC         );
      MSGCASE(WM_SAVEAPPLICATION       );
      MSGCASE(WM_SEMANTICEVENT         );
      MSGCASE(WM_HELPBASE              );
      MSGCASE(WM_HELPTOP               );
      MSGCASE(WM_USER                  );
      MSGCASE(WM_VRNDISABLED           );
      MSGCASE(WM_VRNENABLED            );
      MSGCASE(WM_MOUSEMOVE             );
      MSGCASE(WM_BUTTON1DOWN           );
      MSGCASE(WM_BUTTON1UP             );
      MSGCASE(WM_BUTTON1DBLCLK         );
      MSGCASE(WM_BUTTON2DOWN           );
      MSGCASE(WM_BUTTON2UP             );
      MSGCASE(WM_BUTTON2DBLCLK         );
      MSGCASE(WM_BUTTON3DOWN           );
      MSGCASE(WM_BUTTON3UP             );
      MSGCASE(WM_BUTTON3DBLCLK         );
      MSGCASE(WM_MOUSEMAP              );
      MSGCASE(WM_EXTMOUSEFIRST         );
      MSGCASE(WM_EXTMOUSELAST          );
      MSGCASE(WM_BUTTON1MOTIONSTART    );
      MSGCASE(WM_BUTTON1MOTIONEND      );
      MSGCASE(WM_BUTTON1CLICK          );
      MSGCASE(WM_BUTTON2MOTIONSTART    );
      MSGCASE(WM_BUTTON2MOTIONEND      );
      MSGCASE(WM_BUTTON2CLICK          );
      MSGCASE(WM_BUTTON3MOTIONSTART    );
      MSGCASE(WM_BUTTON3MOTIONEND      );
      MSGCASE(WM_BEGINDRAG             );
      MSGCASE(WM_ENDDRAG               );
      MSGCASE(WM_SINGLESELECT          );
      MSGCASE(WM_OPEN                  );
      MSGCASE(WM_CONTEXTMENU           );
      MSGCASE(WM_CONTEXTHELP           );
      MSGCASE(WM_TEXTEDIT              );
      MSGCASE(WM_BEGINSELECT           );
      MSGCASE(WM_ENDSELECT             );
      MSGCASE(WM_PICKUP                );
      MSGCASE(WM_PENFIRST              );
      MSGCASE(WM_PENLAST               );
      MSGCASE(WM_MMPMFIRST             );
      MSGCASE(WM_MMPMLAST              );
      MSGCASE(WM_STDDLGFIRST           );
      MSGCASE(WM_STDDLGLAST            );
      MSGCASE(WM_BIDI_FIRST            );
      MSGCASE(WM_BIDI_LAST             );
      MSGCASE(WM_CHAR                  );
      MSGCASE(WM_VIOCHAR               );
      MSGCASE(WM_JOURNALNOTIFY         );
      MSGCASE(WM_MSGBOXINIT            );
      MSGCASE(WM_MSGBOXDISMISS         );
      MSGCASE(WM_FLASHWINDOW           );
      MSGCASE(WM_FORMATFRAME           );
      MSGCASE(WM_UPDATEFRAME           );
      MSGCASE(WM_FOCUSCHANGE           );
      MSGCASE(WM_SETBORDERSIZE         );
      MSGCASE(WM_TRACKFRAME            );
      MSGCASE(WM_MINMAXFRAME           );
      MSGCASE(WM_SETICON               );
      MSGCASE(WM_QUERYICON             );
      MSGCASE(WM_SETACCELTABLE         );
      MSGCASE(WM_QUERYACCELTABLE       );
      MSGCASE(WM_TRANSLATEACCEL        );
      MSGCASE(WM_QUERYTRACKINFO        );
      MSGCASE(WM_QUERYBORDERSIZE       );
      MSGCASE(WM_NEXTMENU              );
      MSGCASE(WM_ERASEBACKGROUND       );
      MSGCASE(WM_QUERYFRAMEINFO        );
      MSGCASE(WM_QUERYFOCUSCHAIN       );
      MSGCASE(WM_OWNERPOSCHANGE        );
      MSGCASE(WM_CALCFRAMERECT         );
      MSGCASE(WM_WINDOWPOSCHANGED      );
      MSGCASE(WM_ADJUSTFRAMEPOS        );
      MSGCASE(WM_QUERYFRAMECTLCOUNT    );
      MSGCASE(WM_QUERYHELPINFO         );
      MSGCASE(WM_SETHELPINFO           );
      MSGCASE(WM_ERROR                 );
      MSGCASE(WM_REALIZEPALETTE        );
      MSGCASE(WM_CTLCOLORCHANGE        );
      MSGCASE(WM_QUERYCTLTYPE          );
      MSGCASE(WM_RENDERFMT             );
      MSGCASE(WM_RENDERALLFMTS         );
      MSGCASE(WM_DESTROYCLIPBOARD      );
      MSGCASE(WM_PAINTCLIPBOARD        );
      MSGCASE(WM_SIZECLIPBOARD         );
      MSGCASE(WM_HSCROLLCLIPBOARD      );
      MSGCASE(WM_VSCROLLCLIPBOARD      );
      MSGCASE(WM_DRAWCLIPBOARD         );
      MSGCASE(WM_DDE_INITIATE          );
      MSGCASE(WM_DDE_REQUEST           );
      MSGCASE(WM_DDE_ACK               );
      MSGCASE(WM_DDE_DATA              );
      MSGCASE(WM_DDE_ADVISE            );
      MSGCASE(WM_DDE_UNADVISE          );
      MSGCASE(WM_DDE_POKE              );
      MSGCASE(WM_DDE_EXECUTE           );
      MSGCASE(WM_DDE_TERMINATE         );
      MSGCASE(WM_DDE_INITIATEACK       );
      MSGCASE(WM_DDE_LAST              );
      MSGCASE(WM_QUERYCONVERTPOS       );
   }
   return "unknown msg";
}

/*
 * Trace logging of window messages.
 */
void kTraceLogMessage(const char *pszFile, HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    static BOOL fInUse = FALSE;
    ULONG       ulMs;
    char        szName[64];
    char        szTitle[256];
    char        szBuffer[256];
    const char *pszMsg = getWindowMsgDescription(msg);

    if (fInUse)
        return;
    fInUse = TRUE;

    /*
     * Get class name and title
     */
    szName[0] = szTitle[0] = '\0';
    WinQueryClassName(hwnd, sizeof(szName), &szName[0]);
    WinQueryWindowText(hwnd, sizeof(szTitle), &szTitle[0]);

    /*
     * Message specific info.
     */
    szBuffer[0] = '\0';
    PSWP pswp;
    switch (msg)
    {
        case WM_WINDOWPOSCHANGED:
            if (LONGFROMMP(mp2) & AWP_MINIMIZED )     strcat(szBuffer, " AWP_MINIMIZED");
            if (LONGFROMMP(mp2) & AWP_MAXIMIZED )     strcat(szBuffer, " AWP_MAXIMIZED");
            if (LONGFROMMP(mp2) & AWP_RESTORED  )     strcat(szBuffer, " AWP_RESTORED");
            if (LONGFROMMP(mp2) & AWP_ACTIVATE  )     strcat(szBuffer, " AWP_ACTIVATE");
            if (LONGFROMMP(mp2) & AWP_DEACTIVATE)     strcat(szBuffer, " AWP_DEACTIVATE");
            pswp = (PSWP)mp1;
            kTraceLogMessage_DumpSWP(pswp, szBuffer, sizeof(szBuffer));
            strcat(szBuffer, " -> ");
            kTraceLogMessage_DumpSWP(pswp + 1, szBuffer, sizeof(szBuffer));
            break;

        case WM_ADJUSTFRAMEPOS:
            kTraceLogMessage_DumpSWP((PSWP)mp1, szBuffer, sizeof(szBuffer));
            break;

        case WM_ADJUSTWINDOWPOS:
            kTraceLogMessage_DumpSWP((PSWP)mp1, szBuffer, sizeof(szBuffer));
            break;

        case WM_FOCUSCHANGE:
            strcpy(szBuffer, SHORT1FROMMP(mp2) ? "GetFocus " : "LoseFocus ");
            if (SHORT2FROMMP(mp2) & FC_NOSETFOCUS)
                strcat(szBuffer, "FC_NOSETFOCUS ");
            if (SHORT2FROMMP(mp2) & FC_NOLOSEFOCUS)
                strcat(szBuffer, "FC_NOLOSEFOCUS ");
            if (SHORT2FROMMP(mp2) & FC_NOSETACTIVE)
                strcat(szBuffer, "FC_NOSETACTIVE ");
            if (SHORT2FROMMP(mp2) & FC_NOLOSEACTIVE)
                strcat(szBuffer, "FC_NOLOSEACTIVE ");
            if (SHORT2FROMMP(mp2) & FC_NOSETSELECTION)
                strcat(szBuffer, "FC_NOSETSELECTION ");
            if (SHORT2FROMMP(mp2) & FC_NOLOSESELECTION)
                strcat(szBuffer, "FC_NOLOSESELECTION ");
            break;
    }

    /*
     * Get timestamp and write message.
     */
    DosQuerySysInfo(QSV_MS_COUNT, QSV_MS_COUNT, &ulMs, sizeof(ulMs));

    fprintf(stderr,
            "%08d - %s: %21s %08x %08x %08x %s %s %s\n",
            ulMs,
            pszFile,
            pszMsg,
            hwnd,
            mp1,
            mp2,
            szTitle,
            szName,
            szBuffer);
    fflush(stderr);

    fInUse = FALSE;
}


/**
 * Dumps the passed in SWP structure to the text buffer.
 * @param       pswp        Pointer to SWP struct.
 * @param       pszBuffer   Buffer - supposed to contain string data!
 * @param       cchBuffer   Buffer size.
 */
void kTraceLogMessage_DumpSWP(PSWP pswp, char *pszBuffer, int cchBuffer)
{
    if (pswp == NULL) return;

    cchBuffer -= strlen(pszBuffer);
    pszBuffer += strlen(pszBuffer);
    sprintf(pszBuffer,
            "x=%d, y=%d, cx=%d, cy=%d fl=%x",
            pswp->x,
            pswp->y,
            pswp->cx,
            pswp->cy,
            pswp->fl
            );
    if (pswp->fl & SWP_SIZE           ) strcat(pszBuffer, " SWP_SIZE");
    if (pswp->fl & SWP_MOVE           ) strcat(pszBuffer, " SWP_MOVE");
    if (pswp->fl & SWP_ZORDER         ) strcat(pszBuffer, " SWP_ZORDER");
    if (pswp->fl & SWP_SHOW           ) strcat(pszBuffer, " SWP_SHOW");
    if (pswp->fl & SWP_HIDE           ) strcat(pszBuffer, " SWP_HIDE");
    if (pswp->fl & SWP_NOREDRAW       ) strcat(pszBuffer, " SWP_NOREDRAW");
    if (pswp->fl & SWP_NOADJUST       ) strcat(pszBuffer, " SWP_NOADJUST");
    if (pswp->fl & SWP_ACTIVATE       ) strcat(pszBuffer, " SWP_ACTIVATE");
    if (pswp->fl & SWP_DEACTIVATE     ) strcat(pszBuffer, " SWP_DEACTIVATE");
    if (pswp->fl & SWP_EXTSTATECHANGE ) strcat(pszBuffer, " SWP_EXTSTATECHANGE");
    if (pswp->fl & SWP_MINIMIZE       ) strcat(pszBuffer, " SWP_MINIMIZE");
    if (pswp->fl & SWP_MAXIMIZE       ) strcat(pszBuffer, " SWP_MAXIMIZE");
    if (pswp->fl & SWP_RESTORE        ) strcat(pszBuffer, " SWP_RESTORE");
    if (pswp->fl & SWP_FOCUSACTIVATE  ) strcat(pszBuffer, " SWP_FOCUSACTIVATE");
    if (pswp->fl & SWP_FOCUSDEACTIVATE) strcat(pszBuffer, " SWP_FOCUSDEACTIVATE");
    if (pswp->fl & SWP_NOAUTOCLOSE    ) strcat(pszBuffer, " SWP_NOAUTOCLOSE");
    return ;
}


/**
 * Traceing.
 */
void kTraceLog(const char *pszFunction, const char *pszFile, int iLine, const char *pszMessage)
{
    ULONG   ulMs;

    DosQuerySysInfo(QSV_MS_COUNT, QSV_MS_COUNT, &ulMs, sizeof(ulMs));
    fprintf(stderr,
            "%08d - %s(%d) - %s: %s\n",
            ulMs,
            pszFile,
            iLine,
            pszFunction,
            pszMessage
            );
    fflush(stderr);
    _heap_check();
}

/**************************/
/* KLIB replacements.     */
/**************************/
#ifndef USE_KLIB
void kDebugLog(char *pszFormat, ...)
{
   /* do nothing! */
   pszFormat = pszFormat;
}

BOOL _assertMsg(const char *pszExpr, const char *pszFile, const char *pszFunction,
                ULONG ulLine)
{
   static char szBuf[512];
   PTIB        ptib;  /*  A pointer to the address of the TIB in which the current thread is returned. */
   PPIB        ppib;  /*  The address of a pointer to the PIB in which the current process is returned. */
   TID         tid = 0;
   ULONG       ulType = 0;
   int         rc = TRUE;

   if (DosGetInfoBlocks(&ptib, &ppib) == NO_ERROR)
   {
      tid    = ptib->tib_ptib2->tib2_ultid;
      ulType = ppib->pib_ultype;
   }

   /* log assert */
   sprintf(szBuf,
           "\r\n"
           "Assertion failed - thread id %d:\r\n"
           "  File: %s\r\n"
           "  Function: %s\r\n"
           "  Lineno: %d\r\n"
           "  Expression: %s\r\n",
           tid, pszFile, pszFunction, ulLine, pszExpr
           );
   kDebugLog(szBuf);

   /* display assert message */
   if (ulType == 3)
   {
      HAB hab;
      HMQ hmq;
      hmq = WinCreateMsgQueue(hab = WinInitialize(0), 0);
      sprintf(szBuf,
              "Assertion failed - thread id %d:\r"
              "  File: %s\r"
              "  Function: %s\r"
              "  Lineno: %d\r"
              "  Expression: %s",
              tid, pszFile, pszFunction, ulLine, pszExpr
              );
      if (WinMessageBox(HWND_DESKTOP,
                        NULLHANDLE,
                        szBuf,
                        "Assert failed!",
                        0,
                        MB_ABORTRETRYIGNORE | MB_CRITICAL)
          == MBID_IGNORE)
         rc = FALSE;

      if (hmq != NULLHANDLE)
      {
         WinDestroyMsgQueue(hmq);
         WinTerminate(hab);
      }
   }
   else
   {
      DosPutMessage(0, strlen(szBuf), szBuf);
      rc = TRUE;
   }

   return rc;
}


#endif

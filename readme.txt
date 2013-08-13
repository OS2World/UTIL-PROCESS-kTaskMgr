
    kTaskMgr v0.0.3 alpha preview  -- knut st. osmundsen - 19 June 2000
    ===================================================================



Features:
---------
 o  kTaskMgr is a nice little tool which will present information on OS/2,
    processes mainly using the DosQuerySysState API.
    This currently includes information on processes, modules and open files.

 o  It will allow you to kill processes. Two different methods for killing
    processes are provided:
        1. The menuitem "Kill" will try kill the process using
           the DosKillProcess API.
        2. The menuitem "kill -9" will try use the XF86SUP.SYS
           device drivers "death" method.

 o  OS/2 Queue information. This is probably a feature which is unique
    to kTaskMgr. (It's a nice little hack....)

    You should really thank Joao Luiz Pereira Junior for encouraging me
    to add this feature! I had a lots of fun exploring the inner workings
    of OS/2 queues.

 o  "Memory map" at an early stage of development.


Features to come:
-----------------
 o  Information about shared memory and semaphores. Soon to be done.

 o  I hoped it should be possible to change priority on processes too, using
    the DosSetPrioirty API. But, this API requires the process specified to
    be a decentant of the process issuing the call. This functionality may
    though be provided later by using win32k.sys to change the priority.
    (Win32k.sys will then provide a simple IOCtl to change the priority of
    a single thread slot, not process.)


Fixed bugs
-----------
v0.0.3:

 o  Another DosQuerySysState bug in pre fixpack 13 kerneles, pFS is not
    set. Code is added to compensate for this problem.
    Thanks goes to Frank Winkler who reported this bug.


v0.0.2:

 o  Not all modules (especially dlls) are reported by DosQuerySysState
    on my fixpack 13 machine.


Known bugs:
------------
 o  kill -9 don't seem to work on my Warp Fixpack 13 machine. This
    is probably solved by updating XF86SUP.SYS

 o  Sorting don't work 100%.

 o  Periodically refreshing is only implemented for the process page(s).
    This will be fixed sometime soon I hope.

 o  ...



Notes on Toolkit 4.5 and DosQuerySysState
------------------------------------------
The qsSft_t structure should be not packed at 4 bytes.
(This structure contains a BUGBUG. All QS structures should be 4 bytes aligned.
Without the padding qsSft_t is align on 4 bytes (20 bytes). The Short padding
makes it 22 bytes, and no longer 4 bytes aligned. So, there shouldn't have been
any padding in it.)
Fix: Wrap the qsSft_t declaration in bsedos.h inside a #pragma pack(1).



Bugs/Features found in the DosQuerySysState API:
-------------------------------------------------

BUGBUG! - ctSft isn't correct, or rather Sft records are missing!!! (qsFrec_t)
    The number of qsSft_t records is always 1, no matter what the
    ctSft member says!

    This is verified! The defect is located in _qsReadMFT. When
    writing the qsSft_t records the record pointer is always
    initialized to _pCurCache in each iteration. This is probably
    correct, but _pCurCache is _NOT_ changed during the iteration.
    With the excelent result that the next Sft record overwrites
    the previous one.

    Solution: To move the call to _qsUpdatePtrs into the loopbody,
              after the Sft record is written.
              This way _pCurCache will be updated at end of each
              iteration.
    This is a problem on Aurora, Warp 4 and Warp 3.

    It seems like it's corrected in warp 4 fixpack 10, or at least it's
    working with fixpack 13 exactly as I've described it! (Probably
    JR12792 or JR12762.)


BUGBUG!  - pFSRec isn't set for pre fixpack 13 kernels.
    The data is there and it could be found by using the pShrMemRec
    pointer, knowing that the pFSRec is following the array of shared
    memory object handles.

    (Haven't spent time tracing this bug in the OS/2 kernel as it seems
    to corrected.)



FEATURE/BUGBUG: cFH isn't correct - it don't match with the number of
    entries in the pFSRec array. (qsProc_t)

    This is verified! There is a "bug" in the _qsProcFS worker (this
    is the function which generates the SFT table pointed to by pFSRec).
    cFH is set to JFN_Length by _qsReadPtda. While _qsProcFS skips the
    entries containing 0xFFFF (unused handle placeholder).
    This way the cFH is really the maximum number of handles and the
    size of the pFSRec table has to be determined by checking pointers
    to the next structure. Thread records are by specifications
    following the FS records, pointer to these is used to do this.

    Note. JFN is not equal to the index into the SFN table due to this bug.

    This is a problem on Aurora, Warp 4 and Warp 3.

    It seems like it's corrected in warp 4 fixpack 10, or at least it's
    working with fixpack 13. (Probably JR12792 or JR12762.)


BUGBUG: When testing kTaskMgr on Warp fixpack 13 not all DLL are reported
    for some reason. This bug is currently unexplored. Probably a buffer
    problem again.



XF86SUP.SYS
-----------

Holger Veits' helper driver for the XFree86/OS2.
http://set.gmd.de/~veit/os2/xf86sup.html



Source code, Web site, CVS tree, Project...
--------------------------------------------

The kTaskMgr is hosted at SourceForge (http://www.sourceforge.net).

Project Website: (not ready)
    http://kTaskMgr.sourceforge.net/

Project URL:
    http://sourceforge.net/project/?group_id=4988

Project FTP:
    ftp://kTaskMgr.sourceforge.net/pub/kTaskMgr

Anonymous CVS tree:
    CVSROOT=:pserver:anonymous@cvs.kTaskMgr.sourceforge.net:/cvsroot/kTaskMgr
    or
    cvs -d:pserver:anonymous@cvs.kTaskMgr.sourceforge.net:/cvsroot/kTaskMgr login
        (no password, just press enter)
    cvs -d:pserver:anonymous@cvs.kTaskMgr.sourceforge.net:/cvsroot/kTaskMgr co .



------


Have fun,
    knut st. osmundsen (knut.stange.osmundsen@mynd.no)


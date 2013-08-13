# $Id: makefile,v 1.17 2000/06/19 20:08:42 stknut Exp $
#
# kTaskMgr makefile.
#
# Copyright (c) 2000 knut st. osmundsen
#


#
# kTaskMgr Version.
#
#CVER  = -DVER=\"0.0.3 alpha\"
RCVER = -D TITLE="kTaskMgr 0.0.3 alpha"

#
# Tools and flags.
#
CC          = icc
LD          = ilink
RC          = rc
RCFLAGS     = -n -r -x2 $(RCVER) -i .\include -i ..\kClassLib\include
RCLFLAGS    = -n
CINCLUDES   = -I.\include -I..\kClassLib\include
!ifdef DEBUG
CDEFINES    = $(CVER) -DDEBUG
CFLAGS      = -Q+ -Ti+ -Tm+ -Gm+ -Gh+ -Wall+ppt-ppc-inl-cnv-gnr-vft-ext-eff-
LFLAGS_PM   = -pmtype:pm -Map -de -noe -dbgpack
MSG         = debug
!else
CDEFINES    = $(CVER)
CFLAGS      = -Q+ -Gm+ +G4 -O+        -Wall+ppt-ppc-inl-cnv-gnr-vft-ext-eff- # -Wall+ppt-ppc-inl-cnv-gnr-vft-gen-uni-ext-
LFLAGS_PM   = -pmtype:pm -Map -exepack:2
MSG         = release
!endif

# Tracing
!if 0
CDEFINES  = $(CDEFINES) -DTRACE_ALL
!endif
!if 0
CDEFINES  = $(CDEFINES) -DTRACE -DTRACE_MESSAGES
!endif

# Profiling
!if 0
LFLAGS_PM = $(LFLAGS_PM)
CFLAGS    = $(CFLAGS) -Gh+
!endif

# K-Lib
!if 0
LFLAGS_PM = $(LFLAGS_PM)
LIBS      = $(LIBS) h:\coding\mywork\klib\klib.lib
CFLAGS    = $(CFLAGS) -DUSE_KLIB=1 -DDEBUG_ALLOC -Ih:\coding\mywork\klib\include
!endif


#
# Libraries.
#
LIBS    = ..\kClassLib\kClassLib.lib os2386.lib cppos30.lib


# Object directory.
OBJDIR  = .\object

#
# Object files.
#
OBJS    =\
$(OBJDIR)\kOS2Info.obj \
$(OBJDIR)\kQuerySysState.obj \
$(OBJDIR)\kQueueHack.obj \
\
$(OBJDIR)\kBeliever.obj \
$(OBJDIR)\kAboutPage.obj \
$(OBJDIR)\kMemMapPage.obj \
$(OBJDIR)\kModulePage.obj \
$(OBJDIR)\kProcessPage.obj \
$(OBJDIR)\kProcessDetailsPage.obj \
$(OBJDIR)\kQueuePage.obj \
$(OBJDIR)\kQueueOpenPage.obj \
$(OBJDIR)\kSFNPage.obj \
$(OBJDIR)\kSFNDetailsPage.obj \
$(OBJDIR)\kThreadPage.obj \
\
$(OBJDIR)\kDetailsRecord.obj \
\
$(OBJDIR)\kObjectRecord.obj \
$(OBJDIR)\kObjectContainer.obj \
\
$(OBJDIR)\kMemMapRecord.obj \
$(OBJDIR)\kMemMapContainer.obj \
\
$(OBJDIR)\kModuleRecord.obj \
$(OBJDIR)\kModuleContainer.obj \
\
$(OBJDIR)\kProcessRecord.obj \
$(OBJDIR)\kProcessContainer.obj \
$(OBJDIR)\kProcessDetailsContainer.obj \
\
$(OBJDIR)\kQueueRecord.obj \
$(OBJDIR)\kQueueContainer.obj \
\
$(OBJDIR)\kQueueOpenRecord.obj \
$(OBJDIR)\kQueueOpenContainer.obj \
\
$(OBJDIR)\kSFNRecord.obj \
$(OBJDIR)\kSFNContainer.obj \
$(OBJDIR)\kSFNDetailsContainer.obj \
\
$(OBJDIR)\kThreadRecord.obj \
$(OBJDIR)\kThreadContainer.obj \
\
$(OBJDIR)\kDetailsBase.obj \
$(OBJDIR)\kModuleDetails.obj \
$(OBJDIR)\kProcessDetails.obj \
$(OBJDIR)\kProcessPriority.obj \
$(OBJDIR)\kSFNDetails.obj \
\
$(OBJDIR)\kContainerPageBase.obj \
$(OBJDIR)\kTaskMgrRecordBase.obj \
$(OBJDIR)\kTaskMgrContainerBase.obj \
$(OBJDIR)\kTaskMgr.obj



#
# Interference rules.
#
{src}.cpp.obj:
   @echo compiling - $(MSG): $<
   @$(CC) -c $(CFLAGS) $(CINCLUDES) $(CDEFINES) -Fo$(OBJDIR)\$(@B).obj $<

{src}.c.obj:
   @echo compiling - $(MSG): $<
   @$(CC) -c $(CFLAGS) $(CINCLUDES) $(CDEFINES) -Fo$(OBJDIR)\$(@B).obj $<

.rc{$(OBJDIR)}.res:
   @$(RC) $(RCFLAGS) $< $@


#
# All rule.
#
all:    kTaskMgr.exe

# create kTaskMgr.exe
kTaskMgr.exe: \
        $(OBJS) \
        $(OBJDIR)\kTaskMgr.res \
        $(OBJDIR)\kTaskMgr.lnk \
        ..\kClassLib\kClassLib.lib \
        kTaskMgr.def
   @echo linking: $@
   $(LD) $(LFLAGS_PM) @$(OBJDIR)\kTaskMgr.lnk
   @echo linking resoureces....
   @$(RC) $(RCLFLAGS) $(OBJDIR)\kTaskMgr.res $@

# create linker file.
$(OBJDIR)\kTaskMgr.lnk: makefile
    echo @<<$@
/OUT:$(*B).exe
/MAP:$(OBJDIR)\$(*B).map
$(OBJS: =^
)
$(*B).def
$(LIBS)
<<KEEP

# kClassLib rule.
..\kClassLib\kClassLib.lib:
    cd ..\kClassLib
    $(MAKE) -nologo kClassLib.lib
    cd $(MAKEDIR)

# clean rule.
clean:
   -@del $(OBJDIR)\*.obj
   -@del $(OBJDIR)\*.res
   -@del $(OBJDIR)\*.map
   -@del $(OBJDIR)\*.lnk
   -@del kTaskMgr.exe


# create dependencies.
dep:
   fastdep -I. $(CINCLUDES) -o$$(OBJDIR) src\* include\* *.rc *.dlg

# dependencies
!if [..\tools\Exists.cmd .depend] == 0
!   include .depend
!else
!   if [echo .depend doesn't exist]
!   endif
!endif

# create object directory
!if [mkdir $(OBJDIR) > nul 2>&1]
!endif


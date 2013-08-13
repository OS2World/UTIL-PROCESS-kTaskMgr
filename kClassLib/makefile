# $Id: makefile,v 1.4 2000/05/23 13:32:13 stknut Exp $
#
# kClassLib makefile.
#
# Copyright (c) 2000 knut st. osmundsen
#



#
# Tools and flags.
#
AR        =  ilib
ARFLAGS   =  /nobackup /nologo
CC        =  icc
CINCLUDES =  -I.\include
!ifdef DEBUG
CDEFINES  =  -DDEBUG
CFLAGS    =  -Q+ -Ti+ -Tm+ -Gm+ -Gh+ -Wall+ppt-ppc-inl-cnv-gnr-vft-ext-eff-
MSG       =  debug
!else
CDEFINES  =
CFLAGS    =  -Q+ -Gm+ +G4 -O+        -Wall+ppt-ppc-inl-cnv-gnr-vft-ext-eff- # -Wall+ppt-ppc-inl-cnv-gnr-vft-gen-uni-ext-
MSG       =  release
!endif


#
# Tracing
#
!if 0
CDEFINES  = $(CDEFINES) -DTRACE_ALL
!endif
!if 0
CDEFINES  = $(CDEFINES) -DTRACE -DTRACE_MESSAGES
!endif

#
# Profiling
#
!if 0
LFLAGS_PM = $(LFLAGS_PM)
CFLAGS    = $(CFLAGS) -Gh+
!endif

#
# K-Lib
#
!if 0
LIBS      = $(LIBS) h:\coding\mywork\klib\klib.lib
CFLAGS    = $(CFLAGS) -DUSE_KLIB=1 -DDEBUG_ALLOC -Ih:\coding\mywork\klib\include
!endif


#
# Object directory
#
OBJDIR = .\object


#
# Object files.
#
OBJS =\
$(OBJDIR)\kBase.obj\
$(OBJDIR)\kDlgBase.obj\
$(OBJDIR)\kMenuBase.obj\
$(OBJDIR)\kNotebookBase.obj\
$(OBJDIR)\kNotebookPageBase.obj\
$(OBJDIR)\kContainer.obj\
$(OBJDIR)\kClickDlg.obj\
$(OBJDIR)\kError.obj


#
# Interference rules
#
{src}.cpp.obj:
    @echo compiling - $(MSG): $<
    @$(CC) -c $(CFLAGS) $(CINCLUDES) $(CDEFINES) -Fo$(OBJDIR)\$(*B).obj $<

{src}.c.obj:
    @echo compiling - $(MSG): $<
    @$(CC) -c $(CFLAGS) $(CINCLUDES) $(CDEFINES) -Fo$(OBJDIR)\$(*B).obj $<



#
# All rule
#
all:    kClassLib.lib

# creates the library file.
kClassLib.lib: $(OBJS)
    -@del $@          > nul 2>&1
    $(AR) $(ARFLAGS) $@ $(OBJS: = +);


# clean rule
clean:
    -del $(OBJDIR)\*.obj
    -del kClassLib.lib


# create dependencies. (fastdep is found in Odin32)
dep:
   fastdep -I. $(CINCLUDES) -o$$(OBJDIR) src\* include\*

# dependencies (must exist!)
!if [..\tools\Exists.cmd .depend] == 0
!   include .depend
!else
!   if [echo .depend doesn't exist]
!   endif
!endif

!if [mkdir $(OBJDIR) > nul 2>&1]
!endif

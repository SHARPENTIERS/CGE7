THREAD = SINGLE

!include "C:\Borland\BCC55\INCLUDE\makeincl.inc"

###########################     Targets     #######################
all: CGE7.exe

OBJFILES = CGE7.obj vram.obj cpal.obj saveload.obj undobuf.obj \
	   strinput.obj anim.obj config.obj
RESFILES = CGE7.res
LIBFILES = MSIMG32.LIB SHLWAPI.LIB
#LIBFILES = WININET.LIB USER32.LIB

CGE7.obj: CGE7.c CGE7.h cpal.h

cpal.obj: cpal.c cpal.h

CGE7.res: CGE7.rc CGE7.h cpal.h
    $(RC) $(RCFLAGS) CGE7.rc

CGE7.exe: $(OBJFILES) $(RESFILES)
    $(CPP) $(CPPFLAGS) $(OBJFILES) $(LIBFILES)
    $(LINKER) ,CGE7.exe,,,,$(RESFILES)

###########################  Implicit Rules #######################
.c.obj:
    $(CPP) $(CPPFLAGS) -c {$? }

.cpp.obj:
    $(CPP) $(CPPFLAGS) -c {$? }

.obj.exe:
    $(CPP) $(CPPFLAGS) $<

.DEFAULT_GOAL := npipe.exe

CC=cl
CFLAGS=/nologo /O2 /TC

ifeq ($(STATIC),1)
CFLAGS+=/MT
else
CFLAGS+=/MD
endif

LD=link
LDFLAGS=/nologo /release

RC=rc
RCFLAGS=/nologo

define CVTRES
	cvtres /nologo /machine:x64 /out:$(2) $(1)
endef

DIR_GUARD=@if not exist build mkdir build

ALL_OBJECTS=build/npipe.obj
ALL_RESOURCES=build/version.obj

build/npipe.obj : src/npipe.c

$(ALL_OBJECTS) :
	$(DIR_GUARD)
	$(CC) $(CFLAGS) /Fo$@ /c $<

build/version.obj : rsrc/version.rc

$(ALL_RESOURCES) :
	$(RC) $(RCFLAGS) /fo $(basename $@).res $<
	$(call CVTRES,$(basename $@).res,$@)

npipe.exe : $(ALL_OBJECTS) $(ALL_RESOURCES)
	$(LD) $(LDFLAGS) /out:$@ $(ALL_OBJECTS) $(ALL_RESOURCES)

.PHONY: clean
clean :
	rd /q /s build
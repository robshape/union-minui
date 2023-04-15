
###########################################################

ifeq (,$(PLATFORM))
PLATFORM=$(UNION_PLATFORM)
endif

ifeq (,$(PLATFORM))
$(error please specify PLATFORM, eg. make PLATFORM=trimui)
endif
ifeq (,$(CROSS_COMPILE))
$(error missing CROSS_COMPILE for this toolchain)
endif

###########################################################

CXX:=$(CROSS_COMPILE)g++
CXXFLAGS:=-DPLATFORM_$(shell echo $(PLATFORM) | tr a-z A-Z) -O3 -fomit-frame-pointer -ffast-math -funroll-loops

ifeq (miyoomini,$(PLATFORM))
CXXFLAGS+= -marm -mtune=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7ve -O3
endif

ifeq (rg35xx,$(PLATFORM))
CXXFLAGS+= -marm -mtune=cortex-a9 -mfpu=neon-fp16 -mfloat-abi=hard -march=armv7-a -O3
endif

RESDIR:=res

CXXFLAGS+= -Wno-unknown-pragmas -Wno-format -Wno-write-strings # -Wall 
CXXFLAGS+=-DRESDIR="\"$(RESDIR)\""
LINKFLAGS+=-s
LINKFLAGS+=-lSDL -lSDL_image -lSDL_ttf

ifdef V
	CMD:=
	SUM:=@\#
else
	CMD:=@
	SUM:=@echo
endif

OUTDIR:=output

EXECUTABLE:=$(OUTDIR)/DinguxCommander

OBJS:=main.o sdlutils.o resourceManager.o fileLister.o commander.o panel.o \
      dialog.o window.o fileutils.o viewer.o keyboard.o

DEPFILES:=$(patsubst %.o,$(OUTDIR)/%.d,$(OBJS))

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(addprefix $(OUTDIR)/,$(OBJS))
	$(SUM) "  LINK    $@"
	$(CMD)$(CXX) $(LINKFLAGS) -o $@ $^

$(OUTDIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(SUM) "  CXX     $@"
	$(CMD)$(CXX) $(CXXFLAGS) -MP -MMD -MF $(@:%.o=%.d) -c $< -o $@
	@touch $@ # Force .o file to be newer than .d file.

clean:
	$(SUM) "  RM      $(OUTDIR)"
	$(CMD)rm -rf $(OUTDIR)

# Load dependency files.
-include $(DEPFILES)

# Generate dependencies that do not exist yet.
# This is only in case some .d files have been deleted;
# in normal operation this rule is never triggered.
$(DEPFILES):

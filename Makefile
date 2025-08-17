# COM1 shall be used as the default port. A different port can be specified via the command line
ifndef PORT
	PORT= COM1
endif

PLATFORM=esp32:esp32:uPesy_wroom
# was before: esp32wroverkit

# Define OS specific commands depending on platform
ifdef USE_WINDOWS_CALLS
# Windows
	RM = rmdir /Q /S $(subst /,\,$1)
	CP = copy
	ConvertPath = $(subst /,\,$1)
	MKDIR = mkdir $(subst /,\,$1) > nul 2>&1 || (exit 0)
else
# *NIX
	RM = rm -rf $1
	CP = cp
	ConvertPath = $1
	MKDIR = mkdir -p $1
endif

# Define project, SDK and build directories
PRJROOT=./src
BUILDDIR= $(PRJROOT)/../build

# Export Debugger Symbols
DEBUGMODE= --optimize-for-debug

# Define project include path
INCRAWPATH =$(PRJROOT)/
INCRAWPATH+=$(wildcard $(PRJROOT)/**/)
INCRAWPATH+=$(wildcard $(PRJROOT)/**/**/)
INCRAWPATH+=$(wildcard $(PRJROOT)/**/**/**/)
INCRAWPATHONLYPATH = $(filter %/, $(INCRAWPATH))

INCPATH = $(foreach dir,$(INCRAWPATHONLYPATH),--library $(dir))

# Add all .c and .cpp files in src dir and all subdirectories up to depth 3
CSRC=  $(wildcard src/*.c)
CSRC+= $(wildcard src/**/*.c)
CSRC+= $(wildcard src/**/**/*.c)
CSRC+= $(wildcard src/**/**/**/*.c)
CPPSRC=  $(wildcard src/*.cpp)
CPPSRC+= $(wildcard src/**/*.cpp)
CPPSRC+= $(wildcard src/**/**/*.cpp)
CPPSRC+= $(wildcard src/**/**/**/*.cpp)
SRC= $(CSRC) $(CPPSRC)

HDR=  $(wildcard src/*.h)
HDR+= $(wildcard src/**/*.h)
HDR+= $(wildcard src/**/**/*.h)
HDR+= $(wildcard src/**/**/**/*.h)
HDR+= $(wildcard src/*.hpp)
HDR+= $(wildcard src/**/*.hpp)
HDR+= $(wildcard src/**/**/*.hpp)
HDR+= $(wildcard src/**/**/**/*.hpp)

# Collect the ino file anywhere in the src directory and all subdirectories up to depth 3
INO=  $(wildcard src/*.ino)
INO+= $(wildcard src/**/*.ino)
INO+= $(wildcard src/**/**/*.ino)
INO+= $(wildcard src/**/**/**/*.ino)

# Check if there is really only a single .ino file
COUNT_INO_FILES=$(words $(INO))
ifeq ($(COUNT_INO_FILES), 1)
	TARGET=$(notdir $(INO)).elf
else
	$(error There must be exactly one .ino file, found $(COUNT_INO_FILES))
endif

all: $(BUILDDIR)/$(TARGET)

# Having all source files as dependency lets Make recognize changed files
$(BUILDDIR)/$(TARGET): $(SRC) $(HDR) $(INO) $(BUILDDIR)
	arduino-cli compile $(INO) $(DEBUGMODE) -b $(PLATFORM) $(INCPATH) --build-path $(BUILDDIR) --build-cache-path $(BUILDDIR)/cache --warnings all

$(BUILDDIR):
	@$(call MKDIR, $(BUILDDIR))

.PHONY: clean rebuild test upload

upload: all
	arduino-cli upload -p $(PORT) -b $(PLATFORM) --input-dir $(BUILDDIR) --verify

test:
	$(info SRC is $(SRC))
	@$(call MKDIR, $(BUILDDIR)/test)

rebuild: clean all

clean:
	$(call RM, $(BUILDDIR))

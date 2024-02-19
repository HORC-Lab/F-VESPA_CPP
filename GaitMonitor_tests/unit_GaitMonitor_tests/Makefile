# If you get a no rule error, make sure to super duper quadruple check your file names and paths

CC = clang++
CFLAGS = -std=c++14 -Wall
PROJDIR = ../../ #Project directory path
VPATH = $(PROJDIR) #Set the vpath to the project directory so that make checks there for source files
# Build location to drop executable
BUILDLOC = build

# Source files
SRC = GaitMonitor_unit_tests.cpp components/implementation/Comp_GaitMonitor.cpp  

# App name
APPNAME = GaitMonitor_unit_tests.exe

.PHONY: clean debug

all: $(APPNAME) $(BUILDLOC)/GaitMonitor_unit_tests.exe

$(APPNAME): $(SRC) | $(BUILDLOC)
	$(CC) $(CCFLAGS) $^ -o $(BUILDLOC)/$@ -I $(PROJDIR)

debug: CCFLAGS += -DLOG_VERBOSE_LEVEL=1
debug: $(APPNAME)

$(BUILDLOC)/GaitMonitor_unit_tests.exe: GaitMonitor_unit_tests.cpp GaitMonitor_tests/unit_GaitMonitor_tests/test_macros.h | $(BUILDLOC)
	$(CC) $< -o $@ -I $(PROJDIR)

$(BUILDLOC):
	mkdir -p $@

clean:
	rm -f $(BUILDLOC)/GaitMonitor_unit_tests.exe

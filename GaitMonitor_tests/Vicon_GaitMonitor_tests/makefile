# If you get a no rule error, make sure to super duper quadruple check your file names and paths

CC = clang++
CFLAGS = -std=c++14 -Wall

# Setting up the project directory path and vpath
PROJDIR = ../../# Project directory path
VPATH = $(PROJDIR)# Set the vpath to the project directory so that make checks there for source files
LIBDIR = $(PROJDIR)/include/Vicon/lib
LIBRARY = -L$(LIBDIR) -lViconDataStreamSDK_CPP

# Build location to drop executable
BUILDLOC = build

# Source files
SRC = Test_GaitMonitor.cpp components/implementation/Comp_GaitMonitor.cpp 

# App name
APPNAME = Test_GaitMonitor.exe

.PHONY: clean debug

all: $(BUILDLOC)/$(APPNAME) $(BUILDLOC)/ViconDataStreamSDK_CPPTest.exe

$(BUILDLOC)/$(APPNAME): $(SRC) | $(BUILDLOC)
	$(CC) $(CCFLAGS) $^ -o $@ -I $(PROJDIR)

debug: CCFLAGS += -DLOG_VERBOSE_LEVEL=1
debug: $(APPNAME)

$(BUILDLOC)/ViconDataStreamSDK_CPPTest.exe: ViconDataStreamSDK_CPPTest.cpp | $(BUILDLOC)
	$(CC) $< $(LIBRARY) -o $@  -I $(PROJDIR)
	cp $(LIBDIR)/ViconDataStreamSDK_CPP.dll $(BUILDLOC)/ViconDataStreamSDK_CPP.dll

$(BUILDLOC):
	mkdir -p $@

clean:
	rm -f $(BUILDLOC)/Test_GaitMonitor.exe $(BUILDLOC)/ViconDataStreamSDK_CPPTest.exe


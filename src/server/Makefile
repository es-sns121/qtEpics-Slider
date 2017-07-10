TOP = ../..

BIN = $(TOP)/bin

CONFIG = $(TOP)/config/config.mk

include $(CONFIG)

# Executable
EXE = $(BIN)/server

# Source files
SRC = main.cpp record.cpp alarmLimit.cpp

# Rule dependencies
DEP = pv/record.h pv/alarmLimit.h $(SRC)
 
all: server

server : $(DEP)
	@mkdir -p $(TOP)/bin
	g++ $(CPP_FLAGS) $(EPICS_INCLUDE) $(EPICS_LIBRARY) $(SRC) -o $(EXE)

.PHONY: clean
clean:
	@printf "Cleaning server binary...\n"
	@mkdir -p $(TOP)/bin
	rm -f $(BIN)/server

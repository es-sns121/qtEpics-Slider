TOP = ../..

BIN = $(TOP)/bin

CONFIG = $(TOP)/config/config.mk

include $(CONFIG)

# Executable
EXE = $(BIN)/server

# Source files
SRC = main.cpp record.cpp

# Rule dependencies
DEP = pv/record.h $(SRC)
 
all: server

server : $(DEP)
	g++ $(CPP_FLAGS) $(EPICS_INCLUDE) $(EPICS_LIBRARY) $(SRC) -o $(EXE)

.PHONY: clean
clean:
	@printf "Cleaning server binary...\n"
	rm -f $(BIN)/$(EXE)

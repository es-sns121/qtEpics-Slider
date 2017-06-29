TOP = ../..

BIN = $(TOP)/bin
OBJ = $(TOP)/obj
MOC = $(TOP)/moc

CONFIG = $(TOP)/config/config.mk

include $(CONFIG)

# Executable
EXE = $(BIN)/client

# Source files
SRC = main.cpp
viewSRC = view.cpp
mocSRC = moc_output.cpp
modelSRC = model.cpp

# Objects
OBJS = $(OBJ)/model.o $(OBJ)/view.o $(OBJ)/moc.o

# Client rule dependencies
DEP = $(SRC) $(OBJS)
 
all: client

client : $(DEP)
	@mkdir -p $(TOP)/bin
	g++ $(CPP_FLAGS) $(INC) $(LIB) $(SRC) $(OBJS) -o $(EXE)

$(OBJ)/model.o: model.h $(modelSRC)
	@mkdir -p $(TOP)/obj
	g++ $(CPP_FLAGS) $(EPICS_INCLUDE) $(EPICS_LIBRARY) -c $(modelSRC) -o $(OBJ)/model.o

$(OBJ)/view.o: $(viewSRC)
	@mkdir -p $(TOP)/obj
	g++ $(CPP_FLAGS) $(INC) $(LIB) -c $(viewSRC) -o $(OBJ)/view.o

$(OBJ)/moc.o: $(MOC)/$(mocSRC)
	g++ $(CPP_FLAGS) $(INC) $(LIB) -c $(MOC)/$(mocSRC) -o $(OBJ)/moc.o

$(MOC)/moc_output.cpp: view.h
	@mkdir -p $(TOP)/moc
	moc -o $(MOC)/moc_output.cpp view.h

.PHONY: clean
clean:
	@printf "Cleaning client binaries, objects, and moc output...\n"
	rm -f $(BIN)/$(EXE) $(OBJS) $(MOC)/*  

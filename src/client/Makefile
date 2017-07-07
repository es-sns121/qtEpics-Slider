top = ../..

bin = $(top)/bin
obj = $(top)/obj
moc = $(top)/moc

config = $(top)/config/config.mk

include $(config)

# Executable
exe = $(bin)/client

all: client

client : main.cpp $(obj)/model.o $(obj)/view.o $(obj)/moc_view.o 
	@mkdir -p $(top)/bin
	g++ $(CPP_FLAGS) $(INC) $(LIB) main.cpp $(obj)/model.o $(obj)/view.o $(obj)/moc_view.o -o $(exe)

$(obj)/model.o: model.h model.cpp
	@mkdir -p $(top)/obj
	g++ $(CPP_FLAGS) $(INC) $(LIB) -c model.cpp -o $(obj)/model.o

$(obj)/view.o: view.cpp
	@mkdir -p $(top)/obj
	g++ $(CPP_FLAGS) $(INC) $(LIB) -c view.cpp -o $(obj)/view.o

$(obj)/moc_view.o: $(moc)/moc_view.cpp 	
	g++ $(CPP_FLAGS) $(INC) $(LIB) -c $(moc)/moc_view.cpp -o $(obj)/moc_view.o

$(moc)/moc_view.cpp: view.h
	@mkdir -p $(top)/moc
	moc -o $(moc)/moc_view.cpp view.h

.PHONY: clean
clean:
	@printf "Cleaning client binary, object(s), and moc output file(s)...\n"
	@mkdir -p $(top)/bin
	@mkdir -p $(top)/obj
	@mkdir -p $(top)/moc
	rm -f $(bin)/$(exe) $(obj)/* $(moc)/*

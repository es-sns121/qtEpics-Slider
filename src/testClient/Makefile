top = ../..

bin = $(top)/bin

config = $(top)/config/config.mk

include $(config)

# Executable
exe = $(bin)/testClient

# Source files
src = main.cpp test.cpp

all: testClient

testClient : test.h $(src)
	@mkdir -p $(top)/bin
	g++ $(CPP_FLAGS) $(EPICS_INCLUDE) $(EPICS_LIBRARY) $(src) -o $(exe)

.PHONY: clean
clean:
	@printf "Cleaning testClient binary...\n"
	@mkdir -p $(top)/bin
	rm -f $(exe)

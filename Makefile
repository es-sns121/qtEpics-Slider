
all: server client

# =================================== Directories ============================================

TOP = .

BIN = $(TOP)/bin
SRC = $(TOP)/src
clientSRC = $(SRC)/client
serverSRC = $(SRC)/server
OBJ = $(TOP)/obj
MOC = $(TOP)/moc

# ======================================== Rules =============================================

server:
	@printf "\nMaking Server:\n\n"
	$(MAKE) -C $(serverSRC)
	@printf "\n"

client:
	@printf "\nMaking Client:\n\n"
	$(MAKE) -C $(clientSRC)
	@printf "\n"

.PHONY: clean
clean: 
	@printf "Cleaning binaries, objects, and moc output...\n"
	rm -f $(BIN)/* $(OBJ)/* $(MOC)/* 


all: server client

# =================================== Directories ============================================

TOP = .

SRC = $(TOP)/src
clientSRC = $(SRC)/client
serverSRC = $(SRC)/server

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
	$(MAKE) -C $(clientSRC) clean
	$(MAKE) -C $(serverSRC) clean



all: server client testClient

# =================================== Directories ============================================

top = .

src = $(top)/src
clientSRC = $(src)/client
serverSRC = $(src)/server
testClientSRC = $(src)/testClient
# ======================================== Rules =============================================

server:
	@printf "\nMaking Server:\n\n"
	$(MAKE) -C $(serverSRC)
	@printf "\n"

client:
	@printf "\nMaking Client:\n\n"
	$(MAKE) -C $(clientSRC)
	@printf "\n"

testClient:
	@printf "\nMaking testClient:\n\n"
	$(MAKE) -C $(testClientSRC)

.PHONY: clean
clean: 
	@printf "Cleaning binaries, objects, and moc output...\n"
	$(MAKE) -C $(clientSRC)     clean
	$(MAKE) -C $(serverSRC)     clean
	$(MAKE) -C $(testClientSRC) clean

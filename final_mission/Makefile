# Directorios
BIN_DIR = ./bin
SRC_DIR = ./src
INCLUDE_DIR = ./include
LIB_DIR = ./lib

all:
	#(cd $(SRC_DIR); make)
	(cd $(BIN_DIR); make)	

.PHONY: clear

clean:
	(cd $(SRC_DIR); make clean)
	(cd $(BIN_DIR); make clean)	

run:
	make
	(cd $(BIN_DIR); make run)


LIBS = -lGL -lGLU -lglut -lm -lstdc++ -lopenal -I../include -lalut

SRC_DIR = ../src

FILE = main.cpp
TARGET = main

$(TARGET): $(SRC_DIR)/$(FILE)
	$(CC) $(SRC_DIR)/$(FILE) $(SRC_DIR)/*.o $(LIBS) -o $(TARGET)

.PHONY: clean run

clean:
	-rm main
run:
	make clean
	make
	./main

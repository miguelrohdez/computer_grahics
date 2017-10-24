LIBS = -lGL -lGLU -lglut

CODIGO_FUENTE = main.cpp
TARGET = main

$(TARGET): $(CODIGO_FUENTE)
	gcc $(CODIGO_FUENTE) $(LIBS) -o $(TARGET)

.PHONY: clean run

clean:
	-rm $(TARGET)

run:
	-make
	-./main
	
$(TARGET): $(CODIGO_FUENTE)

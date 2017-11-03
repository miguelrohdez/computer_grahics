LIBS = -lGL -lGLU -lglut -lm -lstdc++
CODIGO_FUENTE = Prisma.cpp Cube.cpp Texture.cpp main.cpp #TODO
TARGET = main #TODO


$(TARGET): $(CODIGO_FUENTE)
	$(CXX) $(CODIGO_FUENTE) $(LIBS) -o $(TARGET)

.PHONY: clean run

clean:
	-rm $(TARGET)

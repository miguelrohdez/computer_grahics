LIBS = -lGL -lGLU -lglut -lm -lstdc++
CODIGO_FUENTE = Prisma.cpp Texture.cpp Color.cpp main.cpp #TODO
TARGET = main #TODO


main: $(CODIGO_FUENTE)
	$(CXX) $(CODIGO_FUENTE) $(LIBS) -o $(TARGET)

.PHONY: clean run

clean:
	-rm $(TARGET)

run:
	make
	./main

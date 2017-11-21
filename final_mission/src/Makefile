LIBS = -lGL -lGLU -lglut -lm -lstdc++ -I../include

CODIGO_FUENTE = Prisma.cpp Texture.cpp Color.cpp Camera.cpp \
				Plane.cpp Vertex.cpp TextureLoader.cpp Cylinder.cpp \
				Cone.cpp AnimationPlane.cpp KeyFrame.cpp
TARGET = objetos


main: $(CODIGO_FUENTE)
	$(CXX) -c $(CODIGO_FUENTE) $(LIBS)

.PHONY: clean

clean:
	-rm *.o

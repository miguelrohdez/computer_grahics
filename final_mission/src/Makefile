LIBS = -lGL -lGLU -lglut -lm -lstdc++ -I ../include

CODIGO_FUENTE = Prisma.cpp Texture.cpp Color.cpp Camera.cpp \
				Plane.cpp Vertex.cpp TextureLoader.cpp Cylinder.cpp \
				Cone.cpp AnimationPlane.cpp AnimationRocket.cpp KeyFrame.cpp KeyFrameReloj.cpp \
				AnimationReloj.cpp Elements.cpp AnimationFridge.cpp\
				Audio.cpp
TARGET = main


$(TARGET): $(CODIGO_FUENTE)
	$(CXX) -c $(CODIGO_FUENTE) $(LIBS)

.PHONY: clean

clean:
	-rm *.o

*.o: $(CODIGO_FUENTE)

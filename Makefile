#g++ Modeller.cpp QuadMesh.cpp QuadMesh.h CubeMesh.h VECTOR3D.h -lglut -lGL -lGLU

CC=g++

CFLAGS=-c -Wall

LDFLAGS=

LIBS=-lglut -lGL -lGLU

SOURCES=Modeller.cpp QuadMesh.cpp

OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=graphics_a1


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *o graphics_a1

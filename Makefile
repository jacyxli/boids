# Compilation Flags
OBJS = main.o vector.o boid.o flock.o util.o obstacle.o
CC = g++
CFLAGS = -Wall -g 
ifeq ($(shell uname -s), Darwin)
FRAMEWORK = -framework OpenGL -framework GLUT
else
FRAMEWORK = -lX11 -lGL -lGLU -lglut -lm
endif

all: boids

boids:	$(OBJS)
	$(CC) $(CFLAGS) -o boids $(OBJS) $(FRAMEWORK)

main.o:	main.h main.cpp 
	 $(CC) $(CFLAGS) -c main.cpp

util.o:	util.h util.cpp
	$(CC) $(CFLAGS) -c util.cpp

flock.o:	flock.h flock.cpp
	$(CC) $(CFLAGS) -c flock.cpp

boid.o:	boid.h boid.cpp
	$(CC) $(CFLAGS) -c boid.cpp

obstacle.o:	obstacle.h obstacle.cpp
	$(CC) $(CFLAGS) -c obstacle.cpp

vector.o:	vector.h vector.cpp
	$(CC) $(CFLAGS) -c vector.cpp

clean:
	rm *.o hw3 *~
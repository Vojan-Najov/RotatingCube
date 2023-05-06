NAME1 = RotatingCube
NAME2 = RotatingCubeVertexArray

CC = gcc
PKG-CONFIG = $(shell which pkg-config)
RM = rm -f

CFLAGS = -Wall -Wextra -Werror
CFLAGS_GLUT = $(shell $(PKG-CONFIG) --cflags glut)
LIBS = -lGL
LIBS_GLUT = $(shell $(PKG-CONFIG) --libs glut)

$(NAME1): rotating_cube.o
	$(CC) -o $@ $? $(LIBS) $(LIBS_GLUT)

$(NAME2): rotating_cube_vertex_array.o
	$(CC) -o $@ $? $(LIBS) $(LIBS_GLUT)

rotating_cube.o: rotating_cube.c
	$(CC) $(CFLAGS) $(CFLAGS_GLUT) -c $@ $?

rotating_cube_vertex_array.o: rotating_cube_vertex_array.c
	$(CC) $(CFLAGS) $(CFLAGS_GLUT) -c $@ $?

clean:
	$(RM) *.o
	$(RM) $(NAME1)
	$(RM) $(NAME2)

.PHONY: clean

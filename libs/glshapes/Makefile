# Makefile para Linux e macOS

PROG = glshapes
FONTES = main.c
FONTES2 = shapes.c
OBJETOS = $(FONTES:.c=.o)
OBJETOS2 = $(FONTES2:.c=.o)
CFLAGS = -g -O3 -DGL_SILENCE_DEPRECATION -Wno-deprecated # -Wall -g  # Todas as warnings, infos de debug

UNAME = $(shell uname)

ifeq ($(UNAME),Darwin)
	LDFLAGS = -framework OpenGL -framework Cocoa -framework GLUT -lm
else
	LDFLAGS = -lGL -lGLU -lglut -lm
endif

$(PROG): $(OBJETOS) libshapes.a
	$(CC) $(CFLAGS) $(OBJETOS) -o $(PROG) -L. -lshapes $(LDFLAGS)

libshapes2.so: $(OBJETOS2)
	gcc -fPIC -shared -o libshapes2.so $(OBJETOS2) $(LDFLAGS)

glshapes-shared: $(OBJETOS) libshapes2.so
	$(CC) $(OBJETOS) -o glshapes-shared -L. -lshapes2 $(LDFLAGS)

libshapes.a: $(OBJETOS2)
	ar rcs libshapes.a $(OBJETOS2)

clean:
	-@ rm -f $(OBJETOS) $(OBJETOS2) $(PROG) libshapes.a

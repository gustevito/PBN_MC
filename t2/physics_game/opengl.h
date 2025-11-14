#ifndef MYOPENGL_H
#define MYOPENGL_H

#ifdef _WIN32
#include <windows.h> // includes only in MSWindows not in UNIX
#include "gl/glut.h"
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <chipmunk.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // M_PI

#define LARGURA_JAN 1712
#define ALTURA_JAN 962

// PLAYER 1
#define XPOS_PLAYER1 (LARGURA_JAN)/6
#define YPOS_PLAYER1 (ALTURA_JAN)/2

// PLAYER 2
#define XPOS_PLAYER2 (LARGURA_JAN)/1.2
#define YPOS_PLAYER2 (ALTURA_JAN)/2

// DISK
#define XPOS_DISK (LARGURA_JAN)/2
#define YPOS_DISK (ALTURA_JAN)/2

extern float mapWidth;
extern float mapHeight;



typedef enum
{
    CIRCLE,
    BOX
} ShapeType;

// Definição dos parâmetros das funções de movimento
// (tipo do ponteiro de função)
typedef void (*bodyMotionFunc)(cpBody *body, void *data);

typedef struct
{
    GLuint tex;
    cpFloat radius;
    cpFloat width, height;
    cpShape *shape;
    ShapeType shapeType;
    bodyMotionFunc func;
} UserData;

void init(int argc, char **argv);
GLuint loadImage(char *img);

#endif

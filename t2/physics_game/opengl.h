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

#define LARGURA_JAN 712
#define ALTURA_JAN 840

#define XPOS_INICIO 400
#define YPOS_INICIO 220

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

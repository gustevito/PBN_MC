#include "opengl.h"
#include <string.h>
#include <math.h>
#include <SOIL.h>

// ************************************************************
//
// A PARTIR DESTE PONTO, O PROGRAMA NÃO DEVE SER ALTERADO
//
// A NÃO SER QUE VOCÊ SAIBA ***EXATAMENTE*** O QUE ESTÁ FAZENDO
//
// ************************************************************

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // M_PI

#define PLAYER_SPEED 20

// movement fix ##
extern int keys[256];
extern int special_keys[256];

extern cpSpace *space;
extern cpFloat timeStep;

extern int gameState;
void drawMenu();
void menuKeyboard(unsigned char key, int x, int y);

extern int score;
extern int gameOver; 

GLuint backgroundTex;

// Gravidade: usada aqui apenas para testar o liga/desliga
extern cpVect gravity;

// Está aqui apenas para o teste de movimentação via teclado
extern cpBody *playerBody;

// Opções para o debugdraw
cpSpaceDebugDrawOptions drawOptions;

// Funções presentes em main.c
extern void initCM();
extern void freeCM();
extern void restartCM();

GLuint loadImage(char *img);
void loadImages();
void drawScore();
void drawBackground();

// Funções da interface gráfica e OpenGL
void display();
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y); // movement fix ##
void arrow_keys(int key, int x, int y);
void arrow_keys_up(int key, int x, int y);  // movement fix ##
void reshape(int w, int h);
void init(int argc, char **argv);
void timer(int val);
void updateMovement(); // movement fix ##




// Funções para a Chipmunk
void eachBodyFunc(cpBody *body, void *data);
void drawBody(cpVect pos, cpFloat angle, UserData *ud);



#ifndef __FREEGLUT_H__
void glutBitmapString(void *font, char *string);
#endif

// Protótipos para o debugdraw
void DrawCircle(cpVect p, cpFloat a, cpFloat r, cpSpaceDebugColor outline, cpSpaceDebugColor fill, cpDataPointer data);
void DrawSegment(cpVect a, cpVect b, cpSpaceDebugColor color, cpDataPointer data);
void DrawFatSegment(cpVect a, cpVect b, cpFloat r, cpSpaceDebugColor outline, cpSpaceDebugColor fill, cpDataPointer data);
void DrawPolygon(int count, const cpVect *verts, cpFloat r, cpSpaceDebugColor outline, cpSpaceDebugColor fill, cpDataPointer data);
void DrawDot(cpFloat size, cpVect pos, cpSpaceDebugColor color, cpDataPointer data);

GLuint loadImage(char *img)
{
    GLuint t = SOIL_load_OGL_texture(
        img,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_MULTIPLY_ALPHA
    );

    if (!t)
    {
        printf("SOIL loading error: '%s'\n", SOIL_last_result());
        exit(1);
    }
    return t;
}

void loadImages()
{
    backgroundTex = loadImage("images/background.jpg");
    printf("Background tex id: %d\n", backgroundTex);
}

// Seria usada por debugdraw
cpSpaceDebugColor ColorForShape(cpShape *shape, cpDataPointer data)
{
    cpSpaceDebugColor cor = {1, 0, 0, 1};
    return cor;
}

void init(int argc, char **argv)
{
glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(10, 10);

    // Define o tamanho da janela gráfica do programa
    glutInitWindowSize(LARGURA_JAN, ALTURA_JAN);
    glutCreateWindow("Physics Game");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrow_keys);
    glutKeyboardFunc(menuKeyboard);
    glutTimerFunc(1, timer, 0);
    
    // movement fix ##
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialUpFunc(arrow_keys_up);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    loadImages();
    initCM();

    // Parâmetros para o debugdraw
    drawOptions.drawCircle = DrawCircle;
    drawOptions.drawSegment = DrawSegment;
    drawOptions.drawFatSegment = DrawFatSegment;
    drawOptions.drawPolygon = DrawPolygon;
    drawOptions.drawDot = DrawDot;
    drawOptions.colorForShape = ColorForShape;
    drawOptions.flags = (CP_SPACE_DEBUG_DRAW_SHAPES | CP_SPACE_DEBUG_DRAW_CONSTRAINTS | CP_SPACE_DEBUG_DRAW_COLLISION_POINTS);
}

void timer(int val)
{
    updateMovement(); // Processa movimento contínuo
    cpSpaceStep(space, timeStep);
    glutTimerFunc(1, timer, 0);
    glutPostRedisplay();
}

// Função executada para cada corpo presente no sistema
void eachBodyFunc(cpBody *body, void *data)
{
    cpVect pos = cpBodyGetPosition(body);
    cpFloat angle = cpBodyGetAngle(body);
    UserData *ud = cpBodyGetUserData(body);
    drawBody(pos, angle, ud);
    // Se há uma função de movimento associada ao corpo, chama ela
    if (ud->func != NULL && !gameOver)
        (*ud->func)(body, ud);
}

const float C80PI = 180 / M_PI;
// Desenha um corpo com a textura associada
void drawBody(cpVect pos, cpFloat angle, UserData *ud)
{
    cpFloat radius = ud->radius;
    cpFloat width = ud->width / 2;
    cpFloat height = ud->height / 2;
    if (ud->shapeType == CIRCLE)
    {
        width = height = radius;
    }
    // Aplica translação e rotação no objeto
    glPushMatrix();
    glTranslatef(pos.x, pos.y, 0);
    glRotatef(angle * C80PI, 0, 0, 1);

    glBindTexture(GL_TEXTURE_2D, ud->tex);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);

    glTexCoord2f(0, 0);
    glVertex2f(-width, -height);

    glTexCoord2f(1, 0);
    glVertex2f(width, -height);

    glTexCoord2f(1, 1);
    glVertex2f(width, height);

    glTexCoord2f(0, 1);
    glVertex2f(-width, height);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

// Desenha uma imagem retangular ocupando toda a janela
void drawBackground()
{
    glBindTexture(GL_TEXTURE_2D, backgroundTex);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);

    glTexCoord2f(0, 0);
    glVertex2f(0, 0);

    glTexCoord2f(1, 0);
    glVertex2f(LARGURA_JAN - 1, 0);

    glTexCoord2f(1, 1);
    glVertex2f(LARGURA_JAN - 1, ALTURA_JAN - 1);

    glTexCoord2f(0, 1);
    glVertex2f(0, ALTURA_JAN - 1);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}

// Escreve o score na tela
void drawScore()
{
    char strscore[30];
    sprintf(strscore, "Score: %d", score);
    // Sistema de coordenadas: Y cresce para BAIXO
    glRasterPos2i(20, 30);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, strscore);

    // Se o jogo terminou, exibe a mensagem de game over
    if (gameOver)
    {
        char strgameover[100];
        glRasterPos2i(LARGURA_JAN / 2 - 200, ALTURA_JAN / 2);
        sprintf(strgameover, "Game Over (score: %d)", score);
        glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, strgameover);
    }
}

// Desenha a tela
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (gameState == 0) {
        drawMenu();
        return;
    }

    // se gameState == 1, desenha o jogo normalmente
    glMatrixMode(GL_MODELVIEW);
    drawBackground();
    drawScore();
    cpSpaceEachBody(space, eachBodyFunc, NULL);
    cpSpaceDebugDraw(space, &drawOptions);
    glutSwapBuffers();
}


// Callback de redimensionamento da janela
void reshape(int w, int h)
{
    // Resseta a projeção
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Ajusta a viewport para ser toda a janela
    glViewport(0, 0, w, h);
    // Ajusta a janea de visualização para corresponder aos pixels
    float aspect = (float)w / (float)h;
    if (aspect > (float)LARGURA_JAN / (float)ALTURA_JAN)
    {
        // Janela mais larga que o desejado: ajusta largura
        float novaLarg = aspect * ALTURA_JAN;
        float dif = (novaLarg - LARGURA_JAN) / 2;
        glOrtho(-dif, LARGURA_JAN - 1 + dif, ALTURA_JAN - 1, 0, -1, 1);
    }
    else
    {
        // Janela mais alta que o desejado: ajusta altura
        float novaAlt = LARGURA_JAN / aspect;
        float dif = (novaAlt - ALTURA_JAN) / 2;
        glOrtho(0, LARGURA_JAN - 1, ALTURA_JAN-1+dif, -dif, -1, 1);
    }
    // Volta à matriz de transformação e visualização
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Callback de teclado
void keyboard(unsigned char key, int x, int y)
{
    keys[key] = 1; // Marca tecla como pressionada
    
    switch (key)
    {
    case 27:
        freeCM();
        exit(1);
        break;
    case 'f':
        gameOver = 1;
        break;
    case 'r':
        restartCM();
        break;
    case 'g':
        cpVect grav = cpSpaceGetGravity(space);
        if (grav.y == 0)
            grav.y = gravity.y;
        else
            grav.y = 0;
        printf("Gravity: %f\n", grav.y);
        cpSpaceSetGravity(space, grav);
        break;
    }
}

// Callback de teclado (teclas especiais)
void arrow_keys(int key, int x, int y)
{
    special_keys[key] = 1; // Marca tecla especial como pressionada
}

// Callback para quando uma tecla especial é solta
void arrow_keys_up(int key, int x, int y)
{
    special_keys[key] = 0; // Marca tecla especial como NÃO pressionada
}

void keyboardUp(unsigned char key, int x, int y)
{
    keys[key] = 0;
}

// processa movimento continuo
void updateMovement()
{
    if (gameOver || !playerBody) return;
    
    int dx = 0, dy = 0;
    
    // Verifica teclas normais
    if (keys['w'] || keys['W']) dy -= PLAYER_SPEED;
    if (keys['s'] || keys['S']) dy += PLAYER_SPEED;
    if (keys['a'] || keys['A']) dx -= PLAYER_SPEED;
    if (keys['d'] || keys['D']) dx += PLAYER_SPEED;
    
    // Verifica teclas especiais (setas)
    if (special_keys[GLUT_KEY_UP]) dy -= PLAYER_SPEED;
    if (special_keys[GLUT_KEY_DOWN]) dy += PLAYER_SPEED;
    if (special_keys[GLUT_KEY_LEFT]) dx -= PLAYER_SPEED;
    if (special_keys[GLUT_KEY_RIGHT]) dx += PLAYER_SPEED;
    
    if (dx != 0 || dy != 0)
    {
        cpVect pos = cpBodyGetPosition(playerBody);
        cpBodyApplyImpulseAtWorldPoint(playerBody, cpv(dx, dy), pos);
    }
}

//
// Funções para o debug draw da Chipmunk
//
const float PI180 = 3.1415926 / 180;
void DrawCircle(cpVect pos, cpFloat angle, cpFloat radius, cpSpaceDebugColor outline, cpSpaceDebugColor fill, cpDataPointer data)
{
    glBegin(GL_LINE_LOOP);
    float x, y, inc = 10 * PI180;
    for (float ang = 0; ang < 2 * M_PI; ang += inc)
    {
        x = pos.x + cos(ang) * radius;
        y = pos.y + sin(ang) * radius;
        glVertex2f(x, y);
    }
    glEnd();
}

void DrawSegment(cpVect a, cpVect b, cpSpaceDebugColor color, cpDataPointer data)
{
    glColor3f(1,0,0);
    glBegin(GL_LINES);
    glVertex2f(a.x, a.y);
    glVertex2f(b.x, b.y);
    glEnd();
}

void DrawFatSegment(cpVect a, cpVect b, cpFloat r, cpSpaceDebugColor outline, cpSpaceDebugColor fill, cpDataPointer data)
{
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(a.x, a.y);
    glVertex2f(b.x, b.y);
    glEnd();
}

void DrawPolygon(int count, const cpVect *verts, cpFloat r, cpSpaceDebugColor outline, cpSpaceDebugColor fill, cpDataPointer data)
{
    glBegin(GL_LINE_LOOP);
    for(int i=0; i<count; i++)
        glVertex2f(verts[i].x, verts[i].y);
    glEnd();
}

void DrawDot(cpFloat size, cpVect pos, cpSpaceDebugColor color, cpDataPointer data)
{
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex2f(pos.x, pos.y);
    glEnd();
}

// Caso ainda seja GLUT, e não FreeGLUT...
#ifndef __FREEGLUT_H__
void glutBitmapString(void *font, char *string)
{
    while (*string)
        glutBitmapCharacter(font, *string++);
}

#endif
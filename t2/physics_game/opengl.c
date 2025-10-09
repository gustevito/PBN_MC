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

#define PLAYER_SPEED 300

extern cpSpace *space;
extern cpFloat timeStep;

// Score do jogo - definido em main.c
extern int score;
extern int gameOver; // 1 se o jogo tiver acabado

// Tex id para a imagem de fundo
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
void mouse(int button, int state, int x, int y);
void arrow_keys(int key, int x, int y);
void reshape(int w, int h);
void init(int argc, char **argv);
void timer(int val);

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
    GLuint t = SOIL_load_OGL_texture(img, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_MULTIPLY_ALPHA);
    if (!t)
    {
        printf("SOIL loading error: '%s'\n", SOIL_last_result());
        exit(1);
    }
    return t;
}

void loadImages()
{
    backgroundTex = loadImage("images/background.png");
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
    glutMouseFunc(mouse);
    glutTimerFunc(1, timer, 0);

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
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Fundo de tela preto
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    drawBackground();
    drawScore();

    // Desenha e movimenta todos os corpos: jogador e alvo
    cpSpaceEachBody(space, eachBodyFunc, NULL);

    // Debugdrag: desenha as formas de colisão para debug
    // Comente caso não queria mais ver
    cpSpaceDebugDraw(space, &drawOptions);

    // Exibe a tela
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
    cpVect pos, grav;
    int dx = 0;
    int dy = 0;
    switch (key)
    {
    case 27:
        // ESC libera memória e finaliza a aplicação
        freeCM();
        exit(1);
        break;
    case 'f':
        // F gera um game over (teste)
        gameOver = 1;
        break;
    case 'r':
        // R faz restart na simulação (chama função restartCM)
        restartCM();
        break;
        // Teste: controla jogador com WASD
    case 's':
        dx = 0;
        dy = PLAYER_SPEED;
        break;
    case 'w':
        dx = 0;
        dy = -PLAYER_SPEED;
        break;
    case 'a':
        dx = -PLAYER_SPEED;
        dy = 0;
        break;
    case 'd':
        dx = PLAYER_SPEED;
        dy = 0;
        break;
    case 'g':
        // Teste: g liga e desliga a gravidade
        grav = cpSpaceGetGravity(space);
        if (grav.y == 0)
            grav.y = gravity.y;
        else
            grav.y = 0;
        printf("Gravity: %f\n", grav.y);
        cpSpaceSetGravity(space, grav);
        break;
    }
    if (dx != 0 || dy != 0)
    {
        pos = cpBodyGetPosition(playerBody);
        cpBodyApplyImpulseAtWorldPoint(playerBody, cpv(dx, dy), cpv(pos.x, pos.y));
        glutPostRedisplay();
    }
}

// Callback de mouse (se necessário)
void mouse(int button, int state, int x, int y)
{
    // printf("%d %d %d\n",button,x,y);
    // if(button == 3) {
    //     //
    // }
    // else if(button == 4) {
    //     //
    // }
    glutPostRedisplay();
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

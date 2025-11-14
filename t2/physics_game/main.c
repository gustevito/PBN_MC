#include <math.h>
#include <chipmunk.h>
#include <SOIL.h>
#include <string.h>

// Rotinas para acesso da OpenGL
#include "opengl.h"

// Funções para movimentação de objetos
void movePlayer(cpBody *body, void *data);
void moveTarget(cpBody *body, void *data);

void initCM();
void freeCM();
void restartCM();

void onGoalScored();
void loadMenuComponents();

cpShape *newLine(cpCollisionType objType, cpVect inicio, cpVect fim, cpFloat fric, cpFloat elast);
cpBody *newCircle(cpCollisionType objType, cpVect pos, cpFloat radius, cpFloat mass, char *img, bodyMotionFunc func, cpFloat fric, cpFloat elast);
cpBody *newRect(cpCollisionType objType, cpVect pos, cpFloat width, cpFloat height, cpFloat mass, char *img, bodyMotionFunc func, cpFloat fric, cpFloat elast);

// Estado atual do jogo
#define STATE_MENU 0
#define STATE_GAME 1
int gameState = STATE_MENU;

// score_playerOne do jogo
int score_playerOne = 0;
int score_playerTwo = 0;

// Flag de controle: 1 se o jogo tiver acabado
int gameOver = 0;

// movement
int keys[256] = {0};
int special_keys[256] = {0};

// cpVect e' um vetor 2D e cpv() e' uma forma rapida de inicializar ele.
cpVect gravity;

// O ambiente
cpSpace *space;

// Paredes "invisíveis" do ambiente
cpShape *leftWall, *rightWall, *topWall, *bottomWall;

cpShape *finishLineCollision;

// Tipos de objetos (para a determinação de colisões específicas)
#define PLAYERONE 1
#define PLAYERTWO 2
#define WALL 3
#define DISK 4
#define GOAL 5

cpBody *playerOne;
cpBody *playerTwo;
cpBody *disk;
cpBody *goal1;
cpBody *goal2;

// timer (desenvolvido em conjunto com IA)
int countdownTimer = 0;         // contador em frames (60 frames = 1 segundo)
int timerActive = 0;            // flag se timer está ativo
char timerMessage[50] = "";     // mensagem durante countdown
void (*timerCallback)() = NULL; // função a executar quando timer acabar

#ifdef _WIN32
// Aparentemente não é mais necessário que seja 240
#define TIMESTEP 60
#else
#define TIMESTEP 60
#endif
// Cada passo de simulação é 1/60 seg.
cpFloat timeStep = 1.0 / TIMESTEP;

cpBool collisionHandler(cpArbiter *arb, cpSpace *space, void *data)
{
    if (gameOver)
        return cpTrue;

    cpShape *a, *b;
    cpArbiterGetShapes(arb, &a, &b);

    cpShape *goalShape = NULL;
    if (cpShapeGetCollisionType(a) == GOAL)
        goalShape = a;
    else if (cpShapeGetCollisionType(b) == GOAL)
        goalShape = b;

    cpBody *goalBody = cpShapeGetBody(goalShape);

    if (goalBody == goal1)
    {
        score_playerTwo++;
        printf("GOL do Player 2! Placar: P1=%d P2=%d\n", score_playerOne, score_playerTwo);
    }
    else if (goalBody == goal2)
    {
        score_playerOne++;
        printf("GOL do Player 1! Placar: P1=%d P2=%d\n", score_playerOne, score_playerTwo);
    }

    // reseta disco no centro e players nas posicoes
    onGoalScored();
    if (score_playerOne >= 5 || score_playerTwo >= 5)
    {
        gameOver = 1;
    }

    return cpTrue;
}

// Inicializa o ambiente: é chamada por init() em opengl.c, pois necessita do contexto OpenGL para a leitura das imagens
void initCM()
{
    // movement fix ## reseta estado das teclas de movimentação
    memset(keys, 0, sizeof(keys));
    memset(special_keys, 0, sizeof(special_keys));

    gravity = cpv(0, 300);
    // Cria o universo
    space = cpSpaceNew();
    // Seta o fator de damping, isto é, de atrito do ar (10% da velocidade é perdida a cada segundo)
    cpSpaceSetDamping(space, 0.6);

    cpSpaceSetGravity(space, cpv(0, 0));

    // Adiciona 4 linhas estáticas para formarem as "paredes" do ambiente
    leftWall = newLine(WALL, cpv(0, 0), cpv(0, ALTURA_JAN), 0, 1.0);
    rightWall = newLine(WALL, cpv(LARGURA_JAN, 0), cpv(LARGURA_JAN, ALTURA_JAN), 0, 1.0);
    bottomWall = newLine(WALL, cpv(0, 0), cpv(LARGURA_JAN, 0), 0, 1.0);
    topWall = newLine(WALL, cpv(0, ALTURA_JAN), cpv(LARGURA_JAN, ALTURA_JAN), 0, 1.0);

    goal1 = newRect(GOAL, cpv(0, (ALTURA_JAN / 2)), 100, 200, 0, "images/titlemenu.png", NULL, 0, 0);
    goal2 = newRect(GOAL, cpv(LARGURA_JAN, (ALTURA_JAN / 2)), 100, 200, 0, "images/titlemenu.png", NULL, 0, 0);

    // Agora criamos um corpo...
    // Os parâmetros são:
    //   - posição: cpVect (vetor: x e y)
    //   - raio
    //   - massa
    //   - imagem a ser carregada
    //   - ponteiro para a função de movimentação (chamada a cada passo, pode ser NULL)
    //   - coeficiente de fricção
    //   - coeficiente de elasticidade

    playerOne = newRect(PLAYERONE, cpv(XPOS_PLAYER1, YPOS_PLAYER1), 40, 80, 10, "images/carv2.png", NULL, 0.1, 1);
    cpBodySetAngle(playerOne, M_PI * 0.5f);
    playerTwo = newRect(PLAYERTWO, cpv(XPOS_PLAYER2, YPOS_PLAYER2), 40, 80, 10, "images/carv2.png", NULL, 0.1, 1);
    cpBodySetAngle(playerTwo, -M_PI * 0.5f);
    disk = newCircle(DISK, cpv(XPOS_DISK, YPOS_DISK), 40, 1, "images/hockeydisk.png", NULL, 0.1, 0.9);

    cpBodySetType(goal1, CP_BODY_TYPE_STATIC);
    cpBodySetType(goal2, CP_BODY_TYPE_STATIC);


    // Tratamento de colisões: callback
    cpCollisionHandler *handler = cpSpaceAddCollisionHandler(space, DISK, GOAL);
    handler->beginFunc = collisionHandler;
}

// Libera memória ocupada por cada corpo, forma e ambiente
// Acrescente mais linhas caso necessário
void freeCM()
{
    printf("Cleaning up!\n");
    UserData *ud = cpBodyGetUserData(playerTwo);
    cpShapeFree(ud->shape);
    cpBodyFree(playerTwo);

    ud = cpBodyGetUserData(playerOne);
    cpShapeFree(ud->shape);
    cpBodyFree(playerOne);

    ud = cpBodyGetUserData(disk);
    cpShapeFree(ud->shape);
    cpBodyFree(disk);

    cpShapeFree(leftWall);
    cpShapeFree(rightWall);
    cpShapeFree(bottomWall);
    cpShapeFree(topWall);

    cpSpaceFree(space);
}

// Função chamada para reiniciar a simulação
void restartCM()
{
    score_playerOne = 0;
    score_playerTwo = 0;

    cpBodySetPosition(playerOne, cpv(XPOS_PLAYER1, YPOS_PLAYER1));
    cpBodySetAngle(playerOne, M_PI * 0.5f);
    cpBodySetPosition(playerTwo, cpv(XPOS_PLAYER2, YPOS_PLAYER2));
    cpBodySetAngle(playerTwo, -M_PI * 0.5f);
    cpBodySetPosition(disk, cpv(XPOS_DISK, YPOS_DISK));

    cpBodySetVelocity(playerOne, cpvzero);
    cpBodySetVelocity(playerTwo, cpvzero);
    cpBodySetVelocity(disk, cpvzero);

    gameOver = 0;
}




void onGoalScored()
{
    printf("Reiniciando após gol!\n");
    cpBodySetPosition(disk, cpv(XPOS_DISK, YPOS_DISK));
    cpBodySetPosition(playerOne, cpv(XPOS_PLAYER1, YPOS_PLAYER1));
    cpBodySetPosition(playerTwo, cpv(XPOS_PLAYER2, YPOS_PLAYER2));
    cpBodySetVelocity(disk, cpvzero);
}

int main(int argc, char **argv)
{
    init(argc, argv);
    loadMenuComponents();
    glutMainLoop();
    return 0;
}

// Cria e adiciona uma nova linha estática (segmento) ao ambiente
cpShape *newLine(cpCollisionType objType, cpVect inicio, cpVect fim, cpFloat fric, cpFloat elast)
{
    cpShape *aux = cpSegmentShapeNew(cpSpaceGetStaticBody(space), inicio, fim, 0);
    cpShapeSetCollisionType(aux, objType);
    cpShapeSetFriction(aux, fric);
    cpShapeSetElasticity(aux, elast);
    cpSpaceAddShape(space, aux);
    return aux;
}

// Cria e adiciona um novo corpo dinâmico, com formato circular
cpBody *newCircle(cpCollisionType objType, cpVect pos, cpFloat radius, cpFloat mass, char *img, bodyMotionFunc func, cpFloat fric, cpFloat elast)
{
    cpFloat moment = cpMomentForCircle(mass, 0, radius, cpvzero);
    cpBody *newBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));

    cpBodySetPosition(newBody, pos);

    cpShape *newShape = cpSpaceAddShape(space, cpCircleShapeNew(newBody, radius, cpvzero));
    cpShapeSetCollisionType(newShape, objType);
    cpShapeSetFriction(newShape, fric);
    cpShapeSetElasticity(newShape, elast);

    UserData *newUserData = malloc(sizeof(UserData));
    newUserData->tex = loadImage(img);
    newUserData->radius = radius;
    newUserData->shape = newShape;
    newUserData->func = func;
    newUserData->shapeType = CIRCLE;
    cpBodySetUserData(newBody, newUserData);
    printf("newCircle: loaded img %s\n", img);
    return newBody;
}

cpBody *newRect(cpCollisionType objType, cpVect pos, cpFloat width, cpFloat height, cpFloat mass, char *img, bodyMotionFunc func, cpFloat fric, cpFloat elast)
{
    cpFloat radius = 0.01;
    cpFloat moment = cpMomentForBox(mass, width, height);
    

    cpBody *newBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));

    cpBodySetPosition(newBody, pos);
    radius /= 2;
    cpShape *newShape = cpSpaceAddShape(space, cpBoxShapeNew(newBody, width, height, radius));
    cpShapeSetCollisionType(newShape, objType);
    cpShapeSetFriction(newShape, fric);
    cpShapeSetElasticity(newShape, elast);

    UserData *newUserData = malloc(sizeof(UserData));
    newUserData->tex = loadImage(img);
    newUserData->radius = radius;
    newUserData->width = width;
    newUserData->height = height;
    newUserData->shape = newShape;
    newUserData->func = func;
    newUserData->shapeType = BOX;
    cpBodySetUserData(newBody, newUserData);
    printf("newRect: loaded img %s\n", img);
    return newBody;
}

// // Exemplo de função de movimentação: move o personagem em direção ao outro corpo
// void movePlayer(cpBody *body, void *data)
// {
//     // Veja como obter e limitar a velocidade do corpo:
//     cpVect vel = cpBodyGetVelocity(body);
//     // printf("\nvel: %f %f", vel.x,vel.y);

//     // Limita o vetor em 300 unidades
//     vel = cpvclamp(vel, 300);
//     // E seta novamente a velocidade do corpo
//     cpBodySetVelocity(body, vel);

//     // Obtém a posição dos dois corpos
//     cpVect playerPos = cpBodyGetPosition(body);
//     cpVect targetPos = cpBodyGetPosition(playerTwo);

//     // Calcula um vetor do jogador ao alvo (DELTA = B - R)
//     cpVect pos = playerPos;
//     pos.x = -playerPos.x;
//     pos.y = -playerPos.y;
//     cpVect delta = cpvadd(targetPos, pos);

//     // Limita o impulso em 20 unidades
//     delta = cpvmult(cpvnormalize(delta), 20);
//     // Finalmente, aplica impulso no jogador
//     cpBodyApplyImpulseAtWorldPoint(body, delta, playerPos);
// }
#include <math.h>
#include <chipmunk.h>
#include <SOIL.h>
#include <string.h>

// Rotinas para acesso da OpenGL
#include "opengl.h"

// Funções para movimentação de objetos
void movePlayer(cpBody *body, void *data);
void moveTarget(cpBody *body, void *data);

// Prototipos
void initCM();
void freeCM();
void restartCM();

void loadMenuComponents();

cpShape *newLine(cpCollisionType objType, cpVect inicio, cpVect fim, cpFloat fric, cpFloat elast);
cpBody *newCircle(cpCollisionType objType, cpVect pos, cpFloat radius, cpFloat mass, char *img, bodyMotionFunc func, cpFloat fric, cpFloat elast);
cpBody *newRect(cpCollisionType objType, cpVect pos, cpFloat width, cpFloat height, cpFloat mass, char *img, bodyMotionFunc func, cpFloat fric, cpFloat elast);

// Estado atual do jogo
#define STATE_MENU 0
#define STATE_GAME 1
int gameState = STATE_MENU;

// Score do jogo
int score = 0;

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
#define PLAYER 1
#define TARGET 2 
#define WALL 3
#define FINISH_LINE 4

// O "alvo"
cpBody *targetBody;

// Um jogador
cpBody *playerCar;


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
    // Se o jogo já acabou, não faz nada
    if (gameOver)
        return cpTrue;

    // Teste: Incrementa o score ao bater na parede
    score++;
    printf("Score: %d\n", score);
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
    cpSpaceSetDamping(space, 0.5);

    cpSpaceSetGravity(space, cpv(0,0));

    // Adiciona 4 linhas estáticas para formarem as "paredes" do ambiente
    leftWall = newLine(WALL, cpv(0, 0), cpv(0, mapHeight), 0, 1.0);
    rightWall = newLine(WALL, cpv(mapWidth, 0), cpv(mapWidth, mapHeight), 0, 1.0);
    bottomWall = newLine(WALL, cpv(0, 0), cpv(mapWidth, 0), 0, 1.0);
    topWall = newLine(WALL, cpv(0, mapHeight), cpv(mapWidth, mapHeight), 0, 1.0);

    // finishLineCollision = newLine(FINISH_LINE, cpv(0, 850), cpv(LARGURA_JAN, 850), 0, 1.0); // finish line

    // Agora criamos um corpo...
    // Os parâmetros são:
    //   - posição: cpVect (vetor: x e y)
    //   - raio
    //   - massa
    //   - imagem a ser carregada
    //   - ponteiro para a função de movimentação (chamada a cada passo, pode ser NULL)
    //   - coeficiente de fricção
    //   - coeficiente de elasticidade

    targetBody = newRect(TARGET, cpv(200, 200), 50, 100, 1, "images/car.png", NULL, 0.2, 0.2);
    playerCar = newRect(PLAYER, cpv(XPOS_INICIO, YPOS_INICIO), 50, 100, 10, "images/carv2.png", NULL, 0.2, 0.5);



    // Se você quiser criar um corpo ESTÁTICO, isto é, que não irá se movimentar, faça:
    cpBodySetType(targetBody, CP_BODY_TYPE_STATIC);

    // Tratamento de colisões: callback
    cpCollisionHandler *handler = cpSpaceAddCollisionHandler(space, PLAYER, FINISH_LINE); 
    handler->beginFunc = collisionHandler;
}

// Exemplo: move o alvo aleatoriamente
void moveTarget(cpBody *body, void *data)
{
    cpVect vel = cpBodyGetVelocity(body);
    // printf("vel: %f %f", vel.x,vel.y);

    // Limita o vetor em 300 unidades
    vel = cpvclamp(vel, 300);
    // E seta novamente a velocidade do corpo
    cpBodySetVelocity(body, vel);

    // Sorteia um impulso entre -10 e 10, para x e y
    // Depois normaliza e limita o vetor em 50 unidades
    cpVect impulso = cpvmult(cpvnormalize(cpv(rand() % 20 - 10, rand() % 20 - 10)), 100);
    // E aplica no corpo
    cpBodyApplyImpulseAtWorldPoint(body, impulso, cpBodyGetPosition(body));
}

// Libera memória ocupada por cada corpo, forma e ambiente
// Acrescente mais linhas caso necessário
void freeCM()
{
    printf("Cleaning up!\n");
    UserData *ud = cpBodyGetUserData(targetBody);
    cpShapeFree(ud->shape);
    cpBodyFree(targetBody);

    ud = cpBodyGetUserData(playerCar);
    cpShapeFree(ud->shape);
    cpBodyFree(playerCar);

    cpShapeFree(leftWall);
    cpShapeFree(rightWall);
    cpShapeFree(bottomWall);
    cpShapeFree(topWall);

    cpSpaceFree(space);
}

// Função chamada para reiniciar a simulação
void restartCM()
{
    // movement fix ## reset de teclas
    // memset(keys, 0, sizeof(keys));
    // memset(special_keys, 0, sizeof(special_keys));
    // Escreva o código para reposicionar os personagens, ressetar o score, etc.

    // Não esqueça de ressetar a variável gameOver!
    gameOver = 0;
}

// ************************************************************
//
// A PARTIR DESTE PONTO, O PROGRAMA NÃO DEVE SER ALTERADO
//
// A NÃO SER QUE VOCÊ SAIBA ***EXATAMENTE*** O QUE ESTÁ FAZENDO
//
// ************************************************************

int main(int argc, char **argv)
{
    // Inicialização da janela gráfica
    init(argc, argv);

    loadMenuComponents();

    // Não retorna... a partir daqui, interação via teclado e mouse apenas, na janela gráfica
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
//     cpVect targetPos = cpBodyGetPosition(targetBody);

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
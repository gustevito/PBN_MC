// car_physics.c
// Física simples de carro com aceleração, rotação e drift (tecla 'C')

#include <math.h>
#include <chipmunk.h>
#include "opengl.h"

extern int keys[256];
extern int special_keys[256];
extern cpBody *playerCar;

// parâmetros básicos
static const float ENGINE_FORCE = 1600.0f;
static const float MAX_SPEED    = 600.0f;
static const float TURN_SPEED   = 0.03f;
static float steering = 0.0f;

// função principal: chamada a cada frame
void updateCarPhysics()
{
    if (!playerCar) return;

    // ângulo e vetores
    cpFloat angle = cpBodyGetAngle(playerCar);
    cpVect forward = cpv(-sin(angle), cos(angle));

    // entrada de controle
    int forwardKey  = (keys['w'] || keys['W'] || special_keys[GLUT_KEY_UP]);
    int backwardKey = (keys['s'] || keys['S'] || special_keys[GLUT_KEY_DOWN]);
    int leftKey     = (keys['a'] || keys['A'] || special_keys[GLUT_KEY_LEFT]);
    int rightKey    = (keys['d'] || keys['D'] || special_keys[GLUT_KEY_RIGHT]);

    // movimento básico (aceleração e ré)
    if (forwardKey)
        cpBodyApplyForceAtWorldPoint(playerCar, cpvmult(forward, ENGINE_FORCE), cpBodyGetPosition(playerCar));
    else if (backwardKey)
        cpBodyApplyForceAtWorldPoint(playerCar, cpvmult(forward, -ENGINE_FORCE * 0.6f), cpBodyGetPosition(playerCar));

    // rotação do carro
    if (leftKey)
        steering -= TURN_SPEED;
    else if (rightKey)
        steering += TURN_SPEED;
    else
        steering *= 0.1f;
    

    float speedFactor = cpvlength(cpBodyGetVelocity(playerCar)) / MAX_SPEED;
    cpBodySetAngle(playerCar, angle + steering * (speedFactor * 0.1));

    // limitar velocidade máxima
    cpVect vel = cpBodyGetVelocity(playerCar);
    float speed = cpvlength(vel);
    if (speed > MAX_SPEED)
        cpBodySetVelocity(playerCar, cpvmult(cpvnormalize(vel), MAX_SPEED));

    if (steering > 0.5f) steering = 0.5f;
    if (steering < -0.5f) steering = -0.5f;

}

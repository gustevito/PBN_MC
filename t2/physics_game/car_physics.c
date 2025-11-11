// car_physics.c
// Física de carro com aceleração, rotação, frenagem (Z) e boost (X)

#include <math.h>
#include <chipmunk.h>
#include "opengl.h"

extern int keys[256];
extern int special_keys[256];
extern cpBody *playerCar;

// parâmetros básicos
static const float ENGINE_FORCE = 2400.0f;
static const float MAX_SPEED = 800.0f;
static const float TURN_SPEED = 0.08f;
static const float BRAKE_FORCE = 1800.0f;
static const float BOOST_FORCE = 4800.0f;
static float steering = 0.0f;

// função auxiliar segura para normalização
static inline cpVect cpvnormalize_safe(cpVect v)
{
    float len = cpvlength(v);
    return (len > 1e-5f) ? cpvmult(v, 1.0f / len) : cpvzero;
}

// função principal: chamada a cada frame
void updateCarPhysics()
{
    if (!playerCar) return;

    // ângulo e vetores
    cpFloat angle = cpBodyGetAngle(playerCar);
    cpVect forward = cpv(sin(angle), -cos(angle));

    // entrada de controle
    int forwardKey  = (keys['w'] || keys['W'] || special_keys[GLUT_KEY_UP]);
    int backwardKey = (keys['s'] || keys['S'] || special_keys[GLUT_KEY_DOWN]);
    int leftKey     = (keys['a'] || keys['A'] || special_keys[GLUT_KEY_LEFT]);
    int rightKey    = (keys['d'] || keys['D'] || special_keys[GLUT_KEY_RIGHT]);
    int brakeKey    = (keys['z'] || keys['Z']);
    int boostKey    = (keys['x'] || keys['X']);

    cpVect pos = cpBodyGetPosition(playerCar);
    cpVect vel = cpBodyGetVelocity(playerCar);
    float speed = cpvlength(vel);

    // movimento básico
    if (forwardKey)
        cpBodyApplyForceAtWorldPoint(playerCar, cpvmult(forward, ENGINE_FORCE), pos);
    else if (backwardKey)
        cpBodyApplyForceAtWorldPoint(playerCar, cpvmult(forward, -ENGINE_FORCE * 0.6f), pos);

    // frenagem (Z)
    if (brakeKey && speed > 10.0f) {
        cpVect brake = cpvmult(cpvnormalize_safe(vel), -BRAKE_FORCE);
        cpBodyApplyForceAtWorldPoint(playerCar, brake, pos);
    }

    // boost (X)
    if (boostKey && speed < MAX_SPEED * 1.5f) {
        cpBodyApplyForceAtWorldPoint(playerCar, cpvmult(forward, BOOST_FORCE), pos);
    }

    // rotação suave proporcional à velocidade
    if (leftKey)
        steering -= TURN_SPEED;
    else if (rightKey)
        steering += TURN_SPEED;
    else
        steering *= 0.1f;

    // suavizar curva conforme a velocidade
    float speedFactor = fminf(speed / MAX_SPEED, 1.0f);
    cpBodySetAngle(playerCar, angle + steering * (speedFactor * 0.1f));

    // limitar velocidade máxima
    if (speed > MAX_SPEED * 1.8f)
        cpBodySetVelocity(playerCar, cpvmult(cpvnormalize(vel), MAX_SPEED * 1.8f));

    // limitar ângulo máximo de curva
    if (steering > 0.5f) steering = 0.5f;
    if (steering < -0.5f) steering = -0.5f;
}

// car_physics.c
// Física de carro com aceleração, rotação, frenagem (Z) e boost (X)

#include <math.h>
#include <chipmunk.h>
#include "opengl.h"

extern int keys[256];
extern int special_keys[256];
extern cpBody *playerOne;

// parâmetros básicos
static const float ENGINE_FORCE = 2400.0f;
static const float MAX_SPEED = 800.0f;
static const float TURN_SPEED = 0.08f;
static const float BRAKE_FORCE = 3600.0f;
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
    if (!playerOne)
        return;

    // ângulo e vetores
    cpFloat angle = cpBodyGetAngle(playerOne);
    cpVect forward = cpv(sin(angle), -cos(angle));

    // entrada de controle
    int forwardKey = (special_keys[GLUT_KEY_UP]);
    int backwardKey = (special_keys[GLUT_KEY_DOWN]);
    int leftKey = (special_keys[GLUT_KEY_LEFT]);
    int rightKey = (special_keys[GLUT_KEY_RIGHT]);
    int brakeKey = (keys['z'] || keys['Z']);
    int boostKey = (keys['x'] || keys['X']);

    cpVect pos = cpBodyGetPosition(playerOne);
    cpVect vel = cpBodyGetVelocity(playerOne);
    float speed = cpvlength(vel);

    // Força do motor base
    float motorForce = 0.0f;
    if (forwardKey)
    {
        motorForce = ENGINE_FORCE;
        if (boostKey)
        {
            motorForce = BOOST_FORCE; // Boost só funciona acelerando
        }
    }
    else if (backwardKey)
    {
        motorForce = -ENGINE_FORCE * 0.6f;
    }

    // Aplicar força do motor
    if (motorForce != 0.0f)
    {
        cpBodyApplyForceAtWorldPoint(playerOne, cpvmult(forward, motorForce), pos);
    }

    // // movimento básico
    // if (forwardKey)
    //     cpBodyApplyForceAtWorldPoint(playerOne, cpvmult(forward, ENGINE_FORCE), pos);
    // else if (backwardKey)
    //     cpBodyApplyForceAtWorldPoint(playerOne, cpvmult(forward, -ENGINE_FORCE * 0.6f), pos);

    // frenagem (Z)
    if (brakeKey && speed > 10.0f)
    {
        cpVect brake = cpvmult(cpvnormalize_safe(vel), -BRAKE_FORCE);
        cpBodyApplyForceAtWorldPoint(playerOne, brake, pos);
    }

    // boost (X)
    if (boostKey && speed < MAX_SPEED * 1.5f)
    {
        cpBodyApplyForceAtWorldPoint(playerOne, cpvmult(forward, BOOST_FORCE), pos);
    }

    // rotação suave proporcional à velocidade

    static const float STEERING_ACCEL = 0.15f;
    float targetSteering = 0.0f;
    if (leftKey)
        targetSteering = -1.0f;
    else if (rightKey)
        targetSteering = 1.0f;
    steering += (targetSteering - steering) * STEERING_ACCEL;


    // suavizar curva conforme a velocidade
    float speedFactor = fminf(speed / MAX_SPEED, 2.0f);
    cpBodySetAngularVelocity(playerOne, steering * speedFactor * 20.0f);
    
    // cpBodySetAngle(playerOne, angle + steering * (speedFactor * 0.3f));

    // limitar velocidade máxima
    if (speed > MAX_SPEED * 1.8f)
        cpBodySetVelocity(playerOne, cpvmult(cpvnormalize(vel), MAX_SPEED * 1.8f));

    // limitar ângulo máximo de curva
    if (steering > 0.5f)
        steering = 0.5f;
    if (steering < -0.5f)
        steering = -0.5f;
}

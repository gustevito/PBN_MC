#include <math.h>
#include <chipmunk.h>
#include "opengl.h"

extern int keys[256];
extern int special_keys[256];

extern cpBody *playerOne;
extern cpBody *playerTwo;

// parâmetros básicos
static const float ENGINE_FORCE = 2400.0f;
static const float MAX_SPEED = 800.0f;
static const float TURN_SPEED = 0.03f;
static const float BRAKE_FORCE = 3600.0f;
static const float BOOST_FORCE = 4800.0f;

static const float DRIFT_DAMPING = 0.92f;       // reduz derrapagem lateral
static const float SPEED_TURN_REDUCTION = 0.4f; // reduz curva em alta velocidade
static float steering = 0.0f;

typedef struct
{
    float steering;
} CarControlState;

static CarControlState car1 = {0};
static CarControlState car2 = {0};

// função auxiliar segura para normalização
static inline cpVect cpvnormalize_safe(cpVect v)
{
    float len = cpvlength(v);
    return (len > 1e-5f) ? cpvmult(v, 1.0f / len) : cpvzero;
}

void updateCarPhysicsForPlayer(
    cpBody *car,
    CarControlState *state,
    int forwardKey,
    int backwardKey,
    int leftKey,
    int rightKey,
    int brakeKey,
    int boostKey)

{
    if (!car)
        return;

    cpFloat angle = cpBodyGetAngle(car);
    cpVect forward = cpv(sin(angle), -cos(angle));

    cpVect pos = cpBodyGetPosition(car);
    cpVect vel = cpBodyGetVelocity(car);
    float speed = cpvlength(vel);

    cpVect right = cpv(cos(angle), sin(angle));
    float lateralVel = cpvdot(vel, right);

    float motorForce = 0.0f;
    if (forwardKey)
    {
        motorForce = ENGINE_FORCE;
        if (boostKey)
            motorForce = BOOST_FORCE;
    }
    else if (backwardKey)
    {
        motorForce = -ENGINE_FORCE * 0.6f;
    }

    if (motorForce != 0.0f)
        cpBodyApplyForceAtWorldPoint(car, cpvmult(forward, motorForce), pos);

    if (brakeKey && speed > 10.0f)
    {
        cpVect brake = cpvmult(cpvnormalize_safe(vel), -BRAKE_FORCE);
        cpBodyApplyForceAtWorldPoint(car, brake, pos);
    }

    if (boostKey && speed < MAX_SPEED * 1.5f)
        cpBodyApplyForceAtWorldPoint(car, cpvmult(forward, BOOST_FORCE), pos);

    static const float STEERING_ACCEL = 0.15f;
    float targetSteering = 0.0f;
    if (leftKey)
        targetSteering = -1.0f;
    else if (rightKey)
        targetSteering = 1.0f;

    state->steering += (targetSteering - state->steering) * STEERING_ACCEL;

    if (fabs(lateralVel) > 5.0f)
    {
        cpVect lateralDamping = cpvmult(right, -lateralVel * DRIFT_DAMPING * 20.0f);
        cpBodyApplyForceAtWorldPoint(car, lateralDamping, pos);
    }

    float speedFactor = fminf(speed / MAX_SPEED, 2.0f);
    float speedReduction = 1.0f / (1.0f + speed * SPEED_TURN_REDUCTION / MAX_SPEED);
    cpBodySetAngularVelocity(car, state->steering * speedFactor * speedReduction * 20.0f);

    if (speed > MAX_SPEED * 1.8f)
        cpBodySetVelocity(car, cpvmult(cpvnormalize(vel), MAX_SPEED * 1.8f));

    if (state->steering > 0.5f)
        state->steering = 0.5f;
    if (state->steering < -0.5f)
        state->steering = -0.5f;
}

// função principal: chamada a cada frame
void updateCarPhysics()
{
    updateCarPhysicsForPlayer(
        playerOne,
        &car1,
        keys['w'] || keys['W'],
        keys['s'] || keys['S'],
        keys['a'] || keys['A'],
        keys['d'] || keys['D'],
        keys['v'] || keys['V'],
        keys['b'] || keys['B']
    );

    updateCarPhysicsForPlayer(
        playerTwo,
        &car2,
        special_keys[GLUT_KEY_UP],
        special_keys[GLUT_KEY_DOWN],
        special_keys[GLUT_KEY_LEFT],
        special_keys[GLUT_KEY_RIGHT],
        keys['2'],
        keys['3']
    );
}

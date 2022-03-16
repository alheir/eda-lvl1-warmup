/**
 * @file OrbitalSim.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-03-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "OrbitalSim.h"
#include "OrbitalBody.h"
#include "ephemerides.h"


OrbitalSim::OrbitalSim(float timeStep,
                       int system = SOLAR,
                       bool setBlackHole = false,
                       bool tweakJupiterMass = false,
                       unsigned int asteroidsNum = 0,
                       bool partyTime = false)
{
    EphemeridesBody *systemInfo;

    switch (system)
    {
    case SOLAR:
        bodyNumCore = SOLARSYSTEM_BODYNUM;
        systemInfo = solarSystem;
        break;

    case ALPHACENTAURI:
        bodyNumCore = ALPHACENTAURISYSTEM_BODYNUM;
        systemInfo = alphaCentauriSystem;

        break;

    default:
        break;
    }
}

void OrbitalSim::update()
{
    time += timeStep;

    int i;

    for (i = 0; i < bodyNum; i++)
    {
        bodies[i].setAcceleration(raylib::Vector3Zero());
    }

    for (i = 0; i < sim->bodyNumCore; i++)
    {
        for (int j = i + 1; j < bodyNum; j++)
        {
            // Parte vectorial
            raylib::Vector3 vectorDiff = raylib::Vector3Subtract(bodies[i].getPosition(), bodies[j].getPosition);

            // Norma de distancia
            float vectorLen = raylib::Vector3Length(vectorDiff);

            // Cálculo sin factor de masa
            raylib::Vector3 partialAcceleration = raylib::Vector3Scale(vectorDiff,
                                                                       (-1.0F) * GRAVITATIONAL_CONSTANT / (vectorLen * vectorLen));

            // Aceleración de i a causa de j
            bodies[i].setAcceleration(raylib::Vector3Add(bodies[i].getAcceleration(),
                                                         raylib::Vector3Scale(partialAcceleration,
                                                                              bodies[j].getMass() / vectorLen)));

            // Aceleración de j a causa de i
            bodies[j].setAcceleration(raylib::Vector3Add(bodies[j].getAcceleration(),
                                                         raylib::Vector3Scale(partialAcceleration,
                                                                              (-1.0F) * bodies[i].getMass() / vectorLen)));
        }
    }

    for (i = 0; i < bodyNum; i++)
    {
        raylib::Vector3 velocity = raylib::Vector3Add(bodies[i].getVelocity(),
                                                      raylib::Vector3Scale(bodies[i].getAcceleration(),
                                                                           timeStep));

        bodies[i].setVelocity(velocity);

        bodies[i].setPosition(raylib::Vector3Add(bodies[i].getPosition(),
                                                 raylib::Vector3Scale(velocity,
                                                                      timeStep)));
    }
}

float OrbitalSim::getTimeStep()
{
    return timeStep;
}

float OrbitalSim::getTime()
{
    return time;
}

int OrbitalSim::getBodyNumCore()
{
    return bodyNumCore;
}

int OrbitalSim::getBodyNum()
{
    return bodyNum;
}

void OrbitalSim::setTimeStep(float timeStep)
{
    this->timeStep = timeStep;
}


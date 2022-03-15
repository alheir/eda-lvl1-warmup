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

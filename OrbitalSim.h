/**
 * @file OrbitalSim.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-03-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ORBITALSIM_H
#define ORBITALSIM_H

#include "OrbitalBody.h"

#include "raylib-cpp.hpp"

enum PLANETARY_SYSTEM
{
    SOLAR,
    ALPHACENTAURI
};

class OrbitalSim
{
public:

    /**
     * @brief Construct a new Orbital Sim object
     * 
     * @param timeStep timeStep used to do physics calculations
     * @param system see PLANETARY_SYSTEM enum
     * @param setBlackHole true or false
     * @param tweakJupiterMass true or false (only for SOLAR system)
     * @param asteroidsNum from 0 to "+inf"
     * @param partyTime = true or false, modifies asteroids colors
     */
    OrbitalSim(float timeStep,
               int system = SOLAR,
               bool setBlackHole = false,
               bool tweakJupiterMass = false,
               unsigned int asteroidsNum = 0,
               bool partyTime = false);

    void update();
    void render3D();
    void render2D();

    float getTimeStep();
    float getTime();
    int getBodyNumCore();
    int getBodyNum();
    void setTimeStep(float timeStep);
    bool getPartyTime();

private:
    float timeStep;
    float time;
    int bodyNumCore;
    int bodyNum;
    bool partyTime;
    int system;
    bool tweakJupiterMass;
    bool setBlackHole;
    unsigned int asteroidsNum;
    OrbitalBody *bodies;

    void placeAsteroid(OrbitalBody &body, float centerMass);
};

#endif

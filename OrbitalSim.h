/**
 * @file OrbitalSim.h
 * @authors Alejandro Heir, Matías Álvarez
 * @brief Clase OrbitalSim, para crear y actualizar lógicamente una simulación de sistema planetario.
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
     * @brief Construct a new OrbitalSim object
     *
     * @param daysPerSecond a whole nomber greater than 1
     * @param system see PLANETARY_SYSTEM enum
     * @param asteroidsNum from 0 to "+inf"
     * @param setBlackHole true or false
     * @param tweakJupiterMass true or false (only for SOLAR system)
     * @param partyTime true or false, modifies asteroids colors
     * @param easterEgg true or false, aligns asteroids spawn (radially)
     */
    OrbitalSim(int daysPerSecond = 100,
               int system = SOLAR,
               unsigned int asteroidsNum = 0,
               bool setBlackHole = false,
               bool tweakJupiterMass = false,
               bool partyTime = false,
               bool easterEgg = false);

    ~OrbitalSim();

    /**
     * @brief Actualiza los datos de la simulación, siendo posible redefinir los fps de referencia
     *
     * @param referenceFps
     */
    void update(float referenceFps = 0.0F);

    void render3D();
    void render2D();

    // Getters
    float getTime();
    int getBodyNumCore();
    int getBodyNum();
    int getSystem();
    OrbitalBody getOrbitalBody(int bodyNum);
    bool getTweakJupiterMassFlag();
    bool getBlackHoleFlag();

private:
    float timeStep;
    float timeMultiplier;
    float time;

    int system;

    int bodyNumCore;
    int bodyNum;
    unsigned int asteroidsNum;
    OrbitalBody *bodies;

    bool tweakJupiterMass;
    bool blackHole;
    bool partyTime;
    bool easterEgg;

    /**
     * @brief Place an asteroid in the simulation
     *
     * @param body OrbitalBody object to store asteroid data
     * @param centerMass Mass of the most massive object in the planetary system
     */
    void placeAsteroid(OrbitalBody &body, float centerMass);
};

#endif // ORBITALSIM_H

/**
 * @file orbitalSim.h
 * @authors Marc Ressl - Alejandro Heir, Matías Álvarez
 * @brief Orbital simulation. Cálculos astronómicos.
 * @version 0.1
 * @date 2022-03-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ORBITALSIM_H
#define ORBITALSIM_H

#include "raylib.h"
#include "raymath.h"

/**********************************************************************/
/**************************ARCHITECT'S CONSOLE*************************/
/**********************************************************************/

#define DAYS_PER_SECOND 100

enum PLANETARY_SYSTEMS
{
    SOLAR,
    ALPHACENTAURI
};

#define CHOSEN_SYSTEM SOLAR // see PLANTEARY_SYSTEMS enum

// Se llegó 30FPS con 50000 asteroides en Linux
#define ASTEROIDS_NUM 15000

#define TWEAK_JUPITER_MASS false // true or false
#define JUPITER_ID 5
#define TWEAK_JUPITER_MASS_FACTOR 1000 // veces de la masa de Júpiter

#define BLACK_HOLE false             // true or false
#define BLACK_HOLE_MASS_FACTOR 10000 // veces de la masa mayor del sistema

#define PARTY_TIME true // true or false
#define EASTER_EGG true // true or false

/**********************************************************************/
/**************************ARCHITECT'S CONSOLE*************************/
/**********************************************************************/

struct OrbitalBody
{
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    float mass;
    float radius;
    Color color;
};

struct OrbitalSim
{
    float timeStep;
    float time;
    int bodyNumCore;
    int bodyNum;
    OrbitalBody **bodies;
};

// Makes an orbital simulation, with a given update timestep
OrbitalSim *makeOrbitalSim(float timeStep);

// Updates a given orbital simulation
void updateOrbitalSim(OrbitalSim *sim);

// Destroys a given orbital simulation
void freeOrbitalSim(OrbitalSim *sim);

#endif

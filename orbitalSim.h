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

OrbitalSim *makeOrbitalSim(float timeStep);
void updateOrbitalSim(OrbitalSim *sim);
void freeOrbitalSim(OrbitalSim *sim);

/**********************************************************************/
/**************************ARCHITECT'S CONSOLE*************************/
/**********************************************************************/

enum PLANETARY_SYSTEMS
{
    SOLAR,
    ALPHACENTAURI
};

enum ASTEROIDS_COLORS
{
    PARTY,
    BOKE,
    MESSI,
    DEFAULT
};

#define CHOSEN_SYSTEM SOLAR // see PLANTEARY_SYSTEMS enum
#define ASTEROIDS_NUM 10000

#define TWEAK_JUPITER_MASS false // true or false
#define JUPITER_ID 5
#define TWEAK_JUPITER_MASS_FACTOR 1000 // veces de la masa de Júpiter

//#define BLACK_HOLE // comment or uncomment
#define BLACK_HOLE_MASS_FACTOR 100000 // veces de la masa mayor del sistema

#define CHOSEN_ASTEROIDS_COLORS 1 // see ASTEROIDS_COLORS enum
/**********************************************************************/
/**************************ARCHITECT'S CONSOLE*************************/
/**********************************************************************/

#endif

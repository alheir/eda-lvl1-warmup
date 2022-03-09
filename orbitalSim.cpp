/*
 * Orbital simulation
 *
 * 22.08 EDA
 * Copyright (C) 2022 Marc S. Ressl
 */

#include "orbitalSim.h"
#include "ephemerides.h"
#include <stdlib.h>

#define GRAVITATIONAL_CONSTANT 6.6743E-11F
#define ASTEROIDS_MEAN_RADIUS 4E11F

// Gets a random value between min and max
float getRandomFloat(float min, float max)
{
    return min + (max - min) * rand() / (float)RAND_MAX;
}

// Places an asteroid
//
// centerMass: mass of the most massive object in the star system
void placeAsteroid(OrbitalBody *body, float centerMass)
{
    // Logit distribution
    float x = getRandomFloat(0, 1);
    float l = logf(x) - logf(1 - x) + 1;

    // https://mathworld.wolfram.com/DiskPointPicking.html
    float r = ASTEROIDS_MEAN_RADIUS * sqrtf(fabs(l));
    float phi = getRandomFloat(0, 2 * 3.14);

    // Surprise!
    // phi = 0;

    // https://en.wikipedia.org/wiki/Circular_orbit#Velocity
    float v = sqrtf(GRAVITATIONAL_CONSTANT * centerMass / r) * getRandomFloat(0.6F, 1.2F);
    float vy = getRandomFloat(-1E2F, 1E2F);

    // Fill in with your own fields:
    // body->mass = 1E12F;  // Typical asteroid weight: 1 billion tons
    // body->radius = 2E3F; // Typical asteroid radius: 2km
    // body->color = GRAY;
    // body->position = {r * cosf(phi), 0, r * sinf(phi)};
    // body->velocity = {-v * sinf(phi), vy, v * cosf(phi)};
}


// Make an orbital simulation
OrbitalSim *makeOrbitalSim(float timeStep)
{
    OrbitalBody **bodies = (OrbitalBody**)malloc(SOLARSYSTEM_BODYNUM * sizeof(OrbitalBody*));

    int i;
    for (i = 0; i < SOLARSYSTEM_BODYNUM; i++)
    {
        bodies[i] = (OrbitalBody*) malloc(sizeof(OrbitalBody));
        bodies[i]->color = solarSystem[i].color;
        bodies[i]->mass = solarSystem[i].mass;
        bodies[i]->velocity = solarSystem[i].velocity;
        bodies[i]->position = solarSystem[i].position;
        bodies[i]->radius = solarSystem[i].radius;
    }

    OrbitalSim *temp = (OrbitalSim *) malloc(sizeof(OrbitalSim));

    temp->timeStep  = timeStep;
    temp->time = 0;
    temp->bodyNum = SOLARSYSTEM_BODYNUM;
    temp->bodysArray = bodies;

    return temp;
}

// Simulates a timestep
void updateOrbitalSim(OrbitalSim *sim)
{
}

void freeOrbitalSim(OrbitalSim *sim)
{
    int i;
    for (i = 0; i < SOLARSYSTEM_BODYNUM; i++)
    {
        free(sim->bodysArray[i]);
    }
    free(sim->bodysArray);
    free(sim);
}

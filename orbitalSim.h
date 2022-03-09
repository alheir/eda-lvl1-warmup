/*
 * Orbital simulation
 *
 * 22.08 EDA
 * Copyright (C) 2022 Marc S. Ressl
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
    int bodyNum;
    OrbitalBody **bodysArray ;
};

OrbitalSim *makeOrbitalSim(float timeStep);
void updateOrbitalSim(OrbitalSim *sim);
void freeOrbitalSim(OrbitalSim *sim);

#endif

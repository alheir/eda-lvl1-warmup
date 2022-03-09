﻿/**
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
	int bodyNum;
	OrbitalBody **bodysArray;
};

OrbitalSim *makeOrbitalSim(float timeStep);
void updateOrbitalSim(OrbitalSim *sim);
void freeOrbitalSim(OrbitalSim *sim);

#endif

/**
 * @file orbitalSim.cpp
 * @authors Marc Ressl - Alejandro Heir, Matías Álvarez
 * @brief Orbital simulation. Cálculos astronómicos.
 * @version 0.1
 * @date 2022-03-09
 *
 * @copyright Copyright (c) 2022
 *
 *
 * Sobre uso de floats:
 *
 * Sobre simulación lenta y complejidad algorítmica:
 *
 * Sobre solución de lo anterior:
 *
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
	OrbitalBody **bodies = (OrbitalBody **)malloc(SOLARSYSTEM_BODYNUM * sizeof(OrbitalBody *));

	int i;
	for (i = 0; i < SOLARSYSTEM_BODYNUM; i++)
	{
		bodies[i] = (OrbitalBody *)malloc(sizeof(OrbitalBody));

		*(bodies[i]) = {solarSystem[i].position,
						solarSystem[i].velocity,
						{0, 0, 0},
						solarSystem[i].mass,
						solarSystem[i].radius,
						solarSystem[i].color};
	}

	OrbitalSim *tempOrbitalSim = (OrbitalSim *)malloc(sizeof(OrbitalSim));
	*tempOrbitalSim = {timeStep, 0, SOLARSYSTEM_BODYNUM, bodies};

	return tempOrbitalSim;
}

// Simulates a timestep
void updateOrbitalSim(OrbitalSim *sim)
{
	int i, j;

	sim->time += sim->timeStep;

	// Se pasa por cada body y se le calcula su aceleración en base a la masa de los otros.
	for (i = 0; i < sim->bodyNum; i++)
	{

		Vector3 targetBodyPosition = sim->bodysArray[i]->position;
		sim->bodysArray[i]->acceleration = {0, 0, 0};

		for (j = 0; j < i; j++)
		{

			if (j == i)
				continue;

			// Parte adimencional de fuerza gravitatoria, omitiendo la masa del targetBody
			float scalar = (-1) * GRAVITATIONAL_CONSTANT * sim->bodysArray[j]->mass *
						   (1 / pow(Vector3Length(Vector3Subtract(targetBodyPosition,
																  sim->bodysArray[j]->position)),
									3));

			// Cálculo y acumulación de aceleración
			sim->bodysArray[i]->acceleration = Vector3Add(sim->bodysArray[i]->acceleration,
														  Vector3Scale(Vector3Subtract(targetBodyPosition,
																					   sim->bodysArray[j]->position),
																	   scalar));
		}
	}

	float timeStep = sim->timeStep;

	// Se integra discretamente la aceleración, para obtener velocidad y posición
	for (i = 0; i < sim->bodyNum; i++)
	{
		sim->bodysArray[i]->velocity = Vector3Add(sim->bodysArray[i]->velocity,
												  Vector3Scale(sim->bodysArray[i]->acceleration,
															   timeStep));

		sim->bodysArray[i]->position = Vector3Add(sim->bodysArray[i]->position,
												  Vector3Scale(sim->bodysArray[i]->velocity,
															   timeStep));
	}
}

void freeOrbitalSim(OrbitalSim *sim)
{
	int i;

	for (i = 0; i < sim->bodyNum; i++)
	{
		free(sim->bodysArray[i]);
	}

	free(sim->bodysArray);
	free(sim);
}

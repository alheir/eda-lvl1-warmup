/**
 * @file orbitalSim.cpp
 * @authors Marc Ressl - Alejandro Heir, Matías Álvarez
 * @brief Orbital simulation. Cálculos astronómicos.
 * @version 0.1
 * @date 2022-03-09
 *
 * @copyright Copyright (c) 2022
 *
 * Sobre uso de floats: considerando que la masa del Sol (la mayor empleada) ronda los 1.9*10^30kg, es claro
 * 		que no se puede usar un tipo de dato entero como int o long int, cuyos rangos máximos se encuentran
 * 		claramente por debajo. Con float de 32bits, se puede llegar hasta aproximadamente 3.4*10^38, lo
 * 		cual es suficiente para representar, en este caso, masas astronómicas.
 *
 * 		Observamos que el tipo de dato Vector3 de raylib implementa vectores con float, por lo que consideramos que tendrá la presición y rango necesarios. Además, la información
 * 		proveída en ephemerides.h ya venía con masas, posiciones y velocidades con tipo de dato float.
 *
 * Sobre simulación lenta y complejidad algorítmica: la "peor" complejidad está en la lógica de
 * 		updateOrbitalSim, en donde el doble for causa una O(n^2). Se repiten cálculos "innecesarios", o más
 * 		bien, se hacen cálculos iguales y opuestos a cálculos previos.
 *
 * Sobre solución de lo anterior:
 *
 */

#include "orbitalSim.h"
#include "ephemerides.h"
#include <stdlib.h>
#include <string.h>

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
	body->mass = 1E12F;	 // Typical asteroid weight: 1 billion tons
	body->radius = 2E3F; // Typical asteroid radius: 2km
	body->color = GRAY;
	body->position = {r * cosf(phi), 0, r * sinf(phi)};
	body->velocity = {-v * sinf(phi), vy, v * cosf(phi)};
}

// Make an orbital simulation
OrbitalSim *makeOrbitalSim(float timeStep)
{
	int i;
	OrbitalSim *tempOrbitalSim = NULL;
	OrbitalBody **bodies = NULL;
	const int asteroids = 1000;
	const int bodyNum = SOLARSYSTEM_BODYNUM + asteroids;

	if (!(tempOrbitalSim = (OrbitalSim *)malloc(sizeof(OrbitalSim))))
		return NULL;
	if (!(bodies = (OrbitalBody **)malloc(bodyNum * sizeof(OrbitalBody *))))
	{
		free(tempOrbitalSim);
		return NULL;
	}

	*tempOrbitalSim = {timeStep, 0, bodyNum, bodies};

	for (i = 0; i < tempOrbitalSim->bodyNum; i++)
	{
		if (!(bodies[i] = (OrbitalBody *)malloc(sizeof(OrbitalBody))))
		{
			int j;
			for (j = 0; j < i; j++)
				free(bodies[j]);
			free(bodies);
			free(tempOrbitalSim);

			return NULL;
		}

		if (i < SOLARSYSTEM_BODYNUM)
		{
			*(bodies[i]) = {solarSystem[i].position,
							solarSystem[i].velocity,
							{0, 0, 0},
							solarSystem[i].mass,
							solarSystem[i].radius,
							solarSystem[i].color};
		}

		else
		{
			placeAsteroid(bodies[i], bodies[0]->mass);
		}
	}

	return tempOrbitalSim;
}

// Simulates a timestep
void updateOrbitalSim(OrbitalSim *sim)
{
	int i, j;

	sim->time += sim->timeStep;

	for (i = 0; i < sim->bodyNum; i++)
		sim->bodysArray[i]->acceleration = {0, 0, 0};

	// Se pasa por cada body y se le calcula su aceleración en base a la masa de los otros.
	for (i = 0; i < sim->bodyNum; i++)
	{
		for (j = i + 1; j < sim->bodyNum - 1; j++)
		{
			Vector3 vectorDiff = Vector3Subtract(sim->bodysArray[i]->position,
												 sim->bodysArray[j]->position);

			float vectorLen = Vector3Length(vectorDiff);

			double scalar = (-1) * GRAVITATIONAL_CONSTANT * sim->bodysArray[j]->mass *
						   (1 / (vectorLen * vectorLen * vectorLen));

			Vector3 vectorial = Vector3Scale(vectorDiff,
											 scalar);

			sim->bodysArray[i]->acceleration = Vector3Add(sim->bodysArray[i]->acceleration,
														  vectorial);

			sim->bodysArray[j]->acceleration = Vector3Add(sim->bodysArray[j]->acceleration,
														  Vector3Scale(Vector3Negate(vectorial),
																	   (sim->bodysArray[i]->mass/sim->bodysArray[j]->mass)));
		}
	}

	// Se integra discretamente la aceleración, para obtener velocidad y posición
	for (i = 0; i < sim->bodyNum; i++)
	{
		Vector3 velocity = Vector3Add(sim->bodysArray[i]->velocity,
									  Vector3Scale(sim->bodysArray[i]->acceleration,
												   sim->timeStep));

		sim->bodysArray[i]->velocity = velocity;

		sim->bodysArray[i]->position = Vector3Add(sim->bodysArray[i]->position,
												  Vector3Scale(velocity,
															   sim->timeStep));
	}
}

void freeOrbitalSim(OrbitalSim *sim)
{
	int i;

	for (i = 0; i < sim->bodyNum; i++)
		free(sim->bodysArray[i]);

	free(sim->bodysArray);
	free(sim);
}

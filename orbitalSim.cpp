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
 * Sobre solución de lo anterior: la O(n^2) se redujo haciendo dobles asignaciones de los cálculos en
 * 		cuestión: "si calculo para el Sol comparando con los otros cuerpos, también guardo 'los otros
 * 		cuerpos vs. el Sol', con el signo cambiado".
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
    // body->mass = 1E12F;	 // Typical asteroid weight: 1 billion tons
    // body->radius = 2E3F; // Typical asteroid radius: 2km
    body->mass = getRandomFloat(1E9F, 1E15F);
    body->radius = getRandomFloat(2E2F, 2E4F);
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
    EphemeridesBody *systemInfo;
    int systemBodyNumCore, systemBodyNum;

    switch (CHOSEN_SYSTEM)
    {
    case SOLAR:
        systemBodyNumCore = SOLARSYSTEM_BODYNUM;
        systemInfo = solarSystem;

        if (TWEAK_JUPITER_MASS)
            systemInfo[JUPITER_ID].mass *= TWEAK_JUPITER_MASS_FACTOR;

        break;

    case ALPHACENTAURI:
        systemBodyNumCore = ALPHACENTAURISYSTEM_BODYNUM;
        systemInfo = alphaCentauriSystem;
        break;

    default:
        break;
    }

    systemBodyNum = systemBodyNumCore + ASTEROIDS_NUM;

#ifdef BLACK_HOLE
    systemBodyNumCore++;
    systemBodyNum++;

    const OrbitalBody blacky = {solarSystem[0].position,
                                solarSystem[5].velocity,
                                Vector3Zero(),
                                systemInfo[0].mass * BLACK_HOLE_MASS_FACTOR,
                                systemInfo[0].radius,
                                DARKGRAY};
#endif

    if (!(tempOrbitalSim = (OrbitalSim *)malloc(sizeof(OrbitalSim))))
        return NULL;
    if (!(bodies = (OrbitalBody **)malloc(systemBodyNum * sizeof(OrbitalBody *))))
    {
        free(tempOrbitalSim);
        return NULL;
    }

    *tempOrbitalSim = {timeStep, 0, systemBodyNumCore, systemBodyNum, bodies};

    for (i = 0; i < systemBodyNum; i++)
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

        // Cuerpos principales del sistema (no asteroides)
        if (i < systemBodyNumCore)
        {

#ifdef BLACK_HOLE
            if (!i)
            {
                *(bodies[0]) = blacky;
                continue;
            }
#endif

            *(bodies[i]) = {systemInfo[i].position,
                            systemInfo[i].velocity,
                            Vector3Zero(),
                            systemInfo[i].mass,
                            systemInfo[i].radius,
                            systemInfo[i].color};
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

    /*
    for (i = 0; i < sim->bodyNum; i++)
    {

        Vector3 targetBodyPosition = sim->bodies[i]->position;
        sim->bodies[i]->acceleration = Vector3Zero();

        for (j = 0; j < i; j++)
        {

            if (j == i)
                continue;

            float vectorLen = Vector3Length(Vector3Subtract(targetBodyPosition, sim->bodies[j]->position));

            // Parte adimencional de fuerza gravitatoria, omitiendo la masa del targetBody
            float scalar = (-1) * GRAVITATIONAL_CONSTANT * sim->bodies[j]->mass *
                           (1 / (vectorLen * vectorLen));

            // Cálculo y acumulación de aceleración
            sim->bodies[i]->acceleration = Vector3Add(sim->bodies[i]->acceleration,
                                                      Vector3Scale(Vector3Subtract(targetBodyPosition,
                                                                                   sim->bodies[j]->position),
                                                                   scalar / vectorLen));
        }
    }
    */

    for (i = 0; i < sim->bodyNum; i++)
    {
        sim->bodies[i]->acceleration = Vector3Zero();
    }

    // Se pasa por cada body y se le calcula su aceleración en base a la masa de los otros.
    for (i = 0; i < sim->bodyNum; i++)
    {
        for (j = i + 1; j < sim->bodyNum; j++)
        {
            // Parte vectorial
            Vector3 vectorDiff = Vector3Subtract(sim->bodies[i]->position,
                                                 sim->bodies[j]->position);

            // Norma de distancia
            float vectorLen = Vector3Length(vectorDiff);

            // Cálculo sin factor de masa
            Vector3 partialAcceleration = Vector3Scale(vectorDiff,
                                                       (-1.0F) *
                                                           GRAVITATIONAL_CONSTANT /
                                                           (vectorLen * vectorLen));

            // Aceleración de i a causa de j
            sim->bodies[i]->acceleration = Vector3Add(sim->bodies[i]->acceleration,
                                                      Vector3Scale(partialAcceleration,
                                                                   sim->bodies[j]->mass / vectorLen));

            // Aceleración de j a causa de i
            sim->bodies[j]->acceleration = Vector3Add(sim->bodies[j]->acceleration,
                                                      Vector3Scale(partialAcceleration,
                                                                   (-1.0F) * sim->bodies[i]->mass / vectorLen));
        }
    }

    // Se integra discretamente la aceleración, para obtener velocidad y posición
    for (i = 0; i < sim->bodyNum; i++)
    {
        Vector3 velocity = Vector3Add(sim->bodies[i]->velocity,
                                      Vector3Scale(sim->bodies[i]->acceleration,
                                                   sim->timeStep));

        sim->bodies[i]->velocity = velocity;

        sim->bodies[i]->position = Vector3Add(sim->bodies[i]->position,
                                              Vector3Scale(velocity,
                                                           sim->timeStep));
    }
}

void freeOrbitalSim(OrbitalSim *sim)
{
    int i;

    for (i = 0; i < sim->bodyNum; i++)
        free(sim->bodies[i]);

    free(sim->bodies);
    free(sim);
}

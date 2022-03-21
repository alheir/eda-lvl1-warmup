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
 * 		Observamos que el tipo de dato Vector3 de raylib implementa vectores con float, por lo que consideramos
 *      que tendrá la presición y rango necesarios. Además, la información proveída en ephemerides.h ya venía
 *      con masas, posiciones y velocidades con tipo de dato float.
 *
 * Sobre simulación lenta y complejidad algorítmica: la "peor" complejidad está en la lógica de
 * 		updateOrbitalSim, en donde el doble for causa una O(n^2). Se repiten cálculos "innecesarios", o más
 * 		bien, se hacen cálculos iguales y opuestos a cálculos previos. Adicionalmente, creemos que también
 *      puede haber una complicación al renderizar tantas esferas ("DrawSphere()") y puntos.
 *
 * Sobre solución de lo anterior: Primero, optimizamos el funcionamiento haciendo dobles asignaciones de los
 *      cálculos: "si calculo para el Sol comparando con los otros cuerpos, también guardo 'los otros cuerpos
 *      vs. el Sol', con el signo cambiado". Luego, se redujo la complejidad de los problemas anteriormente
 *      mencionados, solucionando problemas relacionados con los cuellos de botella en la parte gráfica y la
 *      parte de cálculo. En ambos casos, la clave se encuentra en  despreciar la masa de los asteroides en
 *      comparación con la del resto de los cuerpos celestes; decidimos que graficar los asteroides con un
 *      punto ("DrawPoint3D()") era suficiente, y que la interacción que pueden llegar a tener con el resto de
 *      los planetas y entre sí no es significante. Es decir, no se calculan las fuerzas gravitacionales de
 *      los asteroides entre asteroides.
 *
 * CITAS:
 *      -Ayudante Martín Zahnd:
 *          Ayuda/guía en la etapa de optimización del código para soportar más asteroides y con mayor
 *          rendimiento, haciéndonos concluir que las masas de los asteroides son insignificantes antes
 *          la de las estrellas y planetas.
 *          Los cambios realizados a partir de estos figuran líneas arriba, en "Sobre solución de lo anterior"
 *
 */

#include "orbitalSim.h"
#include "ephemerides.h"
#include <stdlib.h>
#include <string.h>

#define GRAVITATIONAL_CONSTANT 6.6743E-11F
#define ASTEROIDS_MEAN_RADIUS 4E11F

/**
 * @brief Get a random flot between min and max
 *
 * @param min
 * @param max
 * @return float
 */
float getRandomFloat(float min, float max);

/**
 * @brief Get a random unsigned char between min and max
 *
 * @param min
 * @param max
 * @return unsigned char
 */
unsigned char getRandomUChar(unsigned char min, unsigned max);

/**
 * @brief Place an asteroid in the planetary system simulation
 *
 * @param body Pointer to a OrbitalBody structure to store asteroid data
 * @param centerMass Mass of the most massive object in the planetary system
 */
void placeAsteroid(OrbitalBody *body, float centerMass);

OrbitalSim *makeOrbitalSim(float timeStep)
{
    int i;
    int systemBodyNumCore, systemBodyNum;

    OrbitalSim *tempOrbitalSim = NULL;
    OrbitalBody **bodies = NULL;
    EphemeridesBody *systemInfo;

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

    /*********BLACK_HOLE*********/
    if (BLACK_HOLE)
    {
        systemBodyNumCore++;
        systemBodyNum++;
    }

    // Template de black hole, con posicion y velocidad iniciales totalmente empíricos
    const OrbitalBody blacky = {Vector3Subtract(solarSystem[3].position, solarSystem[6].position),
                                Vector3Scale(solarSystem[5].velocity, -1),
                                Vector3Zero(),
                                systemInfo[0].mass * BLACK_HOLE_MASS_FACTOR,
                                systemInfo[0].radius,
                                DARKGRAY};
    /*********BLACK_HOLE*********/

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
            for (j = 0; j < i - 1; j++)
                free(bodies[j]);

            free(bodies);
            free(tempOrbitalSim);

            return NULL;
        }

        if (BLACK_HOLE && (i == systemBodyNumCore - 1))
        {
            *(bodies[i]) = blacky;
        }

        // Cuerpos principales del sistema (no asteroides)
        else if (i < systemBodyNumCore)
        {
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

    for (i = 0; i < sim->bodyNum; i++)
    {
        sim->bodies[i]->acceleration = Vector3Zero();
    }

    for (i = 0; i < sim->bodyNumCore; i++)
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

float getRandomFloat(float min, float max)
{
    return min + (max - min) * rand() / (float)RAND_MAX;
}

unsigned char getRandomUChar(unsigned char min, unsigned max)
{
    return min + (max - min) * rand() / (unsigned char)RAND_MAX;
}

void placeAsteroid(OrbitalBody *body, float centerMass)
{
    // Logit distribution
    float x = getRandomFloat(0, 1);
    float l = logf(x) - logf(1 - x) + 1;

    // https://mathworld.wolfram.com/DiskPointPicking.html
    float r = ASTEROIDS_MEAN_RADIUS * sqrtf(fabs(l));
    float phi = getRandomFloat(0, 2 * 3.14) * !EASTER_EGG;

    // Surprise!
    // phi = 0;

    // https://en.wikipedia.org/wiki/Circular_orbit#Velocity
    float v = sqrtf(GRAVITATIONAL_CONSTANT * centerMass / r) * getRandomFloat(0.6F, 1.2F);
    float vy = getRandomFloat(-1E2F, 1E2F);

    body->mass = 1E12F;  // Typical asteroid weight: 1 billion tons
    body->radius = 2E3F; // Typical asteroid radius: 2km
    body->position = {r * cosf(phi), 0, r * sinf(phi)};

    if (PARTY_TIME)
        body->color = {getRandomUChar(0, 255), getRandomUChar(0, 255), getRandomUChar(0, 255), 126};

    else
        body->color = GRAY;

    body->velocity = {-v * sinf(phi), vy, v * cosf(phi)};
}

/**
 * @file OrbitalSim.cpp
 * @authors Alejandro Heir, Matías Álvarez
 * @brief Clase OrbitalSim, para crear una simulación de sistema planetario.
 * @version 0.1
 * @date 2022-03-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <string.h>
#include "OrbitalSim.h"
#include "ephemerides.h"

#define GRAVITATIONAL_CONSTANT 6.6743E-11F
#define ASTEROIDS_MEAN_RADIUS 4E11F

/* Internal function prototypes */
float getRandomFloat(float min, float max);
unsigned char getRandomUChar(unsigned char min, unsigned max);
const char *getISODate(float currentTime);
/* Internal function prototypes */

OrbitalSim::OrbitalSim(int daysPerSec,
                       int system,
                       unsigned int asteroidsNum,
                       bool setBlackHole,
                       bool tweakJupiterMass,
                       bool partyTime,
                       bool easterEgg)
{
    // time stuff
    const float secondsPerDay = 86400.0F;
    const int daysPerSecond = daysPerSec;
    const float fps = 60.0F;                              // frames per second
    this->timeMultiplier = daysPerSecond * secondsPerDay; // Simulation speed: days per real second
    const float timeStep = timeMultiplier / fps;

    const int jupiterID = 5;
    const int tweakJupiterMassFactor = 1000;
    const int blackHoleMassFactor = 100000;

    this->partyTime = partyTime;
    this->system = system;
    this->tweakJupiterMass = tweakJupiterMass;
    this->asteroidsNum = asteroidsNum;
    this->timeStep = timeStep;
    this->blackHole = setBlackHole;
    this->easterEgg = easterEgg;

    EphemeridesBody *systemInfo = NULL;

    switch (system)
    {
    case SOLAR:
        bodyNumCore = SOLARSYSTEM_BODYNUM;
        systemInfo = solarSystem;

        if (tweakJupiterMass)
            systemInfo[jupiterID].mass *= tweakJupiterMassFactor;

        break;

    case ALPHACENTAURI:
        bodyNumCore = ALPHACENTAURISYSTEM_BODYNUM;
        systemInfo = alphaCentauriSystem;

        break;

    default:
        system = SOLAR;

        bodyNumCore = SOLARSYSTEM_BODYNUM;
        systemInfo = solarSystem;

        if (tweakJupiterMass)
            systemInfo[jupiterID].mass *= tweakJupiterMassFactor;

        break;
    }

    bodyNum = bodyNumCore + asteroidsNum;

    /*********BLACK_HOLE*********/
    if (blackHole)
    {
        bodyNum++;
        bodyNumCore++;
    }

    OrbitalBody blacky;

    blacky.setPosition(Vector3Subtract(solarSystem[3].position, solarSystem[6].position));
    blacky.setAcceleration(Vector3Zero());
    blacky.setMass(systemInfo[0].mass * blackHoleMassFactor);
    blacky.setRadius(systemInfo[0].radius);
    blacky.setColor(DARKGRAY);
    /*********BLACK_HOLE*********/

    bodies = new OrbitalBody[bodyNum];

    for (int i = 0; i < bodyNum; i++)
    {
        if (blackHole && (i == (bodyNumCore - 1)))
        {
            bodies[i++] = blacky;
        }

        else if (i < bodyNumCore)
        {
            bodies[i].setPosition(systemInfo[i].position);
            bodies[i].setVelocity(systemInfo[i].velocity);
            bodies[i].setMass(systemInfo[i].mass);
            bodies[i].setRadius(systemInfo[i].radius);
            bodies[i].setColor(systemInfo[i].color);
            bodies[i].setAcceleration(Vector3Zero());
        }

        else
        {
            placeAsteroid(bodies[i], systemInfo[0].mass);
        }
    }
}

OrbitalSim::~OrbitalSim()
{
    delete[] bodies;
}

void OrbitalSim::render3D()
{
    for (int i = 0; i < bodyNum; i++)
    {

        Vector3 position = Vector3Scale(bodies[i].getPosition(), 1E-11F);
        float radius = logf(bodies[i].getRadius()) * 0.005F;
        Color color = bodies[i].getColor();

        if (i < bodyNumCore)
        {
            DrawSphere(position,
                       radius,
                       color);
        }

        DrawPoint3D(position, color);
    }
}

void OrbitalSim::render2D()
{
    DrawFPS(0, 0);

    raylib::DrawText(getISODate(time), 0, 25, 14, GOLD);

    raylib::DrawText("Planetary system: ", 0, 45, 14, GOLD);
    switch (system)
    {
    case SOLAR:
        raylib::DrawText("Solar", 0, 60, 14, GOLD);
        break;

    case ALPHACENTAURI:
        raylib::DrawText("Alphacentauri", 0, 60, 14, GOLD);
        break;

    default:
        break;
    }

    raylib::DrawText("Planetary system bodies: ", 0, 80, 14, GOLD);
    raylib::DrawText(std::to_string(bodyNumCore), 0, 95, 14, GOLD);

    raylib::DrawText("Asteroids: ", 0, 115, 14, GOLD);
    raylib::DrawText(std::to_string(asteroidsNum), 0, 130, 14, GOLD);

    if (blackHole)
    {
        raylib::DrawText("Black hole ON", 0, 150, 14, GREEN);
    }
    else
    {
        raylib::DrawText("Black hole OFF", 0, 150, 14, GREEN);
    }

    if (tweakJupiterMass)
    {
        raylib::DrawText("Jupiter mass tweak ON", 0, 170, 14, GREEN);
    }
    else
    {
        raylib::DrawText("Jupiter mass tweak OFF", 0, 170, 14, GREEN);
    }
}

void OrbitalSim::placeAsteroid(OrbitalBody &body, float centerMass)
{
    // Logit distribution
    float x = getRandomFloat(0, 1);
    float l = logf(x) - logf(1 - x) + 1;

    // https://mathworld.wolfram.com/DiskPointPicking.html
    float r = ASTEROIDS_MEAN_RADIUS * sqrtf(fabs(l));
    float phi = getRandomFloat(0, 2 * 3.14) * (!easterEgg);

    // Surprise!
    //
    // already implemented; see easterEgg variable above
    //
    // phi = 0

    // https://en.wikipedia.org/wiki/Circular_orbit#Velocity
    float v = sqrtf(GRAVITATIONAL_CONSTANT * centerMass / r) * getRandomFloat(0.6F, 1.2F);
    float vy = getRandomFloat(-1E2F, 1E2F);

    // Fill in with your own fields:
    body.setMass(1E12F);  // Typical asteroid weight: 1 billion tons
    body.setRadius(2E3F); // Typical asteroid radius: 2km
    body.setPosition({r * cosf(phi), 0, r * sinf(phi)});

    if (partyTime)
        body.setColor({getRandomUChar(0, 255), getRandomUChar(0, 255), getRandomUChar(0, 255), 126});

    else
        body.setColor(GRAY);

    body.setVelocity({-v * sinf(phi), vy, v * cosf(phi)});
}

void OrbitalSim::update(float referenceFps)
{
    // Se hacen coincidir FPS de raylib con los de la cuenta de timeStep para que
    // este ultimo siempre "esté bien", sin importar los fps de raylib.
    // En un principio se ponía un tope a los fps con SetTargetFPS, pero si eran
    // menores al seteado, la progreción temporal quedaba mal.
    //
    // Luego, se consigue NO limitar los fps (que sean los máximos que da raylib), y que
    // el timeStep siempre sea correcto.
    //
    // No se notaron impactos en el rendimiento al agregar una división en cada vuelta del loop
    //
    // !!!
    // Notar que referenceFps viene preinicializado, por lo que no es necesario pasar algo como argumento
    // !!!
    if (referenceFps > 5)
    {
        timeStep = timeMultiplier / referenceFps;
    }

    time += timeStep;

    int i;

    for (i = 0; i < bodyNum; i++)
    {
        bodies[i].setAcceleration(Vector3Zero());
    }

    for (i = 0; i < bodyNumCore; i++)
    {
        for (int j = i + 1; j < bodyNum; j++)
        {
            // Parte vectorial
            raylib::Vector3 vectorDiff = Vector3Subtract(bodies[i].getPosition(), bodies[j].getPosition());

            // Norma de distancia
            float vectorLen = Vector3Length(vectorDiff);

            // C�lculo sin factor de masa
            raylib::Vector3 partialAcceleration = Vector3Scale(vectorDiff,
                                                               (-1.0F) * GRAVITATIONAL_CONSTANT / (vectorLen * vectorLen));

            // Aceleraci�n de i a causa de j
            bodies[i].setAcceleration(Vector3Add(bodies[i].getAcceleration(),
                                                 Vector3Scale(partialAcceleration,
                                                              bodies[j].getMass() / vectorLen)));

            // Aceleraci�n de j a causa de i
            bodies[j].setAcceleration(Vector3Add(bodies[j].getAcceleration(),
                                                 Vector3Scale(partialAcceleration,
                                                              (-1.0F) * bodies[i].getMass() / vectorLen)));
        }
    }

    for (i = 0; i < bodyNum; i++)
    {
        raylib::Vector3 velocity = Vector3Add(bodies[i].getVelocity(),
                                              Vector3Scale(bodies[i].getAcceleration(),
                                                           timeStep));

        bodies[i].setVelocity(velocity);

        bodies[i].setPosition(Vector3Add(bodies[i].getPosition(),
                                         Vector3Scale(velocity,
                                                      timeStep)));
    }
}

float OrbitalSim::getTime()
{
    return time;
}

int OrbitalSim::getBodyNumCore()
{
    return bodyNumCore;
}

int OrbitalSim::getBodyNum()
{
    return bodyNum;
}

float getRandomFloat(float min, float max)
{
    return min + (max - min) * rand() / (float)RAND_MAX;
}

unsigned char getRandomUChar(unsigned char min, unsigned max)
{
    return min + (max - min) * rand() / (unsigned char)RAND_MAX;
}

const char *getISODate(float currentTime)
{
    // Epoch: 2022-01-01
    struct tm epoch_tm = {0, 0, 0, 1, 0, 122};
    time_t epoch = mktime(&epoch_tm);

    // Convert epoch time to local time
    time_t local_time = epoch + (time_t)currentTime;

    // Print ISO date for local time
    struct tm *local_tm = localtime(&local_time);
    return TextFormat("Date: %04d-%02d-%02d", 1900 + local_tm->tm_year, local_tm->tm_mon + 1, local_tm->tm_mday);
}

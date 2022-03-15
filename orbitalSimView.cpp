/**
 * @file orbitalSimView.cpp
 * @authors Marc Ressl - Alejandro Heir, Matías Álvarez
 * @brief Orbital simulation view. Módulo gráfico
 * @version 0.1
 * @date 2022-03-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <time.h>
#include <stdio.h>

#include "orbitalSimView.h"

const char *getISODate(float currentTime);
void renderOrbitalSim3D(OrbitalSim *sim);
void renderOrbitalSim2D(OrbitalSim *sim);

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

void renderOrbitalSim3D(OrbitalSim *sim)
{
    int i;

    for (i = 0; i < sim->bodyNum; i++)
    {

        Vector3 position = Vector3Scale(sim->bodies[i]->position, 1E-11F);
        float radius = logf(sim->bodies[i]->radius) * 0.005F;
        Color color = sim->bodies[i]->color;

        // Solo se dibujan esferas para los cuerpos principales del sistema, pues
        // los asteriodes tienen masa insignificante frente a los cuerpos del sistema.
        //
        // Referencias:
        //     -Ayudante Martín Zahnd

        if (i < sim->bodyNumCore)
        {
            DrawSphere(position,
                       radius,
                       color);
        }

        DrawPoint3D(position, color);
    }
}

void renderOrbitalSim2D(OrbitalSim *sim)
{
    static int asteroidNum = sim->bodyNum - sim->bodyNumCore;
    static int coreNum = sim->bodyNumCore;

    char auxiliarString[6];

    DrawFPS(0, 0);

    DrawText(getISODate(sim->time), 0, 25, 14, GOLD);

    DrawText("Planetary system: ", 0, 45, 14, GOLD);

    DrawText("Planetary system bodies: ", 0, 65, 14, GOLD);
    sprintf(auxiliarString, "%d", coreNum);
    DrawText(auxiliarString, 0, 80, 14, GOLD);

    DrawText("Asteroids: ", 0, 100, 14, GOLD);
    sprintf(auxiliarString, "%d", asteroidNum);
    DrawText(auxiliarString, 0, 115, 14, GOLD);
    
}

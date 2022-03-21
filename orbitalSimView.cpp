/**
 * @file orbitalSimView.cpp
 * @authors Marc Ressl - Alejandro Heir, Matías Álvarez
 * @brief Orbital simulation view. Módulo gráfico
 * @version 0.1
 * @date 2022-03-09
 *
 * @copyright Copyright (c) 2022
 *
 * CITAS:
 *      -Ayudante Martín Zahnd:
 *         (ver encabezado de orbitalSim.cpp)
 *
 */

#include <time.h>
#include <stdio.h>

#include "orbitalSimView.h"

// Agregado para conocer la ARCHITECT'S CONSOLE, para mostrar datos en pantalla
#include "orbitalSim.h"

/**
 * @brief Dado tiempo en segundos, devuelve string con tiempo en formato ISO 8601
 *
 * @param currentTime
 * @return const char*
 */
const char *getISODate(float currentTime);

void renderOrbitalSim3D(OrbitalSim *sim)
{
    int i;

    for (i = 0; i < sim->bodyNum; i++)
    {

        Vector3 position = Vector3Scale(sim->bodies[i]->position, 1E-11F);
        float radius = logf(sim->bodies[i]->radius) * 0.005F;
        Color color = sim->bodies[i]->color;

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
    static int coreNum = sim->bodyNumCore;

    char auxiliarString[6];

    DrawFPS(0, 0);

    DrawText(getISODate(sim->time), 0, 25, 14, GOLD);

    DrawText("Planetary system: ", 0, 45, 14, GOLD);
    switch (CHOSEN_SYSTEM)
    {
    case SOLAR:
        DrawText("Solar", 0, 60, 14, GOLD);
        break;

    case ALPHACENTAURI:
        DrawText("Alphacentauri", 0, 60, 14, GOLD);
        break;

    default:
        break;
    }

    DrawText("Planetary system bodies: ", 0, 80, 14, GOLD);
    sprintf(auxiliarString, "%d", coreNum);
    DrawText(auxiliarString, 0, 95, 14, GOLD);

    DrawText("Asteroids: ", 0, 115, 14, GOLD);
    sprintf(auxiliarString, "%d", ASTEROIDS_NUM);
    DrawText(auxiliarString, 0, 130, 14, GOLD);

    if (BLACK_HOLE)
    {
        DrawText("Black hole ON", 0, 150, 14, GOLD);
    }
    else
    {
        DrawText("Black hole OFF", 0, 150, 14, GOLD);
    }

    if (TWEAK_JUPITER_MASS)
    {
        DrawText("Jupiter mass tweak ON", 0, 170, 14, GOLD);
    }
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

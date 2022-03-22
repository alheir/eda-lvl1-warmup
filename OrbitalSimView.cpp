/**
 * @file OrbitalSimView.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-03-21
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "OrbitalSimView.h"

/**
 * @brief Dado tiempo en segundos, devuelve string con tiempo en formato ISO 8601
 *
 * @param currentTime
 * @return const char*
 */
const char *getISODate(float currentTime);

void OrbitalSim::render3D()
{
}

/**
 * @brief Renders simulation data
 *
 * @param sim
 */
void OrbitalSimView::render2D(const OrbitalSim &sim)
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

/**
 * @brief Renders the 3D views of a given orbital simulation
 *
 * @param sim
 */
void OrbitalSimView::render3D(const OrbitalSim &sim)
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
